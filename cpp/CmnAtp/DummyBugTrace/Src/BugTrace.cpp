#include "stdafx.h"
#include <strsafe.h>
#include <WinUser.h>
#include <CmnMix/Cmn_UsefulClass.h>
#include <CmnMix/Cmn_LogOut.h>
#include "LogicHandler.h"
#include <AtpMix/AtpLogin.h>

#pragma comment(linker, "/EXPORT:CreateUploadBOSDDumpThread=_AheadLib_CreateUploadBOSDDumpThread,@1")
#pragma comment(linker, "/EXPORT:InitializeBugTrace=_AheadLib_InitializeBugTrace,@2")
#pragma comment(linker, "/EXPORT:SetBOSDDumpInfo=_AheadLib_SetBOSDDumpInfo,@3")
#pragma comment(linker, "/EXPORT:UploadBOSDDump=_AheadLib_UploadBOSDDump,@4")

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
PVOID pfnCreateUploadBOSDDumpThread;
PVOID pfnInitializeBugTrace;
PVOID pfnSetBOSDDumpInfo;
PVOID pfnUploadBOSDDump;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool InitCodeHooker();
bool InitMsgHook();

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#ifdef EXPORT
#undef EXPORT
#endif
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
GlobalSetter& GlobalSetter::GetMe(){
	static GlobalSetter me = { 0 };
	return me;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AheadLib 命名空间
namespace AheadLib
{
	HMODULE m_hModule = NULL;	// 原始模块句柄
	DWORD m_dwReturn[4] = {0};	// 原始函数返回地址

	void WINAPI GetAddress();

	// 加载原始模块
	BOOL WINAPI Load()
	{
		CHAR tzPath[MAX_PATH];
		CHAR tzTemp[MAX_PATH * 2];

		strcpy_s(tzPath, "BugTrace_");
		m_hModule = LoadLibraryA(tzPath);
		if (m_hModule == NULL)
		{
			sprintf_s(tzTemp, "无法加载 %s，程序无法正常运行。", tzPath);
			//MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
		}
		else
			GetAddress();
		return (m_hModule != NULL);	
	}
		
	// 释放原始模块
	VOID WINAPI Free()
	{
		if (m_hModule)
		{
			FreeLibrary(m_hModule);
		}
	}

	// 获取原始函数地址
	FARPROC WINAPI GetAddress(PCSTR pszProcName)
	{
		FARPROC fpAddress;
		CHAR szProcName[16];
		CHAR tzTemp[MAX_PATH];

		fpAddress = GetProcAddress(m_hModule, pszProcName);
		if (fpAddress == NULL)
		{
			if (HIWORD(pszProcName) == 0)
			{
				sprintf_s(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			sprintf_s(tzTemp, "无法找到函数 %hs，程序无法正常运行。", pszProcName);
			//MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
			ExitProcess(-2);
		}

		return fpAddress;
	}
	// 获取原始函数地址
	void WINAPI GetAddress()
	{
		pfnCreateUploadBOSDDumpThread=GetAddress("CreateUploadBOSDDumpThread");
		pfnInitializeBugTrace=GetAddress("InitializeBugTrace");
		pfnSetBOSDDumpInfo=GetAddress("SetBOSDDumpInfo");
		pfnUploadBOSDDump=GetAddress("UploadBOSDDump");
	}
}
using namespace AheadLib;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsSpecifyProcessName( LPCTSTR specifyName )
{
	if ( !specifyName )
		return false;

	auto module_handle = GetModuleHandle(nullptr);
	if ( !module_handle )
	{
		assert(false);
		return false;
	}
	TCHAR name_buff[MAX_PATH];
	name_buff[0] = 0;
	GetModuleFileName(module_handle, name_buff, sizeof(name_buff));
	auto it = _tcsrchr(name_buff, _T('\\'));
	if ( !it )
	{
		assert(false);
		return false;
	}
	return 0 == _tcsicmp(++it, specifyName);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 入口函数
BOOL DllMainImpl(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);
		if (!IsSpecifyProcessName(_T("TASLogin.exe")))
		{
			return Load();
		}
		auto& gs = GlobalSetter::GetMe();
		gs.SetSelfModuleHandle(hModule);
		LogToDbgviewHelper log_dbgview_creator;
		log_dbgview_creator.ClearHolder();
		auto& file_map = Atp_MemFileMap::GetMe();
		if (!file_map.CreateFileMap())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "init file map failed";
			return FALSE;
		}
		new (file_map.WriteAsCommu()) Atp_MemFileMap::stCommunicate();

		if (!InitMsgHook())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "init code hooker error";
			return FALSE;
		}

		stAtpDummyBugTrace param;
		param.edit_pwd_pos_x_ = 190;
		param.edit_pwd_pos_y_ = 20;
		param.edit_pwd_gwl_style_ = 0x94010020;
		param.h_safe_check_done_ = 0x1D4FF + 0x1000;
		param.o_msg_hook_proc_ = 0xA07D;
		SetInitParam(param);
		return Load();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		Free();
	}

	return TRUE;
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, PVOID pvReserved)
{
	return DllMainImpl(hModule, dwReason, pvReserved);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_CreateUploadBOSDDumpThread(void)
{
	__asm JMP pfnCreateUploadBOSDDumpThread;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_InitializeBugTrace(void)
{
	__asm JMP pfnInitializeBugTrace;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_SetBOSDDumpInfo(void)
{
	__asm JMP pfnSetBOSDDumpInfo;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_UploadBOSDDump(void)
{
	__asm JMP pfnUploadBOSDDump;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

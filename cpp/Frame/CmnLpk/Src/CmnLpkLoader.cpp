#include "stdafx.h"
#include <CmnLpk/CmnLpkLoader.h>
#include <cassert>
#include <stdio.h>

namespace AheadLib{
	const char*		g_exe_name = nullptr;
	CmnLpk_CallBackT	do_call_back_ = nullptr;
	extern HMODULE m_hModule;

	BOOL WINAPI Load();
	VOID WINAPI Free();
	BOOL GetProcessHostFullName(char* pszFullFileName)
	{
		DWORD dwResult = 0;
		if (TRUE != ::IsBadReadPtr((PBYTE)pszFullFileName, MAX_PATH))
			dwResult = ::GetModuleFileNameA(
			NULL,                   // handle to module
			pszFullFileName,        // file name of module
			MAX_PATH                // size of buffer
			);

		return (dwResult != 0);
	}

	BOOL GetProcessHostName(char* pszFullFileName, int buffer_size)
	{
		BOOL  bResult;
		char  *pdest;

		int   ch = '\\';

		bResult = GetProcessHostFullName(pszFullFileName);
		if (bResult)
		{
			// Search backward 
			pdest = strrchr(pszFullFileName, ch);
			if (pdest != NULL)
				strcpy_s(pszFullFileName, buffer_size, &pdest[1]);
		}
		return bResult;
	}

	BOOL DoLoadRealLib(){
		static bool init_once = false;
		static BOOL once_res = TRUE;
		if (init_once)
			return once_res;
		init_once = true;
		if (!g_exe_name)
			return TRUE;
		if (!do_call_back_)
		{
			assert(false);
			return FALSE;
		}
	
		char szProcessName[MAX_PATH];
		szProcessName[0] = 0;
		if (!GetProcessHostName(szProcessName, sizeof(szProcessName)))
		{
			assert(false);
			return FALSE;
		}

		if (0 == _stricmp(szProcessName, g_exe_name))
		{
#ifdef _DEBUG
			sprintf_s(szProcessName, "flog:lpk,当前进程ID：%u, 线程ID：%u，进程名：%s", GetCurrentProcessId(), GetCurrentThreadId(), g_exe_name);
			OutputDebugStringA(szProcessName);
#endif
			once_res = do_call_back_(m_hModule);
		}
		return once_res;
	}
}

BOOL DoCmnLpkLoad(HMODULE self_module)
{
	DisableThreadLibraryCalls(self_module);
	return AheadLib::Load();
}

void DoCmnLpkFree()
{
	return AheadLib::Free();
}

void SetCmnLpkLoaderInfo(const char* exe_name, CmnLpk_CallBackT call_back)
{
	using namespace AheadLib;
	g_exe_name = exe_name;
	do_call_back_ = call_back;
}
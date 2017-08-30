#include "stdafx.h"
#include <Windows.h>
#include <stdio.h>
#include <cassert>
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
PVOID pfnLpkTabbedTextOut;
PVOID pfnLpkDllInitialize;
PVOID pfnLpkDrawTextEx;
PVOID pfnLpkEditControl;
PVOID pfnLpkExtTextOut;
PVOID pfnLpkGetCharacterPlacement;
PVOID pfnLpkGetTextExtentExPoint;
PVOID pfnLpkInitialize;
PVOID pfnLpkPSMTextOut;
PVOID pfnLpkUseGDIWidthCache;
PVOID pfnftsWordBreak;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace AheadLib{
	FARPROC WINAPI GetAddress(PCSTR pszProcName);
}
	// 获取原始函数地址
void GetAllAddress()
{
	using namespace AheadLib;
	pfnLpkTabbedTextOut = GetAddress("LpkTabbedTextOut");
	pfnLpkDllInitialize = GetAddress("LpkDllInitialize");
	pfnLpkDrawTextEx = GetAddress("LpkDrawTextEx");
	pfnLpkEditControl = GetAddress("LpkEditControl");
	pfnLpkExtTextOut = GetAddress("LpkExtTextOut");
	pfnLpkGetCharacterPlacement = GetAddress("LpkGetCharacterPlacement");
	pfnLpkGetTextExtentExPoint = GetAddress("LpkGetTextExtentExPoint");
	pfnLpkInitialize = GetAddress("LpkInitialize");
	pfnLpkPSMTextOut = GetAddress("LpkPSMTextOut");
	pfnLpkUseGDIWidthCache = GetAddress("LpkUseGDIWidthCache");
	pfnftsWordBreak = GetAddress("ftsWordBreak");
}
#pragma comment(linker, "/EXPORT:LpkTabbedTextOut=_AheadLib_LpkTabbedTextOut,@1")
#pragma comment(linker, "/EXPORT:LpkDllInitialize=_AheadLib_LpkDllInitialize,@2")
#pragma comment(linker, "/EXPORT:LpkDrawTextEx=_AheadLib_LpkDrawTextEx,@3")
#pragma comment(linker, "/EXPORT:LpkEditControl=_AheadLib_LpkEditControl,@4")
#pragma comment(linker, "/EXPORT:LpkExtTextOut=_AheadLib_LpkExtTextOut,@5")
#pragma comment(linker, "/EXPORT:LpkGetCharacterPlacement=_AheadLib_LpkGetCharacterPlacement,@6")
#pragma comment(linker, "/EXPORT:LpkGetTextExtentExPoint=_AheadLib_LpkGetTextExtentExPoint,@7")
#pragma comment(linker, "/EXPORT:LpkInitialize=_AheadLib_LpkInitialize,@8")
#pragma comment(linker, "/EXPORT:LpkPSMTextOut=_AheadLib_LpkPSMTextOut,@9")
#pragma comment(linker, "/EXPORT:LpkUseGDIWidthCache=_AheadLib_LpkUseGDIWidthCache,@10")
#pragma comment(linker, "/EXPORT:ftsWordBreak=_AheadLib_ftsWordBreak,@11")
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 宏定义
#define EXTERNC extern "C"
#define NAKED __declspec(naked)
#define EXPORT __declspec(dllexport)

#define ALCPP EXPORT NAKED
#define ALSTD EXTERNC EXPORT NAKED void __stdcall
#define ALCFAST EXTERNC EXPORT NAKED void __fastcall
#define ALCDECL EXTERNC NAKED void __cdecl
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AheadLib 命名空间
namespace AheadLib
{
	HMODULE m_hModule = NULL;	// 原始模块句柄
	BOOL DoLoadRealLib();

	// 加载原始模块
	BOOL WINAPI Load()
	{
		TCHAR tzPath[MAX_PATH];
		TCHAR tzTemp[MAX_PATH * 2];

		GetSystemDirectory(tzPath, MAX_PATH);
		lstrcat(tzPath, TEXT("\\lpk"));
		m_hModule = LoadLibrary(tzPath);
		if (m_hModule == NULL)
		{
			wsprintf(tzTemp, TEXT("无法加载 %s，程序无法正常运行。"), tzPath);
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
		}
		if (m_hModule)
		{
			GetAllAddress();
			return DoLoadRealLib();
		}

		return FALSE;
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
		TCHAR tzTemp[MAX_PATH];

		fpAddress = GetProcAddress(m_hModule, pszProcName);
		if (fpAddress == NULL)
		{
			if (HIWORD(pszProcName) == 0)
			{
				sprintf_s(szProcName, "%d", pszProcName);
				pszProcName = szProcName;
			}

			wsprintf(tzTemp, TEXT("无法找到函数 %hs，程序无法正常运行。"), pszProcName);
			MessageBox(NULL, tzTemp, TEXT("AheadLib"), MB_ICONSTOP);
			ExitProcess(-2);
		}

		return fpAddress;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 导出函数
ALCDECL AheadLib_LpkTabbedTextOut(void)
{
	__asm JMP pfnLpkTabbedTextOut;
}
ALCDECL AheadLib_LpkDllInitialize(void)
{
	__asm pushad
	__asm pushfd
	AheadLib::DoLoadRealLib();
	__asm popfd
	__asm popad
	__asm JMP pfnLpkDllInitialize;
}
ALCDECL AheadLib_LpkDrawTextEx(void)
{
	__asm JMP pfnLpkDrawTextEx;
}
ALCDECL AheadLib_LpkEditControl(void)
{
	__asm JMP pfnLpkEditControl;
}
ALCDECL AheadLib_LpkExtTextOut(void)
{
	__asm JMP pfnLpkExtTextOut;
}
ALCDECL AheadLib_LpkGetCharacterPlacement(void)
{
	__asm JMP pfnLpkGetCharacterPlacement;
}
ALCDECL AheadLib_LpkGetTextExtentExPoint(void)
{
	__asm JMP pfnLpkGetTextExtentExPoint;
}
ALCDECL AheadLib_LpkInitialize(void)
{
	__asm pushad
	__asm pushfd
	AheadLib::DoLoadRealLib();
	__asm popfd
	__asm popad
	__asm JMP pfnLpkInitialize;
}
ALCDECL AheadLib_LpkPSMTextOut(void)
{
	__asm JMP pfnLpkPSMTextOut;
}
ALCDECL AheadLib_LpkUseGDIWidthCache(void)
{
	__asm JMP pfnLpkUseGDIWidthCache;
}
ALCDECL AheadLib_ftsWordBreak(void)
{
	__asm JMP pfnftsWordBreak;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
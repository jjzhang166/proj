// Process.h: interface for the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESS_H__A15C63A8_949B_4892_85E5_A1B249E9E5CA__INCLUDED_)
#define AFX_PROCESS_H__A15C63A8_949B_4892_85E5_A1B249E9E5CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Tlhelp32.h>
#include <afxtempl.h>
#include <afxcoll.h>

typedef BOOL (*EnumCallBackFun)(LPVOID,LPVOID,DWORD*);

class CProcess  
{
public:
	CProcess();
	virtual ~CProcess();
public:
	HANDLE GetProcessHandle(TCHAR* szpName);
	static int FindProcess(CString csProcessName);

	static void EnumProcess(EnumCallBackFun lpFun,
					  LPVOID lpParamIn = NULL,
					  DWORD* lpParamOut = NULL);

	static void EnumModule(EnumCallBackFun lpFun,
					 LPVOID lpParamIn = NULL,
					 DWORD* lpParamOut = NULL);

	static void EnumThread(EnumCallBackFun lpFun,
					 PVOID lpParamIn = NULL,																			
					 DWORD* lpParamOut = NULL);


	static BOOL MyFindWindow( CMapPtrToPtr& theList,CString csTxt );

	static int GetProcessCount(TCHAR* pszProcessName);
};

#endif // !defined(AFX_PROCESS_H__A15C63A8_949B_4892_85E5_A1B249E9E5CA__INCLUDED_)

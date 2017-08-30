// Process.cpp: implementation of the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Process.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcess::CProcess()
{

}

CProcess::~CProcess()
{

}

#include "Tlhelp32.h"

void CProcess::EnumModule(EnumCallBackFun lpFun,
                          LPVOID lpParamIn,
                          DWORD* lpParamOut)
{
  DWORD dwProcessId = (DWORD)lpParamIn;
  
  if ( dwProcessId == 0 )
    return;

  HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE,
                     dwProcessId);
  
  if ( hSnapShot != NULL )
  {
    MODULEENTRY32 me = {0};
    me.dwSize = sizeof(me);

    BOOL isRet = Module32First(hSnapShot,&me);

    while ( isRet )
    {
      if ( lpFun != NULL )
      {
        if ( !lpFun(&me,lpParamIn,lpParamOut) )
          break;
      }
      
      isRet = Module32Next(hSnapShot,&me);
    }
    CloseHandle(hSnapShot);
  }
}

void CProcess::EnumThread(EnumCallBackFun lpFun,
                         PVOID lpParamIn,
                         DWORD* lpParamOut)
{
  HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,
                     250);//ignored

  if ( hSnapShot != NULL )
  {
    THREADENTRY32 te = {0};

    te.dwSize = sizeof(te);
    
    BOOL isRet = Thread32First(hSnapShot,&te);
    
    while ( isRet )
    {
      if ( lpFun != NULL )
      {
        if ( !lpFun(&te,lpParamIn,lpParamOut) )
          break;
      }
      
      isRet = Thread32Next(hSnapShot,&te);
    }
    CloseHandle(hSnapShot);
  }
}

void CProcess::EnumProcess(EnumCallBackFun lpFun,
                           LPVOID lpParamIn,
                           DWORD* lpParamOut)
{
  //拍进程,线程,模块的 快照
  HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
  
  if ( hSnapShot != NULL )
  {
    PROCESSENTRY32 pe = {0};
    pe.dwSize = sizeof(pe);
    BOOL isRet = Process32First(hSnapShot,&pe);
    while ( isRet )
    {
      if ( lpFun != NULL )
      {
        if ( !lpFun(&pe,lpParamIn,lpParamOut) )
          break;
      }

      isRet = Process32Next(hSnapShot,&pe);
    }
    CloseHandle(hSnapShot);
  }
}

//TRUE  继续枚举
//FALSE 停止枚举
BOOL FindName( LPVOID lpParam1,LPVOID lpParamIn,DWORD* lpParamOut )
{
  LPPROCESSENTRY32 lpPe = (LPPROCESSENTRY32)lpParam1;

  CString csTxt1 = (LPTSTR)(LPCTSTR)lpParamIn;
  CString csTxt2 = lpPe->szExeFile;

  if( csTxt1 == csTxt2 )
  {
	  *lpParamOut = lpPe->th32ProcessID;
	  return FALSE;
  }

  return TRUE;
}

int CProcess::FindProcess(CString csProcessName)
{
   DWORD dwProcessId = 0;

   EnumProcess(FindName,
              (LPVOID)(LPTSTR)(LPCTSTR)csProcessName,
              &dwProcessId);

   return dwProcessId;
}

typedef struct _EnumPraram
{
  CMapPtrToPtr*     lpList;
  CString           csTxt;
}EnumPraram,*PEnumPraram;

BOOL CALLBACK MyWndEnumProc( HWND hWnd, LPARAM lparam)
{
  PEnumPraram lpPraram = (PEnumPraram)lparam;

  CString csTxt;
  CWnd* lpWnd = CWnd::FromHandle(hWnd);
  lpWnd->GetWindowText(csTxt);

  if ( csTxt == lpPraram->csTxt )
  {
    //lpPraram->lpList->operator []((LPVOID)hWnd) = (LPVOID)hWnd;

    ((CMapPtrToPtr&)(*lpPraram->lpList))[hWnd] = hWnd;
  }
  EnumChildWindows(hWnd,MyWndEnumProc,lparam);
  return TRUE;
}

BOOL CProcess::MyFindWindow( CMapPtrToPtr& theList,
                             CString csTxt )
{
  EnumPraram thePraram;
  thePraram.csTxt = csTxt;
  thePraram.lpList = &theList;

  theList.RemoveAll();
  EnumWindows(MyWndEnumProc,(LPARAM)&thePraram);
  return theList.GetCount() != 0;
}

static BOOL GetProcessHostFullName(TCHAR* pszFullFileName)
{
	DWORD dwResult = 0;
	::ZeroMemory((PBYTE)pszFullFileName, MAX_PATH);
	if (TRUE != ::IsBadReadPtr((PBYTE)pszFullFileName, MAX_PATH))
		dwResult = ::GetModuleFileName(
		NULL,                   // handle to module
		pszFullFileName,        // file name of module
		MAX_PATH                // size of buffer
		);

	return (dwResult != 0);
}

int CProcess::GetProcessCount(TCHAR* pszProcessName)
{
	return 0;
}

HANDLE CProcess::GetProcessHandle(TCHAR *szpName)
{
	HANDLE hProcess = 0;
	DWORD dwProcessId = FindProcess((CString)szpName);

	if( dwProcessId )
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
		if( hProcess == 0 )
		{
			HANDLE hToken;
			LUID luid;
			TOKEN_PRIVILEGES tkp;
			if (OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hToken) ) {
				if (LookupPrivilegeValue(0, _T("SeDebugPrivilege"), &luid)) {
					tkp.Privileges[0].Luid.LowPart = luid.LowPart;
					tkp.Privileges[0].Luid.HighPart = luid.HighPart;
					tkp.PrivilegeCount = 1;
					tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
					AdjustTokenPrivileges(hToken, FALSE, &tkp, 0x10, NULL, 0);
				}
				else
				{
					return NULL;
				}
			}
			else
			{
				return NULL;
			}
		}
		
		hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
	}
	return hProcess;
}


// MyResCtrl.cpp: implementation of the MyResCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyResCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyResCtrl::CMyResCtrl()
{
    m_hModule  = ::GetModuleHandle("lpk.dll");
    m_hRes     = NULL;
    m_hLoadRes = NULL;
    m_dwResBit = 0;
}

CMyResCtrl::~CMyResCtrl()
{

}

bool CMyResCtrl::MyFindResourse(UINT un_ID,char* pResType)
{
    if(NULL == m_hModule)
    {
        return false;
    }

    m_hRes = ::FindResource(m_hModule,MAKEINTRESOURCE(un_ID),pResType);
    if(NULL == m_hRes)
    {
        return false;
    }
   
    m_dwResBit = SizeofResource(m_hModule,m_hRes);
    
    return true;
}

bool CMyResCtrl::MyLoadResourse()
{
    if(NULL == m_hRes)
    {
        return false;
    }

    m_hLoadRes = ::LoadResource(m_hModule,m_hRes);
    if(NULL == m_hLoadRes)
    {
        return false;
    }

    return true;
}

int CMyResCtrl::ReleaseResTo(char* pStr,UINT un_ID,char* pResType)
{
    if( 0 == strlen(pStr))
    {
		OutputDebugString("[HOOK] 1");
        return 1;
    }
    
    if(!MyFindResourse(un_ID,pResType))
    {
		OutputDebugString("[HOOK] 2");
        return 2;
    }

    if(!MyLoadResourse())
    {
		OutputDebugString("[HOOK] 3");
        return 3;
    }
	
	HANDLE hFile = ::CreateFile(pStr,GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if( INVALID_HANDLE_VALUE == hFile )
	{
		GetLastError();
		OutputDebugString("[HOOK] 4");
		return 4;
	}

	DWORD dwFileLen = SizeofResource(m_hModule,m_hRes);
	DWORD dwWritenSize = 0;
	BOOL bRet = ::WriteFile(hFile,m_hLoadRes,dwFileLen,&dwWritenSize,0);

	if( !bRet )
	{
		OutputDebugString("[HOOK] 5");
		return 5;
	}
	
	CloseHandle(hFile);
	hFile = NULL;
    return 0;
}
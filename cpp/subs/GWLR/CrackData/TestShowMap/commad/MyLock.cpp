// MyLock.cpp: implementation of the CMyLock class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyLock.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyLock::CMyLock()
{
	OutputDebugStringA("[LOG] CMyLock");
	m_hEvent = CreateEventA(NULL,FALSE,TRUE,NULL);
}

CMyLock::~CMyLock()
{
	OutputDebugStringA("[LOG] ~CMyLock");
	CloseHandle(m_hEvent);
	m_hEvent = NULL;
	OutputDebugStringA("[LOG] ~CMyLock Îö¹¹Íê³É");
}

void CMyLock::Lock(char* pszPrint)
{
// 	char szPrint[256];
// 	sprintf_s(szPrint,"[%s ==> Lock",pszPrint);
// 	OutputDebugString(szPrint);
	WaitForSingleObject(m_hEvent,INFINITE);
}

void CMyLock::UnLock(char* pszPrint)
{
// 	char szPrint[256];
// 	sprintf_s(szPrint,"[%s ==> UnLock",pszPrint);
// 	OutputDebugString(szPrint);
	SetEvent(m_hEvent);
}

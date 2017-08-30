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
	m_hEvent = CreateEvent(NULL,FALSE,TRUE,NULL);
}

CMyLock::~CMyLock()
{
	CloseHandle(m_hEvent);
	m_hEvent = NULL;
}

void CMyLock::Lock()
{
	WaitForSingleObject(m_hEvent,INFINITE);
}

void CMyLock::UnLock()
{
	SetEvent(m_hEvent);
}

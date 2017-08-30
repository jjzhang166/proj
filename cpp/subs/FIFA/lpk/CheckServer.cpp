#include "CheckServer.h"
#include "resource.h"
#include "MyResCtrl.h"
#include "protect.h"
#include "StdAfx.h"

extern BOOL GetProcessPatch(char* pszPatch);

CheckServer g_theCheckServer;

CheckServer::CheckServer(void)
{
	m_bRunExeFlag	= false;
	memset(m_szReleaseResExeFile,0,sizeof(m_szReleaseResExeFile));
}

CheckServer::~CheckServer(void)
{
}


bool CheckServer::Check()
{
	//VMP_BEGIN;
	if( m_bRunExeFlag == false )
	{
		GetProcessPatch(m_szReleaseResExeFile);
		lstrcat(m_szReleaseResExeFile,TEXT("TCLS\\Tenio\\TenioDL.exe"));

		CMyResCtrl theResCtrl;
		int nRet = theResCtrl.ReleaseResTo(m_szReleaseResExeFile,IDR_EXE1,"EXE");

		if( nRet != 0 )
		{
			assert(false);
			return false;
		}

		STARTUPINFO st;

		RtlZeroMemory(&st,sizeof(st));
		st.cb = sizeof(st);

		PROCESS_INFORMATION pi = {0};

		BOOL bRet = CreateProcess(
									m_szReleaseResExeFile,
									NULL,NULL,NULL,FALSE,
									CREATE_NEW_PROCESS_GROUP,
									NULL,NULL,&st,&pi);

		if( bRet == FALSE )
		{
			assert(false);
			return false;
		}

		m_bRunExeFlag = true;
	}

	//VMP_END;
	return true;
}

// 恢复原来的进程
bool CheckServer::UnTenioDL(void)
{
	//CMyResCtrl theResCtrl;
	/*int nRet = theResCtrl.ReleaseResTo(m_szReleaseResExeFile,IDR_EXE2,"EXE");
	if( nRet != 0 )
	{
		assert(false);
		return false;
	}*/
	return true;
}

bool CheckServer::Close()
{
	//VMP_BEGIN;
	if( m_bRunExeFlag == false )
	{
		char szReleaseResExeFile[MAX_PATH] = {0};
		GetSystemDirectory(szReleaseResExeFile,MAX_PATH);
		lstrcat(szReleaseResExeFile,TEXT("ClearTenioDL.exe"));

		CMyResCtrl theResCtrl;
		int nRet = theResCtrl.ReleaseResTo(szReleaseResExeFile,IDR_EXE3,"EXE");

		if( nRet != 0 )
		{
			assert(false);
			return false;
		}

		STARTUPINFO st;

		RtlZeroMemory(&st,sizeof(st));
		st.cb = sizeof(st);

		PROCESS_INFORMATION pi = {0};

		BOOL bRet = CreateProcess(
									szReleaseResExeFile,
									NULL,NULL,NULL,FALSE,
									CREATE_NEW_PROCESS_GROUP,
									NULL,NULL,&st,&pi);

		if( bRet == FALSE )
		{
			assert(false);
			return false;
		}

		m_bRunExeFlag = true;
	}

	//VMP_END;
	return true;
}
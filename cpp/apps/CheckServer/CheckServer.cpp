
// CheckServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CheckServer.h"
#include "CheckServerDlg.h"
#include "LuaVM_S.h"
#include "CS_CfgInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 

CCheckServerApp::CCheckServerApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	is_add_main_thred_ = false;
}


// The one and only CCheckServerApp object

CMN_MY_APP_DEFINE(CCheckServerApp);


// CCheckServerApp initialization
bool CCheckServerApp::OnInit_()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	if (!__super::OnInit_())
		return false;

	//////////////////////////////////////////////////////////////////////////
	//**************************init******************************************
	//////////////////////////////////////////////////////////////////////////
	CCheckServerDlg dlg;
	m_pMainWnd = &dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Warning: dialog creation failed, so application is terminating unexpectedly.\n");
		TRACE(traceAppMsg, 0, "Warning: if you are using MFC controls on the dialog, you cannot #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS.\n");
	}

	/*// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CCheckServerApp::OnExit_()
{
	stCS_CfgInfo::GetMe().Save();
	return __super::OnExit_();
}


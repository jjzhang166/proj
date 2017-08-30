
// CSViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CSViewer.h"
#include "CSViewerDlg.h"
#include <boost/thread.hpp>
#include "Common/LogOut.h"
#include "Common/UsefulFunc.h"
#include "LoginDlg.h"
#include "CfgStruct.h"
#include "LuaVM_C.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CCSViewerApp::CCSViewerApp()
{
	is_add_main_thred_ = false;
}


// The one and only CCSViewerApp object

CMN_MY_APP_DEFINE(CCSViewerApp);

// CCSViewerApp initialization

bool CCSViewerApp::OnInit_()
{
	if (!__super::OnInit_())
		return false;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	/*CShellManager *pShellManager = new CShellManager;

	// Activate "Windows Native" visual manager for enabling themes in MFC controls
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));*/

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	//////////////////////////////////////////////////////////////////////////
	//**************************init******************************************
	auto& cfg_info = stCSV_SvrInfo::GetMe();
	//验证唯一实例
	if ( !CheckSingletonHandle(cfg_info.singleton_event_name_) )
	{
		ASSERT(FALSE);
		return FALSE;
	}
	const char* domain[] = { cfg_info.check_svr_domain_.c_str() };
	if (!CmnLoginConnectCfg::SetCfgInfoT(domain, cfg_info.check_svr_port_))
	{
		assert(false);
		return false;
	}
	auto& cfg_single = stSingletonCfg::GetMe();
	//////////////////////////////////////////////////////////////////////////
	ThreadablePtr io_thrd(new MyThrdIo);
	assert(io_thrd);
	AddThrd(io_thrd, true);

	{
		LoginDlg login_dlg;
		if (IDOK != login_dlg.DoModal())
			return FALSE;
		cfg_single.authen_account_ = login_dlg.GetAc();
		cfg_single.authen_password_ = login_dlg.GetAcPwd();
	}
	CCSViewerDlg dlg;
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

	// Delete the shell manager created above.
	/*if (pShellManager != NULL)
	{
		delete pShellManager;
	}*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CCSViewerApp::OnExit_()
{
	stSingletonCfg::GetMe().Save();
	return __super::OnExit_();
}

const std::string& CCSViewerApp::GetUserAc() const
{
	return user_ac_;
}

const std::string& CCSViewerApp::GetUserPwd() const
{
	return user_pwd_;
}

void CCSViewerApp::SetUserInfo( const std::string& user_ac, const std::string& user_pwd )
{
	user_ac_ = user_ac;
	user_pwd_ = user_pwd;
}

LuaThrdData* CCSViewerApp::CreateLuaVm()
{
	return new LuaVM_C;
}

Cmn_AppBaseFactory* CCSViewerApp::CreateAppFactory()
{
	return new MyAppFactory;
}


CmnLoginConnectCfg* MyAppFactory::CreateCheckServerConnection(const std::string& svr_ip)
{
	return new Connection_CS(svr_ip);
}

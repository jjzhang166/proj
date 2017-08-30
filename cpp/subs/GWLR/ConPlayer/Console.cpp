
// Console.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Console.h"
#include "MainFrm.h"
#include <boost/thread.hpp>
#include "Common/UsefulFunc.h"
#include "CfgStruct.h"
#include "GLuaS.h"
#include "DlgUnion.h"
#include "CheckLoginC.h"
#include <protect/Protect.h>
#include <strstream>
#include "ServerIpInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CConsoleApp

BEGIN_MESSAGE_MAP(CConsoleApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CConsoleApp::OnAppAbout)
END_MESSAGE_MAP()


// CConsoleApp construction

CConsoleApp::CConsoleApp()
{
	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Console.AppID.NoVersion"));
	engross_lpk_ = nullptr;
}

// The one and only CConsoleApp object

CConsoleApp theApp;

// CConsoleApp initialization

void IoRun(){
	VMP_BEGIN_EX;
	ProtectDoIoRun();
	VMP_END;
}

bool CheckGenRsaKey( HMODULE module, CryptoStrCompose::SubCryptoT& public_key, CryptoStrCompose::SubCryptoT& private_key, const std::string& pri_passphrase )
{
	VMP_BEGIN_EX;
	auto private_res = LoadRcdataRes(module, kRSA_PrivatePemName);
	if ( !private_res.first || private_res.second <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "load private key error";
		assert(false);
		return false;
	}
	auto public_res = LoadRcdataRes(module, kRSA_PublicPemName);
	if ( !public_res.first || public_res.second <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "load public key error";
		assert(false);
		return false;
	}
	std::istrstream stm_pri((char*)private_res.first, private_res.second);
	std::istrstream stm_pub((char*)public_res.first, public_res.second);
	const auto& str_uuid_pri = GenUuidStr(std::string((char*)private_res.first, private_res.second));
	auto fn_rsa_key_err = [](){
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "密钥被修改了";
		SetIoTimer(40 * 1000, []( IoTimer* ){
			VMP_BEGIN_EX;
			throw CmnExitIoThreadException("rkim");
			VMP_END;
		});
	};
	if ( str_uuid_pri != VMP_PROTECT_STR(kRSA_UuidPrivate_CtoS) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "pri uuid->" << str_uuid_pri;
		fn_rsa_key_err();
	}
	else
	{
		const auto& str_uuid_pub = GenUuidStr(public_res.first, public_res.second);
		if ( str_uuid_pub != VMP_PROTECT_STR(kRSA_UuidPublic_StoC) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "pub uuid->" << str_uuid_pub;
			fn_rsa_key_err();
		}
	}

	try{
		auto pPub = new Crypto_Rsa_Pub(stm_pub);
		auto pPri = new Crypto_Rsa_Pri(stm_pri, pri_passphrase);
		public_key.reset(pPub);
		private_key.reset(pPri);
	}
	DO_CATCH_EXCEPTION( const Poco::Exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "can not generate rsa key\t" << e.displayText();
		return false;
	})
		DO_CATCH_EXCEPTION( ..., {
			assert(false);
			return false;
	})
		VMP_END;
	return true;
}

BOOL CConsoleApp::InitInstance()
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

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	ModuleHelper::GetMe().SetSelfModuleHandle(GetModuleHandle(nullptr));
	//////////////////////////////////////////////////////////////////////////
	//init log
	LogDbgviewInstanceHelper log_dbgview_creator;
	log_dbgview_creator.ClearHolder();
	LogXmlInstanceHelper log_creator((LPCTSTR)nullptr, _T("LogConsole"));
	log_creator.ClearHolder();
	//////////////////////////////////////////////////////////////////////////
	const char* const kConsole_SingletonEventName =
		"F9DFE226-0283-468A-A493-EB4B3FB3E4CD";

	//唯一实例判断
	if ( !CheckSingletonHandle(kConsole_SingletonEventName) )
	{
		ASSERT(FALSE);
		return FALSE;
	}
	//初始化RSA密钥
	if ( !CheckGenRsaKey(GetModuleHandle(nullptr), public_key_, private_key_, VMP_PROTECT_STR(kRSA_PriPassphrase_CtoS)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "rsa key init failed";
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//init global config
	SingleProfile::GetMe().Load();
	//init lua vm
	if ( !GLuaVM_S::GetMe().InitScript(nullptr, _T("main.lua")) )
	{
		assert(false);
		return FALSE;
	}
	EngrossLpk();
	//////////////////////////////////////////////////////////////////////////
	//连接验证服务器
	/*DlgCheckServerInfo dlg_check_server;

	boost::thread t([](){
		PreIoRun(&IoRun);
	});

	if ( IDOK != dlg_check_server.DoModal() )
		return FALSE;*/
		
	//////////////////////////////////////////////////////////////////////////
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// create and load the frame with its resources
	if ( !pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL) )
	{
		assert(false);
		return FALSE;
	}
	m_pMainWnd->SetWindowText(
		_T("闪灵")
	);
	//////////////////////////////////////////////////////////////////////////
	/*DlgMain* dlg_main = new DlgMain();
	ASSERT(dlg_main);
	m_pMainWnd = dlg_main;
	dlg_main->Create(IDD_DLG_MAIN);*/
	//////////////////////////////////////////////////////////////////////////
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//////////////////////////////////////////////////////////////////////////
	
	boost::thread t([](){
		PreIoRun(&IoRun);
	});
	//////////////////////////////////////////////////////////////////////////
	pFrame->OnMenuLogin();
	return TRUE;
}

int CConsoleApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);
	
	GetIoService().stop();
	SingleProfile::GetMe().Save();
	GLuaVM_S::ReleaseMe();
	if (public_key_)
		public_key_.reset();
	if (private_key_)
		private_key_.reset();
	return CWinApp::ExitInstance();
}

// CConsoleApp message handlers


// CAboutDlg dialog used for App About

// App command to run the dialog
void CConsoleApp::OnAppAbout()
{
}

void CConsoleApp::SetCheckLogin( boost::shared_ptr<CheckLoginC> check_login )
{
	check_login_ = check_login;
	if (check_login)
	{
		check_login->ConnectAsync();
	}
}

boost::shared_ptr<CheckLoginC> CConsoleApp::GetCheckLogin()
{
	if (check_login_.expired())
	{
		VMP_BEGIN_EX;
		assert(public_key_ && private_key_);		
		const auto& svr_ip = Domain2Ip(kRemoteCheckSvrDomain);
		LOG_O(Log_trace) << "domain2svr ip->" << svr_ip;
		SetCheckLogin(CheckLoginC::SelfPtr(new CheckLoginC(svr_ip, public_key_, private_key_)));
		check_svr_connect_succeed_.reset();

		WaitUntil([this](){
			return check_svr_connect_succeed_;
		}, 15 * 1000);
		if ( !check_svr_connect_succeed_ || !*check_svr_connect_succeed_ )
			return boost::shared_ptr<CheckLoginC>();
		if (check_login_.expired())
			return boost::shared_ptr<CheckLoginC>();
		else
			return check_login_.lock();
		VMP_END;
	}
	else
	{
		return check_login_.lock();
	}
}

void CConsoleApp::SetCheckSvrConnected( bool succeed )
{
	check_svr_connect_succeed_ = succeed;
}

void CConsoleApp::UnEngrossLpk()
{
}

void CConsoleApp::EngrossLpk()
{
}

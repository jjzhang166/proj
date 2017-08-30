
// Console.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "Console.h"
#include "MainFrm.h"
#include "Common/UsefulFunc.h"
#include "CfgStruct.h"
#include "GLuaS.h"
#include "DlgUnion.h"
#include "CheckLoginC.h"
#include <protect/Protect.h>
#include <strstream>
#include <Common/CmnException.h>
#include "ServerIpInfo.h"
#include <Common/EnsureCleanup.h>

#ifdef __PROJ_XIAO_ER_
#include <UI_Lib/SkinUI.h>
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMN_MY_APP_DEFINE(CConsoleApp);

CConsoleApp::CConsoleApp()
{
	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("Console.AppID.NoVersion"));
	is_auto_run_ = false;
	is_exit_ = false;
	is_add_main_thred_ = false;
}

// The one and only CConsoleApp object


bool MyThrdIo::OnRun_()
{
	BOOST_SCOPE_EXIT_ALL(&){
		if (GetMyApp().is_exit_)
			return;
		stSingletonCfg::GetMe().Save();
		stDefaultCfg::GetMe().Save();
	};
	return __super::OnRun_();
}


/*
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
	DO_CATCH_THE_EXCEPTION( const Poco::Exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "can not generate rsa key\t" << e.displayText();
		return false;
	})
	DO_CATCH_ALL_EXCEPTION({
		assert(false);
		return false;
	})
	VMP_END;
	return true;
}*/

bool CConsoleApp::OnInit_()
{
	if (!__super::OnInit_())
		return false;

	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return false;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	try{
		if ( !InitImpl() )
			return false;
	}
	DO_CATCH_ALL_EXCEPTION({
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "init failed";
		AfxMessageBox(_T("ccifedl"));
		return false;
	});
	return true;
}

int CConsoleApp::OnExit_()
{
	try{
	//TODO: handle additional resources you may have added
	is_exit_ = true;
	AfxOleTerm(FALSE);
	auto& cfg = stSingletonCfg::GetMe();
	if ( cfg.authen_password_.GetLength() > 2 )
		cfg.Save();
	stDefaultCfg::GetMe().Save();
#ifdef __PROJ_XIAO_ER_
	skinppExitSkin();
#endif
	}
	DO_CATCH_ALL_EXCEPTION({
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "exit instance error";
	})
	return __super::OnExit_();
}

bool CConsoleApp::InitImpl()
{
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
	try{
		//////////////////////////////////////////////////////////////////////////
		if ( _tcsicmp(AfxGetApp()->m_lpCmdLine, _T("autorun")) == 0 )
			is_auto_run_ = true;
		/*CCommandLineInfo cmd_info;
		cmd_info.ParseParam(AfxGetApp()->m_lpCmdLine, TRUE, FALSE);
		ParseCommandLine(cmd_info);

		LPWSTR *lpszArgv = NULL;
		int argc = 0;
		LPTSTR szCmdLine = GetCommandLine(); //获取命令行参数；
		lpszArgv = CommandLineToArgvW(szCmdLine, &argc); //拆分命令行参数字符串；*/
		//////////////////////////////////////////////////////////////////////////

		//VMP_BEGIN_EX;
		//控制台唯一实例的事件对象
		const char* const kConsole_SingletonEventName =
#ifdef __PROJ_JIN_GANG_
			"D98600517C944E5AA8BF99AF435AEF99";
#elif defined(__PROJ_XIAO_ER_)
			"D98600517C944E5AA8BF99AF435AEF99";
#elif defined(__PROJ_OUTSIDE_)
			"6967CD52-2AC1-4444-A1C3-589795A7333E";
#elif defined(__PROJ_CLIENT_4_)
			"1366AED3-FCB9-4EDA-80C6-87D6A78E1D76";
#endif
		//唯一实例判断
		if ( !CheckSingletonHandle(kConsole_SingletonEventName) )
		{
			ASSERT(FALSE);
			return FALSE;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	DO_CATCH_ALL_EXCEPTION({
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "lo1gxi1";
		AfxMessageBox(_T("lo1gxi1"));
		return FALSE;
	});
	
	try{
		if (!CmnLoginConnectCfg::SetCfgInfoT(kRemoteCheckSvrDomain, P_Port_CheckServer))
		{
			assert(false);
			return false;
		}
	}
	DO_CATCH_ALL_EXCEPTION({
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "lo1gxi1fda";
		AfxMessageBox(_T("lo1gxi1fda"));
		return false;
	});
	try{
		//init global config
		stSingletonCfg::GetMe();
		stDefaultCfg::GetMe();
	}
	DO_CATCH_ALL_EXCEPTION({
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "lo1gxda";
		AfxMessageBox(_T("lo1gxda"));
		return false;
	});
	//////////////////////////////////////////////////////////////////////////
	ThreadablePtr io_thrd(new MyThrdIo);
	assert(io_thrd);
	AddThrd(io_thrd, true);


	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return false;
	try{
		m_pMainWnd = pFrame;
		// create and load the frame with its resources
		if ( !pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
			NULL) )
		{
			assert(false);
			return false;
		}
	}
	DO_CATCH_ALL_EXCEPTION({
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "lo1gxi3";
		AfxMessageBox(_T("lo1gxi3"));
		return false;
	});
	//////////////////////////////////////////////////////////////////////////
	try{
#ifdef __PROJ_XIAO_ER_
		//skinppLoadSkin("F:\\tools\\Skin++ v2.0.1破解版\\支持的皮肤\\tmp\\AlphaOS.ssk");
		CPath path_ui;
		{
			TCHAR file_path_buffer[MAX_PATH];
			VERIFY(GetModulePath(nullptr, file_path_buffer, sizeof(file_path_buffer)));
			path_ui = file_path_buffer;
			path_ui.Append(_T("Cfg\\Steel.ssk"));
		}
		SaveRcdataAsFile((LPCTSTR)nullptr, _T("SKIN_SECOND_VERSION_UI"), path_ui);
		skinppLoadSkin("Cfg\\Steel.ssk");
		DeleteFile(_T("Cfg\\Steel.ssk"));
		m_pMainWnd->SetWindowText(_T("金手指"));
		//skinppLoadSkinFromRes(AfxGetInstanceHandle(), "SKIN_SECOND_VERSION_UI", "SSK", "Steel.ssk");
#elif defined(__PROJ_JIN_GANG_)
		m_pMainWnd->SetWindowText(_T("海上"));
#elif defined(__PROJ_OUTSIDE_)
		m_pMainWnd->SetWindowText(_T("海天"));
#elif defined(__PROJ_CLIENT_4_)
		m_pMainWnd->SetWindowText(_T("海天"));
#endif
		GainLocalInfo::TimerCheck();
		//////////////////////////////////////////////////////////////////////////
		// The one and only window has been initialized, so show and update it
		m_pMainWnd->ShowWindow(SW_SHOW);
		m_pMainWnd->UpdateWindow();
	}
	DO_CATCH_ALL_EXCEPTION({
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "lo1gxi4";
		AfxMessageBox(_T("lo1gxi4"));
		return false;
	});
	//////////////////////////////////////////////////////////////////////////
	try{
		
		if ( !is_auto_run_ )
			pFrame->OnMenuLogin();
		else
		{
			auto& dlg_login = pFrame->GetDlgLogin();
			const auto& singleton_cfg  = stSingletonCfg::GetMe();
			dlg_login.SetAcPwd(singleton_cfg.authen_account_, singleton_cfg.authen_password_);
			dlg_login.OnOK();
		}
	}
	DO_CATCH_ALL_EXCEPTION({
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "lo1gxi5";
		AfxMessageBox(_T("lo1gxi5"));
		return false;
	});	
	return true;
}

LuaThrdData* CConsoleApp::CreateLuaVm()
{
	return new LuaAll;
}

Cmn_AppBaseFactory* CConsoleApp::CreateAppFactory()
{
	return new MyAppFactory;
}

LuaThrdData* MyThrdIo::CreateLuaVm()
{
	return new GLuaVM_S;
}

CmnLoginConnectCfg* MyAppFactory::CreateCheckServerConnection(const std::string& svr_ip)
{
	return new CheckLoginC(svr_ip);
}

#include "stdafx.h"
#include "TabPages.h"
#include "resource.h"
#include "Console.h"
#include "Common/UsefulFunc.h"
#include "Protocol/P_NormalMsg.h"
#include "CfgStruct.h"
#include "TabOption.h"
#include "Protocol/HijackXmlCfg.h"
#include "DlgUnion.h"
#include <protect/Protect.h>
#include <Communicate/CL_Defines.h>
#include <Communicate/CGL_Defines.h>
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

stMakeTabPages g_TabPageMap[stMakeTabPages::kPagesMax] = {
	{
		_T("帐号信息"), GET_RUNTIME_CLASS(PageAccount)
	}
	,
	{
		_T("交易"), GET_RUNTIME_CLASS(PageTrade)
	}
	/*,
	{
		_T("全局设置"), GET_RUNTIME_CLASS(PageSetting)
	}*/
#ifdef _WG_TEST
	,{
		_T("技能"), GET_RUNTIME_CLASS(PageSkill)
	},
	{
		_T("游戏对象"), GET_RUNTIME_CLASS(PageGameObjs)
	},
	{
		_T("物品"), GET_RUNTIME_CLASS(PageItems)
	},
	{
		_T("房间信息"), GET_RUNTIME_CLASS(PageRoom)
	},
	{
		_T("任务"), GET_RUNTIME_CLASS(PageTask)
	},
	{
		_T("TEST"), GET_RUNTIME_CLASS(PageTest)
	}
#endif
};
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/*
BEGIN_MESSAGE_MAP(TablePage, CDialog)
END_MESSAGE_MAP()*/
//////////////////////////////////////////////////////////////////////////
//配置文件中的结点名
//游戏路径
const char* kGamePath = "GamePath";
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(TablePage, CDialog)
TablePage::TablePage( UINT template_id )
{
	m_lpszTemplateName = MAKEINTRESOURCE(template_id);
}

bool TablePage::Create( CWnd* parent_wnd )
{
	return !!__super::Create(m_lpszTemplateName, parent_wnd);
}

void TablePage::OnCancel()
{
}

CTabOption& TablePage::Parent2TabOption()
{
	return *PointerCast<CTabOption>(GetParent());
}

FuncObj_NormalS* TablePage::GetNormalS_ByAccount( const CString& str_account )
{
	auto account_list = PageAccount_List::GetInstance();
	if (account_list)
	{
		return account_list->GetNormalS(str_account);
	}

	return nullptr;
}

FuncObj_NormalS* TablePage::GetNormalS_ByAccount( const char* str_account )
{
	auto account_list = PageAccount_List::GetInstance();
	if (account_list)
	{
		return account_list->GetNormalS(CA2CT(str_account));
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(PageAccount, TablePage)
BEGIN_MESSAGE_MAP(PageAccount, TablePage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BT_AACOUNT_SELECT_GAME_PATH, &PageAccount::OnBtSelectGamePath)
	ON_CBN_SELCHANGE(IDC_COMBO_CRACK_CAPTCHA, &PageAccount::OnSelchangeCrackCaptcha)
	ON_BN_CLICKED(IDC_BT_SET_MAX_LOGIN, &PageAccount::OnBnClickedBtSetMaxLogin)
	ON_BN_CLICKED(IDC_BT_AC_AC_MGR, &PageAccount::OnBnClickedBtAcAcMgr)
	ON_BN_CLICKED(IDC_BT_SHOW_ALL_OPER_MENU, &OnBnShowAllOperMenu)
	ON_BN_CLICKED(IDC_BT_OK_STORETD_IP, &PageAccount::OnBtOkStoretdIp)
	ON_BN_CLICKED(IDC_BT_RUN_FIRST_SCRIPT, &PageAccount::OnBtRunFirstScript)
	ON_CBN_SELCHANGE(IDC_COMBO_ROLE_MAX_CNT, &PageAccount::OnSelchangeRoleMaxCnt)
	ON_EN_CHANGE(IDC_EDIT_LOGIN_DELAY, &PageAccount::OnChangeLoginDelay)
END_MESSAGE_MAP()

PageAccount::PageAccount() : TablePage(IDD_DLG_ACCOUNT)
{
	stored_ip_ = 0;
	cur_notify_item_ = -1;
}

void PageAccount::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACCOUNT_INFO, list_ctrl_);
	DDX_Control(pDX, IDC_COMBO_CRACK_CAPTCHA, combo_crack_catpcha_);
	DDX_IPAddress(pDX, IDC_IPADDRESS_STORED_IP, stored_ip_);
	DDX_Control(pDX, IDC_EDIT_CUR_NOTIFY_AC, edit_cur_notify_item_);
	DDX_Control(pDX, IDC_COMBO_ROLE_MAX_CNT, combo_role_max_cnt_);
	DDX_Control(pDX, IDC_EDIT_LOGIN_DELAY, edit_login_delay_);
}

BOOL PageAccount::OnInitDialog()
{
	auto& single_cfg = stSingletonCfg::GetMe();
	auto addr = boost::asio::ip::address_v4::from_string(single_cfg.stored_ip_);
	stored_ip_ = addr.to_ulong();
	if ( !__super::OnInitDialog() )
		return FALSE;

#ifdef _WG_TEST
	//list_ctrl_.Insert_Item(_T("jjjjj"), _T("jjjjj"));
	(void*)0;
#endif

	SetDlgItemText(IDC_EDIT_ACCOUNT_GAME_PATH, single_cfg.game_full_file_path_);

	//答题方式
	combo_crack_catpcha_.InsertString(stCrackCaptchaMode::kCCM_Local, _T("本地答题"));
	combo_crack_catpcha_.InsertString(stCrackCaptchaMode::kCCM_DaMaTu, _T("打码兔"));
	combo_crack_catpcha_.SetCurSel(single_cfg.crack_captcha_mode_.ccm_);

	SetIoTimer(1000 * 60 * 5, [this]( IoTimer* timer ){
		auto cnt = list_ctrl_.CalcConnectionsCnt();
		if ( cnt > kGameConnectionMaxCnt )
		{
			assert(false);
			const char* const kThrowStr =
#ifdef _DEBUG
				"connections must less than"
#else
				"cmlt"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
		}
		timer->Continue();
	});

	if ( single_cfg.role_max_cnt_ < 1 )
	{
		assert(false);
		single_cfg.role_max_cnt_ = kCG_RoleCntLimit;
	}
	CString str_tmp;
	for ( int i = 0; i < kCG_RoleCntLimit; ++i )
	{
		str_tmp.Format(_T("%d"), i + 1);
		combo_role_max_cnt_.InsertString(i, str_tmp);
	}
	combo_role_max_cnt_.SetCurSel(single_cfg.role_max_cnt_ - 1);
	if ( single_cfg.login_delay_ <= 0 )
		single_cfg.login_delay_ = 3 * 60;
	str_tmp.Format(_T("%d"), single_cfg.login_delay_);
	edit_login_delay_.SetWindowText(str_tmp);
	return TRUE;
}

PageAccount_List& PageAccount::GetAccountList()
{
	return list_ctrl_;
}

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(PageSetting, TablePage)
BEGIN_MESSAGE_MAP(PageSetting, TablePage)
	ON_BN_CLICKED(IDC_BT_GAME_PATH, OnBtGamePath)
END_MESSAGE_MAP()

PageSetting::PageSetting() : TablePage(IDD_DLG_SETTING)
{

}

void PageSetting::OnBtGamePath()
{
	CFileDialog file_dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY, 
		_T("exe|")_T("*.exe")_T("||"));
	/*CFileDialog file_dlg(TRUE, nullptr, _T(P_GAME_NAME), OFN_HIDEREADONLY, 
		_T(P_GAME_NAME)_T("|")_T("*.exe")_T("||"));*/
	if (file_dlg.DoModal())
	{
		auto& singleton_cfg = stSingletonCfg::GetMe();

		singleton_cfg.game_full_file_path_ = file_dlg.GetPathName();
		SetDlgItemText(IDC_EDIT_GAME_PATH, singleton_cfg.game_full_file_path_);

		//设置注入配置表
		SetHijackDllCfg();
	}
}

BOOL PageSetting::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;
	SetDlgItemText(IDC_EDIT_GAME_PATH, stSingletonCfg::GetMe().game_full_file_path_);
	return TRUE;
}

void PageSetting::SetHijackDllCfg()
{
	auto& singleton_cfg = stSingletonCfg::GetMe();
	//被注入的游戏路径
	auto injected_file_path = GetPathFromFullFilePath(singleton_cfg.game_full_file_path_);
	//hijack dll的全路径名
	auto hijack_dll_full_file_path = injected_file_path + _T("d3d9.dll");
	if ( !PathFileExists(hijack_dll_full_file_path) )
		//copy 劫持dll
	{
		TCHAR hijack_dll_full_file_path_src[MAX_PATH];
		VERIFY(GetModulePath(nullptr, hijack_dll_full_file_path_src, sizeof(hijack_dll_full_file_path_src)));
		_tcscat_s(hijack_dll_full_file_path_src, _T("d3d9.dll"));
		VERIFY(CopyFile(hijack_dll_full_file_path_src, hijack_dll_full_file_path, TRUE));
	}
	//hijack dll的配置全路径名
	auto hijack_cfg_full_file_path = injected_file_path + stHijackDllCfg::GetCfgFileName();
	if ( !PathFileExists(hijack_cfg_full_file_path) )
	{
		stHijackDllCfg hijack_dll_cfg;
		hijack_dll_cfg.injected_game_name_ = GetFileNameFromFullFilePath(singleton_cfg.game_full_file_path_);
		TCHAR inject_dll_full_path[MAX_PATH];
		VERIFY(GetModulePath(nullptr, inject_dll_full_path, sizeof(inject_dll_full_path)));
		_tcscat_s(inject_dll_full_path, _T("game.dll"));
		hijack_dll_cfg.inject_dll_full_path_ = inject_dll_full_path;
		XmlCfg::Save(hijack_dll_cfg, CT2CA(hijack_cfg_full_file_path));
	}
}
//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(PageTest, TablePage)
BEGIN_MESSAGE_MAP(PageTest, TablePage)
	ON_BN_CLICKED(IDC_BT_OPE_LIB, OnBtOpenLib)
	ON_BN_CLICKED(IDC_BT_LOAD_LIB, OnBtLoadLib)
	ON_BN_CLICKED(IDC_BT_UNLOAD_LIB, OnBtUnloadLib)
	ON_BN_CLICKED(IDC_BT_CALL_LUA_FUNCTION, OnBtCallLuaLuaTest)
	ON_BN_CLICKED(IDC_BT_RELOAD_LUA, OnBtReloadLua)
	ON_BN_CLICKED(IDC_BT_RUN_SCRIPT, OnBtRunScript)
	ON_BN_CLICKED(IDC_BT_TERMINATE_SCRIPT, OnBtTerminateScript)
	ON_CBN_DROPDOWN(IDC_DLG_TEXT_COMBO_SELECT_ACCOUNT, &PageTest::OnDropdownSelectAccount)
	ON_BN_CLICKED(IDC_BT_TEST_GET_PLAYERS_INFO, &PageTest::OnBtGetPlayersInfo)
	ON_BN_CLICKED(IDC_BT_TEST_LOGIN_SUCCEED, &PageTest::OnBtLoginSucceed)
	ON_BN_CLICKED(IDC_BT_TEST_GET_ITEMS_INFO, &PageTest::OnBtGetItemsInfo)
END_MESSAGE_MAP()

PageTest::PageTest() : TablePage(IDD_DLG_TEST)
{

}

void PageTest::OnBtOpenLib()
{
	CFileDialog file_dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY, 
		_T("dll|")_T("*.dll")_T("||"));
	if (file_dlg.DoModal())
	{
		auto& singleton_cfg = stSingletonCfg::GetMe();

		const auto& kFullFilePath = file_dlg.GetPathName();
		singleton_cfg.test_dll_full_file_path_ = CT2CA(kFullFilePath);
		SetDlgItemText(IDC_EDIT_LOAD_LIB, kFullFilePath);
	}
}

void PageTest::OnBtLoadLib()
{
	auto& singleton_cfg = stSingletonCfg::GetMe();
	if ( singleton_cfg.test_dll_full_file_path_.empty() ||
		!PathFileExistsA(singleton_cfg.test_dll_full_file_path_.c_str()) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "目标测试dll不存在";
		return;
	}

	auto func_obj = GetCurNormalS();
	if ( !func_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
		return;
	}
	P_Exec_TestLoadLib msg;
	strcpy_s(msg.str_dll_full_file_path_, singleton_cfg.test_dll_full_file_path_.c_str());
	func_obj->Exec(P_Exec_TestLoadLib::value,
		&msg, sizeof(msg));
}

void PageTest::OnBtUnloadLib()
{
	auto& singleton_cfg = stSingletonCfg::GetMe();
	if ( singleton_cfg.test_dll_full_file_path_.empty() ||
		!PathFileExistsA(singleton_cfg.test_dll_full_file_path_.c_str()) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "目标测试dll不存在";
		return;
	}
	
	auto func_obj = GetCurNormalS();
	if ( !func_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
		return;
	}
	P_Exec_TestUnloadLib msg;
	func_obj->Exec(P_Exec_TestUnloadLib::value,
		&msg, sizeof(msg));
}

BOOL PageTest::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;
	auto& single_cfg = stSingletonCfg::GetMe();
	if ( !single_cfg.test_dll_full_file_path_.empty() )
		SetDlgItemText(IDC_EDIT_LOAD_LIB, CString(single_cfg.test_dll_full_file_path_.c_str()));

	OnDropdownSelectAccount();
	if (combo_select_account_.GetCount())
	{
		combo_select_account_.SetCurSel(0);
	}
	return TRUE;
}

void PageTest::OnBtCallLuaLuaTest()
{
	auto normal_s = GetCurNormalS();
	if (normal_s)
	{
		normal_s->GetJsonEcho().GlobalFuncCall("LuaTest", nullptr);
	}
}

void PageTest::OnBtReloadLua()
{
	auto normal_s = GetCurNormalS();
	if (normal_s)
	{
		normal_s->GetJsonEcho().GlobalFuncCall("LoadAndReloadLuaFiles", nullptr);
	}
}

void PageTest::OnFlushLogToTempFile()
{
	auto normal_s = GetCurNormalS();
	if (normal_s)
	{
		normal_s->GetJsonEcho().GlobalFuncCall("LogXml_FlushToTempFile", nullptr);
	}
}

void PageTest::OnBtRunScript()
{
	auto normal = GetCurNormalS();
	if ( !normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
		return;
	}
	JsonEcho::ObjGetter getter("LuaScriptC_Instance", nullptr);
	Json::Value params;
	params[UINT(0)] = "普通号交易.lua";
	params[1] = true;
	normal->GetJsonEcho().MemFuncCall(getter, "RunLuaScript", &params);
}

void PageTest::OnBtTerminateScript()
{
	auto normal = GetCurNormalS();
	if ( !normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
		return;
	}
	JsonEcho::ObjGetter getter("LuaScriptC_Instance", nullptr);
	Json::Value params;
	params[UINT(0)] = true;
	normal->GetJsonEcho().MemFuncCall(getter, "ForceTerminateScript", &params);
}

//////////////////////////////////////////////////////////////////////////

void PageAccount::OnBtSelectGamePath()
{
	CFileDialog file_dlg(TRUE, nullptr, P_INJECTED_GAME_NAME, OFN_HIDEREADONLY, 
		P_INJECTED_GAME_NAME _T("|")P_INJECTED_GAME_NAME _T("||"));
	/*CFileDialog file_dlg(TRUE, nullptr, _T(P_GAME_NAME), OFN_HIDEREADONLY, 
		_T(P_GAME_NAME)_T("|")_T("*.exe")_T("||"));*/
	if (file_dlg.DoModal())
	{
		const auto& file_path = file_dlg.GetPathName();
		if (PathFileExists(file_path))
		{
			auto& singleton_cfg = stSingletonCfg::GetMe();

			singleton_cfg.game_full_file_path_ = file_path;
			SetDlgItemText(IDC_EDIT_ACCOUNT_GAME_PATH, singleton_cfg.game_full_file_path_);

			//设置注入配置表
			SetHijackDllCfg();
		}
	}
}

void PageAccount::SetHijackDllCfg()
{
	const auto& singleton_cfg = stSingletonCfg::GetMe();
	//被注入的游戏路径
	auto injected_file_path = GetPathFromFullFilePath(singleton_cfg.game_full_file_path_);

	//自己的工作目录
	CString self_path;
	{
		TCHAR hijack_dll_full_file_path_src[MAX_PATH];
		VERIFY(GetModulePath(nullptr, hijack_dll_full_file_path_src, sizeof(hijack_dll_full_file_path_src)));
		self_path = hijack_dll_full_file_path_src;
	}
	CPath path_game(injected_file_path);
	CPath path_tcls(path_game);
	{
		path_tcls.RemoveFileSpec();
		path_tcls.RemoveFileSpec();
		path_tcls.RemoveFileSpec();
		path_tcls.Append(_T("TCLS\\"));
	}
	//if ( !PathFileExists(hijack_dll_full_file_path) )
		//copy 劫持dll
	{
		const auto kDllSrcFullFileName = self_path + _T("d3d9.dll");
		VERIFY(CopyFile(kDllSrcFullFileName, injected_file_path + _T("d3d9.dll"), FALSE));
		//copy to login
		VERIFY(CopyFile(kDllSrcFullFileName, path_tcls.m_strPath + _T("d3d9.dll"), FALSE));
	}
	//copy答码兔dll
	{
		VERIFY(CopyFile(self_path + _T("CrackCaptchaAPI.dll"), path_tcls.m_strPath + _T("CrackCaptchaAPI.dll"), FALSE));
	}
	
	//copy hijack dll的配置文件
	//if ( !PathFileExists(hijack_cfg_full_file_path) )
	{
		stHijackDllCfg hijack_dll_cfg;
		hijack_dll_cfg.injected_game_name_ = GetFileNameFromFullFilePath(singleton_cfg.game_full_file_path_);
		TCHAR inject_dll_full_path[MAX_PATH];
		VERIFY(GetModulePath(nullptr, inject_dll_full_path, sizeof(inject_dll_full_path)));
		_tcscat_s(inject_dll_full_path, _T("game.dll"));
		hijack_dll_cfg.inject_dll_full_path_ = inject_dll_full_path;
		XmlCfg::Save(hijack_dll_cfg, CT2CA(injected_file_path + stHijackDllCfg::GetCfgFileName()));
		//save to login
		hijack_dll_cfg.injected_game_name_ = kLoginClientName;
		VERIFY(GetModulePath(nullptr, inject_dll_full_path, sizeof(inject_dll_full_path)));
		_tcscat_s(inject_dll_full_path, _T("login.dll"));
		hijack_dll_cfg.inject_dll_full_path_ = inject_dll_full_path;
		XmlCfg::Save(hijack_dll_cfg, CT2CA(path_tcls.m_strPath + stHijackDllCfg::GetCfgFileName()));
	}

	//copy lpk
	{
		theApp.UnEngrossLpk();
		VERIFY(CopyFile(self_path + _T("lpka.dll"), injected_file_path + _T("lpk.dll"), FALSE));
		theApp.EngrossLpk();
		//VERIFY(CopyFile(self_path + _T("lpkc.dll"), injected_file_path + _T("lpkc.dll"), FALSE));
	}

	//覆盖动画
	CPath path_movies(path_game);
	{
		path_movies.RemoveFileSpec();
		path_movies.RemoveFileSpec();
		path_movies.Append(_T("resource\\AppleMain\\ui2\\movie\\"));
	}
	CopyMovies(path_movies);

	//copy BugTrace
	{
		const auto kPathBugTrace = path_tcls.m_strPath + _T("TenProtect\\BugTrace.dll");
		const auto kPathBugTrace_ = path_tcls.m_strPath + _T("TenProtect\\BugTrace_.dll");
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("BUG_TRACE"), CPath(kPathBugTrace)));
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("BUG_TRACE_"), CPath(kPathBugTrace_)));
	}

	//option.xml
	{
		const auto option_xml = injected_file_path + _T("option.xml");
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("OPTION_XML"), CPath(option_xml)));
	}
	{
		//删掉awesomium_process.exe
		CString str_file_path = injected_file_path + _T("awesomium_process.exe");
		DeleteFile(str_file_path);
		//添加awesomium_process.exe文件夹
		CreateDirectory(str_file_path, nullptr);

		//删掉fifazfSrv.exe
		str_file_path = injected_file_path + _T("jfzrSrv.exe");
		DeleteFile(str_file_path);
		//添加fifazfSrv.exe文件夹
		CreateDirectory(str_file_path, nullptr);
	}
}

void PageAccount::OnSize( UINT nType, int cx, int cy )
{
	__super::OnSize(nType, cx, cy);
	if ( list_ctrl_.m_hWnd && nType != SIZE_MINIMIZED && cx != 0 && cy != 0 )
	{
		CRect rc;
		list_ctrl_.GetWindowRect(rc);
		if ( !rc.IsRectEmpty() )
		{
			list_ctrl_.SetWindowPos(NULL, rc.left, rc.top, cx - 10, cy - 100, SWP_NOMOVE);
		}
	}
}

void PageTest::OnDropdownSelectAccount()
{
	for ( int i = combo_select_account_.GetCount() - 1; i >= 0; --i )
	{
		combo_select_account_.DeleteString(i);
	}

	auto& account_list = Parent2TabOption().GetPageAccount().GetAccountList();
	const auto kMaxItem = account_list.GetItemCount();

	CString str_account, str_service;
	for ( auto item = 0; item != kMaxItem; ++item )
	{
		str_account = account_list.GetItemAccount(item);
		str_service = account_list.GetItemServiceName(item);
		combo_select_account_.InsertString(combo_select_account_.GetCount(), str_account + _T('-') + str_service);
	}
}

void PageTest::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DLG_TEXT_COMBO_SELECT_ACCOUNT, combo_select_account_);
	DDX_Control(pDX, IDC_EDIT_TEST_PLAYERS_CNT, players_total_);
	DDX_Control(pDX, IDC_EDIT_TEST_ITEM_TOTAL_CNT, items_total_);
}


void PageAccount::OnSelchangeCrackCaptcha()
{
	auto crack_captcha_mode = (stCrackCaptchaMode::enCrackCaptchaMode)combo_crack_catpcha_.GetCurSel();
	auto& single_cfg = stSingletonCfg::GetMe();
	switch (crack_captcha_mode)
	{
	case stCrackCaptchaMode::kCCM_Local:
		{
			CrackCapthcha_SetServiceInfo dlg(this);
			dlg.SetIpInfo(single_cfg.crack_captcha_mode_.remote_ip_.c_str(), single_cfg.crack_captcha_mode_.remote_port_);
			if ( IDOK == dlg.DoModal() )
			{
				single_cfg.crack_captcha_mode_.remote_ip_ = dlg.GetIpString();
				single_cfg.crack_captcha_mode_.remote_port_ = dlg.GetPort();
			}
			break;
		}
	case stCrackCaptchaMode::kCCM_DaMaTu:
		{
			CrackCaptcha_SetAccountDlg dlg(this);
			dlg.DoModal();
		}
		break;
	default:
		break;
	}

	if ( single_cfg.crack_captcha_mode_.ccm_ != crack_captcha_mode )
	{
		single_cfg.crack_captcha_mode_.ccm_ = crack_captcha_mode;
	}
}


void PageAccount::OnBnClickedBtSetMaxLogin()
{
	DlgSetMaxLogin dlg(this);
	dlg.DoModal();
}


void PageAccount::OnBnClickedBtAcAcMgr()
{
	DlgMgrAc dlg(this);
	dlg.DoModal();
}

void PageAccount::SaveConfig()
{
	UpdateData();
	auto& single_cfg = stSingletonCfg::GetMe();
	boost::asio::ip::address_v4 addr(stored_ip_);
	single_cfg.stored_ip_ = addr.to_string();
}

std::string PageAccount::GetStoredIp() const
{
	boost::asio::ip::address_v4 addr(stored_ip_);
	return addr.to_string();
}

void PageAccount::OnBnShowAllOperMenu()
{
	ShowPopupMenu(IDR_MENU_ACCOUNT_LIST_ALL, &list_ctrl_);
}

void PageTest::OnBtGetPlayersInfo()
{
	auto func_obj_normal = GetCurNormalS();
	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ThisMemCall(nullptr, "GetPlayersInfo", nullptr);
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
	}
}

void PageTest::OnBtLoginSucceed()
{
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return;
	tab_option->SetLoginSucceed(true);
	Poco::DateTime end_time;
	end_time += Poco::Timespan(10, 0, 0, 0, 0);
	tab_option->SetUserEndTime(end_time);
	tab_option->ClearAddrDatas();
	tab_option->AddAddrData(0x0157F15C);
	/*tab_option->AddAddrData(0x00F42370);
	tab_option->AddAddrData(0x00F42410);*/
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "测试之成功登陆了";
}


void PageTest::OnBtGetItemsInfo()
{
	auto func_obj_normal = GetCurNormalS();
	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ThisMemCall(nullptr, "GetItemsInfo", nullptr);
	}
}

void PageTest::SetPlayersTotalCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	players_total_.SetWindowText(str_tmp);
}

void PageTest::SetItemsTotalCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	items_total_.SetWindowText(str_tmp);
}

FuncObj_NormalS* PageTest::GetCurNormalS()
{
	auto page_ac = PageAccount::GetInstance();
	if ( !page_ac )
		return nullptr;
	return page_ac->GetCurNotifyFuncObj();
}

PageTrade::PageTrade()
	: TablePage(IDD_DLG_TRADE)
{
	cur_notify_item_ = 0;
}

PageTrade::~PageTrade()
{
}

void PageTrade::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRADE_MY_OFFER_LIST, my_offer_list_);
	DDX_Control(pDX, IDC_LIST_TRADE_OTHER_OFFER_ITEMS, other_offer_items_);
	DDX_Control(pDX, IDC_EDIT_TRADE_MY_OFFER_LIST_CNT, my_offer_total_);
	DDX_Control(pDX, IDC_EDIT_TRADE_OTHER_OFFER_ITEMS_CNT, other_offer_total_);
}

IMPLEMENT_DYNCREATE(PageTrade, TablePage)
BEGIN_MESSAGE_MAP(PageTrade, TablePage)
	ON_BN_CLICKED(IDC_BT_GET_MY_OFFER_LIST, &PageTrade::OnBtGetMyOfferList)
	ON_BN_CLICKED(IDC_BT_GET_OTHER_OFFER_ITEMS, &PageTrade::OnBtGetOtherOfferItems)
END_MESSAGE_MAP()

void PageTrade::OnBtGetMyOfferList()
{
	auto func_obj_normal = GetCurNormalS();
	if (func_obj_normal)
	{
		GetIoService().post([this](){
			VMP_BEGIN_EX;
			my_offer_list_.DeleteAllItems();
			auto func_obj = GetCurNormalS();
			if ( !func_obj )
				return;
			func_obj->GetSessionOper().ThisMemCall(nullptr, "GetMyOfferList", nullptr);
			VMP_END;
		});
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
	}
}

void PageTrade::OnBtGetOtherOfferItems()
{
	auto func_obj_normal = GetCurNormalS();
	if (func_obj_normal)
	{
		GetIoService().post([this](){
			VMP_BEGIN_EX;
			other_offer_items_.DeleteAllItems();
			auto func_obj = GetCurNormalS();
			if ( !func_obj )
				return;
			func_obj->GetSessionOper().ThisMemCall(nullptr, "GetOtherOfferItems", nullptr);
			VMP_END;
		});
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
	}
}

FuncObj_NormalS* PageTrade::GetCurNormalS()
{
	auto page_ac = PageAccount_List::GetInstance();
	if ( !page_ac )
	{
		assert(false);
		return nullptr;
	}
	return page_ac->GetNormalS(cur_notify_item_);
}

void PageTrade::SetMyOfferListCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	my_offer_total_.SetWindowText(str_tmp);
}

void PageTrade::SetOtherOfferItemsCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	other_offer_total_.SetWindowText(str_tmp);
}

void PageTrade::SetCurNotifyItem( int item )
{
	VMP_BEGIN_EX;
	cur_notify_item_ = item;
	VMP_END;
}

void PageAccount::OnBtOkStoretdIp()
{
	VMP_BEGIN;
	UpdateData();
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return;
	tab_option->ConnectCoordinate();
	VMP_END;
}


void PageAccount::OnBtRunFirstScript()
{
	const auto kMaxItem = list_ctrl_.GetItemCount();
	for ( auto i = 0; i != kMaxItem; ++i )
	{
		auto funcobj = list_ctrl_.GetNormalS(i);
		if (funcobj)
		{
			list_ctrl_.StartItemScript(i);
			break;
		}
	}
}

int PageAccount::GetCurNotifyItem() const
{
	return cur_notify_item_;
}

void PageAccount::RefreshCurNotifyItem()
{
	CString str_tmp;
	if ( cur_notify_item_ < 0 )
		cur_notify_item_ = -1;
	else if ( !list_ctrl_.GetNormalS(cur_notify_item_) )
		cur_notify_item_ = -1;
	else
	{
		str_tmp.Format(_T("%d"), cur_notify_item_);
	}

	edit_cur_notify_item_.SetWindowText(str_tmp);
}

void PageAccount::SetCurNotifyItem( int cur_item )
{
	cur_notify_item_ = cur_item;
	RefreshCurNotifyItem();
}

FuncObj_NormalS* PageAccount::GetCurNotifyFuncObj()
{
	if ( cur_notify_item_ < 0 )
		return nullptr;
	return list_ctrl_.GetNormalS(cur_notify_item_);
}

void PageAccount::CopyMovies( const CPath& movie_path )
{
	using namespace std;
	auto res_data = LoadRcdataRes((LPCTSTR)nullptr, _T("BLACK.BIK"));
	if ( !res_data.first || res_data.second <= 0 )
	{
		assert(false);
		return;
	}
	const std::string file_path = CT2CA(movie_path.m_strPath);
	if ( !DoExtensionFileItem(file_path, ".bik", [&file_path, res_data]( const boost::filesystem::directory_iterator& it ){
		const auto& file_path = it->path();
		const auto& file_name = file_path.filename().string();
		if ( 0 == _stricmp(file_name.c_str(), "black.bik") )
			return;
		const auto& path_file = it->path().string();
		ofstream fstm(path_file, ios_base::out | ios_base::trunc | ios_base::binary);
		fstm.write((const char*)res_data.first, res_data.second);
		fstm.close();
	}) )
	{
		assert(false);
		return;
	}
	return;
}

void PageAccount::OnSelchangeRoleMaxCnt()
{
	auto& single_cfg = stSingletonCfg::GetMe();
	single_cfg.role_max_cnt_ = combo_role_max_cnt_.GetCurSel() + 1;
}

void PageAccount::OnChangeLoginDelay()
{
	CString str_tmp;
	edit_login_delay_.GetWindowText(str_tmp);
	auto login_delay = _tstoi(str_tmp);
	auto& cfg = stSingletonCfg::GetMe();
	cfg.login_delay_ = login_delay;
}

IMPLEMENT_DYNCREATE(PageSkill, TablePage)
BEGIN_MESSAGE_MAP(PageSkill, TablePage)
	ON_BN_CLICKED(IDC_BT_REFRESH_SKILL_INFO, &PageSkill::OnBtRefreshSkillInfo)
END_MESSAGE_MAP()

PageSkill::PageSkill() : TablePage(IDD_DLG_PAGE_SKILL)
{

}

void PageSkill::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SKILL_INFO, list_skills_);
	DDX_Control(pDX, IDC_EDIT_SKILLS_TOTAL_CNT, edit_skill_total_cnt_);
}

void PageSkill::SetSkillTotalCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	edit_skill_total_cnt_.SetWindowText(str_tmp);
}

IMPLEMENT_DYNCREATE(PageGameObjs, TablePage)
BEGIN_MESSAGE_MAP(PageGameObjs, TablePage)
	ON_BN_CLICKED(IDC_BT_REFRESH_GAME_OBJS, &PageGameObjs::OnBtRefreshGameObjs)
END_MESSAGE_MAP()

PageGameObjs::PageGameObjs() : TablePage(IDD_DLG_PAGE_GAME_OBJS)
{

}

void PageGameObjs::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_GAME_OBJS, list_game_objs_);
	DDX_Control(pDX, IDC_EDIT_GAME_OBJS_TOTAL_CNT, edit_gameobj_total_cnt_);
}

void PageGameObjs::SetGameObjTotalCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	edit_gameobj_total_cnt_.SetWindowText(str_tmp);
}

IMPLEMENT_DYNCREATE(PageItems, TablePage)
BEGIN_MESSAGE_MAP(PageItems, TablePage)
	ON_BN_CLICKED(IDC_BT_REFRESH_PAGE_ITEMS, &PageItems::OnBtRefreshPageItems)
END_MESSAGE_MAP()

PageItems::PageItems() : TablePage(IDD_DLG_PAGE_ITEMS)
{

}

void PageItems::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAGE_ITEMS, list_items_);
	DDX_Control(pDX, IDC_EDIT_PAGE_ITEM_TOTAL_CNT, edit_item_total_cnt_);
}


void PageSkill::OnBtRefreshSkillInfo()
{
	GetIoService().post([this](){
		auto page_ac = PageAccount::GetInstance();
		if ( !page_ac )
		{
			assert(false);
			return;
		}
		auto func_obj = page_ac->GetCurNotifyFuncObj();
		if ( !func_obj )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "当前帐号通信项不存在连接";
			return;
		}
		list_skills_.DeleteAllItems();
		auto& session_oper = func_obj->GetSessionOper();
		Json::Value json_array;
		session_oper.ThisMemCallSync(nullptr, "GetSkillsInfo", nullptr, JsonEcho::MakeResult(json_array, json_array));
		if (json_array.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "技能信息为空";
			return;
		}
		CList_Skills::stSkillInfo skill_info;
		for ( const auto& json_v : json_array )
		{
			if ( !skill_info.ParseFromJson(json_v) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "解析技能信息失败";
				assert(false);
				return;
			}
			list_skills_.AddSkillInfo(skill_info);
		}
		SetSkillTotalCnt(json_array.size());
	});
}


void PageGameObjs::OnBtRefreshGameObjs()
{
	GetIoService().post([this](){
		auto page_ac = PageAccount::GetInstance();
		if ( !page_ac )
		{
			assert(false);
			return;
		}
		auto func_obj = page_ac->GetCurNotifyFuncObj();
		if ( !func_obj )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "当前帐号通信项不存在连接";
			return;
		}
		list_game_objs_.DeleteAllItems();
		auto& session_oper = func_obj->GetSessionOper();
		Json::Value json_array;
		session_oper.ThisMemCallSync(nullptr, "GetGameObjsInfo", nullptr, JsonEcho::MakeResult(json_array, json_array));
		if (json_array.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "游戏对象信息为空";
			return;
		}
		CList_GameObjs::stGameObjInfo gameobj_info;
		for ( const auto& json_v : json_array )
		{
			if ( !gameobj_info.ParseFromJson(json_v) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "解析游戏对象信息失败";
				assert(false);
				return;
			}
			list_game_objs_.AddGameObjInfo(gameobj_info);
		}
		SetGameObjTotalCnt(json_array.size());
	});
}

void PageItems::OnBtRefreshPageItems()
{
	GetIoService().post([this](){
		auto page_ac = PageAccount::GetInstance();
		if ( !page_ac )
		{
			assert(false);
			return;
		}
		auto func_obj = page_ac->GetCurNotifyFuncObj();
		if ( !func_obj )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "当前帐号通信项不存在连接";
			return;
		}
		list_items_.DeleteAllItems();
		auto& session_oper = func_obj->GetSessionOper();
		Json::Value json_array;
		session_oper.ThisMemCallSync(nullptr, "GetAllItems", nullptr, JsonEcho::MakeResult(json_array, json_array));
		if (json_array.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "物品信息为空";
			return;
		}
		//CList_Items::stItem item_info;
		//for ( const auto& json_v : json_array )
		{
			/*if ( !item_info.ParseFromJson(json_v) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "解析物品信息失败";
				assert(false);
				return;
			}
			const auto& json_tab = json_v["tab_cate"];
			if ( json_tab.isNull() || !json_tab.isIntegral() )
			{
				assert(false);
				return;
			}
			list_items_.AddItem(item_info, (GType::enTabCategory)json_tab.asUInt());*/
		}
		//SetItemsTotalCnt(json_array.size());
	});
}

void PageItems::SetItemsTotalCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	edit_item_total_cnt_.SetWindowText(str_tmp);
}

IMPLEMENT_DYNCREATE(PageRoom, TablePage)
BEGIN_MESSAGE_MAP(PageRoom, TablePage)
	ON_BN_CLICKED(IDC_BT_REFRESH_ROOM_INFO, &PageRoom::OnBtRefreshRoomInfo)
END_MESSAGE_MAP()

PageRoom::PageRoom() : TablePage(IDD_DLG_PAGE_ROOM_INFO)
{

}

void PageRoom::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAGE_ROOM_INFO, list_room_);
}

IMPLEMENT_DYNCREATE(PageTask, TablePage)
BEGIN_MESSAGE_MAP(PageTask, TablePage)
END_MESSAGE_MAP()

PageTask::PageTask() : TablePage(IDD_DLG_PAGE_TASK)
{

}

void PageTask::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PAGE_TASK, list_task_);
}

void PageRoom::OnBtRefreshRoomInfo()
{
	GetIoService().post([this](){
		auto page_ac = PageAccount::GetInstance();
		if ( !page_ac )
		{
			assert(false);
			return;
		}
		auto func_obj = page_ac->GetCurNotifyFuncObj();
		if ( !func_obj )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "当前帐号通信项不存在连接";
			return;
		}
		list_room_.DeleteAllItems();
		auto& session_oper = func_obj->GetSessionOper();
		Json::Value json_room;
		session_oper.ThisMemCallSync(nullptr, "GetRoomInfo", nullptr, JsonEcho::MakeResult(json_room, json_room));
		if (json_room.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "物品信息为空";
			return;
		}
		/*CList_Room::stRoomInfo room_info;
		if ( !room_info.ParseFromJson(json_room) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "解析物品信息失败";
			assert(false);
			return;
		}
		list_room_.SetRoomInfo(room_info);*/
	});
}

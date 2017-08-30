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
#include <Common/Toolhelp.h>
#include <boost/filesystem/operations.hpp>
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
	,
	{
		_T("物品信息"), GET_RUNTIME_CLASS(PageItemInfo)
	},
	{
		_T("球员信息"), GET_RUNTIME_CLASS(PagePlayerInfo)
	}
#ifdef _WG_TEST
	,
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
	tab_idx_ = -1;
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
	auto parent = P_CAST<CTabOption*>(GetParent());
	assert(parent);
	if ( !parent )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "得不到CTabOption父对象";
	}
	return *parent;
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

void TablePage::SetTabIdx( int tab_idx )
{
	tab_idx_ = tab_idx;
	assert(tab_idx >= 0);
}

int TablePage::GetTabIdx() const
{
	return tab_idx_;
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
	ON_BN_CLICKED(IDC_CHECK_AUTO_SAVE_EP, &PageAccount::OnBnSaveEp)
	ON_BN_CLICKED(IDC_BT_SET_SAVE_EP_PATH, &PageAccount::OnBnSetEpFilePath)
	ON_BN_CLICKED(IDC_CHECK_AUTO_RUN_CONSOLE, &PageAccount::OnBnAutoRunConsoleAt12Time)
END_MESSAGE_MAP()

PageAccount::PageAccount() : TablePage(IDD_DLG_ACCOUNT)
{
	stored_ip_ = 0;
}

void PageAccount::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ACCOUNT_INFO, list_ctrl_);
	DDX_Control(pDX, IDC_COMBO_CRACK_CAPTCHA, combo_crack_catpcha_);
	DDX_IPAddress(pDX, IDC_IPADDRESS_STORED_IP, stored_ip_);

	auto& single_cfg = stSingletonCfg::GetMe();
	int int_tmp = single_cfg.auto_save_ep_;
	DDX_Check(pDX, IDC_CHECK_AUTO_SAVE_EP, int_tmp);
	single_cfg.auto_save_ep_ = !!int_tmp;

	int_tmp = single_cfg.run_at_12_;
	DDX_Check(pDX, IDC_CHECK_AUTO_RUN_CONSOLE, int_tmp);
	single_cfg.run_at_12_ = !!int_tmp;
}

BOOL PageAccount::OnInitDialog()
{
	const auto& single_cfg = stSingletonCfg::GetMe();
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
	
	if (stSingletonCfg::GetMe().run_at_12_)
		SetRunConsoleAt12Time();

	SetIoTimer(1000 * 60 * 2, [this]( IoTimer* timer ){
		auto cnt = list_ctrl_.CalcConnectionsCnt();
		if (cnt > kGameConnectionMaxCnt || cnt > stSingletonCfg::GetMe().GetAllowMaxLoginCnt())
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
		LOG_O(Log_warning) << "目标测试dll不存在";
		return;
	}

	auto func_obj = GetItemNormal(combo_select_account_.GetCurSel());
	if ( !func_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "连接不存在";
		return;
	}
	P_Exec_TestLoadLib msg;
	strcpy_s(msg.str_dll_full_file_path_, singleton_cfg.test_dll_full_file_path_.c_str());
	func_obj->UserMsg(P_Exec_TestLoadLib::value,
		&msg, sizeof(msg));
}

void PageTest::OnBtUnloadLib()
{
	auto& singleton_cfg = stSingletonCfg::GetMe();
	if ( singleton_cfg.test_dll_full_file_path_.empty() ||
		!PathFileExistsA(singleton_cfg.test_dll_full_file_path_.c_str()) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "目标测试dll不存在";
		return;
	}
	
	auto func_obj = GetItemNormal(combo_select_account_.GetCurSel());
	if ( !func_obj )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "连接不存在";
		return;
	}
	P_Exec_TestUnloadLib msg;
	func_obj->UserMsg(P_Exec_TestUnloadLib::value,
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
	auto normal_s = GetItemNormal(combo_select_account_.GetCurSel());
	if (normal_s)
	{
		normal_s->GetJsonEcho().GlobalFuncCall("LuaTest", nullptr);
	}
}

void PageTest::OnBtReloadLua()
{
	auto normal_s = GetItemNormal(combo_select_account_.GetCurSel());
	if (normal_s)
	{
		normal_s->GetJsonEcho().GlobalFuncCall("LoadAndReloadLuaFiles", nullptr);
	}
}

void PageTest::OnFlushLogToTempFile()
{
	auto normal_s = GetItemNormal(combo_select_account_.GetCurSel());
	if (normal_s)
	{
		normal_s->GetJsonEcho().GlobalFuncCall("LogXml_FlushToTempFile", nullptr);
	}
}

void PageTest::OnBtRunScript()
{
	auto normal = GetItemNormal(combo_select_account_.GetCurSel());
	if ( !normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "连接不存在";
		return;
	}
	JsonEcho::ObjGetter getter("LuaScriptC_Instance", nullptr);
	Json::Value params;
	params[UINT(0)] = "普通号脚本.lua";
	params[1] = true;
	normal->GetJsonEcho().MemFuncCall(getter, "RunLuaScript", &params);
}

void PageTest::OnBtTerminateScript()
{
	auto normal = GetItemNormal(combo_select_account_.GetCurSel());
	if ( !normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "连接不存在";
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
	CPath path_fifa = injected_file_path;

	//覆盖之前关闭所有进程
	CToolhelp::TeminateAllProcessAsPath(path_fifa);

	//auto new_path_comp = path_fifa;
	auto path_tmp = path_fifa;
	path_tmp.Append(_T("components"));
	//new_path_comp.Append(_T("components__"));
	//std::rename(CT2CA(path_tmp), CT2CA(new_path_comp));
	{
		boost::filesystem::path bpath(path_tmp);
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);
		bpath = bpath.parent_path();
		bpath /= "TQM";
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);
		bpath = bpath.parent_path();
		bpath /= "TP3Helper.exe";
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);

		bpath = bpath.parent_path();
		bpath /= "TCLS";
		/*bpath /= "TPSvc.exe";
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);
		bpath = bpath.parent_path();*/
		bpath /= "bugreport.exe";
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);
		bpath = bpath.parent_path();
		bpath /= "Assistant.exe";
		/*if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);*/

		bpath = bpath.parent_path();
		bpath /= "TenProtect";
		bpath /= "TenSafe.exe";
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);

		bpath = bpath.parent_path();
		bpath /= "TenSafe_1.exe";
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);

		bpath = bpath.parent_path();
		bpath /= "TenSafe_2.exe";
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);

		bpath = bpath.parent_path();
		bpath = bpath.parent_path();
		bpath /= "Tenio/TenioDL/TenioDL.exe";
		if (boost::filesystem::exists(bpath))
			boost::filesystem::remove_all(bpath);
	}
	//自己的工作目录
	CString self_path;
	{
		TCHAR hijack_dll_full_file_path_src[MAX_PATH];
		VERIFY(GetModulePath(nullptr, hijack_dll_full_file_path_src, sizeof(hijack_dll_full_file_path_src)));
		self_path = hijack_dll_full_file_path_src;
	}
	
	//if ( !PathFileExists(hijack_dll_full_file_path) )
		//copy 劫持dll
	{
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("D3D9_DLL"), CPath(injected_file_path + _T("d3d9.dll"))));
		//copy to login
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("D3D9_DLL"), CPath(injected_file_path + kRelativeClient + _T("d3d9.dll"))));
	}
	//copy答码兔dll
	{
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("CRACK_CAPTCHA_API"), CPath(injected_file_path + kRelativeClient + _T("CrackCaptchaAPI.dll"))));
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("LOCAL_CAPTCHA_DLL"), CPath(injected_file_path + kRelativeClient + kQxAqDllName)));
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("DONOTHING"), CPath(injected_file_path + kRelativeClient + _T("bugreport.exe"))));
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
		XmlCfg::Save(hijack_dll_cfg, CT2CA(injected_file_path + kRelativeClient + stHijackDllCfg::GetCfgFileName()));
	}

	//copy lpk
	VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("LPKA_DLL"), CPath(injected_file_path + _T("lpk.dll"))));
	//VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("LPKB_DLL"), CPath(self_path + _T("lpkb.dll"))));
	{
		//删掉awesomium_process.exe
		CString str_file_path = injected_file_path + _T("awesomium_process.exe");
		DeleteFile(str_file_path);
		//添加awesomium_process.exe文件夹
		CreateDirectory(str_file_path, nullptr);

		//删掉fifazfSrv.exe
		str_file_path = injected_file_path + _T("fifazfSrv.exe");
		DeleteFile(str_file_path);
		//添加fifazfSrv.exe文件夹
		CreateDirectory(str_file_path, nullptr);

		//删掉fifalaunchersrv.exe
		str_file_path = injected_file_path + _T("fifalaunchersrv.exe");
		DeleteFile(str_file_path);
		CreateDirectory(str_file_path, nullptr);
	}

	//copy graphics.ini
	{
		const auto& singleton_cfg = stSingletonCfg::GetMe();
		/*//自己的工作目录
		CString self_path = GetModulePath(nullptr);
		VERIFY(CopyFile(self_path + _T("graphics.ini"), injected_file_path + _T("graphics.ini"), FALSE));*/
		std::ofstream fstr(CT2CA(injected_file_path + _T("graphics.ini")));
		fstr << "FULLSCREEN=0\n"
			"SCREEN_WIDTH=1024\n"
			"SCREEN_HEIGHT=768\n"
			"RENDERING_QUALITY=1\n"
			"BRIGHTNESS=2\n"
			"CONTRAST=1\n"
			"MSAA_MODE=0\n"
			"CROWD_HIDE=1\n"
			"DISABLE_VSYNC=1";
		fstr.flush();
	}

	//copy BugTrace
	{
		const auto kPathBugTrace = injected_file_path + _T("TCLS\\TenProtect\\BugTrace.dll");
		const auto kPathBugTrace_ = injected_file_path + _T("TCLS\\TenProtect\\BugTrace_.dll");
		SaveRcdataAsFile((LPCTSTR)nullptr, _T("BUG_TRACE"), CPath(kPathBugTrace));
		SaveRcdataAsFile((LPCTSTR)nullptr, _T("BUG_TRACE_"), CPath(kPathBugTrace_));
	}

	AutoRegLpk();
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
	DDX_Control(pDX, IDC_LIST_TEST_PLAYERS_INFO, list_players_);
	DDX_Control(pDX, IDC_LIST_TEST_ITEM_INFO, list_items_);
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
	auto func_obj_normal = GetItemNormal(combo_select_account_.GetCurSel());
	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ToCall(BC_MakeP("GetPlayersInfo"));
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "连接不存在";
	}
}

FuncObj_NormalS* PageTest::GetItemNormal( int item )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if (list_ctrl)
		return list_ctrl->GetNormalS(item);
	return nullptr;
}


void PageTest::OnBtLoginSucceed()
{
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return;
	tab_option->SetLoginSucceed(true);
	DateTimeLocal end_time;
	end_time += Poco::Timespan(10, 0, 0, 0, 0);
	tab_option->SetUserEndTime(end_time);
	tab_option->AddAddrData(0x015D496C);
	tab_option->AddAddrData(0x00F6A5F0);
	tab_option->AddAddrData(0x00F6A690);
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "测试之成功登陆了";
}


void PageTest::OnBtGetItemsInfo()
{
	auto func_obj_normal = GetItemNormal(combo_select_account_.GetCurSel());
	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ToCall(BC_MakeP("GetItemsInfo"));
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
	return GetItemNormal(combo_select_account_.GetCurSel());
}

CList_ItemsSingle& PageTest::GetCtrlItem()
{
	return list_items_;
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
			func_obj->GetSessionOper().ToCall(BC_MakeP("GetMyOfferList"));
			VMP_END;
		});
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "连接不存在";
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
			func_obj->GetSessionOper().ToCall(BC_MakeP("GetOtherOfferItems"));
			VMP_END;
		});
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "连接不存在";
	}
}

FuncObj_NormalS* PageTrade::GetCurNormalS()
{
	auto page_ac = PageAccount_List::GetInstance();
	if ( !page_ac )
		return nullptr;
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

void PageAccount::OnBnSaveEp()
{
	UpdateData();
}

void PageAccount::OnBnSetEpFilePath()
{
	CFileDialog file_dlg(TRUE, nullptr, nullptr, OFN_HIDEREADONLY, 
		_T("*.txt|*.txt||"));
	if (file_dlg.DoModal())
	{
		const auto& file_path = file_dlg.GetPathName();
		if (PathFileExists(file_path))
		{
			auto& singleton_cfg = stSingletonCfg::GetMe();
			singleton_cfg.ep_file_path_ = file_path;		
		}
	}
}

void PageAccount::OnBnAutoRunConsoleAt12Time()
{
	UpdateData();
	if (stSingletonCfg::GetMe().run_at_12_)
		SetRunConsoleAt12Time();
}

void PageAccount::SetRunConsoleAt12Time()
{
	AutoRunAtSystemBegin("tjgcs", "autorun");
}

IMPLEMENT_DYNCREATE(PageItemInfo, TablePage)
BEGIN_MESSAGE_MAP(PageItemInfo, TablePage)
	ON_BN_CLICKED(IDC_BT_TEST_GET_ITEMS_INFO, &PageItemInfo::OnBtGetItemsInfo)
END_MESSAGE_MAP()

void PageItemInfo::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TEST_ITEM_TOTAL_CNT, items_total_);
	DDX_Control(pDX, IDC_LIST_TEST_ITEM_INFO, list_items_);
	DDX_Control(pDX, IDC_EDIT_CUR_NOTIFY_ITEM, edit_notify_item_);
}

PageItemInfo::PageItemInfo() : TablePage(IDD_DLG_ITEM_INFO)
{
	notify_item_ = -1;
}

CList_ItemsSingle& PageItemInfo::GetCtrlItem()
{
	return list_items_;
}

void PageItemInfo::SetItemsTotalCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	items_total_.SetWindowText(str_tmp);
}

void PageItemInfo::OnBtGetItemsInfo()
{
	list_items_.DeleteAllItems();
	if ( notify_item_ < 0 )
		return;
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
		return;
	auto func_obj_normal = list_ctrl->GetNormalS(notify_item_);
	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ToCall(BC_MakeP("GetItemsInfo"));
	}
}

void PageItemInfo::SetNofityItem( int notify_item )
{
	notify_item_ = notify_item;
	if ( notify_item < 0 )
		edit_notify_item_.SetWindowText(_T(""));
	else
	{
		CString str_tmp;
		str_tmp.Format(_T("%d"), notify_item_);
		edit_notify_item_.SetWindowText(str_tmp);
	}
}

IMPLEMENT_DYNCREATE(PagePlayerInfo, TablePage)
BEGIN_MESSAGE_MAP(PagePlayerInfo, TablePage)
	ON_BN_CLICKED(IDC_BT_TEST_GET_PLAYERS_INFO, &PagePlayerInfo::OnBtGetPlayersInfo)
END_MESSAGE_MAP()

void PagePlayerInfo::OnBtGetPlayersInfo()
{
	list_players_.DeleteAllItems();
	if ( notify_item_ < 0 )
		return;
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
		return;
	auto func_obj_normal = list_ctrl->GetNormalS(notify_item_);
	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ToCall(BC_MakeP("GetPlayersInfo"));
	}
}

PagePlayerInfo::PagePlayerInfo() : TablePage(IDD_DLG_PLAYER_INFO)
{
	notify_item_ = -1;
}

CList_PlayersSingle& PagePlayerInfo::GetCtrlPlayer()
{
	return list_players_;
}

void PagePlayerInfo::SetItemsTotalCnt( int cnt )
{
	CString str_tmp;
	str_tmp.Format(_T("%d"), cnt);
	players_total_.SetWindowText(str_tmp);
}

void PagePlayerInfo::SetNofityItem( int notify_item )
{
	notify_item_ = notify_item;
	if ( notify_item < 0 )
		edit_notify_item_.SetWindowText(_T(""));
	else
	{
		CString str_tmp;
		str_tmp.Format(_T("%d"), notify_item_);
		edit_notify_item_.SetWindowText(str_tmp);
	}
}

void PagePlayerInfo::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEST_PLAYERS_INFO, list_players_);
	DDX_Control(pDX, IDC_EDIT_TEST_PLAYERS_CNT, players_total_);
	DDX_Control(pDX, IDC_EDIT_CUR_NOTIFY_ITEM, edit_notify_item_);
}

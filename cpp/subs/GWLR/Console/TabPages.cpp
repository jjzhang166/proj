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
#include <CmnMix/Cmn_TemplateUtility.h>
#include <Common/Toolhelp.h>
#include "../CrackData/CD_CommonFiles/CD_AntiCrack.h"
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

stMakeTabPages g_TabPageMap[stMakeTabPages::kPagesMax] = {
#ifdef __PROJ_GWLR_MBP_
	{
		_T("帳號"), GET_RUNTIME_CLASS(PageAccount)
	}
	,
	{
		_T("較億"), GET_RUNTIME_CLASS(PageTrade)
	},
	{
		_T("豬億實鄉"), GET_RUNTIME_CLASS(PageRemark)
	}
#else
	{
		_T("帐号信息"), GET_RUNTIME_CLASS(PageAccount)
	}
	,
	{
		_T("交易"), GET_RUNTIME_CLASS(PageTrade)
	},
	{
		_T("注意事项"), GET_RUNTIME_CLASS(PageRemark)
	}
#endif
#ifdef _WG_TEST
	/*,
	{
		_T("全局设置"), GET_RUNTIME_CLASS(PageSetting)
	}*/
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
	ON_BN_CLICKED(IDC_CHECK_RIGHT_BT_HIT, &PageAccount::OnBnRightBtHit)
	ON_BN_CLICKED(IDC_CHECK_RUN_CONSOLE_AT_3, &PageAccount::OnBnAutoRunConsoleAt3Time)
	ON_BN_CLICKED(IDC_CHECK_STOP_LOGIN_AT_BAN_AC, &PageAccount::OnBtStopLoginAtBanAc)
	ON_BN_CLICKED(IDC_CHECK_USE_XUN_LEI, &PageAccount::OnBtUseXunLei)
	ON_BN_CLICKED(IDC_CHECK_USE_YOU_HUA, &PageAccount::OnBtUseYouHua)
	ON_BN_CLICKED(IDC_CHECK_LOGIN_AT_DISCONNECT_RIGHT_NOW, &PageAccount::OnBtLoginAtDisconnectRightNow)
	ON_BN_CLICKED(IDC_CHECK_OPEN_VILLAGE_PATH, &PageAccount::OnBtOpenVillagePath)
	ON_BN_CLICKED(IDC_CHECK_IGNORE_PWD_ERR, &PageAccount::OnBtIgnorePwdErr)
	ON_BN_CLICKED(IDC_BT_AUTO_DIAL, &PageAccount::OnBtAutoDial)
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

	auto& single_cfg = stSingletonCfg::GetMe();
	int int_tmp = single_cfg.right_bt_hit_;
	DDX_Check(pDX, IDC_CHECK_RIGHT_BT_HIT, int_tmp);
	single_cfg.right_bt_hit_ = !!int_tmp;

	int_tmp = single_cfg.run_at_3_;
	DDX_Check(pDX, IDC_CHECK_RUN_CONSOLE_AT_3, int_tmp);
	single_cfg.run_at_3_ = !!int_tmp;

	int_tmp = single_cfg.stop_login_at_ban_ac_;
	DDX_Check(pDX, IDC_CHECK_STOP_LOGIN_AT_BAN_AC, int_tmp);
	single_cfg.stop_login_at_ban_ac_ = !!int_tmp;

	int_tmp = single_cfg.is_use_xun_lei_;
	DDX_Check(pDX, IDC_CHECK_USE_XUN_LEI, int_tmp);
	single_cfg.is_use_xun_lei_ = !!int_tmp;

	int_tmp = single_cfg.is_use_you_hua_;
	DDX_Check(pDX, IDC_CHECK_USE_YOU_HUA, int_tmp);
	single_cfg.is_use_you_hua_ = !!int_tmp;

	int_tmp = single_cfg.is_login_at_disc_;
	DDX_Check(pDX, IDC_CHECK_LOGIN_AT_DISCONNECT_RIGHT_NOW, int_tmp);
	single_cfg.is_login_at_disc_ = !!int_tmp;

	int_tmp = single_cfg.open_village_path_;
	DDX_Check(pDX, IDC_CHECK_OPEN_VILLAGE_PATH, int_tmp);
	single_cfg.open_village_path_ = !!int_tmp;

	int_tmp = single_cfg.ignore_pwd_err_;
	DDX_Check(pDX, IDC_CHECK_IGNORE_PWD_ERR, int_tmp);
	single_cfg.ignore_pwd_err_ = !!int_tmp;
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
	combo_crack_catpcha_.InsertString(stCrackCaptchaMode::kCCM_DaMaTu, _T("打码兔"));
	combo_crack_catpcha_.InsertString(stCrackCaptchaMode::kCCM_Local, _T("本地答题"));
	combo_crack_catpcha_.SetCurSel(single_cfg.crack_captcha_mode_.ccm_);

	SetRunConsoleAt3Time();

	SetIoTimer(1000 * 60 * 5, [this]( IoTimer* timer ){
		VMP_BEGIN_EX;
		timer->Continue();
		auto cnt = list_ctrl_.CalcConnectionsCnt();
		if (cnt > ConnectCntInfo::kGameConnectionMaxCnt || cnt > stSingletonCfg::GetMe().GetAllowMaxLoginCnt())
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
		VMP_END;
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
		single_cfg.login_delay_ = 5 * 60;
	str_tmp.Format(_T("%d"), single_cfg.login_delay_);
	edit_login_delay_.SetWindowText(str_tmp);

	//YouHuaSetting();
	//////////////////////////////////////////////////////////////////////////
#ifdef __PROJ_GWLR_MBP_
	CString self_path;
	{
		TCHAR self_path_buffer[MAX_PATH];
		VERIFY(GetModulePath(nullptr, self_path_buffer, sizeof(self_path_buffer)));
		self_path = self_path_buffer;
	}
	CPath path_tmp(self_path);
	path_tmp.Append(_T("anti-check.sys"));
	VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("ANTI_CHECK_SYS"), path_tmp));
	path_tmp.RemoveFileSpec();
	path_tmp.Append(_T("anti-check.dll"));
	VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("ANTI_CHECK_DLL"), path_tmp));
	VERIFY(LibraryLoader::LoadTheLibrary(path_tmp));
#endif
	//////////////////////////////////////////////////////////////////////////
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
	func_obj->UserMsg(P_Exec_TestLoadLib::value,
		&msg, sizeof(msg));
}

void PageTest::OnBtUnloadLib()
{
/*
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
	func_obj->UserMsg(P_Exec_TestUnloadLib::value,
		&msg, sizeof(msg));*/

	char szbuffer[MAX_PATH] = { 0 };
	GetSystemDirectoryA(szbuffer, sizeof(szbuffer));
	CPathA path_sys = szbuffer;
	if (!path_sys.IsDirectory())
	{
		assert(false);
		throw CmnExitIoThreadException("dsfafdqwre");
	}
	std::string str_tmp = kLoginHelperName;
	str_tmp += ".dll";
	path_sys.Append(str_tmp.c_str());
	auto cur_process_id = GetCurrentProcessId();
	CToolhelp::DoAllProcessIdUntil([&path_sys, cur_process_id](DWORD prcess_id){
		if (cur_process_id == prcess_id)
			return false;
		EjectLibA(prcess_id, path_sys);
		return false;
	});
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
	/*auto normal_s = GetCurNormalS();
	if (normal_s)
	{
		normal_s->GetJsonEcho().GlobalFuncCall("LuaTest", nullptr);
	}*/
	PageAccount_List::GetInstance()->Test();
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
	params[UINT(0)] = "智能选择脚本运行.lua";
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
	auto injected_file_path = GetPathFromFullFilePath(CString(singleton_cfg.game_full_file_path_));
	CPath path_tcls(injected_file_path);
	path_tcls.RemoveFileSpec();
	path_tcls.RemoveFileSpec();
	auto path_tmp = path_tcls;
	path_tmp.Append(_T("Tools"));
	auto path_tools = path_tmp;

	path_tcls.RemoveFileSpec();
	path_tcls.RemoveFileSpec();
	//覆盖之前关闭所有进程
	CToolhelp::TeminateAllProcessAsPath(path_tcls);

	auto path_componets = path_tcls;
	path_componets.Append(_T("components"));
	path_tcls.Append(_T("TCLS"));
	path_tmp = path_tcls;
	//自己的工作目录
	CString self_path;
	{
		TCHAR hijack_dll_full_file_path_src[MAX_PATH];
		VERIFY(GetModulePath(nullptr, hijack_dll_full_file_path_src, sizeof(hijack_dll_full_file_path_src)));
		self_path = hijack_dll_full_file_path_src;
	}
	//copy 劫持dll
	{
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("D3D9_DLL"), CPath(injected_file_path + _T("d3d9.dll"))));
		path_tmp.Append(_T("d3d9.dll"));
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("D3D9_DLL"), path_tmp));
		path_tmp.RemoveFileSpec();
		path_tmp.Append(_T("CrackCaptchaAPI.dll"));
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("CRACK_CAPTCHA_API"), path_tmp));
		path_tmp.RemoveFileSpec();
		path_tmp.Append(kQxAqDllName);
		SaveRcdataAsFile((LPCTSTR)nullptr, _T("LOCAL_CAPTCHA_DLL"), path_tmp);
		path_tmp.RemoveFileSpec();
		const TCHAR* kBugReport = _T("bugreport.exe");
		path_tmp.Append(kBugReport);
		SaveRcdataAsFile((LPCTSTR)nullptr, _T("DONOTHING"), path_tmp);
		SaveRcdataAsFile((LPCTSTR)nullptr, _T("DONOTHING"), CPath(injected_file_path + kBugReport));
	}
	//copy hijack dll的配置文件
	{
		stHijackProfile hijack_dll_cfg;
		hijack_dll_cfg.SetProfile(std::string(CT2CA(injected_file_path + hijack_dll_cfg.GetCfgFileName())));
		hijack_dll_cfg.injected_game_name_ = CT2CA(GetFileNameFromFullFilePath(singleton_cfg.game_full_file_path_));
		hijack_dll_cfg.inject_dll_full_path_ = CT2CA(self_path + _T("game.dll"));
		hijack_dll_cfg.Save();
		//login.dll
		path_tmp = path_tcls;
		path_tmp.Append(hijack_dll_cfg.GetCfgFileName());
		hijack_dll_cfg.SetProfile(std::string(CT2CA(path_tmp.m_strPath)));
		hijack_dll_cfg.injected_game_name_ = "Client.exe";
		hijack_dll_cfg.inject_dll_full_path_ = CT2CA(self_path + _T("login.dll"));
		hijack_dll_cfg.Save();
	}

	//复制游戏设置
	path_tmp.Append(_T("config"));
	path_tmp.Append(_T("GameSetting.xml"));
	VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("GAME_SETTING"), path_tmp));

	//copy BugTrace
	{
		path_tmp = path_tcls;
		path_tmp.Append(_T("TenProtect"));
		path_tmp.Append(_T("BugTrace.dll"));
		SaveRcdataAsFile((LPCTSTR)nullptr, _T("BUG_TRACE"), path_tmp);
		path_tmp.RemoveFileSpec();
		path_tmp.Append(_T("BugTrace_.dll"));
		SaveRcdataAsFile((LPCTSTR)nullptr, _T("BUG_TRACE_"), path_tmp);
	}

	//屏幕设置
	//////////////////////////////////////////////////////////////////////////
	{
		HKEY the_key = nullptr;
		if ( ERROR_SUCCESS != RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\User Shell Folders"), 0, KEY_QUERY_VALUE, &the_key) )
		{
			assert(false);
			return;
		}
		if ( !the_key )
		{
			assert(false);
			return;
		}
		TCHAR szbuffer[MAX_PATH] = { 0 };
		TCHAR sztmp[] = _T("Personal\0");
		DWORD the_type = REG_EXPAND_SZ, the_size = sizeof(szbuffer);
		VERIFY(ERROR_SUCCESS == RegQueryValueEx(the_key, sztmp, 0, &the_type, (BYTE*)szbuffer, &the_size));
		RegCloseKey(the_key);

		TCHAR szbuffer2[MAX_PATH] = { 0 };
		ExpandEnvironmentStrings(szbuffer, szbuffer2, _countof(szbuffer2));
		CPath the_path(szbuffer2);
		LOG_O(Log_trace) << "游戏配置目录：" << szbuffer2;
		the_path.Append(_T("MonsterHunterOnline\\COMMON"));
		CreateDepthDirectory(the_path);
		if (the_path.IsDirectory())
		{
			the_path.Append(_T("user.cfg"));
			VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("USER_CFG"), the_path));
		}
		else
		{
			assert(false);
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	//copy lpk
/*
#ifdef PREP_UNOPEN_SHORTEST_PATH
	VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("LPKA_DLL"), CPath(injected_file_path + _T("lpk.dll"))));
#else*/
	//VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("LPKA_PRI_DLL"), CPath(injected_file_path + _T("lpk.dll"))));
//#endif
	//VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("LPKA_DLL"), CPath(injected_file_path + _T("lpk.dll"))));
	/*auto str_path_lpka = self_path + _T("lpka.dll");
	::CopyFile(str_path_lpka, CPath(injected_file_path + _T("lpka.dll")), FALSE);*/

	path_tmp.m_strPath = injected_file_path;
	path_tmp.RemoveFileSpec();
	path_tmp.RemoveFileSpec();
	path_tmp.Append(_T("IIPS\\lpk.dll"));
	VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("LPKIIPS_DLL"), path_tmp));
	//::CopyFile(str_path_lpka, path_tmp, FALSE);

	//删掉CrossProxy.exe
	CString str_file_path = injected_file_path + _T("Cross\\CrossProxy.exe");
	DeleteFile(str_file_path);
	CreateDirectory(str_file_path, nullptr);

	//PCMgr.dat
	/*path_componets.Append(_T("PCMgr.dat"));
	SaveRcdataAsFile((LPCTSTR)nullptr, _T("DONOTHING"), path_componets);*/
	auto new_path_comp = path_componets;
	new_path_comp.RemoveFileSpec();
	new_path_comp.Append(_T("components__"));
	std::rename(CT2CA(path_componets), CT2CA(new_path_comp));

	//删掉tqos_reporter.exe
	path_tools.Append(_T("tqos_reporter.exe"));
	::DeleteFile(path_tools);
	CreateDirectory(path_tools, nullptr);

	//最后再注册LPK
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
	DDX_Control(pDX, IDC_EDIT_TEST_PLAYERS_CNT, players_total_);
	DDX_Control(pDX, IDC_EDIT_TEST_ITEM_TOTAL_CNT, items_total_);
}


void PageAccount::OnSelchangeCrackCaptcha()
{
	auto crack_captcha_mode = (stCrackCaptchaMode::enCrackCaptchaMode)combo_crack_catpcha_.GetCurSel();
	auto& single_cfg = stSingletonCfg::GetMe();
	switch (crack_captcha_mode)
	{
	case stCrackCaptchaMode::kCCM_DaMaTu:
		{
			CrackCaptcha_SetAccountDlg dlg(this);
			dlg.DoModal();
		}
		break;
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
	assert(false);
	/*auto func_obj_normal = GetCurNormalS();
	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ThisMemCall(nullptr, "GetPlayersInfo", nullptr);
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
	}*/
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
	tab_option->ClearAddrDatas();
	//tab_option->AddAddrData(kStaticAddrData);
	/*tab_option->AddAddrData(0x00F42370);
	tab_option->AddAddrData(0x00F42410);*/
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "测试之成功登陆了";
}


void PageTest::OnBtGetItemsInfo()
{
	assert(false);
	/*auto func_obj_normal = GetCurNormalS();
	if (func_obj_normal)
	{
		func_obj_normal->GetSessionOper().ThisMemCall(nullptr, "GetItemsInfo", nullptr);
	}*/
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
	DDX_Control(pDX, IDC_BT_GET_MY_OFFER_LIST, bt_refresh_my_list_);
	DDX_Control(pDX, IDC_BT_GET_OTHER_OFFER_ITEMS, bt_refresh_other_list_);
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
			func_obj->GetSessionOper().ToCall(BC_MakeP("GetMySellList"));
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
			func_obj->GetSessionOper().ToCall(BC_MakeP("GetOtherSellInfo"));
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

BOOL PageTrade::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;
	/*bt_refresh_my_list_.ShowWindow(SW_HIDE);
	bt_refresh_other_list_.ShowWindow(SW_HIDE);*/
	return TRUE;
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

void PageAccount::OnBnRightBtHit()
{
	UpdateData();
	NotifyRightBtHit();
}

void PageAccount::NotifyRightBtHit()
{
	const auto kItemMax = list_ctrl_.GetItemCount();
	auto& cfg = stSingletonCfg::GetMe();
	for ( auto i = 0; i < kItemMax; ++i )
	{
		auto nor = list_ctrl_.GetNormalS(i);
		if (nor)
			nor->SetRightBtHit(cfg.right_bt_hit_);
	}
}

void PageAccount::SetRunConsoleAt3Time()
{
	const char* kRegStr = "tjgcs";
	if (stSingletonCfg::GetMe().run_at_3_)
		AutoRunAtSystemBegin(kRegStr, "autorun");
	else
		DeleteAutoRunAtSystemBegin(kRegStr);
}

void PageAccount::OnBnAutoRunConsoleAt3Time()
{
	UpdateData();
	SetRunConsoleAt3Time();
}

void PageAccount::OnBtStopLoginAtBanAc()
{
	UpdateData();
}

void PageAccount::OnBtUseXunLei()
{
	UpdateData();
}

void PageAccount::OnBtUseYouHua()
{
	UpdateData();
	YouHuaSetting();
}

void PageAccount::YouHuaSetting()
{
	auto you_hua = stSingletonCfg::GetMe().is_use_you_hua_;
	list_ctrl_.DoItemFuncObj([you_hua](FuncObj_NormalS& nor){
		nor.SetYouHuan(you_hua);
	});
	//you_hua_event_ = ::CreateEventA(nullptr, FALSE, FALSE, kUseYouHuaEventName);
}

void PageAccount::OnBtLoginAtDisconnectRightNow()
{
	UpdateData();
}

void PageAccount::OnBtOpenVillagePath()
{
	UpdateData();
	NotifyOpenVillagePath();
}

void PageAccount::NotifyOpenVillagePath()
{
	const auto kItemMax = list_ctrl_.GetItemCount();
	auto& cfg = stSingletonCfg::GetMe();
	for ( auto i = 0; i < kItemMax; ++i )
	{
		auto nor = list_ctrl_.GetNormalS(i);
		if (nor)
			nor->SetOpenVillagePath(cfg.open_village_path_);
	}
}

void PageAccount::OnBtIgnorePwdErr()
{
	UpdateData();
}

void PageAccount::OnBtAutoDial()
{
	if ( IDOK == dlg_auto_dial_.DoModal() )
	{

	}
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
	DDX_Control(pDX, IDC_EDIT_PAGE_ITEM_TOTAL_CNT, edit_item_total_cnt_);
}


void PageSkill::OnBtRefreshSkillInfo()
{
	assert(false);
	/*GetIoService().post([this](){
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
		auto& session_oper = func_obj->GetSessionOper();
		Json::Value json_array;
		session_oper.ThisMemCallSync(nullptr, "GetSkillsInfo", nullptr, JsonEcho::MakeResult(json_array, json_array));
		if (json_array.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "技能信息为空";
			return;
		}

	});*/
}


void PageGameObjs::OnBtRefreshGameObjs()
{
	assert(false);
	/*GetIoService().post([this](){
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
		auto& session_oper = func_obj->GetSessionOper();
		Json::Value json_array;
		session_oper.ThisMemCallSync(nullptr, "GetGameObjsInfo", nullptr, JsonEcho::MakeResult(json_array, json_array));
		if (json_array.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "游戏对象信息为空";
			return;
		}
		
		SetGameObjTotalCnt(json_array.size());
	});*/
}

void PageItems::OnBtRefreshPageItems()
{
	assert(false);
	/*GetIoService().post([this](){
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
			/ *if ( !item_info.ParseFromJson(json_v) )
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
			list_items_.AddItem(item_info, (GType::enTabCategory)json_tab.asUInt());* /
		}
		//SetItemsTotalCnt(json_array.size());
	});*/
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
}

void PageRoom::OnBtRefreshRoomInfo()
{
	assert(false);
	/*GetIoService().post([this](){
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
		auto& session_oper = func_obj->GetSessionOper();
		Json::Value json_room;
		session_oper.ThisMemCallSync(nullptr, "GetRoomInfo", nullptr, JsonEcho::MakeResult(json_room, json_room));
		if (json_room.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "物品信息为空";
			return;
		}
		/ *CList_Room::stRoomInfo room_info;
		if ( !room_info.ParseFromJson(json_room) )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "解析物品信息失败";
			assert(false);
			return;
		}
		list_room_.SetRoomInfo(room_info);* /
	});*/
}

IMPLEMENT_DYNCREATE(PageRemark, TablePage)
BEGIN_MESSAGE_MAP(PageRemark, TablePage)
END_MESSAGE_MAP()

PageRemark::PageRemark() : TablePage(IDD_DLG_PAGE_REMARK)
{

}

void PageRemark::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_REMARK, static_remark_);
}

BOOL PageRemark::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;
	LPCTSTR remark_str = _T("\
1.请关闭防火墙，卸载：一切杀毒软件、腾讯QQ(腾讯TM没事)、电脑管家、一切腾讯类其他游戏(比如疾风、天刀等)，然后重启机器。要尽量保持电脑上的纯净环境。\r\n\
2.新系统或者新用户第一次使用本辅助，需要先选择游戏目录，然后成功登录辅助，最后重新启动电脑。\r\n\
3.在更换辅助新版本或游戏更新后，一定要重新选择一下游戏路径。\r\n\
4.如果出现非法模块的情况，请仔细阅读几遍所有注意事项，并按说明操作之，然后必须要重启一次机器，最后重新选择一下游戏路径。\r\n\
5.辅助帐号、密码，打码帐号、密码，都只支持数字与字母的组合，请不要使用特殊字符，否则会出现辅助登陆不上，或卡游戏登陆器的问题。\r\n\
6.请自行设置打码兔帐号，并把软件加到打码兔的白名单中。打码兔返回错误的错误码解释，请到这个网站去查看：http://wiki.dama2.com/index.php?n=ApiDoc.ErrDef。另外，错误码-210极有可能是你的打码兔帐号被封了，请重新注册个打码帐号试试。\r\n\
7.充值的时候，机器上不要插U盘、USB之类的移动设备，否则会出现莫名需要解绑问题。\r\n\
8.每次解绑扣3天，所有请使用推荐的系统，否则查不出的问题会被判定为系统问题。\r\n\
9.建议：更换新版本的时候，先不要覆盖到旧版本中，把旧版本目录下的“Cfg”目录拷贝到新版本目录下，然后开启辅助，选择游戏路径，先挂一个小号测试一下，没有问题再更换大号继续挂着。\r\n\
10.每次登陆账号选择批量上号，角色状态显示待登陆则才会自动换下一个账号。\r\n\
11.虽然本辅助已经非常稳定，但特别情况下或因游戏更新造成的卡点问题或其他不正常行为，请及时截图，\n并尽可能反馈当时的情况，信息提供得越多越详细越好(描述一定要清楚)，如若必要，请提供一下游戏帐号。\r\n\
12.挂着游戏的时候，如果遇到一些提示信息，比如“把该副本告诉代理”，就请及时关掉该游戏，不要再继续挂着该帐号了。\r\n\
13.是否启用优化的选项如果变了，下一个启动的游戏才会有变更后的效果。\r\n\
14.建议大家下载个EditPlus编辑软件,既可以用它搜索、查看、编辑某一目录下的所有脚本文件，又可用它查看所有log日志输出文件，用以排查错误。如果你想再高端些，可以下载个时时跟踪log的DbgView与专业编辑Lua的LuaForWindows。\r\n\
		");
	static_remark_.SetWindowText(remark_str);
	return TRUE;
}

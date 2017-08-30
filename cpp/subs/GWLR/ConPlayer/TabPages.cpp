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
#include <Common/TemplateUtility.h>
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

stMakeTabPages g_TabPageMap[stMakeTabPages::kPagesMax] = {
	{
		_T(""), GET_RUNTIME_CLASS(PageAccount)
	}
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
	ON_BN_CLICKED(IDC_BT_SET_MULTI_HIT, &PageAccount::OnBtSetMultiHit)
	ON_BN_CLICKED(IDC_BT_DISCONNECT, &PageAccount::OnBtDisconnect)
END_MESSAGE_MAP()

PageAccount::PageAccount() : TablePage(IDD_DLG_ACCOUNT)
{
	nor_ = nullptr;
}

void PageAccount::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SET_MULTI_HIT, edit_multi_hit_);
	DDX_Control(pDX, IDC_EDIT_THE_RUN_STATE, edit_run_state_);
	DDX_Control(pDX, IDC_STATIC_THE_MULTI_HIT, static_multi_hit_);
	
	auto& sin_cfg = SingleProfile::GetMe();
	DDX_Text(pDX, IDC_EDIT_SET_MULTI_HIT, sin_cfg.multi_hit_);
}

BOOL PageAccount::OnInitDialog()
{
	if ( !__super::OnInitDialog() )
		return FALSE;

	SetDlgItemText(IDC_EDIT_ACCOUNT_GAME_PATH, CA2CT(SingleProfile::GetMe().game_path_.c_str()));
	CString str_tmp;
	str_tmp.Format(_T("倍攻范围(%d-%d)："), kMultiHitRate_Min, kMultiHitRate_Max);
	static_multi_hit_.SetWindowText(str_tmp);
	return TRUE;
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
	assert(false);
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
			auto& singleton_cfg = SingleProfile::GetMe();

			singleton_cfg.game_path_ = CT2CA(file_path);
			SetDlgItemText(IDC_EDIT_ACCOUNT_GAME_PATH, file_path);
			singleton_cfg.Save();
			//设置注入配置表
			SetHijackDllCfg();
		}
	}
}

void PageAccount::SetHijackDllCfg()
{
	const auto& singleton_cfg = SingleProfile::GetMe();
	//被注入的游戏路径
	auto injected_file_path = GetPathFromFullFilePath(CString(CA2CT(singleton_cfg.game_path_.c_str())));

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
	}
	//copy hijack dll的配置文件
	//if ( !PathFileExists(hijack_cfg_full_file_path) )
	{
		stHijackProfile hijack_dll_cfg;
		hijack_dll_cfg.SetProfile(std::string(CT2CA(injected_file_path + hijack_dll_cfg.GetCfgFileName())));
		hijack_dll_cfg.injected_game_name_ = CT2CA(GetFileNameFromFullFilePath(CString(CA2CT(singleton_cfg.game_path_.c_str()))));
#ifdef _DEBUG
		hijack_dll_cfg.inject_dll_full_path_ = CT2CA(self_path + _T("game.dll"));
#else
		hijack_dll_cfg.inject_dll_full_path_ = CT2CA(injected_file_path + _T(GAME_MODULE_NAME)_T(".dll"));
#endif
		hijack_dll_cfg.Save();
	}
	//copy game dll
	{
#ifndef _DEBUG
		VERIFY(SaveRcdataAsFile((LPCTSTR)nullptr, _T("GAME_DLL"), CPath(injected_file_path + _T(GAME_MODULE_NAME)_T(".dll"))));
#endif
	}
}

void PageAccount::OnSize( UINT nType, int cx, int cy )
{
	__super::OnSize(nType, cx, cy);
}

void PageAccount::OnBtSetMultiHit()
{
	UpdateData();
	auto& sin_cfg = SingleProfile::GetMe();
	sin_cfg.multi_hit_ = std::min(sin_cfg.multi_hit_, kMultiHitRate_Max);
	sin_cfg.multi_hit_ = std::max(sin_cfg.multi_hit_, kMultiHitRate_Min);
	CString str_tmp;
	str_tmp.Format(_T("%d"), sin_cfg.multi_hit_);
	edit_multi_hit_.SetWindowText(str_tmp);

	if ( !nor_ )
		return;
	nor_->GetSessionOper().ThisMemCall(nullptr, "SetMultiHit", &JsonEcho::MakeParams(sin_cfg.multi_hit_));
}

void PageAccount::OnBtDisconnect()
{
	if (nor_)
		nor_->Disconnect();
}

void PageAccount::SetSession( FuncObj_NormalS* nors )
{
	nor_ = nors;
	if (nor_)
	{
		edit_run_state_.SetWindowText(_T("正常"));
	} 
	else
	{
		edit_run_state_.SetWindowText(_T("断开"));
	}
}

FuncObj_NormalS* PageAccount::GetSession()
{
	return nor_;
}

void PageTest::OnDropdownSelectAccount()
{
}

void PageTest::DoDataExchange( CDataExchange* pDX )
{
	__super::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DLG_TEXT_COMBO_SELECT_ACCOUNT, combo_select_account_);
	DDX_Control(pDX, IDC_EDIT_TEST_PLAYERS_CNT, players_total_);
	DDX_Control(pDX, IDC_EDIT_TEST_ITEM_TOTAL_CNT, items_total_);
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
	DateTimeLocal end_time;
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
	assert(false);
	return nullptr;
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
	
}


void PageGameObjs::OnBtRefreshGameObjs()
{
	
}

void PageItems::OnBtRefreshPageItems()
{
	
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
	
}

#include "stdafx.h"
#include "ListCtrls.h"
#include "resource.h"
#include "Common/UsefulFunc.h"
#include "DlgUnion.h"
#include "Socket/IoTimer.h"
#include "Protocol/P_Defines.h"
#include "Common/Toolhelp.h"
#include "TabOption.h"
#include "CoordinateC.h"
#include <Common/UsefulFunc.h>
#include <protect/Protect.h>
#include "Console.h"
#include <Communicate/CG_Defines.h>
#include <Communicate/CL_Defines.h>
#include <Communicate/CGL_Defines.h>
#include <Common/TemplateUtility.h>
//////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(ListNormal, CListCtrl)
	ON_WM_RBUTTONDOWN()
	ON_BN_CLICKED(ID_MENU_BEGIN, OnMenuItemStartItemScript)
	ON_BN_CLICKED(ID_MENU_STOP, OnMenuItemStopScript)
	ON_COMMAND(ID_MENU_ADD_ACCOUNT, &ListNormal::OnMenuAddAccount)
	ON_COMMAND(ID_MENU_DELETE_ACCOUNT, &ListNormal::OnMenuDeleteAccount)
	ON_COMMAND(ID_MENU_LIST_SHOW_GAME_WINDOW, &ListNormal::OnMenuListShowGameWindow)
	ON_COMMAND(ID_MENU_LIST_SET_SCRIPT, &ListNormal::OnMenuListSetScript)
	ON_COMMAND(ID_MENU_SHOW_ACCOUNT_INFO, &ListNormal::OnMenuShowAccountInfo)
	ON_COMMAND(ID_MENU_START_ALL_SCRIPT, &ListNormal::OnMenuStartAllScript)
	ON_COMMAND(ID_MENU_STOP_ALL_SCRIPT, &ListNormal::OnMenuStopAllScript)
	ON_COMMAND(ID_MENU_SHOW_ALL_ACCOUNT_INFO, &ListNormal::OnMenuShowAllAccountInfo)
	ON_COMMAND(ID_MENU_CLOSE_PROCESS, &ListNormal::OnMenuCloseProcess)
	ON_COMMAND(ID_MENU_CLOSE_ALL_PROCESS, &ListNormal::OnCloseAllProcess)
	ON_COMMAND(ID_Menu_BEGIN_LOGIN, &ListNormal::OnMenuBeginLogin)
	ON_COMMAND(ID_Menu_SELECT_SERVICE, &ListNormal::OnMenuSelectService)
	ON_COMMAND(ID_Menu_LOGIN_SOME_GAME, &ListNormal::OnMenuLoginSomeGame)
	ON_COMMAND(ID_Menu_AUTO_RUN_SCRIPT, &ListNormal::OnMenuAutoRunScript)
	ON_COMMAND(ID_Menu_ALL_AUTO_RUN_SCRIPT, &ListNormal::OnMenuAllAutoRunScript)
	ON_COMMAND(ID_Menu_STOP_LOGIN_ALL, &ListNormal::OnMenuStopLoginAll)
	ON_COMMAND(ID_Menu_SELECT_GAME_AC_TYPE, &ListNormal::OnMenuSelectGameAcType)
	ON_COMMAND(ID_Menu_AS_NOTIFY_OBJ, &ListNormal::OnMenuAsNotifyObj)
	ON_COMMAND(ID_Menu_CancelProhibit, &ListNormal::OnMenuCancelprohibit)
	ON_COMMAND(ID_Menu_NOTIFY_AS_THIS, &ListNormal::OnMenuNotifyAsThis)
END_MESSAGE_MAP()

void ListNormal::PreSubclassWindow()
{
	//style
	{
		DWORD dwStyle = GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;		//选中某行使整行高亮（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_GRIDLINES;			//网格线（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_CHECKBOXES;			//item前生成checkbox控件
		SetExtendedStyle(dwStyle);	//设置扩展风格
	}

	const int kColumnWidth = 75;
	InsertColumn(kSubitemAccount, _T("帐号"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemPassword, _T("密码"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemService, _T("服务器"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemRunState, _T("运行状态"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemGameAcType, _T("号的种类"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemAutoRunScript, _T("自动运行脚本"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemCurHp, _T("HP"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemRoleName, _T("角色名"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemCurRoleIdx, _T("角色索引"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemRoleLevel, _T("角色等级"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemFatigueData, _T("疲劳值"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemCurMapRoomName, _T("地图或房间"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemCurGold, _T("当前金币"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemCurScript, _T("应用的脚本"), LVCFMT_LEFT, kColumnWidth + 40);
	InsertColumn(kSubitemGainedGold, _T("获得的金币"), LVCFMT_LEFT, kColumnWidth + 30);
	InsertColumn(kSubitemJobName, _T("职业"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSubitemScriptOutputInfo, _T("脚本输出信息"), LVCFMT_LEFT, kColumnWidth + 50);
	InsertColumn(kSubitemLastNotifyTime, _T("最后通信时间"), LVCFMT_LEFT, kColumnWidth + 50);

	//Insert_Item(_T("jfkdal"), _T("jfdka"));
}

void ListNormal::OnRButtonDown( UINT nFlags, CPoint point )
{
	__super::OnRButtonDown(nFlags, point);
	if ( nullptr != GetFirstSelectedItemPosition() )
	{
		ShowPopupMenu(IDR_MENU_ACCOUNT_LIST, this);
	}
	else
	{
		ShowPopupMenu(IDR_MENU_ACCOUNT_LIST_ALL, this);
	}
}

void ListNormal::StartItemScript( int item )
{
	if ( !GetCheck(item) )
		return;

	auto normal = GetNormalS(item);
	if ( !normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
		return;
	}
	
	const std::string kScriptName = CT2CA(GetItemText(item, kSubitemCurScript));
	/*JsonEcho::ObjGetter getter("LuaScriptC_Instance", nullptr);
	Json::Value params;
	params[UINT(0)] = kScriptName;
	params[1] = true;
	normal->GetJsonEcho().MemFuncCall(getter, "RunLuaScript", &params);*/
	//SetItemText(item, kSubitemScriptOutputInfo, _T("等待运行脚本"));
	normal->GetLuaScript().RunLuaScript(kScriptName);
	return;
}

void ListNormal::StopItemScript( int item )
{
	auto normal = GetNormalS(item);
	if ( !normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
		return;
	}
	/*JsonEcho::ObjGetter getter("LuaScriptC_Instance", nullptr);
	Json::Value params;
	params[UINT(0)] = true;
	normal->GetJsonEcho().MemFuncCall(getter, "ForceTerminateScript", &params);*/
	normal->GetLuaScript().TerminateScript();
	SetItemText(item, kSubitemScriptOutputInfo, _T("等待停止脚本"));
}

void ListNormal::Delete_Item( int item )
{
	StopItemScript(item);

	auto normal = GetNormalS(item);
	if (normal)
		normal->Disconnect();

	std::string str_account = CT2CA(GetItemAccount(item));
	std::string service_name = CT2CA(GetItemServiceName(item));
	SaveCfgByAccount(str_account, service_name);
	specific_cfgs_.erase(std::make_pair(str_account, service_name));
	DeleteItem(item);
}

void ListNormal::OnMenuItemStartItemScript()
{
	auto pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		StartItemScript(GetNextSelectedItem(pos));
	}
}

void ListNormal::OnMenuItemStopScript()
{
	auto pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		StopItemScript(GetNextSelectedItem(pos));
	}
}

void ListNormal::Insert_Item( LPCTSTR account, LPCTSTR password, LPCTSTR service_name )
{
	//判断是否存在了
	if ( FindItemByAccount(account, service_name) >= 0 )
		return;

	//比对密码
	if ( !CheckSamePwd(account, password) )
		return;

	const auto kItemMax = GetItemCount();
	InsertItem(kItemMax, account);
	SetItemText(kItemMax, kSubitemPassword, password);
	SetItemText(kItemMax, kSubitemService, service_name);

	std::string str_account = CT2CA(account);
	std::string str_ser_name = CT2CA(service_name);
	assert(!FindSpecificCfg(str_account, str_ser_name));
	
	if (stSpecificCfg::HasSpecificCfg(str_account, game_service_mgr_.GetServiceIdx(str_ser_name)))
	{
		GenSpecificCfg(str_account, str_ser_name);
	}

	auto& cfg = GetAccountCfg(str_account, str_ser_name);
	SetItemText(kItemMax, kSubitemCurScript, CA2CT(cfg.script_name_.c_str()));
	SetCheck(kItemMax);

	LPCTSTR kText = _T("是");
	if ( !cfg.auto_run_script_ )
		kText = _T("否");
	SetItemText(kItemMax, kSubitemAutoRunScript, kText);

	if ( kGAT_Normal == cfg.game_ac_type_ )
		kText = _T("普通号");
	else if ( kGAT_Stored == cfg.game_ac_type_ )
		kText = _T("仓库号");
	else if ( kGAT_TransStation == cfg.game_ac_type_ )
		kText = _T("中转站");
	SetItemText(kItemMax, kSubitemGameAcType, kText);

	kText = nullptr;
	//帐号状态
	if ( stSpecificCfg::kPR_BanAc == cfg.ac_pro_state_.pro_reason_ )
	{
		kText = _T("帐号被封");
		AddProhibitItem(str_account, str_ser_name, stSpecificCfg::kPR_BanAc);
	}
	else if ( stSpecificCfg::kPR_RolesUseOut == cfg.ac_pro_state_.pro_reason_ )
	{
		if ( !const_cast<stSpecificCfg&>(cfg).FindFirstFatigueValidRole() )
		{
			kText = _T("角色刷完");
			AddProhibitItem(str_account, str_ser_name, stSpecificCfg::kPR_RolesUseOut);
		}
		else
		{
			auto cfg = FindSpecificCfg(str_account, str_ser_name);
			if (cfg)
			{
				cfg->ac_pro_state_.pro_reason_ = stSpecificCfg::kPR_Valid;
			}
		}
	}
	if (kText)
		SetItemText(kItemMax, kSubitemRunState, kText);
}

int ListNormal::FindItemByAccount( LPCTSTR account, LPCTSTR service_name ) const
{
	const auto kItemMax = GetItemCount();
	for ( auto i = 0; i < kItemMax; ++i )
	{
		if ( GetItemAccount(i) == account && GetItemServiceName(i) == service_name )
			return i;
	}
	return -1;
}

void ListNormal::SetItemFuncObj( int item, FuncObj_NormalS* func_obj )
{
	if ( item >= 0 && item < GetItemCount() )
	{
		if ( GetItemData(item) != reinterpret_cast<DWORD_PTR>(func_obj) )
		{
			SetItemData(item, reinterpret_cast<DWORD_PTR>(func_obj));
			if (func_obj)
			{
				//初始化
				func_obj->SetServiceIdx(GetItemServiceIdx(item));
				func_obj->SetGameAcType(GetItemGameAcType(item));

				func_obj->Accept(this);
				//DoLoginOne();
			}
			else
			{
				str_run_state_ = _T("");
			}
			SetItemText(item, kSubitemRunState, str_run_state_);
		}
	}
}

void ListNormal::Visit( FuncObj_NormalS* session_account )
{
	str_run_state_ = _T("登陆状态");
}

void ListNormal::Visit( FuncObj_CreateTrainerS* player )
{
	str_run_state_ = _T("游戏中");
}

void ListNormal::FreeItemByFuncObj( FuncObj& func_obj )
{
	const auto kItemMax = GetItemCount();
	DWORD_PTR obj_ptr = (DWORD_PTR)&func_obj;
	for ( auto i = 0; i < kItemMax; ++i )
	{
		if ( GetItemData(i) == obj_ptr )
		{
			SetItemData(i, 0);
			str_run_state_ = _T("");
			SetItemText(i, kSubitemRunState, str_run_state_);

			std::string account = CT2CA(GetItemAccount(i));
			std::string service_name = CT2CA(GetItemServiceName(i));
			auto cfg_it = specific_cfgs_.find(std::make_pair(account, service_name));
			if ( cfg_it != specific_cfgs_.end() )
			{
				auto service_idx = game_service_mgr_.GetServiceIdx(service_name);
				if ( service_idx >= 0 )
					cfg_it->second.SaveAsAccount(account, service_idx);
			}

			auto& index_set = login_helper_.get<LoginHelper::tagItem>();
			auto login_helper = index_set.find(i);
			if ( login_helper != index_set.end() )
				index_set.erase(login_helper);
			return;
		}
	}
}

FuncObj* ListNormal::GetItemFuncObj( int item )
{
	if ( item >= 0 && item < GetItemCount() )
	{
		return (FuncObj*)GetItemData(item);
	}
	return nullptr;
}


void ListNormal::OnMenuAddAccount()
{
	DlgAddAccountHelper dlg_add;
	dlg_add.DoModal();
}


void ListNormal::OnMenuDeleteAccount()
{
	auto pos = GetFirstSelectedItemPosition();
	std::vector<int> items;
	while (pos)
	{
		items.push_back(GetNextSelectedItem(pos));
	}
	std::reverse(items.begin(), items.end());
	for ( const auto item : items )
	{
		Delete_Item(item);
	}
}


void ListNormal::OnMenuListShowGameWindow()
{
	auto pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		auto normal_s = GetNormalS(GetNextSelectedItem(pos));
		if (normal_s)
		{
			auto& json_echo = normal_s->GetSessionOper();

			VERIFY(json_echo.ThisMemCall(nullptr, "ForegroundTheWindow", nullptr));
		}
	}
}


void ListNormal::OnMenuListSetScript()
{
	DlgSelectScript dlg_select_script(this);
	if ( dlg_select_script.DoModal() == IDOK )
	{
		auto pos = GetFirstSelectedItemPosition();
		while (pos)
		{
			auto item = GetNextSelectedItem(pos);
			if ( item < 0 )
				continue;

			std::string str_account = CT2CA(GetItemAccount(item));
			std::string str_ser_name = CT2CA(GetItemServiceName(item));
			auto specific_cfg = GenSpecificCfg(str_account, str_ser_name);
			assert(specific_cfg);
			if ( !specific_cfg )
				continue;
			specific_cfg->script_name_ = CT2CA(dlg_select_script.GetSelectedScriptName());
			specific_cfg->SaveAsAccount(str_account, game_service_mgr_.GetServiceIdx(str_ser_name));

			SetItemText(item, kSubitemCurScript, dlg_select_script.GetSelectedScriptName());
		}
	}
}

stSpecificCfg* ListNormal::FindSpecificCfg( const std::string& ac, const std::string& service_name )
{
	auto it = specific_cfgs_.find(std::make_pair(ac, service_name));
	if ( it != specific_cfgs_.end() )
		return &(it->second);
	else
	{
		return nullptr;
	}
}

void ListNormal::SaveCfgByAccount( const std::string& ac, const std::string& service_name )
{
	auto it = specific_cfgs_.find(std::make_pair(ac, service_name));
	if ( it != specific_cfgs_.end() )
	{
		auto idx = game_service_mgr_.GetServiceIdx(service_name);
		if ( idx >= 0 )
			it->second.SaveAsAccount(ac, idx);
	}
}

void ListNormal::SaveAllSpecificCfg()
{
	auto it_end = specific_cfgs_.end();
	for ( auto it = specific_cfgs_.begin(); it != it_end; ++it )
	{
		auto idx = game_service_mgr_.GetServiceIdx(it->first.second);
		if ( idx >= 0 )
			it->second.SaveAsAccount(it->first.first, idx);
	}
}

stSpecificCfg* ListNormal::GenSpecificCfg( const std::string& ac, const std::string& service_name )
{
	auto it = specific_cfgs_.find(std::make_pair(ac, service_name));
	if ( it != specific_cfgs_.end() )
		return &(it->second);

	CString cstr_ac = CA2CT(ac.c_str());
	CString cstr_ser_name = CA2CT(service_name.c_str());
	auto item = FindItemByAccount(cstr_ac, cstr_ser_name);
	if ( item < 0 )
		return nullptr;

	stSpecificCfg specific_cfg = stDefaultCfg::GetMe();

	auto service_idx = game_service_mgr_.GetServiceIdx(service_name);
	if ( service_idx < 0 )
		return nullptr;

	if (stSpecificCfg::HasSpecificCfg(ac, service_idx))
	{
		specific_cfg.RestoreAsAccount(ac, service_idx);
	}

	auto kSpecificKey = std::make_pair(ac, service_name);
	auto new_it = specific_cfgs_.insert(std::move(SpecificCfgT::value_type(kSpecificKey, specific_cfg)));
	if ( new_it.first != specific_cfgs_.end() )
		return &new_it.first->second;
	return nullptr;
	/*it = specific_cfgs_.find(specific_key);
	if ( it != specific_cfgs_.end() )
		return &(it->second);
	return nullptr;*/
}

const stSpecificCfg& ListNormal::GetAccountCfg( const std::string& ac, const std::string& service_name )
{
	auto cfg = FindSpecificCfg(ac, service_name);
	if (cfg)
		return *cfg;
	else
	{
		return stDefaultCfg::GetMe();
	}
}

CString ListNormal::GetItemAccount( int item ) const
{
	return GetItemText(item, kSubitemAccount);
}


void ListNormal::OnMenuShowAccountInfo()
{
	AfxMessageBox(_T("现在还不支持，会是马上就会支持"));
}

FuncObj_NormalS* ListNormal::GetNormalS( LPCTSTR account )
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		if ( GetItemAccount(item) == account )
		{
			auto func_obj_normal = GetNormalS(item);
			if (func_obj_normal)
				return func_obj_normal;
		}
	}
	return nullptr;
}

FuncObj_NormalS* ListNormal::GetNormalS( int item )
{
	auto func_obj = GetItemFuncObj(item);
	if (func_obj)
	{
		return PointerCast<FuncObj_NormalS>(func_obj);
	}
	return nullptr;
}

FuncObj_NormalS* ListNormal::GetNormalS( const std::string& game_ac, int ser_idx )
{
	const auto kItemMax = GetItemCount();
	for ( auto i = 0;i != kItemMax; ++i )
	{
		auto func_obj = GetNormalS(i);
		if (func_obj)
		{
			if ( func_obj->GetGameAc() == game_ac && func_obj->GetServiceIdx() == ser_idx )
				return func_obj;
		}
	}
	return nullptr;
}

FuncObj_NormalS* ListNormal::GetNormalS( LPCTSTR ac, LPCTSTR service_name )
{
	/*auto item = FindItemByAccount(account, service_name);
	if ( item >= 0 )
	{
		return GetNormalS(item);
	}*/
	return nullptr;
}

PageAccount_List::PageAccount_List() /*: timer_clr_prohibit_(GetIoService())*/
{
	if (this_)
	{
		assert(false);
		return;
	}

	this_ = this;

	//5分钟检测一次
	SetIoTimer(5 * 60 * 1000, []( IoTimer* timer ){
		auto list_normal = PageAccount_List::GetInstance();
		if ( !list_normal )
		{
			assert(false);
			return;
		}
		assert(timer);
		DateTimeLocal time_now;
		auto& singleton_cfg = stSingletonCfg::GetMe();
		auto span_update = time_now - singleton_cfg.io_h_.last_check_roles_;
		if ( span_update.totalHours() >= 24 )
		{
			singleton_cfg.io_h_.last_check_roles_ = time_now;
			singleton_cfg.Save();
			list_normal->Timer_ClrProhibit();
			timer->Continue();
			return;
		}
		DateTimeLocal time_clear(time_now.year(), time_now.month(), time_now.day(), 6);
		if ( time_now > time_clear && singleton_cfg.io_h_.last_check_roles_ <= time_clear )
		{
			singleton_cfg.io_h_.last_check_roles_ = time_now;
			singleton_cfg.Save();
			list_normal->Timer_ClrProhibit();
			timer->Continue();
			return;
		}
		timer->Continue();
	});
}

PageAccount_List::~PageAccount_List()
{
	assert(this_);
	this_ = nullptr;
}

PageAccount_List* PageAccount_List::GetInstance()
{
	return this_;
}

void PageAccount_List::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto& sigleton_cfg = stSingletonCfg::GetMe();
	for ( const auto& item_account : sigleton_cfg.cfg_of_accounts_ )
	{
		const auto& account = item_account.first;
		const auto& password = item_account.second;
		Insert_Item(account, password, item_account.service_name_);
	}
}

void PageAccount_List::SaveAllAccountCfg()
{
	auto& cfg_of_accounts = stSingletonCfg::GetMe().cfg_of_accounts_;
	cfg_of_accounts.clear();
	const auto kItemMax = GetItemCount();
	//CString cstr_tmp;
	stSingletonCfg::AcountContT::value_type value_tmp;
	for ( auto item = 0; item != kItemMax; ++item )
	{
		value_tmp.first = GetItemAccount(item);
		value_tmp.second = GetItemText(item, kSubitemPassword);
		value_tmp.service_name_ = GetItemServiceName(item);
		cfg_of_accounts.push_back(value_tmp);
		/*cstr_tmp = GetItemText(item, kSubitemAccount);
		cfg_of_accounts[cstr_tmp] = GetItemText(item, kSubitemPassword);*/
	}
}

template <typename T>
class IterFuncObjNormalBase : public T{
public:
	IterFuncObjNormalBase( enGameAcType game_ac_type, int ser_idx ){
		game_ac_type_ = game_ac_type;
		ser_idx_ = ser_idx;
		cur_item_ = -1;
		auto list_normal = PageAccount_List::GetInstance();
		if (list_normal)
			max_item_cnt_ = list_normal->GetItemCount();
		else
			max_item_cnt_ = 0;
	}
	virtual void First(){
		cur_item_ = -1;
		NextValid();
	}
	virtual void Next(){
		NextValid();
	}
	virtual bool IsDone() const{
		assert(cur_item_ >= 0);
		return cur_item_ >= max_item_cnt_;
	}

private:
	void NextValid(){
		auto list_normal = PageAccount_List::GetInstance();
		if ( !list_normal )
		{
			cur_item_ = max_item_cnt_;
			return;
		}
		for ( ++cur_item_; cur_item_ < max_item_cnt_; ++cur_item_ )
		{
			auto func_obj_normal = list_normal->GetNormalS(cur_item_);
			if (func_obj_normal)
			{
				if ( func_obj_normal->GetGameAcType() == game_ac_type_ && func_obj_normal->GetServiceIdx() == ser_idx_ )
				{
					return;
				}
			}
		}
	}

protected:
	enGameAcType			game_ac_type_;
	int						ser_idx_;
	int						cur_item_;
	int						max_item_cnt_;
};
class IterFuncObjNormal : public IterFuncObjNormalBase<FuncObj_NormalS::Iterator>{
public:
	IterFuncObjNormal( enGameAcType game_ac_type, int ser_idx ) : IterFuncObjNormalBase(game_ac_type, ser_idx){}

public:
	virtual Item CurItem() const{
		auto list_normal = PageAccount_List::GetInstance();
		if ( !list_normal )
		{
			assert(false);
			return nullptr;
		}
		auto func_obj_normal = list_normal->GetNormalS(cur_item_);
		assert(func_obj_normal);
		return func_obj_normal;
	}
};
class IterJsonEchoBase : public IterFuncObjNormalBase<JsonEchoTrans::ReceiIterator>{
public:
	IterJsonEchoBase( enGameAcType game_ac_type, int ser_idx ) : IterFuncObjNormalBase(game_ac_type, ser_idx){}

public:
	virtual Item CurItem() const{
		auto list_normal = PageAccount_List::GetInstance();
		if ( !list_normal )
		{
			assert(false);
			return nullptr;
		}
		auto func_obj_normal = list_normal->GetNormalS(cur_item_);
		assert(func_obj_normal);
		if (func_obj_normal)
		{
			return &func_obj_normal->GetSessionOper();
		}
		return nullptr;
	}
};
FuncObj_NormalS::IteratorPtr PageAccount_List::CreateIterByGameAcType( enGameAcType game_ac_type, int service_idx )
{
	return FuncObj_NormalS::IteratorPtr (new IterFuncObjNormal(game_ac_type, service_idx));
}

JsonEchoTrans::ReceiIteratorPtr PageAccount_List::CreateReceiverIter( enGameAcType game_ac_type, int service_idx )
{
	return JsonEchoTrans::ReceiIteratorPtr (new IterJsonEchoBase(game_ac_type, service_idx));
}

int PageAccount_List::CalcConnectionsCnt()
{
	int cnt = 0;
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		cnt += !!GetNormalS(item);
	}
	return cnt;
}

void PageAccount_List::Timer_ClrProhibit()
{
	std::vector<int> vec_items;
	ProhibitT prohibit_another;
	for ( const auto& pro_item : prohibit_login_ )
	{
		if ( stSpecificCfg::kPR_RolesUseOut != pro_item.prohibit_reason_ )
			prohibit_another.insert(pro_item);
		else
		{
			auto cfg = FindSpecificCfg(pro_item.first, pro_item.second);
			if (cfg)
			{
				if ( !cfg->FindFirstFatigueValidRole() )
				{
					prohibit_another.insert(pro_item);
					continue;
				}
				cfg->ac_pro_state_.pro_reason_ = stSpecificCfg::kPR_Valid;
			}
			auto item = FindItemByAccount(pro_item.first, pro_item.second);
			if ( item >= 0 )
				vec_items.push_back(item);
		}
	}
	std::swap(prohibit_login_, prohibit_another);
	for ( const auto item : vec_items )
	{
		BeginLogin(item);
	}
}

PageAccount_List* PageAccount_List::this_ = nullptr;


void ListNormal::OnMenuStartAllScript()
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		StartItemScript(item);
	}
}


void ListNormal::OnMenuStopAllScript()
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		StopItemScript(item);
	}
}


void ListNormal::OnMenuShowAllAccountInfo()
{
	AfxMessageBox(_T("现在还不支持，会是马上就会支持"));
}

ListNormal::ListNormal()
{
}

void ListNormal::SetRunState( FuncObj_NormalS& func_obj_normal )
{
	auto item = FindItemByFuncObj(func_obj_normal);
	if ( item < 0 )
	{
		assert(false);
		return;
	}

	const auto& game_info = func_obj_normal.GetGameInfo();

	SetItemText(item, kSubitemRunState, game_info.running_state_);

	UpdateLastNotifyTime(item);
}

void ListNormal::SetScriptOutputInfo( FuncObj_NormalS& func_obj_normal, const char* output_info )
{
	auto item = FindItemByFuncObj(func_obj_normal);
	if ( item < 0 )
	{
		//assert(false);
		return;
	}
	SetItemText(item, kSubitemScriptOutputInfo, CA2CT(output_info));

	UpdateLastNotifyTime(item);
}

void ListNormal::UpdateLastNotifyTime( int item )
{
	CTime cur_time = CTime::GetCurrentTime();
	auto str_last_time = cur_time.Format(_T("%m-%d %H:%M:%S"));
	SetItemText(item, kSubitemLastNotifyTime, str_last_time);
}

ListNormal::stGameInfo::stGameInfo()
{
	cur_ep_ = 0;
	gain_total_ep_ = 0;
	match_cnt_ = 0;
	trainer_level_ = 0;
}


void ListNormal::OnMenuCloseProcess()
{
	auto pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		if ( !CloseGameProcess(GetNextSelectedItem(pos)) )
			continue;
	}
}


void ListNormal::OnCloseAllProcess()
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		CloseGameProcess(item);
	}
}

bool ListNormal::CloseGameProcess( int item )
{
	auto normal = GetNormalS(item);
	if ( !normal )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接不存在";
		return false;
	}
	normal->Disconnect();
	return true;
}

bool ListNormal::BeginLogin( int item )
{
	if ( !AddLoginItem(item) )
		return false;
	DoLoginOne();
	return true;
}

void ListNormal::OnMenuBeginLogin()
{
	auto pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		if ( !BeginLogin(GetNextSelectedItem(pos)) )
			continue;
	}
}

void ListNormal::OnMenuSelectService()
{
	SelectGameService dlg_select(this);
	if ( dlg_select.DoModal() == IDOK )
	{
		auto pos = GetFirstSelectedItemPosition();
		const auto& service = dlg_select.GetSelectedService();
		while (pos)
		{
			auto item = GetNextSelectedItem(pos);
			if ( item < 0 )
				continue;
			SetItemService(item, service);
		}
	}
}

void WriteLoginRecord( const CString& qq_num,  DWORD service_id, LPCTSTR full_file_name )
{
	const TCHAR* const kStrSection = _T("LoginUserRecord");
	const TCHAR* const kStrZero = _T("0");
	CString str_service_id, str_qq_num;
	str_service_id.Format(_T("%d"), service_id);
	str_qq_num = _T("QQ") + qq_num;
	WritePrivateProfileString(kStrSection, str_qq_num, str_service_id, full_file_name);
	WritePrivateProfileString(kStrSection, str_qq_num + _T("Mid"), kStrZero, full_file_name);
	WritePrivateProfileString(kStrSection, str_qq_num + _T("TH"), kStrZero, full_file_name);
}

bool ListNormal::DoLoginOne()
{
	//return true;
	decltype(login_helper_.begin()) it;
	decltype(prohibit_login_.end()) prohibit_it_end = prohibit_login_.end();
	for ( it = login_helper_.begin(); it != login_helper_.end(); ++it )
	{
		CString cstr_ac = GetItemAccount(it->item_);
		//该帐号是否已经登陆了
		if (GetNormalS(cstr_ac))
			continue;
		ProhibitT::value_type pro_item;
		pro_item.first = CT2CA(cstr_ac);
		pro_item.second = CT2CA(GetItemText(it->item_, kSubitemService));
		if ( prohibit_login_.find(pro_item) == prohibit_it_end )
		{
			break;
		}
	}
	if ( it == login_helper_.end() )
		return false;
	if (it->is_logining_)
	{
		return false;
	}

	const auto& singleton_cfg = stSingletonCfg::GetMe();
	if ( singleton_cfg.game_full_file_path_.GetLength() < 3 )
		return false;

	const auto& single_cfg = stSingletonCfg::GetMe();
	auto cur_login_cnt = GetLoginingCnt();
	if ( cur_login_cnt >= single_cfg.max_login_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "登陆数量为：" << single_cfg.max_login_ << "\t已不能再登陆了";
		return false;
	}
	if ( cur_login_cnt >= kGameConnectionMaxCnt )
	{
		//assert(false);
		return true;
	}

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	TCHAR buffer_file_path[MAX_PATH];
	_tcscpy_s(buffer_file_path, singleton_cfg.game_full_file_path_);

	{
		auto it = _tcsrchr(buffer_file_path, _T('\\'));
		assert(it);
		if (it)
		{
			*++it = 0;
		}
		else
		{
			assert(false);
			return false;
		}
	}

	//_tcscat(buffer_file_path, kRelativeClient);
	CPath path_tcls(buffer_file_path);
	{
		path_tcls.RemoveFileSpec();
		path_tcls.RemoveFileSpec();
		path_tcls.RemoveFileSpec();
		path_tcls.Append(_T("TCLS\\"));
	}
	//写LoginQ.dat配置文件
	{
		if ( it->item_ < 0 )
		{
			assert(false);
			return false;
		}

		std::string str_service_name = CT2CA(GetItemServiceName(it->item_));
		const auto kServiceId = game_service_mgr_.GetServiceId(str_service_name);
		if ( 0 == kServiceId )
		{
			assert(false);
			return false;
		}
		CString login_record = path_tcls.m_strPath;
		login_record += _T("config\\LoginQ.dat");
		WriteLoginRecord(GetItemAccount(it->item_), kServiceId, login_record);
	}

	TCHAR process_full_file_name[MAX_PATH];
	CString str_login_path = path_tcls.m_strPath + kLoginClientName;
	{
		_tcscpy_s(process_full_file_name, str_login_path);
	}
	CString str_tp_path = path_tcls.m_strPath + + _T("TenProtect\\");

	//关掉没关掉的进程
	CToolhelp::TeminateAllProcess(CPath(str_login_path));
	CToolhelp::TeminateAllProcess(CPath(str_tp_path + _T("TASLogin.exe")));

	if ( !CreateProcess(process_full_file_name, nullptr, nullptr, nullptr, 
		FALSE, 
		/*CREATE_NEW_PROCESS_GROUP*/CREATE_BREAKAWAY_FROM_JOB | CREATE_DEFAULT_ERROR_MODE,
		nullptr, buffer_file_path, &si, &pi) )
	{
		assert(false);
		return false;
	}

	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	
	auto login_helper = *it;
	login_helper.is_logining_ = true;
	login_helper.login_process_id_ = pi.dwProcessId;
	login_helper_.erase(it);

	//登陆计时
	login_helper.timer_login_ = CreateIoTimer(stSingletonCfg::GetMe().login_delay_ * 1000, [login_helper]( IoTimerPtr timer ){
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "登陆超时了";
		auto ctrl_list = PageAccount_List::GetInstance();
		if ( !ctrl_list )
			return;
		if ( !ctrl_list->FindLoginHelperByPid(login_helper.login_process_id_) )
			return;
		auto& login_set = ctrl_list->login_helper_.get<LoginHelper::tagItem>();
		auto it = login_set.find(login_helper.item_);
		if ( it == login_set.end() )
			return;
		if ( it->timer_login_.get() != timer.get() )
			return;
		if (ctrl_list->GetNormalS(login_helper.item_))
			return;
		login_set.erase(it);
		LoginHelper login_tmp;
		login_tmp.is_logining_ = false;
		login_tmp.item_ = login_helper.item_;
		login_tmp.login_process_id_ = 0;
		ctrl_list->login_helper_.insert(login_tmp);
		ctrl_list->DoLoginOne();
	}, true);
	
	assert(login_helper.timer_login_);
	login_helper_.insert(login_helper);

	//////////////////////////////////////////////////////////////////////////
	//连接协调服务
	auto tab_option = CTabOption::GetInstance();
	if (tab_option)
		tab_option->ConnectCoordinate();
	//////////////////////////////////////////////////////////////////////////
	return true;
}

int ListNormal::AcceptSession( DWORD client_proccess_id )
{
	auto& index_set = login_helper_.get<LoginHelper::tagLoginProcessId>();
	auto it = index_set.find(client_proccess_id);
	/*auto sz = index_set.size();
	DWORD process_id_tmp = 0;
	for ( auto it = index_set.begin(); it != index_set.end(); ++it )
	{
		process_id_tmp = it->login_process_id_;
	}*/
	if ( it == index_set.end() )
		return -1;
	
	if ( !it->session_login_.expired() )
		it->session_login_.lock()->Disconnect();
	auto item = it->item_;
	auto& login_item = const_cast<LoginHelper&>(*it);
	login_item.FreeLoginTimer();

	index_set.erase(it);
	assert(item >= 0);
	if ( item >= GetItemCount() )
		return -1;
	return item;
}

int ListNormal::FindItemByFuncObj( FuncObj& func_obj )
{
	const auto kItemMax = GetItemCount();
	DWORD_PTR obj_ptr = (DWORD_PTR)&func_obj;
	for ( auto i = 0; i < kItemMax; ++i )
	{
		if ( GetItemData(i) == obj_ptr )
		{
			return i;
		}
	}
	return -1;
}


void ListNormal::OnMenuLoginSomeGame()
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		BeginLogin(item);
	}
}

void ListNormal::OnMenuAutoRunScript()
{
	AutoRunScript_LoginSucceed dlg_run_script(this);
	if ( dlg_run_script.DoModal() == IDOK )
	{
		auto pos = GetFirstSelectedItemPosition();
		auto is_auto_run = dlg_run_script.IsAutoRunScript();
		while (pos)
		{
			auto item = GetNextSelectedItem(pos);
			if ( item < 0 )
				continue;
			SetAutoRunScript(item, is_auto_run);
		}
	}
}

void ListNormal::OnMenuAllAutoRunScript()
{
	AutoRunScript_LoginSucceed dlg_run_script(this);
	if ( dlg_run_script.DoModal() != IDOK )
		return;

	const auto kMaxItemCnt = GetItemCount();
	const auto kAutoRunScript = dlg_run_script.IsAutoRunScript();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		SetAutoRunScript(item, kAutoRunScript);
	}
}

void ListNormal::SetAutoRunScript( int item, bool auto_run_script )
{
	const std::string account = CT2CA(GetItemAccount(item));
	const std::string service_name = CT2CA(GetItemServiceName(item));
	auto specific_cfg = GenSpecificCfg(account, service_name);
	if ( !specific_cfg )
	{
		assert(false);
		return;
	}

	if ( specific_cfg->auto_run_script_ == auto_run_script )
		return;
	specific_cfg->auto_run_script_ = auto_run_script;
	specific_cfg->SaveAsAccount(account, game_service_mgr_.GetServiceIdx(service_name));
	LPCTSTR kText = _T("是");
	if ( !specific_cfg->auto_run_script_ )
		kText = _T("否");
	SetItemText(item, kSubitemAutoRunScript, kText);
}

void ListNormal::StartRunScript( FuncObj_NormalS& func_obj_normal )
{
	auto item = FindItemByFuncObj(func_obj_normal);
	if ( item < 0 )
	{
		assert(false);
		return;
	}
	StartItemScript(item);
}

LoginHelper* ListNormal::FindLoginHelperBySession( Session* login_session )
{
	//auto& index_set = login_helper_.get<LoginHelper::tagItem>();
	/*auto login_helper = index_set.find(login_session);
	if ( login_helper != index_set.end() )
		return const_cast<LoginHelper*>(&*login_helper);
	else
		return nullptr;*/
	for ( auto it = login_helper_.begin(); it != login_helper_.end(); ++it )
	{
		if ( !it->session_login_.expired() && it->session_login_.lock().get() == login_session )
			return const_cast<LoginHelper*>(&*it);
	}
	return nullptr;
}

void ListNormal::EraseLoginHelperBySession( Session* login_session )
{
	auto login_helper = FindLoginHelperBySession(login_session);
	if (login_helper)
		login_helper->session_login_.reset();
}

LoginHelper* ListNormal::FindLoginHelperByPid( DWORD pid )
{
	auto& index_set = login_helper_.get<LoginHelper::tagLoginProcessId>();
	auto login_helper = index_set.find(pid);
	if ( login_helper != index_set.end() )
		return const_cast<LoginHelper*>(&*login_helper);
	else
		return nullptr;
}

void ListNormal::OnMenuStopLoginAll()
{
	for ( const auto& item : login_helper_ )
	{
		if ( !item.session_login_.expired() )
			item.session_login_.lock()->Disconnect();
		SetItemText(item.item_, kSubitemRunState, _T(""));
		auto& login_item = const_cast<LoginHelper&>(item);
		login_item.FreeLoginTimer();
	}
	login_helper_.clear();
}

int ListNormal::GetFreeItem()
{
	const auto kMaxItemCnt = GetItemCount();
	for ( int item = 0;item != kMaxItemCnt;++item )
	{
		CString cstr_ac = GetItemAccount(item);
		//如果未勾选，则不启动进程
		if ( GetCheck(item) && !GetNormalS(cstr_ac) )	
		{
			//寻找一个不被禁止项
			ProhibitT::value_type pro_item;
			pro_item.first = CT2CA(cstr_ac);
			pro_item.second = CT2CA(GetItemText(item, kSubitemService));
			if ( prohibit_login_.find(pro_item) == prohibit_login_.end() )
			{
				auto& index_set = login_helper_.get<LoginHelper::tagItem>();
				auto it = index_set.find(item);
				if ( it == index_set.end() )
					return item;
			}
		}
	}
	return -1;
}

void ListNormal::DeleteAllItems()
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = kMaxItemCnt - 1; item >= 0; --item )
	{
		Delete_Item(item);
	} 
}

int ListNormal::GetNoProhibitItem( const std::string& user_ac, const std::string& service_name )
{
	ProhibitT::value_type pro_item;
	pro_item.first = user_ac;
	pro_item.second = service_name;
	if ( prohibit_login_.find(pro_item) != prohibit_login_.end() )
		return -1;
	CString cstr_ac = CA2CT(user_ac.c_str());
	CString cstr_ser_name = CA2CT(service_name.c_str());
	return FindItemByAccount(cstr_ac, cstr_ser_name);
}

void ListNormal::AddProhibitItem( const std::string& user_ac, const std::string& service_name, stSpecificCfg::enProhibitReason reason )
{
	ProhibitT::value_type pro_item;
	pro_item.first = user_ac;
	pro_item.second = service_name;
	pro_item.prohibit_reason_ = reason;
	prohibit_login_.insert(pro_item);

	auto cfg = GenSpecificCfg(user_ac, service_name);
	if ( !cfg )
	{
		assert(false);
		return;
	}
	cfg->ac_pro_state_.pro_reason_ = reason;
	cfg->ac_pro_state_.pro_at_time_ = DateTimeLocal();
}

void ListNormal::RemoveProhibitItem( const std::string& user_ac, const std::string& game_service )
{
	auto it = FindProhibitIter(user_ac, game_service);
	if ( it != prohibit_login_.end() )
	{
		prohibit_login_.erase(it);

		auto cfg = FindSpecificCfg(user_ac, game_service);
		if (cfg)
		{
			cfg->ac_pro_state_.pro_reason_ = stSpecificCfg::kPR_Valid;
		}
	}
}

DWORD ListNormal::GetLoginingCnt() const
{
	DWORD cnt = 0;
	const auto kItemMax = GetItemCount();
	for ( auto i = 0; i < kItemMax; ++i )
	{
		if (GetItemData(i))
		{
			++cnt;
		}
	}

	decltype(login_helper_.end()) it_login_end = login_helper_.end();
	for ( auto it = login_helper_.begin(); it != it_login_end; ++it )
	{
		if ( it->is_logining_ && !it->session_login_.expired() )
		{
			++cnt;
		}
	}
	return cnt;
}

bool ListNormal::SetItemService( int item, LPCTSTR service_name )
{
	if ( item < 0 )
	{
		assert(false);
		return false;
	}

	CString cstr_ac = GetItemAccount(item);
	//帐号和服务器的键必须唯一
	if ( FindItemByAccount(cstr_ac, service_name) >= 0 )
		return false;

	/*std::string str_account = CT2CA(cstr_ac);
	std::string str_ser_name = CT2CA(service_name);
	auto specific_cfg = GenSpecificCfg(str_account, str_ser_name);
	assert(specific_cfg);
	if ( !specific_cfg )
		return false;

	specific_cfg->game_service_ = CT2CA(service_name);
	specific_cfg->SaveAsAccount(str_account, game_service_mgr_.GetServiceIdx(str_ser_name));*/

	std::string str_account = CT2CA(cstr_ac);
	std::string str_ser_name_old = CT2CA(GetItemServiceName(item));
	auto it = specific_cfgs_.find(std::make_pair(str_account, str_ser_name_old));
	if ( it != specific_cfgs_.end() )
	{
		auto cfg_content = it->second;
		cfg_content.ac_pro_state_.pro_reason_ = stSpecificCfg::kPR_Valid;
		auto ser_idx_old = game_service_mgr_.GetServiceIdx(str_ser_name_old);
		if ( ser_idx_old >= 0 )
		{
			it->second.SaveAsAccount(str_account, ser_idx_old);
		}
		std::string str_ser_name_new = CT2CA(service_name);
		auto service_idx_new = game_service_mgr_.GetServiceIdx(str_ser_name_new);
		if ( service_idx_new >= 0 )
		{
			if (stSpecificCfg::HasSpecificCfg(str_account, service_idx_new))
			{
				cfg_content.RestoreAsAccount(str_account, service_idx_new);
			}
		}
		specific_cfgs_.erase(it);
		specific_cfgs_.insert(SpecificCfgT::value_type(std::make_pair(str_account, str_ser_name_new),
			cfg_content));

		//保存更新后的配置
		SaveCfgByAccount(str_account, str_ser_name_new);

		LPCTSTR kText = _T("");
		const auto& cfg = GetAccountCfg(str_account, str_ser_name_new);
		//帐号状态
		if ( stSpecificCfg::kPR_BanAc == cfg.ac_pro_state_.pro_reason_ )
		{
			kText = _T("帐号被封");
			AddProhibitItem(str_account, str_ser_name_new, stSpecificCfg::kPR_BanAc);
		}
		else if ( stSpecificCfg::kPR_RolesUseOut == cfg.ac_pro_state_.pro_reason_ )
		{
			if ( !const_cast<stSpecificCfg&>(cfg).FindFirstFatigueValidRole() )
			{
				kText = _T("角色刷完");
				AddProhibitItem(str_account, str_ser_name_new, stSpecificCfg::kPR_RolesUseOut);
			}
			else
			{
				auto cfg = FindSpecificCfg(str_account, str_ser_name_new);
				if (cfg)
				{
					cfg->ac_pro_state_.pro_reason_ = stSpecificCfg::kPR_Valid;
				}
			}
		}
		SetItemText(item, kSubitemRunState, kText);
	}
	return !!SetItemText(item, kSubitemService, service_name);
}

bool ListNormal::DoItem( const std::function<bool(int, const ListNormal&)>& func )
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		if ( !func(item, *this) )
			return false;
	}
	return true;
}

CString ListNormal::GetItemServiceName( int item ) const
{
	return GetItemText(item, kSubitemService);
}

int ListNormal::GetItemServiceIdx( int item ) const
{
	if ( item < 0 )
		return -1;
	std::string service_name = CT2CA(GetItemServiceName(item));
	return game_service_mgr_.GetServiceIdx(service_name);
}

int ListNormal::GetItemServiceIdx( const std::string& ac, const std::string& service_name ) const
{
	CString cac = CA2CT(ac.c_str());
	CString str_ser_name = CA2CT(service_name.c_str());
	auto item = FindItemByAccount(cac, str_ser_name);
	if ( item < 0 )
		return -1;
	return GetItemServiceIdx(item);
}

bool ListNormal::CheckSamePwd( LPCTSTR ac, LPCTSTR pwd )
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		if ( GetItemAccount(item) == ac )
			return GetItemText(item, kSubitemPassword) == pwd;
	}
	return true;
}

void ListNormal::ClearProhibit_51()
{
	ProhibitT prohibit_another;
	for ( const auto& pro_item : prohibit_login_ )
	{
		if ( stSpecificCfg::kPR_RolesUseOut != pro_item.prohibit_reason_ )
			prohibit_another.insert(pro_item);
	}
	std::swap(prohibit_login_, prohibit_another);
}

LoginHelper::LoginHelper()
{
	 item_ = -1;
	 login_process_id_ = 0;
	 is_logining_ = true;
}

void LoginHelper::FreeLoginTimer()
{
	if (timer_login_)
	{
		timer_login_->Cancel();
		timer_login_.reset();
	}
}

GameServiceMgr::GameServiceMgr()
{
	std::vector<std::string> service_names;
	auto& lua_vm = GLuaVM_S::GetMe();
	VERIFY(lua_vm.VCall("Login_GetServiceNames", &service_names));
	
	for ( auto it = service_names.begin(); it != service_names.end(); ++it )
	{
		const auto kServiceId = GLuaVM_S::GetMe().Call<DWORD>("Login_GetServiceId", 0, it->c_str());
		assert(kServiceId);
		services_info_.push_back(ContT::value_type(*it, kServiceId));
	}
}

DWORD GameServiceMgr::GetServiceId( const std::string& service_name ) const
{
	auto it = std::find_if(services_info_.begin(), services_info_.end(), [&service_name]( const ServiceInfoT& service_info ){
		return service_info.first == service_name;
	});
	if ( it != services_info_.end() )
	{
		return it->second;
	}
	assert(false);
	return 0;
}

const GameServiceMgr::ContT& GameServiceMgr::GetServicesInfo() const
{
	return services_info_;
}

int GameServiceMgr::GetServiceIdx( const std::string& service_name ) const
{
	int idx = 0;
	for ( const auto& service_info : services_info_ )
	{
		if ( service_info.first == service_name )
			return idx;
		++idx;
	}
	assert(false);
	return -1;
}


void ListNormal::OnMenuSelectGameAcType()
{
	DlgSelectGameAcType dlg(this);
	if ( IDOK != dlg.DoModal() )
		return;
	const auto kGameAcType = dlg.GetGameAcType();
	LPCTSTR kText = _T("");
	if ( kGAT_Normal == kGameAcType )
		kText = _T("普通号");
	else if ( kGAT_Stored == kGameAcType )
		kText = _T("仓库号");
	else if ( kGAT_TransStation == kGameAcType )
		kText = _T("中转站");

	auto pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		auto item = GetNextSelectedItem(pos);
		if ( item < 0 )
			continue;
		if (GetNormalS(item))
			continue;
		std::string str_account = CT2CA(GetItemAccount(item));
		std::string str_ser_name = CT2CA(GetItemServiceName(item));
		auto specific_cfg = GenSpecificCfg(str_account, str_ser_name);
		assert(specific_cfg);
		if ( !specific_cfg )
			continue;
		if ( kGameAcType == specific_cfg->game_ac_type_ )
			continue;

		specific_cfg->game_ac_type_ = kGameAcType;
		specific_cfg->SaveAsAccount(str_account, game_service_mgr_.GetServiceIdx(str_ser_name));

		SetItemText(item, kSubitemGameAcType, kText);

		/*auto func_obj = GetNormalS(item);
		if (func_obj)
		{
		func_obj->SetGameAcType(kGameAcType);
		}*/
	}
}

enGameAcType ListNormal::GetItemGameAcType( int item )
{
	if ( item < 0 )
		return kGAT_Normal;
	const auto kMaxCnt = GetItemCount();
	if ( item >= kMaxCnt )
		return kGAT_Normal;

	std::string str_ac = CT2CA(GetItemAccount(item));
	std::string str_ser_name = CT2CA(GetItemServiceName(item));
	const auto& cfg = GetAccountCfg(str_ac, str_ser_name);
	return cfg.game_ac_type_;
}

void ListNormal::DoItemFuncObj( const std::function<void(FuncObj_NormalS&)>& func )
{
	const auto kMaxItemCnt = GetItemCount();
	for ( auto item = 0; item < kMaxItemCnt; ++item )
	{
		auto func_obj_normal = GetNormalS(item);
		if (func_obj_normal)
			func(*func_obj_normal);
	}
}

void ListNormal::OnMenuAsNotifyObj()
{
	auto pos = GetFirstSelectedItemPosition();
	if ( !pos )
		return;
	auto item = GetNextSelectedItem(pos);
	if ( item < 0 )
		return;

	auto page_trade = PageTrade::GetInstance();
	if ( !page_trade )
		return;
	auto func_obj = GetNormalS(item);
	if ( !func_obj )
		return;
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return;
	page_trade->SetCurNotifyItem(item);
	tab_option->SetCurFocus(1);
	tab_option->SetCurSel(1);
}

int ListNormal::FindItemByAccount( const std::string& game_ac, const std::string& game_ser_name ) const
{
	if ( game_ac.empty() || game_ser_name.empty() )
		return -1;
	CString ac = CA2CT(game_ac.c_str());
	CString pwd = CA2CT(game_ser_name.c_str());
	return FindItemByAccount(ac, pwd);
}

void ListNormal::OnMenuCancelprohibit()
{
	auto pos = GetFirstSelectedItemPosition();
	while (pos)
	{
		RemoveProhibitItem(GetNextSelectedItem(pos));
	}
}

void ListNormal::RemoveProhibitItem( int item )
{
	if ( item < 0  || item >= GetItemCount() )
	{
		assert(false);
		return;
	}
	std::string str_ac = CT2CA(GetItemAccount(item));
	std::string str_ser = CT2CA(GetItemServiceName(item));
	auto it = FindProhibitIter(str_ac, str_ser);
	if ( it == prohibit_login_.end() )
		return;
	prohibit_login_.erase(it);
	SetItemText(item, kSubitemRunState, _T(""));

	auto cfg = FindSpecificCfg(str_ac, str_ser);
	if (cfg)
	{
		cfg->ac_pro_state_.pro_reason_ = stSpecificCfg::kPR_Valid;
	}
}

ListNormal::ProhibitT::iterator 
ListNormal::FindProhibitIter( const std::string& user_ac, const std::string& game_service )
{
	for ( auto it = prohibit_login_.begin(); it != prohibit_login_.end(); ++it )
	{
		if ( it->first == user_ac && it->second == game_service )
		{
			return it;
		}
	}
	return prohibit_login_.end();
}


void ListNormal::OnMenuNotifyAsThis()
{
	
}

void ListNormal::UpdateRoleInfo( FuncObj_NormalS& func_obj_normal )
{
	auto item = FindItemByFuncObj(func_obj_normal);
	if ( item < 0 )
	{
		assert(false);
		return;
	}

	const auto& game_info = func_obj_normal.GetGameInfo();
	CString str_tmp;
	str_tmp.Format(_T("%d"), game_info.cur_hp_);
	SetItemText(item, kSubitemCurHp, str_tmp);
	SetItemText(item, kSubitemRoleName, game_info.role_name_);
	str_tmp.Format(_T("%d"), game_info.role_lvl_);
	SetItemText(item, kSubitemRoleLevel, str_tmp);
	str_tmp.Format(_T("%d"), game_info.fatigue_data_);
	SetItemText(item, kSubitemFatigueData, str_tmp);
	SetItemText(item, kSubitemCurMapRoomName, game_info.map_room_name_);
	str_tmp.Format(_T("%d"), game_info.cur_gold_);
	SetItemText(item, kSubitemCurGold, str_tmp);
	str_tmp.Format(_T("%d"), game_info.gained_gold_);
	SetItemText(item, kSubitemGainedGold, str_tmp);
	SetItemText(item, kSubitemJobName, game_info.job_name_);

	UpdateLastNotifyTime(item);
}

stSpecificCfg::enProhibitReason ListNormal::GetProhibitReason( const std::string& user_ac, const std::string& game_service )
{
	ProhibitT::value_type pro_item;
	pro_item.first = user_ac;
	pro_item.second = game_service;
	auto it = prohibit_login_.find(pro_item);
	if ( it == prohibit_login_.end() )
		return stSpecificCfg::kPR_Valid;
	return it->prohibit_reason_;
}

bool ListNormal::AddLoginItem( int item )
{
	//如果未勾选，则不启动进程
	if ( !GetCheck(item) )
		return false;
	if (GetNormalS(item))
		return false;
	std::string user_ac = CT2CA(GetItemAccount(item));
	std::string service_name = CT2CA(GetItemText(item, kSubitemService));
	ProhibitT::value_type pro_item;
	pro_item.first = user_ac;
	pro_item.second = service_name;
	if ( prohibit_login_.find(pro_item) != prohibit_login_.end() )
		//该项被禁止启动了
			return false;

	//是否已经在登陆了
	auto& index_set = login_helper_.get<LoginHelper::tagItem>();
	auto it = index_set.find(item);
	if ( it != index_set.end() )
		return false;

	LoginHelper login_helper;
	login_helper.item_ = item;
	login_helper.login_process_id_ = 0;
	login_helper.is_logining_ = false;
	index_set.insert(login_helper);

	SetItemText(item, kSubitemRunState, _T("待登陆"));
	return true;
}

void ListNormal::EraseLoginHelper( LoginHelper* login_hlp )
{
	if ( !login_hlp )
	{
		assert(false);
		return;
	}
	auto& index_set = login_helper_.get<LoginHelper::tagLoginProcessId>();
	auto login_helper = index_set.find(login_hlp->login_process_id_);
	if ( login_helper != index_set.end() )
	{
		index_set.erase(login_helper);
	}
}

void ListNormal::UpdateCurRoleIndex( FuncObj_NormalS& nor, int idx )
{
	auto item = FindItemByFuncObj(nor);
	if ( item < 0 )
	{
		assert(false);
		return;
	}
	SetItemText(item, kSubitemCurRoleIdx, CA2CT(std::to_string(idx).c_str()));
}

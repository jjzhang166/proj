#include "stdafx.h"
#include "FuncObjS.h"
#include "Common/XmlCfg.h"
#include "TabOption.h"
#include "ListCtrl_Players.h"
#include <protect/Protect.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Communicate/CG_Defines.h>
#include "CoordinateC.h"
#include "CfgStruct.h"
#include <Common/Toolhelp.h>
#include <CommonX/CmnX_DllApi.h>

FuncObj_NormalS::FuncObj_NormalS( Server& server )
	: Session(server), func_obj_inter_(this), glua_(this), json_echo_(GLuaVM_S::GetMe(), *this), session_oper_(*this), lua_script_(GLuaVM_S::GetMe(), *this),
	coor_trans_(*this, CoorCMgr::GetMe().MakeTransTrackIterPtr())
{
	game_ac_type_ = kGAT_Normal;
	service_idx_ = 0;
	game_process_id_ = 0;
	no_relogin_at_disconnect_ = false;
	is_you_hua_ = true;
}

FuncObj_IInter& FuncObj_NormalS::GetFuncObjInter()
{
	return func_obj_inter_;
}

GLuaS& FuncObj_NormalS::GetGLua()
{
	return glua_;
}

bool FuncObj_NormalS::OnUserMsg( P_MsgNT msg_num, void* param, size_t param_size )
{
	switch (msg_num)
	{
	case P_Exec_CallLuaFuncResult::value:
		{
			if ( sizeof(P_Exec_CallLuaFuncResult) != param_size )
				break;
			return OnCallLuaFuncResult(*(P_Exec_CallLuaFuncResult*)param);
		}
	case P_Exec_GameProccessInfo::value:
		{
			if ( sizeof(P_Exec_GameProccessInfo) != param_size )
				break;
			return OnGameProccessInfo(*(P_Exec_GameProccessInfo*)param);
		}
	default:
		break;
	}
	return __super::OnUserMsg(msg_num, param, param_size);
}

bool FuncObj_NormalS::OnCallLuaFuncResult( P_Exec_CallLuaFuncResult& func_res )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "LUA函数名：" << func_res.call_func_name << "\t参数：" << func_res.func_param << "\t结果：" << func_res.func_result;
	return true;
}

JsonEcho& FuncObj_NormalS::GetJsonEcho()
{
	return json_echo_;
}

bool FuncObj_NormalS::SetAccountInfo( const P_Modify_Account& account_info )
{
	account_ = account_info.account_;
	password_ = account_info.password_;
	service_name_ = account_info.service_name_;
#ifdef _MULTI_LOGIN_
	VMP_BEGIN_EX;
	auto& cfg = stSingletonCfg::GetMe();
	auto ac_pwd = cfg.FindFreeOne();
	if ( !ac_pwd )
	{
		assert(false);
		Disconnect();
		return false;
	}
	if ( !ac_pwd->AddBindGameAcInfo(account_, service_name_) )
	{
		assert(false);
		return false;
	}
	VMP_END;
#endif
	return true;
}

void FuncObj_NormalS::CallRemoteLua( const char* lua_func_name, const char* lua_func_param )
{
	//assert(!"用JsonCall吧，不要用这个了");
	P_Exec_CallLuaFunction msg;
	strcpy_s(msg.call_func_name, lua_func_name);
	if (lua_func_param)
		strcpy_s(msg.func_param, lua_func_param);
	UserMsg(P_Exec_CallLuaFunction::value, &msg, sizeof(msg));
}

bool FuncObj_NormalS::OnGameProccessInfo( const P_Exec_GameProccessInfo& game_proccess_info )
{
	auto tbl_option = CTabOption::GetInstance();
	if ( !tbl_option )
		return false;
	auto& ctrl_list = tbl_option->GetPageAccount().GetAccountList();
	auto the_login_process_id = CToolhelp::GetParentProcessID(game_proccess_info.self_process_id_);
	//auto the_login_process_id = game_proccess_info.self_process_id_;
	if ( -1 == (int)the_login_process_id )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "game process info invalid";
	}
	auto item = ctrl_list.AcceptSession(the_login_process_id);
	if ( item < 0 )
	{
		item = ctrl_list.GetFreeItem();
	}

	if ( item >= 0 )
	{
		P_Modify_Account msg;
		
		strcpy_s(msg.account_, CT2CA(ctrl_list.GetItemAccount(item)));
		strcpy_s(msg.password_, CT2CA(ctrl_list.GetItemText(item, kSubitemPassword)));
		std::string ser_name = CT2CA(ctrl_list.GetItemServiceName(item));
		strcpy_s(msg.service_name_, ser_name.c_str());
		msg.auto_run_script_ = ctrl_list.GetAccountCfg(msg.account_, ser_name).auto_run_script_;

		if ( !SetAccountInfo(msg) )
			return false;
		if ( !UserMsg(P_Modify_Account::value, &msg, sizeof(msg)) )
			return false;
		DoSomeConnected();

		last_active_time_ = DateTimeLocal();
		ctrl_list.SetItemFuncObj(item, this);
		game_process_id_ = game_proccess_info.self_process_id_;
		assert(game_process_id_);
		return true;
	}
	return false;
}

SessionOperS& FuncObj_NormalS::GetSessionOper()
{
	return session_oper_;
}

FuncObj_NormalS::stGameInfo& FuncObj_NormalS::GetGameInfo()
{
	return game_info_;
}

void FuncObj_NormalS::AddProhibitItem( stSpecificCfg::enProhibitReason reason, LPCTSTR str_run_state )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if (ctrl_list)
	{
		//添加禁止启动项
		ctrl_list->AddProhibitItem(account_, service_name_, reason);

		CString cstr_ac = CA2CT(account_.c_str());
		CString cstr_ser_name = CA2CT(service_name_.c_str());
		//关闭游戏进程
		auto item = ctrl_list->FindItemByAccount(cstr_ac, cstr_ser_name);

		str_final_run_state_ = str_run_state;

		ctrl_list->CloseGameProcess(item);
	}
}

void FuncObj_NormalS::DoSomeConnected()
{
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return;
	VMP_BEGIN_EX;
	//////////////////////////////////////////////////////////////////////////
#ifdef _MULTI_LOGIN_
	auto& cfg = stSingletonCfg::GetMe();
	auto ac_pwd = cfg.FindByAc_Ser(account_, service_name_);
	if ( !ac_pwd )
	{
		assert(false);
		return;
	}
	const DateTimeLocal& time_now = G_GetSerCurTime();
	if (ac_pwd->TestTimeEnd(time_now))
	{
		assert(false);
		return;
	}
	bool login_res = ac_pwd->IsTheLoginSucceed();
	if ( !login_res )
	{
		assert(false);
		return;
	}
	const auto& end_date_time_ptr = ac_pwd->GetUseEndDayTime();
	if ( !end_date_time_ptr )
	{
		assert(false);
		return;
	}
	const auto& end_date_time = *end_date_time_ptr;
	const auto& addr_datas = ac_pwd->GetAddrDatas();
	const auto& str_datas = ac_pwd->GetStrDatas();
#else
	bool login_res = tab_option->IsLoginSucceed();
	const auto& end_date_time = tab_option->GetEndDateTime();
	const auto& addr_datas = tab_option->GetAddrDatas();
#endif
	//设置验证信息
	session_oper_.ToCall(BC_MakeP("SetLoginSucceed")<<login_res);

	
#ifdef _DEBUG
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << Poco::DateTimeFormatter::format(end_date_time, Poco::DateTimeFormat::SORTABLE_FORMAT);
#endif
	session_oper_.ToCall(BC_MakeP("4AA7E8C2E7CFE703")<<end_date_time.timestamp().epochTime());

	//设置关键数据
	if (addr_datas.size())
	{
		Json::Value json_addr_datas;
		for ( const auto& addr_value : addr_datas )
		{
			json_addr_datas[json_addr_datas.size()] = (Json::UInt)addr_value;
		}
		session_oper_.ToCall(BC_MakeP("sadsex")<<json_addr_datas);
	}
	if (str_datas.size())
	{
		Json::Value json_str_datas;
		for ( const auto& str_value : str_datas )
		{
			json_str_datas[json_str_datas.size()] = str_value;
		}
		session_oper_.ToCall(BC_MakeP("ssdtaseqdc")<<json_str_datas);
	}
	//////////////////////////////////////////////////////////////////////////
	//设置固定的数据
	session_oper_.ToCall(BC_MakeP("SetMatchTime")<<GenUuidStr(VMP_PROTECT_STR(kStableStr_ConsoleToGame)));
	session_oper_.ToCall(BC_MakeP("SetHitM_Info")<<tab_option->GetConsoleUuid()<<GetCurrentProcessId());
	SetRightBtHit(cfg.right_bt_hit_);
	SetOpenVillagePath(cfg.open_village_path_);
	session_oper_.ToCall(BC_MakeP("SetYouHua") << is_you_hua_);
	VMP_END;
}

CoorTrans_CS& FuncObj_NormalS::GetCoorTrans()
{
	return coor_trans_;
}

enGameAcType FuncObj_NormalS::GetGameAcType() const
{
	return game_ac_type_;
}

int FuncObj_NormalS::GetServiceIdx() const
{
	return service_idx_;
}

void FuncObj_NormalS::SetGameAcType( enGameAcType game_ac_type )
{
	game_ac_type_ = game_ac_type;
	session_oper_.ToCall(BC_MakeP("SetGameAcType")<<pt_int(game_ac_type));
}

void FuncObj_NormalS::SetServiceIdx( int service_idx )
{
	service_idx_ = service_idx;
	session_oper_.ToCall(BC_MakeP("SetGameSerIdx")<<service_idx);
}

const std::string& FuncObj_NormalS::GetGameAc() const
{
	return account_;
}

LuaScriptS& FuncObj_NormalS::GetLuaScript()
{
	return lua_script_;
}

void FuncObj_NormalS::OnDisconnect( IoConnectBase* connect )
{
	if (timer_active_handle_)
	{
		if (timer_active_handle_->IsClocking())
			timer_active_handle_->Cancel();
		timer_active_handle_.reset();
	}

	__super::OnDisconnect(connect);
	auto the_process_id = game_process_id_;
	if ( the_process_id != 0 )
	{
		//强制关闭进程
		auto the_handle = ::OpenProcess(PROCESS_TERMINATE, FALSE, the_process_id);
		if (the_handle)
		{
			//SetIoTimer(1000 * 6, [the_process_id, the_handle](IoTimer*){
				LOG_O(Log_debug) << "控制台强制关闭游戏进程，进程ID：" << the_process_id;
				::TerminateProcess(the_handle, 0);
				::CloseHandle(the_handle);
			//});
		}
	}
	if (timer_login_succeed_)
		timer_login_succeed_->Cancel();

	VMP_BEGIN_EX;
#ifdef _MULTI_LOGIN_

	auto& cfg = stSingletonCfg::GetMe();
	auto ac_pwd = cfg.FindByAc_Ser(account_, service_name_);
	//assert(ac_pwd);
	if (ac_pwd)
		VERIFY(ac_pwd->DeleteTheBindGameAcInfo(account_, service_name_));
#endif
	//return;
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		return;
	}
	//设置运行状态信息
	list_ctrl->FreeItemByFuncObj(*this, str_final_run_state_);

	//掉线重登陆
	if ( list_ctrl->ban_ac_cnt_ <= 0 )
	{
		//assert(false);
		auto item = list_ctrl->GetNoProhibitItem(account_, service_name_);
		if ( item >= 0 )
		{
			if ( stSingletonCfg::GetMe().is_login_at_disc_ && !no_relogin_at_disconnect_ )
				list_ctrl->AddFirstLoginItem(item);
			else
				list_ctrl->AddLoginItem(item, 1);
		}

		if ( stSpecificCfg::kPR_Restricted == list_ctrl->GetProhibitReason(account_, service_name_) )
		{
			item = list_ctrl->FindItemByAccount(account_, service_name_);
			if ( item < 0 )
			{
				assert(false);
				return;
			}
			list_ctrl->RemoveProhibitItem(item);
			if ( stSingletonCfg::GetMe().is_login_at_disc_ && !no_relogin_at_disconnect_ )
				list_ctrl->AddFirstLoginItem(item);
			else
				list_ctrl->AddLoginItem(item, 0);
		}

		list_ctrl->DoLoginOne();
	}
	VMP_END;
}

bool FuncObj_NormalS::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	VMP_BEGIN_EX;
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return false;
	if ( !tab_option->IsLoginSucceed() )
		return false;
#ifdef _MULTI_LOGIN_
	auto& cfg = stSingletonCfg::GetMe();
	if ( !cfg.FindFreeOne() )
	{
		//assert(false);
		return false;
	}
#else
	const DateTimeLocal& time_now = G_GetSerCurTime();
	if ( time_now >= tab_option->GetEndDateTime() )
		return false;
#endif
	VMP_END;
	is_you_hua_ = cfg.is_use_you_hua_;
	auto shared_this = shared_from_this();
	assert(shared_this);
	timer_login_succeed_ = CreateIoTimer(1000 * 60 * 10, [shared_this]( const IoTimerPtr& ){
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "登陆未收到成功，判定登陆超时";
		CMN_ASSERT(false);
		if (shared_this)
			shared_this->DoDisconnect();
	}, true);
	return true;
}

void FuncObj_NormalS::SetRightBtHit( bool is_right_hit )
{
	session_oper_.ToCall(BC_MakeP("SetRightBtHit")<<is_right_hit);
}

const std::string& FuncObj_NormalS::GetGameService() const
{
	return service_name_;
}

FuncObj_NormalS::~FuncObj_NormalS()
{
	if (timer_login_succeed_)
		timer_login_succeed_->Cancel();
}

void FuncObj_NormalS::SetOpenVillagePath( bool is_open_village_path )
{
	//session_oper_.ToCall(BC_MakeP("SetOpenVillagePath")<<is_open_village_path);
}

void FuncObj_NormalS::SetYouHuan(bool you_hua)
{
	if (is_you_hua_ == you_hua)
		return;
	session_oper_.ToCall(BC_MakeP("SetYouHua") << you_hua);
	is_you_hua_ = you_hua;
}

bool FuncObj_NormalS::IsActiveTimeout(DateTimeLocal& time_now) const
{
	auto time_span = time_now - last_active_time_;
	if (abs(time_span.totalSeconds()) >= 60 * 6)
		return true;
	return false;
}

void FuncObj_NormalS::SetActiveTimeoutHandle()
{
	if (timer_active_handle_)
	{
		if (timer_active_handle_->IsClocking())
			return;
		timer_active_handle_.reset();
	}

	auto ac = account_;
	auto ser = service_name_;
	timer_active_handle_ = CreateIoTimer(1000 * 60, [ac, ser](IoTimerPtr&){
		auto ctrl_list = PageAccount_List::GetInstance();
		if (ctrl_list)
		{
			auto item = ctrl_list->FindItemByAccount(ac, ser);
			if (item >= 0)
			{
				auto nor = ctrl_list->GetNormalS(item);
				if (nor)
				{
					LOG_O(Log_debug) << "长时间未收到SetRunInfo，关闭连接";
#ifdef _DEBUG
					if (IDCANCEL == MessageBoxTimeout(nullptr, "长时间未收到SetRunInfo，是否关闭连接", nullptr, MB_OKCANCEL, 0, 1000 * 10))
						return;
					CMN_ASSERT(false);
#endif
					nor->Disconnect();
				}
			}
		}
	}, true);

	LOG_O(Log_debug) << "活跃时间超时，帐号：" << account_;
	session_oper_.ToCall(BC_MakeP("GetRunInfo") << 1);
}

SessionOperS::SessionOperS( FuncObj_NormalS& func_obj_normal ) : BufCall_SocketExcept(&func_obj_normal, kObjIdNormal), func_obj_normal_(func_obj_normal)
{

}

void SessionOperS::SetRunState( const char* str_run_state )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}

	func_obj_normal_.game_info_.running_state_ = str_run_state;

	list_ctrl->SetRunState(func_obj_normal_);
}

void SessionOperS::SetScriptOutputInfo( const std::string& output_info )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}
	list_ctrl->SetScriptOutputInfo(func_obj_normal_, output_info.c_str());

	func_obj_normal_.last_active_time_ = DateTimeLocal();
	if (func_obj_normal_.timer_active_handle_)
	{
		if (func_obj_normal_.timer_active_handle_->IsClocking())
			func_obj_normal_.timer_active_handle_->Cancel();
		func_obj_normal_.timer_active_handle_.reset();
	}
}

void SessionOperS::StartRunScript()
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}
	list_ctrl->StartRunScript(func_obj_normal_);
}

void SessionOperS::EchoMsg( const char* msg )
{
	func_obj_normal_.GetJsonEcho().GlobalFuncCall(
		"LuaLogT", &JsonEcho::MakeParams(msg));
}

void SessionOperS::SetItemsInfo( const Json::Value& items_array )
{

}

void SessionOperS::BanAccount()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "帐号->" << func_obj_normal_.account_ << "\t被封停";
	if (stSingletonCfg::GetMe().stop_login_at_ban_ac_)
	{
		auto list_ctrl = PageAccount_List::GetInstance();
		if (list_ctrl)
		{
			++list_ctrl->ban_ac_cnt_;
			list_ctrl->OnMenuStopLoginAll();
		}
	}	
	func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_BanAc, _T("帐号被封"));
}

void SessionOperS::SetMyOfferList( const Json::Value& sold_info )
{
	GetIoService().post([sold_info](){
		auto list_my_offer_list = CList_MyOfferList::GetInstance();
		if ( !list_my_offer_list )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "我的出售列表的控件不存在";
			return;
		}
		stAuctionItemInfoEx offer_item;

		std::vector<stAuctionItemInfoEx> offer_items;
		for ( const auto& json_item : sold_info )
		{
			if ( !offer_item.ParseFromJsonObj(json_item) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "解析出错";
				return;
			}
			if ( !offer_item.ParseFromJsonObjEx(json_item) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "解析出错1";
				return;
			}

			offer_items.push_back(offer_item);
		}
		for ( const auto& item : offer_items )
		{
			list_my_offer_list->AddOfferItem(item);
		}

		auto page_trade = PageTrade::GetInstance();
		if (page_trade)
			page_trade->SetMyOfferListCnt(offer_items.size());
	});
}

void SessionOperS::SetOtherOfferItems( const Json::Value& sold_players )
{
	GetIoService().post([sold_players](){
		auto list_other_offers = CList_OtherOffers::GetInstance();
		if ( !list_other_offers )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "我的出售列表的控件不存在";
			return;
		}
		stAuctionItemInfoEx offer_item;
		std::vector<stAuctionItemInfoEx> offer_items;
		for ( const auto& json_item : sold_players )
		{
			if ( !offer_item.ParseFromJsonObj(json_item) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "1解析出错";
				return;
			}
			if ( !offer_item.ParseFromJsonObjEx(json_item) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "1解析出错1";
				return;
			}
			offer_items.push_back(offer_item);
		}
		for ( const auto& item : offer_items )
		{
			list_other_offers->AddOfferItem(item);
		}
		auto page_trade = PageTrade::GetInstance();
		if (page_trade)
			page_trade->SetOtherOfferItemsCnt(offer_items.size());
	});
}

void SessionOperS::ExchangeGameAc()
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	auto item = ctrl_list->FindItemByAccount(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( item < 0 )
		return;
	ctrl_list->SetCheck(item, FALSE);
	ctrl_list->CloseGameProcess(item);
	
	//启动一个新的帐号
	//ctrl_list->DoLoginOne();
}

void SessionOperS::SetRoleName( const std::string& role_name )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.role_name_ = CA2CT(role_name.c_str());
	if ( func_obj_normal_.game_info_.role_name_a_ != role_name )
	{
		func_obj_normal_.game_info_.begin_gold_.reset();
		func_obj_normal_.game_info_.cur_gold_ = 0;
	}
	func_obj_normal_.game_info_.role_name_a_ = role_name;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);

	const auto specifg_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specifg_cfg )
	{
		assert(false);
		return;
	}
	auto role_info = specifg_cfg->FindRoleInfoByName(role_name);
	if (role_info)
	{
		auto role_idx = specifg_cfg->RoleInfo2Index(role_info);
		if ( role_idx < 0 )
		{
			assert(false);
			return;
		}
		auto the_role_idx = role_idx + 1;
		ctrl_list->UpdateCurRoleIndex(func_obj_normal_, the_role_idx);
		specifg_cfg->role_idx_.Format(_T("%d"), the_role_idx);
	}
	specifg_cfg->role_name_ = func_obj_normal_.game_info_.role_name_;
}

void SessionOperS::SetRoleLvl( int role_lvl )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.role_lvl_ = role_lvl;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);

	const auto specifg_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specifg_cfg )
	{
		assert(false);
		return;
	}
	specifg_cfg->role_lvl_.Format(_T("%d"), role_lvl);
}

void SessionOperS::SetCurMapRoomName( const std::string& map_room_name )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.map_room_name_ = CA2CT(map_room_name.c_str());
	ctrl_list->UpdateRoleInfo(func_obj_normal_);
}

void SessionOperS::SetCurGold( int cur_gold )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.cur_gold_ = cur_gold;
	if ( !func_obj_normal_.game_info_.begin_gold_ )
		func_obj_normal_.game_info_.begin_gold_ = cur_gold;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);

	const auto specifg_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specifg_cfg )
	{
		assert(false);
		return;
	}
	specifg_cfg->cur_gold_.Format(_T("%d"), cur_gold);
}

void SessionOperS::SetFreeLieShouCnt( int lie_shou_cnt )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	assert(lie_shou_cnt >= 0);
	func_obj_normal_.game_info_.free_lie_shou_cnt_ = lie_shou_cnt;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);

	const auto specifg_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specifg_cfg )
	{
		assert(false);
		return;
	}
	specifg_cfg->shou_lie_quan_.Format(_T("%d"), lie_shou_cnt);
}

void SessionOperS::SetRoleListInfo( const Json::Value& json_array )
{
	if ( !json_array.isArray() )
	{
		assert(false);
		return;
	}
	auto list_nor = PageAccount_List::GetInstance();
	if ( !list_nor )
	{
		assert(false);
		return;
	}
	auto specific_cfg = list_nor->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specific_cfg )
	{
		assert(false);
		return;
	}
	CG_RoleInfo::RoleInfoContT roles_info;
	if ( !json_array.empty() )
	{
		if ( !json_array.isEmptyArray() )
		{
			CG_RoleInfo role_tmp;
			for ( const auto& json_v : json_array )
			{
				if ( !role_tmp.ParseFromJosn(json_v) )
				{
					assert(false);
					return;
				}
				roles_info.push_back(role_tmp);
			}
		}	
	}
	specific_cfg->SetRolesInfo(roles_info);
	list_nor->SaveCfgByAccount(func_obj_normal_.account_, func_obj_normal_.service_name_);
}

void SessionOperS::AutoCreateOrSelectRole()
{
	/*struct Helper
	{
		Helper( SessionOperS* session_oper ){
			session_oper_ = session_oper;
		}
		~Helper(){
			session_oper_->ThisMemCall(nullptr, "SetAutoCreateOrSelectRoleDone", nullptr);
		}

		SessionOperS*			session_oper_;
	};
	Helper hlp(this);*/
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	
	const auto specifg_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specifg_cfg )
	{
		assert(false);
		return;
	}
	auto role_info = specifg_cfg->FindFirstFatigueValidRole();
	if ( !role_info )
	{
		if ( !specifg_cfg->CanCreateNewRole() )
		{
			SetRolesUseOut();
			return;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "控制台开始远程创建角色";
		if ( !ToCall(BC_MakeP("CreateRandNameRole")) )
		{
			//assert(false);
			LOG_O(Log_debug) << "控制台远程调用创建角色失败";
			return;
		}
		//LOG_O(Log_debug) << "控制台远程创建角色失败次数过多";
		return;

		role_info = specifg_cfg->FindFirstFatigueValidRole();
		if ( !role_info )
		{
			//assert(false);
			SetRolesUseOut();
			return;
		}
	}
	assert(role_info);
	assert(role_info->fatigue_data_ > 0);
	if ( !ToCall(BC_MakeP("SelectRole")<<role_info->role_info_.role_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "选择角色信息失败:" << role_info->role_info_.role_name_;
		//assert(false);
		return;
	}
	auto role_idx = specifg_cfg->RoleInfo2Index(role_info);
	if ( role_idx < 0 )
	{
		assert(false);
		return;
	}
	ctrl_list->UpdateCurRoleIndex(func_obj_normal_, role_idx + 1);
}

void SessionOperS::GameLoginSucceed()
{
	CMN_ASSERT(func_obj_normal_.timer_login_succeed_ && func_obj_normal_.timer_login_succeed_->IsClocking());
	if (func_obj_normal_.timer_login_succeed_)
		func_obj_normal_.timer_login_succeed_->Cancel();
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		return;
	}
	auto item = list_ctrl->FindItemByFuncObj(func_obj_normal_);
	if ( item < 0 )
	{
		assert(false);
		return;
	}
	//list_ctrl->SetItemText(item, kSubitemScriptOutputInfo, _T("等待运行"));
	list_ctrl->DoLoginOne();
}

void SessionOperS::RoleRestricted()
{
	func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_Restricted, _T("被限制"));
}

void SessionOperS::OnCreateRoleFailed()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "之前创建角色失败了，再次创建角色";
	if ( !ToCall(BC_MakeP("CreateRandNameRole")) )
	{
		//assert(false);
		LOG_O(Log_debug) << "控制台远程调用创建角色失败";
		return;
	}
}

void SessionOperS::SetSingleExp( int single_exp )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.single_exp_ = single_exp;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);

	const auto specifg_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specifg_cfg )
	{
		assert(false);
		return;
	}
	specifg_cfg->single_exp_.Format(_T("%d"), single_exp);
}

void SessionOperS::SetDoubleExp( int double_exp )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.double_exp_ = double_exp;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);

	const auto specifg_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specifg_cfg )
	{
		assert(false);
		return;
	}
	specifg_cfg->double_exp_.Format(_T("%d"), double_exp);
}

void SessionOperS::SetFatigueData( pt_qword fatigue_data )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	assert(fatigue_data >= 0);
	func_obj_normal_.game_info_.fatigue_data_ = fatigue_data;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);
	if (func_obj_normal_.game_info_.role_name_a_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "控制台设置疲劳前未设置名称";
		return;
	}
	auto specific_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specific_cfg )
	{
		assert(false);
		return;
	}
	specific_cfg->SetRoleFatigueData(func_obj_normal_.game_info_.role_name_a_, fatigue_data);
}

void SessionOperS::SetRolesUseOut()
{
	auto auto_dial = DlgAutoDial::GetInstance();
	if (auto_dial)
		++auto_dial->cur_role_use_out_cnt_;

	func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_RolesUseOut, _T("角色刷完"));
}

void SessionOperS::SetBanAcTime( const std::string& time )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
		return;
	list_ctrl->SetProhibitTime(func_obj_normal_, time);
}

void SessionOperS::NoReLoginAtDisconnect()
{
	func_obj_normal_.no_relogin_at_disconnect_ = true;
}

BC_REG_CLS_DEFINE(SessionOperS){
	bufcall::class_<SessionOperS>("SessionOperS")
	.def("SetRunState", &SessionOperS::SetRunState)
	.def("SetScriptOutputInfo", &SessionOperS::SetScriptOutputInfo)
	.def("StartRunScript", &SessionOperS::StartRunScript)
	.def("EchoMsg", &SessionOperS::EchoMsg)
	.def("BanAccount", &SessionOperS::BanAccount)
	.def("SetItemsInfo", &SessionOperS::SetItemsInfo)
	.def("SetMyOfferList", &SessionOperS::SetMyOfferList)
	.def("SetOtherOfferItems", &SessionOperS::SetOtherOfferItems)
	.def("ExchangeGameAc", &SessionOperS::ExchangeGameAc)
	.def("SetRoleName", &SessionOperS::SetRoleName)
	.def("SetRoleLvl", &SessionOperS::SetRoleLvl)
	.def("SetFreeLieShouCnt", &SessionOperS::SetFreeLieShouCnt)
	.def("SetSingleExp", &SessionOperS::SetSingleExp)
	.def("SetDoubleExp", &SessionOperS::SetDoubleExp)
	.def("SetCurMapRoomName", &SessionOperS::SetCurMapRoomName)
	.def("SetCurGold", &SessionOperS::SetCurGold)
	.def("SetRoleListInfo", &SessionOperS::SetRoleListInfo)
	.def("AutoCreateOrSelectRole", &SessionOperS::AutoCreateOrSelectRole)
	.def("GameLoginSucceed", &SessionOperS::GameLoginSucceed)
	.def("RoleRestricted", &SessionOperS::RoleRestricted)
	.def("OnCreateRoleFailed", &SessionOperS::OnCreateRoleFailed)
	.def("SetFatigueData", &SessionOperS::SetFatigueData)
	.def("SetRolesUseOut", &SessionOperS::SetRolesUseOut)
	.def("SetBanAcTime", &SessionOperS::SetBanAcTime)
	.def("NoReLoginAtDisconnect", &SessionOperS::NoReLoginAtDisconnect)
	.def("SetRunInfo", &SessionOperS::SetRunInfo);
}

void SessionOperS::SetRunInfo(int cnt, bool script_running, const DateTimeLocal& last_recv_msg_time)
{
	active_cnt_ = cnt;
	//if (script_running)
	{
		DateTimeLocal time_now;
		auto time_span = time_now - last_recv_msg_time;
		if (abs(time_span.totalMilliseconds()) >= 1000 * 60)
		{
			LOG_O(Log_debug) << "SetRunInfo之最后收包时间超时了，断开连接，次数" << cnt;
#ifdef _DEBUG
			if (IDCANCEL == MessageBoxTimeout(nullptr, "SetRunInfo之最后收包时间超时了，是否断开连接", nullptr, MB_OKCANCEL, 0, 1000 * 10))
				return;
			CMN_ASSERT(false);
#endif
			func_obj_normal_.Disconnect();
		}
		else
		{
			const auto kSecond = 10;
			if (1 == cnt)
			{
				LOG_O(Log_trace) << "虽然活跃未超时，但需要每隔段时间来查看下游戏的运行信息，次数：" << cnt;
				if (func_obj_normal_.timer_active_handle_)
				{
					if (func_obj_normal_.timer_active_handle_->IsClocking())
						func_obj_normal_.timer_active_handle_->Cancel();
					func_obj_normal_.timer_active_handle_.reset();
				}
				func_obj_normal_.timer_active_handle_ = CreateIoTimer(1000 * kSecond, [this](IoTimerPtr& timer){
					timer->Continue();
					ToCall(BC_MakeP("GetRunInfo") << active_cnt_ + 1);
				}, true);
			}
			else if (cnt >= 60 * 50 / kSecond && script_running)
			{
				LOG_O(Log_debug) << "SetRunInfo之长时间未收到脚本更新信息，断开连接";
#ifdef _DEBUG
				if (IDCANCEL == MessageBoxTimeout(nullptr, "SetRunInfo之长时间未收到脚本更新信息，是否断开连接", nullptr, MB_OKCANCEL, 0, 1000 * 10))
					return;
				CMN_ASSERT(false);
#endif
				func_obj_normal_.Disconnect();
			}
		}
	}
	/*else
	{
		LOG_O(Log_trace) << "SetRunInfo之并未运行脚本";
		if (func_obj_normal_.timer_active_handle_)
		{
			if (func_obj_normal_.timer_active_handle_->IsClocking())
				func_obj_normal_.timer_active_handle_->Cancel();
			func_obj_normal_.timer_active_handle_.reset();
		}
		func_obj_normal_.last_active_time_ = DateTimeLocal();
	}*/
}

FuncObj_NormalS::stGameInfo::stGameInfo()
{
	role_lvl_ = 0;
	cur_gold_ = 0;
	free_lie_shou_cnt_ = 0;
	single_exp_ = 0;
	double_exp_ = 0;
	fatigue_data_ = 0;
}

CoorTrans_CS::CoorTrans_CS( FuncObj_NormalS& func_obj_normal, const TrackIterPtrT& track_iter ) : CoorTransTrack(&func_obj_normal, track_iter), nor_(func_obj_normal)
{
}

P_CoorTypeT CoorTrans_CS::GetCoorType() const
{
	return MakeTransRouteType(nor_.game_ac_type_, nor_.service_idx_);
}

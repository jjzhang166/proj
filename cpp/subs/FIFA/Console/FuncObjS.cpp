#include "stdafx.h"
#include "FuncObjS.h"
#include "Common/XmlCfg.h"
#include "TabOption.h"
#include "../Game/FuncObjC.h"
#include "ListCtrl_Players.h"
#include <protect/Protect.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Communicate/CG_Defines.h>
#include "MainFrm.h"
#include <Communicate/CGL_Defines.h>
#include "GlobalFuncs.h"
#include <Common/Toolhelp.h>
#include "CoordinateC.h"

FuncObj_NormalS::FuncObj_NormalS( Server& server )
	: Session(server), func_obj_inter_(this), glua_(this), json_echo_(*GLuaVM_S::GetInstance(), *this), session_oper_(*this), lua_script_(*this),
	coordinate_(*this, CoorCMgr::GetMe().MakeTransTrackIterPtr())
{
	game_ac_type_ = kGAT_Normal;
	service_idx_ = 0;
	game_process_id_ = 0;
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
	case P_Exec_MatchInfo::value:
		{
			if ( sizeof(P_Exec_MatchInfo) != param_size )
				break;
			return OnMatchInfo(*(P_Exec_MatchInfo*)param);
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
	LOG_O(Log_info) << "LUA函数名：" << func_res.call_func_name << "\t参数：" << func_res.func_param << "\t结果：" << func_res.func_result;
	return true;
}

bool FuncObj_NormalS::OnMatchInfo( P_Exec_MatchInfo& match_info )
{
	const auto& kMatchInfoPath = XmlCfg::GetCfgPath();

	std::ofstream match_info_file;
	match_info_file.open((kMatchInfoPath + "match_info.txt").c_str(), std::ios::app | std::ios::out);

	match_info_file << "本次比赛是第" << match_info.match_cnt << "次\n";
	CTime time_begin(match_info.time_match_begin), time_end(match_info.time_match_end);

	const TCHAR* kStrFormat = _T("%Y-%m-%d %I:%M:%S");

	std::string str_time_begin(CT2CA(time_begin.Format(kStrFormat)));
	std::string str_time_end(CT2CA(time_end.Format(kStrFormat)));
	match_info_file << "开始时间：" << str_time_begin << "\t结束时间：" << str_time_end;
	auto time_dist = time_end - time_begin;
	match_info_file << "\n本次比赛所花时间：" << time_dist.GetHours() << "小时" << time_dist.GetMinutes() << "分" << time_dist.GetSeconds() << "秒\n///////////////////\n";
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

	VMP_BEGIN_EX;
	auto& cfg = stSingletonCfg::GetMe();
	auto ac_pwd = cfg.FindFreeOne();
	if (!ac_pwd)
	{
		assert(false);
		Disconnect();
		return false;
	}
	if (!ac_pwd->AddBindGameAcInfo(account_, service_name_))
	{
		assert(false);
		return false;
	}
	VMP_END;
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
	auto the_login_process_id = CToolhelp::GetParentProcessID(CToolhelp::GetParentProcessID(game_proccess_info.self_process_id_));
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

		if (!SetAccountInfo(msg))
			return false;
		if ( !UserMsg(P_Modify_Account::value, &msg, sizeof(msg)) )
			return false;
		DoOnAccepted();

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

void FuncObj_NormalS::DoOnAccepted()
{
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return;
	VMP_BEGIN_EX;
	//////////////////////////////////////////////////////////////////////////
	auto& cfg = stSingletonCfg::GetMe();
	auto ac_pwd = cfg.FindByAc_Ser(account_, service_name_);
	if (!ac_pwd)
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
	bool is_login_succeed = ac_pwd->IsTheLoginSucceed() && tab_option->IsLoginSucceed();
	//设置验证信息
	session_oper_.ToCall(BC_MakeP(g_call_fn_name_set_login_succeed)<<is_login_succeed);

	auto& the_end_date_time = ac_pwd->GetUseEndDayTime();
	if ( !the_end_date_time )
		return;
	const auto& end_date_time = tab_option->GetEndDateTime();
	if ( end_date_time != *the_end_date_time )
		return;
#ifdef _DEBUG
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << Poco::DateTimeFormatter::format(end_date_time, Poco::DateTimeFormat::SORTABLE_FORMAT);
#endif
	session_oper_.ToCall(BC_MakeP("SetEndDateTime")<<end_date_time.timestamp().epochTime());

	//设置关键数据
	const auto& addr_datas = ac_pwd->GetAddrDatas();
	if (addr_datas.size())
	{
		Json::Value json_addr_datas;
		for ( const auto& addr_value : addr_datas )
		{
			json_addr_datas[json_addr_datas.size()] = (Json::UInt)addr_value;
		}
		session_oper_.ToCall(BC_MakeP("sads")<<json_addr_datas);
	}
	//////////////////////////////////////////////////////////////////////////
	//设置固定的数据
	session_oper_.ToCall(BC_MakeP("SetMatchTime")<<GenUuidStr(VMP_PROTECT_STR(kStableStr_ConsoleToGame)));
	VMP_END;
}

CoorTrans_CS& FuncObj_NormalS::GetCoorTrans()
{
	return coordinate_;
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
	session_oper_.ToCall(BC_MakeP("SetGameAcType")<<(int)game_ac_type);
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
	__super::OnDisconnect(connect);

	auto handle_process = OpenProcess(PROCESS_TERMINATE, FALSE, game_process_id_);
	if (handle_process)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "强制关闭进程id->" << game_process_id_;
		GetIoService().post([handle_process](){
			TerminateProcess(handle_process, 0);
			CloseHandle(handle_process);
		});
	}
	auto& cfg = stSingletonCfg::GetMe();
	auto ac_pwd = cfg.FindByAc_Ser(account_, service_name_);
	//assert(ac_pwd);
	if (ac_pwd)
		VERIFY(ac_pwd->DeleteTheBindGameAcInfo(account_, service_name_));

	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		return;
	}
	//设置运行状态信息
	list_ctrl->FreeItemByFuncObj(*this, str_final_run_state_);

	//掉线重登陆
	if ( !account_.empty() )
	{
		auto item = list_ctrl->GetNoProhibitItem(account_, service_name_);
		if ( item >= 0 )
		{
			list_ctrl->BeginLogin(item);
		}
		list_ctrl->DoLoginOne();
	}

	list_ctrl->SaveEpToTheFile();
}

bool FuncObj_NormalS::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	VMP_BEGIN_EX;
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return false;
	const DateTimeLocal& time_now = G_GetSerCurTime();
	if ( time_now >= tab_option->GetEndDateTime() )
		return false;
	if ( !tab_option->IsLoginSucceed() )
		return false;
	auto& cfg = stSingletonCfg::GetMe();
	if (!cfg.FindFreeOne())
		return false;
	VMP_END;
	return true;
}

SessionOperS::SessionOperS( FuncObj_NormalS& func_obj_normal ) : BufCall_SocketExcept(&func_obj_normal, kObjIdNormal), func_obj_normal_(func_obj_normal)
{

}

void SessionOperS::SetMatchAddict()
{
	func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_51End, _T("51完"));
}

void SessionOperS::SetEP( double ep )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}

	if ( 0 == func_obj_normal_.game_info_.cur_ep_ )
	{
		func_obj_normal_.game_info_.cur_ep_ = ep;
	}
	else
	{
		func_obj_normal_.game_info_.gain_total_ep_ += ep - func_obj_normal_.game_info_.cur_ep_;
		func_obj_normal_.game_info_.cur_ep_ = ep;
	}

	list_ctrl->SetEP(func_obj_normal_);
}

void SessionOperS::SetMatchCnt( int match_cnt )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}

	func_obj_normal_.game_info_.match_cnt_ = match_cnt;

	list_ctrl->SetMatchCnt(func_obj_normal_);
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

void SessionOperS::SetTrainerName( const char* str_trainer_name )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}

	func_obj_normal_.game_info_.trainer_name_ = str_trainer_name;

	list_ctrl->SetTrainerName(func_obj_normal_);
}

void SessionOperS::SetTrainerLevel( int level )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}

	func_obj_normal_.game_info_.trainer_level_ = level;

	list_ctrl->SetTrainerLevel(func_obj_normal_);
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

void SessionOperS::OnGameState( int game_state )
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}

	func_obj_normal_.game_info_.game_state_ = GType::enGameState(game_state);

	list_ctrl->OnGameState(func_obj_normal_);
}

void SessionOperS::EchoMsg( const char* msg )
{
	func_obj_normal_.GetJsonEcho().GlobalFuncCall(
		"LuaLogT", &JsonEcho::MakeParams(msg));
}

void SessionOperS::SetPlayersInfo( const Json::Value& players_array )
{
	LOG_ENTER_FUNC;
	auto page_player = PagePlayerInfo::GetInstance();
	if ( !page_player )
	{
		LOG_O(Log_error) << "page_player不存在";
		return;
	}
	auto& item_player = page_player->GetCtrlPlayer();
	item_player.DeleteAllItems();

	PlayerInfoS player_tmp;
	std::vector<PlayerInfoS> players;
	
	for ( const auto& json_player : players_array )
	{
		if ( !player_tmp.ParseFromJsonObj(json_player) )
		{
			LOG_O(Log_error) << "设置球员信息出错";
			return;
		}
		players.push_back(player_tmp);
	}

	std::sort(players.begin(), players.end(), []( const PlayerInfoS& lhs, const PlayerInfoS& rhs ){
		return lhs.player_pos_ > rhs.player_pos_;
	});

	for ( const auto& player : players )
	{
		item_player.AddPlayer(player);
	}

	page_player->SetItemsTotalCnt(players.size());

	LOG_O(Log_trace) << "成功设置球员信息";
}

void SessionOperS::SetItemsInfo( const Json::Value& items_array )
{
	LOG_ENTER_FUNC;
	auto page_item = PageItemInfo::GetInstance();
	if ( !page_item )
	{
		LOG_O(Log_error) << "page_item不存在";
		return;
	}
	auto& item_ctrl = page_item->GetCtrlItem();
	item_ctrl.DeleteAllItems();

	ItemInfoS item_info;

	int cnt = 0;
	for ( const auto& json_item : items_array )
	{
		if ( !item_info.ParseFromJsonObj(json_item) )
		{
			LOG_O(Log_error) << "设置物品信息出错";
			return;
		}
		item_ctrl.AddItemInfo(item_info);
		++cnt;
	}

	page_item->SetItemsTotalCnt(cnt);

	LOG_O(Log_trace) << "设置物品信息成功";
}

void SessionOperS::BanAccount()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "帐号->" << func_obj_normal_.account_ << "\t被封停";
	func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_BanAc, _T("帐号被封"));
}

void SessionOperS::SetMyOfferList( const Json::Value& sold_players )
{
	GetIoService().post([sold_players](){
		auto list_my_offer_list = CList_MyOfferList::GetInstance();
		if ( !list_my_offer_list )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "我的出售列表的控件不存在";
			return;
		}
		CList_MyOfferList::stSelfOfferItem offer_item;

		std::vector<CList_MyOfferList::stSelfOfferItem> offer_items;
		for ( const auto& json_item : sold_players )
		{
			if ( !offer_item.ParseFromJsonObj(json_item) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "解析出错";
				return;
			}
			
			offer_items.push_back(offer_item);
		}
		std::sort(offer_items.begin(), offer_items.end(), []( const CList_MyOfferList::stSelfOfferItem& lhs, const CList_MyOfferList::stSelfOfferItem& rhs ){
			return lhs.issued_at_ < rhs.issued_at_;
		});
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
			LOG_O(Log_warning) << "我的出售列表的控件不存在";
			return;
		}
		CList_OtherOffers::stOtherOfferItem offer_item;
		std::vector<CList_OtherOffers::stOtherOfferItem> offer_items;
		for ( const auto& json_item : sold_players )
		{
			if ( !offer_item.ParseFromJsonObj(json_item) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "解析出错";
				return;
			}
			offer_items.push_back(offer_item);
		}
		std::sort(offer_items.begin(), offer_items.end(), []( const CList_OtherOffers::stOtherOfferItem& lhs, const CList_OtherOffers::stOtherOfferItem& rhs ){
			return lhs.issued_at_ < rhs.issued_at_;
		});
		for ( const auto& item : offer_items )
		{
			list_other_offers->AddOfferItem(item);
		}
		auto page_trade = PageTrade::GetInstance();
		if (page_trade)
			page_trade->SetOtherOfferItemsCnt(offer_items.size());
	});
}

void SessionOperS::DiscoverGoodPlayer()
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}
	list_ctrl->DiscoverGoodPlayer(func_obj_normal_);
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
	ctrl_list->DoLoginOne();
}

bool SessionOperS::VersionEndTime()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "game send that version is end time";
	SetIoTimer(7 * 1000, []( IoTimer* ){
		auto main_frm = CMainFrame::GetInstance();
		if ( !main_frm )
		{
			assert(false);
			return;
		}
		main_frm->SetWindowText(VMP_PROTECT_STR(_T("你需要更新最新版本")));
		auto lua_vm = GLuaVM_S::GetInstance();
		assert(lua_vm);
		lua_vm->LuaObjCall<void>("MixLua");
	});
	return true;
}

void SessionOperS::MatchLimitCnt()
{
	func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_51End, _T("比赛次数达到上限，换号"));
}

BC_REG_CLS_DEFINE(SessionOperS){
	bufcall::class_<SessionOperS>("SessionOperS")
	.def("SetMatchAddict", &SessionOperS::SetMatchAddict)
	.def(g_call_fn_name_set_ep, &SessionOperS::SetEP)
	.def("SetMatchCnt", &SessionOperS::SetMatchCnt)
	.def("SetRunState", &SessionOperS::SetRunState)
	.def(g_call_fn_name_set_trainer_name, &SessionOperS::SetTrainerName)
	.def("SetTrainerLevel", &SessionOperS::SetTrainerLevel)
	.def("SetScriptOutputInfo", &SessionOperS::SetScriptOutputInfo)
	.def("StartRunScript", &SessionOperS::StartRunScript)
	.def("OnGameState", &SessionOperS::OnGameState)
	.def("EchoMsg", &SessionOperS::EchoMsg)
	.def("BanAccount", &SessionOperS::BanAccount)
	.def("SetPlayersInfo", &SessionOperS::SetPlayersInfo)
	.def("SetItemsInfo", &SessionOperS::SetItemsInfo)
	.def("SetMyOfferList", &SessionOperS::SetMyOfferList)
	.def("SetOtherOfferItems", &SessionOperS::SetOtherOfferItems)
	.def("DiscoverGoodPlayer", &SessionOperS::DiscoverGoodPlayer)
	.def("ExchangeGameAc", &SessionOperS::ExchangeGameAc)
	.def("ved", &SessionOperS::VersionEndTime)
	.def("MatchLimitCnt", &SessionOperS::MatchLimitCnt)
	.def("SetOutRes", &SessionOperS::SetOutRes)
	.def("SetSpid", &SessionOperS::SetSpid);
}

void SessionOperS::SetOutRes(pt_csz out_res)
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if (!list_ctrl)
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}
	auto item = list_ctrl->FindItemByFuncObj(func_obj_normal_);
	if (item < 0)
	{
		//assert(false);
		return;
	}
	list_ctrl->SetItemText(item, kSubitemOutRes, CA2CT(out_res));
}

void SessionOperS::SetSpid(pt_csz spid)
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if (!list_ctrl)
	{
		assert(false);
		func_obj_normal_.Disconnect();
		return;
	}
	auto item = list_ctrl->FindItemByFuncObj(func_obj_normal_);
	if (item < 0)
	{
		//assert(false);
		return;
	}
	CString str = CA2CT(spid);
	list_ctrl->SetItemText(item, kSubitemSpid, str);
}

FuncObj_NormalS::stGameInfo::stGameInfo()
{
	cur_ep_ = 0;
	gain_total_ep_ = 0;
	match_cnt_ = 0;
	trainer_level_ = 0;
	game_state_ = GType::enGameState_Invalid;
}

CoorTrans_CS::CoorTrans_CS( FuncObj_NormalS& func_obj_normal, const TrackIterPtrT& track_iter )
	: CoorTransTrack(&func_obj_normal, track_iter), nor_(func_obj_normal)
{

}

P_CoorTypeT CoorTrans_CS::GetCoorType() const
{
	return MakeTransRouteType(nor_.game_ac_type_, nor_.service_idx_);
}

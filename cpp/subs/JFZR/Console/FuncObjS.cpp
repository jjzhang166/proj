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

FuncObj_NormalS::FuncObj_NormalS( Server& server )
	: Session_Aes(server), func_obj_inter_(this), glua_(this), json_echo_(GLuaVM_S::GetMe(), *this), session_oper_(*this), lua_script_(GLuaVM_S::GetMe(), *this),
	coor_trans_(*this, CoorCMgr::GetMe().MakeTransTrackIterPtr())
{
	game_ac_type_ = kGAT_Normal;
	service_idx_ = 0;
	game_process_id_ = 0;
}

void FuncObj_NormalS::Accept( FuncObjVisitor* visitor )
{
	assert(visitor);
	visitor->Visit(this);
}

FuncObj_IInter& FuncObj_NormalS::GetFuncObjInter()
{
	return func_obj_inter_;
}

GLuaS& FuncObj_NormalS::GetGLua()
{
	return glua_;
}

bool FuncObj_NormalS::OnExec( P_MsgNT msg_num, void* param, size_t param_size )
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
	return __super::OnExec(msg_num, param, param_size);
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

void FuncObj_NormalS::SetAccountInfo( const P_Modify_Account& account_info )
{
	account_ = account_info.account_;
	password_ = account_info.password_;
	service_name_ = account_info.service_name_;
}

void FuncObj_NormalS::CallRemoteLua( const char* lua_func_name, const char* lua_func_param )
{
	//assert(!"用JsonCall吧，不要用这个了");
	P_Exec_CallLuaFunction msg;
	strcpy_s(msg.call_func_name, lua_func_name);
	if (lua_func_param)
		strcpy_s(msg.func_param, lua_func_param);
	Exec(P_Exec_CallLuaFunction::value, &msg, sizeof(msg));
}

bool FuncObj_NormalS::OnGameProccessInfo( const P_Exec_GameProccessInfo& game_proccess_info )
{
	auto tbl_option = CTabOption::GetInstance();
	if ( !tbl_option )
		return false;
	auto& ctrl_list = tbl_option->GetPageAccount().GetAccountList();
	if ( -1 == (int)game_proccess_info.login_client_proccess_id_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "game process info invalid";
	}
	auto item = ctrl_list.AcceptSession(game_proccess_info.login_client_proccess_id_);
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

		SetAccountInfo(msg);
		if ( !Modify(P_Modify_Account::value, &msg, sizeof(msg)) )
			return false;
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
	//设置验证信息
	session_oper_.ThisMemCall(nullptr, "SetLoginSucceed", &JsonEcho::MakeParams(tab_option->IsLoginSucceed()));

	const auto& end_date_time = tab_option->GetEndDateTime();
#ifdef _DEBUG
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << Poco::DateTimeFormatter::format(end_date_time, Poco::DateTimeFormat::SORTABLE_FORMAT);
#endif
	auto end_time = SplitInt64(end_date_time.timestamp().epochTime());
	session_oper_.ThisMemCall(nullptr, "SetEndDateTime", &JsonEcho::MakeParams(end_time.first, end_time.second));

	//设置关键数据
	const auto& addr_datas = tab_option->GetAddrDatas();
	if (addr_datas.size())
	{
		Json::Value json_addr_datas;
		for ( const auto& addr_value : addr_datas )
		{
			json_addr_datas[json_addr_datas.size()] = (Json::UInt)addr_value;
		}
		session_oper_.ThisMemCall(nullptr, "sadsex", &JsonEcho::MakeParams(json_addr_datas));
	}
	//////////////////////////////////////////////////////////////////////////
	//设置固定的数据
	session_oper_.ThisMemCall(nullptr, "SetMatchTime", &JsonEcho::MakeParams(GenUuidStr(VMP_PROTECT_STR(kStableStr_ConsoleToGame))));
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
	session_oper_.ThisMemCall(nullptr, "SetGameAcType", &JsonEcho::MakeParams(game_ac_type));
}

void FuncObj_NormalS::SetServiceIdx( int service_idx )
{
	service_idx_ = service_idx;
	session_oper_.ThisMemCall(nullptr, "SetGameSerIdx", &JsonEcho::MakeParams(service_idx));
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
	//return;
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
	{
		assert(false);
		return;
	}
	//设置运行状态信息
	if ( !str_final_run_state_.IsEmpty() )
	{
		auto item = list_ctrl->FindItemByAccount(account_, service_name_);
		if ( item >= 0 )
			list_ctrl->SetItemText(item, kSubitemRunState, str_final_run_state_);
	}

	//掉线重登陆
	if ( !account_.empty() )
	{
		auto item = list_ctrl->GetNoProhibitItem(account_, service_name_);
		if ( item >= 0 )
		{
			list_ctrl->BeginLogin(item);
		}
		list_ctrl->DoLoginOne();

		if ( stSpecificCfg::kPR_Restricted == list_ctrl->GetProhibitReason(account_, service_name_) )
		{
			item = list_ctrl->FindItemByAccount(account_, service_name_);
			if ( item < 0 )
			{
				assert(false);
				return;
			}
			list_ctrl->RemoveProhibitItem(item);
			list_ctrl->AddLoginItem(item);
		}
	}
}

bool FuncObj_NormalS::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	VMP_BEGIN_EX;
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return false;
	Poco::DateTime time_now;
	if ( time_now >= tab_option->GetEndDateTime() )
		return false;
	if ( !tab_option->IsLoginSucceed() )
		return false;
	VMP_END;
	DoSomeConnected();
	return true;
}

void FuncObj_CreateTrainerS::Accept( FuncObjVisitor* visitor )
{
	assert(visitor);
	visitor->Visit(this);
}

SessionOperS::SessionOperS( FuncObj_NormalS& func_obj_normal ) : JsonEchoBase(GLuaVM_S::GetMe(), func_obj_normal, kObjIdNormal), func_obj_normal_(func_obj_normal)
{

}

bool SessionOperS::OnThisMemCall( const stThisCallHelper& helper )
{
	return OnThisFuncCallImpl(this, helper);
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

void SessionOperS::SetPlayersInfo( const Json::Value& players_array )
{
}

void SessionOperS::SetItemsInfo( const Json::Value& items_array )
{

}

void SessionOperS::BanAccount()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "帐号->" << func_obj_normal_.account_ << "\t被封停";
	func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_BanAc, _T("帐号被封"));
}

void SessionOperS::SetMyOfferList( const Json::Value& sold_players )
{
	
}

void SessionOperS::SetOtherOfferItems( const Json::Value& sold_players )
{
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

void SessionOperS::SetCurHp( int hp )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.cur_hp_ = hp;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);
}

void SessionOperS::SetRoleName( const std::string& role_name )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.role_name_ = CA2CT(role_name.c_str());
	if ( func_obj_normal_.game_info_.role_name_a_ != role_name )
	{
		func_obj_normal_.game_info_.gained_gold_ = 0;
		func_obj_normal_.game_info_.cur_gold_ = 0;
	}
	func_obj_normal_.game_info_.role_name_a_ = role_name;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);
}

void SessionOperS::SetRoleLvl( int role_lvl )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.role_lvl_ = role_lvl;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);
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
	if ( 0 == func_obj_normal_.game_info_.cur_gold_ )
	{
		func_obj_normal_.game_info_.cur_gold_ = cur_gold;
	}
	else
	{
		func_obj_normal_.game_info_.gained_gold_ += cur_gold - func_obj_normal_.game_info_.cur_gold_;
		func_obj_normal_.game_info_.cur_gold_ = cur_gold;
	}
	ctrl_list->UpdateRoleInfo(func_obj_normal_);
}

void SessionOperS::SetJobName( const std::string& job_name )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	func_obj_normal_.game_info_.job_name_ = CA2CT(job_name.c_str());
	ctrl_list->UpdateRoleInfo(func_obj_normal_);

	auto specific_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specific_cfg )
	{
		assert(false);
		return;
	}
	if (func_obj_normal_.game_info_.role_name_a_.empty())
		return;
	specific_cfg->SetRoleInfo(func_obj_normal_.game_info_.role_name_a_, job_name);
}

void SessionOperS::SetFatigueData( int fatigue_data )
{
	auto ctrl_list = PageAccount_List::GetInstance();
	if ( !ctrl_list )
		return;
	assert(fatigue_data >= 0);
	func_obj_normal_.game_info_.fatigue_data_ = fatigue_data;
	ctrl_list->UpdateRoleInfo(func_obj_normal_);
	if (func_obj_normal_.game_info_.role_name_a_.empty())
		return;
	auto specific_cfg = ctrl_list->GenSpecificCfg(func_obj_normal_.account_, func_obj_normal_.service_name_);
	if ( !specific_cfg )
	{
		assert(false);
		return;
	}
	specific_cfg->SetRoleFatigueData(func_obj_normal_.game_info_.role_name_a_, fatigue_data);
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
			func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_RolesUseOut, _T("角色刷完"));
			return;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "控制台开始远程创建角色";
		if ( !ThisMemCall(nullptr, "CreateRandNameRole", nullptr) )
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
			func_obj_normal_.AddProhibitItem(stSpecificCfg::kPR_RolesUseOut, _T("角色刷完"));
			return;
		}
	}
	assert(role_info);
	assert(role_info->fatigue_data_ > 0);
	if ( !ThisMemCall(nullptr, "SelectRole", &JsonEcho::MakeParams(role_info->role_info_.role_name_)) )
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
	if ( !ThisMemCall(nullptr, "CreateRandNameRole", nullptr) )
	{
		//assert(false);
		LOG_O(Log_debug) << "控制台远程调用创建角色失败";
		return;
	}
}

FuncObj_NormalS::stGameInfo::stGameInfo()
{
	cur_hp_ = 0;
	role_lvl_ = 0;
	cur_gold_ = 0;
	gained_gold_ = 0;
	fatigue_data_ = 0;
}

CoorTrans_CS::CoorTrans_CS( FuncObj_NormalS& func_obj_normal, const TrackIterPtrT& track_iter ) : CoorTransTrack(&func_obj_normal, track_iter), nor_(func_obj_normal)
{
}

P_CoorTypeT CoorTrans_CS::GetCoorType() const
{
	return nor_.game_ac_type_ + (nor_.service_idx_ << 4);
}

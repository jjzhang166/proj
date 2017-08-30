#include "stdafx.h"
#include "CheckServerS.h"
#include "LuaVM_S.h"
#include "CheckServerDlg.h"
#include <CmnMix/Cmn_TemplateUtility.h>
#include <Protocol/P_PriDefines.h>
#include "CS_CfgInfo.h"

CheckServer::CheckServer( DbSingleton::SelfPtr& db_singleton, WORD self_listen_port ) 
	: Server_Aes_Rsa(self_listen_port)
{
	db_singleton_ = db_singleton;
	SetIoTimer(1000, []( IoTimer* timer ){
		auto check_s = CheckServer::GetInstance();
		if ( !check_s )
		{
			assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "验证服务对象不存在";
			(void*)0;
		}
		else
		{
			check_s->SessionTimeout();
		}
		assert(timer);
		timer->Continue();
	});
}

Session* CheckServer::CreateSession()
{
	if (!rsa_key_.public_key_ || !rsa_key_.private_key_)
	{
		assert(false);
		return nullptr;
	}
	auto ses = new Session_CS(*this);
	assert(ses);
	ses->SetSingleDb(db_singleton_);
	return ses;
}

void CheckServer::DoItem( const std::function<void(Session_CS&)>& func )
{
	for ( auto& item : sessions_ )
	{
		if ( !item )
			continue;
		if ( !item->socket().is_open() )
			continue;
		auto shared_this = boost::static_pointer_cast<Session_CS>(item->shared_from_this());
		if ( !shared_this )
			continue;
		func(*shared_this);
	}
}

void CheckServer::SessionTimeout()
{
	DateTimeLocal time_now;
	std::vector<Session_CS*> ses_vec;
	DoItem([&time_now, &ses_vec]( Session_CS& session ){
		if (session.IsTimeout(time_now))
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "user name->" << session.GetLoginUserName() << "\t超时断开连接";
			ses_vec.push_back(&session);
		}
	});
	for ( auto ses : ses_vec )
	{
		if ( !ses )
		{
			assert(false);
			continue;
		}
		ses->Disconnect();
	}
}

bool CheckServer::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	auto ptr = PointerCast<Session>(connect);
	sessions_.insert(SmartCast<session_ptr>(ptr->shared_from_this()));
	return true;
}

void CheckServer::OnDisconnect( IoConnectBase* connect )
{
	__super::OnDisconnect(connect);
	auto ptr = PointerCast<Session>(connect);
	sessions_.erase(SmartCast<session_ptr>(ptr->shared_from_this()));
}

Session_CS::Session_CS( CheckServer& server ) : Session_Rsa(server, server.GetRsaPublicKey(),
																server.GetRsaPrivateKey()),
	user_oper_(*this)
{

}

bool UserOperS::Login(pt_bool is_only_login, const stCheckLoginInfo<pt_csz>& login_info, pt_int ver)
{
	if ( !session_.user_name_.empty() || !session_.db_singleton_ || !session_.ObjValid() )
	{
		session_.Disconnect();
		return false;
	}
	session_.user_name_.clear();
	session_.ActiveSessionTime();
	
	bool res = false;
	std::string info;
	auto& cfg = stCS_CfgInfo::GetMe();
	std::string str_new_cookie;
	pt_dword ac_type = 0;
	if ( ver >= cfg.min_version_ )
	{
		const auto& login_ip = session_.socket_.remote_endpoint().address().to_string();
		if (session_.db_singleton_->Login(login_info.ac_.value_, login_info.pwd_.value_,
			login_info.machine_code_.value_, login_info.str_mix_.value_, login_info.cookie_.value_, &str_new_cookie,
			&login_ip, &info, ac_type))
		{
			res = true;
		}
	}
	else
		info = "请更换最新版本";

	if ( !res )
	{
		std::time_t time_zero = 0;
		return OnLoginRes(res, info.c_str(), nullptr, nullptr, time_zero, time_zero, AddrDatasT(), StrDatasT(), ac_type);
	}

	DateTimeLocal user_end_date_time(1980, 1, 1);
	//用户使用期限
	if (!session_.db_singleton_->GetTbl_Account().GetEndDateTime(login_info.ac_.value_, user_end_date_time))
		res = false;

	AddrDatasT addr_datas;
	if (!session_.db_singleton_->GetAddrDatas(addr_datas.first, addr_datas.second))
	{
		res = false;
	}
	StrDatasT str_data;
	if (!session_.db_singleton_->GetStrDatas(str_data))
	{
		res = false;
	}

	if ( !res )
	{
		info = "未知严重的错误985";
		std::time_t time_zero = 0;
		return OnLoginRes(res, info.c_str(), nullptr, nullptr, time_zero, time_zero, AddrDatasT(), StrDatasT(), ac_type);
	}

	if (!is_only_login)
	{
		session_.user_name_ = login_info.ac_.value_;
	}
	const auto& the_mix_str = GenUuidStr(login_info.str_mix_.value_, kUuidStrLen);
	return OnLoginRes(res, info.c_str(), the_mix_str.c_str(), str_new_cookie.c_str(),
		user_end_date_time.timestamp().epochTime(), DateTimeLocal().timestamp().epochTime(), addr_datas, str_data, ac_type);
}

void Session_CS::SetSingleDb( DbSingleton::SelfPtr& db_singleton )
{
	db_singleton_ = db_singleton;
}

Session_CS::~Session_CS()
{
	/*if (timer_active_)
		timer_active_->Cancel();*/
}

void Session_CS::ActiveSessionTime()
{
	active_time_ = DateTimeLocal();
	/*const size_t kTimeout = / *5 * 1000* /kCheckSvrActiveTime;
	if ( !timer_active_ )
	{
		auto shared_this = shared_from_this();
		timer_active_ = CreateIoTimer(kTimeout, [=]( IoTimerPtr io_timer ){
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "长时间未活动，关闭连接";
			shared_this->Disconnect();
		}, true);
		return;
	}

	assert(timer_active_);
	timer_active_->Cancel();
	auto shared_this = boost::static_pointer_cast<Session_CS>(shared_from_this());
	GetIoService().post([=](){
		if (shared_this)
		{
			if (shared_this->timer_active_)
				shared_this->timer_active_->Continue();
		}
	});*/
}

bool Session_CS::IsTimeout( const DateTimeLocal& time_now ) const
{
	if ( time_now < active_time_ )
	{
		assert(false);
		return true;
	}
	auto time_span = time_now - active_time_;
	return time_span.totalMilliseconds() >= kCheckSvrActiveTime;
}

const std::string& Session_CS::GetLoginUserName() const
{
	return user_name_;
}

void Session_CS::OnDisconnect( IoConnectBase* connect )
{
	__super::OnDisconnect(connect);
}

bool Session_CS::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "接受一个连接，目标IP->" << socket_.remote_endpoint().address().to_string();
	//assert(!timer_active_);
	ActiveSessionTime();
	return true;
}

UserOperS::UserOperS( Session_CS& session ) : BufCall_Socket(&session, kFuncObjTemplateId_Login), session_(session)
{

}

UserOperS::~UserOperS()
{
}

bool UserOperS::RegUser( pt_csz user_name, pt_csz user_pwd, pt_csz str_qq, pt_csz reg_key_id )
{
	if ( !P_StringCheck::ValidUserName(user_name) || !P_StringCheck::ValidUserPwd(user_pwd) || !P_StringCheck::ValidQQ(str_qq) || !P_StringCheck::ValidCard(reg_key_id) )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	bool res = session_.db_singleton_->RegUser(user_name, user_pwd, str_qq, 
		session_.socket_.remote_endpoint().address().to_string(), reg_key_id, info);
	return OnRegUser(res, info.c_str());
}

bool UserOperS::UserPay( pt_csz user_name, pt_csz card_id, pt_csz card_pwd )
{
	if ( !P_StringCheck::ValidUserName(user_name) || !P_StringCheck::ValidCard(card_id) || !P_StringCheck::ValidCard(card_pwd) )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	bool res = session_.db_singleton_->UserPay(user_name, card_id, card_pwd, info);
	return OnUserPay(res, info.c_str());
}

bool UserOperS::Grant_OpenCard(pt_word card_cnt, pt_dword card_minutes, pt_qword grant, pt_dword ac_type)
{
	if ( card_cnt <= 0 || card_minutes <= 0 )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	DataH_Card::CardsInfoT vec_cards_info;
	auto res = session_.db_singleton_->Grant_OpenCard(session_.user_name_, card_cnt, card_minutes, ac_type, &vec_cards_info, grant, info);

	return OnGrant_OpenCard(res, info.c_str(), vec_cards_info);
}

bool UserOperS::RemoveBind( pt_csz user_name, pt_csz user_pwd, pt_csz machine_code )
{
	if ( !P_StringCheck::ValidUserName(user_name) || !P_StringCheck::ValidUserPwd(user_pwd) || !P_StringCheck::ValidMachineCode(machine_code) )
	{
		assert(false);
		session_.Disconnect();
		return false;
	}
	auto check_dlg = CCheckServerDlg::GetInstance();
	if ( !check_dlg )
	{
		assert(false);
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	bool res = session_.db_singleton_->RemoveBind(user_name, user_pwd, machine_code, check_dlg->CalcDecrementMinutes(), info);

	return OnRemoveBind(res, info.c_str());
}

bool UserOperS::Grant_RemoveBind( pt_csz user_name_dst )
{
	if ( !P_StringCheck::ValidUserName(user_name_dst) )
	{
		assert(false);
		session_.Disconnect();
		return false;
	}
	auto check_dlg = CCheckServerDlg::GetInstance();
	if ( !check_dlg )
	{
		assert(false);
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		assert(false);
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		assert(false);
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	auto res = session_.db_singleton_->Grant_RemoveBind(session_.user_name_, user_name_dst, check_dlg->CalcDecrementMinutes(), info);
	return OnGrant_RemoveBind(res, info.c_str());
}

bool UserOperS::CheckLogin(const CheckLoginInfoContT<pt_csz>& login_infos)
{
	if ( !session_.db_singleton_ || !session_.ObjValid() )
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();
	/*
	验证登陆操作不能更新cookie，必须得由正常的登陆操作才能更新cookie。
	因为如果用验证登陆操作来更新cookie的话，那么这将会是一个漏洞，破解者可能直接通过验证登陆操作来更新cookie。
	验证登陆操作必须只做“只读”的事情。
	*/

	CheckLoginResContT login_res;
	auto& the_db = *session_.db_singleton_;
	pt_dword ac_type = 0;
	for (auto& v : login_infos.value_)
	{
		if (!the_db.Login(v.ac_.value_, v.pwd_.value_, v.machine_code_.value_,
			v.str_mix_.value_, v.cookie_.value_, nullptr, nullptr, nullptr, ac_type))
		{
			stCheckLoginResInfo res_tmp;
			res_tmp.ac_ = v.ac_.value_;
			login_res.push_back(res_tmp);
		}
	}

	return OnCheckLogin(login_res);
}

bool UserOperS::Grant_GenRegKey(pt_word reg_key_cnt, pt_dword ac_type)
{
	if ( reg_key_cnt <= 0 )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	DataH_RegKey::RegKeysInfoT vec_cards_info;
	auto res = session_.db_singleton_->Grant_GenRegKey(session_.user_name_, reg_key_cnt, ac_type, &vec_cards_info, info);
	return OnGrant_GenRegKey(res, info.c_str(), vec_cards_info);
}

bool UserOperS::LeaveMsg( pt_csz msg )
{
	if ( !P_StringCheck::ValidMsgBoard(msg) )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	auto res = session_.db_singleton_->UserLeaveMsg(msg, session_.user_name_, info);
	return OnLeaveMsg(res, info.c_str());
}

bool UserOperS::Grant_PeerAcByCard( pt_csz card_id, pt_csz card_pwd )
{
	if ( !P_StringCheck::ValidCard(card_id) || !P_StringCheck::ValidCard(card_pwd) )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	stData_Account::AcContT ac_cont;
	auto res = session_.db_singleton_->Grant_PeerAcByCard(session_.user_name_, card_id, card_pwd, ac_cont, info);
	stData_AcState::ContT ac_states;
	session_.db_singleton_->GetAcStates(ac_cont, ac_states);
	return OnGrant_PeerAcByCard(res, info.c_str(), ac_cont, ac_states);
}

bool UserOperS::Grant_PeerCardByAc( pt_csz ac, pt_csz ac_pwd )
{
	if ( !P_StringCheck::ValidUserName(ac) || !P_StringCheck::ValidUserPwd(ac_pwd) )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	stData_Card::CardContT card_cont;
	auto res = session_.db_singleton_->Grant_PeerCardByAc(session_.user_name_, ac, ac_pwd, card_cont, info);
	return OnGrant_PeerCardByAc(res, info.c_str(), card_cont);
}

bool UserOperS::Grant_PeerAcInfo( pt_csz ac, pt_csz ac_pwd )
{
	if ( !P_StringCheck::ValidUserName(ac) || !P_StringCheck::ValidUserPwd(ac_pwd) )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	stData_Account::AcContT ac_cont;
	auto res = session_.db_singleton_->Grant_PeerAcInfo(session_.user_name_, ac, ac_pwd, ac_cont, info);
	stData_AcState::ContT ac_states;
	session_.db_singleton_->GetAcStates(ac_cont, ac_states);
	return OnGrant_PeerAcInfo(res, info.c_str(), ac_cont, ac_states);
}

bool UserOperS::Grant_PeerCardInfo( pt_csz card_id, pt_csz card_pwd )
{
	if ( !P_StringCheck::ValidCard(card_id) || !P_StringCheck::ValidCard(card_pwd) )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	stData_Card::CardContT card_cont;
	auto res = session_.db_singleton_->Grant_PeerCardInfo(session_.user_name_, card_id, card_pwd, card_cont, info);
	return OnGrant_PeerCardInfo(res, info.c_str(), card_cont);
}

bool UserOperS::Grant_RollbackPay( pt_csz card_id, pt_csz card_pwd, bool direction )
{
	if ( !P_StringCheck::ValidCard(card_id) || !P_StringCheck::ValidCard(card_pwd) )
	{
		session_.Disconnect();
		return false;
	}
	if ( !session_.db_singleton_ )
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	auto res = session_.db_singleton_->Grant_RollbackPay(session_.user_name_, card_id, card_pwd, direction, info);
	return OnGrant_RollbackPay(res, info.c_str());
}

bool UserOperS::Grant_SetAddrData( pt_int id, pt_dword addr_data )
{
	if (!session_.db_singleton_)
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	bool res = false;
	if ( !session_.db_singleton_->GetTbl_UserGrant().HasTheGrant(session_.user_name_, kUG_ModifyProjData) )
	{
		info = "没有该权限";
	}
	else
		res = session_.db_singleton_->Grant_SetAddrData(id, addr_data, info);
	return OnGrant_SetAddrData(res, info.c_str());
}

bool UserOperS::Grant_SetStrData( pt_int id, pt_csz str_data )
{
	if ( !P_StringCheck::ValidStrData(str_data) )
	{
		session_.Disconnect();
		return false;
	}
	if (!session_.db_singleton_)
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	bool res = false;
	if ( !session_.db_singleton_->GetTbl_UserGrant().HasTheGrant(session_.user_name_, kUG_ModifyProjData) )
	{
		info = "没有该权限";
	}
	else
		res = session_.db_singleton_->Grant_SetStrData(id, str_data, info);
	return OnGrant_SetStrData(res, info.c_str());
}

BC_REG_CLS_DEFINE(UserOperS){
	bufcall::class_<UserOperS>("UserOperS")
	.def("dlo", &UserOperS::Login)
	.def("RegUser", &UserOperS::RegUser)
	.def("UserPay", &UserOperS::UserPay)
	.def("Grant_OpenCard", &UserOperS::Grant_OpenCard)
	.def("RemoveBind", &UserOperS::RemoveBind)
	.def("Grant_RemoveBind", &UserOperS::Grant_RemoveBind)
	.def("ckl", &UserOperS::CheckLogin)
	.def("Grant_GenRegKey", &UserOperS::Grant_GenRegKey)
	.def("LeaveMsg", &UserOperS::LeaveMsg)
	.def("Grant_PeerAcByCard", &UserOperS::Grant_PeerAcByCard)
	.def("Grant_PeerCardByAc", &UserOperS::Grant_PeerCardByAc)
	.def("Grant_PeerAcInfo", &UserOperS::Grant_PeerAcInfo)
	.def("Grant_PeerCardInfo", &UserOperS::Grant_PeerCardInfo)
	.def("Grant_RollbackPay", &UserOperS::Grant_RollbackPay)
	.def("Grant_SetAddrData", &UserOperS::Grant_SetAddrData)
	.def("Grant_SetStrData", &UserOperS::Grant_SetStrData)
	.def("Grant_SetAcState", &UserOperS::Grant_SetAcState);
}

bool UserOperS::OnLoginRes( bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, const TimeT& end_day_time, 
	const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas, pt_dword ac_type)
{
	return ToCall(BC_MakeP("OnLoginRes")<<res<<info<<ac_pwd_machine_mix<<new_cookie<<end_day_time<<ser_cur_time<<addr_datas<<str_addr_datas<<ac_type);
}

bool UserOperS::OnRemoveBind( bool res, pt_csz info )
{
	return ToCall(BC_MakeP("OnRemoveBind")<<res<<info);
}

bool UserOperS::OnRegUser( bool res, pt_csz info )
{
	return ToCall(BC_MakeP("OnRegUser")<<res<<info);
}

bool UserOperS::OnUserPay( bool res, pt_csz info )
{
	return ToCall(BC_MakeP("OnUserPay")<<res<<info);
}

bool UserOperS::OnLeaveMsg( bool res, pt_csz info )
{
	return ToCall(BC_MakeP("OnLeaveMsg")<<res<<info);
}

bool UserOperS::OnCheckLogin(const CheckLoginResContT& res_info)
{
	return ToCall(BC_MakeP("OnCheckLogin")<<res_info);
}

bool UserOperS::OnGrant_RemoveBind( bool res, pt_csz info )
{
	return ToCall(BC_MakeP("OnGrant_RemoveBind")<<res<<info);
}

bool UserOperS::OnGrant_OpenCard( bool res, pt_csz info, const CardInfoT& card_info )
{
	return ToCall(BC_MakeP("OnGrant_OpenCard")<<res<<info<<card_info);
}

bool UserOperS::OnGrant_GenRegKey( bool res, pt_csz info, const RegKeyInfoT& reg_key )
{
	return ToCall(BC_MakeP("OnGrant_GenRegKey")<<res<<info<<reg_key);
}

bool UserOperS::OnGrant_PeerAcByCard(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	return ToCall(BC_MakeP("OnGrant_PeerAcByCard")<<res<<info<<ac_info<<ac_state);
}

bool UserOperS::OnGrant_PeerCardByAc(bool res, pt_csz info, const stData_Card::CardContT& card_info)
{
	return ToCall(BC_MakeP("OnGrant_PeerCardByAc")<<res<<info<<card_info);
}

bool UserOperS::OnGrant_PeerAcInfo(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	return ToCall(BC_MakeP("OnGrant_PeerAcInfo")<<res<<info<<ac_info<<ac_state);
}

bool UserOperS::OnGrant_PeerCardInfo(bool res, pt_csz info, const stData_Card::CardContT& card_info)
{
	return ToCall(BC_MakeP("OnGrant_PeerCardInfo")<<res<<info<<card_info);
}

bool UserOperS::OnGrant_RollbackPay( bool res, pt_csz info )
{
	return ToCall(BC_MakeP("OnGrant_RollbackPay")<<res<<info);
}

bool UserOperS::OnGrant_SetAddrData( bool res, pt_csz info )
{
	return ToCall(BC_MakeP("OnGrant_SetAddrData")<<res<<info);
}

bool UserOperS::OnGrant_SetStrData( bool res, pt_csz info )
{
	return ToCall(BC_MakeP("OnGrant_SetStrData")<<res<<info);
}

bool UserOperS::Grant_SetAcState(pt_csz user_name_dst, enAcState ac_state, Poco::UInt32 period)
{
	if (!P_StringCheck::ValidUserName(user_name_dst))
	{
		session_.Disconnect();
		return false;
	}
	if (!session_.db_singleton_)
	{
		session_.Disconnect();
		return false;
	}
	if (session_.user_name_.empty())
	{
		session_.Disconnect();
		return false;
	}
	session_.ActiveSessionTime();

	std::string info;
	bool res = session_.db_singleton_->Grant_SetAcState(session_.user_name_, user_name_dst, ac_state, period, info);
	return OnGrant_SetAcState(res, info.c_str());
}

bool UserOperS::OnGrant_SetAcState(bool res, pt_csz info)
{
	return ToCall(BC_MakeP("OnGrant_SetAcState") << res << info);
}

#include "stdafx.h"
#include <Net/CmnLogin.h>
#include <Common/UsefulClass.h>
#include <CmnMix/Cmn_LogOut.h>


CmnCheckLoginOperC::CmnCheckLoginOperC( ICmnLoginSink& sink, FuncObj* parent ) : login_sink_(sink), BufCall_SocketExcept(parent, kFuncObjTemplateId_Login)
{
	login_res_ = false;
	remove_bind_res_ = false;
	reg_user_res_ = false;
	user_pay_res_ = false;
	leave_msg_res_ = false;
	check_login_res_ = false;
}

BC_REG_CLS_DEFINE(CmnCheckLoginOperC){
	bufcall::class_<CmnCheckLoginOperC>("CmnCheckLoginOperC")
	.def("OnLoginRes", &CmnCheckLoginOperC::OnLoginRes)
	.def("OnRemoveBind", &CmnCheckLoginOperC::OnRemoveBind)
	.def("OnRegUser", &CmnCheckLoginOperC::OnRegUser)
	.def("OnUserPay", &CmnCheckLoginOperC::OnUserPay)
	.def("OnLeaveMsg", &CmnCheckLoginOperC::OnLeaveMsg)
	.def("OnCheckLogin", &CmnCheckLoginOperC::OnCheckLogin);
}

bool CmnCheckLoginOperC::Login( bool is_only_login, pt_csz user_name, pt_csz user_pwd, pt_int version )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidUserName(user_name) || !P_StringCheck::ValidUserPwd(user_pwd) )
	{
		assert(false);
		return false;
	}

	login_res_.reset();
	const auto& mcode = GetMachineCode();
	std::string str_login_mix_key = user_name;
	str_login_mix_key += user_pwd;
	str_login_mix_key += mcode;

	user_account_tmp_.resize(StdEx::TStrLen(user_name));
	XorBufferData((char*)user_account_tmp_.c_str(), user_name, user_account_tmp_.size(), 10);
	str_login_mix_key = GenUuidStr(str_login_mix_key);
	const auto& str_cookie = GainLocalInfo::GetMe().GetCookie(str_login_mix_key);
	//const auto& str_cookie = CookieHelper::GetMe().GetCookie(GenUuidStr(str_login_mix_key));
	stCheckLoginInfo<pt_csz> login_info;
	login_info.ac_ = user_name;
	login_info.pwd_ = user_pwd;
	login_info.machine_code_ = mcode.c_str();
	login_info.str_mix_ = str_login_mix_key.c_str();
	login_info.cookie_ = str_cookie.c_str();

	return ToCallSync(BC_MakeP("dlo")<<is_only_login<<login_info<<version, login_res_);
	VMP_END;
}

bool CmnCheckLoginOperC::RemoveBind( pt_csz user_name, pt_csz user_pwd )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidUserName(user_name) || !P_StringCheck::ValidUserPwd(user_pwd) )
	{
		assert(false);
		return false;
	}
	user_account_tmp_.resize(StdEx::TStrLen(user_name));
	XorBufferData((char*)user_account_tmp_.c_str(), user_name, user_account_tmp_.size(), 11);
	return ToCallSync(BC_MakeP("RemoveBind")<<user_name<<user_pwd<<GetMachineCode(), remove_bind_res_);
	VMP_END;
}

bool CmnCheckLoginOperC::RegUser( pt_csz user_name, pt_csz user_pwd, pt_csz str_qq, pt_csz reg_key_id )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidUserName(user_name) || !P_StringCheck::ValidUserPwd(user_pwd) || !P_StringCheck::ValidCard(reg_key_id) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("RegUser")<<user_name<<user_pwd<<str_qq<<reg_key_id, reg_user_res_);
	VMP_END;
}

bool CmnCheckLoginOperC::UserPay( pt_csz user_name, pt_csz card_id, pt_csz card_pwd )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidUserName(user_name) || !P_StringCheck::ValidCard(card_id) || !P_StringCheck::ValidCard(card_pwd) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("UserPay")<<user_name<<card_id<<card_pwd, user_pay_res_);
	VMP_END;
}

bool CmnCheckLoginOperC::LeaveMsg( pt_csz msg )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidMsgBoard(msg) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("LeaveMsg")<<msg, leave_msg_res_);
	VMP_END;
}

inline void OnRecvAddrDatas_(std::vector<DWORD>& addr_vec, const std::string& str_key, const CmnCheckLoginOperC::AddrDatasContT& addr_datas)
{
	if (addr_datas.size() <= 0)
		return;
	Secret_XorKey algo_xor_key;
	algo_xor_key.SetKey(str_key.c_str(), str_key.size());
	char cstr_tmp[50];
	std::string str_original;
	for (const auto& addr_data : addr_datas)
	{
		if (addr_data.size() <= 0)
			return;
		algo_xor_key.Decrypt(addr_data.c_str(), addr_data.size(), cstr_tmp);
		str_original.assign(cstr_tmp, addr_data.size());
		auto v = std::stoi(str_original);
		addr_vec.push_back(v);
	}
	//LOG_O(Log_trace) << "接收关键数据成功,size->" << addr_datas.size();
}

bool CmnCheckLoginOperC::OnLoginRes( bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, const TimeT& end_day_time,
	const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas, pt_dword ac_type)
{
	VMP_BEGIN_EX;
	if (login_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9871a");
	}
	login_res_ = res;

	std::string user_name;
	user_name.resize(user_account_tmp_.size());
	XorBufferData((char*)user_name.c_str(), user_account_tmp_.c_str(), user_account_tmp_.size(), 10);
	if (res)
	{
		G_SetCurTime(ser_cur_time);
		//LOG_O(Log_trace) << "更新cookie:" << new_cookie;
		GainLocalInfo::GetMe().SetCookie(ac_pwd_machine_mix, new_cookie);
		//CookieHelper::GetMe().UpdateCookie(ac_pwd_machine_mix, new_cookie);

		auto login_info_mgr = CmnLoginInfoMgrBaseImpl::GetDerived<CmnLoginInfoMgrBase>();
		if (login_info_mgr)
		{
			auto login_info = login_info_mgr->FindByAc(user_name);
			if (!login_info)
			{
				assert(false);
				return false;
			}
			login_info->GenUuid();
			login_info->SetLoginRes(res);
			login_info->SetAcType(ac_type);
			login_info->SetLoginInfo(std::string(info));
			login_info->SetUseEndDayTime(DateTimeLocal(Poco::Timestamp::fromEpochTime(end_day_time)));
			std::vector<DWORD> addr_vec;
			OnRecvAddrDatas_(addr_vec, addr_datas.first, addr_datas.second);
			login_info->SetAddrDatas(addr_vec);
			login_info->SetStrDatas(str_addr_datas);

			login_info_mgr->RectifyAcceptMaxCnt();
		}
	}
	else
	{
		auto login_info_mgr = CmnLoginInfoMgrBaseImpl::GetDerived<CmnLoginInfoMgrBase>();
		if (login_info_mgr)
		{
			auto login_info = login_info_mgr->FindByAc(user_name);
			if (!login_info)
			{
				assert(false);
				return false;
			}
			login_info->GenUuid();
			login_info->ResetAll();
			login_info->SetLoginInfo(std::string(info));
		}
	}
	return login_sink_.OnLoginRes(res, info, ac_pwd_machine_mix, new_cookie, end_day_time, ser_cur_time, addr_datas, str_addr_datas, ac_type);
	VMP_END;
}

bool CmnCheckLoginOperC::OnRemoveBind( bool res, pt_csz info )
{
	VMP_BEGIN_EX;
	if (remove_bind_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9871b");
	}
	remove_bind_res_ = res;

	if (res)
	{
		std::string user_name;
		user_name.resize(user_account_tmp_.size());
		XorBufferData((char*)user_name.c_str(), user_account_tmp_.c_str(), user_account_tmp_.size(), 11);

		auto login_info_mgr = CmnLoginInfoMgrBaseImpl::GetDerived<CmnLoginInfoMgrBase>();
		if (login_info_mgr)
		{
			auto login_info = login_info_mgr->FindByAc(user_name);
			if (login_info)
			{
				login_info->GenUuid();
				login_info->ResetAll();
			}
		}
	}
	return login_sink_.OnRemoveBind(res, info);
	VMP_END;
}

bool CmnCheckLoginOperC::OnRegUser( bool res, pt_csz info )
{
	VMP_BEGIN_EX;
	if (reg_user_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9871c");
	}
	reg_user_res_ = res;
	return login_sink_.OnRegUser(res, info);
	VMP_END;
}

bool CmnCheckLoginOperC::OnUserPay( bool res, pt_csz info )
{
	VMP_BEGIN_EX;
	if (user_pay_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9871d");
	}
	user_pay_res_ = res;
	return login_sink_.OnUserPay(res, info);
	VMP_END;
}

bool CmnCheckLoginOperC::OnLeaveMsg( bool res, pt_csz info )
{
	VMP_BEGIN_EX;
	if (leave_msg_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9871f");
	}
	leave_msg_res_ = res;
	return login_sink_.OnLeaveMsg(res, info);
	VMP_END;
}

bool CmnCheckLoginOperC::OnCheckLogin(const CheckLoginResContT& res_info)
{
	VMP_BEGIN_EX;
	if (check_login_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9871g");
	}
	check_login_res_ = true;
	auto login_info_mgr = CmnLoginInfoMgrBaseImpl::GetDerived<CmnLoginInfoMgrBase>();
	if (!login_info_mgr)
	{
		assert(false);
		throw CmnExitIoThreadException("fdjaoeu");
	}
	for (auto& v : res_info)
	{
		auto login_info = login_info_mgr->FindByAc(v.ac_);
		if (!login_info)
		{
			assert(false);
			throw CmnExitIoThreadException("fdjuiqr");
		}
		login_info->ResetAll();
	}
	return login_sink_.OnCheckLogin(res_info);
	VMP_END;
}

std::string CmnCheckLoginOperC::GetMachineCode()
{
#ifdef _DEBUG
	return LocalInfo::GetTheMachineCode();
#else
	return GainLocalInfo::GetMe().GetMachineCode();
#endif
}

bool CmnCheckLoginOperC::GetLoginRes() const
{
	return CheckOptionalBoolRes(login_res_);
}

bool CmnCheckLoginOperC::CheckLogin(const CheckLoginInfoContT<std::string>& login_infos)
{
	VMP_BEGIN_EX;
	assert(!login_infos.value_.empty());
	check_login_res_.reset();
	return ToCall(BC_MakeP("ckl") << login_infos);
	VMP_END;
}

BC_REG_CLS_DEFINE(CmnCheckLoginGrantOperC){
	bufcall::class_<CmnCheckLoginGrantOperC, CmnCheckLoginOperC>("CmnCheckLoginGrantOperC")
	.def("OnGrant_RemoveBind", &CmnCheckLoginGrantOperC::OnGrant_RemoveBind)
	.def("OnGrant_OpenCard", &CmnCheckLoginGrantOperC::OnGrant_OpenCard)
	.def("OnGrant_GenRegKey", &CmnCheckLoginGrantOperC::OnGrant_GenRegKey)
	.def("OnGrant_PeerAcByCard", &CmnCheckLoginGrantOperC::OnGrant_PeerAcByCard)
	.def("OnGrant_PeerCardByAc", &CmnCheckLoginGrantOperC::OnGrant_PeerCardByAc)
	.def("OnGrant_PeerAcInfo", &CmnCheckLoginGrantOperC::OnGrant_PeerAcInfo)
	.def("OnGrant_PeerCardInfo", &CmnCheckLoginGrantOperC::OnGrant_PeerCardInfo)
	.def("OnGrant_RollbackPay", &CmnCheckLoginGrantOperC::OnGrant_RollbackPay)
	.def("OnGrant_SetAddrData", &CmnCheckLoginGrantOperC::OnGrant_SetAddrData)
	.def("OnGrant_SetStrData", &CmnCheckLoginGrantOperC::OnGrant_SetStrData)
	.def("OnGrant_SetAcState", &CmnCheckLoginGrantOperC::OnGrant_SetAcState);
}

CmnCheckLoginGrantOperC::CmnCheckLoginGrantOperC( ICmnLoginGrantSink& sink, FuncObj* parent ) : CmnCheckLoginOperC(sink, parent), login_sink_ex_(sink)
{
	grant_remove_bind_res_ = false;
	grant_open_card_res_ = false;
	grant_gen_reg_key_res_ = false;
	grant_peer_ac_by_card_res_ = false;
	grant_peer_card_by_ac_res_ = false;
	grant_peer_ac_info_res_ = false;
	grant_peer_card_info_res_ = false;
	grant_peer_rool_back_pay_res_ = false;
	grant_set_addr_data_res_ = false;
	grant_set_str_data_res_ = false;
	grant_set_ac_state_ = false;
}

bool CmnCheckLoginGrantOperC::Grant_RemoveBind( pt_csz user_name_dst )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidUserName(user_name_dst) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_RemoveBind")<<user_name_dst, grant_remove_bind_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_OpenCard(pt_word card_cnt, pt_dword card_minutes, pt_qword grant, pt_dword ac_type)
{
	VMP_BEGIN_EX;
	if ( card_cnt <= 0 || card_minutes <= 0 )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_OpenCard") << card_cnt << card_minutes << grant << ac_type, grant_open_card_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_GenRegKey(pt_word reg_key_cnt, pt_dword ac_type)
{
	VMP_BEGIN_EX;
	if ( reg_key_cnt <= 0 )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_GenRegKey")<<reg_key_cnt<<ac_type, grant_gen_reg_key_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_PeerAcByCard( pt_csz card_id, pt_csz card_pwd )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidCard(card_id) || !P_StringCheck::ValidCard(card_pwd) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_PeerAcByCard")<<card_id<<card_pwd, grant_peer_ac_by_card_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_PeerCardByAc( pt_csz ac, pt_csz ac_pwd )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidUserName(ac) || !P_StringCheck::ValidUserPwd(ac_pwd) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_PeerCardByAc")<<ac<<ac_pwd, grant_peer_card_by_ac_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_PeerAcInfo( pt_csz ac, pt_csz ac_pwd )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidUserName(ac) || !P_StringCheck::ValidUserPwd(ac_pwd) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_PeerAcInfo")<<ac<<ac_pwd, grant_peer_ac_info_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_PeerCardInfo( pt_csz card_id, pt_csz card_pwd )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidCard(card_id) || !P_StringCheck::ValidCard(card_pwd) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_PeerCardInfo")<<card_id<<card_pwd, grant_peer_card_info_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_RollbackPay( pt_csz card_id, pt_csz card_pwd, pt_bool direction )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidCard(card_id) || !P_StringCheck::ValidCard(card_pwd) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_RollbackPay")<<card_id<<card_pwd<<direction, grant_peer_rool_back_pay_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_SetAddrData( pt_int id, pt_dword addr_data )
{
	VMP_BEGIN_EX;
	return ToCallSync(BC_MakeP("Grant_SetAddrData")<<id<<addr_data, grant_set_addr_data_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_SetStrData( pt_int id, pt_csz str_data )
{
	VMP_BEGIN_EX;
	if ( !P_StringCheck::ValidStrData(str_data) )
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_SetStrData")<<id<<str_data, grant_set_str_data_res_);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_RemoveBind( bool res, pt_csz info )
{
	VMP_BEGIN_EX;
	if (grant_remove_bind_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872a");
	}
	grant_remove_bind_res_ = res;
	return login_sink_ex_.OnGrant_RemoveBind(res, info);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_OpenCard( bool res, pt_csz info, const CardInfoT& card_info )
{
	VMP_BEGIN_EX;
	if (grant_open_card_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872b");
	}
	grant_open_card_res_ = res;
	return login_sink_ex_.OnGrant_OpenCard(res, info, card_info);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_GenRegKey( bool res, pt_csz info, const RegKeyInfoT& reg_key )
{
	VMP_BEGIN_EX;
	if (grant_gen_reg_key_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872c");
	}
	grant_gen_reg_key_res_ = res;
	return login_sink_ex_.OnGrant_GenRegKey(res, info, reg_key);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_PeerAcByCard(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	VMP_BEGIN_EX;
	if (grant_peer_ac_by_card_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872d");
	}
	grant_peer_ac_by_card_res_ = res;
	return login_sink_ex_.OnGrant_PeerAcByCard(res, info, ac_info, ac_state);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_PeerCardByAc(bool res, pt_csz info, const stData_Card::CardContT& card_info)
{
	VMP_BEGIN_EX;
	if (grant_peer_card_by_ac_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872reqw");
	}
	grant_peer_card_by_ac_res_ = res;
	return login_sink_ex_.OnGrant_PeerCardByAc(res, info, card_info);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_PeerAcInfo(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	VMP_BEGIN_EX;
	if (grant_peer_ac_info_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872fdas");
	}
	grant_peer_ac_info_res_ = res;
	return login_sink_ex_.OnGrant_PeerAcInfo(res, info, ac_info, ac_state);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_PeerCardInfo(bool res, pt_csz info, const stData_Card::CardContT& card_info)
{
	VMP_BEGIN_EX;
	if (grant_peer_card_info_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872fdase");
	}
	grant_peer_card_info_res_ = res;
	return login_sink_ex_.OnGrant_PeerCardInfo(res, info, card_info);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_RollbackPay( bool res, pt_csz info )
{
	VMP_BEGIN_EX;
	if (grant_peer_rool_back_pay_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872ere");
	}
	grant_peer_rool_back_pay_res_ = res;
	return login_sink_ex_.OnGrant_RollbackPay(res, info);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_SetAddrData( bool res, pt_csz info )
{
	VMP_BEGIN_EX;
	if (grant_set_addr_data_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872fd");
	}
	grant_set_addr_data_res_ = res;
	return login_sink_ex_.OnGrant_SetAddrData(res, info);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_SetStrData( bool res, pt_csz info )
{
	VMP_BEGIN_EX;
	if (grant_set_str_data_res_)
	{
		assert(false);
		throw CmnExitIoThreadException("9872aa");
	}
	grant_set_str_data_res_ = res;
	return login_sink_ex_.OnGrant_SetStrData(res, info);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::OnGrant_SetAcState(bool res, pt_csz info)
{
	VMP_BEGIN_EX;
	if (grant_set_ac_state_)
	{
		assert(false);
		throw CmnExitIoThreadException("98723d");
	}
	grant_set_ac_state_ = res;
	return login_sink_ex_.OnGrant_SetAcState(res, info);
	VMP_END;
}

bool CmnCheckLoginGrantOperC::Grant_SetAcState(pt_csz user_name_dst, enAcState ac_state, Poco::UInt32 period)
{
	VMP_BEGIN_EX;
	if (!P_StringCheck::ValidUserName(user_name_dst))
	{
		assert(false);
		return false;
	}
	return ToCallSync(BC_MakeP("Grant_SetAcState") << user_name_dst << ac_state << period, grant_set_ac_state_);
	VMP_END;
}

bool ICmnLoginSink::OnLoginRes( bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, const TimeT& end_day_time, 
	const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas, pt_dword ac_type)
{
	return true;
}

bool ICmnLoginSink::OnRemoveBind( bool res, pt_csz info )
{
	return true;
}

bool ICmnLoginSink::OnRegUser( bool res, pt_csz info )
{
	return true;
}

bool ICmnLoginSink::OnUserPay( bool res, pt_csz info )
{
	return true;
}

bool ICmnLoginSink::OnLeaveMsg( bool res, pt_csz info )
{
	return true;
}

bool ICmnLoginSink::OnCheckLogin(const CheckLoginResContT& res_info)
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_RemoveBind( bool res, pt_csz info )
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_OpenCard( bool res, pt_csz info, const CardInfoT& card_info )
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_GenRegKey( bool res, pt_csz info, const RegKeyInfoT& reg_key )
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_PeerAcByCard(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_PeerCardByAc(bool res, pt_csz info, const stData_Card::CardContT& card_info)
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_PeerAcInfo(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state)
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_PeerCardInfo(bool res, pt_csz info, const stData_Card::CardContT& card_info)
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_RollbackPay( bool res, pt_csz info )
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_SetAddrData( bool res, pt_csz info )
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_SetStrData( bool res, pt_csz info )
{
	return true;
}

bool ICmnLoginGrantSink::OnGrant_SetAcState(bool res, pt_csz info)
{
	return true;
}

CmnLoginConnectCfg::CmnLoginConnectCfg(const std::string& server_ip) :
	Connection_Rsa(server_ip, GetCfgInfo().port_, GetCfgInfo().rsa_.public_key_, GetCfgInfo().rsa_.private_key_)
{

}

bool CmnLoginConnectCfg::SetCfgInfo(const std::vector<std::string>& ip_info, pt_word port)
{
	if (ip_info.empty())
	{
		assert(false);
		return false;
	}
	if (!connect_cfg_info_.ip_info_.empty())
	{
		assert(false);
		return false;
	}
	if (!connect_cfg_info_.rsa_.Init())
	{
		assert(false);
		return false;
	}
	connect_cfg_info_.ip_info_ = ip_info;
	connect_cfg_info_.port_ = port;
	return true;
}

const CmnLoginConnectCfg::stConnectCfgInfo& CmnLoginConnectCfg::GetCfgInfo()
{
	assert(connect_cfg_info_.port_ && connect_cfg_info_.rsa_.public_key_);
	return connect_cfg_info_;
}

bool CmnLoginConnectCfg::SetCfgInfo(const luabind::object& tbl_ip_info, pt_word port)
{
	std::vector<std::string> ip_info;
	luabind::iterator it(tbl_ip_info), end;
	for (; it != end; ++it)
	{
		const auto& str = LuaObjCast(*it, std::string());
		if (str.empty())
		{
			assert(false);
			return false;
		}
		ip_info.push_back(str);
	}
	if (ip_info.empty())
	{
		assert(false);
		return false;
	}
	return SetCfgInfo(ip_info, port);
}

CmnLoginConnectCfg::stConnectCfgInfo CmnLoginConnectCfg::connect_cfg_info_;

CmnLoginConnectCfg::stConnectCfgInfo::stConnectCfgInfo()
{
	port_ = 0;
}

CmnDoLogin::CmnDoLogin(const std::string& check_server_ip) : CmnCheckLoginC(check_server_ip)
{
	is_doing_ = false;
}

luabind::object CmnDoLogin::Login(const luabind::object& is_only_login, pt_csz user_name, pt_csz user_pwd)
{
	if (is_doing_)
	{
		assert(false);
		return luabind::object();
	}
	if (!is_only_login.is_valid())
	{
		assert(false);
		return luabind::object();
	}
	is_doing_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_doing_ = false;
	};
	try{
		if (!GetUserOper().Login(LuaObjCast(is_only_login, true), user_name, user_pwd, GetVersionInfo().version_))
		{
			assert(false);
			return luabind::object();
		}
	}
	catch (const OperExceptionT& e)
	{
		auto res = luabind::newtable(is_only_login.interpreter());
		res[1] = e.res_;
		res[2] = e.what();
		return res;
	}
	return luabind::object();
}

luabind::object CmnDoLogin::RemoveBind(const luabind::object& user_name, pt_csz user_pwd)
{
	if (is_doing_)
	{
		assert(false);
		return luabind::object();
	}
	if (!user_name.is_valid())
	{
		assert(false);
		return luabind::object();
	}
	is_doing_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_doing_ = false;
	};
	try{
		if (!GetUserOper().RemoveBind(LuaObjCast(user_name, ""), user_pwd))
		{
			assert(false);
			return luabind::object();
		}
	}
	catch (const OperExceptionT& e)
	{
		auto res = luabind::newtable(user_name.interpreter());
		res[1] = e.res_;
		res[2] = e.what();
		return res;
	}
	return luabind::object();
}

luabind::object CmnDoLogin::RegUser(const luabind::object& user_name, pt_csz user_pwd, pt_csz str_qq, pt_csz reg_key_id)
{
	if (is_doing_)
	{
		assert(false);
		return luabind::object();
	}
	if (!user_name.is_valid())
	{
		assert(false);
		return luabind::object();
	}
	is_doing_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_doing_ = false;
	};
	try{
		if (!GetUserOper().RegUser(LuaObjCast(user_name, ""), user_pwd, str_qq, reg_key_id))
		{
			assert(false);
			return luabind::object();
		}
	}
	catch (const OperExceptionT& e)
	{
		auto res = luabind::newtable(user_name.interpreter());
		res[1] = e.res_;
		res[2] = e.what();
		return res;
	}
	return luabind::object();
}

luabind::object CmnDoLogin::UserPay(const luabind::object& user_name, pt_csz card_id, pt_csz card_pwd)
{
	if (is_doing_)
	{
		assert(false);
		return luabind::object();
	}
	if (!user_name.is_valid())
	{
		assert(false);
		return luabind::object();
	}
	is_doing_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_doing_ = false;
	};
	try{
		if (!GetUserOper().UserPay(LuaObjCast(user_name, ""), card_id, card_pwd))
		{
			assert(false);
			return luabind::object();
		}
	}
	catch (const OperExceptionT& e)
	{
		auto res = luabind::newtable(user_name.interpreter());
		res[1] = e.res_;
		res[2] = e.what();
		return res;
	}
	return luabind::object();
}

luabind::object CmnDoLogin::LeaveMsg(const luabind::object& msg)
{
	if (is_doing_)
	{
		assert(false);
		return luabind::object();
	}
	if (!msg.is_valid())
	{
		assert(false);
		return luabind::object();
	}
	is_doing_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		is_doing_ = false;
	};
	try{
		if (!GetUserOper().LeaveMsg(LuaObjCast(msg, "")))
		{
			assert(false);
			return luabind::object();
		}
	}
	catch (const OperExceptionT& e)
	{
		auto res = luabind::newtable(msg.interpreter());
		res[1] = e.res_;
		res[2] = e.what();
		return res;
	}
	return luabind::object();
}

static void __CheckLocalMachinePort__()
{
#ifdef _DEBUG
	return;
#endif
	if (!VMP_CRC_VALID() || VMP_IS_DEBUGGER())
	{
		SetIoTimer1(10 * 1000, [](IoTimer& timer){
			VMP_BEGIN_EX;
			const char* const kThrowStr =
#ifdef _DEBUG
				"is on debug or crc1"
#else
				"iodoc1"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
			VMP_END;
		});
	}
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(CmnLoginConnectCfg::GetCfgInfo().port_);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	int nRet = bind(listenSock, (sockaddr*)&sin, (int)(sizeof(sin)));
	if (nRet == SOCKET_ERROR)
	{
		auto errCode = WSAGetLastError();
		if (WSAEADDRINUSE == errCode)
		{
			assert(false);
			SetIoTimer1(13 * 1000, [](IoTimer& timer){
				VMP_BEGIN_EX;
				const char* const kThrowStr =
#ifdef _DEBUG
					"CheckLocalMachinePort error"
#else
					"clmpe"
#endif
					;
				throw CmnExitIoThreadException(kThrowStr);
				VMP_END;
			});
		}
		return;
	}

	closesocket(listenSock);
}

bool CmnDoLogin::OnLoginRes(bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie,
	const TimeT& end_day_time, const TimeT& ser_cur_time, const AddrDatasT& addr_datas, const StrDatasT& str_addr_datas, pt_dword ac_type)
{
	p_supper::OnLoginRes(res, info, ac_pwd_machine_mix, new_cookie, end_day_time, ser_cur_time, addr_datas, str_addr_datas, ac_type);

	__CheckLocalMachinePort__();
	
	//前面已经有处理了
	/*if (res)
	{
		auto login_info_mgr = CmnLoginInfoMgrBaseImpl::GetDerived<CmnLoginInfoMgrBase>();
		if (!login_info_mgr)
		{
			assert(false);
			return false;
		}
		login_info_mgr->RectifyAcceptMaxCnt();
	}*/
	
	if (is_doing_)
		throw OperExceptionT(res, info);
	return true;
}

bool CmnDoLogin::OnRemoveBind(bool res, pt_csz info)
{
	if (is_doing_)
		throw OperExceptionT(res, info);
	return true;
}

bool CmnDoLogin::OnRegUser(bool res, pt_csz info)
{
	if (is_doing_)
		throw OperExceptionT(res, info);
	return true;
}

bool CmnDoLogin::OnUserPay(bool res, pt_csz info)
{
	if (is_doing_)
		throw OperExceptionT(res, info);
	return true;
}

bool CmnDoLogin::OnLeaveMsg(bool res, pt_csz info)
{
	if (is_doing_)
		throw OperExceptionT(res, info);
	return true;
}

luabind::object CmnDoLogin::__Login__(const luabind::object& is_only_login, pt_csz user_name, pt_csz user_pwd)
{
	auto login_info_mgr = CmnLoginInfoMgrBaseImpl::GetDerived<CmnLoginInfoMgrBase>();
	if (!login_info_mgr)
	{
		assert(false);
		return luabind::object();
	}
	auto check_login = CmnDoLogin::GetSelfPtr();
	if (!check_login)
		return luabind::object();
	
	login_info_mgr->ClearAllLoginInfos();
	login_info_mgr->AddLoginInfo(user_name, user_pwd);
	return check_login->Login(is_only_login, user_name, user_pwd);
}

luabind::object CmnDoLogin::__RemoveBind__(const luabind::object& user_name, pt_csz user_pwd)
{
	auto check_login = CmnDoLogin::GetSelfPtr();
	if (!check_login)
		return luabind::object();
	return check_login->RemoveBind(user_name, user_pwd);
}

luabind::object CmnDoLogin::__RegUser__(const luabind::object& user_name, pt_csz user_pwd, pt_csz str_qq, pt_csz reg_key_id)
{
	auto check_login = CmnDoLogin::GetSelfPtr();
	if (!check_login)
		return luabind::object();
	return check_login->RegUser(user_name, user_pwd, str_qq, reg_key_id);
}

luabind::object CmnDoLogin::__UserPay__(const luabind::object& user_name, pt_csz card_id, pt_csz card_pwd)
{
	auto check_login = CmnDoLogin::GetSelfPtr();
	if (!check_login)
		return luabind::object();
	return check_login->UserPay(user_name, card_id, card_pwd);
}

static bool __sci__(const luabind::object& tbl_ip_info, pt_word port)
{
	return CmnDoLogin::SetCfgInfo(tbl_ip_info, port);
}
void CmnDoLogin::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l, "cdl")
	[
		def("l", &CmnDoLogin::__Login__),
		def("rb", &CmnDoLogin::__RemoveBind__),
		def("ru", &CmnDoLogin::__RegUser__),
		def("up", &CmnDoLogin::__UserPay__),
		def("sci", &__sci__),
		def("svi", &CmnDoLogin::SetVersionInfo)
	];
}

void CmnDoLogin::SetVersionInfo(const std::string& str_version, pt_int version)
{
	if (g_login_version_info_.version_ != 0)
	{
		assert(false);
		return;
	}
	assert(version != 0);
	g_login_version_info_.version_ = version;
	g_login_version_info_.str_version_ = str_version;
}

const CmnDoLogin::stVersionInfo& CmnDoLogin::GetVersionInfo()
{
	assert(g_login_version_info_.version_ != 0);
	return g_login_version_info_;
}

CmnDoLogin::stVersionInfo CmnDoLogin::g_login_version_info_;

CmnLoginException::CmnLoginException(bool res, const std::string& msg) : CmnExceptionBase(msg)
{
	res_ = res;
}

CmnDoLogin::stVersionInfo::stVersionInfo()
{
	version_ = 0;
}

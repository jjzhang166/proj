#include "stdafx.h"
#include "CheckLoginC.h"
#include "GLuaS.h"
#include "ListCtrls.h"
#include "DlgUnion.h"
#include "Console.h"
#include <protect/Protect.h>
#include "TabOption.h"
#include "MainFrm.h"
#include "../CheckServer/CS_msg/OperRetMsg.h"
#include "../CheckServer/CS_msg/CS_Defines.h"
#include "AntiCrack.h"
#include "ServerIpInfo.h"

CheckLoginC::CheckLoginC( const std::string& check_server_ip, const CryptoStrCompose::SubCryptoT& stm_pub, 
						 const CryptoStrCompose::SubCryptoT& stm_pri ) : 
	Con_Aes_Rsa(check_server_ip, P_Port_CheckServer,stm_pub, stm_pri), check_server_oper_(*this)
{
}

CheckLoginOperC& CheckLoginC::GetUserOper()
{
	return check_server_oper_;
}

CheckLoginC::SelfPtr CheckLoginC::GetSelfPtr()
{
	return theApp.GetCheckLogin();
}

void CheckLoginC::OnConnectFailed()
{
	__super::OnConnectFailed();
	theApp.SetCheckSvrConnected(false);
}

bool CheckLoginC::AfterLoginSucced( const std::function<void( SelfPtr& conne_self )>& func )
{
	auto dlg_login = DlgLogin::GetInstance();
	if ( !dlg_login )
	{
		assert(false);
		return false;
	}
	auto conne = CheckLoginC::GetSelfPtr();
	if ( !conne )
		return false;
	if ( conne->login_res_ && *conne->login_res_ )
	{
		func(conne);
		return true;
	}
	
	auto& user_oper = conne->GetUserOper();
	std::string user_ac = CT2CA(dlg_login->GetAc());
	std::string user_pwd = CT2CA(dlg_login->GetPwd());
	user_oper.Login(user_ac, user_pwd);

	if ( conne->login_res_ && *conne->login_res_ )
	{
		func(conne);
		return true;
	}
	return false;
}

bool CheckLoginC::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	VMP_BEGIN_EX;
	auto dlg_check_server = DlgCheckServerInfo::GetInstance();
	if (dlg_check_server)
	{
		dlg_check_server->ConnectSucceed();
	}

	theApp.SetCheckSvrConnected(true);
	VMP_END;
	return true;
}

void CheckLoginC::OnDisconnect( IoConnectBase* connect )
{
	/*GetIoService().post([](){
		auto list_ctrl = PageAccount_List::GetInstance();
		if (list_ctrl)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_info) << "验证服务器关闭了连接，关闭所有游戏进程";
			list_ctrl->OnCloseAllProcess();
		}
	});*/
	
	__super::OnDisconnect(connect);

	auto dlg_check_server = DlgCheckServerInfo::GetInstance();
	if (dlg_check_server)
	{
		dlg_check_server->SetConnecting(false);
	}

	theApp.SetCheckSvrConnected(false);
}

CheckLoginOperC::CheckLoginOperC( CheckLoginC& check_login ) : JsonEchoVmp(GLuaVM_S::GetMe(), check_login, kObjId_UserOper), check_login_(check_login)
{

}

void CheckLoginOperC::Login( const std::string& user_name, const std::string& user_pwd )
{
	VMP_BEGIN_EX;
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
	{
		assert(false);
		return;
	}
	CheckLocalMachinePort();

	bool res = false;
	ThisMemCallSync(nullptr, "Login", &JsonEcho::MakeParams(user_name, user_pwd, GetMachineCode(), kProjectId), JsonEcho::MakeResult(res, res));

	/*//等待前先重置一下
	check_login_.login_res_.reset();

	WaitUntil([this]()->bool{		
		return this->check_login_.login_res_;
	}, 10 * 1000);*/
	
	auto dlg_login = DlgLogin::GetInstance();
	if (dlg_login)
		dlg_login->SetDoing(false);

	/*if (check_login_.login_res_)
	{
		if (!*check_login_.login_res_)
		{
			check_login_.login_res_.reset();
		}
		else
		{
			GetAddrDatas();
		}
	}*/
	if (res)
	{
		GetIoService().post([=](){
			VMP_BEGIN_EX;
			tab_option->SetLoginSucceed(res);
			auto dlg_login = DlgLogin::GetInstance();
			if (dlg_login)
			{
				dlg_login->OnLoginSucceed();
			}
			auto main_wnd = CMainFrame::GetInstance();
			if (main_wnd)
			{
				main_wnd->BeginCheckAcValid();
			}
			VMP_END;
		});
		GetAddrDatas();
		auto tab_option = CTabOption::GetInstance();
		if ( !tab_option )
			return;
		tab_option->TimerCheckLogin();

		//检测目标IP
		check_login_.login_res_ = true;
	}
	VMP_END;
}

bool CheckLoginOperC::OnThisMemCall( const stThisCallHelper& helper )
{
	VMP_BEGIN;
	return OnThisFuncCallImpl(this, helper);
	VMP_END;
}

void CheckLoginOperC::RemoveBind( const std::string& user_name, const std::string& user_pwd, const std::string& machine_code )
{
	VMP_BEGIN_EX;
	if ( !ThisMemCall(nullptr, "RemoveBind", &JsonEcho::MakeParams(user_name, user_pwd, machine_code)) )
		return;
	//等待前先重置一下
	check_login_.remove_bind_.reset();

	WaitUntil([this](){
		return this->check_login_.remove_bind_;
	}, 10 * 1000);

	check_login_.remove_bind_.reset();

	auto dlg_login = DlgLogin::GetInstance();
	if (dlg_login)
		dlg_login->SetDoing(false);
	VMP_END;
}

void CheckLoginOperC::OnRemoveBind( bool succeed )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "解绑返回结果->" << succeed;
	check_login_.remove_bind_ = succeed;

	if (succeed)
	{
		//2秒后结束
		SetIoTimer(2 * 1000, []( IoTimer* ){
			auto tab_option = CTabOption::GetInstance();
			if (tab_option)
				tab_option->ClearLoginInfo();
		});
	}
}

void CheckLoginOperC::OnRegUser( bool succeed )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "注册返回结果->" << succeed;
	check_login_.reg_user_ = succeed;

	if (succeed)
	{

	}
}

void CheckLoginOperC::OnUserPay( bool succeed )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "充值返回结果->" << succeed;
	check_login_.user_pay_ = succeed;

	if (succeed)
	{

	}
}

void CheckLoginOperC::RegUser( const std::string& user_name, const std::string& user_pwd, const std::string& str_qq, const std::string& reg_key_id )
{
	if ( !ThisMemCall(nullptr, "RegUser", &JsonEcho::MakeParams(user_name, user_pwd, str_qq, kProjectId, reg_key_id)) )
		return;
	//等待前先重置一下
	check_login_.reg_user_.reset();

	WaitUntil([this](){
		return this->check_login_.reg_user_;
	}, 10 * 1000);

	check_login_.reg_user_.reset();

	auto dlg_reg = DlgReg::GetInstance();
	if (dlg_reg)
		dlg_reg->SetDoing(false);
}

void CheckLoginOperC::Pay( const std::string& user_name, const std::string& card_id, const std::string& card_pwd )
{
	if ( !ThisMemCall(nullptr, "UserPay", &JsonEcho::MakeParams(user_name, card_id, card_pwd)) )
		return;
	check_login_.user_pay_.reset();

	WaitUntil([this](){
		return this->check_login_.user_pay_;
	}, 10 * 1000);

	check_login_.user_pay_.reset();

	auto dlg_pay = DlgPay::GetInstance();
	if (dlg_pay)
		dlg_pay->SetDoing(false);
}

std::string CheckLoginOperC::GetMachineCode()
{
	char buffer[kMachineCodeLen + 1];
#ifndef _DEBUG
	if ( !GetMixHWID(buffer, kMachineCodeLen) )
#endif
		VMP_GET_CUR_HWID(buffer, sizeof(buffer));
	//return std::string(buffer, kMachineCodeLen);
	return GenUuidStr(buffer, kMachineCodeLen);
}

void CheckLoginOperC::OnLoginInfo( const std::string& info )
{
	VMP_BEGIN_EX;
	auto dlg_login = DlgLogin::GetInstance();
	if (dlg_login)
	{
		dlg_login->SetWndText(info.c_str());
	}
	VMP_END;
}

void CheckLoginOperC::OnRemoveBindInfo( const std::string& info )
{
	auto dlg_login = DlgLogin::GetInstance();
	if (dlg_login)
	{
		dlg_login->SetWndText(info.c_str());
	}
}

void CheckLoginOperC::OnRegUserInfo( const std::string& info )
{
	auto dlg_reg = DlgReg::GetInstance();
	if (dlg_reg)
	{
		dlg_reg->SetWndText(info.c_str());
	}
}

void CheckLoginOperC::OnUserPayInfo( const std::string& info )
{
	auto dlg_pay = DlgPay::GetInstance();
	if (dlg_pay)
	{
		dlg_pay->SetWndText(info.c_str());
	}
}

void CheckLoginOperC::OnUserEndTime( Poco::UInt32 timestamp_lower, Poco::UInt32 timestamp_upper )
{
	VMP_BEGIN_EX;
	Poco::Timestamp poco_timestamp = Poco::Timestamp::fromEpochTime((time_t)MakeInt64(timestamp_lower, timestamp_upper));
	auto tab_option = CTabOption::GetInstance();
	if (tab_option)
	{
		tab_option->SetUserEndTime(poco_timestamp);
	}
	VMP_END;
}

void CheckLoginOperC::OnRecvAddrDatas( const std::string& str_key, const Json::Value& addr_datas )
{
	if ( addr_datas.isNull() || !addr_datas.isArray() )
		return;
	if ( addr_datas.size() <= 0 )
		return;
	VMP_BEGIN_EX;
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return;
	const auto& addr_values = tab_option->GetAddrDatas();
	assert(0 == addr_values.size());
	Secret_XorKey algo_xor_key;
	algo_xor_key.SetKey(str_key.c_str(), str_key.size());
	char cstr_tmp[50];
	std::string str_addr, str_original;
	for ( const auto& addr_data : addr_datas )
	{
		if ( addr_data.isNull() || !addr_data.isString() )
			return;
		str_addr = addr_data.asString();
		if ( str_addr.size() <= 0 )
			return;
		algo_xor_key.Decrypt(str_addr.c_str(), str_addr.size(), cstr_tmp);
		str_original.assign(cstr_tmp, str_addr.size());
		
		tab_option->AddAddrData(std::stoi(str_original));
	}

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "接收关键数据成功,size->" << addr_datas.size();
	VMP_END;
}

void CheckLoginOperC::GetAddrDatas()
{
	VMP_BEGIN;
	/*if ( !check_login_.login_res_ )
		return;
	if ( !*check_login_.login_res_ )
		return;*/
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
		return;
	const auto& addr_datas = tab_option->GetAddrDatas();
	if ( addr_datas.size() > 0 )
		return;
	ThisMemCall(nullptr, "GetAddrDatas", nullptr);
	VMP_END;
}

void CheckLoginOperC::OnLoginAcInfo( const std::string& ac_pwd )
{
	VMP_BEGIN_EX;
	auto main_frm = CMainFrame::GetInstance();
	if ( !main_frm )
		return;
	main_frm->SetValidAcInfo(ac_pwd);
	VMP_END;
}

void CheckLoginOperC::CheckLogin( const std::string& user_name, const std::string& user_pwd )
{
	VMP_BEGIN_EX;
	auto failed_cnt = GetLPD_CheckLoginFailedCnt();
	assert(failed_cnt >= 0);
	SetLPD_CheckLoginFailedCnt(++failed_cnt);
	auto tab_option = CTabOption::GetInstance();
	if ( !tab_option )
	{
		assert(false);
		return;
	}
	if ( !tab_option->IsLoginSucceed() )
		return;
	auto check_login = CheckLoginC::GetSelfPtr();
	if (check_login)
	{
		const auto& kUuidMachineCode = GenUuidStr(GetMachineCode());
		check_login->check_server_oper_.ThisMemCall(nullptr, "clm", &JsonEcho::MakeParams(user_name, user_pwd, kUuidMachineCode, kProjectId));
	}
	VMP_END;
}

std::string CheckLoginOperC::LeaveMsg( const std::string& msg )
{
	std::string out_info("错误");
	ThisMemCallSync(nullptr, "LeaveMsg", &JsonEcho::MakeParams(msg), JsonEcho::MakeResult(out_info, out_info));
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "leave msg result->" << out_info;
	return out_info;
}

void CheckLoginOperC::OnCheckLoginResult( int res )
{
	VMP_BEGIN_EX;
	if ( kCSR_CL_failed == res )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "验证登陆失败";
		auto tab_option = CTabOption::GetInstance();
		if (tab_option)
			tab_option->ClearLoginInfo();
		SetIoTimer(5 * 1000, []( IoTimer* ){
			const char* const kThrowStr = 
#ifdef _DEBUG
				"check login failed"
#else
				"clf"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
		});
	}
	else if ( kCSR_CL_Succeed != res )
	{
		auto failed_cnt = GetLPD_CheckLoginFailedCnt();
		if ( failed_cnt >= 20 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "验证登陆无效次数已达上限，无效次数->" << failed_cnt;
			assert(false);
			const char* const kThrowStr = 
#ifdef _DEBUG
				"check login failed cnt limit 1"
#else
				"clfcl1"
#endif
				;
			throw CmnExitIoThreadException(kThrowStr);
		}
		++failed_cnt;
		SetLPD_CheckLoginFailedCnt(failed_cnt);
	}
	else
	{
		auto failed_cnt = GetLPD_CheckLoginFailedCnt();
		assert(failed_cnt > 0);
		SetLPD_CheckLoginFailedCnt(--failed_cnt);
	}
	VMP_END;
}

#include "stdafx.h"
#include "AC_Conne.h"
#include "LuaAc.h"
#include "../CheckServer/CS_msg/CS_Defines.h"
#include <protect/Protect.h>
#include "../CheckServer/CS_msg/OperRetMsg.h"
#include "Process.h"

AC_Connect::AC_Connect( const std::string& check_server_ip, const CryptoStrCompose::SubCryptoT& stm_pub, const CryptoStrCompose::SubCryptoT& stm_pri )
	: ac_oper_(*this), Con_Aes_Rsa(check_server_ip, P_Port_CheckServer, stm_pub, stm_pri)
{

}

AC_OperC& AC_Connect::GetOper()
{
	return ac_oper_;
}

AC_Connect::SelfPtr AC_Connect::GetSelfPtr()
{
	static boost::weak_ptr<AC_Connect> weak_ac_connec;
	if (weak_ac_connec.expired())
	{
		VMP_BEGIN_EX;
		const char* const kSvrIp =
			kRemoteCheckServerIp
			//kSelfIpStr
			//"192.168.1.10"
			//"121.235.105.223"
			;
		assert(public_key_ && private_key_);
		{
			auto share_this = AC_Connect::SelfPtr(new AC_Connect(kSvrIp, public_key_, private_key_));
			assert(share_this);
			if ( !share_this )
				return SelfPtr();
			share_this->ConnectAsync();
			check_svr_connect_succeed_.reset();
			weak_ac_connec = share_this;
		}

		WaitUntil([](){
			return check_svr_connect_succeed_;
		}, 20 * 1000);
		if ( !check_svr_connect_succeed_ || !*check_svr_connect_succeed_ )
			return boost::shared_ptr<AC_Connect>();
		if (weak_ac_connec.expired())
			return boost::shared_ptr<AC_Connect>();
		else
			return weak_ac_connec.lock();
		VMP_END;
	}
	else
	{
		return weak_ac_connec.lock();
	}
}

bool AC_Connect::InitConne()
{
	//初始化RSA密钥
	if ( !GenRsaKey(GetModuleHandle(nullptr), public_key_, private_key_, VMP_PROTECT_STR(kRSA_PriPassphrase_CtoS)) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "rsa key init failed";
		return false;
	}
	return true;
}

void AC_Connect::ClrConne()
{
	if (public_key_)
		public_key_.reset();
	if (private_key_)
		private_key_.reset();
}

void AC_Connect::OnConnectFailed()
{
	__super::OnConnectFailed();
	check_svr_connect_succeed_ = false;
}

bool AC_Connect::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;

	check_svr_connect_succeed_ = true;
	return true;
}

CryptoStrCompose::SubCryptoT AC_Connect::private_key_;

CryptoStrCompose::SubCryptoT AC_Connect::public_key_;

boost::optional<bool> AC_Connect::check_svr_connect_succeed_;

AC_OperC::AC_OperC( AC_Connect& ac_conne ) : ac_connect_(ac_conne), JsonEchoBase(Lua_Ac::GetMe(), ac_conne, kObjId_UserOper)
{

}

bool AC_OperC::OnThisMemCall( const stThisCallHelper& helper )
{
	return __super::OnThisFuncCallImpl(this, helper);
}

void AC_OperC::CheckLogin( const std::string& user_name, const std::string& user_pwd )
{
	VMP_BEGIN_EX;

	LOG_ENTER_FUNC;
	enCSRet_CheckLogin res = kCSR_CL_invalid;
	ThisMemCallSync(nullptr, "cl", &JsonEcho::MakeParams(user_name, user_pwd, kProjectId), JsonEcho::MakeResult(res, res));
	if ( kCSR_CL_failed == res )
	{
		LOG_O(Log_warning) << "验证登陆失败";
		CProcess theProcess;
		HANDLE hProcess = (HANDLE)theProcess.GetProcessHandle(_T("Console.exe"));

		LOG_O(Log_trace) << "process handle:" << hProcess;
		if( hProcess != NULL )
		{
			TerminateProcess(hProcess,0);
		}
	}
	else
	{
		LOG_O(Log_trace) << "验证通过";
	}
	LOG_O(Log_trace) << "关闭套接字";
	ac_connect_.Disconnect();
	VMP_END;
}

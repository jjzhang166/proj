#include "stdafx.h"
#include "LoginS.h"
#include "GLuaS.h"
#include "Protect/Protect.h"
#include "TabOption.h"
#include "Socket/IoTimer.h"
#include <Communicate/CL_Defines.h>
#include <Common/TemplateUtility.h>

LoginServer::LoginServer() : Server(P_Port_Login)
{

}

Session* LoginServer::CreateSession()
{
	return new LoginS(*this);
}

bool LoginServer::OnConnected( IoConnectBase* connect )
{
	VMP_BEGIN;
	auto tab_option = CTabOption::GetInstance();
	if ( tab_option && tab_option->IsLoginSucceed() )
		return __super::OnConnected(connect);
	VMP_END;
	return false;
}

void LoginServer::OnDisconnect( IoConnectBase* connect )
{
	__super::OnDisconnect(connect);

	auto ptr = PointerCast<Session>(connect);
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
		return;
	auto login_hlp = list_ctrl->FindLoginHelperBySession(ptr);
	if (login_hlp)
	{
		list_ctrl->SetItemText(login_hlp->item_, kSubitemRunState, _T("等待启动游戏"));
	}
	list_ctrl->EraseLoginHelperBySession(ptr);
	return;
}

LoginS::LoginS( Server& server ) : Session_Aes(server), json_echo_(GLuaVM_S::GetMe(), *this), login_oper_(*this)
{

}

JsonEcho& LoginS::GetJsonEcho()
{
	return json_echo_;
}

bool LoginS::OnExec( P_MsgNT msg_num, void* param, size_t param_size )
{
	switch (msg_num)
	{
	case kLoginMsg_BindLoginPid:
		{
			if ( sizeof(P_Exec_BindLoginPid) != param_size )
				break;
			return OnBindLoginPid(*(P_Exec_BindLoginPid*)param);
		}
	default:
		break;
	}
	return __super::OnExec(msg_num, param, param_size);
}

bool LoginS::OnBindLoginPid( P_Exec_BindLoginPid& msg )
{
	VMP_BEGIN_EX;
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
		return false;
	auto login_helper = list_ctrl->FindLoginHelperByPid(msg.login_pid_);
	if ( !login_helper )
		return false;
	//assert(!login_helper->session_login_);
	login_helper->session_login_ = boost::static_pointer_cast<Session>(shared_from_this());

	JsonEcho::ObjGetter getter("LoginC_Instance", nullptr);
	std::string account = CT2CA(list_ctrl->GetItemAccount(login_helper->item_));
	std::string password = CT2CA(list_ctrl->GetItemText(login_helper->item_, kSubitemPassword));
	json_echo_.MemFuncCall(getter, "SetAccount_Password", &JsonEcho::MakeParams(account, password));

	{
		POINT pt;
		const auto& ser_name = list_ctrl->GetItemServiceName(login_helper->item_);
		std::string kServiceName = CT2CA(ser_name);
		//server region
		pt.x = GLuaVM_S::GetMe().Call<long>("Login_GetSerRegionPosX", 0, kServiceName);
		pt.y = GLuaVM_S::GetMe().Call<long>("Login_GetSerRegionPosY", 0, kServiceName);
		json_echo_.MemFuncCall(getter, "SetSerRegion", &JsonEcho::MakeParams(pt.x, pt.y));
		//server
		pt.x = GLuaVM_S::GetMe().Call<long>("Login_GetServicePosX", 0, kServiceName);
		pt.y = GLuaVM_S::GetMe().Call<long>("Login_GetServicePosY", 0, kServiceName);
		json_echo_.MemFuncCall(getter, "SetJoinedService", &JsonEcho::MakeParams(pt.x, pt.y));
	}

	const auto& singleton_cfg = stSingletonCfg::GetMe();
	//答码方式
	json_echo_.MemFuncCall(getter, "SetCrackCaptchaMode", &JsonEcho::MakeParams((int)singleton_cfg.crack_captcha_mode_.ccm_,
		singleton_cfg.crack_captcha_mode_.account_, singleton_cfg.crack_captcha_mode_.password_,
		singleton_cfg.crack_captcha_mode_.remote_ip_, singleton_cfg.crack_captcha_mode_.remote_port_));
	//开始登陆
	json_echo_.MemFuncCall(getter, "BeginLogin", nullptr);

	//设置正在登陆
	list_ctrl->SetItemText(login_helper->item_, kSubitemRunState, _T("正在登陆"));

	login_oper_.ThisMemCall(nullptr, "cls", &JsonEcho::MakeParams(GenUuidStr(kStableStr_ConsoleToLogin)));
	VMP_END;
	return true;
}

bool LoginS::OnConnected( IoConnectBase* connect )
{
	if ( !__super::OnConnected(connect) )
		return false;
	auto tab_option = CTabOption::GetInstance();
	return tab_option && tab_option->IsLoginSucceed();
}

LoginOperS::LoginOperS( LoginS& login_s ) : login_s_(login_s), JsonEchoBase(GLuaVM_S::GetMe(), login_s, kObjId_LoginOper)
{

}

bool LoginOperS::OnThisMemCall( const stThisCallHelper& helper )
{
	return __super::OnThisFuncCallImpl(this, helper);
}

bool LoginOperS::AcOrPwdErr()
{
	auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
		return false;
	auto login_hlp = list_ctrl->FindLoginHelperBySession(&login_s_);
	if ( !login_hlp )
	{
		assert(false);
		return false;
	}

	auto login_item = login_hlp->item_;
	list_ctrl->EraseLoginHelper(login_hlp);
	login_hlp = nullptr;
	list_ctrl->BeginLogin(login_item);
	return true;
}

bool LoginOperS::AcFreeze()
{
	/*auto list_ctrl = PageAccount_List::GetInstance();
	if ( !list_ctrl )
		return false;
	auto login_hlp = list_ctrl->FindLoginHelperBySession(&login_s_);
	if ( !login_hlp )
	{
		assert(false);
		return false;
	}
	list_ctrl->SetItemText(login_hlp->item_, kSubitemRunState, _T("帐号冻结"));
	list_ctrl->SetCheck(login_hlp->item_, FALSE);

	std::string ac = CT2CA(list_ctrl->GetItemAccount(login_hlp->item_));
	std::string ser_name = CT2CA(list_ctrl->GetItemServiceName(login_hlp->item_));
	list_ctrl->AddProhibitItem(ac, ser_name, stSpecificCfg::kPR_AcFreeze);

	list_ctrl->EraseLoginHelper(login_hlp);
	login_hlp = nullptr;
	list_ctrl->DoLoginOne();*/
	assert(false);
	return true;
}

void LoginOperS::OnCheckLoginVersion( bool res )
{
	VMP_BEGIN_EX;
	if ( !res )
	{
		auto shared_this = login_s_.shared_from_this();
		SetIoTimer(1 * 1000, [shared_this]( IoTimer* ){
			assert(shared_this);
			shared_this->DoDisconnect();
		});
		SetIoTimer(1 * 1000, []( IoTimer* ){
			VMP_BEGIN_EX;
			const auto* const kThrowStr = 
#ifdef _DEBUG
				"check login str error"
#else
				"clse"
#endif
				;
			assert(false);
			throw CmnExitIoThreadException(kThrowStr);
			VMP_END;
		});
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "cls result->" << res;
	VMP_END;
}
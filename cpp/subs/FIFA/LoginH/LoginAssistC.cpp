#include "stdafx.h"
#include "LoginAssistC.h"
#include "LoginH_Lua.h"

bool LoginAssistOperC::OnThisMemCall( const stThisCallHelper& helper )
{
	return OnThisFuncCallImpl(this, helper);
}

LoginAssistOperC::LoginAssistOperC( LoginAssistC& login_assist ) : JsonEchoBase(*LoginH_Lua::GetInstance(), login_assist), login_assist_(login_assist)
{

}

void LoginAssistOperC::Test()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "test";
}

/*
LoginAssistC::LoginAssistC() : Connection_Aes(kSelfIpStr, P_Port_LoginAssist), oper_(*this)
{

}*/

LoginAssistOperC& LoginAssistC::GetLoginAssistOper()
{
	return oper_;
}

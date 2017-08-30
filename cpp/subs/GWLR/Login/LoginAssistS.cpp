#include "stdafx.h"
#include "LoginAssistS.h"
#include "Lua_l.h"

Session* LoginAssistServer::CreateSession()
{
	return new LoginAssistS(*this);
}

LoginAssistOperS& LoginAssistS::GetLoginAssistOper()
{
	return oper_;
}

LoginAssistS::LoginAssistS( Server& ser ) : Session(ser), oper_(*this)
{

}

LoginAssistOperS::LoginAssistOperS( LoginAssistS& login_assist ) : JsonEchoBase(*Lua_l::GetInstance(), login_assist), login_assist_(login_assist)
{

}

bool LoginAssistOperS::OnThisMemCall( const stThisCallHelper& helper )
{
	return OnThisFuncCallImpl(this, helper);
}

void LoginAssistOperS::SafeCheckDone()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "安全检查完全";
}

#pragma once
/*
@author zhp
@date 2014/7/25 22:59
@purpose for login helper,against for TASLogin.exe
*/
#include <Net/server.h>
#include <Common/UsefulClass.h>
#include <Net/JsonCall.h>

class LoginAssistServer : public Server, public Singleton<LoginAssistServer, Singleton_Instance>{
public:
	LoginAssistServer();

private:
	virtual Session* CreateSession();
};

class LoginAssistS;
class LoginAssistOperS : public JsonEchoBase{
public:
	LoginAssistOperS( LoginAssistS& login_assist );
	//安全检查完成
	void SafeCheckDone();

protected:
	virtual bool OnThisMemCall( const stThisCallHelper& helper );

private:
	LoginAssistS&				login_assist_;
};

class LoginAssistS : public Session{
public:
	LoginAssistS( Server& ser );

public:
	LoginAssistOperS& GetLoginAssistOper();

private:
	LoginAssistOperS				oper_;
};
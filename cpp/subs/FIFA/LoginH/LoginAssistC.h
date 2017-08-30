#pragma once
/*
@author zhp
@date 2014/7/25 23:36
@purpose connection of login assist
*/
#include "Socket/client.h"
#include <Common/UsefulClass.h>
#include <Script/JsonCall.h>

class LoginAssistC;
class LoginAssistOperC : public JsonEchoBase{
public:
	LoginAssistOperC( LoginAssistC& login_assist );
	void Test();

protected:
	virtual bool OnThisMemCall( const stThisCallHelper& helper );

private:
	LoginAssistC&				login_assist_;
};

class LoginAssistC : public Connection_Aes, public Singleton<LoginAssistC, Singleton_Instance>{
public:
	LoginAssistC();
	LoginAssistOperC& GetLoginAssistOper();

private:
	LoginAssistOperC				oper_;
};
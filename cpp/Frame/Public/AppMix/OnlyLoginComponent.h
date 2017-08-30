#pragma once
/*
@author zhp
@date 2017/5/3 17:46
@purpose for login component
*/
#include "GameComponent.h"
#include <Common/CrackCaptcha.h>

//////////////////////////////////////////////////////////////////////////
struct Cmn_LoginDllData : Cmn_DllData{
	Cmn_LoginDllData();
};
//////////////////////////////////////////////////////////////////////////
class LoginConnectionOper : public CmnGameConnectionOper{
public:
	LoginConnectionOper(LoginConnectionC& connection);

protected:
	void SetLocalCrackCaptcha_(const char* remote_ip, pt_word remote_port);
	void SetDmtCrackCaptcha_(const char* account, const char* password);
	void BeginLogin_();

private:
	void OnCheckInfo_(pt_csz version);

private:
	BC_REG_CLS_DECLARE(LoginConnectionOper);
};
//////////////////////////////////////////////////////////////////////////
class LoginConnectionC : public CmnGameConnectionC{
public:
	LoginConnectionC(Cmn_GameDllApp& app, pt_word dest_port);
	void ReloginAc();

protected:
	bool PreConnect() override;
	bool OnConnected(IoConnectBase* connect) override;
	void OnDisconnect(IoConnectBase* connect) override;

private:
	LoginConnectionOper* CreateConnectionOper() override final;
};
//////////////////////////////////////////////////////////////////////////
class OnlyLoginAppFactory : public GameAppFactory{
public:
	LoginConnectionC* CreateLoginConnection(pt_word dest_port) override;
	LoginConnectionOper* CreateLoginConnectionOper(LoginConnectionC& connection) override;
	Cmn_LoginDllData* CreateLoginDllData() override;
	virtual DaMaTu* CreateDaMaTu();
};

class Cmn_OnlyLoginDllApp : public Cmn_GameDllApp{
	friend class LoginConnectionOper;

public:
	Cmn_OnlyLoginDllApp();
	~Cmn_OnlyLoginDllApp();
	OnlyLoginAppFactory& GetAppFactory() const;
	Cmn_LoginDllData& GetLoginDllData() const;
	static Cmn_OnlyLoginDllApp& GetApp();
	LoginConnectionC* GetConnection();
	virtual bool PreBeginLogin();
	virtual void BeginLogin();

protected:
	bool OnInit_() override;
	OnlyLoginAppFactory* CreateAppFactory() override;
	void BindFrame(lua_State* l) override;

private:
	void __SetLocalCrackCaptcha__(const char* remote_ip, pt_word remote_port);
	void __SetDmtCrackCaptcha__(const char* account, const char* password);
	void __BeginLogin__();

protected:
	ICrackCaptcha*		crack_captcha_;
	bool				is_begin_login_;
};
//////////////////////////////////////////////////////////////////////////
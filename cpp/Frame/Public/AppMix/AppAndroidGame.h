#pragma once
/*
@author zhp
@date 2017/4/3 15:52
@purpose for android
*/
#include "OnlyGameComponent.h"
#include "OnlyLoginComponent.h"
#include "AndroidExecEnv.h"

//////////////////////////////////////////////////////////////////////////
class AndroidGameLuaThrdData;
class AndroidGameConnection;
class AndroidLoginConnection;
class AndroidGameConnectionOper;
class AndroidLoginConnectionOper;
class AndroidGameAppFactory;
class AndroidGameDllApp;
//////////////////////////////////////////////////////////////////////////
class AndroidGameLuaThrdData : public GameDllLuaThrdData{
public:
	explicit AndroidGameLuaThrdData(lua_State* lstate);

protected:
	void BindScriptCommand(luabind::module_& mod);

	//Ω≈±æ√¸¡Ó
public:
	static bool CopyAssetFileToShareDir(const char* file_name, const char* new_file_name);
	static bool InstallApk(const char* apk_name, const char* package_name);
	static bool PmInstallApk(const char* apk_name, const char* package_name);
	static bool UnInstallApk(const char* package_name);
	static bool StartApp(const char* package_name, const char* activity_name);
	static bool StopApp(const char* package_name);
};
//////////////////////////////////////////////////////////////////////////
//**************opers
class AndroidLoginConnectionOper : public LoginConnectionOper{
public:
	AndroidLoginConnectionOper(LoginConnectionC& connection);

private:
	void SetMemuEnvPath_(const std::string& path);

private:
	BC_REG_CLS_DECLARE(AndroidLoginConnectionOper);
};

class AndroidGameConnectionOper : public GameConnectionOper{
public:
	AndroidGameConnectionOper(GameConnectionC& connection);

private:
	BC_REG_CLS_DECLARE(AndroidGameConnectionOper);
};
//////////////////////////////////////////////////////////////////////////
//**************connections
class AndroidLoginConnection : public LoginConnectionC{
public:
	AndroidLoginConnection(AndroidGameDllApp& app, pt_word dest_port);

protected:
	bool OnConnected(IoConnectBase* connect) override;
};

class AndroidGameConnection : public GameConnectionC{
public:
	AndroidGameConnection(AndroidGameDllApp& app, pt_word dest_port);
	AndroidGameDllApp& GetApp();

protected:
	bool OnConnected(IoConnectBase* connect) override;
	void OnDisconnect(IoConnectBase* connect) override;
};
//////////////////////////////////////////////////////////////////////////
class AndroidGameAppFactory : public OnlyGameAppFactory{
public:
	AndroidGameLuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
	AndroidGameConnection* CreateGameConnection(pt_word dest_port) override;
	AndroidLoginConnection* CreateLoginConnection(pt_word dest_port) override;
	AndroidGameConnectionOper* CreateGameConnectionOper(GameConnectionC& connection) override;
	AndroidLoginConnectionOper* CreateLoginConnectionOper(LoginConnectionC& connection) override;
	CmnGameScriptEcho* CreateScriptEcho(GameConnectionC& connection) override;
	ScriptRunRAII* CreateScriptRunRAII() override;
	LuaCfgParser* CreateLuaCfgParser(GameConnectionC& connection) override;
	Cmn_GameDllData* CreateGameDllData() override;
	Cmn_LoginDllData* CreateLoginDllData() override;
	virtual MEmuExecEnv* CreateMEmuExecEnv();
};

class AndroidGameDllApp : public Cmn_OnlyGameDllApp{
public:
	AndroidGameDllApp();
	~AndroidGameDllApp();
	Cmn_LoginDllData& GetLoginDllData() const;
	Cmn_GameDllData& GetGameDllData() const;
	AndroidGameAppFactory& GetAppFactory() const;
	MEmuExecEnv& GetExecEnv();
	static AndroidGameDllApp& GetApp();

protected:
	bool OnInit_() override;
	void BindFrame(lua_State* l) override;
	AndroidGameAppFactory* CreateAppFactory() override;

protected:
	MEmuExecEnv*			memu_exec_env_;

public:
	MEmuExecEnv::IpInfoT	memu_adb_ip_info_;
	std::string				adb_device_info_;

protected:
	bool					is_use_dm_;
	bool					is_use_state_node_;
	bool						is_use_filesystem_;
};
//////////////////////////////////////////////////////////////////////////
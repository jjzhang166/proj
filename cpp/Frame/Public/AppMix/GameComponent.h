#pragma once
/*
@author zhp
@date 2016/12/23 15:56
@purpose game component
*/
#include <AppBase/CmnWxDllApp.h>
#include "WgLuaVm.h"
#include <Net/client.h>
#include <Net/Bufcall_Socket.h>
#include <Actions/CA_Types.h>

//////////////////////////////////////////////////////////////////////////
class LoginConnectionC;
class GameConnectionC;
class GameConnectionOper;
class LoginConnectionOper;
class CmnGameConnectionC;
class Cmn_GameDllApp;
class CmnGameScriptEcho;
class ScriptRunRAII;
//////////////////////////////////////////////////////////////////////////
struct Cmn_DllData{
	Cmn_DllData();
	virtual ~Cmn_DllData();
	virtual void clear();
	CMN_CAST_TO

	std::string		dll_version_;
	pt_word			port_;
};
//////////////////////////////////////////////////////////////////////////
//**************opers
class CmnGameConnectionOper : public BufCall_SocketExcept{
public:
	CmnGameConnectionOper(CmnGameConnectionC& connection);

private:
	void SetAcInfo_(const stAsSerSessionInfo& ac_info);
	void OnConnectDone_(bool is_connected);

protected:
	CmnGameConnectionC&		connection_;

	BC_REG_CLS_DECLARE(CmnGameConnectionOper);
};
//////////////////////////////////////////////////////////////////////////
//**************connections
class CmnGameConnectionC : public Connection{
	friend class CmnGameConnectionOper;
public:
	CmnGameConnectionC(Cmn_GameDllApp& app, pt_word dest_port);
	~CmnGameConnectionC();
	Cmn_GameDllApp& GetApp();
	CmnGameConnectionOper* GetConnectionOper() const;
	const stAsSerSessionInfo& GetAcInfo() const;
	void SendAcPwdErr();
	void SendBanAc();
	void SendFreeze();
	void SetStateAtDisconnect(const char* final_state);
	virtual void ModifyConsoleCfg(const std::string& key, const luabind::object& value);
	virtual void ModifyConsoleCfg(const std::string& key, const std::string& value, bool as_string);

protected:
	bool PreConnect() override;
	bool OnConnected(IoConnectBase* connect) override;
	virtual CmnGameConnectionOper* CreateConnectionOper() = 0;

private:
	bool ConnectDone() override final;

protected:
	CmnGameConnectionOper*		connect_oper_;
	Cmn_GameDllApp&				my_app_;
	stAsSerSessionInfo			ac_info_;

private:
	OptionalBool				notify_connected_done_;
};

//////////////////////////////////////////////////////////////////////////
class GameAppFactory : public Cmn_DllRunAppFactory{
public:
	CmnGameLuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
	virtual CmnGameConnectionC* CreateGameConnection(pt_word dest_port);
	virtual CmnGameConnectionC* CreateLoginConnection(pt_word dest_port);
	virtual GameConnectionOper* CreateGameConnectionOper(GameConnectionC& connection);
	virtual LoginConnectionOper* CreateLoginConnectionOper(LoginConnectionC& connection);
	virtual CmnGameScriptEcho* CreateScriptEcho(GameConnectionC& connection);
	virtual ScriptRunRAII* CreateScriptRunRAII();
	virtual LuaCfgParser* CreateLuaCfgParser(GameConnectionC& connection);
	virtual Cmn_DllData* CreateGameDllData();
	virtual Cmn_DllData* CreateLoginDllData();
	virtual CA_T<CA_AsTimerAction> MakeAsTimerAction(const CA_ActionPtr& decoration, const CA_ActionPtr& timer_action);
};

class Cmn_GameDllApp : public Cmn_DllRunApp{
	friend class LoginConnectionC;
	friend class GameConnectionC;
	typedef boost::shared_ptr<CmnGameConnectionC> GameConnectionPtr;
	typedef boost::shared_ptr<CmnGameConnectionC> LoginConnectionPtr;

public:
	Cmn_GameDllApp();
	~Cmn_GameDllApp();
	const std::string& GetAcItemKey() const;
	GameAppFactory& GetAppFactory() const;
	Cmn_DllData& GetGameDllData() const;
	Cmn_DllData& GetLoginDllData() const;
	const stAsSerSessionInfo& GetAcInfo() const;
	void SetAcInfo(const stAsSerSessionInfo& ac_info);
	void ModifyConsoleCfg(const std::string& key, const luabind::object& value);
	void ModifyConsoleCfg(const std::string& key, const std::string& value, bool as_string);
	virtual luabind::object GetConsoleCfg();
	static Cmn_GameDllApp& GetApp();

protected:
	bool DoInit() override;
	bool OnInit_() override;
	int OnExit_() override;
	void BindFrame(lua_State* l) override;
	CmnGameConnectionC* GetGameConnection();
	CmnGameConnectionC* GetLoginConnection();
	GameAppFactory* CreateAppFactory() override;
	virtual void OnSetConsoleCfg(const luabind::object& console_cfg);

private:
	void OnLoginDisconnect_();
	std::string GetAssetDirString() const;

protected:
	GameConnectionPtr			game_connection_;
	LoginConnectionPtr			login_connection_;
	std::string					ac_item_key_;
	stAsSerSessionInfo			ac_info_;
	Cmn_DllData*				game_dll_data_;
	Cmn_DllData*				login_dll_data_;

private:
	bool						is_on_exited_;
};
//////////////////////////////////////////////////////////////////////////
class CmnGameRoleInfo : public LuaSerializable{
public:
	enum enRoleState{
		kRS_Free,
		kRS_TimeRestricted,		//时间限制的
		kRS_Boring,				//相当于黑名单
	};

public:
	CmnGameRoleInfo();
	explicit CmnGameRoleInfo(const std::string& role_key);
	bool SerializeR(const luabind::object& role_info) override;
	bool SerializeW(luabind::object& role_info) const override;

public:
	std::string			role_key_;			//表示一个角色
	enRoleState			role_state_;
	DateTimeLocal		time_restricted_;	//限制截止日期
};
//////////////////////////////////////////////////////////////////////////
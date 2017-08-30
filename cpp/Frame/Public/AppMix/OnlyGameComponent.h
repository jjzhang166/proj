#pragma once
/*
@author zhp
@date 2017/5/3 17:41
@purpose for game component
*/
#include "GameComponent.h"
#include <Script/LuaScript.h>
#include <WgFacility/CmnSceneHandler.h>

//////////////////////////////////////////////////////////////////////////
class CmnGameScriptEcho;
//////////////////////////////////////////////////////////////////////////
struct Cmn_GameDllData : Cmn_DllData{
	struct stScriptExecEnvInfo{
		std::string			key_;
		luabind::object		exec_env_;
	};
	typedef std::vector<stScriptExecEnvInfo> ScriptExecEnvContT;
	//////////////////////////////////////////////////////////////////////////
	Cmn_GameDllData();
	void clear() override;
	//添加脚本执行环境
	bool AddScriptExecEnv(const char* key, const luabind::object& exec_env);
	bool EraseScriptExecEnv(const char* key);
	void FreeScriptExecEnv();

	WXHWND				game_top_hwnd_;
	luabind::object		script_exec_env_;

private:
	ScriptExecEnvContT	script_exec_env_array_;
};
//////////////////////////////////////////////////////////////////////////
class GameConnectionOper : public CmnGameConnectionOper{
public:
	GameConnectionOper(GameConnectionC& connection);
	GameConnectionC& GetConnection() const;

private:
	void OnCheckInfo_(const DateTimeLocal& end_date_time, const std::vector<DWORD>& addr_datas, const std::vector<std::string>& str_datas, pt_csz version);
	void OnSetLuaCfgStr_(const std::string& lua_serialize);
	void ForegroundTheWindow();

private:
	BC_REG_CLS_DECLARE(GameConnectionOper);
};
//////////////////////////////////////////////////////////////////////////
class GameConnectionC : public CmnGameConnectionC{
	friend class GameConnectionOper;
	friend class CmnGameScriptEcho;
public:
	GameConnectionC(Cmn_GameDllApp& app, pt_word dest_port);
	~GameConnectionC();
	CmnGameScriptEcho* GetScriptEcho() const;
	LuaCfgParser& GetLuaCfgParser();
	const luabind::object& GetConsoleCfg() const;
	void ModifyConsoleCfg(const std::string& key, const luabind::object& value) override;
	void ModifyConsoleCfg(const std::string& key, const std::string& value, bool as_string) override;
	void GameLoginSucceed();

protected:
	bool PreConnect() override;
	bool OnConnected(IoConnectBase* connect) override;
	void OnDisconnect(IoConnectBase* connect) override;

private:
	GameConnectionOper* CreateConnectionOper() override final;
	void SetLuaSerialize(const std::string& lua_str);

protected:
	boost::optional<DateTimeLocal>				end_date_time_;
	boost::optional<std::vector<DWORD> >		addr_datas_;
	boost::optional<std::vector<std::string> >	str_datas_;
	CmnGameScriptEcho*							script_echo_;
	LuaCfgParser*								cfg_parser_;
	luabind::object								console_cfg_;
};
//////////////////////////////////////////////////////////////////////////
class CmnGameScriptEcho : public LuasScriptEchoC{
public:
	CmnGameScriptEcho(GameConnectionC& parent);

protected:
	bool RunLuaScript(const std::string& lua_file_name, const std::string& lua_serialize) override;
	bool OperResult(enOperResult result) override;
	void PreCallScriptFn(const luabind::object& lua_file_func) override;
	luabind::object GetConsoleCfg() const override;

private:
	ScriptRunRAII* CreateRunRAII() override final;

protected:
	GameConnectionC&		connection_;

	BC_REG_CLS_DECLARE(CmnGameScriptEcho);
};
//////////////////////////////////////////////////////////////////////////
class OnlyGameAppFactory : public GameAppFactory{
public:
	GameDllLuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
	GameConnectionC* CreateGameConnection(pt_word dest_port) override;
	GameConnectionOper* CreateGameConnectionOper(GameConnectionC& connection) override;
	CmnGameScriptEcho* CreateScriptEcho(GameConnectionC& connection) override;
	ScriptRunRAII* CreateScriptRunRAII() override;
	LuaCfgParser* CreateLuaCfgParser(GameConnectionC& connection) override;
	Cmn_GameDllData* CreateGameDllData() override;
	virtual CmnGameRoleInfo* CreateGameRoleInfo();
	virtual CSH_HandlerPtr CreateSceneRecycleHandler(const luabind::object& tbl);
	CA_T<CA_AsTimerAction> MakeAsTimerAction(const CA_ActionPtr& decoration, const CA_ActionPtr& timer_action) override;
};

class Cmn_OnlyGameDllApp : public Cmn_GameDllApp{
public:
	typedef boost::shared_ptr<CmnGameRoleInfo> GameRoleInfoPtr;
	typedef std::vector<GameRoleInfoPtr> GameRoleInfoCont;
	typedef std::vector<std::string> GameRolesKeyCont;

public:
	Cmn_OnlyGameDllApp();
	Cmn_GameDllData& GetGameDllData() const;
	GameConnectionC* GetGameConnection();
	GameConnectionC* GetConnection();
	static Cmn_OnlyGameDllApp& GetApp();
	void SendGameLoginSucceed();
	OnlyGameAppFactory& GetAppFactory() const;
	void SetStateAtDisconnect(const char* final_state);
	bool ActiveInitThread();
	void CloseInitThread();
	luabind::object GetConsoleCfg() override;
	//////////////////////////////////////////////////////////////////////////
	//角色信息相关
public:
	void AddGameRolesInfo(const GameRolesKeyCont& roles_key);
	void SetGameRoleInfo(const std::string& role_key, CmnGameRoleInfo::enRoleState role_state);
	void SetGameRoleInfo(const std::string& role_key, const DateTimeLocal& time_restricted);
	//设置最大角色数量，如果数量超过此数，就不会再自动创建角色了
	void SetMaxGameRoles(pt_dword max_game_roles);
	//自动创建选择角色
	virtual bool AutoCreateSelectRole();

protected:
	virtual bool SelectEnterToGame(const CmnGameRoleInfo& role_info);
	virtual enCmnCreateRoleRes CreateEnterToGame(const std::string& role_name);
	virtual void OnInitThreadRun();
	virtual std::string RandomGameRoleName(pt_dword min_cnt, pt_dword max_cnt) const;

private:
	void SaveGameRolesInfo();
	std::string _RandomGameRoleNameImpl_();
	static void SetMaxGameRoles_(pt_dword max_game_roles);
	static bool AutoCreateSelectRole_();
	//////////////////////////////////////////////////////////////////////////
	bool _AddScriptExecEnv_1_(const luabind::object& exec_env);
	bool _AddScriptExecEnv_2_(const char* key, const luabind::object& exec_env);
	bool _EraseScriptExecEnv_(const char* key);
	long _GetGameTopHwnd_() const;

protected:
	bool OnInit_() override;
	int OnExit_() override;
	OnlyGameAppFactory* CreateAppFactory() override;
	void BindFrame(lua_State* l) override;
	void OnSetConsoleCfg(const luabind::object& console_cfg) override;

public:
	pt_dword				as_timer_action_time_;

protected:
	GameRoleInfoCont		game_roles_;
	pt_dword				max_game_roles_;
	bool					ch_role_name_;
	std::size_t				init_thread_stack_size_;

private:
	HANDLE					event_init_thread_;
	bool					init_thread_valid_;
};
//////////////////////////////////////////////////////////////////////////
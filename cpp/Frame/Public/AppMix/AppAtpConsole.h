#pragma once
/*
@author zhp
@date 2017/4/3 9:24
@purpose for atp app
*/
#include "ConsoleComponent.h"

class AtpConsoleLoginSessionOper : public ConsoleLoginSessionOper{
public:
	AtpConsoleLoginSessionOper(ConsoleLoginSessionS& session);

private:
	BC_REG_CLS_DECLARE(AtpConsoleLoginSessionOper);
};

class AtpConsoleLoginSession : public ConsoleLoginSessionS{
public:
	AtpConsoleLoginSession(Server& server);
};

class AtpUiOperMain : public ConsoleAppUiOperMain{
public:
	AtpUiOperMain(WxLuaApp_Console& app);

protected:
	bool DoLoginOne(const std::string& key, const stAcSerInfo& ac_info) override;
};

class AtpAppConsoleFactory : public ConsoleAppFactory{
public:
	AtpConsoleLoginSession* CreateLoginSession(ConsoleLoginServer& server) override;
	AtpConsoleLoginSessionOper* CreateLoginSessionOper(ConsoleLoginSessionS& session) override;
	AtpUiOperMain* CreateAppUiOper(WxLuaApp_Console& app) override;
};

class AtpAppConsole : public WxLuaApp_Console{
	friend class AtpConsoleLoginSession;
public:
	AtpAppConsole();
	static AtpAppConsole& GetApp();

protected:
	AtpAppConsoleFactory* CreateAppFactory() override;
	void BindFrame(lua_State* l) override;
	void AddAreaSer2Id(const std::string& area_name, const std::string& ser_name, pt_dword ser_id);
	void ParseAreaSer2Id(const luabind::object& tbl);
	bool PreLoginGame() override;
	boost::filesystem::path GetStarterRalativePath() const override;
};

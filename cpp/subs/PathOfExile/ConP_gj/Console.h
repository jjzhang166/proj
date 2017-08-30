#pragma once
/*
@author zhp
@date 2016/11/20 11:49
@purpose for jyzj
*/
#include <AppMix/ConsoleComponent.h>
#include "MyAcOper.h"
#include <Data/public/CG_Public.h>
#include <CommonX/CmnX_Math.h>

class IoLuaThrdData : public LuaThrdData{
};

class MyThrdIo : public Cmn_ThrdIo_Console{
};


class AppLuaThrdData : public LuaThrdData{
public:
	AppLuaThrdData(lua_State* lstate);

private:
	//void BindClass() override;
};

class ConsoleLoginMgrImpl : public ConsoleLoginMgr{
public:
	ConsoleLoginMgrImpl(ConsoleAppUiOperIo& ui_oper);

protected:
	bool HandleAttachSessionGame(long pid, const std::string& item_key, CmnConsoleSession& session) override;
};

class MySessionOper : public ConsoleSessionOper{
public:
	MySessionOper(ConsoleSessionS& session) : ConsoleSessionOper(session){}
	DlgMapOper* GetDlgMapOper();
	void OnSetMapData(const CG_MapBitset& bset, const stSizeInt& map_size, const std::string& map_name);
	void OnSetPlayerPos(const stVec2Int& player_pos);
	BC_REG_CLS_DECLARE(MySessionOper);
};

class MyConsoleSession : public ConsoleSessionS{
public:
	MyConsoleSession(Server& server) : ConsoleSessionS(server){}
};

class MyAppFactory : public ConsoleAppFactory{
	LuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
	MyThrdIo* CreateIoThrd() override;
	LuaThrdData* CreateIoThrdLuaVm() override;
	ConsoleLoginMgr* CreateConsoleLoginMgr(ConsoleAppUiOperIo& ui_oper) override;
	MySessionOper* CreateGameSessionOper(ConsoleSessionS& session) override;
	MyConsoleSession* CreateGameSession(ConsoleServer& server) override;
};

class CConsoleApp : public WxLuaApp_Console{
public:
	CConsoleApp();
	MyAcOperMgr& GetAcOperMgr(){ return my_ac_oper_mgr_; }

	// Overrides
private:
	//bool PreLoginGame() override{ return true; }
	bool OnInit_() override;
	int OnExit_() override;
	void BindFrame(lua_State* l) override;
	MyAppFactory* CreateAppFactory() override;
	bool ModifySomeGameFiles() override;
	boost::filesystem::path GetStarterRalativePath() const override;
	void OnOpenMfcDlg();

private:
	void Test(lua_State* l);

private:
	MyAcOperMgr		my_ac_oper_mgr_;
};

CMN_MY_WX_APP_DECLARE(CConsoleApp);

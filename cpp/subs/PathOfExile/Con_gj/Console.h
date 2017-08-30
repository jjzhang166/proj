#pragma once
/*
@author zhp
@date 2016/11/20 11:49
@purpose for jyzj
*/
#include <AppMix/ConsoleComponent.h>


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

class MyAppFactory : public ConsoleAppFactory{
	LuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
	MyThrdIo* CreateIoThrd() override;
	LuaThrdData* CreateIoThrdLuaVm() override;
	ConsoleLoginMgr* CreateConsoleLoginMgr(ConsoleAppUiOperIo& ui_oper) override;
};

class CConsoleApp : public WxLuaApp_Console{
public:
	CConsoleApp();

	// Overrides
private:
	bool OnInit_() override;
	int OnExit_() override;
	void BindFrame(lua_State* l) override;
	MyAppFactory* CreateAppFactory() override;
	bool ModifySomeGameFiles() override;
	boost::filesystem::path GetStarterRalativePath() const override;
	pt_dword GetMaxConnectedCntByAcType(pt_dword ac_type) const override;

private:
	void Test(lua_State* l);

public:
};

CMN_MY_WX_APP_DECLARE(CConsoleApp);

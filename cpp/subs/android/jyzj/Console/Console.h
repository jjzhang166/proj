#pragma once
/*
@author zhp
@date 2016/11/20 11:49
@purpose for jyzj
*/
#include <AppMix/AppComponent.h>


class IoLuaThrdData : public LuaThrdData{
};

class MyThrdIo : public AndroidThrdIo_Console{
};


class AppLuaThrdData : public LuaThrdData{
public:
	AppLuaThrdData(lua_State* lstate);

private:
	//void BindClass() override;
};

class MyAppFactory : public AndroidConsoleAppFactory{
	LuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
	MyThrdIo* CreateIoThrd() override;
	LuaThrdData* CreateIoThrdLuaVm() override;
	Cmn_LuaCfgResolver* CreateLuaCfgResolver(Cmn_LuaCfgResolver::enTag tag) override;
};

class CConsoleApp : public AndroidApp_Console{
public:
	CConsoleApp();

	// Overrides
private:
	bool OnInit_() override;
	int OnExit_() override;
	void BindFrame(lua_State* l) override;
	MyAppFactory* CreateAppFactory() override;

private:
	void Test(lua_State* l);

public:
};

CMN_MY_WX_APP_DECLARE(CConsoleApp);

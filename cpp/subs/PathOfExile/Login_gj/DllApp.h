#pragma once
/*
@author zhp
@date 2016/12/23 14:34
@purpose dll app
*/
#include <AppMix/OnlyLoginComponent.h>

class MyAppFactory : public OnlyLoginAppFactory{
};

class App : public Cmn_OnlyLoginDllApp{
public:
	App();

private:
	bool OnInit_() override;
	void BindFrame(lua_State* l) override;
	MyAppFactory* CreateAppFactory() override;
};
CMN_MY_APP_DECLARE(App);

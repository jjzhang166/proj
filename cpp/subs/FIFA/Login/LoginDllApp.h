#pragma once
/*
@author zhp
@date 2016/11/17 23:01
@purpose for login
*/
#include <AppBase/CmnWxDllApp.h>
#include <AppBase/CmnWxThrd.h>

class MyIoThrd : public Cmn_ThrdIo{
private:
	bool OnInit_() override;
	LuaThrdData* CreateLuaVm() override;
};

class App : public Cmn_DllApp{
public:
	App();

private:
	bool OnInit_() override;
};
CMN_MY_APP_DECLARE(App);
#pragma once
/*
@author zhp
@date 2016/11/17 17:23
@purpose for game
*/
#include <AppBase/CmnWxDllApp.h>
#include <AppBase/CmnWxThrd.h>


class MyIoThrd : public Cmn_ThrdIo{
private:
	bool OnInit_() override;
	int OnExit_() override;
	LuaThrdData* CreateLuaVm() override;

private:
	std::shared_ptr<LogToDbgviewHelper> g_log_dbg_view;
};

class App : public Cmn_DllApp{
public:
	App();

private:
	bool OnInit_() override;

public:
	HWND		game_main_wnd_;
};
CMN_MY_APP_DECLARE(App);

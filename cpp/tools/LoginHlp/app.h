#pragma once
/*
@author zhp
@date 2016/11/15 18:15
@purpose for Login Help
*/
#include <AppBase/CmnWxDllApp.h>
#include <AppBase/CmnWxThrd.h>


class MyIoThrd : public Cmn_ThrdIo{
private:
	bool OnInit_() override;
};

class App : public Cmn_DllApp{
public:
	App();

private:
	bool OnRun_() override;
	int OnExit_() override;
};
CMN_MY_APP_DECLARE(App);
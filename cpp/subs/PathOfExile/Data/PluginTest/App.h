#pragma once
/*
@author zhp
@date 2017/1/23 22:10
@purpose for plugin test app
*/

#include <AppBase/CmnPluginTest.h>


class App : public CmnPluginTest{
public:
	App();

private:
	bool OnInit_() override;
	void BindFrame(lua_State* l) override;
};
CMN_MY_APP_DECLARE(App);

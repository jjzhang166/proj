#pragma once
/*
@author zhp
@date 2016/12/23 14:34
@purpose dll app
*/
#include <AppMix/GameComponent.h>


class App : public AndroidGameDllApp{
public:
	App();

private:
	bool OnInit_() override;
	void BindFrame(lua_State* l) override;
};
CMN_MY_APP_DECLARE(App);

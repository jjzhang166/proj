#pragma once
/*
@author zhp
@date 2016/12/23 14:34
@purpose dll app
*/
#include <AppMix/AppAtpLogin.h>

class MyAppFactory : public AtpAppLoginFactory{
	DaMaTu* CreateDaMaTu() override;
};

class App : public AtpAppLogin{
public:
	App();

private:
	bool OnInit_() override;
	void BindFrame(lua_State* l) override;
	MyAppFactory* CreateAppFactory() override;
	pt_cwsz GetLoginClientWndTitle() const override;
	stVec2Int GetCaptchaInputPos() const override;
	bool EnteredServiceScene() const override;
};
CMN_MY_APP_DECLARE(App);

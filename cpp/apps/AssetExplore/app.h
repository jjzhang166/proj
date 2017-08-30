#pragma once
/*
@author zhp
@date 2016/11/7 13:01
@purpose for Asset Explore application
*/
#include <AppBase/WxLuaApp.h>


class AppLuaThrdData : public LuaThrdData{
public:
	AppLuaThrdData(lua_State* lstate);

private:
	void BindClass() override;
};

class MyAppFactory : public WxLuaAppFactory{
	LuaThrdData* CreateAppLuaVm(lua_State* lstate);
};

class App : public WxLuaApp{
public:
	App();
	int FileCount(bool is_in) const;
	const char* GetFileName(bool is_in, int idx) const;
	bool AutoAddCfgFiles(AS_NodePtr& node_root) const;

private:
	void OnInitCmdLine(wxCmdLineParser& parser) override;
	bool OnCmdLineParsed(wxCmdLineParser& parser) override;

protected:
	MyAppFactory* CreateAppFactory() override;

private:
	std::vector<std::string>		in_files_;
	std::vector<std::string>		out_files_;
};

CMN_MY_WX_APP_DECLARE(App);

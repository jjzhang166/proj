#pragma once
/*
@author zhp
@date 2016/11/7 12:36
@purpose for wxlua
*/
#include "CmnWxApp.h"
#include "./DependSelfLib.h"
#include <wxlua/wxlstate.h>
#include <luabind/LuaBase.hpp>


class WxLuaAppFactory : public Cmn_AppBaseFactory{
public:
	LuaThrdData* CreateAppLuaVm(lua_State* lstate) override;
};

//±ðÍüÁËIMPLEMENT_APP(MyApp) ºÍ DECLARE_APP(MyApp)
class WxLuaApp : public Cmn_App, public luabind::lua_base{
public:
	WxLuaApp();
	WxLuaAppFactory& GetAppFactory() const;

protected:
	bool OnInit_() override;
	int  OnExit_() override;
	void BindFrame(lua_State* l) override;
	WxLuaAppFactory* CreateAppFactory() override;

private:
	void OnLua(wxLuaEvent& event);
	void HandleLuaMessage(const wxString &msg, int type);
	LuaThrdData* CreateLuaVm() override final;

public:
	wxLuaState          wxlState;

private:
	DECLARE_EVENT_TABLE();
};

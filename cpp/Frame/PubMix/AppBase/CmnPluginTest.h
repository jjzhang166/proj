#pragma once
/*
@author zhp
@date 2017/1/23 20:59
@purpose for plugin test
*/
#include "CmnWxDllApp.h"


class CmnPluginTest : public Cmn_DllRunApp{
protected:
	//bool OnRun_() override;		//可以在这个函数里面实现显示对话框等功能
	void BindFrame(lua_State* l) override;
	void LuaVmSetting(LuaThrdData* lua_vm) const override;

private:
	void RunWithMsgBox(const luabind::object& fn, const std::string& msg);
};
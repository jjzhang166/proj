// Console.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Console.h"
#include <CmnMix/Cmn_LogOut.h>
#include <luabind/raw_call_policy.hpp>
#include <Script/FileSystem.h>

CMN_MY_WX_APP_DEFINE(CConsoleApp);

CConsoleApp::CConsoleApp()
{
	glo_auto_run_script_ = false;
}

bool CConsoleApp::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	return true;
}

int CConsoleApp::OnExit_()
{
	return __super::OnExit_();
}

void CConsoleApp::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<CConsoleApp, AndroidApp_Console>("CConsoleApp")
			.def("Test", &CConsoleApp::Test, raw_call(_2)),
			def("GetMyApp", &GetMyApp)
		];
	RegFileSystemToLua(l);
}

Cmn_LuaCfgResolver* MyAppFactory::CreateLuaCfgResolver(Cmn_LuaCfgResolver::enTag tag)
{
	return __super::CreateLuaCfgResolver(Cmn_LuaCfgResolver::kNormal);
}

void CConsoleApp::Test(lua_State* l)
{
	lua_pushvalue(l, -1);
}

MyAppFactory* CConsoleApp::CreateAppFactory()
{
	return new MyAppFactory;
}

AppLuaThrdData::AppLuaThrdData(lua_State* lstate) : LuaThrdData(lstate)
{

}

MyThrdIo* MyAppFactory::CreateIoThrd()
{
	return new MyThrdIo;
}

LuaThrdData* MyAppFactory::CreateAppLuaVm(lua_State* lstate)
{
	return new AppLuaThrdData(lstate);
}

LuaThrdData* MyAppFactory::CreateIoThrdLuaVm()
{
	return new IoLuaThrdData;
}

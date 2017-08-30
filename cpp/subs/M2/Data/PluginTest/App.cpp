#include "stdafx.h"
#include "App.h"
#include <CD_CommonFiles/Data/CD_Interface.h>

CMN_MY_DLLAPP_DEFINE(App);

App::App()
{

}

bool App::OnInit_()
{
	if (!__super::OnInit_())
	{
		assert(false);
		return false;
	}
	return true;
}

void App::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<App, CmnPluginTest>("App"),
			def("GetMyApp", &GetMyApp)
		];
	CD_Interface::RegSomeForLua(l);
	CD_Interface::RegTestForLua(l);
	CD_Interface::RegDbgInfoForLua(l);
}

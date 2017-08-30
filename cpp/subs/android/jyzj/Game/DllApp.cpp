#include "stdafx.h"
#include "DllApp.h"
#include <Common/UsefulClass.h>

CMN_MY_DLLAPP_DEFINE(App);

bool App::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	return true;
}

App::App()
{

}

void App::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<App, AndroidGameDllApp>("App"),
			def("GetMyApp", &GetMyApp)
		];
}

#include "stdafx.h"
#include "DllApp.h"
#include <Common/UsefulClass.h>

CMN_MY_DLLAPP_DEFINE(App);

bool App::OnInit_()
{
#ifdef _DEBUG
	//wxSetEnv(wxT("__MyZhpAcItemKey__"), "11ab");
#endif
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
			class_<App, Cmn_OnlyLoginDllApp>("App"),
			def("GetMyApp", &GetMyApp)
		];
}

MyAppFactory* App::CreateAppFactory(){
	return new MyAppFactory;
}

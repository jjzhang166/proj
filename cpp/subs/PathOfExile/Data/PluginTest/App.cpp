#include "stdafx.h"
#include "App.h"
#include <CD_CommonFiles/Data/CD_Interface.h>

CMN_MY_DLLAPP_DEFINE(App);

pd_export_c bool DllGetRolePosPtr(stVec3Float& vec3)
{
	auto p = CD_Interface::GetPlayerEntity();
	if (!p)
	{
		assert(false);
		return false;
	}
	auto pos = p->GetPositioned();
	if (!p)
	{
		assert(false);
		return false;
	}
	vec3.x = pos->pos_.x;
	vec3.y = pos->pos_.y;
	return true;
}

pd_export_c void DllGetMapId(std::string& map_id)
{
	auto w = CD_Interface::GetWorld();
	if (!w)
	{
		assert(false);
		return;
	}
	if (!w->world_area_.IsValid())
	{
		assert(false);
		return;
	}
	map_id = WideCharToGbk(CD_SafeName(w->world_area_.tbl_item_->ch_map_name_));
}

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
	CD_Interface::RegTestForLua(l);
	CD_Interface::RegDbgInfoForLua(l);
}

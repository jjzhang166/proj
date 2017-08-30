#include "bindwx_base.h"
#include <wxbind/include/wxcore_bind.h>

luabind::object LuaBindWxPolicy::Functions::WxLuaInitUserData(lua_State* L, const void* obj_ptr, int wxl_type)
{
	assert(wxl_type >= 0);
#ifdef _DEBUG
	auto top = lua_gettop(L);
#endif
	wxluaT_pushuserdatatype(L, obj_ptr, wxl_type);
	assert(top == lua_gettop(L) - 1);
	luabind::object res(luabind::from_stack(L, -1));
	lua_pop(L, 1);
	return res;
}

void LuaBindWxPolicy::Functions::WxLuaAddTrackedWindow(lua_State* L, wxObject* obj)
{
	wxluaW_addtrackedwindow(L, obj);
}

void LuaBindWxPolicy::InitPreExec(tagTrackWindow, lua_State* L, wxObject* obj)
{
	return Functions::WxLuaAddTrackedWindow(L, obj);
}

int LuaBindWxPolicy::GetWxType(wxControl*, wxControl*)
{
	return wxluatype_wxControl;
}

int LuaBindWxPolicy::GetWxType(wxApp*, wxApp*)
{
	return wxluatype_wxApp;
}

int LuaBindWxPolicy::GetWxType(wxPoint*, wxPoint*)
{
	return wxluatype_wxPoint;
}

int LuaBindWxPolicy::GetWxType(wxMenu*, wxMenu*)
{
	return wxluatype_wxMenu;
}

int LuaBindWxPolicy::GetWxType(wxWindow*, wxWindow*)
{
	return wxluatype_wxWindow;
}

int LuaBindWxPolicy::GetWxType(wxSize*, wxSize*)
{
	return wxluatype_wxSize;
}

int LuaBindWxPolicy::GetWxType(wxNotifyEvent*, wxNotifyEvent*)
{
	return wxluatype_wxNotifyEvent;
}

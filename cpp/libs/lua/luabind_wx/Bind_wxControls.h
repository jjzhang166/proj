#pragma once
/*
@author zhp
@date 2016/12/5 14:29
@purpose bind wxControls
*/
#include "bindwx_base.h"
#include <wxlua/wxlstate.h>
#include <wx/headerctrl.h>

class B_wxHeaderColumn : public wxHeaderColumn{
public:
	static void RegForLua(lua_State* l);

private:
	std::string _GetTitle_() const;
};

class B_wxSettableHeaderColumn : public wxSettableHeaderColumn{
public:
	static void RegForLua(lua_State* l);

private:
	void _SetTitle_(const char* title);
};

class B_wxHeaderColumnSimple : public wxHeaderColumnSimple{
public:
	B_wxHeaderColumnSimple(const char* title, int width = wxCOL_WIDTH_DEFAULT, wxAlignment align = wxALIGN_NOT, int flags = wxCOL_DEFAULT_FLAGS);
	static void RegForLua(lua_State* l);
};

LUABIND_WX_DEFINE_CLS(wxHeaderCtrl, tagTrackWindow){
public:
	static void RegForLua(lua_State* l);

private:
	void _SetColumnsOrder_(const luabind::object& tbl_array);
	luabind::object _GetColumnsOrder_(lua_State* l) const;
	bool _ShowColumnsMenu_1(const luabind::object& pt);
	bool _ShowColumnsMenu_2(const luabind::object& pt, const char* title);
	void _AddColumnsItems_1(const luabind::object& menu);
	void _AddColumnsItems_2(const luabind::object& menu, int idColumnsBase);
};

//wxHeaderCtrlEvent可用代理的方式去实现，因为用LUABIND_WX_DEFINE_CLS的方式无法兼容wxLua。
LUABIND_WX_DEFINE_CLS(wxHeaderCtrlSimple, tagTrackWindow){
public:
	B_wxHeaderCtrlSimple();
	B_wxHeaderCtrlSimple(const luabind::object& parent, wxWindowID winid = wxID_ANY, const luabind::object& pos = luabind::NilObject,
		const luabind::object& size = luabind::NilObject, long style = wxHD_DEFAULT_STYLE, const char* name = wxHeaderCtrlNameStr);
	static void RegForLua(lua_State* l);
};

class B_wxHeaderCtrlEvent : public LuaBindWxProxy<wxHeaderCtrlEvent, wxNotifyEvent>{
public:
	explicit B_wxHeaderCtrlEvent(const luabind::object& event) : LuaBindWxProxy(event){}
	int GetColumn() const;
	void SetColumn(int col);
	int GetWidth() const;
	void SetWidth(int width);
	unsigned int GetNewOrder() const;
	void SetNewOrder(unsigned int order);
	static void RegForLua(lua_State* l);
};
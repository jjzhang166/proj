#include "Bind_wxControls.h"
#include <luabind/raw_policy.hpp>


void B_wxHeaderCtrl::_SetColumnsOrder_(const luabind::object& tbl_array)
{
	using namespace luabind;
	if (LUA_TTABLE != type(tbl_array))
	{
		assert(false);
		return;
	}
	wxArrayInt tmp;
	for (iterator it(tbl_array), it_end; it != it_end; ++it)
	{
		tmp.push_back(object_cast<int>(*it));
	}
	return SetColumnsOrder(tmp);
}

luabind::object B_wxHeaderCtrl::_GetColumnsOrder_(lua_State* l) const
{
	using namespace luabind;
	object tbl = newtable(l);
	const auto& order = GetColumnsOrder();
	int i = 1;
	for (auto v : order)
	{
		tbl[i++] = v;
	}
	return tbl;
}

bool B_wxHeaderCtrl::_ShowColumnsMenu_2(const luabind::object& pt, const char* title)
{
	auto the_pos = LuaBindWxPolicy::CastTo(pt, (wxPoint*)nullptr);
	if (!the_pos)
	{
		assert(false);
		return false;
	}
	return ShowColumnsMenu(*the_pos, lua2wx(title));
}

bool B_wxHeaderCtrl::_ShowColumnsMenu_1(const luabind::object& pt)
{
	auto the_pos = LuaBindWxPolicy::CastTo(pt, (wxPoint*)nullptr);
	if (!the_pos)
	{
		assert(false);
		return false;
	}
	return ShowColumnsMenu(*the_pos);
}

void B_wxHeaderCtrl::_AddColumnsItems_1(const luabind::object& menu)
{
	auto the_menu = LuaBindWxPolicy::CastTo(menu, (wxMenu*)nullptr);
	if (!the_menu)
	{
		assert(false);
		return;
	}
	return AddColumnsItems(*the_menu);
}

void B_wxHeaderCtrl::_AddColumnsItems_2(const luabind::object& menu, int idColumnsBase)
{
	auto the_menu = LuaBindWxPolicy::CastTo(menu, (wxMenu*)nullptr);
	if (!the_menu)
	{
		assert(false);
		return;
	}
	return AddColumnsItems(*the_menu, idColumnsBase);
}

std::string B_wxHeaderColumn::_GetTitle_() const
{
	return (const char*)wx2lua(GetTitle());
}

void B_wxHeaderColumn::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l, "wx")[
		class_<wxHeaderColumn>("wxHeaderColumn")
		.def("GetTitle", &B_wxHeaderColumn::_GetTitle_)
		.def("GetWidth", &wxHeaderColumn::GetWidth)
		.def("GetMinWidth", &wxHeaderColumn::GetMinWidth)
		.def("GetAlignment", &wxHeaderColumn::GetAlignment)
		.def("GetFlags", &wxHeaderColumn::GetFlags)
		.def("HasFlag", &wxHeaderColumn::HasFlag)
		.def("IsResizeable", &wxHeaderColumn::IsResizeable)
		.def("IsSortable", &wxHeaderColumn::IsSortable)
		.def("IsReorderable", &wxHeaderColumn::IsReorderable)
		.def("IsHidden", &wxHeaderColumn::IsHidden)
		.def("IsShown", &wxHeaderColumn::IsShown)
		.def("IsSortKey", &wxHeaderColumn::IsSortKey)
		.def("IsSortOrderAscending", &wxHeaderColumn::IsSortOrderAscending)
	];
}

void B_wxSettableHeaderColumn::_SetTitle_(const char* title)
{
	SetTitle(lua2wx(title));
}

void B_wxSettableHeaderColumn::RegForLua(lua_State* l)
{
	B_wxHeaderColumn::RegForLua(l);
	using namespace luabind;
	module(l, "wx")[
		class_<wxSettableHeaderColumn, wxHeaderColumn>("wxSettableHeaderColumn")
		.def("SetTitle", &B_wxSettableHeaderColumn::_SetTitle_)
		.def("SetWidth", &wxSettableHeaderColumn::SetWidth)
		.def("SetMinWidth", &wxSettableHeaderColumn::SetMinWidth)
		.def("SetAlignment", &wxSettableHeaderColumn::SetAlignment)
		.def("SetFlags", &wxSettableHeaderColumn::SetFlags)
		.def("ChangeFlag", &wxSettableHeaderColumn::ChangeFlag)
		.def("SetFlag", &wxSettableHeaderColumn::SetFlag)
		.def("ClearFlag", &wxSettableHeaderColumn::ClearFlag)
		.def("ToggleFlag", &wxSettableHeaderColumn::ToggleFlag)
		.def("SetResizeable", &wxSettableHeaderColumn::SetResizeable)
		.def("SetSortable", &wxSettableHeaderColumn::SetSortable)
		.def("SetReorderable", &wxSettableHeaderColumn::SetReorderable)
		.def("SetHidden", &wxSettableHeaderColumn::SetHidden)
		.def("UnsetAsSortKey", &wxSettableHeaderColumn::UnsetAsSortKey)
		.def("SetSortOrder", &wxSettableHeaderColumn::SetSortOrder)
		.def("ToggleSortOrder", &wxSettableHeaderColumn::ToggleSortOrder)
	];
}

void B_wxHeaderColumnSimple::RegForLua(lua_State* l)
{
	B_wxSettableHeaderColumn::RegForLua(l);
	B_wxHeaderCtrlEvent::RegForLua(l);
	using namespace luabind;
	module(l, "wx")[
		class_<wxHeaderColumnSimple, wxSettableHeaderColumn>("wxHeaderColumnSimple_")
		.enum_("")
		[
			value("wxCOL_WIDTH_DEFAULT", wxCOL_WIDTH_DEFAULT),
			value("wxCOL_WIDTH_AUTOSIZE", wxCOL_WIDTH_AUTOSIZE),
			//////////////////////////////////////////////////////////////////////////
			value("wxCOL_RESIZABLE", wxCOL_RESIZABLE),
			value("wxCOL_SORTABLE", wxCOL_SORTABLE),
			value("wxCOL_REORDERABLE", wxCOL_REORDERABLE),
			value("wxCOL_HIDDEN", wxCOL_HIDDEN),
			value("wxCOL_DEFAULT_FLAGS", wxCOL_DEFAULT_FLAGS)
		],

		class_<B_wxHeaderColumnSimple, wxHeaderColumnSimple>("wxHeaderColumnSimple")
		.def(constructor<const char*>())
		.def(constructor<const char*, int>())
		.def(constructor<const char*, int, wxAlignment>())
		.def(constructor<const char*, int, wxAlignment, int>())
	];
}

B_wxHeaderColumnSimple::B_wxHeaderColumnSimple(const char* title, int width, wxAlignment align, int flags) 
	: wxHeaderColumnSimple(lua2wx(title), width, align, flags)
{

}

void B_wxHeaderCtrl::RegForLua(lua_State* l)
{
	using namespace luabind;
	B_wxHeaderColumnSimple::RegForLua(l);

	module(l, "wx")[
		class_<wxHeaderCtrl>("wxHeaderCtrl")
		.luabase("wx.wxControl")
		.def("SetColumnCount", &wxHeaderCtrl::SetColumnCount)
		.def("GetColumnCount", &wxHeaderCtrl::GetColumnCount)
		.def("IsEmpty", &wxHeaderCtrl::IsEmpty)
		.def("UpdateColumn", &wxHeaderCtrl::UpdateColumn)
		.def("SetColumnsOrder", &B_wxHeaderCtrl::_SetColumnsOrder_)
		.def("GetColumnsOrder", &B_wxHeaderCtrl::_GetColumnsOrder_, raw(_2))
		.def("GetColumnAt", &wxHeaderCtrl::GetColumnAt)
		.def("GetColumnPos", &wxHeaderCtrl::GetColumnPos)
		.def("ResetColumnsOrder", &wxHeaderCtrl::ResetColumnsOrder)
		.def("ShowColumnsMenu", &B_wxHeaderCtrl::_ShowColumnsMenu_1)
		.def("ShowColumnsMenu", &B_wxHeaderCtrl::_ShowColumnsMenu_2)
		.def("AddColumnsItems", &B_wxHeaderCtrl::_AddColumnsItems_1)
		.def("AddColumnsItems", &B_wxHeaderCtrl::_AddColumnsItems_2)
		.def("ShowCustomizeDialog", &wxHeaderCtrl::ShowCustomizeDialog)
		.def("GetColumnTitleWidth", &wxHeaderCtrl::GetColumnTitleWidth)
		.def("AcceptsFocusFromKeyboard", &wxHeaderCtrl::AcceptsFocusFromKeyboard)
	];
}

void B_wxHeaderCtrlSimple::RegForLua(lua_State* l)
{
	B_wxHeaderCtrl::RegForLua(l);
	using namespace luabind;
	module(l, "wx")[
		class_<wxHeaderCtrlSimple, wxHeaderCtrl>("wxHeaderCtrlSimple_")
		.def("InsertColumn", &B_wxHeaderCtrlSimple::InsertColumn)
		.def("AppendColumn", &B_wxHeaderCtrlSimple::AppendColumn)
		.def("DeleteColumn", &B_wxHeaderCtrlSimple::DeleteColumn)
		.def("DeleteAllColumns", &B_wxHeaderCtrlSimple::DeleteAllColumns)
		.def("ShowColumn", &B_wxHeaderCtrlSimple::ShowColumn)
		.def("HideColumn", &B_wxHeaderCtrlSimple::HideColumn)
		.def("ShowSortIndicator", &B_wxHeaderCtrlSimple::ShowSortIndicator)
		.def("RemoveSortIndicator", &B_wxHeaderCtrlSimple::RemoveSortIndicator)
		.enum_("")
		[
			value("wxHD_ALLOW_REORDER", wxHD_ALLOW_REORDER),
			value("wxHD_ALLOW_HIDE", wxHD_ALLOW_HIDE),
			value("wxHD_DEFAULT_STYLE", wxHD_DEFAULT_STYLE)
		],

		class_<B_wxHeaderCtrlSimple, wxHeaderCtrlSimple, weak_self_ptr<B_wxHeaderCtrlSimple> >("wxHeaderCtrlSimple")
		.def(constructor<>())
		.def(constructor<const luabind::object&>())
		.def(constructor<const luabind::object&, wxWindowID>())
		.def(constructor<const luabind::object&, wxWindowID, const luabind::object&>())
		.def(constructor<const luabind::object&, wxWindowID, const luabind::object&, const luabind::object&>())
		.def(constructor<const luabind::object&, wxWindowID, const luabind::object&, const luabind::object&, long>())
		.def(constructor<const luabind::object&, wxWindowID, const luabind::object&, const luabind::object&, long, const char*>())
	];
}

B_wxHeaderCtrlSimple::B_wxHeaderCtrlSimple()
{

}

B_wxHeaderCtrlSimple::B_wxHeaderCtrlSimple(const luabind::object& parent, wxWindowID winid, const luabind::object& pos, 
	const luabind::object& size, long style, const char* name)
	: wxHeaderCtrlSimple(LuaBindWxPolicy::CastTo(parent, (wxWindow*)nullptr), winid, 
	LuaBindWxPolicy::CastTo(pos, wxDefaultPosition), LuaBindWxPolicy::CastTo(size, wxDefaultSize), style, lua2wx(name))
{

}

int B_wxHeaderCtrlEvent::GetColumn() const
{
	return impl_->GetColumn();
}

void B_wxHeaderCtrlEvent::SetColumn(int col)
{
	return impl_->SetColumn(col);
}

int B_wxHeaderCtrlEvent::GetWidth() const
{
	return impl_->GetWidth();
}

void B_wxHeaderCtrlEvent::SetWidth(int width)
{
	return impl_->SetWidth(width);
}

unsigned int B_wxHeaderCtrlEvent::GetNewOrder() const
{
	return impl_->GetNewOrder();
}

void B_wxHeaderCtrlEvent::SetNewOrder(unsigned int order)
{
	return impl_->SetNewOrder(order);
}

void B_wxHeaderCtrlEvent::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l, "wx")[
		class_<B_wxHeaderCtrlEvent>("wxHeaderCtrlEvent")
		.def(constructor<const luabind::object&>())
		.def("GetColumn", &B_wxHeaderCtrlEvent::GetColumn)
		.def("SetColumn", &B_wxHeaderCtrlEvent::SetColumn)
		.def("GetWidth", &B_wxHeaderCtrlEvent::GetWidth)
		.def("SetWidth", &B_wxHeaderCtrlEvent::SetWidth)
		.def("GetNewOrder", &B_wxHeaderCtrlEvent::GetNewOrder)
		.def("SetNewOrder", &B_wxHeaderCtrlEvent::SetNewOrder)
	];
}

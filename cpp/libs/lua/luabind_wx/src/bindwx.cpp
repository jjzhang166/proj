#include "bindwx.h"

void Luabind_Wx_All(lua_State* L)
{
	LuaBind_Wx_Enum(L);
	B_wxHeaderCtrlSimple::RegForLua(L);
}

void LuaBind_Wx_Enum(lua_State* L)
{
	using namespace luabind;
	module(L, "wx")[
		class_<wxAlignment>("wxAlignment")
			.enum_("")
			[
				value("wxALIGN_INVALID", wxALIGN_INVALID),
				value("wxALIGN_NOT", wxALIGN_NOT),
				value("wxALIGN_CENTER_HORIZONTAL", wxALIGN_CENTER_HORIZONTAL),
				value("wxALIGN_CENTRE_HORIZONTAL", wxALIGN_CENTRE_HORIZONTAL),
				value("wxALIGN_LEFT", wxALIGN_LEFT),
				value("wxALIGN_TOP", wxALIGN_TOP),
				value("wxALIGN_RIGHT", wxALIGN_RIGHT),
				value("wxALIGN_BOTTOM", wxALIGN_BOTTOM),
				value("wxALIGN_CENTER_VERTICAL", wxALIGN_CENTER_VERTICAL),
				value("wxALIGN_CENTRE_VERTICAL", wxALIGN_CENTRE_VERTICAL),
				value("wxALIGN_CENTER", wxALIGN_CENTER),
				value("wxALIGN_CENTRE", wxALIGN_CENTRE),
				value("wxALIGN_MASK", wxALIGN_MASK)
			]
	];
}

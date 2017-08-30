#include "stdafx.h"
#include "LoginH_Lua.h"
#include "LoginAssistC.h"

void LoginH_Lua::BindClass()
{
	__super::BindClass();

	using namespace luabind;
	/*module(lua())
		[
			class_<LoginH_Lua, LuaVM>("LoginH_Lua"),
			def("LoginH_LuaInst", &LoginH_Lua::GetInstance),
			class_<LoginAssistOperC, JsonEchoBase>("LoginAssistOperC")
			.def("Test", &LoginAssistOperC::Test)
		];*/
}

LoginH_Lua::LoginH_Lua() : LuaVM(_T("_lass"))
{

}

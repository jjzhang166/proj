#include "stdafx.h"
#include "Lua_l.h"
#include "LoginC.h"
#include "LoginAssistS.h"

Lua_l::Lua_l( FuncObj* parent ) : GLua(parent), LuaVM(_T("_l"))
{

}

void Lua_l::BindGlobalFn()
{
	__super::BindGlobalFn();
}

void Lua_l::BindStruct()
{
	__super::BindStruct();
}

void Lua_l::BindClass()
{
	__super::BindClass();

	using namespace luabind;
	module(lua())
		[
			class_<Lua_l, LuaVM>("Lua_l"),
			def("Lua_l_Instance", &Lua_l::GetInstance),

			class_<LoginC>("LoginC")
			.def("SetAccount_Password", &LoginC::SetAccount_Password)
			.def("SetSerRegion", &LoginC::SetSerRegion)
			.def("SetJoinedService", &LoginC::SetJoinedService)
			.def("SetCrackCaptchaMode", &LoginC::SetCrackCaptchaMode)
			.def("BeginLogin", &LoginC::BeginLogin),
			def("LoginC_Instance", &LoginC::GetInstance),

			class_<LoginOperC, JsonEchoBase>("LoginOperC")
			.def("cls", &LoginOperC::CheckLoginStr)

			/*class_<LoginAssistOperS, JsonEchoBase>("LoginAssistOperS")
			.def("SafeCheckDone", &LoginAssistOperS::SafeCheckDone)*/
		];
}

void Lua_l::BindConst()
{
	__super::BindConst();
}

void Lua_l::BindOther()
{
	__super::BindOther();
}
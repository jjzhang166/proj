#include "stdafx.h"
#include "LuaAc.h"
#include "AC_Conne.h"

void Lua_Ac::BindGlobalFn()
{
	__super::BindGlobalFn();
}

void Lua_Ac::BindStruct()
{
	__super::BindStruct();
}

void Lua_Ac::BindClass()
{
	__super::BindClass();

	using namespace luabind;

	module(lua())
		[
			class_<AC_OperC, JsonEchoBase>("AC_OperC"),
			class_<Lua_Ac, LuaVM>("GLuaVM_S"),
			def("Lua_Ac_Me", &Lua_Ac::GetMe)
		];	
}

void Lua_Ac::BindConst()
{
	__super::BindConst();
}

void Lua_Ac::BindOther()
{
	__super::BindOther();
}

bool Lua_Ac::DoMainLuaFileByFile()
{
	return
#ifdef _LUA_PACK_IN_RES
		false
#else
		true
#endif
		;
}

Lua_Ac::Lua_Ac() : LuaVM(_T("ac"))
{

}

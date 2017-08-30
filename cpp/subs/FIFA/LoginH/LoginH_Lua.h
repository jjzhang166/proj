#pragma once
/*
@author zhp
@date 2014/7/25 23:35
@purpose lua of login helper
*/
#include <Script/lua_base.h>
#include <Common/UsefulClass.h>

class LoginH_Lua : public LuaVM, public Singleton<LoginH_Lua, Singleton_Instance>{
public:
	LoginH_Lua();

protected:
	virtual void BindClass();
};
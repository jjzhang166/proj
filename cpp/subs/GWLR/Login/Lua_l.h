#pragma once
/*
@author zhp
@date 2014/4/9 15:59
@purpose lua of login
*/
#include "Script/lua_base.h"
#include "FuncObjs/FuncObj.h"
#include "Common/UsefulClass.h"

class Lua_l : public LuaVM, public GLua, public Singleton<Lua_l, Singleton_Instance>{
public:
	Lua_l( FuncObj* parent );

protected:
	virtual void BindGlobalFn();
	virtual void BindStruct();
	virtual void BindClass();
	virtual void BindConst();
	virtual void BindOther();
};
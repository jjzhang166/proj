#pragma once
/*
@author zhp
@date 2014/4/9 15:59
@purpose lua of login
*/
#include "Script/lua_base.h"
#include "FuncObjs/FuncObj.h"
#include "Common/UsefulClass.h"

class Lua_l : public LuaVM{
protected:
	void BindClass() override;
};
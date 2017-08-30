#pragma once
/*
@author zhp
@date 2014/6/24 2:07
@purpose for anti crack lua
*/
#include <Script/lua_base.h>
#include <Common/UsefulClass.h>

class Lua_Ac : public LuaVM, public Singleton<Lua_Ac, Singleton_MakeMe>{
public:
	Lua_Ac();

protected:
	//全局函数
	virtual void BindGlobalFn();
	//结构体
	virtual void BindStruct();
	//类
	virtual void BindClass();
	//常量、枚举类型等
	virtual void BindConst();
	//其他
	virtual void BindOther();
	virtual bool DoMainLuaFileByFile();
};
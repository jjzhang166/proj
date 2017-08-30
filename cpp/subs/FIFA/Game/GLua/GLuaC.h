#pragma once
/*
@author zhp
@date 2014-2-24 13:43
@purpose for game lua
*/
#include <FuncObjs/FuncObj.h>
#include <Script/lua_base.h>
#include <Script/LuaScript.h>

class GLuaC : public LuaVM{
public:
	GLuaC();

	//重载
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
	bool InitThrdData() override;
};
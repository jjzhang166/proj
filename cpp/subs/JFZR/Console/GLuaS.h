#pragma once
/*
@author zhp
@date 2014-2-24 13:49
@purpose for game lua
*/
#include "FuncObjs/FuncObj.h"
//#include "Script/lua_base.h"
#include <Script/LuaScript.h>

class GLuaS : public GLua{
public:
	GLuaS( FuncObj* parent );
};

class FuncObj_NormalS;
class LuaScriptS : public LuasScriptEchoS{
public:
	LuaScriptS( LuaVM& lua_vm, FuncObj_NormalS& func_obj );
	bool OutputScriptInfo( const std::string& script_info );

private:
	FuncObj_NormalS&			func_obj_;
};

class GLuaVM_S : public LuaVM{
	GLuaVM_S();

public:
	static GLuaVM_S& GetMe();
	static void ReleaseMe();
	static GLuaVM_S* GetInstance();
	bool InitScript( LPCTSTR module_name, LPCTSTR main_lua_file_name );

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

private:
	static GLuaVM_S*			lua_vm_;
};
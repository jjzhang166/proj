#pragma once
/*
@author zhp
@date 2014-2-24 13:43
@purpose for game lua
*/
#include <FuncObjs/FuncObj.h>
#include <Script/LuaScript.h>
#include "EventsImpl.h"
#include <Common/CmnPriLuaData.h>

class ScriptRunRAII_Impl : public ScriptRunRAII{
public:
	struct stRaiiImpl{
		stRaiiImpl( lua_State* lua_state );

		CmnPriLuaData		obj_filter_lua_env_;
	};

public:
	ScriptRunRAII_Impl( lua_State* lua_state );
	bool PreRun();
	void AfterRun();

private:
	lua_State*					lua_state_;
	std::shared_ptr<stRaiiImpl>	raii_impl_ptr_;
};

class LuasScriptEchoC_Impl : public LuasScriptEchoC{
public:
	LuasScriptEchoC_Impl( LuaVM& lua_vm, FuncObj& parent );

private:
	ScriptRunRAII* GetScriptRunRAII();
	void RunScriptImpl( const std::string& lua_file_name );

public:
	bool						is_some_err_;

private:
	CmnEventsFatory_Impl		events_factory_impl_;
	ScriptRunRAII_Impl			script_raii_impl_;
};

class GLuaC : public LuaVM, public GLua{
public:
	GLuaC( FuncObj* parent );
	virtual bool InitScript( HMODULE hmodule, LPCTSTR main_lua_file_name );

	//重载
public:
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
	virtual bool DoPrivateLuaFileByFile();

private:
	//LuaScriptC::PtrType				lua_script_;
};
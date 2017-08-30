#include "stdafx.h"
#include "GLuaC.h"
#include "FuncObjC.h"
#include "Facility/GPlayer.h"

GLuaC::GLuaC( FuncObj* parent ) : GLua(parent), LuaVM(_T("c"))
{

}

bool GLuaC::InitScript( HMODULE hmodule, LPCTSTR main_lua_file_name )
{
	if ( !__super::InitScript(hmodule, main_lua_file_name) )
		return false;
	LuasScriptEchoC::RegForLua(*this);

	Call(tagNoResult(), "RegForChineseScript");
	return true;
}

bool GLuaC::DoPrivateLuaFileByFile()
{
	return
#ifdef _LUA_PACK_IN_RES
	false
#else
	true
#endif
	;
}

LuasScriptEchoC_Impl::LuasScriptEchoC_Impl( LuaVM& lua_vm, FuncObj& parent )
	: LuasScriptEchoC(lua_vm, parent), events_factory_impl_(lua_vm.lua()), script_raii_impl_(lua_vm.lua())
{
	events_factory_ = &events_factory_impl_;
	is_some_err_ = false;
}

ScriptRunRAII* LuasScriptEchoC_Impl::GetScriptRunRAII()
{
	return &script_raii_impl_;
}

void LuasScriptEchoC_Impl::RunScriptImpl( const std::string& lua_file_name )
{
	if (is_some_err_)
		return;
	__super::RunScriptImpl(lua_file_name);
}

bool ScriptRunRAII_Impl::PreRun()
{
	raii_impl_ptr_.reset(new stRaiiImpl(lua_state_));
	SetGObjFilterLuaEnv(&raii_impl_ptr_->obj_filter_lua_env_);
	GPlayerMe::GetMe().PreRunScript();
	return true;
}

void ScriptRunRAII_Impl::AfterRun()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;
	norc->AfterRunScript();
	SetGObjFilterLuaEnv(nullptr);
	raii_impl_ptr_.reset();

	GPlayerMe::GetMe().AfterRunScript();
	auto& ambient = GClass::GAmbient::GetMe();
	ambient.CancelTimer_FbWin();
}

ScriptRunRAII_Impl::ScriptRunRAII_Impl( lua_State* lua_state )
{
	assert(lua_state);
	lua_state_ = lua_state;
}

ScriptRunRAII_Impl::stRaiiImpl::stRaiiImpl( lua_State* lua_state ) : obj_filter_lua_env_(lua_state, "_A9A024156EC_")
{

}

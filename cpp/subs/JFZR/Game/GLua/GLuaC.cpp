#include "stdafx.h"
#include "GLuaC.h"
#include "FuncObjC.h"
#include "Facility/GPlayer.h"

GLuaC::GLuaC( FuncObj* parent ) : GLua(parent), LuaVM(_T("c"))
{

}

bool GLuaC::InitScript( LPCTSTR module_name, LPCTSTR main_lua_file_name )
{
	/*if (lua_script_)
	{
		assert(!"已经存在了");
		return false;
	}*/

	if ( !__super::InitScript(module_name, main_lua_file_name) )
		return false;
	LuasScriptEchoC::RegForLua(*this);
	//lua_script_.reset(new LuaScriptC(this));

	VCall("RegForChineseScript");
	//lua_script_->RegKeywordForChinese("结束代码块", "end");
	return true;
}

bool GLuaC::DoMainLuaFileByFile()
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
}

ScriptRunRAII* LuasScriptEchoC_Impl::GetScriptRunRAII()
{
	return &script_raii_impl_;
}

void LuasScriptEchoC_Impl::RunScriptImpl( const std::string& lua_file_name )
{
	using namespace luabind;
	try{
		__super::RunScriptImpl(lua_file_name);
	}
	DO_CATCH_EXCEPTION( const luabind::error& e, {
		std::string str_err_msg = "脚本错误,";
		VERIFY(OutputScriptInfo(str_err_msg));
		str_err_msg += "->";
		object err_msg(from_stack(e.state(), -1));		
		str_err_msg += ObjectCast(err_msg, "没有错误信息的脚本错误");
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << str_err_msg;
		assert(false);
		LOG_O(Log_error) << "脚本错误了，断开连接,";
		auto nor = FuncObj_NormalC::GetInstance();
		if (nor)
			nor->Disconnect();
	})
	DO_CATCH_EXCEPTION( const luabind::error* e, {
		assert(e);
		std::string str_err_msg = "脚本错误.";
		VERIFY(OutputScriptInfo(str_err_msg));
		str_err_msg += "->";
		object err_msg(from_stack(e->state(), -1));		
		str_err_msg += ObjectCast(err_msg, "没有错误信息的脚本错误");
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << str_err_msg;
		assert(false);
		LOG_O(Log_error) << "脚本错误了，断开连接.";
		auto nor = FuncObj_NormalC::GetInstance();
		if (nor)
			nor->Disconnect();
	})
}

bool ScriptRunRAII_Impl::PreRun()
{
	raii_impl_ptr_.reset(new stRaiiImpl(lua_state_));
	SetGObjFilterLuaEnv(&raii_impl_ptr_->obj_filter_lua_env_);

	auto& packet = GPlayerSelf::GetMe().GetPacket();
	packet.GetTabNormal().InitItemFilter();
	packet.GetTabMaterial().InitItemFilter();
	packet.GetTabConsume().InitItemFilter();
	return true;
}

void ScriptRunRAII_Impl::AfterRun()
{
	auto& packet = GPlayerSelf::GetMe().GetPacket();
	packet.GetTabNormal().ClearItemFilters();
	packet.GetTabMaterial().ClearItemFilters();
	packet.GetTabConsume().ClearItemFilters();
	SetGObjFilterLuaEnv(nullptr);
	raii_impl_ptr_.reset();
}

ScriptRunRAII_Impl::ScriptRunRAII_Impl( lua_State* lua_state )
{
	assert(lua_state);
	lua_state_ = lua_state;
}

ScriptRunRAII_Impl::stRaiiImpl::stRaiiImpl( lua_State* lua_state ) : obj_filter_lua_env_(lua_state, "_A9A024156EC_")
{

}

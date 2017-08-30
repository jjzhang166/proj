#include "stdafx.h"
#include <Script/LuaCoroutine.h>
#include <Net/IoTimer.h>
#include <Common/UsefulFunc.h>
#include <fstream>
#include <CmnMix/Cmn_LogOut.h>
#include <AssetSys/AS_Sys.h>
#include <AppBase/Cmn_App.h>

ILuaCoroutine::ILuaCoroutine( LuaVM* lua_vm ) : lua_vm_(lua_vm)
{
	assert(lua_vm);
	state_ = kState_Invalid;
	force_terminate_script_run_ = false;
	force_terminate_script_ = false;

	RegForLua();
}

bool ILuaCoroutine::IsValid() const
{
	return kState_Invalid != state_;
}

bool ILuaCoroutine::IsFree() const
{
	return kState_Free == state_;
}

bool ILuaCoroutine::IsRunning() const
{
	return kState_Running == state_;
}

bool ILuaCoroutine::Delay( size_t millisecond )
{
	if (force_terminate_script_run_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "强制中止run";
		return false;
	}
	auto shared_this = shared_from_this();
	SetIoTimer(millisecond, [shared_this]( std::size_t ){
		if ( !shared_this->IsValid() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "脚本coroutine失效了，很可能是脚本指令产生了错误";
			return;
		}
		shared_this->ResumeT();
	}, 1);
	return true;
}

bool ILuaCoroutine::YieldOneFree()
{
	if (force_terminate_script_run_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "强制中止run";
		return false;
	}
	auto shared_this = shared_from_this();
	GetIoService().post([shared_this](){
		shared_this->ResumeT();
	});
	return true;
}

bool ILuaCoroutine::IsTerminateScriptRun()
{
	if (force_terminate_script_)
		return true;

	return force_terminate_script_run_;
}

void ILuaCoroutine::ForceTerminateScriptRun( bool is_terminate )
{
	if ( is_terminate && !IsValid() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "脚本无效状态，无法强制中止脚本";
		return;
	}
	force_terminate_script_run_ = is_terminate;
	if (is_terminate)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "要强制中止run了";
	}
}

bool ILuaCoroutine::IsExecNextRun()
{
	return true;
}

void ILuaCoroutine::RunToEnd()
{
	state_ = kState_Free;
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "脚本执行完了，脚本名："  << lua_file_name_;
}

void ILuaCoroutine::PreRun()
{
	state_ = kState_Running;
	ForceTerminateScriptRun(false);
	LOG_ENTER_FUNC;
	LOG_O(Log_info) << "要执行脚本了，脚本名："  << lua_file_name_;
}

bool ILuaCoroutine::RunLuaScript( const char* lua_file_name, bool is_force )
{
	if ( !lua_file_name )
		return false;

	if (is_force)
	{
		lua_file_name_ = lua_file_name;
		auto shared_this = shared_from_this();
		if (IsRunning())
		{
			GetIoService().post([shared_this](){
				shared_this->ForceTerminateScriptRun(true);
				//中止之前的脚本
				shared_this->ResumeT();
				if ( !shared_this->IsFree() )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_debug) << "强制中止run了，为啥还不在空闲状态";
				}
				shared_this->ForceTerminateScriptRun(false);
				//开始新的脚本
				shared_this->ResumeT();
			});			
		}
		else
		{
			GetIoService().post([shared_this](){
				//shared_this->ForceTerminateScriptRun(false);
				//shared_this->ForceTerminateScript(false);
				shared_this->ResumeT();
			});
		}
	}
	else
	{
		if (IsRunning())
			return false;
		lua_file_name_ = lua_file_name;
		auto shared_this = shared_from_this();
		GetIoService().post([shared_this](){
			shared_this->ResumeT();
		});

	}
	ForceTerminateScriptRun(false);
	return true;
}

void ILuaCoroutine::SetCoState( CoState co_state )
{
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "coroutine state, 之前：" << state_ << "  之后：" << co_state;*/
	state_ = co_state;
}

void ILuaCoroutine::RegForLua()
{
	static bool is_init = false;
	if (is_init)
		return;
	is_init = true;

	using namespace luabind;
	module(lua_vm_->lstate_)
		[
			class_<ILuaCoroutine>("ILuaCoroutine")
			.def("GetScriptFileNameForRun", &ILuaCoroutine::GetScriptFileNameForRun)
			.def("SetCoState", &ILuaCoroutine::SetCoState)
			.def("IsTerminateScriptRun", &ILuaCoroutine::IsTerminateScriptRun)
			.def("IsTerminateScript", &ILuaCoroutine::IsTerminateScript)
			.def("IsExecNextRun", &ILuaCoroutine::IsExecNextRun)
			.def("PreRun", &ILuaCoroutine::PreRun)
			.def("RunToEnd", &ILuaCoroutine::RunToEnd)
			.def("RunLuaScript", &ILuaCoroutine::RunLuaScript)
			.def("Delay", &ILuaCoroutine::Delay)
			.def("YieldOneFree", &ILuaCoroutine::YieldOneFree)
			.def("ForceTerminateScriptRun", &ILuaCoroutine::ForceTerminateScriptRun)
			.def("ForceTerminateScript", &ILuaCoroutine::ForceTerminateScript)
			.def("TerminateScriptByConsole", &ILuaCoroutine::TerminateScriptByConsole),

			class_<CoState>("CoState")
			.enum_("constants")
			[
				value("kState_Invalid", kState_Invalid),
				value("kState_Free", kState_Free),
				value("kState_Running", kState_Running)
			]
		];
}

const char* ILuaCoroutine::GetScriptFileNameForRun()
{
	return lua_file_name_.c_str();
}

ILuaCoroutine::~ILuaCoroutine()
{

}

bool ILuaCoroutine::IsTerminateScript() const
{
	return force_terminate_script_;
}

void ILuaCoroutine::ForceTerminateScript( bool is_terminate )
{
	if ( is_terminate && !IsValid() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "脚本无效，无法强制中止脚本";
		return;
	}
	force_terminate_script_ = is_terminate;
	if (is_terminate)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "要强制中止脚本了，脚本名：" << lua_file_name_;
	}
}

void ILuaCoroutine::TerminateScriptByConsole()
{
	ForceTerminateScript(true);
	auto shared_this = shared_from_this();
	GetIoService().post([shared_this](){
		//恢复
		shared_this->ForceTerminateScript(false);
	});
}

const char* kLuaCo_MainFuncName = "__LuaCoroutine_MainFunc__";
LuaCoroutine::LuaCoroutine( LuaVM* lua_vm ) : ILuaCoroutine(lua_vm)
{
	assert(lua_vm);
	auto lua_state = lua_vm->lstate_;
	assert(lua_state);
	//register
	RegForLua();
	lua_state_new_ = lua_newthread(lua_state);
	assert(lua_state_new_);
}

void LuaCoroutine::RegForLua()
{
	static bool is_init = false;
	if (is_init)
		return;
	is_init = true;

	using namespace luabind;
	module(lua_vm_->lstate_)
		[
			def(kLuaCo_MainFuncName, &MainFunc),

			class_<LuaCoroutine, ILuaCoroutine>("LuaCoroutine")
			.def("Delay", &LuaCoroutine::Delay)
			.def("YieldOneFree", &LuaCoroutine::YieldOneFree)
			//.def("IsForceTerminateRun", &LuaCoroutine::IsForceTerminateRun)
		];
}

void LuaCoroutine::ResumeT()
{
	if ( !lua_state_new_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "lua_state_new_为空";
		return;
	}
	luabind::resume_function<void>(lua_state_new_, kLuaCo_MainFuncName, this);
	auto status = lua_status(lua_state_new_);
	if ( 0 != status && LUA_YIELD != status )
	{
		lua_close(lua_state_new_);
		lua_state_new_ = nullptr;
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "LUA之恢复线程出错";
	}
}

LuaCoroutine::~LuaCoroutine()
{
	if (lua_state_new_)
	{
		lua_close(lua_state_new_);
		lua_state_new_ = nullptr;
	}
}

void LuaCoroutine::YieldT()
{
	//lua_yield(lua_state_new_, 0);
	//lua_vm_->Call(tagNoResult(), "TestYield");
	luabind::call_function<void>(lua_state_new_, "TestYield");
}

void LuaCoroutine::TerminateT()
{
	if (lua_state_new_)
	{
		lua_close(lua_state_new_);
		lua_state_new_ = nullptr;
	}

	SetCoState(kState_Invalid);
}

bool LuaCoroutine::IsValid() const
{
	if ( !__super::IsValid() )
		return false;
	if ( !lua_state_new_ )
		return false;
	auto status = lua_status(lua_state_new_);
	return 0 == status || LUA_YIELD == status;
}

void LuaCoroutine::Restart()
{
	if ( !lua_vm_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "lua_state_src_为空";
		return;
	}
	TerminateT();
	assert(!lua_state_new_);
	lua_state_new_ = lua_newthread(lua_vm_->lstate_);
	assert(lua_state_new_);
}

bool LuaCoroutine::Delay( size_t millisecond )
{
	if ( !__super::Delay(millisecond) )
		return false;
	YieldT();
	return true;
}

bool LuaCoroutine::YieldOneFree()
{
	if ( !__super::YieldOneFree() )
		return false;
	YieldT();
	return true;
}

void LuaCoroutine::MainFunc( LuaCoroutine* lua_co )
{
	assert(lua_co);
	LOG_ENTER_FUNC;
	do
	{
		lua_co->PreRun();
		LOG_O(Log_trace) << "开始执行新的lua coroutine run";

		lua_co->Run();

		lua_co->RunToEnd();
	}
	while (lua_co->IsExecNextRun());

	lua_co->SetCoState(kState_Invalid);
}

bool LuaCoroutine::IsTerminateScriptRun()
{
	if (__super::IsTerminateScriptRun())
		return true;
	if (IsRunning())
		YieldOneFree();
	return __super::IsTerminateScriptRun();
}

bool LuaCoroutine::IsExecNextRun()
{
	YieldT();
	return __super::IsExecNextRun();
}

LuaCoScript::LuaCoScript( LuaVM* lua_vm ) : LuaCoroutine(lua_vm)
{
	RegForLua();
}

void LuaCoScript::RegForLua()
{
	return;
	static bool is_init = false;
	if (is_init)
		return;
	is_init = true;

	using namespace luabind;
	module(lua_vm_->lstate_)
		[
			class_<LuaCoScript, LuaCoroutine>("LuaCoScript")
			//.def("RunLuaScript", &LuaCoScript::RunLuaScript)
		];
}

void LuaCoScript::Run()
{
}

LuaRunCo::LuaRunCo( LuaVM* lua_vm, const char* co_name, const char* co_run_func_name ) : 
	ILuaCoroutine(lua_vm), co_name_(co_name), co_run_func_name_(co_run_func_name)
{
	Init(co_name, co_run_func_name);
}

LuaRunCo::LuaRunCo( LuaVM* lua_vm ) : 
	ILuaCoroutine(lua_vm), co_name_("__g_tblRunOneScript__"), co_run_func_name_("__CoroutineRunScript__")
{
	Init(co_name_.c_str(), co_run_func_name_.c_str());
}

void LuaRunCo::ResumeT()
{
	if ( IsValid() && IsTerminateScript() )
	{
		TerminateT();
		ForceTerminateScript(false);
		return;
	}
	if ( !IsValid() )
		Restart();
	if (!lua_vm_->LuaObjCallByDlt("__CoroutineResume__", false, co_name_.c_str(), this))
	{
		
		//assert(false);
		/*if ( !IsValid() )
		{
			Restart();
			if (lua_vm_->LuaObjCallByDlt("__CoroutineResume__", false, co_name_.c_str(), this))
			{
				return;
			}
		}*/
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "__CoroutineResume__失败";
		return;
	}
}

void LuaRunCo::TerminateT()
{
	lua_vm_->LuaObjCall<void>("__CoroutineReset__", co_name_.c_str());
	if (IsValid())
		LOG_O(Log_warning) << "中止脚本：" << lua_file_name_;
	SetCoState(kState_Invalid);
}

void LuaRunCo::Restart()
{
	TerminateT();
	if (!lua_vm_->LuaObjCallByDlt("__CreateCoroutine__", false, co_name_.c_str(), co_run_func_name_.c_str()))
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "调用__CreateCoroutine__失败";
	}
	else
	{
		SetCoState(kState_Free);
	}
}

void LuaRunCo::RegForLua()
{
	static bool is_init = false;
	if (is_init)
		return;
	is_init = true;

	using namespace luabind;
	typedef std::string (LuaRunCo::*NoParamT)();
	typedef std::string (LuaRunCo::*ParamC_T)( const char* );
	module(lua_vm_->lstate_)
		[
			class_<LuaRunCo, ILuaCoroutine>("LuaRunCo")
			.def("IsEnableForChinese", &LuaRunCo::IsEnableForChinese)
			.def("RegKeywordForChinese", &LuaRunCo::RegKeywordForChinese)
			.def("ConvertToLua", (NoParamT)&LuaRunCo::ConvertToLua)
			.def("ConvertToLua", (ParamC_T)&LuaRunCo::ConvertToLua)
		];

	lua_vm_->DoString("	\
		function __CoroutineNameExist__( coroutine_name )	\
			return _G[coroutine_name] ~= nil;	\
		end	\
		\
		function __CoroutineReset__( coroutine_name )	\
			_G[coroutine_name] = nil;	\
		end	\
		\
		function __CoroutineResume__( coroutine_name, lua_run_co, ... )	\
			local co = __CoroutineObj__(coroutine_name);	\
			if co == nil then	\
				return false;	\
			end	\
			\
			if type(co) == \"thread\" and coroutine.status(co) ~= \"dead\" then	\
				lua_run_co:SetCoState(CoState.kState_Running);	\
			else	\
				lua_run_co:SetCoState(CoState.kState_Invalid);	\
			end	\
			\
			local is_succeed, msg_err = coroutine.resume(co, lua_run_co, ...);	\
			if is_succeed then	\
				if coroutine.status(co) ~= \"dead\" then	\
					lua_run_co:SetCoState(CoState.kState_Free);	\
				end	\
				--LuaLog(\"__CoroutineResume__恢复成功了\");	\n	\
				return true;	\
			else	\
				lua_run_co:SetCoState(CoState.kState_Invalid);	\
				LuaLogE(\"__CoroutineResume__恢复没有成功,错误信息是：\" .. msg_err);	\
				return false;	\
			end	\
			return false;	\
		end		\
			\
		function __CoroutineObj__( coroutine_name )	\
			local co = _G[coroutine_name];	\
			if co == nil then	\
				return;	\
			end	\
			if type(co) ~= \"thread\" then	\
				LuaLogE(\"coroutine name:\" .. coroutine_name .. \"的全局变量已经存在了，但却不是thread类型\");	\
				assert(false, \"coroutine出现了严重问题，务必解决\");	\
				return;	\
			end	\
			return co;	\
		end	\
		\
		function __CreateCoroutine__( coroutine_name, run_func_name )	\
			if coroutine_name == nil or run_func_name == nil then	\
				LuaLogE(\"__CreateCoroutine__参数为空\");	\
				return false;	\
			end	\
			local fn = _G[run_func_name];	\
			if fn == nil or type(fn) ~= \"function\" then	\
				LuaLogE(\"__CreateCoroutine__判断失败\");	\
				return false;	\
			end	\
			_G[coroutine_name] = coroutine.create(fn);	\
			return true;	\
		end	\
		\
		function __CoroutineRunScript__( lua_run_co )	\
			if lua_run_co == nil then	\
				LuaLogE(\"__CoroutineRunScript__的lua_run_co参数为空\");	\
				return;	\
			end	\
			\
			--repeat	\n	\
			lua_run_co:PreRun();	\
			\
			local lua_file_name = lua_run_co:GetScriptFileNameForRun();	\
			\
			if lua_run_co:IsEnableForChinese() then	\
				local lua_str = lua_run_co:ConvertToLua();	\
				assert(loadstring(lua_str))();	\
			else	\
				local fn = loadfile(GetScriptDirectory() .. lua_file_name);	\
				fn();	\
			end	\
			\
			lua_run_co:RunToEnd();	\
			--until not CoIsExecNextRun(lua_run_co)		\n	\
			\
			lua_run_co:SetCoState(CoState.kState_Invalid);	\
		end	\
		\
		function CoIsExecNextRun( lua_fun_co )	\
			coroutine.yield();	\
			return lua_fun_co:IsExecNextRun();	\
		end	\
		\
		function IsTerminateScriptRun()	\
			local lua_run_co = LuaScriptC_Instance();	\
			if lua_run_co == nil then	\
				LuaLogE(\"IsTerminateScriptRun之找不到lua_run_co\");	\
				return true;	\
			end	\
			lua_run_co:YieldOneFree();	\
			coroutine.yield();	\
			return lua_run_co:IsTerminateScriptRun();	\
		end	\
		\
		function IsTerminateScript()	\
			local lua_run_co = LuaScriptC_Instance();	\
			if lua_run_co == nil then	\
				LuaLogE(\"IsTerminateScriptRun之找不到lua_run_co\");	\
				return true;	\
			end	\
			return lua_run_co:IsTerminateScript();	\
		end");
		/*
		lua_vm_->DoString("function Delay( millisecond )	\
									local lua_run_co = LuaScriptC_Instance();	\
									if lua_run_co == nil then	\
									LuaLogE(\"Delay之找不到lua_run_co\");	\
									Sleep(millisecond);	\
									return;	\
									end	\
									lua_run_co:Delay(millisecond);	\
									--LuaLog(\"延迟的时间是：\" .. millisecond);	\n	\
									coroutine.yield();	\
									end	\
									");*/

		//加载一个中文脚本
		lua_vm_->DoString("	\
					function LoadScriptFile( file_name )	\
						local lua_run_co = LuaScriptC_Instance();	\
						if lua_run_co == nil then	\
						   LuaLogE(\"LoadScriptFile之找不到lua_run_co\");	\
						   return;	\
						end	\
						local script_str = lua_run_co:ConvertToLua(file_name);	\
						assert(loadstring(script_str))();	\
					end	\
				");
}

bool LuaRunCo::IsEnableForChinese() const
{
	return !replace_from_chinese_.empty();
}

bool LuaRunCo::RegKeywordForChinese( const char* str_word_src, const char* str_word_chinese )
{
	return replace_from_chinese_.AddMap_Src2Dst_A(str_word_src, str_word_chinese);
}

std::string LuaRunCo::ConvertToLua()
{
	return std::move(ConvertToLua(lua_file_name_.c_str()));
}

std::string LuaRunCo::ConvertToLua( const char* script_file_name )
{
	if ( !script_file_name )
	{
		assert(false);
		return "LuaLogE(\"LuaRunCo::ConvertToLua出错\");";
	}
	auto& lua_root = AS_Sys::GetMe().GetRoot();
	if (!lua_root)
	{
		assert(false);
		return kEmptyStr;
	}
	auto& script_dir = lua_root->FindPosterity(Cmn_AppBase::GetMe().GetScriptDirName());
	if (!script_dir)
	{
		assert(false);
		return kEmptyStr;
	}
	boost::filesystem::path script_path(script_file_name);
	if (!script_path.has_extension())
		script_path += ".lua";
	auto node = script_dir->Find(script_path);
	if (!node)
	{
		node = script_dir->FindPosterity(script_path);
		if (!node)
		{
			assert(false);
			return kEmptyStr;
		}
	}
	return replace_from_chinese_.Replace_A(node->GetString());
}

void LuaRunCo::Init( const char* co_name, const char* co_run_func_name )
{
	assert(lua_vm_);
	assert(co_name);
	assert(co_run_func_name);
	RegForLua();

	lua_State* lua_state = lua_vm_->lstate_;
	assert(!lua_vm_->LuaObjCallByDlt("__CoroutineNameExist__", true, co_name));
	if (!lua_vm_->LuaObjCallByDlt("__CreateCoroutine__", false, co_name, co_run_func_name))
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "调用__CreateCoroutine__失败";
	}
	else
	{
		SetCoState(kState_Free);
	}
}


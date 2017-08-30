#include "stdafx.h"
#include <Script/LuaScript.h>
#include <fstream>
#include <Common/CmnException.h>
#include <Common/EnsureCleanup.h>
#include <Script/lua_base.h>
#include <CmnMix/Cmn_LogOut.h>
#include <AssetSys/AS_Sys.h>
#include <AppBase/Cmn_App.h>

//////////////////////////////////////////////////////////////////////////
class _ChangeRunningScript_ : public CmnExceptionBase{
public:
	_ChangeRunningScript_(const std::string& cur_script_name, pt_dword dst_idx) : script_name_(cur_script_name){
		dst_idx_ = dst_idx;
	}

public:
	std::string		script_name_;
	pt_dword		dst_idx_;

	CMN_EXCEPTION_CLONE
};

class _RunWaitEventExcept_ : public CmnExceptionBase{
public:
	_RunWaitEventExcept_(const LuasScriptEchoC::stEvents* dst_event, const std::string* dst_event_name){
		assert(dst_event);
		assert(dst_event_name);
		dst_event_ = dst_event;
		dst_event_name_ = dst_event_name;
	}

public:
	const LuasScriptEchoC::stEvents*	dst_event_;
	const std::string*					dst_event_name_;

	CMN_EXCEPTION_CLONE
};
//////////////////////////////////////////////////////////////////////////
LuasScriptEchoC::LuasScriptEchoC(FuncObj& parent) : LuaScriptEchoBase(parent)
{
	running_ = false;
	script_raii_ = nullptr;
	struct stRegHelper{
		const char*		cn_;
		const char*		en_;
	};
	const stRegHelper key_words[] = {
		{ "如果", "if" },
		{ "那么", "then" },
		{ "结束", "end" },
		{ "否则", "else" },
		{ "否则如果", "elseif" },
		{ "循环", "while" },
		{ "局部变量", "local" },
		{ "返回", "return" },
		{ "空", "nil" },
		{ "真", "true" },
		{ "假", "false" },
		{ "不", "not" },
		{ "没有", "not" },
		{ "没", "not" },
		{ "执行", "do" },
		{ "且", "and" },
		{ "或", "or" },
		{ "转换成字符串", "tostring" },
		{ "定义函数", "function" },
		{ "重复", "repeat" },
		{ "直到", "until" },
		{ "中断", "break" },
	};
	for (auto& v : key_words)
	{
		VERIFY(RegKeywordForChinese(v.cn_, v.en_));
	}
}

BC_REG_CLS_DEFINE(LuasScriptEchoC){
	bufcall::class_<LuasScriptEchoC>("LuasScriptEchoC")
	.def("RunLuaScript", &LuasScriptEchoC::RunLuaScript)
	.def("TerminateScript", &LuasScriptEchoC::TerminateScript);
}

BC_REG_CLS_DEFINE(LuasScriptEchoS){
	bufcall::class_<LuasScriptEchoS>("LuasScriptEchoS")
	.def("OutputScriptInfo", &LuasScriptEchoS::OutputScriptInfo)
	.def("OperResult", &LuasScriptEchoS::OperResult);
}

static luabind::object LoadScriptFile_(const std::string& script_file_name)
{
	auto script = LuasScriptEchoC::GetInstance();
	if (!script)
	{
		assert(false);
		return luabind::object();
	}
	return script->LoadScriptFile(script_file_name, luabind::object(), false);
}

luabind::object LuasScriptEchoC::LoadScriptFile(const std::string& script_file_name, 
	const luabind::object& script_param, bool is_new_env)
{
	auto& lua_vm = LuaThrdData::GetMe();
	const auto& lua_str = ConvertToLua(script_file_name);
	const auto& script_fn = lua_vm.LoadBuffer(lua_str.c_str(), lua_str.size(), script_file_name.c_str());
	PreCallScriptFn(script_fn);
	if (!is_new_env)
		return LuaObjCall<luabind::object>(script_fn);
	LuaNewFEnv new_env(script_fn);
	if (script_param.is_valid())
		new_env.new_fenv_["脚本参数"] = script_param;
	return LuaObjCall<luabind::object>(new_env.fn_);
}

void LuasScriptEchoC::RegForLua(lua_State* l)
{
	static bool is_init = false;
	if (is_init)
		return;
	using namespace luabind;

	module(l)
		[
			def("LoadScriptFile", &LoadScriptFile_),
			def("OutputScriptInfo", &OutputScriptInfo_),
			def("AutoChooseRunScript", &AutoChooseRunScript),
			def("ChangeRunningScript", &ChangeRunningScript),
			def("RunWaitEvent", &RunWaitEvent),
			def("RunWaitEvent", &RunWaitEvent_),
			def("TriggerEvent", &TriggerEvent)
		];
}

bool LuasScriptEchoC::OutputScriptInfo( const std::string& script_info )
{
	if (script_info.empty())
	{
		assert(false);
		return false;
	}
	return ToCall(BC_MakeP("OutputScriptInfo")<<script_info);
}

bool LuasScriptEchoC::RunLuaScript(const std::string& lua_file_name, const std::string& lua_serialize)
{
	if (running_)
	{
		OperResult(kStopped);
		VERIFY(OutputScriptInfo("已经在运行"));
		return true;
	}
	if (timer_to_run_script_)
	{
		assert(false);
		return true;
	}
	VERIFY(OperResult(kBeginRun));
	timer_to_run_script_ = CreateIoTimer(200, true, [=](IoTimerPtr& timer){
		auto& io_s = GetIoService();
		const auto cur_exec_depth = io_s.GetExecDepth();
		auto script = LuasScriptEchoC::GetInstance();
		if (!script || !script->timer_to_run_script_)
		{
			OperResult(kStopped);
			//CMN_ASSERT(false);
			return;
		}
		if (1 == cur_exec_depth)
		{
			script->timer_to_run_script_->Cancel();
			script->timer_to_run_script_.reset();
			script->CatchRunScript(lua_file_name);
			return;
		}
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "运行脚本时，当前执行深度不为1，为：" << cur_exec_depth;
		if (!timer)
		{
			CMN_ASSERT(false);
			return;
		}
		timer->Continue();
	});
	return true;
}

bool LuasScriptEchoC::TerminateScript()
{
	if (timer_to_run_script_)
	{
		timer_to_run_script_->Cancel();
		timer_to_run_script_.reset();
	}
	if ( !running_ )
	{
		OperResult(kStopped);
		VERIFY(OutputScriptInfo("并未运行"));
		return false;
	}	
	GetIoService().post([=](){
		TerminateScriptImpl();
	});
	return true;
}

std::string LuasScriptEchoC::ConvertToLua( const std::string& script_file_name )
{
	if (script_file_name.empty())
	{
		assert(false);
		return "LuaLogE(\"LuasScriptEchoC::ConvertToLua出错\");";
	}
	auto& script_dir = Cmn_AppBase::GetAppBase().GetScriptDir();
	if (!script_dir)
	{
		assert(false);
		return kEmptyStr;
	}
	auto node = LStateWrapperAS::FindAS_LuaFile(script_dir, script_file_name.c_str());
	if (!node)
	{
		assert(false);
		return kEmptyStr;
	}
	return replace_from_chinese_.Replace_A(node->GetString());
}

bool LuasScriptEchoC::RegKeywordForChinese( const char* str_word_src, const char* str_word_chinese )
{
	return replace_from_chinese_.AddMap_Src2Dst_A(str_word_src, str_word_chinese);
}

void LuasScriptEchoC::CatchRunScript( const std::string& lua_file_name )
{
	using namespace luabind;
	if (running_)
	{
		assert(false);
		return;
	}
	auto lstate = LuaThrdData::GetMe().lua();
	assert(lstate);
	auto top_index_old = ::lua_gettop(lstate);
	//assert(0 == top_index_old);
#ifdef _DEBUG
	auto old_lua_use_kbytes = lua_gc(lstate, LUA_GCCOUNT, 0);
#endif
	BOOST_SCOPE_EXIT_ALL(=){
		OperResult(kStopped);
		assert(running_);
		running_ = false;
		//垃圾回收
		lua_gc(lstate, LUA_GCCOLLECT, 0);

#ifdef _DEBUG
		auto new_lua_use_kbytes = lua_gc(lstate, LUA_GCCOUNT, 0);
		if ( new_lua_use_kbytes > old_lua_use_kbytes + 1 )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "运行脚本结束后较未运行占用了更多的内存，运行脚本前：" << old_lua_use_kbytes << " 脚本结束后：" << new_lua_use_kbytes;
		}
#endif

		//恢复堆栈
		::lua_settop(lstate, top_index_old);
	};
	
	const auto& the_msg = CmnExceptionBase::DefCatchCall([=, &lua_file_name](){
		running_ = true;
		RunScriptImpl(lua_file_name);
		return kEmptyStr;
	});
	if (!the_msg.empty())
		VERIFY(OutputScriptInfo(the_msg));
}

void LuasScriptEchoC::TerminateScriptImpl()
{
	if (!running_)
	{
		OperResult(kStopped);
		LOG_O(Log_debug) << "准备停止脚本时，发现脚本已经停止了";
		return;
	}
	throw CmnTerminateScriptException();
}

void LuasScriptEchoC::RunScriptImpl(const std::string& lua_file_name)
{
	using namespace luabind;
	if (!script_raii_)
	{
		script_raii_ = CreateRunRAII();
		if (!script_raii_)
		{
			assert(false);
			return;
		}
	}
	if (!script_raii_->PreRun())
		return;
	BOOST_SCOPE_EXIT_ALL(this){
		script_raii_->AfterRun();
	};
	try{
		auto console_cfg = GetConsoleCfg();
		LoadScriptFile(lua_file_name, console_cfg, true);
	} DO_CATCH_THE_EXCEPTION(const CmnTerminateScriptException&, {
		return;
	})
	GetIoService().post([this](){
		VERIFY(OutputScriptInfo("正常结束"));
	});
}

bool LuasScriptEchoC::IsRunning()
{
	auto script_c = LuasScriptEchoC::GetInstance();
	if ( !script_c )
		return false;
	return script_c->running_;
}

LuaScriptEchoBase::LuaScriptEchoBase(FuncObj& parent) : BufCall_SocketExcept(&parent, kFuncObjTemplateId_LuaScript)
{
}

LuasScriptEchoS::LuasScriptEchoS(FuncObj& parent) : LuaScriptEchoBase(parent)
{

}

bool LuasScriptEchoS::RunLuaScript(const std::string& lua_file_name, const std::string& lua_serialize)
{
	if (lua_file_name.empty())
	{
		assert(false);
		return false;
	}
	//LOG_O(Log_trace) << "RunLuaScript，lua_serialize size:" << lua_serialize.size();
	return ToCall(BC_MakeP("RunLuaScript") << lua_file_name << lua_serialize);
}

bool LuasScriptEchoS::TerminateScript()
{
	return ToCall(BC_MakeP("TerminateScript"));
}

bool LuasScriptEchoS::OperResult(enOperResult result)
{
	assert(false);
	return false;
}

ScriptRunRAII::~ScriptRunRAII()
{

}

bool ScriptRunRAII::PreRun()
{
	return true;
}

void ScriptRunRAII::AfterRun()
{

}

bool LuasScriptEchoC::OperResult(enOperResult result)
{
	if (kBeginRun == result)
		return OutputScriptInfo("开始执行");
	else if (kStopped == result)
		return OutputScriptInfo("已停止");
	assert(false);
	return false;
}

ScriptRunRAII* LuasScriptEchoC::CreateRunRAII()
{
	return new ScriptRunRAII;
}

void LuasScriptEchoC::PreCallScriptFn(const luabind::object& lua_file_func)
{

}

bool LuasScriptEchoC::OutputScriptInfo_(const std::string& script_info)
{
	auto ptr = LuasScriptEchoC::GetInstance();
	if (!ptr)
	{
		assert(false);
		return false;
	}
	return ptr->OutputScriptInfo(script_info);
}

luabind::object LuasScriptEchoC::AutoChooseRunScript(const CSH_HandlerPtr& scene_handler, const luabind::object& fn_get_script_name)
{
	using namespace luabind;
	if (!fn_get_script_name.is_valid() || LUA_TFUNCTION != type(fn_get_script_name))
	{
		LOG_O(Log_error) << "自动选择运行脚本之第2个参数必须是函数类型";
		assert(false);
		return object();
	}
	if (scene_handler && scene_handler->InScene())
	{
		if (!scene_handler->Handle())
		{
			LOG_O(Log_error) << "自动选择运行脚本之场景处理不成功";
			assert(false);
			return object();
		}
	}
	auto cur_script_name = LuaObjCall<std::string>(fn_get_script_name);
	if (cur_script_name.empty())
	{
		LOG_O(Log_error) << "自动选择运行脚本之函数必须返回当前要运行的脚本名";
		assert(false);
		return object();
	}
	auto the_script = LuasScriptEchoC::GetInstance();
	if (!the_script)
	{
		assert(false);
		return object();
	}
	the_script->level0_scripts_.push_back({ cur_script_name, fn_get_script_name });
	BOOST_SCOPE_EXIT_ALL(&){
		auto script = LuasScriptEchoC::GetInstance();
		if (!script)
		{
			assert(false);
			return;
		}
		if (script->level0_scripts_.empty())
		{
			assert(false);
			return;
		}
		script->level0_scripts_.pop_back();
	};
	LOG_O(Log_info) << "开始运行顶层脚本：" << cur_script_name;
	while (true)
	{
		try{
			return LoadScriptFile_(cur_script_name);
		} DO_CATCH_THE_EXCEPTION(const _ChangeRunningScript_& e, {
			if (e.dst_idx_ + 1 != the_script->level0_scripts_.size())
				throw;
			LOG_O(Log_info) << "切换顶层脚本，由：" << cur_script_name << "，切换成：" << e.script_name_;
			assert(!the_script->level0_scripts_.empty());
			cur_script_name = e.script_name_;
			the_script->level0_scripts_.back().level0_script_name_ = cur_script_name;
		})
	}
	assert(false);
	return object();
}

void LuasScriptEchoC::ChangeRunningScript(int top_n, bool is_rerun)
{
	auto script = LuasScriptEchoC::GetInstance();
	if (!script) return;
	if (script->level0_scripts_.empty())
		return;
	auto& script_info = script->level0_scripts_.back();
	auto cur_script_name = LuaObjCall<std::string>(script_info.fn_get_script_name_);
	if (cur_script_name.empty())
		return;
	if (!is_rerun && script_info.level0_script_name_ == cur_script_name)
		return;
	pt_dword the_idx = 0;
	for (auto& v : script->level0_scripts_)
	{
		if (v.level0_script_name_ == cur_script_name)
		{
			throw _ChangeRunningScript_(cur_script_name, the_idx);
		}
		++the_idx;
	}
	assert(the_idx == script->level0_scripts_.size());
	if (top_n < 0)
		top_n += (int)the_idx;
	if (top_n < 0)
		top_n = 0;
	if (top_n >= (int)the_idx)
		top_n = the_idx - 1;
	assert(top_n >= 0);
	throw _ChangeRunningScript_(cur_script_name, top_n);
}

luabind::object LuasScriptEchoC::RunWaitEvent(const luabind::object& fn_run, const luabind::object& fn_event_handler,
	const std::string& event_name)
{
	if (event_name.empty())
	{
		LOG_O(Log_error) << "事件名不能为空";
		assert(false);
		return luabind::object();
	}
	stEvents cur_events;
	cur_events.event_names_.push_back(event_name);
	return RunWaitEventImpl_(fn_run, fn_event_handler, cur_events);
}

luabind::object LuasScriptEchoC::RunWaitEvent_(const luabind::object& fn_run, const luabind::object& fn_event_handler,
	const luabind::object& event_names)
{
	using namespace luabind;
	if (!event_names.is_valid() || LUA_TTABLE != type(event_names))
	{
		LOG_O(Log_error) << "事件参数要么是字符串类型，要么是表类型，表里的元素是事件名";
		assert(false);
		return luabind::object();
	}
	stEvents cur_events;
	for (auto& v : event_names)
	{
		const auto& event_name = LuaObjCast(v, kEmptyStr);
		if (event_name.empty())
		{
			LOG_O(Log_error) << "事件参数的表里的元素必须是事件名，事件名不能为空，且必须是字符串类型";
			assert(false);
			return luabind::object();
		}
		cur_events.event_names_.push_back(event_name);
	}
	if (cur_events.event_names_.empty())
	{
		LOG_O(Log_error) << "事件参数的表难道是个空表？";
		assert(false);
		return luabind::object();
	}
	return RunWaitEventImpl_(fn_run, fn_event_handler, cur_events);
}

luabind::object LuasScriptEchoC::RunWaitEventImpl_(const luabind::object& fn_run, const luabind::object& fn_event_handler, 
	const stEvents& events)
{
	using namespace luabind;
	if (!fn_run.is_valid() || LUA_TFUNCTION != type(fn_run))
	{
		LOG_O(Log_error) << "执行并等待事件之第一个参数是做事函数，必须是函数类型";
		assert(false);
		return luabind::object();
	}
	if (!fn_event_handler.is_valid() || LUA_TFUNCTION != type(fn_event_handler))
	{
		LOG_O(Log_error) << "执行并等待事件之第二个参数是事件处理函数，必须是函数类型";
		assert(false);
		return luabind::object();
	}
	assert(!events.event_names_.empty());
	const std::string* trigger_event = nullptr;
	while (true)
	{
		if (trigger_event)
		{
			auto the_event_name = trigger_event;
			trigger_event = nullptr;
			LOG_O(Log_info) << "事件被触发：" << *the_event_name;
			auto handler_res = LuaObjCall<luabind::object>(fn_event_handler, *the_event_name);
			if (LuaObjCast(handler_res, false) != true)
			{
				LOG_O(Log_trace) << "事件处理函数未返回true，表示退出 执行并等待事件，并把事件处理函数的结果返回之";
				return handler_res;
			}
		}
		try{
			auto the_script = LuasScriptEchoC::GetInstance();
			if (!the_script)
			{
				assert(false);
				return object();
			}
			the_script->cur_events_.push_back(&events);
			BOOST_SCOPE_EXIT_ALL(&){
				auto script = LuasScriptEchoC::GetInstance();
				if (!script)
				{
					assert(false);
					return;
				}
				if (script->cur_events_.empty())
				{
					assert(false);
					return;
				}
				script->cur_events_.pop_back();
			};
			return LuaObjCall<luabind::object>(fn_run);
		} DO_CATCH_THE_EXCEPTION(const _RunWaitEventExcept_& e, {
			if (e.dst_event_ != &events)
				throw;
			trigger_event = e.dst_event_name_;
			assert(trigger_event);
		})
	}
	assert(false);
	return object();
}

void LuasScriptEchoC::TriggerEvent(const std::string& event_name)
{
	if (event_name.empty())
	{
		assert(false);
		return;
	}
	auto the_script = LuasScriptEchoC::GetInstance();
	if (!the_script)
	{
		assert(false);
		return;
	}
	for (auto it = the_script->cur_events_.crbegin(), it_end = the_script->cur_events_.crend(); it != it_end; ++it)
	{
		auto the_event = *it;
		assert(the_event);
		for (auto& e_name : the_event->event_names_)
		{
			if (e_name == event_name)
			{
				throw _RunWaitEventExcept_(the_event, &e_name);
			}
		}
	}
}

LuasScriptEchoC::~LuasScriptEchoC()
{
	if (script_raii_)
		delete script_raii_;
}

luabind::object LuasScriptEchoC::GetConsoleCfg() const
{
	return luabind::object();
}

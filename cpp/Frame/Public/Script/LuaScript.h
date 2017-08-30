#pragma once
/*
@author zhp
@date 2014/8/31 3:30
@JsonCall->BufCall:  2016/12/26 21:26
@purpose for chinese lua script
*/
#include <Net/Bufcall_Socket.h>
#include <Common/UsefulClass.h>
#include <Net/IoTimer.h>
#include "../DependSelfLib.h"
#include <CmnMix/Cmn_RegexMore.h>
#include <WgFacility/CmnSceneHandler.h>
#include <vector>
#include <luabind/luabind.hpp>


class LuaScriptEchoBase : public BufCall_SocketExcept{
protected:
	LuaScriptEchoBase(FuncObj& parent);
	enum enOperResult{
		kBeginRun,	//开始运行
		kStopped,	//已停止
	};

public:
	//执行某个脚本
	virtual bool RunLuaScript(const std::string& lua_file_name, const std::string& lua_serialize) = 0;
	//中止脚本
	virtual bool TerminateScript() = 0;
	//输出脚本信息
	virtual bool OutputScriptInfo( const std::string& script_info ) = 0;
	//操作结果
	virtual bool OperResult(enOperResult result) = 0;

private:
	BC_REG_CLS_DECLARE(LuaScriptEchoBase);
};

class ScriptRunRAII{
public:
	virtual ~ScriptRunRAII();
	virtual bool PreRun();
	virtual void AfterRun();
};

class LuasScriptEchoC : public LuaScriptEchoBase, public Singleton<LuasScriptEchoC, Singleton_Instance>{
	struct stLevel0ScriptInfo{
		std::string			level0_script_name_;
		luabind::object		fn_get_script_name_;
	};
	typedef std::vector<stLevel0ScriptInfo> Level0Scripts;

public:
	struct stEvents{
		std::vector<std::string>		event_names_;
	};
	typedef std::vector<const stEvents*> EventsCont;

public:
	LuasScriptEchoC(FuncObj& parent);
	~LuasScriptEchoC();
	static void RegForLua(lua_State* l);
	static bool IsRunning();

public:
	bool TerminateScript() override;
	bool OutputScriptInfo(const std::string& script_info) override;	
	//转换到lua
	std::string ConvertToLua(const std::string& script_file_name);
	luabind::object LoadScriptFile(const std::string& script_file_name, const luabind::object& script_param, bool is_new_env);

	//智能换脚本相关
public:
	//自动选择运行脚本
	static luabind::object AutoChooseRunScript(const CSH_HandlerPtr& scene_handler, const luabind::object& fn_get_script_name);
	//改变运行的脚本。如果top_n=0，则弹出到最顶层的脚本;top_n=1，则弹出到最顶层的下一层脚本，依此类推;top_n=-1，则弹出当前运行的顶层脚本，依此类推。
	static void ChangeRunningScript(int top_n = 0, bool is_rerun = false);

	//事件相关
public:
	//执行并等待事件
	//fn_event_handler或是返回真，表示事件处理成功，并会再一次执行fn_run。否则会退出 执行并等待事件，并把事件处理函数的结果返回之。
	static luabind::object RunWaitEvent(const luabind::object& fn_run, const luabind::object& fn_event_handler, const std::string& event_name);
	//event_names是个表，表示一系列的事件
	static luabind::object RunWaitEvent_(const luabind::object& fn_run, const luabind::object& fn_event_handler, const luabind::object& event_names);
	//触发事件
	static void TriggerEvent(const std::string& event_name);

private:
	static luabind::object RunWaitEventImpl_(const luabind::object& fn_run, const luabind::object& fn_event_handler, const stEvents& events);

protected:
	bool RunLuaScript(const std::string& lua_file_name, const std::string& lua_serialize) override;
	bool OperResult(enOperResult result) override;
	virtual void RunScriptImpl(const std::string& lua_file_name);
	virtual ScriptRunRAII* CreateRunRAII();
	virtual void PreCallScriptFn(const luabind::object& lua_file_func);
	virtual luabind::object GetConsoleCfg() const;

private:
	void CatchRunScript(const std::string& lua_file_name);
	void TerminateScriptImpl();
	//注册关键字
	bool RegKeywordForChinese(const char* str_word_src, const char* str_word_chinese);
	static bool OutputScriptInfo_(const std::string& script_info);

private:
	bool				running_;
	RegexWordReplace	replace_from_chinese_;		//从中文转换到lua
	IoTimerPtr			timer_to_run_script_;
	Level0Scripts		level0_scripts_;			//当前运行的顶级脚本名
	EventsCont			cur_events_;				//当前事件

protected:
	ScriptRunRAII*		script_raii_;

	BC_REG_CLS_DECLARE(LuasScriptEchoC);
};

class LuasScriptEchoS : public LuaScriptEchoBase{
public:
	LuasScriptEchoS(FuncObj& parent);
	bool RunLuaScript(const std::string& lua_file_name, const std::string& lua_serialize = kEmptyStr) override;
	bool TerminateScript() override;

protected:
	bool OperResult(enOperResult result) override;

	BC_REG_CLS_DECLARE(LuasScriptEchoS);
};

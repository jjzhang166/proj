#pragma once
/*
@author zhp
@date 2016/4/10 20:48
@purpose lua协程，现已不用了。把旧代码单独移到该文件中。
*/
#include <boost/enable_shared_from_this.hpp>
#include <CmnMix/Cmn_LuaOper.h>
#include <Script/lua_base.h>
#include <CmnMix/Cmn_RegexMore.h>

class ILuaCoroutine : public boost::enable_shared_from_this<ILuaCoroutine>{
public:
	enum CoState{
		kState_Invalid,
		kState_Free,
		kState_Running,
	};

public:
	ILuaCoroutine( LuaVM* lua_vm );
	typedef boost::shared_ptr<ILuaCoroutine> PtrType;
	virtual ~ILuaCoroutine();
	//resume thread
	virtual void ResumeT() = 0;
	//terminate thread
	virtual void TerminateT() = 0;
	//是否有效
	virtual bool IsValid() const;
	//是否空闲状态
	virtual bool IsFree() const;
	//是否在运行状态
	virtual bool IsRunning() const;
	//重新从头开始执行
	virtual void Restart() = 0;
	//延迟
	virtual bool Delay( size_t millisecond );
	//让出一次空闲时间(只是让post一次resume消息而已)
	virtual bool YieldOneFree();
	//是否中止脚本中的循环
	virtual bool IsTerminateScriptRun();
	//是否中止脚本
	bool IsTerminateScript() const;
	//强制中止run
	virtual void ForceTerminateScriptRun( bool is_terminate );
	//强制中止脚本
	void ForceTerminateScript( bool is_terminate );
	//执行某个脚本
	bool RunLuaScript( const char* lua_file_name, bool is_force );
	//set coroutine state
	void SetCoState( CoState co_state );
	//get the run script file name
	const char* GetScriptFileNameForRun();
	//控制台执行关闭脚本
	void TerminateScriptByConsole();

protected:
	//是否需要再次执行run
	virtual bool IsExecNextRun();
	//run结束了
	virtual void RunToEnd();
	//再次执行一个run之前的动作
	virtual void PreRun();

private:
	//register for lua
	void RegForLua();

protected:
	std::string				lua_file_name_;
	LuaVM*const				lua_vm_;

private:
	bool					force_terminate_script_run_;
	bool					force_terminate_script_;
	CoState					state_;
};

//lua coroutine
class LuaCoroutine : public ILuaCoroutine{
protected:
	LuaCoroutine( LuaVM* lua_vm );

public:
	~LuaCoroutine();
	//resume thread
	virtual void ResumeT();
	//yield thread
	//coroutine.yield与coroutine.resume的调用层之间不能存在C++的函数调用
	virtual void YieldT();
	//terminate thread
	virtual void TerminateT();
	//是否有效
	virtual bool IsValid() const;
	//重新从头开始执行
	virtual void Restart();
	//延迟
	virtual bool Delay( size_t millisecond );
	//让出一次空闲时间(只是让post一次resume消息而已)
	virtual bool YieldOneFree();
	//是否中止run
	virtual bool IsTerminateScriptRun();

protected:
	//run of the new thread
	virtual void Run() = 0;
	virtual bool IsExecNextRun();

private:
	//register for lua
	void RegForLua();
	//main function of the new thread
	static void MainFunc( LuaCoroutine* lua_co );

protected:
	lua_State*				lua_state_new_;
};

//lua coroutine script
class LuaCoScript : public LuaCoroutine{
protected:
	LuaCoScript( LuaVM* lua_vm );

protected:
	virtual void Run();

private:
	void RegForLua();
};

//lua执行coroutine
class LuaRunCo : public ILuaCoroutine{
public:
	LuaRunCo( LuaVM* lua_vm, const char* co_name, const char* co_run_func_name );
	LuaRunCo( LuaVM* lua_vm );

	virtual void ResumeT();
	virtual void TerminateT();
	virtual void Restart();

public:
	//是否启用中文脚本环境
	bool IsEnableForChinese() const;
	//注册关键字
	bool RegKeywordForChinese( const char* str_word_src, const char* str_word_chinese );
	//转换到lua
	std::string ConvertToLua();
	std::string ConvertToLua( const char* script_file_name );

private:
	void RegForLua();
	void Init( const char* co_name, const char* co_run_func_name );

private:
	std::string				co_name_;
	std::string				co_run_func_name_;
	RegexWordReplace		replace_from_chinese_;			//从中文转换到lua
};

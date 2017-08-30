#pragma once
/*
@author zhp
@date 2016/4/10 19:55
@purpose for exceptions
*/
#include <stdexcept>
#include <luabind/error.hpp>
#include <luabind/object.hpp>
#include <boost/current_function.hpp>
#include "../DependSelfLib.h"
#include <boost/thread/tss.hpp>

#define DO_CATCH_ALL_EXCEPTION(__b__) catch ( ... ){		\
	try{	\
		{	\
			int int_tmp = 2;	\
			int* ptr_tmp = &int_tmp;	\
			ptr_tmp += 0;	\
			*ptr_tmp = 3;	\
		}	\
		__b__	\
	}	\
	catch ( ... ){ \
		throw;	\
	}	\
}	\
/**/
#define DO_CATCH_THE_EXCEPTION(__a__, __b__) catch ( __a__ ){		\
	try{	\
		{	\
			int int_tmp = 2;	\
			int* ptr_tmp = &int_tmp;	\
			ptr_tmp += 0;	\
			*ptr_tmp = 3;	\
		}	\
		__b__	\
	}	\
	catch ( ... ){	\
		throw;	\
	}	\
}	\
/**/
#define CMN_EXCEPTION_CLONE	\
private:	\
	CmnExceptionBase* Clone() const override{	\
		typedef std::remove_cv<std::remove_pointer<decltype(this)>::type>::type SelfType;	\
		return new SelfType(*this);	\
	}	\
	void Rethrow() const override{	\
		typedef std::remove_cv<std::remove_pointer<decltype(this)>::type>::type SelfType;	\
		throw SelfType(*this);	\
	}
/**/
#define DO_EXCEPTION_TRY	try{ (void*)0	\
/**/
#define DO_CATCH_ALL_ASSERT_CODE(code)	\
	} DO_CATCH_ALL_EXCEPTION({ assert(false); code })	\
/**/
#define CMN_ASSERT_(_Expression, prompt_str) (void)( (!!(_Expression)) || (assert(CmnExceptionBase::IsIgnoreException_(#_Expression, BOOST_CURRENT_FUNCTION, __FILE__, __LINE__, prompt_str)), 0) )	\
/**/
#define CMN_ASSERT(_Expression) CMN_ASSERT_(_Expression, nullptr)	\
/**/
#define DO_CATCH_ALL_MSG_BOX_CODE( code )	\
	} DO_CATCH_ALL_EXCEPTION({ CMN_ASSERT_(false, "DO_CATCH_ALL_MSG_BOX_CODE捕获到了异常"); code })	\
/**/
//////////////////////////////////////////////////////////////////////////
//************************pause id****************************************
#define DEFINE_THE_PAUSE_ID(pause_id, value)	\
	const CmnPauseLogicException::PauseIdentity pause_id = { (int)(&value) }	\
/**/
#define DEFINE_DEFAULT_PAUSE_ID(value) DEFINE_THE_PAUSE_ID(pause_id, value)
/**/
#define DO_CATCH_THE_PAUSE_ID( pause_id, code )	\
	DO_CATCH_THE_EXCEPTION( const CmnPauseLogicException& e, {	\
		if ( !e.IsIdentityEqual(pause_id) )	\
			throw;	\
		code	\
	}) \
/**/
#define DO_CATCH_DEFAULT_PAUSE_ID( code ) DO_CATCH_THE_PAUSE_ID(pause_id, code)
/**/
#define DO_CATCH_THE_PAUSE_ID_DO_NOTHING( pause_id )	\
	DO_CATCH_THE_EXCEPTION( const CmnPauseLogicException& e, {	\
		if ( !e.IsIdentityEqual(pause_id) )	\
			throw;	\
	})	\
/**/
#define DO_CATCH_DEFAULT_PAUSE_ID_DO_NOTHING() DO_CATCH_THE_PAUSE_ID_DO_NOTHING(pause_id)
/**/
//////////////////////////////////////////////////////////////////////////
/**/
class CmnExceptionBase : public std::runtime_error{
public:
	CmnExceptionBase();
	explicit CmnExceptionBase(const std::string& msg);
	explicit CmnExceptionBase(const char* msg);
	CmnExceptionBase(const CmnExceptionBase& rhs);
	virtual ~CmnExceptionBase();
	virtual CmnExceptionBase* Clone() const = 0;
	virtual void Rethrow() const = 0;

	//延迟异常
public:
	static const CmnExceptionBase* GetDelayException();
	//捕捉下异常，看这种异常是自己抛出的，还是其他错误
	static void CatchDelayException();
	//捕捉异常的时候，最好这样调用的方式去捕捉：CmnExceptionBase::HandleDelayException();
	//去看下使用例子便知道了。调用LUA函数的时候，最好用LuaObjCall
	static void HandleDelayException();
	static void SetDelayException(const CmnExceptionBase& cur_exception);
	static void SetDelayException(const CmnExceptionBase* cur_exception);
	//安全执行之跳过异常
	static luabind::object SafeRun_SkipExcept(const luabind::object& lua_func, const luabind::object& deault_res);

	//another
public:
	static bool IsIgnoreException_(const char* expression_str, const char* func_name, const char* file_name, unsigned int file_line, const char* another_prompt);
	//默认捕获调用
	static std::string DefCatchCall(const std::function<std::string()>& func);

private:
	static boost::thread_specific_ptr<CmnExceptionBase>	delay_exception_;
};

class CmnTerminateScriptException : public CmnExceptionBase{
public:
	explicit CmnTerminateScriptException();
	explicit CmnTerminateScriptException(const char* msg);
	explicit CmnTerminateScriptException(const std::string& msg);
	CMN_EXCEPTION_CLONE
};

class CmnSafeRun_SkipExcept : public CmnExceptionBase{
public:
	explicit CmnSafeRun_SkipExcept(const std::string& msg);
	explicit CmnSafeRun_SkipExcept(const char* msg);
	CmnSafeRun_SkipExcept();
	CMN_EXCEPTION_CLONE
};

//中止逻辑的异常，用于中止逻辑
class CmnPauseLogicException : public CmnExceptionBase{
public:
	//问题：不知为何，对于同一个全局变量，其地址的数值有可能不一样，所以在使用PauseIdentity pause_id; pause_id.addr_ = &value;时，最好只有这么一段代码，然后用复制pause_id的方式。或者对于pause_id，引用同一个变量。
	//解答：value不能是全局常量，因为value所在的文件被包含到不同的源文件中时，可能存在多份实例，所以&value得到的数值就有可能不同。
	union PauseIdentity{
		int				id_;
		const void*		addr_;
	};
	static_assert(sizeof(PauseIdentity) == sizeof(int), "");

public:
	CmnPauseLogicException(PauseIdentity id, const char* msg);
	CmnPauseLogicException(PauseIdentity id);
	bool IsIdentityEqual(PauseIdentity id) const;
	CMN_EXCEPTION_CLONE

public:
	PauseIdentity			id_;
};

class CmnGeneralRunException : public CmnExceptionBase{
public:
	explicit CmnGeneralRunException(const std::string& msg);
	explicit CmnGeneralRunException(const char* msg);
	CMN_EXCEPTION_CLONE
};

class CmnLuaException : public CmnExceptionBase{
public:
	explicit CmnLuaException(const std::string& msg);
	CMN_EXCEPTION_CLONE
};

class CmnExitIoThreadException : public CmnExceptionBase{
public:
	CmnExitIoThreadException();
	explicit CmnExitIoThreadException(const std::string& msg);
	explicit CmnExitIoThreadException(const char* msg);
	CMN_EXCEPTION_CLONE
};

class CmnReDoIoRunException : public CmnExceptionBase{
public:
	CmnReDoIoRunException();
	explicit CmnReDoIoRunException(const std::string& msg);
	explicit CmnReDoIoRunException(const char* msg);
	CMN_EXCEPTION_CLONE
};

class CmnLetIoThreadDoErr : public CmnExceptionBase{
public:
	CmnLetIoThreadDoErr();
	explicit CmnLetIoThreadDoErr(const std::string& msg);
	explicit CmnLetIoThreadDoErr(const char* msg);
	CMN_EXCEPTION_CLONE
};

class CmnCastException : public CmnExceptionBase{
public:
	CmnCastException(const luabind::type_id& src, const luabind::type_id& to);
	CmnCastException(const luabind::type_id& src, const std::string& middle_str_info, const luabind::type_id& to);
	CmnCastException(const luabind::type_id& src, const char* middle_str_info, const luabind::type_id& to);
	CMN_EXCEPTION_CLONE
};

class CmnCloseProcessException : public CmnExceptionBase{
public:
	CmnCloseProcessException();
	explicit CmnCloseProcessException(const std::string& msg);
	explicit CmnCloseProcessException(const char* msg);
	CMN_EXCEPTION_CLONE
};

class CmnTerminateProcessException : public CmnExceptionBase{
public:
	CmnTerminateProcessException();
	explicit CmnTerminateProcessException(const std::string& msg);
	explicit CmnTerminateProcessException(const char* msg);
	CMN_EXCEPTION_CLONE
};

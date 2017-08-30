#pragma once
/*
@author zhp
@date 2014-1-09
@purpose log support
*/
//#pragma warning(push)

#pragma warning(disable: 4819)
#include <boost/log/core.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/preprocessor/comparison/greater_equal.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <config/targetver.h>
#include "../DependSelfLib.h"
/*
enum{
	Log_trace = ::boost::log::trivial::severity_level::trace,
	Log_debug,
	Log_info,
	Log_warning,
	Log_error,
	Log_fatal
};*/
#define Log_trace 0
static_assert(Log_trace == ::boost::log::trivial::severity_level::trace, "");
#define Log_debug 1
#define Log_info 2
#define Log_warning 3
#define Log_error 4
#define Log_fatal 5

#ifndef _DEBUG
	#ifndef _LOG_OUTPUT_EMPTY
	 #ifndef _LOG_OUT_BY_LEVEL		/*__RELEASE_LOG_INFO_*/
	 #define _LOG_OUTPUT_EMPTY
	 #endif
	#endif
#endif
//log out macro
#ifdef _LOG_OUTPUT_EMPTY	/**/
 #define LOG_O(log_level) LogEmpty()
 #define LOG_ENTER_FUNC
 #define LOG_O_EMPTY_MSG(log_level)
 #define LOG_O_EMPTY_MSG_FAST(log_level)
 #define LOG_STM(stm_var, lvl) LogEmpty stm_var()
#else
 #ifdef _LOG_OUT_BY_LEVEL
  #define LOG_ENTER_FUNC
 #else
  #define LOG_ENTER_FUNC BOOST_LOG_FUNCTION()
  #define _LOG_OUT_BY_LEVEL	Log_trace	/**/
 #endif
 //#define LOG_O(log_level) BOOST_LOG_SEV(GetLogger(), (boost::log::trivial::severity_level)log_level)
 #define LOG_O(log_level) BOOST_PP_IF(BOOST_PP_GREATER_EQUAL(log_level, _LOG_OUT_BY_LEVEL), BOOST_LOG_SEV(GetLogger(), (::boost::log::trivial::severity_level)log_level) /*<< std::dec*/, LogEmpty())	/**/
 #define LOG_O_EMPTY_MSG(log_level) LOG_O(log_level) << ""
 #define LOG_O_EMPTY_MSG_FAST(log_level) LOG_ENTER_FUNC; LOG_O(log_level) << ""

//by zhp, 2016/12/30 22:20
 #define LOG_STM_INTERNAL_(rec_var, rec_pump, stm_var, params_seq)	\
 auto rec_var = GetLogger().open_record((BOOST_PP_SEQ_ENUM(params_seq)));	\
 assert(!!rec_var);	\
 const auto& rec_pump = ::boost::log::aux::make_record_pump(GetLogger(), rec_var);	\
 auto& stm_var = rec_pump.stream()
 /**/
 #define LOG_STM_(stm_var, lvl)	\
 	LOG_STM_INTERNAL_(BOOST_LOG_UNIQUE_IDENTIFIER_NAME(_boost_log_record_),	\
 		BOOST_LOG_UNIQUE_IDENTIFIER_NAME(_boost_log_pump_),	\
 		stm_var, (::boost::log::keywords::severity = (::boost::log::trivial::severity_level)(lvl)))
 /**/
 #define LOG_STM(stm_var, lvl)	LOG_STM_(stm_var, lvl)
 /**/
#endif

#define LOG_HEAD_TAG "flog"

typedef ::boost::log::sources::severity_logger<::boost::log::trivial::severity_level> BoostLogger;

//得到logger
BoostLogger& GetLogger();

class LogSink{
public:
	typedef boost::shared_ptr<boost::log::sinks::sink> LogSinkPtrT;

public:
	virtual ~LogSink();
	const LogSinkPtrT& GetSinkPtr() const;
	void swap( LogSink& rhs );
	void ClearSinkPtr();

public:
	static bool AddLogSink( const LogSink& initor );
	//关闭所有log输出
	static void CloseAllLogOutput();

protected:
	//init log sink
	virtual bool InitSink() = 0;

protected:
	LogSinkPtrT			sink_ptr_;
};

//log to xml
class LogToXml : public LogSink{
	LogToXml();
	friend class LogToXmlHelper;
	typedef boost::log::sinks::text_file_backend XmlSinkBackend;
	// Create a text file sink
	typedef boost::shared_ptr<boost::log::sinks::synchronous_sink<XmlSinkBackend> > XmlSinkT;

public:
	//生成LOG FilePath
	bool GenLogFilePath(const char* relative_path_name);
	//设置文件前缀名
	bool SetPreFileName( const std::string& pre_str );

private:
	bool InitSink() override;
	//更新到临时文件中，已被废弃
	static void FlushToTempFile();

private:
	bool RefreshFilePath();
	bool RefreshFileNamePattern();
	static std::string GetDefFilePattern();

private:
	std::string				log_path_;
	std::string				log_file_pattern_;
	XmlSinkT				xml_sink_;
};
class LogToXmlHelper{
	//设置某模块目录+relative_path_name的目录为log的path
public:
#ifdef _LOG_OUTPUT_EMPTY
	inline LogToXmlHelper(const char* relative_path_name){}
	inline void ClearHolder(){}
	inline bool SetPreFileName( const std::string& pre_str ){ return true; }
#else
	//module_name==0-->表示当前exe所在目录
	LogToXmlHelper(const char* relative_path_name);
	void ClearHolder();
	bool SetPreFileName( const std::string& pre_str );

private:
	LogToXml		log_holder_;
#endif
};

//log to dbgview
class LogToDbgview : public LogSink{
	LogToDbgview();
	friend class LogToDbgviewHelper;

public:
	bool SetDbgViewPreString(const std::string& pre_str);

private:
	bool InitSink() override;
};
class LogToDbgviewHelper{
public:
#ifdef _LOG_OUTPUT_EMPTY
	inline LogToDbgviewHelper(){}
	inline void ClearHolder(){}
	inline bool SetDbgViewPreString( const std::string& pre_str ){ return true; }
#else
	LogToDbgviewHelper();
	void ClearHolder();
	bool SetDbgViewPreString( const std::string& pre_str );

private:
	LogToDbgview		log_holder_;
#endif
};

class LogToConsole : public LogSink{
	LogToConsole();
	friend class LogToConsoleHelper;
private:
	bool InitSink() override;
};
class LogToConsoleHelper{
public:
#ifdef _LOG_OUTPUT_EMPTY
	inline LogToConsoleHelper(){}
	inline void ClearHolder(){}
#else
	LogToConsoleHelper();
	void ClearHolder();

private:
	LogToConsole		log_holder_;
#endif
};

//log empty
class LogEmpty{
public:
	inline LogEmpty(){}
	template<typename T>
	inline LogEmpty& operator << (const T&){
		return *this;
	}
	inline LogEmpty& write(const char*, size_t){
		return *this;
	}
	inline LogEmpty& write(const wchar_t*, size_t){
		return *this;
	}
};

//#pragma warning(pop)

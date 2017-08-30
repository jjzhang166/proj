#include "stdafx.h"
#include <CmnMix/Cmn_LogOut.h>
#include <cassert>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/sinks/async_frontend.hpp>
#include <boost/smart_ptr/shared_ptr.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/optional.hpp>
#include <AssetSys/AS_Sys.h>
#include <AssetSys/AS_Dir.h>
#include <boost/log/utility/setup/console.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

const char* const kGlobalAttr_DbgViewPreStr = "DbgV_PreStr";
boost::optional<std::pair< logging::attribute_set::iterator, bool > > it_DbgViewPreStr;

bool LogSink::AddLogSink( const LogSink& initor )
{
#ifndef _LOG_OUTPUT_EMPTY
	if ( !const_cast<LogSink&>(initor).InitSink() )
		return false;
	static bool global_inited = false;
	if ( !global_inited )
	{
		global_inited = true;
		//boost::log::add_common_attributes();
		auto pCore = logging::core::get();
		if ( !pCore )
		{
			assert(false);
			return false;
		}
		pCore->add_global_attribute(
			logging::aux::default_attribute_names::line_id(),
			attrs::counter< unsigned int >(1));
		pCore->add_global_attribute(
			logging::aux::default_attribute_names::timestamp(),
			attrs::local_clock());
		/*pCore->add_global_attribute(
			"ProcessID",
			attrs::current_process_id());*/
		//pCore->add_thread_attribute("Scope", attrs::named_scope());
		pCore->add_global_attribute("Scope", attrs::named_scope());

		auto str_process_id = std::to_string(::GetCurrentProcessId());
		it_DbgViewPreStr = pCore->add_global_attribute(kGlobalAttr_DbgViewPreStr, attrs::make_constant(str_process_id));
	}
#endif
	return true;
}

LogSink::~LogSink()
{
	if (sink_ptr_)
	{
		auto& log_core = boost::log::core::get();
		if (log_core)
			log_core->remove_sink(sink_ptr_);
	}
}

//create sink
template<typename T>
sinks::synchronous_sink<T>* CreateLogSink(){
	return new sinks::synchronous_sink<T>(
		//keywords::file_name = LogSinkInitor_Xml::GetDefFilePattern(),
		keywords::rotation_size = 1 * 1024 * 1024
		);
}
bool LogToXml::InitSink()
{
#ifndef _LOG_OUTPUT_EMPTY
	//typedef sinks::synchronous_sink<XmlSinkBackend> file_sink;
	XmlSinkT sink(CreateLogSink<XmlSinkBackend>());
	if ( !sink )
	{
		assert(false);
		return false;
	}
	if (log_file_pattern_.empty())
		log_file_pattern_ = GetDefFilePattern();
	sink_ptr_ = sink;
	xml_sink_ = sink;

	if ( !RefreshFilePath() )
		return false;
	if ( !RefreshFileNamePattern() )
		return false;
	// Upon restart, scan the directory for files matching the file_name pattern
	//sink->locked_backend()->scan_for_files();

	sink->set_formatter
		(
		expr::format("\t<record id=\"%1%\" level=\"%2%\" timestamp=\"%3%\" scope=\"%4%\">%5%</record>")
		% expr::attr< unsigned int >("LineID")
		% expr::attr< logging::trivial::severity_level >("Severity")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y.%m.%d-%H:%M:%S.%f")
		% expr::format_named_scope("Scope", keywords::format = "%n")
		% expr::xml_decor[ expr::stream << expr::smessage ]
		/*expr::stream << "\t<record id=\"" <<
		expr::attr< unsigned int >("LineID") << "\" level=\"" << 
		expr::attr< logging::trivial::severity_level >("Severity") <<
		"\" timestamp=\""
		expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y.%m.%d %H:%M:%S.%f") <<
		"\" scope=\"" << "\">" <<
		expr::format_named_scope("Scope", keywords::format = "%n") << 
		expr::xml_decor[ expr::stream << expr::smessage ]*/
	);

	// Set header and footer writing functors
	namespace bll = boost::lambda;
	auto& backend = sink->locked_backend();
	if ( !backend )
	{
		assert(false);
		return false;
	}
	backend->set_open_handler
		(
		bll::_1 << "<?xml version=\"1.0\"?>\n<log>\n"
		);
	backend->set_close_handler
		(
		bll::_1 << "</log>\n"
		);
	//自动刷新，也就是时时刷新到文件中的
	backend->auto_flush();

	//set filter，貌似没有必要了
	/*sink->set_filter(
		logging::trivial::severity >=
#ifdef _DEBUG
		logging::trivial::trace
#else
		logging::trivial::trace
#endif
		);*/
	// Add the sink to the core
	logging::core::get()->add_sink(sink);
#endif
	return true;
}

LogToXml::LogToXml() : log_path_("")
{
}

void LogToXml::FlushToTempFile()
{
	auto ptr = boost::log::core::get();
	if (ptr)
		ptr->flush();
}

bool LogToXml::GenLogFilePath(const char* relative_path_name)
{
	if (!relative_path_name)
	{
		assert(false);
		return false;
	}
	auto node = AS_Sys::GetMe().GetRoot();
	if (!node)
	{
		assert(false);
		return false;
	}
	node = node->GetSysDir();
	if (!node)
	{
		assert(false);
		return false;
	}
	auto log_path = node->FindChild(relative_path_name);
	if (!log_path)
	{
		AS_NodePtr ptr(new AS_DiskDir);
		assert(ptr);
		ptr->SetPath(relative_path_name, nullptr);
		node->Add(ptr);
		log_path = ptr;
	}
	log_path_ = log_path->GetAbsolutePath_().string();
	return true;
}

bool LogToXml::RefreshFilePath()
{
	if (log_path_.empty())
	{
		assert(false);
		return false;
	}
	if ( !xml_sink_ )
	{
		assert(false);
		return false;
	}
	auto& the_sink = xml_sink_->locked_backend();
	if ( !the_sink )
	{
		assert(false);
		return false;
	}
	the_sink->set_file_collector(sinks::file::make_collector(
		keywords::target = log_path_,					/*< the target directory >*/
		keywords::max_size = 16 * 1024 * 1024,          /*< maximum total size of the stored files, in bytes >*/
		keywords::min_free_space = 100 * 1024 * 1024    /*< minimum free space on the drive, in bytes >*/
		));
	return true;
}

bool LogToXml::RefreshFileNamePattern()
{
	if ( log_path_.empty() || log_file_pattern_.empty() )
	{
		assert(false);
		return false;
	}
	if ( !xml_sink_ )
	{
		assert(false);
		return false;
	}
	auto& the_sink = xml_sink_->locked_backend();
	if ( !the_sink )
	{
		assert(false);
		return false;
	}
	boost::filesystem::path path_file_name(log_path_);
	path_file_name /= log_file_pattern_;
	the_sink->set_file_name_pattern(path_file_name);
	return true;
}

std::string LogToXml::GetDefFilePattern()
{
	CStringA file_name_str;
	file_name_str.Format("%d.xml", ::GetCurrentProcessId());
	//年月日时分秒，%N表示是第几个LOG
	std::string file_pattern = "%Y-%m-%d_%H-%M-%S#%N#";
	file_pattern += file_name_str;
	return file_pattern;
}

bool LogToXml::SetPreFileName( const std::string& pre_str )
{
	if (pre_str.empty())
	{
		assert(false);
		return false;
	}
	log_file_pattern_ = pre_str + "#" + log_file_pattern_;
	return RefreshFileNamePattern();
}

typedef std::shared_ptr<BoostLogger> BoostLoggerPtr;
BoostLoggerPtr g_boost_logger_ptr_;
boost::mutex	g_log_mutex_;
BoostLogger& GetLogger()
{
	if (g_boost_logger_ptr_)
		return *g_boost_logger_ptr_;
	boost::lock_guard<boost::mutex> l(g_log_mutex_);
	if (g_boost_logger_ptr_)
		return *g_boost_logger_ptr_;
	g_boost_logger_ptr_ = BoostLoggerPtr(new BoostLogger);
	assert(g_boost_logger_ptr_);
	return *g_boost_logger_ptr_;
}

void LogSink::CloseAllLogOutput()
{
	//boost::log::core::get()->remove_all_sinks();
	assert(false);
	boost::lock_guard<boost::mutex> l(g_log_mutex_);
	g_boost_logger_ptr_.reset();
	boost::log::core::get().reset();
}

const LogSink::LogSinkPtrT& LogSink::GetSinkPtr() const
{
	return sink_ptr_;
}

void LogSink::swap( LogSink& rhs )
{
	sink_ptr_.swap(rhs.sink_ptr_);
}

void LogSink::ClearSinkPtr()
{
	sink_ptr_.reset();
}

void my_formatter( logging::record_view const& rec, logging::formatting_ostream& )
{	
	std::ostringstream str_strm;
	/*static DWORD kPid = ::GetCurrentProcessId();
	str_strm << "zhp:pid=\"" << kPid <<
	"\" id=\"" << logging::extract<unsigned int>("LineID", rec) <<*/
	str_strm << LOG_HEAD_TAG ":id=\"" <<
		logging::extract<unsigned int>("LineID", rec) <<
		"\" level=\"" <<
		logging::extract<logging::trivial::severity_level>("Severity", rec) <<
		"\" scope=\"" <<
		logging::extract<attrs::named_scope>("Scope", rec).get_ptr()->get_scopes()
		<< "\" msg=\"" <<
		rec[expr::smessage] << "\"";
	OutputDebugStringA(str_strm.str().c_str());
}

class DbgviewStreambuf{
public:   
	typedef char      char_type;  
	typedef boost::iostreams::sink_tag  category;    
	std::streamsize write(const char* s, std::streamsize n){
		if (n <= 0)
			return n;
		assert(s);
		static const char kLogTag[] = LOG_HEAD_TAG;
		static_assert(_countof(kLogTag) == 5, "");
		auto c_end = (char*)(s + n);
		auto cur = s;
		for (auto c = (char*)s; c < c_end; ++c)
		{
			if (*c == '\n')
			{
				if (cur == s)
				{
					if (n >= _countof(kLogTag) - 1 && memcmp(kLogTag, cur, _countof(kLogTag) - 1) == 0)
					{
						auto next = c + 1;
						if (next >= c_end)
							break;
						auto old = *next;
						*next = 0;
						OutputDebugStringA(cur);
						*next = old;
						cur = next;
						continue;
					}
					OutputDebugStringA("\n");
				}
				std::string tmp(LOG_HEAD_TAG"\t");
				tmp.append(cur, c - cur + 1);
				OutputDebugStringA(tmp.c_str());
				cur = c + 1;
			}
		}
		if (cur < c_end)
		{
			if (n < _countof(kLogTag) - 1 || memcmp(kLogTag, cur, _countof(kLogTag) - 1) != 0)
			{
				if (cur == s)
					OutputDebugStringA("\n");
				std::string tmp(LOG_HEAD_TAG"\t");
				tmp.append(cur, c_end);
				OutputDebugStringA(tmp.c_str());
				return n;
			}
			char c_old = *c_end;
			*c_end = 0;
			OutputDebugStringA(cur);
			*c_end = c_old;
		}
		return n;
	}
};
class DbgviewStream : public std::ostream{
public:
	DbgviewStream() : stm_buf_(dbgview_streambuf_), std::ostream(&stm_buf_){
	}

	DbgviewStreambuf									dbgview_streambuf_;
	boost::iostreams::stream_buffer<DbgviewStreambuf>	stm_buf_;
};

bool LogToDbgview::InitSink()
{
#ifndef _LOG_OUTPUT_EMPTY
	auto core = logging::core::get();
	if ( !core )
	{
		assert(false);
		return false;
	}
	/*backend->add_stream(
		boost::shared_ptr< std::ostream >(&std::clog, boost::empty_deleter()));*/

	// Complete sink type
	typedef sinks::synchronous_sink< sinks::text_ostream_backend > sink_t;
	// Wrap it into the frontend and register in the core
	boost::shared_ptr<sink_t> sink(new sink_t);
	if ( !sink )
	{
		assert(false);
		return false;
	}
	assert(sink);
	core->add_sink(sink);
	sink_ptr_ = sink;

	// You can manage filtering and formatting through the sink interface
	//貌似没有必要了
	/*sink->set_filter(
		logging::trivial::severity >= 
#ifdef _DEBUG
		logging::trivial::trace
#else
		//logging::trivial::warning
		logging::trivial::trace
#endif
		);*/
	//sink->set_formatter(&my_formatter);
	// You can also manage backend in a thread-safe manner
	/*{
		sink_t::locked_backend_ptr p = sink->locked_backend();
		p->add_stream(boost::make_shared<StreamT>());
	} // the backend gets released here*/
	auto cur_process_id = GetCurrentProcessId();
	sink->set_formatter
		(
		expr::format(LOG_HEAD_TAG "[%1%]:id=\"%2%\" level=\"%3%\" msg=\"%4%\" scope=\"%5%\" timestamp=\"%6%\"")
		% expr::attr<std::string>(kGlobalAttr_DbgViewPreStr)
		% expr::attr< unsigned int >("LineID")
		% expr::attr< logging::trivial::severity_level >("Severity")
		% expr::smessage
		% expr::format_named_scope("Scope", keywords::format = "%n")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y.%m.%d-%H:%M:%S.%f")
		//% expr::xml_decor[ expr::stream << expr::smessage ]

	/**/

		/*expr::stream << "\t<record id=\"" <<
		expr::attr< unsigned int >("LineID") << "\" level=\"" << 
		expr::attr< logging::trivial::severity_level >("Severity") <<
		"\" timestamp=\""
		expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y.%m.%d %H:%M:%S.%f") <<
		"\" scope=\"" << "\">" <<
		expr::format_named_scope("Scope", keywords::format = "%n") << 
		expr::xml_decor[ expr::stream << expr::smessage ]*/
	);
	boost::shared_ptr<std::ostream> dbgview_buf(new DbgviewStream);
	auto& backend = sink->locked_backend();
	if ( !backend )
	{
		assert(false);
		return false;
	}
	backend->add_stream(dbgview_buf);
	backend->auto_flush();
#endif
	return true;
}

LogToDbgview::LogToDbgview()
{

}

bool LogToDbgview::SetDbgViewPreString( const std::string& pre_str )
{
	if (pre_str.empty())
	{
		assert(false);
		return false;
	}
	auto pCore = logging::core::get();
	if ( !pCore )
	{
		assert(false);
		return false;
	}
	if ( !it_DbgViewPreStr )
	{
		assert(false);
		return false;
	}
	if ( !it_DbgViewPreStr->second )
	{
		assert(false);
		return false;
	}
	pCore->remove_global_attribute(it_DbgViewPreStr->first);
	it_DbgViewPreStr = pCore->add_global_attribute(kGlobalAttr_DbgViewPreStr, attrs::make_constant(pre_str));
	return it_DbgViewPreStr->second;
}

#ifndef _LOG_OUTPUT_EMPTY
LogToXmlHelper::LogToXmlHelper(const char* relative_path_name)
{
	static bool init_once = false;
	if (init_once)
	{
		assert(false);
		return;
	}
	init_once = true;
	if (!relative_path_name)
	{
		assert(false);
		return;
	}
	if (log_holder_.GenLogFilePath(relative_path_name))
	{
		VERIFY(LogToXml::AddLogSink(log_holder_));
		return;
	}	
	assert(false);
}

void LogToXmlHelper::ClearHolder()
{
	log_holder_.ClearSinkPtr();
}

bool LogToXmlHelper::SetPreFileName( const std::string& pre_str )
{
	return log_holder_.SetPreFileName(pre_str);
}

LogToDbgviewHelper::LogToDbgviewHelper()
{
	static bool init_once = false;
	if (init_once)
	{
		assert(false);
		return;
	}
	init_once = true;
	LogToDbgview::AddLogSink(log_holder_);	
}

void LogToDbgviewHelper::ClearHolder()
{
	log_holder_.ClearSinkPtr();
}

bool LogToDbgviewHelper::SetDbgViewPreString( const std::string& pre_str )
{
	return log_holder_.SetDbgViewPreString(pre_str);
}


LogToConsoleHelper::LogToConsoleHelper()
{
	static bool init_once = false;
	if (init_once)
	{
		assert(false);
		return;
	}
	init_once = true;
	LogToConsole::AddLogSink(log_holder_);
}

void LogToConsoleHelper::ClearHolder()
{
	log_holder_.ClearSinkPtr();
}

#endif // !_LOG_OUTPUT_EMPTY

LogToConsole::LogToConsole()
{

}

bool LogToConsole::InitSink()
{
#ifndef _LOG_OUTPUT_EMPTY
	//clog和cerr都属于标准出错流，而cout属于标准输出流。
	logging::add_console_log(std::cout, keywords::format = expr::format("id=%1% level=%2% timestamp=%3% scope=%4% \n msg->%5%")
		% expr::attr< unsigned int >("LineID")
		% expr::attr< logging::trivial::severity_level >("Severity")
		% expr::format_date_time< boost::posix_time::ptime >("TimeStamp", "%Y.%m.%d-%H:%M:%S.%f")
		% expr::format_named_scope("Scope", keywords::format = "%n")
		% expr::smessage, keywords::auto_flush = true);
#endif
	return true;
}

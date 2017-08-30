#include "stdafx.h"
#include "Net/PocoMore.h"
#include <assert.h>
#include "Script/lua_base.h"
#include <strstream>
#include "Poco/InflatingStream.h"
#include "Poco/DeflatingStream.h"
#include "Json/writer.h"
#include <boost/algorithm/string/find.hpp>
#include <Common/UsefulFunc.h>
#include <CmnMix/Cmn_LogOut.h>

HttpPackageReadFromBuffer::HttpPackageReadFromBuffer()
{
}

const char* HttpPackageReadFromBuffer::RetrieveRealMsg( const char* buffer )
{
	if (buffer)
	{
		auto real_msg = strstr(buffer, "\r\n\r\n");
		if ( !real_msg )
			return nullptr;
		real_msg += 4;
		//if ( *real_msg )
		return real_msg;
	}
	return nullptr;
}

const std::string& HttpPackageReadFromBuffer::GetRealMsg() const
{
	return real_msg_;
}

bool HttpPackageReadFromBuffer::ReadFromBuffer( const std::string& buffer )
{
	return ReadFromBuffer(buffer.c_str(), buffer.size());
}

void HTTPResponse_RFB::ReadImpl( std::istream& istr )
{
	__super::read(istr);
}

bool HttpPackageReadFromBuffer::DeContentAsGzip( std::string& str_out ) const
{
	if (real_msg_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "real_msg_为空";
		return false;
	}

	using namespace Poco;
	std::istringstream istr(real_msg_); 
	InflatingInputStream infalter(istr, InflatingStreamBuf::STREAM_GZIP);

	while (infalter.good())
	{  
		str_out.append(1, infalter.get());
	}
	return true;
}

bool HttpPackageReadFromBuffer::DeGzipContentToJson( Json::Value& root ) const
{
	std::string str_src;
	if ( !DeContentAsGzip(str_src) )
	{
		return false;
	}

	return JsonReader::GetMe().parse(str_src, root, false);
}

void RegPocoForLua::RegHttpStrHandlers( LuaVM* lua_vm )
{
	assert(lua_vm);
	assert(lua_vm->lstate_);
	using namespace luabind;
	using namespace Poco::Net;

	module(lua_vm->lstate_)
		[
			class_<HttpPackageReadFromBuffer>("HttpPackageReadFromBuffer")
			.def("DeContentToJson", &HttpPackageReadFromBuffer::DeContentToJson),

			class_<HTTPRequest_RFB, HttpPackageReadFromBuffer>("HTTPRequest_RFB")
			.def("GetURI", &HTTPRequest_RFB::getURI)
			.def("GetMethod", &HTTPRequest_RFB::getMethod)
		];
}

void HTTPRequest_RFB::ReadImpl( std::istream& istr )
{
	__super::read(istr);
}

void HTTPRequest_RFB::SetContentImpl( const std::string& msg )
{
	setContentLength(msg.size());
}

void HTTPRequest_RFB::write( std::ostream& ostr ) const
{
	__super::write(ostr);
	ostr.write(real_msg_.c_str(), real_msg_.size());
}

void HTTPRequest_RFB::read( std::istream& istr )
{
	__super::read(istr);
	while (istr.good())
	{  
		real_msg_.append(1, istr.get());
	}
}

bool HttpPackageReadFromBuffer::EnContentAsGzip( const std::string& content_src )
{
	using namespace Poco;
	std::ostringstream ostr;
	DeflatingOutputStream deflater(ostr, DeflatingStreamBuf::STREAM_GZIP);
	deflater << content_src;

	// ensure buffers get flushed before connected stream is closed
	deflater.close();

	real_msg_ = ostr.str();
	SetContentImpl(real_msg_);
	return true;
}

bool HttpPackageReadFromBuffer::EnJsonContentAsGzip( const Json::Value& root )
{
	Json::FastWriter writer;
	real_msg_ = writer.write(root);
	return EnContentAsGzip(real_msg_);
}

/*
void HttpPackageReadFromBuffer::ReadFromBuffer( const char* buffer )
{
	if ( !buffer )
		return;
	const char* msg_line = buffer;
	assert(msg_line);
	auto it_real_msg = RetrieveRealMsg(msg_line);

	const auto buffer_size = strlen(buffer);
	if (it_real_msg)
	{
		real_msg_.assign(it_real_msg, msg_line + buffer_size);
	}

	std::istrstream str_stream(msg_line, it_real_msg ? std::distance(msg_line, it_real_msg) : buffer_size);
	ReadImpl(str_stream);
}*/

bool HttpPackageReadFromBuffer::ReadFromBuffer( const char* buffer, pt_dword buff_size )
{
	if ( !buffer || buff_size <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "buffer不能为空，长度不能为0";
		return false;
	}
	const char* msg_line = buffer;
	assert(msg_line);
	auto it_real_msg = RetrieveRealMsg(msg_line);
	if ( !it_real_msg )
		return false;

	const auto buffer_size = buff_size;
	real_msg_.assign(it_real_msg, msg_line + buffer_size);

	std::istrstream str_stream(msg_line, it_real_msg ? std::distance(msg_line, it_real_msg) : buffer_size);
	ReadImpl(str_stream);
	return true;
}

bool HttpPackageReadFromBuffer::DeContentToJson( Json::Value& root ) const
{
	return JsonReader::GetMe().parse(real_msg_, root, false);
}

void HttpPackageReadFromBuffer::SetContent( const std::string& msg )
{
	real_msg_ = msg;
	SetContentImpl(msg);
}

bool HTTPResponse_RFB::ContentTypeIsJson() const
{
	return boost::find_first(getContentType(), "/json");
	//return getContentType() == "application/json";
}

bool HTTPResponse_RFB::ContentTypeIsGzip() const
{
	return boost::find_first(getContentType(), "/x-gzip");
	//return getContentType() == "application/x-gzip";
}

bool HTTPResponse_RFB::AddContent( const char* buffer, pt_dword buffer_size )
{
	if ( !buffer || buffer_size <= 0 )
		return false;
	real_msg_.append(buffer, buffer_size);
	return true;
}

void HTTPResponse_RFB::SwapContent( std::string& content )
{
	std::swap(content, real_msg_);
}

void HTTPResponse_RFB::SetContentImpl( const std::string& msg )
{
	setContentLength(msg.size());
}

void HTTPResponse_RFB::write( std::ostream& ostr ) const
{
	__super::write(ostr);
	ostr.write(real_msg_.c_str(), real_msg_.size());
}

void HTTPResponse_RFB::read( std::istream& istr )
{
	__super::read(istr);
	while (istr.good())
	{  
		real_msg_.append(1, istr.get());
	}
}

TimeDuration::TimeDuration( pt_dword time_duration ) : last_time_(1980, 1, 1)
{
	time_duration_ = time_duration;
}

TimeDuration::TimeDuration()
{
	time_duration_ = 0;
}

TimeDuration TimeDuration::FromNow( pt_dword time_duration )
{
	TimeDuration tmp;
	tmp.time_duration_ = time_duration;
	return tmp;
}

bool TimeDuration::IsTimeout() const
{
	return IsTimeout(time_duration_);
}

bool TimeDuration::IsTimeout( pt_dword time_duration ) const
{
	if ( time_duration <= 0 )
		return true;
	DateTimeLocal time_now;
	assert(time_now >= last_time_);
	return (time_now - last_time_).totalMilliseconds() >= time_duration;
}

bool TimeDuration::DoTimeout( pt_dword time_duration )
{
	if ( time_duration <= 0 )
		return true;
	DateTimeLocal time_now;
	assert(time_now >= last_time_);
	if ( (time_now - last_time_).totalMilliseconds() >= time_duration )
	{
		last_time_ = time_now;
		return true;
	}
	return false;
}

bool TimeDuration::DoTimeout()
{
	return DoTimeout(time_duration_);
}

void TimeDuration::UpdateTime()
{
	last_time_ = DateTimeLocal();
}

void TimeDuration::SetDuration( pt_dword time_duration )
{
	time_duration_ = time_duration;
}

pt_dword TimeDuration::GetDuration() const
{
	return time_duration_;
}

const DateTimeLocal& TimeDuration::GetLastTime() const
{
	return last_time_;
}

pt_dword TimeDuration::CalcLeftDuration( pt_dword time_duration ) const
{
	if ( time_duration <= 0 )
		return 0;
	DateTimeLocal time_now;
	assert(time_now >= last_time_);
	auto total_duation = (time_now - last_time_).totalMilliseconds();
	if ( total_duation >= time_duration )
		return 0;
	return (pt_dword)((decltype(total_duation))time_duration - total_duation);
}

pt_dword TimeDuration::CalcLeftDuration() const
{
	return CalcLeftDuration(time_duration_);
}

bool TimeDuration::TimeToDo( const DoFuncT& func, pt_dword time_duration )
{
	if (IsTimeout(time_duration))
	{
		BOOST_SCOPE_EXIT_ALL(=){
			DoTimeout(time_duration);
		};
		func();
		return true;
	}
	return false;
}

bool TimeDuration::TimeToDo( const DoFuncT& func )
{
	return TimeToDo(func, time_duration_);
}

bool TimeDuration::TimeToDoSync( const DoFuncSyncT& func, pt_dword time_duration )
{
	auto left_duration = CalcLeftDuration(time_duration);
	if ( left_duration <= 0 )
	{
		BOOST_SCOPE_EXIT_ALL(&){
			UpdateTime();
		};
		return func();
	}
	DummySleep(left_duration);
	BOOST_SCOPE_EXIT_ALL(=){
		DoTimeout(time_duration);
	};
	return func();
}

bool TimeDuration::TimeToDoSync( const DoFuncSyncT& func )
{
	return TimeToDoSync(func, time_duration_);
}

bool TimeDuration::TimeToDoSync(const DoFuncSyncT& func, pt_dword time_duration, const WaitFuncT& fn_wait)
{
	if (time_duration == 0)
		time_duration = time_duration_;
	auto left_duration = CalcLeftDuration(time_duration);
	if (left_duration <= 0)
	{
		BOOST_SCOPE_EXIT_ALL(&){
			UpdateTime();
		};
		return func();
	}
	if (!fn_wait(left_duration))
		return false;
	BOOST_SCOPE_EXIT_ALL(= ){
		DoTimeout(time_duration);
	};
	return func();
}

bool TimeDuration::WaitWithRunOne()
{
	while (true)
	{
		auto left_duration = CalcLeftDuration(time_duration_);
		if ( left_duration <= 0 )
		{
			BOOST_SCOPE_EXIT_ALL(&){
				UpdateTime();
			};
			return true;
		}
		GetIoService().RunOne();
	}
	assert(false);
	return false;
}

void TimeDuration::Wait() const
{
	auto left_time = CalcLeftDuration();
	if (left_time > 0)
		DummySleep(left_time);
}

void TimeDuration::WaitUpdate()
{
	Wait();
	UpdateTime();
}

TimeElapse::TimeElapse() : last_time_(1970, 1, 1)
{
	time_dist_ = 0;
	let_first_time_pass_ = true;
	is_first_time_ = true;
	is_start_ = true;
}

TimeElapse::TimeElapse( pt_dword time_dist, bool let_fist_time_pass ) : last_time_(1970, 1, 1)
{
	time_dist_ = time_dist;
	let_first_time_pass_ = let_fist_time_pass;
	is_first_time_ = true;
	is_start_ = true;
}

void TimeElapse::SetTimeDist( pt_dword time_dist )
{
	time_dist_ = time_dist;
}

pt_dword TimeElapse::GetTimeDist() const
{
	return time_dist_;
}

void TimeElapse::SetLetFirstTimePass( bool let_first_time_pass )
{
	let_first_time_pass_ = let_first_time_pass;
}

bool TimeElapse::IsTimeout() const
{
	if ( !is_start_ )
		return false;
	if (is_first_time_)
	{
		is_first_time_ = false;
		last_time_ = DateTimeLocal();
		return let_first_time_pass_;
	} 
	else
	{
		DateTimeLocal time_now;
		const auto& time_span = time_now - last_time_;
		return time_span.totalMilliseconds() >= time_dist_;
	}
}

bool TimeElapse::DoTimeout()
{
	if ( !is_start_ )
		return false;
	if (is_first_time_)
	{
		is_first_time_ = false;
		last_time_ = DateTimeLocal();
		return let_first_time_pass_;
	} 
	else
	{
		DateTimeLocal time_now;
		const auto& time_span = time_now - last_time_;
		auto res = time_span.totalMilliseconds() >= time_dist_;
		if (res)
		{
			last_time_ = time_now;
			return true;
		}
		return false;
	}
}

void TimeElapse::RefreshCurTime()
{
	last_time_ = DateTimeLocal();
}

void TimeElapse::SetTimerStart( bool start )
{
	is_start_ = start;
}

void TimeElapse::ResetLastTime()
{
	last_time_ = DateTimeLocal(1970, 1, 1);
}

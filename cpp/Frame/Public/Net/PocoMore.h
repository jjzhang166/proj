#pragma once
/*
@author zhp
@date 2014-3-7 17:42
@purpose for more POCO library
*/
#include <boost/asio.hpp>			//这个文件必须在使用Poco之前包含进来，否则就会有编译错误
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Common/JsonMore.h>
#include <Poco/DateTime.h>
#include <CmnMix/Cmn_PocoMore.h>
#include "../DependSelfLib.h"
#include <ProtocolX/P_DefinesX.h>

//http package read from buffer
class HttpPackageReadFromBuffer{
public:
	HttpPackageReadFromBuffer();

public:
	//read from buffer
	bool ReadFromBuffer( const std::string& buffer );
	//void ReadFromBuffer( const char* buffer );
	bool ReadFromBuffer( const char* buffer, pt_dword buff_size );
	//得到消息正文
	const std::string& GetRealMsg() const;
	//索引到消息正文
	static const char* RetrieveRealMsg( const char* buffer );
	//设置real msg
	void SetContent( const std::string& msg );
	//////////////////////////////////////////////////////////////////////////
	//decode content as gzip
	bool DeContentAsGzip( std::string& str_out ) const;
	//decode gzip content to json
	bool DeGzipContentToJson( Json::Value& root ) const;
	//decode content to json
	bool DeContentToJson( Json::Value& root ) const;
	//encode content as gzip
	bool EnContentAsGzip( const std::string& content_src );
	//encode Json content as gzip
	bool EnJsonContentAsGzip( const Json::Value& root );
	//////////////////////////////////////////////////////////////////////////

protected:
	//执行读
	virtual void ReadImpl( std::istream& istr ) = 0;
	virtual void SetContentImpl( const std::string& msg ) = 0;

protected:
	std::string				real_msg_;
};

//http response more of read from buffer
class HTTPResponse_RFB : public Poco::Net::HTTPResponse, public HttpPackageReadFromBuffer{
public:
	//内容是否是json
	bool ContentTypeIsJson() const;
	//内容是否是gzip
	bool ContentTypeIsGzip() const;
	//增加消息内容
	bool AddContent( const char* buffer, pt_dword buffer_size );
	//交换消息内容
	void SwapContent( std::string& content );

public:
	void write( std::ostream& ostr ) const;
	void read( std::istream& istr );

protected:
	//执行读
	virtual void ReadImpl( std::istream& istr );
	virtual void SetContentImpl( const std::string& msg );
};

//http request more of read from buffer
class HTTPRequest_RFB : public Poco::Net::HTTPRequest, public HttpPackageReadFromBuffer{	
public:
	void write( std::ostream& ostr ) const;
	void read( std::istream& istr );

protected:
	//执行读
	virtual void ReadImpl( std::istream& istr );
	virtual void SetContentImpl( const std::string& msg );
};

class LuaVM;
//poco register for lua
class RegPocoForLua{
public:
	//注册http字符串处理类
	static void RegHttpStrHandlers( LuaVM* lua_vm );
};
//////////////////////////////////////////////////////////////////////////
class TimeDuration{
public:
	typedef FunctionVoid DoFuncT;
	typedef FunctionBool DoFuncSyncT;
	typedef std::function<bool(pt_dword)> WaitFuncT;

public:
	TimeDuration(pt_dword time_duration);
	bool IsTimeout() const;
	bool IsTimeout(pt_dword time_duration) const;
	pt_dword CalcLeftDuration() const;
	pt_dword CalcLeftDuration(pt_dword time_duration) const;
	bool DoTimeout();
	bool DoTimeout(pt_dword time_duration);
	void UpdateTime();
	void Wait() const;
	void WaitUpdate();
	void SetDuration(pt_dword time_duration);
	pt_dword GetDuration() const;
	const DateTimeLocal& GetLastTime() const;
	bool TimeToDo(const DoFuncT& func, pt_dword time_duration);
	bool TimeToDo(const DoFuncT& func);
	bool TimeToDoSync(const DoFuncSyncT& func, pt_dword time_duration);
	bool TimeToDoSync(const DoFuncSyncT& func);
	bool TimeToDoSync(const DoFuncSyncT& func, pt_dword time_duration, const WaitFuncT& fn_wait);
	//如果没到指定的间隔，那么就run one一次
	bool WaitWithRunOne();

public:
	static TimeDuration FromNow(pt_dword time_duration);

private:
	TimeDuration();

private:
	DateTimeLocal			last_time_;
	pt_dword				time_duration_;
};
//////////////////////////////////////////////////////////////////////////
class TimeElapse{
public:
	//默认第一次放行
	TimeElapse();
	//默认第一次放行
	TimeElapse( pt_dword time_dist, bool let_fist_time_pass = true );
	void SetTimeDist( pt_dword time_dist );
	void SetTimerStart( bool start );
	pt_dword GetTimeDist() const;
	void SetLetFirstTimePass( bool let_first_time_pass );
	bool IsTimeout() const;
	void RefreshCurTime();
	bool DoTimeout();
	void ResetLastTime();

private:
	mutable DateTimeLocal	last_time_;
	pt_dword				time_dist_;					//单位毫秒
	bool					let_first_time_pass_;		//是否第一次放行
	mutable bool			is_first_time_;
	bool					is_start_;
};
//////////////////////////////////////////////////////////////////////////
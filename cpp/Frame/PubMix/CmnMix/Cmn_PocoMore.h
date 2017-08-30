#pragma once
/*
@author zhp
@date 2016/5/28 17:47
@purpose 兼容BufCall
*/
#include <Poco/DateTime.h>
#include <luabind/luabind.hpp>

//local datetime
class DateTimeLocal : public Poco::DateTime{
public:
	static void RegForLua(lua_State* l);
	DateTimeLocal();
	DateTimeLocal(const Poco::Timestamp& timestamp);
	DateTimeLocal(int year, int month, int day, int hour = 0, int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);
	DateTimeLocal(double julianDay);
	DateTimeLocal(Poco::Timestamp::UtcTimeVal utcTime, Poco::Timestamp::TimeDiff diff);
	DateTimeLocal(const Poco::DateTime& dt);
	void swap(DateTimeLocal& dateTime);
	//从UTC转换为本地时间
	DateTimeLocal FromUtc(const Poco::DateTime& dt);
	//是否是今天
	bool IsToday() const;
	//日期是否相同
	bool IsSameDay(const DateTimeLocal& rhs) const;	
	std::string FormatString() const;
	bool FromFormatString(const std::string& format_str);
	std::time_t GetEpochTime() const;
	void FromEpochTime(std::time_t time);

private:

};
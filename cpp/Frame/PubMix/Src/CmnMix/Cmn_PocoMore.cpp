#include "stdafx.h"
#include <CmnMix/Cmn_PocoMore.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeParser.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include <luabind/operator.hpp>

DateTimeLocal::DateTimeLocal()
{
	makeLocal(GetTzd());
}

DateTimeLocal::DateTimeLocal(const Poco::Timestamp& timestamp) : Poco::DateTime(timestamp)
{
}

DateTimeLocal::DateTimeLocal(int year, int month, int day, int hour, int minute, int second, int millisecond, int microsecond)
	: Poco::DateTime(year, month, day, hour, minute, second, millisecond, microsecond)
{
}

DateTimeLocal::DateTimeLocal(double julianDay) : Poco::DateTime(julianDay)
{
}

DateTimeLocal::DateTimeLocal(Poco::Timestamp::UtcTimeVal utcTime, Poco::Timestamp::TimeDiff diff)
	: Poco::DateTime(utcTime, diff)
{
}

void DateTimeLocal::swap(DateTimeLocal& dateTime)
{
	__super::swap(dateTime);
}

DateTimeLocal::DateTimeLocal(const Poco::DateTime& dt)
	: Poco::DateTime(dt)
{
}

DateTimeLocal DateTimeLocal::FromUtc(const Poco::DateTime& dt)
{
	Poco::DateTime local_dt(dt);
	local_dt.makeLocal(GetTzd());
	return DateTimeLocal(local_dt);
}

bool DateTimeLocal::IsToday() const
{
	DateTimeLocal time_now;
	return IsSameDay(time_now);
}

bool DateTimeLocal::IsSameDay(const DateTimeLocal& rhs) const
{
	return rhs.year() == year() && rhs.month() == month() && rhs.day() == day();
}

std::string DateTimeLocal::FormatString() const
{
	return Poco::DateTimeFormatter::format(*this, Poco::DateTimeFormat::SORTABLE_FORMAT);
}

std::time_t DateTimeLocal::GetEpochTime() const
{
	return timestamp().epochTime();
}

void DateTimeLocal::FromEpochTime(std::time_t time)
{
	__super::operator = (Poco::Timestamp::fromEpochTime(time));
}

bool DateTimeLocal::FromFormatString(const std::string& format_str)
{
	int time_zone = 0;
	if (!Poco::DateTimeParser::tryParse(Poco::DateTimeFormat::SORTABLE_FORMAT, format_str, *this, time_zone))
	{
		assert(false);
		return false;
	}
	return true;
}

void DateTimeLocal::RegForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<DateTimeLocal>("DateTimeLocal")
		.def(constructor<>())
		.def(constructor<int, int, int, int, int, int, int>())
		.def(self < DateTimeLocal())
		.def(self <= DateTimeLocal())
		.def(self == DateTimeLocal())
		.def("swap", &DateTimeLocal::swap)
		.def("IsToday", &DateTimeLocal::IsToday)
		.def("FormatString", &DateTimeLocal::FormatString)
		.def("FromFormatString", &DateTimeLocal::FromFormatString)
		.def("GetEpochTime", &GetEpochTime)
		.def("FromEpochTime", &FromEpochTime)
	];
}

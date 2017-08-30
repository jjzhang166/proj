#include "stdafx.h"
#include <Common/CmnUuid.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/name_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/nil_generator.hpp>
#include <Common/UsefulClass.h>
#include <Poco/StreamCopier.h>

void CmnUuid::GT_Nil()
{
	using namespace boost::uuids;
	static nil_generator gen;
	__super::operator = (gen());
	assert(is_nil());
}

static boost::uuids::string_generator g_string_gen_uuid_;

void CmnUuid::GT_String( const char* str )
{
	using namespace boost::uuids;
	assert(str);
	__super::operator = (g_string_gen_uuid_(str));
}

void CmnUuid::GT_String( const wchar_t* str )
{
	using namespace boost::uuids;
	assert(str);
	__super::operator = (g_string_gen_uuid_(str));
}

void CmnUuid::GT_String( const std::string& str )
{
	using namespace boost::uuids;
	assert(!str.empty());
	__super::operator = (g_string_gen_uuid_(str));
}

void CmnUuid::GT_String( const std::wstring& str )
{
	using namespace boost::uuids;
	assert(!str.empty());
	__super::operator = (g_string_gen_uuid_(str));
}

struct stDltStringUuid : CmnUuid{
	stDltStringUuid(){
		GT_String("9AF5C6EE-7244-4243-9007-CE8DB643D162");
	}
};
struct stNameGenHelper{
	stNameGenHelper() : dlt_string_uuid_(), name_gen_(dlt_string_uuid_){}

	stDltStringUuid					dlt_string_uuid_;
	boost::uuids::name_generator	name_gen_;
};
static stNameGenHelper g_name_gen_helper_;

void CmnUuid::GT_Name( const char* str )
{
	assert(str);
	__super::operator = (g_name_gen_helper_.name_gen_(str));
}

void CmnUuid::GT_Name( const wchar_t* str )
{
	assert(str);
	__super::operator = (g_name_gen_helper_.name_gen_(str));
}

void CmnUuid::GT_Name( const std::string& str )
{
	assert(!str.empty());
	__super::operator = (g_name_gen_helper_.name_gen_(str));
}

void CmnUuid::GT_Name( const std::wstring& str )
{
	assert(!str.empty());
	__super::operator = (g_name_gen_helper_.name_gen_(str));
}

void CmnUuid::GT_Name( const void* buffer, pt_dword buffer_size )
{
	assert(buffer_size > 0);
	assert(buffer);
	__super::operator = (g_name_gen_helper_.name_gen_(buffer, buffer_size));
}

void CmnUuid::GT_Name( std::istream& istm )
{
	std::string str_tmp;
	Poco::StreamCopier::copyToString(istm, str_tmp);
	GT_Name(str_tmp.c_str(), str_tmp.size());
}

void CmnUuid::GT_Name( const char* str, const CmnUuid& uuid_base )
{
	assert(str);
	boost::uuids::name_generator name_gen(uuid_base);
	__super::operator = (name_gen(str));
}

void CmnUuid::GT_Name( const wchar_t* str, const CmnUuid& uuid_base )
{
	assert(str);
	boost::uuids::name_generator name_gen(uuid_base);
	__super::operator = (name_gen(str));
}

void CmnUuid::GT_Name( const std::string& str, const CmnUuid& uuid_base )
{
	assert(!str.empty());
	boost::uuids::name_generator name_gen(uuid_base);
	__super::operator = (name_gen(str));
}

void CmnUuid::GT_Name( const std::wstring& str, const CmnUuid& uuid_base )
{
	assert(!str.empty());
	boost::uuids::name_generator name_gen(uuid_base);
	__super::operator = (name_gen(str));
}

void CmnUuid::GT_Name( const void* buffer, pt_dword buffer_size, const CmnUuid& uuid_base )
{
	assert(buffer);
	assert(buffer_size > 0);
	boost::uuids::name_generator name_gen(uuid_base);
	__super::operator = (name_gen(buffer, buffer_size));
}

void CmnUuid::GT_Name( std::istream& istm, const CmnUuid& uuid_base )
{
	std::string str_tmp;
	Poco::StreamCopier::copyToString(istm, str_tmp);
	boost::uuids::name_generator name_gen(uuid_base);
	__super::operator = (name_gen(str_tmp.c_str(), str_tmp.size()));
}

void CmnUuid::GT_Random()
{
	static boost::uuids::random_generator rgen;
	__super::operator = (rgen());
}

std::string CmnUuid::ToString() const
{
	assert(!is_nil());
	return std::move(boost::uuids::to_string(*this));
}

std::wstring CmnUuid::ToWString() const
{
	assert(!is_nil());
	return std::move(boost::uuids::to_wstring(*this));
}

bool CmnUuid::ToJson( Json::Value& json_v ) const
{
	static_assert(sizeof(double) * 2 == sizeof(*this), "");
	Json::Value json_tmp;
	assert(!is_nil());
	json_tmp[Json::UInt(0)] = *(const double*)(const char*)this;
	json_tmp[1] = *(const double*)((const char*)this + static_size() / 2);
	json_v.swap(json_tmp);
	return true;
}

CmnUuid::CmnUuid()
{
	GT_Nil();
}

CmnUuid::CmnUuid( const boost::uuids::uuid& rhs ) : uuid(rhs)
{

}

bool CmnUuid::FromJson( const Json::Value& json_v )
{
	if ( json_v.isNull() || !json_v.isArray() || 2 != json_v.size() )
	{
		assert(false);
		return false;
	}
	const auto& json_0 = json_v[Json::UInt(0)];
	if ( json_0.isNull() || !json_0.isNumeric() )
	{
		assert(false);
		return false;
	}
	const auto& json_1 = json_v[1];
	if ( json_1.isNull() || !json_1.isNumeric() )
	{
		assert(false);
		return false;
	}
	*(double*)(char*)this = json_0.asDouble();
	*(double*)((char*)this + static_size() / 2) = json_1.asDouble();
	return true;
}

CmnUuid& CmnUuid::operator=( const boost::uuids::uuid& rhs )
{
	__super::operator = (rhs);
	return *this;
}

Json::Value CmnUuid::ToJson() const
{
	Json::Value json_v;
	if ( !ToJson(json_v) )
	{
		assert(false);
		return json_v;
	}
	return json_v;
}

#pragma once
/*
@author zhp
@date 2014/10/4 15:22
@purpose uuid
*/
#include <boost/uuid/uuid.hpp>
#include <string>
#include <json/value.h>
#include "../DependSelfLib.h"

class CmnUuid : public boost::uuids::uuid{
	//initialisation, generate to
public:
	CmnUuid();
	CmnUuid( const boost::uuids::uuid& rhs );
	CmnUuid& operator = ( const boost::uuids::uuid& rhs );
	void GT_Nil();
	void GT_String( const char* str );
	void GT_String( const wchar_t* str );
	void GT_String( const std::string& str );
	void GT_String( const std::wstring& str );
	void GT_Name( const char* str );
	void GT_Name( const wchar_t* str );
	void GT_Name( const std::string& str );
	void GT_Name( const std::wstring& str );
	void GT_Name( const void* buffer, pt_dword buffer_size );
	void GT_Name( std::istream& istm );
	void GT_Name( const char* str, const CmnUuid& uuid_base );
	void GT_Name( const wchar_t* str, const CmnUuid& uuid_base );
	void GT_Name( const std::string& str, const CmnUuid& uuid_base );
	void GT_Name( const std::wstring& str, const CmnUuid& uuid_base );
	void GT_Name( const void* buffer, pt_dword buffer_size, const CmnUuid& uuid_base );
	void GT_Name( std::istream& istm, const CmnUuid& uuid_base );
	void GT_Random();

public:
	std::string ToString() const;
	std::wstring ToWString() const;
	bool ToJson( Json::Value& json_v ) const;
	Json::Value ToJson() const;
	bool FromJson( const Json::Value& json_v );
};
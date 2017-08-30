#include "stdafx.h"
#include "CG_Defines.h"

bool CG_RoleInfo::ParseFromJosn( const Json::Value& json_v )
{
	const auto& json_job = json_v["job"];
	if ( json_job.isNull() || !json_job.isString() )
	{
		assert(false);
		return false;
	}
	const auto& json_name = json_v["name"];
	if ( json_name.isNull() || !json_name.isString() )
	{
		assert(false);
		return false;
	}
	role_job_ = json_job.asString();
	role_name_ = json_name.asString();
	return true;
}

bool CG_RoleInfo::ConvertToJson( Json::Value& json_v )
{
	json_v["job"] = role_job_;
	json_v["name"] = role_name_;
	return true;
}

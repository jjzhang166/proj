#include "stdafx.h"
#include <Common/GameObjBase.h>
#include <Common/UsefulFunc.h>

const CmnPriLuaData* g_obj_filter_lua_env_ = nullptr;

void SetGObjFilterLuaEnv( const CmnPriLuaData* pri_lua_data )
{
	g_obj_filter_lua_env_ = pri_lua_data;
}

GObjFilterBase::~GObjFilterBase()
{

}

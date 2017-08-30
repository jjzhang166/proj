#include "stdafx.h"
#include <Common/CmnPriLuaData.h>
#include <Common/UsefulFunc.h>

CmnPriLuaData::CmnPriLuaData( lua_State* lua_state ) : private_tbl_(RandomUniqueStr())
{
	lstate_ = lua_state;
	assert(lua_state);
}

CmnPriLuaData::CmnPriLuaData( lua_State* lua_state, const char* private_tbl ) : private_tbl_(private_tbl)
{
	lstate_ = lua_state;
	assert(lua_state);
	assert(private_tbl);
	assert(!private_tbl_.empty());
}

CmnPriLuaData::~CmnPriLuaData()
{
	using namespace luabind;
	if ( lstate_ && !private_tbl_.empty() )
	{
		assert(lstate_);
		const auto& regs = registry(lstate_);
		assert(LUA_TTABLE == type(regs));
		regs[private_tbl_] = object();
	}
}

void CmnPriLuaData::SetLuaObj( const std::string& obj_name, const luabind::object& lua_obj ) const
{
	using namespace luabind;
	if (obj_name.empty())
	{
		assert(false);
		return;
	}
	assert(lstate_);
	const auto& regs = registry(lstate_);
	assert(LUA_TTABLE == type(regs));
	object the_tbl = regs[private_tbl_];
	const auto kTblType = type(the_tbl);
	if ( LUA_TNIL == kTblType )
	{
		the_tbl = newtable(lstate_);
		regs[private_tbl_] = the_tbl;
	}
	else
		assert(LUA_TTABLE == kTblType);
	the_tbl[obj_name] = lua_obj;
}

luabind::object CmnPriLuaData::GetLuaObj( const std::string& obj_name ) const
{
	using namespace luabind;
	if (obj_name.empty())
	{
		assert(false);
		return object();
	}
	assert(lstate_);
	const auto& regs = registry(lstate_);
	assert(LUA_TTABLE == type(regs));
	const object& the_tbl = regs[private_tbl_];
	if ( LUA_TTABLE != type(the_tbl) )
	{
		assert(LUA_TNIL == type(the_tbl));
		return object();
	}
	return the_tbl[obj_name];
}

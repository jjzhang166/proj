#pragma once
/*
@author zhp
@date 2014/9/23 17:04
@purpose for private lua data
*/
#include <luabind/object.hpp>
#include "../DependSelfLib.h"

class CmnPriLuaData{
public:
	CmnPriLuaData( lua_State* lua_state );
	CmnPriLuaData( lua_State* lua_state, const char* private_tbl );
	~CmnPriLuaData();
	void SetLuaObj( const std::string& obj_name, const luabind::object& lua_obj ) const;
	luabind::object GetLuaObj( const std::string& obj_name ) const;
	lua_State* lua() const{ return lstate_; }

private:
	std::string		private_tbl_;

public:
	lua_State*		lstate_;
};
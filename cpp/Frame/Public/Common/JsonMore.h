#pragma once
/*
@author zhp
@date 2014-3-4
@purpose for improve jsoncpp
*/
#include "json/reader.h"
#include "json/value.h"
//#include "lua/lstate.h"
#include "../DependSelfLib.h"

class LuaVM;
//reader singleton
class JsonReader : public Json::Reader{
	JsonReader();
public:
	//only one
	static JsonReader& GetMe();
	//parse c string
	bool ParseCStr( const char* document, Json::Value &root );
	//parse form file
	bool ParseFromFile( const char* full_file_name, Json::Value &root );
};

//json register for lua
class RegJsonForLua{
	RegJsonForLua();

public:
	//×¢²ájson value
	static void RegValue( LuaVM* lua_vm );
	//×¢²ájson reader
	static void RegReader( LuaVM* lua_vm );
	//×¢²ájson path
	static void RegPath( LuaVM* lua_vm );
};
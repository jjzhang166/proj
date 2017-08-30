#pragma once
/*
@author zhp
@date 2014/9/26 19:02
@purpose for json echo coordinate translation
现已被废弃不用，由BufCallCoorTrans代替，2016/4/23 18:57
*/
#include "JsonCall.h"
#include <FuncObjs/FuncObjCoor.h>
#include "../DependSelfLib.h"

class JECoorTrans;
class JECoorEndpoint : public CoorEndpoint{
public:
	JECoorEndpoint( FuncObj* parent, JECoorTrans& json_echo );

protected:
	//bool OnTrans( const P_CoorPacketToT& source_route, const void* param, size_t param_size );

private:
	JECoorTrans&			json_echo_;
};

class JECoorTrans : public JsonEcho{
public:
	JECoorTrans( LuaVM& lua_vm, FuncObj& parent );
	void SetCurRoute( const P_CoorPacketToT& cur_route );
	const P_CoorPacketToT& GetCurRoute() const;
	void SetSourceRoute( const P_CoorPacketToT& source_route );
	const P_CoorPacketToT& GetSourceRoute() const;

public:
	//static void RegForLua( LuaVM& lua_vm );

protected:
	bool DoInnerImpl( const std::string& str_buffer );

private:
	//JECoorEndpoint				trans_endpoint_;
	P_CoorPacketToT			cur_route_;
	P_CoorPacketToT			source_route_;
};
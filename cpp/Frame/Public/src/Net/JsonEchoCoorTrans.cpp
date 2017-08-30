#include "stdafx.h"
#include <Net/JsonEchoCoorTrans.h>

JECoorTrans::JECoorTrans( LuaVM& lua_vm, FuncObj& parent ) : JsonEcho(lua_vm, parent, kFuncObjTemplateId_JsonEchoCoorTrans)/*, trans_endpoint_(&parent, *this)*/
{

}

bool JECoorTrans::DoInnerImpl( const std::string& str_buffer )
{
	assert(false);
	return false;
	//assert(3 == cur_route_.size());
	//return trans_endpoint_.DoTrans(cur_route_, str_buffer.c_str(), str_buffer.size());
}

void JECoorTrans::SetCurRoute( const P_CoorPacketToT& cur_route )
{
	cur_route_ = cur_route;
}

const P_CoorPacketToT& JECoorTrans::GetCurRoute() const
{
	return cur_route_;
}

void JECoorTrans::SetSourceRoute( const P_CoorPacketToT& source_route )
{
	source_route_ = source_route;
}

const P_CoorPacketToT& JECoorTrans::GetSourceRoute() const
{
	return source_route_;
}

/*
void JECoorTrans::RegForLua( LuaVM& lua_vm )
{
	static bool init_once = false;
	if (init_once)
		return;
	init_once = true;

	__super::RegForLua(lua_vm);

	using namespace luabind;
	module(lua_vm.lua())
		[
			class_<JECoorTrans, JsonEcho>("JECoorTrans")
		];
}*/

JECoorEndpoint::JECoorEndpoint( FuncObj* parent, JECoorTrans& json_echo ) : CoorEndpoint(parent), json_echo_(json_echo)
{

}

/*
bool JECoorEndpoint::OnTrans( const P_CoorPacketToT& source_route, const void* param, size_t param_size )
{
	json_echo_.SetSourceRoute(source_route);
	return json_echo_.DecodeJson((void*)param, param_size);
}*/

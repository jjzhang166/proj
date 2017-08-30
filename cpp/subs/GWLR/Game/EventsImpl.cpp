#include "stdafx.h"
#include "EventsImpl.h"
#include "EventsName.h"
#include <luabind/function.hpp>
#include <Script/lua_base.h>
#include <Protocol/P_Defines.h>
#include "Facility/GPlayer.h"
#include "FuncObjC.h"

CmnEventsMgr* CmnEventsFatory_Impl::CreateEventsMgr()
{
	return new CmnEventsMgr_Impl(lua_state_);
}

CmnEventsFatory_Impl::CmnEventsFatory_Impl( lua_State* lua_state )
{
	lua_state_ = lua_state;
	assert(lua_state);
}

CmnEventsMgr_Impl::CmnEventsMgr_Impl( lua_State* lua_state ) : CmnEventsMgr(lua_state)
{
}

void CmnEventsMgr_Impl::LoadProtoEvents()
{
	
}

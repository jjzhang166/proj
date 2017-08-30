#include "stdafx.h"
#include <Script/lua_base.h>
#include <Common/EnsureCleanup.h>
#include <Common/GameObjBase.h>
#include <Common/JsonMore.h>
#include <Net/PocoMore.h>
#include <Common/MultiStrMatchOnTime.h>
#include <Script/LuaScript.h>

void LuaVM::BindGlobalFn()
{
	__super::BindGlobalFn();

	using namespace luabind;
	module(lstate_)
		[
			def("LoadLib", &LoadLib),
			def("FreeLib", &FreeLib)
		];
}

void LuaVM::BindClass()
{
	__super::BindClass();

	using namespace luabind;
	module(lstate_)
		[
			class_<LuaVM, LuaThrdData>("LuaVM"),

			class_<IoTimer, IoTimerPtr>("IoTimer")
			.def("Continue", &IoTimer::Continue)
			.def("Cancel", &IoTimer::Cancel)
			.def("SetDuration", &IoTimer::SetDuration)
			.def("IsClocking", &IoTimer::IsClocking),

			class_<GObjFilterBase, boost::shared_ptr<GObjFilterBase> >("GObjFilterBase")
			.def("添加过滤处理", &GObjFilterBase::AddLuaFilterHandler)
			.def("添加过滤器", &GObjFilterBase::AddFilter),

			class_<GGameObjBase>("GGameObjBase")
			.def("Update", &GGameObjBase::Update)
			.def("DebugInfo", &GGameObjBase::DebugInfo),

			class_<GGameObjMgrBase>("GGameObjMgrBase")
			.def("UpdateAll", &GGameObjMgrBase::UpdateAll)
			.def("RebuildAll", &GGameObjMgrBase::RebuildAll)
			.def("DebugInfo", &GGameObjMgrBase::DebugInfo)
		];

#ifdef _LUA_USE_JSON
	RegJsonForLua::RegValue(this);
	RegJsonForLua::RegReader(this);
	RegJsonForLua::RegPath(this);
#endif

#ifdef _LUA_USE_POCO_PARSE_HTTP_STR
	RegPocoForLua::RegHttpStrHandlers(this);
#endif

#ifdef _LUA_USE_MULTI_STR_MATCH
	MultiStrMatchOnTimeForLua::RegForLua(this);
#endif
}

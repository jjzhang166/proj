#include "stdafx.h"
#include <AppMix/WgLuaVm.h>
#include <AppMix/GameComponent.h>
#include <Common/GameObjBase.h>
#include <Common/UsefulFunc.h>

CmnGameLuaThrdData::CmnGameLuaThrdData(lua_State* lstate) : LuaThrdData(lstate)
{
	cmd_namespace_ = "scmd";		//script command
}

void CmnGameLuaThrdData::BindGlobalFn()
{
	using namespace luabind;
	__super::BindGlobalFn();
	module_ mod(lstate_, cmd_namespace_);
	BindScriptCommand(mod);
}

void CmnGameLuaThrdData::BindScriptCommand(luabind::module_& mod)
{
	using namespace luabind;
	mod[
		def("CopyAssetFile", &CopyAssetFile)
	];
}

bool CmnGameLuaThrdData::CopyAssetFile(const char* file_name, const char* new_file_path)
{
	if (!file_name || !new_file_path)
	{
		assert(false);
		return false;
	}
	auto& app = Cmn_GameDllApp::GetApp();
	auto node = app.GetAssetDir();
	if (!node)
	{
		assert(false);
		return false;
	}
	node = node->FindPosteritySmart(file_name);
	if (!node)
	{
		assert(false);
		return false;
	}
	if (!node->SaveToDisk(new_file_path))
	{
		assert(false);
		return false;
	}
	return true;
}

GameDllLuaThrdData::GameDllLuaThrdData(lua_State* lstate) : CmnGameLuaThrdData(lstate)
{

}

void GameDllLuaThrdData::BindGlobalFn()
{
	__super::BindGlobalFn();
	using namespace luabind;
	module(lstate_)[
		def("LoadLib", &LoadLib),
		def("FreeLib", &FreeLib)
	];
	BindTest();
}

void GameDllLuaThrdData::BindClass()
{
	__super::BindClass();
	using namespace luabind;
	module(lstate_)[
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
}

void GameDllLuaThrdData::BindTest()
{

}

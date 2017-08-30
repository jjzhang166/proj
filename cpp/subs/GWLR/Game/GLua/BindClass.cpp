#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GClass.h"
#include "FuncObjC.h"
#include "Facility/GPlayer.h"
#include "Facility/GameObjMgr.h"
#include "Facility/GTask.h"
#include "Facility/GCloneMap.h"
#include "Facility/GMonster.h"

using namespace GClass;

GLuaC* GLuac_Instance()
{
	auto normal_c = FuncObj_NormalC::GetInstance();
	if ( !normal_c )
		return nullptr;
	return &normal_c->GetGLua();
}

void GLuaC::BindClass()
{
	__super::BindClass();
	using namespace luabind;
	module(lua())
		[
			class_<GLuaC, LuaVM>("GLuaC"),
			def("GLuac_Instance", &GLuac_Instance),

			class_<FuncObj_NormalC>("FuncObj_NormalC")
			.def("Test", &FuncObj_NormalC::Test),
			def("FuncObj_NormalC_Instance", &FuncObj_NormalC::GetInstance),

			class_<GameObjBase, GGameObjBase>("GameObjBase")
			.def("SetDir", &GameObjBase::SetDir),

			class_<GMonster, GameObjBase>("GMonster"),

			class_<GameObjMgrBase, GGameObjMgrBase>("GameObjMgrBase"),

			class_<GameObjMgr, GameObjMgrBase>("GameObjMgr")
			.def("TraverseAllDebugInfo", &GameObjMgr::TraverseAllDebugInfo)
			.def("FindMonsterByName", &GameObjMgr::FindByName)
			.def("FindNearestBeatableMonster", &GameObjMgr::FindNearestBeatableMonster_RoomNum)
			.def("ClearDeadMonsters", &GameObjMgr::ClearDeadMonsters)
			.def("DebugInfo_DeadMonsters", &GameObjMgr::DebugInfo_DeadMonsters),
			def("GameObjMgr_Me", &GameObjMgr::GetMe),

			class_<GPlayer, GameObjBase>("GPlayer"),

			class_<GPlayerMe, GameObjBase>("GPlayerMe")
			.def("SendWalk", &GPlayerMe::SendWalk)
			.def("SendSay", &GPlayerMe::SendSay)
			//.def("DoWalk", &GPlayerMe::DoWalk)
			//.def("DeltaWalk", &GPlayerMe::DeltaWalk)
			.def("HitByLeftBt", &GPlayerMe::HitByLeftBt)
			.def("HitByRightBt", &GPlayerMe::HitByRightBt)
			.def("SwitchKnife", &GPlayerMe::SwitchKnife)
			//.def("SetPosInfoAsAnotherObj", &GPlayerMe::SetPosInfoAsAnotherObj)
			//.def("HitMonster", &GPlayerMe::HitMonster)
			.def("SetDeltaAngle", &GPlayerMe::SetDeltaAngle)
			.def("SetRoleForward", &GPlayerMe::SetRoleForward)
			//.def("MoveForward", &GPlayerMe::MoveForward)
			//.def("SetRoleToBack", &GPlayerMe::SetRoleToBack)
			//.def("SetRoleToBackSync", &GPlayerMe::SetRoleToBackSync)
			.def("AttrackGameObj", &GPlayerMe::AttrackGameObj)
			.def("SetRoleForwardByPos", &GPlayerMe::SetRoleForwardByPos)
			.def("UpdateGrids", &GPlayerMe::UpdateGrids)
			.def("DebugGridsInfo", &GPlayerMe::DebugGridsInfo)
			.def("UpdateReceivedTasks", &GPlayerMe::UpdateReceivedTasks)
			.def("DebugReceivedTaskInfo", &GPlayerMe::DebugReceivedTaskInfo)
			.def("UpdateAcceptableTasks", &GPlayerMe::UpdateAcceptableTasks)
			.def("DebugAcceptableTasksInfo", &GPlayerMe::DebugAcceptableTasksInfo)
			.def("GetReceivedTaskMgr", &GPlayerMe::GetReceivedTaskMgr)
			.def("GetAcceptableTasks", &GPlayerMe::GetAcceptableTasks)
			.def("OpenNpc", &GPlayerMe::OpenNpc)
			//.def("SendAcceptTask", &GPlayerMe::SendAcceptTask)
			.def("UpdateEnterableFb", &GPlayerMe::UpdateEnterableFb)
			.def("DebugEnterableFb", &GPlayerMe::DebugEnterableFb)
			.def("DebugObjStatus", &GPlayerMe::DebugObjStatus)
			.def("SetMoveStepDist", &GPlayerMe::SetMoveStepDist)
			.def("DebugMySellList", &GPlayerMe::DebugMySellList)
			.def("DebugGainAuctionPagesInfo", &GPlayerMe::DebugGainAuctionPagesInfo)
			.def("SendRefreshMySellList", &GPlayerMe::SendRefreshMySellList)
			.def("SetHitBodyPart", &GPlayerMe::SetHitBodyPart)
			.def("UpdateCurFarmFacilityInfo", &GPlayerMe::UpdateCurFarmFacilityInfo)
			.def("DebugCurFarmFacilityInfo", &GPlayerMe::DebugCurFarmFacilityInfo)
			.def("Test", &GPlayerMe::Test)
			.def("Test1", &GPlayerMe::Test1),
			def("GPlayer_Me", &GPlayerMe::GetMe),

			class_<GTaskDb>("GTaskTemplate")
			.def("GetTaskId", &GTaskDb::GetTaskId)
			.def("GetTaskName", &GTaskDb::GetTaskName),

			class_<GTaskDbMgr, GGameObjMgrBase>("GTaskTemplateMgr")
			.def("AssertUniqueTaskName", &GTaskDbMgr::AssertUniqueTaskName)
			.def("TraverseMainTaskByName", &GTaskDbMgr::TraverseMainTaskByName)
			.def("TraverseAllTaskByName", &GTaskDbMgr::TraverseAllTaskByName),
			def("GTaskTemplateMgr_Me", &GTaskDbMgr::GetMe),

			class_<GTaskDbMgr::TaskIterator, IteratorBase, GTaskDbMgr::TaskIteratorPtr>("GTaskTemplateMgr_Iter")
			.def("CurItem", &GTaskDbMgr::TaskIterator::CurItem),

			class_<GReceivedTask>("GReceivedTask")
			.def("IsDone", &GReceivedTask::IsDone),

			class_<GReceivedTaskMgr, GGameObjMgrBase>("GReceivedTaskMgr")
			.def("FindById", &GReceivedTaskMgr::FindById)
			.def("CpyIdToLua", &GReceivedTaskMgr::CpyIdToLua),

			class_<GAcceptableTasks, GGameObjBase>("GAcceptableTasks")
			.def("CpyIdToLua", &GAcceptableTasks::CpyIdToLua),

			class_<MapDbMgr, GGameObjBase>("AllFbInfo"),
			def("AllFbInfo_Me", &MapDbMgr::GetMe),

			class_<GameObjMgr_AnyObj, GGameObjMgrBase>("GameObjMgr_AnyObj"),

			class_<FbDoorsMgr, GameObjMgr_AnyObj>("FbDoorsMgr"),
			def("FbDoorsMgr_Me", &FbDoorsMgr::GetMe),

			class_<GCollectObjMgr, GGameObjMgrBase>("GCollectObjMgr"),
			def("GCollectObjMgr_Me", &GCollectObjMgr::GetMe),

			class_<GItem>("GItem"),

			class_<GClass::BuffStateMgr, GGameObjMgrBase>("BuffStateMgr"),
			def("BuffStateMgr_Me", &GClass::BuffStateMgr::GetMe),

			class_<FarmDbMgr, GGameObjBase>("FarmDbMgr"),
			def("FarmDbMgr_Me", &FarmDbMgr::GetMe),

			class_<ItemDb, GGameObjBase>("ItemDb"),

			class_<ItemDbMgr, GGameObjMgrBase>("ItemDbMgr")
			.def("FindByName", &ItemDbMgr::FindByName),
			def("ItemDbMgr_Me", &ItemDbMgr::GetMe),

			class_<GManufactureDbMgr, GGameObjBase>("GManufactureInfo"),
			def("GManufactureDbMgr_Me", &GManufactureDbMgr::GetMe),

			class_<GSupplyBox, GGameObjBase>("GSupplyBox"),
			def("GSupplyBox_Me", &GSupplyBox::GetMe),

			class_<GClass::CraftDbMgr, GGameObjBase>("CraftInfo"),
			def("CraftDbMgr_Me", &GClass::CraftDbMgr::GetMe),

			class_<GClass::FarmSeedDbMgr, GGameObjBase>("FarmSeedInfo"),
			def("FarmSeedDbMgr_Me", &GClass::FarmSeedDbMgr::GetMe),

			class_<GStaticBeatableObjMgr, GameObjMgr_AnyObj>("GStaticBeatableObjMgr"),
			def("GStaticBeatableObjMgr_Me", &GStaticBeatableObjMgr::GetMe),

			class_<GGiftItemMgr, GGameObjMgrBase>("GGiftItemMgr"),
			def("GGiftItemMgr_Me", &GGiftItemMgr::GetMe),

			class_<GClass::GTaskFbInfo, GGameObjBase>("GTaskFbInfo"),
			def("GTaskFbInfo_Me", &GClass::GTaskFbInfo::GetMe)
		];
}
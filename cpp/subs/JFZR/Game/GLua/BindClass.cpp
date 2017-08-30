#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GClass.h"
#include "FuncObjC.h"
#include "Facility/GPlayer.h"
#include "Facility/GameObjMgr.h"
#include "Facility/GVillage.h"
#include "Facility/GRoom.h"
#include "Facility/GPacket.h"
#include "Facility/GMap.h"
#include "Facility/GStrMap.h"
#include "Facility/GCloneMap.h"

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
	using namespace GClass;
	using namespace std;
	RegWideCharForLua();
	module(lua())
		[
			class_<GLuaC, LuaVM>("GLuaC"),
			def("GLuac_Instance", &GLuac_Instance),

			class_<GGameObjBase>("GGameObjBase")
			.def("Update", &GGameObjBase::Update),

			class_<GGameObjMgrBase>("GGameObjMgrBase")
			.def("UpdateAll", &GGameObjMgrBase::UpdateAll)
			.def("RebuildUpdateAll", &GGameObjMgrBase::RebuildUpdateAll),

			class_<FuncObj_NormalC>("FuncObj_NormalC")
			.property("game_state", &FuncObj_NormalC::GetGameState, &FuncObj_NormalC::SetGameState)
			.def("ChooseChannel", &FuncObj_NormalC::ChooseChannel)
			.def("AutoCreateOrSelectRole", &FuncObj_NormalC::AutoCreateOrSelectRole)
			.def("RetChooseRoleScene", &FuncObj_NormalC::RetChooseRoleScene)
			.def("UntilShowGameMenuByPressEsp", &FuncObj_NormalC::UntilShowGameMenuByPressEsp)
			.def("ButtonClick", &FuncObj_NormalC::ButtonClick)
			.def("KeyClick", &FuncObj_NormalC::KeyClick)
			.def("SelectRoleA", &FuncObj_NormalC::SelectRoleA)
			.def("Test", &FuncObj_NormalC::Test),
			def("NormalC_Inst", &FuncObj_NormalC::GetInstance),

			class_<GSkillObj>("GSkillObj"),

			class_<GSkillMgr, GGameObjMgrBase>("GSkillMgr")
			.def("FindSkillObjById", &GSkillMgr::FindSkillObjById),

			class_<GameObjBase, GameObjBase::SelfPtrT>("GameObjBase")
			.def("Update", &GameObjBase::Update)
			.def("GetJobName", &GameObjBase::GetJobName),

			class_<GPlayer, GameObjBase>("GPlayer")
			.def("GetLevel", &GPlayer::GetLevel),

			class_<GPlayerSelf, GPlayer>("GPlayerSelf")
			.def("UseSkill", &GPlayerSelf::HitTargets)
			.def("GetPacket", &GPlayerSelf::GetPacket)
			.def("EnterCloneMap", &GPlayerSelf::EnterCloneMap)
			.def("EnterAnotherRoom", &GPlayerSelf::EnterAnotherRoom)
			.def("ReturnCity", &GPlayerSelf::ReturnCity)
			.def("RestartCloneMap", &GPlayerSelf::RestartCloneMap)
			.def("RepairAllWearEquip", &GPlayerSelf::RepairAllWearEquip)
			.def("KillAllMonster", &GPlayerSelf::KillAllMonster)
			.def("PickUpAllItems", &GPlayerSelf::PickUpAllItems)
			.def("PickUpAllGold", &GPlayerSelf::PickUpAllGold)
			.def("FightThroughAllRooms", &GPlayerSelf::FightThroughAllRooms)
			.def("FightAllGraph", &GPlayerSelf::FightAllGraph)
			.def("AutoLvlUpSkills", &GPlayerSelf::AutoLvlUpSkills)
			.def("AutoLearnLvlUpSkills", &GPlayerSelf::AutoLearnLvlUpSkills)
			.def("CommitCompleteTasks", &GPlayerSelf::CommitCompleteTasks)
			.def("CommitNoCompleteHandlerTasks", &GPlayerSelf::CommitNoCompleteHandlerTasks)
			.def("AcceptTheTask", &GPlayerSelf::AcceptTheTask)
			.def("GetReceivedTask", &GPlayerSelf::GetReceivedTask)
			.def("IsAcceptedTheTask", &GPlayerSelf::IsAcceptedTheTask)
			.def("TheTaskCompletable", &GPlayerSelf::TheTaskCompletable)
			.def("MoveToTheVillage", &GPlayerSelf::MoveToTheVillage)
			.def("MoveToTheCloneMap", &GPlayerSelf::MoveToTheCloneMap)
			.def("SellNormalTabAllEquips", &GPlayerSelf::SellNormalTabAllEquips)
			.def("LearnTheSkill", &GPlayerSelf::LearnTheSkill)
			.def("AcceptTheTaskW", &GPlayerSelf::AcceptTheTaskW)
			.def("AcceptLocalMapAnyTask", &GPlayerSelf::AcceptLocalMapAnyTask)
			.def("AcceptLocalMapAnyAllTasks", &GPlayerSelf::AcceptLocalMapAnyAllTasks)
			.def("CommitTask", &GPlayerSelf::CommitTask)
			.def("DestroyAllMisc", &GPlayerSelf::DestroyAllMisc)
			.def("GetFatigueData", &GPlayerSelf::GetFatigueData)
			.def("GetSkillMgr", &GPlayerSelf::GetSkillMgr)
			.def("GetSpecifyJobName", &GPlayerSelf::GetSpecifyJobName)
			.def("ChangeJobAsDlt", &GPlayerSelf::ChangeJobAsDlt)
			.def("NeedChangeJob", &GPlayerSelf::NeedChangeJob)
			.def("SaleProductAsPrice", &GPlayerSelf::SaleProductAsPrice)
			.def("SaleProductAsPriceBase", &GPlayerSelf::SaleProductAsPriceBase)
			.def("CalcSellingProductCnt", &GPlayerSelf::CalcSellingProductCnt)
			.def("SendMailItem", &GPlayerSelf::SendMailItemDelay)
			.def("SendMailGold", &GPlayerSelf::SendMailGold)
			.def("SellTheItemToShop", &GPlayerSelf::SellTheItemToShop)
			//test
			//////////////////////////////////////////////////////////////////////////
			.def("Test", &GPlayerSelf::Test)
			.def("Test1", &GPlayerSelf::Test1)
			.def("TestAllowedV_CM", &GPlayerSelf::TestAllowedV_CM)
			.def("TestEnterCloneMap", &GPlayerSelf::TestEnterCloneMap)
			//////////////////////////////////////////////////////////////////////////
			,
			def("GPlayer_Me", &GPlayerSelf::GetMe),

			class_<SessionOperC, JsonEchoBase>("SessionOperC")
			.def("ForegroundTheWindow", &SessionOperC::ForegroundTheWindow)
			.def("SetLoginSucceed", &SessionOperC::SetLoginSucceed)
			.def("SetEndDateTime", &SessionOperC::SetEndDateTime)
			.def("sadsex", &SessionOperC::SetAddrDatas)
			.def("SetGameSerIdx", &SessionOperC::SetGameSerIdx)
			.def("SetGameAcType", &SessionOperC::SetGameAcType)
			.def("SetMatchTime", &SessionOperC::StableString)
			.def("SetPlayerInfo", &SessionOperC::CheckRsaPubKey)
			.def("GetSkillsInfo", &SessionOperC::GetSkillsInfo)
			.def("GetGameObjsInfo", &SessionOperC::GetGameObjsInfo)
			.def("GetAllItems", &SessionOperC::GetAllItems)
			.def("GetRoomInfo", &SessionOperC::GetRoomInfo)
			.def("CreateRandNameRole", &SessionOperC::CreateRandNameRole)
			.def("SelectRole", &SessionOperC::SelectRole)
			.def("GetMyOfferList", &SessionOperC::GetMyOfferList)
			.def("GetOtherOfferItems", &SessionOperC::GetOtherOfferItems),

			class_<JECoorTransImpl, JECoorTrans>("JECoorTransImpl")
			.def("GainPriProductsInfo", &JECoorTransImpl::GainPriProductsInfo)
			.def("GainProductsInfo", &JECoorTransImpl::GainProductsInfo)
			.def("OnProductsInfo", &JECoorTransImpl::OnProductsInfo),

			class_<GameObjMgr::GameObjIterBase, GameObjMgr::GameObjIterPtr>("GameObjIterBase")
			.def("First", &GameObjMgr::GameObjIterBase::First)
			.def("Next", &GameObjMgr::GameObjIterBase::Next)
			.def("IsDone", &GameObjMgr::GameObjIterBase::IsDone)
			.def("CurItem", &GameObjMgr::GameObjIterBase::CurItem),

			class_<GameObjMgr, GGameObjMgrBase>("GameObjMgr")
			.def("TraverseGameObjByType", &GameObjMgr::TraverseGameObjByType),
			def("GameObjMgr_Me", &GameObjMgr::GetMe),

			class_<GVillage, GGameObjBase>("GVillage"),
			def("GVillage_Me", &GVillage::GetMe),

			class_<GRoomBase, GGameObjBase>("GRoom")
			.def("GetTheDoor", &GRoomBase::GetTheDoor),

			class_<GCloneMap, GRoomBase>("GCloneMap"),
			def("GCloneMap_Me", &GCloneMap::GetMe),

			class_<GRoomDoor, GGameObjBase>("GRoomDoor"),

			class_<GPacket, GGameObjMgrBase>("GPacket")
			.def("AutoReplaceEquip", &GPacket::AutoReplaceEquip)
			.def("GetTabNormal", &GPacket::GetTabNormal)
			.def("GetTabMaterial", &GPacket::GetTabMaterial)
			.def("GetTabConsume", &GPacket::GetTabConsume),

			class_<GMap, GGameObjBase>("GMap")
			.def("IsTown", &GMap::IsTown),
			def("GMap_Me", &GMap::GetMe),

			class_<GameEn2ChNodeMgr::StrIdexBase, GameEn2ChNodeMgr::StrIndexPtr>("En2Ch_StrIdexBase")
			.def("En2Ch", &GameEn2ChNodeMgr::StrIdexBase::En2Ch)
			.def("Ch2EnA", &GameEn2ChNodeMgr::StrIdexBase::Ch2EnA)
			.def("TwoCh2EnA", &GameEn2ChNodeMgr::StrIdexBase::TwoCh2EnA)
			.def("ThreeCh2EnA", &GameEn2ChNodeMgr::StrIdexBase::ThreeCh2EnA),

			class_<GameEn2ChNodeMgr, GGameObjMgrBase>("GameEn2ChNodeMgr")
			.def("GetQuestIndex", &GameEn2ChNodeMgr::GetQuestIndex)
			.def("GetQuestBeginDescIndex", &GameEn2ChNodeMgr::GetQuestBeginDescIndex)
			.def("GetItemIndex", &GameEn2ChNodeMgr::GetItemIndex),
			def("GameEn2ChNodeMgr_Me", &GameEn2ChNodeMgr::GetMe),

			class_<TaskCurJobTemplate>("TaskCurJobTemplate"),
			class_<TaskCurJobTemplateMgr, GGameObjMgrBase>("TaskCurJobMgr")
			.def("FindByTaskId", &TaskCurJobTemplateMgr::FindByTaskId),
			def("TaskCurJobTemplateMgr_Me", &TaskCurJobTemplateMgr::GetMe),
			
			class_<TaskReceived, GGameObjBase>("TaskReceived")
			.def("GetStrTaskId", &TaskReceived::GetStrTaskId),

			class_<TaskReceivedMgr::AcceptedTaskIterBase, TaskReceivedMgr::AcceptedTaskIterPtr>("AcceptedTaskIterBase")
			.def("First", &TaskReceivedMgr::AcceptedTaskIterBase::First)
			.def("Next", &TaskReceivedMgr::AcceptedTaskIterBase::Next)
			.def("IsDone", &TaskReceivedMgr::AcceptedTaskIterBase::IsDone)
			.def("CurItem", &TaskReceivedMgr::AcceptedTaskIterBase::CurItem),

			class_<TaskReceivedMgr, GGameObjMgrBase>("TaskReceivedMgr")
			.def("TraverseUncompleteTasks", &TaskReceivedMgr::TraverseUncompleteTasks)
			.def("TraverseCompleteTasks", &TaskReceivedMgr::TraverseCompleteTasks),
			
			class_<TaskNpc, GGameObjBase>("TaskNpc")
			.def("GetAcceptableMgr", &TaskNpc::GetAcceptableMgr),

			class_<TaskNpc::TaskMgr, GGameObjMgrBase>("TaskNpcTaskMgr")
			.def("TraverseAllTask", &TaskNpc::TaskMgr::TraverseAllTask),

			class_<TaskNpc::TaskNode, GGameObjBase>("TaskNpcTaskNode")
			.def("GetTaskId", &TaskNpc::TaskNode::GetTaskId),

			class_<TaskNpcMgr, GGameObjMgrBase>("TaskNpcMgr")
			.def("TraverseAcceptableNpc", &TaskNpcMgr::TraverseAcceptableNpc),
			def("TaskNpcMgr_Me", &TaskNpcMgr::GetMe),

			class_<TaskNpcMgr::NpcTaskIterBase, TaskNpcMgr::NpcTaskIterPtr>("AcceptedTaskIterBase")
			.def("First", &TaskNpcMgr::NpcTaskIterBase::First)
			.def("Next", &TaskNpcMgr::NpcTaskIterBase::Next)
			.def("IsDone", &TaskNpcMgr::NpcTaskIterBase::IsDone)
			.def("CurItem", &TaskNpcMgr::NpcTaskIterBase::CurItem),

			class_<TaskNpc::TaskMgr::TaskNodeIterBase, TaskNpc::TaskMgr::TaskNodeIterPtr>("AcceptedTaskIterBase")
			.def("First", &TaskNpc::TaskMgr::TaskNodeIterBase::First)
			.def("Next", &TaskNpc::TaskMgr::TaskNodeIterBase::Next)
			.def("IsDone", &TaskNpc::TaskMgr::TaskNodeIterBase::IsDone)
			.def("CurItem", &TaskNpc::TaskMgr::TaskNodeIterBase::CurItem),

			class_<GSkillTemplateMgr, GGameObjMgrBase>("GSkillTemplateMgr"),
			def("GSkillTemplateMgr_Me", &GSkillTemplateMgr::GetMe),

			class_<GItem>("GItem")
			.def("GetItemId", &GItem::GetItemId)
			.def("GetItemLevel", &GItem::GetItemLevel)
			.def("GetItemQualityValue", &GItem::GetItemQualityValue)
			.def("GetItemCnt", &GItem::GetItemCnt)
			.def("GetItemCnName", &GItem::GetItemCnName),

			class_<GItemMgr>("GItemMgr")
			.def("AddTradeFilterHandler", &GItemMgr::AddTradeFilterHandler),

			class_<GItemMgr_Normal, GItemMgr>("GItemMgr_Normal"),
			class_<GItemMgr_Consume, GItemMgr>("GItemMgr_Consume")
			.def("OpenItemA", &GItemMgr_Consume::OpenItemA),

			class_<GItemTemplateMgr, GGameObjMgrBase>("GItemTemplateMgr"),
			def("GItemTemplateMgr_Me", &GItemTemplateMgr::GetMe),

			class_<GAmbient>("GAmbient")
			.def("UntilCityUiNow", &GAmbient::UntilCityUiNow)
			.def("UntilEnteredFb", &GAmbient::UntilEnteredFb)
			.def("UntilCanRetCity", &GAmbient::UntilCanRetCity),
			def("GAmbient_Me", &GAmbient::GetMe),

			class_<GItemMgr::ItemFilterT, GObjFilterBase>("ItemFilter"),

			class_<CMDR_OnRequestProductList::stProductItem>("CFA457BAE278")
		];
}
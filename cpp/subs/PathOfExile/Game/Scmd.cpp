#include "stdafx.h"
#include <Scmd.h>
#include "Facility/GPlayer.h"
#include "Facility/GameObjMgr.h"
#include "Facility/GMonster.h"
#include "Facility/GSkill.h"
#include "Facility/GMap.h"
#include "Facility/GType.h"
#include <luabind/raw_policy.hpp>
#include "Facility/GItem.h"
#include "Facility/GTask.h"
#include "Facility/GNpc.h"
#include "DllApp.h"

MyLuaThrdData::MyLuaThrdData(lua_State* lstate) : GameDllLuaThrdData(lstate)
{
}

void MyLuaThrdData::BindGlobalFn()
{
	__super::BindGlobalFn();
	using namespace luabind;
	auto m = module(lstate_);
	CD_Interface::RegTestForLua(lstate_);
	CD_Interface::RegDbgInfoForLua(lstate_);
	CD_Interface::RegSomeForLua(lstate_);

	m[
		def("AddMapRoomRate", &AddMapRoomRate),
		def("UpdateCurMap", &UpdateCurMap),
		def("UpdateSelfArea", &UpdateSelfArea),
		def("SetMapAreaInfo", &SetMapAreaInfo)

#ifdef _DEBUG
		,
		def("DbgObjs", &DbgObjs),
		def("DbgSortedObjs", &DbgSortedObjs),
		def("DbgPakcageItems", &DbgPakcageItems),
		def("DbgGSP_Resolver", &DbgGSP_Resolver),
		def("DbgTestPrint", &DbgTestPrint),

		class_<GPathMark>("GPathMark")
		.def("DebugPosInfo", &GPathMark::DebugPosInfo)
		.def("DebugPosInfo", &GPathMark::DebugPosInfo1),
		def("GPathMarkMe", &GPathMark::GetMe),

		def("TestAutoLvlUpPassiveSkills", &TestAutoLvlUpPassiveSkills),
		def("TestCreateSelectRole", &TestCreateSelectRole),
		def("TestAutoDetectTerrainObjs", &TestAutoDetectTerrainObjs),
		def("TestDetectUniqueTerrain", &TestDetectUniqueTerrain),
		def("TestDetectOneTerrainArea", &TestDetectOneTerrainArea),
		def("TestDetectUniqueTerrainByPos", &TestDetectUniqueTerrainByPos),
		def("TestTerrainSameInfo", &TestTerrainSameInfo),
		def("TestLoginAccount", &TestLoginAccount),
		def("TestActorUseSkill", &TestActorUseSkill),
		def("TestTerminateCurSkill", &TestTerminateCurSkill),
		def("TestGetInGameMainUiDlgCnt", &TestGetInGameMainUiDlgCnt)
#endif
	];
}

void MyLuaThrdData::BindClass()
{
	__super::BindClass();
	GQuestResolver::RegForLua(lstate_);
	CA_IAction::RegForLua(lstate_);
	CSV_Node::RegForLua(lstate_);
	pt_value::RegForLua(lstate_);
	using namespace luabind;
	module(lstate_)[
		class_<GameObjBase, GGameObjBase, GameObjBasePtrT>("GameObjBase"),

		class_<GPlayer, GameObjBase>("GPlayer"),

		class_<GPlayerMe, GPlayer>("GPlayerMe_")
		.def("TestDebug", &GPlayerMe::TestDebug)
		.def("TestChangeMap", &GPlayerMe::TestChangeMap)
		.def("TestCloseUi", &GPlayerMe::TestCloseUi)
		.def("TestTalkToNpc", &GPlayerMe::TestTalkToNpc)
		.def("TestPlayerMove", &GPlayerMe::TestPlayerMove),
		def("GPlayerMe", &GPlayerMe::GetMe),

		class_<GNpc, GameObjBase>("GNpc")
		.def("可访问", &GNpc::NeedVisitable),

		class_<GItemBase, GItemBasePtr>("GItemBase"),

		class_<GWayPointDlgMgr, GGameObjMgrBase>("GSP_village")
		.def("TestWayPoint", &GWayPointDlgMgr::TestWayPoint),
		def("GWayPointDlgMgrMe", &GWayPointDlgMgr::GetMe),

		class_<GMap, GGameObjBase>("GMap")
		.def("DebugSmallAStarPosInfo", &GMap::DebugSmallAStarPosInfo),
		def("GMapMe", &GMap::GetMe),

		class_<GSkillMgr, GGameObjMgrBase>("GSkillMgr"),
		def("GSkillMgrMe", &GSkillMgr::GetMe), 

		class_<GDbMapPinsMgr, GGameObjMgrBase>("GDbMapPinsMgr"),
		def("GDbMapPinsMgrMe", &GDbMapPinsMgr::GetMe),

		class_<GA_SkillBase, CA_IAction>("GA_SkillBase")
		.def("释放间隔", &GA_SkillBase::SetTimeSpan)
		.def("技能CD", &GA_SkillBase::SetCD)
		.def("血值区间", &GA_SkillBase::SetHp)
		.def("蓝值区间", &GA_SkillBase::SetMp)
		.def("施法距离", &GA_SkillBase::SetSkillDistance)
		.def("优先级", &GA_SkillBase::SetPriority)
		.def("边逃边杀", &GA_SkillBase::SetEnsureKill)
		.def("直接施放", &GA_SkillBase::SetUseDirect)
		.def("增加前置技能", &GA_SkillBase::AddPreSkill)
		.def("增加前置技能", &GA_SkillBase::AddPreSkills)
		.def("增加后置技能", &GA_SkillBase::AddSuffixSkill)
		.def("增加后置技能", &GA_SkillBase::AddSuffixSkills),

		class_<GIFilterItem>("GIFilterItem"),
		class_<FilterItemBase, GIFilterItem, GFilterItemPtr>("FilterItemBase"),
		//.def("设置过滤", &FilterItemBase::SetCombine),

		class_<GA_SkillDecorator, GA_SkillBase>("GA_SkillDecorator"),
		class_<GA_Skill, GA_SkillBase>("GA_Skill"),
		class_<GA_SkillToHittable, GA_Skill>("GA_SkillToHittable"),
		class_<GA_SkillToCorpse, GA_Skill>("GA_SkillToCorpse"),

		class_<GTerrainParser>("GTerrainParser")
		.def("AddAny", &GTerrainParser::AddAny)
		.def("AddMap", &GTerrainParser::AddMap)
		.def("AddMap", &GTerrainParser::AddMap_)
		.def("AddMap", &GTerrainParser::AddMap1_)
		.def("AddMapArea", &GTerrainParser::AddMapArea)
		.def("AddMapArea", &GTerrainParser::AddMapArea_),
		def("GetDefTerrainParser", &GTerrainParser::GetDefTerrainParser),

		class_<GItemAgainRinse, GItemAgainRinsePtr>("GitemAginRinse"),
		class_<GItemTypeConfig, GItemAgainRinse>("GItemTypeConfig")
		.def("装备颜色", &GItemTypeConfig::MakeItemColor)
		.def("槽子数量", &GItemTypeConfig::MakeItemSlotCnt)
		.def("槽子连接", &GItemTypeConfig::MakeItemSlotLink)
		.def("插槽颜色", &GItemTypeConfig::MakeItemSlotColor)
		.def("品质", &GItemTypeConfig::MakeItemQuality),

		class_<G_AStar>("G_AStar")
		.def("SetAStarParam", &G_AStar::SetAStarParam)
		.def("DebugPosInfo", &G_AStar::DebugPosInfo)
		.def("DebugPosInfo", &G_AStar::DebugPosInfo1)
		.def("DebugPathInfo", &G_AStar::DebugPathInfo)
		.def("DebugPosRectInfo", &G_AStar::DebugPosRectInfo1)
		.def("DebugPosRectInfo", &G_AStar::DebugPosRectInfo2)
		.def("DebugRectDataInfo", &G_AStar::DebugRectDataInfo1)
		.def("DebugRectPosDataInfo", &G_AStar::DebugRectPosDataInfo1)
		.def("CastToGamePos", &G_AStar::CastToGamePos_),
		def("G_AStarMe", &G_AStar::GetMe)
	];
}

void MyLuaThrdData::BindScriptCommand(luabind::module_& mod)
{
	__super::BindScriptCommand(mod);
	using namespace luabind;
	mod[
		def("KillAllMonster", &KillAllMonster),
		def("KillToOpenObj", &KillToOpenObj),
		def("KillToKillTheObj", &KillToKillTheObj),
		def("KillToKillTheObj", &KillToKillTheObj_),
		def("MoveToTheObj", &MoveToTheObj),
		def("MoveToTheObj", &MoveToTheObj_),
		def("KillToTheRoom", &KillToTheRoom),
		def("WaitRoleCanMove", &WaitRoleCanMove),
		def("FindPakcetItem", &FindPakcetItem),
		def("AutoWearSomeEquips", &AutoWearSomeEquips),
		def("GetRoleLvl", &GetRoleLvl),
		def("SetGlobalDangerDistance", &SetGlobalDangerDistance),
		def("MakeSkill", &MakeSkill),
		def("MakeCorpseSkill", &MakeCorpseSkill),
		def("AddSkill", &AddSkill),
		def("ClearSkills", &ClearSkills),
		def("ClearSkillGemLvlUpInfo", &ClearSkillGemLvlUpInfo),
		def("AddSkillGemLvlUpInfo", &AddSkillGemLvlUpInfo),
		def("SmartMoveMap", &SmartMoveMap),
		def("CloseAllPanel", &CloseAllPanel),
		def("OpenEscapenMenu", &OpenEscapenMenu),
		def("SelectEscapenMenu", &SelectEscapenMenu),
		def("TalkToNpc", &TalkToNpc), 
		def("TakeNpcItem", &TakeNpcItem),
		def("TakeNpcItem", &TakeNpcItem_),
		def("OpenObj", &OpenObj),
		def("OpenObj", &OpenObj_),
		def("FindNearestObj", &FindNearestObj),
		def("FindNearestObj", &FindNearestObj_),
		def("CreateGroupFilter", &CreateGroupFilter),
		def("CreateFilterObj", &CreateFilterObj),
		def("AddStoreFilter", &AddStoreFilter), 
		def("AddPickUpFilter", &AddPickUpFilter),
		def("AddSellFilter", &AddSellFilter),
		def("UseItem", &UseItem),
		def("UseItemToDst", &UseItemToDst), 
		def("KillToTheMap", &KillToTheMap),
		def("KillToTheObj", &KillToTheObj),
		def("KillTokillNpc", &KillToKillNpc),
		def("AutoIdentifiedItem", &AutoIdentifiedItem), 
		def("AutoStoreBagItemToStore", &AutoStoreBagItemToStore),
		def("AutoSellBagItem", &AutoSellBagItem),
		def("EnableAutoOpenChest", &EnableAutoOpenChest),
		def("EnableAutoPickupItem", &EnableAutoPickupItem),
		def("EnableAutoOpenWaypoint", &EnableAutoOpenWaypoint),
		def("EnableAutoOpenTransitionable", &EnableAutoOpenTransitionable), 
		def("EnableAutoLvlUpSkillGemByDef", &EnableAutoLvlUpSkillGemByDef),
		def("EnableAutoWearEquip", &EnableAutoWearEquip),
		def("SetPassiveSkillTree", &SetPassiveSkillTree),
		def("GetItemCnt", &GetItemCnt),
		def("IsOpenWayPointByMap", &IsOpenWayPointByMap),
		def("SetAutoUseFlask", &SetAutoUseFlask),
		def("SetGemLink", &SetGemLink),
		def("ClearAllGemLink", &ClearAllGemLink),
		def("SetWearWeaponType", &SetWearWeaponType), 
		def("HasTheQuestByName", &HasTheQuestByName),
		def("SetMainBagMinFree", &SetMainBagMinFree), 
		def("BuyItemByNpc", &BuyItemByNpc),
		def("PickUpQuestItem", &PickUpQuestItem),
		def("InTown", &InTown),
		def("SelectEnterGame", &SelectEnterGame),
		def("FindNearestMonsterObj", &FindNearestMonsterObj),
		def("SetDefCreateProfessionType", &SetDefCreateProfessionType),
		def("SetDefCreateGameArea", &SetDefCreateGameArea),
		def("SetItemScore", &SetItemScore), 
		def("GetCurDiffLvl", &GetCurDiffLvl),
		def("SetFlaskCnt", &SetBagFlaskCnt),
		def("ClearItemAttrParser", &ClearItemAttrParser),
		def("CreateItemAttrParser", &CreateItemAttrParser),
		def("OpenMapDevice", &OpenMapDevice), 
		def("FindYiJieTransitonable", &FindYiJieTransitonable),
		def("TakeOutYiJieMapFromStorage", &TakeOutYiJieMapFromStorage),
		def("GetItemCntByType", &GetItemCntByType), 
		def("GetCurMapName", &GetCurMapName), 
		def("UseWayReelToMap", &UseWayReelToMap),
		def("SetYiJieFilter", &SetYiJieFilter),
		def("TakeOutItemByName", &TakeOutItemByName), 
		def("SetEmboitementCnt", &SetEmboitementCnt), 
		def("GetCurYiJieMapName", &GetCurYiJieMapName),
		def("CheckStoreEmboitementToBag", &CheckStoreEmboitementToBag),
		def("SetJoinRoomOpenWayReel", &SetJoinRoomOpenWayReel),
		def("ClearFilter", &ClearFilter),
		def("PrintBagItemWeight", &PrintBagItemWeight),
		def("PrintBagItemTipsInfo", &PrintBagItemTipsInfo),
		def("KeepBuyItem", &KeepBuyItem),
		def("AddItemPathway", &AddItemPathway),
		def("UpLevelStashMap", &UpLevelStashMap),
		def("AutoWearSomeGem", &AutoWearSomeGem),
		def("OpenHideoutMapDeviceByCarft", &OpenHideoutMapDeviceByCarft),
		def("AutoAgainRinseEquip", &AutoAgainRinseEquip),
		def("AddAginRinseFilter", &AddAginRinseFilter),
		def("CreateItemTypeRinse", &CreateItemTypeRinse),
		def("KillMoveTo", &KillMoveTo),
		def("KillMoveTo", &KillMoveTo_),
		def("SmartMoveTo", &SmartMoveTo),
		def("CheckStoreGemTotalQualityToBag", &CheckStoreGemTotalQualityToBag),
		def("AdjustPack", &AdjustPack), 
		def("CheckNearestObjAndOpen", &CheckNearestObjAndOpen),
		def("WaitCalledDie", &WaitCalledDie), 
		def("SetResetMap", &SetResetMap),
		def("GetCurChapter", &GetCurChapter),
		def("GetUndoneLabyrinthMap", &GetUndoneLabyrinthMap), 
		def("OpenLabyrinthDevice", &OpenLabyrinthDevice),
		def("GScene_LoginState", &CSH_MakeHandler<GScene_LoginState, CSH_tagPer>),
		def("GScene_PreGameState", &CSH_MakeHandler<GScene_PreGameState, CSH_tagPer>),
		def("GScene_InGameState", &CSH_MakeHandler<GScene_InGameState, CSH_tagPer>),
		def("AutoLoginAccount", &AutoLoginAccount),
		def("KillFollowMovement", &KillFollowMovement)
	];
}

void MyLuaThrdData::BindTest()
{
	__super::BindTest();
	using namespace luabind;
	/*module(lstate_)[
	];*/
}

void MyLuaThrdData::DbgObjs(bool is_alive, const luabind::object& tbl)
{
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return;
	}
	auto obj_mgr_ptr = new GameObjMgr;
	GameObjMgrPtr objs(obj_mgr_ptr);
	if (!objs)
	{
		assert(false);
		return;
	}
	iterator it(tbl), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		obj_mgr_ptr->AddFilterType(object_cast<enGameObjType>(v));
	}
	if (is_alive)
	{
		FilterGameObjAlive filter_alive(objs->GetFilter());
		objs->RebuildAll();
		objs->DebugInfo();
	}
	else
	{
		objs->RebuildAll();
		objs->DebugInfo();
	}
}

bool MyLuaThrdData::KillAllMonster(bool clear_mark_pos_first, float tra_map_rate, int remaining)
{
	if (clear_mark_pos_first)
		GMap::GetMe().ClearAllMarkInfo();
	return GPlayerMe::GetMe().KillAllMonsters(tra_map_rate, remaining);
}

bool MyLuaThrdData::MoveToTheObj(const std::string& obj_name)
{
	return !!GPlayerMe::GetMe().MoveToTheObj(nullptr, true, obj_name);
}

void MyLuaThrdData::AutoWearSomeEquips()
{
	if (!GetAutoWearEquip()) return;
	auto& gpm  = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return;
	}
	AutoIdentifiedItem();
	AutoWearSomeEquipsImpl();
	AutoWearSomeGem();
	return;
}

bool MyLuaThrdData::SetItemColor(int item_color, enCD_ItemColor out_item_color)
{
	auto color = (enCD_ItemColor)item_color;
	if (color <= kIC_Non || color > kIC_Gold)
	{
		LOG_O(Log_debug) << "物品颜色的数值，从0 到 3之间，以此是 白、蓝、黄、金";
		assert(false);
		return false;
	}

	out_item_color = color;
	return true;
}

void MyLuaThrdData::AutoWearSomeEquipsImpl()
{
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return;
	}
	GInventoryMgr::GetMe().AutoWearSomeEquips();
}

int MyLuaThrdData::AutoWearSomeGem()
{
	auto& bagMgr = GInventoryMgr::GetMe();
	if (!bagMgr.AutoTakeOfInvalidGem())
	{
		LOG_O(Log_error) << "自动脱掉身上失效宝石失败";
		return 0;
	}
	AutoWearLinkGem();
	//if (GPlayerMe::GetMe().GetRoleLvl() >= max_Levle_Wear_Surplus_Gem)
	//	return 0;
	//auto bag = bagMgr.GetPackBase(kIT_MainBag);
	//if (!bag)
	//{
	//	assert(false);
	//	return 0;
	//}
	//auto& item = bag->GetItems();
	//if (item.empty())
	//	return 0;

	//std::vector<GItemBasePtr>  GoodGem;
	//for (auto& bagItem : item)
	//{
	//	if (!bagItem->IsGem())
	//		continue;
	//	if (!bagItem->CanWear())
	//		continue;
	//	GoodGem.push_back(bagItem);
	//}
	//if (GoodGem.empty())
	//	return 0;
	//int index(-1);
	//enCD_InventoryType type(kIT_Invalid);
	//for (auto& k : GoodGem)
	//{
	//	if (!bagMgr.FindFreeGemSolt(k->GetItemGemColor(), type, index))
	//		continue;
	//	if (k->InsideGemTo(index, type) < kIGR_Succeed)
	//	{
	//		LOG_O(Log_debug) << "镶嵌宝石失败，宝石名：" << k->GetItemName() << ",位置:" << "(" << index << "," << type << ")";
	//		continue;
	//	}
	//}
	return 0;
}

GItemBasePtr MyLuaThrdData::FindPakcetItem(const std::string& item_name)
{
	if (item_name.empty())
		return nullptr;
	
	auto item = FindPacketItemByName(kIT_MainBag, item_name);
	if (item)
		return item;
	return nullptr;
}
int MyLuaThrdData::GetItemCnt(const std::string& item_name)
{
	auto item = FindPakcetItem(item_name);
	if (!item)
		return 0;
	if (item->GetItemMaxStack() > 0)
		return item->GetItemCnt();
	return 1;
}

GItemBasePtr MyLuaThrdData::FindPacketItemByName(enCD_InventoryType type,const std::string& item_name)
{
	if (item_name.empty())
		return nullptr;
	auto item = GInventoryMgr::GetMe().FindItemByName(item_name, type);
	if (!item.second)
		return nullptr;
	return item.second;
}

bool MyLuaThrdData::AutoSellBagItem(const std::string& npc_name)
{
	return GInventoryMgr::GetMe().AutoSellBagItem(npc_name);
	//	return false;
	//auto main_bag = GInventoryMgr::GetMe().GetPackBase(kIT_MainBag);
	//if (!main_bag)
	//{
	//	assert(false);
	//	return false;
	//}
	//return main_bag->AdjustPack();
}

bool MyLuaThrdData::AutoStoreBagItemToStore()
{
	return GInventoryMgr::GetMe().AutoStoreBagItemToStore();
	//	return false;
	//auto main_bag = GInventoryMgr::GetMe().GetPackBase(kIT_MainBag);
	//if (!main_bag)
	//{
	//	assert(false);
	//	return false;
	//}
	//return main_bag->AdjustPack();
}

DWORD MyLuaThrdData::GetRoleLvl()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	return gpm.GetRoleLvl();
}

bool MyLuaThrdData::SmartMoveMap(const std::string& fb_name, int diff)
{
	if (fb_name.empty())
		return false;
	auto& gpm = GPlayerMe::GetMe();
	if (diff <= 0)
		diff = gpm.GetDiffLvl();
	auto diff_lvl = (enCD_kMapDifficulty)diff;
	if (diff_lvl <= kMD_Invalid || diff_lvl > KMD_Max)
	{
		LOG_O(Log_error) << "移动到指定的副本之难度等级 在 1 -3 之间 ";
		return false;
	}

	return gpm.SmartMoveMap(fb_name, diff_lvl);
}

bool MyLuaThrdData::CloseAllPanel(bool close_npc_panel)
{
	auto& gmp = GPlayerMe::GetMe();
	return gmp.CloseAllPanel(close_npc_panel);
}

bool MyLuaThrdData::OpenEscapenMenu()
{
	auto& gmp = GPlayerMe::GetMe();
	return gmp.OpenEscapeMenu();
}

bool MyLuaThrdData::SelectEscapenMenu(int index)
{
	auto esc_select = (enCD_EscapeMenuButton)index;
	if (esc_select < kEMB_Continue || esc_select > kEMB_LeaveGame)
	{
		LOG_O(Log_error) << "选择ESC面板项之参数必须为数字 1 - 4 ";
		return false;
	}
		
	auto& gmp = GPlayerMe::GetMe();
	return gmp.PressEscapeMenuItem(esc_select);
}

bool MyLuaThrdData::TalkToNpc(const std::string& talk_content)
{
	if (talk_content.empty())
		return false;

	auto& gmp = GPlayerMe::GetMe();
	return gmp.TalkToNpc(talk_content);
}

bool MyLuaThrdData::OpenObj(const std::string& item_name)
{
	if (item_name.empty())
	{
		LOG_O(Log_error) << "打开目标之参数不能为空";
		return false;
	}
	return GPlayerMe::GetMe().MoveToOpenObj(nullptr, true, item_name) >= kOVR_Succeed;
}

bool MyLuaThrdData::OpenObj_(const GameObjBasePtrT& obj)
{
	if (!obj)
	{
		LOG_O(Log_error) << "打开对象之对象不能为空";
		return false;
	}
	return GPlayerMe::GetMe().MoveToOpenObj(nullptr, obj) >= kOVR_Succeed;
}

bool MyLuaThrdData::UseItem(const std::string& item_name)
{
	return GInventoryMgr::GetMe().UseItem(item_name);
}

bool MyLuaThrdData::UseItemToDst(const std::string& src_name, const std::string& dst_name)
{
	return GInventoryMgr::GetMe().UseItemToDst(src_name, dst_name);
	auto& stash_mgr = GPackStashMgr::GetMe();
	if (!stash_mgr.UpdateAllPackPage())
	{
		assert(false);
		return false;
	}
	auto item = stash_mgr.FindItemByAllPack("知识卷轴");
	if (!item)
	{
		assert(false);
		return false;
	}
	GItemStaticContT item_all;
	GFilterItemCont filter_cont;
	FilterItemNotIdentified filter_identified(filter_cont);
	stash_mgr.GetItemByFilter(filter_cont, item_all);
	if (item_all.empty())
	{
		assert(false);
		return false;
	}
	for (auto& i : item_all)
	{
		if (!item->UseTo(*i))
		{
			assert(false);
			return false;
		}
		break;
	}
	return false;
	//return GInventoryMgr::GetMe().UseItemToDst(src_name, dst_name);
}

GameObjBasePtrT MyLuaThrdData::FindNearestObj(enGameObjType obj_type)
{
	return GPlayerMe::GetMe().FindNearestObj(obj_type);
}

GameObjBasePtrT MyLuaThrdData::FindNearestMonsterObj(const std::string& mon_name)
{
	return GPlayerMe::GetMe().FindNearestMonsterObj(mon_name);
}

void MyLuaThrdData::BindConst()
{
	__super::BindConst();
	using namespace luabind;
	module(lua())[
		class_<enGameObjType>("GameObjType")
		.enum_("kGOT_Npc")
		[
			value("kGOT_Npc", kGOT_Npc),
			value("kGOT_Monster", kGOT_Monster),
			value("kGOT_Player", kGOT_Player),
			value("kGOT_Waypoint", kGOT_Waypoint),
			value("kGOT_AreaTransition", kGOT_AreaTransition),
			value("kGOT_Stash", kGOT_Stash),
			value("kGOT_Chest", kGOT_Chest),
			value("kGOT_WorldItem", kGOT_WorldItem)
		],

		class_<enItemFilterType>("过滤类型")
		.enum_("constants")
		[
			value("白名单", kIFT_WhiteName),
			value("黑名单", kIFT_BlackName),
			value("宝石槽数量", kIFT_Slot),
			value("颜色", kIFT_Colour),
			value("宝石槽连接", kIFT_GemLink),
			value("品质", kIFT_Quality), 
			value("物品类型", kIFT_Type),
			value("物品数量", kIFT_ItemCnt),
			value("物品等级", KIFT_Level),
			value("绑定状态", KIFT_Bind),
			value("三色连", KIFT_ThreeColorLink),
			value("套装", KIFT_Emboitement),
			value("属性", kIFT_ItemProp),
			value("装备", kIFT_Equip),
			value("等级", kIFT_Level),
			value("失效宝石", KIFT_InvalidGem)
		],

		class_<enCD_ItemColor>("物品颜色")
		.enum_("constants")
		[
			value("白色", kIC_White),
			value("蓝色", kIC_Blue),
			value("金色", kIC_Yellow),
			value("暗金", kIC_Gold)
		],

		class_<enCD_GemSlotColor>("插槽颜色")
		.enum_("constants")
		[
			value("红色", kGSC_Red),
			value("绿色", kGSC_Green),
			value("蓝色", kGSC_Blue),
			value("白色", KGSC_White)
		],

		class_<enWeaponCompositeCate>("武器类型")
		.enum_("constants")
		[
			value("单手", kWCC_OneHand),
			value("限定位置的", kWCC_Restricted),
			value("双手", kWCC_TwoHand)
		],

		class_<enCD_ItemType>("物品类型")
		.enum_("constants")
		[
			value("主动宝石", kITYPE_ActiveSkillGem),
			value("辅助宝石", kITYPE_SupportSkillGem),
			value("暴力药剂", kITYPE_UtilityFlaskCritical),
			value("可堆叠通货", kITYPE_StackableCurrency),
			value("地图碎片", kITYPE_MapFragment),
			value("异界地图", kITYPE_Map),
			value("珠宝", kITYPE_Jewel), 
			value("命运卡", kITYPE_DivinationCard),

			value("头部", kITYPE_Helmet),
			value("项链", kITYPE_Amulet),
			value("腰带", kITYPE_Belt),
			value("戒指", kITYPE_Ring),
			value("鞋子", kITYPE_Boots),
			value("手套", kITYPE_Gloves),
			value("胸甲", kITYPE_BodyArmour),
			value("盾", kITYPE_Shield),
			value("爪", kITYPE_Claw),
			value("匕首", kITYPE_Dagger),
			value("法杖", kITYPE_Wand),
			value("单手剑", kITYPE_OneHandSword),
			value("细剑", kITYPE_ThrustingOneHandSword),
			value("单手斧", kITYPE_OneHandAxe),
			value("单手锤", kITYPE_OneHandMace),
			value("弓", kITYPE_Bow),
			value("箭袋", kITYPE_Quiver),
			value("长杖", kITYPE_Staff),
			value("双手剑", kITYPE_TwoHandSword),
			value("双手斧", kITYPE_TwoHandAxe),
			value("双手锤", kITYPE_TwoHandMace),
			value("短杖", kITYPE_Sceptre)
		],

		class_<enGradeValue>("评分类型")
		.enum_("constants")
		[
			value("装备颜色", kGV_EquipColor),
			value("槽子连接", kGV_SlotLink),
			value("槽子颜色", kGV_SlotColor),
			value("品质", kGV_Quality),
			value("武器", kGV_Weapon),
			value("属性", kGV_Prop)
		],

		class_<enCD_FlaskType>("药品类型")
		.enum_("constants")
		[
			value("生命", kFT_Hp),
			value("魔力", kFT_Mp),
			value("水银", kFT_Utility)
		],

		class_<enOpenChestFlag>("开箱子标志")
		.enum_("")
		[
			value("开启", kOCF_Enable),
			value("只开显示名字的", kOCF_OnlyShowName)
		],

		class_<enTerrainParserFlag>("enTerrainParserFlag")
		.enum_("")
		[
			value("kTPF_Cmp", kTPF_Cmp),
			value("kTPF_Strstr", kTPF_Strstr),
			value("kTPF_ObjDbPath", kTPF_ObjDbPath)
		]
	];
}

GameObjBasePtrT MyLuaThrdData::FindNearestObj_(const std::string& obj_name)
{
	return GPlayerMe::GetMe().FindNearestObj(obj_name);
}

bool MyLuaThrdData::KillToKillTheObj_(const GameObjBasePtrT& obj)
{
	return GPlayerMe::GetMe().KillToKillTheObj(obj);
}

void MyLuaThrdData::AddStoreFilter(GFilterItemPtr filter_ptr)
{
	if (!filter_ptr)
	{
		LOG_O(Log_error) << "添加存仓过滤失败";
		assert(false);
		return;
	}
	save_Store_filter_.SetFilter(filter_ptr);
	return;
}

void MyLuaThrdData::AddAginRinseFilter(GFilterItemPtr filter_ptr)
{
	if (!filter_ptr)
	{
		LOG_O(Log_error) << "添加洗练过滤失败";
		assert(false);
		return;
	}
	again_Rinse_filter_.SetFilter(filter_ptr);
	return;
}

void MyLuaThrdData::AddSellFilter(GFilterItemPtr filter_ptr)
{
	if (!filter_ptr)
	{
		LOG_O(Log_error) << "添加出售过滤失败";
		assert(false);
		return;
	}
	sell_item_filter_.SetFilter(filter_ptr);
	return;
}

void MyLuaThrdData::AddPickUpFilter(GFilterItemPtr filter_ptr)
{
	if (!filter_ptr)
	{
		LOG_O(Log_error) << "添加拾取过滤失败";
		assert(false);
		return;
	}
	pick_up_filter_.SetFilter(filter_ptr);
}

GFilterItemPtr MyLuaThrdData::CreateFilterObj(enItemFilterType filter_type, const luabind::object& tbl)
{
	auto filter_ptr = GItemFilterFactory::CreateItemFilter(filter_type, tbl);
	if (!filter_ptr)
	{
		assert(false);
		return nullptr;
	}
	return filter_ptr;
}

GFilterItemPtr MyLuaThrdData::CreateGroupFilter(const luabind::object& tbl)
{
	auto group_filter = CreateFilterObj(KIFT_Group, tbl);
	if (!group_filter)
	{
		assert(false);
		return nullptr;
	}
	return group_filter;
}

bool MyLuaThrdData::UseFlack(int index)
{
	if (index < 0 || index > 5)
	{
		LOG_O(Log_error) << "使用药剂之索引从 0 - 4之间";
		assert(false);
		return false;
	}
	
	auto& inventory = GInventoryMgr::GetMe();
	auto flask_bag = inventory.GetPackBase(kIT_Flasks);
	if (!flask_bag)
	{
		assert(false);
		return false;
	}
	stCD_VecInt pos(index, 0);
	auto flask_item = flask_bag->FindItemByPos(pos);
	if (!flask_item)
	{
		assert(false);
		return false;
	}
	return flask_item->Use();
}

void MyLuaThrdData::AutoUseUtilityFlask()
{
	int index = -1;
	if (auto_eat_hp_mp_.CalcUtilityIndex(index))
	{
		if (index < 0)
		{
			assert(false);
			return;
		}
		if (!UseFlack(index))
		{
			LOG_O(Log_error) << "自动使用水银药剂失败";
			assert(false);
			return;
		}
	}
}

void MyLuaThrdData::AutoUseFlack(stCDR_OnHpMp::enValueType value_type)
{
	if (stCDR_OnHpMp::kVT_Hp != value_type && stCDR_OnHpMp::kVT_Mp != value_type)
		return;
	if (!LuasScriptEchoC::IsRunning())
		return;
	auto& gpm = GPlayerMe::GetMe();
	auto old_lock = gpm.IsLockUseFlask();
	gpm.LockUseFlask(true);
	BOOST_SCOPE_EXIT_ALL(old_lock, &gpm){
		gpm.LockUseFlask(old_lock);
	};

	if (!gpm.Update())
	{
		assert(false);
		return;
	}
	int index = -1;
	if (stCDR_OnHpMp::kVT_Hp == value_type)
	{
		if (auto_eat_hp_mp_.CalcHpIndex(index))
		{
			if (index < 0)
			{
				assert(false);
				return;
			}
			if (!UseFlack(index))
			{
				LOG_O(Log_error) << "自动补血失败";
				assert(false);
				return;
			}
		}
		return;
	}
	assert(stCDR_OnHpMp::kVT_Mp == value_type);
	index = -1;
	if (auto_eat_hp_mp_.CalcMpIndex(index))
	{
		if (index < 0)
		{
			assert(false);
			return;
		}
		if (!UseFlack(index))
		{
			LOG_O(Log_error) << "自动补蓝失败";
			assert(false);
			return;
		}
	}
}

bool MyLuaThrdData::AutoIdentifiedItem(bool all_indentified)
{
	auto& inventory = GInventoryMgr::GetMe();
	auto main_bag  = inventory.GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	GFilterItemCont filter_cont;
	FilterItemNotIdentified filter_no_indentified(filter_cont);
	auto iter_filter = main_bag->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (!iter_filter)
	{
		assert(false);
		return false;
	}
	GItemStaticContT	all_item;
	for (auto& item : *iter_filter)
		all_item.push_back(item);
	for (auto& item : all_item)
	{
		if (!main_bag->Update())
		{
			assert(false);
			return false;
		}
		if (!all_indentified && item->IsEquip())
		{
			if (!item->CanWear() || !inventory.NeedTakeupTheDropItem(item))
				continue;
		}
		auto new_item = main_bag->FindItemByPos(item->GetCurItemPos());
		if (!new_item)
		{
			assert(false);
			return false;
		}
		if (!main_bag->IdentifiedOneItem(*new_item))
		{
			LOG_O(Log_debug) << "鉴定物品:" << new_item->GetItemName() << " 失败;";
			return false;
		}
	}
	return true;
}

bool MyLuaThrdData::UseWayReelToMap(const std::string& dst_map_name)
{
	////先看下附近是否有传送点
	//if (InTown())
	//	return true;
	//GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_AreaTransition));
	//if (!obj_mgr)
	//{
	//	assert(false);
	//	return false;
	//}
	//auto chapter = GMap::GetMe().GetChapter();
	//auto twon_name = GSP_Resolver::GetMe().GetCurChapterTownName(chapter);

	//FilterTransferTown filter(obj_mgr->GetFilter());
	////GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_Waypoint));
	////if (!obj_mgr)
	////{
	////	assert(false);
	////	return false;
	////}
	//if (!obj_mgr->RebuildAll())
	//{
	//	assert(false);
	//	return false;
	//}
	//auto transiton = obj_mgr->GetFirstObj();
	//if (transiton && transiton->GetCnName() == twon_name)
	//{
	//	if (!KillToOpenObj(transiton->GetCnName()))
	//	{
	//		assert(false);
	//		return false;
	//	}
	//}
	//return GPlayerMe::GetMe().UseWayReelToMap();
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_Waypoint));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	if (!obj_mgr->RebuildAll())
	{
		assert(false);
		return false;
	}
	if (obj_mgr->GetFirstObj())
		return false;
	return GPlayerMe::GetMe().UseWayReelToMap(dst_map_name);
}

bool MyLuaThrdData::CheckHidePosMap(const std::string& dst_map, enCD_kMapDifficulty diff_lvl)
{
	auto& map = GMap::GetMe();
	auto& gpm = GPlayerMe::GetMe();
	if (!map.IsCangShenChu() && dst_map != kMapName_CangShenChu)
		return false;
	if (map.IsCangShenChu() && dst_map == kMapName_CangShenChu)
		return true;
	auto path_map = map.GetMapName();
	if (map.IsCangShenChu())
		path_map = dst_map;
	GSP_Resolver::MapNameCont sorted_map;
	if (!GSP_Resolver::GetMe().GetTargetMapShortestWayPoint(path_map, gpm.GetDiffLvl(), sorted_map))
	{
		assert(false);
		return false;
	}
	if (sorted_map.empty())
	{
		assert(false);
		return false;
	}
	auto it = sorted_map.rbegin();
	if (!map.IsCangShenChu())
	{
		if (!KillToTheMap(*it, gpm.GetDiffLvl(), true, true))
		{
			assert(false);
			return false;
		}
	}
	if (!KillToOpenObj(kObjName_Waypoint))
	{
		assert(false);
		return false;
	}
	if (dst_map == kMapName_CangShenChu)
		return gpm.MoveHidePosByWayPoint();
	if (!gpm.MoveOtherMapByWayPoint(diff_lvl, *it))
	{
		assert(false);
		return false;
	}
	return false;
}

bool MyLuaThrdData::KillToTheMap(const std::string& dst_map_name, int diff_lvl, bool use_way, bool use_near_way)
{
	if (dst_map_name.empty())
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	if (diff_lvl <= 0)
		diff_lvl = gpm.GetDiffLvl();
	if (diff_lvl <= kMD_Invalid || diff_lvl >= KMD_Max)
	{
		assert(false);
		return false;
	}

	auto& cur_map = GMap::GetMe();
	auto& way_point = GWayPointDlgMgr::GetMe();
	if (!way_point.RebuildAll())
	{
		assert(false);
		return false;
	}
	auto dst_diff_lvl = (enCD_kMapDifficulty)diff_lvl;
	if (CheckHidePosMap(dst_map_name, dst_diff_lvl))
	{
		LOG_O(Log_debug) << "藏身处处理函数已经处理目标移动到地图:" << dst_map_name;
		return true;
	}
	auto end_map_name = dst_map_name;
	way_point.GetWayPointDstName(end_map_name, dst_diff_lvl);
	if (cur_map.GetMapName() == end_map_name && dst_diff_lvl == gpm.GetDiffLvl())
	{
		LOG_O(Log_debug) << "击杀到目标地图 与当前地图 是同一地图";
		return true;
	}
	auto& resolver = GSP_Resolver::GetMe();
	if (!resolver.RebuildAll())
	{
		assert(false);
		return false;
	}
	if (resolver.GetCurChapterTownName(cur_map.GetChapter()) == end_map_name && way_point.IsOpenedWayPoint(dst_map_name, dst_diff_lvl))
	{
		if (use_way && UseWayReelToMap(kEmptyStr))
			return true;
	}
	
	GSP_Resolver::DequeHelp  short_path;
	if (!resolver.GetDijkLatelyPath(end_map_name, dst_diff_lvl, short_path))
	{
		assert(false);
		return false;
	}
	if (short_path.empty())
	{
		LOG_O(Log_error) << "杀怪到某个地图 获取路径失败";
		assert(false);
		return false;
	}
	
#ifdef _DEBUG
	LOG_O(Log_debug) << "开始地图: " << cur_map.GetMapName() << ",  结束地图：" << dst_map_name;
	std::ostringstream ofs;
	for (auto& i : short_path)
		ofs << i.end_name_ << ":" << i.type_ << "->";
	LOG_O(Log_debug) << "路径：" << cur_map.GetMapName() << "->" << ofs.str();
#endif
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_AreaTransition));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	FilterTransferTown filter(obj_mgr->GetFilter());
	if (!obj_mgr->RebuildAll())
	{
		assert(false);
		return false;
	}
	auto way_door = obj_mgr->GetFirstObj();
	if (way_door && use_near_way)
	{
		auto way_name = way_door->GetCnName();
		assert(!way_name.empty());
		auto& tra_mgr = GMapTraMgr::GetMe();
		auto it_map = std::find_if(short_path.begin(), short_path.end(), [&way_name](const GSP_Resolver::stMoveHelp& tmp){return tmp.end_name_ == way_name; });
		if (it_map != short_path.end() && (way_name == kMapName_laboratory || !way_point.IsOpenedWayPoint(way_name, dst_diff_lvl) && !tra_mgr.HasMapAreaByCurMap(way_name)))
		{
			short_path.erase(short_path.begin(), it_map + 1);
			if (!KillToOpenObj(way_name))
			{
				assert(false);
				return false;
			}
		}
	}
	while (!short_path.empty())
	{
		GSP_Resolver::stMoveHelp move_help = short_path.front();
		if (move_help.end_name_.empty())
		{
			short_path.pop_front();
			continue;
		}
		if (!cur_map.Update())
		{
			assert(false);
			return false;
		}
		if (cur_map.GetMapName() == end_map_name)
			break;

		if (move_help.type_ == kGOT_Waypoint)
		{
			if (!KillToOpenObj(kObjName_Waypoint))
			{
				LOG_O(Log_debug) << "KillToOpenObj Faild:" << kObjName_Waypoint;
				return false;
			}
			if (!gpm.MoveOtherMapByWayPoint(move_help.difficulty_, move_help.end_name_))
			{
				assert(false);
				return false;
			}
		}
		else if (move_help.type_ == kGOT_AreaTransition)
		{
			if (!KillToOpenObj(move_help.end_name_))
			{
				LOG_O(Log_debug) << "KillToOpenObj Faild:" << move_help.end_name_;
				//assert(false);
				return false;
			}
		}
		if (!TimerDo(50, 1000 * 60, [&cur_map, &move_help](){
			if (!cur_map.Update())
			{
				assert(false);
				return false;
			}
			return move_help.ori_map_ == cur_map.GetMapName();
		}))
		{
			assert(false);
			return false;
		}
		short_path.pop_front();
	}
	return true;
}

bool MyLuaThrdData::WaitRoleCanMove(pt_dword timeout)
{
	return GPlayerMe::GetMe().WaitRoleCanMove(timeout);
}

bool MyLuaThrdData::KillToOpenObj(const std::string& dst_name, bool remem_find)
{
	return GPlayerMe::GetMe().KillToOpenObj(dst_name, remem_find);
}

bool MyLuaThrdData::KillToKillTheObj(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToKillTheObj( dst_name);
}

bool MyLuaThrdData::KillToTheObj(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToTheObj(dst_name);
}

void MyLuaThrdData::EnableAutoOpenChest(enOpenChestFlag flag)
{
	GPlayerMe::GetMe().auto_open_chest_ = flag;
}

void MyLuaThrdData::EnableAutoPickupItem(bool enable)
{
	GPlayerMe::GetMe().auto_pickup_item_ = enable;
}

void MyLuaThrdData::EnableAutoOpenWaypoint(bool enable)
{
	GPlayerMe::GetMe().auto_open_waypoint_ = enable;
}

void MyLuaThrdData::EnableAutoOpenTransitionable(bool enable)
{
	GPlayerMe::GetMe().auto_open_transitionable_ = enable;
}

void MyLuaThrdData::EnableAutoLvlUpSkillGemByDef(bool enable)
{
	GPlayerMe::GetMe().auto_lvl_up_skill_gem_by_default_ = enable;
}

void MyLuaThrdData::SetAutoUseFlask(float hp_rate, float mp_rate)
{
	auto_eat_hp_mp_.SetRateInfo(hp_rate, mp_rate);
}

bool MyLuaThrdData::IsOpenWayPointByMap(const std::string& map_name)
{
	if (map_name.empty())
	{
		LOG_O(Log_error) << "地图是否开启传送点 地图名字不能为空";
		return false;
	}
	auto& way = GWayPointDlgMgr::GetMe();
	if (!way.RebuildAll())
	{
		assert(false);
		return false;
	}
	auto& gmp = GPlayerMe::GetMe();
	if (!gmp.Update())
	{
		assert(false);
		return false;
	}

	return way.IsOpenedWayPoint(map_name, gmp.GetDiffLvl());
}

void MyLuaThrdData::SetMainBagMinFree(int wight,int hight)
{
	if (wight <= 0 || hight <= 0)
	{
		LOG_O(Log_error) << "设置背包保留格子数量之保留大小不能为空";
		return;
	}
	auto& inventory = GInventoryMgr::GetMe();
	inventory.SetMainPackageRetainSize(stSizeInt(wight, hight));
	return;
}

bool MyLuaThrdData::SetGemLink(pt_dword min_lvl, pt_dword max_lvl, const std::string& main_gem, const luabind::object& tbl)
{
	using namespace luabind;
	if (main_gem.empty())
	{
		LOG_O(Log_error) << "增加宝石连接之主宝石不能为空";
		return false;
	}

	if (LUA_TTABLE != type(tbl))
	{
		LOG_O(Log_error) << "增加宝石连接之第二个参数必须是表类型";
		assert(false);
		return false;
	}

	GAutoUpDateGem::GetMe().AddAutoGemLink(main_gem, min_lvl, max_lvl, tbl);
	return true;
}

void MyLuaThrdData::ClearAllGemLink()
{
	GAutoUpDateGem::GetMe().ClearAllGemLink();
}

bool MyLuaThrdData::AutoWearLinkGem()
{
	GAutoUpDateGem::GemDstPos  wear_gemm_pos;
	GAutoUpDateGem::GetMe().CalcAllGemPos(wear_gemm_pos);
	auto& gpm = GPlayerMe::GetMe();
	auto& bagMgr = GInventoryMgr::GetMe();
	auto main_bag = bagMgr.GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	static GAutoUpDateGem::GemDstPos wear_finish_gem;
	wear_finish_gem.clear();
	GItemBasePtr tmp_gem = nullptr;
	auto it = wear_gemm_pos.begin();
	while (!wear_gemm_pos.empty())
	{
		if (it == wear_gemm_pos.end())
			break;
		if (!tmp_gem)
		{
			auto dst_inventory = kIT_Invalid;
			stCD_VecInt item_pos(-1, -1);
			int index = 0;
			if (bagMgr.FindGemPosByEquip(dst_inventory, item_pos, index, it->gem_name_, wear_finish_gem))
			{
				if (item_pos.AnyLess(0))
				{
					assert(false);
					continue;
				}
				auto iter = std::find_if(wear_gemm_pos.begin(), wear_gemm_pos.end(), [dst_inventory, index, &it](const GAutoUpDateGem::stGemPosInfo& tmp){return tmp.dst_inventory_ == dst_inventory && tmp.index_ == index && tmp.gem_name_ == it->gem_name_; });
				if (iter != wear_gemm_pos.end())
				{
					wear_finish_gem.push_back(*iter);
					wear_gemm_pos.erase(iter);
					it = wear_gemm_pos.begin();
					continue;
				}
				//if (dst_inventory == it->dst_inventory_ && it->index_ == index)
				//{
				//	wear_finish_gem.push_back(*it);
				//	wear_gemm_pos.erase(it);
				//	it = wear_gemm_pos.begin();
				//	continue;
				//}
				if (!gpm.TakeoffGem(dst_inventory, item_pos, index))
				{
					assert(false);
					return false;
				}
			}
			else
			{
				if (!main_bag->Update())
				{
					assert(false);
					return false;
				}
				auto gem_item = main_bag->FindItemByName(it->gem_name_);
				if (!gem_item || !gem_item->CanWear())
				{
					LOG_O(Log_error) << "自动镶嵌宝石之没有找到该宝石，宝石名:" << it->gem_name_;
					return false;
				}
				if (!gpm.TakeupItem(gem_item->GetInventoryType(), gem_item->GetCurItemPos()))
				{
					assert(false);
					return false;
				}
			}
		}
		auto dst_bag = bagMgr.GetPackBase(it->dst_inventory_);
		if (!dst_bag)
		{
			assert(false);
			return false;
		}
		auto equip = dst_bag->GetFirstItem();
		if (!equip)
		{
			assert(false);
			return false;
		}
		if (!gpm.InsideGem(it->dst_inventory_, equip->GetItemId(), it->index_))
		{
			LOG_O(Log_error) << "自动镶嵌宝石之失败 宝石名:" << tmp_gem->GetItemName() << ",目标仓库:" << it->dst_inventory_ << ",目标索引:" << it->index_;
			assert(false);
			return false;
		}
		tmp_gem.reset();
		wear_finish_gem.push_back(*it);
		wear_gemm_pos.erase(it);
		if (GInterface::MouseAttachedItem())
		{
			tmp_gem = GInterface::GetMouseAttachItem();
			if (!tmp_gem)
			{
				assert(false);
				return false;
			}
			if (!tmp_gem->IsGem())
			{
				assert(false);
				return false;
			}
			auto it_tmp = std::find_if(wear_gemm_pos.begin(), wear_gemm_pos.end(), [&tmp_gem](GAutoUpDateGem::stGemPosInfo tmp){return tmp_gem->GetItemName() == tmp.gem_name_; });
			if (it_tmp == wear_gemm_pos.end())
			{
				auto cell_pos = main_bag->FindFreePosBySize(stSizeInt(1, 1));
				if (cell_pos.AllLessZero())
				{
					LOG_O(Log_error) << "自动镶嵌宝石之卸载宝石之背包满，不能卸载;";
					return false;
				}
				if (!gpm.PutDownItem(cell_pos, kIT_MainBag))
				{
					assert(false);
					return false;
				}
				tmp_gem.reset();
			}
			else
			{
				it = it_tmp;
				continue;
			}
		}
		it = wear_gemm_pos.begin();
	}
	return true;
}

bool MyLuaThrdData::TakeNpcItem(const std::string& item_name)
{
	static boost::shared_ptr<GAwardItemPack> award_pack(new GAwardItemPack);
	if (!award_pack->IsOpenAwardPanel())
	{
		assert(false);
		return false;
	}
	if (!award_pack->Update())
	{
		assert(false);
		return false;
	}
	auto item = award_pack->FindItemOrDefault(item_name);
	if (!item)
	{
		assert(false);
		return false;
	}
	return award_pack->NpcItemToMainBagAndWear(*item);
}

bool MyLuaThrdData::TakeNpcItem_(int index)
{
	static boost::shared_ptr<GAwardItemPack> award_pack(new GAwardItemPack);
	if (!award_pack->IsOpenAwardPanel())
	{
		assert(false);
		return false;
	}
	if (!award_pack->Update())
	{
		assert(false);
		return false;
	}
	index = std::max(index, 1);
	auto item = award_pack->FindItemOrDefault(index);
	if (!item)
	{
		assert(false);
		return false;
	}
	return award_pack->NpcItemToMainBagAndWear(*item);
}

void MyLuaThrdData::SetWearWeaponType(enWeaponCompositeCate type)
{
	wear_weapon_type_ = type;
}

void MyLuaThrdData::AddMapRoomRate(const std::string& map_name, int zoom_rate)
{
	G_AStar::GetMe().SetMapZoomRate(map_name, zoom_rate);
}

void MyLuaThrdData::UpdateCurMap()
{
	GMap::GetMe().UpdateCurMap_();
}

void MyLuaThrdData::DbgSortedObjs(bool is_alive, const luabind::object& tbl)
{
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return;
	}
	auto obj_mgr_ptr = new GameObjMgr;
	GameObjMgrPtr objs(obj_mgr_ptr);
	if (!objs)
	{
		assert(false);
		return;
	}
	iterator it(tbl), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		obj_mgr_ptr->AddFilterType(object_cast<enGameObjType>(v));
	}
	FilterGameObjAlive filter_alive(obj_mgr_ptr->GetFilter());
	if (!is_alive)
		objs->GetFilter().Erase(filter_alive);
	if (!objs->RebuildAll())
	{
		assert(false);
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return;
	}
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return;
	}
	objs->SortByWeightDistance(*player_pos);
	objs->DebugInfo();
}

void MyLuaThrdData::DbgPakcageItems(int package_type)
{
	auto package = GInventoryMgr::GetMe().NewPackage((enCD_InventoryType)package_type);
	if (!package)
	{
		assert(false);
		return;
	}
	package->DebugInfo();
}

void MyLuaThrdData::DbgGSP_Resolver(bool is_vertex)
{
	auto& resolver = GSP_Resolver::GetMe();
	if (!resolver.RebuildAll())
	{
		assert(false);
		return;
	}
	if (is_vertex)
		resolver.DbgAllVertices();
	else
		resolver.DbgAllEdges();
}

void MyLuaThrdData::SetMapAreaInfo(const std::string& map_name, const luabind::object& areas)
{
	GMapTraMgr::GetMe().SetMapAreaInfo(map_name, areas);
}

bool MyLuaThrdData::HasTheQuestByName(const std::string& quest_name)
{
	auto& quest_ui = GUiQuestStateInfo::GetMe();
	if (!quest_ui.Update())
	{
		assert(false);
		return false;
	}
	return quest_ui.HasTheQuestByName(quest_name);
}

void MyLuaThrdData::DbgTestPrint(int value)
{
	if (value == 1)
	{
		auto& mgr = GUiQuestStateInfo::GetMe();
		if (!mgr.Update())
		{
			assert(false);
			return;
		}
		LOG_O(Log_debug) << "-----任务状态--------";
		mgr.DebugInfo();
	}
	else if (value == 2)
	{
		auto& gsp = GSP_Resolver::GetMe();
		if (!gsp.RebuildAll())
		{
			assert(false);
			return;
		}
		LOG_O(Log_debug) << "--------------地图DB----------------";
		gsp.DbgAllVertices();
	}
	else if (value == 3)
	{
		LOG_O(Log_debug) << "--------------主背包物品----------------";
		DbgPakcageItems((int)kIT_MainBag);
	}
}

bool MyLuaThrdData::KillToTheRoom(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToTheRoom(dst_name);
}

bool MyLuaThrdData::KillToKillNpc(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToKillNpc(dst_name);
}

int MyLuaThrdData::BuyItemByNpc(const std::string npc_name, const std::string& item_name,int cnt)
{
	if (npc_name.empty() || item_name.empty() || cnt <= 0)
	{
		assert(false);
		return false;
	}
	return GInventoryMgr::GetMe().BuyItemByNpc(npc_name, item_name, cnt);
}


bool MyLuaThrdData::InTown()
{
	auto& map = GMap::GetMe();
	if (!map.Update())
	{
		assert(false);
		return false;
	}
	auto& gsp_resolver = GSP_Resolver::GetMe();
	if (!gsp_resolver.RebuildAll())
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	return gsp_resolver.IsTownByMapName(map.GetMapName(), gpm.GetDiffLvl());
}

void MyLuaThrdData::SetPassiveSkillTree(const luabind::object& job_skill_nodes)
{
	return GPassiveSkillResolver::GetMe().SetNodesInfo(job_skill_nodes);
}

int MyLuaThrdData::TestAutoLvlUpPassiveSkills(int type)
{
	auto& resolver = GPassiveSkillResolver::GetMe();
	if (type == 0)
		resolver.AutoLvlUpPassiveSkills();
	else if (type == 1)
		resolver.DebugInfo(L"女巫", L"超自然领域");
	return 0;
}

bool MyLuaThrdData::PickUpQuestItem()
{
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_WorldItem));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	FilterQuestWorldObj filter(obj_mgr->GetFilter());
	if (!obj_mgr->RebuildAll())
	{
		assert(false);
		return false;
	}
	auto quest_item = obj_mgr->GetFirstObj();
	if (!quest_item)
	{
		//assert(false);
		return false;
	}
	if (!GPlayerMe::GetMe().KillToOpenObj(quest_item))
	{
		LOG_O(Log_debug) << "KillToOpenObj Faild:" << quest_item;
		return false;
	}
	if (!GPackageMgrBase().NewPackage(kIT_MainBag))
	{
		assert(false);
		return false;
	}
	return true;
}

void MyLuaThrdData::TestCreateSelectRole(const std::string& area_name, int profession_type, const std::string& role_name)
{
	GInterface::CreateRole(area_name, (enCD_ProfessionType)profession_type, role_name);
}

bool MyLuaThrdData::SelectEnterGame(const std::string& role_name)
{
	return GInterface::EnterGame(role_name);
}

void MyLuaThrdData::SetDefCreateProfessionType(const std::string& profession_name)
{
	return GetMyApp().SetDefCreateProfessionType(profession_name);
}

void MyLuaThrdData::SetDefCreateGameArea(const std::string& game_area)
{
	return GetMyApp().SetDefCreateGameArea(game_area);
}

void MyLuaThrdData::EnableAutoWearEquip(bool enble)
{
	auto_wear_equip_ = enble;
}

void MyLuaThrdData::SetGlobalDangerDistance(float screen_rate)
{
	GA_TesterDanger::SetDangerDistance(screen_rate);
}

CA_ActionPtr MyLuaThrdData::MakeSkill(const std::string& skill_name)
{
	if (skill_name.empty())
	{
		LOG_O(Log_error) << "技能名不能为空";
		return nullptr;
	}
	return GA_Factory::GetMe().MakeSkill(skill_name);
}

void MyLuaThrdData::AddSkill(GA_SkillBase& skill)
{
	GA_SkillBase::AddSkill(skill);
}

void MyLuaThrdData::ClearSkills()
{
	GA_SkillBase::ClearSkills();
}

void MyLuaThrdData::SetItemScore(enGradeValue score_type, const luabind::object& tbl)
{
	auto& equip_score = GEquipScore::GetMe();
	if (score_type == kGV_EquipColor)
		equip_score.SetColorSocre(tbl);
	else if (score_type == kGV_SlotLink)
		equip_score.SetLinkScore(tbl);
	else if (score_type == kGV_SlotColor)
		equip_score.SetGemSlotRed(tbl);
	else if (score_type == kGV_Quality)
		equip_score.SetQuality(tbl);
	else if (score_type == kGV_Weapon)
		equip_score.SetWeaponSocre(tbl);
	else if (score_type == kGV_Prop)
		equip_score.SetItemPropScore(tbl);
	return;
}

CA_ActionPtr MyLuaThrdData::MakeCorpseSkill(const std::string& skill_name)
{
	if (skill_name.empty())
	{
		LOG_O(Log_error) << "技能名不能为空";
		return nullptr;
	}
	return GA_Factory::GetMe().MakeCorpseSkill(skill_name);
}

int MyLuaThrdData::GetCurDiffLvl()
{
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	return gpm.GetDiffLvl();
}

void MyLuaThrdData::SetBagFlaskCnt(const luabind::object& tbl)
{
	using namespace luabind;
	if (type(tbl) != LUA_TTABLE)
	{
		assert(false);
		return;
	}
	pt_dword tmp_cnt = 0;
	iterator it(tbl), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TTABLE)
			continue;
		auto flask_type = LuaObjCast(v[1], 0);
		auto cnt = LuaObjCast(v[2], 0);
		tmp_cnt += cnt;
		if ( tmp_cnt > kFlaskMaxCnt)
		{
			LOG_O(Log_error) << "设置药品数量之超过可存放最大数量";
			return;
		}
		GPackageFlasks::SetBagFlaskCnt((enCD_FlaskType)flask_type, cnt);
	}
}

bool MyLuaThrdData::MoveToTheObj_(const GameObjBasePtrT& obj)
{
	return !!GPlayerMe::GetMe().MoveToTheObj(nullptr, obj);
}

int MyLuaThrdData::TakeOutYiJieMapFromStorage(int cnt)
{
	FilterItemType filter_type(yi_jie_map_filter_, kITYPE_Map);
	return GInventoryMgr::GetMe().TakeOutStorageItem(yi_jie_map_filter_, cnt);
}

int MyLuaThrdData::TakeOutItemByName(const std::string& item_name, int cnt)
{
	if (item_name.empty())
	{
		assert(false);
		return 0;
	}
	GFilterItemCont filte_cont;
	FilterItemName filter_name(filte_cont, item_name);
	return GInventoryMgr::GetMe().TakeOutStorageItem(filte_cont, cnt);
}

bool MyLuaThrdData::OpenHideoutMapDeviceByCarft(const std::string& craft_name)
{
	if (GPlayerMe::GetMe().MoveToOpenObj(nullptr, true, kObjName_HideoutMapDevice) < kOVR_Succeed)
	{
		LOG_O(Log_error) << "移动到开启地图装置失败";
		assert(false);
		return false;
	}
	static boost::shared_ptr<GHideoutLocatorPack> yijie_pack(new GHideoutLocatorPack);
	if (!yijie_pack)
	{
		assert(false);
		return false;
	}
	if (!yijie_pack->Update())
	{
		assert(false);
		return false;
	}
	cur_yi_jie_map_.clear();
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	GFilterItemCont filter_cont;
	FilterItemType filter_name(filter_cont, kITYPE_Map);
	auto iter_filter = main_bag->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (!iter_filter)
	{
		assert(false);
		return false;
	}
	for (auto& item : *iter_filter)
	{
		auto pos = yijie_pack->FindFreePosBySize(item->GetItemSize());
		if (pos.AnyLess(0))
		{
			LOG_O(Log_error) << "开启地图装置之没有继续放入的位置了";
			break;
		}
		if (item->MoveTo(*yijie_pack, pos) < kMITR_Succeed)
		{
			assert(false);
			return false;
		}
		break;
	}
	yijie_pack->Select(craft_name);
	if (!yijie_pack->StartButton())
	{
		LOG_O(Log_error) << "启动异界装置失败";
		assert(false);
		return false;
	}
	return true;
}

bool MyLuaThrdData::OpenMapDevice(const std::string& map_name)
{
	if (GPlayerMe::GetMe().MoveToOpenObj(nullptr, true, kObjName_MapDevice) < kOVR_Succeed)
	{
		LOG_O(Log_error) << "移动到开启地图装置失败";
		assert(false);
		return false;
	}
	static boost::shared_ptr<GYiJiePack> yijie_pack(new GYiJiePack);
	if (!yijie_pack)
	{
		assert(false);
		return false;
	}
	if (!yijie_pack->Update())
	{
		CloseAllPanel(true);
		assert(false);
		return false;
	}
	cur_yi_jie_map_.clear();
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	GFilterItemCont filter_cont;
	FilterItemType filter_type(filter_cont, kITYPE_Map);
	FilterItemName filter_name(filter_cont, map_name);
	if (map_name.empty())
		filter_cont.Erase(filter_name);
	else
		filter_cont.Erase(filter_type);
	auto iter_filter = main_bag->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (!iter_filter)
	{
		assert(false);
		return false;
	}
	for (auto& item : *iter_filter)
	{
		auto pos = yijie_pack->FindFreePosBySize(item->GetItemSize());
		if (pos.AnyLess(0))
		{
			LOG_O(Log_error) << "开启地图装置之没有继续放入的位置了";
			break;
		}
		if (item->MoveTo(*yijie_pack, pos) < kMITR_Succeed)
		{
			assert(false);
			return false;
		}
		break;
	}
	if (!yijie_pack->StartButton())
	{
		LOG_O(Log_error) << "启动异界装置失败";
		assert(false);
		return false;
	}
	return true;
}

GameObjBasePtrT MyLuaThrdData::FindYiJieTransitonable()
{
	auto& factory = GA_Factory::GetMe();
	auto consumer = factory.MakeSmartConsumer();
	if (!consumer)
	{
		assert(false);
		return false;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_AreaTransition));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	FilterTransferTown filter(obj_mgr->GetFilter());
	auto tester_any_obj = factory.MakeTesterAnyObj(obj_mgr);
	if (!tester_any_obj)
	{
		assert(false);
		return false;
	}
	auto tra_full_map = factory.MakeTraFullMap(consumer, tester_any_obj, false, -1, -1);
	if (!tra_full_map)
	{
		assert(false);
		return false;
	}
	tester_any_obj->SetParent(consumer);
	consumer->Add(tra_full_map);
	const auto& run_res = consumer->Run();
	if (run_res != tester_any_obj)
		return nullptr;
	auto obj_ptr = tester_any_obj->GetFirstObj();
	if (obj_ptr)
		cur_yi_jie_map_.assign(obj_ptr->GetCnName());
	return tester_any_obj->GetFirstObj();
}

int MyLuaThrdData::GetItemCntByType(enCD_ItemType type)
{
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	return main_bag->GetItemCntByType(type);
}

const std::string& MyLuaThrdData::GetCurMapName()
{
	return GMap::GetMe().GetMapName();
}

void MyLuaThrdData::SetYiJieFilter(enItemFilterType filter_type, const luabind::object& tbl)
{
	using namespace luabind;
	if (filter_type == kIFT_BlackName)
	{
		static FilterItemBlackNameList filter_black_name(yi_jie_map_filter_);
		if(!filter_black_name.Parse(tbl))
		{
			assert(false);
			return;
		}
	}
	else if (filter_type == KIFT_Level)
	{
		static FilterItemLessLvl filter_lvl(yi_jie_map_filter_);
		if (!filter_lvl.Parse(tbl))
		{
			assert(false);
			return;
		}
	}	
	else if (filter_type == kIFT_Colour)
	{
		static FilterItemLessEqualColour filter_color(yi_jie_map_filter_);
		if (!filter_color.Parse(tbl))
		{
			assert(false);
			return;
		}
	}
	else if (filter_type == kIFT_ItemProp)
	{
		static FilterItemPropValue filter_prop_value(yi_jie_map_filter_);
		if (!filter_prop_value.Parse(tbl))
		{
			assert(false);
			return;
		}
	}
}

void MyLuaThrdData::SetEmboitementCnt(int cnt)
{
	if (cnt <= 0)
	{
		LOG_O(Log_error) << "设置套装数量之数量最好保持在 1-5 之间(请考虑仓库空间)";
		assert(false);
		return;
	}
	return GInventoryMgr::GetMe().SetEmboitementCnt(cnt);
}

bool MyLuaThrdData::CheckStoreEmboitementToBag()
{
	return GInventoryMgr::GetMe().CheckStoreEmboitementToBag();
}

void MyLuaThrdData::ClearItemAttrParser()
{
	return GItemTipsPanel::ClearItemAttrParser();
}

CSV_NodePtr MyLuaThrdData::CreateItemAttrParser(const std::string& pro_name)
{
	if (pro_name.empty())
	{
		assert(false);
		return nullptr;
	}
	return GItemTipsPanel::CreateItemAttrParser(pro_name);
}

const std::string& MyLuaThrdData::GetCurYiJieMapName()
{
	return cur_yi_jie_map_;
}

void MyLuaThrdData::SetJoinRoomOpenWayReel(bool open)
{
	GPlayerMe::GetMe().room_open_way_reel_ = open;
}

void MyLuaThrdData::ClearFilter(int filter_type)
{
	auto type = (enFilterType)filter_type;
	if (type == kFT_Store)
		save_Store_filter_.ClearFilter();
	else if (type == kFT_Sell)
		sell_item_filter_.ClearFilter();
	else if (type == kFT_PickUp)
		pick_up_filter_.ClearFilter();
	else if (type == KFT_Again)
		again_Rinse_filter_.ClearFilter();
	else
		assert(false);
}

void MyLuaThrdData::PrintBagItemWeight()
{
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return;
	}
	GFilterItemCont filter_cont;
	FilterItemEquip filter_equip(filter_cont);
	auto it_item = main_bag->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (!it_item)
	{
		assert(false);
		return;
	}
	for (auto& i : *it_item)
	{
		LOG_O(Log_info) << "-------------------------------------------------";
		LOG_O(Log_info) << "物品评分计算如下:";
		LOG_O(Log_info) << "物品名字:" << i->GetItemName();
		i->PrintWeight();
		LOG_O(Log_info) << "总分值:" << i->Weight();
	}
	LOG_O(Log_info) << "-------------------------------------------------";
}

void MyLuaThrdData::PrintBagItemTipsInfo()
{
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return;
	}
	GFilterItemCont filter_cont;
	auto it_item = main_bag->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (!it_item)
	{
		assert(false);
		return;
	}
	for (auto& i : *it_item)
	{
		LOG_O(Log_info) << "-------------------------------------------------";
		LOG_O(Log_info) << "物品Tips信息如下:";
		LOG_O(Log_info) << "物品名字:" << i->GetItemName();
		i->GetItemBaseProp().DebugInfo();
		i->GetItemSubjoinProp().DebugInfo();
	}
	LOG_O(Log_info) << "-------------------------------------------------";

}

bool MyLuaThrdData::AddItemPathway(const std::string& item_name, const std::string& town_name, const std::string& npc_name)
{
	if (town_name.empty())
	{
		LOG_O(Log_error) << "添加物品获取方式之第二个参数必须是城镇名字";
		assert(false);
		return false;
	}
	if (npc_name.empty())
	{
		LOG_O(Log_error) << "添加物品获取方式之第三个参数必须是NPC名字";
		assert(false);
		return false;
	}
	if (item_name.empty())
	{
		LOG_O(Log_error) << "添加物品获取方式之第一个参数必须是物品名字";
		assert(false);
		return false;
	}
	return GKeepBuyItem::GetMe().Parse(item_name, town_name, npc_name);
}

int MyLuaThrdData::KeepBuyItem(const std::string& item_name, int item_cnt)
{
	if (item_name.empty() || item_cnt <= 0)
	{
		assert(false);
		return 0;
	}
	auto& keep = GKeepBuyItem::GetMe();
	if (!keep.CanKeepBuyItemCnt(item_name, item_cnt))
		return 0;
	return keep.KeepBuyItem(item_name, item_cnt);
}

bool MyLuaThrdData::UpLevelStashMap(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}
	std::vector<std::string> item_stuff;
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TSTRING)
		{
			assert(false);
			return false;
		}
		auto item_name = LuaObjCast(v, kEmptyStr);
		if (item_name.empty())
		{
			assert(false);
			return false;
		}
		item_stuff.push_back(item_name);
	}
	if (item_stuff.empty())
	{
		assert(false);
		return false;
	}
	if (!GInventoryMgr::GetMe().OpenStoreAndUpdate(false))
	{
		assert(false);
		return false;
	}
	auto& stash_mgr = GPackStashMgr::GetMe();
	GFilterItemCont filter_cont;
	FilterItemColor filter_Color(filter_cont, kIC_White);
	FilterItemType filter_type(filter_cont, kITYPE_Map);
	GItemStaticContT all_item;
	stash_mgr.GetItemByFilter(filter_cont, all_item);
	if (all_item.empty())
		return true;
	if (!GInventoryMgr::GetMe().OpenStoreAndUpdate(true))
	{
		assert(false);
		return false;
	}
	for (auto& v : item_stuff)
	{
		for (auto& k : stash_mgr.GetPages())
		{
			while (true)
			{
				if (all_item.empty())
					return true;
				if (!k->Update())
				{
					assert(false);
					return false;
				}
				auto item = k->FindItemByName(v);
				if (!item)
					break;
				auto item_map = all_item.front();
				if (!item_map)
					return true;
				if (!item->UseTo(*item_map))
				{
					LOG_O(Log_error) << "点亮地图之使用物品失败: 材料名:" << item->GetItemName() << ",地图名字:" << item_map->GetItemName();
					return false;
				}
				all_item.pop_front();
			}
		}
	}
	return false;
}

bool MyLuaThrdData::CheckStoreGemTotalQualityToBag(pt_dword total_quality)
{
	if (total_quality <= 0)
		return false;
	return GInventoryMgr::GetMe().CheckStoreGemTotalQualityToBag(total_quality);
}

void MyLuaThrdData::ClearSkillGemLvlUpInfo()
{
	stCD_UiLvlUpSkillGemItem::ClearLvlUpInfo();
}

void MyLuaThrdData::AddSkillGemLvlUpInfo(const std::string& skill_gem_name, int max_lvl)
{
	if (max_lvl < 0)
		max_lvl = 0;
	stCD_UiLvlUpSkillGemItem::AddLvlUpInfo(GbkToWideChar(skill_gem_name), max_lvl);
}

void MyLuaThrdData::UpdateSelfArea()
{
	GMap::GetMe().UpdateSelfArea_();
}

void MyLuaThrdData::TestAutoDetectTerrainObjs(const luabind::object& obj_names)
{
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return;
	}
	auto& astar = G_AStar::GetMe();
	for (auto& v : obj_names)
	{
		auto obj_name = LuaObjCast(v, kEmptyStr);
		LOG_O(Log_trace) << "准备发现这个对象:" << obj_name;
		if (!MoveToTheObj(obj_name))
		{
			LOG_O(Log_debug) << "移动到目标失败：" << obj_name;
			return;
		}
		auto the_obj = FindNearestObj_(obj_name);
		if (!the_obj)
		{
			assert(false);
			continue;
		}
		stCD_MapDataTerrainModel* model = nullptr;
		stCD_VecInt pos;
		stCD_MapData* map_data = nullptr;
		GWndExecSync([&the_obj, &model, &pos, &map_data](){
			auto w = CD_Interface::GetWorld();
			if (!w)
			{
				assert(false);
				return false;
			}
			map_data = &w->map_data_;
			model = w->map_data_.GetTerrainFrom(the_obj->GetGameEntityBase(), pos);
			if (model)
				model->DebugInfo(pos);
			return true;
		});
		if (!map_data)
		{
			assert(false);
			return;
		}
		if (!model)
		{
			assert(false);
			continue;
		}
		pos = stCD_MapData::ShrinkPos2Pos(pos);
		if (!model->model_.IsValid())
		{
			//assert(false);
			continue;
		}
		auto model_path = model->model_.tbl_item_->model_path_;
		assert(model_path);
		//////////////////////////////////////////////////////////////////////////
		int same_cnt = 0;
		stCD_VecInt pos_tmp, pos_tmp1;
		for (pos_tmp.y = 0; pos_tmp.y < map_data->shrink_size_.y; ++pos_tmp.y)
		{
			auto h = pos_tmp.y * map_data->shrink_size_.x;
			for (pos_tmp.x = 0; pos_tmp.x < map_data->shrink_size_.x; ++pos_tmp.x)
			{
				int idx = h + pos_tmp.x;
				auto model = map_data->terrain_models_ + idx;
				if (!model->model_.IsValid())
					continue;
				if (StdEx::TStrCmp(model->model_.tbl_item_->model_path_, model_path) != 0)
					continue;
				pos_tmp1 = map_data->ShrinkPos2Pos(pos_tmp);
				++same_cnt;
				LOG_O(Log_trace) << "匹配序号：" << same_cnt << ",目标与自己的距离：" << player_pos->Distance(pos_tmp1)
					<< ",目标房间序号：" << (int)astar.GetAreaId(pos_tmp1) << ",自己所在房间序号：" << (int)astar.GetAreaId(*player_pos);
			}
		}
		//////////////////////////////////////////////////////////////////////////
		LOG_O(Log_trace) << "对象名：" << obj_name << ",模型地址：" << model << ",坐标：" << pos
			<< ",模型路径匹配个数：" << same_cnt
			<< ",路径：" << model_path;
		if (GPlayerMe::GetMe().SmartMove(pos))
		{
			LOG_O(Log_trace) << "测试到目标点移动成功";
		}
		else
		{
			LOG_O(Log_debug) << "测试到目标点移动失败";
		}
	}
}

void MyLuaThrdData::TestDetectUniqueTerrain(int radius, int max_cnt)
{
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return;
	}
	auto entity = stCD_GameBase::GetMe().GetPlayerEntity();
	if (!entity)
	{
		assert(false);
		return;
	}
	auto positioned = entity->GetPositioned();
	if (!positioned)
	{
		assert(false);
		return;
	}
	auto w = CD_Interface::GetWorld();
	if (!w)
	{
		assert(false);
		return;
	}
	auto& map_data = w->map_data_;
	stRectInt rc(positioned->pos_, positioned->pos_);
	rc.left_bottom_ -= radius;
	rc.right_top_ += radius;
	rc.left_bottom_ = map_data.Pos2ShrinkPos(rc.left_bottom_);
	rc.right_top_ = map_data.Pos2ShrinkPos(rc.right_top_);
	rc.RectifyAtRange(stCD_VecInt(0, 0), map_data.shrink_size_);
	auto& astar = G_AStar::GetMe();
	G_AStar::PosContT pos_cont_tmp;
	stCD_VecInt pos_tmp, dst_pos_tmp;

	struct stHelp{
		stCD_MapDataTerrainModel*	model_;
		float						weight_;
		stVec2Int					pos_;
	};

	CmnBufferVector<stHelp, 1000> models_tmp;
	for (pos_tmp.y = rc.left_bottom_.y; pos_tmp.y <= rc.right_top_.y; ++pos_tmp.y)
	{
		auto h = pos_tmp.y * map_data.shrink_size_.x;
		for (pos_tmp.x = rc.left_bottom_.x; pos_tmp.x <= rc.right_top_.x; ++pos_tmp.x)
		{
			int idx = h + pos_tmp.x;
			auto model = map_data.terrain_models_ + idx;
			if (!model->model_.IsValid())
				continue;
			auto model_path = model->model_.tbl_item_->model_path_;
			if (!model_path)
			{
				assert(false);
				continue;
			}
			auto same_cnt = map_data.GetSameModelPathCnt(model_path);
			if (same_cnt > max_cnt)
				continue;
			dst_pos_tmp = map_data.ShrinkPos2Pos(pos_tmp);
			if (!astar.GenFindPath(positioned->pos_, dst_pos_tmp, pos_cont_tmp))
				continue;
			models_tmp.push_back({ model, positioned->pos_.Distance(dst_pos_tmp), dst_pos_tmp });
		}
	}
	LOG_O(Log_trace) << "总共有这些个唯一地形：" << models_tmp.size();
	std::sort(models_tmp.begin(), models_tmp.end(), [](const stHelp& lhs, const stHelp& rhs){
		return lhs.weight_ < rhs.weight_;
	});
	stCD_VecInt pos_tmp1;
	for (auto& v : models_tmp)
	{
		LOG_O(Log_trace) << "地形与自己的距离：" << v.weight_
			<< ",目标房间序号：" << (int)astar.GetAreaId(v.pos_) << ",自己所在房间序号：" << (int)astar.GetAreaId(*player_pos)
			<< ",路径：" << v.model_->model_.tbl_item_->model_path_;
		int same_cnt = 0;
		for (pos_tmp.y = 0; pos_tmp.y < map_data.shrink_size_.y; ++pos_tmp.y)
		{
			auto h = pos_tmp.y * map_data.shrink_size_.x;
			for (pos_tmp.x = 0; pos_tmp.x < map_data.shrink_size_.x; ++pos_tmp.x)
			{
				int idx = h + pos_tmp.x;
				auto model = map_data.terrain_models_ + idx;
				if (!model->model_.IsValid())
					continue;
				if (StdEx::TStrCmp(model->model_.tbl_item_->model_path_, v.model_->model_.tbl_item_->model_path_) != 0)
					continue;
				pos_tmp1 = map_data.ShrinkPos2Pos(pos_tmp);
				++same_cnt;
				LOG_O(Log_trace) << "匹配序号：" << same_cnt << ",目标与自己的距离：" << player_pos->Distance(pos_tmp1)
					<< ",目标房间序号：" << (int)astar.GetAreaId(pos_tmp1) << ",自己所在房间序号：" << (int)astar.GetAreaId(*player_pos);
			}
		}
	}
}

void MyLuaThrdData::TestDetectOneTerrainArea(int radius, int terrain_cnt)
{
	GTerrainParser::GetDefTerrainParser().TestDetectOneTerrainArea(radius, terrain_cnt);
}

void MyLuaThrdData::TestDetectUniqueTerrainByPos(const luabind::object& positions)
{
	GTerrainParser::GetDefTerrainParser().TestDetectUniqueTerrainByPos(positions);
}

void MyLuaThrdData::TestTerrainSameInfo(const std::string& terrain_path)
{
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return;
	}
	auto the_terrain_path = GbkToWideChar(terrain_path);
	auto w = CD_Interface::GetWorld();
	if (!w)
	{
		assert(false);
		return;
	}
	auto& map_data = w->map_data_;
	auto& astar = G_AStar::GetMe();
	G_AStar::PosContT pos_cont_tmp;
	stCD_VecInt pos_tmp, dst_pos_tmp;

	int cnt = 0;
	for (pos_tmp.y = 0; pos_tmp.y < map_data.shrink_size_.y; ++pos_tmp.y)
	{
		auto h = pos_tmp.y * map_data.shrink_size_.x;
		for (pos_tmp.x = 0; pos_tmp.x < map_data.shrink_size_.x; ++pos_tmp.x)
		{
			int idx = h + pos_tmp.x;
			auto model = map_data.terrain_models_ + idx;
			if (!model->model_.IsValid())
				continue;
			if (the_terrain_path == model->model_.tbl_item_->model_path_)
			{
				++cnt;
				dst_pos_tmp = map_data.ShrinkPos2Pos(pos_tmp);
				LOG_O(Log_trace) << "地形与自己的距离：" << player_pos->Distance(dst_pos_tmp)
					<< ",目标房间序号：" << (int)astar.GetAreaId(dst_pos_tmp)
					<< ",自己所在房间序号：" << (int)astar.GetAreaId(*player_pos);
			}
		}
	}
	LOG_O(Log_trace) << "总数量：" << cnt;
}

GItemAgainRinsePtr MyLuaThrdData::CreateItemTypeRinse(enCD_ItemType item_type, pt_dword min_lvl, pt_dword max_lvl)
{
	return GItemRinseMgr::CreateItemRinseConfig(item_type, min_lvl, max_lvl);
}

void MyLuaThrdData::AutoAgainRinseEquip()
{
	PackagesContT packages;
	if (!GPackageMgrBase().NewPackage(kIT_MainBag, kIT_Belt, packages))
	{
		assert(false);
		return;
	}
	auto it_pack = std::find_if(packages.begin(), packages.end(), [](const GPackageBasePtr& pack){return pack->GetPackageType() == kIT_MainBag; });
	if (it_pack == packages.end())
	{
		assert(false);
		return;
	}
	auto main_bag = *it_pack;
	assert(main_bag);
	static GItemRinseMgr  rinse_mgr;
	GItemBasePtr dissatisfy_equip[kIT_Flasks] = {};
	for (auto& v : packages)
	{
		if (main_bag == v)
			continue;
		auto equip = v->GetFirstItem();
		if (equip)
		{
			if (rinse_mgr.MeetEquip(*equip))
				continue;
		}
		dissatisfy_equip[v->GetPackageType()] = equip;
	}
	std::vector<enCD_ItemType> item_type_cont;
	for (auto& k : dissatisfy_equip)
	{
		if (!k)continue;
		item_type_cont.push_back(k->GetItemType());
	}

	GItemStaticContT  equip_item;
	auto& bag_item = main_bag->GetItems();
	for (auto& i : bag_item)
	{
		if (!again_Rinse_filter_.DoFilt(*i))
			continue;
		auto cur_type = i->GetItemType();
		auto it = std::find_if(item_type_cont.begin(), item_type_cont.end(), [cur_type](enCD_ItemType type){return type == cur_type; });
		if (it == item_type_cont.end())
			continue;
		item_type_cont.erase(it);
		equip_item.push_back(i);
	}
	if (equip_item.empty())
		return;
#ifdef _DEBUG
	for (auto& i : equip_item)
		LOG_O(Log_debug) << "要洗练的装备:" << i->GetItemName();
#endif

	for (auto& i : equip_item)
	{
		if (!rinse_mgr.CanRinse(*i))
			continue;

		if (!rinse_mgr.BeginRinse(*i))
		{
			LOG_O(Log_debug) << "洗练失败:" << i->GetItemName();
			continue;
		}
		LOG_O(Log_debug) << "洗练成功:" << i->GetItemName();
	}
	return;
}

void MyLuaThrdData::TestLoginAccount(const std::string& ac, const std::string& pwd, const std::string& gateway)
{
	GInterface::LoginAccount(GbkToWideChar(ac), GbkToWideChar(pwd), GbkToWideChar(gateway));
}

bool MyLuaThrdData::AutoLoginAccount()
{
	auto& my_app = GetMyApp();
	const auto& ac_info = my_app.GetAcInfo();
	auto login_res = GInterface::LoginAccount(GbkToWideChar(ac_info.ac_), GbkToWideChar(ac_info.pwd_), GbkToWideChar(ac_info.ser_));
	if (login_res >= kLAR_Succeed)
		return true;
	auto connect = my_app.GetConnection();
	if (!connect)
	{
		assert(false);
		return false;
	}
	if (login_res == kLAR_AcError)
	{
		LOG_O(Log_error) << "帐号不存在";
		DummySleep(2000);
		connect->SendAcPwdErr();
		return false;
	}
	else if (login_res == kLAR_PwdError)
	{
		LOG_O(Log_error) << "密码不正确";
		DummySleep(2000);
		connect->SendAcPwdErr();
		return false;
	}
	else if (login_res == kLAR_AcBaned)
	{
		LOG_O(Log_fatal) << "帐号已疯";
		DummySleep(2000);
		connect->SendBanAc();
		return false;
	}
	assert(false);
	return false;
}

void MyLuaThrdData::TestActorUseSkill(const std::string& skill_name, const stCD_VecInt& dst_pos)
{
	auto the_skill_name = GbkToWideChar(skill_name);
	GWndExecSync([&the_skill_name, &dst_pos](){
		auto player = CD_Interface::GetPlayerEntity();
		if (!player)
		{
			assert(false);
			return false;
		}
		auto actor = player->GetActor();
		if (!actor)
		{
			assert(false);
			return false;
		}
		if (the_skill_name.empty())
		{
			actor->UseSkill(CD_Interface::GetSkillIdMove(), &dst_pos);
		}
		else
		{
			const stCD_SkillInfo* the_skill = nullptr;
			actor->GetMySkillMgr().TraMySkill([&the_skill, &the_skill_name](const stCD_SkillInfo* skill_info){
				auto active_skill = skill_info->GetActiveSkillDb();
				if (!active_skill)
					return true;
				if (active_skill->ch_name_ != the_skill_name)
					return true;
				the_skill = skill_info;
				return false;
			});
			if (!the_skill)
			{
				assert(false);
				return false;
			}
			actor->UseSkill(the_skill->skill_id_, &dst_pos);
		}
		return true;
	});
}

void MyLuaThrdData::TestTerminateCurSkill()
{
	GWndExecSync([](){
		auto logic = CD_Interface::GetGameLogic();
		if (!logic)
		{
			assert(false);
			return false;
		}
		logic->TerminateCurUsingSkill();
		auto entity = CD_Interface::GetPlayerEntity();
		if (!entity)
		{
			assert(false);
			return false;
		}
		auto actor = entity->GetActor();
		if (!actor)
		{
			assert(false);
			return false;
		}
		actor->TerminateCurSkill();
		return true;
	});
}

bool MyLuaThrdData::KillMoveTo(const std::string& name)
{
	return GPlayerMe::GetMe().KillMoveTo(name);
}

bool MyLuaThrdData::KillMoveTo_(const stCD_VecInt& dst_pos)
{
	return GPlayerMe::GetMe().KillMoveTo(dst_pos);
}

bool MyLuaThrdData::KillChangeState()
{
	return true;
}

bool MyLuaThrdData::AdjustPack()
{
	auto main_bag = GInventoryMgr::GetMe().GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	return main_bag->AdjustPack();
}

bool MyLuaThrdData::CheckNearestObjAndOpen(const std::string& obj_name)
{
	auto obj = FindNearestObj_(obj_name);
	if (!obj)
		return false;
	return OpenObj_(obj);
}

bool MyLuaThrdData::WaitCalledDie(bool wait_called)
{
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	auto pos = gpm.GetPos();
	if (!pos)
	{
		assert(false);
		return false;
	}
	auto newpos = *pos;
	auto& buff_mgr = GBuffMgr::GetMe();
	if (wait_called)
	{
		IoTimer::lock_guard locker;
		TimerDo(800, 1000 * 15, [&buff_mgr](){
			if (!buff_mgr.Update())
			{
				assert(false);
				return false;
			}
			return buff_mgr.IsEmpty();
		});
	}
	auto& suggest = GMapTraMgr::GetMe().GetTraGuess();
	auto& area_connections = suggest.GetAreaConnections();
	if (!area_connections.empty())
	{
		auto cur_area = gpm.GetAreaId();
		for (auto& i : area_connections)
		{
			if (i.first->GetAreaId() == cur_area)
				i.second->SetAreaState(kAPS_CanPass);
			if (i.second->GetAreaId() == cur_area)
				i.first->SetAreaState(kAPS_CanPass);
		}
	}
	GInterface::Revive(kRAP_AtRecord);
	return true;
}

bool MyLuaThrdData::SmartMoveTo(const stCD_VecInt& dst_pos)
{
	return GPlayerMe::GetMe().SmartMove(dst_pos);
}

bool MyLuaThrdData::KillFollowMovement()
{
	return GPlayerMe::GetMe().KillFollowMovement();
}

bool MyLuaThrdData::OpenLabyrinthDevice(pt_int select_level)
{
	if (select_level <= 0)
	{
		assert(false);
		return false;
	}
	auto map_name = GetCurMapName();
	if (map_name != kMapName_Aspirants)
	{
		LOG_O(Log_error) << "当前地图不是试炼地图 ";
		return false;
	}
	if (GPlayerMe::GetMe().MoveToOpenObj(nullptr, true, kObjName_LabyrinthDevice) < kOVR_Succeed)
	{
		LOG_O(Log_error) << "移动到开启试炼装置失败";
		assert(false);
		return false;
	}
	auto& labyrinth = GLabyrinthTrialDlg::GetMe();
	if (!labyrinth.RebuildAll())
	{
		assert(false);
		return false;
	}
	return labyrinth.SelectLabyrinthByLevel(select_level);
}

void MyLuaThrdData::SetResetMap(bool reset)
{
	auto_reset_map_ = reset;
}

void MyLuaThrdData::SetLabyrinthLevel(pt_int level)
{
	if (level < 0)
		return;
	open_labyrinth_Lvl_ = level;
}

luabind::object MyLuaThrdData::GetUndoneLabyrinthMap()
{
	using namespace luabind;
	auto& lua_thrd = LuaThrdData::GetMe();
	auto lua = lua_thrd.lua();
	assert(lua);
	object lua_roles(newtable(lua));
	auto& labyrinth = GTrialPlaqueDlg::GetMe();
	auto map_info = labyrinth.GetUndoneMap();
	if (!map_info)
	{
		assert(false);
		return lua_roles;
	}
	lua_roles["地图"] = map_info->map_name_;
	lua_roles["难度"] = map_info->diff_lvl_;
	return lua_roles;
}

void MyLuaThrdData::TestGetInGameMainUiDlgCnt(int flag)
{
	LOG_O(Log_debug) << GInterface::GetInGameMainUiDlgCnt(enCD_TraUiShownFlag(flag));
}

int MyLuaThrdData::GetCurChapter()
{
	return GMap::GetMe().GetChapter();
}

GAutoEatHpMp MyLuaThrdData::auto_eat_hp_mp_((float)0.4, (float)0.3);
//GAutoUpDateGem MyLuaThrdData::auto_update_gem_;

GFilterItemContSmart		MyLuaThrdData::again_Rinse_filter_;
GFilterItemContSmart		MyLuaThrdData::save_Store_filter_;
GFilterItemContSmart		MyLuaThrdData::sell_item_filter_;
GFilterItemContPickUp		MyLuaThrdData::pick_up_filter_;
enWeaponCompositeCate		MyLuaThrdData::wear_weapon_type_ = kWCC_Invalid;
GFilterItemCont				MyLuaThrdData::yi_jie_map_filter_;
std::string					MyLuaThrdData::cur_yi_jie_map_;
bool						MyLuaThrdData::auto_wear_equip_ = true;
bool						MyLuaThrdData::auto_reset_map_ = false;
pt_int						MyLuaThrdData::open_labyrinth_Lvl_ = 50;



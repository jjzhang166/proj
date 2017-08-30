#include "stdafx.h"
#include "Scmd.h"
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
	m[
		def("AddMapRoomRate", &AddMapRoomRate),
		def("UpdateCurMap", &UpdateCurMap),
		def("SetMapAreaInfo", &SetMapAreaInfo)

#ifdef _DEBUG
		,
		def("DbgObjs", &DbgObjs),
		def("DbgSortedObjs", &DbgSortedObjs),
		def("DbgPakcageItems", &DbgPakcageItems),
		def("DbgGSP_Resolver", &DbgGSP_Resolver),

		class_<G_AStar>("G_AStar")
		.def("SetAStarParam", &G_AStar::SetAStarParam)
		.def("DebugPosInfo", &G_AStar::DebugPosInfo)
		.def("DebugPosInfo", &G_AStar::DebugPosInfo1)
		.def("DebugPathInfo", &G_AStar::DebugPathInfo)
		.def("DebugPosRectInfo", &G_AStar::DebugPosRectInfo1)
		.def("DebugRectDataInfo", &G_AStar::DebugRectDataInfo1)
		.def("DebugRectPosDataInfo", &G_AStar::DebugRectPosDataInfo1),
		def("G_AStarMe", &G_AStar::GetMe),

		class_<GPathMark>("GPathMark")
		.def("DebugPosInfo", &GPathMark::DebugPosInfo)
		.def("DebugPosInfo", &GPathMark::DebugPosInfo1),
		def("GPathMarkMe", &GPathMark::GetMe),

		def("TestAutoLvlUpPassiveSkills", &TestAutoLvlUpPassiveSkills),
		def("TestCreateSelectRole", &TestCreateSelectRole)
#endif
	];
	CD_Interface::RegSomeForLua(lstate_);
}

void MyLuaThrdData::BindClass()
{
	__super::BindClass();
	GQuestResolver::RegForLua(lstate_);
	CA_IAction::RegForLua(lstate_);
	using namespace luabind;
	module(lstate_)[
		class_<GameObjBase, GGameObjBase, GameObjBasePtrT>("GameObjBase"),

		class_<GPlayer, GameObjBase>("GPlayer"),

		class_<GPlayerMe, GPlayer>("GPlayerMe_")
		.def("SmartMove", &GPlayerMe::SmartMove)
		.def("TestDebug", &GPlayerMe::TestDebug)
		.def("TestChangeMap", &GPlayerMe::TestChangeMap)
		.def("TestCloseUi", &GPlayerMe::TestCloseUi)
		.def("TestTalkToNpc", &GPlayerMe::TestTalkToNpc)
		.def("TestAvoidPos", &GPlayerMe::TestAvoidPos)
		.def("TestPlayerMove", &GPlayerMe::TestPlayerMove),
		def("GPlayerMe", &GPlayerMe::GetMe),

		class_<GNpc, GameObjBase>("GNpc")
		.def("可访问", &GNpc::NeedVisitable),

		class_<GItemBase, GItemBasePtr>("GItemBase"),

		//class_<GSP_Resolver, GGameObjMgrBase>("GSP_village")
		//.def("DijkFindPath", &GSP_Resolver::TestDijkFindPath),
		//def("GSP_Resolver", &GSP_Resolver::GetMe),

		class_<GWayPointDlgMgr, GGameObjMgrBase>("GSP_village")
		.def("TestWayPoint", &GWayPointDlgMgr::TestWayPoint),
		def("GWayPointDlgMgrMe", &GWayPointDlgMgr::GetMe),

		class_<GMap, GGameObjBase>("GMap"),
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
		class_<GA_SkillToCorpse, GA_Skill>("GA_SkillToCorpse")
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
		def("UseWayToTown", &UseWayToTown),
		def("SetYiJieFilter", &SetYiJieFilter),
		def("TakeOutItemByName", &TakeOutItemByName), 
		def("SetEmboitementCnt", &SetEmboitementCnt), 
		def("CheckStoreEmboitementToBag", &CheckStoreEmboitementToBag),
		def("GScene_PreGameState", &CSH_MakeHandler<GScene_PreGameState, CSH_tagPer>),
		def("GScene_InGameState", &CSH_MakeHandler<GScene_InGameState, CSH_tagPer>)
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
		obj_mgr_ptr->AddFilterType(object_cast<GameObjType>(v));
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

bool MyLuaThrdData::KillAllMonster(bool clear_mark_pos_first)
{
	if (clear_mark_pos_first)
		GMap::GetMe().ClearAllMarkInfo();
	return GPlayerMe::GetMe().KillAllMonsters();
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
	auto bag = bagMgr.GetPackBase(kIT_MainBag);
	if (!bag)
	{
		assert(false);
		return 0;
	}
	auto& item = bag->GetItems();
	if (item.empty())
		return 0;

	std::vector<GItemBasePtr>  GoodGem;
	for (auto& bagItem : item)
	{
		if (!bagItem->IsGem())
			continue;
		if (!bagItem->CanWear())
			continue;
		GoodGem.push_back(bagItem);
	}
	if (GoodGem.empty())
		return 0;
	int index(-1);
	enCD_InventoryType type(kIT_Invalid);
	for (auto& k : GoodGem)
	{
		if (!bagMgr.FindFreeGemSolt(k->GetItemGemColor(), type, index))
			continue;
		if (k->InsideGemTo(index, type) < kIGR_Succeed)
		{
			LOG_O(Log_debug) << "镶嵌宝石失败，宝石名：" << k->GetItemName() << ",位置:" << "(" << index << "," << type << ")";
			continue;
		}
	}
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
	if (!item)
		return nullptr;
	return item;
}

bool MyLuaThrdData::AutoSellBagItem()
{
	return GInventoryMgr::GetMe().AutoSellBagItem();
}

bool MyLuaThrdData::AutoStoreBagItemToStore()
{
	if (!GInventoryMgr::GetMe().AutoStoreBagItemToStore())
		return false;
	auto main_bag = GInventoryMgr::GetMe().GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	return main_bag->AdjustPack();
}

DWORD MyLuaThrdData::GetRoleLvl()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	return gpm.GetRoleLvl();
}

enCD_InventoryType MyLuaThrdData::IndexToPackType(int index)
{
	if (1 == index)
		return kIT_MainBag;
	else if (2 == index)
		return kIT_BodyArmour;
	else if (3 == index)
		return kIT_WeaponLeft;
	else if (4 == index)
		return kIT_WeaponRight;
	else if (5 == index)
		return kIT_Helm;
	else if (6 == index)
		return kIT_Necklace;
	else if (7 == index)
		return kIT_RingLeft;
	else if (8 == index)
		return kIT_RingRight;
	else if (9 == index)
		return kIT_Glove;
	else if (10 == index)
		return kIT_Shoe;
	else if (11 == index)
		return kIT_Belt;
	else if (12 == index)
		return kIT_Flasks;
	else if (15 == index)
		return kIT_WeaponLeftHide;
	else if (16 == index)
		return kIT_WeaponRightHide;
	else if (31 == index)
		return kIT_StashTab_1;
	else if (32 == index)
		return kIT_StashTab_2;
	else if (33 == index)
		return kIT_StashTab_3;
	else if (34 == index)
		return kIT_StashTab_4;
	else
		return kIT_Invalid;
}

bool MyLuaThrdData::SmartMoveMap(const std::string& fb_name, int diff)
{
	
	if (fb_name.empty())
		return false;

	auto diff_lvl = (enCD_kMapDifficulty)diff;
	if (diff_lvl <= kMD_Invalid || diff_lvl > KMD_Max)
	{
		LOG_O(Log_error) << "移动到指定的副本之难度等级 在 1 -3 之间 ";
		return false;
	}

	auto& gmp = GPlayerMe::GetMe();
	return gmp.SmartMoveMap(fb_name, diff_lvl);

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
	return GPlayerMe::GetMe().MoveToOpenObj(nullptr, false, obj) >= kOVR_Succeed;
}

bool MyLuaThrdData::UseItem(const std::string& item_name)
{
	return GInventoryMgr::GetMe().UseItem(item_name);
}

bool MyLuaThrdData::UseItemToDst(const std::string& src_name, const std::string& dst_name)
{
	return GInventoryMgr::GetMe().UseItemToDst(src_name, dst_name);
}

GameObjBasePtrT MyLuaThrdData::FindNearestObj(GameObjType obj_type)
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
		class_<GameObjType>("GameObjType")
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

		class_<en_ItemFilterType>("过滤类型")
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
			value("物品属性", KIFT_Prop),
			value("绑定状态", KIFT_Bind),
			value("三色连", KIFT_ThreeColorLink),
			value("套装", KIFT_Emboitement)
			
			
		],

		class_<enCD_ItemColor>("物品颜色")
		.enum_("constants")
		[
			value("白色", kIC_White),
			value("蓝色", kIC_Blue),
			value("黄色", kIC_Yellow),
			value("金色", kIC_Gold)
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
			value("命运卡", kITYPE_DivinationCard)
		],

		class_<en_NpcMonster>("NPC怪操作")
		.enum_("constants")
		[
			value("击败", kNM_BeatFailure),
			value("帮助", kNM_Help),
			value("访问", kITYPE_UtilityFlaskCritical)
		],

		class_<en_GradeValue>("评分类型")
		.enum_("constants")
		[
			value("装备颜色", kGV_EquipColor),
			value("槽子连接", kGV_SlotLink),
			value("槽子颜色", kGV_SlotColor),
			value("品质", kGV_Quality)
		],

		class_<enCD_FlaskType>("药品类型")
		.enum_("constants")
		[
			value("生命", kFT_Hp),
			value("魔力", kFT_Mp),
			value("水银", kFT_Utility)
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

GFilterItemPtr MyLuaThrdData::CreateFilterObj(en_ItemFilterType filter_type, const luabind::object& tbl)
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
	auto& gmp = GPlayerMe::GetMe();
	if (!gmp.Update())
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

bool MyLuaThrdData::AutoIdentifiedItem()
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
		if (item->IsEquip())
		{
			if (!inventory.NeedTakeupTheDropItem(item))
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
			LOG_O(Log_error) << "鉴定物品:" << new_item->GetItemName() << " 失败;";
			return false;
		}
	}
	//while (true)
	//{
		//if (!main_bag->Update())
		//{
		//	assert(false);
		//	return false;
		//}
		//auto item = main_bag->GetOneWaitIdentifiedItem();
		//if (!item)
		//	break;
		//if (!main_bag->IdentifiedOneItem(*item))
		//	return false;
		//if (item->IsIdentified())
		//{
		//	assert(false);
		//	break;
		//}
		//auto indentify_item = main_bag->FindItemByName(kObjName_Identify);
		//if (!indentify_item)
		//{
		//	LOG_O(Log_error) << "自动鉴定之没有知识卷轴了";
		//	break;
		//}
		//if (!gpm.UseItem(*indentify_item, *item))
		//{
		//	LOG_O(Log_error) << "自动鉴定物品 失败，物品：" << item->GetItemName();
		//	break;
		//}
	//}
	return true;
}

bool MyLuaThrdData::UseWayToTown()
{
	return GPlayerMe::GetMe().UseWayToTown();
}

bool MyLuaThrdData::KillToTheMap(const std::string& dst_map_name, int diff_lvl, bool use_way)
{
	if (dst_map_name.empty())
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
	if (diff_lvl == kMD_Invalid)diff_lvl = gmp.GetDiffLvl();
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
	//auto cur_diff_lvl = gmp.GetDiffLvl();
	auto end_map_name = dst_map_name;
	way_point.GetWayPointDstName(end_map_name, dst_diff_lvl);
	if (cur_map.GetMapName() == end_map_name && dst_diff_lvl == gmp.GetDiffLvl())
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
		if (use_way && UseWayToTown())
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
			if (!gmp.MoveOtherMapByWayPoint(move_help.difficulty_, move_help.end_name_))
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

bool MyLuaThrdData::KillToOpenObj(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToOpenObj(dst_name);
}

bool MyLuaThrdData::KillToKillTheObj(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToKillTheObj( dst_name);
}

bool MyLuaThrdData::KillToTheObj(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToTheObj(dst_name);
}

void MyLuaThrdData::EnableAutoOpenChest(bool enable)
{
	GPlayerMe::GetMe().auto_open_chest_ = enable;
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

bool MyLuaThrdData::SetGemLink(const std::string& main_gem, const luabind::object& tbl)
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

	auto_update_gem_.AddAutoGemLink(main_gem, tbl);
	return true;
}

void MyLuaThrdData::ClearAllGemLink()
{
	auto_update_gem_.ClearAllGemLink();
}

bool MyLuaThrdData::AutoWearLinkGem()
{
	GAutoUpDateGem::GemDstPos  wear_gemm_pos;
	auto_update_gem_.CalcAllGemPos(wear_gemm_pos);
	auto& gpm = GPlayerMe::GetMe();
	auto& bagMgr = GInventoryMgr::GetMe();
	auto main_bag = bagMgr.GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	GItemBasePtr tmp_gem;
	auto it = wear_gemm_pos.begin();
	while (!wear_gemm_pos.empty())
	{
		if (it == wear_gemm_pos.end())
			break;
		if (!main_bag->Update())
		{
			assert(false);
			return false;
		}
		if (!tmp_gem)
		{
			auto gem_item = main_bag->FindItemByName(it->gem_name_);
			if (!gem_item || !gem_item->CanWear())
			{
				auto dst_inventory = kIT_Invalid;
				stCD_VecInt item_pos(-1,-1);
				int index = 0;
				if (!bagMgr.FindGemPosByEquip(dst_inventory, item_pos, index, it->gem_name_))
				{
					LOG_O(Log_error) << "自动镶嵌宝石之没有找到该宝石，宝石名:" << it->gem_name_;
					return false;
				}
				if (item_pos.AnyLess(0))
					continue;
				if (dst_inventory == it->dst_inventory_ && it->index_ == index)
				{
					wear_gemm_pos.erase(it);
					it = wear_gemm_pos.begin();
					continue;
				}
				if (!gpm.TakeoffGem(dst_inventory, item_pos, index))
				{
					assert(false);
					return false;
				}
			}
			else
			{
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
		obj_mgr_ptr->AddFilterType(object_cast<GameObjType>(v));
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
	GMap::GetMe().SetMapAreaInfo(map_name, areas);
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

bool MyLuaThrdData::KillToTheRoom(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToTheRoom(dst_name);
}

bool MyLuaThrdData::KillToKillNpc(const std::string& dst_name)
{
	return GPlayerMe::GetMe().KillToKillNpc(dst_name);
}

bool MyLuaThrdData::BuyItemByNpc(const std::string npc_name, const std::string& item_name)
{
	if (npc_name.empty() || item_name.empty())
	{
		assert(false);
		return false;
	}
	return GInventoryMgr::GetMe().BuyItemByNpc(npc_name, item_name);
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

void MyLuaThrdData::SetItemScore(en_GradeValue score_type, const luabind::object& tbl)
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
	return !!GPlayerMe::GetMe().MoveToTheObj(nullptr, false, obj);
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

bool MyLuaThrdData::OpenMapDevice()
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
		assert(false);
		return false;
	}
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
	auto tra_full_map = factory.MakeTraFullMap(consumer, tester_any_obj, false);
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

void MyLuaThrdData::SetYiJieFilter(en_ItemFilterType filter_type, const luabind::object& tbl)
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
	else if (filter_type == KIFT_Prop)
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

CSV_NodePtr MyLuaThrdData::CreateItemAttrParser(const luabind::object& fn)
{
	return GItemTipsPanel::CreateItemAttrParser(fn);
}

GAutoEatHpMp MyLuaThrdData::auto_eat_hp_mp_((float)0.4, (float)0.3);
GAutoUpDateGem MyLuaThrdData::auto_update_gem_;

GFilterItemContSmart	 MyLuaThrdData::save_Store_filter_;
GFilterItemContSmart	 MyLuaThrdData::sell_item_filter_;
//GFilterItemContForLuaFalse MyLuaThrdData::pick_up_filter_;
GFilterItemContPickUp		MyLuaThrdData::pick_up_filter_;

enWeaponCompositeCate MyLuaThrdData::wear_weapon_type_ = kWCC_Invalid;
GFilterItemCont		MyLuaThrdData::yi_jie_map_filter_;
bool MyLuaThrdData::auto_wear_equip_ = true;


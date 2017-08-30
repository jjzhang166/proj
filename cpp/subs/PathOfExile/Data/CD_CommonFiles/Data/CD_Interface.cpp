#include "stdafx.h"
#include "CD_Interface.h"
#include <CmnMix/Cmn_LogOut.h>
#include "CD_FuncCall.h"
#include <Script/LuaScript.h>
#include <CommonX/CmnX_DllApi.h>

using namespace CD_Interface;
InterfaceSink* g_interface_sink_ = nullptr;
void CD_Interface::DebugStateInfo()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (!ptr)
		return;
	ptr->DebugInfo();
}

void CD_Interface::DebugAddrInfo()
{
	
}

void CD_Interface::DebugSendPacket()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (!ptr)
		return;
	LOG_O(Log_info) << ptr->GetSendPacket();
}

void CD_Interface::RegDbgInfoForLua(lua_State* l)
{
	using namespace luabind;
	module(l, "CD_Interface")
		[
			def("DebugStateInfo", &DebugStateInfo),
			def("DebugAddrInfo", &DebugAddrInfo),
			def("DebugSendPacket", &DebugSendPacket),
			def("DebugPlayerInfo", &DebugPlayerInfo),
			def("DebugPlayerEntityInfo", &DebugPlayerEntityInfo),
			def("DebugWorldInfo", &DebugWorldInfo),
			def("DebugGameBaseWndInfo", &DebugGameBaseWndInfo),
			def("DebugPlayerPosInfo", &DebugPlayerPosInfo),
			def("DebugDstMapDataInfo", &DebugDstMapDataInfo),
			def("DebugEntityObjs", &DebugEntityObjs),
			def("DebugPlayerAttrInfo", &DebugPlayerAttrInfo),
			def("DebugPassiveSkill", &DebugPassiveSkill),
			def("DebugDbInfo", &DebugDbInfo),
			def("DebugDbPassiveSkill", &DebugDbPassiveSkill),
			def("DebugDbQuest", &DebugDbQuest),
			def("DebugDbQuestStates", &DebugDbQuestStates),
			def("DebugDbActiveSkills", &DebugDbActiveSkills),
			def("DebugDbBaseItemTypes", &DebugDbBaseItemTypes),
			def("DebugDbItemClasses", &DebugDbItemClasses),
			def("DebugDbSkillGems", &DebugDbSkillGems),
			def("DebugDbGenTags", &DebugDbGenTags),
			def("DebugDbGrantedEffects", &DebugDbGrantedEffects),
			def("DebugInventoryItems", &DebugInventoryItems),
			def("DebugFindGridPanelFromDlgProp", &DebugFindGridPanelFromDlgProp),
			def("DebugDlgItems", &DebugDlgItems),
			def("DebugBodyItems", &DebugBodyItems),
			def("DebugEquipsInsideGem", &DebugEquipsInsideGem),
			def("DebugBodyEquipsGem", &DebugBodyEquipsGem),
			def("DebugSkillGems", &DebugSkillGems),
			def("DebugGemSkill", &DebugGemSkill),
			def("DebugBodyGemSkill", &DebugBodyGemSkill),
			def("DebugInventorySkill", &DebugInventorySkill),
			def("DebugMySkillInfo", &DebugMySkillInfo),
			def("DebugTheObjSkillInfo", &DebugTheObjSkillInfo),
			def("DebugCurQuestInfo", &DebugCurQuestInfo),
			def("DebugDbWorldArea", &DebugDbWorldArea),
			def("DebugDbStats", &DebugDbStats),
			def("DebugDbMapConnections", &DebugDbMapConnections),
			def("DebugDbMapPins", &DebugDbMapPins),
			def("DebugDbWeaponTypes", &DebugDbWeaponTypes),
			def("DebugDbClientStrings", &DebugDbClientStrings),
			def("DebugDbPassiveSkillGraph", &DebugDbPassiveSkillGraph),
			def("DebugDbChests", &DebugDbChests),
			def("DebugMonsterTypes", &DebugMonsterTypes),
			def("DebugMonsterVarieties", &DebugMonsterVarieties),
			def("DebugMapModels", &DebugMapModels),
			def("DbgPlayerPosModel", &DbgPlayerPosModel),
			def("DebugInGameState", &DebugInGameState),
			def("DebugDlgWaypoint", &DebugDlgWaypoint),
			def("DebugUiWaypointItems", &DebugUiWaypointItems),
			def("DebugTheWaypointItem", &DebugTheWaypointItem),
			def("DebugUiFullInfo", &DebugUiFullInfo),
			def("DebugTheUiInfo", &DebugTheUiInfo),
			def("DebugTheParentUiInfo", &DebugTheParentUiInfo),
			def("DebugInGameMainUiAddrInfo", &DebugInGameMainUiAddrInfo),
			def("DebugNpcPanel", &DebugNpcPanel),
			def("DebugNpcPanelMenuItemInfo", &DebugNpcPanelMenuItemInfo),
			def("DebugQuestAwardInventory", &DebugQuestAwardInventory),
			def("DebugUiQuestStateItem", &DebugUiQuestStateItem),
			def("DebugUiQuestItem", &DebugUiQuestItem),
			def("DebugCallerBuff", &DebugCallerBuff),
			def("DebugQuestLvlStatePanel", &DebugQuestLvlStatePanel),
			def("DebugDlgSelfMapLocator", &DebugDlgSelfMapLocator),
			def("DebugLoginState", &DebugLoginState),
			def("DebugPreGameState", &DebugPreGameState),
			def("DebugSelectRoleTop", &DebugSelectRoleTop),
			def("DebugUiBottom", &DebugUiBottom),
			def("DebugTheDbWorldArea", &DebugTheDbWorldArea),
			def("DebugDlgLabyrinthSelect", &DebugDlgLabyrinthSelect),
			def("DebugDlgTrialPlaque", &DebugDlgTrialPlaque),
			def("DebugDlgStash", &DebugDlgStash),
			def("DebugDlgSelectRole", &DebugDlgSelectRole),
			def("DebugCreateCharacterState", &DebugCreateCharacterState)
		];
}

void CD_Interface::TraverseState(const TraFn_Scene& fn)
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (!ptr)
	{
		assert(false);
		return;
	}
	return ptr->TraverseState(fn);
}

void CD_Interface::TestCreateRole()
{
	stCDS_CreateRole msg;
	msg.role_name_ = L"woyaoaiai1";
	msg.area_name_ = L"裂隙赛季";
	//msg.ukn_stack_addr_ = 0x0018E880;
	msg.str_name_ = L"StrDexIntDefault";
	Send(msg);
}

void CD_Interface::RegTestForLua(lua_State* l)
{
	using namespace luabind;
	module(l, "CD_Interface")
		[
			def("TestCreateRole", &TestCreateRole),
			def("TestHooks", &TestHooks),
			def("TestFindPath", &TestFindPath),
			def("TestPressNpcItem", &TestPressNpcItem),
			def("TestRecvOutDebugInfo", &TestRecvOutDebugInfo),
			def("TestAnalysisRecvPacket", &TestAnalysisRecvPacket),
			def("TestAnalysisSendPacket", &TestAnalysisSendPacket),
			def("TestAnalysisSendPacketBlocked", &TestAnalysisSendPacketBlocked),
			def("TestShowUi", &TestShowUi),
			def("TestShowUi", &TestShowUi_)
		];
}

void CD_Interface::TestHooks(int tag)
{
	//typedef CD_CMDR_Serialize<stCDR_CreateRole> CmdCreateRole;
	static Hooks hooks;
	//hooks.AddCmd<stCDR_MsgCreateRole>();
	if (1 == tag)
		hooks.HookRecvPacketRead();
	else if (2 == tag)
		hooks.HookCheckPacket_Heartbeat();
	else if (3 == tag)
		hooks.HookOnMoveToAvoidDamage();
}

stCD_ScenePreGameState* CD_Interface::GetPreGameState()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	return ptr->GetPreGameState();
}

stCD_SceneInGameState* CD_Interface::GetInGameState()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	return ptr->GetInGameState();
}

stCD_PlayerMe* CD_Interface::GetPlayer()
{
	return stCD_GameBase::GetMe().GetPlayer();
}

void CD_Interface::DebugPlayerInfo()
{
	auto p = GetPlayer();
	if (!p)
	{
		assert(false);
		return;
	}
	p->DebugInfo();
}

void CD_Interface::DebugPlayerEntityInfo()
{
	auto c = GetPlayerEntity();
	if (c)
		c->DebugInfo();
}

void CD_Interface::DebugWorldInfo()
{
	auto p = GetWorld();
	if (!p)
		return;
	p->DebugInfo();
}

stCD_World* CD_Interface::GetWorld()
{
	return stCD_GameBase::GetMe().GetWorld();
}

void CD_Interface::DebugGameBaseWndInfo()
{
	auto ptr = stCD_GameBaseWndInfo::GetInstance();
	if (!ptr)
		return;
	ptr->DebugInfo();
}

HWND CD_Interface::GetGameWndHandle()
{
	auto ptr = stCD_GameBaseWndInfo::GetInstance();
	if (!ptr)
		return nullptr;
	auto handle = ptr->wnd_handle_;
	if (!handle)
		return nullptr;
	if (!::IsWindow(handle))
	{
		//assert(false);
		return nullptr;
	}
	return handle;
}

void CD_Interface::DebugPlayerPosInfo()
{
	auto p = GetPlayerEntity();
	if (!p)
		return;
	auto pos = p->GetPositioned();
	if (!pos)
		return;
	LOG_O(Log_debug) << pos->pos_;
}

void CD_Interface::DebugDstMapDataInfo(const stCD_VecInt& pos, bool is_src_map_data)
{
	auto w = GetWorld();
	if (!w)
		return;
	auto addr_info = w->map_data_.GetDstMapDataAddr(pos, is_src_map_data);
	LOG_O(Log_debug) << "data:" << (int)w->map_data_.GetDstMapData(pos, is_src_map_data)
		<< ",addr:" << addr_info.first << ",addr_data:" << (int)addr_info.second;
}

stCD_Positioned* CD_Interface::GetPlayerPositioned()
{
	auto ptr = GetPlayerEntity();
	if (!ptr)
		return nullptr;
	auto res = ptr->GetPositioned();
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_Life* CD_Interface::GetPlayerLife()
{
	auto ptr = GetPlayerEntity();
	if (!ptr)
		return nullptr;
	auto res = ptr->GetLife();
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res;
}

void CD_Interface::TestFindPath(int x, int y)
{
	assert(false);
	/*PosContT pos_info;
	if (!GenFindPath(stCD_VecInt(x, y), pos_info))
	{
		assert(false);
		return;
	}
	LOG_STM(stm, Log_debug);
	for (auto& v : pos_info)
	{
		stm << v;
	}*/
}

stCD_Pathfinding* CD_Interface::GetPlayerPathfinding()
{
	auto ptr = GetPlayerEntity();
	if (!ptr)
		return nullptr;
	auto res = ptr->GetPathfinding();
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res;
}

bool CD_Interface::PlayerMove(const stCD_VecInt& dst_pos)
{
	auto p = GetPlayerPathfinding();
	if (!p)
		return false;
	return p->PlayerMove(dst_pos);
}

void CD_Interface::RegSomeForLua(lua_State* l)
{
	using namespace luabind;
	module(l)[
		class_<stCD_VecInt>("stCD_VecInt")
		.def(constructor<>())
		.def(constructor<int, int>())
	];
}

stCD_EntityPlayerMe* CD_Interface::GetPlayerEntity()
{
	return stCD_GameBase::GetMe().GetPlayerEntity();
}

int CD_Interface::TraEntityObj(const Fn_TraObj& fn, bool is_obj)
{
	auto w = GetWorld();
	if (!w)
	{
		assert(false);
		return 0;
	}
	return w->obj_mgr_.TraEntityObj(fn, is_obj);
}

void CD_Interface::DebugEntityObjs(int type)
{
	auto w = GetWorld();
	if (!w)
		return;
	if (type == -1)
	{
		auto cnt = TraEntityObj([](stCD_EntityPosBased* entity){
			entity->DebugInfo();
			return true;
		}, false);
		LOG_O(Log_debug) << "对象总数bbb：" << cnt << ", 游戏中的数量：" << w->obj_mgr_.doodad_objs_.size();
		return;
	}
	else if (type < 1)
	{
		LOG_O(Log_debug) << "obj_mgr_addr:" << &w->obj_mgr_;
		auto cnt = TraEntityObj([](stCD_EntityPosBased* entity){
			entity->DebugInfo();
			return true;
		}, true);
		LOG_O(Log_debug) << "对象总数aaa：" << cnt << ", 游戏中的数量：" << w->obj_mgr_.objs_.size();
		cnt = TraEntityObj([](stCD_EntityPosBased* entity){
			entity->DebugInfo();
			return true;
		}, false);
		LOG_O(Log_debug) << "对象总数bbb：" << cnt << ", 游戏中的数量：" << w->obj_mgr_.doodad_objs_.size();
		return;
	}
	stCD_EntityPosBased::Fn_EntityDebugInfo fn = nullptr;
	if (1 == type)
		fn = &stCD_EntityPosBased::DebugInfoAsNpc;
	else if (2 == type)
		fn = &stCD_EntityPosBased::DebugInfoAsPlayer;
	else if (3 == type)
		fn = &stCD_EntityPosBased::DebugInfoAsMonster;
	else if (4 == type)
		fn = &stCD_EntityPosBased::DebugInfoAsWaypoint;
	else if (5 == type)
		fn = &stCD_EntityPosBased::DebugInfoAsAreaTransition;
	else if (6 == type)
		fn = &stCD_EntityPosBased::DebugInfoNoHuman;
	else if (7 == type)
		fn = &stCD_EntityPosBased::DebugInfoChest;
	else if (8 == type)
		fn = &stCD_EntityPosBased::DebugInfoWorldItem;
	else if (9 == type)
		fn = &stCD_EntityPosBased::DebugInfoTransitionable;
	else if (10 == type)
		fn = &stCD_EntityPosBased::DebugMonsterSkillInfo;
	else
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "遍历之前，游戏中的数量1：" << w->obj_mgr_.objs_.size();
	auto cnt = TraEntityObj([&fn](stCD_EntityPosBased* entity){
		(entity->*fn)();
		return true;
	}, true);
	LOG_O(Log_debug) << "对象总数1：" << cnt << ", 游戏中的数量：" << w->obj_mgr_.objs_.size();

	LOG_O(Log_debug) << "遍历之前，游戏中的数量2：" << w->obj_mgr_.doodad_objs_.size();
	cnt = TraEntityObj([&fn](stCD_EntityPosBased* entity){
		(entity->*fn)();
		return true;
	}, false);
	LOG_O(Log_debug) << "对象总数2：" << cnt << ", 游戏中的数量：" << w->obj_mgr_.doodad_objs_.size();
}

void CD_Interface::DebugPlayerAttrInfo(int type)
{
	auto ptr = GetPlayerEntity();
	if (!ptr)
		return;
	if (type < 0)
	{
		auto db_tbl = ptr->GetDbTbl();
		if (!db_tbl)
			return;
		db_tbl->DebugInfo();
		return;
	}
	switch ((enCD_EntityAttr_Player)type)
	{
	case enCD_EntityAttr_Player::kEAP_Life:
	{
		auto l = ptr->GetLife();
		if (!l)
			return;
		l->DebugInfo();
		break;
	}
	//case enCD_EntityAttr_Player::
	default:
		assert(false);
		break;
	}
}

stCD_GameLogic* CD_Interface::GetGameLogic()
{
	return stCD_GameBase::GetMe().GetGameLogic();
}

void CD_Interface::DebugPassiveSkill()
{
	auto l = GetGameLogic();
	if (!l)
		return;
	l->passive_skill_.DebugInfo();
}

void CD_Interface::DebugDbInfo(const char* file_path)
{
	const auto& str = GbkToWideChar(file_path);
	LOG_O(Log_debug) << GetDbMgr().FindDb(str.c_str());
}

void CD_Interface::DebugDbPassiveSkill()
{
	auto p = GetDbMgr().GetDbPassiveSkill();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbQuestStates()
{
	auto p = GetDbMgr().GetDbQuestStates();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbQuest()
{
	auto p = GetDbMgr().GetDbQuest();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbSkillGems()
{
	auto p = GetDbMgr().GetDbSkillGems();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbActiveSkills()
{
	auto p = GetDbMgr().GetDbActiveSkills();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbItemClasses()
{
	auto p = GetDbMgr().GetDbItemClasses();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbBaseItemTypes()
{
	auto p = GetDbMgr().GetDbBaseItemTypes();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbGenTags()
{
	auto p = GetDbMgr().GetDbGenTags();
	if (p)
		p->DebugInfo();
}

stCD_InventoryMgr* CD_Interface::GetInventoryMgr()
{
	auto l = GetGameLogic();
	if (!l)
		return nullptr;
	return &l->inventory_mgr_;
}

int CD_Interface::TraBodyItems(const Fn_TraItem& fn, bool include_flask, bool include_hide_weapon)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
		return 0;
	return mgr->TraBodyItems(fn, include_flask, include_hide_weapon);
}

void CD_Interface::DebugBodyItems(bool include_flask, bool include_hide_weapon)
{
	auto cnt = TraBodyItems([](stCD_ItemGridsInfo* item_grid_info, stCD_EntityItemBased* entity){
		item_grid_info->DebugInfo();
		entity->DebugInfo();
		LOG_O(Log_debug) << "物品实体对象遍历结束\n";
		return true;
	}, include_flask, include_hide_weapon);
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

int CD_Interface::TraEquipsInsideGem(enCD_InventoryType type, const Fn_TraEquipsInsideGem& fn)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
		return 0;
	auto inventory = mgr->GetInventory(type);
	if (!inventory)
		return 0;
	return inventory->TraEquipsInsideGem(fn);
}

void CD_Interface::DebugEquipsInsideGem(int type)
{
	auto cnt = TraEquipsInsideGem((enCD_InventoryType)type, [](stCD_EntityItemBased* equip, stCD_EntityItemBased* gem){
		gem->DebugInfo();
		LOG_O(Log_debug) << "物品实体对象遍历结束\n";
		return true;
	});
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

int CD_Interface::TraBodyEquipsGem(const Fn_TraEquipsInsideGem& fn, bool include_hide_weapon)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
		return 0;
	return mgr->TraBodyEquipsGem(fn, include_hide_weapon);
}

void CD_Interface::DebugBodyEquipsGem(bool include_hide_weapon)
{
	auto cnt = TraBodyEquipsGem([](stCD_EntityItemBased* equip, stCD_EntityItemBased* gem){
		gem->DebugInfo();
		LOG_O(Log_debug) << "物品实体对象遍历结束\n";
		return true;
	}, include_hide_weapon);
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

int CD_Interface::TraInventoryItems(enCD_InventoryType type, const Fn_TraItem& fn)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
		return 0;
	auto inventory = mgr->GetInventory(type);
	if (!inventory)
		return 0;
	return inventory->TraItems(fn);
}

void CD_Interface::DebugInventoryItems(int type)
{
	auto cnt = TraInventoryItems((enCD_InventoryType)type, [](stCD_ItemGridsInfo* item_grid_info, stCD_EntityItemBased* entity){
		item_grid_info->DebugInfo();
		entity->DebugInfo();
		LOG_O(Log_debug) << "物品实体对象遍历结束\n";
		return true;
	});
	LOG_O(Log_debug) << "物品总数：" << cnt;
}

void CD_Interface::DebugDbGrantedEffects()
{
	auto p = GetDbMgr().GetDbGrantedEffects();
	if (p)
		p->DebugInfo();
}

int CD_Interface::TraSkillGems(enCD_InventoryType type, const Fn_TraSkillGem& fn)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
		return 0;
	auto inventory = mgr->GetInventory(type);
	if (!inventory)
		return 0;
	return inventory->TraSkillGems(fn);
}

int CD_Interface::TraGemSkill(enCD_InventoryType type, const Fn_TraGemSkill& fn, enCD_TraGemSkillFlag flag)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
		return 0;
	auto inventory = mgr->GetInventory(type);
	if (!inventory)
		return 0;
	return inventory->TraGemSkill(fn, flag);
}

void CD_Interface::DebugSkillGems(int type)
{
	auto cnt = TraSkillGems((enCD_InventoryType)type, [](stCD_EntityItemBased* gem){
		gem->DebugInfo();
		LOG_O(Log_debug) << "物品实体对象遍历结束\n";
		return true;
	});
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

void CD_Interface::DebugGemSkill(int type, int flag)
{
	auto cnt = TraGemSkill((enCD_InventoryType)type, [](stCD_EntityItemBased* equip, stCD_EntityItemBased* gem, stCD_AttrDbSkillGem* db_skill_gem){
		/*if (equip != gem)
			equip->DebugInfo();
		gem->DebugInfo();*/
		db_skill_gem->DebugInfo();
		LOG_O(Log_debug) << "物品实体对象遍历结束\n";
		return true;
	}, enCD_TraGemSkillFlag(flag));
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

int CD_Interface::TraBodyGemSkill(const Fn_TraGemSkill& fn, bool active_skill, bool passive_skill, bool include_hide_weapon)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
		return 0;
	return mgr->TraBodyGemSkill(fn, active_skill, passive_skill, include_hide_weapon);
}

void CD_Interface::DebugBodyGemSkill(bool active_skill, bool passive_skill, bool include_hide_weapon)
{
	auto cnt = TraBodyGemSkill([](stCD_EntityItemBased* equip, stCD_EntityItemBased* gem, stCD_AttrDbSkillGem* db_skill_gem){
		/*if (equip != gem)
			equip->DebugInfo();
		gem->DebugInfo();*/
		db_skill_gem->DebugInfo();
		LOG_O(Log_debug) << "物品实体对象遍历结束\n";
		return true;
	}, active_skill, passive_skill, include_hide_weapon);
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

pt_word CD_Interface::GetSkillIdMove()
{
	return (pt_word)stCD_SomeSkillId::GetMe().skill_id_move_;
}

void CD_Interface::DebugInventorySkill(int type)
{
	auto in = GetInGameState();
	if (!in)
		return;
	if (IsUnValidPtr(in->inventory_mgr_))
	{
		assert(false);
		return;
	}
	in->inventory_mgr_->DebugInfo();
}

void CD_Interface::DebugMySkillInfo()
{
	auto p = GetPlayerEntity();
	if (!p)
		return;
	auto actor = p->GetActor();
	if (!actor)
	{
		assert(false);
		return;
	}
	auto& mgr = actor->GetMySkillMgr();
	mgr.DebugInfo();
	auto cnt = TraMySkill([](stCD_SkillInfo* skill_info){
		skill_info->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总共遍历到的数量：" << cnt;
}

int CD_Interface::TraMySkill(const Fn_TraMySkill& fn)
{
	auto p = GetPlayerEntity();
	if (!p)
		return 0;
	auto actor = p->GetActor();
	if (!actor)
	{
		assert(false);
		return 0;
	}
	return actor->GetMySkillMgr().TraMySkill(fn);
}

bool CD_Interface::DoTouchTargetObj(enCD_SkillId flag, pt_dword net_id, enCD_TouchTargetSmallFlag small_flag)
{
	auto l = GetGameLogic();
	if (!l)
	{
		assert(false);
		return false;
	}
	return l->DoTouchTargetObj(flag, net_id, small_flag);
}

bool CD_Interface::CloseNpcPanel()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return false;
	}
	return main_ui->CloseNpcPanel();
}

void CD_Interface::DebugCurQuestInfo()
{
	auto p = GetPlayer();
	if (!p)
	{
		assert(false);
		return;
	}
	p->DebugInfo();
	auto cnt = TraCurQuestInfo([](stCD_CurQuestInfo* quest_info){
		quest_info->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

int CD_Interface::TraCurQuestInfo(const Fn_TraCurQuestInfo& fn)
{
	auto p = GetPlayer();
	if (!p)
	{
		assert(false);
		return 0;
	}
	return p->TraCurQuestInfo(fn);
}

stCD_GameBaseDb& CD_Interface::GetDbMgr()
{
	return stCD_GameBaseDb::GetMe();
}

void CD_Interface::DebugDbWorldArea()
{
	auto p = GetDbMgr().GetDbWorldArea();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugMapModels(int radius, bool has_obj)
{
	auto map = GetWorld();
	if (!map)
	{
		assert(false);
		return;
	}
	map->map_data_.DebugModelsInfo(radius, has_obj);
}

void CD_Interface::DebugInGameState()
{
	auto state = GetInGameState();
	if (!state)
	{
		assert(false);
		return;
	}
	state->DebugInfo();
}

stCD_StateUiLogic* CD_Interface::GetInGameUiLogic()
{
	auto state = GetInGameState();
	if (!state)
		return nullptr;
	return &state->ui_logic_;
}

stCD_InGameTopUi* CD_Interface::GetInGameTopUi()
{
	auto state = GetInGameState();
	if (!state)
		return nullptr;
	return state->GetTopUi();
}

void CD_Interface::DebugUiWaypointItems(int difficulty, int flag)
{
	auto dlg = GetDlgWayPoint();
	if (!dlg)
	{
		assert(false);
		return;
	}
	auto cnt = dlg->TraWaypointItem((enCD_TraUiShownFlag)flag, (enCD_kMapDifficulty)difficulty, [](enCD_kMapDifficulty difficult, stCD_DlgWaypointMap* map, stCD_WaypointPointItem* item){
		item->DebugInfo(difficult, map);
		return true;
	});
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

void CD_Interface::DebugUiFullInfo(pt_dword ui_addr, int flag, int state_cate)
{
	if (IsUnValidPtr(ui_addr))
	{
		if (1 == state_cate)
		{
			ui_addr = (int)GetInGameTopUi();
			if (!ui_addr)
			{
				assert(false);
				return;
			}
			LOG_O(Log_debug) << "此地址默认为in game的top Ui地址";
		}
		else if (2 == state_cate)
		{
			ui_addr = (int)GetInGameMainUi();
			if (!ui_addr)
			{
				assert(false);
				return;
			}
			LOG_O(Log_debug) << "此地址默认为in game的主ui地址";
		}
		else if (3 == state_cate)
		{
			ui_addr = (int)GetEscapeTopUi();
			if (!ui_addr)
			{
				assert(false);
				return;
			}
			LOG_O(Log_debug) << "此地址默认为Escape的top ui地址";
		}
		else if (4 == state_cate)
		{
			ui_addr = (int)GetPreInGameTopUi();
			if (!ui_addr)
			{
				assert(false);
				return;
			}
			LOG_O(Log_debug) << "此地址默认为PreInGame的top ui地址";
		}
		else if (5 == state_cate)
		{
			auto p = GetPreInGameTopUi();
			if (!p)
			{
				assert(false);
				return;
			}
			auto ptr = p->GetDlgSelectRole();
			if (!ptr)
			{
				assert(false);
				return;
			}
			ptr->DebugInfo();
			return;
		}
		else
		{
			assert(false);
			return;
		}
	}
	((stCD_UiObjBase*)ui_addr)->DebugFullInfo(enCD_TraUiShownFlag(flag));
}

void CD_Interface::DebugTheUiInfo(int level, pt_dword root_ui_addr, pt_dword addr, int filter_type, int flag)
{
	if (root_ui_addr)
	{
		if (IsUnValidPtr(root_ui_addr))
		{
			assert(false);
			return;
		}
	}
	else
	{
		root_ui_addr = (int)GetInGameMainUi();
		if (!root_ui_addr)
		{
			assert(false);
			return;
		}
		LOG_O(Log_debug) << "此地址默认为主ui地址";
	}
	
	/*if (IsUnValidPtr(addr))
	{
		assert(false);
		return;
	}*/
	auto cnt = ((stCD_UiObjBase*)root_ui_addr)->TraSubUiObjEx(level, (pt_pcvoid)addr, 
		(enCD_TraUiFilterType)filter_type, (enCD_TraUiShownFlag)flag,
		[](stCD_UiObjBase* item){
		item->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

void CD_Interface::DebugTheParentUiInfo(pt_dword ui_addr)
{
	if (IsUnValidPtr(ui_addr))
	{
		assert(false);
		return;
	}
	((stCD_UiObjBase*)ui_addr)->DebugParentsInfo();
}

void CD_Interface::DebugTheWaypointItem(int difficulty, const std::string& map_name)
{
	auto dlg = GetDlgWayPoint();
	if (!dlg)
	{
		assert(false);
		return;
	}
	stCD_DlgWaypoint::stWaypointInfo info;
	if (!dlg->FindWaypoint((enCD_kMapDifficulty)difficulty, GbkToWideChar(map_name), info))
	{
		LOG_O(Log_debug) << "没找到传送的点";
		return;
	}
	info.waypoint_->DebugInfo(info.difficulty_, info.map_);
}

stCD_DlgWaypoint* CD_Interface::GetDlgWayPoint()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgWaypoint();
}

stCD_InGameMainUi* CD_Interface::GetInGameMainUi()
{
	auto top_ui = GetInGameTopUi();
	if (!top_ui)
	{
		assert(false);
		return nullptr;
	}
	auto main_ui = top_ui->GetMainUi();
	assert(main_ui);
	return main_ui;
}

stCD_EscapeTopUi* CD_Interface::GetEscapeTopUi()
{
	auto state = GetEscapeState();
	if (!state)
		return nullptr;
	return state->GetTopUi();
}

stCD_SceneEscapeState* CD_Interface::GetEscapeState()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	return ptr->GetEscapeState();
}

bool CD_Interface::PressEscapeBtItem(enCD_EscapeMenuButton bt_idx)
{
	auto state = GetEscapeState();
	if (!state)
	{
		assert(false);
		return false;
	}
	return state->PressBtItem(bt_idx);
}

bool CD_Interface::SwitchToEscapeState()
{
	auto state = GetInGameState();
	if (!state)
	{
		assert(false);
		return false;
	}
	return state->SwitchToEscapeState();
}

stCD_SceneHolder* CD_Interface::GetSceneHolder()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	return ptr;
}

int CD_Interface::GetInGameMainUiDlgCnt(enCD_TraUiShownFlag show_flag)
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return 0;
	}
	return main_ui->GetDlgCnt(show_flag);
}

void CD_Interface::DebugInGameMainUiAddrInfo()
{
	auto ui = GetInGameMainUi();
	if (!ui)
	{
		assert(false);
		return;
	}
	ui->DebugAddrInfo();
}

void CD_Interface::DebugNpcPanel(int panel_type)
{
	auto npc_panel = GetNpcPanel(false);
	if (!npc_panel)
	{
		assert(false);
		return;
	}
	npc_panel->DebugInfo();
	auto cnt = npc_panel->TraMenuItem((enCD_NpcPanelItemsContainer)panel_type, [](stCD_UiNpcPanelMenuItem* item){
		item->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总共数量：" << cnt;
	auto c = npc_panel->GetMenuItemsContainer((enCD_NpcPanelItemsContainer)panel_type);
	if (!c)
	{
		assert(false);
		return;
	}
	c->DebugInfo();
}

void CD_Interface::TestPressNpcItem(int panel_type, const std::string& lable)
{
	auto npc_panel = GetNpcPanel(false);
	if (!npc_panel)
	{
		assert(false);
		return;
	}
	npc_panel->PressItem(enCD_NpcPanelItemsContainer(panel_type), GbkToWideChar(lable));
}

stCD_UiNpcPanel* CD_Interface::GetNpcPanel(bool must_shown)
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetNpcPanel(must_shown);
}

void CD_Interface::DebugDbStats()
{
	auto p = GetDbMgr().GetDbStats();
	if (p)
		p->DebugInfo();
}

bool CD_Interface::MainSubUiShowState(pt_pcvoid vtbl, enCD_TraUiShownFlag show_flag)
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return false;
	}
	auto sub_ui = main_ui->FindSubUiObjByVtbl(1, vtbl, kTUSF_ShowHide, 1);
	if (!sub_ui)
	{
		assert(false);
		return false;
	}
	return sub_ui->SameShowState(show_flag);
}

bool CD_Interface::NpcPanelSubUiShowState(enCD_NpcPanelItemsContainer panel, enCD_TraUiShownFlag show_flag)
{
	auto npc_panel = GetNpcPanel(true);
	if (!npc_panel)
	{
		//assert(false);
		return false;
	}
	auto c = npc_panel->GetMenuItemsContainer(panel);
	if (!c)
		return false;
	return c->SameShowState(show_flag);
}

void CD_Interface::DebugQuestAwardInventory()
{
	auto inventory = GetQuestAwardInventory();
	if (!inventory)
		return;
	inventory->DebugInfo(true);
}

stCD_Inventory* CD_Interface::GetQuestAwardInventory()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	auto dlg = main_ui->GetDlgQuestAward();
	if (!dlg)
	{
		assert(false);
		return nullptr;
	}
	return dlg->GetInventory();
}

void CD_Interface::DebugDbMapPins()
{
	auto p = GetDbMgr().GetDbMapPins();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbWeaponTypes()
{
	auto p = GetDbMgr().GetDbWeaponTypes();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugDbClientStrings()
{
	auto p = GetDbMgr().GetDbClientStrings();
	if (p)
		p->DebugInfo(1);
}

void CD_Interface::DebugDbMapConnections()
{
	auto p = GetDbMgr().GetDbMapConnections();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugMonsterTypes()
{
	auto p = GetDbMgr().GetDbMonsterTypes();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugMonsterVarieties()
{
	auto p = GetDbMgr().GetDbMonsterVarieties();
	if (p)
		p->DebugInfo();
}

#ifdef _DEBUG
static bool g_recv_out_debug_info = false;
#endif

static void* Hooker_RecvPacketRead(IHooker& hooker, JmpHookerAmbient& ambient)
{
	if (IsUnValidPtr(ambient.ebx_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto hooker_obj = Hooks::GetInstance();
	if (!hooker_obj)
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto recv_packet = (stCD_Socket*)ambient.ebx_;
	auto data = recv_packet->GetRecvOrgData();
	if (!data)
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto msg_size = recv_packet->recv_msg_len_;
	if (msg_size < sizeof(pt_word))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	msg_size -= sizeof(pt_word);
	pt_word msg_id = ntohs(*(pt_word*)data);
	auto msg_data = pt_pcvoid((pt_csz)data + sizeof(pt_word));
	//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	if (g_recv_out_debug_info && msg_id != 0x108 && msg_id != 0x10
		&& msg_id != 0xda	//别的玩家走会收到这个包，自己走路也会收到该包、怪物走路也会收到该包
		)
	{
		LOG_O(Log_debug) << "收到包，msg_id:" << (pt_pcvoid)msg_id << ",大小：" << msg_size << ",msg_data:" << msg_data;
		auto msg_data_tmp = msg_data;
		FnForOd("HookRecvPacket");
	}
#endif
	//////////////////////////////////////////////////////////////////////////
	return hooker_obj->GetRecvPacketMgr(recv_packet->GetSocketType()).HandleMsg(hooker.GetCodeOrig(), msg_id, msg_data, msg_size).ret_addr_;
}
void CD_Interface::Hooks::HookRecvPacketRead()
{
	/*if (recv_packet_mgr_.Empty())
	{
		//assert(false);
		return;
	}*/
	HOOKER_SetCodeHooker(Hooker_RecvPacketRead, CD_EXE_DATA(g_h_recv_packet_read_), Hooker_RecvPacketRead);
}

void CD_Interface::TestRecvOutDebugInfo(bool open)
{
#ifdef _DEBUG
	g_recv_out_debug_info = open;
#endif
}

void CD_Interface::DebugNpcPanelMenuItemInfo(int type)
{
	auto npc_panel = GetNpcPanel(false);
	if (!npc_panel)
		return;
	npc_panel->DebugMenuItemInfo(type);
}

stCD_EntityItemBased* CD_Interface::GetMouseAttachItem()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	auto ui_mouse_attch_item = main_ui->GetMouseAttachItem();
	if (!ui_mouse_attch_item)
	{
		assert(false);
		return nullptr;
	}
	return ui_mouse_attch_item->GetItem();
}

bool CD_Interface::CanPutItemToTheInventory(stCD_EntityItemBased* item, enCD_InventoryType dst_inventory_type, int dst_pos_x, int dst_pos_y)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
	{
		assert(false);
		return false;
	}
	return mgr->CanPutItToTheInventory(item, dst_inventory_type, dst_pos_x, dst_pos_y);
}

bool CD_Interface::CanPutItemToTheInventory(enCD_InventoryType src_inventory_type, pt_dword item_id, enCD_InventoryType dst_inventory_type, int dst_pos_x, int dst_pos_y)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
	{
		assert(false);
		return false;
	}
	auto src_inventory = mgr->GetInventory(src_inventory_type);
	if (!src_inventory)
	{
		assert(false);
		return false;
	}
	auto item = src_inventory->FindItem(item_id);
	if (!item)
	{
		assert(false);
		return false;
	}
	return mgr->CanPutItToTheInventory(item, dst_inventory_type, dst_pos_x, dst_pos_y);
}

stCD_Inventory* CD_Interface::GetInventory(enCD_InventoryType inventory_type)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
	{
		assert(false);
		return nullptr;
	}
	return mgr->GetInventory(inventory_type);
}

stCD_EntityItemBased* CD_Interface::FindInventoryItem(enCD_InventoryType inventory_type, pt_dword item_id)
{
	auto inventory = GetInventory(inventory_type);
	if (!inventory)
	{
		assert(false);
		return nullptr;
	}
	return inventory->FindItem(item_id);
}

int CD_Interface::TraUiQuestStateItem(const Fn_TraQuestItem& fn)
{
	auto m = GetInGameMainUi();
	if (!m)
	{
		assert(false);
		return 0;
	}
	auto quest_ui = m->GetQuestLvlStatePanel();
	if (!quest_ui)
	{
		assert(false);
		return 0;
	}
	return quest_ui->TraQuestItem(fn);
}

void CD_Interface::DebugUiQuestStateItem()
{
	auto cnt = TraUiQuestStateItem([](stCD_UiQuestStateQuestItem* item){
		item->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总数量：" << cnt;
}

void CD_Interface::DebugDlgWaypoint()
{
	auto dlg = GetDlgWayPoint();
	if (!dlg)
	{
		assert(false);
		return;
	}
	dlg->DebugInfo();
}

int CD_Interface::TraUiQuestItem(enCD_TraUiShownFlag show_flag, enCD_kMapDifficulty difficulty, int chapter, 
	const Fn_TraUiQuestItem& fn)
{
	auto dlg = GetDlgWayPoint();
	if (!dlg)
	{
		assert(false);
		return 0;
	}
	return dlg->TraQuestItem(show_flag, difficulty, chapter, fn);
}

void CD_Interface::DebugUiQuestItem(int show_flag, int difficulty, int chapter)
{
	auto cnt = TraUiQuestItem(enCD_TraUiShownFlag(show_flag), enCD_kMapDifficulty(difficulty), chapter, 
		[](enCD_kMapDifficulty the_d, int the_chapter, stCD_DlgWaypointQuestItem* item){
		item->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

bool CD_Interface::Revive(enCD_ReviveAtPosition at_position)
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return false;
	}
	return main_ui->Revive(at_position);
}

void CD_Interface::DebugTheObjSkillInfo(pt_dword entity_id)
{
	stCD_EntityPosBased* the_obj = nullptr;
	TraEntityObj([&the_obj, entity_id](stCD_EntityPosBased* obj){
		if (obj->id_ == entity_id)
		{
			the_obj = obj;
			return false;
		}
		return true;
	}, true);
	if (!the_obj)
	{
		LOG_O(Log_debug) << "没有找到该对象";
		return;
	}
	auto actor = the_obj->GetActor();
	if (!actor)
	{
		LOG_O(Log_debug) << "没有actor属性";
		return;
	}
	auto& mgr = actor->GetMySkillMgr();
	mgr.DebugInfo();
	auto cnt = mgr.TraMySkill([](stCD_SkillInfo* skill_info){
		skill_info->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总共遍历到的数量：" << cnt;
}

int CD_Interface::TraInventory(const Fn_TraInventory& fn)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
	{
		assert(false);
		return 0;
	}
	return mgr->TraInventory(fn);
}

int CD_Interface::TraInventory(const FilterInventoryType& filter, const Fn_TraInventory& fn)
{
	auto mgr = GetInventoryMgr();
	if (!mgr)
	{
		assert(false);
		return 0;
	}
	return mgr->TraInventory(filter, fn);
}

bool CD_Interface::CloseSomeBoringUi()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return false;
	}
	if (!main_ui->CloseDlgModal())
	{
		assert(false);
		return false;
	}
	main_ui->SkipAllTutorials();
	return true;
}

int CD_Interface::PressAllLvlUpSkillGem()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return 0;
	}
	auto the_panel = main_ui->GetQuestLvlStatePanel();
	if (!the_panel)
		return 0;
	return the_panel->PressAllLvlUpSkillGem();
}

bool CD_Interface::ChooseKillTheMonster()
{
	auto c = GetNpcPanelContainer();
	if (!c)
		return false;
	auto dlg = c->GetDlgChoice2();
	if (!dlg)
		return false;
	return dlg->Choose(1);
}

stCD_UiNpcPanelContainer* CD_Interface::GetNpcPanelContainer()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetNpcPanelContainer();
}

stCD_DlgProp* CD_Interface::GetDlgProp()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgProp();
}

stCD_DlgSell* CD_Interface::GetDlgSell()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgSell();
}

stCD_DlgBuy* CD_Interface::GetDlgBuy()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgBuy();
}

stCD_DlgStash* CD_Interface::GetDlgStash()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgStash();
}

stCD_UiDlgQuestAward* CD_Interface::GetDlgQuestAward()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgQuestAward();
}

void CD_Interface::DebugDlgItems(int dlg_type)
{
	if (0 == dlg_type)
	{
		auto dlg = GetDlgBuy();
		if (!dlg)
		{
			assert(false);
			return;
		}
		auto pages = dlg->GetTabPages();
		if (!pages)
		{
			assert(false);
			return;
		}
		pages->TraPages([](const stCD_DlgBuy::PageInfo& page){
			auto grid_panel = page.GetGridPanel<stCD_UiBuyTabPageItem>();
			if (!grid_panel)
				return true;
			auto inventory = grid_panel->GetInventory();
			if (!inventory)
			{
				assert(false);
				return true;
			}
			auto cnt = inventory->TraItems([](stCD_ItemGridsInfo*, stCD_EntityItemBased* item){
				item->DebugInfo();
				return true;
			});
			LOG_O(Log_debug) << "inventory_type:" << grid_panel->inventory_type_ << "总数量：" << cnt;
			return true;
		});
	}
	else if (1 == dlg_type)
	{
		auto dlg = GetDlgStash();
		if (!dlg)
		{
			assert(false);
			return;
		}
		auto pages = dlg->GetTabPages();
		if (!pages)
		{
			assert(false);
			return;
		}
		pages->TraPages([](const stCD_DlgStash::PageInfo& page){
			auto grid_panel = page.GetGridPanel<stCD_UiStashTabPageItem>();
			if (!grid_panel)
				return true;
			auto inventory = grid_panel->GetInventory();
			if (!inventory)
			{
				assert(false);
				return true;
			}
			auto cnt = inventory->TraItems([](stCD_ItemGridsInfo*, stCD_EntityItemBased* item){
				item->DebugInfo();
				return true;
			});
			LOG_O(Log_debug) << "inventory_type:" << grid_panel->inventory_type_ << "总数量：" << cnt;
			return true;
		});
	}
	else if (2 == dlg_type)
	{
		auto dlg = GetDlgStash();
		if (!dlg)
		{
			assert(false);
			return;
		}
		auto pages = dlg->GetTabPages();
		if (!pages)
		{
			assert(false);
			return;
		}
		pages->TraPages([](const stCD_DlgStash::PageInfo& page){
			auto page_item = page.GetPageItemT<stCD_UiStashTabPageItem>();
			if (!page_item)
				return true;
			auto mutli = page_item->GetMultiContainer(kTUSF_Shown);
			if (!mutli)
				return true;
			auto cnt = mutli->TraGridPanel([](stCD_UiGridPanel* grid_panel){
				LOG_O(Log_debug) << "-------------------------";
				auto inventory = grid_panel->GetInventory();
				if (!inventory)
				{
					assert(false);
					return true;
				}
				auto cnt = inventory->TraItems([](stCD_ItemGridsInfo*, stCD_EntityItemBased* item){
					item->DebugInfo();
					return true;
				});
				LOG_O(Log_debug) << "inventory_type:" << grid_panel->inventory_type_ << "总数量：" << cnt;
				LOG_O(Log_debug) << "-------------------------";
				return true;
			});
			LOG_O(Log_debug) << "GridPanel的总数量：" << cnt;
			return true;
		});
	}
	else
		assert(false);
}

stCD_UiPreInGameTop* CD_Interface::GetPreInGameTopUi()
{
	auto p = GetPreGameState();
	if (!p)
	{
		assert(false);
		return nullptr;
	}
	return p->GetTopUi();
}

stCD_UiSelectRoleTop* CD_Interface::GetSelectRoleTop()
{
	auto state = GetSelectCharacterState();
	if (!state)
		return nullptr;
	auto ui_top = state->GetUiSelectRole();
	if (!ui_top)
		return nullptr;
	auto ui = GetPreInGameTopUi();
	if (!ui)
		return nullptr;
	auto dlg = ui->GetDlgSelectRole();
	if (!dlg)
		return nullptr;
	//return dlg->GetTopUi();
	return ui_top;
}

void CD_Interface::DebugDbPassiveSkillGraph()
{
	auto p = GetDbMgr().GetDbPassiveSkillGraph();
	if (p)
		p->DebugInfo();
}

stCD_UiGridPanel* CD_Interface::FindGridPanelFromDlgProp(enCD_InventoryType inventory_type)
{
	auto dlg = GetDlgProp();
	if (!dlg) return nullptr;
	return dlg->FindGridPanel(inventory_type);
}

void CD_Interface::DebugFindGridPanelFromDlgProp(int type)
{
	auto dlg = GetDlgProp();
	if (!dlg)
	{
		assert(false);
		return;
	}
	dlg->DebugInfo();
	auto ui_grid_panel = dlg->FindGridPanel((enCD_InventoryType)type);
	if (!ui_grid_panel)
	{
		assert(false);
		return;
	}
	ui_grid_panel->DebugInfo();
}

stCD_SceneCreateCharacterState* CD_Interface::GetCreateCharacterState()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	return ptr->GetCreateCharacterState();
}

stCD_SceneSelectCharacterState* CD_Interface::GetSelectCharacterState()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	return ptr->GetSelectCharacterState();
}

int CD_Interface::TraCallerBuff(const Fn_TraCallerBuff& fn)
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return 0;
	}
	auto buff = main_ui->GetUiBuff();
	if (!buff)
		return 0;
	return buff->TraCallerBuff(fn);
}

void CD_Interface::DebugCallerBuff()
{
	auto cnt = TraCallerBuff([](stCD_SkillInfo* skill_info, stCD_UiBuffItem* ui_buff_item){
		skill_info->DebugInfo();
		ui_buff_item->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总数量：" << cnt;
}

stCD_DlgMapLocator* CD_Interface::GetDlgMapLocator()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgMapLocator();
}

stCD_UiItemTips* CD_Interface::GetFirstItemTips()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	auto ui_bottom = main_ui->GetUiBottom();
	if (!ui_bottom)
	{
		assert(false);
		return nullptr;
	}
	auto ui_tips = ui_bottom->GetFirstTips();
	if (!ui_tips)
	{
		assert(false);
		return nullptr;
	}
	return ui_tips;
}

CmnRecvPacketMgr& CD_Interface::Hooks::GetRecvPacketMgr(enCD_SocketType socket_type)
{
	//return recv_packet_mgr_in_game_;
	if (socket_type == kST_PreGameState)
		return recv_packet_mgr_pre_in_game_;
	else
		return recv_packet_mgr_in_game_;
}

bool CD_Interface::Hooks::DoHooks()
{
	//return true;
	HookForTest();
	HookRecvPacketRead();
	HookCheckPacket_Heartbeat();
	HookOnMoveToAvoidDamage();
	HookInitMap();
	HookModifyGameEnv();
	HookAnalysisRecvPacket();
	HookAnalysisSendPacket();
	HookSkillObjUpdateMousePos();
	HookGetForegroundWindow();
	HookOperateCommand();
	HookIsWow64Process();
	HookDisableClientSwitching();
	HookForAntiCheckPacket();
	HookAntiCollision();
	HookGetKeyState();
	HookModifyLifeData();
	HookPromptInfo();
	HookForDuoKai();
#ifdef VER_GF_
	HookAntiCheck_GF();
#endif
	return true;
}

static void* Hooker_RetCheckPacket_0x1(IHooker& hooker, JmpHookerAmbient& ambient)
{
	return (void*)g_so_ret_check_packet_0x1_;
}
static void* Hooker_RetHeartbeat_0xf(IHooker& hooker, JmpHookerAmbient& ambient)
{
	return (void*)g_so_ret_heartbeat_packet_0xf_;
}
void CD_Interface::Hooks::HookCheckPacket_Heartbeat()
{
	//封号可能是因为某些hook
//#ifdef _DEBUG
	HOOKER_SetCodeHooker(Hooker_RetCheckPacket_0x1, CD_EXE_DATA(g_p_ret_0x1_0xf_check_packet_), Hooker_RetCheckPacket_0x1);
	HOOKER_SetCodeHooker(Hooker_RetHeartbeat_0xf, g_so_hook_heartbeat_packet_, Hooker_RetHeartbeat_0xf);
//#endif
}

//////////////////////////////////////////////////////////////////////////
static OnRecvMoveMsg* g_on_recv_move_msg = nullptr;
static void* Hooker_HookOnMoveToAvoidDamage(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto ret_code_addr = hooker.GetCodeOrig();
	auto msg_reciever = g_on_recv_move_msg;
	if (!msg_reciever)
		return ret_code_addr;
	if (IsUnValidPtr(ambient.edi_))
	{
		assert(false);
		return ret_code_addr;
	}
	auto msg_on_move = (stCDGR_OnMove*)ambient.edi_;
	auto player_entity = GetPlayerEntity();
	if (!player_entity)
	{
		assert(false);
		return ret_code_addr;
	}
	if (msg_on_move->entity_.entity_id_ == player_entity->id_)
	{
		if (msg_reciever)
			msg_reciever->OnSelfMoveMsg(*msg_on_move);
		return ret_code_addr;
	}
	return ret_code_addr;
	
	/*if (msg_on_move->skill_id_ == GetSkillIdMove())
		return ret_code_addr;
	
	if (IsUnValidPtr(player_entity->world_))
	{
		assert(false);
		return ret_code_addr;
	}
	const auto& objs = player_entity->world_->obj_mgr_.objs_;
	auto obj_it = objs.find(msg_on_move->entity_.entity_id_);
	if (obj_it == objs.end())
	{
		//assert(false);
		return ret_code_addr;
	}
	auto the_obj = obj_it->second;
	if (IsUnValidPtr(the_obj))
	{
		assert(false);
		return ret_code_addr;
	}
	if (!the_obj->IsMonster())
		return ret_code_addr;
	if (IsUnValidPtr(player_entity->attr_pos_))
	{
		assert(false);
		return ret_code_addr;
	}
	auto self_pos = &player_entity->attr_pos_->pos_;
	auto self_monster_distance = self_pos->Distance(msg_on_move->pos_src_);
	//if (self_monster_distance > 59)
	if (self_monster_distance > 40)
		return ret_code_addr;
	auto the_obj_actor = the_obj->GetActor();
	if (!the_obj_actor)
	{
		assert(false);
		return ret_code_addr;
	}
	auto& the_skill_mgr = the_obj_actor->GetMySkillMgr();
	if (CD_kSkillId_NormalHit == msg_on_move->skill_id_)
	{
		//空着手的怪物，普通技能有可能会扔石头
		if (kWST_EmptyWeapon == the_skill_mgr.weapon_skill_type_)
		{
			if (self_monster_distance > 18)
				return ret_code_addr;
		}
		else if (kWST_GongJian != the_skill_mgr.weapon_skill_type_)
		{
			if (self_monster_distance > 15)
				return ret_code_addr;
		}
	}
	if (!calcutor->CanShooted(*self_pos, (float)11.1, msg_on_move->pos_dst_, msg_on_move->pos_src_))
		return ret_code_addr;
	stCD_VecInt out_res;
	auto calc_res = calcutor->GetAvoidPos(*self_pos, 
		27, 27, 7,
		calcutor->GetLovedDstPos(),
		msg_on_move->pos_src_, msg_on_move->pos_dst_, out_res);
#ifdef _DEBUG
	auto the_obj_render = the_obj->GetRender();
	if (!the_obj_render)
	{
		assert(false);
		return ret_code_addr;
	}
	auto skill_info = the_skill_mgr.FindSkill(msg_on_move->skill_id_);
	if (!skill_info)
	{
		assert(false);
		return ret_code_addr;
	}
	pt_cwsz skill_name = L"";
	auto active_skill = skill_info->GetActiveSkillDb();
	if (active_skill)
		skill_name = active_skill->ch_name_;
	if (!calc_res)
	{
		LOG_O(Log_debug) << "ToAvoidDamage计算躲避坐标失败，[" << the_obj_render->name_ << "]与自己距离过近，原始距离："
			<< self_pos->Distance(msg_on_move->pos_src_) << "，目标的距离：" << self_pos->Distance(msg_on_move->pos_dst_)
			<< "，技能名：" << CD_SafeName(skill_name) << ",SkillInfoAddr:" << skill_info
			<< ",SkillId:" << (pt_pcvoid)skill_info->skill_id_
			<< ",GrantedEffectsPerLevelAddr:" << skill_info->granted_effects_per_level_.tbl_item_;
		return ret_code_addr;
	}
	
#endif
	if (!calc_res)
		return ret_code_addr;
	if (*self_pos != out_res && !calcutor->MoveTo(out_res, msg_on_move->entity_.entity_id_, msg_on_move->skill_id_))
	{
		assert(false);
		return ret_code_addr;
	}
	return ret_code_addr;*/
}
void CD_Interface::Hooks::HookOnMoveToAvoidDamage()
{
	HOOKER_SetCodeHooker(Hooker_HookOnMoveToAvoidDamage, CD_EXE_DATA(g_h_recv_on_move_), Hooker_HookOnMoveToAvoidDamage);
}

void CD_Interface::Hooks::SetRecvOnMoveMsg(OnRecvMoveMsg* reciever)
{
	g_on_recv_move_msg = reciever;
}

static OnHookAfterInitMap* g_on_init_map_sink = nullptr;
static void* Hooker_BeforeInitMap(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto sink = g_on_init_map_sink;
	if (!sink)
		return hooker.GetCodeOrig();
	auto in_game_state = CD_Interface::GetInGameState();
	if (!in_game_state)
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto new_logic = in_game_state->tmp_logic_;
	if (IsUnValidPtr(new_logic))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if (IsUnValidPtr(new_logic->world_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	sink->OnBeforeInitMap(*new_logic, *new_logic->world_);
	return hooker.GetCodeOrig();
}
static void* Hooker_AfterInitMap(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto sink = g_on_init_map_sink;
	if (!sink)
		return hooker.GetCodeOrig();
	sink->OnAfterInitMap();
	return hooker.GetCodeOrig();
}
void CD_Interface::Hooks::HookInitMap()
{
	HOOKER_SetCodeHooker(Hooker_BeforeInitMap, CD_EXE_DATA(g_h_before_init_game_map_), Hooker_BeforeInitMap);
	HOOKER_SetCodeHooker(Hooker_AfterInitMap, CD_EXE_DATA(g_h_after_init_game_map_), Hooker_AfterInitMap);
}

void CD_Interface::Hooks::SetOnInitMapSink(OnHookAfterInitMap* sink)
{
	g_on_init_map_sink = sink;
}

/*
1:下雨，但有点紧张
2:下雨，比较轻松
3、4：海边的音效，3比较紧张，4比较轻松
5：比较亮，比如轻松
*/
static const pt_dword kModifiedGameEnvId = 1;
static void* Hooker_ModifyGameEnvAtChageRoom(IHooker& hooker, JmpHookerAmbient& ambient)
{
	ambient.eax_ = kModifiedGameEnvId;
	return hooker.GetCodeOrig();
}
static void* Hooker_ModifyGameEnvAtChageMap(IHooker& hooker, JmpHookerAmbient& ambient)
{
	static_assert(kModifiedGameEnvId > 0, "");
	auto game_env = (stCD_TblItemEnvironmentsPair*)(ambient.eax_);
	if (IsUnValidPtr(game_env))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if (IsUnValidPtr(game_env->db_))
		return hooker.GetCodeOrig();
	auto& tbl_items = game_env->db_->GetDbTbl()->tbl_items_;
	if (tbl_items.size() < kModifiedGameEnvId)
		return hooker.GetCodeOrig();
	ambient.ecx_ = (pt_dword)&tbl_items[kModifiedGameEnvId - 1];
	return hooker.GetCodeOrig();
}
void CD_Interface::Hooks::HookModifyGameEnv()
{
	//如果添加了此处的hook，则会看不到物品的所有属性。
	return;
	/*HOOKER_SetCodeHooker(Hooker_ModifyGameEnvAtChageRoom, CD_EXE_DATA(g_h_modify_game_env_id_at_change_room_), Hooker_ModifyGameEnvAtChageRoom);
	HOOKER_SetCodeHooker(Hooker_ModifyGameEnvAtChageMap, CD_EXE_DATA(g_h_modify_game_env_id_at_change_map_), Hooker_ModifyGameEnvAtChageMap);*/
}
#ifdef _DEBUG
//////////////////////////////////////////////////////////////////////////
static std::set<pt_word> g_msg_ids_ignore_;
static pt_word g_msg_id_break_ = 0;
static boost::mutex g_mutex_msg_ids_;
bool g_enable_analysis_recv_packet_ = false;
//////////////////////////////////////////////////////////////////////////
static std::set<pt_word> g_send_msg_ids_ignore_;
static pt_word g_send_msg_id_break_ = 0;
static boost::mutex g_send_mutex_msg_ids_;
bool g_enable_analysis_send_packet_ = false;
//////////////////////////////////////////////////////////////////////////
static std::set<pt_word> g_send_msg_ids_blocked_;
static boost::mutex g_send_mutex_msg_ids_blocked_;
bool g_enable_analysis_send_packet_blocked_ = false;
//////////////////////////////////////////////////////////////////////////
static void* Hooker_AnalysisRecvPacket(IHooker& hooker, JmpHookerAmbient& ambient)
{
	if (g_enable_analysis_recv_packet_)
	{
		auto msg_id = (pt_word)ambient.edx_;
		boost::unique_lock<boost::mutex> locker(g_mutex_msg_ids_);
		auto it = g_msg_ids_ignore_.find(msg_id);
		if (it == g_msg_ids_ignore_.end())
		{
			LOG_O(Log_trace) << "收到包,msg_id:" << (pt_pcvoid)msg_id;
		}
		if (g_msg_id_break_ && g_msg_id_break_ == msg_id)
		{
			FnForOd("AnalysisRecvPacket");
		}
	}	
	return hooker.GetCodeOrig();
}
#endif
static void* Hooker_AnalysisSendPacket(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto hooker_obj = Hooks::GetInstance();
	if (!hooker_obj)
	{
		assert(false);
		return hooker;
	}
	auto game_socket = (stCD_Socket*)ambient.ecx_;
	if (IsUnValidPtr(game_socket))
	{
		assert(false);
		return hooker;
	}
	auto msg_data = (char*)game_socket->GetSendMsgData();
	if (!msg_data)
	{
		assert(false);
		return hooker;
	}
	auto msg_size = game_socket->GetSendMsgSize();
	if (0 == msg_size)
		return hooker;

#ifdef _DEBUG
	if (g_enable_analysis_send_packet_)
	{
		if (msg_size < 2)
		{
			assert(false);
			return hooker;
		}
		auto msg_id = *(pt_word*)msg_data;
		msg_id = ntohs(msg_id);
		boost::unique_lock<boost::mutex> locker(g_send_mutex_msg_ids_);
		auto it = g_send_msg_ids_ignore_.find(msg_id);
		if (it == g_send_msg_ids_ignore_.end())
		{
			LOG_O(Log_trace) << "发包,socketType:" << game_socket->GetSocketType()
				<< ",socketAddr:" << game_socket
				<< ",msg_id:" << (pt_pcvoid)msg_id << ", 大小：" << msg_size << ", msg_data : " << (void*)msg_data;
		}
		if (g_send_msg_id_break_ && g_send_msg_id_break_ == msg_id)
		{
			FnForOd("AnalysisSendPacket");
		}
	}
#endif

#ifdef ANTI_BY_NET
	if (game_socket->GetSocketType() != kST_InGameState)
		return hooker;

	auto& packet_mgr = hooker_obj->GetSendPacketMgr();
	while (true)
	{
		if (msg_size < 2)
		{
			assert(false);
			break;
		}
		auto msg_id = *(pt_word*)msg_data;
		msg_id = ntohs(msg_id);
		//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
		if (g_enable_analysis_send_packet_blocked_)
		{
			boost::unique_lock<boost::mutex> locker(g_send_mutex_msg_ids_blocked_);
			auto it = g_send_msg_ids_blocked_.find(msg_id);
			if (it != g_send_msg_ids_blocked_.end())
			{
				LOG_O(Log_trace) << "阻塞发包,msg_id:" << (pt_pcvoid)msg_id;
				game_socket->ClearSendingBuffer();
				ambient.eax_ = ambient.ecx_;
				return NakedRetP1;
			}
		}
#endif
		//////////////////////////////////////////////////////////////////////////
		msg_size -= sizeof(pt_word);
		msg_data += sizeof(pt_word);
		auto res = packet_mgr.HandleMsg(hooker.GetCodeOrig(), msg_id, msg_data, msg_size);
		if (res.reason_ == CmnRecvPacketMgr::kHMR_HasNotMsgId)
		{
			LOG_O(Log_debug) << "不在白名单内，阻止发包：" << pt_pcvoid(msg_id);
			game_socket->ClearSendingBuffer();
			ambient.eax_ = ambient.ecx_;
			return NakedRetP1;
		}
		else if (res.reason_ == CmnRecvPacketMgr::kHMR_DoDirectFaild)
		{
			LOG_O(Log_debug) << "发包check失败：" << pt_pcvoid(msg_id);
			assert(false);
			game_socket->ClearSendingBuffer();
			ambient.eax_ = ambient.ecx_;
			return NakedRetP1;
		}
		else if (res.reason_ != CmnRecvPacketMgr::kHMR_Succeed)
			return res.ret_addr_;
		msg_size -= res.read_cnt_;
		msg_data += res.read_cnt_;
		if (msg_size == 0)
			return res.ret_addr_;
	}
#endif
	return hooker;
}

void CD_Interface::TestAnalysisRecvPacket(bool enable, pt_word break_id, const luabind::object& tbl)
{
#ifdef _DEBUG
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return;
	}
	boost::unique_lock<boost::mutex> locker(g_mutex_msg_ids_);
	g_msg_ids_ignore_.clear();
	for (auto& v : tbl)
	{
		auto id = LuaObjCast(v, (pt_word)0);
		if (0 == id)
			continue;
		g_msg_ids_ignore_.insert(id);
	}
	g_msg_id_break_ = break_id;
	g_enable_analysis_recv_packet_ = enable;
#endif
}

void CD_Interface::TestAnalysisSendPacket(bool enable, pt_word break_id, const luabind::object& tbl)
{
#ifdef _DEBUG
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return;
	}
	boost::unique_lock<boost::mutex> locker(g_send_mutex_msg_ids_);
	g_send_msg_ids_ignore_.clear();
	for (auto& v : tbl)
	{
		auto id = LuaObjCast(v, (pt_word)0);
		if (0 == id)
			continue;
		g_send_msg_ids_ignore_.insert(id);
	}
	g_send_msg_id_break_ = break_id;
	g_enable_analysis_send_packet_ = enable;
#endif
}

void CD_Interface::DebugDbChests()
{
	auto p = GetDbMgr().GetDbChests();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugQuestLvlStatePanel()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return;
	}
	auto the_panel = main_ui->GetQuestLvlStatePanel();
	if (!the_panel)
		return;
	the_panel->DebugInfo();
}

void CD_Interface::TestShowUi(pt_dword ui_addr, bool show)
{
	if (IsUnValidPtr(ui_addr))
	{
		assert(false);
		return;
	}
	((stCD_UiObjBase*)ui_addr)->is_shown_ = show;
}

void CD_Interface::TestShowUi_(pt_dword ui_addr)
{
	if (IsUnValidPtr(ui_addr))
	{
		assert(false);
		return;
	}
	auto ptr = &((stCD_UiObjBase*)ui_addr)->is_shown_;
	*ptr = !*ptr;
}

stCD_DlgSelfMapLocator* CD_Interface::GetDlgSelfMapLocator()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgSelfMapLocator();
}

void CD_Interface::DebugDlgSelfMapLocator()
{
	auto p = GetDlgSelfMapLocator();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DbgPlayerPosModel()
{
	auto w = GetWorld();
	if (!w)
		return;
	w->map_data_.DbgPlayerPosModel();
}

stCD_SceneLoginState* CD_Interface::GetLoginState()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	return ptr->GetLoginState();
}

void CD_Interface::DebugLoginState()
{
	auto l = GetLoginState();
	if (l)
		l->DebugInfo();
}

void CD_Interface::DebugPreGameState()
{
	auto pre = GetPreGameState();
	if (pre)
		pre->DebugInfo();
}

void CD_Interface::DebugSelectRoleTop()
{
	auto p = GetSelectRoleTop();
	if (p)
		p->DebugInfo();
}

void CD_Interface::DebugUiBottom()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
		return;
	auto ui_bottom = main_ui->GetUiBottom();
	if (ui_bottom)
		ui_bottom->DebugInfo();
}

void CD_Interface::DebugTheDbWorldArea(const std::string& world_area_name)
{
	auto db_world_area = GetDbMgr().GetDbWorldArea();
	if (!db_world_area)
	{
		assert(false);
		return;
	}
	auto tab = db_world_area->GetDbTbl();
	if (IsUnValidPtr(tab))
	{
		assert(false);
		return;
	}

	if (!tab->tbl_items_.IsValid())
	{
		assert(false);
		return;
	}
	auto w_world_area_name = GbkToWideChar(world_area_name);
	for (auto& world_area : tab->tbl_items_)
	{
		if (world_area.ch_map_name_ == w_world_area_name)
		{
			world_area.DebugInfo();
			LOG_O(Log_debug) << "--------------------------------";
		}
	}
}

stCD_Socket* CD_Interface::GetInGameStateSocket()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		//assert(false);
		return nullptr;
	}
	return ptr->GetInGameStateSocket();
}

stCD_Socket* CD_Interface::GetPreGameStateSocket()
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		//assert(false);
		return nullptr;
	}
	return ptr->GetPreGameStateSocket();
}

void CD_Interface::TestAnalysisSendPacketBlocked(bool enable, const luabind::object& tbl)
{
#ifdef _DEBUG
	using namespace luabind;
	if (LUA_TTABLE != type(tbl))
	{
		assert(false);
		return;
	}
	boost::unique_lock<boost::mutex> locker(g_send_mutex_msg_ids_blocked_);
	g_send_msg_ids_blocked_.clear();
	for (auto& v : tbl)
	{
		auto id = LuaObjCast(v, (pt_word)0);
		if (0 == id)
			continue;
		g_send_msg_ids_blocked_.insert(id);
	}
	g_enable_analysis_send_packet_blocked_ = enable;
#endif
}

bool CD_Interface::TopStateSameName(const std::wstring& state_name)
{
	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return false;
	}
	return ptr->TopStateSameName(state_name);
}

stCD_DlgLabyrinthSelect* CD_Interface::GetDlgLabyrinthSelect()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgLabyrinthSelect();
}

void CD_Interface::DebugDlgLabyrinthSelect()
{
	auto dlg = GetDlgLabyrinthSelect();
	if (dlg)
		dlg->DebugInfo();
}

stCD_DlgTrialPlaque* CD_Interface::GetDlgTrialPlaque()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgTrialPlaque();
}

void CD_Interface::DebugDlgTrialPlaque()
{
	auto dlg = GetDlgTrialPlaque();
	if (dlg)
		dlg->DebugInfo();
}

stCD_DlgInstanceMgr* CD_Interface::GetDlgInstanceMgr()
{
	auto main_ui = GetInGameMainUi();
	if (!main_ui)
	{
		assert(false);
		return nullptr;
	}
	return main_ui->GetDlgInstanceMgr();
}

void CD_Interface::DebugDlgStash()
{
	auto dlg = GetDlgStash();
	if (dlg)
		dlg->DebugInfo();
}

void CD_Interface::DebugDlgSelectRole()
{
	auto ui = GetPreInGameTopUi();
	if (!ui)
		return;
	auto dlg = ui->GetDlgSelectRole();
	if (!dlg)
		return;
	dlg->DebugInfo();
}

void CD_Interface::DebugCreateCharacterState()
{
	auto c = GetCreateCharacterState();
	if (c)
		c->DebugInfo();
}

void CD_Interface::Hooks::HookAnalysisRecvPacket()
{
#ifdef _DEBUG
	HOOKER_SetCodeHooker(Hooker_AnalysisRecvPacket, CD_EXE_DATA(g_h_AnalysisRecvPacket_), Hooker_AnalysisRecvPacket);
#endif
}

void CD_Interface::Hooks::HookAnalysisSendPacket()
{
	HOOKER_SetCodeHooker(Hooker_AnalysisSendPacket, CD_EXE_DATA(g_c_SendPacket_), Hooker_AnalysisSendPacket);
}

static void* Hooker_SkillObjUpdateMousePos(IHooker& hooker, JmpHookerAmbient& ambient)
{
	char* ret_addr = (char*)hooker.GetHookedCodeAddr();
	return ret_addr + g_so_skill_obj_update_mouse_pos_jmp_addr_;
}
void CD_Interface::Hooks::HookSkillObjUpdateMousePos()
{
	HOOKER_SetCodeHooker(Hooker_SkillObjUpdateMousePos, CD_EXE_DATA(g_h_skill_obj_update_mouse_pos_), Hooker_SkillObjUpdateMousePos);
}

static void* Hooker_GetForegroundWindow(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto wnd = GetGameWndHandle();
	if (::IsWindow(wnd))
	{
		ambient.eax_ = (pt_dword)wnd;
		return NakedRetP0;
	}
	return hooker;
}
void CD_Interface::Hooks::HookGetForegroundWindow()
{
	auto user32 = LoadLibraryA("user32.dll");
	if (!user32)
	{
		assert(false);
		return;
	}
	auto the_addr = GetProcAddress(user32, "GetForegroundWindow");
	if (!the_addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_GetForegroundWindow, the_addr, Hooker_GetForegroundWindow);
}

static void* Hooker_OperateCommand(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto str_cmd = *(const GameWString**)(ambient.esp_ + 4);
	if (IsUnValidPtr(str_cmd))
	{
		assert(false);
		return hooker;
	}
	auto command = str_cmd->c_str();
	if (stCD_GameLogic::is_self_operate_command)
	{
		LOG_O(Log_trace) << "自己的操作命令：" << command;
		return hooker;
	}
	if (!stCD_GameLogic::enable_ignore_operate_command)
	{
		LOG_O(Log_trace) << "游戏发送指令：" << command;
		return hooker;
	}
	LOG_O(Log_debug) << "游戏发送指令，忽略之：" << command;
	ambient.eax_ = 1;
	return NakedRetP1;
}
void CD_Interface::Hooks::HookOperateCommand()
{
	HOOKER_SetCodeHooker(Hooker_OperateCommand, CD_EXE_DATA(g_c_logic_oper_command_), Hooker_OperateCommand);
}

static void* Hooker_Test(IHooker& hooker, JmpHookerAmbient& ambient)
{
	assert(false);
	return hooker;
}

void CD_Interface::Hooks::HookForTest()
{
	//HOOKER_SetCodeHooker(Hooker_Test, CD_EXE_DATA(g_h_create_process_), Hooker_Test);
}

static void* Hooker_IsWow64Process(IHooker& hooker, JmpHookerAmbient& ambient)
{
	ambient.eax_ = 1;
	auto the_process = (PBOOL)*(void**)(ambient.esp_ + 8);
	if (the_process)
		*the_process = FALSE;
	return NakedRetP2;
}
void CD_Interface::Hooks::HookIsWow64Process()
{
	//因为--disable-client-switching的缘故，所以不必要此hook了。
	/*auto kernel32 = LoadLibraryA("Kernel32.dll");
	if (!kernel32)
	{
		assert(false);
		return;
	}
	auto the_addr = GetProcAddress(kernel32, "IsWow64Process");
	if (!the_addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_IsWow64Process, the_addr, Hooker_IsWow64Process);*/
}

static void* Hooker_DisableClientSwitching(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto process_info = *(stCD_ProcessInfo**)ambient.ecx_;
	if (IsUnValidPtr(process_info))
	{
		assert(false);
		return hooker;
	}
	process_info->DebugInfo();
	return hooker;
}
void CD_Interface::Hooks::HookDisableClientSwitching()
{
#ifdef _DEBUG
#if 0
	HOOKER_SetCodeHooker(Hooker_DisableClientSwitching, CD_EXE_DATA(g_h_disable_client_switching_), Hooker_DisableClientSwitching);
#endif
#endif
}

static void* Hooker_HFAC_Thrd(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto thrd = *(stCD_ThrdParam**)(ambient.esp_ + 4);
	if (IsUnValidPtr(thrd))
	{
		assert(false);
		return hooker;
	}
	thrd->DebugInfo();
	return hooker;
}
static void* Hooker_HFAC_Cpuid8(IHooker& hooker, JmpHookerAmbient& ambient){
	JmpHookerAmbient ambient_tmp;
	ambient_tmp.eax_ = *(pt_dword*)(ambient.ebp_ + 8);
	auto res = HookSomeThing::GetMe().DoAntiCpuid(ambient_tmp);
	if (res == HookSomeThing::kACR_RetPassCpuid)
	{
		ambient.AssignRegisterABCD(ambient_tmp);
	}
	return hooker;
}
static void* Hooker_HFAC_Connect(IHooker& hooker, JmpHookerAmbient& ambient){
	auto addr_name = *(SOCKADDR_IN**)(ambient.esp_ + 8);
	auto addr_name_len = *(int*)(ambient.esp_ + 12);
	if (addr_name_len != sizeof(SOCKADDR_IN))
	{
		assert(false);
		return hooker;
	}
	auto port = ntohs(addr_name->sin_port);
#ifdef _DEBUG
	auto ret_addr = *(void**)ambient.esp_;
	char module_name[MAX_PATH];
	if (!GetAddressModuleName(ret_addr, module_name, sizeof(module_name)))
	{
		assert(false);
		return hooker;
	}
	auto ip = inet_ntoa(addr_name->sin_addr);
	if (!ip)
	{
		assert(false);
		return hooker;
	}
#endif
	if (port == 12995		//patch102.pathofexile.com:12995
		|| port == 20481	//PreInGame时的游戏连接
		|| port == 6112		//InGameState的游戏连接
		)
	{
#ifdef _DEBUG
		LOG_O(Log_trace) << "允许该Connect,module:" << module_name << ",ip:" << ip << ",port:" << port;
#endif
		return hooker;
	}
#ifdef _DEBUG
	LOG_O(Log_trace) << "禁止该Connect,module:" << module_name << ",ip:" << ip << ",port:" << port;
#endif
	ambient.eax_ = WSAECONNREFUSED;
	return NakedRetP0;
}
static void* Hooker_HFAC_TakeupItem(IHooker& hooker, JmpHookerAmbient& ambient){
	auto msg_takeup_item = (stCDGS_TakeupItem*)ambient.ecx_;
	if (IsUnValidPtr(msg_takeup_item))
	{
		assert(false);
		return hooker;
	}
	if (msg_takeup_item->msg_id_ != kMIS_TakeupItem)
	{
		assert(false);
		return hooker;
	}
	if (msg_takeup_item->ukn_flag_ != kSIOF_Normal)
	{
		assert(false);
		msg_takeup_item->ukn_flag_ = kSIOF_Normal;
		return hooker;
	}
	return hooker;
}
void CD_Interface::Hooks::HookForAntiCheckPacket()
{
#ifdef VER_GF_
	return;
#endif
	//HOOKER_SetCodeHooker(Hooker_HFAC_Thrd, CD_EXE_DATA(g_h_hfac_thrd_), Hooker_HFAC_Thrd);
	//////////////////////////////////////////////////////////////////////////
	//************tag_获得机器码_cpuid
	auto& hook_some = HookSomeThing::GetMe();
	hook_some.AntiCpuid<1>(CD_EXE_DATA(g_h_cpuid1_));
	hook_some.AntiCpuid<2>(CD_EXE_DATA(g_h_cpuid2_));
	hook_some.AntiCpuid<3>(CD_EXE_DATA(g_h_cpuid3_));
	hook_some.AntiCpuid<4>(CD_EXE_DATA(g_h_cpuid4_));
	hook_some.AntiCpuid<5>(CD_EXE_DATA(g_h_cpuid5_));
	hook_some.AntiCpuid<6>(CD_EXE_DATA(g_h_cpuid6_));
	hook_some.AntiCpuid<7>(CD_EXE_DATA(g_h_cpuid7_));
	HOOKER_SetCodeHooker(Hooker_HFAC_Cpuid8, CD_EXE_DATA(g_h_cpuid8_), Hooker_HFAC_Cpuid8);
	//////////////////////////////////////////////////////////////////////////
	auto ws2_32 = LoadLibraryA("Ws2_32.dll");
	if (!ws2_32)
	{
		assert(false);
		return;
	}
	auto the_fn_addr = GetProcAddress(ws2_32, "connect");
	if (!the_fn_addr)
	{
		assert(false);
		return;
	}
#ifdef ANTI_BY_NET
	//hook了连接，返回选择角色时有可能会不正常，比如游戏崩掉
	HOOKER_SetCodeHooker(Hooker_HFAC_Connect, the_fn_addr, Hooker_HFAC_Connect);
#endif
	HOOKER_SetCodeHooker(Hooker_HFAC_TakeupItem, CD_EXE_DATA(g_h_build_takeup_item_packet_), Hooker_HFAC_TakeupItem);
}

CmnRecvPacketMgr& CD_Interface::Hooks::GetSendPacketMgr()
{
	return send_packet_mgr_;
}

static void* Hooker_AntiCollision(IHooker& hooker, JmpHookerAmbient& ambient){
	if (LuasScriptEchoC::IsRunning())
		return NakedRetP1;
	return hooker;
}
static void* Hooker_AntiModifyMapData(IHooker& hooker, JmpHookerAmbient& ambient){
#ifdef _DEBUG
	{
		/*auto entity = *(stCD_EntityPosBased**)(ambient.esp_ + 4);
		if (IsUnValidPtr(entity))
		{
			assert(false);
			return hooker;
		}
		auto entity_db_path = entity->GetDbPath();
		if (!entity_db_path)
		{
			assert(false);
			return hooker;
		}
		LOG_O(Log_trace) << "ModifyMapData,EntityAddr:" << entity << ",DbPath:" << entity_db_path->c_str();*/
		//entity->DebugInfo();
	}
#endif
	if (!LuasScriptEchoC::IsRunning())
		return hooker;
	//auto map_data = (stCD_MapData*)ambient.ecx_;
	auto entity = *(stCD_EntityPosBased**)(ambient.esp_ + 4);
	if (IsUnValidPtr(entity))
	{
		assert(false);
		return hooker;
	}
	auto entity_db_path = entity->GetDbPath();
	if (!entity_db_path)
	{
		assert(false);
		return hooker;
	}
	if (entity_db_path->compare(L"Metadata/Terrain/Labyrinth/Traps/LabyrinthCascadeSpikeTrap") == 0	//迷宫里的长钉
		)
	{
		return NakedRetP1;
	}
	return hooker;
}
void CD_Interface::Hooks::HookAntiCollision()
{
	HOOKER_SetCodeHooker(Hooker_AntiCollision, CD_EXE_DATA(g_h_is_collisioned_), Hooker_AntiCollision);
	HOOKER_SetCodeHooker(Hooker_AntiCollision1, CD_EXE_DATA(g_h_is_in_collision_range_), Hooker_AntiCollision);
	HOOKER_SetCodeHooker(Hooker_AntiModifyMapData, CD_EXE_DATA(g_h_map_data_modify_by_entity_), Hooker_AntiModifyMapData);
}

static bool g_enable_control_pressed_ = false;
static void* Hooker_GetKeyState(IHooker& hooker, JmpHookerAmbient& ambient){
	if (g_enable_control_pressed_)
	{
		auto virt_key = *(int*)(ambient.esp_ + 4);
		if (virt_key == VK_CONTROL)
		{
			ambient.eax_ = 0xFFFFFF80;
			return NakedRetP1;
		}
	}
	return hooker;
}
void CD_Interface::Hooks::HookGetKeyState()
{
	auto lib = LoadLibraryA("user32.dll");
	if (!lib)
	{
		assert(false);
		return;
	}
	auto addr = GetProcAddress(lib, "GetKeyState");
	if (!addr)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_GetKeyState, addr, Hooker_GetKeyState);
}

void CD_Interface::Hooks::EnableDummyCtrlPress(bool ctrl_pressed)
{
	g_enable_control_pressed_ = ctrl_pressed;
}

static void* Hooker_OnModifyLifeData(IHooker& hooker, JmpHookerAmbient& ambient){
	auto sink = g_interface_sink_;
	if (!sink)
		return hooker;
	auto entity = GetPlayerEntity();
	if (!entity)
		return hooker;
	auto life = entity->GetLife();
	if (!life)
		return hooker;
	auto life_data = (stCD_LifeData*)ambient.edi_;
	if (IsUnValidPtr(life_data))
	{
		assert(false);
		return hooker;
	}
	if (life_data->parent_ != life)
		return hooker;
	if (life_data == &life->hp_)
		sink->OnModifyHp(ambient.eax_);
	else if (life_data == &life->mp_)
		sink->OnModifyMp(ambient.eax_);
	return hooker;
}
void CD_Interface::Hooks::HookModifyLifeData()
{
	HOOKER_SetCodeHooker(Hooker_OnModifyLifeData, CD_EXE_DATA(g_h_modify_life_data_), Hooker_OnModifyLifeData);
}

void CD_Interface::Hooks::SetInterfaceSink(InterfaceSink* sink)
{
	g_interface_sink_ = sink;
}

static void* Hooker_PromptInfo(IHooker& hooker, JmpHookerAmbient& ambient){
	auto sink = g_interface_sink_;
	if (!sink)
		return hooker;
	auto prompt_info = (stCDGR_PromptInfo*)ambient.ebp_;
	if (IsUnValidPtr(prompt_info))
	{
		assert(false);
		return hooker;
	}
	if (prompt_info->prompt_id_ == stCDR_PromptInfo::kRemaining)
	{
		sink->OnPromptRemaining(prompt_info->GetRemaining());
	}
	return hooker;
}
void CD_Interface::Hooks::HookPromptInfo()
{
	HOOKER_SetCodeHooker(Hooker_PromptInfo, CD_EXE_DATA(g_h_prompt_info_), Hooker_PromptInfo);
}

static void* Hooker_CreateEvent(IHooker& hooker, JmpHookerAmbient& ambient){
	auto event_name = *(LPCWSTR*)(ambient.esp_ + 0x10);
	if (!IsUnValidPtr(event_name))
	{
		LOG_O(Log_trace) << "Hooker_CreateEvent, event_name:" << event_name;
	}
	return hooker;
}
static void* Hooker_CreateEventEx(IHooker& hooker, JmpHookerAmbient& ambient){
	auto event_name = *(LPCWSTR*)(ambient.esp_ + 8);
	if (!IsUnValidPtr(event_name))
	{
		LOG_O(Log_trace) << "Hooker_CreateEventEx, event_name:" << event_name;
	}
	return hooker;
}
typedef struct{
	char*	pScrCur;
	int		nRemainBufLen;
	char*	pScrBuf;
}JFZRBASE_SPRINTF, *PJFZRBASE_SPRINTF;
static void* Hooker_Sprintf(IHooker& hooker, JmpHookerAmbient& ambient){
	auto ptheScrBuf = (PJFZRBASE_SPRINTF)ambient.edi_;
	if (IsUnValidPtr(ptheScrBuf))
	{
		assert(false);
		return hooker;
	}
	if (strstr(ptheScrBuf->pScrBuf, "Global-Event-") != 0)
	{
		char szProcessId[256] = { 0 };
		sprintf_s(szProcessId, "%p", GetCurrentProcessId());

		int nLen = strlen(szProcessId);
		memcpy(ptheScrBuf->pScrCur, szProcessId, nLen);
		ptheScrBuf->nRemainBufLen -= nLen;
		ptheScrBuf->pScrCur += nLen;
	}
	return hooker;
}
static void* Hooker_DuoKaiTwoCopies(IHooker& hooker, JmpHookerAmbient& ambient){
	return NakedRetP0;
}
void CD_Interface::Hooks::HookForDuoKai()
{
	/*auto lib = LoadLibraryA("Kernel32.dll");
	if (!lib)
	{
		assert(false);
		return;
	}
	auto fn = GetProcAddress(lib, "CreateEventW");
	if (!fn)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_CreateEvent, fn, Hooker_CreateEvent);

	fn = GetProcAddress(lib, "CreateEventExW");
	if (!fn)
	{
		assert(false);
		return;
	}
	HOOKER_SetCodeHooker(Hooker_CreateEventEx, fn, Hooker_CreateEventEx);*/

#ifdef VER_GF_
	HOOKER_SetCodeHooker(Hooker_Sprintf1, CD_Base_DATA(g_h_base_sprintf1_), Hooker_Sprintf);
	HOOKER_SetCodeHooker(Hooker_Sprintf2, CD_Base_DATA(g_h_base_sprintf2_), Hooker_Sprintf);
	HOOKER_SetCodeHooker(Hooker_Sprintf3, CD_Base_DATA(g_h_base_sprintf3_), Hooker_Sprintf);
#endif
	HOOKER_SetCodeHooker(Hooker_DuoKaiTwoCopies, CD_EXE_DATA(g_h_duo_kai_two_copies_), Hooker_DuoKaiTwoCopies);
}
//////////////////////////////////////////////////////////////////////////
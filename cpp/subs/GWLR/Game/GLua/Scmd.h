#pragma once
/*
@author zhp
@date 2016/5/29	22:57
@purpose for script command
*/
#include <Common/UsefulClass.h>
#include <luabind/object.hpp>
#include "Facility/GameObjMgr.h"
#include "Facility/GItem.h"

struct Env_InGame{
	bool operator()(){
		return true;
	}
};

struct Env_InVillage{
	bool operator()(){
		return true;
	}
};

struct Env_InFb{
	bool operator()(){
		return true;
	}
};

class scmd : public Singleton<scmd, Singleton_MakeMe>{
public:
	DWORD FindNpcByName(const luabind::object& obj_name, const luabind::object& obj_title);
	DWORD FindBeatableMonsterByName(const luabind::object& obj_name);
	DWORD FindTheBoss();
	bool WaitAppearBoss(int wait_time);
	bool SetRoleToBack(DWORD package_id, int steps);
	bool SetRoleToBack(const GameObjMgr_AnyObj::PacketIdT& package_id, int steps);
	bool OpenNpc(DWORD package_id);
	bool OpenNpc_Grid(DWORD package_id);
	bool AcceptTaskById(int task_id);
	bool GiveUpTask(int task_id);
	bool CommitTaskById(int task_id);
	bool DoNpcChat(int task_id);
	bool MoveToTheVillage(const std::string& dst_village_name, int room_num);
	bool MoveToTheFb(const std::string& fb_name);
	bool MoveToTheFbById(int fb_id);
	bool MoveToTheFb_Cat(const std::string& cat_fb_name);
	bool MoveToTheFb_King(const std::string& king_fb_name);
	bool MoveToTheFb_King_Cream(const std::string& king_fb_name, const std::string& unlock_item_name);
	void AddFbBlackList_Cat(int fb_id);
	void ClrFbBlackList_Cat();
	bool CanDoTheFb_Cat(const std::string& cat_fb_name);
	bool GiveUpFb();
	bool GiveUpXinShouXunLian();
	bool MoveToTheRoom(int room_num);
	int GainSupplyItem(const std::string& item_name);
	int HitTheMonsterToDie(DWORD packet_id, int sleep_time, bool is_any_direction, bool resume_hit_mode);
	int DestoryStaticBeatableMonster(const std::string& obj_name, int every_hit_cnt, int obj_cnt, int delay_time);
	void DebugInfoStaticBeatableObjs();
	bool IsValidGameObj(DWORD packet_id);
	bool IsValidGameObj(const GameObjMgr_AnyObj::PacketIdT& packet_id);
	bool CollectTheCollectableObj(DWORD packet_id, int collect_time);
	void AddFbBlackList(int fb_id);
	void ClrFbBlackList();
	bool AutoCollectNearestCollectableObjAsSameRoom(int collect_time);
	int AutoCollectOnceAllRoomCollectableObj(int collect_time);
	int AutoCollectOverAllObj(int collect_time);
	int AutoCollectSpecifc(int collect_time, int room_num, int obj_cnt, int every_collect_cnt, 
		const std::string& collect_obj_name, const std::string& gained_item, const luabind::object& do_after_every_collect);
	int AutoCollectSpecifc_Farm(int collect_time, int obj_cnt, int every_collect_cnt, const std::string& collect_obj_name);
	int AutoCollectOverTheObj(const std::string& obj_name, int collect_time);
	bool TakeInKnife();
	bool UsePackageItem(const std::string& item_name);
	int UseSomeItems(const std::string& item_name, int use_cnt);
	bool UseTaskItem(const std::string& item_name);
	bool ManuFactureItem(const std::string& item_name);
	bool WaitCrossedGraph(int wait_time);
	bool WaitRetCity(int wait_time);
	bool PressKeyZ();
	bool PressKeyQ();
	bool PressKeyW();
	bool PressKeyS();
	bool PressKeyA();
	bool PressKeyD();
	bool PressKeySpace();
	bool PressKeyEsc();
	bool CutNearestSkinOnce(int wait_time, int room_num);
	bool WaitForCutSkin(int wait_time);
	bool WaitTheCollectableObj(int wait_time, const std::string& obj_name, int room_num);
	int CutOverNearestSkin(int wait_time, int room_num);
	bool WaitToFree(int wait_time);
	void DebugInfoMonsterInfo();
	void DebugInfoAccetableTask();
	DWORD FindCollectObjByName(const std::string& obj_name);
	int AutoCollectOverNearestCollectableObjAsSameRoom(int collect_time);
	void DebugInfoPackageItemInfo();
	void DebugInfoCollectableObjs();
	void DebugInfoStaticObjs(const std::string& obj_name);
	void DebugInfoTheFbId(const std::string& fb_name);
	void DebuInfo_AllBodyPart();
	void DebugInfo_CurMapGates();
	void DebugInfo_King();
	void DebugInfo_KingCream();
	void DebugInfo_AllPlayers();
	void EnableRecvDebugInof(bool enable);
	std::string GetItemName(const GItem& item);
	int GetItemLvl(const GItem& item);
	int GetItemCnt(const GItem& item);
	int GetItemRoleLvl(const GItem& item);
	int GetTheItemTotalCnt(const std::string& item_name);
	bool IsTaiDaoType(const GItem& item);
	GItem GetSelfWeapon();
	bool IsValidItem(const GItem& item);
	GItem FindPakcetItem(const std::string& item_name);
	GItem FindStoreItem(const std::string& item_name);
	GItem FindTaskItem(const std::string& item_name);
	int GetTaskTotalItemCnt(const std::string& item_name);
	GItem FindItemFromPacketAndStore(const std::string& item_name);
	luabind::object GetCurFbAllRoomNum();
	bool DoCraft(const std::string& item_name);
	DWORD FindNearestNormalMonsterFirst(int the_room_num);
	bool IsTheBoss(DWORD packet_id);
	bool SetRoleToBackImpl(DWORD package_id, int steps, GNormalGameObjMgrBase* obj_mgr, bool is_any_direction);
	int KillOverTheRoomMonsters(int sleep_time, bool is_collect, int room_num);
	int KillTheMonsterAsCnt(const std::string& obj_name, int cnt, const std::string& use_item_name, int steps_at_use_item);
	bool BuyTheItemAsMaxCnt(const std::string& shop_category_name, const std::string& item_name,
		int the_buy_cnt, int max_cnt, const std::string& village_name, const std::string& npc_name, bool gold_limit);
	bool LvlUpEquip(const std::string& equip_name, int lvl_up_equip_idx);
	bool AutoLvlUpBodyEquips(const luabind::object& village_npc);
	bool MM_ExistTheItem(const std::string& material_name);
	bool MM_DoGainTheMaterial(const std::string& material_name, int cnt_limit);
	int AutoDoGainCurWeaponMaterial();
	void AddAutoLvlUpBlackList(const std::string& item_name);
	void ClrAutoLvlUpEquipBlackList();
	bool LeftButtonClick(int pos_x, int pos_y);
	bool CloseAllUi();
	bool IsValidTaskId(int task_id);
	int GetLeftAcceptableShangJinCntToday();
	luabind::object GetAnyAcceptableShangJinTaskId(const luabind::object& the_lua_param);
	luabind::object GetAnyCanDoShangJinTaskId(const luabind::object& the_lua_param);
	int GetAnyCommitableShangJinTaskId();
	luabind::object GetTaskToDstFbName(int task_id);
	luabind::object GetTaskToDstFbId(int task_id);
	std::string GetTheFbNameById(int fb_id);
	int GetTheFbIdByName(const std::string& fb_name);
	luabind::object GetTheTaskName(int task_id);
	bool MoveItem(int tab_category_src, const std::string& item_name, int tab_category_dst, int dst_slot_idx);
	bool StoreSomePackageItemsToStore(int reserve_cnt);
	bool GainItemFromStore(const std::string& item_name);
	bool PackageItemToStore(const std::string& item_name);
	int GainSomeItemsFromStore(const std::string& item_name, int cnt);
	void AddStoreWhiteList(const std::string& item_name);
	void ClrStoreWhiteList();
	bool EnterSelfFarm();
	bool EnterXinShou();
	bool LeaveFarm();
	void ClrHandleAtHitting();
	bool AddHandleAtHitting(const luabind::object& judge_func, const luabind::object& do_func);
	void ClrHandlersAtScriptStoped();
	bool AddHandlersAtScriptStoped(const luabind::object& func);
	int GetRoleCurHp();
	int HandleAllMails(const std::string& villige_name, const std::string& npc_name);
	int LvlUpCollectOfFarm(const std::string& collect_obj_name, int lvl_limit, bool is_open_npc);
	bool IsTimeToCollectFarm(const std::string& collect_name);
	std::string GetExistFarmSeedName();
	bool PlantFarmSeed(const std::string& seed_name, const std::string& land_name);
	bool SelectRoleByName(const std::string& role_name);
	bool SeletRoleByIdx(int role_idx);
	bool WaitForSelectableRole(int wait_time);
	bool AutoCreateRole();
	bool CreateTheRole(const std::string& role_name);
	bool AutoCreateOrSelectRole();
	bool IsAtSelectRole();
	bool IsAtInGame();
	bool ChangeRole();
	bool WearEquip(const std::string& item_name);
	void AddAutoWearBlackList(const std::string& item_name);
	void ClrAutoWearBlackList();
	void AddAutoWearWhiteList(const std::string& item_name, const std::string& item_type);
	void ClrAutoWearWhiteList();
	bool NeedCreateRole();
	int GetRoleCnt();
	bool WaitForTheJuQing(const std::string& str_ju_qing, int wait_time);
	int StepOverTheStaticObjs(const std::string& obj_name);
	bool AutoToDoTheSociatyTask(const std::string& task_lvl, const std::string& task_name, const luabind::object& do_func);
	bool SellToAuction(const std::string& item_name, int item_cnt, int begin_price, int smart_price, int relative_smart_price, int min_smart_price, int average_cnt, bool begin_price_direct);
	bool SellToOtherSide(const std::string& item_name, int item_cnt, int begin_price, int smart_price);
	int SmartBuy(const std::string& item_name, int seach_page_cnt, const std::string& seller, int price_limit, int buy_cnt);
	int SmartBuyOtherSide();
	int QuerySoldCnt(const std::string& item_name);
	int SellStored_PacketRubbishItems(int packet_reserve_cnt, int stored_reserve_cnt, const std::string& village_name, const std::string& npc_name);
	void SetItemFilterBlackList(const luabind::object& item_func);
	void SetItemFilterWhiteList(const luabind::object& item_func);
	void AddItemNameBlackList(const std::string& item_name);
	void ClrItemNameBlackList();
	void AddItemNameWhiteList(const std::string& item_name);
	void ClrItemNameWhiteList();
	void SetItemInfoWhiteList(int item_lvl, int item_quality);
	void SetCraftItemWhiteList(int item_lvl, int item_quality);
	void SetLvlUpEquipWhiteList(int item_lvl, int item_quality);
	void SetFactureWhiteList(int item_lvl, int item_quality);
	bool WaitRoleCanMove_ByPressMove(int wait_time);
	bool WaitRoleCanMove(int wait_time);
	bool WaitGameActive(int wait_time);
	void SetRoleToPos(float x, float y, float z);
	void SetDirFromDstObj(float x, float y, float z);
	bool IsTheNewRole();
	bool GetOffEquipByIdx(int equip_part_idx);
	int GetCurRoleRoom();
	void TestOutEquipMaterialFrom();
	bool WaitAppearTheNpc(const std::string& obj_name, int wait_time);
	bool WaitAppearBeatableMonster(const std::string& obj_name, int wait_time);
	void SetFatigueData(int fatigue_data);
	bool ZuoReQiQiu();
	std::string FindSuitableFbName();
	bool IsHotStatus();
	bool IsColdStatus();
	bool IsPoisoningStatus();
	bool SetAfterEnteredFb(const luabind::object& func);
	bool NeedMoDao();
	int SendMail(const luabind::object& tbl_items, int gold, const std::string& target_role_name, const std::string& village_name, const std::string& npc_name);
	int SendAnyMail(int item_reach_cnt, int gold, const std::string& target_role_name, const std::string& village_name, const std::string& npc_name);
	int GetRoleLvl();
	int GetRoleCurGold();
	int GetRoleCurSilver();
	void AddShangJinWhiteList(const std::string& task_name, const std::string& fb_diff_lvl);
	void ClrShangJinWhiteList();
	void AddShangJinDstFbBlackList(int fb_id);
	void ClrShangJinDstFbBlackList();
	void AddShangJinDstFbStableBlackList(const std::string& fb_name);
	void ClrShangJinDstFbStableBlackList();
	bool ArrangeItems_Store();
	int LeftShouLieQuanCnt();
	int GetCurFbValidConsumeMode();
	int G_GetLeftSingleExp();
	int G_GetLeftDoubleExp();
	void ChangeGameAc(const std::string& prompt_str);
	luabind::object HandleGameScene(const luabind::object& func);
	void SetMinMoneyToKeepBuy(int min_gold, int min_silver);
	bool AddNewFriend(const std::string& role_name);
	int GetError_FbSerIsFull();
	void SetCurDeleteRoleIdx(int role_idx);
	int GetCurDeleteRoleIdx();
	int GetCurRoleIdxAtRoleList();
	void DoDeleteRole(bool change_ac_at_delete_one_role);
	void SetCurSerLine(int cur_ser_line);
	bool DoDealFaceToFace(const luabind::object& func_pre_deal, const luabind::object& func_do_deal, const luabind::object& func_after_deal);
	bool Deal_RequestDeal(const std::string& player_name, int wait_time);
	bool Deal_UntilAcceptOneDealRequest(int time_out);
	bool Deal_MoveSomeItems_Gold(const luabind::object& tbl_items, int gold);
	bool Deal_LockDealTab();
	bool Deal_EnterDeal();
	bool IsNiRenState();
	bool HandleNiRenState(int time_out);
	bool IsRanShaoStatus();
	bool HandleRanShaoStatus(int time_out);
	bool IsErChouStatus();
	bool IsSnowManStatus();
	bool HandleSnowManState(int time_out);
	bool IsChanRaoStatus();
	bool HandleChanRaoStatus(int time_out);
	void SetFbConsumeMode(int mode, int min_silver);
	int AutoGainGiftBag();
	void SetCurBossHitBodyPart(const std::string& part_name);
	void AutoSetCurBossHitBodyPartAsTheTask(int task_id);
	bool AddMonsterBodyPartInfo(const std::string& monster_name, const std::string& part_name, int body_part_idx);
	void TestMatchBossBodyPartInfo(int part_id, int hit_cnt);
	void TestTheFbType(const luabind::object& fb_tbl, const luabind::object& fb_types);
	bool GainWeekShouLieQuan();
	void SetMonsterMaxHigh(float high);
	void AddCollectBlackList(const std::string& collect_name);
	void ClrCollectBlackList();
	int DoEveryObj_Bucket(const luabind::object& do_func, int do_max_cnt);
	luabind::object HandleRoleEnteredGame(const luabind::object& func);
	void DoChangeRunScriptLevel0(const std::string& new_script_name);
	std::string GetNextChangeScriptNameLevel0();
	int AutoWearSomeEquips(int equip_quality_limit, bool is_auto_gian);
	void AutoGainOneAutoWearWhiteListEquip();
	bool IsInVillage();
	bool OpenTheNpcOfVillage(const std::string& village_name, int room_num, const std::string& npc_name,
		const std::string& npc_title, bool cur_village_first);
	bool OpenVillageNpcById(const std::string& village_name, int room_num, int obj_db_id, bool cur_village_first);
	bool AutoMakeEquip(const std::string& equip_name, int equip_max_cnt);
	void ExchangeGameAc();
	bool IsInFb();
	bool SetHitMode_UsePacketItem(const std::string& item_name, int steps);
	bool SetCurTimeHitMode(const std::string& str_hit_mode);
	int GetTaskNpcId(bool is_accept, int task_id);
};
#pragma once
/*
@author zhp
@date 2017/2/5	22:33
@purpose for script command
*/
#include <Common/UsefulClass.h>
#include <luabind/object.hpp>
#include <AppMix/WgLuaVm.h>
#include "Facility/GItem.h"
#include "Facility/GType.h"
#include "Facility/GPackageItemFilter.h"
#include "Facility/GActions.h"

class MyLuaThrdData : public GameDllLuaThrdData{
public:
	explicit MyLuaThrdData(lua_State* lstate);
	typedef std::vector<std::string> StrCont;

protected:
	void BindGlobalFn() override;
	void BindClass() override;
	void BindScriptCommand(luabind::module_& mod) override;
	void BindConst() override;
	void BindTest() override;

	//配置
public:
	static void EnableAutoOpenChest(enOpenChestFlag flag);
	static void EnableAutoPickupItem(bool enable);
	static void EnableAutoOpenWaypoint(bool enable);
	static void EnableAutoOpenTransitionable(bool enable);
	static void EnableAutoLvlUpSkillGemByDef(bool enable);
	static void EnableAutoWearEquip(bool enble);
	//配置天赋树
	static void SetPassiveSkillTree(const luabind::object& job_skill_nodes);
	//设置默认创建角色的职业
	static void SetDefCreateProfessionType(const std::string& profession_name);
	//设置默认创建角色的区
	static void SetDefCreateGameArea(const std::string& game_area);
	//设置重置地图
	static void SetResetMap(bool reset);
	//设置试炼迷宫开启等级
	static void SetLabyrinthLevel(pt_int level);

	//物品配置
public:
	//创建组合过滤
	static GFilterItemPtr CreateGroupFilter(const luabind::object& tbl);
	//创建过滤对象
	static GFilterItemPtr CreateFilterObj(enItemFilterType filter_type, const luabind::object& tbl);
	//添加拾取过滤
	static void AddPickUpFilter(GFilterItemPtr filter_ptr);
	//添加存仓过滤		
	static void AddStoreFilter(GFilterItemPtr filter_ptr);
	//添加出售过滤
	static void AddSellFilter(GFilterItemPtr filter_ptrl);
	//添加洗练过滤
	static void AddAginRinseFilter(GFilterItemPtr filter_ptr);
	//设置自动使用药剂
	static void SetAutoUseFlask(float hp_rate, float mp_rate);
	//设置背包格子空余数量
	static void SetMainBagMinFree(int wight, int hight);
	//设置宝石连接
	static bool SetGemLink(pt_dword min_lvl, pt_dword max_lvl, const std::string& main_gem, const luabind::object& tbl);
	//设置穿戴武器类型
	static void SetWearWeaponType(enWeaponCompositeCate type);
	//清理所有宝石连接
	static void ClearAllGemLink();
	//设置评分
	static void SetItemScore(enGradeValue score_type, const luabind::object& tbl);
	//设置药品数量
	static void SetBagFlaskCnt(const luabind::object& tbl);
	//设置异界过滤
	static void SetYiJieFilter(enItemFilterType filter_type, const luabind::object& tbl);
	//设置套装数量
	static void SetEmboitementCnt(int cnt);
	//设置进入房间开启传送
	static void SetJoinRoomOpenWayReel(bool open);
	//清空物品属性解析
	static void ClearItemAttrParser();
	//物品属性解析
	static CSV_NodePtr CreateItemAttrParser(const std::string& pro_name);
	//清除过滤
	static void ClearFilter(int filter_type);
	//设置洗练装备
	static GItemAgainRinsePtr CreateItemTypeRinse(enCD_ItemType item_type, pt_dword min_lvl, pt_dword max_lvl);

	//核心
public:
	//移动到指定位置
	static bool SmartMoveTo(const stCD_VecInt& dst_pos);
	//全图杀怪
	static bool KillAllMonster(bool clear_mark_pos_first, float tra_map_rate, int remaining);
	//杀到打开目标
	static bool KillToOpenObj(const std::string& dst_name, bool remem_find = true);
	//杀到杀死目标
	static bool KillToKillTheObj(const std::string& dst_name);
	static bool KillToKillTheObj_(const GameObjBasePtrT& obj);
	//杀到指定地图
	static bool KillToTheMap(const std::string& dst_map_name, int diff_lvl, bool use_way, bool use_near_way);
	//杀到目标
	static bool KillToTheObj(const std::string& dst_name);
	//杀到杀死NPC怪
	static bool KillToKillNpc(const std::string& dst_name);
	//移动到目标
	static bool MoveToTheObj(const std::string& obj_name);
	static bool MoveToTheObj_(const GameObjBasePtrT& obj);
	//杀到房间
	static bool KillToTheRoom(const std::string& dst_name);
	//等待可移动
	static bool WaitRoleCanMove(pt_dword timeout);
	//移动到指定地图
	static bool SmartMoveMap(const std::string& fb_name, int diff);
	//使用传送卷轴到地图
	static bool UseWayReelToMap(const std::string& dst_map_name);
	//杀到指定位置
	static bool KillMoveTo(const std::string& name);
	static bool KillMoveTo_(const stCD_VecInt& dst_pos);
	//杀到改变状态
	static bool KillChangeState();
	//杀着跟随可移动柱子
	static bool KillFollowMovement();

	//对象相关
public:
	//打开目标
	static bool OpenObj(const std::string& item_name);
	static bool OpenObj_(const GameObjBasePtrT& obj);
	static GameObjBasePtrT FindNearestObj(enGameObjType obj_type);
	static GameObjBasePtrT FindNearestObj_(const std::string& obj_name);
	static GameObjBasePtrT FindNearestMonsterObj(const std::string& mon_name);
	//获得角色等级
	static DWORD GetRoleLvl();
	//对话Npc(npc面板上的内容为参数)
	static bool TalkToNpc(const std::string& talk_content);
	//购买物品及领取奖励
	static bool TakeNpcItem(const std::string& item_name);
	static bool TakeNpcItem_(int index);
	//打开附近对象
	static bool CheckNearestObjAndOpen(const std::string& obj_name);

	//ui相关
public:
	//关闭所有面板
	static bool CloseAllPanel(bool close_npc_panel);
	//打开EscapenMenu(按ESC键 弹出的对话框)
	static bool OpenEscapenMenu();
	//选择EscapenMenu
	static bool SelectEscapenMenu(int index);

	//物品相关
public:
	//自动镶嵌宝石
	static int AutoWearSomeGem();
	//自动穿戴装备
	static void AutoWearSomeEquips();
	//自动吃药
	static void AutoUseFlack(stCDR_OnHpMp::enValueType value_type);
	//自动使用水印药剂
	static void AutoUseUtilityFlask();
	//自动洗练
	static void AutoAgainRinseEquip();
	//查找背包物品
	static GItemBasePtr FindPakcetItem(const std::string& item_name);
	//自动存仓
	static bool AutoStoreBagItemToStore();
	//自动卖物品  
	static bool AutoSellBagItem(const std::string& npc_name);
	//使用物品
	static bool UseItem(const std::string& item_name);
	//使用物品到目标(背包内的使用)
	static bool UseItemToDst(const std::string& src_name, const std::string& dst_name);
	//物品数量
	static int GetItemCnt(const std::string& item_name);
	//根据类型获得物品数量
	static int GetItemCntByType(enCD_ItemType type);
	//检测任务
	static bool HasTheQuestByName(const std::string& quest_name);
	//购买物品
	static int BuyItemByNpc(const std::string npc_name, const std::string& item_name,int cnt);
	//是否在城镇
	static bool InTown();
	//拾取任务物品
	static bool PickUpQuestItem();
	//获得当前难度等级
	static int GetCurDiffLvl();
	//从仓库取地图(只供异界取地图使用，增加了地图过滤)
	static int TakeOutYiJieMapFromStorage(int cnt);
	//从仓库提取物品
	static int TakeOutItemByName(const std::string& item_name, int cnt);
	//检测提取套装
	static bool CheckStoreEmboitementToBag();
	//输出背包物品评分
	static void PrintBagItemWeight();
	//输出背包物品tips信息
	static void PrintBagItemTipsInfo();
	//添加物品获取方式
	static bool AddItemPathway(const std::string& item_name, const std::string& town_name, const std::string& npc_name);
	//保持购买物品
	static int KeepBuyItem(const std::string& item_name, int item_cnt);
	//自动升级仓库地图
	static bool UpLevelStashMap(const luabind::object& lua_obj);
	//设置自动穿戴剩余宝石等级
	static void SetAutoWearSurplusGem(pt_dword max_level);
	//检测仓库宝石套装到背包
	static bool CheckStoreGemTotalQualityToBag(pt_dword total_quality);
	//整理背包
	static bool AdjustPack();

	//技能
public:
	//设置全局危险距离
	static void SetGlobalDangerDistance(float screen_rate);
	//创建技能
	static CA_ActionPtr MakeSkill(const std::string& skill_name);
	//创建尸体技能
	static CA_ActionPtr MakeCorpseSkill(const std::string& skill_name);
	//添加技能
	static void AddSkill(GA_SkillBase& skill);
	//清空技能
	static void ClearSkills();
	//清空技能宝石升级信息
	static void ClearSkillGemLvlUpInfo();
	//添加技能宝石升级信息
	static void AddSkillGemLvlUpInfo(const std::string& skill_gem_name, int max_lvl);

	//其他
public:
	//当前地图传送点是否开启
	static bool IsOpenWayPointByMap(const std::string& map_name);
	//选择进入游戏
	static bool SelectEnterGame(const std::string& role_name);
	//开启异界装置
	static bool OpenMapDevice(const std::string& map_name);
	//遍历查找传送门(主要用于查找异界开启的传送门)
	static GameObjBasePtrT FindYiJieTransitonable();
	//获得当前地图
	static const std::string& GetCurMapName();
	//获得当前进行的异界地图
	static const std::string& GetCurYiJieMapName();
	//开启黄金制图仪
	static bool OpenHideoutMapDeviceByCarft(const std::string& craft_name);
	//自动登陆帐号
	static bool AutoLoginAccount();
	//等待召唤物死亡
	static bool WaitCalledDie(bool wait_called);
	//开启迷宫装置
	static bool OpenLabyrinthDevice(pt_int select_level);
	//获得未完成试炼
	static luabind::object GetUndoneLabyrinthMap();
	//获得当前章节
	static int GetCurChapter();

	//注册到lua中的私用函数
public:
	static void AddMapRoomRate(const std::string& map_name, int zoom_rate);
	static void UpdateCurMap();
	static void UpdateSelfArea();
	static void SetMapAreaInfo(const std::string& map_name, const luabind::object& areas);

public:
	inline static enWeaponCompositeCate GetWearWeaponType(){ return wear_weapon_type_; }
	inline static GFilterItemContPickUp& GetPickUpFilter() { return pick_up_filter_; }
	inline static GFilterItemContSmart& GetSaveStoreFilter(){ return save_Store_filter_; }
	inline static GFilterItemContSmart& GetSellItemFilter(){ return sell_item_filter_; }
	inline static GFilterItemContSmart& GetAgainRinseFilter(){ return again_Rinse_filter_; }
	inline static bool GetAutoWearEquip(){ return auto_wear_equip_; }
	inline static bool GetResetMap(){ return auto_reset_map_; }
	inline static pt_int GetOpenLabyrinthLvl(){ return open_labyrinth_Lvl_; }

private:
	static void AutoWearSomeEquipsImpl();
	static bool InStoreWhiteList(const GItemBase& item);
	static GItemBasePtr FindPacketItemByName(enCD_InventoryType type, const std::string& item_name);
	static bool SetItemColor(int item_color, enCD_ItemColor out_item_color);
	static bool AutoIdentifiedItem(bool all_indentified = false);
	static bool UseFlack(int index);
	static bool AutoWearLinkGem();
	static bool CheckHidePosMap(const std::string& dst_map, enCD_kMapDifficulty diff_lvl);

	//test
public:
	static int TestAutoLvlUpPassiveSkills(int type);
	static void TestCreateSelectRole(const std::string& area_name, int profession_type, const std::string& role_name);
	//先移动到目标，然后判断目标位置的模型是否是唯一地形，并输出。
	static void TestAutoDetectTerrainObjs(const luabind::object& obj_names);
	//根据当前角色的位置，输出以radius为半径的附近地形信息，要求该地形在整个地图中出现的次数最多不超过max_cnt次。即max_cnt==1时，便表示输出唯一地形。
	static void TestDetectUniqueTerrain(int radius, int max_cnt);
	//根据地形区域来定位一个地形，地形区域的内容包括上一个地形与下一个地形之间的距离
	//地形区域中定位到的第一个地形即为要定位的地形
	//terrain_cnt最好为2，为3的话，可能会崩掉
	//当前角色所在的地形属于地形区域中的第一个地形，也是最终想要定位或捕获的地形
	//该函数输出上个地形与下个地形的距离等信息，主要由距离等信息定位地形区域。
	//terrain_cnt==2表示会输出两个地形信息
	static void TestDetectOneTerrainArea(int radius, int terrain_cnt);
	//根据一系列坐标点来定位一个地形区域
	static void TestDetectUniqueTerrainByPos(const luabind::object& positions);
	//遍历当前地图中所有指定的地形
	static void TestTerrainSameInfo(const std::string& terrain_path);
	static void TestLoginAccount(const std::string& ac, const std::string& pwd, const std::string& gateway);
	static void TestActorUseSkill(const std::string& skill_name, const stCD_VecInt& dst_pos);
	static void TestTerminateCurSkill();
	static void TestGetInGameMainUiDlgCnt(int flag);

	//debug
public:
	static void DbgObjs(bool is_alive, const luabind::object& tbl);
	static void DbgSortedObjs(bool is_alive, const luabind::object& tbl);
	static void DbgPakcageItems(int package_type);
	static void DbgGSP_Resolver(bool is_vertex);
	static void DbgTestPrint(int value);
	
private:
	static GFilterItemContSmart							sell_item_filter_;
	static GFilterItemContSmart							save_Store_filter_;
	static GFilterItemContPickUp						pick_up_filter_;
	static GFilterItemCont								yi_jie_map_filter_;
	static GFilterItemContSmart							again_Rinse_filter_;

	static GAutoEatHpMp									auto_eat_hp_mp_;
	//static GAutoUpDateGem								auto_update_gem_;
	static enWeaponCompositeCate						wear_weapon_type_;
	static std::string									cur_yi_jie_map_;
	static pt_int										open_labyrinth_Lvl_;
	static bool											auto_reset_map_;
	static bool											auto_wear_equip_;
};
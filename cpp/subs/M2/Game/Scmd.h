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
	typedef std::map<std::string, en_NpcMonster> NpcMonsterT;

protected:
	void BindGlobalFn() override;
	void BindClass() override;
	void BindScriptCommand(luabind::module_& mod) override;
	void BindConst() override;
	void BindTest() override;

	//配置
public:
	static void EnableAutoOpenChest(bool enable);
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

	//物品配置
public:
	//创建组合过滤
	static GFilterItemPtr CreateGroupFilter(const luabind::object& tbl);
	//创建过滤对象
	static GFilterItemPtr CreateFilterObj(en_ItemFilterType filter_type, const luabind::object& tbl);
	//添加拾取过滤
	static void AddPickUpFilter(GFilterItemPtr filter_ptr);
	//添加存仓过滤		
	static void AddStoreFilter(GFilterItemPtr filter_ptr);
	//添加出售过滤
	static void AddSellFilter(GFilterItemPtr filter_ptrl);
	//设置自动使用药剂
	static void SetAutoUseFlask(float hp_rate, float mp_rate);
	//设置背包格子空余数量
	static void SetMainBagMinFree(int wight, int hight);
	//设置宝石连接
	static bool SetGemLink(const std::string& main_gem, const luabind::object& tbl);
	//设置穿戴武器类型
	static void SetWearWeaponType(enWeaponCompositeCate type);
	//清理所有宝石连接
	static void ClearAllGemLink();
	//设置评分
	static void SetItemScore(en_GradeValue score_type, const luabind::object& tbl);
	//设置药品数量
	static void SetBagFlaskCnt(const luabind::object& tbl);
	//设置异界过滤
	static void SetYiJieFilter(en_ItemFilterType filter_type, const luabind::object& tbl);
	//设置套装数量
	static void SetEmboitementCnt(int cnt);
	//清空物品属性解析
	static void ClearItemAttrParser();
	//物品属性解析
	static CSV_NodePtr CreateItemAttrParser(const luabind::object& fn);

	//核心
public:
	//全图杀怪
	static bool KillAllMonster(bool clear_mark_pos_first);
	//杀到打开目标
	static bool KillToOpenObj(const std::string& dst_name);
	//杀到杀死目标
	static bool KillToKillTheObj(const std::string& dst_name);
	static bool KillToKillTheObj_(const GameObjBasePtrT& obj);
	//杀到指定地图
	static bool KillToTheMap(const std::string& dst_map_name, int diff_lvl, bool use_way);
	//杀到目标
	static bool KillToTheObj(const std::string& dst_name);
	//杀到杀死NPC怪
	static bool KillToKillNpc(const std::string& dst_name);
	//移动到目标
	static bool MoveToTheObj(const std::string& obj_name);
	static bool MoveToTheObj_(const GameObjBasePtrT& obj);
	//杀到房间
	static bool KillToTheRoom(const std::string& dst_name);
	//等待角色可移动
	static bool WaitRoleCanMove(pt_dword timeout);
	//移动到指定的副本（名字，难度等级）
	static bool SmartMoveMap(const std::string& fb_name, int diff);
	//传送回城
	static bool UseWayToTown();

	//对象相关
public:
	//打开对象
	static bool OpenObj(const std::string& item_name);
	static bool OpenObj_(const GameObjBasePtrT& obj);
	static GameObjBasePtrT FindNearestObj(GameObjType obj_type);
	static GameObjBasePtrT FindNearestObj_(const std::string& obj_name);
	static GameObjBasePtrT FindNearestMonsterObj(const std::string& mon_name);
	//获得角色等级
	static DWORD GetRoleLvl();
	//对话Npc(npc面板上的内容为参数)
	static bool TalkToNpc(const std::string& talk_content);
	//购买物品及领取奖励
	static bool TakeNpcItem(const std::string& item_name);
	static bool TakeNpcItem_(int index);

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
	//查找背包物品
	static GItemBasePtr FindPakcetItem(const std::string& item_name);
	//存放一些物品到仓库
	static bool AutoStoreBagItemToStore();
	//自动卖物品   //在黑名单的直接卖掉！在白名单和大于等于其他类型值的不卖
	static bool AutoSellBagItem();
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
	static bool BuyItemByNpc(const std::string npc_name, const std::string& item_name);
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

	//其他
public:
	//当前地图传送点是否开启
	static bool IsOpenWayPointByMap(const std::string& map_name);
	//选择进入游戏
	static bool SelectEnterGame(const std::string& role_name);
	//开启异界装置
	static bool OpenMapDevice();
	//遍历查找传送门(主要用于查找异界开启的传送门)
	static GameObjBasePtrT FindYiJieTransitonable();
	//获得当前地图
	static const std::string& GetCurMapName();

	//注册到lua中的私用函数
public:
	static void AddMapRoomRate(const std::string& map_name, int zoom_rate);
	static void UpdateCurMap();
	static void SetMapAreaInfo(const std::string& map_name, const luabind::object& areas);
	//static StrCont& GetAllNpcMon();

public:
	inline static enWeaponCompositeCate GetWearWeaponType(){ return wear_weapon_type_; }
	inline static GFilterItemContPickUp& GetPickUpFilter() { return pick_up_filter_; }
	inline static GFilterItemContSmart& GetSaveStoreFilter(){ return save_Store_filter_; }
	inline static GFilterItemContSmart& GetSellItemFilter(){ return sell_item_filter_; }
	inline static bool GetAutoWearEquip(){ return auto_wear_equip_; }

private:
	static void AutoWearSomeEquipsImpl();
	static enCD_InventoryType IndexToPackType(int index);
	static bool InStoreWhiteList(const GItemBase& item);
	static GItemBasePtr FindPacketItemByName(enCD_InventoryType type, const std::string& item_name);
	static bool SetItemColor(int item_color, enCD_ItemColor out_item_color);
	static bool AutoIdentifiedItem();
	static bool UseFlack(int index);
	static bool AutoWearLinkGem();

	//test
public:
	static int TestAutoLvlUpPassiveSkills(int type);
	static void TestCreateSelectRole(const std::string& area_name, int profession_type, const std::string& role_name);

	//debug
public:
	static void DbgObjs(bool is_alive, const luabind::object& tbl);
	static void DbgSortedObjs(bool is_alive, const luabind::object& tbl);
	static void DbgPakcageItems(int package_type);
	static void DbgGSP_Resolver(bool is_vertex);

private:
	static GFilterItemContSmart							sell_item_filter_;
	static GFilterItemContSmart							save_Store_filter_;
	static GFilterItemContPickUp						pick_up_filter_;
	static GFilterItemCont								yi_jie_map_filter_;
	
	static GAutoEatHpMp									auto_eat_hp_mp_;
	static GAutoUpDateGem								auto_update_gem_;
	static enWeaponCompositeCate						wear_weapon_type_;
	static bool											auto_wear_equip_;
};
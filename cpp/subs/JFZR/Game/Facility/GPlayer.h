#pragma once
/*
@author zhp
@date 2014-3-10 14:59
@purpose for game player info
*/
#include "GameObj.h"
#include <Common/UsefulClass.h>
#include "GSkill.h"
#include "GameObjMgr.h"
#include "GClass.h"
#include "GPacket.h"
#include "GRoom.h"
#include "GTask.h"
#include "GStrMap.h"
#include <luabind/object.hpp>

class GPlayer : public GameObjBase{
public:
	GPlayer();
	 
	//type definition
public:

	//interfaces
public:
	//人物等级
	int GetLevel() const;

	//override
public:
	bool Update();

protected:
	GType::stGameVector			direction_;
	int							player_lvl_;
};

class GPlayerSelf : public GPlayer, public Singleton<GPlayerSelf, Singleton_MakeMe>{
public:
	typedef std::shared_ptr<CMDR_OnMailList> MailListPtrT;
	typedef std::shared_ptr<CMDR_OnMyProductList> MyProductListPtrT;
	typedef std::shared_ptr<CMDR_OnCreateNpcByClient> OnCreateNpcByClientT;
	typedef std::queue<std::string> TradeEventQueueT;
	typedef std::shared_ptr<CMDR_OnRequestProductInfo> CurRequestProductInfoT;
	typedef ProduceToDone<CMDR_OnRequestProductList::stProductItem> BuyProductItemsT;
	typedef std::shared_ptr<CMDR_OnRequestProductList> CurRequestProductListT;
	struct stSendMailList{
		GItem			item_;
		std::wstring	receive_role_name_;
	};
	typedef std::vector<stSendMailList> SendMailListContT;

public:
	GPlayerSelf();
	//得到疲劳值
	int GetFatigueData() const;
	GPacket& GetPacket();
	TaskReceivedMgr& GetReceivedTask();
	//得到详细的职业
	GType::enJobCategory GetJob() const;
	const std::wstring GetSpecifyJobName() const;
	//主职业，比如剑斗士、刃武者、控魔师
	GType::enJobCategory GetMainJob() const;
	const std::wstring& GetChangeMainJobNpcName() const;
	static GType::enJobCategory JobToMainJob( GType::enJobCategory job );
	const GClassInfoTemplate* GetClassInfoTemplate() const;
	const std::wstring& GetCurVillageName() const;

	//技能相关
public:
	//使用技能
	bool HitTargets( const GSkillObj& skill_obj, const GameObjMgr::GameObjIterPtr& target_gameobjs );
	bool HitTargetsOneByOne( const GSkillObj& skill_obj, const GameObjMgr::GameObjIterPtr& target_gameobjs );
	//使用技能攻击一个
	bool HitTarget( const GSkillObj& skill_obj, const GameObjBase::SelfPtrT& target_gameobj );
	//只攻击一个
	bool HitToDie( const GSkillObj& skill_obj, const GameObjBase::SelfPtrT& target_gameobj );
	//攻击多个
	bool HitMultiToDie( const GSkillObj& skill_obj, const GameObjMgr::GameObjIterPtr& target_gameobjs );
	GSkillMgr& GetSkillMgr();
	//学习技能，升一级
	bool LearnSkill( const GSkillObj& skill_obj, int lvl_addition );
	//自动升级技能
	bool AutoLvlUpSkills();
	//自动学习和升级所有技能
	bool AutoLearnLvlUpSkills();
	//学习所有未学过的技能
	bool LearnAllSkills();
	//学习指定技能
	bool LearnTheSkill( const std::string& skill_ch_name );
	bool LearnTheSkillW( const std::wstring& skill_id );
	//学习某个分支中未学过的技能
	bool LearnTheSkillBranch( const GClassInfoTemplate::SkillBranch& skill_branch );
	//发送默认连击数
	bool SendSkillActivityCntAsDlt() const;

	//物品相关
public:
	//拾取物品
	bool PickUpItem( const GameObjBase::SelfPtrT& drop_item );
	bool PickUpItems( GameObjMgr::GameObjIterPtr& drop_items );
	//站立到并拾取物品
	bool StandAndPickUpItem( const GameObjBase::SelfPtrT& drop_item );
	bool StandAndPickUpItems( GameObjMgr::GameObjIterPtr& drop_items );
	//修理所有已穿装备
	bool RepairAllWearEquip();
	//穿戴装备
	bool EquipItemSync( const GItem& item );
	//脱掉装备
	bool UnEquipItemSync( const GItem& item );
	//卖掉普通栏的所有装备
	bool SellNormalTabAllEquips();
	//卖掉指定的物品
	bool SellTheItemToShop( GItem& item );
	//分解物品
	bool DisassemblyTheItem( GItem& item );

	//移动相关
public:
	//站立到某个坐标位置
	bool FbStandToPos( const GType::stGameVector& pos );
	bool VillageStandToPos( const GType::stGameVector& pos );
	//站立到某个游戏对象
	bool FbStandToGameObj( const GameObjBase::SelfPtrT& gameobj );
	bool VillageStandToGameObj( const GameObjBase::SelfPtrT& gameobj );
	//站立到某个游戏对象身边
	bool FbStandToGameObjSide( const GameObjBase::SelfPtrT& gameobj );
	//副本中怪物的移动
	bool FbMonsterMove( const GameObjBase::SelfPtrT& gameobj );
	bool FbMonstersMove( const GameObjMgr::GameObjIterPtr& gameobjs );
	//修改角色本地坐标
	bool ChangeRoleCurPos( const GType::stGameVector& new_pos );
	//本地移动到各个怪物中
	bool TouchConditionThatLocalMoveToMonsters();
	void LocalMoveToMonsters();

	//跨越地图
public:
	//进入副本
	bool EnterCloneMap( const GClass::AllowedVillageCloneMap& clone_map, GType::enFbDifficulty difficulty );
	//默认等级
	bool EnterCloneMapDefault( const GClass::AllowedVillageCloneMap& clone_map );
	//进入另一个房间
	bool EnterAnotherRoom( const GRoomDoor& door );
	//返回城镇
	bool ReturnCity();
	//再次挑战副本
	bool RestartCloneMap();
	//改变城镇
	bool ChangeVillage( const std::wstring& village_gate_id );
	//移动到指定的城镇
	bool MoveToTheVillage( const std::string& village_ch_name );
	bool MoveToTheVillageW( const std::wstring& village_dst_id );
	//移动到指定的副本
	bool MoveToTheCloneMap( const std::string& clone_map_ch_name );
	bool MoveToTheCloneMapW( const std::wstring& clone_map_id );
	bool MoveToTheStage( const std::wstring& stage_id );
	const std::wstring* Fb2Village( const std::wstring& clone_map_id );
	//bool MoveToTheCloneMapW( const std::wstring& clone_map_id );
	//移动到指定任务所在的副本
	//bool MoveToCloneMapOfTheTask( const std::string& quest_ch_name );

	//交易相关
public:
	//发送邮件,21级以上才可以发邮件
	bool SendMail( const std::wstring& receive_role_name, const std::wstring& mail_title, const std::wstring& mail_content, DWORD gold, const GItem* item );
	bool SendMail( const std::wstring& receive_role_name, const std::wstring& mail_title, const std::wstring& mail_content, DWORD gold, GItemMgr::GameItemIterPtr& item_iter );
	bool SendMailItem( const std::wstring& receive_role_name, const GItem& item );
	bool SendMailItemDelay( const std::wstring& receive_role_name, const GItem& item );
	bool SendMailGold( const std::wstring& receive_role_name, int reverse_gold, int delta_gold );
	bool SendMailImpl( const std::wstring& receive_role_name, int gold, const GItem* item );
	void ClearSendMailList();
	void DoSendMailList();
	void ResetMailList( const MailListPtrT& mail_list );
	void ResetMyProductList( const MyProductListPtrT& product_list );
	const MailListPtrT& GetMailList() const;
	const MyProductListPtrT& GetMyProductList() const;
	const CMDR_OnMyProductList::stMyProduct* FindMyProductByItemId( const std::wstring& item_id ) const;
	int GetTheProductCnt( const std::wstring& item_id ) const;
	/*warning*/int CalcSellingProductCnt( const std::wstring& item_id, int item_quality, int item_level ) const;
	void SucceedToGetAttachments( LONGLONG mail_serial );
	bool ReceiveOneMailAttachments();
	bool RemoveOneRubbishMail();
	//刷新我的拍卖列表
	bool RefreshMyProductList();
	//拍卖物品
	bool SaleProductAsPrice( GItem& item, double price, int item_cnt, int product_cnt, int ac_type );
	bool SaleProductAsPriceBase( GItem& item, int price_delta, double dlt_price, int item_cnt, int product_cnt, int ac_type );
	//购买拍卖物品
	bool BuyOneOtherProduct();
	TradeEventQueueT& GetTradeEventQueue();
	void PushTradeEvent( const std::string& event_name );
	void ClearTradeEventQueue();
	void OnRegisterProductResult( const CMDR_OnRegisterProduct& on_result );
	bool RichToSaleAsPrice( double total_price ) const;
	void ResetCurRequestProductInfo( const CurRequestProductInfoT& cur_info );
	//获取某种号的私有拍卖信息
	bool GainPriProductInfoByAcType( int ac_type );
	bool GainProductInfoByAcType( int product_ac_type, int ac_type );
	//告诉自己的私有拍卖信息给某种号
	bool TellSelfPriProductInfoToAcType( int ac_type );
	bool TellSelfProductInfoToAcType( int product_ac_type, int ac_type );
	bool GenPriProductsInfo( Json::Value& out_json ) const;
	bool GenProductsInfoByAcType( Json::Value& out_json, enGameAcType ac_type ) const;
	GType::RoleProductInfoContT& GetOthersProductInfo();
	void AddOtherProductInfo( const CmnUuid& role_uuid, const GType::stProductInfo& other_product );
	void EraseOtherProductInfo( const CmnUuid& role_uuid, const GType::stProductInfo& other_product );
	//查询拍卖列表
	bool RequestProductList( const GItemTemplate& item_template );
	//清空扫拍卖条目
	void ClearBuyProductItems();
	//添加扫拍卖条目
	bool AddBuyProductItem( const std::string& item_name, int min_price, int item_cnt, int item_cnt_limit );
	void ResetCurRequestProductList( const CurRequestProductListT& cur_product_list );
	//进行扫拍卖
	void DoBuyProductItem();

	//任务相关
public:
	//接受任务
	bool AcceptTask( const std::wstring& quest_id );
	bool SendAcceptTask( const std::wstring& quest_id );
	//完成任务
	bool CompleteTask( const std::wstring& quest_id, const std::wstring& reward_item_id );
	//提交任务，默认选择奖励
	bool CommitTask( const std::wstring& quest_id );
	//选择默认奖励
	const std::wstring& ChooseTaskDftReward( const std::wstring& quest_id );
	const std::wstring& ChooseTaskDftReward( const TaskCurJobTemplate& task_template );
	const TaskCurJobTemplate* GetTaskTemplate( const std::wstring& quest_id );
	//自动完成所有已完成任务
	bool CommitCompleteTasks();
	//自动提交没有完成处理的所有任务
	bool CommitNoCompleteHandlerTasks();
	//接指定的任务
	bool AcceptTheTask( const std::string& quest_ch_name );
	bool AcceptTheTaskW( const std::wstring& quest_id );
	//是否已接指定的任务
	bool IsAcceptedTheTask( const std::string& quest_ch_name );
	//任务是否已完成
	bool TheTaskCompletable( const std::string& quest_ch_name );
	GameEn2ChNodeMgr& GetEn2Ch();
	//接受任意一个任务
	bool AcceptLocalMapAnyTask();
	//是否能接受该任务
	bool CanAcceptTheTask( const std::wstring& quest_id );
	//接受任意所有任务(本地图中)
	int AcceptLocalMapAnyAllTasks();
	bool PreDoTask();
	//自动做一个已接任务
	bool AutoDoneOneAcceptedTask();
	//自动完成一个已完成任务
	bool AutoCompleteOneCompletedTask();
	//自动去已完成任务的城镇去提交任务
	bool AutoGotoCommitCompletedTask( bool village_move );
	//寻找一个存在于某个城镇的任务
	static bool FindQuestIdOfVillage( const TaskReceivedMgr::AcceptedTaskIterPtr& task_iter, const std::wstring& village_id, std::wstring& out_stage_id, std::wstring& out_quest_id );
	void ResetTaskBlacklist();
	void AddTaskBlacklist( const std::wstring& quest_id );
	void AddProhibitTask( const std::wstring& quest_ch_name );
	bool TaskBlacklistExistTheQuest( const std::wstring& quest_id );
	//清空建议城镇
	void ClearRecommandVillages();
	//生成建议城镇
	void GenRecommandVillages();
	//执行一个建议城镇
	bool DoOneRecommandVillage();

	//for lua
public:
	//杀死所有的怪物
	bool KillAllMonster();
	//破坏所有固态物品
	bool DestroyAllMisc();
	//拾取所有物品
	bool PickUpAllItems();
	//拾取所有金钱
	bool PickUpAllGold();
	//打完所有房间
	bool FightThroughAllRooms( const luabind::object& func );
	bool FightThroughAllRooms_( const std::function<void()>& func );
	//全图打副本
	bool FightAllGraph( const luabind::object& func );
	bool FightAllGraph_( const std::function<void()>& func );
	bool DoTheFb( const std::wstring& fb_id );
	bool DoTheFbA( const std::string& fb_ch_name );
	void DoWhenAtRoom();
	//打怪直到开门
	void HitUntilClearCurSector( const std::wstring& cur_sector );

	//other
public:
	//BOSS房间选择奖励
	bool FbBossRoomReward();
	//转职
	bool ChangeJob( int sub_job_idx );
	//默认转职
	bool ChangeJobAsDlt();
	bool NeedChangeJob() const;
	CmnUuid GetMyRoleUuid() const;
	void ResetOnCreateNpcByClient( const OnCreateNpcByClientT& the_msg );
	//角色死亡了
	void PlayerDied();
	//重置死亡不回城副本
	void ResetNoRetCityAtDie();
	//添加死亡不回城副本
	void AddNoRetCityAtDie( const std::wstring& fb_ch_name );

	//test
public:
	void Test();
	void Test1();
	//测试所有可以进去的副本或地图信息
	void TestAllowedV_CM();
	void TestEnterCloneMap();

	//static
public:
	
	//override
public:
	bool Update();
	bool UpdateFatigueData();

private:
	//学完某个技能的SP
	bool UseOutSkillSp( GSkillObj& skill_obj );

private:
	bool TryUpdateImpl();
	bool DoUpdateImpl();

private:
	GSkillMgr				skill_mgr_;
	int						fatigue_data_;			//疲劳值
	GPacket					packet_;				//包裹
	std::wstring			village_name_;			//当前所在城镇名称
	TaskReceivedMgr			received_tasks_;		//已接任务
	int						left_sp_;				//剩余技能点
	std::wstring			channel_id_;
	GType::enJobCategory	job_;
	std::wstring			role_class_type_;
	int						gold_;
	GClass::TimerSyncExec	timer_sync_exec_;
	const GClassInfoTemplate*	job_branch_skill_template_;
	MailListPtrT			mail_list_;
	MyProductListPtrT		my_product_list_;
	TradeEventQueueT		trade_event_queue_;
	CurRequestProductInfoT	cur_request_product_info_;
	GType::RoleProductInfoContT	others_product_info_;
	std::set<std::wstring>	task_blacklist_;
	std::set<std::wstring>	task_prohibited_ch_name_;
	int						buy_other_min_gold_;
	BuyProductItemsT		buy_product_items_;		//扫拍卖
	CurRequestProductListT	cur_product_list_;
	std::queue<std::wstring>	recommand_villages_;
	OnCreateNpcByClientT		on_create_npc_by_client_;
	SendMailListContT			send_mail_list_;
	bool						player_died_;
	std::set<std::wstring>		no_ret_city_at_die_;			//死亡不回城副本
};
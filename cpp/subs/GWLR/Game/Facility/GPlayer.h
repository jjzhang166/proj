#pragma once
/*
@author zhp
@date 2014-3-10 14:59
@purpose for game player info
*/
#include "GTypes.h"
#include "GameObj.h"
#include "GVillage.h"
#include "GItem.h"
#include "GTask.h"
#include "GCloneMap.h"
#include "../GloFuncs.h"
#include "GProtocolR.h"
#include "GMapPath.h"

class GPlayer : public GameObjBase{
public:
	GPlayer();
};

typedef std::pair<GType::stGameVector3, GType::stGameVector3> GameObjDirT;
class GPlayerMe : public GPlayer, public Singleton<GPlayerMe, Singleton_MakeMe>{
	friend class GClass::GHitMonsterMode_Mouse;
	friend class GClass::GHitMonsterMode_UsePacketItem;
public:
	typedef GSP_StagePosPath::StepsT PosQueueT;
	typedef std::shared_ptr<CMDR_OnEnterFb> MsgOnEnterFbT;
	typedef std::shared_ptr<CMDR_OnEnterFbError> MsgOnEnterFbErrT;
	typedef std::shared_ptr<CMDR_OnFbInfo> MsgOnFbInfoT;
	typedef boost::optional<MsgOnFbInfoT> MsgOnFbInfoOptionalT;
	typedef std::shared_ptr<CMDR_OnRefreshMySellList> MsgMySellListT;
	typedef std::shared_ptr<CMDR_OnGainAuctionPageInfo> MsgAuctionPageInfoT;
	typedef std::vector<MsgAuctionPageInfoT> AuctionPagesInfoT;
	typedef std::shared_ptr<CMDR_OnSafeTime> MsgOnLeftTimeT;
	typedef std::shared_ptr<CMDR_OnBuyAuction> MsgOnBuyAuctionT;
	typedef std::list<stAuctionItemInfo> AuctionItemsContT;
	typedef std::shared_ptr<CMDR_OnAcceptMail> MsgOnAcceptMailT;
	typedef std::shared_ptr<CMDR_OnOpenNpc> MsgOnOpenNpcT;
	typedef std::shared_ptr<CMDR_OnSoldAuction> MsgOnSoldAuctionT;
	typedef std::shared_ptr<CMDR_OnSoldItemToShop> MsgOnSoldToShopT;
	typedef std::shared_ptr<CMDR_OnCommitTaskFailed> MsgCommitTaskFailedT;
	typedef std::shared_ptr<CMDR_OnDoNpcChat> MsgOnDoNpcChatT;
	typedef std::shared_ptr<CMDR_OnSendMail> MsgOnSendMailT;
	typedef std::shared_ptr<CMDR_OnSendMail2> MsgOnSendMail2T;
	typedef std::shared_ptr<CMDR_OnUseItemRet1> MsgOnUseItemRet1T;
	typedef std::shared_ptr<CMDR_OnUseItemRet2> MsgOnUseItemRet2T;
	typedef std::shared_ptr<CMDR_OnBuyItem> MsgOnBuyItemT;
	typedef std::shared_ptr<CMDR_OnCollectRet> MsgOnCollectRetT;
	typedef std::shared_ptr<CMDR_OnArrangeItemsRet> MsgOnArrangeItemsRetT;
	typedef std::shared_ptr<CMDR_OnSendMoveRoomRet> MsgOnSendMoveRoomRetT;
	typedef std::shared_ptr<CMDR_OnAcceptTaskRet> MsgOnAcceptTaskRetT;
	typedef std::shared_ptr<CMDR_OnGiveupTaskRet> MsgOnGiveupTaskRetT;
	typedef std::shared_ptr<CMDR_OnRequestDeal>	MsgOnRequestDealT;
	typedef std::shared_ptr<CMDR_OnRequestDealFail> MsgOnRequestDealFailT;
	typedef std::shared_ptr<CMDR_OnRecvQuestDeal> MsgOnRecvQuestDealT;
	typedef std::shared_ptr<CMDR_OnCancelDeal> MsgOnCancelDealT;
	typedef std::shared_ptr<CMDR_OnDealFail> MsgOnDealFailT;
	typedef std::shared_ptr<CMDR_OnMoveItemToDealTab> MsgOnMoveItemToDealTabT;
	typedef std::shared_ptr<CMDR_OnMoveDealGold> MsgOnMoveDealGoldT;
	typedef std::shared_ptr<CMDR_OnRecvDealStatus> MsgOnRecvDealStatusT;
	typedef std::shared_ptr<CMDR_OnRecvDealEnd> MsgOnRecvDealEndT;
	typedef std::shared_ptr<CMDR_OnGetMailItemsInfo> MsgOnGetMailItemsInfoT;
	typedef NameList<std::string> NameList_StrT;
	typedef std::pair<std::string, std::string> WearWhiteListPairT;
	typedef NameList<WearWhiteListPairT, std::vector<WearWhiteListPairT> > WearWhiteListT;
	struct stShangJinNameListItem{
		std::string				task_sub_name_;
		CD_En_FbDifficulty		fb_diff_;
		bool					is_king_fb_;

		friend bool operator == ( const stShangJinNameListItem& lhs, const stShangJinNameListItem& rhs ){
			return lhs.is_king_fb_ == rhs.is_king_fb_ && lhs.fb_diff_ == rhs.fb_diff_ && lhs.task_sub_name_ == rhs.task_sub_name_;
		}
	};
	typedef NameList<stShangJinNameListItem, std::vector<stShangJinNameListItem> > ShangJinWhiteListT;
	typedef NameList<int> NameList_IntT;
	struct stShangJinTaskInfo{
		const GTaskDb*		task_template_;
		GMapDb				fb_info_;
		bool				test_fb_diff_lvl_;
		stShangJinTaskInfo();
	};
	typedef std::vector<stShangJinTaskInfo> ShanJinTaskInfoT;
	struct stSendMailItem : GClass::stItemInfo{
		bool	reach_cnt_;		//达到指定数量才会邮寄
	};
	typedef std::vector<stSendMailItem> SendMailContT;
	struct stLastSendMoveRoomInfo{
		std::string		cmd_;
		int				dst_room_num_;
	};
	struct stNeedMaterialInfo{
		int			material_item_id_;		//需要的材料ID
		int			total_item_cnt_;		//需要的材料总数
		int			need_cnt_;				//需要的材料数
	};
	enum enSendMail{
		kSM_Error = -2,
		kSM_Again = -1,
		kSM_Empty = 0,
		kSM_Succeed = 1,
	};

public:
	GPlayerMe();
	void ResetAllUpdateData();
	void ResetPassRoomData();
	void OnRoleDead();

public:
	int GetHitHp() const;
	void SetHitHp( DWORD package_id, int hit_hp );
	int GetFeebHp() const;
	void SetFeebHp( DWORD package_id, int feeb_hp );
	int GetCurHp() const;
	const std::string& GetCurMapName() const;
	int GetCurTemplateMapId() const;
	int GetCurMapId() const;
	const std::string& GetCurTemplateMapName() const;
	void ResetNofityDatas();
	void OnCrossMap();
	//每次角色进入游戏都要执行
	void ExecAtEveryRoleEnterGame();
	GReceivedTaskMgr& GetReceivedTaskMgr();
	GAcceptableTasks& GetAcceptableTasks();
	GEnterableFbMgr& GetEnterableFbMgr();
	GItemMgr_Package& GetGridPackage();
	GItemMgr_Store& GetStorePackage();
	GItemMgr_SuCai& GetGridSuCai();
	GItemMgr_Equip& GetEquipPackage();
	GItemMgr_Task& GetGridsTask();
	GItemMgrBase* GetItemMgr(CD_En_TabCategory tab_category);
	GClass::CurFarmFieldInfo& GetCaoYaoTianInfo();
	const MsgOnEnterFbT& GetMsg_OnEnterFb() const;
	int GetRoleLvl() const;
	int GetLeftShouLieQuanCnt() const;
	int GetCurGold() const;
	int GetCurSilver() const;
	void SetOnEnterFb( const MsgOnEnterFbT& msg );
	void SetOnFbInfo( const MsgOnFbInfoOptionalT& fb_info );
	void SetOnRefreshMySellList( const MsgMySellListT& msg );
	void SetOnGainAuctionPageInfo( const MsgAuctionPageInfoT& msg );
	void SetOnLeftTime( const MsgOnLeftTimeT& msg );
	void SetOnBuyAuction( const MsgOnBuyAuctionT& msg );
	void SetOnAcceptMail( const MsgOnAcceptMailT& msg );
	void SetOnOpenNpc( const MsgOnOpenNpcT& msg );
	void SetOnSoldAuction( const MsgOnSoldAuctionT& msg );
	void SetOnSoldToShop( const MsgOnSoldToShopT& msg );
	void SetCommitTaskFailed( const MsgCommitTaskFailedT& msg );
	void SetOnDoNpcChat( const MsgOnDoNpcChatT& msg );
	void SetOnSendMail( const MsgOnSendMailT& msg );
	void SetOnSendMail2( const MsgOnSendMail2T& msg );
	void SetOnUseItemRet1( const MsgOnUseItemRet1T& msg );
	void SetOnUseItemRet2( const MsgOnUseItemRet2T& msg );
	void OnBuyItem( const MsgOnBuyItemT& msg );
	void OnCollectRet( const MsgOnCollectRetT& msg );
	MsgOnCollectRetT& GetCollectRet();
	void OnArrangeItemsRet( const MsgOnArrangeItemsRetT& msg );
	bool SetLianQiCao();
	void SetRightBtHit( bool is_right_hit );
	void SetMonsterMaxHigh( float high );
	void ResetSendRoomTime();
	int GetLeftSingleExp() const;
	int GetLeftDoubleExp() const;
	void HitRaii();

	//移动/走路相关
public:
	//发送走路封包
	bool SendWalk( const GType::stGameVector3& dst_pos, const GType::stDirection2d& dir );
	bool DoWalk( const GType::stGameVector3& dst_pos );
	bool DeltaWalk( const GType::stGameVector3& delta_pos );
	bool SetPosInfoAsAnotherObj( const GameObjBase& obj );
	//设置人物朝向
	bool SetRoleForward( const GameObjBase& obj );
	bool SetRoleForwardByPos( const GType::stGameVector3& pos_src, const GType::stGameVector3& pos_dst );
	//移动
	bool MoveForward( int steps = 1 );
	//发送坐标
	bool SendToSerThePos( const GType::stGameVector3& the_pos, bool send_force );
	//设置坐标到游戏对象背后
	bool SetRoleToBackDirect( const GameObjBase& obj, int steps = 1 );
	bool TestTheMonsterPosZ( const GType::stGameVector3& the_pos );
	bool IsTestTheMonsterPosZ( GameObjBase& obj );
	OptinalBool SetRoleToBackIndirect( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int obj_room_num, int role_room_num, bool is_any_direction, int steps );
	bool SetPosEnsureSync( const GType::stGameVector3& pos );
	typedef std::function<bool()> SetRolePosCallBackT;
	OptinalBool SetRoleToPosIndirect( const GType::stGameVector3& dst_pos, int obj_room_num, int role_room_num, 
		int target_room_num, const SetRolePosCallBackT* call_back, bool is_test_dst_pos_z, bool is_pass_room = false );
	bool TestObjPosInfoAtFindingPath( DWORD obj_packet_id, GNormalGameObjMgrBase* obj_mgr, int obj_room_num, const GType::stGameVector3& pos_tmp );
	bool TestObjBodyPartPosInfoAtFindingPath( DWORD obj_packet_id, GNormalGameObjMgrBase* obj_mgr, int obj_room_num, const GType::stGameVector3& pos_tmp );
	//吸怪
	bool AttrackGameObj( GameObjBase& obj );
	//过城镇门
	bool StepVillageDoor( const stCrossDoorPosInfo& door_pos_info );
	//移动到指定的村庄
	OptinalBool MoveToTheVillage( const std::string& dst_village_name, int room_num );
	bool SendMoveVillage( const std::string& gate_name );
	//移动到指定的副本
	OptinalBool MoveToTheFb( const std::string& fb_name );
	OptinalBool MoveToTheFbById( int fb_id );
	OptinalBool MoveToTheFbImpl( const GMapDb& fb_info );
	OptinalBool SendMoveToTheFb( const GMapDb& fb_info, GType::enFbConsumeMode fb_mode );
	bool DoSendMoveToFbErr();
	//放弃关卡
	bool GiveUpFb();
	bool SendGiveUpFb();
	//退出新手训练
	bool GiveUpXinShouXunLian();
	bool SendGiveUpXinShouXunLian();
	//进入新手训练
	bool EnterXinShou();
	//移动到指定的房间
	OptinalBool SendMoveToTheRoom( const std::string& move_cmd, const GType::stGameVector3& door_pos, int target_room_num, bool force_send );
	//按键直到过房间
	bool PressKeyUntilSendMoveRoom( DWORD v_k, const GType::stGameVector3& door_pos );
	void OnGameSendMoveRoom( const CMDR_Hook_SendMoveRoom& msg );
	bool MoveToTheRoom( int room_num );
	bool MoveToTheRoomOfObj( GameObjBase& obj );
	//设置每移动一次的距离
	void SetMoveStepDist( float step_dist );
	//步过每个对象
	int StepOverObjs( const GameObjsContT& game_objs );
	//步过指定名字的静止对象
	int StepOverTheStaticObjs( const std::string& obj_name );
	//寻找一个合适的副本
	const GMapDb* FindSuitableFbInfo();
	//设置进入副本后的处理
	bool SetAfterEnteredFb( const luabind::object& func );

	//攻击/技能相关
public:
	bool UseSkill(CD_En_SkillId skill_category, WORD hit_times = 1);
	bool HitByLeftBt( WORD hit_times = 1 );
	bool HitByRightBt( WORD hit_times = 1 );
	bool PressKeyZ();
	bool SwitchKnife();
	//收刀
	bool TakeInKnife();
	//等待到空闲状态
	bool WaitToFree( int wait_time );
	//等待到doing状态
	bool WaitToDoingStatus( int wait_time );
	//调用攻击的时候，一定要考虑角色死亡的情况，还要清除攻击部位的数据
	OptinalBool HitMonster( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int obj_room_num, int role_room_num, bool is_any_direction );
	OptinalBool HitMonsterByPacketId( DWORD packet_id, GNormalGameObjMgrBase* obj_mgr, bool is_any_direction );
	//左键攻击指定的怪物直到死亡
	int HitTheMonsterToDie( DWORD packet_id, GNormalGameObjMgrBase* obj_mgr, int sleep_time, bool is_any_direction );
	//打怪时的处理
	void HandleAtHitting();
	//设置攻击部位信息
	void SetHitBodyPart( int body_part_idx, int hit_cnt, DWORD cur_packet_id );
	//设置此次攻击模式
	void SetCurHitMode(GClass::GHitMonsterMode_Mouse::enHitMode hit_mide);
	//设置攻击模式_使用背包物品
	bool SetHitMode_UsePacketItem( const std::string& item_name, int steps );
	//在炎热状态
	bool IsHotStatus();
	//在寒冷状态
	bool IsColdStatus();
	//在中毒状态
	bool IsPoisoningStatus();
	//存在奥义觉醒
	bool HasAoYiJueXing();
	//是泥人状态
	bool IsNiRenStatus();
	//是否存在指定的buff
	bool IsTheCurStatus( const std::string& buff_name );
	//燃烧状态
	bool IsRanShaoStatus();
	//臭气状态
	bool IsErChouStatus();
	//雪人状态
	bool IsSnowManStatus();
	//缠绕状态
	bool IsChanRaoStatus();

	//任务
public:
	//发包接任务
	bool SendAcceptTask( int task_id );
	bool AcceptTask( int task_id );
	//放弃任务
	bool GiveUpTask( int task_id );
	//发包交任务
	bool SendCommitTask( int task_id );
	bool CommitTask( int task_id );
	//进行对话
	bool DoNpcChat( int task_id );
	bool SendDoNpcChat( int task_id );
	//接受公会委托
	bool AcceptSociatyTask( const std::string& task_lvl, const std::string& task_name );
	bool SendAcceptSociatyTask( GType::enSociatyTaskLvl sociaty_task_lvl, DWORD task_fb_id );
	//自动去做指定的公会委托，这个函数是联系起GainTheFbInfo，是可以不用提供任务等级参数的，以后做到喵喵或千狩副本时再去完善
	bool AutoToDoTheSociatyTask( const std::string& task_lvl, const std::string& task_name, const luabind::object& do_func );
	//查询副本信息
	bool SendGainFbInfo(CD_En_FbType fb_type);
	bool GainTheFbInfo(CD_En_FbType fb_type);
	//坐热气球
	bool ZuoReQiQiu();
	//添加赏金任务白名单
	void AddShangJinWhiteList(const std::string& task_name, CD_En_FbDifficulty fb_diff, bool is_king_fb);
	//清空赏金任务白名单
	void ClrShangJinWhiteList();
	const GTaskDb* FilterShangJinWhiteList(const ShanJinTaskInfoT& shang_jin_task_info, int& the_fb_id) const;
	//添加赏金目标副本黑名单
	void AddShangJinDstFbBlackList( int fb_id );
	//清空赏金目标副本黑名单
	void ClrShangJinDstFbBlackList();
	bool IsInShangJinDstFbBlackList( int fb_id ) const;
	//添加喵喵副本黑名单
	void AddFbBlackList_Cat( int fb_id );
	//清空喵喵副本黑名单
	void ClrFbBlackList_Cat();
	//在喵喵副本黑名单
	bool IsInFbBlackList_Cat( int fb_id ) const;
	//添加副本黑名单
	void AddFbBlackList( int fb_id );
	//清空副本黑名单
	void ClrFbBlackList();
	//在副本黑名单
	bool IsInFbBlackList( int fb_id ) const;
	//添加赏金目标副本固定黑名单
	void AddShangJinDstFbStableBlackList( int fb_id );
	//清空赏金目标副本固定黑名单
	void ClrShangJinDstFbStableBlackList();
	bool IsInShangJinDstFbStableBlackList( int fb_id ) const;
	//添加采集黑名单
	void AddCollectBlackList( const std::string& collect_name );
	//清空采集黑名单
	void ClrCollectBlackList();
	bool IsInCollectBlackList( const std::string& collect_full_name ) const;

	//npc相关
public:
	bool SendOpenNpc( DWORD npc_packet_id, GType::enNpcOpenType open_type );
	bool OpenNpc( const GameObjBase& npc, GType::enNpcOpenType open_type = GType::kNOT_Normal );

	//item
public:
	//获取补给箱物品
	int GainSupplyItem( const std::string& item_name );
	bool SendGainSupplyItem( int item_id, int item_cnt, int slot_idx );
	bool UseItem(CD_En_ItemPackageCategory ipc, int item_slot_idx);
	bool UseItemByName( const std::string& item_name );
	int UseSomeItems( const std::string& item_name, int use_cnt );
	//使用任务物品
	bool UseTaskItem( const std::string& item_name );
	//锻造
	bool DoCraft( const std::string& item_name );
	bool DoCraftByDstEquipItemId( const GClass::CraftDbMgr::stCraftDb& craft_item );
	//购买
	bool DoBuy( const std::string& shop_category_name, const std::string& item_name, int buy_cnt, const std::string& village_name, const std::string& npc_name );
	//升级装备(只能升级穿在身上的装备)
	bool LvlUpEquip( const std::string& equip_name, int lvl_up_equip_idx );
	bool LvlUpTheEquip( GItem& the_equip, int lvl_up_equip_idx, bool is_log );
	//自动升级装备黑名单
	void AddAutoLvlUpEquipBlackList( const std::string& item_name );
	//清空自动升级装备黑名单
	void ClrAutoLvlUpEquipBlackList();
	bool IsInAutoLvlUpEquipBlackList( const GItem& the_item ) const;
	const ItemDb::stEquipLvlUpInfo* GetTheEquipLvlUpInfo( const ItemDb& the_equip, int lvl_up_equip_idx );
	//获取武器所差材料信息
	void GainWeaponNeedLvlUpInfo( std::vector<stNeedMaterialInfo>& out_info );
	bool SendLvlUpEquip( const GItem& item, const ItemDb::stEquipLvlUpInfo& lvl_up_info );
	//////////////////////////////////////////////////////////////////////////
	//****************移动物品*************************
	//智能移动物品，如果目标位置没有物品，则整个移动，如果存在物品则分割移动等
	bool SmartMoveItem(GItem& item_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst);
	//移动整个物品
	bool MoveTotalItem(GItem& item_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst);
	//合并移动物品
	bool MoveMergeItem( GItem& item_src, const GItem& item_dst );
	//分割移动物品
	bool MoveSplitItem(GItem& item_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst);
	bool SendMoveItem(const LONGLONG& item_serial, CD_En_TabCategory tbl_category_src, int slot_idx_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst);
	bool SendMoveMergeItem(const GItem& item, CD_En_TabCategory tbl_category_src, int slot_idx_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst, const LONGLONG& item_serial_dst);
	bool SendMoveSplitItem(const GItem& item, CD_En_TabCategory tbl_category_src, int slot_idx_src, CD_En_TabCategory tbl_category_dst, int slot_idx_dst);
	//////////////////////////////////////////////////////////////////////////
	//卖给商店
	bool SendSellToShop( const GItem& item );
	int SellItemsToShop( const GItemMgrBase::ContT& items, const std::string& village_name, const std::string& npc_name );
	//打开邮箱
	bool SendOpenMail();
	//删除邮箱
	bool SendDeleteMail( const LONGLONG& mail_serial );
	//获取邮件
	bool SendGainMailItem( const LONGLONG& mail_serial );
	//邮寄
	enSendMail SendMail(const SendMailContT* items, int item_reach_cnt, 
		int gold, const std::string& target_role_name, const std::string& village_name, const std::string& npc_name);
	bool SendMailHelper( int& send_item_cnt, std::deque<GItem>& send_items, GItemMgrBase::GameItemIterPtr& tra_it );
	bool BuildSendMailMsg(std::deque<GItem>& send_items, stCD_Msg_SendMail& msg);
	//从仓库和背包中查找
	boost::optional<GItem> FindItemFromPacketAndStore( const std::string& item_name, bool is_bind = true, int item_cnt = 1 );
	//自动存放物品到仓库
	void AutoStoreItemsToStored();
	//////////////////////////////////////////////////////////////////////////
	//***************************物品黑名单***********************************
	//初始化物品黑名单和白名单
	void InitItemBlackWhiteList();
	//清空物品黑名单和白名单
	void AfterRunScript();
	void PreRunScript();
	//卖掉仓库和背包的垃圾物品
	int SellStored_PacketRubbishItems( int packet_reserve_cnt, int stored_reserve_cnt, const std::string& village_name, const std::string& npc_name );
	void SellStored_PacketRubbishItemsImpl( int reserve_cnt, const GItemMgrBase::ContT& items, GItemMgrBase::ContT& black_list, luabind::object* filter_white_ptr, luabind::object* filter_black_ptr );
	//设置物品过滤黑名单
	void SetItemFilterBlackList( const luabind::object& item_func );
	//设置物品过滤白名单
	void SetItemFilterWhiteList( const luabind::object& item_func );
	//添加卖商店物品名黑名单
	void AddItemNameBlackList( const std::string& item_name );
	//清空卖商店物品名黑名单
	void ClrItemNameBlackList();
	//添加卖商店物品名黑名单
	void AddItemNameWhiteList( const std::string& item_name );
	//清空卖商店物品名黑名单
	void ClrItemNameWhiteList();
	//设置物品信息白名单
	void SetItemInfoWhiteList( int item_lvl, int item_quality );
	//设置锻造物品白名单
	void SetCraftItemWhiteList( int item_lvl, int item_quality );
	//设置升级装备白名单
	void SetLvlUpEquipWhiteList( int item_lvl, int item_quality );
	//设置调和白名单
	void SetFactureWhiteList( int item_lvl, int item_quality );
	//////////////////////////////////////////////////////////////////////////
	//添加存仓库物品白名单
	void AddStoreWhiteList( const std::string& item_name );
	//清空存仓库物品白名单
	void ClrStoreWhiteList();
	bool InStoreWhiteList( const GItem& item );
	//从仓库中移动到背包中这些白名单物品
	int StoreToPackageTheWhileList();
	//////////////////////////////////////////////////////////////////////////
	//整理物品
	bool ArrangeItems(CD_En_TabCategory tab_cate);
	//寻找物品
	GItem FindTotalItem( int item_id );
	GItem FindTotalItemByName( const std::string& item_name );
	GItem FindTotalItemByMatchName( const std::string& item_name );
	GItem FindTotalItem_TaskByMatchName( const std::string& item_name );
	GItem FindTotalItemFromPackeAndStoreByMatchName( const std::string& item_name );
	GItem FindTotalItemFromPackeAndStoreNormalByMatchName( const std::string& item_name );
	//领取礼包
	bool GainGiftBag( int gift_bag_id );
	//自动领取礼包
	int AutoGainGiftBag();
	//领取周狩猎券
	bool GainWeekShouLieQuan();
	//添加自动穿戴装备黑名单
	void AddAutoWearBlackList( const std::string& item_name );
	//清空自动穿戴装备黑名单
	void ClrAutoWearBlackList();
	bool IsInAutoWearBlackList( const GItem& item ) const;
	//在自动穿装白名单
	const WearWhiteListPairT* FindAutoWearWhiteListItem( const std::string& equip_name );
	void AddAutoWearWhiteList( const std::string& item_name, const std::string& item_type );
	//解锁王立精英副本
	bool UnlockKingCream(int fb_id, const std::string& unlock_item_name);

	//采集
public:
	//发包采集
	//bool SendCollect( DWORD );
	//采集一次指定的可采集对象
	bool CollectTheCollectableObj( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int collect_time );
	bool CollectTheCollectableObjById( DWORD packet_id, GNormalGameObjMgrBase* obj_mgr, int collect_time );
	bool CollectTheCollectableObjImpl( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int collect_time );
	//剥皮
	bool CutSkin( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int wait_time );
	bool CutSkinById( DWORD packet_id, GNormalGameObjMgrBase* obj_mgr, int wait_time );
	bool CutSkinImpl( GameObjBase& obj, GNormalGameObjMgrBase* obj_mgr, int wait_time );
	//升级庄园采集
	int LvlUpCollectOfFarm(CD_En_FarmFacilityIdx idx, int lvl_limit, bool is_open_npc);
	bool IsFreeTimeToCollectOfFarm(CD_En_FarmFacilityIdx idx);
	//进入自己的庄园
	bool EnterSelfFarm();
	//离开庄园
	bool LeaveFarm();
	//获得一个已有的种子名
	std::string GetExistFarmSeedName();
	//种植种子
	bool PlantFarmSeed( const std::string& seed_name, const std::string& land_name );
	//计算下次庄园采集的时间
	bool CalcNextCollectFarmTime();
	//到了庄园采集的时间
	bool IsTimeToCollectFarm( const std::string& collect_name );

	//交易相关
public:
	//挂拍卖
	bool SendSellToAuction( const GItem& item, int begin_price, int smart_price );
	bool SellToAuction( const std::string& item_name, int item_cnt, int begin_price, int smart_price, bool private_sell, 
		const boost::optional<int>& relative_smart_price, const boost::optional<int>& min_smart_price, const boost::optional<int>& average_cnt, bool begin_price_direct );
	//刷新我的拍卖
	bool SendRefreshMySellList( bool force_fresh );
	bool RefreshMySellList( bool force_fresh = false );
	//以防换角色的时候，会把上一个角色拍卖的东西都给删除咯
	void ClrMySellList();
	//通过我的拍卖列表数据的变化，自动通知对方阵营
	void NotifyOtherSideByMySellListChanged();
	//查询已拍卖条目数
	int QuerySoldCnt( const std::string& item_name );
	//一口价购买
	bool SendSmartBuy( const stAuctionItemInfo& item );
	bool CanSmartBuy( const stAuctionItemInfo& item );
	int SmartBuy( const std::string& item_name, int seach_page_cnt, const std::string& seller, int price_limit, int buy_cnt );
	//购买一些对方阵营的拍卖物品
	int SmartBuyOtherSide();
	const stAuctionItemInfo* FindOtherSideItem( const stAuctionItemInfo& item_info );
	void AddOtherSideItem( const stAuctionItemInfo& item_info );
	void EraseOtherSideItem( const stAuctionItemInfo& item_info );
	void EraseMySellItem( const stAuctionItemInfo& item_info );
	bool AddOtherSideItemsFromJson( const Json::Value& j_value );
	void EraseOtherSideItemsFromJson( const Json::Value& j_value );
	void GenPrivateToJson( Json::Value& j_value );
	void GenMySellListToJson( Json::Value& j_value );
	void GenOthersSellToJson( Json::Value& j_value );
	const stAuctionItemInfo* SmartBuyOneImpl( const std::string& seller, int price_limit );
	//查询拍卖信息
	bool SendGainAuctionInfo( const ItemDb& item_template );
	//查寻指定页面的拍卖信息
	bool SendGainAuctionThePageInfo( int page_idx );
	//是否是私有拍卖
	static bool IsPrivateSell( int price );
	static int MakePrivateSellPrice( int price );
	static int MakePublicSellPrice( int price );
	//////////////////////////////////////////////////////////////////////////
	//*******************当面交易*********************************************
	//请求交易
	bool RequestDeal( const GPlayer& other );
	//接受一个交易请求
	bool AcceptOneDealRequest();
	//移动物品到交易栏
	bool MoveItemToDealTab( const GItem& item, int dst_slot_idx );
	//锁定交易栏
	bool LockDealTab();
	//确认交易
	bool EnterDeal();
	//交易金币
	bool DealGold( int gold );
	//////////////////////////////////////////////////////////////////////////

	//调和
public:
	//制作调和物品
	bool ManuFactureItem( const std::string& item_name );
	
	//math
public:
	//向量到方向
	GameObjDirT TwoPos2Dir( const GType::stGameVector3& vec_src, const GType::stGameVector3& vec_dst ) const;
	GameObjDirT Vec2Dir( const GType::stGameVector3& vec_dir ) const;
	//方向到向量
	GType::stGameVector3 Dir2Vec( const GType::stGameVector3& dir_lhs, const GType::stGameVector3& dir_rhs ) const;
	//distance2steps
	bool Dist2Steps( const GType::stGameVector3& pos_src, const GType::stGameVector3& pos_dst, PosQueueT& pos_steps );
	//从pos info中得到2D向量，主要用于修改发包
	bool PosInfo2Dir2d(const stCD_PosDirInfo& pos_info_src, const stCD_BodyPartInfo& body_part_info, GType::stDirection2d& out_dir);
	//根据部位信息得到角色应移动到的坐标信息
	bool BodyPartInfo2RolePos(const stCD_PosDirInfo& pos_info_src, const stCD_BodyPartInfo& body_part_info, GType::stGameVector3& out_pos, GameObjDirT& out_dir);

	//update some data
public:
	bool Update() override;
	bool UpdateRolePos();
	bool UpdateRolePosSync();
	bool UpdatePacketId() override;
	bool UpdatePacketIdSync();
	bool UpdateCurMapInfo();
	bool UpdateCurMapInfo_Notify();
	bool UpdateGrids();
	bool UpdateReceivedTasks();
	bool UpdateAcceptableTasks();
	bool UpdateEnterableFb();
	bool UpdateRoleLvl();
	bool UpdateRoleLvl_Notify();
	bool UpdateExp();
	bool UpdateExp_Notify();
	bool UpdateHp();
	bool UpdateHpSync();
	bool UpdateGold();
	bool UpdateGold_Notify();
	bool UpdateLianQiCao();
	bool UpdateLianQiCaoSync();
	bool UpdateZhanWeiZhi();
	bool UpdateZhanWeiZhiSync();
	bool RefreshZhanWeiZhiMax();
	bool UpdatePoisoningState();
	bool UpdatePoisoningStateSync();
	bool UpdateShouLieQuanCnt();
	bool UpdateLeftShouLieQuanCnt_Notify();
	bool UpdateJingSuanDian();
	bool UpdateJingSuanDianSync();
	bool UpdateCurFarmFacilityInfo();
	bool UpdateOwnerRoomNum() override;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

	//another
public:
	bool IsInVillage() const;
	bool IsInFb() const;
	bool IsInSelfFarm() const;
	//等待角色复活
	bool WaitRoleAlive();
	//普通副本已经打完
	//bool IsNormalFbOver() const;
	//等待安全时间
	void WaitSafeTime();
	bool IsSafeTime();
	//告诉疲劳值
	void TellFatigueData( pt_qword fatigue_data );
	//计算疲劳值
	unsigned __int64 CalcFatigueData();
	//需要磨刀
	bool NeedMoDao();
	//是否在高度黑名单中
	bool IsInMonsterHighBlackList( const std::string& boss_name ) const;
	//添加好友
	bool AddNewFriend( const std::string& role_name );
	//得到当前有效副本消耗模式
	GType::enFbConsumeMode GetCurFbValidConsumeMode();

	//test
public:
	void Test( int param );
	void Test1( const luabind::object& func );
	bool SendSay( const std::string& say_msg );
	void DebugInfo() const override;
	void SetDeltaAngle( float delta_angle );
	void DebugGridsInfo() const;
	void DebugReceivedTaskInfo() const;
	void DebugAcceptableTasksInfo() const;
	void DebugEnterableFb() const;
	void DebugObjStatus() const;
	void DebugMySellList() const;
	void DebugGainAuctionPagesInfo() const;
	void DebugCurFarmFacilityInfo() const;

private:
	const char*				actor_obj_addr_;
	stCD_NetObj_Role*		cur_role_;
	stCD_SkillObj*			game_skill_;
	stCD_MapDb*			cur_map_db_;

private:
	GItemMgr_Package		grids_package_;			//背包
	GItemMgr_Equip			grids_equip_;			//装备
	GItemMgr_Store			grids_store_;			//仓库
	GItemMgr_SuCai			grids_su_cai_;			//素材
	GItemMgr_Task			grids_task_;			//任务
	GReceivedTaskMgr		received_task_mgr_;
	GAcceptableTasks		acceptable_tasks_;
	GEnterableFbMgr			enterable_fb_;
	GClass::CurFarmFacilityInfo	cur_farm_facility_info_;
	GClass::CurFarmFieldInfo	cao_yao_tian_info_;

	//游戏变量
private:
	int						cur_hp_;
	int						last_update_hit_hp_;
	int						feeb_hp_;
	int						role_lvl_;
	int						left_single_exp_;
	int						left_double_exp_;
	int						left_shou_lie_quan_cnt_;	//剩余狩猎券
	int						gold_;		//金币
	int						silver_;	//银币
	int						wait_safe_time_cnt_;
	int						lian_qi_cao_;
	int						zhan_wei_zhi_;
	int						zhan_wei_zhi_max_;
	CD_En_PoisonStatus		poisoning_state_;
	int						jing_suan_dian_;
	std::string				cur_map_name_;			//当前所在地图的名字
	int						cur_map_id_;
	int						cur_template_map_id_;
	std::string				cur_map_db_info_name_;

	//msg
public:
	MsgOnEnterFbT			msg_on_enter_fb_;
	MsgOnEnterFbErrT		msg_on_enter_fb_err_;
	MsgOnFbInfoOptionalT	msg_last_fb_info_;
	//MsgOnFbInfoT			msg_fb_info_sociaty_;
	//MsgOnFbInfoT			msg_fb_info_miao_miao_;
	//MsgOnFbInfoT			msg_fb_info_qian_shou_;
	MsgMySellListT			msg_my_sell_list_old_;
	TimeElapse				time_to_refresh_my_sell_list_;
	MsgMySellListT			msg_on_my_sell_list_;
	MsgAuctionPageInfoT		msg_last_page_info_;
	AuctionPagesInfoT		msg_auction_pages_info_;
	MsgOnLeftTimeT			msg_left_time_;
	MsgOnBuyAuctionT		msg_on_buy_auction_;
	AuctionItemsContT		other_side_items_;
	MsgOnAcceptMailT		msg_on_accept_mail_;
	MsgOnOpenNpcT			msg_on_open_npc_;
	MsgOnSoldAuctionT		msg_on_sold_auction_;
	MsgOnSoldToShopT		msg_on_sold_to_shop_;
	MsgCommitTaskFailedT	msg_commit_task_failed_;
	MsgOnDoNpcChatT			msg_on_do_npc_chat_;
	MsgOnSendMailT			msg_on_send_mail_;
	MsgOnSendMail2T			msg_on_send_mail2_;
	MsgOnUseItemRet1T		msg_on_use_item_ret1_;
	MsgOnUseItemRet2T		msg_on_use_item_ret2_;
	MsgOnBuyItemT			msg_on_buy_item_;
	MsgOnCollectRetT		msg_on_collect_ret_;
	MsgOnArrangeItemsRetT	msg_on_arrange_items_ret_;
	MsgOnSendMoveRoomRetT	msg_on_send_move_ret_;
	MsgOnAcceptTaskRetT		msg_on_accept_task_;
	MsgOnGiveupTaskRetT		msg_on_giveup_task_;
	MsgOnRequestDealT		msg_on_request_deal_;
	MsgOnRequestDealFailT	msg_on_request_deal_fail_;
	MsgOnRecvQuestDealT		msg_on_recv_quest_deal_;
	MsgOnCancelDealT		msg_on_cancel_deal_;
	MsgOnDealFailT			msg_on_deal_fail_;
	MsgOnMoveItemToDealTabT	msg_on_move_item_deal_tab_;
	MsgOnMoveDealGoldT		msg_on_move_deal_gold_;
	MsgOnRecvDealStatusT	msg_on_recv_deal_status_;
	MsgOnRecvDealStatusT	msg_on_deal_status_self_;
	MsgOnRecvDealStatusT	msg_on_deal_status_other_;
	MsgOnRecvDealEndT		msg_on_recv_deal_end_;
	MsgOnGetMailItemsInfoT	msg_on_get_mail_items_info_;

	//辅助变量
public:
	float					delta_angle_;			//世界坐标系与面向坐标系之间的夹角
	float					move_step_dist_;
	CMDR_Hook_SendMoveRoom	msg_on_hook_move_room_;
	//SharedMutex				mutex_msg_on_hook_move_room_;
	volatile bool			is_send_move_room_;
	int						last_room_id_for_send_room_;
	//boost::optional<GType::stGameVector3>		last_send_role_pos_;
	bool					is_active_cross_map_;		//是否主动过图
	bool					is_hitting_monster_;
	bool					is_throw_data_error_;
	int						min_gold_to_keep_buy_;				//身上的金币大于这个数，才会去保持购买
	int						min_silver_to_keep_buy_;			//身上的金币大于这个数，才会去保持购买
	bool					is_open_village_path_;
	int						fb_server_full_cnt_;					//服务器已满次数
	bool					is_dealing_handle_;
	bool					handle_can_not_move_state_;
	GType::enFbConsumeMode	fb_consume_mode_;
	int						fb_consume_mode_min_silver_;
	bool					move_fb_strict_;

	bool					is_right_bt_hit_;
	boost::optional<stLastSendMoveRoomInfo>	last_send_move_cmd_;
	std::pair<GType::stGameVector3, int>	last_valid_pos_;

	//物品黑名单和白名单
private:
	std::shared_ptr<CmnPriLuaData>	item_black_white_list_;
	NameList_StrT						item_name_black_list_;
	NameList_StrT						item_name_white_list_;
	int							white_list_item_lvl_;			
	CD_En_ItemQuality		white_list_item_quality_;
	int							white_list_craft_item_lvl_;			//锻造后的物品的等级
	CD_En_ItemQuality		white_list_craft_item_quality_;
	int							white_list_lvl_up_equip_lvl_;
	CD_En_ItemQuality		white_list_lvl_up_equip_quality_;
	int							white_list_facture_lvl_;
	CD_En_ItemQuality		white_list_facture_quality_;

	NameList_StrT					auto_lvl_up_equip_black_list_;
	NameList_StrT					store_white_name_list_;

	ShangJinWhiteListT			shang_jin_white_list_;
	NameList_IntT				shang_jin_dst_fb_black_list_;
	NameList_IntT				shang_jin_dst_fb_stable_black_list_;
	NameList_IntT				cat_fb_black_list_;
	NameList_IntT				fb_black_list_;					//副本黑名单

	NameList_StrT					collect_black_list_;			//采集黑名单
	NameList_StrT					auto_wear_black_list_;			//自动穿戴装黑名单

public:
	WearWhiteListT					auto_wear_white_list_;

public:
	GClass::GHitMonsterModeBase*	cur_hit_mode_;
	GClass::GHitMonsterMode_Mouse	hit_mode_mouse_;
	GClass::GHitMonsterMode_UsePacketItem	hit_mode_use_packet_item_;
	CMDR_OnFbInfo::FbIdsT					fb_info_cat_;			//收包得到的喵喵副本
};
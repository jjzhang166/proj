#pragma once
/*
@author zhp
@date 2016/4/11 21:39
*/
#include <ProtocolX/P_DefinesX.h>
#include <cstddef>
#include <Windows.h>
#include "CD_StructsBase.h"

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//MM0_RECV_REQ_ITEM_BY_TYPE
struct stCD_MsgR_ReqItemByType{
	DWORD	dwReqItemById;		// 拍卖的物品id，非物品id，需要查表确认物品id
	DWORD	dwReqItemByKey;		// 操作拍卖物品需要用到的key值
	DWORD	dwUnKnow1;
	DWORD	dwUnKnow2;
	char	szReqItemByPlayerName[kRoleBufferMaxSize];
	pt_byte	bFlag;
	DWORD	dwUnKnow3;
	int		nReqItemByPrice1;		// 起拍价
	int		nReqItemByPrice2;		// 一口价
	int		nReqItemByPrice3;		// 竞拍价
	p_mem_placeholder(g_o_msg_req_item_1_ - 4 - 61);
	pt_word	ukn_;		//UPDATES_MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET1
	int		ukn_id_;	//UPDATES_MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET2
	p_mem_placeholder(0xef - 4 - g_o_msg_req_item_2_);
};
static_assert(offsetof(stCD_MsgR_ReqItemByType, nReqItemByPrice3) == 61, "");
static_assert(sizeof(stCD_MsgR_ReqItemByType) == 65 + 0xae, "");
static_assert(offsetof(stCD_MsgR_ReqItemByType, ukn_) == g_o_msg_req_item_1_, "");
static_assert(offsetof(stCD_MsgR_ReqItemByType, ukn_id_) == g_o_msg_req_item_2_, "");

struct stCD_MsgR_OnRefreshMySellList{
	p_mem_placeholder(8);
	stCD_MsgArray<stCD_MsgR_ReqItemByType, pt_word, 1> req_items_;		//MMOCLIENT_REVERSAL_PACKET_RECV_MSG_MY_REQ_COUNT_OFFSET___ MMOCLIENT_REVERSAL_PACKET_RECV_MSG_MY_REQ_DATA_OFFSET___
};

//MMO_RECV_REQ_ITEM_BY_HEADER
struct stCD_MsgR_OnGainAuctionPageInfo{
	DWORD	dwRet;		// 返回值
	int		nDoCount;	// 计数
	DWORD	dwUnKnow;	// 不清楚，不知道
	int		nReqItemCount;	// 物品的总数量，页数需要除以8，如果存在余数，需要商+1
	stCD_MsgArray<stCD_MsgR_ReqItemByType, pt_word, 1> req_items_;
};

//玩家链表结构 MM0_PLAYER_LIST
struct stCD_MsgR_RoleList_Role{
	pt_dint		ukn1_;
	DWORD		dwPlayerId;
	char		szPlayerName[kRoleBufferMaxSize];
	pt_byte		bChosseFlag;
	int			nPlayerLevel;
	pt_byte		bszUnKnow[0x501 - 0x31];
};
static_assert(sizeof(stCD_MsgR_RoleList_Role) == 0x501, "");

struct stCD_MsgR_RoleList{
	int			ukn_[3];
	stCD_MsgArray<stCD_MsgR_RoleList_Role, pt_word, 6>	role_list_;	//MMOCLIENT_REVERSAL_RECV_PLAYER_LIST_COUNT___  MMOCLIENT_REVERSAL_RECV_PLAYER_INFO_OFFSET___
};

//MMO_GAME_IN_FB_RET_PACKET
struct stCD_MsgR_OnEnterFb{
	DWORD	dwRolePacketId;
	int		nError;		// 0表示可以正常进入副本  非0表示不可进入副本，如激运副本疲劳状态，公会委托副本已经打完，注意：公会副本可以打3次，但是每个副本只能打一次
	//int		nRet;		// 1表示普通副本返回值和激运副本返回值  2表示公会副本返回值 
};
//sizeof(stCD_MsgR_OnEnterFb)==66

//MMO_RECV_USE_ITEM_RET
struct stCD_MsgR_OnUseItemRet1{
	byte	bRet;			// 返回值=1
	WORD	wUnKnow;
	DWORD	dwItemId;		// 物品id
	byte	bUnKnow;
	WORD	wSlotIndex;		// 物品所在格子
	//其实这个结构不是pod
	p_mem_placeholder(56 - 10);
};

struct stCD_MsgR_OnSendMail{
	int			res_;
	pt_dint		ukn_;		//==0
	char		str_dst_role_name_[20];			//被邮寄的名字
};

//MMO_RECV_SEND_MAIL_RET2
struct stCD_MsgR_OnSendMail2{
	int		nRet;		// 返回值测试时一直为1
	DWORD	dwErrorValue;	// 封包错误值
	int		ukn_1_;		//==0
	int		ukn_2_;
	//这里还有个字段，应该表示的是字符串
};

//MMO_RECV_REQUEST_DEAL_FIAL_RET
struct stCD_MsgR_OnRequestDealFail{
	enum enSubType{
		kOnDealFail = 4,
	};

	DWORD		dwRequestDealFailValue;		// 返回值
	DWORD		dwUnKnow1;
	pt_dint		ukn_;
	pt_byte		bUnKnow2;
	DWORD		dwUnKnow3;
	pt_byte		sub_type_;	//==4的时候，szRoleName字段才表示名字
	char		szRoleName[21];
};

struct stCD_MsgR_OnRecvQuestDeal{
	pt_dword	deal_key_;
	pt_byte		ukn_1_;		//==1
	int			ukn2_;		//这个字段要不是deal_key_，要不就是交易者的ID
	pt_byte		ukn3_;		//==1,以后是一个序列化的数组。
};

//MMO_RECV_OPEN_NPC_RET
struct stCD_MsgR_OnOpenNpc{
	pt_byte	res_;		//打开成功返回值=2
	int		ukn_zero_;
	int		open_flag_;	//==1  nOpenFlag
	int		npc_packet_id_;		//100000c5
	int		ukn_2_;
	int		ukn_3_;

	int		role_packet_id_;
	int		ukn_4_;
	int		ukn_5_;
	int		ukn_6_;
	int		ukn_7_;
};

//////////////////////////////////////////////////////////////////////////
//////////////////////*******以下继承自stCD_CheckMsgAtRuntime**************
//MMO_GAME_LINE
BC_DEFINE_POD(stCD_MsgR_SerLineInfo){
	WORD	wLineIndex;
	pt_byte bLineLevel;
	pt_byte bUnKnow;
};
struct stCD_MsgR_SerLine : stCD_CheckMsgAtRuntime{	
	template<typename T>
	void serialize(T& stm){
		stm & ser_lines_;
	}

	stCD_MsgArray<stCD_MsgR_SerLineInfo, pt_word>		ser_lines_;
};

//MMO_FB_SERVER_FULL_RET
struct stCD_MsgR_OnEnterFbError : stCD_CheckMsgAtRuntime{
	template<typename T>
	void serialize(T& stm){
		stm & dwUnKnow1
			& dwUnKnow2
			& dwRetValue
			& dwRolePacketId
			& BC_OriStrArray<decltype(szRoleName)&>(szRoleName);
	}

	DWORD dwUnKnow1;
	DWORD dwUnKnow2;
	DWORD dwRetValue;		// 测试服务器已满返回值=0x73 放弃副本返回值=0x6A
	DWORD dwRolePacketId;	// 角色对象的id
	char  szRoleName[20];	// 角色名字
};

//MMO_TASK_FB_INFO_
struct stCD_MsgR_TaskFbInfo{
	DWORD	dwFbType;		// 副本类型  喵喵=0x00000065 千狩=0x000000C9
	stCD_MsgArray<int, pt_word, 0x32>	fb_ids_;

	template<typename T>
	void serialize(T& stm){
		stm & dwFbType & fb_ids_;
	}
};

//MMO_TASK_FB_INFO
struct stCD_MsgR_OnFbInfo : stCD_CheckMsgAtRuntime{
	template<typename T>
	void serialize(T& stm){
		stm & dwRetValue & fb_info_;
	}

	DWORD	dwRetValue;
	//DWORD	dwTaskFbType;		// 01表示公会副本  04 表示千狩副本  03 表示喵喵副本
	stCD_MsgArray<stCD_MsgR_TaskFbInfo, pt_word, 1> fb_info_;
};

//MMO_RECV_COLLECT_ITEM_BAG_FULL_RET
struct stCD_MsgR_OnCollectRet : stCD_CheckMsgAtRuntime{
	int		nRet;			// 返回值为1，其余不用管
	DWORD	dwCollectObjPacketId;	// 当前被采集的对象id
	DWORD	dwUnKnow1;

	struct stItemInfo{
		DWORD	dwCollectItemId;		// 采集得到的物品id
		int		item_cnt_;
		int		ukn_zeor_;

		template<typename T>
		void serialize(T& stm){
			stm & dwCollectItemId & item_cnt_ & ukn_zeor_;
		}
	};
	stCD_MsgArray<stItemInfo, int> gained_items_;

	template<typename T>
	void serialize(T& stm){
		stm & nRet & dwCollectObjPacketId & dwUnKnow1 & gained_items_;
	}
};

//MMO_RECV_SELL_ITEM_RET
struct stCD_MsgR_OnSoldItemToShop : stCD_CheckMsgAtRuntime{
	int		nRet;			// 成功=0 不可出售=8
	DWORD	dwSellItemId;	// 成功后有出售物品的id
	int		sell_item_cnt_;
	int		ukn1_;
	stCD_MsgArray<pt_byte, pt_word>	ukn_;

	template<typename T>
	void serialize(T& stm){
		stm & nRet & dwSellItemId & sell_item_cnt_ & ukn1_ & ukn_;
	}
};
//////////////////////////////////////////////////////////////////////////
//////////////////////*******以下继承自stCD_CheckMsgAsPOD******************
struct stCD_MsgR_OnCreateRole : stCD_CheckMsgAsPOD{
	int		res_;
	int		ukn_[2];
	pt_word	ukn1_;
};
static_assert(sizeof(stCD_MsgR_OnCreateRole) == 0xe, "");

struct stCD_MsgR_OnSafeTime : stCD_CheckMsgAsPOD{
	int		left_time_;
};

//MMO_RECV_REQ_BUY_ITEM_RET
struct stCD_MsgR_OnBuyAuction : stCD_CheckMsgAsPOD{
	DWORD dwBuyItemRet;		// 购买成功 = 0
	pt_dint	req_item_id_;	//购买的物品拍卖id
	pt_int	ukn_;		//==0xa
	pt_byte	ukn1_;		//==0
};

//PMM0_ACCEPT_MAIL_RET_VALUE
struct stCD_MsgR_OnAcceptMail : stCD_CheckMsgAsPOD{
	int		nSize;
	DWORD	nRetValue;			// nRetValue = 0， nRetError = -1 表示提取附件失败
	pt_dint	mail_serial_;
	int		nRetError;			// nRetValue = 0x29， nRetError = -2 表示提取附件失败:超出携带上限
	int		ukn_;		//==3
	pt_byte	ukn_1_;		//==1
};

//MMO_RECV_SHANG_JIA_ITEM_RET
struct stCD_MsgR_OnSoldAuction : stCD_CheckMsgAsPOD{
	DWORD	dwRetValue;		// 上架成功返回值=0
	pt_dint	product_id_;	//上架物品的ID
};

//MMO_RECV_COMPLETE_TASK_GAIL_RET
struct stCD_MsgR_OnCommitTaskFailed : stCD_CheckMsgAsPOD{
	int		nRet;		// 背包已满，无法提交任务 = 3，无法接赏金任务的返回值 = 1
	DWORD	dwTaskId;	// 提交任务的id
};

struct stCD_MsgR_OnDoNpcChat : stCD_CheckMsgAsPOD{
	int		task_id_;
	int		zero_;	//=0
	int		ukn_1_;	//=1
};

//MMO_RECV_FEEB_HP_RET
struct stCD_MsgR_OnBuffStatus : stCD_CheckMsgAsPOD{
	DWORD	dwActorPacketId;
	DWORD	dwUnKnow1;
	DWORD	dwUnKnow2;
	DWORD	dwUnKnow3;
	DWORD	dwUnKnow4;
	DWORD	dwPoisoningFeebHpFlag;	// 中毒持续减血标志 如果为0则表示炎热天气或者寒冷天气导致的持续减血  如果不为0，则表示中毒
};

//MMO_RECV_BUY_ITEM_RET
struct stCD_MsgR_OnBuyItem : stCD_CheckMsgAsPOD{
	int		nRet;			// 成功返回0 购买数量已达上限 0x20
	DWORD	dwUnKnow;
	DWORD	dwShopItemId;	// 商品id，非物品id
	int		nCount;			// 购买数量
};

//GUO_TU_RET
struct stCD_MsgR_OnSendMoveRoomRet : stCD_CheckMsgAsPOD{
	pt_dint	ukn_;
	DWORD	dwRolePacketId;
	DWORD	dst_room_num_;		//目标房间号
	DWORD	dwUnKnow4;			//==0
	DWORD	dwUnKnow5;			//==0
	int		ukn_1_;
	int		ukn_2_;
	stCD_Direction2		theRoleDirection;
	stCD_Vector3		theRoleVector;
};

//MMO_ACCEPT_TASK
struct stCD_MsgR_OnAcceptTaskRet : stCD_CheckMsgAsPOD{
	DWORD				dwTaskId;
	DWORD				dwTaskZero;
};

//MMO_GIVE_UP_TASK
struct stCD_MsgR_OnGiveupTaskRet : stCD_CheckMsgAsPOD{
	DWORD				dwTaskId;
};

//MMO_RECV_DELETE_PLAYER
struct stCD_MsgR_OnDeleteRole : stCD_CheckMsgAsPOD{
	int nRet;
	int nRetParamer;
};

//MMO_RECV_REQUEST_DEAL_RET
struct stCD_MsgR_OnRequestDeal : stCD_CheckMsgAsPOD{
	DWORD dwRequestDealRoleId;		// 请求交易的对象id		注意：谁请求，就谁在前，这个包请求和接受通用的
	DWORD dwByRequestDealRoleId;	// 同意交易的对象id
};

struct stCD_MsgR_OnCancelDeal : stCD_CheckMsgAsPOD{
	pt_dword	role_id_of_cancel_;
};

//MMO_RECV_DEAL_FAIL_RET
struct stCD_MsgR_OnDealFail : stCD_CheckMsgAsPOD{
	/*pt_word	res_;
	pt_word	ukn_;*/
	int		dwDealFailRet;		//其实这个字段是两个word类型。0x0029001E 物品超出携带上限  0x0029000B 对方已经在交易中
	int		deal_role_id_;		//交易对象的id
	pt_word	ukn1_[3];
};

//MMO_DEAL_MOVE_ITEM
struct stCD_MsgR_OnMoveItemToDealTab : stCD_CheckMsgAsPOD{
	WORD	wScrItemTabIndex;	// 背包=0 仓库素材=0x0A 普通仓库=2
	WORD	wScrItemSlotIndex;	// 背包或仓库栏格子，从0开始计数
	WORD	wMoveItemCount;		// 移动物品的数量
	WORD	wDealSlotaIndex;	// 交易栏的格子，从0开始计数
};

//MMO_RECV_MOVE_DEAL_GOLD_RET
struct stCD_MsgR_OnMoveDealGold : stCD_CheckMsgAsPOD{
	DWORD dwMoveDealGoldRoleId;
	int	  nMoveDealGoldValue;
};

//MMO_RECV_DEAL_STATUS_RET
struct stCD_MsgR_OnRecvDealStatus : stCD_CheckMsgAsPOD{
	DWORD dwDoDealRoleId;		// 操作交易的对象id
	pt_byte  bDoDealIndex;			// 1 表示取消锁定 2 表示操作的锁定交易 3表示操作的确认交易
};

struct stCD_MsgR_OnRecvDealEnd : stCD_CheckMsgAsPOD{
	pt_byte		res_;
};

struct stCD_MsgR_MonsterDead : stCD_CheckMsgAsPOD{
	p_mem_placeholder(8);
	pt_dword		monster_id_;	//MMOCLIENT_REVERSAL_PACKET_MSG_MOSTER_SIWANG_PACKET_ID
};

struct stCD_MsgR_RoleDead : stCD_CheckMsgAsPOD{
	pt_dword		packet_id_;	//MMOCLIENT_REVERSAL_RECV_PACKET_DATA_OFFSET___
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

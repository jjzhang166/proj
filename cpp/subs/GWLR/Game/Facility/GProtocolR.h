#pragma once
/*
@author zhp
@date 2014/8/19 15:44
@purpose for recv/read
*/
#include "GProtocolBase.h"
#include <memory>
#include <Communicate/CG_Defines.h>
#include <boost/optional.hpp>
#include "GTypes.h"
#include <CD_CommonFiles/Data/CD_ProtocolS.h>
#include <CD_CommonFiles/Data/CD_ProtocolR.h>

typedef stCD_MsgBase CMDR_HeaderBase;
static_assert(sizeof(CMDR_HeaderBase) == sizeof(stCD_MsgBase), "");

struct CMDR_MsgBase : std::enable_shared_from_this<CMDR_MsgBase>{
public:
	typedef std::shared_ptr<CMDR_MsgBase> SelfPtr;

public:
	virtual CMDR_MsgBase* Clone() const;
	//返回true则OnReceived会被调用，否则不会调用
	virtual bool DoDirect( const CMDR_HeaderBase* msg );
	virtual void OnReceived() const;
	virtual boost::optional<WORD> NewRetOffset() const;
	virtual const char* NewRetAddr() const;
};
#define CMDR_INPUT_IMPL(DerivedT) \
	DerivedT* Clone() const override{	\
	return new DerivedT(*this);	\
}	\
/**/
//////////////////////////////////////////////////////////////////////////
struct CMDR_MonsterDead : CMDR_MsgBase{
	CMDR_MonsterDead();
	CMDR_INPUT_IMPL(CMDR_MonsterDead)
	static const WORD kMsgId = 0x283;	// MMOCLIENT_REVERSAL_PACKET_MSG_MOSTER_SIWANG___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	DWORD		monster_id_;
};

struct CMDR_RoleDead : CMDR_MsgBase{
	CMDR_RoleDead();
	CMDR_INPUT_IMPL(CMDR_RoleDead)
	static const WORD kMsgId = 0x00000249;	// MMOCLIENT_REVERSAL_PACKET_MSG_PLAYER_SIWANG___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	DWORD		packet_id_;
};

struct CMDR_PlayerList : CMDR_MsgBase{
public:
	struct stPlayerInfo{
		DWORD			player_id_;
		std::string		player_name_;
		int				player_lvl_;

		bool ConvertToRoleInfo( CG_RoleInfo& role_info ) const;
	};
	typedef std::vector<stPlayerInfo> PlayerInfoContT;

public:
	CMDR_INPUT_IMPL(CMDR_PlayerList)
	static const WORD kMsgId = 0x02;	// MMOCLIENT_REVERSAL_PACKET_RECV_MSG_PLAYER_LIST___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

protected:
	bool GenRolesInfoToJson( Json::Value& json_array ) const;

public:
	PlayerInfoContT		players_info_;
};

struct CMDR_PlayerLine : CMDR_MsgBase{
public:
	struct stLineInfo{
		WORD						line_idx_;
		GType::enPlayerLineType		line_lvl_;
	};
	typedef std::vector<stLineInfo> LineInfoContT;

public:
	CMDR_INPUT_IMPL(CMDR_PlayerLine)
	static const WORD kMsgId = 0x00000054;	// MMOCLIENT_REVERSAL_PACKET_RECV_MSG_LINE___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	LineInfoContT		lines_info_;
};

struct CMDR_OnCreatePlayer : CMDR_MsgBase{
	enum enErrCode{
		kEC_CntLimit = 1,	//角色数量已达到上限
		kEC_InvalidChar = 3,	//角色名含有非法字符
		kEC_Reged = 4,		//角色名已被注册
	};

	CMDR_OnCreatePlayer();
	CMDR_INPUT_IMPL(CMDR_OnCreatePlayer)
	static const WORD kMsgId = 0x00000004;	// MMOCLIENT_REVERSAL_PACKET_RECV_MSG_CREATE_ROLE_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int		res_;
};

struct CMDR_OnEnterFb : CMDR_MsgBase{
	enum enFbType{
		kFT_Normal = 1,			//普通副本返回值
		kFT_Sociaty = 2,		//公会副本返回值
	};

	CMDR_OnEnterFb();
	CMDR_INPUT_IMPL(CMDR_OnEnterFb)
	static const WORD kMsgId = 0x22;	// MMOCLIENT_REVERSAL_PACKET_RECV_MSG_IN_FB_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	enFbType	fb_type_;
	int			error_res_;
};

struct CMDR_OnEnterFbError : CMDR_MsgBase{
	CMDR_OnEnterFbError();
	CMDR_INPUT_IMPL(CMDR_OnEnterFbError)
	static const WORD kMsgId = 0x2D;	// MMOCLIENT_REVERSAL_PACKET_RECV_MSG_FB_SERVER_FULL_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			error_res_;
};

struct CMDR_OnFbInfo : CMDR_MsgBase{
	typedef std::vector<int> FbIdsT;
	struct stFbInfo{
		int		fb_type_;		//喵喵=0x00000065 千狩=0x000000C9
		FbIdsT	fb_ids_;
	};
	typedef std::vector<stFbInfo> FbInfoT;

	CMDR_OnFbInfo();
	CMDR_INPUT_IMPL(CMDR_OnFbInfo)
	static const WORD kMsgId = 0x0000003E;	// MMOCLIENT_REVERSAL_PACKET_RECV_TASK_FB_INFO___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;
	FbIdsT GetFbInfo_Cat() const;

private:
	FbIdsT GetFbInfoBy( int fb_type ) const;

public:
	int						res_;
	CD_En_FbType			fb_type_;
	FbInfoT					fb_info_;
};

struct CMDR_AuctionItemsInfo : CMDR_MsgBase{
	/*
	游戏处理解密逻辑
	建立一张游戏的表，key值 = dwReqItemById 和 dwReqItemByKey
	recv获取对拍卖物品信息时，首先查表，如果没有，则调用解密函数 GetReqItemAddr 随后加入到表中，游戏针对每一个物品都进行一次解密，之后再次访问到此对象时，则不在解密
	warning：游戏中与注入线程可以同时存在两个表了
	*/
	typedef std::map<__int64, stCD_ReqItem*> ReqItemMapT;

	bool DoDirectImpl(const stCD_MsgR_ReqItemByType* req_item_ptr, int product_item_cnt);
	void DebugInfo() const;
	const stAuctionItemInfo* FindByName( const std::string& role_name );
	int QuerySoldCnt( int item_id );
	const stAuctionItemInfo* FindByLimitPrice( int price_limit );
	void Sort();

	AuctionItemsInfoT		items_info_;
};

//刷新我的拍卖返回值
struct CMDR_OnRefreshMySellList : CMDR_AuctionItemsInfo{
	CMDR_INPUT_IMPL(CMDR_OnRefreshMySellList)
	static const WORD kMsgId = 0x00001512;	// MMOCLIENT_REVERSAL_PACKET_RECV_REQ_ITEM_BY_TYPE_MY___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;
};

//获得拍卖信息返回值
struct CMDR_OnGainAuctionPageInfo : CMDR_AuctionItemsInfo{
	CMDR_OnGainAuctionPageInfo();
	CMDR_INPUT_IMPL(CMDR_OnGainAuctionPageInfo)
	static const WORD kMsgId = 0x00001509;	// MMOCLIENT_REVERSAL_PACKET_RECV_REQ_ITEM_BY_TYPE___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
	int			total_item_cnt_;
	int			cur_page_item_cnt_;		//当前页的拍卖物品数量
};

struct CMDR_OnSafeTime : CMDR_MsgBase{
	CMDR_OnSafeTime();
	CMDR_INPUT_IMPL(CMDR_OnSafeTime)
	static const WORD kMsgId = 0x00001904;	// MMOCLIENT_REVERSAL_AN_QUAN_BAO_HU_TIME___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			left_time_;
};

struct CMDR_OnBuyAuction : CMDR_MsgBase{
	CMDR_OnBuyAuction();
	CMDR_INPUT_IMPL(CMDR_OnBuyAuction)
	static const WORD kMsgId = 0x00001506;	// MMOCLIENT_REVERSAL_PACKET_RECV_REQ_BUY_ITEM_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
};

struct CMDR_OnAcceptMail : CMDR_MsgBase{
	CMDR_OnAcceptMail();
	CMDR_INPUT_IMPL(CMDR_OnAcceptMail)
	static const WORD kMsgId = 0x00001B08;	// MMOCLIENT_REVERSAL_ACCEPT_MAIL_VALUE_RET_MSG_ID___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
	int			error_;
};

struct CMDR_OnOpenNpc : CMDR_MsgBase{
	CMDR_OnOpenNpc();
	CMDR_INPUT_IMPL(CMDR_OnOpenNpc)
	static const WORD kMsgId = 0x00001402;	// MMOCLIENT_REVERSAL_PACKET_RECV_OPEN_NPC_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
};

struct CMDR_OnSoldAuction : CMDR_MsgBase{
	CMDR_OnSoldAuction();
	CMDR_INPUT_IMPL(CMDR_OnSoldAuction)
	static const WORD kMsgId = 0x00001502;	// MMOCLIENT_REVERSAL_PACKET_RECV_SHANG_JIA_ITEM_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
};

struct CMDR_OnSoldItemToShop : CMDR_MsgBase{
	CMDR_OnSoldItemToShop();
	CMDR_INPUT_IMPL(CMDR_OnSoldItemToShop)
	static const WORD kMsgId = 0x00000F04;	// MMOCLIENT_REVERSAL_PACKET_RECV_SELL_ITEM_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
};

struct CMDR_OnCommitTaskFailed : CMDR_MsgBase{
	CMDR_OnCommitTaskFailed();
	CMDR_INPUT_IMPL(CMDR_OnCommitTaskFailed)
	static const WORD kMsgId = 0x00000E55;	// MMOCLIENT_REVERSAL_PACKET_RECV_COMPLETE_TASK_FAIL_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
	int			task_id_;
};

struct CMDR_OnDoNpcChat : CMDR_MsgBase{
	CMDR_OnDoNpcChat();
	CMDR_INPUT_IMPL(CMDR_OnDoNpcChat)
	static const WORD kMsgId = 0x00000E57;	// MMOCLIENT_REVERSAL_PACKET_RECV_NPC_CHAT_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			cur_task_id_;
};

struct CMDR_OnBuffStatus : CMDR_MsgBase{
	CMDR_OnBuffStatus();
	CMDR_INPUT_IMPL(CMDR_OnBuffStatus)
	static const WORD kMsgId = 0x00000405;	// MMOCLIENT_REVERSAL_PACKET_RECV_FEEB_HP_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	//void OnReceived() const override;

	int			iPoisoningFeebHpFlag;
};

//这个用来判断中毒状态的
struct CMDR_OnRoleHpState : CMDR_MsgBase{
	CMDR_OnRoleHpState();
	CMDR_INPUT_IMPL(CMDR_OnRoleHpState)
	static const WORD kMsgId = 0x00000508;	// MMOCLIENT_REVERSAL_PACKET_RECV_ROLE_HP_STATE_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;
	bool IsPoisoning() const;

	int			res_;
	int			role_state_;
};

struct CMDR_OnSendMail : CMDR_MsgBase{
	CMDR_OnSendMail();
	CMDR_INPUT_IMPL(CMDR_OnSendMail)
	static const WORD kMsgId = 0x00001B0A;	// MMOCLIENT_REVERSAL_PACKET_RECV_SEND_MAIL_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;		//290004->角色名不存在
};

struct CMDR_OnSendMail2 : CMDR_MsgBase{
	CMDR_OnSendMail2();
	CMDR_INPUT_IMPL(CMDR_OnSendMail2)
	static const WORD kMsgId = 0x00001B2D;	// MMOCLIENT_REVERSAL_PACKET_RECV_SEND_MAIL_RET2___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;
	//安全时间
	bool IsInSafeTime() const;
	//角色名不存在
	bool NoExistRole() const;
	//发送太频繁
	bool MailIsQuick() const;
	//邮件不合法
	bool MailItemWrong() const;
	//超出每日邮寄限制
	bool MailLimit() const;

	stCD_MsgR_OnSendMail2	res_;
};

//使用后物品还存在着的
struct CMDR_OnUseItemRet1 : CMDR_MsgBase{
	CMDR_OnUseItemRet1();
	CMDR_INPUT_IMPL(CMDR_OnUseItemRet1)
	static const WORD kMsgId = 0x00003002;	// MMOCLIENT_REVERSAL_PACKET_RECV_USE_ITEM_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
};

//物品使用完后会收到这个包
struct CMDR_OnUseItemRet2 : CMDR_MsgBase{
	CMDR_INPUT_IMPL(CMDR_OnUseItemRet2)
	static const WORD kMsgId = 0x00003003;	// MMOCLIENT_REVERSAL_PACKET_RECV_USE_ITEM_RET2___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

};

struct CMDR_OnBuyItem : CMDR_MsgBase{
	CMDR_OnBuyItem();
	CMDR_INPUT_IMPL(CMDR_OnBuyItem)
	static const WORD kMsgId = 0x00000F02;	// MMOCLIENT_REVERSAL_PACKET_RECV_BUY_ITEM_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			res_;
};

struct CMDR_OnCollectRet : CMDR_MsgBase{
	enum enErrorCode{
		kEC_PacketIsFull = 1,	//背包已满
		kEC_CntLimit = 2,		//背包采集物已达上限
	};
	CMDR_OnCollectRet();
	CMDR_INPUT_IMPL(CMDR_OnCollectRet)
	static const WORD kMsgId = 0x00000C02;	// MMOCLIENT_REVERSAL_PACKET_RECV_COLLECT_ITEM_BAG_FULL_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;
	bool IsSucceed() const;
	bool CanNotGainTheCollectItem() const;

	int			res_;
	int			gain_item_id_;
};

struct CMDR_OnArrangeItemsRet : CMDR_MsgBase{
	CMDR_INPUT_IMPL(CMDR_OnArrangeItemsRet)
	static const WORD kMsgId = 0x0000300C;	// MMOCLIENT_REVERSAL_PACKET_RECV_TIDY_INVENTORY_ITEM_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;
};

struct CMDR_OnSendMoveRoomRet : CMDR_MsgBase{
	CMDR_INPUT_IMPL(CMDR_OnSendMoveRoomRet)
	static const WORD kMsgId = 0x0000025A;	// MMOCLIENT_REVERSAL_PACKET_RECV_GUOTU_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	stCD_MsgR_OnSendMoveRoomRet		msg_;
};

struct CMDR_OnAcceptTaskRet : CMDR_MsgBase{
	CMDR_OnAcceptTaskRet();
	CMDR_INPUT_IMPL(CMDR_OnAcceptTaskRet)
	static const WORD kMsgId = 0x0E51;	// MMOCLIENT_REVERSAL_ACCEPT_TASK_MSG_ID___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			task_id_;
};

struct CMDR_OnGiveupTaskRet : CMDR_MsgBase{
	CMDR_OnGiveupTaskRet();
	CMDR_INPUT_IMPL(CMDR_OnGiveupTaskRet)
	static const WORD kMsgId = 0x0E52;	// MMOCLIENT_REVERSAL_GIVE_UP_TASK_MSG_ID___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	int			task_id_;
};

struct CMDR_OnDeleteRole : CMDR_MsgBase{
	enum enErrorCode{
		kEC_Succeed = 0,		//删除成功
		kEC_Protected = 2,		//保护状态
		kEC_Deleting = 5,		//处于删除中，可在protect_time_内恢复角色
	};
	CMDR_OnDeleteRole();
	CMDR_INPUT_IMPL(CMDR_OnDeleteRole)
	static const WORD kMsgId = 0x00000006;	// MMOCLIENT_REVERSAL_PACKET_RECV_MSG_DELETE_PLAYER_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	enErrorCode		res_;
	int				protect_time_;			//单位秒
};

struct CMDR_OnRequestDeal : CMDR_MsgBase{
	CMDR_OnRequestDeal();
	CMDR_INPUT_IMPL(CMDR_OnRequestDeal)
	static const WORD kMsgId = 0x00001105;	// MMOCLIENT_REVERSAL_PACKET_RECV_REQUEST_DEAL_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	stCD_MsgR_OnRequestDeal			deal_info_;
};

struct CMDR_OnRequestDealFail : CMDR_MsgBase{
	enum enErrorCode{
		kEC_Succeed = 0,
		kEC_Refuse = 3035,		//交易被拒绝
	};

	CMDR_OnRequestDealFail();
	CMDR_INPUT_IMPL(CMDR_OnRequestDealFail)
	static const WORD kMsgId = 0x00000105;	// MMOCLIENT_REVERSAL_PACKET_RECV_REQUEST_DEAL_FIAL_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	std::string			the_role_name_;			//被请求玩家名
	enErrorCode			res_;
};

struct CMDR_OnRecvQuestDeal : CMDR_MsgBase{
	CMDR_OnRecvQuestDeal();
	CMDR_INPUT_IMPL(CMDR_OnRecvQuestDeal)
	static const WORD kMsgId = 0x00001901;	// MMOCLIENT_REVERSAL_PACKET_RECV_ACCEPT_DEAL_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	DWORD			deal_key_;
	DateTimeLocal	recv_time_;
};

struct CMDR_OnCancelDeal : CMDR_MsgBase{
	CMDR_OnCancelDeal();
	CMDR_INPUT_IMPL(CMDR_OnCancelDeal)
	static const WORD kMsgId = 0x00001113;	// MMOCLIENT_REVERSAL_PACKET_RECV_QUXIAO_DEAL_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	DWORD			role_id_of_cancel_;		// 谁取消，就是谁的id，如果为0，则表示交易失败
};

// 双方都确认交易后的返回值 交易失败 物品超出携带上限 对方已经在交易中
struct CMDR_OnDealFail : CMDR_MsgBase{
	enum enErrorCode{
		//kEC_Succeed = 0,
		kEC_ItemCntLimit = 0x0029001E,			//物品超出携带上限
		kEC_DstDealing = 0x0029000B,			//对方已经在交易中
	};
	CMDR_OnDealFail();
	CMDR_INPUT_IMPL(CMDR_OnDealFail)
	static const WORD kMsgId = 0x00001101;	// MMOCLIENT_REVERSAL_PACKET_RECV_DEAL_FAIL_RET1___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	enErrorCode		res_;
	DWORD			dst_role_id_;
};

struct CMDR_OnMoveItemToDealTab : CMDR_MsgBase{
	CMDR_OnMoveItemToDealTab();
	CMDR_INPUT_IMPL(CMDR_OnMoveItemToDealTab)
	static const WORD kMsgId = 0x00001107;	// MMOCLIENT_REVERSAL_PACKET_RECV_MOVE_DEAL_ITEM_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	stCD_MsgR_OnMoveItemToDealTab		the_move_item_;
};

struct CMDR_OnMoveDealGold : CMDR_MsgBase{
	CMDR_OnMoveDealGold();
	CMDR_INPUT_IMPL(CMDR_OnMoveDealGold)
	static const WORD kMsgId = 0x0000110D;	// MMOCLIENT_REVERSAL_PACKET_RECV_MOVE_DEAL_GOLD_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	stCD_MsgR_OnMoveDealGold		deal_info_;
};

// 交易状态返回值 ==》 锁定交易栏的返回值 对方锁定交易栏的返回值 对方确认交易的返回值认交易的返回值
struct CMDR_OnRecvDealStatus : CMDR_MsgBase{
	enum enOperator{
		kO_CancelLock = 1,		//表示取消锁定
		kO_LockDeal	= 2,		//锁定
		kO_Enter = 3,			//确认交易
	};
	CMDR_OnRecvDealStatus();
	CMDR_INPUT_IMPL(CMDR_OnRecvDealStatus)
	static const WORD kMsgId = 0x00001111;	// MMOCLIENT_REVERSAL_PACKET_RECV_DEAL_STATUS_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	DWORD			the_deal_role_id_;
	enOperator		deal_oper_;
};

// 双方都确认交易后的返回值 交易结束
struct CMDR_OnRecvDealEnd : CMDR_MsgBase{
	enum enErrorCode{
		//kEC_Succeed = 3208704,		//交易成功
	};
	CMDR_OnRecvDealEnd();
	CMDR_INPUT_IMPL(CMDR_OnRecvDealEnd)
	static const WORD kMsgId = 0x00001114;	// MMOCLIENT_REVERSAL_PACKET_RECV_DEAL_SUCESS_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	enErrorCode		res_;
};

struct CMDR_OnGetMailItemsInfo : CMDR_MsgBase{
	enum enErrorCode{
	};
	//CMDR_OnGetMailItemsInfo();
	CMDR_INPUT_IMPL(CMDR_OnGetMailItemsInfo)
	static const WORD kMsgId = 0x00001B06;	// MMOCLIENT_REVERSAL_PACKET_RECV_EMIAL_INFO_RET___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;

	//enErrorCode		res_;
};
//////////////////////////////////////////////////////////////////////////
struct CMDR_Hook_Hit : CMDR_MsgBase{
	CMDR_INPUT_IMPL(CMDR_Hook_Hit)
	static const WORD kMsgId = 0x000002C1;	// MMOCLIENT_REVERSAL_PACKET_MSG_ID_HIT___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
};

struct CMDR_Hook_SendMoveRoom : CMDR_MsgBase{
	CMDR_Hook_SendMoveRoom();
	CMDR_INPUT_IMPL(CMDR_Hook_SendMoveRoom)
	static const WORD kMsgId = 0x0000025B;	// MMOCLIENT_REVERSAL_PACKET_MSG_ID_GUO_TU___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	const char* NewRetAddr() const override;
	static bool IsSendMoveRoolMsgSame( const stCD_Msg_MoveRoom& lhs, const stCD_Msg_MoveRoom& rhs );

	stCD_Msg_MoveRoom		msg_;
	DateTimeLocal			msg_time_;
	bool					offset_valid_;
};

struct CMDR_Hook_CreateRole : CMDR_MsgBase{
	CMDR_INPUT_IMPL(CMDR_Hook_CreateRole)
	static const WORD kMsgId = 0x00000003;	// MMOCLIENT_REVERSAL_PACKET_MSG_ID_CREATE_ROLE___;
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	void OnReceived() const override;
};

struct CMDR_Hook_CancelDeal : CMDR_MsgBase{
	CMDR_INPUT_IMPL(CMDR_Hook_CancelDeal)
	enum{
		kMsgId = stCD_Msg_AcceptDeal::kMsgId
	};
	bool DoDirect(const CMDR_HeaderBase* msg) override;
	const char* NewRetAddr() const override;

	bool		is_ignore_ = false;
};
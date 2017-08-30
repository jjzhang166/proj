#pragma once
/*
@author zhp
@date 2014/8/19 15:44
@purpose for recv/read
*/
#include "GProtocolBase.h"
#include "GameArray.h"
#include <memory>
#include <Communicate/CG_Defines.h>
#include <boost/optional.hpp>

struct CMDR_HeaderBase : CMD_HeaderBase{
protected:
	CMDR_HeaderBase();

public:
	WORD		hash_uri_;			//消息号

	const void* GetRealMsgAddr() const{
		static_assert(sizeof(*this) == 6, "");
		return (char*)this + sizeof(*this);
	}
	WORD GetRealMsgSize() const{
		assert(total_length_ >= sizeof(*this));
		return total_length_ - sizeof(*this);
	}
};
static_assert(sizeof(CMDR_HeaderBase) == sizeof(PACKET_HEADER), "");

typedef stBStreamInput CMDR_InBase;
struct CMDR_MsgBase : CMDR_InBase, std::enable_shared_from_this<CMDR_MsgBase>{
public:
	typedef std::shared_ptr<CMDR_MsgBase> SelfPtr;
	using CMDR_InBase::Read;

public:
	B_STREAM_INPUT(CMDR_MsgBase)
	virtual CMDR_MsgBase* Clone() const;
	virtual void OnReceived() const;
	virtual boost::optional<WORD> NewRetOffset() const;
};
#define CMDR_INPUT_IMPL(DerivedT) B_STREAM_INPUT(DerivedT)	\
	DerivedT* Clone() const{	\
	return new DerivedT;	\
}	\
/**/

//选择频道时取值发包
struct CMDR_AllowSwitchChannel : CMDR_MsgBase{
	struct stServerInfo : CMDR_InBase{
		stServerInfo();
		bool Read( StrInputBStreamBase& istm );
		B_STREAM_INPUT(stServerInfo)

		WORD		allm_net_address_;
		std::string	str_ip_;
		WORD		port_;
	};

	CMDR_AllowSwitchChannel();
	bool Read( StrInputBStreamBase& istm );
	CMDR_INPUT_IMPL(CMDR_AllowSwitchChannel)

	DWORD						unknown_result_;
	std::wstring				channel_id_;
	std::wstring				account_id_;
	std::wstring				session_key_;
	WORD						stage_server_addresses_;
	GameArrayT<stServerInfo>	server_info_;
	std::wstring				prev_channel_id_;
};

//频道链表
struct CMDR_ChannelList : CMDR_MsgBase{
	struct stChannelInfo : CMDR_InBase{
		stChannelInfo();
		bool Read( StrInputBStreamBase& istm );
		B_STREAM_INPUT(stChannelInfo)
		//空闲权重
		int CalcFreeWeight() const;

		WORD			wApple_ChannelElementForClient;
		DWORD			dwChannelNumber;
		std::wstring	channel_id_;					// 频道id 如： "channel_03_01"
		std::wstring	title_;						// 标题 如：频道1
		bool			enabled_;						// 激活状态  1为可登陆  0为不可登陆
		int				capacity_;						// 可进入频道最大人数
		int				player_count_;					// 实际玩家人数
		DWORD			dwMinPvpPoint;
		DWORD			dwMaxPvpPoint;
		DWORD			dwChannelPropertyId;
	};

	CMDR_ChannelList();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_ChannelList)

	WORD						channels_;
	GameArrayT<stChannelInfo>	channels_info_;;
};

//选择角色场景，角色信息
struct CMDR_PartialListPlayersStaticInfo : CMDR_MsgBase{
	CMDR_PartialListPlayersStaticInfo();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	bool ConvertToRoleInfo( CG_RoleInfo& role_info ) const;
	CMDR_INPUT_IMPL(CMDR_PartialListPlayersStaticInfo)

	std::wstring				account_id_;
	WORD						wStaticInfo;
	WORD						wApple_StaticPlayerInfo;
	std::wstring				player_name_;			// 玩家姓名
	LONGLONG					lPlayerSerial;
	std::wstring				strPlayerGender;		// 玩家性别
	std::wstring				player_job_name_;			// 职业类型
	WORD						wCreatedDateTime;
	WORD						wApple_DateTime;
	WORD						wYear;
	WORD						wMonth;
	WORD						wDay;
	WORD						wHour;
	DWORD						dwMinute;
	DWORD						dwSecond;
	DWORD						dwMilliSec;
	bool						bIsInfinity;
	bool						is_deleted_;				// 角色是否在删除状态  00 正常状态， 01 删除状态
};

//结束发送角色信息的封包
struct CMDR_EndPartialListPlayers : CMDR_MsgBase{
	CMDR_EndPartialListPlayers();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_EndPartialListPlayers)

	std::wstring	account_id_;
	std::wstring	selected_player_id_;
	DWORD			dwConnectionSerial;
	DWORD			dwSendingType;
};

//响应创建角色
struct CMDR_OnCreatePlayer : CMDR_MsgBase{
	enum enResult{
		kSucceed = 0,
		kWordProhibit = 1,			//含有违禁字符
		kNameIsUsed = 6,			//角色名已被使用
	};
	CMDR_OnCreatePlayer();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnCreatePlayer)

	enResult			result_;
};

//响应邮件
struct CMDR_OnMailList : CMDR_MsgBase{
	struct stMailItem : CMDR_InBase{
		stMailItem();
		bool Read( StrInputBStreamBase& istm );
		B_STREAM_INPUT(stMailItem)

		LONGLONG			mail_serial_;
		std::wstring		title_;
		std::wstring		sender_;
		LONGLONG			gold_;
		std::wstring		sample_item_id_;
		std::wstring		mail_type_;
	};

	CMDR_OnMailList();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnMailList)

	DWORD					result_;
	GameArrayT<stMailItem>	mail_items_;
};

//接受邮件物品后，更新此邮件内容
struct CMDR_RecvMailContents : CMDR_MsgBase{
	CMDR_RecvMailContents();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_RecvMailContents)

	int				result_;
	LONGLONG		mail_serial_;
	int				mail_item_cnt_;
};

//响应get attachments
struct CMDR_OnGetAttachments : CMDR_MsgBase{
	CMDR_OnGetAttachments();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnGetAttachments)

	int				result_;
	LONGLONG		mail_serial_;
};

//响应我的拍卖列表
struct CMDR_OnMyProductList : CMDR_MsgBase{
	struct stMyProduct : CMDR_InBase{
		stMyProduct();
		bool Read( StrInputBStreamBase& istm );
		B_STREAM_INPUT(stMyProduct)

		LONGLONG			oid_market_;
		std::wstring		item_id_;
		int					item_cnt_;
		int					item_grade_;		//需要物品品质与物品等级方面的信息，这个不确定是否是物品品质的信息
		LONGLONG			price_;
	};

	CMDR_OnMyProductList();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnMyProductList)

	GameArrayT<stMyProduct>		my_products_;
	int							result_;
};

//响应挂拍卖
struct CMDR_OnRegisterProduct : CMDR_MsgBase{
public:
	enum enRegisterProductResult{
		kRPR_Succeed,
		kRPR_PerAcLimit,
		kRPR_PerRoleLimit,
		kRPR_CanNotMoveItem,
		kRPR_Invalid,
	};

public:
	CMDR_OnRegisterProduct();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnRegisterProduct)

	enRegisterProductResult		result_;
};

//响应买拍卖物品
struct CMDR_OnBuyProduct : CMDR_MsgBase{
	CMDR_OnBuyProduct();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnBuyProduct)

	int		result_;
};

//响应拍卖物品信息
struct CMDR_OnRequestProductInfo : CMDR_MsgBase{
	CMDR_OnRequestProductInfo();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnRequestProductInfo)

	int			result_;
	LONGLONG	average_price_;			//最近四周的
	int			trade_count_;			//最近交易次数
	LONGLONG	lower_price_;
};

struct CMDR_OnRequestProductList : CMDR_MsgBase{
	struct stProductItem : CMDR_InBase{
		stProductItem();
		bool Read( StrInputBStreamBase& istm );
		B_STREAM_INPUT(stProductItem)

		LONGLONG			oid_market_;
		std::wstring		item_id_;
		std::wstring		item_ch_name_;
		int					item_cnt_;
		int					item_grade_;		//需要物品品质与物品等级方面的信息，这个不确定是否是物品品质的信息
		LONGLONG			price_;
	};

	CMDR_OnRequestProductList();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnRequestProductList)

	GameArrayT<stProductItem>	product_items_;
	int							result_;
};

//响应登陆结果
struct CMDR_OnLoginResult : CMDR_MsgBase{
	enum enResult{
		kSucceed = 0,
		kBanAc = 3,			//封号
	};
	CMDR_OnLoginResult();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnLoginResult)

	int							result_;
};

//卖物品响应结果
struct CMDR_OnSellItemResult : CMDR_MsgBase{
	enum enResult{
		kSucceed = 0,
		kRestrict = 6,
	};
	CMDR_OnSellItemResult();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnSellItemResult)

	enResult					result_;
};

//响应交任务
struct CMDR_OnAcceptQuestResult : CMDR_MsgBase{
	enum enResult{
		kSucceed = 0,
		kRestrict = 0x12,
	};
	CMDR_OnAcceptQuestResult();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnAcceptQuestResult)

	enResult					result_;
};

//进入副本结果
struct CMDR_OnStartStageResult : CMDR_MsgBase{
	enum enResult{
		kSucceed = 0,
		kLimitCnt = 6,		//已超出入场次数上限
		kRestrict = 0x0f,	//被限制进入副本
	};
	CMDR_OnStartStageResult();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnStartStageResult)

	struct stPlayersInfo : CMDR_InBase{
		bool Read( StrInputBStreamBase& istm );
		B_STREAM_INPUT(stPlayersInfo)

		std::wstring	player_id_;
	};

	GameArrayT_<stPlayersInfo>	players_info_;
	enResult					result_;
};

//hook change village
struct CMDR_OnChangeVillage : CMDR_MsgBase{
	enum enResult{
		kSucceed = 0,
		kRestrict = 1,		//被限制进入副本
	};
	CMDR_OnChangeVillage();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnChangeVillage)

	enResult					result_;
};

//create NPC
struct CMDR_OnCreateNPC : CMDR_MsgBase{
	CMDR_OnCreateNPC();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnCreateNPC)

	std::wstring			obj_id_;
	std::wstring			team_id_;
	bool					is_dummy_;
	bool					has_valid_data_;
	DWORD					valid_data_x_;
	DWORD					valid_data_y_;
};

struct CMDR_OnCreateNpcByClient : CMDR_MsgBase{
	CMDR_OnCreateNpcByClient();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnCreateNpcByClient)

	DWORD				task_npc_serial_;
};

struct CMDR_OnPlayerDead : CMDR_MsgBase{
	CMDR_OnPlayerDead();
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_OnPlayerDead)

	DWORD				player_serial_;
};
//////////////////////////////////////////////////////////////////////////
//*************************hook send msg**********************************
//hook每进入个副本都会发的包
struct CMDR_Hook_EnteredCloneMap : CMDR_MsgBase{
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_Hook_EnteredCloneMap)
};

//响应选择角色
struct CMDR_Hook_SelectPlayer : CMDR_MsgBase{
	bool Read( StrInputBStreamBase& istm );
	void OnReceived() const;
	CMDR_INPUT_IMPL(CMDR_Hook_SelectPlayer)
};

//hook游戏发送选择频道的封包
struct CMDR_Hook_RequestSwitchChannel : CMDR_MsgBase{
	bool Read( StrInputBStreamBase& istm );
	boost::optional<WORD> NewRetOffset() const;
	CMDR_INPUT_IMPL(CMDR_Hook_RequestSwitchChannel)
};

//hook攻击封包
struct CMDR_Hook_RequestProjectileHit : CMDR_MsgBase{
	bool Read( StrInputBStreamBase& istm );
	CMDR_INPUT_IMPL(CMDR_Hook_RequestProjectileHit)
};

//hook add buff
struct CMDR_Hook_RequestAddBuff : CMDR_MsgBase{
	bool Read( StrInputBStreamBase& istm );
	boost::optional<WORD> NewRetOffset() const;
	CMDR_INPUT_IMPL(CMDR_Hook_RequestAddBuff)
};

//hook change hp
struct CMDR_Hook_RequestChangeHp : CMDR_MsgBase{
	bool Read( StrInputBStreamBase& istm );
	CMDR_INPUT_IMPL(CMDR_Hook_RequestChangeHp)
};

//change sector
struct CMDR_Hook_ChangeSector : CMDR_MsgBase{
	bool Read( StrInputBStreamBase& istm );
	boost::optional<WORD> NewRetOffset() const;
	CMDR_INPUT_IMPL(CMDR_Hook_ChangeSector)
};
//////////////////////////////////////////////////////////////////////////
#pragma once
/*
@author zhp
@date 2014/6/15 15:10
@purpose console.exe与game.dll之间的交互
*/
#include <Protocol/P_Defines.h>
#include <Common/JsonMore.h>
#include <boost/uuid/uuid.hpp>

//控制台发给game模块一个固定的字符串
const char* const kStableStr_ConsoleToGame =
#ifdef __PROJ_GWLR_MB_
	/*kStableStr_ConsoleToGame_mb*/"2e13cf97-253f-4b63-9395-c442e0848519";
#elif (__PROJ_GWLR_XE_)
	/*kStableStr_ConsoleToGame_xe*/"a71ae380-1092-4184-8796-ba42085ff981";
#elif (__PROJ_GWLR_MBP_)
	/*kStableStr_ConsoleToGame_mbp*/"4fa4d2b6-8232-4012-a20c-1fb3f502ac04";
#endif
//不开启最短寻路
//#define PREP_UNOPEN_SHORTEST_PATH

//控制台发给game模块，用来效验证公钥有效性
//const char* const kCheckPubKey_ConsoleToGame = "310279A0293B4EF0B99B9AADFC554592";
//funcobj normal
const P_ObjId kObjIdNormal = DEF_THE_CUSTOM_TEMPLATE_OBJ_ID_(33);

//角色信息
struct CG_RoleInfo{
	typedef std::vector<CG_RoleInfo> RoleInfoContT;
	bool ParseFromJosn( const Json::Value& json_v );
	bool ConvertToJson( Json::Value& json_v );
	
	std::string		role_job_;
	std::string		role_name_;
};
//角色数量上限
const int kCG_RoleCntLimit = 6;

//////////////////////////////////////////////////////////////////////////
enum enGameAcType{
	kGAT_Normal = 0,
	kGAT_Stored,
	kGAT_Invalid,
};

enum enDstServerType{
	kDST_Normal = 1,
	kDST_Invalid,
};

//倍攻
const int kMultiHitRate_Min = 1;
const int kMultiHitRate_Max = 50;

//static addr data
//const unsigned int kStaticAddrData = 0x9842a;

//服务器上接收的关键数据，加上这个值才是真正有效的
const int kAddrDatasAdditional = 10;

struct stAuctionItemInfo{
	DWORD			req_item_id_;
	DWORD			req_item_key_;
	std::string		req_item_by_player_name_;
	int				item_id_;
	int				item_cnt_;
	int				begin_price_;		//起拍价
	int				smart_price_;		//一口价
	int				jing_pai_price_;	//竞拍价

	mutable boost::uuids::uuid		item_info_uuid_;

	stAuctionItemInfo();
	friend bool operator == ( const stAuctionItemInfo& lhs, const stAuctionItemInfo& rhs );
	friend bool operator < ( const stAuctionItemInfo& lhs, const stAuctionItemInfo& rhs );
	bool ParseFromJsonObj( const Json::Value& j_value );
	bool ConvertToJson( Json::Value& j_value ) const;
	void DebugInfo() const;
	const boost::uuids::uuid& GetItemInfoUuid() const;
};
typedef std::vector<stAuctionItemInfo> AuctionItemsInfoT;

struct stAuctionItemInfoEx : stAuctionItemInfo{
	stAuctionItemInfoEx();
	explicit stAuctionItemInfoEx( const stAuctionItemInfo& rhs );
	bool ParseFromJsonObjEx( const Json::Value& j_value );
	bool ConvertToJsonEx( Json::Value& j_value ) const;

	std::string			item_name_;
};
typedef std::vector<stAuctionItemInfoEx> AuctionItemsInfoExT;

P_CoorTypeT MakeTransRouteType( enGameAcType game_ac_type, int game_ser_id );

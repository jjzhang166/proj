#pragma once
/*
@author zhp
@date 2014/5/11 13:49
@purpose for test, players info
*/
#include "stdafx.h"
#include "Facility/GPlayer.h"
#include <Common/UsefulClass.h>
#include <Common/UiCtrls.h>

//球员列表
enum enListPlayers_SubItems{
	kLPSI_Id = 0,			//球员ID
	kLPSI_PlayerName,		//球员名
	kLPSI_Grade,			//球员等级
	kLPSI_Price,			//球员价值
	kLPSI_Life,				//剩余场次
	kLPSI_Pos,				//球员职位
	kLPSI_Spid,				//球员spid
};
//物品列表
enum enListItems_SubItems{
	kLISI_Id = 0,			//物品id
	//kLISI_ItemId,			//物品item_id
	kLISI_Remains,			//物品数量
	kLISI_Expired,			//是否到期
};

//球员信息
class PlayerInfoS : public GPlayer::stPlayer{
public:
	bool ParseFromJsonObj( const Json::Value& player_obj );

public:
	std::string				name_;
};

//物品信息
class ItemInfoS : public GPlayer::stItem{
public:
	bool ParseFromJsonObj( const Json::Value& item_obj );
};

class CList_Players : public CListCtrlStyled{
protected:
	virtual void PreSubclassWindow();
};

class CList_PlayersSingle : public CList_Players{
public:
	//添加一个球员
	void AddPlayer( const PlayerInfoS& player );
};

class CList_Items : public CListCtrlStyled{
protected:
	virtual void PreSubclassWindow();
};

class CList_ItemsSingle : public CList_Items{
public:
	//添加一个物品信息
	void AddItemInfo( const ItemInfoS& item_info );
};

//我的出售列表
class CList_MyOfferList : public CListCtrlStyled, public Singleton<CList_MyOfferList, Singleton_Instance>{
	//type definitions
public:
	enum enColumns{
		kPrice = 0,			//价钱
#ifdef _DEBUG
		kPlayerId,
		kId,
		kSpid,
#endif
		kSoldState,					
		kIssuedAt,			//拍卖时间
		kGrade,				//强化等级
	};
	struct stSelfOfferItem : public GPlayer::stSelfOfferItem{
		bool ParseFromJsonObj( const Json::Value& sold_player );
	};

public:
	void AddOfferItem( const stSelfOfferItem& offer_item );

protected:
	virtual void PreSubclassWindow();
};

//其他号的出售项
class CList_OtherOffers : public CListCtrlStyled, public Singleton<CList_OtherOffers, Singleton_Instance>{
	//type definitions
public:
	enum enColumns{
		kPrice = 0,			//价钱
#ifdef _DEBUG
		kId,
		kSpid,
#endif
		kIssuedAt,			//拍卖时间
		kGrade,				//强化等级
	};
	struct stOtherOfferItem : public GPlayer::stOfferItem{
		bool ParseFromJsonObj( const Json::Value& sold_player );
	};

public:
	void AddOfferItem( const stOtherOfferItem& offer_item );

protected:
	virtual void PreSubclassWindow();
};
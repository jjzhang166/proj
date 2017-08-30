#include "stdafx.h"
#include "ListCtrl_Players.h"

void CList_Players::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 175;
	InsertColumn(kLPSI_Id, _T("球员ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLPSI_PlayerName, _T("球员名"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLPSI_Grade, _T("球员等级"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLPSI_Price, _T("球员价值"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLPSI_Life, _T("剩余场次"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLPSI_Pos, _T("球员职位"), LVCFMT_LEFT, kColumnWidth);
	//InsertColumn(kLPSI_Spid, _T("球员spid"), LVCFMT_LEFT, kColumnWidth);
}

bool PlayerInfoS::ParseFromJsonObj( const Json::Value& player_obj )
{
	const auto& id = player_obj["_id"];
	if ( id.isNull() || !id.isString() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& grade = player_obj["grade"];
	if ( grade.isNull() || !grade.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& price = player_obj["price"];
	if ( price.isNull() || !price.isNumeric() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& life = player_obj["life"];
	if ( life.isNull() || !life.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& pos = player_obj["position"];
	if ( pos.isNull() || !pos.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& spid = player_obj["spid"];
	if ( spid.isNull() || !spid.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& player_name = player_obj["name"];
	if ( player_name.isNull() || !player_name.isString() )
	{
		goto LABEL_RET_ERROR;
	}

	id_ = id.asCString();
	grade_ = grade.asInt();
	price_ = price.asDouble();
	life_ = life.asInt();
	spid_ = spid.asInt();
	name_ = player_name.asCString();
	player_pos_ = (GType::enPlayerPosition)pos.asInt();

	return true;
LABEL_RET_ERROR:
	return false;
}

void CList_PlayersSingle::AddPlayer( const PlayerInfoS& player )
{
	const auto kItemMax = GetItemCount();
	CString str_tmp;
	str_tmp.Format(_T("%d"), player.spid_);
	InsertItem(kItemMax, str_tmp);
	SetItemText(kItemMax, kLPSI_PlayerName, CA2CT(player.name_.c_str()));
	str_tmp.Format(_T("%d"), player.grade_);
	SetItemText(kItemMax, kLPSI_Grade, str_tmp);
	str_tmp.Format(_T("%d"), player.price_);
	SetItemText(kItemMax, kLPSI_Price, str_tmp);
	str_tmp.Format(_T("%d"), player.life_);
	SetItemText(kItemMax, kLPSI_Life, str_tmp);
	/*str_tmp.Format(_T("%d"), player.spid_);
	SetItemText(kItemMax, kLPSI_Spid, str_tmp);*/

	using namespace GType;
	switch (player.player_pos_)
	{
	case enPlayerPosition_No:
		str_tmp = _T("无职位");
		break;
	case enPlayerPosition_Third:
		str_tmp = _T("候补");
		break;
	case enPlayerPosition_Second:
		str_tmp = _T("替补");
		break;
	case enPlayerPosition_First:
		str_tmp = _T("首发");
		break;
	default:
		str_tmp = _T("错误");
	}
	SetItemText(kItemMax, kLPSI_Pos, str_tmp);
}

bool ItemInfoS::ParseFromJsonObj( const Json::Value& item_obj )
{
	const auto& id = item_obj["id"];
	if ( id.isNull() || !id.isString() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& item_id = item_obj["itemid"];
	if ( item_id.isNull() || !item_id.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& remains = item_obj["remains"];
	if ( remains.isNull() || !remains.isInt() )
	{
		goto LABEL_RET_ERROR;
	}

	const auto& expired = item_obj["expired"];
	if ( expired.isNull() || !expired.isBool() )
	{
		goto LABEL_RET_ERROR;
	}

	id_ = id.asCString();
	itemid_ = item_id.asInt();
	remains_ = remains.asInt();
	expired_ = expired.asBool();

	return true;
LABEL_RET_ERROR:
	return false;
}

void CList_Items::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 225;
	InsertColumn(kLISI_Id, _T("物品ID"), LVCFMT_LEFT, kColumnWidth);
	//InsertColumn(kLISI_ItemId, _T("物品item_id"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLISI_Remains, _T("物品数量"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kLISI_Expired, _T("是否到期"), LVCFMT_LEFT, kColumnWidth);
}

void CList_ItemsSingle::AddItemInfo( const ItemInfoS& item_info )
{
	const auto kItemMax = GetItemCount();
	CString str_tmp;
	str_tmp.Format(_T("%d"), item_info.itemid_);
	InsertItem(kItemMax, str_tmp);
	str_tmp.Format(_T("%d"), item_info.remains_);
	SetItemText(kItemMax, kLISI_Remains, str_tmp);
	if (item_info.expired_)
		str_tmp = _T("是");
	else
		str_tmp = _T("否");
	SetItemText(kItemMax, kLISI_Expired, str_tmp);
}

void CList_MyOfferList::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 175;
	InsertColumn(kPrice, _T("价钱"), LVCFMT_LEFT, kColumnWidth);
#ifdef _DEBUG
	InsertColumn(kPlayerId, _T("球员ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kId, _T("拍卖的ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSpid, _T("spid"), LVCFMT_LEFT, kColumnWidth);
#endif
	InsertColumn(kSoldState, _T("拍卖状态"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kIssuedAt, _T("拍卖时间"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kGrade, _T("强化等级"), LVCFMT_LEFT, kColumnWidth);
}

void CList_MyOfferList::AddOfferItem( const stSelfOfferItem& offer_item )
{
	const auto kItemMax = GetItemCount();
	CString str_tmp;
	str_tmp.Format(_T("%lf"), offer_item.price_);
	InsertItem(kItemMax, str_tmp);
#ifdef _DEBUG
	SetItemText(kItemMax, kPlayerId, CA2CT(offer_item.player_id_.c_str()));
	SetItemText(kItemMax, kId, CA2CT(offer_item.id_.c_str()));
	str_tmp.Format(_T("%d"), offer_item.spid_);
	SetItemText(kItemMax, kSpid, str_tmp);
#endif
	if ( stSelfOfferItem::kSS_Expired == offer_item.sold_state_ )
		str_tmp = _T("到期");
	else if ( stSelfOfferItem::kSS_Sold == offer_item.sold_state_ )
		str_tmp = _T("已卖掉");
	else if ( stSelfOfferItem::kSS_Waiting == offer_item.sold_state_ )
		str_tmp = _T("等待卖掉");
	SetItemText(kItemMax, kSoldState, str_tmp);
	auto str_issued_at = Poco::DateTimeFormatter::format(offer_item.issued_at_, Poco::DateTimeFormat::SORTABLE_FORMAT);
	SetItemText(kItemMax, kIssuedAt, CA2CT(str_issued_at.c_str()));
	str_tmp.Format(_T("%d"), offer_item.grade_);
	SetItemText(kItemMax, kGrade, str_tmp);
}

void CList_OtherOffers::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 225;
	InsertColumn(kPrice, _T("价钱"), LVCFMT_LEFT, kColumnWidth);
#ifdef _DEBUG
	InsertColumn(kId, _T("拍卖的ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSpid, _T("spid"), LVCFMT_LEFT, kColumnWidth);
#endif
	InsertColumn(kIssuedAt, _T("拍卖时间"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kGrade, _T("强化等级"), LVCFMT_LEFT, kColumnWidth);
}

void CList_OtherOffers::AddOfferItem( const stOtherOfferItem& offer_item )
{
	const auto kItemMax = GetItemCount();
	CString str_tmp;	
	str_tmp.Format(_T("%lf"), offer_item.price_);
	InsertItem(kItemMax, str_tmp);
#ifdef _DEBUG
	SetItemText(kItemMax, kId, CA2CT(offer_item.id_.c_str()));
	str_tmp.Format(_T("%d"), offer_item.spid_);
	SetItemText(kItemMax, kSpid, str_tmp);
#endif
	auto str_issued_at = Poco::DateTimeFormatter::format(offer_item.issued_at_, Poco::DateTimeFormat::SORTABLE_FORMAT);
	SetItemText(kItemMax, kIssuedAt, CA2CT(str_issued_at.c_str()));
	str_tmp.Format(_T("%d"), offer_item.grade_);
	SetItemText(kItemMax, kGrade, str_tmp);
}

bool CList_OtherOffers::stOtherOfferItem::ParseFromJsonObj( const Json::Value& sold_player )
{
	const auto& id = sold_player["_id"];
	if ( id.isNull() || !id.isString() )
		return false;
	const auto& price = sold_player["price"];
	if ( price.isNull() || !price.isNumeric() )
		return false;

	const auto& spid = sold_player["spid"];
	if ( spid.isNull() || !spid.isInt() )
		return false;

	const auto& issued_at = sold_player["issuedat"];
	if ( issued_at.isNull() || !issued_at.isString() )
		return false;

	const auto& grade = sold_player["grade"];
	if ( grade.isNull() || !grade.isInt() )
		return false;

	id_ = id.asCString();
	int time_zone = 0;
	issued_at_ = Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, issued_at.asString(), time_zone);
	issued_at_.makeLocal(GetTzd());
	spid_ = spid.asInt();
	price_ = price.asDouble();
	grade_ = grade.asInt();
	return true;	
}

bool CList_MyOfferList::stSelfOfferItem::ParseFromJsonObj( const Json::Value& sold_player )
{
	const auto& price = sold_player["price"];
	if ( price.isNull() || !price.isNumeric() )
		return false;

	const auto& spid = sold_player["spid"];
	if ( spid.isNull() || !spid.isInt() )
		return false;

	const auto& issued_at = sold_player["issuedat"];
	if ( issued_at.isNull() || !issued_at.isString() )
		return false;

	const auto& grade = sold_player["grade"];
	if ( grade.isNull() || !grade.isInt() )
		return false;
	const auto& id = sold_player["_id"];
	if ( id.isNull() || !id.isString() )
		return false;

	const auto& player_id = sold_player["playerid"];
	if ( player_id.isNull() || !player_id.isString() )
		return false;

	const auto& state = sold_player["status"];
	if ( state.isNull() || !state.isString() )
		return false;
	const auto& str_state = state.asString();
	if ( str_state == "sold" )
		sold_state_ = kSS_Sold;
	else if ( str_state == "waiting" )
		sold_state_ = kSS_Waiting;
	else if ( str_state == "expired" )
		sold_state_ = kSS_Expired;
	else 
	{
		assert(false);
		return false;
	}

	int time_zone = 0;
	issued_at_ = Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, issued_at.asString(), time_zone);
	issued_at_.makeLocal(GetTzd());
	spid_ = spid.asInt();
	price_ = price.asDouble();
	grade_ = grade.asInt();
	id_ = id.asCString();
	player_id_ = player_id.asCString();
	return true;
}

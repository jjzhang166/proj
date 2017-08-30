#include "stdafx.h"
#include "CG_Defines.h"
#include <Common/LogOut.h>
#include <Common/UsefulFunc.h>

bool CG_RoleInfo::ParseFromJosn( const Json::Value& json_v )
{
	const auto& json_job = json_v["job"];
	if ( json_job.isNull() || !json_job.isString() )
	{
		assert(false);
		return false;
	}
	const auto& json_name = json_v["name"];
	if ( json_name.isNull() || !json_name.isString() )
	{
		assert(false);
		return false;
	}
	role_job_ = json_job.asString();
	role_name_ = json_name.asString();
	return true;
}

bool CG_RoleInfo::ConvertToJson( Json::Value& json_v )
{
	json_v["job"] = role_job_;
	json_v["name"] = role_name_;
	return true;
}

stAuctionItemInfo::stAuctionItemInfo()
{
	ZeroMemory(&item_info_uuid_, sizeof(item_info_uuid_));
	req_item_id_ = 0;
	req_item_key_ = 0;
	item_id_ = 0;
	item_cnt_ = 0;
	begin_price_ = 0;
	smart_price_ = 0;
	jing_pai_price_ = 0;
}

bool stAuctionItemInfo::ParseFromJsonObj( const Json::Value& j_value )
{
	const auto& req_item_id = j_value["rqid"];
	if ( req_item_id.isNull() || !req_item_id.isIntegral() )
		return false;
	const auto& req_item_key = j_value["rqik"];
	if ( req_item_key.isNull() || !req_item_key.isIntegral() )
		return false;
	const auto& player_name = j_value["pname"];
	if ( player_name.isNull() || !player_name.isString() )
		return false;
	const auto& item_id = j_value["iid"];
	if ( item_id.isNull() || !item_id.isInt() )
		return false;
	const auto& item_cnt = j_value["icnt"];
	if ( item_cnt.isNull() || !item_cnt.isInt() )
		return false;
	const auto& begin_price = j_value["bprice"];
	if ( begin_price.isNull() || !begin_price.isInt() )
		return false;
	const auto& smart_price = j_value["smprice"];
	if ( smart_price.isNull() || !smart_price.isInt() )
		return false;
	const auto& jing_pai = j_value["jjprice"];
	if ( jing_pai.isNull() || !jing_pai.isInt() )
		return false;
	req_item_id_ = req_item_id.asUInt();
	req_item_key_ = req_item_key.asUInt();
	req_item_by_player_name_ = player_name.asString();
	item_id_ = item_id.asInt();
	item_cnt_ = item_cnt.asInt();
	begin_price_ = begin_price.asInt();
	smart_price_ = smart_price.asInt();
	jing_pai_price_ = jing_pai.asInt();
	return true;
}

bool stAuctionItemInfo::ConvertToJson( Json::Value& j_value ) const
{
	j_value["rqid"] = Json::UInt(req_item_id_);
	j_value["rqik"] = Json::UInt(req_item_key_);
	j_value["pname"] = req_item_by_player_name_;
	j_value["iid"] = item_id_;
	j_value["icnt"] = item_cnt_;
	j_value["bprice"] = begin_price_;
	j_value["smprice"] = smart_price_;
	j_value["jjprice"] = jing_pai_price_;
	return true;
}

void stAuctionItemInfo::DebugInfo() const
{
	LOG_O(Log_trace) << "auction item info,req item player name:" << req_item_by_player_name_ << 
		"  item id:" << item_id_ << "  item cnt:" << item_cnt_ << "  起拍价:" << begin_price_ <<
		"  一口价：" << smart_price_ << "  竞拍价：" << jing_pai_price_;
}

const boost::uuids::uuid& stAuctionItemInfo::GetItemInfoUuid() const
{
	if ( !item_info_uuid_.is_nil() )
		return item_info_uuid_;

	std::string str_tmp = std::to_string(req_item_id_);
	str_tmp += std::to_string(req_item_key_);
	str_tmp += req_item_by_player_name_;
	str_tmp += std::to_string(item_id_);
	str_tmp += std::to_string(item_cnt_);
	str_tmp += std::to_string(begin_price_);
	str_tmp += std::to_string(smart_price_);
	item_info_uuid_ = GenUuidFromBuff(str_tmp.c_str(), str_tmp.size());
	return item_info_uuid_;
}

bool operator == ( const stAuctionItemInfo& lhs, const stAuctionItemInfo& rhs )
{
	return lhs.GetItemInfoUuid() == rhs.GetItemInfoUuid();
}

bool operator < ( const stAuctionItemInfo& lhs, const stAuctionItemInfo& rhs )
{
	return lhs.GetItemInfoUuid() < rhs.GetItemInfoUuid();
}

P_CoorTypeT MakeTransRouteType( enGameAcType game_ac_type, int game_ser_id )
{
	return game_ac_type + (game_ser_id << 4);
}

bool stAuctionItemInfoEx::ParseFromJsonObjEx( const Json::Value& j_value )
{
	const auto& item_name = j_value["iname"];
	if ( item_name.isNull() || !item_name.isString() )
		return false;
	item_name_ = item_name.asString();
	return true;
}

bool stAuctionItemInfoEx::ConvertToJsonEx( Json::Value& j_value ) const
{
	j_value["iname"] = item_name_;
	return true;
}

stAuctionItemInfoEx::stAuctionItemInfoEx( const stAuctionItemInfo& rhs ) : stAuctionItemInfo(rhs)
{

}

stAuctionItemInfoEx::stAuctionItemInfoEx()
{

}

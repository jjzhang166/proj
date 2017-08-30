#include "stdafx.h"
#include "GTypes.h"
#include "Common/LogOut.h"
#include "GFuncCall.h"

GType::stProductInfo::stProductInfo()
{
	serial_ = 0;
	price_ = 0;
	item_cnt_ = 0;
}

bool GType::stProductInfo::FromJson( const Json::Value& json_v )
{
	const auto& json_se1 = json_v["ser1"];
	if ( json_se1.isNull() || !json_se1.isNumeric() )
		return false;
	const auto& json_se2 = json_v["ser2"];
	if ( json_se2.isNull() || !json_se2.isNumeric() )
		return false;
	const auto& json_p1 = json_v["p1"];
	if ( json_p1.isNull() || !json_p1.isNumeric() )
		return false;
	const auto& json_p2 = json_v["p2"];
	if ( json_p2.isNull() || !json_p2.isNumeric() )
		return false;
	const auto& json_cnt = json_v["c"];
	if ( json_cnt.isNull() || !json_cnt.isNumeric() )
		return false;
	serial_ = MakeInt64(json_se1.asUInt(), json_se2.asUInt());
	price_ = MakeInt64(json_p1.asUInt(), json_p2.asUInt());
	item_cnt_ = json_cnt.asInt();
	return true;
}

bool GType::stProductInfo::ConverToJson( Json::Value& json_v ) const
{
	auto double_v = SplitInt64(serial_);
	json_v["se1"] = double_v.first;
	json_v["ser2"] = double_v.second;
	double_v = SplitInt64(price_);
	json_v["p1"] = double_v.first;
	json_v["p2"] = double_v.second;
	json_v["c"] = item_cnt_;
	return true;
}

bool GType::IsPrivateProduct( LONGLONG price )
{
	//return !!(price % 2);
	return Price2GameAcType(price) != kGAT_Invalid;
}

/*
LONGLONG GType::MakePrivateProduct( LONGLONG price )
{
	return price + !(price % 2);
}

LONGLONG GType::MakePublicProduct( LONGLONG price )
{
	return price + price % 2;
}*/

enGameAcType GType::Price2GameAcType( LONGLONG price )
{
	assert(price > kGAT_Invalid);
	return enGameAcType(price % (kGAT_Invalid + 1));
}

LONGLONG GType::MakePriceByAcType( LONGLONG price, enGameAcType game_ac_type )
{
	assert(game_ac_type >= kGAT_Normal && game_ac_type <= kGAT_Invalid);
	assert(price > 0);
	int tmp = (int)(price % (kGAT_Invalid + 1));
	tmp = (int)game_ac_type - tmp;
	price += tmp;
	assert(price > 0);
	return price;
}

#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GTypes.h"
#include "Facility/GPlayer.h"
#include "Facility/GClass.h"

void GLuaC::BindStruct()
{
	__super::BindStruct();
	using namespace luabind;
	using namespace GType;
	module(lua())
		[
			class_<stMatchInfo>("stMatchInfo")
			.property("room_id", &stMatchInfo::GetRoomId, &stMatchInfo::SetRoomId)
			.property("match_id", &stMatchInfo::GetMatchId, &stMatchInfo::SetMatchId),

			class_<GPlayer::stLevelInfo>("GPlayer_LevelInfo")
			.def(constructor<>())
			.def(constructor<int, int, int>())
			.def(constructor<const GPlayer::stLevelInfo&>())
			.property("level", &GPlayer::stLevelInfo::GetLevel, &GPlayer::stLevelInfo::SetLevel)
			.property("exp", &GPlayer::stLevelInfo::GetExp, &GPlayer::stLevelInfo::SetExp)
			.property("to", &GPlayer::stLevelInfo::GetTo, &GPlayer::stLevelInfo::SetTo),

			class_<GPlayer::stPlayer>("GPlayer_Player")
			.property("id", &GPlayer::stPlayer::GetId, &GPlayer::stPlayer::SetId)
			.property("grade", &GPlayer::stPlayer::GetGrade, &GPlayer::stPlayer::SetGrade)
			.property("price", &GPlayer::stPlayer::GetPrice, &GPlayer::stPlayer::SetPrice)
			.property("life", &GPlayer::stPlayer::GetLife, &GPlayer::stPlayer::SetLife)
			.property("player_pos", &GPlayer::stPlayer::GetPosition, &GPlayer::stPlayer::SetPosition)
			.def("CanExchangePlayer", &GPlayer::stPlayer::CanExchangePlayer)
			.def("ExchangePlayer", &GPlayer::stPlayer::ExchangePlayer)
			.def("CanExchangePosition", &GPlayer::stPlayer::CanExchangePosition)
			.def("ExchangePostion", &GPlayer::stPlayer::ExchangePostion)
			.def("SaleSelfAs", &GPlayer::stPlayer::SaleSelfAs)
			.def("SaleSelfAs_NoStrict", &GPlayer::stPlayer::SaleSelfAs_NoStrict),

			class_<GPlayer::stItem>("GPlayer_Item")
			.property("id", &GPlayer::stItem::GetId, &GPlayer::stItem::SetId)
			.property("item_id", &GPlayer::stItem::GetItemId, &GPlayer::stItem::SetItemId)
			.property("remains", &GPlayer::stItem::GetRemains, &GPlayer::stItem::SetRemains)
			.property("expired", &GPlayer::stItem::GetExpired, &GPlayer::stItem::SetExpired)
			.def("CanUse", &GPlayer::stItem::CanUse)
			.def("Use", &GPlayer::stItem::Use),
			def("ItemId_BlueCard", &GPlayer::stItem::ItemId_BlueCard),
			def("ItemId_GreenCard", &GPlayer::stItem::ItemId_GreenCard),
			def("ItemId_RedCard", &GPlayer::stItem::ItemId_RedCard),
			def("ItemId_PurpleCard", &GPlayer::stItem::ItemId_PurpleCard),
			def("ItemId_EpCard", &GPlayer::stItem::ItemId_EpCard),
			def("ItemId_EpCardNew", &GPlayer::stItem::ItemId_EpCardNew),
			def("ItemId_ChangeCard", &GPlayer::stItem::ItemId_ChangeCard),
			def("ItemId_AutoMatchTicket", &GPlayer::stItem::ItemId_AutoMatchTicket),
			def("ItemId_BlueBallCard", &GPlayer::stItem::ItemId_BlueBallCard),
			def("ItemId_GreenBallCard", &GPlayer::stItem::ItemId_GreenBallCard),
			def("ItemId_RedBallCard", &GPlayer::stItem::ItemId_RedBallCard),
			def("ItemId_13ForwardCard", &GPlayer::stItem::ItemId_13ForwardCard),
			def("ItemId_13MidCard", &GPlayer::stItem::ItemId_13MidCard),
			def("ItemId_13BackCard", &GPlayer::stItem::ItemId_13BackCard),
			def("ItemId_13GateCard", &GPlayer::stItem::ItemId_13GateCard),
			def("ItemId_32CountryCard", &GPlayer::stItem::ItemId_32CountryCard),
			
			class_<GClass::HandleRecvPackage::stHttpQuestInfo>("stHttpQuestInfo")
			.property("method", &GClass::HandleRecvPackage::stHttpQuestInfo::GetMethod, &GClass::HandleRecvPackage::stHttpQuestInfo::SetMethod)
			.property("uri", &GClass::HandleRecvPackage::stHttpQuestInfo::GetUri, &GClass::HandleRecvPackage::stHttpQuestInfo::SetUri),

			class_<GPlayer::stSellingPriceRange>("stSellingPriceRange")
			.property("price_lower", &GPlayer::stSellingPriceRange::GetPriceLower, &GPlayer::stSellingPriceRange::SetPriceLower)
			.property("price_upper", &GPlayer::stSellingPriceRange::GetPriceUpper, &GPlayer::stSellingPriceRange::SetPriceUpper)
			.property("price_default", &GPlayer::stSellingPriceRange::GetPriceDefault, &GPlayer::stSellingPriceRange::SetPriceDefault)
			.def("Update", &GPlayer::stSellingPriceRange::Update)
		];
}
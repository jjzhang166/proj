#include "stdafx.h"
#include "GLuaC.h"
#include "Facility/GClass.h"
#include "FuncObjC.h"
#include "Facility/MatchInterface.h"
#include "Facility/GPlayer.h"
#include <Communicate/CGL_Defines.h>

using namespace GClass;

GLuaC* GLuac_Instance()
{
	auto normal_c = FuncObj_NormalC::GetInstance();
	if ( !normal_c )
		return nullptr;
	return &normal_c->GetGLua();
}

void GLuaC::BindClass()
{
	__super::BindClass();
	using namespace luabind;
	module(lua())
		[
			class_<GLuaC, LuaVM>("GLuaC"),
			def("GLuac_Instance", &GLuac_Instance),

			class_<FuncObj_NormalC>("FuncObj_NormalC")
			.def("LuaContinueOnClick", &FuncObj_NormalC::LuaContinueOnClick)
			.def("LuaCloseNoticeDialog", &FuncObj_NormalC::LuaCloseNoticeDialog)
			.property("game_state", &FuncObj_NormalC::GetGameState, &FuncObj_NormalC::SetGameState)
			.def("LuaTrainerSelectScene", &FuncObj_NormalC::LuaTrainerSelectScene)
			.def("LuaTrainerSelectedScene", &FuncObj_NormalC::LuaTrainerSelectedScene)
			.def("LuaCloseTaskDlg", &FuncObj_NormalC::LuaCloseTaskDlg)
			.def("CloseLevelUpDlg", &FuncObj_NormalC::CloseLevelUpDlg)
			.def("ForegroundTheWindow", &FuncObj_NormalC::ForegroundTheWindow)
			.def("PressDxinputKey", &FuncObj_NormalC::PressDxinputKey)
			.def("InputCoachName", &FuncObj_NormalC::InputCoachName)
			.def("BanAccount", &FuncObj_NormalC::BanAccount)
			.def("GetCoordinate", &FuncObj_NormalC::GetCoordinate)
			.def("CanNormalTradeProcedure", &FuncObj_NormalC::CanNormalTradeProcedure)
			.def("CanStoredTradeProcedure", &FuncObj_NormalC::CanStoredTradeProcedure)
			.def("Disconnect", &FuncObj_NormalC::Disconnect),
			def("FuncObj_NormalC_Instance", &FuncObj_NormalC::GetInstance),

			class_<IFightMode>("IFightMode")
			.def("GetFightMode", &IFightMode::GetFightMode)
			.def("GetParent", &IFightMode::GetParent)
			.def("BeginMatch", &IFightMode::BeginMatch)
			.def("EnterWaiting", &IFightMode::EnterWaiting)
			.def("EnterChoosePlayer", &IFightMode::EnterChoosePlayer)
			.def("EnterMatchReady", &IFightMode::EnterMatchReady)
			.def("EnterMatchAllReady", &IFightMode::EnterMatchAllReady)
			.def("MatchEnd", &IFightMode::MatchEnd)
			.def("MatchEnd_Prize", &IFightMode::MatchEnd_Prize)
			.def("MatchEnd_Back", &IFightMode::MatchEnd_Back),

			class_<IMatchMode>("IMatchMode")
			.def("IsSupportFightMode", &IMatchMode::IsSupportFightMode)
			.def("ChooseFightMode", &IMatchMode::ChooseFightMode)
			.def("GetMatchMode", &IMatchMode::GetMatchMode)
			.def("FightMode", &IMatchMode::FightMode)
			.def("SafeDelte", &IMatchMode::SafeDelte)
			.def("EnteredRoom", &IMatchMode::EnteredRoom)
			.def("GetParent", &IMatchMode::GetParent)
			.def("GetMatchInfo", &IMatchMode::GetMatchInfo)
			.def("MatchRoom_CloseContractDlg", &IMatchMode::MatchRoom_CloseContractDlg),

			class_<IGMatchMgr>("IGMatchMgr")
			.def("IsSupportMatchMode", &IGMatchMgr::IsSupportMatchMode)
			.def("ChooseMatch", &IGMatchMgr::ChooseMatch)
			.def("GetCurMatchMode", &IGMatchMgr::GetCurMatchMode)
			.def("SafeDelete", &IGMatchMgr::SafeDelete)
			.def("MatchMode", &IGMatchMgr::MatchMode)
			.def("PressFriendshipMatch", &IGMatchMgr::PressFriendshipMatch),
			def("GMatchMgr_Instance", &IGMatchMgr::GetMe),

			class_<GPlayer>("GPlayer")
			.property("ep", &GPlayer::GetEp, &GPlayer::SetEp)
			.property("gp", &GPlayer::GetGp, &GPlayer::SetGp)
			.property("level_info", &GPlayer::GetLevelInfo, &GPlayer::SetLevelInfo)
			.property("name", &GPlayer::GetName, &GPlayer::SetName)
			.property("cc_state", &GPlayer::GetCC_SubState, &GPlayer::SetCC_SubState)
			.property("create_coach_succeed", &GPlayer::IsCreateCoachSucceed, &GPlayer::SetCreateCoachSucceed)
			.property("ability_filter", &GPlayer::GetAbilityFilter, &GPlayer::SetAbilityFilter)
			.property("exch_price_filter", &GPlayer::GetExchPriceFilter, &GPlayer::SetExchPriceFilter)
			.property("high_price_filter", &GPlayer::GetHighPriceFilter, &GPlayer::SetHighPriceFilter)
			.property("getting_gp_data", &GPlayer::IsGettingGpData, &GPlayer::SetGettingGpDataTag)
			.property("against_offer_price", &GPlayer::GetAgainstOfferPrice, &GPlayer::SetAgainstOfferPrice)
			.property("match_limit_cnt", &GPlayer::GetMatchLimitCnt, &GPlayer::SetMatchLimitCnt)
			.def("FindPlayerById", &GPlayer::FindPlayerById)
			.def("SetBagItems", &GPlayer::SetBagItems)
			.def("FindItemById", &GPlayer::FindItemById)
			.def("FindItemByItemId", &GPlayer::FindItemByItemId)
			.def("Init_SetAllPlayers", &GPlayer::Init_SetAllPlayers)
			.def("MatchEnd_UpdatePlayers", &GPlayer::MatchEnd_UpdatePlayers)
			.def("IsCanExchangePlayer", &GPlayer::IsCanExchangePlayer)
			.def("CreateExchangePlayerIter", &GPlayer::CreateExchangePlayerIter)
			.def("IsNeedAutoSelectPlayer", &GPlayer::IsNeedAutoSelectPlayer)
			.def("AutoSelectPlayerBySend", &GPlayer::AutoSelectPlayerBySend)
			.def("SetFirstSecondPlayers", &GPlayer::SetFirstSecondPlayers)
			.def("SetTotalPlayersCnt", &GPlayer::SetTotalPlayersCnt)
			.def("ContractFirstAndSecondPlayers", &GPlayer::ContractFirstAndSecondPlayers)
			.def("HasAutoMatchTicket", &GPlayer::HasAutoMatchTicket)
			.def("SendAutoMatch", &GPlayer::SendAutoMatch)
			.def("GetAllPlayersCnt", &GPlayer::GetAllPlayersCnt)
			.def("IsNeedFirePlayer", &GPlayer::IsNeedFirePlayer)
			.def("AutoFirePlayers", &GPlayer::AutoFirePlayers)
			.def("AutoSelectSickEnemy", &GPlayer::AutoSelectSickEnemy)
			.def("IsSelectedSickEnemy", &GPlayer::IsSelectedSickEnemy)
			.def("SetEnemyInfo", &GPlayer::SetEnemyInfo)
			.def("AutoMatchTicketCnt", &GPlayer::AutoMatchTicketCnt)
			.def("UseSomeAutoMatchTicket", &GPlayer::UseSomeAutoMatchTicket)
			.def("SetFirstPlayers", &GPlayer::SetFirstPlayers)
			.def("SetSecondPlayers", &GPlayer::SetSecondPlayers)
			.def("CanJoinedPlayersCnt", &GPlayer::CanJoinedPlayersCnt)
			.def("DegradeAllThirdRubbishPlayers", &GPlayer::DegradeAllThirdRubbishPlayers)
			.def("FillFullThirdPlayers", &GPlayer::FillFullThirdPlayers)
			.def("NeedSupplyThirdPlayers", &GPlayer::NeedSupplyThirdPlayers)
			.def("IsTimeoutAtMatchRoom", &GPlayer::IsTimeoutAtMatchRoom)
			.def("IsCreateCoachProcedure", &GPlayer::IsCreateCoachProcedure)
			.def("GenCoachName", &GPlayer::GenCoachName)
			.def("SaleSpecifyPlayers", &GPlayer::SaleSpecifyPlayers)
			.def("SetAchievementInfo", &GPlayer::SetAchievementInfo)
			.def("SetGiftPackageInfo", &GPlayer::SetGiftPackageInfo)
			.def("BuyGiftPackageByIdx", &GPlayer::BuyGiftPackageByIdx)
			.def("CanBuyGiftPackage", &GPlayer::CanBuyGiftPackage)
			.def("SetMatchAddict", &GPlayer::SetMatchAddict)
			.def("FindPlayerBySpid", &GPlayer::FindPlayerBySpid)
			.def("SendFirstAndSecondPlayersInfo", &GPlayer::SendFirstAndSecondPlayersInfo)
			.def("ErasePlayerById", (void(GPlayer::*)(const char*, const char*))&GPlayer::ErasePlayerById)
			.def("OnSellPlayerSucceed", &GPlayer::OnSellPlayerSucceed)
			.def("GetFirstPlayerByPosition", &GPlayer::GetFirstPlayerByPosition)
			.def("GetCanBeSoldPlayer", &GPlayer::GetCanBeSoldPlayer)
			.def("KeepSellingPlayerAs", &GPlayer::KeepSellingPlayerAs)
			.def("KeepPlayersCntMoreThan", &GPlayer::KeepPlayersCntMoreThan)
			.def("KeepSellingRubbishPlayer", &GPlayer::KeepSellingRubbishPlayer)
			.def("KeepSellingGoodPlayers", &GPlayer::KeepSellingGoodPlayers)
			.def("BuyOtherPlayers", &GPlayer::BuyOtherPlayers)
			.def("OpenSomePlayersAs", &GPlayer::OpenSomePlayersAs)
			.def("NeedRefreshSelfOffserList", &GPlayer::NeedRefreshSelfOffserList)
			.def("RefreshSelfOfferList", &GPlayer::RefreshSelfOfferList)
			.def("SalePlayersAsDefault", &GPlayer::SalePlayersAsDefault)
			.def("UpdateSellingPrice", &GPlayer::UpdateSellingPrice)
			.def("FindSellingPrice", &GPlayer::FindSellingPrice)
			.def("LuaDegradeFirstAndSecondSync", &GPlayer::LuaDegradeFirstAndSecondSync)
			.def("ExchPlayersInPosNo", &GPlayer::ExchPlayersInPosNo)
			.def("ChooseExchPlayer", &GPlayer::ChooseExchPlayer)
			.def("GetSecondTeamCnt", &GPlayer::GetSecondTeamCnt)
			.def("UnpackAllBags", &GPlayer::UnpackAllBags)
			.def("RefreshCoachMe", &GPlayer::RefreshCoachMe)
			.def("SetJsonLevelInfo", &GPlayer::SetJsonLevelInfo)
			.def("HandleTimeCancelSale", &GPlayer::HandleTimeCancelSale)
			.def("SetTimeWaitSalePlayer", &GPlayer::SetTimeWaitSalePlayer)
			.def("SaleSomePlayersAsMaxPrice", &GPlayer::SaleSomePlayersAsMaxPrice)
			.def("SaleThePlayersAsMaxPrice", &GPlayer::SaleThePlayersAsMaxPrice)
			.def("SaleSomePlayersAsMinPrice", &GPlayer::SaleSomePlayersAsMinPrice)
			.def("SaleThePlayersAsMinPrice", &GPlayer::SaleThePlayersAsMinPrice)
			.def("SaleThePlayersAsPrice_Cnt", &GPlayer::SaleThePlayersAsPrice_Cnt)
			.def("SaleThePlayersSpecific", &GPlayer::SaleThePlayersSpecific)
			.def("IsFirstCalcDoubleTime", &GPlayer::IsFirstCalcDoubleTime)
			.def("IsAtDoubleTime", &GPlayer::IsAtDoubleTime)
			.def("UpdateDoubleTime", &GPlayer::UpdateDoubleTime)
			.def("IsAtTheTimeSpan", &GPlayer::IsAtTheTimeSpan)
			.def("IsJieCardTheTimeSpan", &GPlayer::IsJieCardTheTimeSpan)
			.def("IsJieCardTheTime_Sec", &GPlayer::IsJieCardTheTime_Sec)
			.def("Test", &GPlayer::Test)
			.def("JieCardThePlayer", &GPlayer::JieCardThePlayer)
			.def("BuyThePlayerAsPrice", &GPlayer::BuyThePlayerAsPrice)
			/*.def("SendSquadInfoAtMatch", &GPlayer::SendSquadInfoAtMatch)*/,
			def("GPlayerMe", &GPlayer::GetMe),

			class_<GPlayer::stPlayerIter, GPlayer::SharedPlayerIter>("GPlayer_PlayerIter")
			.def("First", &GPlayer::stPlayerIter::First)
			.def("Next", &GPlayer::stPlayerIter::Next)
			.def("IsDone", &GPlayer::stPlayerIter::IsDone)
			.def("CurItem", &GPlayer::stPlayerIter::CurItem),

			class_<HttpPackageSend>("HttpPackageSend")
			.def("SendPackageHit", &HttpPackageSend::SendPackageHit),
			def("HttpPackageSend_Me", &HttpPackageSend::GetMe)
		];
}
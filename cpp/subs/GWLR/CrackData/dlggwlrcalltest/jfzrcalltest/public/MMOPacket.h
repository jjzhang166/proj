#pragma once

#include "GameStructs.h"
class MMO_SendPrivate{
public:
	typedef bool (*MMO_SendImplT)(const void*, int, WORD);

private:
	template<typename T>
	friend bool MMO_sendT( const T& msg, WORD msg_id );
	friend bool MMO_sendT( const void* pSendBuf, int nLen, WORD wMethod );
	friend MMO_SendImplT GFC_GetMMO_SendImpl();

private:
	static bool MMO_send( const void* pSendBuf, int nLen, WORD wMethod );
};
#ifdef USE_MMO_SENDT_OF_SU_HUI__
template<typename T>
bool MMO_sendT( const T& msg, WORD msg_id ){
	return MMO_SendPrivate::MMO_send(&msg, sizeof(msg), msg_id);
}
inline bool MMO_sendT( const void* pSendBuf, int nLen, WORD wMethod ){
	return MMO_SendPrivate::MMO_send(pSendBuf, nLen, wMethod);
}
#endif
bool MMO_hooksend( const void* pSendBuf, int nLen, WORD wMethod );
bool MMO_gamehooksend( const void* pSendBuf);
bool MMO_accepttask(DWORD dwTaskId);
bool MMO_completetask(DWORD dwTaskId);
bool MMO_giveuptask(DWORD dwTaskId);
bool MMO_say(char* pszSayBuf,int nSayType);
bool MMO_walk(float fx, float fy, float fz);
bool MMO_Hit(void* pSendBuf, const MMO_Vector3& pos, const MMO_Direction2& dir);
bool MMO_FbGuoTu( const MMO_Vector3& theDoorCoord,char* pszGuoTuCmd);
bool MMO_CityGuoTu(char* pszGuoTuCmd);
bool MMO_GotoFb1(DWORD dwFbId,int nDiffLevel,DWORD dwFbType);
bool MMO_GotoFb2(DWORD dwFbId,DWORD dwType);
bool MMO_GotoFb3(DWORD dwType);
bool MMO_GotoFbChooseMode(DWORD dwFbId,DWORD dwMode);
bool MMO_ItmeMoveTo( const MMO_ITEM_MOVE_TO& theItemMoveTo);
bool MMO_ItmeCountMoveTo( const MMO_ITEM_MOVE_TO_COUNT& theItemMoveTo );
bool MMO_ItmeCountMoveToEmpty( const MMO_ITEM_MOVE_TO_COUNT& theItemMoveTo );
bool MMO_DoBuJiXiang(DWORD dwItemId,int nItemSlotIndex,DWORD dwItemCount);
bool MMO_facture();
bool MMO_ManuFactureItem(DWORD dwManuFactureId,DWORD dwTabIndex);
bool MMO_SellItem(int nItemTabIndex,int nItemSlotIndex,int nItemCount);
bool MMO_GetMailItem(LONGLONG llMailId,int nItemTabIndex);
bool MMO_GetMailItemInfo(LONGLONG llMailId);
bool MMO_OpenMail();
bool MMO_GetMailInfo(LONGLONG llMailId);
bool MMO_DeleteMail(LONGLONG llMailId);
bool MMO_TaskNpcChat(DWORD dwTaskId);
bool MMO_ChooseRole(DWORD dwRoleId);
bool MMO_DeleteRole(DWORD dwRoleId);
bool MMO_ChooseLine(DWORD dwLineId);
bool MMO_RetutnChooseRole(DWORD dwRoleId);
bool MMO_BuyItem(DWORD dwShopItemId, int nCount,int nBuyShopId);
bool MMO_CraftItem(DWORD dwCraftId);
bool MMO_UpEquip( const MMO_SEND_UP_EQUIP_PACKET& theUpEquip);
bool MMO_CreateRole( const char* pszRoleName);
bool MMO_OpenNpc(DWORD dwNpcId,int nOpenNpcType = 2);
bool MMO_LeaveZhuangYuan();
bool MMO_GotoZhuangYuan(LONGLONG llZhuangYuanId);
bool MMO_UpCollect(int nCollectIndex);
bool MMO_Dismantling( const MMO_DISMANTLING& theDismantling);
bool MMO_AcceptGongHuiTask(DWORD dwTaskFbLevelType,DWORD dwGongHuiTaskId);
bool MMO_AcceptQianShouTask(DWORD dwQianShouFbId);
bool MMO_ReqItemBytype(byte bTabIndex, byte bTypeIndex,byte bSlotIndex);
bool MMO_ReqItemBytype(const char* pszReqItemName,DWORD dwItemId);
bool MMO_GetSearchItemListByPage(int nPageIndex);
bool MMO_ReqBuyItem( const MMO_REQ_BUY_ITEM& theReqBuyItem);
bool MMO_GetMySellDetail();
bool MMO_ShangJiaAuctionItem( const MMO_SHANG_JIA_AUCTION_ITEM& theAuctionItem);
bool MMO_InviteTeam(DWORD dwActorPacketId);
bool MMO_JoinTeam(DWORD dwActorPacketId,char* pszTeamPwd);
bool MMO_ReqQuitTeam();
bool MMO_ReqKickMember(DWORD dwActorPacketId);
bool MMO_CreateTeamReq(char* pszTeamPwd);
bool MMO_ServerLineLaunch(int nServerLineIndex);
bool MMO_RequestEntrustList(/*DWORD dwFbTypeIndex,*/DWORD dwFbKeyFlag);
bool MMO_GetInstanceEnterData();
bool MMO_ZuoQiQiuQianWangMeiJieBoErTan();
bool MMO_OnReturnTown_OK();
bool MMO_FramSeed(DWORD dwFramSeedId,int nFramIndex);
bool MMO_SendMail( const MMO_SEND_MAIL& theSendMail);
bool MMO_GetGfitBag(DWORD dwGfitBagId);
bool MMO_TidyInventoryItem(PMMO_TIDY_ITEM theTidyItem);
bool MMO_XinShouXunLian_LiKaiJiaoXue();
bool MMO_JinRuJiaoXueFb(DWORD dwJiaoXueFbId );
bool MMO_GetWeekCreditCount( int nWeekCredietCount, MMO_WEEK_CREDIT& theWeekCredit );
LONGLONG GetSynCoorDinatesRandom();
bool MMO_SynCoorDinates( const SYN_COOR_DINATES& theSynCoorDinates);
bool MMO_AddFrient( const char* pszFrientName );
bool MMO_Request_Deal(DWORD dwDealRoleId);
bool MMO_Accept_Deal(DWORD dwRequest_DealKey);
bool MMO_Deal_Move_Item( const MMO_DEAL_MOVE_ITEM& theDealMoveItem );
bool MMO_Lock_Deal();
bool MMO_Enter_Deal();
bool MMO_Deal_Gold(int nGold);
bool MMO_UnLock_KingFb(DWORD dwKingFbId);

LONGLONG GetDynamicValue();
LONGLONG GetRand(LONGLONG llDynamic);

void mmo_copy(void* pSendBuf);

LONGLONG GetZhuangYuanId();
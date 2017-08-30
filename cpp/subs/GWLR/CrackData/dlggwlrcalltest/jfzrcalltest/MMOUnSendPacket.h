#pragma once

class MMOUnSendPacket
{
public:
	MMOUnSendPacket(void);
	~MMOUnSendPacket(void);
public:
	bool MmoPrintSend(void* pszSendBuf,void* pCallBack);
	void MmoHookSend(void);
	void MmoUnHookSend(void);
public:
	bool MmoPacketHit(void* pszSendBuf);
	bool MmoPacketGuoTu(void* pszSendBuf);
	bool MmoPacketCityGuoTu(void* pszSendBuf);
	bool MmoPacketReqItembyType(void* pszSendBuf);
};

extern MMOUnSendPacket theMmoUnSendPacket;


class MMOUnRecvPacket
{
public:
	MMOUnRecvPacket(void);
	~MMOUnRecvPacket(void);
public:
	void MmoHookRecv();
	void MmoUnHookRecv();
public:
	bool MmoAcceptTask(void* pszRecvBuf);
	bool MmoGiveUpTask(void* pszRecvBuf);
	bool MmoAcceptMailRetValue(void* pszRecvBuf);
	bool MmoRecvPlayerList(void* pszRecvBuf);
	bool MmoRecvDeletePlayer(void* pszRecvBuf);
	bool MmoRecvPlayerLine(void* pszRecvBuf);
	bool MmoRecvCreateRole(void* pszRecvBuf);
	bool MmoRecvReqItemBytype(void* pszRecvBuf);
	bool MmoRecvMyReqItemBytype(void* pszRecvBuf);
	bool MmoRecvInviteTeamFail(void* pszRecvBuf);
	bool MmoRecvTeamInfo(void* pszRecvBuf);
	bool MmoRecvJoinTeamInfo(void* pszRecvBuf);
	bool MmoRecvleaveTeam(void* pszRecvBuf);
	bool MmoRecvBeleaveTeam(void* pszRecvBuf);
	bool MmoRecvDisTeam(void* pszRecvBuf);
	bool MmoRecvTeamRetError(void* pszRecvBuf);
	bool MmoRecvTaskFbInfo(void* pszRecvBuf);
	bool MmoRecvReqBuyItemRet(void* pszRecvBuf);
	bool MmoRecvJiYunFbRet(void* pszRecvBuf);
	bool MmoRecvDoFbRet(void* pszRecvBuf);
	bool MmoRecvFbServerFullRet(void* pszRecvBuf);
	bool MmoRecvOpenNpcRet(void* pszRecvBuf);
	bool MmoRecvShangJiaAuctionItemRet(void* pszRecvBuf);
	bool MmoRecvSellItemRet(void* pszRecvBuf);
	bool MmoRecvCompleteTaskRet(void* pszRecvBuf);
	bool MmoRecvCompleteTaskFailRet(void* pszRecvBuf);
	bool MmoRecvTaskNpcChatRet(void* pszRecvBuf);
	bool MmoRecvFramSeedRet(void* pszRecvBuf);
	bool MmoRecvFeebHpRet(void* pszRecvBuf);
	bool MmoRecvSendMailRet(void* pszRecvBuf);
	bool MmoRecvUseItemRet(void* pszRecvBuf);
	bool MmoRecvUseItemRet2(void* pszRecvBuf);
	bool MmoRecvBuyItemRet(void* pszRecvBuf);
	bool MmoRecvGetGfitBagRet(void* pszRecvBuf);
	bool MmoRecvCollectItemBagFullRet(void* pszRecvBuf);
	bool MmoRecvTidyInventoryItemRet(void* pszRecvBuf);
	bool MmoRecvSendMailRet2(void* pszRecvBuf);
	bool MmoRecvSendGuoTuRet(void* pszRecvBuf);
	bool MmoRecvRequestDealRet(void* pszRecvBuf);
	bool MmoRecvRequestDealFailRet(void* pszRecvBuf);
	bool MmoRecvAcceptDealRet(void* pszRecvBuf);
	bool MmoRecvQuXiaoDealRet(void* pszRecvBuf);
	bool MmoRecvMoveDealItemRet(void* pszRecvBuf);
	bool MmoRecvMoveDealGoldRet(void* pszRecvBuf);
	bool MmoRecvDealStatusRet(void* pszRecvBuf);
	bool MmoRecvDealFailRet(void* pszRecvBuf);
	bool MmoRecvDealSucessRet(void* pszRecvBuf);
};
extern MMOUnRecvPacket theMmoUnRecvPacket;
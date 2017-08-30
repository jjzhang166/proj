#pragma once

#include "PacketString.h"

class UnPacket : public PacketString
{
public:
	UnPacket(void);
	UnPacket(const BYTE* lpPacket);
	~UnPacket(void);
public:

	bool HookUnPacket(void);
	bool UnHookUnPacket(void);
	bool UnPacketFun(LPVOID lpPraram);

	bool UnPacket_AllowSwitchChannel(const BYTE* pszRecvPacket);
	bool UnPacket_ChannelList(const BYTE* pszRecvPacket);
	bool UnPacket_PartialListPlayersStaticInfo(const BYTE* pszRecvPacket);
	bool UnPacket_RequestProductList(const BYTE* pszRecvPacket);
	bool UnPacket_RequestMyProductList(const BYTE* pszRecvPacket);
	bool UnPacket_MailList(const BYTE* pszRecvPacket);
	bool UnPacket_CreatePlayer(const BYTE* pszRecvPacket);
	bool UnPacket_EndPartialListPlayers(const BYTE* pszRecvPacket);
	bool UnPacket_Message(const BYTE* pszRecvPacket);
	bool UnPacket_RegisterProduct(const BYTE* pszRecvPacket);
	bool UnPacket_BuyProduct(const BYTE* pszRecvPacket);
	bool UnPacket_UnregisterProduct(const BYTE* pszRecvPacket);
	bool UnPacket_StartStageFailed(const BYTE* pszRecvPacket);
	bool UnPacket_MailContents(const BYTE* pszRecvPacket);
	bool UnPacket_GetAttachments(const BYTE* pszRecvPacket);
	bool UnPacket_RequestProductInfo(const BYTE* pszRecvPacket);
	bool UnPacket_Login(const BYTE* pszRecvPacket);
	bool UnPacket_SellItemResult(const BYTE* pszRecvPacket);
	bool UnPacket_BuyItemResult(const BYTE* pszRecvPacket);
	bool UnPacket_CreateNpc(const BYTE* pszRecvPacket);
	bool UnPacket_CreatePopInfoNpc(const BYTE* pszRecvPacket);
	bool UnPacket_AcceptQuestResult(const BYTE* pszRecvPacket);
	bool UnPacket_PlayerDead(const BYTE* pszRecvPacket);
	bool UnPacket_ChangeVillage(const BYTE* pszRecvPacket);
};

extern UnPacket	g_theUnPacket;
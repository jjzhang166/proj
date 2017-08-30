#pragma once

#include "PacketString.h"

class SendPacket : public PacketString
{
public:
	SendPacket();
	SendPacket(const BYTE* lpPacket);
	~SendPacket(void);
public:
	bool UnPacket_RequestProjectileHit(const BYTE* pszSendPacket);

	// 使用技能封包
	bool UnPacket_UseSkill(const BYTE* pszSendPacket);
	bool UnPacket_SkillActionSyncPacket(const BYTE* pszSendPacket);
	bool UnPacket_ActionSyncPacket(const BYTE* pszSendPacket);
	bool UnPacket_ProjectileSerialBound(const BYTE* pszSendPacket);
	bool UnPacket_DestroyProjectile(const BYTE* pszSendPacket);
	bool UnPacket_Activity(const BYTE* pszSendPacket);
	bool UnPacket_StartStage(const BYTE* pszSendPacket);
	bool UnPacket_StepInWarpGate(const BYTE* pszSendPacket);
	bool UnPacket_DirectionSyncPacket(const BYTE* pszSendPacket);
	bool UnPacket_ChangeSector(const BYTE* pszSendPacket);
	bool UnPacket_ReturnVillage(const BYTE* pszSendPacket);
	bool UnPacket_PickUpItem(const BYTE* pszSendPacket);
	bool UnPacket_RepairItem(const BYTE* pszSendPacket);
	bool UnPacket_VillageActionSyncPacket(const BYTE* pszSendPacket);
	bool UnPacket_EquipItem(const BYTE* pszSendPacket);
	bool UnPacket_UnequipItem(const BYTE* pszSendPacket);
	bool UnPacket_UseForItem(const BYTE* pszSendPacket);
	bool UnPacket_BuyItem(const BYTE* pszSendPacket);
	bool UnPacket_SellItem(const BYTE* pszSendPacket);
	bool UnPacket_ChangeVillage(const BYTE* pszSendPacket);
	bool UnPacket_LearnSkillMultiple(const BYTE* pszSendPacket);
	bool UnPacket_SelectPlayer(const BYTE* pszSendPacket);
	bool UnPacket_RequestSwitchChannel(const BYTE* pszSendPacket);
	bool UnPacket_SwitchChannel(const BYTE* pszSendPacket);
	bool UnPacket_MailSend(const BYTE* pszSendPacket);
	bool UnPacket_GetAttachments(const BYTE* pszSendPacket);
	bool UnPacket_MailRemove(const BYTE* pszSendPacket);
	bool UnPacket_JoinVillage(const BYTE* pszSendPacket);
	bool UnPacket_Accept(const BYTE* pszSendPacket);
	bool UnPacket_Complete(const BYTE* pszSendPacket);
	bool UnPacket_TeleportToVillage(const BYTE* pszSendPacket);
	bool UnPacket_Disassembly(const BYTE* pszSendPacket);
	bool UnPacket_StageReward(const BYTE* pszSendPacket);
	bool UnPacket_RequestProductList(const BYTE* pszSendPacket);
	bool UnPacket_BuyProduct(const BYTE* pszSendPacket);
	bool UnPacket_RegisterProduct(const BYTE* pszSendPacket);
	bool UnPacket_RequestMyProductList(const BYTE* pszSendPacket);
	bool UnPacket_UnregisterProduct(const BYTE* pszSendPacket);
	bool UnPacket_OpenItemPackage(const BYTE* pszSendPacket);
	bool UnPacket_CreateNpcByClient(const BYTE* pszSendPacket);
	bool UnPacket_UseItem(const BYTE* pszSendPacket);
	bool UnPacket_DestroyActors(const BYTE* pszSendPacket);
	bool UnPacket_RequestChangeHp(const BYTE* pszSendPacket);

	void Hook(void);
	void UnHook(void);
	void GameSendPacketHook(void);
	void UnGameSendPacketHook(void);
};

extern SendPacket	g_theSendPacket;


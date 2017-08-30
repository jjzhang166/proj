#pragma once
#include "stdafx.h"
typedef int (*PSENDPACKET)(LPVOID lpParameter,OUT char* pszsendpackebuffer);

DWORD GetTimeStamp();
DWORD GetTimeStampAddr();

class JFZRActionSystem
{
public:
	JFZRActionSystem(void);
	~JFZRActionSystem(void);
public:
	bool send(LPVOID lpParameter,PSENDPACKET pFun);
private:
	JFZRSkill		m_theSkill;
public:
	bool useskill(char* pszskillname,BYTE  bFirstHit,BYTE  bIsCounterAttack);
	bool actionuseskill(char* pszskillname,BYTE  bFirstHit,BYTE  bIsCounterAttack,DWORD dwSerial);
	bool useskill2(char* pszskillname,BYTE  bFirstHit,BYTE  bIsCounterAttack);
	bool StartStage(wchar_t* pszFbName,wchar_t* pszWarpGateId,DWORD dwLevel,BYTE	bIsRestart);
	bool GotoNextRoom(wchar_t* pszWarp);
	bool RetCity(void);
	bool PickUpItem(void);
	DWORD PassivePickUpItem(FLOAT& fx,FLOAT& fy,FLOAT& fz);
	bool RepairItem(void);
	bool ActionSyncPacket(wchar_t* pwszCmdWalk,float fx, float fy, float fz);
	bool LocalActionSyncPacket(float fx, float fy, float fz);
	bool NpcActionSyncPacket(wchar_t* pwszCmdWalk,PJFZR_MONSTER_OBJ ptheMonster);
	bool NpcHijacketActionSyncPacket(LPVOID ptheActionSyncPacket);
	bool DirectionSyncPacket(float fx, float fy, float fz);
	bool VillageActionSyncPacket(wchar_t* pwszCmdWalk,float fx, float fy, float fz);
	bool EquipItem(int nEquipSlot, int nInvenSlot);
	bool UnEquipItem(int nEquipSlot, int nInvenSlot);
	bool UseForItem(int nUsingSlotIndex, int nTargetSlotIndex);
	bool ChangeVillage(wchar_t* pwszVillageId);
	bool SellItem(wchar_t* pwszNpcName, int nTabCategory, int nSlotIndex, int nCount);
	bool BuyItem(wchar_t* pwszNpcName,DWORD dwSellId,int nCount);
	bool LearnSkill(wchar_t* pwszSkillName, DWORD dwSkillLevel);
	bool CreatePlayer(wchar_t* pszRoleName,DWORD dwClassType,DWORD dwPlayerGender);
	bool SelectPlayer(wchar_t* pszRoleName);
	bool RequestSwitchChannel(wchar_t* pszChannelName);
	bool SwitchChannel(const wchar_t* pszChannelName,const wchar_t* pszAccountId,const wchar_t* pszSessionKey);
	bool MailSend(int nItemSlotIndex);
	bool JoinVillage(void);
	bool Accept(wchar_t* pwszQuestId);
	bool Complete(wchar_t* pwszQuestId,wchar_t*	pwszRewardItemId);
	bool GiveUp(wchar_t* pwszQuestId);
	bool TeleportToVillage(wchar_t* pwszQuestId, DWORD dwState);
	bool Disassembly(DWORD dwTabCategory, DWORD dwSlotIndex,WORD wIsAvatarItem);
	bool DeselectPlayer();
	bool ListPlayers();
	bool StageReward(DWORD dwRewardIndex);
	bool RequestProductList(wchar_t* pwszCategory,wchar_t* pwszKeyWord,DWORD dwPageNum);
	bool BuyProduct(LONGLONG llOidMarket, DWORD dwCount);
	bool RegisterProduct(wchar_t* pwszName, DWORD dwItemTabCategory, DWORD dwItemSlotIndex, LONGLONG llPrice, DWORD dwPeriod, DWORD dwCount);
	bool RequestMyProductList();
	bool UnregisterProduct(LONGLONG llOidMarket);
	bool GetAttachments(LONGLONG llMailSerial, BYTE bIsFirst);
	bool MailRemove(LONGLONG llMailSerial, BYTE bIsFirst);
	bool Activity(DWORD dwType,DWORD dwCount,DWORD dwDefenderSerial,DWORD dwdwAttackerSerial);
	bool RequestProductInfo(DWORD dwItemTabCategory,DWORD dwItemSlotIndex);
	bool OpenItemPackage(int nItemPackageTabIndex,WORD wItemPackageSlotIndex,int nRequireItemTabIndex,WORD wRequireItemSlotIndex);
	bool UseItem(int nTabIndex,WORD wSlotIndex);
	bool CreateNpcByClient(DWORD dwMasterSerial,DWORD dwRequestSerial);
	bool DestroyActors(DWORD dwSerial);
	bool Test(DWORD dwIndex);
};

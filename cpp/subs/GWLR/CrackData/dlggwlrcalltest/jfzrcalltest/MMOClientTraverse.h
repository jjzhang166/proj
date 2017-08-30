#pragma once
#include "public/GameStructs.h"


class MMOActor
{
public:
	MMOActor();
	~MMOActor();
public:
	DWORD				m_dwActorObjAddr;				// 遍历对象时的地址，非addr2
	DWORD				m_dwActorId;					// 角色本地id
	MMO_ActorVector*	m_ptheActorVector;				// 角色坐标，面向信息
	DWORD				m_dwActorPacketId;				// 角色封包id
	int					m_nActorXianLu;					// 角色在几线
	int					m_nLieShouXuKeZhengCount;		// 猎守许可证数量，也就是进入副本剩余多少次
public:
	bool ReadActor();
	CString GetRoleInMap();		// 获得玩家所在地图
	void WirtePos(float fx, float fy, float fz, float fdir);
	void WirtePos2(float fx, float fy, float fz, float fdir);
	void WirtePos3(float fx, float fy, float fz, float fdir);
};

void NpcWirtePos(DWORD dwObjAddr1,float fx, float fy, float fz);


CString TraverseObj();
CString TraversePlayer();
CString TraverseItem();
CString GetTaskInfo();
CString GetReceivedTaskInfo();
CString GetUiInfo();
CString TraverseReceivedTask();
CString GetReceivedNpcTask();
CString GetSuoYouShiXianDuiXiang(DWORD Dwtype =-1);
CString GetInFbInfo();
CString GetTemplateItemInfo();
CString GetMailInfo();
CString GetCollectObjAddr2();
CString GetShopInfo(int nNpcIndex);
CString GetCraftInfo();
CString GetManuFactureInfo();
CString GetCDialogInfo();
CString GetFarmSeedInfo();
CString GetFarmFacilityInfo();
CString GetFarmFacilityState();
CString GetRequireCurSlotSupplysAll();
CString GetRoleBuffState();
CString GetBuffInfo();
CString GetGiftBagInfo();
CString GetCPlayerLevelUpInfo();
CString GetTaskFbInfo();
CString GetShangJinTaskInfo();
CString GetKingQuestInfo();

void HookMonsterHpCall();
void HookActorHpCall();
void HookLoadMapCallEnd();
void HookDialogInfoCall();
void HookJingcai();//HOOK精彩镜头
// 瞬移
void WritePos(float fx, float fy, float fz);
#include "StdAfx.h"
#include "SendPacket.h"
#include "jfzrpacketstruct.h"


SendPacket	g_theSendPacket;
DWORD	g_dwSendPacketCallAddr = 0;
char szSendPacketData[0x4096] = {0};
char szPrint[0x4196] = {0};

// HOOK 输出所有明文包信息
bool PrintSendPacket(const BYTE* pszSendPacket)
{
	try
	{
		DWORD dwlen = *(WORD*)pszSendPacket;

		SendPacket theSendPacket;

		WORD wMethodType = *(WORD*)(pszSendPacket+2);

		if( wMethodType == 0x73DE )
		{
			//assert(false);

			WORD wUrlSendType = *(WORD*)(pszSendPacket+4);
			if( wUrlSendType == 0x0000A6B8 )
			{
				//assert(false);
			}
		}

		if( wMethodType != 0x55E0 )
		{
			return true;
		}

		WORD wUrlSendType = *(WORD*)(pszSendPacket+4);
		
		switch( wUrlSendType )
		{
		case 0x0000A6B8:	//0xF816:		// Apple::Protocol::ToServer::ActionSystem::RequestProjectileHit	"c492f0d9b91babcb562db514f3a9a86b"
			{
				//return false;
				return theSendPacket.UnPacket_RequestProjectileHit(pszSendPacket);		// 技能攻击，只需要拦截此封包，就可以无敌了
			}
			break;
		case 0x00003EEC:	//0xD671:		// Apple::Protocol::Both::ActionSystem::SkillActionSyncPacket		"74c86e7c7a5628a1af55467d081c8b97"
			{
				return theSendPacket.UnPacket_SkillActionSyncPacket(pszSendPacket);
			}
			break;
		case 0x000066DC:	//0x15FA:		// Apple::Protocol::Both::ActionSystem::ActionSyncPacket		"038f2071d608b0d6942d097326ccbb27"
			{
				return theSendPacket.UnPacket_ActionSyncPacket(pszSendPacket);		// 开始走路和结束走路封包
			}
			break;
		case 0x00001379:	// e1ddc887f465dd79053399f012a3cac9"  "Apple::Protocol::Both::ActionSystem::DirectionSyncPacket"
			{
				return false;
				return theSendPacket.UnPacket_DirectionSyncPacket(pszSendPacket);	// 同步方向封包
			}
			break;
		case 0x0000A99E:	//0xFA66:		// Apple::Protocol::ToServer::ActionSystem::UseSkill		"16e232a49cbd80393d7f4c0009244f20"
			{
				return theSendPacket.UnPacket_UseSkill(pszSendPacket);
			}
			break;	
		case 0x0000F938:	//0x2063:		// Apple::Protocol::Both::ActionSystem::ProjectileSerialBound		 "88caf8ce0047c589575cb2f9fafd737c"
			{
				//return theSendPacket.UnPacket_ProjectileSerialBound(pszSendPacket);		// 这个使用技能后的封包疑似被去掉了
			}
			break;
		case 0x0000DC60:	//0xE5A8:		// Apple::Protocol::Both::ActionSystem::DestroyProjectile		"c1c00f59beadd90ddf13e336885143c5"
			{
				//return theSendPacket.UnPacket_DestroyProjectile(pszSendPacket);			// 这个使用技能后的封包疑似被去掉了
			}
			break;
		case 0x00009033:	//0x000085E0:		// 只有进入副本房间，才会来一次，其他都不会到		"ae9d2edb864599d0041860fb6c6ad153"
			{
				//OutputDebugString("[JFZR] Apple::Protocol::ToServer::StageSystem::ActorsLoaded");
				//MessageBox(NULL,"进入副本，或过房间","",MB_OK);
			}
			break;
		case 0x0000D20F:	// "Apple::Protocol::ToServer::StageSystem::StartStage"		"dd246d8973092d9a60c0bddb6f8f39b0"
			{
				return theSendPacket.UnPacket_StartStage(pszSendPacket);			// 进入副本，再次挑战副本
			}
			break;
		case 0x000075AA:	// "Apple::Protocol::ToServer::ActorSystem::StepInWarpGate"  "6490af8101d3c41132b3366b2a46a72d" 
			{
				return theSendPacket.UnPacket_StepInWarpGate(pszSendPacket);		// 告诉服务器步入传送门
			}
			break;
		case 0x0000D753:	// "51d107379e1955235e4e5dc41dd2e391"   "Apple::Protocol::ToServer::StageSystem::ChangeSector"
			{
				return theSendPacket.UnPacket_ChangeSector(pszSendPacket);
			}
			break;
		case 0x0000943F:	// "85ac893b6ce6de3a6dee371c56807d8e"   "Apple::Protocol::ToServer::StageSystem::ReturnVillage"
			{
				return theSendPacket.UnPacket_ReturnVillage(pszSendPacket);
			}
			break;
		case 0x0000825B:	// "76883bc7e7b32d16bdf424a2071c1ee8"   "Apple::Protocol::ToServer::InventorySystem::PickUpItem"
			{
				return theSendPacket.UnPacket_PickUpItem(pszSendPacket);
			}
			break;
		case 0x0000D1E1:	// "a1cf156de551bb58d0271c523b5ad578"    "Apple::Protocol::ToServer::ShopSystem::RepairItem"
			{
				return theSendPacket.UnPacket_RepairItem(pszSendPacket);
			}
			break;
		case 0x0000F8A2:	// "982d94fcd2d7650a650a2644ff41d9ac"      "Apple::Protocol::Both::ActionSystem::VillageActionSyncPacket"
			{
				return theSendPacket.UnPacket_VillageActionSyncPacket(pszSendPacket);
			}
			break;
		case 0x0000C43A:	// "cebca38adfe1a51de79f579873e41133"    "Apple::Protocol::ToServer::InventorySystem::EquipItem"
			{
				return theSendPacket.UnPacket_EquipItem(pszSendPacket);
			}
			break;
		case 0x0000AA2E:	// "d5b24ed93d2a59535d3291aca7173c8e"	"Apple::Protocol::ToServer::InventorySystem::UnequipItem"
			{
				return theSendPacket.UnPacket_UnequipItem(pszSendPacket);
			}
			break;
		case 0x0000B185:	// "fcf22fe368eaea7f7a2433ab1fea257a"	"Apple::Protocol::ToServer::InventorySystem::UseForItem"
			{
				return theSendPacket.UnPacket_UseForItem(pszSendPacket);
			}
			break;
		case 0x00002878:	// "35bf1b03b248e6642008740c7cca0f01"	"Apple::Protocol::ToServer::ShopSystem::BuyItem"
			{
				return theSendPacket.UnPacket_BuyItem(pszSendPacket);
			}
			break;
		case 0x0000BE87:	// "2f6401c12fc6a415ea86fada78c3617c"	"Apple::Protocol::ToServer::ShopSystem::SellItem"
			{
				return theSendPacket.UnPacket_SellItem(pszSendPacket);
			}
			break;
		case 0x0000EE28:	// "8700c48e9de9aa244ec92d34c498fe78"	"Apple::Protocol::ToServer::StageSystem::ChangeVillage"
			{
				return theSendPacket.UnPacket_ChangeVillage(pszSendPacket);
			}
			break;
		case 0x0000F395:	// "a31bfe5025bd74f8cfd945c56fb74251"	"Apple::Protocol::ToServer::ShopSystem::LearnSkillMultiple"
			{
				return theSendPacket.UnPacket_LearnSkillMultiple(pszSendPacket);
			}
			break;
		case 0x00008CAF:	// "5cefcbe0615c112023bbcca2d1cbd8bc"	"Apple::Protocol::ToServer::SessionSystem::SelectPlayer"
			{
				return theSendPacket.UnPacket_SelectPlayer(pszSendPacket);
			}
			break;
		case 0x00001299:	// "8bedfbed6e9106a8a208e391c60cfeb3"	"Apple::Protocol::ToServer::SessionSystem::RequestSwitchChannel"
			{
				return theSendPacket.UnPacket_RequestSwitchChannel(pszSendPacket);		// 拦截此封包可禁止新号进入游戏，直接到创建角色界面
			}
			break;
		case 0x0000B685:	// "827d94dc425544c8bf32122e7d3f0f84"	"Apple::Protocol::ToServer::SessionSystem::SwitchChannel"
			{
				return theSendPacket.UnPacket_SwitchChannel(pszSendPacket);
			}
			break;
		case 0x000059D4:	// "5fc62cc2d7a859c3e039eb81b3235d3c"	"Apple::Protocol::ToServer::MailSystem::MailSend"
			{
				return theSendPacket.UnPacket_MailSend(pszSendPacket);
			}
			break;
		case 0x00003096:	// "f8260de7249e4d60ab7ee464e5f499e1"	"Apple::Protocol::ToServer::MailSystem::GetAttachments"
			{	
				return theSendPacket.UnPacket_GetAttachments(pszSendPacket);
			}
			break;
		case 0x000028F6:	// "246578b8220355a2c542e01995fe3eff"	"Apple::Protocol::ToServer::MailSystem::MailRemove"
			{
				return theSendPacket.UnPacket_MailRemove(pszSendPacket);
			}
			break;
		case 0x000067EE:	// "e56a7bfbcf0df1471b9c59d6b0444703"		"Apple::Protocol::ToServer::StageSystem::JoinVillage"
			{
				return theSendPacket.UnPacket_JoinVillage(pszSendPacket);
			}
			break;
		case 0x000004B2:	// "6c32a7184cafa0cfd343ecc1a6ea3a8d"	"Apple::Protocol::ToServer::QuestSystem::Accept"
			{
				return theSendPacket.UnPacket_Accept(pszSendPacket);
			}
			break;
		case 0x00000138:	// "7824a9c8b4fc5eff77a1a20c43fe6135"	"Apple::Protocol::ToServer::QuestSystem::Complete"
			{
				return theSendPacket.UnPacket_Complete(pszSendPacket);
			}
			break;
		case 0x000090A7:	// "92ad2a60a8f9da49132538defa4acd13"	"Apple::Protocol::ToServer::QuestSystem::TeleportToVillage"
			{
				return theSendPacket.UnPacket_TeleportToVillage(pszSendPacket);
			}
			break;
		case 0x00006F6B:	// "920f9bdd034c400bd59dc8c770e73d5a"	"Apple::Protocol::ToServer::ItemTransformSystem::Disassembly"
			{
				return theSendPacket.UnPacket_Disassembly(pszSendPacket);
			}
			break;
		case 0x00001B1D:	// "a8d7b003267a116610630b3c583ee7b3"	"Apple::Protocol::ToServer::StageSystem::StageReward"
			{
				return theSendPacket.UnPacket_StageReward(pszSendPacket);
			}
			break;
		case 0x00004498:	// "f93c6af2752f096fd94bde35d0422520"	"Apple::Protocol::ToServer::MarketSystem::RequestProductList"
			{
				return theSendPacket.UnPacket_RequestProductList(pszSendPacket);
			}
			break;
		case 0x00003356:	// "9a6e248c23b03341309edf0af3d02c93"	"Apple::Protocol::ToServer::MarketSystem::BuyProduct"
			{
				return theSendPacket.UnPacket_BuyProduct(pszSendPacket);
			}
			break;
		case 0x00007373:	// "c9c08a120d4964bd79fe1ee858d6a665"	"Apple::Protocol::ToServer::MarketSystem::RegisterProduct"
			{
				return theSendPacket.UnPacket_RegisterProduct(pszSendPacket);
			}
			break;
		case 0x00003894:	// "7fe1f4a833cc612bbef1a9e14aecdc2c"	"Apple::Protocol::ToServer::MarketSystem::RequestMyProductList"
			{
				return theSendPacket.UnPacket_RequestMyProductList(pszSendPacket);
			}
			break;
		case 0x00009E6C:	// "56392c225f0a796f2184b88856b055fb"	"Apple::Protocol::ToServer::MarketSystem::UnregisterProduct"
			{
				return theSendPacket.UnPacket_UnregisterProduct(pszSendPacket);
			}
			break;
		case 0x0000BF6A: // 0x00004122:		// "1127a4bae67312f0d7a2fd8ffdf915dd"	"Apple::Protocol::ToServer::ActorSystem::Activity"					连击封包
			{
				return theSendPacket.UnPacket_Activity(pszSendPacket);
			}
			break;
		case 0x0000B11E:	// "415c2a704e50d150c6a26919d9c32405"	 "Apple::Protocol::ToServer::InventorySystem::OpenItemPackage"
			{
				return theSendPacket.UnPacket_OpenItemPackage(pszSendPacket);
			}
			break;
		case 0x00003BB9:	//  "e14a6a7ed25052d68ba65f06039b538a"	"Apple::Protocol::ToServer::ActorSystem::CreateNpcByClient"
			{
				return theSendPacket.UnPacket_CreateNpcByClient(pszSendPacket);
			}
			break;	
		case 0x0000BBC1:	// "3ed486eb9fcc46121a77b379e97f3b61"	"Apple::Protocol::ToServer::InventorySystem::UseItem"
			{
				return theSendPacket.UnPacket_UseItem(pszSendPacket);
			}
			break;
		case 0x00008C56:	// "99a17b89ddaa7cddb22110d75f526fa0"	"Apple::Protocol::ToServer::StageSystem::DestroyActors"
			{
				return theSendPacket.UnPacket_DestroyActors(pszSendPacket);
			}
			break;
		// 拦截这个封包，处理BUFF扣血
// 		case 0x00002CCA:	// "d3ef9eb8f5b784744e2cefec23510043"	"Apple::Protocol::Both::BuffSystem::RequestAddBuff"
// 			{
// 				return false;
// 			}
// 			break;
		case 0x0000493E:	// "7822688b9bdfadbac7d6719c9242c75b"	"Apple::Protocol::Both::StatusSystem::RequestChangeHp"
			{
				return theSendPacket.UnPacket_RequestChangeHp(pszSendPacket);
			}
			break;
		case 0x00001699:	// "d78608a5f470c9da44d365ec767d8219"	"Apple::Protocol::Both::BuffSystem::Immune"		// 怪物死亡
			{
				
			}
			break;
		case 0x000009DA:	// "d40ed83df59956f3382dc31f4d14a3a7"	"Apple::Protocol::Both::StatusSystem::ChangeStatus"
			{
				//OutputDebugString("[JFZR] Apple::Protocol::Both::StatusSystem::ChangeStatus");
				return false;
			}
			break;
		case 0x00000B0C:		// 副本进度条	"170a65fa47f184ede038f02806d9ca87" "Apple::Protocol::ToServer::StageSystem::SyncProgressBarValue"
			{
				DWORD	dwValue = 0;
				dwValue = *(DWORD*)(pszSendPacket+6);
				if( dwValue >= 0x63 )
				{
					OutputDebugString("[JFZR] Apple::Protocol::ToServer::StageSystem::SyncProgressBarValue");
				}
			}
		case 0x0000FE97:	// 0x0000E5DB:		// Apple::Protocol::ToServer::StageSystem::LoadEnd	"4eeded8773de7d78bc7c47fb9296251b"
			{
				//MessageBox(NULL,"LoadEnd","",MB_OK);
			}
			break;
		case 0x00002997:
			{
				MessageBox(NULL,"RequestIntrusionNpcPop","",MB_OK);
			}		
			break;
		case 0x000031F1:
			{
				MessageBox(NULL,"RequestIntrusionNpcPopResult","",MB_OK);
			}
			break;
		case 0x00000019:		// Apple::Protocol::Both::AntiHackingSystem::RawData	"3808f130d834b3f159ff62dee631a352"
			{

			}
			break;
 		case 0x0000B308:		// "455e0b7c001cad4661d39e29a754e232"  Apple::Protocol::Both::ActionSystem::PassiveActionSyncPacket		同步被动行动封包
			{
				return false;
			}
			break;
		case 0x00001E82:	// "33062ebfe3aaf4e8ab75edc77b5cea5d"	"Apple::Protocol::Both::ActionSystem::InterpolDirectionPacket"
			{
				return false;
			}
			break;
// 		case 0x0000CF9E:		// Apple::Protocol::Both::StatusSystem::ChangeStatus
// 		case 0x00005D5A:		// Apple::Protocol::Both::ActionSystem::InterpolDirectionPacket
// 		case 0x00009B44:		// Apple::Protocol::Both::StatusSystem::EndStatusDelta				结束对立状态
// 		case 0x00009A2B:		// Apple::Protocol::ToServer::InventorySystem::PickUpItem			拾取物品
// 		case 0x00000081:		// Apple::Protocol::ToServer::ShopSystem::GetRepairPrice
// 		case 0x00002EA3:		// Apple::Protocol::ToServer::GuildSystem::RequestGuildInfo
// 		case 0x00007788:		// Apple::Protocol::ToServer::QuestSystem::RequestCompletedQuestList
// 		case 0x00000541:		// Apple::Protocol::Both::BuffSystem::PassiveDestroyBegin		"e788b895821540ab2677faa35e2de558"	开始被动销毁，好像是怪物销毁的封包
// 		case 0x1E82:
// 		case 0xB308:
// 			{
// 
// 			}
// 			break;
		default:
			{
				for (DWORD i = 0 ; i < dwlen ; i++)
				{
					wsprintfA(szSendPacketData+i*3,"%02X ",pszSendPacket[i]);
				}
				szSendPacketData[dwlen*3] = 0;

				wsprintfA(szPrint,"[JFZR] send %s",szSendPacketData);
				OutputDebugStringA(szPrint);
			}
		}
	}
	catch (...)
	{
		assert(false);
	}

	return true;
}

_declspec(naked) void HookSendPacket()
{
	__asm
	{
		pushfd
		pushad	// 0x24
	}

	__asm
	{
		mov eax, dword ptr[esp+0x2C]			// mov eax, dword ptr[esp+0x4] 这是HOOK封包CALL的上一层调用CALL时取得明文数据  JFZR_REVERSAL_SDB_BASE_ADDR_UP_DATA_HOOK___  或者取EAX
		mov eax, dword ptr[eax+0x8]
		push eax
		lea eax, PrintSendPacket
		call eax
		add esp, 4

		test al, al
		jnz PASS
		popad
		popfd
		mov al, 1
		retn
PASS:
	}

	__asm
	{
		popad
		popfd

		PUSH EBP
		MOV EBP,ESP
		PUSH -0x1
		mov eax, g_dwSendPacketCallAddr
		add eax, 5
		jmp eax
	}
}

SendPacket::SendPacket() : PacketString((const BYTE *)"")
{

}

SendPacket::SendPacket(const BYTE* lpPacket) : PacketString(lpPacket)
{

}

SendPacket::~SendPacket(void)
{
}

//extern void Hook(DWORD dwHookAddr,LPVOID pfun);

void SendPacket::Hook(void)
{
	g_dwSendPacketCallAddr = JFZR_REVERSAL_SEND_ADDR_CALL;
	::Hook(g_dwSendPacketCallAddr,HookSendPacket);
}

void SendPacket::UnHook(void)
{
	byte szCode[] = {0x55,0x8B,0xEC,0x6A,0xFF};
	_WriteProcessMemory(g_dwSendPacketCallAddr,szCode,sizeof(szCode));
}


_declspec(naked) void HookGameSendPacket()
{
	__asm
	{
		pushfd
		pushad	// 0x24
	}

	__asm
	{
		LEA EAX,DWORD PTR SS:[EBP-0x2C]
		mov eax, dword ptr[eax+0x8]
		push eax
		lea eax, PrintSendPacket
		call eax
		add esp, 4

		test al, al
		jnz PASS
		popad
		popfd
		mov eax, JFZR_REVERSAL_SEND_ADDR_PRE_CALL_HOOK
		add eax, 5
		push eax
		mov al, 1
		retn
PASS:
	}

	__asm
	{
		popad
		popfd

// 		LEA EAX,DWORD PTR SS:[EBP-0x2C]
// 		PUSH EAX
// 		PUSH ECX

		mov eax, JFZR_REVERSAL_SEND_ADDR_CALL
		call eax

		mov eax, JFZR_REVERSAL_SEND_ADDR_PRE_CALL_HOOK
		add eax, 5
		jmp eax
	}
}

void SendPacket::GameSendPacketHook(void)
{
	::Hook(JFZR_REVERSAL_SEND_ADDR_PRE_CALL_HOOK,HookGameSendPacket);
}

void SendPacket::UnGameSendPacketHook(void)
{
	byte szCode[] = {0xE8,0xB7,0xFB,0xFF,0xFF};		// 每次更新游戏必变
	_WriteProcessMemory(JFZR_REVERSAL_SEND_ADDR_PRE_CALL_HOOK,szCode,sizeof(szCode));
}

wchar_t wszPrint[0x4196] = {0};

bool SendPacket::UnPacket_RequestProjectileHit(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_RequestProjectileHit*********************");

// 	JFZR_MONSTER_OBJ_STRUCT theObjStruct;
// 	memset(&theObjStruct,0,sizeof(theObjStruct));
// 	memcpy(theObjStruct.str_szType,L"NPC",wcslen(L"NPC")*2);
// 
// 	// 遍历对象
// 	theMonsterObj.TraverseObj(false);
// 
// 	if( theMonsterObj.GetObjCount(L"NPC") == 0 )
// 	{
// 		assert(false);
// 	}
// 	theObjStruct.str_nCount = theMonsterObj.GetArrayObj(&theObjStruct,100);

	extern bool g_bChangeSectorFlag;
	if( g_bChangeSectorFlag == true )
	{
		try
		{
			GetTimeStampAddr();
		}
		catch (...)
		{
			assert(false);
		}

		JFZRActionSystem theActionSystem;
		theActionSystem.useskill("HandShot",0,0);

		return 0;
	}

	

	
	SEND_SKILL_HIT_HEADER	theRequestProjectileHitHeader;
	*this = (pszSendPacket);
	*this >> theRequestProjectileHitHeader.theHeader.wPacketLen;
	*this >> theRequestProjectileHitHeader.theHeader.wMethodType;
	*this >> theRequestProjectileHitHeader.theHeader.wUrlType;
	*this >> theRequestProjectileHitHeader.wRequestHitInfos;
	*this >> theRequestProjectileHitHeader.warray;
	*this >> theRequestProjectileHitHeader.dwsize;

	for (DWORD i = 0; i<theRequestProjectileHitHeader.dwsize; i++)
	{
		SEND_REQUEST_PROJECTILE_HIT_BODY	theRequestProjectileHitBody;
		*this >> theRequestProjectileHitBody.wItem_i;
		*this >> theRequestProjectileHitBody.wHitInfo;
		*this >> theRequestProjectileHitBody.dwProjectileSerial;
		*this >> theRequestProjectileHitBody.dwDefenderSerial;
		*this >> theRequestProjectileHitBody.dwAttackerSerial;
		*this >> theRequestProjectileHitBody.wHitResultLen;
		*this >> theRequestProjectileHitBody.strHitResult;
		*this >> theRequestProjectileHitBody.dwHitType;
		*this >> theRequestProjectileHitBody.dwDeltaHp;
		*this >> theRequestProjectileHitBody.dwRandomSeed;
		*this >> theRequestProjectileHitBody.wHitProjectileInfo;
		*this >> theRequestProjectileHitBody.wAppleHitProjectileInfo;

 		//*this >> theRequestProjectileHitBody.fAttackPowerRate;
		theRequestProjectileHitBody.fAttackPowerRate = -1;			// 将NPC的攻击角色的攻击改为1
		*this << theRequestProjectileHitBody.fAttackPowerRate;

 		*this >> theRequestProjectileHitBody.fDamagePlusRateToSuperArmor;
		*this >> theRequestProjectileHitBody.fDamagePlusRateForCounter;
		*this >> theRequestProjectileHitBody.wPhysicalAttribute;
 		*this >> theRequestProjectileHitBody.wElementalAttributeLen;
 		*this >> theRequestProjectileHitBody.strElementalAttribute;
		*this >> theRequestProjectileHitBody.dwEventOptionSerial;
		*this >> theRequestProjectileHitBody.dwEventOptionDamageReduceRate;
		*this >> theRequestProjectileHitBody.bIsCounterAttack;
		*this >> theRequestProjectileHitBody.bIsHitSuperArmor;
		*this >> theRequestProjectileHitBody.dwProjectileHash;
		*this >> theRequestProjectileHitBody.bFirstHit;
		*this >> theRequestProjectileHitBody.dwActionHash;
		*this >> theRequestProjectileHitBody.dwSkillHash;
		*this >> theRequestProjectileHitBody.dwSkillLevel;
		*this >> theRequestProjectileHitBody.fProjectileScalex;
		*this >> theRequestProjectileHitBody.fProjectileScaley;
		*this >> theRequestProjectileHitBody.fProjectileScalez;
		*this >> theRequestProjectileHitBody.dwTimeStamp;



	}

	assert(theRequestProjectileHitHeader.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_UseSkill(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_UseSkill*********************");
	SEND_ACTION_SYSTEM_USESKILL theUseSkill;

	*this = (pszSendPacket);
	*this >> theUseSkill.theHeader.wPacketLen;
	*this >> theUseSkill.theHeader.wMethodType;
	*this >> theUseSkill.theHeader.wUrlType;
	*this >> theUseSkill.dwSkillHash;
	*this >> theUseSkill.wParam;
	*this >> theUseSkill.wassociative_array;
	*this >> theUseSkill.dwsize;
	for (DWORD i=0; i<theUseSkill.dwsize; i++)
	{
		*this >> theUseSkill.theParam[i].witem_first_0Len;
		*this >> theUseSkill.theParam[i].stritem_first_0;
		*this >> theUseSkill.theParam[i].witem_second_0Len;
		*this >> theUseSkill.theParam[i].stritem_second_0;
	}
	*this >> theUseSkill.wSelectedSkillTabIndex;
	*this >> theUseSkill.fMaxDt;
	

	assert(theUseSkill.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));

	return true;
}

bool SendPacket::UnPacket_SkillActionSyncPacket(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_SkillActionSyncPacket*********************");
	SKILL_ACTION_SYNC_PACKET theSkillActionSyncPacket;

	*this = (pszSendPacket);
	*this >> theSkillActionSyncPacket.theHeader.wPacketLen;
	*this >> theSkillActionSyncPacket.theHeader.wMethodType;
	*this >> theSkillActionSyncPacket.theHeader.wUrlType;
	*this >> theSkillActionSyncPacket.dwSerial;
	*this >> theSkillActionSyncPacket.dwActionSerial;
	*this >> theSkillActionSyncPacket.dwActionHash;
	*this >> theSkillActionSyncPacket.bExistParams;

	if( theSkillActionSyncPacket.bExistParams == 1 )
	{
		*this >> theSkillActionSyncPacket.theExistParams.wParams;
		*this >> theSkillActionSyncPacket.theExistParams.wassociative_array;
		*this >> theSkillActionSyncPacket.theExistParams.dwsize;

		for (DWORD i=0; i<theSkillActionSyncPacket.theExistParams.dwsize; i++)
		{
			*this >> theSkillActionSyncPacket.theExistParams.theParams[i].witem_first_0Len;
			*this >> theSkillActionSyncPacket.theExistParams.theParams[i].stritem_first_0;
			*this >> theSkillActionSyncPacket.theExistParams.theParams[i].witem_second_0Len;
			*this >> theSkillActionSyncPacket.theExistParams.theParams[i].stritem_second_0;
		}
	}

	*this >> theSkillActionSyncPacket.fActionStartPosition_X;
	*this >> theSkillActionSyncPacket.fActionStartPosition_Z;
	*this >> theSkillActionSyncPacket.fActionStartPosition_Y;
	*this >> theSkillActionSyncPacket.wDirectionX;
	*this >> theSkillActionSyncPacket.wDirectionY;
	*this >> theSkillActionSyncPacket.wMovingDirectionX;
	*this >> theSkillActionSyncPacket.wMovingDirectionY;
	*this >> theSkillActionSyncPacket.fClientAbsoluteTime;
	*this >> theSkillActionSyncPacket.dwRandomSeed;
	*this >> theSkillActionSyncPacket.dwPrevActionSerial;
	*this >> theSkillActionSyncPacket.dwPrevActionPlayTime;
	*this >> theSkillActionSyncPacket.bSkipValidation1;
	*this >> theSkillActionSyncPacket.dwSkillId;
	*this >> theSkillActionSyncPacket.wSkillLevel;
	*this >> theSkillActionSyncPacket.bSkipValidation2;

	assert(theSkillActionSyncPacket.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_ActionSyncPacket(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_ActionSyncPacket*********************");

	SEND_ACTION_SYNC_PACKET	theActionSyncPacket;
	*this = (pszSendPacket);
	*this >> theActionSyncPacket.theHeader.wPacketLen;
	*this >> theActionSyncPacket.theHeader.wMethodType;
	*this >> theActionSyncPacket.theHeader.wUrlType;
	*this >> theActionSyncPacket.dwSerial;
	*this >> theActionSyncPacket.dwActionSerial;
	*this >> theActionSyncPacket.dwActionHash;
	*this >> theActionSyncPacket.bExistParams;
	if( theActionSyncPacket.bExistParams == 1 )
	{
		*this >> theActionSyncPacket.wParams;
		*this >> theActionSyncPacket.wassociative_array;
		*this >> theActionSyncPacket.dwsize;
		for (DWORD i = 0; i<theActionSyncPacket.dwsize; i++)
		{
			*this >> theActionSyncPacket.Item[i].witem_first_iLen;
			*this >> theActionSyncPacket.Item[i].stritem_first_i;
			*this >> theActionSyncPacket.Item[i].witem_second_iLen;
			*this >> theActionSyncPacket.Item[i].stritem_second_i;
		}
	}

	*this >> theActionSyncPacket.theActionStartPosition.fx;
	*this >> theActionSyncPacket.theActionStartPosition.fz;
	*this >> theActionSyncPacket.theActionStartPosition.fy;
	*this >> theActionSyncPacket.wDirectionX;
	*this >> theActionSyncPacket.wDirectionY;
	*this >> theActionSyncPacket.wMovingDirectionX;
	*this >> theActionSyncPacket.wMovingDirectionY;
	*this >> theActionSyncPacket.fClientAbsoluteTime;
	*this >> theActionSyncPacket.dwRandomSeed;
	*this >> theActionSyncPacket.dwPrevActionSerial;
	*this >> theActionSyncPacket.fPrevActionPlayTime;
	*this >> theActionSyncPacket.bSkipValidation;

	assert(theActionSyncPacket.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));

	static DWORD dwSerial = 0;

	if( dwSerial == 0 )
	{
		JFZRRole theRole;
		theRole.UpData();

		dwSerial = theRole.m_dwSerial;
	}

	extern bool g_bChangeSectorFlag;
	if( g_bChangeSectorFlag == true )
	{
		if( theActionSyncPacket.dwSerial != dwSerial )
		{
			OutputDebugString("[JFZR] 禁止发送NPC当前位置信息!!!!");
			return false;
		}
	}


	if( theActionSyncPacket.dwSerial != dwSerial )
	{
		extern DWORD GetNpcActionHash(DWORD dwSerial,wchar_t* pActionId);

		if( g_bChangeSectorFlag == true )
		{
			if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Stand") == theActionSyncPacket.dwActionHash )
			{
				JFZRActionSystem theActionSystem;
				if( theActionSystem.NpcHijacketActionSyncPacket(&theActionSyncPacket) )
				{
					try
					{
						GetTimeStampAddr();
					}
					catch (...)
					{
						assert(false);
					}


					JFZRActionSystem theActionSystem;
					theActionSystem.actionuseskill("HandShot",0,0,theActionSyncPacket.dwSerial);
					//theActionSystem.actionuseskill("Stinger",0,0,theActionSyncPacket.dwSerial);
					return false;

				}
			}
		}

		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Stand") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] Stand!!!!!!!");
		}
		
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Realize") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] Realize!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Standby_Hold_Realize") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] Standby_Hold_Realize!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"RandomRun") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] RandomRun!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Run") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] Run!!!!!!!");
		}
	
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"FastRun") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] FastRun!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"StandBy_ShoulderHang") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] StandBy_ShoulderHang!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Standby_Hold") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] Standby_Hold!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Standby_IdleAction_a") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] Standby_IdleAction_a!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Standby_IdleAction_b") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] Standby_IdleAction_a!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"Standby_IdleAction") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] Standby_IdleAction!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"BullDog_Alert") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] BullDog_Alert!!!!!!!");
		}
		else if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"DownStart") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] DownStart!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"DeadDownStart") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] DeadDownStart!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"DownAir") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] DownAir!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"DownEnd") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] DownEnd!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"DownFinish") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] DownFinish!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"StandBy_SitDownSleep") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] StandBy_SitDownSleep!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"StandBy_ShoulderHang_Realize") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] StandBy_ShoulderHang_Realize!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"RightWalk") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] RightWalk!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"BackWalk") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] BackWalk!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"LeftRun") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] LeftRun!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"WeakAttack_A") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] WeakAttack_A!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"AiActionC") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] AiActionC!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"IdleStand") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] IdleStand!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"StandBy_Run_Realize") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] StandBy_Run_Realize!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"StandBy_Up") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] StandBy_Up!!!!!!!");
		}
		if( GetNpcActionHash(theActionSyncPacket.dwSerial,L"StandBy_Up_Realize") == theActionSyncPacket.dwActionHash )
		{
			OutputDebugString("[JFZR] StandBy_Up_Realize!!!!!!!");
		}
		





	}

	return true;
}


bool SendPacket::UnPacket_ProjectileSerialBound(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_ProjectileSerialBound*********************");
	struct  
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		WORD	wProjectileSerialBounds;
		WORD	warray;
		DWORD	dwsize;
		struct  
		{
			DWORD	dwItem_i;
		}ITEM[100];
	}USE_SKILL;

	*this = (pszSendPacket);
	*this >> USE_SKILL.wPacketLen;
	*this >> USE_SKILL.wMethodType;
	*this >> USE_SKILL.wUrlType;
	*this >> USE_SKILL.wProjectileSerialBounds;
	*this >> USE_SKILL.warray;
	*this >> USE_SKILL.dwsize;

	for (DWORD i=0; i<USE_SKILL.dwsize; i++)
	{
		*this >> USE_SKILL.ITEM[i].dwItem_i;
	}

	//assert(USE_SKILL.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}


bool SendPacket::UnPacket_DestroyProjectile(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_DestroyProjectile*********************");
	struct  
	{
		WORD	wPacketLen;
		WORD	wMethodType;			// 方法类型，比如 http中的 get,post,put,delete
		WORD	wUrlType;				// 请求实现方法具体类型
		WORD	wProjectileSerials;
		WORD	warray;
		DWORD	dwsize;
		struct  
		{
			DWORD	dwItem_i;
		}ITEM[100];
	}USE_SKILL;

	*this = (pszSendPacket);
	*this >> USE_SKILL.wPacketLen;
	*this >> USE_SKILL.wMethodType;
	*this >> USE_SKILL.wUrlType;
	*this >> USE_SKILL.wProjectileSerials;
	*this >> USE_SKILL.warray;
	*this >> USE_SKILL.dwsize;

	for (DWORD i=0; i<USE_SKILL.dwsize; i++)
	{
		*this >> USE_SKILL.ITEM[i].dwItem_i;
	}

	//assert(USE_SKILL.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_Activity(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_Activity*********************");
	SEND_ACTOR_SYSTEM_ACTIVITY	theActivity = {0};

	*this = (pszSendPacket);
	*this >> theActivity.theHeader.wPacketLen;
	*this >> theActivity.theHeader.wMethodType;
	*this >> theActivity.theHeader.wUrlType;
	*this >> theActivity.dwType;
	*this >> theActivity.dwCount;
	*this >> theActivity.dwDefenderSerial;
	*this >> theActivity.dwAttackerSerial;

	assert(theActivity.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_StartStage(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_StartStage*********************");

	SEND_STAGE_SYSTEM_START_STAGE	theStartStage;

	*this = (pszSendPacket);
	*this >> theStartStage.theHeader.wPacketLen;
	*this >> theStartStage.theHeader.wMethodType;
	*this >> theStartStage.theHeader.wUrlType;
	*this >> theStartStage.dwStageHash;
	*this >> theStartStage.dwDifficulty;
	*this >> theStartStage.bIsFirstPlay;
	*this >> theStartStage.wWarpGateIdLen;
	*this >> theStartStage.strWarpGateId;
	*this >> theStartStage.bIsRestart;
	*this >> theStartStage.bUseIntrusionItem;
	*this >> theStartStage.bIsMaxLevelExpendTutorialPlay;

	assert(theStartStage.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));

	return true;
}

bool SendPacket::UnPacket_StepInWarpGate(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_StepInWarpGate*********************");

	SEND_STEP_IN_WARP_GATE	theStepInWarpGate;
	*this = (pszSendPacket);
	*this >> theStepInWarpGate.theHeader.wPacketLen;
	*this >> theStepInWarpGate.theHeader.wMethodType;
	*this >> theStepInWarpGate.theHeader.wUrlType;
	*this >> theStepInWarpGate.wPlayerIdLen;
	*this >> theStepInWarpGate.strPlayerId;
	*this >> theStepInWarpGate.bStepIn;
	*this >> theStepInWarpGate.wWarpGateIdLen;
	*this >> theStepInWarpGate.strWarpGateId;

	assert(theStepInWarpGate.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));

	return false;
}

bool SendPacket::UnPacket_DirectionSyncPacket(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_DirectionSyncPacket*********************");

	SEND_DIRECTION_SYNC_PACKET	theDirectionSyncPacket;

	*this = (pszSendPacket);
	*this >> theDirectionSyncPacket.theHeader.wPacketLen;
	*this >> theDirectionSyncPacket.theHeader.wMethodType;
	*this >> theDirectionSyncPacket.theHeader.wUrlType;
	*this >> theDirectionSyncPacket.dwSerial;

// 	JFZRActionSystem	theActionSystem;
// 	if( theDirectionSyncPacket.dwSerial == 
// 		theActionSystem.PassivePickUpItem(theDirectionSyncPacket.thePosition.fx,theDirectionSyncPacket.thePosition.fy,theDirectionSyncPacket.thePosition.fz) )
// 	{
// 		*this << theDirectionSyncPacket.thePosition.fx;
// 		*this << theDirectionSyncPacket.thePosition.fz;
// 		*this << theDirectionSyncPacket.thePosition.fy;
// 	}
// 	else
	{
		*this >> theDirectionSyncPacket.thePosition.fx;
		*this >> theDirectionSyncPacket.thePosition.fz;
		*this >> theDirectionSyncPacket.thePosition.fy;
	}

	*this >> theDirectionSyncPacket.wDirectionX;
	*this >> theDirectionSyncPacket.wDirectionY;
	*this >> theDirectionSyncPacket.wMovingDirectionX;
	*this >> theDirectionSyncPacket.wMovingDirectionY;

	assert(theDirectionSyncPacket.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));

	return true;
}


bool SendPacket::UnPacket_ChangeSector(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_ChangeSector*********************");

	SEND_CHANGE_SECTOR	theChangeSector;

	*this = (pszSendPacket);
	*this >> theChangeSector.theHeader.wPacketLen;
	*this >> theChangeSector.theHeader.wMethodType;
	*this >> theChangeSector.theHeader.wUrlType;
	*this >> theChangeSector.wWarpGateIdLen;
	*this >> theChangeSector.strWarpGateId;
	*this >> theChangeSector.fAttackSpeed;
	*this >> theChangeSector.fMoveSpeed;
	*this >> theChangeSector.fEvadeRating;
	*this >> theChangeSector.bIsDummyExist;
	*this >> theChangeSector.dwValidationData_Y;
	*this >> theChangeSector.dwValidationData_X;

	assert(theChangeSector.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}


bool SendPacket::UnPacket_ReturnVillage(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_ReturnVillage*********************");

	SEND_RETURN_VILLAGE	theReturnVillage;

	*this = (pszSendPacket);
	*this >> theReturnVillage.theHeader.wPacketLen;
	*this >> theReturnVillage.theHeader.wMethodType;
	*this >> theReturnVillage.theHeader.wUrlType;
	*this >> theReturnVillage.bAlone;
	*this >> theReturnVillage.dwSumOfDeltaHp;

	__asm
	{
		mov eax, theReturnVillage.dwSumOfDeltaHp
		neg eax
		mov theReturnVillage.dwSumOfDeltaHp, eax
	}

	{
		char szPrint[256];
		sprintf_s(szPrint,"[JFZR] DeltaHp = %d",theReturnVillage.dwSumOfDeltaHp);
		OutputDebugString(szPrint);
	}


	assert(theReturnVillage.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_PickUpItem(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_PickUpItem*********************");

	SEND_PICK_UP_ITEM	thePickUpItem;

	*this = (pszSendPacket);
	*this >> thePickUpItem.theHeader.wPacketLen;
	*this >> thePickUpItem.theHeader.wMethodType;
	*this >> thePickUpItem.theHeader.wUrlType;
	*this >> thePickUpItem.dwSerial;

	assert(thePickUpItem.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_RepairItem(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_RepairItem*********************");

	SEND_REPAIR_ITEM	theRepairItem;
	*this = (pszSendPacket);
	*this >> theRepairItem.theHeader.wPacketLen;
	*this >> theRepairItem.theHeader.wMethodType;
	*this >> theRepairItem.theHeader.wUrlType;
	*this >> theRepairItem.bIsEquipmentRepair;

	assert(theRepairItem.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_VillageActionSyncPacket(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_VillageActionSyncPacket*********************");

	SEND_VILLAGE_ACTION_SYNC_PACKET	theVillageActionSyncPacket;

	*this = (pszSendPacket);
	*this >> theVillageActionSyncPacket.theHeader.wPacketLen;
	*this >> theVillageActionSyncPacket.theHeader.wMethodType;
	*this >> theVillageActionSyncPacket.theHeader.wUrlType;
	*this >> theVillageActionSyncPacket.dwSerial;
	*this >> theVillageActionSyncPacket.dwActionSerial;
	*this >> theVillageActionSyncPacket.dwActionHash;
	*this >> theVillageActionSyncPacket.bExistParams;
	if( theVillageActionSyncPacket.bExistParams == 1 )
	{
		*this >> theVillageActionSyncPacket.wParams;
		*this >> theVillageActionSyncPacket.wassociative_array;
		*this >> theVillageActionSyncPacket.dwsize;

		for (DWORD i=0; i<theVillageActionSyncPacket.dwsize; i++)
		{
			*this >> theVillageActionSyncPacket.Item[i].witem_first_iLen;
			*this >> theVillageActionSyncPacket.Item[i].stritem_first_i;
			*this >> theVillageActionSyncPacket.Item[i].witem_second_iLen;
			*this >> theVillageActionSyncPacket.Item[i].stritem_second_i;
		}
	}
	
	*this >> theVillageActionSyncPacket.theActionStartPosition.fx;
	*this >> theVillageActionSyncPacket.theActionStartPosition.fz;
	*this >> theVillageActionSyncPacket.theActionStartPosition.fy;
	*this >> theVillageActionSyncPacket.wDirectionX;
	*this >> theVillageActionSyncPacket.wDirectionY;
	*this >> theVillageActionSyncPacket.wMovingDirectionX;
	*this >> theVillageActionSyncPacket.wMovingDirectionY;
	*this >> theVillageActionSyncPacket.fClientAbsoluteTime;

	assert(theVillageActionSyncPacket.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_EquipItem(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_EquipItem*********************");

	SEND_EQUIP_ITEM	theEquipItem;

	*this = (pszSendPacket);
	*this >> theEquipItem.theHeader.wPacketLen;
	*this >> theEquipItem.theHeader.wMethodType;
	*this >> theEquipItem.theHeader.wUrlType;
	*this >> theEquipItem.wEquipTabLen;
	*this >> theEquipItem.strEquipTab;
	*this >> theEquipItem.wEquipSlot;
	*this >> theEquipItem.wInventoryTabLen;
	*this >> theEquipItem.strInventoryTab;
	*this >> theEquipItem.wInvenSlot;

	assert(theEquipItem.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_UnequipItem(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_UnequipItem*********************");

	SEND_UN_EQUIP_ITEM	theUnequipItem;

	*this = (pszSendPacket);
	*this >> theUnequipItem.theHeader.wPacketLen;
	*this >> theUnequipItem.theHeader.wMethodType;
	*this >> theUnequipItem.theHeader.wUrlType;
	*this >> theUnequipItem.wEquipTabLen;
	*this >> theUnequipItem.strEquipTab;
	*this >> theUnequipItem.wEquipSlot;
	*this >> theUnequipItem.wInventoryTabLen;
	*this >> theUnequipItem.strInventoryTab;
	*this >> theUnequipItem.wInvenSlot;

	assert(theUnequipItem.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_UseForItem(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_UseForItem*********************");

	SEND_USE_FOR_ITEM	theUseForItem;

	*this = (pszSendPacket);
	*this >> theUseForItem.theHeader.wPacketLen;
	*this >> theUseForItem.theHeader.wMethodType;
	*this >> theUseForItem.theHeader.wUrlType;
	*this >> theUseForItem.dwUsingTabIndex;
	*this >> theUseForItem.wUsingSlotIndex;
	*this >> theUseForItem.dwTargetTabIndex;
	*this >> theUseForItem.wTargetSlotIndex;

	assert(theUseForItem.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_BuyItem(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_BuyItem*********************");

	SEND_BUY_ITEM	theBuyItem;

	*this = (pszSendPacket);
	*this >> theBuyItem.theHeader.wPacketLen;
	*this >> theBuyItem.theHeader.wMethodType;
	*this >> theBuyItem.theHeader.wUrlType;
	*this >> theBuyItem.dwFixedNpcHash;
	*this >> theBuyItem.wSellId;
	*this >> theBuyItem.wCount;

	assert(theBuyItem.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_SellItem(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_SellItem*********************");

	SEND_SELL_ITEM	theSellItem;

	*this = (pszSendPacket);
	*this >> theSellItem.theHeader.wPacketLen;
	*this >> theSellItem.theHeader.wMethodType;
	*this >> theSellItem.theHeader.wUrlType;
	*this >> theSellItem.dwFixedNpcHash;
	*this >> theSellItem.dwTabCategory;
	*this >> theSellItem.dwSlotIndex;
	*this >> theSellItem.dwCount;

	assert(theSellItem.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_ChangeVillage(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_ChangeVillage*********************");

	SEND_CHANGE_VILLGAE	theChangeVillage;

	*this = (pszSendPacket);
	*this >> theChangeVillage.theHeader.wPacketLen;
	*this >> theChangeVillage.theHeader.wMethodType;
	*this >> theChangeVillage.theHeader.wUrlType;
	*this >> theChangeVillage.wWarpGateIdLen;
	*this >> theChangeVillage.strWarpGateId;

	assert(theChangeVillage.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_LearnSkillMultiple(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_LearnSkillMultiple*********************");

	SEND_LEARN_SKILL_MULTIPLE	theLearnSkillMultiple;

	*this = (pszSendPacket);
	*this >> theLearnSkillMultiple.theHeader.wPacketLen;
	*this >> theLearnSkillMultiple.theHeader.wMethodType;
	*this >> theLearnSkillMultiple.theHeader.wUrlType;
	*this >> theLearnSkillMultiple.dwSkillSet;
	*this >> theLearnSkillMultiple.wRequestLearnSkillList;
	*this >> theLearnSkillMultiple.warray;
	*this >> theLearnSkillMultiple.dwsize;
	for (DWORD i = 0; i< theLearnSkillMultiple.dwsize; i++)
	{
		*this >> theLearnSkillMultiple.Item[i].witem_i;
		*this >> theLearnSkillMultiple.Item[i].wpair;
		*this >> theLearnSkillMultiple.Item[i].dwkey;
		*this >> theLearnSkillMultiple.Item[i].dwvalue;
	}
	
	assert(theLearnSkillMultiple.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_SelectPlayer(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_SelectPlayer*********************");

	SEND_SELECT_PLAYER	theSelectPlayer;

	*this = (pszSendPacket);
	*this >> theSelectPlayer.theHeader.wPacketLen;
	*this >> theSelectPlayer.theHeader.wMethodType;
	*this >> theSelectPlayer.theHeader.wUrlType;
	*this >> theSelectPlayer.wPlayerIdLen;
	*this >> theSelectPlayer.strPlayerId;

	assert(theSelectPlayer.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_RequestSwitchChannel(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_RequestSwitchChannel*********************");

	SEND_REQUEST_SWITCH_CHANNEL	theRequestSwitchChannel;

	*this = (pszSendPacket);
	*this >> theRequestSwitchChannel.theHeader.wPacketLen;
	*this >> theRequestSwitchChannel.theHeader.wMethodType;
	*this >> theRequestSwitchChannel.theHeader.wUrlType;
	*this >> theRequestSwitchChannel.wChannelIdLen;
	*this >> theRequestSwitchChannel.strChannelId;

	assert(theRequestSwitchChannel.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_SwitchChannel(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_SwitchChannel*********************");

	SEND_SWITCH_CHANNEL	theSwitchChannel;

	*this = (pszSendPacket);
	*this >> theSwitchChannel.theHeader.wPacketLen;
	*this >> theSwitchChannel.theHeader.wMethodType;
	*this >> theSwitchChannel.theHeader.wUrlType;
	*this >> theSwitchChannel.wChannelIdLen;
	*this >> theSwitchChannel.strChannelId;
	*this >> theSwitchChannel.wAccountIdLen;
	*this >> theSwitchChannel.strAccountId;
	*this >> theSwitchChannel.wSessionKeyLen;
	*this >> theSwitchChannel.strSessionKey;

	assert(theSwitchChannel.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_MailSend(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_MailSend*********************");

	SEND_MAIL_SEND_HEADER	theMailSendHeader;

	*this = (pszSendPacket);
	*this >> theMailSendHeader.theHeader.wPacketLen;
	*this >> theMailSendHeader.theHeader.wMethodType;
	*this >> theMailSendHeader.theHeader.wUrlType;
	*this >> theMailSendHeader.dwType;
	*this >> theMailSendHeader.dwSystemMailType;
	*this >> theMailSendHeader.wTitleLen;
	*this >> theMailSendHeader.strTitle;
	*this >> theMailSendHeader.llOidReceiver;
	*this >> theMailSendHeader.wReceiverIdLen;
	*this >> theMailSendHeader.strReceiverId;
	*this >> theMailSendHeader.wMessageLen;
	*this >> theMailSendHeader.strMessage;
	*this >> theMailSendHeader.llGold;
	*this >> theMailSendHeader.dwOidMailItemList;
	*this >> theMailSendHeader.wItems;
	*this >> theMailSendHeader.warray;
	*this >> theMailSendHeader.dwsize;

	SEND_MAIL_SEND_BODY	theMailSendBody;
	for (DWORD i=0; i<theMailSendHeader.dwsize; i++)
	{
		*this >> theMailSendBody.witem_i;
		*this >> theMailSendBody.wApple_SendingMailItem;
		*this >> theMailSendBody.wInventoryTabLen;
		*this >> theMailSendBody.strInventoryTab;
		*this >> theMailSendBody.dwItemSlotIndex;
		*this >> theMailSendBody.dwCount;
	}

	SEND_MAIL_SEND_TAIL	theMailSendTail;

	*this >> theMailSendTail.wExpireDate;
	*this >> theMailSendTail.theAppleDateTime.wApple_DateTime;
	*this >> theMailSendTail.theAppleDateTime.wYear;
	*this >> theMailSendTail.theAppleDateTime.wMonth;
	*this >> theMailSendTail.theAppleDateTime.wDay;
	*this >> theMailSendTail.theAppleDateTime.dwHour;
	*this >> theMailSendTail.theAppleDateTime.dwMinute;
	*this >> theMailSendTail.theAppleDateTime.dwSecond;
	*this >> theMailSendTail.theAppleDateTime.dwMilliSec;
	*this >> theMailSendTail.theAppleDateTime.IsInfinity;

	assert(theMailSendHeader.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_GetAttachments(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_GetAttachments*********************");

	SEND_GETATTACHMEMTS	theGetAttachments;

	*this = (pszSendPacket);
	*this >> theGetAttachments.theHeader.wPacketLen;
	*this >> theGetAttachments.theHeader.wMethodType;
	*this >> theGetAttachments.theHeader.wUrlType;
	*this >> theGetAttachments.llMailSerial;
	*this >> theGetAttachments.bIsFirst;

	assert(theGetAttachments.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_MailRemove(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_MailRemove*********************");

	SEND_MAIL_REMOVE	theMailRemove;

	*this = (pszSendPacket);
	*this >> theMailRemove.theHeader.wPacketLen;
	*this >> theMailRemove.theHeader.wMethodType;
	*this >> theMailRemove.theHeader.wUrlType;
	*this >> theMailRemove.llMailSerial;
	*this >> theMailRemove.bIsFirst;

	assert(theMailRemove.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_JoinVillage(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_JoinVillage*********************");

	SEND_JOIN_VILLAGE	theJoinVillage;

	*this = (pszSendPacket);
	*this >> theJoinVillage.theHeader.wPacketLen;
	*this >> theJoinVillage.theHeader.wMethodType;
	*this >> theJoinVillage.theHeader.wUrlType;
	*this >> theJoinVillage.bIgnoreFistPlay;
	*this >> theJoinVillage.bSwitchChannelInVillage;

	assert(theJoinVillage.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_Accept(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_Accept*********************");
	SEND_ACCEPT	theAccept;

	*this = (pszSendPacket);
	*this >> theAccept.theHeader.wPacketLen;
	*this >> theAccept.theHeader.wMethodType;
	*this >> theAccept.theHeader.wUrlType;
	*this >> theAccept.wQuestIdLen;
	*this >> theAccept.strQuestId;
	*this >> theAccept.dwRewardItemId;
	*this >> theAccept.dwInventoryTabIndex;
	*this >> theAccept.dwInventorySlotIndex;

	assert(theAccept.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_Complete(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_Complete*********************");

	SEND_COMPLETE	theComplete;

	*this = (pszSendPacket);
	*this >> theComplete.theHeader.wPacketLen;
	*this >> theComplete.theHeader.wMethodType;
	*this >> theComplete.theHeader.wUrlType;
	*this >> theComplete.wQuestIdLen;
	*this >> theComplete.strQuestId;
	*this >> theComplete.wRewardItemIdLen;
	*this >> theComplete.strRewardItemId;

	assert(theComplete.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_TeleportToVillage(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_TeleportToVillage*********************");

	SEND_TELEPORT_TO_VILLAGE	theTeleportToVillage;

	*this = (pszSendPacket);
	*this >> theTeleportToVillage.theHeader.wPacketLen;
	*this >> theTeleportToVillage.theHeader.wMethodType;
	*this >> theTeleportToVillage.theHeader.wUrlType;
	*this >> theTeleportToVillage.wQuestIdLen;
	*this >> theTeleportToVillage.strQuestId;
	*this >> theTeleportToVillage.dwState;

	assert(theTeleportToVillage.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_Disassembly(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_Disassembly*********************");

	SEND_DISASSEMBLY	theDisassembly;

	*this = (pszSendPacket);
	*this >> theDisassembly.theHeader.wPacketLen;
	*this >> theDisassembly.theHeader.wMethodType;
	*this >> theDisassembly.theHeader.wUrlType;
	*this >> theDisassembly.dwTabCategory;
	*this >> theDisassembly.dwSlotIndex;
	*this >> theDisassembly.wIsAvatarItem;

	assert(theDisassembly.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_StageReward(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_StageReward*********************");

	SEND_STAGE_REWARD	theStageReward;
	*this = (pszSendPacket);
	*this >> theStageReward.theHeader.wPacketLen;
	*this >> theStageReward.theHeader.wMethodType;
	*this >> theStageReward.theHeader.wUrlType;
	*this >> theStageReward.dwRewardIndex;

	assert(theStageReward.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_RequestProductList(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_RequestProductList*********************");

	SEND_REQUEST_PRODUCT_LIST	theRequestProductList;
	*this = (pszSendPacket);
	*this >> theRequestProductList.theHeader.wPacketLen;
	*this >> theRequestProductList.theHeader.wMethodType;
	*this >> theRequestProductList.theHeader.wUrlType;
	*this >> theRequestProductList.dwOnePageProductCount;
	*this >> theRequestProductList.dwPageNum;
	*this >> theRequestProductList.wCategoryLen;
	*this >> theRequestProductList.strCategory;
	*this >> theRequestProductList.dwGrade;
	*this >> theRequestProductList.bIsUseable;
	*this >> theRequestProductList.dwMinLv;
	*this >> theRequestProductList.dwMaxLv;
	*this >> theRequestProductList.dwMinEnhantLevel;
	*this >> theRequestProductList.dwMaxEnhantLevel;
	*this >> theRequestProductList.wKeyWordLen;
	*this >> theRequestProductList.strKeyWord;
	*this >> theRequestProductList.dwOrderType;
	*this >> theRequestProductList.wOptionList;
	*this >> theRequestProductList.wassociative_array;
	*this >> theRequestProductList.dwsize;

	assert(theRequestProductList.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_BuyProduct(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_BuyProduct*********************");

	SEND_BUY_PRODUCT	theBuyProduct;
	*this = (pszSendPacket);
	*this >> theBuyProduct.theHeader.wPacketLen;
	*this >> theBuyProduct.theHeader.wMethodType;
	*this >> theBuyProduct.theHeader.wUrlType;
	*this >> theBuyProduct.llOidMarket;
	*this >> theBuyProduct.wCount;

	assert(theBuyProduct.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_RegisterProduct(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_RegisterProduct*********************");

	SEND_REGISTER_PRODUCT	theRegisterProduct;
	*this = (pszSendPacket);
	*this >> theRegisterProduct.theHeader.wPacketLen;
	*this >> theRegisterProduct.theHeader.wMethodType;
	*this >> theRegisterProduct.theHeader.wUrlType;
	*this >> theRegisterProduct.wCategoryLen;
	*this >> theRegisterProduct.strCategory;
	*this >> theRegisterProduct.dwItemTabCategory;
	*this >> theRegisterProduct.dwItemSlotIndex;
	*this >> theRegisterProduct.llPrice;
	*this >> theRegisterProduct.dwPeriod;
	*this >> theRegisterProduct.dwCount;

	assert(theRegisterProduct.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_RequestMyProductList(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_RequestMyProductList*********************");

	SEND_REQUEST_MY_PRODUCT_LIST	theRequestMyProductList;
	*this = (pszSendPacket);
	*this >> theRequestMyProductList.theHeader.wPacketLen;
	*this >> theRequestMyProductList.theHeader.wMethodType;
	*this >> theRequestMyProductList.theHeader.wUrlType;

	assert(theRequestMyProductList.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_UnregisterProduct(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_UnregisterProduct*********************");

	SEND_UNREGISTER_PRODUCT	theUnregisterProduct;
	*this = (pszSendPacket);
	*this >> theUnregisterProduct.theHeader.wPacketLen;
	*this >> theUnregisterProduct.theHeader.wMethodType;
	*this >> theUnregisterProduct.theHeader.wUrlType;
	*this >> theUnregisterProduct.llOidMarket;

	assert(theUnregisterProduct.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_OpenItemPackage(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_OpenItemPackage*********************");

	SEND_OPEN_ITEM_PACKAGE	theOpenItemPackage;
	*this = (pszSendPacket);
	*this >> theOpenItemPackage.theHeader.wPacketLen;
	*this >> theOpenItemPackage.theHeader.wMethodType;
	*this >> theOpenItemPackage.theHeader.wUrlType;
	*this >> theOpenItemPackage.nItemPackageTabIndex;
	*this >> theOpenItemPackage.wItemPackageSlotIndex;
	*this >> theOpenItemPackage.nRequireItemTabIndex;
	*this >> theOpenItemPackage.wRequireItemSlotIndex;

	assert(theOpenItemPackage.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}


bool SendPacket::UnPacket_CreateNpcByClient(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_CreateNpcByClient*********************");

	SEND_CREATE_NPC_BY_CLIENT	theCreateNpcByClient;
	*this = (pszSendPacket);
	*this >> theCreateNpcByClient.theHeader.wPacketLen;
	*this >> theCreateNpcByClient.theHeader.wMethodType;
	*this >> theCreateNpcByClient.theHeader.wUrlType;
	*this >> theCreateNpcByClient.dwMasterSerial;
	*this >> theCreateNpcByClient.dwRequestSerial;
	*this >> theCreateNpcByClient.wNpcIdLen;
	*this >> theCreateNpcByClient.strNpcId;

	*this >> theCreateNpcByClient.thePosition.fx;
	*this >> theCreateNpcByClient.thePosition.fz;
	*this >> theCreateNpcByClient.thePosition.fy;
	*this >> theCreateNpcByClient.theDirection.fx;
	*this >> theCreateNpcByClient.theDirection.fz;
	*this >> theCreateNpcByClient.theDirection.fy;

	*this >> theCreateNpcByClient.wTeamLen;
	*this >> theCreateNpcByClient.strTeam;
	*this >> theCreateNpcByClient.bIsControllable;
	*this >> theCreateNpcByClient.bDropItemWhenDie;
	*this >> theCreateNpcByClient.bGiveExpWhenDie;
	*this >> theCreateNpcByClient.bIsStageObject;
	*this >> theCreateNpcByClient.bUninfluencedByQuest;
	*this >> theCreateNpcByClient.wDefaultActionIdLen;
	*this >> theCreateNpcByClient.strDefaultActionId;
	*this >> theCreateNpcByClient.dwNpcType;

	extern DWORD	g_dwWarpSerial;
	g_dwWarpSerial = theCreateNpcByClient.dwMasterSerial;

	assert(theCreateNpcByClient.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_UseItem(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_UseItem*********************");

	SEND_USE_ITEM	theUseItem;
	*this = (pszSendPacket);
	*this >> theUseItem.theHeader.wPacketLen;
	*this >> theUseItem.theHeader.wMethodType;
	*this >> theUseItem.theHeader.wUrlType;
	*this >> theUseItem.nTabIndex;
	*this >> theUseItem.wSlotIndex;
	assert(theUseItem.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}



bool SendPacket::UnPacket_DestroyActors(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_DestroyActors*********************");

	SEND_DESTROY_ACTORS	theDestroyActors;
	*this = (pszSendPacket);
	*this >> theDestroyActors.theHeader.wPacketLen;
	*this >> theDestroyActors.theHeader.wMethodType;
	*this >> theDestroyActors.theHeader.wUrlType;
	*this >> theDestroyActors.wSerials;
	*this >> theDestroyActors.warray;
	*this >> theDestroyActors.dwsize;
	for (DWORD i=0; i<theDestroyActors.dwsize; i++)
	{
		*this >> theDestroyActors.theSerial[i].dwSerial;
	}

	*this >> theDestroyActors.fMaxDt;
	*this >> theDestroyActors.fClientAbsoluteTime;
	*this >> theDestroyActors.wSumOfDeltaHps;
	*this >> theDestroyActors.wassociative_array;
	*this >> theDestroyActors.dwassociativesize;

	for (DWORD i=0; i<theDestroyActors.dwassociativesize; i++)
	{
		*this >> theDestroyActors.theSumOfDeltaHps[i].dwitem_first;
		*this >> theDestroyActors.theSumOfDeltaHps[i].dwitem_second;
	}

	assert(theDestroyActors.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}

bool SendPacket::UnPacket_RequestChangeHp(const BYTE* pszSendPacket)
{
	DEBUG_PRINT("*********************Send_UnPacket_RequestChangeHp*********************");

	REQUEST_CHANGE_HP	theRequestChangeHp;
	*this = (pszSendPacket);
	*this >> theRequestChangeHp.theHeader.wPacketLen;
	*this >> theRequestChangeHp.theHeader.wMethodType;
	*this >> theRequestChangeHp.theHeader.wUrlType;
	*this >> theRequestChangeHp.dwSerial;

	//*this >> theRequestChangeHp.dwDeltaHp;
	theRequestChangeHp.dwDeltaHp = -1;		// 将BUFF的伤害值改为1
	*this << theRequestChangeHp.dwDeltaHp;

	*this >> theRequestChangeHp.dwReason;
	*this >> theRequestChangeHp.dwCauserSerial;
	*this >> theRequestChangeHp.dwEventOptionSerial;
	
	assert(theRequestChangeHp.theHeader.wPacketLen == (m_lpPacketCurrent-m_lpPacketBegin));
	return true;
}
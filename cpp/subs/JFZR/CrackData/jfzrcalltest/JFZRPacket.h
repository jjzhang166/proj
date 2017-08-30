/////////////////////////////////////////////////////////////////////////////////////////
// 疾风之刃项目之发包函数声明
// 说明：所有的和封包有关的函数接口均提供在此
// by reversal
/////////////////////////////////////////////////////////////////////////////////////////


#define JFZR_REVERSAL_PACKET_SEND_HEADER				0x55e0


/////////////////////////////////////////////////////////////////////////////////////////
// 函数功能接口部分
WORD GetHash(const wchar_t* wszStr);
DWORD GetHash(const wchar_t* pszStr1, const wchar_t* pszStr2);
DWORD GetSigHash(const wchar_t* pszStr);
DWORD WINAPI JFZR_GetTickCount();
DWORD WINAPI JFZR_AllmMemory_Alloc(int nsize);
void WINAPI JFZR_AllmMemory_Free(DWORD dwAllocAddr);
const wchar_t* GetChinese(const wchar_t* ptheEnglish);
DWORD GetProjectileSerial();
DWORD GetSkillObjAddr(DWORD ptheSkillObjAddr,DWORD* dwPoint);
DWORD LockAdd(DWORD dwLockAddr);
void UserSkillLockAdd();
FLOAT GetSpeed(DWORD dwSpeedAddr);
WORD GetDirection(FLOAT fDirection);
void JFZR_GameMemcpy(DWORD lpAddr, WCHAR* pszStr);
/////////////////////////////////////////////////////////////////////////////////////////
// 封包接口部分
int WINAPI JFZR_UserSkillPacket1(
						OUT char* pOutszSendBuffer,
						DWORD dwSkillId
						);

int WINAPI JFZR_UserSkillPacket2( 
						OUT char* pOutszSendBuffer, 
						const wchar_t* pszJobName , 
						const wchar_t* pszSkillName, 
						float x, 
						float y, 
						float z, 
						DWORD dwDirectionX,
						DWORD dwDirectionY,
						FLOAT	fClientAbsoluteTime,
						DWORD	dwSkillId,
						DWORD dwSkillLevel,
						DWORD dwPrevActionPlayTime
						);

int WINAPI JFZR_UserSkillPacket3(
						OUT char* pOutszSendBuffer,
						const wchar_t* pszJobName,
						float x,
						float y,
						float z,
						DWORD dwDirectionX,
						DWORD dwDirectionY,
						FLOAT	fClientAbsoluteTime,
						DWORD dwPrevActionPlayTime
						);

int WINAPI JFZR_SkillHitPacketHeader(
									 OUT char* pOutszSendBuffer,
									 int nsize);

int WINAPI JFZR_SkillHitPacketBody(
								   OUT char* pOutszSendBuffer,
								   int nitem,					// 多个被攻击对象的序号,从0开始，相当于结构体数组的下标
								   DWORD dwProjectileSerial,	// 抛射序号，测试给0有效
								   DWORD dwDefenderSerial,		// 防御者序号
								   DWORD dwAttackerSerial,		// 攻击者序号
								   FLOAT fAttackPowerRate,		// 攻击前进速度
								   DWORD dwProjectileHash,		// 技能名的hash，但有些技能不一样，具体技能数据看提供资料
								   DWORD dwActionHash,			// 行动hash
								   DWORD dwSkillHash,			// 技能hash
								   DWORD dwSkillLevel,			// 技能等级
								   DWORD dwTimeStamp,			// 时间戳，测试给0有效
								   BYTE  bFirstHit,
								   BYTE  bIsCounterAttack,
								   WORD  wPhysicalAttribute,
								   wchar_t* pwszElementalAttribute,		// 技能元素属性
								   DWORD	dwDeltaHp
								   );

int WINAPI JFZR_StartStage(
								   OUT char* pOutszSendBuffer,
								   const wchar_t* pszStartStageName,
								   const wchar_t* pszWarpGateId,
								   int nStageLevel,
								   BYTE	bIsRestart);

int WINAPI JFZR_GotoNextRoom1(
							 OUT char* pOutszSendBuffer,
							 const wchar_t* pszRoleName,		// 玩家名称
							 const wchar_t* pszWarp				// 第几个门
							 );


int WINAPI JFZR_GotoNextRoom2(
							  OUT char* pOutszSendBuffer,
							  DWORD dwRoleSerial,
							  float x,
							  float y,
							  float z,
							  DWORD dwDirectionX,
							  DWORD dwDirectionY
							  );

int WINAPI JFZR_ChangeSector(
							 OUT char*			pOutszSendBuffer,
							 const wchar_t*	pszWarp,		// 第几个门
							 DWORD				dwValidationData_Y,
							 DWORD				dwValidationData_X,
							 BYTE				bIsDummyExist,
							 FLOAT				fAttackSpeed,
							 FLOAT				fMoveSpeed,
							 FLOAT				fEvadeRating
							  );

int WINAPI JFZR_RetCity(
						OUT char* pOutszSendBuffer
						);

int WINAPI JFZR_PickUpItem1(
						   OUT char* pOutszSendBuffer,
						   DWORD dwItemSerial		// 物品序号
						   );

int WINAPI JFZR_PickUpItem2(
							OUT char* pOutszSendBuffer,
							DWORD dwFlag
							);

int WINAPI JFZR_RepairItem(  OUT char* pOutszSendBuffer );

int WINAPI JFZR_ActionSyncPacket(
					 OUT char*	pOutszSendBuffer,
					 DWORD		dwRoleSerial,			// 人物序号
					 DWORD		dwActionHash,
					 float		fx,						// 坐标
					 float		fy,
					 float		fz,
					 float		fDirectionX,			// 方向1
					 float		fDirectionY,
					 float		fDirectionZ,
					 DWORD		dwDirectionX,			// 方向2
					 DWORD		dwDirectionY,
					 BYTE		bSkipValidation,
					 float		fPrevActionPlayTime,
					 DWORD		dwActionSerial,
					 DWORD		dwPrevActionSerial,
					 DWORD		dwRandomSeed,
					 FLOAT		fClientAbsoluteTime,
					 BYTE		bbExistParams
					 );

int WINAPI JFZR_VillageActionSyncPacket(
										OUT char*	pOutszSendBuffer,
										DWORD		dwRoleSerial,			// 人物序号
										const wchar_t*	pszJobName,			// 角色职业
										const wchar_t*	pszCmdName,			// "FastRun" 或 "Stand"
										float		fx,						// 坐标
										float		fy,
										float		fz,
										float		fDirectionX,			// 方向1
										float		fDirectionY,
										float		fDirectionZ,
										DWORD		dwDirectionX,			// 方向2
										DWORD		dwDirectionY
										);

int WINAPI JFZR_EquipItem(
						  OUT char*	pOutszSendBuffer,
						  int nEquipSlot,					// 装备槽
						  int nInvenSlot,					// 要被穿戴的物品背包槽
						  const wchar_t* pwszEquipTab,		// L"BASIC"
						  const wchar_t* pwszInvenTab		// L"EQUIPMENT_TAB"
						  );

int WINAPI JFZR_UnEquipItem(
							OUT char*	pOutszSendBuffer,
							int nEquipSlot,					// 装备槽
							int nInvenSlot,					// 没有物品的背包槽
							const wchar_t* pwszEquipTab,	// L"BASIC"
							const wchar_t* pwszInvenTab		// L"EQUIPMENT_TAB"
							);
int WINAPI JFZR_UseForItem(
						   OUT char*	pOutszSendBuffer,
						   int nUsingSlotIndex,					// 鉴定书或解封石位置
						   int nTargetSlotIndex					// 装备位置
						   );

int WINAPI JFZR_BuyItem(
						OUT char*	pOutszSendBuffer,
						const wchar_t*		pwszNpcName,// NPC名称
						DWORD		dwSellId,			// 物品ID
						int			nCount				// 物品数量
						);

int WINAPI JFZR_SellItem(
						 OUT char*	pOutszSendBuffer,
						 const wchar_t*	pwszNpcName,		// NPC名称
						 int			nTabCategory,		// 0,表示普通栏; 1,表示消耗品栏;2,表示任务栏;3,表示材料栏
						 int			nSlotIndex,			// 单个对象背包栏第几格
						 int			nCount				// 物品数量
						 );

int WINAPI JFZR_ChangeVillage(
							  OUT char*	pOutszSendBuffer,
							  const wchar_t*	pwszVillageId			// 村庄门id
							  );
int WINAPI JFZR_LearnSkill(
						   OUT char*		pOutszSendBuffer,
						   const wchar_t*	pwszSkillName,			// 技能名称
						   DWORD			dwSkillLevel			// 升级后的技能等级
							  );

int WINAPI JFZR_CreatePlayer(
							 OUT char*	pOutszSendBuffer,
							 const wchar_t*	pszRoleName,		// 创建角色的名称
							 DWORD dwPlayerGender,				// 角色性别，默认第一个都是0，此参数一直默认为0
							 DWORD dwClassType					// 职业类型 // 1.剑斗士  2.刃武者  9.控魔师
							 );

int WINAPI JFZR_SelectPlayer(
							 OUT char*		pOutszSendBuffer,
							 const wchar_t*	pszRoleName				// 角色名称
							 );

int WINAPI JFZR_DeselectPlayer(
							   OUT char*		pOutszSendBuffer
							   );
int WINAPI JFZR_ListPlayers(
							OUT char*		pOutszSendBuffer
							);

int WINAPI JFZR_RequestSwitchChannel(
									 OUT char*		pOutszSendBuffer,
									 const wchar_t*	pszChannelName				// 频道名称
									 );


int WINAPI JFZR_SwitchChannel(
							  OUT char*		pOutszSendBuffer,
							  const wchar_t*	pszChannelName,				// 频道名称
							  const wchar_t*	pszAccountId,				// QQ账号
							  const wchar_t*	pszSessionKey				// KEY
							  );

int WINAPI JFZR_MailSendHeader(
							   OUT char*			pOutszSendBuffer,
							   const wchar_t*		pszTitle,				// 邮件标题
							   const wchar_t*		pszReceiverId,			// 玩家名称
							   const wchar_t*		pszMessage,				// 邮件内容
							   UINT64				nGold,					// 邮件金币
							   int					nsize,					// 邮件物品个数
							   LONGLONG				llOidReceiver
									 );

int WINAPI JFZR_MailSendBody(
							 OUT char*			pOutszSendBuffer,
							 int					nitem,				// 第n个物品
							 wchar_t*				pwszInventoryTab,	// 装备:"EQUIPMENT_TAB" 材料:"MATERIAL_TAB"
							 int					nItemSlotIndex,		// 物品所在背包槽
							 int					nCount				// 物品数量
							 );

// 函数功能：发送邮件尾部
int WINAPI JFZR_MailSendTail(
							 OUT char*			pOutszSendBuffer
							 );

int WINAPI JFZR_GetAttachments(
							   OUT char*			pOutszSendBuffer,
							   LONGLONG		llMailSerial,
							   BYTE			bIsFirst
							   );

int WINAPI JFZR_MailRemove(
						   OUT char*			pOutszSendBuffer,
						   LONGLONG		llMailSerial,
						   BYTE			bIsFirst
						   );

int WINAPI JFZR_JoinVillage(
							OUT char*			pOutszSendBuffer
							);

// 函数功能：接收任务
int WINAPI JFZR_Accept(
					   OUT		char*		pOutszSendBuffer,
					   const	wchar_t*	pszQuestId,					// 任务id
					   DWORD				dwRewardItemId,				// 1
					   DWORD				dwInventoryTabIndex,		// -1
					   DWORD				dwInventorySlotIndex		// -1
					   );

// 函数功能：完成任务
int WINAPI JFZR_Complete(
						 OUT		char*		pOutszSendBuffer,
						 const	wchar_t*	pszQuestId,					// 任务id
						 const	wchar_t*	pszRewardItemId
						 );

int WINAPI JFZR_GiveUp(
					   OUT		char*		pOutszSendBuffer,
					   const		wchar_t*	pszQuestId					// 任务id
					   );

int WINAPI JFZR_TeleportToVillage(
								  OUT		char*		pOutszSendBuffer,
								  const		wchar_t*	pwszQuestId,
								  DWORD		dwState
								  );

int WINAPI JFZR_Disassembly(
							  OUT		char*		pOutszSendBuffer,
							  DWORD		dwTabCategory,
							  DWORD		dwSlotIndex,
							  WORD		wIsAvatarItem
							  );

int WINAPI JFZR_StageReward(
							OUT		char*		pOutszSendBuffer,
							DWORD	dwRewardIndex
							);

int WINAPI JFZR_RequestProductList(
								   OUT		char*		pOutszSendBuffer,
								   wchar_t*	pwszCategory,
								   wchar_t*	pwszKeyWord,
								   DWORD		dwPageNum
								   );

int WINAPI JFZR_BuyProduct(
						   OUT		char*		pOutszSendBuffer,
						   LONGLONG		llOidMarket,
						   DWORD		dwCount
						   );

int WINAPI JFZR_RegisterProduct(
								OUT		char*		pOutszSendBuffer,
								wchar_t*	pwszName,
								DWORD		dwItemTabCategory,
								DWORD		dwItemSlotIndex,
								LONGLONG	llPrice,
								DWORD		dwPeriod,
								DWORD		dwCount
								);

int WINAPI JFZR_RequestMyProductList(
									 OUT		char*		pOutszSendBuffer
									 );

int WINAPI JFZR_UnregisterProduct(
								  OUT		char*		pOutszSendBuffer,
								  LONGLONG	llOidMarket
									 );

int WINAPI JFZR_Activity(
						 OUT		char*		pOutszSendBuffer,
						 DWORD		dwType,
						 DWORD		dwCount,
						 DWORD		dwDefenderSerial,
						 DWORD		dwdwAttackerSerial
						 );


int WINAPI JFZR_Test(
					 OUT		char*		pOutszSendBuffer,
					 DWORD	dwIndex
					 );

int WINAPI JFZR_RequestProductInfo(
								   OUT		char*		pOutszSendBuffer,
								   DWORD		dwItemTabCategory,
								   DWORD		dwItemSlotIndex
								   );

int WINAPI JFZR_OpenItemPackage(
								OUT		char*		pOutszSendBuffer,
								int			nItemPackageTabIndex,	// 物品栏
								WORD		wItemPackageSlotIndex,	// 物品在第几格
								int			nRequireItemTabIndex,	// 配合打开物品需要的物品  -1表示没有
								WORD		wRequireItemSlotIndex	// 物品在第几格，如果不存在物品，则置0
								);
int WINAPI JFZR_UseItem(
						OUT		char*		pOutszSendBuffer,
						int			nTabIndex,	// 物品栏
						WORD		wSlotIndex	// 物品在第几格
						);

int WINAPI JFZR_CreateNpcByClient(
								  OUT		char*			pOutszSendBuffer,
								  DWORD			dwMasterSerial,
								  DWORD			dwRequestSerial
								  );

int WINAPI JFZR_DestroyActors(
							  OUT		char*		pOutszSendBuffer,
							  DWORD		dwSerial
							  );
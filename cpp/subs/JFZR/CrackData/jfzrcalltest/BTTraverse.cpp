#include "StdAfx.h"
#include "BTTraverse.h"

wchar_t* GetString(PGAME_STRING_ID ptheGameString)
{
	if( ptheGameString->str_dwStrLen <= 0 )
	{
		return L"";
	}

	if( ptheGameString->str_dwStrLen_ >= 8 )
	{
		return ptheGameString->wstr_ptr_id;
	}

	return ptheGameString->wstr_id;
}

wchar_t* GetString_(PGAME_STRING_ID_ ptheGameString)
{
	if( ptheGameString->str_dwStrLen <= 0 )
	{
		return L"";
	}

	if( ptheGameString->str_dwStrLen_ >= 8 )
	{
		return ptheGameString->wstr_ptr_id;
	}
	return ptheGameString->wstr_id;
}


wchar_t* GetVString(PGAME_STRING_VF_ID ptheGameString)
{
	if( ptheGameString->str_dwStrLen <= 0 )
	{
		return L"";
	}

	if( ptheGameString->str_dwStrLen_ >= 8 )
	{
		return ptheGameString->wstr_ptr_id;
	}
	return ptheGameString->wstr_id;
}





void Traverse( DWORD lpSubTreeRoot, DWORD dwNullNode, PFUN_PRINT pFunPrint)
{
	if ( lpSubTreeRoot != NULL )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(lpSubTreeRoot,&theNode,sizeof(theNode));

		if( lpSubTreeRoot != dwNullNode )
		{
			pFunPrint(lpSubTreeRoot);
		}

		if( lpSubTreeRoot != dwNullNode )
		{
			Traverse( theNode.str_dwNextAddrLeft,dwNullNode,pFunPrint );
			Traverse( theNode.str_dwNextAddrRight,dwNullNode,pFunPrint );
		}
	}
}

int i;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 职业任务
CString	g_csTraverseJobTask;

void TaskCollectItemPrint(DWORD lpSubTreeRoot)
{
	COLLECT_ITEM	theCollectItem = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theCollectItem,sizeof(theCollectItem));

	wchar_t	wszPrint[256] = {0};

	if( theCollectItem.str_theCollectItemNameId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"%p : Collect Item = %s, Count = %p\r\n",lpSubTreeRoot,theCollectItem.str_theCollectItemNameId.wstr_ptr_id,theCollectItem.str_dwCollectItemCount);
	}
	else
	{
		swprintf_s(wszPrint,L"%p : Collect Item = %s, Count = %p\r\n",lpSubTreeRoot,theCollectItem.str_theCollectItemNameId.wstr_id,theCollectItem.str_dwCollectItemCount);
	}
	
	g_csTraverseJobTask += wszPrint;
}	

void TaskRewardItemPrint(DWORD lpSubTreeRoot)
{
	COLLECT_ITEM	theRewardItem = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theRewardItem,sizeof(theRewardItem));
	wchar_t	wszPrint[256] = {0};
	if( theRewardItem.str_theCollectItemNameId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"%p : Reward Item = %s, Count = %p\r\n",lpSubTreeRoot,theRewardItem.str_theCollectItemNameId.wstr_ptr_id,theRewardItem.str_dwCollectItemCount);
	}
	else
	{
		swprintf_s(wszPrint,L"%p : Reward Item = %s, Count = %p\r\n",lpSubTreeRoot,theRewardItem.str_theCollectItemNameId.wstr_id,theRewardItem.str_dwCollectItemCount);
	}

	g_csTraverseJobTask += wszPrint;
}	


// 当前职业的任务遍历
void TaskPrint(DWORD lpSubTreeRoot)
{	
	g_csTraverseJobTask += "\r\n***************************************遍历当前职业所有任务*******************************************\r\n";
	TASK_OBJ	theTaskObj = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theTaskObj,sizeof(theTaskObj));

	TASK_INFO	theTaskInfo = {0};
	_ReadProcessMemory(theTaskObj.str_dwTaskObjAddr,&theTaskInfo,sizeof(theTaskInfo));

	
	wchar_t wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"[TASK] Index = %d <%p:%p> ==> Category = 0x%p, Level = %d\r\n",
							++i,lpSubTreeRoot,theTaskObj.str_dwTaskObjAddr,theTaskInfo.str_dwCategory,theTaskInfo.str_dwLevel);
	OutputDebugStringW(wszPrint);
	g_csTraverseJobTask += wszPrint;

	// 10.30增加新的任务所在区域读取方法
	if( theTaskInfo.str_theStageIds.str_dwBeginAddr != 0 )
	{
		PGAME_STRING_ID	ptheStageIds = (PGAME_STRING_ID)theTaskInfo.str_theStageIds.str_dwBeginAddr;
		swprintf_s(wszPrint,L"StageIds = %s\r\n",GetString(ptheStageIds));
		g_csTraverseJobTask += wszPrint;
	}

	if( theTaskInfo.str_theTaskId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"Id = %s\r\n",theTaskInfo.str_theTaskId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L"Id = %s\r\n",theTaskInfo.str_theTaskId.wstr_id);
	}
	g_csTraverseJobTask += wszPrint;

	if( theTaskInfo.str_theMainTitleId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"MainTitle = %s\r\n",theTaskInfo.str_theMainTitleId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L"MainTitle = %s\r\n",theTaskInfo.str_theMainTitleId.wstr_id);
	}
	g_csTraverseJobTask += wszPrint;

	if( theTaskInfo.str_theSubTitleId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"SubTitle = %s\r\n",theTaskInfo.str_theSubTitleId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L"SubTitle = %s\r\n",theTaskInfo.str_theSubTitleId.wstr_id);
	}
	g_csTraverseJobTask += wszPrint;

	if( theTaskInfo.str_theBeginDescId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"BeginDesc = %s\r\n",theTaskInfo.str_theBeginDescId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L"BeginDesc = %s\r\n",theTaskInfo.str_theBeginDescId.wstr_id);
	}
	g_csTraverseJobTask += wszPrint;

	if( theTaskInfo.str_theEndDescId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"EndDesc = %s\r\n",theTaskInfo.str_theEndDescId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L"EndDesc = %s\r\n",theTaskInfo.str_theEndDescId.wstr_id);
	}
	g_csTraverseJobTask += wszPrint;


	// 任务NPC
	ACCEPT_COMPLETE		theTaskNpcId = {0};
	_ReadProcessMemory(theTaskObj.str_dwTaskObjAddr+JFZR_REVERSAL_TASK_ACCEPT_NPC_OBJ_OFFSET___,&theTaskNpcId,sizeof(theTaskNpcId));


	if( theTaskNpcId.str_theAcceptTaskNpcId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"AcceptTaskNpc = %s",theTaskNpcId.str_theAcceptTaskNpcId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L"AcceptTaskNpc = %s",theTaskNpcId.str_theAcceptTaskNpcId.wstr_id);
	}
	g_csTraverseJobTask += wszPrint;

	if( theTaskNpcId.str_theCompleteNpcId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L" ==> CompleteNpc = %s\r\n",theTaskNpcId.str_theCompleteNpcId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L" ==> CompleteNpc = %s\r\n",theTaskNpcId.str_theCompleteNpcId.wstr_id);
	}
	g_csTraverseJobTask += wszPrint;



	// 接收条件
	ACCPET_CONDITION	theAccpetCondition = {0};
	try
	{
		_ReadProcessMemory(theTaskObj.str_dwTaskObjAddr+JFZR_REVERSAL_TASK_ACCPET_CONDITION_OFFSET___,&theAccpetCondition,sizeof(theAccpetCondition));

		swprintf_s(wszPrint,L"condition level = %f Preceding Quests = Null\r\n",theAccpetCondition.str_fLevel);		// 可接任务的等级和上一个任务的id
	}
	catch (...)
	{
		assert(false);
	}

		
	try
	{
		GAME_STRING_ID	thePrecedingQuestsId;
		if( theAccpetCondition.str_thePrecedingQuests.str_dwBeginAddr != 0 )
		{
			_ReadProcessMemory(theAccpetCondition.str_thePrecedingQuests.str_dwBeginAddr,&thePrecedingQuestsId,sizeof(thePrecedingQuestsId));
			swprintf_s(wszPrint,L"condition level = %f Preceding Quests = %s\r\n",theAccpetCondition.str_fLevel,GetString(&thePrecedingQuestsId));
		}

		g_csTraverseJobTask += wszPrint;
	}
	catch (...)
	{
		char szPrint[256];
		sprintf_s(szPrint,"[TASK] error %p",theAccpetCondition.str_thePrecedingQuests.str_dwBeginAddr);
		OutputDebugString(szPrint);
		assert(false);
	}


	// 完成条件
	COMPLETE_CONDITION	theCompleteCondition = {0};
	_ReadProcessMemory(theTaskObj.str_dwTaskObjAddr+JFZR_REVERSAL_TASK_COMPLETE_CONDITION_OFFSET___,&theCompleteCondition,sizeof(theCompleteCondition));

	// 任务打怪需求
	if( theCompleteCondition.str_theNpcConditions.str_dwBeginAddr != 0 && theCompleteCondition.str_theNpcConditions.str_dwEndAddr1 != 0 )
	{
		g_csTraverseJobTask += "任务打怪需求:\r\n";
		for (DWORD i = theCompleteCondition.str_theNpcConditions.str_dwBeginAddr; i<theCompleteCondition.str_theNpcConditions.str_dwEndAddr1; i+= sizeof(NPC_CONDITION))
		{
			NPC_CONDITION	theNpcCondition = {0};
			_ReadProcessMemory(i,&theNpcCondition,sizeof(theNpcCondition));

			if( theNpcCondition.str_theKey.str_dwStrLen > 7 )
			{
				swprintf_s(wszPrint,L"NpcConditionType = %d, Key:%s, Value = %d, BackAttackCount = %d, SpecialAttackUseCount = %d, DeadlySkillUseCount = %d",
									theNpcCondition.str_dwNpcConditionType,
									theNpcCondition.str_theKey.wstr_ptr_id,
									theNpcCondition.str_dwValue,
									theNpcCondition.str_dwBackAttackCount,
									theNpcCondition.str_dwSpecialAttackUseCount,
									theNpcCondition.str_dwDeadlySkillUseCount);
			}
			else
			{
				swprintf_s(wszPrint,L"NpcConditionType = %d, Key:%s, Value = %d, BackAttackCount = %d, SpecialAttackUseCount = %d, DeadlySkillUseCount = %d",
									theNpcCondition.str_dwNpcConditionType,
									theNpcCondition.str_theKey.wstr_id,
									theNpcCondition.str_dwValue,
									theNpcCondition.str_dwBackAttackCount,
									theNpcCondition.str_dwSpecialAttackUseCount,
									theNpcCondition.str_dwDeadlySkillUseCount);
			}

			g_csTraverseJobTask += wszPrint;

			

			// SkillUsageCounts
			if( theNpcCondition.str_theSkillUsageCounts.str_dwBeginAddr != 0 && theNpcCondition.str_theSkillUsageCounts.str_dwEndAddr1 != 0 )
			{
				// 未有实例，暂访问到此
				swprintf_s(wszPrint,L"SkillUsageCounts: %p >> %p >> %p >> %p",
										theNpcCondition.str_theSkillUsageCounts.str_dwBeginAddr,
										theNpcCondition.str_theSkillUsageCounts.str_dwEndAddr1,
										theNpcCondition.str_theSkillUsageCounts.str_dwEndAddr2,
										theNpcCondition.str_theSkillUsageCounts.str_dwCommandAddr);
				g_csTraverseJobTask += wszPrint;
			}


			// ApprovedStages
			if( theNpcCondition.str_theApprovedStages.str_dwBeginAddr != 0 && theNpcCondition.str_theApprovedStages.str_dwEndAddr1 != 0 )
			{
				GAME_STRING_ID	theApprovedStages = {0};
				_ReadProcessMemory(theNpcCondition.str_theApprovedStages.str_dwBeginAddr,&theApprovedStages,sizeof(theApprovedStages));

				if( theApprovedStages.str_dwStrLen > 7 )
				{
					swprintf_s(wszPrint,L"ApprovedStages = %s\r\n",theApprovedStages.wstr_ptr_id);
				}
				else
				{
					swprintf_s(wszPrint,L"ApprovedStages = %s\r\n",theApprovedStages.wstr_id);
				}

				g_csTraverseJobTask += wszPrint;
			}
 		}
	}

	// 任务收集物品需求
	if( theCompleteCondition.str_theItems.str_dwCount != 0 )
	{
		g_csTraverseJobTask += "任务收集物品需求:\r\n";
		NODE	theNode = {0};
		_ReadProcessMemory(theCompleteCondition.str_theItems.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

		// 遍历所有需要收集的物品对象
		Traverse(theNode.str_dwNextAddrParent,theCompleteCondition.str_theItems.str_dwBinaryTreeNullMapAddr,TaskCollectItemPrint);
	}

	// 暂时只需要做这两种任务，待增加时再增加

// 	swprintf_s(wszPrint,L"accpet VFun = 0x%p,Classes = 0x%p,Level = %f,Gold = %d,RequiredTierAttackPower = 0x%p,Awakening = 0x%p,PvpPoint = %d,PvpWinCounts = 0x%p\
// 						 \r\nPvpLoseCounts = 0x%p,PvpTotalCounts = 0x%p,Items = 0x%p,Licenses = %p,InacceptableQuests = 0x%p,PrecedingQuests = 0x%p,PrecedingQuestsTypeOr = 0x%p,\
// 						 \r\nDifficulty = %d,CompleteType = %d,NpcConditions = 0x%p,PvpConditions = 0x%p,StageConditions = 0x%p,UseItems = 0x%p,EnhanceItems = 0x%p,\
// 						 \r\nEquipmentSetInfos = 0x%p,StatusConditions = 0x%p,InventoryCapacity = 0x%p,PlayerBankCapacity = 0x%p,AccountBankCapacity = 0x%p\r\n",
// 						 theAccpetCondition.str_dwVFunAddr,
// 						 theAccpetCondition.str_dwClasses,
// 						 theAccpetCondition.str_fLevel,
// 						 theAccpetCondition.str_dwGold,
// 						 theAccpetCondition.str_dwRequiredTierAttackPower,
// 						 theAccpetCondition.str_dwAwakening,
// 						 theAccpetCondition.str_dwPvpPoint,
// 						 theAccpetCondition.str_thePvpWinCounts.str_dwBeginAddr,
// 						 theAccpetCondition.str_thePvpLoseCounts.str_dwBeginAddr,
// 						 theAccpetCondition.str_thePvpTotalCounts.str_dwBeginAddr,
// 						 theAccpetCondition.str_theItems.str_dwCount,
// 						 theAccpetCondition.str_theLicenses.str_dwBeginAddr,
// 						 theAccpetCondition.str_theInacceptableQuests.str_dwBeginAddr,
// 						 theAccpetCondition.str_thePrecedingQuests.str_dwBeginAddr,
// 						 theAccpetCondition.str_thePrecedingQuestsTypeOr.str_dwBeginAddr,
// 						 theAccpetCondition.str_dwDifficulty,
// 						 theAccpetCondition.str_dwCompleteType,
// 						 theAccpetCondition.str_theNpcConditions.str_dwBeginAddr,
// 						 theAccpetCondition.str_thePvpConditions.str_dwBeginAddr,
// 						 theAccpetCondition.str_theStageConditions.str_dwBeginAddr,
// 						 theAccpetCondition.str_theUseItems.str_dwBeginAddr,
// 						 theAccpetCondition.str_theEnhanceItems.str_dwBeginAddr,
// 						 theAccpetCondition.str_theEquipmentSetInfos.str_dwBeginAddr,
// 						 theAccpetCondition.str_theStatusConditions.str_dwBeginAddr,
// 						 theAccpetCondition.str_dwInventoryCapacity,
// 						 theAccpetCondition.str_dwPlayerBankCapacity,
// 						 theAccpetCondition.str_dwAccountBankCapacity
// 						);
// 
// 	g_csTraverseJobTask += wszPrint;


	// 选择奖励物品
	QUEST_REWARD	theQuestReward = {0};
	_ReadProcessMemory(theTaskObj.str_dwTaskObjAddr+JFZR_REVERSAL_TASK_QUEST_REWARD_OFFSET___,&theQuestReward,sizeof(theQuestReward));

	if( theQuestReward.str_theSelectableItems.str_dwCount > 0 )
	{
		g_csTraverseJobTask += "选择奖励物品:\r\n";
		NODE	theNode = {0};
		_ReadProcessMemory(theQuestReward.str_theSelectableItems.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		// 遍历奖励物品
		Traverse(theNode.str_dwNextAddrParent,theQuestReward.str_theSelectableItems.str_dwBinaryTreeNullMapAddr,TaskRewardItemPrint);
	}

	// 接收传送  5级时一个任务"临时情报驿官","Ep02Stage01_009",实际是需要移动到的城镇
	TELEPORTLOCATION	theTeleportLocation = {0};
	_ReadProcessMemory(theTaskObj.str_dwTaskObjAddr+JFZR_REVERSAL_TAKS_ACCEPT_TELEPORT_OFFSET___,&theTeleportLocation,sizeof(theTeleportLocation));
	if( theTeleportLocation.str_theVillageId.str_dwStrLen != 0 )
	{
		g_csTraverseJobTask += "接收传送:\r\n";
		if( theTeleportLocation.str_theVillageId.str_dwStrLen > 7 )
		{
			swprintf_s(wszPrint,L"VillageId = %s",theTeleportLocation.str_theVillageId.wstr_ptr_id);
		}
		else
		{
			swprintf_s(wszPrint,L"VillageId = %s",theTeleportLocation.str_theVillageId.wstr_id);
		}

		g_csTraverseJobTask += wszPrint;
	}
}

CString TraverseJobTask(void)
{
	// 遍历所有任务
	BINARY_TREE_MAP		theTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_TASK_OBJ_ADDR1,&theTreeMap,sizeof(theTreeMap));
	

	g_csTraverseJobTask = "";
	i = 0;

	if( theTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		assert(theTreeMap.str_dwBinaryTreeNullMapAddr);
		_ReadProcessMemory(theTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		assert(theNode.str_dwNextAddrParent);

		// 遍历当前角色职业详细的任务对象
		Traverse(theNode.str_dwNextAddrParent,theTreeMap.str_dwBinaryTreeNullMapAddr,TaskPrint);

		g_csTraverseJobTask += "\r\n";
	}

	return g_csTraverseJobTask;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString	g_csPrintAllTask;


typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theGameTaskId;
	GAME_STRING_ID		str_theGameTaskNpcId;
}ALL_TASK,*PALL_TASK;

// 所有职业的任务遍历
void TraverseAllTaskPrint(DWORD lpSubTreeRoot)
{
	ALL_TASK  theTask;
	_ReadProcessMemory(lpSubTreeRoot,&theTask,sizeof(theTask));

	wchar_t		wszPrint[256] = {0};
	if( theTask.str_theGameTaskId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"[TASK] Index = %d <%p> ==> task id: %s",++i,lpSubTreeRoot,theTask.str_theGameTaskId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L"[TASK] Index = %d <%p> ==> task id: %s",++i,lpSubTreeRoot,theTask.str_theGameTaskId.wstr_id);
	}

	g_csPrintAllTask += wszPrint;

	if( theTask.str_theGameTaskNpcId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L" ==> npc id: %s\r\n",theTask.str_theGameTaskNpcId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L" ==> npc id: %s\r\n",theTask.str_theGameTaskNpcId.wstr_id);
	}

	g_csPrintAllTask += wszPrint;
}

CString TraverseAllTask(void)
{
	BINARY_TREE_MAP	theTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_TASK_OBJ_ADDR2,&theTreeMap,sizeof(theTreeMap));

	i = 0;
	g_csPrintAllTask = "";

	if( theTreeMap.str_dwCount > 0 )
	{
		assert(theTreeMap.str_dwBinaryTreeNullMapAddr);
		NODE	theNode = {0};
		_ReadProcessMemory(theTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		assert(theNode.str_dwNextAddrParent);

		// 遍历所有的任务对象
		Traverse(theNode.str_dwNextAddrParent,theTreeMap.str_dwBinaryTreeNullMapAddr,TraverseAllTaskPrint);
	}

	return g_csPrintAllTask;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 已接任务
CString	g_csTraverseReceicedTask;

// 已接任务遍历
void ReceivedTaskPrint(DWORD lpSubTreeRoot)
{
	RECEIVED_TASK	theReceivedTask = {0};

	_ReadProcessMemory(lpSubTreeRoot,&theReceivedTask,sizeof(theReceivedTask));

	wchar_t		wszPrint[256] = {0};

	if( theReceivedTask.str_theGameTaskId.str_dwStrLen > 7 )
	{
		swprintf_s(wszPrint,L"[TASK] Index = %d <%p> ==> %s",++i,lpSubTreeRoot,theReceivedTask.str_theGameTaskId.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszPrint,L"[TASK] Index = %d <%p> ==> %s",++i,lpSubTreeRoot,theReceivedTask.str_theGameTaskId.wstr_id);
	}

	OutputDebugStringW(wszPrint);

	g_csTraverseReceicedTask += wszPrint;


	ACTIVE_CONDITION	theActiveConditon = {0};
	_ReadProcessMemory(lpSubTreeRoot+JFZR_REVERSAL_TASK_ACTIVE_CONDITION_OFFSET___,&theActiveConditon,sizeof(theActiveConditon));

	// Conditions=0x200 表示打怪任务,Conditions=0表示对话任务,Conditions=0x10表示收集任务   
	// Completable=1 表示任务已完成,Completable=0 表示任务未完成;
	swprintf_s(wszPrint,L" ==> Conditions = %p,Completable = %p",theActiveConditon.str_dwConditions,theActiveConditon.str_bCompletable);
	g_csTraverseReceicedTask += wszPrint;

	// 收集物品信息 已接任务处是没有表示字段的，通过遍历背包来判断收集的物品还需要多少
	// 打怪信息
	if( theActiveConditon.str_theRemainCondition.str_theRemainKillNpcInfos.str_dwBeginAddr != 0 )
	{
		g_csTraverseReceicedTask += "\r\n打怪信息:\r\n";
		for (DWORD i = theActiveConditon.str_theRemainCondition.str_theRemainKillNpcInfos.str_dwBeginAddr; 
			i<theActiveConditon.str_theRemainCondition.str_theRemainKillNpcInfos.str_dwEndAddr1; i+=sizeof(REMAIN_NPC_CONDITION))
		{
			REMAIN_NPC_CONDITION	theRemainNpcCondition = {0};
			_ReadProcessMemory(i,&theRemainNpcCondition,sizeof(theRemainNpcCondition));

			if( theRemainNpcCondition.str_theType.str_dwStrLen > 7 )
			{
				swprintf_s(wszPrint,L" ==> type:%s",theRemainNpcCondition.str_theType.wstr_ptr_id);
			}
			else
			{
				swprintf_s(wszPrint,L" ==> type:%s",theRemainNpcCondition.str_theType.wstr_id);
			}

			g_csTraverseReceicedTask += wszPrint;

			if( theRemainNpcCondition.str_dwBackAttackCount != 0 )
			{
				g_csTraverseReceicedTask += "背后攻击次数,";
			}

			if( theRemainNpcCondition.str_dwSpecialAttackUseCount != 0 )
			{
				g_csTraverseReceicedTask += "特殊攻击使用数,";
			}

			if( theRemainNpcCondition.str_dwDeadlySkillUseCount != 0 )
			{
				g_csTraverseReceicedTask += "致命的技能使用数,";
			}

			if( theRemainNpcCondition.str_theKey.str_dwStrLen > 7 )
			{
				swprintf_s(wszPrint,L" ==> npc:%s kill count = %d",theRemainNpcCondition.str_theKey.wstr_ptr_id,
					theRemainNpcCondition.str_dwValue);
			}
			else
			{
				swprintf_s(wszPrint,L" ==> npc:%s kill count = %d",theRemainNpcCondition.str_theKey.wstr_id,
					theRemainNpcCondition.str_dwValue);
			}

			g_csTraverseReceicedTask += wszPrint;

			GAME_STRING_ID	theMapNameId = {0};
			_ReadProcessMemory(theRemainNpcCondition.str_theStages.str_dwBeginAddr,&theMapNameId,sizeof(theMapNameId));

			if( theMapNameId.str_dwStrLen > 7 )
			{
				swprintf_s(wszPrint,L" ==> map:%s\r\n",theMapNameId.wstr_ptr_id);
			}
			else
			{
				swprintf_s(wszPrint,L" ==> map:%s\r\n",theMapNameId.wstr_id);
			}

			g_csTraverseReceicedTask += wszPrint;
		}
	}
	else
	{
		g_csTraverseReceicedTask += "\r\n";
	}

	
}

CString TraverseReceivedTask(void)
{
	// 遍历已接任务
	DWORD	dwReceivedTaskObjAddr = 0;
	_ReadProcessMemory(JFZR_REVERSAL_ROLE_OBJ_ADDR+JFZR_REVERSAL_RECEIVED_TASK_OBJ_OFFSET1+JFZR_REVERSAL_RECEIVED_TASK_OBJ_OFFSET2,&dwReceivedTaskObjAddr,sizeof(dwReceivedTaskObjAddr));
	assert(dwReceivedTaskObjAddr);
	BINARY_TREE_MAP	theTreeMap = {0};
	_ReadProcessMemory(dwReceivedTaskObjAddr+JFZR_REVERSAL_RECEIVED_TASK_OBJ_OFFSET3+JFZR_REVERSAL_RECEIVED_TASK_OBJ_OFFSET4,&theTreeMap,sizeof(theTreeMap));

	g_csTraverseReceicedTask = "";
	i = 0;
	if( theTreeMap.str_dwCount > 0 )
	{
		NODE	theReceived = {0};
		_ReadProcessMemory(theTreeMap.str_dwBinaryTreeNullMapAddr,&theReceived,sizeof(theReceived));
		Traverse(theReceived.str_dwNextAddrParent,theTreeMap.str_dwBinaryTreeNullMapAddr,ReceivedTaskPrint);
	}

	return g_csTraverseReceicedTask;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DWORD g_dwLocalAllObjMapAddr = 0;
DWORD g_dwNpcTaskObjAddr = 0;				// 当前人物经过的所有城镇的NPC任务对象

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theObjId;
}LOCAL_OBJ,*PLOCAL_OBJ;


CString		g_csTraverseTaskNpc;

_declspec(naked) void HookLocalAllObjMapAddr()
{
	__asm
	{
		pushfd
		pushad
	}

	__asm
	{
		mov g_dwLocalAllObjMapAddr, ecx
	}

	__asm
	{
		popad
		popfd

		LEA EDX,DWORD PTR SS:[EBP+0x10]
		PUSH EDX
		PUSH EDI

		mov eax, JFZR_REVERSAL_ALLOBJ_MAP_FIND_ADDR_HOOK
		add eax, 5
		jmp eax
	}
}

// 对象遍历
void LocalObjPrint(DWORD lpSubTreeRoot)
{
	LOCAL_OBJ	theLocalObj = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theLocalObj,sizeof(theLocalObj));

	wchar_t wszLocalName[256] = {0};

	if( theLocalObj.str_theObjId.str_dwStrLen > 7 )
	{
		wcsncpy_s(wszLocalName,theLocalObj.str_theObjId.wstr_ptr_id,theLocalObj.str_theObjId.str_dwStrLen*2);
	}
	else
	{
		wcsncpy_s(wszLocalName,theLocalObj.str_theObjId.wstr_id,theLocalObj.str_theObjId.str_dwStrLen*2);
	}

	wchar_t		wszPrint[256] = {0};
	swprintf_s(wszPrint,L"[TASK] Index = %d <%p> ==> %s\r\n",++i,lpSubTreeRoot,wszLocalName);
	OutputDebugStringW(wszPrint);

	g_csTraverseTaskNpc += wszPrint;

	if( wcscmp(wszLocalName,L"GetActiveQuests") == 0 )
	{
		g_dwNpcTaskObjAddr = lpSubTreeRoot;
	}
}



// 遍历NPC可接任务
void CanAccpetTaskPrint(DWORD lpSubTreeRoot)
{
	CAN_ACCPET_NPC_TASK	theCanAccpetTask = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theCanAccpetTask,sizeof(theCanAccpetTask));

	wchar_t wszLocalName[256] = {0};

	if( theCanAccpetTask.str_theId.str_dwStrLen > 7 )
	{
		wcsncpy_s(wszLocalName,theCanAccpetTask.str_theId.wstr_ptr_id,theCanAccpetTask.str_theId.str_dwStrLen*2);
	}
	else
	{
		wcsncpy_s(wszLocalName,theCanAccpetTask.str_theId.wstr_id,theCanAccpetTask.str_theId.str_dwStrLen*2);
	}

	wchar_t		wszPrint[256] = {0};
	swprintf_s(wszPrint,L"npc accpet quest==> %s\r\n",wszLocalName);
	OutputDebugStringW(wszPrint);

	g_csTraverseTaskNpc += wszPrint;
}

// 遍历NPC已接任务
void NpcReceivedTaskPrint(DWORD lpSubTreeRoot)
{
	CAN_ACCPET_NPC_TASK	theCanAccpetTask = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theCanAccpetTask,sizeof(theCanAccpetTask));

	wchar_t		wszPrint[256] = {0};
	swprintf_s(wszPrint,L"npc received quest==> %s\r\n",GetString(&theCanAccpetTask.str_theId));
	OutputDebugStringW(wszPrint);

	g_csTraverseTaskNpc += wszPrint;
}

// 遍历NPC可完成任务
void CanCompleteTaskPrint(DWORD lpSubTreeRoot)
{
	CAN_COMPLETE_NPC_TASK	theCanCompleteTask = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theCanCompleteTask,sizeof(theCanCompleteTask));

	wchar_t wszLocalName[256] = {0};

	if( theCanCompleteTask.str_theId.str_dwStrLen > 7 )
	{
		wcsncpy_s(wszLocalName,theCanCompleteTask.str_theId.wstr_ptr_id,theCanCompleteTask.str_theId.str_dwStrLen*2);
	}
	else
	{
		wcsncpy_s(wszLocalName,theCanCompleteTask.str_theId.wstr_id,theCanCompleteTask.str_theId.str_dwStrLen*2);
	}

	wchar_t		wszPrint[256] = {0};
	swprintf_s(wszPrint,L"npc complete quest==> %s\r\n",wszLocalName);
	OutputDebugStringW(wszPrint);

	g_csTraverseTaskNpc += wszPrint;
}



// NPC遍历
void NpcTaskPrint(DWORD lpSubTreeRoot)
{
	TASK_NPC_OBJ	theTaskNpc = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theTaskNpc,sizeof(theTaskNpc));

	wchar_t wszLocalName[256] = {0};

	if( theTaskNpc.str_theTaskNpcId.str_dwStrLen > 7 )
	{
		wcsncpy_s(wszLocalName,theTaskNpc.str_theTaskNpcId.wstr_ptr_id,theTaskNpc.str_theTaskNpcId.str_dwStrLen*2);
	}
	else
	{
		wcsncpy_s(wszLocalName,theTaskNpc.str_theTaskNpcId.wstr_id,theTaskNpc.str_theTaskNpcId.str_dwStrLen*2);
	}

	wchar_t		wszPrint[256] = {0};
	swprintf_s(wszPrint,L"[TASK] Index = %d <%p> ==> %s accpet task obj = 0x%p\r\n",++i,lpSubTreeRoot,wszLocalName,theTaskNpc.str_dwNpcTaskObjAddr);
	OutputDebugStringW(wszPrint);
	g_csTraverseTaskNpc += wszPrint;

	// NPC可接受任务
	BINARY_TREE_MAP	theAccpetTaskNpcObj = {0};
	_ReadProcessMemory(theTaskNpc.str_dwNpcTaskObjAddr+JFZR_REVERSAL_TASK_NPC_ACCPET_QUEST_OFFSET___,&theAccpetTaskNpcObj,sizeof(theAccpetTaskNpcObj));

	if( theAccpetTaskNpcObj.str_dwCount != 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theAccpetTaskNpcObj.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

		Traverse(theNode.str_dwNextAddrParent,theAccpetTaskNpcObj.str_dwBinaryTreeNullMapAddr,CanAccpetTaskPrint);
	}

	// NPC已接任务
	BINARY_TREE_MAP	theReceivedTaskNpcObj = {0};
	_ReadProcessMemory(theTaskNpc.str_dwNpcTaskObjAddr+JFZR_REVERSAL_TASK_NPC_RECEIVED_QUEST_OFFFSET___,&theReceivedTaskNpcObj,sizeof(theReceivedTaskNpcObj));

	if( theReceivedTaskNpcObj.str_dwCount != 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theReceivedTaskNpcObj.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

		Traverse(theNode.str_dwNextAddrParent,theReceivedTaskNpcObj.str_dwBinaryTreeNullMapAddr,NpcReceivedTaskPrint);
	}

	// NPC可完成任务
	BINARY_TREE_MAP	theCompleteTaskNpcObj = {0};
	_ReadProcessMemory(theTaskNpc.str_dwNpcTaskObjAddr+JFZR_REVERSAL_TASK_NPC_COMPLETE_QUEST_OFFSET___,&theCompleteTaskNpcObj,sizeof(theCompleteTaskNpcObj));
	if( theCompleteTaskNpcObj.str_dwCount != 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theCompleteTaskNpcObj.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

		Traverse(theNode.str_dwNextAddrParent,theCompleteTaskNpcObj.str_dwBinaryTreeNullMapAddr,CanCompleteTaskPrint);
	}

	// 既没有可接任务，又没有可完成任务，就表示此NPC的任务已经被接
}

// 遍历存在任务的NPC，注意，所有未到过的城镇的任务NPC是遍历不到的，只有角色到过的城镇的任务NPC才可以遍历到
CString TraverseTaskNpc(void)
{
	DWORD	dwNpcTaskObj = 0;
	g_dwLocalAllObjMapAddr = 0;
	Hook(JFZR_REVERSAL_ALLOBJ_MAP_FIND_ADDR_HOOK,HookLocalAllObjMapAddr);

	while (!g_dwLocalAllObjMapAddr)
	{
		Sleep(1);
	}

	unsigned char szCode[] = {JFZR_REVERSAL_ALLOBJ_MAP_FUND_CODE};
	_WriteProcessMemory(JFZR_REVERSAL_ALLOBJ_MAP_FIND_ADDR_HOOK,szCode,5);

	g_dwLocalAllObjMapAddr += JFZR_REVERSAL_ALLOBJ_MAP_FIN_ADDR_OFFSET;


	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(g_dwLocalAllObjMapAddr,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwCount);
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csTraverseTaskNpc = "";
		i = 0;
		g_dwNpcTaskObjAddr = 0;

		// 遍历出很多对象，其中包含NPC任务对象（一个对象，表中包含若干NPC任务对象）
		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,LocalObjPrint);


		if( g_dwNpcTaskObjAddr != 0 )
		{
			g_csTraverseTaskNpc += "任务NPC对象\r\n";
			i = 0;

			DWORD dwLocalNpcTaskObjAddr = 0;
			_ReadProcessMemory(g_dwNpcTaskObjAddr+JFZR_REVERSAL_SINGLE_TASK_OBJ_OFFSET1,&dwLocalNpcTaskObjAddr,sizeof(dwLocalNpcTaskObjAddr));
			_ReadProcessMemory(dwLocalNpcTaskObjAddr+JFZR_REVERSAL_SINGLE_TASK_OBJ_OFFSET2,&dwLocalNpcTaskObjAddr,sizeof(dwLocalNpcTaskObjAddr));
			_ReadProcessMemory(dwLocalNpcTaskObjAddr+JFZR_REVERSAL_SINGLE_TASK_OBJ_OFFSET3+JFZR_REVERSAL_SINGLE_TASK_OBJ_OFFSET4___,&dwLocalNpcTaskObjAddr,sizeof(dwLocalNpcTaskObjAddr));


			char szPrint[256] = {0};
			sprintf_s(szPrint,"%p\r\n",dwLocalNpcTaskObjAddr);

			g_csTraverseTaskNpc += szPrint;

			NODE theNode = {0};
			_ReadProcessMemory(dwLocalNpcTaskObjAddr,&theNode,sizeof(theNode));

			// 通过NPC对象，找出角色经过城镇的NPC任务对象（一个对象，单位到单个对象）
			Traverse(theNode.str_dwNextAddrParent,dwLocalNpcTaskObjAddr,NpcTaskPrint);
		}
	}
	
	return g_csTraverseTaskNpc;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////
// 遍历中英文映射
CString	g_csChineseEnglish;

typedef struct  
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theEnglish;
	GAME_STRING_ID		str_theChinese;
}ENGLISH_CHINESE,*PENGLISH_CHINESE;

void ChineseMapPrint(DWORD lpSubTreeRoot)
{
	ENGLISH_CHINESE	theEnglishChinese = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theEnglishChinese,sizeof(theEnglishChinese));
	wchar_t		wszEnglishName[0x1024] = {0};
	if( theEnglishChinese.str_theEnglish.str_dwStrLen > 7 )
	{
		swprintf_s(wszEnglishName,L"%s",theEnglishChinese.str_theEnglish.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszEnglishName,L"%s",theEnglishChinese.str_theEnglish.wstr_id);
	}

	wchar_t		wszChineseName[0x2046] = {0};
	if( theEnglishChinese.str_theChinese.str_dwStrLen > 7 )
	{
		swprintf_s(wszChineseName,L"%s",theEnglishChinese.str_theChinese.wstr_ptr_id);
	}
	else
	{
		swprintf_s(wszChineseName,L"%s",theEnglishChinese.str_theChinese.wstr_id);
	}

	wchar_t		wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"[TASK] %d : %p ==> %s >> %s\r\n",++i,lpSubTreeRoot,wszEnglishName,wszChineseName);
	g_csChineseEnglish += wszPrint;
}

CString TraverseChineseEnglish(void)
{
	DWORD	dwChineseMapObjAddr = 0;
	_ReadProcessMemory(JFZR_REVERSAL_CHINESE_ENGLISH_MAP_ADDR,&dwChineseMapObjAddr,sizeof(dwChineseMapObjAddr));
	ASSERT(dwChineseMapObjAddr);

	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(dwChineseMapObjAddr+JFZR_REVERSAL_CHINESE_ENGLISH_MAP_OFFSET___,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);
	ASSERT(theBinaryTreeMap.str_dwCount);

	NODE	theNode = {0};
	_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
	ASSERT(theNode.str_dwNextAddrParent);

	g_csChineseEnglish = "";
	i = 0;
	Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,ChineseMapPrint);

	return g_csChineseEnglish;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 技能模板
CString		g_csSdbSkill;

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theId;
	FLOAT				str_fValue;
}SKILL_DESC_PARAMS,*PSKILL_DESC_PARAMS;

void SkillDescParamsPrint(DWORD lpSubTreeRoot)
{
	SKILL_DESC_PARAMS	theSkillDescParams = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theSkillDescParams,sizeof(theSkillDescParams));

	wchar_t	wszPrint[256] = {0};
	swprintf_s(wszPrint,L"%s = %f ",GetString(&theSkillDescParams.str_theId),theSkillDescParams.str_fValue);

	g_csSdbSkill += wszPrint;
}

void SdbSkillPrint(DWORD lpSubTreeRoot)
{
	try
	{
		SDB_INDEX		theSdbSkillIndex = {0};
		_ReadProcessMemory(lpSubTreeRoot,&theSdbSkillIndex,sizeof(theSdbSkillIndex));

		SDB_SKILL theSdbSkill = {0};
		_ReadProcessMemory(theSdbSkillIndex.str_dwObjAddr,&theSdbSkill,sizeof(theSdbSkill));

		wchar_t	wszSkillName[256] = {0};
		swprintf_s(wszSkillName,L"Skill.Warrior.%s.Name",GetVString(&theSdbSkill.str_theId));

		wchar_t		wszSkillIndexPrint[0x1024] = {0};
		swprintf_s(wszSkillIndexPrint,L"Index = %d ==> hash = 0x%p, skill obj = 0x%p id = %s name = %s ElementalAttribute = %d TargetTypes = %d\r\n",
			++i,
			theSdbSkillIndex.str_dwId,
			theSdbSkillIndex.str_dwObjAddr,
			GetVString(&theSdbSkill.str_theId),
			wszSkillName,
			theSdbSkill.str_dwElementalAttribute,
			theSdbSkill.str_dwTargetTypes
			);

		g_csSdbSkill += wszSkillIndexPrint;

		swprintf_s(wszSkillIndexPrint,L"==> level Info addr = 0x%p size = 0x%p\r\n",theSdbSkill.str_theSkillInfoPerLevels.str_dwBeginAddr,sizeof(SKILL_INFO_PER_LEVEL));
		g_csSdbSkill += wszSkillIndexPrint;

		try
		{
			for (DWORD i = theSdbSkill.str_theSkillInfoPerLevels.str_dwBeginAddr; i<theSdbSkill.str_theSkillInfoPerLevels.str_dwEndAddr1; i+=sizeof(SKILL_INFO_PER_LEVEL))
			{
				SKILL_INFO_PER_LEVEL	theSkillInfoPerLevel = {0};
				_ReadProcessMemory(i,&theSkillInfoPerLevel,sizeof(theSkillInfoPerLevel));

				swprintf_s(wszSkillIndexPrint,L"==> Level = %d , MinLevelToLearn = %d , SpToLearn = %d AttackPowerRate = %f Cooldown = %f ",
					theSkillInfoPerLevel.str_dwLevel,
					theSkillInfoPerLevel.str_dwMinLevelToLearn,
					theSkillInfoPerLevel.str_dwSpToLearn,
					theSkillInfoPerLevel.str_fAttackPowerRate,
					theSkillInfoPerLevel.str_fCooldown
					);
				g_csSdbSkill += wszSkillIndexPrint;

				if( theSkillInfoPerLevel.str_theDescParams.str_dwCount > 0 )
				{
					NODE	theNode = {0};
					_ReadProcessMemory(theSkillInfoPerLevel.str_theDescParams.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
					ASSERT(theNode.str_dwNextAddrParent);

					if( theNode.str_dwNextAddrParent != 0 )
						Traverse(theNode.str_dwNextAddrParent,theSkillInfoPerLevel.str_theDescParams.str_dwBinaryTreeNullMapAddr,SkillDescParamsPrint);
				}
				g_csSdbSkill += "\r\n";
			}
		}
		catch (...)
		{
			
		}
	}
	catch (...)
	{
		
	}

	
}

CString TraverseSdbSkill(void)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_SDB_SKILL_ADDR,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csSdbSkill = "";
		i = 0;
		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,SdbSkillPrint);
	}

	return g_csSdbSkill;
}


//////////////////////////////////////////////////////////////////////////
// 通过技能id，和技能等级，获取当前角色的技能攻击力

FLOAT		g_fAttackPowerRate;
wchar_t*	g_pszSkillId;
DWORD		g_dwSkillLevel;



void SdbSkillInfo(DWORD lpSubTreeRoot)
{
	try
	{
		SDB_INDEX		theSdbSkillIndex = {0};
		_ReadProcessMemory(lpSubTreeRoot,&theSdbSkillIndex,sizeof(theSdbSkillIndex));

		SDB_SKILL theSdbSkill = {0};
		_ReadProcessMemory(theSdbSkillIndex.str_dwObjAddr,&theSdbSkill,sizeof(theSdbSkill));


		if( wcscmp(GetVString(&theSdbSkill.str_theId),g_pszSkillId) == 0 )
		{
			try
			{
				for (DWORD i = theSdbSkill.str_theSkillInfoPerLevels.str_dwBeginAddr; i<theSdbSkill.str_theSkillInfoPerLevels.str_dwEndAddr1; i+=sizeof(SKILL_INFO_PER_LEVEL))
				{
					SKILL_INFO_PER_LEVEL	theSkillInfoPerLevel = {0};
					_ReadProcessMemory(i,&theSkillInfoPerLevel,sizeof(theSkillInfoPerLevel));

					if( theSkillInfoPerLevel.str_dwLevel == g_dwSkillLevel )
					{
						assert(!g_fAttackPowerRate);
						g_fAttackPowerRate = theSkillInfoPerLevel.str_fAttackPowerRate;
					}

				}
			}
			catch (...)
			{
				assert(false);
			}
		}
	}
	catch (...)
	{
		assert(false);
	}


}

FLOAT GetSdbSkillInfo(wchar_t* pszSkillId,DWORD dwSkillLevel)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_SDB_SKILL_ADDR,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	g_fAttackPowerRate	= 0.0f;
	g_pszSkillId		= pszSkillId;
	g_dwSkillLevel		= dwSkillLevel;

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,SdbSkillInfo);
	}

	return g_fAttackPowerRate;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 遍历物品模板信息
CString	g_csIdbItmeStr;


void SdbItemPrint(DWORD lpSubTreeRoot)
{
	SDB_INDEX		theIdbItemIndex = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theIdbItemIndex,sizeof(theIdbItemIndex));

	SDB_ITEM_INFO theIdbItem = {0};
	_ReadProcessMemory(theIdbItemIndex.str_dwObjAddr,&theIdbItem,sizeof(theIdbItem));

	wchar_t		wszItemIndexPrint[0x1024] = {0};
	swprintf_s(wszItemIndexPrint,L"Index = %d ==> hash = 0x%p, item obj = 0x%p Category = %p ItemLevel = %d CanSellTotheShop = %d id = %s\r\n",
		++i,
		theIdbItemIndex.str_dwId,
		theIdbItemIndex.str_dwObjAddr,
		theIdbItem.str_dwCategory,
		theIdbItem.str_dwItemLevel,
		theIdbItem.str_bCanSellTotheShop,
		GetVString(&theIdbItem.str_theId)
		);

	g_csIdbItmeStr += wszItemIndexPrint;

	if( theIdbItem.theEquipInfo.str_bEnable == 1 )
	{
		g_csIdbItmeStr += "装备类型:\r\n";
		swprintf_s(wszItemIndexPrint,L"EquipPart = %d LimitLevel = %d MaxDurability = %f Identified = %d Sealed = %d IsTemplateItem = %d EquipCategory = %s\r\n",
						theIdbItem.theEquipInfo.str_dwEquipPart,
						theIdbItem.theEquipInfo.str_dwLimitLevel[0],
						theIdbItem.theEquipInfo.str_fMaxDurability,
						theIdbItem.theEquipInfo.str_bIdentified,
						theIdbItem.theEquipInfo.str_bSealed,
						theIdbItem.theEquipInfo.str_bIsTemplateItem,
						GetString(&theIdbItem.theEquipInfo.str_theEquipCategory));

		g_csIdbItmeStr += wszItemIndexPrint;
	}

	if( theIdbItem.theUsableInfo.str_bEnable == 1 )
	{
		g_csIdbItmeStr += "可用信息:\r\n";
		swprintf_s(wszItemIndexPrint,L"CanUseInVillage = %d UseEffect = %d TargetLimitItemLevel min = %d TargetLimitItemLevel max = %d CooldownCategory = %s\r\n",
						theIdbItem.theUsableInfo.str_bCanUseInVillage,
						theIdbItem.theUsableInfo.str_dwUseEffect,
						theIdbItem.theUsableInfo.str_dwTargetLimitItemLevel[0],
						theIdbItem.theUsableInfo.str_dwTargetLimitItemLevel[1],
						GetString(&theIdbItem.theUsableInfo.str_theCooldownCategory)
						);

		g_csIdbItmeStr += wszItemIndexPrint;
	}

}

CString TraverseSdbItem(void)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_SDB_ITEM_INFO_ADDR,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csIdbItmeStr = "";
		i = 0;
		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,SdbItemPrint);
	}
	return g_csIdbItmeStr;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Stage模板
CString	g_csSdbStageStr;


void SdbSectorPrint(DWORD lpSubTreeRoot)
{
	SDB_SECTOR	theSdbSector = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theSdbSector,sizeof(theSdbSector));

	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"Sector id = %s IsBossSector = %p SectorResetType = %p \r\n",
				GetString(&theSdbSector.str_theId),
				theSdbSector.str_bIsBossSector,
				theSdbSector.str_dwSectorResetType);

	g_csSdbStageStr += wszPrint;

	if( theSdbSector.str_theFixedNpcs.str_dwBeginAddr != 0 )
	{
		for (DWORD i = theSdbSector.str_theFixedNpcs.str_dwBeginAddr; i<theSdbSector.str_theFixedNpcs.str_dwEndAddr1; i+=sizeof(FIXD_NPC))
		{
			FIXD_NPC	theFixdNpc = {0};
			_ReadProcessMemory(i,&theFixdNpc,sizeof(theFixdNpc));
			
			swprintf_s(wszPrint,L"npc id = %s\r\n",GetString(&theFixdNpc.str_theId));
			g_csSdbStageStr += wszPrint;
		}
	}
}

void SdbStagePrint(DWORD lpSubTreeRoot)
{
	SDB_INDEX		theSdbStageIndex = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theSdbStageIndex,sizeof(theSdbStageIndex));

	SDB_STAGE_INFO theSdbStage = {0};
	_ReadProcessMemory(theSdbStageIndex.str_dwObjAddr,&theSdbStage,sizeof(theSdbStage));

	BYTE bIsArenaStage = false;		// 角斗场的标志  bIsArenaStage = true 表示为角斗场
	_ReadProcessMemory(theSdbStageIndex.str_dwObjAddr+JFZR_REVERSAL_SDB_STAGE_INFO_IS_ARENA_STAGE_OFFSET___,&bIsArenaStage,sizeof(bIsArenaStage));

	// 如果是角斗场，则 
	// id = Rg01_Arena 寒风营寨
	// id = Rg05_Arena 莱温竞技场
	// id = Rg04_Arena 冰霜獠牙演武场
	// id = Rg06_Arena 贝恩酒馆决斗场
	// id = Rg03_Arena 迈尔地下角斗场

	wchar_t		wszItemIndexPrint[0x1024] = {0};
	swprintf_s(wszItemIndexPrint,L"Index = %d ==> hash = 0x%p, item obj = 0x%p StageType = %d RecommandLevel<%d,%d> ExperiencedDifficulty = %d id = %s x = %f y = %f Fatigue = %d IsArenaStage = %d\r\n",
		++i,
		theSdbStageIndex.str_dwId,
		theSdbStageIndex.str_dwObjAddr,
		theSdbStage.str_dwStageType,
		theSdbStage.str_dwRecommandLevel[0],
		theSdbStage.str_dwRecommandLevel[1],
		theSdbStage.str_dwExperiencedDifficulty,
		GetString(&theSdbStage.str_theId),
		theSdbStage.str_fStartingPositionX,
		theSdbStage.str_fStartingPositionY,
		theSdbStage.str_dwFatigue,
		bIsArenaStage
		);

	g_csSdbStageStr += wszItemIndexPrint;
	
	// 传送门
	if( theSdbStage.str_theWarpGateLinkInfos.str_dwEndAddr1 != 0 )
	{
		g_csSdbStageStr += "传送门:\r\n";
		for (DWORD i = theSdbStage.str_theWarpGateLinkInfos.str_dwBeginAddr; i<theSdbStage.str_theWarpGateLinkInfos.str_dwEndAddr1; i+=sizeof(WARP_GATE_LINK_INFOS_NODE))
		{
			WARP_GATE_LINK_INFOS_NODE theWarpGateLinkInfosNode = {0};
			_ReadProcessMemory(i,&theWarpGateLinkInfosNode,sizeof(theWarpGateLinkInfosNode));

			WARP_GATE_LINK theWarpGateLink = {0};
			_ReadProcessMemory(theWarpGateLinkInfosNode.str_theWarpGeteLinkInfosNode.str_dwBeginAddr,&theWarpGateLink,sizeof(theWarpGateLink));

			swprintf_s(wszItemIndexPrint,L"< %s : %s : %s Enable = %d> ==> < %s : %s : %s Enable = %d>\r\n",
						GetString(&theWarpGateLink.str_theWarpGate1.str_theStageId),
						GetString(&theWarpGateLink.str_theWarpGate1.str_theSectorId),
						GetString(&theWarpGateLink.str_theWarpGate1.str_theWarpGateId),
						theWarpGateLink.str_theWarpGate1.str_bEnable,
						GetString(&theWarpGateLink.str_theWarpGate2.str_theStageId),
						GetString(&theWarpGateLink.str_theWarpGate2.str_theSectorId),
						GetString(&theWarpGateLink.str_theWarpGate2.str_theWarpGateId),
						theWarpGateLink.str_theWarpGate2.str_bEnable
						);
			g_csSdbStageStr += wszItemIndexPrint;
		}
	}

	// 房间
	if( theSdbStage.str_theSectors.str_dwCount > 0 )
	{
		g_csSdbStageStr += "房间:\r\n";
		NODE	theNode = {0};
		_ReadProcessMemory(theSdbStage.str_theSectors.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		Traverse(theNode.str_dwNextAddrParent,theSdbStage.str_theSectors.str_dwBinaryTreeNullMapAddr,SdbSectorPrint);
	}


}

CString TraverseSdbStage(void)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_SDB_STAGE_INFO_ADDR,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csSdbStageStr = "";
		i = 0;
		try
		{
			Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,SdbStagePrint);
		}
		catch (...)
		{
			ASSERT(false);
		}
	}
	return g_csSdbStageStr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 商品模板
std::wstring		g_csSdbShopInfoStr;


void SdbShopInfoPrint(DWORD lpSubTreeRoot)
{
	try
	{
		SDB_INDEX		theSdbStageIndex = {0};
		_ReadProcessMemory(lpSubTreeRoot,&theSdbStageIndex,sizeof(theSdbStageIndex));

		SDB_SHOP_INFO theSdbShopInfo = {0};
		_ReadProcessMemory(theSdbStageIndex.str_dwObjAddr,&theSdbShopInfo,sizeof(theSdbShopInfo));

		wchar_t wszPrint[1024] = {0};
		swprintf_s(wszPrint,L"Index = %d 0x%p id = %s ShopType = %d\r\n",
			++i,
			theSdbStageIndex.str_dwObjAddr,
			GetString(&theSdbShopInfo.str_theId),
			theSdbShopInfo.str_dwShopType);
		g_csSdbShopInfoStr += wszPrint;

		if( theSdbShopInfo.str_theSellItems.str_dwBeginAddr != 0 )
		{
			for (DWORD i = theSdbShopInfo.str_theSellItems.str_dwBeginAddr; i<theSdbShopInfo.str_theSellItems.str_dwEndAddr1; i+=sizeof(SELL_ITEM))
			{
				SELL_ITEM	theSellItem = {0};
				try
				{
					_ReadProcessMemory(i,&theSellItem,sizeof(theSellItem));

					swprintf_s(wszPrint,L"[JFZR] id = 0x%p OutputItemId = %s Gold = %d FriendPoint = %d GuildCoin = %d Level<%d,%d> TierAttack<%d,%d> GuildLevel<%d,%d>",
						theSellItem.str_dwId,
						GetString(&theSellItem.str_theOutputItemId),
						theSellItem.thePriceInfo.str_dwGold,
						theSellItem.thePriceInfo.str_dwFriendPoint,
						theSellItem.thePriceInfo.str_dwGuildCoin,
						theSellItem.str_dwRequiredLevelRange[0],
						theSellItem.str_dwRequiredLevelRange[1],
						theSellItem.str_dwRequiredTierAttackPowerRange[0],
						theSellItem.str_dwRequiredTierAttackPowerRange[1],
						theSellItem.str_dwRequiredGuildLevelRange[0],
						theSellItem.str_dwRequiredGuildLevelRange[1]);
					g_csSdbShopInfoStr += wszPrint;
				}
				catch (...)
				{
					assert(false);
				}
	
				try
				{
					if( theSellItem.thePriceInfo.str_theCrests.str_dwBeginAddr != 0 )
					{
						for (DWORD j=theSellItem.thePriceInfo.str_theCrests.str_dwBeginAddr; j<theSellItem.thePriceInfo.str_theCrests.str_dwEndAddr1; j+=sizeof(CREST_INFO))
						{
							CREST_INFO	theCrestInfo = {0};
							_ReadProcessMemory(j,&theCrestInfo,sizeof(theCrestInfo));
							swprintf_s(wszPrint,L" Crest<%s,%d>",GetString(&theCrestInfo.str_theId),theCrestInfo.str_dwCount);

							g_csSdbShopInfoStr += wszPrint;
						}
					}
					g_csSdbShopInfoStr += L"\r\n";
				}
				catch (...)
				{
					assert(false);
				}
		
			}
		}
	}
	catch (...)
	{
		assert(false);
	}
}


std::wstring TraverseShopInfo(void)
{
	try
	{
		BINARY_TREE_MAP	theBinaryTreeMap = {0};
		_ReadProcessMemory(JFZR_REVERSAL_SHOP_INFO_SELL_ITEM_ADDR,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
		ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

		if( theBinaryTreeMap.str_dwCount > 0 )
		{
			NODE	theNode = {0};
			_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
			ASSERT(theNode.str_dwNextAddrParent);

			g_csSdbShopInfoStr = L"";
			i = 0;
			Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,SdbShopInfoPrint);
		}
	}
	catch (...)
	{
		assert(false);
	}

	return g_csSdbShopInfoStr;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// class
CString		g_csSdbClassInfoStr;

void SkillCategoryPrint(DWORD lpSubTreeRoot)
{	
	SKILL_CATEGORY	theSkillCategory = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theSkillCategory,sizeof(theSkillCategory));

	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"objaddr = 0x%p id = %s maxlevel = %d\r\n",
		lpSubTreeRoot,
		GetString(&theSkillCategory.str_theId),
		theSkillCategory.str_dwMaxSkillLevel
		);

	g_csSdbClassInfoStr += wszPrint;
}

void SdbSkillCategoryPrint(DWORD lpSubTreeRoot)
{
	SDB_SKILL_CATEGORY	theSdbSkillCategory = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theSdbSkillCategory,sizeof(theSdbSkillCategory));

	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"objaddr = 0x%p id = %s name = %s\r\n",
						lpSubTreeRoot,
						GetString(&theSdbSkillCategory.theSkillCategory.str_theId),
						GetVString(&theSdbSkillCategory.theSkillCategory.str_theName)
						);

	g_csSdbClassInfoStr += wszPrint;

	if( theSdbSkillCategory.theSkillCategory.str_theSkills.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theSdbSkillCategory.theSkillCategory.str_theSkills.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		Traverse(theNode.str_dwNextAddrParent,theSdbSkillCategory.theSkillCategory.str_theSkills.str_dwBinaryTreeNullMapAddr,SkillCategoryPrint);
	}
}

void SdbClassInfoPrint(DWORD lpSubTreeRoot)
{
	SDB_INDEX		theSdbStageIndex = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theSdbStageIndex,sizeof(theSdbStageIndex));

	SDB_CLASS_INFO theSdbClassInfo = {0};
	_ReadProcessMemory(theSdbStageIndex.str_dwObjAddr,&theSdbClassInfo,sizeof(theSdbClassInfo));

	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"Index = %d 0x%p id = %s IsCreateableClass = %d ActionListId = %s class name = %s\r\n",
					++i,
					lpSubTreeRoot,
					GetString(&theSdbClassInfo.str_theId),
					theSdbClassInfo.str_theIsCreateableClass,
					GetString(&theSdbClassInfo.str_theActionListId),
					GetVString(&theSdbClassInfo.str_theClassName)
					);

	g_csSdbClassInfoStr += wszPrint;

	if( theSdbClassInfo.str_theSkillCategories.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theSdbClassInfo.str_theSkillCategories.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		Traverse(theNode.str_dwNextAddrParent,theSdbClassInfo.str_theSkillCategories.str_dwBinaryTreeNullMapAddr,SdbSkillCategoryPrint);
	}

	g_csSdbClassInfoStr += "装备类别:";
	for (DWORD i=theSdbClassInfo.str_theEquipCategories.str_dwBeginAddr; i<theSdbClassInfo.str_theEquipCategories.str_dwEndAddr1; i+=sizeof(GAME_STRING_ID))
	{
		GAME_STRING_ID	theEquipCategories = {0};
		_ReadProcessMemory(i,&theEquipCategories,sizeof(theEquipCategories));
		swprintf_s(wszPrint,L"%s ",GetString(&theEquipCategories));
		g_csSdbClassInfoStr += wszPrint;
	}
	g_csSdbClassInfoStr += "\r\n";

}

CString TraverseClassInfo(void)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_CLASS_INFO_ADDR,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csSdbClassInfoStr = "";
		i = 0;
		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,SdbClassInfoPrint);
	}
	return g_csSdbClassInfoStr;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 副本和城镇之间的映射关系
CString	g_csSdbRegionInfoStr;

void SdbRegionInfoPrint(DWORD lpSubTreeRoot)
{
	SDB_INDEX		theSdbStageIndex = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theSdbStageIndex,sizeof(theSdbStageIndex));

	SDB_REGION_INFO theSdbRegionInfo = {0};
	_ReadProcessMemory(theSdbStageIndex.str_dwObjAddr,&theSdbRegionInfo,sizeof(theSdbRegionInfo));

	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"Index = %d addr = 0x%p id = %s name = %s\r\n",
					++i,
					lpSubTreeRoot,
					GetString(&theSdbRegionInfo.str_theId),
					GetVString(&theSdbRegionInfo.str_theName)
					);

	g_csSdbRegionInfoStr += wszPrint;

	for (DWORD i = theSdbRegionInfo.str_theStageIds.str_dwBeginAddr; i<theSdbRegionInfo.str_theStageIds.str_dwEndAddr1; i+=sizeof(GAME_STRING_ID))
	{
		GAME_STRING_ID	theStageId = {0};
		_ReadProcessMemory(i,&theStageId,sizeof(theStageId));
		swprintf_s(wszPrint,L"stage id = %s\r\n",GetString(&theStageId));
		g_csSdbRegionInfoStr += wszPrint;
	}
	
}


CString TraverseRegionInfo(void)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_REGION_INFO_ADDR,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csSdbRegionInfoStr = "";
		i = 0;
		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,SdbRegionInfoPrint);
	}
	return g_csSdbRegionInfoStr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Actioncommands

CString	g_csActionCommandsStr;

typedef struct  
{
	NODE			str_theNode;
	GAME_STRING_ID	str_theId1;
	GAME_STRING_ID	str_theId2;
	DWORD			str_dwFlag;
}ACTION_COMMANDS,*PACTION_COMMANDS;

void ActioncommandsPrint(DWORD lpSubTreeRoot)
{
	ACTION_COMMANDS		theActionCommands = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theActionCommands,sizeof(theActionCommands));


	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"Index = %d addr = 0x%p id1 = %s id2 = %s flag = %p\r\n",
		++i,
		lpSubTreeRoot,
		GetString(&theActionCommands.str_theId1),
		GetString(&theActionCommands.str_theId2),
		theActionCommands.str_dwFlag
		);
	
	g_csActionCommandsStr += wszPrint;

}

// 018FB070  00000000
// 018FB074  097429A4
// 018FB078  00000084


CString TraverseActioncommands(void)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(0x018FB070,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csActionCommandsStr = "";
		i = 0;
		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,ActioncommandsPrint);
	}
	return g_csActionCommandsStr;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 技能多次攻击时，所需的技能hash
CString g_csSkillHitHashStr;

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theJobId;
	BINARY_TREE_MAP		str_theJobSkillHitHashBase;
}JOB_SKILL_HASH,*PJOB_SKILL_HASH;

typedef struct
{
	NODE				str_theNode;
	GAME_STRING_ID		str_theSkillId;
	DWORD				str_dwFlag;
	DWORD				str_dwId;
}SKILL_HIT_HASH,*PSKILL_HIT_HASH;

void SkillHitHashPrint(DWORD lpSubTreeRoot)
{
	PSKILL_HIT_HASH	ptheHitHash = (PSKILL_HIT_HASH)lpSubTreeRoot;

	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"hit addr = 0x%p hit hash = %s id = 0x%p flag = %d\r\n",lpSubTreeRoot,GetString(&ptheHitHash->str_theSkillId),ptheHitHash->str_dwId,ptheHitHash->str_dwFlag);
	g_csSkillHitHashStr += wszPrint;

}

void ClassSkillPrint(DWORD lpSubTreeRoot)
{
	JOB_SKILL_HASH	theJobSkillHash;
	_ReadProcessMemory(lpSubTreeRoot,&theJobSkillHash,sizeof(theJobSkillHash));

	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"addr = 0x%p job = %s :\r\n",lpSubTreeRoot,GetString(&theJobSkillHash.str_theJobId));
	g_csSkillHitHashStr += wszPrint;
	
	if( theJobSkillHash.str_theJobSkillHitHashBase.str_dwBinaryTreeNullMapAddr != 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theJobSkillHash.str_theJobSkillHitHashBase.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		Traverse(theNode.str_dwNextAddrParent,theJobSkillHash.str_theJobSkillHitHashBase.str_dwBinaryTreeNullMapAddr,SkillHitHashPrint);
	}
}

CString TraverseSkillHitHash()
{
	BINARY_TREE_MAP	theSkillHitHashBase;
	_ReadProcessMemory(JFZR_REVERSAL_SKILL_HIT_HASH_ADDR,&theSkillHitHashBase,sizeof(theSkillHitHashBase));
	assert(theSkillHitHashBase.str_dwBinaryTreeNullMapAddr);

	if( theSkillHitHashBase.str_dwCount > 0 )
	{
		g_csSkillHitHashStr = "";
		NODE	theNode = {0};
		_ReadProcessMemory(theSkillHitHashBase.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);
		Traverse(theNode.str_dwNextAddrParent,theSkillHitHashBase.str_dwBinaryTreeNullMapAddr,ClassSkillPrint);
	}
	return g_csSkillHitHashStr;
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 获取移动封包使需要的 PrevActionPlayTime

typedef void (*PFUN_PRINT_)(DWORD lpSubTreeRoot, LPVOID lpParameter);

// CString g_csPrevActionPlayTimeStr_;
DWORD	g_dwLocalPrevActionPlayTimeAddr = 0;

void Traverse_( DWORD lpSubTreeRoot, DWORD dwNullNode, LPVOID lpParameter,PFUN_PRINT_ pFunPrint)
{
	if ( lpSubTreeRoot != NULL )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(lpSubTreeRoot,&theNode,sizeof(theNode));

		if( lpSubTreeRoot != dwNullNode )
		{
			pFunPrint(lpSubTreeRoot,lpParameter);
		}

		if( lpSubTreeRoot != dwNullNode )
		{
			Traverse_( theNode.str_dwNextAddrLeft,dwNullNode,lpParameter,pFunPrint );
			Traverse_( theNode.str_dwNextAddrRight,dwNullNode,lpParameter,pFunPrint );
		}
	}
}

_declspec(naked) void HookGetLocalPrevActionPlayTimeAddr()
{
	__asm
	{
		mov g_dwLocalPrevActionPlayTimeAddr, ecx
	}
	__asm
	{
		PUSH EBP
		MOV EBP,ESP
		PUSH -0x1

		mov eax, JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_HOOK
		add eax, 5
		jmp eax
	}
}

void PrevActionPlayTimePrint2_(DWORD lpSubTreeRoot,LPVOID lpParameter)
{
	try
	{
		PPREV_ACTION_DATA	pthePrevActionData = (PPREV_ACTION_DATA)lpParameter;

		PREV_ACTION_PLAY_TIME_3	thePrevActionPlayTime_3;
		_ReadProcessMemory(lpSubTreeRoot,&thePrevActionPlayTime_3,sizeof(thePrevActionPlayTime_3));

		_ReadProcessMemory(thePrevActionPlayTime_3.dwLocalObjAddr,&pthePrevActionData->dwPrevActionSerial,sizeof(pthePrevActionData->dwPrevActionSerial));
		_WriteProcessMemory(thePrevActionPlayTime_3.dwLocalObjAddr,&pthePrevActionData->dwActionSerial,sizeof(pthePrevActionData->dwActionSerial));

		// 读取上次行动时间，此时间在一直计时
		_ReadProcessMemory(thePrevActionPlayTime_3.dwLocalObjAddr+JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET3,&pthePrevActionData->fPrevActionPlayTime,sizeof(pthePrevActionData->fPrevActionPlayTime));

		// 读完清0，必须使用才调用
		FLOAT fActionPlaytimeData = 0.0f;
		_WriteProcessMemory(thePrevActionPlayTime_3.dwLocalObjAddr+JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET3,&fActionPlaytimeData,sizeof(fActionPlaytimeData));
	}
	catch (...)
	{
		assert(false);
	}
}

void PrevActionPlayTimePrint_(DWORD lpSubTreeRoot,LPVOID lpParameter)
{
	try
	{
		PREV_ACTION_PLAY_TIME_1	thePrevActionPlayTime_1;
		_ReadProcessMemory(lpSubTreeRoot,&thePrevActionPlayTime_1,sizeof(thePrevActionPlayTime_1));

		if( thePrevActionPlayTime_1.dwLocalObjAddr == 0 )
		{
			return ;
		}

		PREV_ACTION_PLAY_TIME_2	thePrevActionPlayTime_2;
		_ReadProcessMemory(thePrevActionPlayTime_1.dwLocalObjAddr,&thePrevActionPlayTime_2,sizeof(thePrevActionPlayTime_2));

		if( thePrevActionPlayTime_2.theMapAddr.str_dwBinaryTreeNullMapAddr != 0 && thePrevActionPlayTime_2.theMapAddr.str_dwCount > 0 )
		{
			NODE theNode;
			_ReadProcessMemory(thePrevActionPlayTime_2.theMapAddr.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

// 			char szPrint[256] = {0};
// 			sprintf_s(szPrint,"[JFZR] %p %p\r\n",theNode.str_dwNextAddrParent,thePrevActionPlayTime_2.theMapAddr.str_dwBinaryTreeNullMapAddr);
// 
// 			g_csPrevActionPlayTimeStr_ += szPrint;

			PPREV_ACTION_DATA pthePrevActionData = (PPREV_ACTION_DATA)lpParameter;
			if( pthePrevActionData->dwSerial == thePrevActionPlayTime_2.dwSerial )
			{
				Traverse_(theNode.str_dwNextAddrParent,thePrevActionPlayTime_2.theMapAddr.str_dwBinaryTreeNullMapAddr,lpParameter,PrevActionPlayTimePrint2_);
			}
		}
	}
	catch (...)
	{
		assert(false);
	}
}

bool TraversePrevActionPlayTime_(PPREV_ACTION_DATA pthePrevActionData)
{
	try
	{
		//g_csPrevActionPlayTimeStr_ = "";
		g_dwLocalPrevActionPlayTimeAddr = 0;
		Hook(JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_HOOK,HookGetLocalPrevActionPlayTimeAddr);

		for (int i=0; i<100; i++)
		{
			if( g_dwLocalPrevActionPlayTimeAddr )
			{
				break;
			}
			Sleep(10);
		}

		if( !g_dwLocalPrevActionPlayTimeAddr )
		{
			return false;
		}
	}
	catch (...)
	{
		assert(false);
	}


	DWORD	dwLocalPrevActionPlayTimeAddr = 0;
	_ReadProcessMemory(g_dwLocalPrevActionPlayTimeAddr+JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET1,&dwLocalPrevActionPlayTimeAddr,sizeof(dwLocalPrevActionPlayTimeAddr));
	assert(dwLocalPrevActionPlayTimeAddr);

	BINARY_TREE_MAP thePrevActionPlayTime;
	_ReadProcessMemory(dwLocalPrevActionPlayTimeAddr+JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET2,&thePrevActionPlayTime,sizeof(thePrevActionPlayTime));


	try
	{
		if( thePrevActionPlayTime.str_dwBinaryTreeNullMapAddr != 0 && thePrevActionPlayTime.str_dwCount > 0 )
		{
			NODE theNode;
			_ReadProcessMemory(thePrevActionPlayTime.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

		
			Traverse_(theNode.str_dwNextAddrParent,thePrevActionPlayTime.str_dwBinaryTreeNullMapAddr,pthePrevActionData,PrevActionPlayTimePrint_);
		}
	}
	catch (...)
	{
		assert(false);
		return false;
	}

	return true;
}

CString g_csPrevActionPlayTimeStr;

FLOAT	g_fActionPlaytimeData = 0.0f;

void PrevActionPlayTimePrint2(DWORD lpSubTreeRoot)
{
	PREV_ACTION_PLAY_TIME_3	thePrevActionPlayTime_3;
	_ReadProcessMemory(lpSubTreeRoot,&thePrevActionPlayTime_3,sizeof(thePrevActionPlayTime_3));

	// 读取上次行动时间，此时间在一直计时
	_ReadProcessMemory(thePrevActionPlayTime_3.dwLocalObjAddr+JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET3,&g_fActionPlaytimeData,sizeof(g_fActionPlaytimeData));

	// 读完清0，必须使用才调用
	FLOAT fActionPlaytimeData = 0.0f;
	_WriteProcessMemory(thePrevActionPlayTime_3.dwLocalObjAddr+JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET3,&fActionPlaytimeData,sizeof(fActionPlaytimeData));
}

void PrevActionPlayTimePrint(DWORD lpSubTreeRoot)
{
	PREV_ACTION_PLAY_TIME_1	thePrevActionPlayTime_1;
	_ReadProcessMemory(lpSubTreeRoot,&thePrevActionPlayTime_1,sizeof(thePrevActionPlayTime_1));

	if( thePrevActionPlayTime_1.dwLocalObjAddr == 0 )
	{
		return ;
	}

	PREV_ACTION_PLAY_TIME_2	thePrevActionPlayTime_2;
	_ReadProcessMemory(thePrevActionPlayTime_1.dwLocalObjAddr,&thePrevActionPlayTime_2,sizeof(thePrevActionPlayTime_2));

	if( thePrevActionPlayTime_2.theMapAddr.str_dwBinaryTreeNullMapAddr != 0 && thePrevActionPlayTime_2.theMapAddr.str_dwCount > 0 )
	{
		JFZRRole theRole;
		theRole.UpData();

		NODE theNode;
		_ReadProcessMemory(thePrevActionPlayTime_2.theMapAddr.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

		char szPrint[256] = {0};
		sprintf_s(szPrint,"[JFZR] %p %p\r\n",theNode.str_dwNextAddrParent,thePrevActionPlayTime_2.theMapAddr.str_dwBinaryTreeNullMapAddr);
		
		g_csPrevActionPlayTimeStr += szPrint;

		if( theRole.m_dwSerial == thePrevActionPlayTime_2.dwSerial )
		{
			Traverse(theNode.str_dwNextAddrParent,thePrevActionPlayTime_2.theMapAddr.str_dwBinaryTreeNullMapAddr,PrevActionPlayTimePrint2);
		}
	}

}

FLOAT TraversePrevActionPlayTime()
{
	g_csPrevActionPlayTimeStr = "";
	g_fActionPlaytimeData = 0.0f;
	g_dwLocalPrevActionPlayTimeAddr = 0;
	Hook(JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_HOOK,HookGetLocalPrevActionPlayTimeAddr);

	for (int i=0; i<100; i++)
	{
		if( g_dwLocalPrevActionPlayTimeAddr )
		{
			break;
		}
		Sleep(10);
	}

	if( !g_dwLocalPrevActionPlayTimeAddr )
	{
		return 0.0f;
	}

	DWORD	dwLocalPrevActionPlayTimeAddr = 0;
	_ReadProcessMemory(g_dwLocalPrevActionPlayTimeAddr+JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET1,&dwLocalPrevActionPlayTimeAddr,sizeof(dwLocalPrevActionPlayTimeAddr));
	assert(dwLocalPrevActionPlayTimeAddr);

	BINARY_TREE_MAP thePrevActionPlayTime;
	_ReadProcessMemory(dwLocalPrevActionPlayTimeAddr+JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET2,&thePrevActionPlayTime,sizeof(thePrevActionPlayTime));

	try
	{
		if( thePrevActionPlayTime.str_dwBinaryTreeNullMapAddr != 0 && thePrevActionPlayTime.str_dwCount > 0 )
		{
			NODE theNode;
			_ReadProcessMemory(thePrevActionPlayTime.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));

			Traverse(theNode.str_dwNextAddrParent,thePrevActionPlayTime.str_dwBinaryTreeNullMapAddr,PrevActionPlayTimePrint);
		}
	}
	catch (...)
	{
		assert(false);
	}

	return g_fActionPlaytimeData;
}


//////////////////////////////////////////////////////////////////////////
// Projectile 模板
CString g_csProjectilePrintStr;

typedef struct
{
	NODE		theNode;
	DWORD		dwProjectileId;
	DWORD		dwProjectileAddr;
	DWORD		dwProjectilePoint;
}PROJECTILE_NODE,*PPROJECTILE_NODE;

typedef struct	// "Apple::Sdb::Projectile"
{
	DWORD			str_dwFunAddr;
	GAME_STRING_ID	str_theId;
	DWORD			str_dwprojectiletype;
	GAME_STRING_ID_	str_theModelPath;
	GAME_STRING_ID_	str_theDefaultAnimationPath;
	XYZ				str_theScale;
}SDB_PROJECTILE,*PSDB_PROJECTILE;

void ProjectilePrint(DWORD lpSubTreeRoot)
{
	PROJECTILE_NODE		theProjectileNode = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theProjectileNode,sizeof(theProjectileNode));


// 	wchar_t	wszPrint[1024] = {0};
// 	swprintf_s(wszPrint,L"Index = %d addr = 0x%p id1 = %s id2 = %s flag = %p\r\n",
// 		++i,
// 		lpSubTreeRoot,
// 		GetString(&theActionCommands.str_theId1),
// 		GetString(&theActionCommands.str_theId2),
// 		theActionCommands.str_dwFlag
// 		);

//	g_csProjectilePrintStr += wszPrint;

}

CString TraverseProjectile(void)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(JFZR_REVERSAL_PROJECTILE_ADDR,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csProjectilePrintStr = "";
		i = 0;
		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,ProjectilePrint);
	}
	return g_csProjectilePrintStr;
}


//////////////////////////////////////////////////////////////////////////
// 主线任务对应章节
CString g_csChapterPrintStr;

typedef struct
{
	NODE			theNode;
	GAME_STRING_ID	theTaskId;
	GAME_STRING_ID	theChapterId;
}CHAPTER,*PCHAPTER;

void ChapterPrint(DWORD lpSubTreeRoot)
{
	CHAPTER		theChapterNode = {0};
	_ReadProcessMemory(lpSubTreeRoot,&theChapterNode,sizeof(theChapterNode));


	wchar_t	wszPrint[1024] = {0};
	swprintf_s(wszPrint,L"addr = 0x%p task id = %s chapter id = %s\r\n",
		lpSubTreeRoot,
		GetString(&theChapterNode.theTaskId),
		GetString(&theChapterNode.theChapterId)
		);

	g_csChapterPrintStr += wszPrint;

}

CString TraverseChapter(void)
{
	BINARY_TREE_MAP	theBinaryTreeMap = {0};
	_ReadProcessMemory(0x1993128,&theBinaryTreeMap,sizeof(theBinaryTreeMap));
	ASSERT(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr);

	if( theBinaryTreeMap.str_dwCount > 0 )
	{
		NODE	theNode = {0};
		_ReadProcessMemory(theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,&theNode,sizeof(theNode));
		ASSERT(theNode.str_dwNextAddrParent);

		g_csChapterPrintStr = "";
		i = 0;
		Traverse(theNode.str_dwNextAddrParent,theBinaryTreeMap.str_dwBinaryTreeNullMapAddr,ChapterPrint);
	}
	return g_csChapterPrintStr;
}
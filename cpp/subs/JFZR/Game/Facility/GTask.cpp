#include "stdafx.h"
#include "GTask.h"
#include "GOffset.h"
#include "GAddr.h"
#include "GameObjMgr.h"
#include "GPlayer.h"

bool TaskCurJobTemplateMgr::UpdateAll()
{
	assert(false);
	if ( !btree_.empty() )
		return true;
	return __super::UpdateAll();
}

bool TaskCurJobTemplateMgr::RebuildUpdateAll()
{
	using namespace GClass;
	using namespace GAddr;
	using namespace GOffset;
	/*if ( !btree_.empty() )
		return true;*/
	const auto tree_nodes = (const stGBTreeNodes*)(g_pCurJobAllTask);
	if ( tree_nodes->node_cnt_ == btree_.size() )
		return true;
	SetGameNodes(tree_nodes);
	return GWndExecSync([this]() -> bool{
		return UpdateImpl();
	});	
}

const TaskCurJobTemplate* TaskCurJobTemplateMgr::FindByTaskId( const std::wstring& task_id ) const
{
	if (task_id.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( const auto& task_template : btree_ )
	{
		assert(task_template.handler_);
		if ( task_template.handler_->GetStrTaskId() == task_id )
			return task_template.handler_.get();
	}
	return nullptr;
}

TaskCurJobTemplate::SharedPtrT TaskCurJobTemplateMgr::FindTaskPtrById( const std::wstring& task_id ) const
{
	if (task_id.empty())
	{
		assert(false);
		return TaskCurJobTemplate::SharedPtrT();
	}
	for ( const auto& task_template : btree_ )
	{
		assert(task_template.handler_);
		if ( task_template.handler_->GetStrTaskId() == task_id )
			return task_template.handler_;
	}
	return TaskCurJobTemplate::SharedPtrT();
}

bool TaskCurJobTemplateMgr::UpdateImpl()
{
	if ( !__super::RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	assert(!btree_.empty());
	//return btree_.size() == tree_nodes->node_cnt_;
	return true;
}

struct stCurJobTaskInfo{
	typedef GClass::GGameTArray<std::vector<GClass::GGameTypePlaceholder<GClass::stGameString> > >::stGameTArray GGameTArrayPlaceholder;


	GClass::stGameStringEx		task_id_;			//任务名称id
	GGameTArrayPlaceholder		stage_ids_;			//StageId,任务所在副本，在哪个副本完成该任务
	GClass::stGameStringEx		main_title_id_;		//主标题，任务名称，任务目录里显示的任务标题
	GClass::stGameStringEx		sub_title_id_;		//副标题
	GType::enTaskCategory		task_category_;
	int							task_level_;		//任务等级，角色达到该等级才能接受该任务
	GClass::stGameStringEx		begin_desc_id_;		//任务完成前，任务右窗口的内容
	GClass::stGameStringEx		end_desc_id_;		//任务完成后，任务右窗口的内容
};
static_assert(sizeof(stCurJobTaskInfo) == sizeof(TASK_INFO), "");

struct stCurJobTask : TaskCurJobTemplate::stBTreeNodeBase{
	const DWORD				task_id_;				//任务ID
	const stCurJobTaskInfo*	task_obj_addr_;			//任务对象
	const DWORD				unknown_ptr_task_obj_;	//指向任务对象(无用)
	const bool				str_unknown_flag_;
	const void*				unknwon_str_addr_;
};
static_assert(sizeof(stCurJobTask) == sizeof(TASK_OBJ), "");

struct stAcceptCondition{
	DWORD										str_dwVFunAddr;
	union  
	{
		DWORD									str_dwClasses;
		DWORD									str_dwClassesArray[4];
	};
	union  
	{
		FLOAT									str_fLevel;
		FLOAT									str_fLevelArray[2];
	};
	DWORD										str_dwGold;
	union  
	{
		DWORD									str_dwRequiredTierAttackPower;
		DWORD									str_dwRequiredTierAttackPowerArray[2];
	};
	DWORD										str_dwAwakening;
	//DWORD										str_dwPvpPoint;

	GClass::BTreeMgrBase::stGBTreeNodes			str_thePvpWinCounts;
	GClass::BTreeMgrBase::stGBTreeNodes			str_thePvpLoseCounts;
	GClass::BTreeMgrBase::stGBTreeNodes			str_thePvpTotalCounts;
	TaskCurJobTemplate::CollectItemsMgr::stGBTreeNodes				collect_items_;						//收集物品
	GClass::BTreeMgrBase::stGBTreeNodes			str_theLicenses;
	GClass::GGameTArrayPlaceholder				str_theInacceptableQuests;
	DWORD						str_dwInacceptableWhenCompleted;	// 11.30新增数据段
	GClass::GGameTArrayPlaceholder				str_thePrecedingQuests;
	GClass::GGameTArrayPlaceholder				str_thePrecedingQuestsTypeOr;

	DWORD										str_dwDifficulty;
	DWORD										str_dwCompleteType;

	TaskCurJobTemplate::NpcConditionTArray::stGameTArray			npc_conditions_;					//击杀怪物
	GClass::GGameTArrayPlaceholder				str_thePvpConditions;
	GClass::BTreeMgrBase::stGBTreeNodes			str_theStageConditions;
	GClass::GGameTArrayPlaceholder				str_theUseItems;
	GClass::GGameTArrayPlaceholder				str_theEnhanceItems;
	GClass::BTreeMgrBase::stGBTreeNodes			str_theEquipmentSetInfos;
	GClass::BTreeMgrBase::stGBTreeNodes			str_theStatusConditions;

	DWORD										str_dwInventoryCapacity;
	DWORD										str_dwPlayerBankCapacity;
	DWORD										str_dwAccountBankCapacity;
};
static_assert(sizeof(ACCPET_CONDITION) == sizeof(stAcceptCondition), "");
typedef stAcceptCondition stCompleteCondition;

struct stQuestReward{
	DWORD				str_dwVFunAddr;
	DWORD				str_dwExpRate;
	DWORD				str_dwBonusExp;		// 11.30不删档改变字段
	DWORD				str_dwGold;
	DWORD				str_dwRemoveGold;
	GClass::BTreeMgrBase::stGBTreeNodes		str_theBuffsToGive;
	GClass::BTreeMgrBase::stGBTreeNodes		str_theBuffsToRemove;
	GClass::BTreeMgrBase::stGBTreeNodes		str_theItemsToGive;
	BINARY_TREE_MAP		str_theItemsToGiveMail;		// 11.30不删档新增字段
	GClass::BTreeMgrBase::stGBTreeNodes		str_theItemsToRemove;
	TaskCurJobTemplate::QuestRewardItemsMgr::stGBTreeNodes		str_theSelectableItems;				// 选择奖励物品
	GClass::BTreeMgrBase::stGBTreeNodes		str_theLicensesToGive;
	GClass::BTreeMgrBase::stGBTreeNodes		str_theLicensesToRemove;
	GClass::stGameStringEx					str_theChangeClass;
	DWORD									str_dwMakeAwaken;
	DWORD									str_dwAwakenPoint;
	GClass::GGameTArrayPlaceholder			str_theUnlockBasicEquipSlots;
	GClass::stGameString					str_theTitle;
	DWORD				str_dwAccountMileage;		// 11.30不删档新增字段
	FLOAT				str_fTierAttackPower;		// 11.30不删档新增字段
};
static_assert(sizeof(stQuestReward) == sizeof(QUEST_REWARD), "");

struct stTaskNpcInfo{
	GClass::stGameString		accept_task_npc_id_;	//接任务NPC id
	GClass::stGameString		complete_npc_id_;		//交任务NPC id
};
static_assert(sizeof(stTaskNpcInfo) == sizeof(ACCEPT_COMPLETE), "");
bool TaskCurJobTemplate::Update()
{
	using namespace GType;
	using namespace GOffset;
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	const auto cur_job_task = (stCurJobTask*)game_cur_node_;
	task_id_ = cur_job_task->task_id_;
	const auto task_info = cur_job_task->task_obj_addr_;
	if ( !task_info )
	{
		assert(false);
		return false;
	}
	if ( !task_info->task_id_.game_str_.CopyTo(str_task_id_) )
	{
		assert(false);
		return false;
	}
	if (task_info->stage_ids_.obj_begin_)
	{
		if ( !task_info->stage_ids_.obj_begin_->CopyTo(clone_map_id_) )
		{
			assert(false);
			return false;
		}
	}
	else
		clone_map_id_.clear();
	if ( !task_info->main_title_id_.game_str_.CopyTo(task_main_title_) )
	{
		assert(false);
		return false;
	}
	/*if ( task_info->task_category_ <= kTC_InvalidBegin || task_info->task_category_ >= kTC_InvalidEnd )
	{
		assert(false);
		return false;
	}*/
	task_category_ = task_info->task_category_;
	task_level_ = task_info->task_level_;
	assert(task_level_ >= 0 && task_level_ <= 200);
	const auto task_obj_addr = (int)task_info;
	const auto npc_info = (const stTaskNpcInfo*)(task_obj_addr + g_o_accept_complete_npc_info);
	if ( !npc_info->accept_task_npc_id_.CopyTo(accept_task_npc_) )
	{
		assert(false);
		return false;
	}
	if ( !npc_info->complete_npc_id_.CopyTo(complete_task_npc_) )
	{
		assert(false);
		return false;
	}
	const auto complete_condition = (const stCompleteCondition*)(task_obj_addr + g_o_complete_task_condition);
	ac_npc_condition_.SetGameTArray(&complete_condition->npc_conditions_);
	if ( !ac_npc_condition_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	collect_items_mgr_.SetGameNodes(&complete_condition->collect_items_);
	if ( !collect_items_mgr_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	const auto game_quest_reward = (const stQuestReward*)(task_obj_addr + g_o_task_reward_items);
	reward_items_mgr_.SetGameNodes(&game_quest_reward->str_theSelectableItems);
	if ( !reward_items_mgr_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	const auto teleport_location = (const stTeleportLocation*)(task_obj_addr + g_o_task_accept_teleport);
	TelePortLocation telep;
	if ( !telep.From(*teleport_location) )
	{
		assert(false);
		return false;
	}
	if ( !telep.village_id_.empty() )
		teleport_location_ = telep;
	return true;
}

TaskCurJobTemplate::TaskCurJobTemplate()
{
	task_id_ = 0;
	task_category_ = GType::kTC_InvalidBegin;
	task_level_ = 0;
}

DWORD TaskCurJobTemplate::GetTaskId() const
{
	return task_id_;
}

const std::wstring& TaskCurJobTemplate::GetStrTaskId() const
{
	return str_task_id_;
}

const std::wstring& TaskCurJobTemplate::GetCloneMapId() const
{
	return clone_map_id_;
}

const std::wstring& TaskCurJobTemplate::GetMainTitle() const
{
	return task_main_title_;
}

GType::enTaskCategory TaskCurJobTemplate::GetTaskCategory() const
{
	return task_category_;
}

int TaskCurJobTemplate::GetTaskLevel() const
{
	return task_level_;
}

const std::wstring& TaskCurJobTemplate::GetAcceptTaskNpc() const
{
	return accept_task_npc_;
}

const std::wstring& TaskCurJobTemplate::GetCompleteTaskNpc() const
{
	return complete_task_npc_;
}

const TaskCurJobTemplate::NpcConditionTArray& TaskCurJobTemplate::GetNpcCondition() const
{
	return ac_npc_condition_;
}

const TaskCurJobTemplate::CollectItemsMgr& TaskCurJobTemplate::GetCollectItemCondition() const
{
	return collect_items_mgr_;
}

const TaskCurJobTemplate::QuestRewardItemsMgr& TaskCurJobTemplate::GetRewardItems() const
{
	return reward_items_mgr_;
}

bool TaskCurJobTemplate::IsAcceptable() const
{
	using namespace GType;
	return kTC_Normal_KillMonster == task_category_ || kTC_Main == task_category_ || kTC_CollectMaterial == task_category_;
}

TaskCurJobTemplate::CollectItem::CollectItem()
{
	item_cnt_ = 0;
}

const std::wstring& TaskCurJobTemplate::CollectItem::GetCollectItemId() const
{
	return item_name_id_;
}

int TaskCurJobTemplate::CollectItem::GetCollectItemCnt() const
{
	return item_cnt_;
}

bool TaskCurJobTemplate::CollectItem::Update()
{
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	const auto collect_item = (const stSdbCollectItem*)game_cur_node_;
	if ( !collect_item->item_name_id_.CopyTo(item_name_id_) )
	{
		assert(false);
		return false;
	}
	if ( collect_item->item_cnt_ < 0 )
	{
		assert(false);
		return false;
	}
	item_cnt_ = collect_item->item_cnt_;
	return true;
}

TaskCurJobTemplate::QuestRewardItem::QuestRewardItem()
{
	reward_item_cnt_ = 0;
}

const std::wstring& TaskCurJobTemplate::QuestRewardItem::GetRewardItemId() const
{
	return reward_item_name_id_;
}

int TaskCurJobTemplate::QuestRewardItem::GetRewardItemCnt() const
{
	return reward_item_cnt_;
}

bool TaskCurJobTemplate::QuestRewardItem::Update()
{
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	const auto reward_item = (const stQuestRewardItem*)game_cur_node_;
	if ( !reward_item->item_name_id_.CopyTo(reward_item_name_id_) )
	{
		assert(false);
		return false;
	}
	if ( reward_item->item_cnt_ < 0 )
	{
		assert(false);
		return false;
	}
	reward_item_cnt_ = reward_item->item_cnt_;
	return true;
}

bool TaskCurJobTemplate::TelePortLocation::From( const stTeleportLocation& gtele )
{
	if ( !gtele.village_id_.game_str_.CopyTo(village_id_) )
	{
		assert(false);
		return false;
	}
	village_pos_ = gtele.village_pos_;
	village_direction_ = gtele.village_direction_;
	return true;
}

bool TaskCurJobTemplate::NpcCondition::Update()
{
	if ( !cur_obj_addr_ )
	{
		assert(false);
		return false;
	}
	const auto npc_condition = (const stNpcCondition*)cur_obj_addr_;
	if ( !npc_condition->npc_obj_id_.CopyTo(monster_id_) )
	{
		assert(false);
		return false;
	}
	if ( npc_condition->need_kill_cnt_ < 0 )
	{
		assert(false);
		return false;
	}
	need_kill_cnt_ = npc_condition->need_kill_cnt_;
	approved_clone_map_.SetGameTArray(&npc_condition->approved_stages_);
	if ( !approved_clone_map_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	//assert(0 == need_kill_cnt_ && approved_clone_map_.empty());
	return true;
}

TaskCurJobTemplate::NpcCondition::NpcCondition()
{
	need_kill_cnt_ = 0;
}

const std::wstring& TaskCurJobTemplate::NpcCondition::GetMonsterId() const
{
	return monster_id_;
}

int TaskCurJobTemplate::NpcCondition::GetNeedKillCnt() const
{
	return need_kill_cnt_;
}

const TaskCurJobTemplate::NpcCondition::ApprovedStagesTArray& TaskCurJobTemplate::NpcCondition::GetApprovedCloneMap() const
{
	return approved_clone_map_;
}

std::size_t TaskCurJobTemplate::NpcCondition::GetApprovedSize() const
{
	return approved_clone_map_.size();
}

const TaskCurJobTemplate::NpcCondition::ApprovedStage* TaskCurJobTemplate::NpcCondition::GetFirstApproved() const
{
	if (approved_clone_map_.empty())
		return nullptr;
	auto it_first = approved_clone_map_.GetCont().begin();
	return &*it_first;
}

bool TaskCurJobTemplate::NpcCondition::ApprovedStage::Update()
{
	if ( !cur_obj_addr_ )
	{
		assert(false);
	}
	const auto game_str = (const GameType*)(cur_obj_addr_);
	if ( !game_str->CopyTo(clone_map_id_) )
	{
		assert(false);
		return false;
	}
	return true;
}

const std::wstring& TaskCurJobTemplate::NpcCondition::ApprovedStage::GetCloneMapId() const
{
	return clone_map_id_;
}
struct stSdbReceivedTask : TaskReceived::stBTreeNodeBase{
	int						unknown_;
	GClass::stGameString	task_id_;
};
static_assert(sizeof(stSdbReceivedTask) == sizeof(RECEIVED_TASK), "");

struct stSdbActiveCondition{
	GClass::stGameStringEx		id_;
	DWORD						str_dwConditions;				// Conditions=0x200 表示打怪任务,Conditions=0表示对话任务,Conditions=0x10表示收集任务
	bool						completable_;					// Completable=1 表示任务已完成,Completable=0 表示任务未完成;
	BYTE						unknown_[3];
	struct
	{
		DWORD						str_dwVFunAddr2;
		GAME_STRUCT_ARRAY_POINT		str_theRemainUseItems;
		TaskReceived::ActiveNpcTArray::stGameTArray		str_theRemainKillNpcInfos;			// 剩余杀怪数量
		GAME_STRUCT_ARRAY_POINT		str_theRemainPvpConditions;
		BINARY_TREE_MAP				str_theRemainStageIds;
		GAME_STRUCT_ARRAY_POINT		str_theRemainEnhance;
		BINARY_TREE_MAP				str_theRemainStatus;
	}str_theRemainCondition;

	DWORD					str_dwNullAddr;
	struct
	{
		DWORD				str_dwOidActiveCondition1;
		DWORD				str_dwOidActiveCondition2;
	}str_theOidActiveCondition;
};
static_assert(sizeof(stSdbActiveCondition) == sizeof(ACTIVE_CONDITION), "");

bool TaskReceived::Update()
{
	using namespace GOffset;
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	const auto task_received = (const stSdbReceivedTask*)game_cur_node_;
	if ( !task_received->task_id_.CopyTo(task_id_) )
	{
		assert(false);
		return false;
	}
	const auto active_condition = (const stSdbActiveCondition*)((int)game_cur_node_ + g_o_received_task_active_condition);
	assert(0 == active_condition->completable_ || 1 == active_condition->completable_);
	completable_ = !!active_condition->completable_;
	remain_active_npc_.SetGameTArray(&active_condition->str_theRemainCondition.str_theRemainKillNpcInfos);
	if ( !remain_active_npc_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

const std::wstring& TaskReceived::GetStrTaskId() const
{
	return task_id_;
}

TaskReceived::TaskReceived()
{
	completable_ = false;
}

bool TaskReceived::TaskCompletable() const
{
	if ( !completable_ )
		return false;
	auto pthis = (TaskReceived*)this;
	auto task_template = pthis->GetTaskTemplate();
	if ( !task_template )
		return false;
	if (task_template->GetCompleteTaskNpc().empty())
		return false;
	return true;
}

const std::wstring& TaskReceived::GetCompleteStageId()
{
	auto template_task = GetTaskTemplate();
	if (template_task)
		return template_task->GetCloneMapId();
	return kEmptyWStr;
}

const TaskReceived::ActiveNpcTArray& TaskReceived::GetRemainActiveNpcInfo() const
{
	return remain_active_npc_;
}

const TaskCurJobTemplate* TaskReceived::GetTaskTemplate()
{
	if ( !task_template_ )
	{
		auto& task_mgr = TaskCurJobTemplateMgr::GetMe();
		task_mgr.RebuildUpdateAll();
		task_template_ = task_mgr.FindTaskPtrById(task_id_);
		if ( !task_template_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "找不到任务模板->" << task_id_;
			//assert(false);
			return nullptr;
		}
	}
	return task_template_.get();
}

bool TaskReceived::CanDoTheAcceptedTask() const
{
	auto pthis = (TaskReceived*)this;
	auto task_template = pthis->GetTaskTemplate();
	if ( !task_template )
		return false;
	auto player_lvl = GPlayerSelf::GetMe().GetLevel();
	/*auto& quest_index = GameEn2ChNodeMgr::GetMe().GetQuestIndex();
	if ( !quest_index )
	{
		assert(false);
		return false;
	}*/
	//const auto& ch_name = quest_index->En2Ch(task_id_);
	auto lvl_span = player_lvl - task_template->GetTaskLevel();

	//LOG_O(Log_trace) << "111 ch name->" << ch_name << "\tlvl";

	if ( lvl_span <= 0 )
		return true;
	if ( player_lvl <= 20 )
	{
		if ( lvl_span <= 5 )
			return true;
		return false;
	}
	else if ( player_lvl <= 35 )
	{
		if ( lvl_span <= 4 )
			return true;
		return false;
	}
	else if ( player_lvl <= 50 )
	{
		if ( lvl_span <= 3 )
			return true;
		return false;
	}
	else
	{
		if ( lvl_span <= 2 )
			return true;
		return false;
	}
	assert(false);
}

bool TaskNpcMgr::RebuildUpdateAll()
{
	return timer_sync_exec_.TimerGWndExecSync([this](){
		return DoUpdateImpl();
	});
}

TaskNpcMgr::NpcTaskIterPtr TaskNpcMgr::TraverseAcceptableNpc()
{
	return MakeIterator<TraTraits>(btree_, []( TraTraits::DoFuncItemT npc_task ){
		return !npc_task.GetNpcId().empty() && !npc_task.GetAcceptableMgr().GetAllNodes().empty();
	});
}

TaskNpcMgr::NpcTaskIterPtr TaskNpcMgr::TraverseLocalMapAcceptableNpc()
{
	auto obj_mgr = &GameObjMgr::GetMe();
	assert(obj_mgr);
	return MakeIterator<TraTraits>(btree_, [obj_mgr]( TraTraits::DoFuncItemT npc_task ){
		const auto& npc_id = npc_task.GetNpcId();
		if (npc_id.empty())
			return false;
		if (npc_task.GetAcceptableMgr().GetAllNodes().empty())
			return false;
		if ( !obj_mgr->FindByName(npc_id) )
			return false;
		return true;
	});
}

bool TaskNpcMgr::DoUpdateImpl()
{
	using namespace GAddr;
	using namespace GClass;
	using namespace GOffset;
	struct Helper{
		Helper(){
			npc_task_obj_node_ = nullptr;
		}
		bool FindTaskNpc(){
			if ( !g_pLocalAllObjMapAddr )
			{
				assert(false);
				return false;
			}
			BTreeMgrPlaceholder all_obj_map;
			all_obj_map.SetGameNodes((const BTreeMgrPlaceholder::stGBTreeNodes*)((int)g_pLocalAllObjMapAddr + g_o_all_local_obj_btree));
			return all_obj_map.DoUntil([this]( const BTreeBase::stBTreeNodeBase* node ){
				assert(node);
				struct stGameObjNode : public BTreeBase::stBTreeNodeBase{
					stGameString		obj_id_;
				};
				const auto gameobj_node = (const stGameObjNode*)node;
				static const std::wstring kActiveQuest = L"GetActiveQuests";
				if (gameobj_node->obj_id_.EqualTo(kActiveQuest))
				{
					npc_task_obj_node_ = node;
					return true;
				}
				return false;
			});
		}

		const BTreeBase::stBTreeNodeBase* npc_task_obj_node_;
	};
	static Helper hlp;

	if ( !hlp.npc_task_obj_node_ )
	{
		if ( !hlp.FindTaskNpc() )
		{
			//assert(false);
			return false;
		}
		assert(hlp.npc_task_obj_node_);
	}
	const BTreeBase::stBTreeNodeBase* npc_task_node = nullptr;
	if ( !MemOperApplyR(npc_task_node, (int)hlp.npc_task_obj_node_, g_o_task_npc) || !npc_task_node )
	{
		assert(false);
		return false;
	}
	SetRootNode(npc_task_node);
	if ( !__super::RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

struct stSdbNpcTaskNpc : TaskNpc::stBTreeNodeBase{
	GClass::stGameString	npc_id_;
	void*					task_npc_obj_;
	void*					task_npc_unknown_ptr_;
	DWORD					accept_flag_;		//这个标志是没有用的，一个未知的标志，表示1时，表示此NPC有任务可接；表示0时，大部分是没有可接任务，但是活动任务除外
};
static_assert(sizeof(stSdbNpcTaskNpc) == sizeof(TASK_NPC_OBJ), "");

bool TaskNpc::Update()
{
	using namespace GOffset;
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	const auto task_npc = (const stSdbNpcTaskNpc*)game_cur_node_;
	if ( !task_npc->npc_id_.CopyTo(npc_id_) )
	{
		assert(false);
		return false;
	}
	const auto kNpcInfoAddr = (int)task_npc->task_npc_obj_;
	if ( !kNpcInfoAddr )
	{
		assert(false);
		return false;
	}
	const TaskMgr::stGBTreeNodes* task_nodes = nullptr;
	task_nodes = (const TaskMgr::stGBTreeNodes*)(kNpcInfoAddr + g_o_task_npc_acceptable);
	acceptable_task_.SetGameNodes(task_nodes);
	if ( !acceptable_task_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	task_nodes = (const TaskMgr::stGBTreeNodes*)(kNpcInfoAddr + g_o_task_npc_complete);
	complete_task_.SetGameNodes(task_nodes);
	if ( !complete_task_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

TaskNpc::TaskMgr& TaskNpc::GetAcceptableMgr()
{
	return acceptable_task_;
}

const std::wstring& TaskNpc::GetNpcId() const
{
	return npc_id_;
}

struct stSdbNpcTaskInfo : TaskNpc::TaskNode::stBTreeNodeBase{
	GClass::stGameString		task_id_;
};
static_assert(sizeof(stSdbNpcTaskInfo) == sizeof(CAN_ACCPET_NPC_TASK), "");

bool TaskNpc::TaskNode::Update()
{
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	auto task_info = (const stSdbNpcTaskInfo*)game_cur_node_;
	if ( !task_info->task_id_.CopyTo(task_id_) )
	{
		assert(false);
		return false;
	}
	return true;
}

const std::wstring& TaskNpc::TaskNode::GetTaskId() const
{
	return task_id_;
}

const TaskCurJobTemplate::SharedPtrT& TaskNpc::TaskNode::GetTaskTemplate()
{
	if (task_template_)
		return task_template_;
	auto& task_mgr = TaskCurJobTemplateMgr::GetMe();
	task_mgr.RebuildUpdateAll();
	task_template_ = task_mgr.FindTaskPtrById(task_id_);
	assert(task_template_);
	return task_template_;
}

TaskReceived* TaskReceivedMgr::FindTask( const std::wstring& quest_id )
{
	if (btree_.empty())
		return nullptr;
	auto it = std::find_if(btree_.begin(), btree_.end(), [&quest_id]( const TaskReceived& task_r ){
		return quest_id == task_r.GetStrTaskId();
	});
	if ( it == btree_.end() )
		return nullptr;
	return &*it;
}

TaskReceivedMgr::AcceptedTaskIterPtr TaskReceivedMgr::TraverseUncompleteTasks()
{
	return MakeIterator<TraTraits>(btree_, []( TraTraits::DoFuncItemT accepted_task ){
		if (accepted_task.TaskCompletable())
			return false;
		if ( !accepted_task.CanDoTheAcceptedTask() )
			return false;
		return true;
	});
}

TaskReceivedMgr::AcceptedTaskIterPtr TaskReceivedMgr::TraverseCompleteTasks()
{
	return MakeIterator<TraTraits>(btree_, []( TraTraits::DoFuncItemT accepted_task ){
		if ( !accepted_task.TaskCompletable() )
			return false;
		if ( !accepted_task.CanDoTheAcceptedTask() )
			return false;
		return true;
	});
}

bool TaskReceivedMgr::IsFulled() const
{
	assert(btree_.size() <= 25);
	return btree_.size() >= 25;
}

bool TaskReceivedMgr::RebuildUpdateAll()
{
	using namespace GAddr;
	using namespace GOffset;
	int unknown_addr = 0;
	if ( !MemOperApplyR(unknown_addr, g_pRoleObjAddr, g_o_task_received1 + g_o_task_received2) || !unknown_addr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "accepted task unknonw addr read error";
		btree_.clear();
		return false;
	}
	const auto tree_nodes = (const stGBTreeNodes*)(unknown_addr + g_o_task_received3 + g_o_task_received4);
	if ( tree_nodes->node_cnt_ <= 0 )
	{
		assert(0 == tree_nodes->node_cnt_);
		btree_.clear();
		return true;
	}
	SetGameNodes(tree_nodes);
	return __super::RebuildUpdateAll();
}

TaskNpc::TaskMgr::TaskNodeIterPtr TaskNpc::TaskMgr::TraverseAllTask()
{
	return MakeIterator<TraTraits>(btree_, []( TraTraits::DoFuncItemT task_node ){
		if (task_node.GetTaskId().empty())
		{
			assert(false);
			return false;
		}
		return true;
	});
}

TaskReceived::RemainNpc::RemainNpc()
{
	remaint_cnt_ = 0;
}

bool TaskReceived::RemainNpc::Update()
{
	if ( !cur_obj_addr_ )
	{
		assert(false);
		return false;
	}
	auto remain_condition = (const stRemainNpcConditions*)cur_obj_addr_;
	assert(remain_condition->str_dwValue >= 0);
	remaint_cnt_ = remain_condition->str_dwValue;
	if ( 1 != std::distance(remain_condition->str_theStages.obj_begin_, remain_condition->str_theStages.obj_end_) )
	{
		assert(false);
		return false;
	}
	if ( !remain_condition->str_theStages.obj_begin_->CopyTo(stage_id_) )
	{
		assert(false);
		return false;
	}
	return true;
}

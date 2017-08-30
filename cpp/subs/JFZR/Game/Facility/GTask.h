#pragma once
/*
@author zhp
@date 2014/8/7 12:24
@purpose for task
*/
#include "GClass.h"
#include <boost/optional.hpp>
#include "GAddr.h"

//当前职业的任务
class TaskCurJobTemplate : public GClass::BTreeBase{
public:
	typedef std::shared_ptr<TaskCurJobTemplate> SharedPtrT;
	
	class NpcCondition : public GClass::GGameTypeBase{
	public:
		class ApprovedStage : public GClass::GGameTypeBase{
		public:
			typedef GClass::stGameString GameType;

		public:
			bool Update();
			const std::wstring& GetCloneMapId() const;

		private:
			std::wstring			clone_map_id_;
		};
		typedef GClass::GGameTArray<std::vector<ApprovedStage> > ApprovedStagesTArray;
		//击杀怪物
		struct stNpcCondition{
			void*								vtbl_;
			DWORD								str_dwNpcConditionType;
			GClass::stGameString				npc_obj_id_;						//NPC id
			int									need_kill_cnt_;						//需要击杀的数量
			DWORD								str_dwBackAttackCount;
			DWORD								str_dwSpecialAttackUseCount;
			DWORD								str_dwDeadlySkillUseCount;
			GClass::GGameTArrayPlaceholder		str_theSkillUsageCounts;
			ApprovedStagesTArray::stGameTArray	approved_stages_;					//怪物所在副本地图
			GClass::BTreeMgrBase::stGBTreeNodes	str_theNpcKillBySkill;
		};
		static_assert(sizeof(stNpcCondition) == sizeof(NPC_CONDITION), "");
		typedef stNpcCondition GameType;

	public:
		NpcCondition();
		bool Update();
		const std::wstring& GetMonsterId() const;
		int GetNeedKillCnt() const;
		const ApprovedStagesTArray& GetApprovedCloneMap() const;
		std::size_t GetApprovedSize() const;
		const ApprovedStage* GetFirstApproved() const;

	private:
		std::wstring			monster_id_;
		int						need_kill_cnt_;
		ApprovedStagesTArray	approved_clone_map_;			//怪物所在副本
	};
	typedef GClass::GGameTArray<std::vector<NpcCondition> > NpcConditionTArray;
	class CollectItem : public GClass::BTreeBase{
	public:
		struct stSdbCollectItem : stBTreeNodeBase{
			GClass::stGameString		item_name_id_;		//需要收集物品的名称id
			int							item_cnt_;			//需要收集物品的数量
		};
		static_assert(sizeof(stSdbCollectItem) == sizeof(COLLECT_ITEM), "");

	public:
		CollectItem();
		bool Update();
		const std::wstring& GetCollectItemId() const;
		int GetCollectItemCnt() const;

	private:
		std::wstring			item_name_id_;
		int						item_cnt_;
	};
	class CollectItemsMgr : public GClass::BTreeMgr_SkipRoot<CollectItem>{};
	class QuestRewardItem : public GClass::BTreeBase{
	public:
		typedef CollectItem::stSdbCollectItem stQuestRewardItem;

	public:
		QuestRewardItem();
		bool Update();
		const std::wstring& GetRewardItemId() const;
		int GetRewardItemCnt() const;

	private:
		std::wstring			reward_item_name_id_;
		int						reward_item_cnt_;
	};
	class QuestRewardItemsMgr : public GClass::BTreeMgr_SkipRoot<QuestRewardItem>{};
		
	struct stTeleportLocation{
		GClass::stGameStringEx		village_id_;
		GType::stGameVector			village_pos_;
		GType::stGameVector			village_direction_;
	};
	static_assert(sizeof(stTeleportLocation) == sizeof(TELEPORTLOCATION), "");
	struct TelePortLocation{
		std::wstring			village_id_;
		GType::stGameVector		village_pos_;
		GType::stGameVector		village_direction_;

		bool From( const stTeleportLocation& gtele );
	};

public:
	TaskCurJobTemplate();
	bool Update();

	//get
public:
	DWORD GetTaskId() const;
	const std::wstring& GetStrTaskId() const;
	const std::wstring& GetCloneMapId() const;
	const std::wstring& GetMainTitle() const;
	GType::enTaskCategory GetTaskCategory() const;
	int GetTaskLevel() const;
	const std::wstring& GetAcceptTaskNpc() const;
	const std::wstring& GetCompleteTaskNpc() const;
	const NpcConditionTArray& GetNpcCondition() const;
	const CollectItemsMgr& GetCollectItemCondition() const;
	const QuestRewardItemsMgr& GetRewardItems() const;
	bool IsAcceptable() const;

private:
	DWORD						task_id_;
	std::wstring				str_task_id_;
	std::wstring				clone_map_id_;				//完成任务所在副本
	std::wstring				task_main_title_;			//任务名称
	GType::enTaskCategory		task_category_;
	int							task_level_;				//任务等级
	std::wstring				accept_task_npc_;			//接任务npc
	std::wstring				complete_task_npc_;			//完成任务npc

	//accept condition
private:

	//teleport location,传送站
private:
	boost::optional<TelePortLocation>		teleport_location_;		//目标传送站

	//complete condition
private:
	NpcConditionTArray			ac_npc_condition_;			//打怪需求
	CollectItemsMgr				collect_items_mgr_;			//收集物品需求
	QuestRewardItemsMgr			reward_items_mgr_;			//奖励物品
};
//当前职业的所有任务
class TaskCurJobTemplateMgr : public GClass::BTreeMgr_SkipRoot<GClass::BTreeBasePtrWrapper<TaskCurJobTemplate> >, public Singleton<TaskCurJobTemplateMgr, Singleton_MakeMe>{
public:
	bool RebuildUpdateAll();
	bool UpdateAll();

private:
	bool UpdateImpl();

	//遍历与索引
public:
	const TaskCurJobTemplate* FindByTaskId( const std::wstring& task_id ) const;
	TaskCurJobTemplate::SharedPtrT FindTaskPtrById ( const std::wstring& task_id ) const;
};

//已接任务
class TaskReceived : public GClass::BTreeBase{
public:
	class RemainNpc : public GClass::GGameTypeBase{
	public:
		typedef GClass::GGameTArray<std::vector<GClass::GGameTypePlaceholder<GClass::stGameString> > > StageTArray;
		//击杀怪物
		struct stRemainNpcConditions{
			DWORD				str_dwVFunAddr;
			GAME_STRING_ID		str_theType;
			GAME_STRING_ID		str_theKey;						// 任务需要消灭NPC的名称id
			int					str_dwValue;					// 需要消灭NPC对象的个数
			DWORD				str_dwBackAttackCount;						// 背后攻击次数
			DWORD				str_dwSpecialAttackUseCount;				// 特殊攻击使用数
			DWORD				str_dwDeadlySkillUseCount;					// 致命的技能使用数
			GAME_STRUCT_ARRAY_POINT			str_theSkillUsageCounts;		// 技能的使用数量
			StageTArray::stGameTArray			str_theStages;					// 任务阶段，即所属地图
			BINARY_TREE_MAP					str_theNpcKillBySkill;			// 使用技能杀死怪物
		};
		static_assert(sizeof(stRemainNpcConditions) == sizeof(REMAIN_NPC_CONDITION), "");
		typedef stRemainNpcConditions GameType;
		

	public:
		RemainNpc();
		bool Update();

		std::wstring	stage_id_;				//要去的副本
		int				remaint_cnt_;			//剩余消灭数量
	};
	typedef GClass::GGameTArray<std::vector<RemainNpc> > ActiveNpcTArray;

public:
	TaskReceived();
	bool Update();
	const std::wstring& GetStrTaskId() const;
	bool TaskCompletable() const;
	bool CanDoTheAcceptedTask() const;
	const TaskCurJobTemplate* GetTaskTemplate();
	const std::wstring& GetCompleteStageId();
	const ActiveNpcTArray& GetRemainActiveNpcInfo() const;

private:
	std::wstring					task_id_;
	bool							completable_;			//是否已完成
	TaskCurJobTemplate::SharedPtrT	task_template_;
	ActiveNpcTArray					remain_active_npc_;
};

//所有已接任务
class TaskReceivedMgr : public GClass::BTreeMgr_SkipRoot<TaskReceived>{
public:
	typedef TraverseTraits<TaskReceived&, const TaskReceived&> TraTraits;
	typedef TraTraits::IteratorBase AcceptedTaskIterBase;
	typedef TraTraits::IteratorBasePtr AcceptedTaskIterPtr;

public:
	bool RebuildUpdateAll();
	TaskReceived* FindTask( const std::wstring& quest_id );
	bool IsFulled() const;
	//迭代所有未完成任务
	AcceptedTaskIterPtr TraverseUncompleteTasks();
	AcceptedTaskIterPtr TraverseCompleteTasks();
};

//npc任务
class TaskNpc : public GClass::BTreeBase{
public:
	class TaskNode : public GClass::BTreeBase{
	public:
		bool Update();
		const std::wstring& GetTaskId() const;
		const TaskCurJobTemplate::SharedPtrT& GetTaskTemplate();

	private:
		std::wstring					task_id_;
		TaskCurJobTemplate::SharedPtrT	task_template_;
	};
	class TaskMgr : public GClass::BTreeMgr_SkipRoot<TaskNode>{
	public:
		typedef TraverseTraits<const TaskNode&, const TaskNode&> TraTraits;
		typedef TraTraits::IteratorBase TaskNodeIterBase;
		typedef TraTraits::IteratorBasePtr TaskNodeIterPtr;

	public:
		TaskNodeIterPtr TraverseAllTask();
	};

public:
	bool Update();
	TaskMgr& GetAcceptableMgr();
	const std::wstring& GetNpcId() const;

private:
	std::wstring	npc_id_;
	TaskMgr			acceptable_task_;			//可接受的任务
	TaskMgr			complete_task_;				//已经完成的任务
};

class TaskNpcMgr : public GClass::BTreeMgr_SkipRoot<TaskNpc>, public Singleton<TaskNpcMgr, Singleton_MakeMe>{
public:
	typedef TraverseTraits<TaskNpc&, TaskNpc&> TraTraits;
	typedef TraTraits::IteratorBase NpcTaskIterBase;
	typedef TraTraits::IteratorBasePtr NpcTaskIterPtr;

public:
	bool RebuildUpdateAll();
	//遍历可以接受任务的NPC
	NpcTaskIterPtr TraverseAcceptableNpc();
	//遍历本地图可以接受任务的NPC
	NpcTaskIterPtr TraverseLocalMapAcceptableNpc();

private:
	bool DoUpdateImpl();

private:
	GClass::TimerSyncExec	timer_sync_exec_;
};
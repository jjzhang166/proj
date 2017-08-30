#pragma once
/*
@author zhp
@date 2014/8/7 12:24
@purpose for task
*/
#include "GClass.h"
#include <boost/optional.hpp>
#include <vector>
#include "GameObj.h"

class GTaskDb : public GGameObjBase{
public:
	struct stItemInfo{
		int			item_id_;
		int			item_cnt_;
	};

public:
	GTaskDb();

public:
	void SetTaskDb(stCD_TaskDb* task_db);
	int GetTaskId() const;
	void SetTaskId( int task_id );
	const std::string& GetTaskName() const;
	bool HasTheDoFb( int fb_id ) const;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

	//test
public:
	void DebugInfo() const override;

public:
	const stCD_TaskDb*	task_db_;

public:
	int					task_id_;
	std::string			task_name_;
	CD_En_TaskType		task_type_;
	std::string			task_desc_;				//任务描述
	NameList<int, std::vector<int> >		do_fb_ids_;
	std::vector<stItemInfo>	task_rewards_;
	GObjDbInfo			accept_npc_;
	GObjDbInfo			complete_npc_;
};

class GTaskDbMgr : public GGameObjMgrBase, public Singleton<GTaskDbMgr, Singleton_MakeMe>{
public:
	typedef TraverseTraits<GTaskDb&, GTaskDb&> TaskTraits;
	typedef TaskTraits::DoFuncItemT TaskItemT;
	typedef TaskTraits::IteratorBase TaskIterator;
	typedef TaskTraits::IteratorBasePtr TaskIteratorPtr;
	typedef TaskTraits::DoAnyTraverse DoAnyFuncT;

	typedef std::vector<GTaskDb> TasksContT;

public:
	GTaskDbMgr();
	bool RebuildAll() override;

private:
	bool UpdateAllImpl() override;
	bool RebuildAllImpl() override;
	void DoRebuildUpdateClear() override;

	//traverse
public:
	bool DoAnyMainTask( const DoAnyFuncT& do_any_func );
	TaskIteratorPtr TraverseMainTask( const DoAnyFuncT& do_any_func );
	TaskIteratorPtr TraverseMainTaskByName( const std::string& task_name );
	TaskIteratorPtr TraverseAllTaskByName( const std::string& task_name );
	const GTaskDb* FindById( int task_id );
	const TasksContT& GetTasks() const;

	//test
public:
	void DebugInfo() const override;
	void AssertUniqueTaskName();

private:
	TasksContT		tasks_;
};

class GReceivedTask : public GClass::BTreeBase{
public:
	GReceivedTask();
	bool IsDone() const;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

	//test
public:
	void DebugInfo() const override;

	int			task_id_;
	int			complete_value_;
};

class GReceivedTaskMgr : public GClass::BTreeMgr_SkipRoot<GReceivedTask>{
public:
	GReceivedTask* FindById( int task_id );
	void CpyIdToLua( const luabind::object& tbl, bool is_done ) const;

private:
	bool RebuildAllImpl() override;

	//test
public:
	void DebugInfo() const override;
};

class GAcceptableTasks : public GGameObjBase{
	typedef std::vector<int> TasksContT;

public:
	GAcceptableTasks();
	bool Update() override;
	bool Empty() const;
	void CpyIdToLua( const luabind::object& tbl ) const;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

	//test
public:
	void DebugInfo() const override;

	TasksContT	acceptable_tasks_;			//可接的任务
};
#include "stdafx.h"
#include "GTask.h"
#include "GameObjMgr.h"
#include "GPlayer.h"
#include "GConst.h"
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>

bool GTaskDb::UpdateImpl()
{
	if (IsUnValidPtr(task_db_))
	{
		assert(false);
		return false;
	}
	
	task_name_.assign(CDF_EmptyStr(task_db_->task_name_));

	if ( task_name_.size() < 3 )
		assert(false);

	task_id_ = task_db_->task_id_;
	task_type_ = task_db_->task_type_;
	task_desc_.assign(CDF_EmptyStr(task_db_->task_desc_));
		
	do_fb_ids_.Clear();
	if (CD_En_TaskType::kTT_Shang == task_type_)
	{
		task_db_->dst_fb_info_.Traverse([this](stCD_TaskDstFbInfo** ptr){
			auto dst_fb_info = *ptr;
			if (IsUnValidPtr(dst_fb_info))
				return true;
			if (IsUnValidPtr(dst_fb_info->dst_fbs_.begin_))
				do_fb_ids_.Add(dst_fb_info->fb_id_);
			else
			{
				dst_fb_info->dst_fbs_.Traverse([this](stCD_DstFbInfo** ptr){
					auto fb_info = *ptr;
					if (IsUnValidPtr(fb_info))
						return true;
					do_fb_ids_.Add(fb_info->fb_id_);
					return true;
				});
			}
			return true;
		});
	}

	//任务奖励
	task_rewards_.clear();
	task_db_->task_reward_.Traverse([this](stCD_TaskReward** ptr){
		auto reward = *ptr;
		if (IsUnValidPtr(reward))
			return true;
		reward->reward_items_.Traverse([this](stCD_RewardItem** ptr){
			auto reward = *ptr;
			if (IsUnValidPtr(reward))
				return true;
			if (IsUnValidPtr(reward->item_db_))
			{
				assert(false);
				return false;
			}
			stItemInfo item_info;
			item_info.item_id_ = reward->item_db_->item_id_;
			item_info.item_cnt_ = reward->item_cnt_;
			assert(item_info.item_cnt_ > 0);
			task_rewards_.push_back(item_info);
			return true;
		});
		return true;
	});
	if (!IsUnValidPtr(task_db_->accept_npc_db_))
	{
		accept_npc_.obj_db_info_ = task_db_->accept_npc_db_;
		if (!accept_npc_.Update())
		{
			assert(false);
			return false;
		}
	}
	if (!IsUnValidPtr(task_db_->complete_npc_db_))
	{
		complete_npc_.obj_db_info_ = task_db_->complete_npc_db_;
		if (!complete_npc_.Update())
		{
			assert(false);
			return false;
		}
	}
	return true;
}

GTaskDb::GTaskDb()
{
	task_db_ = nullptr;
	task_id_ = 0;
	task_type_ = CD_En_TaskType::kTT_Invalid;
}

void GTaskDb::SetTaskDb(stCD_TaskDb* task_db)
{
	task_db_ = task_db;
}

void GTaskDb::DebugInfo() const
{
	LOG_O(Log_trace) << "task id:" << task_id_ << "  task name:" << task_name_ << "  task type:" << task_type_;
}

int GTaskDb::GetTaskId() const
{
	return task_id_;
}

void GTaskDb::SetTaskId( int task_id )
{
	assert(task_id >= 0);
	task_id_ = task_id;
}

const std::string& GTaskDb::GetTaskName() const
{
	return task_name_;
}

bool GTaskDb::HasTheDoFb( int fb_id ) const
{
	return do_fb_ids_.IsIn(fb_id);
}

void GTaskDb::DoUpdateClear()
{
	task_rewards_.clear();
	do_fb_ids_.Clear();
	task_name_.clear();
	task_id_ = 0;
}

bool GTaskDbMgr::UpdateAllImpl()
{
	assert(false);
	return false;
}

bool GTaskDbMgr::RebuildAll()
{
	if ( !tasks_.empty() )
		return true;
	return __super::RebuildAll();
}

void GTaskDbMgr::DebugInfo() const
{
	LOG_O(Log_trace) << "total task cnt:" << tasks_.size();
	for ( auto& task : tasks_ )
	{
		task.DebugInfo();
	}
}

void GTaskDbMgr::AssertUniqueTaskName()
{
	std::vector<std::string> tasks_name;
	DoAnyMainTask([&tasks_name]( GTaskDb& task ){
		tasks_name.push_back(task.task_name_);
		return false;
	});
	std::sort(tasks_name.begin(), tasks_name.end());
	auto it_erase = Unique(tasks_name.begin(), tasks_name.end());
	if ( it_erase == tasks_name.end() )
	{
		LOG_O(Log_trace) << "主线任务中没有重名的任务";
	}
	else
	{
		LOG_O(Log_trace) << "主线任务中有几下的任务重名";
		while ( it_erase != tasks_name.end() )
		{
			LOG_O(Log_trace) << *it_erase;
			++it_erase;
		}
	}
	tasks_name.clear();

	for ( auto& task : tasks_ )
	{
		tasks_name.push_back(task.task_name_);
	}
	std::sort(tasks_name.begin(), tasks_name.end());

	it_erase = Unique(tasks_name.begin(), tasks_name.end());
	if ( it_erase == tasks_name.end() )
	{
		LOG_O(Log_trace) << "所有任务中没有重名的任务";
	}
	else
	{
		LOG_O(Log_trace) << "所有任务中有几下的任务重名";
		while ( it_erase != tasks_name.end() )
		{
			LOG_O(Log_trace) << *it_erase;
			++it_erase;
		}
	}
}

bool GTaskDbMgr::DoAnyMainTask( const DoAnyFuncT& do_any_func )
{
	for ( auto& task : tasks_ )
	{
		if (CD_En_TaskType::kTT_Main != task.task_type_)
			continue;
		if (do_any_func(task))
			return true;
	}
	return false;
}

GTaskDbMgr::TaskIteratorPtr GTaskDbMgr::TraverseMainTask( const DoAnyFuncT& do_any_func )
{
	return MakeIterator<TaskTraits>(tasks_, [do_any_func]( TaskItemT task ){
		if (CD_En_TaskType::kTT_Main != task.task_type_)
			return false;
		return do_any_func(task);
	});
}

GTaskDbMgr::TaskIteratorPtr GTaskDbMgr::TraverseMainTaskByName( const std::string& task_name )
{
	if (task_name.empty())
	{
		assert(false);
		return TaskIteratorPtr();
	}
	return MakeIterator<TaskTraits>(tasks_, [task_name]( TaskItemT task ){
		if (CD_En_TaskType::kTT_Main != task.task_type_)
			return false;
		if ( task_name != task.task_name_ )
			return false;
		return true;
	});
}

GTaskDbMgr::GTaskDbMgr()
{
	/*GTaskTemplate tmp;
	tmp.task_type_ = CD_En_TaskType::kTT_Main;
	tmp.task_id_ = 132;
	tmp.task_name_ = "abcd";
	tasks_.push_back(tmp);*/
}

GTaskDbMgr::TaskIteratorPtr GTaskDbMgr::TraverseAllTaskByName( const std::string& task_name )
{
	if (task_name.empty())
	{
		assert(false);
		return TaskIteratorPtr();
	}
	return MakeIterator<TaskTraits>(tasks_, [task_name]( TaskItemT task ){
		if ( task_name != task.task_name_ )
			return false;
		return true;
	});
}

const GTaskDb* GTaskDbMgr::FindById( int task_id )
{
	for ( auto& v : tasks_ )
	{
		if ( v.task_id_ == task_id )
			return &v;
	}
	return nullptr;
}

bool GTaskDbMgr::RebuildAllImpl()
{
	tasks_.clear();
	return CD_Traverse::TraverseTasksDb([this](stCD_TaskDb* task_db){
		//task_tmp必须就得是临时的，每次Update都得换新的才行
		GTaskDb task_tmp;
		task_tmp.SetTaskDb(task_db);
		if (!task_tmp.Update())
		{
			assert(false);
			return false;
		}
		tasks_.push_back(task_tmp);
		return true;
	}) > 100;
}

void GTaskDbMgr::DoRebuildUpdateClear()
{
	tasks_.clear();
}

const GTaskDbMgr::TasksContT& GTaskDbMgr::GetTasks() const
{
	return tasks_;
}

void GReceivedTaskMgr::DebugInfo() const
{
	LOG_O(Log_trace) << "received task total cnt:" << btree_.size();
	for ( auto& task : btree_ )
	{
		task.DebugInfo();
	}
}

GReceivedTask* GReceivedTaskMgr::FindById( int task_id )
{
	if ( task_id < 0 )
	{
		assert(false);
		return false;
	}
	for ( auto& v : btree_ )
	{
		if ( task_id == v.task_id_ )
			return &v;
	}
	return nullptr;
}

void GReceivedTaskMgr::CpyIdToLua( const luabind::object& tbl, bool is_done ) const
{
	int i = 1;
	if (is_done)
	{
		for ( auto& v : btree_ )
		{
			if (v.IsDone())
				tbl[i++] = v.task_id_;
		}
	}
	else
	{
		for ( auto& v : btree_ )
		{
			if ( !v.IsDone() )
				tbl[i++] = v.task_id_;
		}
	}
}

bool GReceivedTaskMgr::RebuildAllImpl()
{
	auto node = CD_FuncCall::GetReceivedTaskRootNode();
	if (IsUnValidPtr(node))
		return 0;
	SetRootNode(node);
	if ( !__super::RebuildAllImpl() )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GReceivedTask::UpdateImpl()
{
	if (IsUnValidPtr(GetCurNode()))
	{
		assert(false);
		return false;
	}
	auto the_obj = GetCurNode()->CastTo<stCD_TaskNode>();
	if (IsUnValidPtr(the_obj->task_))
		return false;
	task_id_ = the_obj->id_;
	assert(task_id_ >= 0);

	complete_value_ = the_obj->task_->complete_state_;
	return true;
}

GReceivedTask::GReceivedTask()
{
	task_id_ = -1;
	complete_value_ = 0;
}

void GReceivedTask::DebugInfo() const
{
	auto task_db = GTaskDbMgr::GetMe().FindById(task_id_);
	if (task_db)
	{
		LOG_O(Log_trace) << "receive task id:" << task_id_ << "\tcomplete value:" << complete_value_ << " task_name:" << task_db->task_name_;
	}
	else
	{
		LOG_O(Log_trace) << "receive task id:" << task_id_ << "\tcomplete value:" << complete_value_;
	}
}

bool GReceivedTask::IsDone() const
{
	return 4 == complete_value_;
}

void GReceivedTask::DoUpdateClear()
{
	task_id_ = 0;
}

bool GAcceptableTasks::Update()
{
	GPlayerMe::GetMe().UpdateRoleLvl_Notify();
	return __super::Update();
}

void GAcceptableTasks::DebugInfo() const
{
	LOG_O(Log_info) << "可接的任务总数是:" << acceptable_tasks_.size();
	auto& task_mgr = GTaskDbMgr::GetMe();
	task_mgr.RebuildAll();
	for ( auto v : acceptable_tasks_ )
	{
		auto task_template = task_mgr.FindById(v);
		if ( !task_template )
			continue;
		LOG_O(Log_info) << "可接的任务，任务ID：" << v << "\t任务名：" << task_template->task_name_;
	}
}

void GAcceptableTasks::CpyIdToLua( const luabind::object& tbl ) const
{
	int i = 1;
	for ( auto v : acceptable_tasks_ )
	{
		tbl[i++] = v;
	}
}

GAcceptableTasks::GAcceptableTasks()
{
	/*acceptable_tasks_.push_back(2);
	acceptable_tasks_.push_back(3);
	acceptable_tasks_.push_back(1);*/
}

bool GAcceptableTasks::UpdateImpl()
{
	
	acceptable_tasks_.clear();

	auto& mgr = GTaskDbMgr::GetMe();
	if (!mgr.RebuildAll())
	{
		assert(false);
		return false;
	}
	for (auto& v : mgr.GetTasks())
	{
		if (!v.task_db_)
		{
			assert(false);
			continue;
		}
		if (CD_FuncCall::CanAcceptTheTask(*v.task_db_))
		{
			acceptable_tasks_.push_back(v.task_id_);
		}
	}
	
	return true;
}

void GAcceptableTasks::DoUpdateClear()
{
	acceptable_tasks_.clear();
}

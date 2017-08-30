#pragma once
/*
@author zhp
@date 2016/1/25 23:08
@purpose 生产者与消费者模式
*/
#include <Windows.h>
#include <functional>
#include <queue>
#include "../DependSelfLib.h"
#include <cassert>
#include "CmnX_Lock.h"
#include <ProtocolX/P_DefinesX.h>

class Cmnx_TaskQueueBase{
private:
	Cmnx_TaskQueueBase( const Cmnx_TaskQueueBase& );
	Cmnx_TaskQueueBase& operator = ( const Cmnx_TaskQueueBase& );

protected:
	Cmnx_TaskQueueBase();

public:
	virtual ~Cmnx_TaskQueueBase();
	int RunOneTask(DWORD dwMilliseconds = INFINITE);
	int RunSomeTask(int min_task_cnt = 1, DWORD dwMilliseconds = INFINITE);
	int RunTasks(DWORD dwMilliseconds = INFINITE);
	bool RunWait(const FunctionBool& fn, const FunctionVoid* idle_fn, DWORD dwMilliseconds);
	virtual bool IsEmpty() const = 0;
	bool InitTaskQueue();
	virtual bool NotifyEvent() = 0;
	//唤醒自己
	virtual bool WakeUp() = 0;

protected:
	virtual bool InitOnce() = 0;
	virtual bool PreAddTask();
	virtual bool DoTaskLock( bool is_lock ) = 0;
	virtual bool DoWaitForEvent(DWORD dwMilliseconds) = 0;
	virtual void DoPopRun() = 0;

private:
	bool				is_init_;
	bool				init_res_;
};

class Cmnx_TaskQueueBase_ProcessBase : public Cmnx_TaskQueueBase{
public:
	Cmnx_TaskQueueBase_ProcessBase();
	~Cmnx_TaskQueueBase_ProcessBase();

public:
	bool NotifyEvent() override;
	bool WakeUp() override;

protected:
	bool DoWaitForEvent(DWORD dwMilliseconds) override;

protected:
	HANDLE				queue_event_;

private:
	HANDLE				self_event_;
};

class Cmnx_TaskQueueBase_SelfProcess : public Cmnx_TaskQueueBase_ProcessBase{
public:
	Cmnx_TaskQueueBase_SelfProcess();
	~Cmnx_TaskQueueBase_SelfProcess();

protected:
	bool DoTaskLock( bool is_lock );

private:
	bool InitOnce();

private:
	CRITICAL_SECTION	task_lock_;
};

class Cmnx_TaskQueueBase_MultiProcess : public Cmnx_TaskQueueBase_ProcessBase{
public:
	typedef std::shared_ptr<Cmnx_Mutex_MutexA> LockT;

public:
	void SetLockName( const std::string& task_lock_name, const std::string& queue_event_name );
	LockT& GetTaskLock();

protected:
	bool DoTaskLock( bool is_lock );

private:
	bool InitOnce();

private:
	LockT				task_lock_;					
	std::string			task_lock_name_;
	std::string			queue_event_name_;
};

template<typename TasksT_>
class Cmnx_TaskQueue_AlgoMore_Normal{
public:
	static bool PreAddTaskMore( const TasksT_& tasks ){
		return true;
	}
};

template<typename TasksT_>
class Cmnx_TaskQueue_AlgoMore_BuffRecycle{
public:
	static bool PreAddTaskMore( TasksT_& tasks ){
		if (tasks.is_full())
			tasks.pop();
		return true;
	}
};

template<typename TasksT_>
class Cmnx_TaskQueue_TasksNil{
public:
	typedef TasksT_ TasksT;

protected:
	virtual TasksT& GetTasks() = 0;
	virtual const TasksT& GetTasks() const = 0;
};
template<typename TasksT_>
class Cmnx_TaskQueue_TasksHolder{
public:
	typedef TasksT_ TasksT;

protected:
	TasksT& GetTasks(){
		return tasks_;
	}
	const TasksT& GetTasks() const{
		return tasks_;
	}

private:
	TasksT			tasks_;
};

template<typename TaskFuncT_, typename TaskHolderT_, typename AlgoMoreT_, typename TaskQueueBaseT_>
class Cmnx_TaskQueueImplT : public TaskQueueBaseT_, public TaskHolderT_{
public:
	typedef TaskFuncT_ TaskFuncT;
	typedef TaskHolderT_ TasksHolderT;
	typedef AlgoMoreT_ AlgoMoreT;
	typedef TaskQueueBaseT_ TaskQueueBaseT;
	typedef typename TasksHolderT::TasksT TasksT;
	typedef typename TasksT::size_type size_type;

public:
	bool AddTask( const TaskFuncT& task ){
		if ( !PreAddTask() )
		{
			assert(false);
			return false;
		}
		GetTasks().push(task);
		if ( !DoTaskLock(false) )
		{
			assert(false);
			return false;
		}
		if ( !NotifyEvent() )
		{
			assert(false);
			return false;
		}
		return true;
	}
	bool IsEmpty() const{
		return GetTasks().empty();
	}

protected:
	virtual void DoExecTask( TaskFuncT& cur_task ) = 0;
	bool PreAddTask(){
		if ( !__super::PreAddTask() )
		{
			assert(false);
			return false;
		}
		return AlgoMoreT::PreAddTaskMore(GetTasks());
	}

private:
	void DoPopRun(){
		auto& tasks = GetTasks();
		auto cur_task = tasks.front();
		tasks.pop();
		DoTaskLock(false);
		DoExecTask(cur_task);
	}
};

template<typename TaskFuncT_, typename TaskHolderT_, typename AlgoMoreT_, typename TaskQueueBaseT_>
class Cmnx_TaskQueueImpl : public Cmnx_TaskQueueImplT<TaskFuncT_, TaskHolderT_, AlgoMoreT_, TaskQueueBaseT_>{
protected:
	virtual void DoExecTask( TaskFuncT& cur_task ){
		cur_task();
	}
};

class Cmnx_TaskQueue : public Cmnx_TaskQueueImpl<FunctionVoid, Cmnx_TaskQueue_TasksHolder<std::queue<FunctionVoid > >, 
	Cmnx_TaskQueue_AlgoMore_Normal<std::queue<FunctionVoid > >, Cmnx_TaskQueueBase_SelfProcess>{
};
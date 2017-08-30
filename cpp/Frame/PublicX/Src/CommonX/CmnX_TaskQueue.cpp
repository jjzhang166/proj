/*
@author zhp
@date 2016/1/25 23:08
@purpose 生产者与消费者模式
*/
#include "stdafx.h"
#include <CommonX/CmnX_TaskQueue.h>
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>

int Cmnx_TaskQueueBase::RunOneTask(DWORD dwMilliseconds)
{
	return RunSomeTask(1, dwMilliseconds);
}

Cmnx_TaskQueueBase::Cmnx_TaskQueueBase()
{
	is_init_ = false;
	init_res_ = false;
}

Cmnx_TaskQueueBase::~Cmnx_TaskQueueBase()
{

}

int Cmnx_TaskQueueBase::RunTasks(DWORD dwMilliseconds)
{
	auto run_total_task_cnt = 0;
	while (true)
	{
		auto task_cnt = RunOneTask(dwMilliseconds);
		if ( task_cnt <= 0 )
			break;
		run_total_task_cnt += task_cnt;
	}
	return run_total_task_cnt;
}

bool Cmnx_TaskQueueBase::RunWait(const FunctionBool& fn, const FunctionVoid* idle_fn, DWORD dwMilliseconds)
{
	if (fn())
		return true;
	DWORD period = dwMilliseconds == 0 ? 0 : 50;
	for (DWORD time = 0; time <= dwMilliseconds; time += period)
	{
		if (RunOneTask(period) > 0)
		{
			if (fn())
				return true;
		}

		if (idle_fn)
			(*idle_fn)();
	}
	return false;
}

bool Cmnx_TaskQueueBase::PreAddTask()
{
	if ( !InitTaskQueue() )
	{
		assert(false);
		return false;
	}
	if ( !DoTaskLock(true) )
		return false;
	return true;
}

bool Cmnx_TaskQueueBase::InitTaskQueue()
{
	if (is_init_)
		return init_res_;
	is_init_ = true;
	init_res_ = InitOnce();
	return init_res_;
}

int Cmnx_TaskQueueBase::RunSomeTask(int min_task_cnt, DWORD dwMilliseconds)
{
	if (!InitTaskQueue())
	{
		assert(false);
		return -1;
	}
	int cnt = 0;
	while (true)
	{
		if (!DoTaskLock(true))
		{
			assert(false);
			return -1;
		}
		if (IsEmpty())
		{
			if (!DoTaskLock(false))
			{
				assert(false);
				return -1;
			}
			if (!DoWaitForEvent(dwMilliseconds))
			{
				//assert(false);
				return cnt;
			}
			if (cnt >= min_task_cnt)
				return cnt;
		}
		else
		{
			DoPopRun();
			++cnt;
			if (cnt >= min_task_cnt)
				return cnt;
		}
	}
	assert(!"不能执行到这里");
	return -1;
}

bool Cmnx_TaskQueueBase_SelfProcess::DoTaskLock( bool is_lock )
{
	if (is_lock)
		::EnterCriticalSection(&task_lock_);
	else
		::LeaveCriticalSection(&task_lock_);
	return true;
}

bool Cmnx_TaskQueueBase_SelfProcess::InitOnce()
{
	if ( !InitializeCriticalSectionAndSpinCount(&task_lock_, 0) )
	{
		assert(false);
		return false;
	}
	queue_event_ = ::CreateEventA(GetNullDACL(), FALSE,
		FALSE, nullptr);
	if ( !queue_event_ )
	{
		assert(false);
		return false;
	}
	return true;
}

Cmnx_TaskQueueBase_SelfProcess::Cmnx_TaskQueueBase_SelfProcess()
{
	ZeroMemory(&task_lock_, sizeof(task_lock_));
}

Cmnx_TaskQueueBase_SelfProcess::~Cmnx_TaskQueueBase_SelfProcess()
{
	DeleteCriticalSection(&task_lock_);
}

bool Cmnx_TaskQueueBase_MultiProcess::InitOnce()
{
	task_lock_.reset(new Cmnx_Mutex_MutexA(task_lock_name_.empty() ? nullptr : task_lock_name_.c_str()));
	if ( !task_lock_ )
	{
		assert(false);
		return false;
	}
	if (queue_event_name_.empty())
	{
		queue_event_ = ::CreateEventA(GetNullDACL(), FALSE,
			FALSE, nullptr);
	}
	else
	{
		std::string str_name = "Global\\";
		str_name += queue_event_name_;
		queue_event_ = ::CreateEventA(GetNullDACL(), FALSE,
			FALSE, str_name.c_str());
	}	
	
	if ( !queue_event_ )
	{
		assert(false);
		return false;
	}
	return true;
}

void Cmnx_TaskQueueBase_MultiProcess::SetLockName( const std::string& task_lock_name, const std::string& queue_event_name )
{
	task_lock_name_ = task_lock_name;
	queue_event_name_ = queue_event_name;
}

bool Cmnx_TaskQueueBase_MultiProcess::DoTaskLock( bool is_lock )
{
	if ( !task_lock_ )
	{
		assert(false);
		return false;
	}
	if (is_lock)
	{
		task_lock_->lock();
		return true;
	}
	else
	{
		task_lock_->unlock();
		return true;
	}
	assert(false);
	return false;
}

Cmnx_TaskQueueBase_MultiProcess::LockT& Cmnx_TaskQueueBase_MultiProcess::GetTaskLock()
{
	return task_lock_;
}

Cmnx_TaskQueueBase_ProcessBase::Cmnx_TaskQueueBase_ProcessBase()
{
	queue_event_ = nullptr;
	self_event_ = ::CreateEventA(nullptr, FALSE,
		FALSE, nullptr);
	assert(self_event_);
}

bool Cmnx_TaskQueueBase_ProcessBase::DoWaitForEvent(DWORD dwMilliseconds)
{
	HANDLE handles[] = { queue_event_, self_event_ };
	DWORD dwRet = ::WaitForMultipleObjects(_countof(handles), handles, FALSE, dwMilliseconds);
	if (dwRet == WAIT_OBJECT_0 || dwRet == WAIT_OBJECT_0 + 1)
		return true;
	else if (dwRet == WAIT_TIMEOUT)
		return false;
	assert(false);
	return false;
}

bool Cmnx_TaskQueueBase_ProcessBase::NotifyEvent()
{
	if (::SetEvent(queue_event_))
		return true;
	assert(false);
	return false;
}

Cmnx_TaskQueueBase_ProcessBase::~Cmnx_TaskQueueBase_ProcessBase()
{
	if (queue_event_)
	{
		::CloseHandle(queue_event_);
		queue_event_ = nullptr;
	}
}

bool Cmnx_TaskQueueBase_ProcessBase::WakeUp()
{
	if (::SetEvent(self_event_))
		return true;
	assert(false);
	return false;
}

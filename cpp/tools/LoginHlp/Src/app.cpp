#include "stdafx.h"
#include "app.h"
#include <CmnMix/Cmn_LogOut.h>
#include <Net/LoginLocalInfoS.h>


CMN_MY_DLLAPP_DEFINE(App);

//////////////////////////////////////////////////////////////////////////
bool is_run = false;
Cmnx_TaskQueueBase*	g_task_queue = nullptr;
//////////////////////////////////////////////////////////////////////////
App::App()
{
}

bool App::OnRun_()
{
	ThreadablePtr io_thrd(new MyIoThrd);
	assert(io_thrd);
	is_run = true;
	AddThrd(io_thrd, true);
	return true;
}

int App::OnExit_()
{
	is_run = false;
	if (g_task_queue)
	{
		g_task_queue->WakeUp();
		::Sleep(500);
	}
	return __super::OnExit_();
}

void DoRun__(){
	auto p_id = GetCurrentProcessId();
	LOG_O(Log_trace) << "LoginHlp.dll注入成功，process id:" << p_id;
	GainLocalInfoS ser;
	BOOST_SCOPE_EXIT_ALL(&ser){
		ser.Close();
		g_task_queue = nullptr;
	};
	if (!ser.InitSelf())
	{
		assert(false);
		return;
	}
	auto task_queue = ser.GetTaskQueue();
	if (!task_queue)
	{
		assert(false);
		return;
	}
	g_task_queue = task_queue;
	while (is_run)
	{
		LOG_O(Log_trace) << "LoginHlp执行一个任务, process id:" << p_id;
		if (task_queue->RunSomeTask(0) < 0)
			break;
	}
	LOG_O(Log_trace) << "LoginHlp执行完所有任务, process id:" << p_id;
}
bool MyIoThrd::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	this->GetIoService().post(DoRun__);
	return true;
}


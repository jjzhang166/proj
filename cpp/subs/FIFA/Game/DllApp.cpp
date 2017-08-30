#include "stdafx.h"
#include "DllApp.h"
#include <TlHelp32.h>
#include "FuncObjC.h"
#include <cassert>
#include "protect/Protect.h"
#include <Common/EnsureCleanup.h>

CMN_MY_DLLAPP_DEFINE(App);

//全局初始化
extern bool InitGame();

bool MyIoThrd::OnInit_()
{
	boost::shared_ptr<FuncObj_NormalC> con_account(new FuncObj_NormalC);
	if (!__super::OnInit_())
		return false;
	//init log
	g_log_dbg_view.reset(new LogToDbgviewHelper);
	if (!g_log_dbg_view)
	{
		assert(false);
		return false;
	}

	if (!InitGame())
		return false;
	if (!con_account->InitGame())
		return false;
	con_account->ConnectAsync();
	GetIoService().post([this](){
		auto norc = FuncObj_NormalC::GetInstance();
		if (!norc)
			return;
		if (!norc->WaitForConnectDone())
			return;
		if (!g_log_dbg_view)
			return;
		auto log_pre_str = norc->account_;
		log_pre_str += "-";
		log_pre_str += norc->game_ser_name_;
		g_log_dbg_view->SetDbgViewPreString(log_pre_str);
	});
	return true;
}

LuaThrdData* MyIoThrd::CreateLuaVm()
{
	return new GLuaC;
}

int MyIoThrd::OnExit_()
{
	g_log_dbg_view.reset();
	return __super::OnExit_();
}

bool App::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	ThreadablePtr io_thrd(new MyIoThrd);
	assert(io_thrd);
	AddThrd(io_thrd, true);
	return true;
}

App::App()
{
	game_main_wnd_ = nullptr;
	is_log_dbgview_ = false;
}

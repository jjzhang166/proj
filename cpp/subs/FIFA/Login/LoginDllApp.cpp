#include "stdafx.h"
#include "LoginDllApp.h"
#include <cassert>
#include "LoginC.h"
#include "Common/LogOut.h"
#include "LoginAssistS.h"


CMN_MY_DLLAPP_DEFINE(App);

//全局初始化
extern bool InitGame();

App::App()
{

}

bool App::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	if (!InitGame())
	{
		assert(false);
		return false;
	}
	ThreadablePtr io_thrd(new MyIoThrd);
	assert(io_thrd);
	AddThrd(io_thrd, true);
	return true;
}

bool MyIoThrd::OnInit_()
{
	if (!__super::OnInit_())
		return false;
	boost::shared_ptr<LoginC> con_account(new LoginC);
	con_account->ConnectAsync();
	return true;
}

LuaThrdData* MyIoThrd::CreateLuaVm()
{
	return new Lua_l;
}
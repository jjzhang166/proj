#include "stdafx.h"
#include <AppBase/CmnWxThrd.h>

Cmn_WxThrd::Cmn_WxThrd() : wxThread(wxTHREAD_JOINABLE)
{
}

Cmn_WxThrd::ExitCode Cmn_WxThrd::Entry()
{
	return (ExitCode)OnEntry();
}

Cmn_WxThrd::ThrdIdT Cmn_WxThrd::GetThrdId() const
{
	return GetId();
}

bool Cmn_WxThrd::IsThrdAlive() const
{
	return IsAlive();
}

bool Cmn_WxThrd::DoRun()
{
	auto err = Run();
	if (err == wxTHREAD_NO_ERROR)
		return true;
	return false;
}

bool Cmn_WxThrd::DoExit()
{
	__super::DoExit();
	if (!IsAlive())
		return true;
	if (force_terminate_)
	{
		if (wxTHREAD_NO_ERROR == Kill())
			return true;
		assert(false);
		return false;
	}
	auto res = Wait();
	if (res)
	{
		assert(false);
		return false;
	}
	return true;
}

void Cmn_WxThrd::ActiveIoService()
{

}

bool Cmn_WxThrd::Pause()
{
	if (wxThread::Pause() == wxTHREAD_NO_ERROR)
		return true;
	assert(false);
	return false;
}

bool Cmn_WxThrd::Resume()
{
	if (wxThread::Resume() == wxTHREAD_NO_ERROR)
		return true;
	assert(false);
	return false;
}

bool Cmn_ThrdIo::IsIoThrd() const
{
	return true;
}

void Cmn_ThrdIo::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			def("Shell", &Cmn_ThrdIo::__Shell_c__),
			def("Shell", &Cmn_ThrdIo::__Shell_cc__),
			def("Shell", &Cmn_ThrdIo::__Shell_cb__),
			def("Shell", &Cmn_ThrdIo::__Shell_ccb__)
		];
}

luabind::object Cmn_ThrdIo::Shell(const char* lua_cmd, const char* cmd_name, bool async)
{
	return ShellM(lua_cmd, cmd_name, async);
}

void Cmn_ThrdIo::LuaVmSetting(LuaThrdData* lua_vm) const
{
	__super::LuaVmSetting(lua_vm);
	lua_vm->SetMainLuaName("mainio.lua");
}

luabind::object Cmn_ThrdIo::__Shell_c__(const char* lua_cmd)
{
	return Shell(lua_cmd);
}

luabind::object Cmn_ThrdIo::__Shell_cc__(const char* lua_cmd, const char* cmd_name)
{
	return Shell(lua_cmd, cmd_name);
}

luabind::object Cmn_ThrdIo::__Shell_cb__(const char* lua_cmd, bool sync)
{
	return Shell(lua_cmd, nullptr, sync);
}

luabind::object Cmn_ThrdIo::__Shell_ccb__(const char* lua_cmd, const char* cmd_name, bool sync)
{
	return Shell(lua_cmd, cmd_name, sync);
}

void Cmn_Thrd::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			def("Shell", &Cmn_Thrd::__Shell_c__),
			def("Shell", &Cmn_Thrd::__Shell_cc__),
			def("Shell", &Cmn_Thrd::__Shell_cb__),
			def("Shell", &Cmn_Thrd::__Shell_ccb__)
		];
}

luabind::object Cmn_Thrd::Shell(const char* lua_cmd, const char* cmd_name, bool async)
{
	return ShellIo(lua_cmd, cmd_name, async);
}

void Cmn_Thrd::LuaVmSetting(LuaThrdData* lua_vm) const
{
	__super::LuaVmSetting(lua_vm);
	lua_vm->SetMainLuaName("maincmnthrd.lua");
}

luabind::object Cmn_Thrd::__Shell_c__(const char* lua_cmd)
{
	return Shell(lua_cmd);
}

luabind::object Cmn_Thrd::__Shell_cc__(const char* lua_cmd, const char* cmd_name)
{
	return Shell(lua_cmd, cmd_name);
}

luabind::object Cmn_Thrd::__Shell_cb__(const char* lua_cmd, bool sync)
{
	return Shell(lua_cmd, nullptr, sync);
}

luabind::object Cmn_Thrd::__Shell_ccb__(const char* lua_cmd, const char* cmd_name, bool sync)
{
	return Shell(lua_cmd, cmd_name, sync);
}

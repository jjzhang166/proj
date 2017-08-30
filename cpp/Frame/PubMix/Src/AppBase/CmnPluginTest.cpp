#include "stdafx.h"
#include <AppBase/CmnPluginTest.h>
#include <CmnMix/Cmn_LogOut.h>
#include <ProtocolX/P_DefinesX.h>


void CmnPluginTest::BindFrame(lua_State* l)
{
	__super::BindFrame(l);
	using namespace luabind;
	module(l)
		[
			class_<CmnPluginTest, Cmn_DllRunApp>("CmnPluginTest")
			.def("RunWithMsgBox", &CmnPluginTest::RunWithMsgBox)
		];
}

void CmnPluginTest::LuaVmSetting(LuaThrdData* lua_vm) const
{
	__super::LuaVmSetting(lua_vm);
	lua_vm->SetMustHasLuaRoot(true);
}

void CmnPluginTest::RunWithMsgBox(const luabind::object& fn, const std::string& msg)
{
	using namespace luabind;
	if (LUA_TFUNCTION != type(fn))
	{
		assert(false);
		return;
	}
	auto io_s = GetIoServicePtr();
	while (MessageBoxA(nullptr, msg.c_str(), "执行一次按确定，否则按取消", MB_OKCANCEL) == IDOK)
	{
		const auto& msg = CmnExceptionBase::DefCatchCall([&fn](){
			LuaObjCall<void>(fn);
			return kEmptyStr;
		});
		if (!msg.empty())
		{
			LOG_O(Log_info) << msg;
		}
		if (io_s)
			io_s->Poll();
	}
}

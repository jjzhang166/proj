#include "stdafx.h"
#include "Lua_l.h"
#include "LoginC.h"
#include "LoginAssistS.h"
#include <Communicate/CGL_Defines.h>


void Lua_l::BindClass()
{
	__super::BindClass();

	using namespace luabind;
	module(lua())
		[
			class_<LoginC>("LoginC")
			.def(g_call_fn_name_set_accout_wpd, &LoginC::SetAccount_Password)
			.def("SetJoinedService", &LoginC::SetJoinedService)
			.def("SetCrackCaptchaMode", &LoginC::SetCrackCaptchaMode)
			.def("BeginLogin", &LoginC::BeginLogin)
			.def("SetGamePath", &LoginC::SetGamePath),
			def("LoginC_Instance", &LoginC::GetInstance),

			class_<LoginOperC, JsonEchoBase>("LoginOperC")
			.def("cls", &LoginOperC::CheckLoginStr)

			/*class_<LoginAssistOperS, JsonEchoBase>("LoginAssistOperS")
			.def("SafeCheckDone", &LoginAssistOperS::SafeCheckDone)*/
		];
}

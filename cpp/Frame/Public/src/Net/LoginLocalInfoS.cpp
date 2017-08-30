#include "stdafx.h"
#include <Net/LoginLocalInfoS.h>
#include <Common/UsefulClass.h>
#include <protect/Protect.h>

BC_REG_CLS_DEFINE(GainLocalInfoS){
	bufcall::class_<GainLocalInfoS>("GainLocalInfoS")
	.def("GMC", &GainLocalInfoS::GetMachineCode)
	.def("GC", &GainLocalInfoS::GetCookie)
	.def("SC", &GainLocalInfoS::SetCookie);
}

bool GainLocalInfoS::InitSelf()
{
	VMP_BEGIN_EX;
	if (IsValid())
		return true;
	if (OpenFileMap(self_name_))
		return true;
	if (!CreateFileMap(self_name_))
	{
		assert(false);
		return false;
	}
	return true;
	VMP_END;
}

void GainLocalInfoS::GetMachineCode(const std::string& session_src)
{
	VMP_BEGIN_EX;
	if (!PrepareTo(session_src))
		return;
	static auto m_code = LocalInfo::GetTheMachineCode();
	if (!to_.ToCall(BC_MakeP("OGMC") << m_code))
		assert(false);
	to_.Close();
	VMP_END;
}

void GainLocalInfoS::GetCookie(const std::string& session_src, const std::string& whose)
{
	VMP_BEGIN_EX;
	if (!PrepareTo(session_src))
		return;
	/*如果有多个的话，会有问题*/
	//const auto& cookie = CookieHelper::GetMe().GetCookie(GenUuidStr(whose));
	const auto& cookie = LocalInfo::GetTheCookie(GenUuidStr(whose));
	if (!to_.ToCall(BC_MakeP("OGC") << cookie))
		assert(false);
	to_.Close();
	VMP_END;
}

void GainLocalInfoS::SetCookie(const std::string& session_src, const std::string& whose, const std::string& cookie)
{
	VMP_BEGIN_EX;
	if (!PrepareTo(session_src))
		return;
	//CookieHelper::GetMe().UpdateCookie(whose, cookie);
	LocalInfo::SetNewCookie(whose, cookie);
	if (!to_.ToCall(BC_MakeP("OSC")))
		assert(false);
	to_.Close();
	VMP_END;
}

GainLocalInfoS::GainLocalInfoS()
{
	self_name_ = kLoginLocalInfo_FileMapNameS;
}

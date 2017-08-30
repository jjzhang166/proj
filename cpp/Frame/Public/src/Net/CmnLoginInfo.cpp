#include "stdafx.h"
#include <Net/CmnLoginInfo.h>
#include <algorithm>

const char* const CmnLoginInfo::kLoginData_LoginSucceed = "8BC89803E607";

const char* const CmnLoginInfo::kLoginData_AcType = "aBC8a803E697";

const char* const CmnLoginInfo::kLoginData_LoginInfo = "740A881FE4B4";

const char* const CmnLoginInfo::kLoginData_EndDateTime = "C3B0EAC9FE44";

const char* const CmnLoginInfo::kLoginData_AddrDatas = "37514EA3BC82";

const char* const CmnLoginInfo::kLoginData_StrDatas = "B4B3DA1BEC7D";

const char* const CmnLoginInfo::kLoginData_AcceptMaxCnt = "7204F7B993C1";

const char* const CmnLoginInfo::kLoginData_BindGameAcInfo = "BAB6DF9081A1";

CmnLoginInfo* CmnLoginInfoMgrBase::FindByAc(const std::string& ac)
{
	for (auto& v : login_info_)
	{
		if (v.ac_ == ac)
			return &v;
	}
	return nullptr;
}

bool CmnLoginInfoMgrBase::DelByAc(const std::string& ac)
{
	auto it = std::find_if(login_info_.begin(), login_info_.end(), [&ac](const CmnLoginInfo& ac_pwd){
		return ac == ac_pwd.ac_;
	});
	if (it != login_info_.end())
	{
		it->ResetAll();
		login_info_.erase(it);
		return true;
	}
	return false;
}

void CmnLoginInfoMgrBase::DelAllByAc(const std::string& ac)
{
	while (DelByAc(ac)){}	
}

void CmnLoginInfoMgrBase::UniqueLoginInfo()
{
	std::vector<std::string> the_ac;
	for (auto& v : login_info_)
		the_ac.push_back(v.ac_);
	if (the_ac.empty())
		return;
	std::sort(the_ac.begin(), the_ac.end());
	auto it_end = Unique(the_ac.begin(), the_ac.end());
	while (it_end != the_ac.end())
	{
		DelAllByAc(*it_end++);
	}
}

const std::vector<CmnLoginInfo>& CmnLoginInfoMgrBase::GetLoginInfos() const
{
	return login_info_;
}

void CmnLoginInfoMgrBase::AddLoginInfo(const std::string& ac, const std::string& pwd)
{
	CmnLoginInfo tmp;
	tmp.ac_ = ac;
	tmp.pwd_ = pwd;
	tmp.GenUuid();
	login_info_.push_back(tmp);
}

void CmnLoginInfoMgrBase::ClearAllLoginInfos()
{
	for ( auto& v : login_info_ )
	{
		v.ResetAll();
	}
	login_info_.clear();
}

pt_dword CmnLoginInfoMgrBase::GetConnectedMaxCnt() const
{
	pt_dword cnt = 0;
	for (auto& v : login_info_)
	{
		if (!v.IsTheLoginSucceed())
			continue;
		auto& res = v.GetBindGameAcInfo();
		cnt += res.size();
	}
	return cnt;
}

CmnLoginInfoMgrBaseImpl::CmnLoginInfoMgrBaseImpl()
{
	lua_data_ = nullptr;
}

void CmnLoginInfoMgrBaseImpl::SetLuaData(CmnPriLuaData* lua_data)
{
	assert(lua_data);
	lua_data_ = lua_data;
}

pt_dword CmnLoginInfoMgrBaseImpl::GetMaxConnectedCntByAcType(pt_dword ac_type) const
{
	return 100;
}

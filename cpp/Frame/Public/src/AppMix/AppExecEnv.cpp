#include "stdafx.h"
#include <AppMix/AppExecEnv.h>
#include <boost/filesystem/path.hpp>

bool AppExecEnvBase::Init()
{
	exec_env_.env.clear();
	if (!wxGetEnvMap(&exec_env_.env))
	{
		assert(false);
		return false;
	}
	return true;
}

long AppExecEnvBase::EnvCreateProcess(const std::string& file_path, const std::string& item_key)
{
	exec_env_.env["__MyZhpAcItemKey__"] = item_key;
	boost::filesystem::path cwd = file_path;
	exec_env_.cwd = cwd.parent_path().native();
	auto pid = wxExecute(file_path, create_flags_, nullptr, &exec_env_);
	assert(pid != 0);
	return pid;
}

std::string AppExecEnvBase::GetEnvAcItemKey()
{
	wxString str_tmp;
	if (!wxGetEnv(wxT("__MyZhpAcItemKey__"), &str_tmp))
	{
		assert(false);
		return std::string();
	}
	return std::string(str_tmp);
}

AppExecEnvBase::~AppExecEnvBase()
{

}

AppExecEnvBase::AppExecEnvBase()
{
	create_flags_ = wxEXEC_ASYNC | wxEXEC_NOEVENTS;
}

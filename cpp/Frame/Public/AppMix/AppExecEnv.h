#pragma once
/*
@author zhp
@date 2017/4/3 17:53
@purpose for app exec env
*/
#include <wx/wxprec.h>
#include <ProtocolX/P_DefinesX.h>

class AppExecEnvBase{
public:
	AppExecEnvBase();
	virtual ~AppExecEnvBase();
	bool Init();
	static std::string GetEnvAcItemKey();
	long EnvCreateProcess(const std::string& file_path, const std::string& item_key);
	CMN_CAST_TO

protected:
	wxExecuteEnv		exec_env_;
	int					create_flags_;
};

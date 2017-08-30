#pragma once
#include "stdafx.h"
#include "P_Defines.h"
#include "../DependSelfLib.h"

//account
struct P_Modify_Account{
	enum 
	{
		value = 1
	};

	bool			auto_run_script_;					//是否自动运行脚本
	char			account_[kAccountMaxLen + 1];
	char			password_[kPasswordMaxLen + 1];
	char			service_name_[50];
};

//加载测试DLL
struct P_Exec_TestLoadLib{
	P_Exec_TestLoadLib();
	enum
	{
		value = 2
	};
	char			str_dll_full_file_path_[MAX_PATH];		//dll的全路径
};
//卸载测试DLL
struct P_Exec_TestUnloadLib
{
	enum
	{
		value = 3
	};
};

//调用Lua中的函数：LuaTest
struct P_Exec_CallLuaFunction
{
	P_Exec_CallLuaFunction();
	enum
	{
		value = 4
	};
	enum
	{
		kStrSize = 100
	};
	char			call_func_name[kStrSize];
	char			func_param[kStrSize];
};

struct P_Exec_CallLuaFuncResult : public P_Exec_CallLuaFunction
{
	P_Exec_CallLuaFuncResult();
	enum
	{
		value = 5
	};
	
	char			func_result[kStrSize];
};

//比赛信息
struct P_Exec_MatchInfo
{
	enum
	{
		value = 6
	};
	time_t				time_match_begin;			//比赛开始时间
	time_t				time_match_end;				//比赛结束时间
	int					match_cnt;					//比赛次数
};

//游戏进程id
struct P_Exec_GameProccessInfo
{
	enum
	{
		value = 7
	};

	DWORD				login_client_proccess_id_;			//登陆进程ID
	DWORD				self_process_id_;
};

struct P_Exec_Login_AcPwd{
	enum 
	{
		value = 8
	};

	char			account_[kAccountMaxLen + 1];
	char			password_[kPasswordMaxLen + 1];
};

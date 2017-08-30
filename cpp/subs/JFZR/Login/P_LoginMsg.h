#pragma once
/*
@author zhp
@date 2014/4/9 19:07
@purpose for msg definition
*/
#include "stdafx.h"

enum enLoginMsg_Exec{
	kLoginMsg_BindLoginPid = 1,			//关联login proccess id，P_Exec_BindLoginPid
};

struct P_Exec_BindLoginPid{
	DWORD				login_pid_;		//登陆进程的proccess id
};
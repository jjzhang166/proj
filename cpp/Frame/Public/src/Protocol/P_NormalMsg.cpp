#include "stdafx.h"
#include "Protocol/P_NormalMsg.h"

P_Exec_TestLoadLib::P_Exec_TestLoadLib()
{
	str_dll_full_file_path_[0] = 0;
}

P_Exec_CallLuaFunction::P_Exec_CallLuaFunction()
{
	call_func_name[0] = '\0';
	func_param[0] = '\0';
}

P_Exec_CallLuaFuncResult::P_Exec_CallLuaFuncResult()
{
	func_result[0] = '\0';
}

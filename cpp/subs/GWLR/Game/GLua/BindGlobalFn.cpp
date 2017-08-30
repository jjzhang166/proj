#include "stdafx.h"
#include "GLuaC.h"
#include "FuncObjC.h"
#include <CmnAntiCheck/CmnRbuThread.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <Common/Toolhelp.h>
#include "../GloFuncs.h"


//设置脚本输出信息
void SetScriptOutputInfo( const char* output_info )
{
	if ( !output_info )
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "output_info为空";
		return;
	}
	auto normal_c = FuncObj_NormalC::GetInstance();
	if (normal_c)
	{
		normal_c->GetSessionOper().ToCall(BC_MakeP("SetScriptOutputInfo") << output_info);
	}
}

bool IsStringEmpty( const std::string& str )
{
	return str.empty();
}

void OutputScriptInfo(const std::string& str)
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return;
	norc->GetLuaScript().OutputScriptInfo(str);
}

void L_Test(pt_dword thrd_id)
{
	CToolhelp th(TH32CS_SNAPMODULE, GetCurrentProcessId());
	MODULEENTRY32 m_entry;
	m_entry.dwSize = sizeof(m_entry);
	BOOL fFound = FALSE;
	for (BOOL fOk = th.ModuleFirst(&m_entry); fOk; fOk = th.ModuleNext(&m_entry)) {
		LOG_O(Log_trace) << m_entry.szModule;
	}
	
}

void GLuaC::BindGlobalFn()
{
	__super::BindGlobalFn();
	using namespace luabind;

	module(lua())
		[
			def("SetScriptOutputInfo", &SetScriptOutputInfo),
			def("OutputScriptInfo", &OutputScriptInfo),
			def("IsStringEmpty", &IsStringEmpty),
			def("L_Test", &L_Test),
			def("EnablePassRoom", &EnablePassRoom)
		];
}

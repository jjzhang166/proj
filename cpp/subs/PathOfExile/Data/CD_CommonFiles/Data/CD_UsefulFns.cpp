#include "stdafx.h"
#include "CD_UsefulFns.h"
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>
#include "CD_Const.h"


pt_dword CD_GetExeModule()
{
	static pt_dword module = (pt_dword)GetModuleHandleA("PathOfExile.exe");
	assert(module);
	return module;
}

pt_dword CD_GetPathOfExileBase()
{
	static pt_dword module = (pt_dword)GetModuleHandleA("PathOfExileBase.dll");
	assert(module);
	return module;
}

StaticTaskQueueT& GetStaticTaskQueue()
{
	static StaticTaskQueueT g_static_task_queue_;
	return g_static_task_queue_;
}

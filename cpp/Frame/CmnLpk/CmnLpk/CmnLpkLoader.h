#pragma once
/*
@author zhp
@date 2016/6/10 17:16
@purpose for lpk loader
*/
#include <Windows.h>
#include "../DependSelfLib.h"

typedef bool(*CmnLpk_CallBackT)(HMODULE self_module);
BOOL DoCmnLpkLoad(HMODULE self_module);
void DoCmnLpkFree();
void SetCmnLpkLoaderInfo(const char* exe_name, CmnLpk_CallBackT call_back);
#include "stdafx.h"
#include "GloFns.h"
#include <cassert>

DWORD GetModuleFifazfbase()
{
	static DWORD dwCryGameBaseAddr = (DWORD)GetModuleHandleA("fifazfBase.dll");

	assert(dwCryGameBaseAddr);

	return dwCryGameBaseAddr;
}

std::string GetDefFileDir()
{
	static std::string def_dir("f:\\tmp\\FeatureCode_fi\\");
	return def_dir;
}

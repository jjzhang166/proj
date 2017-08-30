#include "stdafx.h"
#include "GloFns.h"
#include <cassert>

DWORD GetModuleCryGameBase()
{
	static DWORD dwCryGameBaseAddr = (DWORD)GetModuleHandleA("CryGame.dll");

	assert(dwCryGameBaseAddr);

	return dwCryGameBaseAddr;
}

DWORD GetModuleCryActionBase()
{
	static DWORD dwCryGameBaseAddr = (DWORD)GetModuleHandleA("CryAction.dll");

	assert(dwCryGameBaseAddr);

	return dwCryGameBaseAddr;
}

DWORD GetModuleMhoClientBase()
{
	static DWORD dwCryGameBaseAddr = (DWORD)GetModuleHandleA("MHOClientBase.dll");

	assert(dwCryGameBaseAddr);

	return dwCryGameBaseAddr;
}

DWORD GetModuleTersafeBase()
{
	static DWORD dwCryGameBaseAddr = (DWORD)GetModuleHandleA("tersafe.dll");

	assert(dwCryGameBaseAddr);

	return dwCryGameBaseAddr;
}

std::string GetDefFileDir()
{
	static std::string def_dir("f:\\tmp\\FeatureCode_gw\\");
	return def_dir;
}

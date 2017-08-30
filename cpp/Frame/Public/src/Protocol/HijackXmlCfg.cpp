#include "stdafx.h"
#include "Protocol/HijackXmlCfg.h"

stHijackDllCfg::stHijackDllCfg()
{
	injected_game_name_ = _T("RequreExeName.exe");
	inject_dll_full_path_ = _T("");
}

LPCTSTR stHijackDllCfg::GetCfgFileName()
{
	return _T("d3d9.xml");
}

const char* const stHijackProfile::kMainSetion = "cfg";

bool stHijackProfile::Load()
{
	Read(kMainSetion, "name", injected_game_name_, injected_game_name_);
	Read(kMainSetion, "path", inject_dll_full_path_, inject_dll_full_path_);
	return true;
}

bool stHijackProfile::Save()
{
	std::string tmp;
	Read(kMainSetion, "name", tmp, kEmptyStr);
	if (tmp != injected_game_name_)
		Write(kMainSetion, "name", injected_game_name_);
	Read(kMainSetion, "path", tmp, kEmptyStr);
	if (tmp != inject_dll_full_path_)
		Write(kMainSetion, "path", inject_dll_full_path_);
	return true;
}

LPCTSTR stHijackProfile::GetCfgFileName()
{
	return _T("d3d9.cfg");
}

#pragma once
/*
@author zhp
@date 2014-3-9 21:59
@purpose for hijack dll xml config
*/
#include "stdafx.h"
#include "Common/XmlCfg.h"
#include <boost/serialization/version.hpp>
#include "Common/UsefulClass.h"
#include "../DependSelfLib.h"

//劫持DLL使用
struct stHijackDllCfg
{
	stHijackDllCfg();
	//配置文件名
	static LPCTSTR GetCfgFileName();

	friend class boost::serialization::access;
	template<class Archive>
	void serialize( Archive & ar, const unsigned int version ){
		ar & BOOST_SERIALIZATION_NVP(injected_game_name_)
			& BOOST_SERIALIZATION_NVP(inject_dll_full_path_);
	}

	CString			injected_game_name_;			//被注入的游戏的模块名
	CString			inject_dll_full_path_;			//注入DLL的全路径
};
BOOST_CLASS_VERSION(stHijackDllCfg, 1)

class stHijackProfile : public IniProfile{
public:
	bool Load();
	bool Save();
	//配置文件名
	static LPCTSTR GetCfgFileName();

public:
	std::string		injected_game_name_;
	std::string		inject_dll_full_path_;

public:
	static const char* const kMainSetion;
};
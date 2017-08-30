#pragma once
#include "../Console/CfgStruct.h"

struct Ac_SingletonXml : public stSingletonCfg{
	Ac_SingletonXml( const std::string& str_cfg_path ) : stSingletonCfg(str_cfg_path){}

	static Ac_SingletonXml& GetSelf( const std::string& str_sing_path ){
		static Ac_SingletonXml s_x(str_sing_path/*"cfg\\singleton.xml quan lu jing"*/);
		return s_x;
	}
};
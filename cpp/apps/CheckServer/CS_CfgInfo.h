#pragma once
/*
@author zhp
@date 2015-3-27
@purpose configure of check server 
*/
#include "Common/XmlCfg.h"
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include <Common/UsefulFunc.h>

struct stCS_CfgInfo{
public:
	stCS_CfgInfo(){
		self_svr_port_ = 0;
		use_reg_key_ = true;
		radio_time_unit_ = 0;
		decrement_time_ = 3;
		min_version_ = 1;
	}

	friend class boost::serialization::access;
	template<class Archive>
	void serialize( Archive & ar, const unsigned int version ){
		ar & BOOST_SERIALIZATION_NVP(singleton_event_name_)
			& BOOST_SERIALIZATION_NVP(wnd_title_)
			& BOOST_SERIALIZATION_NVP(sql_svr_ip_)
			& BOOST_SERIALIZATION_NVP(proj_db_name_)
			& BOOST_SERIALIZATION_NVP(self_svr_port_)
			& BOOST_SERIALIZATION_NVP(use_reg_key_)
			& BOOST_SERIALIZATION_NVP(radio_time_unit_)
			& BOOST_SERIALIZATION_NVP(decrement_time_)
			& BOOST_SERIALIZATION_NVP(min_version_)
			;
	}
	static stCS_CfgInfo& GetMe(){
		struct stCfgHlp{
			stCfgHlp(){
				XmlCfg::RestoreHelper<stCS_CfgInfo> restore_helper(&cfg_info_, 
					stCS_CfgInfo::GetFullFileName());
			}
			stCS_CfgInfo		cfg_info_;
		};
		static stCfgHlp the_me;	
		return the_me.cfg_info_;
	}
	void Save(){
		XmlCfg::Save(*this, stCS_CfgInfo::GetFullFileName());
	}

private:
	static std::string GetFullFileName(){
		auto the_path = GetModulePath(nullptr);
		CPath the_full_path(the_path);
		the_full_path.Append(_T("cs_svr_info.xml"));
		return std::string(CT2CA(the_full_path.m_strPath));
	}

public:
	std::string		singleton_event_name_;
	std::string		wnd_title_;
	std::string		sql_svr_ip_;
	std::string		proj_db_name_;
	WORD			self_svr_port_;
	bool			use_reg_key_;
	int				radio_time_unit_;			//时间模式，采用的是以天为单位，还是以时间为单位
	int				decrement_time_;			//时间减量
	int				min_version_;
};
BOOST_CLASS_VERSION(stCS_CfgInfo, 1)

#pragma once
/*
@author zhp
@date 2014/7/15 23:25
@purpose for csviewer,config
*/
#include <Common/XmlCfg.h>

//global/singleton xml
//顾名思义，全局唯一的配置
struct stSingletonCfg
{
protected:
	stSingletonCfg( const std::string& str_cfg_full_path ){
		radio_time_unit_ = 0;
		rollback_direction_ = true;
		XmlCfg::RestoreHelper<stSingletonCfg> restore_helper(this, 
			str_cfg_full_path);
	}

public:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize( Archive & ar, const unsigned int version ){
		ar	& BOOST_SERIALIZATION_NVP(authen_account_)
			& BOOST_SERIALIZATION_NVP(authen_password_)
			& BOOST_SERIALIZATION_NVP(radio_time_unit_)
			& BOOST_SERIALIZATION_NVP(rollback_direction_)
			;
	}
	//保存
	void Save(){
		XmlCfg::Save(*this, GetSingletonFullFileName());
	}

	//type definition
public:

public:
	//唯一实例
	static stSingletonCfg& GetMe(){
		static stSingletonCfg me(GetSingletonFullFileName());
		return me;
	}
	static const std::string& GetSingletonFullFileName(){
		struct Helper
		{
			Helper(){
				full_file_name = XmlCfg::GetCfgPath() + "csv_singleton.xml";
			}
			std::string			full_file_name;
		};
		static Helper helper;
		return helper.full_file_name;
	}

public:
	//for authenticate
	CString									authen_account_;				//wg account
	CString									authen_password_;				//wg password
	int										radio_time_unit_;				//卡的时间单位
	bool									rollback_direction_;			//支付回滚的方向，如果true表示减时间，否则是加时间
};
BOOST_CLASS_VERSION(stSingletonCfg, 1)

struct stCSV_SvrInfo{
public:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize( Archive & ar, const unsigned int version ){
		ar & BOOST_SERIALIZATION_NVP(singleton_event_name_)
			& BOOST_SERIALIZATION_NVP(wnd_title_)
			& BOOST_SERIALIZATION_NVP(check_svr_domain_)
			& BOOST_SERIALIZATION_NVP(check_svr_port_)
			;
	}
	static stCSV_SvrInfo& GetMe(){
		struct stCfgHlp{
			stCfgHlp(){
				XmlCfg::RestoreHelper<stCSV_SvrInfo> restore_helper(&cfg_info_, 
					stCSV_SvrInfo::GetFullFileName());
			}
			stCSV_SvrInfo		cfg_info_;
		};
		static stCfgHlp the_me;	
		return the_me.cfg_info_;
	}
	void Save(){
		XmlCfg::Save(*this, stCSV_SvrInfo::GetFullFileName());
	}

private:
	static std::string GetFullFileName(){
		auto the_path = GetModulePath(nullptr);
		CPath the_full_path(the_path);
		the_full_path.Append(_T("Cfg\\csv_svr_info.xml"));
		return std::string(CT2CA(the_full_path.m_strPath));
	}

public:
	std::string		singleton_event_name_;
	std::string		wnd_title_;
	std::string		check_svr_domain_;
	WORD			check_svr_port_;
};
BOOST_CLASS_VERSION(stCSV_SvrInfo, 1)

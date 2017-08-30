#include "stdafx.h"
#include <Common/XmlCfg.h>
#include <Common/UsefulFunc.h>

const std::string& XmlCfg::GetCfgPath()
{
	struct Helper{
		Helper(){
			TCHAR cfg_file_name[MAX_PATH];
			if (GetModulePath(nullptr, cfg_file_name, sizeof(cfg_file_name)))
			{
				_tcscat_s(cfg_file_name, _T("Cfg\\"));
				file_path = CT2CA(cfg_file_name);
				CreateDirectory(cfg_file_name, nullptr);
			}
		}

		std::string		file_path;
	};

	static Helper helper;
	
	return helper.file_path;
}

std::string XmlCfg::GenFileNameBy( const char* account )
{
	return std::string(account) + ".xml";
}

std::string XmlCfg::MakeFullFileName( const char* account )
{
	return GetCfgPath() + GenFileNameBy(account);
}

const char* XmlCfg::GetFileName_DefaultCfg()
{
	return "default.xml";
}

const std::string& XmlCfg::GetFullFileName_SingletonCfg()
{
	struct Helper
	{
		Helper(){
			full_file_name = GetCfgPath() + "singleton.xml";
		}
		std::string			full_file_name;
	};
	static Helper helper;
	return helper.full_file_name;
}

namespace boost{
	namespace serialization{
		void serialize( boost::archive::xml_oarchive& ar, CString& str, const unsigned int var )
		{
			std::string str_(CT2CA((LPCTSTR)str));
			//ar & BOOST_SERIALIZATION_NVP(str_);
			ar << boost::serialization::make_nvp("CString", str_);
		}
		void serialize( boost::archive::xml_iarchive& ar, CString& str, const unsigned int var ) 
		{
			std::string str_;
			ar & boost::serialization::make_nvp("CString", str_);
			//ar & BOOST_SERIALIZATION_NVP(str_);
			str = CA2CT(str_.c_str());
		}
	}
}
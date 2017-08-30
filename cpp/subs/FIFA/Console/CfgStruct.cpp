#include "stdafx.h"
#include "CfgStruct.h"
#include "Protocol/P_Defines.h"
#include <boost/algorithm/string.hpp>
#include <Common/UsefulFunc.h>
#include <protect/Protect.h>
#include "ListCtrls.h"
#include "TabOption.h"

void stSpecificCfg::Save( const char* file_name )
{
	if (file_name)
		XmlCfg::Save(*this, XmlCfg::GetCfgPath() + file_name);
}

void stSpecificCfg::Restore( const char* file_name )
{
	XmlCfg::RestoreHelper<stSpecificCfg> helper(this, XmlCfg::GetCfgPath() + file_name);
}

void stSpecificCfg::SaveAsAccount( const std::string& ac, int suffix )
{
	if ( suffix < 0 )
	{
		assert(false);
		return;
	}

	char str_suffix[30];
	_itoa(suffix, str_suffix, 10);

	std::string file_name(ac);
	file_name += '_';
	file_name += str_suffix;
	file_name += ".xml";
	Save(file_name.c_str());
}

void stSpecificCfg::RestoreAsAccount( const std::string& ac, int suffix )
{
	if ( suffix < 0 )
	{
		assert(false);
		return;
	}
	char str_suffix[30];
	_itoa(suffix, str_suffix, 10);
	
	std::string file_name(ac);
	file_name += '_';
	file_name += str_suffix;
	file_name += ".xml";
	Restore(file_name.c_str());
}

bool stSpecificCfg::HasSpecificCfg( const std::string& ac, int suffix )
{
	if ( suffix < 0 )
	{
		assert(false);
		return false;
	}
	char str_suffix[30];
	_itoa(suffix, str_suffix, 10);
	
	std::string file_name(ac);
	file_name += '_';
	file_name += str_suffix;
	file_name += ".xml";

	return !!PathFileExistsA((XmlCfg::GetCfgPath() + file_name).c_str());
}

stDefaultCfg& stDefaultCfg::GetMe()
{
	static stDefaultCfg me;
	return me;
}

stDefaultCfg::stDefaultCfg()
{
	script_name_ = "普通号脚本.lua";
	//game_service_ = "华东电信";
	auto_run_script_ = true;
	game_ac_type_ = kGAT_Normal;
	Restore(XmlCfg::GetFileName_DefaultCfg());
}

void stDefaultCfg::Save()
{
	__super::Save(XmlCfg::GetFileName_DefaultCfg());
}

stSingletonCfg& stSingletonCfg::GetMe()
{
	static stSingletonCfg me(XmlCfg::GetFullFileName_SingletonCfg());
	return me;
}

void stSingletonCfg::Save()
{
	XmlCfg::Save(*this, XmlCfg::GetFullFileName_SingletonCfg());
}

void stSingletonCfg::DoCloseTheSession(const BindAcInfoT& the_clr_ac)
{
	if (the_clr_ac.empty())
		return;
	SetIoTimer(1000 * 6, [the_clr_ac](IoTimer*){
		VMP_BEGIN_EX;
		auto page_list = PageAccount_List::GetInstance();
		if (!page_list)
		{
			assert(false);
			throw CmnExitIoThreadException("fdjkalcm");
		}
		for (auto& v : the_clr_ac)
		{
			auto item = page_list->FindItemByAccount(v.first, v.second);
			if (item < 0)
			{
				//assert(false);
				continue;
			}
			auto nor = page_list->GetNormalS(item);
			if (nor)
				nor->Disconnect();
		}
		auto tab_option = CTabOption::GetInstance();
		if (tab_option)
			tab_option->ClearLoginInfo();
		VMP_END;
		if (the_clr_ac.empty())
			return;
		SetIoTimer(1000 * 10, [](IoTimer*){
			VMP_BEGIN_EX;
			stSingletonCfg::GetMe().RectifyAcceptMaxCnt();
			VMP_END;
		});
	});
}

AcIniCfgMgr::stAcIniCfg::stAcIniCfg()
{
	service_name_ = _T("华东电信");
	ep_ = 0;
}

bool AcIniCfgMgr::stAcIniCfg::Read( std::size_t setion_idx, LPCTSTR full_file_name )
{
	TCHAR szsetion[50];
	if ( 0 != _itot_s(setion_idx, szsetion, 10) )
		return false;

	TCHAR szbuffer[MAX_PATH];
	static CString kStrDlt(_T(""));

	//帐号
	GetPrivateProfileString(szsetion, _T("帐号"), kStrDlt, szbuffer, sizeof(szbuffer), full_file_name);
	if ( kStrDlt == szbuffer )
		return false;
	ac_ = szbuffer;

	//密码
	GetPrivateProfileString(szsetion, _T("密码"), kStrDlt, szbuffer, sizeof(szbuffer), full_file_name);
	if ( kStrDlt == szbuffer )
		return false;
	pwd_ = szbuffer;

	//服务器
	GetPrivateProfileString(szsetion, _T("服务器"), kStrDlt, szbuffer, sizeof(szbuffer), full_file_name);
	if ( kStrDlt != szbuffer )
	{
		service_name_ = szbuffer;
	}
	return true;
}

bool AcIniCfgMgr::stAcIniCfg::Write( std::size_t setion_idx, LPCTSTR full_file_name ) const
{
	TCHAR szsetion[50];
	if ( 0 != _itot_s(setion_idx, szsetion, 10) )
		return false;
	if ( !WritePrivateProfileString(szsetion, _T("帐号"), ac_, full_file_name) )
	{
		assert(false);
		return false;
	}
	if ( !WritePrivateProfileString(szsetion, _T("密码"), pwd_, full_file_name) )
	{
		assert(false);
		return false;
	}
	if ( !WritePrivateProfileString(szsetion, _T("服务器"), service_name_, full_file_name) )
	{
		assert(false);
		return false;
	}
	return true;
}

bool AcIniCfgMgr::stAcIniCfg::ReadLine( const std::string& str_line )
{
	std::vector<std::wstring> vec_seg_tag;    
	static const std::wstring kTags(L"  ，,\t");
	boost::split(vec_seg_tag, GbkToWideChar(str_line), boost::is_any_of(kTags), boost::token_compress_on);
	if ( vec_seg_tag.size() < 3 )
		return false;

	ac_ = CW2CT(vec_seg_tag[0].c_str());
	pwd_ = CW2CT(vec_seg_tag[1].c_str());
	service_name_ = CW2CT(vec_seg_tag[2].c_str());
	return true;
}

bool AcIniCfgMgr::stAcIniCfg::WriteLine( std::string& str_line ) const
{
	str_line = CT2CA(ac_);
	str_line += ',';
	str_line += CT2CA(pwd_);
	str_line += ',';
	str_line += CT2CA(service_name_);
	str_line += ',';
	str_line += std::to_string(ep_);
	return true;
}

const AcIniCfgMgr::ContT& AcIniCfgMgr::GetCont() const
{
	return cfg_;
}

bool AcIniCfgMgr::Load( const CPath& full_file_name )
{
	if ( !full_file_name.FileExists() )
	{
		assert(false);
		return false;
	}
	cfg_.clear();

	for ( std::size_t i = 1;i != 500; ++i )
	{
		stAcIniCfg cfg_tmp;
		if ( !cfg_tmp.Read(i, full_file_name) )
			return false;
		cfg_.push_back(cfg_tmp);
	}
	return true;
}

bool AcIniCfgMgr::Save( const CPath& full_file_name )
{
	std::size_t idx = 1;
	for ( auto it = cfg_.begin(); it != cfg_.end(); ++it, ++idx )
	{
		if ( !it->Write(idx, full_file_name) )
			return false;
	}
	return true;
}

void AcIniCfgMgr::Add( const CString& ac, const CString& pwd, const CString& service_name, int ep )
{
	stAcIniCfg tmp;
	tmp.ac_ = ac;
	tmp.pwd_ = pwd;
	if ( !service_name.IsEmpty() )
		tmp.service_name_ = service_name;
	tmp.ep_ = ep;
	cfg_.push_back(tmp);
}

bool AcIniCfgMgr_New::Load( const CPath& full_file_name )
{
	if ( !full_file_name.FileExists() )
	{
		assert(false);
		return false;
	}
	cfg_.clear();

	std::ifstream fstm(CT2CA(full_file_name.m_strPath));
	std::string str_line;
	while (fstm.good())
	{
		std::getline(fstm, str_line);
		stAcIniCfg cfg_tmp;
		if ( !cfg_tmp.ReadLine(str_line) )
			return false;
		cfg_.push_back(cfg_tmp);
	}
	return true;
}

bool AcIniCfgMgr_New::Save( const CPath& full_file_name )
{
	std::ofstream oftm(full_file_name.m_strPath, std::ios_base::out | std::ios_base::trunc);
	std::string str_line;
	for ( const auto& ac_cfg : cfg_ )
	{
		if ( !ac_cfg.WriteLine(str_line) )
		{
			assert(false);
			return false;
		}
		oftm << str_line << '\n';
	}
	return true;
}

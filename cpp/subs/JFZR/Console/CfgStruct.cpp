#include "stdafx.h"
#include "CfgStruct.h"
#include "Protocol/P_Defines.h"
#include <boost/algorithm/string.hpp>
#include <Common/UsefulFunc.h>

void stSpecificCfg::Save( const char* file_name )
{
	if (file_name)
		XmlCfg::Save(*this, XmlCfg::GetCfgPath() + file_name);
}

void stSpecificCfg::Restore( const char* file_name )
{
	XmlCfg::RestoreHelper<stSpecificCfg> helper(this, XmlCfg::GetCfgPath() + file_name);
}

void stSpecificCfg::SaveAsAccount( const std::string& ac, int server_id )
{
	if ( server_id < 0 )
	{
		assert(false);
		return;
	}
	server_id = ServerIdx2Suffix(server_id);
	char str_suffix[30];
	_itoa(server_id, str_suffix, 10);

	std::string file_name(ac);
	file_name += '_';
	file_name += str_suffix;
	file_name += ".xml";
	Save(file_name.c_str());
}

void stSpecificCfg::RestoreAsAccount( const std::string& ac, int server_id )
{
	if ( server_id < 0 )
	{
		assert(false);
		return;
	}
	server_id = ServerIdx2Suffix(server_id);
	char str_suffix[30];
	_itoa(server_id, str_suffix, 10);
	
	std::string file_name(ac);
	file_name += '_';
	file_name += str_suffix;
	file_name += ".xml";
	Restore(file_name.c_str());
}

bool stSpecificCfg::HasSpecificCfg( const std::string& ac, int server_id )
{
	if ( server_id < 0 )
	{
		assert(false);
		return false;
	}
	server_id = ServerIdx2Suffix(server_id);
	char str_suffix[30];
	_itoa(server_id, str_suffix, 10);
	
	std::string file_name(ac);
	file_name += '_';
	file_name += str_suffix;
	file_name += ".xml";

	return !!PathFileExistsA((XmlCfg::GetCfgPath() + file_name).c_str());
}

const stSpecificCfg::stRoleInfo* stSpecificCfg::FindRoleInfoByName( const std::string& role_name ) const
{
	for ( const auto& role_info : roles_info_ )
	{
		if ( role_info.role_info_.role_name_ == role_name )
			return &role_info;
	}
	return nullptr;
}

stSpecificCfg::stRoleInfo* stSpecificCfg::FindRoleInfoByName( const std::string& role_name )
{
	for ( auto& role_info : roles_info_ )
	{
		if ( role_info.role_info_.role_name_ == role_name )
			return &role_info;
	}
	return nullptr;
}

void stSpecificCfg::SetRolesInfo( const CG_RoleInfo::RoleInfoContT& roles_info )
{
	if (roles_info.empty())
	{
		roles_info_.clear();
		return;
	}
	RoleInfoContT roles_info_tmp;
	for ( const auto& role_info : roles_info )
	{
		stRoleInfo role_info_tmp;
		auto role_ptr = FindRoleInfoByName(role_info.role_name_);
		if (role_ptr)
			role_info_tmp = *role_ptr;
		else
			role_info_tmp.role_info_ = role_info;
		roles_info_tmp.push_back(role_info_tmp);
	}
	std::swap(roles_info_tmp, roles_info_);
}

const stSpecificCfg::stRoleInfo* stSpecificCfg::FindFirstFatigueValidRole()
{
	if (roles_info_.empty())
	{
		return nullptr;
	}
	DateTimeLocal time_now;
	DateTimeLocal time_morning6(time_now.year(), time_now.month(), time_now.day(), 6);
	if ( time_now <= time_morning6 )
	{
		auto yesterday_moning6 = time_morning6 - Poco::Timespan(1, 0, 0, 0, 0);
		for ( auto& role : roles_info_ )
		{
			if ( role.fatigue_data_ > 0 )
				return &role;
			else
			{
				assert(0 == role.fatigue_data_);
				if ( role.the_time_ < yesterday_moning6 )
				{
					role.ResetFatigueData();
					return &role;
				}
			}
		}
	}
	else
	{
		for ( auto& role : roles_info_ )
		{
			if ( role.fatigue_data_ > 0 )
				return &role;
			else
			{
				assert(0 == role.fatigue_data_);
				if ( role.the_time_ < time_morning6 )
				{
					role.ResetFatigueData();
					return &role;
				}
			}
		}
	}
	return nullptr;
}

bool stSpecificCfg::CanCreateNewRole() const
{
	return roles_info_.size() < stSingletonCfg::GetMe().role_max_cnt_;
}

void stSpecificCfg::SetRoleFatigueData( const std::string& role_name, int fatigue_data )
{
	auto role = FindRoleInfoByName(role_name);
	if ( !role )
	{
		roles_info_.push_back(stRoleInfo(role_name, fatigue_data));
		return;
	}
	role->SetFatigueData(fatigue_data);
}

void stSpecificCfg::SetRoleInfo( const std::string& role_name, const std::string& job )
{
	auto role = FindRoleInfoByName(role_name);
	if ( !role )
	{
		roles_info_.push_back(stRoleInfo(role_name, job));
		return;
	}
	role->role_info_.role_job_ = job;
}

int stSpecificCfg::ServerIdx2Suffix( int server_id )
{
	return server_id ^ 0xe1d3;
}

void stSpecificCfg::ResetAllFatigueData()
{
	for ( auto& role : roles_info_ )
		role.ResetFatigueData();
}

int stSpecificCfg::RoleInfo2Index( const stRoleInfo* role_info ) const
{
	if ( !role_info )
	{
		assert(false);
		return -1;
	}
	for ( auto it = roles_info_.begin(); it != roles_info_.end(); ++it )
	{
		if ( it->role_info_.role_name_ == role_info->role_info_.role_name_ )
		{
			return std::distance(roles_info_.begin(), it);
		}
	}
	return -1;
}

stDefaultCfg& stDefaultCfg::GetMe()
{
	static stDefaultCfg me;
	return me;
}

stDefaultCfg::stDefaultCfg()
{
	script_name_ = "普通号交易.lua";
	//game_service_ = "山城兰特";
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

AcIniCfgMgr::stAcIniCfg::stAcIniCfg()
{
	service_name_ = _T("山城兰特");
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
	if ( 3 != vec_seg_tag.size() )
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

void AcIniCfgMgr::Add( const CString& ac, const CString& pwd, const CString& service_name )
{
	stAcIniCfg tmp;
	tmp.ac_ = ac;
	tmp.pwd_ = pwd;
	if ( !service_name.IsEmpty() )
		tmp.service_name_ = service_name;
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

stSpecificCfg::stRoleInfo::stRoleInfo( const std::string& role_name, int fatigule_data )
{
	assert(fatigule_data >= 0);
	assert(!role_name.empty());
	role_info_.role_name_ = role_name;
	fatigue_data_ = fatigule_data;
	/*if ( fatigue_data_ <= 0 )
		RefreshTime();*/
}

stSpecificCfg::stRoleInfo::stRoleInfo( const std::string& role_name, const std::string& job )
{
	assert(!role_name.empty());
	assert(!job.empty());
	role_info_.role_name_ = role_name;
	ResetFatigueData();
	role_info_.role_job_ = job;
}

void stSpecificCfg::stRoleInfo::RefreshTime()
{
	the_time_ = DateTimeLocal();
}

void stSpecificCfg::stRoleInfo::SetFatigueData( int fatigue_data )
{
	if ( fatigue_data <= 0 )
	{
		if ( fatigue_data_ > 0 )
			RefreshTime();
	}
	fatigue_data_ = fatigue_data;
	assert(fatigue_data >= 0);
}

void stSpecificCfg::stRoleInfo::ResetFatigueData()
{
	fatigue_data_ = 10000;
}

#pragma once
/*
@author zhp
@date 2014-1-17
@purpose config struct saved to xml
*/
#include "Common/XmlCfg.h"
#include <boost/serialization/version.hpp>
#include <boost/serialization/vector.hpp>
#include "Protocol/P_Defines.h"
#include <atlpath.h>
#include <Net/PocoMore.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeParser.h>
#include <Communicate/CG_Defines.h>
#include <Communicate/CGL_Defines.h>
#include "GLuaS.h"
#include "GlobalFuncs.h"
#include <Net/CmnLoginInfo.h>
//////////////////////////////////////////////////////////////////////////
//答题方式
struct stCrackCaptchaMode{
	enum enCrackCaptchaMode
	{
		kCCM_DaMaTu,				//答码兔
		kCCM_Local,					//本地答题
	};

	stCrackCaptchaMode() : remote_ip_(kSelfIpStr){
		ccm_ = kCCM_DaMaTu;
		remote_port_ = P_Port_ForGame + 3;
	}

	template<class Archive>
	void serialize( Archive & ar, const unsigned int version ){
		ar & BOOST_SERIALIZATION_NVP(account_)
			& BOOST_SERIALIZATION_NVP(password_)
			& BOOST_SERIALIZATION_NVP(ccm_)
			& BOOST_SERIALIZATION_NVP(remote_ip_)
			& BOOST_SERIALIZATION_NVP(remote_port_)
			;
	}

	std::string					account_;			//某答题方式的帐号
	std::string					password_;			//某答题方式的密码
	enCrackCaptchaMode			ccm_;				//答题方式
	std::string					remote_ip_;			//一般只是用于本地答码
	WORD						remote_port_;		//一般只是用于本地答码
};
//////////////////////////////////////////////////////////////////////////
//global/singleton xml
//顾名思义，全局唯一的配置
struct stSingletonCfg : CmnLoginInfoMgr<ConnectCntInfo>{
protected:
	stSingletonCfg( const std::string& str_cfg_full_path ) : stored_ip_(kSelfIpStr){
		max_login_ = 15;
		role_max_cnt_ = std::min(kCG_RoleCntLimit, 2);
		login_delay_ = 5 * 60;
		right_bt_hit_ = true;
		run_at_3_ = true;
		stop_login_at_ban_ac_ = true;
		is_use_xun_lei_ = false;
		is_use_you_hua_ = true;
		is_login_at_disc_ = true;
		open_village_path_ = false;
		ignore_pwd_err_ = false;

		XmlCfg::RestoreHelper<stSingletonCfg> restore_helper(this, 
			str_cfg_full_path);
	}

public:
	friend class boost::serialization::access;
	template<class Archive>
	void serialize( Archive & ar, const unsigned int version ){
		ar & BOOST_SERIALIZATION_NVP(game_full_file_path_)
//#ifdef _WG_TEST
#ifdef false
			& BOOST_SERIALIZATION_NVP(test_dll_full_file_path_)
#endif
			& BOOST_SERIALIZATION_NVP(cfg_of_accounts_)
			& BOOST_SERIALIZATION_NVP(authen_account_)
			& BOOST_SERIALIZATION_NVP(authen_password_)
			& BOOST_SERIALIZATION_NVP(crack_captcha_mode_)
			& BOOST_SERIALIZATION_NVP(max_login_)
			& BOOST_SERIALIZATION_NVP(stored_ip_)
			//& BOOST_SERIALIZATION_NVP(io_h_)
			& BOOST_SERIALIZATION_NVP(role_max_cnt_)
			& BOOST_SERIALIZATION_NVP(login_delay_)
			& BOOST_SERIALIZATION_NVP(right_bt_hit_)
			& BOOST_SERIALIZATION_NVP(run_at_3_)
			& BOOST_SERIALIZATION_NVP(stop_login_at_ban_ac_)
			& BOOST_SERIALIZATION_NVP(is_use_xun_lei_)
			& BOOST_SERIALIZATION_NVP(is_use_you_hua_)
			& BOOST_SERIALIZATION_NVP(is_login_at_disc_)
			& BOOST_SERIALIZATION_NVP(open_village_path_)
			& BOOST_SERIALIZATION_NVP(ignore_pwd_err_)
			& BOOST_SERIALIZATION_NVP(auto_dial_info_)
			;
		p_supper::serialize(ar, version);
	}
	//保存
	void Save();

	//type definition
public:
	struct AccountType : std::pair<CString, CString>{
		template<class Archive>
		void serialize( Archive & ar, const unsigned int version ){
			ar & BOOST_SERIALIZATION_NVP(first)
				& BOOST_SERIALIZATION_NVP(second)
				& BOOST_SERIALIZATION_NVP(service_name_)
				;
		}
		friend bool operator == ( const AccountType& lhs, const AccountType& rhs ){
			return lhs.first == rhs.first && lhs.second == rhs.second && lhs.service_name_ == rhs.service_name_;
		}

		CString					service_name_;
	};
	typedef std::vector<AccountType> AcountContT;
	/*struct IoHelper{
		IoHelper() : last_check_roles_(1999, 1, 1){}
		void serialize( boost::archive::xml_oarchive& ar, const unsigned int var ){
			ar << boost::serialization::make_nvp("last_check_roles_", 
				Poco::DateTimeFormatter::format(last_check_roles_, Poco::DateTimeFormat::SORTABLE_FORMAT));
		}
		void serialize( boost::archive::xml_iarchive& ar, const unsigned int var ){
			std::string str_pro_at_time;
			ar & boost::serialization::make_nvp("last_check_roles_", str_pro_at_time);
			int time_zone = 0;
			if ( !Poco::DateTimeParser::tryParse(Poco::DateTimeFormat::SORTABLE_FORMAT, str_pro_at_time, last_check_roles_, time_zone) )
			{
				assert(false);
				last_check_roles_ = DateTimeLocal(1999, 1, 1);
			}
		}

		DateTimeLocal			last_check_roles_;				//上一次验证角色的时间
	};*/
public:
	//唯一实例
	static stSingletonCfg& GetMe();
	struct stAutoDialInfo{
		stAutoDialInfo(){
			is_auto_dial_ = false;
			auto_dial_mode_ = kDM_AsAc;
			run_ac_cnt_ = 15;
			run_time_span_ = 15;
		}
		enum enDialMode{
			kDM_AsAc = 0,		//按刷号拨号
			kDM_AsTime,			//按时间拨号
		};
		template<class Archive>
		void serialize( Archive & ar, const unsigned int version ){
			ar & BOOST_SERIALIZATION_NVP(is_auto_dial_)
				& BOOST_SERIALIZATION_NVP(auto_dial_mode_)
				& BOOST_SERIALIZATION_NVP(run_ac_cnt_)
				& BOOST_SERIALIZATION_NVP(run_time_span_)
				;
		}
		
		bool				is_auto_dial_;
		enDialMode			auto_dial_mode_;
		int					run_ac_cnt_;
		int					run_time_span_;
	};

public:
	void DoCloseTheSession(const BindAcInfoT& the_clr_ac) override;

public:
	CString									game_full_file_path_;			//游戏路径
	std::string								test_dll_full_file_path_;		//测试DLL的全游戏
	AcountContT								cfg_of_accounts_;				//帐号的配置
	//for authenticate
	CString									authen_account_;				//wg account
	CString									authen_password_;				//wg password
	stCrackCaptchaMode						crack_captcha_mode_;			//答题模式
	WORD									max_login_;						//登陆最大上线
	std::string								stored_ip_;						//仓库号所在IP
	//IoHelper								io_h_;
	int										role_max_cnt_;
	int										login_delay_;					//登陆延迟
	bool									right_bt_hit_;					//右键无限攻击
	bool									run_at_3_;						//3点自动重启
	bool									stop_login_at_ban_ac_;			//封号时停止登陆
	bool									is_use_xun_lei_;				//启用迅雷加速
	bool									is_use_you_hua_;				//启用优化
	bool									is_login_at_disc_;				//掉线立即重登
	bool									open_village_path_;				//开启城镇寻路
	bool									ignore_pwd_err_;				//忽略密码错误
	stAutoDialInfo							auto_dial_info_;
};
BOOST_CLASS_VERSION(stSingletonCfg, 1)

//specific config
//具体配置，即区别于default的配置
struct stSpecificCfg{
	//type definitions
public:
	//帐号被禁止的原因
	enum enProhibitReason{
		kPR_Valid = 0,			//没有被禁止，即有效
		kPR_RolesUseOut,		//角色刷完
		kPR_BanAc,				//帐号被封
		kPR_Restricted,			//被限制
		kPR_AcFreeze,			//帐号被冻结
		kPR_AcPwdErr,			//密码错误
	};
	//帐号禁止状态
	struct stAcProState{
		stAcProState() : pro_at_time_(2000, 1, 1){
			pro_reason_ = kPR_Valid;
		}
		void serialize( boost::archive::xml_oarchive& ar, const unsigned int var ){
			ar << boost::serialization::make_nvp("pro_reason_", pro_reason_)
				<< boost::serialization::make_nvp("pro_at_time_", 
				Poco::DateTimeFormatter::format(pro_at_time_, Poco::DateTimeFormat::SORTABLE_FORMAT));
		}
		void serialize( boost::archive::xml_iarchive& ar, const unsigned int var ){
			std::string str_pro_at_time;
			ar & boost::serialization::make_nvp("pro_reason_", pro_reason_)
				& boost::serialization::make_nvp("pro_at_time_", str_pro_at_time);
			int time_zone = 0;
			if ( !Poco::DateTimeParser::tryParse(Poco::DateTimeFormat::SORTABLE_FORMAT, str_pro_at_time, pro_at_time_, time_zone) )
			{
				assert(false);
				pro_at_time_ = DateTimeLocal(2000, 1, 1);
			}
		}

		enProhibitReason				pro_reason_;			//禁止原因
		DateTimeLocal					pro_at_time_;			//禁止时间
	};
	struct stRoleInfo{
		stRoleInfo() : the_time_(2000, 1, 1){
			ResetFatigueData();
		}
		stRoleInfo( const std::string& role_name, unsigned __int64 fatigule_data );
		stRoleInfo( const std::string& role_name, const std::string& job );
		void serialize( boost::archive::xml_oarchive& ar, const unsigned int var ){
			ar << boost::serialization::make_nvp("role_info_.role_name_", role_info_.role_name_)
				<< boost::serialization::make_nvp("role_info_.role_job_", role_info_.role_job_)
				<< boost::serialization::make_nvp("fatigue_data_", fatigue_data_)
				<< boost::serialization::make_nvp("time", 
				Poco::DateTimeFormatter::format(the_time_, Poco::DateTimeFormat::SORTABLE_FORMAT));
		}
		void serialize( boost::archive::xml_iarchive& ar, const unsigned int var ){
			std::string str_pro_at_time;
			ar & BOOST_SERIALIZATION_NVP(role_info_.role_name_)
				& BOOST_SERIALIZATION_NVP(role_info_.role_job_)
				& BOOST_SERIALIZATION_NVP(fatigue_data_)
				& boost::serialization::make_nvp("time", str_pro_at_time);
			int time_zone = 0;
			if ( !Poco::DateTimeParser::tryParse(Poco::DateTimeFormat::SORTABLE_FORMAT, str_pro_at_time, the_time_, time_zone) )
			{
				assert(false);
				the_time_ = DateTimeLocal(2000, 1, 1);
			}
		}
		void RefreshTime();
		void SetFatigueData( unsigned __int64 fatigue_data );
		void ResetFatigueData();

		CG_RoleInfo			role_info_;
		unsigned __int64	fatigue_data_;
		DateTimeLocal		the_time_;
	};
	typedef std::vector<stRoleInfo> RoleInfoContT;

public:
	template<class Archive>
	void serialize( Archive & ar, const unsigned int version ){
		ar & BOOST_SERIALIZATION_NVP(script_name_)
			//& BOOST_SERIALIZATION_NVP(game_service_)
			& BOOST_SERIALIZATION_NVP(auto_run_script_)
			& BOOST_SERIALIZATION_NVP(game_ac_type_)
			& BOOST_SERIALIZATION_NVP(ac_pro_state_)
			& BOOST_SERIALIZATION_NVP(roles_info_)
			& BOOST_SERIALIZATION_NVP(prohibit_time_)
			& BOOST_SERIALIZATION_NVP(role_name_)
			& BOOST_SERIALIZATION_NVP(role_idx_)
			& BOOST_SERIALIZATION_NVP(role_lvl_)
			& BOOST_SERIALIZATION_NVP(shou_lie_quan_)
			& BOOST_SERIALIZATION_NVP(single_exp_)
			& BOOST_SERIALIZATION_NVP(double_exp_)
			& BOOST_SERIALIZATION_NVP(cur_gold_)
			& BOOST_SERIALIZATION_NVP(gain_gold_);
	}
	//保存
	void Save( const char* file_name );
	//恢复
	void Restore( const char* file_name );
	//根据帐号保存
	void SaveAsAccount( const std::string& ac, int server_id );
	//根据帐号恢复 
	void RestoreAsAccount( const std::string& ac, int server_id );

	void SetRolesInfo( const CG_RoleInfo::RoleInfoContT& roles_info );
	void SetRoleFatigueData( const std::string& role_name, unsigned __int64 fatigue_data );
	void SetRoleInfo( const std::string& role_name, const std::string& job );
	const stRoleInfo* FindRoleInfoByName( const std::string& role_name ) const;
	stRoleInfo* FindRoleInfoByName( const std::string& role_name );
	const stRoleInfo* FindFirstFatigueValidRole();
	int RoleInfo2Index( const stRoleInfo* role_info ) const;
	void ResetAllFatigueData();
	bool CanCreateNewRole() const;
	//是否有指定的配置
	static bool HasSpecificCfg( const std::string& ac, int server_id );

private:
	static int ServerIdx2Suffix( int server_id );

public:
	enum{
		Version = 1
	};

public:
	std::string				script_name_;			//默认脚本名
	//std::string				game_service_;		//游戏服务器
	bool					auto_run_script_;		//登陆完了是否自动运行脚本
	enGameAcType			game_ac_type_;			//游戏号的类型
	stAcProState			ac_pro_state_;			//游戏帐号的禁止状态
	RoleInfoContT			roles_info_;			//角色信息
	CString					prohibit_time_;			//禁止时间
	CString					role_name_;				//角色名
	CString					role_idx_;				//角色索引
	CString					role_lvl_;				//角色等级
	CString					shou_lie_quan_;			//狩猎券
	CString					single_exp_;			//单倍经验
	CString					double_exp_;			//双倍经验
	CString					cur_gold_;				//当前金币
	CString					gain_gold_;				//获得的金币
};
BOOST_CLASS_VERSION(stSpecificCfg, stSpecificCfg::Version)

//default.xml
struct stDefaultCfg : public stSpecificCfg
{
private:
	stDefaultCfg();
	using stSpecificCfg::Restore;

public:
	//唯一实例
	static stDefaultCfg& GetMe();
	//保存
	void Save();
};
BOOST_CLASS_VERSION(stDefaultCfg, stDefaultCfg::Version)

class AcIniCfgMgr{
public:
	//帐号.ini文件
	struct stAcIniCfg{
		stAcIniCfg();
		bool Read( std::size_t setion_idx, LPCTSTR full_file_name );
		bool Write( std::size_t setion_idx, LPCTSTR full_file_name ) const;
		bool ReadLine( const std::string& str_line );
		bool WriteLine( std::string& str_line ) const;

		CString				ac_;
		CString				pwd_;
		CString				service_name_;
	};

	typedef std::vector<stAcIniCfg> ContT;

public:
	const ContT& GetCont() const;
	virtual bool Load( const CPath& full_file_name );
	virtual bool Save( const CPath& full_file_name );
	void Add( const CString& ac, const CString& pwd, const CString& service_name );

protected:
	ContT			cfg_;
};

//新的格式
class AcIniCfgMgr_New : public AcIniCfgMgr{
public:
	virtual bool Load( const CPath& full_file_name );
	virtual bool Save( const CPath& full_file_name );
};
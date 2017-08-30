#pragma once
/*
@author zhp
@date 2014/4/22 14:37
@purpose for database data
*/
#include <string>
#include "PocoMore.h"
#include <Protocol/P_Defines.h>
#include <vector>

//////////////////////////////////////////////////////////////////////////
//tbl_Account
struct stData_Account{
	typedef std::vector<stData_Account> AcContT;
	stData_Account(){
		ac_type_ = 0;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & user_name_
			& user_pwd_
			& machine_code_
			& last_login_time_
			& last_pay_time_
			& last_login_ip_
			& reg_time_
			& end_day_time_
			& qq_
			& reg_ip_
			& cookie_
			& cookie_end_time_
			& ac_type_;
	}

	std::string				user_name_;					//用户名
	std::string				user_pwd_;					//用户密码
	std::string				machine_code_;				//机器码
	DateTimeLocal			last_login_time_;			//最后登陆时间
	DateTimeLocal			last_pay_time_;				//最后支付时间
	std::string				last_login_ip_;				//最后登陆时间
	DateTimeLocal			reg_time_;					//注册时间
	DateTimeLocal			end_day_time_;				//用户截止时间
	std::string				qq_;						//qq号码
	std::string				reg_ip_;					//注册时的IP
	std::string				cookie_;					//cookie
	DateTimeLocal			cookie_end_time_;			//cookie截止时间
	Poco::UInt32			ac_type_;					//帐号类型
};

//tbl_Card
struct stData_Card{
	typedef std::vector<stData_Card> CardContT;
	stData_Card(){
		card_minutes_ = 0;
		is_used_ = false;
		ac_type_ = 0;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & card_id_
			& card_pwd_
			& use_time_
			& add_time_
			& add_user_
			& is_used_
			& user_name_
			& card_minutes_
			& grant_
			& ac_type_;
	}

	std::string				card_id_;					//卡号
	std::string				card_pwd_;					//卡密码
	DateTimeLocal			use_time_;					//使用时间
	DateTimeLocal			add_time_;					//添加的时间
	std::string				add_user_;					//添加者
	bool					is_used_;					//是否被使用
	std::string				user_name_;					//使用者
	Poco::UInt32			card_minutes_;				//能增加的时间,单位->分钟
	Poco::UInt64			grant_;						//卡的权限
	Poco::UInt32			ac_type_;					//帐号类型
};

struct stData_AcState{
	typedef std::vector<stData_AcState> ContT;
	stData_AcState(){
		state_ = enAcState::kAS_Normal;
		last_state_ = state_;
		period_ = 0;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & user_name_
			& state_
			& last_state_
			& begin_time_
			& end_time_
			& period_;
	}

	std::string				user_name_;
	enAcState				state_;				//帐号状态，enAcState
	enAcState				last_state_;		//上次状态
	DateTimeLocal			begin_time_;		//开始状态的时间
	DateTimeLocal			end_time_;			//结束状态的时间
	Poco::UInt32			period_;			//状态持续时间，单位：分钟
};

//tbl_MsgBoard
struct stData_MsgBoard{
	stData_MsgBoard(){
		id_ = 0;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & id_
			& msg_
			& user_name_
			& add_time_;
	}

	Poco::UInt32			id_;						//留言版消息号
	std::string				msg_;						//留言信息
	std::string				user_name_;					//留言者
	DateTimeLocal			add_time_;					//留言时间
};

//tbl_UserGrant
struct stData_UserGrant{
	stData_UserGrant(){
		user_grant_ = kUG_No;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & user_name_
			& user_grant_;
	}

	//set
	void SetUserGrant( enUserGrant user_grant );
	enUserGrant GetUserGrant() const;
	bool operator < ( const stData_UserGrant& rhs );

	std::string				user_name_;
	enUserGrant				user_grant_;				//用户权限,enUserGrant
};

//tbl_RegKey
struct stData_RegKey{
	stData_RegKey(){
		is_used_ = false;
		ac_type_ = 0;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & reg_key_id_
			& add_time_
			& add_user_
			& is_used_
			& user_name_
			& use_time_
			& ac_type_;
	}

	std::string				reg_key_id_;
	DateTimeLocal			add_time_;
	std::string				add_user_;
	bool					is_used_;
	std::string				user_name_;
	DateTimeLocal			use_time_;
	Poco::UInt32			ac_type_;					//帐号类型
};

//tbl_AddrData
struct stData_AddrData{
	stData_AddrData(){
		id_ = 0;
		addr_data_ = 0;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & id_
			& addr_data_;
	}

	Poco::UInt32			id_;						//关键地址数据id
	Poco::UInt32			addr_data_;					//关键地址
};

//tbl_StrData
struct stData_StrData{
	stData_StrData(){
		id_ = 0;
	}
	template<class Archive>
	void serialize(Archive & ar){
		ar & id_
			& str_data_;
	}

	Poco::UInt32			id_;						//关键地址数据id
	std::string				str_data_;					//关键地址
};

//////////////////////////////////////////////////////////////////////////
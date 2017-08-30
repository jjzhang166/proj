#include "stdafx.h"
#include "DataHandlerS.h"
#include <Common/UsefulFunc.h>
#include <Protocol/P_PriDefines.h>
#include "CS_CfgInfo.h"

DataH_Account::DataH_Account( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "tbl_Account")
{

}

bool DataH_Account::AddUser(const std::string& user_name, const std::string& user_pwd, const std::string& str_qq, 
	const std::string& str_reg_ip, pt_dword ac_type, std::string& out_info)
{
	if (FindUser(user_name))
	{
		out_info = "用户名已存在";
		return false;
	}
	const auto kUserNameSize = user_name.size();
	if ( kUserNameSize < kUserNameMinLen || kUserNameSize > kUserNameMaxLen )
	{
		out_info = "用户名长度不对";
		return false;
	}
	const auto kUserPwdSize = user_pwd.size();
	if ( kUserPwdSize < kUserPwdMinLen || kUserPwdSize > kUserPwdMaxLen )
	{
		out_info = "密码长度不对";
		return false;
	}
	const auto kQQSize = str_qq.size();
	if ( kQQSize > kQQMaxLen || kQQSize < kQQMinLen )
	{
		out_info = "qq长度不对";
		return false;
	}
	DataType data_tmp;
	data_tmp.user_name_ = user_name;
	data_tmp.user_pwd_ = user_pwd;
	data_tmp.qq_ = str_qq;
	data_tmp.reg_ip_ = str_reg_ip;
	data_tmp.ac_type_ = ac_type;

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	InsertInto(), use(data_tmp), now;
	datas_.push_back(data_tmp);
	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

stData_Account* DataH_Account::FindUser( const std::string& user_name )
{
	auto it = std::find_if(datas_.begin(), datas_.end(), [&user_name]( const DataType& data ){
		return data.user_name_ == user_name;
	});
	if ( it != datas_.end() )
		return &*it;
	return nullptr;
}

const stData_Account* DataH_Account::FindUser( const std::string& user_name ) const
{
	auto it = std::find_if(datas_.begin(), datas_.end(), [&user_name]( const DataType& data ){
		return data.user_name_ == user_name;
	});
	if ( it != datas_.end() )
		return &*it;
	return nullptr;
}

bool DataH_Account::UpdateLoginInfo(const std::string& user_name, const std::string& login_ip, std::string& cookie, DateTimeLocal& cookie_end_time)
{
	auto data_ac = FindUser(user_name);
	if ( !data_ac )
		return false;

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	auto ip = login_ip;
	DateTimeLocal time_now;
	Update(DataColumn("LastLoginTime").Add("LastLoginIP").Add("Cookie").Add("CookieEndTime"),
		DataWhere("UserName", DataCondition::eq, data_ac->user_name_)),
		use(time_now), use(ip), use(cookie), use(cookie_end_time),
		now;

	data_ac->last_login_time_ = time_now;
	data_ac->last_login_ip_ = login_ip;
	data_ac->cookie_ = cookie;
	data_ac->cookie_end_time_ = cookie_end_time;
	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

bool DataH_Account::Pay( const std::string& user_name, Poco::UInt32 minutes_addition, std::string& out_info )
{
	auto data_ac = FindUser(user_name);
	if ( !data_ac )
	{
		out_info = "支付的用户不存在";
		return false;
	}
	DateTimeLocal end_day_time;
	DateTimeLocal last_pay_time(end_day_time);

	if ( end_day_time < data_ac->end_day_time_ )
		end_day_time = data_ac->end_day_time_;
	end_day_time += Poco::Timespan(0, 0, minutes_addition, 0, 0);

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	
	Update(DataColumn("EndDayTime").Add("LastPayTime"), DataWhere("UserName", DataCondition::eq, data_ac->user_name_)), 
		use(end_day_time), use(last_pay_time),
		now;

	data_ac->end_day_time_ = end_day_time;
	data_ac->last_pay_time_ = last_pay_time;
	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

bool DataH_Account::UpdateMachineCode( const std::string& user_name, const std::string& new_machine_code )
{
	if ( kUuidStrLen !=  new_machine_code.size() )
		return false;
	auto user_ac = FindUser(user_name);
	if ( !user_ac )
		return false;

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	Update(DataColumn("MachineCode"), DataWhere("UserName", DataCondition::eq, user_name)),
		use(const_cast<std::string&>(new_machine_code)), now;
	user_ac->machine_code_ = new_machine_code;
	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

bool DataH_Account::RemoveMachineCode( const std::string& user_name, int minus_minutes )
{
	auto user_ac = FindUser(user_name);
	if ( !user_ac )
		return false;
	if (user_ac->machine_code_.empty())
		return true;
	auto end_day_time = user_ac->end_day_time_;
	end_day_time -= Poco::Timespan(0, 0, minus_minutes, 0, 0);
	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	static std::string str_empty;
	DateTimeLocal time_now;
	Update(DataColumn("MachineCode").Add("EndDayTime").Add("Cookie").Add("CookieEndTime"), DataWhere("UserName", DataCondition::eq, user_name)),
		use(str_empty), use(end_day_time), use(str_empty), use(time_now), now;
	user_ac->machine_code_.clear();
	user_ac->end_day_time_ = end_day_time;
	user_ac->cookie_.clear();
	user_ac->cookie_end_time_ = time_now;
	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

bool DataH_Account::GetEndDateTime( const std::string& user_name, DateTimeLocal& end_date_time ) const
{
#ifdef FREE_LOGIN_
	end_date_time = DateTimeLocal(2015, 1, 15);
	return true;
#endif
	auto user_ac = FindUser(user_name);
	if ( !user_ac )
		return false;
	end_date_time = user_ac->end_day_time_;
	return true;
}

bool DataH_Account::UpdateEndDayTime( const std::string& user_name, const DateTimeLocal& new_end_day_time, std::string& out_info )
{
	auto data_ac = FindUser(user_name);
	if ( !data_ac )
	{
		out_info = "用户名不存在";
		return false;
	}
	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	auto the_time = new_end_day_time;

	Update(DataColumn("EndDayTime"), DataWhere("UserName", DataCondition::eq, data_ac->user_name_)), 
		use(the_time),
		now;

	data_ac->end_day_time_ = new_end_day_time;
	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

DataH_Card::DataH_Card( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "tbl_Card")
{

}

bool DataH_Card::AddCard( const std::string& add_user, const std::string& card_id, const std::string& card_pwd, 
	Poco::UInt32 card_minutes, Poco::UInt64 grant, pt_dword ac_type, std::string& info)
{
	if (card_id.empty())
	{
		info = "card id is empty";
		return false;
	}
	if (card_pwd.empty())
	{
		info = "card pwd is empty";
		return false;
	}

	auto card = FindCard(card_id);
	if (card)
	{
		info = "生成的卡号是重复的";
		return false;
	}

	DataType card_tmp;
	card_tmp.card_id_ = card_id;
	card_tmp.card_pwd_ = card_pwd;
	card_tmp.card_minutes_ = card_minutes;
	card_tmp.add_user_ = add_user;
	card_tmp.grant_ = grant;
	card_tmp.ac_type_ = ac_type;

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	InsertInto(), use(card_tmp), now;

	datas_.push_back(card_tmp);
	DATA_EXCEPTION_END(exec_helper);
	return true;
}

stData_Card* DataH_Card::FindCard( const std::string& card_id )
{
	auto it = std::find_if(datas_.begin(), datas_.end(), [&card_id]( const DataType& data ){
		return data.card_id_ == card_id;
	});
	if ( it != datas_.end() )
		return &*it;
	return nullptr;
}

bool DataH_Card::Use( const std::string& user_name, const std::string& card_id, 
					 const std::string& card_pwd, Poco::UInt32* card_minutes, std::string& out_info )
{
	auto card = FindCard(card_id);
	if ( !card )
	{
		out_info = "卡号不存在";
		return false;
	}
	if (card->is_used_)
	{
		out_info = "该卡已使用过";
		return false;
	}
	if ( card->card_pwd_ != card_pwd )
	{
		out_info = "卡密错误";
		return false;
	}

	DateTimeLocal use_time;
	bool is_used = true;
	card->user_name_ = user_name;

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	
	Update(DataColumn("UseTime").Add("IsUsed").Add("UserName"), 
		DataWhere("CardId", DataCondition::eq, card->card_id_)),
		use(use_time), use(is_used), use(card->user_name_),
		now;

	card->use_time_ = use_time;
	card->is_used_ = is_used;
	if (card_minutes)
		*card_minutes = card->card_minutes_;

	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

void DataH_Card::DoItem( const std::function<void(const stData_Card&)>& func ) const
{
	for ( const auto& card : datas_ )
	{
		func(card);
	}
}

DataH_MsgBoard::DataH_MsgBoard( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "tbl_MsgBoard")
{

}

bool DataH_MsgBoard::AddLeaveMsg( const std::string& msg, const std::string& user_name, std::string& out_info )
{
	const auto kMsgSize = msg.size();
	if ( kMsgSize > kMsgBoardMsgMaxLen || kMsgSize < 1 )
	{
		out_info = "留言长度判定失败";
		return false;
	}
	auto msg_cnt = GetUserMsgCntToday(user_name);
	if ( msg_cnt >= kMsgBoardMaxCnt_EveryDay )
	{
		out_info = "留言失败：阁下今天留言过多";
		return false;
	}
	msg_cnt = GetUserMsgTotalCnt(user_name);
	if ( msg_cnt >= kMsgBoardMaxTotalCnt_OneAc )
	{
		out_info = "留言失败：阁下帐号留言次数已超过最大限制";
		return false;
	}
	auto free_id = GetFreeId();
	assert(free_id > 0);

	DataType data_tmp;
	data_tmp.id_ = free_id;
	data_tmp.msg_ = msg;
	data_tmp.user_name_ = user_name;

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	InsertInto(), use(data_tmp), now;
	datas_.push_back(data_tmp);
	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

Poco::UInt32 DataH_MsgBoard::GetFreeId() const
{
	return datas_.size() + 1;
}

std::size_t DataH_MsgBoard::GetUserMsgTotalCnt( const std::string& user_name ) const
{
	std::size_t cnt = 0;
	for ( const auto& one_msg : datas_ )
	{
		if ( user_name == one_msg.user_name_ )
			++cnt;
	}
	return cnt;
}

std::size_t DataH_MsgBoard::GetUserMsgCntToday( const std::string& user_name ) const
{
	std::size_t cnt = 0;
	DateTimeLocal time_now;
	for ( const auto& one_msg : datas_ )
	{
		if (time_now.IsSameDay(one_msg.add_time_))
			++cnt;
	}
	return cnt;
}

DataH_AddrData::DataH_AddrData( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "tbl_AddrData")
{

}

bool DataH_AddrData::GetEncrytAddrData( ISecretMaker* secret_maker, EncryptAddrDataT& encrypt_data )
{
	if ( !secret_maker )
		return false;
	char data_tmp[50];
#ifdef FREE_LOGIN_
	DWORD addr_datas[] = {
		0x0157F1DC, 0x00F42160, 0x00F42200
	};
	int i = 0;
	for ( int i = 0; i != _countof(addr_datas); ++i )
	{
		auto str_data = std::to_string(addr_datas[i]);
		secret_maker->Encrypt(str_data.c_str(), str_data.size(), data_tmp);
		encrypt_data.push_back(EncryptAddrDataT::value_type(data_tmp, str_data.size()));
	}
	return true;
#endif
	for ( const auto& addr_data : datas_ )
	{
		auto str_data = std::to_string(addr_data.addr_data_);
		secret_maker->Encrypt(str_data.c_str(), str_data.size(), data_tmp);
		encrypt_data.push_back(EncryptAddrDataT::value_type(data_tmp, str_data.size()));
	}
	return true;
}

void DataH_AddrData::OrderById()
{
	std::sort(datas_.begin(), datas_.end(), []( const DataType& lhs, const DataType& rhs ){
		return lhs.id_ < rhs.id_;
	});
}

bool DataH_AddrData::SetAddrData( Poco::UInt32 id, Poco::UInt32 addr_data )
{
	using namespace Poco::Data::Keywords;
	auto the_data = FindById(id);
	if (the_data)
	{
		if ( the_data->addr_data_ == addr_data )
			return true;

		DataExecH exec_helper;
		DATA_EXCEPTION_BEGIN;
		Update(DataColumn("AddrData"), 
			DataWhere("id", DataCondition::eq, id)),
			use(addr_data), now;
		the_data->addr_data_ = addr_data;
		DATA_EXCEPTION_END(exec_helper);
		return exec_helper;
	}
	else
	{
		DataType new_data;
		new_data.id_ = id;
		new_data.addr_data_ = addr_data;

		DataExecH exec_helper;
		DATA_EXCEPTION_BEGIN;
		using namespace Poco::Data::Keywords;
		InsertInto(), use(new_data), now;
		datas_.push_back(new_data);
		OrderById();
		DATA_EXCEPTION_END(exec_helper);
		return exec_helper;
	}
}

DataH_AddrData::DataType* DataH_AddrData::FindById( Poco::UInt32 id )
{
	auto it = std::find_if(datas_.begin(), datas_.end(), [id]( const DataType& the_data ){
		return id == the_data.id_;
	});
	if ( it == datas_.end() )
		return nullptr;
	return &*it;
}

DbSingleton::DbSingleton( const std::string& server_ip, WORD ip_port, const std::string& db_name )
	: DatabaseMgrBase(server_ip, ip_port, db_name, kSqlSvrAc, kSqlSvrPwd),
	tbl_account_(*this),
	tbl_card_(*this),
	tbl_msg_board_(*this), tbl_user_grant_(*this), tbl_reg_key_(*this),
	tbl_addr_data_(*this), tbl_str_data_(*this), tbl_ac_state_(*this)
{
}

const DataH_Account& DbSingleton::GetTbl_Account() const
{
	return tbl_account_;
}

const DataH_Card& DbSingleton::GetTbl_Card() const
{
	return tbl_card_;
}

bool DbSingleton::SelectAllTblData()
{
	tbl_account_.ClearData();
	tbl_card_.ClearData();
	tbl_msg_board_.ClearData();
	tbl_user_grant_.ClearData();
	tbl_reg_key_.ClearData();
	tbl_addr_data_.ClearData();
	tbl_str_data_.ClearData();
	tbl_ac_state_.ClearData();

	if (tbl_account_.SelectData()
		&& tbl_card_.SelectData()
		&& tbl_msg_board_.SelectData()
		&& tbl_user_grant_.SelectData()
		&& tbl_reg_key_.SelectData()
		&& tbl_addr_data_.SelectData()
		&& tbl_str_data_.SelectData()
		&& tbl_ac_state_.SelectData())
	{
		tbl_addr_data_.OrderById();
		tbl_str_data_.OrderById();
		return true;
	}
	return false;
}

bool DbSingleton::Login( const std::string& user_name, const std::string& user_pwd, const std::string& machine_code, 
	pt_csz str_mix, const std::string& str_cookie, std::string* new_cookie, const std::string* login_ip, std::string* out_info
	, pt_dword& ac_type)
{
	if (login_ip && login_ip->empty())
	{
		assert(false);
		return false;
	}
	std::string str_mix_value = user_name;
	str_mix_value += user_pwd;
	str_mix_value += machine_code;
	str_mix_value = GenUuidStr(str_mix_value);
	if (str_mix_value != str_mix)
	{
		if (out_info)
			*out_info = "未知严重错误9168";
		return false;
	}
	auto data_ac = tbl_account_.FindUser(user_name);
	if ( !data_ac )
	{
		if (out_info)
			*out_info = "用户名不存在";
		return false;
	}
	if ( data_ac->user_pwd_ != user_pwd )
	{
		if (out_info)
			*out_info = "密码错误";
		return false;
	}
	if (!tbl_ac_state_.CanLogin(user_name, out_info))
		return false;

	//使用时间到期
	DateTimeLocal time_now;
#ifndef FREE_LOGIN_	
	if ( time_now >= data_ac->end_day_time_ )
	{
		if (out_info)
			*out_info = "没有使用时间了";
		return false;
	}
#endif
	
	if (out_info)
	{
		auto login_time_span = time_now - data_ac->last_login_time_;
		const int kPinFan = 90;
		if (login_time_span.totalSeconds() < kPinFan)
		{
			if (!tbl_user_grant_.FindUserGrant(user_name))
			{
				*out_info = "登陆太频繁,等待间隔：";
				*out_info += std::to_string(kPinFan);
				*out_info += "秒";
				return false;
			}
		}

		if (!data_ac->last_login_ip_.empty() && login_ip && *login_ip != data_ac->last_login_ip_)
		{
			const int kFeiFa = 30 + kPinFan;
			if (login_time_span.totalSeconds() < kFeiFa)
			{
				*out_info = "非法操作，禁止";
				*out_info += std::to_string(kFeiFa);
				*out_info += "秒";
				return false;
			}
		}
	}

	if (data_ac->machine_code_.empty())
	{
		if (!tbl_account_.UpdateMachineCode(user_name, machine_code))
			return false;
	}
	else
	{
		if (data_ac->machine_code_ != machine_code)
		{
			if (out_info)
				*out_info = "该用户已绑定机器，不能在其他机器上登陆";
			return false;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//cookie
	if (!data_ac->cookie_.empty() && time_now < data_ac->cookie_end_time_ && data_ac->cookie_ != str_cookie)
	{
		if (out_info)
			*out_info = "未知严重错误=1";
		return false;
	}
	if (!out_info)
		return true;
	if (!login_ip || !new_cookie)
	{
		assert(false);
		*out_info = "超严重错误165";
		return false;
	}
	//cookie有效期为24个小时
	assert(new_cookie->empty());
	*new_cookie = RandomUniqueStrEx(kCookieStrLen);
	//////////////////////////////////////////////////////////////////////////
	DateTimeLocal cookie_end_time = time_now + cookie_time_span;
	if (!tbl_account_.UpdateLoginInfo(user_name, *login_ip, *new_cookie, cookie_end_time))
	{
		*out_info = "登陆信息之未知严重错误";
		return false;
	}

	*out_info = Poco::DateTimeFormatter::format(data_ac->end_day_time_, Poco::DateTimeFormat::SORTABLE_FORMAT);
	ac_type = data_ac->ac_type_;
	return true;
}

bool DbSingleton::RegUser( const std::string& user_name, const std::string& user_pwd, const std::string& str_qq,
						  const std::string& str_reg_ip, const std::string& reg_key_id, std::string& out_info )
{
	pt_dword ac_type = 0;
	if (stCS_CfgInfo::GetMe().use_reg_key_)
	{
		if (!tbl_reg_key_.TryUse(reg_key_id, ac_type, out_info))
			return false;
	}	

	if (!tbl_account_.AddUser(user_name, user_pwd, str_qq, str_reg_ip, ac_type, out_info))
	{
		if (out_info.empty())
		{
			out_info = "未知添加用户严重错误";
		}
		return false;
	}
	if (stCS_CfgInfo::GetMe().use_reg_key_)
	{
		if ( !tbl_reg_key_.Use(user_name, reg_key_id, out_info) )
		{
			if (out_info.empty())
			{
				out_info = "使用注册码之未知错误";
			}
			return false;
		}
	}	
	out_info = "注册成功";
	return true;
}

bool DbSingleton::UserPay( const std::string& user_name, const std::string& card_id, const std::string& card_pwd, std::string& out_info )
{
	auto the_ac = tbl_account_.FindUser(user_name);
	if (!the_ac)
	{
		out_info = "用户不存在";
		return false;
	}
	auto card = tbl_card_.FindCard(card_id);
	if (!card)
	{
		out_info = "无此卡号";
		return false;
	}
	if (the_ac->ac_type_ != card->ac_type_)
	{
		out_info = "该帐号不能被此卡密充值:";
		out_info += std::to_string(the_ac->ac_type_);
		out_info += "-";
		out_info += std::to_string(card->ac_type_);
		return false;
	}
	Poco::UInt32 card_minutes = 0;
	if ( !tbl_card_.Use(user_name, card_id, card_pwd, &card_minutes, out_info) )
	{
		if (out_info.empty())
		{
			out_info = "使用卡未知严重错误";
		}
		return false;
	}
	if ( !tbl_account_.Pay(user_name, card_minutes, out_info) )
	{
		if (out_info.empty())
		{
			out_info = "支付未知错误，请联系代理";
		}
		return false;
	}
	if ( !tbl_user_grant_.AddGrant(user_name, card->grant_) )
	{
		out_info = "添加权限失败";
		return false;
	}
	out_info = "支付成功";
	return true;
}

bool DbSingleton::Grant_OpenCard(const std::string& add_user, Poco::UInt16 card_cnt, Poco::UInt32 card_minutes, pt_dword ac_type,
						   DataH_Card::CardsInfoT* cards_info, Poco::UInt64 grant, std::string& out_info)
{
	/*auto access_levl = tbl_grant_.Lvl_AccessTbl(add_user, tbl_card_.GetTblName());
	if ( !(access_levl & kTAL_Insert) )*/
	if ( !tbl_user_grant_.HasTheGrant(add_user, kUG_OpenCard) )
	{
		out_info = "没有开卡权限";
		return false;
	}
	if ( card_minutes > 2 * 365 * 24 * 60 )
	{
		out_info = "开卡时间不大得大于2年";
		return false;
	}

	std::string card_id, card_pwd;
	for ( Poco::UInt16 i = 0;i != card_cnt; ++i )
	{
		GenCardInfo(card_id, card_pwd);
		if ( !tbl_card_.AddCard(add_user, card_id, card_pwd, card_minutes, grant, ac_type, out_info) )
		{
			if (out_info.empty())
				out_info = "开卡失败";
			return false;
		}
		if (cards_info)
			cards_info->push_back(std::make_pair(card_id, card_pwd));
	}
	out_info = "开卡成功";
	return true;
}

bool DbSingleton::RemoveBind( const std::string& user_name, const std::string& user_pwd, const std::string& machine_code,
							  int minus_minutes, std::string& out_info )
{
	auto data_ac = tbl_account_.FindUser(user_name);
	if ( !data_ac )
	{
		out_info = "用户不存在";
		return false;
	}

	if ( data_ac->user_pwd_ != user_pwd )
	{
		out_info = "密码不正确";
		return false;
	}
	if (tbl_user_grant_.FindUserGrant(user_name))
	{
		out_info = "该用户是特殊用户，你无权解绑";
		return false;
	}
	if (data_ac->machine_code_.empty())
	{
		out_info = "已解绑，勿需再解";
		return false;
	}
	/*if ( data_ac->machine_code_ != machine_code )
	{
		out_info = "机器码不正确";
		return false;
	}*/
#ifndef FREE_LOGIN_
	//使用时间到期
	DateTimeLocal time_now;
	if ( time_now >= data_ac->end_day_time_ )
	{
		out_info = "没有使用时间了，不能解绑";
		return false;
	}
#endif

	if ( !tbl_account_.RemoveMachineCode(user_name, minus_minutes) )
	{
		out_info = "未知严重错误";
		return false;
	}
	const auto& cfg = stCS_CfgInfo::GetMe();
	if ( 0 == cfg.radio_time_unit_ )
	{
		out_info = "解绑成功，扣除的天数：";
		out_info += std::to_string(cfg.decrement_time_);
	}
	else
	{
		out_info = "解绑成功，扣除的分钟数：";
		out_info += std::to_string(cfg.decrement_time_);
	}
	return true;
}

void DbSingleton::GenCardInfo( std::string& card_id, std::string& card_pwd )
{
	card_id = RandomUniqueStrEx(kCardIdLenEx);
	card_pwd = RandomUniqueStrEx(kCardPwdLenEx);
}

DataH_UserGrant& DbSingleton::GetTbl_UserGrant()
{
	return tbl_user_grant_;
}

bool DbSingleton::Grant_RemoveBind( const std::string& user_name_src, const std::string& user_name_dst, int minus_minutes, std::string& out_info )
{	
	auto user_ac_dst = tbl_account_.FindUser(user_name_dst);
	if ( !user_ac_dst )
	{
		out_info = "该用户不存在->" + user_name_dst;
		return false;
	}
	bool remove_bind_king = false;
	if ( !tbl_user_grant_.HasTheGrant(user_name_src, kUG_RemoveBind) )
	{
		remove_bind_king = tbl_user_grant_.HasTheGrant(user_name_src, kUG_RemoveBindKing);
		if ( !remove_bind_king )
		{
			out_info = "没有解绑权限";
			return false;
		}
	}
	if (tbl_user_grant_.LessEqGrant(user_name_src, user_name_dst))
	{
		out_info = "该用户的权限等级高于等于你，你解绑不了的";
		return false;
	}
	if (tbl_user_grant_.HasTheGrant(user_name_dst, kUG_RestrictBind))
	{
		if ( !remove_bind_king )
		{
			remove_bind_king = tbl_user_grant_.HasTheGrant(user_name_src, kUG_RemoveBindKing);
			if ( !remove_bind_king )
			{
				out_info = "对方帐号限制解绑，你解绑不了的";
				return false;
			}
		}
	}

	//使用时间到期
	DateTimeLocal time_now;
	if ( time_now >= user_ac_dst->end_day_time_ )
	{
		out_info = "目标用户没有使用时间了，不能解绑";
		return false;
	}
	if (user_ac_dst->machine_code_.empty())
	{
		out_info = "已解绑，勿需再解";
		return false;
	}
	if ( !tbl_account_.RemoveMachineCode(user_name_dst, minus_minutes) )
	{
		out_info = "未知严重错误";
		return false;
	}
	const auto& cfg = stCS_CfgInfo::GetMe();
	if ( 0 == cfg.radio_time_unit_ )
	{
		out_info = "解绑成功，扣除的天数：";
		out_info += std::to_string(cfg.decrement_time_);
	}
	else
	{
		out_info = "解绑成功，扣除的分钟数：";
		out_info += std::to_string(cfg.decrement_time_);
	}
	return true;
}

const DataH_RegKey& DbSingleton::GetTbl_RegKey() const
{
	return tbl_reg_key_;
}

bool DbSingleton::Grant_GenRegKey(const std::string& add_user, Poco::UInt16 reg_key_cnt, 
	pt_dword ac_type, DataH_RegKey::RegKeysInfoT* reg_keys, std::string& out_info)
{
	if ( !tbl_user_grant_.HasTheGrant(add_user, kUG_GenRegKey) )
	{
		out_info = "没有生成注册码的权限";
		return false;
	}

	std::string reg_key_id;
	for ( Poco::UInt16 i = 0; i != reg_key_cnt; ++i )
	{
		GenRegKeyInfo(reg_key_id);
		if ( !tbl_reg_key_.AddRegKey(add_user, reg_key_id, ac_type) )
		{
			out_info = "生成注册码失败";
			return false;
		}
		if (reg_keys)
			reg_keys->push_back(reg_key_id);
	}
	out_info = "注册码生成成功";
	return true;
}

void DbSingleton::GenRegKeyInfo( std::string& reg_key )
{
	reg_key = RandomUniqueStrEx(kRegKeyIdLenEx);
}

bool DbSingleton::UserLeaveMsg( const std::string& msg, const std::string& user_name, std::string& out_info )
{
	if ( !tbl_msg_board_.AddLeaveMsg(msg, user_name, out_info) )
	{
		if (out_info.empty())
		{
			out_info = "留言之未知错误";
		}
		return false;
	}
	out_info = "留言成功，我们会尊重阁下的宝贵意见";
	return true;
}

bool DbSingleton::Grant_PeerAcByCard( const std::string& user_name, const std::string& card_id, const std::string& card_pwd, stData_Account::AcContT& ac_cont, std::string& out_info )
{
	auto user_ac = tbl_account_.FindUser(user_name);
	if ( !user_ac )
	{
		out_info = "该用户不存在->" + user_name;
		return false;
	}
	if ( !tbl_user_grant_.HasTheGrant(user_name, kUG_PeerAcByCard) )
	{
		out_info = "没有该权限";
		return false;
	}
	auto card_info = tbl_card_.FindCard(card_id);
	if ( !card_info )
	{
		out_info = "卡号不存在";
		return false;
	}
	if ( card_info->card_pwd_ != card_pwd )
	{
		out_info = "卡密错误";
		return false;
	}
	if ( !card_info->is_used_ )
	{
		out_info = "该卡还未被使用";
		return false;
	}
	if (card_info->user_name_.empty())
	{
		out_info = "该卡虽被使用了，但使用者竟然为空";
		return false;
	}
	auto dst_user_ac = tbl_account_.FindUser(card_info->user_name_);
	if ( !dst_user_ac )
	{
		out_info = "该卡虽然有使用者，但是其实该用户竟是不存在的";
		return false;
	}
	if (tbl_user_grant_.LessEqGrant(user_name, card_info->user_name_))
	{
		out_info = "卡的使用者的权限等级高于等于你，你无法查看对方的信息";
		return false;
	}
	ac_cont.push_back(*dst_user_ac);
	out_info = "查看帐号信息成功";
	return true;
}

bool DbSingleton::Grant_PeerCardByAc( const std::string& user_name, const std::string& ac, const std::string& ac_pwd, stData_Card::CardContT& card_cont, std::string& out_info )
{
	auto user_ac = tbl_account_.FindUser(user_name);
	if ( !user_ac )
	{
		out_info = "该用户不存在->" + user_name;
		return false;
	}
	if ( !tbl_user_grant_.HasTheGrant(user_name, kUG_PeerCardByAc) )
	{
		out_info = "没有该权限";
		return false;
	}
	auto dst_user_ac = tbl_account_.FindUser(ac);
	if ( !dst_user_ac )
	{
		out_info = "要查询的帐号不存在";
		return false;
	}
	if (tbl_user_grant_.LessEqGrant(user_name, ac))
	{
		out_info = ac + "的权限等级高于等于你，你无法查看对方的信息";
		return false;
	}
	if ( dst_user_ac->user_pwd_ != ac_pwd )
	{
		out_info = "目标帐号的密码错误";
		return false;
	}

	tbl_card_.DoItem([&card_cont, &ac]( const stData_Card& card ){
		if ( card.is_used_ && card.user_name_ == ac )
			card_cont.push_back(card);
	});
	if (card_cont.empty())
	{
		out_info = ac + "的时间没有通过充值卡的方式来获得，所以查询不到卡信息";
		return false;
	}
	out_info = "查看帐号信息成功";
	return true;
}

bool DbSingleton::Grant_PeerAcInfo( const std::string& user_name, const std::string& ac, const std::string& ac_pwd, stData_Account::AcContT& ac_cont, std::string& out_info )
{
	auto user_ac = tbl_account_.FindUser(user_name);
	if ( !user_ac )
	{
		out_info = "该用户不存在->" + user_name;
		return false;
	}
	if ( !tbl_user_grant_.HasTheGrant(user_name, kUG_PeerAcInfo) )
	{
		out_info = "没有该权限";
		return false;
	}
	auto dst_user_ac = tbl_account_.FindUser(ac);
	if ( !dst_user_ac )
	{
		out_info = "要查询的帐号不存在";
		return false;
	}
	if (tbl_user_grant_.LessEqGrant(user_name, ac))
	{
		out_info = ac + "的权限等级高于等于你，你无法查看对方的信息";
		return false;
	}
	if ( dst_user_ac->user_pwd_ != ac_pwd )
	{
		out_info = "目标帐号的密码错误";
		return false;
	}

	ac_cont.push_back(*dst_user_ac);
	out_info = "查看帐号信息成功";
	return true;
}

bool DbSingleton::Grant_PeerCardInfo( const std::string& user_name, const std::string& card_id, const std::string& card_pwd, stData_Card::CardContT& card_cont, std::string& out_info )
{
	auto user_ac = tbl_account_.FindUser(user_name);
	if ( !user_ac )
	{
		out_info = "该用户不存在->" + user_name;
		return false;
	}
	if ( !tbl_user_grant_.HasTheGrant(user_name, kUG_PeerCardInfo) )
	{
		out_info = "没有该权限";
		return false;
	}
	auto card_info = tbl_card_.FindCard(card_id);
	if ( !card_info )
	{
		out_info = "卡号不存在";
		return false;
	}
	if ( card_info->card_pwd_ != card_pwd )
	{
		out_info = "卡密错误";
		return false;
	}
	card_cont.push_back(*card_info);
	out_info = "查看帐号信息成功";
	return true;
}

bool DbSingleton::Grant_RollbackPay( const std::string& user_name, const std::string& card_id, const std::string& card_pwd, bool direction, std::string& out_info )
{
	auto user_ac = tbl_account_.FindUser(user_name);
	if ( !user_ac )
	{
		out_info = "该用户不存在->" + user_name;
		return false;
	}
	if ( !tbl_user_grant_.HasTheGrant(user_name, kUG_RollbackPay) )
	{
		out_info = "没有该权限";
		return false;
	}
	auto card_info = tbl_card_.FindCard(card_id);
	if ( !card_info )
	{
		out_info = "卡号不存在";
		return false;
	}
	if ( card_info->card_pwd_ != card_pwd )
	{
		out_info = "卡密错误";
		return false;
	}
	if ( !card_info->is_used_ )
	{
		out_info = "该卡并未被使用";
		return false;
	}
	if (card_info->user_name_.empty())
	{
		out_info = "该卡虽然被使用了，但并未充值给任何人";
		return false;
	}
	auto dst_user = tbl_account_.FindUser(card_info->user_name_);
	if ( !dst_user )
	{
		out_info = "该卡虽然被使用了，但是现在已找不到该用户了，用户名：";
		out_info += card_info->user_name_;
		return false;
	}
	auto old_time = dst_user->end_day_time_;
	DateTimeLocal end_day_time = old_time;
	if (direction)
		end_day_time -= Poco::Timespan(0, 0, card_info->card_minutes_, 0, 0);
	else
		end_day_time += Poco::Timespan(0, 0, card_info->card_minutes_, 0, 0);

	if ( !tbl_account_.UpdateEndDayTime(card_info->user_name_, end_day_time, out_info) )
	{
		if (out_info.empty())
		{
			out_info = "未知严重错误";
			return false;
		}
		return false;
	}

	out_info = "支付回滚成功，用户[";
	out_info += card_info->user_name_;
	if (direction)
		out_info += "]减去的分钟数：";
	else
		out_info += "]加上的分钟数：";
	out_info += std::to_string(card_info->card_minutes_);
	out_info += " 旧的到期时间：";
	out_info += Poco::DateTimeFormatter::format(old_time, Poco::DateTimeFormat::SORTABLE_FORMAT);
	out_info += " 新的到期时间：";
	out_info += Poco::DateTimeFormatter::format(end_day_time, Poco::DateTimeFormat::SORTABLE_FORMAT);
	return true;
}

void DbSingleton::OnReconnected()
{
	if (IsConnected())
		SelectAllTblData();
}

bool DbSingleton::GetAddrDatas(std::string& str_key, DataH_AddrData::EncryptAddrDataT& out_data)
{
	Secret_XorKey algo_xor_key;
	str_key = RandomUniqueStr(5);
	algo_xor_key.SetKey(str_key.c_str(), str_key.size());

	return tbl_addr_data_.GetEncrytAddrData(&algo_xor_key, out_data);
}

bool DbSingleton::GetStrDatas(std::vector<std::string>& the_data)
{
	return tbl_str_data_.GetAllStrData(the_data);
}

bool DbSingleton::Grant_SetAddrData(Poco::UInt32 id, Poco::UInt32 addr_data, std::string& info)
{
	if ( !tbl_addr_data_.SetAddrData(id, addr_data) )
	{
		info = "发生严重错误";
		return false;
	}
	info = "设置addr data成功";
	return true;
}

bool DbSingleton::Grant_SetStrData(Poco::UInt32 id, const std::string& str_data, std::string& info)
{
	if ( !tbl_str_data_.SetStrData(id, str_data) )
	{
		info = "发生严重错误";
		return false;
	}
	info = "设置str data成功";
	return true;
}

bool DbSingleton::Grant_SetAcState(const std::string& user_name_src, const std::string& user_name_dst, enAcState ac_state, Poco::UInt32 period, std::string& out_info)
{
	if (ac_state >= enAcState::kAS_Invalid)
	{
		out_info = "状态数值错误";
		return false;
	}
	if (enAcState::kAS_Ban == ac_state && 0 == period)
	{
		out_info = "无效的封号时间";
		return false;
	}
	auto user_ac_dst = tbl_account_.FindUser(user_name_dst);
	if (!user_ac_dst)
	{
		out_info = "该用户不存在->" + user_name_dst;
		return false;
	}
	if (!tbl_user_grant_.HasTheGrant(user_name_src, kUG_MgrAcState))
	{
		out_info = "没有管理帐号的权限";
		return false;
	}
	if (tbl_user_grant_.LessEqGrant(user_name_src, user_name_dst))
	{
		out_info = "该用户的权限等级高于等于你";
		return false;
	}

	auto ac_state_dst = tbl_ac_state_.FindState(user_name_dst);
	if (ac_state_dst)
	{
		if (ac_state_dst->state_ == ac_state)
		{
			out_info = "帐号帐状态已经设置成功了，无需重复操作";
			return false;
		}
		//只有从冻结状态下恢复，时间才不会流逝。
		if (enAcState::kAS_Freeze == ac_state_dst->state_ && enAcState::kAS_Normal == ac_state)
		{
			if (user_ac_dst->end_day_time_ > ac_state_dst->begin_time_)
			{
				auto time_span = user_ac_dst->end_day_time_ - ac_state_dst->begin_time_;
				DateTimeLocal time_now;
				if (!tbl_account_.UpdateEndDayTime(user_name_dst, time_now + time_span, out_info))
				{
					if (out_info.empty())
						out_info = "未知错误981";
					return false;
				}
			}
		}
	}
	else
	{
		if (enAcState::kAS_Normal == ac_state)
		{
			out_info = "该用户帐号正常，无需恢复";
			return false;
		}
	}
	if (!tbl_ac_state_.SetState(user_name_dst, ac_state, period))
	{
		out_info = "未知严重错误324";
		return false;
	}
	out_info = "帐号状态设置成功";
	return true;
}

bool DbSingleton::GetAcStates(const stData_Account::AcContT& ac_cont, stData_AcState::ContT& ac_states)
{
	if (ac_cont.empty())
		return true;
	for (auto& v : ac_cont)
	{
		auto ac_state = tbl_ac_state_.FindState(v.user_name_);
		if (ac_state && enAcState::kAS_Normal != ac_state->state_)
			ac_states.push_back(*ac_state);
	}
	return true;
}

const Poco::Timespan DbSingleton::cookie_time_span(0, 24, 0, 0, 0);


DataH_UserGrant::DataH_UserGrant( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "tbl_UserGrant")
{

}

bool DataH_UserGrant::HasTheGrant( const std::string& user_ac, enUserGrant user_grant )
{
	auto grant = GetUserGrant(user_ac);
	if ( kUG_No == grant )
		return false;
	grant &= user_grant;
	return grant == user_grant;
}

enUserGrant DataH_UserGrant::GetUserGrant( const std::string& user_ac )
{
	auto user_grant = FindUserGrant(user_ac);
	if ( !user_grant )
		return kUG_No;
	
	return user_grant->GetUserGrant();
}

stData_UserGrant* DataH_UserGrant::FindUserGrant( const std::string& user_ac )
{
	if (user_ac.empty())
	{
		assert(false);
		return nullptr;
	}
	auto it = std::find_if(datas_.begin(), datas_.end(), [&user_ac]( const DataType& data ){
		return data.user_name_ == user_ac;
	});
	if ( it == datas_.end() )
		return nullptr;
	return &*it;
}

bool DataH_UserGrant::LessGrant( const std::string& lhs, const std::string& rhs )
{
	auto lhs_grant = GetUserGrant(lhs);
	auto rhs_grant = GetUserGrant(rhs);
	return lhs_grant < rhs_grant;
}

bool DataH_UserGrant::LessEqGrant( const std::string& lhs, const std::string& rhs )
{
	auto lhs_grant = GetUserGrant(lhs);
	auto rhs_grant = GetUserGrant(rhs);
	return lhs_grant <= rhs_grant;
}

bool DataH_UserGrant::AddGrant( const std::string& user_ac, enUserGrant grant )
{
	using namespace Poco::Data::Keywords;
	if ( grant == kUG_No )
		return true;
	auto the_grant = FindUserGrant(user_ac);
	if (the_grant)
	{
		auto new_grant = the_grant->user_grant_ & grant;
		if ( new_grant == grant )
			return true;
		new_grant = the_grant->user_grant_ | grant;
		DataExecH exec_helper;
		DATA_EXCEPTION_BEGIN;
		auto the_name = user_ac;
		Update(DataColumn("user_grant"), 
			DataWhere("UserName", DataCondition::eq, the_name)),
			use(new_grant), now;
		the_grant->user_grant_ = new_grant;
		DATA_EXCEPTION_END(exec_helper);
		return exec_helper;
	}
	else
	{
		DataType data_grant;
		data_grant.user_name_ = user_ac;
		data_grant.user_grant_ = grant;

		DataExecH exec_helper;
		DATA_EXCEPTION_BEGIN;
		using namespace Poco::Data::Keywords;
		InsertInto(), use(data_grant), now;
		datas_.push_back(data_grant);
		DATA_EXCEPTION_END(exec_helper);
		return exec_helper;
	}
}

DataH_RegKey::DataH_RegKey( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "tbl_RegKey")
{

}

stData_RegKey* DataH_RegKey::FindRegKey( const std::string& reg_key_id )
{
	auto it = std::find_if(datas_.begin(), datas_.end(), [&reg_key_id]( const DataType& data ){
		return data.reg_key_id_ == reg_key_id;
	});
	if ( it != datas_.end() )
		return &*it;
	return nullptr;
}

bool DataH_RegKey::AddRegKey(const std::string& add_user, const std::string& reg_key_id, pt_dword ac_type)
{
	if (reg_key_id.empty())
		return false;

	if (FindRegKey(reg_key_id))
		return false;

	DataType reg_key_tmp;
	reg_key_tmp.reg_key_id_ = reg_key_id;
	reg_key_tmp.add_user_ = add_user;
	reg_key_tmp.ac_type_ = ac_type;

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;
	InsertInto(), use(reg_key_tmp), now;
	datas_.push_back(reg_key_tmp);
	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

bool DataH_RegKey::Use( const std::string& user_name, const std::string& reg_key_id, std::string& out_info )
{
#ifdef FREE_LOGIN_
	return true;
#endif

	auto reg_key = FindRegKey(reg_key_id);
	if ( !reg_key )
	{
		out_info = "注册码不存在";
		return false;
	}
	if (reg_key->is_used_)
	{
		out_info = "注册码已被使用过";
		return false;
	}
	DateTimeLocal use_time;
	bool is_used = true;
	reg_key->user_name_ = user_name;
	
	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	using namespace Poco::Data::Keywords;

	Update(DataColumn("IsUsed").Add("UserName").Add("UseTime"), 
		DataWhere("RegKeyId", DataCondition::eq, reg_key_id)),
		use(is_used), use(reg_key->user_name_), use(use_time),
		now;

	reg_key->is_used_ = is_used;	
	reg_key->use_time_ = use_time;

	DATA_EXCEPTION_END(exec_helper);

	return exec_helper;
}

bool DataH_RegKey::TryUse(const std::string& reg_key_id, pt_dword& ac_type, std::string& out_info)
{
#ifdef FREE_LOGIN_
	return true;
#endif
	auto reg_key = FindRegKey(reg_key_id);
	if ( !reg_key )
	{
		out_info = "注册码不存在";
		return false;
	}
	if (reg_key->is_used_)
	{
		out_info = "注册码已被使用过";
		return false;
	}
	ac_type = reg_key->ac_type_;
	return true;
}

void DataH_StrData::OrderById()
{
	std::sort(datas_.begin(), datas_.end(), []( const DataType& lhs, const DataType& rhs ){
		return lhs.id_ < rhs.id_;
	});
}

DataH_StrData::DataH_StrData( DatabaseMgrBase& db_mgr ) : DataTblBase(db_mgr, "tbl_StrData")
{
}

bool DataH_StrData::GetAllStrData( std::vector<std::string>& all_datas )
{
	for ( const auto& str_data : datas_ )
		all_datas.push_back(str_data.str_data_);
	return true;
}

DataH_StrData::DataType* DataH_StrData::FindById( Poco::UInt32 id )
{
	auto it = std::find_if(datas_.begin(), datas_.end(), [id]( const DataType& the_data ){
		return id == the_data.id_;
	});
	if ( it == datas_.end() )
		return nullptr;
	return &*it;
}

bool DataH_StrData::SetStrData( Poco::UInt32 id, std::string str_data )
{
	using namespace Poco::Data::Keywords;
	auto the_data = FindById(id);
	if (the_data)
	{
		if ( the_data->str_data_ == str_data )
			return true;

		DataExecH exec_helper;
		DATA_EXCEPTION_BEGIN;
		Update(DataColumn("StrData"), 
			DataWhere("id", DataCondition::eq, id)),
			use(str_data), now;
		the_data->str_data_ = str_data;
		DATA_EXCEPTION_END(exec_helper);
		return exec_helper;
	}
	else
	{
		DataType new_data;
		new_data.id_ = id;
		new_data.str_data_ = str_data;

		DataExecH exec_helper;
		DATA_EXCEPTION_BEGIN;
		using namespace Poco::Data::Keywords;
		InsertInto(), use(new_data), now;
		datas_.push_back(new_data);
		OrderById();
		DATA_EXCEPTION_END(exec_helper);
		return exec_helper;
	}
}

bool DataH_Account::UpdateCookie(const std::string& user_name, std::string& cookie, DateTimeLocal& cookie_end_time)
{
	using namespace Poco::Data::Keywords;
	auto the_data = FindUser(user_name);
	if (!the_data)
	{
		assert(false);
		return false;
	}

	if (the_data->cookie_ == cookie && the_data->cookie_end_time_ == cookie_end_time)
		return true;

	DataExecH exec_helper;
	DATA_EXCEPTION_BEGIN;
	Update(DataColumn("Cookie").Add("CookieEndTime"),
		DataWhere("UserName", DataCondition::eq, user_name)),
		use(cookie), use(cookie_end_time), now;
	the_data->cookie_ = cookie;
	the_data->cookie_end_time_ = cookie_end_time;
	DATA_EXCEPTION_END(exec_helper);
	return exec_helper;
}

stData_AcState* DataH_AcState::FindState(const std::string& user_name)
{
	auto it = std::find_if(datas_.begin(), datas_.end(), [&user_name](const DataType& data){
		return data.user_name_ == user_name;
	});
	if (it != datas_.end())
		return &*it;
	return nullptr;
}

bool DataH_AcState::SetState(const std::string& user_name, enAcState state, Poco::UInt32 period)
{
	using namespace Poco::Data::Keywords;
	auto the_data = FindState(user_name);
	if (the_data)
	{
		if (the_data->state_ == state)
		{
			assert(false);
			return true;
		}
		DataType data = *the_data;
		data.state_ = state;
		data.last_state_ = the_data->state_;

		if (enAcState::kAS_Normal == state)
			data.end_time_ = DateTimeLocal();
		else
		{
			data.begin_time_ = DateTimeLocal();
			data.period_ = period;
		}

		DataExecH exec_helper;
		DATA_EXCEPTION_BEGIN;
		Update(DataColumn("State").Add("LastState").Add("BeginTime").Add("EndTime").Add("Period"),
			DataWhere("UserName", DataCondition::eq, data.user_name_)),
			use(data.state_), use(data.last_state_), use(data.begin_time_), use(data.end_time_), use(data.period_),
			now;
		*the_data = data;
		DATA_EXCEPTION_END(exec_helper);
		return exec_helper;
	}
	else
	{
		if (enAcState::kAS_Normal == state)
		{
			assert(false);
			return false;
		}
		DataType new_data;
		new_data.user_name_ = user_name;
		new_data.state_ = state;
		new_data.period_ = period;

		DataExecH exec_helper;
		DATA_EXCEPTION_BEGIN;
		using namespace Poco::Data::Keywords;
		InsertInto(), use(new_data), now;
		datas_.push_back(new_data);
		DATA_EXCEPTION_END(exec_helper);
		return exec_helper;
	}
}

DataH_AcState::DataH_AcState(DatabaseMgrBase& db_mgr) : DataTblBase(db_mgr, "tbl_AcState")
{
}

bool DataH_AcState::CanLogin(const std::string& user_name, std::string* out_info)
{
	using namespace Poco;
	auto ac_state = FindState(user_name);
	if (!ac_state)
		return true;
	if (enAcState::kAS_Normal == ac_state->state_)
		return true;
	else if (enAcState::kAS_Freeze == ac_state->state_)
	{
		if (out_info)
			*out_info = "您的帐号因非法操作已被冻结";
	}
	else if (enAcState::kAS_Ban == ac_state->state_)
	{
		auto end_time = ac_state->begin_time_ + Poco::Timespan(0, 0, ac_state->period_, 0, 0);
		if (end_time <= DateTimeLocal())
		{
			if (out_info)
			{
				if (SetState(user_name, enAcState::kAS_Normal, ac_state->period_))
					return true;
				else
				{
					*out_info = "未知错误685";
				}
			}
			else
				return true;
		}
		if (out_info)
		{
			*out_info = "解封时间：";
			*out_info += DateTimeFormatter::format(end_time, DateTimeFormat::SORTABLE_FORMAT);
		}
	}
	else
	{
		assert(false);
		if (out_info)
			*out_info = "未知帐号状态异常";
	}
	return false;
}

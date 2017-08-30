#pragma once
/*
@author zhp
@date 2014/4/21 17:03
@purpose for check server server and session
*/
#include "Net/server.h"
#include "Common/UsefulClass.h"
#include "Net/JsonCall.h"
#include "DataHandlerS.h"
#include "Net/IoTimer.h"
#include <boost/optional.hpp>
#include <set>
#include <functional>
#include <Net/CmnLogin.h>

class Session_CS;
class CheckServer : public Server_Aes_Rsa, public Singleton<CheckServer, Singleton_Instance>{
public:
	CheckServer( DbSingleton::SelfPtr& db_singleton, WORD self_listen_port );
	typedef boost::shared_ptr<CheckServer> SelfPtr;
	typedef std::set<session_ptr> SessionsT;

	//interface
public:
	void DoItem( const std::function<void(Session_CS&)>& func );
	void SessionTimeout();

private:
	bool OnConnected( IoConnectBase* connect );
	void OnDisconnect( IoConnectBase* connect );
	virtual Session* CreateSession();

private:
	DbSingleton::SelfPtr	db_singleton_;
	SessionsT				sessions_;
};

class UserOperS : public BufCall_Socket, public ICmnLoginGrantSink{
public:
	UserOperS( Session_CS& session );
	~UserOperS();

	//用户操作
public:
	//登陆
	bool Login(pt_bool is_only_login, const stCheckLoginInfo<pt_csz>& login_info, pt_int ver);
	//解绑
	bool RemoveBind( pt_csz user_name, pt_csz user_pwd, pt_csz machine_code );
	//注册用户
	bool RegUser( pt_csz user_name, pt_csz user_pwd, pt_csz str_qq, pt_csz reg_key_id );
	//用户支付
	bool UserPay( pt_csz user_name, pt_csz card_id, pt_csz card_pwd );
	//留言
	bool LeaveMsg( pt_csz msg );
	//验证登陆
	bool CheckLogin(const CheckLoginInfoContT<pt_csz>& login_infos);

	//权限管理
public:
	//解绑
	bool Grant_RemoveBind( pt_csz user_name_dst );
	//开卡
	bool Grant_OpenCard(pt_word card_cnt, pt_dword card_minutes, pt_qword grant, pt_dword ac_type);
	//生成注册码
	bool Grant_GenRegKey(pt_word reg_key_cnt, pt_dword ac_type);
	//通过卡号查询帐号信息
	bool Grant_PeerAcByCard( pt_csz card_id, pt_csz card_pwd );
	//通过帐号查询卡号信息
	bool Grant_PeerCardByAc( pt_csz ac, pt_csz ac_pwd );
	//查询帐号信息
	bool Grant_PeerAcInfo( pt_csz ac, pt_csz ac_pwd );
	//查询卡号信息
	bool Grant_PeerCardInfo( pt_csz card_id, pt_csz card_pwd );
	//支付回滚
	bool Grant_RollbackPay( pt_csz card_id, pt_csz card_pwd, bool direction );
	//设置addr data
	bool Grant_SetAddrData( pt_int id, pt_dword addr_data );
	//设置str data
	bool Grant_SetStrData( pt_int id, pt_csz str_data );
	//设置帐号状态
	bool Grant_SetAcState(pt_csz user_name_dst, enAcState ac_state, Poco::UInt32 period);

private:
	//响应登陆
	bool OnLoginRes(bool res, pt_csz info, pt_csz ac_pwd_machine_mix, pt_csz new_cookie, 
		const TimeT& end_day_time, const TimeT& ser_cur_time, const AddrDatasT& addr_datas, 
		const StrDatasT& str_addr_datas, pt_dword ac_type) override;
	//响应解绑
	bool OnRemoveBind( bool res, pt_csz info ) override;
	//响应注册
	bool OnRegUser( bool res, pt_csz info ) override;
	//响应支付
	bool OnUserPay( bool res, pt_csz info ) override;
	//响应留言
	bool OnLeaveMsg( bool res, pt_csz info ) override;
	//响应验证登陆
	bool OnCheckLogin(const CheckLoginResContT& res_info) override;

private:
	bool OnGrant_RemoveBind( bool res, pt_csz info ) override;
	bool OnGrant_OpenCard( bool res, pt_csz info, const CardInfoT& card_info ) override;
	bool OnGrant_GenRegKey( bool res, pt_csz info, const RegKeyInfoT& reg_key ) override;
	bool OnGrant_PeerAcByCard(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state) override;
	bool OnGrant_PeerCardByAc(bool res, pt_csz info, const stData_Card::CardContT& card_info) override;
	bool OnGrant_PeerAcInfo(bool res, pt_csz info, const stData_Account::AcContT& ac_info, const stData_AcState::ContT& ac_state) override;
	bool OnGrant_PeerCardInfo(bool res, pt_csz info, const stData_Card::CardContT& card_info) override;
	bool OnGrant_RollbackPay( bool res, pt_csz info ) override;
	bool OnGrant_SetAddrData( bool res, pt_csz info ) override;
	bool OnGrant_SetStrData( bool res, pt_csz info ) override;
	bool OnGrant_SetAcState(bool res, pt_csz info) override;

private:
	Session_CS&				session_;

	BC_REG_CLS_DECLARE(UserOperS);
};

class Session_CS : public Session_Rsa{
	friend class UserOperS;

public:
	Session_CS( CheckServer& server );
	~Session_CS();

	//interface
public:
	//设置single数据库
	void SetSingleDb( DbSingleton::SelfPtr& db_singleton );
	const std::string& GetLoginUserName() const;
	bool IsTimeout( const DateTimeLocal& time_now ) const;

private:
	//活跃会话
	void ActiveSessionTime();

private:
	bool OnConnected( IoConnectBase* connect );
	void OnDisconnect( IoConnectBase* connect );

private:
	UserOperS				user_oper_;
	DateTimeLocal			active_time_;

private:
	DbSingleton::SelfPtr	db_singleton_;

private:
	std::string				user_name_;			//用户名
};
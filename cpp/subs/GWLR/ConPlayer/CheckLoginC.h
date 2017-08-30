#pragma once
/*
@author zhp
@date 2014/4/28 0:08
@purpose for check and login
*/
#include "Script/JsonCall.h"
#include "socket/client.h"
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <Poco/Types.h>

class CheckLoginC;
class CheckLoginOperC : public JsonEchoVmp{
public:
	CheckLoginOperC( CheckLoginC& check_login );

	//用户操作
public:
	//登陆
	void Login( const std::string& user_name, const std::string& user_pwd );
	//解绑
	void RemoveBind( const std::string& user_name, const std::string& user_pwd, const std::string& machine_code );
	//注册
	void RegUser( const std::string& user_name, const std::string& user_pwd, const std::string& str_qq, const std::string& reg_key_id );
	//充值
	void Pay( const std::string& user_name, const std::string& card_id, const std::string& card_pwd );
	//得到关键数据
	void GetAddrDatas();
	//验证登陆
	static void CheckLogin( const std::string& user_name, const std::string& user_pwd );
	//留言
	std::string LeaveMsg( const std::string& msg );
	//得到机器码
	static std::string GetMachineCode();

	//用户操作结果
public:
	//响应登陆
	void OnLoginAcInfo( const std::string& ac_pwd );
	//响应解绑
	void OnRemoveBind( bool succeed );
	//响应注册
	void OnRegUser( bool succeed );
	//响应支付
	void OnUserPay( bool succeed );
	//CheckLogin result
	void OnCheckLoginResult( int res_reson );

	//返回信息
public:
	//登陆信息
	void OnLoginInfo( const std::string& info );
	//解绑信息
	void OnRemoveBindInfo( const std::string& info );
	//注册信息
	void OnRegUserInfo( const std::string& info );
	//支付信息
	void OnUserPayInfo( const std::string& info );
	//接收关键数据
	void OnRecvAddrDatas( const std::string& str_key, const Json::Value& addr_datas );

	//返回其他
public:
	//截止时间
	void OnUserEndTime( Poco::UInt32 timestamp_lower, Poco::UInt32 timestamp_upper );

protected:
	virtual bool OnThisMemCall( const stThisCallHelper& helper );

private:
	CheckLoginC&				check_login_;
};

class CheckLoginC : public Con_Aes_Rsa{
	friend class CheckLoginOperC;
public:
	typedef boost::shared_ptr<CheckLoginC> SelfPtr;
	typedef boost::weak_ptr<CheckLoginC> WeakPtr;

public:
	CheckLoginC( const std::string& check_server_ip, const CryptoStrCompose::SubCryptoT& stm_pub, 
		const CryptoStrCompose::SubCryptoT& stm_pri );
	//登陆成功后做的事
	static bool AfterLoginSucced( const std::function<void( SelfPtr& conne_self )>& func );

	//interface
public:
	CheckLoginOperC& GetUserOper();
	static SelfPtr GetSelfPtr();
	void OnDisconnect( IoConnectBase* connect );
	
protected:
	bool OnConnected( IoConnectBase* connect );
	virtual void OnConnectFailed();

private:
	CheckLoginOperC				check_server_oper_;

private:
	boost::optional<bool>		login_res_;
	boost::optional<bool>		remove_bind_;
	boost::optional<bool>		reg_user_;
	boost::optional<bool>		user_pay_;
};
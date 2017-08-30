#pragma once
/*
@author zhp
@date 2014/6/24 2:24
@purpose for anti-crack connection
*/
#include <Socket/client.h>
#include <Script/JsonCall.h>
#include <boost/optional.hpp>

class AC_Connect;

class AC_OperC : public JsonEchoBase{
public:
	AC_OperC( AC_Connect& ac_conne );

	//user operators
public:
	//验证登陆
	void CheckLogin( const std::string& user_name, const std::string& user_pwd );

protected:
	virtual bool OnThisMemCall( const stThisCallHelper& helper );

private:
	AC_Connect&				ac_connect_;
};

class AC_Connect : public Con_Aes_Rsa{
	friend class AC_OperC;
public:
	AC_Connect( const std::string& check_server_ip, const CryptoStrCompose::SubCryptoT& stm_pub, 
		const CryptoStrCompose::SubCryptoT& stm_pri );

	AC_OperC& GetOper();

public:
	typedef boost::shared_ptr<AC_Connect> SelfPtr;
	typedef boost::weak_ptr<AC_Connect> WeakPtr;
	static SelfPtr GetSelfPtr();
	static bool InitConne();
	static void ClrConne();

protected:
	bool OnConnected( IoConnectBase* connect );
	virtual void OnConnectFailed();

private:
	AC_OperC				ac_oper_;

private:
	static boost::optional<bool>					check_svr_connect_succeed_;			//验证服务器是否连接成功
	static CryptoStrCompose::SubCryptoT				public_key_;
	static CryptoStrCompose::SubCryptoT				private_key_;
};
#pragma once
/*
@author zhp
@date 2013-12-21
@purpose client connection
*/
#include "FuncObjs/FuncObj.h"
#include "IoConnect.h"
#include "IoTimer.h"
#include "../DependSelfLib.h"

/*
#include <boost/thread/lock_types.hpp>
#include <boost/thread/mutex.hpp>*/

class Server;
class Session : public FuncObjTop_Secret, public IoConnectBase, public IoConnectSelfHolder{
public:
	Session( Server& server );
	~Session();
	void start();
	void Disconnect() override;
	void SetKey(const std::string& key);
	const std::string& GetKey() const;
	
protected:
	virtual bool InitOnce();

private:
	void DoDisconnect() override;
	bool DoConnectDone() override;
	bool SortSecretKey( pt_csz recv_key, pt_dword key_size ) override;

protected:
	Server&			server_;
	std::string		key_;
};

typedef boost::shared_ptr<Session> session_ptr;

class Server : public IoConnectsMgr{
/*
public:
	typedef boost::mutex MutexT;
	typedef boost::unique_lock<MutexT> MutextLockT;
*/
public:
	Server(  pt_word host_port );
	session_ptr FindSessionByKey(const std::string& key) const;
	
protected:
	void handle_accept(session_ptr new_session,
		const boost::system::error_code& error);

protected:
	virtual Session* CreateSession();

protected:
	boost::asio::ip::tcp::acceptor	acceptor_;
};

//aesº”√‹
class Session_Aes : public Session{
private:
	Session_Aes( Server& server );

protected:
	bool HandleSecrectKey( const char* sercret_key, size_t sercret_key_size );
	bool SetMixSecretKey( const char* server_key, size_t server_key_size );
	bool InitOnce();
	bool AddNewAesMixKey( const std::string& new_mix_key );
	bool SetAcPwdMachineCodeMixKey( const std::string& new_mix_key );

protected:
	CryptoStrCompose::SubCryptoT		secret_aes_send_;
	CryptoStrCompose::SubCryptoT		secret_aes_recv_;

private:
	bool								ac_pwd_machine_code_;
};

//aes°¢rsaº”√‹
class Session_Rsa : public Session{
public:
	Session_Rsa( Server& server, const CryptoStrCompose::SubCryptoT& stm_pub, 
		const CryptoStrCompose::SubCryptoT& stm_pri );

protected:
	bool InitOnce();

private:
	CryptoStrCompose::SubCryptoT		stm_pub_;
	CryptoStrCompose::SubCryptoT		stm_pri_;
};

//server aes°¢rsaº”√‹
class Server_Aes_Rsa : public Server{
public:
	explicit Server_Aes_Rsa(pt_word host_port);
	CryptoStrCompose::SubCryptoT& GetRsaPublicKey();
	CryptoStrCompose::SubCryptoT& GetRsaPrivateKey();

protected:
	Crypto_RsaInitor			rsa_key_;
};
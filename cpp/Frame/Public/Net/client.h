#pragma once
/*
@author zhp
@date 2013-12-22
@purpose client connection
*/
#include "FuncObjs/FuncObj.h"
#include "IoConnect.h"
#include "IoTimer.h"
#include "../DependSelfLib.h"

class Connection : public FuncObjTop_Secret, public IoConnectBase, public IoConnectSelfHolder{
public:
	Connection( const std::string& dest_ip, pt_word dest_port );
	//设置ip，端口
	void SetServerInfo( const std::string& dest_ip, pt_word dest_port );
	//执行连接
	void ConnectAsync();
	bool Connect();
	void Disconnect() override;
	//等待直到连接完成
	bool WaitForConnectDone() const;

protected:
	void handle_connect( const boost::system::error_code& error );
	//连接前的初始化
	virtual bool PreConnect();
	//connect failed
	virtual void OnConnectFailed();

private:
	void DoDisconnect() override;
	void OccurSomeError_() override;
	bool DoConnectDone() override;
	bool SortSecretKey( pt_csz recv_key, pt_dword key_size ) override;

private:
	pt_word				dest_port_;
	std::string			dest_ip_;
	OptionalBool		connected_done_;
};

class ConnectionAesBase : public Connection{
public:
	ConnectionAesBase( const std::string& dest_ip, pt_word dest_port );

protected:
	bool HandleSecrectKey( const char* sercret_key, size_t sercret_key_size );
	bool SetMixSecretKey( const char* server_key, size_t server_key_size );
	bool AddNewAesMixKey( const std::string& new_mix_key );
	bool SetAcPwdMachineCodeMixKey( const std::string& new_mix_key );

protected:
	CryptoStrCompose::SubCryptoT		secret_aes_send_;
	CryptoStrCompose::SubCryptoT		secret_aes_recv_;

private:
	bool								ac_pwd_machine_code_;
};

/*
aes加密现已废弃，原因有二：
1.有了RC4与RSA足矣，AES已无必要
2.服务器与客户端未成功设置好密钥的情况下发包，一定会出现问题。
*/
//aes加密
class Connection_Aes : public ConnectionAesBase{
private:
	Connection_Aes( const std::string& dest_ip, pt_word dest_port );

protected:
	bool PreConnect() override;	
};

//aes、rsa加密
class Connection_Rsa : public Connection{
public:
	Connection_Rsa( const std::string& dest_ip, pt_word dest_port, const CryptoStrCompose::SubCryptoT& stm_pub, 
		const CryptoStrCompose::SubCryptoT& stm_pri );

protected:
	bool PreConnect() override;

private:
	CryptoStrCompose::SubCryptoT		stm_pub_;
	CryptoStrCompose::SubCryptoT		stm_pri_;
};
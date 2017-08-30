#pragma once
/*
@author zhp
@date 2014/4/16 23:30
@purpose for encrypt or decrypt
*/
#include <string>
#include <deque>
#include "../DependSelfLib.h"
#include <memory>

class ISecretMaker{
public:
	virtual ~ISecretMaker();
	//设置密钥
	virtual void SetKey( const char* key, pt_dword key_size ) = 0;
	//加密
	virtual void Encrypt( const char* buffer_src, pt_dword buffer_size, char* cryptograph ) = 0;
	//解密
	virtual void Decrypt( const char* cryptograph, pt_dword cryptograph_size, char* buffer_src ) = 0;
};

//字符串加密
class ICryptoStr{
public:
	enum Encoding{
		ENC_NONE         = 0x00, /// Plain binary output
		ENC_BASE64       = 0x01, /// Base64-encoded output
		ENC_BINHEX       = 0x02, /// BinHex-encoded output
		ENC_BASE64_NO_LF = 0x81, /// Base64-encoded output, no linefeeds
		ENC_BINHEX_NO_LF = 0x82, /// BinHex-encoded output, no linefeeds
	};
	virtual ~ICryptoStr();
	virtual void SetKey( const std::string& str_key ) = 0;
	virtual void SetKey( std::istream& istrm ) = 0;
	virtual std::string Encrypt( const std::string& str_src ) = 0;
	virtual std::string Decrypt( const std::string& str_crypto ) = 0;
	virtual Encoding SetEncoding( Encoding enco ) = 0;
};

//组合加密
class CryptoStrCompose : public ICryptoStr{
public:
	typedef std::shared_ptr<ICryptoStr> SubCryptoT;
	virtual void SetKey( const std::string& str_key );
	virtual void SetKey( std::istream& istrm );
	virtual std::string Encrypt( const std::string& str_src );
	virtual std::string Decrypt( const std::string& str_crypto );
	void AddCrypto( const SubCryptoT& crypto );
	void AddCryptoToFirst( const SubCryptoT& crypto );
	void EraseTheCrypto( const SubCryptoT& crypto );
	virtual Encoding SetEncoding( Encoding enco );

private:
	std::deque<SubCryptoT>		cryptos_;
};

//加密与解密算法相同
class Secret_DoSameAlgo : public ISecretMaker{
public:
	virtual void Encrypt( const char* buffer_src, pt_dword buffer_size, char* cryptograph );
	virtual void Decrypt( const char* cryptograph, pt_dword cryptograph_size, char* buffer_src );

protected:
	virtual void DoAlgo( const char* src, pt_dword src_size, char* dst ) = 0;
};

//RC4流密码
class Secret_RC4 : public Secret_DoSameAlgo{
public:
	Secret_RC4();

public:
	virtual void SetKey( const char* key, pt_dword key_size );
	void MixOldKey( const char* key, pt_dword key_size );

private:
	virtual void DoAlgo( const char* src, pt_dword src_size, char* dst );

private:
	char			stream_[256];
};

//仅仅是异或
class Secret_XorKey : public Secret_DoSameAlgo{
public:
	Secret_XorKey();

public:
	virtual void SetKey( const char* key, pt_dword key_size );

private:
	virtual void DoAlgo( const char* src, pt_dword src_size, char* dst );

private:
	std::string					key_;
};

//Poco::Crypto
class Crypto_Base : public ICryptoStr{
public:
	Crypto_Base( Encoding ecd );
	~Crypto_Base();
	virtual void SetKey( const std::string& str_key );
	virtual void SetKey( std::istream& istrm );
	virtual std::string Encrypt( const std::string& str_src );
	virtual std::string Decrypt( const std::string& str_crypto );
	virtual Encoding SetEncoding( Encoding enco );

protected:
	virtual void InitCipher( const std::string& str_key ) = 0;
	virtual void InitCipher( std::istream& istrm );

protected:
	void*			cipher_;
	Encoding		encoding_;
};

//aes
class Crypto_Aes : public Crypto_Base{
public:
	Crypto_Aes( const std::string& str_key, Encoding enco = ENC_NONE );

private:
	void InitCipher( const std::string& str_key );
};

class Crypto_RsaBase : public Crypto_Base{
protected:
	Crypto_RsaBase( Encoding ecd );
};

//rsa public key
class Crypto_Rsa_Pub : public Crypto_RsaBase{
public:
	Crypto_Rsa_Pub( std::istream& istrm, Encoding enco = ENC_NONE );
	virtual std::string Decrypt( const std::string& str_crypto );

private:
	void InitCipher( const std::string& str_key );
	virtual void InitCipher( std::istream& istrm );
};

//rsa private key
class Crypto_Rsa_Pri : public Crypto_RsaBase{
public:
	Crypto_Rsa_Pri( std::istream& istrm, const std::string& passphrase, Encoding enco = ENC_NONE );
	virtual std::string Encrypt( const std::string& str_src );

private:
	void InitCipher( const std::string& str_key );
	virtual void InitCipher( std::istream& istrm );

private:
	std::string				passphrase_;
};

struct Crypto_RsaInitor{
	bool Init();

	CryptoStrCompose::SubCryptoT			public_key_;
	CryptoStrCompose::SubCryptoT			private_key_;
};

#include "stdafx.h"
#include <Common/MakeSecret.h>
#include "protect/Protect.h"
#include <cassert>
#include <Poco/Crypto/CipherFactory.h>
#include <Poco/Crypto/CipherKey.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/RSAKey.h>
#include <Common/UsefulClass.h>
#include <Common/LogOut.h>
#include <strstream>
#include <Poco/StreamCopier.h>
#include <Common/CmnException.h>
#include <Common/UsefulClass.h>
#include <AssetSys/AS_Sys.h>

typedef Poco::Crypto::Cipher CipherT;

Secret_RC4::Secret_RC4()
{
	for ( int i = 0;i != _countof(stream_); ++i )
		stream_[i] = (pt_char)i;
}

void Secret_RC4::SetKey( const char* key, pt_dword key_size )
{
	if ( !key || key_size <= 0 )
	{
		assert(false);
		return;
	}
	VMP_BEGIN_EX;
	int total_value = 0;
	for ( pt_dword i = 0;i != key_size; ++i )
	{
		total_value += key[i];
	}
	const auto kIdxLimit = total_value % _countof(stream_);
	char element_value = 0;
	for ( auto i = kIdxLimit; i != _countof(stream_); ++i )
	{
		stream_[i] = element_value++;
	}
	for ( auto i = 0;i != kIdxLimit; ++i )
	{
		stream_[i] = element_value++;
	}
	FrameCheck::GetMe().TryCheck();
	int j = 0;
	for ( auto i = 0;i != _countof(stream_); ++i )
	{
		j = (j + stream_[i] + key[i % key_size]) % _countof(stream_);
		std::swap(stream_[i], stream_[j]);
	}
	//跟常量再异或下
	const char kConst = 76;
	const auto kAver = _countof(stream_) / 4;
	assert(kAver > 0);
	const auto kTotal = kAver * 2;
	assert(kTotal <= _countof(stream_));
	for ( auto i = kAver;i != kTotal; ++i )
	{
		stream_[i] ^= kConst;
	}
	VMP_END;
}

void Secret_RC4::DoAlgo( const char* src, pt_dword src_size, char* dst )
{
	if ( !src || src_size <= 0 || !dst )
	{
		assert(false);
		return;
	}
	VMP_BEGIN;
	int i = 0, j = 0;
	int idx = 0;
	VMP_END;
	while ( idx != src_size )
	{
		i = (i + 1) % _countof(stream_);
		j = (j + stream_[i]) % _countof(stream_);
		std::swap(stream_[i], stream_[j]);
		int t = (stream_[i] + stream_[j]) % _countof(stream_);
		auto k = stream_[t];
		
		dst[idx] ^= k;
		++idx;
	}
}

void Secret_RC4::MixOldKey( const char* key, pt_dword key_size )
{
	for ( int i = 0; i != _countof(stream_); ++i )
	{
		stream_[i] ^= *(key + i % key_size);
	}
}

ISecretMaker::~ISecretMaker()
{

}

Secret_XorKey::Secret_XorKey() : key_("error")
{
}

void Secret_XorKey::SetKey( const char* key, pt_dword key_size )
{
	VMP_BEGIN_EX;
	if ( !key || key_size <= 0 )
	{
		assert(false);
		return;
	}
	FrameCheck::GetMe().TryCheck();
	key_.assign(key, key_size);
	auto aver = key_size / 4;
	aver = std::max((decltype(aver))1, aver);
	auto total = aver * 2;
	total = std::min(total, key_size);
	key_ += GenUuidStr(std::string(key + aver, key + total));
	VMP_END;
}

void Secret_XorKey::DoAlgo( const char* src, pt_dword src_size, char* dst )
{
	if ( !src || src_size <= 0 || !dst )
	{
		assert(false);
		return;
	}
	VMP_BEGIN_EX;
	const auto kKeySize = key_.size();
	if ( kKeySize <= 0 )
	{
		assert(false);
		return;
	}
	CopyMemory(dst, src, src_size);
	VMP_END;
	return;
	for ( pt_dword i = 0;i != src_size; ++i )
	{
		*(dst + i) = *(src + i) ^ key_[i % kKeySize];
	}
}

void Secret_DoSameAlgo::Encrypt( const char* buffer_src, pt_dword buffer_size, char* cryptograph )
{
	DoAlgo(buffer_src, buffer_size, cryptograph);
}

void Secret_DoSameAlgo::Decrypt( const char* cryptograph, pt_dword cryptograph_size, char* buffer_src )
{
	DoAlgo(cryptograph, cryptograph_size, buffer_src);
}

ICryptoStr::~ICryptoStr()
{
}

Crypto_Aes::Crypto_Aes( const std::string& str_key, Encoding enco ) : Crypto_Base(enco)
{
	if ( !str_key.empty() )
	{
		InitCipher(str_key);
		assert(cipher_);
	}
}

void Crypto_Aes::InitCipher( const std::string& str_key )
{
	VMP_BEGIN_EX;
	auto aes = (CipherT*)cipher_;
	if (aes)
		delete aes;
	FrameCheck::GetMe().TryCheck();
	using namespace Poco::Crypto;
	CipherFactory& cf = CipherFactory::defaultFactory();
	cipher_ = cf.createCipher(CipherKey("aes256", str_key));
	assert(cipher_);
	VMP_END;
}

Crypto_Base::~Crypto_Base()
{
	/*auto aes = (CipherT*)cipher_;
	if (aes)
		delete aes;*/
}

std::string Crypto_Base::Encrypt( const std::string& str_src )
{
	auto aes = (CipherT*)cipher_;
	if ( !aes )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "encrypt erro, src str:" << str_src;
		//assert(false);
		return kEmptyStr;
	}
	try{
		return aes->encryptString(str_src, (CipherT::Encoding)encoding_);
	}
	DO_CATCH_THE_EXCEPTION( const Poco::Exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << e.displayText();
	})
	DO_CATCH_THE_EXCEPTION(const std::exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << e.what();
	})
	return kEmptyStr;
}

std::string Crypto_Base::Decrypt( const std::string& str_crypto )
{
	auto aes = (CipherT*)cipher_;
	if ( !aes )
	{
		assert(false);
		return kEmptyStr;
	}
	try{
		return aes->decryptString(str_crypto, (CipherT::Encoding)encoding_);
	}
	DO_CATCH_THE_EXCEPTION(const Poco::Exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << e.displayText();
	})
	DO_CATCH_THE_EXCEPTION(const std::exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << e.what();
	})
	return kEmptyStr;
}

void Crypto_Base::SetKey( const std::string& str_key )
{
	InitCipher(str_key);
}

void Crypto_Base::SetKey( std::istream& istrm )
{
	InitCipher(istrm);
}

Crypto_Base::Crypto_Base( Encoding ecd )
{
	encoding_ = ecd;
	cipher_ = nullptr;

	static_assert(ENC_NONE == CipherT::ENC_NONE, "");
	static_assert(ENC_BASE64 == CipherT::ENC_BASE64, "");
	static_assert(ENC_BINHEX == CipherT::ENC_BINHEX, "");
	static_assert(ENC_BASE64_NO_LF == CipherT::ENC_BASE64_NO_LF, "");
	static_assert(ENC_BINHEX_NO_LF == CipherT::ENC_BINHEX_NO_LF, "");
}

void Crypto_Base::InitCipher( std::istream& istrm )
{
	std::string str_tmp;
	Poco::StreamCopier::copyToString(istrm, str_tmp);
	if (str_tmp.empty())
	{
		assert(false);
		return;
	}
	InitCipher(str_tmp);
}

ICryptoStr::Encoding Crypto_Base::SetEncoding( Encoding enco )
{
	std::swap(encoding_, enco);
	return enco;
}

Crypto_Rsa_Pub::Crypto_Rsa_Pub( std::istream& istrm, Encoding enco ) : Crypto_RsaBase(enco)
{
	InitCipher(istrm);
	assert(cipher_);
}

void Crypto_Rsa_Pub::InitCipher( const std::string& str_key )
{
}

void Crypto_Rsa_Pub::InitCipher( std::istream& istrm )
{
	VMP_BEGIN_EX;
	auto aes = (CipherT*)cipher_;
	if (aes)
		delete aes;
	FrameCheck::GetMe().TryCheck();
	using namespace Poco::Crypto;
	RSAKey pub_key(&istrm);
	cipher_ = CipherFactory::defaultFactory().createCipher(pub_key);
	assert(cipher_);
	VMP_END;
}

std::string Crypto_Rsa_Pub::Decrypt( const std::string& str_crypto )
{
	assert(false);
	return "error";
}

std::string Crypto_Rsa_Pri::Encrypt( const std::string& str_src )
{
	assert(false);
	return "error";
}

void Crypto_Rsa_Pri::InitCipher( const std::string& str_key )
{

}

void Crypto_Rsa_Pri::InitCipher( std::istream& istrm )
{
	VMP_BEGIN_EX;
	auto aes = (CipherT*)cipher_;
	if (aes)
		delete aes;
	FrameCheck::GetMe().TryCheck();
	using namespace Poco::Crypto;
	RSAKey pri_key(0, &istrm, passphrase_);
	cipher_ = CipherFactory::defaultFactory().createCipher(pri_key);
	assert(cipher_);
	VMP_END;
}

Crypto_Rsa_Pri::Crypto_Rsa_Pri( std::istream& istrm, const std::string& passphrase, Encoding enco ) : Crypto_RsaBase(enco), passphrase_(passphrase)
{
	InitCipher(istrm);
	assert(cipher_);
}

void CryptoStrCompose::SetKey( const std::string& str_key )
{
	VMP_BEGIN_EX;
	for ( auto crypto : cryptos_ )
	{
		assert(crypto);
		if (crypto)
			crypto->SetKey(str_key);
	}
	VMP_END;
}

void CryptoStrCompose::SetKey( std::istream& istrm )
{
	VMP_BEGIN_EX;
	for ( auto crypto : cryptos_ )
	{
		assert(crypto);
		if (crypto)
			crypto->SetKey(istrm);
	}
	VMP_END;
}

std::string CryptoStrCompose::Encrypt( const std::string& str_src )
{
	VMP_BEGIN_EX;
	std::string str_tmp = str_src;
	for ( auto crypto : cryptos_ )
	{
		assert(crypto);
		if (crypto)
			str_tmp = crypto->Encrypt(str_tmp);
	}
	return str_tmp;
	VMP_END;
}

std::string CryptoStrCompose::Decrypt( const std::string& str_crypto )
{
	VMP_BEGIN_EX;
	std::string str_tmp = str_crypto;
	for ( auto crypto : cryptos_ )
	{
		assert(crypto);
		if (crypto)
			str_tmp = crypto->Decrypt(str_tmp);
	}
	return str_tmp;
	VMP_END;
}

void CryptoStrCompose::AddCrypto( const SubCryptoT& crypto )
{
	assert(crypto);
	if ( !crypto )
		return;
	cryptos_.push_back(crypto);
}

ICryptoStr::Encoding CryptoStrCompose::SetEncoding( Encoding enco )
{
	VMP_BEGIN_EX;
	Encoding enco_res;
	for ( auto crypto : cryptos_ )
	{
		assert(crypto);
		if (crypto)
			enco_res = crypto->SetEncoding(enco);
	}
	return enco_res;
	VMP_END;
}

void CryptoStrCompose::AddCryptoToFirst( const SubCryptoT& crypto )
{
	assert(crypto);
	if ( !crypto )
		return;
	cryptos_.push_front(crypto);
}

void CryptoStrCompose::EraseTheCrypto( const SubCryptoT& crypto )
{
	auto& name_list = MakeNameListRef(cryptos_);
	name_list.Delete(crypto);
}

Crypto_RsaBase::Crypto_RsaBase( Encoding ecd ) : Crypto_Base(ecd)
{
}

bool Crypto_RsaInitor::Init()
{
	//这个函数里面不能加VMP，不知为何
	auto node = AS_Sys::GetMe().GetRoot();
	if (!node)
	{
		assert(false);
		return false;
	}
	node = node->FindChild("skey");
	if (!node)
	{
		assert(false);
		return false;
	}
	const auto& key_public = node->FindChild("public");
	if (!key_public)
	{
		assert(false);
		return false;
	}
	const auto& key_private = node->FindChild("private");
	if (!key_private)
	{
		assert(false);
		return false;
	}
	const auto& key = node->FindChild("key");
	if (!key)
	{
		assert(false);
		return false;
	}
	std::stringstream stm_pri;
	if (!key_private->Serialize((std::ostream&)stm_pri))
	{
		assert(false);
		return false;
	}
	std::stringstream stm_pub;
	if (!key_public->Serialize((std::ostream&)stm_pub))
	{
		assert(false);
		return false;
	}

	try{
		auto pPub = new Crypto_Rsa_Pub(stm_pub);
		auto pPri = new Crypto_Rsa_Pri(stm_pri, key->GetString());
		public_key_.reset(pPub);
		private_key_.reset(pPri);
	}
	DO_CATCH_THE_EXCEPTION(const Poco::Exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "can not generate rsa key\t" << e.displayText();
		return false;
	})
	DO_CATCH_ALL_EXCEPTION({
		assert(false);
		return false;
	})
	return true;
}

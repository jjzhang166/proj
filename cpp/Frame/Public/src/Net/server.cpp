#include "stdafx.h"
#include <Common/UsefulFunc.h>
#include "Net/server.h"
#include <boost/bind.hpp>
#include <Common/LogOut.h>
#include "protect/Protect.h"
#include <Common/CmnException.h>
#include <Net/PocoMore.h>
#include <CmnMix/Cmn_TemplateUtility.h>

using boost::asio::ip::tcp;

Session::Session( Server& server ) : server_(server)
{
	VMP_BEGIN_EX;
	if (VMP_IS_DEBUGGER())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "检测到调试器";
		throw CmnExitIoThreadException("s:sde");
	}
	if ( !VMP_CRC_VALID() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "crc is not valid";
		throw CmnExitIoThreadException("s:sce");
	}
	auto total_connects = IoConnectsMgrSingle::GetInstance();
	if (total_connects)
		AddSink(total_connects);
	AddSink(this);
	VMP_END;
}

void Session::start()
{
	VMP_BEGIN_EX;
	if (timer_check_valid_)
	{
		assert(false);
		Disconnect();
		return;
	}
	if ( !InitOnce() )
	{
		assert(false);
		Disconnect();
		return;
	}

	HandShake_Send();

	AsyncReadFullPackage();
	auto shared_this = SmartCast<session_ptr>(shared_from_this());
	timer_check_valid_ = CreateIoTimer(1 * 60 * 1000, true, [shared_this](IoTimerPtr&){
		//assert(shared_this->ObjValid());
		if (shared_this->ObjValid())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "server未收到secret_key_mix_信息，断开连接";
			shared_this->Disconnect();
		}		
	});
	VMP_END;
}

Session::~Session()
{

}

void Session::Disconnect()
{
	__super::Disconnect();
	if ( !socket_.is_open() )
		return;
	/*LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "close the socket";*/
	try
	{
		socket_.shutdown(tcp::socket::shutdown_both);
		socket_.close();
	}
	DO_CATCH_THE_EXCEPTION(const std::exception& e, {
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "close the socket occur exception : " << e.what();
	})
}

void Session::DoDisconnect()
{
	Disconnect();
}

bool Session::InitOnce()
{
	return true;
}

bool Session::DoConnectDone()
{
	return ConnectDone();
}

bool Session::SortSecretKey( pt_csz recv_key, pt_dword key_size )
{
	if (self_key_info_.empty())
	{
		assert(false);
		return false;
	}
	if ( !recv_key || key_size <= 0 )
	{
		assert(false);
		return false;
	}
	return OnSortSecretKey(recv_key, key_size, self_key_info_.c_str(), self_key_info_.size());
}

void Session::SetKey(const std::string& key)
{
	assert(!key.empty());
	assert(key_.empty());
	key_ = key;
}

const std::string& Session::GetKey() const
{
	return key_;
}

//////////////////////////////////////////////////////////////////////////
Server::Server( pt_word host_port )
	: acceptor_(GetIoService().GetImpl(), tcp::endpoint(tcp::v4(), host_port))
{
	GetIoService().post([this](){
		session_ptr new_session(this->CreateSession());
		this->acceptor_.async_accept(new_session->socket(),
			boost::bind(&Server::handle_accept, this, new_session,
			boost::asio::placeholders::error));
	});
}

void Server::handle_accept( session_ptr new_session, const boost::system::error_code& error )
{
	if (!error)
	{
		new_session->AddSink(this);
		io_connects_.insert(new_session.get());
		new_session->start();
		new_session.reset(CreateSession());
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&Server::handle_accept, this, new_session,
			boost::asio::placeholders::error));
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "handle accept, error msg:" << error.message();
	}
}


Session* Server::CreateSession()
{
	return new Session(*this);
}

session_ptr Server::FindSessionByKey(const std::string& key) const
{
	if (key.empty())
	{
		assert(false);
		return nullptr;
	}
	Session* ptr_s = nullptr;
	if (!FindConnectBy([&key, &ptr_s](IoConnectBase* connect){
		ptr_s = static_cast<Session*>(connect);
		assert(ptr_s);
		return ptr_s->GetKey() == key;
	}))
		return nullptr;
	return boost::static_pointer_cast<Session>(ptr_s->shared_from_this());
}

Session_Aes::Session_Aes( Server& server ) : Session(server)
{
	ac_pwd_machine_code_ = false;
}

bool Session_Aes::InitOnce()
{
	VMP_BEGIN_EX;
	auto& compo_recv = GetCompoRecv();
	secret_aes_recv_.reset(new Crypto_Aes(kEmptyStr));
	compo_recv.AddCrypto(secret_aes_recv_);

	auto& compo_send = GetCompoSend();
	secret_aes_send_.reset(new Crypto_Aes(kEmptyStr));
	compo_send.AddCrypto(secret_aes_send_);
	VMP_END;
	return true;
}

Session_Rsa::Session_Rsa( Server& server, const CryptoStrCompose::SubCryptoT& stm_pub, 
								 const CryptoStrCompose::SubCryptoT& stm_pri )
	: Session(server)
{
	stm_pri_ = stm_pri;
	stm_pub_ = stm_pub;
}

bool Session_Rsa::InitOnce()
{
	VMP_BEGIN_EX;
	auto& compo_send = GetCompoSend();
	/*secret_aes_send_.reset(new Crypto_Aes(kEmptyStr));
	compo_send.AddCrypto(secret_aes_send_);*/
	compo_send.AddCrypto(stm_pub_);

	auto& compo_recv = GetCompoRecv();
	compo_recv.AddCrypto(stm_pri_);
	/*secret_aes_recv_.reset(new Crypto_Aes(kEmptyStr));
	compo_recv.AddCrypto(secret_aes_recv_);*/

	VMP_END;
	return true;
}

Server_Aes_Rsa::Server_Aes_Rsa(pt_word host_port) : Server(host_port)
{
	VERIFY(rsa_key_.Init());
}

CryptoStrCompose::SubCryptoT& Server_Aes_Rsa::GetRsaPublicKey()
{
	return rsa_key_.public_key_;
}

CryptoStrCompose::SubCryptoT& Server_Aes_Rsa::GetRsaPrivateKey()
{
	return rsa_key_.private_key_;
}

bool Session_Aes::AddNewAesMixKey( const std::string& new_mix_key )
{
	if (new_mix_key.empty())
	{
		assert(false);
		return false;
	}
	auto& compo_send = GetCompoSend();
	auto key_new_send = CryptoStrCompose::SubCryptoT(new Crypto_Aes(kEmptyStr));
	compo_send.AddCrypto(key_new_send);

	auto& compo_recv = GetCompoRecv();
	auto key_new_recv = CryptoStrCompose::SubCryptoT(new Crypto_Aes(kEmptyStr));
	compo_recv.AddCryptoToFirst(key_new_recv);

	key_new_send->SetKey(new_mix_key);
	key_new_recv->SetKey(new_mix_key);
	return true;
}

bool Session_Aes::SetAcPwdMachineCodeMixKey( const std::string& new_mix_key )
{
	if (ac_pwd_machine_code_)
		return true;
	GetCompoSend().EraseTheCrypto(secret_aes_send_);
	secret_aes_send_.reset();
	GetCompoRecv().EraseTheCrypto(secret_aes_recv_);
	secret_aes_recv_.reset();
	ac_pwd_machine_code_ = AddNewAesMixKey(new_mix_key);
	return ac_pwd_machine_code_;
}

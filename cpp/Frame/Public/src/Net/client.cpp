#include "stdafx.h"
#include <Common/UsefulFunc.h>
#include "Net/client.h"
#include <boost/bind.hpp>
#include <Common/LogOut.h>
#include "protect/Protect.h"
#include <Common/CmnException.h>
#include <CmnMix/Cmn_TemplateUtility.h>

using boost::asio::ip::tcp;

Connection::Connection( const std::string& dest_ip, pt_word dest_port ) : dest_port_(dest_port), dest_ip_(dest_ip)
{
	VMP_BEGIN_EX;
	if (VMP_IS_DEBUGGER())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "检测到调试器";
		throw CmnExitIoThreadException("c:cde");
	}
	if ( !VMP_CRC_VALID() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "crc is not valid";
		throw CmnExitIoThreadException("c:cce");
	}

	auto total_connects = IoConnectsMgrSingle::GetInstance();
	if (total_connects)
		AddSink(total_connects);
	AddSink(this);	
	VMP_END;
}

void Connection::handle_connect( const boost::system::error_code& error )
{
	if ( !error )
	{
		HandShake_Send();

		bool res = false;
		BOOST_SCOPE_EXIT_ALL(&){
			if ( !res )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "PreConnect失败或套接字已关闭";
				OccurSomeError_();
			}
		};
		res = PreConnect();			
	}
	else
	{
		LOG_ENTER_FUNC;
		auto err_msg = error.message();
		LOG_O(Log_debug) << "handle connection, error msg:" << err_msg;
		BOOST_SCOPE_EXIT_ALL(&){
			Disconnect();
		};
		OnConnectFailed();
	}
}

void Connection::ConnectAsync()
{
	if ( !socket_.is_open() )
	{
		if (dest_ip_.empty())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "目标IP不能为空，链接失败1";
			OccurSomeError_();
			return;
		}
		auto shared_this = boost::static_pointer_cast<Connection>(shared_from_this());
		boost::system::error_code err;
		auto the_addr = boost::asio::ip::address().from_string(dest_ip_, err);
		if (err)
		{
			assert(false);
			handle_connect(err);
		}
		else
			socket_.async_connect(tcp::endpoint(the_addr, dest_port_),
			boost::bind(&Connection::handle_connect, shared_this,
			boost::asio::placeholders::error));
	}
}

void Connection::SetServerInfo( const std::string& dest_ip, pt_word dest_port )
{
	if (socket_.is_open())
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "连接已建立，不能再设置ip和端口了";
		return;
	}
	dest_ip_ = dest_ip;
	dest_port_ = dest_port;
}

bool Connection::PreConnect()
{
	//LOG_ENTER_FUNC;
	//LOG_O(Log_trace) << "begin pre connect";
	
	VMP_BEGIN_EX;
	if (timer_check_valid_)
	{
		assert(false);
		return false;
	}
	auto shared_this = SmartCast<boost::shared_ptr<Connection> >(shared_from_this());
	timer_check_valid_ = CreateIoTimer(3 * 60 * 1000, true, [shared_this](IoTimerPtr&){
		//assert(shared_this->ObjValid());
		if (shared_this->ObjValid())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "client未收到secret_key_mix_信息，断开连接";
			shared_this->Disconnect();
		}
	});

	AsyncReadFullPackage();
	VMP_END;
	//LOG_O(Log_trace) << "end pre connect";
	return true;
}

void Connection::OnConnectFailed()
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "连接失败，OnConnectFailed";
	connected_done_ = false;
	//CMN_ASSERT(false);
}

void Connection::Disconnect()
{
	connected_done_ = false;
	__super::Disconnect();
	if (!socket_.is_open())
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

void Connection::DoDisconnect()
{
	Disconnect();
}

bool Connection::Connect()
{
	if (socket_.is_open())
		return false;
	if (dest_ip_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "目标IP不能为空，链接失败2";
		OccurSomeError_();
		return false;
	}
	boost::system::error_code err;
	auto the_addr = boost::asio::ip::address().from_string(dest_ip_, err);
	if (err)
	{
		assert(false);
	}
	else
		socket_.connect(tcp::endpoint(the_addr, dest_port_), err);
	if (err)
	{
		LOG_O(Log_debug) << "handle connection2, error msg:" << err.message();
		//assert(false);
		OccurSomeError_();
		return false;
	}
	else
	{
		HandShake_Send();

		bool res = false;
		BOOST_SCOPE_EXIT_ALL(&){
			if ( !res )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "PreConnect2失败或套接字已关闭";
				OccurSomeError_();
			}
		};
		res = PreConnect();
		return res;
	}

	assert(false);
	return false;
}

void Connection::OccurSomeError_()
{
	OnConnectFailed();
	__super::OccurSomeError_();
}

bool Connection::WaitForConnectDone() const
{
	if (!WaitUntil(kToServerWaitTime, [this](){
		if (connected_done_)
			return true;
		if (!this->ObjValid())
			return true;
		return false;
	}))
		return false;
	return CheckOptionalBoolRes(connected_done_);
}

bool Connection::DoConnectDone()
{
	connected_done_ = ConnectDone();
	return *connected_done_;
}

bool Connection::SortSecretKey( pt_csz recv_key, pt_dword key_size )
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
	return OnSortSecretKey(self_key_info_.c_str(), self_key_info_.size(), recv_key, key_size);
}

Connection_Aes::Connection_Aes( const std::string& dest_ip, pt_word dest_port ) : ConnectionAesBase(dest_ip, dest_port)
{
	
}

bool Connection_Aes::PreConnect()
{
	/*VMP_BEGIN_EX;
	auto& compo_recv = GetCompoRecv();
	secret_aes_recv_ = CryptoStrCompose::SubCryptoT(new Crypto_Aes(kEmptyStr));
	compo_recv.AddCrypto(secret_aes_recv_);

	auto& compo_send = GetCompoSend();
	secret_aes_send_ = CryptoStrCompose::SubCryptoT(new Crypto_Aes(kEmptyStr));
	compo_send.AddCrypto(secret_aes_send_);
	VMP_END;*/
	return __super::PreConnect();
}


/*
bool ConnectionAesBase::SetMixSecretKey( const char* server_key, size_t server_key_size )
{
	VMP_BEGIN_EX;
	if ( !client_secret_key_mix_ )
	{
		assert(false);
		return false;
	}

	if ( client_secret_key_mix_->size() < 20 )
	{
		assert(false);
		return false;
	}
	if ( !secret_aes_send_ || !secret_aes_recv_ )
	{
		assert(false);
		return false;
	}
	std::string str_tmp(client_secret_key_mix_->begin(), client_secret_key_mix_->end());
	str_tmp.append(server_key, server_key_size);
	str_tmp = GenUuidStr(str_tmp);
	secret_aes_send_->SetKey(str_tmp);
	secret_aes_recv_->SetKey(str_tmp);
	VMP_END;
	return __super::SetMixSecretKey(server_key, server_key_size / 2);
}*/

Connection_Rsa::Connection_Rsa( const std::string& dest_ip, pt_word dest_port, const CryptoStrCompose::SubCryptoT& stm_pub, 
						 const CryptoStrCompose::SubCryptoT& stm_pri )
	: Connection(dest_ip, dest_port)
{
	stm_pub_ = stm_pub;
	stm_pri_ = stm_pri;
}

bool Connection_Rsa::PreConnect()
{
	VMP_BEGIN_EX;
	auto& compo_send = GetCompoSend();
	/*secret_aes_send_ = CryptoStrCompose::SubCryptoT(new Crypto_Aes(kEmptyStr));
	compo_send.AddCrypto(secret_aes_send_);*/
	compo_send.AddCrypto(stm_pub_);

	auto& compo_recv = GetCompoRecv();
	compo_recv.AddCrypto(stm_pri_);
	/*secret_aes_recv_ = CryptoStrCompose::SubCryptoT(new Crypto_Aes(kEmptyStr));
	compo_recv.AddCrypto(secret_aes_recv_);*/

	VMP_END;
	return __super::PreConnect();
}

ConnectionAesBase::ConnectionAesBase( const std::string& dest_ip, pt_word dest_port ) : Connection(dest_ip, dest_port)
{
	ac_pwd_machine_code_ = false;
}

bool ConnectionAesBase::AddNewAesMixKey( const std::string& new_mix_key )
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

bool ConnectionAesBase::SetAcPwdMachineCodeMixKey( const std::string& new_mix_key )
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

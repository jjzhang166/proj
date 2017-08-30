#include "stdafx.h"
#include "FuncObjs/FuncObj.h"
#include "Protocol/P_Defines.h"
#include <algorithm>
#include <Common/LogOut.h>
#include <boost/bind.hpp>
#include <Common/UsefulFunc.h>
#include <protect/Protect.h>
#include <boost/scope_exit.hpp>
//#include <boost/pool/singleton_pool.hpp>

FuncObj::FuncObj( FuncObj* parent, P_ObjId id ) : parent_(parent), id_(id)
{
	if (parent)
		parent->InnerAdd(*this);
}

bool FuncObj::Inner( P_MsgNT msg_num, const void* param, size_t param_size )
{
	return OperEncode(P_FuncObjOper::MakeOper(P_Operate_Inner, 
		msg_num), param, param_size, this);
}

bool FuncObj::UserMsg( P_MsgNT msg_num, const void* param, size_t param_size )
{
	return OperEncode(P_FuncObjOper::MakeOper(P_Operate_UserMsg, 
		msg_num), param, param_size, this);
}

bool FuncObj::OperEncode( P_FuncObjOper oper, const void* param, pt_dword param_size, FuncObj* msg_owner )
{
	if ( !parent_ )
		return false;
	assert(this != parent_);
	return parent_->OperEncode(oper, param, param_size, msg_owner ? msg_owner : this);
}

FuncObj* FuncObj::GetParent()
{
	return parent_;
}

P_ObjId FuncObj::GetId()
{
	return id_;
}

FuncObj* FuncObj::FindSubObj( P_ObjId id )
{
	return nullptr;
}

FuncObj::~FuncObj()
{
}

FuncObj* FuncObj::GetMosttop()
{
	FuncObj* res = this;
	FuncObj* parent_tmp = GetParent();
	while (parent_tmp)
	{
		res = parent_tmp;
		parent_tmp = parent_tmp->GetParent();
	}
	return res;
}

bool FuncObj::InnerAdd( FuncObj& func_obj )
{
	return false;
}

bool FuncObj::ObjValid() const
{
	return true;
}

bool FuncObj::DoDecode( P_FuncObjOper oper, void* param, pt_dword param_size )
{
	if ( P_Operate_Inner == oper.operate )
		return OnInner(oper.msg_num, param, param_size);
	else if ( P_Operate_UserMsg == oper.operate )
		return OnUserMsg(oper.msg_num, param, param_size);
	return false;
}

int FuncObj::GetAdditionalParamSize() const
{
	return 0;
}

void FuncObj::EncodeAddtionalParam( void* additional_param ) const
{
}

bool P_Main::Decode( FuncObj& func_obj )
{
	if ( obj_cnt < 1 )
	{
		//assert(false);
		return false;
	}

	auto objs_id = GetObjsId();
	if ( objs_id[0] != func_obj.GetId() )
	{
		//assert(false);
		return false;
	}

	//PreDecode(P_Main);

	auto left_len = msg_len - ((char*)objs_id - (char*)this - 
		offsetof(P_Main, msg_len) - sizeof(msg_len));
	auto obj = &func_obj;
	for ( pt_word id_idx = 1; obj && id_idx < obj_cnt;
		obj = obj->FindSubObj(objs_id[id_idx]), ++id_idx, left_len -= sizeof(P_ObjId) )
	{
		if ( left_len < sizeof(P_ObjId) )
			return false;
	}
	if ( !obj )
	{
		//assert(false);
		return false;
	}

	if (obj->DoDecode(obj_oper, GetParam(), ParamSize()))
		return true;
	//assert(false);
	return false;
}

bool P_Main::Encode( FuncObj& func_obj, P_FuncObjOper oper, const void* param, pt_dword param_size,
					pt_dword max_size )
{
#ifdef _DEBUG
	{
		auto mem_size = CalcNeedMemory(func_obj, oper, param, param_size);
		assert(mem_size <= max_size && mem_size >= sizeof(*this));
	}
#endif
	if ( sizeof(*this) > max_size )
	{
		assert(false);
		return false;
	}
	if ( 0 != param_size && !param )
	{
		assert(false);
		return false;
	}

	memset(this, 0, sizeof(*this));
	max_size -= sizeof(*this);

	auto objs_id = GetObjsId();
	assert(objs_id);
	for ( auto obj = &func_obj; obj; obj = obj->GetParent(), ++obj_cnt,
		max_size -= sizeof(P_ObjId) )
	{
		if ( max_size < sizeof(P_ObjId) )
		{
			assert(false);
			return false;
		}
		objs_id[obj_cnt] = obj->GetId();
	}

	if ( max_size < param_size )
	{
		assert(false);
		return false;
	}
	auto additonal_param = (char*)GetParam() + param_size;

	auto addition_size = func_obj.GetAdditionalParamSize();
	if ( 0 != addition_size )
	{
		max_size -= param_size;
		if ( (int)max_size < addition_size )
		{
			assert(false);
			return false;
		}
		func_obj.EncodeAddtionalParam(additonal_param);
	}

	memcpy(GetParam(), param, param_size);
	std::reverse(objs_id, objs_id + obj_cnt);
	obj_oper = oper;

	msg_len = additonal_param + addition_size - (
		(char*)this + offsetof(P_Main, msg_len) + sizeof(msg_len));

	//LOG_O(Log_error) << GetTotalLength();
	return true;
}

P_Main::P_Main()
{
	memset(this, 0, sizeof(*this));
	static_assert(offsetof(P_Main, msg_len) == 0, "msg_len必须是P_Main的第一个元素");
}

pt_dword P_Main::ParamSize()
{
	return msg_len - ((char*)GetParam() - 
		((char*)this + offsetof(P_Main, msg_len) + sizeof(msg_len)));
}

void* P_Main::GetParam()
{
	return GetObjsId() + obj_cnt;
}

P_ObjId* P_Main::GetObjsId()
{
	return reinterpret_cast<P_ObjId*>(reinterpret_cast<char*>(
		this) + sizeof(*this));
}

pt_dword P_Main::GetTotalLength()
{
	return msg_len + offsetof(P_Main, msg_len) + sizeof(msg_len);
}

pt_dword P_Main::CalcNeedMemory( FuncObj& func_obj, P_FuncObjOper, 
								  const void* param, pt_dword param_size )
{
	pt_dword mem_size = 0;

	if ( 0 == param_size || !param )
	{
		assert(false);
		goto LABLE_RET;
	}
	for ( auto obj = &func_obj; obj; obj = obj->GetParent() )
	{
		mem_size += sizeof(P_ObjId);
	}

	mem_size += param_size;
	mem_size += sizeof(P_Main);
	mem_size += func_obj.GetAdditionalParamSize();
LABLE_RET:
	return mem_size;
}

void* P_Main::GetFollowMsgAddr()
{
	return (char*)this + sizeof(msg_len);
}



P_FuncObjOper P_FuncObjOper::MakeOper( P_Operate oper, pt_word msg )
{
	P_FuncObjOper tmp;
	tmp.operate = oper;
	tmp.msg_num = msg;
	return tmp;
}

/*
class SingletonPool_PMain : public boost::singleton_pool<P_Main,
	sizeof(P_BufferT)>{
public:
	static P_Main* Construct(){
		auto buffer = (P_Main*)malloc();
		if ( !buffer )
		{
			assert(false);
			LOG_ENTER_FUNC;
			LOG_O(Log_fatal) << "singeton pool 分配内存失败";
			return nullptr;
		}
		return new (buffer) P_Main();
	}
	static void Destroy( P_Main* p_main ){
		if ( !p_main )
			return;
		p_main->~P_Main();
		free(p_main);
	}
};*/

bool FuncObj_Buffer::OperEncode( P_FuncObjOper oper, const void* param, pt_dword param_size, FuncObj* msg_owner )
{
	if ( !ObjValid() )
	{
		assert(false);
		return false;
	}
	auto mem_size = P_Main::CalcNeedMemory(msg_owner ? *msg_owner : *this, oper, param, param_size);
	if ( 0 == mem_size )
		return false;
	auto p_main = (P_Main*)malloc(mem_size);
	if ( !p_main )
	{
		assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "分配内存出错了";
		return false;
	}

	bool res = false;
	BOOST_SCOPE_EXIT_ALL(&, p_main){
		if ( !res )
			free(p_main);
	};
	res = p_main->Encode(msg_owner ? *msg_owner : *this, 
		oper, param, param_size, mem_size);
	if ( !res )
		return false;

	buffers_send_.push(std::move(P_Main_UseT(false, p_main)));
	return res;
}

FuncObj_Buffer::FuncObj_Buffer( FuncObj* parent, P_ObjId id )
	: FuncObjs(parent, id)
{
}

FuncObj_Buffer::~FuncObj_Buffer()
{
	while ( !buffers_send_.empty() )
	{
		free(buffers_send_.front().second);
		buffers_send_.pop();
	}
}

FuncObj* FuncObjs::FindSubObj( P_ObjId id )
{
	auto it = FindSubObjIt(id);
	return it == objs_.end() ? nullptr : *it;
}

FuncObjs::FuncObjs( FuncObj* parent, P_ObjId id ) : FuncObj(parent, id)
{

}

FuncObjs::~FuncObjs()
{
	/*for ( auto it = objs_.begin(), end = objs_.end(); it != end; ++it )
	{
		delete *it;
	}*/
}

bool FuncObjs::InnerDelete( FuncObj* subobj )
{
	if ( !subobj )
		return false;
	auto obj = FindSubObjIt(subobj->GetId());
	if ( obj != objs_.end() )
	{
		objs_.erase(obj);
		return true;
	}
	return false;
}

FuncObjs::ContT::iterator FuncObjs::FindSubObjIt( P_ObjId id )
{
	auto end = objs_.end();
	for ( auto it = objs_.begin(); it != end; ++it )
	{
		if ( id == (*it)->GetId() )
			return it;
	}
	return end;
}

bool FuncObjs::InnerAdd( FuncObj& func_obj )
{
	auto obj = FindSubObj(func_obj.GetId());
	if ( !obj && obj != &func_obj )
	{
		objs_.push_back(&func_obj);
		func_obj.parent_ = this;
		return true;
	}
	assert(false);
	return false;
}

FuncObjTop::FuncObjTop() : FuncObj_Buffer(nullptr, kFuncObjTemplateId_Account), socket_(GetIoService().GetImpl()), real_send_(*this)
{
	send_count_ = 0;
	recv_count_ = 0;
	send_one_ = &real_send_;
}

void FuncObjTop::AsyncReadFullPackage()
{
	/*auto p_main = SingletonPool_PMain::Construct();
	assert(p_main);
	socket_.async_read_some(boost::asio::buffer(p_main, sizeof(P_BufferT)),
		make_custom_alloc_handler(allocator_,
		boost::bind(&FuncObjTop::handle_read,
		shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)));
	buffers_recv_.push(p_main);*/

	buffers_recv_.push(std::move(RecvHelper()));
	auto& recv_helper = buffers_recv_.back();
	boost::asio::async_read(socket_,
		boost::asio::buffer(&recv_helper.msg_len_, sizeof(recv_helper.msg_len_)),
		make_custom_alloc_handler(allocator_,
		boost::bind(&FuncObjTop::HandleReadHeader,
		shared_from_this(),
		boost::asio::placeholders::error)));
}

void FuncObjTop::HandleReadBody( const boost::system::error_code& error, size_t bytes_transferred )
{
	if (error)
	{
		DoSomeReadError_(&error);
	}
	else
	{
		if ( 1 != buffers_recv_.size() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "HandleReadBody, 1 != buffers_recv_.size()" ;
			DoSomeReadError_(nullptr);
			return;
		}
		RecvHelper recv_helper = buffers_recv_.front();
		buffers_recv_.pop();
		AsyncReadFullPackage();

		//shared_this->DoDisconnect() 这样写存在问题的，比如在发包或收包的情况下去停止脚本，会造成断开连接的问题
		/*auto shared_this = shared_from_this();
		GetIoService().post([=](){
			bool res = false;
			BOOST_SCOPE_EXIT_ALL(&){
				if ( !res )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_debug) << "DecodeOnePackage error";
					shared_this->DoDisconnect();
				};
			};
			res = shared_this->DecodeOnePackage(*recv_helper.msg_, bytes_transferred + sizeof(recv_helper.msg_len_));
		});*/

		if ( !DecodeOnePackage(*recv_helper.msg_, bytes_transferred + sizeof(recv_helper.msg_len_)) )
			DoDisconnect();
	}
}

void FuncObjTop::handle_write( const boost::system::error_code& error )
{
	if (error)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "发包出错error msg:" << error.message();
		BOOST_SCOPE_EXIT_ALL(&){
			while ( !buffers_send_.empty() )
			{
				free(buffers_send_.front().second);
				buffers_send_.pop();
			}
		};
		DoDisconnect();
	}
	else
	{
		assert(!buffers_send_.empty());
		assert(buffers_send_.front().first);
		free(buffers_send_.front().second);
		buffers_send_.pop();

		assert(send_one_);
		send_one_->DoSendOne();
	}
}

bool FuncObjTop::OperEncode( P_FuncObjOper oper, const void* param, pt_dword param_size, FuncObj* msg_owner )
{
	LockGuard lock_guard(send_mutex_);
	if ( !__super::OperEncode(oper, param, param_size, msg_owner) )
		return false;

	if (buffers_send_.empty())
	{
		assert(false);
		return false;
	}

	assert(send_one_);
	send_one_->DoSendOne();
	return true;
}

void FuncObjTop::SendOnePackage( P_Main_UseT& package )
{
	assert(!package.first);
	package.first = true;

	const auto kSendMaxLen = package.second->GetTotalLength();
	boost::asio::async_write(socket_,
		boost::asio::buffer(package.second, kSendMaxLen),
		boost::asio::transfer_at_least(kSendMaxLen),
		make_custom_alloc_handler(allocator_,
		boost::bind(&FuncObjTop::handle_write,
		shared_from_this(),
		boost::asio::placeholders::error)));

	/*socket_.async_write_some(boost::asio::buffer(buffer_send_, 
		((P_Main*)buffer_send_)->GetTotalLength()),
		make_custom_alloc_handler(allocator_,
		boost::bind(&Session::handle_write,
		shared_from_this(),
		boost::asio::placeholders::error)));*/
}

bool FuncObjTop::DecodeOnePackage( P_Main& p_main, int bytes_transferred )
{
	const int kDecodeSize = p_main.GetTotalLength();
	//if ( bytes_transferred < kDecodeSize )
	if ( bytes_transferred != kDecodeSize )
	{
		assert(false);
		(void*)0;
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "bytes_transferred error";
	}
	else if ( p_main.msg_count_ != ++recv_count_ )
	{
		assert(false);
		(void*)0;
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "收包计数错误";
	}
	else
	{
		if (p_main.Decode(*this))
			return true;
		LOG_O(Log_debug) << "decode error";
	}

	DoSomeReadError_(nullptr);
	return false;
}

void FuncObjTop::HandleReadHeader( const boost::system::error_code& error )
{
	if (error)
	{
		DoSomeReadError_(&error);
	}
	else
	{
		if ( 1 != buffers_recv_.size() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "HandleReadHeader, 1 != buffers_recv_.size()";
			assert(false);
			DoSomeReadError_(nullptr);
			return;
		}
		auto& recv_helper = buffers_recv_.front();
		if (recv_helper.msg_)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "HandleReadHeader, recv_helper.msg_ == nullptr";
			assert(false);
			DoSomeReadError_(nullptr);
			return;
		}
		recv_helper.SetMsgPtr(RecvHelper::MakePMain(recv_helper.msg_len_));
		if ( !recv_helper.msg_ )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "HandleReadHeader, !recv_helper.msg_";
			assert(false);
			DoSomeReadError_(nullptr);
			return;
		}
		boost::asio::async_read(socket_,
			boost::asio::buffer(recv_helper.msg_->GetFollowMsgAddr(), recv_helper.msg_len_),
			make_custom_alloc_handler(allocator_,
			boost::bind(&FuncObjTop::HandleReadBody,
			shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)));
	}
}

boost::asio::ip::tcp::socket& FuncObjTop::socket()
{
	return socket_;
}

bool FuncObjTop::ObjValid() const
{
	return socket_.is_open();
}

void FuncObjTop::DoSomeReadError_( const boost::system::error_code* error )
{
	BOOST_SCOPE_EXIT_ALL(&){
		while ( !buffers_recv_.empty() )
			buffers_recv_.pop();
	};
	if (error)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "DoSomeReadError_, error msg:" << error->message();
	}
	DoDisconnect();
}

void FuncObjTop::BeginSendAnyPacket()
{
	if ( !buffers_send_.empty() )
	{
		auto& first_send = buffers_send_.front();
		if ( !first_send.first )
		{
			first_send.second->msg_count_ = ++send_count_;
			SendOnePackage(first_send);
		}
	}
}

void FuncObjTop_Secret::HandShake_Send()
{
	VMP_BEGIN_EX;
	self_key_info_ = RandomUniqueStrEx(32 * 2);
	auto key_str = self_key_info_.c_str();
	auto key_size = self_key_info_.size();
	Inner(kInnerMsg_SetSecrectKey, key_str, key_size);
	send_one_ = &delay_send_;
	VMP_END;
}

bool FuncObjTop_Secret::OnInner( P_MsgNT msg_num, void* param, size_t param_size )
{
	if ( kInnerMsg_SetSecrectKey == msg_num )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "收到kInnerMsg_SetSecrectKey";*/
		VMP_BEGIN_EX;
		if ( param_size != 64 )
		{
			assert(false);
			return false;
		}
		if (timer_check_valid_)
		{
			timer_check_valid_->Cancel();
			timer_check_valid_.reset();
		}
		else
		{
			assert(false);
			return false;
		}

		if ( !SortSecretKey(pt_csz(param), param_size) )
			return false;

		self_key_info_.clear();

		assert(send_one_);
		send_one_->DoSendOne();
		VMP_END;

		//不在这里加sink了，以防无法收到OnDisconnect的情况
		/*AddSink(this);
		auto total_connects = IoConnectsMgrSingle::GetInstance();
		if (total_connects)
			AddSink(total_connects);*/
		bool res = false;
		BOOST_SCOPE_EXIT_ALL(&){
			if ( !res )
			{
				LOG_O(Log_debug) << "ConnectDone return false";
				OccurSomeError_();
			}
		};
		res = DoConnectDone();
		if (res)
			LOG_O(Log_trace) << "连接成功";
		return res;
	}
	
	return __super::OnInner(msg_num, param, param_size);
}

FuncObj_Dispatcher::FuncObj_Dispatcher( FuncObj* parent, FuncObj* recv, FuncObj* send ) 
	: FuncObj(parent, kFuncObjTemplateId_Dispatcher)
{
	recv_ = recv;
	send_ = send;
	assert(recv);
	assert(send);
}

bool FuncObj_Dispatcher::OnInner( P_MsgNT msg_num, void* param, size_t param_size )
{
	return recv_->Inner(msg_num, param, param_size);
}

bool FuncObj_Dispatcher::OnUserMsg( P_MsgNT msg_num, void* param, size_t param_size )
{
	return recv_->UserMsg(msg_num, param, param_size);
}

bool FuncObj_Dispatcher::Inner( P_MsgNT msg_num, const void* param, size_t param_size )
{
	return send_->Inner(msg_num, param, param_size);
}

bool FuncObj_Dispatcher::UserMsg( P_MsgNT msg_num, const void* param, size_t param_size )
{
	return send_->UserMsg(msg_num, param, param_size);
}

FuncObj_Buffer::RecvHelper::RecvHelper()
{
	msg_len_ = 0;
}

void FuncObj_Buffer::RecvHelper::SetMsgPtr( P_Main* msg )
{
	if ( !msg )
	{
		assert(false);
		return;
	}
	msg_.reset(msg, []( P_Main* the_msg ){
		free(the_msg);
	});
}

P_Main* FuncObj_Buffer::RecvHelper::MakePMain( decltype(P_Main::msg_len) msg_len )
{
	assert(msg_len + sizeof(msg_len) >= sizeof(P_Main));
	auto msg = (P_Main*)malloc(msg_len + sizeof(msg_len));
	assert(msg);
	if ( !msg )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_fatal) << "分配内存出错";
		return nullptr;
	}
	msg->msg_len = msg_len;
	return msg;
}

void FuncObjTop_Secret::SendOnePackage( P_Main_UseT& package )
{
	VMP_BEGIN;
	auto follow_msg = (char*)package.second->GetFollowMsgAddr();
	secret_rc4_send_.Encrypt(follow_msg, package.second->msg_len, follow_msg);
	__super::SendOnePackage(package);
	VMP_END;
}

bool FuncObjTop_Secret::DecodeOnePackage( P_Main& p_main, int bytes_transferred )
{
	auto cryptograph = (char*)p_main.GetFollowMsgAddr();
	auto cryptograph_size = bytes_transferred - offsetof(P_Main, msg_len) - sizeof(p_main.msg_len);
	secret_rc4_recv_.Decrypt(cryptograph, cryptograph_size, cryptograph);
	return __super::DecodeOnePackage(p_main, bytes_transferred);
}

CryptoStrCompose& FuncObjTop_Secret::GetCompoSend()
{
	return compo_send_;
}

CryptoStrCompose& FuncObjTop_Secret::GetCompoRecv()
{
	return compo_recv_;
}

void FuncObjTop_Secret::OccurSomeError_()
{
	DoDisconnect();
}

bool FuncObjTop_Secret::OnSortSecretKey( pt_csz key_a, pt_dword key_size_a, pt_csz key_b, pt_dword key_size_b )
{
	VMP_BEGIN_EX;
	send_one_ = &real_send_;
	//rc4的recv和send此时必须设置为相同的密钥
	secret_rc4_recv_.SetKey(key_a, key_size_a);
	secret_rc4_recv_.MixOldKey(key_b, key_size_b);
	secret_rc4_send_.SetKey(key_a, key_size_a);
	secret_rc4_send_.MixOldKey(key_b, key_size_b);
	VMP_END;
	return true;
}

FuncObj_Game::FuncObj_Game( FuncObj* parent ) : FuncObj(parent, kFuncObjTemplateId_Game)
{

}

GLua::GLua( FuncObj* parent ) : FuncObj(parent, kFuncObjTemplateId_GLua)
{

}

FuncObjTop::SendImpl::SendImpl( FuncObjTop& obj ) : obj_(obj)
{

}

void FuncObjTop::SendImpl::DoSendOne()
{
	obj_.BeginSendAnyPacket();
}

void FuncObjTop::DelaySend::DoSendOne()
{

}

#include "stdafx.h"
#include <WgFacility/CmnRecvPacket.h>
#include <cassert>
#include <Common/UsefulFunc.h>

CmnRecvPacketMgr::stHandleMsgRes CmnRecvPacketMgr::HandleMsg(pt_pvoid def_ret_addr, pt_dword msg_id,
	pt_pcvoid msg_data, pt_dword msg_size)
{
	stHandleMsgRes handle_res = {
		def_ret_addr, 0, kHMR_Failed
	};
	if (!msg_data)
	{
		assert(false);
		return handle_res;
	}
	const auto it = cmd_map_.find(msg_id);
	if (it == cmd_map_.end())
	{
		handle_res.reason_ = kHMR_HasNotMsgId;
		return handle_res;
	}
	auto handler_impl = it->second.handler_msg_;
	handle_res.read_cnt_ = handler_impl->DoDirect(msg_data, msg_size);
	if (handle_res.read_cnt_ < 0)
	{
		handle_res.reason_ = kHMR_DoDirectFaild;
		auto new_addr = handler_impl->NewRetAddr();
		if (new_addr)
			def_ret_addr = (pt_pvoid)new_addr;
		handle_res.ret_addr_ = (pt_sz)def_ret_addr + it->second.ret_offset_;
		return handle_res;
	}
	auto new_addr = handler_impl->NewRetAddr();
	if (new_addr)
		def_ret_addr = (pt_pvoid)new_addr;
	handle_res.ret_addr_ = (pt_sz)def_ret_addr + it->second.ret_offset_;
	if (!handler_impl->IsPostToIoService())
	{
		handle_res.reason_ = kHMR_Succeed;
		return handle_res;
	}
	Cmn_CMDR_MsgBase::SelfPtr cmd(handler_impl->Clone());
	if (!cmd)
	{
		assert(false);
		return handle_res;
	}	
	GetIoService().post([cmd](){
		assert(cmd);
		cmd->OnReceived();
	});
	handle_res.reason_ = kHMR_Succeed;
	return handle_res;
}

void CmnRecvPacketMgr::AddCmdImpl(pt_dword msg_id, Cmn_CMDR_MsgBase* cmd_placeholder, pt_word ret_offset)
{
	if (!cmd_placeholder)
	{
		assert(false);
		return;
	}
	if (cmd_map_.find(msg_id) != cmd_map_.end())
	{
		assert(false);
		return;
	}
	stMsgInfo msg_info = {
		cmd_placeholder, ret_offset
	};
	cmd_map_[msg_id] = msg_info;
}

bool CmnRecvPacketMgr::Empty() const
{
	return cmd_map_.empty();
}

void Cmn_CMDR_MsgBase::OnReceived() const
{

}

Cmn_CMDR_MsgBase::~Cmn_CMDR_MsgBase()
{

}

pt_csz Cmn_CMDR_MsgBase::NewRetAddr() const
{
	return nullptr;
}

bool Cmn_CMDR_MsgBase::IsPostToIoService() const
{
	return true;
}

Cmn_CMDR_Serialize::Cmn_CMDR_Serialize()
{
	msg_data_ = nullptr;
	msg_size_ = 0;
	cur_read_size_ = 0;
}

void Cmn_CMDR_Serialize::SetMsg(pt_pcvoid msg_data, pt_dword msg_size)
{
	assert(!msg_data_);
	assert(msg_data);
	msg_data_ = msg_data;
	msg_size_ = msg_size;
}

bool Cmn_CMDR_Serialize::ReadCopyTo(pt_pvoid data, pt_dword size)
{
	assert(data && size >= 0);
	if (cur_read_size_ >= msg_size_)
	{
		assert(false);
		return false;
	}
	if (cur_read_size_ + size > msg_size_)
	{
		assert(false);
		return false;
	}
	if (!Cmnx_CpyMem(data, (pt_csz)msg_data_ + cur_read_size_, size))
	{
		assert(false);
		return false;
	}
	cur_read_size_ += size;
	return true;
}

pt_dword Cmn_CMDR_Serialize::LeftSize() const
{
	return msg_size_ - cur_read_size_;
}

pt_dword Cmn_CMDR_Serialize::GetMsgSize() const
{
	return msg_size_;
}

pt_dword Cmn_CMDR_Serialize::GetCurReadSize() const
{
	return cur_read_size_;
}

bool Cmn_CMDR_Serialize::Ignore(pt_dword size)
{
	if (cur_read_size_ + size > msg_size_)
	{
		assert(false);
		return false;
	}
	cur_read_size_ += size;
	return true;
}

void Cmn_CMDR_Serialize::IgnoreAll()
{
	Ignore(LeftSize());
}

CmnRecvMsgHandler* CmnRecvMsgHandler::instance_ = nullptr;

CmnRecvMsgHandler::~CmnRecvMsgHandler()
{
	assert(instance_);
	instance_ = nullptr;
}

CmnRecvMsgHandler::CmnRecvMsgHandler()
{
	assert(!instance_);
	instance_ = this;
}

CmnRecvMsgHandler* CmnRecvMsgHandler::GetInstance()
{
	assert(instance_);
	return instance_;
}

#include "stdafx.h"
#include "GRecvMsgMgr.h"
#include "GFuncCall.h"
#include <Common/LogOut.h>

bool g_enable_recv_output = false;
void GRecvMsgMgrBase::EnableOutput(bool enable)
{
	g_enable_recv_output = enable;
}

GRecvMsgMgrBase::stNewRetAddrInfo GRecvMsgMgrBase::HandleMsg( const CMDR_HeaderBase* cmd_header )
{
	stNewRetAddrInfo ret_info = { 0 };
	if ( !cmd_header )
	{
		assert(false);
		return ret_info;
	}
#ifdef _DEBUG
	if (g_enable_recv_output)
	{
		/*if (0x506 != cmd_header->msg_id_ && 0x2d1 != cmd_header->msg_id_ && 0x25e != cmd_header->msg_id_
			&& 0x504 != cmd_header->msg_id_ && 0x263 != cmd_header->msg_id_ && 0x264 != cmd_header->msg_id_
			&& 0x261 != cmd_header->msg_id_ && 0x262 != cmd_header->msg_id_ && 0x251 != cmd_header->msg_id_
			&& 0x29d != cmd_header->msg_id_ && 0x2ef != cmd_header->msg_id_ && 0x505 != cmd_header->msg_id_
			&& 0x509 != cmd_header->msg_id_ && 0x402 != cmd_header->msg_id_ && 0x299 != cmd_header->msg_id_)*/
			LOG_O(Log_trace) << "收到包，ID：" << std::hex << "0x" << cmd_header->msg_id_ << " 消息大小：" << std::dec << cmd_header->msg_size_;
	}
#endif
	const auto it = cmd_map_.find(cmd_header->msg_id_);
	if ( it == cmd_map_.end() )
		return ret_info;
	auto handler_impl = it->second.handler_msg_;
	auto res = handler_impl->DoDirect(cmd_header);
	if ( !res )
	{
		auto new_ret_offset = handler_impl->NewRetOffset();
		if (new_ret_offset)
			ret_info.ret_offset_ = *new_ret_offset;
		else
			ret_info.ret_offset_ = it->second.ret_offset_;
		ret_info.new_addr_ = handler_impl->NewRetAddr();
		return ret_info;
	}
	CMDR_MsgBase::SelfPtr cmd(handler_impl->Clone());
	if ( !cmd )
	{
		assert(false);
		return ret_info;
	}
	auto new_ret_offset = cmd->NewRetOffset();
	if (new_ret_offset)
		ret_info.ret_offset_ = *new_ret_offset;
	else
		ret_info.ret_offset_ = it->second.ret_offset_;
	ret_info.new_addr_ = cmd->NewRetAddr();
	GetIoService().post([cmd](){
		assert(cmd);
		cmd->OnReceived();
	});
	return ret_info;
}

void GRecvMsgMgrBase::AddCmd( WORD msg_id, CMDR_MsgBase* cmd_placeholder, WORD ret_offset )
{
	assert(msg_id > 0);
	const auto it = cmd_map_.find(msg_id);
	if ( it != cmd_map_.end() )
	{
		assert(false);
		return;
	}
	stMsgInfo msg_info = {
		cmd_placeholder, ret_offset
	};
	cmd_map_[msg_id] = msg_info;
}

GHookGameSendMsgMgr::GHookGameSendMsgMgr()
{
	//AddTemplateCmd<CMDR_Hook_Hit>();
	AddTemplateCmd<CMDR_Hook_SendMoveRoom>();
	AddTemplateCmd<CMDR_Hook_CreateRole>();
	AddTemplateCmd<CMDR_Hook_CancelDeal>();
}

GHookGameSendMsgMgr GHookGameSendMsgMgr::msg_mrg_;

GHookAllSendMsgMgr GHookAllSendMsgMgr::msg_mrg_;

GRecvMsgMgr GRecvMsgMgr::msg_mrg_;

GRecvMsgMgr::GRecvMsgMgr()
{
	//AddTemplateCmd<CMDR_MonsterDead>();
	AddTemplateCmd<CMDR_PlayerList>();
	AddTemplateCmd<CMDR_PlayerLine>();
	AddTemplateCmd<CMDR_OnCreatePlayer>();
	AddTemplateCmd<CMDR_OnEnterFb>();
	AddTemplateCmd<CMDR_OnFbInfo>();
	AddTemplateCmd<CMDR_OnRefreshMySellList>();
	AddTemplateCmd<CMDR_OnGainAuctionPageInfo>();
	AddTemplateCmd<CMDR_OnSafeTime>();
	AddTemplateCmd<CMDR_OnBuyAuction>();
	AddTemplateCmd<CMDR_OnAcceptMail>();
	AddTemplateCmd<CMDR_OnOpenNpc>();
	AddTemplateCmd<CMDR_OnSoldAuction>();
	AddTemplateCmd<CMDR_OnSoldItemToShop>();
	AddTemplateCmd<CMDR_OnCommitTaskFailed>();
	AddTemplateCmd<CMDR_OnDoNpcChat>();
	//AddTemplateCmd<CMDR_OnBuffStatus>();
	//AddTemplateCmd<CMDR_OnRoleHpState>();
	AddTemplateCmd<CMDR_OnSendMail>();
	AddTemplateCmd<CMDR_OnSendMail2>();
	AddTemplateCmd<CMDR_OnUseItemRet1>();
	AddTemplateCmd<CMDR_OnUseItemRet2>();
	AddTemplateCmd<CMDR_OnBuyItem>();
	AddTemplateCmd<CMDR_OnCollectRet>();
	AddTemplateCmd<CMDR_OnArrangeItemsRet>();
	AddTemplateCmd<CMDR_OnSendMoveRoomRet>();
	AddTemplateCmd<CMDR_OnAcceptTaskRet>();
	AddTemplateCmd<CMDR_OnGiveupTaskRet>();
	AddTemplateCmd<CMDR_OnEnterFbError>();
	//AddTemplateCmd<CMDR_RoleDead>();
	AddTemplateCmd<CMDR_OnDeleteRole>();
	AddTemplateCmd<CMDR_OnRequestDeal>();
	AddTemplateCmd<CMDR_OnRequestDealFail>();
	AddTemplateCmd<CMDR_OnRecvQuestDeal>();
	AddTemplateCmd<CMDR_OnCancelDeal>();
	AddTemplateCmd<CMDR_OnDealFail>();
	AddTemplateCmd<CMDR_OnMoveItemToDealTab>();
	AddTemplateCmd<CMDR_OnMoveDealGold>();
	AddTemplateCmd<CMDR_OnRecvDealStatus>();
	AddTemplateCmd<CMDR_OnRecvDealEnd>();
	AddTemplateCmd<CMDR_OnGetMailItemsInfo>();
}

DateTimeLocal GRecvMsgMgr::GetLastRecvTime()
{
	DateTimeLocal res;
	{
		Cmnx_Lockable lock(time_lock_);
		res = last_recv_;
	}
	return res;
}

void GRecvMsgMgr::RefreshLastRecvTime()
{
	Cmnx_Lockable lock(time_lock_);
	last_recv_ = DateTimeLocal();
}

GHookAllSendMsgMgr::GHookAllSendMsgMgr()
{

}

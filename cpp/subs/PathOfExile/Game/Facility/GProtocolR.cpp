#include "stdafx.h"
#include "GProtocolR.h"
#include <Common/LogOut.h>
#include <Common/UsefulFunc.h>
#include "GPlayer.h"
#include <Scmd.h>
#include <Script/LuaScript.h>

static GRecvMsgHandler g_recv_msg_handler;
GRecvMsgHandler& GRecvMsgHandler::GetMe()
{
	return g_recv_msg_handler;
}

void GRecvMsgHandler::RecvMsg(const OptionPromptInfo& msg)
{
	LOG_O(Log_debug) << "收到提示信息，prompt_id:" << (pt_pcvoid)msg->prompt_id_;
	if (msg->prompt_id_ == stCDR_PromptInfo::kItemDestroyed)
		return;
	else if (msg->prompt_id_ == stCDR_PromptInfo::kRoleDie)
	{
		/*LOG_O(Log_warning) << "角色死亡了";
		if (!LuasScriptEchoC::IsRunning())
			return;
		LuasScriptEchoC::TriggerEvent(kEventName_ReviveFront);
		GInterface::Revive(kRAP_AtRecord);*/
		return;
	}
	prompt_info_ = msg;
}

void GRecvMsgHandler::RecvMsg(const OptionPreGamePromptInfo& msg)
{
	LOG_O(Log_debug) << "收到PreGame提示信息，prompt_id:" << (pt_pcvoid)msg->prompt_id_;
	pre_game_prompt_info_ = msg;
}

void GRecvMsgHandler::RecvMsg(const OptionOnItem& msg)
{
	on_item_ = msg;
	//LOG_O(Log_debug) << "收到物品操作的包";
}

void GRecvMsgHandler::RecvMsg(const OptionNpcPanelInfo& msg)
{
	on_npc_panel_info_ = msg;
	//LOG_O(Log_debug) << "收到响应NpcPanel的包";
}

void GRecvMsgHandler::RecvMsg(const stCDGR_OnMove& msg)
{
	if (msg.flag_ >= 0 && msg.skill_id_ != cur_skill_id_)
		return;
	on_self_move_ = msg;
#ifdef _DEBUG
	if (msg.skill_id_ == 0x2909)
		return;
	//LOG_O(Log_debug) << "收到stCDGR_OnMove，技能ID：" << (pt_pcvoid)msg.skill_id_ << ",flag:" << msg.flag_ << ",ukn2_:" << msg.ukn2_;
#endif
}

void GRecvMsgHandler::RecvMsg(const OptionOnHpMp& msg)
{
	auto& msg_hp_mp = *msg;
	/*if (msg_hp_mp.IsDied())
	{
		LOG_O(Log_warning) << "角色死亡了";
		if (!LuasScriptEchoC::IsRunning())
			return;
		GInterface::Revive(kRAP_AtRecord);
		return;
	}*/
	MyLuaThrdData::AutoUseFlack(msg_hp_mp.value_type_);
}

void GRecvMsgHandler::BeginItemOper()
{
	prompt_info_.reset();
	on_item_.reset();
}

bool GRecvMsgHandler::GetItemOperRes() const
{
	if (on_item_)
		return true;
	if (prompt_info_)
		return false;
	assert(false);
	return false;
}

bool GRecvMsgHandler::WaitItemOperRes() const
{
	IoTimer::lock_guard locker;
	if (!TimerDo(50, 1000 * /* 60 **/ 2, [this](){
		if (prompt_info_ || on_item_)
			return true;
		return false;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

void GRecvMsgHandler::BeginNpcPanelOper()
{
	on_npc_panel_info_.reset();
}

bool GRecvMsgHandler::WaitNpcPanelRes() const
{
	if (!TimerDo(50, 1000 * 60 * 2, [this](){
		if (on_npc_panel_info_)
			return true;
		return false;
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GRecvMsgHandler::GetNpcPanelRes() const
{
	if (on_npc_panel_info_)
		return true;
	return false;
}

void GRecvMsgHandler::BeginMoveSkillOper(pt_word skill_id)
{
	on_self_move_.reset();
	cur_skill_id_ = skill_id;
	is_move_skill_ = skill_id == CD_Interface::GetSkillIdMove();
}

enUseSkillRes GRecvMsgHandler::WaitMoveSkillRes(pt_dword timeout, const FunctionBool& fn) const
{
	BOOST_SCOPE_EXIT_ALL(&){
		cur_skill_id_ = 0;
	};
	enUseSkillRes res = kUSR_Error;
	TimerDo(1, timeout, [this, &res, &fn](){
		if (!fn())
			return true;
		res = GetMoveSkillRes();
		if (kUSR_Error != res)
			return true;
		return false;
	});
	return res;
}

enUseSkillRes GRecvMsgHandler::GetMoveSkillRes() const
{
	if (!on_self_move_)
		return kUSR_Error;
	if (on_self_move_->flag_ < 0)
		return kUSR_DstNotValid;
	if (!is_move_skill_)
		return kUSR_Error;
	return kUSR_Succeed;
}

void GRecvMsgHandler::OnSelfMoveMsg(const stCDGR_OnMove& msg)
{
	stCDGR_OnMove msg_tmp = msg;
	GetIoService().post([this, msg_tmp](){
		RecvMsg(msg_tmp);
	});
}

GRecvMsgHandler::GRecvMsgHandler()
{
	CD_Interface::Hooks::SetRecvOnMoveMsg(this);
	cur_skill_id_ = 0;
	is_move_skill_ = false;
}

void GRecvMsgHandler::BeginCreateRole()
{
	prompt_info_.reset();
	pre_game_prompt_info_.reset();
}

bool GRecvMsgHandler::GetCreateRoleRes() const
{
	if (!pre_game_prompt_info_)
		return true;
	auto pre_id = pre_game_prompt_info_->prompt_id_;
	if (pre_id == stCDR_PreGamePromptInfo::kNameTooShort || 
		pre_id == stCDR_PreGamePromptInfo::kNameUsed || pre_id == stCDR_PreGamePromptInfo::kNameMinGan)
		return false;
	if (!prompt_info_)
		return true;
	auto id = prompt_info_->prompt_id_;
	if (id == stCDR_PromptInfo::kNameTooShort || id == stCDR_PromptInfo::kNameUsed || id == stCDR_PromptInfo::kNameMinGan)
		return false;
	return true;
}

void GRecvMsgHandler::AddCmds(CD_Interface::Hooks& hooks)
{
	hooks.AddCmdR<HandlerPromptInfo>();
	hooks.AddCmdR<HandlerOnItem>();
	hooks.AddCmdR<HandlerNpcPanelInfo>();
	//hooks.AddCmdR<HandlerUseSkillEffectAtPos>();
	hooks.AddCmdR<HandlerOnHpMp>();
	hooks.AddCmdPreR<HandlerPreGamePromptInfo>();
}

bool GR_HandlerOnHpMp::DirectMsg() const
{
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	if (msg_.info_.entity_id_ != gpm.GetGameId())
	{
		if (msg_.IsDied())
		{
			auto entity_id = msg_.info_.entity_id_;
			GetIoService().post([&gpm, entity_id](){
				gpm.OnMonsterDied(entity_id);
			});
		}
		return false;
	}
	return true;
}

#pragma once
/*
@author zhp
@date 2017/2/5 22:39
@purpose for recv/read
*/
#include <memory>
#include <boost/optional.hpp>
#include <CD_CommonFiles/Data/CD_ProtocolR.h>
#include <CD_CommonFiles/Data/CD_Interface.h>
#include "GConst.h"

class GRecvMsgHandler;

template<typename MsgT>
class GR_HandlerT : public CD_CMDR_Serialize<MsgT, GRecvMsgHandler>{
private:
	CMN_CLONE_IMPL(GR_HandlerT)
};

class GR_HandlerOnHpMp : public GR_HandlerT<stCDR_OnHpMp>{
private:
	bool DirectMsg() const override;
};

class GRecvMsgHandler : public CmnRecvMsgHandler, public CD_Interface::OnRecvMoveMsg{
public:
	typedef GR_HandlerT<stCDR_PromptInfo> HandlerPromptInfo;
	typedef HandlerPromptInfo::OptionMsgType OptionPromptInfo;
	typedef GR_HandlerT<stCDR_PreGamePromptInfo> HandlerPreGamePromptInfo;
	typedef HandlerPreGamePromptInfo::OptionMsgType OptionPreGamePromptInfo;
	typedef GR_HandlerT<stCDR_OnItem> HandlerOnItem;
	typedef HandlerOnItem::OptionMsgType OptionOnItem;
	typedef GR_HandlerT<stCDR_NpcPanelInfo> HandlerNpcPanelInfo;
	typedef HandlerNpcPanelInfo::OptionMsgType OptionNpcPanelInfo;
	typedef boost::optional<stCDGR_OnMove> OptionOnMove;
	typedef GR_HandlerOnHpMp HandlerOnHpMp;
	typedef HandlerOnHpMp::OptionMsgType OptionOnHpMp;

public:
	GRecvMsgHandler();
	void AddCmds(CD_Interface::Hooks& hooks);
	void RecvMsg(const OptionPromptInfo& msg);
	void RecvMsg(const OptionPreGamePromptInfo& msg);
	void RecvMsg(const OptionOnItem& msg);
	void RecvMsg(const OptionNpcPanelInfo& msg);
	void RecvMsg(const stCDGR_OnMove& msg);
	void RecvMsg(const OptionOnHpMp& msg);

public:
	void BeginItemOper();
	void BeginNpcPanelOper();
	void BeginMoveSkillOper(pt_word skill_id);
	void BeginCreateRole();

public:
	bool WaitItemOperRes() const;
	bool WaitNpcPanelRes() const;
	enUseSkillRes WaitMoveSkillRes(pt_dword timeout, const FunctionBool& fn) const;

public:
	bool GetItemOperRes() const;
	bool GetNpcPanelRes() const;
	enUseSkillRes GetMoveSkillRes() const;
	bool GetCreateRoleRes() const;

private:
	void OnSelfMoveMsg(const stCDGR_OnMove& msg) override;

public:
	static GRecvMsgHandler& GetMe();

public:
	OptionPromptInfo			prompt_info_;
	OptionPreGamePromptInfo		pre_game_prompt_info_;
	OptionOnItem				on_item_;
	OptionNpcPanelInfo			on_npc_panel_info_;
	OptionOnMove				on_self_move_;
	mutable pt_word				cur_skill_id_;
	bool						is_move_skill_;
};

#include "stdafx.h"
#include "CD_Structs.h"
#include "CD_UsefulFns.h"
#include <CommonX/CmnX_UsefulFunc.h>
#include "CD_FuncCall.h"
#include <cassert>
#include <CmnMix/Cmn_LogOut.h>
#include "CD_Data.h"
#include <stack>
#include <set>
#include <CommonX/CmnX_StaticConts.h>
#include <StdEx/RttiEx.h>
#include "CD_ProtocolR.h"
//#include <Game/Facility/GMap.h>

//////////////////////////////////////////////////////////////////////////
class _stCD_UiFilterVtbl : public stCD_UiObjBase::UiFilterBase{
	bool Filter(const stCD_UiObjBase& ui) const override{
		return ui.vtbl_ == vtbl_;
	}

public:
	_stCD_UiFilterVtbl(pt_pcvoid vtbl){
		assert(vtbl);
		vtbl_ = vtbl;
	}

private:
	pt_pcvoid		vtbl_;
};

class _stCD_UiFilterMultiVtbl : public stCD_UiObjBase::UiFilterBase{
public:
	template<typename... Args>
	_stCD_UiFilterMultiVtbl(Args... args){
		AddVtbl(args...);
	}
	template<typename... Args>
	void AddVtbl(pt_dword vtbl, Args... args){
		vtbls_.push_back(vtbl);
		AddVtbl(args...);
	}
	void AddVtbl(){}
	bool Filter(const stCD_UiObjBase& ui) const override{
		for (auto vtbl : vtbls_)
		{
			if (ui.vtbl_ == (pt_pcvoid)vtbl)
				return true;
		}
		return false;
	}

	CmnStaticVector<pt_dword, 10>	vtbls_;
};

class _stCD_UiFilterUiAddr : public stCD_UiObjBase::UiFilterBase{
	bool Filter(const stCD_UiObjBase& ui) const override{
		return &ui == ui_addr_;
	}

public:
	_stCD_UiFilterUiAddr(stCD_UiObjBase* ui_addr){
		assert(ui_addr);
		ui_addr_ = ui_addr;
	}

private:
	stCD_UiObjBase*		ui_addr_;
};

class _stCD_UiFilterAny : public stCD_UiObjBase::UiFilterBase{
	bool Filter(const stCD_UiObjBase& ui) const override{
		return true;
	}
};
//////////////////////////////////////////////////////////////////////////
pt_dword stCD_Socket::GetSendMsgSize() const
{
	assert(send_msg_total_len_ >= send_msg_offset_);
	return send_msg_total_len_ - send_msg_offset_;
}

pt_pcvoid stCD_Socket::GetSendMsgData() const
{
	return (pt_csz)send_msg_data_.data() + send_msg_offset_;
}

stCD_Encryptor* stCD_Socket::GetEncryptor() const
{
	if (IsUnValidPtr(encryptor_))
	{
		assert(false);
		return nullptr;
	}
	return encryptor_;
}

stCD_GameBase& stCD_GameBase::GetMe()
{
	return *(stCD_GameBase*)(void*)CD_EXE_DATA(g_a_game_base_);
}

void stCD_SceneBase::DebugInfo() const
{
	LOG_O(Log_info) << "addr:" << this << "\tflag:" << flag_ << "\tname_:" << name_;
}

void stCD_SceneAbovePart::DebugInfo() const
{
	LOG_O(Log_info) << "addr:" << this << "\tflag_:" << flag_ << "\tid:" << id_;
}

stCD_SceneHolder* stCD_GameBase::GetSceneHolder() const
{
	if (!game_state_cont_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	for (auto it = game_state_cont_.begin(); it < game_state_cont_.end(); ++it)
	{
		auto ptr = *it;
		if (IsUnValidPtr(ptr))
		{
			assert(false);
			return nullptr;
		}
		auto vtbl = ptr->vtbl_;
		if (IsUnValidPtr(vtbl))
		{
			assert(false);
			return nullptr;
		}
		if (vtbl->fn_ == (pt_pcvoid)CD_EXE_DATA(g_c_scene_holder_vtbl_fn_))
			return static_cast<stCD_SceneHolder*>(ptr);
	}
	return nullptr;
}

stCD_PlayerMe* stCD_GameBase::GetPlayer() const
{
	auto c = GetPlayerEntity();
	if (!c)
	{
		assert(false);
		return nullptr;
	}
	return c->GetPlayer();
}

stCD_GameLogic* stCD_GameBase::GetGameLogic() const
{
	auto state = GetInGameState();
	if (!state)
		return nullptr;
	if (IsUnValidPtr(state->logic_))
	{
		assert(false);
		return nullptr;
	}
	return state->logic_;
}

stCD_SceneInGameState* stCD_GameBase::GetInGameState() const
{
	auto ptr = GetSceneHolder();
	if (!ptr)
		return nullptr;
	return ptr->GetInGameState();
}

stCD_World* stCD_GameBase::GetWorld() const
{
	auto l = GetGameLogic();
	if (!l)
		return nullptr;
	if (IsUnValidPtr(l->world_))
	{
		assert(false);
		return nullptr;
	}
	return l->world_;
}

stCD_EntityPlayerMe* stCD_GameBase::GetPlayerEntity() const
{
	auto ptr = GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	auto in_game_state = ptr->GetInGameState();
	if (!in_game_state)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(in_game_state->logic_))
	{
		//assert(false);
		return nullptr;
	}
	auto holder = in_game_state->logic_->world_;
	if (IsUnValidPtr(holder))
	{
		assert(false);
		return nullptr;
	}
	auto res = holder->player_entity_;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

void stCD_SceneHolder::TraverseState(const IterFnT& fn) const
{
	if (!game_state_cont_.IsValid())
	{
		assert(false);
		return;
	}
	for (auto beg = game_state_cont_.begin(); beg < game_state_cont_.end(); ++beg)
	{
		auto back_part = beg->real_obj_;
		if (IsUnValidPtr(back_part))
			continue;
		auto the_name = back_part->name_;
		if (!CD_SafeName(the_name))
			continue;
		if (!fn(back_part))
			return;
	}
}

stCD_SceneBase* stCD_SceneHolder::GetStateByName(const std::wstring& state_name) const
{
	stCD_SceneBase* res = nullptr;
	TraverseState([&state_name, &res](stCD_SceneBase* p){
		if (p->name_.c_str() == state_name)
		{
			res = p;
			return false;
		}
		return true;
	});
	return res;
}

stCD_Socket* stCD_SceneHolder::GetSendPacket() const
{
	stCD_Socket* res = nullptr;
	TraverseState([&res](stCD_SceneBase* p){
		auto name = p->name_.c_str();
		if (name == CD_StateNamePreGameState)
		{
			auto pre_game_state = p->CastTo<stCD_ScenePreGameState>();
			if (!pre_game_state)
			{
				assert(false);
				return false;
			}
			auto holder = pre_game_state->logic_;
			if (IsUnValidPtr(holder))
			{
				assert(false);
				return false;
			}
			auto the_ptr = holder->send_packet_;
			if (IsUnValidPtr(the_ptr))
			{
				assert(false);
				return false;
			}
			res = the_ptr;
			return false;
		}
		else if (name == CD_StateNameInGameState)
		{
			auto in_game_state = p->CastTo<stCD_SceneInGameState>();
			if (!in_game_state)
			{
				assert(false);
				return false;
			}
			auto holder = in_game_state->logic_;
			if (IsUnValidPtr(holder))
			{
				assert(false);
				return false;
			}
			auto the_ptr = holder->send_packet_;
			if (IsUnValidPtr(the_ptr))
			{
				assert(false);
				return false;
			}
			res = the_ptr;
			return false;
		}
		return true;
	});
	return res;
}

stCD_SceneInGameState* stCD_SceneHolder::GetInGameState() const
{
	auto res = GetStateByName(CD_StateNameInGameState);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_SceneInGameState>();
}

stCD_ScenePreGameState* stCD_SceneHolder::GetPreGameState() const
{
	auto res = GetStateByName(CD_StateNamePreGameState);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_ScenePreGameState>();
}

void stCD_SceneHolder::DebugInfo() const
{
	LOG_O(Log_debug) << "SceneHolderAddr:" << this << ",game_state_cont_Addr:" << &game_state_cont_;
	TraverseState([](stCD_SceneBase* p){
		p->DebugInfo();
		return true;
	});
}

stCD_SceneBase* stCD_SceneHolder::GetTopState() const
{
	if (!game_state_cont_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	if (game_state_cont_.empty())
		return nullptr;
	auto it = game_state_cont_.rbegin();
	if (it == game_state_cont_.rend())
	{
		assert(false);
		return nullptr;
	}
	auto res = it->real_obj_;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

bool stCD_SceneHolder::TopStateSameName(const std::wstring& state_name) const
{
	auto top = GetTopState();
	if (!top)
	{
		assert(false);
		return false;
	}
	return state_name == CD_SafeName(top->name_);
}

bool stCD_SceneHolder::TopStateIsEscape() const
{
	return TopStateSameName(CD_StateNameEscapeState);
}

bool stCD_SceneHolder::TopStateIsAreaLoading() const
{
	return TopStateSameName(CD_StateNameAreaLoadingState);
}

stCD_SceneEscapeState* stCD_SceneHolder::GetEscapeState() const
{
	auto res = GetStateByName(CD_StateNameEscapeState);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_SceneEscapeState>();
}

stCD_SceneCreateCharacterState* stCD_SceneHolder::GetCreateCharacterState() const
{
	auto res = GetStateByName(CD_StateNameCreateCharacterState);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_SceneCreateCharacterState>();
}

stCD_SceneSelectCharacterState* stCD_SceneHolder::GetSelectCharacterState() const
{
	auto res = GetStateByName(CD_StateNameSelectCharacterState);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_SceneSelectCharacterState>();
}

stCD_SceneLoginState* stCD_SceneHolder::GetLoginState() const
{
	auto res = GetStateByName(CD_StateNameLoginState);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_SceneLoginState>();
}

stCD_Socket* stCD_SceneHolder::GetInGameStateSocket() const
{
	auto in_game = GetInGameState();
	if (!in_game)
		return nullptr;
	auto holder = in_game->logic_;
	if (IsUnValidPtr(holder))
	{
		//assert(false);
		return nullptr;
	}
	auto the_ptr = holder->send_packet_;
	if (IsUnValidPtr(the_ptr))
	{
		assert(false);
		return nullptr;
	}
	return the_ptr;
}

stCD_Socket* stCD_SceneHolder::GetPreGameStateSocket() const
{
	auto pre_game = GetPreGameState();
	if (!pre_game)
		return nullptr;
	auto holder = pre_game->logic_;
	if (IsUnValidPtr(holder))
	{
		//assert(false);
		return nullptr;
	}
	auto the_ptr = holder->send_packet_;
	if (IsUnValidPtr(the_ptr))
	{
		assert(false);
		return nullptr;
	}
	return the_ptr;
}

pt_pcvoid stCD_Socket::GetLastRecvOrgData() const
{
	/*
	$-A5     0078D710     53                         PUSH EBX                                 ; 这个函数是反序列化函数，与之对应的是发包时的BuildPacket函数
	$-A4     0078D711     8BD9                       MOV EBX, ECX
	$-A2     0078D713     80BB B2000000 00           CMP BYTE PTR DS:[EBX+0xB2], 0x0
	$-9B     0078D71A     0F85 EF000000              JNZ PathOfEx.0078D80F
	$-95     0078D720     8B8B C4000000              MOV ECX, DWORD PTR DS:[EBX+0xC4]
	$-8F     0078D726     8B83 D8000000              MOV EAX, DWORD PTR DS:[EBX+0xD8]
	$-89     0078D72C     2B83 D4000000              SUB EAX, DWORD PTR DS:[EBX+0xD4]
	$-83     0078D732     55                         PUSH EBP
	$-82     0078D733     8B6C24 10                  MOV EBP, DWORD PTR SS:[ESP+0x10]
	$-7E     0078D737     03CD                       ADD ECX, EBP
	$-7C     0078D739     3BC8                       CMP ECX, EAX
	$-7A     0078D73B     76 26                      JBE SHORT PathOfEx.0078D763
	$-78     0078D73D     51                         PUSH ECX
	$-77     0078D73E     8BCB                       MOV ECX, EBX
	$-75     0078D740     E8 FB070000                CALL PathOfEx.0078DF40
	$-70     0078D745     84C0                       TEST AL, AL
	$-6E     0078D747     75 1A                      JNZ SHORT PathOfEx.0078D763
	$-6C     0078D749     6A 02                      PUSH 0x2
	$-6A     0078D74B     FF33                       PUSH DWORD PTR DS:[EBX]
	$-68     0078D74D     FF15 0455D800              CALL NEAR DWORD PTR DS:[0xD85504]
	$-62     0078D753     5D                         POP EBP
	$-61     0078D754     66:C783 B0000000 0000      MOV WORD PTR DS:[EBX+0xB0], 0x0
	$-58     0078D75D     8BC3                       MOV EAX, EBX
	$-56     0078D75F     5B                         POP EBX
	$-55     0078D760     C2 0800                    RETN 0x8
	$-52     0078D763     8B83 C0000000              MOV EAX, DWORD PTR DS:[EBX+0xC0]
	$-4C     0078D769     8B8B C4000000              MOV ECX, DWORD PTR DS:[EBX+0xC4]
	$-46     0078D76F     2BC1                       SUB EAX, ECX
	$-44     0078D771     3BE8                       CMP EBP, EAX
	$-42     0078D773     76 0E                      JBE SHORT PathOfEx.0078D783
	$-40     0078D775     5D                         POP EBP
	$-3F     0078D776     C683 B2000000 01           MOV BYTE PTR DS:[EBX+0xB2], 0x1
	$-38     0078D77D     8BC3                       MOV EAX, EBX
	$-36     0078D77F     5B                         POP EBX
	$-35     0078D780     C2 0800                    RETN 0x8
	$-32     0078D783     8B83 BC000000              MOV EAX, DWORD PTR DS:[EBX+0xBC]
	$-2C     0078D789     33D2                       XOR EDX, EDX
	$-2A     0078D78B     03C1                       ADD EAX, ECX
	$-28     0078D78D     8B8B D8000000              MOV ECX, DWORD PTR DS:[EBX+0xD8]
	$-22     0078D793     2B8B D4000000              SUB ECX, DWORD PTR DS:[EBX+0xD4]
	$-1C     0078D799     F7F1                       DIV ECX                                  ; 取模运算。总收包长度%缓冲区长度
	$-1A     0078D79B     8B8B D8000000              MOV ECX, DWORD PTR DS:[EBX+0xD8]
	$-14     0078D7A1     2B8B D4000000              SUB ECX, DWORD PTR DS:[EBX+0xD4]
	$-E      0078D7A7     8D042A                     LEA EAX, DWORD PTR DS:[EDX+EBP]
	$-B      0078D7AA     3BC1                       CMP EAX, ECX
	$-9      0078D7AC     8B83 D4000000              MOV EAX, DWORD PTR DS:[EBX+0xD4]
	$-3      0078D7B2     77 1D                      JA SHORT PathOfEx.0078D7D1
	$-1      0078D7B4     55                         PUSH EBP                                 ; 长度
	$ ==>    0078D7B5     03C2                       ADD EAX, EDX                             ; 执行后eax=当前缓冲数据地址
	$+2      0078D7B7     50                         PUSH EAX                                 ; 原始缓冲数据地址
	$+3      0078D7B8     FF7424 14                  PUSH DWORD PTR SS:[ESP+0x14]             ; 目标数据存放地址
	$+7      0078D7BC     E8 8FE04F00                CALL PathOfEx.00C8B850                   ; copy到目标地址中
	$+C      0078D7C1     83C4 0C                    ADD ESP, 0xC
	$+F      0078D7C4     8BC3                       MOV EAX, EBX
	$+11     0078D7C6     01AB C4000000              ADD DWORD PTR DS:[EBX+0xC4], EBP
	$+17     0078D7CC     5D                         POP EBP
	$+18     0078D7CD     5B                         POP EBX
	$+19     0078D7CE     C2 0800                    RETN 0x8
	$+1C     0078D7D1     56                         PUSH ESI
	$+1D     0078D7D2     8B7424 10                  MOV ESI, DWORD PTR SS:[ESP+0x10]
	$+21     0078D7D6     03C2                       ADD EAX, EDX
	$+23     0078D7D8     57                         PUSH EDI
	$+24     0078D7D9     8BBB D8000000              MOV EDI, DWORD PTR DS:[EBX+0xD8]
	$+2A     0078D7DF     2BBB D4000000              SUB EDI, DWORD PTR DS:[EBX+0xD4]
	$+30     0078D7E5     2BFA                       SUB EDI, EDX
	$+32     0078D7E7     57                         PUSH EDI
	$+33     0078D7E8     50                         PUSH EAX
	$+34     0078D7E9     56                         PUSH ESI
	$+35     0078D7EA     E8 61E04F00                CALL PathOfEx.00C8B850
	$+3A     0078D7EF     8BC5                       MOV EAX, EBP
	$+3C     0078D7F1     2BC7                       SUB EAX, EDI
	$+3E     0078D7F3     50                         PUSH EAX
	$+3F     0078D7F4     FFB3 D4000000              PUSH DWORD PTR DS:[EBX+0xD4]
	$+45     0078D7FA     8D0437                     LEA EAX, DWORD PTR DS:[EDI+ESI]
	$+48     0078D7FD     50                         PUSH EAX
	$+49     0078D7FE     E8 4DE04F00                CALL PathOfEx.00C8B850
	$+4E     0078D803     83C4 18                    ADD ESP, 0x18
	$+51     0078D806     01AB C4000000              ADD DWORD PTR DS:[EBX+0xC4], EBP
	$+57     0078D80C     5F                         POP EDI
	$+58     0078D80D     5E                         POP ESI
	$+59     0078D80E     5D                         POP EBP
	$+5A     0078D80F     8BC3                       MOV EAX, EBX
	$+5C     0078D811     5B                         POP EBX
	$+5D     0078D812     C2 0800                    RETN 0x8
	*/
	auto msg_len = recv_msg_len_;
	if (msg_len <= 0)
	{
		assert(false);
		return nullptr;
	}
	auto buffer_size = recv_org_buffer_.size();
	if (buffer_size <= 0)
	{
		assert(false);
		return nullptr;
	}
	/*if (recv_total_len_ < msg_len)
	{
		assert(false);
		return nullptr;
	}*/
	auto res = (char*)recv_org_buffer_.data() + recv_total_len_ % buffer_size - msg_len;
	return res;
}

pt_pcvoid stCD_Socket::GetRecvOrgData() const
{
	auto msg_len = recv_msg_len_;
	if (msg_len <= 0)
	{
		assert(false);
		return nullptr;
	}
	auto buffer_size = recv_org_buffer_.size();
	if (buffer_size <= 0)
	{
		assert(false);
		return nullptr;
	}
	/*if (recv_total_len_ < msg_len)
	{
		assert(false);
		return nullptr;
	}*/
	auto res = (char*)recv_org_buffer_.data() + recv_total_len_ % buffer_size;
	return res;
}

void stCD_Socket::ClearSendingBuffer()
{
	send_msg_total_len_ = 0;
}

enCD_SocketType stCD_Socket::GetSocketType() const
{
	auto holder = stCD_GameBase::GetMe().GetSceneHolder();
	if (!holder)
		return kST_Ukn;
	if (holder->GetInGameStateSocket() == this) return kST_InGameState;
	else if (holder->GetPreGameStateSocket() == this) return kST_PreGameState;
	else if (holder->TopStateIsAreaLoading()) return kST_InGameState;
	assert(false);
	return kST_Ukn;
}

stCD_Player* stCD_EntityPlayer::GetPlayer() const
{
	return FindAttrComponent<stCD_Player>(CD_kAttrInfoPlayer);
}

stCD_Life* stCD_EntityPlayer::GetLife() const
{
	return FindAttrComponent<stCD_Life>(CD_kAttrInfoLife);
}

void stCD_EntityPlayer::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << "player info:";
	auto p = GetPlayer();
	if (p)
		p->DebugInfo();
	LOG_O(Log_debug) << "\n\n";
	LOG_O(Log_debug) << "life info:";
	auto l = GetLife();
	if (l)
		l->DebugInfo();
	/*LOG_O(Log_debug) << "\n\n";
	LOG_O(Log_debug) << "position info:";
	auto pos = GetPositioned();
	if (pos)
		pos->DebugInfo();*/
	LOG_O(Log_debug) << "\n\n";
	LOG_O(Log_debug) << "actor info:";
	auto actor = GetActor();
	if (actor)
		actor->DebugInfo();
	LOG_O(Log_debug) << "\n\n";
	LOG_O(Log_debug) << "pathfinding info:";
	auto path = GetPathfinding();
	if (path)
		path->DebugInfo();
}

stCD_Actor* stCD_EntityPlayer::GetActor() const
{
	return FindAttrComponent<stCD_Actor>(CD_kAttrInfoActor);
}

void stCD_Player::DebugInfo() const
{
	LOG_O(Log_debug) << "player addr:" << this << ",name:" << name_ << ",power:" << power_ << ",smart:" <<
		smart_ << ",brain:" << brain_ << ",level:" << level_ 
#ifndef VER3_
		<< ",difficulty:" << difficulty_
#endif
		<< ",cur_quest_info_cnt:" << cur_quest_cont_.size() << ",cur_quest_info_addr:" << &cur_quest_cont_;
	auto attr_db = GetAttrDb();
	if (attr_db)
	{
		LOG_O(Log_debug) << "profession, name:" << CD_SafeName(attr_db->profession_name_) << ",id:" << attr_db->id_;
	}
#ifndef VER3_
	assert(difficulty_ > 0 && difficulty_ < 4);
#endif
}

stCD_AttrDbPlayer* stCD_Player::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_->CastTo<stCD_AttrDbPlayer>();
}

int stCD_Player::TraCurQuestInfo(const Fn_TraCurQuestInfo& fn) const
{
	if (!cur_quest_cont_.IsValid())
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (auto v : cur_quest_cont_)
	{
		if (IsUnValidPtr(v))
		{
			assert(false);
			return cnt;
		}
		auto vtbl = v->vtbl_;
		if (IsUnValidPtr(vtbl))
		{
			assert(false);
			continue;
		}
		if ((pt_pcvoid)CD_EXE_DATA(g_c_player_quest_info_vfn_) != vtbl->fn_quest_info_tag_)
			continue;
		auto quest_info = v->CastTo<stCD_CurQuestInfo>();
		if (!quest_info->quest_state_.IsValid())
			continue;
		if (!quest_info->quest_.IsValid())
		{
			assert(false);
			continue;
		}
		++cnt;
		if (!fn(quest_info))
			break;
	}
	return cnt;
}

stCD_QuestStatesPair* stCD_Player::FindQuestState(stCD_QuestStatesPair& out_res, const stCD_QuestPair& quest) const
{
	return FindQuestStateImpl(out_res, quest,
#ifdef VER3_
		kMD_Normal
#else
		difficulty_
#endif
		);
}

stCD_QuestStatesPair* stCD_Player::FindQuestState(stCD_QuestStatesPair& out_res, const stCD_QuestPair& quest, enCD_kMapDifficulty difficult) const
{
	return FindQuestStateImpl(out_res, quest, difficult);
}

void stCD_Life::DebugInfo() const
{
	LOG_O(Log_debug) << "-----------------------------";
	LOG_O(Log_debug) << "life addr:" << this << ",hp:";
	hp_.DebugInfo();
	LOG_O(Log_debug) << "mp:";
	mp_.DebugInfo();
	LOG_O(Log_debug) << "shield:";
	shield_.DebugInfo();
	if (buff_cont_.IsValid())
	{
		for (auto v : buff_cont_)
		{
			if (IsUnValidPtr(v))
			{
				assert(false);
				continue;
			}
			v->DebugInfo();
		}
		LOG_O(Log_debug) << "Buff总数：" << buff_cont_.size();
	}
	LOG_O(Log_debug) << "-----------------------------";
}

void stCD_Actor::DebugInfo() const
{
	LOG_O(Log_debug) << "-----------------------------";
	LOG_O(Log_debug) << "ActorAddr:" << this;
	skill_mgr_.DebugInfo();
	LOG_O(Log_debug) << "-----------------------------";
}

const stCD_ActorSkillMgr& stCD_Actor::GetMySkillMgr() const
{
	return skill_mgr_;
}

bool stCD_Actor::DummyMsg_OnMove(bool dst_direct, const stCD_VecInt& dst_pos) const
{
	if (IsUnValidPtr(entity_))
	{
		assert(false);
		return false;
	}
	stCDGR_OnMove msg;
	auto player = entity_->CastTo<stCD_EntityPosBased>();
	if (!player)
	{
		assert(false);
		return false;
	}
	auto positioned = player->GetPositioned();
	if (!positioned)
	{
		assert(false);
		return false;
	}
	auto old_pos = positioned->pos_;
	msg.msg_id_ = kMIR_OnMove;
	msg.entity_.entity_id_ = player->id_;
	msg.skill_id_ = stCD_SomeSkillId::GetMe().skill_id_move_;
	msg.pos_dst_ = dst_pos;
	if (dst_direct)
		msg.pos_src_ = dst_pos;
	else
		msg.pos_src_ = old_pos;
	msg.ukn_ = 0x815f;
	if (!HandleMsg(msg))
	{
		assert(false);
		return false;
	}
	return true;
	/*if (dst_direct)
		return true;
	if (old_pos.Distance(dst_pos) < 10)
		return true;
	auto path_find = player->GetPathfinding();
	if (!path_find)
	{
		assert(false);
		return false;
	}
	return path_find->is_moving_;*/
}

pt_word stCD_Actor::IncUseSkillCnt()
{
#ifdef VER_GJ_
	assert(false);
#endif
	if (server_use_skill_cnt_ >= 0)
		return use_skill_cnt_;
	static const auto kMaxShort = std::numeric_limits<short>::max();
	use_skill_cnt_ = server_use_skill_cnt_ & kMaxShort;
	auto res = use_skill_cnt_ + 1;
	if (res == 0)
		res = 1;
	return res;

	/*++use_skill_cnt_;
	use_skill_cnt_ &= kMaxShort;
	if (use_skill_cnt_ == 0)
		use_skill_cnt_ = 1;
	return use_skill_cnt_;*/
}

bool stCD_Actor::CanUseSkill() const
{
	/*if (using_skill_timed_)
		return false;*/
	if (cur_using_skill_)
		return false;
	if (skill_left_time_)
		return false;
	return true;
}

void stCD_Pathfinding::DebugInfo() const
{
	LOG_O(Log_debug) << "pathfinding addr:" << this << ",is_moving:" << is_moving_ << ",is_3d:" << (int)is_3d_ 
		<< ",dst_pos:" << dst_pos_;
}

void stCD_World::DebugInfo() const
{
	LOG_O(Log_debug) << "world addr:" << this << ",world_area:";
	if (!world_area_.IsValid())
	{
		assert(false);
		return;
	}
	world_area_.tbl_item_->DebugInfo();
	map_data_.DebugInfo();
}

stCD_GameBaseWndInfo* stCD_GameBaseWndInfo::GetInstance()
{
	auto ptr = *(stCD_GameBaseWndInfo**)CD_EXE_DATA(g_a_game_base_wnd_info_);
	if (IsUnValidPtr(ptr))
	{
		//assert(false);
		return nullptr;
	}
	return ptr;
}

void stCD_GameBaseWndInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "addr:" << this << ",wnd_handle:" << wnd_handle_;
}

stCD_VecInt stCD_MapData::GetMapSize() const
{
	return stCD_VecInt(shrink_size_.x * CD_kMapRatio, shrink_size_.y * CD_kMapRatio);
}

enCD_MapDataFlag stCD_MapData::GetDstMapData(const stCD_VecInt& dst_pos, bool is_src_map_data) const
{
	pt_byte res = 0;
	SlowExecute([is_src_map_data, this, &res, &dst_pos](){
		pt_byte the_res;
		auto dst_pos_tmp = &dst_pos;
		auto int_is_src_map_data = (int)is_src_map_data;
		auto this_ptr_tmp = this;
		auto ptr_res = &res;
		auto call_addr = CD_EXE_DATA(g_c_get_map_data_byte_);
		__asm
		{
			//如果是0，则取当前游戏状态下的地图数据，如果是1，则取原始的地图数据
			push int_is_src_map_data
			push dst_pos_tmp
			mov ecx, this_ptr_tmp
			call call_addr
			mov the_res, al
		}
		*ptr_res = the_res;
	});	
	//LOG_O(Log_debug) << "map_data:" << (int)res << ", dst_pos:" << dst_pos;
	return (enCD_MapDataFlag)res;
}

stCD_MapData::Fn_GetDstMapData stCD_MapData::GetFn_GetDstMapData()
{
	auto addr = CD_EXE_DATA(g_c_get_map_data_byte_);
	return (Fn_GetDstMapData)*(Fn_GetDstMapData*)&addr;
}

void stCD_MapData::DebugInfo() const
{
	LOG_O(Log_debug) << "MapDataAddr:" << this
		<< ",shrink_size:" << shrink_size_;
}

void stCD_MapData::DebugModelsInfo(int radius, bool has_obj) const
{
	int cnt = 0;
	/*for (auto it = TerrainBegin(), it_end = TerrainEnd(); it < it_end; ++it)
	{
		if (has_obj)
		{
			if (it->terrain_objs_.empty())
				continue;
		}
		++cnt;
		it->DebugInfo();
	}*/
	stCD_VecInt pos_tmp;
	if (radius > 0)
	{
		auto entity = stCD_GameBase::GetMe().GetPlayerEntity();
		if (!entity)
		{
			assert(false);
			return;
		}
		auto positioned = entity->GetPositioned();
		if (!positioned)
		{
			assert(false);
			return;
		}
		stRectInt rc(positioned->pos_, positioned->pos_);
		rc.left_bottom_ -= radius;
		rc.right_top_ += radius;
		rc.left_bottom_ = Pos2ShrinkPos(rc.left_bottom_);
		rc.right_top_ = Pos2ShrinkPos(rc.right_top_);
		rc.RectifyAtRange(stCD_VecInt(0, 0), shrink_size_);
		for (pos_tmp.y = rc.left_bottom_.y; pos_tmp.y < rc.right_top_.y; ++pos_tmp.y)
		{
			auto h = pos_tmp.y * shrink_size_.x;
			for (pos_tmp.x = rc.left_bottom_.x; pos_tmp.x < rc.right_top_.x; ++pos_tmp.x)
			{
				int idx = h + pos_tmp.x;
				auto model = terrain_models_ + idx;
				if (has_obj)
				{
					if (model->terrain_objs_.empty())
						continue;
				}
				++cnt;
				model->DebugInfo(pos_tmp);
			}
		}
	}
	else
	{
		for (pos_tmp.y = 0; pos_tmp.y < shrink_size_.y; ++pos_tmp.y)
		{
			auto h = pos_tmp.y * shrink_size_.x;
			for (pos_tmp.x = 0; pos_tmp.x < shrink_size_.x; ++pos_tmp.x)
			{
				int idx = h + pos_tmp.x;
				auto model = terrain_models_ + idx;
				if (has_obj)
				{
					if (model->terrain_objs_.empty())
						continue;
				}
				++cnt;
				model->DebugInfo(pos_tmp);
			}
		}
	}	
	LOG_O(Log_debug) << "总数量：" << cnt;
}

stCD_MapDataTerrainModel* stCD_MapData::TerrainBegin() const
{
	if (IsUnValidPtr(terrain_models_))
	{
		assert(false);
		return nullptr;
	}
	return terrain_models_;
}

stCD_MapDataTerrainModel* stCD_MapData::TerrainEnd() const
{
	auto it = TerrainBegin();
	if (!it) return nullptr;
	return it + shrink_size_.y * shrink_size_.x;
}

stCD_VecInt stCD_MapData::Pos2ShrinkPos(const stCD_VecInt& pos)
{
	return stCD_VecInt((float)pos.x / (float)CD_kMapRatio, (float)pos.y / (float)CD_kMapRatio);
}

stCD_VecInt stCD_MapData::ShrinkPos2Pos(const stCD_VecInt& shrink_pos)
{
	return stCD_VecInt(shrink_pos.x * CD_kMapRatio, shrink_pos.y * CD_kMapRatio);
}

stCD_MapDataTerrainModel* stCD_MapData::GetTerrainFrom(stCD_EntityPosBased* entity, stCD_VecInt& out_pos) const
{
	if (IsUnValidPtr(entity))
	{
		assert(false);
		return nullptr;
	}
	auto positioned = entity->GetPositioned();
	if (!positioned)
	{
		assert(false);
		return nullptr;
	}
	assert(terrain_models_);
	stCD_VecInt pos_tmp;
	for (pos_tmp.y = 0; pos_tmp.y < shrink_size_.y; ++pos_tmp.y)
	{
		auto h = pos_tmp.y * shrink_size_.x;
		for (pos_tmp.x = 0; pos_tmp.x < shrink_size_.x; ++pos_tmp.x)
		{
			int idx = h + pos_tmp.x;
			auto model = terrain_models_ + idx;
			for (auto& obj_info : model->terrain_objs_)
			{
				if (obj_info.obj_positioned_ == positioned)
				{
					out_pos = pos_tmp;
					return model;
				}
			}
		}
	}
	return nullptr;
}

int stCD_MapData::GetSameModelPathCnt(pt_cwsz model_path) const
{
	assert(model_path);
	int cnt = 0;
	stCD_VecInt pos_tmp;
	for (pos_tmp.y = 0; pos_tmp.y < shrink_size_.y; ++pos_tmp.y)
	{
		auto h = pos_tmp.y * shrink_size_.x;
		for (pos_tmp.x = 0; pos_tmp.x < shrink_size_.x; ++pos_tmp.x)
		{
			int idx = h + pos_tmp.x;
			auto model = terrain_models_ + idx;
			if (!model->model_.IsValid())
			{
				//assert(false);
				continue;
			}
			if (StdEx::TStrCmp(model->model_.tbl_item_->model_path_, model_path) == 0)
				++cnt;
		}
	}
	return cnt;
}

void stCD_MapData::DbgPlayerPosModel() const
{
	auto entity = stCD_GameBase::GetMe().GetPlayerEntity();
	if (!entity)
	{
		assert(false);
		return;
	}
	auto positioned = entity->GetPositioned();
	if (!positioned)
	{
		assert(false);
		return;
	}
	auto shrink_pos = Pos2ShrinkPos(positioned->pos_);
	shrink_pos.RectifyAtRange(stVec2Int(), shrink_size_);
	auto idx = shrink_pos.y * shrink_size_.x + shrink_pos.x;
	if (IsUnValidPtr(terrain_models_))
	{
		assert(false);
		return;
	}
	auto model = terrain_models_ + idx;
	if (!model->model_.IsValid())
	{
		assert(false);
		return;
	}
	model->DebugInfo(shrink_pos);
}

stCD_MapData::MapDataAddrInfo stCD_MapData::GetDstMapDataAddr(const stCD_VecInt& dst_pos, bool is_src_map_data)
{
	MapDataAddrInfo res(nullptr, kMDF_EmptyPoint);
	if (dst_pos.x < 0)
	{
		assert(false);
		return res;
	}
	if (dst_pos.y < 0)
	{
		assert(false);
		return res;
	}
	if (dst_pos.x >= shrink_size_.x * CD_kMapRatio)
	{
		assert(false);
		return res;
	}
	if (dst_pos.y >= shrink_size_.y * CD_kMapRatio)
	{
		assert(false);
		return res;
	}
	stCD_MapDataVector* vec = nullptr;
	if (is_src_map_data)
		vec = &src_map_data_;
	else
		vec = &cur_map_data_;

	res.first = &(*vec)[dst_pos.y * shrink_width_ + dst_pos.x / 2];
	res.second = *res.first;
	if (pt_byte(dst_pos.x & 1) == 1)
	{
		res.second = enCD_MapDataFlag(res.second & kMDF_MaxMask);
	}
	else
	{
		res.second = enCD_MapDataFlag(res.second >> 4);
	}
	return res;
}

int stCD_ObjMgr::TraEntityObj(const Fn_TraObj& fn, bool is_obj) const
{
	const ObjContT* obj_cont;
	if (is_obj)
		obj_cont = &objs_;
	else
		obj_cont = &doodad_objs_;
	int cnt = 0;
	for (auto& v : *obj_cont)
	{
		if (IsUnValidPtr(v.second))
		{
			assert(false);
			break;
		}
		if (!v.second->IsValid())
		{
			assert(false);
			break;
		}
		++cnt;
		if (!fn(v.second))
			break;
	}
	return cnt;
}

stCD_Render* stCD_EntityNpc::GetRender() const
{
	return __super::GetRender();
}

void stCD_EntityNpc::DebugInfo() const
{
	__super::DebugInfo();
}

stCD_EntityAttrNPC* stCD_EntityNpc::GetNpc() const
{
	return __super::GetNpc();
}

stCD_EntityAttrClientNpc* stCD_EntityNpc::GetClientNpc() const
{
	return __super::GetClientNpc();
}

void stCD_Render::DebugInfo() const
{
	LOG_O(Log_debug) << "render_addr:" << this << ",name:" << CD_SafeName(name_);
	if (!IsUnValidPtr(attr_db_))
		attr_db_->DebugInfo();
	else
		assert(false);
}

stCD_Render* stCD_EntityMonster::GetRender() const
{
	return __super::GetRender();
}

stCD_Life* stCD_EntityMonster::GetLife() const
{
	return FindAttrComponent<stCD_Life>(CD_kAttrInfoLife);
}

void stCD_EntityMonster::DebugInfo() const
{
	__super::DebugInfo();
	auto l = GetLife();
	if (l)
		l->DebugInfo();
	auto m = GetDbMonster();
	if (m)
		m->DebugInfo();
	auto a = GetActor();
	if (a)
		a->DebugInfo();
}

stCD_EntityAttrDbMonster* stCD_EntityMonster::GetDbMonster() const
{
	auto res = FindAttrDb<stCD_EntityAttrDbMonster>(CD_kAttrInfoMonster);
	assert(res);
	return res;
}

bool stCD_EntityMonster::IsCalled() const
{
	auto db_monster = GetDbMonster();
	if (!db_monster)
	{
		assert(false);
		return false;
	}
	return db_monster->IsCalled();
}

stCD_EntityAttrMonster* stCD_EntityMonster::GetMonster() const
{
	return __super::GetMonster();
}

stCD_EntityAttrNPC* stCD_EntityMonster::GetNpc() const
{
	//第四章巨兽之腹怪有这个属性
	if (FindAttrInfo(CD_kAttrInfoTransitionable) && !FindAttrInfo(CD_kAttrInfoClientNpc))
		return nullptr;
	return __super::GetNpc();
}

stCD_EntityAttrClientNpc* stCD_EntityMonster::GetClientNpc() const
{
	return __super::GetClientNpc();
}

bool stCD_EntityMonster::CanHit() const
{
	if (IsCalled())
		return false;
	auto npc = GetNpc();
	if (npc && !npc->CanMoved())
		return false;
	if (GetDiesAfterTime())
		return false;
	auto life = GetLife();
	if (life)
	{
		const std::wstring* black_buff_names[] = {
			&CD_kBuffEnName_hidden_monster_disable_minions,
			&CD_kBuffEnName_visual_display_buff_not_hidden,
			//&CD_kBuffEnName_cannot_be_damaged,
		};
		for (auto& buff : life->buff_cont_)
		{
			auto buff_name = buff->GetBuffEnName();
			if (!buff_name)
			{
				assert(false);
				continue;
			}
			int empty_cnt = 0;
			for (auto& black_name : black_buff_names)
			{
				if (!black_name)
				{
					++empty_cnt;
					continue;
				}
				if (*black_name == buff_name)
				{
					++empty_cnt;
					black_name = nullptr;
				}
			}
			if (empty_cnt == _countof(black_buff_names))
				return false;
		}
	}
	//某个怪需要先打开对象 才会被激活
	auto attr_transiable = GetTransitionable();
	if (attr_transiable && !attr_transiable->IsOpened())
	{
		auto game_mon = GetRender();
		if (IsUnValidPtr(game_mon))
		{
			assert(false);
			return false;
		}

		if (!CD_SafeName(game_mon->name_))
		{
			assert(false);
			return false;
		}
		for (auto& v : kMonTransitionableName)
		{
			if (game_mon->name_ == v)
				return false;
		}
	}
	return true;
}

bool stCD_EntityMonster::NeedVisitNpc() const
{
	auto client_npc = GetClientNpc();
	if (!client_npc)
		return false;
	return client_npc->NeedVisit();
}

void stCD_EntityWaypoint::DebugInfo() const
{
	__super::DebugInfo();
}

stCD_Render* stCD_EntityWaypoint::GetRender() const
{
	return __super::GetRender();
}

void stCD_EntityAreaTransition::DebugInfo() const
{
	__super::DebugInfo();
}

stCD_Render* stCD_EntityAreaTransition::GetRender() const
{
	return __super::GetRender();
}

stCD_EntityAttrAreaTransition* stCD_EntityAreaTransition::GetAreaTransition() const
{
	return FindAttrComponent<stCD_EntityAttrAreaTransition>(CD_kAttrInfoAreaTransition);
}

stCD_PlayerMe* stCD_EntityPlayerMe::GetPlayer() const
{
	auto p = __super::GetPlayer();
	if (!p)
		return nullptr;
	return p->CastTo<stCD_PlayerMe>();
}

int stCD_InventoryMgr::TraTotalItems(const Fn_TraItem& fn) const
{
	if (!bag_item_cont_.IsValid())
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (auto bag_cate_it = bag_item_cont_.begin(); bag_cate_it < bag_item_cont_.end(); ++bag_cate_it)
	{
		auto bag = bag_cate_it->bag_info_.real_obj_;
		if (IsUnValidPtr(bag))
			continue;
		if (!bag->items_.IsValid())
			continue;
		/*bag_cate_it->DebugInfo();
		bag->DebugInfo();*/
		cnt += bag->TraItems(fn);
	}
	return cnt;
}

stCD_Inventory* stCD_InventoryMgr::GetMainBag() const
{
	return GetInventory(kIT_MainBag);
}

stCD_Inventory* stCD_InventoryMgr::GetInventory(enCD_InventoryType type) const
{
	assert(type > 0 && type < 50);
	auto idx = type - 1;
	if (idx >= (int)bag_item_cont_.size())
	{
		assert(false);
		return nullptr;
	}
	auto& v = bag_item_cont_[idx];
	if (IsUnValidPtr(v.bag_info_.real_obj_))
		return nullptr;
	assert(v.bag_type_ == type);
	//v.DebugInfo();
	return v.bag_info_.real_obj_;
}

int stCD_InventoryMgr::TraBodyItems(const Fn_TraItem& fn, bool include_flask, bool include_hide_weapon) const
{
	int idx_end;
	if (include_flask)
		idx_end = kIT_Flasks;
	else
		idx_end = kIT_Belt;
	int cnt = 0;
	for (auto idx = (int)kIT_BodyArmour; idx <= idx_end; ++idx)
	{
		auto inventory = GetInventory((enCD_InventoryType)idx);
		if (!inventory)
		{
			assert(false);
			continue;
		}
		cnt += inventory->TraItems(fn);
	}
	if (!include_hide_weapon)
		return cnt;
	for (auto idx = (int)kIT_WeaponLeftHide; idx <= kIT_WeaponRightHide; ++idx)
	{
		auto inventory = GetInventory((enCD_InventoryType)idx);
		if (!inventory)
		{
			assert(false);
			continue;
		}
		cnt += inventory->TraItems(fn);
	}
	return cnt;
}

stCD_EntityItemBased* stCD_InventoryMgr::GetFirstItem(enCD_InventoryType type) const
{
	auto res = GetInventory(type);
	if (!res)
		return nullptr;
	return res->GetFirstItem();
}

int stCD_InventoryMgr::TraBodyEquipsGem(const Fn_TraEquipsInsideGem& fn, bool include_hide_weapon) const
{
	int cnt = 0;
	for (auto idx = (int)kIT_BodyArmour; idx <= kIT_Belt; ++idx)
	{
		auto inventory = GetInventory((enCD_InventoryType)idx);
		if (!inventory)
		{
			assert(false);
			continue;
		}
		cnt += inventory->TraEquipsInsideGem(fn);
	}
	if (!include_hide_weapon)
		return cnt;
	for (auto idx = (int)kIT_WeaponLeftHide; idx <= kIT_WeaponRightHide; ++idx)
	{
		auto inventory = GetInventory((enCD_InventoryType)idx);
		if (!inventory)
		{
			assert(false);
			continue;
		}
		cnt += inventory->TraEquipsInsideGem(fn);
	}
	return cnt;
}

int stCD_InventoryMgr::TraBodyGemSkill(const Fn_TraGemSkill& fn, bool active_skill, bool passive_skill, bool include_hide_weapon) const
{
	assert(active_skill || passive_skill);
	int cnt = 0;
	int the_flag = kTGSF_Inside;
	if (active_skill)
		the_flag |= kTGSF_ActiveSkill;
	if (passive_skill)
		the_flag |= kTGSF_PassiveSkill;

	for (auto idx = (int)kIT_BodyArmour; idx <= kIT_Belt; ++idx)
	{
		auto inventory = GetInventory((enCD_InventoryType)idx);
		if (!inventory)
		{
			assert(false);
			continue;
		}
		cnt += inventory->TraGemSkill(fn, (enCD_TraGemSkillFlag)the_flag);
	}
	if (!include_hide_weapon)
		return cnt;
	for (auto idx = (int)kIT_WeaponLeftHide; idx <= kIT_WeaponRightHide; ++idx)
	{
		auto inventory = GetInventory((enCD_InventoryType)idx);
		if (!inventory)
		{
			assert(false);
			continue;
		}
		cnt += inventory->TraGemSkill(fn, (enCD_TraGemSkillFlag)the_flag);
	}
	return cnt;
}

int stCD_InventoryMgr::TraInventory(const Fn_TraInventory& fn) const
{
	int cnt = 0;
	for (auto& v : bag_item_cont_)
	{
		if (v.bag_type_ > 1000)
		{
			assert(false);
			break;
		}
		if (IsUnValidPtr(v.bag_info_.real_obj_))
		{
			assert(false);
			break;
		}
		++cnt;
		if (!fn(v.bag_type_, v.bag_info_.real_obj_))
			break;
	}
	return cnt;
}

int stCD_InventoryMgr::TraInventory(const FilterInventoryType& filter, const Fn_TraInventory& fn) const
{
	int cnt = 0;
	for (auto& v : bag_item_cont_)
	{
		if (v.bag_type_ > 1000)
		{
			assert(false);
			break;
		}
		if (IsUnValidPtr(v.bag_info_.real_obj_))
		{
			assert(false);
			break;
		}
		if (!filter.DoFilt(v.bag_type_))
			continue;
		++cnt;
		if (!fn(v.bag_type_, v.bag_info_.real_obj_))
			break;
	}
	return cnt;
}

void stCD_InventoryCate::DebugInfo() const
{
	LOG_O(Log_debug) << "BagCateAddr:" << this << ",bag_type:" << bag_type_ << ",flag:" << flag_;
	if (!IsUnValidPtr(bag_info_.above_part_))
	{
		bag_info_.above_part_->DebugInfo();
	}
}

void stCD_BagAbovePart::DebugInfo() const
{
	LOG_O(Log_debug) << "BagAbovePartAddr:" << this << ",ukn1:" << ukn1_ << ",ukn2:" << ukn2_;
}

int stCD_Inventory::TraItems(const Fn_TraItem& fn) const
{
	if (!items_.IsValid())
		return 0;
	auto item_size = items_.size();
	assert((int)item_size < CD_kBagItemArrayMaxSize);
	CmnStaticVector<bool, CD_kBagItemArrayMaxSize> item_array(item_size);
	pt_dword cnt = 0;
	for (auto item_holder_it = items_.begin(); item_holder_it < items_.end(); ++item_holder_it)
	{
		auto item_grid = *item_holder_it;
		if (IsUnValidPtr(item_grid))
			continue;
		if (IsUnValidPtr(item_grid->item_entity_))
			continue;
		int cursor = 0;
		while (true)
		{
			auto array_idx = item_grid->idx_info_.TraItemGridIdx(cursor, grid_width_);
			if (array_idx < 0)
				break;
			assert(array_idx < (int)item_size);
			if (item_array[array_idx])
				goto LABEL_CONTINU;
			item_array[array_idx] = true;
		}
		++cnt;
		if (!fn(item_grid, item_grid->item_entity_))
			return cnt;
#ifndef _DEBUG
		if (cnt >= item_cnt_)
			break;
#endif
	LABEL_CONTINU:
		continue;
	}
	return cnt;
}

void stCD_Inventory::DebugInfo(bool is_debug_items) const
{
	LOG_O(Log_debug) << "InventoryAddr:" << this << ",grid_width:" << grid_width_ << ",grid_height:" << grid_height_;
	if (!is_debug_items)
		return;
	auto cnt = TraItems([](stCD_ItemGridsInfo* item_grid_info, stCD_EntityItemBased* entity){
		item_grid_info->DebugInfo();
		entity->DebugInfo();
		LOG_O(Log_debug) << "物品实体对象遍历结束\n";
		return true;
	});
	LOG_O(Log_debug) << "物品总数：" << cnt;
}

stCD_EntityItemBased* stCD_Inventory::GetFirstItem() const
{
	stCD_EntityItemBased* res = nullptr;
	TraItems([&res](stCD_ItemGridsInfo*, stCD_EntityItemBased* item){
		res = item;
		return false;
	});
	return res;
}

int stCD_Inventory::TraEquipsInsideGem(const Fn_TraEquipsInsideGem& fn) const
{
	int cnt = 0;
	TraItems([&fn, &cnt](stCD_ItemGridsInfo*, stCD_EntityItemBased* equip){
		auto s = equip->GetAttrSockets();
		if (!s)
			return true;
		s->TraInsideGem([equip, &fn, &cnt](stCD_EntityItemBased* gem){
			if (fn(equip, gem))
			{
				++cnt;
				return true;
			}
			return false;
		});
		return true;
	});
	return cnt;
}

int stCD_Inventory::TraSkillGems(const Fn_TraSkillGem& fn) const
{
	int cnt = 0;
	TraItems([&fn, &cnt](stCD_ItemGridsInfo*, stCD_EntityItemBased* gem){
		if (!gem->IsSkillGem())
			return true;
		++cnt;
		if (!fn(gem))
			return false;
		return true;
	});
	return cnt;
}

int stCD_Inventory::TraGemSkill(const Fn_TraGemSkill& fn, enCD_TraGemSkillFlag flag) const
{
	assert((flag & kTGSF_Gem) || (flag & kTGSF_Inside));
	assert((flag & kTGSF_ActiveSkill) || (flag & kTGSF_PassiveSkill));
	int cnt = 0;
	TraItems([&fn, &cnt, flag](stCD_ItemGridsInfo*, stCD_EntityItemBased* equip){
		if (flag & kTGSF_Gem)
		{
			auto db_gem = equip->GetAttrDbSkillGem();
			if (db_gem)
			{
				auto active_skill = db_gem->GetActiveSkill();
				if ((flag & kTGSF_ActiveSkill) && active_skill)
				{
					++cnt;
					if (!fn(equip, equip, db_gem))
						return false;
				}
				else if ((flag & kTGSF_PassiveSkill) && !active_skill)
				{
					++cnt;
					if (!fn(equip, equip, db_gem))
						return false;
				}
				return true;
			}
		}
		if (0 == (flag & kTGSF_Inside))
			return true;
		auto s = equip->GetAttrSockets();
		if (!s)
			return true;
		s->TraInsideGem([equip, &fn, &cnt, flag](stCD_EntityItemBased* gem){
			auto db_gem = gem->GetAttrDbSkillGem();
			if (!db_gem)
			{
				assert(false);
				return false;
			}
			auto active_skill = db_gem->GetActiveSkill();
			if ((flag & kTGSF_ActiveSkill) && active_skill)
			{
				++cnt;
				if (!fn(equip, gem, db_gem))
					return false;
			}
			else if ((flag & kTGSF_PassiveSkill) && !active_skill)
			{
				++cnt;
				if (!fn(equip, gem, db_gem))
					return false;
			}
			return true;
		});
		return true;
	});
	return cnt;
}

stCD_EntityItemBased* stCD_Inventory::FindItem(pt_dword item_id) const
{
	stCD_EntityItemBased* res = nullptr;
	TraItems([item_id, &res](stCD_ItemGridsInfo* grids_info, stCD_EntityItemBased* item){
		if (grids_info->item_id_ == item_id)
		{
			res = item;
			return false;
		}
		return true;
	});
	return res;
}

void stCD_ItemGridsInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "ItemGridsInfoAddr:" << this << ",item_id:" << item_id_ << ",item_entityAddr:" << item_entity_;
	idx_info_.DebugInfo();
}

bool stCD_ItemGridsInfo::IsValid() const
{
	if (IsUnValidPtr(item_entity_))
		return false;
	return idx_info_.IsValid();
}

stCD_AttrDbMods* stCD_EntityAttrMods::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

void stCD_EntityAttrMods::DebugInfo() const
{
	LOG_O(Log_debug) << "EntityAttrModsAddr:" << this << ",attr_db_addr:" << attr_db_ << ",is_identified:" << is_identified_
		<< ",item_color:" << item_color_ << ",player_lvl:" << player_lvl_;
	if (words_info_.IsValid())
	{
		for (auto& v : words_info_)
		{
			if (IsUnValidPtr(v.words_dat_) || IsUnValidPtr(v.words_name_))
			{
				assert(false);
				continue;
			}
			v.words_name_->DebugInfo();
		}
	}
}

void stCD_ItemWordsName::DebugInfo() const
{
	LOG_O(Log_debug) << "ItemWordsNameAddr:" << this << ",en_words:" << CD_SafeName(en_words_) << ",cn_words:" << CD_SafeName(cn_words_);
}

void stCD_AttrDbAttributeRequirements::DebugInfo() const
{
	LOG_O(Log_debug) << "AttrDbAttributeRequirementsAddr:" << this << ",power:" << power_ << ",smart:" << smart_ << ",brain:" << brain_
		<< ",unused_level:" << unused_level_;
}

void stCD_AttrDbStack::DebugInfo() const
{
	LOG_O(Log_debug) << "AttrDbStackAddr:" << this << ",max_stack_cnt:" << max_stack_cnt_ 
		<< ",function_desc:" << CD_SafeStr(function_desc_);
}

stCD_AttrDbStack* stCD_EntityAttrStack::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

void stCD_EntityAttrStack::DebugInfo() const
{
	LOG_O(Log_debug) << "EntityAttrStackAddr:" << this << ",item_cnt:" << item_cnt_;
	auto db = GetAttrDb();
	if (db)
		db->DebugInfo();
}

stCD_AttrDbSockets* stCD_EntityAttrSockets::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

void stCD_EntityAttrSockets::DebugInfo() const
{
	LOG_O(Log_debug) << "EntityAttrSocketsAddr:" << this;
	gem_slot_.DebugInfo();
}

int stCD_EntityAttrSockets::TraInsideGem(const Fn_TraInsideGem& fn) const
{
	int cursor = 0;
	int cnt = 0;
	while (true)
	{
		auto gem = gem_slot_.TraInsideGem(cursor);
		if (!gem)
			break;
		if (!fn(gem))
			break;
		++cnt;
	}
	return cnt;
}

int stCD_PassiveSkillGraphMgr::GetTalentPoint() const
{
	//tag_计算剩余天赋点
	if (!learned_skill_id_cont_.IsValid())
	{
		assert(false);
		return -1;
	}
	return (ukn_point1_ + ukn_point2_ + ukn_point3_ + ukn_point4_ + ukn_point5_) - (int)learned_skill_id_cont_.size() - 1;
}

int stCD_PassiveSkillGraphMgr::GetTotalTotalPoint() const
{
	//到底需不需要减1呢？猜测是需要的。
	return ukn_point1_ + ukn_point2_ + ukn_point3_ + ukn_point4_ + ukn_point5_ - 1;
}

void stCD_PassiveSkillGraphMgr::DebugInfo() const
{
	LOG_O(Log_debug) << "PassiveSkillAddr:" << this << ",regret_point:" << regret_point_
		<< ",GetTalentPoint:" << GetTalentPoint()
		<< ",ukn_point1:" << ukn_point1_ << ",ukn_point2:" << ukn_point2_ << ",ukn_point3:"
		<< ukn_point3_ << ",ukn_point4:" << ukn_point4_ << ",ukn_point5:" << ukn_point5_ << ",以下输出所学的技能ID:";
	if (learned_skill_id_cont_.IsValid())
	{
		for (auto v : learned_skill_id_cont_)
		{
			LOG_O(Log_debug) << v;
		}
		LOG_O(Log_debug) << "所学技能总数：" << learned_skill_id_cont_.size();
	}
}

stCD_GameBaseDb& stCD_GameBaseDb::GetMe()
{
	return *(stCD_GameBaseDb*)(void*)CD_EXE_DATA(g_a_db_base_addr_);
}

stCD_Db* stCD_GameBaseDb::FindDb(pt_cwsz db_path) const
{
	/*assert(db_path);
	auto it = db_hash_map_.find(db_path);
	if (it == db_hash_map_.end())
	{
		assert(false);
		return nullptr;
	}
	auto res = it->second;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;*/
	auto res = db_hash_map_.end();
	//这个call如果成功，则返回指向结果的指针
	SlowExecute([db_path, &res, this](){
		auto path_addr_tmp = &db_path;
		auto res_ptr = &res;
		auto this_ptr = this;
		auto call_addr = CD_EXE_DATA(g_c_find_db_file_);
		__asm
		{
			push path_addr_tmp
			push res_ptr
			mov ecx, this_ptr
			call call_addr
		}
	});
	if (res == db_hash_map_.end())
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(res->second))
	{
		assert(false);
		return nullptr;
	}
	return res->second;
}

stCD_DbEntity* stCD_GameBaseDb::FindDbEntity(pt_cwsz db_path) const
{
	auto res = FindDb(db_path);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_DbEntity>();
}

stCD_GameBaseDb::stCD_DbPassiveSkill* stCD_GameBaseDb::GetDbPassiveSkill() const
{
	return FindDbT<stCD_DbPassiveSkill>(CD_kFileDbPath_PassiveSkill);
}

stCD_GameBaseDb::stCD_DbQuestStates* stCD_GameBaseDb::GetDbQuestStates() const
{
	return FindDbT<stCD_DbQuestStates>(CD_kFileDbPath_QuestStates);
}

stCD_GameBaseDb::stCD_DbQuest* stCD_GameBaseDb::GetDbQuest() const
{
	return FindDbT<stCD_DbQuest>(CD_kFileDbPath_Quest);
}

stCD_DbSkillGems* stCD_GameBaseDb::GetDbSkillGems() const
{
	return FindDbT<stCD_DbSkillGems>(CD_kFileDbPath_SkillGems);
}

stCD_GameBaseDb::stCD_DbActiveSkills* stCD_GameBaseDb::GetDbActiveSkills() const
{
	return FindDbT<stCD_DbActiveSkills>(CD_kFileDbPath_ActiveSkills);
}

stCD_DbItemClasses* stCD_GameBaseDb::GetDbItemClasses() const
{
	return FindDbT<stCD_DbItemClasses>(CD_kFileDbPath_ItemClasses);
}

stCD_DbBaseItemTypes* stCD_GameBaseDb::GetDbBaseItemTypes() const
{
	return FindDbT<stCD_DbBaseItemTypes>(CD_kFileDbPath_BaseItemTypes);
}

stCD_DbGemTags* stCD_GameBaseDb::GetDbGenTags() const
{
	return FindDbT<stCD_DbGemTags>(CD_kFileDbPath_GemTags);
}

stCD_DbGrantedEffects* stCD_GameBaseDb::GetDbGrantedEffects() const
{
	return FindDbT<stCD_DbGrantedEffects>(CD_kFileDbPath_GrantedEffects);
}

stCD_DbMapConnections* stCD_GameBaseDb::GetDbMapConnections() const
{
	return FindDbT<stCD_DbMapConnections>(CD_kFileDbPath_MapConnections);
}

stCD_DbWorldArea* stCD_GameBaseDb::GetDbWorldArea() const
{
	return FindDbT<stCD_DbWorldArea>(CD_kFileDbPath_WorldAreas);
}

stCD_DbStats* stCD_GameBaseDb::GetDbStats() const
{
	return FindDbT<stCD_DbStats>(CD_kFileDbPath_Stats);
}

stCD_DbMapPins* stCD_GameBaseDb::GetDbMapPins() const
{
	return FindDbT<stCD_DbMapPins>(CD_kFileDbPath_MapPins);
}

stCD_DbWeaponTypes* stCD_GameBaseDb::GetDbWeaponTypes() const
{
	return FindDbT<stCD_DbWeaponTypes>(CD_kFileDbPath_WeaponTypes);
}

stCD_DbMonsterTypes* stCD_GameBaseDb::GetDbMonsterTypes() const
{
	return FindDbT<stCD_DbMonsterTypes>(CD_kFileDbPath_MonsterTypes);
}

stCD_DbMonsterVarieties* stCD_GameBaseDb::GetDbMonsterVarieties() const
{
	return FindDbT<stCD_DbMonsterVarieties>(CD_kFileDbPath_MonsterVarieties);
}

stCD_DbClientStrings* stCD_GameBaseDb::GetDbClientStrings() const
{
	return FindDbT<stCD_DbClientStrings>(CD_kFileDbPath_ClientStrings);
}

stCD_DbModelArmoursShields* stCD_GameBaseDb::GetModelArmoursShields() const
{
	return FindDbT<stCD_DbModelArmoursShields>(CD_kFileModel_ArmoursShields);
}

stCD_DbEnvironments* stCD_GameBaseDb::GetDbEnvironments() const
{
	return FindDbT<stCD_DbEnvironments>(CD_kFileDbPath_Environments);
}

stCD_DbPassiveSkillGraph* stCD_GameBaseDb::GetDbPassiveSkillGraph() const
{
	return FindDbT<stCD_DbPassiveSkillGraph>(CD_kFileDbPath_PassiveSkillGraph);
}

stCD_DbSkillStatDescriptions* stCD_GameBaseDb::GetDbSkillStatDescriptions() const
{
	return FindDbT<stCD_DbSkillStatDescriptions>(CD_kFileDbPath_SkillStatDescriptions);
}

stCD_DbMinionSpellSkillStat* stCD_GameBaseDb::GetDbMinionSpellSkillStat() const
{
	return FindDbT<stCD_DbMinionSpellSkillStat>(CD_kFileDbPath_MinionSpellSkillStat);
}

stCD_DbChests* stCD_GameBaseDb::GetDbChests() const
{
	return FindDbT<stCD_DbChests>(CD_kFileDbPath_Chests);
}

void stCD_AttrDbSkillGem::DebugInfo() const
{
	if (!granted_effects_pair_.IsValid() || !skill_gem_pair_.IsValid())
	{
		assert(false);
		return;
	}
	if (!skill_gem_pair_.tbl_item_->item_pair_.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "AttrDbSkillGemAddr:" << this 
		<< ",ItemName:" << CD_SafeName(skill_gem_pair_.tbl_item_->item_pair_.tbl_item_->item_name_)
		<< ",skill_gem_db_addr:" << skill_gem_pair_.tbl_item_
		<< ",gem_color_:" << gem_color_ << ",ukn_dw1_:" << ukn_dw1_ << ",ukn_dw2_:" << ukn_dw2_
		<< ",ukn_dw3_:" << ukn_dw3_ << ",ukn_dw4_:" << ukn_dw4_ << ",ukn_dw5_:" << ukn_dw5_ << ",ukn_dw6_:" << ukn_dw6_;
}

stCD_TblItemActiveSkills* stCD_AttrDbSkillGem::GetActiveSkill() const
{
	if (IsUnValidPtr(granted_effects_pair_.tbl_item_))
	{
		assert(false);
		return nullptr;
	}
	auto res = granted_effects_pair_.tbl_item_->GetActiveSkill();
	if (!res)
		return nullptr;
#ifdef _DEBUG
	if (!skill_gem_pair_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	if (!skill_gem_pair_.tbl_item_->granted_effects_pair_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	if (res != skill_gem_pair_.tbl_item_->granted_effects_pair_.tbl_item_->GetActiveSkill())
	{
		assert(false);
		return nullptr;
	}
#endif
	return res;
}

stCD_TblItemGrantedEffectsPerLevel* stCD_AttrDbSkillGem::GetDbPerLevelByLevel(pt_dword lvl) const
{
	if (!granted_effects_pair_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	return granted_effects_pair_.FindDbPerLevelBySkillGemLvl(lvl);
}

stCD_InventorySkillItem* stCD_DownInventoryMgr::GetSkillItem(enCD_DownInventoryType type) const
{
	if (type < kSGT_LeftBt || type > kSGT_KeyT)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(skill_mgr_))
	{
		assert(false);
		return nullptr;
	}
	auto res = skill_mgr_->skills_[type - kSGT_LeftBt];
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

const stCD_Skill_IdInfo* stCD_DownInventoryMgr::GetSkillIdInfo(enCD_DownInventoryType type) const
{
	auto skill = GetSkillItem(type);
	if (!skill)
		return nullptr;
	return &skill->skill_info_.skill_;
}

void stCD_DownInventoryMgr::DebugInfo() const
{
	LOG_O(Log_debug) << "DownInventoryMgrAddr:" << this;
	if (IsUnValidPtr(skill_mgr_))
	{
		assert(false);
		return;
	}
	skill_mgr_->DebugInfo();
}

stCD_SkillTypeInfo* stCD_SceneInGameState::GetLastUsedSkill() const
{
	if (IsUnValidPtr(using_skill_))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(cur_using_skill_end_))
	{
		assert(false);
		return nullptr;
	}
	if (((int)using_skill_end_ - (int)using_skill_) % sizeof(*using_skill_) != 0)
	{
		assert(false);
		return nullptr;
	}
	return using_skill_;
}

stCD_SkillTypeInfo* stCD_SceneInGameState::GetCurUsingSkill() const
{
	if (IsUnValidPtr(using_skill_))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(using_skill_end_))
	{
		assert(false);
		return nullptr;
	}
	if (using_skill_end_ < using_skill_)
	{
		assert(false);
		return nullptr;
	}
	if (using_skill_end_ == using_skill_)
		return nullptr;
	if (((int)using_skill_end_ - (int)using_skill_) % sizeof(*using_skill_) != 0)
	{
		assert(false);
		return nullptr;
	}
	return cur_using_skill_end_ - 1;
}

void stCD_SceneInGameState::DebugInfo() const
{
	LOG_O(Log_debug) << "InGameStateAddr:" << this << ",logic_:" << logic_ << ",stCD_UiMgrTopAddr:" << &ui_logic_
		<< ",using_skill_:" << using_skill_ << ",inventory_mgr_:" << inventory_mgr_;
	if (logic_)
		logic_->DebugInfo();
}

stCD_InGameTopUi* stCD_SceneInGameState::GetTopUi() const
{
	if (IsUnValidPtr(ui_logic_.top_ui_))
	{
		assert(false);
		return nullptr;
	}
	return ui_logic_.top_ui_->CastTo<stCD_InGameTopUi>();
}

stCD_SomeSkillId& stCD_SomeSkillId::GetMe()
{
	return *(stCD_SomeSkillId*)(void*)CD_EXE_DATA(g_a_skill_id_addr_);
}

void stCD_InventorySkillMgr::DebugInfo() const
{
	LOG_O(Log_debug) << "InventorySkillMgrAddr:" << this << ",skills_addr:" << &skills_;
	for (auto v : skills_)
	{
		if (IsUnValidPtr(v))
		{
			assert(false);
			continue;
		}
		v->DebugInfo();
		LOG_O(Log_debug) << "stCD_InventorySkillItem结束";
	}
}

void stCD_InventorySkillItem::DebugInfo() const
{
	LOG_O(Log_debug) << "InventorySkillItemAddr:" << this << ",skill_info_addr:" << &skill_info_;
	skill_info_.DebugInfo();
}

void stCD_InventorySkillItemInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "InventorySkillItemInfoAddr:" << this << ",skill_id_:" << skill_id_;
	skill_.DebugInfo();
}

void stCD_Skill_IdInfo::DebugInfo() const
{
	if (IsUnValidPtr(skill_info_))
	{
		LOG_O(Log_debug) << "Skill_IdInfoAddr:" << this << ",skill_info地址为空:" << skill_info_ << ",skill_ref_info_Addr:" << skill_ref_info_;
		return;
	}
	if (IsUnValidPtr(skill_ref_info_))
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "Skill_IdInfoAddr:" << this << ",skill_info_addr:" 
		<< skill_info_ << ",SkillId:" << skill_info_->skill_id_;
	skill_info_->DebugInfo();
}

void stCD_SkillInfo::DebugInfo() const
{
	auto active_skill = GetActiveSkillDb();
	if (active_skill)
	{
		LOG_O(Log_debug) << "SkillInfoAddr:" << this
			<< ",skill_id:" << (pt_pcvoid)skill_id_
			<< ",技能中文名：" << CD_SafeName(active_skill->ch_name_)
			<< ",技能英文名：" << CD_SafeName(active_skill->en_name_)
			<< ",DbActiveSkillAddr:" << active_skill
			<< ",granted_effects_per_level_Addr:" << granted_effects_per_level_.tbl_item_;
	}
	else
	{
		LOG_O(Log_debug) << "SkillInfoAddr:" << this
			<< ",该技能没有在主动技能表里面，skill_id:" << (pt_pcvoid)skill_id_
			<< ",granted_effects_per_level_Addr:" << granted_effects_per_level_.tbl_item_;
	}
	DebugSkillAttrInfo();
}

stCD_TblItemActiveSkills* stCD_SkillInfo::GetActiveSkillDb() const
{
	/*
	$-81     00623C5A     C1FA 02               SAR EDX, 0x2
	$-7E     00623C5D     50                    PUSH EAX
	$-7D     00623C5E     52                    PUSH EDX
	$-7C     00623C5F     E8 6CAE3D00           CALL PathOfEx.009FEAD0
	$-77     00623C64     8B8B B4080000         MOV ECX, DWORD PTR DS:[EBX+0x8B4]
	$-71     00623C6A     8D95 64FFFFFF         LEA EDX, DWORD PTR SS:[EBP-0x9C]
	$-6B     00623C70     52                    PUSH EDX
	$-6A     00623C71     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$-68     00623C73     FF50 78               CALL NEAR DWORD PTR DS:[EAX+0x78]
	$-65     00623C76     0F2805 D0D2F300       MOVAPS XMM0, DQWORD PTR DS:[0xF3D2D0]
	$-5E     00623C7D     8B8B B4080000         MOV ECX, DWORD PTR DS:[EBX+0x8B4]
	$-58     00623C83     0F1145 34             MOVUPS DQWORD PTR SS:[EBP+0x34], XMM0
	$-54     00623C87     81C1 3C080000         ADD ECX, 0x83C
	$-4E     00623C8D     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$-4C     00623C8F     8D55 34               LEA EDX, DWORD PTR SS:[EBP+0x34]
	$-49     00623C92     52                    PUSH EDX
	$-48     00623C93     FF50 1C               CALL NEAR DWORD PTR DS:[EAX+0x1C]
	$-45     00623C96     8B8B B4080000         MOV ECX, DWORD PTR DS:[EBX+0x8B4]
	$-3F     00623C9C     6A 00                 PUSH 0x0
	$-3D     00623C9E     6A 01                 PUSH 0x1
	$-3B     00623CA0     6A 00                 PUSH 0x0
	$-39     00623CA2     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$-37     00623CA4     FF90 34010000         CALL NEAR DWORD PTR DS:[EAX+0x134]
	$-31     00623CAA     8B8B B4080000         MOV ECX, DWORD PTR DS:[EBX+0x8B4]
	$-2B     00623CB0     6A 00                 PUSH 0x0
	$-29     00623CB2     6A 01                 PUSH 0x1
	$-27     00623CB4     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$-25     00623CB6     FF90 30010000         CALL NEAR DWORD PTR DS:[EAX+0x130]
	$-1F     00623CBC     8B8B B4080000         MOV ECX, DWORD PTR DS:[EBX+0x8B4]
	$-19     00623CC2     81C1 B0080000         ADD ECX, 0x8B0
	$-13     00623CC8     6A 03                 PUSH 0x3
	$-11     00623CCA     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$-F      00623CCC     FF50 08               CALL NEAR DWORD PTR DS:[EAX+0x8]
	$-C      00623CCF     8B7D 50               MOV EDI, DWORD PTR SS:[EBP+0x50]         ; 执行后，edi=stCD_Skill_IdInfo*
	$-9      00623CD2     8D45 3C               LEA EAX, DWORD PTR SS:[EBP+0x3C]
	$-6      00623CD5     50                    PUSH EAX                                 ; eax指向stCD_ActiveSkillPairT的结构
	$-5      00623CD6     8B0F                  MOV ECX, DWORD PTR DS:[EDI]
	$-3      00623CD8     83C1 04               ADD ECX, 0x4                             ; 执行后，ecx=stCD_SkillInfoDb*
	$ ==>    00623CDB     E8 1036EAFF           CALL PathOfEx.004C72F0                   ; 填充stCD_ActiveSkillPairT结构
	$+5      00623CE0     C645 FC 09            MOV BYTE PTR SS:[EBP-0x4], 0x9           ; eax=stCD_ActiveSkillPairT*
	$+9      00623CE4     8B40 04               MOV EAX, DWORD PTR DS:[EAX+0x4]          ; 把带有技能的装备放到身上的时候会断住
	$+C      00623CE7     8B70 04               MOV ESI, DWORD PTR DS:[EAX+0x4]          ; 访问主动技能名
	$+F      00623CEA     33C0                  XOR EAX, EAX
	$+11     00623CEC     C745 20 07000000      MOV DWORD PTR SS:[EBP+0x20], 0x7
	$+18     00623CF3     C745 1C 00000000      MOV DWORD PTR SS:[EBP+0x1C], 0x0
	$+1F     00623CFA     66:8945 0C            MOV WORD PTR SS:[EBP+0xC], AX
	$+23     00623CFE     66:3906               CMP WORD PTR DS:[ESI], AX
	$+26     00623D01     75 04                 JNZ SHORT PathOfEx.00623D07
	$+28     00623D03     33C9                  XOR ECX, ECX
	$+2A     00623D05     EB 18                 JMP SHORT PathOfEx.00623D1F
	$+2C     00623D07     8BCE                  MOV ECX, ESI
	$+2E     00623D09     8D51 02               LEA EDX, DWORD PTR DS:[ECX+0x2]
	$+31     00623D0C     0F1F40 00             NOP DWORD PTR DS:[EAX]
	$+35     00623D10     66:8B01               MOV AX, WORD PTR DS:[ECX]
	$+38     00623D13     83C1 02               ADD ECX, 0x2
	$+3B     00623D16     66:85C0               TEST AX, AX
	$+3E     00623D19   ^ 75 F5                 JNZ SHORT PathOfEx.00623D10
	$+40     00623D1B     2BCA                  SUB ECX, EDX
	$+42     00623D1D     D1F9                  SAR ECX, 1
	$+44     00623D1F     51                    PUSH ECX
	$+45     00623D20     56                    PUSH ESI
	$+46     00623D21     8D4D 0C               LEA ECX, DWORD PTR SS:[EBP+0xC]
	$+49     00623D24     E8 479FE4FF           CALL PathOfEx.0046DC70
	$+4E     00623D29     C645 FC 0A            MOV BYTE PTR SS:[EBP-0x4], 0xA
	$+52     00623D2D     8D55 0C               LEA EDX, DWORD PTR SS:[EBP+0xC]
	$+55     00623D30     8B8B B4080000         MOV ECX, DWORD PTR DS:[EBX+0x8B4]
	$+5B     00623D36     81C1 B0080000         ADD ECX, 0x8B0
	$+61     00623D3C     52                    PUSH EDX
	$+62     00623D3D     8B01                  MOV EAX, DWORD PTR DS:[ECX]
	$+64     00623D3F     FF50 04               CALL NEAR DWORD PTR DS:[EAX+0x4]
	$+67     00623D42     8B45 20               MOV EAX, DWORD PTR SS:[EBP+0x20]         ; d3d9.7010A113
	$+6A     00623D45     83F8 08               CMP EAX, 0x8
	$+6D     00623D48     72 0F                 JB SHORT PathOfEx.00623D59
	$+6F     00623D4A     40                    INC EAX
	*/
	if (!granted_effects_per_level_.IsValid())
	{
		//assert(false);
		return nullptr;
	}
	auto ptr = granted_effects_per_level_.tbl_item_->granted_effects_.tbl_item_;
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	if (!ptr->active_skill_pair_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	return ptr->active_skill_pair_.tbl_item_;
}

stCD_SkillAttrObj stCD_SkillInfo::GetSkillAttrObj() const
{
	stCD_SkillAttrObj res;
	if (IsUnValidPtr(attr_info_))
	{
		assert(false);
		return res;
	}
	res.db_stats_ = stCD_GameBaseDb::GetMe().GetDbStats();
	if (!res.db_stats_)
	{
		assert(false);
		return res;
	}
	res.attr_info_ = attr_info_;
	res.vtbl_ = (stCD_Vtbl_SkillAttrObj*)(pt_pcvoid)CD_EXE_DATA(g_va_skill_attr_obj_);
	return res;
}

bool stCD_SkillInfo::GetSkillAttrValues(stCD_SkillAttrValueCont& attr_values) const
{
	attr_values.clear();
	auto active_skill = GetActiveSkillDb();
	if (!active_skill)
		return true;
	auto& db_mgr = stCD_GameBaseDb::GetMe();
	auto attr_name_cont = stCD_DbSkillStatDescriptions::FindSkillAttr(active_skill->en_name_);
	if (!attr_name_cont)
		return true;
	if (attr_name_cont->empty())
		return true;
	//////////////////////////////////////////////////////////////////////////
	struct stName_ValueType{
		const std::wstring*			attr_name_;
		enCD_SkillAttrValueType		value_type_;
	};
	CmnStaticVector<stName_ValueType, 50> name_value_type_cont;

	struct stHelp{
		typedef std::map<pt_cwsz, enCD_SkillAttrValueType, StdEx::LessChar<wchar_t> > Name2ValueTypeMap;

		stHelp(){
			//僵尸复苏
			name2value_type_.insert(std::make_pair(L"number_of_zombies", kSAVT_CallerMaxCnt));
			//召唤愤怒狂灵
			name2value_type_.insert(std::make_pair(L"number_of_raging_spirits", kSAVT_CallerMaxCnt));
			//唤醒幽魂
			name2value_type_.insert(std::make_pair(L"number_of_spectres", kSAVT_CallerMaxCnt));
			//召唤巨石魔像
			name2value_type_.insert(std::make_pair(L"number_of_golems", kSAVT_CallerMaxCnt));
		}

		Name2ValueTypeMap		name2value_type_;
	};
	static stHelp hlp;

	for (auto& v : *attr_name_cont)
	{
		auto it = hlp.name2value_type_.find(v.c_str());
		if (it == hlp.name2value_type_.end())
			continue;
		name_value_type_cont.push_back({ &v, it->second });
	}
	if (name_value_type_cont.empty())
		return true;
	//////////////////////////////////////////////////////////////////////////
	auto db_spell = db_mgr.GetDbMinionSpellSkillStat();
	if (!db_spell)
	{
		assert(false);
		return false;
	}
	auto tbl_spell = db_spell->GetDbTbl();
	if (!tbl_spell)
	{
		assert(false);
		return false;
	}
	struct stCD_SkillAttrObjWraper : stCD_SkillAttrObj{
		stCD_SkillAttrObjWraper(){
			ZeroMemory(buffer_, sizeof(buffer_));
		}

		stCD_SkillAttrObj		impl_;
		char					buffer_[100];
	};
	stCD_SkillAttrObjWraper skill_attr_obj;
	skill_attr_obj.impl_ = GetSkillAttrObj();
	if (!skill_attr_obj.impl_.IsValid())
	{
		assert(false);
		return false;
	}
	for (auto& v : name_value_type_cont)
	{
		auto skill_attr = tbl_spell->FindSkillAttr(v.attr_name_->c_str());
		if (!skill_attr)
			continue;
		stCD_SkillAttrValue value_tmp;
		for (auto& attr_value_id : skill_attr->attr_value_ids_)
		{
			auto the_value = skill_attr_obj.impl_.GetSkillAttrValue(attr_value_id);
			if (the_value <= 0)
				continue;
			value_tmp.values_.push_back(the_value);
		}
		if (value_tmp.values_.empty())
			continue;
		value_tmp.value_type_ = v.value_type_;
		attr_values.push_back(value_tmp);
	}
	return true;
}

void stCD_SkillInfo::DebugSkillAttrInfo() const
{
	auto active_skill = GetActiveSkillDb();
	if (!active_skill)
		return;
	auto& db_mgr = stCD_GameBaseDb::GetMe();
	auto db_desc = db_mgr.GetDbSkillStatDescriptions();
	if (!db_desc)
	{
		assert(false);
		return;
	}
	if (!db_desc->ResolveMyHashMap())
	{
		assert(false);
		return;
	}
	auto attr_name_cont = stCD_DbSkillStatDescriptions::FindSkillAttr(active_skill->en_name_);
	if (!attr_name_cont)
		return;
	if (attr_name_cont->empty())
		return;
	auto db_spell = db_mgr.GetDbMinionSpellSkillStat();
	if (!db_spell)
	{
		assert(false);
		return;
	}
	auto tbl_spell = db_spell->GetDbTbl();
	if (!tbl_spell)
	{
		assert(false);
		return;
	}
	auto attr_obj = GetSkillAttrObj();
	if (!attr_obj.IsValid())
	{
		assert(false);
		return;
	}
	std::stringstream sstm;
	for (auto& v : *attr_name_cont)
	{
		auto skill_attr = tbl_spell->FindSkillAttr(v.c_str());
		if (!skill_attr)
			continue;
		sstm << WideCharToGbk(v.c_str()) << " : ";
		for (auto& attr_value_id : skill_attr->attr_value_ids_)
		{
			auto the_value = attr_obj.GetSkillAttrValue(attr_value_id);
			if (the_value <= 0)
				continue;
			sstm << the_value << '\t';
		}
		sstm << '\n';
	}
	LOG_O(Log_debug) << sstm.str();
}

void stCD_UiSelectMySkill::DebugInfo() const
{
	LOG_O(Log_debug) << "UiSelectMySkillAddr:" << this << ",my_skills_Addr:" << &my_skills_;
}

int stCD_UiSelectMySkill::TraMySkill(const Fn_TraMySkill& fn) const
{
	int cnt = 0;
	for (auto& v : my_skills_)
	{
		if (IsUnValidPtr(v.second.skill_info_))
			continue;
		auto skill_info_ = v.second.skill_info_;
		if (IsUnValidPtr(skill_info_))
		{
			assert(false);
			return false;
		}
		auto active_skill = skill_info_->GetActiveSkillDb();
		if (!active_skill)
			continue;
		++cnt;
		if (!fn(skill_info_))
			break;
	}
	return cnt;
}

void stCD_ActorSkillMgr::DebugInfo() const
{
	LOG_O(Log_debug) << "ActorSkillMgrAddr:" << this << ",weapon_skill_type_:" << (pt_pcvoid)weapon_skill_type_
		<< ",weapon_skill_value_:" << (pt_pcvoid)weapon_skill_value_
		<< ",skill_ui_mgr_个数：" << skill_ui_mgr_.size()
		<< ",my_skills_个数：" << my_skills_.size();
	/*auto ui_skill = GetUiSkillMgr();
	if (ui_skill)
		ui_skill->DebugInfo();*/
	auto cnt = TraMySkill([](stCD_SkillInfo* skill_info){
		skill_info->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总共遍历到的技能数量：" << cnt;
}

stCD_MySkillMgrEx* stCD_ActorSkillMgr::GetUiSkillMgr() const
{
	if (!skill_ui_mgr_.IsValid())
	{
		//assert(false);
		return nullptr;
	}
	for (auto v : skill_ui_mgr_)
	{
		if (IsUnValidPtr(v->vtbl_))
		{
			assert(false);
			return nullptr;
		}
		if ((pt_cdword)v->vtbl_->fn_ == CD_EXE_DATA(g_c_my_skill_mgr_base_fn_tag_))
			return v->CastTo<stCD_MySkillMgrEx>();
	}
	assert(false);
	return nullptr;
}

stCD_UiSelectMySkill* stCD_ActorSkillMgr::GetUiSelectMySkill() const
{
	auto res = GetUiSkillMgr();
	if (!res)
		return nullptr;
	return res->GetMySkillMgr();
}

int stCD_ActorSkillMgr::TraMySkill(const Fn_TraMySkill& fn) const
{
	if (!my_skills_.IsValid())
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (auto& v : my_skills_)
	{
		if (IsUnValidPtr(v.skill_info_))
			continue;
		/*auto active_skill = v.skill_info_->GetActiveSkillDb();
		if (!active_skill)
			continue;*/
		++cnt;
		if (!fn(v.skill_info_))
			break;
	}
	return cnt;
}

stCD_SkillInfo* stCD_ActorSkillMgr::FindSkill(pt_word skill_id) const
{
	if (!my_skills_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	for (auto& v : my_skills_)
	{
		if (IsUnValidPtr(v.skill_info_))
			continue;
		if (v.skill_info_->skill_id_ == skill_id)
			return v.skill_info_;
	}
	return nullptr;
}

void stCD_MySkillMgrEx::DebugInfo() const
{
	LOG_O(Log_debug) << "MySkillMgrExAddr:" << this;
	auto mgr = GetMySkillMgr();
	if (mgr)
		mgr->DebugInfo();
}

stCD_UiSelectMySkill* stCD_MySkillMgrEx::GetMySkillMgr() const
{
	auto res = (stCD_UiSelectMySkill*)((pt_byte*)this + CD_DATA(g_o_get_my_skill_mgr_));
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_UiObjBase* stCD_UiEventMgr::FindUiObjImpl(pt_pcvoid holder_vfn, pt_pcvoid fn_ui_handle_logic) const
{
	stCD_UiObjBase* res = nullptr;
	TraUiObjImpl(holder_vfn, fn_ui_handle_logic, [&res](stCD_UiObjBase* ui_obj){
		assert(ui_obj);
		res = ui_obj;
		return false;
	});
	assert(res);
	return res;
}

int stCD_UiEventMgr::TraUiObjImpl(pt_pcvoid holder_vfn, pt_pcvoid fn_ui_handle_logic, const Fn_TraUiObjBase& fn) const
{
	//tag_Ui事件遍历算法。tag_响应ui事件
	if (IsUnValidPtr(first_obj_))
	{
		assert(false);
		return 0;
	}
	auto the_ptr = first_obj_;
	int cnt = 0;
	while (true)
	{
		auto vtbl = the_ptr->vtbl_;
		if (IsUnValidPtr(vtbl))
			break;
		if (vtbl->handle_logic_ == holder_vfn)
		{
			auto holder_ptr = the_ptr->CastTo<stCD_UiEvent>();
			assert(holder_ptr);
			if (holder_ptr->fn_handle_ui_logic_ == fn_ui_handle_logic)
			{
				++cnt;
				if (!fn(holder_ptr->GetUiObj()))
					return cnt;
			}
		}
		auto the_next_fn = *(stCD_UiEventVtbl::Fn_GetNextOffset*)&vtbl->fn_get_next_offset_;
		auto offset = (the_ptr->*the_next_fn)();
		the_ptr = (stCD_UiEventBase*)((pt_byte*)the_ptr + offset);
		if (IsUnValidPtr(the_ptr))
		{
			assert(false);
			return cnt;
		}
	}
	//assert(false);
	return cnt;
}

stCD_Render* stCD_EntityNoHuman::GetRender() const
{
	return __super::GetRender();
}

void stCD_EntityNoHuman::DebugInfo() const
{
	__super::DebugInfo();
}

void stCD_CurQuestInfo::DebugInfo() const
{
	if (!quest_.IsValid())
	{
		assert(false);
		return;
	}
	if (!quest_state_.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "CurQuestInfoAddr:" << this << ",ukn_dw_:" << ukn_dw_ << ",ukn_dw1_:" << ukn_dw1_;
	quest_.tbl_item_->DebugInfo();
	quest_state_.tbl_item_->DebugInfo();
	if (map_pin_.IsValid())
		map_pin_.tbl_item_->DebugInfo();
}

void stCD_MapDataTerrainModel::DebugInfo(const stCD_VecInt& pos) const
{
	if (!model_.IsValid())
	{
		LOG_O(Log_error) << "MapDataModelAddr:" << this;
		//assert(false);
		return;
	}
	LOG_O(Log_debug) << "MapDataModelAddr:" << this 
		//<< ",pos:" << pos_ 
		<< ",shrink_pos:" << pos 
		//<< ",GamePos1:" << stCD_VecInt(pos_.x * CD_kMapRatio, pos_.y * CD_kMapRatio)
		<< ",GamePos2:" << stCD_VecInt(pos.x * CD_kMapRatio, pos.y * CD_kMapRatio)
		<< ",TerrainObjCnt:" << terrain_objs_.size()
		<< ",ukn_dw_:" << ukn_dw_
		<< ",ukn_w_:" << ukn_w_ << ",ukn_w1_:" << ukn_w1_
		<< ",model_path:" << CD_SafePath(model_.tbl_item_->model_path_);
	for (auto& v : terrain_objs_)
	{
		v.DebugInfo();
	}
}

stCD_UiObjBase* stCD_UiEvent::GetUiObj() const
{
	if (IsUnValidPtr(ui_obj_))
	{
		assert(false);
		return nullptr;
	}
	return (stCD_UiObjBase*)((pt_byte*)ui_obj_ + ui_obj_offset_);
}

void stCD_DlgWaypointMap::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << "DlgWaypointMapAddr:" << this << ",connections_num_:" << connections_num_
#ifndef VER3_
		<< ",difficulty_:" << difficulty_
#endif
		<< ",chapter_idx_:" << chapter_idx_;
}

bool stCD_InGameMainUi::CloseDlgModal()
{
	auto dlg_modal = GetDlgModal();
	if (!dlg_modal)
		return true;
	if (!dlg_modal->is_shown_)
		return true;
	dlg_modal->Show(false);
	dlg_modal_ = nullptr;

	return CloseDarkPanel();
}

stCD_DlgWaypointQuestsContainer* stCD_DlgWaypointQuestsPanel::GetUiQuests() const
{
	auto c = GetUiQuestsImpl();
	if (!c)
	{
		assert(false);
		return nullptr;
	}
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_item_vtbl_));
	auto res = c->FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgWaypointQuestsContainer>();
}

bool stCD_EntityAttrClientNpc::NeedVisit() const
{
	if (IsUnValidPtr(entity_effect_))
		return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
int stCD_UiObjBase::TraSubUiObj(int sub_lvl, const UiFilterBase& filter, enCD_TraUiShownFlag show_flag, const Fn_TraSubUiObj& fn) const
{
	int valid_cnt = 0;
	if (show_flag & kTUSF_Shown)
	{
		if (is_shown_)
			++valid_cnt;
	}
	if (show_flag & kTUSF_UnShown)
	{
		if (!is_shown_)
			++valid_cnt;
	}
	if (valid_cnt == 0)
		return -1;
	if (1 == sub_lvl)
	{
		if (!sub_uis_.sub_ui_cont_.IsValid())
			return -1;
		int cnt = 0;
		for (auto sub_ui : sub_uis_.sub_ui_cont_)
		{
			if (IsUnValidPtr(sub_ui))
			{
				assert(false);
				break;
			}
			valid_cnt = 0;
			if (show_flag & kTUSF_Shown)
			{
				if (sub_ui->is_shown_)
					++valid_cnt;
			}
			if (show_flag & kTUSF_UnShown)
			{
				if (!sub_ui->is_shown_)
					++valid_cnt;
			}
			if (valid_cnt == 0)
				continue;
			if (!filter.Filter(*sub_ui))
				continue;
			++cnt;
			if (!fn(sub_ui))
				return cnt;
		}
		if (cnt > 0)
		{
			//LOG_O(Log_debug) << "this_obj_addr:" << this << ",level:" << sub_lvl;
			return cnt;
		}
		return -1;
	}
	else if (sub_lvl < 1)
	{
		assert(false);
		return -1;
	}
	if (!sub_uis_.sub_ui_cont_.IsValid())
		return -1;
	--sub_lvl;
	int the_res = -1;
	for (auto sub_ui : sub_uis_.sub_ui_cont_)
	{
		if (IsUnValidPtr(sub_ui))
		{
			assert(false);
			return -1;
		}
		auto res = sub_ui->TraSubUiObj(sub_lvl, filter, show_flag, fn);
		if (res >= 0)
		{
			if (show_flag & kTUSF_AllNode)
			{
				if (the_res < 0)
					the_res = 0;
				the_res += res;
			}
			else
			{
				//LOG_O(Log_debug) << "parent_ui_addr:" << this << ",sub_lvl:" << sub_lvl + 1;
				//this->DebugInfo();
				return res;
			}
		}		
	}
	return the_res;
}

int stCD_UiObjBase::TraSubUiObjEx(int sub_lvl, pt_pcvoid addr, enCD_TraUiFilterType filter_type,
	enCD_TraUiShownFlag show_flag, const Fn_TraSubUiObj& fn) const
{
	switch (filter_type)
	{
	case kTUFT_ByVtbl:
	{
		_stCD_UiFilterVtbl f_vtbl(addr);
		return TraSubUiObj(sub_lvl, f_vtbl, show_flag, fn);
	}
	case kTUFT_ByAddr:
	{
		_stCD_UiFilterUiAddr f_addr((stCD_UiObjBase*)addr);
		return TraSubUiObj(sub_lvl, f_addr, show_flag, fn);
	}
	case kTUFT_AllSubNodes:
		break;
	default:
		assert(false);
		return 0;
	}
	_stCD_UiFilterAny filter;
	return TraAllSubUiObj(filter, kTUSF_ShowHide, fn);
}

void stCD_UiObjBase::DebugInfo() const
{
	LOG_O(Log_debug) << "UiObjBaseAddr:" << this << ",vtbl:" << vtbl_ 
		//<< ",ClassName:" << StdEx::RttiEx::TypeName(this)
		<< ",is_shown:" << is_shown_ << ",ukn_bool:" << ukn_bool_;
}

void stCD_UiObjBase::DebugFullInfo(enCD_TraUiShownFlag show_flag) const
{
	LOG_O(Log_debug) << "UiObjBaseAddr:" << this << ",vtbl:" << vtbl_
		//<< ",ClassName:" << StdEx::RttiEx::TypeName(this)
		<< ",is_shown:" << is_shown_ << ",ukn_bool:" << ukn_bool_
		<< ",子ui数量：" << sub_uis_.sub_ui_cont_.size();
	if (!sub_uis_.sub_ui_cont_.IsValid())
		return;
	int cnt = 0;
	for (auto v : sub_uis_.sub_ui_cont_)
	{
		if (IsUnValidPtr(v))
		{
			assert(false);
			return;
		}
		int valid_cnt = 0;
		if (show_flag & kTUSF_Shown)
		{
			if (v->is_shown_)
				++valid_cnt;
		}
		if (show_flag & kTUSF_UnShown)
		{
			if (!v->is_shown_)
				++valid_cnt;
		}
		if (valid_cnt == 0)
			continue;
		++cnt;
		v->DebugInfo();
	}
	LOG_O(Log_debug) << "遍历到的数量：" << cnt;
}

stCD_UiObjBase* stCD_UiObjBase::FindSubUiObj(int sub_lvl, const UiFilterBase& filter, enCD_TraUiShownFlag show_flag, int the_cnt) const
{
	if (the_cnt < 1)
	{
		assert(false);
		return nullptr;
	}
	stCD_UiObjBase* res = nullptr;
	TraSubUiObj(sub_lvl, filter, show_flag, [&res, &the_cnt](stCD_UiObjBase* ui){
		--the_cnt;
		if (0 == the_cnt)
		{
			res = ui;
			return false;
		}
		return true;
	});
	return res;
}

int stCD_UiObjBase::TraAllSubUiObj(const UiFilterBase& filter, enCD_TraUiShownFlag show_flag, const Fn_TraSubUiObj& fn) const
{
	auto res = TraAllSubUiObjImpl(filter, show_flag, fn);
	if (res < 0)
		return -res;
	return res;
}

int stCD_UiObjBase::TraAllSubUiObjImpl(const UiFilterBase& filter, enCD_TraUiShownFlag show_flag, const Fn_TraSubUiObj& fn) const
{
	int valid_cnt = 0;
	if (show_flag & kTUSF_Shown)
	{
		if (is_shown_)
			++valid_cnt;
	}
	if (show_flag & kTUSF_UnShown)
	{
		if (!is_shown_)
			++valid_cnt;
	}
	if (valid_cnt == 0)
		return 0;
	if (!filter.Filter(*this))
		return 0;
	if (!fn(const_cast<stCD_UiObjBase*>(this)))
		return -1;
	if (!sub_uis_.sub_ui_cont_.IsValid())
		return 1;
	int the_res = 1;
	for (auto sub_ui : sub_uis_.sub_ui_cont_)
	{
		if (IsUnValidPtr(sub_ui))
		{
			assert(false);
			if (the_res > 0)
				the_res = -the_res;
			break;
		}
		auto tmp_res = sub_ui->TraAllSubUiObj(filter, show_flag, fn);
		if (tmp_res < 0)
		{
			the_res = abs(tmp_res) + abs(the_res);
			the_res = -the_res;
			break;
		}
		else
		{
			assert(the_res > 0);
			the_res += tmp_res;
		}
	}
	return the_res;
}

bool stCD_UiObjBase::IsPropPanel() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_prop_) == vtbl_;
}

bool stCD_UiObjBase::IsStashPanel() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_stash_) == vtbl_;
}

bool stCD_UiObjBase::IsPassiveSkillPanel() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_passive_skill_) == vtbl_;
}

bool stCD_UiObjBase::IsWaypointPanel() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_waypoint_vtbl_) == vtbl_;
}

bool stCD_UiObjBase::IsQuestAward() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_quest_award_) == vtbl_;
}

bool stCD_UiObjBase::IsDlgGuide() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_guide_) == vtbl_;
}

bool stCD_UiObjBase::SameShowState(enCD_TraUiShownFlag show_flag) const
{
	if (kTUSF_Shown & show_flag)
	{
		if (is_shown_)
			return true;
		return false;
	}
	else if (kTUSF_UnShown & show_flag)
	{
		if (is_shown_)
			return false;
		return true;
	}
	assert(false);
	return false;
}

stCD_UiObjBase* stCD_UiObjBase::FindSubUiObjByVtbl(int sub_lvl, pt_pcvoid vtbl, enCD_TraUiShownFlag show_flag, int the_cnt) const
{
	_stCD_UiFilterVtbl filter(vtbl);
	return FindSubUiObj(sub_lvl, filter, show_flag, the_cnt);
}

stCD_UiObjBase* stCD_UiObjBase::Parent() const
{
	if (IsUnValidPtr(parent_))
		return nullptr;
	return parent_;
}

stCD_UiObjBase* stCD_UiObjBase::Parent(int lvl) const
{
	auto parent = parent_;
	while (lvl > 0)
	{
		if (IsUnValidPtr(parent))
			break;
		--lvl;
		parent = parent->parent_;
	}
	if (lvl > 0)
		return nullptr;
	if (IsUnValidPtr(parent))
		return nullptr;
	return parent;
}

stCD_StateUiLogic* stCD_UiObjBase::GetUiLogic() const
{
	if (IsUnValidPtr(ui_logic_))
	{
		assert(false);
		return nullptr;
	}
	return ui_logic_;
}

void stCD_UiObjBase::DebugParentsInfo() const
{
	auto parent = parent_;
	int lvl = 0;
	DebugInfo();
	while (!IsUnValidPtr(parent))
	{
		parent->DebugInfo();
		parent = parent->parent_;
		++lvl;
	}
	LOG_O(Log_debug) << "父结点总数量：" << lvl;
}

bool stCD_UiObjBase::IsDlgBuy() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_buy_) == vtbl_;
}

bool stCD_UiObjBase::IsDlgSell() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_sell_) == vtbl_;
}

bool stCD_UiObjBase::IsDlgMapLocator() const
{
	return (pt_pcvoid)CD_EXE_DATA(g_va_dlg_map_locator_) == vtbl_;
}

int stCD_UiObjBase::GetSubUiCount() const
{
	return (int)sub_uis_.sub_ui_cont_.size();
}

stCD_InGameMainUi* stCD_InGameTopUi::GetMainUi() const
{
	assert(vtbl_ == (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	stCD_UiObjBase* res = nullptr;
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_top_main_ui_vtbl_));
	TraSubUiObj(1, filter, kTUSF_Shown, [&res](stCD_UiObjBase* ui_obj){
		res = ui_obj;
		return false;
	});
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_InGameMainUi>();
}

void stCD_WaypointPointItem::DebugInfo(enCD_kMapDifficulty difficult, stCD_DlgWaypointMap* map) const
{
	__super::DebugInfo();
	if (!map_pin_.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "map_ch_name:" << CD_SafeName(map_pin_.tbl_item_->map_ch_name_)
		<< ",difficult:" << difficult << ",map_chapter_idx:" << map->chapter_idx_
		<< ",TheMapHasWaypoint:" << map->TheMapHasWaypoint(*this) << ",DiscovedTheMap:" << map->DiscovedTheMap(*this)
		<< ",IsTheWaypointOpened:" << map->IsTheWaypointOpened(*this);
}

bool stCD_WaypointPointItem::EqualMapName(const std::wstring& map_name) const
{
	if (!map_pin_.IsValid())
	{
		assert(false);
		return false;
	}
	return map_name == map_pin_.tbl_item_->map_ch_name_;
}

stCD_TblItemMapPins* stCD_WaypointPointItem::GetMapPin() const
{
	if (!map_pin_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	return map_pin_.tbl_item_;
}

stCD_DlgWaypoint* stCD_InGameMainUi::GetDlgWaypoint() const
{
	stCD_UiObjBase* res = nullptr;
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_waypoint_vtbl_));
	TraSubUiObj(1, filter, kTUSF_ShowHide, [&res](stCD_UiObjBase* ui_obj){
		res = ui_obj;
		return false;
	});
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgWaypoint>();
}

int stCD_InGameMainUi::GetDlgCnt(enCD_TraUiShownFlag show_flag) const
{
	_stCD_UiFilterAny filter;
	int cnt = 0;
	TraSubUiObj(1, filter, show_flag, [&cnt](stCD_UiObjBase* ui){
		++cnt;
		return true;
	});
	return cnt;
}

stCD_UiNpcPanelContainer* stCD_InGameMainUi::GetNpcPanelContainer() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 2);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiNpcPanelContainer>();
}

stCD_UiNpcPanel* stCD_InGameMainUi::GetNpcPanel(bool must_shown) const
{
	auto c = GetNpcPanelContainer();
	if (!c)
	{
		assert(false);
		return nullptr;
	}
	return c->GetNpcPanel(must_shown);
}

void stCD_InGameMainUi::DebugAddrInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << "InGameMainUiAddr:" << this;
	LOG_O(Log_debug) << "NpcPanelContainerAddr:" << GetNpcPanelContainer();
	LOG_O(Log_debug) << "GetNpcPanelAddr:" << GetNpcPanel(false);
	LOG_O(Log_debug) << "GetDlgWaypointAddr:" << GetDlgWaypoint();
	auto npc_panel = GetNpcPanel(true);
	if (npc_panel)
		npc_panel->DebugAddrInfo();
	auto quest_lvl = GetQuestLvlStatePanel();
	if (quest_lvl)
		quest_lvl->DebugInfo();
}

stCD_UiDlgQuestAward* stCD_InGameMainUi::GetDlgQuestAward() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_quest_award_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
	{
		LOG_O(Log_debug) << "没有找到stCD_UiDlgQuestAward";
		//assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiDlgQuestAward>();
}

stCD_UiMouseAttachItem* stCD_InGameMainUi::GetMouseAttachItem() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_mouse_attach_item_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiMouseAttachItem>();
}

stCD_UiRightMostPanel* stCD_InGameMainUi::GetRightMostPanel(enCD_RightMostPanel idx) const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, (int)idx);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiRightMostPanel>();
}

stCD_UiQuestLvlStatePanel* stCD_InGameMainUi::GetQuestLvlStatePanel() const
{
	auto res = GetRightMostPanel(kRMP_QuestLvlState);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_UiQuestLvlStatePanel>();
}

stCD_UiDiedPanel* stCD_InGameMainUi::GetDiedPanel() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_died_panel));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_UiDiedPanel>();
}

stCD_DlgInteraction* stCD_InGameMainUi::GetDlgModal() const
{
	if (!dlg_modal_)
		return nullptr;
	if (IsUnValidPtr(dlg_modal_))
	{
		assert(false);
		return nullptr;
	}
	if (dlg_modal_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_passive_skill_warning_panel))
	{
		assert(false);
		return nullptr;
	}
	return dlg_modal_;
}

stCD_UiObjBase* stCD_InGameMainUi::GetDarkPanel() const
{
	if (!ui_dark_panel_)
		return nullptr;
	if (IsUnValidPtr(ui_dark_panel_))
	{
		assert(false);
		return nullptr;
	}
	if (ui_dark_panel_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_))
	{
		assert(false);
		return nullptr;
	}
	return ui_dark_panel_;
}

bool stCD_InGameMainUi::CloseDarkPanel() const
{
	auto panel = GetDarkPanel();
	if (!panel)
		return true;
	if (!panel->is_shown_)
		return true;
	panel->Show(false);
	return true;
}

stCD_UiMap* stCD_InGameMainUi::GetUiMap() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_map_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiMap>();
}

stCD_UiEntityDescTop* stCD_InGameMainUi::GetEntityDescTop() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_entity_desc_top_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiEntityDescTop>();
}

stCD_UiStashTop* stCD_DlgStash::GetUiTop() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiStashTop>();
}

stCD_DlgStash::UiTabPages* stCD_DlgStash::GetTabPages() const
{
	auto ui = GetUiTop();
	if (!ui)
		return nullptr;
	return ui->GetTabPages();
}

void stCD_DlgStash::DebugInfo() const
{
	LOG_O(Log_debug) << "DlgStashAddr:" << this << ",UiTopAddr:" << GetUiTop() << ",TabPagesAddr:" << GetTabPages();
}

stCD_DlgProp* stCD_InGameMainUi::GetDlgProp() const
{
	auto dlg_prop = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_prop_), kTUSF_ShowHide, 1);
	if (!dlg_prop)
	{
		assert(false);
		return nullptr;
	}
	return dlg_prop->CastTo<stCD_DlgProp>();
}

stCD_DlgBuy* stCD_InGameMainUi::GetDlgBuy() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_buy_), kTUSF_Shown, 1);
	if (!res) return nullptr;
	return res->CastTo<stCD_DlgBuy>();
}

stCD_DlgSell* stCD_InGameMainUi::GetDlgSell() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_sell_), kTUSF_Shown, 1);
	if (!res) return nullptr;
	return res->CastTo<stCD_DlgSell>();
}

stCD_DlgStash* stCD_InGameMainUi::GetDlgStash() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_stash_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgStash>();
}

stCD_DlgPassiveSkill* stCD_InGameMainUi::GetDlgPassiveSkill() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_passive_skill_), kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgPassiveSkill>();
}

stCD_UiBuff* stCD_InGameMainUi::GetUiBuff() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_item_vtbl_), kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_UiBuff>();
}

stCD_DlgMapLocator* stCD_InGameMainUi::GetDlgMapLocator() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_dlg_map_locator_), kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_DlgMapLocator>();
}

stCD_UiBottom* stCD_InGameMainUi::GetUiBottom() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_ShowHide, 2);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!res->is_shown_)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiBottom>();
}

stCD_DlgSelfMapLocator* stCD_InGameMainUi::GetDlgSelfMapLocator() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_self_map_locator_), kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_DlgSelfMapLocator>();
}

stCD_UiTutorials* stCD_InGameMainUi::GetUiTutorials() const
{
	auto res = FindSubUiObjByVtbl(1, (void*)CD_EXE_DATA(g_va_ui_tutorials_), kTUSF_Shown, 1);
	if (res)
		return res->CastTo<stCD_UiTutorials>();
	return nullptr;
}

stCD_DlgLabyrinthSelect* stCD_InGameMainUi::GetDlgLabyrinthSelect() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_labyrinth_select_), kTUSF_Shown, 1);
	if (res)
		return res->CastTo<stCD_DlgLabyrinthSelect>();
	assert(false);
	return nullptr;
}

stCD_DlgTrialPlaque* stCD_InGameMainUi::GetDlgTrialPlaque() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_trial_plaque_), kTUSF_Shown, 1);
	if (res)
		return res->CastTo<stCD_DlgTrialPlaque>();
	assert(false);
	return nullptr;
}

stCD_DlgInstanceMgr* stCD_InGameMainUi::GetDlgInstanceMgr() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_instance_mgr_), kTUSF_Shown, 1);
	if (res) return res->CastTo<stCD_DlgInstanceMgr>();
	return nullptr;
}

stCD_DlgLabyrinthMap* stCD_InGameMainUi::GetDlgLabyrinthMap() const
{
	assert(false);
	return nullptr;
	/*auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_labyrinth_map_), kTUSF_Shown, 1);
	if (res) return res->CastTo<stCD_DlgLabyrinthMap>();
	return nullptr;*/
}

int stCD_DlgWaypoint::TraTabDifficulty(enCD_TraUiShownFlag show_flag, const Fn_TraTabDifficulty& fn) const
{
	auto c = GetTabDifficultContainer();
	if (!c)
	{
		assert(false);
		return 0;
	}
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	return c->TraSubUiObj(1, filter, show_flag, [&fn](stCD_UiObjBase* ui){
		return fn(ui->CastTo<stCD_DlgWaypointTabDifficulty>());
	});
}

int stCD_DlgWaypoint::TraWaypointItem(enCD_TraUiShownFlag show_flag, enCD_kMapDifficulty difficulty, const Fn_TraWaypointItem& fn) const
{
	int cnt = 0;
	bool ret = true;
	int cur_difficulty = 0;
	TraTabDifficulty(show_flag, [show_flag, &fn, &cnt, &ret, &cur_difficulty, difficulty](stCD_DlgWaypointTabDifficulty* tab_difficult){
		++cur_difficulty;
		if (cur_difficulty < difficulty)
			return ret;
		if (difficulty != kMD_Invalid && cur_difficulty > difficulty)
		{
			ret = false;
			return false;
		}
		tab_difficult->TraTabChapter(show_flag, [show_flag, &fn, &cnt, &ret, cur_difficulty](stCD_DlgWaypointTabChapter* tab_chapter){
			auto map = tab_chapter->GetMap();
			if (!map)
			{
				assert(false);
				return false;
			}
			cnt += tab_chapter->TraWaypointItem(show_flag, &ret, [cur_difficulty, map, &fn, &ret](stCD_WaypointPointItem* point_item){
				if (!fn((enCD_kMapDifficulty)cur_difficulty, map, point_item))
				{
					ret = false;
					return false;
				}
				return ret;
			});
			return ret;
		});
		return ret;
	});
	return cnt;
}

stCD_UiObjBase* stCD_DlgWaypoint::GetTabDifficultContainer() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	auto res = FindSubUiObj(3, filter, kTUSF_ShowHide, 2);
	assert(res);
	return res;
}

bool stCD_DlgWaypoint::FindWaypoint(enCD_kMapDifficulty the_difficulty, const std::wstring& map_name, stWaypointInfo& out_res) const
{
	bool res = false;
	TraWaypointItem(kTUSF_ShowHide, the_difficulty, [&map_name, &res, &out_res](enCD_kMapDifficulty difficult, stCD_DlgWaypointMap* map, stCD_WaypointPointItem* item){
		if (!item->map_pin_.IsValid())
		{
			assert(false);
			return false;
		}
		if (map_name == item->map_pin_.tbl_item_->map_ch_name_)
		{
			res = true;
			out_res.difficulty_ = difficult;
			out_res.map_ = map;
			out_res.waypoint_ = item;
			return false;
		}
		return true;
	});
	return res;
}

int stCD_DlgWaypoint::TraMap(enCD_TraUiShownFlag show_flag, const Fn_TraMap& fn) const
{
	int cnt = 0;
	bool ret = true;
	TraTabDifficulty(show_flag, [show_flag, &fn, &cnt, &ret](stCD_DlgWaypointTabDifficulty* tab_difficult){
		tab_difficult->TraTabChapter(show_flag, [show_flag, &fn, &cnt, &ret](stCD_DlgWaypointTabChapter* tab_chapter){
			auto map = tab_chapter->GetMap();
			if (!map)
			{
				assert(false);
				return false;
			}
			++cnt;
			if (!fn(map))
			{
				ret = false;
				return false;
			}
			return true;
		});
		return ret;
	});
	return cnt;
}

void stCD_DlgWaypoint::DebugInfo() const
{
	stCD_DlgWaypointTabDifficulty* difficult = nullptr;
	TraTabDifficulty(kTUSF_Shown, [&difficult](stCD_DlgWaypointTabDifficulty* d){
		difficult = d;
		return false;
	});
	if (!difficult)
	{
		assert(false);
		return;
	}
	stCD_DlgWaypointTabChapter* chapter = nullptr;
	difficult->TraTabChapter(kTUSF_Shown, [&chapter](stCD_DlgWaypointTabChapter* c){
		chapter = c;
		return false;
	});
	if (!chapter)
	{
		assert(false);
		return;
	}
	auto chapter_content = chapter->GetContent();
	if (!chapter_content)
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "DlgWaypointAddr:" << this
		<< ", difficultAddr:" << difficult
		<< ",chapterAddr:" << chapter
		<< ",chapter_contentAddr:" << chapter_content
		<< ",MapAddr:" << chapter_content->GetMap()
		<< ",QuestsPanelAddr:" << chapter_content->GetQuestsPanel()
		<< ",QuestInfoPanelAddr:" << chapter_content->GetQuestInfoPanel();
}

int stCD_DlgWaypoint::TraQuestItem(enCD_TraUiShownFlag show_flag, enCD_kMapDifficulty difficulty, int chapter, 
	const Fn_TraQuestItem& fn) const
{
	bool ret = true;
	int cur_difficulty = 0;
	int cnt = 0;
	TraTabDifficulty(show_flag, [show_flag, &cur_difficulty, &ret, difficulty, &cnt, &fn, chapter](stCD_DlgWaypointTabDifficulty* tab_difficult){
		++cur_difficulty;
		if (cur_difficulty < difficulty)
			return ret;
		if (difficulty != kMD_Invalid && cur_difficulty > difficulty)
		{
			ret = false;
			return false;
		}
		tab_difficult->TraTabChapter(show_flag, [show_flag, &fn, &cnt, &ret, cur_difficulty, chapter](stCD_DlgWaypointTabChapter* tab_chapter){
			auto map = tab_chapter->GetMap();
			if (!map)
			{
				assert(false);
				return false;
			}
			if ((int)map->chapter_idx_ < chapter)
				return ret;
			if (chapter > 0 && (int)map->chapter_idx_ > chapter)
			{
				ret = false;
				return false;
			}
			cnt += tab_chapter->TraQuestsItem(&ret, [cur_difficulty, &fn, map, &ret](stCD_DlgWaypointQuestItem* quest_item){
				if (!fn((enCD_kMapDifficulty)cur_difficulty, map->chapter_idx_, quest_item))
				{
					ret = false;
					return false;
				}
				return ret;
			});
			return ret;
		});
		return ret;
	});
	return cnt;
}

stCD_UiObjBase* stCD_DlgWaypointTabDifficulty::GetChapterContainer() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	auto res = FindSubUiObj(3, filter, kTUSF_ShowHide, 2);
	assert(res);
	return res;
}

int stCD_DlgWaypointTabDifficulty::TraTabChapter(enCD_TraUiShownFlag show_flag, const Fn_TraTabChapter& fn) const
{
	auto c = GetChapterContainer();
	if (!c)
	{
		assert(false);
		return 0;
	}
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	return c->TraSubUiObj(1, filter, show_flag, [&fn](stCD_UiObjBase* ui){
		return fn(ui->CastTo<stCD_DlgWaypointTabChapter>());
	});
}

stCD_DlgWaypointMap* stCD_DlgWaypointChapterContent::GetMap() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_waypoint_map_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgWaypointMap>();
}

stCD_DlgWaypointQuestsPanel* stCD_DlgWaypointChapterContent::GetQuestsPanel() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_waypoint_quests_panel));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgWaypointQuestsPanel>();
}

stCD_DlgWaypointQuestInfoPanel* stCD_DlgWaypointChapterContent::GetQuestInfoPanel() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_waypoint_quest_info_panel));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgWaypointQuestInfoPanel>();
}

int stCD_DlgWaypointTabChapter::TraWaypointItem(enCD_TraUiShownFlag show_flag, bool* fn_ret, const Fn_TraWaypointItem& fn) const
{
	auto m = GetMap();
	if (!m)
	{
		assert(false);
		return 0;
	}
	return m->TraWaypointItem(show_flag, fn_ret, fn);
}

stCD_DlgWaypointChapterContent* stCD_DlgWaypointTabChapter::GetContent() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_waypoint_chapter_content));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgWaypointChapterContent>();
}

stCD_DlgWaypointMap* stCD_DlgWaypointTabChapter::GetMap() const
{
	auto c = GetContent();
	if (!c)
	{
		assert(false);
		return nullptr;
	}
	return c->GetMap();
}

stCD_DlgWaypointQuestsContainer* stCD_DlgWaypointTabChapter::GetUiQuests() const
{
	auto c = GetContent();
	if (!c)
	{
		assert(false);
		return nullptr;
	}
	auto q = c->GetQuestsPanel();
	if (!q)
	{
		assert(false);
		return nullptr;
	}
	return q->GetUiQuests();
}

int stCD_DlgWaypointTabChapter::TraQuestsItem(bool* fn_ret, const Fn_TraQuestItem& fn) const
{
	auto p = GetUiQuests();
	if (!p)
	{
		assert(false);
		return 0;
	}
	return p->TraQuestItem(fn_ret, fn);
}

int stCD_DlgWaypointMap::TraWaypointItem(enCD_TraUiShownFlag show_flag, bool* fn_ret, const Fn_TraWaypointItem& fn) const
{
	auto c = GetWaypointContainer();
	if (!c)
	{
		assert(false);
		return 0;
	}
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_waypoint_item_vtbl_));
	return c->TraSubUiObj(1, filter, show_flag, [&fn, fn_ret](stCD_UiObjBase* ui){
		if (fn_ret)
		{
			*fn_ret = fn(ui->CastTo<stCD_WaypointPointItem>());
			return *fn_ret;
		}
		return fn(ui->CastTo<stCD_WaypointPointItem>());
	});
}

stCD_UiObjBase* stCD_DlgWaypointMap::GetWaypointContainer() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	assert(res);
	return res;
}

bool stCD_DlgWaypointMap::TheMapHasWaypoint(const stCD_WaypointPointItem& point_item) const
{
	return !!GetTransferWolrdItem(point_item);
}

const stCD_TblItemWorldArea* stCD_DlgWaypointMap::GetTransferWolrdItem(const stCD_WaypointPointItem& point_item) const
{
	return point_item.map_pin_.GetTransferWorldItem(
#ifndef VER3_
		difficulty_
#endif
		);
}

stCD_WaypointPointItem* stCD_DlgWaypointMap::FindWaypoint(enCD_TraUiShownFlag show_flag, const std::wstring& map_name) const
{
	stCD_WaypointPointItem* res = nullptr;
	TraWaypointItem(show_flag, nullptr, [&map_name, &res](stCD_WaypointPointItem* waypoint){
		if (waypoint->EqualMapName(map_name))
		{
			res = waypoint;
			return false;
		}
		return true;
	});
	return res;
}

stCD_BtHidePos* stCD_DlgWaypointMap::GetBtHidePos() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_lvl_up_skill_gem_button), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_BtHidePos>();
}

stCD_EscapeTopUi* stCD_SceneEscapeState::GetTopUi() const
{
	if (IsUnValidPtr(ui_logic_.top_ui_))
	{
		assert(false);
		return nullptr;
	}
	return ui_logic_.top_ui_->CastTo<stCD_EscapeTopUi>();
}

stCD_EscapeMenu* stCD_SceneEscapeState::GetMenu() const
{
	auto top = GetTopUi();
	if (!top)
		return nullptr;
	return top->GetMenu();
}

stCD_UiObjBase* stCD_SceneEscapeState::FindButton(enCD_EscapeMenuButton bt_idx) const
{
	auto m = GetMenu();
	if (!m)
	{
		assert(false);
		return nullptr;
	}
	auto res = m->FindButton(bt_idx);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_UiObjBase* stCD_EscapeTopUi::GetMenuCotainer() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	assert(res);
	return res;
}

stCD_EscapeMenu* stCD_EscapeTopUi::GetMenu() const
{
	auto c = GetMenuCotainer();
	if (!c)
	{
		assert(false);
		return nullptr;
	}
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	auto res = c->FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_EscapeMenu>();
}

stCD_UiObjBase* stCD_EscapeMenu::FindButton(enCD_EscapeMenuButton bt_idx) const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_bt_vtbl_));
	return FindSubUiObj(1, filter, kTUSF_ShowHide, (int)bt_idx);
}

stCD_DlgWaypoint::stWaypointInfo::stWaypointInfo()
{
	ZeroMemory(this, sizeof(*this));
}

stCD_UiNpcPanel* stCD_UiNpcPanelContainer::GetNpcPanel(bool must_shown) const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (must_shown && !res->is_shown_)
	{
		//assert(!"npc menu是隐藏着的");
		return nullptr;
	}
	return res->CastTo<stCD_UiNpcPanel>();
}

stCD_UiNpcDlgChoice2* stCD_UiNpcPanelContainer::GetDlgChoice2() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_dlg_choice2_), kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_UiNpcDlgChoice2>();
}

stCD_UiNpcPanelMenuItemsContainer* stCD_UiNpcPanel::GetMenuItemsContainer(enCD_NpcPanelItemsContainer panel_type) const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_items_container_));
	stCD_UiNpcPanelMenuItemsContainer* res = nullptr;
	TraSubUiObj(1, filter, kTUSF_Shown, [&res, panel_type](stCD_UiObjBase* ui){
		auto c = ui->CastTo<stCD_UiNpcPanelMenuItemsContainer>();
		if (kNPIC_Normal == panel_type)
		{
			if (c->IsNpcMenuPanel())
			{
				res = c;
				return false;
			}
		}
		else if (kNPIC_TalkPanel == panel_type)
		{
			if (c->IsNpcTalkPanel())
			{
				res = c;
				return false;
			}
		}
		return true;
	});
	return res;
}

stCD_UiNpcPanelMenuItems* stCD_UiNpcPanelMenuItemsContainer::GetMenuItems() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiNpcPanelMenuItems>();
}

int stCD_UiNpcPanel::TraMenuItem(enCD_NpcPanelItemsContainer panel_type, const Fn_TraMenuItem& fn) const
{
	auto c = GetMenuItemsContainer(panel_type);
	if (!c)
	{
		//assert(false);
		return 0;
	}
	auto menus = c->GetMenuItems();
	if (!menus)
	{
		assert(false);
		return 0;
	}
	return menus->TraMenuItem(fn);
}

int stCD_UiNpcPanel::TraPressEventParam(enCD_NpcPanelItemsContainer panel_type, const Fn_TraPressEventParam& fn) const
{
	auto c = GetMenuItemsContainer(panel_type);
	if (!c)
	{
		//assert(false);
		return 0;
	}
	return c->TraPressEventParam(fn);
}

const stCD_NpcPanelPressEventParam* stCD_UiNpcPanel::FindPressEventParam(enCD_NpcPanelItemsContainer panel_type, const std::wstring& lable) const
{
	auto c = GetMenuItemsContainer(panel_type);
	if (!c)
	{
		//assert(false);
		return nullptr;
	}
	return c->FindPressEventParam(lable);
}

bool stCD_UiNpcPanel::PressItem(enCD_NpcPanelItemsContainer panel_type, const std::wstring& lable) const
{
	auto param = FindPressEventParam(panel_type, lable);
	if (!param)
	{
		LOG_O(Log_debug) << "无法对话该标签：" << lable;
		//assert(false);
		return false;
	}
	return PressItem(*param);
}

stCD_UiObjBase* stCD_UiNpcPanel::GetTitleLabelContainer() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	assert(res);
	return res;
}

stCD_UiLabel* stCD_UiNpcPanel::GetUiLabel() const
{
	auto c = GetTitleLabelContainer();
	if (!c)
	{
		assert(false);
		return nullptr;
	}
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_label_vtbl_));
	auto res = c->FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiLabel>();
}

bool stCD_UiNpcPanel::EqualTitleLabel(const std::wstring& title) const
{
	auto label = GetUiLabel();
	if (!label)
	{
		assert(false);
		return false;
	}
	return label->EqualLableStr(title);
}

const GameWString* stCD_UiNpcPanel::GetTitle() const
{
	auto label = GetUiLabel();
	if (!label)
	{
		assert(false);
		return nullptr;
	}
	return &label->GetFormatStr();
}

void stCD_UiNpcPanel::DebugInfo() const
{
	__super::DebugInfo();
	auto title = GetTitle();
	if (!title)
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "UiNpcPanelAddr:" << this << ",title:" << *title;
}

void stCD_UiNpcPanel::DebugAddrInfo() const
{
	auto addr = GetMenuItemsContainer(kNPIC_Normal);
	if (addr)
	{
		LOG_O(Log_debug) << "Npc正常Container地址：" << addr;
	}
	addr = GetMenuItemsContainer(kNPIC_TalkPanel);
	if (addr)
	{
		LOG_O(Log_debug) << "Npc对话Container地址：" << addr;
	}
}

void stCD_UiNpcPanel::DebugMenuItemInfo(int type) const
{
	if (type < kNPIC_Normal)
	{
		_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_items_container_));
		TraSubUiObj(1, filter, kTUSF_Shown, [](stCD_UiObjBase* ui_obj){
			auto c = ui_obj->CastTo<stCD_UiNpcPanelMenuItemsContainer>();
			LOG_O(Log_debug) << "UiNpcPanelMenuItemsContainerAddr:" << c;
			auto menus = c->GetMenuItems();
			if (!menus)
			{
				assert(false);
				return false;
			}
			menus->TraMenuItem([](stCD_UiNpcPanelMenuItem* ui_item){
				ui_item->DebugInfo();
				return true;
			});
			return true;
		});
		return;
	}
	stCD_UiNpcPanelMenuItemsContainer* c = nullptr;
	if (kNPIC_Normal == type)
		c = GetMenuItemsContainer(kNPIC_Normal);
	else if (kNPIC_TalkPanel == type)
		c = GetMenuItemsContainer(kNPIC_TalkPanel);
	if (!c)
		return;
	c->DebugFullInfo(kTUSF_ShowHide);
}

int stCD_UiNpcPanelMenuItems::TraMenuItem(const Fn_TraMenuItem& fn) const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_item_vtbl_));
	return TraSubUiObj(1, filter, kTUSF_Shown, [&fn](stCD_UiObjBase* ui){
		return fn(ui->CastTo<stCD_UiNpcPanelMenuItem>());
	});
}

stCD_UiLabel* stCD_UiNpcPanelMenuItem::GetLabel() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_label_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiLabel>();
}

void stCD_UiNpcPanelMenuItem::DebugInfo() const
{
	__super::DebugInfo();
	auto ui_label = GetLabel();
	if (!ui_label)
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "ui_label_Addr:" << ui_label << ",menu_item_label:" << ui_label->GetFormatStr();
}

bool stCD_UiNpcPanelMenuItem::EqualLable(const std::wstring& lable) const
{
	auto ui = GetLabel();
	if (!ui)
	{
		assert(false);
		return false;
	}
	return ui->EqualLableStr(lable);
}

int stCD_UiNpcPanelMenuItemsContainer::TraPressEventParam(const Fn_TraPressEventParam& fn) const
{
	if (!items_info_cont_.IsValid())
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (auto& v : items_info_cont_)
	{
		if (IsUnValidPtr(v.menu_item_))
		{
			assert(false);
			continue;
		}
		if (IsUnValidPtr(v.item_info_))
			continue;
		if (v.menu_item_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_item_vtbl_))
			continue;
		++cnt;
		if (!fn(v))
			break;
	}
	return cnt;
}

void stCD_UiNpcPanelMenuItemsContainer::DebugInfo() const
{
	__super::DebugInfo();
	auto cnt = TraPressEventParam([](const stCD_NpcPanelPressEventParam& info){
		info.DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总共数量：" << cnt;
}

const stCD_NpcPanelPressEventParam* stCD_UiNpcPanelMenuItemsContainer::FindPressEventParam(const std::wstring& lable) const
{
	const stCD_NpcPanelPressEventParam* res = nullptr;
	TraPressEventParam([&res, &lable](const stCD_NpcPanelPressEventParam& param){
		if (!param.menu_item_->EqualLable(lable))
			return true;
		res = &param;
		return false;
	});
	return res;
}

bool stCD_UiNpcPanelMenuItemsContainer::IsNpcMenuPanel() const
{
	return !!FindPressEventParam(CD_kNTL_Bye);
}

bool stCD_UiNpcPanelMenuItemsContainer::IsNpcTalkPanel() const
{
	return !!FindPressEventParam(CD_kNTL_Cotinue);
}

void stCD_NpcPanelPressEventParam::DebugInfo() const
{
	LOG_O(Log_debug) << "NpcPanelPressEventParamAddr:" << this
		<< ",menu_itemAddr:" << menu_item_ << ",menu_infoAddr:" << item_info_
		<< ",mouse_hover_:" << mouse_hover_ << ",ukn_true_:" << ukn_true_;
}

bool stCD_UiLabel::EqualLableStr(const std::wstring& lable_str) const
{
	return lable_str == format_str_.c_str();
}

const GameWString& stCD_UiLabel::GetFormatStr() const
{
	return format_str_;
}

const GameWString& stCD_UiLabel::GetTotalStr() const
{
	return total_str_;
}

stCD_Render* stCD_EntityChest::GetRender() const
{
	return __super::GetRender();
}

void stCD_EntityChest::DebugInfo() const
{
	LOG_O(Log_debug) << "-----------------------------";
	__super::DebugInfo();
	auto chest = GetChest();
	if (chest)
		chest->DebugInfo();
	LOG_O(Log_debug) << "-----------------------------";
}

stCD_EntityAttrChest* stCD_EntityChest::GetChest() const
{
	auto res = FindAttrComponent<stCD_EntityAttrChest>(CD_kAttrInfoChest);
	assert(res);
	return res;
}

void stCD_EntityAttrDbChest::DebugInfo() const
{
	LOG_O(Log_debug) << "EntityAttrDbChestAddr:" << this << ",ukn_b_:" << ukn_b_
		<< ",show_name_:" << show_name_ << ",ukn_b2_:" << ukn_b2_ << ",ukn_b3_:" << ukn_b3_
		<< ",ukn_b4_:" << ukn_b4_ << ",ukn_b5_:" << ukn_b5_ << ",ukn_b6_:" << ukn_b6_
		<< ",ukn_b7_:" << ukn_b7_ << ",ukn_b8_:" << ukn_b8_ << ",ukn_b9_:" << ukn_b9_
		<< ",ukn_dw_:" << ukn_dw_ << ",ukn_dw2_:" << ukn_dw2_;
	if (!db_chest_.IsValid())
	{
		assert(false);
		return;
	}
	db_chest_.tbl_item_->DebugInfo();
}

bool stCD_EntityAttrDbChest::IsQuest() const
{
	if (!db_chest_.IsValid())
	{
		assert(false);
		return false;
	} 
	return !!StdEx::TStrStr(db_chest_.tbl_item_->meta_so_path_, (pt_cwsz)L"QuestChests");
}

bool stCD_EntityAttrDbChest::IsIgnore() const
{
	if (strong_box_.IsValid())
		return false;
	if (!show_name_)
		return true;
	return false;
}

stCD_EntityAttrWorldItem* stCD_EntityWorldItem::GetWorldItem() const
{
	auto res = FindAttrComponent<stCD_EntityAttrWorldItem>(CD_kAttrInfoWorldItem);
	assert(res);
	return res;
}

void stCD_EntityWorldItem::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << "EntityWorldItemAddr:" << this;
	auto attr_world_item = GetWorldItem();
	if (!attr_world_item)
	{
		assert(false);
		return;
	}
	attr_world_item->DebugInfo();
}

stCD_EntityAttrDbWorldItem* stCD_EntityAttrWorldItem::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

stCD_EntityItemBased* stCD_EntityAttrWorldItem::GetItemEntity() const
{
	if (IsUnValidPtr(item_entity_))
	{
		assert(false);
		return nullptr;
	}
	return item_entity_;
}

void stCD_EntityAttrWorldItem::DebugInfo() const
{
	LOG_O(Log_debug) << "EntityAttrWorldItemAddr:" << this << ",attr_db_Addr:" << attr_db_
		<< ",item_entity_Addr:" << item_entity_;
	if (IsUnValidPtr(item_entity_))
	{
		assert(false);
		return;
	}
	item_entity_->DebugInfo();
}

stCD_UiGridPanel* stCD_UiDlgQuestAward::GetGridPanel() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_grid_panel_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiGridPanel>();
}

stCD_Inventory* stCD_UiDlgQuestAward::GetInventory() const
{
	auto panel = GetGridPanel();
	if (!panel)
	{
		assert(false);
		return nullptr;
	}
	return panel->GetInventory();
}

stCD_Inventory* stCD_UiGridPanel::GetInventory() const
{
	if (IsUnValidPtr(inventory_))
	{
		assert(false);
		return nullptr;
	}
	return inventory_;
}

bool stCD_UiGridPanel::CanPutIt(const stCD_EntityItemBased* item, int dst_pos_x, int dst_pos_y) const
{
	auto mgr = inventory_mgr_;
	if (IsUnValidPtr(mgr))
	{
		assert(false);
		return false;
	}
	return mgr->CanPutItToTheInventory(item, inventory_type_, dst_pos_x, dst_pos_y);
}

stCD_UiMouseAttachItem* stCD_UiGridPanel::GetMouseAttachItem() const
{
	if (IsUnValidPtr(mouse_attach_item_))
	{
		assert(false);
		return nullptr;
	}
	return mouse_attach_item_;
}

int stCD_UiGridPanel::TraItemGrids(const Tra_ItemGrid& fn) const
{
	//这里没有判断到g_va_ui_item_grid_，可能会有问题，不过暂时这样吧。
	return TraSubUiObjEx(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_item_grid_), kTUFT_AllSubNodes, kTUSF_Shown, [&fn](stCD_UiObjBase* ui){
		return fn(ui->CastTo<stCD_UiItemGrid>());
	});
}

stCD_UiItemGrid* stCD_UiGridPanel::FindItemGrid(pt_dword item_id) const
{
	stCD_UiItemGrid* res = nullptr;
	TraItemGrids([item_id, &res](stCD_UiItemGrid* ui){
		if (ui->item_id_ == item_id)
		{
			res = ui;
			return false;
		}
		return true;
	});
	assert(res);
	return res;
}

stCD_Vtbl_UiGridPanel* stCD_UiGridPanel::Vtbl() const
{
	if (IsUnValidPtr(vtbl_))
	{
		assert(false);
		return nullptr;
	}
	return vtbl_->CastTo<stCD_Vtbl_UiGridPanel>();
}

stCD_EntityAttrDbChest* stCD_EntityAttrChest::GetDbChest() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

void stCD_EntityAttrChest::DebugInfo() const
{
	LOG_O(Log_debug) << "EntityAttrChestAddr:" << this << ",is_opened_:" << is_opened_;
	if (!IsUnValidPtr(attr_db_))
		attr_db_->DebugInfo();
	else
		assert(false);
}

stCD_Render* stCD_EntityTransitionable::GetRender() const
{
	return __super::GetRender();
}

void stCD_EntityTransitionable::DebugInfo() const
{
	__super::DebugInfo();
}

bool stCD_EntityTransitionable::IsOpened() const
{
	auto attr = GetTransitionable();
	if (!attr)
	{
		assert(false);
		return false;
	}
	return attr->IsOpened();
}

stCD_EntityAttrDbUsable* stCD_EntityAttrUsable::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

stCD_EntityAttrDbCharges* stCD_EntityAttrCharges::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

void stCD_EntityAttrDbMonster::DebugInfo() const
{
	LOG_O(Log_debug) << "-----------------------------";
	LOG_O(Log_debug) << "EntityAttrDbMonsterAddr:" << this;
	if (!variety_.IsValid())
	{
		assert(false);
		return;
	}
	variety_.tbl_item_->DebugInfo();
	if (!variety_.tbl_item_->monster_type_.IsValid())
	{
		assert(false);
		return;
	}
	variety_.tbl_item_->monster_type_.tbl_item_->DebugInfo();
	LOG_O(Log_debug) << "-----------------------------";
}

stCD_TblItemMonsterTypes* stCD_EntityAttrDbMonster::GetMonsterType() const
{
	if (!variety_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	if (!variety_.tbl_item_->monster_type_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	return variety_.tbl_item_->monster_type_.tbl_item_;
}

bool stCD_EntityAttrDbMonster::IsCalled() const
{
	auto monster_type = GetMonsterType();
	if (!monster_type)
		return false;
	return monster_type->is_called_;
}

bool stCD_EntityAttrTransitionable::IsOpened() const
{
	return state_ == kTranState_Opened;
}

bool stCD_EntityAttrNPC::CanMoved() const
{
	return is_locked_ == false && is_locked1_ == false;
}

stCD_UiQuestStatePanelContainer* stCD_UiQuestLvlStatePanel::GetQuestStatePanelContainer() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_quest_state_panel_container));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_UiQuestStatePanelContainer>();
}

stCD_UiLvlStatePanelContainer* stCD_UiQuestLvlStatePanel::GetLvlStatePanelContianer() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_lvl_state_panel_container));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_UiLvlStatePanelContainer>();
}

int stCD_UiQuestLvlStatePanel::TraQuestItem(const Fn_TraQuestItem& fn) const
{
	auto quest_panel_cont = GetQuestStatePanelContainer();
	if (!quest_panel_cont)
		return 0;
	auto quest_panel = quest_panel_cont->GetQuestStatePanel();
	if (!quest_panel)
		return 0;
	return quest_panel->TraQuestItem(fn);
}

void stCD_UiQuestLvlStatePanel::DebugInfo() const
{
	auto p_lvl = GetLvlStatePanelContianer();
	LOG_O(Log_debug) << "UiQuestLvlStatePanelAddr:" << this
		<< ",QuestStatePanelContainerAddr:" << GetQuestStatePanelContainer()
		<< ",LvlStatePanelContianerAddr:" << p_lvl;
	if (p_lvl)
		p_lvl->DebugInfo();
}

int stCD_UiQuestLvlStatePanel::PressAllLvlUpSkillGem() const
{
	auto container = GetLvlStatePanelContianer();
	if (!container)
		return 0;
	auto the_panel = container->GetLvlStatePanel();
	if (!the_panel)
		return 0;
	return the_panel->TraLvlUpSkillGemItem([](stCD_UiLvlUpSkillGemItem* item){
		return item->PressLvlUp();
	});
}

stCD_UiQuestStatePanel* stCD_UiQuestStatePanelContainer::GetQuestStatePanel() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_quest_state_panel));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_UiQuestStatePanel>();
}

int stCD_UiQuestStatePanel::TraQuestItem(const Fn_TraQuestItem& fn) const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	return TraSubUiObj(1, filter, kTUSF_Shown, [&fn](stCD_UiObjBase* ui){
		return fn(ui->CastTo<stCD_UiQuestStateQuestItem>());
	});
}

stCD_UiLabel* stCD_UiQuestStateQuestItem::GetUiLabel() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_label_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiLabel>();
}

bool stCD_UiQuestStateQuestItem::GetQuestName(wchar_t* out_buffer, pt_dword buffer_size) const
{
	assert(out_buffer);
	assert(buffer_size > 0);
	auto ui_label = GetUiLabel();
	if (!ui_label)
	{
		assert(false);
		return false;
	}
	const auto& lable = ui_label->GetTotalStr();
	auto pos = lable.find(L'\n');
	if (pos != lable.npos)
	{
		if (pos * sizeof(wchar_t) + sizeof(wchar_t) > buffer_size)
		{
			assert(false);
			return false;
		}
		memcpy(out_buffer, lable.c_str(), pos * sizeof(wchar_t));
		out_buffer[pos] = 0;
		return true;
	}
	auto len = lable.size() * sizeof(wchar_t) + sizeof(wchar_t);
	if (len > buffer_size)
	{
		assert(false);
		return false;
	}
	memcpy(out_buffer, lable.c_str(), len);
	return true;
}

bool stCD_UiQuestStateQuestItem::GetQuestState(wchar_t* out_buffer, pt_dword buffer_size) const
{
	assert(out_buffer);
	assert(buffer_size > 0);
	auto ui_label = GetUiLabel();
	if (!ui_label)
	{
		assert(false);
		return false;
	}
	const auto& lable = ui_label->GetTotalStr();
	auto pos = lable.find(L'\n');
	const wchar_t* the_begin;
	int size;
	if (pos != lable.npos)
	{
		the_begin = lable.c_str() + pos + 1;
		size = (lable.size() - pos - 1) * sizeof(wchar_t);
		assert(size >= 0);
	}
	else
	{
		size = 0;
	}
	if (size <= 0)
	{
		if (buffer_size < sizeof(wchar_t))
		{
			assert(false);
			return false;
		}
		out_buffer[0] = 0;
		return true;
	}
	if (size + sizeof(wchar_t) > buffer_size)
	{
		assert(false);
		return false;
	}
	memcpy(out_buffer, the_begin, size);
	*(wchar_t*)((char*)out_buffer + size) = 0;
	return true;
}

void stCD_UiQuestStateQuestItem::DebugInfo() const
{
	wchar_t quest_name[100], quest_state[100];
	if (!GetQuestName(quest_name, sizeof(quest_name)))
	{
		assert(false);
		return;
	}
	if (!GetQuestState(quest_state, sizeof(quest_state)))
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "UiQuestStateQuestItemAddr:" << this
		<< ",quest_name:" << (pt_cwsz)quest_name << ",quest_state:" << quest_state;
}

stCD_DlgWaypointQuestsScrollBar* stCD_DlgWaypointQuestsPanel::GetScrollBar() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_waypoint_quests_scroll_bar));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgWaypointQuestsScrollBar>();
}

stCD_UiObjBase* stCD_DlgWaypointQuestsPanel::GetUiQuestsImpl() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_ShowHide, 1);
	assert(res);
	return res;
}

int stCD_DlgWaypointQuestsContainer::TraQuestItem(bool* fn_ret, const Fn_TraQuestItem& fn) const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_waypoint_quest_item));
	return TraSubUiObj(1, filter, kTUSF_ShowHide, [&fn, fn_ret](stCD_UiObjBase* ui){
		if (fn_ret)
		{
			*fn_ret = fn(ui->CastTo<stCD_DlgWaypointQuestItem>());
			return *fn_ret;
		}
		return fn(ui->CastTo<stCD_DlgWaypointQuestItem>());
	});
}

void stCD_DlgWaypointQuestItem::DebugInfo() const
{
	if (!db_quest_.IsValid())
	{
		assert(false);
		return;
	}
	auto map = GetMap();
	if (!map)
	{
		assert(false);
		return;
	}
	auto player = stCD_GameBase::GetMe().GetPlayer();
	if (!player)
	{
		assert(false);
		return;
	}
	stCD_QuestStatesPair quest_state;
	if (!player->FindQuestState(quest_state, db_quest_
#ifndef VER3_
		, map->difficulty_
#endif
		))
	{
		assert(false);
		return;
	}
	if (!quest_state.IsValid())
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "DlgWaypointQuestItemAddr:" << this
		<< ",db_questAddr:" << db_quest_.tbl_item_
		<< ",questName:" << CD_SafeName(db_quest_.tbl_item_->ch_name_)
		<< ",quest_state:" << quest_state.tbl_item_->quest_state_
		<< ",questDesc:" << CD_SafeStr(quest_state.tbl_item_->state_desc_);
}

stCD_DlgWaypointChapterContent* stCD_DlgWaypointQuestItem::GetChapterContent() const
{
	auto c = Parent(3);
	if (!c)
	{
		assert(false);
		return nullptr;
	}
	if (c->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_waypoint_chapter_content))
	{
		assert(false);
		return nullptr;
	}
	return c->CastTo<stCD_DlgWaypointChapterContent>();
}

stCD_DlgWaypointMap* stCD_DlgWaypointQuestItem::GetMap() const
{
	auto c = GetChapterContent();
	if (!c)
		return nullptr;
	return c->GetMap();
}

stCD_EntityAttrDbMonster* stCD_EntityAttrMonster::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

stCD_AttrDbItemBase* stCD_EntityAttrItemBase::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

void stCD_GameLogic::DebugInfo() const
{
	LOG_O(Log_debug) << "GameLogicAddr:" << this << ",world_Addr:" << world_
		<< ",send_packet_Addr:" << send_packet_ << ",sockets_mgr_Addr:" << &sockets_mgr_
		<< ",passive_skill_Addr:" << &passive_skill_ << ",inventory_mgr_Addr:" << &inventory_mgr_;
}

bool stCD_GameLogic::enable_ignore_operate_command = false;

bool stCD_GameLogic::is_self_operate_command = false;

stCD_UiLvlStatePanel* stCD_UiLvlStatePanelContainer::GetLvlStatePanel() const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_));
	auto res = FindSubUiObj(1, filter, kTUSF_Shown, 1);
	if (!res)
		return nullptr;
	return res->CastTo<stCD_UiLvlStatePanel>();
}

void stCD_UiLvlStatePanelContainer::DebugInfo() const
{
	auto the_panel = GetLvlStatePanel();
	if (the_panel)
		the_panel->DebugInfo();
}

int stCD_UiLvlStatePanel::TraLvlUpSkillGemItem(const Fn_TraLvlUpSkillGemItem& fn) const
{
	_stCD_UiFilterVtbl filter((pt_pcvoid)CD_EXE_DATA(g_va_ui_lvl_up_skill_gem_item));
	int cnt = 0;
	TraSubUiObj(1, filter, kTUSF_Shown, [&fn, &cnt](stCD_UiObjBase* ui){
		auto ui_item = ui->CastTo<stCD_UiLvlUpSkillGemItem>();
		if (!ui_item)
		{
			assert(false);
			return false;
		}
		if (!ui_item->CanLvlUpSkillGem())
			return true;
		++cnt;
		return fn(ui_item);
	});
	return cnt;
}

void stCD_UiLvlStatePanel::DebugInfo() const
{
	auto cnt = TraLvlUpSkillGemItem([](stCD_UiLvlUpSkillGemItem* ui){
		ui->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "UiLvlUpSkillItem数量：" << cnt;
}

stCD_UiObjBase* stCD_UiLvlUpSkillGemItem::GetButton() const
{
	return FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_lvl_up_skill_gem_button), kTUSF_Shown, 1);
}

void stCD_UiLvlUpSkillGemItem::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << "ButtonAddr:" << GetButton();
}

void stCD_UiLvlUpSkillGemItem::ClearLvlUpInfo()
{
	lvl_up_info_.clear();
}

void stCD_UiLvlUpSkillGemItem::AddLvlUpInfo(const std::wstring& item_name, pt_dword max_lvl)
{
	if (item_name.empty())
	{
		assert(false);
		return;
	}
	for (auto& v : lvl_up_info_)
	{
		if (v.item_name_ == item_name)
		{
			assert(false);
			return;
		}
	}
	lvl_up_info_.push_back({ item_name, max_lvl });
}

stCD_UiLvlUpSkillGemItem::LvlUpInfo stCD_UiLvlUpSkillGemItem::lvl_up_info_;

stCD_AttrDbSkillGem* stCD_EntityAttrSkillGem::GetAttrDb() const
{
	if (IsUnValidPtr(attr_db_))
	{
		assert(false);
		return nullptr;
	}
	return attr_db_;
}

stCD_TblItemGrantedEffectsPerLevel* stCD_EntityAttrSkillGem::GetDbPerLevel() const
{
	auto attr_db = GetAttrDb();
	if (!attr_db)
	{
		assert(false);
		return nullptr;
	}
	assert(cur_lvl_ > 0 && cur_lvl_ < 1000);
	return attr_db->GetDbPerLevelByLevel(cur_lvl_);
}

pt_dword stCD_EntityAttrSkillGem::GetRequireLvl() const
{
	auto db_per_lvl = GetDbPerLevel();
	if (!db_per_lvl)
	{
		assert(false);
		return 0;
	}
	return db_per_lvl->require_lvl_;
}

stCD_MiniMap* stCD_UiMap::GetMiniMap() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_mini_map_), kTUSF_ShowHide, 2);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_MiniMap>();
}

stCD_BigMap* stCD_UiMap::GetBigMap() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_mini_map_), kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_BigMap>();
}

stCD_UiObjBase* stCD_UiMap::GetMapInfo() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_), kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_UiObjBase* stCD_UiEntityDescTop::GetSubContainer(int idx) const
{
	return FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_Shown, idx);
}

stCD_UiEntityDescContainer* stCD_UiEntityDescTop::GetEntityDescContainer() const
{
	auto res = GetSubContainer(1);
	if (!res) return nullptr;
	return res->CastTo<stCD_UiEntityDescContainer>();
}

int stCD_UiEntityDescContainer::TraWorldItems(const Fn_TraSubUiObj& fn) const
{
	return TraSubUiObjEx(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_world_item_desc_), kTUFT_ByVtbl, kTUSF_Shown, fn);
}

stCD_UiObjBase* stCD_UiStashTop::GetTopContainer() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_), kTUSF_Shown, 1);
	assert(res);
	return res;
}

stCD_UiStashTop::UiTabPages* stCD_UiStashTop::GetTabPages() const
{
	auto ui = GetTopContainer();
	if (!ui) return nullptr;
	ui = ui->FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_stash_tab_pages_), kTUSF_Shown, 1);
	if (!ui) return nullptr;
	return ui->CastTo<UiTabPages>();
}

stCD_UiObjBase* stCD_UiTabPages::GetPageButtons() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_Shown, 1);
	assert(ui);
	return ui;
}

stCD_UiObjBase* stCD_UiTabPages::GetPagesContainer() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_Shown, 2);
	assert(ui);
	return ui;
}

int stCD_UiTabPages::GetPagesCount() const
{
	if (!pages_.IsValid())
	{
		assert(false);
		return false;
	}
	return (int)pages_.size();
}

int stCD_UiTabPages::TraPages(const Fn_TraPage& fn) const
{
	int cnt = 0;
	for (auto& v : pages_)
	{
		if (!v.IsValid())
			continue;
		++cnt;
		if (!fn(v))
			break;
	}
	return cnt;
}

const stCD_UiTabPages::PageInfo* stCD_UiTabPages::GetCurPage() const
{
	if (cur_page_idx_ < 0 || cur_page_idx_ >= pages_.size())
	{
		assert(false);
		return nullptr;
	}
	auto& res = pages_[cur_page_idx_];
	if (!res.IsValid())
	{
		assert(false);
		return nullptr;
	}
	return &res;
}

const stCD_UiTabPages::PageInfo* stCD_UiTabPages::FindPageInfo(const stCD_UiObjBase* tab_button) const
{
	for (auto& v : pages_)
	{
		if (v.tab_button_ == tab_button)
			return &v;
	}
	assert(false);
	return nullptr;
}

static stCD_UiGridPanel* __FindGridPanelImpl__(const stCD_UiObjBase* ui_this, enCD_InventoryType inventory_type)
{
	_stCD_UiFilterMultiVtbl filter(CD_EXE_DATA(g_va_ui_dlg_prop_grid_panel_), CD_EXE_DATA(g_va_ui_flasks_grid_panel_));
	stCD_UiGridPanel* res = nullptr;
	ui_this->TraSubUiObj(1, filter, kTUSF_ShowHide, [inventory_type, &res](stCD_UiObjBase* ui){
		auto grid_panel = ui->CastTo<stCD_UiGridPanel>();
		if (grid_panel->inventory_type_ == inventory_type)
		{
			res = grid_panel;
			return false;
		}
		return true;
	});
	assert(res);
	return res;
}

stCD_UiGridPanel* stCD_DlgProp::FindGridPanel(enCD_InventoryType inventory_type) const
{
#ifdef VER3_
	auto ui = GetUiInventories();
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->FindGridPanel(inventory_type);
#else
	return __FindGridPanelImpl__(this, inventory_type);
#endif
}

#ifdef VER3_
stCD_DlgPropInventories* stCD_DlgProp::GetUiInventories() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_ShowHide, 2);
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->CastTo<stCD_DlgPropInventories>();
}
#endif

stCD_DlgBuy::UiTabPages* stCD_DlgBuy::GetTabPages() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_stash_tab_pages_), kTUSF_Shown, 1);
	if (!res) return nullptr;
	return res->CastTo<UiTabPages>();
}

stCD_UiCheckBox* stCD_DlgBuy::GetBindCheckBox() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_check_box_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiCheckBox>();
}

bool stCD_DlgBuy::EnableUseBind(bool enable)
{
	auto ui = GetBindCheckBox();
	if (!ui)
	{
		assert(false);
		return false;
	}
	return ui->Enable(enable);
}

stCD_UiSellContainer* stCD_DlgSell::GetTabPagesContainer() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_sell_tab_pages_container_), kTUSF_Shown, 1);
	if (!res) return nullptr;
	return res->CastTo<stCD_UiSellContainer>();
}

bool stCD_DlgSell::PressButton(bool accept) const
{
	auto c = GetTabPagesContainer();
	if (!c)
	{
		assert(false);
		return false;
	}
	return c->PressButton(accept);
}

stCD_UiGridPanel* stCD_UiSellContainer::GetMyItemPage() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_sell_my_items_page_), kTUSF_Shown, 1);
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->CastTo<stCD_UiGridPanel>();
}

stCD_UiGridPanel* stCD_UiSellContainer::GetNpcItemPage() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_sell_npc_items_page_), kTUSF_Shown, 1);
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->CastTo<stCD_UiGridPanel>();
}

stCD_UiObjBase* stCD_UiSellContainer::GetButtonAccept() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_bt_vtbl_), kTUSF_Shown, 1);
	assert(ui);
	return ui;
}

stCD_UiObjBase* stCD_UiSellContainer::GetButtonCancel() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_bt_vtbl_), kTUSF_Shown, 2);
	assert(ui);
	return ui;
}

stCD_UiStashSingleGridPanel* stCD_UiStashTabPageItem::GetSingleContainer() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_tab_page_item_), kTUSF_Shown, 1);
	if (!res) return nullptr;
/*
#ifdef _DEBUG
	if (!res)
		LOG_O(Log_trace) << "难道是第一次打开仓库该页面？";
#endif*/
	return res->CastTo<stCD_UiStashSingleGridPanel>();
}

stCD_UiGridPanel* stCD_UiStashTabPageItem::GetGridPanel() const
{
	auto ui = GetSingleContainer();
	if (!ui) return nullptr;
	return ui->GetGridPanel();
}

stCD_UiStashMultiGridPanel* stCD_UiStashTabPageItem::GetMultiContainer(enCD_TraUiShownFlag show_flag) const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_stash_multi_container_), show_flag, 1);
	if (!ui) return nullptr;
	return ui->CastTo<stCD_UiStashMultiGridPanel>();
}

stCD_UiGridPanel* stCD_UiStashSingleGridPanel::GetGridPanel() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_prop_grid_panel_), kTUSF_Shown, 1);
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->CastTo<stCD_UiGridPanel>();
}

stCD_UiGridPanel* stCD_UiBuyTabPageItem::GetGridPanel() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_grid_panel_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiGridPanel>();
}

stCD_AttrDbItemBase* stCD_ItemPrice::GetDbItemBase() const
{
	if (IsUnValidPtr(db_item_))
	{
		assert(false);
		return nullptr;
	}
	auto attr_db = db_item_->FindAttrDb<stCD_AttrDbItemBase>(CD_kAttrInfoBase);
	assert(attr_db);
	return attr_db;
}

stCD_UiPreInGameTop* stCD_ScenePreGameState::GetTopUi() const
{
	if (IsUnValidPtr(ui_logic_.top_ui_))
	{
		assert(false);
		return nullptr;
	}
	return ui_logic_.top_ui_->CastTo<stCD_UiPreInGameTop>();
}

bool stCD_ScenePreGameState::CloseDlgModal()
{
	auto dlg_modal = GetDlgModal();
	if (!dlg_modal)
		return true;
	if (!dlg_modal->is_shown_)
		return true;
	dlg_modal->Show(false);
	cur_shown_dlg_modal_ = nullptr;

	return CloseDarkPanel();
}

stCD_DlgInteraction* stCD_ScenePreGameState::GetDlgModal() const
{
	if (!cur_shown_dlg_modal_)
		return nullptr;
	if (IsUnValidPtr(cur_shown_dlg_modal_))
	{
		assert(false);
		return nullptr;
	}
	if (cur_shown_dlg_modal_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_passive_skill_warning_panel))
	{
		assert(false);
		return nullptr;
	}
	if (cur_shown_dlg_modal_ != dlg_modal_)
	{
		assert(false);
		return nullptr;
	}
	return cur_shown_dlg_modal_;
}

stCD_UiObjBase* stCD_ScenePreGameState::GetDarkPanel() const
{
	if (!modal_dark_panel_)
		return nullptr;
	if (IsUnValidPtr(modal_dark_panel_))
	{
		assert(false);
		return nullptr;
	}
	if (modal_dark_panel_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_))
	{
		assert(false);
		return nullptr;
	}
	return modal_dark_panel_;
}

bool stCD_ScenePreGameState::CloseDarkPanel() const
{
	auto panel = GetDarkPanel();
	if (!panel)
		return true;
	if (!panel->is_shown_)
		return true;
	panel->Show(false);
	return true;
}

void stCD_ScenePreGameState::DebugInfo() const
{
	__super::DebugInfo();
	auto dlg = GetDlgModal();
	if (dlg)
		dlg->DebugInfo();
}

bool stCD_ScenePreGameState::SelectCharacter(enCD_ProfessionType profession_type) const
{
	if (IsUnValidPtr(character_info_))
	{
		assert(false);
		return false;
	}
	return character_info_->SelectCharacter(profession_type);
}

stCD_DlgSelectRole* stCD_UiPreInGameTop::GetDlgSelectRole() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_ShowHide, 3);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgSelectRole>();
}

stCD_DlgCreateRole* stCD_UiPreInGameTop::GetDlgCreateRole() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_ShowHide, 4);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_DlgCreateRole>();
}

stCD_UiSelectRoleTop* stCD_DlgSelectRole::GetTopUi() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_select_role_top_), kTUSF_ShowHide, 1);
	if (!res)
	{
		//assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectRoleTop>();
}

void stCD_DlgSelectRole::DebugInfo() const
{
	LOG_O(Log_debug) << "DlgSelectRoleAddr:" << this;
	auto p = GetTopUi();
	if (p)
		p->DebugInfo();
}

stCD_UiSelectRoleContainerTop* stCD_UiSelectRoleTop::GetContainerTop() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectRoleContainerTop>();
}

void stCD_UiSelectRoleTop::DebugInfo() const
{
	LOG_O(Log_debug) << "UiSelectRoleTopAddr:" << this;
	auto p = GetContainerTop();
	if (p)
		p->DebugInfo();
}

int stCD_UiSelectRoleTop::TraRoleItems(const Fn_TraRoleItem& fn) const
{
	if (IsUnValidPtr(role_list_))
	{
		assert(false);
		return 0;
	}
	return role_list_->TraRoleItems(fn);
}

stCD_UiSelectRoleContainer* stCD_UiSelectRoleContainerTop::GetContainer() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectRoleContainer>();
}

void stCD_UiSelectRoleContainerTop::DebugInfo() const
{
	LOG_O(Log_debug) << "UiSelectRoleContainerTopAddr:" << this;
	auto p = GetContainer();
	if (p)
		p->DebugInfo();
}

stCD_UiSelectRoleUpPart* stCD_UiSelectRoleContainer::GetUpPart() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectRoleUpPart>();
}

void stCD_UiSelectRoleContainer::DebugInfo() const
{
	LOG_O(Log_debug) << "SelectRoleContainerAddr:" << this;
	auto p = GetUpPart();
	if (p)
		p->DebugInfo();
}

stCD_UiSelectRoleUpContent* stCD_UiSelectRoleUpPart::GetContent() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectRoleUpContent>();
}

void stCD_UiSelectRoleUpPart::DebugInfo() const
{
	LOG_O(Log_debug) << "UiSelectRoleUpPartAddr:" << this;
	auto p = GetContent();
	if (p)
		p->DebugInfo();
}

stCD_UiSelectRoleList* stCD_UiSelectRoleUpContent::GetRoleList() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_select_role_list_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectRoleList>();
}

stCD_UiSelectRoleOper* stCD_UiSelectRoleUpContent::GetRoleOper() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_Shown, 3);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectRoleOper>();
}

void stCD_UiSelectRoleUpContent::DebugInfo() const
{
	LOG_O(Log_debug) << "UiSelectRoleUpContentAddr:" << this;
	auto l = GetRoleList();
	if (l)
		l->DebugInfo();
	auto p = GetRoleOper();
	if (p)
		p->DebugInfo();
}

void stCD_UiSelectRoleList::DebugInfo() const
{
	LOG_O(Log_debug) << "SelectRoleListAddr:" << this << ",当前选择的角色索引：" << cur_selected_;
	auto cnt = TraRoleItems([](stCD_stCD_UiRoleListItem* item){
		item->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "角色数量：" << cnt;
}

stCD_stCD_UiSelectRoleListItems* stCD_UiSelectRoleList::GetUiListItems() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_stCD_UiSelectRoleListItems>();
}

int stCD_UiSelectRoleList::TraRoleItems(const Fn_TraRoleItem& fn) const
{
	if (!list_items_.IsValid())
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (auto& v : list_items_)
	{
		if (IsUnValidPtr(v.real_obj_))
		{
			assert(false);
			continue;
		}
		if (IsUnValidPtr(*v.real_obj_))
		{
		}
		++cnt;
		if (!fn(*v.real_obj_))
			break;
	}
	return cnt;
}

bool stCD_UiSelectRoleList::SetCurSelectItem(const stCD_stCD_UiRoleListItem* item)
{
	int idx = 0;
	for (auto& v : list_items_)
	{
		if (IsUnValidPtr(v.real_obj_))
		{
			assert(false);
			continue;
		}
		if (*v.real_obj_ != item)
		{
			++idx;
			continue;
		}
		if (cur_selected_ == idx)
			return true;
		cur_selected_ = idx;
		return true;
	}
	assert(false);
	return false;
}

void stCD_UiSelectRoleOper::DebugInfo() const
{
	LOG_O(Log_debug) << "SelectRoleOper:" << this;
}

int stCD_stCD_UiSelectRoleListItems::TraRoleItems(const Fn_TraRoleItem& fn) const
{
	return TraSubUiObjEx(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_role_item_), kTUFT_ByVtbl, kTUSF_Shown, [&fn](stCD_UiObjBase* ui){
		return fn(ui->CastTo<stCD_stCD_UiRoleListItem>());
	});
}

std::string stCD_stCD_UiRoleListItem::GetSerArea() const
{
	return WideCharToGbk(CD_SafeName(ser_area_.c_str()));
}

std::string stCD_stCD_UiRoleListItem::GetRoleName() const
{
	return WideCharToGbk(CD_SafeName(role_name_.c_str()));
}

void stCD_stCD_UiRoleListItem::DebugInfo() const
{
	LOG_O(Log_debug) << "UiRoleListItemAddr:" << this
		<< ",SerArea:" << GetSerArea() << ",RoleName:" << GetRoleName();
}

bool stCD_TabPageInfo::IsValid() const
{
	if (IsUnValidPtr(page_item_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(tab_button_))
	{
		assert(false);
		return false;
	}
	return true;
}

stCD_TabPageItemBase* stCD_TabPageInfo::GetPageItem() const
{
	if (IsUnValidPtr(page_item_))
	{
		assert(false);
		return nullptr;
	}
	return page_item_;
}

stCD_UiPassiveSkillTop* stCD_DlgPassiveSkill::GetUiTop() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_passive_skill_top_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiPassiveSkillTop>();
}

stCD_UiSelectAreaTop* stCD_DlgCreateRole::GetSelectAreaTop() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_select_area_top_), kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectAreaTop>();
}

stCD_UiCreateRoleTop* stCD_DlgCreateRole::GetCreateRoleTop() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_item_vtbl_), kTUSF_ShowHide, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiCreateRoleTop>();
}

stCD_UiSelectAreas* stCD_UiSelectAreaTop::GetUiAreas() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_item_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectAreas>();
}

stCD_UiSelectAreaOpers* stCD_UiSelectAreaTop::GetUiOpers() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_item_vtbl_), kTUSF_Shown, 2);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiSelectAreaOpers>();
}

stCD_UiEdit* stCD_SceneCreateCharacterState::GetNameEdit() const
{
	if (IsUnValidPtr(name_edit_))
	{
		assert(false);
		return nullptr;
	}
	if (name_edit_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_create_role_name_edit_))
	{
		assert(false);
		return nullptr;
	}
	return name_edit_;
}

bool stCD_SceneCreateCharacterState::InputRoleName(const std::wstring& role_name)
{
	auto name_edit = GetNameEdit();
	if (!name_edit)
	{
		assert(false);
		return false;
	}
	return name_edit->edit_string_.SetString(role_name);
}

bool stCD_SceneCreateCharacterState::SelectGameArea(const stCD_GameArea& game_area)
{
	pt_dword idx = 0;
	for (auto& area : game_areas_)
	{
		if (&area == &game_area)
			goto LABEL_FIND;
		++idx;
	}
	assert(false);
	return false;
LABEL_FIND:
	cur_area_idx_ = idx;
	return true;
}

void stCD_SceneCreateCharacterState::DebugInfo() const
{
	LOG_O(Log_debug) << "CreateCharacterStateAddr:" << this;
}

bool stCD_SceneCreateCharacterState::IsSelectCharacterAtBottom() const
{
	if (!ui_select_character_at_bottom_)
		return false;
	if (IsUnValidPtr(ui_select_character_at_bottom_))
	{
		assert(false);
		return false;
	}
	if (ui_select_character_at_bottom_->vtbl_ != (void*)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_))
	{
		assert(false);
		return false;
	}
	if (!ui_select_character_at_bottom_->is_shown_)
		return false;
	//如果不是在创建角色界面，则子ui数量为0.
	if (ui_select_character_at_bottom_->GetSubUiCount() > 0)
		return true;
	return false;
}

bool stCD_EditString::SetString(const std::wstring& new_str)
{
	if (new_str.empty())
	{
		string_.clear();
		return true;
	}
	string_.assign(new_str.begin(), new_str.end());
	return true;
}

stCD_UiCallerBuff* stCD_UiBuff::GetCallerBuff() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_caller_buff_), kTUSF_Shown, 1);
	if (!res) return nullptr;
	return res->CastTo<stCD_UiCallerBuff>();
}

int stCD_UiBuff::TraCallerBuff(const Fn_TraCallerBuff& fn) const
{
	int cnt = 0;
	auto ui_buff = GetCallerBuff();
	if (!ui_buff)
		return cnt;
	if (ui_buff->buff_items_.empty())
		return cnt;
	auto player = stCD_GameBase::GetMe().GetPlayerEntity();
	if (!player)
	{
		assert(false);
		return cnt;
	}
	auto actor = player->GetActor();
	if (!actor)
	{
		assert(false);
		return cnt;
	}
	auto& skill_mgr = actor->GetMySkillMgr();
	for (auto& v : ui_buff->buff_items_)
	{
		if (IsUnValidPtr(v.buff_item_))
		{
			assert(false);
			return cnt;
		}
		auto skill_info = skill_mgr.FindSkill(v.skill_id_);
		if (!skill_info)
		{
			assert(false);
			continue;
		}
		++cnt;
		if (!fn(skill_info, v.buff_item_))
			break;
	}
	return cnt;
}

stCD_UiBuffItemsContainer* stCD_UiCallerBuff::GetBuffContainer() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_npc_menu_item_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiBuffItemsContainer>();
}

int stCD_UiBuffItemsContainer::TraUiBuffItem(const Fn_TraUiBuffItem& fn) const
{
	return TraSubUiObjEx(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_buff_item_), kTUFT_ByVtbl, kTUSF_Shown, [&fn](stCD_UiObjBase* ui){
		return fn(ui->CastTo<stCD_UiBuffItem>());
	});
}

stCD_UiBuffItemContainer* stCD_UiBuffItem::GetContainer() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_popup_menu_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiBuffItemContainer>();
}

void stCD_UiBuffItem::DebugInfo() const
{
	LOG_O(Log_debug) << "UiBuffItemAddr:" << this << ",实体ID数量：" << entity_ids_.size();
}

stCD_UiBuffNum* stCD_UiBuffItemContainer::GetUiBuffNum() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiBuffNum>();
}

stCD_UiLabel* stCD_UiBuffNum::GetBuffLabel() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_label_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiLabel>();
}

stCD_SkillAttrObj::stCD_SkillAttrObj()
{
	ZeroMemory(this, sizeof(*this));
}

bool stCD_SkillAttrObj::IsValid() const
{
	return vtbl_ && attr_info_ && db_stats_;
}

stCD_UiObjBase* stCD_DlgMapLocator::GetBtClose() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_lvl_up_skill_gem_button), kTUSF_Shown, 1);
	assert(res);
	return res;
}

stCD_UiObjBase* stCD_DlgMapLocator::GetBtStart() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_bt_vtbl_), kTUSF_Shown, 1);
	assert(res);
	return res;
}

stCD_UiObjBase* stCD_DlgMapLocator::GetBtYiJieTuJian() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_bt_vtbl_), kTUSF_Shown, 2);
	assert(false);
	return res;
}

stCD_UiGridPanel* stCD_DlgMapLocator::GetUiGridPanel() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_prop_grid_panel_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiGridPanel>();
}

stCD_ItemTipsParams& stCD_ItemTipsParams::GetMe()
{
	return *(stCD_ItemTipsParams*)(void*)CD_EXE_DATA(g_a_item_tips_params_);
}

stCD_UiObjBase* stCD_UiBottom::GetLeftBottomFlasks() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_ShowHide, 2);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!res->is_shown_)
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_UiFlasksMgr* stCD_UiBottom::GetFlasksMgr() const
{
	auto ui_up = GetLeftBottomFlasks();
	if (!ui_up)
		return nullptr;
	auto res = ui_up->FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_bag_flasks_up_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiFlasksMgr>();
}

stCD_UiObjBase* stCD_UiBottom::GetFlasks() const
{
	auto ui_up = GetFlasksMgr();
	if (!ui_up)
		return nullptr;
	auto res = ui_up->FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_bag_flasks_), kTUSF_Shown, 1);
	assert(res);
	return res;
}

stCD_UiBottomFlask* stCD_UiBottom::GetFirstFlask() const
{
	auto ui_up = GetFlasks();
	if (!ui_up)
		return nullptr;
	auto res = ui_up->FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_bag_flask_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiBottomFlask>();
}

stCD_UiItemTips* stCD_UiBottom::GetFirstTips() const
{
	auto res = GetFirstFlask();
	if (!res)
		return nullptr;
	return res->GetTips();
}

stCD_UiHpMpCircle* stCD_UiBottom::GetMpCircle() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_hp_mp_circle_), kTUSF_Shown, 2);
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->CastTo<stCD_UiHpMpCircle>();
}

stCD_UiHpMpCircle* stCD_UiBottom::GetHpCircle() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_hp_mp_circle_), kTUSF_Shown, 1);
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->CastTo<stCD_UiHpMpCircle>();
}

void stCD_UiBottom::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << "Hp:";
	auto hp = GetHpCircle();
	if (hp)
		hp->DebugInfo();
	LOG_O(Log_debug) << "Mp:";
	auto mp = GetMpCircle();
	if (mp)
		mp->DebugInfo();
}

stCD_UiButton* stCD_UiBottom::GetBtSkipAllTutorials() const
{
	stCD_UiButton* res = nullptr;
	TraSubUiObjEx(1, (void*)CD_EXE_DATA(g_a_ui_bt_vtbl_), kTUFT_ByVtbl, kTUSF_ShowHide, [&res](stCD_UiObjBase* ui){
		auto bt = ui->CastTo<stCD_UiButton>();
		assert(bt);
		if (bt->EqualTitle(CD_kBtTitle_SkillAllTutorials))
		{
			res = bt;
			return false;
		}
		return true;
	});
	if (res && res->is_shown_)
		return res;
	return nullptr;
}

stCD_UiItemTips* stCD_UiItemTipsUp::GetItemTips() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_item_tips_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiItemTips>();
}

stCD_UiItemTipsUp* stCD_UiBottomFlask::GetTipsUp() const
{
	if (IsUnValidPtr(tips_up_))
	{
		assert(false);
		return nullptr;
	}
	return tips_up_;
}

stCD_UiItemTips* stCD_UiBottomFlask::GetTips() const
{
	if (IsUnValidPtr(tips_))
	{
		assert(false);
		return nullptr;
	}
	return tips_;
}

int stCD_UiStashMultiGridPanel::TraGridPanel(const Fn_TraGridPanel& fn) const
{
	return TraSubUiObjEx(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_stash_multi_grid_panel_item_), kTUFT_ByVtbl, kTUSF_Shown,
		[&fn](stCD_UiObjBase* ui){
		return fn(ui->CastTo<stCD_UiGridPanel>());
	});
}

stCD_UiGridPanel* stCD_UiStashMultiGridPanel::FindGridPanel(int idx) const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_stash_multi_grid_panel_item_), kTUSF_Shown, idx + 1);
	if (!ui)
		return nullptr;
	return ui->CastTo<stCD_UiGridPanel>();
}

void stCD_AttrDbRender::DebugInfo() const
{
	LOG_O(Log_debug) << "AttrDbRenderAddr:" << this << ",UknDw:" << ukn_dw_ << ",UknDw1:" << ukn_dw1_
		<< ",name:" << name_ << ",ukn_str:" << ukn_str_ << ",ukn_dw2_:" << ukn_dw2_ << ",ukn_w_:" << ukn_w_;
}

stCD_UiLabel* stCD_UiCheckBox::GetUiLabel() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_label_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiLabel>();
}

stCD_MapLocatorMapAttrList* stCD_DlgSelfMapLocator::GetMapAttrList() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_map_attr_list_), kTUSF_Shown, 1);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_MapLocatorMapAttrList>();
}

stCD_MapLocatorLocator* stCD_DlgSelfMapLocator::GetLocator() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_a_ui_dlg_tab_vtbl_), kTUSF_Shown, 2);
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_MapLocatorLocator>();
}

void stCD_DlgSelfMapLocator::DebugInfo() const
{
	auto p = GetMapAttrList();
	if (p)
		p->DebugInfo();
}

stCD_UiGridPanel* stCD_DlgSelfMapLocator::GetUiGridPanel() const
{
	if (IsUnValidPtr(grid_panel_))
	{
		assert(false);
		return nullptr;
	}
	if (grid_panel_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_dlg_self_map_locator_grid_panel_))
	{
		assert(false);
		return nullptr;
	}
	return grid_panel_;
}

void stCD_MapLocatorMapAttrList::stListItem::DebugInfo() const
{
	LOG_O(Log_debug) << "stListItemAddr:" << this << ",UiItemAddr:" << ui_item_;
	if (data_.IsValid())
		data_.tbl_item_->DebugInfo();
	else
		assert(false);
}

int stCD_MapLocatorMapAttrList::TraListItem(const Fn_TraListItem& fn) const
{
	int cnt = 0;
	for (auto& v : list_items_)
	{
		if (IsUnValidPtr(v.real_obj_))
		{
			assert(false);
			continue;
		}
		++cnt;
		if (!fn(v.real_obj_))
			break;
	}
	return cnt;
}

void stCD_MapLocatorMapAttrList::DebugInfo() const
{
	LOG_O(Log_debug) << "MapLocatorMapAttrListAddr:" << this << ",PtrThis:" << ptr_this_
		<< ",CurSelected:" << cur_selected_;
	auto cnt = TraListItem([](stListItem* item){
		item->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总数：" << cnt;
}

pt_dword stCD_UiEventGetterAddr::get()
{
	return CD_EXE_DATA(g_c_main_ui_obj_handle_logic_);
}

pt_dword stCD_UiEventGetterAddr1::get()
{
	return CD_EXE_DATA(g_c_main_ui_obj_handle_logic_ret_c_);
}

void stCD_MapDataTerrainModel::stTerrainObjInfo::DebugInfo() const
{
	if (IsUnValidPtr(obj_positioned_))
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "stTerrainObjInfoAddr:" << this << ",ukn_byte_:" << (pt_pcvoid)ukn_byte_;
	LOG_O(Log_debug) << "---------------------------------------";
	if (IsUnValidPtr(obj_positioned_->entity_))
	{
		assert(false);
		return;
	}
	obj_positioned_->entity_->DebugInfo();
	obj_positioned_->DebugInfo();
	LOG_O(Log_debug) << "---------------------------------------";
}

stCD_UiEdit* stCD_SceneLoginState::GetEditAc() const
{
	if (IsUnValidPtr(edit_ac_))
	{
		assert(false);
		return nullptr;
	}
	if (edit_ac_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_create_role_name_edit_))
	{
		assert(false);
		return nullptr;
	}
	return edit_ac_;
}

stCD_UiEdit* stCD_SceneLoginState::GetEditPwd() const
{
	if (IsUnValidPtr(edit_pwd_))
	{
		assert(false);
		return nullptr;
	}
	if (edit_pwd_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_create_role_name_edit_))
	{
		assert(false);
		return nullptr;
	}
	return edit_pwd_;
}

bool stCD_SceneLoginState::InputLoginInfo(const std::wstring& ac, const std::wstring& pwd, const std::wstring& gateway)
{
	for (auto& v : gateway_info_)
	{
		if (gateway == v.name_.c_str())
		{
			goto LABLE_HERE;
		}
	}
	assert(false);
	return false;
LABLE_HERE:
	auto edit_ac = GetEditAc();
	if (!edit_ac)
	{
		assert(false);
		return false;
	}
	auto edit_pwd = GetEditPwd();
	if (!edit_pwd)
	{
		assert(false);
		return false;
	}
	auto edit_gateway = GetEditGateway();
	if (!edit_gateway)
	{
		assert(false);
		return false;
	}
	if (!edit_ac->SetString(ac))
	{
		assert(false);
		return false;
	}
	if (!edit_pwd->SetString(pwd))
	{
		assert(false);
		return false;
	}
	if (CD_AutoSelectGateway != gateway)
	{
		if (!edit_gateway->SetString(gateway))
		{
			assert(false);
			return false;
		}
	}
	return true;
}

stCD_UiEdit* stCD_SceneLoginState::GetEditGateway() const
{
	if (IsUnValidPtr(gateway_))
	{
		assert(false);
		return nullptr;
	}
	return gateway_->GetEditGateway();
}

void stCD_SceneLoginState::DebugInfo() const
{
	__super::DebugInfo();
	for (auto& v : gateway_info_)
	{
		v.DebugInfo();
	}
	LOG_O(Log_trace) << "网关数量：" << gateway_info_.size();
}

stCD_UiEdit* stCD_UiLoginGateway::GetEditGateway() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_create_role_name_edit_), kTUSF_Shown, 1);
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->CastTo<stCD_UiEdit>();
}

bool stCD_UiEdit::SetString(const std::wstring& new_str)
{
	return edit_string_.SetString(new_str);
}

void stCD_GatewayInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "GatewayInfoAddr:" << this << ",ShortName:" << short_name_ << ",Name:" << name_;
}

void stCD_DlgInteraction::DebugInfo() const
{
	__super::DebugInfo();
	auto title = GetUiTitle();
	if (title)
		LOG_O(Log_debug) << "title:" << title->GetTotalStr();
	auto content = GetUiContent();
	if (content)
		LOG_O(Log_debug) << "content:" << content->GetTotalStr();
}

stCD_UiLabel* stCD_DlgInteraction::GetUiTitle() const
{
	if (IsUnValidPtr(dlg_title_))
	{
		assert(false);
		return nullptr;
	}
	if (dlg_title_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_label_vtbl_))
	{
		assert(false);
		return nullptr;
	}
	return dlg_title_;
}

stCD_UiLabel* stCD_DlgInteraction::GetUiContent() const
{
	if (IsUnValidPtr(dlg_content_))
	{
		assert(false);
		return nullptr;
	}
	if (dlg_content_->vtbl_ != (pt_pcvoid)CD_EXE_DATA(g_va_ui_label_vtbl_))
	{
		assert(false);
		return nullptr;
	}
	return dlg_content_;
}

bool stCD_DlgInteraction::EqualDlgTitle(const std::wstring& title) const
{
	auto ui_title = GetUiTitle();
	if (!ui_title)
		return false;
	return title == ui_title->GetTotalStr().c_str();
}

bool stCD_DlgInteraction::EqualDlgContent(const std::wstring& content) const
{
	auto ui_content = GetUiContent();
	if (!ui_content)
		return false;
	return content == ui_content->GetTotalStr().c_str();
}

void stCD_ProcessInfo::DebugInfo() const
{
	if (IsUnValidPtr(process_commands_))
	{
		assert(false);
		return;
	}
	process_commands_->DebugInfo();
}

void stCD_ProcessCommands::DebugInfo() const
{
	for (auto& cmd : commands_)
	{
		if (IsUnValidPtr(cmd.second.real_obj_))
		{
			assert(false);
			continue;
		}
		cmd.second.real_obj_->DebugInfo(cmd.first);
	}
	LOG_O(Log_trace) << "总命令数：" << commands_.size();
}

void stCD_ProcessCommandOption::DebugInfo(const GameWString& key) const
{
	LOG_O(Log_trace) << "ProcessCommandOptionAddr:" << this
		<< ",Key:" << key << ",command:" << command_ << ",cmd:" << cmd_
		<< ",desc:" << desc_ << ",ukn_dw:" << ukn_dw_ << ",ukn_dw1:" << ukn_dw1_;
}

stCD_HpMpCircleValue* stCD_UiHpMpCircle::GetUiValue() const
{
	auto ui = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_EXE_DATA(g_va_ui_hp_mp_circle_value_), kTUSF_ShowHide, 1);
	if (!ui)
	{
		assert(false);
		return nullptr;
	}
	return ui->CastTo<stCD_HpMpCircleValue>();
}

void stCD_UiHpMpCircle::DebugInfo() const
{
	LOG_O(Log_debug) << "UiHpMpCircleAddr:" << this;
	auto p = GetUiValue();
	if (p)
		p->DebugInfo();
}

void stCD_HpMpCircleValue::DebugInfo() const
{
	LOG_O(Log_debug) << "HpMpCircleValueAddr:" << this << ",cur_max_value:" << cur_max_value_ << ",cur_value:" << cur_value_
		<< ",full_max_value:" << full_max_value_ << ",full_value:" << full_value_
		<< ",max_shield:" << max_shield_ << ",cur_shield:" << cur_shield_;
}

stCD_UiGridPanel* stCD_DlgPropInventories::FindGridPanel(enCD_InventoryType inventory_type) const
{
	return __FindGridPanelImpl__(this, inventory_type);
}

void stCD_ThrdParam::DebugInfo() const
{
	LOG_O(Log_debug) << "ThrdBaseAddr:" << this
		<< ",ThrdRun:" << ThrdRun;
}

stCD_UiLabel* stCD_UiButton::GetUiLabel() const
{
	auto res = FindSubUiObjByVtbl(1, (void*)CD_EXE_DATA(g_va_ui_label_vtbl_), kTUSF_Shown, 1);
	if (!res)
	{
		//assert(false);
		return nullptr;
	}
	return res->CastTo<stCD_UiLabel>();
}

bool stCD_UiButton::EqualTitle(const std::wstring& title) const
{
	auto label = GetUiLabel();
	if (!label) return false;
	return label->EqualLableStr(title);
}

stCD_UiLabel* stCD_UiLabyrinthListItem::GetUiLabelByIdx(int idx) const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_DATA(g_va_ui_label_vtbl_), kTUSF_Shown, idx);
	if (res)
		return res->CastTo<stCD_UiLabel>();
	assert(false);
	return nullptr;
}

stCD_UiLabel* stCD_UiLabyrinthListItem::GetLabelTitle() const
{
	return GetUiLabelByIdx(1);
}

stCD_UiLabel* stCD_UiLabyrinthListItem::GetLabelLvl() const
{
	return GetUiLabelByIdx(2);
}

stCD_UiLabel* stCD_UiLabyrinthListItem::GetLabelCost() const
{
	return GetUiLabelByIdx(3);
}

stCD_UiObjBase* stCD_UiLabyrinthListItem::GetUiBalls() const
{
	auto res = FindSubUiObjByVtbl(1, (pt_pcvoid)CD_DATA(g_va_ui_npc_menu_item_vtbl_), kTUSF_Shown, 1);
	assert(false);
	return res;
}

bool stCD_UiLabyrinthListItem::IsActived() const
{
	return is_actived_;
}

stCD_UiLabyrinthListItem::MyTrialInfoCont stCD_UiLabyrinthListItem::GetTrialInfoCont(enCD_GetLabyrinthTrialFlag flag) const
{
	MyTrialInfoCont cont;
	if (!trial_info_cont_.IsValid())
	{
		assert(false);
		return cont;
	}
	for (auto& v : trial_info_cont_)
	{
		if (!v.trial_.IsValid())
		{
			assert(false);
			continue;
		}
		if (IsUnValidPtr(v.ui_ball_))
		{
			assert(false);
			continue;
		}
		stMyTrialInfo trail_info = {
			v.trial_.tbl_item_, v.trial_.tbl_item_->world_area_.tbl_item_, v.IsCompleted()
		};
		if (IsUnValidPtr(trail_info.world_area_))
		{
			assert(false);
			continue;
		}
		if (trail_info.is_completed_)
		{
			if (flag & kGLT_Completed)
			{
				cont.push_back(trail_info);
				continue;
			}
		}
		else
		{
			if (flag & kGLT_UnCompleted)
			{
				cont.push_back(trail_info);
				continue;
			}
		}
	}
	return cont;
}

void stCD_UiLabyrinthListItem::DebugInfo() const
{
	auto cont = GetTrialInfoCont(kGLT_All);
	LOG_O(Log_debug) << "UiLabyrinthListItemAddr:" << this << ",IsActived:" << IsActived() << ",TrialInfoSize:" << cont.size();
	for (auto& v : cont)
	{
		v.DebugInfo();
	}
	LOG_O(Log_trace) << "--------------------------------";
}

const GameWString* stCD_UiContainer::GetPicPath() const
{
	auto ptr = pic_info_.pic_.real_obj_;
	if (IsUnValidPtr(ptr))
	{
		//assert(false);
		return nullptr;
	}
	return &ptr->pic_.pic_path_;
}

bool stCD_UiContainer::IsTrialComplete() const
{
	auto path = GetPicPath();
	if (!path)
	{
		assert(false);
		return false;
	}
	return *path == L"Art/2DArt/UIImages/InGame/Ascendancy/TrialComplete";
}

bool stCD_UiLabyrinthListItem::stTrialInfo::IsCompleted() const
{
	if (IsUnValidPtr(ui_ball_))
	{
		assert(false);
		return false;
	}
	return ui_ball_->IsTrialComplete();
}

stCD_UiLabyrinthList::ListItemCont stCD_UiLabyrinthList::GetListItems(bool must_actived) const
{
	ListItemCont cont;
	if (!list_items_.IsValid())
	{
		assert(false);
		return cont;
	}
	for (auto& v : list_items_)
	{
		if (IsUnValidPtr(v.real_obj_))
		{
			assert(false);
			continue;
		}
		auto ui_item = v.real_obj_->ui_item_;
		if (IsUnValidPtr(ui_item))
		{
			assert(false);
			continue;
		}
		if (must_actived && !ui_item->IsActived())
			continue;
		cont.push_back(ui_item);
	}
	return cont;
}

void stCD_UiLabyrinthList::DebugInfo() const
{
	auto cont = GetListItems(false);
	LOG_O(Log_debug) << "UiLabyrinthListAddr:" << this << ",CurSelected:" << cur_selected_ << ",ListItemSize:" << cont.size();
	for (auto v : cont)
	{
		v->DebugInfo();
	}
}

void stCD_UiLabyrinthListItem::stMyTrialInfo::DebugInfo() const
{
	assert(world_area_);
	LOG_O(Log_debug) << "stMyTrialInfo,area_name:" << CD_SafeName(world_area_->ch_map_name_) << ",IsComplete:" << is_completed_;
}

void stCD_DlgLabyrinthSelect::DebugInfo() const
{
	LOG_O(Log_debug) << "DlgLabyrinthSelectAddr:" << this;
	if (IsUnValidPtr(list_))
	{
		assert(false);
		return;
	}
	list_->DebugInfo();
}

stCD_DataLabyrinths* stCD_DlgTrialPlaque::GetDataLabyrinth() const
{
	if (!data_labyrinth_.IsValid())
	{
		assert(false);
		return nullptr;
	}
	return data_labyrinth_.tbl_item_;
}

stCD_DlgTrialPlaque::MyTrialInfoCont stCD_DlgTrialPlaque::GetTrialInfoCont() const
{
	MyTrialInfoCont cont;
	if (!ui_balls_.IsValid())
	{
		assert(false);
		return cont;
	}
	auto data_lab = GetDataLabyrinth();
	if (!data_lab)
		return cont;
	if (ui_balls_.size() != data_lab->trials_.size())
	{
		assert(false);
		return cont;
	}
	for (pt_dword i = 0; i < data_lab->trials_.size(); ++i)
	{
		auto& the_trial = data_lab->trials_[i];
		if (!the_trial.IsValid())
		{
			assert(false);
			continue;
		}
		if (!the_trial.tbl_item_->world_area_.IsValid())
		{
			assert(false);
			continue;
		}
		auto ui_ball = ui_balls_[i];
		if (IsUnValidPtr(ui_ball))
		{
			assert(false);
			continue;
		}
		stMyTrialInfo info = { the_trial.tbl_item_, the_trial.tbl_item_->world_area_.tbl_item_, ui_ball->IsTrialComplete() };
		cont.push_back(info);
	}
	return cont;
}

void stCD_DlgTrialPlaque::DebugInfo() const
{
	auto cont = GetTrialInfoCont();
	LOG_O(Log_debug) << "DlgTrialPlaqueAddr" << this << ",TrialInfoContSize:" << cont.size();
	for (auto& v : cont)
	{
		v.DebugInfo();
	}
}

void stCD_DlgTrialPlaque::stMyTrialInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "data_trial_Addr:" << data_trial_ << ",world_area_name:" << CD_SafeName(world_area_->ch_map_name_)
		<< ",IsComplete:" << is_complete_;
}

int stCD_UiLabyrinthList::TraListItem(const Fn_TraListItem& fn) const
{
	int cnt = 0;
	for (auto& v : list_items_)
	{
		if (IsUnValidPtr(v.real_obj_))
		{
			assert(false);
			continue;
		}
		if (IsUnValidPtr(v.real_obj_->ui_item_))
		{
			assert(false);
			continue;
		}
		++cnt;
		if (!fn(v.real_obj_))
			break;
	}
	return cnt;
}

stCD_UiSelectRoleTop* stCD_SceneSelectCharacterState::GetUiSelectRole() const
{
	if (IsUnValidPtr(select_role_))
		return nullptr;
	if (!select_role_->is_shown_)
		return nullptr;
	return select_role_;
}

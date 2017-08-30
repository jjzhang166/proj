#include "stdafx.h"
#include "CD_FuncCall.h"
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>
#include <CmnMix/Cmn_MemOper.h>
#include "CD_UsefulFns.h"
#include "CD_Const.h"
#include "CD_ProtocolS.h"
#include "CD_Updates.h"
//////////////////////////////////////////////////////////////////////////
const auto g_c_find_xml_str_by_id = MakeMemOper(Oper_ThisCall(), g_c_find_xml_str_by_id_);
const auto g_c_get_chapter_info_by_id = MakeMemOper(Oper_Cdecl(), g_c_get_chapter_info_by_id_);
const auto g_c_get_quest_name_by_id = MakeMemOper(Oper_Cdecl(), g_c_get_quest_name_by_id_);
const auto g_c_get_skill_slot_by_idx = MakeMemOper(Oper_ThisCall(), g_c_get_skill_slot_by_idx_);
const auto g_c_get_square_pos = MakeMemOper(Oper_ThisCall(), g_c_get_square_pos_);
const auto g_c_game_socket_send = MakeMemOper(Oper_ThisCall(), g_c_game_socket_send_);
const auto g_c_construct_out_packet = MakeMemOper(Oper_ThisCall(), g_c_construct_out_packet_);
const auto g_c_out_packet_send = MakeMemOper(Oper_ThisCall(), g_c_out_packet_send_);
const auto g_c_msvcr90_malloc = MakeMemOper(Oper_Cdecl(), MemOper_RetrieveAddr(), g_c_msvcr90_malloc_);
const auto g_c_msvcr90_free = MakeMemOper(Oper_Cdecl(), MemOper_RetrieveAddr(), g_c_msvcr90_free_);
const auto g_c_create_role_by_name = MakeMemOper(Oper_ThisCall(), g_c_create_role_by_name_);
//////////////////////////////////////////////////////////////////////////
void FnForOd(const char* str)
{
	str += 1;
}

void* CD_Malloc(pt_dword size)
{
#ifdef _DEBUG
	assert(size > 0);
	auto res = g_c_msvcr90_malloc.Call<void*>(size);
	assert(!IsUnValidPtr(res));
	return res;
#else
	return g_c_msvcr90_malloc.Call<void*>(size);
#endif
}

void CD_Free(pt_pcvoid ptr)
{
	assert(!IsUnValidPtr(ptr));
	return g_c_msvcr90_free.Call<void>(ptr);
}

//////////////////////////////////////////////////////////////////////////

pt_cwsz stCD_XmlString::FindStr(enCD_XmlStrIdType type, pt_dword str_id) const
{
	pt_cwsz res = nullptr;
	g_c_find_xml_str_by_id.Call<void>(this, &res, type, str_id);
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_Data_Chapter* stCD_Data_Chapter::GetChapter(pt_dword chapter_id)
{
	struct stHelp{
		stHelp(){
			ZeroMemory(this, sizeof(*this));
		}

		pt_dword				ukn_;
		stCD_Data_Chapter*	chapter_;
		p_mem_placeholder(100);
	};
	stHelp hlp;
	g_c_get_chapter_info_by_id.Call<void>(&hlp, chapter_id);
	if (IsUnValidPtr(hlp.chapter_))
	{
		assert(false);
		return nullptr;
	}
	return hlp.chapter_;
}

pt_cwsz stCD_Data_Quest::GetQuestName() const
{
	struct stHelp{
		stHelp(){
			ZeroMemory(this, sizeof(*this));
		}

		pt_cwsz		res_;
		p_mem_placeholder(50);
	};
	stHelp hlp;
	g_c_get_quest_name_by_id.Call<void>(&hlp, quest_id_);
	if (IsUnValidPtr(hlp.res_))
	{
		assert(false);
		return nullptr;
	}
	return hlp.res_;
}

stCD_SkillSlot* stCD_UIMainPlayerInfo::FindSkillSlotByIdx(pt_dword idx) const
{
	auto res = g_c_get_skill_slot_by_idx.Call<stCD_SkillSlot*>(this, idx);
	if (IsUnValidPtr(res))
	{
		//assert(false);
		return nullptr;
	}
	return res;
}

stCD_Vec3Float stCD_Model::GetPos() const
{
	stCD_Vec3Float res;
	g_c_get_square_pos.Call<void>(this, 0xFD95CA52, &res, 0xF000000F);
	return res;
}

bool stCD_OutPacket::Send() const
{
	g_c_out_packet_send.Call<void>(this);
	return true;
}

stCD_OutPacket::stCD_OutPacket(pt_dword msg_id)
{
	g_c_construct_out_packet.Call<void>(this, msg_id, (stUkn*)nullptr);
}

stCD_OutPacket::~stCD_OutPacket()
{
	if (buffer_.buffer_)
		CD_Free(buffer_.buffer_);
}

bool stCD_GameSocket::Send(const stCD_OutPacket& packet) const
{
	g_c_game_socket_send.Call<void>(this, &packet);
	return true;
}

bool stCD_UICharacterCreateDialog::CreateRole(pt_cwsz role_name) const
{
	if (!role_name)
	{
		assert(false);
		return false;
	}
	g_c_create_role_by_name.Call<void>(this, &role_name);
	return true;
}

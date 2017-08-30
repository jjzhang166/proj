#include "stdafx.h"
#include "GFuncCall.h"
#include "GAddr.h"
#include "GOffset.h"
#include "GForeach.h"
#include "GClass.h"
#include <Common/MemOper.h>
#include "GPlayer.h"
//#include "FuncObjC.h"

using namespace GAddr;
using namespace GOffset;
using namespace GForeach;
using namespace GType;
using namespace GClass;

namespace GFuncCall{
	const WORD kGameHashArray = CalcHash_WORD(L"array");

_declspec(naked) void GetStringHash_()
{
	__asm
	{
		PUSH EBP
			MOV EBP,ESP
			PUSH ESI
			MOV ESI,DWORD PTR SS:[EBP+0x8]
		MOVZX EAX,WORD PTR DS:[ESI]
		XOR ECX,ECX
			TEST AX,AX
			JE SHORT local_end
			PUSH EDI
local_do:
		MOVZX EAX,AX
			CDQ
			MOV EDI,0x1F
			IDIV EDI
			ADD ESI,0x2
			MOV EAX,EDX
			MOV EDX,ECX
			SHL EDX,0x5
			SUB EDX,ECX
			ADD EAX,EDX
			XOR EDX,EDX
			MOV ECX,0x3D09000
			DIV ECX
			MOVZX EAX,WORD PTR DS:[ESI]
		MOV ECX,EDX
			TEST AX,AX
			JNZ SHORT local_do
			POP EDI
local_end:
		MOV EAX,ECX
			POP ESI
			POP EBP
			RETN
	}
}

WORD CalcHash_WORD( const wchar_t* wstr )
{
	if ( !wstr )
	{
		assert(false);
		return 0;
	}
	DWORD	dwHash = 0;
	__asm
	{
		push wstr
		lea eax, GetStringHash_
		call eax
		add esp, 4
		mov dwHash, eax
	}

	return (WORD)dwHash;
}

WORD CalcHash_WORD( const std::wstring& wstr )
{
	return CalcHash_WORD(wstr.c_str());
}

DWORD CalcHash_DWORD( const wchar_t* wstr_lhs, const wchar_t* wstr_rhs )
{
	if ( !wstr_lhs || !wstr_rhs )
	{
		assert(false);
		return 0;
	}
	DWORD	dwHash = 0;
	int i = 0;
	while( *(WORD*)(wstr_lhs+i) != 0 )
	{
		dwHash *= 0x1003F;
		dwHash += *(WORD*)(wstr_lhs+i);
		i++;
	}

	i = 0;
	while( *(WORD*)(wstr_rhs+i) != 0 )
	{
		dwHash *= 0x1003F;
		dwHash += *(WORD*)(wstr_rhs+i);
		i++;
	}
	return dwHash;
}

DWORD CalcHash_DWORD( const wchar_t* wstr )
{
	if ( !wstr )
	{
		assert(false);
		return 0;
	}
	DWORD	dwHash = 0;
	int i = 0;
	while( *(WORD*)(wstr+i) != 0 )
	{
		dwHash *= 0x1003F;
		dwHash += *(WORD*)(wstr+i);
		i++;
	}
	return dwHash;
}

DWORD CalcHash_DWORD( const std::wstring& wstr_lhs, const std::wstring& wstr_rhs )
{
	return CalcHash_DWORD(wstr_lhs.c_str(), wstr_rhs.c_str());
}

DWORD CalcHash_DWORD( const std::wstring& wstr )
{
	return CalcHash_DWORD(wstr.c_str());
}

void* GetFbUnknownObjByName( const std::wstring& room_id )
{
	struct stStrHlp{
		const wchar_t*			str_ptr_;
		DWORD					unknown_[3];
		DWORD					str_len_;
		DWORD					unknown_flag_;
		stStrHlp( const std::wstring& str ){
			static_assert(sizeof(*this) == 0x18, "");
			str_ptr_ = str.c_str();
			ZeroMemory(unknown_, sizeof(unknown_));
			str_len_ = str.size();
			if ( str_len_ < 0x0f )
				unknown_flag_ = 0x0f;
			else
				unknown_flag_ = 0x17;
		}
	};
	assert(false);
	return nullptr;
	/*stStrHlp str_hlp(room_id);
	return g_pGetFbUnknownObj.Call<void*>(g_pRoleObjAddr + g_o_CloneMapOffset1 + g_o_CloneMapOffset2 + g_o_CloneMapOffset3,
		&str_hlp);*/
}

float GetPreActionPlayTime()
{
	if ( !g_pPreActionPlayTimeAddr )
	{
		assert(false);
		return 0.;
	}
	BTreeMgrBase::stGBTreeNodes* the_node = nullptr;
	if ( !MemOperApplyR(the_node, (int)g_pPreActionPlayTimeAddr, g_o_pre_action_play_time1) )
	{
		assert(false);
		return 0.;
	}
	if ( !the_node )
	{
		assert(false);
		return 0.;
	}
	the_node = (BTreeMgrBase::stGBTreeNodes*)((char*)the_node + g_o_pre_action_play_time2);

	//////////////////////////////////////////////////////////////////////////
	struct stPreActionPlayTime : GClass::BTreeBase{
		stPreActionPlayTime(){
			serial_ = 0;
			next_node_ = nullptr;
		}
		bool Update(){
			assert(0 == serial_);
			assert(!next_node_);
			auto thePrevActionPlayTime_1 = (const PREV_ACTION_PLAY_TIME_1*)game_cur_node_;
			if( !thePrevActionPlayTime_1->dwLocalObjAddr )
				return true;
			assert(thePrevActionPlayTime_1->dwLocalObjAddr > WORD(-1));
			next_node_ = (const PREV_ACTION_PLAY_TIME_2*)thePrevActionPlayTime_1->dwLocalObjAddr;
			serial_ = next_node_->dwSerial;
			return true;
		}
		DWORD							serial_;
		const PREV_ACTION_PLAY_TIME_2*	next_node_;
	};
	struct stPreActionPlayTimeMgr : BTreeMgr_SkipRoot<stPreActionPlayTime>{
		const stPreActionPlayTime* FindBySerial( DWORD serial ){
			assert(serial);
			auto it = std::find_if(btree_.begin(), btree_.end(), [serial]( const stPreActionPlayTime& pa ){
				return pa.serial_ == serial;
			});
			if ( it == btree_.end() )
				return nullptr;
			return &*it;
		}
	};
	//////////////////////////////////////////////////////////////////////////
	stPreActionPlayTimeMgr the_mgr;
	the_mgr.SetGameNodes(the_node);
	if ( !the_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return 0.;
	}
	auto the_papt = the_mgr.FindBySerial(GPlayerSelf::GetMe().GetObjSerial());
	if ( !the_papt )
	{
		assert(false);
		return 0.;
	}
	if ( !the_papt->next_node_ )
	{
		assert(false);
		return 0.;
	}
	//////////////////////////////////////////////////////////////////////////
	struct stPapt : BTreeBase{
		stPapt(){
			action_play_time = 0.;
		}
		bool Update(){
			auto pdata = (const PREV_ACTION_PLAY_TIME_3*)game_cur_node_;
			if ( pdata->dwLocalObjAddr < WORD(-1) )
			{
				assert(false);
				return false;
			}
			action_play_time = *(float*)(pdata->dwLocalObjAddr + g_o_pre_action_play_time3);
			return true;
		}

		float		action_play_time;
	};
	struct stPaptMgr : BTreeMgr_SkipRoot<stPapt>{};
	//////////////////////////////////////////////////////////////////////////
	the_node = (BTreeMgrBase::stGBTreeNodes*)&the_papt->next_node_->theMapAddr;
	stPaptMgr papt_mgr;
	papt_mgr.SetGameNodes(the_node);
	if ( !papt_mgr.RebuildUpdateAll() )
	{
		assert(false);
		return 0.;
	}
	const auto& the_cont = papt_mgr.GetAllNodes();
	if ( 1 != the_cont.size() )
	{
		assert(false);
		return 0.;
	}
	return the_cont[0].action_play_time;
}

}
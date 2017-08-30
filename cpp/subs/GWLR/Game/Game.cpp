// Game.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Protocol/P_Defines.h"
#include "FuncObjC.h"
#include "Facility/GClass.h"
#include "Protocol/P_BaseStruct.h"
#include <set>
#include "Facility/GFuncCall.h"
#include "Facility/GRecvMsgMgr.h"
#include "Facility/GameObjMgr.h"
#include "Facility/GPlayer.h"
#include "Facility/GMonster.h"
#include "GloFuncs.h"
#include <Common/EnsureCleanup.h>
#include <CommonX/CmnX_HookerMore.h>
#include <CmnMix/Cmn_CharArray.h>
#include <Common/CmnImageHlp.h>
#include <Common/CmnDllApi.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>
//////////////////////////////////////////////////////////////////////////
//global var
//////////////////////////////////////////////////////////////////////////
extern bool InitGame()
{
	extern void InitDoorLinkInfo();
	InitDoorLinkInfo();
	return true;
}

void EndOfRun()
{
}
//////////////////////////////////////////////////////////////////////////
//for gain main wnd handle
void* Hooker_GainMainWndHandle( IHooker& hooker, JmpHookerAmbient& ambient )
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "通过hook code得到主窗口句柄";
	auto func_obj_normal = FuncObj_NormalC::GetInstance();
	auto& gs = GlobalSetter::GetMe();
	if ( gs.GetGameMainWnd() )
		return hooker.GetCodeOrig();
	gs.SetGameMainWnd((HWND)ambient.eax_);
	if (func_obj_normal)
	{
		GetIoService().post([](){
			auto func_obj_normal = FuncObj_NormalC::GetInstance();
			if ( !func_obj_normal )
			{
				assert(false);
				return;
			}
			assert(!func_obj_normal->GetInterGWnd());

			if ( !func_obj_normal->InitInter(GlobalSetter::GetMe().GetGameMainWnd()) )
			{
				LOG_O(Log_fatal) << "初始化func obj inter失败";
			}
		});		
	}
	return hooker.GetCodeOrig();
}

void* Hooker_RecvPacket( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	auto cmd_header = (CMDR_HeaderBase*)(ambient.edi_);
	if (IsUnValidPtr(cmd_header))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto& recv_mgr = GRecvMsgMgr::GetMe();
	recv_mgr.RefreshLastRecvTime();
	auto& ret_addr_info = recv_mgr.HandleMsg(cmd_header);
	if ( !ret_addr_info.new_addr_ )
		ret_addr_info.new_addr_ = (char*)hooker.GetCodeOrig();
	return (char*)ret_addr_info.new_addr_ + ret_addr_info.ret_offset_;
}

void* Hooker_GameSendOnly( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	auto cmd_header = *(CMDR_HeaderBase**)(ambient.esp_ + 4);
	if (IsUnValidPtr(cmd_header))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto& recv_mgr = GHookGameSendMsgMgr::GetMe();
	auto& ret_addr_info = recv_mgr.HandleMsg(cmd_header);
	if ( !ret_addr_info.new_addr_ )
		ret_addr_info.new_addr_ = (char*)hooker.GetCodeOrig();
	return (char*)ret_addr_info.new_addr_ + ret_addr_info.ret_offset_;
}

void* Hooker_GainMonsterHp( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	if (IsUnValidPtr(ambient.esp_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	//为了解决梦的起点之打怪，怪物不死的问题
	/*auto last_call_addr = *(unsigned int*)ambient->esp_;
	if ( Offset2DllAddr(g_pMonsterHpLastCall) != last_call_addr )
		return hooker.GetCodeOrig();*/

	auto param_info = *(const stCD_HitParamInfo**)(ambient.esp_ + 4);
	if (IsUnValidPtr(param_info))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto hitted_info = (const stCD_HittedObjInfo*)ambient.ecx_;
	if (IsUnValidPtr(hitted_info))
	{
		//assert(false);
		return hooker.GetCodeOrig();
	}
	if (IsUnValidPtr(hitted_info->net_obj_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto monster_packet_id = hitted_info->net_obj_->packet_id_;
	auto cur_hp = param_info->cur_hp_;
	GetIoService().post([monster_packet_id, cur_hp](){
		GameObjMgr::GetMe().UpdateMonsterHp(monster_packet_id, cur_hp);
	});
	return (char*)hooker.GetCodeOrig();
}

void* Hooker_GainHitHp( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	if (IsUnValidPtr(ambient.esp_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}

	auto param_info = *(const stCD_HitParamInfo**)(ambient.esp_ + 4);
	if (IsUnValidPtr(param_info))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto hitted_info = (const stCD_HittedObjInfo*)ambient.ecx_;
	if (IsUnValidPtr(hitted_info))
	{
		//assert(false);
		return hooker.GetCodeOrig();
	}
	if (IsUnValidPtr(hitted_info->net_obj_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto obj_packet_id = hitted_info->net_obj_->packet_id_;
	auto cur_hp = param_info->cur_hp_;
	GetIoService().post([obj_packet_id, cur_hp](){
		GPlayerMe::GetMe().SetHitHp(obj_packet_id, cur_hp);
	});
	return (char*)hooker.GetCodeOrig();
}

void* Hooker_GainFeebHp( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	if (IsUnValidPtr(ambient.esp_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}

	auto param_info = *(const stCD_HitParamInfo**)(ambient.esp_ + 4);
	if (IsUnValidPtr(param_info))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto hitted_info = (const stCD_HittedObjInfo*)ambient.ecx_;
	if (IsUnValidPtr(hitted_info))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if (IsUnValidPtr(hitted_info->net_obj_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto obj_packet_id = hitted_info->net_obj_->packet_id_;
	auto cur_hp = param_info->cur_hp_;
	GetIoService().post([obj_packet_id, cur_hp](){
		GPlayerMe::GetMe().SetFeebHp(obj_packet_id, cur_hp);
	});
	return (char*)hooker.GetCodeOrig();
}

void OnCrossMap()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;

	auto& ambient = GClass::GAmbient::GetMe();
	ambient.CancelTimer_FbWin();

	norc->SetGameState(GType::kGS_InGame);
	ambient.cond_cross_graph_.TouchCond();
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "过图";
}

void* Hooker_LoadMap1( IHooker& hooker, JmpHookerAmbient& ambient )
{
	GetIoService().post(&OnCrossMap);
	return hooker.GetCodeOrig();
}

void* Hooker_LoadMap2( IHooker& hooker, JmpHookerAmbient& ambient )
{
	GetIoService().post(&OnCrossMap);
	return hooker.GetCodeOrig();
}

void* Hooker_FbGameOver_Win( IHooker& hooker, JmpHookerAmbient& ambient )
{
	GetIoService().post([](){
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "副本结束之胜利了";
		auto& ambient = GClass::GAmbient::GetMe();
		ambient.cond_fb_game_over_win_.TouchCond();
		//ambient.BeginTimer_FbWin();
	});
	return hooker.GetCodeOrig();
}

void* Hooker_FbGameOver_Lose( IHooker& hooker, JmpHookerAmbient& ambient )
{
	GetIoService().post([](){
		LOG_ENTER_FUNC;
		LOG_O(Log_trace) << "副本结束之结束了";
		GClass::GAmbient::GetMe().cond_fb_game_over_lose_.TouchCond();
	});
	return hooker.GetCodeOrig();
}

void* Hooker_ChooseRoleScene( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GType;
	GetIoService().post([](){
		auto norc = FuncObj_NormalC::GetInstance();
		if ( !norc )
			return;
		//if ( kGS_InGame != norc->GetGameState() )
		{
			norc->SetGameState(kGS_SelectRole);
			LOG_ENTER_FUNC;
			LOG_O(Log_trace) << "hook到选择角色界面";
		}
	});
	return hooker.GetCodeOrig();
}

void* Hooker_JuQing( IHooker& hooker, JmpHookerAmbient& ambient )
{
	if (IsUnValidPtr(ambient.eax_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto ju_qing_info = (const stCD_JuQingInfo*)ambient.eax_;
	auto str_ju_qing = ju_qing_info->str_info_;
	if (IsUnValidPtr(str_ju_qing))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	if (GClass::GAmbient::GetMe().cond_ju_qing_.CondtionTrue())
		return hooker.GetCodeOrig();
	std::string ju_qing(str_ju_qing);
	GetIoService().post([ju_qing](){
		GClass::GAmbient::GetMe().SetJuQingStr(ju_qing);
	});
	return hooker.GetCodeOrig();
}

void* Hooker_GainBodyParts( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	if (IsUnValidPtr(ambient.ecx_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	static GMonster_HitPartBase hit_part_hlp;
	if (!hit_part_hlp.SetBodyPartsObjAddr((stCD_BodyPartObj*)ambient.ecx_))
		return hooker.GetCodeOrig();
	auto& hit_part = GMonster_HitPart::GetMonsterHitPart();
	if (!hit_part.HasHitCnt())
		return hooker.GetCodeOrig();
	if ( !hit_part_hlp.ObjValid() )
		return hooker.GetCodeOrig();
	if (hit_part_hlp.GetPacketId() != hit_part.cur_hit_obj_packet_id_)
		return hooker.GetCodeOrig();
	if (!hit_part.From(hit_part_hlp))
		return hooker.GetCodeOrig();
	//hit_part.Update();
	return hooker.GetCodeOrig();
}

void* Hooker_ModifyHitForBodyPart( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	using namespace GType;
	/*
	const int g_o_hit_msg_part_idx = MMOCLIENT_REVERSAL_HIT_BU_WEI_ID_PARAMETER_OFFSET___;
	const int g_o_hit_msg_part_pos_x = MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET___;
	const int g_o_hit_msg_part_pos_y = MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET___;
	const int g_o_hit_msg_part_pos_z = MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET___;
	const int g_o_hit_msg_dir_x = MMOCLIENT_REVERSAL_HIT_BU_WEI_DIR_X_PARAMETER_OFFSET___;
	const int g_o_hit_msg_dir_y = MMOCLIENT_REVERSAL_HIT_BU_WEI_DIR_Y_PARAMETER_OFFSET___;
	const int g_o_hit_msg_unknown_1 = MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET4___;
	const int g_o_hit_msg_unknown_2 = MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET4___;
	const int g_o_hit_msg_unknown_3 = MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET2___;
	const int g_o_hit_msg_unknown_4 = MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET2___;
	const int g_o_hit_msg_unknown_5 = MMOCLIENT_REVERSAL_HIT_BU_WEI_X_PARAMETER_OFFSET3___;
	const int g_o_hit_msg_unknown_6 = MMOCLIENT_REVERSAL_HIT_BU_WEI_Y_PARAMETER_OFFSET3___;
	const int g_o_hit_msg_unknown_7 = MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET4___;
	const int g_o_hit_msg_unknown_8 = MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET2___;
	const int g_o_hit_msg_unknown_9 = MMOCLIENT_REVERSAL_HIT_BU_WEI_Z_PARAMETER_OFFSET3___;

	auto hit_info_addr = *(char**)(ambient.esp_ + 4);
	if (IsUnValidPtr(hit_info_addr))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto& monster_body_part = GMonster_HitPart::GetMonsterHitPart();
	auto part_info = monster_body_part.GetCurHitPart();
	if ( !part_info )
		return hooker.GetCodeOrig();
	auto monster_pos_info = monster_body_part.GetPosInfo();
	if (IsUnValidPtr(monster_pos_info))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	monster_body_part.DecHitCnt();
	GType::stDirection2d the_dir;
	auto& gpm = GPlayerMe::GetMe();
	if ( !gpm.PosInfo2Dir2d(*monster_pos_info, *part_info, the_dir) )
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	
	*(int*)(hit_info_addr + g_o_hit_msg_part_idx) = part_info->dwBuWeiId;
	LOG_O(Log_trace) << "the old dir(" << *(float*)(hit_info_addr + g_o_hit_msg_dir_x) << "," << *(float*)(hit_info_addr + g_o_hit_msg_dir_y) << ")the new dir(" << the_dir.dir_x << "," << the_dir.dir_y << ")"
		<< "left z:" << part_info->theRangeLeft.fz;
	/ *the_dir.dir_x = 0.5f;
	the_dir.dir_y = 0.5f;* /
	*(float*)(hit_info_addr + g_o_hit_msg_dir_x) = the_dir.dir_x;
	*(float*)(hit_info_addr + g_o_hit_msg_dir_y) = the_dir.dir_y;

	/ *auto the_pos_x = (part_info->theRangeLeft.fx + part_info->theRangeRight.fx ) / 2;
	auto the_pos_y = (part_info->theRangeLeft.fy + part_info->theRangeRight.fy) / 2;* /
	auto the_pos_x = part_info->theRangeLeft.fx / *- 0.2f* /;
	auto the_pos_y = part_info->theRangeLeft.fy / *- 0.2f* /;
	//auto the_pos_z = part_info->theRangeLeft.fz;
	*(float*)(hit_info_addr + g_o_hit_msg_part_pos_x) = the_pos_x;
	*(float*)(hit_info_addr + g_o_hit_msg_part_pos_y) = the_pos_y;
	// *(float*)(hit_info_addr + g_o_hit_msg_part_pos_z) = the_pos_z;

	//把其他的坐标都修改成攻击点的坐标，这样攻击后，显示的血值是攻击部位弹出
	*(float*)(hit_info_addr + g_o_hit_msg_unknown_1) = the_pos_x;
	*(float*)(hit_info_addr + g_o_hit_msg_unknown_2) = the_pos_y;
	// *(float*)(hit_info_addr + g_o_hit_msg_unknown_7) = the_pos_z;
	*(float*)(hit_info_addr + g_o_hit_msg_unknown_3) = the_pos_x;
	*(float*)(hit_info_addr + g_o_hit_msg_unknown_4) = the_pos_y;
	// *(float*)(hit_info_addr + g_o_hit_msg_unknown_8) = the_pos_z;
	*(float*)(hit_info_addr + g_o_hit_msg_unknown_5) = the_pos_x;
	*(float*)(hit_info_addr + g_o_hit_msg_unknown_6) = the_pos_y;
	// *(float*)(hit_info_addr + g_o_hit_msg_unknown_9) = the_pos_z;

	LOG_O(Log_trace) << "攻击部分修改完毕";*/
	return hooker.GetCodeOrig();
}

void* Hooker_DlgInfo( IHooker& hooker, JmpHookerAmbient& ambient )
{
	if (IsUnValidPtr(ambient.esi_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto dlg_info = (const stCD_PopDlgInfo*)ambient.esi_;

	auto str_ptr = dlg_info->str_info_;
	if (IsUnValidPtr(str_ptr))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}

	std::shared_ptr<std::string> auto_str_ptr;
	auto_str_ptr.reset(new std::string(str_ptr));
	/*GetIoService().post([auto_str_ptr](){
		if ( !auto_str_ptr )
		{
			assert(false);
			return;
		}*/
		GClass::GAmbient::GetMe().MatchDlgInfoStr(auto_str_ptr, ambient.ebp_);
	//});
	return hooker.GetCodeOrig();
}

void* Hooker_ShowCursor( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	if (IsUnValidPtr(ambient.esp_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto is_show = *(BOOL*)(ambient.esp_ + 4);
	GetIoService().post([is_show](){
		GClass::GAmbient::GetMe().is_show_cursor_ = is_show;
	});
	return hooker.GetCodeOrig();
}

void* Hooker_AmbientStr( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	if (IsUnValidPtr(ambient.ebp_))
	{
		assert(false);
		return hooker.GetCodeOrig();
	}
	auto the_str = *(const char**)(ambient.ebp_ + 0x0c);
	if (IsUnValidPtr(the_str))
		return hooker.GetCodeOrig();
	std::shared_ptr<std::string> auto_str_ptr;
	auto_str_ptr.reset(new std::string(the_str));
	GetIoService().post([auto_str_ptr](){
		if ( !auto_str_ptr )
		{
			assert(false);
			return;
		}
		GAmbient::GetMe().MatchAmbientStr(auto_str_ptr);
	});
	return hooker.GetCodeOrig();
}

void* Hooker_GamePassRoom( IHooker& hooker, JmpHookerAmbient& ambient )
{
	extern bool g_enable_pass_room_;
	if (g_enable_pass_room_ && !LuasScriptEchoC::IsRunning())
		return hooker.GetCodeOrig();
	void* res;
	_asm mov res, offset LABEL_RET
	return res;

LABEL_RET:
	_asm retn 4
}

void* Hooker_GamePassVillage( IHooker& hooker, JmpHookerAmbient& ambient )
{
	using namespace GClass;
	if ( !LuasScriptEchoC::IsRunning() )
		return hooker.GetCodeOrig();
	void* res;
	_asm mov res, offset LABEL_RET
	return res;

LABEL_RET:
	_asm retn 0x8
}

void* Hooker_NtReadVirtualMemory( IHooker& hooker, JmpHookerAmbient& ambient )
{
	//NtReadVirtualMemory参数意义与ReadProcessMemory一样
	typedef int (__stdcall *NtReadVirtualMemory)(IN HANDLE ProcessHandle,IN PVOID BaseAddress,OUT PVOID Buffer,IN ULONG NumberOfBytesToRead,OUT PULONG NumberOfBytesReaded OPTIONAL );

	const int kParam_ProcessHandle = 1;
	const int kParam_BaseAddr = 2;
	const int kParam_Buffer = 3;
	const int kParam_Size = 4;
	const int kParam_NumberOfBytesReaded = 5;

	//////////////////////////////////////////////////////////////////////////
	auto the_process_handle = ambient.EspParamAs<kParam_ProcessHandle, HANDLE>();
	auto the_process_id = ::GetProcessId(the_process_handle);
	if ( the_process_id != GlobalSetter::GetMe().GetCurProcessId() )
	{
		//LOG_O(Log_debug) << "Hooker_NtReadVirtualMemory，读取了其他进程内存，进程ID：" << the_process_id;
		return hooker.GetCodeOrig();
	}
	auto cur_thread_id = ::GetCurrentThreadId();
	auto& anti_hook = CmnX_AntiHookCheck::GetRealMe();
	if (anti_hook.IsInThreadWhiteList(cur_thread_id))
		return hooker.GetCodeOrig();
	//////////////////////////////////////////////////////////////////////////

	auto base_addr = ambient.EspParamAs<kParam_BaseAddr, void*>();
	if (IsUnValidPtr(base_addr))
	{
		LOG_O(Log_debug) << "Hooker_NtReadVirtualMemory，base addr参数不正确：" << base_addr << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);
		return hooker.GetCodeOrig();
	}
	auto out_buffer = ambient.EspParamAs<kParam_Buffer, void*>();
	if (IsUnValidPtr(out_buffer))
	{
		LOG_O(Log_debug) << "Hooker_NtReadVirtualMemory，out_buffer参数不正确：" << out_buffer << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);
		return hooker.GetCodeOrig();
	}
	auto n_size = ambient.EspParamAs<kParam_Size, int>();
	if ( n_size <= 0 )
	{
		LOG_O(Log_debug) << "Hooker_NtReadVirtualMemory，n_size参数不正确：" << n_size << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);
		return hooker.GetCodeOrig();
	}
	auto the_number = ambient.EspParamAs<kParam_NumberOfBytesReaded, PULONG>();

	ULONG read_size = 0;
	ambient.eax_ = ((NtReadVirtualMemory)hooker.GetCodeOrig())(the_process_handle, base_addr, out_buffer, n_size, &read_size);
	if ( 0 == ambient.eax_ )
	{
		if (anti_hook.OutputSrcCodeInfoByAddr(base_addr, read_size, out_buffer))
		{
			/*LOG_O(Log_debug) << "NtReadVirtualMemory检测到hook了，地址：" << base_addr << " 大小：" << n_size << " 读取到的大小：" << read_size << " 缓冲区地址：" <<
				out_buffer << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);*/
		}
	}
	else
	{
		LOG_O(Log_debug) << "读取内存返回失败，地址：" << base_addr << " 大小：" << n_size << " 读取到的大小：" << read_size << " 缓冲区地址：" << out_buffer 
			<< " LastError：" << GetLastError() << " 结果：" << ambient.eax_ << " 线程ID：" << cur_thread_id << " 线程入口：" << GetThreadStartAddr(cur_thread_id);
	}
	
	if (the_number)
		*the_number = read_size;

	void* ret_addr;
	_asm mov ret_addr, offset LABEL_RET
	return ret_addr;
LABEL_RET:
	_asm retn 20
}

struct stMemCpyParam{
	const char*		str_info_;
	const void*		dst_addr_;
	const void*		src_addr_;
	int				size_;
	DWORD			thread_id_;
};
void MemCpy_PromptInfo( void* param, unsigned short param_size )
{
	auto the_param = (const stMemCpyParam*)param;
	assert(param_size == sizeof(stMemCpyParam));
	LOG_O(Log_debug) << the_param->str_info_ << " 线程ID：" << the_param->thread_id_ << " 线程入口：" << GetThreadStartAddr(the_param->thread_id_) << " dst addr:" << the_param->dst_addr_ << " src addr:" << the_param->src_addr_ << " 大小：" << the_param->size_;
}

//Hooker_Memcpy里做事要非常小心，因为这个API太常用了
void* Hooker_Memcpy( IHooker& hooker, JmpHookerAmbient& ambient )
{
	typedef DWORD (__cdecl *Nt_memcpy)( void* dst, const void* src, size_t count );

	//////////////////////////////////////////////////////////////////////////
	auto& anti_hook = CmnX_AntiHookCheck::GetRealMe();
	auto cur_thread_id = ::GetCurrentThreadId();
	if (anti_hook.IsInThreadWhiteList(cur_thread_id))
		return hooker.GetCodeOrig();
	//////////////////////////////////////////////////////////////////////////
	const int kParam_Dst = 1;
	const int kParam_src = 2;
	const int kParam_count = 3;

	auto dst_addr = ambient.EspParamAs<kParam_Dst, void*>();
	auto src_addr = ambient.EspParamAs<kParam_src, const void*>();
	auto n_size = ambient.EspParamAs<kParam_count, int>();
	if (IsUnValidPtr(dst_addr))
	{
#ifdef _DEBUG
		stMemCpyParam param = { "Hooker_Memcpy，dst_addr参数不正确：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));
#endif
		return hooker.GetCodeOrig();
	}
	if (IsUnValidPtr(src_addr))
	{
#ifdef _DEBUG
		/*stMemCpyParam param = { "Hooker_Memcpy，src_addr参数不正确：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));*/
#endif
		return hooker.GetCodeOrig();
	}
	if ( n_size <= 0 )
	{
#ifdef _DEBUG
		/*stMemCpyParam param = { "Hooker_Memcpy，n_size参数不正确：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));*/
#endif
		return hooker.GetCodeOrig();
	}

	ambient.eax_ = ((Nt_memcpy)hooker.GetCodeOrig())(dst_addr, src_addr, n_size);

	if ( !IsUnValidPtr(ambient.eax_) )
	{
#ifdef _DEBUG
		/*stMemCpyParam param = { "Hooker_Memcpy开始反crc：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));*/
#endif
		if (anti_hook.OutputSrcCodeInfoByAddr(src_addr, n_size, (void*)ambient.eax_))
		{
#ifdef _DEBUG
			/*stMemCpyParam param = { "Hooker_Memcpy发现检测：", dst_addr, src_addr, n_size, cur_thread_id };
			GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));*/
#endif
		}
	}
	else
	{
#ifdef _DEBUG
		stMemCpyParam param = { "Hooker_Memcpy返回值不正确：", dst_addr, src_addr, n_size, cur_thread_id };
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(MemCpy_PromptInfo, param));
#endif
	}

	void* ret_addr;
	_asm mov ret_addr, offset LABEL_RET
	return ret_addr;
LABEL_RET:
	_asm retn
}

struct stLogSinkParam{
	const char*		str_info_;
	char			hooker_name_[kAntiHookCheck_MaxHookerNameSize + 1];
	const void*		hooker_addr_;
	int				size_;
	DWORD			thread_id_;
};

void LogSink_PromptInfo( void* param, unsigned short param_size )
{
	auto the_param = (const stLogSinkParam*)param;
	assert(param_size == sizeof(stLogSinkParam));
	LOG_O(Log_debug) << the_param->str_info_ << " 线程ID：" << the_param->thread_id_ << " 线程入口：" << GetThreadStartAddr(the_param->thread_id_) << " hook名:" << the_param->hooker_name_ << " hooker addr:" << the_param->hooker_addr_ << " 大小：" << the_param->size_;
}
struct stLogSinkParam_OnCheck{
	const char*		str_info_;
	char			hooker_name_[kAntiHookCheck_MaxHookerNameSize + 1];
	const void*		check_addr_;
	int				check_size_;
	const void*		hooked_addr_;
	int				hooked_cpy_size_;
	DWORD			thread_id_;
};
void LogSink_PromptInfo_OnCheck( void* param, unsigned short param_size )
{
	auto the_param = (const stLogSinkParam_OnCheck*)param;
	assert(param_size == sizeof(stLogSinkParam_OnCheck));
	LOG_O(Log_debug) << the_param->str_info_ << " 线程ID：" << the_param->thread_id_ << " 线程入口：" << GetThreadStartAddr(the_param->thread_id_) << " hook名:" << the_param->hooker_name_ << " check addr:" << the_param->check_addr_ << " 检查大小：" << the_param->check_size_
		<< " hooked addr:" << the_param->hooked_addr_ << " hooked copy size:" << the_param->hooked_cpy_size_;
}
class HookerLogSink : public CmnX_AntiHookLogSink{
	void OnAddHookCodeInfo( const char* hooker_name, const void* the_hook_addr, std::size_t hook_cpy_size ) const{
		stLogSinkParam param;
		param.str_info_ = "添加了一个hook：";
		Cmnx_StrCpy(param.hooker_name_, hooker_name, sizeof(param.hooker_name_));
		param.hooker_addr_ = the_hook_addr;
		param.size_ = hook_cpy_size;
		param.thread_id_ = ::GetCurrentThreadId();
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(LogSink_PromptInfo, param));
	}
	void OnHookChecked( const char* hooker_name, const void* check_addr, std::size_t checked_size, const void* hooked_addr, std::size_t hooked_cpy_size ) const{
		stLogSinkParam_OnCheck param;
		param.str_info_ = "检测到hook：";
		Cmnx_StrCpy(param.hooker_name_, hooker_name, sizeof(param.hooker_name_));
		param.check_addr_ = check_addr;
		param.check_size_ = checked_size;
		param.hooked_addr_ = hooked_addr;
		param.hooked_cpy_size_ = hooked_cpy_size;
		param.thread_id_ = ::GetCurrentThreadId();
		GetStaticTaskQueue().AddTask(StaticTaskQueueT::MakeFuncInfo(LogSink_PromptInfo_OnCheck, param));
	}
};

void* Hooker_JingCaiJingTou( IHooker& hooker, JmpHookerAmbient& ambient )
{
	auto the_ptr_ptr = *(DWORD**)(ambient.esp_ + 8);
	if (IsUnValidPtr(the_ptr_ptr))
		return hooker.GetCodeOrig();
	if ( 0x56F == *the_ptr_ptr )
	{
		GetIoService().post([](){
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "hook到精彩镜头，接下来就有框弹出了";
			GClass::GAmbient::GetMe().cond_fb_game_over_lose_.TouchCond();
		});
	}
	return hooker.GetCodeOrig();
}

bool HandleMonsterHit(const stCD_MonsterHitInfo* hit_info)
{
	if (IsUnValidPtr(hit_info))
	{
		assert(false);
		return false;
	}
	bool is_find = false;
	CD_Traverse::TraverseAllLocalObjs([hit_info, &is_find](stCD_ObjBase* obj){
		if (obj->GetType() != CD_En_LocalObjType::kNpc)
			return true;
		if (IsUnValidPtr(obj->name_))
			return true;
		//木马怪的名称是在net obj中的。
		if (strstr(obj->name_, "TechnologyTest") != nullptr /*|| strstr(obj->name_, "木马怪") != nullptr*/)
		{
			if (hit_info->hit_obj_id_ == obj->id_)
			{
				is_find = true;
				return false;
			}
		}
		return true;
	});
	if (is_find)
		return false;

	auto role = CD_FuncCall::GetCurRole();
	if (!role)
	{
		assert(false);
		return false;
	}
	int role_lvl = role->lvl_;
	if (role_lvl <= 6)
		return false;

	if (role->id_ == hit_info->hitted_obj_id_)
		return true;
	return false;
}

void* Hooker_MonsterHit(IHooker& hooker, JmpHookerAmbient& ambient)
{
	extern volatile bool g_is_enable_wu_di_;
	if (g_is_enable_wu_di_ && HandleMonsterHit((stCD_MonsterHitInfo*)ambient.eax_))
		//不调用GameHit函数
	{		
		static const pt_csz the_addr = (pt_sz)hooker.GetHookedCodeAddr() + 5;

		void* ret_addr;
		_asm mov ret_addr, offset LABEL_RET
		return ret_addr;
	LABEL_RET:
		_asm add esp, 8
		_asm jmp the_addr
	}
	return hooker.GetCodeOrig();
}

void* Hooker_MonsterRemoteHit(IHooker& hooker, JmpHookerAmbient& ambient)
{
	extern volatile bool g_is_enable_wu_di_;
	if (g_is_enable_wu_di_)
		//不调用GameHit函数
	{
		auto hit_info = *(stCD_MonsterRemoteHitInfo**)(ambient.esp_ + 4);
		if (IsUnValidPtr(hit_info))
		{
			assert(false);
			return hooker.GetCodeOrig();
		}
		if (IsUnValidPtr(hit_info->hitted_obj_))
			return hooker.GetCodeOrig();
		auto role = CD_FuncCall::GetCurRole();
		if (!role)
		{
			assert(false);
			return hooker.GetCodeOrig();
		}
		if (role->id_ != hit_info->hitted_obj_->id_)
			return hooker.GetCodeOrig();
		LOG_O(Log_trace) << "hit_info:" << hit_info << " role_local_id:" << std::hex << role->id_ 
			<< " stCD_MonsterRemoteHitObj:" << (stCD_MonsterRemoteHitObj*)ambient.ecx_;

		/*void* ret_addr;
		_asm mov ret_addr, offset LABEL_RET
		return ret_addr;
	LABEL_RET:
		_asm ret 4*/

		auto hit_obj = (stCD_MonsterRemoteHitObj*)ambient.ecx_;
		hit_obj->ukn_flag_ = true;
	}
	return hooker.GetCodeOrig();
}

void* Hooker_Render(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto nor = FuncObj_NormalC::GetInstance();
	if (nor && nor->is_you_hua_)
	{
		//优化怪物猎人渲染线程
		//::Sleep(0x40);
		Sleep(0x60);
	}	
	return hooker.GetCodeOrig();
}

void* Hooker_BlackScreen(IHooker& hooker, JmpHookerAmbient& ambient)
{
	auto nor = FuncObj_NormalC::GetInstance();
	if (!nor || !nor->is_you_hua_)
		return hooker.GetCodeOrig();

	void* ret_addr;
	_asm mov ret_addr, offset LABEL_RET
	return ret_addr;
LABEL_RET:
	__asm xor eax, eax
	__asm retn 4
}

void* Hooker_DxPress(IHooker& hooker, JmpHookerAmbient& ambient)
{
	//按下F1
	if (ambient.eax_ == 0x3B && *(const bool*)&ambient.ecx_ == true)
	{
	}
	return hooker.GetCodeOrig();
}

//#define IS_NO_HOOK
bool InitCodeHooker_Pre( HookerPtrStored& hooks )
{
	auto& anti_crc = CmnX_AntiHookCheck::GetRealMe();

#ifdef _DEBUG
	static HookerLogSink hooker_log_sink;
	anti_crc.SetLogSink(&hooker_log_sink);
	anti_crc.EnableOutputHookName(true);
#else
	CmnX_AntiHookCheck::GetRealMe().EnableOutputHookName(false);
#endif
	//////////////////////////////////////////////////////////////////////////
	//通过hook memcpy的方式反遍历自己的模块不可取，因为有些代码会访问本模块常量，而执行这些代码的线程却不知道是哪个。
	/*auto self_module_handle = GlobalSetter::GetMe().GetSelfModuleHandle();
	assert(self_module_handle);
	unsigned char buffer_anti_self_module[kAntiHookCheck_DefHookCpySize];
	Cmnx_MemSetValue(buffer_anti_self_module, sizeof(buffer_anti_self_module), 0xcc);
	anti_crc.AddDirectHookCodeInfo("antiselfmodule", (void*)self_module_handle, CmnImageHlp::GetPEImageSize(self_module_handle), buffer_anti_self_module, kAntiHookCheck_DefHookCpySize);*/
	//////////////////////////////////////////////////////////////////////////
#ifdef IS_NO_HOOK
	return true;
#endif

	auto& hooker_factory = HookerFactory::GetMe();
	HMODULE hlib = nullptr;
	FARPROC the_lib_addr = nullptr;
	//////////////////////////////////////////////////////////////////////////
	//一些软件会hook一些地方
	hlib = LibraryLoader::GetLib_Kernel32().GetLibrary();
	the_lib_addr = LibFuncLoader::GetTheProcAddress(hlib, "CancelIo");
	if (IsUnValidPtr(the_lib_addr))
	{
		assert(false);
		return false;
	}
	if ( !anti_crc.AddHookCodeInfo("CancelIo", the_lib_addr, 5) )
	{
		assert(false);
		return false;
	}

	the_lib_addr = LibFuncLoader::GetTheProcAddress(hlib, "CancelIoEx");
	if (IsUnValidPtr(the_lib_addr))
	{
		assert(false);
		return false;
	}
	if ( !anti_crc.AddHookCodeInfo("CancelIoEx", the_lib_addr, 5) )
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	hlib = LibraryLoader::GetLib_Ntdll().GetLibrary();
	if ( !hlib )
	{
		assert(false);
		return false;
	}
	the_lib_addr = LibFuncLoader::GetTheProcAddress(hlib, "NtReadVirtualMemory");
	if (IsUnValidPtr(the_lib_addr))
	{
		assert(false);
		return false;
	}
	struct tagNtReadVirtualMemory{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagNtReadVirtualMemory, (void*)the_lib_addr,
		Hooker_NtReadVirtualMemory, true));

	the_lib_addr = LibFuncLoader::GetTheProcAddress(hlib, "memcpy");
	if (IsUnValidPtr(the_lib_addr))
	{
		assert(false);
		return false;
	}
	//LOG_O底层会调用memcpy的
	//LOG_O(Log_trace) << "开始hook memcpy";
	//CopyMemory和RtlCopyMemory底层用的都是memcpy。
	struct tagMemcpy{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagMemcpy, (void*)the_lib_addr,
		Hooker_Memcpy, true));
	//LOG_O(Log_trace) << "hook memcpy结束";
	//////////////////////////////////////////////////////////////////////////
	bool DoAntiCheck(HookerPtrStored& hooks);
	if (!DoAntiCheck(hooks))
		return false;

	//hook ShowCursor
	hlib = LibraryLoader::GetLib_User32().GetLibrary();
	if ( !hlib )
	{
		assert(false);
		return false;
	}
	the_lib_addr = LibFuncLoader::GetTheProcAddress(hlib, "ShowCursor");
	if (IsUnValidPtr(the_lib_addr))
	{
		assert(false);
		return false;
	}
	struct tagShowCursor{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagShowCursor, (void*)the_lib_addr,
		Hooker_ShowCursor, true));

	//UPDATES_MMOCLIENT_REVERSAL_GAME_CREATEWINDOW_HOOK
	//game main wnd
	hooks.StoreHook(hooker_factory.CreateCodeHooker<
		(void*)(int)g_h_MainWndAddr_>(
		"Hooker_GainMainWndHandle", Hooker_GainMainWndHandle, true));

	//////////////////////////////////////////////////////////////////////////
	///////////////////////********优化******************************
	//优化渲染线程
	hooks.StoreHook(hooker_factory.CreateCodeHooker<
		(void*)(int)g_h_lpkb_render_>(
		"Hooker_Render", Hooker_Render, true));
	//////////////////////////////////////////////////////////////////////////
	//hook dx按键
	/*hooks.StoreHook(hooker_factory.CreateCodeHooker<
		(void*)(int)g_h_lpkb_dx_keyboard_press_>(
		"Hooker_DxPress", Hooker_DxPress, true));*/
	return true;
}

bool WaitForTheLib( const char* str_lib )
{
	auto dll_cnt = 0;
	const auto kWaitMaxCnt = 100;
	TimeDuration time_dur(1000);
	for ( ; dll_cnt != kWaitMaxCnt; ++dll_cnt )
	{
		if (GetModuleHandleA(str_lib))
			break;
		time_dur.WaitWithRunOne();
	}
	if ( dll_cnt >= kWaitMaxCnt )
	{
		return false;
	}
	return true;
}

//Init code hooker
bool InitCodeHooker( HookerPtrStored& hooks )
{
#ifdef IS_NO_HOOK
	return true;
#endif
	//return true;
	//////////////////////////////////////////////////////////////////////////
	auto& hooker_factory = HookerFactory::GetMe();
	HMODULE hlib = nullptr;
	FARPROC the_lib_addr = nullptr;
	//////////////////////////////////////////////////////////////////////////
	if ( !WaitForTheLib("CryGame.dll") )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "特别严重的错误968";
		return false;
	}
	LOG_O(Log_trace) << "CryGame.dll load succeed";
	if ( !WaitForTheLib("CryAction.dll") )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "特别严重的错误969";
		return false;
	}
	LOG_O(Log_trace) << "CryAction.dll load succeed";
	if (!WaitForTheLib("MHOClientBase.dll"))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "特别严重的错误96a";
		return false;
	}
	LOG_O(Log_trace) << "MHOClientBase.dll load succeed";
	if (!WaitForTheLib(kD3d9DllName))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "特别严重的错误96a";
		return false;
	}
	LOG_O(Log_trace) << kD3d9DllName << " load succeed";

	//return true;
	//////////////////////////////////////////////////////////////////////////
	/*struct tagAntiCrcClientBase{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagAntiCrcClientBase, (void*)CDF_Offset2MhoClientBaseAdd(g_h_AntiCrc_ClientBase_),
		Hooker_AntiCrcClientBase, true));*/

	//UPDATES_MMOCLIENT_REVERSAL_HIT_BU_WEI_HOOK
	hooks.StoreHook(hooker_factory.CreateCodeHooker<
		(void*)(int)g_h_HookGainBodyPart_>(
		"Hooker_GainBodyParts", Hooker_GainBodyParts, true));
	//////////////////////////////////////////////////////////////////////////

	//UPDATES_MMOCLIENT_REVERSAL_RECV_HOOK
	struct tagHookRecv{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagHookRecv, (void*)Offset2DllAddr(g_h_pHookRecv_),
		Hooker_RecvPacket, true));
	//UPDATES_MMOCLIENT_REVERSAL_MONSTER_HP_CALL_HOOK
	struct tagHookGainMonster{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagHookGainMonster, (void*)Offset2DllAddr(g_h_HookGainMonsterHp_),
		Hooker_GainMonsterHp, true));
	//return true;
	//UPDATES_MMOCLIENT_REVERSAL_ACTOR_HIT_HP_CALL_HOOK
	struct tagGainHitHp{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagGainHitHp, (void*)Offset2DllAddr(g_h_HookGainHitHp_),
		Hooker_GainHitHp, true));
	//UPDATES_MMOCLIENT_REVERSAL_ACTOR_FEEB_HP_CALL_HOOK
	struct tagGainFeebHp{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagGainFeebHp, (void*)Offset2DllAddr(g_h_HookGainFeebHp_),
		Hooker_GainFeebHp, true));
	//UPDATES_MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK
	struct tagLoadMap1{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagLoadMap1, (void*)Offset2DllAddr(g_h_pLoadMap_Village_),
		Hooker_LoadMap1, true));
	//UPDATES_MMOCLIENT_REVERSAL_LOAD_MAP_END_HOOK2
	struct tagLoadMap2{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagLoadMap2, (void*)Offset2DllAddr(g_h_LoadMap_Fb_),
		Hooker_LoadMap2, true));
	//UPDATES_MMOCLIENT_REVERSAL_FB_GAME_OVER_WIN_HOOK
	struct tagGameOverWin{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagGameOverWin, (void*)Offset2DllAddr(g_h_FbGameOver_Win_),
		Hooker_FbGameOver_Win, true));
	//UPDATES_MMOCLIENT_REVERSAL_FB_GAME_OVER_LOSE_HOOK
	struct tagGameOverLose{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagGameOverLose, (void*)Offset2DllAddr(g_h_FbGameOver_Lose_),
		Hooker_FbGameOver_Lose, true));
	//UPDATES_MMOCLIENT_REVERSAL_RET_ROLE_CHOOSE_JIE_MIAN_HOOK
	struct tagRoleScene{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagRoleScene, (void*)Offset2DllAddr(g_h_ChooseRoleScene_),
		Hooker_ChooseRoleScene, true));
	//UPDATES_MMOCLIENT_REVERSAL_SPRINTF_JU_QING_STRING_HOOK
	struct tagJuQing{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagJuQing, (void*)Offset2DllAddr(g_h_HookJuQing_),
		Hooker_JuQing, true));
	/*struct tagModifyBodyPart{};
	hooks.StoreHook(hooker_factory.CreateCodeHooker<tagModifyBodyPart>((void*)Offset2DllAddr(g_pHookModifyHitForBodyPart),
		Hooker_ModifyHitForBodyPart, true));*/
	//UPDATES_MMOCLIENT_REVERSAL_DIALOG_HOOK
	struct tagDlgInfo{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagDlgInfo, (void*)Offset2DllAddr(g_h_HookDlgInfo_),
		Hooker_DlgInfo, true));
	//UPDATES_MMOCLIENT_REVERSAL_SEND_CALL
	struct tagSendOnly{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagSendOnly, (void*)Offset2DllAddr(g_h_HookGameSendOnly_),
		Hooker_GameSendOnly, true));
	//UPDATES_MMOCLIENT_REVERSAL_COUNT_DOWN_HOOK
	struct tagAmbientStr{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagAmbientStr, (void*)Offset2DllAddr(g_h_HookAmbientStr_),
		Hooker_AmbientStr, true));
	//UPDATES_MMOCLIENT_REVERSAL_GUOTU_CODE_HOOK
	struct tagGamePassRoom{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagGamePassRoom, (void*)Offset2DllAddr(g_h_HookGamePassRoom_),
		Hooker_GamePassRoom, true));
	//UPDATES_MMOCLIENT_REVERSAL_CITY_GUOTU_CODE_HOOK
	struct tagGamePassVillage{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagGamePassVillage, (void*)Offset2DllAddr(g_h_HookGamePassVillage_),
		Hooker_GamePassVillage, true));
	//UPDATES_MMOCLIENT_REVERSAL_HIGHIGHT_HOOK
	struct tagJingCaiJingTou{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagJingCaiJingTou, (void*)Offset2DllAddr(g_h_HookJingCaiJingTou_),
		Hooker_JingCaiJingTou, true));
	//无敌
	//UPDATES_MMOCLIENT_REVERSAL_MONSTER_HIT_ADDR_HOOK2
	struct tagMonsterHit{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagMonsterHit, (void*)Offset2DllAddr(g_h_MonsterHit_),
		Hooker_MonsterHit, true));
	//远程无敌不能开，一开就会数据异常。以后有空再搞吧。
	//远程无敌
	//UPDATES_MMOCLIENT_REVERSAL_MONSTER_REMOTE_HIT_ADDR_HOOK
	struct tagMonsterRemoteHit{};
	/*hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagMonsterRemoteHit, (void*)Offset2DllAddr(g_h_MonsterRemoteHit_),
		Hooker_MonsterRemoteHit, true));*/
	//////////////////////////////////////////////////////////////////////////
	//反检测函数还就得放这，不然辅助无法正确初始化。
	GetIoService().dispatch([](){
		bool DoTraCheckThread();
		if (!DoTraCheckThread())
		{
			LOG_O(Log_error) << "游戏未能正确初始化";
			assert(false);
			throw CmnTerminateProcessException();
		}
	});
	//////////////////////////////////////////////////////////////////////////
	//return true;
	//在空中也可以打怪
	pt_byte sky_use_skill[] = { 0xEB };
	static_assert(sizeof(sky_use_skill) == 1, "");
	//UPDATES_MMOCLIENT_REVERSAL_KONG_ZHONG_USE_SKILL_JMP_ADDR
	if (!WriteMemory_AntiCrc("空中打怪", (void*)Offset2DllAddr(g_p_sky_use_skill_jmp_), sky_use_skill, sizeof(sky_use_skill)))
	{
		assert(false);
		return false;
	}

	//处理泥人
	const unsigned char code_nop[] = { 0x90, 0x90 };
	//UPDATES_REVERSAL_KILLNIREN_ADDR_HOOK
	if (!WriteMemory_AntiCrc("handle ni ren", (void*)Offset2DllAddr(g_p_HandleNiRen_), code_nop, sizeof(code_nop)))
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	//黑屏优化
	struct tagBlackScreen{};
	hooks.StoreHook(hooker_factory.HOOKER_CreateCodeHooker(tagBlackScreen, (void*)CDF_Offset2CryRenderD3d9(g_h_lpkb_black_screen_render_),
		Hooker_BlackScreen, true));

	return true;
	//倍攻
	//::MMO_HookHit();

	//优化
	//MMO_Render(0x70);

	//add buff
	//MMO_HookAddBuffer();

	//得到地面z坐标
	//MMO_HookMapCoorDinates();
	return true;
}
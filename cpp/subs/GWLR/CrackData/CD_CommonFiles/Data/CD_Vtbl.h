#pragma once
/*
@author zhp
@date 2016/4/14 21:27
@purpose for vtbl
ukn->unknown
*/
#include <ProtocolX/P_DefinesX.h>
#include <cstddef>


#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////

struct stCD_Vtbl_ObjMgr{
	pt_byte				ukn1_[0x14];
	pt_pcvoid			fn_get_obj_by_packet_id_;		typedef pt_pvoid (pc_this* Fn_get_obj_by_packet_id_)( pt_dword packet_id );
};
static_assert(offsetof(stCD_Vtbl_ObjMgr, fn_get_obj_by_packet_id_) == 0x14, "");

struct stCD_Vtbl_ObjCont{
	pt_byte				ukn1_[0x14];
	pt_pcvoid			fn_get_obj_by_packet_id_;		typedef pt_pvoid (pc_this*Fn_get_obj_by_packet_id_)( pt_dword packet_id ); /*调用这样的函数来实现的:*/ typedef void (pc_this* Fn_get_obj_by_packet_id_1)( pt_pvoid* out_obj_addr, pt_dword* in_packet_id );
};
//////////////////////////////////////////////////////////////////////////
struct stCD_Vtbl_Common_ThrdStart{
	pt_pcvoid			ukn_;		//析构函数
	pt_pcvoid			fn_run_thrd_;
	pt_pcvoid			ukn1_;
	pt_pcvoid			fn_on_exit_thrd_;
};

struct stCD_Vtbl_CryGame_ThrdStart{
	pt_pcvoid			ukn_;
	pt_pcvoid			fn_run_thrd_;
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

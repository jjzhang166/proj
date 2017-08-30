#include "stdafx.h"
#include "CD_FuncCall.h"
#include <cassert>
#include "CD_Addr.h"
#include <CommonX/CmnX_UsefulFunc.h>
#include <CmnMix/Cmn_MemOper.h>
#include "CD_UsefulFns.h"
#include "CD_Const.h"
#include "CD_ProtocolS.h"
#include "CD_Traverse.h"
//////////////////////////////////////////////////////////////////////////
struct MemOper_DoOther_CryGame : MemOper_DoElse{
	void* RetrieveAddr(const MemOperBase* mem_oper_base){
		return (pt_sz)CDF_Offset2CryGameAddr((pt_dword)mem_oper_base->addr_);
	}
};
struct MemOper_DoOther_CryAction : MemOper_DoElse{
	void* RetrieveAddr(const MemOperBase* mem_oper_base){
		return (pt_sz)CDF_Offset2CryActionAddr((pt_dword)mem_oper_base->addr_);
	}
};
/*
这个函数比较小，只返回全局地址内容，其内容全局唯一，所以任意线程任意调用。
类似：
0C09F580    A1 6CC2870C     MOV EAX, DWORD PTR DS:[C87C26C]
0C09F585    C3              RETN
*/
const MemOper<Oper_StdCall, MemOper_DoOther_CryGame>		g_c_GlobalData(g_c_GetGlobalData_);			//只读，线程安全
const MemOper<Oper_ThisCall, MemOper_DoOther_CryAction>		g_c_FindNetObj(g_c_FindNetObj_);	//UPDATES_MMOCLIENT_REVERSAL_GAME_OBJ_CALL
//const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_FindLocalObj(g_c_FindLocalObj_);			//UPDATES_MMOCLIENT_REVERSAL_GET_ACTOR_OBJADDR_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_decrypt_int(g_c_decrypt_int_);			//UPDATES_MMOCLIENT_REVERSAL_DECRY_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_encrypt_int(g_c_encrypt_int_);			//UPDATES_MMOCLIENT_REVERSAL_ENCRYPTION_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindItemDb(g_c_FindItemDb_);			//UPDATES_MMOCLIENT_REVERSAL_GET_ITEM_INFO_ADDR_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindTaskDb(g_c_FindTaskDb_);			//UPDATES_MMOCLIENT_REVERSAL_GET_TASK_OBJ_ADDR_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindMapDb(g_c_FindMapDb_);			//UPDATES_MMOCLIENT_REVERSAL_GET_FB_INFO_ADDR_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_AnyNpcHasTheTask(g_c_AnyNpcHasTheTask_);			//UPDATES_MMOCLINET_REVERSAL_GAME_GET_CAN_ACCPET_TASK_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_GetChapterInfo(g_c_GetChapterInfo_);			//UPDATES_MMOCLIENT_REVERSAL_ZHU_XIAN_TASK_FB_INFO_ADDR_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_IsTheFbDone(g_c_IsTheFbDone_);			//UPDATES_MMOCLIENT_REVERSAL_IS_IN_TASK_FB_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_GetFarmDbId(g_c_GetFarmDbId_);			//UPDATES_MMOCLIENT_REVERSAL_GET_FARM_FACILITY_ID_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindFarmDb(g_c_FindFarmDb_);			//UPDATES_MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindFarmTimeDb(g_c_FindFarmTimeDb_);			//UPDATES_MMOCLIENT_REVERSAL_FARM_CP_INFO_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_GetFarmFiledLeftTime(g_c_GetFarmFiledLeftTime_);			//UPDATES_MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_TIME_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindShopDb(g_c_FindShopDb_);			//UPDATES_MMOCLIENT_REVERSAL_GET_EQUIP_INF0_BASE_ADDR_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindSeedDb(g_c_FindSeedDb_);			//UPDATES_MMOCLIENT_REVERSAL_FARM_SEED_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindCraftDb(g_c_FindCraftDb_);			//UPDATES_MMOCLIENT_REVERSAL_GET_CRAFT_INFO_ADDR_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_GetMenufactureObj(g_c_GetMenufactureObj_);			//UPDATES_MMOCLIENT_REVERSAL_GET_MANU_FACTURE_INFO_ADDR_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_FindMenufactureDb(g_c_FindMenufactureDb_);			//UPDATES_MMOCLIENT_REVERSAL_GET_UI_INFO_ADDR_CALL
const MemOper<Oper_ThisCall, MemOper_DoVtbl<g_vo_net_obj_ukn_supply_ukn2_> >		g_c_net_obj_ukn_supply_ukn2;		//MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET2___
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindBuffDb(g_c_FindBuffDb_);			//UPDATES_MMOCLIENT_REVERSAL_GET_BUFF_INFO_ADDR_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_SendMsg(g_c_SendMsg_);			//UPDATES_MMOCLIENT_REVERSAL_SEND_CALL2
const MemOper<Oper_ThisCall, MemOper_DoVtbl<g_vo_shang_jin_mgr_shang_jin_array_> >		g_c_shang_jin_mgr_shang_jin_array;		//UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_CALL_OFFSET
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_GetKingQuestInfo(g_c_GetKingQuestInfo_);			//UPDATES_MMOCLIENT_REVERSAL_GET_KING_QUEST_INFO_ADDR_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_GetItemSellPrice(g_c_GetItemSellPrice_);			//UPDATES_MMOCLIENT_REVERSAL_GET_SELL_ITEM_PRICE_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_UseSkill(g_c_UseSkill_);			//UPDATES_MMOCLIENT_REVERSAL_SKILL_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_GetWeekCreditInfo(g_c_GetWeekCreditInfo_);			//UPDATES_MMOCLIENT_REVERSAL_WEEK_CREDIT_INFO_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_GetWeekCreditCnt(g_c_GetWeekCreditCnt_);			//UPDATES_MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_CALL
const MemOper<Oper_Cdecl, MemOper_DoOther_CryGame>			g_c_FindReqItem(g_c_FindReqItem_);			//UPDATES_MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_CALL
const MemOper<Oper_ThisCall, MemOper_DoVtbl<g_svo_ui_system_handle_mouse_msg> >		g_c_handle_mouse_msg;
const MemOper<Oper_ThisCall>								g_c_handle_key_msg(g_c_handle_key_msg_);			//UPDATES_MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_CALL
const MemOper<Oper_ThisCall, MemOper_DoVtbl<g_vo_shang_jin_mgr_shang_jin_info_> >		g_c_shang_jin_mgr_shang_jin_info;		//UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_COUNT_CALL_OFFSET
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_ShangJinTotalCnt(g_c_ShangJinTotalCnt_);			//UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_COUNT_TOTAL_CALL
//g_c_shang_jin_task_state_	UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_STATUS_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_TidyItemSize(g_c_TidyItemSize_);			//UPDATES_MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_SIZE_CALL
const MemOper<Oper_ThisCall, MemOper_DoOther_CryGame>		g_c_TidyItem(g_c_TidyItem_);			//UPDATES_MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_CALL
const MemOper<Oper_ThisCall, MemOper_DoVtbl<g_svo_SynCoorDinatesRandom1> >		g_c_SynCoorDinatesRandom1;
const MemOper<Oper_ThisCall, MemOper_DoVtbl<g_svo_SynCoorDinatesRandom2> >		g_c_SynCoorDinatesRandom2;
const MemOper<Oper_ThisCall, MemOper_DoVtbl<g_svo_ui_ctrl_handle_msg> >		g_c_ui_ctrl_handle_msg;
const MemOper<Oper_ThisCall>								g_c_UiCtrlEditClrStr(g_c_UiCtrlEditClrStr_);			//UPDATES_REVERSAL_UI_CLAENSTRING_CALL
const MemOper<Oper_ThisCall, MemOper_DoVtbl<g_svo_ui_ctrl_edit_put_char> >		g_c_ui_ctrl_edit_put_char;
//////////////////////////////////////////////////////////////////////////

pc_naked void CDPF_EncryptMsgBuffer(pt_pvoid call_addr, pt_dword msg_id, stCD_EncryptMsgInfo* msg_info, pt_dword main_id, void* msg_addr){
	__asm{
		push ebp;
		mov ebp, esp;
		pushad;

		mov edx, msg_addr;
		mov ecx, main_id;
		mov eax, call_addr;
		push msg_info;
		push 0;
		push msg_id;
		call eax;

		popad;
		pop ebp;
		ret;
	}
}

int stCD_MsgBase::GetMsgLength() const
{
	auto addr = (pt_pvoid)CDF_Offset2CryGameAddr(g_c_EncryptMsgBuffer_);
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return -1;
	}
	stCD_EncryptMsgInfo msg_info = {
		&CD_FuncCall::GetEncodeMsgBuffer(), 0, kMaxSendBufferSize - sizeof(stCD_MsgBase)
	};
	//FnForOd("3413");
	//这个函数没有改写全局变量，是线程安全的
	CDPF_EncryptMsgBuffer((pt_pvoid)addr, msg_id_, &msg_info, kMainMsgId, MsgAs<void>());
	return msg_info.msg_size_;
}

stCD_GlobalData* CD_FuncCall::GetGlobalData()
{
	auto res = g_c_GlobalData.Call<stCD_GlobalData*>();
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_NetObjRealCont* CD_FuncCall::GetAllNetObjs()
{
	auto obj_mgr = GetNetObjsMgr();
	if (IsUnValidPtr(obj_mgr))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(obj_mgr->obj_cont_))
	{
		assert(false);
		return nullptr;
	}
	return &obj_mgr->obj_cont_->obj_cont_;
}

stCD_NetObjMgr* CD_FuncCall::GetNetObjsMgr()
{
	auto gd = GetGlobalData();
	if (!gd)
		return nullptr;
	if (IsUnValidPtr(gd->obj_mgr_))
	{
		assert(false);
		return nullptr;
	}
	return gd->obj_mgr_;
}

stCD_LocalObjNode* CD_FuncCall::GetLocalObjsNode()
{
	auto tmp = GetGloLocalIdMgr();
	if (!tmp)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(tmp->the_mgr_))
	{
		assert(false);
		return nullptr;
	}
	auto mgr = tmp->the_mgr_;
	if (IsUnValidPtr(mgr->the_mgr_))
	{
		assert(false);
		return nullptr;
	}
	return &mgr->the_mgr_->objs_;
}

stCD_NetObj* CD_FuncCall::FindNetObj(pt_dword packet_id)
{
	if (kInvalidPacketId == packet_id)
		return nullptr;
	auto addr = (void**)CDF_Offset2CryActionAddr(g_a_global_net_obj_mgr_);	//UPDATES_MMOCLIENT_REVERSAL_GAME_OBJ_ADDR
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return nullptr;
	}
	auto this_addr = *addr;
	if (IsUnValidPtr(this_addr))
	{
		assert(false);
		return nullptr;
	}

	auto res = g_c_FindNetObj.Call<stCD_NetObj*>(this_addr, packet_id);
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_NetObj_Role* CD_FuncCall::GetCurRole()
{
	auto the_ptr = GetUknRoleInfo1();
	if (!the_ptr)
		return nullptr;
	if (IsUnValidPtr(the_ptr->ukn_role_info_))
	{
		assert(false);
		return nullptr;
	}
	auto role = the_ptr->ukn_role_info_->role_;
	if (IsUnValidPtr(role))
	{
		assert(false);
		return nullptr;
	}
	return role;
}

stCD_NodeT<void>* stCD_NodeT<void>::FindById(int id) const
{
	if (IsUnValidPtr(parent_))
		return nullptr;
	if (this == parent_)
		return 0;

	std::stack<stCD_NodeT<void>*> btree_stack;
	btree_stack.push(parent_->CastTo<stCD_NodeT<void> >());
	while (!btree_stack.empty())
	{
		auto cur_node_ptr = btree_stack.top();
		while (true)
		{
			if (!cur_node_ptr || cur_node_ptr == this)
				break;
			if (cur_node_ptr->id_ == id)
				return cur_node_ptr;

			btree_stack.push(cur_node_ptr->left_->CastTo<stCD_NodeT<void> >());
			cur_node_ptr = cur_node_ptr->left_->CastTo<stCD_NodeT<void> >();
		}
		btree_stack.pop();
		if (!btree_stack.empty())
		{
			cur_node_ptr = btree_stack.top();
			btree_stack.pop();
			btree_stack.push(cur_node_ptr->right_->CastTo<stCD_NodeT<void> >());
		}
	}
	return nullptr;
}

stCD_LocalObj* CD_FuncCall::FindLocalObj(pt_dword local_id)
{
	auto root_node = GetLocalObjsNode();
	if (IsUnValidPtr(root_node))
	{
		assert(false);
		return nullptr;
	}
	return root_node->FindTheObj(local_id);
	/*stCD_LocalObjNode* res = nullptr;
	g_c_FindLocalObj.Call<void>(root_node, &res, local_id);
	if (IsUnValidPtr(res))
		return nullptr;
	if (res == root_node)
		return nullptr;
	if (IsUnValidPtr(res->local_obj_))
		return nullptr;
	if (res->id_ != local_id)
	{
		assert(false);
		return nullptr;
	}
	return res->local_obj_;*/
}

int CD_FuncCall::DecryptValue(const stCD_EncryptInt* addr)
{
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return -1;
	}
	return g_c_decrypt_int.Call<int>(addr);
}

void CD_FuncCall::EncryptInt(const stCD_EncryptInt* addr, int value)
{
	g_c_encrypt_int.Call<void>(addr, &value);
}

stCD_MapDb* CD_FuncCall::GetCurMapDb()
{
	auto the_ptr = GetUknRoleInfo1();
	if (!the_ptr)
		return nullptr;
	if (IsUnValidPtr(the_ptr->cur_map_db_))
	{
		assert(false);
		return nullptr;
	}
	return the_ptr->cur_map_db_;
}

stCD_UknRoleInfo1* CD_FuncCall::GetUknRoleInfo1()
{
	auto tmp = GetGloRoleInfoData();
	if (!tmp)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(tmp->ukn_role_info_))
	{
		assert(false);
		return nullptr;
	}
	return tmp->ukn_role_info_;
}

stCD_PackagesMgr* CD_FuncCall::GetPackagesMgr()
{
	auto role = GetCurRole();
	if (!role)
		return nullptr;
	if (IsUnValidPtr(role->packages_mgr_))
	{
		assert(false);
		return nullptr;
	}
	return role->packages_mgr_;
}

stCD_ItemDb* CD_FuncCall::FindItemDb(int item_id)
{
	auto res = g_c_FindItemDb.Call<stCD_ItemDb*>(item_id);
	if (IsUnValidPtr(res))
	{
		return nullptr;
	}
	return res;
}

stCD_TaskDb* CD_FuncCall::FindTaskDb(int task_id)
{
	auto res = g_c_FindTaskDb.Call<stCD_TaskDbUknObj*>(task_id);
	if (IsUnValidPtr(res))
	{
		return nullptr;
	}
	if (IsUnValidPtr(res->task_db_))
	{
		assert(false);
		return nullptr;
	}
	return res->task_db_;
}

stCD_MapDb* CD_FuncCall::FindMapDb(int fb_id)
{
	auto res = g_c_FindMapDb.Call<stCD_MapDb*>(fb_id);
	if (IsUnValidPtr(res))
	{
		return nullptr;
	}
	return res;
}

stCD_TaskNode* CD_FuncCall::GetReceivedTaskRootNode()
{
	auto addr_tmp = CDF_Offset2CryGameAddr(g_a_global_received_task_);	//UPDATES_MMOCLIENT_REVERSAL_TASK_BASE_ADDR
	if (IsUnValidPtr(addr_tmp))
		return nullptr;
	auto tmp = *(stCD_GloReceivedTask**)(addr_tmp);
	if (IsUnValidPtr(tmp))
	{
		assert(false);
		return nullptr;
	}
	return &tmp->task_root_node_;
}

bool CD_FuncCall::CanAcceptTheTask(int task_id)
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(role->npc_mgr_))
	{
		assert(false);
		return false;
	}
	if (!role->npc_mgr_->IsAnyNpcHasTheTask(task_id))
		return false;
	auto task_db = FindTaskDb(task_id);
	if (!task_db)
	{
		assert(false);
		return false;
	}
	auto lvl_info = task_db->GetRoleLvlInfo();
	if (!lvl_info)
		return false;
	if (!lvl_info->InRange(role->lvl_))
		return false;
	return true;
}

bool CD_FuncCall::CanAcceptTheTask(const stCD_TaskDb& task_db)
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(role->npc_mgr_))
	{
		assert(false);
		return false;
	}
	if (!role->npc_mgr_->IsAnyNpcHasTheTask(task_db.task_id_))
		return false;
	auto lvl_info = task_db.GetRoleLvlInfo();
	if (!lvl_info)
		return false;
	if (!lvl_info->InRange(role->lvl_))
		return false;
	return true;
}

CD_FuncCall::ChapterInfoT* CD_FuncCall::GetChapterInfo()
{
	//GetMianBanTaskFbInfoBaseAddr
	auto res = g_c_GetChapterInfo.Call<ChapterInfoT*>(int(1));
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

bool CD_FuncCall::IsTheFbDone(int fb_id)
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(role->done_fb_mgr_))
	{
		assert(false);
		return false;
	}
	return role->done_fb_mgr_->IsTheFbDone(fb_id);
}

stCD_EnterableFbMgr* CD_FuncCall::GetEnterableFbMgr()
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(role->enterable_fb_mgr_))
	{
		assert(false);
		return false;
	}
	return role->enterable_fb_mgr_;
}

int CD_FuncCall::GetFarmDbId(CD_En_FarmFacilityIdx facility_idx, int facility_lvl)
{
	return g_c_GetFarmDbId.Call<int>(0/*庄园的id,0表示自己的*/,
		facility_idx, facility_lvl);
}

stCD_FarmDb* CD_FuncCall::FindFarmDb(int farm_db_id)
{
	auto res = g_c_FindFarmDb.Call<stCD_FarmDb*>(farm_db_id);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

//这个函数不是获取db的，而是判断成熟时间的吧
stCD_FarmTimeDb* CD_FuncCall::FindFarmTimeDbBy(int farm_db_id)
{
	auto res = g_c_FindFarmTimeDb.Call<stCD_FarmTimeDb*>(farm_db_id);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

int CD_FuncCall::GetFarmLeftTime(int farm_db_id, int cur_time)
{
	if (cur_time <= 0)
		return 0;

	// 获取田地模板数据
	auto time_db = FindFarmTimeDbBy(farm_db_id);
	if (!time_db)
	{
		//assert(false);
		return 0;
	}

	/*++
	$ ==>    0C938ED6      E8 A19DDF00     CALL <JMP.&MSVCR90._time64>
	$+5      0C938EDB      8B5424 10       MOV EDX,DWORD PTR SS:[ESP+0x10]
	$+9      0C938EDF      8B4A 1C         MOV ECX,DWORD PTR DS:[EDX+0x1C]
	$+C      0C938EE2      8BD1            MOV EDX,ECX
	$+E      0C938EE4      C1E2 04         SHL EDX,0x4
	$+11     0C938EE7      2BD1            SUB EDX,ECX
	$+13     0C938EE9      8B4C24 14       MOV ECX,DWORD PTR SS:[ESP+0x14]
	$+17     0C938EED      8B49 04         MOV ECX,DWORD PTR DS:[ECX+0x4]
	$+1A     0C938EF0      8D0C91          LEA ECX,DWORD PTR DS:[ECX+EDX*4]
	$+1D     0C938EF3      2BC8            SUB ECX,EAX
	--*/
	// 获取采集点植物成熟周期时间
	int grown_time = time_db->grown_time_;

	pt_qword left_time = grown_time << 4;
	left_time -= grown_time;
	left_time *= 4;
	left_time += cur_time;
	left_time -= _time64(0);
	return (int)left_time;
}

stCD_FarmFacility* CD_FuncCall::GetCurFarmFacility()
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return nullptr;
	}
	return &role->farm_facility_;
}

int CD_FuncCall::GetFarmFieldLeftTime(stCD_FarmFacility* farm_facility, CD_En_FarmFieldIdx idx)
{
	return g_c_GetFarmFiledLeftTime.Call<int>(farm_facility, idx);
}

stCD_ShopDb* CD_FuncCall::FindShopDb(CD_En_ShopInfoCategory shop_db_id)
{
	auto res = g_c_FindShopDb.Call<stCD_ShopDb*>(shop_db_id);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

stCD_SeedDb* CD_FuncCall::FindSeedDb(int item_id)
{
	auto res = g_c_FindSeedDb.Call<stCD_SeedDb*>(item_id);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

stCD_CraftDb* CD_FuncCall::FindCraftDb(int craft_id)
{
	auto res = g_c_FindCraftDb.Call<stCD_CraftDb*>(craft_id);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

stCD_MenufactureObj* CD_FuncCall::GetMenufactureObj()
{
	auto res = g_c_GetMenufactureObj.Call<stCD_MenufactureObj*>(0, (pt_csz)"CManufactureInfo", 0);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

stCD_MenufactureDb* CD_FuncCall::FindMenufactureDb(int id)
{
	auto obj = GetMenufactureObj();
	if (!obj)
		return nullptr;
	return obj->FindMenufactureDb(id);
}

stCD_GiftNode* CD_FuncCall::GetGiftRootNode()
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(role->gift_bag_))
	{
		assert(false);
		return nullptr;
	}
	return &role->gift_bag_->gift_root_node_;
}

stCD_GridItemNode* CD_FuncCall::GetSupplyFirstItemNode()
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(role->ukn_))
	{
		assert(false);
		return nullptr;
	}
	auto ptr = role->ukn_->GetSupplyUkn();
	if (!ptr)
	{
		assert(false);
		return nullptr;
	}
	auto ptr1 = ptr->ukn_.ukn_;
	if (IsUnValidPtr(ptr1))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(ptr1->ukn_))
	{
		assert(false);
		return nullptr;
	}
	return &ptr1->ukn_->first_item_node_;
}

stCD_MailNode* CD_FuncCall::GetMailRootNode()
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(role->mail_mgr_))
	{
		assert(false);
		return nullptr;
	}
	return &role->mail_mgr_->mail_root_node_;
}

stCD_BuffDb* CD_FuncCall::FindBuffDb(int id)
{
	auto res = g_c_FindBuffDb.Call<stCD_BuffDb*>(id);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

stCD_BuffMgr* CD_FuncCall::GetBuffMgr()
{
	auto role = GetCurRole();
	if (!role)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(role->role_buf_mgr_))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(role->role_buf_mgr_->buf_mgr_))
	{
		assert(false);
		return nullptr;
	}
	return role->role_buf_mgr_->buf_mgr_;
}

stCD_AllLocalObjMgr* CD_FuncCall::GetAllLocalObjMgr()
{
	auto tmp = GetGloRoleInfoData();
	if (!tmp)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(tmp->all_local_obj_mgr_))
	{
		assert(false);
		return nullptr;
	}
	return tmp->all_local_obj_mgr_;
}

stCD_GloRoleInfoData* CD_FuncCall::GetGloRoleInfoData()
{
	auto addr_tmp = CDF_Offset2CryGameAddr(g_a_global_role_info_data_);	//UPDATES_MMOCLIENT_REVERSAL_GAME_ADDR	UPDATES_MMOCLIENT_REVERSAL_GWLR_WINDOWS_BASE_ADDR
	if (IsUnValidPtr(addr_tmp))
		return nullptr;
	auto tmp = *(stCD_GloRoleInfoData**)(addr_tmp);
	if (IsUnValidPtr(tmp))
	{
		assert(false);
		return nullptr;
	}
	return tmp;
}

stCD_SocketMgr3* CD_FuncCall::GetSocketMgr()
{
	auto tmp = GetGloLocalIdMgr();
	if (!tmp)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(tmp->socket_mgr_))
	{
		assert(false);
		return nullptr;
	}
	auto ptr = tmp->socket_mgr_->socket_mgr_;
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(ptr->socket_mgr_))
	{
		assert(false);
		return nullptr;
	}
	return ptr->socket_mgr_;
}

stCD_GloLocalIdMgr* CD_FuncCall::GetGloLocalIdMgr()
{
	auto addr_tmp = CDF_Offset2CryGameAddr(g_a_global_local_objs_);	//UPDATES_MMOCLIENT_REVERSAL_SOCKET_BASE_ADDR UPDATES_MMOCLIENT_REVERSAL_GAME_RAND_BASE_ADDR
	if (IsUnValidPtr(addr_tmp))
		return nullptr;
	auto tmp = *(stCD_GloLocalIdMgr**)(addr_tmp);
	if (IsUnValidPtr(tmp))
	{
		assert(false);
		return nullptr;
	}
	return tmp;
}

bool CD_FuncCall::SendImpl(stCD_MsgBase& msg)
{
	auto mgr = GetSocketMgr();
	if (!mgr)
	{
		assert(false);
		return false;
	}
	return mgr->SendMsg(msg);
}

bool CD_FuncCall::GetShangJinInfoArray(stCD_ShangJinArrayT& out_array)
{
	auto mgr = GetShangJinInfoMgr();
	if (!mgr)
	{
		assert(false);
		return false;
	}
	mgr->GetShangJinArray(out_array);
	return true;
}

stCD_KingQuestFbInfo* CD_FuncCall::GetKingQuestFbInfo(CD_En_KingFbType king_fb_type)
{
	auto res = g_c_GetKingQuestInfo.Call<stCD_KingQuestFbInfo*>(king_fb_type);
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

CD_MsgBufferType& CD_FuncCall::GetSendMsgBuffer()
{
	static CD_MsgBufferType buffer;
	return buffer;
}

CD_MsgBufferType& CD_FuncCall::GetEncodeMsgBuffer()
{
	static CD_MsgBufferType buffer;
	return buffer;
}

stCD_UseItemMgr* CD_FuncCall::GetUseItemMgr()
{
	auto addr = (stCD_UseItemMgr**)CDF_Offset2CryGameAddr(g_a_use_item_mgr_);	//UPDATES_MMOCLIENT_REVERSAL_USE_ITEM_OBJ_ADDR
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return nullptr;
	}
	auto the_mgr = *addr;
	if (IsUnValidPtr(the_mgr))
	{
		assert(false);
		return nullptr;
	}
	return the_mgr;
}

bool stCD_ShangJinInfoMgr::GetShangJinArray(stCD_ShangJinArrayT& out_array) const
{
	g_c_shang_jin_mgr_shang_jin_array.Call<void>(this, &out_array);
	return true;
}

stCD_MenufactureDb* stCD_MenufactureObj::FindMenufactureDb(int id) const
{
	auto res = g_c_FindMenufactureDb.Call<stCD_MenufactureDb*>(this, id);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

void FnForOd(const char* str)
{
	str += 1;
}

bool stCD_NpcMgr::IsAnyNpcHasTheTask(int task_id) const
{
	return g_c_AnyNpcHasTheTask.Call<bool>(this, task_id);
}


bool stCD_DoneFbMgr::IsTheFbDone(int fb_id) const
{
	/*++
	函数功能： 判断主线任务是否完成，判断正常副本是否完成黄金以上评价，判断喵喵副本是否完成

	返回=1 表示主线完成
	返回=1 表示此副本黄金评价以上
	返回=1 表示喵喵副本已经完成，不可再进

	--*/
	return g_c_IsTheFbDone.Call<bool>(this, fb_id);
}


stCD_SupplyUkn2* stCD_NetObjUkn::GetSupplyUkn() const
{
	auto res = g_c_net_obj_ukn_supply_ukn2.Call<stCD_SupplyUkn2*>(this);
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

bool stCD_SocketMgr3::SendMsg(stCD_MsgBase& msg) const
{
	return !g_c_SendMsg.Call<bool>(this, &msg);
}

int stCD_ItemDb::GetItemSellPrice() const
{
	return g_c_GetItemSellPrice.Call<int>(this, 1);
}

bool stCD_SkillObj::UseSkill(CD_En_SkillId skill_id) const
{
	DWORD dwValue[20];

	if (skill_id == CD_En_SkillId::kSC_KeyF)
	{
		dwValue[0] = 4;
		dwValue[1] = 1;
		dwValue[2] = (int)skill_id;
		dwValue[3] = 0xFFFFFFFF;
		dwValue[4] = 0xFFFFFFFF;
		dwValue[5] = 0xFFFFFFFF;
		dwValue[6] = 0xBF800000;
		dwValue[7] = 0;
		dwValue[8] = 0;
		dwValue[9] = 0;
		dwValue[10] = 2;
		dwValue[11] = 0x24;
		dwValue[12] = 0x17;
	}
	else
	{
		dwValue[0] = 4;
		dwValue[1] = 1;
		dwValue[2] = (int)skill_id;
		dwValue[3] = 0xFFFFFFFF;
		dwValue[4] = 0xFFFFFFFF;
		dwValue[5] = 0xFFFFFFFF;
		dwValue[6] = 0xBF800000;
		dwValue[7] = 0;
		dwValue[8] = 0;
		dwValue[9] = 0;
		dwValue[10] = 0;
		dwValue[11] = 0;
		dwValue[12] = 0;
	}
	g_c_UseSkill.Call<void>(this, dwValue);

	/*auto the_call = CDF_Offset2CryGameAddr((pt_dword)g_c_UseSkill.addr_);
	if (IsUnValidPtr(the_call))
	{
		assert(false);
		return false;
	}
	try
	{
		//调用这个函数会一直抛出异常，无所谓了，只要能达到效果即可
		__asm
		{
			mov ecx, this
			lea eax, dwValue
			push eax
			mov eax, the_call
			call eax
		}
	} DO_CATCH_ALL_EXCEPTION({

	})*/
	
	return true;
}

_declspec(naked)void GetHitSendDir_(const stCD_PosDirInfo* dwDirAddr, float* fdirx, float* fdiry)
{
	__asm
	{
		PUSH EBP
			MOV EBP, ESP
			AND ESP, 0xFFFFFFF0
			SUB ESP, 0x74
			PUSH EBX
			MOV EBX, DWORD PTR SS : [dwDirAddr]
			PUSH ESI
			PUSH EDI

			MOV ESI, DWORD PTR SS : [EBP + 0x8]
			MOVSS XMM7, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR1_]		//UPDATES_MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR1
			MOVSS XMM6, DWORD PTR DS : [ESI + 0x28]
			MOVSS XMM5, DWORD PTR DS : [ESI + 0x14]
			MOVSS XMM4, DWORD PTR DS : [ESI]
			MOVSS XMM3, DWORD PTR DS : [ESI + 0x8]
			MOVSS XMM0, DWORD PTR DS : [ESI + 0x10]
			MOVSS XMM2, DWORD PTR DS : [ESI + 0x20]
			MOVSS XMM1, DWORD PTR DS : [ESI + 0x24]
			MOVSS DWORD PTR SS : [ESP + 0x6C], XMM6
			MOVSS DWORD PTR SS : [ESP + 0x1C], XMM7
			MOVSS DWORD PTR SS : [ESP + 0x20], XMM7
			MOVSS DWORD PTR SS : [ESP + 0x24], XMM7
			XORPS XMM7, XMM7
			ADDSS XMM6, XMM5
			ADDSS XMM6, XMM4
			COMISS XMM6, XMM7
			//JBE SHORT 00951627
			JBE GO_TO_00951627

			ADDSS XMM6, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR1_]
			XORPS XMM4, XMM4
			MOVSS XMM4, XMM6
			SQRTSS XMM4, XMM4
			MOVAPS XMM5, XMM4
			MOVSS XMM4, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR2_]		//UPDATES_MMOCLIENT_REVERSAL_HIT_DIR_CALC_CALL_BASE_ADDR2
			DIVSS XMM4, XMM5
			MOVSS DWORD PTR SS : [ESP + 0x18], XMM5
			MOVSS XMM5, DWORD PTR DS : [ESI + 0x18]
			SUBSS XMM1, XMM5
			MULSS XMM1, XMM4
			MOVAPS XMM6, XMM1
			MOVSS XMM1, DWORD PTR DS : [ESI + 0x4]
			SUBSS XMM0, XMM1
			MULSS XMM0, XMM4
			MOVAPS XMM5, XMM0
			MOVSS XMM0, DWORD PTR SS : [ESP + 0x18]
			MULSS XMM0, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR2_]

			SUBSS XMM3, XMM2
			MULSS XMM3, XMM4
			MOVAPS XMM7, XMM3
			//JMP 00951777
			JMP GO_TO_00951777
		GO_TO_00951627 :


		COMISS XMM4, XMM5
			MOVSS XMM6, DWORD PTR SS : [ESP + 0x6C]
			//JB SHORT MHOClien.00951698
			JB GO_TO_00951698
			COMISS XMM4, XMM6
			//JB SHORT MHOClien.00951698
			JB GO_TO_00951698
			SUBSS XMM4, XMM5
			SUBSS XMM4, XMM6
			ADDSS XMM4, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR1_]
			MOVSS XMM6, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR2_]
			XORPS XMM5, XMM5
			MOVSS XMM5, XMM4
			SQRTSS XMM5, XMM5
			MOVAPS XMM4, XMM6
			DIVSS XMM4, XMM5
			MULSS XMM5, XMM6
			MOVAPS XMM6, XMM5
			MOVSS XMM5, DWORD PTR DS : [ESI + 0x4]
			ADDSS XMM0, XMM5
			MULSS XMM0, XMM4
			MOVAPS XMM7, XMM0
			MOVSS XMM0, DWORD PTR DS : [ESI + 0x18]
			SUBSS XMM1, XMM0
			ADDSS XMM2, XMM3
			MULSS XMM2, XMM4
			MULSS XMM1, XMM4
			MOVAPS XMM5, XMM2
			MOVAPS XMM0, XMM1
			//JMP MHOClien.00951777
			JMP GO_TO_00951777
		GO_TO_00951698 :
		COMISS XMM5, XMM4
			//JB SHORT MHOClien.00951703
			JB GO_TO_00951703
			COMISS XMM5, XMM6
			//JB SHORT MHOClien.00951703
			JB GO_TO_00951703
			MOVSS XMM7, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR2_]
			SUBSS XMM5, XMM6
			MOVSS XMM6, DWORD PTR DS : [ESI + 0x4]
			SUBSS XMM5, XMM4
			ADDSS XMM5, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR1_]
			XORPS XMM4, XMM4
			MOVSS XMM4, XMM5
			SQRTSS XMM4, XMM4
			MOVAPS XMM5, XMM4
			ADDSS XMM0, XMM6
			MOVAPS XMM4, XMM7
			DIVSS XMM4, XMM5
			MULSS XMM0, XMM4
			MOVAPS XMM6, XMM0
			MOVSS XMM0, DWORD PTR DS : [ESI + 0x18]
			MULSS XMM5, XMM7
			ADDSS XMM1, XMM0
			SUBSS XMM3, XMM2
			MOVAPS XMM7, XMM5
			MULSS XMM1, XMM4
			MULSS XMM3, XMM4
			MOVAPS XMM5, XMM1
			MOVAPS XMM0, XMM3
			//JMP SHORT MHOClien.00951777
			JMP GO_TO_00951777
		GO_TO_00951703 :
		COMISS XMM6, XMM4
			//JB SHORT MHOClien.00951769
			JB GO_TO_00951769
			COMISS XMM6, XMM5
			//JB SHORT MHOClien.00951769
			JB GO_TO_00951769
			SUBSS XMM6, XMM4
			SUBSS XMM6, XMM5
			ADDSS XMM6, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR1_]
			XORPS XMM4, XMM4
			MOVSS XMM4, XMM6
			SQRTSS XMM4, XMM4
			MOVAPS XMM5, XMM4
			MOVSS XMM4, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR2_]
			DIVSS XMM4, XMM5
			MULSS XMM5, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR2_]
			ADDSS XMM2, XMM3
			MULSS XMM2, XMM4
			MOVAPS XMM6, XMM2
			MOVSS XMM2, DWORD PTR DS : [ESI + 0x18]
			ADDSS XMM1, XMM2
			MULSS XMM1, XMM4
			MOVAPS XMM7, XMM1
			MOVSS XMM1, DWORD PTR DS : [ESI + 0x4]
			SUBSS XMM0, XMM1
			MULSS XMM0, XMM4
			//JMP SHORT MHOClien.00951777
			JMP GO_TO_00951777
		GO_TO_00951769 :
		MOVSS XMM0, DWORD PTR DS : [G_HIT_DIR_CALC_CALL_BASE_ADDR1_]
			MOVAPS XMM6, XMM7
			MOVAPS XMM5, XMM7
		GO_TO_00951777 :
		//取 XMMO=y和XMM5=x
		mov eax, dword ptr[fdirx]
			MOVSS DWORD PTR SS : [eax], XMM5

			mov eax, dword ptr[fdiry]
			MOVSS DWORD PTR SS : [eax], XMM0

			POP EDI
			POP ESI
			POP EBX
			MOV ESP, EBP
			POP EBP
			retn
	}
}

stCD_Direction2 CD_FuncCall::CalcHitSendDir(const stCD_PosDirInfo& pos_dir_info)
{
	stCD_Direction2 theDirection = { 0 };
	GetHitSendDir_(&pos_dir_info, &theDirection.dir_x, &theDirection.dir_y);
	return theDirection;
}

int CD_FuncCall::GetWeekCreditCnt()
{
	auto info = GetWeekCreditInfo();
	if (!info)
	{
		assert(false);
		return 0;
	}
	return info->GetWeekCreditCnt();
}

stCD_WeekCreditInfoImpl* CD_FuncCall::GetWeekCreditInfo()
{
	auto cur_role = GetCurRole();
	if (!cur_role)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(cur_role->week_credit_mgr_))
	{
		assert(false);
		return nullptr;
	}
	auto info = cur_role->week_credit_mgr_->credit_mgr_.GetCreditInfo(CD_En_WeekCreditIndex::kUkn);
	if (!info)
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(info->impl_))
	{
		assert(false);
		return nullptr;
	}
	return info->impl_;
}

int CD_FuncCall::GetGainedWeekCreditCnt()
{
	auto info = GetWeekCreditInfo();
	if (!info)
	{
		assert(false);
		return -1;
	}
	return info->gained_cnt_;
}

stCD_ReqItem* CD_FuncCall::FindReqItem(pt_word ukn, pt_pcint id_ptr)
{
	//id_ptr这个参数应该是个64位的ID
	int index = 0;
	auto res = g_c_FindReqItem.Call<stCD_ReqItem*>((int)ukn, id_ptr, &index);
	if (IsUnValidPtr(res))
		return nullptr;
	return res;
}

bool CD_FuncCall::HandleMouseMsg(int x, int y, CD_En_MouseOper oper)
{
	auto ptr = GetGloRoleInfoData();
	if (!ptr)
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(ptr->ui_system_))
	{
		assert(false);
		return false;
	}
	return ptr->ui_system_->HandleMouseMsg(x, y, oper);
}

bool CD_FuncCall::PressKey(DWORD virtual_key, bool is_down)
{
	auto mgr = *(stCD_GloDxMgr**)g_a_glo_dx_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_BASE_ADDR
	if (IsUnValidPtr(mgr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(mgr->dx_mgr_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(mgr->dx_mgr_->key_mgr_))
	{
		assert(false);
		return false;
	}
	return mgr->dx_mgr_->key_mgr_->PressKey(virtual_key, is_down);
}

stCD_ShangJinInfoMgr* CD_FuncCall::GetShangJinInfoMgr()
{
	auto addr = (stCD_ShangJinInfoMgr**)CDF_Offset2CryGameAddr(g_a_global_shang_jin_info_mgr_);		//UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_BASE_ADDR
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return false;
	}
	auto mgr = *addr;
	if (IsUnValidPtr(mgr))
	{
		assert(false);
		return false;
	}
	return mgr;
}

int CD_FuncCall::GetShangJinCompleteCnt()
{
	auto mgr = GetShangJinInfoMgr();
	if (!mgr)
	{
		assert(false);
		return -1;
	}
	auto tmp = mgr->GetShangJinInfo();
	if (!tmp)
	{
		assert(false);
		return -1;
	}
	if (tmp->complete_cnt_ < 0 || tmp->complete_cnt_ > 100)
	{
		assert(false);
		return -1;
	}
	return tmp->complete_cnt_;
}

int CD_FuncCall::GetShangJinTotalCnt()
{
	auto mgr = GetShangJinInfoMgr();
	if (!mgr)
	{
		assert(false);
		return -1;
	}
	auto tmp = mgr->GetShangJinInfo();
	if (!tmp)
	{
		assert(false);
		return -1;
	}
	return tmp->GetTotalCnt();
}

int CD_FuncCall::GetShangJinTaskState(int task_id)
{
	int nRetValue = 0;

	DWORD dwShangJinTaskStatusCall = CDF_Offset2CryGameAddr(g_c_shang_jin_task_state_);	//UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_TASK_STATUS_CALL
	if (IsUnValidPtr(dwShangJinTaskStatusCall))
	{
		assert(false);
		return nRetValue;
	}
	__asm
	{
		mov esi, task_id
		mov eax, dwShangJinTaskStatusCall
		call eax

		mov nRetValue, eax
	}
	return nRetValue;
}

bool CD_FuncCall::TidyInventoryItem(CD_En_TabCategory tab_cate)
{
	auto mgr = GetPackagesMgr();
	if (!mgr)
	{
		assert(false);
		return false;
	}
	auto res = mgr->TidyInventoryItemSize(tab_cate);
	--res;
	return mgr->TidyInventoryItem(tab_cate, res);
}

pt_dint CD_FuncCall::GetSynCoorDinatesRandom()
{
	auto mgr = GetGloRoleInfoData();
	if (!mgr)
	{
		assert(false);
		return 0;
	}
	auto addr = mgr->local_id_mgr_;
	if (IsUnValidPtr(addr))
	{
		assert(false);
		return 0;
	}
	auto res = g_c_SynCoorDinatesRandom1.Call<void*>(addr);
	if (IsUnValidPtr(res))
	{
		assert(false);
		return 0;
	}
	return g_c_SynCoorDinatesRandom2.Call<pt_dint>(res);
}

stCD_UiMgr* CD_FuncCall::GetUiMgr()
{
	//g_a_glo_ui_mgr_  g_so_ui_mgr1
	/*
	$-FE     0D37FFEF    /75 2F                    JNZ SHORT CryActio.0D380020
	$-FC     0D37FFF1    |33C9                     XOR ECX, ECX
	$-FA     0D37FFF3    |8D71 01                  LEA ESI, DWORD PTR DS:[ECX+1]
	$-F7     0D37FFF6    |3BC6                     CMP EAX, ESI
	$-F5     0D37FFF8    |76 17                    JBE SHORT CryActio.0D380011
	$-F3     0D37FFFA    |8D53 04                  LEA EDX, DWORD PTR DS:[EBX+4]
	$-F0     0D37FFFD    |8D49 00                  LEA ECX, DWORD PTR DS:[ECX]
	$-ED     0D380000    |833A 00                  CMP DWORD PTR DS:[EDX], 0
	$-EA     0D380003    |75 0A                    JNZ SHORT CryActio.0D38000F
	$-E8     0D380005    |46                       INC ESI
	$-E7     0D380006    |83C2 04                  ADD EDX, 4
	$-E4     0D380009    |3BF0                     CMP ESI, EAX
	$-E2     0D38000B   ^|72 F3                    JB SHORT CryActio.0D380000
	$-E0     0D38000D    |EB 02                    JMP SHORT CryActio.0D380011
	$-DE     0D38000F    |8B0A                     MOV ECX, DWORD PTR DS:[EDX]
	$-DC     0D380011    |85C9                     TEST ECX, ECX
	$-DA     0D380013    |75 0B                    JNZ SHORT CryActio.0D380020
	$-D8     0D380015    |3BF0                     CMP ESI, EAX
	$-D6     0D380017    |73 42                    JNB SHORT CryActio.0D38005B
	$-D4     0D380019    |8B0CB3                   MOV ECX, DWORD PTR DS:[EBX+ESI*4]
	$-D1     0D38001C    |85C9                     TEST ECX, ECX
	$-CF     0D38001E    |74 3B                    JE SHORT CryActio.0D38005B
	$-CD     0D380020    \8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$-CB     0D380022     D945 FC                  FLD DWORD PTR SS:[EBP-4]
	$-C8     0D380025     8B42 04                  MOV EAX, DWORD PTR DS:[EDX+4]
	$-C5     0D380028     51                       PUSH ECX
	$-C4     0D380029     D91C24                   FSTP DWORD PTR SS:[ESP]
	$-C1     0D38002C     FFD0                     CALL EAX
	$-BF     0D38002E     8B1F                     MOV EBX, DWORD PTR DS:[EDI]
	$-BD     0D380030     8B47 04                  MOV EAX, DWORD PTR DS:[EDI+4]
	$-BA     0D380033     2BC3                     SUB EAX, EBX
	$-B8     0D380035     C1F8 02                  SAR EAX, 2
	$-B5     0D380038     8D56 01                  LEA EDX, DWORD PTR DS:[ESI+1]
	$-B2     0D38003B     33C9                     XOR ECX, ECX
	$-B0     0D38003D     3BD0                     CMP EDX, EAX
	$-AE     0D38003F     73 16                    JNB SHORT CryActio.0D380057
	$-AC     0D380041     8D3493                   LEA ESI, DWORD PTR DS:[EBX+EDX*4]
	$-A9     0D380044     833E 00                  CMP DWORD PTR DS:[ESI], 0
	$-A6     0D380047     75 0C                    JNZ SHORT CryActio.0D380055
	$-A4     0D380049     42                       INC EDX
	$-A3     0D38004A     83C6 04                  ADD ESI, 4
	$-A0     0D38004D     3BD0                     CMP EDX, EAX
	$-9E     0D38004F   ^ 72 F3                    JB SHORT CryActio.0D380044
	$-9C     0D380051     8BF2                     MOV ESI, EDX
	$-9A     0D380053   ^ EB BC                    JMP SHORT CryActio.0D380011
	$-98     0D380055     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-96     0D380057     8BF2                     MOV ESI, EDX
	$-94     0D380059   ^ EB B6                    JMP SHORT CryActio.0D380011
	$-92     0D38005B     8347 0C FF               ADD DWORD PTR DS:[EDI+C], -1
	$-8E     0D38005F     75 7E                    JNZ SHORT CryActio.0D3800DF
	$-8C     0D380061     807F 10 00               CMP BYTE PTR DS:[EDI+10], 0
	$-88     0D380065     74 78                    JE SHORT CryActio.0D3800DF
	$-86     0D380067     8B77 04                  MOV ESI, DWORD PTR DS:[EDI+4]
	$-83     0D38006A     8B07                     MOV EAX, DWORD PTR DS:[EDI]
	$-81     0D38006C     8D4D 0F                  LEA ECX, DWORD PTR SS:[EBP+F]
	$-7E     0D38006F     51                       PUSH ECX
	$-7D     0D380070     8D55 E4                  LEA EDX, DWORD PTR SS:[EBP-1C]
	$-7A     0D380073     52                       PUSH EDX
	$-79     0D380074     56                       PUSH ESI
	$-78     0D380075     50                       PUSH EAX
	$-77     0D380076     C745 E4 00000000         MOV DWORD PTR SS:[EBP-1C], 0
	$-70     0D38007D     E8 BEE0FDFF              CALL CryActio.0D35E140
	$-6B     0D380082     83C4 10                  ADD ESP, 10
	$-68     0D380085     3BC6                     CMP EAX, ESI
	$-66     0D380087     74 38                    JE SHORT CryActio.0D3800C1
	$-64     0D380089     8BD0                     MOV EDX, EAX
	$-62     0D38008B     83C0 04                  ADD EAX, 4
	$-5F     0D38008E     3BC6                     CMP EAX, ESI
	$-5D     0D380090     74 12                    JE SHORT CryActio.0D3800A4
	$-5B     0D380092     8B08                     MOV ECX, DWORD PTR DS:[EAX]
	$-59     0D380094     85C9                     TEST ECX, ECX
	$-57     0D380096     74 05                    JE SHORT CryActio.0D38009D
	$-55     0D380098     890A                     MOV DWORD PTR DS:[EDX], ECX
	$-53     0D38009A     83C2 04                  ADD EDX, 4
	$-50     0D38009D     83C0 04                  ADD EAX, 4
	$-4D     0D3800A0     3BC6                     CMP EAX, ESI
	$-4B     0D3800A2   ^ 75 EE                    JNZ SHORT CryActio.0D380092
	$-49     0D3800A4     8BC2                     MOV EAX, EDX
	$-47     0D3800A6     3BC6                     CMP EAX, ESI
	$-45     0D3800A8     74 17                    JE SHORT CryActio.0D3800C1
	$-43     0D3800AA     8B5F 04                  MOV EBX, DWORD PTR DS:[EDI+4]
	$-40     0D3800AD     2BDE                     SUB EBX, ESI
	$-3E     0D3800AF     74 0D                    JE SHORT CryActio.0D3800BE
	$-3C     0D3800B1     53                       PUSH EBX
	$-3B     0D3800B2     56                       PUSH ESI
	$-3A     0D3800B3     50                       PUSH EAX
	$-39     0D3800B4     E8 3D4A7000              CALL <JMP.&MSVCR90.memmove>
	$-34     0D3800B9     83C4 0C                  ADD ESP, 0C
	$-31     0D3800BC     03C3                     ADD EAX, EBX
	$-2F     0D3800BE     8947 04                  MOV DWORD PTR DS:[EDI+4], EAX
	$-2C     0D3800C1     807F 11 00               CMP BYTE PTR DS:[EDI+11], 0
	$-28     0D3800C5     74 10                    JE SHORT CryActio.0D3800D7
	$-26     0D3800C7     8B0F                     MOV ECX, DWORD PTR DS:[EDI]
	$-24     0D3800C9     3B4F 04                  CMP ECX, DWORD PTR DS:[EDI+4]
	$-21     0D3800CC     75 09                    JNZ SHORT CryActio.0D3800D7
	$-1F     0D3800CE     57                       PUSH EDI
	$-1E     0D3800CF     E8 5C9CFFFF              CALL CryActio.0D379D30
	$-19     0D3800D4     83C4 04                  ADD ESP, 4
	$-16     0D3800D7     C647 10 00               MOV BYTE PTR DS:[EDI+10], 0
	$-12     0D3800DB     C647 11 00               MOV BYTE PTR DS:[EDI+11], 0
	$-E      0D3800DF     8345 F8 04               ADD DWORD PTR SS:[EBP-8], 4
	$-A      0D3800E3     836D F4 01               SUB DWORD PTR SS:[EBP-C], 1
	$-6      0D3800E7   ^ 0F85 E6FEFFFF            JNZ CryActio.0D37FFD3
	$ ==>    0D3800ED     A1 3056BA0D              MOV EAX, DWORD PTR DS:[DBA5630]          ; g_a_glo_ui_mgr_
	$+5      0D3800F2     83B8 BC000000 00         CMP DWORD PTR DS:[EAX+BC], 0             ; g_so_ui_mgr1
	$+C      0D3800F9     74 14                    JE SHORT CryActio.0D38010F
	$+E      0D3800FB     8B88 BC000000            MOV ECX, DWORD PTR DS:[EAX+BC]
	$+14     0D380101     D945 F0                  FLD DWORD PTR SS:[EBP-10]
	$+17     0D380104     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+19     0D380106     8B42 18                  MOV EAX, DWORD PTR DS:[EDX+18]
	$+1C     0D380109     51                       PUSH ECX
	$+1D     0D38010A     D91C24                   FSTP DWORD PTR SS:[ESP]
	$+20     0D38010D     FFD0                     CALL EAX
	$+22     0D38010F     8B5D EC                  MOV EBX, DWORD PTR SS:[EBP-14]
	$+25     0D380112     D945 FC                  FLD DWORD PTR SS:[EBP-4]
	$+28     0D380115     8B8B 1C050000            MOV ECX, DWORD PTR DS:[EBX+51C]
	$+2E     0D38011B     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+30     0D38011D     8B42 2C                  MOV EAX, DWORD PTR DS:[EDX+2C]
	$+33     0D380120     51                       PUSH ECX
	$+34     0D380121     D91C24                   FSTP DWORD PTR SS:[ESP]
	$+37     0D380124     FFD0                     CALL EAX
	$+39     0D380126     A1 3056BA0D              MOV EAX, DWORD PTR DS:[DBA5630]
	$+3E     0D38012B     83B8 A8000000 00         CMP DWORD PTR DS:[EAX+A8], 0
	$+45     0D380132     74 14                    JE SHORT CryActio.0D380148
	$+47     0D380134     8B88 A8000000            MOV ECX, DWORD PTR DS:[EAX+A8]
	$+4D     0D38013A     D945 FC                  FLD DWORD PTR SS:[EBP-4]
	$+50     0D38013D     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+52     0D38013F     8B42 08                  MOV EAX, DWORD PTR DS:[EDX+8]
	$+55     0D380142     51                       PUSH ECX
	$+56     0D380143     D91C24                   FSTP DWORD PTR SS:[ESP]
	$+59     0D380146     FFD0                     CALL EAX
	$+5B     0D380148     8B8B 18060000            MOV ECX, DWORD PTR DS:[EBX+618]
	$+61     0D38014E     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+63     0D380150     8B42 08                  MOV EAX, DWORD PTR DS:[EDX+8]
	$+66     0D380153     FFD0                     CALL EAX
	$+68     0D380155     83F8 01                  CMP EAX, 1
	$+6B     0D380158     0F94C1                   SETE CL
	$+6E     0D38015B     0FB6D1                   MOVZX EDX, CL
	$+71     0D38015E     52                       PUSH EDX
	$+72     0D38015F     E8 7CC5AAFF              CALL CryActio.0CE2C6E0
	$+77     0D380164     8BC8                     MOV ECX, EAX
	$+79     0D380166     E8 B5C5AAFF              CALL CryActio.0CE2C720
	$+7E     0D38016B     D945 FC                  FLD DWORD PTR SS:[EBP-4]
	$+81     0D38016E     51                       PUSH ECX
	$+82     0D38016F     D91C24                   FSTP DWORD PTR SS:[ESP]
	$+85     0D380172     E8 69C5AAFF              CALL CryActio.0CE2C6E0
	$+8A     0D380177     8BC8                     MOV ECX, EAX
	$+8C     0D380179     E8 72CDAAFF              CALL CryActio.0CE2CEF0
	$+91     0D38017E     8B8B 14060000            MOV ECX, DWORD PTR DS:[EBX+614]
	$+97     0D380184     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+99     0D380186     8B50 08                  MOV EDX, DWORD PTR DS:[EAX+8]
	$+9C     0D380189     FFD2                     CALL EDX
	$+9E     0D38018B     83F8 01                  CMP EAX, 1
	$+A1     0D38018E     0F94C0                   SETE AL
	$+A4     0D380191     0FB6C8                   MOVZX ECX, AL
	$+A7     0D380194     51                       PUSH ECX
	$+A8     0D380195     E8 D660AAFF              CALL CryActio.0CE26270
	$+AD     0D38019A     8BC8                     MOV ECX, EAX
	$+AF     0D38019C     E8 0F61AAFF              CALL CryActio.0CE262B0
	$+B4     0D3801A1     D945 FC                  FLD DWORD PTR SS:[EBP-4]
	$+B7     0D3801A4     51                       PUSH ECX
	$+B8     0D3801A5     D91C24                   FSTP DWORD PTR SS:[ESP]
	$+BB     0D3801A8     E8 C360AAFF              CALL CryActio.0CE26270
	$+C0     0D3801AD     8BC8                     MOV ECX, EAX
	$+C2     0D3801AF     E8 CC65AAFF              CALL CryActio.0CE26780
	$+C7     0D3801B4     A1 3056BA0D              MOV EAX, DWORD PTR DS:[DBA5630]
	$+CC     0D3801B9     8378 68 00               CMP DWORD PTR DS:[EAX+68], 0
	$+D0     0D3801BD     0F84 0A010000            JE CryActio.0D3802CD
	$+D6     0D3801C3     80B8 78010000 00         CMP BYTE PTR DS:[EAX+178], 0
	$+DD     0D3801CA     0F84 FD000000            JE CryActio.0D3802CD
	$+E3     0D3801D0     8B48 68                  MOV ECX, DWORD PTR DS:[EAX+68]
	$+E6     0D3801D3     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+E8     0D3801D5     8B42 20                  MOV EAX, DWORD PTR DS:[EDX+20]
	$+EB     0D3801D8     FFD0                     CALL EAX
	$+ED     0D3801DA     8945 FC                  MOV DWORD PTR SS:[EBP-4], EAX
	$+F0     0D3801DD     33FF                     XOR EDI, EDI
	$+F2     0D3801DF     90                       NOP
	$+F3     0D3801E0     8B87 6410B70D            MOV EAX, DWORD PTR DS:[EDI+DB71064]
	$+F9     0D3801E6     85C0                     TEST EAX, EAX
	$+FB     0D3801E8     7C 05                    JL SHORT CryActio.0D3801EF
	$+FD     0D3801EA     3B45 FC                  CMP EAX, DWORD PTR SS:[EBP-4]
	$+100    0D3801ED     7C 4B                    JL SHORT CryActio.0D38023A
	$+102    0D3801EF     33F6                     XOR ESI, ESI
	$+104    0D3801F1     3975 FC                  CMP DWORD PTR SS:[EBP-4], ESI
	$+107    0D3801F4     7E 44                    JLE SHORT CryActio.0D38023A
	$+109    0D3801F6     EB 08                    JMP SHORT CryActio.0D380200
	$+10B    0D3801F8     8DA424 00000000          LEA ESP, DWORD PTR SS:[ESP]
	$+112    0D3801FF     90                       NOP
	$+113    0D380200     8B0D 3056BA0D            MOV ECX, DWORD PTR DS:[DBA5630]          ; mhoclien.01826850
	$+119    0D380206     8B49 68                  MOV ECX, DWORD PTR DS:[ECX+68]           ; CryGame.0B056720
	$+11C    0D380209     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+11E    0D38020B     8B42 24                  MOV EAX, DWORD PTR DS:[EDX+24]
	$+121    0D38020E     56                       PUSH ESI
	$+122    0D38020F     FFD0                     CALL EAX
	$+124    0D380211     85C0                     TEST EAX, EAX
	$+126    0D380213     74 17                    JE SHORT CryActio.0D38022C
	$+128    0D380215     8B8F 4C10B70D            MOV ECX, DWORD PTR DS:[EDI+DB7104C]
	$+12E    0D38021B     8B50 04                  MOV EDX, DWORD PTR DS:[EAX+4]
	$+131    0D38021E     51                       PUSH ECX
	$+132    0D38021F     52                       PUSH EDX
	$+133    0D380220     E8 6B497000              CALL <JMP.&MSVCR90._stricmp>
	$+138    0D380225     83C4 08                  ADD ESP, 8
	$+13B    0D380228     85C0                     TEST EAX, EAX
	$+13D    0D38022A     74 08                    JE SHORT CryActio.0D380234
	$+13F    0D38022C     46                       INC ESI
	$+140    0D38022D     3B75 FC                  CMP ESI, DWORD PTR SS:[EBP-4]
	$+143    0D380230   ^ 7C CE                    JL SHORT CryActio.0D380200
	$+145    0D380232     EB 06                    JMP SHORT CryActio.0D38023A
	$+147    0D380234     89B7 6410B70D            MOV DWORD PTR DS:[EDI+DB71064], ESI
	$+14D    0D38023A     8B87 6410B70D            MOV EAX, DWORD PTR DS:[EDI+DB71064]
	$+153    0D380240     85C0                     TEST EAX, EAX
	$+155    0D380242     7C 48                    JL SHORT CryActio.0D38028C
	$+157    0D380244     3B45 FC                  CMP EAX, DWORD PTR SS:[EBP-4]
	$+15A    0D380247     7D 43                    JGE SHORT CryActio.0D38028C
	$+15C    0D380249     8B0D 3056BA0D            MOV ECX, DWORD PTR DS:[DBA5630]          ; mhoclien.01826850
	$+162    0D38024F     8B49 68                  MOV ECX, DWORD PTR DS:[ECX+68]           ; CryGame.0B056720
	$+165    0D380252     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+167    0D380254     50                       PUSH EAX
	$+168    0D380255     8B42 24                  MOV EAX, DWORD PTR DS:[EDX+24]
	$+16B    0D380258     FFD0                     CALL EAX
	$+16D    0D38025A     8BF0                     MOV ESI, EAX
	$+16F    0D38025C     85F6                     TEST ESI, ESI
	$+171    0D38025E     74 2C                    JE SHORT CryActio.0D38028C
	$+173    0D380260     8B8F 4C10B70D            MOV ECX, DWORD PTR DS:[EDI+DB7104C]
	$+179    0D380266     8B56 04                  MOV EDX, DWORD PTR DS:[ESI+4]
	$+17C    0D380269     51                       PUSH ECX
	$+17D    0D38026A     52                       PUSH EDX
	$+17E    0D38026B     E8 20497000              CALL <JMP.&MSVCR90._stricmp>
	$+183    0D380270     83C4 08                  ADD ESP, 8
	$+186    0D380273     85C0                     TEST EAX, EAX
	$+188    0D380275     75 15                    JNZ SHORT CryActio.0D38028C
	$+18A    0D380277     F3:0F1046 30             MOVSS XMM0, DWORD PTR DS:[ESI+30]
	$+18F    0D38027C     F3:0F5887 0495BA0D       ADDSS XMM0, DWORD PTR DS:[EDI+DBA9504]
	$+197    0D380284     F3:0F1187 0495BA0D       MOVSS DWORD PTR DS:[EDI+DBA9504], XMM0
	$+19F    0D38028C     83C7 04                  ADD EDI, 4
	$+1A2    0D38028F     83FF 18                  CMP EDI, 18
	$+1A5    0D380292   ^ 0F82 48FFFFFF            JB CryActio.0D3801E0
	$+1AB    0D380298     A1 0095BA0D              MOV EAX, DWORD PTR DS:[DBA9500]
	$+1B0    0D38029D     40                       INC EAX
	$+1B1    0D38029E     83F8 3C                  CMP EAX, 3C
	$+1B4    0D3802A1     A3 0095BA0D              MOV DWORD PTR DS:[DBA9500], EAX
	$+1B9    0D3802A6     7C 25                    JL SHORT CryActio.0D3802CD
	$+1BB    0D3802A8     33C0                     XOR EAX, EAX
	$+1BD    0D3802AA     A3 0495BA0D              MOV DWORD PTR DS:[DBA9504], EAX
	$+1C2    0D3802AF     A3 0895BA0D              MOV DWORD PTR DS:[DBA9508], EAX
	$+1C7    0D3802B4     A3 0C95BA0D              MOV DWORD PTR DS:[DBA950C], EAX
	$+1CC    0D3802B9     A3 1095BA0D              MOV DWORD PTR DS:[DBA9510], EAX
	$+1D1    0D3802BE     A3 1495BA0D              MOV DWORD PTR DS:[DBA9514], EAX
	$+1D6    0D3802C3     A3 1895BA0D              MOV DWORD PTR DS:[DBA9518], EAX
	$+1DB    0D3802C8     A3 0095BA0D              MOV DWORD PTR DS:[DBA9500], EAX
	$+1E0    0D3802CD     8B4B 4C                  MOV ECX, DWORD PTR DS:[EBX+4C]
	$+1E3    0D3802D0     8B01                     MOV EAX, DWORD PTR DS:[ECX]
	$+1E5    0D3802D2     8B50 34                  MOV EDX, DWORD PTR DS:[EAX+34]
	$+1E8    0D3802D5     6A 01                    PUSH 1
	$+1EA    0D3802D7     FFD2                     CALL EDX
	$+1EC    0D3802D9     8B4B 6C                  MOV ECX, DWORD PTR DS:[EBX+6C]
	$+1EF    0D3802DC     85C9                     TEST ECX, ECX
	$+1F1    0D3802DE     74 30                    JE SHORT CryActio.0D380310
	$+1F3    0D3802E0     E8 0B54FCFF              CALL CryActio.0D3456F0
	$+1F8    0D3802E5     84C0                     TEST AL, AL
	$+1FA    0D3802E7     74 27                    JE SHORT CryActio.0D380310
	$+1FC    0D3802E9     A1 4895BA0D              MOV EAX, DWORD PTR DS:[?s_pThis@CCryActi>
	$+201    0D3802EE     8378 64 00               CMP DWORD PTR DS:[EAX+64], 0
	$+205    0D3802F2     74 1C                    JE SHORT CryActio.0D380310
	$+207    0D3802F4     8B0D 3056BA0D            MOV ECX, DWORD PTR DS:[DBA5630]          ; mhoclien.01826850
	$+20D    0D3802FA     8B49 2C                  MOV ECX, DWORD PTR DS:[ECX+2C]
	$+210    0D3802FD     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+212    0D3802FF     8B82 84000000            MOV EAX, DWORD PTR DS:[EDX+84]
	$+218    0D380305     6A 00                    PUSH 0
	$+21A    0D380307     6A 00                    PUSH 0
	$+21C    0D380309     68 B0F8AE0D              PUSH CryActio.0DAEF8B0                   ; ASCII "disconnect"
	$+221    0D38030E     FFD0                     CALL EAX
	$+223    0D380310     8B13                     MOV EDX, DWORD PTR DS:[EBX]              ; CryActio.0DAF1A30
	$+225    0D380312     8B82 8C020000            MOV EAX, DWORD PTR DS:[EDX+28C]
	$+22B    0D380318     8BCB                     MOV ECX, EBX
	$+22D    0D38031A     FFD0                     CALL EAX
	$+22F    0D38031C     85C0                     TEST EAX, EAX
	$+231    0D38031E     74 07                    JE SHORT CryActio.0D380327
	$+233    0D380320     8BC8                     MOV ECX, EAX
	$+235    0D380322     E8 292CE0FF              CALL CryActio.0D182F50
	$+23A    0D380327     83BB 88050000 00         CMP DWORD PTR DS:[EBX+588], 0
	$+241    0D38032E     74 1A                    JE SHORT CryActio.0D38034A
	$+243    0D380330     8B13                     MOV EDX, DWORD PTR DS:[EBX]              ; CryActio.0DAF1A30
	$+245    0D380332     8B42 44                  MOV EAX, DWORD PTR DS:[EDX+44]
	$+248    0D380335     8BCB                     MOV ECX, EBX
	$+24A    0D380337     FFD0                     CALL EAX
	*/
	//g_so_ui_mgr2  g_so_ui_mgr2_ukn  g_so_ui_mgr3
	/*
	$-B6     0D22FCC5     F3:0F114D 08             MOVSS DWORD PTR SS:[EBP+8], XMM1
	$-B1     0D22FCCA     01BB 14010000            ADD DWORD PTR DS:[EBX+114], EDI
	$-AB     0D22FCD0     8BBB 08010000            MOV EDI, DWORD PTR DS:[EBX+108]
	$-A5     0D22FCD6     8B83 0C010000            MOV EAX, DWORD PTR DS:[EBX+10C]
	$-9F     0D22FCDC     81C3 08010000            ADD EBX, 108
	$-99     0D22FCE2     2BC7                     SUB EAX, EDI
	$-97     0D22FCE4     C1F8 02                  SAR EAX, 2
	$-94     0D22FCE7     85C0                     TEST EAX, EAX
	$-92     0D22FCE9     76 06                    JBE SHORT CryActio.0D22FCF1
	$-90     0D22FCEB     8B0F                     MOV ECX, DWORD PTR DS:[EDI]
	$-8E     0D22FCED     85C9                     TEST ECX, ECX
	$-8C     0D22FCEF     75 2F                    JNZ SHORT CryActio.0D22FD20
	$-8A     0D22FCF1     33C9                     XOR ECX, ECX
	$-88     0D22FCF3     8D71 01                  LEA ESI, DWORD PTR DS:[ECX+1]
	$-85     0D22FCF6     3BC6                     CMP EAX, ESI
	$-83     0D22FCF8     76 17                    JBE SHORT CryActio.0D22FD11
	$-81     0D22FCFA     8D57 04                  LEA EDX, DWORD PTR DS:[EDI+4]
	$-7E     0D22FCFD     8D49 00                  LEA ECX, DWORD PTR DS:[ECX]
	$-7B     0D22FD00     833A 00                  CMP DWORD PTR DS:[EDX], 0
	$-78     0D22FD03     75 0A                    JNZ SHORT CryActio.0D22FD0F
	$-76     0D22FD05     46                       INC ESI
	$-75     0D22FD06     83C2 04                  ADD EDX, 4
	$-72     0D22FD09     3BF0                     CMP ESI, EAX
	$-70     0D22FD0B   ^ 72 F3                    JB SHORT CryActio.0D22FD00
	$-6E     0D22FD0D     EB 02                    JMP SHORT CryActio.0D22FD11
	$-6C     0D22FD0F     8B0A                     MOV ECX, DWORD PTR DS:[EDX]
	$-6A     0D22FD11     85C9                     TEST ECX, ECX
	$-68     0D22FD13     75 0B                    JNZ SHORT CryActio.0D22FD20
	$-66     0D22FD15     3BF0                     CMP ESI, EAX
	$-64     0D22FD17     73 42                    JNB SHORT CryActio.0D22FD5B
	$-62     0D22FD19     8B0CB7                   MOV ECX, DWORD PTR DS:[EDI+ESI*4]
	$-5F     0D22FD1C     85C9                     TEST ECX, ECX
	$-5D     0D22FD1E     74 3B                    JE SHORT CryActio.0D22FD5B
	$-5B     0D22FD20     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$-59     0D22FD22     D945 08                  FLD DWORD PTR SS:[EBP+8]
	$-56     0D22FD25     8B42 14                  MOV EAX, DWORD PTR DS:[EDX+14]
	$-53     0D22FD28     51                       PUSH ECX
	$-52     0D22FD29     D91C24                   FSTP DWORD PTR SS:[ESP]
	$-4F     0D22FD2C     FFD0                     CALL EAX
	$-4D     0D22FD2E     8B3B                     MOV EDI, DWORD PTR DS:[EBX]              ; CryActio.0DAF1A30
	$-4B     0D22FD30     8B43 04                  MOV EAX, DWORD PTR DS:[EBX+4]            ; CryActio.0DAF1A28
	$-48     0D22FD33     2BC7                     SUB EAX, EDI
	$-46     0D22FD35     C1F8 02                  SAR EAX, 2
	$-43     0D22FD38     8D56 01                  LEA EDX, DWORD PTR DS:[ESI+1]
	$-40     0D22FD3B     33C9                     XOR ECX, ECX
	$-3E     0D22FD3D     3BD0                     CMP EDX, EAX
	$-3C     0D22FD3F     73 16                    JNB SHORT CryActio.0D22FD57
	$-3A     0D22FD41     8D3497                   LEA ESI, DWORD PTR DS:[EDI+EDX*4]
	$-37     0D22FD44     833E 00                  CMP DWORD PTR DS:[ESI], 0
	$-34     0D22FD47     75 0C                    JNZ SHORT CryActio.0D22FD55
	$-32     0D22FD49     42                       INC EDX
	$-31     0D22FD4A     83C6 04                  ADD ESI, 4
	$-2E     0D22FD4D     3BD0                     CMP EDX, EAX
	$-2C     0D22FD4F   ^ 72 F3                    JB SHORT CryActio.0D22FD44
	$-2A     0D22FD51     8BF2                     MOV ESI, EDX
	$-28     0D22FD53   ^ EB BC                    JMP SHORT CryActio.0D22FD11
	$-26     0D22FD55     8B0E                     MOV ECX, DWORD PTR DS:[ESI]
	$-24     0D22FD57     8BF2                     MOV ESI, EDX
	$-22     0D22FD59   ^ EB B6                    JMP SHORT CryActio.0D22FD11
	$-20     0D22FD5B     8343 0C FF               ADD DWORD PTR DS:[EBX+C], -1
	$-1C     0D22FD5F     75 07                    JNZ SHORT CryActio.0D22FD68
	$-1A     0D22FD61     8BCB                     MOV ECX, EBX
	$-18     0D22FD63     E8 88D9FFFF              CALL CryActio.0D22D6F0
	$-13     0D22FD68     8B5D F8                  MOV EBX, DWORD PTR SS:[EBP-8]
	$-10     0D22FD6B     80BB D0000000 00         CMP BYTE PTR DS:[EBX+D0], 0
	$-9      0D22FD72     74 07                    JE SHORT CryActio.0D22FD7B
	$-7      0D22FD74     8BCB                     MOV ECX, EBX
	$-5      0D22FD76     E8 C5BEFFFF              CALL CryActio.0D22BC40
	$ ==>    0D22FD7B     8DBB B8000000            LEA EDI, DWORD PTR DS:[EBX+B8]           ; g_so_ui_mgr2
	$+6      0D22FD81     C683 69010000 01         MOV BYTE PTR DS:[EBX+169], 1
	$+D      0D22FD88     8B77 08                  MOV ESI, DWORD PTR DS:[EDI+8]            ; g_so_ui_mgr2_ukn
	$+10     0D22FD8B     3BF7                     CMP ESI, EDI
	$+12     0D22FD8D     74 76                    JE SHORT CryActio.0D22FE05
	$+14     0D22FD8F     90                       NOP
	$+15     0D22FD90     8B4E 14                  MOV ECX, DWORD PTR DS:[ESI+14]			;g_so_ui_mgr3
	$+18     0D22FD93     D945 08                  FLD DWORD PTR SS:[EBP+8]
	$+1B     0D22FD96     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+1D     0D22FD98     8B42 54                  MOV EAX, DWORD PTR DS:[EDX+54]
	$+20     0D22FD9B     51                       PUSH ECX
	$+21     0D22FD9C     D91C24                   FSTP DWORD PTR SS:[ESP]
	$+24     0D22FD9F     FFD0                     CALL EAX									;函数内部便有下级偏移
	$+26     0D22FDA1     B8 01000000              MOV EAX, 1
	$+2B     0D22FDA6     3905 4C3DBA0D            CMP DWORD PTR DS:[DBA3D4C], EAX
	$+31     0D22FDAC     75 1A                    JNZ SHORT CryActio.0D22FDC8
	$+33     0D22FDAE     3905 483DBA0D            CMP DWORD PTR DS:[DBA3D48], EAX
	$+39     0D22FDB4     75 12                    JNZ SHORT CryActio.0D22FDC8
	$+3B     0D22FDB6     3905 443DBA0D            CMP DWORD PTR DS:[DBA3D44], EAX
	$+41     0D22FDBC     75 0A                    JNZ SHORT CryActio.0D22FDC8
	$+43     0D22FDBE     8B4E 14                  MOV ECX, DWORD PTR DS:[ESI+14]
	$+46     0D22FDC1     8B11                     MOV EDX, DWORD PTR DS:[ECX]
	$+48     0D22FDC3     8B42 58                  MOV EAX, DWORD PTR DS:[EDX+58]
	$+4B     0D22FDC6     FFD0                     CALL EAX
	$+4D     0D22FDC8     8B46 0C                  MOV EAX, DWORD PTR DS:[ESI+C]
	$+50     0D22FDCB     85C0                     TEST EAX, EAX
	$+52     0D22FDCD     74 14                    JE SHORT CryActio.0D22FDE3
	$+54     0D22FDCF     8BF0                     MOV ESI, EAX
	$+56     0D22FDD1     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+8]
	$+59     0D22FDD4     85C0                     TEST EAX, EAX
	$+5B     0D22FDD6     74 29                    JE SHORT CryActio.0D22FE01
	$+5D     0D22FDD8     8BF0                     MOV ESI, EAX
	$+5F     0D22FDDA     8B46 08                  MOV EAX, DWORD PTR DS:[ESI+8]
	$+62     0D22FDDD     85C0                     TEST EAX, EAX
	$+64     0D22FDDF   ^ 75 F7                    JNZ SHORT CryActio.0D22FDD8
	$+66     0D22FDE1     EB 1E                    JMP SHORT CryActio.0D22FE01
	$+68     0D22FDE3     8B46 04                  MOV EAX, DWORD PTR DS:[ESI+4]
	$+6B     0D22FDE6     3B70 0C                  CMP ESI, DWORD PTR DS:[EAX+C]
	$+6E     0D22FDE9     75 0F                    JNZ SHORT CryActio.0D22FDFA
	$+70     0D22FDEB     EB 03                    JMP SHORT CryActio.0D22FDF0
	$+72     0D22FDED     8D49 00                  LEA ECX, DWORD PTR DS:[ECX]
	$+75     0D22FDF0     8BF0                     MOV ESI, EAX
	$+77     0D22FDF2     8B40 04                  MOV EAX, DWORD PTR DS:[EAX+4]
	$+7A     0D22FDF5     3B70 0C                  CMP ESI, DWORD PTR DS:[EAX+C]
	$+7D     0D22FDF8   ^ 74 F6                    JE SHORT CryActio.0D22FDF0
	$+7F     0D22FDFA     3946 0C                  CMP DWORD PTR DS:[ESI+C], EAX
	$+82     0D22FDFD     74 02                    JE SHORT CryActio.0D22FE01
	$+84     0D22FDFF     8BF0                     MOV ESI, EAX
	$+86     0D22FE01     3BF7                     CMP ESI, EDI
	$+88     0D22FE03   ^ 75 8B                    JNZ SHORT CryActio.0D22FD90
	$+8A     0D22FE05     C683 69010000 00         MOV BYTE PTR DS:[EBX+169], 0
	$+91     0D22FE0C     5E                       POP ESI
	$+92     0D22FE0D     5F                       POP EDI
	$+93     0D22FE0E     5B                       POP EBX
	$+94     0D22FE0F     8BE5                     MOV ESP, EBP
	$+96     0D22FE11     5D                       POP EBP
	$+97     0D22FE12     C2 0400                  RETN 4
	*/
	//g_so_ui_mgr4
	/*
	$-6      0D23C0A0     55                       PUSH EBP
	$-5      0D23C0A1     8BEC                     MOV EBP, ESP
	$-3      0D23C0A3     56                       PUSH ESI
	$-2      0D23C0A4     8BF1                     MOV ESI, ECX
	$ ==>    0D23C0A6     837E 64 00               CMP DWORD PTR DS:[ESI+64], 0             ; g_so_ui_mgr4
	$+4      0D23C0AA     74 66                    JE SHORT CryActio.0D23C112
	$+6      0D23C0AC     8B4E 64                  MOV ECX, DWORD PTR DS:[ESI+64]
	$+9      0D23C0AF     8B01                     MOV EAX, DWORD PTR DS:[ECX]              ; CryActio.0DAF2EE8
	$+B      0D23C0B1     8B90 90000000            MOV EDX, DWORD PTR DS:[EAX+90]
	$+11     0D23C0B7     FFD2                     CALL EDX                                 ; 函数内部有下级偏移
	$+13     0D23C0B9     84C0                     TEST AL, AL
	$+15     0D23C0BB     74 55                    JE SHORT CryActio.0D23C112
	$+17     0D23C0BD     B8 01000000              MOV EAX, 1
	$+1C     0D23C0C2     8405 303EBA0D            TEST BYTE PTR DS:[DBA3E30], AL
	$+22     0D23C0C8     75 21                    JNZ SHORT CryActio.0D23C0EB
	$+24     0D23C0CA     0905 303EBA0D            OR DWORD PTR DS:[DBA3E30], EAX           ; CryActio.0D23C0A0
	$+2A     0D23C0D0     A1 3056BA0D              MOV EAX, DWORD PTR DS:[DBA5630]
	$+2F     0D23C0D5     8B48 2C                  MOV ECX, DWORD PTR DS:[EAX+2C]           ; CryActio.0DBA3E30
	$+32     0D23C0D8     8B11                     MOV EDX, DWORD PTR DS:[ECX]              ; CryActio.0DAF2EE8
	$+34     0D23C0DA     8B42 54                  MOV EAX, DWORD PTR DS:[EDX+54]           ; CryActio.0D23C0A0
	$+37     0D23C0DD     68 101CAF0D              PUSH CryActio.0DAF1C10                   ; ASCII "gfx_draw_only"
	$+3C     0D23C0E2     FFD0                     CALL EAX                                 ; CryActio.0D23C0A0
	$+3E     0D23C0E4     A3 2C3EBA0D              MOV DWORD PTR DS:[DBA3E2C], EAX          ; CryActio.0D23C0A0
	$+43     0D23C0E9     EB 05                    JMP SHORT CryActio.0D23C0F0
	$+45     0D23C0EB     A1 2C3EBA0D              MOV EAX, DWORD PTR DS:[DBA3E2C]
	$+4A     0D23C0F0     85C0                     TEST EAX, EAX                            ; CryActio.0D23C0A0
	$+4C     0D23C0F2     74 0D                    JE SHORT CryActio.0D23C101
	$+4E     0D23C0F4     8B10                     MOV EDX, DWORD PTR DS:[EAX]
	$+50     0D23C0F6     8BC8                     MOV ECX, EAX                             ; CryActio.0D23C0A0
	$+52     0D23C0F8     8B42 08                  MOV EAX, DWORD PTR DS:[EDX+8]            ; CryActio.0D23C030
	$+55     0D23C0FB     FFD0                     CALL EAX                                 ; CryActio.0D23C0A0
	$+57     0D23C0FD     85C0                     TEST EAX, EAX                            ; CryActio.0D23C0A0
	$+59     0D23C0FF     74 11                    JE SHORT CryActio.0D23C112
	$+5B     0D23C101     8B4E 64                  MOV ECX, DWORD PTR DS:[ESI+64]
	$+5E     0D23C104     D945 08                  FLD DWORD PTR SS:[EBP+8]
	$+61     0D23C107     8B11                     MOV EDX, DWORD PTR DS:[ECX]              ; CryActio.0DAF2EE8
	$+63     0D23C109     8B42 38                  MOV EAX, DWORD PTR DS:[EDX+38]           ; CryActio.0D2444B0
	$+66     0D23C10C     51                       PUSH ECX
	$+67     0D23C10D     D91C24                   FSTP DWORD PTR SS:[ESP]
	$+6A     0D23C110     FFD0                     CALL EAX                                 ; CryActio.0D23C0A0
	$+6C     0D23C112     5E                       POP ESI                                  ; 4A51F6D8
	$+6D     0D23C113     5D                       POP EBP                                  ; 4A51F6D8
	$+6E     0D23C114     C2 0400                  RETN 4
	*/
	//g_so_ui_mgr5
	/*
	$-F      00E2F0C0     53                       PUSH EBX
	$-E      00E2F0C1     56                       PUSH ESI
	$-D      00E2F0C2     8BF1                     MOV ESI, ECX
	$-B      00E2F0C4     57                       PUSH EDI
	$-A      00E2F0C5     8B7E 78                  MOV EDI, DWORD PTR DS:[ESI+78]
	$-7      00E2F0C8     8BCF                     MOV ECX, EDI
	$-5      00E2F0CA     E8 A1255DFF              CALL mhoclien.00401670
	$ ==>    00E2F0CF     8B4E 5C                  MOV ECX, DWORD PTR DS:[ESI+5C]           ; g_so_ui_mgr5
	$+3      00E2F0D2     32DB                     XOR BL, BL
	$+5      00E2F0D4     85C9                     TEST ECX, ECX
	$+7      00E2F0D6     74 09                    JE SHORT mhoclien.00E2F0E1
	$+9      00E2F0D8     8B01                     MOV EAX, DWORD PTR DS:[ECX]              ; mhoclien.0147D308
	$+B      00E2F0DA     8B50 28                  MOV EDX, DWORD PTR DS:[EAX+28]           ; mhoclien.00E2E610
	$+E      00E2F0DD     FFD2                     CALL EDX                                 ; mhoclien.00E2F0C0
	$+10     00E2F0DF     8AD8                     MOV BL, AL
	$+12     00E2F0E1     8BCF                     MOV ECX, EDI
	$+14     00E2F0E3     E8 98255DFF              CALL mhoclien.00401680
	$+19     00E2F0E8     5F                       POP EDI                                  ; CryActio.0D23C0B9
	$+1A     00E2F0E9     5E                       POP ESI                                  ; CryActio.0D23C0B9
	$+1B     00E2F0EA     8AC3                     MOV AL, BL
	$+1D     00E2F0EC     5B                       POP EBX                                  ; CryActio.0D23C0B9
	$+1E     00E2F0ED     C3                       RETN
	$+1F     00E2F0EE     CC                       INT3
	$+20     00E2F0EF     CC                       INT3
	$+21     00E2F0F0     55                       PUSH EBP
	$+22     00E2F0F1     8BEC                     MOV EBP, ESP
	$+24     00E2F0F3     83EC 18                  SUB ESP, 18
	$+27     00E2F0F6     53                       PUSH EBX
	$+28     00E2F0F7     56                       PUSH ESI
	$+29     00E2F0F8     8BF1                     MOV ESI, ECX
	$+2B     00E2F0FA     57                       PUSH EDI
	$+2C     00E2F0FB     8B7E 78                  MOV EDI, DWORD PTR DS:[ESI+78]
	$+2F     00E2F0FE     8BCF                     MOV ECX, EDI
	$+31     00E2F100     E8 6B255DFF              CALL mhoclien.00401670
	$+36     00E2F105     8B76 5C                  MOV ESI, DWORD PTR DS:[ESI+5C]           ; g_so_ui_mgr5
	$+39     00E2F108     32DB                     XOR BL, BL
	$+3B     00E2F10A     85F6                     TEST ESI, ESI
	$+3D     00E2F10C     74 38                    JE SHORT mhoclien.00E2F146
	$+3F     00E2F10E     8B4D 0C                  MOV ECX, DWORD PTR SS:[EBP+C]
	$+42     00E2F111     6A 01                    PUSH 1
	$+44     00E2F113     8D45 E8                  LEA EAX, DWORD PTR SS:[EBP-18]
	$+47     00E2F116     E8 C5E1FFFF              CALL mhoclien.00E2D2E0
	$+4C     00E2F11B     50                       PUSH EAX                                 ; mhoclien.0147D308
	$+4D     00E2F11C     8B45 08                  MOV EAX, DWORD PTR SS:[EBP+8]
	$+50     00E2F11F     50                       PUSH EAX                                 ; mhoclien.0147D308
	$+51     00E2F120     8BCE                     MOV ECX, ESI
	$+53     00E2F122     E8 59C41800              CALL mhoclien.00FBB580
	$+58     00E2F127     8B4D EC                  MOV ECX, DWORD PTR SS:[EBP-14]
	$+5B     00E2F12A     C1E9 06                  SHR ECX, 6
	$+5E     00E2F12D     8AD8                     MOV BL, AL
	$+60     00E2F12F     F6C1 01                  TEST CL, 1
	$+63     00E2F132     74 12                    JE SHORT mhoclien.00E2F146
	$+65     00E2F134     8B45 F0                  MOV EAX, DWORD PTR SS:[EBP-10]
	$+68     00E2F137     8B4D E8                  MOV ECX, DWORD PTR SS:[EBP-18]
	$+6B     00E2F13A     8B11                     MOV EDX, DWORD PTR DS:[ECX]              ; mhoclien.0147D308
	$+6D     00E2F13C     8B52 08                  MOV EDX, DWORD PTR DS:[EDX+8]
	$+70     00E2F13F     50                       PUSH EAX                                 ; mhoclien.0147D308
	$+71     00E2F140     8D45 E8                  LEA EAX, DWORD PTR SS:[EBP-18]
	$+74     00E2F143     50                       PUSH EAX                                 ; mhoclien.0147D308
	$+75     00E2F144     FFD2                     CALL EDX                                 ; mhoclien.00E2F0C0
	$+77     00E2F146     8BCF                     MOV ECX, EDI
	$+79     00E2F148     E8 33255DFF              CALL mhoclien.00401680
	$+7E     00E2F14D     5F                       POP EDI                                  ; CryActio.0D23C0B9
	$+7F     00E2F14E     5E                       POP ESI                                  ; CryActio.0D23C0B9
	$+80     00E2F14F     8AC3                     MOV AL, BL
	$+82     00E2F151     5B                       POP EBX                                  ; CryActio.0D23C0B9
	$+83     00E2F152     8BE5                     MOV ESP, EBP
	$+85     00E2F154     5D                       POP EBP                                  ; CryActio.0D23C0B9
	$+86     00E2F155     C2 0800                  RETN 8
	*/
	auto tmp = (stCD_GloUiMgr**)CDF_Offset2CryActionAddr(g_a_glo_ui_mgr_);	//UPDATES_REVERSAL_UI_BASE_ADDR
	if (IsUnValidPtr(tmp))
	{
		assert(false);
		return nullptr;
	}
	auto tmp1 = *tmp;
	if (IsUnValidPtr(tmp1))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(tmp1->ui_mgr_))
	{
		assert(false);
		return nullptr;
	}
	auto tmp2 = tmp1->ui_mgr_->ukn_.ui_mgr_;
	if (IsUnValidPtr(tmp2))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(tmp2->ui_mgr_))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(tmp2->ui_mgr_->ui_mgr_))
	{
		assert(false);
		return nullptr;
	}
	auto tmp3 = tmp2->ui_mgr_->ui_mgr_;
	if (IsUnValidPtr(tmp3))
	{
		assert(false);
		return nullptr;
	}
	if (IsUnValidPtr(tmp3->ui_mgr_))
	{
		assert(false);
		return nullptr;
	}
	return tmp3->ui_mgr_;
}

bool CD_FuncCall::UiCtrl_LeftBtClick(const pt_csz cls_name[], int cnt)
{
	/*stCD_UiCtrlMsg msg[4] = { 0 };
	msg[0].type_ = 0x10;
	msg[1].type_ = 0x1000;
	msg[2].type_ = 0x20;
	msg[3].type_ = 0x0100000C;*/
	static const CD_En_UiCtrlMsg the_msgs[] = {
		CD_En_UiCtrlMsg::kLeftBtDown, CD_En_UiCtrlMsg::kLeftBtUp
	};	
	int done_cnt = 0, idx = 0;
	for (auto v : the_msgs)
	{
		++idx;
		stCD_UiCtrlMsg msg;
		msg.type_ = v;
		auto res = GWndExecSync([&done_cnt, cls_name, cnt, &msg](){
			DO_EXCEPTION_TRY;
			auto res = FindUiCtrl(cls_name, cnt);
			if (res)
			{
				done_cnt += !!res->HandleMsg(msg);
				return true;
			}
			DO_CATCH_ALL_MSG_BOX_CODE(
				return false;);
			return false;
		});
		if (res)
		{
			if (idx < _countof(the_msgs))
				::DummySleep(500);
		}
		else
		{
			if (1 == idx)
				break;
		}
	}
	return done_cnt > 0;
}

stCD_UiCtrl* CD_FuncCall::FindUiCtrl(const pt_csz cls_name[], int cnt)
{
	stCD_UiCtrl* res = nullptr;
	CD_Traverse::TraverseUiCtrls([&cls_name, cnt, &res](stCD_UiCtrl* ui_ctrl){
		if (ui_ctrl->MatchTheFullClassName(cls_name, cnt))
		{
			res = ui_ctrl;
			return false;
		}
		return true;
	});
	return res;
}

bool CD_FuncCall::UiCtrl_LeftBtClickByFullClsName(pt_csz full_cls_name)
{
	static std::string sep(".");
	std::vector<std::string> res;
	SeparateString(full_cls_name, sep, res);
	std::vector<pt_csz> the_cls_name;
	auto it = res.crbegin();
	auto it_end = res.crend();
	for (; it != it_end; ++it)
	{
		the_cls_name.push_back(it->c_str());
	}
	return UiCtrl_LeftBtClick(the_cls_name.data(), the_cls_name.size());
}

stCD_UiCtrl* CD_FuncCall::FindUiCtrlByFullClsName(pt_csz full_cls_name)
{
	static std::string sep(".");
	std::vector<std::string> res;
	SeparateString(full_cls_name, sep, res);
	std::vector<pt_csz> the_cls_name;
	auto it = res.crbegin();
	auto it_end = res.crend();
	for (; it != it_end; ++it)
	{
		the_cls_name.push_back(it->c_str());
	}
	return FindUiCtrl(the_cls_name.data(), the_cls_name.size());
}

bool CD_FuncCall::UiCtrlEdit_PutString(stCD_UiCtrl* ui_ctrl, pt_cwsz str)
{
	if (!ui_ctrl)
	{
		assert(false);
		return false;
	}
	auto ptr = ui_ctrl->CastTo<stCD_UiCtrlEdit>();
	if (IsUnValidPtr(ptr->ukn_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(ptr->ukn_->str_mgr_))
	{
		assert(false);
		return false;
	}
	if (!ptr->ukn_->str_mgr_->ClearString())
		return false;
	return ptr->PutString(str);
}

stCD_UiGlobalMng* CD_FuncCall::GetUIGlobalMng()
{
	//UPDATES_MMOCLIENT_REVERSAL_TRAVERSAL_ALL_OBJ_ADDR
	return (stCD_UiGlobalMng*)CDF_Offset2CryGameAddr(g_a_ui_global_mng_);
}

stCD_UiMng_Minimap* CD_FuncCall::FindUiMng_Minimap()
{
	stCD_UiMng* res = nullptr;
	CD_Traverse::TraverseUiMng([&res](stCD_UiMng* ui_mng){
		if (IsUnValidPtr(ui_mng->name_))
			return true;
		if (strcmp(ui_mng->name_, "Minimap") == 0)
		{
			res = ui_mng;
			return false;
		}
		return true;
	});
	if (res)
		return res->CastTo<stCD_UiMng_Minimap>();
	return nullptr;
}

stCD_WeekCreditInfo* stCD_WeekCreditInfo::GetCreditInfo(CD_En_WeekCreditIndex idx) const
{
	auto res = g_c_GetWeekCreditInfo.Call<stCD_WeekCreditInfo*>(this, &idx);
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	if (res == this)
		return nullptr;
	return res;
}

int stCD_WeekCreditInfoImpl::GetWeekCreditCnt() const
{
	return g_c_GetWeekCreditCnt.Call<int>(this);
}

bool stCD_Msg_WeekCredit::GainCnt(int cnt)
{
	auto left_cnt = CD_FuncCall::GetWeekCreditCnt();

	if (0 >= left_cnt || left_cnt < cnt)
	{
		return false;
	}

	dwWeekCreditIndex = CD_En_WeekCreditIndex::kUkn;
	nWeekCreditCount1 = cnt;
	nHasReceiveWeekCreditCount = CD_FuncCall::GetGainedWeekCreditCnt();
	if (nHasReceiveWeekCreditCount == -1)
	{
		assert(false);
		return false;
	}
	nWeekCreditCount2 = cnt;
	return true;
}

bool stCD_UiSystem::HandleMouseMsg(int x, int y, CD_En_MouseOper oper) const
{
	g_c_handle_mouse_msg.Call<void>(this, x, y, oper, 0);
	return true;
}

bool stCD_KeyboardMgr::PressKey(DWORD virtual_key, bool is_down) const
{
	g_c_handle_key_msg.Call<void>(this, virtual_key, (int)is_down);
	return true;
}

stCD_ShangJinInfos* stCD_ShangJinInfoMgr::GetShangJinInfo() const
{
	auto res = g_c_shang_jin_mgr_shang_jin_info.Call<stCD_ShangJinInfos*>(this);
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

int stCD_ShangJinInfos::GetTotalCnt() const
{
	auto res = g_c_ShangJinTotalCnt.Call<int>(this);
	if (res < 0 || res > 100)
	{
		assert(false);
		return -1;
	}
	return res;
}

int stCD_PackagesMgr::TidyInventoryItemSize(CD_En_TabCategory tab_cate) const
{
	return g_c_TidyItemSize.Call<int>(this, tab_cate);
}

bool stCD_PackagesMgr::TidyInventoryItem(CD_En_TabCategory tab_cate, int size) const
{
	g_c_TidyItem.Call<void>(this, tab_cate, 0, size);
	return true;
}


bool stCD_UiCtrl::HandleMsg(const stCD_UiCtrlMsg& msg) const
{
	//mhoclien中的函数，专门处理控件消息的。只有一个参数，是stCD_UiCtrlMsg
	/*
	$ ==>    01018760       55                     PUSH EBP                                 ; ui ctrl handle msg
	$+1      01018761       8BEC                   MOV EBP, ESP
	$+3      01018763       56                     PUSH ESI
	$+4      01018764       8BF1                   MOV ESI, ECX                             ; ntdll.77150179
	$+6      01018766       8A46 41                MOV AL, BYTE PTR DS:[ESI+41]
	$+9      01018769       84C0                   TEST AL, AL
	$+B      0101876B       74 22                  JE SHORT mhoclien.0101878F
	$+D      0101876D       0FB6C0                 MOVZX EAX, AL
	$+10     01018770       8B1486                 MOV EDX, DWORD PTR DS:[ESI+EAX*4]
	$+13     01018773       8D0C86                 LEA ECX, DWORD PTR DS:[ESI+EAX*4]
	$+16     01018776       8B42 08                MOV EAX, DWORD PTR DS:[EDX+8]
	$+19     01018779       FFD0                   CALL EAX
	$+1B     0101877B       8B10                   MOV EDX, DWORD PTR DS:[EAX]
	$+1D     0101877D       8BC8                   MOV ECX, EAX
	$+1F     0101877F       8B42 7C                MOV EAX, DWORD PTR DS:[EDX+7C]
	$+22     01018782       FFD0                   CALL EAX
	$+24     01018784       84C0                   TEST AL, AL
	$+26     01018786       74 07                  JE SHORT mhoclien.0101878F
	$+28     01018788       32C0                   XOR AL, AL
	$+2A     0101878A       5E                     POP ESI                                  ; ntdll.7715FD91
	$+2B     0101878B       5D                     POP EBP                                  ; ntdll.7715FD91
	$+2C     0101878C       C2 0400                RETN 4
	$+2F     0101878F       8A46 41                MOV AL, BYTE PTR DS:[ESI+41]
	$+32     01018792       57                     PUSH EDI
	$+33     01018793       8B7D 08                MOV EDI, DWORD PTR SS:[EBP+8]
	$+36     01018796       84C0                   TEST AL, AL
	$+38     01018798       74 1C                  JE SHORT mhoclien.010187B6
	$+3A     0101879A       0FB6C8                 MOVZX ECX, AL
	$+3D     0101879D       8B148E                 MOV EDX, DWORD PTR DS:[ESI+ECX*4]
	$+40     010187A0       8B42 08                MOV EAX, DWORD PTR DS:[EDX+8]
	$+43     010187A3       8D0C8E                 LEA ECX, DWORD PTR DS:[ESI+ECX*4]
	$+46     010187A6       FFD0                   CALL EAX
	$+48     010187A8       8B10                   MOV EDX, DWORD PTR DS:[EAX]
	$+4A     010187AA       8BC8                   MOV ECX, EAX
	$+4C     010187AC       8B42 20                MOV EAX, DWORD PTR DS:[EDX+20]
	$+4F     010187AF       57                     PUSH EDI
	$+50     010187B0       FFD0                   CALL EAX
	$+52     010187B2       84C0                   TEST AL, AL
	$+54     010187B4       75 18                  JNZ SHORT mhoclien.010187CE
	$+56     010187B6       8B4E 20                MOV ECX, DWORD PTR DS:[ESI+20]
	$+59     010187B9       85C9                   TEST ECX, ECX                            ; ntdll.77150179
	$+5B     010187BB       74 11                  JE SHORT mhoclien.010187CE
	$+5D     010187BD       8B11                   MOV EDX, DWORD PTR DS:[ECX]
	$+5F     010187BF       8B82 94010000          MOV EAX, DWORD PTR DS:[EDX+194]
	$+65     010187C5       57                     PUSH EDI
	$+66     010187C6       FFD0                   CALL EAX
	$+68     010187C8       5F                     POP EDI                                  ; ntdll.7715FD91
	$+69     010187C9       5E                     POP ESI                                  ; ntdll.7715FD91
	$+6A     010187CA       5D                     POP EBP                                  ; ntdll.7715FD91
	$+6B     010187CB       C2 0400                RETN 4
	$+6E     010187CE       5F                     POP EDI                                  ; ntdll.7715FD91
	$+6F     010187CF       32C0                   XOR AL, AL
	$+71     010187D1       5E                     POP ESI                                  ; ntdll.7715FD91
	$+72     010187D2       5D                     POP EBP                                  ; ntdll.7715FD91
	$+73     010187D3       C2 0400                RETN 4
	*/
	//在某个控件上，左键按下时会断住，mhoclient
	/*
	$-FF     011F7422        885D E7                   MOV BYTE PTR SS:[EBP-19], BL
	$-FC     011F7425        885D E0                   MOV BYTE PTR SS:[EBP-20], BL
	$-F9     011F7428        FFD2                      CALL EDX
	$-F7     011F742A        8B75 F8                   MOV ESI, DWORD PTR SS:[EBP-8]
	$-F4     011F742D        8B8E CC010000             MOV ECX, DWORD PTR DS:[ESI+1CC]
	$-EE     011F7433        3BCB                      CMP ECX, EBX
	$-EC     011F7435        74 05                     JE SHORT mhoclien.011F743C
	$-EA     011F7437        E8 A49EDBFF               CALL mhoclien.00FB12E0
	$-E5     011F743C        8B7D E8                   MOV EDI, DWORD PTR SS:[EBP-18]
	$-E2     011F743F        899E CC010000             MOV DWORD PTR DS:[ESI+1CC], EBX
	$-DC     011F7445        804F 34 04                OR BYTE PTR DS:[EDI+34], 4
	$-D8     011F7449        EB 67                     JMP SHORT mhoclien.011F74B2
	$-D6     011F744B        3BF3                      CMP ESI, EBX
	$-D4     011F744D        75 63                     JNZ SHORT mhoclien.011F74B2
	$-D2     011F744F        8B45 08                   MOV EAX, DWORD PTR SS:[EBP+8]
	$-CF     011F7452        8B4D CC                   MOV ECX, DWORD PTR SS:[EBP-34]
	$-CC     011F7455        69C0 D0000000             IMUL EAX, EAX, 0D0
	$-C6     011F745B        8DB408 C0010000           LEA ESI, DWORD PTR DS:[EAX+ECX+1C0]
	$-BF     011F7462        8B46 04                   MOV EAX, DWORD PTR DS:[ESI+4]
	$-BC     011F7465        3BC3                      CMP EAX, EBX
	$-BA     011F7467        76 46                     JBE SHORT mhoclien.011F74AF
	$-B8     011F7469        8B16                      MOV EDX, DWORD PTR DS:[ESI]
	$-B6     011F746B        8D4C82 FC                 LEA ECX, DWORD PTR DS:[EDX+EAX*4-4]
	$-B2     011F746F        894D F0                   MOV DWORD PTR SS:[EBP-10], ECX           ; ntdll.771501F0
	$-AF     011F7472        8945 F4                   MOV DWORD PTR SS:[EBP-C], EAX
	$-AC     011F7475        8B55 F0                   MOV EDX, DWORD PTR SS:[EBP-10]
	$-A9     011F7478        8B0A                      MOV ECX, DWORD PTR DS:[EDX]
	$-A7     011F747A        3BCB                      CMP ECX, EBX
	$-A5     011F747C        74 05                     JE SHORT mhoclien.011F7483
	$-A3     011F747E        E8 5D9EDBFF               CALL mhoclien.00FB12E0
	$-9E     011F7483        836D F0 04                SUB DWORD PTR SS:[EBP-10], 4
	$-9A     011F7487        836D F4 01                SUB DWORD PTR SS:[EBP-C], 1
	$-96     011F748B      ^ 75 E8                     JNZ SHORT mhoclien.011F7475
	$-94     011F748D        F746 08 FEFFFFFF          TEST DWORD PTR DS:[ESI+8], FFFFFFFE
	$-8D     011F7494        76 19                     JBE SHORT mhoclien.011F74AF
	$-8B     011F7496        8B06                      MOV EAX, DWORD PTR DS:[ESI]
	$-89     011F7498        3BC3                      CMP EAX, EBX
	$-87     011F749A        74 10                     JE SHORT mhoclien.011F74AC
	$-85     011F749C        8B0D 1C50A401             MOV ECX, DWORD PTR DS:[1A4501C]
	$-7F     011F74A2        8B11                      MOV EDX, DWORD PTR DS:[ECX]
	$-7D     011F74A4        50                        PUSH EAX
	$-7C     011F74A5        8B42 30                   MOV EAX, DWORD PTR DS:[EDX+30]
	$-79     011F74A8        FFD0                      CALL EAX
	$-77     011F74AA        891E                      MOV DWORD PTR DS:[ESI], EBX
	$-75     011F74AC        895E 08                   MOV DWORD PTR DS:[ESI+8], EBX
	$-72     011F74AF        895E 04                   MOV DWORD PTR DS:[ESI+4], EBX
	$-6F     011F74B2        885D FE                   MOV BYTE PTR SS:[EBP-2], BL
	$-6C     011F74B5        385D EE                   CMP BYTE PTR SS:[EBP-12], BL
	$-69     011F74B8        0F86 8D000000             JBE mhoclien.011F754B
	$-63     011F74BE        33F6                      XOR ESI, ESI
	$-61     011F74C0        8975 F0                   MOV DWORD PTR SS:[EBP-10], ESI
	$-5E     011F74C3        8B55 A8                   MOV EDX, DWORD PTR SS:[EBP-58]
	$-5B     011F74C6        8BCE                      MOV ECX, ESI
	$-59     011F74C8        D3EA                      SHR EDX, CL
	$-57     011F74CA        F6C2 01                   TEST DL, 1
	$-54     011F74CD        74 67                     JE SHORT mhoclien.011F7536
	$-52     011F74CF        B8 01000000               MOV EAX, 1
	$-4D     011F74D4        D3E0                      SHL EAX, CL
	$-4B     011F74D6        8547 1C                   TEST DWORD PTR DS:[EDI+1C], EAX
	$-48     011F74D9        75 5B                     JNZ SHORT mhoclien.011F7536
	$-46     011F74DB        8A4D 08                   MOV CL, BYTE PTR SS:[EBP+8]
	$-43     011F74DE        8A55 FF                   MOV DL, BYTE PTR SS:[EBP-1]
	$-40     011F74E1        C745 B0 10000000          MOV DWORD PTR SS:[EBP-50], 10            ; 消息ID CD_En_UiCtrlMsg
	$-39     011F74E8        895D B4                   MOV DWORD PTR SS:[EBP-4C], EBX
	$-36     011F74EB        895D B8                   MOV DWORD PTR SS:[EBP-48], EBX
	$-33     011F74EE        885D C3                   MOV BYTE PTR SS:[EBP-3D], BL
	$-30     011F74F1        884D C1                   MOV BYTE PTR SS:[EBP-3F], CL
	$-2D     011F74F4        885D C0                   MOV BYTE PTR SS:[EBP-40], BL
	$-2A     011F74F7        8855 C2                   MOV BYTE PTR SS:[EBP-3E], DL
	$-27     011F74FA        395D C8                   CMP DWORD PTR SS:[EBP-38], EBX
	$-24     011F74FD        74 05                     JE SHORT mhoclien.011F7504
	$-22     011F74FF        8B75 C8                   MOV ESI, DWORD PTR SS:[EBP-38]
	$-1F     011F7502        EB 09                     JMP SHORT mhoclien.011F750D
	$-1D     011F7504        8B45 CC                   MOV EAX, DWORD PTR SS:[EBP-34]
	$-1A     011F7507        8BB0 B4000000             MOV ESI, DWORD PTR DS:[EAX+B4]
	$-14     011F750D        8A4D FE                   MOV CL, BYTE PTR SS:[EBP-2]
	$-11     011F7510        884D BC                   MOV BYTE PTR SS:[EBP-44], CL
	$-E      011F7513        8B16                      MOV EDX, DWORD PTR DS:[ESI]
	$-C      011F7515        8B92 94010000             MOV EDX, DWORD PTR DS:[EDX+194]          ; 虚函数偏移 g_svo_ui_ctrl_handle_msg
	$-6      011F751B        8D45 B0                   LEA EAX, DWORD PTR SS:[EBP-50]
	$-3      011F751E        50                        PUSH EAX
	$-2      011F751F        8BCE                      MOV ECX, ESI
	$ ==>    011F7521        FFD2                      CALL EDX									;调用g_c_ui_ctrl_handle_msg
	$+2      011F7523        8B45 F0                   MOV EAX, DWORD PTR SS:[EBP-10]
	$+5      011F7526        56                        PUSH ESI
	$+6      011F7527        50                        PUSH EAX
	$+7      011F7528        8BCF                      MOV ECX, EDI
	$+9      011F752A        E8 0193DFFF               CALL mhoclien.00FF0830
	$+E      011F752F        804F 34 04                OR BYTE PTR DS:[EDI+34], 4
	$+12     011F7533        8B75 F0                   MOV ESI, DWORD PTR SS:[EBP-10]
	$+15     011F7536        8A45 FE                   MOV AL, BYTE PTR SS:[EBP-2]
	$+18     011F7539        FEC0                      INC AL
	$+1A     011F753B        46                        INC ESI
	$+1B     011F753C        8845 FE                   MOV BYTE PTR SS:[EBP-2], AL
	$+1E     011F753F        8975 F0                   MOV DWORD PTR SS:[EBP-10], ESI
	$+21     011F7542        3A45 EE                   CMP AL, BYTE PTR SS:[EBP-12]
	$+24     011F7545      ^ 0F82 78FFFFFF             JB mhoclien.011F74C3
	$+2A     011F754B        395D C8                   CMP DWORD PTR SS:[EBP-38], EBX
	$+2D     011F754E        74 29                     JE SHORT mhoclien.011F7579
	$+2F     011F7550        8B4D C8                   MOV ECX, DWORD PTR SS:[EBP-38]
	$+32     011F7553        E8 189EDBFF               CALL mhoclien.00FB1370
	$+37     011F7558        8BF0                      MOV ESI, EAX
	$+39     011F755A        8B47 08                   MOV EAX, DWORD PTR DS:[EDI+8]
	$+3C     011F755D        3BC3                      CMP EAX, EBX
	$+3E     011F755F        74 13                     JE SHORT mhoclien.011F7574
	$+40     011F7561        8300 FF                   ADD DWORD PTR DS:[EAX], -1
	$+43     011F7564        75 0E                     JNZ SHORT mhoclien.011F7574
	$+45     011F7566        8B0D 1C50A401             MOV ECX, DWORD PTR DS:[1A4501C]
	$+4B     011F756C        8B11                      MOV EDX, DWORD PTR DS:[ECX]
	$+4D     011F756E        50                        PUSH EAX
	$+4E     011F756F        8B42 30                   MOV EAX, DWORD PTR DS:[EDX+30]
	$+51     011F7572        FFD0                      CALL EAX
	$+53     011F7574        8977 08                   MOV DWORD PTR DS:[EDI+8], ESI
	$+56     011F7577        EB 1D                     JMP SHORT mhoclien.011F7596
	$+58     011F7579        8B47 08                   MOV EAX, DWORD PTR DS:[EDI+8]
	$+5B     011F757C        3BC3                      CMP EAX, EBX
	$+5D     011F757E        74 13                     JE SHORT mhoclien.011F7593
	$+5F     011F7580        8300 FF                   ADD DWORD PTR DS:[EAX], -1
	$+62     011F7583        75 0E                     JNZ SHORT mhoclien.011F7593
	$+64     011F7585        8B0D 1C50A401             MOV ECX, DWORD PTR DS:[1A4501C]
	$+6A     011F758B        8B11                      MOV EDX, DWORD PTR DS:[ECX]
	$+6C     011F758D        50                        PUSH EAX
	$+6D     011F758E        8B42 30                   MOV EAX, DWORD PTR DS:[EDX+30]
	$+70     011F7591        FFD0                      CALL EAX
	$+72     011F7593        895F 08                   MOV DWORD PTR DS:[EDI+8], EBX
	$+75     011F7596        8067 34 F7                AND BYTE PTR DS:[EDI+34], 0F7
	$+79     011F759A        8B4D 94                   MOV ECX, DWORD PTR SS:[EBP-6C]
	$+7C     011F759D        895F 30                   MOV DWORD PTR DS:[EDI+30], EBX
	$+7F     011F75A0        5F                        POP EDI                                  ; ntdll.7715FD91
	$+80     011F75A1        5E                        POP ESI                                  ; ntdll.7715FD91
	$+81     011F75A2        3BCB                      CMP ECX, EBX
	$+83     011F75A4        74 05                     JE SHORT mhoclien.011F75AB
	$+85     011F75A6        E8 359DDBFF               CALL mhoclien.00FB12E0
	$+8A     011F75AB        8B4D D0                   MOV ECX, DWORD PTR SS:[EBP-30]
	$+8D     011F75AE        3BCB                      CMP ECX, EBX
	$+8F     011F75B0        74 05                     JE SHORT mhoclien.011F75B7
	$+91     011F75B2        E8 299DDBFF               CALL mhoclien.00FB12E0
	$+96     011F75B7        8B4D C8                   MOV ECX, DWORD PTR SS:[EBP-38]
	$+99     011F75BA        3BCB                      CMP ECX, EBX
	$+9B     011F75BC        5B                        POP EBX                                  ; ntdll.7715FD91
	$+9C     011F75BD        74 05                     JE SHORT mhoclien.011F75C4
	$+9E     011F75BF        E8 1C9DDBFF               CALL mhoclien.00FB12E0
	$+A3     011F75C4        8BE5                      MOV ESP, EBP
	$+A5     011F75C6        5D                        POP EBP                                  ; ntdll.7715FD91
	$+A6     011F75C7        C2 0400                   RETN 4
	*/
	
	g_c_ui_ctrl_handle_msg.Call<void>(this, &msg);
	return true;
}

bool stCD_UiCtrlEdit_StrMgr::ClearString() const
{
	//mhoclien
	/*
	$ ==>    01096440        55                        PUSH EBP
	$+1      01096441        8BEC                      MOV EBP, ESP
	$+3      01096443        8B45 0C                   MOV EAX, DWORD PTR SS:[EBP+C]
	$+6      01096446        8B55 08                   MOV EDX, DWORD PTR SS:[EBP+8]
	$+9      01096449        3BC2                      CMP EAX, EDX
	$+B      0109644B        72 10                     JB SHORT mhoclien.0109645D
	$+D      0109644D        8B49 08                   MOV ECX, DWORD PTR DS:[ECX+8]
	$+10     01096450        2BC2                      SUB EAX, EDX
	$+12     01096452        50                        PUSH EAX
	$+13     01096453        52                        PUSH EDX
	$+14     01096454        E8 57F30000               CALL mhoclien.010A57B0                   ; call ClearString，即call g_c_UiCtrlEditClrStr
	$+19     01096459        5D                        POP EBP                                  ; mhoclien.01096459
	$+1A     0109645A        C2 0800                   RETN 8
	$+1D     0109645D        8B49 08                   MOV ECX, DWORD PTR DS:[ECX+8]
	$+20     01096460        33C0                      XOR EAX, EAX
	$+22     01096462        50                        PUSH EAX
	$+23     01096463        52                        PUSH EDX
	$+24     01096464        E8 47F30000               CALL mhoclien.010A57B0
	$+29     01096469        5D                        POP EBP                                  ; mhoclien.01096459
	$+2A     0109646A        C2 0800                   RETN 8
	*/
	//g_c_UiCtrlEditClrStr函数
	/*
	$ ==>    010A57B0        55                        PUSH EBP
	$+1      010A57B1        8BEC                      MOV EBP, ESP
	$+3      010A57B3        83EC 1C                   SUB ESP, 1C
	$+6      010A57B6        837D 0C FF                CMP DWORD PTR SS:[EBP+C], -1
	$+A      010A57BA        53                        PUSH EBX
	$+B      010A57BB        56                        PUSH ESI
	$+C      010A57BC        57                        PUSH EDI
	$+D      010A57BD        8BF9                      MOV EDI, ECX
	$+F      010A57BF        897D FC                   MOV DWORD PTR SS:[EBP-4], EDI
	$+12     010A57C2        75 08                     JNZ SHORT mhoclien.010A57CC
	$+14     010A57C4        E8 17AFFFFF               CALL mhoclien.010A06E0
	$+19     010A57C9        8945 0C                   MOV DWORD PTR SS:[EBP+C], EAX
	$+1C     010A57CC        8B5D 0C                   MOV EBX, DWORD PTR SS:[EBP+C]
	$+1F     010A57CF        8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+21     010A57D1        8B75 08                   MOV ESI, DWORD PTR SS:[EBP+8]
	$+24     010A57D4        8B50 0C                   MOV EDX, DWORD PTR DS:[EAX+C]
	$+27     010A57D7        53                        PUSH EBX
	$+28     010A57D8        56                        PUSH ESI
	$+29     010A57D9        8BCF                      MOV ECX, EDI
	$+2B     010A57DB        FFD2                      CALL EDX
	$+2D     010A57DD        8D45 F4                   LEA EAX, DWORD PTR SS:[EBP-C]
	$+30     010A57E0        50                        PUSH EAX
	$+31     010A57E1        56                        PUSH ESI
	$+32     010A57E2        8D4D E4                   LEA ECX, DWORD PTR SS:[EBP-1C]
	$+35     010A57E5        51                        PUSH ECX
	$+36     010A57E6        8BCF                      MOV ECX, EDI
	$+38     010A57E8        E8 03AEFFFF               CALL mhoclien.010A05F0
	$+3D     010A57ED        8B75 E4                   MOV ESI, DWORD PTR SS:[EBP-1C]
	$+40     010A57F0        895D F8                   MOV DWORD PTR SS:[EBP-8], EBX
	$+43     010A57F3        8B5D E8                   MOV EBX, DWORD PTR SS:[EBP-18]
	$+46     010A57F6        C745 F0 00000000          MOV DWORD PTR SS:[EBP-10], 0
	$+4D     010A57FD        C645 0B 00                MOV BYTE PTR SS:[EBP+B], 0
	$+51     010A5801        85F6                      TEST ESI, ESI
	$+53     010A5803        0F84 17020000             JE mhoclien.010A5A20
	$+59     010A5809        85DB                      TEST EBX, EBX
	$+5B     010A580B        7C 49                     JL SHORT mhoclien.010A5856
	$+5D     010A580D        3B5E 04                   CMP EBX, DWORD PTR DS:[ESI+4]
	$+60     010A5810        7D 44                     JGE SHORT mhoclien.010A5856
	$+62     010A5812        8B16                      MOV EDX, DWORD PTR DS:[ESI]              ; mhoclien.01496A34
	$+64     010A5814        8B0C9A                    MOV ECX, DWORD PTR DS:[EDX+EBX*4]
	$+67     010A5817        8B51 04                   MOV EDX, DWORD PTR DS:[ECX+4]
	$+6A     010A581A        8B7D 0C                   MOV EDI, DWORD PTR SS:[EBP+C]
	$+6D     010A581D        8BC2                      MOV EAX, EDX
	$+6F     010A581F        2B45 F4                   SUB EAX, DWORD PTR SS:[EBP-C]            ; mhoclien.01096459
	$+72     010A5822        3945 0C                   CMP DWORD PTR SS:[EBP+C], EAX
	$+75     010A5825        72 02                     JB SHORT mhoclien.010A5829
	$+77     010A5827        8BF8                      MOV EDI, EAX
	$+79     010A5829        3BFA                      CMP EDI, EDX
	$+7B     010A582B        77 29                     JA SHORT mhoclien.010A5856
	$+7D     010A582D        8B45 F4                   MOV EAX, DWORD PTR SS:[EBP-C]            ; mhoclien.01096459
	$+80     010A5830        03C7                      ADD EAX, EDI
	$+82     010A5832        3BC2                      CMP EAX, EDX
	$+84     010A5834        8B55 F4                   MOV EDX, DWORD PTR SS:[EBP-C]            ; mhoclien.01096459
	$+87     010A5837        50                        PUSH EAX
	$+88     010A5838        52                        PUSH EDX
	$+89     010A5839        0F9345 0B                 SETNB BYTE PTR SS:[EBP+B]
	$+8D     010A583D        894D F0                   MOV DWORD PTR SS:[EBP-10], ECX
	$+90     010A5840        E8 3BDEFFFF               CALL mhoclien.010A3680
	$+95     010A5845        8B45 0C                   MOV EAX, DWORD PTR SS:[EBP+C]
	$+98     010A5848        2BC7                      SUB EAX, EDI
	$+9A     010A584A        3B5E 04                   CMP EBX, DWORD PTR DS:[ESI+4]
	$+9D     010A584D        8945 F8                   MOV DWORD PTR SS:[EBP-8], EAX
	$+A0     010A5850        7D 04                     JGE SHORT mhoclien.010A5856
	$+A2     010A5852        43                        INC EBX
	$+A3     010A5853        895D E8                   MOV DWORD PTR SS:[EBP-18], EBX
	$+A6     010A5856        85F6                      TEST ESI, ESI
	$+A8     010A5858        0F84 C2010000             JE mhoclien.010A5A20
	$+AE     010A585E        85DB                      TEST EBX, EBX
	$+B0     010A5860        0F8C 53010000             JL mhoclien.010A59B9
	$+B6     010A5866        3B5E 04                   CMP EBX, DWORD PTR DS:[ESI+4]
	$+B9     010A5869        0F8D 4A010000             JGE mhoclien.010A59B9
	$+BF     010A586F        8B06                      MOV EAX, DWORD PTR DS:[ESI]              ; mhoclien.01496A34
	$+C1     010A5871        8B3C98                    MOV EDI, DWORD PTR DS:[EAX+EBX*4]
	$+C4     010A5874        8B47 04                   MOV EAX, DWORD PTR DS:[EDI+4]
	$+C7     010A5877        8B55 F8                   MOV EDX, DWORD PTR SS:[EBP-8]
	$+CA     010A587A        8945 EC                   MOV DWORD PTR SS:[EBP-14], EAX
	$+CD     010A587D        3BD0                      CMP EDX, EAX
	$+CF     010A587F        0F82 01010000             JB mhoclien.010A5986
	$+D5     010A5885        8B4D FC                   MOV ECX, DWORD PTR SS:[EBP-4]
	$+D8     010A5888        8B11                      MOV EDX, DWORD PTR DS:[ECX]              ; mhoclien.01496AD0
	$+DA     010A588A        8B42 14                   MOV EAX, DWORD PTR DS:[EDX+14]
	$+DD     010A588D        57                        PUSH EDI
	$+DE     010A588E        FFD0                      CALL EAX
	$+E0     010A5890        8B0D 1C50A401             MOV ECX, DWORD PTR DS:[1A4501C]
	$+E6     010A5896        8B11                      MOV EDX, DWORD PTR DS:[ECX]              ; mhoclien.01496AD0
	$+E8     010A5898        8B07                      MOV EAX, DWORD PTR DS:[EDI]
	$+EA     010A589A        8B52 30                   MOV EDX, DWORD PTR DS:[EDX+30]
	$+ED     010A589D        50                        PUSH EAX
	$+EE     010A589E        FFD2                      CALL EDX
	$+F0     010A58A0        33C0                      XOR EAX, EAX
	$+F2     010A58A2        8907                      MOV DWORD PTR DS:[EDI], EAX
	$+F4     010A58A4        8947 08                   MOV DWORD PTR DS:[EDI+8], EAX
	$+F7     010A58A7        8947 04                   MOV DWORD PTR DS:[EDI+4], EAX
	$+FA     010A58AA        3B5E 04                   CMP EBX, DWORD PTR DS:[ESI+4]
	$+FD     010A58AD        0F8D C5000000             JGE mhoclien.010A5978
	$+103    010A58B3        837E 04 01                CMP DWORD PTR DS:[ESI+4], 1
	$+107    010A58B7        75 40                     JNZ SHORT mhoclien.010A58F9
	$+109    010A58B9        8B06                      MOV EAX, DWORD PTR DS:[ESI]              ; mhoclien.01496A34
	$+10B    010A58BB        6A 01                     PUSH 1
	$+10D    010A58BD        50                        PUSH EAX
	$+10E    010A58BE        E8 1DE5FFFF               CALL mhoclien.010A3DE0
	$+113    010A58C3        83C4 08                   ADD ESP, 8
	$+116    010A58C6        F746 08 FEFFFFFF          TEST DWORD PTR DS:[ESI+8], FFFFFFFE
	$+11D    010A58CD        76 21                     JBE SHORT mhoclien.010A58F0
	$+11F    010A58CF        8B06                      MOV EAX, DWORD PTR DS:[ESI]              ; mhoclien.01496A34
	$+121    010A58D1        85C0                      TEST EAX, EAX
	$+123    010A58D3        74 14                     JE SHORT mhoclien.010A58E9
	$+125    010A58D5        8B0D 1C50A401             MOV ECX, DWORD PTR DS:[1A4501C]
	$+12B    010A58DB        8B11                      MOV EDX, DWORD PTR DS:[ECX]              ; mhoclien.01496AD0
	$+12D    010A58DD        50                        PUSH EAX
	$+12E    010A58DE        8B42 30                   MOV EAX, DWORD PTR DS:[EDX+30]
	$+131    010A58E1        FFD0                      CALL EAX
	$+133    010A58E3        C706 00000000             MOV DWORD PTR DS:[ESI], 0
	$+139    010A58E9        C746 08 00000000          MOV DWORD PTR DS:[ESI+8], 0
	$+140    010A58F0        C746 04 00000000          MOV DWORD PTR DS:[ESI+4], 0
	$+147    010A58F7        EB 7F                     JMP SHORT mhoclien.010A5978
	$+149    010A58F9        8B0E                      MOV ECX, DWORD PTR DS:[ESI]              ; mhoclien.01496A34
	$+14B    010A58FB        8B3C99                    MOV EDI, DWORD PTR DS:[ECX+EBX*4]
	$+14E    010A58FE        3BF8                      CMP EDI, EAX
	$+150    010A5900        74 54                     JE SHORT mhoclien.010A5956
	$+152    010A5902        8B57 14                   MOV EDX, DWORD PTR DS:[EDI+14]
	$+155    010A5905        8B47 10                   MOV EAX, DWORD PTR DS:[EDI+10]
	$+158    010A5908        52                        PUSH EDX
	$+159    010A5909        50                        PUSH EAX
	$+15A    010A590A        E8 61B9FFFF               CALL mhoclien.010A1270
	$+15F    010A590F        8B0D 1C50A401             MOV ECX, DWORD PTR DS:[1A4501C]
	$+165    010A5915        8B11                      MOV EDX, DWORD PTR DS:[ECX]              ; mhoclien.01496AD0
	$+167    010A5917        8B47 10                   MOV EAX, DWORD PTR DS:[EDI+10]
	$+16A    010A591A        8B52 30                   MOV EDX, DWORD PTR DS:[EDX+30]
	$+16D    010A591D        83C4 08                   ADD ESP, 8
	$+170    010A5920        50                        PUSH EAX
	$+171    010A5921        FFD2                      CALL EDX
	$+173    010A5923        8B4F 0C                   MOV ECX, DWORD PTR DS:[EDI+C]
	$+176    010A5926        894D F4                   MOV DWORD PTR SS:[EBP-C], ECX
	$+179    010A5929        85C9                      TEST ECX, ECX
	$+17B    010A592B        74 1B                     JE SHORT mhoclien.010A5948
	$+17D    010A592D        8301 FF                   ADD DWORD PTR DS:[ECX], -1
	$+180    010A5930        75 16                     JNZ SHORT mhoclien.010A5948
	$+182    010A5932        E8 396DF7FF               CALL mhoclien.0101C670
	$+187    010A5937        8B0D 1C50A401             MOV ECX, DWORD PTR DS:[1A4501C]
	$+18D    010A593D        8B01                      MOV EAX, DWORD PTR DS:[ECX]              ; mhoclien.01496AD0
	$+18F    010A593F        8B55 F4                   MOV EDX, DWORD PTR SS:[EBP-C]            ; mhoclien.01096459
	$+192    010A5942        8B40 30                   MOV EAX, DWORD PTR DS:[EAX+30]
	$+195    010A5945        52                        PUSH EDX
	$+196    010A5946        FFD0                      CALL EAX
	$+198    010A5948        8B0D 1C50A401             MOV ECX, DWORD PTR DS:[1A4501C]
	$+19E    010A594E        8B11                      MOV EDX, DWORD PTR DS:[ECX]              ; mhoclien.01496AD0
	$+1A0    010A5950        8B42 30                   MOV EAX, DWORD PTR DS:[EDX+30]
	$+1A3    010A5953        57                        PUSH EDI
	$+1A4    010A5954        FFD0                      CALL EAX
	$+1A6    010A5956        8B0E                      MOV ECX, DWORD PTR DS:[ESI]              ; mhoclien.01496A34
	$+1A8    010A5958        8B56 04                   MOV EDX, DWORD PTR DS:[ESI+4]
	$+1AB    010A595B        8D0499                    LEA EAX, DWORD PTR DS:[ECX+EBX*4]
	$+1AE    010A595E        2BD3                      SUB EDX, EBX
	$+1B0    010A5960        8D0C95 FCFFFFFF           LEA ECX, DWORD PTR DS:[EDX*4-4]
	$+1B7    010A5967        51                        PUSH ECX
	$+1B8    010A5968        8D50 04                   LEA EDX, DWORD PTR DS:[EAX+4]
	$+1BB    010A596B        52                        PUSH EDX
	$+1BC    010A596C        50                        PUSH EAX
	$+1BD    010A596D        E8 261C2800               CALL mhoclien.01327598                   ; JMP 到 MSVCR90.memmove
	$+1C2    010A5972        83C4 0C                   ADD ESP, 0C
	$+1C5    010A5975        FF4E 04                   DEC DWORD PTR DS:[ESI+4]
	$+1C8    010A5978        8B45 EC                   MOV EAX, DWORD PTR SS:[EBP-14]
	$+1CB    010A597B        2945 F8                   SUB DWORD PTR SS:[EBP-8], EAX
	$+1CE    010A597E      ^ 0F85 D2FEFFFF             JNZ mhoclien.010A5856
	$+1D4    010A5984        EB 33                     JMP SHORT mhoclien.010A59B9
	$+1D6    010A5986        8B4D F0                   MOV ECX, DWORD PTR SS:[EBP-10]           ; mhoclien.010A03A0
	$+1D9    010A5989        85C9                      TEST ECX, ECX
	$+1DB    010A598B        74 2C                     JE SHORT mhoclien.010A59B9
	$+1DD    010A598D        807D 0B 00                CMP BYTE PTR SS:[EBP+B], 0
	$+1E1    010A5991        74 26                     JE SHORT mhoclien.010A59B9
	$+1E3    010A5993        2BC2                      SUB EAX, EDX
	$+1E5    010A5995        50                        PUSH EAX
	$+1E6    010A5996        8B41 04                   MOV EAX, DWORD PTR DS:[ECX+4]
	$+1E9    010A5999        50                        PUSH EAX
	$+1EA    010A599A        52                        PUSH EDX
	$+1EB    010A599B        8B55 FC                   MOV EDX, DWORD PTR SS:[EBP-4]
	$+1EE    010A599E        8B42 08                   MOV EAX, DWORD PTR DS:[EDX+8]
	$+1F1    010A59A1        57                        PUSH EDI
	$+1F2    010A59A2        50                        PUSH EAX
	$+1F3    010A59A3        E8 28E5FFFF               CALL mhoclien.010A3ED0
	$+1F8    010A59A8        57                        PUSH EDI
	$+1F9    010A59A9        8D4D E4                   LEA ECX, DWORD PTR SS:[EBP-1C]
	$+1FC    010A59AC        51                        PUSH ECX
	$+1FD    010A59AD        8B4D FC                   MOV ECX, DWORD PTR SS:[EBP-4]
	$+200    010A59B0        E8 ABFDFFFF               CALL mhoclien.010A5760
	$+205    010A59B5        C645 0B 00                MOV BYTE PTR SS:[EBP+B], 0
	$+209    010A59B9        85F6                      TEST ESI, ESI
	$+20B    010A59BB        74 43                     JE SHORT mhoclien.010A5A00
	$+20D    010A59BD        85DB                      TEST EBX, EBX
	$+20F    010A59BF        7C 3F                     JL SHORT mhoclien.010A5A00
	$+211    010A59C1        3B5E 04                   CMP EBX, DWORD PTR DS:[ESI+4]
	$+214    010A59C4        7D 3A                     JGE SHORT mhoclien.010A5A00
	$+216    010A59C6        8B16                      MOV EDX, DWORD PTR DS:[ESI]              ; mhoclien.01496A34
	$+218    010A59C8        8B3C9A                    MOV EDI, DWORD PTR DS:[EDX+EBX*4]
	$+21B    010A59CB        8B47 04                   MOV EAX, DWORD PTR DS:[EDI+4]
	$+21E    010A59CE        85C0                      TEST EAX, EAX
	$+220    010A59D0        74 21                     JE SHORT mhoclien.010A59F3
	$+222    010A59D2        8B4D F0                   MOV ECX, DWORD PTR SS:[EBP-10]           ; mhoclien.010A03A0
	$+225    010A59D5        85C9                      TEST ECX, ECX
	$+227    010A59D7        74 27                     JE SHORT mhoclien.010A5A00
	$+229    010A59D9        807D 0B 00                CMP BYTE PTR SS:[EBP+B], 0
	$+22D    010A59DD        74 21                     JE SHORT mhoclien.010A5A00
	$+22F    010A59DF        8B51 04                   MOV EDX, DWORD PTR DS:[ECX+4]
	$+232    010A59E2        50                        PUSH EAX
	$+233    010A59E3        8B45 FC                   MOV EAX, DWORD PTR SS:[EBP-4]
	$+236    010A59E6        52                        PUSH EDX
	$+237    010A59E7        8B50 08                   MOV EDX, DWORD PTR DS:[EAX+8]
	$+23A    010A59EA        6A 00                     PUSH 0
	$+23C    010A59EC        57                        PUSH EDI
	$+23D    010A59ED        52                        PUSH EDX
	$+23E    010A59EE        E8 DDE4FFFF               CALL mhoclien.010A3ED0
	$+243    010A59F3        8B4D FC                   MOV ECX, DWORD PTR SS:[EBP-4]
	$+246    010A59F6        57                        PUSH EDI
	$+247    010A59F7        8D45 E4                   LEA EAX, DWORD PTR SS:[EBP-1C]
	$+24A    010A59FA        50                        PUSH EAX
	$+24B    010A59FB        E8 60FDFFFF               CALL mhoclien.010A5760
	$+250    010A5A00        8B4D 0C                   MOV ECX, DWORD PTR SS:[EBP+C]
	$+253    010A5A03        85F6                      TEST ESI, ESI
	$+255    010A5A05        74 19                     JE SHORT mhoclien.010A5A20
	$+257    010A5A07        85DB                      TEST EBX, EBX
	$+259    010A5A09        7C 15                     JL SHORT mhoclien.010A5A20
	$+25B    010A5A0B        3B5E 04                   CMP EBX, DWORD PTR DS:[ESI+4]
	$+25E    010A5A0E        7D 10                     JGE SHORT mhoclien.010A5A20
	$+260    010A5A10        8B16                      MOV EDX, DWORD PTR DS:[ESI]              ; mhoclien.01496A34
	$+262    010A5A12        8B049A                    MOV EAX, DWORD PTR DS:[EDX+EBX*4]
	$+265    010A5A15        2948 1C                   SUB DWORD PTR DS:[EAX+1C], ECX
	$+268    010A5A18        3B5E 04                   CMP EBX, DWORD PTR DS:[ESI+4]
	$+26B    010A5A1B      ^ 7D E6                     JGE SHORT mhoclien.010A5A03
	$+26D    010A5A1D        43                        INC EBX
	$+26E    010A5A1E      ^ EB E3                     JMP SHORT mhoclien.010A5A03
	$+270    010A5A20        8B7D FC                   MOV EDI, DWORD PTR SS:[EBP-4]
	$+273    010A5A23        8B4F 10                   MOV ECX, DWORD PTR DS:[EDI+10]
	$+276    010A5A26        8D41 FF                   LEA EAX, DWORD PTR DS:[ECX-1]
	$+279    010A5A29        85C0                      TEST EAX, EAX
	$+27B    010A5A2B        7C 0E                     JL SHORT mhoclien.010A5A3B
	$+27D    010A5A2D        3BC1                      CMP EAX, ECX
	$+27F    010A5A2F        7D 0A                     JGE SHORT mhoclien.010A5A3B
	$+281    010A5A31        8B4F 0C                   MOV ECX, DWORD PTR DS:[EDI+C]
	$+284    010A5A34        8B0481                    MOV EAX, DWORD PTR DS:[ECX+EAX*4]
	$+287    010A5A37        85C0                      TEST EAX, EAX
	$+289    010A5A39        75 0D                     JNZ SHORT mhoclien.010A5A48
	$+28B    010A5A3B        6A 00                     PUSH 0
	$+28D    010A5A3D        8BCF                      MOV ECX, EDI
	$+28F    010A5A3F        E8 1CE7FFFF               CALL mhoclien.010A4160
	$+294    010A5A44        85C0                      TEST EAX, EAX
	$+296    010A5A46        74 39                     JE SHORT mhoclien.010A5A81
	$+298    010A5A48        8B48 04                   MOV ECX, DWORD PTR DS:[EAX+4]
	$+29B    010A5A4B        85C9                      TEST ECX, ECX
	$+29D    010A5A4D        76 23                     JBE SHORT mhoclien.010A5A72
	$+29F    010A5A4F        8B10                      MOV EDX, DWORD PTR DS:[EAX]
	$+2A1    010A5A51        8D71 FF                   LEA ESI, DWORD PTR DS:[ECX-1]
	$+2A4    010A5A54        85D2                      TEST EDX, EDX
	$+2A6    010A5A56        74 09                     JE SHORT mhoclien.010A5A61
	$+2A8    010A5A58        3BF1                      CMP ESI, ECX
	$+2AA    010A5A5A        73 05                     JNB SHORT mhoclien.010A5A61
	$+2AC    010A5A5C        8D0C72                    LEA ECX, DWORD PTR DS:[EDX+ESI*2]
	$+2AF    010A5A5F        EB 02                     JMP SHORT mhoclien.010A5A63
	$+2B1    010A5A61        33C9                      XOR ECX, ECX
	$+2B3    010A5A63        0FB709                    MOVZX ECX, WORD PTR DS:[ECX]
	$+2B6    010A5A66        66:83F9 0D                CMP CX, 0D
	$+2BA    010A5A6A        74 15                     JE SHORT mhoclien.010A5A81
	$+2BC    010A5A6C        66:83F9 0A                CMP CX, 0A
	$+2C0    010A5A70        74 0F                     JE SHORT mhoclien.010A5A81
	$+2C2    010A5A72        8B57 1C                   MOV EDX, DWORD PTR DS:[EDI+1C]
	$+2C5    010A5A75        8B4F 08                   MOV ECX, DWORD PTR DS:[EDI+8]
	$+2C8    010A5A78        52                        PUSH EDX
	$+2C9    010A5A79        51                        PUSH ECX
	$+2CA    010A5A7A        8BC8                      MOV ECX, EAX
	$+2CC    010A5A7C        E8 7FE1FFFF               CALL mhoclien.010A3C00
	$+2D1    010A5A81        5F                        POP EDI                                  ; mhoclien.01096459
	$+2D2    010A5A82        5E                        POP ESI                                  ; mhoclien.01096459
	$+2D3    010A5A83        5B                        POP EBX                                  ; mhoclien.01096459
	$+2D4    010A5A84        8BE5                      MOV ESP, EBP
	$+2D6    010A5A86        5D                        POP EBP                                  ; mhoclien.01096459
	$+2D7    010A5A87        C2 0800                   RETN 8
	*/
	g_c_UiCtrlEditClrStr.Call<void>(this, 0, 100);
	return true;
}

bool stCD_UiCtrlEdit::PutChar(pt_wchar c) const
{
	//mhoclien
	/*
	$-156    00FC6AB0     55                    PUSH EBP
	$-155    00FC6AB1     8BEC                  MOV EBP, ESP
	$-153    00FC6AB3     83EC 28               SUB ESP, 28
	$-150    00FC6AB6     56                    PUSH ESI
	$-14F    00FC6AB7     57                    PUSH EDI
	$-14E    00FC6AB8     6A FF                 PUSH -1
	$-14C    00FC6ABA     8BF9                  MOV EDI, ECX                             ; ntdll.771501F0
	$-14A    00FC6ABC     8B47 18               MOV EAX, DWORD PTR DS:[EDI+18]
	$-147    00FC6ABF     6A 01                 PUSH 1
	$-145    00FC6AC1     68 F0B34801           PUSH mhoclien.0148B3F0                   ; ASCII "MovieImpl::ProcessKeyboard"
	$-140    00FC6AC6     50                    PUSH EAX
	$-13F    00FC6AC7     8D4D EC               LEA ECX, DWORD PTR SS:[EBP-14]
	$-13C    00FC6ACA     E8 5106FFFF           CALL mhoclien.00FB7120
	$-137    00FC6ACF     8B75 08               MOV ESI, DWORD PTR SS:[EBP+8]
	$-134    00FC6AD2     8B46 08               MOV EAX, DWORD PTR DS:[ESI+8]
	$-131    00FC6AD5     83C6 04               ADD ESI, 4
	$-12E    00FC6AD8     C745 FC 00000000      MOV DWORD PTR SS:[EBP-4], 0
	$-127    00FC6ADF     85C0                  TEST EAX, EAX
	$-125    00FC6AE1     0F84 D8000000         JE mhoclien.00FC6BBF
	$-11F    00FC6AE7     807E 0B 00            CMP BYTE PTR DS:[ESI+B], 0
	$-11B    00FC6AEB     74 0E                 JE SHORT mhoclien.00FC6AFB
	$-119    00FC6AED     B9 40000000           MOV ECX, 40
	$-114    00FC6AF2     C745 08 05000000      MOV DWORD PTR SS:[EBP+8], 5
	$-10D    00FC6AF9     EB 0C                 JMP SHORT mhoclien.00FC6B07
	$-10B    00FC6AFB     B9 80000000           MOV ECX, 80
	$-106    00FC6B00     C745 08 06000000      MOV DWORD PTR SS:[EBP+8], 6
	$-FF     00FC6B07     8B16                  MOV EDX, DWORD PTR DS:[ESI]
	$-FD     00FC6B09     0FB6C9                MOVZX ECX, CL
	$-FA     00FC6B0C     8955 DC               MOV DWORD PTR SS:[EBP-24], EDX
	$-F7     00FC6B0F     8A56 09               MOV DL, BYTE PTR DS:[ESI+9]
	$-F4     00FC6B12     8945 E0               MOV DWORD PTR SS:[EBP-20], EAX
	$-F1     00FC6B15     8A46 08               MOV AL, BYTE PTR DS:[ESI+8]
	$-EE     00FC6B18     894D D8               MOV DWORD PTR SS:[EBP-28], ECX           ; ntdll.771501F0
	$-EB     00FC6B1B     8A4E 0A               MOV CL, BYTE PTR DS:[ESI+A]
	$-E8     00FC6B1E     80CA 80               OR DL, 80
	$-E5     00FC6B21     8845 E4               MOV BYTE PTR SS:[EBP-1C], AL
	$-E2     00FC6B24     C645 E8 00            MOV BYTE PTR SS:[EBP-18], 0
	$-DE     00FC6B28     C645 EB 00            MOV BYTE PTR SS:[EBP-15], 0
	$-DA     00FC6B2C     884D E9               MOV BYTE PTR SS:[EBP-17], CL
	$-D7     00FC6B2F     8855 EA               MOV BYTE PTR SS:[EBP-16], DL
	$-D4     00FC6B32     84C0                  TEST AL, AL
	$-D2     00FC6B34     75 0B                 JNZ SHORT mhoclien.00FC6B41
	$-D0     00FC6B36     8D4D D8               LEA ECX, DWORD PTR SS:[EBP-28]
	$-CD     00FC6B39     E8 129B0200           CALL mhoclien.00FF0650
	$-C8     00FC6B3E     8845 E4               MOV BYTE PTR SS:[EBP-1C], AL
	$-C5     00FC6B41     53                    PUSH EBX
	$-C4     00FC6B42     8B5F 30               MOV EBX, DWORD PTR DS:[EDI+30]
	$-C1     00FC6B45     85DB                  TEST EBX, EBX
	$-BF     00FC6B47     76 25                 JBE SHORT mhoclien.00FC6B6E
	$-BD     00FC6B49     8DA424 00000000       LEA ESP, DWORD PTR SS:[ESP]
	$-B6     00FC6B50     8B47 2C               MOV EAX, DWORD PTR DS:[EDI+2C]
	$-B3     00FC6B53     8B4CD8 FC             MOV ECX, DWORD PTR DS:[EAX+EBX*8-4]
	$-AF     00FC6B57     8B11                  MOV EDX, DWORD PTR DS:[ECX]
	$-AD     00FC6B59     8B92 98010000         MOV EDX, DWORD PTR DS:[EDX+198]
	$-A7     00FC6B5F     8D45 FC               LEA EAX, DWORD PTR SS:[EBP-4]
	$-A4     00FC6B62     50                    PUSH EAX
	$-A3     00FC6B63     8D45 D8               LEA EAX, DWORD PTR SS:[EBP-28]
	$-A0     00FC6B66     50                    PUSH EAX
	$-9F     00FC6B67     FFD2                  CALL EDX
	$-9D     00FC6B69     83EB 01               SUB EBX, 1
	$-9A     00FC6B6C   ^ 75 E2                 JNZ SHORT mhoclien.00FC6B50
	$-98     00FC6B6E     0FB646 0A             MOVZX EAX, BYTE PTR DS:[ESI+A]
	$-94     00FC6B72     5B                    POP EBX                                  ; ntdll.7715FD91
	$-93     00FC6B73     83F8 06               CMP EAX, 6
	$-90     00FC6B76     73 0F                 JNB SHORT mhoclien.00FC6B87
	$-8E     00FC6B78     69C0 7C060000         IMUL EAX, EAX, 67C
	$-88     00FC6B7E     8D8C38 48130000       LEA ECX, DWORD PTR DS:[EAX+EDI+1348]
	$-81     00FC6B85     EB 02                 JMP SHORT mhoclien.00FC6B89
	$-7F     00FC6B87     33C9                  XOR ECX, ECX                             ; ntdll.771501F0
	$-7D     00FC6B89     8B45 FC               MOV EAX, DWORD PTR SS:[EBP-4]
	$-7A     00FC6B8C     50                    PUSH EAX
	$-79     00FC6B8D     8B47 28               MOV EAX, DWORD PTR DS:[EDI+28]
	$-76     00FC6B90     8D55 D8               LEA EDX, DWORD PTR SS:[EBP-28]
	$-73     00FC6B93     52                    PUSH EDX
	$-72     00FC6B94     50                    PUSH EAX
	$-71     00FC6B95     E8 C6A00200           CALL mhoclien.00FF0C60
	$-6C     00FC6B9A     8B87 843F0000         MOV EAX, DWORD PTR DS:[EDI+3F84]
	$-66     00FC6BA0     C1E8 1E               SHR EAX, 1E
	$-63     00FC6BA3     83F8 03               CMP EAX, 3
	$-60     00FC6BA6     74 05                 JE SHORT mhoclien.00FC6BAD
	$-5E     00FC6BA8     83F8 01               CMP EAX, 1
	$-5B     00FC6BAB     74 62                 JE SHORT mhoclien.00FC6C0F
	$-59     00FC6BAD     8B4D 0C               MOV ECX, DWORD PTR SS:[EBP+C]
	$-56     00FC6BB0     8B55 08               MOV EDX, DWORD PTR SS:[EBP+8]
	$-53     00FC6BB3     51                    PUSH ECX                                 ; ntdll.771501F0
	$-52     00FC6BB4     56                    PUSH ESI
	$-51     00FC6BB5     52                    PUSH EDX
	$-50     00FC6BB6     8BCF                  MOV ECX, EDI
	$-4E     00FC6BB8     E8 53DAFFFF           CALL mhoclien.00FC4610
	$-49     00FC6BBD     EB 50                 JMP SHORT mhoclien.00FC6C0F
	$-47     00FC6BBF     833E 00               CMP DWORD PTR DS:[ESI], 0
	$-44     00FC6BC2     74 4B                 JE SHORT mhoclien.00FC6C0F
	$-42     00FC6BC4     0FB64E 0A             MOVZX ECX, BYTE PTR DS:[ESI+A]
	$-3E     00FC6BC8     0FB69439 643F0000     MOVZX EDX, BYTE PTR DS:[ECX+EDI+3F64]
	$-36     00FC6BD0     8D45 08               LEA EAX, DWORD PTR SS:[EBP+8]
	$-33     00FC6BD3     C1E2 06               SHL EDX, 6
	$-30     00FC6BD6     50                    PUSH EAX
	$-2F     00FC6BD7     8D8C3A 743B0000       LEA ECX, DWORD PTR DS:[EDX+EDI+3B74]
	$-28     00FC6BDE     E8 AD79FFFF           CALL mhoclien.00FBE590
	$-23     00FC6BE3     8B7D 08               MOV EDI, DWORD PTR SS:[EBP+8]
	$-20     00FC6BE6     85FF                  TEST EDI, EDI
	$-1E     00FC6BE8     74 25                 JE SHORT mhoclien.00FC6C0F
	$-1C     00FC6BEA     FF47 04               INC DWORD PTR DS:[EDI+4]
	$-19     00FC6BED     8BCF                  MOV ECX, EDI
	$-17     00FC6BEF     E8 ECA6FEFF           CALL mhoclien.00FB12E0
	$-12     00FC6BF4     0FB64E 0A             MOVZX ECX, BYTE PTR DS:[ESI+A]
	$-E      00FC6BF8     8B07                  MOV EAX, DWORD PTR DS:[EDI]
	$-C      00FC6BFA     8B16                  MOV EDX, DWORD PTR DS:[ESI]
	$-A      00FC6BFC     8B80 88010000         MOV EAX, DWORD PTR DS:[EAX+188]
	$-4      00FC6C02     51                    PUSH ECX                                 ; ntdll.771501F0
	$-3      00FC6C03     52                    PUSH EDX                                 ; 字符
	$-2      00FC6C04     8BCF                  MOV ECX, EDI
	$ ==>    00FC6C06     FFD0                  CALL EAX                                 ; call PutChar, 即call g_c_ui_ctrl_edit_put_char
	$+2      00FC6C08     8BCF                  MOV ECX, EDI
	$+4      00FC6C0A     E8 D1A6FEFF           CALL mhoclien.00FB12E0
	$+9      00FC6C0F     8B7D F4               MOV EDI, DWORD PTR SS:[EBP-C]            ; KERNELBA.75026FF0
	$+C      00FC6C12     85FF                  TEST EDI, EDI
	$+E      00FC6C14     74 18                 JE SHORT mhoclien.00FC6C2E
	$+10     00FC6C16     8B37                  MOV ESI, DWORD PTR DS:[EDI]
	$+12     00FC6C18     83C6 08               ADD ESI, 8
	$+15     00FC6C1B     E8 D0C80000           CALL mhoclien.00FD34F0
	$+1A     00FC6C20     2B45 EC               SUB EAX, DWORD PTR SS:[EBP-14]           ; MHOCli_1.0F77BD4F
	$+1D     00FC6C23     8BCF                  MOV ECX, EDI
	$+1F     00FC6C25     1B55 F0               SBB EDX, DWORD PTR SS:[EBP-10]
	$+22     00FC6C28     52                    PUSH EDX
	$+23     00FC6C29     8B16                  MOV EDX, DWORD PTR DS:[ESI]
	$+25     00FC6C2B     50                    PUSH EAX
	$+26     00FC6C2C     FFD2                  CALL EDX
	$+28     00FC6C2E     5F                    POP EDI                                  ; ntdll.7715FD91
	$+29     00FC6C2F     5E                    POP ESI                                  ; ntdll.7715FD91
	$+2A     00FC6C30     8BE5                  MOV ESP, EBP
	$+2C     00FC6C32     5D                    POP EBP                                  ; ntdll.7715FD91
	$+2D     00FC6C33     C2 0800               RETN 8
	*/
	//g_c_ui_ctrl_edit_put_char函数
	/*
	$ ==>    01096EC0     55                    PUSH EBP
	$+1      01096EC1     8BEC                  MOV EBP, ESP
	$+3      01096EC3     56                    PUSH ESI
	$+4      01096EC4     8BF1                  MOV ESI, ECX                             ; ntdll.771501F0
	$+6      01096EC6     8B86 80000000         MOV EAX, DWORD PTR DS:[ESI+80]
	$+C      01096ECC     66:8B48 54            MOV CX, WORD PTR DS:[EAX+54]
	$+10     01096ED0     66:C1E9 0C            SHR CX, 0C
	$+14     01096ED4     F6C1 01               TEST CL, 1
	$+17     01096ED7     74 07                 JE SHORT mhoclien.01096EE0
	$+19     01096ED9     32C0                  XOR AL, AL
	$+1B     01096EDB     5E                    POP ESI                                  ; ntdll.7715FD91
	$+1C     01096EDC     5D                    POP EBP                                  ; ntdll.7715FD91
	$+1D     01096EDD     C2 0800               RETN 8
	$+20     01096EE0     8B56 10               MOV EDX, DWORD PTR DS:[ESI+10]           ; MHOCli_1.0F77BD4F
	$+23     01096EE3     837A 08 00            CMP DWORD PTR DS:[EDX+8], 0
	$+27     01096EE7     74 65                 JE SHORT mhoclien.01096F4E
	$+29     01096EE9     8B86 84000000         MOV EAX, DWORD PTR DS:[ESI+84]           ; CryRende.168C28B0
	$+2F     01096EEF     83B8 A4000000 00      CMP DWORD PTR DS:[EAX+A4], 0
	$+36     01096EF6     74 56                 JE SHORT mhoclien.01096F4E
	$+38     01096EF8     8BCE                  MOV ECX, ESI
	$+3A     01096EFA     E8 01F6FFFF           CALL mhoclien.01096500
	$+3F     01096EFF     84C0                  TEST AL, AL
	$+41     01096F01     74 0B                 JE SHORT mhoclien.01096F0E
	$+43     01096F03     8BCE                  MOV ECX, ESI
	$+45     01096F05     E8 C6F5FFFF           CALL mhoclien.010964D0
	$+4A     01096F0A     84C0                  TEST AL, AL
	$+4C     01096F0C     74 40                 JE SHORT mhoclien.01096F4E
	$+4E     01096F0E     57                    PUSH EDI
	$+4F     01096F0F     8B7D 0C               MOV EDI, DWORD PTR SS:[EBP+C]
	$+52     01096F12     39BE AC000000         CMP DWORD PTR DS:[ESI+AC], EDI
	$+58     01096F18     75 33                 JNZ SHORT mhoclien.01096F4D
	$+5A     01096F1A     0FB64E 41             MOVZX ECX, BYTE PTR DS:[ESI+41]
	$+5E     01096F1E     8B148E                MOV EDX, DWORD PTR DS:[ESI+ECX*4]
	$+61     01096F21     8B42 10               MOV EAX, DWORD PTR DS:[EDX+10]
	$+64     01096F24     8D0C8E                LEA ECX, DWORD PTR DS:[ESI+ECX*4]
	$+67     01096F27     FFD0                  CALL EAX
	$+69     01096F29     8B10                  MOV EDX, DWORD PTR DS:[EAX]
	$+6B     01096F2B     57                    PUSH EDI
	$+6C     01096F2C     8B7D 08               MOV EDI, DWORD PTR SS:[EBP+8]
	$+6F     01096F2F     8BC8                  MOV ECX, EAX
	$+71     01096F31     8B42 70               MOV EAX, DWORD PTR DS:[EDX+70]
	$+74     01096F34     57                    PUSH EDI
	$+75     01096F35     FFD0                  CALL EAX
	$+77     01096F37     84C0                  TEST AL, AL
	$+79     01096F39     74 12                 JE SHORT mhoclien.01096F4D
	$+7B     01096F3B     8B8E 84000000         MOV ECX, DWORD PTR DS:[ESI+84]           ; CryRende.168C28B0
	$+81     01096F41     8B89 A4000000         MOV ECX, DWORD PTR DS:[ECX+A4]
	$+87     01096F47     57                    PUSH EDI
	$+88     01096F48     E8 43F50200           CALL mhoclien.010C6490
	$+8D     01096F4D     5F                    POP EDI                                  ; ntdll.7715FD91
	$+8E     01096F4E     B0 01                 MOV AL, 1
	$+90     01096F50     5E                    POP ESI                                  ; ntdll.7715FD91
	$+91     01096F51     5D                    POP EBP                                  ; ntdll.7715FD91
	$+92     01096F52     C2 0800               RETN 8
	*/
	return g_c_ui_ctrl_edit_put_char.Call<bool>(this, (int)c, focus_);
}

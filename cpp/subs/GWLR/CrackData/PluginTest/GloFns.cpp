#include "stdafx.h"
#include "GloFns.h"
#include <CD_CommonFiles/Data/CD_FuncCall.h>

pd_export_c const stCD_Vector3* DllGetRolePosPtr()
{
	auto cur_role = CD_FuncCall::GetCurRole();
	if (!cur_role)
		return nullptr;
	if (IsUnValidPtr(cur_role->local_obj_))
	{
		assert(false);
		return nullptr;
	}
	auto obj_info = cur_role->local_obj_->obj_info_;
	if (IsUnValidPtr(obj_info))
	{
		assert(false);
		return nullptr;
	}
	return &obj_info->pos_;
}

pd_export_c int DllGetMapId()
{
	auto cur_map_db = CD_FuncCall::GetCurMapDb();
	if (!cur_map_db)
	{
		assert(false);
		return -1;
	}
	return cur_map_db->map_db_info_id_;
}

pd_export_c int DllGetRoomId()
{
	auto cur_role = CD_FuncCall::GetCurRole();
	if (!cur_role)
		return -1;
	return cur_role->room_num_;
}

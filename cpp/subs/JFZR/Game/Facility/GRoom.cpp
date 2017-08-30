#include "stdafx.h"
#include "GRoom.h"
#include <Common/LogOut.h>
#include "GAddr.h"
#include "GOffset.h"
#include "GClass.h"
#include "GPlayer.h"

static GRoomDoor g_room_door_placeholder_;

bool GRoomBase::Update()
{
	if ( !room_obj_base_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "room obj is empty";
		assert(false);
		return false;
	}
	return timer_sync_exec_.TimerGWndExecSync([this](){
		return DoUpdateImpl();
	});
}

GRoomDoor& GRoomBase::GetTheDoor( int door_idx )
{
	auto& the_cont = the_doors_.GetAllNodes();
	if (the_cont.empty())
	{
		assert(false);
		return g_room_door_placeholder_;
	}
	--door_idx;
	door_idx = std::max(0, door_idx);
	//return doors_[door_idx % _countof(doors_)];
	door_idx = std::min(door_idx, (int)the_cont.size() - 1);
	return the_cont[door_idx];
}

/*
GType::enFbDifficulty GRoom::GetMaxDifficulty( const std::wstring& room_id )
{
	using namespace GFuncCall;
	using namespace GOffset;
	if (room_id.empty())
	{
		assert(false);
		return GType::kFD_Invalid;
	}
	auto unknown_ptr = (char*)GetFbUnknownObjByName(room_id);
	if ( !unknown_ptr )
	{
		assert(false);
		return GType::kFD_Invalid;
	}
	static_assert(sizeof(GType::enFbDifficulty) == sizeof(DWORD), "");
	assert(false);
	return GType::kFD_Invalid;
	auto fb_diff = *(GType::enFbDifficulty*)(unknown_ptr + g_o_CloneMapCurMaxLevel);
	if ( fb_diff >= GType::kFD_Invalid )
	{
		assert(false);
		return (GType::enFbDifficulty)((int)GType::kES_Invalid - 1);
	}
	else if ( fb_diff < GType::kFD_Normal )
	{
		assert(false);
		return GType::kFD_Normal;
	}
	return fb_diff;
}*/

const std::wstring& GRoomBase::GetRoomId() const
{
	return room_id_;
}

bool GRoomBase::UnsameDoorCheck( int door_idx ) const
{
	auto& the_cont = the_doors_.GetAllNodes();
	if (the_cont.empty())
	{
		assert(false);
		return false;
	}
	if ( door_idx > the_cont.size() )
	{
		assert(false);
		return false;
	}
	if ( door_idx < 1 )
	{
		assert(false);
		return false;
	}
	--door_idx;
	const auto the_door_addr = the_cont[door_idx].GetDoorObjAddr();
	for ( auto idx = door_idx + 1; idx < the_cont.size(); ++idx )
	{
		if ( the_door_addr == the_cont[idx].GetDoorObjAddr() )
			return false;
	}
	return true;
}

GRoomDoor* GRoomBase::GetDoorExcept( const std::vector<std::wstring>& door_name )
{
	auto& the_cont = the_doors_.GetAllNodes();
	if (the_cont.empty())
	{
		assert(false);
		return nullptr;
	}
	GRoomDoor* doors_tmp[100];
	assert(_countof(doors_tmp) >= the_cont.size());

	int valid_door = 0;
	for ( int idx = the_cont.size() - 1; idx >= 0; --idx )
	{
		auto& the_door = the_cont[idx];
		auto it = std::find(door_name.begin(), door_name.end(), the_door.GetGateName());
		if ( it == door_name.end() )
		{
			//return &the_door;
			doors_tmp[valid_door++] = &the_door;
			assert(valid_door <= the_cont.size());
		}
	}
	std::sort(doors_tmp, doors_tmp + valid_door, []( GRoomDoor* lhs, GRoomDoor* rhs ){
		assert(lhs);
		assert(rhs);
		return lhs->GetGateName() > rhs->GetGateName();
	});
	if (valid_door)
		return doors_tmp[0];
	return nullptr;
}

bool GRoomBase::DoUpdateImpl()
{
	using namespace GAddr;
	using namespace GOffset;
	using namespace GClass;
	auto game_str_tmp = (stGameString*)((int)room_obj_base_ + g_o_RoomMapName);
	if ( !game_str_tmp->CopyTo(room_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read room name error";
		assert(false);
		return false;
	}
	game_str_tmp = (stGameString*)((int)room_obj_base_ + g_o_RoomMapId);
	if ( !game_str_tmp->CopyTo(room_id_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read room id error";
		assert(false);
		return false;
	}

	/*struct stDoorData{
		void*	unknown1_;
		void*	door_ptr_;
		int		door_cnt_;
		void*	unknown2_;
	};
	assert(room_obj_base_);
	auto door_data = (const stDoorData*)(room_obj_base_ + g_o_RoomMapDoorObj);
	if ( door_data->door_cnt_ <= 0 )
		return true;
	assert(3 == door_data->door_cnt_);
	DWORD* door_array = (DWORD*)door_data->door_ptr_;
	for ( auto i = 0; i != _countof(doors_); ++i )
	{
		doors_[i].SetObjAddr((void*)*(door_array + i));
		if ( !doors_[i].Update() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "door obj update error";
			assert(false);
			return false;
		}
	}*/
	the_doors_.SetGameNodes((GDoorsMgr::stGBTreeNodes*)(room_obj_base_ + g_o_RoomMapDoorObj));
	if ( !the_doors_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

GRoomDoor* GRoomBase::GetDoorById( const std::wstring& wrap_gate_id )
{
	auto& the_cont = the_doors_.GetAllNodes();
	if (the_cont.empty())
	{
		assert(false);
		return nullptr;
	}
	assert(!wrap_gate_id.empty());
	for ( auto i = 0;i != the_cont.size(); ++i )
	{
		if ( the_cont[i].GetGateName() == wrap_gate_id )
			return &the_cont[i];
	}
	return nullptr;
}

struct stWarpGateInfo{
	NODE				str_theNode;
	GClass::stGameString		str_theWarp;
	struct
	{
		DWORD			str_dwVFunAddr;
		GAME_STRING_ID	str_theId;
		FLOAT			str_fPositionX;
		FLOAT			str_fPositionY;
		FLOAT			str_fDirectionX;
		FLOAT			str_fDirectionZ;
		FLOAT			str_fDirectionY;
	}theWarpGate;
};
static_assert(sizeof(stWarpGateInfo) == sizeof(SDB_WARP_GATE), "");

bool GRoomDoor::Update()
{
	using namespace GAddr;
	using namespace GOffset;
	using namespace GClass;
	using namespace GType;
	if ( !game_cur_node_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "door obj is empty";
		assert(false);
		return false;
	}
	auto warp_gate = (const stWarpGateInfo*)game_cur_node_;
	if ( !warp_gate->str_theWarp.CopyTo(door_name_) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read door name error";
		assert(false);
		return false;
	}
	/*auto the_pos = (const stDirection2d*)(&warp_gate->theWarpGate.str_fPositionX);
	door_pos_.FromDirection(*the_pos);*/
	stGameVectorCopier* vec_copier = (stGameVectorCopier*)(&warp_gate->theWarpGate.str_fPositionX);
	vec_copier->CopyTo(door_pos_);
	std::swap(door_pos_.y_, door_pos_.z_);
	return true;
}

const std::wstring& GRoomDoor::GetGateName() const
{
	return door_name_;
}

const GType::stGameVector& GRoomDoor::GetGatePos() const
{
	//door_pos_.z_ = gps.GetPos().z_;
	return door_pos_;
}

const void* GRoomDoor::GetDoorObjAddr() const
{
	return game_cur_node_;
}

bool GTownRoom::Update()
{
	using namespace GAddr;
	room_obj_base_ = (int)g_pLocalTownRoomObjAddr;
	return __super::Update();
}

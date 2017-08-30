#include "stdafx.h"
#include "GameObj.h"
#include <Common/MemOper.h>
#include "../FuncObjC.h"
#include "GFuncCall.h"
#include "../GloFuncs.h"
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include "GameObjMgr.h"


void GameObjBase::SetObjType( GameObjType obj_type )
{
	assert(kGOT_Invalid == obj_type_ || obj_type_ == obj_type);
	obj_type_ = obj_type;
}

GameObjType GameObjBase::GetGameObjType() const
{
	return obj_type_;
}

bool GameObjBase::UpdateImpl()
{
	using namespace GClass;
	
	if (!local_obj_info_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "game object addr is empty";
		assert(false);
		return false;
	}

	if ( !UpdatePacketId() )
	{
		assert(false);
		return false;
	}
	if (!UpdateOwnerRoomNum())
		return false;

	dir_pos_info_ = &local_obj_info_->pos_dir_info;
	pos_ptr_ = local_obj_info_->pos_.CastTo<GType::stGameVector3>();
	if (local_obj_)
		obj_status_ = &local_obj_->status_;

	obj_id_ = local_obj_info_->id_;
	name_.assign(CDF_EmptyStr(local_obj_info_->name_));
	return true;
}

GameObjBase::GameObjBase( GameObjType gameobj_type )
{
	obj_type_ =  gameobj_type;
	ResetAllUpdateData();
}

bool GameObjBase_Base::SetCurNode(const stBTreeNodeBase* game_cur_node)
{
	if (!__super::SetCurNode(game_cur_node))
		return false;
	auto local_obj_node = game_cur_node->CastTo<stCD_LocalObjNode>();
	return SetCurLocalObj(local_obj_node->obj_);
}

void GameObjBase::DebugInfo() const
{
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return;
	}
	GType::stGameVector3 pos = GetPos(), dir_lhs = dir_pos_info_->dir_1, dir_rhs = dir_pos_info_->dir_2;
	LOG_O(Log_debug) << "obj id:" << obj_id_ << "\tpos:(" << pos.fx << "," << pos.fy << "," << pos.fz << ")\t" << "dir lhs:("
		<< dir_lhs.fx << "," << dir_lhs.fy << ")\tdir rhs:(" <<
		dir_rhs.fx << "," << dir_rhs.fy
		<<
		")\tname:" << name_ << "  room id:" << owner_room_num_
		<< " packet id:" << packet_id_;
}

bool GameObjBase::SetPos(const stCD_Vector3& pos)
{
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(pos_ptr_))
	{
		assert(false);
		return false;
	}
	*pos_ptr_ = pos;

	dir_pos_info_->fx = pos.fx;
	dir_pos_info_->fy = pos.fy;
	dir_pos_info_->fz = pos.fz;
	return true;
}

const stCD_PosDirInfo* GameObjBase::GetPosInfo() const
{
	return dir_pos_info_;
}

GType::stGameVector3 GameObjBase::GetPos() const
{
	if (IsUnValidPtr(pos_ptr_))
	{
		//assert(false);
		return GType::kVecEmpty;
	}
	/*GType::stGameVector3 pos;
	pos.ConvertToPos(*dir_pos_info_);
	return pos;*/
	return *pos_ptr_;
}

GType::stGameVector3 GameObjBase::GetDirLhs() const
{
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return GType::kVecEmpty;
	}
	GType::stGameVector3 dir_lhs = dir_pos_info_->dir_1;
	assert(dir_lhs.fx >= -5 && dir_lhs.fx <= 5);
	assert(dir_lhs.fy >= -5 && dir_lhs.fy <= 5);
	return dir_lhs;
}

GType::stGameVector3 GameObjBase::GetDirRhs() const
{
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return GType::kVecEmpty;
	}
	GType::stGameVector3 dir_rhs = dir_pos_info_->dir_2;
	assert(dir_rhs.fx >= -5 && dir_rhs.fx <= 5);
	assert(dir_rhs.fy >= -5 && dir_rhs.fy <= 5);
	return dir_rhs;
}

bool GameObjBase::SetDir( const GType::stGameVector3& dir_lhs, const GType::stGameVector3& dir_rhs )
{
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return false;
	}
	assert(dir_lhs.fx >= -1 && dir_lhs.fx <= 1);
	assert(dir_lhs.fy >= -1 && dir_lhs.fy <= 1);
	assert(dir_rhs.fx >= -1 && dir_rhs.fx <= 1);
	assert(dir_rhs.fy >= -1 && dir_rhs.fy <= 1);

	dir_pos_info_->dir_1.fx = dir_lhs.fx;
	dir_pos_info_->dir_1.fy = dir_lhs.fy;
	dir_pos_info_->dir_2.fx = dir_rhs.fx;
	dir_pos_info_->dir_2.fy = dir_rhs.fy;
	return true;
}

bool GameObjBase::SetDirAsync(const stCD_PosDirInfo* pos_info)
{
	if (IsUnValidPtr(pos_info))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return false;
	}
	auto gwnd = FuncObj_NormalC::GetGWnd();
	if ( !gwnd )
	{
		assert(false);
		return false;
	}
	auto dir_pos_info = dir_pos_info_;
	if ( !gwnd->AsyncExec([dir_pos_info, pos_info](){
		dir_pos_info->dir_1.fx = pos_info->dir_1.fx;
		dir_pos_info->dir_1.fy = pos_info->dir_1.fy;
		dir_pos_info->dir_2.fx = pos_info->dir_2.fx;
		dir_pos_info->dir_2.fy = pos_info->dir_2.fy;
	}) )
	{
		assert(false);
		return false;
	}

	return true;
}

/*
bool GameObjBase::SetPosByPosInfoSync( const MMO_ActorVector* pos_info )
{
	if (IsUnValidPtr(pos_info))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return false;
	}
	if ( !GWndExecSync([this, pos_info](){
		GType::stGameVector3 pos;
		pos.ConvertToPos(*pos_info);
		return SetPos(pos);
	}) )
	{
		assert(false);
		return false;
	}

	return true;
}*/

bool GameObjBase::SetDirSync(const stCD_PosDirInfo* pos_info)
{
	if (IsUnValidPtr(pos_info))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return false;
	}
	auto dir_pos_info = dir_pos_info_;
	if ( !GWndExecSync([dir_pos_info, pos_info](){
		dir_pos_info->dir_1.fx = pos_info->dir_1.fx;
		dir_pos_info->dir_1.fy = pos_info->dir_1.fy;
		dir_pos_info->dir_2.fx = pos_info->dir_2.fx;
		dir_pos_info->dir_2.fy = pos_info->dir_2.fy;
		return true;
	}) )
	{
		assert(false);
		return false;
	}

	return true;
}

/*
bool GameObjBase::SetPosSync( const MMO_Direction3& pos )
{
	if (IsUnValidPtr(dir_pos_info_))
	{
		assert(false);
		return false;
	}
	if ( !GWndExecSync([this, &pos](){
		return SetPos(pos);
	}) )
	{
		assert(false);
		return false;
	}

	return true;
}
*/

bool GameObjBase::SetPosEnsure(const stCD_Vector3& pos)
{
	//LOG_O(Log_trace) << "设置坐标:" << GType::stGameVector3(pos);
	return SetPos(pos);
	/*for ( int valid_cnt = 0, idx = 0; idx != 101; ++idx )
	{
		if ( !SetPos(pos) )
			return false;
		if ( FloatEqual(dir_pos_info_->fx, pos.fx) && FloatEqual(dir_pos_info_->fy, pos.fy) && FloatEqual(dir_pos_info_->fz, pos.fz) && pos_ptr_->FloatAllEqual(pos) )
		{
			if ( ++valid_cnt >= 5 )
				return true;
			DummySleep(20);
		}
		else
			valid_cnt = 0;
		
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "设置坐标次数达到上限，还未设置成功";
	return false;*/
}

DWORD GameObjBase::GetPacketId() const
{
	return packet_id_;
}

bool GameObjBase::IsPackageIdValid() const
{
	return kInvalidPacketId != packet_id_;
}

const std::string& GameObjBase::GetName() const
{
	return name_;
}

int GameObjBase::GetOwnerRoomIdx() const
{
	return owner_room_num_;
}

boost::optional<CD_En_ObjStatus> GameObjBase::GetObjStatus() const
{
	boost::optional<CD_En_ObjStatus> res;
	if (obj_status_)
		res = *obj_status_;
	return res;
}

bool GameObjBase::UpdateOwnerRoomNum()
{
	if (net_obj_)
		owner_room_num_ = net_obj_->room_num_;
	else
		owner_room_num_ = kInvalidIntValue;
	return true;
}

void GameObjBase::ResetAllUpdateData()
{
	obj_id_ = 0;
	dir_pos_info_ = nullptr;
	packet_id_ = std::numeric_limits<DWORD>::max();
	owner_room_num_ = -1;
	obj_status_ = nullptr;
	pos_ptr_ = nullptr;

	local_obj_ = nullptr;
	local_obj_info_ = nullptr;
}

bool GameObjBase::SetDirEnsure( const GType::stGameVector3& dir_lhs, const GType::stGameVector3& dir_rhs )
{
	for ( int valid_cnt = 0, idx = 0; idx != 101; ++idx )
	{
		if ( !SetDir(dir_lhs, dir_rhs) )
			return false;
		auto& new_lhs = GetDirLhs();
		auto& new_rhs = GetDirRhs();
		if ( new_lhs.IsXYAllEqual(dir_lhs) && new_rhs.IsXYAllEqual(dir_rhs) )
		{
			if ( ++valid_cnt >= 5 )
				return true;
			DummySleep(20);
		}
		else
			valid_cnt = 0;
		
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "设置坐标次数达到上限，还未设置成功";
	return false;
}

void GameObjBase::DoUpdateClear()
{
	ResetAllUpdateData();
}

bool GameObjBase::UpdatePacketId()
{
	if (!local_obj_info_)
		return false;
	packet_id_ = local_obj_info_->packet_id_;
	return true;
}

DWORD GameObjBase::GetLocalId() const
{
	return obj_id_;
}

GAnyObj::GAnyObj() : GameObjBase(kGOT_AnyObj)
{
}

bool GAnyObj::UpdateImpl()
{
	if (!owner_)
	{
		assert(false);
		return false;
	}
	if ( !__super::UpdateImpl() )
		return false;
	auto mgr = owner_->CastTo<GameObjMgr_AnyObj>();
	if (!mgr->obj_name_.empty())
	{
		if (name_.find(mgr->obj_name_) == std::string::npos)
			return false;
	}
	return UpdateOwnerRoomNum();
}

void GAnyObj::DebugInfo() const
{
	LOG_O(Log_debug) << "任意对象房间号: " << owner_room_num_
		<< "  任意对象名称:" << name_;
}

bool GAnyObj::UpdateOwnerRoomNum()
{
	if (IsUnValidPtr(local_obj_info_))
	{
		assert(false);
		return false;
	}
	auto ptr = local_obj_info_->CastTo<stCD_LocalObjStatic>();
	owner_room_num_ = ptr->room_num_;
	if (!IsUnValidPtr(net_obj_))
	{
		owner_room_num_ = net_obj_->room_num_;
		if (owner_room_num_ < 0)
			return false;
	}
	return true;
}

void GAnyObj::DoUpdateClear()
{
	name_.clear();
	owner_room_num_ = -1;
}

GameObjBase* GNormalGameObjMgrBase::FindByPacketId( DWORD packet_id )
{
	assert(false);
	return nullptr;
}

GameObjBase* GNormalGameObjMgrBase::FindByName( const std::string& name )
{
	assert(false);
	return nullptr;
}

bool GNormalGameObjMgrBase::IsDeadMonsterByPacketId( DWORD packet_id )
{
	assert(false);
	return false;
}

GameObjBase_Base::GameObjBase_Base()
{
	local_obj_ = nullptr;
	local_obj_info_ = nullptr;
	net_obj_ = nullptr;
}

bool GameObjBase_Base::SetCurLocalObj(stCD_LocalObj* local_obj)
{
	if (IsUnValidPtr(local_obj))
		return false;
	/*if (!local_obj->ObjValid())
		return false;*/
	if (!SetCurLocalObjInfo(local_obj->obj_info_))
		return false;
	local_obj_ = local_obj;
	return true;
}

bool GameObjBase_Base::SetCurLocalObjInfo(stCD_ObjBase* local_obj_info)
{
	if (IsUnValidPtr(local_obj_info))
	{
		assert(false);
		return false;
	}
	if (!local_obj_info->ObjValid())
		return false;
	local_obj_info_ = local_obj_info;

	if (!IsUnValidPtr(local_obj_info->net_obj_))
	{
		net_obj_ = local_obj_info->net_obj_;
	}
	else
		net_obj_ = nullptr;
	return true;
}

stCD_ObjBase* GameObjBase_Base::GetObjBase() const
{
	return local_obj_info_;
}

stCD_NetObj* GameObjBase_Base::GetNetObj() const
{
	return net_obj_;
}

GObjDbInfo::GObjDbInfo()
{
	obj_db_info_ = nullptr;
	obj_db_id_ = kInvalidIntValue;
	type_ = CD_En_ObjSubType(-1);
}

bool GObjDbInfo::UpdateImpl()
{
	if (IsUnValidPtr(obj_db_info_))
	{
		assert(false);
		return false;
	}
	obj_db_id_ = obj_db_info_->obj_db_id_;
	ch_name_ = CDF_EmptyStr(obj_db_info_->ch_name_);
	type_ = obj_db_info_->type_;
	en_name_ = CDF_EmptyStr(obj_db_info_->en_name_);
	if (!IsUnValidPtr(obj_db_info_->title_info_))
		title_ = CDF_EmptyStr(obj_db_info_->title_info_->title_);
	return true;
}

void GObjDbInfo::DoUpdateClear()
{
	obj_db_id_ = -1;
}

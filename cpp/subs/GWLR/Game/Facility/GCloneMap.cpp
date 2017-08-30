#include "stdafx.h"
#include "GCloneMap.h"
#include "GConst.h"
#include "GPlayer.h"
#include "GConst.h"
#include "GFuncCall.h"
#include "GameObjMgr.h"
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <CD_CommonFiles/Data/CD_Traverse.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>

GCloneMap::GCloneMap()
{

}

bool GEnterableFbMgr::UpdateAllImpl()
{
	assert(false);
	return false;
}

void GEnterableFbMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "enterable fb total cnt:" << enterable_fb_.size();
	for ( auto fb : enterable_fb_ )
	{
		fb->DebugInfo();
	}
}

const GMapDb* GEnterableFbMgr::FindByName( const std::string& fb_name )
{
	if (fb_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto v : enterable_fb_ )
	{
		if ( v->map_name_ == fb_name )
			return v;
	}
	return nullptr;
}

bool GEnterableFbMgr::RebuildAllImpl()
{
	enterable_fb_.clear();

	auto& db_mgr = MapDbMgr::GetMe();
	if (!db_mgr.Update())
	{
		assert(false);
		return false;
	}
	auto mgr = CD_FuncCall::GetEnterableFbMgr();
	if (!mgr)
	{
		assert(false);
		return false;
	}
	mgr->fb_info_.Traverse([&db_mgr, this](int* fb_id_ptr){
		auto fb_id = *fb_id_ptr;
		auto map_db = db_mgr.FindById(fb_id);
		if (!map_db)
		{
			assert(false);
			return false;
		}
		enterable_fb_.push_back(map_db);
		return true;
	});
	return true;
}

const GMapDb* GEnterableFbMgr::FindById( int fb_id ) const
{
	for ( auto v : enterable_fb_ )
	{
		if ( v->map_id_ == fb_id )
			return v;
	}
	return nullptr;
}

void GEnterableFbMgr::DoRebuildUpdateClear()
{
	enterable_fb_.clear();
}

GMapDb::GMapDb()
{
	map_id_ = 0;
	map_db_ = nullptr;
	map_db_info_id_ = 0;
	role_lvl_ = -1;
	fb_type_ = (CD_En_FbType)-1;
	fb_diff_level_ = (CD_En_FbDifficulty)-1;
}

bool GMapDb::UpdateImpl()
{
	if (IsUnValidPtr(map_db_))
	{
		assert(false);
		return false;
	}
	map_id_ = map_db_->map_id_;
	map_name_.assign(CDF_EmptyStr(map_db_->map_name_));
	map_db_info_id_ = map_db_->map_db_info_id_;
	role_lvl_ = map_db_->role_lvl_;
	assert(role_lvl_ >= 0 );
	fb_type_ = map_db_->type_;
	assert((int)fb_type_ >= 0 && (int)fb_type_ < 100);
	fb_diff_level_ = map_db_->diff_lvl_;
	assert((int)fb_diff_level_ >= 0 && (int)fb_diff_level_ < 100);
	return true;
}

void GMapDb::DebugInfo() const
{
	LOG_O(Log_trace) << "enterable fb, id:" << map_id_ << "  fb name:" << map_name_ << " 角色等级限制:" << role_lvl_;
}

int GMapDb::GetRequireRoleLvl() const
{
	//凡是类型为 8,23,16的，都不判断等级，8和23的表现主线任务的副本，16大图标的副本且需求等级为0的
	if (CD_En_FbType::kFT_8 == fb_type_ || CD_En_FbType::kFT_16 == fb_type_ || CD_En_FbType::kFT_23 == fb_type_)
		return 0;
	return role_lvl_;
}

int GMapDb::GetFbRoleLvlLimit() const
{
	return role_lvl_;
}

void GMapDb::DoUpdateClear()
{
	map_name_.clear();
	map_db_info_id_ = -1;
	role_lvl_ = -1;
}

bool GMapDb::IsKingFb() const
{
	if (CD_En_FbType::kFT_WangLi == fb_type_ || CD_En_FbType::kFT_WangLiCream1 == fb_type_)
	{
#ifdef _DEBUG
		auto& task_fb_info = GClass::GTaskFbInfo::GetMe();
		task_fb_info.Update();
		assert(task_fb_info.FindByFbId_King(map_id_));
#endif
		return true;
	}
	else
	{
/*
#ifdef _DEBUG
		auto& task_fb_info = GClass::GTaskFbInfo::GetMe();
		task_fb_info.Update();
		assert(!task_fb_info.FindByFbId_King(fb_id_));
#endif*/
		return false;
	}
}

bool GMapDb::IsKingCreamFb() const
{
	if (CD_En_FbType::kFT_WangLiCream == fb_type_ || CD_En_FbType::kFT_WangLiCream1 == fb_type_ || CD_En_FbType::kFT_WangLiCream2 == fb_type_)
	{
#ifdef _DEBUG
		auto& task_fb_info = GClass::GTaskFbInfo::GetMe();
		task_fb_info.Update();
		assert(task_fb_info.FindByFbId_King(map_id_));
#endif
		return true;
	}
	else
	{
/*
#ifdef _DEBUG
		auto& task_fb_info = GClass::GTaskFbInfo::GetMe();
		task_fb_info.Update();
		assert(!task_fb_info.FindByFbId_King(fb_id_));
#endif*/
		return false;
	}
}

MapDbMgr::MapDbMgr()
{
}

bool MapDbMgr::Update()
{
	if ( !all_map_.empty() )
		return true;
	return __super::Update();
}

void MapDbMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "all fb info, total cnt:" << all_map_.size();
	for ( auto& fb : all_map_ )
	{
		fb.DebugInfo();
	}
}

const GMapDb* MapDbMgr::FindByName( const std::string& fb_name )
{
	if (fb_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& v : all_map_ )
	{
		if ( v.map_name_ == fb_name )
			return &v;
	}
	return nullptr;
}

const GMapDb* MapDbMgr::FindById( int fb_id )
{
	for ( auto& v : all_map_ )
	{
		if ( v.map_id_ == fb_id )
			return &v;
	}
	return nullptr;
}

bool MapDbMgr::UpdateImpl()
{
	all_map_.clear();
	CD_Traverse::TraverseMapDb([this](stCD_MapDb* fb_db){
		GMapDb tmp;
		tmp.map_db_ = fb_db;
		if (!tmp.Update())
		{
			assert(false);
			return true;
		}
		all_map_.push_back(tmp);
		return true;
	});
	assert(!all_map_.empty());
	return true;
}

void MapDbMgr::DoUpdateClear()
{
	all_map_.clear();
}

void MapDbMgr::DebugInfo_King() const
{
	LOG_ENTER_FUNC;
	for ( auto& fb : all_map_ )
	{
		if (fb.IsKingFb())
			LOG_O(Log_trace) << "副本名：" << fb.map_name_ << " id:" << fb.map_id_ << " 副本类型：" << fb.fb_type_ << " 难度等级：" << fb.fb_diff_level_;
	}
}

void MapDbMgr::DebugInfo_KingCream() const
{
	LOG_ENTER_FUNC;
	for ( auto& fb : all_map_ )
	{
		if (fb.IsKingCreamFb())
			LOG_O(Log_trace) << "副本名：" << fb.map_name_ << " id:" << fb.map_id_ << " 副本类型：" << fb.fb_type_ << " 难度等级：" << fb.fb_diff_level_;
	}
}

void GDoorLinkMgr::AddLink( const stFbDoorLink& door_link )
{
	if ( door_link.template_map_id_ <= 0 )
	{
		assert(false);
		return;
	}
	if ( door_link.room_id_src_ < 0 )
	{
		assert(false);
		return;
	}
	if (door_link.door_name_src_.empty())
	{
		assert(false);
		return;
	}
	if ( door_link.room_id_dst_ < 0 )
	{
		assert(false);
		return;
	}
	auto& room_ids = fb_def_id2room_ids_[door_link.template_map_id_];
	auto& doors_link = room_ids[door_link.room_id_src_];
	for ( auto& v : doors_link )
	{
		if ( v.first == door_link.door_name_src_ )
		{
			assert(false);
			return;
		}
	}
	doors_link.push_back(std::make_pair(door_link.door_name_src_, door_link.room_id_dst_));
}

boost::optional<int> GDoorLinkMgr::FindTheDstRoomId( int template_map_id, int room_id_src, const std::string& door_name_src ) const
{
	boost::optional<int> res;
	if ( template_map_id < 0 )
	{
		assert(false);
		return res;
	}
	if ( room_id_src < 0 )
	{
		assert(false);
		return res;
	}
	if (door_name_src.empty())
	{
		assert(false);
		return res;
	}
	auto it_fb_def_id2room_ids = fb_def_id2room_ids_.find(template_map_id);
	if ( it_fb_def_id2room_ids == fb_def_id2room_ids_.end() )
	{
		//assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有当前副本的地图信息，请联系老师";
		return res;
	}
	auto& room_ids = it_fb_def_id2room_ids->second;

	auto it_room_ids = room_ids.find(room_id_src);
	if ( it_room_ids == room_ids.end() )
	{
		//assert(false);
		return res;
	}
	auto& doors_link = it_room_ids->second;
	for ( auto& v : doors_link )
	{
		if ( v.first == door_name_src )
		{
			res = v.second;
			return res;
		}
	}
	return res;
}

void AddDoorLinkInfo( const stFbDoorLink& door_link ){
	GDoorLinkMgr::GetMe().AddLink(door_link);
}

FbDoor::FbDoor() : GameObjBase(kGOT_Door)
{
}

bool FbDoor::UpdateImpl()
{
	if ( !__super::UpdateImpl() )
	{
		assert(false);
		return false;
	}
	return UpdateOwnerRoomNum();
}

void FbDoor::DebugInfo() const
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
		")\tname:" << name_ << "\tcur room idx:" << owner_room_num_ << "\tthe room id:" << owner_room_num_;
}

bool FbDoor::ObjValid()
{
	if (IsUnValidPtr(local_obj_info_))
	{
		assert(false);
		return false;
	}
	auto ptr = local_obj_info_->CastTo<stCD_LocalObjStatic>();
	if (IsUnValidPtr(ptr->door_link_str_))
		return false;
	return true;
}

void FbDoor::DoUpdateClear()
{

}

bool FbDoor::UpdateOwnerRoomNum()
{
	if (IsUnValidPtr(local_obj_info_))
	{
		assert(false);
		return false;
	}
	auto ptr = local_obj_info_->CastTo<stCD_LocalObjStatic>();
	owner_room_num_ = ptr->room_num_;
	if (owner_room_num_ < 0)
		return false;
	assert(owner_room_num_ >= 0 && owner_room_num_ <= kMaxRoomId);
	return true;
}

FbDoorsMgr::FbDoorsMgr()
{
	AddTypeMap(TypePairT(CD_En_LocalObjType::kDoor1, kGOT_Door));
	AddTypeMap(TypePairT(CD_En_LocalObjType::kDoor2, kGOT_Door));
}

bool FbDoorsMgr::RebuildAll()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateCurMapInfo_Notify();

	return __super::RebuildAll();
}

GCollectObj::GCollectObj() : GameObjBase(kGOT_Collect)
{
	monster_packet_id_ = 0;
}

bool GCollectObj::UpdateImpl()
{
	if ( !__super::UpdateImpl() )
	{
		assert(false);
		return false;
	}
	if (!net_obj_)
	{
		assert(false);
		return false;
	}
	name_.clear();
	auto collect_obj = net_obj_->CastTo<stCD_NetObj_Collect>();
	if (!IsUnValidPtr(collect_obj->collect_name_))
	{
		auto the_name_ptr = *collect_obj->collect_name_;
		if (IsUnValidPtr(the_name_ptr))
			name_.assign(CDF_EmptyStr(collect_obj->name_));
		else
			name_.assign(CDF_EmptyStr(the_name_ptr));
	}
	monster_packet_id_ = collect_obj->monster_packet_id_;

	return UpdateOwnerRoomNum();
}

void GCollectObj::DebugInfo() const
{
	LOG_O(Log_info) << "采集物房间号: " << owner_room_num_
		<< "  采集物名称:" << name_;
}

bool GCollectObj::UpdateOwnerRoomNum()
{
	DO_EXCEPTION_TRY;
	if (!net_obj_)
	{
		assert(false);
		return false;
	}
	auto collect_obj = net_obj_->CastTo<stCD_NetObj_Collect>();
	//怪物尸体时，该字段为空
	if (IsUnValidPtr(collect_obj->room_info_))
	{
		owner_room_num_ = -1;
		return true;
	}
	owner_room_num_ = collect_obj->room_info_->room_num_;
	return true;
	DO_CATCH_ALL_MSG_BOX_CODE(
		owner_room_num_ = -1;
		return false;);
}

void GCollectObj::DoUpdateClear()
{
	name_.clear();
	owner_room_num_ = -1;
	monster_packet_id_ = 0;
}

bool GCollectObj::ObjValid()
{
	if (!net_obj_)
		return false;
	if (!IsUnValidPtr(net_obj_->str_area_))
	{
		if (0 == strcmp(net_obj_->str_area_, "SceneBuffArea"))
			return false;
	}
	return true;
}

GCollectObjMgr::GCollectObjMgr()
{
	AddTypeMap(TypePairT(CD_En_LocalObjType::kCollect, kGOT_Collect));
}

GameObjBase* GCollectObjMgr::FindNearestAsSameRoom( const GameObjBase& obj )
{
	if (btree_.empty())
		return nullptr;

	struct stHelper{
		GCollectObj*		obj_ptr_;
		double				the_dist_;
	};
	
	auto& pos_src = obj.GetPos();
	auto owner_room_idx = obj.GetOwnerRoomIdx();
	if ( owner_room_idx < 0 )
	{
		assert(false);
		return nullptr;
	}
	std::vector<stHelper> dist_cont;
	Traverse([&dist_cont, owner_room_idx, &pos_src](GameObjBase& v){
		auto collect_obj = v.CastTo<GCollectObj>();
		if (!collect_obj)
		{
			assert(false);
			return true;
		}
		if (owner_room_idx != collect_obj->GetOwnerRoomIdx())
			return true;
		auto& obj_pos = v.GetPos();
		stHelper hlp = {
			collect_obj, GameObjMgr::CalcObjDistance(obj_pos, pos_src, 0)
		};
		dist_cont.push_back(hlp);
		return true;
	});
	auto it = std::min_element(dist_cont.begin(), dist_cont.end(), []( const stHelper& lhs, const stHelper& rhs ){
		return lhs.the_dist_ < rhs.the_dist_;
	});
	if ( it != dist_cont.end() )
	{
		return it->obj_ptr_;
	}
	return nullptr;
}

GameObjBase* GCollectObjMgr::FindDeadMonster( const GameObjBase& obj )
{
	return FindDeadMonsterByPacketId(obj.GetPacketId());
}

GameObjBase* GCollectObjMgr::FindDeadMonsterByPacketId( DWORD packet_id )
{
	GameObjBase* res = nullptr;
	Traverse([packet_id, &res](GameObjBase& v){
		auto collect_obj = v.CastTo<GCollectObj>();
		if (!collect_obj)
		{
			assert(false);
			return true;
		}
		if (collect_obj->monster_packet_id_ == packet_id)
		{
			res = collect_obj;
			return false;
		}
		return true;
	});
	return res;
}

GObjStaticMgr::GObjStaticMgr( const std::string& obj_name )
{
	AddTypeMap(TypePairT(CD_En_LocalObjType::kAppear, kGOT_Static));
	SetFilterObjName(obj_name);
}

void GObjStatic::DebugInfo() const
{
	LOG_O(Log_info) << "静止对象房间号: " << owner_room_num_
		<< "  静止对象名称:" << name_;
}

bool GObjStatic::UpdateImpl()
{
	
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

GObjStatic::GObjStatic() : GameObjBase(kGOT_Static)
{

}

void GObjStatic::DoUpdateClear()
{
	name_.clear();
	owner_room_num_ = -1;
}

bool GObjStatic::UpdateOwnerRoomNum()
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

void InitDoorLinkInfo()
{
	stFbDoorLink door_link;
	door_link.template_map_id_ = 0x000003E9;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "RSTrigger_0to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "RSTrigger_1to0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_1to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_1to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "RSTrigger_2to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_2to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_2to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "RSTrigger_3to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_3to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_3to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "RSTrigger_4to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_4to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003EA;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTriggerCto5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTriggerCto1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger1to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1toC";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger2to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger2to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger2to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger3to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger4to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger4to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger5toC";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger5to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger5to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger6to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger6to2";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	/*door_link.door_name_src_ = "SwitchRegionTrigger6to2_drama";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);*/
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003EB;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger22_0-2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1_0-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger3_1-4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1_1-C";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger17_1-5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger2-C";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_2-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger16";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger20";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger6_4-7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger5";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger7_5-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger8_5-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger9_5-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger18_6-5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger13_6-7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SwitchRegionTrigger10_7-4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger11_7-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger21_7-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003EC;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger_0to1A";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_0to6";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger_1to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_1to0A";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_1to2A";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger_2to1B";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_2to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger_3to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_3to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_3to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger_4to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_4to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger_5to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_5to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger_6to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_6to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SwitchRegionTrigger_7to0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_7to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003EE;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger0t1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger1t0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1t2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1t7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger2t3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger2t6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger2t1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger3t6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3t9";
	door_link.room_id_dst_ = 9;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3t2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger4t7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger4t5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger5t8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger5t4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger6t8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger6t3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger6t2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SwitchRegionTrigger7t1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger7t4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger7t9";
	door_link.room_id_dst_ = 9;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 8;
	door_link.door_name_src_ = "SwitchRegionTrigger8t6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger8t5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 9;
	door_link.door_name_src_ = "SwitchRegionTrigger9t3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger9t7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003ED;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "RSTrigger_0to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_0to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "RSTrigger_1to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_1to0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_1to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "RSTrigger_2to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_2to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_2to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "RSTrigger_3to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_3to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_3to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "RSTrigger_4to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_4to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_4to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "RSTrigger_5to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_5to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "RSTrigger_6to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_6to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_6to0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "RSTrigger_7to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_7to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "RSTrigger_7to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x00000579;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003EF;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger_0to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_0to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger_1to0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_1to8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_1to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger_2to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_2to0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger_3to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_3to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger_4to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_4to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_4to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger_5to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_5to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger_6to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_6to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_6to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SwitchRegionTrigger_7to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_7to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_7to8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 8;
	door_link.door_name_src_ = "SwitchRegionTrigger_8to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_8to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003F0;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger0-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger0-5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger1-2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1-0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger2-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger2-4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger3-4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3-5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger4-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger4-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger4-2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger5-7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger5-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger5-0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger6-4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger6-7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger6-8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SwitchRegionTrigger7-5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger7-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger7-8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 8;
	door_link.door_name_src_ = "SwitchRegionTrigger8-7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger8-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003F2;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTriggerC-8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTriggerC-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger1-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1-C";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger1-2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger2-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigge2-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger2-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger3-5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3-2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3-4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger3-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger4-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger5-8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger5-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger5-7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger6-3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger6-7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger6-2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger6-9";
	door_link.room_id_dst_ = 9;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SwitchRegionTrigger7-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger7-5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 8;
	door_link.door_name_src_ = "SwitchRegionTrigger8-9";
	door_link.room_id_dst_ = 9;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger8-5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger8-C";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 9;
	door_link.door_name_src_ = "SwitchRegionTrigger9-6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger9-8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger9-10";
	door_link.room_id_dst_ = 10;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 10;
	door_link.door_name_src_ = "SwitchRegionTrigger10-9";
	door_link.room_id_dst_ = 9;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000003F3;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger_0to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_0to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger_1to0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_1to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger_2to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_2to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger_3to1";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_3to7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_3to6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_3to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger_4to3";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger_5to8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_5to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger_6to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SwitchRegionTrigger_7to2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_7to4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);
	door_link.door_name_src_ = "SwitchRegionTrigger_7to3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 8;
	door_link.door_name_src_ = "SwitchRegionTrigger_8to5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x000004B2;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTriggerC-1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger1-C";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x0000070A;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "Teleport_To_Cat_Area";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "Teleport_To_MainArea";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "Teleport_To_Main_Area";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "Teleport_To_Government_Building";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x3f6;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SRT_0_To_1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SRT_1_To_0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_1_To_2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_1_To_4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_1_To_3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SRT_2_To_1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_2_To_4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_2_To_6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_2_To_5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SRT_3_To_1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_3_To_8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_3_To_4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SRT_4_To_1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_4_To_2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_4_To_3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_4_To_7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SRT_5_To_2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_5_To_6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SRT_6_To_2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_6_To_5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_6_To_7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_6_To_9";
	door_link.room_id_dst_ = 9;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SRT_7_To_4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_7_To_6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_7_To_8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 8;
	door_link.door_name_src_ = "SRT_8_To_3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_8_To_7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_8_To_9";
	door_link.room_id_dst_ = 9;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 9;
	door_link.door_name_src_ = "SRT_9_To_6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SRT_9_To_8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x3f9;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger0_1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "SwitchRegionTrigger1_4";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger1_2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger1_0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 2;
	door_link.door_name_src_ = "SwitchRegionTrigger2_1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger2_3";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger2_7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 3;
	door_link.door_name_src_ = "SwitchRegionTrigger3_5_2";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger3_2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);

	/*door_link.door_name_src_ = "SwitchRegionTrigger3_5_1";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);*/

	door_link.room_id_src_ = 4;
	door_link.door_name_src_ = "SwitchRegionTrigger4_5_2";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);

	/*door_link.door_name_src_ = "SwitchRegionTrigger4_5_1";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);*/

	door_link.door_name_src_ = "SwitchRegionTrigger4_1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 5;
	door_link.door_name_src_ = "SwitchRegionTrigger5_3_1";
	door_link.room_id_dst_ = 3;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger5_4_2";
	door_link.room_id_dst_ = 4;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger5_6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 6;
	door_link.door_name_src_ = "SwitchRegionTrigger6_5";
	door_link.room_id_dst_ = 5;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger6_8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger6_7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 7;
	door_link.door_name_src_ = "SwitchRegionTrigger7_6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger7_8";
	door_link.room_id_dst_ = 8;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger7_2";
	door_link.room_id_dst_ = 2;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 8;
	door_link.door_name_src_ = "SwitchRegionTrigger8_7";
	door_link.room_id_dst_ = 7;
	AddDoorLinkInfo(door_link);

	door_link.door_name_src_ = "SwitchRegionTrigger8_6";
	door_link.room_id_dst_ = 6;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x4B4;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "RegionSwitch_0_to_1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);

	door_link.room_id_src_ = 1;
	door_link.door_name_src_ = "RegionSwitch_1_to_0";
	door_link.room_id_dst_ = 0;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
	door_link.template_map_id_ = 0x57F;
	door_link.room_id_src_ = 0;
	door_link.door_name_src_ = "SwitchRegionTrigger_0to1";
	door_link.room_id_dst_ = 1;
	AddDoorLinkInfo(door_link);
	//////////////////////////////////////////////////////////////////////////
}

GObjMieHuoTong::GObjMieHuoTong()
{
	AddTypeMap(TypePairT(CD_En_LocalObjType::kAppear, kGOT_AnyObj));
	SetFilterObjName("Bucket");
}

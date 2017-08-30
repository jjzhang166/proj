#include "stdafx.h"
#include "GMap.h"
#include "GPlayer.h"
#include "GClass.h"
#include "GProtocolR.h"

GWayPoint::GWayPoint() : GameObjBase(kGOT_Waypoint)
{

}

void GWayPoint::DebugInfo() const
{
	LOG_O(Log_debug) << "传送点信息--------------";
	__super::DebugInfo();
	return;
}

bool GWayPoint::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		assert(false);
		return false;
	}

	auto entity = GetGameEntityBase();
	if (IsUnValidPtr(entity))
	{
		assert(false);
		return false;
	}
	auto area_transiton = entity->CastToWaypoint();
	if (IsUnValidPtr(area_transiton))
	{
		assert(false);
		return false;
	}

	auto rander = area_transiton->GetRender();
	if (IsUnValidPtr(rander))
	{
		assert(false);
		return false;
	}
	if (!CD_SafeName(rander->name_))
	{
		assert(false);
		return false;
	}

	SetName(rander->name_.GetStdString());

	return true;
}

bool GWayPoint::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	return visitor.VisitMainSubPanelNormal(*this, is_pre_open, open_res, g_a_ui_dlg_waypoint_vtbl_);
}

int GWayPoint::GetCanOpenedDistance() const
{
	return kOpenObjValidDistance;
}

bool GWayPoint::CloseObject() const
{
	return GPlayerMe::GetMe().CloseAllPanel(false);
}

bool GWayPoint::MovingDoTryOpen() const
{
	auto& map = GMap::GetMe();
	if (!map.Update())
	{
		assert(false);
		return false;
	}
	auto& way_point_mgr = GWayPointDlgMgr::GetMe();
	if (!way_point_mgr.RebuildAll())
	{
		assert(false);
		return false;
	}
	if (way_point_mgr.IsOpenedWayPoint(map.GetMapName(), GPlayerMe::GetMe().GetDiffLvl()))
		return false;
	return true;
}

GAreaTransition::GAreaTransition() : GameObjBase(kGOT_AreaTransition)
{
	map_difficulty_ = kMD_Invalid;
	is_town_area_ = false;
}

bool GAreaTransition::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		assert(false);
		return false;
	}

	auto entity = GetGameEntityBase();
	if (IsUnValidPtr(entity))
	{
		assert(false);
		return false;
	}
	auto area_transiton = entity->CastToAreaTransition();
	if (IsUnValidPtr(area_transiton))
	{
		assert(false);
		return false;
	}
	if (entity->GetPortal())
		is_town_area_ = true;

	auto rander = area_transiton->GetRender();
	if (IsUnValidPtr(rander))
	{
		assert(false);
		return false;
	}
	if (!CD_SafeName(rander->name_))
	{
		assert(false);
		return false;
	}

	SetName(rander->name_.GetStdString());
	return true;
}

void GAreaTransition::DebugInfo() const
{
	LOG_O(Log_debug) << "传送门信息--------------";
	__super::DebugInfo();
	return;
}

bool GAreaTransition::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	return visitor.VisitAreaTransition(*this, is_pre_open, open_res);
}

void GAreaTransition::RememberLastMapInfo()
{
	GMap::GetMe().BeginChangeMap(last_map_info_);
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return;
	}
	map_difficulty_ = gpm.GetDiffLvl();
}

const GClass::stLastMapInfo& GAreaTransition::GetLastMapInfo() const
{
	return last_map_info_;
}

enCD_kMapDifficulty GAreaTransition::GetLastDifficulty() const
{
	return map_difficulty_;
}

int GAreaTransition::GetCanOpenedDistance() const
{
	return kOpenObjValidDistance;
}

bool GAreaTransition::MovingDoTryOpen() const
{
	return true;
}

bool GAreaTransition::IsTransitonable() const
{
	return true;
}

bool GAreaTransition::IsTownTranstionable() const
{
	return is_town_area_;
}

bool GMap::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		assert(false);
		return false;
	}
	if (!game_world_ || !game_world_area_)
	{
		assert(false);
		return false;
	}
	map_name_ = WideCharToGbk(CD_SafeName(game_world_area_->ch_map_name_));
	if (map_name_.empty())
	{
		assert(false);
		return false;
	}
	map_size_ = game_world_->map_data_.GetMapSize();
	chapter_ = game_world_area_->chapter_;
	if (chapter_ < 1 || chapter_ > 1000)
	{
		assert(false);
		return false;
	}
	return true;
}

GMap::GMap() : memory_maps_(5)
{
	chapter_ = 0;
	is_changed_map_ = false;
}

bool GMap::LastEqual() const
{
	return last_map_info_.last_world_ == game_world_ 
		&& last_map_info_.last_world_area_ == game_world_area_ 
		&& last_map_info_.last_map_size_ == map_size_
		&& last_map_info_.last_map_name_ == map_name_;
}

bool GMap::RememberLastInfo()
{
	return RememberLastInfo(last_map_info_);
}

bool GMap::RememberLastInfo(stLastMapInfo& last_map_info) const
{
	last_map_info.last_world_ = game_world_;
	last_map_info.last_world_area_ = game_world_area_;
	last_map_info.last_map_size_ = map_size_;
	last_map_info.last_map_name_ = map_name_;
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	last_map_info.last_area_id_ = G_AStar::GetMe().GetAreaId(*player_pos);
	return true;
}

const stCD_VecInt& GMap::GetMapSize() const
{
	return map_size_;
}

bool GMap::Update()
{
	return true;
	/*if (!__super::Update())
	{
		auto& gpm = GPlayerMe::GetMe();
		gpm.ClearRememberedObjs();
		ForgetLastInfo();
		return false;
	}*/
}

void GMap::ForgetLastInfo()
{
	last_map_info_.clear();
}

const std::string& GMap::GetMapName() const
{
	return map_name_;
}

void GMap::EnableAvoidSkill(bool enable)
{
	auto& astar = G_AStar::GetMe();
	if (enable)
	{
		if (!Update())
		{
			assert(false);
			astar.EnableAvoidPos(false);
			return;
		}
		astar.EnableAvoidPos(true);
	}
	else
		astar.EnableAvoidPos(false);
}

bool GMap::IsEnableAvoidSkill() const
{
	return G_AStar::GetMe().IsEnabledAvoidPos();
}

void GMap::ParseCurMemoryMapInfo()
{
	area_resolver_.clear();
	auto& cur_memory = GetFreeMemoryMapInfo(map_name_);
	if (cur_memory.map_name_.empty())
		return;
	assert(cur_memory.map_name_ == map_name_);
	auto& astar = G_AStar::GetMe();
	if (cur_memory.astar_data_ != astar)
		return;
	area_resolver_ = cur_memory.area_resolver_;
	GPlayerMe::GetMe().RememberObjs(cur_memory.obj_mgr_);
}

GMap::stMemoryMapInfo& GMap::GetFreeMemoryMapInfo(const std::string& map_name)
{
	assert(!map_name.empty());
	stMemoryMapInfo* free_memory = nullptr;
	for (auto& v : memory_maps_)
	{
		if (!free_memory && v.map_name_.empty())
			free_memory = &v;
		else if (v.map_name_ == map_name)
			return v;
	}
	if (free_memory)
		return *free_memory;
	if (memory_maps_.empty())
	{
		assert(false);
		return *free_memory;
	}
	free_memory = &memory_maps_.front();
	free_memory->map_name_.clear();
	free_memory->obj_mgr_.clear();
	free_memory->area_resolver_.clear();
	return *free_memory;
}

void GMap::RememberLastMapObjInfo()
{
	if (last_map_info_.last_map_name_.empty())
		return;
	auto& memory_objs = GPlayerMe::GetMe().GetRememberedObjs();
	if (memory_objs.empty())
		return;
	auto& map_info = GetFreeMemoryMapInfo(last_map_info_.last_map_name_);
	auto& astar = G_AStar::GetMe();
	if (map_info.map_name_.empty())
	{
		map_info.map_name_ = last_map_info_.last_map_name_;
		map_info.obj_mgr_ = memory_objs;
		map_info.astar_data_ = astar;
		map_info.area_resolver_ = area_resolver_;
		return;
	}
	assert(map_info.map_name_ == last_map_info_.last_map_name_);
	if (map_info.astar_data_ == astar)
	{
		map_info.obj_mgr_ += memory_objs;
		map_info.area_resolver_ += area_resolver_;
		return;
	}
	map_info.obj_mgr_ = memory_objs;
	map_info.astar_data_ = astar;
	map_info.area_resolver_ = area_resolver_;
}

void GMap::DoUpdated()
{
	is_changed_map_ = true;
	if (LastEqual())
	{
		assert(false);
		return;
	}
	RememberLastMapObjInfo();
	auto& gpm = GPlayerMe::GetMe();
	gpm.ClearRememberedObjs();
	ForgetLastInfo();
	if (!G_AStar::GetMe().Init(new_astar_))
	{
		assert(false);
		return;
	}
	//LOG_O(Log_debug) << "开始初始化path mark";
	if (!GPathMark::GetMe().Init())
	{
		assert(false);
		return;
	}
	//LOG_O(Log_debug) << "path mark初始化完成";

	if (!RememberLastInfo())
	{
		assert(false);
		return;
	}
	if (map_name_.empty())
	{
		assert(false);
		return;
	}
	ParseCurMemoryMapInfo();
}

bool GMap::InitNewAstar(stCD_World& new_world)
{
	if (!new_world.world_area_.IsValid())
	{
		assert(false);
		return false;
	}
	auto tbl_world_area = new_world.world_area_.tbl_item_;
	auto new_map_name = WideCharToGbk(CD_SafeName(tbl_world_area->ch_map_name_));
	if (new_map_name.empty())
	{
		assert(false);
		return false;
	}
	new_astar_.SetSideSize(G_AStar::GetMe().GetMapSideSize(new_map_name));
	stMapDataType map_data(new_world.map_data_, new_world.map_data_.GetMapSize());
	//LOG_O(Log_debug) << "开始初始化A星";
	auto res = new_astar_.Init(map_data);
	LOG_O(Log_debug) << "初始化A星的结果：" << res;
	return res;
}

bool GMap::UpdateCurMap_()
{
	if (!game_world_)
	{
		assert(false);
		return false;
	}
	if (!InitNewAstar(*game_world_))
	{
		assert(false);
		return false;
	}
	last_map_info_.clear();
	DoUpdated();
	return true;
}

void GMap::SetMapAreaInfo(const std::string& map_name, const luabind::object& areas)
{
	using namespace luabind;
	if (map_name.empty())
	{
		assert(false);
		return;
	}
	std::vector<std::string> area_names;
	auto lua_t = type(areas);
	if (LUA_TTABLE == lua_t)
	{
		for (auto& v : areas)
		{
			const auto& the_area = LuaObjCast(v, kEmptyStr);
			if (the_area.empty())
			{
				assert(false);
				return;
			}
			area_names.push_back(the_area);
		}
	}
	else if (LUA_TSTRING == lua_t)
	{
		const auto& the_area = LuaObjCast(areas, kEmptyStr);
		if (the_area.empty())
		{
			assert(false);
			return;
		}
		area_names.push_back(the_area);
	}
	else
	{
		assert(false);
		return;
	}
	if (area_names.empty())
	{
		assert(false);
		return;
	}
	map_area_infos_[map_name] = { area_names };
}

void GMap::ClearAllMarkInfo()
{
	GPathMark::GetMe().ClearMarkPos();
}

void GMap::AddAreaConnection(const stAreaVertex& vertex_lhs, const stAreaVertex& vertex_rhs)
{
	if (!area_resolver_.Exist(vertex_lhs))
	{
		if (vertex_lhs.transitionable_)
		{
			vertex_lhs.transitionable_->SetOwner(nullptr);
			vertex_lhs.transitionable_->ResetPos();
		}
		area_resolver_.AddVertex(vertex_lhs);
	}
	if (!area_resolver_.Exist(vertex_rhs))
	{
		if (vertex_rhs.transitionable_)
		{
			vertex_rhs.transitionable_->SetOwner(nullptr);
			vertex_rhs.transitionable_->ResetPos();
		}
		area_resolver_.AddVertex(vertex_rhs);
	}
	if (!area_resolver_.Exist(vertex_lhs, vertex_rhs))
	{
		if (vertex_lhs.transitionable_)
		{
			vertex_lhs.transitionable_->SetOwner(nullptr);
			vertex_lhs.transitionable_->ResetPos();
		}
		if (vertex_rhs.transitionable_)
		{
			vertex_rhs.transitionable_->SetOwner(nullptr);
			vertex_rhs.transitionable_->ResetPos();
		}
		area_resolver_.AddEdge(vertex_lhs, vertex_rhs);
	}
}

bool GMap::GenAreaSteps(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, AreaStepsType& steps) const
{
	steps.clear();
	auto& astar = G_AStar::GetMe();
	auto src_area_id = astar.GetAreaId(pos_src);
	if (src_area_id < 0)
	{
		assert(false);
		return false;
	}
	auto dst_area_id = astar.GetAreaId(pos_dst);
	if (dst_area_id < 0)
	{
		assert(false);
		return false;
	}
	if (src_area_id == dst_area_id)
		return true;
	auto src_vertex = FindNearestAreaVertex(pos_src);
	if (!src_vertex.IsValid())
		return false;
	auto dst_vertex = FindNearestAreaVertex(pos_dst);
	if (!dst_vertex.IsValid())
		return false;
	if (!area_resolver_.Resolve(src_vertex, dst_vertex, steps))
	{
		//assert(false);
		return false;
	}
	if (steps.empty())
	{
		assert(false);
		return false;
	}
	steps.pop_back();
	return true;
}

const GMap::stMapAreaInfo* GMap::GetCurAreaInfo() const
{
	auto it = map_area_infos_.find(map_name_);
	if (it == map_area_infos_.end())
		return nullptr;
	return &it->second;
}

bool GMap::GenSortedAreaSteps(const GameObjBasePtrT& obj, AreaStepsType& steps) const
{
	steps.clear();
	if (!obj)
	{
		assert(false);
		return false;
	}
	stAreaVertex vertex;
	vertex.transitionable_ = obj;
	if (!area_resolver_.Exist(vertex))
	{
		steps.push_back(vertex);
		return true;
	}
	AreaStepsType steps_tmp;
	area_resolver_.Resolve(vertex, steps_tmp);
	if (steps_tmp.empty())
	{
		assert(false);
		return false;
	}
	auto& path_mark = GPathMark::GetMe();
	stCD_VecInt pos_tmp;
	stAreaVertex vertex_dst;
	for (auto& step : steps_tmp)
	{
		if (!path_mark.GetSuggestPos(step.GetValidPos(), pos_tmp))
			continue;
		vertex_dst = step;
		break;
	}
	if (!vertex_dst.IsValid())
		return false;
	if (vertex == vertex_dst)
	{
		steps.push_back(vertex);
		return true;
	}
	area_resolver_.Resolve(vertex, vertex_dst, steps);
	if (steps.empty())
	{
		assert(false);
		return false;
	}
	steps.pop_back();
	return true;
}

stAreaVertex GMap::FindNearestAreaVertex(const stCD_VecInt& pos) const
{
	auto& astar = G_AStar::GetMe();
	auto area_id = astar.GetAreaId(pos);
	stAreaVertex vertex;
	if (area_id < 0)
	{
		assert(false);
		return vertex;
	}
	GMapAreaShortestPathsCont<stAreaVertex> vertices;
	area_resolver_.TraVertices([&astar, area_id, &vertices](const stAreaVertex& vertex){
		if (area_id == astar.GetAreaId(vertex.GetValidPos()))
			vertices.push_back(vertex);
		return true;
	});
	if (vertices.empty())
		return vertex;
	auto pos_tmp = pos;
	//排序的Distance，必须要保证何时调用都是一样的
	std::sort(vertices.begin(), vertices.end(), [pos_tmp](const stAreaVertex& lhs, const stAreaVertex& rhs){
		return lhs.GetValidPos().Distance(pos_tmp) < rhs.GetValidPos().Distance(pos_tmp);
	});
	return vertices.front();
}

stAreaVertex GMap::FindAreaByName(const std::string& area_name) const
{
	auto res = area_resolver_.FindVertex([&area_name](const stAreaVertex& vertex){
		if (!vertex.transitionable_) return false;
		return vertex.transitionable_->GetCnName() == area_name;
	});
	if (!res)
		return stAreaVertex();
	return *res;
}

stAreaVertex GMap::FindConnectedRoom(const stAreaVertex& vertex) const
{
	auto res = area_resolver_.FindConnectedVertex(vertex);
	if (!res)
		return stAreaVertex();
	return *res;
}

bool GMap::BeginChangeMap(stLastMapInfo& last_map_info)
{
	is_changed_map_ = false;
	return RememberLastInfo(last_map_info);
}

bool GMap::BeginChangeMap()
{
	is_changed_map_ = false;
	return true;
}

bool GMap::HasChangedMap(const stLastMapInfo& last_map_info) const
{
	stLastMapInfo map_info;
	if (!RememberLastInfo(map_info))
	{
		assert(false);
		return false;
	}
	if (map_info == last_map_info)
		return false;
	if (map_info.last_map_name_ == last_map_info.last_map_name_)
		return true;
	return is_changed_map_;
}

bool GMap::HasChangedMap() const
{
	return is_changed_map_;
}

stMapDataType::stMapDataType(stCD_MapData& map_data, const stCD_VecInt& map_size) 
	: map_data_(map_data), map_size_(map_size)
{
	fn_ = stCD_MapData::GetFn_GetDstMapData();
}

G_AStar& G_AStar::GetMe()
{
	static G_AStar* astar = new G_AStar;
	assert(astar);
	return *astar;
}

bool G_AStar::GenRaySteps(const stCD_VecInt& pos_dst, PosContT& out_pos_info)
{
	using namespace CD_Interface;
	auto& gpm = GPlayerMe::GetMe();
	auto my_pos = gpm.GetPos();
	if (!my_pos)
	{
		assert(false);
		return false;
	}
	auto& map = GMap::GetMe();
	auto map_d = map.GetMapData();
	if (!map_d)
	{
		assert(false);
		return false;
	}
	auto pos_src = *my_pos;
	return __super::GenRaySteps(pos_src, pos_dst, out_pos_info);
}

bool G_AStar::GetAvoidPos(const stCD_VecInt& center, int half_width, int half_height, float center_distance,
	const stCD_VecInt* dst_pos,
	const stCD_VecInt& pos_from, const stCD_VecInt& pos_to, stCD_VecInt& out_res) const
{
	return RectangleSafePosByShoot(center, half_width, half_height, center_distance, dst_pos, pos_from, pos_to, out_res);
}

static void TimerSendAvoid(pt_dword timeout, const stCD_VecInt& avoid_pos){
	stCDS_UseSkillEnd skill_end;
	if (!GInterface::Send(skill_end))
	{
		assert(false);
		return;
	}
	auto& recv_mgr = GRecvMsgHandler::GetMe();
	stCDS_UseSkill skill_msg;
	skill_msg.dst_pos_ = avoid_pos;
	skill_msg.skill_id_ = CD_Interface::GetSkillIdMove();
	recv_mgr.BeginMoveSkillOper(skill_msg.skill_id_);

	if (!TimerDo(100, timeout, [&avoid_pos, &skill_end, &skill_msg, &recv_mgr](){
		GWndExecSync([&](){
			//CD_Interface::PlayerMove(avoid_pos);
			if (!GInterface::Send(skill_msg))
			{
				assert(false);
				return false;
			}
			if (!GInterface::Send(skill_end))
			{
				assert(false);
				return false;
			}
			return true;
		});

		auto move_res = recv_mgr.GetMoveSkillRes();
		if (kUSR_Error != move_res)
		{
			//LOG_O(Log_debug) << "技能躲避成功";
			return true;
		}
		return false;
	}))
	{
		//LOG_O(Log_debug) << "技能躲避超时失败";
	}
}
bool G_AStar::MoveTo(const stCD_VecInt& pos, pt_dword entity_id, pt_word skill_id) const
{
	assert(!"躲避技能功能已废弃");
#ifdef _DEBUG
	auto world = CD_Interface::GetWorld();
	if (!world)
	{
		assert(false);
		return false;
	}
	const auto& objs = world->obj_mgr_.objs_;
	auto obj_it = objs.find(entity_id);
	if (obj_it == objs.end())
	{
		assert(false);
		return false;
	}
	auto the_obj = obj_it->second;
	if (IsUnValidPtr(the_obj))
	{
		assert(false);
		return false;
	}
	auto the_obj_render = the_obj->GetRender();
	if (!the_obj_render)
	{
		assert(false);
		return false;
	}
	auto the_obj_actor = the_obj->GetActor();
	if (!the_obj_actor)
	{
		assert(false);
		return false;
	}
	auto skill_info = the_obj_actor->GetMySkillMgr().FindSkill(skill_id);
	if (!skill_info)
	{
		assert(false);
		return false;
	}
	pt_cwsz skill_name = L"";
	auto active_skill = skill_info->GetActiveSkillDb();
	if (active_skill)
		skill_name = active_skill->ch_name_;
	auto monster_name = the_obj_render->name_.GetStdString();	
#endif

	GAvoidSkill::GetMe().AddSuggestPos(pos);
	//stCD_VecInt avoid_pos = pos;
	GetIoService().post([
		//avoid_pos
#ifdef _DEBUG
		skill_name, monster_name, skill_id
#endif
	](){
		static bool is_throwing = false;
		if (is_throwing) return;
		stCD_VecInt avoid_pos;
		if (!GAvoidSkill::GetMe().GetOptimalPos(avoid_pos))
			return;
		auto deep = GPlayerMe::GetMe().GetCurAvoidSkillDeep();
#ifdef _DEBUG
		/*LOG_O(Log_debug) << "移动间躲避技能，怪物：" << monster_name << "，技能名：" << CD_SafeName(skill_name)
			<< ",SkillId:" << (pt_pcvoid)skill_id << ",当前deep：" << deep;*/
#endif
		//延迟不能随便乱调的
		TimerSendAvoid(250, avoid_pos);
		if (deep > 0)
		{
			assert(!is_throwing);
			is_throwing = true;
			BOOST_SCOPE_EXIT_ALL(&){
				is_throwing = false;
			};
			//DummySleep(200);
			throw GExcept_AvoidSkill();
		}		
	});
	return true;
}

void G_AStar::EnableAvoidPos(bool enable) const
{
	if (enable)
	{
		CD_Interface::Hooks::SetAvoidDamageParam(this);
	}
	else
		CD_Interface::Hooks::SetAvoidDamageParam(nullptr);
}

bool G_AStar::IsEnabledAvoidPos() const
{
	return CD_Interface::Hooks::GetAvoidDamangeCalculator() == this;
}

bool G_AStar::CanShooted(const stCD_VecInt& center, float radius,
	const stCD_VecInt& pos1, const stCD_VecInt& pos2) const
{
	return CanShootToCircle(center, radius, pos1, pos2);
}

const stCD_VecInt* G_AStar::GetLovedDstPos() const
{
	static stCD_VecInt loved_pos;
	loved_pos = GPlayerMe::GetMe().GetCurMovingDstPos();
	if (loved_pos.x == 0 || loved_pos.y == 0)
		return nullptr;
	return &loved_pos;
}

void G_AStar::DebugPosInfo1(bool dbg_data, const std::string& file_name) const
{
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		if (!gpm.Update())
		{
			assert(false);
			return;
		}
		player_pos = gpm.GetPos();
		if (!player_pos)
		{
			assert(false);
			return;
		}
	}
	return DebugPosInfo(dbg_data, file_name, *player_pos);
}

void G_AStar::DebugPosRectInfo1(const std::string& file_name, int range, bool is_src_map_data) const
{
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		if (!gpm.Update())
		{
			assert(false);
			return;
		}
		player_pos = gpm.GetPos();
		if (!player_pos)
		{
			assert(false);
			return;
		}
	}
	auto& map = GMap::GetMe();
	auto map_d = map.GetMapData();
	if (!map_d)
	{
		assert(false);
		return;
	}
	stMapDataType map_data(*map_d, map.GetMapSize());
	stMapDataType::stDbgSrcMapData wrapper(map_data, is_src_map_data);
	return DebugPosRectInfo(wrapper, file_name, *player_pos, range);
}

void G_AStar::DebugRectDataInfo1(const std::string& file_name, int range, bool is_src_map_data) const
{
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		if (!gpm.Update())
		{
			assert(false);
			return;
		}
		player_pos = gpm.GetPos();
		if (!player_pos)
		{
			assert(false);
			return;
		}
	}
	auto& map = GMap::GetMe();
	auto map_d = map.GetMapData();
	if (!map_d)
	{
		assert(false);
		return;
	}
	stMapDataType map_data(*map_d, map.GetMapSize());
	stMapDataType::stDbgGetMapData wrapper(map_data, is_src_map_data);
	return DebugRectDataInfo(wrapper, file_name, *player_pos, range);
}

void G_AStar::DebugRectPosDataInfo1(const std::string& file_name, int range, bool is_src_map_data) const
{
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		if (!gpm.Update())
		{
			assert(false);
			return;
		}
		player_pos = gpm.GetPos();
		if (!player_pos)
		{
			assert(false);
			return;
		}
	}
	auto& map = GMap::GetMe();
	auto map_d = map.GetMapData();
	if (!map_d)
	{
		assert(false);
		return;
	}
	stMapDataType map_data(*map_d, map.GetMapSize());
	stMapDataType::stDbgGetMapData wrapper(map_data, is_src_map_data);
	return DebugRectPosDataInfo(wrapper, file_name, *player_pos, range);
}

void G_AStar::SetMapZoomRate(const std::string& map_name, int zoom_rate)
{
	if (map_name.empty())
	{
		assert(false);
		return;
	}
	map_zoom_rate_[map_name] = zoom_rate;
}

int G_AStar::GetMapSideSize(const std::string& map_name) const
{
	auto it = map_zoom_rate_.find(map_name);
	if (it != map_zoom_rate_.end())
		return it->second;
	return kSideSize;
}

bool G_AStar::CanLineMoveTo(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst)
{
	auto& map = GMap::GetMe();
	auto map_d = map.GetMapData();
	if (!map_d)
	{
		assert(false);
		return false;
	}
	stMapDataType map_data(*map_d, map.GetMapSize());
	stCD_VecInt pos_src_my, pos_dst_my;
	CastToMyPos(pos_src, pos_src_my);
	CastToMyPos(pos_dst, pos_dst_my);
	if (pos_src_my == pos_dst_my) return map_data.CanMove(pos_src) && map_data.CanMove(pos_dst);
	const auto& form = MakeLineGeneralForm(pos_src_my, pos_dst_my);
	int cursor = 1;
	vector_type pos_tmp;
	while (form.GetPosByStep(1, cursor, pos_tmp))
	{
		CastToGamePos(pos_tmp, pos_tmp);
		if (!map_data.CanMove(pos_tmp))
			return false;
	}
	return true;
}

GPathMark& GPathMark::GetMe()
{
	static GPathMark path_mark(G_AStar::GetMe());
	return path_mark;
}

GPathMark::GPathMark(const G_AStar& astar) : CmnPathMark(astar)
{

}

void GPathMark::DebugPosInfo1(bool dbg_data, const std::string& file_name) const
{
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		if (!gpm.Update())
		{
			assert(false);
			return;
		}
		player_pos = gpm.GetPos();
		if (!player_pos)
		{
			assert(false);
			return;
		}
	}
	return DebugPosInfo(dbg_data, file_name, *player_pos);
}

GNoHuman::GNoHuman(GameObjType obj_type) : GameObjBase(obj_type)
{
	no_humain_ = nullptr;
}

bool GNoHuman::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		assert(false);
		return false;
	}
	auto entity = GetGameEntityBase();
	if (!entity)
	{
		assert(false);
		return false;
	}
	no_humain_ = entity->CastTo<stCD_EntityNoHuman>();
	assert(no_humain_);

	auto rander = no_humain_->GetRender();
	if (IsUnValidPtr(rander))
	{
		assert(false);
		return false;
	}
	if (!CD_SafeName(rander->name_))
	{
		assert(false);
		return false;
	}

	SetName(rander->name_.GetStdString());
	return true;
}

GMapBase::GMapBase()
{
	game_world_ = nullptr;
	is_in_town_ = false;
	game_world_area_ = nullptr;
	game_map_data_ = nullptr;
}

bool GMapBase::UpdateImpl()
{
	game_world_ = CD_Interface::GetWorld();
	if (!game_world_)
	{
		assert(false);
		return false;
	}
	if (!game_world_->world_area_.IsValid())
	{
		assert(false);
		return false;
	}
	game_map_data_ = &game_world_->map_data_;
	game_world_area_ = game_world_->world_area_.tbl_item_;
	is_in_town_ = game_world_area_->is_town_;
	return true;
}

bool GSP_Resolver::RebuildAllImpl()
{
	/*stVertex vertex_tmp, vertex_dst;
	vertex_tmp.name_ = "狮眼守望";
	vertex_tmp.chapter_ = 1;
	vertex_tmp.diff_lvl_ = kMD_Normal;
	vertex_tmp.is_town_ = true;

	resolver_.AddVertex(vertex_tmp);
	vertex_dst = vertex_tmp;
	vertex_dst.is_town_ = false;
	vertex_dst.name_ = "暮光海滩";
	resolver_.AddVertex(vertex_dst);
	resolver_.AddEdge(vertex_tmp, vertex_dst);

	vertex_tmp.name_ = "炙热盐沼";
	vertex_tmp.is_town_ = false;
	resolver_.AddVertex(vertex_tmp);
	resolver_.AddEdge(vertex_dst, vertex_tmp);

	vertex_dst.name_ = "海潮地穴";
	resolver_.AddVertex(vertex_dst);
	resolver_.AddEdge(vertex_tmp, vertex_dst);

	vertex_tmp.name_ = "水声之渊";
	resolver_.AddVertex(vertex_tmp);
	resolver_.AddEdge(vertex_dst, vertex_tmp);
	return true;*/
	auto db_map = CD_Interface::GetDbMgr().GetDbWorldArea();
	if (!db_map)
	{
		assert(false);
		return false;
	}
	auto tab = db_map->GetDbTbl();
	if (IsUnValidPtr(tab))
	{
		assert(false);
		return false;
	}

	if (!tab->tbl_items_.IsValid())
	{
		assert(false);
		return false;
	}
	stVertex vertex_src;
	for (auto& world_area : tab->tbl_items_)
	{
		vertex_src.name_ = WideCharToGbk(CD_SafeName(world_area.ch_map_name_));
		vertex_src.chapter_ = world_area.chapter_;
		vertex_src.is_town_ = world_area.is_town_;
		vertex_src.diff_lvl_ = world_area.GetDiffLvl();
		if (vertex_src.name_.empty())
		{
			//LOG_O(Log_debug) << "更新所遇地图的waypoint，有一个是空的！" <<str_src.name_<<":"<<str_src.is_town_<<":"<<str_src.chapter_;
			continue;
		}
		if (!world_area.next_area_.IsValid())
			continue;
		if (!resolver_.Exist(vertex_src))
			resolver_.AddVertex(vertex_src);
	}
	stVertex vertex_dst;
	for (auto& world_area : tab->tbl_items_)
	{
		vertex_src.name_ = WideCharToGbk(CD_SafeName(world_area.ch_map_name_));
		vertex_src.chapter_ = world_area.chapter_;
		vertex_src.is_town_ = world_area.is_town_;
		vertex_src.diff_lvl_ = world_area.GetDiffLvl();
		if (vertex_src.name_.empty())
			continue;
		if (!world_area.next_area_.IsValid())
			continue;
		for (auto& v : world_area.next_area_)
		{
			if (IsUnValidPtr(v))
				continue;
			vertex_dst.chapter_ = v->chapter_;
			vertex_dst.is_town_ = v->is_town_;
			vertex_dst.name_ = WideCharToGbk(CD_SafeName(v->ch_map_name_));
			vertex_dst.diff_lvl_ = v->GetDiffLvl();
			if (vertex_dst.name_.empty())
			{
				assert(false);
				continue;
			}
			if (vertex_src == vertex_dst)
				continue;
			if (!resolver_.Exist(vertex_src, vertex_dst))
				resolver_.AddEdge(vertex_src, vertex_dst);
		}
	}
	return true;
}

bool GSP_Resolver::GetCurMapShortestWaypoint(const std::string& cur_name, enCD_kMapDifficulty diff_lvl,
	MapNameCont& que_ret) const
{ 
	stVertex vertex_from;
	vertex_from.name_ = cur_name;
	vertex_from.diff_lvl_ = diff_lvl;
	StepsContType steps;
	if (!resolver_.Resolve(vertex_from, steps))
	{
		assert(false);
		return false;
	}
	for (auto& step : steps)
		que_ret.push_back(step.name_);
	return true;
}

bool GSP_Resolver::DijkFindPath(const std::string& str_start, const std::string& str_end,
	enCD_kMapDifficulty diff_lvl, MapNameCont& path_list) const
{
	if (str_start == str_end)
	{
		path_list.push_back(str_start);
		return true;
	}
	stVertex vertex_from, vertex_dst;
	vertex_from.name_ = str_start;
	vertex_from.diff_lvl_ = diff_lvl;
	vertex_dst.name_ = str_end;
	vertex_dst.diff_lvl_ = diff_lvl;
	StepsContType steps;
	if (!resolver_.Resolve(vertex_from, vertex_dst, steps))
	{
		assert(false);
		return false;
	}
	for (auto& step : steps)
		path_list.push_back(step.name_);
	return true;
}

void GSP_Resolver::DebugInfo() const
{

}

GSP_Resolver::GSP_Resolver()
{
	//map_name = get(boost::vertex_name, graph_);
}

bool GSP_Resolver::GetTargetMapShortestWayPoint(const std::string& dst_map_name,enCD_kMapDifficulty diff_lvl,
	MapNameCont& sorted_map) const
{
	if (dst_map_name.empty())
		return false;

	auto& way = GWayPointDlgMgr::GetMe();
	auto& cur_map = GMap::GetMe();
	if (!GetCurMapShortestWaypoint(dst_map_name, diff_lvl, sorted_map))
		return false;

	if (sorted_map.empty())
		return false;
	size_t i = 0;
	for (; i < sorted_map.size(); ++i)
	{
		if (!way.TargetMapCanWayPoint(sorted_map[i], diff_lvl))
			continue;
		break;
	}
	sorted_map.erase(sorted_map.begin() + i + 1, sorted_map.end());
	return true;
}

bool GSP_Resolver::IsTownByMapName(const std::string& map_name, enCD_kMapDifficulty diff_lvl) const
{
	if (map_name.empty())
		return false;
	stVertex vertex;
	vertex.name_ = map_name;
	vertex.diff_lvl_ = diff_lvl;
	auto it = resolver_.FindVertex(vertex);
	if (!it)
	{
		LOG_O(Log_debug) << "是否村庄之没有该地图：" << map_name;
		return false;
	}
	return it->is_town_;
}

const std::string& GSP_Resolver::GetCurChapterTownName(int chapter) const
{
	auto vertex = resolver_.FindVertex([chapter](const stVertex& vertex){
		return chapter == vertex.chapter_ && vertex.is_town_;
	});
	if (vertex)
		return vertex->name_;
	return kEmptyStr;
}
bool GSP_Resolver::GetDijkLatelyPathCurDiff(const std::string& dst_map_name, DequeHelp& path_help) const
{
	auto& way = GWayPointDlgMgr::GetMe();
	if (!way.RebuildAll())
	{
		assert(false);
		return false;
	}
	auto& cur_map = GMap::GetMe();
	if (!cur_map.Update())
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	auto cur_diff = gpm.GetDiffLvl();
	MapNameCont tmp_path;
	if (!DijkFindPath(cur_map.GetMapName(), dst_map_name, cur_diff, tmp_path))
	{
		assert(false);
		return false;
	}
	if (!way.CalcMapLatelyPath(tmp_path, cur_diff))
	{
		assert(false);
		return false;
	}
	for (auto it = tmp_path.begin();;)
	{
		stMoveHelp tmp_help;
		auto& tmp_it = *it;
		++it;
		if (it == tmp_path.end())
			break;
		tmp_help.end_name_ = *it;
		tmp_help.ori_map_ = *it;
		if (way.TargetMapCanWayPoint(tmp_it, cur_diff) && way.TargetMapCanWayPoint(*it, cur_diff))
		{
			tmp_help.type_ = kGOT_Waypoint;
			tmp_help.difficulty_ = cur_diff;
			way.GetWayPointName(tmp_help.end_name_);
		}
		path_help.push_back(tmp_help);
	}

	if (cur_map.GetChapter() != GetMapCurChapter(dst_map_name))
	{
		DequeHelp ret_way;
		if (!GetDijkLatelyPathByWay(dst_map_name, cur_diff, ret_way))
		{
			assert(false);
			return false;
		}
		if (path_help.size() > ret_way.size())
		{
			path_help.clear();
			path_help.swap(ret_way);
		}
	}
	return true;
}

bool GSP_Resolver::GetDijkLatelyPathByWay(const std::string& dst_map_name, enCD_kMapDifficulty dst_Diff_lvl, DequeHelp& path_help) const
{
	auto& way = GWayPointDlgMgr::GetMe();
	if (!way.RebuildAll())
	{
		assert(false);
		return false;
	}
	auto& cur_map = GMap::GetMe();
	if (!cur_map.Update())
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	auto cur_diff = gpm.GetDiffLvl();
	MapNameCont sorted_map;
	if (!GetTargetMapShortestWayPoint(cur_map.GetMapName(), cur_diff, sorted_map))
	{
		assert(false);
		return false;
	}
	if (sorted_map.empty())
	{
		assert(false);
		return false;
	}

	MapNameCont tmp_path;
	if (!DijkFindPath(sorted_map[0], sorted_map[sorted_map.size() - 1], cur_diff, tmp_path))
	{
		assert(false);
		return false;
	}

	//目标地图
	MapNameCont dst_sorted_map;
	if (!GetTargetMapShortestWayPoint(dst_map_name, dst_Diff_lvl, dst_sorted_map))
	{
		assert(false);
		return false;
	}
	MapNameCont dst_tmp;
	if (!DijkFindPath(dst_sorted_map[dst_sorted_map.size() - 1], dst_sorted_map[0], dst_Diff_lvl, dst_tmp))
	{
		assert(false);
		return false;
	}

	for (auto& i : tmp_path)
	{
		stMoveHelp tmp_help;
		tmp_help.difficulty_ = cur_diff;
		tmp_help.begin_name_ = i;
		if (way.TargetMapCanWayPoint(i, cur_diff))
			tmp_help.type_ = kGOT_Waypoint;
		path_help.push_back(tmp_help);
	}
	for (auto& i : dst_tmp)
	{
		stMoveHelp tmp_help;
		tmp_help.difficulty_ = dst_Diff_lvl;
		tmp_help.begin_name_ = i;
		if (way.TargetMapCanWayPoint(i, dst_Diff_lvl))
			tmp_help.type_ = kGOT_Waypoint;
		path_help.push_back(tmp_help);
	}
	path_help.erase(std::unique(path_help.begin(), path_help.end(), [](const stMoveHelp& L, const stMoveHelp& R){return L.difficulty_ == R.difficulty_ && L.begin_name_ == R.begin_name_; }), path_help.end());
	for (auto it = path_help.begin();;)
	{
		auto& tmp_it = *it;
		++it;
		if (it == path_help.end())
			break;
		tmp_it.end_name_ = it->begin_name_;
		tmp_it.ori_map_ = it->begin_name_;
		if (tmp_it.type_ == kGOT_Waypoint && it->type_ == kGOT_Waypoint)
		{
			tmp_it.difficulty_ = it->difficulty_;
			way.GetWayPointName(tmp_it.end_name_);
		}
		else
			tmp_it.type_ = kGOT_AreaTransition;
	}
	return true;
}


bool GSP_Resolver::GetDijkLatelyPath(const std::string& dst_map_name, enCD_kMapDifficulty diff_lvl, DequeHelp& path_help) const
{
	if (dst_map_name.empty())
	{
		assert(false);
		return false;
	}
	auto& cur_map = GMap::GetMe();
	if (!cur_map.Update())
	{
		assert(false);
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return false;
	}
	auto cur_map_diff = gpm.GetDiffLvl();
	if (cur_map.GetMapName() == dst_map_name && diff_lvl == cur_map_diff)
	{
		LOG_O(Log_debug) << "移动到指定地图- 目标地图等于当前地图 : " << dst_map_name << "难度等级" << cur_map_diff << ":" << diff_lvl;
		return true;
	}
	if (diff_lvl == cur_map_diff)
	{
		if (!GetDijkLatelyPathCurDiff(dst_map_name, path_help))
		{
			assert(false);
			return false;
		}
		return true;
	}
	if (!GetDijkLatelyPathByWay(dst_map_name, diff_lvl,path_help))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GSP_Resolver::GetAllTownName(MapNameCont& all_town) const
{
	resolver_.TraVertices([&all_town](const stVertex& vertex){
		if (vertex.is_town_)
			all_town.push_back(vertex.name_);
		return true;
	});
	return true;
}

bool GSP_Resolver::RebuildAll()
{
	if (resolver_.VertexCount() > 0)
		return true;
	return __super::RebuildAll();
}

void GSP_Resolver::DbgAllVertices() const
{
	LOG_O(Log_debug) << "总共数量：" << resolver_.DbgAllVertices();
}

void GSP_Resolver::DbgAllEdges() const
{
	LOG_O(Log_debug) << "总共数量：" << resolver_.DbgAllEdges();
}

int GSP_Resolver::GetMapCurChapter(const std::string& dst_map_name) const
{
	auto vertex = resolver_.FindVertex([&dst_map_name](const stVertex& vertex){
		return dst_map_name == vertex.name_;
	});
	if (vertex)
		return vertex->chapter_;
	return 0;
}

void GWayPointDlgMgr::DebugInfo() const
{
	if (way_point_info_.empty())
		return;
	typedef std::map< int, std::vector<stMapInfo> > map_cha;
	map_cha map_chainfo;
	std::map<enCD_kMapDifficulty, map_cha > map_info;
	for (auto& i : way_point_info_)
	{
		map_cha& tmp= map_info[i.difficulty_];
		tmp[i.chapter_].push_back(i);
	}

	LOG_O(Log_debug) << "是否打开传送门:" << std::boolalpha << open_way_state_ << " ，总数量："<<way_point_info_.size();
	for (auto& v : map_info)
	{
		LOG_O(Log_debug) << "难度等级 " << v.first <<"-----------------------------------";
		for (auto j : v.second)
		{
			LOG_O(Log_debug) << "第 " << j.first << " 章节";
			for (auto& mm : j.second)
				LOG_O(Log_debug) << "地图: " << mm.map_name_ << ",目标地图:" << mm.dst_map_name_<< ",可否传送" << std::boolalpha << mm.is_can_way_ << "，开启状态：" << mm.is_open_way_;
		}
	}
}

GWayPointDlgMgr::GWayPointDlgMgr()
{
	open_way_state_ = false;
}

bool GWayPointDlgMgr::RebuildAllImpl()
{
	auto dlg = CD_Interface::GetDlgWayPoint();
	if (!dlg)
	{
		assert(false);
		return false;
	}

	open_way_state_ = dlg->is_shown_;
	way_point_info_.clear();
	stMapInfo tmp;
	for (size_t i = kMD_Normal; i < KMD_Max; ++i)
	{
		dlg->TraWaypointItem(kTUSF_All, (enCD_kMapDifficulty)i,[this, &tmp](enCD_kMapDifficulty difficult, stCD_DlgWaypointMap* map, stCD_WaypointPointItem* item){
			tmp.chapter_ = map->chapter_idx_;
			tmp.difficulty_ = difficult;
			tmp.is_open_way_ = map->IsTheWaypointOpened(*item);
			tmp.is_can_way_ = map->TheMapHasWaypoint(*item);

			const std::wstring tmp_str = CD_SafeName(item->map_pin_.tbl_item_->map_ch_name_);
			tmp.map_name_ = WideCharToGbk(tmp_str);
			auto dst_map = map->GetTransferWolrdItem(*item);
			tmp.dst_map_name_ = tmp.map_name_;
			if (dst_map)
			{
				const std::wstring dst_str = CD_SafeName(dst_map->ch_map_name_);
				assert(!dst_str.empty());
				tmp.dst_map_name_ = WideCharToGbk(dst_str);
			}
			way_point_info_.push_back(tmp);
			return true;
		});
	}
	return true;
}
bool GWayPointDlgMgr::TargetMapCanWayPoint(const std::string& dst_map, enCD_kMapDifficulty diff_lvl)
{
	if (dst_map.empty())
		return false;

	if (diff_lvl >= KMD_Max || diff_lvl <= 0)
		return false;

	if (way_point_info_.empty())
	{
		RebuildAll();
		if (way_point_info_.empty())
		{
			LOG_O(Log_debug) << "传送点初始化失败";
			return false;
		}
	}
	
	for (auto& v : way_point_info_)
	{
		if (v.difficulty_ != diff_lvl)
			continue;
		if (dst_map != v.map_name_ && dst_map != v.dst_map_name_)
			continue;
		if (!v.is_can_way_)
			continue;
		if (!v.is_open_way_)
			continue;
		return true;
	}

	return false;
}

GWayPointDlgMgr::stMapInfo* GWayPointDlgMgr::GetDstWayPointState(const std::string& dst_map, enCD_kMapDifficulty diff_lvl)
{
	if (dst_map.empty())
		return nullptr;

	if (diff_lvl >= KMD_Max || diff_lvl <= 0)
		return nullptr;

	if (way_point_info_.empty())
	{
		RebuildAll();
		if (way_point_info_.empty())
		{
			LOG_O(Log_debug) << "传送点初始化失败";
			return nullptr;
		}
	}

	for (auto& v : way_point_info_)
	{
		if (v.difficulty_ != diff_lvl)
			continue;
		if (dst_map != v.map_name_)
			continue;
		if (!v.is_can_way_)
			continue;
		return &v;
	}

	return nullptr;
}

void GWayPointDlgMgr::TestWayPoint(const std::string& dst_name, int diff)
{
	//WayPointToOtherMap(dst_name, (enCD_kMapDifficulty)diff);
	auto& gmp = GPlayerMe::GetMe();
	gmp.SmartMoveMap(dst_name, (enCD_kMapDifficulty)diff);
}


bool  GWayPointDlgMgr::GetWayPointName(std::string& dst_map)
{
	if (dst_map.empty())
		return false;

	for (auto& v : way_point_info_)
	{
		if (!v.is_can_way_)
			continue;
		if (!v.is_open_way_)
			continue;
		if (dst_map != v.map_name_ && dst_map != v.dst_map_name_)
			continue;
		dst_map = v.map_name_;
		return true;
	}
	return false;
}
bool GWayPointDlgMgr::CalcMapLatelyPath(GSP_Resolver::MapNameCont& src_lately_map,enCD_kMapDifficulty diff_lvl)
{
	if (src_lately_map.empty())
		return false;

	auto& way = GWayPointDlgMgr::GetMe();

	size_t i = 0;
	int begin_del = 0;
	for (; i < src_lately_map.size(); ++i)
	{
		if (!way.TargetMapCanWayPoint(src_lately_map[i], diff_lvl))
			continue;
		begin_del = i;
		break;
	}

	if (i >= src_lately_map.size() - 1)
		return true;

	int end_del = 0;
	i = src_lately_map.size() - 1;
	for (; i >= 0; --i)
	{
		if (!way.TargetMapCanWayPoint(src_lately_map[i], diff_lvl))
			continue;

		if (src_lately_map[begin_del] == src_lately_map[i])
			return true;

		end_del = i;
		break;
	}
	if (begin_del + 1 >= end_del)
		return true;
	src_lately_map.erase(src_lately_map.begin() + begin_del + 1, src_lately_map.begin() + end_del);
	return true;
}

bool GWayPointDlgMgr::IsOpenedWayPoint(const std::string& dst_name, enCD_kMapDifficulty diff_lvl)
{
	if (dst_name.empty())
		return false;

	for (auto& v : way_point_info_)
	{
		if (!v.is_can_way_)
			continue;
		if (v.difficulty_ != diff_lvl)
			continue;
		if (dst_name != v.map_name_ && dst_name != v.dst_map_name_)
			continue;
		if (!v.is_can_way_)
			continue;
		return v.is_open_way_;
	}
	return false;
}

bool GWayPointDlgMgr::GetWayPointDstName(std::string& dst_map, enCD_kMapDifficulty diff_lvl)
{
	if (dst_map.empty() || diff_lvl == kMD_Invalid)
		return false;

	for (auto& v : way_point_info_)
	{
		if (!v.is_can_way_)
			continue;
		if (v.difficulty_ != diff_lvl)
			continue;
		if (dst_map != v.map_name_ && dst_map != v.dst_map_name_)
			continue;
		dst_map = v.dst_map_name_;
		return true;
	}
	return false;
}

GStash::GStash() : GNoHuman(kGOT_Stash)
{

}

bool GStash::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	return visitor.VisitMainSubPanelNormal(*this, is_pre_open, open_res, g_va_ui_dlg_stash_);
}

bool GTranstionable::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	return visitor.VisitTranstionable(*this, is_pre_open, open_res);
}

bool GTranstionable::UpdateImpl()
{
	if (!__super::UpdateImpl())
		return false;

	auto entity = GetGameEntityBase();
	if (!entity)
	{
		assert(false);
		return false;
	}
	if (!entity->IsValid())
	{
		assert(false);
		return false;
	}

	entity_transitionable_ = entity->CastToTransitionable();
	if (!entity_transitionable_)
	{
		assert(false);
		return false;
	}
	is_opened_ = entity_transitionable_->IsOpened();
	if (is_opened_)
		return false;
	
	auto game_transitionable = entity_transitionable_->GetRender();
	if (IsUnValidPtr(game_transitionable))
	{
		assert(false);
		return false;
	}
	if (!CD_SafeName(game_transitionable->name_))
	{
		assert(false);
		return false;
	}
	if (game_transitionable->name_.empty())
		return false;

	SetName(game_transitionable->name_.GetStdString());

	return true;
}

void GTranstionable::DebugInfo() const
{
	LOG_O(Log_debug) << "可传送对象-------------";
	__super::DebugInfo();
}

GTranstionable::GTranstionable(GameObjType obj_type) : GameObjBase(obj_type)
{
	is_opened_ = false;
}

bool GTranstionable::GetOpenState() const
{
	return is_opened_;
}

int GTranstionable::GetCanOpenedDistance() const
{
	return kMoveOpenTranstionableDistance;
}

bool GTranstionable::MovingDoTryOpen() const
{
	return __super::MovingDoTryOpen();
	//if (GetCnName() == kObjName_QuestWall)
	//	return false;
	//if (GetCnName() == kObjName_UpTriall)
	//	return false;
	//if (GetCnName() == kObjName_TMDImpress)
	//	return false;
	//if (GetCnName() == kObjName_BXProtective)
	//	return false;
	//if (GetCnName() == kObjName_DXRTSeal)
	//	return false;
	//if (GetCnName() == kObjName_TuErMan)
	//	return false;
	//return true;
}

float GTranstionable::WeightDistance(const stCD_VecInt& pos) const
{
	auto my_pos = GetPos();
	if (!my_pos)
		return __super::WeightDistance(pos);
	auto real_distance = __super::WeightDistance(pos);
	return real_distance / 2 - 50;
}

bool GTranstionable::IsTransitonable() const
{
	return true;
}

bool GTranstionable::CanBreakOpenObj() const
{
	return false;
}

GDbMapPinsMgr::GDbMapPinsMgr()
{

}

bool GDbMapPinsMgr::RebuildAllImpl()
{
	if (!map_pins_db_.empty())
		return true;

	auto map_pins = CD_Interface::GetDbMgr().GetDbMapPins();
	if (!map_pins)
	{
		assert(false);
		return false;
	}

	auto tab = map_pins->GetDbTbl();
	if (IsUnValidPtr(tab))
	{
		assert(false);
		return false;
	}

	if (!tab->tbl_items_.IsValid())
	{
		assert(false);
		return false;
	}

	GDbMapPins tmpDb;
	for (auto& i : tab->tbl_items_)
	{
		tmpDb.SetMapPinAddr(&i);
		if (!tmpDb.UpdateImpl())
		{
			assert(false);
			return false;
		}
		map_pins_db_.push_back(tmpDb);
	}

	return true; 
}


void GDbMapPinsMgr::DebugInfo() const
{
	if (map_pins_db_.empty())
		return;

	for (auto& i : map_pins_db_)
		LOG_O(Log_debug) << "物品名：" << i.GetMapName();
}



GDbMapPins::GDbMapPins()
{
	pins_db_ = nullptr;
}

void GDbMapPins::DebugInfo() const
{

}

bool GDbMapPins::UpdateImpl()
{
	if (!pins_db_)
	{
		assert(false);
		return false;
	}

	if (!CD_SafeName(pins_db_->map_ch_name_))
	{
		assert(false);
		return false;
	}

	map_name_.assign(WideCharToGbk(pins_db_->map_ch_name_));
	return true;
}

const std::string& GDbMapPins::GetMapName() const
{
	return map_name_;
}

void GDbMapPins::SetMapPinAddr(stCD_TblItemMapPins* addr)
{
	pins_db_ = addr;
}

GAvoidSkill& GAvoidSkill::GetMe()
{
	static GAvoidSkill me;
	return me;
}

stMapDataType::stDbgSrcMapData::stDbgSrcMapData(stMapDataType& map_data_impl, bool is_src_map_data)
	: map_data_(map_data_impl)
{
	is_src_map_data_ = is_src_map_data;
}

bool stMapDataType::stDbgSrcMapData::CanMove(const stCD_VecInt& pos) const
{
	return (map_data_.map_data_.*map_data_.fn_)(pos, is_src_map_data_) > kMDF_Mountain;
}

stMapDataType::stDbgGetMapData::stDbgGetMapData(stMapDataType& map_data_impl, bool is_src_map_data)
	: map_data_(map_data_impl)
{
	is_src_map_data_ = is_src_map_data;
}

enCD_MapDataFlag stMapDataType::stDbgGetMapData::MapData(const stCD_VecInt& pos) const
{
	return (map_data_.map_data_.*map_data_.fn_)(pos, is_src_map_data_);
}

void GSP_Resolver::stVertex::DebugInfo() const
{
	LOG_O(Log_debug) << "name:" << name_ << ",diff_lvl:" << diff_lvl_ << ",chapter:" << chapter_
		<< ",is_town:" << is_town_;
}

void GSP_Resolver::stVertex::DebugInfo(const stVertex& dst) const
{
	LOG_O(Log_debug) << "src_name:" << name_ << ",src_diff_lvl:" << diff_lvl_ << ",src_chapter:" << chapter_
		<< ",src_is_town:" << is_town_
		<< ",dst_name:" << dst.name_ << ",dst_diff_lvl:" << dst.diff_lvl_ << ",dst_chapter:" << dst.chapter_
		<< ",dst_is_town:" << dst.is_town_;
}

bool stAreaVertex::operator==(const stAreaVertex& rhs) const
{
	if (transitionable_ && rhs.transitionable_)
		return transitionable_->GetCnName() == rhs.transitionable_->GetCnName()
		&& transitionable_->GetBackupPos() == rhs.transitionable_->GetBackupPos();
	return transitionable_pos_ == rhs.transitionable_pos_;
}

bool stAreaVertex::IsValid() const
{
	return transitionable_ || transitionable_pos_.AnyGreater(0);
}

const stCD_VecInt& stAreaVertex::GetValidPos() const
{
	if (transitionable_) return transitionable_->GetBackupPos();
	return transitionable_pos_;
}

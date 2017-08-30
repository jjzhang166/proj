#include "stdafx.h"
#include "GMap.h"
#include "GPlayer.h"
#include "GClass.h"
#include "GProtocolR.h"
#include "GActions.h"
#include "Scmd.h"
#include "../DllApp.h"

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
		//assert(false);
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
	return visitor.VisitMainSubPanelNormal(*this, is_pre_open, open_res, CD_EXE_DATA(g_a_ui_dlg_waypoint_vtbl_));
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
		//assert(false);
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

bool GAreaTransition::MustCanTargetable() const
{
	auto entity = GetGameEntityBase();
	assert(entity);
	auto render = entity->GetRender();
	if (IsUnValidPtr(render))
	{
		assert(false);
		return false;
	}
	if (render->name_ == kMapName_ShiYanShouWang ||
		render->name_ == kMapName_NanBuSenLin ||
		render->name_ == kMapName_SaEnChenFeiXu ||
		render->name_ == kMapName_TongDao ||
		render->name_ == kMapName_CitySarn)
		return true;
	return false;
}

bool GAreaTransition::DoOpen()
{
	if (!MyLuaThrdData::GetResetMap() || !CanResetOpenMap())
		return __super::DoOpen();
	
	SetOpenFlag(kTTSF_WithLeftCtrl);
	if (!__super::DoOpen())
	{
		assert(false);
		return false;
	}
		
	if(!TimerDo(800, 1000 * 30, [](){
		return GWndExecSync([](){return !!CD_Interface::GetDlgInstanceMgr(); });
	}))
	{
		assert(false);
		return false;
	}
	return GWndExecSync([](){
		auto instance_mgr = CD_Interface::GetDlgInstanceMgr();
		if (!instance_mgr)
		{
			assert(false);
			return false;
		}
		return instance_mgr->SelectInstance(-1);
	});
}

bool GAreaTransition::CanResetOpenMap() const
{
	if (IsTownTranstionable())
		return false;
	auto& tra_mgr = GMapTraMgr::GetMe();
	if (tra_mgr.HasTheMapAreaName(GetCnName()))
		return false;
	auto& gsp_resolver = GSP_Resolver::GetMe();
	if (gsp_resolver.HasTheMapName(GetCnName()))
	{
		if (gsp_resolver.IsTownByMapName(GetCnName(), map_difficulty_))
			return false;
		return GetCnName() != GMap::GetMe().GetMapName();
	}
	return false;
}

bool GMap::UpdateImpl()
{
	auto old_map_name = map_name_;

	if (!__super::UpdateImpl())
	{
		//assert(false);
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

	if (old_map_name != map_name_)
	{
		GetMyApp().ModifyConsoleCfg("mapName", map_name_, true);
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
	last_map_info.last_area_id_ = gpm.GetAreaId();
	last_map_info.last_pos_ = *player_pos;
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

void GMap::ParseCurMemoryMapInfo()
{
	auto& suggest = GMapTraMgr::GetMe().GetTraGuess();
	auto& area_resolver = suggest.GetAreaResolver();
	auto& area_connections = suggest.GetAreaConnections();
	area_resolver.clear();
	area_connections.clear();
	auto& cur_memory = GetFreeMemoryMapInfo(map_name_);
	if (cur_memory.map_name_.empty())
		return;
	assert(cur_memory.map_name_ == map_name_);
	auto& astar = G_AStar::GetMe();
	if (cur_memory.astar_data_ != astar)
		return;
	area_resolver = cur_memory.area_resolver_;
	area_connections = cur_memory.area_connections_;
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
	free_memory->area_connections_.clear();
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
	auto& suggest = GMapTraMgr::GetMe().GetTraGuess();
	auto& area_resolver = suggest.GetAreaResolver();
	auto& area_connections = suggest.GetAreaConnections();
	if (map_info.map_name_.empty())
	{
		map_info.map_name_ = last_map_info_.last_map_name_;
		map_info.obj_mgr_ = memory_objs;
		map_info.astar_data_ = astar;
		map_info.area_resolver_ = area_resolver;
		map_info.area_connections_ = area_connections;
		return;
	}
	assert(map_info.map_name_ == last_map_info_.last_map_name_);
	if (map_info.astar_data_ == astar)
	{
		map_info.obj_mgr_ += memory_objs;
		map_info.area_resolver_ += area_resolver;
		map_info.area_connections_ = area_connections;
		return;
	}
	map_info.obj_mgr_ = memory_objs;
	map_info.astar_data_ = astar;
	map_info.area_resolver_ = area_resolver;
	map_info.area_connections_ = area_connections;
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

void GMapTraMgr::SetMapAreaInfo(const std::string& map_name, const luabind::object& areas)
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
	auto& suggest = GMapTraMgr::GetMe().GetTraGuess();
	auto& area_connections = suggest.GetAreaConnections();
	if (area_connections.empty())
		return;
	auto cur_area = GPlayerMe::GetMe().GetAreaId();
	for (auto& i : area_connections)
	{
		auto tmp_veter = i.first;
		if (tmp_veter && tmp_veter->GetAreaId() == cur_area &&tmp_veter->GetPassState() == kAPS_FristPass)
			tmp_veter->SetAreaState(kAPS_CanPass);
		tmp_veter = i.second;
		if (tmp_veter && tmp_veter->GetAreaId() == cur_area &&tmp_veter->GetPassState() == kAPS_FristPass)
			tmp_veter->SetAreaState(kAPS_CanPass);
	}
}

void GMapTraPolicyBase::AddAreaConnection(int vertex_lhs, int vertex_rhs)
{
	if (!area_resolver_.Exist(vertex_lhs))
	{
		area_resolver_.AddVertex(vertex_lhs);
	}
	if (!area_resolver_.Exist(vertex_rhs))
	{
		area_resolver_.AddVertex(vertex_rhs);
	}
	if (!area_resolver_.Exist(vertex_lhs, vertex_rhs))
	{
		area_resolver_.AddEdge(vertex_lhs, vertex_rhs);
	}
}

bool GMapTraPolicyBase::GenAreaSteps(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, GAreaStepsType& steps) const
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
		//assert(false);
		return false;
	}
	if (src_area_id == dst_area_id)
		return true;
	return GenAreaStepsImpl(src_area_id, dst_area_id, steps);
}

bool GMapTraPolicyBase::CalcSuggestAreaStep(GAreaStepsType& steps) const
{
	assert(!is_suggest_);
	is_suggest_ = true;
	BOOST_SCOPE_EXIT_ALL(this){
		is_suggest_ = false;
	};
	steps.clear();
	auto& gpm = GPlayerMe::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	auto area_id = gpm.GetAreaId();
	if (area_id < 0)
	{
		assert(false);
		return false;
	}
	GAreaResolver::StepsContType steps_to_other;
	area_resolver_.Resolve(area_id, steps_to_other);
	if (steps_to_other.empty())
		return GenUnPassedAreaSteps(steps);
	auto& path_mark = GPathMark::GetMe();
	stCD_VecInt pos_tmp;
	for (auto the_area_id : steps_to_other)
	{
		if (path_mark.GetSuggestPos(*player_pos, the_area_id, pos_tmp))
		{
			steps.clear();
			if (GenAreaStepsImpl(area_id, the_area_id, steps))
				return true;
		}
	}
	return GenUnPassedAreaSteps(steps);
}

GMapTraPolicyBase::GMapTraPolicyBase()
{
	is_suggest_ = false;
}

CA_ActionPtr GMapTraPolicyBase::MakeTraMap(const GA_SmartConsumerT& consumer, const CA_ActionPtr& action_tra_do, bool clear_mark_pos) const
{
	return GA_Factory::GetMe().MakeTraGuessArea(consumer, action_tra_do, clear_mark_pos);
}

bool GMapTraDirect::GenAreaStepsImpl(int area_from, int area_dst, GAreaStepsType& steps) const
{
	GAreaResolver::StepsContType area_steps;
	if (!area_resolver_.Resolve(area_from, area_dst, area_steps))
	{
		if (is_suggest_)
			return GenUnPassedAreaSteps(steps);
		return false;
	}
	if (area_steps.empty())
	{
		assert(false);
		return false;
	}
	auto& terrain_parser = GTerrainParser::GetDefTerrainParser();
	auto terrain_info = terrain_parser.GetTheMap(GMap::GetMe().GetMapName());
	if (!terrain_info)
	{
		assert(false);
		return false;
	}
	for (auto it = area_steps.begin(), it_end = area_steps.end(); it != it_end; ++it)
	{
		auto it_next = it + 1;
		if (it_next == it_end)
			break;
		auto connection = GTerrainParser::GetAreaConnection(*terrain_info, *it, *it_next);
		if (connection.first == connection.second)
		{
			assert(false);
			return false;
		}
		steps.push_back(AreaVertexPtr(new AreaVertexDirect(connection.first)));
	}

	if (is_suggest_)
	{
		if (!steps.empty())
			return true;
		return GenUnPassedAreaSteps(steps);
	}
	else
	{
		if (steps.empty())
		{
			assert(false);
			return false;
		}
	}
	return true;
}

AreaVertexPtr GMapTraDirect::FindAreaByName(const std::string& area_name) const
{
	auto terrain = GTerrainParser::GetDefTerrainParser().FindTerrain(area_name);
	if (!terrain)
		return nullptr;
	return AreaVertexPtr(new AreaVertexDirect(terrain));
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

stMapDataType::stMapDataType(stCD_MapData& map_data, const stCD_VecInt& map_size, const stCD_VecInt& translation)
	: map_data_(map_data), map_size_(map_size), translation_(translation)
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

void G_AStar::DebugPosRectInfo2(const std::string& file_name, const stCD_VecInt& pos, int range, bool is_src_map_data) const
{
	auto& map = GMap::GetMe();
	auto map_d = map.GetMapData();
	if (!map_d)
	{
		assert(false);
		return;
	}
	stMapDataType map_data(*map_d, map.GetMapSize());
	stMapDataType::stDbgSrcMapData wrapper(map_data, is_src_map_data);
	return DebugPosRectInfo(wrapper, file_name, pos, range);
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

stCD_VecInt G_AStar::CastToGamePos_(int my_pos_x, int my_pos_y)
{
	stCD_VecInt game_pos;
	CastToGamePos(stVec2Int(my_pos_x, my_pos_y), game_pos);
	return game_pos;
}

SmallAStar* GMap::GetSmallAStar(const stCD_VecInt& center_pos) const
{
	if (IsUnValidPtr(game_map_data_))
	{
		assert(false);
		return false;
	}
	auto translation = center_pos;
	translation -= stCD_VecInt(SmallAStar::kMaxSideSize / 2, SmallAStar::kMaxSideSize / 2);
	stMapDataType map_data(*game_map_data_, stCD_VecInt(SmallAStar::kMaxSideSize, SmallAStar::kMaxSideSize),
		translation);
	LOG_O(Log_trace) << "开始初始化SmallAStar";
	auto res = small_astar_.Init(map_data);
	LOG_O(Log_debug) << "SmallAStar初始化结果：" << res;
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	return &small_astar_;
}

void GMap::DebugSmallAStarPosInfo(bool dbg_data, const std::string& file_name) const
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
	auto small_astar = GetSmallAStar(*player_pos);
	if (!small_astar)
	{
		assert(false);
		return;
	}
	return small_astar->DebugPosInfo(dbg_data, file_name, *player_pos);
}

bool GMap::UpdateArea(const stCD_VecInt& center_pos) const
{
	if (IsUnValidPtr(game_map_data_))
	{
		assert(false);
		return false;
	}
	stMapDataType map_data(*game_map_data_, stCD_VecInt());
	auto& astar = G_AStar::GetMe();
	return astar.UpdateArea(center_pos, 15, map_data);
}

bool GMap::UpdateSelfArea_()
{
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	return UpdateArea(*player_pos);
}

bool GMapTraDirect::ParseAreaConnections()
{
	area_resolver_.clear();
	auto& parser = GTerrainParser::GetDefTerrainParser();
	auto& map_name = GMap::GetMe().GetMapName();
	auto cur_map_tag_info = parser.GetTheMap(map_name);
	if (!cur_map_tag_info) return true;
	for (auto& tag_info : *cur_map_tag_info)
	{
		if (!tag_info.dst_tag_info_)
			continue;
		if (tag_info.area_id_ == tag_info.dst_tag_info_->area_id_)
		{
			LOG_O(Log_debug) << "门配置错误，房间ID不能是一样的，地图名:" << map_name << ",src:" << tag_info.pos_name_
				<< ",dst:" << tag_info.dst_pos_name_;
			continue;
		}
		AddAreaConnection(tag_info.area_id_, tag_info.dst_tag_info_->area_id_);
	}
	return true;
}

bool GMapTraDirect::GenUnPassedAreaSteps(GAreaStepsType& steps) const
{
	return GMapTraMgr::GetMe().GetTraGuess().GenUnPassedAreaSteps(steps);
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

GNoHuman::GNoHuman(enGameObjType obj_type) : GameObjBase(obj_type)
{
	no_humain_ = nullptr;
}

bool GNoHuman::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		//assert(false);
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
		if (vertex_src.name_ == kMapName_Coast)
			continue;
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
			if (!FilterLinkMap(vertex_src,vertex_dst))
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
	for (auto& v : sorted_map)
	{
		++i;
		if (!way.TargetMapCanWayPoint(v, diff_lvl))
			continue;
		break;
	}
	sorted_map.erase(sorted_map.begin() + i, sorted_map.end());
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
			way.GetWayPointName(tmp_help.end_name_,cur_diff);
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
			way.GetWayPointName(tmp_it.end_name_, tmp_it.difficulty_);
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

bool GSP_Resolver::GetAllTownName(MapNameVertex& all_town) const
{
	resolver_.TraVertices([&all_town](const stVertex& vertex){
		if (vertex.is_town_)
			all_town.push_back(vertex);
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

bool GSP_Resolver::FilterLinkMap(const stVertex& src, const stVertex& dst) const
{
	if (src.name_ == kMapName_Hifhgate && dst.name_ == kMapName_BellyBeast)
		return false;
	if (src.name_ == kMapName_BellyBeast && dst.name_ == kMapName_Hifhgate)
		return false;
	if (src.name_ == kMapName_Hifhgate && dst.name_ == kMapName_YULingZhiShi)
		return false;
	if (src.name_ == kMapName_YULingZhiShi && dst.name_ == kMapName_Hifhgate)
		return false;

	return true;
}

bool GSP_Resolver::HasTheMapName(const std::string& map_name) const
{
	auto vertex = resolver_.FindVertex([&map_name](const stVertex& vertex){
		return map_name == vertex.name_;
	});
	if (vertex)
		return true;
	return false;
}

/*
bool GSP_Resolver::UserDefinedLinkMap()
{
	struct stLinkInfo{
		stLinkInfo(const std::string& name, int area_id) : map_name_(name), area_id_(area_id){}
		std::string			map_name_;
		int			 area_id_;
	};
	const stLinkInfo define_link[] = {
		{ "Estate Path", 0 },
		{ "Aspirant's Trial", 61536 },
		{ "Basilica Halls", 0 },
		{ "Sanitorium Passage", 0 },
		{ "Aspirant's Trial", 25204 },
		{ "Domain Walkways", 0 },
		{ "Sanitorium Halls", 0 },
		{ "Aspirant's Trial", 21443 },
		{ "Ascendancy Chamber", 0 }
	};
	for (auto it = std::begin(define_link); it != std::end(define_link);)
	{
		if (it == std::end(define_link))
			break;
		stVertex vertex_src;
		vertex_src.chapter_ = 3;
		vertex_src.diff_lvl_ = kMD_Normal;
		vertex_src.name_ = it->map_name_;
		vertex_src.area_id_ = it->area_id_;
		if (!resolver_.Exist(vertex_src))
			resolver_.AddVertex(vertex_src);
		++it;
		if (it == std::end(define_link))
			break;
		stVertex vertex_dst;
		vertex_dst.chapter_ = 3;
		vertex_dst.diff_lvl_ = kMD_Normal;
		vertex_dst.name_ = it->map_name_;
		vertex_dst.area_id_ = it->area_id_;
		if (!resolver_.Exist(vertex_dst))
			resolver_.AddVertex(vertex_dst);
		if (!resolver_.Exist(vertex_src, vertex_dst))
			resolver_.AddEdge(vertex_src, vertex_dst);
	}
	return true;
}
*/

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
		//if (dst_map != v.map_name_ && dst_map != v.dst_map_name_)
		//	continue;
		if (dst_map != v.dst_map_name_)
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


bool  GWayPointDlgMgr::GetWayPointName(std::string& dst_map, enCD_kMapDifficulty diff_lvl)
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
		if (diff_lvl != v.difficulty_)
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

const std::string& GWayPointDlgMgr::GetCanWayMapByChapter(int chapter)
{
	for (auto& v : way_point_info_)
	{
		if (v.chapter_ != chapter)
			continue;
		if (!v.is_can_way_)
			continue;
		if (!v.is_open_way_)
			continue;
		return v.dst_map_name_;
	}
	return kEmptyStr;
}

GStash::GStash() : GNoHuman(kGOT_Stash)
{

}

bool GStash::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	return visitor.VisitMainSubPanelNormal(*this, is_pre_open, open_res, CD_EXE_DATA(g_va_ui_dlg_stash_));
}

bool GTranstionable::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	auto cn_name = GetCnName();
	if (cn_name == kObjName_LabyrinthDevice)
		return visitor.VisitMainSubPanelNormal(*this, is_pre_open, open_res, CD_EXE_DATA(g_va_ui_dlg_labyrinth_select_));
	else if (cn_name == kObjName_LabyrinthTrialAtCity || cn_name == kObjName_LabyrinthTrialAtFb)
	{
		if (!visitor.VisitMainSubPanelNormal(*this, is_pre_open, open_res, CD_EXE_DATA(g_va_ui_dlg_trial_plaque_)))
			return false;
		if (!is_pre_open)
		{
			auto& trial_plaque = GTrialPlaqueDlg::GetMe();
			return trial_plaque.RebuildAll();
		}
		return true;
	}	
	else
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
	/*is_opened_ = entity_transitionable_->IsOpened();
	if (is_opened_)
		return false;*/
	
	auto game_transitionable = entity_transitionable_->GetRender();
	if (IsUnValidPtr(game_transitionable))
	{
		//assert(false);
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

GTranstionable::GTranstionable(enGameObjType obj_type) : GameObjBase(obj_type)
{
	//is_opened_ = false;
}

int GTranstionable::GetCanOpenedDistance() const
{
	return kMoveOpenTranstionableDistance;
}

bool GTranstionable::MovingDoTryOpen() const
{
	return __super::MovingDoTryOpen();
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

class GTerrainParser_Cmp : public ITerrainParser{
	bool Match(stCD_MapDataTerrainModel* model, pt_cwsz db_path, const std::wstring& tag_str) override{
		return tag_str == db_path;
	}
};

class GTerrainParser_Strstr : public ITerrainParser{
	bool Match(stCD_MapDataTerrainModel* model, pt_cwsz db_path, const std::wstring& tag_str) override{
		return !!StdEx::TStrStr(db_path, tag_str.c_str());
	}
};
class GTerrainParser_ObjDbPath : public ITerrainParser{
	bool Match(stCD_MapDataTerrainModel* model, pt_cwsz db_path, const std::wstring& tag_str) override{
		for (auto& obj_info : model->terrain_objs_)
		{
			if (IsUnValidPtr(obj_info.obj_positioned_))
			{
				assert(false);
				return false;
			}
			auto entity = obj_info.obj_positioned_->entity_;
			if (IsUnValidPtr(entity))
			{
				assert(false);
				return false;
			}
			auto db_path = entity->GetDbPath();
			if (!db_path)
			{
				assert(false);
				return false;
			}
			if (db_path->find(tag_str.c_str()) != GameWString::npos)
				return true;
		}
		return false;
	}
};
static GTerrainParser_Cmp g_terrain_parser_cmp;
static GTerrainParser_Strstr g_terrain_parser_strstr;
static GTerrainParser_ObjDbPath g_terrain_parser_obj_db_path_;

void GTerrainParser::AddAny(enTerrainParserFlag flag, const std::string& pos_name, const std::string& tag_str)
{
	assert(!pos_name.empty());
	assert(!tag_str.empty());
	any_tags_info_.push_back(stTerrainTagInfo());
	auto& tag_info = any_tags_info_.back();
	if (flag & kTPF_Cmp)
		tag_info.parser_ = &g_terrain_parser_cmp;
	else if (flag & kTPF_Strstr)
		tag_info.parser_ = &g_terrain_parser_strstr;
	else if (flag & kTPF_ObjDbPath)
		tag_info.parser_ = &g_terrain_parser_obj_db_path_;
	else
	{
		assert(false);
		return;
	}
	tag_info.pos_name_ = pos_name;
	tag_info.tag_str_ = GbkToWideChar(tag_str);
}

void GTerrainParser::AddMap(const std::string& map_name, enTerrainParserFlag flag, 
	const std::string& pos_name, const std::string& tag_str, const std::string& dst_pos_name)
{
	assert(!map_name.empty());
	assert(!pos_name.empty());
	assert(!tag_str.empty());
	auto& tags_info = map_tags_info_[map_name];
	tags_info.push_back(stTerrainTagInfo());
	auto& tag_info = tags_info.back();
	if (flag & kTPF_Cmp)
		tag_info.parser_ = &g_terrain_parser_cmp;
	else if (flag & kTPF_Strstr)
		tag_info.parser_ = &g_terrain_parser_strstr;
	else if (flag & kTPF_ObjDbPath)
		tag_info.parser_ = &g_terrain_parser_obj_db_path_;
	else
	{
		assert(false);
		return;
	}
	tag_info.pos_name_ = pos_name;
	tag_info.tag_str_ = GbkToWideChar(tag_str);
	tag_info.dst_pos_name_ = dst_pos_name;
}

bool GTerrainParser::ParseUnique(TagInfoCont* map_info, const stCD_MapData& map_data)
{
	CmnStaticVector<stTerrainTagInfo*, 100> tags_info_tmp;
	for (auto& tag_info : any_tags_info_)
	{
		tag_info.pos_.reset();
		tag_info.dst_tag_info_ = nullptr;
		if (tag_info.parser_)
			tags_info_tmp.push_back(&tag_info);
	}
	if (map_info)
	{
		for (auto& tag_info : *map_info)
		{
			tag_info.pos_.reset();
			tag_info.dst_tag_info_ = nullptr;
			if (tag_info.parser_)
				tags_info_tmp.push_back(&tag_info);
		}
	}
	if (tags_info_tmp.empty())
		return true;
	stCD_VecInt pos_tmp;
	auto& astar = G_AStar::GetMe();
	auto the_size = tags_info_tmp.size();
	for (pos_tmp.y = 0; pos_tmp.y < map_data.shrink_size_.y; ++pos_tmp.y)
	{
		auto h = pos_tmp.y * map_data.shrink_size_.x;
		for (pos_tmp.x = 0; pos_tmp.x < map_data.shrink_size_.x; ++pos_tmp.x)
		{
			int idx = h + pos_tmp.x;
			auto model = map_data.terrain_models_ + idx;
			if (!model->model_.IsValid())
				continue;
			auto model_path = model->model_.tbl_item_->model_path_;
			assert(model_path);
			int matched_cnt = 0;
			for (auto tag_info : tags_info_tmp)
			{
				if (tag_info->pos_)
				{
					++matched_cnt;
					continue;
				}
				if (!tag_info->parser_)
				{
					assert(false);
					continue;
				}
				if (!tag_info->parser_->Match(model, model_path, tag_info->tag_str_))
					continue;
				++matched_cnt;
				tag_info->pos_ = map_data.ShrinkPos2Pos(pos_tmp);
				tag_info->area_id_ = astar.GetAreaId(*tag_info->pos_);
				assert(tag_info->area_id_ >= 0);
			}
			if (matched_cnt == the_size)
				goto LABEL_HERE;
		}
	}
LABEL_HERE:
	return true;
}

GTerrainParser& GTerrainParser::GetDefTerrainParser()
{
	struct stHelp{
		stHelp(){
			//parser_.AddMapParser(kMapName_GuJinZiTa, MakeMapTerrainParser(&ParseGuJinZiTa),
			//{ { kMapPosName_UpLouTi, kMapPosName_DownLouTi } });
		}

		GTerrainParser parser_;
	};
	static stHelp hlp;
	return hlp.parser_;
}

const stTerrainTagInfo* GTerrainParser::FindTerrain(const std::string& pos_name) const
{
	CmnStaticVector<const stTerrainTagInfo*, 100> tags_info;
	for (auto& tag_info : any_tags_info_)
	{
		if (!tag_info.pos_)
			continue;
		tags_info.push_back(&tag_info);
	}
	auto it = map_tags_info_.find(GMap::GetMe().GetMapName());
	if (it != map_tags_info_.end())
	{
		for (auto& tag_info : it->second)
		{
			if (!tag_info.pos_)
				continue;
			tags_info.push_back(&tag_info);
		}
	}
	if (tags_info.empty())
		return nullptr;
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return nullptr;
	}
	auto player_area_id = G_AStar::GetMe().GetAreaId(*player_pos);
	assert(player_area_id >= 0);
	struct stHelp{
		const stTerrainTagInfo*	tag_info_;
		int						weight_;
	};
	CmnStaticVector<stHelp, 50> res_cont;
	for (auto tag_info : tags_info)
	{
		if (pos_name != tag_info->pos_name_)
			continue;
		int weight = (abs(player_area_id - tag_info->area_id_) + 1) * player_pos->Distance(*tag_info->pos_);
		res_cont.push_back({ tag_info, weight });
	}
	if (res_cont.empty())
		return nullptr;
	std::sort(res_cont.begin(), res_cont.end(), [](const stHelp& lhs, const stHelp& rhs){
		return lhs.weight_ < rhs.weight_;
	});
	auto terrain = res_cont.front().tag_info_;
	assert(terrain);
	if (terrain->pos_)
		return terrain;
	return nullptr;
}

const GTerrainParser::TagInfoCont* GTerrainParser::GetTheMap(const std::string& map_name) const
{
	auto it = map_tags_info_.find(map_name);
	if (it == map_tags_info_.end()) return nullptr;
	return &it->second;
}

bool GTerrainParser::Parse(const std::string& map_name, const stCD_MapData& map_data)
{
	LOG_O(Log_trace) << "开始解析地形：" << map_name;
	TagInfoCont* map_info = nullptr;
	auto it = map_tags_info_.find(map_name);
	if (it == map_tags_info_.end())
	{
		LOG_O(Log_debug) << "没有当前地图的地形配置数据信息：" << map_name;
	}
	else
	{
		map_info = &it->second;
	}
	//解析唯一的地形
	if (!ParseUnique(map_info, map_data))
	{
		assert(false);
		return false;
	}
	//根据地形区域来定位一个地形，地形区域的内容包括上一个地形与下一个地形之间的距离
	if (!ParseTerrainAreas(map_info, map_data))
	{
		assert(false);
		return false;
	}
	//根据具体的解析器来解析地形
	if (!ParseByParser(map_info, map_data))
	{
		assert(false);
		return false;
	}
	//解析连接信息
	if (!DoParseConnections(map_info))
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
#ifdef _DEBUG
	if (map_info)
	{
		for (auto& tag_info : *map_info)
		{
			if (tag_info.pos_)
				continue;
			LOG_O(Log_debug) << "该地形数据没有匹配到，PosName:" << tag_info.pos_name_;
		}
	}
#endif
	//////////////////////////////////////////////////////////////////////////
	if (!GMapTraMgr::GetMe().InitParseTerrain())
	{
		assert(false);
		return false;
	}
	LOG_O(Log_trace) << "地形解析成功：" << map_name;
	return true;
}

bool GTerrainParser::ParseTerrainAreas(TagInfoCont* map_info, const stCD_MapData& map_data)
{
	if (!map_info)
		return true;
	CmnStaticVector<stTerrainTagInfo*, 50> tags_info;
	for (auto& tag_info : *map_info)
	{
		if (tag_info.terrain_area_.empty())
			continue;
		assert(!tag_info.pos_);
		tags_info.push_back(&tag_info);
	}
	for (auto tag_info : tags_info)
	{
		if (!ParseOneTerrainArea(*tag_info, map_data, false))
		{
			LOG_O(Log_debug) << "未能解析出地形,pos_name：" << tag_info->pos_name_;
			//return false;
			continue;
		}
	}
	return true;
}

bool GTerrainParser::ParseOneTerrainArea(stTerrainTagInfo& tag_info, const stCD_MapData& map_data, bool is_unique) const
{
	if (tag_info.terrain_area_.empty())
		return true;
	struct stHelp{
		pt_cwsz			terrain_path_;
		stCD_VecInt		pos_;
	};
	CmnBufferVector<stHelp, 500 * 500> terrain_cont;
	stCD_VecInt pos_tmp;
	for (pos_tmp.y = 0; pos_tmp.y < map_data.shrink_size_.y; ++pos_tmp.y)
	{
		auto h = pos_tmp.y * map_data.shrink_size_.x;
		for (pos_tmp.x = 0; pos_tmp.x < map_data.shrink_size_.x; ++pos_tmp.x)
		{
			int idx = h + pos_tmp.x;
			auto model = map_data.terrain_models_ + idx;
			if (!model->model_.IsValid())
				continue;
			auto model_path = model->model_.tbl_item_->model_path_;
			assert(model_path);
			if (tag_info.tag_str_ == model_path)
			{
				terrain_cont.push_back({ model_path, map_data.ShrinkPos2Pos(pos_tmp) });
			}
			for (auto& terrain_area : tag_info.terrain_area_)
			{
				if (terrain_area.tag_dst_ == model_path)
				{
					terrain_cont.push_back({ model_path, map_data.ShrinkPos2Pos(pos_tmp) });
				}
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (terrain_cont.empty())
	{
		assert(false);
		return false;
	}
	auto& astar = G_AStar::GetMe();
	auto it_begin = tag_info.terrain_area_.begin(), it_end = tag_info.terrain_area_.end();
	auto it_terrain_begin = terrain_cont.begin(), it_terrain_end = terrain_cont.end();
	int cnt = 0;
	for (auto& terrain : terrain_cont)
	{
		if (terrain.terrain_path_ != tag_info.tag_str_)
			continue;
		tag_info.pos_ = terrain.pos_;
		tag_info.area_id_ = astar.GetAreaId(terrain.pos_);
		if (tag_info.area_id_ < 0)
		{
			assert(false);
			continue;
		}

		auto cur_pos = &terrain.pos_;
		auto it_terrain = it_terrain_begin;
		auto it = it_begin;
		while (it_terrain != it_terrain_end && it != it_end)
		{
			if (it_terrain->terrain_path_ != it->tag_dst_)
			{
				++it_terrain;
				continue;
			}
			if (cur_pos->Distance(it_terrain->pos_) > it->max_distance_)
			{
				++it_terrain;
				continue;
			}
			cur_pos = &it_terrain->pos_;
			++it;
			it_terrain = it_terrain_begin;
		}
		if (it == it_end)
		{
			if (is_unique)
			{
				++cnt;
				if (cnt > 1)
					return false;
			}
			else
				return true;
		}
	}
	if (is_unique && cnt == 1)
		return true;
	//assert(false);
	tag_info.pos_.reset();
	tag_info.area_id_ = -1;
	return false;
}

void GTerrainParser::AddMap_(const std::string& map_name, const std::string& pos_name, const std::string& tag_str)
{
	return AddMap(map_name, kTPF_Cmp, pos_name, tag_str, kEmptyStr);
}

void GTerrainParser::AddMap1_(const std::string& map_name, const std::string& pos_name, const std::string& tag_str
	, const std::string& dst_pos_name)
{
	return AddMap(map_name, kTPF_Cmp, pos_name, tag_str, dst_pos_name);
}

//发现一个地形区域，地形区域的内容包括上个地形与下一个地形的距离。
template<typename BlackCont>
static bool _DetectOneTerrainAreaImpl(const stCD_VecInt& last_terrain_pos, stTerrainTagInfo& tmp_tag_info, 
	const stCD_MapData& map_data, BlackCont& black_cont, const stRectInt& rc, int terrain_cnt){
	tmp_tag_info.terrain_area_.push_back(stTerrainArea());
	auto& cur_terrain_area = tmp_tag_info.terrain_area_.back();
	stCD_VecInt pos_tmp, cur_pos;
	for (pos_tmp.y = rc.left_bottom_.y; pos_tmp.y <= rc.right_top_.y; ++pos_tmp.y)
	{
		auto h = pos_tmp.y * map_data.shrink_size_.x;
		for (pos_tmp.x = rc.left_bottom_.x; pos_tmp.x <= rc.right_top_.x; ++pos_tmp.x)
		{
			int idx = h + pos_tmp.x;
			auto model = map_data.terrain_models_ + idx;
			if (!model->model_.IsValid())
				continue;
			for (auto v : black_cont)
			{
				if (v == model)
					goto LABEL_CONTINUE;
			}
			goto LABEL_NEXT;
		LABEL_CONTINUE:
			continue;
		LABEL_NEXT:
			auto model_path = model->model_.tbl_item_->model_path_;
			if (!model_path)
			{
				assert(false);
				continue;
			}
			cur_pos = stCD_MapData::ShrinkPos2Pos(pos_tmp);
			cur_terrain_area.tag_dst_ = model_path;
			cur_terrain_area.max_distance_ = last_terrain_pos.Distance(cur_pos) + 1;
			if (0 != terrain_cnt)
			{
				if (_DetectOneTerrainAreaImpl(cur_pos, tmp_tag_info, map_data, black_cont, rc, terrain_cnt - 1))
					return true;
				continue;
			}
			if (!GTerrainParser::GetDefTerrainParser().ParseOneTerrainArea(tmp_tag_info, map_data, true))
				continue;
			LOG_O(Log_trace) << "得到地形区域结果，第一个：" << tmp_tag_info.tag_str_;
			for (auto& v : tmp_tag_info.terrain_area_)
			{
				LOG_O(Log_trace) << "tag_dst:" << v.tag_dst_ << ",max_distance:" << v.max_distance_;
			}
			return true;
		}
	}
	tmp_tag_info.terrain_area_.pop_back();
	return false;
}

void GTerrainParser::TestDetectOneTerrainArea(int radius, int terrain_cnt) const
{
	if (terrain_cnt < 2)
	{
		assert(false);
		return;
	}
	--terrain_cnt;
	auto world = CD_Interface::GetWorld();
	if (!world)
	{
		assert(false);
		return;
	}
	auto& map_data = world->map_data_;
	auto player_pos = GPlayerMe::GetMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return;
	}
	auto player_entity = GPlayerMe::GetMe().GetGameEntityBase();
	if (!player_entity)
	{
		assert(false);
		return;
	}
	stCD_VecInt player_terrain_pos;
	auto player_terrain = map_data.GetTerrainFrom(player_entity, player_terrain_pos);
	if (!player_terrain)
	{
		assert(false);
		return;
	}
	stRectInt rc(*player_pos, *player_pos);
	rc.left_bottom_ -= radius;
	rc.right_top_ += radius;
	rc.left_bottom_ = map_data.Pos2ShrinkPos(rc.left_bottom_);
	rc.right_top_ = map_data.Pos2ShrinkPos(rc.right_top_);
	rc.RectifyAtRange(stCD_VecInt(0, 0), map_data.shrink_size_);

	stTerrainTagInfo tmp_tag_info;
	tmp_tag_info.tag_str_ = player_terrain->model_.tbl_item_->model_path_;

	CmnBufferVector<stCD_MapDataTerrainModel*, 500 * 500> black_model;
	black_model.push_back(map_data.terrain_models_ + (player_terrain_pos.y * map_data.shrink_size_.x + player_terrain_pos.x));

	auto pos_player = map_data.ShrinkPos2Pos(player_terrain_pos);
	if (G_AStar::GetMe().GetAreaId(pos_player) < 0)
	{
		assert(false);
		return;
	}
	_DetectOneTerrainAreaImpl(pos_player, tmp_tag_info, map_data, black_model, rc, terrain_cnt - 1);
}

void GTerrainParser::AddMapArea(const std::string& map_name, const std::string& pos_name, 
	const std::string& tag_str, const luabind::object& terrain_area, const std::string& dst_pos_name)
{
	using namespace luabind;
	assert(!map_name.empty());
	assert(!pos_name.empty());
	assert(!tag_str.empty());
	assert(LUA_TTABLE == type(terrain_area));
	
	auto& tags_info = map_tags_info_[map_name];
	tags_info.push_back(stTerrainTagInfo());
	auto& tag_info = tags_info.back();

	tag_info.pos_name_ = pos_name;
	tag_info.tag_str_ = GbkToWideChar(tag_str);
	tag_info.dst_pos_name_ = dst_pos_name;

	for (auto& v : terrain_area)
	{
		auto tag_dst = LuaObjCast(v[1], kEmptyStr);
		if (tag_dst.empty())
		{
			assert(false);
			continue;
		}
		auto max_distance = LuaObjCast(v[2], float(0));
		if (max_distance == 0)
		{
			assert(false);
			continue;
		}
		tag_info.terrain_area_.push_back(stTerrainArea());
		auto& back = tag_info.terrain_area_.back();
		back.tag_dst_ = GbkToWideChar(tag_dst);
		back.max_distance_ = max_distance;
	}
}

void GTerrainParser::AddMapArea_(const std::string& map_name, const std::string& pos_name,
	const std::string& tag_str, const luabind::object& terrain_area)
{
	return AddMapArea(map_name, pos_name, tag_str, terrain_area, kEmptyStr);
}

void GTerrainParser::TestDetectUniqueTerrainByPos(const luabind::object& positions) const
{
	using namespace luabind;
	CmnBufferVector<stCD_VecInt, 100> positions_tmp;
	for (auto& v : positions)
	{
		auto pos = LuaObjCast(v, kInvalidVecInt);
		if (pos == kInvalidVecInt)
		{
			assert(false);
			return;
		}
		positions_tmp.push_back(pos);
	}
	if (positions_tmp.size() < 2)
	{
		assert(false);
		return;
	}
	auto w = CD_Interface::GetWorld();
	if (!w)
		return;
	auto& map_data = w->map_data_;
	auto shrink_pos = map_data.Pos2ShrinkPos(positions_tmp.front());
	shrink_pos.RectifyAtRange(stVec2Int(), map_data.shrink_size_);
	auto idx = shrink_pos.y * map_data.shrink_size_.x + shrink_pos.x;
	if (IsUnValidPtr(map_data.terrain_models_))
	{
		assert(false);
		return;
	}
	auto model = map_data.terrain_models_ + idx;
	if (!model->model_.IsValid())
	{
		assert(false);
		return;
	}
	
	stTerrainTagInfo tmp_tag_info;
	tmp_tag_info.tag_str_ = model->model_.tbl_item_->model_path_;
	positions_tmp.pop_front();

	auto last_pos = map_data.ShrinkPos2Pos(shrink_pos);
	stCD_VecInt pos_tmp;
	for (auto& pos : positions_tmp)
	{
		shrink_pos = map_data.Pos2ShrinkPos(pos);
		shrink_pos.RectifyAtRange(stVec2Int(), map_data.shrink_size_);
		auto idx = shrink_pos.y * map_data.shrink_size_.x + shrink_pos.x;
		auto model = map_data.terrain_models_ + idx;
		if (!model->model_.IsValid())
		{
			assert(false);
			return;
		}
		tmp_tag_info.terrain_area_.push_back(stTerrainArea());
		pos_tmp = map_data.ShrinkPos2Pos(shrink_pos);
		auto& back = tmp_tag_info.terrain_area_.back();
		back.tag_dst_ = model->model_.tbl_item_->model_path_;
		back.max_distance_ = last_pos.Distance(pos_tmp) + 1;
		last_pos = pos_tmp;
	}
	if (ParseOneTerrainArea(tmp_tag_info, map_data, true))
	{
		LOG_O(Log_trace) << "第一个：" << tmp_tag_info.tag_str_;
		for (auto& v : tmp_tag_info.terrain_area_)
		{
			LOG_O(Log_trace) << "tag_dst:" << v.tag_dst_ << ",max_distance:" << v.max_distance_;
		}
	}
	else
	{
		LOG_O(Log_trace) << "不是唯一的地形";
	}
}

GTerrainParser::AreaConnection GTerrainParser::GetAreaConnection(const TagInfoCont& tags_info, int area_id_from, int area_id_dst)
{
	AreaConnection connection;
	for (auto& v : tags_info)
	{
		if (!v.dst_tag_info_)
			continue;
		if (v.area_id_ == area_id_from)
		{
			if (v.dst_tag_info_->area_id_ == area_id_dst)
			{
				connection.first = &v;
				connection.second = v.dst_tag_info_;
				break;
			}
		}
		else if (v.area_id_ == area_id_dst)
		{
			if (v.dst_tag_info_->area_id_ == area_id_from)
			{
				connection.first = v.dst_tag_info_;
				connection.second = &v;
				break;
			}
		}
	}
	return connection;
}

void GTerrainParser::AddMapParser(const std::string& map_name, const MapTerrainParserPtr& parser, 
	const AreaNameConnectionCont& connections)
{
	if (!parser || map_name.empty() || connections.empty())
	{
		assert(false);
		return;
	}
	auto& tags_info = map_tags_info_[map_name];
	for (auto& connect : connections)
	{
		if (connect.first.empty())
		{
			assert(false);
			continue;
		}
		stTerrainTagInfo tmp_tag_info;
		tmp_tag_info.pos_name_ = connect.first;
		tmp_tag_info.dst_pos_name_ = connect.second;
		tags_info.push_back(tmp_tag_info);
		
		if (!connect.second.empty())
		{
			tmp_tag_info.pos_name_ = connect.second;
			tmp_tag_info.dst_pos_name_ = connect.first;
			tags_info.push_back(tmp_tag_info);
		}
	}
}

bool GTerrainParser::ParseGuJinZiTa(GTerrainIterator& terrain_iterator, GTerrainPosName2Pos& out_res)
{
	auto& astar = G_AStar::GetMe();
	auto player_pos = GPlayerMe().GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	auto player_area_id = astar.GetAreaId(*player_pos);
	if (player_area_id < 0)
	{
		assert(false);
		return false;
	}
	for (terrain_iterator.First(); !terrain_iterator.IsDone(); terrain_iterator.Next())
	{
		auto model_path = terrain_iterator.CurModelPath();
		assert(model_path);
		if (StdEx::TStrCmp(model_path, L"这里填路径名") != 0)
			continue;
		auto pos = terrain_iterator.CurPos();
		auto model_area_id = astar.GetAreaId(pos);
		if (model_area_id != player_area_id)
			continue;
		AddPosNameWithPos(out_res, kMapPosName_UpLouTi, pos);
	}
	return true;
}

void GTerrainParser::AddPosNameWithPos(GTerrainPosName2Pos& out_res, const std::string& pos_name, const stCD_VecInt& pos)
{
	assert(out_res.find(pos_name) == out_res.end());
	out_res[pos_name] = pos;
}

bool GTerrainParser::DoParseConnections(TagInfoCont* map_info)
{
	CmnStaticVector<stTerrainTagInfo*, 100> tags_info_tmp;
	for (auto& tag_info : any_tags_info_)
	{
		tags_info_tmp.push_back(&tag_info);
	}
	if (map_info)
	{
		for (auto& tag_info : *map_info)
		{
			tags_info_tmp.push_back(&tag_info);
		}
	}
	for (auto tag_info : tags_info_tmp)
	{
		if (tag_info->dst_pos_name_.empty())
			continue;
		auto it = std::find_if(tags_info_tmp.begin(), tags_info_tmp.end(), [tag_info](stTerrainTagInfo* v){
			return v->pos_name_ == tag_info->dst_pos_name_;
		});
		if (it == tags_info_tmp.end())
		{
			assert(false);
			continue;
		}
		auto dst = *it;
		assert(!tag_info->dst_tag_info_ || dst == tag_info->dst_tag_info_);
		tag_info->dst_tag_info_ = dst;
		if (dst->dst_tag_info_)
		{
			assert(dst->dst_tag_info_ == tag_info);
		}
		else
		{
			dst->dst_tag_info_ = tag_info;
		}
	}
	return true;
}

bool GTerrainParser::ParseByParser(TagInfoCont* map_info, const stCD_MapData& map_data)
{
	if (!map_info)
		return true;
	auto it = map_parser_.find(GMap::GetMe().GetMapName());
	if (it == map_parser_.end()) return true;
	GTerrainIterator terrain_iterator(map_data);
	GTerrainPosName2Pos parse_res;
	it->second->ParseTerrain(terrain_iterator, parse_res);
	for (auto& res : parse_res)
	{
		SetTerrainPosNamePos(*map_info, res.first, res.second);
	}
	return true;
}

void GTerrainParser::SetTerrainPosNamePos(TagInfoCont& tags_info, const std::string& pos_name, const stCD_VecInt& pos)
{
	for (auto& v : tags_info)
	{
		if (v.pos_name_ == pos_name)
		{
			assert(!v.pos_);
			v.pos_ = pos;
			v.area_id_ = G_AStar::GetMe().GetAreaId(pos);
			assert(v.area_id_ >= 0);
			return;
		}
	}
	assert(false);
}

stTerrainTagInfo::stTerrainTagInfo()
{
	parser_ = nullptr;
	area_id_ = -1;
	dst_tag_info_ = nullptr;
}

stTerrainArea::stTerrainArea()
{
	max_distance_ = 0;
}

AreaVertexDirect::AreaVertexDirect()
{
	terrain_info_ = nullptr;
}

AreaVertexDirect::AreaVertexDirect(const stTerrainTagInfo* terrain_info)
{
	terrain_info_ = terrain_info;
}

bool AreaVertexDirect::IsValid() const
{
	if (terrain_info_ && terrain_info_->pos_)
		return true;
	return false;
}

const stCD_VecInt& AreaVertexDirect::GetValidPos() const
{
	if (terrain_info_ && terrain_info_->pos_)
		return *terrain_info_->pos_;
	assert(false);
	return kInvalidVecInt;
}

AreaVertexPtr AreaVertexDirect::GetConnectedRoom() const
{
	if (terrain_info_ && terrain_info_->dst_tag_info_)
		return AreaVertexPtr(new AreaVertexDirect(terrain_info_->dst_tag_info_));
	return nullptr;
}

const std::string& AreaVertexDirect::GetPosName() const
{
	if (terrain_info_)
		return terrain_info_->pos_name_;
	assert(false);
	return kEmptyStr;
}

GameObjBasePtrT AreaVertexDirect::GetTransitionable() const
{
	return nullptr;
}

bool GMapTraMgr::InitParseTerrain()
{
	if (!tra_direct_.ParseAreaConnections())
	{
		assert(false);
		return false;
	}
	if (!tra_guess_.ParseAreaConnections(tra_guess_.GetAreaResolver()))
	{
		assert(false);
		return false;
	}
	return true;
}

IMapTraPolicy* GMapTraMgr::GetCurPolicy()
{
	//return &tra_guess_;
	return &tra_direct_;
}

void GMapTraMgr::AddTransitionableConnect(const AreaVertexGuess& lhs, const AreaVertexGuess& rhs)
{
	tra_guess_.AddConnection(lhs, rhs);
}

bool GMapTraMgr::HasTheMapAreaName(const std::string& area_name) const
{
	if (area_name.empty())
		return false;
	auto it = map_area_infos_.find(GMap::GetMe().GetMapName());
	if (it == map_area_infos_.end())
		return false;
	for (auto& name : it->second.area_names_)
	{
		if (area_name == name)
			return true;
	}
	return false;
}

bool GMapTraMgr::HasTheMapAreaObj(const GameObjBasePtrT& obj) const
{
	auto it = map_area_infos_.find(GMap::GetMe().GetMapName());
	if (it == map_area_infos_.end())
		return false;
	assert(obj);
	const auto& obj_name = obj->GetCnName();
	for (auto& name : it->second.area_names_)
	{
		if (obj_name == name)
			return true;
	}
	return false;
}

bool GMapTraMgr::HasMapAreaByCurMap(const std::string& map_name) const
{
	auto it = map_area_infos_.find(map_name);
	if (it == map_area_infos_.end())
		return false;
	return true;
}

void GMapTraGuess::AddConnection(const AreaVertexGuess& lhs, const AreaVertexGuess& rhs)
{
	auto lhs_area_id = lhs.GetAreaId();
	if (lhs_area_id < 0)
	{
		assert(false);
		return;
	}
	auto rhs_area_id = rhs.GetAreaId();
	if (rhs_area_id < 0)
	{
		assert(false);
		return;
	}
	auto lhs_obj = lhs.GetTransitionable();
	if (lhs_obj)
	{
		lhs_obj->SetOwner(nullptr);
		lhs_obj->ResetPos();
	}
	auto rhs_obj = rhs.GetTransitionable();
	if (rhs_obj)
	{
		rhs_obj->SetOwner(nullptr);
		rhs_obj->ResetPos();
	}

	GAreaConnectionType* the_exist_connect = nullptr;
	for (auto& connect : area_connections_)
	{
		auto the_lhs_area_id = connect.first->GetAreaId();
		auto the_rhs_area_id = connect.second->GetAreaId();
		if (the_lhs_area_id == lhs_area_id)
		{
			if (the_rhs_area_id == rhs_area_id)
			{
				the_exist_connect = &connect;
				connect.first->SetTransitionable(lhs_obj);
				connect.first->SetPos(lhs.GetValidPos());
				connect.first->SetAreaState(lhs.GetPassState());
				connect.second->SetTransitionable(rhs_obj);
				connect.second->SetPos(rhs.GetValidPos());
				connect.second->SetAreaState(rhs.GetPassState());
				break;
			}
		}
		else if (the_lhs_area_id == rhs_area_id)
		{
			if (the_rhs_area_id == lhs_area_id)
			{
				the_exist_connect = &connect;
				connect.first->SetTransitionable(rhs_obj);
				connect.first->SetPos(rhs.GetValidPos());
				connect.first->SetAreaState(rhs.GetPassState());
				connect.second->SetTransitionable(lhs_obj);
				connect.second->SetPos(lhs.GetValidPos());
				connect.second->SetAreaState(lhs.GetPassState());
				break;
			}
		}
	}
	if (!the_exist_connect)
	{
		area_connections_.push_back({ AreaVertexGuessPtr(new AreaVertexGuess(lhs)),
			AreaVertexGuessPtr(new AreaVertexGuess(rhs)) });
		auto& back = area_connections_.back();
		back.first->SetDst(back.second);
		back.second->SetDst(back.first);
	}

	AddAreaConnection(lhs_area_id, rhs_area_id);
}

bool GMapTraGuess::ParseAreaConnections(const GAreaResolver& area_resolver)
{
	area_resolver_ = area_resolver;
	return true;
}

AreaVertexPtr GMapTraGuess::FindAreaByName(const std::string& area_name) const
{
	for (auto& connect : area_connections_)
	{
		if (connect.first->GetPosName() == area_name)
			return connect.first;
		else if (connect.second->GetPosName() == area_name)
			return connect.second;
	}
	return nullptr;
}

bool GMapTraGuess::GenAreaStepsImpl_(bool is_suggest, int area_from, int area_dst, GAreaStepsType& steps) const
{
	GAreaResolver::StepsContType area_steps;
	if (!area_resolver_.Resolve(area_from, area_dst, area_steps))
	{
		if (is_suggest)
			return GenUnPassedAreaSteps(steps);
		return false;
	}
	if (area_steps.empty())
	{
		assert(false);
		return false;
	}
	for (auto it = area_steps.begin(), it_end = area_steps.end(); it != it_end; ++it)
	{
		auto it_next = it + 1;
		if (it_next == it_end)
			break;
		auto connection = GetAreaConnection(*it, *it_next);
		if (connection.first == connection.second)
		{
			break;
		}
		steps.push_back(connection.first);
	}
	if (is_suggest)
	{
		if (!steps.empty())
			return true;
		return GenUnPassedAreaSteps(steps);
	}
	else
	{
		if (steps.empty())
			return false;
	}
	return true;
}

GAreaConnectionType GMapTraGuess::GetAreaConnection(int area_from, int area_dst) const
{
	GAreaConnectionType connection;
	for (auto& connect : area_connections_)
	{
		auto lhs = connect.first->GetAreaId();
		auto rhs = connect.second->GetAreaId();
		if (lhs == area_from)
		{
			if (rhs == area_dst)
			{
				connection = connect;
				break;
			}
		}
		else if (lhs == area_dst)
		{
			if (rhs == area_from)
			{
				connection.first = connect.second;
				connection.second = connect.first;
				break;
			}
		}
	}
	return connection;
}

bool GMapTraGuess::GenUnPassedAreaSteps(GAreaStepsType& steps) const
{
	auto the_door = FindUnPassedDoor();
	if (!the_door)
	{
		LOG_O(Log_debug) << "没有未通过的门了，是不是没记录下传送门？当前地图：" << GMap::GetMe().GetMapName();
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	auto player_area_id = gpm.GetAreaId();
	if (player_area_id < 0)
	{
		assert(false);
		return false;
	}
	auto dst_area_id = the_door->GetAreaId();
	if (dst_area_id < 0)
	{
		assert(false);
		return false;
	}
	if (dst_area_id != player_area_id)
	{
		if (!GenAreaStepsImpl_(false, player_area_id, dst_area_id, steps))
		{
			assert(false);
			return false;
		}
		if (steps.empty())
		{
			assert(false);
			return false;
		}
	}
	auto ptr = new AreaVertexGuess;
	if (!ptr)
	{
		assert(false);
		return false;
	}
	ptr->SetTransitionable(the_door);
	ptr->SetPos(the_door->GetBackupPos());
	ptr->SetAreaId(dst_area_id);
	steps.push_back(AreaVertexPtr(ptr));
	return true;
	//////////////////////////////////////////////////////////////////////////
	/*struct stHelper{
		GameObjBasePtrT		area_transition_;
		float				weight_;
	};
	GMapAreaShortestPathsCont<stHelper> area_transitions;
	auto& remember_objs = gpm.GetRememberedObjs();
	auto& tra_mgr = GMapTraMgr::GetMe();
	auto& path_mark = GPathMark::GetMe();
	auto player_pos = gpm.GetPos();
	if (!player_pos)
	{
		assert(false);
		return false;
	}
	for (auto& obj : remember_objs.GetObjs())
	{
		if (obj->GetObjType() != kGOT_AreaTransition)
			continue;
		if (!tra_mgr.HasTheMapAreaObj(obj))
			continue;
		if (player_area_id != obj->GetAreaId())
			continue;
		path_mark.GetSuggestPos(*player_pos, player_area_id, )
		stHelper tmp = { obj, obj->WeightDistance(*player_pos) };
		auto vertex = FindVertex(obj);
		if (vertex)
		{
			tmp.weight_ += 1000;
			auto connected_vertex = vertex->GetConnectedRoom();
			if (connected_vertex)
			{
				auto the_area_id = astar.GetAreaId(connected_vertex->GetValidPos());
				stCD_VecInt pos_tmp;
				if (!GPathMark::GetMe().GetSuggestPos(*player_pos, the_area_id, pos_tmp))
					tmp.weight_ += 2000;
			}
			else
				assert(false);

		}
		area_transitions.push_back(tmp);
	}
	if (area_transitions.empty())
	{
		LOG_O(Log_debug) << "是不是没记录下传送门？当前地图：" << GMap::GetMe().GetMapName();
		return false;
	}
	std::sort(area_transitions.begin(), area_transitions.end(), []
		(const stHelper& lhs, const stHelper& rhs){
		return lhs.weight_ < rhs.weight_;
	});

	auto ptr = new AreaVertexGuess;
	if (!ptr)
	{
		assert(false);
		return false;
	}
	auto the_obj = area_transitions.front().area_transition_;
	ptr->SetTransitionable(the_obj);
	ptr->SetPos(the_obj->GetBackupPos());
	steps.push_back(AreaVertexPtr(ptr));
	return true;*/
}

AreaVertexGuessPtr GMapTraGuess::FindVertex(const GameObjBasePtrT& obj) const
{
	if (!obj)
	{
		assert(false);
		return nullptr;
	}
	auto obj_id = obj->GetGameId();
	for (auto& connect : area_connections_)
	{
		auto the_obj = connect.first->GetTransitionable();
		if (the_obj)
		{
			if (the_obj->GetGameId() == obj_id)
				return connect.first;
		}
		the_obj = connect.second->GetTransitionable();
		if (the_obj)
		{
			if (the_obj->GetGameId() == obj_id)
				return connect.second;
		}
	}
	return nullptr;
}

AreaVertexGuessPtr GMapTraGuess::FindVertex(const std::string& obj_name) const
{
	if (obj_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for (auto& connect : area_connections_)
	{
		auto the_obj = connect.first->GetTransitionable();
		if (the_obj)
		{
			if (the_obj->GetCnName() == obj_name)
				return connect.first;
		}
		the_obj = connect.second->GetTransitionable();
		if (the_obj)
		{
			if (the_obj->GetCnName() == obj_name)
				return connect.second;
		}
	}
	return nullptr;
}

GameObjBasePtrT GMapTraGuess::FindUnPassedDoor() const
{
	auto& gpm = GPlayerMe::GetMe();
	const auto& remember_objs = gpm.GetRememberedObjs();
	auto& tra_mgr = GMapTraMgr::GetMe();
	for (auto& obj : remember_objs.GetObjs())
	{
		if (obj->GetObjType() != kGOT_AreaTransition)
			continue;
		if (!tra_mgr.HasTheMapAreaObj(obj))
			continue;
		auto vertex = FindVertex(obj);
		if (!vertex && GMap::GetMe().GetMapName() == kMapName_GangwanBridgey)
			vertex = FindVertex(obj->GetCnName());
		if (vertex)
		{
			if (vertex->GetPassState() == kAPS_CanPass)
				return obj;
			else
				continue;
		}
		else
		{
			if (obj->GetAreaId() > 0 && obj->GetAreaId() != gpm.GetAreaId())
				continue;
		}
		return obj;
	}
	return nullptr;
}

bool GMapTraGuess::GenAreaStepsImpl(int area_from, int area_dst, GAreaStepsType& steps) const
{
	return GenAreaStepsImpl_(is_suggest_, area_from, area_dst, steps);
}

AreaVertexPtr GMapTraGuess::FindAreaById(int obj_id) const
{
	if (obj_id < 0)
		return nullptr;
	for (auto& connect : area_connections_)
	{
		auto transition = connect.first->GetTransitionable();
		if (transition && transition->GetGameId() == obj_id)
			return connect.first;
		transition = connect.second->GetTransitionable();
		if (transition && transition->GetGameId() == obj_id)
			return connect.second;
	}
	return nullptr;
}

const std::string& GMapTraGuess::GetRoomNameByAreaId(int area_id) const
{
	for (auto& connect : area_connections_)
	{
		if (connect.first->GetAreaId() == area_id)
			return connect.second->GetPosName();
		if (connect.second->GetAreaId() == area_id)
			return connect.first->GetPosName();
	}
	return kEmptyStr;
}

const stCD_VecInt& AreaVertexGuess::GetValidPos() const
{
	if (transitionable_pos_.AnyGreater(0)) return transitionable_pos_;
	assert(false);
	if (transitionable_)
		return transitionable_->GetBackupPos();
	assert(false);
	return transitionable_pos_;
}

int AreaVertexGuess::GetAreaId() const
{
	if (area_id_ >= 0) return area_id_;
	if (transitionable_)
		area_id_ = transitionable_->GetAreaId();
	else
		area_id_ = G_AStar::GetMe().GetAreaId(transitionable_pos_);
	assert(area_id_ >= 0);
	return area_id_;
}

AreaVertexGuess::AreaVertexGuess()
{
	area_id_ = -1;
	pass_state_ = kAPS_FristPass;
}

AreaVertexPtr AreaVertexGuess::GetConnectedRoom() const
{
	if (dst_vertex_.expired())
	{
		assert(false);
		return nullptr;
	}
	return dst_vertex_.lock();
}

void AreaVertexGuess::SetDst(const AreaVertexGuessPtr& dst)
{
	dst_vertex_ = dst.first;
}

void AreaVertexGuess::SetTransitionable(const GameObjBasePtrT& obj)
{
	transitionable_ = obj;
}

void AreaVertexGuess::SetPos(const stCD_VecInt& pos)
{
	transitionable_pos_ = pos;
}

bool AreaVertexGuess::IsValid() const
{
	if (transitionable_pos_.AnyLess(1))
	{
		assert(false);
		return false;
	}
	return true;
}

const std::string& AreaVertexGuess::GetPosName() const
{
	if (transitionable_)
		return transitionable_->GetCnName();
	//assert(false);
	return kEmptyStr;
}

void AreaVertexGuess::SetAreaId(int area_id)
{
	area_id_ = area_id;
}

GTerrainIterator::GTerrainIterator(const stCD_MapData& map_data) : map_data_(map_data)
{
	cur_idx_ = 0;
	max_idx_ = map_data.shrink_size_.x * map_data.shrink_size_.y;
	assert(max_idx_ > 0);
}

void GTerrainIterator::First()
{
	cur_idx_ = 0;
	NextValid();
}

bool GTerrainIterator::IsDone() const
{
	return cur_idx_ >= max_idx_;
}

void GTerrainIterator::Next()
{
	++cur_idx_;
	NextValid();
}

stCD_VecInt GTerrainIterator::CurShrinkPos() const
{
	assert(cur_idx_ < max_idx_);
	return stCD_VecInt(cur_idx_ % map_data_.shrink_size_.x, cur_idx_ / map_data_.shrink_size_.x);
}

stCD_VecInt GTerrainIterator::CurPos() const
{
	return map_data_.ShrinkPos2Pos(CurShrinkPos());
}

stCD_MapDataTerrainModel* GTerrainIterator::CurTerrain() const
{
	assert(cur_idx_ < max_idx_);
	assert(map_data_.terrain_models_);
	return map_data_.terrain_models_ + cur_idx_;
}

pt_cwsz GTerrainIterator::CurModelPath() const
{
	auto cur_terrain = CurTerrain();
	assert(cur_terrain);
	if (cur_terrain->model_.tbl_item_)
		return cur_terrain->model_.tbl_item_->model_path_;
	return nullptr;
}

void GTerrainIterator::NextValid()
{
	assert(map_data_.terrain_models_);
	while (true)
	{
		if (cur_idx_ >= max_idx_)
			break;
		auto cur_model = map_data_.terrain_models_ + cur_idx_;
		if (cur_model->model_.tbl_item_)
		{
			assert(cur_model->model_.tbl_item_->model_path_);
			break;
		}
		++cur_idx_;
	}
}

bool GLabyrinthTrialDlg::ActiveButton() const
{
	if (!GWndExecSync([](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			LOG_O(Log_error) << "试炼迷宫 更新出错";
			assert(false);
			return false;
		}
		auto ui_Labyrinth_dlg_ = main_ui->GetDlgLabyrinthSelect();
		if (!ui_Labyrinth_dlg_)
		{
			LOG_O(Log_error) << "试炼选择面板没有打开吧";
			assert(false);
			return false;
		}
		return ui_Labyrinth_dlg_->PressActive();
	}))
	{
		assert(false);
		return false;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_AreaTransition));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	FilterTransferTown filter(obj_mgr->GetFilter());
	if (!TimerDo(100, 1000 * 60, [&obj_mgr](){
		if (!obj_mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		return !!obj_mgr->GetFirstObj(); }))
	{
		assert(false);
		return false;
	}
		return true;
}

bool GLabyrinthTrialDlg::RebuildAllImpl()
{
	auto main_ui = CD_Interface::GetInGameMainUi();
	if (!main_ui)
	{
		LOG_O(Log_error) << "试炼迷宫 更新出错";
		assert(false);
		return false;
	}
	auto ui_Labyrinth_dlg_ = main_ui->GetDlgLabyrinthSelect();
	if (!ui_Labyrinth_dlg_)
	{
		LOG_O(Log_error) << "试炼选择面板没有打开吧";
		assert(false);
		return nullptr;
	}
	if (!ui_Labyrinth_dlg_->list_)
	{
		assert(false);
		return false;
	}
	labyrinth_Dlg_List_.clear();
	ui_Labyrinth_dlg_->list_->TraListItem([this](stCD_UiLabyrinthList::stListItem* item){
		GLabyrinthItem labyrinth_item;
		labyrinth_item.SetLabyrinthUiItem(item);
		if (!labyrinth_item.Update())
		{
			assert(false);
			return false;
		}
		labyrinth_Dlg_List_.push_back(labyrinth_item);
		return true;
	});
	return true;
}

void GLabyrinthTrialDlg::DebugInfo() const
{
	for (auto& v : labyrinth_Dlg_List_)
	{
		LOG_O(Log_debug) << "------------------------------------";
		v.DebugInfo();
	}
}

bool GLabyrinthTrialDlg::SelectLabyrinthByLevel(pt_dword select_level) const
{
	if (labyrinth_Dlg_List_.empty())
	{
		assert(false);
		return false;
	}
	for (auto& v : labyrinth_Dlg_List_)
	{
		if (select_level != v.GetLevel())
			continue;
		if (!v.GetActivedState())
			continue;
		if(!v.SeletLabyrinth())
			continue;
		return ActiveButton();
	}
	return false;
}

void GLabyrinthItem::SetLabyrinthUiItem(stCD_UiLabyrinthList::stListItem* ui_item)
{
	item_ = ui_item;
}

bool GLabyrinthItem::UpdateImpl()
{
	if (!item_)
	{
		assert(false);
		return false;
	}
	auto ui_item = item_->ui_item_;
	if (!ui_item)
	{
		assert(false);
		return false;
	}
	if (!ui_item->data_labyrinths_.IsValid())
	{
		assert(false);
		return false;
	}
	min_level_ = ui_item->data_labyrinths_.tbl_item_->lvl_;
	is_actived_ = ui_item->IsActived();
	auto trial_cont = ui_item->GetTrialInfoCont(kGLT_All);
	if (trial_cont.empty())
	{
		assert(false);
		return false;
	}
	for (auto& v : trial_cont)
	{
		if (!v.world_area_)
		{
			assert(false);
			continue;
		}
		if (!CD_SafeName(v.world_area_->ch_map_name_))
		{
			assert(false);
			continue;
		}
		GClass::stLabyrinthMapState map_Info;
		map_Info.is_completed_ = v.is_completed_;
		map_Info.diff_lvl_ = v.world_area_->GetDiffLvl();
		map_Info.map_name_ = WideCharToGbk(v.world_area_->ch_map_name_);
		map_Labyrinth_.push_back(map_Info);
	}
	return true;
}

void GLabyrinthItem::DebugInfo() const
{
	LOG_O(Log_debug) << "迷宫等级:" << min_level_ << ",是否激活:" << is_actived_;
	for (auto& v : map_Labyrinth_)
		LOG_O(Log_debug) << "试炼地图:" << v.map_name_ << ",是否完成:" << v.is_completed_;

}

bool GLabyrinthItem::SeletLabyrinth() const
{
	return GWndExecSync([this](){
		if (!item_)
		{
			assert(false);
			return false;
		}
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			LOG_O(Log_error) << "试炼迷宫 主面板更新出错";
			assert(false);
			return false;
		}
		auto ui_Labyrinth_dlg_ = main_ui->GetDlgLabyrinthSelect();
		if (!ui_Labyrinth_dlg_)
		{
			LOG_O(Log_error) << "试炼选择面板没有打开吧";
			assert(false);
			return false;
		}
		if (!ui_Labyrinth_dlg_->list_)
		{
			assert(false);
			return false;
		}
		if (!ui_Labyrinth_dlg_->list_->SelectItem(item_))
		{
			assert(false);
			return false;
		}
		return true;
	});
}

void GTrialPlaqueDlg::DebugInfo() const
{
	LOG_O(Log_debug) << "试炼面板:---------------------------" ;
	for (auto& v : labyrinth_Plaque_List_)
		LOG_O(Log_debug) << "试炼地图:" << v.map_name_ << ",是否完成:" << v.is_completed_;
}

bool GTrialPlaqueDlg::RebuildAllImpl()
{
	labyrinth_Plaque_List_.clear();
	auto main_ui = CD_Interface::GetInGameMainUi();
	if (!main_ui)
	{
		LOG_O(Log_error) << "试炼面板 更新出错";
		assert(false);
		return false;
	}
	auto ui_Labyrinth_dlg_ = main_ui->GetDlgTrialPlaque();
	if (!ui_Labyrinth_dlg_)
	{
		LOG_O(Log_error) << "试炼显示面板没有打开吧";
		assert(false);
		return nullptr;
	}
	auto labyrinth_date = ui_Labyrinth_dlg_->GetTrialInfoCont();
	if (labyrinth_date.empty())
	{
		assert(false);
		return false;
	}
	for (auto& v : labyrinth_date)
	{
		if (!v.world_area_)
		{
			assert(false);
			continue;
		}
		GClass::stLabyrinthMapState map_Info;
		map_Info.is_completed_ = v.is_complete_;
		map_Info.diff_lvl_ = v.world_area_->GetDiffLvl();
		map_Info.map_name_ = WideCharToGbk(v.world_area_->ch_map_name_);
		labyrinth_Plaque_List_.push_back(map_Info);
	}
	return true;
}

const GClass::stLabyrinthMapState* GTrialPlaqueDlg::GetUndoneMap() const
{
	if (labyrinth_Plaque_List_.empty())
		return nullptr;

	for (auto& v : labyrinth_Plaque_List_)
	{
		if (v.is_completed_)
			continue;
		return &v;
	}
	return nullptr;
}

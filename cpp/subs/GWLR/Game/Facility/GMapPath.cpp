#include "stdafx.h"
#include "GMapPath.h"
#include "GConst.h"
#include "GCloneMap.h"
#include "GPlayer.h"
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <Common/CmnException.h>
#include "../Preprocessor.h"
#include <Script/LuaScript.h>

/*
bool GSP_Village::Construct(){
	return false;
}
 bool GSP_FbRoom::Construct(){
	 return false;
 }
 bool GSP_Resolver::VillageMove( const std::string& village_id_src, const std::string& village_id_dst, GSP_Stage::StepsT& steps_out ){
	 return false;
 }
 GSP_Stage* GSP_Resolver::FindStage( const std::string& stage_id ){
	 return nullptr;
 }
 GSP_Village::GSP_Village(){}*/

bool GSP_Village::Construct()
{
	if (is_init_)
		return true;
	GSP_Gate gate;
	gate.next_stage_ = nullptr;

	gate.wrap_gate_id_ = kMoveCmd_XiMeiLun2LongZhiHu;
	gate.next_stage_name_ = kVillageName_LongZhiHu;
	AddStage_Gate(kVillageName_XiMeiLunShanLu, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(438.291, 376.563, 84.3457));

	gate.wrap_gate_id_ = kMoveCmd_LongZhiHu2XiMeiLun;
	gate.next_stage_name_ = kVillageName_XiMeiLunShanLu;
	AddStage_Gate(kVillageName_LongZhiHu, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(498.588, 529.345, 120.47));

	gate.wrap_gate_id_ = kMoveCmd_XiMeiLun2MiLaDeCun;
	gate.next_stage_name_ = kVillageName_MiLaDeCun;
	AddStage_Gate(kVillageName_XiMeiLunShanLu, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(366.28, 372.544, 89.3575));

	gate.wrap_gate_id_ = kMoveCmd_MiLaDe2XiMeiLun;
	gate.next_stage_name_ = kVillageName_XiMeiLunShanLu;
	AddStage_Gate(kVillageName_MiLaDeCun, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(694.806, 672.582, 134.875));

	gate.wrap_gate_id_ = kMoveCmd_LongZhiHu2MeiJie;
	gate.next_stage_name_ = kVillageName_MeiJiePoErTan;
	AddStage_Gate(kVillageName_LongZhiHu, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(552.135, 478.813, 117.335));

	gate.wrap_gate_id_ = kMoveCmd_MeiJie2LongZhi;
	gate.next_stage_name_ = kVillageName_LongZhiHu;
	AddStage_Gate(kVillageName_MeiJiePoErTan, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(1318.13, 1274.85, 14.2283));

	gate.wrap_gate_id_ = kMoveCmd_MeiJie2RaoYueLin;
	gate.next_stage_name_ = kVillageName_YaoYueLin;
	AddStage_Gate(kVillageName_MeiJiePoErTan, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(1159.51, 1175.21, 32.3919));

	gate.wrap_gate_id_ = kMoveCmd_RaoYueLin2MeiJie;
	gate.next_stage_name_ = kVillageName_MeiJiePoErTan;
	AddStage_Gate(kVillageName_YaoYueLin, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(499.771, 400.16, 47.4333));

	gate.wrap_gate_id_ = kMoveCmd_YaoYueLin2YinBaiAiKou;
	gate.next_stage_name_ = kVillageName_YinBaiAiKou;
	AddStage_Gate(kVillageName_YaoYueLin, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(550.332, 388.957, 49.125));

	gate.wrap_gate_id_ = kMoveCmd_YinBaiAiKou2YaoYueLin;
	gate.next_stage_name_ = kVillageName_YaoYueLin;
	AddStage_Gate(kVillageName_YinBaiAiKou, gate);
	AddGate2PosInfo(gate.wrap_gate_id_, GType::stGameVector3(553.829, 529.623, 30.593));

	BindGate();
	is_init_ = true;
	//////////////////////////////////////////////////////////////////////////
	village_id_2_name_[150101] = kVillageName_XiMeiLunShanLu;
	village_id_2_name_[150201] = kVillageName_LongZhiHu;
	village_id_2_name_[180101] = kVillageName_MiLaDeCun;
	village_id_2_name_[180201] = kVillageName_MeiJiePoErTan;
	village_id_2_name_[150301] = kVillageName_YaoYueLin;
	village_id_2_name_[150401] = kVillageName_YinBaiAiKou;
	//////////////////////////////////////////////////////////////////////////
	village_fb_pos_[kVillageName_XiMeiLunShanLu] = GType::stGameVector3(413.38, 408.529, 84.7644);
	village_fb_pos_[kVillageName_LongZhiHu] = GType::stGameVector3(466.683, 479.662, 115.875);
	village_fb_pos_[kVillageName_YaoYueLin] = GType::stGameVector3(498.899, 462.377, 50.8017);
	village_fb_pos_[kVillageName_YinBaiAiKou] = GType::stGameVector3(508.889, 487.39, 29.5);
	//kVillageName_MeiJiePoErTan 这个城镇也可以进一些副本，但还不支持
	//village_fb_pos_[kVillageName_MeiJiePoErTan] = GType::stGameVector3(498.899, 462.377, 50.8017);
	//////////////////////////////////////////////////////////////////////////
	return true;
}

GSP_Village::GSP_Village()
{
	is_init_ = false;
}

bool GSP_Village::AddGate2PosInfo( const std::string& gate_name, const GType::stGameVector3& pos )
{
	if ( gate_name.empty() )
	{
		assert(false);
		return false;
	}
	auto it = gate_name_2_pos_.find(gate_name);
	if ( it != gate_name_2_pos_.end() )
	{
		assert(false);
		return false;
	}
	gate_name_2_pos_[gate_name] = pos;
	return true;
}

boost::optional<GType::stGameVector3> GSP_Village::Gate2PosInfo( const std::string& gate_name ) const
{
	boost::optional<GType::stGameVector3> res;
	if ( gate_name.empty() )
	{
		assert(false);
		return res;
	}
	auto it = gate_name_2_pos_.find(gate_name);
	if ( it == gate_name_2_pos_.end() )
	{
		assert(false);
		return res;
	}
	res = it->second;
	return res;
}

boost::optional<std::string> GSP_Village::VillageId2Name( int village_id ) const
{
	boost::optional<std::string> res;
	auto it = village_id_2_name_.find(village_id);
	if ( it == village_id_2_name_.end() )
		return res;
	res = it->second;
	return res;
}

boost::optional<GType::stGameVector3> GSP_Village::VillageFbPos( const std::string& village_name ) const
{
	boost::optional<GType::stGameVector3> res;
	auto it = village_fb_pos_.find(village_name);
	if ( it == village_fb_pos_.end() )
		return res;
	res = it->second;
	return res;
}

bool GSP_Resolver::VillageMove( const std::string& village_id_src, const std::string& village_id_dst, GSP_Stage::StepsT& steps_out )
{
	if ( village_id_src == village_id_dst )
		return true;
	auto village_src = FindStage(village_id_src);
	if ( !village_src )
	{
		assert(false);
		return false;
	}
	auto village_dst = FindStage(village_id_dst);
	if ( !village_dst )
	{
		assert(false);
		return false;
	}
	return village_src->MoveTo(village_dst, steps_out);
}

GSP_Stage* GSP_Resolver::FindStage( const std::string& stage_id )
{
	auto it = std::find_if(stages_.begin(), stages_.end(), [&stage_id]( const GSP_Stage::StagePtr& stage ){
		return stage_id == stage->stage_id_;
	});
	if ( it != stages_.end() )
		return it->get();
	return nullptr;
}

void GSP_Resolver::AddStage_Gate( const std::string& stage_id, const GSP_Gate& gate )
{
	assert(!stage_id.empty());
	auto stage = FindStage(stage_id);
	if (stage)
		stage->wrap_gates_.push_back(gate);
	else
	{
		StagePtr stage_ptr(new GSP_Stage);
		stage_ptr->wrap_gates_.push_back(gate);
		stage_ptr->stage_id_ = stage_id;
		stages_.push_back(stage_ptr);
	}
}

void GSP_Resolver::BindGate()
{
	auto stages_tmp = stages_;
	for ( auto& stage : stages_tmp )
	{
		assert(stage);
		for ( auto& gate : stage->wrap_gates_ )
		{
			gate.next_stage_ = FindStage(gate.next_stage_name_);
			if ( !gate.next_stage_ )
			{
				StagePtr stage_tmp(new GSP_Stage);
				if ( !stage_tmp )
				{
					assert(false);
					return;
				}
				stage_tmp->stage_id_ = gate.next_stage_name_;
				stages_.push_back(stage_tmp);
				gate.next_stage_ = FindStage(gate.next_stage_name_);
				if ( !gate.next_stage_ )
				{
					assert(false);
					return;
				}
			}
		}
	}
}

void GSP_Resolver::DebugInfo() const
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateCurMapInfo_Notify();
	auto& fb_mgr = FbDoorsMgr::GetMe();
	fb_mgr.RebuildAll();
	struct stHelper{
		FbDoor*		obj_door_;
		int			use_cnt_;
	};
	std::vector<stHelper> door_objs;
	fb_mgr.Traverse([&door_objs](GameObjBase& obj){
		auto the_door = obj.CastTo<FbDoor>();
		if (!the_door)
		{
			assert(false);
			return true;
		}
		stHelper tmp = {
			the_door, 0
		};
		door_objs.push_back(tmp);
		return true;
	});


	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "当前地图的模板ID:" << gpm.GetCurTemplateMapId() << " 地图模板名：" << gpm.GetCurTemplateMapName() << " 地图ID:" << gpm.GetCurMapId() << " 地图名：" << gpm.GetCurMapName() << ", 有[" << stages_.size() << "]个房间：";
	for ( auto& v : stages_ )
	{
		LOG_O(Log_trace) << "房间id：" << v->stage_id_;
		for ( auto& gate : v->wrap_gates_ )
		{
			auto it = std::find_if(door_objs.begin(), door_objs.end(), [&gate]( const stHelper& h ){
				return gate.wrap_gate_id_ == h.obj_door_->GetName();
			});
			if ( it == door_objs.end() )
			{
				LOG_O(Log_debug) << "此地图中并没有这样名字的门：" << gate.wrap_gate_id_;
				continue;
			}
			++it->use_cnt_;
			const auto& pos = it->obj_door_->GetPos();
			LOG_O(Log_trace) << "门：" << gate.wrap_gate_id_ << " 目标房间：" << gate.next_stage_name_ << " 门坐标:" << pos;
		}
	}

	LOG_O(Log_trace) << "门的异常匹配次数：";
	for ( auto& v : door_objs )
	{
		if ( v.use_cnt_ != 1 )
		{
			LOG_O(Log_debug) << "门：" << v.obj_door_->GetName() << " 门坐标：" << v.obj_door_->GetPos() << " 匹配次数：" << v.use_cnt_;
		}
	}
}

bool GSP_Stage::MoveTo( const GSP_Stage* dst_stage, StepsT& steps_out ) const
{
	if ( !dst_stage )
	{
		assert(false);
		return false;
	}
	steps_out.clear();
	if ( this == dst_stage )
		return true;
	StepsT cur_steps;
	GSP_Step step_tmp;
	for ( const auto& gate : wrap_gates_ )
	{
		assert(cur_steps.empty());
		if ( 1 == steps_out.size() )
			break;
		
		step_tmp.step_gate_ = &gate;
		step_tmp.step_stage_ = this;
		cur_steps.push_back(step_tmp);
		gate.next_stage_->MoveToNext(cur_steps, dst_stage, steps_out);
	}
	return !steps_out.empty();
}

void GSP_Stage::MoveToNext( StepsT& cur_steps, const GSP_Stage* dst_stage, StepsT& steps_out ) const
{
	GSP_Step step_tmp;
	if ( this == dst_stage )
	{
		steps_out = cur_steps;
		assert(!steps_out.empty());
		step_tmp.step_gate_ = nullptr;
		step_tmp.step_stage_ = this;
		steps_out.push_back(step_tmp);
	}
	else
	{
		auto it_end = cur_steps.end();
		auto it = std::find_if(cur_steps.begin(), it_end, [this]( const GSP_Step& step ){
			return this == step.step_stage_;
		});
		if ( it == it_end )
		{
			for ( const auto& gate : wrap_gates_ )
			{
				const auto kRealStepsCnt = steps_out.size();
				if ( kRealStepsCnt > 0 )
				{
					if ( cur_steps.size() >= kRealStepsCnt - 1 )
						break;
				}
				step_tmp.step_gate_ = &gate;
				step_tmp.step_stage_ = this;
				cur_steps.push_back(step_tmp);
				gate.next_stage_->MoveToNext(cur_steps, dst_stage, steps_out);
			}
		}
	}
	assert(!cur_steps.empty());
	cur_steps.pop_back();
}

bool GSP_FbRoom::Construct()
{
	return ConstructImpl();
}

bool GSP_FbRoom::ConstructImpl()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateCurMapInfo_Notify();

	auto& fb_mgr = FbDoorsMgr::GetMe();
	if ( !fb_mgr.RebuildAll() )
	{
		assert(false);
		return false;
	}
	stages_.clear();	
	auto& link_mgr = GDoorLinkMgr::GetMe();

	GSP_Gate gate;
	gate.next_stage_ = nullptr;	

	fb_mgr.Traverse([&link_mgr, this, &gpm, &gate](GameObjBase& obj){
		auto door_ptr = obj.CastTo<FbDoor>();
		if (!door_ptr)
		{
			assert(false);
			return true;
		}
		auto dst_id = link_mgr.FindTheDstRoomId(gpm.GetCurTemplateMapId(), door_ptr->GetOwnerRoomIdx(), door_ptr->GetName());
		if (!dst_id)
		{
			LOG_O(Log_debug) << "构建\"" << gpm.GetCurMapName() << "\"副本门的链接信息时，房间号[" << door_ptr->GetOwnerRoomIdx() << "]，门的名字\"" << door_ptr->GetName() << "\"，没有目标房间号的链接";
			return true;
		}
		gate.wrap_gate_id_ = door_ptr->GetName();
		gate.next_stage_name_ = std::to_string((int)*dst_id);
		AddStage_Gate(std::to_string(door_ptr->GetOwnerRoomIdx()), gate);
		return true;
	});
	BindGate();
	return true;
}

bool GSP_FbRoom::ForceConstruct()
{
	return ConstructImpl();
}

bool GSP_StagePosPath::PosInfo::AddVertex( int vertex_id, float x, float y, float z )
{
	using namespace GType;
	if ( vertex_id < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "顶点ID不能为负，顶点ID：" << vertex_id << "  坐标(" << x << ", " << y << ", " << z << ")";
		return false;
	}
	auto it = FindVertexById(vertex_id);
	if ( it.first != it.second )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "已存在顶点ID，此次添加顶点失败，顶点ID：" << vertex_id << "  坐标(" << x << ", " << y << ", " << z << ")";
		//assert(false);
		return false;
	}
	stVertex vertex;
	vertex.id_ = vertex_id;
	vertex.pos_ = stGameVector3(x, y, z);
	it = FindVertexByPos(vertex.pos_);
	if ( it.first != it.second )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "已存在顶点坐标信息，此次添加顶点失败，顶点ID：" << vertex_id << "  坐标(" << x << ", " << y << ", " << z << ")";
		//assert(false);
		return false;
	}
	boost::add_vertex(vertex, pos_graph_);

	auto sink = GSP_StagePosPath::GetMe().stage_pos_info_sink_;
	if (sink)
		sink->OnAddVertex(sink_id_, vertex_id, x, y, z);
	return true;
}

bool GSP_StagePosPath::PosInfo::AddEdge( int vertex_id_from, int vertex_id_to )
{
	if ( vertex_id_to == vertex_id_from )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "添加边失败,顶点ID不能相同，ID：" << vertex_id_from;
		//assert(false);
		return false;
	}
	auto it_from = FindVertexById(vertex_id_from);
	if ( it_from.first == it_from.second )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "不存在源顶点ID，添加边失败，源顶点ID：" << vertex_id_from << " 目标顶点ID：" << vertex_id_to;
		//assert(false);
		return false;
	}
	auto it_to = FindVertexById(vertex_id_to);
	if ( it_to.first == it_to.second )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "不存在目标顶点ID，添加边失败，源顶点ID：" << vertex_id_from << " 目标顶点ID：" << vertex_id_to;
		//assert(false);
		return false;
	}
	auto the_edge = FindEdgeByVertexIter(*it_from.first, *it_to.first);
	if ( the_edge.first != the_edge.second )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "添加边失败，已存在该边了1, vertex_id_from:" << vertex_id_from << "  vertex_id_to:" << vertex_id_to;
		return false;
	}
	the_edge = FindEdgeByVertexIter(*it_to.first, *it_from.first);
	if ( the_edge.first != the_edge.second )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "添加边失败，已存在该边了2,两个顶点之间只需要添加一次边， vertex_id_from:" << vertex_id_from << "  vertex_id_to:" << vertex_id_to;
		return false;
	}
	const auto& v_from = pos_graph_[*it_from.first].pos_;
	const auto& v_to = pos_graph_[*it_to.first].pos_;
	boost::add_edge(*it_from.first, *it_to.first, v_from.Distance3d(v_to), pos_graph_);

	auto sink = GSP_StagePosPath::GetMe().stage_pos_info_sink_;
	if (sink)
		sink->OnAddEdge(sink_id_, vertex_id_from, vertex_id_to);
	return true;
}

GSP_StagePosPath::VertexIterPairT GSP_StagePosPath::PosInfo::FindVertexById( int vertex_id )
{
	for ( auto it = boost::vertices(pos_graph_); it.first != it.second; ++it.first) {
		const auto& v = *it.first;
		if ( pos_graph_[v].id_ == vertex_id )
			return it;
	}
	return std::make_pair(VertexEnd(), VertexEnd());
}

GSP_StagePosPath::VertexIterPairT GSP_StagePosPath::PosInfo::FindVertexByPos( const GType::stGameVector3& pos )
{
	for ( auto it = boost::vertices(pos_graph_); it.first != it.second; ++it.first) {
		const auto& v = *it.first;
		if (pos_graph_[v].pos_.FloatAllEqual(pos))
			return it;
	}
	return std::make_pair(VertexEnd(), VertexEnd());
}

GSP_StagePosPath::EdgeIterPairT GSP_StagePosPath::PosInfo::FindEdgeByVertexIter( const VertexDescT& vertex_desc_from, const VertexDescT& vertex_desc_to )
{
	for ( auto it = boost::edges(pos_graph_); it.first != it.second; ++it.first )
	{
		const EdgeDescT& e = *it.first;
		const VertexDescT& u = boost::source(e, pos_graph_);
		if ( u != vertex_desc_from )
			continue;
		const VertexDescT& v = boost::target(e, pos_graph_);
		if ( v == vertex_desc_to )
			return it;
	}
	return std::make_pair(EdgeEnd(), EdgeEnd());
}

GSP_StagePosPath::EdgeIterT GSP_StagePosPath::PosInfo::EdgeEnd()
{
	return EdgeIterT(pos_graph_.m_edges.end());
}

GSP_StagePosPath::VertexIterT GSP_StagePosPath::PosInfo::VertexEnd()
{
	return pos_graph_.vertex_set().end();
}

//////////////////////////////////////////////////////////////////////////
template<typename PredecessorMap_, typename PropertyIndexT>
class RecordEdgePredecessors__ : public boost::dijkstra_visitor<>
{
public:
	explicit RecordEdgePredecessors__( PredecessorMap_& p, const PropertyIndexT& p_i )
		: m_predecessor(p), p_i_(p_i){}

	template <class Edge, class Graph>
	void edge_relaxed( const Edge& e, Graph& g ){
		const auto& target_desc = boost::target(e, g);
		int target_idx = p_i_[target_desc];
		int the_size = m_predecessor.size();
		if ( the_size <= target_idx )
		{
			target_idx -= the_size;
			do{
				typedef typename PredecessorMap_::value_type ValueType;
				m_predecessor.push_back(ValueType());
				--the_size;
				--target_idx;
			}
			while ( the_size > 0 || target_idx >= 0 );
		}
		boost::put(&*m_predecessor.begin(), target_desc, e);
	}

protected:
	PredecessorMap_& m_predecessor;
	const PropertyIndexT& p_i_;
};

template <typename PredecessorMap, typename PropertyIndexT>
RecordEdgePredecessors__<PredecessorMap, PropertyIndexT> make_edge_predecessor_recorder__( PredecessorMap& p, const PropertyIndexT& p_i ) 
{
	return RecordEdgePredecessors__<PredecessorMap, PropertyIndexT>(p, p_i);
}

/*
template<typename EdgePredecessorMap, typename VertexPredecessorMap, typename EdgeIndexMap, typename VertexDescriptor>
std::vector<typename boost::property_traits<EdgeIndexMap>::value_type> translate_to_edges_index( const EdgePredecessorMap& em, const VertexPredecessorMap& vm, const EdgeIndexMap& im, VertexDescriptor dest )
{
	std::vector<typename boost::property_traits<EdgeIndexMap>::value_type> res;
	while( vm[dest] != dest )
	{
		res.push_back(im[em[dest]]);
		dest = vm[dest];  
	}
	return res;
}*/
template<typename EdgePredecessorMap, typename VertexPredecessorMap, typename VertexDescriptor>
GSP_StagePosPath::EdgeDescContT translate_to_edges_descriptor__( const EdgePredecessorMap& em, const VertexPredecessorMap& vm, VertexDescriptor dest )
{
	GSP_StagePosPath::EdgeDescContT res;
	while( vm[dest] != dest )
	{
		res.push_back(em[dest]);
		dest = vm[dest];  
	}
	std::reverse(res.begin(), res.end());
	return res;
}

GSP_StagePosPath::EdgeDescContT GSP_StagePosPath::PosInfo::DijkstraShortestPaths_Edges( int vertex_id_from, int vertex_id_to )
{
	using namespace boost;
	const auto& vertex_desc_from = FindVertexById(vertex_id_from);
	if ( vertex_desc_from.first == vertex_desc_from.second )
	{
		assert(false);
		return EdgeDescContT();
	}
	const auto& vertex_desc_to = FindVertexById(vertex_id_to);
	if ( vertex_desc_to.first == vertex_desc_to.second )
	{
		assert(false);
		return EdgeDescContT();
	}
	VertexDescContT p(num_vertices(pos_graph_));
	EdgeDescContT q(num_edges(pos_graph_));
	//dijkstra_shortest_paths(pos_graph_, *vertex_desc_from.first, predecessor_map(&p[0]).visitor(make_edge_predecessor_recorder__(q, get(boost::vertex_index, pos_graph_))));
	const auto& pro_idx = get(boost::vertex_index, pos_graph_);
	dijkstra_shortest_paths(pos_graph_, *vertex_desc_from.first,
		predecessor_map(boost::make_iterator_property_map(p.begin(), pro_idx))
		.visitor(make_edge_predecessor_recorder__(q, pro_idx)));

	return translate_to_edges_descriptor__(q, p, *vertex_desc_to.first);
}

GSP_StagePosPath::VertexDescContT GSP_StagePosPath::PosInfo::DijkstraShortestPaths_Vertex( int vertex_id_from, int vertex_id_to )
{
	const auto& the_edges = DijkstraShortestPaths_Edges(vertex_id_from, vertex_id_to);
	if (the_edges.empty())
		return VertexDescContT();
	VertexDescContT res;
	for ( auto& v : the_edges )
		res.push_back(boost::source(v, pos_graph_));
	res.push_back(boost::target(the_edges.back(), pos_graph_));
	return res;
}

GSP_StagePosPath::StepsT GSP_StagePosPath::PosInfo::DijkstraShortestStepsByVertexId( int vertex_id_from, int vertex_id_to )
{
	const auto& the_vertex = DijkstraShortestPaths_Vertex(vertex_id_from, vertex_id_to);
	if (the_vertex.empty())
		return StepsT();
	StepsT res;
	for ( auto& v : the_vertex )
		res.push_back(pos_graph_[v].pos_);
	return res;
}

GSP_StagePosPath::StepsT GSP_StagePosPath::PosInfo::DijkstraShortestSteps( const GType::stGameVector3& pos_from, const GType::stGameVector3& pos_to )
{
	NearestVertexContT vertexes;
	auto vertex_from = FindNearestVertex(pos_from, vertexes);
	if ( !vertex_from )
	{
		assert(false);
		return StepsT();
	}
	auto vertex_to = FindNearestVertex(pos_to, vertexes);
	if ( !vertex_to )
	{
		assert(false);
		return StepsT();
	}
	auto& steps = DijkstraShortestStepsByVertexId(vertex_from->vertex_.id_, vertex_to->vertex_.id_);
	if (steps.empty())
		return steps;
	steps.push_back(pos_to);
	//steps.push_front(pos_from);
	return steps;
}

const GSP_StagePosPath::PosInfo::stNearestVertexHelper* GSP_StagePosPath::PosInfo::FindNearestVertex( const GType::stGameVector3& pos, NearestVertexContT& vertexes )
{
	vertexes.clear();
	for ( auto it = boost::vertices(pos_graph_); it.first != it.second; ++it.first)
	{
		stNearestVertexHelper tmp;
		tmp.vertex_ = pos_graph_[*it.first];
		//用2D的坐标，不把Z计算进去，为了避免高度太大的问题
		//还是继续用着3D吧
		tmp.dist_ = pos.Distance3d(tmp.vertex_.pos_);
		vertexes.push_back(tmp);
	}
	auto it = std::min_element(vertexes.begin(), vertexes.end(), []( const stNearestVertexHelper& lhs, const stNearestVertexHelper& rhs ){
		return lhs.dist_ < rhs.dist_;
	});
	if ( it == vertexes.end() )
		return nullptr;
	return &*it;
}

GType::stGameVector3 GSP_StagePosPath::PosInfo::FindNearestPos( const GType::stGameVector3& pos )
{
	NearestVertexContT vertexes;
	auto vertex_ptr = FindNearestVertex(pos, vertexes);
	if ( !vertex_ptr )
	{
		assert(false);
		return pos;
	}
	return vertex_ptr->vertex_.pos_;
}

GSP_StagePosPath::PosInfo::PosInfo()
{
	auto sink = GSP_StagePosPath::GetMe().stage_pos_info_sink_;
	if (sink)
		sink_id_ = sink->OnNewPosInfoObj();
	else
		sink_id_ = 0;
}

double GSP_StagePosPath::PosInfo::CalcNearestVertexDist( const GType::stGameVector3& pos )
{
	NearestVertexContT vertexes;
	auto vertex_ptr = FindNearestVertex(pos, vertexes);
	if ( !vertex_ptr )
	{
		assert(false);
		return 0.;
	}
	return vertex_ptr->dist_;
}

//////////////////////////////////////////////////////////////////////////

bool GSP_StagePosPath::RoomInfo::AddPosInfo( int room_id, const PosInfo& pos_info )
{
	auto it = pos_infos_.find(room_id);
	if ( it != pos_infos_.end() )
	{
		LOG_ENTER_FUNC;
		/*LOG_O(Log_debug) << "添加房间信息失败，已存在房间坐标信息映射了，房间号：" << room_id;
		return false;*/
		LOG_O(Log_debug) << "添加房间坐标信息之已经存在了，覆盖旧的房间信息，房间号：" << room_id;
	}
	pos_infos_[room_id] = pos_info;

	auto sink = GSP_StagePosPath::GetMe().stage_pos_info_sink_;
	if (sink)
		sink->OnAddPosInfo(sink_id_, room_id, pos_info.sink_id_);
	return true;
}

bool GSP_StagePosPath::RoomInfo::AddRoom( const RoomInfo& room_info )
{
	for ( auto& v : room_info.pos_infos_ )
	{
		if ( !AddPosInfo(v.first, v.second) )
			return false;
	}
	return true;
}

GSP_StagePosPath::RoomInfo::RoomInfo()
{
	auto sink = GSP_StagePosPath::GetMe().stage_pos_info_sink_;
	if (sink)
		sink_id_ = sink->OnNewRoomInfoObj();
	else
		sink_id_ = 0;
}

bool GSP_StagePosPath::AddStageInfo( int stage_id, const RoomInfo& room_info )
{
#ifdef PREP_UNOPEN_SHORTEST_PATH
	return false;
#endif
	auto sink = GSP_StagePosPath::GetMe().stage_pos_info_sink_;
	if (sink)
		sink->OnAddStageInfo(stage_id, room_info.sink_id_);

	auto it = stages_info_.find(stage_id);
	if ( it != stages_info_.end() )
		return it->second.AddRoom(room_info);
	stages_info_[stage_id] = room_info;
	return true;
}

GSP_StagePosPath::StepsT GSP_StagePosPath::DijkstraShortestSteps( int stage_id, int room_id, const GType::stGameVector3& pos_from, const GType::stGameVector3& pos_to )
{
#ifdef PREP_UNOPEN_SHORTEST_PATH
	return StepsT();
#endif
	if ( stage_id < 0 || stage_id == std::numeric_limits<int>::max() )
		return StepsT();
	if ( room_id < 0 || room_id == std::numeric_limits<int>::max() )
		return StepsT();
	auto it_stage = stages_info_.find(stage_id);
	if ( it_stage == stages_info_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "没有地图坐标信息，请及时处理,stage id：" << stage_id;
		NotifyErrInfo();
		return StepsT();
	}
	auto it_room = it_stage->second.pos_infos_.find(room_id);
	if ( it_room == it_stage->second.pos_infos_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "没有地图房间坐标信息，请及时处理：" << room_id << " " << stage_id;
		NotifyErrInfo();
		return StepsT();
	}
	return it_room->second.DijkstraShortestSteps(pos_from, pos_to);
}

double GSP_StagePosPath::CalcNearestVertexDist( int stage_id, int room_id, const GType::stGameVector3& pos )
{
#ifdef PREP_UNOPEN_SHORTEST_PATH
	return kFindPathMinDistLimitOnHitting;
#endif
	if ( stage_id < 0 || stage_id == std::numeric_limits<int>::max() )
		return kFindPathMinDistLimitOnHitting;
	if ( room_id < 0 || room_id == std::numeric_limits<int>::max() )
		return kFindPathMinDistLimitOnHitting;
	auto it_stage = stages_info_.find(stage_id);
	if ( it_stage == stages_info_.end() )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "没有地图坐标信息，请及时处理2,stage id：" << stage_id;*/
		NotifyErrInfo();
		return 0;
	}
	auto it_room = it_stage->second.pos_infos_.find(room_id);
	if ( it_room == it_stage->second.pos_infos_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "没有地图房间坐标信息，请及时处理2：" << room_id << " " << stage_id;
		NotifyErrInfo();
		return 0;
	}
	return it_room->second.CalcNearestVertexDist(pos);
}

void GSP_StagePosPath::TestValidPosToDoorDist( int stage_id )
{
#ifdef PREP_UNOPEN_SHORTEST_PATH
	return;
#endif

	auto& gpm = GPlayerMe::GetMe();

#ifdef PREP_OPEN_VILLAGE_SHORTEST_PATH
	if ( !gpm.is_open_village_path_ )
	{
#endif
		gpm.UpdateCurMapInfo_Notify();
		if ( !gpm.IsInFb() )
			return;
#ifdef PREP_OPEN_VILLAGE_SHORTEST_PATH
	}
#endif

	if ( stage_id < 0 || stage_id == std::numeric_limits<int>::max() )
		return;
	
	auto it_stage = stages_info_.find(stage_id);
	LOG_ENTER_FUNC;
	if ( it_stage == stages_info_.end() )
	{
		LOG_O(Log_debug) << "TestValidPosToDoorDist,没有地图坐标信息，请及时处理，stage id：" << stage_id << " 副本名：" << gpm.GetCurMapName();
		assert(false);
		NotifyErrInfo();
		return;
	}
	auto& fb_path = GSP_FbRoom::GetMe();
	if ( !fb_path.Construct() )
	{
		assert(false);
		NotifyErrInfo();
		return;
	}
	auto& fb_mgr = FbDoorsMgr::GetMe();
	fb_mgr.RebuildAll();

	int err = 0;
	PosInfo::NearestVertexContT vertexes;
	for ( auto& v : fb_path.stages_ )
	{
		if ( !v )
		{
			assert(false);
			continue;
		}
		auto room_id = boost::lexical_cast<int>(v->stage_id_);
		auto it_room = it_stage->second.pos_infos_.find(room_id);
		if ( it_room == it_stage->second.pos_infos_.end() )
		{
			LOG_O(Log_debug) << "缺少房间[" << room_id << "]的地图信息";
			++err;
			continue;
		}
		for ( auto& gate : v->wrap_gates_ )
		{
			auto door_obj = fb_mgr.FindByName(gate.wrap_gate_id_);
			if ( !door_obj )
			{
				assert(false);
				continue;
			}
			
			const auto& door_pos = door_obj->GetPos();
			auto vertex_ptr = it_room->second.FindNearestVertex(door_pos, vertexes);
			if ( !vertex_ptr )
			{
				assert(false);
				continue;
			}
			if ( vertex_ptr->dist_ >= kFindPathMaxDangerDist )
			{
				auto target_room_num = boost::lexical_cast<int>(gate.next_stage_name_);

				LOG_O(Log_debug) << "房间[" << room_id << "]中的" << "门[" << gate.wrap_gate_id_ << "->" << target_room_num << "]与最近的寻路点距离过大，最近寻路点ID：" << vertex_ptr->vertex_.id_ << 
					" 最近寻路点坐标：" << vertex_ptr->vertex_.pos_ << " 门的坐标：" << door_pos << " 距离是：" << vertex_ptr->dist_;
				++err;
				continue;
			}
		}
	}
	if ( 0 != err )
	{
		assert(false);
		NotifyErrInfo();
	}
}

void GSP_StagePosPath::NotifyErrInfo()
{
#ifdef PREP_UNOPEN_SHORTEST_PATH
	return;
#endif
	LOG_ENTER_FUNC;
	LOG_O(Log_fatal) << "这个地图有问题，请联系代理，反馈一下";
	if (LuasScriptEchoC::IsRunning())
		throw CmnTerminateScriptException("把帐号提供给代理");
}

GType::stGameVector3 GSP_StagePosPath::FindNearestPos( int stage_id, int room_id, const GType::stGameVector3& pos )
{
#ifdef PREP_UNOPEN_SHORTEST_PATH
	return pos;
#endif
	if ( stage_id < 0 || stage_id == std::numeric_limits<int>::max() )
		return pos;
	if ( room_id < 0 || room_id == std::numeric_limits<int>::max() )
		return pos;
	auto it_stage = stages_info_.find(stage_id);
	if ( it_stage == stages_info_.end() )
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "没有地图坐标信息，请及时处理3,stage id：" << stage_id;*/
		NotifyErrInfo();
		return pos;
	}
	auto it_room = it_stage->second.pos_infos_.find(room_id);
	if ( it_room == it_stage->second.pos_infos_.end() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "没有地图房间坐标信息，请及时处理3：" << room_id << " " << stage_id;
		NotifyErrInfo();
		return pos;
	}
	return it_room->second.FindNearestPos(pos);
}

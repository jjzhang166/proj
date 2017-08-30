#pragma once
/*
@author zhp
@date 2017/2/20 2:52
@purpose for map path
*/
#include "../DependSelfLib.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <Common/LogOut.h>
#include <CommonX/CmnX_Math.h>
#include "Details/Details.hpp"

//////////////////////////////////////////////////////////////////////////
template<typename VecT>
struct CMP_Vertex{
	typedef VecT VectorType;
	VectorType		pos_;
	int				id_;
};

template<typename VecT>
struct CMP_PosInfo{
public:
	typedef typename VecT::value_type value_type;
	typedef CMP_Vertex<VecT> VertexType;
	typedef typename VertexType::VectorType VectorType;
	typedef boost::property<boost::edge_weight_t, double> EdgeWeightPropertyT_;
	typedef boost::adjacency_list<boost::vecS, boost::vecS,
		boost::undirectedS/*无向图，对于dijkstra_shortest_paths最短路径算法，只需要在两点间添加一条边就可以了*/,
		VertexType, EdgeWeightPropertyT_> PosGraphT;
	typedef boost::graph_traits<PosGraphT> GraphTraitsT;
	typedef typename GraphTraitsT::vertex_descriptor VertexDescT;
	typedef typename boost::property_map<PosGraphT, boost::vertex_bundle_t>::type VertexBunleT;
	typedef typename GraphTraitsT::vertex_iterator VertexIterT;
	typedef std::pair<VertexIterT, VertexIterT> VertexIterPairT;
	typedef typename GraphTraitsT::edge_iterator EdgeIterT;
	typedef std::pair<EdgeIterT, EdgeIterT> EdgeIterPairT;
	typedef typename GraphTraitsT::edge_descriptor EdgeDescT;
	typedef typename boost::property_map<PosGraphT, boost::edge_weight_t>::type EdgeWeightPropertyT;
	typedef std::vector<EdgeDescT> EdgeDescContT;
	typedef std::vector<VertexDescT> VertexDescContT;
	typedef std::deque<VectorType> StepsT;
	struct stNearestVertexHelper{
		VertexType		vertex_;
		double			dist_;
	};
	typedef std::vector<stNearestVertexHelper> NearestVertexContT;

public:
	inline bool AddVertex(int vertex_id, const VectorType& vec){
		if (vertex_id < 0)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "顶点ID不能为负，顶点ID：" << vertex_id << "，坐标:" << vec;
			return false;
		}
#ifdef _DEBUG
		auto it = FindVertexById(vertex_id);
		if (it.first != it.second)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "已存在顶点ID，此次添加顶点失败，顶点ID：" << vertex_id << "，坐标：" << vec;
			//assert(false);
			return false;
		}
#endif
		VertexType vertex;
		vertex.id_ = vertex_id;
		vertex.pos_ = vec;
#ifdef _DEBUG
		it = FindVertexByPos(vertex.pos_);
		if (it.first != it.second)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "已存在顶点坐标信息，此次添加顶点失败，顶点ID：" << vertex_id << "，坐标：" << vec;
			//assert(false);
			return false;
		}
#endif
		boost::add_vertex(vertex, pos_graph_);
		return true;
	}
	inline bool AddEdge(int vertex_id_from, int vertex_id_to){
#ifdef _DEBUG
		if (vertex_id_to == vertex_id_from)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "添加边失败,顶点ID不能相同，ID：" << vertex_id_from;
			//assert(false);
			return false;
		}
#endif
		auto it_from = FindVertexById(vertex_id_from);
		if (it_from.first == it_from.second)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "不存在源顶点ID，添加边失败，源顶点ID：" << vertex_id_from << " 目标顶点ID：" << vertex_id_to;
			//assert(false);
			return false;
		}
		auto it_to = FindVertexById(vertex_id_to);
		if (it_to.first == it_to.second)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "不存在目标顶点ID，添加边失败，源顶点ID：" << vertex_id_from << " 目标顶点ID：" << vertex_id_to;
			//assert(false);
			return false;
		}
#ifdef _DEBUG
		auto the_edge = FindEdgeByVertexIter(*it_from.first, *it_to.first);
		if (the_edge.first != the_edge.second)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "添加边失败，已存在该边了1, vertex_id_from:" << vertex_id_from << "  vertex_id_to:" << vertex_id_to;
			return false;
		}
		the_edge = FindEdgeByVertexIter(*it_to.first, *it_from.first);
		if (the_edge.first != the_edge.second)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "添加边失败，已存在该边了2,两个顶点之间只需要添加一次边， vertex_id_from:" << vertex_id_from << "  vertex_id_to:" << vertex_id_to;
			return false;
		}
#endif
		const auto& v_from = pos_graph_[*it_from.first].pos_;
		const auto& v_to = pos_graph_[*it_to.first].pos_;
		boost::add_edge(*it_from.first, *it_to.first, v_from.Distance(v_to), pos_graph_);
		return true;
	}
	inline VertexIterPairT FindVertexById(int vertex_id){
		for (auto it = boost::vertices(pos_graph_); it.first != it.second; ++it.first) {
			const auto& v = *it.first;
			if (pos_graph_[v].id_ == vertex_id)
				return it;
		}
		return std::make_pair(VertexEnd(), VertexEnd());
	}
	inline VertexIterPairT FindVertexByPos(const VectorType& pos){
		for (auto it = boost::vertices(pos_graph_); it.first != it.second; ++it.first) {
			const auto& v = *it.first;
			if (pos_graph_[v].pos_ == pos)
				return it;
		}
		return std::make_pair(VertexEnd(), VertexEnd());
	}
	inline EdgeIterPairT FindEdgeByVertexIter(const VertexDescT& vertex_desc_from, const VertexDescT& vertex_desc_to){
		for (auto it = boost::edges(pos_graph_); it.first != it.second; ++it.first)
		{
			const EdgeDescT& e = *it.first;

			const VertexDescT& u = boost::source(e, pos_graph_);
			if (u != vertex_desc_from)
				continue;
			const VertexDescT& v = boost::target(e, pos_graph_);
			if (v == vertex_desc_to)
				return it;
		}
		return std::make_pair(EdgeEnd(), EdgeEnd());
	}
	inline StepsT DijkstraShortestSteps(const VectorType& pos_from, const VectorType& pos_to){
		NearestVertexContT vertexes;
		auto vertex_from = FindNearestVertex(pos_from, vertexes);
		if (!vertex_from)
		{
			assert(false);
			return StepsT();
		}
		auto vertex_to = FindNearestVertex(pos_to, vertexes);
		if (!vertex_to)
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
	inline double CalcNearestVertexDist(const VectorType& pos){
		NearestVertexContT vertexes;
		auto vertex_ptr = FindNearestVertex(pos, vertexes);
		if (!vertex_ptr)
		{
			assert(false);
			return 0.;
		}
		return vertex_ptr->dist_;
	}
	inline VectorType FindNearestPos(const VectorType& pos){
		NearestVertexContT vertexes;
		auto vertex_ptr = FindNearestVertex(pos, vertexes);
		if (!vertex_ptr)
		{
			assert(false);
			return pos;
		}
		return vertex_ptr->vertex_.pos_;
	}

public:
	inline EdgeDescContT DijkstraShortestPaths_Edges(int vertex_id_from, int vertex_id_to){
		using namespace boost;
		const auto& vertex_desc_from = FindVertexById(vertex_id_from);
		if (vertex_desc_from.first == vertex_desc_from.second)
		{
			assert(false);
			return EdgeDescContT();
		}
		const auto& vertex_desc_to = FindVertexById(vertex_id_to);
		if (vertex_desc_to.first == vertex_desc_to.second)
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
	inline VertexDescContT DijkstraShortestPaths_Vertex(int vertex_id_from, int vertex_id_to){
		const auto& the_edges = DijkstraShortestPaths_Edges(vertex_id_from, vertex_id_to);
		if (the_edges.empty())
			return VertexDescContT();
		VertexDescContT res;
		for (auto& v : the_edges)
			res.push_back(boost::source(v, pos_graph_));
		res.push_back(boost::target(the_edges.back(), pos_graph_));
		return res;
	}
	inline StepsT DijkstraShortestStepsByVertexId(int vertex_id_from, int vertex_id_to){
		const auto& the_vertex = DijkstraShortestPaths_Vertex(vertex_id_from, vertex_id_to);
		if (the_vertex.empty())
			return StepsT();
		StepsT res;
		for (auto& v : the_vertex)
			res.push_back(pos_graph_[v].pos_);
		return res;
	}
	inline const stNearestVertexHelper* FindNearestVertex(const VectorType& pos, NearestVertexContT& vertexes){
		vertexes.clear();
		for (auto it = boost::vertices(pos_graph_); it.first != it.second; ++it.first)
		{
			stNearestVertexHelper tmp;
			tmp.vertex_ = pos_graph_[*it.first];
			tmp.dist_ = pos.Distance(tmp.vertex_.pos_);
			vertexes.push_back(tmp);
		}
		auto it = std::min_element(vertexes.begin(), vertexes.end(), [](const stNearestVertexHelper& lhs, const stNearestVertexHelper& rhs){
			return lhs.dist_ < rhs.dist_;
		});
		if (it == vertexes.end())
			return nullptr;
		return &*it;
	}

private:
	inline EdgeIterT EdgeEnd(){
		return EdgeIterT(pos_graph_.m_edges.end());
	}
	inline VertexIterT VertexEnd(){
		return pos_graph_.vertex_set().end();
	}
	template<typename EdgePredecessorMap, typename VertexPredecessorMap, typename VertexDescriptor>
	inline static EdgeDescContT translate_to_edges_descriptor__(const EdgePredecessorMap& em, const VertexPredecessorMap& vm, VertexDescriptor dest)
	{
		EdgeDescContT res;
		while (vm[dest] != dest)
		{
			res.push_back(em[dest]);
			dest = vm[dest];
		}
		std::reverse(res.begin(), res.end());
		return res;
	}

public:
	PosGraphT			pos_graph_;
};

template<typename VecT, typename MapIdT_ = int>
class CMP_MapPosPath{
public:
	typedef CMP_PosInfo<VecT> PosInfo;
	typedef MapIdT_ MapIdType;
	typedef typename PosInfo::StepsT StepsT;
	typedef typename PosInfo::VectorType VectorType;
	//////////////////////////////////////////////////////////////////////////
	public:
		bool AddStageInfo(const MapIdType& stage_id, const PosInfo& pos_info){
			stages_info_[stage_id] = pos_info;
			return true;
		}
		StepsT DijkstraShortestSteps(const MapIdType& stage_id, const VectorType& pos_from,
			const VectorType& pos_to){
			auto it_stage = stages_info_.find(stage_id);
			if (it_stage == stages_info_.end())
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_warning) << "没有地图坐标信息，请及时处理,stage id：" << stage_id;
				NotifyErrInfo();
				return StepsT();
			}
			return it_stage->DijkstraShortestSteps(pos_from, pos_to);
		}
		double CalcNearestVertexDist(const MapIdType& stage_id, const VectorType& pos){
			auto it_stage = stages_info_.find(stage_id);
			if (it_stage == stages_info_.end())
			{
				/*LOG_ENTER_FUNC;
				LOG_O(Log_warning) << "没有地图坐标信息，请及时处理2,stage id：" << stage_id;*/
				NotifyErrInfo();
				return 0;
			}
			return it_stage->CalcNearestVertexDist(pos);
		}
		VectorType FindNearestPos(const MapIdType& stage_id, const VectorType& pos){
			auto it_stage = stages_info_.find(stage_id);
			if (it_stage == stages_info_.end())
			{
				/*LOG_ENTER_FUNC;
				LOG_O(Log_warning) << "没有地图坐标信息，请及时处理3,stage id：" << stage_id;*/
				NotifyErrInfo();
				return pos;
			}
			return it_stage->FindNearestPos(pos);
		}
		void NotifyErrInfo(){
			assert(false);
		}

private:
	std::map<MapIdType, PosInfo>	stages_info_;
};
//////////////////////////////////////////////////////////////////////////

//blind walk，盲走(算法未完成)
class BW_GridBase{
public:
	virtual ~BW_GridBase();
	virtual double Distance(const stCmnVecBase& pos) const = 0;
	virtual bool IsWhite() const;
	virtual bool IsIn(const stCmnVecBase& pos) const = 0;
	virtual bool CanMove(const stCmnVecBase& pos) const = 0;
	virtual void GetNearestPos(const stCmnVecBase& pos_src, stCmnVecBase& pos_dst) const = 0;
	virtual const stCmnVecBase& GetCenter() const = 0;
};

//尺寸格子
class BW_GridSized : public BW_GridBase{

};

//白色格子
class BW_GridWhite : public BW_GridSized{
private:
	bool IsWhite() const override final;
};

//黑色格子
class BW_GridBlack : public BW_GridSized{

};

//详细的格子
class BW_GridSpec : public BW_GridWhite{

};

class BW_MapPathSink{
public:
	virtual bool CanMove(const stCmnVecBase& pos) const;
};

class BW_PathBase{
public:
	virtual void AddPos(const stCmnVecBase& pos) const;
};

class BW_MapPathBase{
public:
	typedef std::vector<BW_GridBase*> GridCont;
	typedef BW_PathBase StepsCont;

public:
	BW_MapPathBase(){
		sink_ = nullptr;
	}
	virtual ~BW_MapPathBase();
	void SetSink(const BW_MapPathSink* sink){
		assert(sink);
		sink_ = sink;
	}
	void ClearGrds();
	const BW_GridBase* FindWhiteGrid(const stCmnVecBase& pos) const;
	const BW_GridBase* FindNearestGrid(const stCmnVecBase& pos) const;
	const BW_GridBase* FindNearestWhiteGrid(const stCmnVecBase& pos) const;
	bool GenPath(const stCmnVecBase& pos_src, const stCmnVecBase& pos_dst, StepsCont& steps) const;

protected:
	GridCont					grids_;
	const BW_MapPathSink*		sink_;
};

template<typename VecT, int GridSize, bool IsWhite_>
struct BW_GridColoredImplT{
	VecT	center_;

	inline double Distance(const VecT& pos) const{
		return center_.Distance(pos);
	}
	inline bool IsIn(const VecT& pos) const{
		return center_.IsInBox(GridSize, GridSize, pos);
	}
	inline bool CanMove(const VecT& pos) const{
		if (IsIn(pos)) return IsWhite_;
		return false;
	}
	inline void GetNearestPos(const VecT& pos_src, VecT& pos_dst) const{
		if (IsIn(pos_src))
		{
			pos_dst = pos_src;
			return;
		}
#ifdef _DEBUG
		auto res =
#endif
			center_.BoxSegmentIntersects(GridSize / 2, GridSize / 2, pos_src, pos_dst);
		assert(res);
	}
	inline const VecT& GetCenter() const{
		return center_;
	}
};

template<typename VecT, int GridSize, bool IsWhite_, typename BaseT>
class BW_GridColoredT : public BaseT{
	typedef BW_GridColoredImplT<VecT, GridSize, IsWhite_> ImplT;

	double Distance(const stCmnVecBase& pos) const override{
		return impl_.Distance(static_cast<const VecT&>(pos));
	}
	bool IsIn(const stCmnVecBase& pos) const override{
		return impl_.IsIn(static_cast<const VecT&>(pos));
	}
	bool CanMove(const stCmnVecBase& pos) const override{
		return impl_.CanMove(static_cast<const VecT&>(pos));
	}
	void GetNearestPos(const stCmnVecBase& pos_src, stCmnVecBase& pos_dst) const override{
		return impl_.GetNearestPos(static_cast<const VecT&>(pos_src), static_cast<const VecT&>(pos_dst));
	}
	const VecT& GetCenter() const override{
		return impl_.GetCenter();
	}

private:
	ImplT		impl_;
};

template<typename VecT, int GridSize>
class BW_GridSpecT : public BW_GridSpec{
public:
	typedef std::bitset<GridSize * GridSize> PosInfoContT;

public:
	VecT			center_;
	PosInfoContT	pos_info_;

	double Distance(const stCmnVecBase& pos) const override{
		return center_.Distance(static_cast<const VecT&>(pos));
	}
	bool IsIn(const stCmnVecBase& pos) const override{
		return center_.IsInBox(GridSize, GridSize, static_cast<const VecT&>(pos));
	}
	bool CanMove(const stCmnVecBase& pos) const override{
		if (!IsIn(pos)) return false;
		auto idx = static_cast<const VecT&>(pos)-center_;
		idx += GridSize / 2;
		assert(idx.AllInRange(0, GridSize));
		return pos_info_.test(idx.CalcIndex(GridSize));
	}
	void GetNearestPos(const stCmnVecBase& pos_src, stCmnVecBase& pos_dst) const override{
		if (IsIn(pos_src))
		{
			static_cast<VecT&>(pos_dst) = static_cast<const VecT&>(pos_src);
			return;
		}
		if (!center_.BoxSegmentIntersects(GridSize / 2, GridSize / 2,
			static_cast<const VecT&>(pos_src), static_cast<const VecT&>(pos_dst)))
		{
			assert(false);
			return;
		}
		auto vec_idx = static_cast<const VecT&>(pos_dst)-center_;
		vec_idx += GridSize / 2;
		assert(vec_idx.AllInRange(0, GridSize));
		auto idx = vec_idx.CalcIndex(GridSize);
		if (pos_info_.test(idx))
			return;
		++idx;
		for (int cnt = 0; cnt < GridSize * GridSize - 1; ++idx, ++cnt)
		{
			if (pos_info_.test(idx % (GridSize * GridSize)))
				return;
		}
		assert(false);
	}
	const VecT& GetCenter() const override{
		return center_;
	}
};

template<typename VecT, int GridSize, int DetectDist>
class BW_MapPathT : public BW_MapPathBase{
	static_assert(GridSize % 2 == 1, "");
	typedef BW_GridColoredT<VecT, GridSize, true, BW_GridWhite> GridWhiteT;
	typedef BW_GridColoredT<VecT, GridSize, false, BW_GridBlack> GridBlackT;
	typedef BW_GridSpecT<VecT, GridSize> GridSpecT;

public:
	bool AddGrid(const VecT& pos){
		if (!sink_)
		{
			assert(false);
			return false;
		}
		auto nearest_grid = FindNearestGrid(pos);
		if (nearest_grid)
		{
			if (nearest_grid->IsIn(pos))
				return true;
			auto distance = nearest_grid->Distance(pos);
			if ((int)distance > DetectDist)
			{
				assert(false);
				return false;
			}
			return AddGridImpl(nearest_grid, pos);
		}
		return AddGridImpl(nullptr, pos);
	}

private:
	bool AddGridImpl(const BW_GridBase* grid, const VecT& pos) const{


	}
	BW_GridSized* CreateGridByCenter(const VecT& pos_center) const{
		GridSpecT grid_tmp;

	}

};

/*
class BW_MapPath : public BW_MapPathT<stCD_VecInt, 23, 23 * 5>{

};*/

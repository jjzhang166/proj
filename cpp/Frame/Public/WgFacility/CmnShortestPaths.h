#pragma once
/*
@author zhp
@date 2017/4/12 17:31
@purpose for shortest path
*/
#include <config/AdjustWin32.h>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <StdEx/VectorEx.h>
#include "Details/Details.hpp"

//////////////////////////////////////////////////////////////////////////
struct CSP_PolicyVertex{};
struct CSP_PolicyVertexWeight{};
//////////////////////////////////////////////////////////////////////////
namespace Details{
	template<typename Policy, template<typename T> class ContT, typename VertexWeightT>
	struct CSP_PolicySteps;

	template<template<typename T> class ContT, typename VertexWeightT>
	struct CSP_PolicySteps<CSP_PolicyVertex, ContT, VertexWeightT>{
		typedef typename VertexWeightT::VertexType VertexType;
		typedef ContT<VertexType> type;

		static void push_back(type& cont, const VertexWeightT& vertex_info){
			cont.push_back(vertex_info.vertex_);
		}
	};

	template<template<typename T> class ContT, typename VertexWeightT>
	struct CSP_PolicySteps<CSP_PolicyVertexWeight, ContT, VertexWeightT>{
		typedef ContT<VertexWeightT> type;

		static void push_back(type& cont, const VertexWeightT& vertex_info){
			cont.push_back(vertex_info);
		}
	};
}
//////////////////////////////////////////////////////////////////////////
template<typename VertexType_,									//顶点类型
	typename EdgeWeightType_ = int,								//边的权重类型
	template<typename T> class StepsCont_ = StdEx::vector,		//结果step容器
	template<typename T> class NormalCont_ = StepsCont_			//算法所需的容器类型
>
class CSP_Resolver{
public:
	typedef VertexType_ VertexType;
	typedef EdgeWeightType_ EdgeWeightType;
	struct VertexWeightType{
		typedef VertexType VertexType;
		typedef EdgeWeightType EdgeWeightType;

		VertexType		vertex_;
		EdgeWeightType	weight_;
	};
	typedef StepsCont_<VertexType> StepsContType;
	typedef StepsCont_<VertexWeightType> StepsWeightContType;
	//////////////////////////////////////////////////////////////////////////
	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
		VertexType, boost::property<boost::edge_weight_t, EdgeWeightType> > GraphType;
	typedef boost::graph_traits<GraphType> GraphTraitsType;
	//////////////////////////////////////////////////////////////////////////
	typedef typename GraphTraitsType::vertices_size_type vertices_size_type;
	typedef typename GraphTraitsType::vertex_descriptor vertex_descriptor;
	typedef typename GraphTraitsType::vertex_iterator vertex_iterator;
	typedef std::pair<vertex_iterator, vertex_iterator> VertexIteratorPair;
	typedef NormalCont_<vertex_descriptor> VertexDescCont;
	//////////////////////////////////////////////////////////////////////////
	typedef typename GraphTraitsType::edges_size_type edges_size_type;
	typedef typename GraphType::edge_property_type edge_property_type;
	typedef typename GraphTraitsType::edge_descriptor edge_descriptor;
	typedef typename GraphTraitsType::edge_iterator edge_iterator;
	typedef std::pair<edge_iterator, edge_iterator> EdgeIteratorPair;
	typedef NormalCont_<edge_descriptor> EdgeDescCont;
	//////////////////////////////////////////////////////////////////////////
private:
	template<typename Policy>
	using PolicyStepsT = Details::CSP_PolicySteps<Policy, StepsCont_, VertexWeightType>;
	//////////////////////////////////////////////////////////////////////////

public:
	CSP_Resolver& operator += (const CSP_Resolver& rhs){
		rhs.TraVertices([this](const VertexType& vertex){
			if (!Exist(vertex))
				AddVertex(vertex);
			return true;
		});
		rhs.TraEdges([this](const VertexType& lhs, const VertexType& rhs, const EdgeWeightType& weight){
			if (!Exist(lhs, rhs))
				AddEdge(lhs, rhs, weight);
			return true;
		});
		return *this;
	}
	bool AddVertex(const VertexType& vertex){
		auto it_pair = FindVertexIter_(vertex);
		if (it_pair.first != it_pair.second)
		{
			assert(false);
			return false;
		}
		boost::add_vertex(vertex, graph_);
		return true;
	}
	bool AddEdge(const VertexType& vertex_from, const VertexType& vertex_dst, const EdgeWeightType& weight){
		auto it_from_pair = FindVertexIter_(vertex_from);
		if (it_from_pair.first == it_from_pair.second)
		{
			assert(false);
			return false;
		}
		auto it_dst_pair = FindVertexIter_(vertex_dst);
		if (it_dst_pair.first == it_dst_pair.second)
		{
			assert(false);
			return false;
		}
		auto the_edge = FindEdgeIter_(*it_from_pair.first, *it_dst_pair.first);
		if (the_edge.first != the_edge.second)
		{
			assert(false);
			return false;
		}
		boost::add_edge(*it_from_pair.first, *it_dst_pair.first, weight, graph_);
		return true;
	}
	bool AddEdge(const VertexType& lhs, const VertexType& rhs){
		return AddEdge(lhs, rhs, EdgeWeightType(1));
	}
	void clear(){
		graph_.clear();
	}
	vertices_size_type VertexCount() const{
		return boost::num_vertices(graph_);
	}
	bool Exist(const VertexType& vertex) const{
		return !!FindVertex(vertex);
	}
	bool Exist(const VertexType& lhs, const VertexType& rhs) const{
		auto it_lhs_pair = FindVertexIter_(lhs);
		if (it_lhs_pair.first == it_lhs_pair.second)
			return false;
		auto it_rhs_pair = FindVertexIter_(rhs);
		if (it_rhs_pair.first == it_rhs_pair.second)
			return false;
		auto the_edge = FindEdgeIter_(*it_lhs_pair.first, *it_rhs_pair.first);
		return the_edge.first != the_edge.second;
	}
	const VertexType* FindVertex(const VertexType& vertex) const{
		for (auto it = boost::vertices(graph_); it.first != it.second; ++it.first)
		{
			const auto& the_v = graph_[*it.first];
			if (the_v == vertex)
				return &the_v;
		}
		return nullptr;
	}
	template<typename Fn_>
	const VertexType* FindVertex(const Fn_& fn) const{
		for (auto it = boost::vertices(graph_); it.first != it.second; ++it.first)
		{
			const auto& the_v = graph_[*it.first];
			if (fn(the_v))
				return &the_v;
		}
		return nullptr;
	}
	const VertexType* FindConnectedVertex(const VertexType& vertex) const{
		const VertexType* connected_vertex = nullptr;
		TraEdges(vertex, [&connected_vertex, &vertex](const VertexType& lhs, const VertexType& rhs, const EdgeWeightType&){
			if (lhs == vertex)
			{
				connected_vertex = &rhs;
				return false;
			}
			else if (rhs == vertex)
			{
				connected_vertex = &lhs;
				return false;
			}
			assert(false);
			return false;
		});
		return connected_vertex;
	}
	template<typename Fn_>
	vertices_size_type TraVertices(const Fn_& fn) const{
		vertices_size_type cnt = 0;
		for (auto it = vertices(graph_); it.first != it.second; ++it.first)
		{
			const auto& the_v = graph_[*it.first];
			++cnt;
			if (!fn(the_v))
				break;
		}
		return cnt;
	}
	template<typename Fn_>
	edges_size_type TraEdges(const Fn_& fn) const{
		using namespace boost;
		edges_size_type cnt = 0;
		for (auto it = edges(graph_); it.first != it.second; ++it.first)
		{
			const auto& edge_desc = *it.first;
			++cnt;
			if (!fn(graph_[source(edge_desc, graph_)],
				graph_[target(edge_desc, graph_)],
				((edge_property_type*)static_cast<const edge_property_type*>(edge_desc.get_property()))->m_value))
				break;
		}
		return cnt;
	}
	template<typename Fn_>
	edges_size_type TraEdges(const VertexType& vertex, const Fn_& fn) const{
		using namespace boost;
		edges_size_type cnt = 0;
		for (auto it = edges(graph_); it.first != it.second; ++it.first)
		{
			const auto& edge_desc = *it.first;
			const auto& vertex_lhs = graph_[source(edge_desc, graph_)];
			const auto& vertex_rhs = graph_[target(edge_desc, graph_)];
			if (vertex_lhs == vertex || vertex_rhs == vertex)
			{
				++cnt;
				if (!fn(vertex_lhs, vertex_rhs,
					((edge_property_type*)static_cast<const edge_property_type*>(edge_desc.get_property()))->m_value))
					break;
			}
		}
		return cnt;
	}
	//////////////////////////////////////////////////////////////////////////
	//两点之间的最短路径
	bool Resolve(const VertexType& vertex_from, const VertexType& vertex_dst, StepsContType& out_vertices) const{
		using namespace boost;
		auto it_from_pair = FindVertexIter_(vertex_from);
		if (it_from_pair.first == it_from_pair.second)
		{
			//assert(false);
			return false;
		}
		auto it_dst_pair = FindVertexIter_(vertex_dst);
		if (it_dst_pair.first == it_dst_pair.second)
		{
			//assert(false);
			return false;
		}
		if (it_from_pair.first == it_dst_pair.first)
		{
			assert(false);
			return false;
		}
		//////////////////////////////////////////////////////////////////////////
		VertexDescCont vertex_desc_cont(num_vertices(graph_));
		EdgeDescCont edge_desc_cont/*(num_edges(graph_))*/;
		const auto& pro_idx = get(boost::vertex_index, graph_);
		boost::dijkstra_shortest_paths(graph_, *it_from_pair.first,
			predecessor_map(boost::make_iterator_property_map(vertex_desc_cont.begin(), pro_idx))
			.visitor(make_edge_predecessor_recorder__(edge_desc_cont, pro_idx)));
		//////////////////////////////////////////////////////////////////////////
		auto dst_desc = *it_dst_pair.first;
		EdgeDescCont sorted_edge;
		while (vertex_desc_cont[dst_desc] != dst_desc)
		{
			sorted_edge.push_back(edge_desc_cont[dst_desc]);
			dst_desc = vertex_desc_cont[dst_desc];
		}
		if (sorted_edge.empty())
			return false;
		std::reverse(sorted_edge.begin(), sorted_edge.end());		
		//////////////////////////////////////////////////////////////////////////
		vertex_desc_cont.clear();
		for (auto& v : sorted_edge)
			vertex_desc_cont.push_back(boost::source(v, graph_));
		vertex_desc_cont.push_back(boost::target(sorted_edge.back(), graph_));
		//////////////////////////////////////////////////////////////////////////
		for (auto& v : vertex_desc_cont)
			out_vertices.push_back(graph_[v]);
		//////////////////////////////////////////////////////////////////////////
		return true;
	}
	//以指定点为起点，到其他可通行的所有点，并对其进行排序（根据到起点的距离，从小到大排序）
	template<typename Policy = CSP_PolicyVertex>
	bool Resolve(const VertexType& vertex_begin, typename PolicyStepsT<Policy>::type& out_vertices) const{
		using namespace boost;
		auto it_begin_pair = FindVertexIter_(vertex_begin);
		if (it_begin_pair.first == it_begin_pair.second)
		{
			//assert(false);
			return false;
		}
		//////////////////////////////////////////////////////////////////////////
		VertexDescCont vertex_desc_cont(num_vertices(graph_));
		NormalCont_<EdgeWeightType> vertex_distance_cont(num_vertices(graph_));
		boost::dijkstra_shortest_paths(graph_, *it_begin_pair.first,
			predecessor_map(&vertex_desc_cont[0]).distance_map(&vertex_distance_cont[0]));
		//////////////////////////////////////////////////////////////////////////
		NormalCont_<VertexWeightType> weight_vertices_;
		const auto kWeightValueMax = std::numeric_limits<EdgeWeightType>::max();
		for (auto it = vertices(graph_); it.first != it.second; ++it.first)
		{
			auto vertex_desc = *it.first;
			if (vertex_distance_cont[vertex_desc] == kWeightValueMax)
				continue;
			weight_vertices_.push_back({ graph_[vertex_desc], vertex_distance_cont[vertex_desc] });
		}
		if (weight_vertices_.empty())
		{
			assert(false);
			return false;
		}
		std::sort(weight_vertices_.begin(), weight_vertices_.end(), [](const VertexWeightType& lhs, const VertexWeightType& rhs){
			return lhs.weight_ < rhs.weight_;
		});
		for (auto& v : weight_vertices_)
			PolicyStepsT<Policy>::push_back(out_vertices, v);
		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	vertices_size_type DbgAllVertices() const{
		return TraVertices([](const VertexType& vertex){
			vertex.DebugInfo();
			return true;
		});
	}
	edges_size_type DbgAllEdges() const{
		return TraEdges([](const VertexType& from, const VertexType& dst, EdgeWeightType){
			from.DebugInfo(dst);
			return true;
		});
	}

private:
	VertexIteratorPair FindVertexIter_(const VertexType& vertex) const{
		auto it = boost::vertices(graph_);
		for (; it.first != it.second; ++it.first)
		{
			const auto& v = *it.first;
			if (graph_[v] == vertex)
				return it;
		}
		return it;
	}
	EdgeIteratorPair FindEdgeIter_(const vertex_descriptor& vertex_desc_lhs, const vertex_descriptor& vertex_desc_rhs) const{
		auto it = boost::edges(graph_);
		for (; it.first != it.second; ++it.first)
		{
			const auto& vertex_desc = *it.first;
			const auto& src = boost::source(vertex_desc, graph_);
			if (src != vertex_desc_lhs)
			{
				if (src != vertex_desc_rhs)
					continue;
				const auto& dst = boost::target(vertex_desc, graph_);
				if (dst == vertex_desc_lhs)
					return it;
			}
			else
			{
				const auto& dst = boost::target(vertex_desc, graph_);
				if (dst == vertex_desc_rhs)
					return it;
			}			
		}
		return it;
	}

private:
	GraphType	graph_;
};

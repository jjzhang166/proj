#pragma once
/*
@author zhp
@date 2017/4/12 21:12
@purpose for details
*/

template<typename PredecessorMap_, typename PropertyIndexT>
class ShortestEdgePredecessors__ : public boost::dijkstra_visitor<>{
public:
	explicit ShortestEdgePredecessors__(PredecessorMap_& p, const PropertyIndexT& p_i)
		: m_predecessor(p), p_i_(p_i){}

	template <class Edge, class Graph>
	void edge_relaxed(const Edge& e, Graph& g){
		const auto& target_desc = boost::target(e, g);
		auto target_idx = p_i_[target_desc];
		if (target_idx >= m_predecessor.size())
			m_predecessor.resize(target_idx + 1);
		boost::put(&*m_predecessor.begin(), target_desc, e);
	}

protected:
	PredecessorMap_&		m_predecessor;
	const PropertyIndexT&	p_i_;
};

template <typename PredecessorMap, typename PropertyIndexT>
ShortestEdgePredecessors__<PredecessorMap, PropertyIndexT> make_edge_predecessor_recorder__(PredecessorMap& p, const PropertyIndexT& p_i)
{
	return ShortestEdgePredecessors__<PredecessorMap, PropertyIndexT>(p, p_i);
}

#pragma once
/*
@author zhp
@date 2014/8/26 2:25
@purpose for village path, move to village or room, GSP->game stage path
*/
#include <deque>
#include <string>
#include <memory>
#include "GMap.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <CD_CommonFiles/IStagePosInfo.h>

//房间连接信息
extern void InitDoorLinkInfo();

class GSP_Stage;
class GSP_Gate{
public:
	const GSP_Stage*	next_stage_;		//进入门后的城镇或副本房间
	std::string			wrap_gate_id_;
	std::string			next_stage_name_;
};

struct GSP_Step{
	const GSP_Stage*	step_stage_;
	const GSP_Gate*		step_gate_;
};

class GSP_Stage{
public:
	typedef std::deque<GSP_Step> StepsT;
	typedef std::shared_ptr<GSP_Stage> StagePtr;
	typedef std::vector<GSP_Gate> GateContT;

public:
	bool MoveTo( const GSP_Stage* dst_stage, StepsT& steps_out ) const;

private:
	void MoveToNext( StepsT& cur_steps, const GSP_Stage* dst_stage, StepsT& steps_out ) const;

public:
	std::string		stage_id_;
	GateContT		wrap_gates_;
};

//求的是最短路径
class GSP_Resolver{
public:
	typedef GSP_Stage::StagePtr StagePtr;
	typedef std::vector<StagePtr> StagesT;

public:
	bool VillageMove( const std::string& village_id_src, const std::string& village_id_dst, GSP_Stage::StepsT& steps_out );
	GSP_Stage* FindStage( const std::string& stage_id );
	virtual bool Construct() = 0;
	void DebugInfo() const;

protected:	
	void AddStage_Gate( const std::string& stage_id, const GSP_Gate& gate );
	void BindGate();

public:
	StagesT			stages_;
};

class GSP_Village : public GSP_Resolver, public Singleton<GSP_Village, Singleton_MakeMe>{
public:
	typedef std::map<std::string, GType::stGameVector3> Gate2PosT;
	typedef std::map<int, std::string> VillageId2NameT;
	typedef std::map<std::string, GType::stGameVector3> VillageFbPosT;

public:
	GSP_Village();
	bool Construct() override;
	boost::optional<GType::stGameVector3> Gate2PosInfo( const std::string& gate_name ) const;
	boost::optional<std::string> VillageId2Name( int village_id ) const;
	boost::optional<GType::stGameVector3> VillageFbPos( const std::string& village_name ) const;

private:
	bool AddGate2PosInfo( const std::string& gate_name, const GType::stGameVector3& pos );

private:
	bool	is_init_;
	Gate2PosT			gate_name_2_pos_;
	VillageId2NameT		village_id_2_name_;
	VillageFbPosT		village_fb_pos_;
};

class GSP_FbRoom : public GSP_Resolver, public Singleton<GSP_FbRoom, Singleton_MakeMe>{
public:
	bool Construct() override;
	bool ForceConstruct();

private:
	bool ConstructImpl();
};

//地图位置路径
class GSP_StagePosPath : public Singleton<GSP_StagePosPath, Singleton_MakeMe>{
public:
	struct stVertex{
		GType::stGameVector3		pos_;
		int							id_;
	};
	typedef boost::property<boost::edge_weight_t, double> EdgeWeightPropertyT_;
	typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS/*无向图，对于dijkstra_shortest_paths最短路径算法，只需要在两点间添加一条边就可以了*/,
		stVertex, EdgeWeightPropertyT_> PosGraphT;
	typedef boost::graph_traits<PosGraphT> GraphTraitsT;
	typedef GraphTraitsT::vertex_descriptor VertexDescT;
	typedef boost::property_map<PosGraphT, boost::vertex_bundle_t>::type VertexBunleT;
	typedef GraphTraitsT::vertex_iterator VertexIterT;
	typedef std::pair<VertexIterT, VertexIterT> VertexIterPairT;
	typedef boost::graph_traits<PosGraphT>::edge_iterator EdgeIterT;
	typedef std::pair<EdgeIterT, EdgeIterT> EdgeIterPairT;
	typedef boost::graph_traits<PosGraphT>::edge_descriptor EdgeDescT;
	typedef boost::property_map<PosGraphT, boost::edge_weight_t>::type EdgeWeightPropertyT;
	typedef std::vector<EdgeDescT> EdgeDescContT;
	typedef std::vector<VertexDescT> VertexDescContT;
	typedef std::deque<GType::stGameVector3> StepsT;

	struct PosInfo{
	public:
		struct stNearestVertexHelper{
			stVertex		vertex_;
			double			dist_;
		};
		typedef std::vector<stNearestVertexHelper> NearestVertexContT;

	public:
		PosInfo();
		bool AddVertex( int vertex_id, float x, float y, float z );
		bool AddEdge( int vertex_id_from, int vertex_id_to );
		VertexIterPairT FindVertexById( int vertex_id );
		VertexIterPairT FindVertexByPos( const GType::stGameVector3& pos );
		EdgeIterPairT FindEdgeByVertexIter( const VertexDescT& vertex_desc_from, const VertexDescT& vertex_desc_to );
		StepsT DijkstraShortestSteps( const GType::stGameVector3& pos_from, const GType::stGameVector3& pos_to );
		double CalcNearestVertexDist( const GType::stGameVector3& pos );
		GType::stGameVector3 FindNearestPos( const GType::stGameVector3& pos );

	public:
		EdgeDescContT DijkstraShortestPaths_Edges( int vertex_id_from, int vertex_id_to );
		VertexDescContT DijkstraShortestPaths_Vertex( int vertex_id_from, int vertex_id_to );
		StepsT DijkstraShortestStepsByVertexId( int vertex_id_from, int vertex_id_to );
		const stNearestVertexHelper* FindNearestVertex( const GType::stGameVector3& pos, NearestVertexContT& vertexes );
		

	private:
		EdgeIterT EdgeEnd();
		VertexIterT VertexEnd();

	public:
		PosGraphT			pos_graph_;
		DWORD				sink_id_;
	};
	struct RoomInfo{
		RoomInfo();
		bool AddPosInfo( int room_id, const PosInfo& pos_info );
		bool AddRoom( const RoomInfo& room_info );

		std::map<int, PosInfo>		pos_infos_;
		DWORD						sink_id_;
	};

public:
	bool AddStageInfo( int stage_id, const RoomInfo& room_info );
	StepsT DijkstraShortestSteps( int stage_id, int room_id, const GType::stGameVector3& pos_from, const GType::stGameVector3& pos_to );
	double CalcNearestVertexDist( int stage_id, int room_id, const GType::stGameVector3& pos );
	GType::stGameVector3 FindNearestPos( int stage_id, int room_id, const GType::stGameVector3& pos );
	//测试距离信息
	void TestValidPosToDoorDist( int stage_id );
	void NotifyErrInfo();

public:
	IStagePosInfo*					stage_pos_info_sink_;

private:
	std::map<int, RoomInfo>			stages_info_;
};
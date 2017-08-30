#pragma once
/*
@author zhp
@date 2017/2/5 22:36
@purpose village or clone map. map == village or clone map
*/
#include <Common/UsefulClass.h>
#include <Common/GameObjBase.h>
#include "GameObj.h"
#include <WgFacility/CmnAStar.h>
#include <WgFacility/CmnShortestPaths.h>
#include "GType.h"
#include "GameObjMgr.h"

class GWayPoint : public GameObjBase{
public:
	GWayPoint();

public:
	void DebugInfo() const override;

private:
	bool UpdateImpl() override;
	bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res) override;
	bool CloseObject() const override;
	int GetCanOpenedDistance() const override;
	bool MovingDoTryOpen() const override;
};

class GAreaTransition : public GameObjBase{
public:
	GAreaTransition();
	void RememberLastMapInfo();
	enCD_kMapDifficulty GetLastDifficulty() const;
	const GClass::stLastMapInfo& GetLastMapInfo() const;
	void DebugInfo() const override;
	bool IsTransitonable() const override;
	bool IsTownTranstionable() const override;

public:
	bool UpdateImpl() override;

private:
	bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res) override;
	int GetCanOpenedDistance() const override;
	bool MovingDoTryOpen() const override;

private:
	GClass::stLastMapInfo		last_map_info_;
	enCD_kMapDifficulty			map_difficulty_;
	bool						is_town_area_;
};

//非人类的npc，比如仓库、告示板、活动告示板，流放者的信属于NPC
class GNoHuman : public GameObjBase{
public:
	GNoHuman(GameObjType obj_type);

private:
	bool UpdateImpl() override;

private:
	stCD_EntityNoHuman*		no_humain_;
};

//仓库
class GStash : public GNoHuman{
public:
	GStash();

private:
	bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res) override;
};

class GMapBase : public GGameObjBase{
public:
	GMapBase();
	inline stCD_MapData* GetMapData() const{ return game_map_data_; }
	inline bool IsInTown() const{ return is_in_town_; }

protected:
	bool UpdateImpl() override;

protected:
	stCD_World*				game_world_;
	stCD_TblItemWorldArea*	game_world_area_;
	stCD_MapData*			game_map_data_;

protected:
	bool					is_in_town_;
};

struct stMapDataType{
public:
	struct stDbgSrcMapData{
		stDbgSrcMapData(stMapDataType& map_data_impl, bool is_src_map_data);
		bool CanMove(const stCD_VecInt& pos) const;

		stMapDataType&	map_data_;
		bool			is_src_map_data_;
	};

	struct stDbgGetMapData{
		stDbgGetMapData(stMapDataType& map_data_impl, bool is_src_map_data);
		enCD_MapDataFlag MapData(const stCD_VecInt& pos) const;

		stMapDataType&	map_data_;
		bool			is_src_map_data_;
	};

public:
	stMapDataType(stCD_MapData& map_data, const stCD_VecInt& map_size);
	inline const stCD_VecInt& GetMapSize() const{
		return map_size_;
	}
	inline bool CanMoveForValidRect(const stCD_VecInt& pos) const{
		return (map_data_.*fn_)(pos, false) > kMDF_Mountain;
	}
	inline bool CanMove(const stCD_VecInt& pos) const{
		return (map_data_.*fn_)(pos, false) > kMDF_Mountain;
	}

private:
	stCD_VecInt						map_size_;
	stCD_MapData&					map_data_;
	stCD_MapData::Fn_GetDstMapData	fn_;
};

//这些参数不能随便乱改，服务器拉回次数过多的话，容易封号。
class G_AStar : public CmnAStar<stCD_VecInt, stMapDataType, char,
	1200,		//最大边长尺寸
	800,		//边长尺寸，不能少于500，否则第一个地图都过不去
	25,			//缩放之允许调整的大小，未缩放的坐标系
	50,			//压缩半径，未缩放的坐标系
	50,			//射线探路之step，未缩放的坐标系
	false
	>, public CD_Interface::CalcAvoidPos{
public:
	static G_AStar& GetMe();
	void EnableAvoidPos(bool enable) const;
	bool IsEnabledAvoidPos() const;
	bool GenRaySteps(const stCD_VecInt& pos_dst, PosContT& out_pos_info);
	void SetMapZoomRate(const std::string& map_name, int zoom_rate);
	int GetMapSideSize(const std::string& map_name) const;
	bool CanLineMoveTo(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst);
	void DebugPosInfo1(bool dbg_data, const std::string& file_name) const;
	void DebugPosRectInfo1(const std::string& file_name, int range, bool is_src_map_data) const;
	void DebugRectDataInfo1(const std::string& file_name, int range, bool is_src_map_data) const;
	void DebugRectPosDataInfo1(const std::string& file_name, int range, bool is_src_map_data) const;

private:
	const stCD_VecInt* GetLovedDstPos() const override;
	bool CanShooted(const stCD_VecInt& center, float radius,
		const stCD_VecInt& pos1, const stCD_VecInt& pos2) const override;
	bool GetAvoidPos(const stCD_VecInt& center, int half_width, int half_height, float center_distance,
		const stCD_VecInt* dst_pos,
		const stCD_VecInt& pos_from, const stCD_VecInt& pos_to, stCD_VecInt& out_res) const override;
	bool MoveTo(const stCD_VecInt& pos, pt_dword entity_id, pt_word skill_id) const override;

private:
	std::map<std::string, int>		map_zoom_rate_;
};

class GPathMark : public CmnPathMark<G_AStar,
	200,	//最大边长尺寸
	60		//矩形，从中心点到边的长度
	, false>{
public:
	GPathMark(const G_AStar& astar);
	void DebugPosInfo1(bool dbg_data, const std::string& file_name) const;
	static GPathMark& GetMe();
};

class GAvoidSkill : public CmnOptimalSuggestPos<stCD_VecInt, StaticMutexLocker,
	1500,		//超过这个时间的建议坐标会被废弃掉
	1500,		//多久的时间间隔才允许计算最优坐标
	15			//密集距离
	>{
public:
	static GAvoidSkill& GetMe();
};

class GMap : public GMapBase, public Singleton<GMap, Singleton_MakeMe>{
	friend class App;

public:
	typedef CSP_Resolver<stAreaVertex, int, GMapAreaShortestPathsCont> AreaResolver;
	typedef AreaResolver::StepsContType AreaStepsType;
	typedef ::GClass::stLastMapInfo stLastMapInfo;
	typedef G_AStar::AStarBaseType AStarBaseType;
	//不要随便定义该对象，因为这个结构体非常大，小心堆栈不够用哦
	struct stMemoryMapInfo{
		AStarBaseType		astar_data_;
		GameObjMgrCont		obj_mgr_;
		std::string			map_name_;
		AreaResolver		area_resolver_;
	};
	typedef std::vector<stMemoryMapInfo> MemoryMapContT;
	typedef std::vector<std::string> AreaNameCont;
	struct stMapAreaInfo{
		AreaNameCont	area_names_;
	};
	typedef std::map<std::string, stMapAreaInfo> MapAreaInfoMap;

public:
	GMap();
	bool LastEqual() const;
	const stCD_VecInt& GetMapSize() const;
	bool Update() override;
	const std::string& GetMapName() const;
	void EnableAvoidSkill(bool enable);
	bool IsEnableAvoidSkill() const;
	inline int GetChapter() const{ return chapter_; }
	bool BeginChangeMap(stLastMapInfo& last_map_info);
	bool HasChangedMap(const stLastMapInfo& last_map_info) const;
	bool BeginChangeMap();
	bool HasChangedMap() const;
	//只供脚本用
	bool UpdateCurMap_();

	//地图房间相关
public:
	void ClearAllMarkInfo();
	void SetMapAreaInfo(const std::string& map_name, const luabind::object& areas);
	void AddAreaConnection(const stAreaVertex& vertex_lhs, const stAreaVertex& vertex_rhs);
	bool GenAreaSteps(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, AreaStepsType& steps) const;
	bool GenSortedAreaSteps(const GameObjBasePtrT& obj, AreaStepsType& steps) const;
	//MovingDo的时候，记得要判断是否已经打开过该门了。通过判断顶点即可。
	const stMapAreaInfo* GetCurAreaInfo() const;
	stAreaVertex FindAreaByName(const std::string& area_name) const;
	stAreaVertex FindConnectedRoom(const stAreaVertex& vertex) const;

private:
	void DoUpdated();
	bool UpdateImpl() override;
	void RememberLastMapObjInfo();
	void ParseCurMemoryMapInfo();
	stMemoryMapInfo& GetFreeMemoryMapInfo(const std::string& map_name);
	bool InitNewAstar(stCD_World& new_world);
	stAreaVertex FindNearestAreaVertex(const stCD_VecInt& pos) const;
	bool RememberLastInfo(stLastMapInfo& last_map_info) const;
	bool RememberLastInfo();
	void ForgetLastInfo();

private:
	std::string				map_name_;
	stCD_VecInt				map_size_;
	int						chapter_;			//地图章节

private:
	stLastMapInfo			last_map_info_;
	MemoryMapContT			memory_maps_;
	AStarBaseType			new_astar_;
	bool					is_changed_map_;

private:
	MapAreaInfoMap			map_area_infos_;
	AreaResolver			area_resolver_;
};

//求的是最短路径
class GSP_Resolver : public GGameObjMgrBase, public Singleton<GSP_Resolver, Singleton_MakeMe>{
public:
	struct stVertex{
		std::string				name_;
		enCD_kMapDifficulty		diff_lvl_;
		int						chapter_;
		bool					is_town_;

		stVertex(){
			diff_lvl_ = kMD_Invalid;
			chapter_ = -1;
			is_town_ = false;
		}
		bool operator == (const stVertex& rhs) const{
			return diff_lvl_ == rhs.diff_lvl_ && name_ == rhs.name_;
		}
		void DebugInfo() const;
		void DebugInfo(const stVertex& dst) const;
	};
	typedef CSP_Resolver<stVertex, int, GMapShortestPathsCont> ResolverImplT;
	typedef ResolverImplT::StepsContType StepsContType;
	typedef std::vector<std::string> MapNameCont;
	struct stMoveHelp{
		stMoveHelp() : type_(kGOT_AreaTransition){}

		std::string			begin_name_;
		std::string			end_name_;
		std::string			ori_map_;
		GameObjType			type_;
		enCD_kMapDifficulty difficulty_;
	};
	typedef std::deque<stMoveHelp> DequeHelp;

public:
	GSP_Resolver();
	bool DijkFindPath(const std::string& start, const std::string& end, enCD_kMapDifficulty diff_lvl, MapNameCont& path_list) const;
	bool GetCurMapShortestWaypoint(const std::string& cur_name, enCD_kMapDifficulty diff_lvl, MapNameCont& que_ret) const;
	bool IsTownByMapName(const std::string& map_name, enCD_kMapDifficulty diff_lvl) const;
	bool GetTargetMapShortestWayPoint(const std::string& dst_map_name, enCD_kMapDifficulty diff_lvl, MapNameCont& sorted_map) const;
	const std::string& GetCurChapterTownName(int chapter) const;
	bool GetDijkLatelyPath(const std::string& dst_map_name, enCD_kMapDifficulty diff_lvl, DequeHelp& path_help) const;
	bool GetAllTownName(MapNameCont& all_town) const;
	int GetMapCurChapter(const std::string& dst_map_name) const;

public:
	void DebugInfo() const override;
	void DbgAllVertices() const;
	void DbgAllEdges() const;
	bool RebuildAll() override;

protected:
	bool RebuildAllImpl() override;
	bool GetDijkLatelyPathCurDiff(const std::string& dst_map_name, DequeHelp& path_help) const;
	bool GetDijkLatelyPathByWay(const std::string& dst_map_name, enCD_kMapDifficulty dst_Diff_lvl, DequeHelp& path_help) const;

private:
	ResolverImplT		resolver_;
};

class GWayPointDlgMgr : public GGameObjMgrBase, public Singleton<GWayPointDlgMgr, Singleton_MakeMe>{
public:
	GWayPointDlgMgr();
	struct stMapInfo{
		stMapInfo() :is_can_way_(false), chapter_(0), difficulty_(kMD_Invalid){ is_open_way_ = false; }
		std::string				map_name_;
		std::string				dst_map_name_;
		enCD_kMapDifficulty		difficulty_;
		int						chapter_;
		bool					is_can_way_;
		bool					is_open_way_;
	};
	typedef std::vector<stMapInfo> GateConT;

public:
	void DebugInfo() const override;
	void TestWayPoint(const std::string& dst_name, int diff);

public:
	stMapInfo* GetDstWayPointState(const std::string& dst_map, enCD_kMapDifficulty diff_lvl);
	bool TargetMapCanWayPoint(const std::string& dst_map, enCD_kMapDifficulty diff_lvl);
	bool GetWayPointName(std::string& dst_map);
	bool GetWayPointDstName(std::string& dst_map,enCD_kMapDifficulty diff_lvl);
	bool CalcMapLatelyPath(GSP_Resolver::MapNameCont& src_lately_map, enCD_kMapDifficulty diff_lvl);
	bool IsOpenedWayPoint(const std::string& dst_name, enCD_kMapDifficulty diff_lvl);

protected:
	bool RebuildAllImpl() override;	

private:
	GateConT	way_point_info_;
	bool		open_way_state_;
};

class GTranstionable : public GameObjBase{
	friend class GDbMapPins;
public:
	GTranstionable(GameObjType obj_type);

public:
	void DebugInfo() const override;
	bool GetOpenState() const;
	float WeightDistance(const stCD_VecInt& pos) const override;
	bool IsTransitonable() const override;

private:
	bool UpdateImpl() override;
	bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res) override;
	int GetCanOpenedDistance() const override;
	bool MovingDoTryOpen() const override;
	bool CanBreakOpenObj() const override;

private:
	stCD_EntityTransitionable*		entity_transitionable_;

private:
	bool			is_opened_;
};
 
class GDbMapPins : public GGameObjBase{
public:
	GDbMapPins();
	void DebugInfo() const override;
	void SetMapPinAddr(stCD_TblItemMapPins* addr);
	bool UpdateImpl() override;

public:
	const std::string& GetMapName() const;

private:
	stCD_TblItemMapPins*	pins_db_;

private:
	std::string				map_name_;
};

class GDbMapPinsMgr : public GGameObjMgrBase, public Singleton<GDbMapPinsMgr, Singleton_MakeMe>{
	typedef std::vector<GDbMapPins>  Cont;
public: 
	GDbMapPinsMgr();
	void DebugInfo() const override;

protected:
	bool RebuildAllImpl() override;
	
private:
	Cont			map_pins_db_;
};

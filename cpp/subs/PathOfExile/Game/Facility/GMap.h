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
#include "GType.h"
#include "GameObjMgr.h"
#include <Data/public/CG_Public.h>
#include "GClass.h"

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
	bool CanResetOpenMap() const;
	bool DoOpen() override;

public:
	bool UpdateImpl() override;

private:
	bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res) override;
	int GetCanOpenedDistance() const override;
	bool MovingDoTryOpen() const override;
	bool MustCanTargetable() const override;

private:
	GClass::stLastMapInfo		last_map_info_;
	enCD_kMapDifficulty			map_difficulty_;
	bool						is_town_area_;
};

//非人类的npc，比如仓库、告示板、活动告示板，流放者的信属于NPC
class GNoHuman : public GameObjBase{
public:
	GNoHuman(enGameObjType obj_type);

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
	stMapDataType(stCD_MapData& map_data, const stCD_VecInt& map_size, const stCD_VecInt& translation);
	inline const stCD_VecInt& GetMapSize() const{
		return map_size_;
	}
	inline bool CanMoveForValidRect(const stCD_VecInt& pos) const{
		return (map_data_.*fn_)(pos, false) > kMDF_Mountain;
	}
	inline bool CanMove(const stCD_VecInt& pos) const{
		return (map_data_.*fn_)(pos, false) > kMDF_Mountain;
	}
	const stCD_VecInt& GetTranslation() const{
		return translation_;
	}

private:
	stCD_VecInt						map_size_;
	stCD_VecInt						translation_;
	stCD_MapData&					map_data_;
	stCD_MapData::Fn_GetDstMapData	fn_;
};

template<int MaxSideSize, int SideSize_>
using GAStarBase = CmnAStar<stCD_VecInt, stMapDataType, char,
	MaxSideSize,
	SideSize_,
	kAStar_RectifyRadius,			//缩放之允许调整的半径，未缩放的坐标系。这个数值不应过小，否则某些地图调整不了坐标，比如在浮桥上
	kAStar_CompressRadius,			//压缩半径，未缩放的坐标系
	kAStar_CompressRadius,			//射线探路之step，未缩放的坐标系
	false>;

typedef GAStarBase<200, 200> SmallAStar;

//这些参数不能随便乱改，服务器拉回次数过多的话，容易封号。
class G_AStar : public GAStarBase<
	CG_kMaxAStarSize,		//最大边长尺寸
	800						//边长尺寸，不能少于500，否则第一个地图都过不去
	>{
public:
	static G_AStar& GetMe();
	stCD_VecInt CastToGamePos_(int my_pos_x, int my_pos_y);
	bool GenRaySteps(const stCD_VecInt& pos_dst, PosContT& out_pos_info);
	void SetMapZoomRate(const std::string& map_name, int zoom_rate);
	int GetMapSideSize(const std::string& map_name) const;
	bool CanLineMoveTo(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst);
	void DebugPosInfo1(bool dbg_data, const std::string& file_name) const;
	void DebugPosRectInfo1(const std::string& file_name, int range, bool is_src_map_data) const;
	void DebugPosRectInfo2(const std::string& file_name, const stCD_VecInt& pos, int range, bool is_src_map_data) const;
	void DebugRectDataInfo1(const std::string& file_name, int range, bool is_src_map_data) const;
	void DebugRectPosDataInfo1(const std::string& file_name, int range, bool is_src_map_data) const;

private:
	std::map<std::string, int>		map_zoom_rate_;
};

class GPathMark : public CmnPathMark<G_AStar,
	200,	//最大边长尺寸
	100		//矩形，从中心点到边的长度
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
	typedef ::GClass::stLastMapInfo stLastMapInfo;
	typedef G_AStar::AStarBaseType AStarBaseType;
	//不要随便定义该对象，因为这个结构体非常大，小心堆栈不够用哦
	struct stMemoryMapInfo{
		AStarBaseType			astar_data_;
		GameObjMgrCont			obj_mgr_;
		std::string				map_name_;
		GAreaResolver			area_resolver_;
		GAreaConnectionCont		area_connections_;
	};
	typedef std::vector<stMemoryMapInfo> MemoryMapContT;

public:
	GMap();
	bool LastEqual() const;
	const stCD_VecInt& GetMapSize() const;
	bool Update() override;
	const std::string& GetMapName() const;
	inline int GetChapter() const{ return chapter_; }
	bool BeginChangeMap(stLastMapInfo& last_map_info);
	bool HasChangedMap(const stLastMapInfo& last_map_info) const;
	bool BeginChangeMap();
	bool HasChangedMap() const;
	bool UpdateArea(const stCD_VecInt& center_pos) const;
	inline bool IsCangShenChu() const { return  map_name_.find(kMapName_CangShenChu) != std::string::npos; /*map_name_.find(kMapName_CangShenChu) != std::string::npos;*/ }

	//只供脚本用
public:
	bool UpdateCurMap_();
	bool UpdateSelfArea_();

	//SmallAstar相关
public:
	SmallAStar* GetSmallAStar(const stCD_VecInt& center_pos) const;
	void DebugSmallAStarPosInfo(bool dbg_data, const std::string& file_name) const;

	//地图房间相关
public:
	void ClearAllMarkInfo();
	
private:
	void DoUpdated();
	bool UpdateImpl() override;
	void RememberLastMapObjInfo();
	void ParseCurMemoryMapInfo();
	stMemoryMapInfo& GetFreeMemoryMapInfo(const std::string& map_name);
	bool InitNewAstar(stCD_World& new_world);
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
	mutable SmallAStar		small_astar_;
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
	typedef std::vector<stVertex> MapNameVertex;
	struct stMoveHelp{
		stMoveHelp() : type_(kGOT_AreaTransition){}

		std::string			begin_name_;
		std::string			end_name_;
		std::string			ori_map_;
		enGameObjType			type_;
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
	bool GetAllTownName(MapNameVertex& all_town) const;
	int GetMapCurChapter(const std::string& dst_map_name) const;
	bool HasTheMapName(const std::string& map_name) const;

public:
	void DebugInfo() const override;
	void DbgAllVertices() const;
	void DbgAllEdges() const;
	bool RebuildAll() override;

protected:
	bool RebuildAllImpl() override;
	bool GetDijkLatelyPathCurDiff(const std::string& dst_map_name, DequeHelp& path_help) const;
	bool GetDijkLatelyPathByWay(const std::string& dst_map_name, enCD_kMapDifficulty dst_Diff_lvl, DequeHelp& path_help) const;
	bool FilterLinkMap(const stVertex& src, const stVertex& dst)  const;
	//bool UserDefinedLinkMap();

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
	bool GetWayPointName(std::string& dst_map, enCD_kMapDifficulty diff_lvl);
	bool GetWayPointDstName(std::string& dst_map,enCD_kMapDifficulty diff_lvl);
	bool CalcMapLatelyPath(GSP_Resolver::MapNameCont& src_lately_map, enCD_kMapDifficulty diff_lvl);
	bool IsOpenedWayPoint(const std::string& dst_name, enCD_kMapDifficulty diff_lvl);
	const std::string& GetCanWayMapByChapter(int chapter);

protected:
	bool RebuildAllImpl() override;	

private:
	GateConT	way_point_info_;
	bool		open_way_state_;
};

class GTranstionable : public GameObjBase{
	friend class GDbMapPins;
public:
	GTranstionable(enGameObjType obj_type);

public:
	void DebugInfo() const override;
	//bool GetOpenState() const;
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
	//bool			is_opened_;
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
//////////////////////////////////////////////////////////////////////////
class GTerrainIterator{
public:
	GTerrainIterator(const stCD_MapData& map_data);
	void First();
	bool IsDone() const;
	void Next();
	stCD_VecInt CurShrinkPos() const;
	stCD_VecInt CurPos() const;
	stCD_MapDataTerrainModel* CurTerrain() const;
	pt_cwsz CurModelPath() const;

private:
	void NextValid();

private:
	const stCD_MapData&		map_data_;
	int						cur_idx_;
	int						max_idx_;
};

class IMapTerrainParser{
public:
	virtual ~IMapTerrainParser(){}
	virtual bool ParseTerrain(GTerrainIterator& terrain_iterator, GTerrainPosName2Pos& out_res) = 0;
};

template<typename FnT>
class GMapTerrainParserT : public IMapTerrainParser{
public:
	GMapTerrainParserT(const FnT& fn) : fn_(fn){}
	bool ParseTerrain(GTerrainIterator& terrain_iterator, GTerrainPosName2Pos& out_res) override{
		return fn_(terrain_iterator, out_res);
	}

private:
	FnT		fn_;
};
template<typename FnT>
MapTerrainParserPtr MakeMapTerrainParser(const FnT& fn){
	return MapTerrainParserPtr(new GMapTerrainParserT<FnT>(fn));
}

class GTerrainParser{
public:
	typedef std::vector<stTerrainTagInfo> TagInfoCont;
	typedef std::map<std::string, TagInfoCont> TagInfoMap;
	typedef std::pair<const stTerrainTagInfo*, const stTerrainTagInfo*> AreaConnection;
	typedef std::pair<std::string, std::string> AreaNameConnection;
	typedef std::vector<AreaNameConnection> AreaNameConnectionCont;
	typedef std::map<std::string, MapTerrainParserPtr> MapParser;

public:
	void AddAny(enTerrainParserFlag flag, const std::string& pos_name, const std::string& tag_str);
	void AddMap(const std::string& map_name, enTerrainParserFlag flag, const std::string& pos_name, const std::string& tag_str,
		const std::string& dst_pos_name);
	void AddMap_(const std::string& map_name, const std::string& pos_name, const std::string& tag_str);
	void AddMap1_(const std::string& map_name, const std::string& pos_name, const std::string& tag_str, const std::string& dst_pos_name);
	void AddMapArea(const std::string& map_name, const std::string& pos_name, const std::string& tag_str, const luabind::object& terrain_area,
		const std::string& dst_pos_name);
	void AddMapArea_(const std::string& map_name, const std::string& pos_name, const std::string& tag_str, const luabind::object& terrain_area);
	void AddMapParser(const std::string& map_name, const MapTerrainParserPtr& parser, const AreaNameConnectionCont& connections);
	bool Parse(const std::string& map_name, const stCD_MapData& map_data);
	static GTerrainParser& GetDefTerrainParser();
	const stTerrainTagInfo* FindTerrain(const std::string& pos_name) const;
	const TagInfoCont* GetTheMap(const std::string& map_name) const;
	static AreaConnection GetAreaConnection(const TagInfoCont& tags_info, int area_id_from, int area_id_dst);
	void TestDetectOneTerrainArea(int radius, int terrain_cnt) const;
	void TestDetectUniqueTerrainByPos(const luabind::object& positions) const;

public:
	bool ParseUnique(TagInfoCont* map_info, const stCD_MapData& map_data);
	bool ParseTerrainAreas(TagInfoCont* map_info, const stCD_MapData& map_data);
	bool ParseOneTerrainArea(stTerrainTagInfo& tag_info, const stCD_MapData& map_data, bool is_unique) const;
	bool ParseByParser(TagInfoCont* map_info, const stCD_MapData& map_data);
	bool DoParseConnections(TagInfoCont* map_info);

	//解析地形
private:
	//古金字塔
	static bool ParseGuJinZiTa(GTerrainIterator& terrain_iterator, GTerrainPosName2Pos& out_res);

private:
	static void AddPosNameWithPos(GTerrainPosName2Pos& out_res, const std::string& pos_name, const stCD_VecInt& pos);
	void SetTerrainPosNamePos(TagInfoCont& tags_info, const std::string& pos_name, const stCD_VecInt& pos);
	
private:
	TagInfoCont		any_tags_info_;
	TagInfoMap		map_tags_info_;
	MapParser		map_parser_;
};
//////////////////////////////////////////////////////////////////////////
class IAreaVertex{
public:
	virtual ~IAreaVertex(){}
	virtual bool IsValid() const = 0;
	virtual const stCD_VecInt& GetValidPos() const = 0;
	virtual AreaVertexPtr GetConnectedRoom() const = 0;
	virtual const std::string& GetPosName() const = 0;
	virtual GameObjBasePtrT GetTransitionable() const = 0;
};

class AreaVertexDirect : public IAreaVertex{
public:
	AreaVertexDirect();
	AreaVertexDirect(const stTerrainTagInfo* terrain_info);
	bool IsValid() const override;
	const stCD_VecInt& GetValidPos() const override;
	AreaVertexPtr GetConnectedRoom() const override;
	const std::string& GetPosName() const override;
	GameObjBasePtrT GetTransitionable() const override;

private:
	const stTerrainTagInfo*	terrain_info_;
};

class AreaVertexGuess : public IAreaVertex{
public:
	AreaVertexGuess();
	bool IsValid() const override;
	const stCD_VecInt& GetValidPos() const override;
	AreaVertexPtr GetConnectedRoom() const override;
	const std::string& GetPosName() const override;
	GameObjBasePtrT GetTransitionable() const override{ return transitionable_; }
	void SetTransitionable(const GameObjBasePtrT& obj);
	void SetPos(const stCD_VecInt& pos);
	void SetDst(const AreaVertexGuessPtr& dst);
	void SetAreaId(int area_id);
	int GetAreaId() const;
	inline enAreaPassState GetPassState() const{ return pass_state_; }
	inline void SetAreaState(enAreaPassState state){ pass_state_ = state; }

private:
	GameObjBasePtrT			transitionable_;
	stCD_VecInt				transitionable_pos_;
	mutable int				area_id_;
	AreaVertexWeakPtr		dst_vertex_;
	enAreaPassState			pass_state_;
};

class IMapTraPolicy{
public:
	virtual bool GenAreaSteps(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, GAreaStepsType& steps) const = 0;
	virtual bool CalcSuggestAreaStep(GAreaStepsType& steps) const = 0;
	virtual AreaVertexPtr FindAreaByName(const std::string& area_name) const = 0;
	virtual CA_ActionPtr MakeTraMap(const GA_SmartConsumerT& consumer, const CA_ActionPtr& action_tra_do, bool clear_mark_pos) const = 0;
};

class GMapTraPolicyBase : public IMapTraPolicy{
public:
	GMapTraPolicyBase();
	GAreaResolver& GetAreaResolver(){ return area_resolver_; }
	bool GenAreaSteps(const stCD_VecInt& pos_src, const stCD_VecInt& pos_dst, GAreaStepsType& steps) const override;
	bool CalcSuggestAreaStep(GAreaStepsType& steps) const override;
	CA_ActionPtr MakeTraMap(const GA_SmartConsumerT& consumer, const CA_ActionPtr& action_tra_do, bool clear_mark_pos) const override;

protected:
	void AddAreaConnection(int vertex_lhs, int vertex_rhs);
	virtual bool GenAreaStepsImpl(int area_from, int area_dst, GAreaStepsType& steps) const = 0;
	virtual bool GenUnPassedAreaSteps(GAreaStepsType& steps) const = 0;

protected:
	GAreaResolver		area_resolver_;
	mutable bool		is_suggest_;
};

//直接遍历
class GMapTraDirect : public GMapTraPolicyBase{
public:
	bool ParseAreaConnections();
	AreaVertexPtr FindAreaByName(const std::string& area_name) const override;

private:
	bool GenAreaStepsImpl(int area_from, int area_dst, GAreaStepsType& steps) const override;
	bool GenUnPassedAreaSteps(GAreaStepsType& steps) const override;
};

//猜测遍历
class GMapTraGuess : public GMapTraPolicyBase{
public:
	GAreaConnectionCont& GetAreaConnections(){ return area_connections_; }
	bool ParseAreaConnections(const GAreaResolver& area_resolver);
	AreaVertexPtr FindAreaByName(const std::string& area_name) const override;
	AreaVertexPtr FindAreaById(int obj_id) const;
	void AddConnection(const AreaVertexGuess& lhs, const AreaVertexGuess& rhs);
	bool GenUnPassedAreaSteps(GAreaStepsType& steps) const override;
	//寻找一个没有通过的门
	GameObjBasePtrT FindUnPassedDoor() const;
	const std::string& GetRoomNameByAreaId(int area_id) const;

private:
	bool GenAreaStepsImpl(int area_from, int area_dst, GAreaStepsType& steps) const override;
	bool GenAreaStepsImpl_(bool is_suggest, int area_from, int area_dst, GAreaStepsType& steps) const;
	GAreaConnectionType GetAreaConnection(int area_from, int area_dst) const;
	AreaVertexGuessPtr FindVertex(const GameObjBasePtrT& obj) const;
	AreaVertexGuessPtr FindVertex(const std::string& obj_name) const;
private:
	GAreaConnectionCont		area_connections_;
};

//探索遍历
//...

class GMapTraMgr : public Singleton<GMapTraMgr, Singleton_MakeMe>{
public:
	typedef std::vector<std::string> AreaNameCont;
	struct stMapAreaInfo{
		AreaNameCont	area_names_;
	};
	typedef std::map<std::string, stMapAreaInfo> MapAreaInfoMap;

public:
	GMapTraGuess& GetTraGuess(){ return tra_guess_; }
	bool InitParseTerrain();
	IMapTraPolicy* GetCurPolicy();
	IMapTraPolicy* GetDirectPolicy() { return &tra_direct_; };
	void SetMapAreaInfo(const std::string& map_name, const luabind::object& areas);
	bool HasTheMapAreaObj(const GameObjBasePtrT& obj) const;
	bool HasTheMapAreaName(const std::string& area_name) const;
	bool HasMapAreaByCurMap(const std::string& map_name) const;
	void AddTransitionableConnect(const AreaVertexGuess& lhs, const AreaVertexGuess& rhs);

private:
	GMapTraDirect			tra_direct_;
	GMapTraGuess			tra_guess_;
	MapAreaInfoMap			map_area_infos_;
};
//////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------
class GLabyrinthItem : public GameObjBase{
	typedef std::vector<GClass::stLabyrinthMapState>	ContT;

public:
	GLabyrinthItem() :is_actived_(false){};
	void SetLabyrinthUiItem(stCD_UiLabyrinthList::stListItem* ui_item);
	void DebugInfo() const override;
	bool SeletLabyrinth() const;
	inline pt_dword GetLevel() const{ return min_level_; }
	inline bool GetActivedState() const{ return is_actived_; }

protected:
	bool UpdateImpl() override;

private:
	stCD_UiLabyrinthList::stListItem* item_;

private:
	ContT						map_Labyrinth_;
	pt_dword					min_level_;
	bool						is_actived_;
};

class GLabyrinthTrialDlg : public GGameObjMgrBase, public Singleton<GLabyrinthTrialDlg, Singleton_MakeMe>{
	typedef std::vector<GLabyrinthItem> ContT;
public:
	GLabyrinthTrialDlg(){};
	bool ActiveButton() const;
	void DebugInfo() const override;
	bool SelectLabyrinthByLevel(pt_dword select_level) const;

protected:
	bool RebuildAllImpl() override;

private:
	ContT		labyrinth_Dlg_List_;
};

class GTrialPlaqueDlg : public GGameObjMgrBase, public Singleton<GTrialPlaqueDlg, Singleton_MakeMe>{
	typedef std::vector<GClass::stLabyrinthMapState> ContT;
public:
	GTrialPlaqueDlg(){};
	void DebugInfo() const override;
	const GClass::stLabyrinthMapState* GetUndoneMap() const;

protected:
	bool RebuildAllImpl() override;

private:
	ContT		labyrinth_Plaque_List_;
};


//////////////////////////////////////////////////////////////////////////
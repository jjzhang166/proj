#pragma once
/*
@author zhp
@date 2014/8/12 16:25
@purpose village or clone map. map == village or clone map
*/
#include <Common/UsefulClass.h>
#include <Common/GameObjBase.h>
#include "GClass.h"
#include "GAddr.h"

class GStageTemplate : public GClass::BTreeBase{
public:
	enum enStageType
	{
		kST_CloneMap = 0,
		kST_Town,
		kST_Unknown,
		kST_PVP,
	};
	class GateLinksInfo : public GClass::GGameTypeBase{
	public:
		class GateLink : public GClass::GGameTypeBase{
		public:
			struct stGameGate{
				DWORD				str_dwVFunAddr;
				GClass::stGameString		str_theStageId;			// id
				GClass::stGameString		str_theSectorId;		// 房间序号
				GClass::stGameString		str_theWarpGateId;		// 门
				BYTE				str_bEnable;			// 是否启用 1为可通过，0为不可通过
				BYTE				str_bUnknow1;
				WORD				str_wUnknow2;
			};
			static_assert(sizeof(stGameGate) == sizeof(WARP_GATE), "");

			struct stGameGateLink{
				stGameGate			gate_src_;
				stGameGate			gate_dst_;
			};
			static_assert(sizeof(stGameGateLink) == sizeof(WARP_GATE_LINK), "");

			typedef stGameGateLink GameType;
			struct stGateInfo{
				std::wstring			stage_id_;			//城镇或副本ID
				std::wstring			sector_id_;			//房间序号
				std::wstring			warp_gate_id_;
				bool					enable_;

				stGateInfo();
				bool CopyFrom( const stGameGate& game_gate );
			};

		public:
			bool Update();

		public:
			stGateInfo			gate_src_;		//从源门进去
			stGateInfo			gate_dst_;		//目标门出去
		};
		class GateLinkMgr : public GClass::GGameTArray<std::vector<GateLink> >{
		public:
			bool RebuildUpdateAll();
		};
		struct stGateLinksInfo{
			DWORD						str_dwVFunAddr;
			GateLinkMgr::stGameTArray		str_theWarpGeteLinkInfosNode;
		};
		static_assert(sizeof(stGateLinksInfo) == sizeof(WARP_GATE_LINK_INFOS_NODE), "");

		typedef stGateLinksInfo GameType;

	public:
		bool Update();
		const GateLinkMgr& GetGateLinkMgr() const;

	public:
		GateLinkMgr			gate_link_mgr_;
	};
	class GateLinksInfoMgr : public GClass::GGameTArray<std::vector<GateLinksInfo> >{};

	typedef GateLinksInfo::GateLink GateLink;
	typedef GateLink::stGateInfo GateInfo;

	//房间
	class GSector : public GClass::BTreeBase{
	public:
		class GFixedNpc : public GClass::GGameTypeBase{
		public:
			struct stFixedNpc{
				DWORD				str_dwVFunAddr;
				GClass::stGameString		str_theId;
				FLOAT				str_fPositionX;
				FLOAT				str_fPositionY;
				XYZ					str_theDirection;
				XYZ					str_theScale;
				struct  
				{
					DWORD			str_dwVFunAddr;	
					DWORD			str_dwStamps;
					GAME_STRUCT_ARRAY_POINT	str_thefixedNpcMovement;
				}theMovementInfo;
			};
			static_assert(sizeof(stFixedNpc) == sizeof(FIXD_NPC), "FIXD_NPC");
			typedef stFixedNpc GameType;

		public:
			bool Update();

		private:
			std::wstring			fixed_npc_id_;
		};
		class GFixedNpcTArray : public GClass::GGameTArray<std::vector<GFixedNpc> >{};

	public:
		GSector();
		bool Update();
		const std::wstring& GetSectorId() const;
		bool IsBossRoom() const;

	private:
		std::wstring			sector_id_;
		bool					boss_room_;			//是否boss房间
		GFixedNpcTArray			fixed_npcs_;
	};
	class GSectorsMgr : public GClass::BTreeMgr_SkipRoot<GSector>{
	public:
		const GSector* GetBossSector() const;
	};
	class GameRoomBoss : public GClass::BTreeBase{
	public:
		bool Update();

	private:
		std::wstring				boss_name_;
	};
	class GameRoomBossMgr : public GClass::BTreeMgr_SkipRoot<GameRoomBoss>{
	};

public:
	GStageTemplate();
	bool Update();
	const GateLinksInfoMgr& GetGateLinksInfo() const;
	bool IsTown() const;
	bool IsCloneMap() const;
	const std::wstring& GetStageId() const;
	const GSectorsMgr& GetSectors() const;
	const std::wstring& GetStartingSectorId() const;
	int FatigueOfUse() const;
	int GetRecommandLvlLower() const;
	int GetRecommandLvlUpper() const;
	bool ValidLvlBetweenRecommandLvl( int the_lvl ) const;
	bool ValidRangeThanRecommandUpperLvl( int the_lvl ) const;
	bool IsNormalRecommandLvlFb() const;
	bool IsArenaStage() const;

private:
	GateLinksInfoMgr				link_infos_;
	enStageType						stage_type_;
	std::wstring					stage_id_;
	std::wstring					starting_sector_id_;			//first room
	GSectorsMgr						sectors_mgr_;
	GameRoomBossMgr					game_room_boss_mgr_;
	int								fatigue_of_use_;
	int								recommand_lvl_lower_;			//建议最低等级
	int								recommand_lvl_upper_;
	bool							is_arena_stage_;				//是否是角斗场
};

class GMap : public GGameObjBase, public Singleton<GMap, Singleton_MakeMe>{
public:
	class ClearSector : public GClass::BTreeBase{
	public:
		bool Update();
		const std::wstring& GetClearedSector() const;

	private:
		std::wstring	cleard_sector_;
	};
	class ClearSectorsMgr : public GClass::BTreeMgr_SkipRoot<ClearSector>{};

public:
	GMap();

	//interface
public:
	//是否在城镇中
	bool IsTown() const;
	bool Update();
	bool UpdateClearSectors();
	bool UpdateClearSectors( const GClass::BTreeMgrBase::stGBTreeNodes* the_node );
	bool UpdateClearSectorsSync();
	GType::enFbDifficulty GetDifficulty() const;
	const std::wstring& GetMapId() const;
	const std::wstring& GetBossRoomName() const;
	const GStageTemplate* GetStageTemplate() const;
	bool ExistTheClearedSector( const std::wstring& sector_id ) const;
	std::wstring GetCurMapFbChName() const;

private:
	bool DoUpdateImpl();

private:
	//bool					is_town_;			//是否城镇
	std::wstring			map_id_;
	GType::enFbDifficulty	map_lvl_;			//副本等级，城镇等级都为0
	const GStageTemplate*	stage_template_;
	GClass::TimerSyncExec	timer_sync_exec_;
	ClearSectorsMgr			the_clear_sectors_;
};

class GStageTemplateMgr : public GClass::BTreeMgr_SkipRoot<GStageTemplate>, public Singleton<GStageTemplateMgr, Singleton_MakeMe>{
public:
	bool UpdateAll();
	bool RebuildUpdateAll();
	const GStageTemplate* FindStage( const std::wstring& stage_id );
};

//城镇到副本的映射
class GVillageFbMap : public Singleton<GVillageFbMap, Singleton_Me>{
public:
	typedef std::multimap<std::wstring, std::wstring> MapT;

public:
	GVillageFbMap();
	const std::wstring* Find_Fb2Village( const std::wstring& clone_map_id ) const;

private:
	void AddMap( const std::wstring& village_id, const std::wstring& clone_map_id );

private:
	//MapT		village_fb_map_;

private:
	static GVillageFbMap			map_instance_;
};

//城镇与副本之间的映射关系
class GRegionStageMapTemplate : public GClass::BTreeBase{
public:
	class SubStage : public GClass::GGameTypeBase{
	public:
		typedef GClass::stGameString GameType;
		static_assert(sizeof(GameType) == sizeof(GAME_STRING_ID), "");

	public:
		bool Update();
		const std::wstring& GetSubStageId() const;

	private:
		std::wstring			stage_id_;
	};
	class SubStageTArray : public GClass::GGameTArray<std::vector<SubStage> >{
	public:
		const SubStage* Fb2Vaillage( const std::wstring& fb_id ) const;
		bool ExistTheStageId( const std::wstring& stage_id ) const;
	};

public:
	bool Update();
	const SubStageTArray& GetSubStages() const;
	bool ObjValid();

private:
	std::wstring				region_id_;
	std::wstring				region_name_;			//Region.Region01.Episode05_2.Name >> 莱温后街
	SubStageTArray				sub_stages_;
};

class GRegionStageMapTemplateMgr : public GClass::BTreeMgr_SkipRoot<GRegionStageMapTemplate>, public Singleton<GRegionStageMapTemplateMgr, Singleton_MakeMe>{
	bool RebuildUpdateAll();
	bool UpdateAll();

public:
	const GRegionStageMapTemplate* FindByStageId( const std::wstring& stage_id ) const;
	const GRegionStageMapTemplate::SubStage* Fb2Village( const std::wstring& fb_id ) const;
};
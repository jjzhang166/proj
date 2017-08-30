#pragma once
/*
@author zhp
@date 2017/2/5 22:39
@purpose for skills
*/
#include <string>
#include <Common/UsefulFunc.h>
#include <Common/GameObjBase.h>
#include "GInterface.h"
#include <WgFacility/CmnShortestPaths.h>
#include <hash_map>

class GDbPassiveSkill : GGameObjBase{
	friend class GDbPassiveSkillMgr;

public:
	GDbPassiveSkill();
	void DebugInfo() const override;

protected:
	bool UpdateImpl() override;

private:
	const stCD_TblItemPassiveSkill*	passive_db_;

private:
	std::wstring	ch_name_;
	pt_dword		skill_id;
};

//天赋树DB
class GDbPassiveSkillMgr : public GGameObjMgrBase, public Singleton<GDbPassiveSkillMgr, Singleton_MakeMe>{
	typedef std::hash_map<std::wstring, stCD_TblItemPassiveSkill*> PassiveSkillHashMap;
	typedef CmnStaticVector<stCD_TblItemPassiveSkill*, kPT_Mask> StartedPassiveSkills;

public:
	GDbPassiveSkillMgr();
	bool GetPassiveSkill(const std::wstring& skill_name, GDbPassiveSkill& out) const;
	stCD_TblItemPassiveSkill* FindPassiveSkill(const std::wstring& skill_name) const;
	stCD_TblItemPassiveSkill* FindPassiveSkill(pt_word skill_id) const;
	stCD_TblItemPassiveSkill* FindStartedPassiveSkill(enCD_ProfessionType profession_type) const;
	stCD_TblItemPassiveSkill* FindStartedPassiveSkill(const std::wstring& profession_name) const;
	bool RebuildAll() override;

protected:
	bool RebuildAllImpl() override;

private:
	stCD_PassiveSkillHashMap*	game_passive_skills_;
	PassiveSkillHashMap			passive_skills_;
	StartedPassiveSkills		started_passive_skills_;		//起点技能
};

class GDbActiveSkill : public GGameObjBase{
	friend class GDbActiveSkillMgr;
public:
	GDbActiveSkill();
	void DebugInfo() const override;

protected:
	bool UpdateImpl() override;

private:
	stCD_TblItemActiveSkills* skill_db_;

private:
	std::wstring			ch_name_;
};

class GDbActiveSkillMgr : public GGameObjMgrBase, public Singleton<GDbActiveSkillMgr, Singleton_MakeMe>{
	typedef std::vector<GDbActiveSkill>  Cont;
public:
	GDbActiveSkillMgr();
	bool RebuildAll() override;
	void DebugInfo() const override;

protected:
	bool RebuildAllImpl() override;

private:
	Cont			skillDb_;
};

class GSkillObj : public GGameObjBase{
public:
	GSkillObj();
	bool SetSkillInfo(stCD_SkillInfo* skill_info);
	const std::string& GetSkillName() const{ return skill_name_; }
	pt_word GetSkillId() const{ return skill_id_; }
	pt_dword GetCallerMaxCnt() const{ return caller_max_cnt_; }
	void DebugInfo() const override;

protected:
	bool UpdateImpl() override;

private:
	stCD_SkillInfo*				game_skill_info_;

private:
	std::string					skill_name_;
	pt_dword					caller_max_cnt_;
	pt_word						skill_id_;
};

class GSkillMgr : public GGameObjMgrBase, public Singleton<GSkillMgr, Singleton_MakeMe>{
	typedef std::vector<GSkillObj> Cont;
public:
	GSkillMgr();
	const GSkillObj* FindByName(const std::string& skill_name) const;
	const GSkillObj* FindById(int skill_id) const;

public:
	void DebugInfo() const override;

protected:
	bool RebuildAllImpl() override;

private:
	Cont			skills_;
};

class GBuffMgr : public GGameObjBase, public Singleton<GBuffMgr, Singleton_MakeMe>{
public:
	typedef std::vector<pt_dword> BuffEntityIdCont;
	struct stBuffItem{
		std::string			buff_name_;
		BuffEntityIdCont	buff_entity_ids_;
	};
	typedef std::vector<stBuffItem> BuffItems;

public:
	const stBuffItem* FindBuff(const GSkillObj& skill_obj) const;
	bool ExistEntityId(pt_dword entity_id) const;

private:
	bool UpdateImpl() override;

private:
	BuffItems		buff_items_;
};

class GAutoUseSkill{
	struct stUseSkillInfo{
		stUseSkillInfo() :use_gap_time_(100){
			skill_consume_min_mp_ = 0;
			skill_consume_max_mp_ = 0;
			skill_consume_min_hp_ = 0;
			skill_consume_max_hp_ = 0;
			use_count_ = 1;
			fire_distance_ = 0;
		}
		std::string  skill_name_;
		float		 skill_consume_min_mp_;
		float	     skill_consume_max_mp_;
		float		 skill_consume_min_hp_;
		float		 skill_consume_max_hp_;
		TimeDuration use_gap_time_;
		int			 use_count_;
		int			 fire_distance_;
	};
	typedef std::vector<stUseSkillInfo>  SkillSelectConT;

public:
	GAutoUseSkill();
	
public:
	const std::string& GetOneUseSkill(int& count);
	void AddFireSkill(const std::string& skill_name, float time_gap, int fire_cnt, float hp_min_limit, float hp_max_limit, float mp_min_limit, float mp_max_limit);
	void ClearAllUseSkill();

private:
	SkillSelectConT		fire_skill_cont_;
};

class GPassiveSkillGraphMgr : public GGameObjBase{
	typedef stCD_PassiveSkillGraphMgr::SkillIdContT SkillIdContT;

public:
	GPassiveSkillGraphMgr();
	int GetPassiveSkillPoints() const{ return passive_skill_points_; }
	SkillIdContT& GetLearnedSkillIds(){ return learned_skill_ids_; }

private:
	bool UpdateImpl() override;

private:
	SkillIdContT	learned_skill_ids_;
	int				passive_skill_points_;		//天赋点数，没有点的天赋点数
};

class GPassiveSkillResolver : public Singleton<GPassiveSkillResolver, Singleton_MakeMe>{
public:
	typedef stCD_TblItemPassiveSkill* stVertex;
	typedef CSP_Resolver<stVertex, int, GPassiveSkillPathsCont> ResolverImplT;
	typedef ResolverImplT::StepsContType StepsContType;
	typedef ResolverImplT::StepsWeightContType StepsWeightContType;
	typedef std::vector<std::wstring> SkillNameCont;
	struct stNodesInfo{
		SkillNameCont		big_nodes_;
		SkillNameCont		small_nodes_;

		bool ExistSmallNode(stCD_TblItemPassiveSkill* passive_skill) const;
	};
	typedef std::map<std::wstring, stNodesInfo> JobNodesInfo;
	typedef std::pair<stCD_TblItemPassiveSkill*, stCD_TblItemPassiveSkill*> PassiveSkillPair;

public:
	GPassiveSkillResolver();
	void Init();
	void SetNodesInfo(const luabind::object& job_skill_nodes);
	int AutoLvlUpPassiveSkills();
	void DebugInfo(const std::wstring& first_node, const std::wstring& last_node);

private:
	bool GenOncePath(StepsContType& steps);
	const stNodesInfo* GetCurJobNodesInfo() const;
	const stNodesInfo* ReGenCurJobNodes();
	bool GenGraphOnce();
	PassiveSkillPair CalcVertexPorts(GPassiveSkillGraphMgr& graph_mgr, const SkillNameCont& nodes) const;

private:
	ResolverImplT			resolver_;
	JobNodesInfo			job_nodes_;
	mutable bool			need_regen_;
	mutable std::wstring	last_job_name_;
	static const int		kEdgeWeightDef = 3;
	static const int		kEdgeWeightLoved = 1;
};

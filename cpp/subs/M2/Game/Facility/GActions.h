#pragma once
/*
@author zhp
@date 2017/4/27 19:06
@purpose for actions
*/
#include <Actions/CA_Action.h>
#include "GType.h"
#include <CommonX/CmnX_UsefulClass.h>
#include "GMap.h"

//////////////////////////////////////////////////////////////////////////
typedef CmnAStarDanger<G_AStar> AStarDangerT;
typedef CmnAStarSafer<G_AStar> AStarSaferT;
//////////////////////////////////////////////////////////////////////////
class GA_TesterAnyObj : public CA_FastTester{
public:
	GA_TesterAnyObj(const GameObjMgrPtr& obj_mgr);
	GameObjBasePtrT& GetFirstObj();
	void clear() override;

protected:
	bool Test() override;

private:
	GameObjMgrPtr		obj_mgr_;
	GameObjBasePtrT		first_obj_;
};

class GA_TesterNearest : public CA_FastTester{
public:
	GA_TesterNearest(const GameObjBasePtrT& dst_obj);
	void SetDstObj(const GameObjBasePtrT& dst_obj);

protected:
	bool Test() override;

private:
	GameObjBasePtrT		dst_obj_;
	GameObjMgrPtr		obj_mgr_;
};

class GA_TesterByName : public CA_FastTester{
public:
	GA_TesterByName(const std::string& obj_name, const GameObjMgrPtr& obj_mgr);
	GameObjBasePtrT GetTheObj() const;

protected:
	bool Test() override;

private:
	std::string			obj_name_;
	GameObjMgrPtr		obj_mgr_;
	GameObjBasePtrT		the_obj_;
};

class GA_TesterInRange : public CA_FastTester{
public:
	GA_TesterInRange(const CA_T<GA_OpenObjs>& action_open_objs);
	~GA_TesterInRange();
	void SetButObj(const GameObjBasePtrT& but_obj);
	static bool HasTheObjs(const GameObjMgrPtr& obj_mgr);
	static bool HasTheObjs(const GameObjMgrPtr& obj_mgr, const GameObjBasePtrT& but_obj);

protected:
	bool Test() override;

public:
	GameObjMgrPtr			obj_mgr_;
	GameObjBasePtrT			but_obj_;

private:
	GFilterGameObjPtr		filter_alive_;
	GFilterGameObjPtr		filter_hittable_;
	CA_WeakT<GA_OpenObjs>	action_open_objs_;

private:
	static const int		kMonsterMaxCnt = 1;
};

class GA_LookMonsterDie : public CA_FastTester{
public:
	GA_LookMonsterDie(const std::string& monster_name);

protected:
	bool Test() override;

private:
	std::string			monster_name_;
	GameObjMgrPtr		obj_mgr_;
	GameObjBasePtrT		the_obj_;
	bool				has_the_obj_;
};

class GA_TesterDanger : public CA_FastTester{
public:
	GA_TesterDanger(const GameObjMgrPtr& obj_mgr);
	void SetButObj(const GameObjBasePtrT& but_obj, float but_danger_distance);
	static float ScreenRate2Distance(float screen_rate);
	static void SetDangerDistance(float screen_rate);

protected:
	bool Test() override;

private:
	GameObjMgrPtr		obj_mgr_;
	GameObjBasePtrT		but_obj_;
	float				but_danger_distance_;

private:
	static const int	kDistanceOfScreen = 100;		//屏幕的距离

public:
	static float		danger_distance_;
};
//////////////////////////////////////////////////////////////////////////
//*********************products
class GA_OpenObjs : public CA_ProductT<>{
public:
	GA_OpenObjs(const GameObjMgrPtr& obj_mgr);
	~GA_OpenObjs();
	void AddBlackNameList(pt_dword obj_id);
	GameObjMgrPtr GetObjMgr() const;
	bool CanKillTheObj(const GameObjBasePtrT& obj) const;

protected:
	bool Test() override;

private:
	GameObjMgrPtr				obj_mgr_;
	CA_T<GA_TesterInRange>		tester_in_range_;
	GFilterGameObjPtr			filter_black_name_list_;
	NameListIdT					black_name_list_;
	GameObjBasePtrT				the_obj_;
	GameObjBasePtrT				must_do_obj_;
	GameObjMgrBase::ObjContT	rec_open_obj_;
};

class GA_KillInRange : public CA_ProductT<>{
public:
	GA_KillInRange(const CA_T<GA_OpenObjs>& action_open_objs);
	~GA_KillInRange();

protected:
	bool Test() override;

private:
	CA_T<GA_KillBase>			action_kill_monster_;
	GameObjMgrPtr				obj_mgr_;
	GFilterGameObjPtr			filter_hittable_;
	GFilterGameObjPtr			filter_alive_;
	CA_WeakT<GA_OpenObjs>		action_open_objs_;
};

class GA_KillNpcMonster : public CA_ProductT<>{
public:
	GA_KillNpcMonster(const std::string& npc_name);
	~GA_KillNpcMonster();

protected:
	bool Test() override;

private:
	GFilterGameObjPtr			filter_visit_;
	GFilterGameObjPtr			filter_name_;
	GameObjMgrPtr				obj_mgr_;
	GameObjBasePtrT				the_obj_;
	bool						exec_once_;
};
//////////////////////////////////////////////////////////////////////////
class GA_MoveTo : public CA_Action{
public:
	typedef CmnBufferVector<stCD_VecInt, 1024> PosContT;
	enum enMoveRes{
		kMR_DistError,
		kMR_AStarError,
		kMR_Error,
		kMR_Succeed,
	};

public:
	GA_MoveTo(const stCD_VecInt& dst_pos);
	void SetDstPos(const stCD_VecInt& dst_pos);

protected:
	CA_RunRes OnRun() override;

private:
	static bool OnTimerDo(bool* failed);
	bool GenPath(const stCD_VecInt& pos_src, PosContT& out_pos_info);
	virtual bool GenPathImpl(const stCD_VecInt& pos_src, G_AStar::PosContT& out_pos_info);
	enMoveRes SmartMove();
	static enMoveRes MoveTo(const stCD_VecInt& dst_pos);

protected:
	stCD_VecInt		dst_pos_;
	GameObjMgrPtr	obj_mgr_;

public:
	static const int	kValidDistance = 150;
};

class GA_MoveToNearly : public GA_MoveTo{
public:
	GA_MoveToNearly(const stCD_VecInt& dst_pos);

private:
	bool GenPathImpl(const stCD_VecInt& pos_src, G_AStar::PosContT& out_pos_info) override;
};

class GA_DangerMoveTo : public GA_MoveTo{
public:
	GA_DangerMoveTo(const stCD_VecInt& dst_pos, const GameObjMgrPtr& obj_mgr);
	void SetObjMgr(const GameObjMgrPtr& obj_mgr);

private:
	bool GenPathImpl(const stCD_VecInt& pos_src, G_AStar::PosContT& out_pos_info) override;

protected:
	GameObjMgrPtr		obj_mgr_;
};

class GA_SaferMoveTo : public GA_DangerMoveTo{
public:
	GA_SaferMoveTo(const GameObjMgrPtr& obj_mgr);

private:
	bool GenPathImpl(const stCD_VecInt& pos_src, G_AStar::PosContT& out_pos_info) override;
};

class GA_TraFullMap : public CA_Decorator{
	enum enDoOtherRes{
		kDOR_Continue,
		kDOR_Failed,
		kDOR_Over,
	};

public:
	GA_TraFullMap(bool clear_mark_pos);

protected:
	CA_RunRes OnRun() override;

private:
	enDoOtherRes DoOther();
	bool MakeAssistActions();

private:
	CA_T<GA_MoveTo>			action_move_to_;
	CA_T<CA_EnsureTester>	ensure_tester_;
	enDoOtherRes			do_other_res_;
	int						clear_mark_pos_cnt_;
	bool					clear_mark_pos_;
};
//////////////////////////////////////////////////////////////////////////
class GA_MoveToObjByObj : public CA_Action{
public:
	GA_MoveToObjByObj(bool clear_mark_pos, const GameObjBasePtrT& game_obj);
	GameObjBasePtrT GetTheObj() const;
	static enRunRes HandleRunRes(const GameObjBasePtrT& dst_obj, const CA_RunRes& run_res);

protected:
	CA_RunRes OnRun() override;

private:
	GameObjBasePtrT			game_obj_;
	GameObjMgrPtr			obj_mgr_;
	CA_T<GA_TesterAnyObj>	tester_any_obj_;
	CA_ActionPtr			as_timer_action_;
	CA_ActionPtr			tra_full_map_;
	CA_T<GA_MoveToNearly>	action_move_to_;
	bool					clear_mark_pos_;
};

class GA_MoveToObjByName : public CA_Action{
public:
	GA_MoveToObjByName(bool clear_mark_pos,
		const std::string& obj_name, const GameObjMgrPtr& obj_mgr);
	GameObjBasePtrT GetTheObj() const;

protected:
	CA_RunRes OnRun() override;

private:
	std::string				obj_name_;
	GameObjMgrPtr			obj_mgr_;
	CA_T<GA_TesterAnyObj>	tester_any_obj_;
	CA_ActionPtr			as_timer_action_;
	CA_ActionPtr			tra_full_map_;
	CA_T<GA_MoveToNearly>	action_move_to_;
	GameObjBasePtrT			dst_obj_;
	bool					clear_mark_pos_;
};
//////////////////////////////////////////////////////////////////////////
class GA_Targetable : public CA_Action{
public:
	virtual void SetDstObj(const GameObjBasePtrT& dst_obj);

protected:
	GameObjBasePtrT		dst_obj_;
	GameObjMgrPtr		obj_mgr_;
};

class GA_KillBase : public GA_Targetable{
};

class GA_KillMonster : public GA_KillBase{
protected:
	CA_RunRes OnRun() override;

private:
	CA_T<GA_MoveToNearly>		action_move_to_;
};

class GA_DangerKill : public GA_KillBase{
	typedef GA_MoveTo::PosContT PosContT;
	enum enHitRes{
		kHR_Error,
		kHR_Danger,
		kHR_Succeed,
	};

protected:
	CA_RunRes OnRun() override;

private:
	enHitRes HitOnce(const CA_T<GA_Skills>& action_skills);
	bool HandleDanger();

private:
	CA_T<GA_TesterDanger>	tester_danger_;
	CA_ActionPtr			as_timer_action_;
	CA_T<GA_SaferMoveTo>	safer_move_to_;

	static const pt_dword	kTestDangerTime = 500;
	static const int		kDangerHandleCnt = 1;
};

class GA_KillMonsters : public CA_Action{
public:
	GA_KillMonsters(const GameObjMgrPtr& obj_mgr);
	~GA_KillMonsters();
	CA_ActionPtr Tester() override;

protected:
	CA_RunRes OnRun() override;

private:
	GameObjMgrPtr				obj_mgr_;
	CA_T<GA_KillBase>			action_kill_monster_;
	CA_T<GA_TesterNearest>		action_tester_nearest_;
	CA_ActionPtr				as_timer_action_;
	GFilterGameObjPtr			filter_hittable_;
	GFilterGameObjPtr			filter_alive_;
	GFilterGameObjPtr			filter_black_name_list_;
	NameListIdT					black_name_list_;
	CA_ActionPtr				other_tester_;
};

class GA_KillUntil : public CA_Action{
public:
	GA_KillUntil(bool clear_mark_pos, const CA_ActionPtr& fast_tester);

protected:
	CA_RunRes OnRun() override;

private:
	CA_ActionPtr		fast_tester_;
	CA_ActionPtr		as_timer_action_;
	bool				clear_mark_pos_;
};
//////////////////////////////////////////////////////////////////////////
//******************技能相关
class GA_SkillBase : public GA_Targetable{
public:
	virtual const std::string& GetSkillName() const = 0;
	virtual float GetDangerDistance() const = 0;
	virtual float GetSkillDistance() const = 0;
	virtual bool CanFire(const GSkillMgr& skill_mgr, const GBuffMgr& buff_mgr, float cur_hp_rate, float cur_mp_rate) const = 0;
	//释放间隔
	virtual CA_ActionPtr SetTimeSpan(float seconds) = 0;
	//技能CD
	virtual CA_ActionPtr SetCD(float seconds) = 0;
	//血值区间
	virtual CA_ActionPtr SetHp(float hp_rate_min, float hp_rate_max) = 0;
	//蓝值区间
	virtual CA_ActionPtr SetMp(float mp_rate_min, float mp_rate_max) = 0;
	//施法距离
	virtual CA_ActionPtr SetSkillDistance(float danger_screen_rate, float skill_screen_rate) = 0;
	//优先级
	virtual CA_ActionPtr SetPriority(int priority) = 0;
	//边逃边杀
	virtual CA_ActionPtr SetEnsureKill() = 0;
	//直接施放
	virtual CA_ActionPtr SetUseDirect() = 0;
	//增加前置技能
	virtual CA_ActionPtr AddPreSkill(const std::string& skill_name) = 0;
	virtual CA_ActionPtr AddPreSkills(const luabind::object& skill_names) = 0;
	//增加后置技能
	virtual CA_ActionPtr AddSuffixSkill(const std::string& skill_name) = 0;
	virtual CA_ActionPtr AddSuffixSkills(const luabind::object& skill_names) = 0;

public:
	//清空技能
	static void ClearSkills();
	//添加技能
	static void AddSkill(GA_SkillBase& skill);
	static const CA_T<GA_Skills>& GetSkills(){ return skills_; }

private:
	static CA_T<GA_Skills>	skills_;
};

class GA_SkillDecorator : public GA_SkillBase{
public:
	GA_SkillDecorator(const CA_T<GA_SkillBase>& decoration);
	const std::string& GetSkillName() const override;
	float GetDangerDistance() const override;
	float GetSkillDistance() const override;
	int Weight() const override;
	bool CanFire(const GSkillMgr& skill_mgr, const GBuffMgr& buff_mgr, float cur_hp_rate, float cur_mp_rate) const override;
	CA_ActionPtr SetTimeSpan(float seconds) override;
	CA_ActionPtr SetCD(float seconds) override;
	CA_ActionPtr SetHp(float hp_rate_min, float hp_rate_max) override;
	CA_ActionPtr SetMp(float mp_rate_min, float mp_rate_max) override;
	CA_ActionPtr SetSkillDistance(float danger_screen_rate, float skill_screen_rate) override;
	CA_ActionPtr SetPriority(int priority) override;
	CA_ActionPtr SetEnsureKill() override;
	CA_ActionPtr SetUseDirect() override;
	CA_ActionPtr AddPreSkill(const std::string& skill_name) override;
	CA_ActionPtr AddPreSkills(const luabind::object& skill_names) override;
	CA_ActionPtr AddSuffixSkill(const std::string& skill_name) override;
	CA_ActionPtr AddSuffixSkills(const luabind::object& skill_names) override;

protected:
	CA_RunRes OnRun() override;

private:
	CA_T<GA_SkillBase>		decoration_;
};

class GA_Skill : public GA_SkillBase{
	struct stSkillInfo{
		CA_WeakT<GA_SkillBase>	skill_;
		std::string				skill_name_;
	};
	typedef std::vector<stSkillInfo> SkillInfoCont;

public:
	GA_Skill(const std::string& skill_name);
	const std::string& GetSkillName() const override{ return skill_name_; }
	float GetDangerDistance() const override{ return danger_distance_; }
	float GetSkillDistance() const override{ return skill_distance_; }
	int Weight() const override;
	bool CanFire(const GSkillMgr& skill_mgr, const GBuffMgr& buff_mgr, float cur_hp_rate, float cur_mp_rate) const override;
	CA_ActionPtr SetTimeSpan(float seconds) override;
	CA_ActionPtr SetCD(float seconds) override;
	CA_ActionPtr SetHp(float hp_rate_min, float hp_rate_max) override;
	CA_ActionPtr SetMp(float mp_rate_min, float mp_rate_max) override;
	CA_ActionPtr SetSkillDistance(float danger_screen_rate, float skill_screen_rate) override;
	CA_ActionPtr SetPriority(int priority) override;
	CA_ActionPtr SetEnsureKill() override;
	CA_ActionPtr SetUseDirect() override;
	CA_ActionPtr AddPreSkill(const std::string& skill_name) override;
	CA_ActionPtr AddPreSkills(const luabind::object& skill_names) override;
	CA_ActionPtr AddSuffixSkill(const std::string& skill_name) override;
	CA_ActionPtr AddSuffixSkills(const luabind::object& skill_names) override;

protected:
	CA_RunRes OnRun() override;
	virtual CA_RunRes DoUseSkill() = 0;
	virtual bool DoUseSkillDirect() = 0;
	CA_RunRes RunTheSkills(SkillInfoCont& skills);

private:
	CA_RunRes UseSkillDirect();
	CA_RunRes PreMove();

protected:
	SkillInfoCont			pre_skills_;
	SkillInfoCont			suffix_skills_;
	std::string				skill_name_;
	TimeDuration			time_span_;
	TimeDuration			cd_;
	float					hp_rate_min_;
	float					hp_rate_max_;
	float					mp_rate_min_;
	float					mp_rate_max_;
	float					danger_distance_;
	float					skill_distance_;
	int						priority_;
	bool					ensure_kill_;
	bool					use_direct_;

private:
	CA_T<GA_DangerMoveTo>	action_move_to_;
};

class GA_SkillToHittable : public GA_Skill{
public:
	GA_SkillToHittable(const std::string& skill_name);

protected:
	CA_RunRes OnRun() override;
	CA_RunRes DoUseSkill() override;
	bool DoUseSkillDirect() override;
	CA_RunRes BeginUseSkill();
	CA_RunRes SkillUsing();
	bool SkillUsingImpl();
};

//对着尸体施放
class GA_SkillToCorpse : public GA_Skill{
public:
	GA_SkillToCorpse(const std::string& skill_name);
	void SetDstObj(const GameObjBasePtrT& dst_obj) override;

protected:
	CA_RunRes OnRun() override;
	CA_RunRes DoUseSkill() override;
	bool DoUseSkillDirect() override;
};

class GA_Skills : public GA_Targetable{
	typedef std::vector<CA_T<GA_SkillBase> > SkillsCont;

public:
	const CA_T<GA_SkillBase>& GetCurSkill() const;
	void SetCurSkill(const CA_T<GA_SkillBase>& cur_skill);
	void AddSkill(const CA_T<GA_SkillBase>& skill);
	void clear() override;
	static bool CalcHpMpInfo(float& cur_hp_rate, float& cur_mp_rate);
	bool GenCurSkill();
	CA_T<GA_SkillBase> FindSkill(const std::string& skill_name) const;

protected:
	CA_RunRes OnRun() override;

private:
	SkillsCont				skills_;
	CA_T<GA_SkillBase>		cur_skill_;
};
//////////////////////////////////////////////////////////////////////////
class GA_Factory : public CA_Factory, public Singleton<GA_Factory, Singleton_MakeMe>{
public:
	CA_T<CA_AsTimerAction> MakeAsTimerAction(const CA_ActionPtr& decoration, const CA_ActionPtr& timer_action);
	CA_T<GA_MoveTo> MakeMoveTo(const stCD_VecInt& dst_pos);
	CA_T<GA_MoveToNearly> MakeMoveToNearly(const stCD_VecInt& dst_pos);
	CA_T<GA_DangerMoveTo> MakeDangerMoveTo(const stCD_VecInt& dst_pos, const GameObjMgrPtr& obj_mgr);
	CA_T<GA_SaferMoveTo> MakeSaferMoveTo(const GameObjMgrPtr& obj_mgr);
	CA_ActionPtr MakeTraFullMap(const GA_SmartConsumerT& consumer, const CA_ActionPtr& action_tra_do, bool clear_mark_pos);
	CA_T<GA_MoveToObjByObj> MakeMoveToObj(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const GameObjBasePtrT& obj);
	CA_T<GA_MoveToObjByName> MakeMoveToObj(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const std::string& obj_name, const GameObjMgrPtr& obj_mgr = nullptr);
	CA_T<GA_KillBase> MakeKillMonster(const GameObjBasePtrT& game_obj);
	CA_ActionPtr MakeKillMonsters(const GameObjMgrPtr& obj_mgr = nullptr);
	CA_ActionPtr MakeKillUntil(const GA_SmartConsumerT& consumer, bool clear_mark_pos, const CA_ActionPtr& fast_tester);
	GA_SmartConsumerT MakeSmartConsumer();
	CA_T<GA_OpenObjs> MakeOpenObjs(const CA_ActionPtr& consumer);
	CA_T<GA_KillInRange> MakeKillInRange(const CA_ActionPtr& consumer, const CA_T<GA_OpenObjs>& open_objs);
	CA_T<GA_KillNpcMonster> MakeKillNpcMonster(const CA_ActionPtr& consumer, const std::string& npc_name);
	CA_T<GA_SkillBase> MakeSkill(const std::string& skill_name);
	CA_T<GA_SkillBase> MakeCorpseSkill(const std::string& skill_name);
	CA_T<GA_Skills> MakeSkills();
	CA_T<GA_TesterAnyObj> MakeTesterAnyObj(const GameObjMgrPtr& obj_mgr);
	CA_T<GA_TesterNearest> MakeTesterNearest(const GameObjBasePtrT& dst_obj);
	CA_T<GA_TesterByName> MakeTesterByName(const std::string& obj_name, const GameObjMgrPtr& obj_mgr = nullptr);
	CA_T<GA_TesterInRange> MakeTesterInRange(const CA_T<GA_OpenObjs>& open_objs);
	CA_ActionPtr MakeTesterLookMonsterDie(const std::string& monster_name);
	CA_T<GA_TesterDanger> MakeTesterDanger(const GameObjMgrPtr& obj_mgr);
};
//////////////////////////////////////////////////////////////////////////
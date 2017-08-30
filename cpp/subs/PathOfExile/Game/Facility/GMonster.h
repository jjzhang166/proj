#pragma once
#include "GameObj.h"

class GMonster : public GameObjBase{
	typedef std::vector<const stCD_TblItemActiveSkills*> ActiveSkills;

public:
	GMonster();
	bool DoOpen() override;
	int GetCurHp() const override;
	bool IsHitable() const override;
	float WeightDistance(const stCD_VecInt& pos) const override;

public:
	void DebugInfo() const override;

private:
	bool UpdateImpl() override;
	bool MustCanTargetable() const override;

private:
	stCD_EntityMonster*		entity_monster_;

private:
	stCD_LifeDataHp			hp_mon_;
	stCD_LifeData			mp_mon_;
	ActiveSkills			active_skills_;
};

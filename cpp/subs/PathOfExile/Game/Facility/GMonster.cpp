#include "stdafx.h"
#include "GMonster.h"
#include <StdEx/XstddefEx.h>
#include "GInterface.h"

GMonster::GMonster() : GameObjBase(kGOT_Monster)
{
	entity_monster_ = nullptr;
}

bool GMonster::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		//assert(false);
		return false;
	}

	auto entity = GetGameEntityBase();
	if (IsUnValidPtr(entity))
	{
		assert(false);
		return false;
	}

	if (!entity->IsValid())
	{
		assert(false);
		return false;
	}
	entity_monster_ = entity->CastTo<stCD_EntityMonster>();
	if (!entity_monster_)
	{
		assert(false);
		return false;
	}
	if (!entity_monster_->CanHit())
		return false;

	auto game_mon = entity_monster_->GetRender();
	if (IsUnValidPtr(game_mon))
	{
		assert(false);
		return false;
	}

	if (!CD_SafeName(game_mon->name_))
	{
		assert(false);
		return false;
	}

	SetName(game_mon->name_.GetStdString());

	auto monpos = entity_monster_->GetPositioned();
	if (IsUnValidPtr(monpos))
	{
		assert(false);
		return false;
	}
	if (GetCnName() == kObjName_JinGuShiXiang || 
		GetCnName() == kMonName_parasite ||
		GetCnName() == kMonName_LaSaEgg ||
		GetCnName() == kMonName_HateFlag
		)
		return false;
	auto monlife = entity_monster_->GetLife();
	if (IsUnValidPtr(monlife))
	{
		assert(false);
		return false;
	}
	hp_mon_ = monlife->hp_;
	mp_mon_ = monlife->mp_;
	if (hp_mon_.max_value_ <= 0 ||
		mp_mon_.max_value_ <= 0 )
	{
		assert(false);
		return false;
	}

	auto actor = entity_monster_->GetActor();
	if (actor)
	{
		actor->GetMySkillMgr().TraMySkill([this](stCD_SkillInfo* skill_info){
			auto active_skill = skill_info->GetActiveSkillDb();
			if (!active_skill)
				return true;
			active_skills_.push_back(active_skill);
			return true;
		});
		assert(active_skills_.size() < 50);
	}
	return true;
}

void GMonster::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << "hp:" << hp_mon_ << ",mp:" << mp_mon_;
#ifdef _DEBUG
	entity_monster_->DebugInfo();
#endif
}

int GMonster::GetCurHp() const
{
	return hp_mon_.cur_value_;
}

bool GMonster::IsHitable() const
{
	return true;
}

float GMonster::WeightDistance(const stCD_VecInt& pos) const
{
	return __super::WeightDistance(pos);
	auto my_pos = GetPos();
	if (!my_pos)
		return __super::WeightDistance(pos);
	auto real_distance = __super::WeightDistance(pos);
	if (active_skills_.empty())
		return real_distance;
	struct stHelper{
		typedef std::set<pt_cwsz, StdEx::LessChar<wchar_t> > SkillNameCont;

		stHelper(){
			skill_names_.insert(L"necromancer_revive_skeleton");		//恶水池的 腐化者.凯达弗斯，召唤死亡生物的技能，没有中文名

			it_end_ = skill_names_.end();
		}

		SkillNameCont			skill_names_;
		SkillNameCont::iterator	it_end_;
	};
	static stHelper helper;
	int rate = 0;
	for (auto active_skill : active_skills_)
	{
		++rate;
		if (!active_skill->en_name_)
			continue;
		auto it = helper.skill_names_.find(active_skill->en_name_);
		if (it != helper.it_end_)
			++rate;
	}
	return real_distance - rate * 18;
}

bool GMonster::DoOpen()
{
	if (GetOpenSkillId() == kSI_OpenObj)
	{
		assert(false);
		return false;
	}
	if (hp_mon_.cur_value_ > 0)
	{
		assert(false);
		return false;
	}
	return __super::DoOpen();
}

bool GMonster::MustCanTargetable() const
{
	auto entity = GetGameEntityBase();
	assert(entity);
	auto render = entity->GetRender();
	if (IsUnValidPtr(render))
	{
		assert(false);
		return false;
	}
	if (render->name_ == kMonName_WaErChaoLing ||
		//render->name_ == kMonName_DominusAscendant ||
		render->name_ == kMonName_DominusHigh ||
		render->name_ == kMonName_Weaver)
		return true;
	return false;
}

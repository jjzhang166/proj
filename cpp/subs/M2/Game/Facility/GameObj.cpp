#include "stdafx.h"
#include "GameObj.h"
#include <Common/MemOper.h>
#include "GItem.h"

GameObjBase::GameObjBase()
{
	id_			= 0;
	game_entity_	= nullptr;
	pos_		= nullptr;
	obj_type_	= kGOT_Invalid;
	open_skill_id_ = kSI_OpenObj;
}

GameObjBase::GameObjBase(GameObjType objtype) : GameObjBase()
{
	obj_type_ = objtype;
}

void GameObjBase::SetName(const std::string& name)
{
	ch_name_ = name;
}

bool GameObjBase::SetGameEntity(stCD_EntityPosBased* entity)
{
	assert(entity);
	if (!entity->IsValid())
	{
		assert(false);
		return false;
	}
	game_entity_ = entity;
	return true;
}

bool GameObjBase::UpdateImpl()
{
	if (!game_entity_)
	{
		assert(false);
		return false;
	}
	
	id_ = game_entity_->id_;
	assert(id_ > 0);
	if (!game_entity_)
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	auto gamepos_ = game_entity_->GetPositioned();
	if (IsUnValidPtr(gamepos_))
	{
		assert(false);
		return false;
	}
	pos_ = &gamepos_->pos_;
	if (pos_->x <= 0 || pos_->y <= 0)
	{
		assert(false);
		return false;
	}
	pos_backup_ = *pos_;
	return true;
}

void GameObjBase::DebugInfo() const
{
	assert(pos_);
	LOG_O(Log_debug) << "game_entity_Addr:" << game_entity_ 
		<< ",ch_name:" << ch_name_ << ", obj_type_ : " << obj_type_ << ", id_ : " << id_ << ", pos : " << *pos_;
}

int GameObjBase::GetCurHp() const
{
	return 0;
}
int GameObjBase::GetCurMp() const
{
	return 0;
}

bool GameObjBase::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	assert(!"子类未重载该函数吗111");
	return false;
}

bool GameObjBase::IsHitable() const
{
	return false;
}

bool GameObjBase::NeedVisitable() const
{
	return false;
}

int GameObjBase::GetCanOpenedDistance() const
{
	assert(false);
	return -1;
}

bool GameObjBase::CloseObject() const
{
	return true;
}

bool GameObjBase::MovingDoTryOpen() const
{
	for (auto& i : kNoAutoOpenObj)
	{
		if (i == ch_name_)
			return false;
	}
	return true;
}

float GameObjBase::WeightDistance(const stCD_VecInt& pos) const
{
	auto my_pos = GetPos();
	if (!my_pos)
	{
		assert(false);
		return 10000000;
	}
	return my_pos->Distance(pos);
	//不能加这个，否则效率会非常低
	/*auto& astar = G_AStar::GetMe();
	auto my_area_id = astar.GetAreaId(*my_pos);
	auto other_area_id = astar.GetAreaId(pos);
	return my_pos->Distance(pos) + 2000 * (my_area_id == other_area_id ? 0 : 1);*/
}

float GameObjBase::Distance(const stCD_VecInt& pos) const
{
	auto my_pos = GetPos();
	if (!my_pos)
	{
		assert(false);
		return 10000000;
	}
	return my_pos->Distance(pos);
}

float GameObjBase::Distance(const GameObjBase& rhs) const
{
	auto my_pos = GetPos();
	auto rhs_pos = rhs.GetPos();
	if (!my_pos || !rhs_pos)
	{
		assert(false);
		return 10000000;
	}
	return my_pos->Distance(*rhs_pos);
}

void GameObjBase::ResetPos()
{
	pos_ = nullptr;
}

bool GameObjBase::IsTransitonable() const
{
	return false;
}

bool GameObjBase::IsTownTranstionable() const
{
	return false;
}

bool GameObjBase::DoFilt() const
{
	return false;
}

void GameObjBase::SetOwner(const GameObjMgrPtr& obj_mgr)
{
	owner_ = obj_mgr;
}

GameObjMgrPtr GameObjBase::GetOwner() const
{
	if (owner_.expired())
		return nullptr;
	return owner_.lock();
}

bool GameObjBase::CanBreakOpenObj() const
{
	return true;
}

void GameObjBase::SetOpenSkillId(enCD_SkillId skill_id)
{
	open_skill_id_ = skill_id;
}

bool GameObjBase::DoOpen()
{
	DateTimeLocal time_now;
	auto& time_duration = GInterface::GetSendSkillTimeDuration();
	auto& last_time = time_duration.GetLastTime();
	assert(time_now >= last_time);
	auto duration = time_duration.GetDuration() + 200;
	auto total_duation = (time_now - last_time).totalMilliseconds();
	if (total_duation < duration)
	{
		duration -= total_duation;
		DummySleep(duration);
	}
	time_duration.UpdateTime();
	if (!GInterface::OpenObj(open_skill_id_, id_))
	{
		assert(false);
		return false;
	}
	stCDS_UseSkillEnd msg_end;
	if (!GInterface::Send(msg_end))
	{
		assert(false);
		return false;
	}
	return true;
}

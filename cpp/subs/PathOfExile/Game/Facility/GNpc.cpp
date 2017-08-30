#include "stdafx.h"
#include "GNpc.h"
#include "GClass.h"
#include "GPlayer.h"

GNpc::GNpc() : GameObjBase(kGOT_Npc)
{
	entity_npc_ = nullptr;
	is_npc_need_visit_ = false;
}


bool GNpc::UpdateImpl()
{
	if (!__super::UpdateImpl())
		return false;

	auto entity = GetGameEntityBase();
	if (!entity)
	{
		assert(false);
		return false;
	}
	if (!entity->IsValid())
	{
		assert(false);
		return false;
	}

	entity_npc_ = entity->CastTo<stCD_EntityNpc>();
	if (!entity_npc_)
	{
		assert(false);
		return false;
	}

	auto game_npc = entity_npc_->GetRender();
	if (IsUnValidPtr(game_npc))
	{
		assert(false);
		return false;
	}
	if (!CD_SafeName(game_npc->name_))
	{
		assert(false);
		return false;
	}

	SetName(game_npc->name_.GetStdString());

	auto npc_attr = entity->GetClientNpc();
	if (npc_attr)
		is_npc_need_visit_ = npc_attr->NeedVisit();
	return true;
}

void GNpc::DebugInfo() const
{
	LOG_O(Log_debug) << "------------NPC--------------" << "vist_state:" << is_npc_need_visit_;
	__super::DebugInfo();

}

bool GNpc::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	return visitor.VisitNpc(*this, is_pre_open, open_res);
}

bool GNpc::NeedVisitable() const
{
	return is_npc_need_visit_;
}

bool GNpc::CloseObject() const
{
	return GPlayerMe::GetMe().CloseNpcPanel();
}


GMovement::GMovement() : GameObjBase(kGOT_Movement)
{
	is_opened_ = false;
}

bool GMovement::UpdateImpl()
{
	if (!__super::UpdateImpl())
		return false;
	auto entity = GetGameEntityBase();
	assert(entity);
	auto attr_trans = entity->GetTransitionable();
	if (!attr_trans)
	{
		//assert(false);
		return false;
	}
	//BeaconPayload对象若是不见了，则attr_trans->IsOpened==true.
	is_opened_ = attr_trans->IsOpened();
	return true;
}

bool GMovement::IsOpened() const
{
	return is_opened_;
}

#include "stdafx.h"
#include "GameObjMgr.h"
#include <Common/LogOut.h>
#include "GNpc.h"
#include "GPlayer.h"
#include <Common/UsefulFunc.h>
#include "GConst.h"
#include "GMonster.h"
#include "GMap.h"
#include "GClass.h"
#include "GItem.h"

GameObjMgrBase::~GameObjMgrBase()
{

}

GameObjBase* GameObjMgrBase::CreateGameObj(enGameObjType type) const
{
	switch (type)
	{
	case kGOT_Invalid:
		//assert(false);
		break;
	case kGOT_Player:
		return new GPlayer;
		break;
	case kGOT_Npc:
		return new GNpc;
		break;
	case kGOT_Monster:
		return new GMonster;
		break;
	case kGOT_AreaTransition:
		return new GAreaTransition;
		break;
	case kGOT_Waypoint:
		return new GWayPoint;
		break;
	case kGOT_Stash:
		return new GStash;
		break;
	case kGOT_NoticeBoard:
		return new GNoHuman(kGOT_NoticeBoard);
		break;
	case kGOT_NoticeBoardPvP:
		return new GNoHuman(kGOT_NoticeBoardPvP);
		break;
	case  kGOT_WorldItem:
		return new GWorldItem(kGOT_WorldItem);
		break;
	case  kGOT_Chest:
		return new GChest(kGOT_Chest);
		break;
	case kGOT_Transitonable:
		return new GTranstionable(kGOT_Transitonable);
	case  kGOT_Movement:
		return new GMovement;
	default:
		break;
	}
	return nullptr;
}

bool GameObjMgrBase::RebuildAllImpl()
{
	objs_.clear();
	auto w = CD_Interface::GetWorld();
	if (IsUnValidPtr(w))
	{
		assert(false);
		return false;
	}
	auto& buff_mgr = GBuffMgr::GetMe();
	if (!buff_mgr.Update())
	{
		assert(false);
		return false;
	}
	w->obj_mgr_.TraEntityObj([this, &buff_mgr](stCD_EntityPosBased* entity){
		return CreateObjsAtTra(buff_mgr, entity);
	}, true);
	return true;
}

GameObjTypeThinCont GameObjMgrBase::CalcObjNodeType(stCD_EntityPosBased* entity) const
{
	//已经判断过了。
	/*if (!entity->IsValid())
	{
		assert(false);
		return retType;
	}*/
	struct stHelp{
		enGameObjType							type_;
		stCD_EntityPosBased::Fn_EntityIf	fn_;
		GameObjTypeThinCont					allowed_types_;
	};
	static const stHelp game_obj_types[] = {
		{ kGOT_Monster, &stCD_EntityPosBased::IsMonster, { kGOT_Npc } },
		{ kGOT_Npc, &stCD_EntityPosBased::IsNpc, {} },
		{ kGOT_Chest, &stCD_EntityPosBased::IsChest, {} },
		{ kGOT_WorldItem, &stCD_EntityPosBased::IsWorldItem, {} },
		{ kGOT_Waypoint, &stCD_EntityPosBased::IsWaypoint, {} },
		{ kGOT_AreaTransition, &stCD_EntityPosBased::IsAreaTransition, {} },
		{ kGOT_Stash, &stCD_EntityPosBased::IsStash, {} },
		//{ kGOT_Player, &stCD_EntityPosBased::IsPlayer, {} },
		{ kGOT_NoticeBoard, &stCD_EntityPosBased::IsNoticeBoard, {} },
		{ kGOT_NoticeBoardPvP, &stCD_EntityPosBased::IsNoticeBoardPvP, {} },
		{ kGOT_Movement, &stCD_EntityPosBased::IsMovement, {} },
		{ kGOT_Transitonable, &stCD_EntityPosBased::IsTransitionable, {} },		//得放到最后面，因为很多都有这个属性
	};
	GameObjTypeThinCont types;
	for (auto& v : game_obj_types)
	{
		if (!(entity->*(v.fn_))())
			continue;
		types.push_back(v.type_);
		for (auto allowed_type : v.allowed_types_)
		{
			for (auto& the_t : game_obj_types)
			{
				if (the_t.type_ != allowed_type)
					continue;
				if (!(entity->*(the_t.fn_))())
					continue;
				types.push_back(the_t.type_);
				if (types.is_full())
					return types;
			}
		}
		break;
	}
	return types;
}

void GameObjMgrBase::DebugInfo() const
{
	for (auto& v : objs_)
	{
		v->DebugInfo();
	}
	LOG_O(Log_debug) << "对象总数：" << objs_.size();
}

GameObjBasePtrT GameObjMgrBase::FindById(pt_dword id) const
{
	for (auto& v : objs_)
	{
		if (v->GetGameId() == id)
			return v;
	}
	return nullptr;
}

GameObjBasePtrT GameObjMgrBase::FindByName(const std::string& name) const
{
	if (name.empty())
	{
		assert(false);
		return nullptr;
	}
	for (auto& v : objs_)
	{
		if (v->GetCnName() == name)
			return v;
	}
	return nullptr;
}

int GameObjMgrBase::GetObjCntByName(const std::string& name) const
{
	if (name.empty())
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (auto& v : objs_)
	{
		if (v->GetCnName() == name)
			++cnt;
	}
	return cnt;
}

bool GameObjMgrBase::FilterObj(const GameObjBasePtrT& obj) const
{
	return GetFilter().DoFilt(obj);
}

const GameObjMgrBase::ObjContT& GameObjMgrBase::GetObjs() const
{
	return objs_;
}

void GameObjMgrBase::SortByPos(const stCD_VecInt& pos)
{
	return GameObjBase::SortAssist<false>(objs_, pos);
}

GameObjBasePtrT GameObjMgrBase::GetFirstObj() const
{
	if (objs_.empty())
		return nullptr;
	return objs_.front();
}

bool GameObjMgrBase::empty() const
{
	return objs_.empty();
}

GameObjTypeThinCont GameObjMgrBase::FilterGameObjType(const GameObjTypeThinCont& types) const
{
	return GameObjTypeThinCont();
}

GameObjMgrBase& GameObjMgrBase::operator+=(const GameObjMgrBase& rhs)
{
	for (auto& v : rhs.objs_)
	{
		if (v->IsTownTranstionable())
			continue;
		assert(v);
		for (auto& self : objs_)
		{
			if (v->GetGameId() == self->GetGameId() && v->GetCnName() == self->GetCnName())
			{
				//更新
				self = v;
				goto LABEL_COTINUE;
			}
		}
		objs_.push_back(v);
	LABEL_COTINUE:
		continue;
	}
	return *this;
}

void GameObjMgrBase::clear()
{
	objs_.clear();
}

GameObjBasePtrT GameObjMgrBase::FindObjByFilter(const IFilterGameObj& filter) const
{
	for (auto& v : objs_)
	{
		if (filter.DoFilt(v))
			return v;
	}
	return nullptr;
}

GameObjBasePtrT GameObjMgrBase::FindObjInCircle(const stCD_VecInt& center, int radius) const
{
	const auto& form = MakeCircleStdForm(center, radius);
	for (auto& v : objs_)
	{
		auto the_pos = v->GetPos();
		if (!the_pos)
		{
			assert(false);
			return nullptr;
		}
		auto the_pos_tmp = *the_pos;
		if (form.GetPointPos(the_pos_tmp) <= 0)
			return v;
	}
	return nullptr;
}

GameObjBasePtrT GameObjMgrBase::FindObjInCircleByType(enGameObjType obj_type,const stCD_VecInt& center, int radius) const
{
	const auto& form = MakeCircleStdForm(center, radius);
	for (auto& v : objs_)
	{
		if(v->GetObjType() != obj_type)
			continue;
		auto the_pos = v->GetPos();
		if (!the_pos)
		{
			assert(false);
			return nullptr;
		}
		auto the_pos_tmp = *the_pos;
		if (form.GetPointPos(the_pos_tmp) <= 0)
			return v;
	}
	return nullptr;
}

bool GameObjMgrBase::HasObjInCircle(const stCD_VecInt& center, int radius, int count) const
{
	const auto& form = MakeCircleStdForm(center, radius);
	int cnt = 0;
	for (auto& v : objs_)
	{
		auto the_pos = v->GetPos();
		if (!the_pos)
		{
			assert(false);
			return false;
		}
		auto the_pos_tmp = *the_pos;
		if (form.GetPointPos(the_pos_tmp) <= 0)
		{
			++cnt;
			if (cnt >= count)
				return true;
		}
	}
	return false;
}

void GameObjMgrBase::SortByWeightDistance(const stCD_VecInt& pos)
{
	return GameObjBase::SortAssist<true>(objs_, pos);
}

void GameObjMgrBase::ResetAll()
{
	for (auto& v : objs_)
	{
		v->ResetPos();
		v->SetOwner(nullptr);
	}
}

bool GameObjMgrBase::CreateObjsAtTra(GBuffMgr& buff_mgr, stCD_EntityPosBased* entity)
{
	auto obj_types = CalcObjNodeType(entity);
	if (obj_types.empty())
		return true;
	obj_types = FilterGameObjType(obj_types);
	if (obj_types.empty())
		return true;
	//是召唤物
	if (buff_mgr.ExistEntityId(entity->id_))
		return true;
	for (auto t : obj_types)
	{
		GameObjBasePtrT obj_tmp(CreateGameObj(t));
		if (!obj_tmp)
			continue;
		if (!obj_tmp->SetGameEntity(entity))
		{
			assert(false);
			return false;
		}
		if (!obj_tmp->UpdateImpl())
			continue;
		if (!FilterObj(obj_tmp))
			continue;
		assert(obj_tmp);
		obj_tmp->SetOwner(shared_from_this());
		objs_.push_back(obj_tmp);
	}	
	return true;
}

void GameObjMgrBase::GetInCircleObjs(const stCD_VecInt& center, int radius, int count, GameObjsBufferCont& out_objs)
{
	const auto& form = MakeCircleStdForm(center, radius);
	int cnt = 0;
	SortByPos(center);
	for (auto& v : objs_)
	{
		auto the_pos = v->GetPos();
		if (!the_pos)
		{
			assert(false);
			break;
		}
		auto the_pos_tmp = *the_pos;
		if (form.GetPointPos(the_pos_tmp) > 0)
			break;
		out_objs.push_back(v);
		++cnt;
		if (cnt >= count)
			break;
	}
}

void GameObjMgrBase::EraseRemObj(const GameObjBase& obj)
{
	if (objs_.empty())
		return;
	auto it = std::find_if(objs_.begin(), objs_.end(), [&obj](const GameObjBasePtrT& v){ return v->GetGameId() == obj.GetGameId() && obj.GetObjType() == v->GetObjType(); });
	if (it != objs_.end())
		objs_.erase(it);
}

GameObjMgr::GameObjMgr(const GameObjMgrBase& rhs) : GameObjMgrBase(rhs)
{
}

GameObjTypeThinCont GameObjMgr::FilterGameObjType(const GameObjTypeThinCont& types) const
{
	GameObjTypeThinCont res;
	for (auto& t : types)
	{
		if (filter_type_.DoFilt(t))
			res.push_back(t);
	}
	return res;
}

void GameObjMgr::IncludeNoHumanFilter()
{
	AddFilterType(kGOT_Waypoint, kGOT_AreaTransition, 
		kGOT_Stash, kGOT_NoticeBoard, kGOT_NoticeBoardPvP, kGOT_Movement);
}

void GameObjMgr::IncludeOpenableFilter()
{
	AddFilterType(kGOT_Npc, kGOT_Waypoint, kGOT_AreaTransition, 
		kGOT_Stash, kGOT_NoticeBoard, kGOT_NoticeBoardPvP,
		kGOT_Chest, kGOT_WorldItem, kGOT_Transitonable);
}

void GameObjMgr::IncludeLifedFilter()
{
	AddFilterType(kGOT_Npc, kGOT_Monster);
}

void GameObjMgr::ClearFilterType()
{
	filters_.clear();
}

FilterGameObjCont& GameObjMgr::GetFilter() const
{
	return filters_;
}

FilterGameObjCont& GameObjMgrCont::GetFilter() const
{
	assert(false);
	static FilterGameObjCont cont;
	return cont;
}

GameObjMgrCont::GameObjMgrCont(const GameObjMgrBase& rhs) : GameObjMgrBase(rhs)
{

}

GameObjMgrCont& GameObjMgrCont::operator=(const GameObjMgrBase& rhs)
{
	__super::operator = (rhs);
	return *this;
}

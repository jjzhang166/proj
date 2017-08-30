#include "stdafx.h"
#include "GObjFilter.h"
#include "GameObj.h"

bool FilterGameObjName::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->GetCnName() == name_;
}

FilterGameObjName::FilterGameObjName(FilterGameObjCont& filter_cont, const std::string& name)
	: FilterGameObjEnsure(filter_cont), name_(name)
{

}

bool FilterGameObjAlive::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->GetCurHp() > 0;
}

FilterGameObjAlive::FilterGameObjAlive(FilterGameObjCont& filter_cont)
	: FilterGameObjEnsure(filter_cont)
{

}

FilterGameObjBlackNameList::FilterGameObjBlackNameList(FilterGameObjCont& filter_cont, const NameListIdT& black_name_list)
	: FilterGameObjEnsure(filter_cont), black_name_list_(black_name_list)
{

}

bool FilterGameObjBlackNameList::DoFilt(const GameObjBasePtrT& obj) const
{
	if (black_name_list_.IsIn(obj->GetGameId()))
		return false;
	return true;
}

FilterGameObjId::FilterGameObjId(FilterGameObjCont& filter_cont, pt_dword obj_id)
	: FilterGameObjEnsure(filter_cont)
{
	obj_id_ = obj_id;
}

bool FilterGameObjId::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->GetGameId() == obj_id_;
}

bool FilterGameObjHittable::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->IsHitable();
}

FilterGameObjHittable::FilterGameObjHittable(FilterGameObjCont& filter_cont)
	: FilterGameObjEnsure(filter_cont)
{

}

bool FilterGameObjNeedVisitable::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->NeedVisitable() && obj->GetCnName() != kObjName_NpcNoVisit;
}

FilterGameObjNeedVisitable::FilterGameObjNeedVisitable(FilterGameObjCont& filter_cont)
	: FilterGameObjEnsure(filter_cont)
{

}

bool FilterRememberObjs::DoFilt(const GameObjBasePtrT& obj) const
{
	for (auto& v : objs_)
	{
		if (v->GetGameId() == obj->GetGameId())
			return false;
	}
	return true;
}

void FilterRememberObjs::Add(const GameObjBasePtrT& obj)
{
	if (!obj)
	{
		assert(false);
		return;
	}
	objs_.push_back(obj);
}

FilterRememberObjs::FilterRememberObjs(FilterGameObjCont& filter_cont)
	: FilterGameObjEnsure(filter_cont)
{

}

FilterGameObjNotTheId::FilterGameObjNotTheId(FilterGameObjCont& filter_cont, pt_dword obj_id)
	: FilterGameObjEnsure(filter_cont)
{
	obj_id_ = obj_id;
}

bool FilterGameObjNotTheId::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->GetGameId() != obj_id_;
}

bool FilterTransferTown::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->IsTownTranstionable();
}

FilterTransferTown::FilterTransferTown(FilterGameObjCont& filter_cont) : FilterGameObjEnsure(filter_cont)
{

}

bool FilterQuestWorldObj::DoFilt(const FittedType& obj) const
{
	return obj->DoFilt();
}

FilterQuestWorldObj::FilterQuestWorldObj(FilterGameObjCont& filter_cont) : FilterGameObjEnsure(filter_cont)
{

}

FilterGameObjNotName::FilterGameObjNotName(FilterGameObjCont& filter_cont, const std::string& name)
	: FilterGameObjEnsure(filter_cont), name_(name)
{

}

bool FilterGameObjNotName::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->GetCnName() != name_;
}

FilterGameObjDied::FilterGameObjDied(FilterGameObjCont& filter_cont) : FilterGameObjEnsure(filter_cont)
{

}

bool FilterGameObjDied::DoFilt(const GameObjBasePtrT& obj) const
{
	return obj->GetCurHp() == 0;
}

FilterObjByType::FilterObjByType(enGameObjType obj_type)
{
	obj_type_ = obj_type;
}

bool FilterObjByType::DoFilt(const FittedType& obj) const
{
	return obj->GetObjType() == obj_type_;
}

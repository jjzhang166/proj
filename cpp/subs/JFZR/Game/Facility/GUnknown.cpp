#include "stdafx.h"
#include "GUnknown.h"
#include "GOffset.h"
#include <Common/MemOper.h>
#include "GConst.h"
#include "DataStructAdapter.h"
#include "GItem.h"

GUnknown::GUnknown() : GameObjBase(kGOT_Unknown)
{
	item_template_ = nullptr;
}

bool GUnknown::Update()
{
	using namespace GClass;
	using namespace GOffset;
	using namespace GFuncCall;
	if ( !__super::Update() )
	{
		assert(false);
		return false;
	}
	const stSdbItemTemplate* game_item_template = nullptr;
	if ( !MemOperApplyR(game_item_template, (int)game_obj_addr_, g_o_DroppedItem) )
	{
		assert(false);
		return false;
	}
	item_template_ = nullptr;
	if ( !game_item_template )
		return true;
	item_template_ = GItemTemplateMgr::GetMe().FindByHashId(CalcHash_DWORD(game_item_template->str_theId.game_str_.GetOriginBegin()));
	if ( !item_template_ )
	{
		assert(false);
		return false;
	}
	return true;
}

bool GUnknown::IsGold() const
{
	if ( !item_template_ )
		return false;
	return kDroppedItemType_Gold == item_template_->item_id_;
}


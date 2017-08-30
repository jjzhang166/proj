#include "stdafx.h"
#include "CD_StructBase.h"
#include <cassert>
#include "CD_UsefulFns.h"
#include <CommonX/CmnX_StaticConts.h>
#include "CD_Structs.h"
#include <StdEx/XstddefEx.h>


void stCD_Data_GameObject::DebugInfo() const
{
	LOG_O(Log_debug) << "Data_GameObjectAddr:" << this << ",ClassName:" << CD_TypeName(this) 
		<< ",friendly:" << friendly_
		<< ",type:" << type_;
}

void stCD_Data_Item::DebugInfo() const
{
	LOG_O(Log_debug) << "Data_ItemAddr:" << this << ",ClassName:" << CD_TypeName(this) 
		<< ",Gender:" << gender_
		<< ",ItemName:" << CD_SafeName(GetItemName());
}

pt_cwsz stCD_Data_Item::GetItemName() const
{
	auto xml = stCD_XmlString::GetInstance();
	if (!xml)
	{
		assert(false);
		return nullptr;
	}
	return xml->FindStr(kXSIT_Item, item_name_xmlid_);
}

stCD_XmlString* stCD_XmlString::GetInstance()
{
	auto res = *(stCD_XmlString**)g_a_xml_string_;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

void stCD_Data_Quest::DebugInfo() const
{
	LOG_O(Log_debug) << "Data_QuestAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",QuestName:" << CD_SafeName(GetQuestName())
		<< ",QuestType:" << quest_type_ << ",QuestId:" << quest_id_
		<< ",QuestLvl:" << quest_lvl_ << ",ChapterId:" << chapter_id_
		<< ",CanGiveUp:" << can_give_up_;
	auto chapter = GetChapter();
	if (chapter)
		chapter->DebugInfo();
}

stCD_Data_Chapter* stCD_Data_Quest::GetChapter() const
{
	return stCD_Data_Chapter::GetChapter(chapter_id_);
}

void stCD_Data_Chapter::DebugInfo() const
{
	LOG_O(Log_debug) << "Data_ChapterAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",ChapterName:" << CD_SafeName(chapter_name_);
}

#pragma once
/*
@author zhp
@date 2017/2/5 13:11
@purpose for struct base
*/
#include <string>
#include <ProtocolX/P_DefinesX.h>
#include <CmnMix/Cmn_LogOut.h>
#include "CD_Offset.h"
#include "CD_Const.h"
#include <WgFacility/CmnGameStruct.h>
#include <CmnMix/Cmn_UsefulFunc.h>
#include "CD_Updates.h"
#include <CommonX/CmnX_Math.h>
#include <CommonX/CmnX_Allocator.h>
#include <StdEx/XstddefEx.h>
#include "CD_Container.h"

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
typedef stVec3Int stCD_Vec3Int;
typedef stVec3Float stCD_Vec3Float;

typedef GameVector<void*> stCD_UknGameVector;
//////////////////////////////////////////////////////////////////////////
struct stCD_LockCriticalSection{
	pt_pcvoid		vtbl_;
	pt_pcvoid		ukn_ptr_;
	int				ukn_value_;		//=-1
	p_mem_placeholder(4 * 4);
};
static_assert(0x1c == sizeof(stCD_LockCriticalSection), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_Data_GameObject{
	void DebugInfo() const;

	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_o_tbl_item_game_obj_friendly_ - 4);
	pt_dword		friendly_;		//==2 是npc ； ==0是怪物 1宠物
	p_mem_placeholder(g_o_tbl_item_game_obj_type_ - g_o_tbl_item_game_obj_friendly_ - 4);
	pt_dword		type_;			//类型 5是宠物或npc 1是怪物  3boss精英
};

struct stCD_Data_Item{
	void DebugInfo() const;
	pt_cwsz GetItemName() const;

	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_so_tbl_item_item_name_xmlid_ - 4);
	pt_dword		item_name_xmlid_;
	p_mem_placeholder(g_so_tbl_item_item_gender_ - 4 - g_so_tbl_item_item_name_xmlid_);
	pt_dword		gender_;		//性别
};
static_assert(g_so_tbl_item_item_name_xmlid_ == offsetof(stCD_Data_Item, item_name_xmlid_), "");
static_assert(g_so_tbl_item_item_gender_ == offsetof(stCD_Data_Item, gender_), "");

struct stCD_Data_Chapter{
	void DebugInfo() const;
	static stCD_Data_Chapter* GetChapter(pt_dword chapter_id);

	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_so_chapter_name_ - 4);
	pt_cwsz			chapter_name_;
};
static_assert(g_so_chapter_name_ == offsetof(stCD_Data_Chapter, chapter_name_), "");

struct stCD_Data_Quest{
	void DebugInfo() const;
	stCD_Data_Chapter* GetChapter() const;
	pt_cwsz GetQuestName() const;

	pt_pcvoid			vtbl_;
	p_mem_placeholder(g_so_quest_type_ - 4);
	enCD_QuestType		quest_type_;
	p_mem_placeholder(g_so_quest_id_ - g_so_quest_type_ - 4);
	pt_dword			quest_id_;
	pt_dword			quest_lvl_;
	pt_dword			chapter_id_;
	p_mem_placeholder(4);
	pt_dword			can_give_up_;
};
static_assert(g_so_quest_type_ == offsetof(stCD_Data_Quest, quest_type_), "");
static_assert(g_so_quest_id_ == offsetof(stCD_Data_Quest, quest_id_), "");
static_assert(0x38 == offsetof(stCD_Data_Quest, can_give_up_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_XmlString{
	static stCD_XmlString* GetInstance();
	pt_cwsz FindStr(enCD_XmlStrIdType type, pt_dword str_id) const;
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

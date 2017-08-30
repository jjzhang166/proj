#pragma once
/*
@author zhp
@date 2017/1/24 17:22
@purpose for vtbl
ukn->unknown
*/
#include <ProtocolX/P_DefinesX.h>
#include <cstddef>
#include <CmnMix/Cmn_MemOper.h>

struct stCD_UiEventBase;

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
struct stCD_UknBaseVtbl{
	pt_pcvoid	fn_;
};

struct stCD_MySkillMgrVtbl{
	pt_pcvoid	ukn_;
	pt_pcvoid	fn_;
};

struct stCD_UiEventVtbl{
	typedef int(stCD_UiEventBase::*Fn_GetNextOffset)() const;
	typedef void(stCD_UiEventBase::*Fn_HandleLogic)(pt_pcvoid, pt_pcvoid) const;

	pt_pcvoid		fn_get_next_offset_;
	pt_pcvoid		ukn_;
	pt_pcvoid		handle_logic_;
};

struct stCD_QuestInfoVtbl{
	pt_pcvoid		ukn_;
	pt_pcvoid		ukn1_;
	pt_pcvoid		fn_quest_info_tag_;
};

struct stCD_UiObjVtbl{
	pt_pcvoid		ukn_;		//很可能是析构函数

	CMN_CAST_TO
};

struct stCD_Vtbl_UiGridPanel : stCD_UiObjVtbl{
	enum{
		kPress_Normal1 = 0x160,
		kPress_Normal2 = 0x168,
		kPress_Shift = 0x16c,
		kPress_Menu = 0x170,
		kPress_Control = 0x174,
		kPress_PutItemDown = 0x17c,		//去搜kPress_PutItemDown
	};
};

struct stCD_Vtbl_SkillAttrObj{
	p_mem_placeholder(4);		//占位符
	enum{
		kGetValue = 0,
	};
};

//去搜
struct stCD_Vtbl_CharacterCreationInfo : stCD_UiObjVtbl{
	enum{
		kSelectCharacter = 0x3c,
		kGetSelectCharacterProfessionType = 0x40,	//没有参数，返回选择的角色的enCD_ProfessionType
	};
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

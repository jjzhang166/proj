#pragma once
/*
@author zhp
@date 2017/1/24 17:22
*/
#include <ProtocolX/P_DefinesX.h>
#include <cstddef>
#include "CD_Serialize.hpp"

#pragma pack(push)
#pragma pack(1)

//////////////////////////////////////////////////////////////////////////
enum CD_enMsgIdR : pt_word{
	kMIR_Invalid = 0,			//无效消息ID，起到占位符的作用
#ifdef VER_GJ3_
	kMIR_PromptInfo = 0xb,
	kMIR_PreGamePromptInfo = 0xd,
	kMIR_OnNpcPanelInfo = 0x43,
	kMIR_OnItem = 0x69,
	kMIR_OnMove = 0xdb,
	kMIR_UseSkillEffectAtPos = 0xdc,
	kMIR_SkillUsing = 0xdd,
	kMIR_UseSkillEnd = 0xde,
	kMIR_EffectToEntity = 0xdf,
	kMIR_NormalHitted = 0xe0,
	kMIR_OnHpMp = 0xe1,
#elif VER_GJ_
	kMIR_PromptInfo = 0xa,
	kMIR_PreGamePromptInfo = 0xd,
	kMIR_OnNpcPanelInfo = 0x41,
	kMIR_OnItem = 0x67,
	kMIR_OnMove = 0xd8,
	kMIR_UseSkillEffectAtPos = 0xd9,
	kMIR_SkillUsing = 0xda,
	kMIR_UseSkillEnd = 0xdb,
	kMIR_EffectToEntity = 0xdc,
	kMIR_NormalHitted = 0xdd,
	kMIR_OnHpMp = 0xde,
#else
	kMIR_PreGamePromptInfo = 0xd,
	kMIR_PromptInfo = 0xf,
	kMIR_OnNpcPanelInfo = 0x47,
	kMIR_OnHpMp = 0xe5,
	kMIR_OnItem = 0x6d,

	//不必更新
	kMIR_OnMove = 0xda,
	kMIR_UseSkillEffectAtPos = 0xdb,
	kMIR_SkillUsing = 0xdc,
	kMIR_UseSkillEnd = 0xdd,
	kMIR_EffectToEntity = 0xde,
	kMIR_NormalHitted = 0xdf,
#endif
};
//////////////////////////////////////////////////////////////////////////
struct stCDGR_PromptInfo : stCD_GameMsgBase{
	typedef GameVector<int> VecValues;

	pt_word			prompt_id_;
	p_mem_placeholder(2);		//short,=-1
	p_mem_placeholder(1);		//byte
	p_mem_placeholder(3);
	p_mem_placeholder(4 * 4);
	VecValues		values_;
	
	//剩余怪物数量
	int GetRemaining() const;
};
static_assert(0x20 == offsetof(stCDGR_PromptInfo, values_), "");

struct stCDR_PromptInfo : stCDR_MsgBase<kMIR_PromptInfo, false>{
	enum enPromptId : pt_word{
#ifdef VER_GJ3_
		kNameUsed = 0xd,						//此角色名称已被使用，名称太短与名称已使用的ID是一样的
		kNameTooShort = 0xd,					//名称过短
		kMIR_YouEntered = 0x76,					//你已进入：xxx
		kNotIdentified = 0x84,					//目标物品尚未鉴定，未更新
		kRemaining = 0x98,						//剩余怪物数量
		kRoleDie = 0xb1,						//角色死亡
		kItemDestroyed = 0xcf,					//鼠标所选取的物品摧毁
		kCannotUseItemAtTown = 0x112,			//在城镇无法使用此物品
		kLeaveModeClose = 0x16a,				//暂离模式关闭，未更新
		kNameMinGan = 0x17f,					//你输入的内容包含敏感词汇，未更新
#elif VER_GJ_
		kNameUsed = 0xd,						//此角色名称已被使用，名称太短与名称已使用的ID是一样的
		kNameTooShort = 0xd,					//名称过短
		kMIR_YouEntered = 0x76,					//你已进入：xxx
		kNotIdentified = 0x84,					//目标物品尚未鉴定，未更新
		kRoleDie = 0xb3,						//角色死亡
		kItemDestroyed = 0xe3,					//鼠标所选取的物品摧毁
		kCannotUseItemAtTown = 0x112,			//在城镇无法使用此物品
		kLeaveModeClose = 0x16a,				//暂离模式关闭，未更新
		kNameMinGan = 0x17f,					//你输入的内容包含敏感词汇，未更新
#else
		kNameUsed = 0x15,						//此角色名称已被使用
		kNameTooShort = 0x17,					//名称过短
		kMIR_YouEntered = 0x76,					//你已进入：xxx
		kNotIdentified = 0x84,					//目标物品尚未鉴定
		kRemaining = 0x98,						//剩余怪物数量
		kRoleDie = 0xb1,						//角色死亡
		kItemDestroyed = 0xcf,					//鼠标所选取的物品摧毁
		kCannotUseItemAtTown = 0x112,			//在城镇无法使用此物品
		kLeaveModeClose = 0x16a,				//暂离模式关闭
		kNameMinGan = 0x17f,					//你输入的内容包含敏感词汇
#endif
	};
	stCDR_PromptInfo();
	template<class Archive>
	void serialize(Archive& ar){
		ar & prompt_id_;
	}

	enPromptId		prompt_id_;
	short			ukn_s_;			//-1
	pt_byte			ukn_flag_;		//kCannotUseItemAtTown时=2
	pt_dword		ukn_dw_;		//kCannotUseItemAtTown时=0
};

struct stCDR_PreGamePromptInfo : stCDR_MsgBase<kMIR_PreGamePromptInfo, true>{
	enum enPromptId : pt_word{
		kNameUsed = 0x15,						//此角色名称已被使用
		kNameMinGan = 0x16,						//你输入的内容包含无效字符
		kNameTooShort = 0x17,					//名称过短
	};
	stCDR_PreGamePromptInfo();
	template<class Archive>
	void serialize(Archive& ar){
		ar & prompt_id_ & flag_;
	}

	enPromptId		prompt_id_;
	pt_byte			flag_;			//登陆期是0，在游戏内是-1
};

//拿起物品会连续收到两个这样的包，大小是不一样的
//放下物品会连续收到两个这样的包，大小是不一样的
//鉴定物品会收到一个这样的包，大小89
struct stCDR_OnItem : stCDR_MsgBase<kMIR_OnItem, false>{
	template<class Archive>
	void serialize(Archive& ar){}
};

//npc相关消息，比如打开NPC、点击再会、点击继续、点击某个MenuItem等
struct stCDR_NpcPanelInfo : stCDR_MsgBase<kMIR_OnNpcPanelInfo, false>{
	template<class Archive>
	void serialize(Archive& ar){}
};

//////////////////////////////////////////////////////////////////////////
//无论怪物、自己、还是其他玩家。比如技能也会发这个包，比如射箭，也会发这个包
//这个包即是移动包，也是使用技能的包，因为移动也是技能的一种。
struct stCDR_OnMove : stCDR_MsgBase<kMIR_OnMove, false>{
	stCD_EntityMsgInfo				entity_msg_;
	enCD_TouchTargetSmallFlag		flag_;		//=8，如果<=0的话，会再多读取几个字段，一般都是大于0的，没有遇到小于等于0的情况。
	pt_dword						ukn_;		//=0x148,经过解密的
	pt_dword						ukn2_;		//=0x17f,经过解密的
	pt_dword						ukn3_;		//=0x164,经过解密的
	pt_dword						ukn4_;		//=0x18b,经过解密的
	pt_word							skill_id_;	//0x4000
	pt_word							skill_entity_id_;		//=0x82a2，猜测是跟技能有关的一个id吧
};

struct stCDGR_OnMove : stCD_GameEntityMsg{
	stCDGR_OnMove();

	pt_dword			ukn_zero_;				//直接就初始化为0了，并不是通过读取数据置0的。
	stCD_VecInt			pos_dst_;				//$18
	stCD_VecInt			pos_src_;				//$20
	pt_word				skill_id_;				//$28，走路时便是2909，即走路的技能id
	pt_word				ukn_;					//这个好像是使用技能的次数，每使用次技能加1
	enCD_TouchTargetSmallFlag		flag_;		//=8，$2c,如果<=0（一般是-8）的话，会再多读取几个字段，一般都是大于0的，没有遇到小于等于0的情况。
	p_mem_placeholder(0x50);					//防患于未然
};
static_assert(0x18 == offsetof(stCDGR_OnMove, pos_dst_), "");
static_assert(0x28 == offsetof(stCDGR_OnMove, skill_id_), "");
static_assert(0x2c == offsetof(stCDGR_OnMove, flag_), "");

//使用一次技能，只会发一次，不管你是否按着键不放。每个技能都会发，紧接着0xe0发的。
//怪物移动也发这个包。大小10
//使用技能到某个地方时，若距离过远，则会告诉一个最近的距离，以便施放该技能。
//施放的技能，只是显示施放的效果/特效
//攻击对方时，先通过stCDR_OnMove移动到对方，然后收到该包表示施放此技能
//这个包也起到中止技能的作用，因为如果不发它的话，会一直播放着施放技能的效果。
struct stCDR_UseSkillEffectAtPos : stCDR_MsgBase<kMIR_UseSkillEffectAtPos, false>{
	stCD_EntityMsgInfo	info_;

	template<class Archive>
	void serialize(Archive& ar){
		ar & info_.entity_id_ & info_.dst_entity_id_ & info_.flag_;
	}
};
static_assert(10 == sizeof(stCDR_UseSkillEffectAtPos), "");

//e0和db是每个技能都会收到的。大小20
struct stCDR_OnHpMp : stCDR_MsgBase<kMIR_OnHpMp, true>{
	enum enValueType : pt_byte{
		kVT_Hp = 0,
		kVT_Mp,
		kVT_Shield,		//能量护盾
	};
	stCDR_OnHpMp();
	bool IsDied() const;
	template<class Archive>
	void serialize(Archive& ar){
		ar & info_.entity_id_ & info_.dst_entity_id_ & info_.flag_
			& cur_value_ & ukn2_ & value_type_ & ukn_b1_;
	}

	stCD_EntityMsgInfo		info_;
	pt_dword				cur_value_;			//当前的蓝、血等信息，根据其他字段来标识它的种类
	pt_dword				ukn2_;				//一直等于0，即便是死亡时也为0
	enValueType				value_type_;		//血值=0，蓝=1
	pt_byte					ukn_b1_;			//一直等于0，不管有没有死亡
};
static_assert(20 == sizeof(stCDR_OnHpMp), "");

struct stCDR_UseSkillIng : stCDR_MsgBase<kMIR_SkillUsing, false>{

};

//使用一次技能，只会发一次，不管你是否按着键不放。有的技能才发，这是最后一个包
struct stCDR_UseSkillEnd : stCDR_MsgBase<kMIR_UseSkillEnd, false>{

};

//角色或怪物受到伤害都会收到这个包。大小大多数为29，有的为31。
//如果return掉这个包的处理，那么被击打时就没有该特效了。
struct stCDR_EffectToEntity : stCDR_MsgBase<kMIR_EffectToEntity, false>{
	struct stCDR_DamageUkn{
		pt_word		ukn_dw_;		//0x8107,0x816a
		pt_byte		ukn_b_;			//=0,=2
		pt_byte		ukn_b1_;		//=0
		pt_dword	dst_entity_id_;		//目标实体id，被攻击者
		//这应该是个子消息id，根据该值来决定读取包的后面的内容
		pt_word		sub_msg_flag_;	//0x201,根据这个值来shr 0xa，然后判断是否为1，如果为1，则读取后面的word，否则跳过
		//后面还有内容，根据子消息id来决定的。
		char		ukn_[0x1c];
	};

	pt_dword			src_entity_id_;		//伤害者，或者说施放技能的人
	pt_dword			ukn_dw1_;		//0xd4,0x80000119
	pt_word				ukn_w_;			//0x7002
	stCDR_DamageUkn		ukn_;
};

//受到了普通攻击。有时候客户端显示遭受了普通攻击，但也会收到这个包，因为此时是未击中的。
//大小为19.
//可能是普通攻击相关的，其实是猜的
struct stCDR_NormalHitted : stCDR_MsgBase<kMIR_NormalHitted, false>{
	stCD_EntityMsgInfo		info_;
	pt_word				ukn_w_;				//0x8131
	pt_byte				ukn_b_;				//=0
	pt_byte				ukn_b1_;			//=0
	pt_dword			dst_entity_id_;		//目标实体id
	pt_byte				ukn_b2_;			//=1
};
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

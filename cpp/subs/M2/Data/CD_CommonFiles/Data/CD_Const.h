#pragma once
/*
@author zhp
@date 2017/1/24 17:21
*/
#include <ProtocolX/P_DefinesX.h>
#include <string>

const int CD_kMaxStrLen = 0x100;
const int CD_kMaxNameLen = 50;
const int CD_kMaxFilePathLen = 0x100;

enum enCD_RoleAttrValueType{
	kRAVT_Power = 0,					//力量
	kRAVT_Smart,						//敏捷
	kRAVT_Brain,						//智力
	kRAVT_Lucky,						//幸运
	kRAVT_Hp,							//生命
	kRAVT_HpResume,						//生命恢复
	kRAVT_Ukn,							//这个未知，数值挺大，3000
	kRAVT_Energy = 7,					//能量
	kRAVT_Ukn1,
	kRAVT_Ukn2,
	kRAVT_Sp = 0xa,						//体力
	kRAVT_MingZhong = 0xf,				//命中
	kRAVT_HuiBi = 0x10,					//回避
	kRAVT_BaoJiMingZhong = 0x11,		//暴击命中
	kRAVT_BaoJiHuiBi = 0x13,			//暴击回避
	kRAVT_Defense = 0x14,				//防御力
	kRAVT_PhysicalAttack = 0x17,		//物理攻击力
	kRAVT_MagicAttack = 0x18,			//魔法攻击力
	kRAVT_PhysicalDefense = 0x19,		//物理抵抗力
	kRAVT_MagicDefense = 0x1a,			//魔法抵抗力
	kRAVT_Max = 0x22,					//总共有34个属性
};

//角色状态
enum enCD_ObjState{
	kOS_Normal = 1,			//正常状态，静止不动状态
	kOS_Move = 2,			//走路
	kOS_Squat = 4,			//蹲下状态，一般从高空落下后，瞬间会有个蹲下的动作
	kOS_Drop = 5,			//落下状态
	kOS_Jump = 6,			//跳
	kOS_ClimbStairs = 8,	//爬梯子状态
	kOS_Skill = 16,			//技能
	kOS_Npc = 22,			//与npc对话、交易等状态
	kOS_Swim = 27,			//游泳
	kOS_ClimbWall = 29,		//爬墙状态
};

//背包种类
enum enCD_InventoryType{
	kIT_Equip = 0,	//装备
	kIT_Fashion,	//时装
	kIT_Call,		//召唤
	kIT_Material,	//材料
	kIT_Life,		//生活
	kIT_Task,		//任务
	kIT_Gem,		//宝石
	kIT_Other,		//其他
	kIT_Max,
};

//xml字符串id的类型
enum enCD_XmlStrIdType{
	kXSIT_Item = 1,
	kXSIT_Action = 3,
	kXSIT_Skill = 4,
};

enum enCD_QuestState{
	kQS_Doing = 1,		//正在做的任务，尚未完成的任务
	kQS_Done = 2,		//已完成
};

enum enCD_QuestType{
	kQT_Main = 0,		//主线
	kQT_Branch = 1,		//支线
	kQT_Guide = 3,		//CGuideQuest 帮派任务
};

//身上装备类型
enum enCD_BodyEquipType{
	kBET_Helm = 6,			//帽子
	kBET_Coat1 = 8,			//长袍
	kBET_Coat2 = 9,			//长袍
	kBET_Hand = 10,			//手套
	kBET_Shoe = 11,			//鞋
};

enum enCD_LoginCharacterState{
	kLCS_Normal = 1,
	kLCS_Selected = 2,
};

enum enCD_TerrainType{
	kTT_Normal = 1,
	kTT_Water = 0x40,		//64，水
};
//////////////////////////////////////////////////////////////////////////
const pt_csz CD_kGameObjType_GameObject = "GameObject";
const pt_csz CD_kGameObjType_DropItemNIF = "DropItemNIF";
//////////////////////////////////////////////////////////////////////////
//***************类名
const pt_csz CD_kClassName_UIMainPlayerInfo = "UIMainPlayerInfo";
const pt_csz CD_kClassName_UICharacterListDialog = "UICharacterListDialog";
const pt_csz CD_kClassName_UICharacterCreateDialog = "UICharacterCreateDialog";

const pt_csz CD_kClassName_TalkAction = "TalkAction";
const pt_csz CD_kClassName_InteractAction = "InteractAction";
const pt_csz CD_kClassName_PortalAction = "PortalAction";
//////////////////////////////////////////////////////////////////////////
//***********自己定义的数据
enum enCD_TraUiShowFlag{
	kTUSF_Show = 1 << 1,
	kTUSF_UnShow = 1 << 2,
};
//////////////////////////////////////////////////////////////////////////
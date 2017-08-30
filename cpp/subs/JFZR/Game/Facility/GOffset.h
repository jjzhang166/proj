#pragma once
/*
@author zhp
@date 2013-12-31
@purpose 存放偏移地址
*/
#include "GTypes.h"
#include "../../CrackData/jfzrcalltest/jfzrbase.h"

//g_o_->偏移地址一律以g_o_开头
namespace GOffset{
	//发包套接字对象的偏移
	const int g_o_SendCallSocket = JFZR_REVERSAL_SEND_PACKET_SOCKET_OFFSET___;
	//发包所需对象偏移
	const int g_o_SendCallNeedObj1 = JFZR_REVERSAL_SEND_PACKET_OFFSET;
	//发包加密对象偏移
	const int g_o_SendCallSecretObj = JFZR_REVERSAL_SEND_PACKET_JIAMI_OBJ_OFFSET;
	//发包任务对象偏移
	const int g_o_SendCallTaskObj = JFZR_REVERSAL_SEND_PACKET_TASK_OBJ_OFFSET;
	//房间里的所有NPC、怪物、活物等
	const int g_o_GainCreatureListInRoom = JFZR_REVERSAL_LOCAL_OBJ_LIST_OFFSET;
	//获取房间里怪物的偏移
	const int g_o_CreatureInRoom[] = { JFZR_REVERSAL_LOCAL_OBJ_OFFSET1, JFZR_REVERSAL_LOCAL_OBJ_OFFSET2, JFZR_REVERSAL_LOCAL_OBJ_OFFSET3, JFZR_REVERSAL_LOCAL_OBJ_OFFSET4___ };
	//对象ID
	const int g_o_CreatureSerial = JFZR_REVERSAL_LOCAL_OBJ_SERIAL_OFFSET___;
	//职业名字的长度
	//const int g_o_CreatureJob_Len = JFZR_REVERSAL_LOCAL_OBJ_JOBLEN_OFFSET___;
	//职业名
	const int g_o_CreatureJob_Name = JFZR_REVERSAL_LOACL_OBJ_JOBNAME_OFFSET___;
	//名字长度
	//const int g_o_CreatureNameLen[] = { JFZR_REVERSAL_LOCAL_OBJ_NAMELEN_OFFSET___ };
	//名字
	const int g_o_CreatureName = JFZR_REVERSAL_LOACL_OBJ_NAME_OFFSET___;
	//类型长度
	//const int g_o_CreatureTypeLen[] = { JRZR_REVERSAL_LOCAL_OBJ_TYPELEN_OFFSET___ };
	//类型名
	/*
	在副本房间中
	MISC：静态对象，副本里的箱子等物品
	NPC：怪物
	PLAYER：玩家
	UNKNOWN：掉落物品
	在城镇中
	NPC：就是NPC
	PLAYER:玩家
	*/
	const int g_o_CreatureType = JFZR_REVERSAL_LOCAL_OBJ_TPYE_OFFSET___;
	//坐标x
	const int g_o_CreaturePosX = JFZR_REVERSAL_LOCAL_OBJ_X_OFFSET;
	/*//坐标y
	const int g_o_CreaturePosY[] = { JFZR_REVERSAL_LOCAL_OBJ_Y_OFFSET };
	//坐标z
	const int g_o_CreaturePosZ[] = { JFZR_REVERSAL_LOCAL_OBJ_Z_OFFSET };*/
	//方向x
	const int g_o_CreatureDirectionX = JFZR_REVERSAL_LOCAL_OBJ_DIRECTION_X_OFFSET___;
	/*//方向y
	const int g_o_CreatureDirectionY[] = { JFZR_REVERSAL_LOCAL_OBJ_DIRECTION_Y_OFFSET___ };
	//方向z
	const int g_o_CreatureDirectionZ[] = { JFZR_REVERSAL_LOCAL_OBJ_DIRECTION_Z_OFFSET___ };*/
	//技能偏移1
	const unsigned int g_o_SkillOffset1 = JFZR_REVERSAL_SKILL_OFFSET1;
	//技能偏移2
	const unsigned int g_o_SkillOffset2 = JFZR_REVERSAL_SKILL_OFFSET2;
	//技能名字的长度
	const int g_o_SkillNameLen = JFZR_REVERSAL_SKILL_OBJ_NAMELEN_OFFSET___;
	//技能名字
	const int g_o_SkillName = JFZR_REVERSAL_SKILL_OBJ_NAME_OFFSET___;
	//技能等级
	const int g_o_SkillLevel = JFZR_REVERSAL_SKILL_OBJ_LEVEL_OFFSET___;
	//技能ID
	const int g_o_SkillId = JFZR_REVERSAL_SKILL_OBJ_ID_OFFSET___;
	//角色金币
	const int g_o_RoleGold1 = JFZR_REVERSAL_ROLE_GOLD_OFFSET1;
	const int g_o_RoleGold2 = JFZR_REVERSAL_ROLE_GOLE_OFFSET2;
	//角色序号，跟怪物NPC的序号是一样的
	const int g_o_RoleSerial[] = { JFZR_REVERSAL_ROLE_SERIAL_OFFSET___ };
	//角色等级
	const int g_o_RoleLevel = JFZR_REVERSAL_ROLE_LEVEL_OFFSET;
	//房间名字
	const int g_o_RoomMapName = JFZR_REVERSAL_LOCAL_ROOM_MAP_OFFSET___;
	//房间名字长度
	//const int g_o_RoomMapNameLen[] = { JFZR_REVERSAL_LOCAL_ROOM_MAP_LEN_OFFSET___ };
	//房间ID
	const int g_o_RoomMapId = JFZR_REVERSAL_LOCAL_ROOM_ID_OFFSET___;
	//房间ID长度
	//const int g_o_RoomMapIdLen[] = { JFZR_REVERSAL_LOCAL_ROOM_ID_LEN_OFFSET___ };
	//房间门对象偏移
	const int g_o_RoomMapDoorObj = JFZR_REVERSAL_LOCAL_ROOM_DOOR_OBJ_OFFSET___;
	//房间门名
	const int g_o_RoomMapWarpName = JFZR_REVERSAL_LOCAL_ROOM_WARP_NAME_OFFSET___;
	//房间门名的长度
	//const int g_o_RoomMapWarpNameLen[] = { JFZR_REVERSAL_LOCAL_ROOM_WARP_LEN_OFFSET___ };
	//背包物品偏移
	const int g_o_ItemOffset1 = JFZR_REVERSAL_ITEM_BAG_OFFSET1;
	const int g_o_ItemOffset2 = JFZR_REVERSAL_ITEM_BAG_OFFSET2;
	const int g_o_ItemOffset3 = JFZR_REVERSAL_ITEM_BAG_OFFSET3;
	const int g_o_ItemOffset4 = JFZR_REVERSAL_ITEM_BAG_OFFSET4;
	//物品名长度
	//const int g_o_ItemNameLen = JFZR_REVERSAL_ITEM_BAG_NAME_LEN_OFFSET___;
	//物品名
	//const int g_o_ItemName = JFZR_REVERSAL_ITEM_BAG_NAME_OFSSET___;
	//物品品质名长度
	//const int g_o_ItemQualityNameLen = JFZR_REVERSAL_ITEM_BAG_QUALITY_NAME_LEN_OFFSET___;
	//物品品质名
	//const int g_o_ItemQualityName = JFZR_REVERSAL_ITEM_BAG_QUALITY_NAME_OFFSET___;
	//物品个数
	//const int g_o_ItemCount = JFZR_REVERSAL_ITEM_BAG_COUNT_OFFSET___;
	//物品耐久度
	//const int g_o_ItemDurable = JFZR_REVERSAL_ITEM_BAG_DURABLE_OFFSET___;
	//物品对象
	//const int g_o_ItemObjInfo = JFZR_REVERSAL_ITEM_BAG_OBJ_OFFSET___;
	//封印
	//const int g_o_ItemSeal = JFZR_REVERSAL_ITEM_BAG_SEAL_OFFSET___;
	//物品鉴定
	//const int g_o_ItemIdentification = JFZR_REVERSAL_ITEM_BAG_IDENTIFICATION_OFFSET___;
	//掉落物品
	const int g_o_DroppedItem = JFZR_REVERSAL_ITEM_EXISTS_OFFSET___;
	//物品使用部位长度
	//const int g_o_ItemLocationLen = JFZR_REVERSAL_ITEM_OBJ_USE_LEN_OFFSET___;
	//物品使用部位名
	//const int g_o_ItemLocationName = JFZR_REVERSAL_ITEM_OBJ_USE_OFFSET___;
	//物品类型长度
	//const int g_o_ItemTypeLen = JFZR_REVERSAL_ITEM_OBJ_TYPE_LEN_OFFSET___;
	//物品类型名
	//const int g_o_ItemTypeName = JFZR_REVERSAL_ITEM_OBJ_TYPE_OFFSET___;
	//装备偏移1
	const int g_o_EquipItemOffset1 = JFZR_REVERSAL_EQUIP_ITEM_OFFSET3;
	//装备偏移2
	const int g_o_EquipItemOffset2 = JFZR_REVERSAL_EQUIP_ITEM_OFFSET4;
	//副本选择
	const int g_o_CloneMapOffset1 = JFZR_REVERSAL_FB_OFFSET1;
	const int g_o_CloneMapOffset2 = JFZR_REVERSAL_FB_OFFSET2;
	const int g_o_CloneMapOffset3 = JFZR_REVERSAL_FB_OFFSET3___;
	//副本最高可挑战的难度等级
	//const int g_o_CloneMapCurMaxLevel = JFZR_REVERSAL_FB_MAX_LEVEL_OFFSET___;
	//疲劳值
	const int g_o_FatigueOffset = JFZR_REVERSAL_FATIGUE_OFFSET;
	//当前城镇名
	const int g_o_VillageName = JFZR_REVERSAL_VILLAGE_NAME_OFFSET;
	//城镇名长度
	//const int g_o_VillageNameLen = JFZR_REVERSAL_VILLAGE_NAME_LEN_OFFSET____;
	//当前HP
	const int g_o_CurHp = JFZR_REVERSAL_LOCAL_OBJ_HP_OFFSET___;
	//HP上限
	const int g_o_HpLimit = JFZR_REVERSAL_LOCAL_OBJ_MAXHP_OFFSET___;
	//当前MP
	const int g_o_CurMp = JFZR_REVERSAL_LOCAL_OBJ_MP_OFFSET___;
	//MP上限
	const int g_o_MpLimit = JFZR_REVERSAL_LOCAL_OBJ_MAXMP_OFFSET___;
	//是否在城镇中
	//const int g_o_is_town = JFZR_REVERSAL_ROLE_WHERE_THE_MAP_FLAG_OFFSET___;
	//房间门对象的坐标x
	const int g_o_room_door_obj_x = JFZR_REVERSAL_LOCAL_ROOM_X_OFFSET___;
	//游戏地图信息对象偏移
	const int g_o_map_obj = JFZR_REVERSAL_LOCAL_MAP_OBJ_OFFSET;
	//中英文映射
	const int g_o_en2_ch = JFZR_REVERSAL_CHINESE_ENGLISH_MAP_OFFSET___;
	//接任务、完成任务npc
	const int g_o_accept_complete_npc_info = JFZR_REVERSAL_TASK_ACCEPT_NPC_OBJ_OFFSET___;
	//接任务条件
	const int g_o_accept_task_condition = JFZR_REVERSAL_TASK_ACCPET_CONDITION_OFFSET___;
	//完成任务条件
	const int g_o_complete_task_condition = JFZR_REVERSAL_TASK_COMPLETE_CONDITION_OFFSET___;
	//任务完成之奖励物品
	const int g_o_task_reward_items = JFZR_REVERSAL_TASK_QUEST_REWARD_OFFSET___;
	//任务之接受传送
	const int g_o_task_accept_teleport = JFZR_REVERSAL_TAKS_ACCEPT_TELEPORT_OFFSET___;
	//已接任务
	const int g_o_task_received1 = JFZR_REVERSAL_RECEIVED_TASK_OBJ_OFFSET1;
	const int g_o_task_received2 = JFZR_REVERSAL_RECEIVED_TASK_OBJ_OFFSET2;
	const int g_o_task_received3 = JFZR_REVERSAL_RECEIVED_TASK_OBJ_OFFSET3;
	const int g_o_task_received4 = JFZR_REVERSAL_RECEIVED_TASK_OBJ_OFFSET4;
	//npc任务对象
	const int g_o_task_npc[] = { JFZR_REVERSAL_SINGLE_TASK_OBJ_OFFSET1, JFZR_REVERSAL_SINGLE_TASK_OBJ_OFFSET2, JFZR_REVERSAL_SINGLE_TASK_OBJ_OFFSET3 + JFZR_REVERSAL_SINGLE_TASK_OBJ_OFFSET4___ };
	//可接受任务
	const int g_o_task_npc_acceptable = JFZR_REVERSAL_TASK_NPC_ACCPET_QUEST_OFFSET___;
	//已经完成的任务
	const int g_o_task_npc_complete = JFZR_REVERSAL_TASK_NPC_COMPLETE_QUEST_OFFSET___;
	//所有本地对象二叉树
	const int g_o_all_local_obj_btree = JFZR_REVERSAL_ALLOBJ_MAP_FIN_ADDR_OFFSET;
	//是否可交易
	//const int g_o_item_tradable = JFZR_REVERSAL_ITEM_BAG_IS_TRADABLE_OFFSET___;
	//剩余技能点
	const int g_o_left_sp1 = JFZR_REVERSAL_SKILL_SP_OFFSET1___;
	const int g_o_left_sp2 = JFZR_REVERSAL_SKILL_SP_OFFSET2___;
	const int g_o_left_sp_index = JFZR_REVERSAL_SKILL_SP_INDEX_OFFSET___;
	//已接任务的条件
	const int g_o_received_task_active_condition = JFZR_REVERSAL_TASK_ACTIVE_CONDITION_OFFSET___;
	//game wnd proc addr
	const unsigned int						g_o_game_wnd_obj_addr = JFZR_REVERSAL_WND_PROC_OFFSET1___;
	//skill timestamp
	const unsigned int						g_o_skill_timestamp = JFZR_REVERSAL_TIME_STAMP_OFFSET;
	//npc type
	const unsigned int						g_o_game_obj_npc_type = JFZR_REVERSAL_LOCAL_OBJ_NPC_TYPE_FLAG_2___;
	//是否是角斗场
	const unsigned int						g_o_is_arena_stage = JFZR_REVERSAL_SDB_STAGE_INFO_IS_ARENA_STAGE_OFFSET___;
	//pre action play time
	const unsigned int						g_o_pre_action_play_time1 = JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET1;
	const unsigned int						g_o_pre_action_play_time2 = JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET2;
	const unsigned int						g_o_pre_action_play_time3 = JFZR_REVERSAL_LOCAL_PREV_ACTION_PLAY_TIME_OFFSET3;
	//local room pos
	const unsigned int						g_o_local_room_pos = JFZR_REVERSAL_GET_LOCAL_ROOM_POSITION_OFFSET___;
	//////////////////////////////////////////////////////////////////////////
}
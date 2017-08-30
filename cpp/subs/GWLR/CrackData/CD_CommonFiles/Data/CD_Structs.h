#pragma once
/*
@author zhp
@date 2016/4/11 21:40
*/
#include <ProtocolX/P_DefinesX.h>
#include <cstddef>
#include "CD_Vtbl.h"
#include "CD_Offset.h"
#include "CD_PT_Updates.h"
#include "CD_Const.h"
#include <array>

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//苏辉的注释：NPC对的细分类型，普通怪物 = 2，boos = 1，箱子 = 3，玩家和玩家的宠物不存在
enum class CD_En_ObjSubType{
	kBoss = 1,			//BOSS
	kXiaoGuai,			//小怪
	kBuJiXiang,			//补给箱，城镇中的NPC也是这个数值
};
p_ostm_to_num(CD_En_ObjSubType)
//游戏对象模板信息
struct stCD_ObjDbInfo{
	struct stCD_ObjTitleInfo{
		pt_csz		title_;		//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_TITLE6___

		friend std::ostream& operator << (std::ostream& ostm, const stCD_ObjTitleInfo& v);
	};

	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_so_obj_db_npc_id - 4);
	int				obj_db_id_;		//游戏中叫NpcId
	p_mem_placeholder(g_so_obj_db_name_ch - 4 - g_so_obj_db_npc_id);
	pt_csz			ch_name_;		//中文名 MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME5___  UPDATES_MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET2
	CD_En_ObjSubType	type_;		//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NPC_TYPE___
	p_mem_placeholder(g_so_obj_db_name_en - 8 - g_so_obj_db_name_ch);
	pt_csz			en_name_;
	p_mem_placeholder(g_so_obj_db_title1 - 4 - g_so_obj_db_name_en);
	stCD_ObjTitleInfo*	title_info_;	//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_TITLE5___

	friend std::ostream& operator << (std::ostream& ostm, const stCD_ObjDbInfo& v);
	bool ObjValid() const;
};
static_assert(g_so_obj_db_npc_id == offsetof(stCD_ObjDbInfo, obj_db_id_), "");
static_assert(g_so_obj_db_name_ch == offsetof(stCD_ObjDbInfo, ch_name_), "");
static_assert(g_so_obj_db_name_en == offsetof(stCD_ObjDbInfo, en_name_), "");
static_assert(g_so_obj_db_title1 == offsetof(stCD_ObjDbInfo, title_info_), "");

enum class CD_En_NetObjType{
	kRole = 0x100,		//角色
	kNpc = 0x101,		//NPC，随行猫也属于NPC
};
p_ostm_to_num(CD_En_NetObjType)
/*
有packet id的才有这个对象
NPC、角色、怪物（怪物对象未测试）等对象都有这么一个结构.
打开NPC操作可断到索引游戏对象的代码处。
*/
struct stCD_NetObjUkn;
struct stCD_RoleBuffMgr;
struct stCD_LocalObj;
//这个类或许主要用于与服务端通信的
struct stCD_NetObj{
	pt_pcvoid			vtbl_;
	pt_dword			packet_id_;			//net id,网络ID	MMOCLIENT_REVERSAL_MONSTER_HP_PACKET_ID_OFFSET___
	pt_dword			id_;				//local id,本地id
	stCD_NetObjUkn*		ukn_;
	stCD_RoleBuffMgr*	role_buf_mgr_;
	p_mem_placeholder(g_so_net_obj_type - 4 - g_so_role_role_buf_mgr);
	CD_En_NetObjType	type_;			//这个有问题，这个字段应该不是表示类型的
	p_mem_placeholder(g_so_net_obj_room_num_ - 4 - g_so_net_obj_type);
	int					room_num_;
	p_mem_placeholder(g_so_net_obj_ukn_impl - 4 - g_so_net_obj_room_num_);
	//此采集物是"骨堆"时，pt_csz**处是个字符串"SetBlackBoard"。
	//庄园里的采集物，此字段有的为空，有的不为空。
	pt_pcvoid			ukn_impl_;		//采集物时，这个字段均是个指针，当"SceneBuffArea"时，该字段为0
	p_mem_placeholder(g_so_net_obj_name - 4 - g_so_net_obj_ukn_impl);
	pt_csz				name_;
	p_mem_placeholder(g_so_net_obj_str_area - 4 - g_so_net_obj_name);
	pt_csz				str_area_;		//采集物="MHCommonArea" 怪物死亡时播放动画的时候="SceneBuffArea"
	p_mem_placeholder(g_so_net_obj_local_obj - 4 - g_so_net_obj_str_area);
	//采集物时，这个字段不是指向LocalObj对象
	stCD_LocalObj*		local_obj_;

	void OutStream(std::ostream& ostm) const;
	friend std::ostream& operator << (std::ostream& ostm, const stCD_NetObj& v);
	p_define_cast
	bool ObjValid() const;
};
static_assert(offsetof(stCD_NetObj, id_) == 8, "");
static_assert(offsetof(stCD_NetObj, ukn_) == g_so_net_obj_ukn, "");
static_assert(offsetof(stCD_NetObj, role_buf_mgr_) == g_so_role_role_buf_mgr, "");
static_assert(offsetof(stCD_NetObj, type_) == g_so_net_obj_type, "");
static_assert(offsetof(stCD_NetObj, room_num_) == g_so_net_obj_room_num_, "");
static_assert(offsetof(stCD_NetObj, ukn_impl_) == g_so_net_obj_ukn_impl, "");
static_assert(offsetof(stCD_NetObj, name_) == g_so_net_obj_name, "");
static_assert(offsetof(stCD_NetObj, str_area_) == g_so_net_obj_str_area, "");
static_assert(offsetof(stCD_NetObj, local_obj_) == g_so_net_obj_local_obj, "");

struct stCD_NetObj_Npc : stCD_NetObj{
	p_mem_placeholder(g_o_net_obj_2_db_info_ - sizeof(stCD_NetObj));
	stCD_ObjDbInfo*		db_info_;	//UPDATES_MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME4

	friend std::ostream& operator << (std::ostream& ostm, const stCD_NetObj_Npc& v);
	bool ObjValid() const;
};
static_assert(offsetof(stCD_NetObj_Npc, db_info_) == g_o_net_obj_2_db_info_, "");

struct stCD_CollectRoomInfo{
	p_mem_placeholder(g_so_collect_room_info_room_num);
	int			room_num_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_CollectRoomInfo& v);
};
static_assert(offsetof(stCD_CollectRoomInfo, room_num_) == g_so_collect_room_info_room_num, "");
struct stCD_NetObj_Collect : stCD_NetObj{
	p_mem_placeholder(g_so_net_obj_collect_name - sizeof(stCD_NetObj));
	pt_csz*		collect_name_;		//采集物中文名,此时采集物的偏移是$+168，而$+170    4FA36610    40000000  UNICODE "求。事件日志可能会包含详细信息。\r\n"
	p_mem_placeholder(g_o_net_obj_collect_monster_packet_id_ - 4 - g_so_net_obj_collect_name);
	pt_dword	monster_packet_id_;	//怪物尸体，怪物死亡后怪物的packet id UPDATES_MMOCLIENT_REVERSAL_COLLECT_OBJ_MONSTER_PACKET_ID_OFFSET
	p_mem_placeholder(g_so_net_obj_room_obj - 4 - g_o_net_obj_collect_monster_packet_id_);
	stCD_CollectRoomInfo*	room_info_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_NetObj_Collect& v);
};
static_assert(offsetof(stCD_NetObj_Collect, collect_name_) == g_so_net_obj_collect_name, "");
static_assert(offsetof(stCD_NetObj_Collect, monster_packet_id_) == g_o_net_obj_collect_monster_packet_id_, "");
static_assert(offsetof(stCD_NetObj_Collect, room_info_) == g_so_net_obj_room_obj, "");

struct stObjNodeInfo{
	p_mem_placeholder(sizeof(pt_pvoid));
	pt_dword			obj_packet_id_;
	stCD_NetObj*		game_obj_;
};
static_assert(offsetof(stObjNodeInfo, game_obj_) == 8, "");

struct stCD_NetObjRealCont{
	pt_dword					ukn_zero_;
	pt_pvoid					ukn_ptr_;
	stObjNodeInfo**				obj_begin_ptr_;
	stObjNodeInfo**				obj_end_ptr_;
};
struct stCD_ObjCont{
	stCD_Vtbl_ObjCont*				vtbl_;
	stCD_NetObjRealCont				obj_cont_;
};

struct stCD_NetObjMgr{
	stCD_Vtbl_ObjMgr*			vtbl_;
	stCD_ObjCont*				obj_cont_;
	p_mem_placeholder(g_so_net_obj_mgr_packet_id - 4 - 4);
	pt_dword					role_packet_id_;
};
static_assert(offsetof(stCD_NetObjMgr, role_packet_id_) == g_so_net_obj_mgr_packet_id, "");

struct stCD_UknObj{
	p_mem_placeholder(g_so_net_obj_mgr);
	stCD_NetObjMgr*	ukn_obj_;
};
static_assert(offsetof(stCD_UknObj, ukn_obj_) == g_so_net_obj_mgr, "");

/*
全局的数据结构，其地址存放在一个全局指针里
*/
struct stCD_GlobalData{
	pt_pcvoid			vtbl_;
	pt_csz				str_client_;		//$+4      377658AC   2F1C6A21  ASCII "client"
	p_mem_placeholder(0x18 - 4 - 4);
	pt_csz				str_dir_bin_;		//$+18     377658C0   2F1C69F8  ASCII "e:\program files\gwlr\怪物猎人online\bin"
	p_mem_placeholder(0x30 - 4 - 0x18);
	pt_csz				str_dir_bind_client_;		//$+30     377658D8   2F1C6A28  ASCII "e:\program files\gwlr\怪物猎人online\bin\client"
	p_mem_placeholder(g_so_global_net_obj_mgr - 4 - 0x30);
	stCD_NetObjMgr*		obj_mgr_;
	p_mem_placeholder(g_so_ukn_obj_2_net_obj_mgr - 4 - g_so_global_net_obj_mgr);
	stCD_UknObj*		ukn_obj_;		//$+A0     3450C6F0   3381DA60
};
static_assert(offsetof(stCD_GlobalData, obj_mgr_) == g_so_global_net_obj_mgr, "");
static_assert(offsetof(stCD_GlobalData, ukn_obj_) == g_so_ukn_obj_2_net_obj_mgr, "");
//////////////////////////////////////////////////////////////////////////
enum class CD_En_LocalObjType{
	kInvalid = 0,
	kNpc = 0x0497,
	kPlayer1 = 0x0597,		//stCD_ObjBase::net_obj_不为空，比如自己的角色
	kPlayer2 = 0x05B0,		//stCD_ObjBase::net_obj_为空
	kStatic = 0x04B0,	//此时的name等于Player_DefaultEntity或EmCommon_DefaultEntity
	//MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_COLLECT___
	kCollect = 0x0695,
	//MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_DOOR___
	kDoor1 = 0x0690,
	//MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_DOOR2___
	kDoor2 = 0x0790,
	//MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_BUCKET___  MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_APEAR___
	kAppear = 0x00000510,	//比如闪光的东西，任务 有用?无用? 之奇特的爱好里灭火球的对象遍历
	//MMOCLIENT_REVERSAL_TRAVERSE_ACTOR_TYPE_ROCK___
	kRock = 0x00000595,		//石块 副本任务 莉莉的风格
};
p_ostm_to_num(CD_En_LocalObjType)
//这个类或许主要用于渲染的
struct stCD_ObjBase{
	pt_pvoid		vtbl_;
	pt_word			type_;				//CD_En_LocalObjType, 对象类型 MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_XYZ_OFFSET_CLASS___
	p_mem_placeholder(g_so_obj_base_id - 6);
	pt_dword		id_;				//local id MMOCLIENT_REVERSAL_TRAVERSE_OBJ_LOCAL_ID_OFSET___
	p_mem_placeholder(g_so_obj_base_tmp_packet_id - 4 - g_so_obj_base_id);
	pt_dword		tmp_packet_id_;		//boss对象这个字段有效
	p_mem_placeholder(g_so_obj_base_pos - 4 - g_so_obj_base_tmp_packet_id);
	stCD_Vector3	pos_;		//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR2_OFFSET___
	p_mem_placeholder(g_so_obj_dir - sizeof(stCD_Vector3) - g_so_obj_base_pos);		//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_DIR_OFFSET___
	stCD_Vector3	obj_dir_;			//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_DIR_OFFSET___
	p_mem_placeholder(g_so_pos_dir_info - sizeof(stCD_Vector3) - g_so_obj_dir);
	stCD_PosDirInfo	pos_dir_info;		//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_ACTOR_VECTOR_OFFSET___
	p_mem_placeholder(g_so_local_obj_base_link_str - sizeof(stCD_PosDirInfo) - g_so_pos_dir_info);
	pt_csz	door_link_str_;	//$+B8     62B86D60   63104138  ASCII "NewLink" 类似这样的字符串
	p_mem_placeholder(g_so_obj_base_name - 4 - g_so_local_obj_base_link_str);
	pt_csz			name_;			//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFFSET_NAME___
	p_mem_placeholder(g_so_obj_base_packet_id - 4 - g_so_obj_base_name);
	pt_dword		packet_id_;			//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_PACKET_ID___
	p_mem_placeholder(g_so_obj_base_net_obj - 4 - g_so_obj_base_packet_id);
	stCD_NetObj*	net_obj_;		//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET_NAME1___

	p_define_cast
	friend std::ostream& operator << (std::ostream& ostm, const stCD_ObjBase& v);
	bool ObjValid() const{
		return true;
	}
	CD_En_LocalObjType GetType() const{
		return CD_En_LocalObjType(type_);
	}
};
static_assert(offsetof(stCD_ObjBase, id_) == g_so_obj_base_id, "");
static_assert(offsetof(stCD_ObjBase, tmp_packet_id_) == g_so_obj_base_tmp_packet_id, "");
static_assert(offsetof(stCD_ObjBase, packet_id_) == g_so_obj_base_packet_id, "");
static_assert(offsetof(stCD_ObjBase, pos_dir_info) == g_so_pos_dir_info, "");
static_assert(offsetof(stCD_ObjBase, door_link_str_) == g_so_local_obj_base_link_str, "");
static_assert(offsetof(stCD_ObjBase, name_) == g_so_obj_base_name, "");
static_assert(offsetof(stCD_ObjBase, pos_) == g_so_obj_base_pos, "");


enum class CD_En_ObjStatus{
	kFree = 0,			//在城镇中，玩家的状态都是这个，比如收刀状态
	kFighting = 3,		//战斗状态，比如拿着刀的时候
	kDoing = 5,			//使用技能状态，即砍的时候，或采集的时候，正在做某事的时候
	kNpc = 0xe,			//Npc和随行猫的状态都是0xe
	kInvalid = -1
};
p_ostm_to_num(CD_En_ObjStatus)
static_assert(sizeof(CD_En_ObjStatus) == sizeof(pt_dword), "");
static_assert((int)CD_En_ObjStatus::kInvalid == -1, "");

enum class CD_En_SkillId{
	kSC_LeftBt = 0,	// 鼠标左键 MMOCLIENT_REVERSAL_SKILL_ID_MOUSE_LEFT
	kSC_RightBt = 1,	// 鼠标右键
	kSC_KeyQ = 3,		// 键盘q，收刀
	kSC_KeyE = 4,		// 键盘z，采集，扒皮
	kSC_KeyZ = 5,		// 键盘e，刺
	kSC_KeyF = 0xc,	// 键盘f，奥义觉醒
};
struct stCD_SkillObj{
	bool UseSkill(CD_En_SkillId skill_id) const;
};
struct stCD_LocalObj{
	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_so_local_obj_id - 4);
	pt_dword		id_;			//local id
	stCD_ObjBase*	obj_info_;		//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_XYZ_OFFSET___
	p_mem_placeholder(g_so_obj_status - 4 - g_so_local_obj_info);
	CD_En_ObjStatus	status_;		//MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_STATUS_OFFSET___
	p_mem_placeholder(g_o_local_obj_skill_ - sizeof(CD_En_ObjStatus) - g_so_obj_status);
	stCD_SkillObj*	skil_obj_;	//UPDATES_MMOCLIENT_REVERSAL_SKILL_OFFSET

	friend std::ostream& operator << (std::ostream& ostm, const stCD_LocalObj& v);
	bool ObjValid() const;
};
static_assert(offsetof(stCD_LocalObj, id_) == g_so_local_obj_id, "");
static_assert(offsetof(stCD_LocalObj, obj_info_) == g_so_local_obj_info, "");
static_assert(offsetof(stCD_LocalObj, status_) == g_so_obj_status, "");
static_assert(offsetof(stCD_LocalObj, skil_obj_) == g_o_local_obj_skill_, "");


typedef stCD_NodeT<stCD_LocalObj> stCD_LocalObjNode;
struct stCD_LocalObjMgr{
	p_mem_placeholder(g_so_local_obj_mgr);
	stCD_LocalObjNode		objs_;			//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_OFFSET4___
};
static_assert(offsetof(stCD_LocalObjMgr, objs_) == g_so_local_obj_mgr, "");

struct stCD_LocalIdMgr1{
	p_mem_placeholder(g_o_local_obj_mgr_);
	stCD_LocalObjMgr*	the_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_TRAVERSE_OBJ_OFFSET3
};
static_assert(offsetof(stCD_LocalIdMgr1, the_mgr_) == g_o_local_obj_mgr_, "");

struct stCD_SocketMgr1;
struct stCD_GloLocalIdMgr{
	p_mem_placeholder(g_so_local_obj_mgr1);
	stCD_LocalIdMgr1*	the_mgr_;		//UPDATES_MMOCLIENT_REVERSAL_GAME_OFFSET2
	p_mem_placeholder(g_o_local_socket_mgr1_ - 4 - g_so_local_obj_mgr1);
	stCD_SocketMgr1*	socket_mgr_;	//UPDATES_MMOLCINET_REVERSAL_SOCKET_OFFSET1
};
static_assert(offsetof(stCD_GloLocalIdMgr, the_mgr_) == g_so_local_obj_mgr1, "");
static_assert(offsetof(stCD_GloLocalIdMgr, socket_mgr_) == g_o_local_socket_mgr1_, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_AllLocalObjMgr{
	p_mem_placeholder(g_o_all_local_obj_mgr_all_objs_);
	stCD_Array<stCD_ObjBase*>		all_objs_;		//UPDATES_MMOCLIENT_REVERSAL_TRAVERSAL_SINGLE_OBJ_OFFSET2
};
//////////////////////////////////////////////////////////////////////////
//地图所在的图的信息
struct stCD_MapDbInfo{
	p_mem_placeholder(g_so_map_db_info_name);
	pt_csz		name_;		//地图模板名 MMOCLIENT_REVERSAL_ACTOR_IN_MAP_MU_BAN_ADDR_NAME_OFFSET___

	friend std::ostream& operator << (std::ostream& ostm, const stCD_MapDbInfo& v);
};
static_assert(offsetof(stCD_MapDbInfo, name_) == g_so_map_db_info_name, "");
//副本类型
enum class CD_En_FbType{
	kFT_Sociaty = 1,	//公会副本 MMOCLIENT_REVERSAL_PACKET_MSG_ID_GONG_HUI_INDEX___
	kFT_Cat = 3,		//喵喵副本 MMOCLIENT_REVERSAL_PACKET_MSG_ID_MIAO_MIAO_INDEX___
	kFT_QianShou = 4,	//千狩 MMOCLIENT_REVERSAL_PACKET_MSG_ID_QIAN_SHOU_INDEX___
	kFT_WangLi = 4,			//王立副本
	//凡是类型为 8,23,16的，都不判断等级，8和23的表现主线任务的副本，16大图标的副本且需求等级为0的
	kFT_8 = 8,
	kFT_16 = 16,
	kFT_23 = 23,
	kFT_WangLiCream = 26,	//王立精英副本
	kFT_WangLiCream1 = 29,	//古龙征伐！钢龙！
	kFT_WangLiCream2 = 34,	//精英！一角龙
	kFT_Invalid,
};
p_ostm_to_num(CD_En_FbType)
//副本难度
enum class CD_En_FbDifficulty{
	kFD_Zero = 0,			//城镇中，房间等级为0
	kFD_ShiLian,			//试炼
	kFD_Fight,				//挑战，王立副本（包括一星、二星）
	kFD_JiXianOrJingYing,	//极限或精英，其副本难度等级为3
	kFD_Invalid,
};
p_ostm_to_num(CD_En_FbDifficulty)
struct stCD_MapDb{
	p_mem_placeholder(g_so_map_db_id);
	pt_dword	map_id_;		//地图ID MMOCLIENT_REVERSAL_ATCOR_IN_MAP_FB_ID_OFFSET___
	pt_dword	map_db_info_id_;		//地图所在的图的ID MMOCLIENT_REVERSAL_ACTOR_IN_MAP_MU_BAN_ID_OFFSET___  	//MMOCLIENT_REVERSAL_FB_INFO_MAP_ID_OFFSET___
	stCD_MapDbInfo*	map_db_info_;		//MMOCLIENT_REVERSAL_ACTOR_IN_MAP_MU_BAN_ADDR_OFFSET___
	pt_csz		map_name_;		//MMOCLIENT_REVERSAL_ACTOR_IN_MAP_OFFFSET3___	//MMOCLIENT_REVERSAL_FB_INFO_NAME_OFFSET___
	CD_En_FbType		type_;		// 选择副本的时候发包需要的数据
	p_mem_placeholder(g_so_map_db_diff_lvl - 4 - g_so_map_db_type);
	CD_En_FbDifficulty		diff_lvl_;	// 副本挑战等级
	p_mem_placeholder(g_so_map_db_role_lvl - 4 - g_so_map_db_diff_lvl);
	int		role_lvl_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_MapDb& v);
};
static_assert(offsetof(stCD_MapDb, map_id_) == g_so_map_db_id, "");
static_assert(offsetof(stCD_MapDb, map_db_info_id_) == g_so_map_db_info_id, "");
static_assert(offsetof(stCD_MapDb, map_db_info_) == g_so_map_db_info, "");
static_assert(offsetof(stCD_MapDb, map_name_) == g_so_map_db_name, "");
static_assert(offsetof(stCD_MapDb, type_) == g_so_map_db_type, "");
static_assert(offsetof(stCD_MapDb, diff_lvl_) == g_so_map_db_diff_lvl, "");
static_assert(offsetof(stCD_MapDb, role_lvl_) == g_so_map_db_role_lvl, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_NpcMgr{
	bool IsAnyNpcHasTheTask(int task_id) const;
};
struct stCD_DoneFbMgr{
	bool IsTheFbDone(int fb_id) const;
};
struct stCD_EnterableFbMgr{
	p_mem_placeholder(g_so_enterable_fb_mgr);
	stCD_Array<int>	fb_info_;

	void DebugInfo() const;
};
static_assert(offsetof(stCD_EnterableFbMgr, fb_info_) == g_so_enterable_fb_mgr, "");
//////////////////////////////////////////////////////////////////////////
//MMO_CUR_FARM_FACILITY
struct stCD_CurFarmFacilityInfo{
	DWORD	dwUnKnow1;
	int		cur_elapse_time_;	//当前流逝时间，单位秒
	DWORD	dwUnKnow3;		//==1
	int		cur_lvl_;		//当前采集点等级
	DWORD	dwUnKnow4;

	void DebugInfo() const;
};
enum class CD_En_FarmFacilityIdx{
	kOre = 0,		//矿场
	kFish,			//鱼塘
	kInsect,		//虫栖地
	kBed,			//育菇木床
	kHouse,			//蜂房
	kField,			//草药田
	kInvalid,
};
p_ostm_to_num(CD_En_FarmFacilityIdx)
//MMO_FARM_FACILITY_STATE
struct stCD_CurFarmFieldInfo{
	int		flag_;		//开启状态 = 1 表示此草药田可以种植; 开启状态 = 0 表示此草药田未开启
	int		seed_id_;	//种植物品id = 0 表示此处没有种植物品，需要种植 如果有值，则是种子的id（遍历种子信息中的id）
	int		nUnKnow;

	void DebugInfo() const;
};
enum class CD_En_FarmFieldIdx{
	kField1 = 0,
	kField2,
	kField3,
	kInvalid
};
p_ostm_to_num(CD_En_FarmFieldIdx)

struct stCD_FarmTimeDb{
	p_mem_placeholder(g_so_farm_time_db_time);
	int		grown_time_;		//植物成熟周期时间，单位秒(猜测)
};
static_assert(offsetof(stCD_FarmTimeDb, grown_time_) == g_so_farm_time_db_time, "");

struct stCD_FarmFacility{
	p_mem_placeholder(g_o_role_self_farm_id_ - g_o_role_farm_facility_);
	pt_dint		self_farm_id_;		//UPDATES_MMOCLIENT_REVERSAL_ACTOR_ZHUANG_YUAN_ID_OFFSET
	p_mem_placeholder(g_so_farm_facility_cur_facility_info - 8 - (g_o_role_self_farm_id_ - g_o_role_farm_facility_));
	std::array<stCD_CurFarmFacilityInfo, (int)CD_En_FarmFacilityIdx::kInvalid>	facility_info_;
	p_mem_placeholder(g_o_farm_facility_cur_field_info_ - g_so_farm_facility_cur_facility_info - sizeof(std::array<stCD_CurFarmFacilityInfo, (int)CD_En_FarmFacilityIdx::kInvalid>));
	std::array<stCD_CurFarmFieldInfo, (int)CD_En_FarmFieldIdx::kInvalid>	field_info_;	//UPDATES_MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_OFFSET2

	void DebugInfo() const;
};
static_assert(offsetof(stCD_FarmFacility, self_farm_id_) == g_o_role_self_farm_id_ - g_o_role_farm_facility_, "");
static_assert(offsetof(stCD_FarmFacility, facility_info_) == g_so_farm_facility_cur_facility_info, "");
static_assert(offsetof(stCD_FarmFacility, field_info_) == g_o_farm_facility_cur_field_info_, "");

struct stCD_FarmDb{
	p_mem_placeholder(g_so_farm_db_id);
	int			id_;
	int			lvl_;
	p_mem_placeholder(g_so_farm_db_point - 4 - g_so_farm_db_lvl);
	int			point_;
	int			silver_;
	p_mem_placeholder(g_so_farm_db_name_info - 4 - g_so_farm_db_silver);
	pt_csz		name_;
	pt_csz		desc_;
	int			role_lvl_;

	void DebugInfo() const;
};
static_assert(offsetof(stCD_FarmDb, id_) == g_so_farm_db_id, "");
static_assert(offsetof(stCD_FarmDb, lvl_) == g_so_farm_db_lvl, "");
static_assert(offsetof(stCD_FarmDb, point_) == g_so_farm_db_point, "");
static_assert(offsetof(stCD_FarmDb, silver_) == g_so_farm_db_silver, "");
static_assert(offsetof(stCD_FarmDb, name_) == g_so_farm_db_name_info, "");
static_assert(offsetof(stCD_FarmDb, desc_) == g_so_farm_db_desc_info, "");
static_assert(offsetof(stCD_FarmDb, role_lvl_) == g_so_farm_db_role_lvl, "");

struct stCD_SeedDb{
	p_mem_placeholder(g_so_seed_db_item_id);
	int		item_id_;
	pt_csz	item_name_;

	void DebugInfo() const;
};
static_assert(offsetof(stCD_SeedDb, item_id_) == g_so_seed_db_item_id, "");
static_assert(offsetof(stCD_SeedDb, item_name_) == g_so_seed_db_item_name, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_RoleSomeEncryptInfo{
	enum class MemGetterParam{
		kNormal = 6,
	};
	typedef int (stCD_RoleSomeEncryptInfo::*MemGetter)(MemGetterParam) const;
};

enum class CD_En_PoisonStatus{
	kOk = 0,			//没在中毒状态
	kPoisoning = 1,		//在中毒状态中
};
p_ostm_to_num(CD_En_PoisonStatus)
struct stCD_PackagesMgr;
struct stCD_GiftBag;
struct stCD_MailMgr;
struct stCD_WeekCreditMgr;
struct stCD_NetObj_Role : stCD_NetObj{
	p_mem_placeholder(g_o_role_encrypt_info_ - sizeof(stCD_NetObj));
	stCD_RoleSomeEncryptInfo	some_info_;
	p_mem_placeholder(g_so_role_lvl - sizeof(stCD_RoleSomeEncryptInfo) - g_o_role_encrypt_info_);
	stCD_EncryptInt		lvl_;	//MMOCLIENT_REVERSAL_ROLE_ENCRYPTION_LEVEL_OFFSET___
	p_mem_placeholder(g_so_role_cur_exp - sizeof(stCD_EncryptInt) - g_so_role_lvl);
	stCD_EncryptInt		cur_exp_;	//MMOCLIENT_REVERSAL_ROLE_CUR_EXP_OFFSET___
	p_mem_placeholder(g_so_role_cur_hp - sizeof(stCD_EncryptInt) - g_so_role_cur_exp);
	stCD_EncryptInt		cur_hp_;	//MMOCLIENT_REVERSAL_ROLE_ENCRYPTION_CURHP_OFFSET___
	stCD_EncryptInt		hp_;		//MaxHp
	p_mem_placeholder(g_so_role_nei_shang_ - sizeof(stCD_EncryptInt) - g_so_role_hp_);
	stCD_EncryptInt		nei_shang_;	//内伤 MMOCLIENT_REVERSAL_ROLE_ENCRYPTION_CURNEISHANG_OFFSET___
	p_mem_placeholder(g_so_role_cur_endure - sizeof(stCD_EncryptInt) - g_so_role_nei_shang_);
	stCD_EncryptInt		cur_endure_;	//耐力
	stCD_EncryptInt		endure_;		//最大耐力
	p_mem_placeholder(g_o_role_zhan_wei_zhi_ - sizeof(stCD_EncryptInt) - g_so_role_endure_);
	stCD_EncryptInt		zhan_wei_zhi_;	//斩味值 武器斩味值 小于 700000，为粉红色，钝口刀型  可设定为需要磨刀 UPDATES_MMOCLIENT_REVERSAL_ROLE_ZHAN_WEI_ZHI_OFFSET
	p_mem_placeholder(g_so_role_gold - sizeof(stCD_EncryptInt) - g_o_role_zhan_wei_zhi_);
	stCD_EncryptInt		gold_;			//流通金币 UPDATES_MMOCLIENT_REVERSAL_ROLE_MEMORY_LIU_TONG_OFFSET
	stCD_EncryptInt		silver_;		//绑定的银币 UPDATES_MMOCLIENT_REVERSAL_ROLE_MEMORY_BANG_DING_OFFSET
	p_mem_placeholder(g_so_role_left_fb_cnt - sizeof(stCD_EncryptInt) - g_so_role_silver);
	stCD_EncryptInt		left_fb_cnt_;	//狩猎券、猎守许可证 UPDATES_MMOCLIENT_REVERSAL_LIE_SHOU_XU_KE_ZHENG_COUNT_OFFSET1
	p_mem_placeholder(g_o_role_lian_qi_cao_ - sizeof(stCD_EncryptInt) - g_so_role_left_fb_cnt);
	stCD_EncryptInt		lian_qi_cao_;	//当炼气槽值=100时，则炼气槽满，右键释放，然后连续右键 UPDATES_MMOCLIENT_REVERSAL_ROLE_LIAN_QI_CAO_OFFSET
	p_mem_placeholder(g_so_role_jing_suan_point - sizeof(stCD_EncryptInt) - g_o_role_lian_qi_cao_);
	stCD_EncryptInt		jing_suan_point_;	//精算点 UPDATES_MMOCLIENT_REVERSAL_ROLE_MEMORY_JING_SUAN_DIAN_OFFSET
	p_mem_placeholder(g_o_role_npc_mgr_ - sizeof(stCD_EncryptInt) - g_so_role_jing_suan_point);
	stCD_NpcMgr*		npc_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_GAME_OBJ_OFFSET1
	p_mem_placeholder(g_o_role_mail_mgr_ - 4 - g_o_role_npc_mgr_);
	stCD_MailMgr*		mail_mgr_;
	stCD_GiftBag*	gift_bag_;	//UPDATES_MMOCLIENT_REVERSAL_GIFT_BAG_INFO_OFFSET1
	stCD_WeekCreditMgr*	week_credit_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET1
	p_mem_placeholder(g_o_role_enterable_fb_mgr_ - 4 - g_o_role_week_credit_mgr_);
	stCD_EnterableFbMgr*	enterable_fb_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_IN_FB_INFO_OFFSET1
	stCD_DoneFbMgr*		done_fb_mgr_;			//UPDATES_MMOCLIENT_REVERSAL_TASK_FB_OFFSET
	p_mem_placeholder(g_so_role_poison_state - 4 - g_o_role_done_fb_mgr_);
	CD_En_PoisonStatus		poison_status_;	//中毒状态 UPDATES_MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET MMOCLIENT_REVERSAL_ROLE_ZHONG_DU_STATE_OFFSET___
	p_mem_placeholder(g_o_role_farm_facility_ - sizeof(CD_En_PoisonStatus) - g_so_role_poison_state);
	stCD_FarmFacility	farm_facility_;		//UPDATES_MMOCLIENT_REVERSAL_FARM_FACILITY_STATE_OFFSET1
	p_mem_placeholder(g_o_role_ser_line_ - sizeof(stCD_FarmFacility) - g_o_role_farm_facility_);
	int			ser_line_;	//服务器线路 UPDATES_MMOCLIENT_REVERSAL_ACTOR_OFFSET_XIAN_LU
	p_mem_placeholder(g_o_role_packages_mgr_ - 4 - g_o_role_ser_line_);
	stCD_PackagesMgr*		packages_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_OFFSET1	UPDATES_MMOCLIENT_REVERSAL_TIDY_INVENTORY_ITEM_OFFSET

	//未找到
	/*p_mem_placeholder(g_so_role_single_exp);
	stCD_EncryptInt		single_exp_;	//MMOCLIENT_REVERSAL_ROLE_SINGLE_EXP_OFFSET___
	p_mem_placeholder(g_so_role_double_exp - sizeof(stCD_EncryptInt) - g_so_role_single_exp);
	stCD_EncryptInt		double_exp_;	//MMOCLIENT_REVERSAL_ROLE_DOUBLE_EXP_OFFSET___*/

	friend std::ostream& operator << (std::ostream& ostm, const stCD_NetObj_Role& v);
	using EncryptInfoT = stCD_RoleSomeEncryptInfo (stCD_NetObj_Role::*);
};

//用来获取角色成员变量数值，其内部封装了解密
struct stCD_MemGetter{
	pt_pcvoid	vtbl_;		//没有被用到
	pt_dword	ukn_num_;	//没有被用到
	stCD_RoleSomeEncryptInfo::MemGetter	mem_getter_;
	stCD_NetObj_Role::EncryptInfoT	encrypt_info_;	//一直等于g_o_role_encrypt_info_

	int GetValue(const stCD_NetObj_Role* role) const;
};
static_assert(offsetof(stCD_NetObj_Role, some_info_) == g_o_role_encrypt_info_, "");
static_assert(offsetof(stCD_NetObj_Role, lvl_) == g_so_role_lvl, "");
static_assert(offsetof(stCD_NetObj_Role, cur_exp_) == g_so_role_cur_exp, "");
static_assert(offsetof(stCD_NetObj_Role, cur_hp_) == g_so_role_cur_hp, "");
static_assert(offsetof(stCD_NetObj_Role, hp_) == g_so_role_hp_, "");
static_assert(offsetof(stCD_NetObj_Role, nei_shang_) == g_so_role_nei_shang_, "");
static_assert(offsetof(stCD_NetObj_Role, cur_endure_) == g_so_role_cur_endure, "");
static_assert(offsetof(stCD_NetObj_Role, endure_) == g_so_role_endure_, "");
static_assert(offsetof(stCD_NetObj_Role, zhan_wei_zhi_) == g_o_role_zhan_wei_zhi_, "");
static_assert(offsetof(stCD_NetObj_Role, gold_) == g_so_role_gold, "");
static_assert(offsetof(stCD_NetObj_Role, silver_) == g_so_role_silver, "");
static_assert(offsetof(stCD_NetObj_Role, left_fb_cnt_) == g_so_role_left_fb_cnt, "");
static_assert(offsetof(stCD_NetObj_Role, lian_qi_cao_) == g_o_role_lian_qi_cao_, "");
static_assert(offsetof(stCD_NetObj_Role, jing_suan_point_) == g_so_role_jing_suan_point, "");
static_assert(offsetof(stCD_NetObj_Role, npc_mgr_) == g_o_role_npc_mgr_, "");
static_assert(offsetof(stCD_NetObj_Role, mail_mgr_) == g_o_role_mail_mgr_, "");
static_assert(offsetof(stCD_NetObj_Role, gift_bag_) == g_o_role_gift_bag_, "");
static_assert(offsetof(stCD_NetObj_Role, week_credit_mgr_) == g_o_role_week_credit_mgr_, "");
static_assert(offsetof(stCD_NetObj_Role, enterable_fb_mgr_) == g_o_role_enterable_fb_mgr_, "");
static_assert(offsetof(stCD_NetObj_Role, done_fb_mgr_) == g_o_role_done_fb_mgr_, "");
static_assert(offsetof(stCD_NetObj_Role, poison_status_) == g_so_role_poison_state, "");
static_assert(offsetof(stCD_NetObj_Role, farm_facility_) == g_o_role_farm_facility_, "");
static_assert(offsetof(stCD_NetObj_Role, ser_line_) == g_o_role_ser_line_, "");
static_assert(offsetof(stCD_NetObj_Role, packages_mgr_) == g_o_role_packages_mgr_, "");

struct stCD_UknRoleInfo2{
	pt_pcvoid	vtbl_;
	p_mem_placeholder(g_so_role_info_3 - 4);
	stCD_NetObj_Role*	role_;		//当前角色 MMOCLIENT_REVERSAL_ACTOR_OFFSET3___
};
static_assert(offsetof(stCD_UknRoleInfo2, role_) == g_so_role_info_3, "");

struct stCD_UknRoleInfo1{
	pt_pcvoid	vtbl_;
	p_mem_placeholder(g_o_ukn_role_map_db_ - 4);
	stCD_MapDb*	cur_map_db_;			//UPDATES_MMOCLIENT_REVERSAL_ACTOR_IN_MAP_OFFFSET2
	p_mem_placeholder(g_o_role_info_2_ - 4 - g_o_ukn_role_map_db_);
	stCD_UknRoleInfo2*	ukn_role_info_;	//UPDATES_MMOCLIENT_REVERSAL_ACTOR_OFFSET2
};
static_assert(offsetof(stCD_UknRoleInfo1, cur_map_db_) == g_o_ukn_role_map_db_, "");
static_assert(offsetof(stCD_UknRoleInfo1, ukn_role_info_) == g_o_role_info_2_, "");

struct stCD_UiSystem;
struct stCD_GloRoleInfoData{
	pt_pcvoid	vtbl_;
	stCD_GloLocalIdMgr*	local_id_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_GAME_OFFSET1
	p_mem_placeholder(g_o_role_info_all_local_obj_mgr_ - 4 - g_o_GloRoleInfoData_GloLocalIdMgr_);
	stCD_AllLocalObjMgr*	all_local_obj_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_TRAVERSAL_SINGLE_OBJ_OFFSET1
	p_mem_placeholder(g_o_glo_role_info_ui_system_ - 4 - g_o_role_info_all_local_obj_mgr_);
	stCD_UiSystem*		ui_system_;		//UPDATES_MMOCLIENT_REVERSAL_MOUSE_LBUTTONDOWN_OFFSET1
	p_mem_placeholder(g_o_role_info_1_ - 4 - g_o_glo_role_info_ui_system_);
	stCD_UknRoleInfo1*	ukn_role_info_;	//UPDATES_MMOCLIENT_REVERSAL_ACTOR_OFFSET1
};
static_assert(offsetof(stCD_GloRoleInfoData, ukn_role_info_) == g_o_role_info_1_, "");
static_assert(offsetof(stCD_GloRoleInfoData, all_local_obj_mgr_) == g_o_role_info_all_local_obj_mgr_, "");
static_assert(offsetof(stCD_GloRoleInfoData, ui_system_) == g_o_glo_role_info_ui_system_, "");
static_assert(offsetof(stCD_GloRoleInfoData, local_id_mgr_) == g_o_GloRoleInfoData_GloLocalIdMgr_, "");
//////////////////////////////////////////////////////////////////////////
//物品栏种类
enum class CD_En_TabCategory{
	kTC_Package = 0,	//背包
	kTC_Task = 1,		//任务
	kTC_Store = 2,		//仓库
	kTC_Equip = 3,		//装备
	kTC_SuCai = 0xa,	//素材
	kTC_Invalid
};
p_ostm_to_num(CD_En_TabCategory)
//物品品质 // 白色=1 绿色=2 淡玫红=3 蓝色=4 深玫红=5 紫色=6 橘黄色=7 金黄色=8 其他=大红色
enum class CD_En_ItemQuality{
	kIQ_Red = 0,		//大红色
	kIQ_White,			//白色
	kIQ_Green,			//绿色
	kIQ_DanMeiRed,		//淡玫红
	kIQ_Blue,			//蓝色
	kIQ_ShenMeiRed,		//深玫红
	kIQ_Purple,			//紫色
	kIQ_JuYellow,		//橘黄色
	kIQ_JinYellow,		//金黄色
	kIQ_Invalid,		//大红色
};
p_ostm_to_num(CD_En_ItemQuality)
//物品种类
enum class CD_En_ItemCategory{
	kIT_Invalid = 0,
	kIT_Material = 1,		//消耗品、材料、工具物品等
	kIT_Equip = 2,			//装备
};
p_ostm_to_num(CD_En_ItemCategory)
//装备部位类型
enum class CD_En_ItemType{
	kEPT_Weapon = 1,	//武器
	kEPT_Defense = 2,	//防具
	kEPT_Decorate = 3,	//戒指、项链、护石
	kEPT_SuCai = 8,		//素材
	kEPT_Invalid
};
p_ostm_to_num(CD_En_ItemType)
//MMO_MATERIAL_ITEM
struct stCD_MaterialInfo{
	int material_item_id_;
	int	  item_cnt_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_MaterialInfo& v);
	void DebugInfo() const;
};
static_assert(sizeof(stCD_MaterialInfo) == 8, "MMOCLIENT_REVERSAL_MATERIAL_SIZE___");

//MMO_UP_EQUIP_ITEM_MATERIAL_INFO
struct stCD_EquipUpInfo{
	pt_pcvoid	vtbl_;
	stCD_Array<stCD_MaterialInfo>	material_info_;		//升级所需材料
	DWORD	dwUnKnow1[2];
	int		dst_item_id_;		//升级后物品ID
	DWORD	dwUnKnow2[7];

	friend std::ostream& operator << (std::ostream& ostm, const stCD_EquipUpInfo& v);
};
static_assert(sizeof(stCD_EquipUpInfo) == 0x38, "这个结构体变了，就得去看下升级武器的代码");

struct stCD_EquipInfo{
	p_mem_placeholder(g_so_equip_info_up_info);
	stCD_Array<stCD_EquipUpInfo>	up_info_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_EquipInfo& v);
};
static_assert(offsetof(stCD_EquipInfo, up_info_) == g_so_equip_info_up_info, "");
//装备部位子类型
enum class CD_En_EquipPartSubType_Weapon{
	kEPSTW_DaJian = 1,		//大剑
	kEPSTW_ChuiZi,			//锤子
	kEPSTW_ChangQiang,		//长枪
	kEPSTW_PianShou,		//片手
	kEPSTW_Nu,				//弩
	kEPSTW_ShuangDao,		//双刀
	kEPSTW_TaiDao,			//太刀
	kEPSTW_DiZi,			//笛子
	kEPSTW_Gong = 10,		//弓
	kEPSTW_Invalid,
};
p_ostm_to_num(CD_En_EquipPartSubType_Weapon)
//物品背包栏
enum class CD_En_ItemPackageCategory{
	kIPC_Item = 0,		//物品栏
	kIPC_Stored = 2,	//仓库栏
	kIPC_Task = 5,		//任务栏
};
struct stCD_ItemDb{
	p_mem_placeholder(g_so_item_db_item_id);
	int			item_id_;
	pt_csz		item_name_;
	p_mem_placeholder(g_so_item_db_item_category - 4 - g_so_item_db_name);
	CD_En_ItemCategory		category_;		// 物品类型，用去区分装备和消耗品之类
	CD_En_ItemType			item_type_;		// 部位类型，用去区分武器和防具
	CD_En_EquipPartSubType_Weapon			part_type_;		// 部位类型2,用于区分是戴在哪个部位
	int			item_lvl_;
	p_mem_placeholder(g_so_item_db_own_max - 4 - g_so_item_db_lvl);
	int			own_max_;	//拥有上限
	CD_En_ItemQuality		quality_;
	int			carry_max_;	//携带上限
	int			dui_die_max_;	//堆叠上限
	p_mem_placeholder(g_so_item_db_fen_jie - 4 - g_so_item_db_dui_die_max);
	int			fen_jie_flag_;		//用于分解
	p_mem_placeholder(g_o_item_db_role_lvl_ - 4 - g_so_item_db_fen_jie);
	int			role_lvl_;	//UPDATES_MMOCLIENT_REVERSAL_ITME_USE_LEVEL_OFFSET
	p_mem_placeholder(g_so_item_db_equip_info - 4 - g_o_item_db_role_lvl_);
	stCD_EquipInfo*	equip_info_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_ItemDb& v);
	// 获取物品的价钱  如果物品价值=0，则为不可出售，其余为出售价格
	int GetItemSellPrice() const;
};
static_assert(offsetof(stCD_ItemDb, item_id_) == g_so_item_db_item_id, "");
static_assert(offsetof(stCD_ItemDb, item_name_) == g_so_item_db_name, "");
static_assert(offsetof(stCD_ItemDb, category_) == g_so_item_db_item_category, "");
static_assert(offsetof(stCD_ItemDb, item_type_) == g_so_item_db_item_type, "");
static_assert(offsetof(stCD_ItemDb, part_type_) == g_so_item_db_part_type, "");
static_assert(offsetof(stCD_ItemDb, item_lvl_) == g_so_item_db_lvl, "");
static_assert(offsetof(stCD_ItemDb, own_max_) == g_so_item_db_own_max, "");
static_assert(offsetof(stCD_ItemDb, quality_) == g_so_item_db_quality, "");
static_assert(offsetof(stCD_ItemDb, carry_max_) == g_so_item_db_carry_max, "");
static_assert(offsetof(stCD_ItemDb, dui_die_max_) == g_so_item_db_dui_die_max, "");
static_assert(offsetof(stCD_ItemDb, fen_jie_flag_) == g_so_item_db_fen_jie, "");
static_assert(offsetof(stCD_ItemDb, role_lvl_) == g_o_item_db_role_lvl_, "");
static_assert(offsetof(stCD_ItemDb, equip_info_) == g_so_item_db_equip_info, "");

enum class CD_En_BindFlag{
	kIBT_Free = 0,		//未绑定
	kIBT_CanBind = 1,	//可绑定
	kIBT_Bind = 2,		//已绑定
	kIBT_Invalid
};
p_ostm_to_num(CD_En_BindFlag)
struct stCD_Item{
	p_mem_placeholder(g_so_item_item_info);
	stCD_ItemDb*		item_db_;
	p_mem_placeholder(g_so_item_serial - 4 - g_so_item_item_info);
	pt_dint			serial_;
	p_mem_placeholder(g_so_item_bind_flag - sizeof(pt_dint) - g_so_item_serial);
	CD_En_BindFlag		bind_flag_;	//物品绑定  未绑定=0 装备后角色绑定=1 已绑定=2
	p_mem_placeholder(g_so_item_cnt - 4 - g_so_item_bind_flag);
	int					item_cnt_;	//物品数量有的为0，当物品是装备的时候，就为0，因为装备不可堆叠

	friend std::ostream& operator << (std::ostream& ostm, const stCD_Item& v);
};
static_assert(offsetof(stCD_Item, item_db_) == g_so_item_item_info, "");
static_assert(offsetof(stCD_Item, serial_) == g_so_item_serial, "");
static_assert(offsetof(stCD_Item, bind_flag_) == g_so_item_bind_flag, "");
static_assert(offsetof(stCD_Item, item_cnt_) == g_so_item_cnt, "");

struct stCD_GridItem{
	stCD_Item*	item_;
	DWORD		dwZero1;
	DWORD		dwZero2;
};
struct stCD_GridItemCont{
	p_mem_placeholder(g_so_grid_item_cont_items);
	stCD_GridItem		grid_items_[1];
};
static_assert(offsetof(stCD_GridItemCont, grid_items_) == g_so_grid_item_cont_items, "");

struct stCD_GridsMgr{
	p_mem_placeholder(g_so_grids_mgr_max_cnt);
	int			grids_max_cnt_;			//有效格子数量
	p_mem_placeholder(g_so_grids_mgr_item_cont - 4 - g_so_grids_mgr_max_cnt);
	stCD_GridItemCont*	item_cont_;

	void DebugInfo() const;
};
static_assert(offsetof(stCD_GridsMgr, grids_max_cnt_) == g_so_grids_mgr_max_cnt, "");
static_assert(offsetof(stCD_GridsMgr, item_cont_) == g_so_grids_mgr_item_cont, "");

struct stCD_PackagesMgr{
	p_mem_placeholder(g_so_packages_mgr_package);
	stCD_GridsMgr*		grids_package_;
	p_mem_placeholder(g_so_packages_mgr_store - 4 - g_so_packages_mgr_package);
	stCD_GridsMgr*		grids_store_;
	stCD_GridsMgr*		grids_equip_;
	p_mem_placeholder(g_so_packages_mgr_task - 4 - g_so_packages_mgr_equip);
	stCD_GridsMgr*		grids_task_;
	p_mem_placeholder(g_so_packages_mgr_su_cai - 4 - g_so_packages_mgr_task);
	stCD_GridsMgr*		grids_su_cai_;

	void DebugInfo() const;
	int TidyInventoryItemSize(CD_En_TabCategory tab_cate) const;
	bool TidyInventoryItem(CD_En_TabCategory tab_cate, int size) const;
};
static_assert(offsetof(stCD_PackagesMgr, grids_package_) == g_so_packages_mgr_package, "");
static_assert(offsetof(stCD_PackagesMgr, grids_store_) == g_so_packages_mgr_store, "");
static_assert(offsetof(stCD_PackagesMgr, grids_equip_) == g_so_packages_mgr_equip, "");
static_assert(offsetof(stCD_PackagesMgr, grids_task_) == g_so_packages_mgr_task, "");
static_assert(offsetof(stCD_PackagesMgr, grids_su_cai_) == g_so_packages_mgr_su_cai, "");
//////////////////////////////////////////////////////////////////////////
//task type
enum CD_En_TaskType{
	kTT_Main = 1,			//主线任务
	kTT_Sub = 1 << 1,		//支线任务(村、城等)
	kTT_Mix = 1 << 3,		//混合的，有支线、日常、赏金、采集等
	kTT_Day = 1 << 5,		//日常
	kTT_Test = 1 << 6,		//测试用的
	kTT_Done = 1 << 9,		//完成、分解、收集等
	kTT_Shang = 1 << 10,	//赏金任务
	kTT_Invalid,
};
p_ostm_to_num(CD_En_TaskType)

struct stCD_LvlRange{
	int		min_lvl_;	//MMOCLIENT_REVERSAL_TASK_MIN_LEVEL_OFFSET___
	int		max_lvl_;	//MMOCLIENT_REVERSAL_TASK_MAX_LEVEL_OFFSET___

	friend std::ostream& operator << (std::ostream& ostm, const stCD_LvlRange& v);
	bool InRange(int lvl) const;
};
struct stCD_TaskLvlObj{
	p_mem_placeholder(g_so_task_lvl_obj_lvl_info);
	stCD_LvlRange*	role_lvl_range_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_TaskLvlObj& v);
};
static_assert(offsetof(stCD_TaskLvlObj, role_lvl_range_) == g_so_task_lvl_obj_lvl_info, "");

struct stCD_DstFbInfo{
	pt_pcvoid	vtbl_;
	p_mem_placeholder(g_so_dst_fb_info_id - 4);
	int			fb_id_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_DstFbInfo& v);
};
static_assert(offsetof(stCD_DstFbInfo, fb_id_) == g_so_dst_fb_info_id, "");
struct stCD_TaskDstFbInfo{
	p_mem_placeholder(g_so_task_dst_fb_info_fbs);
	stCD_Array<stCD_DstFbInfo*>		dst_fbs_;
	int								fb_id_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_TaskDstFbInfo& v);
};
static_assert(offsetof(stCD_TaskDstFbInfo, dst_fbs_) == g_so_task_dst_fb_info_fbs, "");
static_assert(offsetof(stCD_TaskDstFbInfo, fb_id_) == g_so_task_dst_fb_info_fb_id, "");

//MMO_TASK_REWARD_ITEM_INFO
struct stCD_RewardItem{
	pt_pcvoid		vtbl_;
	stCD_ItemDb*	item_db_;
	int				item_cnt_;
};
//MMO_TASK_REWARD_ITEM
struct stCD_TaskReward{
	pt_pcvoid	vtbl_;
	stCD_Array<stCD_RewardItem*>	reward_items_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_TaskReward& v);
};
struct stCD_TaskDb{
	pt_pcvoid	vtbl_;
	int			task_id_;
	pt_csz		task_name_;
	pt_csz		task_desc_;
	CD_En_TaskType		task_type_;
	pt_csz		task_format_str_;	//格式化字符串，做任务时会看到格式化字符串相应的提示
	p_mem_placeholder(g_so_task_db_lvl_info - 4 - g_so_task_db_format_str);
	stCD_Array<stCD_TaskLvlObj*>	lvl_info_;
	p_mem_placeholder(g_o_task_db_npc_info_ - sizeof(stCD_Array<stCD_TaskLvlObj*>) - g_so_task_db_lvl_info);
	stCD_ObjDbInfo*	accept_npc_db_;			//接任务所在NPC。UPDATES_MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET1
	stCD_ObjDbInfo*	complete_npc_db_;	//完成任务所在NPC
	p_mem_placeholder(g_so_task_db_dst_fb_info - 4 - g_so_task_db_complete_npc_info);
	stCD_Array<stCD_TaskDstFbInfo*>		dst_fb_info_;		//赏金任务下，这个字段都是有效的，主线任务时，有的有效，有的无效。
	stCD_Array<stCD_TaskReward*>		task_reward_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_TaskDb& v);
	const stCD_LvlRange* GetRoleLvlInfo() const;
};
//sizeof(stCD_TaskDb) == 0x138
static_assert(offsetof(stCD_TaskDb, task_id_) == g_so_task_db_id, "");
static_assert(offsetof(stCD_TaskDb, task_name_) == g_so_task_db_name, "");
static_assert(offsetof(stCD_TaskDb, task_desc_) == g_so_task_db_desc, "");
static_assert(offsetof(stCD_TaskDb, task_type_) == g_so_task_db_type, "");
static_assert(offsetof(stCD_TaskDb, task_format_str_) == g_so_task_db_format_str, "");
static_assert(offsetof(stCD_TaskDb, lvl_info_) == g_so_task_db_lvl_info, "");
static_assert(offsetof(stCD_TaskDb, accept_npc_db_) == g_o_task_db_npc_info_, "");
static_assert(offsetof(stCD_TaskDb, complete_npc_db_) == g_so_task_db_complete_npc_info, "");
static_assert(offsetof(stCD_TaskDb, dst_fb_info_) == g_so_task_db_dst_fb_info, "");
static_assert(offsetof(stCD_TaskDb, task_reward_) == g_so_task_db_reward_info, "");

struct stCD_TaskDbUknObj{
	p_mem_placeholder(g_o_task_db_ukn_);
	stCD_TaskDb* task_db_;	//UPDATES_MMOCLIENT_REVERSAL_TASK_OBJ_OFFSET1

	friend std::ostream& operator << (std::ostream& ostm, const stCD_TaskDbUknObj& v);
};
static_assert(offsetof(stCD_TaskDbUknObj, task_db_) == g_o_task_db_ukn_, "");

struct stCD_TaskObj{
	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_so_task_obj_task_db - 4);
	stCD_TaskDb*	task_db_;
	p_mem_placeholder(g_so_task_obj_complete_state - 4 - g_so_task_obj_task_db);
	int				complete_state_;	//完成状态
	p_mem_placeholder(g_so_task_obj_is_player_task - 4 - g_so_task_obj_complete_state);
	bool			is_player_task_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_TaskObj& v);
};
static_assert(offsetof(stCD_TaskObj, task_db_) == g_so_task_obj_task_db, "");
static_assert(offsetof(stCD_TaskObj, complete_state_) == g_so_task_obj_complete_state, "");
static_assert(offsetof(stCD_TaskObj, is_player_task_) == g_so_task_obj_is_player_task, "");

//MMO_RECEIVED_TASK_POINT
struct stCD_TaskNode : stCD_Node{
	int				id_;		//任务ID
	stCD_TaskObj*	task_;
};
struct stCD_GloReceivedTask{
	p_mem_placeholder(g_so_glo_received_task_root_node);
	stCD_TaskNode		task_root_node_;
};
static_assert(offsetof(stCD_GloReceivedTask, task_root_node_) == g_so_glo_received_task_root_node, "");

//ZHANG_TASK_FB_INFO
struct stCD_ChapterFbInfo{
	DWORD	dwUnKnow1;
	int		chapter_idx_;	// 章节序号
	pt_csz	str_chapter_;	// 章节字符串
	int		task_fb_id_;		// 任务副本id
	int		chapter_task_idx_;	// 章节内任务的序号
	stCD_Array<void*>	theZhangTaskJieSuoArray;	// 任务解锁副本对应关系
	int		x;
	int		y;
	int		task_owner_map_id_;		// 任务副本所在城镇id
	int		show_flag_;		// 是否显示的标志
	char	szEnterInstance[0x14];
	char*	pszEnterInstance;
	char	szLevelType[0x14];
	char*	pszLevelType;

	void DebugInfo() const;
};
//TASK_FB_INFO
template<typename T>
struct stCD_TaskFbInfoT{
	pt_pcvoid	vtbl_;
	DWORD		dwTaskType;
	DWORD		dwZero1;
	DWORD		dwZero2;
	DWORD		dwUnKnow1;
	stCD_Array<T>	fb_info_;
	DWORD		dwZero3[2];

	void DebugInfo() const;
};
typedef stCD_TaskFbInfoT<stCD_ChapterFbInfo> stCD_ChapterInfo;
//////////////////////////////////////////////////////////////////////////
enum class CD_En_ShopInfoCategory{
	kSIC_LieShouBuJiPin = 2,				//狩猎补给品 MMOCLIENT_SHOP_INFO_LIE_SHOU_BU_JI_PIN
	kSIC_XinShouLieShouEquip = 7,			//新手猎守装备 MMOCLIENT_SHOP_INFO_XIN_SHOU_LIE_SHOU_EQUIP
	kSIC_DanYaoBuJi = 0x0F,					//弹药补给 MMOCLIENT_SHOP_INFO_DAN_YAO_BU_JI
	kSIC_GongJianPingTeMai = 0x10,			//弓箭瓶特卖 MMOCLIENT_SHOP_INFO_GONG_JIAN_PING_TE_MAI
	kSIC_EquipRuMenXuKe = 0x22,				//猎守武器入门许可 MMOCLIENT_SHOP_INFO_RU_MEN_XU_KE
	kSIC_EquipSkill = 0x23,					//武器技能 MMOCLIENT_SHOP_INFO_EQUIP_SKILL
	kSIC_LiWei = 0x26,						//神秘的商人 MMOCLIENT_SHOP_INFO_LI_WEI
	kSIC_AiLuMaoGuYong = 5,					//艾路猫雇佣 MMOCLIENT_SHOP_INFO_AI_LU_MAO_GU_YONG
	kSIC_AiLuNongYeTeGong = 0x06,			//艾露农业特供 MMOCLIENT_SHOP_INFO_AI_LU_NONG_YE_TE_GONG
	kSIC_ZhuangYuanBiXuPin = 0x09,			//庄园必须品 MMOCLIENT_SHOP_INFO_ZHUANG_YUAN_BI_XU_PIN
	kSIC_TeChanShangPin = 0x2F,				//特产商品 MMOCLIENT_SHOP_INFO_TE_CAN_SHANG_DIAN
};
p_ostm_to_num(CD_En_ShopInfoCategory)

struct stCD_ShopInfo{
	p_mem_placeholder(g_so_shop_info_shop_id);
	int		shop_id_;
	p_mem_placeholder(g_so_shop_info_item_id - 4 - g_so_shop_info_shop_id);
	int		item_id_;
	int		item_cnt_;

	void DebugInfo() const;
};
static_assert(offsetof(stCD_ShopInfo, shop_id_) == g_so_shop_info_shop_id, "");
static_assert(offsetof(stCD_ShopInfo, item_id_) == g_so_shop_info_item_id, "");
static_assert(offsetof(stCD_ShopInfo, item_cnt_) == g_so_shop_info_item_cnt, "");
struct stCD_ShopDb{
	p_mem_placeholder(g_so_shop_db_cate_id);
	CD_En_ShopInfoCategory		cate_id_;
	p_mem_placeholder(g_so_shop_db_cate_name - 4 - g_so_shop_db_cate_id);
	pt_csz						cate_name_;		//名字，比如狩猎补给品、新手猎守装备等
	stCD_Array<stCD_ShopInfo*>	shop_info_;

	void DebugInfo() const;
};
static_assert(offsetof(stCD_ShopDb, cate_id_) == g_so_shop_db_cate_id, "");
static_assert(offsetof(stCD_ShopDb, cate_name_) == g_so_shop_db_cate_name, "");
static_assert(offsetof(stCD_ShopDb, shop_info_) == g_so_shop_db_shop_info, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_CraftDb{
	p_mem_placeholder(g_so_craft_db_craft_id);
	int		craft_id_;
	p_mem_placeholder(g_so_craft_db_dst_item_id - 4 - g_so_craft_db_craft_id);
	int		dst_item_id_;
	p_mem_placeholder(g_o_craft_db_material_info_ - 4 - g_so_craft_db_dst_item_id);
	stCD_Array<stCD_MaterialInfo>	material_info_;		//UPDATES_MMOCLIENT_REVERSAL_CRAFT_INF0_MATERIAL_OFFSET

	void DebugInfo() const;
};
static_assert(offsetof(stCD_CraftDb, craft_id_) == g_so_craft_db_craft_id, "");
static_assert(offsetof(stCD_CraftDb, dst_item_id_) == g_so_craft_db_dst_item_id, "");
static_assert(offsetof(stCD_CraftDb, material_info_) == g_o_craft_db_material_info_, "");
//////////////////////////////////////////////////////////////////////////
//MMO_MANU_MATERIAL
struct stCD_MenufactureMaterial{
	int		material_item_id_;		// 需要的材料id
	DWORD	dwUnKnow[6];
	int		item_cnt_;		// 需要的材料数量

	friend std::ostream& operator << (std::ostream& ostm, const stCD_MenufactureMaterial& v);
};
struct stCD_MenufactureDb{
	p_mem_placeholder(g_so_menufacture_db_id);
	int		id_;	//调和物品 的 id，这是所于调和发包要用到的id
	pt_csz	item_name_;
	p_mem_placeholder(g_so_menufacture_db_dst_item_id - 4 - g_so_menufacture_db_item_name);
	int		dst_item_id_;	// 调和完成的 物品id，这是制作后得到的物品id
	p_mem_placeholder(g_so_menufacture_db_meterial_info - 4 - g_so_menufacture_db_dst_item_id);
	stCD_Array<stCD_MenufactureMaterial>	material_info_;

	void DebugInfo() const;
};
static_assert(offsetof(stCD_MenufactureDb, id_) == g_so_menufacture_db_id, "");
static_assert(offsetof(stCD_MenufactureDb, item_name_) == g_so_menufacture_db_item_name, "");
static_assert(offsetof(stCD_MenufactureDb, dst_item_id_) == g_so_menufacture_db_dst_item_id, "");
static_assert(offsetof(stCD_MenufactureDb, material_info_) == g_so_menufacture_db_meterial_info, "");

struct stCD_MenufactureObj{
	stCD_MenufactureDb* FindMenufactureDb(int id) const;
};
//////////////////////////////////////////////////////////////////////////
struct stCD_GiftDb{
	pt_pcvoid	vtbl_;
	int			gift_id_;
	pt_csz		name_;
	pt_csz		desc_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_GiftDb& v);
};
static_assert(offsetof(stCD_GiftDb, gift_id_) == g_so_gift_db_id, "");
static_assert(offsetof(stCD_GiftDb, name_) == g_so_gift_db_name, "");
static_assert(offsetof(stCD_GiftDb, desc_) == g_so_gift_db_desc, "");

struct stCD_GiftObj{
	p_mem_placeholder(g_so_gift_obj_gift_db);
	stCD_GiftDb*	gift_db_;
	p_mem_placeholder(g_so_gift_obj_flag - 4 - g_so_gift_obj_gift_db);
	int				flag_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_GiftObj& v);
};
static_assert(offsetof(stCD_GiftObj, gift_db_) == g_so_gift_obj_gift_db, "");
static_assert(offsetof(stCD_GiftObj, flag_) == g_so_gift_obj_flag, "");

//MMO_GIFT_BAG_INFO
struct stCD_GiftNode : stCD_Node{
	int				gift_id_;	// 礼包的id
	stCD_GiftObj*	gift_;

	void DebugInfo() const;
};

struct stCD_GiftBag{
	p_mem_placeholder(g_so_gift_bag_root_node);
	stCD_GiftNode	gift_root_node_;
};
static_assert(offsetof(stCD_GiftBag, gift_root_node_) == g_so_gift_bag_root_node, "");
//////////////////////////////////////////////////////////////////////////
//MMO_REQUIRE_CUR_SLOT
struct stCD_GridItemInfo{
	int		slot_key_;		//key,用来标识是哪个格子
	int		item_id_;		//物品的id
	int		item_cnt_;		//物品的数量

	void DebugInfo() const;
};

//MMO_REQUIRE_SLOT
struct stCD_GridItemNode{
	stCD_GridItemNode*	next_node_;
	stCD_GridItemNode*	pre_node_;
	stCD_GridItemInfo*	cur_item_;
};

struct stCD_SupplyUknObj{
	p_mem_placeholder(g_so_supply_ukn_obj_firt_item_node);
	stCD_GridItemNode		first_item_node_;
};
static_assert(offsetof(stCD_SupplyUknObj, first_item_node_) == g_so_supply_ukn_obj_firt_item_node, "");

struct stCD_SupplyUkn1{
	p_mem_placeholder(g_so_supply_ukn1_ukn_obj);
	stCD_SupplyUknObj*		ukn_;
};
static_assert(offsetof(stCD_SupplyUkn1, ukn_) == g_so_supply_ukn1_ukn_obj, "");

struct stCD_SupplyUkn2_1{
	p_mem_placeholder(g_so_supply_ukn2_1_ukn1);
	stCD_SupplyUkn1*	ukn_;
};
static_assert(offsetof(stCD_SupplyUkn2_1, ukn_) == g_so_supply_ukn2_1_ukn1, "");

struct stCD_SupplyUkn2{
	p_mem_placeholder(g_so_supply_ukn2_ukn2_1);
	stCD_SupplyUkn2_1	ukn_;
};
static_assert(offsetof(stCD_SupplyUkn2, ukn_) == g_so_supply_ukn2_ukn2_1, "");

struct stCD_NetObjUkn{
	stCD_SupplyUkn2* GetSupplyUkn() const;
};
//////////////////////////////////////////////////////////////////////////
struct stCD_MailItem{
	p_mem_placeholder(g_so_mail_item_item_cnt);
	pt_word	item_cnt_;
	p_mem_placeholder(g_so_mail_item_item_id - 2 - g_so_mail_item_item_cnt);
	int		item_id_;
	p_mem_placeholder(g_so_mail_item_size - 4 - g_so_mail_item_item_id);

	friend std::ostream& operator << (std::ostream& ostm, const stCD_MailItem& v);
};
static_assert(offsetof(stCD_MailItem, item_cnt_) == g_so_mail_item_item_cnt, "");
static_assert(offsetof(stCD_MailItem, item_id_) == g_so_mail_item_item_id, "");
static_assert(sizeof(stCD_MailItem) == g_so_mail_item_size, "");

struct stCD_MailInfo{
	pt_dint			mail_id_;
	p_mem_placeholder(g_so_mail_info_sender - sizeof(pt_dint));
	pt_csz			sender_;
	p_mem_placeholder(g_so_mail_info_title - 4 - g_so_mail_info_sender);
	pt_csz			title_;
	p_mem_placeholder(g_so_mail_info_gold - 4 - g_so_mail_info_title);
	int				gold_;
	p_mem_placeholder(g_so_mail_info_item - 4 - g_so_mail_info_gold);
	stCD_Array<stCD_MailItem>	items_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_MailInfo& v);
};
static_assert(offsetof(stCD_MailInfo, mail_id_) == g_so_mail_info_id, "");
static_assert(offsetof(stCD_MailInfo, sender_) == g_so_mail_info_sender, "");
static_assert(offsetof(stCD_MailInfo, title_) == g_so_mail_info_title, "");
static_assert(offsetof(stCD_MailInfo, gold_) == g_so_mail_info_gold, "");
static_assert(offsetof(stCD_MailInfo, items_) == g_so_mail_info_item, "");

struct stCD_MailNode : stCD_Node{
	pt_dint			mail_id_;
	stCD_MailInfo*	mail_info_;

	void DebugInfo() const;
};

struct stCD_MailMgr{
	p_mem_placeholder(g_o_mail_mgr_mail_root_node_);
	stCD_MailNode		mail_root_node_;
};
static_assert(offsetof(stCD_MailMgr, mail_root_node_) == g_o_mail_mgr_mail_root_node_, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_BuffDb{
	p_mem_placeholder(g_so_buff_db_id);
	int		buff_id_;
	p_mem_placeholder(g_so_buff_db_name - 4 - g_so_buff_db_id);
	pt_csz	buff_name_;
	p_mem_placeholder(g_so_buff_db_desc - 4 - g_so_buff_db_name);
	pt_csz	buff_desc_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_BuffDb& v);
	void DebugInfo() const;
};
static_assert(offsetof(stCD_BuffDb, buff_id_) == g_so_buff_db_id, "");
static_assert(offsetof(stCD_BuffDb, buff_name_) == g_so_buff_db_name, "");
static_assert(offsetof(stCD_BuffDb, buff_desc_) == g_so_buff_db_desc, "");

struct stCD_Buff{
	p_mem_placeholder(g_so_buf_dst_packet_id);
	int		dst_packet_id_;			//目标packet id，表示该目标中了该buf，一般为角色的packet id
	p_mem_placeholder(g_o_buf_flag_ - 4 - g_so_buf_dst_packet_id);
	//这个字段好像不应该叫flag吧，因为当中毒时，这个字段每隔1秒增加2
	int		flag_;	//无效的buff，flag应该是-1	UPDATES_MMOCLIENT_REVERSAL_ROLE_BUFF_FLAG_OFFSET
	p_mem_placeholder(g_o_buf_buf_db_ - 4 - g_o_buf_flag_);
	stCD_BuffDb*	buf_db_;	//UPDATES_MMOCLIENT_REVERSAL_ROLE_BUFF_INFO_ADDR_OFFSET

	friend std::ostream& operator << (std::ostream& ostm, const stCD_Buff& v);
};
static_assert(offsetof(stCD_Buff, dst_packet_id_) == g_so_buf_dst_packet_id, "");
static_assert(offsetof(stCD_Buff, flag_) == g_o_buf_flag_, "");
static_assert(offsetof(stCD_Buff, buf_db_) == g_o_buf_buf_db_, "");

struct stCD_BuffNode : stCD_Node{
	int			buf_id_;
	stCD_Buff*	buf_;	//MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET3___

	void DebugInfo() const;
};

struct stCD_BuffMgr{
	p_mem_placeholder(g_so_buf_mgr_buf_root_node1);
	stCD_BuffNode		buf_root_node1_;
	stCD_BuffNode		buf_root_node2_;
};
static_assert(offsetof(stCD_BuffMgr, buf_root_node1_) == g_so_buf_mgr_buf_root_node1, "");
static_assert(offsetof(stCD_BuffMgr, buf_root_node2_) == g_so_buf_mgr_buf_root_node2, "");

struct stCD_RoleBuffMgr{
	p_mem_placeholder(g_o_role_buf_mgr_buf_mgr_);
	stCD_BuffMgr*	buf_mgr_;		//UPDATES_MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET1
};
static_assert(offsetof(stCD_RoleBuffMgr, buf_mgr_) == g_o_role_buf_mgr_buf_mgr_, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_SocketMgr3{
	bool SendMsg(stCD_MsgBase& msg) const;
};

struct stCD_SocketMgr2{
	p_mem_placeholder(g_o_socket_mgr2_mgr3_);
	stCD_SocketMgr3*	socket_mgr_;	//UPDATES_MMOCLIENT_REVERSAL_SOCKET_OFFSET3
};
static_assert(offsetof(stCD_SocketMgr2, socket_mgr_) == g_o_socket_mgr2_mgr3_, "");

struct stCD_SocketMgr1{
	p_mem_placeholder(g_o_socket_mgr1_mgr2_);
	stCD_SocketMgr2*	socket_mgr_;	//UPDATES_MMOLCINET_REVERSAL_SOCKET_OFFSET2
};
static_assert(offsetof(stCD_SocketMgr1, socket_mgr_) == g_o_socket_mgr1_mgr2_, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_ShangJinInfo{
	p_mem_placeholder(g_so_shang_jin_info_task_id);
	int				task_id_;
	p_mem_placeholder(g_so_shang_jin_task_db - 4 - g_so_shang_jin_info_task_id);
	stCD_TaskDb*	task_db_;

	void DebugInfo() const;
};
static_assert(offsetof(stCD_ShangJinInfo, task_id_) == g_so_shang_jin_info_task_id, "");
static_assert(offsetof(stCD_ShangJinInfo, task_db_) == g_so_shang_jin_task_db, "");
typedef stCD_Array<stCD_ShangJinInfo*> stCD_ShangJinArrayT;

struct stCD_ShangJinInfos{
	p_mem_placeholder(g_o_shang_jin_infos_complete_cnt_);
	int		complete_cnt_;	//UPDATES_MMOCLIENT_REVERSAL_SHANG_JIN_COUNT_COMPLE_OFFSET

	// 获取每日可完成赏金数量
	int GetTotalCnt() const;
};
static_assert(offsetof(stCD_ShangJinInfos, complete_cnt_) == g_o_shang_jin_infos_complete_cnt_, "");

struct stCD_ShangJinInfoMgr{
	bool GetShangJinArray(stCD_ShangJinArrayT& out_array) const;
	stCD_ShangJinInfos* GetShangJinInfo() const;
};
//////////////////////////////////////////////////////////////////////////
//KING_QUEST_INFO，王立副本
struct stCD_KingQuestInfo{
	DWORD dwUnKnow1;
	DWORD dwUnKnow2;
	int		king_fb_id_;
	DWORD dwUnKnow3;
	DWORD dwUnKnow4;
	int		city_id_;
	pt_csz	title_;
	char  szBuffer1[4 * 5];
	char* pszBuffer1;
	char  szBuffset2[4 * 5];
	char* pszBuffer2;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_KingQuestInfo& v);
	void DebugInfo() const;
};
enum class CD_En_KingFbType{
	kKingOneTwo = 2,	//2 王立一星和二星
	kKingThreeFour = 3,	//3 王立三星和四星
	kKingFive = 4,		//五星和大连续狩猎
	kKingSix = 6,		//六星和大连续狩猎
};

typedef stCD_TaskFbInfoT<stCD_KingQuestInfo> stCD_KingQuestFbInfo;
//////////////////////////////////////////////////////////////////////////
enum class CD_En_UseItemFlag{
	kUseItem = 3,	// 触发使用物品的标志  0x03
};
struct stCD_UseItemMgr{
	p_mem_placeholder(g_so_use_item_item_cate);
	CD_En_ItemPackageCategory	item_cate_;
	int							slot_idx_;
	p_mem_placeholder(g_so_use_item_flag  - 4 - g_so_use_item_slot_idx);
	CD_En_UseItemFlag			use_flag_;		//触发标志

	bool UseItem(CD_En_ItemPackageCategory item_cate, int slot_idx);
};
static_assert(offsetof(stCD_UseItemMgr, item_cate_) == g_so_use_item_item_cate, "");
static_assert(offsetof(stCD_UseItemMgr, slot_idx_) == g_so_use_item_slot_idx, "");
static_assert(offsetof(stCD_UseItemMgr, use_flag_) == g_so_use_item_flag, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_WeekCreditInfoImpl{
	p_mem_placeholder(g_so_week_credit_ukn_gained_cnt);
	int		gained_cnt_;		//已领取猎猎券数量

	//可领取狩猎券数量
	int GetWeekCreditCnt() const;
};
static_assert(offsetof(stCD_WeekCreditInfoImpl, gained_cnt_) == g_so_week_credit_ukn_gained_cnt, "");

struct stCD_WeekCreditInfo{
	p_mem_placeholder(g_so_week_credit_info_impl);
	stCD_WeekCreditInfoImpl*		impl_;

	stCD_WeekCreditInfo* GetCreditInfo(CD_En_WeekCreditIndex idx) const;
};
static_assert(offsetof(stCD_WeekCreditInfo, impl_) == g_so_week_credit_info_impl, "");

//周狩猎券
struct stCD_WeekCreditMgr{
	p_mem_placeholder(g_so_week_credit_credit_info);
	stCD_WeekCreditInfo		credit_mgr_;
};
static_assert(offsetof(stCD_WeekCreditMgr, credit_mgr_) == g_so_week_credit_credit_info, "");
//////////////////////////////////////////////////////////////////////////
//拍卖物品相关
struct stCD_ReqItem{
	p_mem_placeholder(g_so_req_item_item_db);
	stCD_ItemDb*		item_db_;
	p_mem_placeholder(g_so_req_item_item_cnt - 4 - g_so_req_item_item_db);
	int					item_cnt_;
};
static_assert(offsetof(stCD_ReqItem, item_db_) == g_so_req_item_item_db, "");
static_assert(offsetof(stCD_ReqItem, item_cnt_) == g_so_req_item_item_cnt, "");
//////////////////////////////////////////////////////////////////////////
enum class CD_En_MouseOper{
	kLeftBtDown = 1,	//MMOCLIENT_REVERSAL_LBUTTONDOWN
	kLeftBtUp = 2,		//MMOCLIENT_REVERSAL_LBUTTONUP
};
struct stCD_UiSystem{
	bool HandleMouseMsg(int x, int y, CD_En_MouseOper oper) const;
};
//////////////////////////////////////////////////////////////////////////
struct stCD_KeyboardMgr{
	bool PressKey(DWORD virtual_key, bool is_down) const;
};

struct stCD_DxMgr{
	stCD_KeyboardMgr*	key_mgr_;
};
static_assert(offsetof(stCD_DxMgr, key_mgr_) == g_so_dx_mgr_key_mgr, "");

struct stCD_GloDxMgr{
	p_mem_placeholder(g_so_glo_dx_mgr_dx_mgr);
	stCD_DxMgr*		dx_mgr_;
};
static_assert(offsetof(stCD_GloDxMgr, dx_mgr_) == g_so_glo_dx_mgr_dx_mgr, "");
//////////////////////////////////////////////////////////////////////////
//比如门
struct stCD_LocalObjStatic : stCD_ObjBase{
	
	p_mem_placeholder(g_so_local_obj_static_room_num - sizeof(stCD_ObjBase));
	int		room_num_;
};
static_assert(offsetof(stCD_LocalObjStatic, room_num_) == g_so_local_obj_static_room_num, "");
//////////////////////////////////////////////////////////////////////////
//UI相关
struct stCD_UiCtrlClassNameInfo{
	p_mem_placeholder(g_so_ui_ctrl_class_name_info_name);
	pt_csz*		name_;
};
static_assert(offsetof(stCD_UiCtrlClassNameInfo, name_) == g_so_ui_ctrl_class_name_info_name, "");
enum class CD_En_UiCtrlFlag{
	kVisible = 1 << 0xe,
};
enum class CD_En_UiCtrlMsg{
	kMouseMove = 0x8,
	kLeftBtDown = 0x10,
	kLeftBtUp = 0x0100000C,
};
struct stCD_UiCtrlMsg{
	CD_En_UiCtrlMsg		type_;
	DWORD dwTmp2;
	DWORD dwTmp4;
	DWORD dwTmp5;
	DWORD dwTmp6;
	DWORD dwTmp7;

	stCD_UiCtrlMsg();
};
struct stCD_UiCtrl{
	p_mem_placeholder(g_so_ui_ctrl_parent_idx);
	int				parent_idx_;
	p_mem_placeholder(g_so_ui_ctrl_parent - 4 - g_so_ui_ctrl_parent_idx);
	stCD_UiCtrl*	parent_;
	p_mem_placeholder(g_so_ui_ctrl_flag - 4 - g_so_ui_ctrl_parent);
	pt_word			flag_;	//CD_En_UiCtrlFlag
	p_mem_placeholder(g_so_ui_ctrl_class_name_info - 2 - g_so_ui_ctrl_flag);
	stCD_UiCtrlClassNameInfo*	name_info_;
	p_mem_placeholder(g_so_ui_ctrl_next - 4 - g_so_ui_ctrl_class_name_info);
	stCD_UiCtrl*	next_;

	p_define_cast
	bool IsVisible() const;
	pt_csz GetClsName() const;
	int	GetClassName(pt_sz name_buffer, int max_buffer) const;
	int GetFullClassName(pt_sz name_buffer, int max_buffer) const;
	bool MatchTheFullClassName(const pt_csz cls_name[], int cnt) const;
	bool HandleMsg(const stCD_UiCtrlMsg& msg) const;
	bool HandleSomeMsg(const stCD_UiCtrlMsg* msg, int cnt) const;

private:
	int GetFullClassNameImpl(pt_sz name_buffer, int max_buffer) const;
};
static_assert(offsetof(stCD_UiCtrl, parent_idx_) == g_so_ui_ctrl_parent_idx, "");
static_assert(offsetof(stCD_UiCtrl, parent_) == g_so_ui_ctrl_parent, "");
static_assert(offsetof(stCD_UiCtrl, flag_) == g_so_ui_ctrl_flag, "");
static_assert(offsetof(stCD_UiCtrl, name_info_) == g_so_ui_ctrl_class_name_info, "");
static_assert(offsetof(stCD_UiCtrl, next_) == g_so_ui_ctrl_next, "");

struct stCD_UiCtrlEdit_StrMgr{
	bool ClearString() const;
};
struct stCD_UiCtrlEditUkn{
	p_mem_placeholder(g_so_ui_ctrl_str_mgr);
	stCD_UiCtrlEdit_StrMgr*		str_mgr_;
};
static_assert(offsetof(stCD_UiCtrlEditUkn, str_mgr_) == g_so_ui_ctrl_str_mgr, "");
struct stCD_UiCtrlEdit : stCD_UiCtrl{
	p_mem_placeholder(g_so_ui_ctrl_edit_edit_ukn - sizeof(stCD_UiCtrl));
	stCD_UiCtrlEditUkn*		ukn_;
	p_mem_placeholder(g_so_ui_ctrl_edit_focus - 4 - g_so_ui_ctrl_edit_edit_ukn);
	int						focus_;

	bool PutChar(pt_wchar c) const;
	bool PutString(pt_cwsz str) const;
};
static_assert(offsetof(stCD_UiCtrlEdit, ukn_) == g_so_ui_ctrl_edit_edit_ukn, "");
static_assert(offsetof(stCD_UiCtrlEdit, focus_) == g_so_ui_ctrl_edit_focus, "");

struct stCD_UiMgr{
	p_mem_placeholder(g_o_ui_mgr_);
	stCD_UiCtrl*		ui_ctrl_;		//UPDATES_REVERSAL_UI_OFFSET6
};
static_assert(offsetof(stCD_UiMgr, ui_ctrl_) == g_o_ui_mgr_, "");
struct stCD_UiMgr4{
	p_mem_placeholder(g_so_ui_mgr5);
	stCD_UiMgr*			ui_mgr_;
};
static_assert(offsetof(stCD_UiMgr4, ui_mgr_) == g_so_ui_mgr5, "");
struct stCD_UiMgr3{
	p_mem_placeholder(g_so_ui_mgr4);
	stCD_UiMgr4*		ui_mgr_;
};
static_assert(offsetof(stCD_UiMgr3, ui_mgr_) == g_so_ui_mgr4, "");
struct stCD_UiMgr2{
	p_mem_placeholder(g_so_ui_mgr3);
	stCD_UiMgr3*		ui_mgr_;
};
static_assert(offsetof(stCD_UiMgr2, ui_mgr_) == g_so_ui_mgr3, "");

struct stCD_UiMgr1Ukn{
	p_mem_placeholder(g_so_ui_mgr2_ukn);
	stCD_UiMgr2*		ui_mgr_;
};
static_assert(offsetof(stCD_UiMgr1Ukn, ui_mgr_) == g_so_ui_mgr2_ukn, "");
struct stCD_UiMgr1{
	p_mem_placeholder(g_so_ui_mgr2);
	stCD_UiMgr1Ukn		ukn_;
};
static_assert(offsetof(stCD_UiMgr1, ukn_) == g_so_ui_mgr2, "");
struct stCD_GloUiMgr{
	p_mem_placeholder(g_so_ui_mgr1);
	stCD_UiMgr1*		ui_mgr_;
};
static_assert(offsetof(stCD_GloUiMgr, ui_mgr_) == g_so_ui_mgr1, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_PopDlgInfo{
	p_mem_placeholder(g_so_pop_dlg_info_str_info);
	pt_csz		str_info_;
};
static_assert(offsetof(stCD_PopDlgInfo, str_info_) == g_so_pop_dlg_info_str_info, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_JuQingInfo{
	p_mem_placeholder(g_so_juqing_info_str_info);
	pt_csz		str_info_;
};
static_assert(offsetof(stCD_JuQingInfo, str_info_) == g_so_juqing_info_str_info, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_HittedObjInfo{
	p_mem_placeholder(g_so_hitted_obj_info_net_obj);
	stCD_NetObj*		net_obj_;
};
static_assert(offsetof(stCD_HittedObjInfo, net_obj_) == g_so_hitted_obj_info_net_obj, "");

struct stCD_HitParamInfo{
	int		cur_hp_;
};
//////////////////////////////////////////////////////////////////////////
//攻击部位的结构体 MMO_BU_WEI_OBJ_STRUCT
struct stCD_BodyPartInfo{
	pt_byte			bUnKnowFloat[0x20];		// 以怪物的头为前，分左右
	stCD_Vector3	theRangeRight;	// 怪物部位右边
	stCD_Vector3	theRangeLeft;	// 怪物部位左边
	DWORD			dwUnKnow1;
	DWORD			dwUnKnow2;
	int				dwBuWeiId;		// 部位  $+40     55CDDA58     00000009
	pt_byte			bUnKnowCode[0x34];
};

struct stCD_BodyPartObj{
	p_mem_placeholder(g_so_body_part_obj_obj_base);
	stCD_ObjBase*		obj_info_;
	p_mem_placeholder(g_so_body_part_obj_part_info - 4 - g_so_body_part_obj_obj_base);
	stCD_BodyPartInfo*	body_part_info_array_;
	int					body_part_cnt_;
};
static_assert(offsetof(stCD_BodyPartObj, obj_info_) == g_so_body_part_obj_obj_base, "");
static_assert(offsetof(stCD_BodyPartObj, body_part_info_array_) == g_so_body_part_obj_part_info, "");
static_assert(offsetof(stCD_BodyPartObj, body_part_cnt_) == g_so_body_part_obj_part_info_cnt, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_MonsterHitInfo{
	int			hit_obj_id_;		//攻击者的本地ID
	int			hitted_obj_id_;		//被攻击者的本地ID
};

struct stCD_MonsterRemoteHitInfo{
	p_mem_placeholder(g_so_monster_remote_hit_info_hiited_obj);
	stCD_ObjBase*		hitted_obj_;		//被攻击者对象
};
static_assert(offsetof(stCD_MonsterRemoteHitInfo, hitted_obj_) == g_so_monster_remote_hit_info_hiited_obj, "");

struct stCD_MonsterRemoteHitObj{
	p_mem_placeholder(g_so_monster_remote_hit_obj_hit_obj_id);
	int			hit_obj_id_;		//攻击者的本地ID
	p_mem_placeholder(g_so_monster_remote_hit_obj_hit_flag - 4 - g_so_monster_remote_hit_obj_hit_obj_id);
	bool		ukn_flag_;			//把它置为true，则可以远程无敌
};
static_assert(offsetof(stCD_MonsterRemoteHitObj, hit_obj_id_) == g_so_monster_remote_hit_obj_hit_obj_id, "");
static_assert(offsetof(stCD_MonsterRemoteHitObj, ukn_flag_) == g_so_monster_remote_hit_obj_hit_flag, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_UiMng{
	p_mem_placeholder(g_so_ui_mng_name);
	pt_csz	name_;

	p_define_cast
	friend std::ostream& operator << (std::ostream& ostm, const stCD_UiMng& v);
};
static_assert(offsetof(stCD_UiMng, name_) == g_so_ui_mng_name, "");

struct stCD_UiMng_Minimap : stCD_UiMng{
	p_mem_placeholder(g_so_ui_mng_minimap_room_num - sizeof(stCD_UiMng));
	int		room_num_;

	friend std::ostream& operator << (std::ostream& ostm, const stCD_UiMng_Minimap& v);
};
static_assert(offsetof(stCD_UiMng_Minimap, room_num_) == g_so_ui_mng_minimap_room_num, "");

struct stCD_UiGlobalMng{
	stCD_UiGlobalMng*		next_;
	p_mem_placeholder(g_so_ui_global_mng_ui_mng - 4);
	stCD_UiMng*		ui_mng_;
};
static_assert(offsetof(stCD_UiGlobalMng, ui_mng_) == g_so_ui_global_mng_ui_mng, "");
//////////////////////////////////////////////////////////////////////////
struct stCD_Common_ThrdStart{
	stCD_Vtbl_Common_ThrdStart*		vtbl_;
	p_mem_placeholder(0x1c - sizeof(pt_pcvoid));
	bool		ukn_flag_;		//线程启动时置1
	bool		is_runing_;		//线程启动时置1，结束前置0
};
static_assert(offsetof(stCD_Common_ThrdStart, ukn_flag_) == 0x1c, "");
static_assert(offsetof(stCD_Common_ThrdStart, is_runing_) == 0x1d, "");

struct stCD_MhoClient_ThrdStart1{
	int			ukn_1_;
	int			ukn_flag1_;		//会比较是否==1
	int			ukn_3_;
	int			ukn_value_;
	pt_pcvoid	ukn_;
	p_mem_placeholder(0x30 - 4 - 0x10);
	pt_pcvoid	ukn_fn_;		//这是个函数
	p_mem_placeholder(8);
	int			flag_;			//线程启动时置1
	int			ukn_flag_;		//线程要结束时会判断是否为0
};
static_assert(offsetof(stCD_MhoClient_ThrdStart1, ukn_value_) == 0xc, "");
static_assert(offsetof(stCD_MhoClient_ThrdStart1, ukn_) == 0x10, "");
static_assert(offsetof(stCD_MhoClient_ThrdStart1, ukn_fn_) == 0x30, "");
static_assert(offsetof(stCD_MhoClient_ThrdStart1, flag_) == 0x3c, "");
static_assert(offsetof(stCD_MhoClient_ThrdStart1, ukn_flag_) == 0x40, "");

struct stCD_MhoClient_ThrdStart2{
	p_mem_placeholder(0x18);
	int			ukn_value_;
	p_mem_placeholder(0x24 - 4 - 0x18);
	int			ukn_flag_;		//-1是无效值
	int			some_value_;	//类似于消息ID，根据值的不同执行不同的代码。
	int			ukn_;
};
static_assert(offsetof(stCD_MhoClient_ThrdStart2, ukn_value_) == 0x18, "");
static_assert(offsetof(stCD_MhoClient_ThrdStart2, ukn_flag_) == 0x24, "");
static_assert(offsetof(stCD_MhoClient_ThrdStart2, some_value_) == 0x28, "");
static_assert(offsetof(stCD_MhoClient_ThrdStart2, ukn_) == 0x2c, "");

//这个类非常大，起码有0x1904C。
struct stCD_MhoClient_ThrdStart3{
	p_mem_placeholder(0x18);
	int		ukn_flag_;		//==1或0吧
	p_mem_placeholder(0x38 - 4 - 0x18);
	pt_pcvoid	ukn_obj_;	//对象，有虚函数
};
static_assert(offsetof(stCD_MhoClient_ThrdStart3, ukn_flag_) == 0x18, "");
static_assert(offsetof(stCD_MhoClient_ThrdStart3, ukn_obj_) == 0x38, "");

struct stCD_CryGame_ThrdStart{
	stCD_Vtbl_CryGame_ThrdStart*	vtbl_;
};
//////////////////////////////////////////////////////////////////////////
template<CD_En_LocalObjType ObjType_>
struct stCD_DerivedConverter_ObjBase{
	static stCD_ObjBase* get(stCD_ObjBase* obj){
		assert(!IsUnValidPtr(obj));
		return obj;
	}
};
template<>
struct stCD_DerivedConverter_ObjBase<CD_En_LocalObjType::kDoor1>{
	static stCD_LocalObjStatic* get(stCD_ObjBase* obj){
		assert(!IsUnValidPtr(obj));
		return obj->CastTo<stCD_LocalObjStatic>();
	}
};
template<>
struct stCD_DerivedConverter_ObjBase<CD_En_LocalObjType::kDoor2>{
	static stCD_LocalObjStatic* get(stCD_ObjBase* obj){
		assert(!IsUnValidPtr(obj));
		return obj->CastTo<stCD_LocalObjStatic>();
	}
};

template<CD_En_LocalObjType ObjType_>
struct stCD_DerivedConverter_NetObj{
	static stCD_NetObj* get(stCD_NetObj* obj){
		assert(!IsUnValidPtr(obj));
		return obj;
	}
};
template<>
struct stCD_DerivedConverter_NetObj<CD_En_LocalObjType::kNpc>{
	static stCD_NetObj_Npc* get(stCD_NetObj* obj){
		assert(!IsUnValidPtr(obj));
		return obj->CastTo<stCD_NetObj_Npc>();
	}
};
template<>
struct stCD_DerivedConverter_NetObj<CD_En_LocalObjType::kCollect>{
	static stCD_NetObj_Collect* get(stCD_NetObj* obj){
		assert(!IsUnValidPtr(obj));
		return obj->CastTo<stCD_NetObj_Collect>();
	}
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

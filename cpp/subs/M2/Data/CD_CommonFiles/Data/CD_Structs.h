#pragma once
/*
@author zhp
@date 2017/1/24 17:22
*/
#include <ProtocolX/P_DefinesX.h>
#include <ProtocolX/P_DefinesX.h>
#include <cstddef>
#include "CD_Vtbl.h"
#include <array>
#include <string>
#include "CD_Serialize.hpp"
#include "CD_Updates.h"
#include "CD_StructBase.h"
#include <WgFacility/CmnFilter.h>
#include <StdEx/XstddefEx.h>
#include <CommonX/CmnX_StaticConts.h>
#include <StdEx/AlgorithmEx.h>

#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
struct stCD_Monster;
struct stCD_Role;
struct stCD_GameObjBase;
struct stCD_GameObject;
struct stCD_Npc;
struct stCD_DropItem;
struct stCD_PortalAction;
struct stCD_TalkAction;
struct stCD_InteractAction;
struct stCD_Portal;
struct stCD_ModelBase;
struct stCD_Model;
//////////////////////////////////////////////////////////////////////////
struct stCD_OutPacket{
	struct stBuffer{
		pt_pcbyte		buffer_;			//默认calloc的长度为0x100
		pt_dword		buffer_size1_;
		pt_dword		buffer_size_;
		pt_dword		ukn_dw2_;
	};
	struct stUkn{};

	stCD_OutPacket(pt_dword msg_id);
	~stCD_OutPacket();
	//只需要调用这个发包函数即可
	bool Send() const;
	//////////////////////////////////////////////////////////////////////////
	virtual void Ukn() = 0;							//$0,好像一直没有被调用
	virtual void PutByte(pt_byte) = 0;				//$4
	virtual void PutWord(pt_word) = 0;				//$8
	virtual void PutDword(pt_dword) = 0;			//$c
	//竟然没转成网络字节？！
	virtual void PutQword(pt_qword) = 0;			//$10
	virtual void PutFloat(pt_float) = 0;			//$14
	//函数内会先转换成宽字符串，以宽字符串的形式发送出去
	virtual void PutWStr(pt_csz) = 0;				//$18
	//strlen(str)的大小为pt_word，接下来是字符串，不包括后面的0
	virtual void PutStr(pt_csz str) = 0;			//$1c
	//strlen(str)的大小为pt_word，接下来是字符串，不包括后面的0
	virtual void PutWStr(pt_cwsz str) = 0;			//$20
	virtual void PutBuffer(pt_pcvoid buffer, pt_dword buffer_size) = 0;		//$24
	//函数内是调用的PutFloat
	virtual void PutVec3(const stCD_Vec3Float&) = 0;						//$28
	//函数内是调用的PutWord，(pt_word)vec.x
	virtual void PutVec3Word(const stCD_Vec3Float& vec) = 0;				//$2c
	//////////////////////////////////////////////////////////////////////////

	stUkn*					ukn_ptr_;
	stBuffer				buffer_;
	pt_dword				cur_msg_size_;
	pt_dword				ukn_dw_;	//发包前=0,发完包后，这里就有值了
	p_mem_placeholder(0x100);			//防止出错
};
static_assert(0x8 == offsetof(stCD_OutPacket, buffer_), "");
static_assert(g_so_out_packet_msg_id_ == offsetof(stCD_OutPacket, cur_msg_size_), "");

struct stCD_SocketThread{};

struct stCD_CNClientPlatform{};

struct stCD_NetworkService{};

struct stCD_GameSocket{
	bool Send(const stCD_OutPacket& packet) const;

	pt_pcvoid					vtbl_;					//CGameSocket
	pt_pcvoid					vtbl1_;					//CSimpleSocket
	pt_dword					ukn_dw_;
	pt_dword					ukn_dw1_;
	stCD_SocketThread*			ptr_socket_thread_;		//CSocketThread,$10
	p_mem_placeholder(4);		//pt_pcvoid
	stCD_LockCriticalSection	locker_;				//$18
	stCD_LockCriticalSection	locker1_;
	pt_pdword					ukn_flag_;				//$50,=1才能发包成功
};
static_assert(0x18 == offsetof(stCD_GameSocket, locker_), "");
static_assert(0x50 == offsetof(stCD_GameSocket, ukn_flag_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_ItemBase{
	void DebugInfo() const;

	pt_pcvoid			vtbl_;
	p_mem_placeholder(g_so_item_id_ - 4);
	pt_qword			item_id_;
	p_mem_placeholder(g_so_item_idx_ - 8 - g_so_item_id_);
	pt_dword			item_idx_;
	p_mem_placeholder(4);		//=-1
	pt_dword			color_;
	p_mem_placeholder(4);		//pt_dword
	pt_csz				item_type_;			//这个参数穿装备需要的，发包需要
	pt_dword			item_cnt_;
	p_mem_placeholder(g_so_item2db_item_ - 4 - g_so_item_cnt_);
	stCD_Data_Item*	db_item_;
};
static_assert(g_so_item_id_ == offsetof(stCD_ItemBase, item_id_), "");
static_assert(g_so_item_idx_ == offsetof(stCD_ItemBase, item_idx_), "");
static_assert(g_so_item_color_ == offsetof(stCD_ItemBase, color_), "");
static_assert(g_so_item_cnt_ == offsetof(stCD_ItemBase, item_cnt_), "");
static_assert(g_so_item2db_item_ == offsetof(stCD_ItemBase, db_item_), "");

struct stCD_InventoryItemInfo{
	pt_dword			ukn_dw_;		//=0
	stCD_ItemBase*		item_;
};
static_assert(0x8 == sizeof(stCD_InventoryItemInfo), "");

struct stCD_InventoryTabPage{
	typedef std::function<bool(stCD_ItemBase*)> Fn_TraItem;

	void DebugInfo() const;
	int TraItems(const Fn_TraItem& fn) const;

	stCD_InventoryItemInfo*		items_;
	pt_dword					max_cnt_;
	pt_dword					max_cnt1_;		//跟上面的值一样，以上面的为准
	pt_dword					ukn_dw_;		//=0
};
static_assert(0x10 == sizeof(stCD_InventoryTabPage), "");

struct stCD_BagMgr{
	typedef std::function<bool(enCD_InventoryType, stCD_InventoryTabPage*)> Fn_TraInventory;

	static stCD_BagMgr* GetInstance();
	void DebugInfo(enCD_InventoryType inventory_type) const;
	int TraInventory(const Fn_TraInventory& fn) const;

	pt_pcvoid				vtbl_;
	p_mem_placeholder(g_so_bag_mgr2inventory_info_ - 4);
	stCD_InventoryTabPage	inventory_[kIT_Max];
};
static_assert(g_so_bag_mgr2inventory_info_ == offsetof(stCD_BagMgr, inventory_), "");

struct stCD_EquipItem{
	void DebugInfo() const;

	pt_pcvoid			vtbl_;
	p_mem_placeholder(g_so_equip_item_item_ - 4);
	stCD_ItemBase*		item_;
};
static_assert(g_so_equip_item_item_ == offsetof(stCD_EquipItem, item_), "");

struct stCD_EquipmentSlot{
	void DebugInfo() const;

	pt_pcvoid			vtbl_;
	p_mem_placeholder(g_so_equip_slot_item_ - 4);
	stCD_EquipItem*		equip_item_;
};
static_assert(g_so_equip_slot_item_ == offsetof(stCD_EquipmentSlot, equip_item_), "");

struct stCD_BodyItemNode{
	void DebugInfo() const;

	enCD_BodyEquipType		type_;
	p_mem_placeholder(4);
	stCD_EquipmentSlot*		equip_slot_;
};

struct stCD_BodyEquipMgr{
	typedef stCD_XTree<stCD_BodyItemNode> BodyItemCont;

	void DebugInfo() const;

	pt_dword			ukn_dw_;
	p_mem_placeholder(0x8);
	pt_pcvoid			ukn_ptr_;
	BodyItemCont		body_equips_;
};
static_assert(0x10 == offsetof(stCD_BodyEquipMgr, body_equips_), "");

struct stCD_MailInfo{
	void DebugInfo() const;

	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_so_mail_info_sender_ - 4);
	pt_cwsz			sender_;
	p_mem_placeholder(g_so_mail_title_ - g_so_mail_info_sender_ - 4);
	pt_cwsz			title_;
	pt_cwsz			content_;
};

struct stCD_MailTypeInfo{
	void DebugInfo() const;

	pt_dword			mail_type_;		//猜的。=0表示系统邮件？
	stCD_MailInfo*		mail_info_;
};
static_assert(8 == sizeof(stCD_MailTypeInfo), "");

struct stCD_MailMgr{
	typedef stCD_Vector<stCD_MailTypeInfo> MailInfoCont;

	stCD_MailMgr() = delete;
	stCD_MailMgr(const stCD_MailMgr&) = delete;
	stCD_MailMgr& operator = (const stCD_MailMgr&) = delete;
	static stCD_MailMgr& GetMe();
	void DebugInfo() const;

	p_mem_placeholder(g_so_mail_info_cont_);
	MailInfoCont		mails_;
};
static_assert(g_so_mail_info_cont_ == offsetof(stCD_MailMgr, mails_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_Skill{
	void DebugInfo() const;
	pt_cwsz GetSkillName() const;

	pt_pcvoid			vtbl_;
	pt_dword			ukn_dw_;
	p_mem_placeholder(4 * 2);
	bool				is_active_;
	p_mem_placeholder(3);
	pt_dword			skill_name_xmlid_;
	pt_word				skill_lvl_;		//这个等级有可能是byte类型
	p_mem_placeholder(2);
};
static_assert(0x10 == offsetof(stCD_Skill, is_active_), "");
static_assert(0x18 == offsetof(stCD_Skill, skill_lvl_), "");

struct stCD_SkillInfo{
	pt_pcvoid		ukn_ptr_;
	pt_dword		ukn_dw_;		//=0
	stCD_Skill*		skill_;
};

struct stCD_SkillsInfo{
	typedef stCD_Vector<stCD_SkillInfo*> SkillCont;

	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_so_skills_info2skills_ - 4);
	SkillCont		skills_;
};
static_assert(g_so_skills_info2skills_ == offsetof(stCD_SkillsInfo, skills_), "");

struct stCD_SkillInfoMgr{
	p_mem_placeholder(4 * 3);
	stCD_SkillsInfo*		skills_;
};
static_assert(0xc == offsetof(stCD_SkillInfoMgr, skills_), "");

struct stCD_SkillMgr{
	typedef std::function<bool(stCD_Skill*)> Fn_TraSkill;

	void DebugInfo() const;
	int TraSkill(const Fn_TraSkill& fn) const;

	pt_pcvoid			ukn_ptr_;
	pt_pcvoid			vtbl_;
	p_mem_placeholder(4);		//pt_pcvoid
	p_mem_placeholder(4);		//=-1
	p_mem_placeholder(4 * 3);	//=0
	p_mem_placeholder(4);		//pt_dword
	stCD_SkillInfoMgr*	skill_info_;
	stCD_SkillInfoMgr*	skill_info1_;	//跟上面的值一样
};
static_assert(0x20 == offsetof(stCD_SkillMgr, skill_info_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_StateMgr{
	p_mem_placeholder(0x10);
	enCD_ObjState		obj_state_;		//角色状态

	void DebugInfo() const;
};
static_assert(0x10 == offsetof(stCD_StateMgr, obj_state_), "");

struct stCD_GameObjectControllerHitReport{
	pt_pcvoid			vtbl_;
	p_mem_placeholder(4 * 5);
	stCD_Model*			model_;			//人物或对象的方块对象
	p_mem_placeholder(4);		//=0
	stCD_GameObjBase*	owner_;
};
static_assert(0x18 == offsetof(stCD_GameObjectControllerHitReport, model_), "");

struct stCD_ModelControl{
	p_mem_placeholder(4 * 4);		//pt_pcvoid
	stCD_GameObjectControllerHitReport		hit_control_;
};

/*
还有个瞬移的数据未更新，到时候再说吧。
*/
struct stCD_GameObjBase{
	CMN_CAST_TO
	void DebugInfo() const;
	void SmartDbgInfo() const;
	//////////////////////////////////////////////////////////////////////////
	stCD_GameObject* CastToGameObject() const;
	stCD_DropItem* CastToDropItem() const;
	//////////////////////////////////////////////////////////////////////////

	pt_pcvoid					vtbl_;
	pt_pcvoid					vtbl1_;
	pt_dword					ukn_type_;
	p_mem_placeholder(4);		//pt_pcvoid
	p_mem_placeholder(4 * 3);	//=0
	p_mem_placeholder(4);		//pt_float
	stCD_Vec3Float				pos_;				//$20
	p_mem_placeholder(4 * 2);
	/*
	right		45
	up			134(约等于135=90+45)
	left		224(up+90)
	down		315(约等于left+90=225+90)
	down+right	0
	up+right	90
	up+left		179(约等于180)
	down+left	270
	*/
	float						dir_angle_;			//$34，朝向的角度，0到360度之间。
	p_mem_placeholder(4 * 6);
	pt_pcvoid					ukn_vtbl_;			//$50
	p_mem_placeholder(4);		//pt_pcvoid
	pt_dword					obj_id_;			//$58
	p_mem_placeholder(4 * 2);
	pt_csz						obj_type_name_;		//$64,"GameObject"
};
static_assert(g_so_game_obj_pos_ == offsetof(stCD_GameObjBase, pos_), "");
static_assert(g_so_game_obj_dir_angle_ == offsetof(stCD_GameObjBase, dir_angle_), "");

struct stCD_DropItem : stCD_GameObjBase{
	void DebugInfo() const;
	void SmartDbgInfo() const;

	p_mem_placeholder(g_o_drop_item_obj2item_ - sizeof(stCD_GameObjBase));
	stCD_ItemBase*			item_;
};
static_assert(g_o_drop_item_obj2item_ == offsetof(stCD_DropItem, item_), "");

typedef pt_dint(stCD_ObjAttrValues)[kRAVT_Max];
struct stCD_GameObject : stCD_GameObjBase{
	void DebugInfo() const;
	void SmartDbgInfo() const;
	static void DebugObjAttrInfo(const stCD_ObjAttrValues& attr_values);
	//////////////////////////////////////////////////////////////////////////
	stCD_Monster* CastToMonster() const;
	stCD_Npc* CastToNpc() const;
	stCD_Role* CastToRole() const;
	//////////////////////////////////////////////////////////////////////////

	p_mem_placeholder(g_so_game_obj_ukn_vector_ - sizeof(stCD_GameObjBase));
	stCD_UknGameVector			ukn_vector_;
	stCD_StateMgr*				state_mgr_;
	stCD_ModelControl*			model_control_;
	p_mem_placeholder(g_o_role_max_attr_value_ - g_o_model_control_ - 4);
	stCD_ObjAttrValues			max_attr_values_;
	p_mem_placeholder(g_o_role_cut_attr_value_ - g_o_role_max_attr_value_ - sizeof(stCD_ObjAttrValues));
	stCD_ObjAttrValues			cur_attr_values_;
	p_mem_placeholder(g_o_role_name_ - g_o_role_cut_attr_value_ - sizeof(stCD_ObjAttrValues));
	pt_cwsz						obj_name_;
	p_mem_placeholder(g_o_game_obj_cube_pos_ - g_o_role_name_ - 4);
	stCD_Vec3Float				cube_pos_;			//人物方块坐标
	p_mem_placeholder(g_o_get_tbl_item_game_obj_ - g_o_game_obj_cube_pos_ - sizeof(stCD_Vec3Float));
	stCD_Data_GameObject*		db_game_obj_;
};
static_assert(g_so_game_obj_ukn_vector_ == offsetof(stCD_GameObject, ukn_vector_), "");
static_assert(g_so_game_obj_state_info_ == offsetof(stCD_GameObject, state_mgr_), "");
static_assert(0xe4 == offsetof(stCD_GameObject, state_mgr_), "");
static_assert(g_o_model_control_ == offsetof(stCD_GameObject, model_control_), "");
static_assert(g_o_role_max_attr_value_ == offsetof(stCD_GameObject, max_attr_values_), "");
static_assert(g_o_role_cut_attr_value_ == offsetof(stCD_GameObject, cur_attr_values_), "");
static_assert(g_o_role_name_ == offsetof(stCD_GameObject, obj_name_), "");
static_assert(g_o_game_obj_cube_pos_ == offsetof(stCD_GameObject, cube_pos_), "");
static_assert(g_o_get_tbl_item_game_obj_ == offsetof(stCD_GameObject, db_game_obj_), "");

struct stCD_Monster : stCD_GameObject{
	void DebugInfo() const;
};

struct stCD_Npc : stCD_GameObject{
	void DebugInfo() const;
};
//////////////////////////////////////////////////////////////////////////
struct stCD_ActionBase{
	CMN_CAST_TO
	void DebugInfo() const;
	void SmartDbgInfo() const;
	stCD_PortalAction* CastToPortalAction() const;
	stCD_TalkAction* CastToTalkAction() const;
	stCD_InteractAction* CastToInteractAction() const;

	pt_pcvoid			vtbl_;
};

struct stCD_PortalAction : stCD_ActionBase{
	void DebugInfo() const;

	p_mem_placeholder(0x18 - sizeof(stCD_ActionBase));
	stCD_Portal*		portal_;
};
static_assert(0x18 == offsetof(stCD_PortalAction, portal_), "");

struct stCD_TalkAction : stCD_ActionBase{
	void DebugInfo() const;

	p_mem_placeholder(0x10 - sizeof(stCD_ActionBase));
	pt_cwsz				npc_name_;
};
static_assert(0x10 == offsetof(stCD_TalkAction, npc_name_), "");

struct stCD_InteractAction : stCD_ActionBase{
	void DebugInfo() const;

	p_mem_placeholder(0xc - sizeof(stCD_ActionBase));
	pt_csz				dst_name_;
};
static_assert(0xc == offsetof(stCD_InteractAction, dst_name_), "");

struct stCD_ActionNode{
	void DebugInfo() const;

	pt_dword			action_id_;
	p_mem_placeholder(4);
	stCD_ActionBase*	action_;
};
static_assert(0xc == sizeof(stCD_ActionNode), "");

struct stCD_ActionMgr{
	typedef stCD_XTree<stCD_ActionNode> ActionCont;
	typedef std::function<bool(const stCD_ActionNode&)> Fn_TraAction;

	void DebugAction() const;
	int TraAction(const Fn_TraAction& fn) const;
	static int TraActionImpl(const ActionCont& actions, const Fn_TraAction& fn);

	pt_dword		ukn_cnt_;
	ActionCont		actions_;
	ActionCont		actions1_;		//TalkAction、PortalAction、PutDownLiftableAction、BuyCubeAction
	ActionCont		actions2_;		//FishingAction，貌似只有一个？
};
//////////////////////////////////////////////////////////////////////////
struct stCD_Role : stCD_GameObject{
	typedef stCD_XTree<stCD_ActionNode> ActionCont;
	typedef std::function<bool(stCD_ActionNode*)> Fn_TraAction;

	void DebugInfo() const;
	void DebugSkills() const;
	
	p_mem_placeholder(g_o_role_lvl_ - sizeof(stCD_GameObject));
	pt_word						role_lvl_;
	p_mem_placeholder(2);
	stCD_BodyEquipMgr			body_equip_mgr_;
	p_mem_placeholder(g_o_skill_mgr_ - g_so_body_equip_mgr_ - sizeof(stCD_BodyEquipMgr));
	stCD_SkillMgr				skill_mgr_;
	p_mem_placeholder(g_o_role_action_mgr_ - g_o_skill_mgr_ - sizeof(stCD_SkillMgr));
	stCD_ActionMgr				action_mgr_;
};
static_assert(g_o_role_lvl_ == offsetof(stCD_Role, role_lvl_), "");
static_assert(g_so_body_equip_mgr_ == offsetof(stCD_Role, body_equip_mgr_), "");
static_assert(g_o_skill_mgr_ == offsetof(stCD_Role, skill_mgr_), "");
static_assert(g_o_role_action_mgr_ == offsetof(stCD_Role, action_mgr_), "");

struct stCD_RoleMe : stCD_Role{};

struct stCD_GameObjNode{
	pt_dword			obj_id_;
	p_mem_placeholder(4);
	stCD_GameObjBase*	game_obj_;
	pt_pcvoid			ukn_ptr_;

	void DebugInfo() const;
};
static_assert(0x10 == sizeof(stCD_GameObjNode), "");
typedef stCD_XTree<stCD_GameObjNode> stCD_GameObjMap;

//传送门
struct stCD_Portal{
	void DebugInfo() const;
	pt_cwsz GetDstMapName() const;

	pt_pcvoid			vtbl_;
	p_mem_placeholder(g_so_portal_pos_ - 4);
	stCD_Vec3Float		pos_;
	p_mem_placeholder(g_so_portal_dst_map_id_ - sizeof(stCD_Vec3Float) - g_so_portal_pos_);
	pt_dword			dst_map_id_;
};
static_assert(g_so_portal_pos_ == offsetof(stCD_Portal, pos_), "");
static_assert(g_so_portal_dst_map_id_ == offsetof(stCD_Portal, dst_map_id_), "");

struct stCD_PortalInfo{
	void DebugInfo() const;

	pt_dword		portal_id_;		// 过图最后一个参数编号id。也就是发包用
	p_mem_placeholder(4);		//=0
	stCD_Portal*	protal_;
	pt_dword		ukn_dw_;	//=0
	pt_dword		ukn_dw1_;
};
static_assert(0x14 == sizeof(stCD_PortalInfo), "");

//MapService
struct stCD_Map{
	typedef stCD_Vector<stCD_PortalInfo*> PortalCont;
	typedef std::function<bool(stCD_PortalInfo*)> Fn_TraPortal;

	stCD_Map() = delete;
	stCD_Map(const stCD_Map&) = delete;
	stCD_Map& operator = (const stCD_Map&) = delete;
	static stCD_Map* GetInstance();
	void DebugInfo() const;
	void DebugGameObjs() const;
	void DebugPortal() const;
	stCD_RoleMe* GetRoleMe() const;
	int TraPortal(const Fn_TraPortal& fn) const;

	p_mem_placeholder(g_so_map2game_objs_);
	stCD_GameObjMap		game_objs_;
	p_mem_placeholder(g_so_map_id_ - g_so_map2game_objs_ - sizeof(stCD_GameObjMap));
	pt_dword			map_id_;			//看着不像id啊
	p_mem_placeholder(g_so_map_cn_name_ - 4 - g_so_map_id_);
	pt_cwsz				map_cn_name_;		//地图中文名
	p_mem_placeholder(g_o_get_role_me_ - 4 - g_so_map_cn_name_);
	stCD_RoleMe*		role_me_;
	p_mem_placeholder(g_so_map_portal_ - 4 - g_o_get_role_me_);
	PortalCont			portals_;
};
static_assert(g_so_map2game_objs_ == offsetof(stCD_Map, game_objs_), "");
static_assert(g_so_map_id_ == offsetof(stCD_Map, map_id_), "");
static_assert(g_so_map_cn_name_ == offsetof(stCD_Map, map_cn_name_), "");
static_assert(g_o_get_role_me_ == offsetof(stCD_Map, role_me_), "");
static_assert(g_so_map_portal_ == offsetof(stCD_Map, portals_), "");
static_assert(0x280 == offsetof(stCD_Map, portals_), "");

struct stCD_CubeItemInfo{
	pt_pcvoid			vtbl_;
};

struct stCD_Cube{
	void DebugInfo() const;

	pt_pcvoid				vtbl_;
	pt_dword				id_;				//id?
	p_mem_placeholder(4);						//=0
	p_mem_placeholder(4 * 3);					//有值，像是指针
	p_mem_placeholder(4);						//=0,$18
	stCD_Model*				model_;
	p_mem_placeholder(4);						//指针？
	stCD_CubeItemInfo*		cube_item_info_;
	stCD_Vec3Int			pos_;
	enCD_TerrainType		terrain_type_;
};
static_assert(0x1c == offsetof(stCD_Cube, model_), "");
static_assert(0x24 == offsetof(stCD_Cube, cube_item_info_), "");
static_assert(0x34 == offsetof(stCD_Cube, terrain_type_), "");

struct stCD_CubeNode{
	pt_dword			id_;			//id?
	stCD_CubeNode*		parent_;
	stCD_CubeNode*		left_;
	stCD_CubeNode*		right_;
	pt_pcvoid			ukn_ptr_;
	pt_dword			ukn_dw_;		//=0
	stCD_Cube*			cube_;
};
static_assert(0x1c == sizeof(stCD_CubeNode), "");

struct stCD_MapInformationService{
	static stCD_MapInformationService* GetInstance();
	void DebugInfo() const;
	template<typename FnT>
	int TraCube(const FnT& fn) const{
		return StdEx::TraverseBTree(cube_root_.parent_, (stCD_CubeNode*)&cube_root_, [&fn](stCD_CubeNode* node) -> bool{
			if (IsUnValidPtr(node->cube_)) return true;
			return fn(node->cube_);
		});
	}

	pt_pcvoid			vtbl_;
	p_mem_placeholder(g_so_cube_root_node_ - 4);
	stCD_CubeNode		cube_root_;
};
static_assert(g_so_cube_root_node_ == offsetof(stCD_MapInformationService, cube_root_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_UiObjBase{
	void DebugInfo(pt_dword ui_id) const;
	CMN_CAST_TO

	pt_pcvoid			vtbl_;
	p_mem_placeholder(4);		//pt_dword
	p_mem_placeholder(4 * 2);
	pt_csz				ui_name_;
	p_mem_placeholder(4 * 7);
	bool				is_shown_;
	p_mem_placeholder(3);
};
static_assert(0x10 == offsetof(stCD_UiObjBase, ui_name_), "");
static_assert(g_so_ui_is_shown_ == offsetof(stCD_UiObjBase, is_shown_), "");

//快捷键？
struct stCD_SkillSlot{
	void DebugInfo() const;

	p_mem_placeholder(g_so_skill_slot_id_);
	pt_dword		slot_id_;		//快捷键ID？技能ID？技能对象的地址？
};
static_assert(g_so_skill_slot_id_ == offsetof(stCD_SkillSlot, slot_id_), "");

struct stCD_UIMainPlayerInfo : stCD_UiObjBase{
	typedef std::function<bool(stCD_SkillSlot*)> Fn_TraSkillSlot;

	void DebugSlotInfo() const;
	int TraSkillSlot(const Fn_TraSkillSlot& fn) const;
	stCD_SkillSlot* FindSkillSlotByIdx(pt_dword idx) const;
};

struct stCD_CharacterInfo{
	void DebugInfo() const;

	pt_pcvoid					vtbl_;
	enCD_LoginCharacterState	state_;
	p_mem_placeholder(g_so_login_character_id_ - 8);
	pt_qword					id_;		//发包用？
	p_mem_placeholder(g_so_login_character_name_ - 8 - g_so_login_character_id_);
	pt_cwsz						character_name_;
};
static_assert(g_so_login_character_id_ == offsetof(stCD_CharacterInfo, id_), "");
static_assert(g_so_login_character_name_ == offsetof(stCD_CharacterInfo, character_name_), "");

struct stCD_UICharacterListDialog : stCD_UiObjBase{
	struct stCharacter{
		pt_dword				ukn_dw_;
		stCD_CharacterInfo*		character_;
	};
	typedef GameVector<stCharacter> CharacterCont;

	void DebugInfo() const;

	p_mem_placeholder(g_so_character_cont_ - sizeof(stCD_UiObjBase));
	CharacterCont		characters_;
};
static_assert(g_so_character_cont_ == offsetof(stCD_UICharacterListDialog, characters_), "");

struct stCD_UICharacterCreateDialog : stCD_UiObjBase{
	//未测试
	bool CreateRole(pt_cwsz role_name) const;
};

struct stCD_UiNode{
	pt_dword			id_;
	pt_pcvoid			ukn_ptr_;
	stCD_UiObjBase*		ui_obj_;
	stCD_UiNode*		next_;
	pt_dword			id1_;		//值跟id一样。
};

struct stCD_UiMgr{
	typedef stCD_Vector<stCD_UiNode*> UiCont;
	typedef std::function<bool(pt_dword, stCD_UiObjBase*)> Fn_TraUi;

	static stCD_UiMgr* GetInstance();
	void DebugInfo(enCD_TraUiShowFlag show_flag) const;
	int TraUi(enCD_TraUiShowFlag show_flag, const Fn_TraUi& fn) const;
	stCD_UiObjBase* FindUi(enCD_TraUiShowFlag show_flag, pt_csz class_name) const;
	template<typename DerivedT>
	DerivedT* FindUi(enCD_TraUiShowFlag show_flag, pt_csz class_name) const{
		auto res = FindUi(show_flag, class_name);
		if (!res) return nullptr;
		return res->CastTo<DerivedT>();
	}
	//////////////////////////////////////////////////////////////////////////
	stCD_UIMainPlayerInfo* GetUiMainPlayerInfo() const;
	stCD_UICharacterListDialog* GetUICharacterListDialog() const;
	stCD_UICharacterCreateDialog* GetUICharacterCreateDialog() const;
	//////////////////////////////////////////////////////////////////////////

	pt_pcvoid		vtbl_;
	p_mem_placeholder(g_so_ui_mgr_ui_cont_ - 4);
	UiCont			ui_cont_;
};
static_assert(g_so_ui_mgr_ui_cont_ == offsetof(stCD_UiMgr, ui_cont_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_Quest{
	void DebugInfo() const;

	pt_pcvoid			vtbl_;
	pt_dword			ukn_dw_;		//这个好像也是任务状态，与quest_state_数值一样
	p_mem_placeholder(4);				//=0
	pt_pcvoid			vtbl1_;
	enCD_QuestState		quest_state_;	//$10
	p_mem_placeholder(g_so_quest_db_quest_ - 0x10 - 4);
	stCD_Data_Quest*	db_quest_;
	p_mem_placeholder(g_so_quest_is_ui_show_ - g_so_quest_db_quest_ - 4);
	bool				is_ui_show_;
	p_mem_placeholder(3);
};
static_assert(0x10 == offsetof(stCD_Quest, quest_state_), "");
static_assert(g_so_quest_db_quest_ == offsetof(stCD_Quest, db_quest_), "");
static_assert(g_so_quest_is_ui_show_ == offsetof(stCD_Quest, is_ui_show_), "");

struct stCD_QuestNode{
	pt_pcvoid		ukn_ptr_;
	p_mem_placeholder(4);
	stCD_Quest*		quest_;
};

struct stCD_QuestMgr{
	typedef stCD_XTree<stCD_QuestNode> QuestCont;
	typedef std::function<bool(stCD_Quest*)> Fn_TraQuest;

	static stCD_QuestMgr* GetInstance();
	void DebugInfo() const;
	int TraQuest(const Fn_TraQuest& fn) const;

	pt_pcvoid		vtbl_;
	pt_pcvoid		vtbl1_;
	pt_pcvoid		vtbl2_;
	p_mem_placeholder(g_so_quest_mgr_cont_ - 0xc);
	QuestCont		quests_;
};
static_assert(g_so_quest_mgr_cont_ == offsetof(stCD_QuestMgr, quests_), "");
//////////////////////////////////////////////////////////////////////////
//没有rtti
struct stCD_ModelBase{
	CMN_CAST_TO
	void DebugInfo() const;
	bool SmartDbgInfo() const;
	stCD_Model* CastToModel() const;

	pt_pcvoid		vtbl_;
};

struct stCD_Model : stCD_ModelBase{
	void DebugInfo() const;
	bool IsValid() const;
	stCD_Vec3Float GetPos() const;
};

//没有rtti
struct stCD_UknSceneObj{
	void DebugInfo() const;
	bool SmartDbgInfo() const;

	pt_pcvoid			vtbl_;
	p_mem_placeholder(g_so_scene_obj_model_obj_ - 4);
	stCD_ModelBase*		model_;
	p_mem_placeholder(g_so_scene_obj_ukn_flag_ - 4 - g_so_scene_obj_model_obj_);
	pt_byte				ukn_flag_;
};
static_assert(g_so_scene_obj_model_obj_ == offsetof(stCD_UknSceneObj, model_), "");
static_assert(g_so_scene_obj_ukn_flag_ == offsetof(stCD_UknSceneObj, ukn_flag_), "");

struct stCD_SceneGraphService{
	typedef GameVector<stCD_UknSceneObj*> ModelCont;
	void DebugInfo() const;

	pt_pcvoid				vtbl_;
	p_mem_placeholder(g_so_scene_resource_path_ - 4);
	pt_csz					resource_path_;			//"Data\Resource"
	p_mem_placeholder(g_so_scene_model_cont_ - 4 - g_so_scene_resource_path_);
	ModelCont				models_;
	stCD_UknGameVector		ukn_vector_;
};
static_assert(g_so_scene_resource_path_ == offsetof(stCD_SceneGraphService, resource_path_), "");
static_assert(g_so_scene_model_cont_ == offsetof(stCD_SceneGraphService, models_), "");

struct stCD_LightService{
	static stCD_LightService* GetInstance();
	void DebugInfo() const;

	pt_pcvoid					vtbl_;
	p_mem_placeholder(g_so_scene_graph_service_ - 4);
	stCD_SceneGraphService*		scene_graph_;
};
static_assert(g_so_scene_graph_service_ == offsetof(stCD_LightService, scene_graph_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_PartyMember{
	void DebugInfo() const;

	pt_pcvoid			vtbl_;
	p_mem_placeholder(g_so_party_mem_id_ - 4);
	pt_qword			id_;
	p_mem_placeholder(g_so_party_mem_name_ - 8 - g_so_party_mem_id_);
	pt_cwsz				name_;
};
static_assert(g_so_party_mem_id_ == offsetof(stCD_PartyMember, id_), "");
static_assert(g_so_party_mem_name_ == offsetof(stCD_PartyMember, name_), "");

//组队
struct stCD_PartyMgr{
	struct stMemberInfo{
		pt_dword			ukn_dw_;		//=0,难道是类型？
		stCD_PartyMember*	member_;
	};
	typedef stCD_Vector<stMemberInfo> MemberCont;

	static stCD_PartyMgr* GetInstance();
	void DebugInfo() const;

	pt_pcvoid			vtbl_;
	pt_dword			team_id_;			//队伍id
	pt_dword			captain_id_;		//队长id
	p_mem_placeholder(g_so_party_member_cont_ - 0xc);
	MemberCont			members_;
	p_mem_placeholder(g_so_party_team_name_ - g_so_party_member_cont_ - sizeof(MemberCont));
	pt_cwsz				team_name_;
	p_mem_placeholder(g_so_party_captain_name_ - 4 - g_so_party_team_name_);
	pt_cwsz				captain_name_;
};
static_assert(g_so_party_member_cont_ == offsetof(stCD_PartyMgr, members_), "");
static_assert(g_so_party_team_name_ == offsetof(stCD_PartyMgr, team_name_), "");
static_assert(g_so_party_captain_name_ == offsetof(stCD_PartyMgr, captain_name_), "");
//////////////////////////////////////////////////////////////////////////
struct stCD_GlobalUtility{
	static pt_dint GetGold();
	//当前频道、当前线路
	static pt_dword GetCurLine();
	static HWND GetWndHandle();
	static void DebugInfo();
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)

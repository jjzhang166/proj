#include "stdafx.h"
#include "CD_Structs.h"
#include "CD_UsefulFns.h"
#include <CommonX/CmnX_UsefulFunc.h>
#include "CD_FuncCall.h"
#include <cassert>
#include <CmnMix/Cmn_LogOut.h>
#include "CD_Updates.h"
#include <stack>
#include <set>
#include <CommonX/CmnX_StaticConts.h>
#include <StdEx/RttiEx.h>
//////////////////////////////////////////////////////////////////////////


stCD_Map* stCD_Map::GetInstance()
{
	auto res = *(stCD_Map**)g_a_map_;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

stCD_RoleMe* stCD_Map::GetRoleMe() const
{
	if (IsUnValidPtr(role_me_))
	{
		assert(false);
		return nullptr;
	}
	return role_me_;
}

void stCD_Map::DebugInfo() const
{
	LOG_O(Log_debug) << "MapAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",MapId:" << (pt_pcvoid)map_id_ << ",MapName:" << CD_SafeName(map_cn_name_);
	auto p = GetRoleMe();
	if (p)
		p->DebugInfo();
}

void stCD_Map::DebugGameObjs() const
{
	game_objs_.DebugInfo();
}

int stCD_Map::TraPortal(const Fn_TraPortal& fn) const
{
	int cnt = 0;
	for (auto portal_info : portals_)
	{
		if (IsUnValidPtr(portal_info))
			continue;
		if (IsUnValidPtr(portal_info->protal_))
			continue;
		++cnt;
		if (!fn(portal_info))
			break;
	}
	return cnt;
}

void stCD_Map::DebugPortal() const
{
	auto cnt = TraPortal([](stCD_PortalInfo* port){
		port->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "数量：" << cnt;
}

void stCD_Role::DebugInfo() const
{
	LOG_O(Log_debug) << "这个是角色，RoleLvl:" << role_lvl_;
	__super::DebugInfo();
}

void stCD_Role::DebugSkills() const
{
	skill_mgr_.DebugInfo();
}

void stCD_ActionMgr::DebugAction() const
{
	auto cnt = TraAction([](const stCD_ActionNode& action_node){
		action_node.DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总数：" << cnt;
}

int stCD_ActionMgr::TraAction(const Fn_TraAction& fn) const
{
	return TraActionImpl(actions_, fn);
	/*auto cnt = TraActionImpl(actions_, fn);
	LOG_O(Log_debug) << "---------------------------";
	cnt += TraActionImpl(actions1_, fn);
	LOG_O(Log_debug) << "---------------------------";
	cnt += TraActionImpl(actions2_, fn);
	return cnt;*/
}

int stCD_ActionMgr::TraActionImpl(const ActionCont& actions, const Fn_TraAction& fn)
{
	return actions.Traverse(fn);
}

void stCD_GameObject::DebugObjAttrInfo(const stCD_ObjAttrValues& attr_values)
{
	LOG_O(Log_debug) << "力量：" << attr_values[kRAVT_Power] << ",敏捷：" << attr_values[kRAVT_Smart];
	LOG_O(Log_debug) << "智力：" << attr_values[kRAVT_Brain] << ",幸运：" << attr_values[kRAVT_Lucky];
	LOG_O(Log_debug) << "生命：" << attr_values[kRAVT_Hp] << ",生命恢复：" << attr_values[kRAVT_HpResume];
	LOG_O(Log_debug) << "能量：" << attr_values[kRAVT_Energy] << ",体力：" << attr_values[kRAVT_Sp];
	LOG_O(Log_debug) << "命中：" << attr_values[kRAVT_MingZhong] << ",回避：" << attr_values[kRAVT_HuiBi];
	LOG_O(Log_debug) << "暴击命中：" << attr_values[kRAVT_BaoJiMingZhong] << ",暴击回避：" << attr_values[kRAVT_BaoJiHuiBi];
	LOG_O(Log_debug) << "防御力：" << attr_values[kRAVT_Defense] << "，物理攻击力：" << attr_values[kRAVT_PhysicalAttack];
	LOG_O(Log_debug) << "魔法攻击力：" << attr_values[kRAVT_MagicAttack] << ",物理抵抗力：" << attr_values[kRAVT_PhysicalDefense];
	LOG_O(Log_debug) << "魔法抵抗力：" << attr_values[kRAVT_MagicDefense];
}

void stCD_GameObject::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << ",ObjName:" << CD_SafeName(obj_name_) << ",CubePos:" << cube_pos_;
	LOG_O(Log_debug) << "CurAttrValues:";
	DebugObjAttrInfo(cur_attr_values_);
	LOG_O(Log_debug) << "MaxAttrValues:";
	DebugObjAttrInfo(max_attr_values_);
	if (!IsUnValidPtr(db_game_obj_))
		db_game_obj_->DebugInfo();
	else
		LOG_O(Log_debug) << "db_game_obj_并不是stCD_GameObject中的成员变量，不过暂时不管了";
	if (!IsUnValidPtr(state_mgr_))
		state_mgr_->DebugInfo();
	else
		assert(false);
}

void stCD_GameObjBase::SmartDbgInfo() const
{
	auto game_obj = CastToGameObject();
	if (game_obj)
	{
		game_obj->SmartDbgInfo();
		return;
	}
	auto drop_item = CastToDropItem();
	if (drop_item)
	{
		drop_item->SmartDbgInfo();
		return;
	}
	DebugInfo();
}

void stCD_StateMgr::DebugInfo() const
{
	LOG_O(Log_debug) << "StateMgrAddr:" << this << ",ObjState:" << obj_state_;
}

void stCD_GameObjNode::DebugInfo() const
{
	LOG_O(Log_debug) << "ObjId:" << obj_id_;
	if (!IsUnValidPtr(game_obj_))
		game_obj_->SmartDbgInfo();
	else
		assert(false);
	LOG_O(Log_debug) << "---------------------------------------------------";
}

void stCD_GameObjBase::DebugInfo() const
{
	LOG_O(Log_debug) << "GameObjBaseAddr:" << this << ",ClassName:" << CD_TypeName(this) 
		<< ",vtbl:" << vtbl_ << ",vtbl1:" << vtbl1_
		<< ",UknType:" << ukn_type_
		<< ",Pos:" << pos_ << ",dir_angle:" << dir_angle_
		<< ",ObjId:" << obj_id_ << ",ObjTypeName:" << CD_SafeName(obj_type_name_);
}

stCD_GameObject* stCD_GameObjBase::CastToGameObject() const
{
	if (StdEx::TStrCmp(obj_type_name_, CD_kGameObjType_GameObject) != 0)
		return nullptr;
	return (stCD_GameObject*)CastTo<stCD_GameObject>();
}

stCD_DropItem* stCD_GameObjBase::CastToDropItem() const
{
	if (StdEx::TStrCmp(obj_type_name_, CD_kGameObjType_DropItemNIF) != 0)
		return nullptr;
	return (stCD_DropItem*)CastTo<stCD_DropItem>();
}

stCD_Monster* stCD_GameObject::CastToMonster() const
{
	if (IsUnValidPtr(db_game_obj_))
		return nullptr;
	if (db_game_obj_->friendly_ != 0)
		return nullptr;
	return (stCD_Monster*)CastTo<stCD_Monster>();
}

void stCD_GameObject::SmartDbgInfo() const
{
	auto m = CastToMonster();
	if (m)
	{
		m->DebugInfo();
		return;
	}
	auto npc = CastToNpc();
	if (npc)
	{
		npc->DebugInfo();
		return;
	}
	auto role = CastToRole();
	if (role)
	{
		role->DebugInfo();
		return;
	}
	DebugInfo();
}

stCD_Npc* stCD_GameObject::CastToNpc() const
{
	if (IsUnValidPtr(db_game_obj_))
		return nullptr;
	if (db_game_obj_->friendly_ != 2)
		return nullptr;
	return (stCD_Npc*)CastTo<stCD_Npc>();
}

stCD_Role* stCD_GameObject::CastToRole() const
{
	return nullptr;
}

void stCD_Monster::DebugInfo() const
{
	LOG_O(Log_debug) << "这个是怪物";
	__super::DebugInfo();
}

void stCD_Npc::DebugInfo() const
{
	LOG_O(Log_debug) << "这个是NPC";
	__super::DebugInfo();
}

void stCD_DropItem::DebugInfo() const
{
	LOG_O(Log_debug) << "掉落物";
	__super::DebugInfo();
	if (!IsUnValidPtr(item_))
		item_->DebugInfo();
	else
		assert(false);
}

void stCD_DropItem::SmartDbgInfo() const
{
	DebugInfo();
}

pt_dint stCD_GlobalUtility::GetGold()
{
	return *(pt_dint*)g_a_gold_;
}

void stCD_GlobalUtility::DebugInfo()
{
	LOG_O(Log_debug) << "金币：" << GetGold() << ",当前频道：" << GetCurLine() << ",窗口句柄：" << GetWndHandle();
}

pt_dword stCD_GlobalUtility::GetCurLine()
{
	return *(pt_dword*)g_a_cur_line_;
}

HWND stCD_GlobalUtility::GetWndHandle()
{
	auto res = *(HWND*)g_a_game_wnd_handle_;
	if (::IsWindow(res))
		return res;
	assert(false);
	return nullptr;
}

stCD_BagMgr* stCD_BagMgr::GetInstance()
{
	return (stCD_BagMgr*)g_a_bag_mgr_;
}

int stCD_BagMgr::TraInventory(const Fn_TraInventory& fn) const
{
	int cnt = 0;
	for (int inventory = kIT_Equip; inventory < kIT_Max; ++inventory)
	{
		++cnt;
		if (!fn((enCD_InventoryType)inventory, (stCD_InventoryTabPage*)&inventory_[inventory]))
			break;
	}
	return cnt;
}

void stCD_BagMgr::DebugInfo(enCD_InventoryType inventory_type) const
{
	if (inventory_type < 0 || inventory_type >= kIT_Max)
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "BagMgrAddr:" << this << ",ClassName:" << CD_TypeName(this);
	inventory_[inventory_type].DebugInfo();
}

void stCD_ItemBase::DebugInfo() const
{
	LOG_O(Log_debug) << "ItemBaseAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",ItemId:" << item_id_
		<< ",item_idx:" << item_idx_ << ",color:" << color_
		<< ",item_type:" << item_type_ << ",item_cnt:" << item_cnt_;
	if (!IsUnValidPtr(db_item_))
		db_item_->DebugInfo();
	else
		assert(false);
}

int stCD_InventoryTabPage::TraItems(const Fn_TraItem& fn) const
{
	if (0 == max_cnt_)
		return 0;
	if (IsUnValidPtr(items_))
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (pt_dword idx = 0; idx < max_cnt_; ++idx)
	{
		auto item_info = items_ + idx;
		if (IsUnValidPtr(item_info->item_))
			continue;
		++cnt;
		if (!fn(item_info->item_))
			break;
	}
	return cnt;
}

void stCD_InventoryTabPage::DebugInfo() const
{
	auto cnt = TraItems([](stCD_ItemBase* item){
		item->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "遍历到的数量：" << cnt;
}

int stCD_SkillMgr::TraSkill(const Fn_TraSkill& fn) const
{
	if (IsUnValidPtr(skill_info_))
	{
		assert(false);
		return 0;
	}
	auto skills_info = skill_info_->skills_;
	if (IsUnValidPtr(skill_info_))
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for (auto the_skill_info : skills_info->skills_)
	{
		if (IsUnValidPtr(the_skill_info))
			continue;
		if (IsUnValidPtr(the_skill_info->skill_))
			continue;
		++cnt;
		if (!fn(the_skill_info->skill_))
			break;
	}
	return cnt;
}

void stCD_SkillMgr::DebugInfo() const
{
	auto cnt = TraSkill([](stCD_Skill* skill){
		skill->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总数量：" << cnt << ",ClassName:" << CD_TypeName(this);
}

void stCD_Skill::DebugInfo() const
{
	LOG_O(Log_debug) << "SkillAddr:" << this << ",ClassName:" << CD_TypeName(this) 
		<< ",IsActive:" << is_active_
		<< ",skill_lvl:" << skill_lvl_ << ",SkillName:" << CD_SafeName(GetSkillName());
}

pt_cwsz stCD_Skill::GetSkillName() const
{
	auto xml = stCD_XmlString::GetInstance();
	if (!xml)
	{
		assert(false);
		return nullptr;
	}
	return xml->FindStr(kXSIT_Skill, skill_name_xmlid_);
}

stCD_UiMgr* stCD_UiMgr::GetInstance()
{
	auto ui_mgr = *(stCD_UiMgr**)g_a_ui_mgr_;
	if (IsUnValidPtr(ui_mgr))
	{
		assert(false);
		return nullptr;
	}
	return ui_mgr;
}

int stCD_UiMgr::TraUi(enCD_TraUiShowFlag show_flag, const Fn_TraUi& fn) const
{
	int cnt = 0;
	for (auto node : ui_cont_)
	{
		for (; !IsUnValidPtr(node); node = node->next_)
		{
			auto ui_obj = node->ui_obj_;
			if (IsUnValidPtr(ui_obj))
				continue;
			int show_valid_cnt = 0;
			if (show_flag & kTUSF_Show)
				show_valid_cnt += !!ui_obj->is_shown_;
			if (show_flag & kTUSF_UnShow)
				show_valid_cnt += !ui_obj->is_shown_;
			if (0 == show_valid_cnt) continue;
			++cnt;
			if (!fn(node->id_, ui_obj))
				goto LABLE_BREAK;
		}
		continue;
	LABLE_BREAK:
		break;
	}
	return cnt;
}

void stCD_UiMgr::DebugInfo(enCD_TraUiShowFlag show_flag) const
{
	int cnt = 0;
	cnt = TraUi(show_flag, [](pt_dword ui_id, stCD_UiObjBase* ui){
		ui->DebugInfo(ui_id);
		return true;
	});
	LOG_O(Log_debug) << "数量：" << cnt << ",ClassName:" << CD_TypeName(this);
}

stCD_UiObjBase* stCD_UiMgr::FindUi(enCD_TraUiShowFlag show_flag, pt_csz class_name) const
{
	stCD_UiObjBase* ui_res = nullptr;
	TraUi(show_flag, [&ui_res, class_name](pt_dword ui_id, stCD_UiObjBase* ui){
		if (0 == StdEx::TStrCmp(StdEx::RttiEx::TypeName(ui), class_name))
		{
			ui_res = ui;
			return false;
		}
		return true;
	});
	assert(ui_res);
	return ui_res;
}

stCD_UIMainPlayerInfo* stCD_UiMgr::GetUiMainPlayerInfo() const
{
	return FindUi<stCD_UIMainPlayerInfo>(kTUSF_Show, CD_kClassName_UIMainPlayerInfo);
}

stCD_UICharacterListDialog* stCD_UiMgr::GetUICharacterListDialog() const
{
	return FindUi<stCD_UICharacterListDialog>(kTUSF_Show, CD_kClassName_UICharacterListDialog);
}

stCD_UICharacterCreateDialog* stCD_UiMgr::GetUICharacterCreateDialog() const
{
	return FindUi<stCD_UICharacterCreateDialog>(kTUSF_Show, CD_kClassName_UICharacterCreateDialog);
}

void stCD_UiObjBase::DebugInfo(pt_dword ui_id) const
{
	LOG_O(Log_debug) << "UiObjBaseAddr:" << this
		<< ",ClassName:" << CD_TypeName(this)
		<< ",UiId:" << ui_id
		<< ",UiName:" << CD_SafeName(ui_name_)
		<< ",IsShown:" << is_shown_;
}

stCD_QuestMgr* stCD_QuestMgr::GetInstance()
{
	auto res = *(stCD_QuestMgr**)g_a_quest_mgr_;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

int stCD_QuestMgr::TraQuest(const Fn_TraQuest& fn) const
{
	return quests_.Traverse([&fn](const stCD_QuestNode& quest_node){
		if (IsUnValidPtr(quest_node.quest_))
			return true;
		return fn(quest_node.quest_);
	});
}

void stCD_QuestMgr::DebugInfo() const
{
	auto cnt = TraQuest([](stCD_Quest* quest){
		quest->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总数：" << cnt << ",ClassName:" << CD_TypeName(this);
}

void stCD_Quest::DebugInfo() const
{
	LOG_O(Log_debug) << "QuestAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",ClassName1:" << CD_VtblTypeName(vtbl1_)
		<< ",QuestState:" << quest_state_ << ",IsUiShown:" << is_ui_show_
		<< ",UknDw:" << ukn_dw_;
	if (!IsUnValidPtr(db_quest_))
		db_quest_->DebugInfo();
	else
		assert(false);
}

void stCD_SkillSlot::DebugInfo() const
{
	LOG_O(Log_debug) << "SkillSlotAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",SlotId:" << (pt_pcvoid)slot_id_;
}

void stCD_UIMainPlayerInfo::DebugSlotInfo() const
{
	auto cnt = TraSkillSlot([](stCD_SkillSlot* slot){
		slot->DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总数：" << cnt;
}

int stCD_UIMainPlayerInfo::TraSkillSlot(const Fn_TraSkillSlot& fn) const
{
	int cnt = 0;
	for (int i = 0; i < 16; ++i)
	{
		auto slot = FindSkillSlotByIdx(i);
		if (!slot)
			continue;
		++cnt;
		if (!fn(slot))
			break;
	}
	return cnt;
}

void stCD_PortalInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "PortalInfoAddr:" << this << ",Id:" << portal_id_ << ",ukn_dw:" << ukn_dw_ << ",ukn_dw1:" << ukn_dw1_;
	assert(protal_);
	protal_->DebugInfo();
}

void stCD_Portal::DebugInfo() const
{
	LOG_O(Log_debug) << "PortalAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",Pos:" << pos_ << ",DstMapId:" << dst_map_id_
		<< ",DstMapName:" << CD_SafeName(GetDstMapName());
}

pt_cwsz stCD_Portal::GetDstMapName() const
{
	auto xml = stCD_XmlString::GetInstance();
	if (!xml)
	{
		assert(false);
		return nullptr;
	}
	return xml->FindStr(kXSIT_Action, dst_map_id_);
}

void stCD_ActionNode::DebugInfo() const
{
	LOG_O(Log_debug) << "ActionId:" << action_id_;
	assert(action_);
	action_->SmartDbgInfo();
}

void stCD_ActionBase::DebugInfo() const
{
	LOG_O(Log_debug) << "ActionBaseAddr:" << this << ",ClassName:" << CD_TypeName(this);
}

void stCD_ActionBase::SmartDbgInfo() const
{
	auto talk = CastToTalkAction();
	if (talk)
	{
		talk->DebugInfo();
		return;
	}
	auto portal = CastToPortalAction();
	if (portal)
	{
		portal->DebugInfo();
		return;
	}
	auto inter = CastToInteractAction();
	if (inter)
	{
		inter->DebugInfo();
		return;
	}
	DebugInfo();
}

stCD_PortalAction* stCD_ActionBase::CastToPortalAction() const
{
	if (0 == StdEx::TStrCmp(StdEx::RttiEx::TypeName(this), CD_kClassName_PortalAction))
		return (stCD_PortalAction*)CastTo<stCD_PortalAction>();
	return nullptr;
}

stCD_TalkAction* stCD_ActionBase::CastToTalkAction() const
{
	if (0 == StdEx::TStrCmp(StdEx::RttiEx::TypeName(this), CD_kClassName_TalkAction))
		return (stCD_TalkAction*)CastTo<stCD_TalkAction>();
	return nullptr;
}

stCD_InteractAction* stCD_ActionBase::CastToInteractAction() const
{
	if (0 == StdEx::TStrCmp(StdEx::RttiEx::TypeName(this), CD_kClassName_InteractAction))
		return (stCD_InteractAction*)CastTo<stCD_InteractAction>();
	return nullptr;
}

void stCD_PortalAction::DebugInfo() const
{
	if (!IsUnValidPtr(portal_))
		portal_->DebugInfo();
	__super::DebugInfo();
}

void stCD_TalkAction::DebugInfo() const
{
	auto npc_name = CD_SafeName(npc_name_);
	if (npc_name && *npc_name != 0)
		LOG_O(Log_debug) << "NpcName:" << npc_name;
	__super::DebugInfo();
}

void stCD_InteractAction::DebugInfo() const
{
	auto dst_name = CD_SafeName(dst_name_);
	if (dst_name && *dst_name != 0)
		LOG_O(Log_debug) << "DstName:" << dst_name;
	__super::DebugInfo();
}

void stCD_BodyEquipMgr::DebugInfo() const
{
	LOG_O(Log_debug) << "BodyEquipMgrAddr:" << this << ",UknDw:" << ukn_dw_;
	auto cnt = body_equips_.Traverse([](const stCD_BodyItemNode& body_item){
		body_item.DebugInfo();
		return true;
	});
	LOG_O(Log_debug) << "总数：" << cnt;
}

void stCD_BodyItemNode::DebugInfo() const
{
	if (IsUnValidPtr(equip_slot_))
		return;
	LOG_O(Log_debug) << "BodyItemNodeAddr:" << this << ",type:" << type_;
	equip_slot_->DebugInfo();
}

void stCD_EquipmentSlot::DebugInfo() const
{
	LOG_O(Log_debug) << "EquipmentSlotAddr:" << this << ",ClassName:" << CD_TypeName(this);
	if (IsUnValidPtr(equip_item_))
		return;
	equip_item_->DebugInfo();
}

void stCD_EquipItem::DebugInfo() const
{
	LOG_O(Log_debug) << "EquipItemAddr:" << this << ",ClassName:" << CD_TypeName(this);
	if (IsUnValidPtr(item_))
		return;
	item_->DebugInfo();
}

stCD_MailMgr& stCD_MailMgr::GetMe()
{
	return *(stCD_MailMgr*)g_a_mail_mgr_;
}

void stCD_MailMgr::DebugInfo() const
{
	int cnt = 0;
	for (auto& v : mails_)
	{
		if (IsUnValidPtr(v.mail_info_))
			continue;
		v.DebugInfo();
		++cnt;
	}
	LOG_O(Log_debug) << "数量：" << cnt;
}

void stCD_MailTypeInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "MailType:" << mail_type_;
	assert(mail_info_);
	mail_info_->DebugInfo();
}

void stCD_MailInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "MailInfoAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",Sender:" << CD_SafeName(sender_) << ",Title:" << CD_SafeName(title_);
}

stCD_LightService* stCD_LightService::GetInstance()
{
	auto res = *(stCD_LightService**)g_a_light_service_;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

void stCD_LightService::DebugInfo() const
{
	LOG_O(Log_debug) << "LightServiceAddr:" << this << ",ClassName:" << CD_TypeName(this);
	if (IsUnValidPtr(scene_graph_))
	{
		assert(false);
		return;
	}
	scene_graph_->DebugInfo();
}

void stCD_SceneGraphService::DebugInfo() const
{
	LOG_O(Log_debug) << "SceneGraphServiceAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",resource_path:" << CD_SafePath(resource_path_);
	int cnt = 0;
	for (auto o : models_)
	{
		if (IsUnValidPtr(o))
			continue;
		cnt += !!o->SmartDbgInfo();
	}
	LOG_O(Log_debug) << "数量：" << cnt;
}

void stCD_UknSceneObj::DebugInfo() const
{
	if (ukn_flag_ == 0xba)
		return;
	if (IsUnValidPtr(model_))
		return;
	LOG_O(Log_debug) << "UknSceneObj:" << this << ",vtbl:" << vtbl_;
	model_->DebugInfo();
}

bool stCD_UknSceneObj::SmartDbgInfo() const
{
	if (IsUnValidPtr(model_))
		return false;
	return model_->SmartDbgInfo();
}

void stCD_ModelBase::DebugInfo() const
{
	LOG_O(Log_debug) << "ModelBaseAddr:" << this << ",vtbl:" << vtbl_;
}

bool stCD_ModelBase::SmartDbgInfo() const
{
	auto model = CastToModel();
	if (model)
	{
		model->DebugInfo();
		return true;
	}
	return false;
}

stCD_Model* stCD_ModelBase::CastToModel() const
{
	if (vtbl_ == (pt_pcvoid)g_va_model_)
		return (stCD_Model*)CastTo<stCD_Model>();
	return nullptr;
}

void stCD_UICharacterListDialog::DebugInfo() const
{
	__super::DebugInfo(0);
	for (auto& v : characters_)
	{
		if (IsUnValidPtr(v.character_))
		{
			assert(false);
			continue;
		}
		v.character_->DebugInfo();
	}
}

void stCD_CharacterInfo::DebugInfo() const
{
	LOG_O(Log_debug) << "CharacterInfoAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",state:" << state_ << ",id:" << (pt_pcvoid)id_ << ",Name:" << CD_SafeName(character_name_);
}

stCD_PartyMgr* stCD_PartyMgr::GetInstance()
{
	auto res = *(stCD_PartyMgr**)g_a_party_mgr_;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

void stCD_PartyMgr::DebugInfo() const
{
	LOG_O(Log_debug) << "PartyMgrAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",TeamId:" << team_id_ << ",CaptainId:" << captain_id_
		<< ",队伍名：" << CD_SafeName(team_name_) << ",队长名：" << CD_SafeName(captain_name_);

	for (auto& mem : members_)
	{
		if (IsUnValidPtr(mem.member_))
			continue;
		mem.member_->DebugInfo();
	}
}

void stCD_PartyMember::DebugInfo() const
{
	LOG_O(Log_debug) << "PartyMemberAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",id:" << id_ << ",Name:" << CD_SafeName(name_);
}

void stCD_MapInformationService::DebugInfo() const
{
	LOG_O(Log_debug) << this << ",ClassName:" << CD_TypeName(this);
	int cnt = 0;
	TraCube([&cnt](stCD_Cube* cube){
		cube->DebugInfo();
		++cnt;
		return true;
	});
	LOG_O(Log_debug) << "数量：" << cnt;
}

stCD_MapInformationService* stCD_MapInformationService::GetInstance()
{
	auto res = *(stCD_MapInformationService**)g_c_map_information_service_;
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	return res;
}

void stCD_Cube::DebugInfo() const
{
	if (IsUnValidPtr(model_))
	{
		assert(false);
		return;
	}
	if (!model_->IsValid())
	{
		assert(false);
		return;
	}
	if (IsUnValidPtr(cube_item_info_))
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "CubeAddr:" << this << ",ClassName:" << CD_TypeName(this)
		<< ",CubeItemInfo_ClassName:" << CD_TypeName(cube_item_info_)
		<< ",pos:" << pos_ << ",terrain_type:" << terrain_type_
		<< ",ModelPos:" << model_->GetPos();
}

bool stCD_Model::IsValid() const
{
	return vtbl_ == (pt_pcvoid)g_va_model_;
}

void stCD_Model::DebugInfo() const
{
	LOG_O(Log_debug) << "ModelAddr:" << this << ",vtbl:" << vtbl_ << ",Pos:" << GetPos();
}

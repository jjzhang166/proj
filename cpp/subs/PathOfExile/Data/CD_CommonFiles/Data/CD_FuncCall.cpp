#include "stdafx.h"
#include "CD_FuncCall.h"
#include <cassert>
#include <CommonX/CmnX_UsefulFunc.h>
#include <CmnMix/Cmn_MemOper.h>
#include "CD_UsefulFns.h"
#include "CD_Const.h"
#include "CD_ProtocolS.h"
#include "CD_Data.h"
#include "CD_Interface.h"
//////////////////////////////////////////////////////////////////////////
const auto g_c_SendPacket = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_SendPacket_));
const auto g_c_BuildPacket = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_BuildPacket_));
const auto g_c_PlayerMove = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_player_move_));
const auto g_c_EncryptByte = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_encrypt_byte_));
const auto g_c_EncryptInt = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_encrypt_int_));
const auto g_c_EncryptByte1 = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_encrypt_byte1_));
const auto g_c_EncryptInt1 = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_encrypt_int1_));
const auto g_c_touch_target_obj = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_touch_target_obj_));
const auto g_c_close_npc_ui = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_close_npc_ui_));
const auto g_c_ui_logic_handle_esc_key = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_ui_logic_handle_esc_key_));
const auto g_c_waypoint_map_change_map = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_waypoint_map_change_map_));
const auto g_c_switch_to_escape_state = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_switch_to_escape_state_));
const auto g_c_escape_press_button = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_escape_press_button_));
const auto g_c_ui_npc_panel_press_menu_item = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_ui_npc_panel_press_menu_item_));
//const MemOper<Oper_ThisCall>		g_c_DecryptGetInt(g_c_decrypt_get_int_);
const auto g_c_logic_oper_command = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_logic_oper_command_));
const auto g_c_can_put_it_to_the_inventory = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_can_put_it_to_the_inventory_));
const auto g_c_ui_mouse_attach_item_get_item = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_ui_mouse_attach_item_get_item_));
const auto g_c_find_quest_state = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_find_quest_state_));
const auto g_c_revive = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_revive_));
const auto g_c_show_hide_ui = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_show_hide_ui_));
const auto g_c_lvl_up_skill_gem = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_lvl_up_skill_gem_));
const auto g_c_can_lvl_up_skill_gem = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_can_lvl_up_skill_gem_));
const auto g_c_get_skill_gem_require_attr = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_get_skill_gem_require_attr_));
const auto g_c_ui_npc_dlg_choice2_choose = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_ui_npc_dlg_choice2_choose_));
const auto g_c_ui_stash_press_switch_page = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_ui_stash_press_switch_page_));
const auto g_c_ui_press_item_normal1 = MakeMemOper<stCD_Vtbl_UiGridPanel::kPress_Normal1>();
const auto g_c_ui_press_item_normal2 = MakeMemOper<stCD_Vtbl_UiGridPanel::kPress_Normal2>();
const auto g_c_ui_press_item_shift = MakeMemOper<stCD_Vtbl_UiGridPanel::kPress_Shift>();
const auto g_c_ui_press_item_menu = MakeMemOper<stCD_Vtbl_UiGridPanel::kPress_Menu>();
const auto g_c_ui_press_item_control = MakeMemOper<stCD_Vtbl_UiGridPanel::kPress_Control>();
const auto g_c_ui_put_item_down = MakeMemOper<stCD_Vtbl_UiGridPanel::kPress_PutItemDown>();
const auto g_c_ui_sell_press_button = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_ui_sell_press_button_));
const auto g_c_get_item_price = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_get_item_price_));
const auto g_c_malloc = MakeMemOper(Oper_Cdecl(), CD_EXE_DATA(g_c_malloc_));
const auto g_c_free = MakeMemOper(Oper_Cdecl(), CD_EXE_DATA(g_c_free_));
const auto g_c_ui_enter_game = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_ui_enter_game_));
const auto g_c_press_create_role_buttons = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_press_create_role_buttons_));
const auto g_c_on_create_role = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_on_create_role_));
//const auto g_c_select_character = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_select_character_));
const auto g_c_select_character_profession = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_select_character_profession_));
const auto g_c_get_skill_attr_value = MakeMemOper<stCD_Vtbl_SkillAttrObj::kGetValue>();
const auto g_c_press_start_yi_jie = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_press_start_yi_jie_));
const auto g_c_ui_create_item_tips = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_ui_create_item_tips_));
const auto g_c_actor_handle_msg = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_actor_handle_msg_));
const auto g_c_gen_item_format_str = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_gen_item_format_str_));
const auto g_c_switch_check_box = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_switch_check_box_));
const auto g_c_can_press_goto_hide_pos = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_can_press_goto_hide_pos_));
const auto g_c_press_goto_hide_pos = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_press_goto_hide_pos_));
const auto g_c_select_map_locator_attr = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_select_map_locator_attr_));
const auto g_c_self_map_locator_press_button = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_self_map_locator_press_button_));
const auto g_c_actor_use_skill = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_actor_use_skill_));
const auto g_c_login_state_press_bt = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_login_state_press_bt_));
const auto g_c_terminate_cur_using_skill = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_terminate_cur_using_skill_));
//gj3.0才是这样
const auto g_c_map_pins_get_waypoint_world_area = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_map_pins_get_waypoint_world_area_));
const auto g_c_inside_gem = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_inside_gem_));
const auto g_c_takeoff_gem = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_takeoff_gem_));
const auto g_c_use_flask = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_use_flask_));
const auto g_c_use_item = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_use_item_));
const auto g_c_use_item_to = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_use_item_to_));
const auto g_c_merge_item = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_merge_item_));
const auto g_c_skip_all_tutorials = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_skip_all_tutorials_));
const auto g_h_map_data_modify_by_entity = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_h_map_data_modify_by_entity_));
const auto g_c_active_labyrinth = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_active_labyrinth_));
const auto g_c_select_labyrinth = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_select_labyrinth_));
const auto g_c_select_map_instance = MakeMemOper(Oper_ThisCall(), CD_EXE_DATA(g_c_select_map_instance_));
const auto g_c_character_creation_select = MakeMemOper<stCD_Vtbl_CharacterCreationInfo::kSelectCharacter>();
//////////////////////////////////////////////////////////////////////////
void FnForOd(const char* str)
{
	str += 1;
}
//////////////////////////////////////////////////////////////////////////

const stCD_Socket& stCD_Socket::BuildPacket(pt_pcvoid data, pt_dword size) const
{
	assert(data);
	assert(size > 0);
#ifdef _DEBUG
	auto res = 
#endif
		g_c_BuildPacket.Call<stCD_Socket*>(this, data, size);
	assert(res == this);
	return *this;
}

pt_dword stCD_Socket::DoSendPacket(pt_dword encrypt_value) const
{
	return g_c_SendPacket.Call<pt_dword>(this, encrypt_value);
}

bool stCD_Pathfinding::PlayerMove(const stCD_VecInt& dst_pos) const
{
	//游戏中调用该函数的对象是个Actor，stCD_Actor*。
	//p4表示移动到的目标点与dst_pos的距离
	static const int kParams[][5] = {
		{ 0, 0, 1, -1, 1 },		//正常走路时
		{ 0, 4, 1, 5, 1 },		//目标点是门或传送点时
		{ 0, 3, 1, 4, 1 },		//NPC
		{ 0, 2, 1, 3, 1 },		//活动告示板、流放者的信等建筑时
		{ 0, 1, 1, 2, 1 },		//仓库
	};

	//[esp+8]!=0||[esp+0c]!=0||[esp+10]!=1||[esp+14]!=-1||[esp+18]!=1
	//return g_c_PlayerMove.Call<bool>(this, &dst_pos, 0, 0, 1, -1, 1);
	//FnForOd("PlayerMove");
	//return g_c_PlayerMove.Call<bool>(this, &dst_pos, 0, 3, 1, 4, 1);
	//改成这个吧，虽然跟移动到NPC效果差不多，但这个毕竟是移动到建筑的。
	//return g_c_PlayerMove.Call<bool>(this, &dst_pos, 0, 2, 1, 3, 1);
	for (auto& param : kParams)
	{
		if (g_c_PlayerMove.Call<bool>(this, &dst_pos, param[0], param[1], param[2], param[3], param[4]))
			return true;
	}
	return false;
}


bool stCD_Pathfinding::PlayerMoveD(const stCD_VecInt& dst_pos, int p1, int p2, int p3, int p4, int p5) const
{
	auto res = g_c_PlayerMove.Call<bool>(this, &dst_pos, p1, p2, p3, p4, p5);
	LOG_O(Log_trace) << res;
	return res;
}

pt_char stCD_Encryptor::Encrypt(const pt_char& v, CD_TagEncrypt1) const
{
	pt_int the_value = v;
	return g_c_EncryptByte.Call<pt_char>(this, the_value);
}

pt_int stCD_Encryptor::Encrypt(const pt_int& v, CD_TagEncrypt1) const
{
	pt_int the_value = v;
	return g_c_EncryptInt.Call<pt_int>(this, the_value);
}

pt_char stCD_Encryptor::Encrypt(const pt_char& v, CD_TagEncrypt2) const
{
	pt_int the_value = v;
	return g_c_EncryptByte1.Call<pt_char>(this, the_value);
}

pt_int stCD_Encryptor::Encrypt(const pt_int& v, CD_TagEncrypt2) const
{
	pt_int the_value = v;
	return g_c_EncryptInt1.Call<pt_int>(this, the_value);
}

bool stCD_GameLogic::DoTouchTargetObj(enCD_SkillId flag, pt_dword net_id, enCD_TouchTargetSmallFlag small_flag) const
{
	auto entity = stCD_GameBase::GetMe().GetPlayerEntity();
	if (!entity)
	{
		assert(false);
		return false;
	}
	auto actor = entity->GetActor();
	if (!actor)
	{
		assert(false);
		return false;
	}
	actor->TerminateCurSkill();
	/*
	传送门：		266	08
	*/
	g_c_touch_target_obj.Call<void>(this, flag, net_id, small_flag);
	//防止会一直发打开对象的包。
	TerminateCurUsingSkill();
	actor->TerminateCurSkill();
	return true;
}

bool stCD_InGameMainUi::CloseNpcPanel() const
{
	//按esc时，参数是0，走路时，参数是1
	g_c_close_npc_ui.Call<void>(this, 1);
	return true;
}

bool stCD_InGameMainUi::EscapePanel() const
{
	//响应ui事件函数有两个参数，p1是子ui对象,p2是这个结构体的指针
	struct stCD_UiEventParam{
		bool			is_handled_;		//处理成功则置true
		p_mem_placeholder(3);				//字节对齐
		pt_dword		msg_id_;
		pt_dword		ukn_;

		stCD_UiEventParam(){
			ZeroMemory(this, sizeof(*this));
		}
	};
	static_assert(0xc == sizeof(stCD_UiEventParam), "");

	stCD_UiEventParam param;
	param.msg_id_ = 0x1b;
	g_c_ui_logic_handle_esc_key.Call<void>(this, 0, &param);
	return true;
}

bool stCD_DlgWaypointMap::CallChangeMap(const stCD_WaypointPointItem* point_item, bool ctrl_pressed)
{
	if (ctrl_pressed)
		CD_Interface::Hooks::EnableDummyCtrlPress(true);
	BOOST_SCOPE_EXIT_ALL(=){
		if (ctrl_pressed)
			CD_Interface::Hooks::EnableDummyCtrlPress(false);
	};
	waypoint_item_ = const_cast<stCD_WaypointPointItem*>(point_item);
	g_c_waypoint_map_change_map.Call<void>(this, point_item, 0);
	return true;
}

bool stCD_SceneInGameState::SwitchToEscapeState() const
{
	g_c_switch_to_escape_state.Call<void>(this);
	return true;
}

bool stCD_SceneEscapeState::PressBtItem(enCD_EscapeMenuButton bt_idx) const
{
	auto bt = FindButton(bt_idx);
	if (!bt)
	{
		assert(false);
		return false;
	}
	g_c_escape_press_button.Call<void>(this, bt);
	return true;
}

stCD_WorldAreaPairT stCD_MapPinsPair::GetTransferWorldItemPair(enCD_kMapDifficulty difficulty) const
{
	stCD_WorldAreaPairT res;
	SlowExecute([difficulty, &res, this](){
		auto res_ptr = &res;
		auto difficulty_tmp = difficulty;
		auto map_pin = this;
		auto call_addr = CD_EXE_DATA(g_c_map_pins_get_waypoint_world_area_);
		__asm{
			push difficulty_tmp
			mov edx, map_pin
			mov ecx, res_ptr
			call call_addr
			add esp, 4
		}
	});
	
	return res;
}

bool stCD_DlgWaypointMap::DiscovedTheMap(const stCD_WaypointPointItem& point_item) const
{
	if (IsUnValidPtr(ukn_obj_))
	{
		assert(false);
		return false;
	}
	bool res = false;
	SlowExecute([this, &res, &point_item](){
		auto ukn_obj = &ukn_obj_->ukn_obj_;
#ifndef VER3_
		auto difficulty = difficulty_;
#endif
		auto ukn_ptr = ukn_ptr_;
		auto map_pin = &point_item.map_pin_;

		bool the_res = false;
		auto res_ptr = &res;
		auto call_addr = CD_EXE_DATA(g_c_ui_waypoint_map_discovered_map_);
		__asm{
			push ukn_obj
#ifndef VER3_
			push difficulty
#endif
			mov ecx, ukn_ptr
			mov edx, map_pin
			call call_addr
#ifdef VER3_
			add esp, 4
#else
			add esp, 8
#endif
			mov the_res, al
		}
		*res_ptr = the_res;
	});	
	return res;
}

bool stCD_DlgWaypointMap::IsTheWaypointOpened(const stCD_WaypointPointItem& point_item) const
{
	auto world_area = point_item.map_pin_.GetTransferWorldItemPair(
#ifndef VER3_
		difficulty_
#endif
		);
	if (!world_area.IsValid())
		return false;
	if (IsUnValidPtr(ukn_obj_))
		return false;
	bool res = false;
	SlowExecute([&world_area, this, &res](){
		auto ukn_obj = &ukn_obj_->ukn_obj_;
		auto ptr_world_area = &world_area;
		bool the_res = false;
		auto res_ptr = &res;
		auto call_addr = CD_EXE_DATA(g_c_is_the_waypoint_opened_);
		__asm{
			mov ecx, ptr_world_area
			mov edx, ukn_obj
			call call_addr
			mov the_res, al
		}
		*res_ptr = the_res;
	});	
	return res;
}

bool stCD_UiNpcPanel::PressItem(const stCD_NpcPanelPressEventParam& param) const
{
	stCD_NpcPanelPressEventParam the_param = param;
	the_param.mouse_hover_ = true;
	g_c_ui_npc_panel_press_menu_item.Call<void>(this, 0, &the_param);
	return true;
}

pt_int stCD_Socket::DecryptGetInt() const
{
	//tag_收包解密dword
	assert(false);
	return 0;
	//return g_c_DecryptGetInt.Call<pt_int>(this);
}

bool stCD_GameLogic::OperateCommand(const stCD_SafeGameWString* command) const
{
	assert(!is_self_operate_command);
	BOOST_SCOPE_EXIT_ALL(this){
		is_self_operate_command = false;
	};
	is_self_operate_command = true;
	assert(command);
	auto res = g_c_logic_oper_command.Call<bool>(this, command);
	assert(res);
	return res;
}

bool stCD_InventoryMgr::CanPutItToTheInventory(const stCD_EntityItemBased* item,
	enCD_InventoryType dst_inventory, int dst_pos_x, int dst_pos_y) const
{
	assert(item);
	assert(dst_pos_x >= 0);
	assert(dst_pos_y >= 0);
	int buffer1[20] = { 0 }, buffer2[20] = { 0 }, buffer3[20] = { 0 };
	return g_c_can_put_it_to_the_inventory.Call<bool>(this, item, dst_inventory, dst_pos_x, dst_pos_y,
		(int*)buffer1,			//其实是个int*，但是为了避免错误，所以内存弄大点
		(int*)buffer2,			//其实是个int*，但是为了避免错误，所以内存弄大点
		(int*)buffer3			//其实是个int*，但是为了避免错误，所以内存弄大点
		);
}

stCD_EntityItemBased* stCD_UiMouseAttachItem::GetItem() const
{
	return g_c_ui_mouse_attach_item_get_item.Call<stCD_EntityItemBased*>(this);
}

stCD_QuestStatesPair* stCD_Player::FindQuestStateImpl(stCD_QuestStatesPair& out_res, const stCD_QuestPair& quest, enCD_kMapDifficulty difficult) const
{
	return g_c_find_quest_state.Call<stCD_QuestStatesPair*>(this, &out_res, &quest
#ifndef VER3_
		, difficult
#endif
		);
}

bool stCD_InGameMainUi::Revive(enCD_ReviveAtPosition at_position) const
{
	stCD_UiObjBase* ui_clicked = nullptr;
	if (kRAP_AtCity == at_position)
		ui_clicked = ui_at_city_;
	else if (kRAP_AtRecord == at_position)
		ui_clicked = ui_at_record_;
	else if (kRAP_AtDied == at_position)
		ui_clicked = ui_at_died_;
	if (IsUnValidPtr(ui_clicked))
	{
		assert(false);
		return false;
	}
	g_c_revive.Call<void>(this, ui_clicked);
	return true;
}

void stCD_UiObjBase::Show(bool is_show) const
{
	g_c_show_hide_ui.Call<void>(this, is_show);
}

bool stCD_UiLvlUpSkillGemItem::PressLvlUp() const
{
	auto bt = GetButton();
	if (!bt)
	{
		assert(false);
		return false;
	}
	struct stHelper{
		stHelper(const stCD_UiLvlUpSkillGemItem* this_ptr){
			ZeroMemory(this, sizeof(*this));
			this_ptr_ = this_ptr;
		}

		const stCD_UiLvlUpSkillGemItem*	this_ptr_;
		p_mem_placeholder(100);			//之所以加占位，是因为想要防患于未然
	};
	stHelper hlp(this);
	g_c_lvl_up_skill_gem.Call<void>(&hlp, bt);
	return true;
}

bool stCD_UiLvlUpSkillGemItem::CanLvlUpSkillGem() const
{
	if (!lvl_up_info_.empty())
	{
		if (IsUnValidPtr(item_))
		{
			assert(false);
			return false;
		}
		auto item_base = item_->GetAttrBase();
		if (!item_base)
		{
			assert(false);
			return false;
		}
		auto db_base = item_base->GetAttrDb();
		if (!db_base)
		{
			assert(false);
			return false;
		}
		if (db_base->name_.empty())
		{
			assert(false);
			return false;
		}
		auto name_str = db_base->name_.c_str();
		assert(name_str);
		for (auto& info : lvl_up_info_)
		{
			if (info.item_name_ == name_str)
			{
				auto skill_gem = item_->GetAttrSkillGem();
				if (!skill_gem)
				{
					assert(false);
					return false;
				}
				if (skill_gem->cur_lvl_ >= info.max_lvl_)
					return false;
				break;
			}
		}
	}
	return g_c_can_lvl_up_skill_gem.Call<bool>(this);
}

pt_dword stCD_EntityAttrSkillGem::GetRequireAttr(enCD_ItemRequireAttr type) const
{
	assert(type < kIRA_Max);
	return g_c_get_skill_gem_require_attr.Call<pt_dword>(this, type, 0);		//第二个参数是0，游戏里面是用的常量0.
}

bool stCD_UiNpcDlgChoice2::Choose(pt_byte choose_item) const
{
	if (choose_item >= _countof(bt_choice_))
	{
		assert(false);
		return false;
	}
	auto bt = bt_choice_[choose_item];
	if (IsUnValidPtr(bt))
	{
		assert(false);
		return false;
	}
	g_c_ui_npc_dlg_choice2_choose.Call<void>(this, bt);
	return true;
}

bool stCD_UiTabPages::PressSwitchPage(const stCD_UiObjBase* tab_button) const
{
	if (!pages_.IsValid())
	{
		assert(false);
		return false;
	}
	if (pages_.empty())
	{
		assert(!"是否没有打开仓库？");
		return false;
	}
	pt_dword page_idx = 0;
	for (auto& v : pages_)
	{
		if (v.tab_button_ == tab_button)
			break;
		++page_idx;
	}
	if (cur_page_idx_ == page_idx)
		return true;
	if (page_idx >= pages_.size())
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(tab_button))
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	stCD_UiEventParam_Pos param;
	g_c_ui_stash_press_switch_page.Call<void>(this, tab_button, &param);
	return true;
}

bool stCD_UiGridPanel::PressGridItem(enCD_PressItemWithKey key_state, pt_dword item_id) const
{
	//搜 g_o_ui_item_grid_item_id_ 可搜到asm信息
	//如果是在没有打开面板的情况下，通过item_id是找不到的，这个判断没有目前测着没有必要。
	/*auto item_grid = FindItemGrid(item_id);
	if (!item_grid)
	{
		assert(false);
		return false;
	}*/
	switch (key_state)
	{
	case kPIWK_Shift:
	{
		g_c_ui_press_item_shift.Call<void>(this, item_id);
		break;
	}
	case kPIWK_Control:
	{
		g_c_ui_press_item_control.Call<void>(this, item_id);
		break;
	}
	case kPIWK_Menu:
	{
		g_c_ui_press_item_menu.Call<void>(this, item_id);
		break;
	}
	case kPIWK_Normal:
	{
		g_c_ui_press_item_normal2.Call<void>(this, item_id, 1);
		g_c_ui_press_item_normal1.Call<void>(this, item_id);
		break;
	}
	default:
		assert(false);
		return false;
	}
	return true;
}

bool stCD_UiGridPanel::PutItemDown(const stCD_VecInt& dst_pos) const
{
	g_c_ui_put_item_down.Call<void>(this, &dst_pos);
	return true;
}

bool stCD_UiSellContainer::PressButton(bool accept) const
{
	stCD_UiObjBase* bt = nullptr;
	if (accept)
		bt = GetButtonAccept();
	else
		bt = GetButtonCancel();
	if (!bt)
	{
		assert(false);
		return false;
	}
	g_c_ui_sell_press_button.Call<void>(this, bt);
	return true;
}

stCD_ItemPriceContT* stCD_EntityAttrItemBase::GetPrice(stCD_ItemPriceContT& price) const
{
	return g_c_get_item_price.Call<stCD_ItemPriceContT*>(this, &price);
}

void* stCD_GameAllocatorImpl::Malloc(pt_dword size)
{
	return g_c_malloc.Call<void*>(size);
}

void stCD_GameAllocatorImpl::Free(void* ptr)
{
	g_c_free.Call<void>(ptr);
}

bool stCD_UiSelectRoleTop::EnterGame(const stCD_stCD_UiRoleListItem* item) const
{
	if (IsUnValidPtr(item))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(role_list_))
	{
		assert(false);
		return false;
	}
	if (!role_list_->SetCurSelectItem(item))
	{
		assert(false);
		return false;
	}
	struct stHelp{
		stHelp(const stCD_UiSelectRoleTop* ptr){
			ZeroMemory(this, sizeof(*this));
			ptr_ui_ = ptr;
		}

		pt_pcvoid						vtbl_;
		const stCD_UiSelectRoleTop*		ptr_ui_;		//只用到了这个变量
		p_mem_placeholder(50);
	};
	stHelp hlp = this;
	g_c_ui_enter_game.Call<void>(&hlp, 0);
	return true;
}

bool stCD_SceneCreateCharacterState::PressButton(enCD_CreateRoleBtIdx bt_idx) const
{
	assert(bt_idx < kCRBI_Max);
	auto bt = buttons_[bt_idx];
	if (IsUnValidPtr(bt))
	{
		assert(false);
		return false;
	}
	g_c_press_create_role_buttons.Call<void>(this, bt);
	return true;
}

bool stCD_SceneSelectCharacterState::PressCreateRole() const
{
	g_c_on_create_role.Call<void>(this);
	return true;
}

bool stCD_SceneCreateCharacterState::SelectCharacter(enCD_ProfessionType profession_type) const
{
	/*stCD_UiCreateRole_Character* character = nullptr;
	for (auto& v : characters_)
	{
		if (v.second == profession_type)
		{
			character = v.first;
			break;
		}
	}
	if (!character)
	{
		assert(false);
		return false;
	}
	stCD_UiEventParam_Pos param;
	g_c_select_character.Call<void>(this, character, &param);*/

	auto ptr = stCD_GameBase::GetMe().GetSceneHolder();
	if (IsUnValidPtr(ptr))
	{
		assert(false);
		return false;
	}
	auto pre_game_state = ptr->GetPreGameState();
	if (!pre_game_state)
	{
		assert(false);
		return false;
	}
	g_c_select_character_profession.Call<void>(this, profession_type);
	pre_game_state->SelectCharacter(profession_type);
	return true;
}

pt_dword stCD_SkillAttrObj::GetSkillAttrValue(pt_dword attr_value_id) const
{
	return g_c_get_skill_attr_value.Call<pt_dword>(this, attr_value_id);
}

bool stCD_DlgMapLocator::PressStartYiJie() const
{
	auto bt_start = GetBtStart();
	if (!bt_start)
	{
		assert(false);
		return false;
	}
	g_c_press_start_yi_jie.Call<void>(this, bt_start);
	return true;
}

stCD_ItemTipsParams* stCD_UiItemTips::FillItemTipsInfo(stCD_EntityItemBased* item) const
{
	if (!item)
	{
		assert(false);
		return false;
	}
	auto old_item = item_;
	BOOST_SCOPE_EXIT_ALL(old_item, this){
		item_ = old_item;
	};
	item_ = item;
	auto res = g_c_ui_create_item_tips.Call<stCD_ItemTipsParams*>(this);
	assert(res == &stCD_ItemTipsParams::GetMe());
	return res;
}

bool stCD_Actor::HandleMsg(const stCD_GameEntityMsg& msg) const
{
	g_c_actor_handle_msg.Call<void>(this, &msg);
	return true;
}

const GameWString* stCD_UiItemTips::GenAttrFormatStr(stCD_EntityItemBased* item) const
{
	if (!item)
	{
		assert(false);
		return false;
	}
	auto old_item = item_;
	BOOST_SCOPE_EXIT_ALL(old_item, this){
		item_ = old_item;
	};
	item_ = item;
	auto res = g_c_gen_item_format_str.Call<const GameWString*>(this);
	if (IsUnValidPtr(res))
	{
		assert(false);
		return nullptr;
	}
	assert(res == &tmp_format_attr_str_);
	return res;
}

bool stCD_UiCheckBox::Enable(bool enable) const
{
	if (enable_ == enable)
		return true;
	g_c_switch_check_box.Call<void>(this);
	if (enable != enable_)
	{
		assert(false);
		return false;
	}
	return true;
}

bool stCD_DlgWaypointMap::CanPressGotoHidePos() const
{
	return g_c_can_press_goto_hide_pos.Call<bool>(this);
}

bool stCD_DlgWaypointMap::PressGotoHidePos() const
{
	if (!CanPressGotoHidePos())
		return false;
	struct stHelp{
		stHelp(const stCD_DlgWaypointMap* ptr_map){
			ZeroMemory(this, sizeof(*this));
			ptr_map_ = ptr_map;
		}

		pt_pcvoid					vtbl_;
		const stCD_DlgWaypointMap*	ptr_map_;
		p_mem_placeholder(100);
	};
	stHelp hlp(this);
	g_c_press_goto_hide_pos.Call<void>(&hlp, 0);
	return true;
}

bool stCD_MapLocatorMapAttrList::Select(const stListItem* item) const
{
	if (IsUnValidPtr(item))
	{
		assert(false);
		return false;
	}
	int idx = 0;
	stCD_UiObjBase* ui_item = nullptr;
	for (auto& v : list_items_)
	{
		if (v.real_obj_ != item)
		{
			++idx;
			continue;
		}
		ui_item = item->ui_item_;
		break;
	}
	if (!ui_item)
	{
		assert(false);
		return false;
	}
	if (idx == cur_selected_)
		return true;
	stCD_UiEventParam_Pos param;
	g_c_select_map_locator_attr.Call<void>(this, ui_item, &param);
	return true;
}

bool stCD_DlgSelfMapLocator::PressStart() const
{
	if (IsUnValidPtr(bt_start_))
	{
		assert(false);
		return false;
	}
	if (!bt_start_->is_enable_)
	{
		assert(false);
		return false;
	}
	g_c_self_map_locator_press_button.Call<void>(this, bt_start_);
	return true;
}

bool stCD_Actor::UseSkill(pt_word skill_id, const stCD_VecInt* dst_pos)
{
	struct stHelp{
		stHelp(){
			ZeroMemory(this, sizeof(*this));
		}

		pt_word		use_skill_cnt_;
		p_mem_placeholder(100);
	};
	stHelp hlp;
	//p4其实是pt_word*
	g_c_actor_use_skill.Call<void>(this, skill_id, dst_pos, kTTSF_Normal, &hlp);
	return true;
}

bool stCD_SceneLoginState::PressBtLogin() const
{
	if (IsUnValidPtr(bt_login_))
	{
		assert(false);
		return false;
	}
	if (!bt_login_->is_shown_)
	{
		assert(false);
		return false;
	}
	if (!bt_login_->is_enable_)
	{
		assert(false);
		return false;
	}
	g_c_login_state_press_bt.Call<void>(this, bt_login_);
	return true;
}

void stCD_GameLogic::TerminateCurUsingSkill() const
{
	g_c_terminate_cur_using_skill.Call<void>(this);
}

bool stCD_Actor::TerminateCurSkill()
{
	if (!cur_using_skill_)
		return true;
	//assert(!using_skill_timed_);
	if (IsUnValidPtr(cur_using_skill_))
	{
		assert(false);
		return false;
	}
	auto old_skill = cur_using_skill_;
	skill_left_time_ = 0;
	cur_using_skill_ = nullptr;
	old_skill->Destruction(true);
	return true;
}

bool stCD_UiGridPanel::InsideGem(pt_dword item_id, pt_dword slot_idx) const
{
	g_c_inside_gem.Call<void>(this, item_id, slot_idx);
	return true;
}

bool stCD_UiGridPanel::TakeoffGem(pt_dword item_id, pt_dword slot_idx) const
{
	g_c_takeoff_gem.Call<void>(this, item_id, slot_idx);
	return true;
}

bool stCD_UiFlasksMgr::UseFlask(int idx) const
{
	g_c_use_flask.Call<void>(this, idx);
	return true;
}

bool stCD_UiGridPanel::UseItem(pt_dword item_id, stCD_EntityItemBased* item) const
{
	if (IsUnValidPtr(item))
	{
		assert(false);
		return false;
	}
	g_c_use_item.Call<void>(this, item_id, item);
	return true;
}

bool stCD_GameLogic::UseItemTo(enCD_InventoryType inventory_src, pt_dword item_id_src, enCD_InventoryType inventory_dst, pt_dword item_id_dst) const
{
	//if (inventory_src < kIT_MainBag || inventory_src > kIT_Flasks)
	//{
	//	assert(false);
	//	return false;
	//}
	//if (inventory_dst < kIT_MainBag || inventory_dst > kIT_Flasks)
	//{
	//	assert(false);
	//	return false;
	//}
	g_c_use_item_to.Call<void>(this, inventory_src, item_id_src, inventory_dst, item_id_dst);
	return true;
}

bool stCD_UiGridPanel::MergeItem(pt_dword dst_item_id) const
{
	g_c_merge_item.Call<void>(this, dst_item_id);
	return true;
}

void stCD_InGameMainUi::SkipAllTutorials()
{
	auto ui_bottom = GetUiBottom();
	if (ui_bottom)
	{
		auto bt_skip = ui_bottom->GetBtSkipAllTutorials();
		if (bt_skip)
		{
			struct stHelp{
				stHelp(stCD_InGameMainUi* main_ui){
					ZeroMemory(this, sizeof(*this));
					main_ui_ = main_ui;
				}

				stCD_InGameMainUi*	main_ui_;
				p_mem_placeholder(100);
			};
			stHelp hlp(this);
			g_c_skip_all_tutorials.Call<void>(&hlp, bt_skip);
		}
	}
	auto ui_tutorials = GetUiTutorials();
	if (ui_tutorials)
	{
		ui_tutorials->Show(false);
	}
}

void stCD_MapData::ModifyByEntity(stCD_EntityPosBased* entity)
{
	if (IsUnValidPtr(entity))
	{
		assert(false);
		return;
	}
	g_h_map_data_modify_by_entity.Call<void>(this, entity);
}

bool stCD_DlgLabyrinthSelect::PressActive() const
{
	if (IsUnValidPtr(bt_active_))
	{
		assert(false);
		return false;
	}
	if (!bt_active_->is_shown_)
	{
		assert(false);
		return false;
	}
	if (!bt_active_->is_enable_)
	{
		assert(false);
		return false;
	}
	g_c_active_labyrinth.Call<void>(this, bt_active_);
	return true;
}

bool stCD_UiLabyrinthList::SelectItem(const stListItem* item)
{
	if (!item)
	{
		assert(false);
		return false;
	}
	if (!list_items_.IsValid())
	{
		assert(false);
		return false;
	}
	pt_dword idx = 0;
	for (auto& v : list_items_)
	{
		if (v.real_obj_ == item)
			break;
		++idx;
	}
	if (idx >= list_items_.size())
	{
		assert(false);
		return false;
	}
	if (idx == cur_selected_)
		return true;
	if (IsUnValidPtr(item->ui_item_))
	{
		assert(false);
		return false;
	}
	stCD_UiEventParam_Pos param;
	g_c_select_labyrinth.Call<void>(this, item->ui_item_, &param);
	return true;
}

bool stCD_DlgInstanceMgr::SelectInstance(int instance_idx) const
{
	g_c_select_map_instance.Call<void>(this, instance_idx);
	return true;
}

bool stCD_CharacterCreationInfo::SelectCharacter(enCD_ProfessionType profession_type) const
{
	g_c_character_creation_select.Call<void>(this, profession_type);
	return true;
}

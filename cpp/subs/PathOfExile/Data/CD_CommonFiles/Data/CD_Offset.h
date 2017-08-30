#pragma once
/*
@author zhp
@date 2017/1/24 17:22

g_so->g_stable_offset
g_o->g_offset
g_dso->g_direct_stable_offset
*/
#include <ProtocolX/P_DefinesX.h>
#include "CD_Data.h"

pt_cdword	g_so_send_packet_encrypt_ = CD_DATA(g_o_send_packet_encrypt_);			//tag_发包之加密函数
pt_cdword	g_so_send_packet_msg_total_len_ = CD_DATA(g_o_send_msg_total_len_);
pt_cdword	g_so_send_packet_msg_data_ = g_so_send_packet_msg_total_len_ + 0x14;
pt_cdword	g_so_login_session_packet_ = 0x828;	//tag_获取发包对象_828
pt_cdword	g_so_packet_holder1_send_packet_ = 0x28;	//tag_获取发包对象的上层对象_28
#ifndef VER3_
pt_cdword	g_so_pre_game_state_main_ui_ = 0x40;
#else
pt_cdword	g_so_pre_game_state_main_ui_ = 0x3c;
#endif
pt_cdword	g_so_game_base2ui_obj_ = 0x38;				//tag_游戏基址索引ui对象偏移_38
pt_cdword	g_so_ukn_packet_holder2_ = 0x10;			//tag_获取UI对象中的成员变量偏移_10
pt_cdword	g_so_get_game_session_ = CD_DATA(g_o_game_logic_);		//tag_游戏内得到发包对象holder_13c
pt_cdword	g_so_player_power_ = 0x34;
pt_cdword	g_so_positioned_pos_ = 0x10;
pt_cdword	g_so_positioned_float_pos_ = 0x1c;
pt_cdword	g_so_life_hp_ = 0x1c;
pt_cdword	g_so_pathfinding_is_moving_ = CD_DATA(g_o_is_moving_);
pt_cdword	g_so_map_data_model_data_ = 0x24;
pt_cdword	g_so_map_data_shrink_size_ = 0x34;		//tag_地图数据之系数_34
pt_cdword	g_so_map_data_shrink_width_ = 0x54;
//pt_cdword	g_so_entity_meta_info_name_ = 0x14;
//pt_cdword	g_so_get_entity_attri_info_ = 0x30;		//tag_获取实体属性信息的偏移_30
pt_cdword	g_so_actor_get_actor_info_ = 0x24;
pt_cdword	g_so_actor_ukn_skill_info_ = 0x38;
pt_cdword	g_so_actor_use_skill_left_time_ = 0x70;
pt_cdword	g_so_actor_use_skill_cnt_ = 0x74;
pt_cdword	g_so_actor_info_file_path_ = 0xc;
pt_cdword	g_so_entity_attr_db_skill_type_ = 0x34;
pt_cdword	g_so_entity_attri_db_cont_ = 0x8;
pt_cdword	g_so_get_entity_attri_info_cont_ = 0x14;
pt_cdword	g_so_render_npc_name_ = 0x58;
#ifdef VER3_
pt_cdword	g_so_entity_id_ = 0x20;
pt_cdword	g_so_entity_base_pos_attr_ = 0x2c;
#else
pt_cdword	g_so_entity_id_ = 0x14;
pt_cdword	g_so_entity_base_pos_attr_ = 0x20;
#endif
pt_cdword	g_so_bag_grid_width_ = 0xc;
pt_cdword	g_so_get_bag_ = 0x20;		//没找到特征码
pt_cdword	g_so_bag_item_cnt_ = 0x30;
pt_cdword	g_so_player_db_render_name_ = 0x10;
pt_cdword	g_so_attr_db_item_base_name_ = 0x10;
pt_cdword	g_so_attr_db_item_base_item_type_pair_ = 0x48;
pt_cdword	g_so_entity_attr_mods_get_db_ = 0x14;
pt_cdword	g_so_entity_attr_mods_is_identified_ = 0x54;
pt_cdword	g_so_entity_attr_mods_name_color_ = 0x58;
pt_cdword	g_so_words_dat_path_ = 0x14;
pt_cdword	g_so_words_ch_words_ = 0x1c;
pt_cdword	g_so_attr_db_AttributeRequirements_power_ = 0x8;
pt_cdword	g_so_attr_db_stack_max_cnt_ = 0x14;
pt_cdword	g_so_attr_stack_get_db_ = 0x8;
pt_cdword	g_so_attr_sockets_get_db_ = 0x8;
pt_cdword	g_so_db_file_node_file_path_ = 0x8;
pt_cdword	g_so_tbl_skill_item_id_ = 0x20;
pt_cdword	g_so_tbl_skill_item_ch_name_ = 0x24;
pt_cdword	g_so_tbl_item_quest_states_size_ = 0x56;			//这个尺寸并不是4的倍数，可以通过这个结构体的第一个字段的值来比对并计算大小，因为第一个字段的值好像都是一样的。
pt_cdword	g_so_tbl_item_quest_states_desc_ = 0x1c;
pt_cdword	g_so_tbl_item_quest_size_ = 0x24;
pt_cdword	g_so_tbl_item_active_skill_dds_file_path_ = 0x10;
pt_cdword	g_so_tbl_item_active_skill_size_ = 0x57;
pt_cdword	g_so_tbl_item_base_item_type_item_name_ = 0x14;
pt_cdword	g_so_tbl_item_base_item_type_db_sound_effects_ = 0x34;
pt_cdword	g_so_tbl_item_base_item_type_size_ = 0xb2;
pt_cdword	g_so_tbl_item_skill_gems_gem_tag_ = 0x1c;
pt_cdword	g_so_tbl_item_skill_gems_size_ = 0x2d;
pt_cdword	g_so_attr_db_skill_gem_size_ = 0x38;
#ifdef VER3_
pt_cdword	g_so_tbl_item_granted_effects_active_skill_pair_ = 0x43;
pt_cdword	g_so_tbl_item_granted_effects_size_ = 0x4d;
#else
pt_cdword	g_so_tbl_item_granted_effects_active_skill_pair_ = 0x3b;
pt_cdword	g_so_tbl_item_granted_effects_size_ = 0x44;
#endif
pt_cdword	g_so_get_my_skills_cont_ = 0x10;
pt_cdword	g_so_my_skill_mgr_array_get_skill_mgr_ = 0x28;		//tag_stCD_MySkillMgrArray偏移_28
pt_cdword	g_so_tbl_item_world_area_ukn_ = 0x98;
//tag_有关于WorldArea结构大小的信息
#ifdef VER_GF_
pt_cdword	g_so_tbl_item_world_area_size_ = 0x18f;
#elif VER3_
pt_cdword	g_so_tbl_item_world_area_size_ = 0x18f;
#endif
pt_cdword	g_so_tbl_item_npcs_size_ = 0x3c;
pt_cdword	g_so_cur_quest_info_quest_state_ = 0x24;			//tag_当前任务信息的任务状态偏移
pt_cdword	g_so_tbl_item_map_pins_size_ = 0x68;
pt_cdword	g_so_tbl_item_map_connections_size_ = 0x24;
pt_cdword	g_so_tbl_item_npc_shop_size_ = 0x34;
pt_cdword	g_so_tbl_item_npc_talk_size_ = 0x87;
pt_cdword	g_so_tbl_item_mod_type_size_ = 0xc;
pt_cdword	g_so_tbl_item_mod_sell_price_type_size_ = 4;
pt_cdword	g_so_tbl_item_npc_master_size_ = 0x63;
pt_cdword	g_so_terrain_model_pos_ = 0x18;
pt_cdword	g_so_terrain_model_size_ = 0x20;
pt_cdword	g_so_tbl_item_model_model_path_ = 0x28;
pt_cdword	g_so_ui_logic_top_ui_ = CD_DATA(g_o_ui_mgr_top_get_dlg_ui_mgr_) - 0x14;		//tag_状态处理ui逻辑的函数,$aa4
pt_cdword	g_so_ui_next_state_info_size_ = 0x80;
pt_cdword	g_so_ui_obj_base_sub_uis_ = CD_DATA(g_o_ui_obj_base_is_shown_) - 0x50;
pt_cdword	g_so_escape_state_get_ui_logic_ = 0x20;			//tag_EscapeState处理UiLogic
pt_cdword	g_so_dlg_waypoint_map_ukn_obj_ = CD_DATA(g_o_dlg_waypoint_map_ukn_obj_);
pt_cdword	g_so_dlg_waypoint_map_ukn_ptr_ = g_so_dlg_waypoint_map_ukn_obj_ + 4;
pt_cdword	g_so_tbl_item_chests_size_ = 0x7a;
pt_cdword	g_so_entity_db_world_item_size_ = 0x5c;
pt_cdword	g_so_tbl_item_stats_size_ = 0x4e;
pt_cdword	g_so_tbl_item_weapon_types_size_ = 0x20;
pt_cdword	g_so_get_entity_attr_db_chest_ = 0x10;
pt_cdword	g_so_chesk_is_opened_ = 0x20;
#ifdef VER_GJ_
pt_cdword	g_so_ret_check_packet_0x1_ = CD_EXE_DATA(g_p_ret_0x1_0xf_check_packet_) + 0x39;
pt_cdword	g_so_hook_heartbeat_packet_ = CD_EXE_DATA(g_p_ret_0x1_0xf_check_packet_) + 0x61;
pt_cdword	g_so_ret_heartbeat_packet_0xf_ = CD_EXE_DATA(g_p_ret_0x1_0xf_check_packet_) + 0x9a;
#else
pt_cdword	g_so_ret_check_packet_0x1_ = CD_EXE_DATA(g_p_ret_0x1_0xf_check_packet_) + 0x41;
pt_cdword	g_so_hook_heartbeat_packet_ = CD_EXE_DATA(g_p_ret_0x1_0xf_check_packet_) + 0x69;
pt_cdword	g_so_ret_heartbeat_packet_0xf_ = CD_EXE_DATA(g_p_ret_0x1_0xf_check_packet_) + 0xaa;
#endif
pt_cdword	g_so_recv_packet_mgr_obj_cont_ = 0xc;
pt_cdword	g_so_sockets_mgr_ = CD_DATA(g_o_game_session_packet_) + 0x14;
pt_cdword	g_so_cur_using_skill_info_ = CD_DATA(g_o_cur_using_skill_info_) + 0xc;
pt_cdword	g_so_tbl_item_granted_effects_per_level_size_ = 0x95;
pt_cdword	g_so_attr_db_usable_type_ = 0x8;
pt_cdword	g_so_attr_db_flask_type_ = 0x18;
pt_cdword	g_so_tbl_item_monster_types_size_ = 0x29;
pt_cdword	g_so_tbl_item_monster_varieties_size_ = 0x1a2;
pt_cdword	g_so_ui_grid_panel_get_mouse_attach_item_ = CD_DATA(g_o_ui_grid_panel_get_inventory_) + 0x14;
pt_cdword	g_so_attr_inventory_equip_size_ = 0x24;
pt_cdword	g_so_GrantedEffectsPerLevel_require_lvl_ = CD_DATA(g_o_GrantedEffectsPerLevel_require_lvl_);
pt_cdword	g_so_world_environments_info_ = CD_DATA(g_o_world2map_data_) - 0x1c;
//pt_cdword	g_so_create_role_name_edit_ = 0x60;
pt_cdword	g_so_create_role_name_edit_ = 0x58;
pt_cdword	g_so_create_role_cur_area_idx = 0x98;
pt_cdword	g_so_create_role_cur_character_ = 0xbc;
pt_cdword	g_v_tbl_item_passive_skill_size_ = 0X00000077;
pt_cdword	g_so_item_tips_param_desc_suffix_ = 0xf4;
pt_cdword	g_sv_game_area_size_ =  1 << CD_DATA(g_v_game_area_size_);
static_assert(0x80 == g_sv_game_area_size_, "");
/*
#else
pt_cdword	g_sv_game_area_size_ = CD_DATA(g_v_game_area_size_);
#endif*/
pt_cdword	g_so_skill_obj_ukn_bool_ = 0x17;
pt_cdword	g_so_skill_obj_update_mouse_pos_ = 0x1c;
pt_cdword	g_so_skill_obj_update_mouse_pos_jmp_addr_ = 0x1e;
pt_cdword	g_so_skill_obj_dst_pos_ = 0x20;
pt_cdword	g_so_skill_obj_size_ = 0x34;
//////////////////////////////////////////////////////////////////////////
//********************国际服中新增的
pt_cdword	g_so_login_state_bt_login_ = 0x40;					//去搜g_so_login_state_bt_login_
pt_cdword	g_so_login_state_list_gateway_ = 0x6c;
pt_cdword	g_so_login_state_edit_pwd_ = 0x8c;
pt_cdword	g_so_login_state_edit_ac_ = 0x94;					//去搜g_so_login_state_edit_ac_
pt_cdword	g_so_login_state_gateway_info_size_ = 0x58;			//去搜g_so_login_state_gateway_info_size_

#ifdef VER3_
pt_cdword	g_so_world_area_area_id_ = 0x1a;
pt_cdword	g_so_map_world_area_ = 0x10;
pt_cdword	g_so_skill_info_attr_ = 0x48;
#else
pt_cdword	g_so_world_area_area_id_ = 0x1f;
pt_cdword	g_so_map_world_area_ = 4;
pt_cdword	g_so_skill_info_attr_ = 0x40;
#endif
pt_cdword	g_so_hp_mp_circle_value_cur_max_value_ = CD_DATA(g_o_hp_mp_circle_value_cur_value_) - 4;
pt_cdword	g_so_character_creation_info_ = 0x38;		//去搜

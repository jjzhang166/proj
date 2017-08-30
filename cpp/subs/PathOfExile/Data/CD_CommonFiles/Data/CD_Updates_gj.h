#pragma once
#include <ProtocolX/P_BaseTypeX.h>
/*
g_a->g_address.
g_o->g_offset.
g_va->g_vtbl_address.
g_vo->g_vtbl_offset.
g_h->g_hook.
g_c->g_call.
g_p->g_patch.
g_v->g_value.
*/

////////////////////////////////////
//hooks
pt_cdword	g_h_before_init_game_map_ = 0X004C1AD6;
pt_cdword	g_h_after_init_game_map_ = 0X002B9ECC;
pt_cdword	g_h_skill_obj_update_mouse_pos_ = 0X004C6ABB;
pt_cdword	g_h_hfac_thrd_ = 0X000EEC00;
pt_cdword	g_h_cpuid1_ = 0X0058BE1D;
pt_cdword	g_h_cpuid2_ = 0X0058BE4C;
pt_cdword	g_h_cpuid3_ = 0X0058BFA8;
pt_cdword	g_h_cpuid4_ = 0X0058BFDD;
pt_cdword	g_h_cpuid5_ = 0X008B2BF9;
pt_cdword	g_h_cpuid6_ = 0X008B2C33;
pt_cdword	g_h_cpuid7_ = 0X008B2CA9;
pt_cdword	g_h_cpuid8_ = 0X0092BE44;
pt_cdword	g_h_build_takeup_item_packet_ = 0X006D63F0;

////////////////////////////////////
//patch
pt_cdword	g_p_ret_0x1_0xf_check_packet_ = 0X004BDB80;

////////////////////////////////////
//call
pt_cdword	g_c_logic_oper_command_ = 0X004C40F0;
pt_cdword	g_c_find_GrantedEffectsPerLevelPair_ = 0X00397090;
pt_cdword	g_c_get_skill_gem_require_attr_ = 0X00340700;
pt_cdword	g_c_get_item_price_ = 0X00339AB0;
pt_cdword	g_c_malloc_ = 0X008B18E5;
pt_cdword	g_c_free_ = 0X006453F0;
pt_cdword	g_c_on_create_role_ = 0X002B2290;
pt_cdword	g_c_select_character_ = 0X0029A8F0;
pt_cdword	g_c_press_start_yi_jie_ = 0X00241A80;
pt_cdword	g_c_actor_handle_msg_ = 0X004B4F20;
pt_cdword	g_c_gen_item_format_str_ = 0X0019D280;
pt_cdword	g_c_actor_use_skill_ = 0X003D6440;
pt_cdword	g_c_login_state_press_bt_ = 0X002A7480;
pt_cdword	g_c_inside_gem_ = 0X00197600;
pt_cdword	g_c_takeoff_gem_ = 0X001976F0;
pt_cdword	g_c_use_flask_ = 0X00185F90;
pt_cdword	g_c_use_item_ = 0X00195D90;
pt_cdword	g_c_use_item_to_ = 0X004C5580;
pt_cdword	g_c_merge_item_ = 0X00196090;

////////////////////////////////////
//addr
pt_cdword	g_a_game_base_ = 0X00C49DD8;
pt_cdword	g_a_game_base_wnd_info_ = 0X00C3E06C;
pt_cdword	g_a_fn_switch_pre_login_scene_ = 0X00316F20;

////////////////////////////////////
//offset
pt_cdword	g_o_transitionable_state_ = 0X0000010C;
pt_cdword	g_o_cur_using_skill_info_ = 0X0000392C;
pt_cdword	g_o_cur_using_skill_id_ = 0X00003944;
pt_cdword	g_c_terminate_cur_using_skill_ = 0X004C5200;
pt_cdword	g_o_GrantedEffectsPerLevel_require_lvl_ = 0X0000003C;

////////////////////////////////////
//vtbl
pt_cdword	g_a_waypoint_item_vtbl_ = 0X00AD8BC0;
pt_cdword	g_a_top_main_ui_vtbl_ = 0X00AE4CA4;
pt_cdword	g_a_ui_dlg_waypoint_vtbl_ = 0X00AD9794;
pt_cdword	g_a_ui_dlg_tab_vtbl_ = 0X00AB58BC;
pt_cdword	g_a_ui_dlg_waypoint_map_vtbl_ = 0X00AD8A68;
pt_cdword	g_a_ui_bt_vtbl_ = 0X00B77650;
pt_cdword	g_va_ui_popup_menu_vtbl_ = 0X00B78838;
pt_cdword	g_va_ui_npc_menu_vtbl_ = 0X00AD2BEC;
pt_cdword	g_va_ui_npc_menu_items_container_ = 0X00B7A98C;
pt_cdword	g_va_ui_npc_menu_item_vtbl_ = 0X00B78D2C;
pt_cdword	g_va_ui_label_vtbl_ = 0X00B77D6C;
pt_cdword	g_va_ui_dlg_prop_ = 0X00AD4AA8;
pt_cdword	g_va_ui_dlg_stash_ = 0X00AD7D20;
pt_cdword	g_va_ui_dlg_passive_skill_ = 0X00AC44F4;
pt_cdword	g_va_ui_dlg_quest_award_ = 0X00AE0390;
pt_cdword	g_va_ui_dlg_guide_ = 0X00ADC1FC;
pt_cdword	g_va_ui_grid_panel_ = 0X00ACF3D4;
pt_cdword	g_va_ui_mouse_attach_item_ = 0X00ACCE0C;
pt_cdword	g_va_ui_quest_state_panel_container = 0X00AC9878;
pt_cdword	g_va_ui_quest_state_panel = 0X00AC99D0;
pt_cdword	g_va_ui_lvl_state_panel_container = 0X00ACD66C;
pt_cdword	g_va_ui_dlg_waypoint_chapter_content = 0X00AD7E88;
pt_cdword	g_va_ui_dlg_waypoint_quests_panel = 0X00AD935C;
pt_cdword	g_va_ui_dlg_waypoint_quest_info_panel = 0X00AD9040;
pt_cdword	g_va_ui_dlg_waypoint_quests_scroll_bar = 0X00B786E0;
pt_cdword	g_va_ui_dlg_waypoint_quest_item = 0X00AD94EC;
pt_cdword	g_va_ui_dlg_died_panel = 0X00AB5AE4;
pt_cdword	g_va_ui_dlg_passive_skill_warning_panel = 0X00ADDA78;
pt_cdword	g_va_ui_lvl_up_skill_gem_item = 0X00ACD324;
pt_cdword	g_va_ui_lvl_up_skill_gem_button = 0X00AD493C;
pt_cdword	g_va_ui_mini_map_ = 0X00AD1104;
pt_cdword	g_va_ui_map_ = 0X00AD13D0;
pt_cdword	g_va_ui_npc_dlg_choice2_ = 0X00ADC64C;
pt_cdword	g_va_ui_entity_desc_top_ = 0X00AE201C;
pt_cdword	g_va_ui_world_item_desc_ = 0X00AE1BD4;
pt_cdword	g_va_ui_npc_title_desc_ = 0X00AE1D6C;
pt_cdword	g_va_ui_stash_tab_pages_ = 0X00B7A544;
pt_cdword	g_va_ui_stash_page_button_ = 0X00B7A800;
pt_cdword	g_va_ui_dlg_buy_ = 0X00ADD630;
pt_cdword	g_va_ui_dlg_prop_grid_panel_ = 0X00ACF014;
pt_cdword	g_va_ui_item_grid_ = 0X00ACFE74;
pt_cdword	g_va_ui_dlg_sell_ = 0X00AE06A0;
pt_cdword	g_va_ui_sell_tab_pages_container_ = 0X00AE0EC4;
pt_cdword	g_va_ui_sell_my_items_page_ = 0X00ACEE60;
pt_cdword	g_va_ui_sell_npc_items_page_ = 0X00ACF1D4;
pt_cdword	g_va_ui_tab_page_item_ = 0X00AC89F4;
pt_cdword	g_va_ui_dlg_select_role_top_ = 0X00AD44F8;
pt_cdword	g_va_ui_select_role_list_ = 0X00AD4230;
pt_cdword	g_va_ui_role_item_ = 0X00AD4654;
pt_cdword	g_va_ui_passive_skill_top_ = 0X00AC1574;
pt_cdword	g_va_ui_flasks_grid_panel_ = 0X00ACE8BC;
pt_cdword	g_va_ui_select_area_top_ = 0X00AE7DB8;
pt_cdword	g_va_ui_create_role_name_edit_ = 0X00B78B74;
pt_cdword	g_va_ui_caller_buff_ = 0X00AD1AC8;
pt_cdword	g_va_ui_buff_item_ = 0X00AD1C20;
pt_cdword	g_va_skill_attr_obj_ = 0X00ADAE2C;
pt_cdword	g_va_dlg_map_locator_ = 0X00ADFC14;
pt_cdword	g_va_ui_item_tips_ = 0X00AB9778;
pt_cdword	g_va_ui_item_tips_up_ = 0X00ACFFD0;
pt_cdword	g_va_ui_bag_flask_ = 0X00AD02E4;
pt_cdword	g_va_ui_bag_flasks_ = 0X00ACE708;
pt_cdword	g_va_ui_bag_flasks_up_ = 0X00ACCB78;
pt_cdword	g_va_msg_receive_obj_vtbl_ = 0X00B39478;
pt_cdword	g_va_ui_stash_multi_container_ = 0X00AC8264;
pt_cdword	g_va_ui_stash_multi_grid_panel_item_ = 0X00AB8018;
pt_cdword	g_va_ui_check_box_ = 0X00B79204;
pt_cdword	g_va_ui_dlg_self_map_locator_ = 0X00ADE034;
pt_cdword	g_va_ui_map_attr_list_ = 0X00ADDE98;
pt_cdword	g_va_ui_dlg_self_map_locator_grid_panel_ = 0X00ACF014;
pt_cdword	g_va_ui_hp_mp_circle_ = 0X00AD3174;
pt_cdword	g_va_ui_hp_mp_circle_value_ = 0X00AD32CC;

////////////////////////////////////
//net
pt_cdword	g_c_SendPacket_ = 0X00382520;
pt_cdword	g_o_send_msg_total_len_ = 0X000000B4;
pt_cdword	g_c_BuildPacket_ = 0X00382400;	//紧挨着的上面那个函数就是读取收包字段内容的函数，即read，而这个函数是write。
pt_cdword	g_o_game_session_packet_ = 0X00003898;
pt_cdword	g_c_scene_holder_vtbl_fn_ = 0X00290030;
pt_cdword	g_h_recv_packet_read_ = 0X00662750;
pt_cdword	g_h_AnalysisRecvPacket_ = 0X00662634;
pt_cdword	g_o_send_packet_encrypt_ = 0X00000058;
pt_cdword	g_c_encrypt_int_ = 0X006D6910;
pt_cdword	g_c_encrypt_byte_ = 0X006D69A0;
pt_cdword	g_c_encrypt_int1_ = 0X006D6800;
pt_cdword	g_c_encrypt_byte1_ = 0X006D6890;
pt_cdword	g_h_recv_on_move_ = 0X006D6CA5;

////////////////////////////////////
//role
pt_cdword	g_o_get_entity_component_ = 0X00000144;

////////////////////////////////////
//item
pt_cdword	g_o_logic_get_bag_ = 0X00003AEC;
pt_cdword	g_o_bag_item_cont_ = 0X00000084;
pt_cdword	g_o_entity_attr_mods_player_lvl_ = 0X0000011C;
pt_cdword	g_o_item_base_is_bind_ = 0X00000082;
pt_cdword	g_o_item_tips_param_item_lvl_ = 0X000001E4;

////////////////////////////////////
//npc
pt_cdword	g_o_entity_attr_npc_is_locked_ = 0X00000110;

////////////////////////////////////
//map
pt_cdword	g_o_world2map_data_ = 0X00000254;
pt_cdword	g_c_get_map_data_byte_ = 0X00098AA0;
pt_cdword	g_c_player_move_ = 0X003F2650;
pt_cdword	g_o_map_obj_btree_ = 0X000001A8;

////////////////////////////////////
//Skill
pt_cdword	g_o_life_get_buff_ = 0X00000094;
pt_cdword	g_o_get_passive_skill_ = 0X000038C4;
pt_cdword	g_o_regret_skill_point_ = 0X00000048;
pt_cdword	g_o_get_down_inventory_mgr_ = 0X000018E0;
pt_cdword	g_o_get_inventory_skill_mgr_ = 0X0000096C;
pt_cdword	g_o_inventory_skill_item_array_ = 0X00000908;
pt_cdword	g_o_get_inventory_skill_item_info_ = 0X00000930;
pt_cdword	g_o_get_cur_skill_type_info_ = 0X00001568;
pt_cdword	g_a_skill_id_addr_ = 0X00C6031C;
pt_cdword	g_o_get_my_skills_ = 0X000008BC;
pt_cdword	g_o_actor_get_skill_mgr_ = 0X00000230;
pt_cdword	g_c_my_skill_mgr_base_fn_tag_ = 0X00219F40;
pt_cdword	g_o_get_my_skill_mgr_ = -0X000008B0;

////////////////////////////////////
//Task
pt_cint		g_v_branch_quest_shl_deta_ = 0X00000007;
pt_cint		g_v_branch_quest_delta_offset_ = -0X00000030;
pt_cdword	g_c_touch_target_obj_ = 0X004C4EC0;
pt_cdword	g_c_close_npc_ui_ = 0X0026E770;
pt_cdword	g_c_player_quest_info_vfn_ = 0X00201AF0;
pt_cdword	g_o_cur_quest_info_ = 0X000002D4;
pt_cdword	g_o_dlg_waypoint_quest_item_db_quest_ = 0X000008B0;
pt_cdword	g_c_find_quest_state_ = 0X003D43C0;

////////////////////////////////////
//db
pt_cdword	g_a_db_base_addr_ = 0X00DB40B0;
pt_cdword	g_c_find_db_file_ = 0X00304D70;

////////////////////////////////////
//ui
pt_cdword	g_o_in_game_state2ui_mgr_ = 0X0000015C;
pt_cdword	g_o_ui_mgr_top2ui_mgr_ = 0X00000880;
pt_cdword	g_c_main_ui_obj_handle_logic_ = 0X000B71C0;
pt_cdword	g_c_ui_logic_handle_esc_key_ = 0X0026BD90;
pt_cdword	g_o_main_ui_get_in_game_state_ = 0X000008C4;
pt_cdword	g_c_switch_to_escape_state_ = 0X002C0A10;
pt_cdword	g_c_escape_press_button_ = 0X002B39B0;
pt_cdword	g_o_ui_waypoint_connections_num_ = 0X000009D4;
pt_cdword	g_o_ui_mgr_top_get_dlg_ui_mgr_ = 0X00000AB8;
pt_cdword	g_c_main_ui_obj_handle_logic_ret_c_ = 0X000C49D0;
pt_cdword	g_o_ui_obj_base_is_shown_ = 0X00000754;
pt_cdword	g_o_dlg_waypoint_map_area_type_ = 0X000008C8;
pt_cdword	g_o_waypoint_item_map_pin_ = 0X000008EC;
pt_cdword	g_c_waypoint_map_change_map_ = 0X00203750;
pt_cdword	g_c_map_pins_get_waypoint_world_area_ = 0X001FECF0;
pt_cdword	g_c_ui_waypoint_map_discovered_map_ = 0X001FED40;
pt_cdword	g_o_ui_waypoint_map_ukn_obj_ukn_obj_ = 0X00003D50;
pt_cdword	g_c_is_the_waypoint_opened_ = 0X00404B90;
pt_cdword	g_o_dlg_waypoint_map_ukn_obj_ = 0X000008BC;
pt_cdword	g_c_ui_npc_panel_press_menu_item_ = 0X001B4E50;
pt_cdword	g_o_ui_lable_str_info_ = 0X00000AC0;
pt_cdword	g_o_ui_npc_panel_items_container_items_info_ = 0X00000C08;
pt_cdword	g_o_ui_npc_panel_items_info_ = 0X000008E8;
pt_cdword	g_o_ui_grid_panel_get_inventory_ = 0X00000950;
pt_cdword	g_c_ui_mouse_attach_item_get_item_ = 0X00186FD0;
pt_cdword	g_o_ui_grid_panel_inventory_mgr_ = 0X000008B8;
pt_cdword	g_c_can_put_it_to_the_inventory_ = 0X003E6CC0;
pt_cdword	g_c_revive_ = 0X00272750;
pt_cdword	g_o_main_ui_died_panel_ = 0X00000AD4;
pt_cdword	g_o_main_ui2ui_your_passive_skill_ = 0X00000AB8;
pt_cdword	g_o_main_ui2ui_dark_panel_ = 0X00000A48;
pt_cdword	g_c_show_hide_ui_ = 0X005E9550;
pt_cdword	g_c_lvl_up_skill_gem_ = 0X00187A80;
pt_cdword	g_c_can_lvl_up_skill_gem_ = 0X000CBA10;
pt_cdword	g_c_ui_npc_dlg_choice2_choose_ = 0X00227090;
pt_cdword	g_o_ui_npc_dlg_choice_help_bt_ = 0X000008D0;
pt_cdword	g_c_ui_stash_press_switch_page_ = 0X00628D80;
pt_cdword	g_o_ui_stash_pages_info_ = 0X00000AD0;
pt_cdword	g_o_ui_item_grid_item_id_ = 0X00000A58;
pt_cdword	g_c_ui_sell_press_button_ = 0X0024D040;
pt_cdword	g_c_ui_enter_game_ = 0X001CD580;
pt_cdword	g_o_ui_bt_begin_game_ = 0X00000D70;
pt_cdword	g_o_ui_role_list_items_ = 0X00000AF8;
pt_cdword	g_o_ui_role_list_item_name_ = 0X00000904;
pt_cdword	g_o_passive_skill_graph_mgr_ = 0X000009BC;
pt_cdword	g_c_press_create_role_buttons_ = 0X00299C30;
pt_cdword	g_o_CreateCharacterState_cancel_bt_ = 0X00000088;
pt_cdword	g_v_game_area_size_ = 0X00000007;
pt_cdword	g_o_edit_string_ = 0X000008B0;
pt_cdword	g_o_edit_string2string_ = 0X00000218;
pt_cdword	g_o_caller_buff_item2entities_ = 0X000008C8;
pt_cdword	g_o_caller_buff_items_ = 0X000008C0;
pt_cdword	g_o_ui_bottom_flask_tips_ = 0X00000A38;
pt_cdword	g_o_LvlUpSkillGemItem_item_ = 0X000008B4;
pt_cdword	g_c_switch_check_box_ = 0X006198A0;
pt_cdword	g_o_ui_check_box_enable_ = 0X00000A64;
pt_cdword	g_c_press_goto_hide_pos_ = 0X00204850;
pt_cdword	g_c_can_press_goto_hide_pos_ = 0X00203D10;
pt_cdword	g_c_select_map_locator_attr_ = 0X001CCF10;
pt_cdword	g_o_map_attr_list_items_ = 0X00000AF8;
pt_cdword	g_c_self_map_locator_press_button_ = 0X002329D0;
pt_cdword	g_o_self_map_locator_bt_start_ = 0X000008D4;
pt_cdword	g_o_pre_in_game_state_dlg_modal_ = 0X00000B84;
pt_cdword	g_o_dlg_interaction_sub_ui_ = 0X000008C0;
pt_cdword	g_o_role_list_ = 0X00000D9C;
pt_cdword	g_o_hp_mp_circle_value_cur_value_ = 0X000008E0;

////////////////////////////////////
//other
pt_cdword	g_c_ui_create_item_tips_ = 0X0019D360;
pt_cdword	g_o_items_tips2item_ = 0X000008CC;
pt_cdword	g_a_item_tips_params_ = 0X00C604A0;

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
pt_cdword	g_h_before_init_game_map_ = 0X0055B866;
pt_cdword	g_h_after_init_game_map_ = 0X00330276;
pt_cdword	g_h_skill_obj_update_mouse_pos_ = 0X00560E7B;
pt_cdword	g_h_disable_client_switching_ = 0X0036721C;
pt_cdword	g_h_hfac_thrd_ = 0X00107EF0;
pt_cdword	g_h_cpuid1_ = 0X00643A20;
pt_cdword	g_h_cpuid2_ = 0X00643A55;
pt_cdword	g_h_cpuid3_ = 0X00643BAC;
pt_cdword	g_h_cpuid4_ = 0X00643BE5;
pt_cdword	g_h_cpuid5_ = 0X00A80B39;
pt_cdword	g_h_cpuid6_ = 0X00A80B73;
pt_cdword	g_h_cpuid7_ = 0X00A80BE9;
pt_cdword	g_h_cpuid8_ = 0X00AF9DD4;
pt_cdword	g_h_build_takeup_item_packet_ = 0X00813AD0;
pt_cdword	g_h_is_collisioned_ = 0X00455C80;
pt_cdword	g_h_is_in_collision_range_ = 0X004B7C50;
pt_cdword	g_h_map_data_modify_by_entity_ = 0X0045D750;
pt_cdword	g_h_modify_life_data_ = 0X0044FA9A;
pt_cdword	g_h_prompt_info_ = 0X0057F23E;
pt_cdword	g_h_duo_kai_two_copies_ = 0X00367100;

////////////////////////////////////
//patch
pt_cdword	g_p_ret_0x1_0xf_check_packet_ = 0X005573E0;

////////////////////////////////////
//call
pt_cdword	g_c_logic_oper_command_ = 0X0055E380;
pt_cdword	g_c_find_GrantedEffectsPerLevelPair_ = 0X00421C30;
pt_cdword	g_c_get_skill_gem_require_attr_ = 0X003C1210;
pt_cdword	g_c_get_item_price_ = 0X003B8600;
pt_cdword	g_c_malloc_ = 0X00A7F9AB;
pt_cdword	g_c_free_ = 0X006F73E0;
pt_cdword	g_c_on_create_role_ = 0X00327B90;
pt_cdword	g_c_select_character_ = 0X00314FA0;
pt_cdword	g_c_press_start_yi_jie_ = 0X00299130;
pt_cdword	g_c_actor_handle_msg_ = 0X0054DEA0;
pt_cdword	g_c_gen_item_format_str_ = 0X001E5D70;
pt_cdword	g_c_actor_use_skill_ = 0X004652C0;
pt_cdword	g_c_login_state_press_bt_ = 0X0031CE10;
pt_cdword	g_c_inside_gem_ = 0X001DF4C0;
pt_cdword	g_c_takeoff_gem_ = 0X001DF610;
pt_cdword	g_c_use_flask_ = 0X001CC880;
pt_cdword	g_c_use_item_ = 0X001DD970;
pt_cdword	g_c_use_item_to_ = 0X0055F8B0;
pt_cdword	g_c_merge_item_ = 0X001DDC70;
pt_cdword	g_c_skip_all_tutorials_ = 0X002D4800;
pt_cdword	g_c_select_map_instance_ = 0X0028E690;

////////////////////////////////////
//addr
pt_cdword	g_a_game_base_ = 0X00E71670;
pt_cdword	g_a_game_base_wnd_info_ = 0X00E644B8;

////////////////////////////////////
//offset
pt_cdword	g_o_transitionable_state_ = 0X0000010C;
pt_cdword	g_o_cur_using_skill_info_ = 0X0000422C;
pt_cdword	g_o_cur_using_skill_id_ = 0X00004244;
pt_cdword	g_c_terminate_cur_using_skill_ = 0X0055F530;
pt_cdword	g_o_GrantedEffectsPerLevel_require_lvl_ = 0X00000064;
pt_cdword	g_o_game_logic_ = 0X00000144;
pt_cdword	g_o_is_moving_ = 0X00000480;
pt_cdword	g_o_ui_select_role_top_ = 0X00000034;

////////////////////////////////////
//vtbl
pt_cdword	g_a_waypoint_item_vtbl_ = 0X00CC1E04;
pt_cdword	g_a_top_main_ui_vtbl_ = 0X00CCEAF4;
pt_cdword	g_a_ui_dlg_waypoint_vtbl_ = 0X00CC29F0;
pt_cdword	g_a_ui_dlg_tab_vtbl_ = 0X00CA0468;
pt_cdword	g_a_ui_dlg_waypoint_map_vtbl_ = 0X00CC1C70;
pt_cdword	g_a_ui_bt_vtbl_ = 0X00D7E5B4;
pt_cdword	g_va_ui_popup_menu_vtbl_ = 0X00D7E294;
pt_cdword	g_va_ui_npc_menu_vtbl_ = 0X00CBB7F4;
pt_cdword	g_va_ui_npc_menu_items_container_ = 0X00D817FC;
pt_cdword	g_va_ui_npc_menu_item_vtbl_ = 0X00D7FB88;
pt_cdword	g_va_ui_label_vtbl_ = 0X00D7ECE4;
pt_cdword	g_va_ui_dlg_prop_ = 0X00CBDCCC;
pt_cdword	g_va_ui_dlg_stash_ = 0X00CC0EA8;
pt_cdword	g_va_ui_dlg_passive_skill_ = 0X00CAD328;
pt_cdword	g_va_ui_dlg_quest_award_ = 0X00CC9A58;
pt_cdword	g_va_ui_dlg_guide_ = 0X00CC52CC;
pt_cdword	g_va_ui_grid_panel_ = 0X00CB808C;
pt_cdword	g_va_ui_mouse_attach_item_ = 0X00CB5C10;
pt_cdword	g_va_ui_quest_state_panel_container = 0X00CB24FC;
pt_cdword	g_va_ui_quest_state_panel = 0X00CB2654;
pt_cdword	g_va_ui_lvl_state_panel_container = 0X00CB6440;
pt_cdword	g_va_ui_dlg_waypoint_chapter_content = 0X00CC1010;
pt_cdword	g_va_ui_dlg_waypoint_quests_panel = 0X00CC25F0;
pt_cdword	g_va_ui_dlg_waypoint_quest_info_panel = 0X00CC22CC;
pt_cdword	g_va_ui_dlg_waypoint_quests_scroll_bar = 0X00D7F674;
pt_cdword	g_va_ui_dlg_waypoint_quest_item = 0X00CC2748;
pt_cdword	g_va_ui_dlg_died_panel = 0X00C9B3A4;
pt_cdword	g_va_ui_dlg_passive_skill_warning_panel = 0X00CC6974;
pt_cdword	g_va_ui_lvl_up_skill_gem_item = 0X00CB612C;
pt_cdword	g_va_ui_lvl_up_skill_gem_button = 0X00CBDB6C;
pt_cdword	g_va_ui_mini_map_ = 0X00CB9DC0;
pt_cdword	g_va_ui_map_ = 0X00CBA088;
pt_cdword	g_va_ui_npc_dlg_choice2_ = 0X00CC5768;
pt_cdword	g_va_ui_entity_desc_top_ = 0X00CCB98C;
pt_cdword	g_va_ui_world_item_desc_ = 0X00CCB564;
pt_cdword	g_va_ui_npc_title_desc_ = 0X00CCB754;
pt_cdword	g_va_ui_stash_tab_pages_ = 0X00D813B4;
pt_cdword	g_va_ui_stash_page_button_ = 0X00D81670;
pt_cdword	g_va_ui_dlg_buy_ = 0X00CC6504;
pt_cdword	g_va_ui_dlg_prop_grid_panel_ = 0X00CB7CD4;
pt_cdword	g_va_ui_item_grid_ = 0X00CB8B2C;
pt_cdword	g_va_ui_dlg_sell_ = 0X00CCA078;
pt_cdword	g_va_ui_sell_tab_pages_container_ = 0X00CCA890;
pt_cdword	g_va_ui_sell_my_items_page_ = 0X00CB7B14;
pt_cdword	g_va_ui_sell_npc_items_page_ = 0X00CB7E94;
pt_cdword	g_va_ui_tab_page_item_ = 0X00CB0FD8;
pt_cdword	g_va_ui_dlg_select_role_top_ = 0X00CBCDAC;
pt_cdword	g_va_ui_select_role_list_ = 0X00CBCAE4;
pt_cdword	g_va_ui_role_item_ = 0X00CBCF08;
pt_cdword	g_va_ui_passive_skill_top_ = 0X00CA9684;
pt_cdword	g_va_ui_flasks_grid_panel_ = 0X00CB74B8;
pt_cdword	g_va_ui_select_area_top_ = 0X00CD2C3C;
pt_cdword	g_va_ui_create_role_name_edit_ = 0X00D7F9B8;
pt_cdword	g_va_ui_caller_buff_ = 0X00CBA71C;
pt_cdword	g_va_ui_buff_item_ = 0X00CBA880;
pt_cdword	g_va_skill_attr_obj_ = 0X00CC3F00;
pt_cdword	g_va_dlg_map_locator_ = 0X00CC8FB8;
pt_cdword	g_va_ui_item_tips_ = 0X00C9F1A0;
pt_cdword	g_va_ui_item_tips_up_ = 0X00CB8C88;
pt_cdword	g_va_ui_bag_flask_ = 0X00CB8F9C;
pt_cdword	g_va_ui_bag_flasks_ = 0X00CB7304;
pt_cdword	g_va_ui_bag_flasks_up_ = 0X00CB5930;
pt_cdword	g_va_msg_receive_obj_vtbl_ = 0X00D3D4C4;
pt_cdword	g_va_ui_stash_multi_container_ = 0X00CB0884;
pt_cdword	g_va_ui_stash_multi_grid_panel_item_ = 0X00C9DA48;
pt_cdword	g_va_ui_check_box_ = 0X00D80068;
pt_cdword	g_va_ui_dlg_self_map_locator_ = 0X00CC6F38;
pt_cdword	g_va_ui_map_attr_list_ = 0X00CC6D8C;
pt_cdword	g_va_ui_dlg_self_map_locator_grid_panel_ = 0X00C9E224;
pt_cdword	g_va_ui_hp_mp_circle_ = 0X00CBBDB4;
pt_cdword	g_va_ui_hp_mp_circle_value_ = 0X00CBBF0C;
pt_cdword	g_va_ui_tutorials_ = 0X00CC52CC;
pt_cdword	g_va_ui_dlg_labyrinth_select_ = 0X00CC8ABC;
pt_cdword	g_va_ui_dlg_trial_plaque_ = 0X00CC61F8;
pt_cdword	g_va_ui_instance_mgr_ = 0X00CC79AC;

////////////////////////////////////
//net
pt_cdword	g_c_SendPacket_ = 0X004114F0;
pt_cdword	g_o_send_msg_total_len_ = 0X0000011C;
pt_cdword	g_c_BuildPacket_ = 0X004113D0;	//紧挨着的上面那个函数就是读取收包字段内容的函数，即read，而这个函数是write。
pt_cdword	g_o_game_session_packet_ = 0X00004198;
pt_cdword	g_c_scene_holder_vtbl_fn_ = 0X0030ACB0;
pt_cdword	g_h_recv_packet_read_ = 0X00725ABD;
pt_cdword	g_h_AnalysisRecvPacket_ = 0X007259A4;
pt_cdword	g_o_send_packet_encrypt_ = 0X00000058;
pt_cdword	g_c_encrypt_int_ = 0X00813FF0;
pt_cdword	g_c_encrypt_byte_ = 0X00814080;
pt_cdword	g_c_encrypt_int1_ = 0X00813EE0;
pt_cdword	g_c_encrypt_byte1_ = 0X00813F70;
pt_cdword	g_h_recv_on_move_ = 0X00814385;

////////////////////////////////////
//role
pt_cdword	g_o_get_entity_component_ = 0X00000158;

////////////////////////////////////
//item
pt_cdword	g_o_logic_get_bag_ = 0X000043EC;
pt_cdword	g_o_bag_item_cont_ = 0X00000084;
pt_cdword	g_o_entity_attr_mods_player_lvl_ = 0X0000011C;
pt_cdword	g_o_item_base_is_bind_ = 0X00000082;
pt_cdword	g_o_item_tips_param_item_lvl_ = 0X000001E4;

////////////////////////////////////
//npc
pt_cdword	g_o_entity_attr_npc_is_locked_ = 0X00000110;

////////////////////////////////////
//map
pt_cdword	g_o_world2map_data_ = 0X00000260;
pt_cdword	g_c_get_map_data_byte_ = 0X0045C830;
pt_cdword	g_c_player_move_ = 0X00481510;
pt_cdword	g_o_map_obj_btree_ = 0X000001B0;

////////////////////////////////////
//Skill
pt_cdword	g_o_life_get_buff_ = 0X0000009C;
pt_cdword	g_o_get_passive_skill_ = 0X000041C4;
pt_cdword	g_o_regret_skill_point_ = 0X00000048;
pt_cdword	g_o_get_down_inventory_mgr_ = 0X000018DC;
pt_cdword	g_o_get_inventory_skill_mgr_ = 0X000009BC;
pt_cdword	g_o_inventory_skill_item_array_ = 0X00000938;
pt_cdword	g_o_get_inventory_skill_item_info_ = 0X00000954;
pt_cdword	g_o_get_cur_skill_type_info_ = 0X00001570;
pt_cdword	g_a_skill_id_addr_ = 0X00E885B4;
pt_cdword	g_o_get_my_skills_ = 0X000008E0;
pt_cdword	g_o_actor_get_skill_mgr_ = 0X00000234;
pt_cdword	g_c_my_skill_mgr_base_fn_tag_ = 0X0026BB70;
pt_cdword	g_o_get_my_skill_mgr_ = -0X000008D4;

////////////////////////////////////
//Task
pt_cdword	g_c_touch_target_obj_ = 0X0055F1F0;
pt_cdword	g_c_close_npc_ui_ = 0X002DB020;
pt_cdword	g_c_player_quest_info_vfn_ = 0X00252D50;
pt_cdword	g_o_cur_quest_info_ = 0X00000230;
pt_cdword	g_o_dlg_waypoint_quest_item_db_quest_ = 0X000008D4;
pt_cdword	g_c_find_quest_state_ = 0X004625C0;

////////////////////////////////////
//db
pt_cdword	g_a_db_base_addr_ = 0X00FE46F0;
pt_cdword	g_c_find_db_file_ = 0X00388210;

////////////////////////////////////
//ui
pt_cdword	g_o_in_game_state2ui_mgr_ = 0X00000164;
pt_cdword	g_o_ui_mgr_top2ui_mgr_ = 0X00000880;
pt_cdword	g_c_main_ui_obj_handle_logic_ = 0X000CD170;
pt_cdword	g_c_ui_logic_handle_esc_key_ = 0X002D7D10;
pt_cdword	g_o_main_ui_get_in_game_state_ = 0X000008E8;
pt_cdword	g_c_switch_to_escape_state_ = 0X00336530;
pt_cdword	g_c_escape_press_button_ = 0X00329420;
pt_cdword	g_o_ui_waypoint_connections_num_ = 0X00000A1C;
pt_cdword	g_o_ui_mgr_top_get_dlg_ui_mgr_ = 0X00000AB8;
pt_cdword	g_c_main_ui_obj_handle_logic_ret_c_ = 0X000DAF80;
pt_cdword	g_o_ui_obj_base_is_shown_ = 0X00000754;
pt_cdword	g_o_waypoint_item_map_pin_ = 0X00000940;
pt_cdword	g_c_waypoint_map_change_map_ = 0X00254720;
pt_cdword	g_c_map_pins_get_waypoint_world_area_ = 0X000BF790;
pt_cdword	g_c_ui_waypoint_map_discovered_map_ = 0X001A1650;
pt_cdword	g_o_ui_waypoint_map_ukn_obj_ukn_obj_ = 0X00004650;
pt_cdword	g_c_is_the_waypoint_opened_ = 0X0048FA50;
pt_cdword	g_o_dlg_waypoint_map_ukn_obj_ = 0X000008E0;
pt_cdword	g_c_ui_npc_panel_press_menu_item_ = 0X001FE5A0;
pt_cdword	g_o_ui_lable_str_info_ = 0X00000B00;
pt_cdword	g_o_ui_npc_panel_items_container_items_info_ = 0X00000C2C;
pt_cdword	g_o_ui_npc_panel_items_info_ = 0X00000914;
pt_cdword	g_o_ui_grid_panel_get_inventory_ = 0X00000978;
pt_cdword	g_c_ui_mouse_attach_item_get_item_ = 0X001CD9D0;
pt_cdword	g_o_ui_grid_panel_inventory_mgr_ = 0X000008DC;
pt_cdword	g_c_can_put_it_to_the_inventory_ = 0X00476AB0;
pt_cdword	g_c_revive_ = 0X002DF090;
pt_cdword	g_o_main_ui_died_panel_ = 0X00000B64;
pt_cdword	g_o_main_ui2ui_your_passive_skill_ = 0X00000B40;
pt_cdword	g_o_main_ui2ui_dark_panel_ = 0X00000AAC;
pt_cdword	g_c_show_hide_ui_ = 0X006B7A60;
pt_cdword	g_c_lvl_up_skill_gem_ = 0X001CE4E0;
pt_cdword	g_c_can_lvl_up_skill_gem_ = 0X000E26B0;
pt_cdword	g_c_ui_npc_dlg_choice2_choose_ = 0X00278C30;
pt_cdword	g_o_ui_npc_dlg_choice_help_bt_ = 0X000008F4;
pt_cdword	g_c_ui_stash_press_switch_page_ = 0X006EAEA0;
pt_cdword	g_o_ui_stash_pages_info_ = 0X00000AF4;
pt_cdword	g_o_ui_item_grid_item_id_ = 0X00000A80;
pt_cdword	g_c_ui_sell_press_button_ = 0X002A9BE0;
pt_cdword	g_c_ui_enter_game_ = 0X002172B0;
pt_cdword	g_o_ui_bt_begin_game_ = 0X00000E28;
pt_cdword	g_o_ui_role_list_items_ = 0X00000B1C;
pt_cdword	g_o_ui_role_list_item_name_ = 0X00000928;
pt_cdword	g_o_passive_skill_graph_mgr_ = 0X000009DC;
pt_cdword	g_c_press_create_role_buttons_ = 0X00315280;
pt_cdword	g_o_CreateCharacterState_cancel_bt_ = 0X00000084;
pt_cdword	g_v_game_area_size_ = 0X00000007;
pt_cdword	g_o_edit_string_ = 0X000008D4;
pt_cdword	g_o_edit_string2string_ = 0X00000234;
pt_cdword	g_o_caller_buff_item2entities_ = 0X000008EC;
pt_cdword	g_o_caller_buff_items_ = 0X000008E4;
pt_cdword	g_o_ui_bottom_flask_tips_ = 0X00000A5C;
pt_cdword	g_o_LvlUpSkillGemItem_item_ = 0X000008D8;
pt_cdword	g_c_switch_check_box_ = 0X006D9940;
pt_cdword	g_o_ui_check_box_enable_ = 0X00000AC4;
pt_cdword	g_c_press_goto_hide_pos_ = 0X00256020;
pt_cdword	g_c_can_press_goto_hide_pos_ = 0X00254CC0;
pt_cdword	g_c_select_map_locator_attr_ = 0X00216F00;
pt_cdword	g_o_map_attr_list_items_ = 0X00000B1C;
pt_cdword	g_c_self_map_locator_press_button_ = 0X00285120;
pt_cdword	g_o_self_map_locator_bt_start_ = 0X000008F8;
pt_cdword	g_o_pre_in_game_state_dlg_modal_ = 0X00000B84;
pt_cdword	g_o_dlg_interaction_sub_ui_ = 0X000008E4;
pt_cdword	g_o_role_list_ = 0X00000E58;
pt_cdword	g_o_hp_mp_circle_value_cur_value_ = 0X00000904;
pt_cdword	g_c_active_labyrinth_ = 0X00294C40;
pt_cdword	g_o_labyrinth_select_list_ = 0X000008F0;
pt_cdword	g_c_select_labyrinth_ = 0X00216F00;
pt_cdword	g_o_labyrinth_list_items_ = 0X00000B1C;
pt_cdword	g_o_labyrinth_item2data_labyrinth_ = 0X00000994;
pt_cdword	g_o_ui_contianer_pic_info_ = 0X0000087C;
pt_cdword	g_o_TrialPlaque2Labyrinths_ = 0X000008F0;

////////////////////////////////////
//other
pt_cdword	g_c_ui_create_item_tips_ = 0X001E5E50;
pt_cdword	g_o_items_tips2item_ = 0X000008F0;
pt_cdword	g_a_item_tips_params_ = 0X00E88988;

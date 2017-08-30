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
pt_cdword	g_h_before_init_game_map_ = 0X0058CA6E;
pt_cdword	g_h_after_init_game_map_ = 0X0035E796;
pt_cdword	g_h_skill_obj_update_mouse_pos_ = 0X00592848;
pt_cdword	g_h_hfac_thrd_ = 0X0010CC00;
pt_cdword	g_h_cpuid1_ = 0X0066C8D0;
pt_cdword	g_h_cpuid2_ = 0X0066C8FF;
pt_cdword	g_h_cpuid3_ = 0X0066CA5C;
pt_cdword	g_h_cpuid4_ = 0X0066CA91;
pt_cdword	g_h_cpuid5_ = 0X00AA6439;
pt_cdword	g_h_cpuid6_ = 0X00AA6473;
pt_cdword	g_h_cpuid7_ = 0X00AA64E9;
pt_cdword	g_h_cpuid8_ = 0X00B1F774;
pt_cdword	g_h_build_takeup_item_packet_ = 0X005A9C80;
pt_cdword	g_h_is_collisioned_ = 0X00486B60;
pt_cdword	g_h_is_in_collision_range_ = 0X004E5E70;
pt_cdword	g_h_map_data_modify_by_entity_ = 0X0048E630;
pt_cdword	g_h_modify_life_data_ = 0X0048097A;
pt_cdword	g_h_prompt_info_ = 0X005B46CE;
pt_cdword	g_h_duo_kai_two_copies_ = 0X00395970;

////////////////////////////////////
//patch
pt_cdword	g_p_ret_0x1_0xf_check_packet_ = 0X00587B30;

////////////////////////////////////
//call
pt_cdword	g_c_logic_oper_command_ = 0X0058F690;
pt_cdword	g_c_find_GrantedEffectsPerLevelPair_ = 0X00453F70;
pt_cdword	g_c_get_skill_gem_require_attr_ = 0X003F19D0;
pt_cdword	g_c_get_item_price_ = 0X003E8DC0;
pt_cdword	g_c_malloc_ = 0X00AA52B9;
pt_cdword	g_c_free_ = 0X0071CB10;
pt_cdword	g_c_on_create_role_ = 0X00355FE0;
pt_cdword	g_c_select_character_profession_ = 0X00347CA0;
pt_cdword	g_c_press_start_yi_jie_ = 0X002C3110;
pt_cdword	g_c_actor_handle_msg_ = 0X0057E1E0;
pt_cdword	g_c_gen_item_format_str_ = 0X0020C690;
pt_cdword	g_c_actor_use_skill_ = 0X00496190;
pt_cdword	g_c_login_state_press_bt_ = 0X0034D530;
pt_cdword	g_c_inside_gem_ = 0X00205FD0;
pt_cdword	g_c_takeoff_gem_ = 0X00206130;
pt_cdword	g_c_use_flask_ = 0X001F30F0;
pt_cdword	g_c_use_item_ = 0X002042F0;
pt_cdword	g_c_use_item_to_ = 0X005910B0;
pt_cdword	g_c_merge_item_ = 0X00204650;
pt_cdword	g_c_skip_all_tutorials_ = 0X002FF800;
pt_cdword	g_c_select_map_instance_ = 0X002B8670;

////////////////////////////////////
//addr
pt_cdword	g_a_game_base_ = 0X00E9C638;
pt_cdword	g_a_game_base_wnd_info_ = 0X00E8F4A4;

////////////////////////////////////
//offset
pt_cdword	g_o_transitionable_state_ = 0X0000010C;
pt_cdword	g_o_cur_using_skill_info_ = 0X000044AC;
pt_cdword	g_o_cur_using_skill_id_ = 0X000044C4;
pt_cdword	g_c_terminate_cur_using_skill_ = 0X00590BA0;
pt_cdword	g_o_GrantedEffectsPerLevel_require_lvl_ = 0X00000064;
pt_cdword	g_o_game_logic_ = 0X00000144;
pt_cdword	g_o_is_moving_ = 0X00000480;
pt_cdword	g_o_ui_select_role_top_ = 0X00000034;

////////////////////////////////////
//vtbl
pt_cdword	g_a_waypoint_item_vtbl_ = 0X00CEC548;
pt_cdword	g_a_top_main_ui_vtbl_ = 0X00CF9B30;
pt_cdword	g_a_ui_dlg_waypoint_vtbl_ = 0X00CED1B0;
pt_cdword	g_a_ui_dlg_tab_vtbl_ = 0X00CC2C24;
pt_cdword	g_a_ui_dlg_waypoint_map_vtbl_ = 0X00CEC3F0;
pt_cdword	g_a_ui_bt_vtbl_ = 0X00DA78AC;
pt_cdword	g_va_ui_popup_menu_vtbl_ = 0X00DA758C;
pt_cdword	g_va_ui_npc_menu_vtbl_ = 0X00CE638C;
pt_cdword	g_va_ui_npc_menu_items_container_ = 0X00DAABA0;
pt_cdword	g_va_ui_npc_menu_item_vtbl_ = 0X00DA9000;
pt_cdword	g_va_ui_label_vtbl_ = 0X00DA81A4;
pt_cdword	g_va_ui_dlg_prop_ = 0X00CE8708;
pt_cdword	g_va_ui_dlg_stash_ = 0X00CEB5F8;
pt_cdword	g_va_ui_dlg_passive_skill_ = 0X00CD5384;
pt_cdword	g_va_ui_dlg_quest_award_ = 0X00CF43F0;
pt_cdword	g_va_ui_dlg_guide_ = 0X00CEFB38;
pt_cdword	g_va_ui_grid_panel_ = 0X00CE2C8C;
pt_cdword	g_va_ui_mouse_attach_item_ = 0X00CE0808;
pt_cdword	g_va_ui_quest_state_panel_container = 0X00CDD034;
pt_cdword	g_va_ui_quest_state_panel = 0X00CDD18C;
pt_cdword	g_va_ui_lvl_state_panel_container = 0X00CE1024;
pt_cdword	g_va_ui_dlg_waypoint_chapter_content = 0X00CEB760;
pt_cdword	g_va_ui_dlg_waypoint_quests_panel = 0X00CECD5C;
pt_cdword	g_va_ui_dlg_waypoint_quest_info_panel = 0X00CECA70;
pt_cdword	g_va_ui_dlg_waypoint_quests_scroll_bar = 0X00DA8B34;
pt_cdword	g_va_ui_dlg_waypoint_quest_item = 0X00CECEF0;
pt_cdword	g_va_ui_dlg_died_panel = 0X00CC2E4C;
pt_cdword	g_va_ui_dlg_passive_skill_warning_panel = 0X00CF121C;
pt_cdword	g_va_ui_lvl_up_skill_gem_item = 0X00CE0D20;
pt_cdword	g_va_ui_lvl_up_skill_gem_button = 0X00DA7E90;
pt_cdword	g_va_ui_mini_map_ = 0X00CE4978;
pt_cdword	g_va_ui_map_ = 0X00CE4C40;
pt_cdword	g_va_ui_npc_dlg_choice2_ = 0X00CEFFD4;
pt_cdword	g_va_ui_entity_desc_top_ = 0X00CF63E0;
pt_cdword	g_va_ui_world_item_desc_ = 0X00CF5F48;
pt_cdword	g_va_ui_npc_title_desc_ = 0X00CF61A8;
pt_cdword	g_va_ui_stash_tab_pages_ = 0X00DAA760;
pt_cdword	g_va_ui_stash_page_button_ = 0X00DAAA24;
pt_cdword	g_va_ui_dlg_buy_ = 0X00CF0DE0;
pt_cdword	g_va_ui_dlg_prop_grid_panel_ = 0X00CE28D4;
pt_cdword	g_va_ui_item_grid_ = 0X00CE36FC;
pt_cdword	g_va_ui_dlg_sell_ = 0X00CF4A68;
pt_cdword	g_va_ui_sell_tab_pages_container_ = 0X00CF5288;
pt_cdword	g_va_ui_sell_my_items_page_ = 0X00CE2720;
pt_cdword	g_va_ui_sell_npc_items_page_ = 0X00CE2A94;
pt_cdword	g_va_ui_tab_page_item_ = 0X00CD9378;
pt_cdword	g_va_ui_dlg_select_role_top_ = 0X00CE7C98;
pt_cdword	g_va_ui_select_role_list_ = 0X00CE79D0;
pt_cdword	g_va_ui_role_item_ = 0X00CE7DF4;
pt_cdword	g_va_ui_passive_skill_top_ = 0X00CD1620;
pt_cdword	g_va_ui_flasks_grid_panel_ = 0X00CE208C;
pt_cdword	g_va_ui_select_area_top_ = 0X00CFE3AC;
pt_cdword	g_va_ui_create_role_name_edit_ = 0X00DA8E48;
pt_cdword	g_va_ui_caller_buff_ = 0X00CE51B0;
pt_cdword	g_va_ui_buff_item_ = 0X00CE5308;
pt_cdword	g_va_skill_attr_obj_ = 0X00CEE798;
pt_cdword	g_va_dlg_map_locator_ = 0X00CF3910;
pt_cdword	g_va_ui_item_tips_ = 0X00CC6E30;
pt_cdword	g_va_ui_item_tips_up_ = 0X00CE3858;
pt_cdword	g_va_ui_bag_flask_ = 0X00CE3B6C;
pt_cdword	g_va_ui_bag_flasks_ = 0X00CE1ED8;
pt_cdword	g_va_ui_bag_flasks_up_ = 0X00CE0528;
pt_cdword	g_va_msg_receive_obj_vtbl_ = 0X00D69D34;
pt_cdword	g_va_ui_stash_multi_container_ = 0X00CD8C2C;
pt_cdword	g_va_ui_stash_multi_grid_panel_item_ = 0X00CC53E8;
pt_cdword	g_va_ui_check_box_ = 0X00DA9488;
pt_cdword	g_va_ui_dlg_self_map_locator_ = 0X00CF183C;
pt_cdword	g_va_ui_map_attr_list_ = 0X00CF1690;
pt_cdword	g_va_ui_dlg_self_map_locator_grid_panel_ = 0X00CC5EA8;
pt_cdword	g_va_ui_hp_mp_circle_ = 0X00CE6920;
pt_cdword	g_va_ui_hp_mp_circle_value_ = 0X00CE6A78;
pt_cdword	g_va_ui_tutorials_ = 0X00CEFB38;
pt_cdword	g_va_ui_dlg_labyrinth_select_ = 0X00CF3410;
pt_cdword	g_va_ui_dlg_trial_plaque_ = 0X00CF0A68;
pt_cdword	g_va_ui_instance_mgr_ = 0X00CF2308;

////////////////////////////////////
//net
pt_cdword	g_c_SendPacket_ = 0X004438D0;
pt_cdword	g_o_send_msg_total_len_ = 0X0000011C;
pt_cdword	g_c_BuildPacket_ = 0X004437B0;	//紧挨着的上面那个函数就是读取收包字段内容的函数，即read，而这个函数是write。
pt_cdword	g_o_game_session_packet_ = 0X00004418;
pt_cdword	g_c_scene_holder_vtbl_fn_ = 0X0033DE30;
pt_cdword	g_h_recv_packet_read_ = 0X0074B3F0;
pt_cdword	g_h_AnalysisRecvPacket_ = 0X0074B2D4;
pt_cdword	g_o_send_packet_encrypt_ = 0X00000058;
pt_cdword	g_c_encrypt_int_ = 0X00837C60;
pt_cdword	g_c_encrypt_byte_ = 0X00837CF0;
pt_cdword	g_c_encrypt_int1_ = 0X00837B50;
pt_cdword	g_c_encrypt_byte1_ = 0X00837BE0;
pt_cdword	g_h_recv_on_move_ = 0X00837FF5;

////////////////////////////////////
//role
pt_cdword	g_o_get_entity_component_ = 0X000001D8;

////////////////////////////////////
//item
pt_cdword	g_o_logic_get_bag_ = 0X0000466C;
pt_cdword	g_o_bag_item_cont_ = 0X00000084;
pt_cdword	g_o_entity_attr_mods_player_lvl_ = 0X0000011C;
pt_cdword	g_o_item_base_is_bind_ = 0X00000082;
pt_cdword	g_o_item_tips_param_item_lvl_ = 0X000001E4;

////////////////////////////////////
//npc
pt_cdword	g_o_entity_attr_npc_is_locked_ = 0X00000110;

////////////////////////////////////
//map
pt_cdword	g_o_world2map_data_ = 0X000002E0;
pt_cdword	g_c_get_map_data_byte_ = 0X0048D710;
pt_cdword	g_c_player_move_ = 0X004B23D0;
pt_cdword	g_o_map_obj_btree_ = 0X00000230;

////////////////////////////////////
//Skill
pt_cdword	g_o_life_get_buff_ = 0X0000009C;
pt_cdword	g_o_get_passive_skill_ = 0X00004444;
pt_cdword	g_o_regret_skill_point_ = 0X00000048;
pt_cdword	g_o_get_down_inventory_mgr_ = 0X000018DC;
pt_cdword	g_o_get_inventory_skill_mgr_ = 0X000009BC;
pt_cdword	g_o_inventory_skill_item_array_ = 0X00000938;
pt_cdword	g_o_get_inventory_skill_item_info_ = 0X00000954;
pt_cdword	g_o_get_cur_skill_type_info_ = 0X00001570;
pt_cdword	g_a_skill_id_addr_ = 0X00EB3668;
pt_cdword	g_o_get_my_skills_ = 0X000008E0;
pt_cdword	g_o_actor_get_skill_mgr_ = 0X00000234;
pt_cdword	g_c_my_skill_mgr_base_fn_tag_ = 0X002939F0;
pt_cdword	g_o_get_my_skill_mgr_ = -0X000008D4;

////////////////////////////////////
//Task
pt_cdword	g_c_touch_target_obj_ = 0X00590870;
pt_cdword	g_c_close_npc_ui_ = 0X00307120;
pt_cdword	g_c_player_quest_info_vfn_ = 0X00279EE0;
pt_cdword	g_o_cur_quest_info_ = 0X00000230;
pt_cdword	g_o_dlg_waypoint_quest_item_db_quest_ = 0X000008D4;
pt_cdword	g_c_find_quest_state_ = 0X00493490;

////////////////////////////////////
//db
pt_cdword	g_a_db_base_addr_ = 0X01013738;
pt_cdword	g_c_find_db_file_ = 0X003B8D00;

////////////////////////////////////
//ui
pt_cdword	g_o_in_game_state2ui_mgr_ = 0X00000164;
pt_cdword	g_o_ui_mgr_top2ui_mgr_ = 0X00000880;
pt_cdword	g_c_main_ui_obj_handle_logic_ = 0X000D1290;
pt_cdword	g_c_ui_logic_handle_esc_key_ = 0X00303460;
pt_cdword	g_o_main_ui_get_in_game_state_ = 0X000008E8;
pt_cdword	g_c_switch_to_escape_state_ = 0X00364A80;
pt_cdword	g_c_escape_press_button_ = 0X00357870;
pt_cdword	g_o_ui_waypoint_connections_num_ = 0X00000A1C;
pt_cdword	g_o_ui_mgr_top_get_dlg_ui_mgr_ = 0X00000AB8;
pt_cdword	g_c_main_ui_obj_handle_logic_ret_c_ = 0X000DFDE0;
pt_cdword	g_o_ui_obj_base_is_shown_ = 0X00000754;
pt_cdword	g_o_waypoint_item_map_pin_ = 0X00000940;
pt_cdword	g_c_waypoint_map_change_map_ = 0X0027B960;
pt_cdword	g_c_map_pins_get_waypoint_world_area_ = 0X000C3930;
pt_cdword	g_c_ui_waypoint_map_discovered_map_ = 0X001C68F0;
pt_cdword	g_o_ui_waypoint_map_ukn_obj_ukn_obj_ = 0X000048D0;
pt_cdword	g_c_is_the_waypoint_opened_ = 0X004C0A80;
pt_cdword	g_o_dlg_waypoint_map_ukn_obj_ = 0X000008E0;
pt_cdword	g_c_ui_npc_panel_press_menu_item_ = 0X002250D0;
pt_cdword	g_o_ui_lable_str_info_ = 0X00000B00;
pt_cdword	g_o_ui_npc_panel_items_container_items_info_ = 0X00000C2C;
pt_cdword	g_o_ui_npc_panel_items_info_ = 0X00000914;
pt_cdword	g_o_ui_grid_panel_get_inventory_ = 0X00000978;
pt_cdword	g_c_ui_mouse_attach_item_get_item_ = 0X001F4250;
pt_cdword	g_o_ui_grid_panel_inventory_mgr_ = 0X000008DC;
pt_cdword	g_c_can_put_it_to_the_inventory_ = 0X004A7A10;
pt_cdword	g_c_revive_ = 0X0030B620;
pt_cdword	g_o_main_ui_died_panel_ = 0X00000B6C;
pt_cdword	g_o_main_ui2ui_your_passive_skill_ = 0X00000B40;
pt_cdword	g_o_main_ui2ui_dark_panel_ = 0X00000AAC;
pt_cdword	g_c_show_hide_ui_ = 0X006DD290;
pt_cdword	g_c_lvl_up_skill_gem_ = 0X001F4D60;
pt_cdword	g_c_can_lvl_up_skill_gem_ = 0X000E73E0;
pt_cdword	g_c_ui_npc_dlg_choice2_choose_ = 0X002A0C00;
pt_cdword	g_o_ui_npc_dlg_choice_help_bt_ = 0X000008F4;
pt_cdword	g_c_ui_stash_press_switch_page_ = 0X00710670;
pt_cdword	g_o_ui_stash_pages_info_ = 0X00000AF4;
pt_cdword	g_o_ui_item_grid_item_id_ = 0X00000A80;
pt_cdword	g_c_ui_sell_press_button_ = 0X002D4190;
pt_cdword	g_c_ui_enter_game_ = 0X0023DD80;
pt_cdword	g_o_ui_bt_begin_game_ = 0X00000E28;
pt_cdword	g_o_ui_role_list_items_ = 0X00000B1C;
pt_cdword	g_o_ui_role_list_item_name_ = 0X00000928;
pt_cdword	g_o_passive_skill_graph_mgr_ = 0X000009DC;
pt_cdword	g_c_press_create_role_buttons_ = 0X003478A0;
pt_cdword	g_o_CreateCharacterState_cancel_bt_ = 0X00000084;
pt_cdword	g_v_game_area_size_ = 0X00000007;
pt_cdword	g_o_edit_string_ = 0X000008D4;
pt_cdword	g_o_edit_string2string_ = 0X00000234;
pt_cdword	g_o_caller_buff_item2entities_ = 0X000008EC;
pt_cdword	g_o_caller_buff_items_ = 0X000008E4;
pt_cdword	g_o_ui_bottom_flask_tips_ = 0X00000A5C;
pt_cdword	g_o_LvlUpSkillGemItem_item_ = 0X000008D8;
pt_cdword	g_c_switch_check_box_ = 0X006FF110;
pt_cdword	g_o_ui_check_box_enable_ = 0X00000AC4;
pt_cdword	g_c_press_goto_hide_pos_ = 0X0027D260;
pt_cdword	g_c_can_press_goto_hide_pos_ = 0X0027BF00;
pt_cdword	g_c_select_map_locator_attr_ = 0X0023D9D0;
pt_cdword	g_o_map_attr_list_items_ = 0X00000B1C;
pt_cdword	g_c_self_map_locator_press_button_ = 0X002AEA90;
pt_cdword	g_o_self_map_locator_bt_start_ = 0X000008F8;
pt_cdword	g_o_pre_in_game_state_dlg_modal_ = 0X00000B84;
pt_cdword	g_o_dlg_interaction_sub_ui_ = 0X000008E4;
pt_cdword	g_o_role_list_ = 0X00000E54;
pt_cdword	g_o_hp_mp_circle_value_cur_value_ = 0X00000904;
pt_cdword	g_c_active_labyrinth_ = 0X002BEC10;
pt_cdword	g_o_labyrinth_select_list_ = 0X000008F0;
pt_cdword	g_c_select_labyrinth_ = 0X0023D9D0;
pt_cdword	g_o_labyrinth_list_items_ = 0X00000B1C;
pt_cdword	g_o_labyrinth_item2data_labyrinth_ = 0X00000994;
pt_cdword	g_o_ui_contianer_pic_info_ = 0X0000087C;
pt_cdword	g_o_TrialPlaque2Labyrinths_ = 0X000008F0;

////////////////////////////////////
//other
pt_cdword	g_c_ui_create_item_tips_ = 0X0020C770;
pt_cdword	g_o_items_tips2item_ = 0X000008F0;
pt_cdword	g_a_item_tips_params_ = 0X00EB3A48;

////////////////////////////////////
//PathOfExileBase
pt_cdword	g_h_base_sprintf1_ = 0X0012F695;
pt_cdword	g_h_base_sprintf2_ = 0X001A244F;
pt_cdword	g_h_base_sprintf3_ = 0X001F8AD8;
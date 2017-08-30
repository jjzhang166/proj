#pragma once
/*
@author zhp
@date 2017/1/24 17:22

g_so->g_stable_offset
g_o->g_offset
g_dso->g_direct_stable_offset
*/
#include <ProtocolX/P_DefinesX.h>
#include "CD_Updates.h"

pt_cdword	g_so_map_id_ = 0x1e4;
pt_cdword	g_so_map_cn_name_ = 0x1f0;
pt_cdword	g_so_game_obj_pos_ = 0x20;
pt_cdword	g_so_game_obj_dir_angle_ = 0x34;
pt_cdword	g_so_obj_type_name_ = 0x64;
pt_cdword	g_so_game_obj_ukn_vector_ = 0xd8;
pt_cdword	g_so_game_obj_state_info_ = g_so_game_obj_ukn_vector_ + 0xc;
pt_cdword	g_so_map2game_objs_ = 0xac;
pt_cdword	g_so_bag_mgr2inventory_info_ = 0x34;
pt_cdword	g_so_item_id_ = 0x28;
pt_cdword	g_so_item_idx_ = 0x44;
pt_cdword	g_so_item_color_ = 0x4c;
pt_cdword	g_so_item_cnt_ = 0x58;
pt_cdword	g_so_item2db_item_ = 0xac;
pt_cdword	g_so_tbl_item_item_name_xmlid_ = 0xe8;
pt_cdword	g_so_tbl_item_item_gender_ = 0x170;
pt_cdword	g_so_skills_info2skills_ = 0x30;
pt_cdword	g_so_ui_mgr_ui_cont_ = 0x20;
pt_cdword	g_so_ui_is_shown_ = 0x30;
pt_cdword	g_so_quest_mgr_cont_ = 0xcc;
pt_cdword	g_so_quest_is_ui_show_ = 0x3c;
pt_cdword	g_so_quest_db_quest_ = 0x18;
pt_cdword	g_so_quest_type_ = 0x1c;
pt_cdword	g_so_quest_id_ = 0x28;
pt_cdword	g_so_chapter_name_ = 0x14;
pt_cdword	g_so_skill_slot_id_ = 0x60;
pt_cdword	g_so_map_portal_ = g_o_get_role_me_ + 0x5c;
pt_cdword	g_so_portal_pos_ = 0x24;
pt_cdword	g_so_portal_dst_map_id_ = 0x11c;
pt_cdword	g_so_body_equip_mgr_ = g_o_role_lvl_ + 4;
pt_cdword	g_so_equip_slot_item_ = 0x2c;
pt_cdword	g_so_equip_item_item_ = 0x14;
pt_cdword	g_so_mail_info_cont_ = 0x74;
pt_cdword	g_so_mail_info_sender_ = 0x28;
pt_cdword	g_so_mail_title_ = 0x3c;
pt_cdword	g_so_scene_graph_service_ = 0x20;
pt_cdword	g_so_scene_resource_path_ = 0x24;
pt_cdword	g_so_scene_model_cont_ = 0xa0;
pt_cdword	g_so_scene_obj_model_obj_ = 0x1c;
pt_cdword	g_so_scene_obj_ukn_flag_ = 0x4c;
pt_cdword	g_so_character_cont_ = 0x12c;
pt_cdword	g_so_login_character_id_ = 0x18;
pt_cdword	g_so_login_character_name_ = 0x17c;
pt_cdword	g_so_party_member_cont_ = 0x1c;
pt_cdword	g_so_party_team_name_ = 0x60;
pt_cdword	g_so_party_captain_name_ = 0x80;
pt_cdword	g_so_party_mem_id_ = 0x28;
pt_cdword	g_so_party_mem_name_ = 0x18c;
pt_cdword	g_so_cube_root_node_ = 0x98;
pt_cdword	g_so_out_packet_msg_id_ = 0x18;
pt_cdword	g_c_msvcr90_malloc_ = g_c_msvcr90_free_ + 0xc;

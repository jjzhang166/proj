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

////////////////////////////////////
//patch

////////////////////////////////////
//call
pt_cdword	g_c_create_role_by_name_ = 0X00A74F70;

////////////////////////////////////
//addr

////////////////////////////////////
//offset

////////////////////////////////////
//vtbl
pt_cdword	g_va_model_ = 0X014F54A0;

////////////////////////////////////
//net
pt_cdword	g_a_cur_line_ = 0X016CA814;
pt_cdword	g_c_game_socket_send_ = 0X007FD6E0;
pt_cdword	g_c_construct_out_packet_ = 0X0052B210;
pt_cdword	g_c_out_packet_send_ = 0X01C54EF1;
pt_cdword	g_c_msvcr90_free_ = 0X013E98E8;

////////////////////////////////////
//role
pt_cdword	g_a_map_ = 0X0172412C;
pt_cdword	g_o_get_role_me_ = 0X00000224;
pt_cdword	g_o_role_name_ = 0X00000F50;
pt_cdword	g_o_role_lvl_ = 0X00001284;
pt_cdword	g_o_role_max_attr_value_ = 0X00000158;
pt_cdword	g_o_role_cut_attr_value_ = 0X00000378;
pt_cdword	g_o_get_tbl_item_game_obj_ = 0X00001228;
pt_cdword	g_o_tbl_item_game_obj_friendly_ = 0X00000044;
pt_cdword	g_o_tbl_item_game_obj_type_ = 0X00000088;
pt_cdword	g_o_drop_item_obj2item_ = 0X00000134;
pt_cdword	g_o_game_obj_cube_pos_ = 0X000010A8;
pt_cdword	g_o_model_control_ = 0X000000E8;

////////////////////////////////////
//item
pt_cdword	g_a_gold_ = 0X01724A60;
pt_cdword	g_a_bag_mgr_ = 0X01724B60;
pt_cdword	g_a_mail_mgr_ = 0X01724CF0;

////////////////////////////////////
//npc

////////////////////////////////////
//map
pt_cdword	g_a_light_service_ = 0X0172E49C;
pt_cdword	g_c_get_square_pos_ = 0X00E29E00;
pt_cdword	g_c_map_information_service_ = 0X01724E88;

////////////////////////////////////
//Skill
pt_cdword	g_o_skill_mgr_ = 0X00001740;

////////////////////////////////////
//Task
pt_cdword	g_o_role_action_mgr_ = 0X00001A50;
pt_cdword	g_a_quest_mgr_ = 0X0172450C;
pt_cdword	g_c_get_chapter_info_by_id_ = 0X009AA6F0;
pt_cdword	g_c_get_quest_name_by_id_ = 0X009AAA90;

////////////////////////////////////
//db
pt_cdword	g_a_xml_string_ = 0X0172251C;
pt_cdword	g_c_find_xml_str_by_id_ = 0X00A1CD10;

////////////////////////////////////
//ui
pt_cdword	g_a_ui_mgr_ = 0X01724148;
pt_cdword	g_c_get_skill_slot_by_idx_ = 0X00AD0610;

////////////////////////////////////
//other
pt_cdword	g_a_game_wnd_handle_ = 0X0173B11C;
pt_cdword	g_a_party_mgr_ = 0X01724E80;

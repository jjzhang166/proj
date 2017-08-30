#pragma once
/*
@author zhp
@date 2016/4/11 21:39

g_so->g_stable_offset
g_o->g_offset
g_dso->g_direct_stable_offset
*/
#include "CD_PT_Updates.h"
#include "CD_StructsBase.h"

pt_cdword	g_so_net_obj_ukn = 0xc;			//MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET1___
pt_cdword	g_so_net_obj_name = 0x48;
pt_cdword	g_so_net_obj_str_area = 0x60;
pt_cdword	g_so_net_obj_local_obj = 0xa0;
pt_cdword	g_so_net_obj_type = 0x24;
pt_cdword	g_so_net_obj_room_num_ = 0x2c;
pt_cdword	g_so_net_obj_ukn_impl = 0x38;
pt_cdword	g_so_net_obj_mgr_packet_id = 0x44;
pt_cdword	g_so_net_obj_mgr = 0x3c;
pt_cdword	g_so_global_net_obj_mgr = 0x78;
pt_cdword	g_so_ukn_obj_2_net_obj_mgr = 0xa0;
pt_cdword	g_so_obj_dir = 0x50;
pt_cdword	g_so_pos_dir_info = 0x64;
pt_cdword	g_so_local_obj_base_link_str = 0xB8;	//遍历门对象的link字符串，判断门是否有效的标志 MMOCLIENT_REVERSAL_DOOR_IN_ROOM_LINK_OFFSET___
pt_cdword	g_so_local_obj_id = 0x8;
pt_cdword	g_so_local_obj_info = 0xc;
pt_cdword	g_so_obj_status = 0x1ac;
pt_cdword	g_so_obj_db_npc_id = 0x14;
pt_cdword	g_so_obj_db_name_ch = 0x1c;
pt_cdword	g_so_obj_db_name_en = 0x38;
pt_cdword	g_so_obj_db_title1 = 0x74;
pt_cdword	g_so_obj_base_name = 0xf0;
pt_cdword	g_so_obj_base_packet_id = 0xfc;
pt_cdword	g_so_obj_base_id = 0x10;
pt_cdword	g_so_obj_base_tmp_packet_id = 0x18;		//MMOCLIENT_REVERSAL_HIT_BU_WEI_MONSTER_PACKET_ID_OFFSET___
pt_cdword	g_so_obj_base_net_obj = 0x108;
pt_cdword	g_so_obj_base_pos = 0x3c;
pt_cdword	g_so_local_obj_mgr = 0x1C;
pt_cdword	g_so_local_obj_mgr1 = 0xc;	//CryAction
pt_cdword	g_so_role_info_3 = 0x214;	//CryGame
pt_cdword	g_so_role_single_exp = 0x14F0;
pt_cdword	g_so_role_double_exp = 0x1A38;
pt_cdword	g_so_role_poison_state = g_o_role_poison_state_ + 0xA * 4;
pt_cdword	g_so_role_lvl = g_o_role_encrypt_info_ + 0xc;
pt_cdword	g_so_role_cur_exp = g_o_role_encrypt_info_ + 0x5c;
pt_cdword	g_so_role_cur_hp = 0xac + g_o_role_encrypt_info_;
pt_cdword	g_so_role_hp_ = g_so_role_cur_hp + sizeof(stCD_EncryptInt);
pt_cdword	g_so_role_nei_shang_ = g_so_role_cur_hp + 0x68;
pt_cdword	g_so_role_cur_endure = 0x146 + g_o_role_encrypt_info_;
pt_cdword	g_so_role_endure_ = g_so_role_cur_endure + sizeof(stCD_EncryptInt);
pt_cdword	g_so_role_left_fb_cnt = g_o_role_left_fb_cnt_ + g_o_role_encrypt_info_;
pt_cdword	g_so_role_gold = g_o_role_gold_ + g_o_role_encrypt_info_;
pt_cdword	g_so_role_silver = g_o_role_silver_ + g_o_role_encrypt_info_;
pt_cdword	g_so_role_jing_suan_point = g_o_role_jing_suan_point_ + g_o_role_encrypt_info_;
pt_cdword	g_so_role_role_buf_mgr = 0x10;	//MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_ADDR_OFFSET___
pt_cdword	g_so_map_db_id = 0x10;
pt_cdword	g_so_map_db_info_id = 0x14;
pt_cdword	g_so_map_db_info = 0x18;
pt_cdword	g_so_map_db_name = 0x1C;
pt_cdword	g_so_map_db_info_name = 0x28;
pt_cdword	g_so_map_db_type = 0x20;	//MMOCLIENT_REVERSAL_FB_INFO_TYPE_OFFSET___
pt_cdword	g_so_map_db_diff_lvl = 0x30;	//MMOCLIENT_REVERSAL_FB_INFO_DIFF_LEVEL_OFFSET__
pt_cdword	g_so_map_db_role_lvl = 0x38;	//MMOCLIENT_REVERSAL_FB_INFO_LEVEL_OFFSET___
pt_cdword	g_so_packages_mgr_package = 0xc;	//MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_ITEM_OFFSET1___
pt_cdword	g_so_packages_mgr_store = 0x14;	//MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_CANG_KU_OFFSET1___
pt_cdword	g_so_packages_mgr_equip = 0x18;	//MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_EQUIP_OFFSET1___
pt_cdword	g_so_packages_mgr_task = 0x20;		//MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_TASK_OFFSET1___
pt_cdword	g_so_packages_mgr_su_cai = 0x34;	//MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_CANG_KU_SU_CAI_OFFSET1___
pt_cdword	g_so_grids_mgr_max_cnt = 0x8;	//MMOCLIENT_REVERSAL_TRAVERSE_ITEM_COUNT_OFFSET__  MMOCLIENT_REVERSAL_TRAVERSE_GE_ZI_COUNT_OFFSET___
pt_cdword	g_so_grids_mgr_item_cont = 0x18;	//MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OFFSET1___
pt_cdword	g_so_grid_item_cont_items = 0x8;	//MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OFFSET2___
pt_cdword	g_so_item_item_info = 0x08;	//MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OBJ_ADDR_OFFSET___
pt_cdword	g_so_item_serial = 0x10;	//MMOCLIENT_REVERSAL_TRAVERSE_ITEM_PACKET_KEY1_OFFSET___ MMOCLIENT_REVERSAL_TRAVERSE_ITEM_PACKET_KEY2_OFFSET___
pt_cdword	g_so_item_bind_flag = 0x28;	//MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OBJ_BANG_DING_OFFSET___
pt_cdword	g_so_item_cnt = 0x3C + 0x10;	//MMOCLIENT_REVERSAL_TRAVERSE_ITEM_OBJ_COUNT_OFFSET___
pt_cdword	g_so_item_db_item_id = 0x10;	//MMOCLIENT_REVERSAL_ITEM_PACKET_ID_OFFSET___
pt_cdword	g_so_item_db_name = 0x14;	//MMOCLIENT_REVERSAL_TRAVERSE_ITME_OBJ_NAME_OFFSET___
/*++
物品描述		  		物品类型		部位类型1		部位类型2
近程武器 大剑				2				1				1
近程武器 锤子				2				1				2
近程武器 长枪				2				1				3
近程武器 片手				2				1				4
远程武器 弩 				2				1				5
近程武器 双刀				2				1				6
近程武器 太刀				2				1				7
近程武器 笛子				2				1				8
远程武器 弓					2				1				10

近程防具 头部				2				2				1
近程防具 腕部				2				2				2
近程防具 胸部				2				2				3
近程防具 腰部				2				2				4
近程防具 腿部				2				2				5
通用首饰 戒指				2				3				1
通用首饰 项链				2				3				2
通用首饰 护石				2				3				4

素材						1				8				1
素材2						1				8				2
--*/
pt_cdword	g_so_item_db_item_category = 0x1c;	//MMOCLIENT_REVERSAL_ITEM_WU_PIN_LEI_XING_OFFSET___
pt_cdword	g_so_item_db_item_type = 0x20;	//MMOCLIENT_REVERSAL_ITEM_BU_WEI_LEI_XING_OFFSET___
pt_cdword	g_so_item_db_part_type = 0x24;	//MMOCLIENT_REVERSAL_ITEM_WU_PIN_LEI_XING_SUO_OFFSET___
pt_cdword	g_so_item_db_lvl = 0x28;	//MMOCLIENT_REVERSAL_ITEM_LEVEL_OFFSET___
pt_cdword	g_so_item_db_own_max = 0x30;	//MMOCLIENT_REVERSAL_ITEM_YONG_YOU_SHANG_XIAN_OFFSET___
pt_cdword	g_so_item_db_quality = 0x34;	//MMOCLIENT_REVERSAL_ITEM_YAN_SE_OFFSET___
pt_cdword	g_so_item_db_carry_max = 0x38;	//MMOCLIENT_REVERSAL_ITEM_XIE_DAI_SHANG_XIAN_OFFSET___
pt_cdword	g_so_item_db_dui_die_max = 0x3c;	//MMOCLIENT_REVERSAL_ITEM_DUI_DIE_SHANG_XIAN_OFFSET___
pt_cdword	g_so_item_db_fen_jie = 0x84;	//MMOCLIENT_REVERSAL_ITEM_WU_PIN_FEN_JIE_OFFSET___
pt_cdword	g_so_item_db_equip_info = 0x1C0;	//MMOCLIENT_REVERSAL_ITEM_UP_EQUIP_INFO_ADDR_OFFSET___
pt_cdword	g_so_equip_info_up_info = 0x1C;	//MMOCLIENT_REVERSAL_ITEM_UP_EQUIP_MATERIAL_INFO_OFFFSET___
pt_cdword	g_so_task_db_id = 0x4;
pt_cdword	g_so_task_db_name = 0x8;	//MMOCLIENT_REVERSAL_TASK_NAME_OFFSET___
pt_cdword	g_so_task_db_desc = g_so_task_db_name + 4;	//MMOCLIENT_REVERSAL_TASK_DESC_OFFSET___
pt_cdword	g_so_task_db_type = 0x10;	//MMOCLIENT_REVERSAL_TASK_TYPE_OFFSET___
pt_cdword	g_so_task_db_format_str = 0x14;
pt_cdword	g_so_task_db_lvl_info = 0x20;	//MMOCLIENT_REVERSAL_TASK_LEVEL_ADDR_OFFSET1___
pt_cdword	g_so_task_db_complete_npc_info = g_o_task_db_npc_info_ + 4;	//UPDATES_MMOCLIENT_REVERSAL_TASK_RECEVIED_NPC_NAME_OFFSET1+4
pt_cdword	g_so_task_db_dst_fb_info = 0x40;	//MMOCLIENT_REVERSAL_DO_TASK_DST_OFFSET1___
pt_cdword	g_so_task_db_reward_info = 0x4C;	//MMOCLIENT_REVERSAL_TASK_REWARD_ITEM_OFFSET___;
pt_cdword	g_so_task_lvl_obj_lvl_info = 0x4;	//MMOCLIENT_REVERSAL_TASK_LEVEL_ADDR_OFFSET2___
pt_cdword	g_so_task_dst_fb_info_fbs = 0x14;	//MMOCLIENT_REVERSAL_DO_TASK_DST_OFFSET2___
pt_cdword	g_so_task_dst_fb_info_fb_id = 0x20;	//MMOCLIENT_REVERSAL_DO_TASK_DST_FB_ID_OFFSET1___
pt_cdword	g_so_dst_fb_info_id = 0x0C;	//MMOCLIENT_REVERSAL_DO_TASK_DST_FB_ID_OFFSET2___
pt_cdword	g_so_glo_received_task_root_node = 0x10;	//MMOCLIENT_REVERSAL_TASK_BASE_OFFSET1___
pt_cdword	g_so_task_obj_task_db = 0x08;	//MMOCLIENT_REVERSAL_RECEIVED_TASK_INFO_ADDR___
pt_cdword	g_so_task_obj_complete_state = 0x14;	//MMOCLIENT_REVERSAL_TASK_COMPLETE_OFFSET___
pt_cdword	g_so_task_obj_is_player_task = 0x1C;	//MMOCLIENT_REVERSAL_RECEIVED_TASK_TRUE_OFFSET___
pt_cdword	g_so_enterable_fb_mgr = 0x8;	//MMOCLIENT_REVERSAL_IN_FB_INFO_OFFSET2___
pt_cdword	g_so_farm_facility_cur_facility_info = 0x2c;	//MMOCLIENT_REVERSAL_CUR_FARM_FACILITY_LEVEL_OFFSET3___
pt_cdword	g_so_farm_db_id = 0x10;	//MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_ID_OFFSET___
pt_cdword	g_so_farm_db_lvl = 0x14;	//MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_LEVEL_OFFSET___
pt_cdword	g_so_farm_db_point = 0x20;	//MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_POINT_OFFSET___
pt_cdword	g_so_farm_db_silver = 0x24;	//MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_YING_OFFSET___
pt_cdword	g_so_farm_db_name_info = 0x50;	//MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_NAME_OFFSET___
pt_cdword	g_so_farm_db_desc_info = 0x54;	//MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_DESC_OFFSET___
pt_cdword	g_so_farm_db_role_lvl = 0x58;	//MMOCLIENT_REVERSAL_FARM_FACILITY_INFO_ROLE_LEVEL_OFFSET___
pt_cdword	g_so_farm_time_db_time = 0x1C;	//MMOCLIENT_REVERSAL_CUR_FARM_CP_TIME_OFFSET1___
pt_cdword	g_so_shop_db_cate_id = 0x10;
pt_cdword	g_so_shop_db_cate_name = 0x18;
pt_cdword	g_so_shop_db_shop_info = 0x1C;	//MMOCLIENT_REVERSAL_GET_SHOP_INFO_OFFSET___
pt_cdword	g_so_shop_info_shop_id = 0x8;	//MMOCLIENT_REVERSAL_SHOP_INFO_SHOP_ID_OFFSET___
pt_cdword	g_so_shop_info_item_id = 0x28;	//MMOCLIENT_REVERSAL_SHOP_INFO_ITEM_ID_OFFSET___
pt_cdword	g_so_shop_info_item_cnt = 0x2C;	//MMOCLIENT_REVERSAL_SHOP_INFO_ITEM_COUNT_OFFSET___
pt_cdword	g_so_seed_db_item_id = 0x10;
pt_cdword	g_so_seed_db_item_name = 0x14;	//MMOCLIENT_REVERSAL_FARM_SEED_NAME_OFFSET___
pt_cdword	g_so_craft_db_craft_id = 0x10;	//MMOCLIENT_REVERSAL_CRAFT_INFO_CRAFT_ID_OFFSET___
pt_cdword	g_so_craft_db_dst_item_id = 0x44;	//MMOCLIENT_REVERSAL_CRAFT_INFO_ITEM_ID_OFFSET___
pt_cdword	g_so_menufacture_db_id = 0x10;	//MMOCLIENT_REVERSAL_MANU_FACTURE_INFO_ID_OFFSET___
pt_cdword	g_so_menufacture_db_item_name = 0x14;	//MMOCLIENT_REVERSAL_MANU_FACTURE_INFO_NAME_OFFSET___
pt_cdword	g_so_menufacture_db_dst_item_id = 0x58;	//MMOCLIENT_REVERSAL_MANU_FACTURE_INFO_ITEM_ID_OFFSET___
pt_cdword	g_so_menufacture_db_meterial_info = 0x84;	//MMOCLIENT_REVERSAL_MANU_FACTURE_INFO_MAIERSAL_OFFSET___
pt_cdword	g_so_gift_bag_root_node = 0x10;	//MMOCLIENT_REVERSAL_GIFT_BAG_INFO_OFFSET2___
pt_cdword	g_so_gift_obj_gift_db = 0x4;	//MMOCLIENT_REVERSAL_GIFT_BAG_INFO_OFFSET3___
pt_cdword	g_so_gift_obj_flag = 0xc;		//MMOCLIENT_REVERSAL_GIFT_BAG_GET_FLAG_OFFSET___
pt_cdword	g_so_gift_db_id = 0x4;
pt_cdword	g_so_gift_db_name = 0x8;		//MMOCLIENT_REVERSAL_GIFT_BAG_INFO_NAME_OFFSET___
pt_cdword	g_so_gift_db_desc = 0xc;		//MMOCLIENT_REVERSAL_GIFT_BAG_INFO_DESC_OFFSET___
pt_cdword	g_so_supply_ukn2_ukn2_1 = 0x18;	//MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET3___
pt_cdword	g_so_supply_ukn2_1_ukn1 = 0x4;	//MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET4___
pt_cdword	g_so_supply_ukn1_ukn_obj = 0x18;	//MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET5___
pt_cdword	g_so_supply_ukn_obj_firt_item_node = 0x18;	//MMOCLIENT_REVERSAL_REQUIRE_SLOT_OFFSET6___
pt_cdword	g_so_mail_info_id = 0;	//MMOCLIENT_REVERSAL_MAIL_ID_OFFSET1___
pt_cdword	g_so_mail_info_sender = 0x2C;	//MMOCLIENT_REVERSAL_MAIL_SENDER_OFFSET___
pt_cdword	g_so_mail_info_title = 0x5C;	//MMOCLIENT_REVERSAL_MAIL_TITLE_OFFSET___
pt_cdword	g_so_mail_info_gold = 0x8C;		//MMOCLIENT_REVERSAL_MAIL_GOLD_OFFSET___
pt_cdword	g_so_mail_info_item = 0xA4;		//MMOCLIENT_REVERSAL_MAIL_ATTACH_ITEM_OFFSET___
pt_cdword	g_so_mail_item_item_cnt = 0x10;	//MMOCLIENT_REVERSAL_MAIL_ATTACH_ITEM_COUNT_OFFSET___
pt_cdword	g_so_mail_item_item_id = 0x14;	//MMOCLIENT_REVERSAL_MAIL_ATTACH_ITEM_ID_OFFSET___
pt_cdword	g_so_mail_item_size = 0x20;		//MMOCLIENT_REVERSAL_MAIL_ATTACH_ITEM_SIZE___
pt_cdword	g_so_buff_db_id = 0x10;	//MMOCLIENT_REVERSAL_BUFF_INFO_ID_OFFSET___
pt_cdword	g_so_buff_db_name = 0x28;	//MMOCLIENT_REVERSAL_BUFF_INFO_NAME_OFFSET___
pt_cdword	g_so_buff_db_desc = 0x3A8;	//MMOCLIENT_REVERSAL_BUFF_INFO_DESC_OFFSET___
pt_cdword	g_so_buf_mgr_buf_root_node1 = 0x04;	//MMOCLIENT_REVERSAL_TIAN_QI_BUFF_STATE_OFFSET___
pt_cdword	g_so_buf_mgr_buf_root_node2 = 0x1C;	//MMOCLIENT_REVERSAL_ROLE_BUFF_STATE_OFFSET2___
pt_cdword	g_so_buf_dst_packet_id = 0x4;	//MMOCLIENT_REVERSAL_ROLE_BUFF_ACTOR_PACKET_ID_OFFSET___
pt_cdword	g_so_shang_jin_info_task_id = 0x10;	//MMOCLIENT_REVERSAL_SHANG_JIN_TASK_ID_OFFSET___
pt_cdword	g_so_shang_jin_task_db = 0x2c;	//MMOCLIENT_REVERSAL_SHANG_JIN_TASK_INFO_ADDR_OFFSET___
pt_cdword	g_so_use_item_item_cate = 0x24;	//MMOCLIENT_REVERSAL_USE_ITEM_TAB_INDEX_OFFSET___
pt_cdword	g_so_use_item_slot_idx = 0x28;	//MMOCLIENT_REVERSAL_USE_ITEM_SLOT_INDEX_OFFSET___
pt_cdword	g_so_use_item_flag = 0x3C;	//MMOCLIENT_REVERSAL_USE_ITEM_TRIGGER_FLAG_OFFSET___
pt_cdword	g_so_week_credit_credit_info = 0xB4;	//MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET2___
pt_cdword	g_so_week_credit_info_impl = 0x14;	//MMOCLIENT_REVERSAL_WEEK_CREDIT_COUNT_OFFSET___
pt_cdword	g_so_week_credit_ukn_gained_cnt = 0x38;	//MMOCLIENT_REVERSAL_WEEK_CREDIT_HAS_RECEIVE_COUNT_OFFSET___
pt_cdword	g_so_req_item_item_db = 0x8;	//MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_ID_OFFSET3___
pt_cdword	g_so_req_item_item_cnt = 0x4C;	//MMOCLIENT_REVERSAL_RECV_REQ_ITEM_BY_GET_ITEM_COUNT_OFFSET___
pt_cdword	g_svo_ui_system_handle_mouse_msg = 0x10;	//MMOCLIENT_REVERSAL_MOUSE_LBUTTONDOWN_OFFSET2___
pt_cdword	g_so_glo_dx_mgr_dx_mgr = 0x48;	//MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_OFFSET1___
pt_cdword	g_so_dx_mgr_key_mgr = 0x0;	//MMOCLIENT_REVERSAL_DX_KEYBOARD_PRESS_OFFSET2___
pt_cdword	g_svo_SynCoorDinatesRandom1 = 0x38;
pt_cdword	g_svo_SynCoorDinatesRandom2 = 0x228;
pt_cdword	g_so_local_obj_static_room_num = 0x120;	//MMOCLIENT_REVERSAL_DOOR_IN_ROOM_INDEX_OFFSET___
pt_cdword	g_so_net_obj_collect_name = 0x168;	//MMOCLIENT_REVERSAL_TRAVERSE_OBJ_NAME2_OFFSET___
pt_cdword	g_so_net_obj_room_obj = g_o_net_obj_collect_monster_packet_id_ + 8;	//MMOCLIENT_REVERSAL_TRAVERSE_COLLECT_ROOM_INDEX_OFFSET1___
pt_cdword	g_so_collect_room_info_room_num = 0x50;
pt_cdword	g_so_ui_mgr1 = 0xbc;	//MMOCLIENT_REVERSAL_UI_OFFSET1____
pt_cdword	g_so_ui_mgr2 = 0xb8;	//MMOCLIENT_REVERSAL_UI_OFFSET2____
pt_cdword	g_so_ui_mgr2_ukn = 0x8;
pt_cdword	g_so_ui_mgr3 = 0x14;	//MMOCLIENT_REVERSAL_UI_OFFSET3____
pt_cdword	g_so_ui_mgr4 = 0x64;	//MMOCLIENT_REVERSAL_UI_OFFSET4____
pt_cdword	g_so_ui_mgr5 = 0x5c;	//MMOCLIENT_REVERSAL_UI_OFFSET5____
pt_cdword	g_so_ui_ctrl_parent_idx = 0x18;
pt_cdword	g_so_ui_ctrl_parent = 0x20;	//MMOCLIENT_REVERSAL_UI_INFO_FATHERWND_OFFSET____
pt_cdword	g_so_ui_ctrl_flag = 0x3e;
pt_cdword	g_so_ui_ctrl_class_name_info = 0x44;
pt_cdword	g_so_ui_ctrl_next = 0x54;	//MMOCLIENT_REVERSAL_UI_INFO_NEXTPOINT_OFFSET____
pt_cdword	g_so_ui_ctrl_class_name_info_name = 0x8;
pt_cdword	g_svo_ui_ctrl_handle_msg = 0x194;
pt_cdword	g_so_ui_ctrl_edit_edit_ukn = 0x84;
pt_cdword	g_so_ui_ctrl_edit_focus = 0xAC;	//MMOCLIENT_REVERSAL_UI_INFO_ISFOCUS_OFFSET_____
pt_cdword	g_svo_ui_ctrl_edit_put_char = 0x188;
pt_cdword	g_so_ui_ctrl_str_mgr = 0x8;
pt_cdword	g_so_pop_dlg_info_str_info = 0x20;	//MMOCLIENT_REVERSAL_GET_DIALOG_INFO_DESC_OFFSET___
pt_cdword	g_so_juqing_info_str_info = 0x14;	//MMOCLIENT_REVERSAL_SPRINTF_JU_QING_STRING_OFFSET___
pt_cdword	g_so_hitted_obj_info_net_obj = 0x4;	//MMOCLIENT_REVERSAL_MONSTER_HP_ADDR_OFFSET1___
pt_cdword	g_so_body_part_obj_obj_base = 0x1C;	//MMOCLIENT_REVERSAL_HIT_BU_WEI_ACOTR_ADDR2_OFFSET___
pt_cdword	g_so_body_part_obj_part_info = 0xC4;	//MMOCLIENT_REVERSAL_HIT_BU_WEI_STRUCT_ADDR_OFFSET___
pt_cdword	g_so_body_part_obj_part_info_cnt = 0xC8;	//MMOCLIENT_REVERSAL_HIT_BU_WEI_STRUCT_COUNT_OFFSET___
pt_cdword	g_so_monster_remote_hit_info_hiited_obj = 0x18;	//MMOCLIENT_REVERSAL_MONSTER_REMOTE_HIT_OBJ_OFFSET___
pt_cdword	g_so_monster_remote_hit_obj_hit_obj_id = 0x40;
pt_cdword	g_so_monster_remote_hit_obj_hit_flag = 0x54;
pt_cdword	g_so_ui_global_mng_ui_mng = 0X00000008;	//UPDATES_MMOCLIENT_REVERSAL_TRAVERSAL_ALL_OBJ_OFFSET
pt_cdword	g_so_ui_mng_name = 0x18;	//MMOCLIENT_REVERSAL_TRAVERSAL_OBJ_MSG_NAME___
pt_cdword	g_so_ui_mng_minimap_room_num = 0x64;	//MMOCLIENT_REVERSAL_MINIMAP_ROLE_ROOM_INDEX_OFFSET___
//////////////////////////////////////////////////////////////////////////
//*****************直接给Game.dll使用的偏移********************************
pt_cdword	g_so_dlg_info_ban_time = 0x10;	//MMOCLIENT_REVERSAL_GET_DIALOG_INFO_DESC_PARAMTER_OFFSET___
//////////////////////////////////////////////////////////////////////////
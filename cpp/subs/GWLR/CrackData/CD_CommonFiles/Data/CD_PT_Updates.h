
#pragma once
#include <ProtocolX/P_BaseTypeX.h>		
/*										
g_a->g_address.
g_o->g_offset.
g_vo->g_vtbl_offset.
g_h->g_hook.
g_c->g_call.
g_p->g_patch.
*/

///////////////////////////hooks///////////////////////////////////////
pt_cdword	g_h_HookGainBodyPart_ = 0X00C9E8CD;	//MHOClient
pt_cdword	g_h_MainWndAddr_ = 0X004689AE;	//MHOClient
pt_cdword	g_h_pHookRecv_ = 0X001CD1C7;	//CryGame
pt_cdword	g_h_HookGainMonsterHp_ = 0X014767E0;	//CryGame
pt_cdword	g_h_HookGainHitHp_ = 0X01411190;	//CryGame
pt_cdword	g_h_HookGainFeebHp_ = 0X01410FB0;	//CryGame
pt_cdword	g_h_pLoadMap_Village_ = 0X00F1FE5A;	//CryGame
pt_cdword	g_h_LoadMap_Fb_ = 0X00F1FB4D;	//CryGame
pt_cdword	g_h_FbGameOver_Win_ = 0X00F1D01D;	//CryGame
pt_cdword	g_h_FbGameOver_Lose_ = 0X00F1D13D;	//CryGame
pt_cdword	g_h_ChooseRoleScene_ = 0X00A92AEC;	//CryGame
pt_cdword	g_h_HookJuQing_ = 0X0030A9D9;	//CryGame
pt_cdword	g_h_HookDlgInfo_ = 0X0030AB06;	//CryGame
pt_cdword	g_h_HookGameSendOnly_ = 0X00E0E180;	//CryGame
pt_cdword	g_h_HookAmbientStr_ = 0X0030AABA;	//CryGame
pt_cdword	g_h_HookGamePassRoom_ = 0X00869B50;	//CryGame
pt_cdword	g_h_HookGamePassVillage_ = 0X00A50A00;	//CryGame
pt_cdword	g_h_HookJingCaiJingTou_ = 0X00EE88E1;	//CryGame
pt_cdword	g_h_MonsterHit_ = 0X00DCD91B;	//CryGame
pt_cdword	g_h_MonsterRemoteHit_ = 0X00BAF810;	//CryGame
////////////////////////patch////////////////////////////////////////////
pt_cdword	g_p_HandleNiRen_ = 0X01415AF9;	//CryGame
pt_cdword	g_p_sky_use_skill_jmp_ = 0X008EE190;	//CryGame
////////////////////////calls////////////////////////////////////////////
pt_cdword	g_c_GetGlobalData_ = 0X01419590;	//CryGame
pt_cdword	g_c_FindNetObj_ = 0X010295D0;	//CryAction
pt_cdword	g_c_decrypt_int_ = 0X001DEC60;	//CryGame
pt_cdword	g_c_encrypt_int_ = 0X001DEBD0;	//CryGame
pt_cdword	g_c_FindItemDb_ = 0X002F7640;	//CryGame
pt_cdword	g_c_FindTaskDb_ = 0X0036EF60;	//CryGame
pt_cdword	g_c_FindMapDb_ = 0X005C9B80;	//CryGame
pt_cdword	g_c_AnyNpcHasTheTask_ = 0X015321A0;	//CryGame
pt_cdword	g_c_GetChapterInfo_ = 0X00E88D70;	//CryGame
pt_cdword	g_c_IsTheFbDone_ = 0X014F0A30;	//CryGame
pt_cdword	g_c_GetFarmDbId_ = 0X014258D0;	//CryGame
pt_cdword	g_c_FindFarmDb_ = 0X006A1040;	//CryGame
pt_cdword	g_c_FindFarmTimeDb_ = 0X006A0F80;	//CryGame
pt_cdword	g_c_GetFarmFiledLeftTime_ = 0X01427F30;	//CryGame
pt_cdword	g_c_FindShopDb_ = 0X003A1B00;	//CryGame
pt_cdword	g_c_FindSeedDb_ = 0X006A1100;	//CryGame
pt_cdword	g_c_FindCraftDb_ = 0X0064B520;	//CryGame
pt_cdword	g_c_GetMenufactureObj_ = 0X005DC700;	//CryGame
pt_cdword	g_c_FindMenufactureDb_ = 0X0030B750;	//CryGame
pt_cdword	g_c_FindBuffDb_ = 0X003A1BD0;	//CryGame
pt_cdword	g_c_EncryptMsgBuffer_ = 0X001ABA20;	//CryGame
pt_cdword	g_c_SendMsg_ = 0X001CDC10;	//CryGame
pt_cdword	g_c_GetKingQuestInfo_ = 0X00E88E30;	//CryGame
pt_cdword	g_c_GetItemSellPrice_ = 0X014A8C30;	//CryGame
pt_cdword	g_c_UseSkill_ = 0X0092E100;	//CryGame
pt_cdword	g_c_GetWeekCreditInfo_ = 0X006E6420;	//CryGame
pt_cdword	g_c_GetWeekCreditCnt_ = 0X01518620;	//CryGame
pt_cdword	g_c_FindReqItem_ = 0X014EA9E0;	//CryGame
pt_cdword	g_c_handle_key_msg_ = 0X00EAAC60;	//MHOClient
pt_cdword	g_c_ShangJinTotalCnt_ = 0X0153F8C0;	//CryGame
pt_cdword	g_c_shang_jin_task_state_ = 0X0068DB60;	//CryGame
pt_cdword	g_c_TidyItemSize_ = 0X0141B430;	//CryGame
pt_cdword	g_c_TidyItem_ = 0X00E7BBE0;	//CryGame
pt_cdword	g_c_UiCtrlEditClrStr_ = 0X010CA170;	//MHOClient
/////////////////////////////address////////////////////////////////////////
pt_cdword	g_a_global_local_objs_ = 0X01B62DC4;	//CryGame
pt_cdword	g_a_global_net_obj_mgr_ = 0X01223114;	//CryAction
pt_cdword	g_a_global_role_info_data_ = 0X01B5F088;	//CryGame
pt_cdword	g_a_global_received_task_ = 0X01DADB80;	//CryGame
pt_cdword	g_a_global_shang_jin_info_mgr_ = 0X01DADBC8;	//CryGame
pt_cdword	g_a_use_item_mgr_ = 0X01DADBA0;	//CryGame
pt_cdword	g_a_glo_dx_mgr_ = 0X01856584;	//MHOClient
pt_cdword	g_a_glo_ui_mgr_ = 0X01174780;	//CryAction
pt_cdword	g_a_ui_global_mng_ = 0X01DAEC6C;	//CryGame
/////////////////////////offset/////////////////////////////////////////
pt_cdword	g_o_local_obj_mgr_ = 0X000004F4;	//CryAction
pt_cdword	g_o_net_obj_2_db_info_ = 0X00000D34;	//CryGame
pt_cdword	g_o_GloRoleInfoData_GloLocalIdMgr_ = 0X00000004;	//CryGame
pt_cdword	g_o_glo_role_info_ui_system_ = 0X00000090;	//CryGame
pt_cdword	g_o_role_info_1_ = 0X000000D0;	//CryGame
pt_cdword	g_o_role_info_2_ = 0X0000007C;	//CryGame
pt_cdword	g_o_role_info_all_local_obj_mgr_ = 0X00000050;	//CryGame
pt_cdword	g_o_role_encrypt_info_ = 0X00000438;	//CryGame
pt_cdword	g_o_role_zhan_wei_zhi_ = 0X00000826;	//CryGame
pt_cdword	g_o_role_gold_ = 0X000009B6;	//CryGame
pt_cdword	g_o_role_silver_ = 0X000009BE;	//CryGame
pt_cdword	g_o_role_left_fb_cnt_ = 0X00000A06;	//CryGame
pt_cdword	g_o_role_jing_suan_point_ = 0X000014FA;	//CryGame
pt_cdword	g_o_role_lian_qi_cao_ = 0X00000FBE;	//CryGame
pt_cdword	g_o_role_npc_mgr_ = 0X0000227C;	//CryGame
pt_cdword	g_o_role_mail_mgr_ = 0X00002290;	//CryGame
pt_cdword	g_o_role_gift_bag_ = 0X00002294;	//CryGame
pt_cdword	g_o_role_week_credit_mgr_ = 0X00002298;	//CryGame
pt_cdword	g_o_role_enterable_fb_mgr_ = 0X00002328;	//CryGame
pt_cdword	g_o_role_done_fb_mgr_ = 0X0000232C;	//CryGame
pt_cdword	g_o_role_poison_state_ = 0X00002358;	//CryGame
pt_cdword	g_o_role_farm_facility_ = 0X00024E38;	//CryGame
pt_cdword	g_o_role_self_farm_id_ = 0X00024E40;	//CryGame
pt_cdword	g_o_role_packages_mgr_ = 0X00025260;	//CryGame
pt_cdword	g_o_role_ser_line_ = 0X00025240;	//CryGame
pt_cdword	g_o_local_obj_skill_ = 0X00001E14;	//CryGame
pt_cdword	g_o_ukn_role_map_db_ = 0X00000068;	//CryGame
pt_cdword	g_o_item_db_role_lvl_ = 0X000000B0;	//CryGame
pt_cdword	g_o_task_db_ukn_ = 0X0000002C;	//CryGame
pt_cdword	g_o_task_db_npc_info_ = 0X00000030;	//CryGame
pt_cdword	g_o_farm_facility_cur_field_info_ = 0X000000BC;	//CryGame
pt_cdword	g_o_craft_db_material_info_ = 0X00000090;	//CryGame
pt_cdword	g_o_mail_mgr_mail_root_node_ = 0X00000054;	//CryGame
pt_cdword	g_o_role_buf_mgr_buf_mgr_ = 0X00000028;	//CryGame
pt_cdword	g_o_buf_flag_ = 0X00000020;	//CryGame
pt_cdword	g_o_buf_buf_db_ = 0X0000003C;	//CryGame
pt_cdword	g_o_all_local_obj_mgr_all_objs_ = 0X0000005C;	//MHOClient
pt_cdword	g_o_local_socket_mgr1_ = 0X00000180;	//CryGame
pt_cdword	g_o_socket_mgr1_mgr2_ = 0X00000004;	//CryGame
pt_cdword	g_o_socket_mgr2_mgr3_ = 0X0000001C;	//CryGame
pt_cdword	g_o_msg_req_item_1_ = 0X0000006D;	//CryGame
pt_cdword	g_o_msg_req_item_2_ = 0X0000006F;	//CryGame
pt_cdword	g_o_shang_jin_infos_complete_cnt_ = 0X00000014;	//CryGame
pt_cdword	g_o_net_obj_collect_monster_packet_id_ = 0X00000198;	//CryGame
pt_cdword	g_o_ui_mgr_ = 0X00003F78;	//MHOClient
/////////////////////////voffset/////////////////////////////////////////
pt_cdword	g_vo_net_obj_ukn_supply_ukn2_ = 0X0000016C;	//CryGame
pt_cdword	g_vo_shang_jin_mgr_shang_jin_array_ = 0X000000DC;	//CryGame
pt_cdword	g_vo_shang_jin_mgr_shang_jin_info_ = 0X000000D8;	//CryGame
/////////////////////////macro/////////////////////////////////////////
#define G_HIT_DIR_CALC_CALL_BASE_ADDR1_	0X0147FF74	//MHOClient
#define G_HIT_DIR_CALC_CALL_BASE_ADDR2_	0X013EFB84	//MHOClient
/////////////////////////lpka/////////////////////////////////////////////
pt_cdword	g_a_lpka_cry_base_crc_ = 0X00E4ED9F;	//MHOClientBase
pt_cdword	g_a_lpka_cry_base_thrd_ = 0X00A5D365;	//MHOClientBase
pt_cdword	g_a_lpka_terSafe_CanNotSleep_ = 0X0077572F;	//terSafe
pt_cdword	g_c_lpka_mhoclient_start_thrd_ = 0X004735F0;	//MHOClient
pt_cdword	g_c_lpka_mhoclient_start_thrd1_ = 0X01307890;	//MHOClient
pt_cdword	g_c_lpka_mhoclient_start_thrd2_ = 0X00FD4880;	//MHOClient
pt_cdword	g_c_lpka_mhoclient_start_thrd3_ = 0X012C3F30;	//MHOClient
pt_cdword	g_c_lpka_crygame_start_thrd_ = 0X00DD8800;	//CryGame
pt_cdword	g_c_lpka_crygame_start_thrd1_ = 0X00D9FAF0;	//CryGame
pt_cdword	g_c_lpka_crygame_start_thrd2_ = 0X0075CF00;	//CryGame
pt_cdword	g_c_lpka_crygame_start_thrd3_ = 0X001D2370;	//CryGame
pt_cdword	g_c_lpka_mhoclient_render_thrd_ = 0X004E2EB0;	//MHOClient
pt_cdword	g_c_lpka_mhoclient_no_show_npc_ = 0X004731F0;	//MHOClient
/////////////////////////lpkb/////////////////////////////////////////////
pt_cdword	g_h_lpkb_render_ = 0X0043BF5B;	//MHOClient
pt_cdword	g_h_lpkb_black_screen_render_ = 0X000F55F0;	//CryRenderD3D11
pt_cdword	g_h_lpkb_sprintf_ = 0X0013BC42;	//MHOClientBase
pt_cdword	g_h_lpkb_tersafe_get_cpuid_ = 0X00090640;	//terSafe
pt_cdword	g_h_lpkb_tersafe_code_crc_ = 0X001743AF;	//terSafe
/**************************************特征码更新结束*******************************************/
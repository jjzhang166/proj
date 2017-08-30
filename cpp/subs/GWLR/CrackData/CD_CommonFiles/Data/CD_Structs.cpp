#include "stdafx.h"
#include "CD_Structs.h"
#include "CD_UsefulFns.h"
#include <CommonX/CmnX_UsefulFunc.h>
#include "CD_FuncCall.h"
#include <cassert>
#include <CmnMix/Cmn_LogOut.h>
#include "CD_Traverse.h"

bool stCD_ObjDbInfo::ObjValid() const
{
	if (!CDF_ValidStr(ch_name_))
		return false;
	if (!CDF_ValidStr(en_name_))
		return false;
	
	return true;
}

stCD_EncryptInt::operator int() const{
	return CD_FuncCall::DecryptValue(this);
}

std::ostream& operator << (std::ostream& ostm, const stCD_EncryptInt& v){
	ostm << CD_FuncCall::DecryptValue(&v);
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_PosDirInfo& v){
	ostm << " { dir1:" << v.dir_1 << " fx:" << v.fx << " dir2:" << v.dir_2
		<< " fy:" << v.fy << " dir3:" << v.dir_3 << " fz:" << v.fz << " }";
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_ObjDbInfo::stCD_ObjTitleInfo& v){
	ostm << "{ title:" << Cmnx_NilStr(v.title_);
	ostm << " }";
	return ostm;
}
std::ostream& operator << (std::ostream& ostm, const stCD_ObjDbInfo& v){
	ostm << "{ addr:" << &v << " vtbl:" << v.vtbl_ << " obj_db_id:" << v.obj_db_id_ <<
		" ch_name:" << Cmnx_NilStr(v.ch_name_) << " type:" << v.type_ << " en_name_:" << Cmnx_NilStr(v.en_name_);
	if (v.title_info_)
		ostm << " title_info_:" << *v.title_info_;
	ostm << " }";
	return ostm;
}

void stCD_NetObj::OutStream(std::ostream& ostm) const{
	ostm << " vtbl:" << vtbl_ << " packet id:" << packet_id_ << " id:" << id_ << " name:" << Cmnx_NilStr(name_) << " type:" << type_;
	ostm << " room_num:" << room_num_;
	ostm << " local_obj_addr:" << local_obj_;
}

bool stCD_NetObj::ObjValid() const{
	if (kInvalidLocalId == id_)
		return false;
	if (kInvalidPacketId == packet_id_)
		return false;
	return true;
}
std::ostream& operator << (std::ostream& ostm, const stCD_NetObj& v){
	ostm << "{ addr:" << &v;
	v.OutStream(ostm);
	ostm << " }";
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_NetObj_Npc& v){
	ostm << "{ addr:" << &v;
	bool is_pass = false;
	if (!IsUnValidPtr(v.str_area_))
	{
		if (0 == strcmp(v.str_area_, "SceneBuffArea"))
			is_pass = true;
	}
	if (is_pass)
	{
		ostm << "这个采集物过滤掉了";
	}
	else
	{
		v.OutStream(ostm);
		if (!IsUnValidPtr(v.db_info_))
			ostm << " db_info:" << *v.db_info_;
	}	
	ostm << " }";
	return ostm;
}

bool stCD_NetObj_Npc::ObjValid() const{
	if (!p_supper::ObjValid())
		return false;
	if (IsUnValidPtr(db_info_))
		return false;
	if (!db_info_->ObjValid())
		return false;
	return true;
}

std::ostream& operator << (std::ostream& ostm, const stCD_ObjBase& v){
	ostm << "{ addr:" << &v << " vtbl:" << v.vtbl_ << " name:" << v.name_ << " type:" << v.type_ 
		<< " id:" << v.id_ << " packet id:" << v.packet_id_ << " net obj:" << v.net_obj_;
	if (!IsUnValidPtr(v.net_obj_))
	{
		ostm << " net_obj_name:" << Cmnx_NilStr(v.net_obj_->name_);
		if (CD_En_LocalObjType::kNpc == v.GetType())
		{
			auto net_obj_npc = v.net_obj_->CastTo<stCD_NetObj_Npc>();
			if (!IsUnValidPtr(net_obj_npc->db_info_))
			{
				ostm << " obj_db_ch_name:" << CDF_NilStr(net_obj_npc->db_info_->ch_name_)
					<< " obj_db_en_name:" << CDF_NilStr(net_obj_npc->db_info_->en_name_);
			}
		}
	}
	ostm << " pos:" << v.pos_;
	ostm << " dir:" << v.obj_dir_ << " pos_dir_info:" << v.pos_dir_info;
	ostm << " }";
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_LocalObj& v){
	ostm << "{ addr:" << &v << " vtbl:" << v.vtbl_ << " id:" << v.id_ << " status_:" << (int)v.status_;
	if (v.obj_info_)
		ostm << " obj info:" << *v.obj_info_;
	ostm << " }";
	return ostm;
}

bool stCD_LocalObj::ObjValid() const{
	if (CD_En_ObjStatus::kInvalid == status_)
		return false;
	if (IsUnValidPtr(obj_info_))
		return false;
	if (!obj_info_->ObjValid())
		return false;
	return true;
}

std::ostream& operator << (std::ostream& ostm, const stCD_NetObj_Role& v){
	ostm << "{ addr:" << &v;
	v.OutStream(ostm);
	ostm << std::dec;
	ostm << " lvl:" << CD_FuncCall::DecryptValue(&v.lvl_) << " cur_exp:" << CD_FuncCall::DecryptValue(&v.cur_exp_)
		<< " gold:" << CD_FuncCall::DecryptValue(&v.gold_) << " silver:" << CD_FuncCall::DecryptValue(&v.silver_)
		<< " cur_hp:" << CD_FuncCall::DecryptValue(&v.cur_hp_) << " hp:" << CD_FuncCall::DecryptValue(&v.hp_) << " nei_shang:" << v.nei_shang_
		<< " LeftFbCnt:" << CD_FuncCall::DecryptValue(&v.left_fb_cnt_) << " JingSuanDian:" << CD_FuncCall::DecryptValue(&v.jing_suan_point_)
		<< " cur_endure:" << CD_FuncCall::DecryptValue(&v.cur_endure_) << " endure:" << CD_FuncCall::DecryptValue(&v.endure_);
	ostm << " zhan_wei_zhi:" << CD_FuncCall::DecryptValue(&v.zhan_wei_zhi_) << " LianQiCao:" << CD_FuncCall::DecryptValue(&v.lian_qi_cao_);
	ostm << " PoisonState:" << v.poison_status_ << " SerLine:" << v.ser_line_;

	//ostm << " SingleExp:" << CD_FuncCall::DecryptValue(&v.single_exp_) << " DoubleExp:" << CD_FuncCall::DecryptValue(&v.double_exp_);
	ostm << " }";
	return ostm;
}

int stCD_MemGetter::GetValue(const stCD_NetObj_Role* role) const
{
	assert(mem_getter_);
	assert(*(pt_dword*)(const void*)&encrypt_info_ == g_o_role_encrypt_info_);
	assert(role);
	return ((role->*encrypt_info_).*mem_getter_)(stCD_RoleSomeEncryptInfo::MemGetterParam::kNormal);
}

std::ostream& operator << (std::ostream& ostm, const stCD_MapDbInfo& v){
	ostm << " map_db_info_name:" << v.name_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_MapDb& v){
	ostm << "{ addr:" << &v;
	ostm << std::dec;
	ostm << " map_id:" << v.map_id_ << " map_db_info_id:" << v.map_db_info_id_ << " map_name:" << v.map_name_;
	if (v.map_db_info_)
		ostm << *v.map_db_info_;
	ostm << " type:" << v.type_ << " diff_lvl:" << v.diff_lvl_ << " role_lvl:" << v.role_lvl_;
	ostm << " }";
	return ostm;
}

void stCD_PackagesMgr::DebugInfo() const
{
	if (grids_package_)
	{
		LOG_O(Log_trace) << " package:";
		grids_package_->DebugInfo();
	}
	if (grids_store_)
	{
		LOG_O(Log_trace) << " store:";
		grids_store_->DebugInfo();
	}
	if (grids_equip_)
	{
		LOG_O(Log_trace) << " equip:";
		grids_equip_->DebugInfo();
	}
	if (grids_task_)
	{
		LOG_O(Log_trace) << " task:";
		grids_task_->DebugInfo();
	}
	if (grids_su_cai_)
	{
		LOG_O(Log_trace) << " su_cai:";
		grids_su_cai_->DebugInfo();
	}
}

void stCD_GridsMgr::DebugInfo() const
{
	LOG_O(Log_trace) << " grid_max_cnt:" << grids_max_cnt_;
	CD_Traverse::TraverseItems(*this, [](stCD_Item* item, int idx){
		LOG_O(Log_trace) << " idx:" << idx << " item:" << *item;
		return true;
	});
}

std::ostream& operator << (std::ostream& ostm, const stCD_Item& v){
	ostm << " addr:" << &v << " item_cnt：" << v.item_cnt_ << " bind_flag:" << v.bind_flag_ << " serial:" << v.serial_;
	ostm << " item_db:" << *v.item_db_;
	return ostm;
}
std::ostream& operator << (std::ostream& ostm, const stCD_ItemDb& v){
	ostm << "{ addr:" << &v;
	ostm << " item_id:" << v.item_id_ << " name:" << v.item_name_
		<< " category:" << v.category_ << " item_type:" << v.item_type_
		<< " part_type:" << v.part_type_ << " item_lvl:" << v.item_lvl_
		<< " own_max:" << v.own_max_ << " color:" << v.quality_
		<< " carry_max:" << v.carry_max_ << " dui_die_max:" << v.dui_die_max_
		<< " fen_jie_flag:" << v.fen_jie_flag_ << " role_lvl:" << v.role_lvl_;
	if (v.category_ == CD_En_ItemCategory::kIT_Equip && !IsUnValidPtr(v.equip_info_))
		ostm << " equip_info:" << *v.equip_info_;
	ostm << " }";
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_EquipInfo& v){
	ostm << "{ addr:" << &v << " up_info:" << v.up_info_;
	ostm << " }";
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_EquipUpInfo& v){
	ostm << " dst_item_id:" << v.dst_item_id_;
	ostm << " material_info:" << v.material_info_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_MaterialInfo& v){
	ostm << " material_id:" << v.material_item_id_ << " item_cnt:" << v.item_cnt_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_TaskDbUknObj& v){
	ostm << " addr:" << &v;
	if (!IsUnValidPtr(v.task_db_))
	{
		ostm << " task_db:" << *v.task_db_;
	}
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_TaskDb& v){
	ostm << "{ addr:" << &v;
	ostm << std::dec;
	ostm << " task_name:" << v.task_name_ << " task_type:" << v.task_type_;
	ostm << " task_id:" << v.task_id_;
	auto lvl_info = v.GetRoleLvlInfo();
	if (lvl_info)
		ostm << " lvl_info:" << *lvl_info;

	if (!IsUnValidPtr(v.accept_npc_db_))
		ostm << " accept_npc_name:" << v.accept_npc_db_->ch_name_;
	if (!IsUnValidPtr(v.complete_npc_db_))
		ostm << " complete_npc_name:" << v.complete_npc_db_->ch_name_;
	if (CD_En_TaskType::kTT_Shang == v.task_type_)
	{
		ostm << " dst_fb_info:{ ";
		v.dst_fb_info_.Traverse([&ostm](stCD_TaskDstFbInfo** ptr){
			auto dst_fb_info = *ptr;
			if (IsUnValidPtr(dst_fb_info))
				return true;
			ostm << *dst_fb_info;
			return true;
		});
		ostm << " }";
	}

	ostm << " reward:{ ";
	v.task_reward_.Traverse([&ostm](stCD_TaskReward** ptr){
		auto reward = *ptr;
		if (IsUnValidPtr(reward))
			return true;
		ostm << *reward;
		return true;
	});
	ostm << " }";

	ostm << " task_desc:" << CDF_NilStr(v.task_desc_);
	ostm << " }";
	return ostm;
}

const stCD_LvlRange* stCD_TaskDb::GetRoleLvlInfo() const
{
	if (lvl_info_.begin_ != lvl_info_.end_ && !IsUnValidPtr(lvl_info_.begin_))
	{
		auto ptr = *lvl_info_.begin_;
		if (IsUnValidPtr(ptr))
		{
			assert(false);
			return nullptr;
		}
		if (IsUnValidPtr(ptr->role_lvl_range_))
		{
			//assert(false);
			return nullptr;
		}
		return ptr->role_lvl_range_;
	}
	return nullptr;
}

std::ostream& operator << (std::ostream& ostm, const stCD_TaskDstFbInfo& v){
	if (IsUnValidPtr(v.dst_fbs_.begin_))
	{
		ostm << " fb_id:" << (int)v.fb_id_;
	}
	else
	{
		ostm << " fb_ids:(";
		v.dst_fbs_.Traverse([&ostm](stCD_DstFbInfo** ptr){
			auto fb_info = *ptr;
			if (IsUnValidPtr(fb_info))
				return true;
			ostm << *fb_info << ' ';
			return true;
		});
		ostm << ")";
	}
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_DstFbInfo& v){
	ostm << " addr:" << &v << " id:" << v.fb_id_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_LvlRange& v){
	ostm << "(" << v.min_lvl_ << "," << v.max_lvl_ << ")";
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_TaskLvlObj& v){
	if (!IsUnValidPtr(v.role_lvl_range_))
		ostm << " role_lvl_range:" << *v.role_lvl_range_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_TaskReward& v){
	v.reward_items_.Traverse([&ostm](stCD_RewardItem** ptr){
		auto reward = *ptr;
		if (IsUnValidPtr(reward))
			return true;
		if (!IsUnValidPtr(reward->item_db_))
			ostm << " item_db_name:" << reward->item_db_->item_name_;
		ostm << " item_cnt:" << reward->item_cnt_;
		return true;
	});
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_TaskObj& v){
	ostm << "{ addr:" << &v << std::dec;
	ostm << " complete_state:" << v.complete_state_
		<< " is_player_task:" << (int)v.is_player_task_;
	if (!IsUnValidPtr(v.task_db_))
		ostm << " task_db_name:" << v.task_db_->task_name_ << " task_db_id:" << v.task_db_->task_id_;
	ostm << " }";
	return ostm;
}

bool stCD_LvlRange::InRange(int lvl) const
{
	const auto kLvlLimit = kMaxRoleLvl * 10;
	if (min_lvl_ < 0 || min_lvl_ > kLvlLimit)
		return false;
	if (max_lvl_ < 0 || max_lvl_ > kLvlLimit)
	{
		assert(false);
		return false;
	}
	if (max_lvl_ < min_lvl_)
	{
		assert(false);
		return false;
	}
	return lvl >= min_lvl_ && lvl <= max_lvl_;
}

void stCD_ChapterInfo::DebugInfo() const
{
	LOG_O(Log_trace) << " chapter_fb_info:{ ";
	fb_info_.Traverse([](stCD_ChapterFbInfo* chapter_fb_info){
		chapter_fb_info->DebugInfo();
		return true;
	});
	LOG_O(Log_trace) << " }";
}

void stCD_ChapterFbInfo::DebugInfo() const
{
	LOG_O(Log_trace) << "chapter_idx:" << chapter_idx_ << " str_chapter:" << CDF_NilStr(str_chapter_)
		<< " task_fb_id:" << task_fb_id_ << " chapter_task_idx:" << chapter_task_idx_
		<< " task_owner_map_id:" << task_owner_map_id_ << " show_flag:" << show_flag_;
}

void stCD_EnterableFbMgr::DebugInfo() const
{
	LOG_O(Log_trace) << " fb_info:{ ";
	fb_info_.Traverse([](int* fb_id_ptr){
		auto fb_id = *fb_id_ptr;
		auto map_db = CD_FuncCall::FindMapDb(fb_id);
		if (IsUnValidPtr(map_db))
		{
			assert(false);
			return false;
		}
		LOG_O(Log_trace) << " map_id:" << fb_id
			<< " map_db_addr:" << map_db << " map_db_name:" << CDF_NilStr(map_db->map_name_);
		return true;
	});
	LOG_O(Log_trace) << " }";
}

void stCD_FarmDb::DebugInfo() const
{
	LOG_O(Log_trace) << "addr:" << std::hex << this << std::dec
		<< " id:" << id_ << " lvl:" << lvl_ << " point:" << point_ << " silver:" << silver_ << " role_lvl:" << role_lvl_
		<< " name:" << CDF_NilStr(name_) << " desc:" << CDF_NilStr(desc_);
}

void stCD_FarmFacility::DebugInfo() const
{
	LOG_O(Log_trace) << "facility_info: { addr:" << &facility_info_;
	for (auto& v : facility_info_)
	{
		v.DebugInfo();
	}
	LOG_O(Log_trace) << " }";
	LOG_O(Log_trace) << "field_info: { addr:" << &field_info_;
	for (auto& v : field_info_)
	{
		v.DebugInfo();
	}
	LOG_O(Log_trace) << " }";
}

void stCD_CurFarmFacilityInfo::DebugInfo() const
{
	LOG_O(Log_trace) << " cur_elapse_time:" << cur_elapse_time_ << " cur_lvl:" << cur_lvl_;
}

void stCD_CurFarmFieldInfo::DebugInfo() const
{
	LOG_O(Log_trace) << " flag:" << flag_ << " seed_id:" << seed_id_;
}

void stCD_ShopDb::DebugInfo() const
{
	LOG_O(Log_trace) << "shop info{ addr:" << this << " cate_id:" << cate_id_ << " cate_name:" << CDF_NilStr(cate_name_);
	shop_info_.Traverse([](stCD_ShopInfo** ptr){
		auto shop_info = *ptr;
		if (IsUnValidPtr(shop_info))
		{
			assert(false);
			return true;
		}
		shop_info->DebugInfo();
		return true;
	});
	LOG_O(Log_trace) << " }";
}

void stCD_ShopInfo::DebugInfo() const
{
	LOG_O(Log_trace) << "shop_id:" << shop_id_ << " item_id:" << item_id_ << " item_cnt:" << item_cnt_;
}

void stCD_SeedDb::DebugInfo() const
{
	LOG_O(Log_trace) << "addr:" << this << " item_id:" << item_id_ << " item_name:" << item_name_;
}

void stCD_MaterialInfo::DebugInfo() const
{
	LOG_O(Log_trace) << *this;
}

void stCD_CraftDb::DebugInfo() const
{
	LOG_O(Log_trace) << "craft_id:" << craft_id_ << " dst_item_id:" << dst_item_id_ << " material_info:" << material_info_;
}

void stCD_MenufactureDb::DebugInfo() const
{
	LOG_O(Log_trace) << " addr:" << this << " id:" << id_ << " item_name:" << CDF_NilStr(item_name_)
		<< " dst_item_id:" << dst_item_id_ << " material_info:" << material_info_;
}

std::ostream& operator << (std::ostream& ostm, const stCD_MenufactureMaterial& v){
	ostm << " item_id:" << v.material_item_id_ << " item_cnt:" << v.item_cnt_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_GiftObj& v){
	ostm << " flag:" << v.flag_;
	if (!IsUnValidPtr(v.gift_db_))
		ostm << *v.gift_db_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_GiftDb& v){
	ostm << " gift_id:" << v.gift_id_ << " name:" << CDF_NilStr(v.name_) << " desc:" << CDF_NilStr(v.desc_);
	return ostm;
}

void stCD_GiftNode::DebugInfo() const
{
	if (!IsUnValidPtr(gift_))
		LOG_O(Log_trace) << *gift_;
}

void stCD_GridItemInfo::DebugInfo() const
{
	LOG_O(Log_trace) << " slot_key:" << slot_key_ << " item_id:" << item_id_ << " item_cnt:" << item_cnt_;
}

void stCD_MailNode::DebugInfo() const
{
	if (IsUnValidPtr(mail_info_))
		LOG_O(Log_trace) << " mail_id:" << mail_id_ << " mail_info_addr:" << mail_info_;
	else
		LOG_O(Log_trace) << " mail_info:" << *mail_info_;
}

std::ostream& operator << (std::ostream& ostm, const stCD_MailInfo& v){
	ostm << " addr:" << &v << " mail_id:" << v.mail_id_ << " sender:" << CDF_NilStr(v.sender_)
		<< " title:" << CDF_NilStr(v.title_) << " gold:" << v.gold_ << " mail_items:" << v.items_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_MailItem& v){
	ostm << " item_id:" << v.item_id_ << " item_cnt:" << v.item_cnt_;
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_BuffDb& v){
	ostm << " buff_id:" << v.buff_id_ << " name:" << Utf8ToGbk(CDF_NilStr(v.buff_name_)) << " desc:" << CDF_NilStr(v.buff_desc_);
	return ostm;
}

void stCD_BuffDb::DebugInfo() const
{
	LOG_O(Log_trace) << " addr:" << this << *this;
}

void stCD_BuffNode::DebugInfo() const
{
	if (IsUnValidPtr(buf_))
	{
		assert(false);
		return;
	}
	LOG_O(Log_trace) << "buff_id:" << buf_id_ << " buff:" << *buf_;
}

std::ostream& operator << (std::ostream& ostm, const stCD_Buff& v){
	ostm << " dst_packet_id:" << v.dst_packet_id_ << " buf_flag:" << v.flag_;
	if (!IsUnValidPtr(v.buf_db_))
		ostm << " buf_db:" << *v.buf_db_;
	return ostm;
}

void stCD_ShangJinInfo::DebugInfo() const
{
	if (IsUnValidPtr(task_db_))
	{
		assert(false);
		return;
	}
	LOG_O(Log_trace) << " task_id:" << task_id_ << " task_db_name:" << CDF_NilStr(task_db_->task_name_);
}

void stCD_KingQuestInfo::DebugInfo() const
{
	LOG_O(Log_trace) << " king_quest_info:" << *this;
}

std::ostream& operator << (std::ostream& ostm, const stCD_KingQuestInfo& v){
	ostm << "addr:" << &v << " king_fb_id:" << v.king_fb_id_ << " city_id:" << v.city_id_ << " title:" << CDF_NilStr(v.title_);
	auto fb_db = CD_FuncCall::FindMapDb(v.king_fb_id_);
	if (fb_db)
		ostm << " king_fb_db_name:" << CDF_NilStr(fb_db->map_name_);
	fb_db = CD_FuncCall::FindMapDb(v.city_id_);
	if (fb_db)
		ostm << " city_db_name:" << CDF_NilStr(fb_db->map_name_);
	return ostm;
}

bool stCD_UseItemMgr::UseItem(CD_En_ItemPackageCategory item_cate, int slot_idx)
{
	item_cate_ = item_cate;
	slot_idx_ = slot_idx;
	use_flag_ = CD_En_UseItemFlag::kUseItem;
	return true;
}

std::ostream& operator << (std::ostream& ostm, const stCD_NetObj_Collect& v){
	ostm << " addr:" << &v;
	if (!IsUnValidPtr(v.collect_name_))
	ostm << " collect_name:" << CDF_NilStr(*v.collect_name_) << " monster_packet_id:" << v.monster_packet_id_;
	ostm << " str_area:" << CDF_NilStr(v.str_area_) << " ukn_impl_is_empty:" << IsUnValidPtr(v.ukn_impl_) ? true : false;
	if (!IsUnValidPtr(v.room_info_))
	{
		ostm << *v.room_info_;
	}
	v.OutStream(ostm);
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_CollectRoomInfo& v){
	ostm << " room_num:" << v.room_num_;
	return ostm;
}

bool stCD_UiCtrl::IsVisible() const
{
	if (!(flag_ & (int)CD_En_UiCtrlFlag::kVisible))
		return false;
	if (IsUnValidPtr(parent_))
		return true;
	return parent_->IsVisible();
}

int stCD_UiCtrl::GetClassName(pt_sz name_buffer, int max_buffer) const
{
	auto the_name = GetClsName();
	if (!the_name)
		return 0;
	auto len = Cmnx_StrMaxLen(the_name, max_buffer);
	if (len >= (pt_dword)max_buffer)
	{
		assert(false);
		return -1;
	}
	if (!Cmnx_StrCpy(name_buffer, the_name, max_buffer))
	{
		return -1;
	}
	return len;
}

int stCD_UiCtrl::GetFullClassName(pt_sz name_buffer, int max_buffer) const
{
	if (max_buffer < 10)
	{
		assert(false);
		return -1;
	}
	name_buffer[0] = 0;
	return GetFullClassNameImpl(name_buffer, max_buffer);
}

int stCD_UiCtrl::GetFullClassNameImpl(pt_sz name_buffer, int max_buffer) const
{
	auto len = Cmnx_StrLen(name_buffer);
	char buffer[0xff];
	auto res = GetClassName(buffer, sizeof(buffer));
	if (res < 0)
		return res;
	else if (res == 0)
		return res + len;
	if (len)
	{
		if (res >= sizeof(buffer) - 1)
		{
			assert(false);
			return -1;
		}
		strcat_s(buffer, ".");
		res += 1;
	}
	res += len;
	if (res >= max_buffer)
	{
		assert(false);
		return -1;
	}
	strcat_s(buffer, name_buffer);
	if (!Cmnx_StrCpy(name_buffer, buffer, max_buffer))
	{
		assert(false);
		return -1;
	}
	if (IsUnValidPtr(parent_))
		return res;
	return parent_->GetFullClassNameImpl(name_buffer, max_buffer);
}

bool stCD_UiCtrl::MatchTheFullClassName(const pt_csz cls_name[], int cnt) const
{
	int idx = 0;
	for (auto obj_ptr = this; idx < cnt && !IsUnValidPtr(obj_ptr); ++idx, obj_ptr = obj_ptr->parent_)
	{
		auto the_name = obj_ptr->GetClsName();
		if (!the_name)
			return false;
		assert(cls_name[idx]);
		if (0 != strcmp(the_name, cls_name[idx]))
			return false;
	}
	return idx == cnt;
}

pt_csz stCD_UiCtrl::GetClsName() const
{
	if (IsUnValidPtr(name_info_))
		return nullptr;
	if (IsUnValidPtr(name_info_->name_))
	{
		assert(false);
		return nullptr;
	}
	auto the_name = *name_info_->name_;
	if (IsUnValidPtr(the_name))
	{
		assert(false);
		return nullptr;
	}
	return the_name;
}

bool stCD_UiCtrl::HandleSomeMsg(const stCD_UiCtrlMsg* msg, int cnt) const
{
	for (int i = 0; i < cnt; ++i)
	{
		if (!HandleMsg(msg[i]))
			return false;
	}
	return true;
}

stCD_UiCtrlMsg::stCD_UiCtrlMsg()
{
	std::memset(this, 0, sizeof(*this));
}

bool stCD_UiCtrlEdit::PutString(pt_cwsz str) const
{
	if (!str)
	{
		assert(false);
		return false;
	}
	while (*str != 0)
	{
		if (!PutChar(*str))
		{
			assert(false);
			return false;
		}
		++str;
	}
	return true;
}

std::ostream& operator << (std::ostream& ostm, const stCD_UiMng& v){
	ostm << " addr:" << &v << " name:" << CDF_NilStr(v.name_);
	return ostm;
}

std::ostream& operator << (std::ostream& ostm, const stCD_UiMng_Minimap& v){
	ostm << (const stCD_UiMng&)v;
	ostm << " room_num:" << v.room_num_;
	return ostm;
}
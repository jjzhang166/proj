#include "stdafx.h"
#include "GItem.h"
#include <Common/LogOut.h>
#include "GClass.h"
#include <Common/MemOper.h>
#include "GConst.h"
#include <boost/regex.hpp>
#include "GPlayer.h"
#include <Protocol/P_Defines.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>


bool operator == ( const GItem& lhs, const GItem& rhs ){
	return lhs.item_serial_ == rhs.item_serial_ && lhs.item_cnt_ == rhs.item_cnt_ && lhs.item_id_ == rhs.item_id_;
}

GItemMgrBase::GItemMgrBase()
{
	tab_category_ = CD_En_TabCategory::kTC_Invalid;
	grids_max_cnt_ = 0;
	grids_mgr_ = nullptr;
}

bool GItemMgrBase::UpdateAllImpl()
{
	assert(false);
	if (items_.empty())
		return false;
	for ( auto& item : items_ )
	{
		if ( !item.Update() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "item update failed";
			assert(false);
			return false;
		}
	}
	return true;
}

GItemMgrBase::ContT& GItemMgrBase::GetItems()
{
	return items_;
}

const GItemMgrBase::ContT& GItemMgrBase::GetItems() const
{
	return items_;
}

CD_En_TabCategory GItemMgrBase::GetTabCategory() const
{
	return tab_category_;
}

GItemMgrBase::GameItemIterPtr GItemMgrBase::TraverseIf( const DoAnyGameItemFuncT& func )
{
	return MakeIterator<ItemTraTraits>(items_, func);
}

GItem* GItemMgrBase::FindIf( const DoAnyGameItemFuncT& func )
{
	for ( auto& item : items_ )
	{
		if (func(item))
			return &item;
	}
	return nullptr;
}

void GItemMgrBase::DebugInfo() const
{
	LOG_O(Log_info) << "物品总数：" << items_.size();
	for ( auto& item : items_ )
	{
		item.DebugInfo();
	}
}

GItem* GItemMgrBase::FindByName( const std::string& item_name, bool is_bind )
{
	using namespace GType;
	if (item_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& item : items_ )
	{
		if ( item.item_name_ == item_name )
		{
			if ( !is_bind )
			{
				if (item.IsFree())
					return &item;
			}
			else
				return &item;
		}
	}
	return nullptr;
}

GItem* GItemMgrBase::FindByName_Lvl( const std::string& item_name, int item_lvl )
{
	if (item_name.empty())
	{
		assert(false);
		return nullptr;
	}
	if ( item_lvl <= 0 )
	{
		assert(false);
		return nullptr;
	}
	for ( auto& item : items_ )
	{
		if ( item_lvl == item.item_lvl_ && item_name == item.item_name_ )
			return &item;
	}
	return nullptr;
}

bool GItemMgrBase::RebuildAllImpl()
{
	items_.clear();
	if (IsUnValidPtr(grids_mgr_))
	{
		assert(false);
		return false;
	}
	grids_max_cnt_ = grids_mgr_->grids_max_cnt_;
	if (grids_max_cnt_ <= 20 || grids_max_cnt_ >= 99999999)
	{
		assert(false);
		return false;
	}

	CD_Traverse::TraverseItems(*grids_mgr_, [this](stCD_Item* item, int idx){
		GItem item_tmp;
		item_tmp.the_item_ = item;
		item_tmp.item_idx_ = idx;
		item_tmp.item_mgr_ptr_ = this;
		if (!item_tmp.Update())
			return false;
		items_.push_back(item_tmp);
		return true;
	});
	return true;
}

int GItemMgrBase::GetGridMaxCnt() const
{
	return grids_max_cnt_;
}

int GItemMgrBase::GetFreeSlotIndex() const
{
	std::vector<int> item_indexes;
	for ( auto& item : items_ )
	{
		item_indexes.push_back(item.item_idx_);
	}
	std::sort(item_indexes.begin(), item_indexes.end());
	
	int idx = 0;
	for ( auto item_idx : item_indexes )
	{
		if ( idx < item_idx )
			return idx;
		++idx;
	}
	if ( idx < grids_max_cnt_ )
		return idx;
	return -1;
}

GItem* GItemMgrBase::FindBySerial( const LONGLONG& item_serial )
{
	for ( auto& item : items_ )
	{
		if ( item_serial == item.item_serial_ )
			return &item;
	}
	return nullptr;
}

GItem* GItemMgrBase::FindBySlotIndex( int slot_index )
{
	for ( auto& item : items_ )
	{
		if ( slot_index == item.item_idx_ )
			return &item;
	}
	return nullptr;
}

int GItemMgrBase::GetFreeCnt() const
{
	auto cnt = grids_max_cnt_ - items_.size();
	assert(cnt >= 0);
	return cnt;
}

GItem* GItemMgrBase::FindById( int item_id )
{
	for ( auto& item : items_ )
	{
		if ( item.item_id_ == item_id )
			return &item;
	}
	return nullptr;
}

int GItemMgrBase::GetTheItemTotalCnt( int item_id )
{
	int cnt = 0;
	for ( auto& v : items_ )
	{
		if ( v.item_id_ == item_id )
			cnt += v.item_cnt_;
	}
	return cnt;
}

GItemMgrBase::GameItemIterPtr GItemMgrBase::TraverseByMatchName( const std::string& item_name )
{
	return TraverseIf([&item_name]( const GItem& item ){
		return item.item_name_.find(item_name) != std::string::npos;
	});
}

GItemMgrBase::GameItemIterPtr GItemMgrBase::TraverseByItemId( int item_id )
{
	return TraverseIf([item_id]( const GItem& item ){
		return item.item_id_ == item_id;
	});
}

GItemMgrBase::GameItemIterPtr GItemMgrBase::TraverseByName( const std::string& item_name )
{
	return TraverseIf([&item_name]( const GItem& item ){
		return item.item_name_ == item_name;
	});
}

bool GItemMgrBase::AllItemNextTo() const
{
	if (items_.empty())
		return true;
	auto items_tmp = items_;
	items_tmp.sort([]( const GItem& lhs, const GItem& rhs ){
		return lhs.item_idx_ < rhs.item_idx_;
	});
	int idx = 0;
	for ( auto& item : items_tmp )
	{
		if ( idx != item.item_idx_ )
			return false;
		++idx;
	}
	return true;
}

int GItemMgrBase::GetTheItemTotalCnt( const std::string& item_name )
{
	if (item_name.empty())
	{
		assert(false);
		return 0;
	}
	int cnt = 0;
	for ( auto& v : items_ )
	{
		if ( v.item_name_ == item_name )
			cnt += v.item_cnt_;
	}
	return cnt;
}

GItem* GItemMgrBase::FindByMatchName( const std::string& item_name, bool is_bind )
{
	using namespace GType;
	if (item_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& item : items_ )
	{
		if ( item.item_name_.find(item_name) != std::string::npos )
		{
			if ( !is_bind )
			{
				if (item.IsFree())
					return &item;
			}
			else
				return &item;
		}
	}
	return nullptr;
}

void GItemMgrBase::DoRebuildUpdateClear()
{
	items_.clear();
}

GItemMgrBase::GameItemIterPtr GItemMgrBase::TraverseByMatchName_Cnt( const std::string& item_name, int cnt )
{
	return TraverseIf([&item_name, cnt]( const GItem& item ){
		if ( item.item_name_.find(item_name) == std::string::npos )
			return false;
		if ( item.item_cnt_ < cnt )
			return false;
		return true;
	});
}

GItem* GItemMgrBase::FindByName_Cnt( const std::string& item_name, bool is_bind, int item_cnt )
{
	using namespace GType;
	if (item_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& item : items_ )
	{
		if ( item.item_name_ == item_name && item.item_cnt_ >= item_cnt )
		{
			if ( !is_bind )
			{
				if (item.IsFree())
					return &item;
			}
			else
				return &item;
		}
	}
	return nullptr;
}

bool GItem::UpdateImpl()
{
	
	using namespace GType;
	if (IsUnValidPtr(the_item_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(the_item_->item_db_))
	{
		assert(false);
		return false;
	}
	if (!From(*the_item_->item_db_))
	{
		assert(false);
		return false;
	}

	item_cnt_ = the_item_->item_cnt_;
	if ( 0 == item_cnt_ )
		item_cnt_ = 1;
	if ( item_cnt_ <= 0 )
	{
		LOG_O(Log_debug) << "物品数量不能小于等于0，物品名：" << item_name_;
		assert(false);
	}

	item_serial_ = the_item_->serial_;
	bind_type_ = the_item_->bind_flag_;
	if (bind_type_ >= CD_En_BindFlag::kIBT_Invalid)
	{
		LOG_O(Log_debug) << "未知物品绑定类型：" << bind_type_ << "  物品名：" << item_name_;
	}
	sell_price_ = the_item_->item_db_->GetItemSellPrice();
	assert(sell_price_ >= 0);
	return true;
}

GItem::GItem()
{
	the_item_ = nullptr;
	item_idx_ = -1;
	item_mgr_ptr_ = nullptr;
	item_template_ = nullptr;
	sell_price_ = 0;
	item_serial_ = 0;
	item_cnt_ = 0;
	bind_type_ = CD_En_BindFlag::kIBT_Invalid;
}

void GItem::DebugInfo() const
{
	LOG_O(Log_info) << "物品名：" << item_name_ << "，物品数量：" << item_cnt_ << "，物品等级：" << item_lvl_;
}

GType::enEquipPartIndex GItem::ItemIdx2EquipPartIdx() const
{
	assert(item_idx_ < GType::kEPI_Invalid);
	if ( item_idx_ >= GType::kEPI_Invalid || item_idx_ < 0 )
		return GType::kEPI_Invalid;
	return GType::enEquipPartIndex(item_idx_);
}

const ItemDb* GItem::GetItemTemplate()
{
	if (item_template_)
		return item_template_;
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	item_template_ = item_mgr.FindById(item_id_);
	return item_template_;
}

GType::enEquipPartIndex GItem::Equip2EquipPartIdx() const
{
	using namespace GType;
	if ( !item_mgr_ptr_ )
	{
		assert(false);
		return kEPI_Invalid;
	}
	auto tab_cate = item_mgr_ptr_->GetTabCategory();
	if (CD_En_TabCategory::kTC_Equip == tab_cate)
		return ItemIdx2EquipPartIdx();
	else if (CD_En_TabCategory::kTC_Store == tab_cate || CD_En_TabCategory::kTC_Package == tab_cate || CD_En_TabCategory::kTC_SuCai == tab_cate)
	{
		if (!IsEquip())
			return kEPI_Invalid;
		if (CD_En_ItemType::kEPT_Weapon == equip_part_type_)
			return kEPI_Weapon;
		else if (CD_En_ItemType::kEPT_Defense == equip_part_type_)
		{
			auto equip_defense = EquipSubType2Defense();
			if ( kEPSTDEF_Invalid == equip_defense )
				return kEPI_Invalid;
			switch (equip_defense)
			{
			case GType::kEPSTDEF_Head:
				return kEPI_Head;
			case GType::kEPSTDEF_Heart:
				return kEPI_Heart;
			case GType::kEPSTDEF_Hand:
				return kEPI_Hand;
			case GType::kEPSTDEF_Pocket:
				return kEPI_Pocket;
			case GType::kEPSTDEF_Foot:
				return kEPI_Foot;
			}
		}
		else if (CD_En_ItemType::kEPT_Decorate == equip_part_type_)
		{
			auto equip_decorate = EquipSubType2Decorate();
			if ( kEPSTDEC_Invalid == equip_decorate )
				return kEPI_Invalid;
			switch (equip_decorate)
			{
			case GType::kEPSTDEC_Ring:
				return kEPI_Ring;
			case GType::kEPSTDEC_Necklace:
				return kEPI_Necklace;
			case GType::kEPSTDEC_HuShi:
				return kEPI_HuShi;
			}
		}
	}
	return kEPI_Invalid;
}

bool GItem::IsEquip() const
{
	return item_type_ == CD_En_ItemCategory::kIT_Equip;
}

CD_En_EquipPartSubType_Weapon GItem::EquipSubType2Weapon() const
{
	using namespace GType;
	if (CD_En_ItemType::kEPT_Weapon == equip_part_type_)
		return equip_part_sub_type_;
	return CD_En_EquipPartSubType_Weapon::kEPSTW_Invalid;
}

GType::enEquipPartSubType_Defense GItem::EquipSubType2Defense() const
{
	using namespace GType;
	if (CD_En_ItemType::kEPT_Defense == equip_part_type_)
		return (enEquipPartSubType_Defense)equip_part_sub_type_;
	return kEPSTDEF_Invalid;
}

GType::enEquipPartSubType_Decorate GItem::EquipSubType2Decorate() const
{
	using namespace GType;
	if (CD_En_ItemType::kEPT_Decorate == equip_part_type_)
		return (enEquipPartSubType_Decorate)equip_part_sub_type_;
	return kEPSTDEC_Invalid;
}

pt_dint GItem::CalcItemGoodsValue() const
{
	using namespace GType;
	pt_dint the_value = player_lvl_ * 3;
	the_value += item_lvl_ * 2;
	the_value += (int)item_quality_ * 5;

	if (CD_En_EquipPartSubType_Weapon::kEPSTW_TaiDao == EquipSubType2Weapon())
		the_value += 1000;
	if (GPlayerMe::GetMe().FindAutoWearWhiteListItem(item_name_))
		the_value += 100000;
	return the_value;
}

bool GItem::IsSuCai() const
{
	using namespace GType;
	if (CD_En_ItemCategory::kIT_Material != item_type_)
		return false;
	if (CD_En_ItemType::kEPT_SuCai != equip_part_type_)
		return false;
	if (CD_En_EquipPartSubType_Weapon::kEPSTW_DaJian == equip_part_sub_type_ || CD_En_EquipPartSubType_Weapon::kEPSTW_ChuiZi == equip_part_sub_type_ || 
		CD_En_EquipPartSubType_Weapon::kEPSTW_ChangQiang == equip_part_sub_type_ || CD_En_EquipPartSubType_Weapon::kEPSTW_Nu == equip_part_sub_type_)
		return true;
	return  false;
}

bool GItem::CanBeUsed() const
{
	if ( item_cnt_ <= 0 )
		return false;
	auto& gpm = GPlayerMe::GetMe();
	if ( player_lvl_ > 0 && gpm.GetRoleLvl() < player_lvl_ )
		return false;
	return true;
}

void GItem::DoUpdateClear()
{
	item_name_.clear();
	item_cnt_ = 0;
}

bool GItem::IsFree() const
{
	if (CD_En_BindFlag::kIBT_Free == bind_type_ || CD_En_BindFlag::kIBT_CanBind == bind_type_)
		return true;
	return false;
}

bool GItemMgr_Package::RebuildAllImpl()
{
	auto packages_mgr = CD_FuncCall::GetPackagesMgr();
	if (IsUnValidPtr(packages_mgr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(packages_mgr->grids_package_))
	{
		assert(false);
		return false;
	}
	grids_mgr_ = packages_mgr->grids_package_;
	return __super::RebuildAllImpl();
}

GItemMgr_Package::GItemMgr_Package()
{
	tab_category_ = CD_En_TabCategory::kTC_Package;
}

int GItemMgr_Package::GetFreeSlotIndex() const
{
	std::vector<int> item_indexes;
	for ( auto& item : items_ )
	{
		item_indexes.push_back(item.item_idx_);
	}
	std::sort(item_indexes.begin(), item_indexes.end());

	int idx = grids_max_cnt_ - 1;
	assert(idx >= 0);
	for ( auto it = item_indexes.rbegin(); it != item_indexes.rend(); ++it, --idx )
	{
		if ( idx > *it )
			return idx;
	}
	if ( idx >= 0 )
		return idx;
	return -1;
}

bool GItemMgr_Equip::RebuildAllImpl()
{
	auto packages_mgr = CD_FuncCall::GetPackagesMgr();
	if (IsUnValidPtr(packages_mgr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(packages_mgr->grids_equip_))
	{
		assert(false);
		return false;
	}
	grids_mgr_ = packages_mgr->grids_equip_;
	return __super::RebuildAllImpl();
}

GItemMgr_Equip::GItemMgr_Equip()
{
	tab_category_ = CD_En_TabCategory::kTC_Equip;
}

bool GItemMgr_Store::RebuildAllImpl()
{
	auto packages_mgr = CD_FuncCall::GetPackagesMgr();
	if (IsUnValidPtr(packages_mgr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(packages_mgr->grids_store_))
	{
		assert(false);
		return false;
	}
	grids_mgr_ = packages_mgr->grids_store_;
	return __super::RebuildAllImpl();
}

GItemMgr_Store::GItemMgr_Store()
{
	tab_category_ = CD_En_TabCategory::kTC_Store;
}

bool ItemDbMgr::UpdateAllImpl()
{
	assert(false);
	return false;
}

bool ItemDbMgr::RebuildAll()
{
	if ( !items_.empty() )
		return true;
	return __super::RebuildAll();
}

const ItemDb* ItemDbMgr::FindById( int item_id ) const
{
	for ( auto& v : items_ )
	{
		if ( item_id == v.item_id_ )
			return &v;
	}
	return nullptr;
}

const ItemDb* ItemDbMgr::FindByName( const std::string& item_name ) const
{
	if (item_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& v : items_ )
	{
		if ( item_name == v.item_name_ )
			return &v;
	}
	return nullptr;
}

const ItemDb* ItemDbMgr::FindByMaterial(int material_item_id, int dst_item_lvl, CD_En_ItemQuality dst_item_quality) const
{
	auto it = std::find_if(items_.begin(), items_.end(), [=]( const ItemDb& item_template ){
		if (item_template.equip_lvl_up_.empty())
			return false;
		for ( auto& v : item_template.equip_lvl_up_ )
		{
			for ( auto& material : v.materials_ )
			{
				if ( material.material_item_id_ == material_item_id )
				{
					auto dst_item_template = FindById(v.dst_item_id_);
					if ( !dst_item_template )
					{
						assert(false);
						continue;
					}
					if ( dst_item_template->item_lvl_ >= dst_item_lvl && dst_item_template->item_quality_ >= dst_item_quality )
						return true;
				}
			}
			
		}
		return false;
	});
	if ( it != items_.end() )
		return &*it;
	return nullptr;
}

void ItemDbMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "物品模板总数：" << items_.size();
	for ( auto& item : items_ )
	{
		item.DebugInfo();
	}
}

bool ItemDbMgr::RebuildAllImpl()
{
	return CD_Traverse::TraverseItemDb([this](stCD_ItemDb* item_db){
		ItemDb item_tmp;
		item_tmp.item_db_ = item_db;
		if (!item_tmp.Update())
			return false;
		items_.push_back(item_tmp);
		return true;
	}) > 500;
}

void ItemDbMgr::DoRebuildUpdateClear()
{
	items_.clear();
}

const ItemDb* ItemDbMgr::FindByMatchName( const std::string& item_name ) const
{
	if (item_name.empty())
	{
		assert(false);
		return nullptr;
	}
	for ( auto& v : items_ )
	{
		if ( v.item_name_.find(item_name) != std::string::npos )
			return &v;
	}
	return nullptr;
}

ItemDb::ItemDb()
{
	item_db_ = nullptr;
}

bool ItemDb::UpdateImpl()
{
	if (IsUnValidPtr(item_db_))
	{
		assert(false);
		return false;
	}
	if (!From(*item_db_))
	{
		assert(false);
		return false;
	}
	equip_lvl_up_.clear();
	if (CD_En_ItemCategory::kIT_Equip == item_type_)
	{
		if ( !IsUnValidPtr(item_db_->equip_info_) )
		{
			item_db_->equip_info_->up_info_.Traverse([this](stCD_EquipUpInfo* up_info){
				stEquipLvlUpInfo equip_tmp;
				equip_tmp.dst_item_id_ = up_info->dst_item_id_;
				up_info->material_info_.Traverse([&equip_tmp](stCD_MaterialInfo* material_info){
					equip_tmp.materials_.push_back(*material_info);
					return true;
				});
				equip_lvl_up_.push_back(equip_tmp);
				return true;
			});
		}
	}
	return true;
}

const stCD_MaterialInfo* ItemDb::FindLvlUpInfoByMaterialId( int material_item_id ) const
{
	for ( auto& v : equip_lvl_up_ )
	{
		for ( auto& material : v.materials_ )
		{
			if ( material.material_item_id_ == material_item_id )
				return &material;
		}
	}
	return nullptr;
}

void ItemDb::DebugInfo() const
{
	LOG_O(Log_debug) << "id:" << item_id_ << " name:" << item_name_ << " lvl:" << item_lvl_ << "role lvl:" << player_lvl_;
}

void ItemDb::DoUpdateClear()
{
	item_name_.clear();
	item_id_ = 0;
}

bool GMailMgr::RebuildAllImpl()
{
	mails_.clear();
	CD_Traverse::TraverseMails([this](stCD_Node* node){
		auto ptr = node->CastTo<stCD_MailNode>();
		if (IsUnValidPtr(ptr->mail_info_))
		{
			assert(false);
			return true;
		}
		GMailItem tmp;
		tmp.mail_ = ptr->mail_info_;
		if (!tmp.Update())
		{
			assert(false);
			return false;
		}
		mails_.push_back(tmp);
		return true;
	});
	return true;
}

const GMailItem* GMailMgr::FindBySerial(const pt_dint& mail_serial)
{
	for ( auto& v : mails_ )
	{
		if ( v.mail_serial_ == mail_serial )
			return &v;
	}
	return nullptr;
}

const std::vector<GMailItem>& GMailMgr::GetMails() const
{
	return mails_;
}

bool GMailMgr::UpdateAllImpl()
{
	assert(false);
	return false;
}

bool GMailItem::UpdateImpl()
{
	if (IsUnValidPtr(mail_))
	{
		assert(false);
		return false;
	}
	mail_serial_ = mail_->mail_id_;
	mail_title_.assign(CDF_EmptyStr(mail_->title_));

	gold_ = mail_->gold_;

	items_.clear();
	mail_->items_.Traverse([this](stCD_MailItem* item){
		stItem item_tmp = {
			item->item_id_,
			item->item_cnt_
		};
		items_.push_back(item_tmp);
		return true;
	});
	return true;
}

GMailItem::GMailItem()
{
	gold_ = 0;
	mail_ = nullptr;
}

void GMailItem::DoUpdateClear()
{
	items_.clear();
	gold_ = 0;
	mail_serial_ = 0;
	mail_title_.clear();
}

GItemMgr_Task::GItemMgr_Task()
{
	tab_category_ = CD_En_TabCategory::kTC_Task;
}

bool GItemMgr_Task::RebuildAllImpl()
{
	auto packages_mgr = CD_FuncCall::GetPackagesMgr();
	if (IsUnValidPtr(packages_mgr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(packages_mgr->grids_task_))
	{
		assert(false);
		return false;
	}
	grids_mgr_ = packages_mgr->grids_task_;
	return __super::RebuildAllImpl();
}

ItemDb::stEquipLvlUpInfo::stEquipLvlUpInfo()
{
	dst_item_id_ = 0;
}

bool GManufactureDbMgr::Update()
{
	
	if ( !items_facture_info_.empty() )
		return true;
	return __super::Update();
}

const GManufactureDbMgr::stItemFactureDb* GManufactureDbMgr::FindByMaterial(int material_item_id, int dst_item_lvl, CD_En_ItemQuality dst_item_quality) const
{
	auto& item_mgr = ItemDbMgr::GetMe();
	auto it = std::find_if(items_facture_info_.begin(), items_facture_info_.end(), [=, &item_mgr]( const stItemFactureDb& facture_info ){
		if (facture_info.materials_.empty())
			return false;
		for ( auto& v : facture_info.materials_ )
		{
			if ( v.material_item_id_ == material_item_id )
			{
				auto dst_item_template = item_mgr.FindById(facture_info.dst_item_id_);
				if ( !dst_item_template )
				{
					assert(false);
					continue;
				}
				if ( dst_item_template->item_lvl_ >= dst_item_lvl && dst_item_template->item_quality_ >= dst_item_quality )
					return true;
			}
		}
		return false;
	});
	if ( it != items_facture_info_.end() )
		return &*it;
	return nullptr;
}

const GManufactureDbMgr::stItemFactureDb* GManufactureDbMgr::FindByDstItemName( const std::string& dst_item_name ) const
{
	if (dst_item_name.empty())
	{
		assert(false);
		return nullptr;
	}
	auto it = std::find_if(items_facture_info_.begin(), items_facture_info_.end(), [&dst_item_name]( const stItemFactureDb& facture_info ){
		return facture_info.dst_item_name_ == dst_item_name;
	});
	if ( it != items_facture_info_.end() )
		return &*it;
	return nullptr;
}

void GManufactureDbMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "调和总数：" << items_facture_info_.size();
	for ( auto& item : items_facture_info_ )
	{
		LOG_O(Log_debug) << "facture id:" << item.facture_id_ << " dst name:" << item.dst_item_name_;
	}
}

bool GManufactureDbMgr::UpdateImpl()
{
	items_facture_info_.clear();
	CD_Traverse::TraverseMenufactureDb([this](stCD_MenufactureDb* db){
		stItemFactureDb facture_info;
		facture_info.facture_id_ = db->id_;
		facture_info.dst_item_name_.assign(CDF_EmptyStr(db->item_name_));
		facture_info.dst_item_id_ = db->dst_item_id_;
		db->material_info_.Traverse([&facture_info](stCD_MenufactureMaterial* material){
			stCD_MaterialInfo tmp;
			tmp.material_item_id_ = material->material_item_id_;
			tmp.item_cnt_ = material->item_cnt_;
			facture_info.materials_.push_back(tmp);
			return true;
		});
		items_facture_info_.push_back(facture_info);
		return true;
	});
	if (items_facture_info_.empty())
	{
		assert(false);
		return false;
	}
	return true;
}

void GManufactureDbMgr::DoUpdateClear()
{
	items_facture_info_.clear();
}

bool GSupplyBox::UpdateImpl()
{
	items_info_.clear();
	CD_Traverse::TraverseSupplyBox([this](stCD_GridItemInfo* item){
		stSupplyItemInfo tmp;
		tmp.slot_key_ = item->slot_key_;
		tmp.item_id_ = item->item_id_;
		tmp.item_cnt_ = item->item_cnt_;
		items_info_.push_back(tmp);
		return true;
	});

	//assert(!items_info_.empty());
	return true;
}

void GSupplyBox::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "补济箱总数：" << items_info_.size();
	auto& item_mgr = ItemDbMgr::GetMe();
	for ( auto& v : items_info_ )
	{
		auto the_item = item_mgr.FindById(v.item_id_);
		if ( !the_item )
		{
			LOG_O(Log_debug) << "补给箱中不存在该物品ID：" << v.item_id_;
			continue;
		}
		LOG_O(Log_debug) << "slot idx:" << v.slot_key_ << " item id:" << v.item_id_ << " item cnt:" << v.item_cnt_ << " 物品名：" << the_item->item_name_;
	}
}

void GSupplyBox::DoUpdateClear()
{
	items_info_.clear();
}

bool GItemMgr_SuCai::RebuildAllImpl()
{
	auto packages_mgr = CD_FuncCall::GetPackagesMgr();
	if (IsUnValidPtr(packages_mgr))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(packages_mgr->grids_su_cai_))
	{
		assert(false);
		return false;
	}
	grids_mgr_ = packages_mgr->grids_su_cai_;
	return __super::RebuildAllImpl();
}

GItemMgr_SuCai::GItemMgr_SuCai()
{
	tab_category_ = CD_En_TabCategory::kTC_SuCai;
}

bool GGiftItemMgr::RebuildAllImpl()
{
	SetRootNode(CD_FuncCall::GetGiftRootNode());
	return __super::RebuildAllImpl();
}

void GGiftItemMgr::DebugInfo() const
{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "gift total cnt:" << btree_.size();
	for ( auto& v : btree_ )
	{
		v.DebugInfo();
	}
}

const GGiftItem* GGiftItemMgr::FindGift( int id ) const
{
	for ( auto& v : btree_ )
	{
		if ( v.gift_bag_id_ == id )
			return &v;
	}
	return nullptr;
}

bool GGiftItem::UpdateImpl()
{
	if (IsUnValidPtr(GetCurNode()))
	{
		assert(false);
		return false;
	}
	auto node = GetCurNode()->CastTo<stCD_GiftNode>();
	if (IsUnValidPtr(node->gift_))
	{
		assert(false);
		return false;
	}
	if (IsUnValidPtr(node->gift_->gift_db_))
	{
		assert(false);
		return false;
	}
	
	gift_bag_id_ = node->gift_id_;
	flag_ =  node->gift_->flag_;
	assert(1 == flag_ || 2 == flag_);
	gifg_name_.assign(CDF_EmptyStr(node->gift_->gift_db_->name_));
	return true;
}

GGiftItem::GGiftItem()
{
	gift_bag_id_ = 0;
	flag_ = 0;
}

void GGiftItem::DebugInfo() const
{
	LOG_O(Log_debug) << "id:" << gift_bag_id_ << " flag:" << flag_ << " gift name:" << gifg_name_;
}

bool GGiftItem::CanGain() const
{
	return 1 == flag_;
}

void GGiftItem::DoUpdateClear()
{
	gifg_name_.clear();
}

ItemInfoBase::ItemInfoBase()
{
	item_lvl_ = 0;
	own_limit_ = 0;
	carry_limit_ = -1;
	item_type_ = CD_En_ItemCategory::kIT_Invalid;
	equip_part_type_ = CD_En_ItemType::kEPT_Invalid;
	equip_part_sub_type_ = CD_En_EquipPartSubType_Weapon::kEPSTW_Invalid;
	player_lvl_ = -1;
	item_quality_ = CD_En_ItemQuality::kIQ_Invalid;
	item_id_ = 0;
	die_jia_limit_ = 0;
}

bool ItemInfoBase::From(const stCD_ItemDb& item_db)
{
	item_name_.assign(CDF_EmptyStr(item_db.item_name_));
	item_lvl_ = item_db.item_lvl_;
	own_limit_ = item_db.own_max_;
	carry_limit_ = item_db.carry_max_;
	if (own_limit_ < 0 || carry_limit_ < 0)
	{
		assert(false);
		return false;
	}
	die_jia_limit_ = item_db.dui_die_max_;
	assert(die_jia_limit_ >= 1);

	item_type_ = item_db.category_;
	equip_part_type_ = item_db.item_type_;
	equip_part_sub_type_ = item_db.part_type_;
	player_lvl_ = item_db.role_lvl_;
	if (player_lvl_ < 0 || player_lvl_ > 1000)
	{
		if (item_name_ != "晓风头盔")
			LOG_O(Log_debug) << "玩家等级不能小于0，玩家等级：" << player_lvl_ << " item name:" << item_name_;
	}
	item_quality_ = item_db.quality_;
	if (item_quality_ < CD_En_ItemQuality::kIQ_Red || item_quality_ > CD_En_ItemQuality::kIQ_JinYellow)
	{
		LOG_O(Log_warning) << "未知物品品质：" << item_quality_ << "  物品名：" << item_name_;
	}
	item_id_ = item_db.item_id_;
	return true;
}

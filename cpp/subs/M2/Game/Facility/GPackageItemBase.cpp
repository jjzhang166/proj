#include "stdafx.h"
#include "GPackageItemBase.h"
#include "GItem.h"
#include "GPlayer.h"
#include "../Scmd.h"

GItemBase::GItemBase(enItemCate item_cate)
{
	item_entity_ = nullptr;
	item_id_ = 0;
	item_cnt_ = 1;
	max_stack_cnt_ = 1;
	is_identified_ = true;
	is_bind_ = false;
	item_cate_ = item_cate;
	item_color_ = kIC_Non;
	item_inventory_type_ = kIT_Invalid;
}

GItemBase::GItemBase() : GItemBase(kICATE_Other)
{
}

bool GItemBase::UpdateImpl()
{
	if (!item_entity_)
	{
		assert(false);
		return false;
	}
	auto attr_base = item_entity_->GetAttrBase();
	if (!attr_base)
	{
		assert(false);
		return false;
	}
	auto attr_db_base = attr_base->GetAttrDb();
	if (!attr_db_base)
	{
		assert(false);
		return false;
	}
	if (!CD_SafeName(attr_db_base->name_))
	{
		assert(false);
		return false;
	}
	item_name_ = attr_db_base->name_.GetStdString();
	is_bind_ = attr_base->is_bind_;

	auto attr_mod = item_entity_->GetAttrMods();
	if (attr_mod)
	{
		astrict_.level_ = attr_mod->player_lvl_;
		is_identified_ = attr_mod->is_identified_;
		item_color_ = attr_mod->item_color_;
		if (item_color_ <= kIC_Non)
		{
			assert(false);
			return false;
		}
	}
	bool has_price = true;
	auto attr_stack = item_entity_->GetAttrStack();
	if (!IsUnValidPtr(attr_stack))
	{
		item_cnt_ = attr_stack->item_cnt_;
		if (item_cnt_ <= 0)
		{
			assert(false);
			return false;
		}
		auto db_stack = attr_stack->attr_db_;
		if (IsUnValidPtr(db_stack))
		{
			assert(false);
			return false;
		}

		max_stack_cnt_ = db_stack->max_stack_cnt_;
		if (max_stack_cnt_ <= 0)
		{
			assert(false);
			return false;
		}
		if (IsUnValidPtr(db_stack->ukn_price_))
			has_price = false;
	}
	if (has_price)
	{
		stCD_ItemPriceContT price;
		auto the_res = attr_base->GetPrice(price);
		assert(the_res == &price);
		for (auto& v : price)
		{
			stSellPrice tmp = {
				v.GetDbItemBase(), v.item_cnt_
			};
			if (IsUnValidPtr(tmp.attr_db_item_base_))
			{
				assert(false);
				continue;
			}
			assert(tmp.item_cnt_ > 0 && tmp.item_cnt_ < 10000);
			price_.push_back(tmp);
		}
	}

	item_type_ = item_entity_->GetItemType();
	if (item_type_ == kITYPE_Invalid)
	{
		assert(false);
		return false;
	}
	if (item_type_ == kITYPE_Unarmed)
		assert(false);

	auto item_base = item_entity_->GetBaseItemType();
	if (!item_base)
	{
		assert(false);
		return false;
	}
	item_size_.SetSize(item_base->grid_width_, item_base->grid_height_);
	if (item_size_.AnyLess(1))
	{
		assert(false);
		return false;
	}

	auto attr_requir = item_entity_->GetAttrDbAttributeRequirements();
	if (!IsUnValidPtr(attr_requir))
	{
		astrict_.brain_ = attr_requir->brain_;
		astrict_.power_ = attr_requir->power_;
		astrict_.smart_ = attr_requir->smart_;
	}

	auto ui_tips = CD_Interface::GetFirstItemTips();
	if (!ui_tips)
	{
		assert(false);
		return false;
	}
	auto tips = ui_tips->FillItemTipsInfo(item_entity_);
	if (!tips)
	{
		assert(false);
		return false;
	}
	if (!item_tips_.From(tips))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GItemBase::Init(stCD_EntityItemBased& item_entity, pt_dword item_id, const GPackageBasePtr& package, stCD_ItemGridsInfo* grid_info)
{
	item_entity_ = &item_entity;
	package_ = package;
	item_id_ = item_id;
	item_inventory_type_ = package ? package->GetPackageType() : item_inventory_type_;
	if (grid_info)
	{
		if (!grid_info->IsValid())
		{
			assert(false);
			return false;
		}
		item_idx_info_ = grid_info->idx_info_;
		item_id_ = grid_info->item_id_;
		assert(item_id_ > 0);
	}
	return true;
}

stCD_VecInt GItemBase::GetCurItemPos() const
{
	return stCD_VecInt(item_idx_info_.GetLeftX(), item_idx_info_.GetTopY());
}

bool GItemBase::Use() const
{
	LOG_O(Log_error) << "物品不能被使用：" << item_name_;
	return false;
}

void GItemBase::DebugInfo() const
{
	auto& astrict = GetBasePro();
	LOG_O(Log_debug) << "item-name :" << GetItemName() << ",item_id :" << GetItemId()
		<< ",item_pos" << GetCurItemPos() << ",Color-"
		<< GetItemColor() << ",playe_lvl-" << astrict.level_ << ",is_identified_-"
		<< IsIdentified() << ",item_power_-" << astrict.power_ << ",item_smart_-"
		<< astrict.smart_ << ",item_brain_-" << astrict.brain_ << ",Max_value" << GetItemMaxStack()
		<< ",物品类型:" << GetItemType();
	item_tips_.DebugInfo();
}

bool GItemBase::operator==(const GItemBase& rhs) const
{
	return GetItemType() == rhs.GetItemType() &&
		GetItemName() == rhs.GetItemName() &&
		GetItemMaxStack() == rhs.GetItemMaxStack() &&
		GetItemSize() == rhs.GetItemSize() &&
		IsBind() == rhs.IsBind();
}

bool GItemBase::IsInSize(const stSizeInt& size) const
{
	auto& grid_info = GetItemGridInfo();
	if (size.Width() < grid_info.GetWidth())
		return false;

	if (size.Height() < grid_info.GetHeight())
		return false;

	return true;
}

int GItemBase::WeightImpl() const
{
	return GEquipScore::GetMe().CalcItemGoodValue(*this) + item_tips_.TipsWeight();
	//pt_int the_value = 0;
	//auto color = GetItemColor();
	//if (color < 0 || color >= KIC_Max)
	//{
	//	assert(false);
	//	return 0;
	//}
	//return the_value += kItemColorScore[color];
}

bool GItemBase::CanWear() const
{
	auto& gmp = GPlayerMe::GetMe();
	if (gmp.GetBasePro() >= GetBasePro())
		return true;
	return false;
}

enCD_InventoryType GItemBase::Equip2BodyPos() const
{
	const auto& types = GEquipTypeMgr::GetMe().GetPositions(*this);
	if (types.empty())
	{
		assert(false);
		return kIT_Invalid;
	}
	return types[0];
}

bool GItemBase::SetItemPos(enCD_InventoryType dst_inventory_type, const stCD_VecInt& coor)
{
	auto& gmp = GPlayerMe::GetMe();
	if (!gmp.TakeupItem(GetInventoryType(), GetCurItemPos()))
	{
		assert(false);
		return false;
	}
	//DummySleep(100);
	if (!gmp.PutDownItem(coor, dst_inventory_type))
	{
		assert(false);
		return false;
	}

	return true;
}

enMoveItemToRes GItemBase::MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return MoveToImpl(this, dst, pos);
}

enTakeoffGemRes GItemBase::TakeoffGemTo(int gem_idx, GPackageBase& dst, const stCD_VecInt& pos) const
{
	return kTOGR_HasNotGem;
}

enPutItemDownRes GItemBase::PutDownTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return dst.PutDownToHere(*this, pos);
}

bool GItemBase::Destroy(bool need_takeup) const
{
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.DestroyItem(*this, need_takeup))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GItemBase::IsCanDestroy() const
{
	if (IsMustKeepItem())
		return false;
	return !DoFilter();
}

bool GItemBase::MergeItemTo(const GItemBase& dst_item) const
{
	if (!(*this == dst_item))
	{
		assert(false);
		return false;
	}
	if (dst_item.GetItemMaxStack() <= 0)
	{
		assert(false);
		return false;
	}
	if (GetItemCnt() + dst_item.GetItemCnt() > dst_item.GetItemMaxStack())
	{
		assert(false);
		return false;
	}

	auto& gmp = GPlayerMe::GetMe();
	if (!gmp.TakeupItem(GetInventoryType(), GetCurItemPos()))
	{
		assert(false);
		return false;
	}
	//DummySleep(100);
	if (!gmp.MergeItem(dst_item))
	{
		assert(false);
		return false;
	}
	return true;
}

enInsideGemRes GItemBase::InsideGemTo(int gem_idx, enCD_InventoryType dst_inventory) const
{
	auto dst = GPackageMgrBase().NewPackage(dst_inventory);
	if (!dst)
	{
		assert(false);
		return kIGR_Failed;
	}
	auto the_equip = dst->GetFirstItem();
	if (!the_equip)
	{
		assert(false);
		return kIGR_Failed;
	}
	return InsideGemTo(gem_idx, *the_equip);
}

enInsideGemRes GItemBase::InsideGemTo(int gem_idx, GItemBase& dst_equip) const
{
	return kIGR_CanNotBeInsided;
}

enInsideGemRes GItemBase::DoInsideGem(int gem_idx, const GItemGem& gem)
{
	return kIGR_CanNotBeInsided;
}

bool GItemBase::DoFilter() const
{
	return MyLuaThrdData::GetPickUpFilter().DoFilt(*this);
}

bool GItemBase::IsMustKeepItem() const
{
	return false;
}

bool GItemBase::UseTo(const GItemBase& dst_item) const
{
	LOG_O(Log_error) << "使用物品到目标之 源物品不能使用,物品名:" << item_name_;
	return false;
}

enCD_FlaskType GItemBase::GetFlaskType() const
{
	assert(false);
	return kFT_Invalid;
}

void GItemBase::GetAllPackGem(GItemContT& all_gem) const
{

}

void GItemBase::PushValidFlaskSpeedInfo(GFlaskSpeedCont& cont) const
{

}

GPackageBasePtr GItemBase::GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const
{
	return nullptr;
}

bool GItemBase::IsEquip() const
{
	return false;
}

void GItemBase::PushGoodToTheResult(GItemStaticContT& out_items)
{
	auto type = GetItemType();
	auto it_out = std::find_if(out_items.begin(), out_items.end(), [type](const GItemBasePtr& the_item){
		return type == the_item->GetItemType();
	});
	if (it_out == out_items.end())
	{
		out_items.push_back(shared_from_this());
		return;
	}
	if ((*it_out)->Weight() < Weight())
		*it_out = shared_from_this();
}

GPackageBasePtr GItemBase::GetPackage() const
{
	if (package_.expired()) return nullptr;
	return package_.lock();
}

enCD_InventoryType GItemBase::GetInventoryType() const
{
	auto package = GetPackage();
	if (!package) 
		return item_inventory_type_;
	return package->GetPackageType();
}

int GItemBase::CheckSum(int bag_width) const
{
	int check_sum = 0;
	int cursor = 0;
	while (true)
	{
		int idx = item_idx_info_.TraItemGridIdx(cursor, bag_width);
		if (idx < 0)
			break;
		check_sum += idx * 5000;
	}
	check_sum += item_cnt_;
	return check_sum;
}

void GItemBase::GetInvalidGem(GemEquipConT& all_gem) const
{

}

int GItemBase::Weight() const
{
	if (!weight_)
		weight_ = WeightImpl();
	return *weight_;
}

GItemBasePtr GItemMgrBase::CreateItem(stCD_EntityItemBased& item_entity, pt_dword item_id, const GPackageBasePtr& package, stCD_ItemGridsInfo* grid_info) const
{
	auto item_cate = CalcItemCate(item_entity);
	GItemBasePtr item;
	switch (item_cate)
	{
	case kICATE_Usable:
	{
		item.reset(new GItemUsable);
		break;
	}
	case kICATE_Flask:
	{
		item.reset(new GItemFlask);
		break;
	}
	case kICATE_Decoration:
	{
		item.reset(new GItemDecoration);
		break;
	}
	case kICATE_Ring:
	{
		item.reset(new GItemRing);
		break;
	}
	case kICATE_Armor:
	{
		item.reset(new GItemArmor);
		break;
	}
	case kICATE_Weapon:
	{
		item.reset(new GItemWeapon);
		break;
	}
	case kICATE_Gem:
	{
		item.reset(new GItemGem);
		break;
	}
	case kICATE_Quest:
	{
		item.reset(new GItemQuest);
		break;
	}
	case kICATE_Other:
	{
		item.reset(new GItemBase);
		break;
	}
	}
	if (!item)
	{
		assert(false);
		return nullptr;
	}
	if (!item->Init(item_entity, item_id, package, grid_info))
	{
		assert(false);
		return nullptr;
	}
	return item;
}

enItemCate GItemMgrBase::CalcItemCate(stCD_EntityItemBased& item_entity) const
{
	struct stHelp{
		stHelp(){
			for (auto& v : item_cates_)
				v = kICATE_Other;
			item_cates_[kITYPE_QuestItem] = kICATE_Quest;
			item_cates_[kITYPE_ActiveSkillGem] = kICATE_Gem;
			item_cates_[kITYPE_SupportSkillGem] = kICATE_Gem;
			item_cates_[kITYPE_LifeFlask] = kICATE_Flask;
			item_cates_[kITYPE_ManaFlask] = kICATE_Flask;
			item_cates_[kITYPE_HybridFlask] = kICATE_Flask;
			item_cates_[kITYPE_UtilityFlask] = kICATE_Flask;
			item_cates_[kITYPE_UtilityFlaskCritical] = kICATE_Flask;
			//////////////////////////////////////////////////////////////////////////
			item_cates_[kITYPE_Helmet] = kICATE_Armor;
			item_cates_[kITYPE_Boots] = kICATE_Armor;
			item_cates_[kITYPE_Gloves] = kICATE_Armor;
			item_cates_[kITYPE_BodyArmour] = kICATE_Armor;
			item_cates_[kITYPE_Amulet] = kICATE_Decoration;
			item_cates_[kITYPE_Belt] = kICATE_Decoration;
			item_cates_[kITYPE_Ring] = kICATE_Ring;
			//////////////////////////////////////////////////////////////////////////
			//单手
			item_cates_[kITYPE_Dagger] = kICATE_Weapon;
			item_cates_[kITYPE_Sceptre] = kICATE_Weapon;
			item_cates_[kITYPE_OneHandSword] = kICATE_Weapon;
			item_cates_[kITYPE_OneHandMace] = kICATE_Weapon;
			item_cates_[kITYPE_OneHandAxe] = kICATE_Weapon;
			item_cates_[kITYPE_Claw] = kICATE_Weapon;
			item_cates_[kITYPE_ThrustingOneHandSword] = kICATE_Weapon;
			//////////////////////////////////////////////////////////////////////////
			//双手
			item_cates_[kITYPE_TwoHandSword] = kICATE_Weapon;
			item_cates_[kITYPE_TwoHandAxe] = kICATE_Weapon;
			item_cates_[kITYPE_TwoHandMace] = kICATE_Weapon;
			item_cates_[kITYPE_Staff] = kICATE_Weapon;
			//////////////////////////////////////////////////////////////////////////
			//限定位置的武器
			item_cates_[kITYPE_Bow] = kICATE_Weapon;
			item_cates_[kITYPE_Quiver] = kICATE_Weapon;
			item_cates_[kITYPE_Shield] = kICATE_Weapon;
			item_cates_[kITYPE_Wand] = kICATE_Weapon;
			//////////////////////////////////////////////////////////////////////////
		}

		enItemCate		item_cates_[kITYPE_Max];
	};
	auto item_type = item_entity.GetItemType();
	if (item_type >= kITYPE_Max)
	{
		assert(false);
		return kICATE_Invalid;
	}
	static stHelp helper;
	auto cate = helper.item_cates_[item_type];
	if (cate != kICATE_Other)
		return cate;
	if (item_entity.GetAttrUsable())
		return kICATE_Usable;
	return cate;
}

GItemEquipBase::GItemEquipBase(enItemCate item_cate) : GItemWearable(item_cate)
{

}

int GItemEquipBase::FindFreeSlot(const enCD_GemSlotColor _color) const
{
	int _index(-1);
	if (gem_solt_.empty())
		return _index;

	for (auto& i : gem_solt_)
	{
		if (i.slot_color_ != _color)
			continue;
		if (i.gem_Item_)
			continue;
		_index = i.index_;
		break;
	}
	return _index;
}

int GItemEquipBase::FindColorSlot(const enCD_GemSlotColor _color) const
{
	int _index(-1);
	if (gem_solt_.empty())
		return _index;


	_index = FindFreeSlot(_color);
	if (_index >= 0)
		return _index;
	for (auto& i : gem_solt_)
	{
		if (i.slot_color_ != _color)
			continue;
		_index = i.index_;
		break;
	}
	return _index;
}

int GItemEquipBase::GetGemIndexByName(const std::string& gem_name)
{
	int index = -1;
	if (gem_solt_.empty())
		return index;

	for (auto& i : gem_solt_)
	{
		if (!i.gem_Item_)
			continue;
		if (i.gem_Item_->GetItemName() != gem_name)
			continue;

		index = i.index_;
	}
	return index;
}

bool GItemEquipBase::IsHasEqualColorSlot()
{
	if (gem_solt_.empty())
		return false;

	for (auto& i : gem_solt_)
	{
		enCD_GemSlotColor curColor = i.slot_color_;
		int cout = std::count_if(gem_solt_.begin(), gem_solt_.end(), [curColor](stItemSlot& slot){ return slot.slot_color_ == curColor; });
		if (cout > 2)
			return true;
	}
	return false;
}

int GItemEquipBase::GetSlotTheColorCnt(enCD_GemSlotColor color) const
{
	int _index(0);
	for (auto& i : gem_solt_)
	{
		if (i.slot_color_ != color)
			continue;
		++_index;
	}
	return _index;
}

int GItemEquipBase::GetGemCnt() const
{
	if (gem_solt_.empty())
		return 0;

	int index = 0;
	for (auto& i : gem_solt_)
	{
		if (i.gem_Item_)
			++index;
	}
	return index;
}

enCD_GemSlotColor GItemEquipBase::GetColorByIndex(int index)
{
	auto ret_color = kGSC_Invalid;
	if (gem_solt_.empty())
		return ret_color;

	for (auto& i : gem_solt_)
	{
		if (i.index_ != index)
			continue;
		return i.slot_color_;
	}
	return ret_color;
}

bool GItemEquipBase::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		assert(false);
		return false;
	}
	auto attr_sockets = item_entity_->GetAttrSockets();
	if (!attr_sockets)
		return true;
	if (attr_sockets->gem_slot_.slot_connect_info_.IsValid())
	{
		for (auto i : attr_sockets->gem_slot_.slot_connect_info_)
			gem_link_.push_back(i);
	}

	gem_solt_.clear();
	int slot_cnt = attr_sockets->gem_slot_.GetSlotCnt();
	for (int i = 0; i < slot_cnt; ++i)
	{
		if (attr_sockets->gem_slot_.slot_[i] == kGSC_Invalid)
			continue;
		stItemSlot slot_tmp;
		slot_tmp.index_ = i;
		slot_tmp.slot_color_ = attr_sockets->gem_slot_.slot_[i];
		gem_solt_.push_back(slot_tmp);
	}
	
	int cursor = 0;
	GItemMgrBase item_mgr;
	while (true)
	{
		auto gem = attr_sockets->gem_slot_.TraInsideGem(cursor);
		if (IsUnValidPtr(gem))
			break;

		assert(cursor - 1 >= 0);
		assert(cursor > 0 && cursor <= CD_kGemSlotMaxCnt);
		auto slot_cl = cursor - 1;
		Cont_Slot::iterator iter = std::find_if(gem_solt_.begin(), gem_solt_.end(), [slot_cl](stItemSlot& slot){return slot.index_ == slot_cl; });
		if (iter == gem_solt_.end())
		{
			assert(false);
			return false;
		}

		assert(iter->slot_color_ == attr_sockets->gem_slot_.slot_[slot_cl]);
		iter->gem_Item_ = item_mgr.CreateItem(*gem, 0, nullptr, nullptr);
		if (!iter->gem_Item_)
		{
			assert(false);
			return false;
		}
		if (!iter->gem_Item_->Update())
		{
			assert(false);
			return false;
		}
	}
	return true;
}

pt_dword GItemEquipBase::GetMaxGemLinkValue() const
{
	if (gem_link_.empty())
		return 0;

	int ret_value = 0;
	for (auto i : gem_link_)
		ret_value = ret_value > i ? ret_value : i;

	return ret_value;
}

const GClass::GemLinkConT* GItemEquipBase::GetGemLink() const
{
	return &gem_link_;
}

void GItemEquipBase::DebugInfo() const
{
	__super::DebugInfo();
	for (auto& i : gem_solt_)
	{
		if (i.gem_Item_)
		{
			LOG_O(Log_debug) << "宝石名：" << i.gem_Item_->GetItemName() << "宝石标号:"
				<< i.index_ << ",颜色：" << i.slot_color_;
		}
		else
		{
			LOG_O(Log_debug) << "宝石标号:" << i.index_ << ",颜色：" << i.slot_color_;
		}
	}
}

enTakeoffGemRes GItemEquipBase::TakeoffGemTo(int gem_idx, GPackageBase& dst, const stCD_VecInt& pos) const
{
	Cont_Slot takeoff;
	if (gem_idx < 0)
	{
		for (auto& v : gem_solt_)
		{
			if (!v.gem_Item_)
				continue;
			takeoff.push_back(v);
		}
	}
	else
	{
		if (gem_idx >= CD_kGemSlotMaxCnt)
		{
			assert(false);
			return kTOGR_Failed;
		}
		for (auto& v : gem_solt_)
		{
			if (v.index_ != gem_idx)
				continue;
			if (!v.gem_Item_)
				break;
			takeoff.push_back(v);
		}
	}
	if (takeoff.empty())
		return kTOGR_HasNotGem;
	auto pos_tmp = pos;
	for (auto& v : takeoff)
	{
		if (!dst.Update())
		{
			assert(false);
			return kTOGR_Failed;
		}
		if (pos_tmp.AnyLess(0))
		{
			pos_tmp = dst.FindFreePos(*v.gem_Item_);
			if (pos_tmp.AnyLess(0))
				return kTOGR_Fulled;
		}
		else
		{
			if (!dst.CanPutDownItem(pos_tmp, *v.gem_Item_))
				return kTOGR_Failed;
		}
		
		auto& gpm = GPlayerMe::GetMe();
		if (!gpm.TakeoffGem(GetInventoryType(),GetCurItemPos(), v.index_))
		{
			assert(false);
			return kTOGR_Failed;
		}

		if (v.gem_Item_->PutDownTo(dst, pos_tmp) < kPIDR_Succeed)
		{
			assert(false);
			return kTOGR_Failed;
		}
		pos_tmp.x = -1;
	}
	return kTOGR_Succeed;
}

enInsideGemRes GItemEquipBase::DoInsideGem(int gem_idx, const GItemGem& gem)
{
	auto gem_color = gem.GetItemGemColor();
	if (gem_color == kGSC_Invalid)
	{
		LOG_O(Log_error) << "镶嵌宝石之该物品没有颜色，宝石名：" << gem.GetItemName();
		return kIGR_Failed;
	}
	if (gem_solt_.empty())
	{
		LOG_O(Log_error) << "镶嵌宝石之该装备不可镶嵌宝石";
		return kIGR_Failed;
	}
	if (gem_idx < 0)
	{
		gem_idx = FindColorSlot(gem_color);
		if (gem_idx < 0)
		{
			LOG_O(Log_error) << "镶嵌宝石之宝石颜色不匹配，宝石名：" << gem.GetItemName();
			return kIGR_DiffColor;
		}
	}
	else
	{
		if (gem_color != GetColorByIndex(gem_idx))
		{
			LOG_O(Log_error) << "镶嵌宝石之宝石颜色不匹配，宝石名：" << gem.GetItemName();
			return kIGR_DiffColor;
		}
	}
	auto slot = FindSlotByIdx(gem_idx);
	if (!slot)
	{
		assert(false);
		return kIGR_Failed;
	}
	if (slot->gem_Item_)
	{
		auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
		if (!main_bag)
		{
			assert(false);
			return kIGR_Failed;
		}
		if (TakeoffGemTo(gem_idx, *main_bag, kInvalidVecInt) < kTOGR_Succeed)
		{
			assert(false);
			return kIGR_Failed;
		}
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.TakeupItem(gem.GetInventoryType(), gem.GetCurItemPos()))
	{
		assert(false);
		return kIGR_Failed;
	}
	if (!gpm.InsideGem(GetInventoryType(), GetItemId(), gem_idx))
	{
		assert(false);
		return kIGR_Failed;
	}
	return kIGR_Succeed;
}

const stItemSlot* GItemEquipBase::FindSlotByIdx(int idx) const
{
	for (auto& i : gem_solt_)
	{
		if (i.index_ != idx)
			continue;
		return &i;
	}
	return nullptr;
}

void GItemEquipBase::GetAllPackGem(GItemContT& all_gem) const
{
	for (auto& v : gem_solt_)
	{
		if (!v.gem_Item_)
			continue;
		//if (!v.gem_Item_->CanWear())
		//	continue;
		all_gem.push_back(v.gem_Item_);
	}
}

void GItemEquipBase::GetInvalidGem(GemEquipConT& all_gem) const
{
	for (auto& v : gem_solt_)
	{
		if (!v.gem_Item_)
			continue;
		if (v.gem_Item_->CanWear())
			continue;
		all_gem[GetInventoryType()].push_back(v.index_);
	}
}

bool GItemEquipBase::HasThreeColorLink() const
{
	auto max_link = GetMaxGemLinkValue();
	if (max_link < 3)
		return false;

	//相连的槽子是按顺序排列的
	if (gem_solt_.size() < max_link)
	{
		assert(false);
		return false;
	}
	int color_cnt[CD_kGemSlotMaxCnt] = { 0 };
	for (size_t i = 0; i < max_link; ++i)
	{
		auto color = gem_solt_[i].slot_color_;
		if (color >= sizeof(color_cnt))
		{
			assert(false);
			return false;
		}
		++color_cnt[color];
	}
	int cnt = 0;
	for (auto v : color_cnt)
	{
		if (v > 0)
			++cnt;
	}
	return cnt >= 3;
}

int GItemEquipBase::WeightImpl() const
{
	auto the_value = __super::WeightImpl();
	the_value += GEquipScore::GetMe().CalcItemGoodValue(*this);
	return the_value;
}

enMoveItemToRes GItemEquipBase::MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return MoveToImpl(this, dst, pos);
}

bool GItemEquipBase::IsEquip() const
{
	return true;
}

GPackageBase::GPackageBase(enCD_InventoryType inventory_type)
{
	package_type_ = inventory_type;
	game_grid_panel_ = nullptr;
	game_inventory_ = nullptr;
	last_pos_id_ = 0;
}

bool GPackageBase::Init(stCD_UiGridPanel* ui_grid_panel, stCD_Inventory* inventory)
{
	game_grid_panel_ = ui_grid_panel;
	if (inventory)
		game_inventory_ = inventory;
	else
	{
		if (ui_grid_panel)
			game_inventory_ = ui_grid_panel->GetInventory();
	}
	if (!game_inventory_)
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPackageBase::UpdateImpl()
{
	if (!game_inventory_)
	{
		assert(false);
		return false;
	}
	package_size_.SetSize(game_inventory_->grid_width_, game_inventory_->grid_height_);
	if (!package_size_.AllGreaterZero())
	{
		assert(false);
		return false;
	}
	if (package_size_.Width() > CD_kBagItemArraySideSize)
	{
		assert(false);
		return false;
	}
	if (package_size_.Height() > CD_kBagItemArraySideSize)
	{
		assert(false);
		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	items_.clear();
	auto this_ptr = shared_from_this();
	assert(this_ptr);
	game_inventory_->TraItems([this, this_ptr](stCD_ItemGridsInfo* item_grid_info, stCD_EntityItemBased* entity){
		GItemBase stash_item;
		if (!item_grid_info)
		{
			assert(false);
			return false;
		}
		if (!entity)
		{
			assert(false);
			return false;
		}
		auto item = CreateItem(*entity, 0, this_ptr, item_grid_info);
		if (!item)
		{
			assert(false);
			return false;
		}
		if (!item->Update())
		{
			assert(false);
			return false;
		}
		items_.push_back(item);
		return true;
	});
	//////////////////////////////////////////////////////////////////////////
	grids_state_.reset();
	if (!items_.empty())
	{
		assert(items_.size() < CD_kBagItemArraySideSize * CD_kBagItemArraySideSize);
		for (auto& i : items_)
		{
			assert(i);
			if (!UpdateItemCellState(*i))
			{
				assert(false);
				return false;
			}
		}
	}
	return true;
}

void GPackageBase::SetGridState(int x, int y, bool state)
{
	if (!package_size_.InSize(x, y))
	{
		assert(false);
		return;
	}
	auto idx = y * CD_kBagItemArraySideSize + x;
	assert(idx >= 0 && idx < CD_kBagItemArraySideSize * CD_kBagItemArraySideSize);
	if (state)
		grids_state_.set(idx);
	else
		grids_state_.reset(idx);
}

bool GPackageBase::GetGridState(int x, int y) const
{
	if (!package_size_.InSize(x, y))
	{
		//assert(false);
		return false;
	}
	auto idx = y * CD_kBagItemArraySideSize + x;
	assert(idx >= 0 && idx < CD_kBagItemArraySideSize * CD_kBagItemArraySideSize);
	return grids_state_.test(idx);
}

bool GPackageBase::GetGridState(const stCD_VecInt& pos) const
{
	return GetGridState(pos.x, pos.y);
}

bool GPackageBase::UpdateItemCellState(const GItemBase& _item)
{
	const auto& item_grids_info = _item.GetItemGridInfo();
	int left_x = item_grids_info.GetLeftX();
	int top_y = item_grids_info.GetTopY();
	int right_x = item_grids_info.GetRightX();
	int bottom_y = item_grids_info.GetDownY();

	if (!package_size_.InSize(left_x, top_y))
	{
		assert(false);
		return false;
	}
	if (!package_size_.InSize(right_x, bottom_y))
	{
		assert(false);
		return false;
	}

	for (int j = left_x; j < left_x + item_grids_info.GetWidth(); ++j)
	{
		for (int k = top_y; k < top_y + item_grids_info.GetHeight(); ++k)
		{
			SetGridState(j, k, true);
		}
	}
	return true;
}

enMoveItemToRes GPackageBase::MoveItToHere(const GItemBase& item, const stCD_VecInt& pos)
{
	return kMITR_CanNotBePutted;
}

enMoveItemToRes GPackageBase::MoveItToHere(const GItemWearable& item, const stCD_VecInt& pos)
{
	return MoveItToHere((const GItemBase&)item, pos);
}

enMoveItemToRes GPackageBase::MoveItToHere(const GItemFlask& item, const stCD_VecInt& pos)
{
	return MoveItToHere((const GItemWearable&)item, pos);
}

enMoveItemToRes GPackageBase::MoveItToHere(const GItemRing& item, const stCD_VecInt& pos)
{
	return MoveItToHere((const GItemWearable&)item, pos);
}

enMoveItemToRes GPackageBase::MoveItToHere(const GItemEquipBase& item, const stCD_VecInt& pos)
{
	return MoveItToHere((const GItemWearable&)item, pos);
}

enMoveItemToRes GPackageBase::MoveItToHere(const GItemWeapon& item, const stCD_VecInt& pos)
{
	return MoveItToHere((const GItemEquipBase&)item, pos);
}

bool GPackageBase::AdjustPack()
{
	return false;
}

GItemBasePtr GPackageBase::FindItemByPos(const stCD_VecInt& _pos /*= stCD_VecInt()*/) const
{
	if (items_.empty())
		return nullptr;

	if (!package_size_.InSize(_pos.x, _pos.y))
		return nullptr;
	return GetPosItem(_pos);
}

GItemBasePtr GPackageBase::FindItemByName(const std::string& item_name) const
{
	if (items_.empty())
		return nullptr;

	for (auto& i : items_)
	{
		if (i->GetItemName() != item_name)
			continue;
		return i;
	}
	return nullptr;
}

GItemBasePtr GPackageBase::FindItemById(pt_dword item_id) const
{
	if (items_.empty())
	{
		assert(false);
		return nullptr;
	}
	for (auto& i : items_)
	{
		if (i->GetItemId() != item_id)
			continue;

		return i;
	}
	return nullptr;
}

GItemBasePtr GPackageBase::GetPosItem(const stCD_VecInt& _pos) const
{
	if (_pos.x < 0 || _pos.y < 0)
	{
		assert(false);
		return nullptr;
	}
	if (!package_size_.InSize(_pos.x, _pos.y))
	{
		assert(false);
		return nullptr;
	}
	if (items_.empty())
		return nullptr;

	for (auto& i : items_)
	{
		auto item_info = i->GetItemGridInfo();
		if (item_info.GetLeftX() != _pos.x)
			continue;
		if (item_info.GetTopY() != _pos.y)
			continue;
		return i;
	}
	return nullptr;
}

pt_dword GPackageBase::GetItemCnt() const
{
	return items_.size();
}

stCD_VecInt GPackageBase::FindFreePosBySize(const stSizeInt& size, const IFilterPos* filter_pos) const
{
	stCD_VecInt pos(-1, -1);
	if (!size.AllGreaterZero())
		return pos;

	bool isFind = false;
	if (package_size_ >= size)
	{
		while (NextNullPos(pos))
		{
			if (filter_pos && filter_pos->DoFilt(pos))
				continue;
			auto itemBig = FindMaxSpaceByPos(pos.x, pos.y);
			if (!itemBig.AllGreaterZero())
				continue;
			if (pos.AnyLess(0))
				break;
			if (itemBig >= size)
			{
				isFind = true;
				break;
			}
		}
	}
	return isFind ? pos : stCD_VecInt(-1, -1);
}

bool GPackageBase::NextNullPos(stCD_VecInt& beginPos) const
{
	int _y = beginPos.y;
	_y += 1;
	_y = std::max(_y, 0);
	beginPos.x = std::max(beginPos.x, 0);
	bool is_find(false);
	stCD_VecInt nextPos;
	for (auto i = beginPos.x; i < package_size_.Width(); ++i)
	{
		if (is_find)break;
		for (auto j = _y; j < package_size_.Height(); ++j)
		{
			if (GetGridState(i, j))
				continue;

			beginPos.x = i;
			beginPos.y = j;
			is_find = true;
			break;
		}
		_y = 0;
	}

	if (!is_find)
		return false;
	return true;
}

stSizeInt GPackageBase::FindMaxSpaceByPos(int _x, int _y) const
{
	stSizeInt itembig;
	if (!package_size_.InSize(_x, _y))
		return itembig;

	if (GetGridState(_x, _y))
		return itembig;

	auto temhight = 0;
	itembig.SetHeight(std::numeric_limits<int>::max());
	int value = 2;
	if (package_type_ == kIT_Flasks)
		value = 1;

	for (auto v = _x; v < std::min(_x + value, package_size_.Width()); ++v)
	{
		if (GetGridState(v, _y))
			break;
		temhight = 0;
		itembig.PlusWidth(1);
		for (auto i = _y; i < std::min(_y + 4, package_size_.Height()); i++)
		{
			if (GetGridState(v, i))
				break;

			temhight += 1;
		}
		itembig.SetHeight(std::min(itembig.Height(), temhight));
	}
	return itembig;
}

stSizeInt GPackageBase::FindMaxSpaceByPos(const stCD_VecInt& pos) const
{
	return FindMaxSpaceByPos(pos.x, pos.y);
}

GItemBasePtr GPackageBase::FindCanOverlayItem(const GItemBase& item) const
{
	if (item.GetItemMaxStack() > 0)
	{
		if (item.GetItemCnt() >= item.GetItemMaxStack())
			return nullptr;

		auto cur_cont = item.GetItemCnt();
		for (auto& i : items_)
		{
			if (i->GetItemMaxStack() <= 0)
				continue;

			if (!(*i == item))
				continue;

			if (i->GetItemCnt() + cur_cont > i->GetItemMaxStack())
				continue;
			return i;
		}
	}
	return nullptr;
}

GItemBasePtr GPackageBase::FindItemByIndex(int index) const
{
	if (index < 0 || (pt_dword)index >= items_.size())
	{
		assert(false);
		return nullptr;
	}
	return items_[index];
}

int GPackageBase::FindFreeGemIndex(const enCD_GemSlotColor gem_color) const
{
	int index(-1);
	if (items_.empty())
		return index;

	for (auto& i : items_)
	{
		int slotIndex = i->FindFreeSlot(gem_color);
		if (slotIndex < 0)
			continue;

		index = slotIndex;
		break;
	}
	return index;
}

void GPackageBase::GetAllPackGem(GItemContT& all_gem) const
{
	if (items_.empty())
		return;
	for (auto& i : items_)
		i->GetAllPackGem(all_gem);
}

bool GPackageBase::CanPutDownItem(const GItemBase& item) const
{
	return !FindFreePosBySize(item.GetItemSize()).AnyLess(0);
}

bool GPackageBase::CanPutDownItem(const stCD_VecInt& pos, const GItemBase& item) const
{
	if (!package_size_.InSize(pos.x, pos.y))
	{
		assert(false);
		return false;
	}
	auto end_pos = pos + item.GetItemSize();
	for (auto y = pos.y; y < end_pos.y; ++y)
	{
		for (auto x = pos.x; x < end_pos.x; ++x)
		{
			if (GetGridState(x, y))
				return false;
		}
	}
	return true;
}

stCD_VecInt GPackageBase::FindFreePos(const GItemBase& item, const IFilterPos* filter_pos) const
{
	return FindFreePosBySize(item.GetItemSize(), filter_pos);
}

enPutItemDownRes GPackageBase::PutDownToHere(const GItemBase& item, const stCD_VecInt& pos)
{
	return kPIDR_CanNotBePutted;
}

enPutItemDownRes GPackageBase::PutDownToHere(const GItemWearable& item, const stCD_VecInt& pos)
{
	return PutDownToHere((const GItemBase&)item, pos);
}

enPutItemDownRes GPackageBase::PutDownToHere(const GItemWeapon& item, const stCD_VecInt& pos)
{
	return PutDownToHere((const GItemWearable&)item, pos);
}

enPutItemDownRes GPackageBase::PutDownToHere(const GItemFlask& item, const stCD_VecInt& pos)
{
	return PutDownToHere((const GItemWearable&)item, pos);
}

enPutItemDownRes GPackageBase::PutDownToHere(const GItemRing& item, const stCD_VecInt& pos)
{
	return PutDownToHere((const GItemWearable&)item, pos);
}

enPutItemDownRes GPackageBase::PutDownToHereImpl(const GItemBase& item, const stCD_VecInt& pos)
{
	auto the_pos = pos;
	if (the_pos.AnyLess(0))
	{
		the_pos = FindFreePosBySize(item.GetItemSize());
		if (the_pos.AnyLess(0))
		{
			LOG_O(Log_error) << "没有空间了，不能放下物品：" << item.GetItemName();
			return kPIDR_Failed;
		}
	}
	if (!GPlayerMe::GetMe().PutDownItem(pos, GetPackageType()))
	{
		assert(false);
		return kPIDR_Failed;
	}
	return kPIDR_Succeed;
}

enMoveItemToRes GPackageBase::MoveItToHereImpl(const GItemBase& item, const stCD_VecInt& pos)
{
	if (pos.AnyLess(0))
	{
		auto dst_item = FindCanOverlayItem(item);
		if (dst_item)
		{
			if (!item.MergeItemTo(*dst_item))
			{
				assert(false);
				return kMITR_Failed;
			}
			return kMITR_Succeed;
		}
		auto pos_tmp = FindFreePosBySize(item.GetItemSize());
		if (pos_tmp.AnyLess(0))
		{
			auto src_package = GPackageMgrBase().NewPackage(item.GetInventoryType());
			if (!src_package)
				return kMITR_CanNotBePutted;
			return NotifyMoveItToHereFailed(*src_package, item, pos);
		}
		return MoveItToHereDirect(item, pos_tmp);
	}	
	if (FindMaxSpaceByPos(pos) >= item.GetItemSize())
		return MoveItToHereDirect(item, pos);

	auto src_package = GPackageMgrBase().NewPackage(item.GetInventoryType());
	if (!src_package)
		return kMITR_CanNotBePutted;
	return NotifyMoveItToHereFailed(*src_package, item, pos);
}

enMoveItemToRes GPackageBase::MoveItToHereDirect(const GItemBase& item, const stCD_VecInt& pos)
{
	if (pos.AnyLess(0))
	{
		assert(false);
		return kMITR_Fulled;
	}
	if (!GPlayerMe::GetMe().TakeupItem(item.GetInventoryType(), item.GetCurItemPos()))
	{
		assert(false);
		return kMITR_Failed;
	}
	//DummySleep(100);
	if (item.PutDownTo(*this, pos) < kPIDR_Succeed)
	{
		assert(false);
		return kMITR_Failed;
	}
	return kMITR_Succeed;
}

enMoveItemToRes GPackageBase::NotifyMoveItToHereFailed(GPackageBase& src_package, const GItemBase& item, const stCD_VecInt& dst_pos)
{
	return src_package.OnMoveItToThereFailed(*this, item, dst_pos);
}

enMoveItemToRes GPackageBase::OnMoveItToThereFailed(GPackageBase& dst_package, const GItemBase& item, const stCD_VecInt& dst_pos)
{
	return kMITR_CanNotBePutted;
}

enMoveItemToRes GPackageBase::OnMoveItToThereFailed(GPackageWearable& dst_package, const GItemBase& item, const stCD_VecInt& dst_pos)
{
	return OnMoveItToThereFailed((GPackageBase&)dst_package, item, dst_pos);
}

const GFlaskSpeedCont* GPackageBase::GetFlaskSpeedCont() const
{
	assert(false);
	return nullptr;
}

const IFilterPos* GPackageBase::GetRetainFilterPos(const GItemBase* item) const
{
	return nullptr;
}

GItemBasePtr GPackageBase::GetFirstItem() const
{
	if (items_.empty())
		return nullptr;
	return items_.front();
}

void GPackageBase::WeightSort(bool by_good)
{
	if (by_good)
	{
		std::sort(items_.begin(), items_.end(), [](const GItemBasePtr& lhs, const GItemBasePtr& rhs){
			return lhs->Weight() > rhs->Weight();
		});
	}
	else
	{
		std::sort(items_.begin(), items_.end(), [](const GItemBasePtr& lhs, const GItemBasePtr& rhs){
			return lhs->Weight() < rhs->Weight();
		});
	}
}

bool GPackageBase::GoodThanAll(const GItemBase& item) const
{
	if (items_.empty())
		return true;
	auto the_value = item.Weight();
	for (auto& v : items_)
	{
		if (v->Weight() < the_value)
			return true;
	}
	return false;
}

stCD_VecInt GPackageBase::GetOneCanPutPos(const GItemFlask& item) const
{
	assert(false);
	return stCD_VecInt(-1, -1);
}

void GPackageBase::GetGoodItems(const GIFilterItem& filter, GItemStaticContT& out_items) const
{
	out_items.clear();
	for (auto& item : items_)
	{
		if (!filter.DoFilt(*item))
			continue;
		item->PushGoodToTheResult(out_items);
	}
}

GItemBasePtr GPackageBase::FindByWeightInfo(const GItemBasePtr& src_item) const
{
	if (!src_item)
	{
		assert(false);
		return nullptr;
	}
	const auto& the_name = src_item->GetItemName();
	auto weight = src_item->Weight();
	for (auto& item : items_)
	{
		if (item->GetItemName() != the_name)
			continue;
		if (item->Weight() != weight)
			continue;
		return item;
	}
	return nullptr;
}

void GPackageBase::DebugInfo() const
{
	for (auto& v : items_)
	{
		v->DebugInfo();
	}
}

enMoveItemToRes GPackageBase::MoveItToThere(const GItemBase& item, GPackageBase& dst, const stCD_VecInt& dst_pos)
{
	return kMITR_MoveToThereFailed;
}

int GPackageBase::CheckSum() const
{
	int check_sum = 0;
	for (auto& item : items_)
	{
		check_sum += item->CheckSum(package_size_.x);
	}
	return check_sum;
}

pt_dword GPackageBase::GetItemCntByType(enCD_ItemType type) const
{
	int cnt = 0;
	if (items_.empty())
		return cnt;
	for (auto& i : items_)
	{
		if (i->GetItemType() != type)
			continue;
		cnt += i->GetItemCnt();
	}
	return cnt;
}

bool GPackageBase::IdentifiedOneItem(const GItemBase& item) const
{
	if (item.IsIdentified())
	{
		assert(false);
		return true;
	}
	auto scroll_item = FindItemByName(kObjName_Identify);
	if (!scroll_item)
	{
		LOG_O(Log_error) << "鉴定物品之没有知识卷轴了";
		return false;
	}
	if (!scroll_item->UseTo(item))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GPackageBase::PressPage() const
{
	assert(false);
	return false;
}

GPackageWearable::GPackageWearable(enCD_InventoryType inventory_type) : GPackageBase(inventory_type)
{
}

enMoveItemToRes GPackageWearable::TakeOffTo(enCD_InventoryType dst_inventory)
{
	GPackageMgrBase mgr;
	auto dst_package = mgr.NewPackage(dst_inventory);
	if (!dst_package)
	{
		assert(false);
		return kMITR_Failed;
	}
	return kMITR_Failed;
}

enMoveItemToRes GPackageWearable::NotifyMoveItToHereFailed(GPackageBase& src_package, const GItemBase& item, const stCD_VecInt& dst_pos)
{
	return src_package.OnMoveItToThereFailed(*this, item, dst_pos);
}

enMoveItemToRes GPackageWearable::MoveItToHere(const GItemWearable& item, const stCD_VecInt& pos)
{
	if (!item.CanWear())
	{
		assert(false);
		return kMITR_CanNotBePutted;
	}
	auto the_pos = pos;
	if (the_pos.AnyLess(0))
		the_pos.assign(0, 0);
	return MoveItToHereImpl(item, the_pos);
}

enPutItemDownRes GPackageWearable::PutDownToHere(const GItemWearable& item, const stCD_VecInt& pos)
{
	return PutDownToHereImpl(item, pos);
}

GPackageBasePtr GPackageMgrBase::CreateNewPackageImpl(stCD_Inventory& inventory, enCD_InventoryType inventory_type) const
{
	GPackageBasePtr package;
	if (kIT_MainBag == inventory_type)
		package.reset(new GPackageMain);
	else if (kIT_WeaponLeft == inventory_type)
		package.reset(new GPackageLeft);
	else if (kIT_WeaponRight == inventory_type)
		package.reset(new GPackageRight);
	else if (inventory_type >= kIT_BodyArmour && inventory_type <= kIT_Belt)
		package.reset(new GPackageEquips(inventory_type));
	else if (kIT_Flasks == inventory_type)
		package.reset(new GPackageFlasks);
	else
		package.reset(new GPackageBase(inventory_type));
	if (!package)
	{
		assert(false);
		return nullptr;
	}
	if (!package->Init(GInterface::FindGridPanelFromDlgProp(inventory_type), &inventory))
	{
		assert(false);
		return nullptr;
	}
	return package;
}

bool GMyPackages::RebuildAllImpl()
{
	assert(!"不要用这种来更新了");
	packages_.clear();
	CD_Interface::TraInventory([this](enCD_InventoryType inventory_type, stCD_Inventory* inventory){
		assert(inventory);
		auto package = CreateNewPackageImpl(*inventory, inventory_type);
		if (!package)
		{
			assert(false);
			return false;
		}
		if (!package->Update())
		{
			assert(false);
			return false;
		}
		packages_[inventory_type] = package;
		return true;
	});
	if (packages_.empty())
	{
		assert(false);
		return false;
	}
	return true;
}

void GMyPackages::DebugInfo() const
{
	for (auto& i : packages_)
		i.second->DebugInfo();
}

GPackageBasePtr GPackageMgrBase::CreateNewPackage_(enCD_InventoryType inventory_type, bool need_update) const
{
	GPackageBasePtr res;
	if (!GWndExecSync([inventory_type, &res, this, need_update](){
		auto mgr = CD_Interface::GetInventoryMgr();
		if (!mgr)
		{
			assert(false);
			return false;
		}
		auto inventory = mgr->GetInventory(inventory_type);
		if (!inventory)
		{
			assert(false);
			return false;
		}
		res = CreateNewPackageImpl(*inventory, inventory_type);
		if (need_update && !res->Update())
		{
			assert(false);
			return false;
		}
		return true;
	}))
	{
		assert(false);
		return nullptr;
	}
	assert(res);
	return res;
}

int GPackageMgrBase::CreateNewSomePackages_(const FilterInventoryType& filter, PackagesContT& out_packages, bool need_update) const
{
	int cnt = 0;
	if (!GWndExecSync([&out_packages, &filter, this, &cnt, need_update](){
		auto mgr = CD_Interface::GetInventoryMgr();
		if (!mgr)
		{
			assert(false);
			return false;
		}
		cnt = mgr->TraInventory(filter, [&out_packages, this, need_update](enCD_InventoryType inventory_type, stCD_Inventory* inventory){
			auto package = CreateNewPackageImpl(*inventory, inventory_type);
			if (!package)
			{
				assert(false);
				return false;
			}
			if (need_update)
			{
				if (!package->Update())
				{
					assert(false);
					return false;
				}
			}
			out_packages.push_back(package);
			return true;
		});
		return true;
	}))
	{
		assert(false);
	}
	return cnt;
}

int GPackageMgrBase::CreateNewSomePackages_(PackagesContT& out_packages, bool need_update, const Fn_TraInventory& fn) const
{
	int cnt = 0;
	if (!GWndExecSync([&out_packages, this, &cnt, need_update, &fn](){
		auto mgr = CD_Interface::GetInventoryMgr();
		if (!mgr)
		{
			assert(false);
			return false;
		}
		cnt = mgr->TraInventory([&out_packages, this, need_update, &fn]
			(enCD_InventoryType inventory_type, stCD_Inventory* inventory){
			if (!fn(inventory_type, inventory))
				return true;
			auto package = CreateNewPackageImpl(*inventory, inventory_type);
			if (!package)
			{
				assert(false);
				return false;
			}
			if (need_update)
			{
				if (!package->Update())
				{
					assert(false);
					return false;
				}
			}
			out_packages.push_back(package);
			return true;
		});
		return true;
	}))
	{
		assert(false);
	}
	return cnt;
}

GPackageBasePtr GPackageMgrBase::NewPackage(enCD_InventoryType inventory_type) const
{
	return CreateNewPackage_(inventory_type, true);
}

int GPackageMgrBase::NewPackage(PackagesContT& out_packages, const Fn_TraInventory& fn) const
{
	return CreateNewSomePackages_(out_packages, true, fn);
}

int GPackageMgrBase::NewPackage(enCD_InventoryType min, enCD_InventoryType max, PackagesContT& out_packages) const
{
	return NewPackage(out_packages, [min, max](enCD_InventoryType inventory_type, stCD_Inventory* inventory){
		return inventory_type >= min && inventory_type <= max;
	});
}

GPackageBasePtr GPackageMgrBase::CreateNewPackage(enCD_InventoryType inventory_type) const
{
	return CreateNewPackage_(inventory_type, false);
}

int GPackageMgrBase::CreateNewPackage(PackagesContT& out_packages, const Fn_TraInventory& fn) const
{
	return CreateNewSomePackages_(out_packages, false, fn);
}

int GPackageMgrBase::CreateNewPackage(enCD_InventoryType min, enCD_InventoryType max, PackagesContT& out_packages) const
{
	return CreateNewPackage(out_packages, [min, max](enCD_InventoryType inventory_type, stCD_Inventory* inventory){
		return inventory_type >= min && inventory_type <= max;
	});
}

GPackageIteratorPtr GPackageMgrBase::CreateIterator(enCD_InventoryType min, enCD_InventoryType max) const
{
	return GPackageIteratorPtr(new GPackageIterInRange(*this, min, max));
}

enMoveItemToRes GItemWearable::MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return MoveToImpl(this, dst, pos);
}

enPutItemDownRes GItemWearable::PutDownTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return dst.PutDownToHere(*this, pos);
}

GPackageBasePtr GItemWearable::GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const
{
	if (positions.empty())
	{
		auto inventory_type = Equip2BodyPos();
		if (inventory_type == kIT_Invalid)
		{
			LOG_O(Log_debug) << "对比物品之该物品没有对应的位置:" << GetItemName();
			assert(false);
			return false;
		}
		auto package = GPackageMgrBase().NewPackage(inventory_type);
		if (!package)
		{
			assert(false);
			return false;
		}
		if (package->GoodThanAll(*this))
			return package;
	}
	else
	{
		for (auto& package : positions)
		{
			if (!package)
			{
				assert(false);
				break;
			}
			if (package->GoodThanAll(*this))
				return package;
		}
	}
	return nullptr;
}


void GItemTips::DebugInfo() const
{
	LOG_O(Log_trace) << "-----------------";
	LOG_O(Log_trace) << "词缀：" << item_words_ << ",物品名:" << item_name_
		<< "，后缀描述：" << desc_suffix_;
	LOG_O(Log_trace) << "-----------------";
	for (auto& v : base_attrs_)
	{
		v.DebugInfo();
	}
	LOG_O(Log_trace) << "-----------------";
	for (auto& v : require_attrs_)
	{
		v.DebugInfo();
	}
	LOG_O(Log_trace) << "-----------------";
	for (auto& v : blue_attrs_)
	{
		LOG_O(Log_trace) << v;
	}
	LOG_O(Log_trace) << "-----------------";
	for (auto& v : add_bule_attrs_)
	{
		LOG_O(Log_trace) << v;
	}
	LOG_O(Log_trace) << "-----------------";
}

bool GItemTips::From(stCD_ItemTipsParams* game_tips)
{
	assert(game_tips);
	item_words_ = game_tips->item_words_.GetStdString();
	item_name_ = game_tips->item_name_.GetStdString();
	for (auto& v : game_tips->base_attrs_)
	{
		base_attrs_.push_back(GItemTipsBaseAttr());
		auto& back = base_attrs_.back();
		back.attr_cn_name_ = v.attr_cn_name_.GetStdString();
		for (auto& value : v.attr_values_)
		{
			back.attr_values_.push_back(value.value_.GetStdString());
		}
	}
	for (auto& v : game_tips->require_attrs_)
	{
		require_attrs_.push_back(GItemTipsBaseAttr());
		auto& back = require_attrs_.back();
		back.attr_cn_name_ = v.attr_cn_name_.GetStdString();
		for (auto& value : v.attr_values_)
		{
			back.attr_values_.push_back(value.value_.GetStdString());
		}
	}
	for (auto& v : game_tips->blue_attrs_)
	{
		blue_attrs_.push_back(v.GetStdString());
	}
	for (auto& v : game_tips->add_bule_attrs_)
	{
		add_bule_attrs_.push_back(v.GetStdString());
	}
	desc_suffix_ = game_tips->desc_suffix_.GetStdString();
	return true;
}

int GItemTips::TipsWeight() const
{
	return 0;
}

void GItemTipsBaseAttr::DebugInfo() const
{
	LOG_O(Log_trace) << "属性中文名：" << attr_cn_name_;
	for (auto& v : attr_values_)
	{
		LOG_O(Log_trace) << "属性值：" << v;
	}
}

void GItemTipsPanel::DebugInfo() const
{
	for (auto& v : rows_)
	{
		LOG_O(Log_trace) << v;
	}
}

bool GItemTipsPanel::From(stCD_ItemTipsParams* game_tips)
{
	assert(game_tips);
	if (!game_tips->item_words_.empty())
		rows_.push_back(game_tips->item_words_.GetStdString());
	for (auto& v : game_tips->base_attrs_)
	{
		rows_.push_back(std::string());
		auto& back = rows_.back();
		back = v.attr_cn_name_.GetStdString();
		for (auto& value : v.attr_values_)
		{
			back += value.value_.GetStdString();
		}
	}
	for (auto& v : game_tips->require_attrs_)
	{
		rows_.push_back(std::string());
		auto& back = rows_.back();
		back = v.attr_cn_name_.GetStdString();
		for (auto& value : v.attr_values_)
		{
			back += value.value_.GetStdString();
		}
	}
	for (auto& v : game_tips->blue_attrs_)
	{
		rows_.push_back(std::string());
		auto& back = rows_.back();
		back = v.GetStdString();
	}
	for (auto& v : game_tips->add_bule_attrs_)
	{
		rows_.push_back(std::string());
		auto& back = rows_.back();
		back = v.GetStdString();
	}
	if (!game_tips->desc_suffix_.empty())
	{
		rows_.push_back(std::string());
		rows_.back() = game_tips->desc_suffix_.GetStdString();
	}
	return true;
}

void GItemTipsPanel::ClearItemAttrParser()
{
	calculator_.clear();
}

CSV_NodePtr GItemTipsPanel::CreateItemAttrParser(const luabind::object& fn)
{
	auto res = CSV_Factory::MakeCalclator(fn);
	if (!res)
		return nullptr;
	calculator_.push_back(res);
	return res;
}

int GItemTipsPanel::TipsWeight() const
{
	if (calculator_.empty())
		return 0;
	int weight = 0;
	PtValue value_tmp;
	for (auto& str : rows_)
	{
		for (auto& v : calculator_)
		{
			value_tmp = v->Parse(str);
			if (!value_tmp)
				continue;
			weight += v->Eval(value_tmp, (int)0);
			break;
		}
	}
	return weight;
}

GItemTipsPanel::CalculatorCont GItemTipsPanel::calculator_;

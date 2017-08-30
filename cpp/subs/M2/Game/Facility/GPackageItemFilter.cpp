#include "stdafx.h"
#include "GPackageItemFilter.h"
#include "GItem.h"

bool FilterItemWhiteByName::DoFilt(const GItemBase& item) const
{
	for (auto& i : vc_item_name_)
	{
		if (item.GetItemName().find(i) != std::string::npos)
			return true;
	}
	return false;
}

bool FilterItemWhiteByName::Parse(const luabind::object& tbl)
{
	using namespace luabind;
	if (type(tbl) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}

	iterator it(tbl), it_end;
	for (; it != it_end;++it)
	{
		object v = *it;
		if (type(v) != LUA_TSTRING)
		{
			LOG_O(Log_debug) << "添加物品过滤白名单之表类型必须使物品名";
			continue;
		}
		auto name = LuaObjCast(v, kEmptyStr);
		if (name.empty())
		{
			assert(false);
			return false;
		}
		vc_item_name_.push_back(name);
	}
	return true;
}

bool FilterItemByQuality::DoFilt(const GItemBase& item) const
{
	return false;
}

FilterItemBySlot::FilterItemBySlot()
{
}

bool FilterItemBySlot::DoFilt(const GItemBase& item) const
{
	auto cnt = item.GetSlotCnt();
	auto it = std::find_if(slot_num_.begin(), slot_num_.end(), [cnt](int value){return value == cnt; });
	if (it != slot_num_.end())
		return true;
	return false;
}

bool FilterItemBySlot::Parse(const luabind::object& tbl)
{
	using namespace luabind;
	if (type(tbl) != LUA_TTABLE)
	{
		LOG_O(Log_debug) << "添加物品过滤之宝石槽数量必须是表类型";
		return false;
	}
	slot_num_.clear();
	iterator it(tbl), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TNUMBER)
		{
			LOG_O(Log_debug) << "添加物品过滤之宝石槽数量必须数值";
			continue;
		}
		auto value = LuaObjCast(v, -1);
		if (value < 0)
		{
			assert(false);
			return false;
		}
		slot_num_.push_back(value);
	}
	return true;
}

bool FilterItemBySlot::Before(const GItemBase& obj) const
{
	if (!obj.IsEquip())
		return false;
	return true;
}

FilterItemByColour::FilterItemByColour()
{
}

bool FilterItemByColour::DoFilt(const GItemBase& item) const
{
	auto color = item.GetItemColor();
	auto it = std::find_if(item_colour_.begin(), item_colour_.end(), [color](enCD_ItemColor tmp){return color == tmp; });
	if (it != item_colour_.end())
		return true;
	return false;
}

bool FilterItemByColour::Parse(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (!lua_obj.is_valid())
	{
		assert(false);
		return false;
	}
	if (type(lua_obj) != LUA_TTABLE)
	{
		LOG_O(Log_debug) << "添加物品过滤物品颜色之参数必须是物品颜色"<<type(lua_obj);
		return false;
	}

	item_colour_.clear();
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TNUMBER)
		{
			LOG_O(Log_debug) << "添加物品过滤之宝石槽数量必须数值";
			continue;
		}
		auto value = LuaObjCast(v, kIC_Non);
		if (value <= kIC_Non)
		{
			assert(false);
			return false;
		}
		item_colour_.push_back(value);
	}
	return true;
}

bool FilterItemByColour::Before(const FittedType& obj) const
{
	if (item_colour_.empty())
		return false;
	return true;
}

FilterItemBlackByName::FilterItemBlackByName()
{

}

bool FilterItemBlackByName::DoFilt(const GItemBase& item) const
{
	for (auto& i : item_names_)
	{
		if (item.GetItemName().find(i) != std::string::npos)
			return true;
	}
	return false;
}

bool FilterItemBlackByName::Parse(const luabind::object& tbl)
{
	using namespace luabind;
	if (type(tbl) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}

	iterator it(tbl), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TSTRING)
		{
			LOG_O(Log_debug) << "添加物品过滤黑名单之表类型必须是物品名";
			continue;
		}
		item_names_.push_back(object_cast<std::string>(v));
	}
	return true;
}

bool FilterItemBlackByName::Before(const GItemBase& obj) const
{
	return false;
}

FilterItemByType::FilterItemByType()
{

}

bool FilterItemByType::DoFilt(const GItemBase& item) const
{
	if (item_type_.empty())
		return false;
	auto it = std::find_if(item_type_.begin(), item_type_.end(), [&item](const enCD_ItemType& type){
		return type == item.GetItemType(); 
	});
	if (it != item_type_.end())
		return true;
	return false;
}

bool FilterItemByType::Parse(const luabind::object& tbl)
{
	using namespace luabind;
	if (type(tbl) != LUA_TTABLE)
	{
		LOG_O(Log_debug) << "添加物品过滤之物品类型参数必须是物品类型";
		return false;
	}

	iterator it(tbl), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TNUMBER)
		{
			LOG_O(Log_debug) << "添加物品过滤之型参数必须是物品类型";
			continue;
		}
		item_type_.push_back(object_cast<enCD_ItemType>(v));
	}
	return true;
}

FilterItemGemLink::FilterItemGemLink()
{
}

bool FilterItemGemLink::DoFilt(const GItemBase& item) const
{
	auto max_link = item.GetMaxGemLinkValue();
	auto it = std::find_if(link_value_.begin(), link_value_.end(), [max_link](int vlaue){return max_link == vlaue; });
	if (it != link_value_.end())
		return true;
	return false;
}

bool FilterItemGemLink::Parse(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (!lua_obj.is_valid())
	{
		assert(false);
		return false;
	}
	if (type(lua_obj) != LUA_TTABLE)
	{
		LOG_O(Log_debug) << "添加物品过滤之宝石槽连接类型必须是表类型";
		return false;
	}
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TNUMBER)
		{
			LOG_O(Log_debug) << "添加物品过滤之宝石槽连接数参数必须是整数";
			continue;
		}
		auto link_cnt = LuaObjCast(v, -1);
		if (link_cnt <= -1)
		{
			assert(false);
			return false;
		}
		link_value_.push_back(link_cnt);
	}
	return true;
}

bool FilterItemGemLink::Before(const FittedType& obj) const
{
	if (!obj.IsEquip())
		return false;
	return true;
}

GPackageIterInRange::GPackageIterInRange(const GPackageMgrBase& package_mgr, enCD_InventoryType min, enCD_InventoryType max)
	: package_mgr_(package_mgr)
{
	min_ = min;
	max_ = max;
	cur_ = min;
	assert(min <= max);
}

void GPackageIterInRange::First()
{
	cur_ = min_;
	cur_item_ = package_mgr_.NewPackage(cur_);
}

void GPackageIterInRange::Next()
{
	cur_ = (enCD_InventoryType)((int)cur_ + 1);
	cur_item_ = package_mgr_.NewPackage(cur_);
}

bool GPackageIterInRange::IsDone() const
{
	return cur_ > max_;
}

GPackageBasePtr GPackageIterInRange::CurItem() const
{
	return cur_item_;
}

bool FilterItemIdentified::DoFilt(const GItemBase& item) const
{
	return item.IsIdentified();
}

bool FilterItemNotBind::DoFilt(const GItemBase& item) const
{
	return !item.IsBind();
}

bool FilterItemGem::DoFilt(const GItemBase& item) const
{
	return item.IsGem();
}

bool FilterItemFlask::DoFilt(const GItemBase& item) const
{
	return item.IsFlask();
}

bool FilterItemEquip::DoFilt(const GItemBase& item) const
{
	return item.IsEquip();
}

bool FilterItemCanWear::DoFilt(const GItemBase& item) const
{
	return item.CanWear();
}

bool FilterItemRing::DoFilt(const GItemBase& item) const
{
	return item.IsRing();
}

bool FilterItemNotIdentified::DoFilt(const GItemBase& item) const
{
	return !item.IsIdentified();
}

bool FilterItemWeapon::DoFilt(const GItemBase& obj) const
{
	return obj.IsWeapon();
}

GEquipTypeMgr::GEquipTypeMgr()
{
	//单手
	item_type2composite_cate_[kITYPE_Dagger] = kWCC_OneHand;
	item_type2composite_cate_[kITYPE_Sceptre] = kWCC_OneHand;
	item_type2composite_cate_[kITYPE_OneHandSword] = kWCC_OneHand;
	item_type2composite_cate_[kITYPE_OneHandMace] = kWCC_OneHand;
	item_type2composite_cate_[kITYPE_OneHandAxe] = kWCC_OneHand;
	item_type2composite_cate_[kITYPE_Claw] = kWCC_OneHand;
	item_type2composite_cate_[kITYPE_ThrustingOneHandSword] = kWCC_OneHand;
	//双手
	item_type2composite_cate_[kITYPE_TwoHandSword] = kWCC_TwoHand;
	item_type2composite_cate_[kITYPE_TwoHandAxe] = kWCC_TwoHand;
	item_type2composite_cate_[kITYPE_TwoHandMace] = kWCC_TwoHand;
	item_type2composite_cate_[kITYPE_Staff] = kWCC_TwoHand;
	//限定位置的
	item_type2composite_cate_[kITYPE_Bow] = kWCC_Restricted;
	item_type2composite_cate_[kITYPE_Quiver] = kWCC_Restricted;
	item_type2composite_cate_[kITYPE_Shield] = kWCC_Restricted;
	item_type2composite_cate_[kITYPE_Wand] = kWCC_Restricted;
	//////////////////////////////////////////////////////////////////////////
	item_type2positions_[kITYPE_Helmet] = { kIT_Helm };
	item_type2positions_[kITYPE_Amulet] = { kIT_Necklace };
	item_type2positions_[kITYPE_Belt] = { kIT_Belt };
	item_type2positions_[kITYPE_Boots] = { kIT_Shoe };
	item_type2positions_[kITYPE_Gloves] = { kIT_Glove };
	item_type2positions_[kITYPE_BodyArmour] = { kIT_BodyArmour };
	item_type2positions_[kITYPE_UtilityFlask] = { kIT_Flasks };
	item_type2positions_[kITYPE_LifeFlask] = { kIT_Flasks };
	item_type2positions_[kITYPE_ManaFlask] = { kIT_Flasks };
	item_type2positions_[kITYPE_HybridFlask] = { kIT_Flasks };
	item_type2positions_[kITYPE_UtilityFlaskCritical] = { kIT_Flasks };
	item_type2positions_[kITYPE_Ring] = { kIT_RingLeft, kIT_RingRight };
	//武器
	item_type2positions_[kITYPE_Bow] = { kIT_WeaponLeft };
	item_type2positions_[kITYPE_Quiver] = { kIT_WeaponRight };
	item_type2positions_[kITYPE_Shield] = { kIT_WeaponRight };
	item_type2positions_[kITYPE_Wand] = { kIT_WeaponLeft, kIT_WeaponRight };
	//////////////////////////////////////////////////////////////////////////
	//戒指比较特殊
	item_type2position_item_types_[kITYPE_Ring] = GEquipPositionItemTypes({
		{ kIT_RingLeft, kWCC_Invalid, { kITYPE_Ring } },
		{ kIT_RingRight, kWCC_Invalid, { kITYPE_Ring } }
	});
	//武器
	item_type2position_item_types_[kITYPE_Bow] = GEquipPositionItemTypes({
		{ kIT_WeaponRight, kWCC_Invalid, { kITYPE_Quiver } } });
	item_type2position_item_types_[kITYPE_Quiver] = GEquipPositionItemTypes({
		{ kIT_WeaponLeft, kWCC_Invalid, { kITYPE_Bow } } });
	item_type2position_item_types_[kITYPE_Shield] = GEquipPositionItemTypes({ 
		{ kIT_WeaponLeft, kWCC_OneHand, {} },
		{ kIT_WeaponLeft, kWCC_Invalid, { kITYPE_Wand } } });
	item_type2position_item_types_[kITYPE_Wand] = GEquipPositionItemTypes({
		{ kIT_WeaponLeft, kWCC_Invalid, { kITYPE_Wand } },
		{ kIT_WeaponRight, kWCC_Invalid, { kITYPE_Wand, kITYPE_Shield } } });
	//////////////////////////////////////////////////////////////////////////
}

GEquipPackageTypeCont GEquipTypeMgr::GetPositions(const GItemBase& item) const
{
	auto it = item_type2positions_.find(item.GetItemType());
	if (it != item_type2positions_.end())
		return it->second;
	auto cate = GetWeaponCompositeCate(item);
	if (cate == kWCC_OneHand)
		return GEquipPackageTypeCont({ kIT_WeaponLeft, kIT_WeaponRight });
	else if (cate == kWCC_TwoHand)
		return GEquipPackageTypeCont({ kIT_WeaponLeft });
	assert(!"是否还未添加对该物品的支持？");
	return GEquipPackageTypeCont();
}

enWeaponCompositeCate GEquipTypeMgr::GetWeaponCompositeCate(const GItemBase& item) const
{
	auto it = item_type2composite_cate_.find(item.GetItemType());
	if (it == item_type2composite_cate_.end())
		return kWCC_Invalid;
	return it->second;
}

GEquipPositionItemTypes GEquipTypeMgr::GetSwitchItemTypes(const GItemBase& item) const
{
	auto it = item_type2position_item_types_.find(item.GetItemType());
	if (it != item_type2position_item_types_.end())
		return it->second;	
	auto cate = GetWeaponCompositeCate(item);
	if (cate == kWCC_TwoHand)
		return GEquipPositionItemTypes();
	else if (cate == kWCC_OneHand)
	{
		static const GEquipPositionItemTypes one_hand_item_types = {
			{ kIT_WeaponLeft, kWCC_OneHand, {} },
			{ kIT_WeaponRight, kWCC_OneHand, {} }
		};
		return one_hand_item_types;
	}
	return GEquipPositionItemTypes();
}

GEquipPositionItemTypes GEquipTypeMgr::GetSwitchItemTypes(const GItemBase& item, enCD_InventoryType but_position) const
{
	auto item_types = GetSwitchItemTypes(item);
	if (item_types.empty())
		return item_types;
	GEquipPositionItemTypes item_types_tmp;
	for (auto& v : item_types)
	{
		if (v.position_ == but_position)
			continue;
		item_types_tmp.push_back(v);
	}
	return item_types_tmp;
}

bool GEquipTypeMgr::CanWearTheEquip(const GEquipPositionItemTypes& item_types, const GItemBase& item, enCD_InventoryType* out_position) const
{
	auto t = item.GetItemType();
	auto cate = GetWeaponCompositeCate(item);
	for (auto& v : item_types)
	{
		for (auto item_type : v.item_types_)
		{
			if (item_type == t)
			{
				if (out_position)
					*out_position = v.position_;
				return true;
			}
		}
		if (v.composite_cate_ != kWCC_Invalid)
		{
			if (cate == v.composite_cate_)
			{
				if (out_position)
					*out_position = v.position_;
				return true;
			}
		}
	}
	return false;
}

bool GEquipTypeMgr::CanWearTheEquips(const GItemBase& lhs, const GItemBase& rhs) const
{
	auto item_types = GetSwitchItemTypes(lhs);
	if (CanWearTheEquip(item_types, rhs, nullptr))
		return true;
	item_types = GetSwitchItemTypes(rhs);
	if (CanWearTheEquip(item_types, lhs, nullptr))
		return true;
	return false;
}

enCD_InventoryType GEquipTypeMgr::SwitchPosition(enCD_InventoryType position)
{
	if (position == kIT_WeaponLeft)
		return kIT_WeaponRight;
	else if (position == kIT_WeaponRight)
		return kIT_WeaponLeft;
	/*else if (position == kIT_RingLeft)
		return kIT_RingRight;
	else if (position == kIT_RingRight)
		return kIT_RingLeft;*/
	return kIT_Invalid;
}

bool FilterItemNotFlask::DoFilt(const GItemBase& item) const
{
	return !item.IsFlask();
}

FilterItemCnt::FilterItemCnt()
{

}

bool FilterItemCnt::DoFilt(const GItemBase& obj) const
{
	auto it = item_retain_cnt_.find(obj.GetItemName());
	if (it == item_retain_cnt_.end())
	{
		assert(false);
		return false;
	}
	////指定物品才会判断
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag || !main_bag->Update())
	{
		assert(false);
		return false;
	}
	GFilterItemCont filter_cont;
	FilterItemName filter_name(filter_cont, it->first);
	auto iter_filtered =main_bag->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (iter_filtered)
	{
		pt_dword cnt = 0;
		for (auto& item : *iter_filtered)
			cnt += item->GetItemCnt();
		return cnt >= it->second;
	}
	return false;
}

bool FilterItemCnt::Parse(const luabind::object& tbl)
{
	using namespace luabind;
	if (type(tbl) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}

	iterator it(tbl), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TTABLE)
		{
			LOG_O(Log_error) << "设置过滤物品数量 之参数必须是表类型 请参看设置函数";
			return false;
		}
		iterator it_value(tbl), it_value_end;
		const auto& item_name = LuaObjCast(v[1], kEmptyStr);
		if (item_name.empty())
		{
			LOG_O(Log_error) << "设置过滤物品数量 表参数第一个必须是物品名字";
			assert(false);
			return false;
		}
		auto item_cnt = LuaObjCast(v[2], 0);
		item_retain_cnt_[item_name] = item_cnt;
		continue;
		
	}
	return true;
}

bool FilterItemName::DoFilt(const GItemBase& obj) const
{
	return obj.GetItemName() == item_name_;
}

bool FilterItemCnt::Before( const GItemBase& obj) const
{
	auto it = item_retain_cnt_.find(obj.GetItemName());
	if (it == item_retain_cnt_.end())
		return false;
	return true;
}

GFilterItemPtr GItemFilterFactory::MakeItemFilter(en_ItemFilterType filter_type)
{
	GFilterItemPtr filter;
	switch (filter_type)
	{
	case kIFT_QuestItem:
		assert(false);
		break;
	case kIFT_BlackName:
		filter.reset(new FilterItemBlackByName());
		break;
	case kIFT_WhiteName:
		filter.reset(new FilterItemWhiteByName());
		break;
	case kIFT_Slot:
		filter.reset(new FilterItemBySlot());
		break;
	case kIFT_Colour:
		filter.reset(new FilterItemByColour());
		break;
	case kIFT_Type:
		filter.reset(new FilterItemByType());
		break;
	case kIFT_GemLink:
		filter.reset(new FilterItemGemLink());
		break;
	case kIFT_ItemCnt:
		filter.reset(new FilterItemCnt());
		break;
	case KIFT_Group:
		filter.reset(new FilterGroupFilter());
		break;
	case KIFT_Bind:
		filter.reset(new FilterItemBindState());
		break;
	case KIFT_ThreeColorLink:
		filter.reset(new FilterItemThreeColorLink());
		break;
	case KIFT_Emboitement:
		filter.reset(new FilterItemEmboitement(GInventoryMgr::GetLackItem()));
		break;
	default:
		break;
	}
	if (filter)
		return filter;
	assert(false);
	return nullptr;
}

bool FilterItemType::DoFilt(const GItemBase& obj) const
{
	return obj.GetItemType() == item_type_;
}

bool FilterItemLessLvl::DoFilt(const GItemBase& obj) const
{
	return true;
}

bool FilterItemLessLvl::Parse(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TNUMBER)
	{
		assert(false);
		return false;
	}
	auto level = LuaObjCast(lua_obj, 0);
	if (level <= 0)
	{
		assert(false);
		return false;
	}
	item_lvl_ = level;
	return true;
}

bool FilterItemBlackNameList::DoFilt(const GItemBase& obj) const
{
	return !item_black_list_.IsIn(obj.GetItemName());
}

bool FilterItemBlackNameList::Parse(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}
	item_black_list_.clear();
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TSTRING)
		{
			assert(false);
			return false;
		}
		auto name = LuaObjCast(v, kEmptyStr);
		if(name.empty())
		{
			assert(false);
			return false;
		}
		item_black_list_.Add(name);
	}
	return true;
}

bool FilterItemLessEqualColour::DoFilt(const GItemBase& obj) const
{
	return obj.GetItemColor() <= item_color_ ;
}

bool FilterItemLessEqualColour::Parse(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TNUMBER)
	{
		assert(false);
		return false;
	}
	auto color = LuaObjCast(lua_obj, kIC_Non);
	if (color == kIC_Non)
	{
		assert(false);
		return false;
	}
	item_color_ = color;
	return true;
}

bool FilterItemPropValue::Parse(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TTABLE)
		{
			assert(false);
			return false;
		}
		auto prop_name = LuaObjCast(v[1], kEmptyStr);
		if (prop_name.empty())
		{
			assert(false);
			return false;
		}
		auto prop_value = LuaObjCast(v[2], -1);
		if (prop_value < 0)
		{
			assert(false);
			return false;
		}
		item_prop_[prop_name] = prop_value;
	}
	return true;
}

bool FilterItemPropValue::DoFilt(const FittedType& obj) const
{
	return true;
}

//GFilterItemPtr FilterItemBase::SetCombine(en_ItemFilterType filter_type, const luabind::object& tbl)
//{
//	auto ptr = GItemFilterFactory::CreateItemFilter(filter_type, tbl);
//	if (!ptr)
//	{
//		assert(false);
//		return nullptr;
//	}
//	Combine_ = ptr;
//	return ptr;
//}

bool FilterItemBindState::DoFilt(const GItemBase& item) const
{
	if (item.IsBind() == bind_state_)
		return true;
	return false;
}

bool FilterItemBindState::Parse(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TBOOLEAN)
	{
		assert(false);
		return false;
	}
	auto state = LuaObjCast(lua_obj, false);
	bind_state_ = state;
	return true;
}

bool FilterGroupFilter::Parse(const luabind::object& lua_obj)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}
	iterator it(lua_obj), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TUSERDATA)
		{
			LOG_O(Log_debug) << "添加组合过滤参数必须是过滤对象，请参看相关事例";
			return false;;
		}
		GFilterItemPtr test = nullptr;
		auto filtr_ptr = LuaObjCast(v, test);
		if (!filtr_ptr)
		{
			assert(false);
			return false;
		}
		group_filter_.push_back(filtr_ptr);
	}
	return true;
}

bool FilterGroupFilter::DoFilt(const GItemBase& obj) const
{
	if (group_filter_.empty())
		return false;
	for (auto& i : group_filter_)
	{
		if (!i->DoFilt(obj))
			return false;
	}
	return true;
}

bool FilterItemThreeColorLink::DoFilt(const GItemBase& obj) const
{
	return obj.HasThreeColorLink();
}

bool FilterItemThreeColorLink::Parse(const luabind::object& lua_obj)
{
	return true;
}

bool FilterItemTypeConT::DoFilt(const GItemBase& obj) const
{
	if (item_Type_.DoFilt(obj.GetItemType()))
		return true;
	return false;
}

bool FilterItemColor::DoFilt(const GItemBase& obj) const
{
	return obj.GetItemColor() == item_color_;
}

bool FilterItemEmboitement::DoFilt(const GItemBase& obj) const
{
	auto it = std::find_if(lock_item_.begin(), lock_item_.end(), [obj](const GClass::stEmboitementInfo& tmp){return tmp.cnt_ > 0 && tmp.item_color_ == obj.GetItemColor() && tmp.item_type == GInventoryMgr::ConvertEmboitementType(obj.GetItemType()); });
	if (it != lock_item_.end())
	{
		--it->cnt_;
		return true;
	}
	return false;
}

bool FilterItemEmboitement::Before(const GItemBase& obj) const
{
	if (!obj.IsEquip() || obj.IsIdentified() || lock_item_.empty())
		return false;
	return true;
}

bool FilterItemEmboitement::Parse(const luabind::object& lua_obj)
{
	return true;
}

#include "stdafx.h"
#include "GItem.h"
#include <Common/LogOut.h>
#include "GClass.h"
#include <Common/MemOper.h>
#include "GConst.h"
#include <boost/regex.hpp>
#include "GPlayer.h"
#include <Scmd.h>
#include "GMap.h"
pt_dword GPackageFlasks::flask_cnt_[kFT_Max] = { 0 };
GClass::LackConT	GInventoryMgr::emboitement_lack_item_;
GItemUsable::GItemUsable(enItemCate item_cate) : GItemBase(item_cate)
{

}

GItemUsable::GItemUsable() : GItemUsable(kICATE_Usable)
{
	usable_type_ = kUT_Invalid;
}

bool GItemUsable::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		//assert(false);
		return false;
	}
	auto attr_usable = item_entity_->GetAttrUsable();
	if (attr_usable)
	{
		auto db_usable = attr_usable->GetAttrDb();
		if (!db_usable)
		{
			assert(false);
			return false;
		}
		usable_type_ = db_usable->usable_type_;
		assert(usable_type_ < kUT_Max);
	}
	return true;
}

bool GItemUsable::Use() const
{
	if (usable_type_ != kUT_UseItem)
	{
		LOG_O(Log_error) << "使用物品之该物品不能直接使用";
		return false;
	}

	if (!GInterface::UseItem(*this))
	{
		LOG_O(Log_error) << "使用物品之使用物品失败，物品名：" << GetItemName();
		return false;
	}
	return true;
}

bool GItemUsable::UseTo(const GItemBase& dst_item) const
{
	if (usable_type_ != kUT_UseItemTo)
	{
		LOG_O(Log_error) << "使用物品到目标之 源物品不能使用到目标，物品名:" << GetItemName();
		return false;
	}
	if (!GInterface::UseItemTo(*this, dst_item))
	{
		LOG_O(Log_error) << "使用物品到目标之使用失败，源物品：" << GetItemName() << "，目标物品：" << dst_item.GetItemName();
		return false;
	}
	return true;
}

GItemFlask::GItemFlask() : GItemWearable(kICATE_Flask)
{
	flask_type_ = kFT_Invalid;
	flask_hp_ = 0;
	flask_mp_ = 0;
	flask_time_ = 0;
	cur_charges_ = 0;
	per_charges_ = 0;
}

bool GItemFlask::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		//assert(false);
		return false;
	}
	assert(item_entity_);

	auto db_flask = item_entity_->GetAttrDbFlask();
	if (!db_flask)
	{
		assert(false);
		return false;
	}
	if (!db_flask->flask_values_.IsValid())
	{
		assert(false);
		return false;
	}
	if (db_flask->flask_values_.empty())
	{
		assert(false);
		return false;
	}
	flask_type_ = db_flask->flask_type_;
	assert(flask_type_ > kFT_Invalid && flask_type_ < kFT_Max + 5);
	auto& flask_values = db_flask->flask_values_[0];
	flask_hp_ = flask_values.hp_.value_;
	flask_mp_ = flask_values.mp_.value_;
	flask_time_ = flask_values.time_.value_;

	auto attr_charges = item_entity_->GetAttrCharges();
	if (!attr_charges)
	{
		assert(false);
		return false;
	}
	auto db_charges = attr_charges->GetAttrDb();
	if (!db_charges)
	{
		assert(false);
		return false;
	}
	cur_charges_ = attr_charges->cur_charges_;
	per_charges_ = db_charges->per_charges_;
	assert(cur_charges_ >= 0);
	assert(per_charges_ > 0);
	return true;
}

void GItemFlask::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_debug) << ",当前充能:" << ",每次消耗:" << per_charges_ << ",持续时间:" << flask_time_ << ",补血:" << flask_hp_ << ",补蓝:" << flask_mp_;
}

int GItemFlask::GetHpMpSpeed() const
{
	int speed = 0;
	if (flask_time_ <= 0)
	{
		//assert(false);
		return 0;
	}
	if (flask_type_ == kFT_Hp)
	{
		speed = flask_hp_ * 10 / flask_time_;
		assert(speed > 0);
	}
	else if (flask_type_ == kFT_Mp)
	{
		speed = flask_mp_ * 10 / flask_time_;
		assert(speed > 0);
	}
	else if (flask_type_ == kFT_Utility)
	{
		speed = per_charges_ * 10 / flask_time_;
	}

	return speed;
}

enMoveItemToRes GItemFlask::MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return MoveToImpl(this, dst, pos);
}

enPutItemDownRes GItemFlask::PutDownTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return dst.PutDownToHere(*this, pos);
}

enCD_FlaskType GItemFlask::GetFlaskType() const
{
	return flask_type_;
}

bool GItemFlask::Use() const
{
	auto& gpm = GPlayerMe::GetMe();
	if (flask_type_ == kFT_Hp)
	{
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (gpm.GetMaxHp() <= gpm.GetCurHp())
		{
			LOG_O(Log_debug) << "使用药剂之当前生命值是满的";
			return true;
		}
	}
	else if (flask_type_ == kFT_Mp)
	{
		if (!gpm.Update())
		{
			assert(false);
			return false;
		}
		if (gpm.GetMaxMp() <= gpm.GetCurMp())
		{
			LOG_O(Log_debug) << "使用药剂之当前魔力值是满的";
			return true;
		}
	}
	if (!gpm.UseEatFlask(*this))
	{
		assert(false);
		return false;
	}
	return true;
}

int GItemFlask::WeightImpl() const
{
	auto the_value = __super::WeightImpl();
	the_value += GEquipScore::GetMe().CalcItemGoodValue(*this);
	return the_value;
}

void GItemFlask::PrintWeight() const
{
	GEquipScore::GetMe().PrintItemWeightInfo(*this);
	__super::PrintWeight();
}

void GItemFlask::PushValidFlaskSpeedInfo(GFlaskSpeedCont& cont) const
{
	if (cur_charges_ <= 0)
		return;
	//判断当前剩余是否够一次
	if (GetSurplusTimes() <= 0)
		return;
	GClass::stFlaskSpeed tmp;
	tmp.index_ = GetCurItemPos().x;
	tmp.speed_ = GetHpMpSpeed();
	cont[flask_type_].insert(tmp);
}

GPackageBasePtr GItemFlask::GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const
{
	GPackageBasePtr pack_ptr;
	if (1 == positions.size())
		pack_ptr = positions[0];
	if (!pack_ptr)
	{
		pack_ptr = GPackageMgrBase().NewPackage(kIT_Flasks);
		if (!pack_ptr)
		{
			assert(false);
			return nullptr;
		}
	}	

	auto pos = pack_ptr->GetOneCanPutPos(*this);
	if (!pos.AnyLess(0))
	{
		if (out_pos)
			*out_pos = pos;
		return pack_ptr;
	}
	return nullptr;
}

bool GItemFlask::IsEquip() const
{
	return true;
}

GItemGem::GItemGem() : GItemBase(kICATE_Gem)
{
	gem_color_ = kGSC_Invalid;
	cur_lvl_ = 0;
}

bool GItemGem::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		//assert(false);
		return false;
	}
	auto attr_skill_gem = item_entity_->GetAttrSkillGem();
	if (!attr_skill_gem)
	{
		assert(false);
		return false;
	}
	auto db_skill_gem = attr_skill_gem->GetAttrDb();
	if (!db_skill_gem)
	{
		assert(false);
		return false;
	}
	astrict_.level_ = attr_skill_gem->GetRequireLvl();
	astrict_.power_ = attr_skill_gem->GetRequireAttr(kIRA_Power);
	astrict_.smart_ = attr_skill_gem->GetRequireAttr(kIRA_Smart);
	astrict_.brain_ = attr_skill_gem->GetRequireAttr(kIRA_Brain);
	cur_lvl_ = attr_skill_gem->cur_lvl_;
	gem_color_ = db_skill_gem->gem_color_;
	return true;
}

enCD_GemSlotColor GItemGem::GetItemGemColor() const
{
	return gem_color_;
}

enInsideGemRes GItemGem::InsideGemTo(int gem_idx, GItemBase& dst_equip) const
{
	return dst_equip.DoInsideGem(gem_idx, *this);
}

void GItemGem::GetAllPackGem(GItemContT& all_gem) const
{
	//if (!CanWear())
	//	return;
	auto this_ptr = (GItemGem*)this;
	all_gem.push_back(this_ptr->shared_from_this());
}

bool GItemGem::IsMustKeepItem() const
{
	//可能需要增加相同的宝石不捡取
	return false;
}

void GItemGem::DebugInfo() const
{
	__super::DebugInfo();
	LOG_O(Log_trace) << "gem_color_:" << gem_color_ << ",cur_lvl:" << cur_lvl_
		<< ",需求等级：" << astrict_.level_
		<< ",需求力量：" << astrict_.power_ << ",需求敏捷：" << astrict_.smart_ << ",需求智力：" << astrict_.brain_;
}

GItemArmor::GItemArmor() : GItemEquipBase(kICATE_Armor)
{

}

GItemQuest::GItemQuest() : GItemUsable(kICATE_Quest)
{

}

bool GItemQuest::DoFilter() const
{
	return true;
}

bool GItemQuest::IsMustKeepItem() const
{
	return true;
}

GItemWearable::GItemWearable(enItemCate item_cate) : GItemBase(item_cate)
{

}

GItemDecoration::GItemDecoration() : GItemDecoration(kICATE_Decoration)
{

}

GItemDecoration::GItemDecoration(enItemCate item_cate) : GItemWearable(item_cate)
{

}

bool GItemDecoration::IsEquip() const
{
	return true;
}

GPackageFlasks::GPackageFlasks() : GPackageWearable(kIT_Flasks)
{

}

enMoveItemToRes GPackageFlasks::MoveItToHere(const GItemBase& item, const stCD_VecInt& pos)
{
	return kMITR_CanNotBePutted;
}

enMoveItemToRes GPackageFlasks::MoveItToHere(const GItemFlask& item, const stCD_VecInt& pos)
{
	if (pos.x >= 5 || pos.y < 0)
	{
		LOG_O(Log_debug) << "装备药瓶的位置必须是 0-5 之间";
		return kMITR_Failed;
	}

	GPackageWearable* ptr = this;
	return ptr->MoveItToHere((const GItemWearable&)item, pos);
}

bool GPackageFlasks::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		//assert(false);
		return false;
	}
	flask_speed_.clear();
	for (auto& i : items_)
		i->PushValidFlaskSpeedInfo(flask_speed_);
	return true;
}

const GFlaskSpeedCont* GPackageFlasks::GetFlaskSpeedCont() const
{
	return &flask_speed_;
}

pt_dword GPackageFlasks::GetFlaskCntByType(enCD_FlaskType flask_type) const
{
	if (items_.empty())
		return 0;

	pt_dword value = 0;
	for (auto& i : items_)
	{
		if (i->GetFlaskType() == flask_type)
			++value;
	}
	return value;
}


bool GPackageFlasks::GoodThanAll(const GItemBase& item) const
{
	if (items_.empty())
		return true;
	auto flas_type = item.GetFlaskType();
	if (flas_type <= kFT_Invalid || flas_type >= kFT_Max)
	{
		assert(false);
		return false;
	}
	pt_dword cur_cnt = GetFlaskCntByType(flas_type);
	if (cur_cnt < flask_cnt_[flas_type])
		return true;

	if (items_.empty())
		return true;
	auto the_value = item.Weight();
	for (auto& v : items_)
	{
		if (v->GetFlaskType() != flas_type)
			continue;

		if (v->Weight() < the_value)
			return true;
	}
	return false;
}

stCD_VecInt GPackageFlasks::GetOneCanPutPos(const GItemFlask& item) const
{
	stCD_VecInt tmp(-1, -1);
	if (items_.empty())
		return tmp;
	
	auto flas_type = item.GetFlaskType();
	if (flas_type <= kFT_Invalid || flas_type >= kFT_Max)
	{
		assert(false);
		return tmp;
	}
	if (!GoodThanAll(item))
		return tmp;
	pt_dword cur_cnt = GetFlaskCntByType(flas_type);
	if (cur_cnt <= flask_cnt_[flas_type])
	{
		auto tmp = FindFreePosBySize(item.GetItemSize());
		if (!tmp.AnyLess(0))
			return tmp;
		auto the_value = item.Weight();
		for (auto& v : items_)
		{
			if (v->GetItemType() != item.GetItemType())
				continue;
			if (v->Weight() < the_value)
				return v->GetCurItemPos();
		}
		//return tmp;
	}

	//没有找到位置  看那个瓶子多了
	std::map<enCD_FlaskType, std::vector<GItemBasePtr>> map_flask;
	for (auto& i : items_)
		map_flask[i->GetFlaskType()].push_back(i);
	for (auto& v : map_flask)
	{
		if (v.first <= kFT_Invalid || v.first >= kFT_Max)
			continue;
		if (v.second.size() <= flask_cnt_[v.first])
			continue;
		std::sort(v.second.begin(), v.second.end(), [](const GItemBasePtr& left, const GItemBasePtr right){return left->Weight() < right->Weight(); });
		auto it = v.second.begin();
		if (it == v.second.end())
		{
			assert(false);
			return tmp;
		}
		tmp = (*it)->GetCurItemPos();
		break;
	}
	return tmp;
}

void GPackageFlasks::SetBagFlaskCnt(enCD_FlaskType type, pt_dword cnt)
{
	if (type >= kFT_Max || type  <= kFT_Invalid)
	{
		assert(false);
		return;
	}
	if (cnt > kFlaskMaxCnt)
	{
		assert(false);
		return;
	}
	flask_cnt_[type] = cnt;
}

GPackageMain::GPackageMain() : GPackageBase(kIT_MainBag)
{
}

bool GPackageMain::AdjustPack()
{
	if (items_.empty())
		return true;

	if (!package_size_.AllGreaterZero())
	{
		LOG_O(Log_debug) << "pack_AdjustPack_error:" << "Pack_Type-" << package_type_ << package_size_;
		assert(false);
		return false;
	}
	InitAdjustData();
	bool is_update = false;
	stCD_VecInt nextpos(-1, -1);
	while (NextNullPos(nextpos))
	{
		auto itemBig = FindMaxSpaceByPos(nextpos.x, nextpos.y);
		if (!itemBig.AllGreaterZero())
			continue;

		auto findItem = FindCanMoveItem(nextpos, itemBig);
		if (!findItem)
		{
			is_update = false;
			continue;
		}
		is_update = true;
		if (!MoveItemToPos(*findItem, nextpos))
		{
			assert(false);
			return true;
		}
		if (is_update)
		{
			if (!UpdateImpl())
			{
				assert(false);
				return false;
			}
			InitAdjustData();
		}
	}
	if (!UpdateImpl())
	{
		assert(false);
		return false;
	}
	return true;
}

void GPackageMain::InitAdjustData()
{
	//LOG_O(Log_debug) << "初始化背包数据 : Type-"<<bag_type_;
	map_item_.clear();
	if (items_.empty())
		return;

	stItemCellState stTmp;
	for (auto& i : items_)
	{
		const auto& tmp = i->GetItemGridInfo();
		assert(tmp.GetItemGridCnt() <= kCellMaxCnt);
		stTmp.pos_info_ = i;
		map_item_[tmp.GetItemGridCnt()].push_back(stTmp);
	}
}

GPackageEquips::GPackageEquips(enCD_InventoryType inventory_type) : GPackageWearable(inventory_type)
{
}

enMoveItemToRes GPackageEquips::MoveItToHere(const GItemEquipBase& item, const stCD_VecInt& pos)
{
	auto the_old = GetFirstItem();
	if (the_old)
	{
		auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
		if (!main_bag)
		{
			assert(false);
			return kMITR_Failed;
		}

		auto res = the_old->TakeoffGemTo(-1, *main_bag, kInvalidVecInt);
		if (res < kTOGR_Succeed)
		{
			if (res != kTOGR_HasNotGem)
			{
				assert(false);
				return kMITR_Failed;
			}
		}
		if (!Update())
		{
			assert(false);
			return kMITR_Failed;
		}
#ifdef _DEBUG
		auto new_item = GetFirstItem();
		if (!new_item)
			assert(false);
		GItemContT all_gem;
		new_item->GetAllPackGem(all_gem);
		if (!all_gem.empty())
			assert(!"严重问题，卸载宝石没有卸载干净");
#endif
	}
	GPackageWearable* ptr = this;
	return ptr->MoveItToHere((const GItemWearable&)item, pos);
}

GItemBasePtr GPackageMain::FindCanMoveItem(const stCD_VecInt& coor, const stSizeInt& big)
{
	if (big >= package_size_)
		return nullptr;

	if (map_item_.empty())
		return nullptr;
	
	GItemBasePtr findItem;
	Map_cont::iterator iterl;
	int big_little = big.Size();

	for (int i = big_little; i > 0; --i)
	{
		if (findItem)
			break;

		iterl =  map_item_.find(i);
		if (iterl == map_item_.end())
			continue;
		
		if (iterl->second.empty())
			continue;
		
		for (auto& i : iterl->second)
		{
			if (!i.pos_info_)
				continue;
		
			auto& itemInfo = i.pos_info_->GetItemGridInfo();

			int retsrc = CalcItemLocation(coor.x, coor.y);
			int retdst = CalcItemLocation(itemInfo.GetLeftX(), itemInfo.GetTopY());
			if (retsrc < 0 || retdst < 0)
			{
				assert(false);
				continue;
			}
			if (retsrc > retdst)
				continue;

			if (!i.pos_info_->IsInSize(big))
				continue;

			findItem = i.pos_info_;
			break;
		}
	}
	return findItem;
}

bool GPackageMain::MoveItemToPos(GItemBase& item, const stCD_VecInt& pos)
{
	int _X = pos.x;
	int _Y = pos.y;
	const stCD_ItemGridsIdxInfo& posinfo =  item.GetItemGridInfo();
	if (!package_size_.InSize(_X, _Y))
	{
		assert(false);
		return false;
	}
		
	//该位置是否有物品
	int itemcnt = 0;
	GItemBasePtr cur_pos_item;
	for (int i = _X; i < _X + posinfo.GetWidth(); ++i)
	{
		for (int j = _Y; j < _Y + posinfo.GetHeight(); ++j)
		{
			if (GetGridState(i, j))
				continue;

			cur_pos_item = GetPosItem(pos);
			if (cur_pos_item)
				++itemcnt;
		}
	}

	if (itemcnt > 1)
	{
		assert(false);
		return false;
	}

	if (!item.SetItemPos(package_type_, pos))
	{
		assert(false);
		return false;
	}
	return true;
}

int GPackageMain::CalcItemLocation(int _x, int _y)
{
	if (_x < 0 || _y < 0)
	{
		assert(false);
		return -1;
	}
	return _x * package_size_.Height() + _y;
}

enPutItemDownRes GPackageMain::PutDownToHere(const GItemBase& item, const stCD_VecInt& pos)
{
	return PutDownToHereImpl(item, pos);
}

enMoveItemToRes GPackageMain::OnMoveItToThereFailed(GPackageWearable& dst_package, const GItemBase& item, const stCD_VecInt& dst_pos)
{
	auto dst_pos_tmp = dst_pos;
	if (dst_pos_tmp.AnyLess(0))
		dst_pos_tmp.assign(0, 0);
	auto dst_item = dst_package.FindItemByPos(dst_pos_tmp);
	if (!dst_item)
		return dst_package.MoveItToHereDirect(item, dst_pos_tmp);
	auto res = dst_package.MoveItToHereDirect(item, dst_pos_tmp);
	if (res < kMITR_Succeed)
	{
		assert(false);
		return res;
	}

	auto attached_item = GInterface::GetMouseAttachItem();
	if (!attached_item)
		return kMITR_Succeed;
	if (attached_item->IsCanDestroy())
	{
#ifdef _DEBUG
		LOG_O(Log_info) << "-------------------------------------------------";
		LOG_O(Log_info) << "要销毁装备评分计算如下:";
		LOG_O(Log_info) << "物品名字:" << attached_item->GetItemName();
		attached_item->PrintWeight();
		LOG_O(Log_info) << "总分值:" << attached_item->Weight();
		LOG_O(Log_info) << "-------------------------------------------------";
		LOG_O(Log_info) << "新装备评分计算如下:";
		LOG_O(Log_info) << "物品名字:" << item.GetItemName();
		item.PrintWeight();
		LOG_O(Log_info) << "总分值:" << item.Weight();
		LOG_O(Log_info) << "-------------------------------------------------";
#endif 

		LOG_O(Log_trace) << "穿戴装备之 销毁替换的装备:" << attached_item->GetItemName();
		if (!attached_item->Destroy(false))
			assert(false);
		return kMITR_Succeed;
	}
	if (!Update())
	{
		assert(false);
		return kMITR_Failed;
	}
	auto ret_pos = FindFreePosBySize(attached_item->GetItemSize());
	if (ret_pos.AllLess(0))
	{
		LOG_O(Log_debug) << "脱下物品 不能放入背包";
		assert(false);
		return kMITR_Failed;
	}
	if (item.PutDownTo(*this, ret_pos) < kPIDR_Succeed)
	{
		assert(false);
		return kMITR_Failed;
	}
	return kMITR_Succeed;
}

const IFilterPos* GPackageMain::GetRetainFilterPos(const GItemBase* item) const
{
	if (item)
	{
		if (item->IsMustKeepItem())
			return nullptr;
	}
	return &GInventoryMgr::GetMe().GetMainBagRetainFilterPos(*this);
}

enMoveItemToRes GPackageMain::MoveItToHere(const GItemBase& item, const stCD_VecInt& pos)
{
	return  MoveItToHereImpl(item, pos);
}

enMoveItemToRes GPackageMain::MoveItToHere(const GItemWearable& item, const stCD_VecInt& pos)
{
	if (item.IsCanDestroy())
	{
		LOG_O(Log_trace) << "移动到背包之 发现装备需要摧毁:" << item.GetItemName();
		if (item.Destroy(true))
			return kMITR_Succeed;
		else
			assert(false);
	}
	return __super::MoveItToHere(item, pos);
}

GPackageItemPair GInventoryMgr::FindItemByName(const std::string& item_name, enCD_InventoryType type)
{
	GPackageItemPair res;
	res.first = NewPackage(type);
	if (!res.first)
	{
		assert(false);
		return GPackageItemPair();
	}
	res.second = res.first->FindItemByName(item_name);
	return res;
}

bool GInventoryMgr::FindFreeGemSolt(enCD_GemSlotColor _solt_color, enCD_InventoryType& _type, int& _index )
{
	PackagesContT packages;
	if (!GetEquipPackages(packages, false))
	{
		assert(false);
		return false;
	}
	bool isfind(false);
	for (auto& i : packages)
	{
		int index = i->FindFreeGemIndex(_solt_color);
		if (index < 0)
			continue;

		_type = i->GetPackageType();
		_index = index;
		isfind = true;
		break;
	}

	return isfind;
}

GPackageBasePtr GInventoryMgr::GetPackBase(const enCD_InventoryType inventoryType)
{
	return NewPackage(inventoryType);
}

bool GInventoryMgr::GetAllGemSlotInfo(GClass::VcSlotInfoConT& ret_vec)
{
	PackagesContT packages;
	if (!GetEquipPackages(packages, false))
	{
		assert(false);
		return false;
	}
	for (auto& i : packages)
	{
		auto equip = i->GetFirstItem();
		if (!equip)
			continue;

		auto slot_link = equip->GetGemLink();
		if (!slot_link || slot_link->empty())
			continue;

		int index = 0;
		GClass::stGemSoltInfo tmp;
		for (auto v : *slot_link)
		{
			tmp.link_value_ = v;
			tmp.dst_inventory_ = equip->GetInventoryType();
			tmp.solt_color_.clear();
			GClass::stSlotInfo slot;
			for (int m = 0; m < v; ++m)
			{
				slot.index_ = index;
				slot.color_ = equip->GetColorByIndex(index);
				++index;
				tmp.solt_color_.push_back(slot);
			}
			ret_vec.push_back(tmp);
		}
	}
	if (!ret_vec.empty())
		std::sort(ret_vec.begin(), ret_vec.end(), [](const GClass::stGemSoltInfo& left, const GClass::stGemSoltInfo& right){return left.link_value_ > right.link_value_; });
	return true;
}

void GInventoryMgr::GetAllPackGem(GItemContT& all_gem)
{
	PackagesContT packages;
	if (!GetEquipPackages(packages, false))
	{
		assert(false);
		return;
	}
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return;
	}
	packages.push_back(main_bag);
	for (auto& i : packages)
		i->GetAllPackGem(all_gem);
	return;
}

bool GInventoryMgr::FindGemPosByEquip(enCD_InventoryType& type, stCD_VecInt& item_pos, int& index, const std::string& gem_name, const GAutoUpDateGem::GemDstPos& filter_gem)
{
	PackagesContT packages;
	if (!GetEquipPackages(packages, false))
	{
		assert(false);
		return false;
	}
	GVectorInt vec_index;
	for (auto& i : packages)
	{
		auto equip = i->GetFirstItem();
		if (!equip)
			continue;
		vec_index.clear();
		equip->GetGemIndexByName(gem_name, vec_index);
		if (vec_index.empty())
			continue;
		type = equip->GetInventoryType();
		item_pos = equip->GetCurItemPos();
		if (filter_gem.empty())
		{
			index = vec_index[0];
			return true;
		}
		for (auto& v : vec_index)
		{
			auto it = std::find_if(filter_gem.begin(), filter_gem.end(), [&equip, v, type](const GAutoUpDateGem::stGemPosInfo& gem_info){return gem_info.dst_inventory_ == type && gem_info.index_ == v; });
			if (it != filter_gem.end())
				continue;
			index = v;
			return true;
		}
		//if (!filter_gem.empty())
		//{
		//	auto equip_inventory = equip->GetInventoryType();
		//	auto it = std::find_if(filter_gem.begin(), filter_gem.end(), [&equip, gem_index, equip_inventory](const GAutoUpDateGem::stGemPosInfo& gem_info){return gem_info.dst_inventory_ == equip_inventory && gem_info.index_ == gem_index; });
		//	if (it != filter_gem.end())
		//		continue;
		//}
		//type = equip->GetInventoryType();
		//item_pos = equip->GetCurItemPos();
		//index = v;
		//return true;
	}
	return false;
}

bool GInventoryMgr::GetEquipPackages(PackagesContT& packages, bool include_flask) const
{
	if (include_flask)
		NewPackage(kIT_BodyArmour, kIT_Flasks, packages);
	else
		NewPackage(kIT_BodyArmour, kIT_Belt, packages);
	if (packages.empty())
	{
		assert(false);
		return false;
	}
	return true;
}

const GFilterRect& GInventoryMgr::GetMainBagRetainFilterPos(const GPackageMain& package)
{	
	const auto& bag_size = package.GetPackageSize();
	if (!bag_size.InSize(main_bag_retain_size_.Width(), main_bag_retain_size_.Height()))
	{
		main_bag_filter_rect_.SetRect(kInvalidVecInt, kInvalidVecInt);
		return main_bag_filter_rect_;
	}
	stCD_VecInt left_bottom(bag_size.Width(), bag_size.Height()), right_top(bag_size.Width(), bag_size.Height());
	left_bottom -= main_bag_retain_size_;
	right_top -= 1;
	main_bag_filter_rect_.SetRect(left_bottom, right_top);
	return main_bag_filter_rect_;
}

GInventoryMgr::GInventoryMgr() : main_bag_filter_rect_(kInvalidVecInt, kInvalidVecInt)
{
	emboitement_Cnt_ = 0;
}

void GInventoryMgr::SetMainPackageRetainSize(const stSizeInt& size)
{
	main_bag_retain_size_ = size;
}

bool GInventoryMgr::UseItem(const std::string& item_name)
{
	if (item_name.empty())
	{
		LOG_O(Log_error) << "使用物品之物品名为空";
		return false;
	}
	auto item = FindItemByName(item_name, kIT_MainBag);
	if (!item.second)
	{
		LOG_O(Log_error) << "使用物品之没有找到该物品，物品名：" << item_name;
		return false;
	}
	return item.second->Use();
}

bool GInventoryMgr::UseItemToDst(const std::string& src_name, const std::string& dst_name)
{
	if (src_name.empty() || dst_name.empty())
	{
		LOG_O(Log_error) << "使用物品到目标之 参数名不能为空";
		assert(false);
		return false;
	}
	auto mai_bag = GetPackBase(kIT_MainBag);
	if (!mai_bag)
	{
		assert(false);
		return false;
	}
	auto src_item = mai_bag->FindItemByName(src_name);
	if (!src_item)
	{
		LOG_O(Log_error) << "使用物品到目标之 没找到源物品，物品名：" << src_name;
		return false;
	}
	auto dst_item = mai_bag->FindItemByName(dst_name);
	if (!dst_item)
	{
		LOG_O(Log_error) << "使用物品到目标之 没找到目标，物品名：" << dst_name;
		return false;
	}
	return src_item->UseTo(*dst_item);
}

void GInventoryMgr::GetGoodEquipsToWear(GBodyTotalEquipsCont& out_equips, GBodyFlasksCont& out_flasks, const GItemBasePtr& addition_item)
{
	PackagesContT packages;
	NewPackage(kIT_MainBag, kIT_Flasks, packages);
	if (packages.size() != kIT_Flasks)
	{
		assert(false);
		return;
	}
	auto main_package = packages[kIT_MainBag - 1];
	if (!main_package)
	{
		assert(false);
		return;
	}
	BOOST_SCOPE_EXIT_ALL(&){
		GEquipScore::GetMe().SetCalcProp(true);
	};
	GFilterItemCont filter_cont;
	FilterItemEquip filter_equip(filter_cont);
	FilterItemCanWear filter_canwear(filter_cont);
	FilterItemIdentified filter_indentified(filter_cont);
	FilterItemNotFlask filter_not_flask(filter_cont);
	CmnStaticVector<GItemBasePtr, 500> main_bag_items_backup;
	auto& main_bag_items = main_package->GetItems();
	if (addition_item)
	{
		if (!addition_item->IsIdentified())
			GEquipScore::GetMe().SetCalcProp(false);
			
		auto iter_filtered = main_package->FilterIterator<IterContTag_ByRef>(filter_cont);
		if (!iter_filtered)
		{
			assert(false);
			return;
		}
		for (auto& item : *iter_filtered)
			main_bag_items_backup.push_back(item);
		main_bag_items.clear();
		for (auto& v : main_bag_items_backup)
			main_bag_items.push_back(v);
		main_bag_items_backup.clear();
		main_bag_items.push_back(addition_item);
		filter_cont.Erase(filter_canwear);
		filter_cont.Erase(filter_indentified);
	}
	auto main_bag_items_backup_tmp = main_bag_items;
	for (int it = kIT_MainBag; it < kIT_Belt; ++it)
	{
		for (auto& v : packages[it]->GetItems())
			main_bag_items.push_back(v);
	}
	GItemStaticContT good_items_tmp;
	main_package->GetGoodItems(filter_cont, good_items_tmp);
	if (!good_items_tmp.empty())
	{
		GWearGoodEquips good_equips((pt_word)kIT_Belt - 1);
		assert(good_equips.size() == kIT_Belt - 1);
		//////////////////////////////////////////////////////////////////////////
		for (int i = 0; i < good_equips.MaxSize; ++i)
		{
			auto& package = packages[i + 1];
			if (!package)
			{
				assert(false);
				continue;
			}
			auto first_item = package->GetFirstItem();
			if (!first_item)
				continue;
			good_equips[i].equip_ = first_item;
			good_equips[i].weight_ = first_item->Weight();
		}
		//////////////////////////////////////////////////////////////////////////
		main_bag_items.clear();
		for (auto& v : good_items_tmp)
			main_bag_items.push_back(v);

		//对于目标没有穿戴物品的，则穿任意一件物品
		for (int i = 0; i < good_equips.MaxSize; ++i)
		{
			auto& package = packages[i + 1];
			if (!package)
			{
				assert(false);
				continue;
			}
			auto first_item = package->GetFirstItem();
			if (first_item)
				continue;
			first_item = main_package->GetAnyCanWearEquip(good_equips, package->GetPackageType());
			if (!first_item)
				continue;
			assert(!good_equips[i].equip_);
			good_equips[i].equip_ = first_item;
			good_equips[i].weight_ = first_item->Weight();
		}
		//////////////////////////////////////////////////////////////////////////
		auto& type_mgr = GEquipTypeMgr::GetMe();
		for (auto& item : good_items_tmp)
		{
			GWearEquipInfo wear_info(item, item->Weight());
			const auto& package_positions = type_mgr.GetPositions(*item);
			for (auto position : package_positions)
			{
				if (position <= kIT_MainBag || position > kIT_Belt)
				{
					assert(false);
					continue;
				}
				auto& cur_equip = good_equips[position - 2];
				const auto& switch_positions = type_mgr.GetSwitchItemTypes(*item, position);
				if (!switch_positions.empty())
				{
					//bool need_ignore = false;
					int valid_cnt = 0;
					for (auto& item_other : good_items_tmp)
					{
						if (item == item_other)
							continue;
						++valid_cnt;
						enCD_InventoryType other_pos = kIT_Invalid;
						if (!type_mgr.CanWearTheEquip(switch_positions, *item_other, &other_pos))
							continue;
						if (other_pos <= kIT_MainBag || other_pos > kIT_Belt)
						{
							assert(false);
							continue;
						}
						auto& other_equip = good_equips[other_pos - 2];
						GWearEquipInfo other_wear_info(item_other, item_other->Weight());
						if (cur_equip.weight_ + other_equip.weight_ >= wear_info.weight_ + other_wear_info.weight_)
						{
							//need_ignore = true;
							continue;
						}
						//need_ignore = true;
						cur_equip = wear_info;
						other_equip = other_wear_info;
						continue;
					}
					/*if (need_ignore)
						continue;
					*/
					if (0 != valid_cnt)
						continue;
					auto it_good = std::find_if(good_equips.begin(), good_equips.end(), [&item](const GWearEquipInfo& equip){
						return equip.equip_ == item;
					});
					if (it_good != good_equips.end())
						continue;
				}
				if (cur_equip.weight_ < wear_info.weight_)
				{
					cur_equip = wear_info;
					auto switch_pos = GEquipTypeMgr::SwitchPosition(position);
					if (switch_pos != kIT_Invalid)
						good_equips[switch_pos - 2].reset();
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		for (pt_word idx = 0; idx < good_equips.size(); ++idx)
		{
			auto& the_equip = good_equips[idx];
			if (!the_equip.equip_)
				continue;
			auto old_position = the_equip.equip_->GetInventoryType();
			if (idx + 2 == old_position)
				continue;
			if (old_position > kIT_MainBag && old_position < kIT_Flasks)
			{
				if (the_equip.equip_->IsRing())
				{
					std::swap(the_equip, good_equips[old_position - 2]);
					continue;
				}
				else if (the_equip.equip_->GetItemType() == kITYPE_Wand)
				{
					auto& other_equip = good_equips[old_position - 2];
					if (other_equip.equip_ && other_equip.equip_->GetItemType() == kITYPE_Wand)
						std::swap(the_equip, other_equip);
				}
				else if (type_mgr.GetWeaponCompositeCate(*the_equip.equip_) == kWCC_OneHand)
				{
					auto& other_equip = good_equips[old_position - 2];
					if (other_equip.equip_)
					{
						auto other_cate = type_mgr.GetWeaponCompositeCate(*other_equip.equip_);
						if (other_cate != kWCC_OneHand)
							continue;
					}
					std::swap(the_equip, other_equip);
					continue;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		for (pt_word idx = 0; idx < good_equips.size(); ++idx)
		{
			auto& the_equip = good_equips[idx];
			if (!the_equip.equip_)
				continue;
			auto position = (enCD_InventoryType)(idx + 2);
			if (position == the_equip.equip_->GetInventoryType())
				continue;
			out_equips.push_back({ position, the_equip.equip_ });
		}
	}	
	//////////////////////////////////////////////////////////////////////////
	FilterItemFlask filter_flask(filter_cont);
	filter_cont.Erase(filter_not_flask);
	main_bag_items.clear();
	for (auto& v : main_bag_items_backup_tmp)
		main_bag_items.push_back(v);
	auto iter = main_package->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (!iter)
	{
		assert(false);
		return;
	}
	auto package_flask = packages.back();
	if (!package_flask)
	{
		assert(false);
		return;
	}
	PackagesThinContT flask_package = { package_flask };
	CmnStaticVector<stBodyFlaskInfo, kFT_Max> good_flask(kFT_Max);
	for (auto& v : *iter)
	{
		stCD_VecInt pos_tmp(0, 0);
		if (v->GoodThanOwner(&pos_tmp, flask_package))
		{
			auto flask_type = v->GetFlaskType();
			if (flask_type >= kFT_Max)
			{
				assert(false);
				continue;
			}
			auto& cur_flask = good_flask[flask_type];
			if (!cur_flask.flask_)
				goto LABLE_FIND;
			if (cur_flask.flask_->Weight() < v->Weight())
				goto LABLE_FIND;
			continue;
		LABLE_FIND:
			cur_flask.flask_ = v;
			cur_flask.pos_ = pos_tmp;
		}
	}
	for (auto& v : good_flask)
	{
		if (!v.flask_)
			continue;
		out_flasks.push_back(v);
	}	
	//////////////////////////////////////////////////////////////////////////
}

bool GInventoryMgr::NeedTakeupTheDropItem(const GItemBasePtr& item)
{
	if (!item)
	{
		assert(false);
		return false;
	}
	if (item->IsMustKeepItem())
		return true;

	if (!MyLuaThrdData::GetAutoWearEquip())
		return false;
	GBodyTotalEquipsCont equips;
	GBodyFlasksCont flasks;
	GetGoodEquipsToWear(equips, flasks, item);
	for (auto& equip : equips)
	{
		if (equip.equip_ == item)
			return true;
	}
	for (auto& flask : flasks)
	{
		if (flask.flask_ == item)
			return true;
	}
	//if (item->DoFilter())
	//	return true;
	return false;
}

int GInventoryMgr::AutoWearSomeEquips()
{
	GBodyTotalEquipsCont equips;
	GBodyFlasksCont flasks;
	GetGoodEquipsToWear(equips, flasks, nullptr);
	int cnt = 0;
	for (auto& equip : equips)
	{
		auto cur_equip = equip.equip_;
		assert(cur_equip);
		auto dst_package = NewPackage(equip.position_);
		if (!dst_package)
		{
			assert(false);
			break;
		}
		auto src_package = NewPackage(cur_equip->GetInventoryType());
		if (!src_package)
		{
			assert(false);
			break;
		}
		cur_equip = src_package->FindByWeightInfo(cur_equip);
		if (!cur_equip)
		{
			if (src_package->GetPackageType() == kIT_MainBag)
			{
				assert(false);
				break;
			}
			auto main_package = NewPackage(kIT_MainBag);
			if (!main_package)
			{
				assert(false);
				break;
			}
			cur_equip = main_package->FindByWeightInfo(equip.equip_);
			if (!cur_equip)
			{
				assert(false);
				break;
			}
		}
		if (cur_equip->MoveTo(*dst_package, kInvalidVecInt) < kMITR_Succeed)
		{
			assert(false);
			break;
		}
		++cnt;
	}
	//////////////////////////////////////////////////////////////////////////
	auto package_flask = NewPackage(kIT_Flasks);
	if (!package_flask)
	{
		assert(false);
		return cnt;
	}
	PackagesThinContT flask_package = { package_flask };
	for (auto it = flasks.begin(), it_end = flasks.end(); it != it_end; ++it)
	{
		auto& the_flask = *it;
		assert(the_flask.flask_);
		if (the_flask.flask_->MoveTo(*package_flask, the_flask.pos_) < kMITR_Succeed)
		{
			assert(false);
			break;
		}
		++cnt;
		if (it + 1 != it_end)
		{
			if (!package_flask->Update())
			{
				assert(false);
				break;
			}
			if (!the_flask.flask_->GoodThanOwner(&(it + 1)->pos_, flask_package))
				break;
		}
	}
	return cnt;
}

bool GInventoryMgr::AutoClearMainBagBySize(const stSizeInt& size)
{
	if (size.AnyLess(0) || size.AnyGreater(kItemMaxSize))
		return false;
	GFilterItemCont filter_cont;
	FilterItemEquip filter_equip(filter_cont);
	auto main_bag_ptr = NewPackage(kIT_MainBag);
	if (!main_bag_ptr)
	{
		assert(false);
		return false;
	}
	auto iter_filtered = main_bag_ptr->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (!iter_filtered)
	{
		assert(false);
		return false;
	}
	GItemStaticContT all_equip;
	for (auto& item : *iter_filtered)
		all_equip.push_back(item);
	if (all_equip.empty())
	{
		LOG_O(Log_trace) << "自动清理出主背包空间之没有找到要清理的装备";
		return false;
	}
	std::sort(all_equip.begin(), all_equip.end(), [](const GItemBasePtr& left, const GItemBasePtr& right){return left->Weight() < right->Weight(); });
	while (!all_equip.empty())
	{
		if (!main_bag_ptr->AdjustPack())
		{
			assert(false);
			return false;
		}
		auto find_pos = main_bag_ptr->FindFreePosBySize(size);
		if (!find_pos.AnyLess(0))
			return true;
		auto it = all_equip.begin();
		auto item = main_bag_ptr->FindByWeightInfo(*it);
		if (!item)
		{
			assert(false);
			all_equip.erase(it);
			continue;
		}
		if (!item->Destroy(true))
		{
			assert(false);
			return false;
		}
		all_equip.erase(it);
		if (!main_bag_ptr->Update())
		{
			assert(false);
			return false;
		}
	}
	return true;
}

bool GInventoryMgr::GetCurEmboitementItem(const GPackStashMgr& stash_mgr, GItemContT& Emboitement_item)
{
	GItemStaticContT all_equip;
	GFilterItemCont filter_cont;
	FilterItemColor filter_color(filter_cont, kIC_Yellow);
	FilterItemNotIdentified filter_not_indentified(filter_cont);
	FilterItemTypeConT filte_type(filter_cont, kITYPE_Helmet, kITYPE_Amulet, kITYPE_Belt, kITYPE_Ring, kITYPE_Boots, kITYPE_Gloves, kITYPE_BodyArmour, kITYPE_TwoHandSword, kITYPE_TwoHandAxe, kITYPE_TwoHandMace);
	if (!stash_mgr.GetItemByFilter(filter_cont, all_equip))
	{
		assert(false);
		return false;
	}
	if (all_equip.empty())
	{
		//assert(false);
		return false;
	}
	struct stHelp
	{
		stHelp(){}
		int MinCntByType(enCD_ItemType type){
			if (kITYPE_Ring == type)
				return 2;
			return 1;
		}
	};
	static stHelp help;
	std::map<enCD_ItemType, std::vector<GItemBasePtr>> calc_cnt;
	for (auto& i : all_equip)
		calc_cnt[ConvertEmboitementType(i->GetItemType())].push_back(i);
	int emboitement_cnt = INT_MAX;
	if (calc_cnt.size() >= kEmboitementCnt - 1)
	{
		int tmp_value = INT_MAX;
		for (auto& v : calc_cnt)
		{
			tmp_value = v.second.size() / help.MinCntByType(v.first);
			emboitement_cnt = std::min(tmp_value, emboitement_cnt);
		}
	}
	//计算需要装备
	InitEmboitement();
	for (auto& k : emboitement_lack_item_)
	{
		auto it = calc_cnt.find(k.item_type);
		if (it != calc_cnt.end())
		{
			auto cnt = help.MinCntByType(k.item_type) * emboitement_Cnt_;
			if ((int)it->second.size() >= cnt)
				continue;
			k.cnt_ = cnt - it->second.size();
		}
		else
			k.cnt_ = help.MinCntByType(k.item_type) * emboitement_Cnt_;
	}
	//计算生成的套装
	if (emboitement_cnt > 0 && emboitement_cnt != INT_MAX)
	{
		for (auto& w : calc_cnt)
		{
			//每次提取一套吧  多套背包空间有可能不足
			auto min_cnt = help.MinCntByType(w.first) * 1;
			if ((int)w.second.size() < min_cnt)
			{
				assert(false);
				return false;
			}
			w.second.erase(w.second.begin() + min_cnt, w.second.end());
		}
		for (auto& k : calc_cnt)
			Emboitement_item.insert(Emboitement_item.end(), k.second.begin(), k.second.end());
	}
	return true;
}

bool GInventoryMgr::CheckStoreEmboitementToBag()
{
	if (!OpenStoreAndUpdate(true))
	{
		assert(false);
		return false;
	}
	auto& stash_mgr = GPackStashMgr::GetMe();
	GItemContT emboitement_item;
	GetCurEmboitementItem(stash_mgr, emboitement_item);
	if (emboitement_item.empty())
		return false;
	auto main_bag = GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
#ifdef _DEBUG
	std::ostringstream ofs;
	ofs << "提取套装物品";
	for (auto& p : emboitement_item)
		ofs << " : " << p->GetItemName();
	LOG_O(Log_debug) << ofs.str();
#endif
	for (auto& i : emboitement_item)
	{
		auto pos = main_bag->FindFreePosBySize(i->GetItemSize());
		if (pos.AnyLess(0))
		{
			LOG_O(Log_error) << "从仓库提取套装之背包没有空位置了";
			assert(false);
			return false;
		}
		auto pack_ptr = i->GetPackage();
		if (!pack_ptr)
		{
			assert(false);
			return false;
		}
		if (!pack_ptr->PressPage())
		{
			assert(false);
			return false;
		}
		if (i->MoveTo(*main_bag, pos) < kMITR_Succeed)
		{
			assert(false);
			return false;
		}
	}
	return true;
}

bool GInventoryMgr::AutoStoreBagItemToStore()
{
	auto& gmp = GPlayerMe::GetMe();
	auto& map = GMap::GetMe();
	auto& resolver = GSP_Resolver::GetMe();
	if (!map.IsCangShenChu() && !resolver.IsTownByMapName(map.GetMapName(), gmp.GetDiffLvl()))
	{
		return false;
	}
	auto main_bag = GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	struct stStashMapPos
	{
		const std::string& GetDstStashIndex(enCD_ItemType item_type) const{
			if (item_type > kITYPE_Invalid && item_type <= kITYPE_UtilityFlaskCritical)
				return kStashPageDefault[1];
			else if (item_type >= kITYPE_ActiveSkillGem && item_type <= kITYPE_SupportSkillGem)
				return kStashPageDefault[3];
			else if (item_type >= kITYPE_Helmet && item_type <= kITYPE_Quiver)
				return kStashPageDefault[1];
			else
				return kStashPageDefault[0];
		}
	};
	static stStashMapPos stash_map;
	auto& stash_mgr = GPackStashMgr::GetMe();
	if (!OpenStoreAndUpdate(false))
	{
		LOG_O(Log_error) << "打开仓库失败";
		assert(false);
		return false;
	}
	
	GItemContT emboitement;
	GetCurEmboitementItem(stash_mgr, emboitement);
	std::map<enCD_ItemType, std::vector<GItemBasePtr>> map_item;
	auto& item = main_bag->GetItems();
	for (auto& i : item)
	{
		if (i->IsQuestItem())
			continue;
		if (!MyLuaThrdData::GetSaveStoreFilter().DoFilt(*i))
			continue;
		map_item[i->GetItemType()].push_back(i);
	}
	if (map_item.empty())
	{
		gmp.CloseAllPanel(false);
		LOG_O(Log_trace) << "自动存仓之没有要存仓的物品";
		return true;
	}
	if (!OpenStoreAndUpdate(true))
	{
		LOG_O(Log_debug) << "存储仓库之 打开仓库失败";
		assert(false);
		return false;
	}
#ifdef  _DEBUG
		std::ostringstream ofs("存仓物品");
		for (auto& i : map_item)
		{
			for (auto& v : i.second)
				ofs << ":" << v->GetItemName();
		}
		LOG_O(Log_trace) << ofs.str();
#endif
	while (!map_item.empty())
	{
		auto it = map_item.begin();
		if (it == map_item.end())
			break;
		if (it->second.empty())
			continue;
		auto dst_index_str = stash_map.GetDstStashIndex(it->first);
		auto dst_page = stash_mgr.FindPackByIndexStr(dst_index_str);
		if (!dst_page)
		{
			assert(false);
			return false;
		}
		while (!it->second.empty())
		{
			auto it_item = it->second.begin();
			if (it_item == it->second.end())
				break;
			if (!dst_page->PressPage())
			{
				assert(false);
				return false;
			}
			if (!dst_page->Update())
			{
				assert(false);
				return false;
			}
			auto pos = dst_page->FindFreePosBySize((*it_item)->GetItemSize());
			if (pos.AnyLess(0))
			{
				dst_page = stash_mgr.FindPackBySize((*it_item)->GetItemSize());
				if (!dst_page)
				{
					LOG_O(Log_trace) << "存储仓库之没有可存储的位置了";
					break;
				}
				continue;
			}
			if ((*it_item)->MoveTo(*dst_page, pos) < kMITR_Succeed)
			{
				assert(false);
				it->second.erase(it_item);
				continue;;
			}
			it->second.erase(it_item);
		}
		map_item.erase(it);
	}
	gmp.CloseAllPanel(true);
	return true;
}

bool GInventoryMgr::AutoSellBagItem(const std::string& npc_name)
{
	auto& gpm = GPlayerMe::GetMe();
	auto& map = GMap::GetMe();
	auto& resolver = GSP_Resolver::GetMe();
	if (!map.IsCangShenChu() && !resolver.IsTownByMapName(map.GetMapName(), gpm.GetDiffLvl()))
	{
		return false;
	}
	struct stSellNpc
	{
		typedef std::map<int, std::string> NpcSell;
		stSellNpc(){
#ifdef VER_GJ3_
			npc_sell_[1] = "Tarkleigh";
			npc_sell_[2] = "Greust";
			npc_sell_[3] = "Hargan";
			npc_sell_[4] = "Kira";
			npc_sell_[5] = "Lani";
			npc_sell_[6] = "Tarkleigh";
			npc_sell_[7] = "Yeena";
			npc_sell_[8] = "Hargan";
			npc_sell_[9] = "Irasha";
			npc_sell_[10] = "Lani";
#elif VER_GJ_
			npc_sell_[1] = "Tarkleigh";
			npc_sell_[2] = "Greust";
			npc_sell_[3] = "Hargan";
			npc_sell_[4] = "Kira";
#else
			npc_sell_[1] = "塔格拉(武器交易)";
			npc_sell_[2] = "古斯特(武器交易)";
			npc_sell_[3] = "贺根(武器交易)";
			npc_sell_[4] = "奇拉(武器交易)";
			npc_sell_[5] = "拉尼";
			npc_sell_[6] = "塔格拉";
			npc_sell_[7] = "伊娜(技能杂货)";
			npc_sell_[8] = "贺根(武器交易)";
			npc_sell_[9] = "伊莎拉";
			npc_sell_[10] = "丽莉罗斯";
			npc_sell_[11] = "拉尼";
#endif
		}
		NpcSell npc_sell_;
	};
	auto sell_npc_name = npc_name;
	if (npc_name.empty())
	{
		static stSellNpc sell_npc;
		auto it = sell_npc.npc_sell_.find(map.GetChapter());
		if (it == sell_npc.npc_sell_.end())
		{
			assert(false);
			return false;
		}
		auto town_name = resolver.GetCurChapterTownName(map.GetChapter());
		if (town_name.empty())
		{
			assert(false);
			return false;
		}
		sell_npc_name = it->second;
	}

	auto main_bag = GetPackBase(kIT_MainBag);
	if (!main_bag)
		return false;
	std::queue<GItemBasePtr> items_tmp;
	auto& item = main_bag->GetItems();
	for (auto& i : item)
	{
		if (!MyLuaThrdData::GetSellItemFilter().DoFilt(*i))
			continue;
		items_tmp.push(i);
	}
	if (items_tmp.empty())
	{
		LOG_O(Log_trace) << "自动出售之没有要出售的物品";
		return true;
	}
#ifdef _DEBUG
	auto tmp_queue = items_tmp;
	std::ostringstream ofs;
	ofs << "出售物品";
	while (!tmp_queue.empty())
	{
		ofs << ":" << tmp_queue.front()->GetItemName();
		tmp_queue.pop();
	}
	LOG_O(Log_trace) << ofs.str();
#endif
	
	if (gpm.MoveToOpenObj(nullptr, true, sell_npc_name) < kOVR_Succeed)
	{
		LOG_O(Log_debug) << "自动卖物品之 打开NPC失败,NPC名：" << sell_npc_name;
		assert(false);
		return false;
	}
	gpm.TalkToNpc(CD_kNTL_CotinueA);
	if (!TimerDo(50, 1000 * 5, [](){
		return GInterface::NpcPanelSubUiShowState(kNPIC_Normal, kTUSF_Shown);
	}))
	{
		LOG_O(Log_error) << "自动卖物品之对话npc没有打开对话面板：" << sell_npc_name;
		gpm.CloseAllPanel(false);
	}
	if (!gpm.TalkToNpc(CD_KNTL_SellA))
	{
		//assert(false);
		LOG_O(Log_error) << "自动卖物品之 对话npc失败，NPC名：" << sell_npc_name;
		return false;
	}
	static GPackSell sell_pack;
	bool is_again_open = false;
	while (!items_tmp.empty())
	{
		if (is_again_open)
		{
			if (gpm.MoveToOpenObj(nullptr, true, sell_npc_name) < kOVR_Succeed)
			{
				LOG_O(Log_debug) << "自动卖物品之 再次打开NPC失败,NPC名：" << sell_npc_name;
				assert(false);
				return false;
			}
			if (!gpm.TalkToNpc(CD_KNTL_SellA))
			{
				LOG_O(Log_error) << "自动卖物品之 再次对话npc失败，NPC名：" << sell_npc_name;
				assert(false);
			}
		}
		if (!sell_pack.RebuildAll())
		{
			assert(false);
			return false;
		}
		auto& the_item = items_tmp.front();
		auto pos = sell_pack.FindMyPosBySize(the_item->GetItemSize());
		if (pos.AnyLess(0))
		{
			if (!sell_pack.PressButton(true))
			{
				assert(false);
				continue;
			}
			is_again_open = true;
			continue;
		}
		is_again_open = false;
		if (the_item->MoveTo(*sell_pack.GetSellMyPack(), pos) < kMITR_Succeed)
		{
			assert(false);
			items_tmp.pop();
			return false;
		}
		items_tmp.pop();
		if (items_tmp.empty())
		{
			if (!sell_pack.PressButton(true))
			{
				assert(false);
				return false;
			}
		}
	}
	gpm.TalkToNpc(CD_kNTL_ByeA);
	return true;
}

int GInventoryMgr::BuyItemByNpc(const std::string& npc_name, const std::string& item_name, int cnt)
{
	int ret_cnt = 0;
	auto& gsp_resolver = GSP_Resolver::GetMe();
	auto& map = GMap::GetMe();
	auto& gpm = GPlayerMe::GetMe();
	if (!gpm.Update())
	{
		assert(false);
		return ret_cnt;
	}
	if (!gsp_resolver.IsTownByMapName(map.GetMapName(), gpm.GetDiffLvl()))
	{
		LOG_O(Log_error) << "该地图不是城镇地图 不能购买物品:" << map.GetMapName();
		return ret_cnt;
	}
	if (gpm.MoveToOpenObj(nullptr, true, npc_name) < kOVR_Succeed)
	{
		LOG_O(Log_error) << "购买物品之没有找到该npc:" << npc_name;
		assert(false);
		return ret_cnt;
	}
	if (!gpm.TalkToNpc(CD_KNTL_BuyA))
	{
		assert(false);
		return ret_cnt;
	}
	static GPackBuyMgr npc_buy_mgr;
	if (!npc_buy_mgr.UpdateAllPackPage())
	{
		assert(false);
		return ret_cnt;
	}

	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return ret_cnt;
	}
	struct stHelp
	{
		std::string item_name_;
		pt_dword	item_cnt_;
		bool		bind_state_;
	};
	std::vector<stHelp> bag_item;
	GFilterItemCont filter_cont;
	for (int i = 0; i < cnt; ++i)
	{
		bool enough_item = false;
		auto item = npc_buy_mgr.FindItemByAllPack(item_name);
		if (!item)
		{
			LOG_O(Log_error) << "购买物品之没有找到该物品:" << item_name;
			break;
		}
		auto item_pack = item->GetPackage();
		if (!item_pack)
		{
			assert(false);
			break;
		}
		if (!item_pack->PressPage())
		{
			assert(false);
			break; 
		}
		auto& item_price = item->GetPrice();
		if (item_price.empty())
		{
			assert(false);
			break;
		}
		if (!main_bag->Update())
		{
			assert(false);
			return ret_cnt;
		}
		auto pos = main_bag->FindFreePosBySize(item->GetItemSize());
		if (pos.AnyLess(0))
		{
			LOG_O(Log_error) << "背包没有充足的空间购买物品:" << item_name;
			break;
		}	
#ifdef VER_GJ_
		int tmp_loop = 1;
#else
		int tmp_loop = 2;
#endif
		for (int k = 0; k < tmp_loop; ++k)
		{
			bool bind_state = !!k; 
			filter_cont.clear();
			FilterItemBind filter_bind(filter_cont, bind_state);
			for (auto& v : item_price)
			{
				if (!v.attr_db_item_base_)
				{
					assert(false);
					break;
				}
				auto price_name = v.attr_db_item_base_->name_.GetStdString();
				auto it = std::find_if(bag_item.begin(), bag_item.end(), [&price_name, bind_state](const stHelp& item_info){return item_info.item_name_ == price_name && bind_state == item_info.bind_state_; });
				if (it == bag_item.end())
				{
					stHelp tmp;
					tmp.bind_state_ = bind_state;
					tmp.item_name_ = price_name;
					FilterItemName filter_name(filter_cont, price_name);
					tmp.item_cnt_ = GetStorAndMainBagFilterItemCnt(filter_cont);
					bag_item.push_back(tmp);
					filter_cont.Erase(filter_name);
				}
				auto it_info = std::find_if(bag_item.begin(), bag_item.end(), [&price_name, bind_state](const stHelp& item_info){return item_info.item_name_ == price_name && bind_state == item_info.bind_state_; });
				if (it_info == bag_item.end())
				{
					assert(false);
					return ret_cnt;
				}
				if (it_info->item_cnt_ >= v.item_cnt_)
				{
					it_info->item_cnt_ -= v.item_cnt_;
					enough_item = true;
					continue;
				}
				enough_item = false;;
			}
			if (!enough_item)
				continue;
#ifndef VER_GJ_
			if (!npc_buy_mgr.EnableUseBind(bind_state))
			{
				assert(false);
				return ret_cnt;
			}
#endif
			break;
		}
		if (!enough_item)
		{
			LOG_O(Log_error) << "购买物品之材料不足:" << item_name;
			break;
		}
		if (!item->MoveTo(*main_bag, pos))
		{
			assert(false);
			break;
		}
		++ret_cnt;
		if (!main_bag->Update())
		{
			assert(false);
			break;
		}
		if (!npc_buy_mgr.UpdateAllPackPage())
		{
			assert(false);
			break;
		}
	}
	gpm.CloseAllPanel(true);
	gpm.TalkToNpc(CD_kNTL_ByeA);
	return ret_cnt;
}

void GInventoryMgr::GetEquipInvalidGem(GemEquipConT& invalid_gem) const
{
	PackagesContT packages;
	if (!GetEquipPackages(packages, false))
	{
		assert(false);
		return;
	}
	for (auto& i : packages)
	{
		auto equip = i->GetFirstItem();
		if (!equip)
			continue;
		equip->GetInvalidGem(invalid_gem);
	}
	return;
}

bool GInventoryMgr::AutoTakeOfInvalidGem()
{
	GemEquipConT invalid_gem;
	GetEquipInvalidGem(invalid_gem);
	if (invalid_gem.empty())
		return true;
	auto main_bag = GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	for (auto& i : invalid_gem)
	{
		auto equip_bag = GetPackBase(i.first);
		if (!equip_bag)
		{
			assert(false);
			return false;
		}
		auto equip_item = equip_bag->GetFirstItem();
		if (!equip_item)
		{
			assert(false);
			return false;
		}
		for (auto& v : i.second)
		{
			if (equip_item->TakeoffGemTo(v, *main_bag, kInvalidVecInt) >= kTOGR_Succeed)
				continue;
			return false;
		}
	}
	return true;
}

int GInventoryMgr::TakeOutStorageItem(const GFilterItemCont& filter_cont, int item_cnt)
{
	auto tmp_cnt = 0;
	if (item_cnt <= 0)
	{
		assert(false);
		return tmp_cnt;
	}
	auto main_bag = GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return tmp_cnt;
	}
	if (main_bag->FindFreePosBySize(stCD_VecInt(1,1)).AnyLess(0))
	{
		LOG_O(Log_error) << "背包没有空位置了";
		return false;
	}
	if (!OpenStoreAndUpdate(true))
	{
		assert(false);
		return false;
	}
	auto& gmp = GPlayerMe::GetMe();
	auto& stash_mgr = GPackStashMgr::GetMe();
	GItemStaticContT item_all;
	for (auto& i : kStashPageDefault)
	{
		auto dst_page = stash_mgr.FindPackByIndexStr(i);
		if (!dst_page)
		{
			assert(false);
			return tmp_cnt;
		}
		if (!dst_page->PressPage())
		{
			assert(false);
			return tmp_cnt;
		}
		auto iter_filter = dst_page->FilterIterator<IterContTag_ByRef>(filter_cont);
		if (!iter_filter)
			continue;
		item_all.clear();
		for (auto& item : *iter_filter)
			item_all.push_back(item);
		if (item_all.empty())
			continue;
		for (auto& i : item_all)
		{
			if (!i)
				continue;
			if (!main_bag->Update())
			{
				assert(false);
				return tmp_cnt;
			}
			auto pos = main_bag->FindFreePosBySize(i->GetItemSize());
			if (pos.AnyLess(0))
			{
				gmp.CloseAllPanel(true);
				return tmp_cnt;
			}
			if (i->MoveTo(*main_bag, pos) < kMITR_Succeed)
			{
				assert(false);
				gmp.CloseAllPanel(true);
				return true;
			}
			tmp_cnt += i->GetItemCnt();
			if (tmp_cnt >= item_cnt)
			{
				gmp.CloseAllPanel(true);
				return tmp_cnt;
			}
		}
	}
	gmp.CloseAllPanel(true);
	return tmp_cnt;
}

void GInventoryMgr::SetEmboitementCnt(int cnt)
{
	if (cnt < 0)
	{
		assert(false);
		return;
	}
	emboitement_Cnt_ = cnt;
}

enCD_ItemType GInventoryMgr::ConvertEmboitementType(enCD_ItemType type)
{
	if (type >= kITYPE_TwoHandSword && type <= kITYPE_TwoHandMace)
		return kITYPE_TwoHandSword;
	return type;

}

void GInventoryMgr::InitEmboitement()
{
	emboitement_lack_item_.clear();
	GClass::stEmboitementInfo tmp;
	for (auto& i : kEmboitementType)
	{
		tmp.item_color_ = kIC_Yellow;
		tmp.item_type = i;
		tmp.cnt_ = 0;
		emboitement_lack_item_.push_back(tmp);
	}
}

bool GInventoryMgr::OpenStoreAndUpdate(bool reopen) const
{
	return GPackStashMgr::GetMe().OpenStoreAndUpdate(reopen);
}

bool GInventoryMgr::CheckStoreGemTotalQualityToBag(pt_dword total_value)
{
	if (!OpenStoreAndUpdate(false))
	{
		assert(false);
		return false;
	}
	auto& stash_mgr = GPackStashMgr::GetMe();
	GItemStaticContT all_gem;
	GFilterItemCont filter_cont;
	FilterItemTypeConT filte_type(filter_cont, kITYPE_ActiveSkillGem, kITYPE_SupportSkillGem);
	FilterItemQualityRegion filter_quality(filter_cont, 1, INT_MAX);
	if (!stash_mgr.GetItemByFilter(filter_cont, all_gem))
	{
		assert(false);
		return false;
	}
	if (all_gem.empty())
		return false;
	pt_dword Total_qual = 0;
	for (auto& i : all_gem)
		Total_qual += i->GetQuality();
	if (Total_qual < total_value)
		return false;
	return TakeOutStorageItem(filter_cont, total_value) != 0;
}

pt_dword GInventoryMgr::GetStorAndMainBagFilterItemCnt(const GFilterItemCont& filter_cont) const
{
	GItemStaticContT all_item;
	GPackStashMgr::GetMe().GetItemByFilter(filter_cont, all_item);
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	auto it_filter = main_bag->FilterIterator<IterContTag_ByRef>(filter_cont);
	if (!it_filter)
	{
		assert(false);
		return 0;
	}
	for (auto& i : *it_filter)
		all_item.push_back(i);
	pt_dword cnt = 0;
	for (auto& v : all_item)
		cnt += v->GetItemCnt();
	return cnt;
}

GPackageIteratorPtr GInventoryMgr::BagInventories()
{
	return CreateIterator(kIT_MainBag, kIT_Flasks);
}

GChecksumType GInventoryMgr::BagInventoriesChecksum()
{
	auto iter = BagInventories();
	if (!iter)
	{
		assert(false);
		return 0;
	}
	GChecksumType sum = 0;
	for (auto& package : *iter)
	{
		sum += package->CheckSum();
	}
	auto attach_item = GInterface::GetMouseAttachItem();
	if (attach_item)
		sum += attach_item->CheckSum(kDummyChecksumBagWidth);
	return sum;
}

GDBBaseItemTypeMgr::GDBBaseItemTypeMgr()
{

}

bool GDBBaseItemTypeMgr::RebuildAllImpl()
{
	auto itemDb = CD_Interface::GetDbMgr().GetDbBaseItemTypes();
	if (IsUnValidPtr(itemDb))
	{
		assert(false);
		return false;
	}

	auto tab = itemDb->GetDbTbl();
	if (IsUnValidPtr(tab))
	{
		assert(false);
		return false;
	}

	if (!tab->tbl_items_.IsValid())
	{
		assert(false);
		return false;
	}

	GDbBaseItemType tmpDb;
	for (auto& i : tab->tbl_items_)
	{
		tmpDb.itemDb_ = &i;
		if (!tmpDb.UpdateImpl())
		{
			assert(false);
			return false;
		}
		item_db_.push_back(tmpDb);
	}

	return true;
}

void GDBBaseItemTypeMgr::DebugInfo() const
{
	if (item_db_.empty())
		return;

	for (auto& i : item_db_)
		i.DebugInfo();
}

bool GDBBaseItemTypeMgr::RebuildAll()
{
	if (!item_db_.empty())
		return true;
	return __super::RebuildAll();
}

GDbBaseItemType::GDbBaseItemType()
{
	itemDb_ = nullptr;
}

bool GDbBaseItemType::UpdateImpl()
{
	if (IsUnValidPtr(itemDb_))
	{
		assert(false);
		return false;
	}

	item_name_.assign(CD_SafeName(itemDb_->item_name_));

	return true;
}

void GDbBaseItemType::DebugInfo() const
{
	LOG_O(Log_debug) << "item_name-" << item_name_;
}

GWorldItem::GWorldItem(enGameObjType obj_type) : GameObjBase(obj_type)
{
	entity_world_item_ = nullptr;
}

bool GWorldItem::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		//assert(false);
		return false;
	}

	auto entity = GetGameEntityBase();
	if (!entity)
	{
		assert(false);
		return false;
	}
	if (!entity->IsValid())
	{
		assert(false);
		return false;
	}

	auto entity_world_item =  entity->CastToWorldItem();
	if (!entity_world_item)
	{
		assert(false);
		return false;
	}
	auto world_item = entity_world_item->GetWorldItem();
	if (!world_item)
	{
		assert(false);
		return false;
	}
	auto item = world_item->GetItemEntity();
	if (!item || !item->IsValid())
	{
		assert(false);
		return false;
	}
	auto base_item = item->GetBaseItemType();
	if (!base_item)
	{
		assert(false);
		return false;
	}
	auto item_base =  item->GetAttrDbBase();
	if (!item_base)
	{
		assert(false);
		return false;
	}

	if (!CD_SafeName(item_base->name_))
	{
		assert(false);
		return false;
	}

	SetName(item_base->name_.GetStdString());
	item_ = GItemMgrBase().CreateItem(*item, entity->id_, nullptr, nullptr);
	if (!item_)
	{
		assert(false);
		return false;
	}
	if (!item_->Update())
	{
		assert(false);
		return false;
	}
	return true;
}

void GWorldItem::DebugInfo() const
{
	LOG_O(Log_debug) << "掉落物----------------";
	__super::DebugInfo();
	item_->DebugInfo();
}

bool GWorldItem::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	return visitor.VisitWorldItem(*this, is_pre_open, open_res);
}

const GItemBasePtr& GWorldItem::GetWorldItem() const
{
	return item_;
}

int GWorldItem::GetCanOpenedDistance() const
{
	return kMovePickupItemDistance;
}

bool GWorldItem::DoFilt() const
{
	if (item_)
		return item_->IsQuestItem();
	return false;
}

bool GWorldItem::CanBreakOpenObj() const
{
	if (!item_)
		return true;
	if (item_->IsEquip() || item_->IsGem())
		return false;
	return true;
}

bool GWorldItem::MovingDoTryOpen() const
{
	return __super::MovingDoTryOpen();
	//判断是否有重复宝石
	//if (!item_)
	//{
	//	assert(false);
	//	return false;
	//}
	//if (!item_->IsGem())
	//	return true;
	//static GItemContT gem_vec(500);
	//gem_vec.clear();
	//GInventoryMgr::GetMe().GetAllPackGem(gem_vec);
	//if (gem_vec.empty())
	//	return true;
	//auto cur_gem = item_->GetItemName();
	//auto it = std::find_if(gem_vec.begin(), gem_vec.end(), [&cur_gem](const GItemBasePtr& gem_item){return gem_item->GetItemName() == cur_gem; });
	//return it == gem_vec.end();
}

GChest::GChest(enGameObjType obj_type) : GameObjBase(obj_type)
{
	entity_chest_ = nullptr;
	is_opened_ = false;
	is_quest_ = false;
}

void GChest::DebugInfo() const
{
	LOG_O(Log_debug) << "箱子 -------------";
	__super::DebugInfo();
	assert(entity_chest_);
	entity_chest_->DebugInfo();
}

bool GChest::UpdateImpl()
{
	if (!__super::UpdateImpl())
	{
		//assert(false);
		return false;
	}

	auto entity = GetGameEntityBase();
	if (!entity)
	{
		assert(false);
		return false;
	}
	if (!entity->IsValid())
	{
		assert(false);
		return false;
	}

	entity_chest_ = entity->CastTo<stCD_EntityChest>();
	if (!entity_chest_)
	{
		assert(false);
		return false;
	}
	auto chest_attr = entity_chest_->GetChest();
	if (!chest_attr)
	{
		assert(false);
		return false;
	}
	if (chest_attr->is_opened_)
		return false;
	is_opened_ = chest_attr->is_opened_;

	auto attr_db = chest_attr->GetDbChest();
	if (!attr_db)
	{
		assert(false);
		return false;
	}
	if (attr_db->IsIgnore())
	{
		if (GPlayerMe::GetMe().auto_open_chest_ & kOCF_OnlyShowName)
			return false;
	}
	is_quest_ = attr_db->IsQuest();

	auto game_render = entity_chest_->GetRender();
	if (IsUnValidPtr(game_render))
	{
		assert(false);
		return false;
	}
	if (!CD_SafeName(game_render->name_))
	{
		assert(false);
		return false;
	}

	SetName(game_render->name_.GetStdString());
	return true;
}

bool GChest::OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res)
{
	return visitor.VisitChestItem(*this, is_pre_open, open_res);
}

int GChest::GetCanOpenedDistance() const
{
	if (is_quest_)
		return kOpenObjValidDistance;
	return kMoveOpenChestDistance;
}

bool GAwardItemPack::UpdateImpl()
{
	auto main_ui = CD_Interface::GetInGameMainUi();
	if (!main_ui)
	{
		LOG_O(Log_debug) << "领取奖励 更新出错GetInGameMainUi";
		//assert(false);
		return false;
	}
	auto dlg = main_ui->GetDlgQuestAward();
	if (!dlg)
	{
		LOG_O(Log_debug) << "领取奖励 更新出错GetDlgQuestAward";
		//assert(false);
		return false;
	}

	auto award_bag = dlg->GetInventory();
	if (!award_bag)
	{
		LOG_O(Log_debug) << "领取奖励 更新出错GetInventory";
		//assert(false);
		return false;
	}
	if (!Init(dlg->GetGridPanel(), award_bag))
	{
		assert(false);
		return false;
	}
	return __super::UpdateImpl();
}

bool GAwardItemPack::IsOpenAwardPanel() const
{
	if (!GInterface::MainSubUiShowState((pt_pvoid)CD_EXE_DATA(g_va_ui_dlg_quest_award_), kTUSF_Shown))
	{
		LOG_O(Log_error) << "领取奖励之NPC奖励面板没有打开";
		return false;
	}
	return true;
}

bool GAwardItemPack::NpcItemToMainBagAndWear(const GItemBase& item) const
{
	auto& inventory = GInventoryMgr::GetMe();
	auto main_bag = inventory.GetPackBase(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	main_bag->AdjustPack();
	auto item_pos = main_bag->FindFreePosBySize(item.GetItemSize());
	if (item_pos.AllLessZero())
	{
		if (!GInventoryMgr::GetMe().AutoClearMainBagBySize(item.GetItemSize()))
		{
			LOG_O(Log_debug) << "获取物品之背包没有空位置了,物品名：" << item.GetItemName();
			assert(false);
			return false;
		}
		item_pos = main_bag->FindFreePosBySize(item.GetItemSize());
		if (item_pos.AllLessZero())
		{
			assert(false);
			return false;
		}
	}
	/*auto& gmp = GPlayerMe::GetMe();
	if (!gmp.NpcItemToInventory(item, item_pos, kIT_MainBag))*/
	if (item.MoveTo(*main_bag, item_pos) < kMITR_Succeed)
	{
		assert(false);
		return false;
	}
	if (!main_bag->Update())
	{
		assert(false);
		return false;
	}
	auto& item_name = item.GetItemName();
	if (CanAutoUse(item_name))
	{
		if (!GInventoryMgr::GetMe().UseItem(item_name))
		{
			LOG_O(Log_error) << "自动使用奖励物品失败:" << item.GetItemName();
			assert(false);
			return false;
		}
	}

	if (item.IsEquip())
		MyLuaThrdData::AutoWearSomeEquips();
	else if (item.IsGem())
		MyLuaThrdData::AutoWearSomeGem();
	return true;
}

GItemBasePtr GAwardItemPack::FindItemOrDefault(const std::string& item_name) const
{
	if (GetItemCnt() <= 0)
	{
		assert(false);
		return nullptr;
	}
	auto item = FindItemByName(item_name);
	if (!item)
	{
		LOG_O(Log_debug) << "获取物品之 没有该物品,物品名:" << item_name << ",默认领取第一个";
		return GetFirstItem();
	}
	return item;
}
bool GAwardItemPack::CanAutoUse(const std::string& item_name) const
{
	if (item_name == kObjName_RegretBook)
		return true;
	if (item_name == kObjName_SkillBook)
		return true;
	return false;
}

GItemBasePtr GAwardItemPack::FindItemOrDefault(int index) const
{
	auto cnt = GetItemCnt();
	if (cnt <= 0)
	{
		assert(false);
		return nullptr;
	}
	--index;
	index = std::abs(index);
	index = std::min((pt_dword)index, cnt-1);
	auto item = FindItemByIndex(index);
	if (!item)
	{
		LOG_O(Log_debug) << "获取物品之 没有该索引位置物品,索引:" << index + 1 << ",默认选取第一个";
		return GetFirstItem();
	}
	return item;
}

GItemRing::GItemRing() : GItemWearable(kICATE_Ring)
{

}

enMoveItemToRes GItemRing::MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return MoveToImpl(this, dst, pos);
}

enPutItemDownRes GItemRing::PutDownTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return dst.PutDownToHere(*this, pos);
}

GPackageBasePtr GItemRing::GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const
{
	auto packages = positions;
	if (packages.empty())
	{
		PackagesContT packages_tmp;
		GPackageMgrBase().NewPackage(packages_tmp, kIT_RingLeft, kIT_RingRight);
		if (packages_tmp.empty())
		{
			assert(false);
			return nullptr;
		}
		for (auto& v : packages_tmp)
			packages.push_back(v);
	}	
	CmnStaticVector<GItemBasePtr, 2> ring_vc;
	for (auto& i : packages)
	{
		auto ring_item = i->GetFirstItem();
		if (ring_item)
			ring_vc.push_back(ring_item);
	}
	if (ring_vc.empty())
		return packages[0];

	std::sort(ring_vc.begin(), ring_vc.end(), [](const GItemBasePtr& left, const GItemBasePtr& right){return left->Weight() < right->Weight(); });
	auto my_value = Weight();
	for (auto& v : ring_vc)
	{
		if (my_value > v->Weight())
		{
			auto it = std::find_if(packages.begin(), packages.end(), [v](const GPackageBasePtr& pack_tmp){ return v->GetInventoryType() == pack_tmp->GetPackageType(); });
			if (it == packages.end())
			{
				assert(false);
				return nullptr;
			}
			return *it;
		}
	}
	return nullptr;
}

bool GItemRing::IsEquip() const
{
	return true;
}

void GItemRing::PushGoodToTheResult(GItemStaticContT& out_items)
{
	auto type = GetItemType();
	CmnStaticVector<GItemStaticContT::iterator, 2> rings;
	for (auto it = out_items.begin(), it_end = out_items.end(); it != it_end; ++it)
	{
		auto& item = *it;
		if (item->GetItemType() == type)
		{
			rings.push_back(it);
			if (rings.is_full())
				break;
		}
	}
	if (rings.size() < 2)
	{
		out_items.push_back(shared_from_this());
		return;
	}
	auto my_weight = Weight();
	for (auto& ring : rings)
	{
		auto& the_ring = *ring;
		if (the_ring->Weight() < my_weight)
		{
			the_ring = shared_from_this();
			break;
		}
	}
}

GPackageBasePtr GItemWeapon::GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const
{
	auto package_types = GEquipTypeMgr::GetMe().GetPositions(*this);
	for (auto pack_type : package_types)
	{
		GPackageBasePtr package;
		auto it = std::find_if(positions.begin(), positions.end(), [pack_type](const GPackageBasePtr& pack){ return pack->GetPackageType() == pack_type; });
		if (it == positions.end())
			package = GPackageMgrBase().NewPackage(pack_type);
		else
			package = *it;
		if (!package)
		{
			assert(false);
			return nullptr;
		}
		if (package->GoodThanAll(*this))
			return package;
	}
	return nullptr;
}

GItemWeapon::GItemWeapon(enItemCate item_cate) : GItemEquipBase(item_cate)
{

}

GItemWeapon::GItemWeapon() : GItemWeapon(kICATE_Weapon)
{
}

enMoveItemToRes GItemWeapon::MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const
{
	return MoveToImpl(this, dst, pos);
}

int GItemWeapon::WeightImpl() const
{
	auto my_weight = __super::WeightImpl();
	my_weight += GEquipScore::GetMe().CalcItemGoodValue(*this);
	//struct stHelpWeight{
	//	stHelpWeight(){
	//		for (auto& v : type2cate_)
	//			v = kWCC_Invalid;
	//		type2cate_[kITYPE_Bow] = kWCC_OneHand;
	//		type2cate_[kITYPE_Quiver] = kWCC_OneHand;
	//		type2cate_[kITYPE_Shield] = kWCC_OneHand;
	//		type2cate_[kITYPE_Wand] = kWCC_OneHand;
	//		//////////////////////////////////////////////////////////////////////////
	//		for (auto& v : cate_weights_)
	//			v = 0;
	//		cate_weights_[kWCC_OneHand] = 1000;
	//		cate_weights_[kWCC_TwoHand] = 1200;
	//	}
	//	enWeaponCompositeCate	type2cate_[kITYPE_Max];
	//	pt_int					cate_weights_[kWCC_Max];
	//};
	//static stHelpWeight helper;
	//auto my_type = GetItemType();

	//if (my_type >= kITYPE_Max)
	//{
	//	assert(false);
	//	return my_weight;
	//}
	//auto my_cate = helper.type2cate_[my_type];
	//if (my_cate == kWCC_Invalid)
	//	my_cate = GEquipTypeMgr::GetMe().GetWeaponCompositeCate(*this);
	//my_weight += helper.cate_weights_[my_cate];

	//auto cfg_cate = MyLuaThrdData::GetWearWeaponType();
	//if (cfg_cate != kWCC_Invalid && cfg_cate == my_cate)
	//	my_weight += 1000;
	return my_weight;
}

void GItemWeapon::PrintWeight() const
{
	GEquipScore::GetMe().PrintItemWeightInfo(*this);
	__super::PrintWeight();
}

GPackageLeft::GPackageLeft() : GPackageWeaponBase(kIT_WeaponLeft)
{

}

enMoveItemToRes GPackageLeft::MoveItToHereSpecific(const GItemWeapon& item, const stCD_VecInt& pos)
{
	auto the_right_package = GPackageMgrBase().NewPackage(kIT_WeaponRight);
	if (!the_right_package)
	{
		assert(false);
		return kMITR_Failed;
	}
	auto right_item = the_right_package->GetFirstItem();
	if (right_item)
	{
		auto& type_mgr = GEquipTypeMgr::GetMe();
		if (!type_mgr.CanWearTheEquips(item, *right_item))
		{
			auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
			if (!main_bag)
			{
				assert(false);
				return kMITR_Failed;
			}
			LOG_O(Log_debug) << "左手武器之先脱掉右手装备:" << right_item->GetItemName();
			auto res = right_item->MoveTo(*main_bag, kInvalidVecInt);
			assert(res >= kPIDR_Succeed);
			return res;
		}
	}
	return kMITR_Succeed;
}

enCD_InventoryType GPackageLeft::SwitchPosition() const
{
	return kIT_WeaponRight;
}

GPackageRight::GPackageRight() : GPackageWeaponBase(kIT_WeaponRight)
{

}

enCD_InventoryType GPackageRight::SwitchPosition() const
{
	return kIT_WeaponLeft;
}

GPackageWeaponBase::GPackageWeaponBase(enCD_InventoryType inventory_type) : GPackageEquips(inventory_type)
{

}

enMoveItemToRes GPackageWeaponBase::NotifyMoveItToHereFailed(GPackageBase& src_package, const GItemBase& item, const stCD_VecInt& dst_pos)
{
	auto first_item = GetFirstItem();
	if (first_item)
	{
		auto my_item_size = first_item->GetItemSize();
		if (my_item_size > item.GetItemSize())
		{
			if (src_package.FindFreePosBySize(my_item_size).AnyLess(0))
				return kMITR_Fulled;
		}
	}
	return __super::NotifyMoveItToHereFailed(src_package, item, dst_pos);
}

enMoveItemToRes GPackageWeaponBase::MoveItToHereSpecific(const GItemWeapon& item, const stCD_VecInt& pos)
{
	return kMITR_Succeed;
}

enMoveItemToRes GPackageWeaponBase::MoveItToHere(const GItemWeapon& item, const stCD_VecInt& pos)
{
	enMoveItemToRes res;
	if (SwitchPosition() == item.GetInventoryType())
	{
		auto pos_tmp = pos;
		if (pos_tmp.AnyLess(0))
			pos_tmp.assign(0, 0);
		res = MoveItToHereDirect(item, pos_tmp);
		if (res < kMITR_Succeed)
		{
			assert(false);
			return res;
		}
		auto attached_item = GInterface::GetMouseAttachItem();
		if (!attached_item)
		{
			assert(false);
			return kMITR_Failed;
		}
		auto& type_mgr = GEquipTypeMgr::GetMe();
		enCD_InventoryType dst_inventory;
		if (type_mgr.CanWearTheEquip(type_mgr.GetSwitchItemTypes(item, GetPackageType()), *attached_item, nullptr))
			dst_inventory = SwitchPosition();
		else
			dst_inventory = kIT_MainBag;
		auto package = GPackageMgrBase().NewPackage(dst_inventory);
		if (!package)
		{
			assert(false);
			return kMITR_Failed;
		}
		if (attached_item->PutDownTo(*package, pos_tmp) < kPIDR_Succeed)
		{
			assert(false);
			return kMITR_Failed;
		}
		return kMITR_Succeed;
	}
	res = MoveItToHereSpecific(item, pos);
	if (res < kMITR_Succeed)
		return res;
	return __super::MoveItToHere(item, pos);
}
///////////////////////////////////////////////////////////////////////////////////////////
bool GPackBuyMgr::RebuildAllImpl()
{
	auto main_ui = CD_Interface::GetInGameMainUi();
	if (!main_ui)
	{
		LOG_O(Log_error) << "购买面板 更新出错GetInGameMainUi";
		assert(false);
		return false;
	}
	auto dlg = main_ui->GetDlgBuy();
	if (!dlg)
	{
		LOG_O(Log_error) << "买物品面板没有打开吧";
		assert(false);
		return nullptr;
	}
	auto tab_ui = dlg->GetTabPages();
	if (!tab_ui)
	{
		assert(false);
		return false;
	}
#ifdef VER_GF_
	auto check_box = dlg->GetBindCheckBox();
	if (!check_box)
	{
		assert(false);
		return false;
	}
	cur_enable_ = check_box->enable_;
#endif
	game_tab_pages_ = tab_ui;
	return __super::RebuildAllImpl();
}

GPackBuyMgr::GPackBuyMgr()
{
#ifdef VER_GF_
	cur_enable_ = false;
#endif
}

stCD_UiGridPanel* GPackBuyMgr::GetGridPanel(const stCD_TabPageInfo* page_info) const
{
	assert(page_info);
	return page_info->GetGridPanel<stCD_UiBuyTabPageItem>();
}

bool GPackBuyMgr::IsThePageShown(const stCD_TabPageInfo* page_info) const
{
	assert(page_info);
	return !!page_info->GetGridPanel<stCD_UiBuyTabPageItem>();
}

bool GPackBuyMgr::EnableUseBind(bool enable) const
{
#ifdef VER_GF_
	return GWndExecSync([enable](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			LOG_O(Log_error) << "购买物品面板 更新出错";
			assert(false);
		}
		auto dlg = main_ui->GetDlgBuy();
		if (!dlg)
		{
			LOG_O(Log_error) << "购买物品面板没有打开吧";
			assert(false);
			return false;
		}
		return dlg->EnableUseBind(enable);
	});
#endif
	return true;
}

bool GSwitchPagesMgrBase::ValidPageItem(const stCD_UiTabPages::PageInfo* page_info) const
{
	return true;
}

bool GPackStashMgr::RebuildAllImpl()
{
	auto main_ui = CD_Interface::GetInGameMainUi();
	if (!main_ui)
	{
		LOG_O(Log_error) << "仓库面板 更新出错";
		assert(false);
		return false;
	}
	auto dlg = main_ui->GetDlgStash();
	if (!dlg)
	{
		LOG_O(Log_error) << "仓库面板没有打开吧";
		assert(false);
		return nullptr;
	}
	auto tab_ui = dlg->GetTabPages();
	if (!tab_ui)
	{
		assert(false);
		return false;
	}
	game_tab_pages_ = tab_ui;
	return __super::RebuildAllImpl();
}

GPackStashMgr::GPackStashMgr()
{

}

stCD_UiGridPanel* GPackStashMgr::GetGridPanel(const stCD_TabPageInfo* page_info) const
{
	assert(page_info);
	return page_info->GetGridPanel<stCD_UiStashTabPageItem>();
}

bool GPackStashMgr::IsThePageShown(const stCD_TabPageInfo* page_info) const
{
	auto page_item = page_info->GetPageItemT<stCD_UiStashTabPageItem>();
	if (!page_item)
		return false;
	if (page_item->GetSingleContainer())
		return true;
	auto multi = page_item->GetMultiContainer(kTUSF_Shown);
	if (multi && multi->FindGridPanel(0))
		return true;
	return false;
}

bool GPackStashMgr::ValidPageItem(const stCD_UiTabPages::PageInfo* page_info) const
{
	return GWndExecSync([page_info](){
		if (IsUnValidPtr(page_info))
		{
			assert(false);
			return false;
		}
		auto page_item = page_info->GetPageItemT<stCD_UiStashTabPageItem>();
		if (!page_item)
			return false;
		if (page_item->GetMultiContainer(kTUSF_ShowHide))
			return false;
		return true;
	});	
}

bool GPackStashMgr::CheckStashPanelOpenState() const
{
	return GWndExecSync([](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			LOG_O(Log_error) << "仓库面板 更新出错";
			assert(false);
			return false;
		}
		auto dlg = main_ui->GetDlgStash();
		if (!dlg)
			return false;
		return !!dlg->GetTabPages();
	});
}

bool GPackStashMgr::OpenStoreAndUpdate(bool reopen)
{
	if (!pack_tab_page_.empty() && !reopen)
		return true;
	auto& gpm = GPlayerMe::GetMe();
	auto& map = GMap::GetMe();
	if (!map.IsCangShenChu() && !GSP_Resolver::GetMe().IsTownByMapName(map.GetMapName(), gpm.GetDiffLvl()))
	{
		return false;
	}
	//if (!CheckStashPanelOpenState())
	{
		if (gpm.MoveToOpenObj(nullptr, true, kObjName_Storage) < kOVR_Succeed)
		{
			assert(false);
			return false;
		}
	}
	if (!UpdateAllPackPage())
	{
		gpm.CloseAllPanel(true);
		assert(false);
		return false;
	}
	return true;
}

GPackSell::GPackSell()
{
}

bool GPackSell::RebuildAllImpl()
{
	auto main_ui = CD_Interface::GetInGameMainUi();
	if (!main_ui)
	{
		LOG_O(Log_error) << "卖物品面板 更新出错GetInGameMainUi";
		assert(false);
		return false;
	}
	auto dlg = main_ui->GetDlgSell();
	if (!dlg)
	{
		LOG_O(Log_error) << "卖物品面板没有打开吧";
		assert(false);
		return nullptr;
	}
	auto tab_container = dlg->GetTabPagesContainer();
	if (!tab_container)
	{
		LOG_O(Log_error) << "卖物品面板更新出错GetTabPages";
		assert(false);
		return false;
	}
	auto my_page = tab_container->GetMyItemPage();
	if (my_page)
	{
		auto my_inventory = my_page->GetInventory();
		if (!my_inventory)
		{
			assert(false);
			return false;
		}
		my_pack_item_.reset(new GServiceSellPack());
		if (!my_pack_item_)
		{
			assert(false);
			return false;
		}
		if (!my_pack_item_->Init(my_page, my_inventory))
		{
			assert(false);
			return false;
		}
		if (!my_pack_item_->Update())
		{
			assert(false);
			return false;
		}
	}
	auto npc_page = tab_container->GetNpcItemPage();
	if (npc_page)
	{
		auto npc_inventory = npc_page->GetInventory();
		if (!npc_inventory)
		{
			assert(false);
			return false;
		}
		npc_pack_item_.reset(new GServiceSellPack());
		if (!npc_pack_item_)
		{
			assert(false);
			return false;
		}
		if (!npc_pack_item_->Init(npc_page, npc_inventory))
		{
			assert(false);
			return false;
		}
		if (!npc_pack_item_->Update())
		{
			assert(false);
			return false;
		}
	}
	return true;
}


bool GPackSell::PressButton(bool accept)
{
	if (!GWndExecSync([accept, this](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			LOG_O(Log_error) << "卖物品出售面板出错";
			assert(false);
			return false;
		}
		auto ui_sell_dlg = main_ui->GetDlgSell();
		if (!ui_sell_dlg)
		{
			LOG_O(Log_error) << "卖物品面板没有打开吧";
			assert(false);
			return false;
		}
		return ui_sell_dlg->PressButton(accept);
	}))
	{
		assert(false);
		return false;
	}
	auto fn_test = [](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			assert(false);
			return false;
		}
		return !main_ui->GetDlgSell();

	};
	if (!TimerDo(50, 1000 * 60, [&fn_test](){
		return GWndExecSync(fn_test);
	}))
	{
		assert(false);
		return false;
	}
	return true;
}

void GPackSell::DebugInfo() const
{
	if (my_pack_item_)
		my_pack_item_->DebugInfo();
	if (npc_pack_item_)
		npc_pack_item_->DebugInfo();
}

stCD_VecInt GPackSell::FindMyPosBySize(const stSizeInt& size) const
{
	if (!my_pack_item_)
	{
		assert(false);
		return kInvalidVecInt;
	}
	return my_pack_item_->FindFreePosBySize(size);
}

GPackageBasePtr GPackSell::GetSellMyPack() const
{
	return my_pack_item_; 
}

void GSwitchPagesMgrBase::DebugInfo() const
{
	for (auto& i : pack_tab_page_)
		i->DebugInfo();
}

GSwitchPagesMgrBase::GSwitchPagesMgrBase()
{
	game_tab_pages_ = nullptr;
}

bool GSwitchPagesMgrBase::PressPage(const GServicePageInfo& page) const
{
	auto tab_bt = page.GetPageTabButton();
	if (!GWndExecSync([tab_bt, this](){
		if (IsUnValidPtr(game_tab_pages_))
		{
			assert(false);
			return false;
		}
		return game_tab_pages_->PressSwitchPage(tab_bt);
	}))
	{
		assert(false);
		return false;
	}
	auto the_fn = [=](){
		auto page_info = game_tab_pages_->FindPageInfo(tab_bt);
		if (!page_info)
		{
			assert(false);
			return false;
		}
		return IsThePageShown(page_info);
	};
	if (!TimerDo(100, 1000 * 60 * 5, [&the_fn](){
		return GWndExecSync(the_fn);
	}))
	{
		assert(false);
		return false;
	}	
	return true;
}

const GSwitchPagesMgrBase::ContT& GSwitchPagesMgrBase::GetPages() const
{
	return pack_tab_page_;
}

bool GSwitchPagesMgrBase::RebuildAllImpl()
{
	pack_tab_page_.clear();
	if (!game_tab_pages_)
	{
		assert(false);
		return false;
	}	
	game_tab_pages_->TraPages([this](const stCD_UiTabPages::PageInfo& page_info){
		if (!ValidPageItem(&page_info))
			return true;
		PackagePtr ptr(new GServicePageInfo(*this, &page_info));
		if (!ptr)
		{
			assert(false);
			return false;
		}
		pack_tab_page_.push_back(ptr);
		return true;
	});
	return true;
}

stCD_UiTabPages* GSwitchPagesMgrBase::GetGameTabPages() const
{
	return game_tab_pages_;
}

const GSwitchPagesMgrBase::PackagePtr GSwitchPagesMgrBase::FindPackByIndexStr(const std::string& index_str) const
{
	if (pack_tab_page_.empty())
	{
		assert(false);
		return nullptr;
	}
	auto it = std::find_if(pack_tab_page_.begin(), pack_tab_page_.end(), [&index_str](const GSwitchPagesMgrBase::PackagePtr& ptr){return ptr->GetIndexStr() == index_str; });
	if (it != pack_tab_page_.end())
		return *it;
	return nullptr;
}

const GSwitchPagesMgrBase::PackagePtr GSwitchPagesMgrBase::FindPackBySize(const stSizeInt& size) const
{
	if (pack_tab_page_.empty())
	{
		assert(false);
		return nullptr;
	}
	auto it = std::find_if(pack_tab_page_.begin(), pack_tab_page_.end(), [&size](const GSwitchPagesMgrBase::PackagePtr& ptr){return !ptr->FindFreePosBySize(size).AnyLess(0); });
	if (it != pack_tab_page_.end())
		return *it;
	return nullptr;
}

bool GSwitchPagesMgrBase::UpdateAllPackPage()
{
	if (!RebuildAll())
	{
		assert(false);
		return false;
	}
	CmnBufferVector<PackagePtr, 100> cont_tmp;
	for (auto& v : pack_tab_page_)
		cont_tmp.push_back(v);
	pack_tab_page_.clear();
	for (auto& v : cont_tmp)
	{
		if (!v->PressPage())
		{
			assert(false);
			return false;
		}
		if (!v->IsValidPage())
			continue;
		if (!v->Update())
		{
			assert(false);
			return false;
		}
		pack_tab_page_.push_back(v);
	}
	return true;
}

const GItemBasePtr GSwitchPagesMgrBase::FindItemByAllPack(const std::string& item_name) const
{
	if (pack_tab_page_.empty())
	{
		assert(false);
		return false;
	}
	GItemBasePtr ret_item = nullptr;
	for (auto& i : pack_tab_page_)
	{
		ret_item = i->FindItemByName(item_name);
		if (ret_item)
			return ret_item;
	}
	return ret_item;
}

bool GSwitchPagesMgrBase::GetItemByFilter(const GFilterItemCont& filter_cont, GItemStaticContT& all_item) const
{
	if (pack_tab_page_.empty())
	{
		assert(false);
		return false;
	}
	GItemBasePtr ret_item = nullptr;
	for (auto& pack_page : pack_tab_page_)
	{
		if (!pack_page)
		{
			assert(false);
			return false;
		}
		auto it_filter = pack_page->FilterIterator<IterContTag_ByRef>(filter_cont);
		if (!it_filter)
		{
			assert(false);
			continue;
		}
		for (auto& i : *it_filter)
			all_item.push_back(i);
	}
	return true;
}

void GSwitchPagesMgrBase::reset()
{
	game_tab_pages_ = nullptr;
	for (auto& v : pack_tab_page_)
	{
		assert(v);
		v->reset();
	}
}

pt_dword GSwitchPagesMgrBase::GetItemCntByName(const std::string& item_name) const
{
	if (pack_tab_page_.empty())
	{
		assert(false);
		return 0;
	}
	pt_dword cnt = 0;
	GItemBasePtr ret_item = nullptr;
	for (auto& i : pack_tab_page_)
		cnt += i->GetItemCntByName(item_name);
	return cnt;
}

GServicePageInfo::GServicePageInfo(GSwitchPagesMgrBase& mgr, const stCD_TabPageInfo* page_info)
	: GServicePage(), package_mgr_(mgr)
{
	assert(page_info);
	game_tab_button_ = page_info->tab_button_;
	page_index_str_.assign(page_info->page_title_.GetStdString());
}

GServicePage::GServicePage() : GPackageBase(kIT_Invalid)
{
}

enMoveItemToRes GServicePage::MoveItToThere(const GItemBase& item, GPackageBase& dst, const stCD_VecInt& dst_pos)
{
	auto old_cnt = CheckSum();
	auto dst_cnt = dst.CheckSum();
	if (!GInterface::PressInventoryItem(*this, item, kPIWK_Control))
	{
		assert(false);
		return kMITR_Failed;
	}
	if (!TimerDo(50, 1000 * 60, [this, old_cnt, dst_cnt, &dst](){
		if (!Update())
		{
			LOG_O(Log_debug) << "更新出问题了，难道面板关闭了？2";
			//assert(false);
			return true;
		}
		if (!dst.Update())
		{
			assert(false);
			return false;
		}
		return CheckSum() != old_cnt || dst_cnt != dst.CheckSum();
	}))
	{
		assert(false);
		return kMITR_Failed;
	}
	return kMITR_Succeed;
}

enMoveItemToRes GServicePage::MoveItToHere(const GItemBase& item, const stCD_VecInt& pos)
{
	auto old_cnt = CheckSum();
	auto src_pack = item.GetPackage();
	if (!src_pack)
	{
		assert(false);
		return kMITR_Failed;
	}
	if (!GInterface::PressInventoryItem(*src_pack, item, kPIWK_Control))
	{
		assert(false);
		return kMITR_Failed;
	}
	if (!TimerDo(50, 1000 * 60, [this, old_cnt](){
		if (!Update())
		{
			LOG_O(Log_debug) << "更新出问题了，难道面板关闭了？";
			//assert(false);
			return true;
		}
		return CheckSum() != old_cnt;
	}))
	{
		assert(false);
		return kMITR_Failed;
	}
	return kMITR_Succeed;
}

void GServicePageInfo::DebugInfo() const
{
	if (!game_grid_panel_)
	{
		assert(false);
		return;
	}
	LOG_O(Log_debug) << "inventory_type:" << GetPackageType() << ",game_grid_panel_Addr:" << game_grid_panel_
		<< ",GridPanelShow:" << game_grid_panel_->is_shown_;
}

stCD_UiObjBase* GServicePageInfo::GetPageTabButton() const
{
	return game_tab_button_;
}

bool GServicePageInfo::PressPage() const
{
	return package_mgr_.PressPage(*this);
}

bool GServicePageInfo::UpdateImpl()
{
	if (!game_grid_panel_)
	{
		if (!game_tab_button_)
		{
			assert(false);
			return false;
		}
		auto game_tab_pages = package_mgr_.GetGameTabPages();
		if (IsUnValidPtr(game_tab_pages))
		{
			assert(false);
			return false;
		}
		auto page_info = game_tab_pages->FindPageInfo(game_tab_button_);
		if (!page_info)
		{
			assert(false);
			return false;
		}
		auto grid_panel = package_mgr_.GetGridPanel(page_info);
		if (!grid_panel)
		{
			assert(false);
			return false;
		}
		if (IsUnValidPtr(grid_panel->inventory_))
		{
			assert(false);
			return false;
		}
		assert(kIT_Invalid == package_type_);
		package_type_ = grid_panel->inventory_type_;
		if (!Init(grid_panel, grid_panel->inventory_))
		{
			assert(false);
			return false;
		}
	}
	return __super::UpdateImpl();
}

bool GServicePageInfo::IsValidPage() const
{
	return GWndExecSync([this](){
		auto game_tab_pages = package_mgr_.GetGameTabPages();
		if (IsUnValidPtr(game_tab_pages))
		{
			assert(false);
			return false;
		}
		auto page_info = game_tab_pages->FindPageInfo(game_tab_button_);
		if (!page_info)
		{
			assert(false);
			return false;
		}
		return package_mgr_.ValidPageItem(page_info);
	});
}

void GServicePageInfo::reset()
{
	game_tab_button_ = nullptr;
}

bool GYiJiePack::UpdateImpl()
{
	auto main_ui = CD_Interface::GetInGameMainUi();
	if (!main_ui)
	{
		LOG_O(Log_error) << "异界面板 更新出错GetInGameMainUi";
		assert(false);
		return false;
	}
	auto ui_locator_dlg_ = main_ui->GetDlgMapLocator();
	if (!ui_locator_dlg_)
	{
		LOG_O(Log_error) << "异界面板没有打开吧";
		assert(false);
		return nullptr;
	}
	auto ui_Grid_dlg = ui_locator_dlg_->GetUiGridPanel();
	if (!ui_Grid_dlg)
	{
		assert(false);
		return false;
	}
	if (!Init(ui_Grid_dlg, ui_Grid_dlg->inventory_))
	{
		assert(false);
		return false;
	}
	return __super::UpdateImpl();
}

bool GYiJiePack::StartButton()
{
	if (GetItemCnt() <= 0)
	{
		LOG_O(Log_error) << "启动异界图鉴之没有发现放入地图";
		assert(false);
		return false;
	}
	auto item_map = GetFirstItem();
	assert(item_map);
	if (!GWndExecSync([](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			LOG_O(Log_error) << "启动异界图鉴面板出错";
			assert(false);
			return false;
		}
		auto ui_lacator_dlg = main_ui->GetDlgMapLocator();
		if (!ui_lacator_dlg)
		{
			LOG_O(Log_error) << "启动异界图鉴没有打开";
			assert(false);
			return false;
		}
		return ui_lacator_dlg->PressStartYiJie();
	}))
	{
		assert(false);
		return false;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_AreaTransition));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	FilterTransferTown filter(obj_mgr->GetFilter());
	if (!TimerDo(100, 1000 * 30, [&obj_mgr, &item_map](){
		if (!obj_mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		assert(item_map);
		auto obj = obj_mgr->GetFirstObj();
		if (!obj || !item_map)
			return false;
		return obj->GetCnName() == item_map->GetItemName(); }))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GHideoutLocatorPack::UpdateImpl()
{
	auto main_ui = CD_Interface::GetInGameMainUi();
	if (!main_ui)
	{
		LOG_O(Log_error) << "藏身处地图装置 更新出错GetInGameMainUi";
		assert(false);
		return false;
	}
	auto ui_self_locator = main_ui->GetDlgSelfMapLocator();
	if (!ui_self_locator)
	{
		assert(false);
		return false;
	}
	auto ui_grid_dlg = ui_self_locator->GetUiGridPanel();
	if (!ui_grid_dlg)
	{
		assert(false);
		return false;
	}
	if (!Init(ui_grid_dlg, ui_grid_dlg->inventory_))
	{
		assert(false);
		return false;
	}
	craft_item_.clear();
	auto map_attr = ui_self_locator->GetMapAttrList();
	if (map_attr)
	{
		map_attr->TraListItem([this](stCD_MapLocatorMapAttrList::stListItem* item){
			if (!item->data_.IsValid())
			{
				assert(false);
				return false;
			}
			stCraft tmp;
			tmp.ui_item_ = item;
			tmp.craft_name_ = WideCharToGbk(CD_SafeName(item->data_.tbl_item_->title_));
			auto require_item = item->data_.tbl_item_->require_item_type_;
			auto require_cnt = item->data_.tbl_item_->require_item_cnt_;
			assert(require_item.size() == require_cnt.size());
			stItemInfo  info;
			for (size_t v = 0; v < require_item.size(); ++v)
			{
				auto name = CD_SafeName(require_item[v].tbl_item_->item_name_);
				info.item_name_ = WideCharToGbk(name);
				info.item_cnt_ = require_cnt[v];
				tmp.item_info_.push_back(info);
			}
			craft_item_.push_back(tmp);
			return true;
		});
	}
	return __super::UpdateImpl();
}

bool GHideoutLocatorPack::StartButton()
{
	if (GetItemCnt() <= 0)
	{
		LOG_O(Log_error) << "启动异界图鉴之没有发现放入地图";
		assert(false);
		return false;
	}
	auto map_item = GetFirstItem();
	if (!map_item)
	{
		assert(false);
		return false;
	}
	if (!GWndExecSync([](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			LOG_O(Log_error) << "启动异界图鉴面板出错";
			assert(false);
			return false;
		}
		auto ui_lacator_dlg = main_ui->GetDlgSelfMapLocator();
		if (!ui_lacator_dlg)
		{
			LOG_O(Log_error) << "启动异界图鉴没有打开";
			assert(false);
			return false;
		}
		return ui_lacator_dlg->PressStart();
	}))
	{
		assert(false);
		return false;
	}
	GameObjMgrPtr obj_mgr(new GameObjMgr(kGOT_AreaTransition));
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	FilterTransferTown filter(obj_mgr->GetFilter());
	FilterGameObjName filter_name(obj_mgr->GetFilter(), map_item->GetItemName());
	if (!TimerDo(100, 1000 * 60, [&obj_mgr](){
		if (!obj_mgr->RebuildAll())
		{
			assert(false);
			return false;
		}
		return !!obj_mgr->GetFirstObj(); }))
	{
		assert(false);
		return false;
	}
	return true;
}

bool GHideoutLocatorPack::Select(const std::string& craft_name)
{
	if (craft_name.empty())
		return false;
	if (craft_item_.empty())
	{
		assert(false);
		return false;
	}
	auto it = std::find_if(craft_item_.begin(), craft_item_.end(), [craft_name](const stCraft& craft){ return craft.craft_name_ == craft_name; });
	if (it == craft_item_.end())
	{
		LOG_O(Log_error) << "选择异界工艺之没有找到该工艺:" << craft_name;
		assert(false);
		return false;
	}
	auto& inventory = GInventoryMgr::GetMe();
	for (auto& v : it->item_info_)
	{
		auto& stash_mgr = GPackStashMgr::GetMe();
		GItemStaticContT all_gem;
		GFilterItemCont filter_cont;
		FilterItemName filte_type(filter_cont, v.item_name_);
		auto cnt = inventory.GetStorAndMainBagFilterItemCnt(filter_cont);
		if (cnt < v.item_cnt_)
		{
			LOG_O(Log_error) << "材料不足！不能使用异界工艺: " << v.item_name_;
			assert(false);
			return false;
		}
	}
	if (!it->ui_item_)
	{
		assert(false);
		return false;
	}
	return GWndExecSync([&it](){
		auto main_ui = CD_Interface::GetInGameMainUi();
		if (!main_ui)
		{
			LOG_O(Log_error) << "藏身处异界图装置出错";
			assert(false);
			return false;
		}
		auto ui_lacator_dlg = main_ui->GetDlgSelfMapLocator();
		if (!ui_lacator_dlg)
		{
			LOG_O(Log_error) << "藏身处异界图装置没有打开";
			assert(false);
			return false;
		}
		auto map_attr = ui_lacator_dlg->GetMapAttrList();
		if (!map_attr)
		{
			assert(false);
			return false;
		}
		return map_attr->Select(it->ui_item_);
	});
}

GItemAgainRinsePtr GItemTypeConfig::MakeItemColor(const luabind::object lua_obj, pt_dword material_max)
{
	GItemAgainRinsePtr res(new GItemColor());
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!res->Parse(lua_obj, material_max))
	{
		assert(false);
		return nullptr;
	}
	config_.push_back(res);
	return shared_from_this();
}

GItemAgainRinsePtr GItemTypeConfig::MakeItemSlotCnt(const luabind::object lua_obj, pt_dword material_max)
{
	GItemAgainRinsePtr res(new GItemSlotCnt());
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!res->Parse(lua_obj, material_max))
	{
		assert(false);
		return nullptr;
	}
	config_.push_back(res);
	return shared_from_this();
}

GItemAgainRinsePtr GItemTypeConfig::MakeItemSlotLink(const luabind::object lua_obj, pt_dword material_max)
{
	GItemAgainRinsePtr res(new GItemSlotLink());
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!res->Parse(lua_obj, material_max))
	{
		assert(false);
		return nullptr;
	}
	config_.push_back(res);
	return shared_from_this();
}

GItemAgainRinsePtr GItemTypeConfig::MakeItemSlotColor(const luabind::object& lua_obj, pt_dword material_max)
{
	GItemAgainRinsePtr res(new GItemSlotColor());
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!res->Parse(lua_obj, material_max))
	{
		assert(false);
		return nullptr;
	}
	config_.push_back(res);
	return shared_from_this();
}

GItemAgainRinsePtr GItemTypeConfig::MakeItemQuality(const luabind::object& lua_obj, pt_dword material_max)
{
	GItemAgainRinsePtr res(new GItemQuality());
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	if (!res->Parse(lua_obj, material_max))
	{
		assert(false);
		return nullptr;
	}
	config_.push_back(res);
	return shared_from_this();
}

bool GItemTypeConfig::DoFilter(const GItemBase& item) const
{
	if (config_.empty())
		return true;
	for (auto& i : config_)
	{
		if (!i->DoFilter(item))
			return false;
	}
	return true;
}

bool GItemTypeConfig::CheckMaterial() const
{
	if (config_.empty())
	{
		assert(false);
		return false;
	}
	for (auto& i : config_)
	{
		if (!i->CheckPackMaterial())
			return false;
	}
	return true;
}

bool GItemTypeConfig::Rinse(const GItemBase& item) const
{
	if (config_.empty())
	{
		assert(false);
		return false;
	}
	auto item_pos = item.GetCurItemPos();
	for (auto& i : config_)
	{
		auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
		if (!main_bag)
		{
			assert(false);
			return false;
		}
		auto new_item = main_bag->FindItemByPos(item_pos);
		if (!new_item)
		{
			assert(false);
			return false;
		}
		if (!i->Rinse(*new_item))
			return false;
	}
	return true;
}

GItemRinseMgr::ConT GItemRinseMgr::rinse_config_;
GItemAgainRinsePtr GItemRinseMgr::CreateItemRinseConfig(enCD_ItemType item_type, pt_dword min_lvl, pt_dword max_lvl)
{
	stBaseInfo  info = {item_type, min_lvl, max_lvl};
	auto it = rinse_config_.find(info);
	if (it != rinse_config_.end())
		it->second.reset();
	GItemAgainRinsePtr res(new GItemTypeConfig());
	if (!res)
	{
		assert(false);
		return nullptr;
	}
	rinse_config_[info] = res;
	return res;
}

bool GItemRinseMgr::MeetEquip(const GItemBase& item) const
{
	auto equip_confit = GetEquipConfig(item);
	if (!equip_confit)
		return true;
	return equip_confit->DoFilter(item);
}

bool GItemRinseMgr::CanRinse(const GItemBase& item) const
{
	auto equip_confit = GetEquipConfig(item);
	if (!equip_confit)
		return false;
	if (equip_confit->DoFilter(item))
		return false;
	return equip_confit->CheckMaterial();
}

GItemAgainRinsePtr GItemRinseMgr::GetEquipConfig(const GItemBase& item) const
{
	auto item_lvl = item.GetItemLvl();
	auto item_type = item.GetItemType();
	for (auto& i : rinse_config_)
	{
		if (i.first.item_type_ != item_type)
			continue;
		if (i.first.min_lvl_ > item_lvl || item_lvl > i.first.max_lvl_)
			continue;
		return i.second;
	}
	return nullptr;
}

bool GItemRinseMgr::BeginRinse(const GItemBase& item) const
{
	auto confit = GetEquipConfig(item);
	if (!confit)
	{
		LOG_O(Log_debug) << "该装备没有配置洗练数据:" << item.GetItemName();
		return false;
	}
	return confit->Rinse(item);
}

GItemColor::GItemColor()
{
	item_color_ = kIC_Non;
}

bool GItemColor::Parse(const luabind::object& lua_obj, pt_dword material_cnt)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		LOG_O(Log_error) << "设置洗练装备之装备颜色参数必须是表类型";
		assert(false);
		return false;
	}
	if (type(lua_obj[1]) != LUA_TNUMBER)
	{
		LOG_O(Log_error) << "设置洗练装备之装备颜色第一个参数必须是整数型";
		assert(false);
		return false;
	}
	item_color_ = LuaObjCast(lua_obj[1], kIC_Non);
	if (type(lua_obj[2]) != LUA_TSTRING)
	{
		LOG_O(Log_error) << "设置洗练装备之装备颜色第二个参数必须是所需材料名字";
		assert(false);
		return false;
	}
	material_max_ = material_cnt;
	material_name_ = LuaObjCast(lua_obj[2], kEmptyStr);
	return true;
}

bool GItemColor::DoFilter(const GItemBase& item) const
{
	LOG_O(Log_debug) << "当前物品:" << item.GetItemName() << ",颜色:" << item.GetItemColor();
	return item.GetItemColor() >= item_color_;
}

GItemSlotCnt::GItemSlotCnt() : slot_cnt_(0)
{
}

bool GItemSlotCnt::Parse(const luabind::object& lua_obj, pt_dword material_cnt)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		LOG_O(Log_error) << "设置洗练装备之装备颜色参数必须是表类型";
		assert(false);
		return false;
	}
	if (type(lua_obj[1]) != LUA_TNUMBER)
	{
		LOG_O(Log_error) << "设置洗练装备之装备颜色第一个参数必须是整数型";
		assert(false);
		return false;
	}
	slot_cnt_ = LuaObjCast(lua_obj[1], kIC_Non);
	if (type(lua_obj[2]) != LUA_TSTRING)
	{
		LOG_O(Log_error) << "设置洗练装备之装备颜色第二个参数必须是所需材料名字";
		assert(false);
		return false;
	}
	material_max_ = material_cnt;
	material_name_ = LuaObjCast(lua_obj[2], kEmptyStr);
	return true;
}

bool GItemSlotCnt::DoFilter(const GItemBase& item) const
{
	LOG_O(Log_debug) << "当前物品:" << item.GetItemName() << ",槽子数量:" << item.GetSlotCnt();
	return item.GetSlotCnt() >= (int)slot_cnt_;
}

GItemSlotLink::GItemSlotLink() : slot_link_(0)
{
}

bool GItemSlotLink::Parse(const luabind::object& lua_obj, pt_dword material_cnt)
{
	using namespace luabind;
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		LOG_O(Log_error) << "设置洗练装备之槽子连接必须是表类型";
		assert(false);
		return false;
	}
	if (type(lua_obj[1]) != LUA_TNUMBER)
	{
		LOG_O(Log_error) << "设置洗练装备之槽子连接第一个参数必须是整数型";
		assert(false);
		return false;
	}
	slot_link_ = LuaObjCast(lua_obj[1], kIC_Non);
	if (type(lua_obj[2]) != LUA_TSTRING)
	{
		LOG_O(Log_error) << "设置洗练装备之槽子连接第二个参数必须是所需材料名字";
		assert(false);
		return false;
	}
	material_name_ = LuaObjCast(lua_obj[2], kEmptyStr);
	material_max_ = material_cnt;
	return true;
}

bool GItemSlotLink::DoFilter(const GItemBase& item) const
{
	LOG_O(Log_debug) << "当前物品:" << item.GetItemName() << ",槽子连接:" << item.GetMaxGemLinkValue();
	return item.GetMaxGemLinkValue() >= slot_link_;
}

GItemSlotColor::GItemSlotColor()
{

}

bool GItemSlotColor::Parse(const luabind::object& lua_obj, pt_dword material_cnt)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE || type(lua_obj[1]) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}
	iterator it(lua_obj[1]), it_end;
	for (; it != it_end; ++it)
	{
		object v = *it;
		if (type(v) != LUA_TNUMBER)
		{
			LOG_O(Log_debug) << "设置洗练装备之槽子颜色必须是指定颜色值";
			continue;
		}
		auto slot_color = LuaObjCast(v, kGSC_Invalid);
		if (slot_color == kGSC_Invalid)
		{
			LOG_O(Log_error) << "设置洗练装备之槽子颜色发现一个无效颜配置";
			continue;
		}
		slot_color_.push_back(slot_color);
	}
	if (type(lua_obj[2]) != LUA_TSTRING)
	{
		assert(false);
		return false;
	}
	material_name_ = LuaObjCast(lua_obj[2], kEmptyStr);
	material_max_ = material_cnt;
	return true;
}

bool GItemSlotColor::DoFilter(const GItemBase& item) const
{
	auto link = item.GetGemLink();
	if (!link || link->empty())
	{
		assert(false);
		return false;
	}
	int index = 0;
	for (auto& i : *link)
	{
		int tmp[kGSC_Invalid] = {};
		for (int v = 0; v < i; ++v)
		{
			auto slot_color = item.GetColorByIndex(index++);
			if (slot_color >= kGSC_Invalid)
				continue;
			++tmp[slot_color];
		}
#ifdef _DEBUG
		int index = 0;
		for (int j = 0; j < kGSC_Invalid; ++j)
		{
			if (tmp[j] <= 0)
				continue;
			LOG_O(Log_debug) << "当前物品:" << item.GetItemName() << ",连接数:" << i << ",(颜色:" << j << ",数量:" << tmp[j] << ")";
		}
#endif
		for (auto& k : slot_color_)
		{
			if (tmp[k] <= 0)
				return false;
			--tmp[k];
		}
		return true;
	}
	return false;
}

bool GItemAgainRinse::Parse(const luabind::object& lua_obj, pt_dword material_cnt)
{
	assert(false);
	return false;
}

bool GItemAgainRinse::CheckPackMaterial() const
{
	auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
	if (!main_bag)
	{
		assert(false);
		return false;
	}
	if (main_bag->GetItemCntByName(material_name_) >= material_max_)
		return true;
	if (MyLuaThrdData::InTown())
	{
		auto& stash_mgr = GPackStashMgr::GetMe();
		if (!stash_mgr.OpenStoreAndUpdate(false))
		{
			assert(false);
			return false;
		}
		return stash_mgr.GetItemCntByName(material_name_) >= material_max_;
	}
	return false;
}

bool GItemAgainRinse::CheckMaterial() const
{
	assert(false);
	return false;
}

bool GItemAgainRinse::Rinse(const GItemBase& item) const
{
	if (DoFilter(item))
		return true;
	pt_dword rinse_cnt = 0;
	while (true)
	{
		auto main_bag = GPackageMgrBase().NewPackage(kIT_MainBag);
		if (!main_bag)
		{
			assert(false);
			return false;
		}
		auto new_item = main_bag->FindItemByPos(item.GetCurItemPos());
		if (!new_item)
		{
			assert(false);
			return false;
		}
		if (DoFilter(*new_item))
			return true;
		if (rinse_cnt++ >= material_max_)
			return false;
		auto material_item = main_bag->FindItemByName(material_name_);
		if (!material_item && MyLuaThrdData::InTown())
		{
			auto& stash_mgr = GPackStashMgr::GetMe();
			if (!stash_mgr.OpenStoreAndUpdate(false))
			{
				assert(false);
				return false;
			}
			GItemStaticContT all_item;
			GFilterItemCont filter_cont;
			FilterItemName filte_name(filter_cont, material_name_);
			stash_mgr.GetItemByFilter(filter_cont, all_item);
			if (all_item.empty())
				return false;
			if (!stash_mgr.OpenStoreAndUpdate(true))
			{
				assert(false);
				return false;
			}
			all_item.clear();
			stash_mgr.GetItemByFilter(filter_cont, all_item);
			assert(!all_item.empty());
			material_item = all_item[0];
			auto item_pack = material_item->GetPackage();
			if (!item_pack)
			{
				assert(false);
				return false;
			}
			if (!item_pack->PressPage())
			{
				assert(false);
				return false;
			}
			if (!stash_mgr.OpenStoreAndUpdate(true))
			{
				assert(false);
				return false;
			}
		}
		if (!material_item->UseTo(*new_item))
			return false;
		LOG_O(Log_debug) << "洗练一次,消耗材料:" << material_item->GetItemName() << ",已消耗材料数量:" << rinse_cnt;
	}
	return false;
}

GItemQuality::GItemQuality() : quality_(0)
{
}

bool GItemQuality::Parse(const luabind::object& lua_obj, pt_dword material_cnt)
{
	using namespace luabind;
	if (type(lua_obj) != LUA_TTABLE)
	{
		assert(false);
		return false;
	}
	if (type(lua_obj[1]) != LUA_TNUMBER)
	{
		assert(false);
		return false;
	}
	quality_ = LuaObjCast(lua_obj[1], 0);
	if (type(lua_obj[2]) != LUA_TSTRING)
	{
		assert(false);
		return false;
	}
	material_name_ = LuaObjCast(lua_obj[2], kEmptyStr);
	material_max_ = material_cnt;
	return true;
}

bool GItemQuality::DoFilter(const GItemBase& item) const
{
	return item.GetQuality() >= quality_;
}

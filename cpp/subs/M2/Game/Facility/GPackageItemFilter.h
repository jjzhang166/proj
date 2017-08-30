#pragma once
/*
@author zhp
@date 2017/3/29 10:24
@purpose for item/package filter
*/
#include "GType.h"
#include <map>
#include <WgFacility/CmnFilter.h>
#include "GClass.h"

template<bool DefRet>
class GF_FilterItemCont : public CF_Cont<GFilterItemTraits, DefRet, GFilterItemPtr,
	CmnStaticVector<GFilterItemPtr, kIFT_Max>, CF_SmartGetter>{
public:
	bool DoFilt(const FittedType& obj) const override{
		for (const auto& filter : filters_)
		{
			const auto& f = getter_type::GetPointer(filter);
			if (!f)
				continue;
			if (!f->Before(obj))
				continue;
			if (f->DoFilt(obj) == DefRet)
				return DefRet;
		}
		return !DefRet;
	}
};
typedef GF_FilterItemCont<true> GFilterItemSmartCont;
//typedef GF_FilterItemCont<false> GFilterItemSellCont;

template<typename BaseT>
class GFilterItemContForLua : public BaseT{
	typedef typename BaseT::FittedType FittedType;

public:
	GFilterItemContForLua(){ 
		GetFilters().GetValues().resize(kIFT_Max);
	};
	void SetFilter(const GFilterItemPtr& filter){
		GetFilters().GetValues()[filter->GetType()] = filter;
	}
	GFilterItemPtr FindFilterTypeByType(en_ItemFilterType type) const{
		if (type >= kIFT_Max)
		{
			assert(false);
			return nullptr;
		}
		return GetFilters().GetValues()[type];
	}
	bool DoFilt(const FittedType& obj) const override{
		auto black_prt = FindFilterTypeByType(kIFT_BlackName);
		if (black_prt && black_prt->DoFilt(obj))
			return false;
		//auto quest_prt = FindFilterTypeByType(kIFT_QuestItem);
		//if (quest_prt && quest_prt->DoFilt(obj))
		//	return BaseT::DefRet;
		return __super::DoFilt(obj);
	}
	//bool Parse(const luabind::object& tbl)
	//{
	//	using namespace luabind;
	//	if (type(tbl) != LUA_TTABLE)
	//	{
	//		assert(false);
	//		return false;
	//	}
	//	std::vector<GFilterItemPtr>  tmp_filter;
	//	iterator it(tbl), it_end;
	//	for (; it != it_end; ++it)
	//	{
	//		object v = *it;
	//		auto filter_type = LuaObjCast(v[1], kIFT_Max);
	//		auto tbl = v[2];
	//		auto filter_ptr = GItemFilterFactory::CreateItemFilter(filter_type, tbl);
	//		if (!filter_ptr)
	//		{
	//			assert(false);
	//			return false;
	//		}
	//		if (!filter_ptr->Parse(tbl))
	//		{
	//			assert(false);
	//			return false;
	//		}
	//		filter_ptr->SetType(filter_type);
	//		tmp_filter.push_back(filter_ptr);
	//	}
	//	if (tmp_filter.empty())
	//		return true;
	//	SetFilter(tmp_filter[0]->GetType(), tmp_filter[0]);
	//	for (auto it = tmp_filter.begin(); it != tmp_filter.end();)
	//	{
	//		auto it_next = it;
	//		++it_next;
	//		if (it_next == tmp_filter.end())
	//			break;
	//		(*it)->SetCombine(*it_next);
	//		++it;
	//	}
	//	return true;
	//}
};

class FilterItemBase : public GIFilterItem{
public:
	inline void SetType(en_ItemFilterType type){ filter_type_ = type; }
	inline en_ItemFilterType GetType() const { return filter_type_; }

private:
	en_ItemFilterType	filter_type_;
	bool				def_ret_;
};

class FilterGroupFilter : public FilterItemBase{
	typedef std::vector<GFilterItemPtr> ConT;
public:
	FilterGroupFilter(){};
	bool DoFilt(const GItemBase& obj) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	ConT		group_filter_;
};

template<bool DefRet>
class FilterItemQuestItem : public FilterItemBase{
public:
	inline bool Before(const GItemBase& obj) const override{ return true; }
	bool DoFilt(const FittedType& obj) const override{
		if (obj.IsQuestItem())
			return DefRet;
		return !DefRet;
	}
};

template<bool DefRet, class BastT>
class GFilterItemContForLuaWithQuest : public BastT{
public:
	GFilterItemContForLuaWithQuest(){
		GFilterItemPtr filter(new FilterItemQuestItem<DefRet>);
		filter->SetType(kIFT_QuestItem);
		SetFilter(filter);
	}
};

typedef GFilterItemContForLua<GFilterItemSmartCont> GFilterItemContForLuaTrue;
//typedef GFilterItemContForLua<GFilterItemSellCont> GFilterItemContForLuaFalse;
typedef GFilterItemContForLuaWithQuest<false, GFilterItemContForLuaTrue> GFilterItemContSmart;
typedef GFilterItemContForLuaWithQuest<true, GFilterItemContForLuaTrue> GFilterItemContPickUp;

class FilterItemWhiteByName : public FilterItemBase{
	typedef std::vector<std::string> ItemNameConT;

public:
	bool DoFilt(const GItemBase& item) const override;
	bool Parse(const luabind::object& tbl) override;

private:
	ItemNameConT		vc_item_name_;
};

class FilterItemBlackByName : public FilterItemBase{
	typedef std::vector<std::string> ItemNameConT;

public:
	FilterItemBlackByName();
	bool Before(const GItemBase& obj) const override;
	bool DoFilt(const GItemBase& item) const override;
	bool Parse(const luabind::object& tbl) override;

private:
	ItemNameConT		item_names_;
};

class FilterItemCnt : public FilterItemBase{
public:
	typedef std::map<std::string, pt_dword> ItemCntConT;

public:
	FilterItemCnt();
	bool Before( const GItemBase& obj) const override;
	bool DoFilt(const GItemBase& obj) const override;
	bool Parse(const luabind::object& tbl) override;

private:
	ItemCntConT		item_retain_cnt_;
};

class FilterItemByQuality : public FilterItemBase{
public:
	bool DoFilt(const GItemBase& item) const override;

private:
	int			quality_;
};

class FilterItemBySlot : public FilterItemBase{
	typedef std::vector<int>	ConT;
public:
	FilterItemBySlot();
	bool Before(const GItemBase& obj) const override; 
	bool DoFilt(const GItemBase& item) const override;
	bool Parse(const luabind::object& tbl) override;

private:
	ConT		slot_num_;
};

class FilterItemByColour : public FilterItemBase{
	typedef std::vector<enCD_ItemColor> ConT;
public:
	FilterItemByColour();
	bool Before(const FittedType& obj) const;
	bool DoFilt(const GItemBase& item) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	ConT		item_colour_;
};

class FilterItemByType : public FilterItemBase{
	typedef std::vector<enCD_ItemType>  TypeCont;

public:
	FilterItemByType();
	bool DoFilt(const GItemBase& item) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	TypeCont	item_type_;
};

class FilterItemGemLink : public FilterItemBase{
	typedef std::vector<int> ConT;
public:
	FilterItemGemLink();
	bool Before(const FittedType& obj) const override;
	bool DoFilt(const GItemBase& item) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	ConT	link_value_;
};

class FilterItemBindState : public FilterItemBase{
public:
	FilterItemBindState(){};
	bool DoFilt(const GItemBase& item) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	bool			bind_state_;
};

class FilterItemThreeColorLink : public FilterItemBase{
public:
	FilterItemThreeColorLink(){};
	bool DoFilt(const GItemBase& obj) const override;
	bool Parse(const luabind::object& lua_obj) override;
};

class FilterItemEmboitement : public FilterItemBase{
public:
	FilterItemEmboitement(GClass::LackConT& loack_item_) : lock_item_(loack_item_){}
	bool Before(const GItemBase& obj) const override;
	bool DoFilt(const GItemBase& obj) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	mutable	GClass::LackConT&	lock_item_;
};
//////////////////////////////////////////////////////////////////////////
class FilterItemIdentified : public GFilterItemEnsure{
public:
	FilterItemIdentified(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const GItemBase& item) const override;
};
class FilterItemNotIdentified : public GFilterItemEnsure{
public:
	FilterItemNotIdentified(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const GItemBase& item) const override;
};

class FilterItemNotBind : public GFilterItemEnsure{
public:
	FilterItemNotBind(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const GItemBase& item) const override;
};

class FilterItemGem : public GFilterItemEnsure{
public:
	FilterItemGem(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const GItemBase& item) const override;
};

class FilterItemFlask : public GFilterItemEnsure{
public:
	FilterItemFlask(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const GItemBase& item) const override;
};

class FilterItemNotFlask : public GFilterItemEnsure{
public:
	FilterItemNotFlask(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const GItemBase& item) const override;
};

class FilterItemEquip : public GFilterItemEnsure{
public:
	FilterItemEquip(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const GItemBase& item) const override;
};

class FilterItemCanWear : public GFilterItemEnsure{
public:
	FilterItemCanWear(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const GItemBase& item) const override;
};
class FilterItemRing : public GFilterItemEnsure{
public:
	FilterItemRing(GFilterItemCont& cont) : GFilterItemEnsure(cont){}
	bool DoFilt(const FittedType& obj) const override;
};

class FilterItemWeapon : public GFilterItemEnsure{
public:
	FilterItemWeapon(GFilterItemCont& cout) :GFilterItemEnsure(cout){}
	bool DoFilt(const GItemBase& obj)const override;
};

class FilterItemName : public GFilterItemEnsure{
public:
	FilterItemName(GFilterItemCont& cout, const std::string& item_name) : GFilterItemEnsure(cout){ item_name_ = item_name; }
	bool DoFilt(const GItemBase& obj) const override;

private:
	std::string		item_name_;
};

class FilterItemColor : public GFilterItemEnsure{
public:
	FilterItemColor(GFilterItemCont& cout, enCD_ItemColor color) : GFilterItemEnsure(cout) { item_color_ = color; }
	bool DoFilt(const GItemBase& obj) const override;

private:
	enCD_ItemColor		item_color_;
};

class FilterItemType : public GFilterItemEnsure{
public:
	FilterItemType(GFilterItemCont& cout, enCD_ItemType item_type) : GFilterItemEnsure(cout){ item_type_ = item_type; }
	bool DoFilt(const GItemBase& obj) const override;

private:
	enCD_ItemType item_type_;
};

class FilterItemPropValue : public GFilterItemEnsure{
	typedef std::map<std::string, int>	ConT;
public:
	FilterItemPropValue(GFilterItemCont& cout) : GFilterItemEnsure(cout){}
	bool DoFilt(const GItemBase& obj) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	ConT		item_prop_;
};

class FilterItemLessLvl : public GFilterItemEnsure{
public:
	FilterItemLessLvl(GFilterItemCont& cout) : GFilterItemEnsure(cout){}
	bool DoFilt(const GItemBase& obj) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	int item_lvl_;
};

class FilterItemLessEqualColour : public GFilterItemEnsure{
public:
	FilterItemLessEqualColour(GFilterItemCont& cout) : GFilterItemEnsure(cout){}
	bool DoFilt(const GItemBase& obj) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	enCD_ItemColor		item_color_;
};

class FilterItemBlackNameList : public GFilterItemEnsure{
public:
	FilterItemBlackNameList(GFilterItemCont& filter_cont) : GFilterItemEnsure(filter_cont){}
	bool DoFilt(const GItemBase& obj) const override;
	bool Parse(const luabind::object& lua_obj) override;

private:
	NameListStringT		item_black_list_;
};

class FilterItemTypeConT : public GFilterItemEnsure{
public:
	template<typename... Args>
	FilterItemTypeConT(GFilterItemCont& filter_cont, const Args... types) : GFilterItemEnsure(filter_cont), item_Type_(types...){};
	bool DoFilt(const GItemBase& obj) const override;

private:
	GFilterItemType			item_Type_;
};
//////////////////////////////////////////////////////////////////////////
template<typename IterContTag_>
class GPackagesItemsIter : public GItemIterator{
public:
	GPackagesItemsIter(const PackagesContT& packages, const GIFilterItem& item_filter)
		: packages_(packages), item_filter_(item_filter){
		it_ = packages_.begin();
		it_end_ = packages_.end();
	}
	void First() override{
		it_ = packages_.begin();
		it_end_ = packages_.end();
		item_it_.reset();
		DoFindFirst();
	}
	void Next() override{
		if (item_it_)
		{	for (item_it_->Next(); !item_it_->IsDone(); item_it_->Next())
				return;
		}
		++it_;
		DoFindFirst();
	}
	bool IsDone() const override{
		if (!item_it_)
			return true;
		return item_it_->IsDone();
	}
	GItemBasePtr CurItem() const override{
		if (!item_it_)
		{
			assert(false);
			return nullptr;
		}
		return item_it_->CurItem();
	}

private:
	void DoFindFirst(){
		for (; it_ != it_end_; ++it_)
		{
			item_it_ = (*it_)->FilterIterator<IterContTag_>(item_filter_);
			if (!item_it_)
			{
				assert(false);
				return;
			}
			for (item_it_->First(); !item_it_->IsDone(); item_it_->Next())
				return;
		}
		item_it_.reset();
	}

private:
	PackagesContT				packages_;
	PackagesContT::iterator		it_;
	PackagesContT::iterator		it_end_;
	GItemIteratorPtr			item_it_;
	const GIFilterItem&			item_filter_;
};

template<typename IterContTag_>
class GPackagesIterateItemsIter : public GItemIterator{
public:
	GPackagesIterateItemsIter(const GPackageIteratorPtr& package_iter, const GIFilterItem& item_filter)
		: package_iter_(package_iter), item_filter_(item_filter){
		assert(package_iter_);
	}
	void First() override{
		package_iter_->First();
		item_it_.reset();
		DoFindFirst();
	}
	void Next() override{
		if (item_it_)
		{
			for (item_it_->Next(); !item_it_->IsDone(); item_it_->Next())
				return;
		}
		package_iter_->Next();
		DoFindFirst();
	}
	bool IsDone() const override{
		if (!item_it_)
			return true;
		return item_it_->IsDone();
	}
	GItemBasePtr CurItem() const override{
		if (!item_it_)
		{
			assert(false);
			return nullptr;
		}
		return item_it_->CurItem();
	}

private:
	void DoFindFirst(){
		for (; !package_iter_->IsDone(); package_iter_->Next())
		{
			auto cur_package = package_iter_->CurItem();
			if (!cur_package)
			{
				assert(false);
				return;
			}
			item_it_ = cur_package->FilterIterator<IterContTag_>(item_filter_);
			if (!item_it_)
			{
				assert(false);
				return;
			}
			for (item_it_->First(); !item_it_->IsDone(); item_it_->Next())
				return;
		}
		item_it_.reset();
	}

private:
	GPackageIteratorPtr			package_iter_;
	GItemIteratorPtr			item_it_;
	const GIFilterItem&			item_filter_;
};
//////////////////////////////////////////////////////////////////////////
class GPackageIterInRange : public GPackageIterator{
public:
	GPackageIterInRange(const GPackageMgrBase& package_mgr, enCD_InventoryType min, enCD_InventoryType max);
	void First() override;
	void Next() override;
	bool IsDone() const override;
	GPackageBasePtr CurItem() const override;

private:
	enCD_InventoryType		min_;
	enCD_InventoryType		max_;
	enCD_InventoryType		cur_;
	const GPackageMgrBase&	package_mgr_;
	GPackageBasePtr			cur_item_;
};
//////////////////////////////////////////////////////////////////////////
class GEquipTypeMgr : public Singleton<GEquipTypeMgr, Singleton_MakeMe>{
	typedef std::map<enCD_ItemType, enWeaponCompositeCate> ItemType2CompositeCate;
	typedef std::map<enCD_ItemType, GEquipPackageTypeCont> ItemType2Positions;
	typedef std::map<enCD_ItemType, GEquipPositionItemTypes> ItemType2PositionItemTypes;

public:
	GEquipTypeMgr();
	enWeaponCompositeCate GetWeaponCompositeCate(const GItemBase& item) const;
	GEquipPackageTypeCont GetPositions(const GItemBase& item) const;
	GEquipPositionItemTypes GetSwitchItemTypes(const GItemBase& item) const;
	GEquipPositionItemTypes GetSwitchItemTypes(const GItemBase& item, enCD_InventoryType but_position) const;
	bool CanWearTheEquip(const GEquipPositionItemTypes& item_types, const GItemBase& item, enCD_InventoryType* out_position) const;
	bool CanWearTheEquips(const GItemBase& lhs, const GItemBase& rhs) const;
	static enCD_InventoryType SwitchPosition(enCD_InventoryType position);

private:
	ItemType2CompositeCate		item_type2composite_cate_;
	ItemType2Positions			item_type2positions_;
	ItemType2PositionItemTypes	item_type2position_item_types_;;
};
//////////////////////////////////////////////////////////////////////////
class GItemFilterFactory{
public:
	static GFilterItemPtr MakeItemFilter(en_ItemFilterType filter_type);
	template<class BaseT>
	static bool CreateItemFilter(GFilterItemContForLua<BaseT>& filter_cont, en_ItemFilterType filter_type, const luabind::object& tbl){
		assert(false);
		auto filter = MakeItemFilter(filter_type);
		if (!filter)
		{
			assert(false);
			return false;
		}
		if (!filter->Parse(tbl))
		{
			assert(false);
			return false;
		}
		filter_cont.SetFilter(filter);
		return true;
	}
	static GFilterItemPtr CreateItemFilter(en_ItemFilterType filter_type, const luabind::object& tbl)
	{
		auto filter = MakeItemFilter(filter_type);
		if (!filter)
		{
			assert(false);
			return false;
		}
		if (!filter->Parse(tbl))
		{
			assert(false);
			return false;
		}
		filter->SetType(filter_type);
		return filter;
	}
};

//////////////////////////////////////////////////////////////////////////
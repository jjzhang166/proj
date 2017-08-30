#pragma once
/*
@author zhp
@date 2017/3/28 15:08
@purpose for item,package
*/
#include <boost/shared_ptr.hpp>
#include <Common/GameObjBase.h>
#include <CD_CommonFiles/Data/CD_Interface.h>
#include "GClass.h"
#include <bitset>
#include <map>
#include "GPackageItemFilter.h"
#include <StringValue/CSV_Node.h>

//////////////////////////////////////////////////////////////////////////
typedef std::vector<std::string> GItemAttrStringCont;
struct GItemTipsBaseAttr{
	std::string				attr_cn_name_;
	GItemAttrStringCont		attr_values_;

	void DebugInfo() const;
};
typedef std::vector<GItemTipsBaseAttr> GItemTipsBaseAttrCont;

struct GItemTips{
	std::string				item_words_;		//词缀
	std::string				item_name_;
	GItemTipsBaseAttrCont	base_attrs_;		//基本属性
	GItemTipsBaseAttrCont	require_attrs_;		//需求属性
	GItemAttrStringCont		blue_attrs_;		//蓝属性
	GItemAttrStringCont		add_bule_attrs_;	//附加蓝属性
	std::string				desc_suffix_;		//橘黄色的说明

	void DebugInfo() const;
	bool From(stCD_ItemTipsParams* game_tips);
	int TipsWeight() const;
};

struct GItemTipsPanel{
	typedef std::vector<CSV_T<CSV_Calculator> > CalculatorCont;

	GItemAttrStringCont		rows_;
	static CalculatorCont	calculator_;

	void DebugInfo() const;
	bool From(stCD_ItemTipsParams* game_tips);
	int TipsWeight() const;
	//清空物品属性解析
	static void ClearItemAttrParser();
	//物品属性解析
	static CSV_NodePtr CreateItemAttrParser(const luabind::object& fn);
};
//////////////////////////////////////////////////////////////////////////

class GItemBase : public GGameObjBase, public boost::enable_shared_from_this<GItemBase>{
	friend class GItemMgrBase;
	friend class GItemGem;
	typedef boost::optional<int> WeightType;

public:
	typedef GClass::stBasePro BasePro;
	typedef CmnStaticVector<stItemSlot, CD_kGemSlotMaxCnt> Cont_Slot;
	struct stSellPrice{
		stCD_AttrDbItemBase*	attr_db_item_base_;
		pt_dword				item_cnt_;
	};
	typedef GItemSellPriceCont<stSellPrice> SellPriceCont;

public:
	GItemBase();

protected:
	GItemBase(enItemCate item_cate);

public:
	bool operator == (const GItemBase& rhs) const;
	inline const std::string& GetItemName() const { return item_name_; }
	inline pt_dword GetItemCnt() const{ return item_cnt_; }
	inline pt_dword GetItemMaxStack() const { return max_stack_cnt_; }
	inline pt_dword GetItemId() const{ return item_id_; }
	inline enCD_ItemColor GetItemColor() const { return item_color_; }
	inline enCD_ItemType GetItemType() const{ return item_type_; }
	inline enItemCate GetItemCate() const{ return item_cate_; }
	inline bool IsIdentified() const{ return is_identified_; }
	inline bool IsBind() const { return is_bind_; }
	inline const stSizeInt& GetItemSize() const{ return item_size_; }
	inline int GetGridsCnt() const{ return item_size_.Size(); }
	inline const BasePro& GetBasePro() const{ return astrict_; }
	GPackageBasePtr GetPackage() const;
	enCD_InventoryType GetInventoryType() const;
	inline const stCD_ItemGridsIdxInfo& GetItemGridInfo() const{ return item_idx_info_; }
	stCD_VecInt GetCurItemPos() const;
	bool IsWeapon() const{ return item_cate_ == kICATE_Weapon; }
	bool IsRing() const { return item_cate_ == kICATE_Ring; }
	bool IsGem() const { return item_cate_ == kICATE_Gem; }
	bool IsFlask() const{ return item_cate_ == kICATE_Flask; }
	virtual bool IsEquip() const;
	bool IsQuestItem() const{ return item_cate_ == kICATE_Quest; }
	enItemCate GetCateType() const { return item_cate_; }
	bool CanWear() const;					//是否满足佩戴需求
	virtual void PushValidFlaskSpeedInfo(GFlaskSpeedCont& cont) const;
	int CheckSum(int bag_width) const;

public:
	virtual void PushGoodToTheResult(GItemStaticContT& out_items);
	int Weight() const;
	//是否比拥有的更好，多用于拾取物品
	virtual GPackageBasePtr GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const;
	virtual bool Use() const;
	virtual bool UseTo(const GItemBase& dst_item) const;
	//移动到，如果pos为负，则自动生成位置
	virtual enMoveItemToRes MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const;
	//脱掉宝石，如果gem_idx为负，则取出所有宝石；如果pos为负，则自动生成位置
	virtual enTakeoffGemRes TakeoffGemTo(int gem_idx, GPackageBase& dst, const stCD_VecInt& pos) const;
	//如果gem_idx为负，则自动生成位置
	virtual enInsideGemRes InsideGemTo(int gem_idx, GItemBase& dst_equip) const;
	enInsideGemRes InsideGemTo(int gem_idx, enCD_InventoryType dst_inventory) const;
	//放下物品，如果pos为负，则自动生成位置
	virtual enPutItemDownRes PutDownTo(GPackageBase& dst, const stCD_VecInt& pos) const;
	virtual bool DoFilter() const;
	//必须要保留的物品
	virtual bool IsMustKeepItem() const;
	bool IsCanDestroy() const;
	bool Destroy(bool need_takeup) const;
	bool MergeItemTo(const GItemBase& dst_item) const;
	bool IsInSize(const stSizeInt& size) const;
	void DebugInfo() const override;

	//要想办法去掉的虚函数或函数
public:
	virtual void GetAllPackGem(GItemContT& all_gem) const;
	virtual void GetInvalidGem(GemEquipConT& all_gem) const;
	virtual enCD_FlaskType GetFlaskType() const;
	//返回-1 表示该颜色没有可用的插槽.
	virtual int FindFreeSlot(const enCD_GemSlotColor _color) const{ return -1; }
	virtual int GetGemIndexByName(const std::string& gem_name){ return -1; }
	virtual enCD_GemSlotColor GetColorByIndex(int index){ return kGSC_Invalid; }
	virtual int GetSlotCnt() const { return -1; }
	virtual pt_dword GetMaxGemLinkValue() const{ return 0; }
	virtual const GClass::GemLinkConT* GetGemLink() const { return nullptr; }
	virtual enCD_GemSlotColor GetItemGemColor() const{ return kGSC_Invalid; }
	virtual bool HasThreeColorLink() const{ return false; };
	//装备到身上位置
	enCD_InventoryType Equip2BodyPos() const;
	bool SetItemPos(enCD_InventoryType dst_inventory_type, const stCD_VecInt& coor);

protected:
	template<typename ThisT, typename PackageT>
	static enMoveItemToRes MoveToImpl(ThisT* ptr_this, PackageT& dst, const stCD_VecInt& pos){
		assert(ptr_this);
		auto package = ptr_this->GetPackage();
		if (package)
		{
			auto mov_res = package->MoveItToThere(*ptr_this, dst, pos);
			if (mov_res != kMITR_MoveToThereFailed)
				return mov_res;
		}
		return dst.MoveItToHere(*ptr_this, pos);
	}
	virtual int WeightImpl() const;

private:
	virtual enInsideGemRes DoInsideGem(int gem_idx, const GItemGem& gem);
	bool Init(stCD_EntityItemBased& item_entity, pt_dword item_id, const GPackageBasePtr& package, stCD_ItemGridsInfo* grid_info);

protected:
	bool UpdateImpl() override;

protected:
	stCD_EntityItemBased*	item_entity_;		//物品对象

protected:
	BasePro					astrict_;

private:
	GPackageBaseWeakPtr		package_;
	enCD_InventoryType		item_inventory_type_;
	std::string				item_name_;
	pt_dword				item_id_;
	pt_dword				item_cnt_;			//物品数量
	pt_dword				max_stack_cnt_;		//最大堆叠数量
	enCD_ItemColor			item_color_;		//颜色
	enCD_ItemType			item_type_;			//物品类型
	enItemCate				item_cate_;
	stSizeInt				item_size_;
	stCD_ItemGridsIdxInfo 	item_idx_info_;		//位置信息
	SellPriceCont			price_;				//买价
	GItemTipsPanel			item_tips_;
	mutable WeightType		weight_;
	bool					is_identified_;		//是否鉴定
	bool					is_bind_;
};

class GItemWearable : public GItemBase{
protected:
	GItemWearable(enItemCate item_cate);

public:
	enMoveItemToRes MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const override;
	enPutItemDownRes PutDownTo(GPackageBase& dst, const stCD_VecInt& pos) const override;
	GPackageBasePtr GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const override;
};

class GItemEquipBase : public GItemWearable{
protected:
	GItemEquipBase(enItemCate item_cate);

public:
	bool IsEquip() const override;
	enMoveItemToRes MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const override;
	//返回-1 表示该颜色没有可用的插槽.
	int FindFreeSlot(const enCD_GemSlotColor _color) const override;
	//有空槽子返回空槽子位置，没有返回同色槽子位置， 其他返回-1
	int FindColorSlot(const enCD_GemSlotColor _color) const;
	int GetGemIndexByName(const std::string& gem_name) override;
	enCD_GemSlotColor GetColorByIndex(int index) override;
	int GetSlotCnt() const override{ return gem_solt_.size(); }
	pt_dword GetMaxGemLinkValue() const override;
	const GClass::GemLinkConT* GetGemLink() const override;
	const stItemSlot* FindSlotByIdx(int idx) const;
	void GetAllPackGem(GItemContT& all_gem) const override;
	void GetInvalidGem(GemEquipConT& all_gem) const override;
	bool HasThreeColorLink() const override;
	inline const Cont_Slot& GetSlot() const{ return gem_solt_; }

public:
	enTakeoffGemRes TakeoffGemTo(int gem_idx, GPackageBase& dst, const stCD_VecInt& pos) const override;

protected:
	bool UpdateImpl() override;
	int WeightImpl() const override;
	void DebugInfo() const override;

private:
	enInsideGemRes DoInsideGem(int gem_idx, const GItemGem& gem) override;
	bool IsHasEqualColorSlot();
	int GetSlotTheColorCnt(enCD_GemSlotColor color) const;
	int GetGemCnt() const;

private:
	Cont_Slot				gem_solt_;
	GClass::GemLinkConT		gem_link_;
};
//////////////////////////////////////////////////////////////////////////
class GItemMgrBase{
public:
	GItemBasePtr CreateItem(stCD_EntityItemBased& item_entity, pt_dword item_id, const GPackageBasePtr& package, stCD_ItemGridsInfo* grid_info) const;

private:
	enItemCate CalcItemCate(stCD_EntityItemBased& item_entity) const;
};

class GPackageBase : public GItemMgrBase, public GGameObjBase, public boost::enable_shared_from_this<GPackageBase>{
	typedef std::bitset<CD_kBagItemArraySideSize * CD_kBagItemArraySideSize> GridsState;
	friend class GPackageMain;
	friend class GPackageWearable;

public:
	typedef std::vector<stCD_VecInt> ItemPosCont;

public:
	GPackageBase(enCD_InventoryType inventory_type);
	bool Init(stCD_UiGridPanel* ui_grid_panel, stCD_Inventory* inventory);
	GItemBasePtr FindItemByPos(const stCD_VecInt& _pos) const;
	GItemBasePtr FindItemByName(const std::string& item_name) const;
	GItemBasePtr FindItemById(pt_dword item_id) const;
	GItemBasePtr FindByWeightInfo(const GItemBasePtr& src_item) const;
	pt_dword GetItemCnt() const;
	pt_dword GetItemCntByType(enCD_ItemType type) const;
	stCD_VecInt FindFreePosBySize(const stSizeInt& size, const IFilterPos* filter_pos = nullptr) const;			//指定大小的位置
	stCD_VecInt FindFreePos(const GItemBase& item, const IFilterPos* filter_pos = nullptr) const;
	stSizeInt FindMaxSpaceByPos(int _x, int _y) const;		//指定位置的大小	
	stSizeInt FindMaxSpaceByPos(const stCD_VecInt& pos) const;
	GItemBasePtr FindCanOverlayItem(const GItemBase& item) const;
	GItemBasePtr FindItemByIndex(int index) const;
	bool CanPutDownItem(const GItemBase& item) const;
	bool CanPutDownItem(const stCD_VecInt& pos, const GItemBase& item) const;
	void WeightSort(bool by_good);
	GItemBasePtr GetFirstItem() const;
	bool IdentifiedOneItem(const GItemBase& item) const;
	int CheckSum() const;
	virtual const GFlaskSpeedCont* GetFlaskSpeedCont() const;
	virtual const IFilterPos* GetRetainFilterPos(const GItemBase* item) const;
	virtual bool PressPage() const;
	virtual bool AdjustPack();
	virtual bool GoodThanAll(const GItemBase& item) const;
	virtual stCD_VecInt GetOneCanPutPos(const GItemFlask& item) const;
	
	//宝石相关
	int FindFreeGemIndex(const enCD_GemSlotColor gem_color) const;	
	void GetAllPackGem(GItemContT& all_gem) const;
	void GetGoodItems(const GIFilterItem& filter, GItemStaticContT& out_items) const;
	void DebugInfo() const override;

public:
	inline pt_dword GetLastPosId()const { return last_pos_id_; }
	inline void SetLastPosId(pt_dword id){ last_pos_id_ = id; }
	inline enCD_InventoryType GetPackageType() const{ return package_type_; }
	inline GItemContT& GetItems() const{ return items_; }
	inline const stSizeInt& GetPackageSize() const { return package_size_; }
	inline stCD_UiGridPanel* GetUiGridPanel() const { return game_grid_panel_; }

	//迭代器
public:
	template<typename IterContTag_, typename IterFilterTag_, typename... Args>
	GItemIteratorPtr CreateIterator(const Args&... args) const{
		typedef GItemIteratorTraitsT<IterContTag_, IterFilterTag_> IterTraits;
		return MakeIterator<IterTraits>(items_, args...);
	}
	template<typename IterContTag_>
	GItemIteratorPtr FilterIterator(const GIFilterItem& filter) const{
		return CreateIterator<IterContTag_, IterFilterTag_Filter>(filter);
	}
	template<typename IterContTag_, typename FilterT_>
	GItemIteratorPtr SafeFilterIterator(const FilterT_& filter) const{
		return CreateIterator<IterContTag_, IterFilterTag_SafeFilter>(filter);
	}
	template<typename IterContTag_, typename... Args>
	GItemIteratorPtr CurstomIterator(const Args&... args) const{
		return CreateIterator<IterContTag_, IterFilterTag_Custom>(args...);
	}

public:
	virtual enMoveItemToRes MoveItToThere(const GItemBase& item, GPackageBase& dst, const stCD_VecInt& dst_pos);
	virtual enMoveItemToRes MoveItToHere(const GItemBase& item, const stCD_VecInt& pos);
	virtual enMoveItemToRes MoveItToHere(const GItemWearable& item, const stCD_VecInt& pos);
	virtual enMoveItemToRes MoveItToHere(const GItemEquipBase& item, const stCD_VecInt& pos);
	virtual enMoveItemToRes MoveItToHere(const GItemFlask& item, const stCD_VecInt& pos);
	virtual enMoveItemToRes MoveItToHere(const GItemRing& item, const stCD_VecInt& pos);
	virtual enMoveItemToRes MoveItToHere(const GItemWeapon& item, const stCD_VecInt& pos);
	virtual enPutItemDownRes PutDownToHere(const GItemBase& item, const stCD_VecInt& pos);
	virtual enPutItemDownRes PutDownToHere(const GItemWearable& item, const stCD_VecInt& pos);
	virtual enPutItemDownRes PutDownToHere(const GItemWeapon& item, const stCD_VecInt& pos);
	virtual enPutItemDownRes PutDownToHere(const GItemFlask& item, const stCD_VecInt& pos);
	virtual enPutItemDownRes PutDownToHere(const GItemRing& item, const stCD_VecInt& pos);

protected:
	bool UpdateImpl() override;
	GItemBasePtr GetPosItem(const stCD_VecInt& _pos) const;
	//查找空位置坐标
	bool NextNullPos(stCD_VecInt& beginPos) const;
	enPutItemDownRes PutDownToHereImpl(const GItemBase& item, const stCD_VecInt& pos);
	enMoveItemToRes MoveItToHereImpl(const GItemBase& item, const stCD_VecInt& pos);
	enMoveItemToRes MoveItToHereDirect(const GItemBase& item, const stCD_VecInt& pos);
	virtual enMoveItemToRes OnMoveItToThereFailed(GPackageBase& dst_package, const GItemBase& item, const stCD_VecInt& dst_pos);
	virtual enMoveItemToRes OnMoveItToThereFailed(GPackageWearable& dst_package, const GItemBase& item, const stCD_VecInt& dst_pos);

private:
	//更新单个物品格子状态
	bool UpdateItemCellState(const GItemBase& _item);
	void SetGridState(int x, int y, bool state);
	virtual enMoveItemToRes NotifyMoveItToHereFailed(GPackageBase& src_package, const GItemBase& item, const stCD_VecInt& dst_pos);
	bool GetGridState(int x, int y) const;
	bool GetGridState(const stCD_VecInt& pos) const;

protected:
	stCD_UiGridPanel*		game_grid_panel_;
	stCD_Inventory*			game_inventory_;

protected:
	enCD_InventoryType		package_type_;
	mutable GItemContT		items_;
	stSizeInt				package_size_;
	pt_dword				last_pos_id_;

private:
	GridsState				grids_state_;
};

//武器  没有宝石的
class GPackageWearable : public GPackageBase{
public:
	GPackageWearable(enCD_InventoryType inventory_type);
	enMoveItemToRes MoveItToHere(const GItemWearable& item, const stCD_VecInt& pos) override;
	enPutItemDownRes PutDownToHere(const GItemWearable& item, const stCD_VecInt& pos);

protected:
	enMoveItemToRes NotifyMoveItToHereFailed(GPackageBase& src_package, const GItemBase& item, const stCD_VecInt& dst_pos) override;

protected:
	enMoveItemToRes TakeOffTo(enCD_InventoryType dst_inventory);
};
//////////////////////////////////////////////////////////////////////////
class GPackageMgrBase{
	typedef CD_Interface::Fn_TraInventory Fn_TraInventory;

public:
	GPackageBasePtr NewPackage(enCD_InventoryType inventory_type) const;
	template<typename... Args>
	int NewPackage(PackagesContT& out_packages, enCD_InventoryType inventory_type, Args... types) const{
		FilterInventoryType filter(inventory_type, types...);
		return CreateNewSomePackages_(filter, out_packages, true);
	}
	int NewPackage(PackagesContT& out_packages, const Fn_TraInventory& fn) const;
	int NewPackage(enCD_InventoryType min, enCD_InventoryType max, PackagesContT& out_packages) const;
	GPackageIteratorPtr CreateIterator(enCD_InventoryType min, enCD_InventoryType max) const;
	template<typename IterContTag_>
	GItemIteratorPtr CreateItemIterator(enCD_InventoryType min, enCD_InventoryType max, const GIFilterItem& item_filter) const{
		auto package_iter = CreateIterator(min, max);
		if (!package_iter)
		{
			assert(false);
			return nullptr;
		}
		return GItemIteratorPtr(new GPackagesIterateItemsIter<IterContTag_>(package_iter, item_filter));
	}
	template<typename IterContTag_, typename... Args>
	GItemIteratorPtr CreateItemIterator(const GIFilterItem& item_filter, enCD_InventoryType inventory_type, Args... types) const{
		PackagesContT packages;
		NewPackage(packages, inventory_type, types...);
		return GItemIteratorPtr(new GPackagesItemsIter<IterContTag_>(packages, item_filter));
	}
	GPackageBasePtr CreateNewPackage(enCD_InventoryType inventory_type) const;
	template<typename... Args>
	int CreateNewPackage(PackagesContT& out_packages, enCD_InventoryType inventory_type, Args... types) const{
		FilterInventoryType filter(inventory_type, types...);
		return CreateNewSomePackages_(filter, out_packages, false);
	}
	int CreateNewPackage(PackagesContT& out_packages, const Fn_TraInventory& fn) const;
	int CreateNewPackage(enCD_InventoryType min, enCD_InventoryType max, PackagesContT& out_packages) const;

protected:
	GPackageBasePtr CreateNewPackageImpl(stCD_Inventory& inventory, enCD_InventoryType inventory_type) const;

private:
	GPackageBasePtr CreateNewPackage_(enCD_InventoryType inventory_type, bool need_update) const;
	int CreateNewSomePackages_(const FilterInventoryType& filter, PackagesContT& out_packages, bool need_update) const;
	int CreateNewSomePackages_(PackagesContT& out_packages, bool need_update, const Fn_TraInventory& fn) const;
};

class GMyPackages : public GGameObjMgrBase, public GPackageMgrBase{
	typedef std::map<enCD_InventoryType, GPackageBasePtr> PackageCont;

public:
	void DebugInfo() const override;

protected:
	bool RebuildAllImpl() override;

protected:
	PackageCont		packages_;
};

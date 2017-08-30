#pragma once
/*
@author zhp
@date 2017/2/5 22:37
@purpose for items
*/
#include <Common/GameObjBase.h>
#include <list>
#include <Common/UsefulFunc.h>
#include <map>
#include <functional>
#include <Common/Iterator.h>
#include <Common/RegexMore.h>
#include <boost/shared_ptr.hpp>
#include "GClass.h"
#include "GInterface.h"
#include "GameObj.h"
#include "GPackageItemBase.h"

//////////////////////////////////////////////////////////////////////////
class GPackStashMgr;
class GPackBuyMgr;
class GItemUsable : public GItemBase{
public:
	GItemUsable();

protected:
	GItemUsable(enItemCate item_cate);

public:
	bool Use() const override;
	bool UseTo(const GItemBase& dst_item) const override;

protected:
	bool UpdateImpl() override;

private:
	enCD_UsableType			usable_type_;
};

class GItemFlask : public GItemWearable{
public:
	GItemFlask();

public:
	bool IsEquip() const override;
	bool Use() const override;
	enMoveItemToRes MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const override;
	enPutItemDownRes PutDownTo(GPackageBase& dst, const stCD_VecInt& pos) const override;
	enCD_FlaskType GetFlaskType() const override;
	int GetHpMpSpeed() const;

protected:
	pt_int GetSurplusTimes() const{ return cur_charges_ / per_charges_; }
	bool UpdateImpl() override;
	int WeightImpl() const override;
	void DebugInfo() const override;

private:
	void PushValidFlaskSpeedInfo(GFlaskSpeedCont& cont) const override;
	GPackageBasePtr GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const override;
	void PrintWeight() const override;

private:
	enCD_FlaskType			flask_type_;			//药剂类型
	pt_int					flask_hp_;				//补充血量
	pt_int					flask_mp_;				//补充蓝量
	pt_int					flask_time_;			//充能时间
	pt_int					cur_charges_;			//当前充能
	pt_int					per_charges_;			//每次充能消耗
};

class GItemGem : public GItemBase{
public:
	GItemGem();

public:
	enCD_GemSlotColor GetItemGemColor() const override;
	enInsideGemRes InsideGemTo(int gem_idx, GItemBase& dst_equip) const override;
	void GetAllPackGem(GItemContT& all_gem) const override;
	bool IsMustKeepItem() const override;

protected:
	bool UpdateImpl() override;
	void DebugInfo() const override;
	
private:
	enCD_GemSlotColor	gem_color_;		//宝石颜色
	pt_dword			cur_lvl_;		//技能宝石当前等级
};

//项链、腰带
class GItemDecoration : public GItemWearable{
public:
	GItemDecoration();
	bool IsEquip() const override;

protected:
	GItemDecoration(enItemCate item_cate);
};

class GItemRing : public GItemWearable{
public:
	GItemRing();
	bool IsEquip() const override;

public:
	void PushGoodToTheResult(GItemStaticContT& out_items) override;
	enMoveItemToRes MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const override;
	enPutItemDownRes PutDownTo(GPackageBase& dst, const stCD_VecInt& pos) const override;
	GPackageBasePtr GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const override;
};

//防具(鞋子，手套 头部)
class GItemArmor : public GItemEquipBase{
public:
	GItemArmor();
};

//武器
class GItemWeapon : public GItemEquipBase{
private:
	GItemWeapon(enItemCate item_cate);

public:
	GItemWeapon();
	GPackageBasePtr GoodThanOwner(stCD_VecInt* out_pos, const PackagesThinContT& positions) const override;

public:
	enMoveItemToRes MoveTo(GPackageBase& dst, const stCD_VecInt& pos) const override;
	void PrintWeight() const override;

protected:
	int WeightImpl() const override;
};

class GItemQuest : public GItemUsable{
public:
	GItemQuest();
	bool DoFilter() const override;
	bool IsMustKeepItem() const override;
};

//////////////////////////////////////////////////////////////////////////
class GPackageMain : public GPackageBase{
	struct stItemCellState{
		GItemBasePtr	pos_info_;
	};
	typedef std::vector<stItemCellState> Cont_move;
	typedef std::map<pt_dword, Cont_move> Map_cont;

public:
	GPackageMain();

public:
	bool AdjustPack() override;
	enPutItemDownRes PutDownToHere(const GItemBase& item, const stCD_VecInt& pos) override;
	const IFilterPos* GetRetainFilterPos(const GItemBase* item) const override;
	enMoveItemToRes MoveItToHere(const GItemBase& item, const stCD_VecInt& pos) override;
	enMoveItemToRes MoveItToHere(const GItemWearable& item, const stCD_VecInt& pos) override;

private:
	GItemBasePtr FindCanMoveItem(const stCD_VecInt& coor, const stSizeInt& big);
	void InitAdjustData();
	bool MoveItemToPos(GItemBase& _item, const stCD_VecInt& _pos);
	int CalcItemLocation(int _x, int _y);
	bool CanAutoUse(const std::string& item_name) const;
	enMoveItemToRes OnMoveItToThereFailed(GPackageWearable& dst_package, const GItemBase& item, const stCD_VecInt& dst_pos) override;

private:
	Map_cont	map_item_;
};

class GPackageEquips : public GPackageWearable{
public:
	GPackageEquips(enCD_InventoryType inventory_type);
	enMoveItemToRes MoveItToHere(const GItemEquipBase& item, const stCD_VecInt& pos) override;
};

class GPackageFlasks : public GPackageWearable{
public:
	GPackageFlasks();

public:
	enMoveItemToRes MoveItToHere(const GItemBase& item, const stCD_VecInt& pos) override;
	enMoveItemToRes MoveItToHere(const GItemFlask& item, const stCD_VecInt& pos) override;
	pt_dword GetFlaskCntByType(enCD_FlaskType flask_type) const;
	bool GoodThanAll(const GItemBase& item) const override;
	stCD_VecInt GetOneCanPutPos(const GItemFlask& item) const override;

public:
	static void SetBagFlaskCnt(enCD_FlaskType type, pt_dword cnt);

protected:
	bool UpdateImpl() override;
	const GFlaskSpeedCont* GetFlaskSpeedCont() const override;

private:
	GFlaskSpeedCont	flask_speed_;
	static pt_dword flask_cnt_[kFT_Max];
};
//////////////////////////////////////////////////////////////////////////
class GPackageWeaponBase : public GPackageEquips{
protected:
	GPackageWeaponBase(enCD_InventoryType inventory_type);
	enMoveItemToRes NotifyMoveItToHereFailed(GPackageBase& src_package, const GItemBase& item, const stCD_VecInt& dst_pos) override;
	enMoveItemToRes MoveItToHere(const GItemWeapon& item, const stCD_VecInt& pos) override;

private:
	virtual enMoveItemToRes MoveItToHereSpecific(const GItemWeapon& item, const stCD_VecInt& pos);
	virtual enCD_InventoryType SwitchPosition() const = 0;
};

//左手背包
class GPackageLeft : public GPackageWeaponBase{
public:
	GPackageLeft();

private:
	enMoveItemToRes MoveItToHereSpecific(const GItemWeapon& item, const stCD_VecInt& pos) override;
	enCD_InventoryType SwitchPosition() const override;
};

//右手背包
class GPackageRight : public GPackageWeaponBase{
public:
	GPackageRight();

private:
	enCD_InventoryType SwitchPosition() const override;
};
//////////////////////////////////////////////////////////////////////////
class GInventoryMgr : public GPackageMgrBase, public Singleton<GInventoryMgr, Singleton_MakeMe>{
	friend class GPackageMain;
public:
	GInventoryMgr();

	//接口
public:
	bool UseItem(const std::string& item_name);
	bool UseItemToDst(const std::string& src_name, const std::string& dst_name);

public:
	GPackageIteratorPtr BagInventories();
	GChecksumType BagInventoriesChecksum();
	int TakeOutStorageItem(const GFilterItemCont& filter_cont, int item_cnt);
	int AutoWearSomeEquips();
	bool AutoTakeOfInvalidGem();
	bool NeedTakeupTheDropItem(const GItemBasePtr& item);
	bool AutoStoreBagItemToStore();
	bool AutoSellBagItem(const std::string& npc_name);
	bool CheckStoreEmboitementToBag();
	bool CheckStoreGemTotalQualityToBag(pt_dword total_value);
	bool AutoClearMainBagBySize(const stSizeInt& size);
	int BuyItemByNpc(const std::string& npc_name, const std::string& item_name, int cnt = 1);
	void SetMainPackageRetainSize(const stSizeInt& size);
	GPackageItemPair FindItemByName(const std::string& item_name, enCD_InventoryType type = kIT_MainBag);
	GPackageBasePtr GetPackBase(const enCD_InventoryType inventoryType);
	void SetEmboitementCnt(int cnt);
	bool UpLevelStoreMap();
	pt_dword GetStorAndMainBagFilterItemCnt(const GFilterItemCont& filter_cont) const;
	//宝石相关/////////////////////////////////////////////////////
	bool FindFreeGemSolt(enCD_GemSlotColor _solt_color, enCD_InventoryType& type, int& _index);
	bool GetAllGemSlotInfo(GClass::VcSlotInfoConT& ret_vec);
	void GetAllPackGem(GItemContT& all_gem);
	void GetEquipInvalidGem(GemEquipConT& invalid_gem) const;
	bool FindGemPosByEquip(enCD_InventoryType& type, stCD_VecInt& item_pos, int& index, const std::string& gem_name, const GAutoUpDateGem::GemDstPos& filter_gem);
	bool OpenStoreAndUpdate(bool reopen) const;
	////////////////////////////////////////////////////////////////////

private:
	bool GetEquipPackages(PackagesContT& packages, bool include_flask) const;
	const GFilterRect& GetMainBagRetainFilterPos(const GPackageMain& package);
	void GetGoodEquipsToWear(GBodyTotalEquipsCont& out_equips, GBodyFlasksCont& out_flasks, const GItemBasePtr& addition_item);
	bool GetCurEmboitementItem(const GPackStashMgr& stash_mgr, GItemContT& Emboitement_item);
	
public:
	static GClass::LackConT& GetLackItem(){ return emboitement_lack_item_; }
	static enCD_ItemType ConvertEmboitementType(enCD_ItemType type);
	static void InitEmboitement();

private:
	GFilterRect					main_bag_filter_rect_;
	stSizeInt					main_bag_retain_size_;
	int							emboitement_Cnt_;
	static GClass::LackConT		emboitement_lack_item_;
};
//////////////////////////////////////////////////////////////////////////
class GServicePage : public GPackageBase{
public:
	GServicePage();
	enMoveItemToRes MoveItToHere(const GItemBase& item, const stCD_VecInt& pos) override;
	enMoveItemToRes MoveItToThere(const GItemBase& item, GPackageBase& dst, const stCD_VecInt& dst_pos) override;
};

class GServicePageInfo : public GServicePage{
public:
	GServicePageInfo(GSwitchPagesMgrBase& mgr, const stCD_TabPageInfo* page_info);
	void reset();
	bool PressPage() const override;
	bool IsValidPage() const;
	stCD_UiObjBase* GetPageTabButton() const;
	void DebugInfo() const override;
	inline const std::string& GetIndexStr() const{ return page_index_str_; }

protected:
	bool UpdateImpl() override;

private:
	stCD_UiObjBase*				game_tab_button_;

private:
	GSwitchPagesMgrBase&		package_mgr_;
	std::string					page_index_str_;
};

class GServicePagesMgrBase : public GPackageMgrBase, public GGameObjMgrBase{
};

class GSwitchPagesMgrBase : public GServicePagesMgrBase{
	typedef boost::shared_ptr<GServicePageInfo> PackagePtr;
	typedef std::vector<PackagePtr> ContT;
public:
	GSwitchPagesMgrBase();
	void reset();
	bool PressPage(const GServicePageInfo& page) const;
	const ContT& GetPages() const;
	virtual stCD_UiGridPanel* GetGridPanel(const stCD_TabPageInfo* page_info) const = 0;
	virtual bool IsThePageShown(const stCD_TabPageInfo* page_info) const = 0;
	virtual bool ValidPageItem(const stCD_UiTabPages::PageInfo* page_info) const;
	stCD_UiTabPages* GetGameTabPages() const;
	const PackagePtr FindPackByIndexStr(const std::string& index_str) const;
	const PackagePtr FindPackBySize(const stSizeInt& size) const;
	const GItemBasePtr FindItemByAllPack(const std::string& item_name) const;
	pt_dword GetItemCntByName(const std::string& item_name) const;
	bool GetItemByFilter(const GFilterItemCont& filter_cont, GItemStaticContT& all_item) const;
	bool UpdateAllPackPage();
	void DebugInfo() const override;

protected:
	bool RebuildAllImpl() override;

protected:
	stCD_UiTabPages*		game_tab_pages_;

protected:
	ContT					pack_tab_page_;
};

class GPackStashMgr : public GSwitchPagesMgrBase, public Singleton<GPackStashMgr, Singleton_MakeMe>{
public:
	GPackStashMgr();
	bool OpenStoreAndUpdate(bool reopen);

protected:
	stCD_UiGridPanel* GetGridPanel(const stCD_TabPageInfo* page_info) const override;
	bool IsThePageShown(const stCD_TabPageInfo* page_info) const override;
	bool RebuildAllImpl() override;
	bool ValidPageItem(const stCD_UiTabPages::PageInfo* page_info) const override;
	bool CheckStashPanelOpenState() const;	

};

class GPackBuyMgr : public GSwitchPagesMgrBase{
public:
	GPackBuyMgr();

protected:
	stCD_UiGridPanel* GetGridPanel(const stCD_TabPageInfo* page_info) const override;
	bool IsThePageShown(const stCD_TabPageInfo* page_info) const override;
	bool RebuildAllImpl() override;

public:
	bool EnableUseBind(bool enable) const;

private:
#ifdef VER_GF_
	bool		cur_enable_;
#endif
};

class GPackSell : public GServicePagesMgrBase{
public:
	GPackSell();
	bool PressButton(bool accept);
	stCD_VecInt FindMyPosBySize(const stSizeInt& size) const;
	GPackageBasePtr GetSellMyPack() const;
	void DebugInfo() const override;

protected:
	bool RebuildAllImpl() override;

private:
	GPackageBasePtr		my_pack_item_;
	GPackageBasePtr		npc_pack_item_;
};

class GServiceSellPack : public GServicePage{
public:
};

class GYiJiePack : public GServicePage{
public:
	GYiJiePack(){};
	bool StartButton();

protected:
	bool UpdateImpl() override;
};

class GHideoutLocatorPack : public GServicePage{
	struct stItemInfo
	{
		std::string item_name_;
		pt_dword	item_cnt_;
	};
	struct stCraft
	{
		stCraft() : ui_item_(nullptr){}
		stCD_MapLocatorMapAttrList::stListItem* ui_item_;
		std::string								craft_name_;
		std::vector<stItemInfo>					item_info_;
	};
	typedef std::vector<stCraft>   ConT;
	
public:
	GHideoutLocatorPack(){};
	bool StartButton();
	bool Select(const std::string& craft_name);

protected:
	bool UpdateImpl() override;

private:
	ConT			craft_item_;
};

//领取
class GAwardItemPack : public GServicePage{
public:
	bool IsOpenAwardPanel() const;
	bool NpcItemToMainBagAndWear(const GItemBase& item) const;
	GItemBasePtr FindItemOrDefault(const std::string& item_name) const;
	GItemBasePtr FindItemOrDefault(int index) const;

protected:
	bool UpdateImpl() override;
	bool CanAutoUse(const std::string& item_name) const;
};
//////////////////////////////////////////////////////////////////////////

class GDbBaseItemType : public GGameObjBase{
	friend class GDBBaseItemTypeMgr;
public:
	GDbBaseItemType();
	void DebugInfo() const override;

protected:
	bool UpdateImpl() override;

private:
	stCD_TblItemBaseItemTypes* itemDb_;

private:
	std::wstring		item_name_;
};

class GDBBaseItemTypeMgr : public GGameObjMgrBase, public Singleton<GDBBaseItemTypeMgr, Singleton_MakeMe>{
	typedef std::vector<GDbBaseItemType> Cont;
public:
	GDBBaseItemTypeMgr();
	void DebugInfo() const override;
	bool RebuildAll() override;

protected:
	bool RebuildAllImpl() override;

private:
	Cont				item_db_;
};
//////////////////////////////////////////////////////////////////////////
//掉落物
class GWorldItem : public GameObjBase{
public:
	GWorldItem(enGameObjType obj_type);
	void DebugInfo() const override;
	
public:
	const GItemBasePtr& GetWorldItem() const;

private:
	bool UpdateImpl() override;
	bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res) override;
	int GetCanOpenedDistance() const override;
	bool DoFilt() const override;
	bool CanBreakOpenObj() const override;
	bool MovingDoTryOpen() const override;

private:
	stCD_EntityWorldItem*	entity_world_item_;

private:
	GItemBasePtr			item_;
};

//箱子
class GChest : public GameObjBase{
public:
	GChest(enGameObjType obj_type);
	bool IsOpened() const override{ return is_opened_; }
	void DebugInfo() const override;

private:
	bool UpdateImpl() override;
	bool OpenVisit(GOpenVisitor& visitor, bool is_pre_open, enOpenVisitorRes& open_res) override;
	int GetCanOpenedDistance() const override;

private:
	stCD_EntityChest*	entity_chest_;
	bool				is_opened_;
	bool				is_quest_;
};

////////////////////////////////////////////////////////////////////////////////
class GItemAgainRinse : public boost::enable_shared_from_this<GItemAgainRinse>{
public:
	GItemAgainRinse() : material_max_(0){}
	virtual bool Parse(const luabind::object& lua_obj, pt_dword material_cnt);
	virtual bool CheckMaterial() const;
	virtual bool Rinse(const GItemBase& item) const;

public:
	bool CheckPackMaterial() const;

public:
	virtual bool DoFilter(const GItemBase& item) const = 0;

protected:
	std::string		material_name_;
	pt_dword		material_max_;
};

class GItemColor : public GItemAgainRinse{
public:
	GItemColor();
	bool Parse(const luabind::object& lua_obj, pt_dword material_cnt) override;
	bool DoFilter(const GItemBase& item) const override;

private:
	enCD_ItemColor	item_color_;
};

class GItemSlotCnt : public GItemAgainRinse{
public:
	GItemSlotCnt();
	bool Parse(const luabind::object& lua_obj, pt_dword material_cnt) override;
	bool DoFilter(const GItemBase& item) const override;

private:
	pt_dword		slot_cnt_;
};

class GItemSlotLink : public GItemAgainRinse{
public:
	GItemSlotLink();
	bool Parse(const luabind::object& lua_obj, pt_dword material_cnt) override;
	bool DoFilter(const GItemBase& item) const override;

private:
	pt_dword	slot_link_;
};

class GItemSlotColor : public GItemAgainRinse{
	typedef std::vector<enCD_GemSlotColor> ConT;
public:
	GItemSlotColor();
	bool Parse(const luabind::object& lua_obj, pt_dword material_cnt) override;
	bool DoFilter(const GItemBase& item) const override;

private:
	ConT	slot_color_;
};

class GItemQuality : public GItemAgainRinse{
public:
	GItemQuality();
	bool Parse(const luabind::object& lua_obj, pt_dword material_cnt) override;
	bool DoFilter(const GItemBase& item) const override;

private:
	pt_dword	quality_;
};

class GItemTypeConfig : public GItemAgainRinse{
	typedef std::vector<GItemAgainRinsePtr>		ConT;
public:
	GItemTypeConfig(){}
	//装备颜色
	GItemAgainRinsePtr MakeItemColor(const luabind::object lua_obj, pt_dword material_max);
	//槽子数量
	GItemAgainRinsePtr MakeItemSlotCnt(const luabind::object lua_obj, pt_dword material_max);
	//槽子连接
	GItemAgainRinsePtr MakeItemSlotLink(const luabind::object lua_obj, pt_dword material_max);
	//槽子颜色
	GItemAgainRinsePtr MakeItemSlotColor(const luabind::object& lua_obj, pt_dword material_max);
	//品质
	GItemAgainRinsePtr MakeItemQuality(const luabind::object& lua_obj, pt_dword material_max);
	//检查材料
	bool CheckMaterial() const override;
	bool DoFilter(const GItemBase& item) const override;
	bool Rinse(const GItemBase& item) const override;

private:
	ConT				config_;
};

class GItemRinseMgr{
	struct stBaseInfo
	{
		enCD_ItemType	item_type_;
		pt_dword		min_lvl_;
		pt_dword		max_lvl_;

		bool operator < (const stBaseInfo& tmp) const
		{
			return item_type_ < tmp.item_type_;
		}
		bool operator == (const stBaseInfo& tmp) const
		{
			return item_type_ == tmp.item_type_ && min_lvl_ == tmp.min_lvl_ && max_lvl_ == tmp.max_lvl_;
		}
	};
	typedef std::map<stBaseInfo, GItemAgainRinsePtr> ConT;
public:
	static GItemAgainRinsePtr CreateItemRinseConfig(enCD_ItemType item_type, pt_dword min_lvl, pt_dword max_lvl);
	bool MeetEquip(const GItemBase& item) const;
	bool CanRinse(const GItemBase& item) const;
	bool BeginRinse(const GItemBase& item) const;

private:
	GItemAgainRinsePtr GetEquipConfig(const GItemBase& item) const;

private:
	static ConT			rinse_config_;
};

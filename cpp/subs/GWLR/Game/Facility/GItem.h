#pragma once
/*
@author zhp
@date 2014/7/1 20:40
@purpose for items
*/
#include <Common/GameObjBase.h>
#include "GTypes.h"
#include <list>
#include <Common/UsefulFunc.h>
#include <json/value.h>
#include <map>
#include <functional>
#include <Common/Iterator.h>
#include <Common/RegexMore.h>
#include "GNpc.h"
#include <boost/shared_ptr.hpp>


class ItemInfoBase{
public:
	ItemInfoBase();

protected:
	bool From(const stCD_ItemDb& item_db);

public:
	std::string				item_name_;
	int						item_lvl_;
	int						own_limit_;			//拥有上限
	int						carry_limit_;		//携带上限
	int						die_jia_limit_;		//叠加上限
	CD_En_ItemCategory		item_type_;
	CD_En_ItemType			equip_part_type_;
	CD_En_EquipPartSubType_Weapon	equip_part_sub_type_;
	int						player_lvl_;
	CD_En_ItemQuality		item_quality_;
	int						item_id_;
};
class ItemDb : public GGameObjBase, public ItemInfoBase{
public:
	//用于装备升级
	struct stEquipLvlUpInfo{
		stEquipLvlUpInfo();

		int									dst_item_id_;
		std::vector<stCD_MaterialInfo>		materials_;
	};

public:
	ItemDb();
	const stCD_MaterialInfo* FindLvlUpInfoByMaterialId( int material_item_id ) const;
	void DebugInfo() const override;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

public:
	const stCD_ItemDb*	item_db_;
	std::vector<stEquipLvlUpInfo>		equip_lvl_up_;
};

class ItemDbMgr : public GGameObjMgrBase, public Singleton<ItemDbMgr, Singleton_MakeMe>{
public:
	bool RebuildAll() override;
	const ItemDb* FindById( int item_id ) const;
	const ItemDb* FindByName( const std::string& item_name ) const;
	const ItemDb* FindByMatchName( const std::string& item_name ) const;
	const ItemDb* FindByMaterial(int material_item_id, int dst_item_lvl, CD_En_ItemQuality dst_item_quality) const;
	void DebugInfo() const override;

private:
	bool UpdateAllImpl() override;
	bool RebuildAllImpl() override;
	void DoRebuildUpdateClear() override;

private:
	std::vector<ItemDb>	items_;
};

class GItemMgrBase;
class GItem : public GGameObjBase, public ItemInfoBase{
public:
	GItem();

public:
	GType::enEquipPartIndex ItemIdx2EquipPartIdx() const;
	GType::enEquipPartIndex Equip2EquipPartIdx() const;
	bool IsEquip() const;
	CD_En_EquipPartSubType_Weapon EquipSubType2Weapon() const;
	GType::enEquipPartSubType_Defense EquipSubType2Defense() const;
	GType::enEquipPartSubType_Decorate EquipSubType2Decorate() const;
	pt_dint CalcItemGoodsValue() const;
	bool IsSuCai() const;
	bool CanBeUsed() const;
	bool IsFree() const;

public:
	const ItemDb* GetItemTemplate();

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

	//another
public:
	//装备比较
	bool EquipLessThan( const GItem& rhs ) const;
	friend bool operator == ( const GItem& lhs, const GItem& rhs );

	//test
public:
	void DebugInfo() const override;

	//static function
public:

public:
	const stCD_Item*		the_item_;
	GItemMgrBase*			item_mgr_ptr_;
	const ItemDb*		item_template_;
	int						item_idx_;			//slot_idx

public:
	pt_dint					item_serial_;
	int						sell_price_;		//出售价格
	int						item_cnt_;
	CD_En_BindFlag			bind_type_;
};

const GItem kItemInvalid;

class GItemMgrBase : public GGameObjMgrBase{
public:
	typedef std::list<GItem> ContT;
	typedef TraverseTraits<GItem&, GItem&> ItemTraTraits;
	typedef ItemTraTraits::IteratorBase GameItemIterBase;
	typedef ItemTraTraits::IteratorBasePtr GameItemIterPtr;
	typedef ItemTraTraits::DoAnyTraverse DoAnyGameItemFuncT;
	typedef ItemTraTraits::DoEveryTraverse DoEveryGameItemFuncT;
	typedef ItemTraTraits::DoCompFuncT DoCompFuncT;
	typedef std::vector<GItem*> ItemPtrContT;
	typedef GObjFilter<GItem> ItemFilterT;
	typedef ItemFilterT::SelfPtrT ItemFilterPtrT;

	//等级与背包格子之间的映射
	struct stLevel2InventoryMaxCnt{
		typedef std::map<std::pair<int, int>, int> MapT;
		typedef MapT::value_type ValueType;
		//由等级得到格子最大数
		int GetInventoryMaxCnt( int lvl ) const;

		MapT				map_;
	};

public:
	GItemMgrBase();
	void SetTabCategory(CD_En_TabCategory tab_category);
	CD_En_TabCategory GetTabCategory() const;
	ContT& GetItems();
	const ContT& GetItems() const;
	//得到空项
	virtual int GetFreeSlotIndex() const;
	//得到最大格子数
	int GetGridMaxCnt() const;
	//得到空闲数
	int GetFreeCnt() const;
	//所有物品都是紧挨着的
	bool AllItemNextTo() const;
	//背包满了
	bool PacketBeFulled() const;
	GItem* FindByName( const std::string& item_name, bool is_bind = true );
	GItem* FindByName_Cnt( const std::string& item_name, bool is_bind = true, int item_cnt = 1 );
	GItem* FindByMatchName( const std::string& item_name, bool is_bind = true );
	GItem* FindById( int item_id );
	int GetTheItemTotalCnt( int item_id );
	int GetTheItemTotalCnt( const std::string& item_name );
	template<typename T>
	void AppendItems(T& the_items){
		std::copy(items_.begin(), items_.end(), std::back_inserter(the_items));
	}

	//遍历和查找
public:
	//条件遍历
	GameItemIterPtr TraverseIf( const DoAnyGameItemFuncT& func );
	//支持模糊匹配
	GameItemIterPtr TraverseByMatchName( const std::string& item_name );
	GameItemIterPtr TraverseByMatchName_Cnt( const std::string& item_name, int cnt );
	GameItemIterPtr TraverseByName( const std::string& item_name );
	GameItemIterPtr TraverseByItemId( int item_id );
	//查找第一个
	GItem* FindIf( const DoAnyGameItemFuncT& func );
	GItem* FindBySlotIndex( int slot_index );
	GItem* FindByName_Lvl( const std::string& item_name, int item_lvl );
	GItem* FindBySerial( const LONGLONG& item_serial );

	//物品过滤
public:

protected:
	bool UpdateAllImpl() override;
	bool RebuildAllImpl() override;
	void DoRebuildUpdateClear() override;

	//test
public:
	void DebugInfo() const override;

protected:
	CD_En_TabCategory		tab_category_;
	ContT					items_;	
	ItemFilterPtrT			trade_filter_;

protected:
	int						grids_max_cnt_;

protected:
	stCD_GridsMgr*			grids_mgr_;
};

//背包物品
class GItemMgr_Package : public GItemMgrBase{
public:
	GItemMgr_Package();
	//得到空项，从大往小
	int GetFreeSlotIndex() const;

private:
	bool RebuildAllImpl() override;
};

//装备物品
class GItemMgr_Equip : public GItemMgrBase{
public:
	GItemMgr_Equip();

private:
	bool RebuildAllImpl() override;
};

//仓库物品
class GItemMgr_Store : public GItemMgrBase{
public:
	GItemMgr_Store();

private:
	bool RebuildAllImpl() override;
};

//素材仓库
class GItemMgr_SuCai : public GItemMgrBase{
public:
	GItemMgr_SuCai();

private:
	bool RebuildAllImpl() override;
};

class GItemMgr_Task : public GItemMgrBase{
public:
	GItemMgr_Task();

private:
	bool RebuildAllImpl() override;
};

//邮件
class GMailItem : public GGameObjBase{
public:
	struct stItem{
		int		item_id_;
		int		item_cnt_;
	};

public:
	GMailItem();

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

public:
	stCD_MailInfo*		mail_;

public:
	std::string			mail_title_;
	pt_dint				mail_serial_;
	std::vector<stItem>	items_;
	int					gold_;
};

class GMailMgr : public GGameObjMgrBase, public Singleton<GMailMgr, Singleton_MakeMe>{
public:
	const GMailItem* FindBySerial( const pt_dint& mail_serial );
	const std::vector<GMailItem>& GetMails() const;

private:
	bool RebuildAllImpl() override;
	bool UpdateAllImpl() override;

private:
	std::vector<GMailItem>		mails_;
};

//调和
class GManufactureDbMgr : public GGameObjBase, public Singleton<GManufactureDbMgr, Singleton_MakeMe>{
public:
	typedef std::vector<stCD_MaterialInfo> MaterialContT;
	struct stItemFactureDb{
		int				facture_id_;		//制作ID
		int				dst_item_id_;
		std::string		dst_item_name_;
		MaterialContT	materials_;
	};
	typedef std::vector<stItemFactureDb> ItemsFactureContT;

public:
	bool Update() override;
	const stItemFactureDb* FindByMaterial(int material_item_id, int dst_item_lvl, CD_En_ItemQuality dst_item_quality) const;
	const stItemFactureDb* FindByDstItemName( const std::string& dst_item_name ) const;
	void DebugInfo() const override;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

public:
	ItemsFactureContT		items_facture_info_;
};

//补给箱
class GSupplyBox : public GGameObjBase, public Singleton<GSupplyBox, Singleton_MakeMe>{
public:
	struct stSupplyItemInfo{
		int		slot_key_;
		int		item_id_;
		int		item_cnt_;
	};
	typedef std::vector<stSupplyItemInfo> ItemsInfoContT;

public:
	void DebugInfo() const override;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

public:
	ItemsInfoContT		items_info_;
};

//礼包
class GGiftItem : public GClass::BTreeBase{
public:
	GGiftItem();
	void DebugInfo() const override;
	bool CanGain() const;

private:
	bool UpdateImpl() override;
	void DoUpdateClear() override;

public:
	int				gift_bag_id_;
	int				flag_;
	std::string		gifg_name_;
};

class GGiftItemMgr : public GClass::BTreeMgr_SkipRoot<GGiftItem>, public Singleton<GGiftItemMgr, Singleton_MakeMe>{
public:
	void DebugInfo() const override;
	const GGiftItem* FindGift( int id ) const;

private:
	bool RebuildAllImpl() override;
};
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
#include "GAddr.h"
#include <boost/shared_ptr.hpp>

//物品模板
class GItemTemplate : public GClass::BTreeBase{
public:
	struct stEquipInfo{
		stEquipInfo();

		GType::enEquipPart		equip_part_;
		int						max_durability_;
		bool					need_identified_;		//需要被鉴定
		bool					need_sealed_;			//需要被解封
	};
	typedef boost::optional<stEquipInfo> EquipInfoOptional;
	struct stUsableInfo{
		stUsableInfo();
		bool LevelInRange( int item_lvl ) const;

		bool					can_use_in_village_;
		int						target_item_lvl_lower_;		//用于另一物品，另一物品需高于等于该等级
		int						target_item_lvl_upper_;		//用于另一物品，另一物品需低于等于该等级
		GType::enItemUsableType	item_usable_type_;
	};
	typedef boost::optional<stUsableInfo> UsableInfoOptional;

public:
	GItemTemplate();
	bool Update();
	//用于鉴定
	bool UseForIdentify() const;
	//用于解封
	bool UseForUnseal() const;
	std::wstring GetChName() const;

public:
	DWORD					item_hash_id_;
	std::wstring			item_id_;
	std::wstring			equip_category_;
	int						item_level_;			//装备等级，角色等级大于等于它才能穿
	int						max_stack_count_;		//最大叠加数量
	int						max_possession_limit_;	//携带数量上限
	bool					can_sell_to_shop_;
	GType::enTabCategory	inventory_tab_;
	EquipInfoOptional		equip_info_;
	UsableInfoOptional		usable_info_;
	std::wstring			item_name_;
	std::wstring			market_category_;
};

class GItemMgr;
class GItem POD_TO_CLASS_UNWIND(: public GGameObjBase){
public:
	typedef char (GameItemT)[JFZR_REVERSAL_ITEM_OBJ_SIZE___];
	struct stGameItemsPtr{
		stGameItemsPtr();

		GameItemT*			item_begin_;
		GameItemT*			item_end_;
		DWORD				unknown1_;
		DWORD				unknown2_;
	};
	//可穿戴的装备类别
	class ItemStatusValue : public GClass::BTreeBase{
	public:
		struct GameType{
			NODE				str_theNode;
			GClass::stGameString		str_theStatusId;
			float				str_fValue;
		};
		static_assert(sizeof(GameType) == sizeof(ITEM_STATUS_VALUE), "");

	public:
		ItemStatusValue();
		bool Update();
		float GetTheValue() const;
		const std::wstring& GetStatusId() const;

	private:
		std::wstring	status_id_;
		float			the_value_;
	};
	class ItemStatusValueMgr : public GClass::BTreeMgr_SkipRoot<ItemStatusValue>{
	public:
		const ItemStatusValue* FindByStatusId( const std::wstring& status_id ) const;
	};

public:
	GItem(){
		game_item_addr_ = nullptr;
		item_durable_ = 0;
		item_cnt_ = 0;
		is_sealed_ = false;
		unidentified_ = false;
		item_mgr_ = nullptr;
		slot_index_ = -1;
		tradable_ = false;
		item_hash_id_ = 0;
		item_template_ = nullptr;
		item_serial_ = 0;
		item_quality_ = GType::kIQ_Invalid;
	}
	bool ConverToJson( Json::Value& out_json ) const{
		out_json["name"] = WideCharToGbk(item_id_);
		out_json["qual"] = item_quality_;
		out_json["dura"] = (Json::Int)item_durable_;
		out_json["cnt"] = (Json::Int)item_cnt_;
		out_json["seal"] = is_sealed_;
		out_json["ident"] = unidentified_;
		out_json["tab"] = (Json::Int)GetCurTabCategory();
		return true;
	}
	bool ParseFromJson( const Json::Value& json_v ){
		const auto& json_name = json_v["name"];
		if ( json_name.isNull() || !json_name.isString() )
			goto LABLE_ERR;
		const auto& json_qual = json_v["qual"];
		if ( json_qual.isNull() || !json_qual.isIntegral() )
			goto LABLE_ERR;
		const auto& json_dura = json_v["dura"];
		if ( json_dura.isNull() || !json_dura.isIntegral() )
			goto LABLE_ERR;
		const auto& json_cnt = json_v["cnt"];
		if ( json_cnt.isNull() || !json_cnt.isIntegral() )
			goto LABLE_ERR;
		const auto& json_seal = json_v["seal"];
		if ( json_seal.isNull() || !json_seal.isBool() )
			goto LABLE_ERR;
		const auto& json_ident = json_v["ident"];
		if ( json_ident.isNull() || !json_ident.isBool() )
			goto LABLE_ERR;
		const auto& json_tab = json_v["tab"];
		item_id_ = GbkToWideChar(json_name.asString());
		item_quality_ = (GType::enItemQuality)json_qual.asInt();
		item_durable_ = json_dura.asInt();
		item_cnt_ = json_cnt.asInt();
		is_sealed_ = json_seal.asBool();
		unidentified_ = json_ident.asBool();
		game_item_addr_ = (GameItemT*)json_tab.asInt();
		return true;
LABLE_ERR:
		assert(false);
		return false;
	}
	void SetGameItemAddr( GameItemT* game_item_addr );
	void SetItemMgr( GItemMgr* item_mgr );

	//get
public:
	int GetItemCnt() const;
	//得到在tab category中的索引
	int GetSlotIndex() const;
	void SetSlotIndex( int slot_index );
	const std::wstring& GetItemId() const;
	GItemMgr* GetParent() const;
	bool NeedSeal() const;
	bool NeedIdentification() const;
	int GetDurable() const;
	bool IsTradable() const;
	DWORD GetItemHashId() const;
	const GItemTemplate* GetItemTemplate() const;
	const LONGLONG& GetItemSerial() const;
	//品质权重
	int QualityWeight() const;
	int GetItemLevel() const;
	GType::enItemQuality GetItemQuality() const;
	int GetItemQualityValue() const;
	const std::wstring& GetItemName() const;
	std::wstring GetItemCnName() const;
	const ItemStatusValueMgr& GetItemStatusValueMgr() const;

	//物品类别相关
public:
	//是否是装备
	bool IsEquip() const;
	//装备当前所属tab类别
	GType::enTabCategory GetCurTabCategory() const;
	//装备原始所属tab类别
	GType::enTabCategory GetSrcTabCategory() const;
	GType::enEquipPart GetEquipPart() const;
	//若是装备的话，从身上到背包-普通之间的切换
	GType::enTabCategory EquipTabCategorySwitch() const;
	//角色是否能够穿得上
	bool RoleCanEquip() const;
	//是否是可使用的
	bool IsUsable() const;

	//发包相关
public:
	//使用物品用于另一物品
	bool UseForSync( GItem& target_item );
	//卖物品
	bool Sell( const GameObjBase::SelfPtrT& npc );
	//分解物品
	bool Disassembly() const;
	//鉴定
	bool Identify( GItem& target_item );
	//解封
	bool Unseal( GItem& target_item );

public:
	bool Update();
	bool UpdateSync();
	//更新物品数量
	bool UpdateItemCnt();
	bool UpdateItemSerailCnt();
	bool UpdateItemSerailCntSync();

	//another
public:
	//装备比较
	bool EquipLessThan( const GItem& rhs ) const;

	//static function
public:

protected:
	GameItemT*				game_item_addr_;
	std::wstring			item_id_;				//物品名称/ID
	GType::enItemQuality	item_quality_;			//物品品质
	int						item_durable_;			//耐久度
	int						item_cnt_;				//物品数量
	bool					is_sealed_;				//是否封印状态
	bool					unidentified_;			//是否需要鉴定
	bool					tradable_;				//是否可以交易
	GItemMgr*				item_mgr_;
	int						slot_index_;			//物品索引
	DWORD					item_hash_id_;
	const GItemTemplate*	item_template_;
	LONGLONG				item_serial_;			//物品序列号
	ItemStatusValueMgr		item_status_value_;
};

class GItemMgr : public GGameObjMgrBase{
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
	GItemMgr();
	void SetTabCategory( GType::enTabCategory tab_category );
	GType::enTabCategory GetTabCategory() const;
	void SetItemsAddrInfo( const GItem::stGameItemsPtr* items_ptr );
	ContT& GetItems();
	const ContT& GetItems() const;
	//得到空项
	int GetFreeSlotIndex() const;
	//得到最大格子数
	int GetGridMaxCnt() const;
	//背包满了
	bool PacketBeFulled() const;
	//tab name
	const std::wstring& GetTabName() const;

	//遍历和查找
public:
	//条件遍历
	GameItemIterPtr TraverseIf( const DoAnyGameItemFuncT& func );
	//查找第一个
	GItem* FindIf( const DoAnyGameItemFuncT& func );
	GItem* FindByItemId( const std::wstring& item_id );
	GItem* FindByChName( const std::wstring& item_ch_name );
	GItem* FindSubChName( const std::wstring& item_sub_ch_name );
	GItem* FindBySerial( const LONGLONG& item_serial );
	const GItem* FindBySlotIndex( int slot_index ) const;
	GItem* FindByEquipPart( GType::enEquipPart equip_part );
	bool GenOrderIf( ItemPtrContT& out_cont, const DoAnyGameItemFuncT& if_func, const DoCompFuncT& comp_func );
	bool GenItemsIf( ItemPtrContT& out_cont, const DoAnyGameItemFuncT& if_func );
	//寻找能够鉴定该装备的鉴定书
	GItem* FindIdentifyItemToIdentify( const GItem& target_item );
	//寻找能够解封该装备的解封石
	GItem* FindUnsealItemToUnseal( const GItem& target_item );

	//物品过滤
public:
	void InitItemFilter();
	void ClearItemFilters();
	bool TryTradeFilter( GItem& item ) const;
	bool DoTradeItemFilter();
	bool AddTradeFilterHandler( const luabind::object& filter_func, const luabind::object& handler_func );

public:
	bool UpdateAll();
	bool RebuildUpdateAll();
	bool RebuildUpdateAllSync();

protected:
	GItem::stGameItemsPtr	items_ptr_;
	GType::enTabCategory	tab_category_;
	ContT					items_;	
	const stLevel2InventoryMaxCnt*		inventory_max_cnt_;
	GClass::TimerSyncExec	timer_sync_exec_;
	ItemFilterPtrT			trade_filter_;
};

//普通栏
class GItemMgr_Normal : public GItemMgr{
public:
	//生成适合穿戴的装备
	bool GenSuitableEquip( ItemPtrContT& out_cont, int item_lvl, GType::enEquipPart equip_part );
};

//普通栏
class GItemMgr_Consume : public GItemMgr{
public:
	//打开箱子
	bool OpenItem( const std::wstring& main_item_ch_name, const std::wstring& requre_item_ch_name );
	bool OpenItemA( const std::string& main_item_ch_name, const std::string& requre_item_ch_name );
	//使用物品
	bool UseItem( const std::wstring& item_ch_name );
};

//所有物品
class GItemTemplateMgr : public GClass::BTreeMgr_SkipRoot<GItemTemplate>, public Singleton<GItemTemplateMgr, Singleton_MakeMe>{
public:
	bool RebuildUpdateAll();
	bool UpdateAll();

public:
	const GItemTemplate* FindById( const std::wstring& item_id ) const;
	const GItemTemplate* FindByHashId( DWORD item_hash_id ) const;
};

//商品模板
class GShopTemplate : public GClass::BTreeBase{
public:
	bool Update();
	const std::wstring& GetNpcId() const;

private:
	std::wstring			npc_id_;
};

class GShopTemplageMgr : public GClass::BTreeMgr_SkipRoot<GShopTemplate>, public Singleton<GShopTemplageMgr, Singleton_MakeMe>{
public:
	bool RebuildUpdateAll();
	bool UpdateAll();

public:
	const GShopTemplate* FindById( const std::wstring& npc_id ) const;
};
#include "stdafx.h"
#include "GItem.h"
#include <Common/LogOut.h>
#include "GOffset.h"
#include "GClass.h"
#include <Common/MemOper.h>
#include "GConst.h"
#include <boost/regex.hpp>
#include "GPlayer.h"
#include <Protocol/P_Defines.h>
#include "GAddr.h"
#include "DataStructAdapter.h"
#include "GMap.h"

//占位符
struct stLevel2Inventory_Placeholder : public GItemMgr::stLevel2InventoryMaxCnt{
	stLevel2Inventory_Placeholder(){
		using namespace std;
		map_.insert(ValueType(make_pair(1, kRoleMaxLevel), 3 * 8));
	}
};
stLevel2Inventory_Placeholder l2i_placeholder;

GItemMgr::GItemMgr()
{
	tab_category_ = GType::kTC_Invalid;
	inventory_max_cnt_ = &l2i_placeholder;
}

void GItemMgr::SetItemsAddrInfo( const GItem::stGameItemsPtr* items_ptr )
{
	if ( !items_ptr )
	{
		assert(false);
		return;
	}
	items_ptr_ = *items_ptr;
}

bool GItemMgr::UpdateAll()
{
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

bool GItemMgr::RebuildUpdateAll()
{
	if ( !items_ptr_.item_begin_ || !items_ptr_.item_end_ || items_ptr_.item_end_ < items_ptr_.item_begin_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "items_ptr is empty";
		assert(false);
		return false;
	}
	ContT items_tmp;
	GItem item_tmp;
	int slot_index = 0;
	for ( auto item_begin_tmp = items_ptr_.item_begin_; item_begin_tmp < items_ptr_.item_end_; ++item_begin_tmp, ++slot_index )
	{
		item_tmp.SetGameItemAddr(item_begin_tmp);
		if ( !item_tmp.UpdateItemCnt() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "更新物品数量信息失败";
			assert(false);
			return false;
		}
		if ( item_tmp.GetItemCnt() <= 0 )
			continue;
		if ( !item_tmp.Update() )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "更新item失败";
			assert(false);
			return false;
		}
		item_tmp.SetItemMgr(this);
		item_tmp.SetSlotIndex(slot_index);
		items_tmp.push_back(item_tmp);
	}
	items_tmp.swap(items_);
	return true;
}

GItemMgr::ContT& GItemMgr::GetItems()
{
	return items_;
}

const GItemMgr::ContT& GItemMgr::GetItems() const
{
	return items_;
}

GType::enTabCategory GItemMgr::GetTabCategory() const
{
	return tab_category_;
}

/*
int GItemMgr::GetItemSlotIndex( const GItem& item )
{
	const GItem* item_ptr = &item;
	auto it_begin = items_.begin(), it_end = items_.end();
	auto it = std::find_if(it_begin, it_end, [item_ptr]( const GItem& gi ){
		return &gi == item_ptr;
	});
	if ( it == it_end )
		return -1;
	return (int)std::distance(it_begin, it);
}*/

int GItemMgr::GetFreeSlotIndex() const
{
	const auto kMaxGridCnt = GetGridMaxCnt();
	if ( kMaxGridCnt <= 0 )
	{
		assert(false);
		return -1;
	}
	assert(kMaxGridCnt >= items_.size());
	int slot_idx = 0;
	for ( auto it_item = items_.begin(); it_item != items_.end(); ++it_item, ++slot_idx )
	{
		if ( it_item->GetSlotIndex() != slot_idx )
			return slot_idx;
	}
	return -1;
}

GItemMgr::GameItemIterPtr GItemMgr::TraverseIf( const DoAnyGameItemFuncT& func )
{
	return MakeIterator<ItemTraTraits>(items_, func);
}

GItem* GItemMgr::FindIf( const DoAnyGameItemFuncT& func )
{
	for ( auto& item : items_ )
	{
		if (func(item))
			return &item;
	}
	return nullptr;
}

GItem* GItemMgr::FindByItemId( const std::wstring& item_id )
{
	if (item_id.empty())
		return nullptr;
	for ( auto& item : items_ )
	{
		if ( item.GetItemId() == item_id )
			return &item;
	}
	return nullptr;
}

int GItemMgr::GetGridMaxCnt() const
{
	auto player_self = GPlayerSelf::GetInstance();
	if ( !player_self )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "player self is empty";
		assert(false);
		return -1;
	}
	auto role_lvl = player_self->GetLevel();
	if ( role_lvl >= 200 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "人物等级大于200级";
		assert(false);
		return -1;
	}
	return inventory_max_cnt_->GetInventoryMaxCnt(role_lvl);
}

bool GItemMgr::PacketBeFulled() const
{
	const auto kMaxGridCnt = GetGridMaxCnt();
	if ( kMaxGridCnt <= 0 )
	{
		assert(false);
		return false;
	}
	assert(kMaxGridCnt >= items_.size());
	return kMaxGridCnt == items_.size();
}

const std::wstring& GItemMgr::GetTabName() const
{
	if ( GType::kTC_Normal == tab_category_ )
		return kTabName_Normal;
	else if ( GType::kTC_Consume == tab_category_ )
		return kTabName_Consume;
	else if ( GType::kTC_Material == tab_category_ )
		return kTabName_Materail;
	return kEmptyWStr;
}

const GItem* GItemMgr::FindBySlotIndex( int slot_index ) const
{
	if ( slot_index < 0 )
		return nullptr;
	for ( const auto& item : items_ )
	{
		if ( item.GetSlotIndex() == slot_index )
			return &item;
	}
	return nullptr;
}

bool GItemMgr::GenOrderIf( ItemPtrContT& out_cont, const DoAnyGameItemFuncT& if_func, const DoCompFuncT& comp_func )
{
	for ( auto& item : items_ )
	{
		if (if_func(item))
			out_cont.push_back(&item);
	}
	if (out_cont.empty())
		return false;
	std::sort(out_cont.begin(), out_cont.end(), [&comp_func]( GItem* lhs, GItem* rhs ){
		assert(lhs);
		assert(rhs);
		return comp_func(*lhs, *rhs);
	});
	return true;
}

bool GItemMgr::RebuildUpdateAllSync()
{
	return timer_sync_exec_.TimerGWndExecSync([this](){
		return RebuildUpdateAll();
	}, 60);
}

bool GItemMgr::GenItemsIf( ItemPtrContT& out_cont, const DoAnyGameItemFuncT& if_func )
{
	for ( auto& item : items_ )
	{
		if (if_func(item))
			out_cont.push_back(&item);
	}
	if (out_cont.empty())
		return false;
	return true;
}

struct stGameAppleItem{
	DWORD				str_dwVFunAddr;
	GClass::stGameString		str_theItemId;					// 物品id
	GAME_STRING_ID_		str_thePrefixOptionName;		// 物品名称
	LONGLONG			str_llSerial;					// 物品序号
	DWORD				str_dwItemObjAddr;				// 物品模板对象
	DWORD				str_dwItemObjPoint;				// 物品模板对象指针对象
	int					str_dwStackCount;				// 物品存在数量
	GClass::stGameString		str_theGradeId;					// 物品品质
	FLOAT				str_fDurability;				// 物品耐久
	GAME_STRUCT_ARRAY_POINT		str_theIncreaseStatusList;		// +74 装备属性
	GAME_STRUCT_ARRAY_POINT		str_theEventOptions;			// +84
	GAME_STRUCT_ARRAY_POINT		str_theIdentifiedOptions;		// +94 鉴定属性
	DWORD						str_dwUnKnow1;
	struct
	{
		DWORD					str_dwVFunAddr;
		DWORD					str_dwVersion;
		DWORD					str_dwUnBindCount;
		DWORD					str_dwReidentifyCount;
		DWORD					str_dwEventOptionChangeCount;
		DWORD					str_dwAddBonusLevel;
		DWORD					str_dwExpirationTime[4];
		GAME_STRUCT_ARRAY_POINT	str_theMasteryStatusIds;
		DWORD					str_dwRebalanceVersion;		// 10.30新增数据段
	}theExtendData;
	DWORD						str_dwUnKnow2;		// 10.30新增未知数据
	bool						str_bIsSealed;
	bool						str_bIsUnidentified;
	bool						str_bIsTradable;
	int							str_dwGoldValue;
	GClass::BTreeMgrBase::stGBTreeNodes				str_theStatus;					// +E8	装备基本属性
	BINARY_TREE_MAP				str_theEnhanceFactors;
	// ...
};
static_assert(sizeof(stGameAppleItem) == sizeof(APPLE_ITEM), "APPLE_ITEM");

bool GItem::Update()
{
	using namespace GClass;
	using namespace GOffset;
	using namespace GFuncCall;
	using namespace GType;
	if ( !game_item_addr_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "game item addr empty";
		assert(false);
		return false;
	}
	auto apple_item = (const stGameAppleItem*)game_item_addr_;
	item_cnt_ = apple_item->str_dwStackCount;
	assert(item_cnt_ >= 0);
	if ( item_cnt_ <= 0 )
		return false;
	if ( !apple_item->str_theItemId.CopyTo(item_id_) )
	{
		assert(false);
		return false;
	}
	assert(!item_id_.empty());
	item_durable_ = (int)apple_item->str_fDurability;
	assert(item_durable_ >= 0);
	assert(0 == apple_item->str_bIsUnidentified || 1 == apple_item->str_bIsUnidentified);
	assert(0 == apple_item->str_bIsSealed || 1 == apple_item->str_bIsSealed);
	assert(0 == apple_item->str_bIsTradable || 1 == apple_item->str_bIsTradable);
	is_sealed_ = apple_item->str_bIsSealed;
	unidentified_ = apple_item->str_bIsUnidentified;
	tradable_ = apple_item->str_bIsTradable;
	item_serial_ = apple_item->str_llSerial;
	assert(item_serial_ > 0);
	item_hash_id_ = CalcHash_DWORD(item_id_);
	assert(item_hash_id_);
	auto& item_template_mgr = GItemTemplateMgr::GetMe();
	item_template_ = item_template_mgr.FindByHashId(item_hash_id_);
	if ( !item_template_ )
	{
		item_template_ = item_template_mgr.FindById(item_id_);
		if ( !item_template_ )
		{
			LOG_O(Log_debug) << "can not find item template,item id->" << item_id_ << "\thash id->" << item_hash_id_ << "\titem template total cnt->" << item_template_mgr.GetAllNodes().size();
			assert(false);
			return false;
		}		
	}
	if ( item_template_->item_id_ != item_id_ )
	{
		assert(false);
		return false;
	}
	if (apple_item->str_theGradeId.EqualTo(kItemQuality_Low))
		item_quality_ = kIQ_Low;
	else if (apple_item->str_theGradeId.EqualTo(kItemQuality_Middle))
		item_quality_ = kIQ_Middle;
	else if (apple_item->str_theGradeId.EqualTo(kItemQuality_High))
		item_quality_ = kIQ_High;
	else if (apple_item->str_theGradeId.EqualTo(kItemQuality_Rare))
		item_quality_ = kIQ_Rare;
	else if (apple_item->str_theGradeId.EqualTo(kItemQuality_Legendary))
		item_quality_ = kIQ_Legendary;
	else
	{
		assert(false);
		return false;
	}
	item_status_value_.SetGameNodes(&apple_item->str_theStatus);
	if ( !item_status_value_.RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	return true;
}

void GItem::SetGameItemAddr( GameItemT* game_item_addr )
{
	using namespace GOffset;
	game_item_addr_ = game_item_addr;
}

int GItem::GetItemCnt() const
{
	return item_cnt_;
}

bool GItem::UpdateItemCnt()
{
	using namespace GClass;
	using namespace GOffset;
	if ( !game_item_addr_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "game item addr empty";
		assert(false);
		return false;
	}
	item_cnt_ = ((const stGameAppleItem*)game_item_addr_)->str_dwStackCount;
	assert(item_cnt_ >= 0);
	return true;
}

void GItem::SetItemMgr( GItemMgr* item_mgr )
{
	item_mgr_ = item_mgr;
}

int GItem::GetSlotIndex() const
{
	return slot_index_;
}

void GItem::SetSlotIndex( int slot_index )
{
	slot_index_ = slot_index;
}

bool GItem::IsEquip() const
{
	if ( !item_template_ )
	{
		assert(false);
		return false;
	}
	return item_template_->equip_info_;
}

GType::enEquipPart GItem::GetEquipPart() const
{
	if ( !IsEquip() )
	{
		//assert(false);
		return GType::kEP_Invalid;
	}
	return item_template_->equip_info_->equip_part_;
}

const std::wstring& GItem::GetItemId() const
{
	return item_id_;
}

GType::enTabCategory GItem::EquipTabCategorySwitch() const
{
	using namespace GType;
	if ( !IsEquip() )
		return kTC_Invalid;
	auto tab_category = GetCurTabCategory();
	if ( kTC_Normal == tab_category )
		return kTC_Equiped;
	else if ( kTC_Equiped == tab_category )
		return kTC_Normal;
	else if ( kTC_Avatar == tab_category )
		return kTC_AvatarEquiped;
	else if ( kTC_AvatarEquiped == tab_category )
		return kTC_Avatar;
	assert(false);
	return kTC_Invalid;
}

GItemMgr* GItem::GetParent() const
{
	return item_mgr_;
}

bool GItem::UseForSync( GItem& target_item )
{
	auto tab_category_src = GetSrcTabCategory();
	if ( GType::kTC_Consume != tab_category_src )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "使用物品源，竟然不是消耗品";
		assert(false);
		return false;
	}
	auto tab_category_dst = target_item.GetSrcTabCategory();
	if ( GType::kTC_Normal != tab_category_dst )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "目标物品竟然不属于kTC_Normal";
		assert(false);
		return false;
	}
	if ( item_cnt_ <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品数量不对，无法使用, 数量->" << item_cnt_;
		return false;
	}
	if ( !item_template_ )
	{
		assert(false);
		return false;
	}
	if ( !item_template_->usable_info_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品没有可使用信息,item id->" << item_id_;
		return false;
	}
	if ( GMap::GetMe().IsTown() && !item_template_->usable_info_->can_use_in_village_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品不能在城镇中使用,item id->" << item_id_;
		return false;
	}
	if ( !item_template_->usable_info_->LevelInRange(target_item.GetItemLevel()) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "物品等级不符，等级范围:(" << item_template_->usable_info_->target_item_lvl_lower_ << ","
			<< item_template_->usable_info_->target_item_lvl_upper_ << "),目标等级->" << target_item.GetItemLevel();
		return false;
	}
	/*if ( !item_mgr_ )
	{
		assert(false);
		return false;
	}*/

	const auto item_cnt_old = item_cnt_;
	const auto item_serial_old = item_serial_;

	CMDS_Msg_UseForItem msg;
	msg.using_slot_ = GetSlotIndex();
	msg.target_slot_ = target_item.GetSlotIndex();
	auto& tcp_send = GClass::TcpSend::GetMe();
	if ( !tcp_send.SendMsg(msg) )
	{
		assert(false);
		return false;
	}
	if ( !TimerDoWaitUntil([=](){
		if ( item_serial_ != item_serial_old )
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "物品序列号变了";
			return true;
		}
		if ( item_cnt_ != item_cnt_old )
		{
			if ( item_cnt_ != item_cnt_old - 1 )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_warning) << "物品数量不对，之前->" << item_cnt_old << "\t之后->" << item_cnt_;
			}
			return true;
		}
		return false;
	}, [this](){
		UpdateItemSerailCntSync();
	}, 50, 10 * 1000) )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "使用物品等级超时失败, item serial id->";
		return false;
	}
	if ( !target_item.UpdateSync() )
	{
		assert(false);
		return false;
	}
	/*if ( !item_mgr_->RebuildUpdateAllSync() )
	{
		assert(false);
		return false;
	}*/
	return true;
}

bool GItem::Sell( const GameObjBase::SelfPtrT& npc )
{
	using namespace GType;
	if ( !npc )
	{
		assert(false);
		return false;
	}
	if ( kGOT_Npc != npc->GetGameObjType() )
	{
		assert(false);
		return false;
	}
	const auto& npc_name = npc->GetObjName();
	if (npc_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "npc名字为空";
		assert(false);
		return false;
	}
	auto tab_category = GetCurTabCategory();
	if ( GType::kTC_Invalid == tab_category )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "can not sell,tab category is invalid";
		assert(false);
		return false;
	}
	auto slot_index = GetSlotIndex();
	if ( slot_index < 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "slot index is invalid, can not sell";
		assert(false);
		return false;
	}
	if ( item_cnt_ <= 0 )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "item count is invalid, can not sell";
		assert(false);
		return false;
	}
	CMDS_Msg_SellItem msg;
	msg.fixed_npc_name_ = npc_name;
	msg.tab_category_ = tab_category;
	msg.slot_index_ = slot_index;
	msg.item_cnt_ = item_cnt_;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GItem::Disassembly() const
{
	auto tab_category = GetSrcTabCategory();
	if ( GType::kTC_Normal != tab_category )
	{
		assert(false);
		return false;
	}
	if ( slot_index_ < 0 )
	{
		assert(false);
		return false;
	}
	CMDS_Msg_DisassemblyItem msg;
	msg.tab_category_ = tab_category;
	msg.slot_index_ = slot_index_;
	auto& tcp_send = GClass::TcpSend::GetMe();
	return tcp_send.SendMsg(msg);
}

bool GItem::EquipLessThan( const GItem& rhs ) const
{
	return QualityWeight() < rhs.QualityWeight();
}

bool GItem::NeedSeal() const
{
	return is_sealed_;
}

bool GItem::NeedIdentification() const
{
	return unidentified_;
}

int GItem::GetDurable() const
{
	return item_durable_;
}

bool GItem::IsTradable() const
{
	return tradable_;
}

GType::enTabCategory GItem::GetCurTabCategory() const
{
	using namespace GType;
	if ( !item_mgr_ )
	{
		assert(false);
		return kTC_Invalid;
	}
	return item_mgr_->GetTabCategory();
}

DWORD GItem::GetItemHashId() const
{
	if ( !item_template_ )
	{
		assert(false);
		return 0;
	}
	return item_template_->item_hash_id_;
}

GType::enTabCategory GItem::GetSrcTabCategory() const
{
	if ( !item_template_ )
	{
		assert(false);
		return GType::kTC_Invalid;
	}
	return item_template_->inventory_tab_;
}

bool GItem::RoleCanEquip() const
{
	if ( !IsEquip() )
		return false;
	auto class_info_template = GPlayerSelf::GetMe().GetClassInfoTemplate();
	if ( !class_info_template )
	{
		assert(false);
		return false;
	}
	return class_info_template->CanEquipTheCategory(item_template_->equip_category_);
}

const GItemTemplate* GItem::GetItemTemplate() const
{
	return item_template_;
}

const LONGLONG& GItem::GetItemSerial() const
{
	return item_serial_;
}

int GItem::QualityWeight() const
{
	if ( !item_template_ )
	{
		assert(false);
		return -1 * 10000000;
	}
	const auto kLvlWeight = 4;
	auto quality_weight = item_template_->item_level_ * kLvlWeight;
	if ( item_durable_ <= 5 )
	{
		if ( item_durable_ <= 0 )
		{
			assert(0 == item_durable_);
			quality_weight -= kLvlWeight * 5;
		}
		else
			quality_weight -= kLvlWeight * 10;
	}
	quality_weight += item_quality_ * kLvlWeight * 5;
	if (item_template_->equip_info_)
	{
		quality_weight += kLvlWeight * 3 * !!item_template_->equip_info_->need_identified_;
		quality_weight += kLvlWeight * 4 * !!item_template_->equip_info_->need_sealed_;
	}
	return quality_weight;
}

int GItem::GetItemLevel() const
{
	if ( !item_template_ )
	{
		assert(false);
		return 0;
	}
	return item_template_->item_level_;
}

bool GItem::UpdateItemSerailCntSync()
{
	return GWndExecSync([this](){
		return UpdateItemSerailCnt();
	});
}

bool GItem::UpdateItemSerailCnt()
{
	using namespace GClass;
	using namespace GOffset;
	if ( !game_item_addr_ )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "game item addr empty";
		assert(false);
		return false;
	}
	auto apple_item = ((const stGameAppleItem*)game_item_addr_);
	item_cnt_ = apple_item->str_dwStackCount;
	assert(item_cnt_ >= 0);
	item_serial_ = apple_item->str_llSerial;
	assert(item_serial_ >= 0);
	return true;
}

bool GItem::IsUsable() const
{
	if ( !item_template_ )
	{
		assert(false);
		return false;
	}
	return item_template_->usable_info_;
}

bool GItem::UpdateSync()
{
	return GWndExecSync([this](){
		return Update();
	});
}

bool GItem::Identify( GItem& target_item )
{
	if ( !target_item.NeedIdentification() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不需要鉴定,item id->" << target_item.GetItemId();
		return false;
	}
	if ( !UseForSync(target_item) )
		return false;
	if (target_item.NeedIdentification())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "鉴定完了，目标就不应该需要鉴定了，item id->" << target_item.GetItemId();
		return false;
	}
	return true;
}

bool GItem::Unseal( GItem& target_item )
{
	if ( !target_item.NeedSeal() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不需要解封,item id->" << target_item.GetItemId();
		return false;
	}
	if ( !UseForSync(target_item) )
		return false;
	if (target_item.NeedSeal())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "解封完了，目标就不应该需要解封了，item id->" << target_item.GetItemId();
		return false;
	}
	return true;
}

GType::enItemQuality GItem::GetItemQuality() const
{
	return item_quality_;
}

int GItem::GetItemQualityValue() const
{
	return item_quality_;
}

const std::wstring& GItem::GetItemName() const
{
	if ( !item_template_ )
	{
		assert(false);
		return kEmptyWStr;
	}
	return item_template_->item_name_;
}

std::wstring GItem::GetItemCnName() const
{
	if ( !item_template_ )
	{
		assert(false);
		return kEmptyWStr;
	}
	return item_template_->GetChName();
}

const GItem::ItemStatusValueMgr& GItem::GetItemStatusValueMgr() const
{
	return item_status_value_;
}

GItem::stGameItemsPtr::stGameItemsPtr()
{
	item_begin_ = nullptr;
	item_end_ = nullptr;
	unknown1_ = 0;
	unknown2_ = 0;
	static_assert(sizeof(*this) == 0x10, "");
}

//一般性的
struct stLevel2Inventory_General : public GItemMgr::stLevel2InventoryMaxCnt{
	stLevel2Inventory_General(){
		using namespace std;
		map_.insert(ValueType(make_pair(1, 19), 3 * 8));
		map_.insert(ValueType(make_pair(20, 39), 4 * 8));
		map_.insert(ValueType(make_pair(40, 59), 5 * 8));
		map_.insert(ValueType(make_pair(60, kRoleMaxLevel), 6 * 8));
	}
};

//装备
struct stLevel2Inventory_Equiped : public GItemMgr::stLevel2InventoryMaxCnt{
	stLevel2Inventory_Equiped(){
		using namespace std;
		map_.insert(ValueType(make_pair(1, kRoleMaxLevel), 9));
	}
};

//时装123
struct stLevel2Inventory_Avatar : public GItemMgr::stLevel2InventoryMaxCnt{
	stLevel2Inventory_Avatar(){
		using namespace std;
		map_.insert(ValueType(make_pair(1, kRoleMaxLevel), 6 * 8/* * 3*/));
	}
};

//时装
struct stLevel2Inventory_AvatarEquiped : public GItemMgr::stLevel2InventoryMaxCnt{
	stLevel2Inventory_AvatarEquiped(){
		using namespace std;
		map_.insert(ValueType(make_pair(1, kRoleMaxLevel), 9));
	}
};

void GItemMgr::SetTabCategory( GType::enTabCategory tab_category )
{
	tab_category_ = tab_category;
	static const stLevel2Inventory_General l2i_general;
	static const stLevel2Inventory_Equiped l2i_equip;
	static const stLevel2Inventory_Avatar l2i_avatar;
	static const stLevel2Inventory_AvatarEquiped l2i_avatar_equiped;
	switch (tab_category)
	{
	case GType::kTC_Normal:
	case GType::kTC_Consume:
	case GType::kTC_Task:
	case GType::kTC_Material:
		inventory_max_cnt_ = &l2i_general;
		break;
	case GType::kTC_Avatar:
		inventory_max_cnt_ = &l2i_avatar;
		break;
	case GType::kTC_Equiped:
		inventory_max_cnt_ = &l2i_equip;
		break;
	case GType::kTC_AvatarEquiped:
		inventory_max_cnt_ = &l2i_avatar_equiped;
		break;
	default:
		assert(false);
		inventory_max_cnt_ = &l2i_placeholder;
		break;
	}
}

GItem* GItemMgr::FindByEquipPart( GType::enEquipPart equip_part )
{
	for ( auto& item : items_ )
	{
		if ( item.GetEquipPart() == equip_part )
			return &item;
	}
	return nullptr;
}

GItem* GItemMgr::FindBySerial( const LONGLONG& item_serial )
{
	for ( auto& item : items_ )
	{
		if ( item.GetItemSerial() == item_serial )
			return &item;
	}
	return nullptr;
}

GItem* GItemMgr::FindIdentifyItemToIdentify( const GItem& target_item )
{
	auto item_lvl = target_item.GetItemLevel();
	if ( item_lvl <= 0 )
	{
		assert(false);
		return nullptr;
	}
	return FindIf([item_lvl]( const GItem& item ){
		if ( !item.IsUsable() )
			return false;
		if ( item.GetItemCnt() <= 0 )
		{
			assert(0 == item.GetItemCnt());
			return false;
		}
		auto item_template = item.GetItemTemplate();
		assert(item_template);
		assert(item_template->usable_info_);
		if ( !item_template->UseForIdentify() )
			return false;
		if ( !item_template->usable_info_->LevelInRange(item_lvl) )
			return false;
		return true;
	});
}

GItem* GItemMgr::FindUnsealItemToUnseal( const GItem& target_item )
{
	auto item_lvl = target_item.GetItemLevel();
	if ( item_lvl <= 0 )
	{
		assert(false);
		return nullptr;
	}
	return FindIf([item_lvl]( const GItem& item ){
		if ( !item.IsUsable() )
			return false;
		if ( item.GetItemCnt() <= 0 )
		{
			assert(0 == item.GetItemCnt());
			return false;
		}
		auto item_template = item.GetItemTemplate();
		assert(item_template);
		assert(item_template->usable_info_);
		if ( !item_template->UseForUnseal() )
			return false;
		if ( !item_template->usable_info_->LevelInRange(item_lvl) )
			return false;
		return true;
	});
}

void GItemMgr::ClearItemFilters()
{
	trade_filter_.reset();
}

bool GItemMgr::TryTradeFilter( GItem& item ) const
{
	if (!item.IsTradable())
		return false;
	if ( !trade_filter_ )
		return false;
	return trade_filter_->TryFilter(item);
}

void GItemMgr::InitItemFilter()
{
	assert(!trade_filter_);
	trade_filter_.reset(new ItemFilterT);
}

bool GItemMgr::AddTradeFilterHandler( const luabind::object& filter_func, const luabind::object& handler_func )
{
	if ( !trade_filter_ )
		return false;
	return trade_filter_->AddLuaFilterHandler(filter_func, handler_func);
}

bool GItemMgr::DoTradeItemFilter()
{
	if ( !trade_filter_ )
	{
		assert(false);
		return false;
	}
	auto items_tmp = items_;
	for ( auto& item : items_tmp )
	{
		if (item.IsTradable())
			trade_filter_->FilterDoObj(item);
	}
	return true;
}

GItem* GItemMgr::FindByChName( const std::wstring& item_ch_name )
{
	if (item_ch_name.empty())
		return nullptr;
	for ( auto& item : items_ )
	{
		if ( item.GetItemCnName() == item_ch_name )
			return &item;
	}
	return nullptr;
}

GItem* GItemMgr::FindSubChName( const std::wstring& item_sub_ch_name )
{
	if (item_sub_ch_name.empty())
		return nullptr;
	for ( auto& item : items_ )
	{
		const auto& item_full_name = item.GetItemCnName();
		auto cnt_pos = item_full_name.find(item_sub_ch_name);
		if ( std::string::npos != cnt_pos )
			return &item;
	}
	return nullptr;
}

int GItemMgr::stLevel2InventoryMaxCnt::GetInventoryMaxCnt( int lvl ) const
{
	for ( const auto& m_v : map_ )
	{
		//开闭区间[]
		auto& lvl_range = m_v.first;
		if ( lvl >= lvl_range.first && lvl <= lvl_range.second )
			return m_v.second;
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "该等级不在等级范围之内，等级->" << lvl;
	assert(false);
	return -1;
}

bool GItemMgr_Normal::GenSuitableEquip( ItemPtrContT& out_cont, int item_lvl, GType::enEquipPart equip_part )
{
	if ( item_lvl < 1 )
	{
		assert(false);
		return false;
	}
	out_cont.clear();
	if ( !GenOrderIf(out_cont, [=]( GItem& item ){
		using namespace GType;
		if ( item.GetEquipPart() != equip_part )
			return false;
		if ( item.GetItemTemplate()->item_level_ > item_lvl )
			return false;
		if ( !item.RoleCanEquip() )
			return false;
		if (item.IsTradable())
		{
			if ( item.GetItemQuality() >= GType::kIQ_Rare )
				return false;
		}
		/*if (item.NeedIdentification())
			return false;
		if (item.NeedSeal())
			return false;*/
		if (TryTradeFilter(item))
			return false;
		return true;
	}, []( const GItem& lhs, const GItem& rhs ){
		return rhs.EquipLessThan(lhs);
	}) )
		return false;
	assert(!out_cont.empty());
	return true;
}

bool GItemTemplateMgr::RebuildUpdateAll()
{
	using namespace GAddr;
	if ( !btree_.empty() )
		return true;
	SetGameNodes((const stGBTreeNodes*)g_pItemTemplate);
	if ( !__super::RebuildUpdateAll() )
	{
		assert(false);
		return false;
	}
	assert(!btree_.empty());
	return true;
}

bool GItemTemplateMgr::UpdateAll()
{
	if ( !btree_.empty() )
		return true;
	return __super::UpdateAll();
}

const GItemTemplate* GItemTemplateMgr::FindById( const std::wstring& item_id ) const
{
	using namespace GFuncCall;	
	auto item_template = FindByHashId(CalcHash_DWORD(item_id));
	if ( !item_template )
		return nullptr;
	if ( item_template->item_id_ != item_id )
	{
		assert(false);
		return nullptr;
	}
	return item_template;
	/*for ( const auto& item : btree_ )
	{
		if ( item.item_id_ == item_id )
			return &item;
	}
	return nullptr;*/
}

const GItemTemplate* GItemTemplateMgr::FindByHashId( DWORD item_hash_id ) const
{
	for ( const auto& item : btree_ )
	{
		if ( item.item_hash_id_ == item_hash_id )
			return &item;
	}
	return nullptr;
}

bool GItemTemplate::Update()
{
	using namespace GType;
	using namespace GFuncCall;
	typedef GClass::SdbIndex<stSdbItemTemplate> stSdbIndex;
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	auto db_index = (const stSdbIndex*)game_cur_node_;
	const auto item_template = db_index->real_obj_;
	if ( !item_template )
	{
		assert(false);
		return false;
	}
	assert(db_index->str_hash_id_ > 0);
	item_hash_id_ = db_index->str_hash_id_;
	if ( !item_template->str_theId.game_str_.CopyTo(item_id_) )
	{
		assert(false);
		return false;
	}
	assert(!item_id_.empty());
	assert(item_hash_id_ == CalcHash_DWORD(item_id_));
	assert(item_template->str_dwItemLevel > 0);
	item_level_ = item_template->str_dwItemLevel;
	assert(item_template->str_dwMaxStackCount > 0);
	max_stack_count_ = item_template->str_dwMaxStackCount;
	assert(item_template->str_dwMaxPossessionLimit > 0);
	max_possession_limit_ = item_template->str_dwMaxPossessionLimit;
	assert(0 == item_template->str_bCanSellTotheShop || 1 == item_template->str_bCanSellTotheShop);
	can_sell_to_shop_ = item_template->str_bCanSellTotheShop;
	if ( -1 == item_template->str_dwInventoryTab )
	{
		inventory_tab_ = kTC_Invalid;
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "inventory tab == -1, item id->" << item_id_;
	}
	else
	{
		assert(item_template->str_dwInventoryTab >= kTC_Normal && item_template->str_dwInventoryTab <= kTC_Avatar);
		inventory_tab_ = item_template->str_dwInventoryTab;
	}	
	assert(0 == item_template->theEquipInfo.str_bEnable || 1 == item_template->theEquipInfo.str_bEnable);
	if (item_template->theEquipInfo.str_bEnable)
	{
		//assert(item_level_ == item_template->theEquipInfo.str_dwLimitLevel[0]);
		stEquipInfo equip_info_tmp;
		assert(item_template->theEquipInfo.str_dwEquipPart >= kEP_Weapon && item_template->theEquipInfo.str_dwEquipPart < kEP_Invalid);
		equip_info_tmp.equip_part_ = item_template->theEquipInfo.str_dwEquipPart;
		assert(kEP_A_HasNo != equip_info_tmp.equip_part_);
		equip_info_tmp.max_durability_ = (int)item_template->theEquipInfo.str_fMaxDurability;
		assert(equip_info_tmp.max_durability_ > 0);
		assert(0 == item_template->theEquipInfo.str_bIdentified || 1 == item_template->theEquipInfo.str_bIdentified);
		equip_info_tmp.need_identified_ = item_template->theEquipInfo.str_bIdentified;
		assert(0 == item_template->theEquipInfo.str_bSealed || 1 == item_template->theEquipInfo.str_bSealed);
		equip_info_tmp.need_sealed_ = item_template->theEquipInfo.str_bSealed;
		equip_info_ = equip_info_tmp;
	}
	assert(0 == item_template->theUsableInfo.str_bEnable || 1 == item_template->theUsableInfo.str_bEnable);
	if (item_template->theUsableInfo.str_bEnable)
	{
		stUsableInfo usable_info_tmp;
		assert(0 == item_template->theUsableInfo.str_bCanUseInVillage || 1 == item_template->theUsableInfo.str_bCanUseInVillage);
		usable_info_tmp.can_use_in_village_ = item_template->theUsableInfo.str_bCanUseInVillage;
		usable_info_tmp.target_item_lvl_lower_ = item_template->theUsableInfo.str_dwTargetLimitItemLevel[0];
		usable_info_tmp.target_item_lvl_upper_ = item_template->theUsableInfo.str_dwTargetLimitItemLevel[1];
		assert(usable_info_tmp.target_item_lvl_lower_ >= 0);
		assert(usable_info_tmp.target_item_lvl_upper_ >= 0);
		assert(usable_info_tmp.target_item_lvl_lower_ <= usable_info_tmp.target_item_lvl_upper_);
		if (item_template->theUsableInfo.str_theCooldownCategory.EqualTo(kItemCooldownCategory_Identify))
			usable_info_tmp.item_usable_type_ = kIUT_Identify;
		else if (item_template->theUsableInfo.str_theCooldownCategory.EqualTo(kItemCooldownCategory_Unseal))
			usable_info_tmp.item_usable_type_ = kIUT_Unseal;
		usable_info_ = usable_info_tmp;
	}
	if ( !item_template->theEquipInfo.str_theEquipCategory.CopyTo(equip_category_) )
	{
		assert(false);
		return false;
	}
	if ( !item_template->str_theName.game_str_.CopyTo(item_name_) )
	{
		assert(false);
		return false;
	}
	if ( !item_template->str_theMarketCategory.CopyTo(market_category_) )
	{
		assert(false);
		return false;
	}
	return true;
}

GItemTemplate::GItemTemplate()
{
	item_hash_id_ = 0;
	item_level_ = 0;
	max_stack_count_ = 0;
	max_possession_limit_ = 0;
	can_sell_to_shop_ = false;
	inventory_tab_ = GType::kTC_Invalid;
}

bool GItemTemplate::UseForIdentify() const
{
	if ( !usable_info_ )
		return false;
	return GType::kIUT_Identify == usable_info_->item_usable_type_;
}

bool GItemTemplate::UseForUnseal() const
{
	if ( !usable_info_ )
		return false;
	return GType::kIUT_Unseal == usable_info_->item_usable_type_;
}

std::wstring GItemTemplate::GetChName() const
{
	return std::move(GameEn2ChNodeMgr::GetMe().FindChByFullPath(item_name_));
}

bool GShopTemplageMgr::RebuildUpdateAll()
{
	using namespace GAddr;
	if ( !btree_.empty() )
		return true;
	SetGameNodes((const stGBTreeNodes*)g_pShopTemplate);
	return __super::RebuildUpdateAll();
}

bool GShopTemplageMgr::UpdateAll()
{
	if ( !btree_.empty() )
		return true;
	return __super::UpdateAll();
}

const GShopTemplate* GShopTemplageMgr::FindById( const std::wstring& npc_id ) const
{
	for ( const auto& shop : btree_ )
	{
		if ( shop.GetNpcId() == npc_id )
			return &shop;
	}
	return nullptr;
}

const std::wstring& GShopTemplate::GetNpcId() const
{
	return npc_id_;
}

struct stSdbShopTemplate{
	DWORD				str_dwVFunAddr;
	GClass::stGameString		str_theId;
	DWORD				str_dwShopType;
	GAME_STRUCT_ARRAY_POINT		str_theTargetVillages;
	GAME_STRING_ID				str_theArenaId;
	GAME_STRUCT_ARRAY_POINT		str_theSellItems;
	GAME_STRUCT_ARRAY_POINT		str_theSkillTrainClasses;
	BINARY_TREE_MAP				str_theCraftItems;
	DWORD						str_dwItemCategoryInfos[5];	// 未知数据类型
};
static_assert(sizeof(stSdbShopTemplate) == sizeof(SDB_SHOP_INFO), "");

bool GShopTemplate::Update()
{
	typedef GClass::SdbIndex<stSdbShopTemplate> stSdbIndex;
	if ( !game_cur_node_ )
	{
		assert(false);
		return false;
	}
	auto db_index = (const stSdbIndex*)game_cur_node_;
	const auto shop_template = db_index->real_obj_;
	if ( !shop_template )
	{
		assert(false);
		return false;
	}
	if ( !shop_template->str_theId.CopyTo(npc_id_) )
	{
		assert(false);
		return false;
	}
	return true;
}

GItemTemplate::stEquipInfo::stEquipInfo()
{
	equip_part_ = GType::kEP_Invalid;
	max_durability_ = 0;
	need_identified_ = false;
	need_sealed_ = false;
}

GItemTemplate::stUsableInfo::stUsableInfo()
{
	can_use_in_village_ = false;
	target_item_lvl_lower_ = 0;
	target_item_lvl_upper_ = 0;
	item_usable_type_ =  GType::kIUT_Invalid;
}

bool GItemTemplate::stUsableInfo::LevelInRange( int item_lvl ) const
{
	assert(item_lvl > 0);
	return item_lvl >= target_item_lvl_lower_ && item_lvl <= target_item_lvl_upper_;
}

bool GItemMgr_Consume::OpenItem( const std::wstring& main_item_ch_name, const std::wstring& requre_item_ch_name )
{
	if (main_item_ch_name.empty())
	{
		assert(false);
		return false;
	}
	auto main_item = FindSubChName(main_item_ch_name);
	if ( !main_item )
		return false;
	auto require_item = FindSubChName(requre_item_ch_name);
	CMDS_Msg_OpenItemPackage msg;
	if (require_item)
		msg.AcceptRequireUse(main_item->GetSlotIndex(), require_item->GetSlotIndex());
	else
		msg.AcceptSingleUse(main_item->GetSlotIndex());
	return GClass::TcpSend::GetMe().SendMsg(msg);
}

bool GItemMgr_Consume::OpenItemA( const std::string& main_item_ch_name, const std::string& requre_item_ch_name )
{
	if (main_item_ch_name.empty())
	{
		assert(false);
		return false;
	}
	if (requre_item_ch_name.empty())
		return OpenItem(GbkToWideChar(main_item_ch_name), kEmptyWStr);
	else return OpenItem(GbkToWideChar(main_item_ch_name), GbkToWideChar(requre_item_ch_name));
}

bool GItemMgr_Consume::UseItem( const std::wstring& item_ch_name )
{
	if (item_ch_name.empty())
	{
		assert(false);
		return false;
	}
	auto the_item = FindSubChName(item_ch_name);
	if ( !the_item )
		return false;
	CMDS_Msg_UseItem msg;
	msg.item_slot_ = the_item->GetSlotIndex();
	return GClass::TcpSend::GetMe().SendMsg(msg);
}

GItem::ItemStatusValue::ItemStatusValue()
{
	the_value_ = -1;
}

float GItem::ItemStatusValue::GetTheValue() const
{
	return the_value_;
}

bool GItem::ItemStatusValue::Update()
{
	if (IsUnValidPtr(game_cur_node_))
	{
		assert(false);
		return false;
	}
	const auto status_value_ptr = (const GameType*)game_cur_node_;
	if ( !status_value_ptr->str_theStatusId.CopyTo(status_id_) )
	{
		assert(false);
		return false;
	}
	the_value_ = status_value_ptr->str_fValue;
	return true;
}

const std::wstring& GItem::ItemStatusValue::GetStatusId() const
{
	return status_id_;
}

const GItem::ItemStatusValue* GItem::ItemStatusValueMgr::FindByStatusId( const std::wstring& status_id ) const
{
	assert(!status_id.empty());
	for ( const auto& v : GetAllNodes() )
	{
		if ( status_id == v.GetStatusId() )
			return &v;
	}
	return nullptr;
}

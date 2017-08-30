#include "stdafx.h"
#include "GPacket.h"
#include "GAddr.h"
#include "GOffset.h"
#include "GPlayer.h"

GPacket::GPacket()
{
	tab_normal_.SetTabCategory(GType::kTC_Normal);
	tab_consume_.SetTabCategory(GType::kTC_Consume);
	tab_task_.SetTabCategory(GType::kTC_Task);
	tab_material_.SetTabCategory(GType::kTC_Material);
	tab_avatar_.SetTabCategory(GType::kTC_Avatar);
	tab_equiped_.SetTabCategory(GType::kTC_Equiped);
	tab_avatar_equiped_.SetTabCategory(GType::kTC_AvatarEquiped);
}

bool GPacket::RebuildUpdateAll()
{
	using namespace GAddr;
	using namespace GOffset;
	int item_offset2_addr = 0;
	if ( !MemOperApplyR(item_offset2_addr, g_pRoleObjAddr, g_o_ItemOffset1 + g_o_ItemOffset2) || !item_offset2_addr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "read item_ofset2 failed";
		assert(false);
		return false;
	}
	GItem::stGameItemsPtr* items_ptr = nullptr;
	if ( !MemOperApplyR(items_ptr, item_offset2_addr, g_o_ItemOffset3 + g_o_ItemOffset4) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取包裹信息失败";
		assert(false);
		return false;
	}
	tab_normal_.SetItemsAddrInfo(items_ptr);
	if ( !tab_normal_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_normal RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	++items_ptr;
	tab_consume_.SetItemsAddrInfo(items_ptr);
	if ( !tab_consume_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_consume_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	++items_ptr;
	tab_task_.SetItemsAddrInfo(items_ptr);
	if ( !tab_task_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_task_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	++items_ptr;
	tab_material_.SetItemsAddrInfo(items_ptr);
	if ( !tab_material_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_material_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	++items_ptr;
	tab_avatar_.SetItemsAddrInfo(items_ptr);
	if ( !tab_avatar_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_fashion_lower_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	if ( !MemOperApplyR(items_ptr, item_offset2_addr, g_o_EquipItemOffset1 + g_o_EquipItemOffset2) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取装备信息失败";
		assert(false);
		return false;
	}
	tab_equiped_.SetItemsAddrInfo(items_ptr);
	if ( !tab_equiped_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_equip_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	++items_ptr;
	tab_avatar_equiped_.SetItemsAddrInfo(items_ptr);
	if ( !tab_avatar_equiped_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_fashion_upper_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::UpdateAll()
{
	if ( !tab_normal_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_normal update failed";
		assert(false);
		return false;
	}
	if ( !tab_consume_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_consume_ update failed";
		assert(false);
		return false;
	}
	if ( !tab_task_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_task_ update failed";
		assert(false);
		return false;
	}
	if ( !tab_material_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_material_ update failed";
		assert(false);
		return false;
	}
	if ( !tab_avatar_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_fashion_lower_ update failed";
		assert(false);
		return false;
	}
	if ( !tab_equiped_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_equip_ update failed";
		assert(false);
		return false;
	}
	if ( !tab_avatar_equiped_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_fashion_upper_ update failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::UpdateNormal()
{
	if ( !tab_normal_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_normal update failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::UpdateConsume()
{
	if ( !tab_consume_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_consume_ update failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::UpdateMaterial()
{
	if ( !tab_material_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_material_ update failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::UpdateTask()
{
	if ( !tab_task_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_task_ update failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::UpdateAvatar()
{
	if ( !tab_avatar_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_fashion_lower_ update failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::UpdateEquiped()
{
	if ( !tab_equiped_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_equip_ update failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::UpdateAvatarEquiped()
{
	if ( !tab_avatar_equiped_.UpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_fashion_upper_ update failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::RebuildUpdateNormal()
{
	using namespace GAddr;
	using namespace GOffset;
	GItem::stGameItemsPtr* items_ptr = nullptr;
	if ( !MemOperApplyR(items_ptr, g_pRoleObjAddr, g_o_ItemOffset1 + g_o_ItemOffset2, g_o_ItemOffset3 + g_o_ItemOffset4) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取包裹信息失败";
		assert(false);
		return false;
	}
	tab_normal_.SetItemsAddrInfo(items_ptr);
	if ( !tab_normal_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_normal RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::RebuildUpdateConsume()
{
	using namespace GAddr;
	using namespace GOffset;
	GItem::stGameItemsPtr* items_ptr = nullptr;
	if ( !MemOperApplyR(items_ptr, g_pRoleObjAddr, g_o_ItemOffset1 + g_o_ItemOffset2, g_o_ItemOffset3 + g_o_ItemOffset4) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取包裹信息失败";
		assert(false);
		return false;
	}
	++items_ptr;
	tab_consume_.SetItemsAddrInfo(items_ptr);
	if ( !tab_consume_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_consume_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::RebuildUpdateTask()
{
	using namespace GAddr;
	using namespace GOffset;
	GItem::stGameItemsPtr* items_ptr = nullptr;
	if ( !MemOperApplyR(items_ptr, g_pRoleObjAddr, g_o_ItemOffset1 + g_o_ItemOffset2, g_o_ItemOffset3 + g_o_ItemOffset4) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取包裹信息失败";
		assert(false);
		return false;
	}
	items_ptr += 2;
	tab_task_.SetItemsAddrInfo(items_ptr);
	if ( !tab_task_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_task_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::RebuildUpdateMaterial()
{
	using namespace GAddr;
	using namespace GOffset;
	GItem::stGameItemsPtr* items_ptr = nullptr;
	if ( !MemOperApplyR(items_ptr, g_pRoleObjAddr, g_o_ItemOffset1 + g_o_ItemOffset2, g_o_ItemOffset3 + g_o_ItemOffset4) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取包裹信息失败";
		assert(false);
		return false;
	}
	items_ptr += 3;
	tab_material_.SetItemsAddrInfo(items_ptr);
	if ( !tab_material_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_material_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::RebuildUpdateAvatar()
{
	using namespace GAddr;
	using namespace GOffset;
	GItem::stGameItemsPtr* items_ptr = nullptr;
	if ( !MemOperApplyR(items_ptr, g_pRoleObjAddr, g_o_ItemOffset1 + g_o_ItemOffset2, g_o_ItemOffset3 + g_o_ItemOffset4) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取包裹信息失败";
		assert(false);
		return false;
	}
	items_ptr += 4;
	tab_avatar_.SetItemsAddrInfo(items_ptr);
	if ( !tab_avatar_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_fashion_lower_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::RebuildUpdateEquiped()
{
	using namespace GAddr;
	using namespace GOffset;
	GItem::stGameItemsPtr* items_ptr = nullptr;
	if ( !MemOperApplyR(items_ptr, g_pRoleObjAddr, g_o_ItemOffset1 + g_o_ItemOffset2, g_o_EquipItemOffset1 + g_o_EquipItemOffset2) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取装备信息失败";
		assert(false);
		return false;
	}
	tab_equiped_.SetItemsAddrInfo(items_ptr);
	if ( !tab_equiped_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_equip_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	return true;
}

bool GPacket::RebuildUpdateAvatarEquiped()
{
	using namespace GAddr;
	using namespace GOffset;
	GItem::stGameItemsPtr* items_ptr = nullptr;
	if ( !MemOperApplyR(items_ptr, g_pRoleObjAddr, g_o_ItemOffset1 + g_o_ItemOffset2, g_o_EquipItemOffset1 + g_o_EquipItemOffset2) || !items_ptr )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "读取装备信息失败";
		assert(false);
		return false;
	}
	++items_ptr;
	tab_avatar_equiped_.SetItemsAddrInfo(items_ptr);
	if ( !tab_avatar_equiped_.RebuildUpdateAll() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "tab_fashion_upper_ RebuildUpdateAll failed";
		assert(false);
		return false;
	}
	return true;
}

GItemMgr_Normal& GPacket::GetTabNormal()
{
	return tab_normal_;
}

GItemMgr_Consume& GPacket::GetTabConsume()
{
	return tab_consume_;
}

GItemMgr& GPacket::GetTabTask()
{
	return tab_task_;
}

GItemMgr& GPacket::GetTabMaterial()
{
	return tab_material_;
}

GItemMgr& GPacket::GetTabAvatar()
{
	return tab_avatar_;
}

GItemMgr& GPacket::GetTabEquiped()
{
	return tab_equiped_;
}

GItemMgr& GPacket::GetTabAvatarEquiped()
{
	return tab_avatar_equiped_;
}

bool GPacket::AutoReplaceEquip()
{
	using namespace GType;
	auto& role = GPlayerSelf::GetMe();
	auto role_lvl = role.GetLevel();
	if ( role_lvl <= 0 )
	{
		if ( !role.Update() )
		{
			assert(false);
			return false;
		}
		role_lvl = role.GetLevel();
		if ( role_lvl <= 0 )
		{
			assert(false);
			return false;
		}
	}
	GItemMgr::ItemPtrContT items_tmp;
	boost::optional<decltype(tab_normal_)> tab_normal_tmp;
	auto& time_exec = GetGlobalTimeDuration();
	for ( int equip_part = kEP_Weapon; equip_part <= kEP_Ring; ++equip_part )
	{
		if (tab_normal_tmp)
		{
			if ( !tab_normal_tmp->GenSuitableEquip(items_tmp, role_lvl, (enEquipPart)equip_part) )
				continue;
		}
		else
		{
			if ( !tab_normal_.GenSuitableEquip(items_tmp, role_lvl, (enEquipPart)equip_part) )
				continue;
		}		
		if (items_tmp.empty())
			continue;
		
		auto weared_item = tab_equiped_.FindByEquipPart((enEquipPart)equip_part);
		GItem* item_to_wear = nullptr;
		for ( auto item : items_tmp )
		{
			assert(item);
			if (weared_item)
			{
				if (weared_item->EquipLessThan(*item))
					item_to_wear = item;
				else
				{
					/*//穿身上的应该没法鉴定吧
					if (weared_item->NeedIdentification())
					{
						GItem* item_identify = tab_consume_.FindIdentifyItemToIdentify(*weared_item);
						if ( !item_identify )
							break;
						if ( !time_exec.TimeToDoSync([=](){
							return item_identify->Identify(*weared_item);
						}) )
						{
							LOG_ENTER_FUNC;
							LOG_O(Log_error) << "鉴定失败1";
							return false;
						}
					}*/
					break;
				}
			}
			else
				item_to_wear = item;
			assert(item_to_wear);
			if (item_to_wear->NeedSeal())
			{
				GItem* item_unseal = tab_consume_.FindUnsealItemToUnseal(*item_to_wear);
				if ( !item_unseal )
				{
					item_to_wear = nullptr;
					continue;
				}
				if ( !time_exec.TimeToDoSync([=](){
					return item_unseal->Unseal(*item_to_wear);
				}) )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_error) << "解封失败";
					return false;
				}
			}
			if (item_to_wear->NeedIdentification())
			{
				GItem* item_identify = tab_consume_.FindIdentifyItemToIdentify(*item_to_wear);
				if ( !item_identify )
					break;
				if ( !time_exec.TimeToDoSync([=](){
					return item_identify->Identify(*item_to_wear);
				}) )
				{
					LOG_ENTER_FUNC;
					LOG_O(Log_error) << "鉴定失败2";
					return false;
				}
			}
			break;
		}
		if (item_to_wear)
		{
			if ( !tab_normal_tmp )
				tab_normal_tmp = tab_normal_;
			if ( !time_exec.TimeToDoSync([&role, item_to_wear](){
				return role.EquipItemSync(*item_to_wear);
			}) )
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_debug) << "自动穿戴物品失败";
				//assert(false);
				return false;
			}
		}
	}
	if ( !role.Update() )
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "更新角色信息失败";
		assert(false);
		return false;
	}
	return true;
}

GItemMgr* GPacket::FindByTabCategory( GType::enTabCategory tab_category )
{
	switch (tab_category)
	{
	case GType::kTC_Normal:
		return &tab_normal_;
	case GType::kTC_Consume:
		return &tab_consume_;
	case GType::kTC_Task:
		return &tab_task_;
	case GType::kTC_Material:
		return &tab_material_;
	case GType::kTC_Avatar:
		return &tab_avatar_;
	case GType::kTC_Equiped:
		return &tab_equiped_;
	case GType::kTC_AvatarEquiped:
		return &tab_avatar_equiped_;
	default:
		assert(false);
		return nullptr;
	}
}

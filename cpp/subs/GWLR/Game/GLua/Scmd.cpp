#include "stdafx.h"
#include "Scmd.h"
#include "Facility/GFuncCall.h"
#include "Facility/GClass.h"
#include <boost/thread/thread.hpp>
#include "FuncObjC.h"
#include "Facility/GClass.h"
#include "Facility/GameObjMgr.h"
#include "Facility/GPlayer.h"
#include "Facility/GConst.h"
#include "Facility/GMonster.h"
#include <Common/EnsureCleanup.h>
#include <dinput.h>
#include <CD_CommonFiles/Data/CD_FuncCall.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include "Facility/GRecvMsgMgr.h"


DWORD FindNpcByNameImpl(const std::string& obj_name, const std::string& obj_title)
{
	if (obj_name.empty())
	{
		assert(false);
		return kMonsterInvalidPacketId;
	}
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	auto obj = obj_mgr.FindMonsterByName_Title(obj_name, obj_title);
	if (obj)
		return obj->GetPacketId();

	auto& gpm = GPlayerMe::GetMe();
	if (gpm.IsInVillage())
	{
		boost::optional<DWORD> res;
		TimerDoWaitUntil([&res, &obj_mgr, &obj_name, &obj_title](){
			auto obj = obj_mgr.FindMonsterByName_Title(obj_name, obj_title);
			if (obj)
			{
				res = obj->GetPacketId();
				return true;
			}
			return false;
		}, [&obj_mgr](){
			obj_mgr.RebuildAll();
		}, 700, 1000 * 5);
		if (res)
			return *res;
	}
	return kMonsterInvalidPacketId;
}

DWORD FindMonsterByObjDbIdImpl(int obj_db_id)
{
	if (kInvalidIntValue == obj_db_id)
	{
		assert(false);
		return kMonsterInvalidPacketId;
	}
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	auto obj = obj_mgr.FindMonsterByObjDbId(obj_db_id);
	if (obj)
		return obj->GetPacketId();

	auto& gpm = GPlayerMe::GetMe();
	if (gpm.IsInVillage())
	{
		boost::optional<DWORD> res;
		TimerDoWaitUntil([&res, &obj_mgr, obj_db_id](){
			auto obj = obj_mgr.FindMonsterByObjDbId(obj_db_id);
			if (obj)
			{
				res = obj->GetPacketId();
				return true;
			}
			return false;
		}, [&obj_mgr](){
			obj_mgr.RebuildAll();
		}, 700, 1000 * 5);
		if (res)
			return *res;
	}
	return kMonsterInvalidPacketId;
}

DWORD FindBeatableMonsterByNameImpl(const std::string& obj_name)
{
	if (obj_name.empty())
		return kMonsterInvalidPacketId;
	if (GClass::GAmbient::GetMe().IsGameOver())
		return kMonsterInvalidPacketId;
	if (!scmd::GetMe().IsInFb())
		return kMonsterInvalidPacketId;
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	auto obj = obj_mgr.FindBeatableMonsterByName(obj_name);
	if (obj)
		return obj->GetPacketId();
	return kMonsterInvalidPacketId;
}

GMonster* FindTheBossImpl()
{
	if (GClass::GAmbient::GetMe().IsGameOver())
		return nullptr;
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	auto obj = obj_mgr.FindIf([&obj_mgr](GameObjBase& obj){
		if (kGOT_Monster != obj.GetGameObjType())
			return false;
		auto monster = obj.CastTo<GMonster>();
		if (!monster)
		{
			assert(false);
			return false;
		}
		if (!monster->IsBoss())
			return false;
		if (obj_mgr.IsDeadMonster(*monster))
			return false;
		return true;
	});
	if (obj)
		return obj->CastTo<GMonster>();
	return nullptr;
}

/*
DWORD FindNearestMonster( int the_room_num )
{
if ( !IsInFb() )
return kMonsterInvalidPacketId;
if (GClass::GAmbient::GetMe().cond_fb_game_over_lose_.CondtionTrue())
return kMonsterInvalidPacketId;
auto& obj_mgr = GameObjMgr::GetMe();
obj_mgr.RebuildAll();
auto& gpm = GPlayerMe::GetMe();
gpm.Update();

auto obj = obj_mgr.FindNearestBeatableMonster_RoomNum(gpm, the_room_num);
if (obj)
return obj->GetPacketId();
return kMonsterInvalidPacketId;
}*/

const GMapDb* GetDoTheFbInfo_Cat(const std::string& cat_fb_name)
{
	if (cat_fb_name.empty())
	{
		CMN_ASSERT(false);
		return nullptr;
	}
	auto& enterable_fb = GPlayerMe::GetMe().GetEnterableFbMgr();
	enterable_fb.RebuildAll();
	auto fb_info = enterable_fb.FindByName(cat_fb_name);
	if (!fb_info)
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_error) << "该副本不可进或副本名不对，副本名：" << cat_fb_name;*/
		return nullptr;
	}

	auto& gpm = GPlayerMe::GetMe();
	if (gpm.fb_info_cat_.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "开始获取可打的喵喵副本信息";
		if (!gpm.SendGainFbInfo(CD_En_FbType::kFT_Cat))
			return nullptr;
		if (gpm.fb_info_cat_.empty())
			return nullptr;
	}
	const auto& name_list = MakeNameListRef(gpm.fb_info_cat_);
	if (!name_list.IsIn(fb_info->map_id_))
		return nullptr;

	if (gpm.IsInFbBlackList_Cat(fb_info->map_id_))
		return nullptr;
	if (GWndExecSync([fb_info](){
		return CD_FuncCall::IsTheFbDone(fb_info->map_id_);
	}))
	{
		return nullptr;
	}
	return fb_info;
}

const GMapDb* GetDoTheFbInfo_King(const std::string& king_fb_name)
{
	if (king_fb_name.empty())
	{
		CMN_ASSERT(false);
		return nullptr;
	}
	auto& enterable_fb = GPlayerMe::GetMe().GetEnterableFbMgr();
	enterable_fb.RebuildAll();
	auto fb_info = enterable_fb.FindByName(king_fb_name);
	if (!fb_info)
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_error) << "该副本不可进或副本名不对，副本名：" << king_fb_name;*/
		return nullptr;
	}
	return fb_info;
}

/*
DWORD FindNearestCollectableObjAsSameRoom()
{
if (GClass::GAmbient::GetMe().IsGameOver())
return kMonsterInvalidPacketId;
auto& gpm = GPlayerMe::GetMe();
gpm.Update();
auto& collect_mgr = GCollectObjMgr::GetMe();
collect_mgr.RebuildAll();
auto collect_obj = collect_mgr.FindNearestAsSameRoom(gpm);
if ( !collect_obj )
return kMonsterInvalidPacketId;
return collect_obj->GetPacketId();
}*/


/*
bool CloseNpcUi()
{
return LeftButtonClick(904, 737);
}*/

bool WaitUntilShowHideCursor()
{
	using namespace GClass;
	auto& ambient = GAmbient::GetMe();
	if (!ambient.PressKeyEscWaitFor(true))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "做事失败963";
		return false;
	}
	if (!ambient.PressKeyEscWaitFor(false))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "做事失败987";
		return false;
	}
	if (!ambient.PressKeyGraveWaitFor(false))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "做事失败742";
		return false;
	}
	return true;
}

boost::optional<GMapDb> GetTaskToDoFbInfo(int task_id)
{
	boost::optional<GMapDb> fb_info;
	if (!scmd::GetMe().IsValidTaskId(task_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "任务ID参数不是有效的任务ID";
		return fb_info;
	}
	auto& task_template_mgr = GTaskDbMgr::GetMe();
	task_template_mgr.RebuildAll();
	auto task_template = task_template_mgr.FindById(task_id);
	if (!task_template)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有找到这样的任务信息，任务ID：" << task_id;
		return fb_info;
	}
	if (task_template->do_fb_ids_.Empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有要去打的副本信息1，任务ID：" << task_id << "  任务名：" << task_template->task_name_;
		return fb_info;
	}
	auto dst_fb_id = *task_template->do_fb_ids_.GetValues().begin();

	auto& all_fb_mgr = MapDbMgr::GetMe();
	all_fb_mgr.Update();
	auto the_fb_info = all_fb_mgr.FindById(dst_fb_id);
	if (!the_fb_info)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有要去打的副本信息2，任务ID：" << task_id << "  任务名：" << task_template->task_name_;
		return fb_info;
	}
	fb_info = *the_fb_info;
	return fb_info;
}

CD_En_TabCategory Num2TabCategory(int num)
{
	if (1 == num)
		return CD_En_TabCategory::kTC_Package;
	else if (2 == num)
		return CD_En_TabCategory::kTC_Store;
	else if (3 == num)
		return CD_En_TabCategory::kTC_Equip;
	else if (4 == num)
		return CD_En_TabCategory::kTC_SuCai;
	return CD_En_TabCategory::kTC_Invalid;
}

bool GainItemFromStoreImpl(GItem& item)
{
	if (scmd::GetMe().IsInFb())
		return false;
	auto& gpm = GPlayerMe::GetMe();
	/*auto& packet = gpm.GetGridPackage();
	packet.RebuildAll();*/

	/*auto dst_slot_idx = packet.GetFreeSlotIndex();
	if ( dst_slot_idx < 0 )
	{
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "从仓库无法再移动物品到背包中，因为背包已满";
	return false;
	}*/
	if (!gpm.SmartMoveItem(item, CD_En_TabCategory::kTC_Package, -1))
		return false;
	return true;
}

GType::enEquipPartIndex EquipPartIdx2EquipPartIdx(int equip_part_idx)
{
	using namespace GType;
	switch (equip_part_idx)
	{
	case 1:
		return kEPI_Weapon;
	case 2:
		return kEPI_Head;
	case 3:
		return kEPI_Heart;
	case 4:
		return kEPI_Hand;
	case 5:
		return kEPI_Pocket;
	case 6:
		return kEPI_Foot;
	case 7:
		return kEPI_Necklace;
	case 8:
		return kEPI_Ring;
	case 9:
		return kEPI_HuShi;
	default:
		return kEPI_Invalid;
	}
}

bool GetOffEquipByPartIdx(GType::enEquipPartIndex equip_part)
{
	auto& gpm = GPlayerMe::GetMe();
	auto& store_mgr = gpm.GetStorePackage();
	auto& equip_mgr = gpm.GetEquipPackage();
	equip_mgr.RebuildAll();

	auto equip_item = equip_mgr.FindBySlotIndex(equip_part);
	if (!equip_item)
		return true;
	auto equip_item_tmp = *equip_item;
	/*store_mgr.RebuildAll();
	auto free_slot = store_mgr.GetFreeSlotIndex();
	if ( free_slot < 0 )
	{
	StoreSomePackageItemsToStore(1);
	store_mgr.RebuildAll();
	free_slot = store_mgr.GetFreeSlotIndex();
	if ( free_slot < 0 )
	{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "脱下装备之背包已满，无法脱下装备";
	return false;
	}
	}*/
	return gpm.SmartMoveItem(equip_item_tmp, CD_En_TabCategory::kTC_Store, -1);
}


bool WearEquipImpl(GItem& item_tmp, GType::enEquipPartIndex equip_part)
{
	auto& gpm = GPlayerMe::GetMe();
	auto& equip_mgr = gpm.GetEquipPackage();
	equip_mgr.RebuildAll();

	auto equip_item = equip_mgr.FindBySlotIndex(equip_part);
	if (equip_item)
	{
		if (!GetOffEquipByPartIdx(equip_part))
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "穿戴装备之已穿戴了这样的装备，脱下装备失败，装备名：" << item_tmp.item_name_;
			return false;
		}
	}
	return gpm.SmartMoveItem(item_tmp, CD_En_TabCategory::kTC_Equip, equip_part);
}

void AutoWearSomeEquipsImpl(CD_En_ItemQuality quality_limit, const GItem* bestest_items[GType::kEPI_Invalid], const GItemMgrBase::ContT& items, GItemMgr_Equip& tab_equip)
{
	using namespace GType;
	auto& gpm = GPlayerMe::GetMe();
	for (auto& item : items)
	{
		auto equip_idx = item.Equip2EquipPartIdx();
		if (kEPI_Invalid == equip_idx)
			continue;
		if (item.item_quality_ > quality_limit)
			continue;
		if (gpm.IsInAutoWearBlackList(item))
			continue;
		assert(equip_idx < kEPI_Invalid);
		if (bestest_items[equip_idx])
		{
			if (bestest_items[equip_idx]->CalcItemGoodsValue() < item.CalcItemGoodsValue())
			{
				bestest_items[equip_idx] = &item;
				continue;
			}
		}
		else
		{
			auto the_equip = tab_equip.FindBySlotIndex(equip_idx);
			if (the_equip)
			{
				if (the_equip->CalcItemGoodsValue() < item.CalcItemGoodsValue())
					bestest_items[equip_idx] = &item;
			}
			else
				bestest_items[equip_idx] = &item;
		}
	}
}

bool NeedRetChooseRoleByLeftExp()
{
	assert(false);
	return false;
	/*if (IsInFb())
	return false;
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	auto fatigue_data = gpm.CalcFatigueData();
	if ( fatigue_data <= 0 )
	{
	gpm.TellFatigueData(fatigue_data);
	return true;
	}
	return false;*/
}

boost::optional<CD_En_FbDifficulty> FbDiffLvlName2Value(const std::string& fb_diff_lvl, bool& is_king_fb)
{
	using namespace GType;
	boost::optional<CD_En_FbDifficulty> diff_value;
	if (fb_diff_lvl.empty())
		diff_value = CD_En_FbDifficulty::kFD_Invalid;
	else if (fb_diff_lvl == "试炼")
		diff_value = CD_En_FbDifficulty::kFD_ShiLian;
	else if (fb_diff_lvl == "挑战")
		diff_value = CD_En_FbDifficulty::kFD_Fight;
	else if (fb_diff_lvl == "王立")
	{
		diff_value = CD_En_FbDifficulty::kFD_Invalid;
		is_king_fb = true;
	}
	else
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "副本难度等级只支持“试炼”、“挑战”、“王立”，不支持：" << fb_diff_lvl;
	}
	return diff_value;
}

//查找指定名字的对象
DWORD scmd::FindNpcByName(const luabind::object& obj_name, const luabind::object& obj_title)
{
	using namespace luabind;
	if (IsInFb() && GClass::GAmbient::GetMe().cond_fb_game_over_lose_.CondtionTrue())
		return kMonsterInvalidPacketId;
	if (type(obj_name) != LUA_TSTRING)
	{
		LOG_O(Log_error) << "查找指定名字的对象之参数必须是字符串类型";
		return kMonsterInvalidPacketId;
	}
	return FindNpcByNameImpl(ObjectCast(obj_name, kEmptyStr), ObjectCast(obj_title, kEmptyStr));
}

DWORD scmd::FindBeatableMonsterByName(const luabind::object& obj_name)
{
	using namespace luabind;
	if (type(obj_name) != LUA_TSTRING)
	{
		LOG_O(Log_error) << "查找指定名字的可打的对象之参数必须是字符串类型";
		return kMonsterInvalidPacketId;
	}
	return FindBeatableMonsterByNameImpl(ObjectCast(obj_name, kEmptyStr));
}

DWORD scmd::FindTheBoss()
{
	auto boss = FindTheBossImpl();
	if (boss)
		return boss->GetPacketId();
	return kMonsterInvalidPacketId;
}

bool scmd::WaitAppearBoss(int wait_time)
{
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return false;
	if (!TimerDoWaitUntil([&ambient, this](){
		if (ambient.IsGameOver())
			return true;
		return FindTheBoss() != kMonsterInvalidPacketId;
	}, kFunc_DoNothing, 700, wait_time))
		return false;
	if (ambient.IsGameOver())
		return false;
	return true;
}

bool scmd::SetRoleToBack(DWORD package_id, int steps)
{
	return SetRoleToBackImpl(package_id, steps, &GameObjMgr::GetMe(), false);
}
bool scmd::SetRoleToBack(const GameObjMgr_AnyObj::PacketIdT& package_id, int steps)
{
	if (!IsValidGameObj(package_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动到指定的对象身后之不是有效的对象";
		return false;
	}
	auto obj_mgr = package_id.value_.obj_mgr_;
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	auto obj = obj_mgr->FindByName(package_id.value_.obj_name_);
	if (!obj)
	{
		obj_mgr->DoRebuildAll();
		obj = obj_mgr->FindByName(package_id.value_.obj_name_);
		if (!obj)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "移动到指定的对象身后之已不存在该对象了";
			return false;
		}
	}
	auto the_obj = obj->shared_from_this();
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	/*auto obj_room_num = -1;
	if (gpm.IsInFb())
	{
	the_obj->UpdateOwnerRoomNum();
	obj_room_num = the_obj->GetOwnerRoomIdx();
	}*/

	return CheckOptionalBoolRes(gpm.SetRoleToBackIndirect(*the_obj, obj_mgr, -1, -1, true, steps));
}

bool scmd::OpenNpc(DWORD package_id)
{
	if (!IsValidGameObj(package_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "打开NPC面板之不是有效的对象";
		return false;
	}
	auto& obj_mgr = GameObjMgr::GetMe();
	auto obj = obj_mgr.FindByPacketId(package_id);
	if (!obj)
	{
		obj_mgr.RebuildAll();
		obj = obj_mgr.FindByPacketId(package_id);
		if (!obj)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "打开NPC面板之已不存在该对象了";
			return false;
		}
	}
	return GPlayerMe::GetMe().OpenNpc(*obj);
}

bool scmd::OpenNpc_Grid(DWORD package_id)
{
	if (!IsValidGameObj(package_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "打开NPC格子面板之不是有效的对象";
		return false;
	}
	auto& obj_mgr = GameObjMgr::GetMe();
	auto obj = obj_mgr.FindByPacketId(package_id);
	if (!obj)
	{
		obj_mgr.RebuildAll();
		obj = obj_mgr.FindByPacketId(package_id);
		if (!obj)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "打开NPC格子面板之已不存在该对象了";
			return false;
		}
	}
	return GPlayerMe::GetMe().OpenNpc(*obj, GType::kNOT_JiaoNaXiang);
}

bool scmd::AcceptTaskById(int task_id)
{
	return GPlayerMe::GetMe().AcceptTask(task_id);
}

bool scmd::GiveUpTask(int task_id)
{
	return GPlayerMe::GetMe().GiveUpTask(task_id);
}

bool scmd::CommitTaskById(int task_id)
{
	return GPlayerMe::GetMe().CommitTask(task_id);
}

bool scmd::DoNpcChat(int task_id)
{
	return GPlayerMe::GetMe().DoNpcChat(task_id);
}

bool scmd::MoveToTheVillage(const std::string& dst_village_name, int room_num)
{
	assert(room_num >= 0);
	auto res = GPlayerMe::GetMe().MoveToTheVillage(dst_village_name, room_num);
	if (res && *res)
		return true;
	if (!res)
	{
		std::string err_str("移动到城镇失败，城镇名：");
		err_str += dst_village_name;
		throw CmnSafeRun_SkipExcept(err_str);
	}
	return false;
}

bool scmd::MoveToTheFb(const std::string& fb_name)
{
	auto res = GPlayerMe::GetMe().MoveToTheFb(fb_name);
	if (res && *res)
		return true;
	if (!res)
	{
		std::string err_str("移动到副本失败，副本名：");
		err_str += fb_name;
		throw CmnSafeRun_SkipExcept(err_str);
	}
	return false;
}

bool scmd::MoveToTheFbById(int fb_id)
{
	auto res = GPlayerMe::GetMe().MoveToTheFbById(fb_id);
	if (res && *res)
		return true;
	if (!res)
	{
		std::string err_str("移动到副本失败，副本id：");
		err_str += std::to_string(fb_id);
		throw CmnSafeRun_SkipExcept(err_str);
	}
	return false;
}

bool scmd::MoveToTheFb_Cat(const std::string& cat_fb_name)
{
	auto fb_info = GetDoTheFbInfo_Cat(cat_fb_name);
	if (!fb_info)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不可做该喵喵副本：" << cat_fb_name;
		return false;
	}
	return MoveToTheFbById(fb_info->map_id_);
}

bool scmd::MoveToTheFb_King(const std::string& king_fb_name)
{
	auto fb_info = GetDoTheFbInfo_King(king_fb_name);
	if (!fb_info)
	{
		/*LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不可做该王立副本：" << king_fb_name;*/
		return false;
	}
	if (!fb_info->IsKingFb())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "该副本不是王立副本：" << king_fb_name;
		return false;
	}
	return MoveToTheFbById(fb_info->map_id_);
}

bool scmd::MoveToTheFb_King_Cream(const std::string& king_fb_name, const std::string& unlock_item_name)
{
	if (king_fb_name.empty())
	{
		CMN_ASSERT(false);
		return false;
	}
	auto& enterable_fb = GPlayerMe::GetMe().GetEnterableFbMgr();
	enterable_fb.RebuildAll();
	auto fb_info = enterable_fb.FindByName(king_fb_name);
	if (!fb_info)
	{
		auto& all_fb = MapDbMgr::GetMe();
		all_fb.Update();
		fb_info = all_fb.FindByName(king_fb_name);
		if (!fb_info)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "没有这样的副本信息：" << king_fb_name;
			return false;
		}
		if (!fb_info->IsKingCreamFb())
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "该副本不是王立精英副本：" << king_fb_name;
			return false;
		}
		auto& gpm = GPlayerMe::GetMe();
		if (!gpm.UnlockKingCream(fb_info->map_id_, unlock_item_name))
			return false;
	}
	return MoveToTheFbById(fb_info->map_id_);
}

void scmd::AddFbBlackList_Cat(int fb_id)
{
	GPlayerMe::GetMe().AddFbBlackList_Cat(fb_id);
}

void scmd::ClrFbBlackList_Cat()
{
	GPlayerMe::GetMe().ClrFbBlackList_Cat();
}

void scmd::AddFbBlackList(int fb_id)
{
	GPlayerMe::GetMe().AddFbBlackList(fb_id);
}

void scmd::ClrFbBlackList()
{
	GPlayerMe::GetMe().ClrFbBlackList();
}

bool scmd::CanDoTheFb_Cat(const std::string& cat_fb_name)
{
	if (GetDoTheFbInfo_Cat(cat_fb_name))
		return true;
	return false;
}


bool scmd::GiveUpFb()
{
	return GPlayerMe::GetMe().GiveUpFb();
}

bool scmd::GiveUpXinShouXunLian()
{
	return GPlayerMe::GetMe().GiveUpXinShouXunLian();
}

bool scmd::MoveToTheRoom(int room_num)
{
	auto& gpm = GPlayerMe::GetMe();
	for (auto i = 0; i != 3; ++i)
	{
		if (gpm.MoveToTheRoom(room_num))
			return true;
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "移动到房间[" << room_num << "]失败，等待角色可以移动然后再试一次";
		if (WaitRoleCanMove(1000 * 60))
		{
			if (gpm.MoveToTheRoom(room_num))
				return true;
		}
	}
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "移动到房间[" << room_num << "]失败，可能游戏掉线或服务器无响应了";
	return false;
}

int scmd::GainSupplyItem(const std::string& item_name)
{
	return GPlayerMe::GetMe().GainSupplyItem(item_name);
}

int scmd::HitTheMonsterToDie(DWORD packet_id, int sleep_time, bool is_any_direction, bool resume_hit_mode)
{
	auto& gpm = GPlayerMe::GetMe();
	BOOST_SCOPE_EXIT_ALL(&gpm, resume_hit_mode){
		gpm.SetHitBodyPart(-1, 0, -1);
		//恢复到默认的方式
		gpm.cur_hit_mode_ = &gpm.hit_mode_mouse_;
		if (resume_hit_mode)
			gpm.SetCurHitMode(GClass::GHitMonsterMode_Mouse::kInvalid);
	};
	if (!IsValidGameObj(packet_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "攻击指定的怪物直到死亡之不是有效的对象";
		return false;
	}
	return gpm.HitTheMonsterToDie(packet_id, &GameObjMgr::GetMe(), sleep_time, is_any_direction);
}

int scmd::DestoryStaticBeatableMonster(const std::string& obj_name, int every_hit_cnt, int obj_cnt, int delay_time)
{
	if (obj_name.empty())
	{
		assert(false);
		return 0;
	}
	if (obj_cnt <= 0)
		obj_cnt = std::numeric_limits<int>::max();
	if (every_hit_cnt < 0)
		every_hit_cnt = 1;

	auto& mgr = GStaticBeatableObjMgr::GetMe();
	auto& gpm = GPlayerMe::GetMe();
	auto& ambient = GClass::GAmbient::GetMe();
	int cnt = 0;
	NameList<std::string> beated_objs;

LABLE_ROLE_IS_DEAD:
	try{
		assert(!gpm.is_hitting_monster_);
		gpm.is_hitting_monster_ = true;
		BOOST_SCOPE_EXIT_ALL(&){
			gpm.SetHitBodyPart(-1, 0, -1);
			gpm.HitRaii();
			gpm.SetCurHitMode(GClass::GHitMonsterMode_Mouse::kInvalid);
		};
		for (; cnt < obj_cnt; ++cnt)
		{
			if (ambient.IsGameOver())
				return cnt;
			if (!IsInFb())
				return cnt;
			mgr.RebuildAll();
			GameObjBase::SelfPtrT the_obj;
			gpm.UpdateOwnerRoomNum();
			mgr.Traverse([&the_obj, &obj_name, &beated_objs](GameObjBase& obj){
				if (obj.GetOwnerRoomIdx() < 0)
					return true;
				auto& the_name = obj.GetName();
				if (the_name.find(obj_name) != std::string::npos && !beated_objs.IsIn(the_name))
				{
					the_obj = obj.shared_from_this();
					return false;
				}
				return true;
			});
			if (!the_obj)
				break;

			if (the_obj->GetOwnerRoomIdx() != gpm.GetOwnerRoomIdx())
			{
				if (!MoveToTheRoom(the_obj->GetOwnerRoomIdx()))
				{
					//assert(false);
					LOG_O(Log_debug) << "DestoryStaticBeatableMonster，移动到[" << the_obj->GetOwnerRoomIdx() << "]失败";
					continue;
				}
				gpm.UpdateOwnerRoomNum();
				if (the_obj->GetOwnerRoomIdx() != gpm.GetOwnerRoomIdx())
				{
					assert(false);
					continue;
				}
			}
			for (auto i = 0; i < every_hit_cnt; ++i)
			{
				if (ambient.IsGameOver())
					return cnt;
				const auto& res = gpm.HitMonster(*the_obj, &mgr, -1, -1, false);
				if (!res)
					break;
				DummySleep(delay_time);
				if (ambient.IsGameOver())
					return cnt;
			}
			beated_objs.Add(the_obj->GetName());
		}
	} DO_CATCH_THE_PAUSE_ID(kPauseId_RoleDead, {
		gpm.OnRoleDead();
		goto LABLE_ROLE_IS_DEAD;
	});
	return cnt;
}

void scmd::DebugInfoStaticBeatableObjs()
{
	GStaticBeatableObjMgr static_mgr;
	static_mgr.RebuildAll();
	static_mgr.DebugInfo();
}

bool scmd::IsValidGameObj(DWORD packet_id)
{
	return packet_id != kMonsterInvalidPacketId;
}
bool scmd::IsValidGameObj(const GameObjMgr_AnyObj::PacketIdT& packet_id)
{
	if (packet_id.value_.obj_name_.empty())
		return false;
	if (!packet_id.value_.obj_mgr_)
		return false;
	return true;
}

//采集指定的可采集对象
bool scmd::CollectTheCollectableObj(DWORD packet_id, int collect_time)
{
	if (GClass::GAmbient::GetMe().IsGameOver())
		return false;
	if (IsInVillage())
		return false;
	if (!IsValidGameObj(packet_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "采集指定的可采集对象之不是有效的对象";
		return false;
	}
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	auto collect_item = collect_mgr.FindByPacketId(packet_id);
	if (!collect_item)
	{
		return false;
	}
	return GPlayerMe::GetMe().CollectTheCollectableObj(*collect_item, &collect_mgr, collect_time);
}

bool scmd::AutoCollectNearestCollectableObjAsSameRoom(int collect_time)
{
	if (GClass::GAmbient::GetMe().IsGameOver())
		return false;
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	auto collect_obj = collect_mgr.FindNearestAsSameRoom(gpm);
	if (!collect_obj)
		return false;
	return CollectTheCollectableObj(collect_obj->GetPacketId(), collect_time);
}

int scmd::AutoCollectOnceAllRoomCollectableObj(int collect_time)
{
	int cnt = 0;
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return cnt;
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	auto role_room_idx = gpm.GetOwnerRoomIdx();
	if (role_room_idx < 0)
	{
		assert(false);
		return cnt;
	}
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	std::vector<DWORD> the_objs;
	collect_mgr.Traverse([&the_objs, role_room_idx](GameObjBase& v){
		auto collect_obj = v.CastTo<GCollectObj>();
		if (!collect_obj)
		{
			assert(false);
			return true;
		}
		if (collect_obj->GetOwnerRoomIdx() == role_room_idx)
			the_objs.push_back(collect_obj->GetPacketId());
		return true;
	});
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "要采集的数量：" << the_objs.size();

	for (auto packet_id : the_objs)
	{
		if (ambient.IsGameOver())
			return cnt;
		CollectTheCollectableObj(packet_id, collect_time);
		++cnt;
	}
	return cnt;
}

int scmd::AutoCollectOverAllObj(int collect_time)
{
	int cnt = 0;
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return cnt;
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	auto role_room_idx = gpm.GetOwnerRoomIdx();
	if (role_room_idx < 0)
	{
		assert(false);
		return cnt;
	}
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	std::vector<DWORD> the_objs;
	collect_mgr.Traverse([&the_objs, role_room_idx](GameObjBase& v){
		auto collect_obj = v.CastTo<GCollectObj>();
		if (!collect_obj)
		{
			assert(false);
			return true;
		}
		if (collect_obj->GetOwnerRoomIdx() == role_room_idx)
			the_objs.push_back(collect_obj->GetPacketId());
		return true;
	});
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "要采集的数量：" << the_objs.size();

	for (auto packet_id : the_objs)
	{
		while (CollectTheCollectableObj(packet_id, collect_time))
		{
			if (ambient.IsGameOver())
				return cnt;
		}
		++cnt;
	}
	return cnt;
}

int scmd::AutoCollectSpecifc(int collect_time, int room_num, int obj_cnt, int every_collect_cnt, 
	const std::string& collect_obj_name, const std::string& gained_item, const luabind::object& do_after_every_collect)
{
	int cnt = 0;
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return cnt;
	boost::optional<int> gained_item_id;
	if (!gained_item.empty())
	{
		auto& item_mgr = ItemDbMgr::GetMe();
		item_mgr.RebuildAll();
		auto the_gained_item = item_mgr.FindByName(gained_item);
		if (the_gained_item)
			gained_item_id = the_gained_item->item_id_;
		else
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "没有这样的物品信息：" << gained_item;
		}
	}

	struct stHelper{
		DWORD			obj_packet_id_;
		double			the_dist_;
		std::string		collect_obj_name_;
	};
	std::vector<stHelper> dist_cont;
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	auto& gpm = GPlayerMe::GetMe();
	const auto& pos_src = gpm.GetPos();
	gpm.UpdateOwnerRoomNum();
	auto role_room_num = gpm.GetOwnerRoomIdx();
	if (role_room_num < 0)
	{
		assert(false);
		return cnt;
	}
	if (room_num < -1)
	{
		room_num = gpm.GetOwnerRoomIdx();
		if (room_num < 0)
		{
			assert(false);
			return cnt;
		}
	}
	collect_mgr.Traverse([&gpm, room_num, &collect_obj_name, &dist_cont, &pos_src, role_room_num](GameObjBase& v){
		auto collect_obj = v.CastTo<GCollectObj>();
		if (!collect_obj)
		{
			assert(false);
			return true;
		}
		auto the_room_num = collect_obj->GetOwnerRoomIdx();
		if (the_room_num < 0)
			return true;
		if (gpm.IsInCollectBlackList(collect_obj->GetName()))
			return true;
		if (room_num != the_room_num)
		{
			if (room_num >= 0 && room_num != the_room_num)
				return true;
		}

		if (!collect_obj_name.empty())
		{
			if (collect_obj->GetName().find(collect_obj_name) == std::string::npos)
				return true;
		}
		stHelper hlp = {
			collect_obj->GetPacketId(), GameObjMgr::CalcObjDistance(collect_obj->GetPos(), pos_src, std::abs(the_room_num - role_room_num)), collect_obj->GetName()
		};
		dist_cont.push_back(hlp);
		return true;
	});

	if (dist_cont.empty())
		return 0;
	std::sort(dist_cont.begin(), dist_cont.end(), [](const stHelper& lhs, const stHelper& rhs){
		return lhs.the_dist_ < rhs.the_dist_;
	});

	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "要采集的对象数量：" << dist_cont.size();
	//////////////////////////////////////////////////////////////////////////
	//*********************采集临时黑名单，用来记录采集失败信息*********************
	struct stCollectFailInfo{
		int			collect_allow_fail_cnt_;			//允许采集失败次数
		int			allow_can_not_gain_item_cnt_;		//允许能够进行采集，但未获取物品次数
	};
	typedef std::map<std::string, stCollectFailInfo> FailInfoT;
	FailInfoT fail_infos;
	const int kMaxFailCnt = 2;
	const int kMaxCanNotGain = 4;
	auto& on_collect_ret = gpm.GetCollectRet();
	//////////////////////////////////////////////////////////////////////////
	bool is_obj_cnt = obj_cnt > 0;

	//int collect_total_cnt = 0;
	for (auto& v : dist_cont)
	{
		auto collect_item = collect_mgr.FindByPacketId(v.obj_packet_id_);
		if (!collect_item)
			continue;
		auto obj_collect = collect_item->CastTo<GCollectObj>();
		if (!obj_collect)
		{
			assert(false);
			continue;
		}
		std::string collect_item_name = obj_collect->GetName();
		if (!collect_item_name.empty())
		{
			auto it = fail_infos.find(collect_item_name);
			if (it == fail_infos.end())
			{
				stCollectFailInfo tmp = {
					kMaxFailCnt, kMaxCanNotGain
				};
				fail_infos.insert(std::make_pair(collect_item_name, tmp));
			}
			else
			{
				if (it->second.collect_allow_fail_cnt_ <= 0 || it->second.allow_can_not_gain_item_cnt_ <= 0)
					continue;
			}
		}

		int collect_correct = 0;
		bool is_added_the_fail_cnt = false;
		for (auto every_cnt = every_collect_cnt > 0 ? every_collect_cnt : 100; every_cnt > 0; --every_cnt)
		{
			if (ambient.IsGameOver())
				return cnt;
			on_collect_ret.reset();
			LOG_O(Log_trace) << "当前要采集的采集物名：" << v.collect_obj_name_;
			auto is_done_collect = CollectTheCollectableObj(v.obj_packet_id_, collect_time);
			CallFunction(tagNoResult(), do_after_every_collect);
			if (!collect_item_name.empty())
			{
				auto it = fail_infos.find(collect_item_name);
				if (it == fail_infos.end())
				{
					assert(false);
					/*if ( !is_done_collect )
					break;
					else
					{
					/ *++collect_total_cnt;
					if ( collect_total_cnt % 9 == 0 )
					{
					LOG_O(Log_trace) << "采集时，先存放物品到仓库中去";
					StoreSomePackageItemsToStore(1000);
					}* /
					if ( on_collect_ret && on_collect_ret->IsSucceed() )
					collect_correct = true;
					}*/
				}
				else
				{
					if (is_done_collect)
					{
						if (it->second.collect_allow_fail_cnt_ < kMaxFailCnt && !is_added_the_fail_cnt)
						{
							is_added_the_fail_cnt = true;
							++it->second.collect_allow_fail_cnt_;
						}
						if (it->second.allow_can_not_gain_item_cnt_ < kMaxCanNotGain)
							++it->second.allow_can_not_gain_item_cnt_;
						++collect_correct;
					}
					else
					{
						if (on_collect_ret)
						{
							assert(!on_collect_ret->IsSucceed());
							--it->second.allow_can_not_gain_item_cnt_;
						}
						else
						{
							--it->second.collect_allow_fail_cnt_;
						}
						break;
					}
				}
			}
			else
			{
				if (!is_done_collect)
					break;
				else
				{
					/*++collect_total_cnt;
					if ( collect_total_cnt % 9 == 0 )
					{
					LOG_O(Log_trace) << "采集时，先存放物品到仓库中去";
					StoreSomePackageItemsToStore(1000);
					}*/
					++collect_correct;
				}
			}

			if (on_collect_ret && gained_item_id && collect_correct > 0)
			{
				if (on_collect_ret->gain_item_id_ != *gained_item_id)
				{
					LOG_O(Log_debug) << "采集获得的物品与想要的不同，想要的物品：" << gained_item;
					--collect_correct;
				}
			}
		}
		cnt += collect_correct;
		if (is_obj_cnt && cnt >= obj_cnt)
			break;
	}
	return cnt;
}

int scmd::AutoCollectSpecifc_Farm(int collect_time, int obj_cnt, int every_collect_cnt, const std::string& collect_obj_name)
{
	using namespace GClass;
	auto& name_map_mgr = GFarmCollectNameMap::GetMe();
	auto collect_info = name_map_mgr.FindByCnName(collect_obj_name);
	if (!collect_info)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "庄园采集失败，不支持这样的名称：" << collect_obj_name;
		name_map_mgr.DebugInfo_CnName();
		return 0;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (collect_info->cao_yao_tian_idx_ < CD_En_FarmFieldIdx::kInvalid)
	{
		auto& cao_yao_tian_info = gpm.GetCaoYaoTianInfo();
		cao_yao_tian_info.Update();
		auto the_info = cao_yao_tian_info.FindByIdx(collect_info->cao_yao_tian_idx_);
		if (!the_info)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "没有这样的草药田信息:" << collect_obj_name;
			assert(false);
			return 0;
		}
		if (!the_info->CanBeCollected())
			return 0;
	}

	if (!gpm.IsFreeTimeToCollectOfFarm(collect_info->collect_idx_))
		return 0;

	int cnt = 0;
	for (auto& v : collect_info->collect_en_name_)
	{
		if (!v.empty())
			cnt += AutoCollectSpecifc(collect_time, -1, obj_cnt, every_collect_cnt, v, kEmptyStr, luabind::object());
	}
	if (collect_info->cao_yao_tian_idx_ < CD_En_FarmFieldIdx::kInvalid)
		LeftButtonClick(217, 523);
	return cnt;
}

int scmd::AutoCollectOverTheObj(const std::string& obj_name, int collect_time)
{
	int cnt = 0;
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return cnt;
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	auto role_room_idx = gpm.GetOwnerRoomIdx();
	if (role_room_idx < 0)
	{
		assert(false);
		return cnt;
	}
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	std::vector<DWORD> the_objs;
	collect_mgr.Traverse([role_room_idx, &obj_name, &the_objs](GameObjBase& v){
		auto collect_obj = v.CastTo<GCollectObj>();
		if (!collect_obj)
		{
			assert(false);
			return true;
		}
		if (collect_obj->GetOwnerRoomIdx() != role_room_idx)
			return true;
		if (obj_name != collect_obj->GetName())
			return true;
		the_objs.push_back(collect_obj->GetPacketId());
		return true;
	});
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "要采集的数量：" << the_objs.size();

	for (auto packet_id : the_objs)
	{
		while (CollectTheCollectableObj(packet_id, collect_time))
		{
			if (ambient.IsGameOver())
				return cnt;
		}
		++cnt;
	}
	return cnt;
}

bool scmd::TakeInKnife()
{
	return GPlayerMe::GetMe().TakeInKnife();
}

bool scmd::UsePackageItem(const std::string& item_name)
{
	return GPlayerMe::GetMe().UseItemByName(item_name);
}

int scmd::UseSomeItems(const std::string& item_name, int use_cnt)
{
	return GPlayerMe::GetMe().UseSomeItems(item_name, use_cnt);
}

bool scmd::UseTaskItem(const std::string& item_name)
{
	return GPlayerMe::GetMe().UseTaskItem(item_name);
}

bool scmd::ManuFactureItem(const std::string& item_name)
{
	return GPlayerMe::GetMe().ManuFactureItem(item_name);
}

bool scmd::WaitCrossedGraph(int wait_time)
{
	if (wait_time < 0)
	{
		assert(false);
		return false;
	}
	return GClass::GAmbient::GetMe().WaitCrossedGraph(wait_time);
}

bool scmd::WaitRetCity(int wait_time)
{
	if (wait_time < 0)
	{
		assert(false);
		return false;
	}
	if (IsInVillage())
		return true;
	auto& gpm = GPlayerMe::GetMe();
	if (gpm.is_active_cross_map_)
	{
		assert(false);
		return false;
	}
	gpm.is_active_cross_map_ = true;
	BOOST_SCOPE_EXIT_ALL(&gpm){
		gpm.is_active_cross_map_ = false;
	};
	auto res = GClass::GAmbient::GetMe().WaitRetVillage(wait_time);
	WaitRoleCanMove(5 * 1000);
	return res;
}

bool scmd::PressKeyZ()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->DxPressKeyZ();
	return true;
}

bool scmd::PressKeyQ()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->DxPressKeyQ();
	return true;
}

bool scmd::PressKeyW()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->DxPressKeyW();
	return true;
}

bool scmd::PressKeyS()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->DxPressKeyS();
	return true;
}

bool scmd::PressKeyA()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->DxPressKeyA();
	return true;
}

bool scmd::PressKeyD()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->DxPressKeyD();
	return true;
}

bool scmd::PressKeySpace()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->DxPressKeySpace();
	return true;
}

bool scmd::PressKeyEsc()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->DxPressKeyEsc();
	return true;
}

bool scmd::CutNearestSkinOnce(int wait_time, int room_num)
{
	if (GClass::GAmbient::GetMe().IsGameOver())
		return false;
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();

	auto dead_monster = obj_mgr.FindNearestDeadCutableMonster_RoomNum(gpm, room_num);
	if (!dead_monster)
		return false;
	return GPlayerMe::GetMe().CutSkin(*dead_monster, &obj_mgr, wait_time);
}

bool scmd::WaitForCutSkin(int wait_time)
{
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return false;
	wait_time = std::max(wait_time, 650);

	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	auto room_num = gpm.GetOwnerRoomIdx();
	if (!TimerDoWaitUntil([&obj_mgr, &gpm, &collect_mgr, room_num, &ambient](){
		if (ambient.IsGameOver())
			return true;
		auto dead_monster = obj_mgr.FindNearestDeadCutableMonster_RoomNum(gpm, room_num);
		if (!dead_monster)
			return false;
		return true;
	}, [&obj_mgr, &collect_mgr](){
		obj_mgr.RebuildAll();
		collect_mgr.RebuildAll();
	}, 700, wait_time))
		return false;
	if (ambient.IsGameOver())
		return false;
	return true;
}

bool scmd::WaitTheCollectableObj(int wait_time, const std::string& obj_name, int room_num)
{
	if (obj_name.empty())
	{
		assert(false);
		return false;
	}
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return false;
	auto& gpm = GPlayerMe::GetMe();
	if (room_num < -1)
	{
		gpm.UpdateOwnerRoomNum();
		room_num = gpm.GetOwnerRoomIdx();
		if (room_num < 0)
		{
			assert(false);
			return false;
		}
	}
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	if (!TimerDoWaitUntil([&ambient, &collect_mgr, room_num, &obj_name](){
		if (ambient.IsGameOver())
			return true;
		bool res = false;
		collect_mgr.Traverse([&res, room_num, &obj_name](GameObjBase& v){
			auto collect_obj = v.CastTo<GCollectObj>();
			if (!collect_obj)
			{
				assert(false);
				return true;
			}
			if (room_num >= 0 && collect_obj->GetOwnerRoomIdx() != room_num)
				return true;
			if (collect_obj->GetName().find(obj_name) != std::string::npos)
			{
				res = true;
				return false;
			}
			return true;
		});
		return res;
	}, [&collect_mgr](){
		collect_mgr.RebuildAll();
	}, 800, wait_time))
		return false;
	if (ambient.IsGameOver())
		return false;
	return true;
}

int scmd::CutOverNearestSkin(int wait_time, int room_num)
{
	int cnt = 0;
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return cnt;
	while (CutNearestSkinOnce(wait_time, room_num))
	{
		++cnt;
		if (ambient.IsGameOver())
			return 0;
	}
	return cnt;
}

bool scmd::WaitToFree(int wait_time)
{
	if (IsInFb() && GClass::GAmbient::GetMe().cond_fb_game_over_lose_.CondtionTrue())
		return false;
	return GPlayerMe::GetMe().WaitToFree(wait_time);
}

void scmd::DebugInfoMonsterInfo()
{
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	obj_mgr.DebugInfo_Name();
}

void scmd::DebugInfoAccetableTask()
{
	auto& accept_task = GPlayerMe::GetMe().GetAcceptableTasks();
	accept_task.Update();
	accept_task.DebugInfo();
}

DWORD scmd::FindCollectObjByName(const std::string& obj_name)
{
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	auto collect_obj = collect_mgr.FindByName(obj_name);
	if (!collect_obj)
		return kMonsterInvalidPacketId;
	return collect_obj->GetPacketId();
}

int scmd::AutoCollectOverNearestCollectableObjAsSameRoom(int collect_time)
{
	int cnt = 0;
	if (GClass::GAmbient::GetMe().IsGameOver())
		return cnt;
	while (AutoCollectNearestCollectableObjAsSameRoom(collect_time))
	{
		++cnt;
	}
	return cnt;
}

void scmd::DebugInfoPackageItemInfo()
{
	auto& packet_item = GPlayerMe::GetMe().GetGridPackage();
	packet_item.RebuildAll();
	packet_item.DebugInfo();
}

void scmd::DebugInfoCollectableObjs()
{
	auto& collect_mgr = GCollectObjMgr::GetMe();
	collect_mgr.RebuildAll();
	collect_mgr.DebugInfo();
}

void scmd::DebugInfoStaticObjs(const std::string& obj_name)
{
	GObjStaticMgr static_mgr(obj_name);
	static_mgr.RebuildAll();
	static_mgr.DebugInfo();
}


void scmd::DebugInfoTheFbId(const std::string& fb_name)
{
	auto& all_fb = MapDbMgr::GetMe();
	all_fb.Update();
	for (auto& v : all_fb.all_map_)
	{
		if (v.map_name_ == fb_name)
		{
			LOG_O(Log_info) << "副本名：" << fb_name << " 副本ID：" << v.map_id_;
		}
	}
}

void scmd::DebuInfo_AllBodyPart()
{
	GMonster_HitPart::GetMonsterHitPart().DebuInfo_AllBodyPart();
}

void scmd::DebugInfo_CurMapGates()
{
	auto& fb_path = GSP_FbRoom::GetMe();
	if (!fb_path.ForceConstruct())
	{
		assert(false);
		return;
	}
	fb_path.DebugInfo();
}

void scmd::DebugInfo_King()
{
	auto& all_fb = MapDbMgr::GetMe();
	all_fb.Update();
	all_fb.DebugInfo_King();
}
void scmd::DebugInfo_KingCream()
{
	auto& all_fb = MapDbMgr::GetMe();
	all_fb.Update();
	all_fb.DebugInfo_KingCream();
}

void scmd::DebugInfo_AllPlayers()
{
	auto& all_players = GameObjMgr_Role::GetMe();
	all_players.RebuildAll();
	all_players.DebugInfo();
}

std::string scmd::GetItemName(const GItem& item)
{
	return item.item_name_;
}

int scmd::GetItemLvl(const GItem& item)
{
	return item.item_lvl_;
}

int scmd::GetItemCnt(const GItem& item)
{
	return item.item_cnt_;
}

int scmd::GetTheItemTotalCnt(const std::string& item_name)
{
	auto& gpm = GPlayerMe::GetMe();
	return gpm.FindTotalItemByName(item_name).item_cnt_;
}

//获得身上的武器
GItem scmd::GetSelfWeapon(){
	auto& gpm = GPlayerMe::GetMe();
	auto& grid_equip = gpm.GetEquipPackage();
	grid_equip.RebuildAll();
	auto item_ptr = grid_equip.FindBySlotIndex(GType::kEPI_Weapon);
	if (item_ptr)
		return *item_ptr;
	return kItemInvalid;
}

bool scmd::IsValidItem(const GItem& item)
{
	return item.item_idx_ >= 0;
}

GItem scmd::FindPakcetItem(const std::string& item_name)
{
	auto& packet_item = GPlayerMe::GetMe().GetGridPackage();
	packet_item.RebuildAll();
	auto item_ptr = packet_item.FindByName(item_name);
	if (item_ptr)
		return *item_ptr;
	return kItemInvalid;
}

GItem scmd::FindStoreItem(const std::string& item_name)
{
	auto& store_items = GPlayerMe::GetMe().GetStorePackage();
	store_items.RebuildAll();
	auto item_ptr = store_items.FindByName(item_name);
	if (item_ptr)
		return *item_ptr;

	auto& su_cai = GPlayerMe::GetMe().GetGridSuCai();
	su_cai.RebuildAll();
	item_ptr = su_cai.FindByName(item_name);
	if (item_ptr)
		return *item_ptr;

	return kItemInvalid;
}

GItem scmd::FindTaskItem(const std::string& item_name)
{
	auto& task_items = GPlayerMe::GetMe().GetGridsTask();
	task_items.RebuildAll();
	auto item_ptr = task_items.FindByName(item_name);
	if (item_ptr)
		return *item_ptr;
	return kItemInvalid;
}

int scmd::GetTaskTotalItemCnt(const std::string& item_name)
{
	const auto& the_item = GPlayerMe::GetMe().FindTotalItem_TaskByMatchName(item_name);
	return the_item.item_cnt_;
}

GItem scmd::FindItemFromPacketAndStore(const std::string& item_name)
{
	auto& packet_item = GPlayerMe::GetMe().GetGridPackage();
	packet_item.RebuildAll();
	auto item_ptr = packet_item.FindByName(item_name);
	if (item_ptr)
	{
		auto item = *item_ptr;
		auto& store_items = GPlayerMe::GetMe().GetStorePackage();
		store_items.RebuildAll();
		item_ptr = store_items.FindByName(item_name);
		if (item_ptr)
		{
			item.item_cnt_ += item_ptr->item_cnt_;
		}
		else
		{
			auto& su_cai = GPlayerMe::GetMe().GetGridSuCai();
			su_cai.RebuildAll();
			item_ptr = su_cai.FindByName(item_name);
			if (item_ptr)
			{
				item.item_cnt_ += item_ptr->item_cnt_;
			}
		}
		return item;
	}
	else
	{
		auto& store_items = GPlayerMe::GetMe().GetStorePackage();
		store_items.RebuildAll();
		item_ptr = store_items.FindByName(item_name);
		if (item_ptr)
			return *item_ptr;
		else
		{
			auto& su_cai = GPlayerMe::GetMe().GetGridSuCai();
			su_cai.RebuildAll();
			item_ptr = su_cai.FindByName(item_name);
			if (item_ptr)
				return *item_ptr;
		}
	}
	return kItemInvalid;
}

luabind::object scmd::GetCurFbAllRoomNum()
{
	using namespace luabind;
	object room_num;

	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return room_num;

	auto& doors_mgr = FbDoorsMgr::GetMe();
	doors_mgr.RebuildAll();
	if (!doors_mgr.GetAllNodes().empty())
	{
		room_num = newtable(norc->GetGLua().lua());
		int idx = 1;
		doors_mgr.Traverse([&room_num, &idx](GameObjBase& obj){
			auto door_ptr = obj.CastTo<FbDoor>();
			if (!door_ptr)
			{
				assert(false);
				return true;
			}
			room_num[idx++] = door_ptr->GetOwnerRoomIdx();
			return true;
		});
	}
	return room_num;
}

bool scmd::DoCraft(const std::string& item_name)
{
	return GPlayerMe::GetMe().DoCraft(item_name);
}

DWORD scmd::FindNearestNormalMonsterFirst(int the_room_num)
{
	if (!IsInFb())
		return kMonsterInvalidPacketId;
	if (GClass::GAmbient::GetMe().cond_fb_game_over_lose_.CondtionTrue())
		return kMonsterInvalidPacketId;
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	auto& gpm = GPlayerMe::GetMe();
	gpm.Update();

	auto obj = obj_mgr.FindNearestBeatableSortedMonster_RoomNum(gpm, the_room_num);
	if (obj)
		return obj->GetPacketId();
	return kMonsterInvalidPacketId;
}

bool scmd::IsTheBoss(DWORD packet_id)
{
	if (!IsValidGameObj(packet_id))
		return false;
	auto& obj_mgr = GameObjMgr::GetMe();
	obj_mgr.RebuildAll();
	auto obj = obj_mgr.FindByPacketId(packet_id);
	if (!obj)
		return false;

	if (kGOT_Monster != obj->GetGameObjType())
		return false;

	auto monster = obj->CastTo<GMonster>();
	if (!monster)
	{
		assert(false);
		return false;
	}
	return monster->IsBoss();
}

bool scmd::SetRoleToBackImpl(DWORD package_id, int steps, GNormalGameObjMgrBase* obj_mgr, bool is_any_direction)
{
	if (!IsValidGameObj(package_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动到指定的对象身后之不是有效的对象";
		return false;
	}
	if (!obj_mgr)
	{
		assert(false);
		return false;
	}
	auto obj = obj_mgr->FindByPacketId(package_id);
	if (!obj)
	{
		obj_mgr->DoRebuildAll();
		obj = obj_mgr->FindByPacketId(package_id);
		if (!obj)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "移动到指定的对象身后之已不存在该对象了";
			return false;
		}
	}
	auto& gpm = GPlayerMe::GetMe();
	auto the_obj = obj->shared_from_this();
	gpm.Update();
	auto obj_room_num = -1;
	if (gpm.IsInFb())
	{
		the_obj->UpdateOwnerRoomNum();
		obj_room_num = the_obj->GetOwnerRoomIdx();
	}

	return CheckOptionalBoolRes(gpm.SetRoleToBackIndirect(*the_obj, obj_mgr, obj_room_num, gpm.GetOwnerRoomIdx(), is_any_direction, steps));
}

int scmd::KillOverTheRoomMonsters(int sleep_time, bool is_collect, int room_num)
{
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return 0;
	int cnt = 0;
	auto packet_id = FindNearestNormalMonsterFirst(room_num);
	BOOST_SCOPE_EXIT_ALL(&){
		GPlayerMe::GetMe().SetCurHitMode(GClass::GHitMonsterMode_Mouse::kInvalid);
	};
	LOG_ENTER_FUNC;
	while (IsValidGameObj(packet_id))
	{
		if (ambient.IsGameOver())
			break;
		cnt += HitTheMonsterToDie(packet_id, sleep_time, false, false) > 0 ? 1 : 0;
		if (is_collect)
		{
			LOG_O(Log_info) << "等待剥皮，等待时间：" << 5000;
			WaitForCutSkin(5000);
			LOG_O(Log_info) << "剥完皮，间隔时间" << 1000;
			CutOverNearestSkin(1000, -1);
		}

		packet_id = FindNearestNormalMonsterFirst(room_num);
	}
	return cnt;
}

int scmd::KillTheMonsterAsCnt(const std::string& obj_name, int cnt, const std::string& use_item_name, int steps_at_use_item)
{
	if (obj_name.empty())
	{
		CMN_ASSERT_(false, "KillTheMonsterAsCnt");
		return 0;
	}
	auto& obj_mgr = GameObjMgr::GetMe();
	int killed_cnt = obj_mgr.GetTheDeadMonsterCnt_ByMatchCnName(obj_name);
	if (cnt > 0 && killed_cnt >= cnt)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_info) << "已经杀死那么多怪物了，怪物名：" << obj_name << " 要杀的数量：" << cnt;
		return killed_cnt;
	}
	auto& gpm = GPlayerMe::GetMe();
	BOOST_SCOPE_EXIT_ALL(&gpm){
		//恢复到默认的方式
		gpm.cur_hit_mode_ = &gpm.hit_mode_mouse_;
		gpm.SetCurHitMode(GClass::GHitMonsterMode_Mouse::kInvalid);
	};

	bool is_any_direction = false;
	/*if ( !use_item_name.empty() )
	is_any_direction = true;*/

	DWORD obj_packet_id = 0;
	int wait_cnt = 0;
	while (true)
	{
		obj_packet_id = FindBeatableMonsterByNameImpl(obj_name);
		if (!IsValidGameObj(obj_packet_id))
		{
			++wait_cnt;
			if (wait_cnt >= 3)
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_warning) << "等待出现怪物对象次数过多，怪物名：" << obj_name << " 要杀的数量：" << cnt;
				break;
			}
			WaitAppearBeatableMonster(obj_name, 1000 * 60);
			obj_packet_id = FindBeatableMonsterByNameImpl(obj_name);
			if (!IsValidGameObj(obj_packet_id))
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "等待一段时间后，还未发现该对象，怪物名：" << obj_name << " 要杀的数量：" << cnt;
				break;
			}
			if (wait_cnt > 0)
				wait_cnt = 0;
		}

		SetHitMode_UsePacketItem(use_item_name, steps_at_use_item);
		HitTheMonsterToDie(obj_packet_id, 500, is_any_direction, false);
		if (cnt > 0)
		{
			killed_cnt = obj_mgr.GetTheDeadMonsterCnt_ByMatchCnName(obj_name);
			if (killed_cnt >= cnt)
				return killed_cnt;
		}
		else
		{
			obj_packet_id = FindBeatableMonsterByNameImpl(obj_name);
			if (!IsValidGameObj(obj_packet_id))
				break;
		}
	}
	killed_cnt = obj_mgr.GetTheDeadMonsterCnt_ByMatchCnName(obj_name);
	return killed_cnt;
}

bool scmd::BuyTheItemAsMaxCnt(const std::string& shop_category_name, const std::string& item_name,
	int the_buy_cnt, int max_cnt, const std::string& village_name, const std::string& npc_name, bool gold_limit)
{
	auto& gpm = GPlayerMe::GetMe();
	if (gold_limit)
	{
		gpm.UpdateGold_Notify();
		if (gpm.GetCurGold() <= gpm.min_gold_to_keep_buy_)
			return false;
		if (gpm.GetCurSilver() <= gpm.min_silver_to_keep_buy_)
			return false;
	}

	auto buy_cnt = 0;
	auto owner_cnt = 0;

	if (max_cnt > 0)
	{
		auto& item = gpm.FindTotalItemByName(item_name);
		if (item.item_cnt_ >= max_cnt)
			return true;
		buy_cnt = max_cnt - item.item_cnt_;
		if (buy_cnt <= 0)
			return true;
		buy_cnt = std::min(buy_cnt, the_buy_cnt);
		if (buy_cnt <= 0)
			return true;
		if (item.item_cnt_ > 0)
			owner_cnt = item.item_cnt_;
	}
	else
	{
		buy_cnt = the_buy_cnt;
		auto& item = gpm.FindTotalItemByName(item_name);
		if (item.item_cnt_ > 0)
			owner_cnt = item.item_cnt_;
	}
	auto& item_template_mgr = ItemDbMgr::GetMe();
	item_template_mgr.RebuildAll();
	auto item_template = item_template_mgr.FindByName(item_name);
	if (!item_template)
	{
		assert(false);
		return false;
	}
	if (item_template->carry_limit_ > 0)
		buy_cnt = std::min(buy_cnt, item_template->carry_limit_ - owner_cnt);
	if (buy_cnt <= 0)
		return true;
	return GPlayerMe::GetMe().DoBuy(shop_category_name, item_name, buy_cnt, village_name, npc_name);
}

bool scmd::LvlUpEquip(const std::string& equip_name, int lvl_up_equip_idx)
{
	return GPlayerMe::GetMe().LvlUpEquip(equip_name, lvl_up_equip_idx - 1);
}

bool scmd::AutoLvlUpBodyEquips(const luabind::object& village_npc)
{
	using namespace luabind;
	assert(LUA_TTABLE == type(village_npc));
	auto& gpm = GPlayerMe::GetMe();
	auto& tab_equip = gpm.GetEquipPackage();
	tab_equip.RebuildAll();
	auto the_weapon = tab_equip.FindBySlotIndex(GType::kEPI_Weapon);
	if (!the_weapon)
		return false;
	if (gpm.IsInAutoLvlUpEquipBlackList(*the_weapon))
		return false;
	auto weapon_tmp = *the_weapon;
	auto item_template = weapon_tmp.GetItemTemplate();
	if (!item_template)
	{
		assert(false);
		return false;
	}
	auto lvl_up_info = gpm.GetTheEquipLvlUpInfo(*item_template, -1);
	if (!lvl_up_info)
		return false;

	auto& item_template_mgr = ItemDbMgr::GetMe();
	auto dst_item = item_template_mgr.FindById(lvl_up_info->dst_item_id_);
	if (!dst_item)
	{
		assert(false);
		return false;
	}
	gpm.UpdateRoleLvl_Notify();
	if (dst_item->player_lvl_ > gpm.GetRoleLvl())
		return false;
	iterator it(village_npc), it_end;
	for (; it != it_end; ++it)
	{
		if (OpenTheNpcOfVillage(ObjectCast((*it)[1], kEmptyStr), 0, ObjectCast((*it)[2], kEmptyStr), kEmptyStr, true))
			break;
	}
	if (it == it_end)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "自动升级身上装备之城镇npc信息参数可能配置错误";
		return false;
	}
	return gpm.LvlUpTheEquip(weapon_tmp, -1, false);
}

bool scmd::MM_ExistTheItem(const std::string& material_name)
{
	using namespace luabind;
	assert(!material_name.empty());
	auto nor = FuncObj_NormalC::GetInstance();
	if (!nor)
	{
		assert(false);
		return false;
	}
	const auto& tbl_globals = globals(nor->GetGLua().lua());
	const object& func_exist = tbl_globals["MM_ExistTheItem"];
	assert(LUA_TFUNCTION == type(func_exist));
	return CallFunction(func_exist, false, material_name);
}

bool scmd::MM_DoGainTheMaterial(const std::string& material_name, int cnt_limit)
{
	using namespace luabind;
	assert(!material_name.empty());
	auto nor = FuncObj_NormalC::GetInstance();
	if (!nor)
	{
		assert(false);
		return false;
	}
	const auto& tbl_globals = globals(nor->GetGLua().lua());
	const object& func_do_gain = tbl_globals["MM_DoGainTheMaterial_"];
	assert(LUA_TFUNCTION == type(func_do_gain));

	auto& gpm = GPlayerMe::GetMe();
	if (gpm.move_fb_strict_)
		return CallFunction(func_do_gain, true, material_name, cnt_limit);

	gpm.move_fb_strict_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		gpm.move_fb_strict_ = false;
	};

	return CallFunction(func_do_gain, true, material_name, cnt_limit);
}

int scmd::AutoDoGainCurWeaponMaterial()
{
	auto& gpm = GPlayerMe::GetMe();
	std::vector<GPlayerMe::stNeedMaterialInfo> lvl_up_info;
	gpm.GainWeaponNeedLvlUpInfo(lvl_up_info);
	if (lvl_up_info.empty())
		return 0;

	auto& item_mgr = ItemDbMgr::GetMe();
	int cnt = 0;
	LOG_ENTER_FUNC;
	for (auto& v : lvl_up_info)
	{
		if (v.need_cnt_ <= 0)
		{
			assert(false);
			continue;
		}
		auto item_template = item_mgr.FindById(v.material_item_id_);
		if (!item_template)
		{
			assert(false);
			continue;
		}
		if (MM_ExistTheItem(item_template->item_name_))
		{
			LOG_O(Log_info) << "自动去获取当前武器所需材料之去获得该材料：" << item_template->item_name_ << "  材料数量上限：" << v.total_item_cnt_ << " 所需材料数：" << v.need_cnt_;
			if (MM_DoGainTheMaterial(item_template->item_name_, v.total_item_cnt_))
			{
				++cnt;
				//continue;
			}
			else
			{
				LOG_O(Log_error) << "自动去获取当前武器所需材料之获得该材料失败：" << item_template->item_name_;
			}
			//break;
		}
		else
		{
			LOG_O(Log_error) << "自动去获取当前武器所需材料之找不到材料处理映射，无法去获得该材料：" << item_template->item_name_;
		}
	}
	return cnt;
}

void scmd::AddAutoLvlUpBlackList(const std::string& item_name)
{
	GPlayerMe::GetMe().AddAutoLvlUpEquipBlackList(item_name);
}

void scmd::ClrAutoLvlUpEquipBlackList()
{
	GPlayerMe::GetMe().ClrAutoLvlUpEquipBlackList();
}

bool scmd::LeftButtonClick(int pos_x, int pos_y)
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
	{
		assert(false);
		return false;
	}
	norc->LeftButtonClick(pos_x, pos_y);
	return true;
}

bool scmd::CloseAllUi()
{
	using namespace GClass;
	auto& ambient = GAmbient::GetMe();
	if (ambient.IsCursorSameAsTheState(false))
		return true;
	for (int i = 0; i != 10; ++i)
	{
		PressKeyEsc();
		if (ambient.IsCursorSameAsTheState(false))
			return true;
		DummySleep(200);
		if (ambient.IsCursorSameAsTheState(false))
			return true;
		if (i > 5)
		{
			if (ambient.cond_fb_game_over_lose_.CondtionTrue())
				return false;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (!ambient.PressKeyGraveWaitFor(false))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "这里ui是失败了";
		return false;
	}
	return true;
}

bool scmd::IsValidTaskId(int task_id)
{
	return task_id != kInvalidTaskId;
}

int scmd::GetLeftAcceptableShangJinCntToday()
{
	return GFC_GetShangJinTotalCount() - GFC_GetShangJinCompleteCount();
}

const GMapDb* TransferFight2ShiLian(const GTaskDb* task_template){
	assert(task_template);
	/*if (1 != task_template->do_fb_ids_.Size())
		return nullptr;*/
	auto& enterable_fb = GPlayerMe::GetMe().GetEnterableFbMgr();
	auto& map_db_mgr = MapDbMgr::GetMe();
	for (auto do_fb_id : task_template->do_fb_ids_.GetValues())
	{
		auto fb_info = map_db_mgr.FindById(do_fb_id);
		if (!fb_info)
			continue;
		auto the_pos = fb_info->map_name_.find(kFbSubName_TiaoZhan);
		if (std::string::npos == the_pos)
			continue;
		auto map_name_tmp = fb_info->map_name_;
		auto str_len = strlen(kFbSubName_TiaoZhan);
		map_name_tmp.replace(the_pos, str_len, kFbSubName_ShiLian);
		fb_info = map_db_mgr.FindByName(map_name_tmp);
		if (!fb_info)
		{
			map_name_tmp = fb_info->map_name_;
			map_name_tmp.replace(the_pos, str_len, "试炼");
			fb_info = map_db_mgr.FindByName(map_name_tmp);
			if (!fb_info)
				continue;
		}
		if (!enterable_fb.FindById(fb_info->map_id_))
			continue;
		return fb_info;
	}
	return nullptr;
}

luabind::object scmd::GetAnyAcceptableShangJinTaskId(const luabind::object& the_lua_param)
{
	using namespace GClass;
	using namespace GType;
	luabind::object res = luabind::newtable(the_lua_param.interpreter());
	res[1] = kInvalidTaskId;
	if (GetLeftAcceptableShangJinCntToday() <= 0)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "今日已不可接赏金任务";
		return res;
	}
	auto& gpm = GPlayerMe::GetMe();
	auto& enterable_fb = gpm.GetEnterableFbMgr();
	enterable_fb.RebuildAll();
	/*auto& acceptable = gpm.GetAcceptableTasks();
	acceptable.Update();*/
	auto& task_template_mgr = GTaskDbMgr::GetMe();
	task_template_mgr.RebuildAll();
	auto& shang_jin_task_info = GCurShangJinInfo::GetMe();
	shang_jin_task_info.Update();

	//const auto& acceptable_name_list = MakeNameListRef(acceptable.acceptable_tasks_);

	GPlayerMe::ShanJinTaskInfoT shang_jin_info;
	for (auto& v : shang_jin_task_info.info_)
	{
		auto task_status = GFC_GetShangJinTaskStatus(v.task_id_);
		//已接了
		if (1 == task_status)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "该赏金任务竟是已接的了，任务ID：" << v.task_id_;
			CMN_ASSERT(false);
			return res;
		}
		//已完成了
		if (10 == task_status)
			continue;
		auto task_template = task_template_mgr.FindById(v.task_id_);
		if (!task_template)
		{
			assert(false);
			continue;
		}
		if (CD_En_TaskType::kTT_Shang != task_template->task_type_)
		{
			assert(false);
			continue;
		}
		const GMapDb* fb_info = nullptr;
		for (auto do_fb_id : task_template->do_fb_ids_.GetValues())
		{
			fb_info = enterable_fb.FindById(do_fb_id);
			if (!fb_info)
				break;
		}
		bool test_fb_diff_lvl = true;
		if (!fb_info)
		{
			fb_info = TransferFight2ShiLian(task_template);
			if (!fb_info)
				continue;
			test_fb_diff_lvl = false;
		}
		/*if ( CD_En_FbType::kFT_WangLi != fb_info->fb_type_ )
		{
		if ( !acceptable_name_list.IsIn(v.task_id_) )
		continue;
		}*/
		if (gpm.IsInShangJinDstFbBlackList(fb_info->map_id_) || gpm.IsInShangJinDstFbStableBlackList(fb_info->map_id_))
			continue;

		GPlayerMe::stShangJinTaskInfo tmp;
		tmp.task_template_ = task_template;
		tmp.fb_info_ = *fb_info;
		tmp.test_fb_diff_lvl_ = test_fb_diff_lvl;
		shang_jin_info.push_back(tmp);
	}
	if (shang_jin_info.empty())
		return res;
	int the_fb_id = 0;
	auto the_task = gpm.FilterShangJinWhiteList(shang_jin_info, the_fb_id);
	if (the_task)
	{
		res[1] = the_task->task_id_;
		res[2] = the_fb_id;
		return res;
	}
	return res;
}

luabind::object scmd::GetAnyCanDoShangJinTaskId(const luabind::object& the_lua_param)
{
	auto& gpm = GPlayerMe::GetMe();
	auto& enterable_fb = gpm.GetEnterableFbMgr();
	enterable_fb.RebuildAll();
	auto& received_mgr = gpm.GetReceivedTaskMgr();
	received_mgr.RebuildAll();
	auto& task_template_mgr = GTaskDbMgr::GetMe();
	task_template_mgr.RebuildAll();
	GPlayerMe::ShanJinTaskInfoT shang_jin_info;
	auto received_tmp = received_mgr.GetAllNodes();

	luabind::object res = luabind::newtable(the_lua_param.interpreter());
	res[1] = kInvalidTaskId;
	for (auto& v : received_tmp)
	{
		if (v.IsDone())
			continue;
		auto task_template = task_template_mgr.FindById(v.task_id_);
		if (!task_template)
		{
			assert(false);
			continue;
		}
		if (CD_En_TaskType::kTT_Shang != task_template->task_type_)
			continue;
		const GMapDb* fb_info = nullptr;
		for (auto do_fb_id : task_template->do_fb_ids_.GetValues())
		{
			fb_info = enterable_fb.FindById(do_fb_id);
			if (!fb_info)
				break;
		}
		bool test_fb_diff_lvl = true;
		if (!fb_info)
		{
			fb_info = TransferFight2ShiLian(task_template);
			if (!fb_info)
			{
				if (!GiveUpTask(v.task_id_))
					return res;
				continue;
			}
			test_fb_diff_lvl = false;
		}
		if (gpm.IsInShangJinDstFbBlackList(fb_info->map_id_) || gpm.IsInShangJinDstFbStableBlackList(fb_info->map_id_))
		{
			if (!GiveUpTask(v.task_id_))
				return res;
			continue;
		}
		GPlayerMe::stShangJinTaskInfo tmp;
		tmp.task_template_ = task_template;
		tmp.fb_info_ = *fb_info;
		tmp.test_fb_diff_lvl_ = test_fb_diff_lvl;
		shang_jin_info.push_back(tmp);
	}
	if (shang_jin_info.empty())
		return res;
	int the_fb_id = 0;
	auto the_task = gpm.FilterShangJinWhiteList(shang_jin_info, the_fb_id);
	if (the_task)
	{
		res[1] = the_task->task_id_;
		res[2] = the_fb_id;
		return res;
	}
	for (auto& v : shang_jin_info)
	{
		if (!v.task_template_)
		{
			CMN_ASSERT(false);
			continue;
		}
		if (!GiveUpTask(v.task_template_->task_id_))
			return res;
	}
	return res;
}

int scmd::GetAnyCommitableShangJinTaskId()
{
	auto& received_mgr = GPlayerMe::GetMe().GetReceivedTaskMgr();
	received_mgr.RebuildAll();
	auto& task_template_mgr = GTaskDbMgr::GetMe();
	task_template_mgr.RebuildAll();
	for (auto& v : received_mgr.GetAllNodes())
	{
		if (!v.IsDone())
			continue;
		auto task_template = task_template_mgr.FindById(v.task_id_);
		if (!task_template)
		{
			assert(false);
			continue;
		}
		if (CD_En_TaskType::kTT_Shang == task_template->task_type_)
			return v.task_id_;
	}
	return kInvalidTaskId;
}

luabind::object scmd::GetTaskToDstFbName(int task_id)
{
	using namespace luabind;
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return object();
	const auto& fb_info = GetTaskToDoFbInfo(task_id);
	if (!fb_info)
		return object();

	return object(norc->GetGLua().lua(), fb_info->map_name_);
}

luabind::object scmd::GetTaskToDstFbId(int task_id)
{
	using namespace luabind;
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return object();
	const auto& fb_info = GetTaskToDoFbInfo(task_id);
	if (!fb_info)
		return object();

	return object(norc->GetGLua().lua(), fb_info->map_id_);
}

std::string scmd::GetTheFbNameById(int fb_id)
{
	auto& all_fb_mgr = MapDbMgr::GetMe();
	all_fb_mgr.Update();
	auto the_fb_info = all_fb_mgr.FindById(fb_id);
	if (!the_fb_info)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的副本信息，副本ID：" << fb_id;
		return kEmptyStr;
	}
	return the_fb_info->map_name_;
}

int scmd::GetTheFbIdByName(const std::string& fb_name)
{
	if (fb_name.empty())
		return kInvalidFbId;
	auto& all_fb_mgr = MapDbMgr::GetMe();
	all_fb_mgr.Update();
	auto the_fb_info = all_fb_mgr.FindByName(fb_name);
	if (!the_fb_info)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的副本信息，副本名：" << fb_name;
		return kInvalidFbId;
	}
	return the_fb_info->map_id_;
}

luabind::object scmd::GetTheTaskName(int task_id)
{
	using namespace luabind;
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return object();
	if (!IsValidTaskId(task_id))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "获取任务名之任务ID参数不是有效的任务ID";
		return object();
	}
	auto& task_template_mgr = GTaskDbMgr::GetMe();
	task_template_mgr.RebuildAll();
	auto task_template = task_template_mgr.FindById(task_id);
	if (!task_template)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "获取任务名之没有找到这样的任务信息，任务ID：" << task_id;
		return object();
	}
	return object(norc->GetGLua().lua(), task_template->task_name_);
}

bool scmd::MoveItem(int tab_category_src, const std::string& item_name, int tab_category_dst, int dst_slot_idx)
{
	assert(!IsInFb());
	/*if ( dst_slot_idx <= 0 )
	{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "移动物品之要移往的物品索引的数值不正确，不能小于1，要以1为起始";
	return false;
	}*/
	--dst_slot_idx;

	auto category_src = Num2TabCategory(tab_category_src);
	if (CD_En_TabCategory::kTC_Invalid == category_src)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动物品之物品所属物品栏的数字不正确，1是背包栏，2是仓库栏，3是装备栏";
		return false;
	}
	auto category_dst = Num2TabCategory(tab_category_dst);
	if (CD_En_TabCategory::kTC_Invalid == category_dst)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "移动物品之要移往的物品栏的数字不正确，1是背包栏，2是仓库栏，3是装备栏";
		return false;
	}

	auto& gpm = GPlayerMe::GetMe();
	auto item_mgr_src = gpm.GetItemMgr(category_src);
	if (!item_mgr_src)
	{
		assert(false);
		return false;
	}
	item_mgr_src->RebuildAll();

	auto item_ptr = item_mgr_src->FindByName(item_name);
	if (!item_ptr)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_warning) << "移动物品之找不到这样的物品，物品名：" << item_name;
		return false;
	}
	if (category_src == category_dst)
	{
		if (item_ptr->item_idx_ == dst_slot_idx)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "移动物品之无法移动到同一个地方，物品名：" << item_name;
			return false;
		}
	}

	/*auto item_mgr_dst = gpm.GetItemMgr(category_dst);
	if ( !item_mgr_dst )
	{
	assert(false);
	return false;
	}
	item_mgr_dst->RebuildAll();

	if (item_mgr_dst->FindBySlotIndex(dst_slot_idx))
	{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "移动物品之目标位置已有物品了，无法移动。物品名：" << item_name;
	return false;
	}*/
	auto item_tmp = *item_ptr;
	return gpm.SmartMoveItem(item_tmp, category_dst, dst_slot_idx);
}

bool scmd::StoreSomePackageItemsToStore(int reserve_cnt)
{
	if (reserve_cnt <= 0)
		return true;
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateCurMapInfo_Notify();
	if (!gpm.IsInVillage() && !gpm.IsInSelfFarm())
		return false;
	gpm.StoreToPackageTheWhileList();

	reserve_cnt = std::max(0, reserve_cnt);

	auto& package_item = gpm.GetGridPackage();
	gpm.ArrangeItems(package_item.GetTabCategory());
	reserve_cnt = std::min(reserve_cnt, package_item.GetGridMaxCnt());
	reserve_cnt -= package_item.GetFreeCnt();
	if (reserve_cnt <= 0)
		return true;
	auto& items = package_item.GetItems();
	auto it = items.begin(), it_end = items.end();
	std::queue<GItem> items_tmp;
	while (reserve_cnt > 0 && it != it_end)
	{
		if (gpm.InStoreWhiteList(*it))
		{
			++it;
			continue;
		}
		items_tmp.push(*it++);
		--reserve_cnt;
	}

	auto& stored_item = gpm.GetStorePackage();
	gpm.ArrangeItems(stored_item.GetTabCategory());
	auto& su_cai = gpm.GetGridSuCai();
	gpm.ArrangeItems(su_cai.GetTabCategory());
	//assert(0 == reserve_cnt);
	while (!items_tmp.empty())
	{
		auto& the_item = items_tmp.front();
		CD_En_TabCategory tbl_category_dst;
		if (the_item.IsSuCai())
			tbl_category_dst = CD_En_TabCategory::kTC_SuCai;
		else
			tbl_category_dst = CD_En_TabCategory::kTC_Store;
		if (!gpm.SmartMoveItem(the_item, tbl_category_dst, -1))
		{
			items_tmp.pop();
			continue;
		}
		items_tmp.pop();
		if (!items_tmp.empty())
		{
			DummySleep(200);
		}
	}
	return !items_tmp.empty();
}

bool scmd::GainItemFromStore(const std::string& item_name)
{
	if (IsInFb())
		return false;
	using namespace GType;
	auto& gpm = GPlayerMe::GetMe();
	auto& stored = gpm.GetStorePackage();
	auto& su_cai = gpm.GetGridSuCai();
	stored.RebuildAll();
	auto item_ptr = stored.FindByName(item_name);
	if (!item_ptr)
	{
		su_cai.RebuildAll();
		item_ptr = su_cai.FindByName(item_name);
		if (!item_ptr)
			return false;
	}
	auto item = *item_ptr;
	return GainItemFromStoreImpl(item);
}

bool scmd::PackageItemToStore(const std::string& item_name)
{
	if (IsInFb())
		return false;
	auto& gpm = GPlayerMe::GetMe();
	auto& package = gpm.GetGridPackage();
	package.RebuildAll();
	auto item_ptr = package.FindByName(item_name);
	if (!item_ptr)
		return false;
	auto item = *item_ptr;

	CD_En_TabCategory tbl_category_dst;
	if (item_ptr->IsSuCai())
		tbl_category_dst = CD_En_TabCategory::kTC_SuCai;
	else
		tbl_category_dst = CD_En_TabCategory::kTC_Store;
	return gpm.SmartMoveItem(item, tbl_category_dst, -1);
}

//支持模糊匹配
int scmd::GainSomeItemsFromStore(const std::string& item_name, int cnt)
{
	if (item_name.empty())
	{
		assert(false);
		return 0;
	}
	if (cnt <= 0)
	{
		assert(false);
		return 0;
	}
	if (IsInFb())
		return 0;
	auto& gpm = GPlayerMe::GetMe();
	auto& stored = gpm.GetStorePackage();
	auto& su_cai = gpm.GetGridSuCai();
	stored.RebuildAll();
	su_cai.RebuildAll();
	auto item_ptr = stored.TraverseByMatchName(item_name);
	if (!item_ptr)
	{
		assert(false);
		return 0;
	}
	std::vector<GItem> items;
	for (item_ptr->First(); cnt > 0 && !item_ptr->IsDone(); item_ptr->Next())
	{
		auto& item = item_ptr->CurItem();
		items.push_back(item);
		cnt -= item.item_cnt_;
	}

	item_ptr = su_cai.TraverseByMatchName(item_name);
	if (!item_ptr)
	{
		assert(false);
		return 0;
	}
	for (item_ptr->First(); cnt > 0 && !item_ptr->IsDone(); item_ptr->Next())
	{
		auto& item = item_ptr->CurItem();
		items.push_back(item);
		cnt -= item.item_cnt_;
	}

	int gain_cnt = 0;
	for (auto& v : items)
	{
		if (!GainItemFromStoreImpl(v))
			break;
		gain_cnt += v.item_cnt_;
	}
	return gain_cnt;
}

void scmd::AddStoreWhiteList(const std::string& item_name)
{
	GPlayerMe::GetMe().AddStoreWhiteList(item_name);
}

void scmd::ClrStoreWhiteList()
{
	GPlayerMe::GetMe().ClrStoreWhiteList();
}

bool scmd::EnterSelfFarm()
{
	if (GPlayerMe::GetMe().EnterSelfFarm())
		return true;
	throw CmnSafeRun_SkipExcept("进入到自己的庄园失败");
	return false;
}

bool scmd::EnterXinShou()
{
	if (GPlayerMe::GetMe().EnterXinShou())
		return true;
	throw CmnSafeRun_SkipExcept("进入新手训练失败");
	return false;
}

bool scmd::LeaveFarm()
{
	return GPlayerMe::GetMe().LeaveFarm();
}

void scmd::ClrHandleAtHitting()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return;
	auto& handle_at_hitting = norc->GetHandleAtHitting();
	if (!handle_at_hitting)
	{
		assert(false);
		return;
	}
	handle_at_hitting->SetLuaObj(kHandleAtHitting, luabind::object());
}

bool scmd::AddHandleAtHitting(const luabind::object& judge_func, const luabind::object& do_func)
{
	using namespace luabind;
	if (LUA_TFUNCTION != type(judge_func))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加打怪时的处理之第一个参数必须是函数类型";
		return false;
	}
	if (LUA_TFUNCTION != type(do_func))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加打怪时的处理之第二个参数必须是函数类型";
		return false;
	}
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	auto& handle_at_hitting = norc->GetHandleAtHitting();
	if (!handle_at_hitting)
	{
		assert(false);
		return false;
	}
	auto& handle_tbl = handle_at_hitting->GetLuaObj(kHandleAtHitting);
	if (!handle_tbl.is_valid() || LUA_TTABLE != type(handle_tbl))
	{
		handle_tbl = newtable(norc->GetGLua().lua());
		assert(LUA_TTABLE == type(handle_tbl));
		handle_at_hitting->SetLuaObj(kHandleAtHitting, handle_tbl);
	}

	int idx = 1;
	for (iterator it(handle_tbl), it_end; it != it_end; ++it)
	{
		++idx;
	}
	auto func_tbl = newtable(norc->GetGLua().lua());
	func_tbl[1] = judge_func;
	func_tbl[2] = do_func;
	handle_tbl[idx] = func_tbl;
	return true;
}

void scmd::ClrHandlersAtScriptStoped()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return;
	auto& handlers = norc->GetHandlersAtScriptStoped();
	if (!handlers)
	{
		assert(false);
		return;
	}
	handlers->SetLuaObj(kHandlersOnScriptStoped, luabind::object());
}

bool scmd::AddHandlersAtScriptStoped(const luabind::object& func)
{
	using namespace luabind;
	if (LUA_TFUNCTION != type(func))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "添加脚本停止时的处理之参数必须是函数类型";
		return false;
	}
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	auto& handlers = norc->GetHandlersAtScriptStoped();
	if (!handlers)
	{
		assert(false);
		return false;
	}
	auto& handle_tbl = handlers->GetLuaObj(kHandlersOnScriptStoped);
	if (!handle_tbl.is_valid() || LUA_TTABLE != type(handle_tbl))
	{
		handle_tbl = newtable(norc->GetGLua().lua());
		assert(LUA_TTABLE == type(handle_tbl));
		handlers->SetLuaObj(kHandlersOnScriptStoped, handle_tbl);
	}

	int idx = 1;
	for (iterator it(handle_tbl), it_end; it != it_end; ++it)
	{
		++idx;
	}
	handle_tbl[idx] = func;
	return true;
}

int scmd::GetRoleCurHp()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateHpSync();
	return gpm.GetCurHp();
}

int scmd::HandleAllMails(const std::string& villige_name, const std::string& npc_name)
{
	int mail_cnt = 0;
	int err_cnt = 0;
	while (true)
	{
		++err_cnt;
		if (err_cnt >= 15)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "处理所有邮件失败次数过多";
			return mail_cnt;
		}
		//必须打开邮件才能遍历得到
		if (!OpenTheNpcOfVillage(villige_name, 0, npc_name, kEmptyStr, true))
		{
			return mail_cnt;
		}
		auto& gpm = GPlayerMe::GetMe();
		if (!gpm.SendOpenMail())
		{
			assert(false);
			return mail_cnt;
		}
		auto& mail_mgr = GMailMgr::GetMe();
		auto& all_mails = mail_mgr.GetMails();

		mail_mgr.RebuildAll();
		if (all_mails.empty())
			return mail_cnt;
		stCD_Msg_GetMailItemInfo msg;
		for (auto& v : all_mails)
		{
			gpm.msg_on_get_mail_items_info_.reset();
			msg.mail_id_ = v.mail_serial_;
			if (!CD_FuncCall::SendT(msg))
			{
				assert(false);
				return false;
			}
			if (!WaitUntil([&gpm](){
				if (gpm.msg_on_get_mail_items_info_)
					return true;
				return false;
			}, 1000 * 60 * 2))
			{
				assert(false);
				return false;
			}
		}
		mail_mgr.RebuildAll();

		do
		{
			auto all_mails_tmp = all_mails;
			for (auto& v : all_mails_tmp)
			{
				if (v.gold_ > 0 || !v.items_.empty())
				{
					if (!gpm.SendGainMailItem(v.mail_serial_))
					{
						assert(false);
						goto LABEL_BREAK;
					}
				}

				if (!gpm.SendDeleteMail(v.mail_serial_))
				{
					assert(false);
					goto LABEL_BREAK;
				}
				++mail_cnt;
			}
			mail_mgr.RebuildAll();
			if (all_mails.empty())
				break;
			continue;
		LABEL_BREAK:
			break;
		} while (true);
	}

	return mail_cnt;
}

int scmd::LvlUpCollectOfFarm(const std::string& collect_obj_name, int lvl_limit, bool is_open_npc)
{
	auto& name_map_mgr = GClass::GFarmCollectNameMap::GetMe();
	auto collect_info = name_map_mgr.FindByCnName(collect_obj_name);
	if (!collect_info)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "升级庄园采集失败，不支持这样的名称：" << collect_obj_name;
		name_map_mgr.DebugInfo_CnName();
		return 0;
	}
	auto res = GPlayerMe::GetMe().LvlUpCollectOfFarm(collect_info->collect_idx_, lvl_limit, is_open_npc);
	if (res > 0)
	{
		//CloseAllUi();
		WaitRoleCanMove(1000 * 60 * 2);
		if (!CloseAllUi())
		{
			DummySleep(5000);
			CloseAllUi();
		}
	}
	return res;
}

bool scmd::IsTimeToCollectFarm(const std::string& collect_name)
{
	return GPlayerMe::GetMe().IsTimeToCollectFarm(collect_name);
}

std::string scmd::GetExistFarmSeedName()
{
	return GPlayerMe::GetMe().GetExistFarmSeedName();
}

bool scmd::PlantFarmSeed(const std::string& seed_name, const std::string& land_name)
{
	return GPlayerMe::GetMe().PlantFarmSeed(seed_name, land_name);
}

bool scmd::SelectRoleByName(const std::string& role_name)
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
	{
		assert(false);
		return false;
	}
	return norc->SelectRoleByName(role_name);
}

bool scmd::SeletRoleByIdx(int role_idx)
{
	if (role_idx < 1)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "选择指定的角色之角色索引最小为1";
		return false;
	}
	--role_idx;
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
	{
		assert(false);
		return false;
	}
	return norc->SelectRoleByIdx(role_idx);
}

bool scmd::WaitForSelectableRole(int wait_time)
{
	using namespace GType;
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	WaitUntil([](){
		auto norc = FuncObj_NormalC::GetInstance();
		if (!norc)
			return true;
		auto gs = norc->GetGameState();
		if (kGS_Invalid != gs)
			return true;
		return false;
	}, wait_time);
	norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	return kGS_Invalid != norc->GetGameState();
}

bool scmd::AutoCreateRole()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	if (!norc->AutoCreateRole())
	{
		return false;
	}
	return true;
}

bool scmd::CreateTheRole(const std::string& role_name)
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	norc->CreateTheRole(role_name);

	if (!norc->on_create_player_)
	{
		//assert(false);
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "等待创建角色失败";
		return false;
	}
	LOG_O(Log_trace) << "创建结果：" << norc->on_create_player_->res_;
	return true;
}

bool scmd::AutoCreateOrSelectRole()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	return norc->AutoCreateOrSelectRole();
}

bool scmd::IsAtSelectRole()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	return GType::kGS_SelectRole == norc->GetGameState();
}

bool scmd::IsAtInGame()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	return GType::kGS_InGame == norc->GetGameState();
}

bool scmd::ChangeRole()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	/*auto& gpm = GPlayerMe::GetMe();
	gpm.TellFatigueData(0);*/
	LOG_ENTER_FUNC;
	LOG_O(Log_trace) << "ChangeRole换角色";
	return norc->RetSelectRole();
}

bool scmd::WearEquip(const std::string& item_name)
{
	using namespace GType;
	/*auto equip_part = EquipPartIdx2EquipPartIdx(equip_part_idx);
	if ( kEPI_Invalid == equip_part )
	{
	LOG_ENTER_FUNC;
	LOG_O(Log_error) << "穿戴物品之装备的索引序号不正确，第一个武器索引号是1，手部的索引号是2，依此类推。装备名：" << item_name << "  装备索引号：" << equip_part_idx;
	return false;
	}*/
	auto& gpm = GPlayerMe::GetMe();
	auto& the_item = gpm.FindItemFromPacketAndStore(item_name);
	if (!the_item)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "穿戴装备之找不到这样的物品，物品名：" << item_name;
		return false;
	}
	auto equip_part = the_item->Equip2EquipPartIdx();
	if (kEPI_Invalid == equip_part)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "穿戴装备之该物品无法穿戴：" << item_name;
		return false;
	}
	auto item_tmp = *the_item;
	return WearEquipImpl(item_tmp, equip_part);
}

void scmd::AddAutoWearBlackList(const std::string& item_name)
{
	GPlayerMe::GetMe().AddAutoWearBlackList(item_name);
}

void scmd::ClrAutoWearBlackList()
{
	GPlayerMe::GetMe().ClrAutoWearBlackList();
}

void scmd::AddAutoWearWhiteList(const std::string& item_name, const std::string& item_type)
{
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto the_item = item_mgr.FindByName(item_name);
	if (!the_item)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在这样的物品信息，物品名：" << item_name;
		return;
	}
	GPlayerMe::GetMe().AddAutoWearWhiteList(item_name, item_type);
}

void scmd::ClrAutoWearWhiteList()
{
	GPlayerMe::GetMe().auto_wear_white_list_.Clear();
}

bool scmd::NeedCreateRole()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	auto& roles_list = norc->GetRolesList();
	if (!roles_list)
		return false;
	if (roles_list->players_info_.empty())
		return true;
	return false;
}

int scmd::GetRoleCnt()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return 0;
	auto& roles_list = norc->GetRolesList();
	if (!roles_list)
		return 0;
	return roles_list->players_info_.size();
}

bool scmd::WaitForTheJuQing(const std::string& str_ju_qing, int wait_time)
{
	return GClass::GAmbient::GetMe().UntilJuQing(str_ju_qing, wait_time);
}

int scmd::StepOverTheStaticObjs(const std::string& obj_name)
{
	return GPlayerMe::GetMe().StepOverTheStaticObjs(obj_name);
}

bool scmd::AutoToDoTheSociatyTask(const std::string& task_lvl, const std::string& task_name, const luabind::object& do_func)
{
	return GPlayerMe::GetMe().AutoToDoTheSociatyTask(task_lvl, task_name, do_func);
}

bool scmd::SellToAuction(const std::string& item_name, int item_cnt, int begin_price, int smart_price, int relative_smart_price, int min_smart_price, int average_cnt, bool begin_price_direct)
{
	return GPlayerMe::GetMe().SellToAuction(item_name, item_cnt, begin_price, smart_price, false, relative_smart_price, min_smart_price, average_cnt, begin_price_direct);
}

bool scmd::SellToOtherSide(const std::string& item_name, int item_cnt, int begin_price, int smart_price)
{
	boost::optional<int> nil_int;
	return GPlayerMe::GetMe().SellToAuction(item_name, item_cnt, begin_price, smart_price, true, nil_int, nil_int, nil_int, true);
}

int scmd::SmartBuy(const std::string& item_name, int seach_page_cnt, const std::string& seller, int price_limit, int buy_cnt)
{
	return GPlayerMe::GetMe().SmartBuy(item_name, seach_page_cnt, seller, price_limit, buy_cnt);
}

int scmd::SmartBuyOtherSide()
{
	return GPlayerMe::GetMe().SmartBuyOtherSide();
}

int scmd::QuerySoldCnt(const std::string& item_name)
{
	return GPlayerMe::GetMe().QuerySoldCnt(item_name);
}

int scmd::SellStored_PacketRubbishItems(int packet_reserve_cnt, int stored_reserve_cnt, const std::string& village_name, const std::string& npc_name)
{
	return GPlayerMe::GetMe().SellStored_PacketRubbishItems(packet_reserve_cnt, stored_reserve_cnt, village_name, npc_name);
}

void scmd::SetItemFilterBlackList(const luabind::object& item_func)
{
	GPlayerMe::GetMe().SetItemFilterBlackList(item_func);
}

void scmd::SetItemFilterWhiteList(const luabind::object& item_func)
{
	GPlayerMe::GetMe().SetItemFilterWhiteList(item_func);
}

void scmd::AddItemNameBlackList(const std::string& item_name)
{
	GPlayerMe::GetMe().AddItemNameBlackList(item_name);
}

void scmd::ClrItemNameBlackList()
{
	GPlayerMe::GetMe().ClrItemNameBlackList();
}

void scmd::AddItemNameWhiteList(const std::string& item_name)
{
	GPlayerMe::GetMe().AddItemNameWhiteList(item_name);
}

void scmd::ClrItemNameWhiteList()
{
	GPlayerMe::GetMe().ClrItemNameWhiteList();
}

void scmd::SetItemInfoWhiteList(int item_lvl, int item_quality)
{
	GPlayerMe::GetMe().SetItemInfoWhiteList(item_lvl, item_quality);
}

void scmd::SetCraftItemWhiteList(int item_lvl, int item_quality)
{
	GPlayerMe::GetMe().SetCraftItemWhiteList(item_lvl, item_quality);
}

void scmd::SetLvlUpEquipWhiteList(int item_lvl, int item_quality)
{
	GPlayerMe::GetMe().SetLvlUpEquipWhiteList(item_lvl, item_quality);
}

void scmd::SetFactureWhiteList(int item_lvl, int item_quality)
{
	GPlayerMe::GetMe().SetFactureWhiteList(item_lvl, item_quality);
}

bool scmd::WaitRoleCanMove_ByPressMove(int wait_time)
{
	using namespace GType;
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
		return false;
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateRolePosSync();
	stGameVector3 last_pos = gpm.GetPos();
	norc->DxPressKeyW();
	//int cnt = 0;
	int press_cnt = 0;
	int valid_cnt = 0;
	typedef void (FuncObj_NormalC::*PressFuncT)();
	static PressFuncT the_press_func[] = {
		&FuncObj_NormalC::DxPressKeyA, &FuncObj_NormalC::DxPressKeyS, &FuncObj_NormalC::DxPressKeyD, &FuncObj_NormalC::DxPressKeyW,
	};
	if (!TimerDoWaitUntil([&last_pos, &gpm, &valid_cnt](){
		/*++cnt;
		if ( !FloatEqual(last_pos.fx, ->fx) )
		return true;
		if ( !FloatEqual(last_pos.fy, pos_info->fy) )
		return true;
		if ( 5 == cnt )
		{
		gpm.UpdateCurMapInfo_Notify();
		if (gpm.IsInVillage())
		return true;
		}*/
		const auto& cur_pos = gpm.GetPos();
		if (!last_pos.FloatAllEqual(cur_pos))
		{
			++valid_cnt;
			if (valid_cnt >= 2)
				return true;
		}
		else
		{
			if (valid_cnt > 0)
				--valid_cnt;
		}
		last_pos = cur_pos;
		return false;
	}, [norc, &press_cnt, &gpm](){
		scmd::GetMe().CloseAllUi();
		(norc->*the_press_func[press_cnt++ % _countof(the_press_func)])();
		gpm.UpdateRolePosSync();
	}, 400, wait_time))
		return false;
	return true;
}

bool scmd::WaitRoleCanMove(int wait_time)
{
	using namespace GType;
	auto& gpm = GPlayerMe::GetMe();
	BOOST_SCOPE_EXIT_ALL(&gpm){
		if (!gpm.is_active_cross_map_)
			return;
		gpm.is_active_cross_map_ = false;
		gpm.OnCrossMap();
	};
	return WaitRoleCanMove_ByPressMove(wait_time);
}

//等待游戏有响应
bool scmd::WaitGameActive(int wait_time)
{
	using namespace GType;
	auto& gpm = GPlayerMe::GetMe();
	BOOST_SCOPE_EXIT_ALL(&gpm){
		if (!gpm.is_active_cross_map_)
			return;
		gpm.is_active_cross_map_ = false;
		gpm.OnCrossMap();
	};

	const auto kPressWaitTime = 1000 * 2;
	if (WaitRoleCanMove_ByPressMove(kPressWaitTime))
		return true;
	auto left_time = wait_time - kPressWaitTime;
	if (left_time <= 0)
		return false;

	if (!TimerDoWaitUntil([&gpm](){
		if (WaitUntilShowHideCursor())
			return true;
		return false;
	}, kFunc_DoNothing, 400, left_time))
		return false;
	return true;
}

void scmd::SetRoleToPos(float x, float y, float z)
{
	GType::stGameVector3 pos(x, y, z);
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateOwnerRoomNum();
	gpm.SetRoleToPosIndirect(pos, -1, gpm.GetOwnerRoomIdx(), -1, nullptr, false);
}

void scmd::SetDirFromDstObj(float x, float y, float z)
{
	auto& gpm = GPlayerMe::GetMe();

	auto& dir_info = gpm.TwoPos2Dir(gpm.GetPos(), GType::stGameVector3(x, y, z));
	if (!gpm.SetDir(dir_info.first, dir_info.second))
	{
		assert(false);
		return;
	}
}

bool scmd::IsTheNewRole()
{
	return kFbName_BeginDream == GPlayerMe::GetMe().GetCurMapName();
}

bool scmd::GetOffEquipByIdx(int equip_part_idx)
{
	using namespace GType;
	auto equip_part = EquipPartIdx2EquipPartIdx(equip_part_idx);
	if (kEPI_Invalid == equip_part)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "脱下装备之没有这样的装备索引，装备索引：" << equip_part_idx;
		return false;
	}
	return GetOffEquipByPartIdx(equip_part);
}

int scmd::GetCurRoleRoom()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateOwnerRoomNum();
	return gpm.GetOwnerRoomIdx();
}

void OutTheEquipMaterialInfo(const ItemDb& item_template, const GClass::CraftDbMgr& craft_mgr, ItemDbMgr& item_mgr)
{
	auto craft_item = craft_mgr.FindByDstItemId(item_template.item_id_);
	if (craft_item)
	{
		for (auto& v : craft_item->materials_)
		{
			auto material_template = item_mgr.FindById(v.material_item_id_);
			if (!material_template)
			{
				assert(false);
				continue;
			}
			if (!scmd::GetMe().MM_ExistTheItem(material_template->item_name_))
			{
				LOG_O(Log_warning) << "装备：" << item_template.item_name_ << " 的材料：" << material_template->item_name_ << " 缺少出处信息，你是否忘记 添加材料获取映射 了";
			}
		}
	}
}

void scmd::TestOutEquipMaterialFrom()
{
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto& gpm = GPlayerMe::GetMe();
	auto& craft_mgr = GClass::CraftDbMgr::GetMe();
	craft_mgr.Update();

	for (auto& v : gpm.auto_wear_white_list_.GetValues())
	{
		auto item_template = item_mgr.FindByName(v.first);
		if (!item_template)
		{
			assert(false);
			continue;
		}
		OutTheEquipMaterialInfo(*item_template, craft_mgr, item_mgr);
	}
}

bool scmd::WaitAppearTheNpc(const std::string& obj_name, int wait_time)
{
	if (obj_name.empty())
	{
		assert(false);
		return false;
	}
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return false;
	if (!TimerDoWaitUntil([&obj_name, &ambient](){
		if (ambient.IsGameOver())
			return true;
		return FindNpcByNameImpl(obj_name, kEmptyStr) != kMonsterInvalidPacketId;
	}, kFunc_DoNothing, 800, wait_time))
		return false;
	if (ambient.IsGameOver())
		return false;
	return true;
}

bool scmd::WaitAppearBeatableMonster(const std::string& obj_name, int wait_time)
{
	if (obj_name.empty())
	{
		assert(false);
		return false;
	}
	auto& ambient = GClass::GAmbient::GetMe();
	if (ambient.IsGameOver())
		return false;
	if (!TimerDoWaitUntil([&obj_name, &ambient](){
		if (ambient.IsGameOver())
			return true;
		return FindBeatableMonsterByNameImpl(obj_name) != kMonsterInvalidPacketId;
	}, kFunc_DoNothing, 800, wait_time))
		return false;
	if (ambient.IsGameOver())
		return false;
	return true;
}

void scmd::SetFatigueData(int fatigue_data)
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.TellFatigueData(fatigue_data);
}

bool scmd::ZuoReQiQiu()
{
	return GPlayerMe::GetMe().ZuoReQiQiu();
}

std::string scmd::FindSuitableFbName()
{
	auto& gpm = GPlayerMe::GetMe();
	auto fb_info = gpm.FindSuitableFbInfo();
	if (!fb_info)
		return kEmptyStr;
	return fb_info->map_name_;
}

bool scmd::IsHotStatus()
{
	return GPlayerMe::GetMe().IsHotStatus();
}

bool scmd::IsColdStatus()
{
	return GPlayerMe::GetMe().IsColdStatus();
}

bool scmd::IsPoisoningStatus()
{
	return GPlayerMe::GetMe().IsPoisoningStatus();
}

bool scmd::SetAfterEnteredFb(const luabind::object& func)
{
	return GPlayerMe::GetMe().SetAfterEnteredFb(func);
}

bool scmd::NeedMoDao()
{
	return GPlayerMe::GetMe().NeedMoDao();
}

int scmd::SendMail(const luabind::object& tbl_items, int gold, const std::string& target_role_name, const std::string& village_name, const std::string& npc_name)
{
	using namespace luabind;
	if (scmd::GetMe().GetRoleLvl() < kSendMailMinRoleLvl)
		return -1;
	auto the_type = type(tbl_items);
	GPlayerMe::SendMailContT items;
	if (LUA_TNIL != the_type)
	{
		if (LUA_TTABLE != the_type)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "发送邮件之物品参数必须是表类型";
			return -1;
		}
		for (iterator it(tbl_items), it_end; it != it_end; ++it)
		{
			auto& v = *it;
			auto& item_name = v[1];
			if (LUA_TSTRING != type(item_name))
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "发送邮件之每项的第一个元素必须是字符串类型";
				return -1;
			}
			auto& item_cnt = v[2];
			if (LUA_TNUMBER != type(item_cnt))
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "发送邮件之每项的第二个元素必须是数字类型";
				return -1;
			}
			GPlayerMe::stSendMailItem mail_item;
			mail_item.item_name_ = ObjectCast(item_name, kEmptyStr);
			mail_item.item_cnt_ = ObjectCast(item_cnt, 0);
			mail_item.reach_cnt_ = ObjectCast(v[3], false);
			items.push_back(mail_item);
		}
	}
	auto& gpm = GPlayerMe::GetMe();
	while (true)
	{
		auto res = gpm.SendMail(&items, -1, gold, target_role_name, village_name, npc_name);
		if (GPlayerMe::kSM_Error == res)
			return -1;
		else if (GPlayerMe::kSM_Again == res)
			continue;
		else if (GPlayerMe::kSM_Empty == res)
			return 0;
		else if (GPlayerMe::kSM_Succeed == res)
			return 1;
		else
		{
			assert(false);
			return -1;
		}
	}

	assert(false);
	return -1;
}

int scmd::SendAnyMail(int item_reach_cnt, int gold, const std::string& target_role_name, const std::string& village_name, const std::string& npc_name)
{
	if (scmd::GetMe().GetRoleLvl() < kSendMailMinRoleLvl)
		return -1;
	auto& gpm = GPlayerMe::GetMe();
	while (true)
	{
		auto res = gpm.SendMail(nullptr, item_reach_cnt, gold, target_role_name, village_name, npc_name);
		if (GPlayerMe::kSM_Error == res)
			return -1;
		else if (GPlayerMe::kSM_Again == res)
			continue;
		else if (GPlayerMe::kSM_Empty == res)
			return 0;
		else if (GPlayerMe::kSM_Succeed == res)
			return 1;
		else
		{
			assert(false);
			return -1;
		}
	}

	assert(false);
	return -1;
}

int scmd::GetRoleLvl()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateRoleLvl_Notify();
	return gpm.GetRoleLvl();
}

int scmd::GetRoleCurGold()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateGold_Notify();
	return gpm.GetCurGold();
}

int scmd::GetRoleCurSilver()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateGold_Notify();
	return gpm.GetCurSilver();
}

void scmd::AddShangJinWhiteList(const std::string& task_name, const std::string& fb_diff_lvl)
{
	bool is_king_fb = false;
	const auto& diff_value = FbDiffLvlName2Value(fb_diff_lvl, is_king_fb);
	if (!diff_value)
		return;
	GPlayerMe::GetMe().AddShangJinWhiteList(task_name, *diff_value, is_king_fb);
}

void scmd::ClrShangJinWhiteList()
{
	GPlayerMe::GetMe().ClrShangJinWhiteList();
}

void scmd::AddShangJinDstFbBlackList(int fb_id)
{
	GPlayerMe::GetMe().AddShangJinDstFbBlackList(fb_id);
}

void scmd::ClrShangJinDstFbBlackList()
{
	GPlayerMe::GetMe().ClrShangJinDstFbBlackList();
}

void scmd::AddShangJinDstFbStableBlackList(const std::string& fb_name)
{
	auto& all_fb_mgr = MapDbMgr::GetMe();
	all_fb_mgr.Update();
	auto the_fb_info = all_fb_mgr.FindByName(fb_name);
	if (!the_fb_info)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的副本信息，副本名：" << fb_name;
		return;
	}
	GPlayerMe::GetMe().AddShangJinDstFbStableBlackList(the_fb_info->map_id_);
}

void scmd::ClrShangJinDstFbStableBlackList()
{
	GPlayerMe::GetMe().ClrShangJinDstFbStableBlackList();
}

bool scmd::ArrangeItems_Store()
{
	GPlayerMe::GetMe().ArrangeItems(CD_En_TabCategory::kTC_Store);
	GPlayerMe::GetMe().ArrangeItems(CD_En_TabCategory::kTC_SuCai);
	return true;
}

int scmd::LeftShouLieQuanCnt()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateLeftShouLieQuanCnt_Notify();
	return gpm.GetLeftShouLieQuanCnt();
}

int scmd::G_GetLeftSingleExp()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateExp_Notify();
	return gpm.GetLeftSingleExp();
}

int scmd::G_GetLeftDoubleExp()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateExp_Notify();
	return gpm.GetLeftDoubleExp();
}

void scmd::ChangeGameAc(const std::string& prompt_str)
{
	if (!prompt_str.empty())
	{
		if (IDCANCEL == G_MessageBoxTimeoutA("是否要换号", prompt_str.c_str(), 1000 * 8))
			return;
	}
	auto norc = FuncObj_NormalC::GetInstance();
	if (norc)
	{
		norc->GetSessionOper().ToCall(BC_MakeP("ExchangeGameAc"));
		DummySleep(5000);
	}
}

luabind::object scmd::HandleGameScene(const luabind::object& func)
{
	using namespace luabind;
	assert(LUA_TFUNCTION == type(func));
	auto& gpm = GPlayerMe::GetMe();
	try{
		gpm.is_throw_data_error_ = false;
		return CallFunction(tagResNoObjectCast(), func);
	} DO_CATCH_THE_EXCEPTION(const CmnPauseLogicException& e, {
		if (e.IsIdentityEqual(kPauseId_GameDataErr))
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "kPauseId_GameDataErr";
			auto norc = FuncObj_NormalC::GetInstance();
			if (norc)
			{
				if (!norc->RetSelectRole())
				{
					norc->Disconnect_Prompt("角色数据异常了");
				}
			}
		}
		else if (e.IsIdentityEqual(kPauseId_PassMapErr))
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "kPauseId_PassMapErr";
			auto norc = FuncObj_NormalC::GetInstance();
			if (norc)
			{
				if (!norc->RetSelectRole())
				{
					norc->Disconnect_Prompt("过图长时间未响应，返回选择角色也无响应");
				}
			}
		}
		else if (e.IsIdentityEqual(kPauseId_ChangeCurRunScript))
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "要更换要运行的脚本了，当前运行的顶级脚本名：" << LuasScriptEchoC::GetCurRunScriptNameLevel0() << " 要更换的顶级脚本名：" << GetNextChangeScriptNameLevel0();
			auto norc = FuncObj_NormalC::GetInstance();
			if (norc)
			{
				return object(norc->GetLuaScript().GetLuaVM().lua(), false);
			}
		}
		else if (e.IsIdentityEqual(kPauseId_DoDeleteRole))
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_warning) << "要删除角色了，当前角色名：" << gpm.GetName() << " 角色等级：" << GetRoleLvl() << " 当前金币：" << GetRoleCurGold();
			auto norc = FuncObj_NormalC::GetInstance();
			if (norc)
			{
				if (!norc->RetSelectRole())
				{
					norc->Disconnect_Prompt("删除角色，返回选择角色无响应");
				}
				else
				{
					if (norc->DeleteRole())
					{
						norc->cur_delete_role_idx_ = -1;
						if (norc->change_ac_at_delete_one_role_)
							ChangeGameAc("删除角色后，换号");
						return object(norc->GetLuaScript().GetLuaVM().lua(), false);
					}
					else
						norc->Disconnect_Prompt("删除角色失败");
				}
			}
		}
		else
		{
			throw;
		}
	})
		return object();
}

void scmd::SetMinMoneyToKeepBuy(int min_gold, int min_silver)
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.min_gold_to_keep_buy_ = min_gold;
	gpm.min_silver_to_keep_buy_ = min_silver;
}

bool scmd::AddNewFriend(const std::string& role_name)
{
	return GPlayerMe::GetMe().AddNewFriend(role_name);
}

int scmd::GetError_FbSerIsFull()
{
	return GPlayerMe::GetMe().fb_server_full_cnt_;
}

void scmd::SetCurDeleteRoleIdx(int role_idx)
{
	if (role_idx < 1)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "设置当前要删除的角色索引之角色索引最小为1";
		return;
	}
	--role_idx;
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
	{
		assert(false);
		return;
	}
	norc->cur_delete_role_idx_ = role_idx;
}

int scmd::GetCurDeleteRoleIdx()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
	{
		assert(false);
		return -1;
	}
	if (norc->cur_delete_role_idx_ < 0)
		return -1;
	return norc->cur_delete_role_idx_ + 1;
}

int scmd::GetCurRoleIdxAtRoleList()
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
	{
		assert(false);
		return -1;
	}
	if (!norc->cur_players_list_)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有角色列表信息";
		return -1;
	}
	auto& gpm = GPlayerMe::GetMe();
	auto& role_name = gpm.GetName();
	auto it = norc->cur_players_list_->players_info_.begin();
	int role_idx = 1;
	for (; it != norc->cur_players_list_->players_info_.end(); ++it, ++role_idx)
	{
		if (it->player_name_ == role_name)
			break;
	}
	if (it == norc->cur_players_list_->players_info_.end())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有指定的角色名，角色名：" << role_name;
		return -1;
	}
	return role_idx;
}

void scmd::DoDeleteRole(bool change_ac_at_delete_one_role)
{
	auto norc = FuncObj_NormalC::GetInstance();
	if (!norc)
	{
		assert(false);
		return;
	}
	norc->change_ac_at_delete_one_role_ = change_ac_at_delete_one_role;
	throw CmnPauseLogicException(kPauseId_DoDeleteRole, "进行删除角色");
}

void scmd::SetCurSerLine(int cur_ser_line)
{
	auto nor = FuncObj_NormalC::GetInstance();
	if (!nor)
		return;
	if (cur_ser_line <= 0)
		nor->ser_line_ = -1;
	else
		nor->ser_line_ = cur_ser_line - 1;
}

bool scmd::DoDealFaceToFace(const luabind::object& func_pre_deal, const luabind::object& func_do_deal, const luabind::object& func_after_deal)
{
	using namespace luabind;
	if (LUA_TFUNCTION != type(func_pre_deal))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "交易前的处理函数参数必须是函数类型";
		return false;
	}
	if (LUA_TFUNCTION != type(func_do_deal))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "交易时的处理函数参数必须是函数类型";
		return false;
	}
	if (LUA_TFUNCTION != type(func_after_deal))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "交易完成后的处理函数必须是函数类型";
		return false;
	}

	auto& gpm = GPlayerMe::GetMe();
	try{
		gpm.is_dealing_handle_ = true;
		BOOST_SCOPE_EXIT_ALL(&){
			gpm.is_dealing_handle_ = false;
		};
		if (!CallFunction(func_pre_deal, true))
		{
			LOG_O(Log_warning) << "交易前的处理返回假";
			return false;
		}
		if (!CallFunction(func_do_deal, true))
		{
			LOG_O(Log_warning) << "交易时的处理返回假";
			return false;
		}
		if (!CallFunction(func_after_deal, true))
		{
			LOG_O(Log_warning) << "交易后的处理返回假";
			return false;
		}
		return true;
	} DO_CATCH_THE_EXCEPTION(const CmnPauseLogicException& e, {
		if (e.IsIdentityEqual(kPauseId_CancelDeal))
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_debug) << "kPauseId_CancelDeal";
			LOG_O(Log_warning) << e.what();
			return false;
		}
		else
		{
			throw;
		}
	})

		assert(!"不能执行到这");
	return false;
}

bool scmd::Deal_RequestDeal(const std::string& player_name, int wait_time)
{
	if (GetRoleLvl() < kRoleMinLvl_FaceToFaceDeal)
		return false;
	LOG_ENTER_FUNC;
	if (player_name.empty())
	{
		LOG_O(Log_error) << "请求交易之玩家名不能为空";
		return false;
	}
	auto& obj_mgr = GameObjMgr_Role::GetMe();
	auto& gpm = GPlayerMe::GetMe();
	for (int total_wait_time = 0, per_wait_time = 1000 * 2; wait_time <= 0 || wait_time >= total_wait_time; total_wait_time += per_wait_time, DummySleep(per_wait_time))
	{
		obj_mgr.RebuildAll();
		auto the_obj = obj_mgr.FindByName(player_name);
		if (!the_obj)
		{
			LOG_O(Log_warning) << "请求交易失败，找不到这样的玩家：" << player_name;
			continue;
		}
		if (kGOT_Player != the_obj->GetGameObjType())
		{
			assert(false);
			return false;
		}
		auto shared_obj = the_obj->shared_from_this();
		if (!shared_obj)
		{
			assert(false);
			return false;
		}
		auto the_player = shared_obj->CastTo<GPlayer>();
		if (!the_player)
		{
			assert(false);
			return false;
		}
		if (!SetRoleToBackImpl(the_player->GetPacketId(), -1, &obj_mgr, false))
		{
			LOG_O(Log_error) << "未能移动到该玩家身前：" << player_name;
			continue;
		}
		if (gpm.RequestDeal(*the_player))
			return true;
	}
	LOG_O(Log_error) << "请求交易失败";
	return false;
}

bool scmd::Deal_UntilAcceptOneDealRequest(int time_out)
{
	auto& gpm = GPlayerMe::GetMe();
	int time_out_count = 0;
	const int kPerTimeout = 1000 * 2;
	while (!gpm.AcceptOneDealRequest())
	{
		DummySleep(kPerTimeout);
		if (time_out > 0)
		{
			time_out_count += kPerTimeout;
			if (time_out_count >= time_out)
				return false;
		}
	}
	return true;
}

bool scmd::Deal_MoveSomeItems_Gold(const luabind::object& tbl_items, int gold)
{
	using namespace luabind;
	auto the_type = type(tbl_items);
	std::vector<GClass::stItemInfo> items;
	if (LUA_TNIL != the_type)
	{
		if (LUA_TTABLE != the_type)
		{
			LOG_ENTER_FUNC;
			LOG_O(Log_error) << "物品参数必须是表类型";
			return false;
		}
		for (iterator it(tbl_items), it_end; it != it_end; ++it)
		{
			auto& v = *it;
			auto& item_name = v[1];
			if (LUA_TSTRING != type(item_name))
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "每项的第一个元素必须是字符串类型";
				return false;
			}
			auto& item_cnt = v[2];
			if (LUA_TNUMBER != type(item_cnt))
			{
				LOG_ENTER_FUNC;
				LOG_O(Log_error) << "每项的第二个元素必须是数字类型";
				return false;
			}
			GClass::stItemInfo item_info;
			item_info.item_name_ = ObjectCast(item_name, kEmptyStr);
			item_info.item_cnt_ = ObjectCast(item_cnt, 0);
			items.push_back(item_info);
		}
	}
	auto& gpm = GPlayerMe::GetMe();
	if (!items.empty())
	{
		for (auto& v : items)
		{
			StoreSomeItemToPackage(v.item_name_, v.item_cnt_, [](const GItem& the_move_item){
				return the_move_item.IsFree();
			});
		}
		gpm.ArrangeItems(CD_En_TabCategory::kTC_Package);
		auto& packet_item = gpm.GetGridPackage();
		packet_item.GetItems().sort([](const GItem& lhs, const GItem& rhs){
			return lhs.item_cnt_ > rhs.item_cnt_;
		});

		int dst_item_slot_idx = 0;
		for (auto& v : items)
		{
			auto item_ptr = packet_item.FindIf([&v](const GItem& item){
				if (item.item_name_.find(v.item_name_) == std::string::npos)
					return false;
				if (!item.IsFree())
					return false;
				if (item.item_cnt_ < v.item_cnt_)
					return false;
				return true;
			});
			if (!item_ptr)
				continue;
			auto send_item_cnt = v.item_cnt_;
			if (send_item_cnt <= 0)
				send_item_cnt = item_ptr->item_cnt_;
			else
				send_item_cnt = std::min(send_item_cnt, item_ptr->item_cnt_);
			if (send_item_cnt <= 0)
				continue;

			auto item_tmp = *item_ptr;
			item_tmp.item_cnt_ = send_item_cnt;

			if (!gpm.MoveItemToDealTab(item_tmp, dst_item_slot_idx))
				break;
			++dst_item_slot_idx;
			if (dst_item_slot_idx >= kDealMaxGridCnt)
				break;
		}
	}

	if (!gpm.DealGold(gold))
		return false;

	return true;
}

bool scmd::Deal_LockDealTab()
{
	auto& gpm = GPlayerMe::GetMe();
	return gpm.LockDealTab();
}

bool scmd::Deal_EnterDeal()
{
	auto& gpm = GPlayerMe::GetMe();
	return gpm.EnterDeal();
}

bool scmd::IsNiRenState()
{
	return GPlayerMe::GetMe().IsNiRenStatus();
}

bool scmd::HandleNiRenState(int time_out)
{
	GFC_EnableWuDi(false);
	auto& gpm = GPlayerMe::GetMe();
	gpm.handle_can_not_move_state_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		GFC_EnableWuDi(true);
		gpm.handle_can_not_move_state_ = false;
	};
	auto the_boss = FindTheBoss();
	if (!IsValidGameObj(the_boss))
	{
		assert(false);
		return false;
	}
	SetRoleToBack(the_boss, -1);
	/*if ( !SetRoleToBack(the_boss, -1) )
	{
	CMN_ASSERT(false);
	return false;
	}*/

	if (!gpm.SendToSerThePos(gpm.GetPos(), true))
	{
		CMN_ASSERT(false);
		return false;
	}
	return TimerDoWaitUntil([](){
		if (scmd::GetMe().IsNiRenState())
			return false;
		return true;
	}, [](){
		BOOST_SCOPE_EXIT_ALL(= ){
			DxKeyUpDown(DIK_W, false);
		};
		DxKeyUpDown(DIK_W, true);
		DummySleep(500);
	}, 200, time_out);
}

bool scmd::IsRanShaoStatus()
{
	return GPlayerMe::GetMe().IsRanShaoStatus();
}

bool scmd::HandleRanShaoStatus(int time_out)
{
	return TimerDoWaitUntil([](){
		if (scmd::GetMe().IsRanShaoStatus())
			return false;
		return true;
	}, [](){
		scmd::GetMe().PressKeySpace();
	}, 200, time_out);
}

bool scmd::IsErChouStatus()
{
	return GPlayerMe::GetMe().IsErChouStatus();
}

bool scmd::IsSnowManStatus()
{
	return GPlayerMe::GetMe().IsSnowManStatus();
}

bool scmd::HandleSnowManState(int time_out)
{
	GFC_EnableWuDi(false);
	auto& gpm = GPlayerMe::GetMe();
	gpm.handle_can_not_move_state_ = true;
	BOOST_SCOPE_EXIT_ALL(&){
		GFC_EnableWuDi(true);
		gpm.handle_can_not_move_state_ = false;
	};
	auto the_boss = FindTheBoss();
	if (!IsValidGameObj(the_boss))
	{
		assert(false);
		return false;
	}
	SetRoleToBack(the_boss, 1);

	if (!gpm.SendToSerThePos(gpm.GetPos(), true))
	{
		CMN_ASSERT(false);
		return false;
	}
	return TimerDoWaitUntil([](){
		if (scmd::GetMe().IsSnowManStatus())
			return false;
		return true;
	}, kFunc_DoNothing, 200, time_out);
}

bool scmd::IsChanRaoStatus()
{
	return GPlayerMe::GetMe().IsChanRaoStatus();
}

bool scmd::HandleChanRaoStatus(int time_out)
{
	return TimerDoWaitUntil([](){
		if (scmd::GetMe().IsChanRaoStatus())
			return false;
		return true;
	}, kFunc_DoNothing, 200, time_out);
}

void scmd::SetFbConsumeMode(int mode, int min_silver)
{
	using namespace GType;
	if (kFCM_ShouLieQuan != mode && kFCM_Silver != mode && kFCM_All != mode)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不支持这种副本消费模式：" << mode;
		return;
	}
	if (min_silver < 0)
	{
		LOG_O(Log_trace) << "副本消耗模式的最低银币参数错误";
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	gpm.fb_consume_mode_ = enFbConsumeMode(mode);
	gpm.fb_consume_mode_min_silver_ = min_silver;
}

int scmd::AutoGainGiftBag()
{
	return GPlayerMe::GetMe().AutoGainGiftBag();
}

void scmd::SetCurBossHitBodyPart(const std::string& part_name)
{
	auto boss = FindTheBossImpl();
	if (!boss)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有查找到BOSS，设置失败";
		return;
	}
	auto idx = GMonster_HitPart::GetMonsterHitPart().FindThePartIdx(boss->GetCnName(), part_name);
	if (idx >= 0)
		GPlayerMe::GetMe().SetHitBodyPart(idx, 0xffffff, boss->GetPacketId());
}

void scmd::AutoSetCurBossHitBodyPartAsTheTask(int task_id)
{
	auto& task_mgr = GTaskDbMgr::GetMe();
	task_mgr.RebuildAll();
	auto the_task = task_mgr.FindById(task_id);
	if (!the_task)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有这样的任务信息，任务ID：" << task_id;
		return;
	}
	auto boss = FindTheBossImpl();
	if (!boss)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有查找到BOSS，设置失败";
		return;
	}
	auto idx = GMonster_HitPart::GetMonsterHitPart().FindPartIdxByTaskDesc(boss->GetCnName(), the_task->task_desc_);
	if (idx >= 0)
		GPlayerMe::GetMe().SetHitBodyPart(idx, 0xffffff, boss->GetPacketId());
}

bool scmd::AddMonsterBodyPartInfo(const std::string& monster_name, const std::string& part_name, int body_part_idx)
{
	return GMonster_HitPart::GetMonsterHitPart().AddMonsterBodyPartInfo(monster_name, part_name, body_part_idx);
}

//测试BOSS的部位信息匹配
//--第一个参数是部位ID，第二个参数是攻击次数，部位ID参数可以为负数，表示不攻击指定的部位，部位ID为正数表示要攻击指定的部位。
//--当部位攻击参数为负数时，攻击次数参数便没用了。
void scmd::TestMatchBossBodyPartInfo(int part_id, int hit_cnt)
{
	auto boss = FindTheBossImpl();
	if (!boss)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "没有查找到BOSS，测试失败";
		return;
	}
	auto& gpm = GPlayerMe::GetMe();
	auto& hit_part = GMonster_HitPart::GetMonsterHitPart();

	BOOST_SCOPE_EXIT_ALL(&gpm, &hit_part){
		gpm.SetHitBodyPart(-1, 0, -1);
		gpm.HitRaii();
		gpm.SetCurHitMode(GClass::GHitMonsterMode_Mouse::kInvalid);
	};

LABLE_ROLE_IS_DEAD:
	try{
		assert(!gpm.is_hitting_monster_);
		gpm.is_hitting_monster_ = true;
		
		auto boss_packet_id = boss->GetPacketId();
		auto the_hit_cnt = hit_cnt;

		if (part_id >= 0)
		{
			if (the_hit_cnt <= 0)
				the_hit_cnt = 1;
			gpm.SetHitBodyPart(part_id, the_hit_cnt, boss_packet_id);
		}
		else
		{
			the_hit_cnt = 1;
			gpm.SetHitBodyPart(1, the_hit_cnt, boss_packet_id);
		}

		//auto boss_name = boss->GetCnName();
		while (true)
		{
			if (CheckOptionalBoolRes(gpm.HitMonsterByPacketId(boss_packet_id, &GameObjMgr::GetMe(), false)))
			{
				if (hit_part.body_parts_obj_ && hit_part.body_parts_info_.value_)
				{
					--the_hit_cnt;
					if (the_hit_cnt <= 0)
						break;
				}
			}
			DummySleep(500);
		}

	} DO_CATCH_THE_PAUSE_ID(kPauseId_RoleDead, {
		gpm.OnRoleDead();
		goto LABLE_ROLE_IS_DEAD;
	});

	auto update_cnt = 0;
	for (update_cnt = 0; update_cnt != 5; ++update_cnt)
	{
		if (hit_part.Update())
			break;
		DummySleep(300);
	}
	if (update_cnt >= 5)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "怪物部位信息更新失败";
		return;
	}
	if (hit_part.TestCurBodyPartInfo())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "部位信息测试成功";
	}
}

bool scmd::GainWeekShouLieQuan()
{
	return GPlayerMe::GetMe().GainWeekShouLieQuan();
}

void scmd::SetMonsterMaxHigh(float high)
{
	GPlayerMe::GetMe().SetMonsterMaxHigh(high);
}

void scmd::AddCollectBlackList(const std::string& collect_name){
	GPlayerMe::GetMe().AddCollectBlackList(collect_name);
}
//清空采集黑名单
void scmd::ClrCollectBlackList(){
	GPlayerMe::GetMe().ClrCollectBlackList();
}

int scmd::DoEveryObj_Bucket(const luabind::object& do_func, int do_max_cnt)
{
	GObjMieHuoTong obj_mgr;
	obj_mgr.RebuildAll();
	obj_mgr.DebugInfo();
	return GameObjMgr_AnyObj::LuaDoEveryObj(obj_mgr, do_func, do_max_cnt);
}

luabind::object scmd::HandleRoleEnteredGame(const luabind::object& func)
{
	/*using namespace luabind;
	assert(LUA_TFUNCTION == type(func));
	try{*/
	return CallFunction(tagResNoObjectCast(), func);
	/*} DO_CATCH_THE_EXCEPTION( const CmnPauseLogicException& e, {
	if ( !e.IsIdentityEqual(kPauseId_GameDataErr) )
	throw;
	LOG_ENTER_FUNC;
	LOG_O(Log_debug) << "kPauseId_GameDataErr";
	})
	return object();*/
}

std::string g_new_script_name_level0_;
void scmd::DoChangeRunScriptLevel0(const std::string& new_script_name)
{
	if (new_script_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "要更换的顶级脚本名不能为空：" << new_script_name;
		return;
	}
	g_new_script_name_level0_ = new_script_name;
	throw CmnPauseLogicException(kPauseId_ChangeCurRunScript, "进行更新顶级脚本");
}

std::string scmd::GetNextChangeScriptNameLevel0()
{
	return g_new_script_name_level0_;
}

void scmd::AutoGainOneAutoWearWhiteListEquip()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateRoleLvl_Notify();
	auto role_lvl = gpm.GetRoleLvl();

	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();

	struct stHelper{
		std::string				item_type_;
		const ItemDb*		item_template_;
	};
	std::vector<stHelper> items_info;
	stHelper tmp;

	for (auto& v : gpm.auto_wear_white_list_.GetValues())
	{
		auto item_template = item_mgr.FindByName(v.first);
		if (!item_template)
		{
			assert(false);
			continue;
		}
		if (item_template->player_lvl_ > 0 && item_template->player_lvl_ > role_lvl)
			continue;

		//在白名单中得到自己能穿的最好的装备
		for (auto& item : items_info)
		{
			if (item.item_type_ == v.second)
			{
				if (item_template->player_lvl_ <= item.item_template_->player_lvl_)
					goto LABEL_COTINUE;
				if (item_template->item_quality_ <= item.item_template_->item_quality_)
					goto LABEL_COTINUE;
				assert(item_template->item_type_ == item.item_template_->item_type_);
				item.item_template_ = item_template;
				goto LABEL_COTINUE;
			}
		}

		tmp.item_template_ = item_template;
		tmp.item_type_ = v.second;
		items_info.push_back(tmp);
	LABEL_COTINUE:
		continue;
	}
	if (items_info.empty())
		return;

	auto& tab_equip = gpm.GetEquipPackage();
	tab_equip.RebuildAll();
	auto& tab_package = gpm.GetGridPackage();
	tab_package.RebuildAll();
	auto& tab_store = gpm.GetStorePackage();
	tab_store.RebuildAll();

	for (auto& v : items_info)
	{
		if (tab_equip.FindByName(v.item_template_->item_name_))
			continue;
		if (tab_package.FindByName(v.item_template_->item_name_))
			continue;
		if (tab_store.FindByName(v.item_template_->item_name_))
			continue;
		if (!MM_ExistTheItem(v.item_template_->item_name_))
			continue;

		if (MM_DoGainTheMaterial(v.item_template_->item_name_, 1))
			break;
	}
}

int scmd::AutoWearSomeEquips(int equip_quality_limit, bool is_auto_gian)
{
	using namespace GType;
	CD_En_ItemQuality quality_limit = CD_En_ItemQuality::kIQ_Invalid;
	if (!SetItemQualityImpl(equip_quality_limit, quality_limit))
		return 0;
	if (is_auto_gian)
		AutoGainOneAutoWearWhiteListEquip();

	auto& gpm = GPlayerMe::GetMe();
	auto& tab_equip = gpm.GetEquipPackage();
	tab_equip.RebuildAll();
	auto& tab_package = gpm.GetGridPackage();
	tab_package.RebuildAll();
	auto& tab_store = gpm.GetStorePackage();
	tab_store.RebuildAll();
	const GItem* bestest_items[kEPI_Invalid] = { 0 };
	AutoWearSomeEquipsImpl(quality_limit, bestest_items, tab_package.GetItems(), tab_equip);
	AutoWearSomeEquipsImpl(quality_limit, bestest_items, tab_store.GetItems(), tab_equip);
	std::vector<GItem> items;
	for (auto v : bestest_items)
	{
		if (v)
			items.push_back(*v);
	}
	if (items.empty())
		return 0;
	int cnt = 0;
	for (auto& item : items)
	{
		auto equip_part = item.Equip2EquipPartIdx();
		if (kEPI_Invalid == equip_part)
		{
			assert(false);
			continue;
		}
		if (WearEquipImpl(item, equip_part))
			++cnt;
	}
	return cnt;
}

bool scmd::IsInVillage()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateCurMapInfo_Notify();
	return gpm.IsInVillage();
}

bool scmd::OpenVillageNpcById(const std::string& village_name, int room_num, int obj_db_id, bool cur_village_first)
{
	if (cur_village_first)
	{
		auto& gpm = GPlayerMe::GetMe();
		gpm.UpdateOwnerRoomNum();
		if (room_num == gpm.GetOwnerRoomIdx() && !scmd::GetMe().IsInFb())
		{
			auto the_npc = FindMonsterByObjDbIdImpl(obj_db_id);
			if (IsValidGameObj(the_npc))
			{
				if (SetRoleToBack(the_npc, -1))
				{
					if (OpenNpc(the_npc))
						return true;
				}
			}
		}
	}

	assert(room_num >= 0);
	if (!village_name.empty() && !MoveToTheVillage(village_name, room_num))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "打开某个城镇的指定NPC之，移动城镇失败，城镇名：" << village_name << " npc_id：" << obj_db_id <<
			" 房间号：" << room_num << " 是否本地图优先：" << cur_village_first;
		return false;
	}

	auto the_npc = FindMonsterByObjDbIdImpl(obj_db_id);
	if (!IsValidGameObj(the_npc))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "打开某个城镇的指定NPC之，没有这样的NPC，城镇名：" << village_name << " npc_id：" << obj_db_id << 
			" 房间号：" << room_num << " 是否本地图优先：" << cur_village_first;
		return false;
	}
	if (!SetRoleToBack(the_npc, -1))
	{
		assert(false);
		return false;
	}
	if (!OpenNpc(the_npc))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "打开某个城镇的指定NPC之，打开NPC面板失败，城镇名：" << village_name << " npc_id：" << obj_db_id
			<< " 房间号：" << room_num << " 是否本地图优先：" << cur_village_first;
		return false;
	}
	return true;
}

bool scmd::OpenTheNpcOfVillage(const std::string& village_name, int room_num, const std::string& npc_name, 
	const std::string& npc_title, bool cur_village_first)
{
	if (npc_name.empty())
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "打开某个城镇的某个NPC之，npc名字不能为空，城镇名：" << village_name << " npc名：" << 
			npc_name << " 房间号：" << room_num << " 是否本地图优先：" << cur_village_first;
		return false;
	}
	if (cur_village_first)
	{
		auto& gpm = GPlayerMe::GetMe();
		gpm.UpdateOwnerRoomNum();
		if (room_num == gpm.GetOwnerRoomIdx() && !scmd::GetMe().IsInFb())
		{
			auto the_npc = FindNpcByNameImpl(npc_name, npc_title);
			if (IsValidGameObj(the_npc))
			{
				if (SetRoleToBack(the_npc, -1))
				{
					if (OpenNpc(the_npc))
						return true;
				}
			}
		}
	}

	assert(room_num >= 0);
	if (!village_name.empty() && !MoveToTheVillage(village_name, room_num))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "打开某个城镇的某个NPC之，移动城镇失败，城镇名：" << village_name << " npc名：" << npc_name << " 房间号：" << room_num << " 是否本地图优先：" << cur_village_first;
		return false;
	}

	auto the_npc = FindNpcByNameImpl(npc_name, npc_title);
	if (!IsValidGameObj(the_npc))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "打开某个城镇的某个NPC之，没有这样的NPC，城镇名：" << village_name << " npc名：" << npc_name << " 房间号：" << room_num << " 是否本地图优先：" << cur_village_first;
		return false;
	}
	if (!SetRoleToBack(the_npc, -1))
	{
		assert(false);
		return false;
	}
	if (!OpenNpc(the_npc))
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_debug) << "打开某个城镇的某个NPC之，打开NPC面板失败，城镇名：" << village_name << " npc名：" << npc_name << " 房间号：" << room_num << " 是否本地图优先：" << cur_village_first;
		return false;
	}
	return true;
}

bool scmd::AutoMakeEquip(const std::string& equip_name, int equip_max_cnt)
{
	if (equip_name.empty())
	{
		CMN_ASSERT(false);
		return false;
	}
	if (equip_max_cnt <= 0)
		equip_max_cnt = std::numeric_limits<int>::max();
	auto& item_mgr = ItemDbMgr::GetMe();
	item_mgr.RebuildAll();
	auto item_template = item_mgr.FindByMatchName(equip_name);
	if (!item_template)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "不存在这样的装备，装备名：" << equip_name;
		return false;
	}
	auto& gpm = GPlayerMe::GetMe();
	if (gpm.FindTotalItem(item_template->item_id_).item_cnt_ >= equip_max_cnt)
		return true;

	auto& craft_mgr = GClass::CraftDbMgr::GetMe();
	craft_mgr.Update();
	auto craft_item = craft_mgr.FindByDstItemId(item_template->item_id_);
	if (!craft_item)
	{
		LOG_ENTER_FUNC;
		LOG_O(Log_error) << "没有相应的打造信息，装备名：" << item_template->item_name_;
		return false;
	}

	//得到所需材料
	std::vector<GPlayerMe::stNeedMaterialInfo> material_info;

	for (auto& v : craft_item->materials_)
	{
		const auto& the_item = gpm.FindTotalItem(v.material_item_id_);
		if (the_item.item_cnt_ >= v.item_cnt_)
			continue;
		GPlayerMe::stNeedMaterialInfo tmp;
		tmp.material_item_id_ = v.material_item_id_;
		tmp.need_cnt_ = v.item_cnt_ - the_item.item_cnt_;
		tmp.total_item_cnt_ = v.item_cnt_;
		CMN_ASSERT(tmp.need_cnt_ > 0);
		material_info.push_back(tmp);
	}
	if (material_info.empty())
		return gpm.DoCraftByDstEquipItemId(*craft_item);
	int cnt = 0;
	LOG_ENTER_FUNC;
	for (auto& v : material_info)
	{
		if (v.need_cnt_ <= 0)
		{
			assert(false);
			continue;
		}
		auto material_template = item_mgr.FindById(v.material_item_id_);
		if (!material_template)
		{
			assert(false);
			continue;
		}
		if (MM_ExistTheItem(material_template->item_name_))
		{
			LOG_O(Log_info) << "自动打造装备之去获得该材料：" << material_template->item_name_ << "  材料数量上限：" << v.total_item_cnt_ << " 所需材料数：" << v.need_cnt_;
			if (MM_DoGainTheMaterial(material_template->item_name_, v.total_item_cnt_))
			{
				++cnt;
			}
			else
			{
				LOG_O(Log_error) << "自动打造装备之获取该材料失败：" << material_template->item_name_;
			}
		}
		else
		{
			LOG_O(Log_error) << "自动打造装备之找不到材料处理映射，无法去获得该材料：" << material_template->item_name_;
		}
	}

	return gpm.DoCraftByDstEquipItemId(*craft_item);
}

//换号
void scmd::ExchangeGameAc()
{
	auto func_obj = FuncObj_NormalC::GetInstance();
	if (!func_obj)
		return;
	func_obj->GetSessionOper().ToCall(BC_MakeP("ExchangeGameAc"));
}

bool scmd::IsInFb()
{
	auto& gpm = GPlayerMe::GetMe();
	gpm.UpdateCurMapInfo_Notify();
	return gpm.IsInFb();
}

bool scmd::SetHitMode_UsePacketItem(const std::string& item_name, int steps)
{
	if (item_name.empty())
		return false;
	return GPlayerMe::GetMe().SetHitMode_UsePacketItem(item_name, steps);
}

int scmd::GetTaskNpcId(bool is_accept, int task_id)
{
	auto& task_db_mgr = GTaskDbMgr::GetMe();
	task_db_mgr.RebuildAll();
	auto task_db = task_db_mgr.FindById(task_id);
	if (!task_db)
	{
		LOG_O(Log_error) << "没有这样的任务ID：" << task_id;
		return kInvalidIntValue;
	}
	if (is_accept)
		return task_db->accept_npc_.obj_db_id_;
	return task_db->complete_npc_.obj_db_id_;
}

int scmd::GetCurFbValidConsumeMode()
{
	return (int)GPlayerMe::GetMe().GetCurFbValidConsumeMode();
}

int scmd::GetItemRoleLvl(const GItem& item)
{
	return item.player_lvl_;
}

bool scmd::SetCurTimeHitMode(const std::string& str_hit_mode)
{
	using namespace GClass;
	static const std::pair<pt_csz, GHitMonsterMode_Mouse::enHitMode> kHitModeMap[] = {
		{ "右键攻击", GHitMonsterMode_Mouse::kRightHit }
	};
	for (auto& v : kHitModeMap)
	{
		if (v.first == str_hit_mode)
		{
			GPlayerMe::GetMe().SetCurHitMode(v.second);
			return true;
		}
	}

	LOG_O(Log_error) << "没有这样的攻击模式：" << str_hit_mode << " 只支持这些攻击模式：";
	for (auto& v : kHitModeMap)
	{
		LOG_O(Log_info) << v.first;
	}
	return false;
}

bool scmd::IsTaiDaoType(const GItem& item)
{
	return CD_En_EquipPartSubType_Weapon::kEPSTW_TaiDao == item.EquipSubType2Weapon();
}

void scmd::EnableRecvDebugInof(bool enable)
{
	return GRecvMsgMgr::GetMe().EnableOutput(enable);
}

void scmd::TestTheFbType(const luabind::object& fb_tbl, const luabind::object& fb_types)
{
	using namespace luabind;
	NameList<int, std::vector<int> > name_list_fb_type;
	for (iterator it(fb_types), it_end; it != it_end; ++it)
	{
		auto id = ObjectCast(*it, 0);
		if (id != 0)
			name_list_fb_type.Add(id);
	}
	auto& all_fb = MapDbMgr::GetMe();
	all_fb.Update();
	for (iterator it(fb_tbl), it_end; it != it_end; ++it)
	{
		auto& v = *it;
		auto& fb_name = v[1];
		const auto& the_fb_name = ObjectCast(fb_name, kEmptyStr);
		auto fb_db = all_fb.FindByName(the_fb_name);
		if (!fb_db)
		{
			LOG_O(Log_debug) << "副本：" << the_fb_name << " 没有副本信息。";
			continue;
		}
		if (!name_list_fb_type.IsIn((int)fb_db->fb_type_))
		{
			LOG_O(Log_debug) << "副本：" << the_fb_name << " 副本类型不对，真实类型为：" << fb_db->fb_type_;
		}
	}
}

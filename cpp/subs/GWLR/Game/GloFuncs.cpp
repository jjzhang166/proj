#include "stdafx.h"
#include "GloFuncs.h"
#include "FuncObjC.h"
#include <Common/CmnDllApi.h>
#include <CD_CommonFiles/Data/CD_UsefulFns.h>
#include "GLua/Scmd.h"
#include "Facility/GPlayer.h"

TimeDuration& GetUpdateTimeDuration()
{
	static TimeDuration td(1000);
	return td;
}

TimeDuration& GetSendTimeDuration()
{
	//发包间隔最好不要小于1秒
	static TimeDuration td(1500);
	return td;
}

int Offset2DllAddr( int offset )
{
	return CDF_Offset2CryGameAddr(offset);
}

void DxKeyUpDown( DWORD virtual_key, bool is_down )
{
	auto norc = FuncObj_NormalC::GetInstance();
	if ( !norc )
		return;
	norc->DxKeyUpDown(virtual_key, is_down);
}

int G_MessageBoxTimeoutA( LPCSTR lpText, LPCSTR lpCaption, DWORD dwMilliseconds )
{
	return MessageBoxTimeout(nullptr, lpText, lpCaption, MB_OKCANCEL, 0, dwMilliseconds);
}

int PackageSomeItemToStore(const std::string& item_name, int cnt, const std::function<bool(const GItem&)>& item_filter)
{
	if (scmd::GetMe().IsInFb())
		return 0;
	if (cnt <= 0)
		cnt = std::numeric_limits<int>::max();

	auto& gpm = GPlayerMe::GetMe();
	auto& package = gpm.GetGridPackage();
	package.RebuildAll();
	auto item_ptr = package.TraverseByMatchName_Cnt(item_name, cnt);
	if (!item_ptr)
	{
		assert(false);
		return 0;
	}
	std::vector<GItem> items;
	for (item_ptr->First(); cnt > 0 && !item_ptr->IsDone(); item_ptr->Next())
	{
		auto& item = item_ptr->CurItem();
		if (item_filter(item))
		{
			cnt -= item.item_cnt_;
			items.push_back(item);
		}
	}
	int move_cnt = 0;
	for (auto& item : items)
	{
		CD_En_TabCategory tbl_category_dst;
		if (item.IsSuCai())
			tbl_category_dst = CD_En_TabCategory::kTC_SuCai;
		else
			tbl_category_dst = CD_En_TabCategory::kTC_Store;
		if (!gpm.SmartMoveItem(item, tbl_category_dst, -1))
			return move_cnt;
		++move_cnt;
	}
	return move_cnt;
}

int StoreSomeItemToPackage(const std::string& item_name, int cnt, const std::function<bool(const GItem&)>& item_filter)
{
	if (scmd::GetMe().IsInFb())
		return 0;
	if (cnt <= 0)
		cnt = std::numeric_limits<int>::max();

	auto& gpm = GPlayerMe::GetMe();
	auto& tab_store = gpm.GetStorePackage();
	tab_store.RebuildAll();
	auto item_ptr = tab_store.TraverseByMatchName_Cnt(item_name, cnt);
	if (!item_ptr)
	{
		assert(false);
		return 0;
	}
	std::vector<GItem> items;
	for (item_ptr->First(); cnt > 0 && !item_ptr->IsDone(); item_ptr->Next())
	{
		auto& item = item_ptr->CurItem();
		if (item_filter(item))
		{
			cnt -= item.item_cnt_;
			items.push_back(item);
		}
	}

	auto& tab_sucai = gpm.GetGridSuCai();
	tab_sucai.RebuildAll();
	item_ptr = tab_sucai.TraverseByMatchName_Cnt(item_name, cnt);
	if (!item_ptr)
	{
		assert(false);
		return 0;
	}
	for (item_ptr->First(); cnt > 0 && !item_ptr->IsDone(); item_ptr->Next())
	{
		auto& item = item_ptr->CurItem();
		if (item_filter(item))
		{
			cnt -= item.item_cnt_;
			items.push_back(item);
		}
	}

	int move_cnt = 0;
	for (auto& item : items)
	{
		if (!gpm.SmartMoveItem(item, CD_En_TabCategory::kTC_Package, -1))
			return move_cnt;
		++move_cnt;
	}
	return move_cnt;
}

bool g_enable_pass_room_ = true;
void EnablePassRoom(bool enable)
{
	g_enable_pass_room_ = enable;
}

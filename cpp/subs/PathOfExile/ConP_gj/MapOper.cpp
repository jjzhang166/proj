#include "stdafx.h"
#include "MapOper.h"
#include "Console.h"

BEGIN_MESSAGE_MAP(MyMfcMap, MfcMap)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_MESSAGE_MAP(DlgMapOper, CDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BT_UPDATE_MAP, &DlgMapOper::OnBtUpdateMap)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////
const int kSizeWidth = 50;
const int kSizeHeight = 50;
const UINT kTimerId_UpdatePos = 2000;
//////////////////////////////////////////////////////////////////////////
stMfcMapData MyMfcMap::GetMapData() const
{
	struct stTestHelp{
		stTestHelp(){
			ZeroMemory(data_, sizeof(data_));
			int idx = 0;
			for (int y = 0; y < kSizeHeight; ++y)
			{
				for (int x = 0; x < kSizeWidth; ++x)
				{
					++idx;
					if (idx % 3 == 0)
						data_[y][x] = 1;
				}
			}
		}

		pt_byte		data_[kSizeWidth][kSizeHeight];
	};
	/*static stTestHelp data;
	stMfcMapData res;
	res.map_data_ = (stMfcMapData::MapDataT)(void*)data.data_;
	res.map_size_.SetSize(kSizeWidth, kSizeHeight);*/

	stMfcMapData res;
	res.map_data_ = (stMfcMapData::MapDataT)(void*)map_data_;
	res.map_size_ = map_size_;
	return res;
}

MyMfcMap::MyMfcMap(DlgMapOper* dlg_map_oper)
{
	dlg_map_oper_ = dlg_map_oper;
	map_data_ = (stMfcMapData::MapDataT)(void*)new pt_byte[CG_kMaxAStarSize * CG_kMaxAStarSize];
	timer_update_pos_ = 0;
}

MyMfcMap::~MyMfcMap()
{
	typedef pt_byte(*ArrayT_)[CG_kMaxAStarSize * CG_kMaxAStarSize];
	ArrayT_ ptr = (ArrayT_)(void*)map_data_;
	delete[] ptr;
}

void MyMfcMap::SetMapData(const CG_MapBitset& bset, const stSizeInt& map_size, const std::string& map_name)
{
	map_size_ = map_size;
	assert(map_data_);
	for (int y = 0; y < map_size.y; ++y)
	{
		auto idx_h = y * map_size.x;
		auto map_data_y = &(*map_data_)[idx_h];
		for (int x = 0; x < map_size.x; ++x)
		{
			if (bset.test(idx_h + x))
				map_data_y[x] = 1;
			else
				map_data_y[x] = 0;
		}
	}
	if (map_name_ != map_name)
	{
		Update();
		assert(dlg_map_oper_);
		dlg_map_oper_->SetMapName(map_name);
	}
	else
		Update(map_size);
	map_name_ = map_name;

	//////////////////////////////////////////////////////////////////////////
	if (!timer_update_pos_)
	{
		timer_update_pos_ = SetTimer(kTimerId_UpdatePos, 600, nullptr);
	}
}

void MyMfcMap::SetDlgMapOper(DlgMapOper* dlg_map_oper)
{
	dlg_map_oper_ = dlg_map_oper;
}

void MyMfcMap::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == kTimerId_UpdatePos)
	{
		assert(dlg_map_oper_);
		dlg_map_oper_->PostToSessionOper([](MyAcOper* ac_oper){
			auto oper = ac_oper->GetSessionOper();
			if (!oper)
			{
				assert(false);
				return;
			}
			oper->ToCall(BC_MakeP("GetPlayerPos"));
		});
	}
	__super::OnTimer(nIDEvent);
}

void MyMfcMap::OnDestroy()
{
	__super::OnDestroy();
	KillTimer(kTimerId_UpdatePos);
	timer_update_pos_ = 0;
}

void MyMfcMap::SetPlayerPos(const stVec2Int& player_pos)
{
	UpdatePlayerPos(player_pos);
}

void MyMfcMap::OnDbClick(const stVec2Int& map_pos, const stVec2Int& wnd_pos)
{
	assert(dlg_map_oper_);
	auto ac_oper = dlg_map_oper_->GetAcOper();
	if (!ac_oper)
	{
		assert(false);
		return;
	}
	auto& item_key = ac_oper->GetAcItemKey();
	ac_oper->ModifyAcItemData(item_key, "dstPosX", std::to_string(map_pos.x));
	ac_oper->ModifyAcItemData(item_key, "dstPosY", std::to_string(map_pos.y));
	ac_oper->RunScript("MoveToDst");
}

DlgMapOper::DlgMapOper(UINT nIDTemplate, MyAcOper* pParentWnd) 
	: CDialog(nIDTemplate, pParentWnd), dlg_map_(this)
{
	ac_oper_ = pParentWnd;
}

DlgMapOper::DlgMapOper(MyAcOper* pParentWnd) : dlg_map_(this)
{
	ac_oper_ = pParentWnd;
}

BOOL DlgMapOper::OnInitDialog()
{
	if (!__super::OnInitDialog())
	{
		assert(false);
		return FALSE;
	}
	if (!dlg_map_.Create(IDD_DLG_MAP, this))
	{
		assert(false);
		return FALSE;
	}
	CRect rc;
	GetWindowRect(rc);
	auto cx = rc.Width(), cy = rc.Height();
	if (!rc.IsRectEmpty())
	{
		dlg_map_.SetWindowPos(NULL, rc.left, rc.top, cx - 10, cy - 100, SWP_NOMOVE);
	}
	dlg_map_.ShowWindow(SW_SHOW);
	return TRUE;
}

void DlgMapOper::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);
	/*if (dlg_map_.m_hWnd && nType != SIZE_MINIMIZED && cx != 0 && cy != 0)
	{
		CRect rc;
		GetWindowRect(rc);
		if (!rc.IsRectEmpty())
		{
			dlg_map_.SetWindowPos(NULL, rc.left, rc.top, cx - 10, cy - 100, SWP_NOMOVE);
		}
	}*/
}

void DlgMapOper::OnBtUpdateMap()
{
	PostToSessionOper([](MyAcOper* ac_oper){
		auto oper = ac_oper->GetSessionOper();
		if (!oper)
		{
			assert(false);
			return;
		}
		oper->ToCall(BC_MakeP("GetMapData"));
	});
}

MyMfcMap* DlgMapOper::GetMfcMap()
{
	if (dlg_map_.IsWindowVisible())
		return &dlg_map_;
	return nullptr;
}

void DlgMapOper::SetMapName(const std::string& map_name)
{
	if (my_wnd_text_.IsEmpty())
	{
		GetWindowText(my_wnd_text_);
	}
	auto wnd_text = my_wnd_text_;
	wnd_text += L"-----";
	wnd_text += CA2CT(map_name.c_str());
	SetWindowText(wnd_text);
}

void DlgMapOper::OnDestroy()
{
	__super::OnDestroy();
	dlg_map_.CloseWindow();
}

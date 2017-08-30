#include "stdafx.h"
#include <mfc/MfcMap.h>

BEGIN_MESSAGE_MAP(MfcMap, CDialog)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

void MfcMap::OnPaint()
{
	//一定不能调用__super::OnPaint()，否则后面便不能使用CPaintDC了，只能使用CClientDC
	//__super::OnPaint();
	CPaintDC dc(this);
	CRect paint_rect = dc.m_ps.rcPaint;
	if (!inited_by_dc_)
	{
		//CRect paint_rect;
		//GetClientRect(paint_rect);
		if (!InitByDc(dc, paint_rect))
		{
			assert(false);
			return;
		}
		inited_by_dc_ = true;
	}
	//////////////////////////////////////////////////////////////////////////
	DoPaint(paint_rect);
	if (!dc.BitBlt(paint_rect.left, paint_rect.top, paint_rect.Width(), paint_rect.Height(), &dc_, paint_rect.left, paint_rect.top, SRCCOPY))
	{
		assert(false);
		return;
	}
	/*if (!dc.BitBlt(0, 0, rect_.Width(), rect_.Height(), &dc_, 0, 0, SRCCOPY))
	{
		assert(false);
		return;
	}*/
}

void MfcMap::OnCancel()
{

}

BOOL MfcMap::OnInitDialog()
{
	if (!__super::OnInitDialog())
	{
		assert(false);
		return FALSE;
	}
	if (!pen_block_.m_hObject && !pen_block_.CreatePen(PS_DOT, 1, GetBlockPointColor()))
	{
		assert(false);
		return FALSE;
	}
	if (!pen_player_.m_hObject && !pen_player_.CreatePen(PS_DOT, 5, GetPlayerPointColor()))
	{
		assert(false);
		return FALSE;
	}
	return TRUE;
}

bool MfcMap::InitByDc(CDC& wnd_dc, const CRect& rect)
{
	if (!dc_.CreateCompatibleDC(&wnd_dc))
	{
		assert(false);
		return false;
	}
	if (!bit_map_.CreateCompatibleBitmap(&wnd_dc, rect.Width(), rect.Height()))
	{
		assert(false);
		return false;
	}
	rect_ = rect;
	dc_.SelectObject(bit_map_);
	return true;
}

void MfcMap::DoPaint(const CRect& paint_rect)
{
	auto map_data = GetMapData();
	if (!map_data.IsValid())
	{
		//assert(false);
		return;
	}
	//背景色
	dc_.FillSolidRect(&paint_rect, GetMovablePointColor());
	auto map_rate = GetMapRate();
	dc_.SelectObject(pen_block_);
	auto block_color = GetBlockPointColor();
	auto& the_map_data = *map_data.map_data_;
	for (int y = paint_rect.top; y < paint_rect.bottom; ++y)
	{
		int map_y = int(float(y) / map_rate);
		if (map_y < 0 || map_y >= map_data.map_size_.y)
		{
			for (int x = paint_rect.left; x < paint_rect.right; ++x)
			{
				::SetPixel(dc_.m_hDC, x, y, block_color);
			}
		}
		else
		{
			auto the_map_data_y = &the_map_data[map_y * map_data.map_size_.x];
			for (int x = paint_rect.left; x < paint_rect.right; ++x)
			{
				int map_x = int(float(x) / map_rate);
				if (map_x < 0 || map_x >= map_data.map_size_.x)
					goto LABEL_SET_PIXEL;
				if (the_map_data_y[map_x] > 0)
					goto LABEL_SET_PIXEL;
				/*if (map_data.IsBlock(map_x, map_y))
					goto LABEL_SET_PIXEL;*/
				continue;
			LABEL_SET_PIXEL:
				//dc_.SetPixel(x, y, block_color);
				::SetPixel(dc_.m_hDC, x, y, block_color);
				//(void*)0;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (player_wnd_pos_.x >= 0 && player_wnd_pos_.y >= 0)
	{
		dc_.SelectObject(pen_player_);
		auto rc_circle = GetPlayerWndRect();
		dc_.Ellipse(rc_circle);
	}
}

COLORREF MfcMap::GetMovablePointColor() const
{
	return RGB(255, 255, 255);
}

COLORREF MfcMap::GetBlockPointColor() const
{
	return RGB(0, 0, 0);
}

void MfcMap::OnDestroy()
{
	__super::OnDestroy();
	inited_by_dc_ = false;
	bit_map_.DeleteObject();
	dc_.DeleteDC();
}

MfcMap::MfcMap() : player_wnd_pos_(-1, -1)
{
	inited_by_dc_ = false;
}

void MfcMap::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	stVec2Int wnd_pos(point.x, point.y);
	auto map_pos = WndPos2MapPos(wnd_pos);
	OnDbClick(map_pos, wnd_pos);
}

float MfcMap::GetMapRate() const
{
	auto map_data = GetMapData();
	if (!map_data.IsValid())
	{
		assert(false);
		return 0;
	}
	float map_rate = float(rect_.Width()) / float(map_data.map_size_.Width());
	float map_rate_heigh = float(rect_.Height()) / float(map_data.map_size_.Height());
	if (map_rate > map_rate_heigh)
		map_rate = map_rate_heigh;
	return map_rate;
}

stVec2Int MfcMap::WndPos2MapPos(const stVec2Int& wnd_pos) const
{
	auto map_rate = GetMapRate();
	return stVec2Int(int(float(wnd_pos.x) / map_rate), int(float(wnd_pos.y) / map_rate));
}

stVec2Int MfcMap::MapPos2WndPos(const stVec2Int& map_pos) const
{
	auto map_rate = GetMapRate();
	return stVec2Int(int(float(map_pos.x) * map_rate), int(float(map_pos.y) * map_rate));
}

void MfcMap::UpdateTheWndRect(const CRect& wnd_rect)
{
	InvalidateRect(wnd_rect, FALSE);
}

void MfcMap::UpdateMapRect(const CRect& map_rect)
{
	auto map_rate = GetMapRate();
	CRect wnd_rect;
	wnd_rect.left = int(float(map_rect.left) * map_rate);
	wnd_rect.top = int(float(map_rect.top) * map_rate);
	wnd_rect.right = wnd_rect.left + int(float(map_rect.Width()) * map_rate);
	wnd_rect.bottom = wnd_rect.top + int(float(map_rect.Height()) * map_rate);
	UpdateTheWndRect(wnd_rect);
}

COLORREF MfcMap::GetPlayerPointColor() const
{
	return RGB(255, 0, 0);
}

void MfcMap::UpdatePlayerPos(const stVec2Int& player_map_pos)
{
	const int kLastDelta = 3;
	auto player_wnd_rect = GetPlayerWndRect();
	player_wnd_pos_.x = -1;
	player_wnd_rect.left -= kLastDelta;
	player_wnd_rect.top -= kLastDelta;
	player_wnd_rect.right += kLastDelta;
	player_wnd_rect.bottom += kLastDelta;
	UpdateTheWndRect(player_wnd_rect);
	player_wnd_pos_ = MapPos2WndPos(player_map_pos);
	player_wnd_rect = GetPlayerWndRect();
	UpdateTheWndRect(player_wnd_rect);
}

CRect MfcMap::GetPlayerWndRect() const
{
	const int kPlayerCircleRadius = 5;
	return CRect(player_wnd_pos_.x - kPlayerCircleRadius, player_wnd_pos_.y - kPlayerCircleRadius,
		player_wnd_pos_.x + kPlayerCircleRadius, player_wnd_pos_.y + kPlayerCircleRadius);
}

void MfcMap::OnDbClick(const stVec2Int& map_pos, const stVec2Int& wnd_pos)
{

}

void MfcMap::Update()
{
	Invalidate(FALSE);
}

void MfcMap::Update(const stSizeInt& map_size)
{
	POINT pos = { 0 };
	SIZE size = {
		map_size.x, map_size.y
	};
	CRect rc(pos, size);
	UpdateMapRect(rc);
}

stMfcMapData::stMfcMapData()
{
	map_data_ = nullptr;
}

bool stMfcMapData::IsValid() const
{
	if (!map_data_)
		return false;
	if (map_size_.x <= 0 || map_size_.y <= 0)
	{
		//assert(false);
		return false;
	}
	return true;
}

bool stMfcMapData::IsBlock(int x, int y) const
{
	if (x < 0 || x >= map_size_.x) return true;
	if (y < 0 || y >= map_size_.y) return true;
	return (*map_data_)[y * map_size_.x + x] > 0;
}

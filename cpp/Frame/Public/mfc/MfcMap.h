#pragma once
/*
@author zhp
@date 2017/7/20 14:16
@purpose for map
*/
#include <afxwin.h>
#include <ProtocolX/P_BaseTypeX.h>
#include <CommonX/CmnX_Math.h>

struct stMfcMapData{
	typedef pt_byte (*MapDataT)[1];

	stMfcMapData();
	bool IsValid() const;
	bool CanMove(const stVec2Int& pos) const;
	bool IsBlock(int x, int y) const;

	MapDataT			map_data_;
	stSizeInt			map_size_;
};

class MfcMap : public CDialog{
public:
	MfcMap();
	void Update();
	void Update(const stSizeInt& map_size);
	void UpdateMapRect(const CRect& map_rect);
	void UpdatePlayerPos(const stVec2Int& player_map_pos);

protected:
	BOOL OnInitDialog() override;
	virtual stMfcMapData GetMapData() const = 0;
	void OnCancel() override;
	virtual void OnDbClick(const stVec2Int& map_pos, const stVec2Int& wnd_pos);

protected:
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

private:
	bool InitByDc(CDC& wnd_dc, const CRect& rect);
	void DoPaint(const CRect& paint_rect);
	COLORREF GetMovablePointColor() const;
	COLORREF GetBlockPointColor() const;
	COLORREF GetPlayerPointColor() const;
	float GetMapRate() const;
	stVec2Int WndPos2MapPos(const stVec2Int& wnd_pos) const;
	stVec2Int MapPos2WndPos(const stVec2Int& map_pos) const;
	void UpdateTheWndRect(const CRect& wnd_rect);
	CRect GetPlayerWndRect() const;

private:
	CDC			dc_;
	CBitmap		bit_map_;
	CRect		rect_;
	CPen		pen_block_;
	CPen		pen_player_;
	stVec2Int	player_wnd_pos_;
	bool		inited_by_dc_;

	DECLARE_MESSAGE_MAP()
};
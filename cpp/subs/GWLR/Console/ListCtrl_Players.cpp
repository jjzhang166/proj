#include "stdafx.h"
#include "ListCtrl_Players.h"

void CListCtrlStyled::PreSubclassWindow()
{
	__super::PreSubclassWindow();
	//style
	{
		DWORD dwStyle = GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;		//选中某行使整行高亮（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_GRIDLINES;			//网格线（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_CHECKBOXES;			//item前生成checkbox控件
		SetExtendedStyle(dwStyle);				//设置扩展风格
	}

	const auto kColumnWidth = 175;
	InsertColumn(kItemName, _T("物品"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kItemCnt, _T("物品数量"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSeller, _T("拍卖人"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kPriceBegin, _T("起拍价"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kPriceSmart, _T("一口价"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kPriceJingPai, _T("竞拍价"), LVCFMT_LEFT, kColumnWidth);
}

void CListCtrlStyled::AddOfferItem( const stAuctionItemInfoEx& offer_item )
{
	const auto kItemMax = GetItemCount();
	InsertItem(kItemMax, CA2CT(offer_item.item_name_.c_str()));
	
	CString str_tmp;
	str_tmp.Format(_T("%d"), offer_item.item_cnt_);
	SetItemText(kItemMax, kItemCnt, str_tmp);	
	SetItemText(kItemMax, kSeller, CA2CT(offer_item.req_item_by_player_name_.c_str()));
	str_tmp.Format(_T("%d"), offer_item.begin_price_);
	SetItemText(kItemMax, kPriceBegin, str_tmp);
	str_tmp.Format(_T("%d"), offer_item.smart_price_);
	SetItemText(kItemMax, kPriceSmart, str_tmp);
	str_tmp.Format(_T("%d"), offer_item.jing_pai_price_);
	SetItemText(kItemMax, kPriceJingPai, str_tmp);
}

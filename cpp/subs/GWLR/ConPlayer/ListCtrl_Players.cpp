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
}
void CList_MyOfferList::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 175;
	InsertColumn(kPrice, _T("价钱"), LVCFMT_LEFT, kColumnWidth);
#ifdef _DEBUG
	InsertColumn(kPlayerId, _T("球员ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kId, _T("拍卖的ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSpid, _T("spid"), LVCFMT_LEFT, kColumnWidth);
#endif
	InsertColumn(kSoldState, _T("拍卖状态"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kIssuedAt, _T("拍卖时间"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kGrade, _T("强化等级"), LVCFMT_LEFT, kColumnWidth);
}

void CList_OtherOffers::PreSubclassWindow()
{
	__super::PreSubclassWindow();

	const auto kColumnWidth = 225;
	InsertColumn(kPrice, _T("价钱"), LVCFMT_LEFT, kColumnWidth);
#ifdef _DEBUG
	InsertColumn(kId, _T("拍卖的ID"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kSpid, _T("spid"), LVCFMT_LEFT, kColumnWidth);
#endif
	InsertColumn(kIssuedAt, _T("拍卖时间"), LVCFMT_LEFT, kColumnWidth);
	InsertColumn(kGrade, _T("强化等级"), LVCFMT_LEFT, kColumnWidth);
}
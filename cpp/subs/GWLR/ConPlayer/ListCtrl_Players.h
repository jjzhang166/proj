#pragma once
/*
@author zhp
@date 2014/5/11 13:49
@purpose for test, players info
*/
#include "stdafx.h"
#include <Common/UsefulClass.h>

class CListCtrlStyled : public CListCtrl{
protected:
	virtual void PreSubclassWindow();
};

//我的出售列表
class CList_MyOfferList : public CListCtrlStyled, public Singleton<CList_MyOfferList, Singleton_Instance>{
	//type definitions
public:
	enum enColumns{
		kPrice = 0,			//价钱
#ifdef _DEBUG
		kPlayerId,
		kId,
		kSpid,
#endif
		kSoldState,					
		kIssuedAt,			//拍卖时间
		kGrade,				//强化等级
	};

protected:
	virtual void PreSubclassWindow();
};

//其他号的出售项
class CList_OtherOffers : public CListCtrlStyled, public Singleton<CList_OtherOffers, Singleton_Instance>{
	//type definitions
public:
	enum enColumns{
		kPrice = 0,			//价钱
#ifdef _DEBUG
		kId,
		kSpid,
#endif
		kIssuedAt,			//拍卖时间
		kGrade,				//强化等级
	};

protected:
	virtual void PreSubclassWindow();
};
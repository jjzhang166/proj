// MyRect.h: interface for the CMyRect class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYRECT_H__C26C1A73_8079_4D21_A0BB_D2D130419855__INCLUDED_)
#define AFX_MYRECT_H__C26C1A73_8079_4D21_A0BB_D2D130419855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyBaseShape.h"
#include "MyBrush.h"

class CMyRect  : public CMyBaseShape
{
	DECLARE_SERIAL(CMyRect)
public:
	CMyRect();
	CMyRect(CPoint&,CPoint&);
	virtual ~CMyRect();
public:
	void OnDraw(CDC* lpDc);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	virtual CPoint GetPoint1() const{ return m_theBegin; }
	virtual CPoint GetPoint2() const{ return m_theEnd; }
public:
	static CMyBaseShape* CreateShape();
	
public:
	CPoint m_theEnd;
	CPoint m_theBegin;
	CMyBrush m_RectBrush;
};

#endif // !defined(AFX_MYRECT_H__C26C1A73_8079_4D21_A0BB_D2D130419855__INCLUDED_)

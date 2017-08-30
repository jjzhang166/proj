// MyLine.h: interface for the CMyLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYLINE_H__216B3E42_6456_477A_A890_75C08B822488__INCLUDED_)
#define AFX_MYLINE_H__216B3E42_6456_477A_A890_75C08B822488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyBaseShape.h"

class CMyLine  : public CMyBaseShape
{
	DECLARE_SERIAL(CMyLine)
public:
	CMyLine();
	CMyLine(CPoint&,CPoint&);
	virtual ~CMyLine();
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
};

#endif // !defined(AFX_MYLINE_H__216B3E42_6456_477A_A890_75C08B822488__INCLUDED_)

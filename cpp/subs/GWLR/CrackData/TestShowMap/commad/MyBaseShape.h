// MyBaseShape.h: interface for the CMyBaseShape class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBASESHAPE_H__7D4387E5_E7FE_4005_B492_FB443C101A76__INCLUDED_)
#define AFX_MYBASESHAPE_H__7D4387E5_E7FE_4005_B492_FB443C101A76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyPen.h"

class CMyBaseShape : public CObject  
{
public:
	CMyBaseShape();
	virtual ~CMyBaseShape();
public:
	virtual void OnDraw(CDC* lpDc) = 0;
	virtual void OnMouseMove(UINT nFlags, CPoint point) = 0;
	virtual void OnLButtonDown(UINT nFlags, CPoint point) = 0;
	virtual CPoint GetPoint1() const = 0;
	virtual CPoint GetPoint2() const = 0;
public:
	CMyPen& GetPen()
	{
		return m_thePen;
	}
	void EditPen( int nPenStyle, int nWidth, COLORREF crColor )
	{
		m_thePen.EditPen(nPenStyle,nWidth,crColor);
	}
	char* GetName()
	{
		return m_theName;
	}
public:
	CMyPen m_thePen;
	char* m_theName;
};

#endif // !defined(AFX_MYBASESHAPE_H__7D4387E5_E7FE_4005_B492_FB443C101A76__INCLUDED_)

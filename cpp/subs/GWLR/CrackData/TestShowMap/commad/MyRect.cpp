// MyRect.cpp: implementation of the CMyRect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyRect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CMyRect, CObject, 0)

CMyRect::CMyRect()
{
	m_theName = "矩形";
}

CMyRect::CMyRect(CPoint& ptBegin,CPoint& ptEnd)
:m_theBegin(ptBegin),m_theEnd(ptEnd)
{
	m_theName = "矩形";
}

CMyRect::~CMyRect()
{

}

void CMyRect::OnDraw(CDC *lpDc)
{	
	//CPen( int nPenStyle画笔样式, int nWidth画笔宽度, COLORREF crColor画笔颜色 );
	HGDIOBJ hOldPen = lpDc->SelectObject(m_thePen);

	lpDc->SelectObject(m_RectBrush);
	if( lpDc != NULL )
	{
		lpDc->Rectangle(CRect(m_theBegin,m_theEnd));
	}

//	lpDc->SelectObject(hOldPen);
}

void CMyRect::OnMouseMove(UINT nFlags, CPoint point) 
{
	//起点只有一个，把每次移动都当成是鼠标的弹起来画
	//	m_theBegin = point;
	m_theEnd = point;
}

void CMyRect::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_theBegin = point;
	m_theName = "矩形";
}

CMyBaseShape* CMyRect::CreateShape()
{
	return new CMyRect;
}


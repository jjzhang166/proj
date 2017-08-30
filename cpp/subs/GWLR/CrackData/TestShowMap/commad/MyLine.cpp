// MyLine.cpp: implementation of the CMyLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CMyLine, CObject, 0)

CMyLine::CMyLine()
{
	m_theName = "线条";
}

CMyLine::~CMyLine()
{

}

CMyLine::CMyLine(CPoint& ptBegin,CPoint& ptEnd)
:m_theBegin(ptBegin),m_theEnd(ptEnd)
{
	m_theName = "线条";
}

void CMyLine::OnDraw(CDC *lpDc)
{	
	m_thePen.m_crColor = RGB(54,109,201);
	//该新对象替换先前的相同类型的对象,返回旧对象的数据
	HGDIOBJ hOldPen = lpDc->SelectObject(m_thePen);

	BYTE r = GetRValue(m_thePen.m_crColor);
	BYTE g = GetGValue(m_thePen.m_crColor);
	BYTE b = GetBValue(m_thePen.m_crColor);

 	lpDc->MoveTo(m_theBegin);
	lpDc->LineTo(m_theEnd);	

	lpDc->SelectObject(hOldPen);
}


void CMyLine::OnMouseMove(UINT nFlags, CPoint point) 
{
	//起点只有一个，把每次移动都当成是鼠标的弹起来画
	//	m_theBegin = point;
	m_theEnd = point;
}

void CMyLine::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_theName = "线条";
	m_theBegin = point;
}


CMyBaseShape* CMyLine::CreateShape()
{
	return new CMyLine;
}


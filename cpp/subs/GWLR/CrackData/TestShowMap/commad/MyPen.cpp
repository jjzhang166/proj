// MyPen.cpp: implementation of the CMyPen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyPen.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyPen::CMyPen()
{
	m_nPenStyle = PS_SOLID;
	m_nWidth = 2;
	m_crColor = RGB(255,0,0);
}

CMyPen::CMyPen( int nPenStyle, int nWidth, COLORREF crColor )
{
	m_nPenStyle = nPenStyle;
	m_nWidth = nWidth;
	m_crColor = crColor;
}

CMyPen::~CMyPen()
{

}

void CMyPen::SetColor(COLORREF color)
{
	m_crColor = color;
}

CArchive& operator<<(CArchive& ar,CMyPen& thePen)
{
	ar << thePen.m_crColor << thePen.m_nPenStyle << thePen.m_nWidth;

	return ar;
}

CArchive& operator>>(CArchive& ar,CMyPen& thePen)
{
	ar >> thePen.m_crColor >> thePen.m_nPenStyle >> thePen.m_nWidth;
	
	return ar;
}
// MyBrush.cpp: implementation of the CMyBrush class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBrush.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBrush::CMyBrush()
{
	m_Color = RGB(255,0,0);
	m_isNULL = NULL;
}

CMyBrush::~CMyBrush()
{

}

CArchive& operator<<(CArchive& ar,CMyBrush& thePen)
{
	ar << thePen.m_Color << thePen.m_isNULL;

	return ar;
}

CArchive& operator>>(CArchive& ar,CMyBrush& thePen)
{
	ar >> thePen.m_Color >> thePen.m_isNULL;

	return ar;
}
// MyPen.h: interface for the CMyPen class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYPEN_H__95E02612_8234_43E8_8A5F_394BE34595BB__INCLUDED_)
#define AFX_MYPEN_H__95E02612_8234_43E8_8A5F_394BE34595BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//CPen( int nPenStyle, int nWidth, COLORREF crColor );
//		样式			线条宽度			颜色
class CMyPen  
{
public:
	CMyPen();
	CMyPen( int nPenStyle, int nWidth, COLORREF crColor );
	virtual ~CMyPen();
	operator HPEN()
	{
		//DeleteObject 删除原有画笔的所有资源
		m_pen.DeleteObject();
		m_pen.CreatePen(m_nPenStyle,m_nWidth,m_crColor);
		return (HPEN)m_pen;
	}
public:
	void EditPen( int nPenStyle, int nWidth, COLORREF crColor )
	{
		m_nPenStyle = nPenStyle;
		m_nWidth = nWidth;
		m_crColor = crColor;
	}
public:
	void SetColor(COLORREF color);
public:
	int m_nPenStyle;
	int m_nWidth;
	COLORREF m_crColor;
public:
	CPen m_pen;

};

CArchive& operator<<(CArchive& ar,CMyPen& thePen);
CArchive& operator>>(CArchive& ar,CMyPen& thePen);

#endif // !defined(AFX_MYPEN_H__95E02612_8234_43E8_8A5F_394BE34595BB__INCLUDED_)

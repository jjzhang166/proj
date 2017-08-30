// MyBrush.h: interface for the CMyBrush class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBRUSH_H__681D929F_0E08_4A0B_826F_25889AA227BB__INCLUDED_)
#define AFX_MYBRUSH_H__681D929F_0E08_4A0B_826F_25889AA227BB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyBrush  
{
public:
	CMyBrush();
	virtual ~CMyBrush();
public:
	//HBRUSH 画刷句柄
	operator HBRUSH()
	{
		//删除画刷
		m_Brush.DeleteObject();
		//LOGBRUSH结构定义了物理刷子的风格、颜色和模板
		LOGBRUSH logbrush;
		logbrush.lbColor = m_Color;
		logbrush.lbHatch = HS_CROSS;
		logbrush.lbStyle = BS_SOLID;
		//创建画刷
		m_Brush.CreateBrushIndirect(&logbrush);
		return (HBRUSH)m_Brush;
	}
	void EditBrush( COLORREF Color,BOOL isNULL)
	{
		m_Color = Color;
		m_isNULL = isNULL;
	}
public:
	COLORREF m_Color;
	BOOL     m_isNULL;
private:
	CBrush   m_Brush;
};

CArchive& operator<<(CArchive& ar,CMyBrush& thePen);
CArchive& operator>>(CArchive& ar,CMyBrush& thePen);
#endif // !defined(AFX_MYBRUSH_H__681D929F_0E08_4A0B_826F_25889AA227BB__INCLUDED_)

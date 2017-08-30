
// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Console.h"
#include "ChildView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this);
}

int CChildView::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if ( __super::OnCreate(lpCreateStruct) == -1 )
		return -1;

	tab_options_.Create(WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 0);
	return 0;
}

void CChildView::OnSize( UINT nType, int cx, int cy )
{
	__super::OnSize(nType, cx, cy);
	if (nType != SIZE_MINIMIZED)
		tab_options_.SetWindowPos(NULL, 0, 0, cx, cy, 0);
}

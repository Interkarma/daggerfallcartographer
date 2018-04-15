// ChildView.cpp : implementation of the CChildView class
//

#include "stdafx.h"
#include "Daggerfall Cartographer.h"
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
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), NULL/*reinterpret_cast<HBRUSH>(COLOR_WINDOW+1)*/, NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CDFCartographerApp *pApp = (CDFCartographerApp*)::AfxGetApp();
	if ( pApp ) {
		pApp->PresentMediaView();
	}
	
	// Do not call CWnd::OnPaint() for painting messages
}


void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	CDFCartographerApp *pApp = (CDFCartographerApp*)::AfxGetApp();
	if ( pApp ) {
		pApp->SizeMediaView( cx, cy );
	}
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();

	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDFCartographerApp *pApp = (CDFCartographerApp*)::AfxGetApp();
	if ( pApp ) {
		pApp->OnKeyDown( nChar, nRepCnt, nFlags );
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

// ArenaBar.cpp : implementation file
//

#include "stdafx.h"
#include "Daggerfall Cartographer.h"
#include "ArenaBar.h"


// CArenaBar

CArenaBar::CArenaBar()
{
	// Initialise
	m_bIsCreated = FALSE;
}

CArenaBar::~CArenaBar()
{
}


BEGIN_MESSAGE_MAP(CArenaBar, baseCViewBar)
	ON_WM_CREATE()
	ON_WM_SIZE()
//	ON_WM_KEYDOWN()
END_MESSAGE_MAP()



// CArenaBar message handlers


int CArenaBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSizingControlBarG::OnCreate(lpCreateStruct) == -1)
		return -1;

	if ( !m_wndArenaDlg.Create( IDD_ARENA_DIALOG, this ) )
		return -1;

	// Add a column to the list view
	LVCOLUMN lvc;
	ZeroMemory( &lvc, sizeof(LVCOLUMN) );
	lvc.mask = LVCF_WIDTH;
	lvc.cx = 190;
	m_wndArenaDlg.m_ctrlArenaList.InsertColumn( 0, &lvc );

	// Create image list
	if ( !m_ilLocations.Create( IDB_LOCATION_TYPES_BITMAP, 16, 1, RGB(0,0,128) ) ) return FALSE;

	// Set image list for the tab controls and list control
	m_wndArenaDlg.m_ctrlRegionCombo.SetImageList( &m_ilLocations );
	m_wndArenaDlg.m_ctrlLocationCombo.SetImageList( &m_ilLocations );
	m_wndArenaDlg.m_ctrlArenaList.SetImageList( &m_ilLocations, LVSIL_SMALL );

	// Set created flag
	m_bIsCreated = TRUE;

	return 0;
}

void CArenaBar::OnSize(UINT nType, int cx, int cy)
{
	CSizingControlBarG::OnSize(nType, cx, cy);

	if ( m_bIsCreated ) {
		// Get size of this bar
		CRect rct;
		GetClientRect( rct );

		// Size the underlying dialog
		m_wndArenaDlg.SetWindowPos( NULL, 0, 0, rct.Width(), rct.Height(), SWP_DEFERERASE|SWP_NOZORDER );

		// Size region combo
		CRect win;
		m_wndArenaDlg.m_ctrlRegionCombo.GetWindowRect( win );
		m_wndArenaDlg.ScreenToClient( win );
		m_wndArenaDlg.m_ctrlRegionCombo.SetWindowPos( NULL, win.left, win.top, rct.Width(), win.Height(), SWP_DEFERERASE|SWP_NOZORDER );

		// Size location combo
		m_wndArenaDlg.m_ctrlLocationCombo.GetWindowRect( win );
		m_wndArenaDlg.ScreenToClient( win );
		m_wndArenaDlg.m_ctrlLocationCombo.SetWindowPos( NULL, win.left, win.top, rct.Width(), win.Height(), SWP_DEFERERASE|SWP_NOZORDER );

		// Size location list
		m_wndArenaDlg.m_ctrlArenaList.GetWindowRect( win );
		m_wndArenaDlg.ScreenToClient( win );
		m_wndArenaDlg.m_ctrlArenaList.SetWindowPos( NULL, win.left, win.top, rct.Width(), rct.Height() - win.top, SWP_DEFERERASE|SWP_NOZORDER );
	}
}

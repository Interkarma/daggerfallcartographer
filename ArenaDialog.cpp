// ArenaDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Daggerfall Cartographer.h"
#include "ArenaDialog.h"


// CArenaDialog dialog

IMPLEMENT_DYNAMIC(CArenaDialog, CDialog)
CArenaDialog::CArenaDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CArenaDialog::IDD, pParent)
{
}

CArenaDialog::~CArenaDialog()
{
}

void CArenaDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ARENA_LIST, m_ctrlArenaList);
	DDX_Control(pDX, IDC_ARENA_REGION_COMBO, m_ctrlRegionCombo);
	DDX_Control(pDX, IDC_ARENA_LOCATION_COMBO, m_ctrlLocationCombo);
}


BEGIN_MESSAGE_MAP(CArenaDialog, CDialog)
	ON_CBN_SELCHANGE(IDC_ARENA_LOCATION_COMBO, OnCbnSelchangeArenaLocationCombo)
	ON_CBN_SELCHANGE(IDC_ARENA_REGION_COMBO, OnCbnSelchangeArenaRegionCombo)
//	ON_NOTIFY(LVN_ITEMCHANGED, IDC_ARENA_LIST, OnLvnItemchangedArenaList)
//	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_ARENA_LIST, OnLvnItemActivateArenaList)
ON_NOTIFY(LVN_ITEMACTIVATE, IDC_ARENA_LIST, OnLvnItemActivateArenaList)
END_MESSAGE_MAP()


// CArenaDialog message handlers

void CArenaDialog::OnCbnSelchangeArenaLocationCombo()
{
	// Notify parent app
	CDFCartographerApp *pApp = (CDFCartographerApp*)::AfxGetApp();
	if ( pApp ) {
		pApp->OnFilterSelected();
	}
}

void CArenaDialog::OnCbnSelchangeArenaRegionCombo()
{
	// Notify parent app
	CDFCartographerApp *pApp = (CDFCartographerApp*)::AfxGetApp();
	if ( pApp ) {
		pApp->OnRegionSelected();
	}
}


void CArenaDialog::OnLvnItemActivateArenaList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	// Notify parent app
	CDFCartographerApp *pApp = (CDFCartographerApp*)::AfxGetApp();
	if ( pApp ) {
		pApp->OnLocationSelected( m_ctrlArenaList.GetItemData( pNMIA->iItem ) );
	}

	*pResult = 0;
}

void CArenaDialog::OnOK()
{
	// This must never be processed, or the view will vanish
}

void CArenaDialog::OnCancel()
{
	// This must never be processed, or the view will vanish
}

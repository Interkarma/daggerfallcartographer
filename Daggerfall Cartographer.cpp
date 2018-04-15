// Daggerfall Cartographer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Daggerfall Cartographer.h"
#include "OpenArena2Dialog.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDFCartographerApp

BEGIN_MESSAGE_MAP(CDFCartographerApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VIEW_ARENAVIEW, OnViewArenaView)
	ON_UPDATE_COMMAND_UI(ID_VIEW_ARENAVIEW, OnUpdateViewArenaView)
	ON_COMMAND(ID_FILE_OPENARENA2, OnFileOpenArena2)
	ON_COMMAND(ID_FILE_CLOSEARENA2, OnFileCloseArena2)
END_MESSAGE_MAP()


// CDFCartographerApp construction

CDFCartographerApp::CDFCartographerApp()
{
	// Initialise
	m_pMediaView = NULL;
	m_pArenaBar = NULL;
	m_lCurRegion = -1;
	m_lCurFilter = 0;
	m_lCurLocation = -1;
	m_nLastDFTextureCache = NULL;
	m_dwDrawMode = OBJDRAW_TEXTURED;

	// Redirect Log
	m_Alchemy.m_Log.Redirect( &m_Log );
}


// The one and only CDFCartographerApp object

CDFCartographerApp theApp;

// CDFCartographerApp initialization

BOOL CDFCartographerApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Capture startup directory
	char pBuffer[MAX_PATH];
	GetCurrentDirectory( MAX_PATH, pBuffer );
	m_strStartDirectory = pBuffer;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Gavin Clayton Applications"));

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object
	CMainFrame* pFrame = new CMainFrame;
	m_pMainWnd = pFrame;

	// create and load the frame with its resources
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);


	// Store pointer to frame window
	m_pFrame = pFrame;

	// Obtain a pointer to the Arena bar
	m_pArenaBar = (CArenaBar*)pFrame->GetArenaBar();

	// Store pointer to media view
	m_pMediaView = pFrame->GetChildView();

	// Starting DFC
	m_Log.Log( DFCAPP_ONINIT );

	// Start Alchemy
	ALCHEMY_CREATESTRUCT accs;
	ZeroMemory( &accs, sizeof(ALCHEMY_CREATESTRUCT) );
	accs.dwFlags = ACCS_ENABLETEXTUREFILTERING;
	accs.hInstance = m_hInstance;
	accs.hViewWnd = m_pMediaView->m_hWnd;
	accs.hInputWnd = m_pFrame->m_hWnd;
	accs.bWindowed = TRUE;
	accs.fNearPlaneDepth = 25.0f;
	accs.fFarPlaneDepth = 90000.0f;
	accs.colDirectionalLight = D3DCOLOR_RGBA(0xff,0xff,0xff,0xff);
	accs.colAmbientLight = D3DCOLOR_RGBA(0xff,0xff,0xff,0xff);
	accs.colRenderErase = D3DCOLOR_RGBA(0x80,0x80,0x80,0xff);
	D3DXVECTOR3 vLight = D3DXVECTOR3(1.0f, 0.0f, 1.0f );
	D3DXVec3Normalize( (D3DXVECTOR3*)&accs.vDirectionalLight, &vLight );
	if ( !m_Alchemy.Create( &accs ) ) {
		m_Log.Log( DFCAPP_FAILACCREATE, E_FAIL );
		return FailInitApp();
	}

	// Create font object
	m_nDefaultFont = m_Alchemy.InstallFont( "Arial", 10, NULL );
	if ( ACNULL == m_nDefaultFont ) {
		m_Log.Log( DFCAPP_FAILFONTCREATE, E_FAIL );
		return FailInitApp();
	}

	// The one and only window has been initialized, so show and update it
	pFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pFrame->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand

	// Clear and present
	m_Alchemy.Clear();
	m_Alchemy.Present();

#ifdef _DEBUG
	// DEBUG: Open my local Arena2 path for development
	m_DaggerTool.OpenArena2( "C:\\Data\\Common\\Storage\\Dagger\\Arena2" );
	InitialiseViews();
#endif

	return TRUE;
}


BOOL CDFCartographerApp::FailInitApp()
{
	// Output error to dialog on failure
	CString str;
	TLOGENTRY le;
	m_Log.GetLastErrorEntry( &le );
	str.Format( "Last Error: %s\nCheck \"%s\\%s\" for more information.\n", le.strMsg.GetBuffer(), m_strStartDirectory.GetBuffer(), DFCAPP_LOGNAME );
	MessageBox( m_pFrame->m_hWnd, str, DFCAPP_ONFAILSTART, MB_ICONWARNING | MB_OK );

	return FALSE;
}


int CDFCartographerApp::ExitInstance()
{
	// Close down Alchemy
	m_Alchemy.Destroy();

	// Save log to file
	CString strPath;
	m_Log.Log( DFCAPP_ONCLOSE );
	strPath.Format( "%s\\%s", m_strStartDirectory, DFCAPP_LOGNAME );
	m_Log.Save( strPath );

	return CWinApp::ExitInstance();
}


void CDFCartographerApp::SizeMediaView( int x, int y )
{
	// Scale the media view
	if ( m_Alchemy.IsCreated() ) {
		m_Alchemy.SetDisplaySize( x, y );
		DrawMediaView();
	}
}/* SizeMediaView */


void CDFCartographerApp::PresentMediaView()
{
	// Copy the back buffer into the view window
	if ( m_Alchemy.IsCreated() ) {
		m_Alchemy.Present();
	}
}/* PresentMediaView */


BOOL CDFCartographerApp::DrawMediaView()
{
	// TODO: Draw contents of media view
	m_Alchemy.Clear();
	m_Alchemy.Present();

	return TRUE;
}/* DrawMediaView */


// CDFCartographerApp message handlers



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CDFCartographerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CDFCartographerApp message handlers

void CDFCartographerApp::OnViewArenaView()
{
	if ( m_pArenaBar->IsVisible() )
		m_pFrame->ShowControlBar( m_pArenaBar, FALSE, FALSE );
	else
		m_pFrame->ShowControlBar( m_pArenaBar, TRUE, TRUE );
}

void CDFCartographerApp::OnUpdateViewArenaView(CCmdUI *pCmdUI)
{
	if ( m_pArenaBar->IsVisible() )
		pCmdUI->SetCheck();
	else
		pCmdUI->SetCheck( FALSE );
}

void CDFCartographerApp::OnFileOpenArena2()
{
	// Set profile string
	CString strSection = "Startup Values";
	CString strPath = "PreviousArena2Path";
	CString strReadOnly = "PreviousReadOnlyState";

	// Get previous settings from registry
	CString strNewPath = GetProfileString( strSection, strPath );
	BOOL bReadOnly = GetProfileInt( strSection, strReadOnly, TRUE );

	// Init open dialog
	COpenArena2Dialog dlg;
	dlg.m_strArena2Path = strNewPath;
	dlg.m_bOpenReadOnly = bReadOnly;

	// Start open dialog
	if ( dlg.DoModal() == IDOK ) {	
		// Open Arena2
		if ( !m_DaggerTool.OpenArena2( dlg.m_strArena2Path, dlg.m_bOpenReadOnly ) ) {
			MessageBox( NULL, "Failed to open Arena2 folder.\nContents may be incomplete or damaged.", "Error!", MB_ICONSTOP|MB_OK );
			return;
		}
		
		// Initialise views
		if ( !InitialiseViews() ) {
			m_DaggerTool.CloseArena2();
			InitialiseViews();
		}

		// Save data
		WriteProfileString( strSection, strPath, dlg.m_strArena2Path );
		WriteProfileInt( strSection, strReadOnly, dlg.m_bOpenReadOnly );
	}
}

void CDFCartographerApp::OnFileCloseArena2()
{
	m_DaggerTool.CloseArena2();
	InitialiseViews();

	// Initialise
	m_lCurRegion = -1;
	m_lCurFilter = 0;
}


BOOL CDFCartographerApp::SetRegion( long nRegion )
{
	_ASSERT( nRegion >= 0 && nRegion < NUM_REGION_COUNT );

	// Exit if Arena2 not open
	if ( !m_DaggerTool.IsArena2Open() ) {
		return FALSE;
	}

	// Open this region
	BSAMaps *pMaps = &m_DaggerTool.m_ArchMaps;
	if ( !pMaps->OpenRegion( nRegion ) ) {
		return FALSE;
	}

	// Delete current list of locations
	m_haLocation.DeleteAll();

	// Build a list of locations in this region
	int nCount = pMaps->GetLocationCount();
	if ( nCount ) {
		m_haLocation.NewPool( nCount );
		char pszBuffer[32];
		for ( int i = 0; i < nCount; i++ ) {
			pMaps->GetLocationName( i, pszBuffer );
			m_haLocation[i].strName = pszBuffer;
			m_haLocation[i].Type = pMaps->GetLocationType( i );
		}
	}

	BuildLocationView();

	m_lCurRegion = nRegion;

	return TRUE;
}/* SetRegion */


BOOL CDFCartographerApp::BuildLocationView()
{
	// Clear the list of names
	CListCtrl *pList = &m_pArenaBar->m_wndArenaDlg.m_ctrlArenaList;
	pList->DeleteAllItems();

	// Populate the list with names
	long nIcon, nType;
	long nCount = m_haLocation.GetCount();
	if ( nCount ) {
		int nItem = 0;
		for ( int i = 0; i < nCount; i++ ) {
			// Get item type
			nType = m_haLocation[i].Type;

			// Filter out items
			switch ( m_lCurFilter )
			{
			case DFCFLT_ALL:
				// Add all items
				break;

			case DFCFLT_DUNGEONS:
				// Add all Dungeon types
				if ( nType == DFCLOC_DUNGEON_LABYRINTHS || nType == DFCLOC_DUNGEON_KEEPS || nType == DFCLOC_DUNGEON_RUINS )
					break;
				else
					continue;

			case DFCFLT_GRAVEYARDS:
				// Add all Graveyard types
				if ( nType == DFCLOC_GRAVEYARD_COMMON || nType == DFCLOC_GRAVEYARD_FORGOTTEN )
					break;
				else
					continue;

			case DFCFLT_HOMES:
				// Add all Home types
				if ( nType == DFCLOC_HOME_FARMS || nType == DFCLOC_HOME_WEALTHY || nType == DFCLOC_HOME_POOR || nType == DFCLOC_HOME_YOURSHIPS )
					break;
				else
					continue;

			case DFCFLT_RELIGIONS:
				// Add all Temple types
				if ( nType == DFCLOC_RELIGION_TEMPLES || nType == DFCLOC_RELIGION_CULTS || nType == DFCLOC_RELIGION_COVENS )
					break;
				else
					continue;

			case DFCFLT_TOWNS:
				// Add all Town types
				if ( nType == DFCLOC_TOWN_CITIES || nType == DFCLOC_TOWN_HAMLETS || nType == DFCLOC_TOWN_VILLAGES )
					break;
				else
					continue;

			default:
				// Specific filter, only add if type matches filter
				if ( nType != m_lCurFilter )
					continue;
				break;
			}

			// Add this item
			switch ( nType )
			{
			case DFCLOC_DUNGEON_LABYRINTHS:			// Dungeons
			case DFCLOC_DUNGEON_KEEPS:
			case DFCLOC_DUNGEON_RUINS:
				nIcon = DFCIMG_DUNGEON;
				break;

			case DFCLOC_GRAVEYARD_COMMON:			// Graveyards
			case DFCLOC_GRAVEYARD_FORGOTTEN:
				nIcon = DFCIMG_GRAVEYARD;
				break;

			case DFCLOC_HOME_FARMS:					// Homes
			case DFCLOC_HOME_WEALTHY:
			case DFCLOC_HOME_POOR:
			case DFCLOC_HOME_YOURSHIPS:
				nIcon = DFCIMG_HOME;
				break;

			case DFCLOC_TAVERN:
				nIcon = DFCIMG_TAVERN;				// Taverns
				break;

			case DFCLOC_RELIGION_TEMPLES:			// Temples
			case DFCLOC_RELIGION_CULTS:
			case DFCLOC_RELIGION_COVENS:
				nIcon = DFCIMG_TEMPLE;
				break;

			case DFCLOC_TOWN_CITIES:				// Towns
			case DFCLOC_TOWN_HAMLETS:
			case DFCLOC_TOWN_VILLAGES:
				nIcon = DFCIMG_TOWN;
				break;

			default:
				nIcon = DFCIMG_UNKNOWN;				// Unknown
			}
			nItem = pList->InsertItem( 0, m_haLocation[i].strName, nIcon );
			pList->SetItemData( nItem, i );
		}
	}

	return TRUE;
}/* BuildLocationView */

typedef struct {
	CString strIndex;
	TStaticArray<unsigned char> oaChar;
} TEMP;

BOOL CDFCartographerApp::InitialiseViews()
{
	/*
	TStaticArray<TEMP> oaTemp;
	oaTemp.NewPool( 45 );
	for ( int i = 0; i < 45; i ++ ) {
		oaTemp[i].strIndex = FileIndices[i];
	}
	LPLOCATION_DESCRIPTION pDesc = NULL;
	for ( int r = 0; r < NUM_REGION_COUNT; r++ ) {
		m_DaggerTool.m_ArchMaps.OpenRegion( r );
		for ( int l = 0; l < m_DaggerTool.m_ArchMaps.GetLocationCount(); l++ ) {
			pDesc = m_DaggerTool.m_ArchMaps.GetLocationDescription( l );
			_ASSERT( pDesc->BlockWidth < 9 );
			_ASSERT( pDesc->BlockHeight < 9 );
			for ( int s = 0; s < 64; s++ ) {
				bool bFound = false;
				char nChar = pDesc->BlockFileChar[s];
				char nIndex = pDesc->BlockFileIndex[s];
				_ASSERT( nIndex != 28 );
				_ASSERT( nIndex != 9 );
				for ( int t = 0; t < oaTemp[nIndex].oaChar.GetCount(); t++ ) {
					if ( oaTemp[nIndex].oaChar[t] == nChar ) {
						bFound = true;
						break;
					}
				}
				if ( !bFound ) {
					UINT h = oaTemp[nIndex].oaChar.New();
					oaTemp[nIndex].oaChar[h] = nChar;
				}
			}// index
		}// location
	}// region

	CFile file;
	file.Open( "c:\\foo.txt", CFile::modeCreate|CFile::modeWrite|CFile::shareDenyNone|CFile::typeBinary );
	for ( i = 0; i < 45; i++ ) {
		file.Write( oaTemp[i].strIndex.GetBuffer(0), oaTemp[i].strIndex.GetLength() );
		file.Write( "\r\n", 2 );

		CString num = " ", tmp;
		int nCount = oaTemp[i].oaChar.GetCount();
		if ( !nCount ) {
			num = "no numbers";
		}
		else {
			for ( int q = 0; q < nCount; q++ ) {
				tmp.Format( "%u ", oaTemp[i].oaChar[q] );
				num += tmp;
			}
		}
		file.Write( num.GetBuffer(0), num.GetLength() );

		file.Write( "\r\n\r\n", 4 );
	}
	file.Close();

	return TRUE;
	*/

	// Arena bar must be available
	_ASSERT( m_pArenaBar );

	// Get pointer to controls
	CComboBoxEx *pRegionCombo = &m_pArenaBar->m_wndArenaDlg.m_ctrlRegionCombo;
	CComboBoxEx *pLocationCombo = &m_pArenaBar->m_wndArenaDlg.m_ctrlLocationCombo;

	// Reset region and location contents
	pRegionCombo->ResetContent();
	pLocationCombo->ResetContent();

	// Clear the list of locations
	m_pArenaBar->m_wndArenaDlg.m_ctrlArenaList.DeleteAllItems();

	// If Arena2 is not open, we're done
	if ( !m_DaggerTool.IsArena2Open() ) {
		return TRUE;
	}

	// Populate region combo box
	int nPos = 0;
	COMBOBOXEXITEM cbi;
	ZeroMemory( &cbi, sizeof(COMBOBOXEXITEM) );
	cbi.mask = CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_LPARAM|CBEIF_TEXT;
	for ( int i = 0; i < NUM_REGION_COUNT; i++ ) {
		switch ( i )
		{
		// Change icon for empty regions
		case 2:
		case 3:
		case 4:
		case 6:
		case 7:
		case 8:
		case 10:
		case 12:
		case 13:
		case 14:
		case 15:
		case 24:
		case 25:
		case 27:
		case 28:
		case 29:
		case 30:
			continue;	// Comment this out to display empty regions
			cbi.iImage = DFCIMG_UNKNOWN;
			cbi.iSelectedImage = DFCIMG_UNKNOWN;
			break;
		default:
			cbi.iImage = DFCIMG_LOCATION;
			cbi.iSelectedImage = DFCIMG_LOCATION;
			break;
		}
		cbi.iItem = nPos++;
		cbi.lParam = i;
		cbi.pszText = RegionNames[i];
		pRegionCombo->InsertItem( &cbi );
	}

	#define AddFilter( iIndent, iImage, lParam, pszText ) {InitFltCBI( &cbi, nPos++, iIndent, iImage, lParam, pszText ); pLocationCombo->InsertItem( &cbi );}

	// Populate location filter combo box
	nPos = 0;
	ZeroMemory( &cbi, sizeof(COMBOBOXEXITEM) );
	cbi.mask = CBEIF_IMAGE|CBEIF_SELECTEDIMAGE|CBEIF_LPARAM|CBEIF_TEXT|CBEIF_INDENT;
	AddFilter( 0, DFCIMG_ALL, DFCFLT_ALL, DFCFLTSTR_ALL );

	// Add Dungeon filters
	AddFilter( 1, DFCIMG_DUNGEON, DFCFLT_DUNGEONS, DFCFLTSTR_DUNGEONS );
	AddFilter( 2, DFCIMG_DUNGEON, DFCFLT_DUNGEON_LABYRINTHS, DFCFLTSTR_DUNGEON_LABYRINTHS );
	AddFilter( 2, DFCIMG_DUNGEON, DFCFLT_DUNGEON_KEEPS, DFCFLTSTR_DUNGEON_KEEPS );
	AddFilter( 2, DFCIMG_DUNGEON, DFCFLT_DUNGEON_RUINS, DFCFLTSTR_DUNGEON_RUINS );

	// Add Graveyard filters
	AddFilter( 1, DFCIMG_GRAVEYARD, DFCFLT_GRAVEYARDS, DFCFLTSTR_GRAVEYARDS );
	AddFilter( 2, DFCIMG_GRAVEYARD, DFCFLT_GRAVEYARD_COMMON, DFCFLTSTR_GRAVEYARD_COMMON );
	AddFilter( 2, DFCIMG_GRAVEYARD, DFCFLT_GRAVEYARD_FORGOTTEN, DFCFLTSTR_GRAVEYARD_FORGOTTEN );

	// Add Home filters
	AddFilter( 1, DFCIMG_HOME, DFCFLT_HOMES, DFCFLTSTR_HOMES );
	AddFilter( 2, DFCIMG_HOME, DFCFLT_HOME_FARMS, DFCFLTSTR_HOME_FARMS );
	AddFilter( 2, DFCIMG_HOME, DFCFLT_HOME_WEALTHY, DFCFLTSTR_HOME_WEALTHY );
	AddFilter( 2, DFCIMG_HOME, DFCFLT_HOME_POOR, DFCFLTSTR_HOME_POOR );
	//AddFilter( 2, DFCIMG_HOME, DFCFLT_HOME_YOURSHIPS, DFCFLTSTR_HOME_YOURSHIPS );

	// Add Tavern filters
	AddFilter( 1, DFCIMG_TAVERN, DFCFLT_TAVERNS, DFCFLTSTR_TAVERNS );

	// Add Temple filters
	AddFilter( 1, DFCIMG_TEMPLE, DFCFLT_RELIGIONS, DFCFLTSTR_RELIGIONS );
	AddFilter( 2, DFCIMG_TEMPLE, DFCFLT_RELIGION_TEMPLES, DFCFLTSTR_RELIGION_TEMPLES );
	AddFilter( 2, DFCIMG_TEMPLE, DFCFLT_RELIGION_CULTS, DFCFLTSTR_RELIGION_CULTS );
	AddFilter( 2, DFCIMG_TEMPLE, DFCFLT_RELIGION_COVENS, DFCFLTSTR_RELIGION_COVENS );

	// Add Town filters
	AddFilter( 1, DFCIMG_TOWN, DFCFLT_TOWNS, DFCFLTSTR_TOWNS );
	AddFilter( 2, DFCIMG_TOWN, DFCFLT_TOWN_CITIES, DFCFLTSTR_TOWN_CITIES );
	AddFilter( 2, DFCIMG_TOWN, DFCFLT_TOWN_HAMLETS, DFCFLTSTR_TOWN_HAMLETS );
	AddFilter( 2, DFCIMG_TOWN, DFCFLT_TOWN_VILLAGES, DFCFLTSTR_TOWN_VILLAGES );

	// TODO: Add search filter

	// Set starting region
	int nIndex = pRegionCombo->FindStringExact( 0, DFCSTR_STARTREGION );
	ZeroMemory( &cbi, sizeof(COMBOBOXEXITEM) );
	cbi.mask = CBEIF_LPARAM;
	cbi.iItem = nIndex;
	pRegionCombo->GetItem( &cbi );
	pRegionCombo->SetCurSel( nIndex );

	// Set starting location filter
	pLocationCombo->SetCurSel( m_lCurFilter );

	// Populate the location list
	SetRegion( cbi.lParam );

	return TRUE;
}/* InitialiseViews */


BOOL CDFCartographerApp::BuildMiniMap()
{
	// Get block layout of the current location
	LOCATION_BLOCKLAYOUT bl;
	if ( !m_DaggerTool.m_ArchMaps.GetLocationBlockLayout( m_lCurLocation, &m_DaggerTool.m_ArchBlocks, &bl ) )
		return FALSE;

	// Prepare automap
	RECT rct = {0,0,64,64};
	UINT nTexture = ACNULL;
	UINT nSprite = ACNULL;
	BLKIMG_AUTOMAP imgAutomap;

	// Prepare sprite
	ACSETTINGS_SPRITE acss;
	ZeroMemory( &acss, sizeof(ACSETTINGS_SPRITE) );
	acss.dwSize = sizeof(ACSETTINGS_SPRITE);
	acss.dwFlags = 0;
	acss.vPos.x = 0.0f;
	acss.vPos.y = 0.0f;
	acss.vScaling.x = acss.vScaling.y = 0.5f;
	acss.colour = 0xffffffff;

	// Layout location as sprites
	int nBlock = 0;
	for ( int y = 0; y < bl.nHeight; y++ ) {
		for ( int x = 0; x < bl.nWidth; x++ ) {
			// Compute offset into tile array
			nBlock = bl.Tiles[y*bl.nWidth+x];
			if ( -1 == nBlock )
				continue;

			// Open block
			if ( !m_DaggerTool.m_ArchBlocks.OpenBlock( nBlock ) ) {
				return FALSE;
			}

			// Install minimap texture
			if ( !m_DaggerTool.m_ArchBlocks.GetBlockAutomap( &imgAutomap ) ) {
				return FALSE;
			}
			nTexture = m_Alchemy.InstallTextureFromMemory( ACNULL, &rct, 64, imgAutomap.pBuffer, ACBD_32 );
			if ( ACNULL == nTexture ) {
				return FALSE;
			}

			// Set sprite texture and position
			acss.nTexture = nTexture;
			acss.vPos.x = (float)16 + x * 32;
			acss.vPos.y = (float)16 + y * 32;

			// Install a sprite for minimap
			nSprite = m_Alchemy.InstallSprite( &acss );
			if ( ACNULL == nSprite ) {
				return FALSE;
			}

			// Decrement automap texture reference count so it dies with the sprite
			m_Alchemy.DecResRef( nTexture );
		}
	}

	return TRUE;
}/* BuildMiniMap */


BOOL CDFCartographerApp::Build3DView()
{
	// Get block layout of the current location
	LOCATION_BLOCKLAYOUT bl;
	if ( !m_DaggerTool.m_ArchMaps.GetLocationBlockLayout( m_lCurLocation, &m_DaggerTool.m_ArchBlocks, &bl ) )
		return FALSE;

	// Layout location using blocks of actors
	int nBlock = 0;
	for ( int y = 0; y < bl.nHeight; y++ ) {
		for ( int x = 0; x < bl.nWidth; x++ ) {
			// Compute offset into tile array
			nBlock = bl.Tiles[(y*bl.nWidth)+(bl.nWidth-1-x)];
			if ( -1 == nBlock )
				continue;

			// Add block to the scene
			BOOL bResult = BuildBlockActors( nBlock, (4096.0f * x), (4096.0f * y) );
			_ASSERT( bResult );
		}
	}

	return TRUE;
}/* Build3DView */


void CDFCartographerApp::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	char buffer[256];
	m_Alchemy.GetKeyboardState( buffer );

	float fAmount = 200.0f;
	switch ( nChar )
	{
	case VK_UP:
		m_Alchemy.CameraThrust( ACNULL, 0.0f, 0.0f, fAmount, true );
		break;

	case VK_DOWN:
		m_Alchemy.CameraThrust( ACNULL, 0.0f, 0.0f, -fAmount, true );
		break;

	case VK_LEFT:
		m_Alchemy.CameraThrust( ACNULL, -fAmount, 0.0f, 0.0f );
		break;

	case VK_RIGHT:
		m_Alchemy.CameraThrust( ACNULL, fAmount, 0.0f, 0.0f );
		break;
	}

	if ( KEYDOWN(buffer, DIK_A) ) m_Alchemy.CameraRotate( ACNULL, 0.0f, -0.05f, 0.0f );
	if ( KEYDOWN(buffer, DIK_D) ) m_Alchemy.CameraRotate( ACNULL, 0.0f, 0.05f, 0.0f );

	if ( KEYDOWN(buffer, DIK_Q) ) m_Alchemy.CameraThrust( ACNULL, 0.0f, -fAmount/4, 0.0f );
	if ( KEYDOWN(buffer, DIK_E) ) m_Alchemy.CameraThrust( ACNULL, 0.0f, fAmount/4, 0.0f );

	if ( KEYDOWN(buffer, DIK_PGUP) ) m_Alchemy.CameraRotate( ACNULL, -0.05f, 0.0f, 0.0f );
	if ( KEYDOWN(buffer, DIK_PGDN) ) m_Alchemy.CameraRotate( ACNULL, 0.05f, 0.0f, 0.0f );

	m_Alchemy.DoWork( ACDOWORK_CLEARDRAW|ACDOWORK_PRESENT );
}


BOOL CDFCartographerApp::BuildMediaView()
{
	if ( -1 == m_lCurLocation )
		return FALSE;

	// Build the minimap
	if ( !BuildMiniMap() )
		return FALSE;

	// Build the 3D view
	if ( !Build3DView() )
		return FALSE;

	ACSETTINGS_CAMERA acsc;
	ZeroMemory( &acsc, sizeof(ACSETTINGS_CAMERA) );
	acsc.Object.dwSize = sizeof(ACSETTINGS_CAMERA);
	acsc.Object.vPos = D3DXVECTOR3( -2000.0f, 3000.0f, -2000.0f );
	m_Alchemy.SetCamera( ACNULL, &acsc );
	//m_Alchemy.CameraRotate( ACNULL, 10.0f, 10.0f, 10.0f );

	m_Alchemy.DoWork( /*ACDOWORK_CLEARDRAW|*/ACDOWORK_PRESENT );

	m_Alchemy.CameraRotate( ACNULL, 0.7f, 0.0f, 0.0f );

	m_Alchemy.DoWork( ACDOWORK_CLEARDRAW|ACDOWORK_PRESENT );

	return TRUE;
}/* BuildMediaView */


void CDFCartographerApp::InitFltCBI( COMBOBOXEXITEM *pcbi, int iItem, int iIndent, int iImage, LPARAM lParam, LPCSTR pszText )
{
	pcbi->iItem = iItem;
	pcbi->iIndent = iIndent;
	pcbi->iImage = iImage;
	pcbi->iSelectedImage = iImage;
	pcbi->lParam = lParam;
	pcbi->pszText = (char*)pszText;
}/* InitFltCBI */


void CDFCartographerApp::OnRegionSelected()
{
	CComboBoxEx *pRegionCombo = &m_pArenaBar->m_wndArenaDlg.m_ctrlRegionCombo;
	COMBOBOXEXITEM cbi;
	ZeroMemory( &cbi, sizeof(COMBOBOXEXITEM) );
	cbi.mask = CBEIF_LPARAM;
	cbi.iItem = pRegionCombo->GetCurSel();
	pRegionCombo->GetItem( &cbi );
	SetRegion( cbi.lParam );
}/* OnRegionSelected */


void CDFCartographerApp::OnFilterSelected()
{
	// Get current filter type
	CComboBoxEx *pLocationCombo = &m_pArenaBar->m_wndArenaDlg.m_ctrlLocationCombo;
	COMBOBOXEXITEM cbi;
	ZeroMemory( &cbi, sizeof(COMBOBOXEXITEM) );
	cbi.mask = CBEIF_LPARAM;
	cbi.iItem = pLocationCombo->GetCurSel();
	pLocationCombo->GetItem( &cbi );
	m_lCurFilter = cbi.lParam;
	BuildLocationView();
}/* OnRegionSelected */


void CDFCartographerApp::OnLocationSelected( long lLocation )
{
	m_Alchemy.PurgeSpriteStore();
	m_Alchemy.PurgeObjectStore();
	m_lCurLocation = lLocation;
	BuildMediaView();
}/* OnLocationSelected */


//
// BOOL	CacheDFTextureCombine( LPCSTR pszName, long nArchive, long nRecord, LPRECT pRectOut, TString& strCacheNameOut )
// Combines unique textures that will be installed into Alchemy
// Return: TRUE if successful, otherwise FALSE
//
inline BOOL CDFCartographerApp::CacheDFTextureCombine( LPCSTR pszName, long nArchive, long nRecord, LPRECT pRectOut, TString& strCacheNameOut )
{
	// Validate
	_ASSERT( pRectOut );

	// Check this texture has not already been cached
	LPDFTEXTURECACHE pCache = NULL;
	LPDFTEXTUREREF pIndex = m_haDFTextureCacheIndexCombine.GetObject( pszName );
	if ( pIndex ) {
		// Return cache RECT and name only
		pCache = m_oaDFTextureCache.GetObject( pIndex->nTexture );
		*pRectOut = pCache->oaSubset[pIndex->nSubset];
		strCacheNameOut = pCache->strName;
		return TRUE;
	}

	// Query current cache texture
	CString strCacheName;
	UINT nTexture = m_nLastDFTextureCache;
	if ( !nTexture ) {
		// Create first cache texture
		nTexture = m_oaDFTextureCache.NewTail();
		if ( !nTexture ) {
			return FALSE;
		}
		m_nLastDFTextureCache = nTexture;

		// Generate name for this texture cache
		pCache = m_oaDFTextureCache.GetObject( nTexture );
		strCacheName.Format( "DE%d", m_Alchemy.GetResourceCount()/*m_oaDFTextureCache.GetCount()*/ );
		pCache->strName = strCacheName;
	}
	else {
		strCacheName = m_oaDFTextureCache.GetObject( m_nLastDFTextureCache )->strName;
	}

	// Get current cache texture
	pCache = m_oaDFTextureCache.GetObject( m_nLastDFTextureCache );

	// Open archive
	CString strFile;
	strFile.Format( "TEXTURE.%.3d", nArchive );
	if ( !m_DaggerTool.OpenTextureArchive( strFile ) ) {
		return FALSE;
	}

	// Get image dimensions
	int	cx, cy;
	RECT rctImage;
	m_DaggerTool.m_dtxArchive.GetImageRect( nRecord, 0, &rctImage );
	cx = rctImage.right - rctImage.left;
	cy = rctImage.bottom - rctImage.top;

	// This assertion is tripped if the texture is too large
	// No Daggerfall texture is beyond these dimensions
	_ASSERT( cx < 256 );
	_ASSERT( cy < 256 );

	// Determine if a new column must be started
	if ( pCache->cy + cy > DE_CACHETEXTURE_HEIGHT ) {
		// Start new column
		pCache->cy = 0;
		pCache->cx += pCache->maxWidth;
		pCache->maxWidth = 0;
	}

	// Determine if this texture will fit into the current cache object
	if ( pCache->cx + cx > DE_CACHETEXTURE_WIDTH ) {
		// Start a new cache object
		nTexture = m_oaDFTextureCache.NewTail();
		if ( !nTexture ) {
			return FALSE;
		}
		m_nLastDFTextureCache = nTexture;

		// Generate name for this texture cache
		strCacheName.Format( "DE%d", m_oaDFTextureCache.GetCount() );

		// Get current cache object
		pCache = m_oaDFTextureCache.GetObject( nTexture );
		if ( !pCache ) {
			return FALSE;
		}
		pCache->strName = strCacheName;
	}

	// Store image rectangle
	UINT nSubset = pCache->oaSubset.NewTail();
	if ( !nSubset ) {
		return FALSE;
	}
	LPRECT pRect = pCache->oaSubset.GetObject( nSubset );
	pRect->left = pCache->cx;
	pRect->top = pCache->cy;
	pRect->right = pCache->cx + cx;
	pRect->bottom = pCache->cy + cy;

	// Store image bits
	DTX_IMAGEDESC img;
	char *pBits = pCache->pBits + (pCache->cy * DE_CACHETEXTURE_PITCH) + (pCache->cx * 3);
	if ( !m_DaggerTool.m_dtxArchive.GetImage( nRecord, 0, &img, pBits, DE_CACHETEXTURE_PITCH ) ) {
		return FALSE;
	}

	// Update position in cache
	if ( cx > pCache->maxWidth ) {
		pCache->maxWidth = cx;
	}
	pCache->cy += cy;

	// Hash name to index and set details
	pIndex = m_haDFTextureCacheIndexCombine.New( pszName );
	if ( !pIndex ) {
		return FALSE;
	}
	pIndex->nTexture = nTexture;
	pIndex->nSubset = nSubset;

	// Store names and dimensions
	pCache->rctTexture.left = 0;
	pCache->rctTexture.top = 0;
	pCache->rctTexture.right = DE_CACHETEXTURE_WIDTH;
	pCache->rctTexture.bottom = DE_CACHETEXTURE_HEIGHT;
	strCacheNameOut = pCache->strName;

	// Set output RECT
	*pRectOut = *pRect;

	return TRUE;
}/* CacheDFTextureCombine */


//
// BOOL	CacheDFTextureVerbose( LPCSTR pszName, long nArchive, long nRecord, LPRECT pRectOut )
// Build a list of unique textures that will be added to the store
// Return: TRUE if successful, otherwise FALSE
//
BOOL CDFCartographerApp::CacheDFTextureVerbose( LPCSTR pszName, long nArchive, long nRecord, LPRECT pRectOut )
{
	// Check this texture has not already been cached
	LPDFTEXTURECACHE pCache = NULL;
	LPDFTEXTUREREF pIndex = m_haDFTextureCacheIndexVerbose.GetObject( pszName );
	if ( pIndex ) {
		// Return cache RECT and name only
		pCache = m_oaDFTextureCache.GetObject( pIndex->nTexture );
		pRectOut->left = pRectOut->top = 0;
		pRectOut->right = pCache->cx;
		pRectOut->bottom = pCache->cy;
		return TRUE;
	}

	// Create new cache texture
	UINT nTexture = m_oaDFTextureCache.NewTail();
	if ( !nTexture ) {
		return FALSE;
	}

	// Hash name to index and set details
	pIndex = m_haDFTextureCacheIndexVerbose.New( pszName );
	if ( !pIndex ) {
		m_oaDFTextureCache.Delete( nTexture );
		return FALSE;
	}
	pIndex->nTexture = nTexture;
	pIndex->nSubset = ACNULL;

	// Open archive
	CString strFile;
	strFile.Format( "TEXTURE.%.3d", nArchive );
	m_DaggerTool.OpenTextureArchive( strFile );

	// Get image rect
	RECT rctImage;
	m_DaggerTool.m_dtxArchive.GetImageRect( nRecord, 0, &rctImage );

	// Store image bits
	DTX_IMAGEDESC img;
	pCache = m_oaDFTextureCache.GetObject( nTexture );
	char *pBits = pCache->pBits + (pCache->cy * DE_CACHETEXTURE_PITCH) + (pCache->cx * 3);
	if ( !m_DaggerTool.m_dtxArchive.GetImage( nRecord, 0, &img, pBits, DE_CACHETEXTURE_PITCH ) ) {
		m_oaDFTextureCache.Delete( nTexture );
		m_haDFTextureCacheIndexVerbose.Delete( pszName );
		return FALSE;
	}

	// Set properties
	pCache->strName = pszName;
	pCache->cx = rctImage.right;
	pCache->cy = rctImage.bottom;

	// Set outgoing properties
	pRectOut->left = pRectOut->top = 0;
	pRectOut->right = pCache->cx;
	pRectOut->bottom = pCache->cy;
	pCache->rctTexture = *pRectOut;

	return TRUE;
}/* CacheDFTextureVerbose */


//
// BOOL CommitDFTextures()
// Commit all cached textures to the Alchemy store
// Return: TRUE if successful, otherwise FALSE
//
BOOL CDFCartographerApp::CommitDFTextures()
{
	// Process all items in list
	UINT nRef = m_oaDFTextureCache.GetRoot();
	while ( nRef ) {
		// Attempt to find texture first, or refs will go crazy
		LPDFTEXTURECACHE pCache = m_oaDFTextureCache.GetObject( nRef );
		if ( ACNULL == m_Alchemy.FindResource( pCache->strName ) ) {
			// Install texture
			UINT nTexture = m_Alchemy.InstallTextureFromMemory( ACNULL, &pCache->rctTexture, DE_CACHETEXTURE_WIDTH, pCache->pBits, ACBD_32, pCache->strName );
			if ( ACNULL == nTexture ) {
				ReleaseDFTextures();
				return FALSE;
			}
		}

		// Get next item in list
		nRef = m_oaDFTextureCache.GetNext( nRef );
	}

	return TRUE;
}/* CommitDFTextures */


//
// BOOL AbortDFTextures()
// Delete all cached textures
// Return: TRUE if successful, otherwise FALSE
//
BOOL CDFCartographerApp::AbortDFTextures()
{
	m_oaDFTextureCache.Destroy();
	m_haDFTextureCacheIndexCombine.Destroy();
	m_haDFTextureCacheIndexVerbose.Destroy();
	m_nLastDFTextureCache = NULL;

	return TRUE;
}/* CommitDFTextures */


//
// void ReleaseDFTextures()
// Dereference textures from the Alchemy store and clear list
// This puts the texture in the hands of Alchemy
//
void CDFCartographerApp::ReleaseDFTextures()
{
	// Process all items in list
	UINT nRef = m_oaDFTextureCache.GetRoot();
	while ( nRef ) {
		// Get reference to texture
		UINT nTexture = m_Alchemy.FindResource( m_oaDFTextureCache[nRef].strName );

		// Remove texture if present
		if ( ACNULL != nTexture ) {
			m_Alchemy.DecResRef( nTexture );
		}

		// Get next item in list
		nRef = m_oaDFTextureCache.GetNext( nRef );
	}

	m_oaDFTextureCache.Destroy();
	m_haDFTextureCacheIndexCombine.Destroy();
	m_haDFTextureCacheIndexVerbose.Destroy();
	m_nLastDFTextureCache = NULL;
}/* ReleaseDFTextures */


//
// BOOL InitialiseMeshObject( TFlexibleMesh* pMesh, DWORD dwVertCount = 0, DWORD dwFaceCount = 0 )
// Correctly initialise a mesh object based on viewing properties
// Return: TRUE if successful, otherwise FALSE
//
inline BOOL CDFCartographerApp::InitialiseMeshObject( TFlexibleMesh* pMesh, DWORD dwVertCount/*=0*/, DWORD dwFaceCount/*=0*/ )
{
	_ASSERT( pMesh );

	// Set mesh flags
	DWORD dwFlags = 0;
	DWORD dwPrimType = D3DPT_TRIANGLELIST;
	switch ( m_dwDrawMode )
	{
	case OBJDRAW_PUREFACES:
		dwPrimType = D3DPT_LINELIST;
		break;

	case OBJDRAW_WIREFRAME:
		dwFlags = AFMFFLAG_DRAWWIREFRAME;
		break;

	case OBJDRAW_SOLID:
	case OBJDRAW_TEXTURED:
		break;

	default:
		return FALSE;
	}

	// Create new mesh object
	BOOL bResult = FALSE;
	if ( OBJDRAW_TEXTURED == m_dwDrawMode ) {
		bResult = pMesh->Create( dwVertCount, dwFaceCount, "store://", dwFlags, dwPrimType );
	}
	else {
		bResult = pMesh->Create( dwVertCount, dwFaceCount, NULL, dwFlags, dwPrimType );
	}

	return bResult;
}/* InitialiseMeshObject */


//
// BuildMesh( long nObject, TFlexibleMesh* pMeshOut )
// Construct a mesh object
// Return: TRUE if successful, otherwise FALSE
//
BOOL CDFCartographerApp::BuildMesh( long nObject, TFlexibleMesh* pMeshOut )
{
	_ASSERT( pMeshOut );

	// Open Arch3D object
	BSAArch3D* pArch3D = &m_DaggerTool.m_Arch3D;
	if ( !pArch3D->OpenObject( nObject ) )
		return FALSE;

	// Add face information to mesh
	int nFace = 0;
	DWORD dwTotalVertCount = 0;
	LPARCH3D_FACE pFace = NULL;
	if ( OBJDRAW_PUREFACES == m_dwDrawMode )
	{
		// Iterate once through faces to get total vertex count
		int nPureFaces = pArch3D->GetFaceCount();
		for ( nFace = 0; nFace < nPureFaces; nFace++ ) {
			pFace = pArch3D->GetFace( nFace );
			if ( !pFace ) {
				return FALSE;
			}
			dwTotalVertCount += pArch3D->GetPointCount( pFace );
		}

		// Buffer must be double size to correctly support line segments
		dwTotalVertCount *= 2;

		// Create temporary vertex buffer
		LPOBJVERTEX pVertBuffer = new OBJVERTEX[dwTotalVertCount];
		if ( !pVertBuffer )
			return FALSE;
		
		// Create new mesh object
		InitialiseMeshObject( pMeshOut, dwTotalVertCount );

		// Load pure face vertices into temporary buffer as line segments
		DWORD dwCurVertex = 0;
		int nCount = 0;
		for ( nFace = 0; nFace < nPureFaces; nFace++ ) {
			pFace = pArch3D->GetFace( nFace );
			if ( !pArch3D->ExportPureFace( pFace, pVertBuffer + dwCurVertex, &nCount ) ) {
				delete[] pVertBuffer;
				return FALSE;
			}
			
			dwCurVertex += nCount;
		}
		
		// Transfer temporary vertices to mesh object
		AFMF_VERTEXDESC avd;
		for ( DWORD dwVertex = 0; dwVertex < dwTotalVertCount; dwVertex++ ) {
			avd.x = pVertBuffer[dwVertex].pos.x;
			avd.y = pVertBuffer[dwVertex].pos.y;
			avd.z = pVertBuffer[dwVertex].pos.z;
			avd.nx = pVertBuffer[dwVertex].normal.x;
			avd.ny = pVertBuffer[dwVertex].normal.y;
			avd.nz = pVertBuffer[dwVertex].normal.z;
			avd.tu = pVertBuffer[dwVertex].tu;
			avd.tv = pVertBuffer[dwVertex].tv;
			pMeshOut->SetVertex( dwVertex, &avd );
		}

		// Free temporary vertex buffer
		delete[] pVertBuffer;
	}
	else {
		// Create a list of triangulated faces and cache textures
		UINT fl;
		CString str;
		int nCount, nPureFaces = pArch3D->GetFaceCount();
		long nArchive, nRecord;
		TLinkedList<WIREFACE> oaFace;
		for ( nFace = 0; nFace < nPureFaces; nFace++ ) {
			// Extend list by one face
			if ( NULL == (fl = oaFace.NewTail() ) ) {
				return FALSE;
			}

			// Get this face
			pFace = pArch3D->GetFace( nFace );
			if ( !pFace ) {
				return FALSE;
			}

			// Choose face export method based on texture mode
			if ( OBJDRAW_TEXTURED == m_dwDrawMode ) {				
				// Get texture assigned to this face
				m_DaggerTool.m_Arch3D.GetFaceTexture( pFace, &nArchive, &nRecord );

				// Adjust texture for weather and region
				ApplyRegion( &nArchive );

				// TEXTURE.036 records do not match TEXTURE.035
				// Work around by setting back to 035
				if ( nArchive == 36 ) {
					nArchive = 35;
				}

				// TEXTURE.409 does not have a matching winter record 4 in TEXTURE.410
				// Work around by setting back to 409.4
				if ( nArchive == 410 && nRecord == 4 ) {
					nArchive = 409;
				}

				// TEXTURE.435 does not have a matching winter archive at all
				// ie. There is no TEXTURE.436, it's missing
				// Work around by setting back to 435
				if ( nArchive == 436 ) {
					nArchive = 435;
				}

				// Compose texture name
				str.Format( "%d.%d", nArchive, nRecord );

				// Cache texture
				RECT rct;
				if ( !CacheDFTextureVerbose( str, nArchive, nRecord, &rct ) ) {
					return FALSE;
				}

				// Store cache texture name in face list
				oaFace[fl].strTexture = str;

				// Export triangulated face
				SIZE sz = {rct.right, rct.bottom};
				if ( !pArch3D->ExportTriangulatedFace( pFace, oaFace[fl].verts, &nCount, &sz, &rct ) ) {
					return FALSE;
				}
			}
			else {
				// Export triangulated face
				if ( !pArch3D->ExportTriangulatedFace( pFace, oaFace[fl].verts, &nCount ) ) {
					return FALSE;
				}
			}// end if ( OBJDRAW_TEXTURED == m_dwDrawMode )

			// Update vertex count
			oaFace[fl].dwVertCount = nCount;
			dwTotalVertCount += nCount;
		}// end for ( nFace = 0; nFace < nPureFaces; nFace++ )

		// Set mesh vertex and face counts
		InitialiseMeshObject( pMeshOut, dwTotalVertCount, (dwTotalVertCount/3) );

		// Transfer faces to mesh object
		nFace = 0;
		LPOBJVERTEX pv;
		AFMF_VERTEXDESC avd;
		AFMF_FACEDESC afd;
		unsigned short nCurVertex = 0, nBaseVertex = 0;
		fl = oaFace.GetRoot();
		while ( fl ) {
			pv = oaFace[fl].verts;
			while ( nCurVertex < oaFace[fl].dwVertCount ) {
				// Compose vertex
				for ( int nVertex = 0; nVertex < 3; nVertex++ ) {
					avd.x = pv[nVertex].pos.x;
					avd.y = pv[nVertex].pos.y;
					avd.z = pv[nVertex].pos.z;
					avd.nx = pv[nVertex].normal.x;
					avd.ny = pv[nVertex].normal.y;
					avd.nz = pv[nVertex].normal.z;
					avd.tu = pv[nVertex].tu;
					avd.tv = pv[nVertex].tv;
					pMeshOut->SetVertex( nBaseVertex + nVertex, &avd );
				}// end for ( int nVertex = 0; nVertex < 3; nVertex++ )
				
				// Compose face
				afd.v0 = nBaseVertex;
				afd.v1 = nBaseVertex + 1;
				afd.v2 = nBaseVertex + 2;
				nCurVertex += 3;
				nBaseVertex += 3;
				pv += 3;

				// Store face in mesh
				if ( OBJDRAW_TEXTURED == m_dwDrawMode )
					pMeshOut->SetFace( nFace++, &afd, oaFace[fl].strTexture );
				else
					pMeshOut->SetFace( nFace++, &afd );
			}// end while ( nCurVertex < oaFace[fl].dwVertCount )

			nCurVertex = 0;
			fl = oaFace.GetNext( fl );
		}// end while ( fl )
	}// end if ( OBJDRAW_PUREFACES == m_dwDrawMode )

	return TRUE;
}/* BuildMesh */


//
// BOOL BuildBlockGroundMesh( LPRMBFLD_SMALLMAPDATA pMap, TFlexibleMesh* pMeshOut )
// Builds a mesh for the land under blocks
// Return: TRUE if successful, otherwise FALSE
// Note: The top-left corner of the mesh is at 0,0,0 - the scale is 1:1 to DF
//
BOOL CDFCartographerApp::BuildBlockGroundMesh( LPRMBFLD_SMALLMAPDATA pMap, TFlexibleMesh* pMeshOut )
{
	_ASSERT( pMap );
	_ASSERT( pMeshOut );

	// Construct mesh
	int x, y, v;
	DWORD dwVertex = 0;
	float fSide = 256.0f;
	if ( OBJDRAW_PUREFACES == m_dwDrawMode ) {
		// Describe pure face ground segment
		float pfSegment[] = {
			0.0f,	0.0f,
			0.0f,	fSide,
			0.0f,	fSide,
			fSide,	fSide,
			fSide,	fSide,
			fSide,	0.0f,
			fSide,	0.0f,
			0.0f,	0.0f,
		};

		// Construct vertices for the ground mesh
		AFMF_VERTEXDESC avd;
		ZeroMemory( &avd, sizeof(AFMF_VERTEXDESC) );
		InitialiseMeshObject( pMeshOut, 2048 );
		for ( y = 0; y < 16; y++ ) {
			for ( x = 0; x < 16; x++ ) {
				for ( v = 0; v < 16; v += 4 ) {
					avd.x = fSide * x + pfSegment[v];
					avd.z = fSide * y + pfSegment[v+1];
					pMeshOut->SetVertex( dwVertex++, &avd );
					avd.x = fSide * x + pfSegment[v+2];
					avd.z = fSide * y + pfSegment[v+3];
					pMeshOut->SetVertex( dwVertex++, &avd );
				}// end for ( v = 0; v < 16; v += 4 )
			}// end for ( x = 0; x < 16; x++ )
		}// end for ( y = 0; y < 16; y++ )
	}
	else {
		// Construct vertices, faces, and textures for the ground mesh
		CString str;
		UINT nFace = 0;
		AFMF_VERTEXDESC avd[4];
		AFMF_FACEDESC afd;
		ZeroMemory( &avd, sizeof(AFMF_VERTEXDESC) * 4 );
		ZeroMemory( &afd, sizeof(AFMF_FACEDESC) );
		InitialiseMeshObject( pMeshOut, 1024, 512 );
		for ( y = 0; y < 16; y++ ) {
			for ( x = 0; x < 16; x++ ) {
				// Add vertex 0
				avd[0].x = fSide * x;
				avd[0].z = fSide * y;
				avd[0].tu = 0.0f;
				avd[0].tv = 0.0f;

				// Add vertex 1
				avd[1].x = fSide * x;
				avd[1].z = fSide * (y+1);
				avd[1].tu = 0.0f;
				avd[1].tv = 0.0f;

				// Add vertex 2
				avd[2].x = fSide * (x+1);
				avd[2].z = fSide * y;
				avd[2].tu = 0.0f;
				avd[2].tv = 0.0f;

				// Add vertex 3
				avd[3].x = fSide * (x+1);
				avd[3].z = fSide * (y+1);
				avd[3].tu = 0.0f;
				avd[3].tv = 0.0f;

				// Cache and align texture
				TString strCacheName = str;
				if ( OBJDRAW_TEXTURED == m_dwDrawMode ) {
					long nArchive = 302;	// Temperate is the most generic texture set, so say I

					// Get record details
					long nRecord = pMap->TextureInfo[y][15-x] & 0x3f;
					bool bRotate = ((pMap->TextureInfo[y][15-x] & 0x40) == 0x40) ? true : false;
					bool bFlip = ((pMap->TextureInfo[y][15-x] & 0x80) == 0x80) ? true : false;

					// Compose texture name
					str.Format( "%d.%d", nArchive, nRecord );

					// Cache texture
					//_ASSERT( nRecord < 56 );
					RECT subRect = {0,0,0,0};
					if ( nRecord > 55 ) {
						// Some records are above the 0-55 range?
						// For now just set them to "water"
						nRecord = 1;
					}
					if ( !CacheDFTextureCombine( str, nArchive, nRecord, &subRect, strCacheName ) ) {
						return FALSE;
					}

					// Align texture UV based on properties
					float fm = 0.01f;
					float ft = 0.99f;
					if ( !bFlip && !bRotate ) {
						// Standard
						avd[2].tu = fm;
						avd[2].tv = fm;
						avd[3].tu = fm;
						avd[3].tv = ft;
						avd[0].tu = ft;
						avd[0].tv = fm;
						avd[1].tu = ft;
						avd[1].tv = ft;
					}
					else if ( bFlip && !bRotate ) {
						// Flip
						avd[1].tu = fm;
						avd[1].tv = fm;
						avd[0].tu = fm;
						avd[0].tv = ft;
						avd[3].tu = ft;
						avd[3].tv = fm;
						avd[2].tu = ft;
						avd[2].tv = ft;
					}
					else if ( !bFlip && bRotate ) {
						// Rotate
						avd[3].tu = fm;
						avd[3].tv = fm;
						avd[1].tu = fm;
						avd[1].tv = ft;
						avd[2].tu = ft;
						avd[2].tv = fm;
						avd[0].tu = ft;
						avd[0].tv = ft;
					}
					else if ( bFlip && bRotate ){
						// Flip and Rotate
						avd[0].tu = fm;
						avd[0].tv = fm;
						avd[2].tu = fm;
						avd[2].tv = ft;
						avd[1].tu = ft;
						avd[1].tv = fm;
						avd[3].tu = ft;
						avd[3].tv = ft;
					}
					else {
						// Unknown
						avd[0].tu = fm;
						avd[0].tv = fm;
						avd[1].tu = fm;
						avd[1].tv = fm;
						avd[2].tu = fm;
						avd[2].tv = fm;
						avd[3].tu = fm;
						avd[3].tv = fm;
					}

					// Adjust UV coordinates to match correct subset of source texture
					float stu, stv;
					for ( v = 0; v < 4; v++ ) {
						stu = (float)subRect.left / DE_CACHETEXTURE_WIDTH;
						stv = (float)subRect.top / DE_CACHETEXTURE_HEIGHT;
						avd[v].tu = (avd[v].tu * 0.25f) + stu;
						avd[v].tv = (avd[v].tv * 0.25f) + stv;
					}
				}// end if ( OBJDRAW_TEXTURED == m_dwDrawMode )

				// Add vertices
				pMeshOut->SetVertex( dwVertex, &avd[0] );
				pMeshOut->SetVertex( dwVertex+1, &avd[1] );
				pMeshOut->SetVertex( dwVertex+2, &avd[2] );
				pMeshOut->SetVertex( dwVertex+3, &avd[3] );

				// Add triangle 0
				afd.v0 = (unsigned short)dwVertex;
				afd.v1 = (unsigned short)dwVertex+1;
				afd.v2 = (unsigned short)dwVertex+2;
				pMeshOut->SetFace( nFace++, &afd, strCacheName );

				// Add triangle 1
				afd.v0 = (unsigned short)dwVertex+2;
				afd.v1 = (unsigned short)dwVertex+1;
				afd.v2 = (unsigned short)dwVertex+3;
				pMeshOut->SetFace( nFace++, &afd, strCacheName );

				dwVertex += 4;
			}
		}
	}// end if ( OBJDRAW_PUREFACES == m_dwDrawMode )

	return TRUE;
}/* BuildBlockGroundMesh */


//
// BOOL BuildBlockActors( long nBlock, float x, float y )
// Install a group of actors to form a single block
// Return: TRUE if successful, otherwise FALSE
//
BOOL CDFCartographerApp::BuildBlockActors( long nBlock, float x, float y )
{
	CString str;
	UINT nMesh, nActor;
	TFlexibleMesh mesh;

	// Open this block
	if ( !m_DaggerTool.m_ArchBlocks.OpenBlock( nBlock ) )
		return FALSE;

	/*
	// Prepare actor
	ACSETTINGS_ACTOR acsa;
	ZeroMemory( &acsa, sizeof(ACSETTINGS_ACTOR) );
	acsa.Object.dwSize = sizeof(ACSETTINGS_ACTOR);
	acsa.Object.bFixed = TRUE;
	acsa.Object.Material.Ambient = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	acsa.Object.Material.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);

	// Reference start of information
	LPRMBFLD pFLD = m_DaggerTool.m_ArchBlocks.GetFLD();

	// Get block name
	char pszBlockName[32];
	m_DaggerTool.m_ArchBlocks.GetRecordDesc( nBlock, pszBlockName, 32 );

	// Compose ground mesh name
	str.Format( "%s.ground", pszBlockName );

	// Look for ground mesh in store
	nMesh = m_Alchemy.FindResource( str );

	// Build block ground mesh if not present in store
	if ( !nMesh ) {
		BuildBlockGroundMesh( &pFLD->SmallMaps, &mesh );
		mesh.Translate( -4096.0, -3.0f, 0.0f );
		CommitDFTextures();
		nMesh = m_Alchemy.InstallMeshFromMemory( &mesh, str );
		ReleaseDFTextures();
	}

	// Create an actor for the ground
	acsa.Object.vPos = D3DXVECTOR3(x,0.0f,y);
	acsa.Object.vPos = D3DXVECTOR3(x,0.0f,y);
	acsa.nMesh = nMesh;
	nActor = m_Alchemy.InstallObject( ACOBJECT_ACTOR, &acsa, FALSE );

	// Nothing more to do if this block has no subrecords
	long nBlocks = m_DaggerTool.m_ArchBlocks.GetBlockSubRecordCount();
	if ( !nBlocks )
		return TRUE;

	// Build all subrecords
	float fRotate;
	long nObjectRecord;
	LPRMB_BLOCKHEADER pBlockHeader;
	LPRMB_BLOCK3DOBJECTS p3DObjects;
	for ( long r = 0; r < nBlocks; r++ ) {
		// Get subrecord header
		pBlockHeader = m_DaggerTool.m_ArchBlocks.GetOutsideBlockSubRecord( r );

		// Get list of subobjects
		p3DObjects = m_DaggerTool.m_ArchBlocks.GetBlockSubRecord3DObjects( pBlockHeader );

		// Create an actor for every 3D object in this subrecord
		for ( int nObject = 0; nObject < pBlockHeader->n3DObjectRecords; nObject++ ) {
			// Get mesh record
			nObjectRecord = m_DaggerTool.m_Arch3D.ResolveObjectID( p3DObjects[nObject].nObjectID1 * 100 + p3DObjects[nObject].nObjectID2 );

			// Compose mesh name
			str.Format( "%u", nObjectRecord );

			// Look for mesh in store
			nMesh = m_Alchemy.FindResource( str );

			// Build the mesh if not present in store
			if ( !BuildMesh( nObjectRecord, &mesh ) )
				return FALSE;

			// Get Rotation2 in degrees (512 is 90 degrees of rotation, so 1024 is 180 degrees, etc.)
			fRotate = (float)(90.0f * p3DObjects[nObject].Rotation / 512.0f );
			MeshSub.RotateDegrees( -fRotate, 0.0f, 0.0f );

			// Adjust Position2 of this mesh
			MeshSub.Translate( (float)-(p3DObjects[nObject].XPos2), (float)-(p3DObjects[nObject].YPos2), (float)-(p3DObjects[nObject].ZPos2) );

			// Merge the submesh into the larger working mesh
			MeshTemp += MeshSub;

			// TODO: Adjust Rotation1

			// TODO: Adjust Position1
		}

		// Adjust Rotation1 of this working mesh
		//fRotate = (float)(90.0f * pFLD->BlockPos[r].BlockRotation / 512.0f);
		//MeshTemp.RotateDegrees( -fRotate, 0.0f, 0.0f );

		// Adjust Position1 of this mesh
		//MeshTemp.Translate( (float)-(pFLD->BlockPos[r].XPos2), 0.0f, (float)(pFLD->BlockPos[r].ZPos2) );

		// Merge working mesh into main mesh object
		//MeshView += MeshTemp;
	}

	return TRUE;
	*/

	// Initialise main mesh object
	TFlexibleMesh MeshView;
	InitialiseMeshObject( &MeshView );

	// Reference start of information
	LPRMBFLD pFLD = m_DaggerTool.m_ArchBlocks.GetFLD();

	// Build all subrecords
	float fRotate;
	long nObjectRecord;
	TFlexibleMesh MeshTemp, MeshSub;
	LPRMB_BLOCKHEADER pBlockHeader;
	LPRMB_BLOCK3DOBJECTS p3DObjects;
	long nBlocks = m_DaggerTool.m_ArchBlocks.GetBlockSubRecordCount();
	for ( long r = 0; r < nBlocks; r++ ) {
		// Get subrecord header
		pBlockHeader = m_DaggerTool.m_ArchBlocks.GetOutsideBlockSubRecord( r );

		// Get list of subobjects
		p3DObjects = m_DaggerTool.m_ArchBlocks.GetBlockSubRecord3DObjects( pBlockHeader );

		// Initialise temporary mesh object
		InitialiseMeshObject( &MeshTemp );

		// Build a combine mesh for all 3D subobjects in this block subrecord
		for ( int nObject = 0; nObject < pBlockHeader->n3DObjectRecords; nObject++ ) {
			// Build the mesh
			nObjectRecord = m_DaggerTool.m_Arch3D.ResolveObjectID( p3DObjects[nObject].nObjectID1 * 100 + p3DObjects[nObject].nObjectID2 );
			if ( !BuildMesh( nObjectRecord, &MeshSub ) )
				return FALSE;

			// Get rotation in degrees (512 is 90 degrees of rotation, so 1024 is 180 degrees, etc.)
			fRotate = (float)(90.0f * p3DObjects[nObject].Rotation / 512.0f );
			MeshSub.RotateDegrees( -fRotate, 0.0f, 0.0f );

			// Adjust position2 of this mesh
			MeshSub.Translate( (float)-(p3DObjects[nObject].XPos2), (float)-(p3DObjects[nObject].YPos2), (float)-(p3DObjects[nObject].ZPos2) );

			// Merge the submesh into the larger working mesh
			MeshTemp += MeshSub;
		}

		// Adjust Rotation of this working mesh
		fRotate = (float)(90.0f * pFLD->BlockPos[r].BlockRotation / 512.0f);
		MeshTemp.RotateDegrees( -fRotate, 0.0f, 0.0f );

		// Adjust position1 of this mesh
		MeshTemp.Translate( (float)-(pFLD->BlockPos[r].XPos2), 0.0f, (float)(pFLD->BlockPos[r].ZPos2) );

		// Merge working mesh into main mesh object
		MeshView += MeshTemp;
	}

	// Build additional outside features
	if ( nBlocks ) {
		InitialiseMeshObject( &MeshTemp );
		p3DObjects = m_DaggerTool.m_ArchBlocks.GetBlock3DObjects();
		for ( r = 0; r < m_DaggerTool.m_ArchBlocks.GetFLD()->nSubRecords2; r++ ) {
			// Build the mesh
			nObjectRecord = m_DaggerTool.m_Arch3D.ResolveObjectID( p3DObjects[r].nObjectID1 * 100 + p3DObjects[r].nObjectID2 );
			if ( !BuildMesh( nObjectRecord, &MeshSub ) )
				return FALSE;

			// Get rotation in degrees (512 is 90 degrees of rotation, so 1024 is 180 degrees, etc.)
			fRotate = (float)(90.0f * p3DObjects[r].Rotation / 512.0f);
			MeshSub.RotateDegrees( -fRotate, 0.0f, 0.0f );

			// Adjust position2 of this mesh
			MeshSub.Translate( (float)-(p3DObjects[r].XPos2), (float)-(p3DObjects[r].YPos2), (float)-(p3DObjects[r].ZPos2) );

			MeshTemp += MeshSub;
		}

		// Merge auxillary 3D objects into outside block
		MeshView += MeshTemp;
	}

	// Build block ground plane
	BuildBlockGroundMesh( &pFLD->SmallMaps, &MeshTemp );
	MeshTemp.Translate( -4096.0, -3.0f, 0.0f );
	MeshView += MeshTemp;

	// Recentre combine mesh so rotation pivots on central axis
	MeshView.Translate( 2048.0f, 0.0f, -2048.0f );

	// Commit texture cache to store
	CommitDFTextures();

	// Install mesh resource into engine
	UINT nViewMesh = m_Alchemy.InstallMeshFromMemory( &MeshView );

	// Create an actor for this mesh
	ACSETTINGS_ACTOR acsa;
	ZeroMemory( &acsa, sizeof(ACSETTINGS_ACTOR) );
	acsa.Object.dwSize = sizeof(ACSETTINGS_ACTOR);
	acsa.Object.vPos = D3DXVECTOR3(x,0.0f,y);
	acsa.Object.bFixed = TRUE;
	acsa.Object.Material.Ambient = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	acsa.Object.Material.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	//acsa.Object.vSpinPos = D3DXVECTOR3( 0.0f, frot, 0.0f );
	acsa.nMesh = nViewMesh;
	UINT nViewActor = m_Alchemy.InstallObject( ACOBJECT_ACTOR, &acsa, FALSE );

	// Release texture list to remove create-reference from each texture
	ReleaseDFTextures();

	return TRUE;


	/////////////////////////////////////////////////////////////////////


	// Remove create-reference from mesh as it will now managed by actor
	//m_Alchemy.DecResRef( nViewMesh );

	// Build block ground plane
	/*
	BuildBlockGroundMesh( &pFLD->SmallMaps, &MeshTemp );
	MeshTemp.Translate( -4096.0, -3.0f, 0.0f );
	MeshView += MeshTemp;
	*/

	/*
	// Install all mesh objects, textures, and actors into Alchemy
	if ( !BuildBlockObjects( pFLD, x, y ) ) {
		AbortDFTextures();
		return FALSE;
	}
	*/

	// Commit texture cache to store
	//CommitDFTextures();

	// Release texture list to remove create-reference from each texture
	//ReleaseDFTextures();

	/*
	// Mesh Building Rules:
	// 1. Build mesh object
	// 2. CommitDFTextures()
	// 3. Install mesh object into store
	// 4. Set actor to use mesh
	// 5. ReleaseDFTextures()
	// 6. DecResRef( mesh )

	// Clear current actor settings
	ACSETTINGS_ACTOR acsa;
	acsa.Object.dwSize = sizeof(ACSETTINGS_ACTOR);
	m_Alchemy.GetObject( m_nViewActor, &acsa );
	acsa.nMesh = ACNULL;
	m_Alchemy.SetObject( m_nViewActor, &acsa, FALSE );

	// Delete existing automap sprite if present
	if ( ACNULL != m_nAutomapSprite ) {
		m_Alchemy.RemoveSprite( m_nAutomapSprite );
		m_nAutomapSprite = NULL;
	}

	// Open this block
	if ( !m_DaggerTool.m_ArchBlocks.OpenBlock( m_nCurBlockObject ) ) {
		m_Alchemy.EnableObject( m_nViewActor, FALSE );
		return FALSE;
	}

	// Exit if no subrecords
	if ( !m_DaggerTool.m_ArchBlocks.GetBlockSubRecordCount() ) {
		m_Alchemy.EnableObject( m_nViewActor, FALSE );
		return FALSE;
	}

	// Reference start of information
	LPRMBFLD pFLD = m_DaggerTool.m_ArchBlocks.GetFLD();

	// Install minimap texture
	RECT rct = {0,0,64,64};
	UINT nTexture = ACNULL;
	BLKIMG_AUTOMAP imgAutomap;
	m_DaggerTool.m_ArchBlocks.GetBlockAutomap( &imgAutomap );
	nTexture = m_Alchemy.InstallTextureFromMemory( ACNULL, &rct, 64, imgAutomap.pBuffer, ACBD_32, "MiniMap" );
	if ( ACNULL == nTexture ) {
		m_DaggerTool.m_ArchBlocks.CloseBlock();
		m_Alchemy.EnableObject( m_nViewActor, FALSE );
		return FALSE;
	}

	// Initialise main mesh object
	InitialiseMeshObject( &m_meshView );

	// Build block objects
	if ( !BuildBlockObjects( pFLD ) ) {
		m_Alchemy.EnableObject( m_nViewActor, FALSE );
		return FALSE;
	}

	// Recentre combine mesh so rotation pivots on central axis
	m_meshView.Translate( 2048.0f, 0.0f, -2048.0f );

	// Commit texture cache to store
	CommitDFTextures();

	// Install mesh resource into engine
	UINT nViewMesh = m_Alchemy.InstallMeshFromMemory( &m_meshView );
	if ( ACNULL == nViewMesh ) {
		ReleaseDFTextures();
		return FALSE;
	}

	// Create a sprite for automap
	ACSETTINGS_SPRITE acss;
	ZeroMemory( &acss, sizeof(ACSETTINGS_SPRITE) );
	acss.dwSize = sizeof(ACSETTINGS_SPRITE);
	acss.dwFlags = 0;
	acss.nTexture = nTexture;
	acss.vPos.x = 0.0f;
	acss.vPos.y = 0.0f;
	acss.vScaling.x = acss.vScaling.y = 2.0f;
	acss.colour = DECOLOUR_SPRITE;
	m_nAutomapSprite = m_Alchemy.InstallSprite( &acss );
	if ( ACNULL == m_nAutomapSprite ) {
		m_Alchemy.DecResRef( nTexture );
		m_DaggerTool.m_ArchBlocks.CloseBlock();
		m_Alchemy.EnableObject( m_nViewActor, FALSE );
		return FALSE;
	}

	// Decrement automap texture reference count so it dies with the sprite
	m_Alchemy.DecResRef( nTexture );

	// Reset object if required
	if ( bResetObject ) {
		switch( m_dwMouseMode )
		{
		case MOUSEMODE_OBJECT:
			ResetObject();
			break;

		case MOUSEMODE_CAMERA:
			ResetCamera();
			break;
		}
	}

	// Set actor to use new mesh
	acsa.Object.dwSize = sizeof(ACSETTINGS_ACTOR);
	m_Alchemy.GetObject( m_nViewActor, &acsa );
	acsa.nMesh = nViewMesh;
	m_Alchemy.SetObject( m_nViewActor, &acsa );

	// Release texture list to remove create-reference from each texture
	ReleaseDFTextures();

	// Remove create-reference from mesh as it will now managed by actor
	m_Alchemy.DecResRef( nViewMesh );

	// Set number of interior objects
	m_nSubBlockObjectCount = m_DaggerTool.m_ArchBlocks.GetBlockSubRecordCount();
	*/

	//return TRUE;
}/* BuildBlockView */


//
// BOOL BuildBlockObjects( LPRMBFLD pFLD, float x, float y )
// Construct the exterior block object
// Return: TRUE if successful, otherwise FALSE
//
BOOL CDFCartographerApp::BuildBlockObjects( LPRMBFLD pFLD, float x, float y )
{
	return TRUE;

	//_ASSERT( pFLD );

	/*
	// Build all subrecords
	CString str;
	UINT nResource = ACNULL;
	float fRotate;
	long nObjectRecord;
	TFlexibleMesh Mesh, MeshSub;
	LPRMB_BLOCKHEADER pBlockHeader;
	LPRMB_BLOCK3DOBJECTS p3DObjects;
	for ( long r = 0; r < m_DaggerTool.m_ArchBlocks.GetBlockSubRecordCount(); r++ ) {
		// Get subrecord header
		pBlockHeader = m_DaggerTool.m_ArchBlocks.GetOutsideBlockSubRecord( r );
		if ( !pBlockHeader )
			return FALSE;

		// Get list of subobjects
		p3DObjects = m_DaggerTool.m_ArchBlocks.GetBlockSubRecord3DObjects( pBlockHeader );

		// Initialise temporary mesh object
		InitialiseMeshObject( &Mesh );

		// Build a combine mesh for all 3D subobjects in this block subrecord
		for ( int nObject = 0; nObject < pBlockHeader->n3DObjectRecords; nObject++ ) {
			// Get RecordID for this mesh
			nObjectRecord = m_DaggerTool.m_Arch3D.ResolveObjectID( p3DObjects[nObject].nObjectID1 * 100 + p3DObjects[nObject].nObjectID2 );

			// Check this mesh is not already in the store
			str.Format( "%u", nObjectRecord );
			nResource = m_Alchemy.FindResource( str );

			// If resource was not found in store, then build mesh
			if ( ACNULL == nResource ) {
				// Build the mesh
				if ( !BuildMesh( nObjectRecord, &Mesh ) )
					return FALSE;

				// KLUDGE: Commit textures before installing mesh
				// This is a real cheat, but texture handling is currently inadequate
				CommitDFTextures();

				// Add it to the store
				nResource = m_Alchemy.InstallMeshFromMemory( &Mesh, str );
				if ( ACNULL == nResource )
					return FALSE;

				// KLUDGE: Related to above KLUDGE
				ReleaseDFTextures();
			}

			// Adjust rotation2 of this actor
			float frot = 0.0f;
			//fRotate = (float)(90.0f * p3DObjects[nObject].Rotation / 512.0f );
			//fRotate = (float)(90.0f * pFLD->BlockPos[r].BlockRotation / 512.0f);
			//frot += D3DXToRadian(-fRotate);

			// Adjust potition2 of this actor
			float xx = 0.0f;
			float yy = 0.0f;
			float zz = 0.0f;
			xx = (float)-(p3DObjects[nObject].XPos2);
			yy = (float)-(p3DObjects[nObject].YPos2);
			zz = (float)-(p3DObjects[nObject].ZPos2);
			//xx += (float)-(pFLD->BlockPos[r].XPos2);
			//zz += (float)(pFLD->BlockPos[r].ZPos2);
			//xx += x;
			//zz += y;

			// Create an actor for this mesh
			ACSETTINGS_ACTOR acsa;
			ZeroMemory( &acsa, sizeof(ACSETTINGS_ACTOR) );
			acsa.Object.dwSize = sizeof(ACSETTINGS_ACTOR);
			acsa.Object.vPos = D3DXVECTOR3(xx,yy,zz);
			acsa.Object.bFixed = TRUE;
			acsa.Object.Material.Ambient = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			acsa.Object.Material.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
			acsa.Object.vSpinPos = D3DXVECTOR3( 0.0f, frot, 0.0f );
			acsa.nMesh = nResource;
			UINT nViewActor = m_Alchemy.InstallObject( ACOBJECT_ACTOR, &acsa, FALSE );
			if ( ACNULL == nViewActor ) {
				_ASSERT(FALSE);
				return FALSE;
			}

			// Adjust rotation1 of this actor
			frot = 0.0f;
			//fRotate = (float)(90.0f * p3DObjects[nObject].Rotation / 512.0f );
			//frot += D3DXToRadian(-fRotate);
			fRotate = (float)(90.0f * pFLD->BlockPos[r].BlockRotation / 512.0f);
			frot += D3DXToRadian(-fRotate);
			m_Alchemy.ObjectRotateYawPitchRoll( nViewActor, 0.0f, frot, 0.0f );

			// Adjust potition1 of this actor
			xx = 0.0f;
			yy = 0.0f;
			zz = 0.0f;
			xx += (float)-(pFLD->BlockPos[r].XPos2);
			zz += (float)(pFLD->BlockPos[r].ZPos2);
			xx += x;
			zz += y;
			m_Alchemy.ObjectTranslate( nViewActor, xx, 0.0f, zz );
		}
	}
	*/

	/*
	// Build block ground plane
	TFlexibleMesh MeshTemp;
	BuildBlockGroundMesh( &pFLD->SmallMaps, &MeshTemp );
	CommitDFTextures();
	MeshTemp.CentreAxis( AFMFCENTRE_X|AFMFCENTRE_Z );
	MeshTemp.RotateDegrees( 0.0f, 0.0f, 0.0f );
	MeshTemp.Translate( -4096.0, -3.0f, 0.0f );
	nResource = m_Alchemy.InstallMeshFromMemory( &MeshTemp );
	_ASSERT(nResource);
	ReleaseDFTextures();
	ACSETTINGS_ACTOR acsa;
	ZeroMemory( &acsa, sizeof(ACSETTINGS_ACTOR) );
	acsa.Object.dwSize = sizeof(ACSETTINGS_ACTOR);
	acsa.Object.vPos = D3DXVECTOR3(x,-3.0f,y);
	acsa.Object.bFixed = TRUE;
	acsa.Object.Material.Ambient = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	acsa.Object.Material.Diffuse = D3DXCOLOR(1.0f,1.0f,1.0f,1.0f);
	acsa.nMesh = nResource;
	UINT nViewActor = m_Alchemy.InstallObject( ACOBJECT_ACTOR, &acsa, FALSE );
	_ASSERT(nViewActor);
	*/

	//return TRUE;

	/*
	// Build all subrecords
	float fRotate;
	long nObjectRecord;
	TFlexibleMesh MeshTemp, MeshSub;
	LPRMB_BLOCKHEADER pBlockHeader;
	LPRMB_BLOCK3DOBJECTS p3DObjects;
	for ( long r = 0; r < m_DaggerTool.m_ArchBlocks.GetBlockSubRecordCount(); r++ ) {
		// Skip if drawing single objects and this is not the current object
		if ( !m_bDrawAllBlocks && r != m_nCurSubBlockObject ) {
			continue;
		}

		// Draw interior if specified
		if ( r == m_nCurSubBlockObject && m_bViewInterior ) {
			BuildInsideBlockObject( pFLD );
			continue;
		}

		// Get subrecord header
		pBlockHeader = m_DaggerTool.m_ArchBlocks.GetOutsideBlockSubRecord( r );
		if ( !pBlockHeader ) {
			m_Alchemy.EnableObject( m_nViewActor, FALSE );
			return FALSE;
		}

		// Get list of subobjects
		p3DObjects = m_DaggerTool.m_ArchBlocks.GetBlockSubRecord3DObjects( pBlockHeader );

		// Initialise temporary mesh object
		InitialiseMeshObject( &MeshTemp );

		// Build a combine mesh for all 3D subobjects in this block subrecord
		for ( int nObject = 0; nObject < pBlockHeader->n3DObjectRecords; nObject++ ) {
			// Build the mesh
			nObjectRecord = m_DaggerTool.m_Arch3D.ResolveObjectID( p3DObjects[nObject].nObjectID1 * 100 + p3DObjects[nObject].nObjectID2 );
			if ( !BuildMesh( nObjectRecord, &MeshSub ) )
				return FALSE;

			// Get rotation in degrees (512 is 90 degrees of rotation, so 1024 is 180 degrees, etc.)
			fRotate = (float)(90.0f * p3DObjects[nObject].Rotation / 512.0f );
			MeshSub.RotateDegrees( -fRotate, 0.0f, 0.0f );

			// Adjust position2 of this mesh
			MeshSub.Translate( (float)-(p3DObjects[nObject].XPos2), (float)-(p3DObjects[nObject].YPos2), (float)-(p3DObjects[nObject].ZPos2) );

			// Merge the submesh into the larger working mesh
			MeshTemp += MeshSub;
		}

		// Adjust Rotation of this working mesh
		fRotate = (float)(90.0f * pFLD->BlockPos[r].BlockRotation / 512.0f);
		MeshTemp.RotateDegrees( -fRotate, 0.0f, 0.0f );

		// Adjust position1 of this mesh
		MeshTemp.Translate( (float)-(pFLD->BlockPos[r].XPos2), 0.0f, (float)(pFLD->BlockPos[r].ZPos2) );

		// Merge working mesh into main mesh object
		m_meshView += MeshTemp;
	}

	// Build additional outside features
	if ( m_bDrawAllBlocks ) {
		InitialiseMeshObject( &MeshTemp );
		p3DObjects = m_DaggerTool.m_ArchBlocks.GetBlock3DObjects();
		for ( r = 0; r < m_DaggerTool.m_ArchBlocks.GetFLD()->nSubRecords2; r++ ) {
			// Build the mesh
			nObjectRecord = m_DaggerTool.m_Arch3D.ResolveObjectID( p3DObjects[r].nObjectID1 * 100 + p3DObjects[r].nObjectID2 );
			if ( !BuildMesh( nObjectRecord, &MeshSub ) )
				return FALSE;

			// Get rotation in degrees (512 is 90 degrees of rotation, so 1024 is 180 degrees, etc.)
			fRotate = (float)(90.0f * p3DObjects[r].Rotation / 512.0f);
			MeshSub.RotateDegrees( -fRotate, 0.0f, 0.0f );

			// Adjust position2 of this mesh
			MeshSub.Translate( (float)-(p3DObjects[r].XPos2), (float)-(p3DObjects[r].YPos2), (float)-(p3DObjects[r].ZPos2) );

			MeshTemp += MeshSub;
		}

		// Merge auxillary 3D objects into outside block
		m_meshView += MeshTemp;
	}

	// Build block ground plane
	if ( m_bDrawGround ) {
		BuildBlockGroundMesh( &pFLD->SmallMaps, &MeshTemp );
		MeshTemp.Translate( -4096.0, -3.0f, 0.0f );
		m_meshView += MeshTemp;
	}
	*/

	//return TRUE;
}/* BuildBlockObjects */


//
// BOOL ApplyRegion( long *pArchive )
// Change texture archive based on supported regions
// Return: TRUE if successful, otherwise FALSE
//
inline BOOL CDFCartographerApp::ApplyRegion( long *pArchive )
{
	_ASSERT( pArchive );

	// Exit if not using processor
	//if ( TERRAIN_NONE == m_dwTerrain ) {
	//	return TRUE;
	//}

	// Determine whether to apply climate offset
	long nClimate = 0;
	DWORD dwTerrain = TERRAIN_TEMPERATE;
	//if ( WEATHER_WINTER == m_dwWeather && TERRAIN_DESERT != m_dwTerrain ) {
	//	nClimate = 1;
	//}

	// Find base index index of this texture
	long nBase = *pArchive / 100;

	// Process region against texture archive
	long nIndex = *pArchive - (nBase * 100);
	switch ( nIndex )
	{
		case STO_TERRAIN:
		case STO_RUINS:
		case STO_CASTLE:
		case STO_CITYA:
		case STO_CITYB:
		case STO_CITYWALLS:
		case STO_FARM:
		case STO_FENCES:
		case STO_MAGESGUILD:
		case STO_MANOR:
		case STO_MERCHANTHOMES:
		case STO_TAVERNEXTERIORS:
		case STO_TEMPLEEXTERIORS:
		case STO_VILLAGE:
		case STO_ROOFS:
			*pArchive = (nIndex += dwTerrain + nClimate);
			break;

		default:
			break;
	};

	return TRUE;
}/* ApplyRegion */
// Daggerfall Cartographer.h : main header file for the Daggerfall Cartographer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "MainFrm.h"
#include "ArenaBar.h"
#include "ChildView.h"
#include "resource.h"       // main symbols


// Defines
#define DFCSTR_STARTREGION		"Daggerfall"
//#define DFCSTR_STARTREGION		"Sentinel"

#define DE_CACHETEXTURE_WIDTH 256
#define DE_CACHETEXTURE_HEIGHT 256
#define DE_CACHETEXTURE_PITCH 768
#define DE_CACHETEXTURE_BYTES 196608


// Location images
enum
{
	DFCIMG_LOCATION,
	DFCIMG_ALL,
	DFCIMG_DUNGEON,
	DFCIMG_HOME,
	DFCIMG_TEMPLE,
	DFCIMG_GRAVEYARD,
	DFCIMG_TAVERN,
	DFCIMG_TOWN,
	DFCIMG_UNKNOWN,
};

// Locations
enum
{
	DFCLOC_DUNGEON_LABYRINTHS	= 132,		// Dungeons
	DFCLOC_DUNGEON_KEEPS		= 135,
	DFCLOC_DUNGEON_RUINS		= 138,

	DFCLOC_GRAVEYARD_COMMON		= 172,		// Graveyards
	DFCLOC_GRAVEYARD_FORGOTTEN	= 140,

	DFCLOC_HOME_FARMS			= 163,		// Homes
	DFCLOC_HOME_WEALTHY			= 168,
	DFCLOC_HOME_POOR			= 171,
	DFCLOC_HOME_YOURSHIPS		= 174,

	DFCLOC_TAVERN				= 166,		// Taverns

	DFCLOC_RELIGION_TEMPLES		= 165,		// Temples
	DFCLOC_RELIGION_CULTS		= 169,
	DFCLOC_RELIGION_COVENS		= 141,

	DFCLOC_TOWN_CITIES			= 160,		// Towns
	DFCLOC_TOWN_HAMLETS			= 161,
	DFCLOC_TOWN_VILLAGES		= 162,

};

// Location filters
enum
{
	DFCFLT_ALL					= 0,

	DFCFLT_DUNGEONS				= 1,
	DFCFLT_DUNGEON_LABYRINTHS	= 132,
	DFCFLT_DUNGEON_KEEPS		= 135,
	DFCFLT_DUNGEON_RUINS		= 138,

	DFCFLT_GRAVEYARDS			= 2,
	DFCFLT_GRAVEYARD_COMMON		= 172,
	DFCFLT_GRAVEYARD_FORGOTTEN	= 140,

	DFCFLT_HOMES				= 3,
	DFCFLT_HOME_FARMS			= 163,
	DFCFLT_HOME_WEALTHY			= 168,
	DFCFLT_HOME_POOR			= 171,
	DFCFLT_HOME_YOURSHIPS		= 174,

	DFCFLT_TAVERNS				= 166,

	DFCFLT_RELIGIONS		= 5,
	DFCFLT_RELIGION_TEMPLES	= 165,
	DFCFLT_RELIGION_CULTS	= 169,
	DFCFLT_RELIGION_COVENS	= 141,

	DFCFLT_TOWNS			= 6,
	DFCFLT_TOWN_CITIES		= 160,
	DFCFLT_TOWN_HAMLETS		= 161,
	DFCFLT_TOWN_VILLAGES	= 162,
};


// Stores a single location
typedef struct _DFC_LOCATION
{
	CString		strName;
	long		Type;
} DFC_LOCATION, *LPDFC_LOCATION;


// Render modes - How an object will be drawn
enum
{
	OBJDRAW_PUREFACES,
	OBJDRAW_WIREFRAME,
	OBJDRAW_SOLID,
	OBJDRAW_TEXTURED,
};

// Stores a single face during triangulation
typedef struct _WIREFACE {
	TString		strTexture;
	DWORD		dwVertCount;
	OBJVERTEX	verts[64];
} WIREFACE;


// Stores a combine texture object during texture cache operations
typedef struct _DFTEXTURECACHE
{
	_DFTEXTURECACHE() {
		UINT nTexture = ACNULL;
		cx = maxWidth = cy = 0;
		memset( pBits, 0, DE_CACHETEXTURE_BYTES );
	}

	TString		strName;							// Name of this texture in the store
	int			cx;									// Position of current column
	int			maxWidth;							// Maximum width of this column
	int			cy;									// Height of current column
	char		pBits[DE_CACHETEXTURE_BYTES];		// R8G8B8 texture bits for this combine texture
	RECT		rctTexture;							// Rect of this texture
	TLinkedList<RECT> oaSubset;						// List of rectangles to reference subset of surface
} DFTEXTURECACHE, *LPDFTEXTURECACHE;

// Reference a combine texture object and an offset
typedef struct _DFTEXTUREREF
{
	UINT		nTexture;						// Reference for texture in cache table
	UINT		nSubset;						// Reference for subset of texture in cache table
} DFTEXTUREREF, *LPDFTEXTUREREF;


// CDFCartographerApp:
// See Daggerfall Cartographer.cpp for the implementation of this class
//

class CDFCartographerApp : public CWinApp
{
public:
	CDFCartographerApp();

private:	// methods
	void			InitFltCBI( COMBOBOXEXITEM *pcbi, int iItem, int iIndent, int iImage, LPARAM lParam, LPCSTR pszText );

private:	// objects
	CDaggerTool		m_DaggerTool;
	TAlchemy		m_Alchemy;
	TLog			m_Log;
	CString			m_strStartDirectory;
	CMainFrame*		m_pFrame;
	CChildView*		m_pMediaView;
	CArenaBar*		m_pArenaBar;
	UINT			m_nDefaultFont;
	long			m_lCurRegion;
	long			m_lCurFilter;
	long			m_lCurLocation;
	DWORD			m_dwDrawMode;

	UINT m_nLastDFTextureCache;
	TLinkedList<DFTEXTURECACHE> m_oaDFTextureCache;
	THashArray<DFTEXTUREREF, 97> m_haDFTextureCacheIndexCombine;
	THashArray<DFTEXTUREREF, 97> m_haDFTextureCacheIndexVerbose;

public:		// methods
	void		PresentMediaView();
	void		SizeMediaView( int x, int y );
	void		OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void		OnRegionSelected();
	void		OnFilterSelected();
	void		OnLocationSelected( long lLocation );

private:	// methods
	BOOL		FailInitApp();
	BOOL		DrawMediaView();
	BOOL		InitialiseViews();
	BOOL		SetRegion( long nRegion );
	BOOL		BuildLocationView();
	BOOL		BuildMediaView();
	BOOL		BuildMiniMap();
	BOOL		Build3DView();

	BOOL		InitialiseMeshObject( TFlexibleMesh* pMesh, DWORD dwVertCount = 0, DWORD dwFaceCount = 0 );
	BOOL		BuildMesh( long nObject, TFlexibleMesh* pMeshOut );
	BOOL		BuildBlockGroundMesh( LPRMBFLD_SMALLMAPDATA pMap, TFlexibleMesh* pMeshOut );

	BOOL		BuildBlockActors( long nBlock, float x, float y );
	BOOL		BuildBlockObjects( LPRMBFLD pFLD, float x, float y );

	BOOL		CacheDFTextureCombine( LPCSTR pszName, long nArchive, long nRecord, LPRECT pRectOut, TString& strCacheNameOut );
	BOOL		CacheDFTextureVerbose( LPCSTR pszName, long nArchive, long nRecord, LPRECT pRectOut );
	BOOL		CommitDFTextures();
	BOOL		AbortDFTextures();
	void		ReleaseDFTextures();

	BOOL		ApplyRegion( long *pArchive );

private:	// objects
	TStaticArray<DFC_LOCATION> m_haLocation;

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	afx_msg void OnViewArenaView();
	afx_msg void OnUpdateViewArenaView(CCmdUI *pCmdUI);
	afx_msg void OnFileOpenArena2();
	afx_msg void OnFileCloseArena2();
};

extern CDFCartographerApp theApp;
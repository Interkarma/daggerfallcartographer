// TDX8Graphics.h: interface for the TDX8Graphics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDX8GRAPHICS_H__B1E6E7C5_0056_4C49_B32A_086AA123B86A__INCLUDED_)
#define AFX_TDX8GRAPHICS_H__B1E6E7C5_0056_4C49_B32A_086AA123B86A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDirectX8.h"


// Helper function to stuff a FLOAT into a DWORD argument
inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }


// Adapter description
typedef struct _tagAdapterDesc
{
	BOOL					bD3DCapable;
	BOOL					bHardwareRZ;
	BOOL					bHardwareTL;
	BOOL					bWindowSupport;
	BOOL					bSupportsPureDevice;
	D3DADAPTER_IDENTIFIER8	AdapterID;
	HMONITOR				hMonitor;
	UINT					nModeCount;
	D3DCAPS8				d3dCaps;

	TStaticArray<D3DDISPLAYMODE> oaMode;
} ADAPTERDESC, *LPADAPTERDESC;


class TDX8Graphics  
{
public:		// construction / destruction
	friend class TDirectX8;
	TDX8Graphics();
	virtual ~TDX8Graphics();

public:		// methods
	BOOL	Attach( HWND hWnd );
	BOOL	Attach( HWND hWnd, UINT nBackBufferMode, BOOL bDefaultRefreshRate = TRUE );
	BOOL	Reset();
	BOOL	Reset( DWORD dwWidth, DWORD dwHeight, DWORD dwColourDepth );
	void	Detach();

	UINT	GetAdapterCount() {return m_nAdapterCount;}
	BOOL	GetAdapterDesc( UINT nAdapter, LPADAPTERDESC pAdapterDescOut );
	BOOL	FindBackBufferMode( UINT* pnModeOut, UINT nAdapter, BOOL bWindowed, DWORD dwWidth, DWORD dwHeight, DWORD dwColourDepth, DWORD dwRefreshRate = 0, D3DFORMAT d3dFormat = D3DFMT_UNKNOWN );

	UINT		GetAdapter() {return m_nAdapter;}
	BOOL		SetAdapter( UINT nAdapter );
	D3DDEVTYPE	GetDeviceType() {return m_d3dDeviceType;}
	BOOL		SetDeviceType( D3DDEVTYPE d3dDevType );
	DWORD		GetBehaviour() {return m_dwDeviceBehaviour;}
	BOOL		SetBehaviour( DWORD dwBehaviourFlags );

	BOOL	GetDevice( LPDIRECT3DDEVICE8* pd3ddOut );
	BOOL	GetDeviceCaps( D3DCAPS8* pCapsOut );
	BOOL	GetAdapterMode( UINT nAdapter, UINT nMode, D3DDISPLAYMODE* pd3ddmOut );
	void	GetDisplayMode( D3DDISPLAYMODE* pd3ddmOut );
	void	GetPresentParameters( D3DPRESENT_PARAMETERS* pd3dppOut );

	BOOL	Reset( D3DPRESENT_PARAMETERS* pd3dpp );
	BOOL	RecoverDevice( D3DPRESENT_PARAMETERS* pd3dpp = NULL );
	BOOL	IsResetInProgress() {return m_bResetInProgress;}

	HRESULT CreateDynamicVertexBuffer( UINT Length, DWORD FVF, LPDIRECT3DVERTEXBUFFER8* ppVertexBuffer );
	HRESULT	CreateStaticIndexBuffer( UINT Length, LPDIRECT3DINDEXBUFFER8* ppIndexBuffer );

private:	// methods
	BOOL	Create( LPDIRECTX pDirectX );
	HRESULT CreateDevice( HWND hWnd, D3DPRESENT_PARAMETERS* pd3dpp );
	void	Destroy();

private:	// inline methods
	inline BOOL		IsCreated() {return m_bIsCreated;}
	inline BOOL		IsAttached() {return m_bIsAttached;}

public:		// objects
	TLog	m_Log;

private:	// objects
	LPDIRECTX			m_pDirectX;
	LPDIRECT3D8			m_pDirect3D;
	LPDIRECT3DDEVICE8	m_pd3dDevice;

	TStaticArray<ADAPTERDESC> m_oaAdapter;

private:	// data
	UINT	m_nAdapter;
	UINT	m_nAdapterCount;
	DWORD	m_dwDeviceBehaviour;
	BOOL	m_bWindowed;
	BOOL	m_bIsCreated;
	BOOL	m_bIsAttached;
	BOOL	m_bResetInProgress;
	HWND	m_hWndOwner;

	D3DCAPS8				m_d3dDeviceCaps;
	D3DDEVTYPE				m_d3dDeviceType;
	D3DDISPLAYMODE			m_d3dDisplayMode;
	D3DPRESENT_PARAMETERS	m_d3dPresentParameters;
};


#endif // !defined(AFX_TDX8GRAPHICS_H__B1E6E7C5_0056_4C49_B32A_086AA123B86A__INCLUDED_)

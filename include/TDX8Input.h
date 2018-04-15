// TDX8Input.h: interface for the TDX8Input class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TDX8INPUT_H__ADA2D5AB_2AED_43BB_9196_301336F68FD8__INCLUDED_)
#define AFX_TDX8INPUT_H__ADA2D5AB_2AED_43BB_9196_301336F68FD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TDirectX8.h"


class TDX8Input  
{
public:		// construction / destruction
	friend class TDirectX8;
	TDX8Input();
	virtual ~TDX8Input();

public:		// methods
	BOOL	Attach( HINSTANCE hInst, HWND hWnd, BOOL bApplyDefaults = FALSE );
	void	Detach();

	BOOL	ManageDevices( DWORD dwDevices );
	BOOL	GetKeyboardState( LPVOID pBuffer );

private:	// methods
	BOOL	Create( LPDIRECTX pDirectX );
	void	Destroy();

public:		// objects
	TLog	m_Log;

private:	// objects
	LPDIRECTX			m_pDirectX;
	LPDIRECTINPUT8		m_pDirectInput;

	LPDIRECTINPUTDEVICE8	m_pSysKeyboard;
	LPDIRECTINPUTDEVICE8	m_pSysMouse;

	BOOL	m_bSysKeyboardLost;

private:	// inline methods
	inline BOOL		IsCreated() {return m_bIsCreated;}
	inline BOOL		IsAttached() {return m_bIsAttached;}

private:	// data
	BOOL	m_bIsCreated;
	BOOL	m_bIsAttached;

	HINSTANCE	m_hInstOwner;
	HWND		m_hWndOwner;

	BOOL		m_bSysKeyboardManaged;
	BOOL		m_bSysMouseManaged;
};


#endif // !defined(AFX_TDX8INPUT_H__ADA2D5AB_2AED_43BB_9196_301336F68FD8__INCLUDED_)

#pragma once

#include "ArenaDialog.h"

// CArenaBar

#ifndef baseCViewBar
#define baseCViewBar CSizingControlBarG
#endif

class CArenaBar : public baseCViewBar
{
public:
	CArenaBar();
	virtual ~CArenaBar();

public:		// objects
	CArenaDialog	m_wndArenaDlg;

private:	// objects
	BOOL			m_bIsCreated;
	CImageList		m_ilLocations;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};



#pragma once

#include "resource.h"
#include "afxwin.h"
#include "afxcmn.h"


// CArenaDialog dialog

class CArenaDialog : public CDialog
{
	DECLARE_DYNAMIC(CArenaDialog)

public:
	CArenaDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CArenaDialog();

// Dialog Data
	enum { IDD = IDD_ARENA_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlArenaList;
	CComboBoxEx m_ctrlRegionCombo;
	CComboBoxEx m_ctrlLocationCombo;
	afx_msg void OnCbnSelchangeArenaLocationCombo();
	afx_msg void OnCbnSelchangeArenaRegionCombo();
	afx_msg void OnLvnItemActivateArenaList(NMHDR *pNMHDR, LRESULT *pResult);
protected:
	virtual void OnOK();
	virtual void OnCancel();
};

#if !defined(AFX_HISTDLG_H__9AE8FB45_7BD7_48B0_B0F4_EFBDDB34E740__INCLUDED_)
#define AFX_HISTDLG_H__9AE8FB45_7BD7_48B0_B0F4_EFBDDB34E740__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistDlg.h : header file
//

#include "HistView.h"
/////////////////////////////////////////////////////////////////////////////
// CHistDlg dialog

class CHistDlg : public CDialog
{
// Construction
public:
	CHistDlg(CWnd* pParent = NULL);   // standard constructor
// Data
	int		m_iOffset_b;	//коррекция яркости снизу
	int		m_iOffset_t;	//коррекция яркости сверху
// Operations
	void SetData(const DWORD *pHist, int Range) {m_ctrlHist.SetData(pHist, Range);};
// Dialog Data
	//{{AFX_DATA(CHistDlg)
	enum { IDD = IDD_HIST };
	CHistView	m_ctrlHist;
	CSliderCtrl	m_ctrlOffset_b;
	CSliderCtrl	m_ctrlOffset_t;
	CString	m_strOffset_b;
	CString	m_strOffset_t;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CHistDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTDLG_H__9AE8FB45_7BD7_48B0_B0F4_EFBDDB34E740__INCLUDED_)

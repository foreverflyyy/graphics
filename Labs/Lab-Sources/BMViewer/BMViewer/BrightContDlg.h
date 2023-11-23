#if !defined(AFX_BRIGHTCONTDLG_H__310F8D78_02DE_4055_A8DD_609EFC475802__INCLUDED_)
#define AFX_BRIGHTCONTDLG_H__310F8D78_02DE_4055_A8DD_609EFC475802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BrightContDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBrightContDlg dialog

class CBrightContDlg : public CDialog
{
// Construction
public:
	CBrightContDlg(CWnd* pParent = NULL);   // standard constructor

	int m_iBrightnessOffset;
	int m_iContrastOffset;
// Dialog Data
	//{{AFX_DATA(CBrightContDlg)
	enum { IDD = IDD_BRIGHT_CONT };
	CSliderCtrl	m_ctrlContrast;
	CSliderCtrl	m_ctrlBrightness;
	CString	m_strBrightness;
	CString	m_strContrast;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrightContDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBrightContDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BRIGHTCONTDLG_H__310F8D78_02DE_4055_A8DD_609EFC475802__INCLUDED_)

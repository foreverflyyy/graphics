#if !defined(AFX_DENOISEDLG_H__58BC603D_E427_4FF4_BD2C_5FBEDCD9A847__INCLUDED_)
#define AFX_DENOISEDLG_H__58BC603D_E427_4FF4_BD2C_5FBEDCD9A847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeNoiseDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeNoiseDlg dialog

class CDeNoiseDlg : public CDialog
{
// Construction
public:
	CDeNoiseDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDeNoiseDlg)
	enum { IDD = IDD_DENOISE };
	CSpinButtonCtrl	m_ctrlSpinK;
	double	m_dK;
	int		m_nWhatToDo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeNoiseDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDeNoiseDlg)
	afx_msg void OnChangePosSpinK(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DENOISEDLG_H__58BC603D_E427_4FF4_BD2C_5FBEDCD9A847__INCLUDED_)

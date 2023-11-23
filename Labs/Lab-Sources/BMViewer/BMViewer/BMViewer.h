// BMViewer.h : main header file for the BMVIEWER application
//

#if !defined(AFX_BMVIEWER_H__854A9B8C_6BD4_44CA_B4E4_F00CEF40C1E7__INCLUDED_)
#define AFX_BMVIEWER_H__854A9B8C_6BD4_44CA_B4E4_F00CEF40C1E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols


/////////////////////////////////////////////////////////////////////////////
// CBMApp:
// See BMViewer.cpp for the implementation of this class
//

class CBMApp : public CWinApp
{
public:
	CBMApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBMApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMVIEWER_H__854A9B8C_6BD4_44CA_B4E4_F00CEF40C1E7__INCLUDED_)

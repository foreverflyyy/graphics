// BMView.h : interface of the CBMView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMVIEW_H__A58B1237_DC72_4389_BA43_93CC54F0F5EA__INCLUDED_)
#define AFX_BMVIEW_H__A58B1237_DC72_4389_BA43_93CC54F0F5EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBMView : public CScrollView
{
	// Идентификатор таймера
	UINT m_nTimer;
	// Заголовок окна 
	CString m_strOrgTitle;

	//Виртуальный экран
	CBitmap m_VirtScreenBitmap;	// растр 
	CDC		m_VirtScreenDC;		// контекст 
	
	// Масштаб вывода картинки
	double	m_dScale;
	int		m_nStretchMode;
	
protected: // create from serialization only
	CBMView();
	DECLARE_DYNCREATE(CBMView)

// Attributes
public:
	CBMDoc* GetDocument();

// Operations
public:
	BOOL UpdateVirtualScreen();

	void OnStartTimer();
	void OnStopTimer();
		
	afx_msg LONG OnStartTransform(UINT wParam, LONG lParam); 
	afx_msg LONG OnEndTransform(UINT wParam, LONG lParam); 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnDraw(CDC* pDC);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBMView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBMView)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnViewZoomin();
	afx_msg void OnViewZoomout();
	afx_msg void OnViewStretchhalftone();
	afx_msg void OnUpdateViewStretchhalftone(CCmdUI* pCmdUI);
	afx_msg void OnViewStretchblackonwhite();
	afx_msg void OnUpdateViewStretchblackonwhite(CCmdUI* pCmdUI);
	afx_msg void OnViewStretchwhiteonblack();
	afx_msg void OnUpdateViewStretchwhiteonblack(CCmdUI* pCmdUI);
	afx_msg void OnViewStretchcoloroncolor();
	afx_msg void OnUpdateViewStretchcoloroncolor(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BMView.cpp
inline CBMDoc* CBMView::GetDocument()
   { return (CBMDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMVIEW_H__A58B1237_DC72_4389_BA43_93CC54F0F5EA__INCLUDED_)

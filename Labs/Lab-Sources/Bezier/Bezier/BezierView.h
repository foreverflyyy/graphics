// Bezievw.h : interface of the CBezierView class
//
/////////////////////////////////////////////////////////////////////////////

class CBezierView : public CView
{
protected: // create from serialization only
	CBezierView();
	DECLARE_DYNCREATE(CBezierView)

// Attributes
public:
	CBezierDoc* GetDocument();
// Operations
	// Рисует базовую кривую
	void DrawBaseLine(CDC *pDC);
	// Рисует сплайновую кривую
	void DrawSplineLine(CDC *pDC);


public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBezierView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBezierView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBezierView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Bezievw.cpp
inline CBezierDoc* CBezierView::GetDocument()
   { return (CBezierDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

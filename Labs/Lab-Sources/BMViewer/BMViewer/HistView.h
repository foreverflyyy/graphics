#if !defined(AFX_HISTVIEW_H__5A2898D2_60B8_477C_A40B_FB7B0B4A7C6C__INCLUDED_)
#define AFX_HISTVIEW_H__5A2898D2_60B8_477C_A40B_FB7B0B4A7C6C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HistView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHistView window

class CHistView : public CStatic
{
// Construction
public:
	CHistView();

// Attributes
public:
	int			m_iRange;		//размер массива гистограммы
	const DWORD	*m_pHist;		//указатель на данные гистограммы
	COLORREF m_Color;		//цвет, которым рисовать гистограмму	 

// Operations
public:
	// Устанавливает данные для отображения
	void SetData(const DWORD *pHist,  int Range) {m_pHist=pHist; m_iRange=Range;};
	// Устанавливает увет рисования гистограммы
	void SetColor(const COLORREF &c) {m_Color=c;};
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHistView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHistView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CHistView)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HISTVIEW_H__5A2898D2_60B8_477C_A40B_FB7B0B4A7C6C__INCLUDED_)

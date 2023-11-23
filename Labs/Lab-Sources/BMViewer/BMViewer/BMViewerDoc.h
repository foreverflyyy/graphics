// BMDoc.h : interface of the CBMDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BMDOC_H__0CC04D31_A879_4674_AAD5_292699AE7BA7__INCLUDED_)
#define AFX_BMDOC_H__0CC04D31_A879_4674_AAD5_292699AE7BA7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxmt.h>
#include "Raster.h"
#include "Filter.h"

#define UM_STARTTRANSFORM	WM_USER+0x8000
#define UM_ENDOFTRANSFORM	UM_STARTTRANSFORM+1

class CBMDoc : public CDocument
{
protected: // create from serialization only
	CBMDoc();
	DECLARE_DYNCREATE(CBMDoc)

// Attributes
public:
	// ����� 
	BOOL	m_bEditable;	// ����� �� ������������� ������
	BOOL	m_bEditHalf;	// ������������� �������� �����������
	CEvent	m_EventDoTransform; // ������� - ����������� ��������������
	// ����������
	LONG	m_lExecutedPercent;

	// ������
	CRaster m_BM[2];	// ��� ������ ��� �����������
	CRaster *m_pCurBM;	// ��������� �� �������� ����� 
	
	//�������
	CFilter	*m_pCurFilter;
	CBrightCont m_BrightContFilter;
	CHistogram	m_HistogramFilter;
	CInvertColors m_InvertColorsFilter;
	CEmboss m_EmbossFilter;

	CBlur	m_BlurFilter;
	CContour m_ContourFilter;
	CSharp	m_SharpFilter;
	CDeNoise	m_DenoiseFilter;

// Operations
public:
	CSize GetCurrentBMSize();
	//���������� ����� ��������� ������
	int	GetNCurrentBM(); 
	//���������� ��������� �� ������� ��������
	CRaster* GetCurrentBMPtr(); 
	//���������� ��������� �� �����
	CRaster* GetBufferBMPtr();
	//�������� ������� ����������� � ������ ������� 
	void SwapBM();
	//���������� ������� ����������� 
	void DrawCurrent();
	// ������� ����� ��������� �������
	// (��� ������ ��� ���������� ������ ����� ������� ��������)
	// ����������� � ������� ���������
	BOOL CreateCompatibleBuffer(LONG width=0, LONG height=0);
	//��������� ����� ��������������
	void Transform();
	
protected:
	//������� ������ ��������������
	static UINT ThreadProc(LPVOID pv);
	//���� �������������� � �������������� ��������� �������
	void TransformLoop();
	//�������� ��������� ���� ����� 
	void InformAllViews(UINT msg, WPARAM wPar=0, LPARAM lPar=0);
		
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBMDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBMDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
// Generated message map functions
protected:
	//{{AFX_MSG(CBMDoc)
	afx_msg void OnEditHistogram();
	afx_msg void OnEditUndo();
	afx_msg void OnEditBrightnessandcontrast();
	afx_msg void OnEditInvertcolors();
	afx_msg void OnEditBlur();
	afx_msg void OnEditSharp();
	afx_msg void OnEditContour();
	afx_msg void OnEditHalf();
	afx_msg void OnUpdateEditHalf(CCmdUI* pCmdUI);
	afx_msg void OnEditEmboss();
	afx_msg void OnUpdateEditBlur(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditHistogram(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditEmboss(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditSharp(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditContour(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditBrightnessandcontrast(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditInvertcolors(CCmdUI* pCmdUI);
	afx_msg void OnEditDenoise();
	afx_msg void OnEditStop();
	afx_msg void OnUpdateEditStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditDenoise(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BMDOC_H__0CC04D31_A879_4674_AAD5_292699AE7BA7__INCLUDED_)

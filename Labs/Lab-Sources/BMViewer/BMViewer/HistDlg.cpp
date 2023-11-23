// HistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMViewer.h"
#include "HistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistDlg dialog


CHistDlg::CHistDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHistDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHistDlg)
	m_strOffset_b = _T("");
	m_strOffset_t = _T("");
	//}}AFX_DATA_INIT

	m_iOffset_b=0;	
	m_iOffset_t=0;	
}


void CHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHistDlg)
	DDX_Control(pDX, IDC_HIST_VIEW, m_ctrlHist);
	DDX_Control(pDX, IDC_SLIDER_HIST_B, m_ctrlOffset_b);
	DDX_Control(pDX, IDC_SLIDER_HIST_T, m_ctrlOffset_t);
	DDX_Text(pDX, IDC_OFFSET_B_TXT, m_strOffset_b);
	DDX_Text(pDX, IDC_OFFSET_T_TXT, m_strOffset_t);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHistDlg, CDialog)
	//{{AFX_MSG_MAP(CHistDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Operations
/////////////////////////////////////////////////////////////////////////////
// CHistDlg message handlers


BOOL CHistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// Ползунок нижней границы
	m_ctrlOffset_b.SetRange(0, 127);
	// Бегунок в крайнем левом положении
	m_ctrlOffset_b.SetPos(0);
	// Ползунок верхней границы
	m_ctrlOffset_t.SetRange(128, 255);
	// Бегунок в крайнем правом положении
	m_ctrlOffset_t.SetPos(255);
	// Текст
	m_strOffset_b="0";
	m_strOffset_t="0";
	// Цвет гистограммы
	m_ctrlHist.SetColor(RGB(0, 50, 50));

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHistDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_strOffset_b.Format(_T("%d"), m_ctrlOffset_b.GetPos());
	m_strOffset_t.Format(_T("%d"), 255-m_ctrlOffset_t.GetPos());	
	UpdateData(FALSE);	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CHistDlg::OnOK() 
{
	m_iOffset_b=m_ctrlOffset_b.GetPos();	
	m_iOffset_t=255-m_ctrlOffset_t.GetPos();		
	CDialog::OnOK();
}

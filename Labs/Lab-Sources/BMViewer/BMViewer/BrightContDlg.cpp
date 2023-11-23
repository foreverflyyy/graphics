// BrightContDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMViewer.h"
#include "BrightContDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrightContDlg dialog


CBrightContDlg::CBrightContDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBrightContDlg::IDD, pParent)
{
	m_iBrightnessOffset=0;
	m_iContrastOffset=0;
	//{{AFX_DATA_INIT(CBrightContDlg)
	m_strBrightness = _T("");
	m_strContrast = _T("");
	//}}AFX_DATA_INIT
}


void CBrightContDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBrightContDlg)
	DDX_Control(pDX, IDC_SLIDER_CONTRAST, m_ctrlContrast);
	DDX_Control(pDX, IDC_SLIDER_BRIGHTNESS, m_ctrlBrightness);
	DDX_Text(pDX, IDC_STATIC_BRIGHTNESS, m_strBrightness);
	DDX_Text(pDX, IDC_STATIC_CONTRAST, m_strContrast);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBrightContDlg, CDialog)
	//{{AFX_MSG_MAP(CBrightContDlg)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrightContDlg message handlers
#define MAX_CORRECTION_OFFSET 100
BOOL CBrightContDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Инициализация ползунков яркости и контрастности
	m_ctrlBrightness.SetRange(0, MAX_CORRECTION_OFFSET*2);
	m_ctrlBrightness.SetPos(MAX_CORRECTION_OFFSET);
	m_ctrlBrightness.SetTic(0);
	m_ctrlBrightness.SetTic(MAX_CORRECTION_OFFSET);
	m_ctrlBrightness.SetTic(MAX_CORRECTION_OFFSET*2);
	
	m_ctrlContrast.SetRange(0, MAX_CORRECTION_OFFSET*2);
	m_ctrlContrast.SetPos(MAX_CORRECTION_OFFSET);
	m_ctrlContrast.SetTic(0);
	m_ctrlContrast.SetTic(MAX_CORRECTION_OFFSET);
	m_ctrlContrast.SetTic(MAX_CORRECTION_OFFSET*2);
	
	m_strBrightness="0";
	m_strContrast="0";
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBrightContDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	m_strBrightness.Format(_T("%d"), m_ctrlBrightness.GetPos()-MAX_CORRECTION_OFFSET);
	m_strContrast.Format(_T("%d"), m_ctrlContrast.GetPos()-MAX_CORRECTION_OFFSET);	
	UpdateData(FALSE);
	
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBrightContDlg::OnOK() 
{
	// Преобразуем диапазон единиц коррекции к половине диапазона яркости
	m_iBrightnessOffset=(m_ctrlBrightness.GetPos()-MAX_CORRECTION_OFFSET)*
						127/MAX_CORRECTION_OFFSET;
	m_iContrastOffset=(m_ctrlContrast.GetPos()-MAX_CORRECTION_OFFSET)*
						127/MAX_CORRECTION_OFFSET;
	CDialog::OnOK();
}

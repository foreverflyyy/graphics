// DeNoiseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BMViewer.h"
#include "DeNoiseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeNoiseDlg dialog


CDeNoiseDlg::CDeNoiseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDeNoiseDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDeNoiseDlg)
	m_dK = 1.0;
	m_nWhatToDo = 0;
	//}}AFX_DATA_INIT
}


void CDeNoiseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDeNoiseDlg)
	DDX_Control(pDX, IDC_SPIN_K, m_ctrlSpinK);
	DDX_Text(pDX, IDC_K, m_dK);
	DDX_Radio(pDX, IDC_NOISE_SMOOTH, m_nWhatToDo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDeNoiseDlg, CDialog)
	//{{AFX_MSG_MAP(CDeNoiseDlg)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_K, OnChangePosSpinK)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeNoiseDlg message handlers

#define Max_K 10
#define K_Gradation 100
void CDeNoiseDlg::OnChangePosSpinK(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	m_dK=(double)pNMUpDown->iPos*Max_K/K_Gradation;
	UpdateData(FALSE);
	*pResult = 0;
}

BOOL CDeNoiseDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_ctrlSpinK.SetRange(0, K_Gradation);
	m_ctrlSpinK.SetPos(m_dK*K_Gradation/Max_K);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeNoiseDlg::OnOK() 
{
	UpdateData();
	CDialog::OnOK();
}

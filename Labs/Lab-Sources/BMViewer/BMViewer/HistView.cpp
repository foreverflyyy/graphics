// HistView.cpp : implementation file
//

#include "stdafx.h"
#include "BMViewer.h"
#include "HistView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHistView

CHistView::CHistView()
{
	m_pHist=NULL;
	m_iRange=0;
	m_Color=RGB(0,0,0); // black
}

CHistView::~CHistView()
{
}


BEGIN_MESSAGE_MAP(CHistView, CStatic)
	//{{AFX_MSG_MAP(CHistView)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHistView message handlers

void CHistView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here

	if(m_pHist==NULL || m_iRange==0 ) return;
	
	
	// Ќайдем среднее значение
	DWORD MaxBright=0, SumBright=0;
	for(int i=0; i<m_iRange; i++)
		SumBright+=m_pHist[i];
	
	// ѕусть максимальное (показываемое на рисунке) значение
	// будет в три раза больше среднего
	MaxBright=3*SumBright/m_iRange;
	if(MaxBright==0) return;
	
	// ѕеро дл€ рисовани€ гистограммы
	CPen HistPen(PS_SOLID, 2, m_Color);
	CPen *pOldPen=dc.SelectObject(&HistPen);
	CGdiObject *pOldBrush=dc.SelectStockObject(NULL_BRUSH);
	
	CRect FrameRect;
	GetWindowRect(&FrameRect);
	ScreenToClient(&FrameRect);
	
	dc.Rectangle(&FrameRect);
	FrameRect.bottom-=1;
	double kx=((double)FrameRect.Width())/m_iRange;
	double ky=((double)FrameRect.Height())/MaxBright;
	
	int x=0, y=0;
	for(int i=0; i<m_iRange; i++)
	{
		x=FrameRect.left+(kx*i);
		y=FrameRect.bottom;
		dc.MoveTo(x, y);
		y=FrameRect.bottom -(ky*m_pHist[i]);
		if(y<FrameRect.top) y=FrameRect.top;
		dc.LineTo(x, y);
	}
	if(pOldPen) 
		dc.SelectObject(pOldPen);
	if(pOldBrush)
		dc.SelectObject(pOldBrush);
	
	// Do not call CStatic::OnPaint() for painting messages
}

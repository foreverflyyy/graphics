// Bezievw.cpp : implementation of the CBezierView class
//

#include "stdafx.h"
#include "Bezier.h"

#include "BezierDoc.h"
#include "BezierView.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBezierView

IMPLEMENT_DYNCREATE(CBezierView, CView)

BEGIN_MESSAGE_MAP(CBezierView, CView)
	//{{AFX_MSG_MAP(CBezierView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBezierView construction/destruction

CBezierView::CBezierView()
{
	// TODO: add construction code here

}

CBezierView::~CBezierView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CBezierView drawing

void CBezierView::OnDraw(CDC* pDC)
{
	CBezierDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	DrawBaseLine(pDC);
	DrawSplineLine(pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CBezierView printing

BOOL CBezierView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBezierView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBezierView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBezierView diagnostics

#ifdef _DEBUG
void CBezierView::AssertValid() const
{
	CView::AssertValid();
}

void CBezierView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBezierDoc* CBezierView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBezierDoc)));
	return (CBezierDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBezierView message handlers

void CBezierView::DrawBaseLine(CDC *pDC)
{
	CBezierDoc* pDoc = GetDocument();

	int nCount=pDoc->m_BasePointsArray.GetSize();
	
	// Рисуем точки
	for(int i=0; i<nCount; i++)
		pDC->Ellipse(	pDoc->m_BasePointsArray[i].x-4, pDoc->m_BasePointsArray[i].y-4,
						pDoc->m_BasePointsArray[i].x+4, pDoc->m_BasePointsArray[i].y+4);
	
	// Перо для рисования базовой кривой
	CPen PenBase; PenBase.CreatePen(PS_DOT, 1, RGB(0,0,255));
	CPen *pPenOld=pDC->SelectObject(&PenBase);
	
	// Соединим прямыми базовые точки
	pDC->Polyline(pDoc->m_BasePointsArray.GetData(), nCount);
	
	// Восстановим старое перо
	pDC->SelectObject(pPenOld);
	
};

void CBezierView::DrawSplineLine(CDC *pDC)
{
	CBezierDoc* pDoc = GetDocument();
	int nCount=pDoc->m_SplinePointsArray.GetSize();

	// Сплайн рисуем красным пером
	CPen PenSpline; PenSpline.CreatePen(PS_SOLID, 2, RGB(255,0,0));
	CPen *pPenOld=pDC->SelectObject(&PenSpline);
	if(pDoc->m_nSplineType==BEZIER)
	{
		// Рисуем кривую Безье методом класса CDC::PolyBezier
		pDC->PolyBezier(pDoc->m_SplinePointsArray.GetData(), nCount/3*3+1);
		// Покажем точки стыковки сегментов
		for(int i=3; i<nCount; i+=3)
		pDC->Ellipse(	pDoc->m_SplinePointsArray[i].x-4, pDoc->m_SplinePointsArray[i].y-4,
						pDoc->m_SplinePointsArray[i].x+4, pDoc->m_SplinePointsArray[i].y+4);

	}
	else
		// Cоединим прямыми базовые точки
		pDC->Polyline(pDoc->m_SplinePointsArray.GetData(), nCount);
	// Восстановим старое перо
	pDC->SelectObject(pPenOld);
}

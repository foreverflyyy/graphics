// BMView.cpp : implementation of the CBMView class
//

#include "stdafx.h"
#include "BMViewer.h"

#include "BMViewerDoc.h"
#include "BMViewerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBMView

IMPLEMENT_DYNCREATE(CBMView, CScrollView)

BEGIN_MESSAGE_MAP(CBMView, CScrollView)
	ON_MESSAGE(UM_STARTTRANSFORM, OnStartTransform)
	ON_MESSAGE(UM_ENDOFTRANSFORM, OnEndTransform)
	//{{AFX_MSG_MAP(CBMView)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_ZOOMIN, OnViewZoomin)
	ON_COMMAND(ID_VIEW_ZOOMOUT, OnViewZoomout)
	ON_COMMAND(ID_VIEW_STRETCHHALFTONE, OnViewStretchhalftone)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STRETCHHALFTONE, OnUpdateViewStretchhalftone)
	ON_COMMAND(ID_VIEW_STRETCHBLACKONWHITE, OnViewStretchblackonwhite)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STRETCHBLACKONWHITE, OnUpdateViewStretchblackonwhite)
	ON_COMMAND(ID_VIEW_STRETCHWHITEONBLACK, OnViewStretchwhiteonblack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STRETCHWHITEONBLACK, OnUpdateViewStretchwhiteonblack)
	ON_COMMAND(ID_VIEW_STRETCHCOLORONCOLOR, OnViewStretchcoloroncolor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STRETCHCOLORONCOLOR, OnUpdateViewStretchcoloroncolor)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMView construction/destruction

CBMView::CBMView()
{
	m_dScale=1.0;
	m_nStretchMode=HALFTONE;
}

CBMView::~CBMView()
{
}

BOOL CBMView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBMView drawing

// Цвет для заливки фона 
#define GRAY RGB(127, 127, 127)
BOOL CBMView::UpdateVirtualScreen()
{
	CBMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// Получили указатель на активную картинку
	CRaster* pCurBM=pDoc->GetCurrentBMPtr(); 
	if(pCurBM==NULL) return FALSE;
	
	// Вычисляем размеры картинки с учетом масштаба
	LONG imgw=static_cast<LONG>(pCurBM->GetBMWidth()*m_dScale);
	LONG imgh=static_cast<LONG>(pCurBM->GetBMHeight()*m_dScale);
	
	// Если битмап уже существует, возьмем ее размер
	BITMAP BMStruct; BMStruct.bmWidth=BMStruct.bmHeight=0;
	if(m_VirtScreenBitmap.GetSafeHandle( ))
		m_VirtScreenBitmap.GetBitmap(&BMStruct);

	// Если размеры виртуального экрана меньше размеров картинки
	// увеличим экран
	if(	BMStruct.bmWidth<imgw || BMStruct.bmHeight<imgh)
	{
		CPaintDC dc(this); 
		// Размеры дисплея в пикселах 
		int scrw=dc.GetDeviceCaps(HORZRES);
		int scrh=dc.GetDeviceCaps(VERTRES);
		
		// Выберем временную битмап в контексте
		// это освободит m_VirtScreenBitmap,
		// (если она была ранее выбрана в контексте)
		// и даст возможность удалить ее 
		CBitmap TempBM; TempBM.CreateCompatibleBitmap(&dc,1,1);
		m_VirtScreenDC.SelectObject(&TempBM);

		// Разрушим ранее существовавшую битмап
		m_VirtScreenBitmap.DeleteObject();
				
		// и на ее месте построим новую по размерам изображения, не меньше размеров дисплея
		if(!m_VirtScreenBitmap.CreateCompatibleBitmap(&dc,	(imgw<scrw?scrw:imgw),
															(imgh<scrh?scrh:imgh)))
			return FALSE;
		// Новую битмап выберем в контексте виртуального экрана
		m_VirtScreenDC.SelectObject(&m_VirtScreenBitmap);
	}
	
	// Очистим виртуальный экран
	CBrush FonBrush(GRAY); // кисть для заливки фона
	m_VirtScreenBitmap.GetBitmap(&BMStruct); // узнаем размеры экрана
	m_VirtScreenDC.FillRect(&CRect(0,0, BMStruct.bmWidth, BMStruct.bmHeight), &FonBrush);

	// Выведем на виртуальный экран картинку
	pCurBM->DrawBitmap(&m_VirtScreenDC, 0, 0, m_dScale, m_nStretchMode);
		
	// Обновим изображение на экране
	Invalidate();
	return TRUE;
};

void CBMView::OnDraw(CDC* pDC) 
{
	// TODO: Add your specialized code here and/or call the base class
	CBMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// Получим размер клиентской части окна
	CRect ClientRect;
	GetClientRect(&ClientRect);

	// Копируем содержимое виртуального экрана
	// с учетом позиции прокрутки
	CPoint ScrollPos=GetScrollPosition();

	pDC->BitBlt(ScrollPos.x, ScrollPos.y, ClientRect.Width(), ClientRect.Height(),
		&m_VirtScreenDC, ScrollPos.x, ScrollPos.y, SRCCOPY);
}

void CBMView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	
//	CSize sizeTotal;
//	// TODO: calculate the total size of this view
//	sizeTotal.cx = sizeTotal.cy = 100;
//	SetScrollSizes(MM_TEXT, sizeTotal);
}

/////////////////////////////////////////////////////////////////////////////
// CBMView printing

BOOL CBMView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBMView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBMView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBMView diagnostics

#ifdef _DEBUG
void CBMView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CBMView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CBMDoc* CBMView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBMDoc)));
	return (CBMDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBMView message handlers

int CBMView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	// Создадим совместимый контекст для виртуального экрана
	CPaintDC dc(this); 
	m_VirtScreenDC.CreateCompatibleDC(&dc);

	return 0;
}

void CBMView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CBMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	// обновим изображение на виртуальном экране
	if(UpdateVirtualScreen())
	{
		// Размер области прокрутки  
		CSize ScrollSize;
		
		// Область прокрутки - весь размер картинки с учетом масштаба
		ScrollSize=pDoc->GetCurrentBMSize();
		ScrollSize.cx=static_cast<LONG>(ScrollSize.cx*m_dScale);
		ScrollSize.cy=static_cast<LONG>(ScrollSize.cy*m_dScale);
		SetScrollSizes(MM_TEXT, ScrollSize);
	}
	else 
		AfxMessageBox((LPCTSTR)"Ошибка при выводе на виртуальный экран");

	// Вызываем метод базового класса
	CScrollView::OnUpdate(pSender, lHint, pHint); 
}

void CBMView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// Разрешение устройства	 
	int DXRes=pDC->GetDeviceCaps(LOGPIXELSX);
	int DYRes=pDC->GetDeviceCaps(LOGPIXELSY);

   	CBMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CRaster* pCurPic=pDoc->GetCurrentBMPtr(); 
	if(pCurPic==NULL) return;
	
	LONG  height, width;
	double resx, resy; 
	// Перерасчет размера изображения под разрешение принтера
   	width=pCurPic->GetBMWidth();
	height=pCurPic->GetBMHeight();

	resx=((double)pCurPic->GetBMInfoPtr()->bmiHeader.biXPelsPerMeter); 
	resy=((double)pCurPic->GetBMInfoPtr()->bmiHeader.biYPelsPerMeter);
	// Переводим в dpi
	resx*=(25.4/1000);
	resy*=(25.4/1000);
	// Приведем к разумным пределам
	if(resx<=0 || resx> 3000) resx=72;
	if(resy<=0 || resy> 3000) resy=72;

	width=(int)(((double)width)*DXRes/resx+0.5);
	height=(int)(((double)height)*DYRes/resy+0.5);

	// Воводим изображение, соответственно новым размерам
	pCurPic->DrawBitmap( pDC, 0, 0, width, height);
	
};


BOOL CBMView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//	Уберем обработчик по умолчанию, чтобы предотвратить мерцание
	//	return CScrollView::OnEraseBkgnd(pDC);
	//  UpdateVirtualScreen();
	return TRUE;
}

LONG CBMView::OnStartTransform(UINT wParam, LONG lParam)
{
	OnStartTimer();
	return 0;
}


LONG CBMView::OnEndTransform(UINT wParam, LONG lParam)
{
	OnStopTimer();
	if(wParam)
		// обновим изображение на виртуальном экране
		UpdateVirtualScreen();
	
	return 0;
}

void CBMView::OnStartTimer() 
{
	CFrameWnd* pFrame=GetParentFrame();
	pFrame->GetWindowText(m_strOrgTitle);
	
	m_nTimer = SetTimer(1, 100, 0);
}

void CBMView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CBMDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	CFrameWnd* pFrame=GetParentFrame();
	
	LONG exproc=0;
	InterlockedExchange(&exproc, pDoc->m_lExecutedPercent);
	
	CString title;
	title.Format(_T("%s : %d "), m_strOrgTitle, exproc);
	pFrame->SetWindowText(title);
	
	CScrollView::OnTimer(nIDEvent);
}

void CBMView::OnStopTimer() 
{
	CFrameWnd* pFrame=GetParentFrame();
	pFrame->SetWindowText(m_strOrgTitle);
	
	KillTimer(m_nTimer);   
}


void CBMView::OnViewZoomin() 
{
	// TODO: Add your command handler code here
	m_dScale*=2;
	OnUpdate(NULL, 0, NULL);  
	
}

void CBMView::OnViewZoomout() 
{
	// TODO: Add your command handler code here
	m_dScale/=2;
	OnUpdate(NULL, 0, NULL);        
	
}

void CBMView::OnViewStretchhalftone() 
{
	// TODO: Add your command handler code here
	m_nStretchMode=HALFTONE;
	OnUpdate(NULL, 0, NULL); 
}

void CBMView::OnUpdateViewStretchhalftone(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nStretchMode==HALFTONE);
}

void CBMView::OnViewStretchcoloroncolor() 
{
	// TODO: Add your command handler code here
	m_nStretchMode=COLORONCOLOR;
	OnUpdate(NULL, 0, NULL); 
}

void CBMView::OnUpdateViewStretchcoloroncolor(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nStretchMode==COLORONCOLOR);	
}

void CBMView::OnViewStretchblackonwhite() 
{
	// TODO: Add your command handler code here
	m_nStretchMode=BLACKONWHITE;
	OnUpdate(NULL, 0, NULL); 
}

void CBMView::OnUpdateViewStretchblackonwhite(CCmdUI* pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nStretchMode==BLACKONWHITE);
}

void CBMView::OnViewStretchwhiteonblack() 
{
	// TODO: Add your command handler code here
	m_nStretchMode=WHITEONBLACK;
	OnUpdate(NULL, 0, NULL); 
}

void CBMView::OnUpdateViewStretchwhiteonblack(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(m_nStretchMode==WHITEONBLACK);
}

// BMDoc.cpp : implementation of the CBMDoc class
//

#include "stdafx.h"
#include "BMViewer.h"

#include "BMViewerDoc.h"
#include "HistDlg.h"
#include "BrightContDlg.h"
#include "DeNoiseDlg.h"
#include "BMViewerView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBMDoc

IMPLEMENT_DYNCREATE(CBMDoc, CDocument)

BEGIN_MESSAGE_MAP(CBMDoc, CDocument)
	//{{AFX_MSG_MAP(CBMDoc)
	ON_COMMAND(ID_EDIT_HISTOGRAM, OnEditHistogram)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_BRIGHTNESSANDCONTRAST, OnEditBrightnessandcontrast)
	ON_COMMAND(ID_EDIT_INVERTCOLORS, OnEditInvertcolors)
	ON_COMMAND(ID_EDIT_BLUR, OnEditBlur)
	ON_COMMAND(ID_EDIT_SHARP, OnEditSharp)
	ON_COMMAND(ID_EDIT_CONTOUR, OnEditContour)
	ON_COMMAND(ID_EDIT_HALF, OnEditHalf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_HALF, OnUpdateEditHalf)
	ON_COMMAND(ID_EDIT_EMBOSS, OnEditEmboss)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BLUR, OnUpdateEditBlur)
	ON_UPDATE_COMMAND_UI(ID_EDIT_HISTOGRAM, OnUpdateEditHistogram)
	ON_UPDATE_COMMAND_UI(ID_EDIT_EMBOSS, OnUpdateEditEmboss)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SHARP, OnUpdateEditSharp)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CONTOUR, OnUpdateEditContour)
	ON_UPDATE_COMMAND_UI(ID_EDIT_BRIGHTNESSANDCONTRAST, OnUpdateEditBrightnessandcontrast)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INVERTCOLORS, OnUpdateEditInvertcolors)
	ON_COMMAND(ID_EDIT_DENOISE, OnEditDenoise)
	ON_COMMAND(ID_EDIT_STOP, OnEditStop)
	ON_UPDATE_COMMAND_UI(ID_EDIT_STOP, OnUpdateEditStop)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DENOISE, OnUpdateEditDenoise)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBMDoc construction/destruction

CBMDoc::CBMDoc():m_EventDoTransform(FALSE, TRUE) 
{
	// TODO: add one-time construction code here
	m_pCurBM=NULL;
	m_bEditable=FALSE;
	m_bEditHalf=FALSE;
	m_pCurFilter=NULL;

	//m_pEventDoTransform=new CEvent(FALSE, TRUE);
}

CBMDoc::~CBMDoc()
{
//	if(m_pEventDoTransform) delete m_pEventDoTransform;
}

BOOL CBMDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBMDoc serialization

void CBMDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBMDoc diagnostics

#ifdef _DEBUG
void CBMDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBMDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG
/////////////////////////////////////////////////////////////////////////////
// CBMDoc operations

CSize CBMDoc::GetCurrentBMSize()
{
	CSize sz;
	if(m_pCurBM!=NULL)
	{
		sz.cx=m_pCurBM->GetBMWidth();
		sz.cy=m_pCurBM->GetBMHeight();
	}
	return sz;
};
int	CBMDoc::GetNCurrentBM()
{
	if(m_pCurBM==NULL || m_pCurBM==&m_BM[0]) return 0;
	else return 1;
};

CRaster* CBMDoc::GetCurrentBMPtr()
{
	return m_pCurBM;
}; 

CRaster* CBMDoc::GetBufferBMPtr()
{
	return &m_BM[1-GetNCurrentBM()];
};

void CBMDoc::SwapBM()
{
	m_pCurBM=GetBufferBMPtr();
};

BOOL CBMDoc::CreateCompatibleBuffer(LONG width/*=0*/, LONG height/*=0*/)
{
	if(!m_pCurBM) return FALSE;
	CRaster *pBuff=GetBufferBMPtr();
	if(!pBuff) return FALSE;
	return	pBuff->CreateCompatible(m_pCurBM->GetBMInfoPtr(), width, height);
};

void CBMDoc::Transform()
{
	AfxBeginThread(ThreadProc, this);
}

UINT CBMDoc::ThreadProc(LPVOID pv)
{
	CBMDoc* pDoc = (CBMDoc*)pv;

    if (pDoc == NULL ||
        !pDoc->IsKindOf(RUNTIME_CLASS(CBMDoc)))
    return 1;   // Не правильный параметр

	pDoc->TransformLoop();

	return 0;
};

void CBMDoc::TransformLoop()
{
	if(m_pCurFilter==NULL) return;
	if(!CreateCompatibleBuffer()) return;

	m_EventDoTransform.SetEvent();
	m_bEditable=FALSE;
	InformAllViews(UM_STARTTRANSFORM);
	
	CRaster	*pSBM=GetCurrentBMPtr(),	// источник
			*pDBM=GetBufferBMPtr();		// приемник 
	// Установили в фильтр источник и приемник преобразований
	m_pCurFilter->SetBuffers(pSBM, pDBM);

	for(LONG y=0; y<pSBM->GetBMHeight(); y++)
	{
		// Процент выполнения
		InterlockedExchange(&m_lExecutedPercent, 100*y/pSBM->GetBMHeight());
		// Проверим не решил ли прервать преобразование
		if(!m_EventDoTransform.Lock(0))
		{
			InformAllViews(UM_ENDOFTRANSFORM, FALSE, 0);
			m_bEditable=TRUE;
			return;
		}
		LONG x=0;
		if( m_bEditHalf ) // Преобразовать только половину изображения
		{
			// Первую половину картинки копируем в буфер без преобразования
			x=pSBM->GetBMWidth()/2;
			BYTE *pSPix=NULL,  *pDPix=NULL;
			// Указатели на начало строк
			if(	(pSPix=pSBM->GetPixPtr(0, y))!=NULL && 
				(pDPix=pDBM->GetPixPtr(0, y))!=NULL)
			// ВНИМАНИЕ! Предполагается, что 1 пиксел = 24 бита = 3 байта
			memcpy(pDPix, pSPix, 3*x); 
		}
		// Преобразование с использованием текущего фильтра
		for(; x<pSBM->GetBMWidth(); x++)
			m_pCurFilter->TransformPix(x, y);
			
	}
	m_EventDoTransform.ResetEvent();
	m_bEditable=TRUE;
	
	SwapBM();	// сделать буфер текущим изображением
	SetModifiedFlag(); // флаг "данные изменились"

	InformAllViews(UM_ENDOFTRANSFORM, TRUE, 0);
	return;
};

void CBMDoc::InformAllViews(UINT msg, WPARAM wPar/*=0*/, LPARAM lPar/*=0*/)
{
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CBMView* pView = (CBMView*)GetNextView(pos);
		PostMessage( pView->m_hWnd, msg, wPar, lPar);
	}   
}

void CBMDoc::DrawCurrent()
{
	// вывести изображение на виртуальный экран всех обликов
	POSITION pos = GetFirstViewPosition();
	while (pos != NULL)
	{
		CBMView* pView = (CBMView*)GetNextView(pos);
		pView->UpdateVirtualScreen();
		
	}   
}


/////////////////////////////////////////////////////////////////////////////
// CBMDoc commands

BOOL CBMDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// Загружаем в первый буфер
	if(m_BM[0].LoadBMP(lpszPathName))
	{
		m_pCurBM=&m_BM[0];
		//Умеем редактировать только RGB888 данные
		if(m_pCurBM->GetBMInfoPtr()->bmiHeader.biBitCount!=24)
			m_bEditable=FALSE;
		else
			m_bEditable=TRUE;
			
		return TRUE;
	}

	return FALSE;
}


BOOL CBMDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	if( m_pCurBM!=NULL && m_pCurBM->SaveBMP(lpszPathName))
	{
		SetModifiedFlag(FALSE);
		return TRUE;
	}
	else 
		return FALSE;
}

void CBMDoc::OnEditUndo() 
{
	SwapBM();			// сделать буфер текущим изображением
	DrawCurrent();		// нарисовать 
	SetModifiedFlag();	// флаг "данные изменились"
}

void CBMDoc::OnEditHistogram() 
{
	const int Range=256;
	DWORD Hist[Range]; // гистограмма из Range градаций яркости

	// Запросим гистограмму у текущего изображения
	if(m_pCurBM==NULL || !m_pCurBM->GetHistogham(Hist, Range))
		return;
	// Создаем объект-диалог
	CHistDlg HDlg; 
	// Передадим гистограмму в диалог
	HDlg.SetData(Hist, Range);
	// Покажем гистограмму
	if(HDlg.DoModal()==IDCANCEL) return;
	// Требуется выполнить коррекцию контрастности	
	if(HDlg.m_iOffset_b !=0 || HDlg.m_iOffset_t!=NULL) 
	{
		// Настраиваем фильтр гистограммы
		m_HistogramFilter.Init(HDlg.m_iOffset_b, HDlg.m_iOffset_t);
		// Делаем фильтр активным 
		m_pCurFilter=&m_HistogramFilter;
		// Выполняем преобразование
		Transform();
	}
}

void CBMDoc::OnEditBrightnessandcontrast() 
{

	CBrightContDlg BCDlg;
	if(BCDlg.DoModal()==IDCANCEL) return;

	if(BCDlg.m_iBrightnessOffset!=0 || BCDlg.m_iContrastOffset!=0) 
	{
		m_BrightContFilter.Init(BCDlg.m_iBrightnessOffset,
								BCDlg.m_iContrastOffset);
		m_pCurFilter=&m_BrightContFilter;
		Transform();
	}
}

void CBMDoc::OnEditInvertcolors() 
{
	m_pCurFilter=&m_InvertColorsFilter;
	Transform();
}

void CBMDoc::OnEditEmboss() 
{
	m_pCurFilter=&m_EmbossFilter;
	Transform();
}
void CBMDoc::OnEditBlur() 
{
	m_pCurFilter=&m_BlurFilter;
	Transform();
}

void CBMDoc::OnEditContour() 
{
	m_pCurFilter=&m_ContourFilter;
	Transform();
}

void CBMDoc::OnEditSharp() 
{
	m_pCurFilter=&m_SharpFilter;
	Transform();
}

void CBMDoc::OnEditDenoise() 
{
	static double k=m_DenoiseFilter.m_dK;
	static int what_to_do=0;
	CDeNoiseDlg DNDlg; 
	DNDlg.m_dK=k;
	DNDlg.m_nWhatToDo=what_to_do;
	
	if(DNDlg.DoModal()!=IDOK) return;
	k=m_DenoiseFilter.m_dK=DNDlg.m_dK;
	what_to_do=m_DenoiseFilter.m_nWhatToDo=DNDlg.m_nWhatToDo;

	m_pCurFilter=&m_DenoiseFilter;

	Transform();
}


void CBMDoc::OnEditHalf() 
{
	m_bEditHalf=!m_bEditHalf;
}

void CBMDoc::OnUpdateEditHalf(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bEditHalf);
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(IsModified());
}

void CBMDoc::OnUpdateEditBlur(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnUpdateEditHistogram(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnUpdateEditEmboss(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnUpdateEditSharp(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnUpdateEditContour(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnUpdateEditBrightnessandcontrast(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnUpdateEditInvertcolors(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnUpdateEditDenoise(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bEditable);
}

void CBMDoc::OnEditStop() 
{
	m_EventDoTransform.ResetEvent();
}

void CBMDoc::OnUpdateEditStop(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_EventDoTransform.Lock(0));	
}


// Beziedoc.cpp : implementation of the CBezierDoc class
//

#include "stdafx.h"
#include "Bezier.h"
#include <time.h>


#include "BezierDoc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBezierDoc

IMPLEMENT_DYNCREATE(CBezierDoc, CDocument)

BEGIN_MESSAGE_MAP(CBezierDoc, CDocument)
	//{{AFX_MSG_MAP(CBezierDoc)
	ON_COMMAND(ID_EDIT_CREATE_BEZIER_0, OnEditCreateBezier0)
	ON_COMMAND(ID_EDIT_CREATE_BEZIER_1, OnEditCreateBezier1)
	ON_COMMAND(ID_EDIT_CREATE_BEZIER_2, OnEditCreateBezier2)
	ON_COMMAND(ID_EDIT_GENERATEBASELINE, OnEditGeneratebaseline)
	ON_COMMAND(ID_EDIT_CTEATECATMULLROM, OnEditCteatecatmullrom)
	ON_COMMAND(ID_EDIT_CERATEBETA, OnEditCeratebeta)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBezierDoc construction/destruction

CBezierDoc::CBezierDoc()
{
	m_nSplineType=ANYSLPINE;
}

CBezierDoc::~CBezierDoc()
{
	m_BasePointsArray.RemoveAll();
	m_SplinePointsArray.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CBezierDoc serialization

void CBezierDoc::Serialize(CArchive& ar)
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
// CBezierDoc diagnostics

#ifdef _DEBUG
void CBezierDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBezierDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBezierDoc commands

void CBezierDoc::GenerateBasePoints()
{
	// Выделим место под базовые точки
	// NBASEPOINTS определено в Beziedoc.h
	m_BasePointsArray.SetSize(NBASEPOINTS); 
	// Зададим значения базовых точек случайным образом 
	// в пределах клиентского окна программы
	CRect ClientRect;
	POSITION pos = GetFirstViewPosition();
	CView* pView =NULL;
	if(pos != NULL)
	   pView = GetNextView(pos);
	if(pView!=NULL)
		pView->GetClientRect(ClientRect);
    else 
		return;
	// Реинициализация генератора случайных чисел
	srand( (unsigned)time( NULL ) ); 
	int step=ClientRect.Width()/(m_BasePointsArray.GetSize()+1);
	// Задаем значения
	for(int i=0; i<m_BasePointsArray.GetSize(); i++)	 
	{	
		m_BasePointsArray[i].x=step/2+step*i+rand()*step/RAND_MAX;
		m_BasePointsArray[i].y=rand()*ClientRect.Height()/RAND_MAX;
	}
	//Удаляем старый сплайн 
	m_SplinePointsArray.RemoveAll();
};


void CBezierDoc::CreateBezier(int nMode/*=0*/)
{
	// Копируем базовые точки
	m_SplinePointsArray.SetSize(m_BasePointsArray.GetSize());
	for(int i=0; i<m_BasePointsArray.GetSize(); i++)
		m_SplinePointsArray[i]=m_BasePointsArray[i];
	
	switch (nMode)
	{
	case 1:
		// Дополняем массив
		for(int i=2; i<m_SplinePointsArray.GetSize()-1; i+=3)
			m_SplinePointsArray.InsertAt(i+1, GetMiddle(&m_SplinePointsArray[i], &m_SplinePointsArray[i+1]));
	break;	
	case 2:		
		// Меняем позицию точек в местах стыковки сегментов
		for(int i=2; i<m_SplinePointsArray.GetSize()-2; i+=3)
			m_SplinePointsArray[i+1]=GetMiddle(&m_SplinePointsArray[i], &m_SplinePointsArray[i+2]);
	break;
	}
	m_nSplineType=BEZIER;
};

void CBezierDoc::CreateCatmullRom()
{
	if(m_BasePointsArray.GetSize()<4) return;
	
	// Добавляем воображаемые базовые точки
	m_BasePointsArray.InsertAt(0, m_BasePointsArray[0]);
	m_BasePointsArray.Add(m_BasePointsArray[m_BasePointsArray.GetUpperBound()]);

	m_SplinePointsArray.SetSize((m_BasePointsArray.GetSize()-3)*NAPPROXCUTS+1);
	// Получим прямой доступ к данным массива базовых точек
	CPoint *pBasePoint=m_BasePointsArray.GetData();
	double	t=0., // параметр t
			dt=1.0/NAPPROXCUTS; // шаг приращения параметра t
	int n=0,		//локальный номер точки внутри сегмента
		nSegment=0; //номер первой точки текущего сегмента
	for(int i=1; i<m_BasePointsArray.GetSize()-2; i++)
	{
		t=0.;
		for(n=0; n<=NAPPROXCUTS; n++, t+=dt)
			m_SplinePointsArray[nSegment+n]=GetCatmullRomPoint(pBasePoint+(i-1), t);
		nSegment+=NAPPROXCUTS;
	}
	
	// Удаляем воображаемые базовые точки
	m_BasePointsArray.RemoveAt(0);
	m_BasePointsArray.RemoveAt(m_BasePointsArray.GetUpperBound());
	
	m_nSplineType=ANYSLPINE;
};


void CBezierDoc::CreateBeta()
{
	if(m_BasePointsArray.GetSize()<4) return;
	double	Beta1=1.0,
			Beta2=0.0;
	// Добавляем воображаемые базовые точки
	m_BasePointsArray.InsertAt(0, m_BasePointsArray[0]);
	m_BasePointsArray.InsertAt(0, m_BasePointsArray[0]);

	m_BasePointsArray.Add(m_BasePointsArray[m_BasePointsArray.GetUpperBound()]);
	m_BasePointsArray.Add(m_BasePointsArray[m_BasePointsArray.GetUpperBound()]);

	m_SplinePointsArray.SetSize((m_BasePointsArray.GetSize()-3)*NAPPROXCUTS+1);
	// Получим прямой доступ к данным массива базовых точек
	CPoint *pBasePoint=m_BasePointsArray.GetData();
	double	t=0., // параметр t
			dt=1.0/NAPPROXCUTS; // шаг приращения параметра t
	int n=0,		//локальный номер точки внутри сегмента
		nSegment=0; //номер первой точки текущего сегмента
	for(int i=1; i<m_BasePointsArray.GetSize()-2; i++)
	{
		t=0.;
		for(n=0; n<=NAPPROXCUTS; n++, t+=dt)
			m_SplinePointsArray[nSegment+n]=GetBetaPoint(pBasePoint+(i-1), t, Beta1, Beta2);
		nSegment+=NAPPROXCUTS;
	}
	
	// Удаляем воображаемые базовые точки
	m_BasePointsArray.RemoveAt(0,2);
	m_BasePointsArray.RemoveAt(m_BasePointsArray.GetUpperBound()-1,2);
		
	m_nSplineType=ANYSLPINE;
};

void CBezierDoc::OnEditGeneratebaseline() 
{
	GenerateBasePoints();
	UpdateAllViews(NULL);
}

void CBezierDoc::OnEditCreateBezier0() 
{
	CreateBezier(0);
	UpdateAllViews(NULL);
}

void CBezierDoc::OnEditCreateBezier1() 
{
	CreateBezier(1);	
	UpdateAllViews(NULL);
}

void CBezierDoc::OnEditCreateBezier2() 
{
	CreateBezier(2);	
	UpdateAllViews(NULL);
}

void CBezierDoc::OnEditCteatecatmullrom() 
{
	CreateCatmullRom();
	UpdateAllViews(NULL);
}
void CBezierDoc::OnEditCeratebeta() 
{
	CreateBeta();
	UpdateAllViews(NULL);
}

/////////////////////////////////////////////////////////////////////////
//Глобальная функция. Возвращает средину отрезка
CPoint GetMiddle(CPoint *pP1, CPoint *pP2)
{
	return CPoint(pP1->x+(pP2->x-pP1->x)/2,	pP1->y+(pP2->y-pP1->y)/2);
}

// Возвращает точку Catmull-Rom сплайна для параметра t=(0...1)
// на участке между точками 1 и 2 четверки точек 
CPoint GetCatmullRomPoint(CPoint *pSegment, double t)
{
	double s=1.0-t, t2=t*t, t3=t2*t;
	CPoint Res;
	Res.x=(LONG)(0.5*(-t*s*s*pSegment[0].x+(2-5*t2+3*t3)*pSegment[1].x+t*(1+4*t-3*t2)*pSegment[2].x-t2*s*pSegment[3].x)+0.5);
	Res.y=(LONG)(0.5*(-t*s*s*pSegment[0].y+(2-5*t2+3*t3)*pSegment[1].y+t*(1+4*t-3*t2)*pSegment[2].y-t2*s*pSegment[3].y)+0.5);
	return Res;
};

CPoint GetBetaPoint(CPoint *pSegment, double t, double beta1, double beta2)
{	
	double	s=1.0-t,
			t2=t*t,
			t3=t2*t,
			b12=beta1*beta1,
			b13=b12*beta1,
			delta=2.0*b13+4.0*b12+4.0*beta1+beta2+2.0,
			d=1.0/delta,
			b0=2*b13*d*s*s*s,
			b3=2*t3*d,
			b1=d*(2*b13*t*(t2-3*t+3)+2*b12*(t3-3*t2+2)+
				2*beta1*(t3-3*t+2)+beta2*(2*t3-3*t2+1)),
			b2=d*(2*b12*t2*(-t+3)+2*beta1*t*(-t2+3)+
				beta2*t2*(-2*t+3)+2*(-t3+1) );
	CPoint Res;
	Res.x=(LONG)(b0*pSegment[0].x+b1*pSegment[1].x+b2*pSegment[2].x+b3*pSegment[3].x +0.5);
	Res.y=(LONG)(b0*pSegment[0].y+b1*pSegment[1].y+b2*pSegment[2].y+b3*pSegment[3].y +0.5);
	return Res;
};



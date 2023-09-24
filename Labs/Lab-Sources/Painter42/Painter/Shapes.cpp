// ôàéë Shapes.cpp
////////////////////////////////////////
//ðåàëèçàöèÿ êëàññîâ
#include "stdafx.h"
#include "shapes.h"
#include "global.h"
#include <math.h>

////////////////////////////////////////
// Ðåàëèçàöèÿ ìåòîäîâ êëàññà CBasePoint
CBasePoint::CBasePoint(): CPoint(0, 0)
{
	m_wSize=1;
	
	m_iPenStyle=PS_SOLID;
	m_iPenWidth=1;
	m_rgbPenColor=RGB(0,0,0);
	m_iBrushStyle=-1; // íå èñïîëüçóåì øòðèõîâêó
	m_rgbBrushColor=RGB(0,0,0);
	m_dwPattern_ID=0; // íåò øàáëîíà çàëèâêè
};

CBasePoint::CBasePoint(int x, int y, WORD s):CPoint(x, y)
{
	m_wSize=s;

	m_iPenStyle=PS_SOLID;
	m_iPenWidth=1;
	m_rgbPenColor=RGB(0,0,0);
	m_iBrushStyle=-1; // íå èñïîëüçóåì øòðèõîâêó
	m_rgbBrushColor=RGB(0,0,0);
	m_dwPattern_ID=0; // íåò øàáëîíà çàëèâêè
};

IMPLEMENT_SERIAL(CBasePoint, CObject , VERSIONABLE_SCHEMA|1)
void CBasePoint::Serialize(CArchive &ar)
{
	if(ar.IsStoring()) // ñîõðàíåíèå
	{
		// Ñîõðàíÿåì ïàðàìåòðû îáúåêòà
		ar<<x;
		ar<<y;
		ar<<m_wSize;
		ar<<m_iPenStyle;
		ar<<m_iPenWidth;
		ar<<m_rgbPenColor;
		ar<<m_iBrushStyle;
		ar<<m_rgbBrushColor;
		ar<<m_dwPattern_ID;
	}
	else	// ÷òåíèå
	{
		// Ïîëó÷èëè âåðñèþ ôîðìàòà
		int Version=ar.GetObjectSchema();
		// Â çàâèñèìîñòè îò âåðñèè
		// ìîæíî âûïîëíèòü ðàçëè÷íûå âàðèàíòû çàãðóçêè
		// Çàãðóæàåì ïàðàìåòðû îáúåêòà
		ar>>x;
		ar>>y;
		ar>>m_wSize;
		ar>>m_iPenStyle;
		ar>>m_iPenWidth;
		ar>>m_rgbPenColor;
		ar>>m_iBrushStyle;
		ar>>m_rgbBrushColor;
		ar>>m_dwPattern_ID;

		SetPen(m_rgbPenColor, m_iPenWidth, m_iPenStyle);
		SetBrush(m_rgbBrushColor, m_dwPattern_ID, m_iBrushStyle );
	}
};
BOOL CBasePoint::SetPen(COLORREF color, int width /*=1*/, int style/*=PS_SOLID*/)
{
	m_iPenStyle=style;
	m_iPenWidth=width;
	m_rgbPenColor=color;
	if(HPEN(m_Pen)!=NULL)	// åñëè ïåðî óæå ñóùåñòâóåò
		if(!m_Pen.DeleteObject()) return FALSE; // óäàëèëè ñòàðîå ïåðî
	// Ñîçäàåì íîâîå ïåðî è âîçâðàùàåì ðåçóëüòàò
	return m_Pen.CreatePen( m_iPenStyle, m_iPenWidth, m_rgbPenColor);
};
BOOL CBasePoint::SetBrush(COLORREF color, DWORD pattern /*=0*/, int style/*=-1*/)
{
	m_iBrushStyle=style;
	m_dwPattern_ID=pattern;
	m_rgbBrushColor=color;
	int res=1;
	if(HBRUSH(m_Brush)!=NULL)	// åñëè êèñòü óæå ñóùåñòâóåò
		if(!m_Brush.DeleteObject()) return FALSE; // óäàëèëè ñòàðóþ êèñòü
	if(m_dwPattern_ID>0)	// åñòü øàáëîí çàëèâêè
	{
		CBitmap Pattern;
		if(!Pattern.LoadBitmap(m_dwPattern_ID)) return FALSE;
		return m_Brush.CreatePatternBrush(&Pattern);
	}
	if(m_iBrushStyle>=0)	// óêàçàí ñòèëü øòðèõîâêè
		return m_Brush.CreateHatchBrush( m_iBrushStyle, m_rgbBrushColor);
	// Ñîçäàåì ñïëîøíóþ êèñòü è âîçâðàùàåì ðåçóëüòàò
	return m_Brush.CreateSolidBrush(m_rgbBrushColor);
};



BOOL CBasePoint::PrepareDC(CDC *pDC)
{
	// Ñîõðàíÿåì ñîñòîÿíèå êîíòåêñòà óñòðîéñòâà
	if(!pDC->SaveDC()) return FALSE;
	// Óñòàíàâëèâàåì ïåðî è êèñòü 
	if(HPEN(m_Pen)!=NULL)
		pDC->SelectObject(&m_Pen);
	if(HBRUSH(m_Brush)!=NULL)
		pDC->SelectObject(&m_Brush);
	return TRUE;
};

BOOL CBasePoint::RestoreDC(CDC *pDC)
{
	// Âîññòàíàâëèâàåì ñîñòîÿíèå êîíòåêñòà óñòðîéñòâà
	return pDC->RestoreDC(-1);
};

void CBasePoint::Show(CDC* pDC) 
{
	// Óñòàíàâëèâàåì ïåðåî è êèñòü
	PrepareDC(pDC);
	// Ðèñóåì êðóæîê, îáîçíà÷àþùèé òî÷êó
	pDC->Ellipse(x-m_wSize, y-m_wSize, x+m_wSize, y+m_wSize);
	// Âîññòàíàâëèâàåì êîíòåêñò
	RestoreDC(pDC);
} 

void CBasePoint::GetRegion(CRgn &Rgn)
{
	Rgn.CreateEllipticRgn(x-m_wSize, y-m_wSize, x+m_wSize, y+m_wSize);
}

void CBasePoint::Transform(const CPoint &point0, double ang, int a, int b)
{
	CPoint res=::Transform(CPoint(x, y), CPoint(0,0), 0, a, b);
	 x=res.x; y=res.y;
};

BOOL CBasePoint::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, UINT nMyFlags) 
{
	BOOL res=TRUE;
	if(nMyFlags & SHIFT_HOLD) //ïîâîðîò
		switch(nChar)
		{
			case 37: 
				Transform(CPoint(0,0), -ROTATE_STEP, 0, 0);
				break;
			case 39:
				Transform(CPoint(0,0), ROTATE_STEP, 0, 0);
				break;
			default:
				res=FALSE;
		}
	else //ïåðåíîñ
		switch(nChar)
		{
			case 38: // ââåðõ
				Transform(CPoint(0,0), 0, 0, MOVE_STEP);
				break; 
			case 40: // âíèç
				Transform(CPoint(0,0), 0, 0, -MOVE_STEP); 
				break;
			case 37: // âëåâî
				Transform(CPoint(0,0), 0, -MOVE_STEP, 0);
				break;
			case 39: // âïðàâî
				Transform(CPoint(0,0), 0, MOVE_STEP, 0);
				break;
			default:
				res=FALSE;
		}	
	return res;
}

////////////////////////////////////////
// Ðåàëèçàöèÿ ìåòîäîâ êëàññà CSquare

CSquare::CSquare(int x, int y, WORD s): CBasePoint(x, y, s)
{
	m_wSize=s;
} 

CSquare::CSquare(): CBasePoint()
{
	m_wSize=40;
} 

IMPLEMENT_SERIAL(CSquare, CObject, 1)
void CSquare::Serialize(CArchive &ar)
{
	CBasePoint::Serialize(ar);
}
void CSquare::Show(CDC* pDC) 
{
	int s=m_wSize/2;
	// Óñòàíàâëèâàåì ïåðåî è êèñòü
	PrepareDC(pDC);
	// Ðèñóåì êâàäðàò
	pDC->Rectangle(x-s, y-s, x+s, y+s);
	// Âîññòàíàâëèâàåì êîíòåêñò
	RestoreDC(pDC);
} 

void CSquare::GetRegion(CRgn &Rgn)
{
	int s=m_wSize/2;
	Rgn.CreateRectRgn(x-s, y-s, x+s, y+s);
}

////////////////////////////////////////
// Ðåàëèçàöèÿ ìåòîäîâ êëàññà CMyFigure

CMyFigure::CMyFigure(int x, int y, WORD s) : CBasePoint(x, y, s)
{
	m_wSize = s;
}

CMyFigure::CMyFigure() : CBasePoint()
{
	m_wSize = 40;
}

IMPLEMENT_SERIAL(CMyFigure, CObject, 1)
void CMyFigure::Serialize(CArchive& ar)
{
	CBasePoint::Serialize(ar);
}
void CMyFigure::Show(CDC* pDC)
{
	CRect rect_1(x - m_wSize, y - m_wSize, x + m_wSize, y + m_wSize);
	CRect rect_2(x - m_wSize * 0.7, y - m_wSize * 0.7, x + m_wSize * 0.7, y + m_wSize * 0.7);

	pDC->FillRect(rect_1, new CBrush(RGB(0, 119, 255)));
	pDC->Ellipse(x - m_wSize, y - m_wSize, x + m_wSize, y + m_wSize);
	pDC->FillRect(rect_2, new CBrush(RGB(255, 0, 64)));

	RestoreDC(pDC);
}

void CMyFigure::GetRegion(CRgn& Rgn)
{
	int s = m_wSize / 2;
	Rgn.CreateRectRgn(x - s, y - s, x + s, y + s);
}

////////////////////////////////////////
// Ðåàëèçàöèÿ ìåòîäîâ êëàññà CPolygon

CPolygon::CPolygon(): CBasePoint()
{
	m_wSize=0;
	m_bPolygon=FALSE;
} 

CPolygon::~CPolygon()
{
	m_PointsArray.RemoveAll( );
} 


IMPLEMENT_SERIAL(CPolygon, CObject, 1)
void CPolygon::Serialize(CArchive &ar)
{
	if(ar.IsStoring()) // ñîõðàíåíèå
	{
		// Ñîõðàíÿåì ïàðàìåòðû îáúåêòà
		ar<<m_bPolygon;
	}
	else	// ÷òåíèå
	{
		// Ïîëó÷èëè âåðñèþ ôîðìàòà
		int Version=ar.GetObjectSchema();
		// Â çàâèñèìîñòè îò âåðñèè
		// ìîæíî âûïîëíèòü ðàçëè÷íûå âàðèàíòû çàãðóçêè
		// Çàãðóæàåì ïàðàìåòðû îáúåêòà
		ar>>m_bPolygon;
	}	
	m_PointsArray.Serialize(ar);
	CBasePoint::Serialize(ar);
}
void CPolygon::Show(CDC* pDC) 
{
	// Óñòàíàâëèâàåì ïåðåî è êèñòü
	PrepareDC(pDC);
	// Ðèñóåì 
	if(m_bPolygon)
		pDC->Polygon(m_PointsArray.GetData(), m_PointsArray.GetSize());
	else
		pDC->Polyline( m_PointsArray.GetData(), m_PointsArray.GetSize());
	// Âîññòàíàâëèâàåì êîíòåêñò
	RestoreDC(pDC);
} 

void CPolygon::GetRegion(CRgn &Rgn)
{
	Rgn. CreatePolygonRgn(m_PointsArray.GetData(), m_PointsArray.GetSize(), ALTERNATE);
}

void CPolygon::Transform(const CPoint &point0, double ang, int a, int b)
{
	for(int i=0; i<m_PointsArray.GetSize(); i++)
		m_PointsArray[i]=::Transform(m_PointsArray[i], m_PointsArray[0], ang, a, b);
};

////////////////////////////////////////
// Класс CStar

CStar::CStar() : CPolygon()
{
	m_wSize = 40;
	m_bPolygon = TRUE;
}

CStar::~CStar()
{
	m_PointsArray.RemoveAll();
}

IMPLEMENT_SERIAL(CStar, CObject, 1)
void CStar::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar << m_bPolygon;
	}
	else
	{
		int Version = ar.GetObjectSchema();
		ar >> m_bPolygon;
	}
	m_PointsArray.Serialize(ar);
	CBasePoint::Serialize(ar);
}
void CStar::Show(CDC* pDC)
{
	PrepareDC(pDC);
	
	pDC->Polygon(m_PointsArray.GetData(), m_PointsArray.GetSize());

	RestoreDC(pDC);
}

void CStar::SetCenter(int x, int y, WORD s) 
{
	m_wSize = s;
	m_PointsArray.Add(CPoint(x + 200, y + 300));
	m_PointsArray.Add(CPoint(x + 1000, y + 400));
	m_PointsArray.Add(CPoint(x + 400, y - 100));
	m_PointsArray.Add(CPoint(x + 700, y - 1000));
	m_PointsArray.Add(CPoint(x, y - 400));
	m_PointsArray.Add(CPoint(x - 700, y - 1000));
	m_PointsArray.Add(CPoint(x - 400, y - 100));
	m_PointsArray.Add(CPoint(x - 1000, y + 400));
	m_PointsArray.Add(CPoint(x - 200, y + 300));
	m_PointsArray.Add(CPoint(x, y + 1200));
	m_PointsArray.Add(CPoint(x + 200, y + 300));
}

void CStar::GetRegion(CRgn& Rgn)
{
	Rgn.CreatePolygonRgn(m_PointsArray.GetData(), m_PointsArray.GetSize(), ALTERNATE);
}

void CStar::Transform(const CPoint& point0, double ang, int a, int b)
{
	for (int i = 0; i < m_PointsArray.GetSize(); i++)
		m_PointsArray[i] = ::Transform(m_PointsArray[i], m_PointsArray[0], ang, a, b);
};

/////////////////////////////////////////////////
//3D Polygon

IMPLEMENT_SERIAL(C3DPolygon, CPolygon, -1)
void C3DPolygon::Serialize(CArchive &ar)
{
	CPolygon::Serialize(ar);
	
	if(ar.IsStoring())
	{
	}
	else
	{
	}
	m_3DPointsArray.Serialize(ar);
	
};

void C3DPolygon::MakeProjection(Perspective P)
{
	// Ïåðåâîä â ðàäèàíû
	P.theta=P.theta*atan(1.0)/45.0; P.phi=P.phi*atan(1.0)/45.0;
	// Ðàñ÷åò êîýôôèöèåíòîâ ìàòðèöû ïðåîáðàçîâàíèÿ
	// Åñëè óñòàíîâëåí ðåæèì îòîáðàæåíèÿ MM_TEXT, ïðè êîòîðîì íà÷àëî êîîðäèíàò
	// â âåðõíåì ëåâîì óãëó, òðåáóåòñÿ ëèøü çàìåíèòü íà ïðîòèâîïîëîæíûé çíàê 
	// ó êîýôôèöèåíòîâ âòîðîãî ñòîëáöà ìàòðèöû ïðåîáðàçîâàíèÿ (ïåðåâåðíóòü îñü Y)

	double  st=sin(P.theta), ct=cos(P.theta), sp=sin(P.phi), cp=cos(P.phi),
	v11=-st,	v12=-cp*ct,	v13=-sp*ct,
	v21=ct,		v22=-cp*st,	v23=-sp*st,
				v32=sp,		v33=-cp,
	v41=P.dx,	v42=P.dy,	v43=P.rho;
	double x, y, z;
	double TempZ=0;
	//ðàñ÷åò âèäîâûõ êîîðäèíàò òî÷åê
	 m_PointsArray.SetSize(m_3DPointsArray.GetSize());
	for(int i=0; i<m_3DPointsArray.GetSize(); i++)
	{
		x=m_3DPointsArray[i].x-P.O.x;
		y=m_3DPointsArray[i].y-P.O.y;
		z=m_3DPointsArray[i].z-P.O.z;

		TempZ=v13*x+v23*y+v33*z+v43;
		m_PointsArray[i].x=(LONG)(v11*x+v21*y+v41+0.5);
		m_PointsArray[i].y=(LONG)(v12*x+v22*y+v32*z+v42+0.5);

		// Ïåðñïåêòèâíûå ïðåîáðàçîâàíèÿ
		if(P.with_perspective)
		{
			m_PointsArray[i].x=(LONG)(P.d*m_PointsArray[i].x/TempZ +0.5);
			m_PointsArray[i].y=(LONG)(P.d*m_PointsArray[i].y/TempZ +0.5);
		}
		
		m_PointsArray[i].x+=(LONG)(P.O.x +0.5);
		m_PointsArray[i].y+=(LONG)(P.O.y +0.5);
	}
};

//////////////////////////////////////////////////////
C3DShape::C3DShape(): CBasePoint()
{
	m_Percpective.O.x=0;
	m_Percpective.O.y=0;
	m_Percpective.O.z=0;
	m_Percpective.rho=50000; //50 ñì. â ðåæèìå MM_HIMETRIC
	m_Percpective.theta=30;  
	m_Percpective.phi=30;
	m_Percpective.d=25000; //25 ñì. â ðåæèìå MM_HIMETRIC
	m_Percpective.with_perspective=TRUE;
	m_Percpective.dx=0;
	m_Percpective.dy=0;
}

C3DShape::~C3DShape()
{
	while(m_PtrPolygonList.GetCount()>0)
		delete m_PtrPolygonList.RemoveHead();
	
};

IMPLEMENT_SERIAL(C3DShape, CBasePoint, -1)
void C3DShape::Serialize(CArchive &ar)
{
	if(ar.IsStoring())
	{
		ar << m_Percpective.O.x;
		ar << m_Percpective.O.y;
		ar << m_Percpective.O.z;
		ar << m_Percpective.rho;
		ar << m_Percpective.theta;
		ar << m_Percpective.phi;
		ar << m_Percpective.d;
		ar << m_Percpective.with_perspective;
		ar << m_Percpective.dx;
		ar << m_Percpective.dy;
	}
	else
	{
		ar >> m_Percpective.O.x;
		ar >> m_Percpective.O.y;
		ar >> m_Percpective.O.z;
		ar >> m_Percpective.rho;
		ar >> m_Percpective.theta;
		ar >> m_Percpective.phi;
		ar >> m_Percpective.d;
		ar >> m_Percpective.with_perspective;
		ar >> m_Percpective.dx;
		ar >> m_Percpective.dy;

	}
	m_PtrPolygonList.Serialize(ar);
};

void C3DShape::Show(CDC *pDC)
{
	// Âûâîä âñåõ ïîëèãîíîâ
	POSITION Pos=NULL;
	if(m_PtrPolygonList.GetCount()>0)
		Pos=m_PtrPolygonList.GetHeadPosition();
	
	while(Pos!=NULL)
		m_PtrPolygonList.GetNext(Pos)->Show(pDC);
};


void C3DShape::GetRegion(CRgn &Rgn)
{
	// Êîíñòðóèðóåì  îáëàñòü çàõâàòà C3DShape,
	// â âèäå ïðÿìîóãîëüíèêà, îõâàòûâàþùåãî èçîáðàæåíèå 
	// ôèãóðû íà ýêðàíå
	CRect Frame; // îõâàòûâàþùèé ïðÿìîóãîëüíèê
	POSITION Pos=NULL;
	int i=0;
	CPolygon *pPolygon=NULL;
	if(m_PtrPolygonList.GetCount()>0)
		Pos=m_PtrPolygonList.GetHeadPosition();
	// Èíèöèàëèçèðóåì ïðÿìîóãîëüíèê çíà÷åíèÿìè ïåðâîé òî÷êè ïåðâîãî ïîëèãîíà
	if(Pos!=NULL && (pPolygon=m_PtrPolygonList.GetAt(Pos))!=NULL && pPolygon->m_PointsArray.GetSize()>0)
	{
		Frame.left=Frame.right=pPolygon->m_PointsArray[0].x;
		Frame.top=Frame.bottom=pPolygon->m_PointsArray[0].y;
	}
	else return;
	// Ïîëó÷àåì ãàáàðèòû ôèãóðû	
	while(Pos!=NULL)
	{
		pPolygon=m_PtrPolygonList.GetNext(Pos);
		for(i=0; i<pPolygon->m_PointsArray.GetSize(); i++)
		{
			if(pPolygon->m_PointsArray[i].x<Frame.left) Frame.left=pPolygon->m_PointsArray[i].x;
			if(pPolygon->m_PointsArray[i].x>Frame.right) Frame.right=pPolygon->m_PointsArray[i].x;
			if(pPolygon->m_PointsArray[i].y>Frame.bottom) Frame.bottom=pPolygon->m_PointsArray[i].y;
			if(pPolygon->m_PointsArray[i].y<Frame.top) Frame.top=pPolygon->m_PointsArray[i].y;
		};
	}
	// Ñîçäàåì îáëàñòü
	Rgn.CreateRectRgn(Frame.left, Frame.top, Frame.right, Frame.bottom);
}

	
void C3DShape::AddPolygon(C3DPolygon *pPolygon)
{
	m_PtrPolygonList.AddTail(pPolygon);	//äîáàâèëè â ñïèñîê
	// ðàñ÷åò öåíòðà
	POSITION Pos=NULL;
	C3DPolygon* pCurPolygon=NULL;
	WORD Count=0, i=0;
	if(m_PtrPolygonList.GetCount()>0)
		Pos=m_PtrPolygonList.GetHeadPosition();
	while(Pos!=NULL)
	{
		pCurPolygon=(C3DPolygon*)m_PtrPolygonList.GetNext(Pos);
		for(i=0; i<pCurPolygon->m_3DPointsArray.GetSize(); i++)
		{
			m_Percpective.O.x+=pCurPolygon->m_3DPointsArray[i].x;	
			m_Percpective.O.y+=pCurPolygon->m_3DPointsArray[i].y;	
			m_Percpective.O.z+=pCurPolygon->m_3DPointsArray[i].z;	
		}
		Count+=i;
	}
	m_Percpective.O.x/=Count;
	m_Percpective.O.y/=Count;
	m_Percpective.O.z/=Count;
};


void C3DShape::MakeProjection()
{
	POSITION Pos=NULL;
	if(m_PtrPolygonList.GetCount()>0)
		Pos=m_PtrPolygonList.GetHeadPosition();
	while(Pos!=NULL)
		((C3DPolygon*)m_PtrPolygonList.GetNext(Pos))->MakeProjection(m_Percpective);
};

BOOL C3DShape::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, UINT nMyFlags)
{
	
	BOOL res=TRUE;
	if(nMyFlags & SHIFT_HOLD)
	switch(nChar)
	{
		case 38: m_Percpective.phi-=ROTATE_STEP; break;	//Up òî÷êà íàáëþäåíèÿ âûøå
		case 40: m_Percpective.phi+=ROTATE_STEP; break;	//Down òî÷êà íàáëþäåíèÿ íèæå
		case 37: m_Percpective.theta-=ROTATE_STEP; break; //Left òî÷êà íàáëþäåíèÿ ëåâåå
		case 39: m_Percpective.theta+=ROTATE_STEP; break; //Right òî÷êà íàáëþäåíèÿ ïðàâåå
		default: res=FALSE;
	}
	else
	if(nMyFlags & CTRL_HOLD)	 
	switch(nChar)
	{
		case 38: m_Percpective.d+=MOVE_STEP; break; //Up ýêðàí äàëüøå
		case 40: m_Percpective.d-=MOVE_STEP; break; //Down ýêðàí áëèæå
		default: res=FALSE;
	}
	else
	{
		//ïåðåíîñ
		switch(nChar)
		{
			case 38:	m_Percpective.dy+= MOVE_STEP; break; // ââåðõ
			case 40: 	m_Percpective.dy-= MOVE_STEP; break; // âíèç
			case 37: 	m_Percpective.dx-= MOVE_STEP; break; // âëåâî
			case 39:    m_Percpective.dx+= MOVE_STEP; break; // âïðàâî
			// êëàâèøà P âêë/âûêë ïåðñïåêòèâíûå ïðåîáðàçîâàíèÿ
			case 80:    m_Percpective.with_perspective=!m_Percpective.with_perspective;
						break; 
			default:	res=FALSE;
		}	
	}
	if(res)
		// Ðàñ÷åò ïðîåêöèè
		MakeProjection();
	return res;
};


/////////////////////////////////////////////////////////////////////////////////
// Ãëîáàëüíûå ôóíêöèè äëÿ ðàáîòû ñ ïîâåðõíîñòÿìè

int AddRsection(C3DShape *pShape, POINT3D *pSur, int x_size, int y_size, double level, COLORREF color)
{
	if(x_size<2 || y_size<2) return 0;
	// Ïîëèãîí äëÿ âðåìåííîãî õðàíåíèÿ òî÷åê ëèíèè óðîâíÿ
	C3DPolygon *pTempPolygon=new C3DPolygon();
	if(pTempPolygon==NULL) return 0; 
	// Ðàçáèâàåì ïîâåðõíîñòü íà òðåóãîëüíèêè è
	// ïðîáóåì íàéòè ïåðåñå÷åíèå äëÿ êàæäîãî òðåóãîëüíèêà è ïëîñêîñòè level.
	// Òî÷êè ïåðåñå÷åíèÿ äîáàâëÿåì â pTempPolygon
	for(int x=0, y=0; y<y_size-1; y++)
		for(int x=0;  x<x_size-1; x++)
		{
			AddTriangleSection(pTempPolygon, &pSur[y*x_size+x], &pSur[(y+1)*x_size+x+1], &pSur[y*x_size+x+1], level);
			AddTriangleSection(pTempPolygon, &pSur[y*x_size+x], &pSur[(y+1)*x_size+x], &pSur[(y+1)*x_size+x+1], level);
		}
	// Èç ïîëó÷åííîãî íàáîðà òî÷åê ñîçäàåì àêêóðàòíûå ïîëèãîí÷èêè
	// Äëÿ óïðîùåíèÿ ðàáîòû ñ òî÷êàìè ïîëó÷èì ññûëêó íà äàííûå
	// Ýòî êîíå÷íî íå ëó÷øàÿ èëëþñòðàöèÿ ïðèíöèïîâ ÎÎÏ, çàòî óäîáíî :)
	CArray <POINT3D, POINT3D> &TempPointsArray=pTempPolygon->m_3DPointsArray;
	
	int pos=0, posmin=0;
	POINT3D EndSegPoint;
	double D=0, dcur=0, dmin=0; // ðàññòîÿíèå ìåæäó òî÷êàìè
	C3DPolygon *pSeg;
	BOOL fContinueSeg=TRUE; // ôëàã "ïðîäîëæèòü òåêóùèé ñåãìåíò"
	// Âû÷èñëÿåì ýòàëîííîå ðàññòîÿíèå ìåæäó òî÷êàìè - äèàãîíàëü ñåòêè íà ïëîñêîñòè
	POINT3D P1=pSur[0], P2=pSur[x_size+1];	P1.z=P2.z=0;
	D=Dist(&P1, &P2);  
	// Ïîêà âî âðåìåííîì ìàññèâå îñòàëàñü õîòÿ áû ïàðà òî÷åê
	// cñîçäàåì èç ìàññèâà ñåãìåíòû ñå÷åíèÿ
	while(TempPointsArray.GetSize()-1>0)
	{
		// Íîâûé ñåãìåíò - ïîëèãîí
		pSeg=new C3DPolygon();	fContinueSeg=TRUE;
		if(pSeg==NULL) return 0;
		// Óñòàíîâèì öâåò
		pSeg->SetPen(color);
		// Ïåðâàÿ òî÷êà-íà÷àëî è êîíåö ñåãìåíòà
		pSeg->AddPoint(TempPointsArray[0]);
		EndSegPoint=TempPointsArray[0];
		// Óäàëÿåì òî÷êó èç îáùåãî ìàññèâà òî÷åê
		TempPointsArray.RemoveAt(0);
		// Ïðîäîëæàåì ïîëèãîí
		while(fContinueSeg )
		{	
			posmin=0;
			dmin=D*2;
			// Ñ íà÷àëà ìàññèâà
			// Âûáèðàåì áëèæàéøóþ ê êîíöó ñåãìåíòà òî÷êó 
			for(pos=0; pos<TempPointsArray.GetSize(); pos++)
			{
			  dcur=Dist(&EndSegPoint, &TempPointsArray[pos]);
			  if(dcur<dmin) 
			    {dmin=dcur; posmin=pos;}//çàïîìèíàåì ïîçèöèþ(íîìåð) áëèæàéøåé òî÷êè
			}
			if(dmin<=D) //ðàññòîÿíèå äî áëèæàéøåé òî÷êè ìåíüøå ýòàëîííîãî,
			{	
				//íî âñå-òàêè òî÷êà íå ñîâïàäàåò ñ êîíöîì ñåãìåíòà,
				//ïîýòîìó äîáàâèì åå â ñåãìåíò
				if(dmin>D/1000) 
				{
					//Áëèæàéøóþ òî÷ó â ñåãìåíò
					pSeg->AddPoint(TempPointsArray[posmin]);
					//Íîâàÿ òî÷êà ñòàíîâèòüñÿ êîíöîì ñåãìåíòà
					EndSegPoint=TempPointsArray[posmin];
				}

				//Óäàëÿåì ýòó òî÷êó
				TempPointsArray.RemoveAt(posmin);
			}
			else //íå íàøëè áëèçêîé ê êîíöó òî÷êè- çàêðûâàåì ñåãìåíò
				fContinueSeg=FALSE;
		} ;
		//Ïðîâåðèì, ìîæåò ñòîèò çàìêíóòü ñåãìåíò
		if(pSeg->m_3DPointsArray.GetSize()>2)
			if(Dist(&pSeg->m_3DPointsArray[0], &pSeg->m_3DPointsArray[pSeg->m_3DPointsArray.GetSize()-1])<D)
				pSeg->AddPoint(pSeg->m_3DPointsArray[0]);
		
			
		//Äîáàâëÿåì ïîëèãîí â ôèãóðó 
		pShape->AddPolygon(pSeg);
	} 	
	//Âðåìåííûé ïîëèãîí íàì áîëüøå íå íóæåí
	delete pTempPolygon;
	return 1;
}



void AddTriangleSection(C3DPolygon *p3DPolygon, POINT3D *pP1, POINT3D *pP2, POINT3D *pP3, double level)
{
	int  f1,f2,f3;
	double x1,x2,x3,y1,y2,y3;
	POINT3D P1, P2;
	if(	!((pP1->z==level)&&(pP2->z==level)&&(pP3->z==level)) && //òðåóãîëüíèê â ïëîñêîñòè
   		!((pP1->z>level)&&(pP2->z>level)&&(pP3->z>level)) &&   //òðåóãîëüíèê è ïëîñêîñòü íå ïåðåñåêàþòñÿ
		!((pP1->z<level)&&(pP2->z<level)&&(pP3->z<level)) )	   //òðåóãîëüíèê è ïëîñêîñòü íå ïåðåñåêàþòñÿ

   	if((pP1->z==level)&&(pP2->z==level))	//ñòîðîíà â ïëîñêîñòè - äîáàâëÿåì
   	{
		p3DPolygon->AddPoint(*pP1);
		p3DPolygon->AddPoint(*pP2);
   	}
	else
		if((pP2->z==level)&&(pP3->z==level)) //ñòîðîíà â ïëîñêîñòè - äîáàâëÿåì
   		{ 
   			p3DPolygon->AddPoint(*pP2);
			p3DPolygon->AddPoint(*pP3);
		}
   		else
   			if((pP3->z==level)&&(pP1->z==level)) //ñòîðîíà â ïëîñêîñòè - äîáàâëÿåì
   		    { 
	   			p3DPolygon->AddPoint(*pP3);
				p3DPolygon->AddPoint(*pP1);
			}
   			else
   			{	// Íàõîäèì 	ïåðåñå÷åíèå êàæäîé ñòîðîíû òðåóãîëüíèêà ñ ïëîñêîñòüþ
				 f1=CutCross(level,pP1, pP2, x1, y1); 
				 f2=CutCross(level,pP2, pP3, x2, y2);
				 f3=CutCross(level,pP3, pP1, x3, y3);
				 if(f1&&f2) 
				 { 
					P1.x=x1; P1.y=y1; P1.z=level; 
					P2.x=x2; P2.y=y2; P2.z=level; 
					p3DPolygon->AddPoint(P1);
					p3DPolygon->AddPoint(P2);
			     }
				 if(f2&&f3)
				 { 
					P1.x=x2; P1.y=y2; P1.z=level; 
					P2.x=x3; P2.y=y3; P2.z=level; 
					p3DPolygon->AddPoint(P1);
					p3DPolygon->AddPoint(P2);
				 }
				 if(f1&&f3)
				 { 
					P1.x=x1; P1.y=y1; P1.z=level; 
					P2.x=x3; P2.y=y3; P2.z=level; 
					p3DPolygon->AddPoint(P1);
					p3DPolygon->AddPoint(P2);
				 }
   }//~else
}

int CutCross(double level, POINT3D *pP1, POINT3D *pP2, double &x, double &y)
{ if( (pP1->z<level && pP2->z<level) || //îòðåçîê ïîä ïëîñêîñòüþ level
      (pP1->z>level && pP2->z>level) ||	//îòðåçîê íàä ïëîñêîñòüþ level
	  (pP1->z==pP2->z)  ) 	 //îòðåçîê â ïëîñêîñòè level
		{x=pP1->x; y=pP1->y; return 0;} 
  else
    {
	 x=pP2->x-(pP1->x-pP2->x)*(level-pP2->z)/(pP2->z-pP1->z);
	 y=pP2->y-(pP1->y-pP2->y)*(level-pP2->z)/(pP2->z-pP1->z);
	 return 1;
	}
}

double Dist(POINT3D *pP1, POINT3D* pP2)
{
	if(pP1==NULL||pP2==NULL) return 0;
	return sqrt(pow(pP1->x-pP2->x, 2)+pow(pP1->y-pP2->y, 2)+pow(pP1->z-pP2->z, 2));
};
// ôàéë Shapes.h
////////////////////////////////////////////////////
//êëàññ áàçîâàÿ òî÷êà
#define MOVE_STEP 100
#define ROTATE_STEP 5
#define SHIFT_HOLD	1
#define CTRL_HOLD	2

class CBasePoint: public CPoint, public CObject
{
 	DECLARE_SERIAL(CBasePoint)
	CPen	m_Pen;	// ïåðî
	CBrush	m_Brush;// êèñòü
protected:
	// Ìåòîä ñåðèàëèçàöèè
	virtual void Serialize(CArchive& ar);
	// Ïîäãîòàâëèâàåò êîíòåêñò óñòðîéñòâà
	virtual BOOL PrepareDC(CDC *pDC);
	// Âîññòàâëèâàåò êîíòåêñò óñòðîéñòâà
	virtual BOOL RestoreDC(CDC *pDC);
public:
	// Äàííûå
	WORD		m_wSize;			//ðàçìåð ôèãóðû
	int			m_iPenStyle;		//ñòèëü ëèíèé
	int			m_iPenWidth;		//øèðèíà ëèíèé
	COLORREF	m_rgbPenColor;		//öâåò ëèíèé
	int			m_iBrushStyle;		//ñòèëü çàëèâêè
	COLORREF	m_rgbBrushColor;	//öâåò çàëèâêè
	DWORD		m_dwPattern_ID;		//èäåíòèôèêàòîð øàáëîíà çàëèâêè
public:		
	// Êîíñòðóêòîðû
	CBasePoint();				//êîíñòðóêòîð áåç ïàðàìåòðîâ
	CBasePoint(int x, int y, WORD s);	//êîíñòðóêòîð ñ ïàðàìåòðàìè
	~CBasePoint(){};			//äåñòðóêòîð
	// Ìåòîäû
	// Îòîáðàæàåò ôèãóðó íà ýêðàíå
	virtual void Show(CDC *pDC);
	// Ñîîáùàåò îáëàñòü çàõâàòà
	virtual void GetRegion(CRgn &Rgn);
	// Óñòàíàâëèâàåò ïàðàìåòðû ëèíèé
	virtual BOOL SetPen(COLORREF color, int width =1, int style=PS_SOLID);
	// Óñòàíàâëèâàåò ïàðàìåòðû çàëèâêè
	virtual BOOL SetBrush(COLORREF color, DWORD pattern =0, int style=-1);
	// Âûïîëíÿåò ïðåîáðàçîâàíèå íà ïëîñêîñòè
	virtual void Transform(const CPoint &point0, double ang, int a, int b);
	// Ðåàêöèÿ íà íàæàòèå êëàâèøè
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, UINT nMyFlags); 
};

////////////////////////////////////////////////////
//êëàññ êâàäðàò
class CSquare: public CBasePoint
{
 	DECLARE_SERIAL(CSquare)   
 protected:  
	 // Ìåòîä ñåðèàëèçàöèè
	void Serialize(CArchive& ar);
 public:		
	// Êîíñòðóêòîðû
	CSquare(int x, int y, WORD s);
	CSquare();
	~CSquare(){};
//Ìåòîäû
	// Îòîáðàæàåò ôèãóðó íà ýêðàíå
	void Show(CDC *pDC);
	// Ñîîáùàåò îáëàñòü çàõâàòà
	void GetRegion(CRgn &Rgn);
};

////////////////////////////////////////////////////
//êëàññ ìîÿ ôèãóðà
class CMyFigure : public CBasePoint
{
	DECLARE_SERIAL(CMyFigure)
protected:
	// Ìåòîä ñåðèàëèçàöèè
	void Serialize(CArchive& ar);
public:
	// Êîíñòðóêòîðû
	CMyFigure(int x, int y, WORD s);
	CMyFigure();
	~CMyFigure() {};
	//Ìåòîäû
		// Îòîáðàæàåò ôèãóðó íà ýêðàíå
	void Show(CDC* pDC);
	// Ñîîáùàåò îáëàñòü çàõâàòà
	void GetRegion(CRgn& Rgn);
};

////////////////////////////////////////////////////
//êëàññ ïîëèãîí
class CPolygon: public CBasePoint
{
 	DECLARE_SERIAL(CPolygon)   
	BOOL	m_bPolygon;	// ðåæèì ðèñîâàíèÿ: 
	// TRUE - çàïîëíåííûé ïîëèãîí,
	// FALSE - ëîìàíàÿ êðèâàÿ. Íåò, ëîìàíàÿ ïðÿìàÿ. Íåò, ëîìàíàÿ ëèíèÿ. Âî! 

 protected:  
	 // Ìåòîä ñåðèàëèçàöèè
	void Serialize(CArchive& ar);
 public:
	 // Äèíàìè÷åñêèé ìàññèâ òî÷åê-âåðøèí
	 CArray <CPoint, CPoint> m_PointsArray;
		
	// Êîíñòðóêòîðû
	 CPolygon();
	~CPolygon();
//Ìåòîäû
	// Îòîáðàæàåò ôèãóðó íà ýêðàíå
	void Show(CDC *pDC);
	// Ñîîáùàåò îáëàñòü çàõâàòà
	void GetRegion(CRgn &Rgn);
	// Óñòàíàâëèâàåò ðåæèì ðèñîâàíèÿ ïîëèãîíà
	void SetPolygon(BOOL p) {m_bPolygon=p;};
	// Âûïîëíÿåò ïðåîáðàçîâàíèå íà ïëîñêîñòè
	void Transform(const CPoint &point0, double ang, int a, int b);
};

////////////////////////////////////////////////////
// Класс CStar
class CStar : public CPolygon
{
	DECLARE_SERIAL(CStar)
	BOOL	m_bPolygon;

protected:
	void Serialize(CArchive & ar);
public:
	CArray <CPoint, CPoint> m_PointsArray;

	CStar();
	~CStar();

	void Show(CDC * pDC);
	void SetCenter(int x, int y, WORD s);
	void GetRegion(CRgn & Rgn);
	void SetPolygon(BOOL p) { m_bPolygon = p; };
	void Transform(const CPoint & point0, double ang, int a, int b);
};

////////////////////////////////////////////////////
// 3D òî÷êà
struct POINT3D
{
	double x, y, z;
}; 

//ïàðàìåòðû  òðåõìåðíîé ñöåíû
struct  Perspective
{
	POINT3D	O;					// òî÷êà, âîêðóã êîòîðîé âûïîëíÿåì ïîâîðîò
	double	rho, theta, phi,	// ïîëÿðíûå êîîðäèíàòû òî÷êè íàáëþäåíèÿ (E)
			d;					// ðàññòîÿíèå îò E äî ýêðàíà
	WORD	with_perspective;   // 1- âêë. ïåðñïåêòèâíûå ïðåîáðàçîâàíèÿ; 0-âûêë.
	LONG	dx, dy;				// ñìåùåíèå ïðîåêöèè íà ýêðàíå
};

////////////////////////////////////////////////////
//êëàññ C3DPolygon
//
class C3DPolygon: public CPolygon
{
 	DECLARE_SERIAL(C3DPolygon)   
 protected:  	// ìåòîä ñåðèàëèçàöèè	
	void Serialize(CArchive& ar);
 public:		
 	// Êîíñòðóêòîðû
	 C3DPolygon(){};
	 ~C3DPolygon(){};
// Äàííûå
	// Äèíàìè÷åñêèé ìàññèâ òî÷åê-âåðøèí â ìèðîâûõ êîîðäèíàòàõ
	CArray <POINT3D, POINT3D> m_3DPointsArray;	
// Ìåòîäû		
	// Äîáàâèòü òî÷êó
	void AddPoint(POINT3D point) {m_3DPointsArray.Add(point);};
	// Ðàñ÷åò ýêðàííûõ êîîðäèíàò
	void MakeProjection(Perspective P); 
};


////////////////////////////////////////////////////
//êëàññ C3DShape
class C3DShape: public CBasePoint
{
 	DECLARE_SERIAL(C3DShape)   
 protected:  	// Âèðòóàëüíûé ìåòîä ñåðèàëèçàöèè	
	void Serialize(CArchive& ar);
	
 public:		
	//êîíñòðóêòîðû
	C3DShape();
	~C3DShape();
//Äàííûå
	Perspective m_Percpective; //ïàðàìåòðû îáçîðà
	CTypedPtrList<CObList, C3DPolygon*> m_PtrPolygonList; // ñïèñîê óêàçàòåëåé íà ïîëèãîíû
//Ìåòîäû
	// Ðàñ÷åò ïðîåêöèè
	void MakeProjection();
	// Îòîáðàæåíèå ôèãóðû íà ýêðàíå
	void Show(CDC *pDC);
	// Ñîîáùàåò îáëàñòü çàõâàòà
	void GetRegion(CRgn &Rgn);
	// Äîáàâèòü ïîëèãîí
	void AddPolygon(C3DPolygon *pPolygon);
	// Ðåàêöèÿ íà íàæàòèå êëàâèøè
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, UINT nMyFlags);
};

/////////////////////////////////////////////////////////////////////////////////
// Ãëîáàëüíûå ôóíêöèè äëÿ ðàáîòû ñ ïîâåðõíîñòÿìè,
// èç êîòîðûõ, ïî-õîðîøåìó, íàäî áû ñäåëàòü êëàññ C3DSurface

// Äîáàâëÿåò ëèíèþ ïåðåñå÷åíèÿ (ëèíèþ óðîâíÿ) ïîâåðõíîñòè,
// çàäàííîé ìàññèâîì òî÷åê POINT3D ñ ïëîñêîñòüþ,
// çàäàííîé çíà÷åíèåì level (óðîâåíü ïî Z)
// Ïàðàìåòðû:
// pShape - óêàçàòåëü íà òðåõìåðíûé îáúåêò, êóäà áóäåò äîáàâëåíî ñå÷åíèå
// pSur - óêàçàòåëü íà ìàññèâ òî÷åê, çàäàþùèõ ïâåðõíîñòü
// x_size, y_size - ðàçìåð ìàññèâà òî÷åê
// level - óðîâåíü ñåêóùåé ïëîñêîñòè
// color - öâåò ëèíèè ïåðåñå÷åíèÿ  
// Âîçâðàùàåìîå çíà÷åíèå:
// 0 - íåóäà÷à
// 1 - óñïåõ
int AddRsection(C3DShape *pShape, POINT3D *pSur, int x_size, int y_size, double level,  COLORREF color);

// Äîáàâëÿåò ëèíèþ ïåðåñå÷åíèÿ òðåóãîëüêà,
// çàäàííîãî òðåìÿ òî÷êàìè ñ ïëîñêîñòüþ,
// çàäàííîé çíà÷åíèåì level (óðîâåíü ïî Z)
// Ïàðàìåòðû:
// p3DPolygon - óêàçàòåëü íà ïîëèãîí, êóäà áóäåò äîáàâëåíû òî÷êè ïåðåñå÷åíèÿ
// pP1, pP2, pP3 - óêàçàòåëè íà òî÷åêè, çàäàþùèå òðåóãîëüíèê
// level - óðîâåíü ñåêóùåé ïëîñêîñòè
void AddTriangleSection(C3DPolygon *p3DPolygon, POINT3D *pP1, POINT3D *pP2, POINT3D *pP3, double level);

// À ýòè ôóíêöèè âïîëíå ìîæíî îñòàâèòü ãëîáàëüíûìè

// Ðàññ÷èòûâàåò êîîðäèíàòû x,y òî÷êè ïåðåñå÷åíèÿ îòðåçêà pP1, pP2 ñ ïîâåðõíîñòüþ level
int CutCross(double level, POINT3D *pP1, POINT3D *pP2, double &x, double &y);
// Ðàññ÷èòûâàåò ðàññòîÿíèå ìåæäó äâóìÿ òî÷êàìè
double Dist(POINT3D *pP1, POINT3D* pP2);
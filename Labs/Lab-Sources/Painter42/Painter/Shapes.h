// ���� Shapes.h
////////////////////////////////////////////////////
//����� ������� �����
#define MOVE_STEP 100
#define ROTATE_STEP 5
#define SHIFT_HOLD	1
#define CTRL_HOLD	2

class CBasePoint: public CPoint, public CObject
{
 	DECLARE_SERIAL(CBasePoint)
	CPen	m_Pen;	// ����
	CBrush	m_Brush;// �����
protected:
	// ����� ������������
	virtual void Serialize(CArchive& ar);
	// �������������� �������� ����������
	virtual BOOL PrepareDC(CDC *pDC);
	// ������������� �������� ����������
	virtual BOOL RestoreDC(CDC *pDC);
public:
	// ������
	WORD		m_wSize;			//������ ������
	int			m_iPenStyle;		//����� �����
	int			m_iPenWidth;		//������ �����
	COLORREF	m_rgbPenColor;		//���� �����
	int			m_iBrushStyle;		//����� �������
	COLORREF	m_rgbBrushColor;	//���� �������
	DWORD		m_dwPattern_ID;		//������������� ������� �������
public:		
	// ������������
	CBasePoint();				//����������� ��� ����������
	CBasePoint(int x, int y, WORD s);	//����������� � �����������
	~CBasePoint(){};			//����������
	// ������
	// ���������� ������ �� ������
	virtual void Show(CDC *pDC);
	// �������� ������� �������
	virtual void GetRegion(CRgn &Rgn);
	// ������������� ��������� �����
	virtual BOOL SetPen(COLORREF color, int width =1, int style=PS_SOLID);
	// ������������� ��������� �������
	virtual BOOL SetBrush(COLORREF color, DWORD pattern =0, int style=-1);
	// ��������� �������������� �� ���������
	virtual void Transform(const CPoint &point0, double ang, int a, int b);
	// ������� �� ������� �������
	virtual BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, UINT nMyFlags); 
};

//////////////////////////////////////////////////////////////////////////////////////
// MY SHIT

class Traffic_light : public CBasePoint
{
	DECLARE_SERIAL(Traffic_light)
protected:
	// ����� ������������
public:
	Traffic_light();				//����������� ��� ����������
	Traffic_light(int x, int y, WORD s);	//����������� � �����������
	~Traffic_light(){};			//����������
//������
	// ���������� ������ �� ������
	void Show(CDC* pDC);
	// �������� ������� �������
};







////////////////////////////////////////////////////
//����� �������
class CSquare: public CBasePoint
{
 	DECLARE_SERIAL(CSquare)   
 protected:  
	 // ����� ������������
	void Serialize(CArchive& ar);
 public:		
	// ������������
	CSquare(int x, int y, WORD s);
	CSquare();
	~CSquare(){};
//������
	// ���������� ������ �� ������
	void Show(CDC *pDC);
	// �������� ������� �������
	void GetRegion(CRgn &Rgn);
};

////////////////////////////////////////////////////
//����� �������
class CPolygon: public CBasePoint
{
 	DECLARE_SERIAL(CPolygon)   
	BOOL	m_bPolygon;	// ����� ���������: 
	// TRUE - ����������� �������,
	// FALSE - ������� ������. ���, ������� ������. ���, ������� �����. ��! 

 protected:  
	 // ����� ������������
	void Serialize(CArchive& ar);
 public:
	 // ������������ ������ �����-������
	 CArray <CPoint, CPoint> m_PointsArray;
		
	// ������������
	 CPolygon();
	~CPolygon();
//������
	// ���������� ������ �� ������
	void Show(CDC *pDC);
	// �������� ������� �������
	void GetRegion(CRgn &Rgn);
	// ������������� ����� ��������� ��������
	void SetPolygon(BOOL p) {m_bPolygon=p;};
	// ��������� �������������� �� ���������
	void Transform(const CPoint &point0, double ang, int a, int b);
};

class PolyFigure : public CPolygon
{
public:
	PolyFigure();
	PolyFigure(int x, int y, int s);
	~PolyFigure();
	
	void Show(CDC* pDC);
};

////////////////////////////////////////////////////
// 3D �����
struct POINT3D
{
	double x, y, z;
}; 

class Spline : public CPolygon
{
public:
	CArray <CPoint, CPoint> m_BasePointsArray;
	CArray <CPoint, CPoint> m_SplinePointsArray;
	Spline();
	~Spline();

	void Show(CDC* pDC);

	void makeSplinePoints();
	void drawBasePoints(CDC* pDC);
	
	double getAngleForCP(CPoint thisPoint, CPoint leftPoint, CPoint rightPoint);

	double GetLenght(CPoint a1, CPoint a2);
	
	CPoint GetMiddle1(CPoint a1, CPoint a2);

	double angle(int x1, int y1, int x2, int y2);
	bool figure = 0;

};

class SplineFigure : public Spline
{
public:
	SplineFigure();
	SplineFigure(int x, int y, float s);
	~SplineFigure();
};

class Vector
{
public:
	int x, y;
	Vector(int x, int y);
	Vector(CPoint start, CPoint end);
	int operator*(Vector& other);
	static double getAngleBetween(Vector& a, Vector& b);
	double length();
	Vector operator*(int num);

};

///////////////////////////////////
//BMP PICTURE

class BMPPicture : public CBasePoint
{
public:
	HBITMAP hBM;
	HBITMAP BM;
	BMPPicture(int x, int y, HBITMAP hBmp);

	void Show(CDC* pDC);
};



//���������  ���������� �����
struct  Perspective
{
	POINT3D	O;					// �����, ������ ������� ��������� �������
	double	rho, theta, phi,	// �������� ���������� ����� ���������� (E)
			d;					// ���������� �� E �� ������
	WORD	with_perspective;   // 1- ���. ������������� ��������������; 0-����.
	LONG	dx, dy;				// �������� �������� �� ������
};

////////////////////////////////////////////////////
//����� C3DPolygon
//
class C3DPolygon: public CPolygon
{
 	DECLARE_SERIAL(C3DPolygon)   
 protected:  	// ����� ������������	
	void Serialize(CArchive& ar);
 public:		
 	// ������������
	 C3DPolygon(){};
	 ~C3DPolygon(){};
// ������
	// ������������ ������ �����-������ � ������� �����������
	CArray <POINT3D, POINT3D> m_3DPointsArray;	
// ������		
	// �������� �����
	void AddPoint(POINT3D point) {m_3DPointsArray.Add(point);};
	// ������ �������� ���������
	void MakeProjection(Perspective P); 
};


////////////////////////////////////////////////////
//����� C3DShape
class C3DShape: public CBasePoint
{
 	DECLARE_SERIAL(C3DShape)   
 protected:  	// ����������� ����� ������������	
	void Serialize(CArchive& ar);
	
 public:		
	//������������
	C3DShape();
	~C3DShape();
//������
	Perspective m_Percpective; //��������� ������
	CTypedPtrList<CObList, C3DPolygon*> m_PtrPolygonList; // ������ ���������� �� ��������
//������
	// ������ ��������
	void MakeProjection();
	// ����������� ������ �� ������
	void Show(CDC *pDC);
	// �������� ������� �������
	void GetRegion(CRgn &Rgn);
	// �������� �������
	void AddPolygon(C3DPolygon *pPolygon);
	// ������� �� ������� �������
	BOOL OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags, UINT nMyFlags);
};

/////////////////////////////////////////////////////////////////////////////////
// ���������� ������� ��� ������ � �������������,
// �� �������, ��-��������, ���� �� ������� ����� C3DSurface

// ��������� ����� ����������� (����� ������) �����������,
// �������� �������� ����� POINT3D � ����������,
// �������� ��������� level (������� �� Z)
// ���������:
// pShape - ��������� �� ���������� ������, ���� ����� ��������� �������
// pSur - ��������� �� ������ �����, �������� ����������
// x_size, y_size - ������ ������� �����
// level - ������� ������� ���������
// color - ���� ����� �����������  
// ������������ ��������:
// 0 - �������
// 1 - �����
int AddRsection(C3DShape *pShape, POINT3D *pSur, int x_size, int y_size, double level,  COLORREF color);

// ��������� ����� ����������� ����������,
// ��������� ����� ������� � ����������,
// �������� ��������� level (������� �� Z)
// ���������:
// p3DPolygon - ��������� �� �������, ���� ����� ��������� ����� �����������
// pP1, pP2, pP3 - ��������� �� ������, �������� �����������
// level - ������� ������� ���������
void AddTriangleSection(C3DPolygon *p3DPolygon, POINT3D *pP1, POINT3D *pP2, POINT3D *pP3, double level);

// � ��� ������� ������ ����� �������� �����������

// ������������ ���������� x,y ����� ����������� ������� pP1, pP2 � ������������ level
int CutCross(double level, POINT3D *pP1, POINT3D *pP2, double &x, double &y);
// ������������ ���������� ����� ����� �������
double Dist(POINT3D *pP1, POINT3D* pP2);
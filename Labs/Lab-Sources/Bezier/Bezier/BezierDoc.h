// Beziedoc.h : interface of the CBezierDoc class
//
/////////////////////////////////////////////////////////////////////////////

// Количество генерируемых базовых точек
#define NBASEPOINTS 10
// Количество отрезков, аппроксимирующих сплайновую кривую
#define NAPPROXCUTS 10

// Тип сплайна
#define ANYSLPINE	0
#define BEZIER	1


class CBezierDoc : public CDocument
{
protected: // create from serialization only
	CBezierDoc();
	DECLARE_DYNCREATE(CBezierDoc)

// Attributes
public:
	 // Динамический массив базовых точек кривой
	 CArray <CPoint, CPoint> m_BasePointsArray;
	 CArray <CPoint, CPoint> m_SplinePointsArray;
	 // Тип сплайна. Так как кривая Безье у нас рисуется методом 
	 // класса CDC, надо дать знать, когда его использовать
	 // Если m_nSplineType == BEZIER - кривая Безье;
	 int m_nSplineType;
	 // Operations
public:
	// Генерирует базовые точки
	void GenerateBasePoints();
	// На основе набора базовых точек создает
	// набор точек для построения кривой Безье
	// методом CDC::PolyBezier
	// Для обеспечения гладкости в местах стыковки сегментов кривой Безье:
	// если nMode == 0 ничего не делает; 
	// если nMode == 1 дополняет набор базовых точек;
	// если nMode == 2 изменяет положение базовых точек в местах стыковки.
	void CreateBezier(int nMode=0);
	// На основе набора базовых точек рассчитывает Catmull-Rom сплайн 
	void CreateCatmullRom();
	// На основе набора базовых точек рассчитывает Beta сплайн 
	void CreateBeta();

	
 

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBezierDoc)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBezierDoc();
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBezierDoc)
	afx_msg void OnEditCreateBezier0();
	afx_msg void OnEditCreateBezier1();
	afx_msg void OnEditCreateBezier2();
	afx_msg void OnEditGeneratebaseline();
	afx_msg void OnEditCteatecatmullrom();
	afx_msg void OnEditCeratebeta();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////
//Глобальная функция. Возвращает средину отрезка
CPoint GetMiddle(CPoint *pP1, CPoint *pP2);
// Возвращает точку Catmull-Rom сплайна для параметра t=(0...1)
// на участке между точками 1 и 2 сегмента из четырех точек
// pSegment - указатель на начало сегмента
CPoint GetCatmullRomPoint(CPoint *pSegment, double t);

// Возвращает точку Бета сплайна для параметра t=(0...1)
// на участке между точками 1 и 2 сегмента из четырех точек
// pSegment - указатель на начало сегмента
// beta1, beta2 - параметры формы
CPoint GetBetaPoint(CPoint *pSegment, double t, double beta1, double beta2);


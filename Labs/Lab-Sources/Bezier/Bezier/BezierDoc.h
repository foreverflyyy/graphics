// Beziedoc.h : interface of the CBezierDoc class
//
/////////////////////////////////////////////////////////////////////////////

// ���������� ������������ ������� �����
#define NBASEPOINTS 10
// ���������� ��������, ���������������� ���������� ������
#define NAPPROXCUTS 10

// ��� �������
#define ANYSLPINE	0
#define BEZIER	1


class CBezierDoc : public CDocument
{
protected: // create from serialization only
	CBezierDoc();
	DECLARE_DYNCREATE(CBezierDoc)

// Attributes
public:
	 // ������������ ������ ������� ����� ������
	 CArray <CPoint, CPoint> m_BasePointsArray;
	 CArray <CPoint, CPoint> m_SplinePointsArray;
	 // ��� �������. ��� ��� ������ ����� � ��� �������� ������� 
	 // ������ CDC, ���� ���� �����, ����� ��� ������������
	 // ���� m_nSplineType == BEZIER - ������ �����;
	 int m_nSplineType;
	 // Operations
public:
	// ���������� ������� �����
	void GenerateBasePoints();
	// �� ������ ������ ������� ����� �������
	// ����� ����� ��� ���������� ������ �����
	// ������� CDC::PolyBezier
	// ��� ����������� ��������� � ������ �������� ��������� ������ �����:
	// ���� nMode == 0 ������ �� ������; 
	// ���� nMode == 1 ��������� ����� ������� �����;
	// ���� nMode == 2 �������� ��������� ������� ����� � ������ ��������.
	void CreateBezier(int nMode=0);
	// �� ������ ������ ������� ����� ������������ Catmull-Rom ������ 
	void CreateCatmullRom();
	// �� ������ ������ ������� ����� ������������ Beta ������ 
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
//���������� �������. ���������� ������� �������
CPoint GetMiddle(CPoint *pP1, CPoint *pP2);
// ���������� ����� Catmull-Rom ������� ��� ��������� t=(0...1)
// �� ������� ����� ������� 1 � 2 �������� �� ������� �����
// pSegment - ��������� �� ������ ��������
CPoint GetCatmullRomPoint(CPoint *pSegment, double t);

// ���������� ����� ���� ������� ��� ��������� t=(0...1)
// �� ������� ����� ������� 1 � 2 �������� �� ������� �����
// pSegment - ��������� �� ������ ��������
// beta1, beta2 - ��������� �����
CPoint GetBetaPoint(CPoint *pSegment, double t, double beta1, double beta2);


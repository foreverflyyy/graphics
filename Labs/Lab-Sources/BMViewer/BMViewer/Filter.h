// Filter.h : interface of CFilter and derived classes
// (C) Alexey Polyakov 2002
/////////////////////////////////////////////////////////////////////////////
#ifndef _FILTER_INCLUDED
#define _FILTER_INCLUDED

class CRaster;
// ������� ����������� �����
class CFilter
{
protected:
	CRaster *m_pSourceBM;
	CRaster *m_pDestBM;
public:
	// ������������� �������� � �������� �����������
	void SetBuffers( CRaster *pSource, CRaster *pDest=NULL)
		{	m_pSourceBM=pSource;	m_pDestBM=pDest;};
	// ����������� ����� �������������� ������� 
	// ����� ������������� � ����������� �������
	virtual BOOL TransformPix(LONG x, LONG y){ return FALSE;};
};

// ������� ����� ��� �������� ��������
class CDotFilter: public CFilter
{
protected:
	 // ������� �������������� ��� ��������� �����
	BYTE BGRTransTable[3][256];
public:
	// ����� �������������� ������� 
	BOOL TransformPix(LONG x, LONG y);
};

// �����������
class CHistogram: public  CDotFilter
{
public:
	BOOL Init(int offset_b, int offset_t);
};

// �������-��������
class CBrightCont: public  CDotFilter
{
public:
	BOOL Init(int b_offset, int c_offset);
};


// �������� ������
class CInvertColors: public  CDotFilter
{
public:
	CInvertColors();
};

// ������ 
class CEmboss: public  CDotFilter
{
public:
	BOOL TransformPix(LONG x, LONG y);
};

// ���������������� (���������) �������
// ������� �����
class CMatrixFilter: public CFilter
{
protected:
	int m_rangX; // ������ ������� �� X � Y
	int m_rangY;
	const int *m_pMatrix; // ��������� �� �������
public:
	// ����� �������������� ������� 
	BOOL TransformPix(LONG x, LONG y);
};

class CBlur: public CMatrixFilter
{
public:
	CBlur();
};

class CContour: public CMatrixFilter
{
public:
	CContour();
};


class CSharp: public CMatrixFilter
{
public:
	CSharp();
	BOOL TransformPix(LONG x, LONG y);
};

class CDeNoise: public CMatrixFilter
{

public:
	double m_dK;
	int m_nWhatToDo;

	CDeNoise();
	BOOL TransformPix(LONG x, LONG y);
};

#endif
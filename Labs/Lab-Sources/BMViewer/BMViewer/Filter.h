// Filter.h : interface of CFilter and derived classes
// (C) Alexey Polyakov 2002
/////////////////////////////////////////////////////////////////////////////
#ifndef _FILTER_INCLUDED
#define _FILTER_INCLUDED

class CRaster;
// Базовый виртуальный класс
class CFilter
{
protected:
	CRaster *m_pSourceBM;
	CRaster *m_pDestBM;
public:
	// Устанавливает исходное и приемное изображения
	void SetBuffers( CRaster *pSource, CRaster *pDest=NULL)
		{	m_pSourceBM=pSource;	m_pDestBM=pDest;};
	// Виртуальный метод преобразования пиксела 
	// будет переопределен в производных классах
	virtual BOOL TransformPix(LONG x, LONG y){ return FALSE;};
};

// Базовый класс для точечных фильтров
class CDotFilter: public CFilter
{
protected:
	 // Таблицы преобразования для компонент цвета
	BYTE BGRTransTable[3][256];
public:
	// Метод преобразования пиксела 
	BOOL TransformPix(LONG x, LONG y);
};

// Гистограмма
class CHistogram: public  CDotFilter
{
public:
	BOOL Init(int offset_b, int offset_t);
};

// Яркость-контраст
class CBrightCont: public  CDotFilter
{
public:
	BOOL Init(int b_offset, int c_offset);
};


// Инверсия цветов
class CInvertColors: public  CDotFilter
{
public:
	CInvertColors();
};

// Рельеф 
class CEmboss: public  CDotFilter
{
public:
	BOOL TransformPix(LONG x, LONG y);
};

// Пространственные (матричные) фильтры
// Базовый класс
class CMatrixFilter: public CFilter
{
protected:
	int m_rangX; // размер матрицы по X и Y
	int m_rangY;
	const int *m_pMatrix; // указатель на матрицу
public:
	// Метод преобразования пиксела 
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
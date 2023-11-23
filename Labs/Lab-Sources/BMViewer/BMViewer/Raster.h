// Raster.h : interface of CRaster class
// (C) Alexey Polyakov 2002-2003
/////////////////////////////////////////////////////////////////////////////
#ifndef _RASTER_INCLUDED
#define _RASTER_INCLUDED
// макрос дл€ определени€ количества байт в выровненной по DWORD строки пикселов в DIB 
// Width - длина строки в пикселах; BPP - бит на пиксел
#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * \
(DWORD)(BPP) + 31) >> 5)) << 2) 

class CRaster
{
	LPBITMAPINFO		m_pBMI;   		//указатель на описание изображени€
	PBYTE				m_pData;		//указатель на начало растровых данных
public:
	CRaster();
	~CRaster();
	void Clear(); //очистка пам€ти
	// ¬озвращает
	LPBITMAPINFO GetBMInfoPtr(){return m_pBMI;}	//указатель на заголовок растра
	RGBQUAD* GetBMColorTabPtr();				//указатель на таблицу цветов
	LONG GetBMWidth(); 							//ширину в пикселах;
	LONG GetBMHeight();							//высоту в пикселах;
	
	BYTE* GetBMDataPtr(){return m_pData;};		//указатель на растровые данные
	BYTE* GetPixPtr(LONG x, LONG y);			//указатель на пиксел 	

	// «агружает из файла
	BOOL LoadBMP(CString FileName);
	// ¬ыводит DIB на контекст pDC
	// x, y - позици€ левого верхнего угла области назначени€
	// cx, cy - размер области назначени€
	// x0, y0 - позици€ левого верхнего угла выводимой части изображени€
	// cx0, cy0 - размер выводимой части изображени€
	// str_mode - режим масштабировани€
	// rop - растрова€ операци€ определ€ет способ наложени€ изображени€
	void DrawBitmap(CDC *pDC, LONG x=0, LONG y=0, LONG cx=0, LONG cy=0,
								 	LONG x0=0, LONG y0=0, LONG cx0=0, LONG cy0=0, int str_mode=COLORONCOLOR, DWORD rop=SRCCOPY);
	// ¬ыводит DIB на контекст pDC с позиции (x,y) в масштабе scale
	void DrawBitmap(CDC *pDC, LONG x, LONG y, double scale, int str_mode=COLORONCOLOR, DWORD rop=SRCCOPY);
	
	// «аписывает BMP в файл
	BOOL SaveBMP(CString FileName);
	// —оздает копию
	BOOL CreateCopy(CRaster *pOrg);
	// —оздает растр заданного размера, совместимый с параметрами BITMAPINFO 
	BOOL CreateCompatible(LPBITMAPINFO pBMI, LONG width=0, LONG height=0);
	// ¬озвращает гисторамму изображени€
	BOOL GetHistogham(DWORD *pHist, int Range);
};
#endif
// Raster.h : interface of CRaster class
// (C) Alexey Polyakov 2002-2003
/////////////////////////////////////////////////////////////////////////////
#ifndef _RASTER_INCLUDED
#define _RASTER_INCLUDED
// ������ ��� ����������� ���������� ���� � ����������� �� DWORD ������ �������� � DIB 
// Width - ����� ������ � ��������; BPP - ��� �� ������
#define BYTESPERLINE(Width, BPP) ((WORD)((((DWORD)(Width) * \
(DWORD)(BPP) + 31) >> 5)) << 2) 

class CRaster
{
	LPBITMAPINFO		m_pBMI;   		//��������� �� �������� �����������
	PBYTE				m_pData;		//��������� �� ������ ��������� ������
public:
	CRaster();
	~CRaster();
	void Clear(); //������� ������
	// ����������
	LPBITMAPINFO GetBMInfoPtr(){return m_pBMI;}	//��������� �� ��������� ������
	RGBQUAD* GetBMColorTabPtr();				//��������� �� ������� ������
	LONG GetBMWidth(); 							//������ � ��������;
	LONG GetBMHeight();							//������ � ��������;
	
	BYTE* GetBMDataPtr(){return m_pData;};		//��������� �� ��������� ������
	BYTE* GetPixPtr(LONG x, LONG y);			//��������� �� ������ 	

	// ��������� �� �����
	BOOL LoadBMP(CString FileName);
	// ������� DIB �� �������� pDC
	// x, y - ������� ������ �������� ���� ������� ����������
	// cx, cy - ������ ������� ����������
	// x0, y0 - ������� ������ �������� ���� ��������� ����� �����������
	// cx0, cy0 - ������ ��������� ����� �����������
	// str_mode - ����� ���������������
	// rop - ��������� �������� ���������� ������ ��������� �����������
	void DrawBitmap(CDC *pDC, LONG x=0, LONG y=0, LONG cx=0, LONG cy=0,
								 	LONG x0=0, LONG y0=0, LONG cx0=0, LONG cy0=0, int str_mode=COLORONCOLOR, DWORD rop=SRCCOPY);
	// ������� DIB �� �������� pDC � ������� (x,y) � �������� scale
	void DrawBitmap(CDC *pDC, LONG x, LONG y, double scale, int str_mode=COLORONCOLOR, DWORD rop=SRCCOPY);
	
	// ���������� BMP � ����
	BOOL SaveBMP(CString FileName);
	// ������� �����
	BOOL CreateCopy(CRaster *pOrg);
	// ������� ����� ��������� �������, ����������� � ����������� BITMAPINFO 
	BOOL CreateCompatible(LPBITMAPINFO pBMI, LONG width=0, LONG height=0);
	// ���������� ���������� �����������
	BOOL GetHistogham(DWORD *pHist, int Range);
};
#endif
// Raster.cpp : implementation of the CRaster class
// (C) Alexey Polyakov 2002
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Raster.h"

CRaster::CRaster()
{
	m_pData=NULL;
	m_pBMI=NULL;
}

CRaster::~CRaster()
{
	Clear();
};

void CRaster::Clear()
{

	if(m_pData!=NULL) delete[] m_pData; 
	m_pData=NULL;

	if(m_pBMI!=NULL) delete[] m_pBMI; 
	m_pBMI=NULL;
	
}; 

RGBQUAD* CRaster::GetBMColorTabPtr()
{
	return(LPRGBQUAD)(((BYTE*)(m_pBMI))+sizeof(BITMAPINFOHEADER));
};


LONG CRaster::GetBMWidth()
{
	if(m_pBMI==NULL) return 0;
	return m_pBMI->bmiHeader.biWidth;
};

LONG CRaster::GetBMHeight()
{
	if(m_pBMI==NULL) return 0;
	return m_pBMI->bmiHeader.biHeight;
};

BYTE* CRaster::GetPixPtr(LONG x, LONG y)
{
	if(	x<0 || x>= m_pBMI->bmiHeader.biWidth ||
		y<0 || y>= m_pBMI->bmiHeader.biHeight ||
		m_pData == NULL)
		return NULL;
	return (m_pData+(BYTESPERLINE(m_pBMI->bmiHeader.biWidth, m_pBMI->bmiHeader.biBitCount)*y +
			x*m_pBMI->bmiHeader.biBitCount/8));
};

BOOL CRaster::LoadBMP(CString FileName)
{
	//�������
	Clear();
	//��������� ����
	CFile File;
	if(!File.Open(FileName, CFile::modeRead)) return FALSE; 
	
	////////////////////////////////////////////////////////
	//��������� �����������
	//��������� ��������� �����. ��� ���� ��� ������ � ���������
	//������ ������
	BITMAPFILEHEADER	FI;
	File.Read(&FI, sizeof(BITMAPFILEHEADER));
	
	//��������� Windows Bitmap ����������� ?
	if(FI.bfType!=0x4D42)
	{ File.Close(); return FALSE;}

	//������� �������
	File.Seek(sizeof(BITMAPFILEHEADER), CFile::begin);
	
	//�������, ��� ��� �� ��������� ����� �� ������ ��������� ������
	//���� BITMAPINFO
	//�������� ������ ��� ���������
	m_pBMI=(LPBITMAPINFO)new BYTE[FI.bfOffBits-sizeof(BITMAPFILEHEADER)];
	if(m_pBMI==NULL) { File.Close(); return FALSE;}
	//������ BITMAPINFO 
	File.Read(m_pBMI, FI.bfOffBits-sizeof(BITMAPFILEHEADER));
	
	//����� �������� ������ � ��������� �������
	if(m_pBMI->bmiHeader.biCompression!=0) 
		{ File.Close(); return FALSE;}

	//������� � ������ ������
	File.Seek(FI.bfOffBits, CFile::begin);
	//�������� ������ ��� ������
	//������ �������
	if(m_pBMI->bmiHeader.biSizeImage==0)
		m_pBMI->bmiHeader.biSizeImage=BYTESPERLINE(	m_pBMI->bmiHeader.biWidth,
													m_pBMI->bmiHeader.biBitCount)*
													m_pBMI->bmiHeader.biHeight;	

	m_pData= new BYTE[m_pBMI->bmiHeader.biSizeImage]; 
	if(m_pData==NULL) { File.Close(); return FALSE;}
	//������ ������ 
	File.Read(m_pData, m_pBMI->bmiHeader.biSizeImage);

	File.Close();
	return TRUE;
};


void CRaster::DrawBitmap(CDC *pDC,	LONG x/*=0*/, LONG y/*=0*/, LONG cx/*=0*/, LONG cy/*=0*/,
									LONG x0/*=0*/, LONG y0/*=0*/, LONG cx0/*=0*/, LONG cy0/*=0*/,
									int str_mode/*=COLORONCOLOR*/, DWORD rop /*=SRCCOPY*/)
{
	if(m_pBMI==NULL || m_pData==NULL) return;
	//������� �� ������ - �������� � ��������
	if(cx==0) cx=GetBMWidth();
	if(cy==0) cy=GetBMHeight();
	
	if(cx0==0) cx0=GetBMWidth();
	if(cy0==0) cy0=GetBMHeight();
	HDC  hdc=pDC->GetSafeHdc();
	if(hdc==NULL) return;
	// ��������� ������ ���������������
	int oldStretchMode=pDC->SetStretchBltMode(str_mode);
	::StretchDIBits(hdc,
		x, 	y,	cx,	cy,			// ������� ����������
		x0, y0,	cx0, cy0,		// ������ � ������ ���������� �����������
		m_pData,				// ������
		m_pBMI,					// ���������
		DIB_RGB_COLORS,			// �����
		rop);					// ��� ���
	if(oldStretchMode!=0)
		pDC->SetStretchBltMode(oldStretchMode);
};

void CRaster::DrawBitmap(CDC *pDC,	LONG x, LONG y, double scale,
						 int str_mode/*=COLORONCOLOR*/, DWORD rop /*=SRCCOPY*/)
{
	if(m_pBMI==NULL || m_pData==NULL) return;
	
	LONG x0=0, y0=0;
	
	LONG cx0=GetBMWidth();
	LONG cy0=GetBMHeight();

	LONG cx=static_cast<LONG>(scale*cx0+0.5);
	LONG cy=static_cast<LONG>(scale*cy0+0.5);

	DrawBitmap(pDC,	x, y, cx, cy, x0, y0, cx0, cy0, str_mode, rop);
};


BOOL CRaster::SaveBMP(CString FileName)
{
	//��������� ����
	CFile File;
	if(!File.Open(FileName, CFile::modeCreate|CFile::modeWrite)) return FALSE; 
	
	////////////////////////////////////////////////////////
	//���������� �����������
	//�������� ������ ��������� ������ ������ � �������� ������
	DWORD SizeOfBMI= (DWORD)m_pBMI->bmiHeader.biSize + 
	m_pBMI->bmiHeader.biClrUsed*sizeof(RGBQUAD);
	
	// ��������� �����
	BITMAPFILEHEADER	FI;
	// ������������� ���� ����� BMP: 0x42 = "B" 0x4d = "M"  
    FI.bfType = 0x4d42;        
    // ������ ����� ����� ������ � ����������� � �������
    FI.bfSize = (DWORD) sizeof(BITMAPFILEHEADER) + SizeOfBMI + 
				m_pBMI->bmiHeader.biSizeImage; 
    FI.bfReserved1 = 0; 
    FI.bfReserved2 = 0; 
    // ��������� �������� �� ������ ��������� ������
    FI.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + SizeOfBMI; 
	// ���������� ��������� ����� 
	File.Write(&FI, sizeof(BITMAPFILEHEADER));
	
		
	//���������� BITMAPINFO ������ � �������� ������
	File.Write(m_pBMI, SizeOfBMI);
	
	//������ 
	File.Write(m_pData, m_pBMI->bmiHeader.biSizeImage);
	
	File.Close();
	return TRUE;
};

BOOL CRaster::CreateCopy(CRaster *pOrg)
{
	Clear();
	if(!pOrg) return FALSE;

	LPBITMAPINFO pOrgBMI=pOrg->GetBMInfoPtr();
	//�������� ������ ��������� ������ ������ � �������� ������
	DWORD SizeOfBMI= (DWORD)pOrgBMI->bmiHeader.biSize + 
	pOrgBMI->bmiHeader.biClrUsed*sizeof(RGBQUAD);

	// ������� ������ ��� ��������� ������
	m_pBMI=(LPBITMAPINFO)new BYTE[SizeOfBMI];
	if(!m_pBMI) return FALSE;
	// �������� ��������� ������
	memcpy(m_pBMI, pOrg->GetBMInfoPtr(), SizeOfBMI);

	// ������ ������� ������ ��� ������
	if(m_pBMI->bmiHeader.biSizeImage==0)
		m_pBMI->bmiHeader.biSizeImage=BYTESPERLINE(	m_pBMI->bmiHeader.biWidth,
													m_pBMI->bmiHeader.biBitCount)*
													m_pBMI->bmiHeader.biHeight;	
	// �������� ������ ��� ������
	m_pData= new BYTE[m_pBMI->bmiHeader.biSizeImage]; 
	if(!m_pData) return FALSE;

	// �������� ������
	memcpy(m_pData, pOrg->GetBMDataPtr(), m_pBMI->bmiHeader.biSizeImage);

	return TRUE;
};

BOOL CRaster::CreateCompatible(LPBITMAPINFO pBMI,	LONG width/*=0*/,
													LONG height/*=0*/)
{
	if(!pBMI) return FALSE;

	if(width==0)	width=pBMI->bmiHeader.biWidth;
	if(height==0)	height=pBMI->bmiHeader.biHeight;

	// ��������� ����� ������������ ����� � ��� ���������
	if( m_pBMI!=NULL &&							// ����������
		m_pBMI->bmiHeader.biWidth==width &&		// ������ �� �������
		m_pBMI->bmiHeader.biHeight==height &&	// � ������� ����� ���������
		m_pBMI->bmiHeader.biBitCount==pBMI->bmiHeader.biBitCount
		)
		return TRUE; // ����� � ��� ���������
	////////////////////////////////////////////////////////
	// ������� ����������� �����
	Clear();
	//�������� ������ ��������� ������ ������ � �������� ������
	DWORD SizeOfBMI= (DWORD) pBMI->bmiHeader.biSize + 
					  pBMI->bmiHeader.biClrUsed*sizeof(RGBQUAD);

 	// ������� ������ ��� ��������� ������
	m_pBMI=(LPBITMAPINFO)new BYTE[SizeOfBMI];
	if(!m_pBMI) return FALSE;
	
	// �������� ��������� ������
	memcpy(m_pBMI, pBMI, SizeOfBMI);

	// ������������� ������ 
	m_pBMI->bmiHeader.biWidth=width;
	m_pBMI->bmiHeader.biHeight=height;

	// ������ ������� ������ ��� ������
	m_pBMI->bmiHeader.biSizeImage=BYTESPERLINE(	m_pBMI->bmiHeader.biWidth,
												m_pBMI->bmiHeader.biBitCount)*
												m_pBMI->bmiHeader.biHeight;	
	// �������� ������ ��� ������
	m_pData= new BYTE[m_pBMI->bmiHeader.biSizeImage]; 
	if(!m_pData) return FALSE;

	return TRUE;
};

BOOL CRaster::GetHistogham(DWORD *pHist, int Range)
{
	// ����� �������� ������ � ������� RGB888
	if(m_pBMI->bmiHeader.biBitCount!=24) return FALSE;
	
	// ������� �������
	for(int i=0; i<Range; i++)
		pHist[i]=0;

	LONG DataStrLength=BYTESPERLINE(m_pBMI->bmiHeader.biWidth, m_pBMI->bmiHeader.biBitCount);
	BYTE *pCurPix=NULL;
	BYTE Brightness=0;
	for(int y=0, x=0; y<m_pBMI->bmiHeader.biHeight; y++)
		for(x=0; x<m_pBMI->bmiHeader.biWidth; x++)
	{
		// ����� �������
		pCurPix=m_pData+y*DataStrLength+x*3;
	
		// ������� �������������� ��� 0.3*Red+0.59*Green+0.11*Blue,
		// �� ���������� ������ �������� � ����� BMP, � ������� BGR 
		Brightness=(BYTE)((	0.11*(*pCurPix) + 
							0.59*(*(pCurPix+1))	+
							0.3*(*(pCurPix+2)))*Range/256);
		pHist[Brightness]+=1;
	}
	return TRUE;
};


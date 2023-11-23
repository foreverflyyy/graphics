// Filter.cpp : implementation of CFilter and derived classes
// (C) Alexey Polyakov 2002
////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Filter.h"
#include "Raster.h"
#include <math.h>


BOOL CDotFilter::TransformPix(LONG x, LONG y)
{
	BYTE *pDPix=NULL, *pSPix=NULL;
	// �������� ���������
	if(	m_pSourceBM==NULL )
		return FALSE;
	// ���� �������� �� �����, �� �������������� �������� � ��������
	if(	m_pDestBM==NULL)
		m_pDestBM=m_pSourceBM;
	// �������� ��������� �� ������� � ��������� � ���������
	if(	(pDPix=m_pDestBM->GetPixPtr(x, y))==NULL ||
		(pSPix=m_pSourceBM->GetPixPtr(x, y))==NULL)
		return FALSE;
	// ��������������. ������� BGR
	*pDPix=BGRTransTable[0][*pSPix];
	*(pDPix+1)=BGRTransTable[1][*(pSPix+1)];
	*(pDPix+2)=BGRTransTable[2][*(pSPix+2)];
	return TRUE;
};

BOOL CHistogram::Init(int offset_b, int offset_t)
{
	int range=0;
    // ��� �������� � �������� � �������� �� 0 �� ������ �������
    // ��������� � 0
	for(int i=0, t=0; t<3; t++)
		for(i=0; i<offset_b; i++)
		{
			BGRTransTable[t][i]=0;
		}
    // ��� �������� � �������� � �������� �� 255 �� ������� �������
    // ��������� � 255
	for(int t=0; t<3; t++)
		for(int i=255; i>=256-offset_t; i--)
		{
			BGRTransTable[t][i]=255;
		}
    // ��� �������� � �������� � �������� �� ������ �� ������� �������
    // ���������� ����������� �� �������� �� 0 �� 255
	double step=256./(256-(offset_b+offset_t));
	for(int t=0; t<3; t++)
	{
		double value=0.;
		for(int i=offset_b; i<256-offset_t; i++)
		{
			BGRTransTable[t][i]=(int)((value)+0.5);
			value+=step;
		}
	}
	return TRUE;
};

// "����� �������"
#define CONTRAST_MEDIAN 159
BOOL CBrightCont::Init(int b_offset, int c_offset)
{
	int i=0,	// ������ ����� � ������� ��������������
		t=0,	// ������ �������
		t_index=0, // ������ �����, ���������������� ������ ������� �������
		b_index=0, // ������ �����, ���������������� ������� ������� �������
		value_offset; // �������� �������� �����
	double value=0.; // ����� �������� �����
	// �������� �������
	for(i, t=0; t<3; t++)
		for(i=0; i<256; i++)
		{
			if( i+b_offset>255) BGRTransTable[t][i]=255;
			else if( i+b_offset<0) BGRTransTable[t][i]=0;
			else BGRTransTable[t][i]=i+b_offset;
		}
	// �������� �������������
	if(c_offset<0)// ��������� �������������
	{
		for(i=0, t=0; t<3; t++)
		for(i=0; i<256; i++)
		if(BGRTransTable[t][i]<CONTRAST_MEDIAN)
		{
			// ����������� �������� � ����������� �� ����������� ����� ��
			// "�����" ������� "�����"
			value_offset=(CONTRAST_MEDIAN-BGRTransTable[t][i])*c_offset/128;
			if(BGRTransTable[t][i]-value_offset>CONTRAST_MEDIAN) BGRTransTable[t][i]=CONTRAST_MEDIAN;
				else BGRTransTable[t][i]-=value_offset;
		}
		else
		{
			// ����������� �������� � ����������� �� ����������� ����� ��
			// "�����" ������� "������"
			value_offset=(BGRTransTable[t][i]-CONTRAST_MEDIAN)*c_offset/128;
			if(BGRTransTable[t][i]+value_offset<CONTRAST_MEDIAN) BGRTransTable[t][i]=CONTRAST_MEDIAN;
				else BGRTransTable[t][i]+=value_offset;
		}
	}
	else	if(c_offset>0)
	//����������� �������������
	{
		// ������ ������ ������� �����
		int offset_b=c_offset*CONTRAST_MEDIAN/128;
		// ��� �������� � ������� ���� ������ ������� ������� �������� 0
		for(t=0; t<3; t++)
		for(b_index=0; b_index<256; b_index++)
		{
			if(BGRTransTable[t][b_index]<offset_b)
				BGRTransTable[t][b_index]=0;
			else break;
		}
		// ������ ������� ������� �����
		int offset_t=c_offset*128/CONTRAST_MEDIAN;
		// ��� �������� ���� ������� ������� ������� �������� 255
		for(t=0; t<3; t++)
		for(t_index=255; t_index>=0; t_index--)
		{
			if(	BGRTransTable[t][t_index]+offset_t>255)
				BGRTransTable[t][t_index]=255;
			else break;
		}
		// ������ ���� ��������� ������������� �����
		double step=256./(256-(offset_b+offset_t));
		// "�����������" ������������� ������ ����� ������ � ������� ���������,
		// ����� ��� �������� ���� �������� �� 0 �� 255
		for(t=0; t<3; t++)
		{
			value=0.;
			for(i=b_index; i<=t_index; i++)
			{
				if(BGRTransTable[t][i]>=offset_b || BGRTransTable[t][i]<256-offset_t)
				{
					value=(int)((BGRTransTable[t][i]-offset_b)*step+0.5);	
					if(value>255) value=255;
					BGRTransTable[t][i]=(int)(value);
				}
			}
		}
	}

	return TRUE;
};

CInvertColors::CInvertColors()
{
	for(int i=0, t=0; t<3; t++)
		for(i=0; i<256; i++)
		{
			BGRTransTable[t][i]=255-i;
		}
};
#define STONE_OFFSET_X 3
#define STONE_OFFSET_Y -3
BOOL CEmboss::TransformPix(LONG x, LONG y)
{
	BYTE *pDPix=NULL, *pSPix1=NULL, *pSPix2=NULL;
	// �������� � �������� ����������
	if(	m_pSourceBM==NULL || m_pDestBM==NULL )
		return FALSE;
	
	// �������� ��������� �� ������� � ��������� � ���������
	if(	(pDPix=m_pDestBM->GetPixPtr(x, y))==NULL || 
		(pSPix1=m_pSourceBM->GetPixPtr(x, y))==NULL)  return FALSE;

	if(	(pSPix2=m_pSourceBM->GetPixPtr(x+STONE_OFFSET_X, y+STONE_OFFSET_Y))==NULL)
		pSPix2=pSPix1;
	// ������ �������	
	BYTE Y1, Y2;
	Y1=(BYTE)(0.11*(*pSPix1) + 0.59*(*(pSPix1+1)) + 0.3*(*(pSPix1+2)));
	Y2=(BYTE)(0.11*(*pSPix2) + 0.59*(*(pSPix2+1)) + 0.3*(*(pSPix2+2)));
	// ������� ������� � ������� �� � ����� �������
	BYTE d=(Y1-Y2+255)/2;

	// ������ �������� ����� ��������
	*pDPix=d;
	*(pDPix+1)=d;
	*(pDPix+2)=d;
	return TRUE;
};

// ���������������� �������

BOOL CMatrixFilter::TransformPix(LONG x, LONG y)
{
	BYTE *pDPix=NULL, *pSPix=NULL;
	// �������� � �������� ����������
	if(	m_pSourceBM==NULL || m_pDestBM==NULL)
		return FALSE;

	// ���������� ���� ���������� ����������� � 
	// ������� ��������������. ��� ��������� ���
	// ��������� ��������, ����������� �� �������� 
	// �����������
	int x_start=0;
	int dx=m_rangX/2, dy=m_rangY/2;

	if(x-dx<0) x_start=dx-x;

	int y_start=0;
	if(y-dy<0) y_start=dy-y;

	int x_finish=m_rangX;
	if(x+dx>m_pSourceBM->GetBMWidth()) 
		x_finish-=(x+dx-m_pSourceBM->GetBMWidth());

	int y_finish=m_rangY;
	if(y+dy>m_pSourceBM->GetBMHeight() ) 
		y_finish-=(y+dy-m_pSourceBM->GetBMHeight());
	
	// ������ ����� �������� ����� �������
	// � ������ �������, �������� � ���� �������� 
	// ������� ��������������
	int NewBGR[3];
	int count=0;
	for(int c=0, mx=0, my=0; c<3; c++)
	{
		NewBGR[c]=0; count=0;
		for(my=y_start; my<y_finish; my++)
		for(mx=x_start; mx<x_finish; mx++)
		{
			if((pSPix=m_pSourceBM->GetPixPtr(x+(mx-dx),
											 y+(my-dy)))!=NULL)
			 {
				NewBGR[c]+=(m_pMatrix[my*m_rangX+mx]*(*(pSPix+c)));
				count+=m_pMatrix[my*m_rangX+mx];
			 }
				
		}
	}
	// ����� ������� � �����������-���������
	pDPix=m_pDestBM->GetPixPtr(x, y);
	
	// ��������� ������ �������� � �������� �����������
	for(int c=0; c<3; c++)
	{
		// ���������� �������� � ����������� ���������
		if(count!=0)
			NewBGR[c]=NewBGR[c]/count;
		if(NewBGR[c]<0)
			NewBGR[c]=0;
		else if(NewBGR[c]>255)
			NewBGR[c]=255;
			
		*(pDPix+c)=NewBGR[c];
	}
	
	return TRUE;
};


const int BlurMatrix[25]=
	{	1,	1,	1, 1,	1,
		1,	1,	1, 1,	1,
		1,	1,	1, 1,	1,
		1,	1,	1, 1,	1,
		1,	1,	1, 1,	1
	};

// ����������� �������� ������
// ��� ����� ����� �������� � �������
#define CONTOUR_COEFF 3
const int ConturMatrix[9]=
	{	-1*CONTOUR_COEFF,	-1*CONTOUR_COEFF,	-1*CONTOUR_COEFF,
		-1*CONTOUR_COEFF,	8*CONTOUR_COEFF,	-1*CONTOUR_COEFF,
		-1*CONTOUR_COEFF,	-1*CONTOUR_COEFF,	-1*CONTOUR_COEFF
	};


CBlur::CBlur()
{
	m_pMatrix=BlurMatrix;
	m_rangX=5;
	m_rangY=5;
};

CContour::CContour()
{
	m_pMatrix=ConturMatrix;
	m_rangX=3;
	m_rangY=3;
}


CSharp::CSharp()
{
	m_pMatrix=BlurMatrix;
	m_rangX=5;
	m_rangY=5;
};
// ����������� ���������� ��������
// ��� ������ ����� �������� � �������
#define SHARP_COEFF 3
BOOL CSharp::TransformPix(LONG x, LONG y)
{
	// ������� ������
	if(!CMatrixFilter::TransformPix(x, y))
		return FALSE;
	BYTE *pDPix=NULL, *pSPix=NULL;
	
	pSPix=m_pSourceBM->GetPixPtr(x,y);
	pDPix=m_pDestBM->GetPixPtr(x, y);
	int d=0;
	for(int c=0; c<3; c++)
	{
		// ����� �������
		d=*(pSPix+c)-*(pDPix+c);
		// ������� �������
		d*=SHARP_COEFF;
		// ��������� ������� ����� ��������
		if(*(pDPix+c)+d <0)	
			*(pDPix+c)=0;
		else
			if(*(pDPix+c)+d > 255)
				*(pDPix+c)=255;
		else 
				*(pDPix+c)+=d;
		
	}
	return TRUE;
}
//Brightness
BYTE Y(PBYTE pPix);

CDeNoise::CDeNoise()
{
	m_pMatrix=NULL;
	m_rangX=5;
	m_rangY=5;

	m_dK=2.0;
	m_nWhatToDo=0;
};

BOOL CDeNoise::TransformPix(LONG x, LONG y)
{

	BYTE *pDPix=NULL, *pSPix=NULL;
	// �������� � �������� ����������
	if(	m_pSourceBM==NULL || m_pDestBM==NULL)
		return FALSE;

	// ���������� ���� ���������� ����������� � 
	// ������� ��������������. ��� ��������� ���
	// ��������� ��������, ����������� �� �������� 
	// �����������
	int x_start=0;
	int dx=m_rangX/2, dy=m_rangY/2;
	
	if(x-dx<0) x_start=dx-x;

	int y_start=0;
	if(y-dy<0) y_start=dy-y;

	int x_finish=m_rangX;
	if(x+dx>m_pSourceBM->GetBMWidth()) 
		x_finish-=(x+dx-m_pSourceBM->GetBMWidth());

	int y_finish=m_rangY;
	if(y+dy>m_pSourceBM->GetBMHeight() ) 
		y_finish-=(y+dy-m_pSourceBM->GetBMHeight());
	
	// ������� ������� �������� �������
	int mx=0, my=0; 
	int avgY=0, count=0;

	for(my=y_start; my<y_finish; my++)
		for(mx=x_start; mx<x_finish; mx++)
		{
			if((pSPix=m_pSourceBM->GetPixPtr(x+(mx-dx),
											 y+(my-dy)))!=NULL)
			{
				avgY+=Y(pSPix);
				count++;
			}
		}

	// ������� ����� ���������� �� ��������	
	int sumVar=0;
	for(my=y_start; my<y_finish; my++)
		for(mx=x_start; mx<x_finish; mx++)
		{
			if((pSPix=m_pSourceBM->GetPixPtr(x+(mx-dx),
											 y+(my-dy)))!=NULL)
			{
				sumVar+=abs(avgY-count*Y(pSPix));
			}
		}

	// ����� ������� � �����������-���������
	pSPix=m_pSourceBM->GetPixPtr(x, y);
	double Pi= sumVar>0?abs(avgY-count*Y(pSPix))/sumVar:0;
	
	// ����� ������� � �����������-���������
	pDPix=m_pDestBM->GetPixPtr(x, y);
	BYTE NewValue=255;
	
	int NewBGR[3], count2=0; 

	switch(m_nWhatToDo)
	{
	case 0: //�������� ����
			if(Pi> m_dK/count) // ���
			{
				NewBGR[0]=NewBGR[1]=NewBGR[2]=0;
				count2=0;
				
				// ������� ��������� �������� "�� �������" ��������
				for(my=y_start; my<y_finish; my++)
					for(mx=x_start; mx<x_finish; mx++)
					{
						if((pSPix=m_pSourceBM->GetPixPtr(x+(mx-dx),	y+(my-dy)))!=NULL &&
						  ((sumVar>0?abs(avgY-count*Y(pSPix))/sumVar:0)<= m_dK/count))
						{
							for(int c=0; c<3; c++)	NewBGR[c]+=*(pSPix+c);
							count2++;
						}
					}
				// �������� ��� ����� ��������� 
				for(int c=0; c<3; c++)	*(pDPix+c)=NewBGR[c]/count2;
			}
			else // �� ���
				//  ������ �������� �������� �������
				for(int c=0; c<3; c++)	*(pDPix+c)=*(pSPix+c);
	break;
	case 1:   // ��������� ����
		if(Pi> m_dK*1.0/count) // noise pixel - select
				NewValue=128;
		for(int c=0; c<3; c++)	*(pDPix+c)=NewValue;
	break;
	}
		
	return TRUE;
}

//Brightness
BYTE Y(PBYTE pPix)
{
	return( (BYTE)(0.11*(*pPix) + 0.59*(*(pPix+1)) + 0.3*(*(pPix+2))) );
}
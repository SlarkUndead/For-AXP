#ifndef _MFC_STRUCTS_060701_H_
#define _MFC_STRUCTS_060701_H_

//////////////////////////////////////////////////////////////////////////
//
// ���������ݽṹ
//
//////////////////////////////////////////////////////////////////////////
#ifndef WIN32 // ��û����ʱ�Զ�����������
	typedef unsigned char       BYTE;
	typedef unsigned short      WORD;
	typedef unsigned long       DWORD;
	typedef long                BOOL;
	typedef unsigned int        UINT;
	typedef long                LONG;
	typedef signed int          INT32;
	typedef void *              HANDLE;
	typedef void *              HDC;

	typedef struct tagRECT
	{
		long left;
		long top;
		long right;
		long bottom;
	} RECT;

	typedef struct tagPOINT
	{
		long x;
		long y;
	} POINT;

	typedef struct tagSIZE
	{
		long cx;
		long cy;
	} SIZE;
	//////////////////////////////////////////////////////////////////////////
	//
	// FreeImage
	//
	#ifndef FREEIMAGE_H // ���û�а���FreeImage.h���Զ���������ɫ�Ľṹ
		typedef struct tagRGBQUAD
		{
			BYTE    rgbBlue;
			BYTE    rgbGreen;
			BYTE    rgbRed;
			BYTE    rgbReserved;
		} RGBQUAD;

		typedef struct tagBITMAPINFOHEADER
		{
			DWORD      biSize;
			LONG       biWidth;
			LONG       biHeight;
			WORD       biPlanes;
			WORD       biBitCount;
			DWORD      biCompression;
			DWORD      biSizeImage;
			LONG       biXPelsPerMeter;
			LONG       biYPelsPerMeter;
			DWORD      biClrUsed;
			DWORD      biClrImportant;
		} BITMAPINFOHEADER ;
	#endif // FREEIMAGE_H
#endif // WIN32
//=============================================================================
//
// ������
//
typedef struct tagRECT_F // ������RECT
{
	double   left ;
	double   top ;
	double   right ;
	double   bottom ;
} RECT_F ;

typedef struct tagPOINT_F // ������POINT
{
	double   x ;
	double   y ;
} POINT_F ;

typedef struct tagSIZE_F // ������SIZE
{
	double   cx ;
	double   cy ;
} SIZE_F ;

//=============================================================================
//	��Ӱ���ݽṹ
typedef struct tagSHADOWDATA
{
	INT32       nSmooth ;	// ģ����
	RGBQUAD     crShadow ;	// ��ɫ/Ӳ��
	INT32       nAlpha ;	// ͸����
	INT32       nOffsetX ;	// Xƫ��
	INT32       nOffsetY ;	// Yƫ��
} SHADOWDATA, * PSHADOWDATA ;

#endif
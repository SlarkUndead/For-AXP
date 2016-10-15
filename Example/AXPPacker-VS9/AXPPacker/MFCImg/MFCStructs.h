#ifndef _MFC_STRUCTS_060701_H_
#define _MFC_STRUCTS_060701_H_

//////////////////////////////////////////////////////////////////////////
//
// 基本的数据结构
//
//////////////////////////////////////////////////////////////////////////
#ifndef WIN32 // 在没定义时自定义以下数据
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
	#ifndef FREEIMAGE_H // 如果没有包含FreeImage.h则自定义容纳颜色的结构
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
// 浮点数
//
typedef struct tagRECT_F // 浮点数RECT
{
	double   left ;
	double   top ;
	double   right ;
	double   bottom ;
} RECT_F ;

typedef struct tagPOINT_F // 浮点数POINT
{
	double   x ;
	double   y ;
} POINT_F ;

typedef struct tagSIZE_F // 浮点数SIZE
{
	double   cx ;
	double   cy ;
} SIZE_F ;

//=============================================================================
//	阴影数据结构
typedef struct tagSHADOWDATA
{
	INT32       nSmooth ;	// 模糊度
	RGBQUAD     crShadow ;	// 颜色/硬度
	INT32       nAlpha ;	// 透明度
	INT32       nOffsetX ;	// X偏移
	INT32       nOffsetY ;	// Y偏移
} SHADOWDATA, * PSHADOWDATA ;

#endif
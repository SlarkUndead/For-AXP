#ifndef _MFC_STANDARD_DEFINE_060701_H_
#define _MFC_STANDARD_DEFINE_060701_H_

//////////////////////////////////////////////////////////////////////////
//
// 宏定义
//
//////////////////////////////////////////////////////////////////////////
#undef FALSE
#undef TRUE
#undef NULL
#undef BI_RGB
#undef BI_BITFIELDS
#define FALSE   0
#define TRUE    1
#define NULL    0
#define BI_RGB        0
#define BI_BITFIELDS  3

#define	MFC_PI	3.1415926535897932384626433832795
#define	MFC_2PI	(2.0*MFC_PI)

//=============================================================================
#define	MFC_R(p)	(((RGBQUAD*)(p))->rgbRed)
#define	MFC_G(p)	(((RGBQUAD*)(p))->rgbGreen)
#define	MFC_B(p)	(((RGBQUAD*)(p))->rgbBlue)
#define	MFC_A(p)	(((RGBQUAD*)(p))->rgbReserved)

//=============================================================================
//	16 位色的掩码
//=============================================================================
#define	MFC_MASK16_RED_565			0xF800
#define	MFC_MASK16_GREEN_565		0x07E0
#define	MFC_MASK16_BLUE_565			0x001F
#define	MFC_MASK16_RED_555			0x7C00
#define	MFC_MASK16_GREEN_555		0x03E0
#define	MFC_MASK16_BLUE_555			0x001F


#endif

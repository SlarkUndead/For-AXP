
#ifndef _MFC_COLOR_060701_H_
#define _MFC_COLOR_060701_H_

#include "MFCStdDefine.h"
#include "MFCBaseTemplate.h"
#include "MFCBaseFunc.h"


#define FLT_EPSILON 1.192092896e-07F

//=============================================================================
/**
*  Color helper class.
*  all function has no param check.
*/
class CMFCColor
{
public:
	/**
	*  Combine two pixel, result put into pDest.
	*  @param pDest : must be 32bpp
	*  @param cr1 : background
	*  @param cr2 : foreground
	*  @param nAlpha2 : alpha value of  cr2
	*/
	static void CombineAlphaPixel(RGBQUAD* pDest, RGBQUAD cr1,RGBQUAD cr2,BYTE nAlpha2) ;

	/// AlphaBlend two pixel, result put into pDest.
	static void AlphaBlendPixel(BYTE* pDest,const BYTE* pSrc,BYTE nAlphaSrc) ;

	/// Calculate pixel's grayscale value.
	static BYTE GetGrayscale(const void* prgb) ;

	/** 
	*  Rapid pixel copy.
	*  @param nBytes : can be 1,2,3,4
	*/
	static void CopyPixel(void* pDest,const void* pSrc,int nBytes) ;


	// RGB ==> HLS(Hue, Lightness, Saturation)
	static void RGBtoHLS(const void* prgb, double* H, double* L, double* S) ;
	// HLS(Hue, Lightness, Saturation) ==> RGB
	static RGBQUAD HLStoRGB(const double& H, const double& L, const double& S) ;

	// RGB ==> HSV(Hue, Saturation, Value).
	static void  RGBtoHSV(const void* prgb, double* H, double* S, double* V) ;
	// HSV(Hue, Saturation, Value) ==> RGB
	static RGBQUAD HSVtoRGB(double H, const double& S, const double& V) ;


	/// Ë«ÏßÐÔ²åÖµComputes bilinear interpolation of four pixels.
	/// The pixels in 'crPixel' in the following (x,y) order: [0,0], [1,0], [0,1], [1,1].
	static RGBQUAD GetBilinearPixel(double x,double y,BOOL bHasAlpha,const BYTE*crPixel[4]);
	
	static RGBQUAD GetMFCColorBlack(void){return MFC_RGBA(0,0,0);}			// Color black.
	static RGBQUAD GetMFCColorWhite(void){return MFC_RGBA(0xFF,0xFF,0xFF);}	// Color white.
	static RGBQUAD GetMFCColorRed(void){return MFC_RGBA(0xFF,0,0);}			// Color red.
	static RGBQUAD GetMFCColorGreen(void){return MFC_RGBA(0,0xFF,0);}		// Color green.
	static RGBQUAD GetMFCColorBlue(void){return MFC_RGBA(0,0,0xFF);}		// Color blue.

private:
	static double HLS_Value(const double& m1,const double& m2,double h) ;
	static bool IsRGBEqual(const void* p1, const void* p2) ;
	static RGBQUAD DoubleRGBToRGB (const double& r, const double& g, const double& b) ;
};

//=============================================================================
// inline implement
//=============================================================================
inline BYTE CMFCColor::GetGrayscale(const void* prgb)
{
	return (30*MFC_R(prgb) + 59*MFC_G(prgb) + 11*MFC_B(prgb))/100 ;
}
//-----------------------------------------------------------------------------
// Rapid pixel copy.
__forceinline void CMFCColor::CopyPixel(void* pDest, const void* pSrc, int nBytes)
{
	switch(nBytes)
	{
		case 1:
		{
			*(BYTE*)pDest = *(BYTE*)pSrc ;
			break;
		}
		case 2:
		{
			*(WORD*)pDest = *(WORD*)pSrc ;
			break;
		}
		case 3:
		{
			*(WORD*)pDest = *(WORD*)pSrc ;
			((BYTE*)pDest)[2] = ((BYTE*)pSrc)[2] ;
			break;
		}
		case 4:
		{
			*(DWORD*)pDest = *(DWORD*)pSrc ;
			break;
		}
		default:
		{
			memcpy(pDest, pSrc, nBytes) ;
			break;
		}
	}
}
//-----------------------------------------------------------------------------
// Combine two pixel, result put into pDest.
__forceinline void CMFCColor::CombineAlphaPixel(RGBQUAD* pDest,RGBQUAD cr1,RGBQUAD cr2,BYTE nAlpha2)
{
	if (MFC_A(&cr1) || nAlpha2)
	{
		if (nAlpha2 == 0)
		{
			*pDest = cr1 ;
			return ;
		}
		if ((MFC_A(&cr1) == 0) || (nAlpha2 == 0xFF))
		{
			*pDest = cr2 ;
			MFC_A(pDest) = nAlpha2 ;
			return ;
		}
		// needn't bound in [0,0xFF], i have checked :-)
		int nTmp1 = 0xFF * MFC_A(&cr1), nTmp2 = 0xFF * nAlpha2,
			nTmp12 = MFC_A(&cr1) * nAlpha2,
			nTemp = nTmp1 + nTmp2 - nTmp12 ;
		MFC_B(pDest) = (nTmp2*MFC_B(&cr2) + (nTmp1 - nTmp12)*MFC_B(&cr1))/nTemp ;
		MFC_G(pDest) = (nTmp2*MFC_G(&cr2) + (nTmp1 - nTmp12)*MFC_G(&cr1))/nTemp ;
		MFC_R(pDest) = (nTmp2*MFC_R(&cr2) + (nTmp1 - nTmp12)*MFC_R(&cr1))/nTemp ;
		MFC_A(pDest) = nTemp / 0xFF ;

		/*      // un-optimized, easier to read
		int  nTemp = 0xFF*(MFC_A(&cr1) + nAlpha2) - MFC_A(&cr1)*nAlpha2 ;
		MFC_B(pDest) = (0xFF*MFC_B(&cr2)*nAlpha2 + (0xFF - nAlpha2)*MFC_B(&cr1)*MFC_A(&cr1))/nTemp ;
		MFC_G(pDest) = (0xFF*MFC_G(&cr2)*nAlpha2 + (0xFF - nAlpha2)*MFC_G(&cr1)*MFC_A(&cr1))/nTemp ;
		MFC_R(pDest) = (0xFF*MFC_R(&cr2)*nAlpha2 + (0xFF - nAlpha2)*MFC_R(&cr1)*MFC_A(&cr1))/nTemp ;
		MFC_A(pDest) = nTemp / 0xFF ;*/
	}
	else
	{
		MFC_B(pDest) = MFC_G(pDest) = MFC_R(pDest) = 0xFF ;
		MFC_A(pDest) = 0 ;
	}
}
//-----------------------------------------------------------------------------
__forceinline void CMFCColor::AlphaBlendPixel(BYTE* pDest, const BYTE* pSrc,BYTE nAlphaSrc)
{
	if (nAlphaSrc == 0xFF)
	{
		*(WORD*)pDest = *(WORD*)pSrc ;
		((BYTE*)pDest)[2] = ((BYTE*)pSrc)[2] ;
		return ;
	}
	if (nAlphaSrc == 0)
		return ;

	// needn't bound in [0,0xFF], i have checked :-)
	pDest[0] = (pSrc[0] - pDest[0]) * nAlphaSrc / 0xFF + pDest[0] ;
	pDest[1] = (pSrc[1] - pDest[1]) * nAlphaSrc / 0xFF + pDest[1] ;
	pDest[2] = (pSrc[2] - pDest[2]) * nAlphaSrc / 0xFF + pDest[2] ;
}
//-----------------------------------------------------------------------------
inline RGBQUAD CMFCColor::DoubleRGBToRGB(const double& r,const double& g,const double& b)
{
	return MFC_RGBA(MFCClamp0255((int)(r*255)),
		MFCClamp0255((int)(g*255)),
		MFCClamp0255((int)(b*255))) ;
}
//-----------------------------------------------------------------------------
inline void CMFCColor::RGBtoHLS (const void* prgb, double* H, double* L, double* S)
{
	const int n_cmax = MFCMax(MFC_R(prgb), MFCMax(MFC_G(prgb), MFC_B(prgb))),
		      n_cmin = MFCMin(MFC_R(prgb), MFCMin(MFC_G(prgb), MFC_B(prgb))) ;

	*L = (n_cmax + n_cmin)/(2.0*255.0);
	if (n_cmax == n_cmin)
	{
		*S = *H = 0.0 ;
		return ;
	}
	const double r = MFC_R(prgb)/255.0,g = MFC_G(prgb)/255.0,b = MFC_B(prgb)/255.0,
		         cmax = n_cmax/255.0,cmin = n_cmin/255.0,delta = cmax - cmin ;
	if(*L < 0.5) 
		*S = delta/(cmax+cmin) ;
	else
		*S = delta/(2.0-cmax-cmin) ;

	if(MFC_R(prgb) == n_cmax)
		*H = (g-b) / delta ;
	else if(MFC_G(prgb) == n_cmax)
		*H = 2.0 + (b-r) / delta ;
	else
		*H = 4.0 + (r-g) / delta ;
	*H /= 6.0 ;
	if (*H < 0.0)
		*H += 1.0 ;
}
//-----------------------------------------------------------------------------
inline double CMFCColor::HLS_Value(const double& m1,const double& m2, double h)
{
	if (h > 6.0)
		h -= 6.0 ;
	else if (h < 0.0)
		h += 6.0 ;

	if (h < 1.0)
		return m1 + (m2 - m1) * h ;
	else if (h < 3.0)
		return m2 ;
	else if (h < 4.0)
		return m1 + (m2 - m1) * (4.0 - h) ;
	return m1 ;
}
inline RGBQUAD CMFCColor::HLStoRGB(const double& H, const double& L, const double& S)
{
	if (S < FLT_EPSILON) // == 0
		return DoubleRGBToRGB(L, L, L) ;

	double     m1, m2 ;
	if (L < 0.5)
		m2 = L * (1.0 + S) ;
	else
		m2 = L + S - L*S ;
	m1 = 2.0*L - m2 ;

	return DoubleRGBToRGB(HLS_Value (m1, m2, H*6.0 + 2.0),
						  HLS_Value (m1, m2, H*6.0),
						  HLS_Value (m1, m2, H*6.0 - 2.0)) ;
}
//-----------------------------------------------------------------------------
inline void CMFCColor::RGBtoHSV(const void* prgb, double* H, double* S, double* V)
{
	const int n_cmax = MFCMax(MFC_R(prgb), MFCMax(MFC_G(prgb), MFC_B(prgb))),
		      n_cmin = MFCMin(MFC_R(prgb), MFCMin(MFC_G(prgb), MFC_B(prgb)));
	const double r = MFC_R(prgb) / 255.0,g = MFC_G(prgb) / 255.0,
		         b = MFC_B(prgb) / 255.0,delta = (n_cmax - n_cmin) / 255.0 ;
	*V = n_cmax / 255.0 ;
	if (n_cmax == n_cmin)
	{
		*S=0.0 ; *H=0.0;
		return ;
	}

	*S = (n_cmax - n_cmin) / (double)n_cmax ;
	if (MFC_R(prgb) == n_cmax)
		*H = (g - b) / delta ;
	else if (MFC_G(prgb) == n_cmax)
		*H = 2.0 + (b - r) / delta ;
	else if (MFC_B(prgb) == n_cmax)
		*H = 4.0 + (r - g) / delta ;

	*H /= 6.0 ;
	if (*H < 0.0)
		*H += 1.0 ;
	else if (*H > 1.0)
		*H -= 1.0 ;
}
//-----------------------------------------------------------------------------
inline RGBQUAD CMFCColor::HSVtoRGB(double h, const double& s, const double& v)
{
	if (s < FLT_EPSILON) // == 0
		return DoubleRGBToRGB (v, v, v) ;

	if (h == 1.0)
		h = 0.0 ;
	h *= 6.0 ;

	const double  f = h - (int)h,
		p = v * (1.0 - s),
		q = v * (1.0 - s * f),
		t = v * (1.0 - s * (1.0 - f)) ;
	switch ((int)h)
	{
		case 0 : return DoubleRGBToRGB(v, t, p) ;
		case 1 : return DoubleRGBToRGB(q, v, p) ;
		case 2 : return DoubleRGBToRGB(p, v, t) ;
		case 3 : return DoubleRGBToRGB(p, q, v) ;
		case 4 : return DoubleRGBToRGB(t, p, v) ;
		case 5 : return DoubleRGBToRGB(v, p, q) ;
	}
	return DoubleRGBToRGB (0, 0, 0) ;
}
//-----------------------------------------------------------------------------
inline bool CMFCColor::IsRGBEqual(const void* p1, const void* p2)
{
	return (MFC_B(p1) == MFC_B(p2)) && (MFC_G(p1) == MFC_G(p2)) && (MFC_R(p1) == MFC_R(p2)) ;
}

#endif
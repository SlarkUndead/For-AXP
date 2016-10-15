
#include "stdafx.h"
#include "MFCColor.h"

//-----------------------------------------------------------------------------
// Computes bilinear interpolation of four pixels.
// The pixels in 'crPixel' in the following order: [0,0], [1,0], [0,1], [1,1].
// !!! the pointer must 24bit or 32bit color
RGBQUAD CMFCColor::GetBilinearPixel(double x, double y, BOOL bHasAlpha, const BYTE* crPixel[4])
{
	const BYTE * pPixel0 = crPixel[0], * pPixel1 = crPixel[1],
		* pPixel2 = crPixel[2], * pPixel3 = crPixel[3] ;
	RGBQUAD crRet = {0xFF, 0xFF, 0xFF, 0xFF} ;

	if( bHasAlpha && (*(DWORD*)pPixel0 == *(DWORD*)pPixel1) &&
		(*(DWORD*)pPixel0 == *(DWORD*)pPixel2) &&
		(*(DWORD*)pPixel0 == *(DWORD*)pPixel3) )
	{
		return *(RGBQUAD*)pPixel0 ;
	}

	if ( !bHasAlpha && IsRGBEqual(pPixel0, pPixel1) &&
		IsRGBEqual(pPixel0, pPixel2) && IsRGBEqual(pPixel0, pPixel3) )
	{
		CMFCColor::CopyPixel(&crRet, pPixel0, 3) ;
		return crRet ;
	}

	// test x-[0,1] y-[0,1]
	//	assert ((x > -FLT_EPSILON) && (x < 1.0) && (y > -FLT_EPSILON) && (y < 1.0)) ;
	// x && y is zero
	if ((x < FLT_EPSILON) && (y < FLT_EPSILON))
	{
		CMFCColor::CopyPixel(&crRet, pPixel0, bHasAlpha ? 4 : 3) ;
		return crRet ;
	}

	if (!bHasAlpha || ((MFC_A(pPixel0) & MFC_A(pPixel1) & MFC_A(pPixel2) & MFC_A(pPixel3)) == 0xFF))
	{
		// if 24bit color
		for (int i=0 ; i < 3 ; i++)
		{
			const double   m0 = pPixel0[i] + x * (pPixel1[i] - pPixel0[i]),
				m1 = pPixel2[i] + x * (pPixel3[i] - pPixel2[i]),
				my = m0 + y * (m1 - m0) ;
			((BYTE*)&crRet)[i] = (int)my ; // needn't bound
		}
	}
	else
	{
		// under is 32bit color with alpha
		int nAlpha[4] = {pPixel0[3], pPixel1[3], pPixel2[3], pPixel3[3]} ;
		// calc dest alpha value
		double m0 = nAlpha[0] + x * (nAlpha[1] - nAlpha[0]),
			m1 = nAlpha[2] + x * (nAlpha[3] - nAlpha[2]),
			my = m0 + y * (m1 - m0) ;
		MFC_A(&crRet) = (int)my ;
		if(MFC_A(&crRet)) // has alpha
		{
			for(int i=0 ; i < 3 ; i++)
			{
				int nSum0 = nAlpha[0] * pPixel0[i],
					nSum2 = nAlpha[2] * pPixel2[i] ;

				m0 = nSum0 + x * (pPixel1[i] * nAlpha[1] - nSum0) ;
				m1 = nSum2 + x * (pPixel3[i] * nAlpha[3] - nSum2) ;
				my = m0 + y * (m1 - m0) ;
				((BYTE*)&crRet)[i] = int(my) / MFC_A(&crRet) ;
			}
		}
	}
	return crRet ;
}
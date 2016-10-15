
#include "stdafx.h"
#include "MFCFreeImageHelper.h"
#include "../MFCImageObj.h"
#include "../MFCArray.h"
#include "FreeImage.h"

//=============================================================================
bool FreeImageToMFCImageObj(FIBITMAP* pFIimg, CMFCImageObj& img)
{
	if (!pFIimg)
		return false ;

	// prepare image info
	BYTE  __bufImgInfo[sizeof(BITMAPINFOHEADER) + 16] = {0} ;
	BITMAPINFOHEADER * pInfo = (BITMAPINFOHEADER*)__bufImgInfo ;
	memcpy(pInfo, FreeImage_GetInfoHeader(pFIimg), sizeof(BITMAPINFOHEADER)) ;
	if (pInfo->biCompression == BI_BITFIELDS)
	{
		DWORD   * pMask = (DWORD*)(pInfo + 1) ;
		pMask[0] = FreeImage_GetRedMask (pFIimg) ;
		pMask[1] = FreeImage_GetGreenMask (pFIimg) ;
		pMask[2] = FreeImage_GetBlueMask (pFIimg) ;
	}

	// create image
	if(!img.Create(pInfo))
		return false ;

	// set palette
	if(img.GetImgColorBits() <= 8)
		img.SetColorTable(0,FreeImage_GetColorsUsed(pFIimg),FreeImage_GetPalette(pFIimg)) ;

	// set pixel
	memcpy(img.GetMemStart(),FreeImage_GetBits(pFIimg),img.GetImgPitch()*img.GetImgHeight()) ;

	// transparency
	if(FreeImage_IsTransparent(pFIimg))
	{
		unsigned i = FreeImage_GetTransparencyCount(pFIimg) ;
		BYTE * trns = FreeImage_GetTransparencyTable(pFIimg) ;
	}

	// image delay time
	FITAG * tag = 0;
	if(FreeImage_GetMetadata(FIMD_ANIMATION, pFIimg,"FrameTime",&tag))
	{
		img.SetNextFrameDelay(*(LONG*)FreeImage_GetTagValue(tag)) ;
	}
	return true ;
}
//=============================================================================
FIBITMAP * AllocateFreeImage(const CMFCImageObj& img)
{
	if (!img.IsValidImage())
		return 0 ;

	// create FreeImage object
	DWORD dwBitFields[3] = {0, 0, 0} ;
	if(img.GetImgColorBits() == 16)
	{
		CMFCArray<BITMAPINFOHEADER> info(img.NewImgInfoWithPalette()) ;
		memcpy (dwBitFields, info.GetArrayPtr() + 1, 12) ;
	}
	FIBITMAP * pFIimg = FreeImage_AllocateT(FIT_BITMAP,img.GetImgWidth(),img.GetImgHeight(), img.GetImgColorBits(),dwBitFields[0],dwBitFields[1],dwBitFields[2]) ;
	if (!pFIimg)
		return 0 ;

	// set pixel
	//assert (img.GetImgPitch() == FreeImage_GetPitch(pFIimg)) ;
	memcpy(FreeImage_GetBits(pFIimg),img.GetMemStart(),img.GetImgPitch()*img.GetImgHeight()) ;

	// set palette
	if(img.GetImgColorBits() <= 8)
	{
		RGBQUAD pPal[256] = {0} ;
		img.GetColorTable (0, 1<<img.GetImgColorBits(), pPal) ;
		memcpy(FreeImage_GetPalette(pFIimg), pPal, FreeImage_GetColorsUsed(pFIimg)*sizeof(RGBQUAD)) ;
	}
	return pFIimg ;
}
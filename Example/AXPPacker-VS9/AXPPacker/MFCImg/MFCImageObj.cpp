
#include "stdafx.h"
#include "MFCImageObj.h"
#include "MFCImageType.h"
#include "MFCColor.h"
#include "MFCArray.h"
#include "MFCArchiveHelper.h"
#include "MFCObjProgress.h"

#include "ImgHandle\MFCIImageHandle.h"
#include "ImgHandle\MFCImageHandleFactory.h"
#include "PixelProcessor\MFCIPixelProcessor.h"
#include "ImgHandle\MFCFreeImageHandleFactory.h"
//=============================================================================

//-----------------------------------------------------------------------------
bool CMFCImageObj::Create(const BITMAPINFOHEADER* pBmif)
{
	// unsupported store format
	if (!pBmif || (pBmif->biHeight <= 0) || (pBmif->biWidth <= 0))
		return false;
	if (!((pBmif->biCompression == BI_RGB) || (pBmif->biCompression == BI_BITFIELDS)))
		return false;
	switch(pBmif->biBitCount) // validate bpp
	{
		case 1 :
		case 4 :
		case 8 :
		case 16 :
		case 24 :
		case 32 : break ;
		default : return false;
	}

	Destroy() ;

	// init struct
	memset (&m_DibInfo, 0, sizeof(m_DibInfo)) ;
	m_DibInfo.biSize          = sizeof(BITMAPINFOHEADER) ;
	m_DibInfo.biWidth         = pBmif->biWidth ;
	m_DibInfo.biHeight        = pBmif->biHeight ;
	m_DibInfo.biPlanes        = 1 ;
	m_DibInfo.biBitCount      = pBmif->biBitCount ;
	m_DibInfo.biCompression   = pBmif->biCompression ;
	m_DibInfo.biXPelsPerMeter = pBmif->biXPelsPerMeter ;
	m_DibInfo.biYPelsPerMeter = pBmif->biYPelsPerMeter ;

	// now flag BI_BITFIELDS is only valid in 16bit image
	if(pBmif->biBitCount == 16)
	{
		m_dwBitFields[0] = MFC_MASK16_RED_555 ; // 16-bit default format : 5-5-5
		m_dwBitFields[1] = MFC_MASK16_GREEN_555 ;
		m_dwBitFields[2] = MFC_MASK16_BLUE_555 ;
		if (pBmif->biCompression == BI_BITFIELDS) // custom
			memcpy (m_dwBitFields, pBmif + 1, 12) ;
	}
	else
		m_DibInfo.biCompression = BI_RGB ; // i think it's unnecessary to use mask in 32bit image

	// create pixel buffer, pixel must must must initialized to zero !!!
	m_pByte = CMFCArchiveHelper::ZeroMalloc(GetImgPitch()*GetImgHeight()) ;
	//assert(((long)m_pByte % 4) == 0) ; // DWORD align

	// create a line pointer, to accelerate pixel access
	m_ppLine = (BYTE **) new BYTE [sizeof(BYTE*) * GetImgHeight()] ;
	const int     nPitch = GetImgPitch() ;
	m_ppLine[0] = m_pByte + (GetImgHeight() - 1) * nPitch ;
	for (int y = 1 ; y < GetImgHeight() ; y++)
	{
		m_ppLine[y] = m_ppLine[y - 1] - nPitch ;
	}

	// 8bit color image default set a gray palette
	if (GetImgColorBits() <= 8)
	{
		m_pPalette = new RGBQUAD[1 << GetImgColorBits()] ;
		SetGrayPalette() ;
	}
	return true ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::Destroy()
{
	if(IsValidImage())
	{
		if(m_ppLine) delete[] m_ppLine;
		CMFCArchiveHelper::ZeroFree(m_pByte);

		if (m_pPalette)	delete[] m_pPalette;
		InitClassMember() ;
		return true;
	}
	return false;
}
//-----------------------------------------------------------------------------
inline BITMAPINFOHEADER* CMFCImageObj::NewImgInfoWithPalette() const
{
	// prepare info
	const int nColorNum = 1 << GetImgColorBits(),
		nPalBytes = ((GetImgColorBits() <= 8) ? (4*nColorNum) : 0) ;
	BITMAPINFOHEADER   * pBmfh = (BITMAPINFOHEADER*) new BYTE[16 + sizeof(BITMAPINFOHEADER) + nPalBytes] ;
	*pBmfh = m_DibInfo ;

	// append palette(<=8bit) or bit-fields(16bit)
	if (GetImgColorBits() <= 8)
		GetColorTable (0, nColorNum, (RGBQUAD*)(pBmfh + 1)) ;
	else
		memcpy (pBmfh + 1, m_dwBitFields, 12) ;
	return pBmfh ;
}

//-----------------------------------------------------------------------------
DWORD CMFCImageObj::GetPixelData(int x, int y) const
{
	if(!IsInside(x, y))
		return 0;

	const BYTE   * pPixel = GetBits (x,y) ;
	switch(GetImgColorBits())
	{
	case  1 : return 0x01 & (*pPixel >> (7 - (x & 7))) ;
	case  4 : return 0x0F & (*pPixel >> (x & 1 ? 0 : 4)) ;
	case  8 : return *(BYTE*)pPixel ;
	case 16 : return *(WORD*)pPixel ;
	case 24 :
		{
			DWORD     dwrgb = 0 ;
			CMFCColor::CopyPixel (&dwrgb, pPixel, 3) ;
			return dwrgb ;
		}
	case 32 : return *(DWORD*)pPixel ;
	}
	return 0 ;
}
//-----------------------------------------------------------------------------
void CMFCImageObj::SetPixelData (int x, int y, DWORD dwPixel)
{
	if (!IsInside(x, y))
		return;

	BYTE   * pPixel = GetBits (x,y) ;
	switch (GetImgColorBits())
	{
	case  1 :
		*pPixel &= ~(1 << (7 - (x & 7))) ;
		*pPixel |= dwPixel << (7 - (x & 7)) ;
		break ;
	case  4 :
		*pPixel &= 0x0F << (x & 1 ? 4 : 0) ;
		*pPixel |= dwPixel << (x & 1 ? 0 : 4) ;
		break ;
	case  8 :
	case 16 :
	case 24 :
	case 32 : 
		CMFCColor::CopyPixel (pPixel, &dwPixel, GetImgColorBits() / 8) ;
		break ;
	}
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::GetColorTable(int iFirstIndex, int iNumber, RGBQUAD* pColors) const
{
	if (!IsValidImage() || (GetImgColorBits() > 8) || (iFirstIndex < 0) || !pColors || !m_pPalette)
		return false;

	const int nColorNum = 1 << GetImgColorBits() ;
	for(int i=0 ; i < iNumber ; i++)
	{
		int nIndex = iFirstIndex + i ;
		if(nIndex < nColorNum)
			pColors[i] = m_pPalette[nIndex] ;
	}
	return true ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::SetColorTable(int iFirstIndex, int iNumber, RGBQUAD* pColors)
{
	if (!IsValidImage() || (GetImgColorBits() > 8) || (iFirstIndex < 0) || !pColors || !m_pPalette)
		return false;

	const int nColorNum = 1 << GetImgColorBits() ;
	for (int i=0 ; i < iNumber ; i++)
	{
		int nIndex = iFirstIndex + i ;
		if(nIndex < nColorNum)
			m_pPalette[nIndex] = pColors[i] ;
	}
	return true ;
}

//-----------------------------------------------------------------------------
void CMFCImageObj::ConvertToTrueColor(int iColor)
{
	if (!IsValidImage() || (GetImgColorBits() == iColor))
		return ;
	if ((iColor != 24) && (iColor != 32))
		return;

	// backup image
	const CMFCImageObj OldPic (*this) ;
	if (!Create(OldPic.GetImgWidth(), OldPic.GetImgHeight(), iColor))
		return ;

	// get palette
	RGBQUAD pPal[256] ;
	if (OldPic.GetImgColorBits() <= 8)
		OldPic.GetColorTable (0, 1 << OldPic.GetImgColorBits(), pPal) ;

	// start color convert
	const int nNewSpan = this->GetImgColorBits() / 8, // 3 or 4
		nOldSpan = OldPic.GetImgColorBits() / 8 ;
	for(int y=0 ; y < GetImgHeight() ; y++)
	{
		const BYTE * pOld = OldPic.GetBits (y) ;
		BYTE * pNew = this->GetBits (y) ;
		for(int x=0 ; x < GetImgWidth() ; x++, pNew+=nNewSpan, pOld+=nOldSpan)
		{
			switch (OldPic.GetImgColorBits())
			{
			case 1 :
			case 4 :
			case 8 : // 1,4,8 ==> 24,32
				CMFCColor::CopyPixel (pNew, &pPal[OldPic.GetPixelData(x,y)], 3) ;
				break ;
			case 16 : // 16 ==> 24,32
				{
					RGBQUAD     crTrans ;
					if (OldPic.m_dwBitFields[1] == MFC_MASK16_GREEN_555)
						crTrans = Split16Bit_555 (*(WORD*)pOld) ;
					else if (OldPic.m_dwBitFields[1] == MFC_MASK16_GREEN_565)
						crTrans = Split16Bit_565 (*(WORD*)pOld) ;
					CMFCColor::CopyPixel (pNew, &crTrans, 3) ;
					break ;
				}
			case 24 :
			case 32 : // 24,32 ==> 32,24
				CMFCColor::CopyPixel (pNew, pOld, 3) ;
				break ;
			}
		}
	}
	if (iColor == 32)
		SetAlphaChannelValue (0xFF) ; // set alpha to 0xFF
}
//-----------------------------------------------------------------------------
void CMFCImageObj::GetAlphaChannel (CMFCImageObj* imgAlpha) const
{
	// create alpha-channel image, it's a 8-bit color image
	if (!imgAlpha || !IsValidImage() || (GetImgColorBits() != 32) || (imgAlpha == this)
		|| !imgAlpha->Create (GetImgWidth(), GetImgHeight(), 8))
		return ;

	// get alpha channel
	for (int y=0 ; y < GetImgHeight() ; y++)
	{
		for (int x=0 ; x < GetImgWidth() ; x++)
		{
			*imgAlpha->GetBits(x,y) = MFC_A(GetBits(x,y)) ;
		}
	}
}
//-----------------------------------------------------------------------------
void CMFCImageObj::AppendAlphaChannel (const CMFCImageObj& alpha)
{
	if (!IsValidImage() || !alpha.IsValidImage() || (GetImgColorBits() != 32) || (alpha.GetImgColorBits() != 8) ||
		(GetImgWidth() != alpha.GetImgWidth()) || (GetImgHeight() != alpha.GetImgHeight()))
		return ;

	// append alpha channel
	for (int y=0 ; y < GetImgHeight() ; y++)
	{
		for (int x=0 ; x < GetImgWidth() ; x++)
		{
			MFC_A(GetBits(x,y)) = *alpha.GetBits(x,y) ;
		}
	}
}
//-----------------------------------------------------------------------------
void CMFCImageObj::SetAlphaChannelValue (int nValue)
{
	if (!IsValidImage() || (GetImgColorBits() != 32))
		return ;

	for (int y=0 ; y < GetImgHeight() ; y++)
	{
		for (int x=0 ; x < GetImgWidth() ; x++)
		{
			MFC_A(GetBits(x,y)) = nValue ;
		}
	}
}
//-----------------------------------------------------------------------------
CMFCImageObj& CMFCImageObj::operator= (const CMFCImageObj& img)
{
	if (!img.IsValidImage() || (&img == this))
		return *this;

	CMFCArray<BITMAPINFOHEADER> bmfh(img.NewImgInfoWithPalette()) ;
	if (Create (bmfh.GetArrayPtr()))
	{
		// copy the pixels
		memcpy (GetMemStart(), img.GetMemStart(), img.GetImgPitch()*img.GetImgHeight()) ;
		// copy the palette
		if (img.GetImgColorBits() <= 8)
			CopyPalette (img) ;

		// copy extended property
		m_nNextFrameDelay = img.m_nNextFrameDelay ;

		// copy position
		CMFCGraphObj::operator=(img) ;
	}
	return *this ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::GetSubBlock (CMFCImageObj* SubImg, RECT rcBlock) const
{
	if (!IsValidImage() || !SubImg || (SubImg == this) || (GetImgColorBits() < 8))
		return false;

	const RECT rcImage = {0, 0, GetImgWidth(), GetImgHeight()} ;
	RECT rcD ;
	//assert (IsRectInRect (rcImage, rcBlock)) ;
	if (::IntersectRect (&rcD, &rcImage, &rcBlock) == 0)
		return false; // rect of destination is empty

	if (!SubImg->Create(GetRectWidth(rcD), GetRectHeight(rcD), GetImgColorBits()))
		return false ;

	// copy pixel
	const int nSubPitch = SubImg->GetImgWidth() * GetImgColorBits() / 8 ;
	for(int i=0 ; i < SubImg->GetImgHeight() ; i++)
	{
		memcpy (SubImg->GetBits(i), GetBits(rcD.left, rcD.top + i), nSubPitch) ;
	}

	// copy palette
	if (GetImgColorBits() <= 8)
		SubImg->CopyPalette (*this) ;

	// set relative position
	SubImg->SetGraphObjPos (rcD.left, rcD.top) ;
	return true ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::CoverBlock (const CMFCImageObj& Img, int x, int y)
{
	if (!IsValidImage() || !Img.IsValidImage() || (GetImgColorBits() != Img.GetImgColorBits()) || (GetImgColorBits() < 8))
		return false;

	// calculate covered RECT
	const RECT rcImage = {0, 0, GetImgWidth(), GetImgHeight()},
		rcCover = {x, y, x+Img.GetImgWidth(), y+Img.GetImgHeight()} ;
	RECT rcD ;
	if (::IntersectRect (&rcD, &rcImage, &rcCover) == 0)
		return false ; // rect of destination is empty

	// copy pixel
	const int nSubPitch = GetRectWidth(rcD) * Img.GetImgColorBits() / 8 ;
	for(int cy=rcD.top ; cy < rcD.bottom ; cy++) // copy
	{
		const BYTE   * pS = Img.GetBits (rcD.left-x, cy-y) ; // calculate edge
		BYTE   * pD = this->GetBits (rcD.left, cy) ;
		memcpy (pD, pS, nSubPitch) ;
	}
	return true ;
}
//-----------------------------------------------------------------------------
void CMFCImageObj::TileBlock (const CMFCImageObj & Img, int x, int y)
{
	int nYStart = y ;
	while(nYStart < GetImgHeight())
	{
		int nXStart = x ;
		while(nXStart < GetImgWidth())
		{
			CoverBlock (Img, nXStart, nYStart) ; // security ensured by CoverBlock
			nXStart += Img.GetImgWidth() ;
		}
		nYStart += Img.GetImgHeight() ;
	}
}
//-----------------------------------------------------------------------------
void CMFCImageObj::CombineImage (const CMFCImageObj& Img32, int x, int y, int nAlphaPercent)
{
	RECT rcD ;

	RECT rcImg = {0, 0, GetImgWidth(), GetImgHeight()},
		rcMask = {x, y, x+Img32.GetImgWidth(), y+Img32.GetImgHeight()} ;
	::IntersectRect(&rcD, &rcImg, &rcMask) ;

	if ((Img32.GetImgColorBits() != 32) || (GetImgColorBits() != 32) || IsRectEmpty(&rcD))
		return;

	nAlphaPercent = MFCClamp(nAlphaPercent, 0, 100) ;
	for(int cy=rcD.top ; cy < rcD.bottom ; cy++)
	{
		RGBQUAD   * pDest = (RGBQUAD*)this->GetBits (rcD.left, cy),
			* pSrc = (RGBQUAD*)Img32.GetBits (rcD.left-x, cy-y) ; // calculate edge
		for (int cx=rcD.left ; cx < rcD.right ; cx++, pDest++, pSrc++)
		{
			CMFCColor::CombineAlphaPixel(pDest, *pDest, *pSrc,
				(nAlphaPercent == 100) ? MFC_A(pSrc):(MFC_A(pSrc)*nAlphaPercent/100)) ;
		}
	}
}
//-----------------------------------------------------------------------------
// alpha混和是图像处理的心脏，它的效率和安全性关系着整个软件
// 因此我才这么麻烦的处理之
// rcSrc一定要在MaskImg32内部，rcDest可以相交
void CMFCImageObj::AlphaBlend (const CMFCImageObj& Img32, const RECT& rcDest, const RECT& rcSrc, int nAlphaPercent)
{
	if (nAlphaPercent == 0)
		return ;

	// parameter check, rcSrc一定要在MaskImg32内部
	const RECT   rcMask = {0, 0, Img32.GetImgWidth(), Img32.GetImgHeight()},
		rcImg = {0, 0, GetImgWidth(), GetImgHeight()} ;
	RECT         rcT ;
	IntersectRect(&rcT, &rcDest, &rcImg) ;
	if (!IsValidImage() || (GetImgColorBits() < 24) || !Img32.IsValidImage() || (Img32.GetImgColorBits() != 32) ||
		!IsRectInRect (rcMask, rcSrc) || IsRectEmpty(&rcT))
		return;

	nAlphaPercent = MFCClamp (nAlphaPercent, 0, 100) ;
	const int   nSpan = GetImgColorBits() / 8 ; // 3 or 4
	if ((GetRectWidth(rcDest) == GetRectWidth(rcSrc)) && (GetRectHeight(rcDest) == GetRectHeight(rcSrc)))
	{
		const int   nSrcX = rcT.left - rcDest.left + rcSrc.left ;
		for (int y=rcT.top ; y < rcT.bottom ; y++)
		{
			const BYTE  * pSrc = Img32.GetBits (nSrcX, y-rcDest.top+rcSrc.top) ; // calculate edge
			BYTE        * pDest = this->GetBits (rcT.left, y) ;
			for (int x=rcT.left ; x < rcT.right ; x++, pDest+=nSpan, pSrc+=4)
			{
				CMFCColor::AlphaBlendPixel (pDest, pSrc, (nAlphaPercent==100) ? MFC_A(pSrc)
					: MFC_A(pSrc)*nAlphaPercent/100) ;
			}
		}
	}
	else
	{
		CMFCArray<int>   pX (GetRectWidth(rcT)),pY(GetRectHeight(rcT));
		int x, y ;

		for (y=rcT.top ; y < rcT.bottom ; y++)
		{
			pY[y-rcT.top] = rcSrc.top+(y-rcDest.top)*GetRectHeight(rcSrc)/GetRectHeight(rcDest) ;
		}
		for (x=rcT.left ; x < rcT.right ; x++)
		{
			pX[x-rcT.left] = rcSrc.left+(x-rcDest.left)*GetRectWidth(rcSrc)/GetRectWidth(rcDest) ;
		}

		for (y=rcT.top ; y < rcT.bottom ; y++)
		{
			BYTE   * pDest = this->GetBits (rcT.left, y) ;
			for (x=rcT.left ; x < rcT.right ; x++, pDest+=nSpan)
			{
				const BYTE   * pSrc = Img32.GetBits (pX[x-rcT.left], pY[y-rcT.top]) ;
				CMFCColor::AlphaBlendPixel (pDest, pSrc, (nAlphaPercent==100) ? MFC_A(pSrc)
					: MFC_A(pSrc)*nAlphaPercent/100) ;
			}
		}
	}
}
//-----------------------------------------------------------------------------
void CMFCImageObj::LogicalBlend (const CMFCImageObj & MaskImg, LOGICAL_OP LogOP, int x, int y)
{
	if (!IsValidImage() || !MaskImg.IsValidImage() || (GetImgColorBits() != MaskImg.GetImgColorBits()) || (this == &MaskImg))
		return ;
	//assert (GetImgColorBits() == 8) ;

	const RECT   rcSrc1 = {0, 0, GetImgWidth(), GetImgHeight()},
		rcSrc2 = {x, y, x+MaskImg.GetImgWidth(), y+MaskImg.GetImgHeight()} ;
	RECT         rcDest ;
	if (::IntersectRect (&rcDest, &rcSrc1, &rcSrc2) == 0)
		return ; // no intersected rect

	const int   nSpan = GetImgColorBits() / 8; // 1,2,3,4
	for (int cy=rcDest.top ; cy < rcDest.bottom ; cy++)
	{
		const BYTE   * pSrc = MaskImg.GetBits (rcDest.left-x, cy-y) ; // calculate edge
		BYTE   * pDest = this->GetBits (rcDest.left, cy) ;
		for (int cx=rcDest.left ; cx < rcDest.right ; cx++, pDest+=nSpan, pSrc+=nSpan)
		{
			switch (LogOP)
			{
			case LOGI_OR  : *pDest |= *pSrc ; break ;
			case LOGI_AND : *pDest &= *pSrc ; break ;
			case LOGI_XOR : *pDest ^= *pSrc ; break ;
			case LOGI_SEL_ADD :
				if (*pSrc == 0xFF)
					*pDest = 0xFF ;
				break ;
			case LOGI_SEL_SUB :
				if (*pSrc == 0xFF)
					*pDest = 0 ;
				break ;
				//				default : assert(false);
			}
		}
	}
}
//-----------------------------------------------------------------------------
int CMFCImageObj::Serialize (bool bSave, BYTE* pSave)
{
	const BYTE   * pBak = pSave ;
	if (bSave) // save
	{
		//		assert (GetImgColorBits() > 8) ; // must true color image
		memcpy (pSave, &m_DibInfo, sizeof(m_DibInfo)) ; pSave += sizeof(m_DibInfo) ;
		memcpy (pSave, m_dwBitFields, 12) ; pSave += 12 ;

		int     nWrite = GetImgPitch() * GetImgHeight() ;
		memcpy (pSave, GetMemStart(), nWrite) ; pSave += nWrite ;
	}
	else // load
	{
		Create ((BITMAPINFOHEADER*)pSave) ;
		pSave += sizeof(m_DibInfo) + 12 ;

		int     nWrite = GetImgPitch() * GetImgHeight() ;
		memcpy (GetMemStart(), pSave, nWrite) ;
		pSave += nWrite ;
	}
	pSave += CMFCGraphObj::Serialize (bSave, pSave) ;
	return (int)(pSave - pBak) ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::Load (CMFCIImageHandle* pHandler)
{
	std::deque<CMFCImageObj*>   imgList ;
	if (pHandler)
		imgList = pHandler->GetLoadImage(false) ;
	if (imgList.empty())
		return false ;

	*this = *imgList[0] ;
	return true ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::Load(const char* szFileName)
{
	MFC_IMAGE_TYPE imgType = CMFCArchiveHelper::GetImageTypeFromName(szFileName) ;
	std::auto_ptr<CMFCIImageHandle>pHandler(GetImageHandleFactory()->CreateImageHandle(imgType)) ;
	if (!pHandler.get())
		return false ;

	bool bRet = pHandler->LoadImageFile(szFileName) ;
	if (bRet)
		bRet = this->Load(pHandler.get()) ;
	//	assert (bRet) ;
	return bRet ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::Load(BYTE* pStart, int iFileSize,MFC_IMAGE_TYPE imgType)
{
	std::auto_ptr<CMFCIImageHandle> pHandler(GetImageHandleFactory()->CreateImageHandle(imgType)) ;
	if (!pHandler.get())
		return false ;

	bool bRet = pHandler->LoadImageMemory(pStart, iFileSize) ;
	if(bRet)
		bRet = this->Load(pHandler.get()) ;
	//	assert (bRet) ;
	return bRet ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::Save(CMFCIImageHandle* pHandler, const char* szFileName, int nFlag) const
{
	if (!IsValidImage() || !pHandler || !szFileName)
		return false ;
	// push current image to save list
	pHandler->PushSaveList(this) ;
	return pHandler->SaveImage(szFileName, nFlag) ;
}
//-----------------------------------------------------------------------------
bool CMFCImageObj::Save(const char* szFileName, int nFlag) const
{
	if (!IsValidImage() || !szFileName)
		return false ;

	MFC_IMAGE_TYPE imgType = CMFCArchiveHelper::GetImageTypeFromName(szFileName) ;
	std::auto_ptr<CMFCIImageHandle> pHandler(GetImageHandleFactory()->CreateImageHandle(imgType)) ;
	if (!pHandler.get())
		return false ;

	return this->Save(pHandler.get(),szFileName, nFlag) ;
}
//-----------------------------------------------------------------------------
void CMFCImageObj::FillImageRect(const CMFCImageObj& img, const RECT& rcBlock, const void* pSrc)
{
	RECT     rc = {0, 0, img.GetImgWidth(), img.GetImgHeight()} ;
	IntersectRect (&rc, &rc, &rcBlock) ;
	if (IsRectEmpty(&rc))
		return  ;

	const int   nSpan = img.GetImgColorBits() / 8 ; // 1, 2, 3, 4
	for (int y=rc.top ; y < rc.bottom ; y++)
	{
		BYTE   * pPixel = img.GetBits (rc.left, y) ;
		for (int x=rc.left ; x < rc.right ; x++, pPixel += nSpan)
		{
			CMFCColor::CopyPixel (pPixel, pSrc, nSpan) ;
		}
	}
}
//-----------------------------------------------------------------------------
// add frame
// bCopyEdge: duplicate edge during copying
void CMFCImageObj::ExpandFrame (bool bCopyEdge, int iLeft, int iTop, int iRight, int iBottom)
{
	if ((GetImgColorBits() < 8) || (iLeft < 0) || (iTop < 0) || (iRight < 0) || (iBottom < 0))
		return ;
	if ((iLeft == 0) && (iTop == 0) && (iRight == 0) && (iBottom == 0))
		return ;

	// backup image then create expanded image
	const CMFCImageObj     imgOld(*this) ;
	if (!Create (imgOld.GetImgWidth()+iLeft+iRight, imgOld.GetImgHeight()+iTop+iBottom, imgOld.GetImgColorBits()))
		return ;


	// adjust image's position
	SetGraphObjPos (imgOld.GetGraphObjPos().x - iLeft, imgOld.GetGraphObjPos().y - iTop) ;

	// duplicate source image
	CoverBlock (imgOld, iLeft, iTop) ;

	// edge disposal
	if (!bCopyEdge)
		return ;

	// duplicate corner
	const RECT rcUL = {0, 0, iTop, iLeft},
		rcUR = {GetImgWidth()-iRight, 0, GetImgWidth(), iTop},
		rcDL = {0, GetImgHeight()-iBottom, iLeft, GetImgHeight()},
		rcDR = {GetImgWidth()-iRight, GetImgHeight()-iBottom, GetImgWidth(), GetImgHeight()} ;
	FillImageRect(*this, rcUL, imgOld.GetBits (0, 0)) ;
	FillImageRect(*this, rcUR, imgOld.GetBits (imgOld.GetImgWidth()-1, 0)) ;
	FillImageRect(*this, rcDL, imgOld.GetBits (0, imgOld.GetImgHeight()-1)) ;
	FillImageRect(*this, rcDR, imgOld.GetBits (imgOld.GetImgWidth()-1, imgOld.GetImgHeight()-1)) ;

	// duplicate four-edge
	const int dwPitch = GetImgPitch(),
		nSpan = GetImgColorBits() / 8,
		nOldLineBytes = imgOld.GetImgWidth() * nSpan ;
	int m = 0;
	BYTE * pSrc = 0, * pDest = 0;

	// up
	pSrc = GetBits (iLeft, iTop) ;
	pDest = pSrc + dwPitch ;
	for (m=0 ; m < iTop ; m++, pDest += dwPitch)
	{
		memcpy (pDest, pSrc, nOldLineBytes) ;
	}
	// bottom
	pSrc = GetBits (iLeft, imgOld.GetImgHeight() + iTop - 1) ;
	pDest = pSrc - dwPitch ;
	for (m=0 ; m < iBottom ; m++, pDest -= dwPitch)
	{
		memcpy (pDest, pSrc, nOldLineBytes) ;
	}

	// left
	pSrc = GetBits (iLeft, iTop) ;
	pDest = GetBits (0, iTop) ;
	for (m=0 ; m < imgOld.GetImgHeight() ; m++, pDest -= dwPitch, pSrc -= dwPitch)
	{
		BYTE     * pTemp = pDest ;
		for (int i=0 ; i < iLeft ; i++, pTemp += nSpan)
		{
			CMFCColor::CopyPixel (pTemp, pSrc, nSpan) ;
		}
	}
	// right
	pSrc = GetBits (iLeft + imgOld.GetImgWidth() - 1, iTop) ;
	pDest = pSrc + nSpan ;
	for (m=0 ; m < imgOld.GetImgHeight() ; m++, pDest -= dwPitch, pSrc -= dwPitch)
	{
		BYTE     * pTemp = pDest ;
		for (int i=0 ; i < iRight ; i++, pTemp += nSpan)
		{
			CMFCColor::CopyPixel (pTemp, pSrc, nSpan) ;
		}
	}
}
//-----------------------------------------------------------------------------
// stretch (>=8 bit)
void CMFCImageObj::Stretch (int nNewWidth, int nNewHeight)
{
	// parameter check
	if (!IsValidImage() || (nNewWidth <= 0) || (nNewHeight <= 0) || (GetImgColorBits() < 8))
		return ;
	if ((nNewWidth == GetImgWidth()) && (nNewHeight == GetImgHeight()))
		return ;

	// first backup image
	const CMFCImageObj     imgOld(*this) ;
	if (!Create (nNewWidth, nNewHeight, imgOld.GetImgColorBits()))
		return ;

	// duplicate palette
	if (GetImgColorBits() <= 8)
		CopyPalette (imgOld) ;

	// initialize index table
	const int        nSpan = GetImgColorBits() / 8 ;
	CMFCArray<int>   pTabX (GetImgWidth()) ;
	for (int xx=0 ; xx < GetImgWidth() ; xx++)
	{
		pTabX[xx] = xx * imgOld.GetImgWidth() / GetImgWidth() ; // force to omit float
		//assert (pTabX[xx] < imgOld.GetImgWidth()) ;
	}
	for (int mm=0 ; mm < (GetImgWidth() - 1) ; mm++)
	{
		pTabX[mm] = (pTabX[mm+1] - pTabX[mm]) * nSpan ;
	}
	// pTabX里现在存放X轴像素字节差

	for (int y=0 ; y < GetImgHeight() ; y++)
	{
		const BYTE   * pOld = imgOld.GetBits (y * imgOld.GetImgHeight() / GetImgHeight()) ;
		BYTE   * pPixel = GetBits (y) ;
		for (int x=0 ; x < GetImgWidth() ; x++)
		{
			CMFCColor::CopyPixel (pPixel, pOld, nSpan) ;
			pOld += pTabX[x] ;
			pPixel += nSpan ;
		}
	}
}
//-----------------------------------------------------------------------------
// stretch (>=24 bit)
void CMFCImageObj::Stretch_Smooth (int nNewWidth, int nNewHeight, CMFCObjProgress * progress)
{
	// parameter check
	if (!IsValidImage() || (nNewWidth <= 0) || (nNewHeight <= 0) || (GetImgColorBits() < 24))
		return ;
	if ((nNewWidth == GetImgWidth()) && (nNewHeight == GetImgHeight()))
		return ;

	// in order to bilinear, the source image's W/H must larger than 2
	if ((GetImgWidth() == 1) || (GetImgHeight() == 1))
	{
		ExpandFrame (true, 0, 0, (GetImgWidth()==1) ? 1 : 0, (GetImgHeight()==1) ? 1 : 0) ;
	}

	// first backup image
	const CMFCImageObj     imgOld(*this) ;
	if (!Create (nNewWidth, nNewHeight, imgOld.GetImgColorBits()))
		return ;

	// initialize index table, to accelerate
	CMFCArray<int>   pTabX (GetImgWidth()),
		pXMod (GetImgWidth()) ;
	for (int i=0 ; i < GetImgWidth() ; i++)
	{
		pTabX[i] = i * imgOld.GetImgWidth() / GetImgWidth() ;
		pXMod[i] = (i * imgOld.GetImgWidth()) % GetImgWidth() ;

		// approximate to last col
		if (pTabX[i] >= imgOld.GetImgWidth()-1)
		{
			pTabX[i] = imgOld.GetImgWidth() - 2 ;
			pXMod[i] = GetImgWidth() - 1 ;
		}
	}

	// stretch pixel
	const int   nSpan = GetImgColorBits() / 8,
		nPitch = imgOld.GetImgPitch() ;
	if (progress)
		progress->ResetProgress() ; // reset to 0
	for (int y=0 ; y < GetImgHeight() ; y++)
	{
		int     nSrcY = y * imgOld.GetImgHeight() / GetImgHeight(),
			nYMod = (y * imgOld.GetImgHeight()) % GetImgHeight() ;
		if (nSrcY >= imgOld.GetImgHeight()-1) // approximate to last row
		{
			nSrcY = imgOld.GetImgHeight() - 2 ;
			nYMod = GetImgHeight() - 1 ;
		}

		const double un_y = nYMod / (double)GetImgHeight() ;
		BYTE * pWrite = GetBits(y) ;
		for (int x=0 ; x < GetImgWidth() ; x++, pWrite += nSpan)
		{
			// 计算原图对应点
			const int    nSrcX = pTabX[x],
				nXMod = pXMod[x] ;

			const BYTE   * pOldPix = imgOld.GetBits (nSrcX, nSrcY) ;
			if ((nXMod == 0) && (nYMod == 0))
			{
				CMFCColor::CopyPixel (pWrite, pOldPix, nSpan) ;
			}
			else
			{
				const BYTE  * pcrPixel[4] =
				{
					pOldPix, pOldPix + nSpan,
						pOldPix - nPitch, pOldPix - nPitch + nSpan
				} ;

				RGBQUAD crRet = CMFCColor::GetBilinearPixel(nXMod/(double)GetImgWidth(), un_y, GetImgColorBits() == 32, pcrPixel) ;
				CMFCColor::CopyPixel (pWrite, &crRet, nSpan) ;
			}
		}
		if (progress)
			progress->SetProgress (y * 100 / GetImgHeight()) ;
	}
}
//-----------------------------------------------------------------------------
void CMFCImageObj::PixelProcessProc(CMFCIPixelProcessor& rProcessor, CMFCObjProgress* pProgress)
{
	if (!rProcessor.ValidateColorBits(this))
		return;

	// before
	rProcessor.OnEnterProcess (this) ;
	if (pProgress)
		pProgress->ResetProgress() ; // reset to 0

	for(int y=0 ; y < GetImgHeight() ; y++)
	{
		for(int x=0 ; x < GetImgWidth() ; x++)
		{
			rProcessor.ProcessPixel(this, x, y, GetBits(x,y)) ;
		}
		if(pProgress)
			pProgress->SetProgress ((y+1) * 100 / GetImgHeight()) ;
	}

	// after
	rProcessor.OnLeaveProcess (this) ;
}
//-----------------------------------------------------------------------------
CMFCImageHandleFactory* CMFCImageObj::ManageImageHandleFactory(bool bGet,CMFCImageHandleFactory * pFactory)
{
	static std::auto_ptr<CMFCImageHandleFactory> s_pFactory(new CMFCFreeImageHandleFactory);
	return s_pFactory.get();
}
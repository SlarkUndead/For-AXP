
#include "stdafx.h"
#include "MFCBmpImageHandle.h"
#include "../MFCArray.h"
#include "../MFCBaseTemplate.h"
//=============================================================================
bool CMFCBmpImageHandle::LoadImageMemory(const BYTE* pStart, int nFileSize)
{
	if (!pStart || (nFileSize <= sizeof(MFCBMPHEADER)))
		return false ;
	if (((MFCBMPHEADER*)pStart)->bfType != 0x4D42)
		return false ;

	// size check
	const int nOffset = (int)((MFCBMPHEADER*)pStart)->bfOffBits ;
	if (nFileSize <= nOffset)
		return false ;

	// create image
	BITMAPINFOHEADER * pBmih = (BITMAPINFOHEADER*)(pStart + sizeof(MFCBMPHEADER)) ;
	CMFCImageObj * pImg = new CMFCImageObj ;
	if(!pImg->Create(pBmih))
	{
		delete pImg; return false;
	}

	// set palette
	if (pImg->GetImgColorBits() <= 8)
		pImg->SetColorTable(0, 1<<pImg->GetImgColorBits(),(RGBQUAD*)((BYTE*)pBmih + pBmih->biSize)) ;

	// set pixel
	int nCopyByte = pImg->GetImgPitch()*pImg->GetImgHeight() ;
	//assert (nFileSize - nOffset >= nCopyByte) ;
	nCopyByte = MFCMin(nCopyByte, nFileSize - nOffset) ;
	memcpy(pImg->GetMemStart(), pStart + nOffset, nCopyByte) ;
	PushObject(pImg) ; // pImg's ownership
	return true ;
}
//-----------------------------------------------------------------------------
bool CMFCBmpImageHandle::SaveImage(const char* szFileName, int nFlag)
{
	if (m_SaveImg.empty() || !m_SaveImg[0]->IsValidImage())
		return false ;
	const CMFCImageObj &img = *m_SaveImg[0] ;

	// create image file, if the file already exists, its contents are discarded.
	std::ofstream outFile (szFileName, std::ios::out|std::ios::binary|std::ios::trunc) ;
	if (!outFile.is_open())
		return false ;

	// calculate total size of bmp file.
	const int nPixs = img.GetImgPitch() * img.GetImgHeight() ;
	int nTotalSize = sizeof(MFCBMPHEADER) + sizeof(BITMAPINFOHEADER) + nPixs ;

	CMFCArray<BITMAPINFOHEADER> info(img.NewImgInfoWithPalette()) ;
	if (img.GetImgColorBits() <= 8)
		nTotalSize += (4 * (1<<img.GetImgColorBits())) ;
	else if (info.GetArrayPtr()->biCompression == BI_BITFIELDS)
		nTotalSize += 12 ;

	// write bmp file header
	MFCBMPHEADER bmpFileHeader ;
	bmpFileHeader.bfType      = 0x4D42 ; // "BM"
	bmpFileHeader.bfSize      = nTotalSize ;
	bmpFileHeader.bfReserved1 = bmpFileHeader.bfReserved2 = 0 ;
	bmpFileHeader.bfOffBits   = nTotalSize - nPixs ;
	outFile.write ((char*)&bmpFileHeader, sizeof(bmpFileHeader)) ;

	// write BMP file info / bitfields / palette
	outFile.write ((char*)info.GetArrayPtr(), nTotalSize - sizeof(MFCBMPHEADER) - nPixs) ;

	// write pixels value
	outFile.write ((char*)img.GetMemStart(), nPixs) ;
	outFile.close();
	return true ;
}
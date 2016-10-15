
#ifndef _MFC_TGA_IMAGEHANDLE_06_07_09_H_
#define _MFC_TGA_IMAGEHANDLE_06_07_09_H_
#include "MFCImageHandle.h"
#include "MFCCompressHelper.h"

//=============================================================================
// Read/Write TGA file.
class CMFCTgaImageHandle : public CMFCImageHandle
{
	virtual bool LoadImageMemory(const BYTE* pStart,int nFileSize) ;

	// param nFlag : 1(use RLE compress) / -1(not use), default(not use)
	virtual bool SaveImage(const char* szFileName, int nFlag = -1) ;

	// Definitions for TGA image types.
	enum
	{
		TGA_NULL    = 0,
		TGA_UCPAL   = 1,
		TGA_UCRGB   = 2,
		TGA_UCMONO  = 3,
		TGA_RLEPAL  = 9,
		TGA_RLERGB  = 10,
		TGA_RLEMONO = 11,
	};

	// TGA file header (same as BMP format, pixel store from left-bottom)
#pragma pack(1)
	struct TGAHEAD
	{
		BYTE	byID_Length ;	// 图像识别信息大小
		BYTE	byPalType ;		// 00 : no-palette     01 : has-palette
		BYTE	byImageType ;	// 见上
		WORD	wPalFirstNdx ;	// 调色板起始索引
		WORD	wPalLength ;	// 调色板长度
		BYTE	byPalBits ;		// 调色板中每一颜色所占位数
		WORD	wLeft ;			// 相对于屏幕左下角X坐标
		WORD	wBottom ;		// 相对于屏幕左下角Y坐标
		WORD	wWidth ;		// width
		WORD	wHeight ;		// height
		BYTE	byColorBits ;	// bpp
		struct
		{
			BYTE	AlphaBits : 4 ;	// 每像素Alpha Channel位数
			BYTE	HorMirror : 1 ;	// 为1表示图像数据左右颠倒存储
			BYTE	VerMirror : 1 ;	// 为1表示图像数据上下颠倒存储
			BYTE	Reserved  : 2 ;
		} Descriptor ;
	}; // 18 - Bytes
#pragma pack()
};

//=============================================================================
// inline Implement
//=============================================================================
inline bool CMFCTgaImageHandle::LoadImageMemory (const BYTE* pStart, int nFileSize)
{
	const TGAHEAD * pTga = (TGAHEAD*)pStart ;
	if(!pStart)
		return false ;
	if((pTga->byPalBits == 15) || (pTga->byPalBits == 16))
		return false ; // not support 15 or 16 bit palette

	// create image
	CMFCImageObj * pImg = new CMFCImageObj ;
	if(!pImg->Create(pTga->wWidth,pTga->wHeight,(pTga->byColorBits == 15) ? 16 : pTga->byColorBits))
	{
		delete pImg; return false ;
	}

	// set palette
	const BYTE * pCurr = pStart + sizeof(TGAHEAD) + pTga->byID_Length ;
	if(pTga->byPalType == 1)
	{
		if(pTga->wPalFirstNdx + pTga->wPalLength > 256)
		{
			delete pImg; return false ;
		}

		RGBQUAD pPal[256] = {0} ;
		for(int i=0 ; i < pTga->wPalLength ; i++)
		{
			MFC_B(&pPal[pTga->wPalFirstNdx + i]) = *pCurr++ ;
			MFC_G(&pPal[pTga->wPalFirstNdx + i]) = *pCurr++ ;
			MFC_R(&pPal[pTga->wPalFirstNdx + i]) = *pCurr++ ;
			if(pTga->byPalBits == 32)
				MFC_A(&pPal[pTga->wPalFirstNdx + i]) = *pCurr++ ;
		}
		pImg->SetColorTable (0, 256, pPal) ;
	}

	// start decode
	for(int i=0 ; i < pImg->GetImgHeight() ; i++)
	{
		BYTE * pDest ;
		if(pStart[17] & 0x20)
			pDest = pImg->GetBits(i) ; // top to bottom
		else
			pDest = pImg->GetBits(pImg->GetImgHeight() - 1 - i) ; // bottom to top

		if((pTga->byImageType == TGA_RLEPAL) || (pTga->byImageType == TGA_RLERGB) || (pTga->byImageType == TGA_RLEMONO)) // 压缩
		{
			pCurr = CMFCCompressHelper::RLE_TGA_DecodeLine (pCurr, pImg->GetImgColorBits(), pImg->GetImgWidth(), pDest) ;
		}
		else // not-compressed
		{
			int nPitch = pImg->GetImgWidth() * pImg->GetImgColorBits() / 8 ;
			memcpy (pDest, pCurr, nPitch) ;
			pCurr += nPitch ;
		}
	}
	PushObject(pImg) ; // pImg's ownership
	return true ;
}
//-----------------------------------------------------------------------------
inline bool CMFCTgaImageHandle::SaveImage(const char* szFileName,int nFlag)
{
	if (m_SaveImg.empty())
		return false ;
	const CMFCImageObj &img = *m_SaveImg[0] ;

	// validate
	if((img.GetImgColorBits() != 8) && (img.GetImgColorBits() != 16) && (img.GetImgColorBits() != 24) && (img.GetImgColorBits() != 32))
		return false ;

	// create image file
	std::ofstream outFile (szFileName, std::ios::out|std::ios::binary|std::ios::trunc) ;
	if(!outFile.is_open())
		return false ;

	// Initialize TGA Header
	const BYTE fTgaInfo[] = {"PhoXo -- TGA"} ;
	TGAHEAD TgaHead ;
	memset(&TgaHead, 0, sizeof(TgaHead)) ;
	TgaHead.byID_Length = sizeof(fTgaInfo) - 1 ; // tga size
	TgaHead.byPalType   = ((img.GetImgColorBits() == 8) ? 1 : 0) ;

	if(nFlag == -1)
		TgaHead.byImageType = ((img.GetImgColorBits() == 8) ? TGA_UCPAL : TGA_UCRGB) ;
	else if(nFlag == 1)
		TgaHead.byImageType = ((img.GetImgColorBits() == 8) ? TGA_RLEPAL : TGA_RLERGB) ;

	TgaHead.wPalFirstNdx    = 0 ;
	TgaHead.wPalLength      = 256 ;
	TgaHead.byPalBits       = 24 ; // palette's bit
	TgaHead.wWidth          = img.GetImgWidth() ;
	TgaHead.wHeight         = img.GetImgHeight() ;
	TgaHead.byColorBits     = (BYTE)img.GetImgColorBits() ;
	((BYTE*)&TgaHead)[17]   = 0x20 ; // top to bottom
	outFile.write((char*)&TgaHead, sizeof(TGAHEAD)) ;
	outFile.write((char*)fTgaInfo, TgaHead.byID_Length) ;

	// write palette
	if(img.GetImgColorBits() == 8)
	{
		RGBQUAD pPal[256] ;
		img.GetColorTable (0, 256, pPal) ;
		for(int i=0 ; i < 256 ; i++)
		{
			outFile.write ((char*)&pPal[i], 3) ;
		}
	}

	// write pixels
	const int nLineByte = img.GetImgColorBits() * img.GetImgWidth() / 8 ;
	if(nFlag == -1)
	{
		// not compress
		for(int y=0 ; y < img.GetImgHeight() ; y++)
		{
			outFile.write ((char*)img.GetBits(y), nLineByte) ;
		}
	}
	else if(nFlag == 1)
	{
		// RLE compress
		CMFCArray<BYTE> pStart(nLineByte * 2 + 4096) ;
		for(int y=0 ; y < img.GetImgHeight() ; y++)
		{
			BYTE  * pEn = CMFCCompressHelper::RLE_TGA_EncodeLine (img.GetBits(y), img.GetImgColorBits(),img.GetImgWidth(),pStart.GetArrayPtr()) ;
			outFile.write((char*)pStart.GetArrayPtr(),(int)(pEn - pStart.GetArrayPtr())) ;
		}
	}
	outFile.close();
	return true ;
}
#endif
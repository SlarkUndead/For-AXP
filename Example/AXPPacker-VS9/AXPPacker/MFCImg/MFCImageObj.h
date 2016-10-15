#ifndef _MFC_IMAGEOBJECT_060701_H_
#define _MFC_IMAGEOBJECT_060701_H_

#include "MFCStdDefine.h"
#include "MFCBaseObject.h"

enum MFC_IMAGE_TYPE;
enum LOGICAL_OP;

class CMFCObjProgress;
class CMFCIPixelProcessor;
class CMFCIImageHandle;
class CMFCImageHandleFactory;


//=============================================================================
// Basic image object.
// 1) the origin (0,0) of image lies at left-top point.
// 2) all coordinate in this class (POINT, RECT...) relative to image's (0,0).
// 3) after image be created, pixel's data are automatically initialized to 0. and if
//    image's bpp <= 8, a gray palette will be set automatically.
// 4) if you want to use Save / Load, you must call SetImageHandleFactory to set a handle factory.
class CMFCImageObj : public CMFCGraphObj
{
public:
	CMFCImageObj();	// Create an empty image.
	CMFCImageObj (const CMFCImageObj& img) ;// Copy constructor.
	 CMFCImageObj (int nWidth, int nHeight, int nColorBit) ;// Create an image.
	 CMFCImageObj (const char* szFileName) ;// Create an image from a image file.
	 CMFCImageObj& operator= (const CMFCImageObj& img) ;
	 virtual ~CMFCImageObj() {Destroy();}

	 virtual int Serialize(bool bSave, BYTE* pSave) ; // Serialize image object in memory (bpp > 8).

	 // Create/Destroy image.
	 // Create image.
	 // 1) if image's bpp <= 8, a gray palette will be set automatically.
	 // 2) after image be created, all pixels are automatically initialized to 0.
	 bool Create(const BITMAPINFOHEADER* pBmif);
	 bool Create(int nWidth, int nHeight, int nColorBit);// Create image.
	 bool Destroy(void);// Destroy image object.


	 // Basic attributes.
	 // Is current image object is valid.
	 bool IsValidImage(void) const{return (m_pByte != 0);}
	 BYTE* GetBits(int iLine) const ; // Don't do boundary check!.
	 BYTE* GetBits(int x, int y) const ;// Don't do boundary check!.
	 // Get image's pixel start-address (address of left-bottom point).
	 BYTE* GetMemStart(void) const {return m_pByte;}
	 // Pixel width of image.
	 int GetImgWidth(void)const {return m_DibInfo.biWidth;}
	 // Pixel height of image.
	 int GetImgHeight(void)const{return m_DibInfo.biHeight;}
	 // Image's bpp (bit per pixel), available : 1,4,8,16,24,32.
	 WORD GetImgColorBits(void)const{return m_DibInfo.biBitCount;}
	 // Bytes pitch between two lines (the value is 4-bytes rounded).
	 int GetImgPitch(void)const{return 4 * ((GetImgWidth() * GetImgColorBits() + 31) / 32);}

	 // Create BITMAPINFOHEADER struct.
	 // palette(<=8bit) or 3-bit-fields(16bit) was appended,
	 // you must use C++ delete[] to delete returned point.
	 // you are high recommended to use CMFCArray wrap this returned pointer. sample:
	 // CMFCArray<BITMAPINFOHEADER> imgInfo (img.NewImgInfoWithPalette()) ;
	 // imgInfo.get()->biWidth ;
	 // pPalette = imgInfo.get() + 1 ;
	 BITMAPINFOHEADER* NewImgInfoWithPalette(void) const ;
	 // Get millisecond to next frame.
	 int GetNextFrameDelay(void) const {return m_nNextFrameDelay;}
	 // Set millisecond to next frame.
	 void SetNextFrameDelay (int nNextFrameDelay) {m_nNextFrameDelay=nNextFrameDelay;}
	 // Bound rc into image.
	 // param rc : coordinate in image.
	 void BoundRect(RECT& rc) const ;

	 // Pixel access.
	 // Is point(x,y) in image.
	 bool IsInside(int x, int y) const {return (x>=0) && (x<GetImgWidth()) && (y>=0) && (y<GetImgHeight());}
	 DWORD GetPixelData(int x, int y) const ;// Get pixel data at (x,y).
	 void SetPixelData(int x, int y, DWORD dwPixel) ;// Set pixel data at (x,y).

	 // Palette operations (bpp : 1,4,8)
	 // Get palette of image.
	 // the iFirstIndex is A zero-based color table index.
	 bool GetColorTable (int iFirstIndex, int iNumber, RGBQUAD* pColors) const ;
	 // Set palette of image.
	 // the iFirstIndex is A zero-based color table index.
	 bool SetColorTable(int iFirstIndex, int iNumber, RGBQUAD* pColors) ;
	 void CopyPalette(const CMFCImageObj& imgSrc) ;// Duplicate palette from imgSrc.

	 // Color convert.
	 // Convert current image's bpp to 24.
	 void ConvertTo24Bit(void){ConvertToTrueColor(24);}
	 // Convert current image's bpp to 32 (alpha channel will set 0xFF).
	 void ConvertTo32Bit(void) {ConvertToTrueColor(32);}

	 // Channel operations (bpp == 32)
	 // Get alpha channel of 32bpp image (imgAlpha's bpp == 8).
	 void GetAlphaChannel(CMFCImageObj* imgAlpha) const ;
	 // Set alpha channel of 32bpp image (alpha8's bpp == 8).
	 void AppendAlphaChannel(const CMFCImageObj& alpha8) ;
	 // Set 32bpp image's alpha channel value.
	 void SetAlphaChannelValue(int nValue) ;

	 // Basic transform.
	 // Get a block of image (bpp >= 8).
	 // if rcBlock exceed image, it will be bounded in image.
	 bool GetSubBlock(CMFCImageObj* SubImg, RECT rcBlock) const ;
	 // Cover Img on position (x,y) (bpp >= 8).
	 bool CoverBlock (const CMFCImageObj& Img, int x, int y) ;
	 void TileBlock (const CMFCImageObj& Img, int x, int y) ;
	 // Alpha blend image (bpp >= 24).
	 void AlphaBlend (const CMFCImageObj& img32, const RECT& rcDest, const RECT& rcSrc, int nAlphaPercent) ;
	 // Combine image (bpp == 32).
	 void CombineImage(const CMFCImageObj& img32, int x=0, int y=0, int nAlphaPercent=100) ;
	 void LogicalBlend(const CMFCImageObj & MaskImg, LOGICAL_OP LogOP, int x=0, int y=0) ;
	 void ExpandFrame(bool bCopyEdge,int iLeft,int iTop,int iRight,int iBottom) ; // add frame:bpp >= 8.
	 void Stretch (int nNewWidth,int nNewHeight) ; // stretch:bpp >= 8.
	 void Stretch_Smooth(int nNewWidth,int nNewHeight,CMFCObjProgress * progress = 0); // stretch:bpp >= 24.
	 // Perform a processor. more detail refer MFCBasePixelProcessor.h
	 void PixelProcessProc(CMFCIPixelProcessor& rProcessor, CMFCObjProgress* pProgress=0) ;

	 // Read/Write image file.
	 // Set image handler factory.
	 // You must use new to create object and after the object be setted, you can't delete it any more, frame will delete it.
	 // CMFCImageObj::SetImageHandleFactory (new FCImageHandleFactory_FreeImage) ;
	 static void SetImageHandleFactory(CMFCImageHandleFactory* pFactory){ManageImageHandleFactory(false,pFactory);}
	 // Get image handler factory.
	 static CMFCImageHandleFactory * GetImageHandleFactory(){return ManageImageHandleFactory(true,0);}
	
	 bool Load(CMFCIImageHandle* pHandler) ; // Load image.
	 bool Load(const char* szFileName);// Load image(determine image type by file's ext name).
	 bool Load (BYTE* pStart, int iFileSize, MFC_IMAGE_TYPE imgType);// Load image.
	 // Save image (determine image type by file's ext name).
	 bool Save(CMFCIImageHandle* pHandler, const char* szFileName, int nFlag = -1) const ;

	 // Save image (determine image type by file's ext name).
	 // nFlag depends on the image type.
	 // TGA : 1(use RLE compress) / -1(not use), default(not use)
	 // JPG : compress quality [1..100], default(82)
	 bool Save (const char* szFileName, int nFlag = -1) const ;

private:
	BITMAPINFOHEADER   m_DibInfo ;			// DIB Info
	BYTE             * m_pByte ;			// Bitmap start bits, from left-bottom start
	BYTE            ** m_ppLine ;			// Line-pointer, ppLine[] ; from top to bottom
	DWORD              m_dwBitFields[3] ;	// only 16bit image, order R,G,B
	RGBQUAD          * m_pPalette ;			// palette
	int                m_nNextFrameDelay ;	// to next frame delay milliseconds

private:
	void SetGrayPalette() ;
	void InitClassMember() ; // initialize the member variant
	void ConvertToTrueColor (int iColor) ; // iColor == 24 or 32
	static RGBQUAD Split16Bit_565 (WORD wPixel) ;
	static RGBQUAD Split16Bit_555 (WORD wPixel) ;
	static void FillImageRect(const CMFCImageObj& img, const RECT& rcBlock, const void* pSrc) ;
	static CMFCImageHandleFactory* ManageImageHandleFactory(bool bGet,CMFCImageHandleFactory * pFactory);
};

//=============================================================================
// inline Implement
//=============================================================================
inline void CMFCImageObj::InitClassMember()
{
	memset (&m_DibInfo, 0, sizeof(m_DibInfo)) ;
	m_dwBitFields[0]=m_dwBitFields[1]=m_dwBitFields[2]=0 ;
	m_pByte=0 ; m_ppLine=0 ; m_pPalette=0 ;

	m_nNextFrameDelay = 0 ;
}
//-----------------------------------------------------------------------------
inline CMFCImageObj::CMFCImageObj()
{
	InitClassMember() ;
}
inline CMFCImageObj::CMFCImageObj(const CMFCImageObj& img)
{
	InitClassMember() ;
	*this = img ;
}
inline CMFCImageObj::CMFCImageObj(int nWidth, int nHeight, int nColorBit)
{
	InitClassMember() ;
	Create(nWidth, nHeight, nColorBit) ;
}
inline CMFCImageObj::CMFCImageObj(const char* szFileName)
{
	InitClassMember() ;
	Load (szFileName) ;
}
//-----------------------------------------------------------------------------
inline bool CMFCImageObj::Create(int nWidth, int nHeight, int nColorBit)
{
	BITMAPINFOHEADER     bmih ;
	memset (&bmih, 0, sizeof(bmih)) ;
	bmih.biWidth = nWidth ;
	bmih.biHeight = nHeight ;
	bmih.biBitCount = nColorBit ;
	bmih.biCompression = BI_RGB ;
	return this->Create(&bmih) ;
}

//-----------------------------------------------------------------------------
inline BYTE* CMFCImageObj::GetBits (int iLine) const // ·¶Î§¼ì²â
{
	//assert (IsInside(0,iLine)) ;
	return m_ppLine[iLine] ;
}
//-----------------------------------------------------------------------------
inline BYTE* CMFCImageObj::GetBits (int x, int y) const
{
	//assert (IsInside(x,y)) ;
	if (GetImgColorBits() == 32)
		return (m_ppLine[y] + x * 4) ;
	if (GetImgColorBits() == 8)
		return (m_ppLine[y] + x) ;
	return (m_ppLine[y] + x * GetImgColorBits() / 8) ;
}
//-----------------------------------------------------------------------------
inline void CMFCImageObj::BoundRect (RECT& rc) const
{
	RECT rcImg = {0, 0, GetImgWidth(), GetImgHeight()} ;
	::IntersectRect(&rc, &rcImg, &rc) ;
}
//-----------------------------------------------------------------------------
inline void CMFCImageObj::CopyPalette(const CMFCImageObj& imgSrc)
{
	if (!IsValidImage() || (GetImgColorBits() > 8) || (GetImgColorBits() != imgSrc.GetImgColorBits()))
		return;

	RGBQUAD pPal[256] ;
	int nNum = 1 << imgSrc.GetImgColorBits() ;
	imgSrc.GetColorTable (0, nNum, pPal) ;
	SetColorTable(0, nNum, pPal) ;
}
//-----------------------------------------------------------------------------
inline void CMFCImageObj::SetGrayPalette()
{
	if (!IsValidImage() || (GetImgColorBits() > 8))
		return;

	// calculate palette
	RGBQUAD pPal[256] ;
	const int nNum = 1 << GetImgColorBits(),
		nSpan = 255 / (nNum - 1) ;
	for(int i=0 ; i < nNum ; i++)
	{
		MFC_R(&pPal[i]) = MFC_G(&pPal[i]) = MFC_B(&pPal[i]) = i * nSpan ;
	}
	SetColorTable (0, nNum, pPal) ;
}

//-----------------------------------------------------------------------------
inline RGBQUAD CMFCImageObj::Split16Bit_565(WORD wPixel)
{
	RGBQUAD     rgb ;
	MFC_R(&rgb) = (MFC_MASK16_RED_565 & wPixel) >> 8 ;
	MFC_G(&rgb) = (MFC_MASK16_GREEN_565 & wPixel) >> 3 ;
	MFC_B(&rgb) = (MFC_MASK16_BLUE_565 & wPixel) << 3 ;
	return rgb ;
}
//-----------------------------------------------------------------------------
inline RGBQUAD CMFCImageObj::Split16Bit_555 (WORD wPixel)
{
	RGBQUAD     rgb ;
	MFC_R(&rgb) = (MFC_MASK16_RED_555 & wPixel) >> 7 ;
	MFC_G(&rgb) = (MFC_MASK16_GREEN_555 & wPixel) >> 2 ;
	MFC_B(&rgb) = (MFC_MASK16_BLUE_555 & wPixel) << 3 ;
	return rgb ;
}
#endif
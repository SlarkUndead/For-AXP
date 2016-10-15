
#ifndef _MFC_IMAGECOMPRESSHELPER_06_07_09_H_
#define _MFC_IMAGECOMPRESSHELPER_06_07_09_H_

//=============================================================================
// Compress helper class.
class CMFCCompressHelper
{
public:
	// pOutBuffer must at least double size than pInBuffer.
	// return bytes of written to pOutBuffer.
	static int RLE_PCX_Encode(const BYTE* pInBuffer,int nInSize,BYTE* pOutBuffer) ;

	// pOutBuffer must large enough.
	// return bytes of written to pOutBuffer.
	static int RLE_PCX_Decode (const BYTE* pInBuffer, int nInSize, BYTE* pOutBuffer) ;

	// pOutBuffer must at least double size than pInBuffer.
	// param iColorBit : bpp - 8, 16, 24, 32
	// param iNumPixel : pixel width of image line
	// return current pOutBuffer position.
	static BYTE* RLE_TGA_EncodeLine (const BYTE* pInBuffer,int iColorBit,int iNumPixel,BYTE* pOutBuffer) ;

	// pOutBuffer must large enough.
	// param iColorBit : bpp - 8, 16, 24, 32
	// param iNumPixel : pixel width of image line
	// return current pInBuffer position.
	static BYTE* RLE_TGA_DecodeLine(const BYTE* pInBuffer,int iColorBit,int iNumPixel,BYTE* pOutBuffer) ;
};

#endif
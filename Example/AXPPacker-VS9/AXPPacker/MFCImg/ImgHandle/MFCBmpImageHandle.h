#ifndef _MFC_BMP_IMAGEHANDLE_06_07_09_H_
#define _MFC_BMP_IMAGEHANDLE_06_07_09_H_

#include "MFCImageHandle.h"


class CMFCBmpImageHandle : public CMFCImageHandle
{
	virtual bool LoadImageMemory(const BYTE* pStart, int nFileSize) ;
	virtual bool SaveImage(const char* szFileName, int nFlag = -1) ;

#pragma pack(2)
	struct MFCBMPHEADER
	{
		WORD    bfType;
		DWORD   bfSize;
		WORD    bfReserved1;
		WORD    bfReserved2;
		DWORD   bfOffBits;
	};
#pragma pack()
};

#endif
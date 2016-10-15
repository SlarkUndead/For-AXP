
#ifndef _MFC_FREE_IMAGEHANDLEFACTORY_06_07_08_H_
#define _MFC_FREE_IMAGEHANDLEFACTORY_06_07_08_H_

#include "MFCBmpImageHandle.h"
#include "MFCTgaImageHandle.h"
#include "MFCFreeImageHandle.h"
//=============================================================================
// Read/Write image via FreeImage lib(Need FreeImage lib).
// BMP      TGA      Jpg      Gif      Tif      Png      Pcx      Ico      Xpm      Psd		  DDS
// Read       O        O        O        O        O        O        O        O        O        O
// Write      O        O        O        O        O        O        O        O        O        0
class CMFCFreeImageHandleFactory : public CMFCImageHandleFactory
{
protected:
	virtual CMFCIImageHandle * CreateImageHandle(MFC_IMAGE_TYPE imgType)
	{
		switch (imgType)
		{
		case MFC_IMG_BMP : return new CMFCBmpImageHandle  ;
		case MFC_IMG_TGA : return new CMFCFreeImageHandle ;//CMFCTgaImageHandle  ;
		case MFC_IMG_GIF : return new CMFCFreeImageHandle ;
		case MFC_IMG_PCX : return new CMFCFreeImageHandle ;
		case MFC_IMG_PNG : return new CMFCFreeImageHandle ;
		case MFC_IMG_TIF : return new CMFCFreeImageHandle ;
		case MFC_IMG_JPG : return new CMFCFreeImageHandle ;
		case MFC_IMG_ICO : return new CMFCFreeImageHandle ;
		case MFC_IMG_XPM : return new CMFCFreeImageHandle ;
		case MFC_IMG_PSD : return new CMFCFreeImageHandle ;
		case MFC_IMG_DDS : return new CMFCFreeImageHandle ;
		}
		return 0 ;
	}
	virtual ~CMFCFreeImageHandleFactory() {}
};
#endif
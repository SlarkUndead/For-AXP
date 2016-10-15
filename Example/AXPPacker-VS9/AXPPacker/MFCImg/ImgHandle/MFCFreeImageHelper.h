#ifndef _MFC_FREEIMAGEHELPER_06_07_08_H_
#define _MFC_FREEIMAGEHELPER_06_07_08_H_

struct FIBITMAP;
class CMFCImageObj;

//=============================================================================
bool FreeImageToMFCImageObj(FIBITMAP* pFIimg, CMFCImageObj& img);
FIBITMAP * AllocateFreeImage(const CMFCImageObj& img);

#endif
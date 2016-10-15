
#ifndef _MFC_IPICTURE_IMAGEHANDLE_06_07_09_H_
#define _MFC_IPICTURE_IMAGEHANDLE_06_07_09_H_
#include "MFCImageHandle.h"

//=============================================================================
// Use WIN32 IPicture to read/write image handler.
class CMFCIPictureImageHandle
{
	virtual bool LoadImageMemory (const BYTE* pStart, int nFileSize)
	{
		if (!pStart || (nFileSize <= 0))
			return false ;

		HGLOBAL hBuffer = ::GlobalAlloc(GMEM_MOVEABLE,nFileSize) ;

		// copy buffer to HGLOBAL
		::CopyMemory(::GlobalLock(hBuffer), pStart, nFileSize) ;
		::GlobalUnlock(hBuffer) ;

		IStream    * pStream = 0 ;
		IPicture   * pIPic = 0 ;
		CMFCImageObj * pImg = new CMFCImageObj ;
		if(::CreateStreamOnHGlobal(hBuffer,TRUE,&pStream) == S_OK)
		{
			if(::OleLoadPicture (pStream,nFileSize,FALSE,IID_IPicture,(LPVOID*)&pIPic) == S_OK)
			{
				OLE_HANDLE hDDB ;
				pIPic->get_Handle (&hDDB) ;

				BITMAP bm ;
				GetObject((HGDIOBJ)hDDB, sizeof(BITMAP), &bm) ;
				if(pImg->Create(bm.bmWidth, bm.bmHeight, 24))
				{
					// dest format
					CMFCArray<BITMAPINFO> bmfh(pImg->NewImgInfoWithPalette()) ;
					HDC hdc = ::GetDC(0) ;
					::GetDIBits (hdc,(HBITMAP)hDDB,0,bm.bmHeight,pImg->GetMemStart(),bmfh.GetArrayPtr(),DIB_RGB_COLORS) ;
					::ReleaseDC(0, hdc) ;
				}
				pIPic->Release() ;
			}
			pStream->Release() ;
		}

		if(!pImg->IsValidImage())
		{
			delete pImg; return false;
		}
		PushObject(pImg) ; // pImg's ownership
		return true ;
	}
};
#endif
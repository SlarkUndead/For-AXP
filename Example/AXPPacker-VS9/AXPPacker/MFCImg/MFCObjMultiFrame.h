#ifndef _MFC_OBJECT_MULTIFRAME_060706_H_
#define _MFC_OBJECT_MULTIFRAME_060706_H_
#include "MFCImageObj.h"
#include "MFCIComposite.h"

class CMFCObjMultiFrame:public CMFCObject,public CMFCIComposite<CMFCImageObj>
{
public:
	// Get image's number.
	//int GetFrameCount() const {return PCL_GetObjectCount();}
	CMFCImageObj* GetFrame(int nFrame) const {return GetObject(nFrame);}// Get image.

	// Read/Write image file,determine image type by file's ext name.
	//bool Load(FCImageHandleBase& rHandler) ;
	bool Load(const char* szFileName) ;
	bool Load(BYTE* pStart, int iFileSize, MFC_IMAGE_TYPE imgType) ;
};

#endif
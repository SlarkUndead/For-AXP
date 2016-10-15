
#ifndef _MFC_IMAGEHANDLE_06_07_08_H_
#define _MFC_IMAGEHANDLE_06_07_08_H_
#include "../MFCIComposite.h"
#include "MFCIImageHandle.h"
#include "../MFCImageObj.h"

//=============================================================================
// It's a middleman.
class CMFCImageHandle : public CMFCIImageHandle,public CMFCIComposite<CMFCImageObj>
{
public:
	virtual std::deque<CMFCImageObj*> GetLoadImage(bool bWantBeOwnner)
	{
		std::deque<CMFCImageObj*> imgList ;
		if(bWantBeOwnner)
			ThrowOwnership(imgList) ;
		else
		{
			for (int i=0 ; i < GetObjectCount() ; i++)
			{
				imgList.push_back(GetObject(i)) ;
			}
		}
		return imgList ;
	}
};
#endif
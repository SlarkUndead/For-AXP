#ifndef _MFC_IMAGEHANDLEFACTORY_06_07_08_H_
#define _MFC_IMAGEHANDLEFACTORY_06_07_08_H_

class CMFCIImageHandle ; // external class

//=============================================================================
// Interface of image handler's factory.
// See FCObjImage::SetImageHandleFactory.
class CMFCImageHandleFactory
{
public:
	// Create image handler by image type.
    // User must use delete to delete returned handler.
    // you are high recommended to use std::auto_ptr wrap this returned pointer.
	// sample:
    // MFC_IMAGE_TYPE imgType = CMFCArchiveHelper::GetImageTypeFromName(szFileName) ;
    // std::auto_ptr<CMFCIImageHandle>  pHandler (CMFCImageObj::GetImageHandleFactory()->CreateImageHandle(imgType)) ;
    virtual CMFCIImageHandle* CreateImageHandle(MFC_IMAGE_TYPE imgType) =0 ;
    virtual ~CMFCImageHandleFactory() {}
};

#endif

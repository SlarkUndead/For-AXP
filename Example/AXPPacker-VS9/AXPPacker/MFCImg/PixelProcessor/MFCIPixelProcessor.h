#ifndef _MFC_INTERFACE_PIXEL_PROCESSOR_06_07_09_H_
#define _MFC_INTERFACE_PIXEL_PROCESSOR_06_07_09_H_

#include "../MFCStdDefine.h"
class CMFCImageObj ; // external class

//=============================================================================
// Pixel processor interface.
class CMFCIPixelProcessor
{
public:
    virtual ~CMFCIPixelProcessor() {}

    // Whether the image can be disposed by this processor.
    virtual bool ValidateColorBits(const CMFCImageObj* pImg) =0 ;

    // Before process.
    virtual void OnEnterProcess(CMFCImageObj* pImg) {}
    // Process (x,y) pixel
    virtual void ProcessPixel(CMFCImageObj* pImg, int x, int y, BYTE* pPixel) {}
    // After process.
    virtual void OnLeaveProcess(CMFCImageObj* pImg) {}
};

//=============================================================================
// inline Implement
//=============================================================================

#endif

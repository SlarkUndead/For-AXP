#ifndef _MFC_IIMAGEHANDLE_06_07_08_H_
#define _MFC_IIMAGEHANDLE_06_07_08_H_


class CMFCImageObj ; // external class

//=============================================================================
// Interface of image handle.
// Used by CMFCImageObj::Load function, to implement read/write image file.
class CMFCIImageHandle
{
public:
    // Read image from file.
    // provide a default implement : call LoadImageMemory after load file into memory
    virtual bool LoadImageFile(const char* szFileName) ;

    // Read image from memory.
    virtual bool LoadImageMemory(const BYTE* pStart, int nFileSize) {return false;}

    // Save image to file.
    // User implement save function stored in m_SaveImg.
    virtual bool SaveImage(const char* szFileName, int nFlag = -1) {return false;}

    // Get loaded image list (such as .gif).
    // bWantBeOwnner : true - the handle will throw the ownership of these image, you must delete youself.
    virtual std::deque<CMFCImageObj*> GetLoadImage(bool bWantBeOwnner) =0 ;

    // Add image object to save list.
    void PushSaveList(const CMFCImageObj* pImg){m_SaveImg.push_back(pImg);}

    virtual ~CMFCIImageHandle(){m_SaveImg.clear();}
protected:
    // Image list to be saved.
    std::deque<const CMFCImageObj*> m_SaveImg ; // NOTE:≤ªø…“‘…æ≥˝!!!
} ;

#endif

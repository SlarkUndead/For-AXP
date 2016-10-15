
#ifndef _MFC_ARCHIVE_HELPER_060705_H_
#define _MFC_ARCHIVE_HELPER_060705_H_
#include "MFCStdDefine.h"
#include "MFCEnum.h"
#include "MFCBaseTemplate.h"

//=============================================================================
 // Some Archive helper funtion.
class CMFCArchiveHelper
{
public:
    // Memory helper.
    static BYTE* ZeroMalloc(int nBytes) ; // Allocate memory with initialized to zero.
    static void ZeroFree(void* pPixel) ;// Free memory alloc by ZeroMalloc.

    // File helper.
    static void SaveBufferToFile(const char* szFilename,const char* pBuffer,int nLength);// Save buffer to file.
    static void LoadFileToBuffer(const char* szFilename,char*& pBuffer,int& nLength) ;// Load file to memory (you must use delete[] to free returned pBuffer).


    // int/long/float/double ==> ASCII
    template<class T> static std::string NumberToASCII(const T& tNumber, int nWidth=0)
    {
        std::ostringstream sOutStream ;
        sOutStream.width(nWidth) ;
        sOutStream.fill ('0') ;
        sOutStream << tNumber ;
        return sOutStream.str() ;
    }
    // ASCII ==> int/long/float/double
    template<class T> static void ASCIIToNumber(const std::string& strNumber, T& tResult)
    {
        tResult = (T)0 ;
        std::stringstream     sTranslation ;
        sTranslation << strNumber ;
        sTranslation >> tResult ;
    }
   
    static std::string EncodeURL (const char* pBufDummy, int nLen) // URL encode.
    {
        const unsigned char  * pBuf = (const unsigned char*)pBufDummy ;
        static std::string   s_HexTable = "0123456789ABCDEF",
                             s_UnsafeTable = "\"<>%\\^[]`+$,@:;/!#?=&" ;
        std::string   s ;
        for(int i=0 ; i < nLen ; i++)
		{
            if((pBuf[i] > 32) && (pBuf[i] < 123) && (s_UnsafeTable.find(pBuf[i]) == std::string::npos))
            {
                s += pBuf[i] ;
            }
            else
            {
                s += "%" ;
                s += s_HexTable[pBuf[i] / 16] ;
                s += s_HexTable[pBuf[i] % 16] ;
            }
		}
        return s ;
    }

    static std::string GetFileExt(const char* pFile) ;					// Get filename's ext name.
    static MFC_IMAGE_TYPE GetImageTypeFromName(const char* szFileName) ;// Get image file's format by ext name.

    /**
     *  @name File type.
     */
    //@{
    // Load Photoshop's .acf file.
    static bool LoadPhotoshopACF(const char* szFileName, std::deque<int>& listElem);
    // Save Photoshop's .acf file.
    static bool SavePhotoshopACF(const char* szFileName,const std::deque<int>& listElem) ;

    // PhoXo setting file format
    struct PHOXOSETTING
    {
        INT32       nUndoLevel ;	// Undo级数, default(20)
        INT32       nBkType ;		// 画布背景类型，0(颜色)，1(检测板)，2(粉红棋盘)
        RGBQUAD     crCanvasBk ;	// canvas背景色
        INT32       nNewWidth ;		// 新建图像宽
        INT32       nNewHeight ;	// 新建图像高
        INT32       nResolution ;	// 新建图像解析度
        RGBQUAD     crFore ;		// 前景色
        RGBQUAD     crBack ;		// 背景色
        INT32       nIsShowLogo ;	// 启动时是否显示logo
        INT32       nAntStepTime ;	// 蚂蚁线间隔时间毫秒数
        INT32       nIdLanguage ;	// 当前OS语言
    };
    
    static void LoadPhoXoSettingFile(const char* szFileName,PHOXOSETTING& oXoSetting);		// Load PhoXo's setting.
    static void SavePhoXoSettingFile(const char* szFileName,const PHOXOSETTING& oXoSetting);// Save PhoXo's setting.
    static void BubbleSort(int pArray[],int iNumElement) ;// Output is from small to big.

private:
    static int __PS_WORD2INT (const BYTE* pWord) ;
};

//=============================================================================
// inline Implement
//=============================================================================
inline BYTE* CMFCArchiveHelper::ZeroMalloc (int nBytes)
{
#ifdef WIN32
    // BoundChecker can's check the memory alloc by <VirtualAlloc>
    #ifdef _DEBUG
        BYTE * pByte = (BYTE*)malloc(nBytes) ;
        memset(pByte, 0, nBytes) ;
        return pByte ;
    #else
        return (BYTE*)VirtualAlloc (0,nBytes,MEM_RESERVE|MEM_COMMIT,PAGE_READWRITE) ;
    #endif
#else
        BYTE * pByte = (BYTE*)malloc(nBytes) ;
        memset(pByte, 0, nBytes) ;
        return pByte ;
#endif
}
//-----------------------------------------------------------------------------
inline void CMFCArchiveHelper::ZeroFree(void* pPixel)
{
    if (!pPixel)
        return ;

#ifdef WIN32
    #ifdef _DEBUG
        free (pPixel) ;
    #else
        ::VirtualFree (pPixel,0,MEM_RELEASE) ;
    #endif
#else
        free (pPixel) ;

#endif
}
//-----------------------------------------------------------------------------
inline void CMFCArchiveHelper::SaveBufferToFile(const char* szFilename,const char* pBuffer,int nLength)
{
   // std::ofstream outFile(szFilename,std::ios::out|std::ios::binary|std::ios::trunc) ;
	std::ofstream outFile(fopen(szFilename, "wb"));
    if(outFile.is_open())
        outFile.write (pBuffer,nLength) ;
	outFile.close();
}
//-----------------------------------------------------------------------------
inline void CMFCArchiveHelper::LoadFileToBuffer (const char* szFilename, char*& pBuffer, int& nLength)
{
    pBuffer=0; nLength=0;
	std::ifstream inFile(fopen(szFilename, "rb"));
    if(!inFile.is_open())
		return;

    // get file length
    inFile.seekg (0, std::ios::end) ;
    nLength = inFile.tellg() ;

    // read file into memory
    pBuffer = new char[nLength + 8] ;
    memset(pBuffer, 0, nLength + 8) ;
    inFile.seekg(0, std::ios::beg) ;
    inFile.read (pBuffer,nLength) ;
	inFile.close();
}
//-----------------------------------------------------------------------------
inline std::string CMFCArchiveHelper::GetFileExt (const char* pFile)
{
    if(!pFile) return "";
    std::string     strFile(pFile), strOut ;
    size_t nPos = strFile.find_last_of (".") ;
    if(nPos != std::string::npos)
        strOut = strFile.substr(nPos + 1) ;
    return strOut ;
}
//-----------------------------------------------------------------------------
inline MFC_IMAGE_TYPE CMFCArchiveHelper::GetImageTypeFromName(const char* szFileName)
{
	static bool bImgTypeNo = true;
	static std::map<std::string,MFC_IMAGE_TYPE> ImgTypeMap;
    if(!szFileName) return MFC_IMG_UNKNOW;

    std::string str(GetFileExt(szFileName)) ;
	std::transform(str.begin(),str.end(),str.begin(),tolower);
	if(bImgTypeNo)
	{
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("jpg",MFC_IMG_JPG));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("jpeg",MFC_IMG_JPG));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("gif",MFC_IMG_GIF));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("png",MFC_IMG_PNG));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("bmp",MFC_IMG_BMP));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("pcx",MFC_IMG_PCX));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("tif",MFC_IMG_TIF));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("tiff",MFC_IMG_TIF));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("tga",MFC_IMG_TGA));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("ico",MFC_IMG_ICO));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("psd",MFC_IMG_PSD));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("xpm",MFC_IMG_XPM));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("oxo",MFC_IMG_PHOXO));
		ImgTypeMap.insert(std::map<std::string,MFC_IMAGE_TYPE>::value_type("dds",MFC_IMG_DDS));
	}
	std::map<std::string,MFC_IMAGE_TYPE>::iterator it = ImgTypeMap.find(str);
	return (it==ImgTypeMap.end())?MFC_IMG_UNKNOW:it->second;
}
//-----------------------------------------------------------------------------
inline int CMFCArchiveHelper::__PS_WORD2INT (const BYTE* pWord)
{
    BYTE bySwap[2] = {pWord[1], pWord[0]} ;
    return *(short*)bySwap ;
}

//-----------------------------------------------------------------------------
inline bool CMFCArchiveHelper::SavePhotoshopACF(const char* szFileName,const std::deque<int> & listElem)
{
    if (listElem.size() != 27) return false;

    // create file
    //std::ofstream outFile(szFileName,std::ios::out|std::ios::binary|std::ios::trunc) ;
	std::ofstream outFile(fopen(szFileName, "wb"));
    if (!outFile.is_open())
        return false ;

    BYTE    pElem[2] ;
    for (int i=0 ; i < (int)listElem.size() ; i++)
    {
        *(short*)pElem = (short)listElem[i] ;
        MFCSwap (pElem[0], pElem[1]) ;
        outFile.write ((char*)pElem, 2) ;
    }
	outFile.close();
    return true ;
}
//-----------------------------------------------------------------------------
inline void CMFCArchiveHelper::LoadPhoXoSettingFile(const char* szFileName,PHOXOSETTING& oXoSetting)
{
	std::ifstream inFile(fopen(szFileName, "rb"));
    if(inFile.is_open())
    {
        // get file length
        inFile.seekg (0, std::ios::end) ;
        if((int)inFile.tellg() == sizeof(PHOXOSETTING))
        {
            inFile.seekg (0, std::ios::beg) ;
            inFile.read ((char*)&oXoSetting, sizeof(PHOXOSETTING)) ;
			inFile.close();
            return ;
        }
    }
	inFile.close();
    // 文件不存在或长度不符，默认设置
    oXoSetting.nUndoLevel = 20 ;	// Undo级数, default(20)
    oXoSetting.nBkType = 2 ;		// 画布背景类型，0(颜色)，1(检测板)，2(粉红棋盘)
    oXoSetting.nNewWidth = 640 ;	// 新建图像宽
    oXoSetting.nNewHeight = 480 ;	// 新建图像高
    oXoSetting.nResolution = 72 ;	// 新建图像解析度
    oXoSetting.crFore = MFC_RGBA(0,0,0,0xFF) ;			// 前景色
    oXoSetting.crBack = MFC_RGBA(0xFF,0xFF,0xFF,0xFF) ; // 背景色
    oXoSetting.nIsShowLogo = 1 ;	// 默认显示logo
    oXoSetting.nAntStepTime = 300 ; // 蚂蚁线间隔时间毫秒数
    oXoSetting.nIdLanguage = 0 ;
}
//-----------------------------------------------------------------------------
inline void CMFCArchiveHelper::SavePhoXoSettingFile(const char* szFileName,const PHOXOSETTING& oXoSetting)
{
    // create file.
    //std::ofstream outFile(szFileName,std::ios::out|std::ios::binary|std::ios::trunc) ;
	
	std::ofstream outFile(fopen(szFileName, "wb"));
    if(outFile.is_open())
        outFile.write((char*)&oXoSetting, sizeof(oXoSetting)) ;
	outFile.close();
}
//-----------------------------------------------------------------------------
inline void CMFCArchiveHelper::BubbleSort(int pArray[],int iNumElement)
{
    if (!pArray)
		return;

    for(int i = iNumElement-1 ; i > 0 ; --i)
    {
        bool bFlag = true ;
        for(int j = 0 ; j < i ; j++)
		{
            if(pArray[j] > pArray[j + 1])
            {
                MFCSwap(pArray[j],pArray[j+1]) ;
                bFlag = false ;
            }
		}
        if(bFlag)
            break ;
    }
}

#endif

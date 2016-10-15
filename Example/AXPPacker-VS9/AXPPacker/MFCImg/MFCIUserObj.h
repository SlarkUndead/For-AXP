#ifndef _MFC_IUSEROBJ_06_07_09_H_
#define _MFC_IUSEROBJ_06_07_09_H_

enum MFC_IMAGE_TYPE;

#define IMG_TYPE MFC_IMAGE_TYPE

class CMFCImageObj;

//=============================================================================
// 外部接口,只需使用此类就可以了
class CMFCIUserObj
{
private:
	std::string		mImgFile;	// 文件名(包含全路径)
	CMFCImageObj *	mImgObj;	// 具体负责图象的Read/Write/Handle
public:
	CMFCIUserObj();
	~CMFCIUserObj();

	bool CreateUserImgObj(int nWidth, int nHeight, int nColorBit);			// Create image.
	void SetPixelData(int x, int y, DWORD dwPixel) ;						// Set pixel data at (x,y).
	DWORD GetPixelData(int x, int y) const ;// Get pixel data at (x,y).

	bool LoadUserImgMemory(BYTE* pStart,int iFileSize, IMG_TYPE imgType);	// 根据内存里的数据生成
	bool LoadUserImgFile(const std::string & file);		// 文件读取
	bool SaveUserImgFile(const std::string & file);		// 图象存储
	
	bool RenderUserImage(HDC hDC,CRect rect);			// 图象显示

	bool CloseUserImage();								// 关闭
	
	std::string GetUserImgFile(void){return mImgFile;}	// 图象文件
	CMFCImageObj * GetUserImgObj(void){return mImgObj;}	// 除删除操作任何操作皆可
	long GetUserImgColorBits(void);	// 一个象素的字节数:1,4,8,16,24,32.
	long GetUserImgWidth(void);		// 图象的宽度
	long GetUserImgHeight(void);	// 图象的高度
};
#endif
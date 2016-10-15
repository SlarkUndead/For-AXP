#ifndef _MFC_IUSEROBJ_06_07_09_H_
#define _MFC_IUSEROBJ_06_07_09_H_

enum MFC_IMAGE_TYPE;

#define IMG_TYPE MFC_IMAGE_TYPE

class CMFCImageObj;

//=============================================================================
// �ⲿ�ӿ�,ֻ��ʹ�ô���Ϳ�����
class CMFCIUserObj
{
private:
	std::string		mImgFile;	// �ļ���(����ȫ·��)
	CMFCImageObj *	mImgObj;	// ���帺��ͼ���Read/Write/Handle
public:
	CMFCIUserObj();
	~CMFCIUserObj();

	bool CreateUserImgObj(int nWidth, int nHeight, int nColorBit);			// Create image.
	void SetPixelData(int x, int y, DWORD dwPixel) ;						// Set pixel data at (x,y).
	DWORD GetPixelData(int x, int y) const ;// Get pixel data at (x,y).

	bool LoadUserImgMemory(BYTE* pStart,int iFileSize, IMG_TYPE imgType);	// �����ڴ������������
	bool LoadUserImgFile(const std::string & file);		// �ļ���ȡ
	bool SaveUserImgFile(const std::string & file);		// ͼ��洢
	
	bool RenderUserImage(HDC hDC,CRect rect);			// ͼ����ʾ

	bool CloseUserImage();								// �ر�
	
	std::string GetUserImgFile(void){return mImgFile;}	// ͼ���ļ�
	CMFCImageObj * GetUserImgObj(void){return mImgObj;}	// ��ɾ�������κβ����Կ�
	long GetUserImgColorBits(void);	// һ�����ص��ֽ���:1,4,8,16,24,32.
	long GetUserImgWidth(void);		// ͼ��Ŀ��
	long GetUserImgHeight(void);	// ͼ��ĸ߶�
};
#endif
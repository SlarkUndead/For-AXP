
#include "stdafx.h"
#include "MFCIUserObj.h"
#include "MFCImageObj.h"
#include "MFCWin32Helper.h"

//-----------------------------------------------------------------------
CMFCIUserObj::CMFCIUserObj():mImgObj(0)
{
	mImgObj = new CMFCImageObj();
}
//-----------------------------------------------------------------------
CMFCIUserObj::~CMFCIUserObj()
{
	delete mImgObj;
	mImgObj = 0;
}
//-----------------------------------------------------------------------
// Create image.
bool CMFCIUserObj::CreateUserImgObj(int nWidth, int nHeight, int nColorBit)
{
	return mImgObj?mImgObj->Create(nWidth,nHeight,nColorBit):false;
}
//-----------------------------------------------------------------------
// Set pixel data at (x,y).
void CMFCIUserObj::SetPixelData(int x, int y, DWORD dwPixel)
{
	mImgObj?mImgObj->SetPixelData(x,y,dwPixel):0;
}
//-----------------------------------------------------------------------
// Get pixel data at (x,y).
 DWORD CMFCIUserObj::GetPixelData(int x, int y) const
 {
	 return mImgObj?mImgObj->GetPixelData(x,y):0;
 }
//-----------------------------------------------------------------------
// 根据内存里的数据生成
bool CMFCIUserObj::LoadUserImgMemory(BYTE* pStart,int iFileSize,IMG_TYPE imgType)
{
	if((!pStart)||(iFileSize<=0))
		return false;

	return mImgObj->Load(pStart,iFileSize,imgType);
}
//-----------------------------------------------------------------------
// 文件读取,NOTE: mImgObj != NULL !!!
bool CMFCIUserObj::LoadUserImgFile(const std::string & file)
{
	if(file.empty()||(!mImgObj->Load(file.c_str())))
	{
		mImgObj->Destroy();
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------
// 图象存储,NOTE: mImgObj != NULL !!!
bool CMFCIUserObj::SaveUserImgFile(const std::string & file)
{
	if(file.empty())
		return false;
	return mImgObj->Save(file.c_str());
}

//-----------------------------------------------------------------------
// 关闭
bool CMFCIUserObj::CloseUserImage()
{
	if(mImgObj)	return mImgObj->Destroy();
	return false;
}

//-----------------------------------------------------------------------
// 图象显示
bool CMFCIUserObj::RenderUserImage(HDC hDC,CRect rect)
{
	return CMFCWin32Helper::DrawImage(*mImgObj,hDC,rect);
}
//-----------------------------------------------------------------------
// 一个象素的字节数:1,4,8,16,24,32.
long CMFCIUserObj::GetUserImgColorBits(void)
{
	return (mImgObj?mImgObj->GetImgColorBits():0);
}
//-----------------------------------------------------------------------
// 图象的宽度
long CMFCIUserObj::GetUserImgWidth(void)
{
	return (mImgObj?mImgObj->GetImgWidth():0);
}
//-----------------------------------------------------------------------
// 图象的高度
long CMFCIUserObj::GetUserImgHeight(void)
{
	return (mImgObj?mImgObj->GetImgHeight():0);
}
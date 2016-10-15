


#ifndef _AXP_ARCHIVE_110810_H_
#define _AXP_ARCHIVE_110810_H_

#include "AxpFile.h"

//#define	USE_MFC			1	// 1: CString | 0: std::string

//#if(USE_MFC)
//	typedef CString					String;
//#else
//	typedef std::string				String;
//#endif





typedef std::vector<AxpItem>		AxpIVct;
typedef std::vector<AxpFile*>		AxpFVct;
typedef std::map<String, AxpFile>	AxpFMap;

class CAxpArchive
{
	DWORD		mSize;		// 文件大小
	FILE*		mhFile;		// 文件指针
	String		mFile;		// 记录下来方便写Log

	void unload();
	void LoadFileList(const AxpItem* pItms, int nCount);


	AxpFileHead		mAxpHd;
	
	AxpFMap			mAxpFMap;	// 查找用的
	AxpFVct			mFiles;		// 
	int				mUnAddr;
	int				mFileNum;	// 实际打入包里的文件数量

public:
	CAxpArchive(): mhFile(0), mSize(0), mUnAddr(0), mFileNum(0){}
	~CAxpArchive(){unload();}

	bool load(const char* file_nm);
	void clear_axp_archive();

	AxpFileHead* GetAxpFileHead(){return &mAxpHd;}
	const AxpFVct& GetAxpFileList()const{return mFiles;}

	int GetFileCount()const{return mFileNum;}

	String GetArchiveFile()const{return mFile;}

	void* GetData(const String& fileNm);
	void* GetData(int nAddr, int nSize);	// 从读取指定位置开始读给定大小数据
	void CloseData(void*&);					// 关闭数据区

	bool SaveToFile(const AxpFile*, const String& file);	// 存到文件里
};



#endif
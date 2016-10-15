


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
	DWORD		mSize;		// �ļ���С
	FILE*		mhFile;		// �ļ�ָ��
	String		mFile;		// ��¼��������дLog

	void unload();
	void LoadFileList(const AxpItem* pItms, int nCount);


	AxpFileHead		mAxpHd;
	
	AxpFMap			mAxpFMap;	// �����õ�
	AxpFVct			mFiles;		// 
	int				mUnAddr;
	int				mFileNum;	// ʵ�ʴ��������ļ�����

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
	void* GetData(int nAddr, int nSize);	// �Ӷ�ȡָ��λ�ÿ�ʼ��������С����
	void CloseData(void*&);					// �ر�������

	bool SaveToFile(const AxpFile*, const String& file);	// �浽�ļ���
};



#endif
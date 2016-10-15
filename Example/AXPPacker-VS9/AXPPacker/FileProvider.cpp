
#include "stdafx.h"
#include "FileProvider.h"


//-----------------------------------------------------------------------------------------------------------------------------------------------
//
//
//------------------------------------------------------------------------------------------------------------
// ���� C �Ļ����ļ�������������Դ
CFileProvider::~CFileProvider()
{
	CloseFile();
}

//------------------------------------------------------------------------------------------------------------
void CFileProvider::CloseFile()
{
	if(Is_Open())
	{
		fclose(mhFile);
		mhFile = 0;
		mSize  = 0;
	}
}

//------------------------------------------------------------------------------------------------------------
// ����Դ, ����������
bool CFileProvider::OpenResourceProvider(const char* pzfile)
{
	if(!pzfile) return false;
	CloseFile();
	mFile = pzfile;
	mhFile = fopen(pzfile, "rb");
	if(!mhFile)
	{
		UILogWriter(CLR_TXT, "CFileProvider::OpenResourceProvider() failed(1). file = %s\n", pzfile);
		return false;
	}
	fseek(mhFile, 0, SEEK_END);
	mSize = ftell(mhFile);
	if(mSize == 0)
	{
		CloseFile();
		UILogWriter(CLR_TXT, "CFileProvider::OpenResourceProvider() failed(2). file = %s\n", pzfile);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------------------------------------
// ��ȡ����
bool CFileProvider::GetDataBuffer(void*& pBf, size_t fsize, size_t pBfOffset, DWORD)
{
	if (!Is_Open())
	{
		UILogWriter(CLR_TXT, "CFileProvider::GetDataBuffer() failed(1). file = %s\n", mFile.c_str());
		return false;
	}
	if (mSize < (pBfOffset + fsize))
	{
		UILogWriter(CLR_TXT, "CFileProvider::GetDataBuffer() failed(2). file = %s, %u, %u, %u\n", mFile.c_str(), mSize, pBfOffset, fsize);
		return false;
	}
	pBf = new char[fsize];
	if(pBf)
	{
		if (fseek(mhFile, pBfOffset, SEEK_SET) == 0)
		{
			size_t len = fread(pBf, 1, fsize, mhFile);
			if (len == fsize)
				return true;
		}
		delete[] pBf;
		pBf = 0;
	}
	UILogWriter(CLR_TXT, "excption : NEW failed. file = %s, ReqSize = %d, Offset = %d\n", mFile.c_str(), fsize, pBfOffset);
	return false;
}

//------------------------------------------------------------------------------------------------------------
// ��ȡ����, �ⲿ�����ڴ�ռ�
bool CFileProvider::GetDataBuffer(void*& pBf, size_t fsize, size_t pBfOffset)
{
	if (!Is_Open())
	{
		UILogWriter(CLR_TXT, "CFileProvider::GetDataBuffer() failed(1). file = %s\n", mFile.c_str());
		return false;
	}
	if (mSize < (pBfOffset + fsize))
	{
		UILogWriter(CLR_TXT, "CFileProvider::GetDataBuffer() failed(2). file = %s, %u, %u, %u\n", mFile.c_str(), mSize, pBfOffset, fsize);
		return false;
	}
	if(pBf)
	{
		if(fseek(mhFile, pBfOffset, SEEK_SET) == 0)
		{
			size_t len = fread(pBf, 1, fsize, mhFile);
			if(len == fsize)	return true;
		}
	}
	UILogWriter(CLR_TXT, "excption : NEW failed. file = %s, ReqSize = %d, Offset = %d\n", mFile.c_str(), fsize, pBfOffset);
	return false;
}



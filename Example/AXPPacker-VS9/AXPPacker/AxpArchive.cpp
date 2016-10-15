
#include "stdafx.h"
#include "AxpFile.h"
#include "AxpArchive.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------------------------------------------------------------
void split_string(const String& pzTxt, StrVct& pLst, const String& szfnd)
{
#if(USE_MFC)
	size_t fndLen     = szfnd.GetLength();
	const char* pzfn  = szfnd;				// 搜索串
	const char* pzNxt = pzTxt;				// 搜索的开始位置
	const char* pzEnd = pzNxt + (pzTxt.GetLength() -1);// 搜索的结束位置
#else
	size_t fndLen     = szfnd.length();
	const char* pzfn  = szfnd.c_str();				// 搜索串
	const char* pzNxt = pzTxt.c_str();				// 搜索的开始位置
	const char* pzEnd = pzNxt + (pzTxt.length() -1);// 搜索的结束位置
#endif
	const char* pzRlt = 0;							// 搜索结果的位置
	int nCount = 0;									// 搜索结果的字节数量
	while(pzNxt)
	{
		pzRlt = strstr(pzNxt, pzfn);
		if(pzRlt)
		{
			nCount = (int)(pzRlt - pzNxt);
			pLst.push_back(String(pzNxt, nCount));
			pzNxt = pzRlt + fndLen;					// 下次搜索的开始位置
			if(pzNxt < pzEnd) continue;
		}
		pLst.push_back(pzNxt);
		pzNxt = 0;
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------
void split_string(const String& pzTxt, StrLst& pLst, const String& szfnd)
{
#if(USE_MFC)
	size_t fndLen     = szfnd.GetLength();
	const char* pzfn  = szfnd;				// 搜索串
	const char* pzNxt = pzTxt;				// 搜索的开始位置
	const char* pzEnd = pzNxt + (pzTxt.GetLength() -1);// 搜索的结束位置
#else
	size_t fndLen     = szfnd.length();
	const char* pzfn  = szfnd.c_str();				// 搜索串
	const char* pzNxt = pzTxt.c_str();				// 搜索的开始位置
	const char* pzEnd = pzNxt + (pzTxt.length() -1);// 搜索的结束位置
#endif
	const char* pzRlt = 0;							// 搜索结果的位置
	int nCount = 0;									// 搜索结果的字节数量
	while(pzNxt)
	{
		pzRlt = strstr(pzNxt, pzfn);
		if(pzRlt)
		{
			nCount = (int)(pzRlt - pzNxt);
			pLst.push_back(String(pzNxt, nCount));
			pzNxt = pzRlt + fndLen;					// 下次搜索的开始位置
			if(pzNxt < pzEnd) continue;
		}
		pLst.push_back(pzNxt);
		pzNxt = 0;
		break;
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------
void CAxpArchive::clear_axp_archive()
{
	unload();
	mFiles.clear();
	mAxpFMap.clear();
}


//--------------------------------------------------------------------------------------------------------------------------------------
bool CAxpArchive::load(const char* file_nm)
{
	clear_axp_archive();
	mFile = file_nm;
	mhFile = fopen(file_nm, "rb");
	if(!mhFile) return false;

	NtfUpdatePubInfo(_T("正在解析文件: ") + mFile);
	memset(&mAxpHd, 0, sizeof(mAxpHd));
	fread((char*)(&mAxpHd), sizeof(mAxpHd), 1, mhFile);

	AxpIVct itemLst;		// AxpFile 的索引表
	DWORD dwSize = mAxpHd.FileCount * sizeof(AxpItem);
	void* pBf = GetData(mAxpHd.TableOffset, dwSize);
	LoadFileList((const AxpItem*)pBf, mAxpHd.FileCount);
	CloseData(pBf);
	NtfDefaultPubInfo();
	return true;
}

//--------------------------------------------------------------------------------------------------------------------------------------
void CAxpArchive::unload()
{
	if(mhFile)
	{
		fclose(mhFile);
		mhFile = 0;
	}
}

void NtfSetProgPos(int& nCur, int& nPre, float fRate)
{
	++nCur;
	//if((nCur - nPre) >= 10)
	{
		nPre = nCur;
		float fpos = fRate * (float)nCur;
		NtfSetProgress((int)fpos);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------
void CAxpArchive::LoadFileList(const AxpItem* pItms, int nCount)
{
	mFiles.clear();
	if(pItms && (nCount > 0))
	{
		int nHdl = 0, nPre = 0;
		NtfStartProgress();
		float fRate = 0.5f/(float)(nCount) * 100.0f;
		const AxpItem& itm = pItms[nCount - 1];
		if(itm.dwSize > 0)
		{
			char *fileData = new char[itm.dwSize + 1];
			fseek(mhFile, itm.dwAddr, 0);
			fread(fileData, sizeof(char), itm.dwSize, mhFile);
			fileData[itm.dwSize] = '\0';
			if('\n' == fileData[itm.dwSize - 1])
			fileData[itm.dwSize - 1] = '\0';

			StrVct pLst0;
			split_string(fileData, pLst0, "\n");
#if(USE_MFC)
			mUnAddr = atoi(pLst0[0]);
			mFileNum= atoi(pLst0[1]);
#else
			mUnAddr = atoi(pLst0[0].c_str());
			mFileNum= atoi(pLst0[1].c_str());
#endif

			AxpFile axpf;
			axpf.dwAddr = 0;
			axpf.dwSize = 0;
			StrVct pLst;
			
			for(size_t i = 2, npre = 0; i < pLst0.size(); ++i)// mFileNum 个有效
			{
				if(IsThreadOver()) break;
				split_string(pLst0[i], pLst, "|");

				axpf.szFile = pLst[0];
				axpf.szSize = pLst[1];
				axpf.szAddr = pLst[2];
#if(USE_MFC)
				axpf.szFile.Replace(_T('/'), _T('\\'));
#else
				std::replace( axpf.szFile.begin(), axpf.szFile.end(), '/', '\\' );
#endif
				mAxpFMap.insert(AxpFMap::value_type(axpf.szFile, axpf));
				mFiles.push_back(&(mAxpFMap[axpf.szFile]));
				pLst.clear();

				NtfSetProgPos(nHdl, nPre, fRate);
			}

			delete [] fileData;
		}

		int nNum = (int)mFiles.size();
		for(int i = 0; (i < nCount - 1) && (i < nNum); ++i)
		{
			if(IsThreadOver()) return;
			const AxpItem& itm = pItms[i];
			AxpFile* fl = mFiles[i];
			fl->dwSize = itm.dwSize;
			fl->dwAddr = itm.dwAddr;

			NtfSetProgPos(nHdl, nPre, fRate);
		}
		NtfEndProgress();
	}
}


//--------------------------------------------------------------------------------------------------------------------------------------
// 从读取指定位置开始读给定大小数据
void* CAxpArchive::GetData(const String& fileNm)
{
	AxpFMap::const_iterator it = mAxpFMap.find(fileNm);
	if(it == mAxpFMap.end()) return 0;
	const AxpFile& itm = it->second;
	return GetData(itm.dwAddr, itm.dwSize);
}

//----------------------------------------------------------------------------------
void* CAxpArchive::GetData(int nAddr, int nSize)
{
	if(0 == fseek(mhFile, nAddr, 0))
	{
		char* ptr = new char[nSize + 1];
		if(fread(ptr, sizeof(char), nSize, mhFile) == nSize)
		{
			ptr[nSize] = '\0';
			return ptr;
		}
		void* p = (void*)ptr;
		CloseData(p);
	}
	return 0;
}

//----------------------------------------------------------------------------------
// 关闭数据区
void CAxpArchive::CloseData(void*& ptr)
{
	delete [] ptr;
	ptr = 0;
}

//----------------------------------------------------------------------------------
// 存到文件里
bool CAxpArchive::SaveToFile(const AxpFile* ptr, const String& file)
{
	if(!ptr) return false;

	if(0 != fseek(mhFile, ptr->dwAddr, 0)) return false;

	FILE* hFile = fopen(file, "wb");
	if(!hFile) return false;

	const int dwBSize = 64 * 1024;
	char pBf[dwBSize];
	//memset(pBf, 0, dwBSize);

	int dwRead = 0, dwALen = ptr->dwSize;
	while(dwALen > 0)
	{
		if(!IsThreadOver())
		{
			if(dwALen > dwBSize) dwRead = dwBSize;
			else dwRead = dwALen;
			if(1 == fread(pBf, dwRead, 1, mhFile))
			{
				fwrite(pBf, dwRead, 1, hFile);
				dwALen -= dwRead;
				continue;
			}
		}
		break;
	}
	fclose(hFile);
	return true;
}

#include "stdafx.h"
#include "MFCArchiveHelper.h"
#include "MFCArray.h"
//-----------------------------------------------------------------------------
bool CMFCArchiveHelper::LoadPhotoshopACF(const char* szFileName,std::deque<int>& listElem)
{
	listElem.clear() ;
	std::ifstream inFile(fopen(szFileName, "rb"));
	if (!inFile.is_open())
		return false ;

	// get file's size
	inFile.seekg (0, std::ios::end) ;
	const int nFileSize = inFile.tellg() ;
	if(nFileSize != 54) // 27.elem * 2.bytes
		return false ;

	// read file into memory.
	CMFCArray<short> pStart(new BYTE[nFileSize]) ;
	inFile.seekg (0, std::ios::beg) ;
	inFile.read ((char*)pStart.GetArrayPtr(), nFileSize) ;

	// 5 x 5 filter & scale & offset
	for(int i=0 ; i < 25 ; i++)
	{
		listElem.push_back (__PS_WORD2INT((BYTE*)&pStart[i])) ;
	}
	listElem.push_back (__PS_WORD2INT((BYTE*)&pStart[25])) ;
	listElem.push_back (__PS_WORD2INT((BYTE*)&pStart[26])) ;
	inFile.close();
	return true ;
}

#include "stdafx.h"
#include "MFCIImageHandle.h"
#include "../MFCArray.h"

//=============================================================================
bool CMFCIImageHandle::LoadImageFile (const char* szFileName)
{
	std::ifstream inFile(fopen(szFileName, "rb"));
	if (!inFile.is_open())
		return false ;

	// get file length
	inFile.seekg (0, std::ios::end) ;
	const int nFileSize = inFile.tellg() ;
	if (nFileSize <= 0)
		return false ;

	// read file into memory
	CMFCArray<BYTE> pStart(nFileSize) ;
	inFile.seekg (0, std::ios::beg) ;
	inFile.read ((char*)pStart.GetArrayPtr(), nFileSize) ;
	inFile.close();
	return LoadImageMemory(pStart.GetArrayPtr(), nFileSize) ;
}
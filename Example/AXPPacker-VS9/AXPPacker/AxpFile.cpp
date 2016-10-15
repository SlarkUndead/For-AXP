
#include "stdafx.h"
#include "AxpArchive.h"
#include "FileProvider.h"



bool LoadAxpFile(String file)
{
	CAxpArchive axpArch;
	axpArch.load(file);
	return true;
}
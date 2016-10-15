
/*==========================================================================
*
*  Copyright (C) 2011.08 --- .  All Rights Reserved.
*
*  Author : leo.li
*  Time   : 2011-08-09
*  File   : AxpFile.h
*  Content: include file.Part of TablePackTool Project.
*			AXP文件格式
*			
*			
*			
*			
*			
*  NOTE:	
*		    1、.
*		    2、
*
****************************************************************************/

#ifndef _GAME_AXP_HEADER_110809_H_
#define _GAME_AXP_HEADER_110809_H_

//#define	USE_MFC			1	// 1: CString | 0: std::string

//#if(USE_MFC)
//	typedef CString					String;
//#else
//	typedef std::string				String;
//#endif


#define FILE_FLAG      "AXPK"

typedef unsigned char uchar;


//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//
// Axp资源包文件格式:
//  ________________________
// |                        |
// |  文件头(AxpFileHead)   |     <------ 40 字节
// |                        |
// |------------------------|
// |                        |
// |                        |
// |       数据区           |
// |                        |
// |                        |
// |------------------------|
// |                        |
// |    索引表(AxpItem)     |
// |________________________|
// |                        |
// |  索引表(AxpFileItem)   |
// |________________________|
//
//-------------------------------------------------------------------------------
struct AxpFileHead 
{
	char	Flags[4];		// 文件类型
	//char	Unknown0[11];	// 
	
	UINT	dwUnknown0;	// 
	UINT	dwUnknown1;	// 
	UINT	dwUnknown2;	// 

	UINT	TableOffset;	// 索引表的位置, data begin address
	UINT	FileCount;		// 文件索引项数量, AxpFileItem 的数量, files count
	//char	Unknown1[28];


	UINT	dwUnknown5;
	UINT	dwUnknown6;
	UINT	dwUnknown7;
	UINT	dwUnknown8;
	char	Unknown1[12];	// 
};

//-------------------------------------------------------------------------------
// 偏移量/大小/未知 等项索引表
struct AxpItem
{
	UINT	dwAddr;		// 文件内容的位置
	UINT	dwSize;		// 文件原始大小
	UINT	dwFlags;	// 标志位
};


//-------------------------------------------------------------------------------
// 偏移量/大小/未知 文件名索引表
struct AxpFile
{
	String	szFile;		// 文件名
	String	szAddr;		// 位置(16进制)
	String	szSize;		// 文件原始大小字符串形式(16进制)
	UINT	dwAddr;		// 文件内容的位置
	UINT	dwSize;		// 文件原始大小
};


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



bool LoadAxpFile(String file);


#endif
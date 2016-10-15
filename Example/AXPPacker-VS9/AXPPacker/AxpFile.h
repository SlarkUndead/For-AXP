
/*==========================================================================
*
*  Copyright (C) 2011.08 --- .  All Rights Reserved.
*
*  Author : leo.li
*  Time   : 2011-08-09
*  File   : AxpFile.h
*  Content: include file.Part of TablePackTool Project.
*			AXP�ļ���ʽ
*			
*			
*			
*			
*			
*  NOTE:	
*		    1��.
*		    2��
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
// Axp��Դ���ļ���ʽ:
//  ________________________
// |                        |
// |  �ļ�ͷ(AxpFileHead)   |     <------ 40 �ֽ�
// |                        |
// |------------------------|
// |                        |
// |                        |
// |       ������           |
// |                        |
// |                        |
// |------------------------|
// |                        |
// |    ������(AxpItem)     |
// |________________________|
// |                        |
// |  ������(AxpFileItem)   |
// |________________________|
//
//-------------------------------------------------------------------------------
struct AxpFileHead 
{
	char	Flags[4];		// �ļ�����
	//char	Unknown0[11];	// 
	
	UINT	dwUnknown0;	// 
	UINT	dwUnknown1;	// 
	UINT	dwUnknown2;	// 

	UINT	TableOffset;	// �������λ��, data begin address
	UINT	FileCount;		// �ļ�����������, AxpFileItem ������, files count
	//char	Unknown1[28];


	UINT	dwUnknown5;
	UINT	dwUnknown6;
	UINT	dwUnknown7;
	UINT	dwUnknown8;
	char	Unknown1[12];	// 
};

//-------------------------------------------------------------------------------
// ƫ����/��С/δ֪ ����������
struct AxpItem
{
	UINT	dwAddr;		// �ļ����ݵ�λ��
	UINT	dwSize;		// �ļ�ԭʼ��С
	UINT	dwFlags;	// ��־λ
};


//-------------------------------------------------------------------------------
// ƫ����/��С/δ֪ �ļ���������
struct AxpFile
{
	String	szFile;		// �ļ���
	String	szAddr;		// λ��(16����)
	String	szSize;		// �ļ�ԭʼ��С�ַ�����ʽ(16����)
	UINT	dwAddr;		// �ļ����ݵ�λ��
	UINT	dwSize;		// �ļ�ԭʼ��С
};


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>



bool LoadAxpFile(String file);


#endif
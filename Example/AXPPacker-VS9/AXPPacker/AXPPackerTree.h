
#pragma once

#define DEF_GRP		_T("Default")

struct AxpFile;
struct CTreeItm;
typedef std::vector<CTreeItm*>		TrItmVct;


struct CTreeItm
{
	CTreeItm*		mpParent;	// 所有者
	CString			mItmName;	// 名称, 以上面为例: asw
	TrItmVct		mSubLst;	// 子目录
	TrItmVct		mFileLst;	// 文件列表
	const AxpFile*	mAxpItem;	// 仅当是文件时有效

	void clear_tree_item();
	CString get_name()const;		// 若 mItmName == DEF_GRP 则返回 ""
	CString get_name_path()const;	// 由 mItmName 构成的路径

	CTreeItm* add_sub_item(CString szNm);						// 加子项
	CTreeItm* add_file_item(CString szNm, const AxpFile* pAxp);	
};
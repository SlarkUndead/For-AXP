
#pragma once

#define DEF_GRP		_T("Default")

struct AxpFile;
struct CTreeItm;
typedef std::vector<CTreeItm*>		TrItmVct;


struct CTreeItm
{
	CTreeItm*		mpParent;	// ������
	CString			mItmName;	// ����, ������Ϊ��: asw
	TrItmVct		mSubLst;	// ��Ŀ¼
	TrItmVct		mFileLst;	// �ļ��б�
	const AxpFile*	mAxpItem;	// �������ļ�ʱ��Ч

	void clear_tree_item();
	CString get_name()const;		// �� mItmName == DEF_GRP �򷵻� ""
	CString get_name_path()const;	// �� mItmName ���ɵ�·��

	CTreeItm* add_sub_item(CString szNm);						// ������
	CTreeItm* add_file_item(CString szNm, const AxpFile* pAxp);	
};
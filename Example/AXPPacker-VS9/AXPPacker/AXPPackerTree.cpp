
#include "stdafx.h"
#include "AXPPackerTree.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------
void CTreeItm::clear_tree_item()
{
	for(size_t i = 0; i < mFileLst.size(); ++i)
	{
		delete mFileLst[i];
	}
	mFileLst.clear();

	for(size_t j = 0; j < mSubLst.size(); ++j)
	{
		CTreeItm* ptr = mSubLst[j];
		ptr->clear_tree_item();
		delete ptr;
	}
	mSubLst.clear();
}

//----------------------------------------------------------------------------------------------------
CString CTreeItm::get_name()const
{
	return (mItmName == DEF_GRP) ? "" : mItmName;
}

//----------------------------------------------------------------------------------------------------
// �� mItmName ���ɵ�·��
CString CTreeItm::get_name_path()const
{
	CString szPth = get_name(), szTmp;
	CTreeItm* ptr = mpParent;
	while(ptr)
	{
		szTmp = ptr->get_name();
		if(!szTmp.IsEmpty())
		{
			szPth = szTmp + "\\" + szPth;
		}
		ptr = ptr->mpParent;
	}
	return szPth;
}

//----------------------------------------------------------------------------------------------------
// ������
CTreeItm* CTreeItm::add_sub_item(CString szNm)
{
	CTreeItm* ptr = 0;
	for(size_t i = 0; i < mSubLst.size(); ++i)
	{
		ptr = mSubLst[i];
		if(ptr->mItmName == szNm)
			return ptr;
	}
	ptr = new CTreeItm;
	ptr->mAxpItem = 0;
	ptr->mpParent = this;
	ptr->mItmName = szNm;
	mSubLst.push_back(ptr);
	return ptr;
}

//----------------------------------------------------------------------------------------------------
CTreeItm* CTreeItm::add_file_item(CString szNm, const AxpFile* pAxp)
{
	CTreeItm* ptr = new CTreeItm;
	ptr->mpParent = this;
	ptr->mItmName = szNm;
	ptr->mAxpItem = pAxp;
	mFileLst.push_back(ptr);
	return ptr;
}

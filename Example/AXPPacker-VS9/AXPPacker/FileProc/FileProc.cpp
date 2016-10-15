
#include "stdafx.h"
#include "FileProc.h"
#include "../AxpFile.h"
#include "../resource.h"		// ������
#include "../FMODAudio.h"
#include "../AxpArchive.h"
#include "../AXPPackerDlg.h"
#include "../MFCImg/MFCIUserObj.h"
#include "../MFCImg/MFCImageType.h"

#define	REG_PROC(ClassT)	RegFileProc(new ClassT)

typedef std::map<CString, CFileType>	ExtFType;	// ��չ����Ӧ���ļ�����
typedef std::map<CFileType, IFileHdlr*>	FileProc;

extern CString get_size_desc(UINT fsize);


ExtFType g_file_ext;
//---------------------------------------------------------------------------------------------------------------------------
// ע��ɴ�����ļ�����
void RegAllFileType()
{
	// ͼ��
	g_file_ext[_T(".dds")] 			= FILE_TYPE_IMAGE;
	g_file_ext[_T(".jpg")] 			= FILE_TYPE_IMAGE;
	g_file_ext[_T(".tga")] 			= FILE_TYPE_IMAGE;
	g_file_ext[_T(".bmp")] 			= FILE_TYPE_IMAGE;
	g_file_ext[_T(".png")] 			= FILE_TYPE_IMAGE;

	// ����
	g_file_ext[_T(".wav")] 			= FILE_TYPE_SOUND;
	g_file_ext[_T(".mp3")] 			= FILE_TYPE_SOUND;
	g_file_ext[_T(".ogg")] 			= FILE_TYPE_SOUND;

	g_file_ext[_T(".mesh")]			= FILE_TYPE_MESH;		// Mesh�ļ�
	g_file_ext[_T(".skeleton")]		= FILE_TYPE_SKELETON,	// skeleton
	g_file_ext[_T(".enscene")]		= FILE_TYPE_ENSCENE;		// ��������
	g_file_ext[_T(".sky")]			= FILE_TYPE_SKY;			// ��պ�����
	g_file_ext[_T(".material")]		= FILE_TYPE_MATERIAL;	// ��������
	g_file_ext[_T(".visual")]		= FILE_TYPE_VISUAL;		// 
	g_file_ext[_T(".asm")]			= FILE_TYPE_ASM;			// 
	g_file_ext[_T(".barticle")]		= FILE_TYPE_BARTICLE;	// 
	g_file_ext[_T(".phyff")]		= FILE_TYPE_PHYFF;		// Physics
	g_file_ext[_T(".effect")]		= FILE_TYPE_EFFECT;		// effect
	g_file_ext[_T(".skill")]		= FILE_TYPE_SKILL;		// skill
	g_file_ext[_T(".scene")]		= FILE_TYPE_SCENE;		// Scene
	g_file_ext[_T(".terrain")]		= FILE_TYPE_TERRAIN;	// Terrain
	g_file_ext[_T(".preload")]		= FILE_TYPE_PRELOAD;	// preload
	g_file_ext[_T(".pla")]			= FILE_TYPE_PLA;		// pla
	g_file_ext[_T(".cg")]			= FILE_TYPE_CG;			// cg
	g_file_ext[_T(".chunk")]		= FILE_TYPE_CHUNK;		// chunk
	g_file_ext[_T(".CLL")]			= FILE_TYPE_CLL;		// CLL
	g_file_ext[_T(".esd")]			= FILE_TYPE_ESD;		// esd(��)
	g_file_ext[_T(".gridinfo")]		= FILE_TYPE_GRIDINFO;	// GridInfo(����������Ϣ)
	g_file_ext[_T(".heightmap")]	= FILE_TYPE_HEIGHTMAP;	// Heightmap
	g_file_ext[_T(".path")]			= FILE_TYPE_PATH;		// path
	g_file_ext[_T(".program")]		= FILE_TYPE_PROGRAM;	// program(shader)
	g_file_ext[_T(".region")]		= FILE_TYPE_REGION;		// Region(��������)
	g_file_ext[_T(".sregion")]		= FILE_TYPE_SREGION;	// SRegion(����)
	g_file_ext[_T(".track")]		= FILE_TYPE_TRACK;		// track
	g_file_ext[_T(".unr")]			= FILE_TYPE_UNR;		// Unreach Region
	g_file_ext[_T(".wcollision")]	= FILE_TYPE_WCOLLISION;	// WCollision
	g_file_ext[_T(".xml")]			= FILE_TYPE_XML;		// xml
	g_file_ext[_T(".xsd")]			= FILE_TYPE_XSD;		// xsd(����xml)

	
}

void RegAllFileProcs();
void ClearAllFileProcs();



void NtfInitFileHanderModel()
{
	RegAllFileType();		// ע��ɴ�����ļ�����
	RegAllFileProcs();
}

void NtfCloseFileHanderModel()
{
	ClearAllFileProcs();
}


//---------------------------------------------------------------------------------------------------------------------------
// ƥ���Ӧ�ļ�����
CFileType get_file_type(CString fileNm)
{
	char pExt[_MAX_EXT]  = "\0";
	_splitpath_s(fileNm, 0, 0, 0, 0,0, 0, pExt, sizeof(pExt));
	CString szExt = pExt;
	szExt.MakeLower();
	ExtFType::const_iterator it = g_file_ext.find(szExt);
	if(it != g_file_ext.end()) return it->second;
	return FILE_TYPE_ERROR;

	//int nfnd = fileNm.Find(_T("."));
	//if(nfnd > 0)
	//{
	//	CString szExt = fileNm.Right(fileNm.GetLength() - nfnd);
	//	szExt.MakeLower();
	//	ExtFType::const_iterator it = g_file_ext.find(szExt);
	//	if(it != g_file_ext.end()) return it->second;
	//}
	//return FILE_TYPE_ERROR;
}


//-----------------------------------------------------------------------------------------------------------
// ȷ��ʵ��ͼ���ļ���ʽ
MFC_IMAGE_TYPE fnd_img_type(CString fileNm)
{
	char pExt[_MAX_EXT]  = "\0";
	_splitpath_s(fileNm, 0, 0, 0, 0,0, 0, pExt, sizeof(pExt));
	CString szExt = pExt;
	szExt.MakeLower();
	
	if(szExt == _T(".bmp")){return MFC_IMG_BMP;}
	if(szExt == _T(".pcx")){return MFC_IMG_PCX;}
	if(szExt == _T(".jpg")){return MFC_IMG_JPG;}
	if(szExt == _T(".gif")){return MFC_IMG_GIF;}
	if(szExt == _T(".tga")){return MFC_IMG_TGA;}
	if(szExt == _T(".tif")){return MFC_IMG_TIF;}
	if(szExt == _T(".png")){return MFC_IMG_PNG;}
	if(szExt == _T(".psd")){return MFC_IMG_PSD;}
	if(szExt == _T(".ico")){return MFC_IMG_ICO;}
	if(szExt == _T(".xpm")){return MFC_IMG_XPM;}
	if(szExt == _T(".dds")){return MFC_IMG_DDS;}

	return MFC_IMG_UNKNOW;

	//int nfnd = fileNm.Find(_T("."));
	//if(nfnd > 0)
	//{
	//	CString szExt = fileNm.Right(fileNm.GetLength() - nfnd);
	//	szExt.MakeLower();
	//	if(szExt == _T(".bmp")){return MFC_IMG_BMP;}
	//	if(szExt == _T(".pcx")){return MFC_IMG_PCX;}
	//	if(szExt == _T(".jpg")){return MFC_IMG_JPG;}
	//	if(szExt == _T(".gif")){return MFC_IMG_GIF;}
	//	if(szExt == _T(".tga")){return MFC_IMG_TGA;}
	//	if(szExt == _T(".tif")){return MFC_IMG_TIF;}
	//	if(szExt == _T(".png")){return MFC_IMG_PNG;}
	//	if(szExt == _T(".psd")){return MFC_IMG_PSD;}
	//	if(szExt == _T(".ico")){return MFC_IMG_ICO;}
	//	if(szExt == _T(".xpm")){return MFC_IMG_XPM;}
	//	if(szExt == _T(".dds")){return MFC_IMG_DDS;}
	//}
	//return MFC_IMG_UNKNOW;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//
// �ļ�������
//
//-------------------------------------------------------------------------------------------------------------------------------------------------
CString IFileHdlr::default_tips(CTreeItm *ptr)
{
	if(ptr)
	{
		const AxpFile* pAxp = ptr->mAxpItem;
		if(pAxp)
		{
			CString szInf;
			CString szDesc = get_file_desc();
			CString szfs = get_size_desc(pAxp->dwSize);
			szInf.Format(_T("Code: %s\n��С: %s\n����: %s\n·��: %s"), pAxp->szAddr, szfs, szDesc, pAxp->szFile);
			return szInf;
		}
	}
	return _T("�������");
}

#define ErrorReport(ptr)				\
		if(!ptr || (!ptr->mAxpItem))	\
		{								\
			mszTips = _T("�������");	\
			return false;				\
		}


//-------------------------------------------------------------------------------------------------------------------------------------------------
// �����ļ�: WAV/MP3/...
struct CSndProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_SOUND;}
	CString get_file_desc()const{return _T("�����ļ�");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);

		const AxpFile* pAxp = ptr->mAxpItem;
		CAxpArchive* pArch = pDlg->GetAxpArchive();
		CMFCIUserObj* pImg = pDlg->GetImgCtrl();

		void* pData = pArch->GetData(pAxp->dwAddr, pAxp->dwSize);
		if(!pData)
		{
			mszTips = _T("�޷���ȡ����: " + pAxp->szFile);
			return false;
		}
		CString szDesc = get_file_desc();
		CString szfs = get_size_desc(pAxp->dwSize);
		float fTm = NtfPlay2DSoundEffect(pAxp->szFile, (const char*)pData, (size_t)pAxp->dwSize);
		mszTips.Format(_T("Code: %s\n����: %.2fs\n��С: %s\n����: %s\n·��: %s"), pAxp->szAddr, fTm, szfs, szDesc, pAxp->szFile);
		
		pArch->CloseData(pData);
		return true;
	}
};


//-------------------------------------------------------------------------------------------------------------------------------------------------
// ͼ���ļ�: JPG/DDS/TGA/..
struct CImgProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_IMAGE;}
	CString get_file_desc()const{return _T("�����ļ�");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);

		const AxpFile* pAxp = ptr->mAxpItem;
		CAxpArchive* pArch = pDlg->GetAxpArchive();
		CMFCIUserObj* pImg = pDlg->GetImgCtrl();

		void* pData = pArch->GetData(pAxp->dwAddr, pAxp->dwSize);
		if(!pData)
		{
			mszTips = _T("�޷���ȡ����: " + pAxp->szFile);
			return false;
		}

		extern MFC_IMAGE_TYPE fnd_img_type(CString fileNm);
		MFC_IMAGE_TYPE eImgT = fnd_img_type(pAxp->szFile);
		if(pImg->LoadUserImgMemory((BYTE*)pData, pAxp->dwSize, eImgT))
		{
			long w  = pImg->GetUserImgWidth();
			long h  = pImg->GetUserImgHeight();
			long bp = pImg->GetUserImgColorBits() >> 3;

			CString szDesc = get_file_desc();
			CString szfs = get_size_desc(pAxp->dwSize);
			mszTips.Format(_T("Code: %s\n��С: %s\n�ߴ�: %d x %d\nͨ��: %d\n����: %s\n·��: %s"), pAxp->szAddr, szfs, w, h, bp, szDesc, pAxp->szFile);
		}
		else mszTips = _T("�޷���ʾ����: ") + pAxp->szFile;
		pArch->CloseData(pData);
		pDlg->Invalidate();
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Mesh�ļ�
struct CMshProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_MESH;}
	CString get_file_desc()const{return _T("Mesh�ļ�");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// skeleton
struct CSkeletonProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_SKELETON;}
	CString get_file_desc()const{return _T("skeleton�ļ�");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// ��������
struct CEnScnProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_ENSCENE;}
	CString get_file_desc()const{return _T("��������");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// ��պ�
struct CSkyProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_SKY;}
	CString get_file_desc()const{return _T("��պ�");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// ��������
struct CMtrProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_MATERIAL;}
	CString get_file_desc()const{return _T("��������");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Visual
struct CVisualProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_VISUAL;}
	CString get_file_desc()const{return _T("????");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// ASM
struct CASMProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_ASM;}
	CString get_file_desc()const{return _T("ASM");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Barticle
struct CBarticleProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_BARTICLE;}
	CString get_file_desc()const{return _T("BArticle");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Physics
struct CPhysicsProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_PHYFF;}
	CString get_file_desc()const{return _T("Physics");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// effect
struct CEffectProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_EFFECT;}
	CString get_file_desc()const{return _T("Effect");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Skill
struct CSkillProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_SKILL;}
	CString get_file_desc()const{return _T("Skill");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Scene
struct CSceneProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_SCENE;}
	CString get_file_desc()const{return _T("Scene");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Terrain
struct CTerrainProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_TERRAIN;}
	CString get_file_desc()const{return _T("Terrain");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Preload
struct CPreloadProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_PRELOAD;}
	CString get_file_desc()const{return _T("Preload");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// pla
struct CPLAProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_PLA;}
	CString get_file_desc()const{return _T("????");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// cg(shader)
struct CCGProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_CG;}
	CString get_file_desc()const{return _T("cg(shader)");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// chunk
struct CChunkProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_CHUNK;}
	CString get_file_desc()const{return _T("chunk");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// CLL
struct CCLLProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_CLL;}
	CString get_file_desc()const{return _T("????");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// esd(��)
struct CESDProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_ESD;}
	CString get_file_desc()const{return _T("esd(��)");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// GridInfo(����������Ϣ)
struct CTGrdInfProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_GRIDINFO;}
	CString get_file_desc()const{return _T("GridInfo(����������Ϣ)");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Heightmap
struct CHMapProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_HEIGHTMAP;}
	CString get_file_desc()const{return _T("�߶�ͼ");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// path
struct CPathProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_PATH;}
	CString get_file_desc()const{return _T("Path");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// program(shader)
struct CProgramProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_PROGRAM;}
	CString get_file_desc()const{return _T("program(shader)");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Region(��������)
struct CRegionProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_REGION;}
	CString get_file_desc()const{return _T("Region(��������)");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// SRegion(����)
struct CSRegionProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_SREGION;}
	CString get_file_desc()const{return _T("SRegion(����)");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// track
struct CTrackProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_TRACK;}
	CString get_file_desc()const{return _T("track");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Unreach Region
struct CUNRProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_UNR;}
	CString get_file_desc()const{return _T("Unreach Region");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// WCollision(��ײ)
struct CWCollProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_WCOLLISION;}
	CString get_file_desc()const{return _T("WCollision(��ײ)");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// XML
struct CXMLProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_XML;}
	CString get_file_desc()const{return _T("XML");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};

//-------------------------------------------------------------------------------------------------------------------------------------------------
// XSD
struct CXSDProc : public IFileHdlr
{
	CFileType get_type()const{return FILE_TYPE_XSD;}
	CString get_file_desc()const{return _T("XSD");}
	bool file_proc(CAXPPackerDlg* pDlg, CTreeItm* ptr)
	{
		ErrorReport(ptr);
		mszTips = default_tips(ptr);
		return true;
	}
};












//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FileProc g_file_proc;
void RegFileProc(IFileHdlr* proc)
{
	g_file_proc[proc->get_type()] = proc;
}

void ClearAllFileProcs()
{
	FileProc::iterator it = g_file_proc.begin();
	for(; it != g_file_proc.end(); ++it)
	{
		delete it->second;
	}
	g_file_proc.clear();
}

// ��: .txt
IFileHdlr* get_file_proc(CFileType eType)
{
	FileProc::iterator it = g_file_proc.find(eType);
	return (it == g_file_proc.end()) ? 0 : it->second;
}

IFileHdlr* get_file_proc(CString szExtNm)
{
	return get_file_proc(get_file_type(szExtNm));
}

void RegAllFileProcs()
{
	#include "FileProcReg.h"
}

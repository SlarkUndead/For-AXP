
#pragma once


enum CFileType
{
	FILE_TYPE_SOUND = 0,	// 声音文件: WAV/MP3/...
	FILE_TYPE_IMAGE,		// 图像文件: JPG/DDS/TGA/..
	FILE_TYPE_MESH,			// Mesh文件
	FILE_TYPE_SKELETON,		// skeleton
	FILE_TYPE_ENSCENE,		// 场景配置
	FILE_TYPE_SKY,			// 天空盒配置
	FILE_TYPE_MATERIAL,		// 材质配置
	FILE_TYPE_VISUAL,		// 
	FILE_TYPE_ASM,			
	FILE_TYPE_BARTICLE,		// 
	FILE_TYPE_PHYFF,		// Physics
	FILE_TYPE_EFFECT,		// effect
	FILE_TYPE_SKILL,		// skill
	FILE_TYPE_SCENE,		// Scene
	FILE_TYPE_TERRAIN,		// Terrain
	FILE_TYPE_PRELOAD,		// preload
	FILE_TYPE_PLA,			// pla

	FILE_TYPE_CG,			// cg(shader)
	FILE_TYPE_CHUNK,		// chunk
	FILE_TYPE_CLL,			// CLL
	FILE_TYPE_ESD,			// esd(表)
	FILE_TYPE_GRIDINFO,		// GridInfo(地形网格信息)
	FILE_TYPE_HEIGHTMAP,	// Heightmap
	FILE_TYPE_PATH,			// path
	FILE_TYPE_PROGRAM,		// program(shader)
	FILE_TYPE_REGION,		// Region(行走区域)
	FILE_TYPE_SREGION,		// SRegion(声音)
	FILE_TYPE_TRACK,		// track
	FILE_TYPE_UNR,			// Unreach Region
	FILE_TYPE_WCOLLISION,	// WCollision(碰撞)
	FILE_TYPE_XML,			// xml
	FILE_TYPE_XSD,			// xsd(检验xml)

	FILE_TYPE_ERROR,
};

void NtfInitFileHanderModel();
void NtfCloseFileHanderModel();


struct CFileTypeDesc
{
	CFileType		mFileType;
	CString			mExtNm;		// 扩展名, 如: .txt
	CString			mExtDesc;	// 文件对话框里用的扩展名过滤

	CFileTypeDesc(CFileType eType, CString szExtNm, CString szExtDessc)
	{
		mFileType = eType;
		mExtNm    = szExtNm;
		mExtDesc  = szExtDessc;
	}
};


CFileType get_file_type(CString fileNm);			// 匹配对应文件类型


//-----------------------------------------------------------------------------------------------------------------------------------
//
// 文件处理器
//
//------------------------------------------------------------------------------
class CAXPPackerDlg;
struct CTreeItm;
struct IFileHdlr
{
	CString		mszTips;
	virtual ~IFileHdlr(){}
	virtual CFileType get_type()const = 0;
	virtual bool file_proc(CAXPPackerDlg*, CTreeItm* ptr) = 0;
	virtual CString get_file_desc()const = 0;

	CString default_tips(CTreeItm* ptr);
	CString get_tips()const{return mszTips;}
};
IFileHdlr* get_file_proc(CFileType);
IFileHdlr* get_file_proc(CString szExtNm);	// 如: .txt

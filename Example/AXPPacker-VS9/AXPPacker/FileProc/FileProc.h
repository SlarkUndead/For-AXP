
#pragma once


enum CFileType
{
	FILE_TYPE_SOUND = 0,	// �����ļ�: WAV/MP3/...
	FILE_TYPE_IMAGE,		// ͼ���ļ�: JPG/DDS/TGA/..
	FILE_TYPE_MESH,			// Mesh�ļ�
	FILE_TYPE_SKELETON,		// skeleton
	FILE_TYPE_ENSCENE,		// ��������
	FILE_TYPE_SKY,			// ��պ�����
	FILE_TYPE_MATERIAL,		// ��������
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
	FILE_TYPE_ESD,			// esd(��)
	FILE_TYPE_GRIDINFO,		// GridInfo(����������Ϣ)
	FILE_TYPE_HEIGHTMAP,	// Heightmap
	FILE_TYPE_PATH,			// path
	FILE_TYPE_PROGRAM,		// program(shader)
	FILE_TYPE_REGION,		// Region(��������)
	FILE_TYPE_SREGION,		// SRegion(����)
	FILE_TYPE_TRACK,		// track
	FILE_TYPE_UNR,			// Unreach Region
	FILE_TYPE_WCOLLISION,	// WCollision(��ײ)
	FILE_TYPE_XML,			// xml
	FILE_TYPE_XSD,			// xsd(����xml)

	FILE_TYPE_ERROR,
};

void NtfInitFileHanderModel();
void NtfCloseFileHanderModel();


struct CFileTypeDesc
{
	CFileType		mFileType;
	CString			mExtNm;		// ��չ��, ��: .txt
	CString			mExtDesc;	// �ļ��Ի������õ���չ������

	CFileTypeDesc(CFileType eType, CString szExtNm, CString szExtDessc)
	{
		mFileType = eType;
		mExtNm    = szExtNm;
		mExtDesc  = szExtDessc;
	}
};


CFileType get_file_type(CString fileNm);			// ƥ���Ӧ�ļ�����


//-----------------------------------------------------------------------------------------------------------------------------------
//
// �ļ�������
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
IFileHdlr* get_file_proc(CString szExtNm);	// ��: .txt


// AXPPacker.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include <direct.h> // Ŀ¼����
#include "AXPPacker.h"
#include "AXPPackerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAXPPackerApp

BEGIN_MESSAGE_MAP(CAXPPackerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAXPPackerApp ����

CAXPPackerApp::CAXPPackerApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CAXPPackerApp ����

CAXPPackerApp theApp;



//-------------------------------------------------------------------------------------------------------------------------------------------------
CProgressCtrl* GetProgressCtrl()
{
	CAXPPackerDlg* pDlg = theApp.GetAXPPackerDlg();
	if(pDlg) return pDlg->GetProgCtrl();
	return 0;
}

// ����������
CProgressCtrl* g_ctrl = 0;
int g_pre_pos = 0;
void NtfStartProgress()
{
	g_ctrl = GetProgressCtrl();
	if(g_ctrl)
	{
		g_pre_pos = 0;
		g_ctrl->SetRange(0, 100);
		g_ctrl->SetStep(1);
		g_ctrl->SetPos(0);
		g_ctrl->ShowWindow(SW_SHOW);
	}
}

// ���ò���[0, 100]
void NtfSetProgress(int nPos)
{
	if(g_pre_pos != nPos)
	{
		g_pre_pos = nPos;
		g_ctrl->SetPos(nPos);
	}
}

// �رս�����
void NtfEndProgress()
{
	g_ctrl->SetPos(100);
	g_ctrl->ShowWindow(SW_HIDE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// ������ʾ��Ϣ
void NtfUpdatePubInfo(CString szInf)
{
	CAXPPackerDlg* pDlg = theApp.GetAXPPackerDlg();
	if(pDlg)
	{
		CStatic* pCtrl = pDlg->GetPubInfoCtrl();
		pCtrl->SetWindowTextA(szInf);
	}
}
void NtfDefaultPubInfo()
{
	NtfUpdatePubInfo(_T("������, ..."));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
// CAXPPackerApp ��ʼ��

BOOL CAXPPackerApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	//SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	CAXPPackerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

//---------------------------------------------------------------------------------------------------------
// ���б�ͷ�Ӹ�������, ��()��
void NotifyModifyColName(int nCol, CListCtrl* pCtrl, const char* szMsg, ...)
{
	if(pCtrl)
	{
		char pBf[2048] = "";
		memset(pBf, 0, sizeof(pBf));
		va_list list = 0;
		va_start(list, szMsg);
		vsprintf_s(pBf, sizeof(pBf), szMsg, list);
		va_end( list );

		HDITEM hdi;
		TCHAR  lpBuffer[512] = "";
		hdi.mask = HDI_TEXT;
		hdi.pszText = lpBuffer;
		hdi.cchTextMax = 512;
		CHeaderCtrl* pHdr = pCtrl->GetHeaderCtrl();
		if(pHdr->GetItem(nCol, &hdi))
		{
			CString szTxt = hdi.pszText;
			int nfnd = szTxt.FindOneOf("(");
			if(nfnd >= 0) szTxt = szTxt.Left(nfnd);
			if(pBf[0] != '\0')
				sprintf_s(hdi.pszText, sizeof(lpBuffer), "%s(%s)", szTxt, pBf);
			else sprintf_s(hdi.pszText, sizeof(lpBuffer), "%s", szTxt);
			pHdr->SetItem(nCol, &hdi);
		}
	}
}

//-----------------------------------------------------------------------
// Ŀ¼ѡ��
CString GetBrowseForFolder()
{
	LPMALLOC pMalloc;

	if(SHGetMalloc(&pMalloc) != NOERROR) return "";

	// Browse for a Folder.
	TCHAR path[ MAX_PATH ] = "";
	BROWSEINFO BI;         // �ļ������
	BI.hwndOwner = AfxGetMainWnd()->m_hWnd;
	BI.pidlRoot = NULL;
	BI.pszDisplayName = path;
	BI.lpszTitle = TEXT( "��ѡ��һ��Ŀ¼..." );
	BI.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	BI.lpfn = NULL;
	BI.lParam = 0;
	BI.iImage = 0;

	int answer = 1;
	ITEMIDLIST* pIDList = 0;
	BOOL bSucceeded = FALSE;
	while(answer == 1)
	{
		answer = 0;
		path[0] = '\0';
		pIDList = SHBrowseForFolder(&BI);
		if(pIDList)
		{
			bSucceeded = SHGetPathFromIDList(pIDList, path);// Get the Path.
			pMalloc->Free(pIDList);							// Free the ID List & the Allocator.
			pMalloc->Release();
			if(!bSucceeded)
			{
				answer = ::MessageBox( NULL, TEXT( "��ѡ��һ����Ч���ļ��У�" ),TEXT( "Error Infor" ), MB_OKCANCEL | MB_ICONWARNING );
			}
		}
	}
	size_t len = strlen(path);
	if((len > 0) && (path[len-1] != '\\'))
	{
		path[len] = '\\';
		path[len+1] = '\0' ;
	}
	return path;
}


//-----------------------------------------------------------------------
void SplitFullFilePath(std::string& path, const char* szFull)
{
	char szDrive[_MAX_DRIVE] = "\0";
	char szDir[_MAX_DIR]     = "\0";
	char szName[_MAX_FNAME]  = "\0";
	_splitpath_s(szFull, szDrive, sizeof(szDrive), szDir, sizeof(szDir),szName, sizeof(szName), 0, 0);
	path = szDrive;
	path += szDir;
}

//-----------------------------------------------------------------------
// �����ļ�Ŀ¼:�ɹ�:true | ʧ��:false
bool NotifyMakeDir(const char* str)
{
	if(!str)	return false;

	std::string last;
	char szDrive[_MAX_DRIVE] = "\0";
	char szDir[_MAX_DIR]     = "\0";
	_splitpath_s(str, szDrive, sizeof(szDrive), szDir, sizeof(szDir), 0, 0, 0, 0);
	last = szDrive;
	last += szDir;

	size_t p = 0;
	std::string mstr,dir;
	std::replace( last.begin(), last.end(), '/', '\\' );

	const char * ls = last.c_str();
	if(ls[last.size()-1] != '\\')
		last.append("\\");

	p = last.find_first_of('\\',4);
	while(p != std::string::npos)
	{
		mstr = last.substr(0, p);
		last = last.substr(p+1, last.size());
		dir.append(mstr);
		if(!::PathIsDirectory(dir.c_str()))	_mkdir(dir.c_str());
		p = last.find_first_of('\\',0);
		dir.append("\\");
	}
	return true;
}
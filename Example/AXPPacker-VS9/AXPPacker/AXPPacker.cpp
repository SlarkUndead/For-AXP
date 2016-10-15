
// AXPPacker.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include <direct.h> // 目录操作
#include "AXPPacker.h"
#include "AXPPackerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAXPPackerApp

BEGIN_MESSAGE_MAP(CAXPPackerApp, CWinAppEx)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAXPPackerApp 构造

CAXPPackerApp::CAXPPackerApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CAXPPackerApp 对象

CAXPPackerApp theApp;



//-------------------------------------------------------------------------------------------------------------------------------------------------
CProgressCtrl* GetProgressCtrl()
{
	CAXPPackerDlg* pDlg = theApp.GetAXPPackerDlg();
	if(pDlg) return pDlg->GetProgCtrl();
	return 0;
}

// 启动进度条
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

// 设置步幅[0, 100]
void NtfSetProgress(int nPos)
{
	if(g_pre_pos != nPos)
	{
		g_pre_pos = nPos;
		g_ctrl->SetPos(nPos);
	}
}

// 关闭进度条
void NtfEndProgress()
{
	g_ctrl->SetPos(100);
	g_ctrl->ShowWindow(SW_HIDE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// 公共提示信息
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
	NtfUpdatePubInfo(_T("空闲中, ..."));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------
// CAXPPackerApp 初始化

BOOL CAXPPackerApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();

	AfxEnableControlContainer();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	//SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CAXPPackerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

//---------------------------------------------------------------------------------------------------------
// 在列表头加附加内容, 在()里
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
// 目录选择
CString GetBrowseForFolder()
{
	LPMALLOC pMalloc;

	if(SHGetMalloc(&pMalloc) != NOERROR) return "";

	// Browse for a Folder.
	TCHAR path[ MAX_PATH ] = "";
	BROWSEINFO BI;         // 文件浏览器
	BI.hwndOwner = AfxGetMainWnd()->m_hWnd;
	BI.pidlRoot = NULL;
	BI.pszDisplayName = path;
	BI.lpszTitle = TEXT( "请选择一个目录..." );
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
				answer = ::MessageBox( NULL, TEXT( "请选择一个有效的文件夹！" ),TEXT( "Error Infor" ), MB_OKCANCEL | MB_ICONWARNING );
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
// 创建文件目录:成功:true | 失败:false
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
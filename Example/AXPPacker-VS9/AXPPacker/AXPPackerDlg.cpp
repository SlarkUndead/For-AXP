
// AXPPackerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AxpFile.h"
#include "MFCMemDC.h"
#include "AXPPacker.h"
#include "FMODAudio.h"
#include "AxpArchive.h"
#include "AXPPackerDlg.h"
#include "FileProc/FileProc.h"
#include "MFCImg/MFCIUserObj.h"
#include "MFCImg/MFCImageType.h"

#ifdef _DEBUG
	#define new DEBUG_NEW
#endif

#define ROOT_REG_KEY	HKEY_CURRENT_USER
#define SUB_REG_KEY		"Software\\PackTool\\AXPPacker"

#define ID_TIMER_APP		60000	// 应用程序使用的时钟
#define TIMER_SLICE			25		// 25毫秒

#define	MIN_CAL_WIDTH		200
#define	MIN_CAL_HEIGHT		200
#define	LOG_EDIT_HEIGHT		200		// 显示LOG区域的高度

#define OPTION_LOCKED()	\
	if(g_blck)			\
	{					\
		::MessageBox(GetSafeHwnd(), _T("正在执行其它操作"), _T("提示"), MB_OK|MB_ICONEXCLAMATION);\
		return;			\
	}

HANDLE g_hThread = 0;

volatile BOOL g_bstp = FALSE;		// TRUE: 线程结束
volatile BOOL g_blck = FALSE;		// TRUE: 操作锁定
volatile BOOL g_ref  = FALSE;		// 刷新操作控制, 左侧控件在删除/增加时会出现设置选中项导致频繁刷新显示

BOOL IsThreadOver(){return g_bstp;}	// TRUE: 线程停止继续执行

//---------------------------------------------------------------------------------------------
// 统一化控件
void NtfFormatListCtrl(CListCtrl* pLst)
{
	if(pLst)
	{
		DWORD dwStyle = pLst->GetExtendedStyle();
		pLst->SetExtendedStyle(dwStyle | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
		pLst->SetBkColor(RGB(247, 247, 255));
		pLst->SetTextColor(RGB(0, 0, 255));
		pLst->SetTextBkColor(RGB(247, 247, 255));
	}
}

//------------------------------------------------------------------------------------------------------------------------------------
// 往注册表里写数据
bool SetRegValueString(const char * ReValueName,const char * pValue)
{
	bool bAsw = false;	HKEY hKey;
	if(RegOpenKeyEx(ROOT_REG_KEY, SUB_REG_KEY, 0, KEY_WRITE,&hKey)!=ERROR_SUCCESS)
		RegCreateKey(ROOT_REG_KEY, SUB_REG_KEY, &hKey);
	if(RegSetValueEx(hKey, ReValueName, 0, REG_SZ,(const unsigned char*)pValue,(DWORD)strlen(pValue)+1)==ERROR_SUCCESS)
		bAsw = true;
	RegCloseKey(hKey);
	return bAsw;
}

//------------------------------------------------------------------------------------------------------------------------------------
// 读注册表, string
CString GetRegValueString(const char* pzValueName)
{
	HKEY hKey = 0;
	char pBuf[_MAX_PATH+1] = "\0";
	if(RegOpenKeyEx(ROOT_REG_KEY, SUB_REG_KEY, 0, KEY_QUERY_VALUE, &hKey)==ERROR_SUCCESS)
	{
		DWORD size = _MAX_PATH;
		RegQueryValueEx(hKey, pzValueName, 0, 0, (unsigned char*)pBuf, &size);
		RegCloseKey(hKey);
	}
	return pBuf;
}

//------------------------------------------------------------------------------------------------------------------------------------
// 把Log写到底部的Debug窗口同时写到文件里
void UILogWriter(COLORREF txtColor, const char* szMsg, ...)
{
	//char pBf[2048] = "";
	//memset(pBf, 0, sizeof(pBf));
	//va_list list = 0;
	//va_start(list, szMsg);
	//vsprintf_s(pBf, sizeof(pBf), szMsg, list);
	//va_end( list );
	//UILogMessage(txtColor, pBf, true);
}

//---------------------------------------------------------------------------------------------------------
void UILogMessage(COLORREF txtColor, const char* msg, bool out_time)
{
	//CTablePackToolDlg* ptr = theApp.GetAppDlg();
	//if(!ptr) return;
	//CString str;
	//if(out_time)
	//{
	//	str = GetCurrentTimeTxt();
	//	str.Append(msg);
	//	ptr->AddLineText(str, txtColor);
	//}
	//else ptr->AddLineText(msg, txtColor);
	//if(g_log) g_log->LogMessage(msg, 0, out_time);
}





//-------------------------------------------------------------------------------------------------------------------------------------------------
// 解包
unsigned __stdcall UnpackAxpFileThreadProc(void *Param)
{
	CAXPPackerDlg* pDlg = (CAXPPackerDlg*)Param;
	if(pDlg)
	{
		CString szfile = pDlg->GetOpenFile();
		if(!szfile.IsEmpty())
		{
			g_blck = TRUE;
			pDlg->EnableAppMenuItem(ID_OPEN_AXP, MF_GRAYED);
			CAxpArchive* pAxpArch = pDlg->GetAxpArchive();
			pAxpArch->load(szfile);
			pDlg->NtfGenTree();
			pDlg->EnableAppMenuItem(ID_OPEN_AXP, MF_ENABLED);
		}
	}
	g_blck = FALSE;
	g_hThread = 0;
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------
// 存选中的文件
unsigned __stdcall SaveAxpFileThreadProc(void *Param)
{
	CAXPPackerDlg* pDlg = (CAXPPackerDlg*)Param;
	if(pDlg) pDlg->NtfExportFiles();
	g_blck = FALSE;
	g_hThread = 0;
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------
// 存储但保持目录结构变
unsigned __stdcall SaveAllAxpFileThreadProc(void *Param)
{
	CAXPPackerDlg* pDlg = (CAXPPackerDlg*)Param;
	if(pDlg) pDlg->NtfExportAllFiles();
	g_blck = FALSE;
	g_hThread = 0;
	return 0;
}




//-------------------------------------------------------------------------------------------------------------------------------------------------
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAXPPackerDlg 对话框




CAXPPackerDlg::CAXPPackerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAXPPackerDlg::IDD, pParent), mbInit(FALSE), mpAxpArch(0), mpImgObj(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mpImgObj = new CMFCIUserObj();
}

CAXPPackerDlg::~CAXPPackerDlg()
{
	delete mpImgObj;
	mpImgObj = 0;
}

void CAXPPackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEM_TREE, mTreeCtrl);
	DDX_Control(pDX, IDC_ITEM_LIST, mLstCtrl);
	DDX_Control(pDX, IDC_PUB_PROG, mProgCtrl);
	DDX_Control(pDX, IDC_PUB_INFO, mPubInfCtrl);
	DDX_Control(pDX, IDC_STATIC_PICTURE, mPicCtrl);
	DDX_Control(pDX, IDC_FILE_INFO, mFileCtrl);
	DDX_Control(pDX, IDC_FIRST_BTN, mBtn0Ctrl);
	DDX_Control(pDX, IDC_PRE_BTN, mBtn1Ctrl);
	DDX_Control(pDX, IDC_NXT_BTN, mBtn2Ctrl);
	DDX_Control(pDX, IDC_LAST_BTN, mBtn3Ctrl);
}

BEGIN_MESSAGE_MAP(CAXPPackerDlg, CDialog)
	ON_COMMAND(ID_OPEN_AXP, &CAXPPackerDlg::OnUnpackAXP)
	ON_COMMAND(ID_SAVE_FILE, &CAXPPackerDlg::OnSaveThisFile)
	ON_COMMAND(ID_EXPORT_ALL, &CAXPPackerDlg::OnSaveAll)

	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGED, IDC_ITEM_TREE, &CAXPPackerDlg::OnTvnSelchangedItemTree)
	ON_NOTIFY(NM_CLICK, IDC_ITEM_LIST, &CAXPPackerDlg::OnNMClickItemList)
	ON_BN_CLICKED(IDC_FIRST_BTN, &CAXPPackerDlg::OnBnClickedFirstBtn)
	ON_BN_CLICKED(IDC_PRE_BTN, &CAXPPackerDlg::OnBnClickedPreBtn)
	ON_BN_CLICKED(IDC_NXT_BTN, &CAXPPackerDlg::OnBnClickedNxtBtn)
	ON_BN_CLICKED(IDC_LAST_BTN, &CAXPPackerDlg::OnBnClickedLastBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_ITEM_LIST, &CAXPPackerDlg::OnNMDblclkItemList)
	ON_NOTIFY(LVN_KEYDOWN, IDC_ITEM_LIST, &CAXPPackerDlg::OnLvnKeydownItemList)
END_MESSAGE_MAP()


// CAXPPackerDlg 消息处理程序

//-----------------------------------------------------------------------------------------------------------
// 主菜单里对应项可用状态控制
void CAXPPackerDlg::EnableAppMenuItem(UINT nItemID, UINT nEnable)
{
	mpMenu.EnableMenuItem(nItemID, nEnable);
	this->RedrawWindow();
}

//-----------------------------------------------------------------------------------------------------------
BOOL CAXPPackerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	NtfCreateFMODAudioSystem(500);		// 声音播放组件
	mpAxpArch = new CAxpArchive();
	mAllPg = mCurPg = 0;
	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	mpMenu.LoadMenu(IDR_MAIN_MENU);
	this->SetMenu(&mpMenu);

	mProgCtrl.SetBarColor(RGB(40, 210, 43));
	mProgCtrl.ShowWindow(SW_HIDE);

	CRect rect;
	
	mLstCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	mLstCtrl.InsertColumn(COL_FILE, "文件", LVCFMT_LEFT, (int)(rect.Width() * 0.4));
	mLstCtrl.InsertColumn(COL_PATH, "路径", LVCFMT_LEFT, (int)(rect.Width() * 0.5));
	NtfFormatListCtrl(&mLstCtrl);

	NtfInitFileHanderModel();
	NtfUpdateInfo(_T("未选中文件"));
	SetTimer(ID_TIMER_APP, TIMER_SLICE, 0);
	mbInit = TRUE;
	NotifyOnSize();
	NtfUpdateBtnState();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

//-----------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//-----------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
		case ID_TIMER_APP:
		{
			NtfUpdateAudioSystem(TIMER_SLICE * 0.001f);
			return;
		}
	}

	CDialog::OnTimer(nIDEvent);
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

//-----------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnPaint()
{
	//if (IsIconic())
	//{
	//	CPaintDC dc(this); // 用于绘制的设备上下文

	//	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	//	// 使图标在工作区矩形中居中
	//	int cxIcon = GetSystemMetrics(SM_CXICON);
	//	int cyIcon = GetSystemMetrics(SM_CYICON);
	//	CRect rect;
	//	GetClientRect(&rect);
	//	int x = (rect.Width() - cxIcon + 1) / 2;
	//	int y = (rect.Height() - cyIcon + 1) / 2;

	//	// 绘制图标
	//	dc.DrawIcon(x, y, m_hIcon);
	//}
	//else
	//{
	//	CDialog::OnPaint();
	//}
	if(mpImgObj)
	{
		CPaintDC dc(this);
		CRect rect;
		mPicCtrl.GetClientRect(&rect);
		CClientDC preDC(&mPicCtrl) ;
		FCMemDC   memDC(&preDC, rect) ;
		memDC.FillSolidRect(rect,RGB(255, 255, 255)) ;

		mpImgObj->RenderUserImage(memDC.m_hDC, rect);
		memDC.FinalBlt();
	}
}

//-----------------------------------------------------------------------------------------------------------
//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CAXPPackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-----------------------------------------------------------------------------------------------------------
// 重置
void CAXPPackerDlg::NtfReset()
{
	mAllPg = mCurPg = 0;
	mLstCtrl.DeleteAllItems();
	mTreeCtrl.DeleteAllItems();
	NtfClearShowList();
	ClearTreeRoot();
	NtfUpdateBtnState();
}

//-----------------------------------------------------------------------------------------------------------
// 窗口大小改变
void CAXPPackerDlg::NotifyOnSize()
{
	if(!mbInit) return;
	CRect rect;
	GetClientRect(&rect);
	//if(rect.Width() <= MIN_CAL_WIDTH) return;
	//if(rect.Height() <= MIN_CAL_HEIGHT) return;


	const LONG lTop = 40;
	RECT mvRct;
	
	// 公共信息框
	mvRct.left   = 10;
	mvRct.top    = 15;
	mvRct.right  = (int)(0.6f * rect.Width());
	mvRct.bottom = mvRct.top + 25;
	mPubInfCtrl.MoveWindow(&mvRct);
	
	// 进度条
	mvRct.left   = mvRct.right + 10;
	mvRct.top    = 15;
	mvRct.right  = rect.Width() - 10;
	mvRct.bottom = mvRct.top + 18;
	mProgCtrl.MoveWindow(&mvRct);


	// 左侧目录树
	mvRct.left   = 10;
	mvRct.top    = lTop;
	mvRct.right  = 300;		// 固定
	mvRct.bottom = rect.bottom - MIN_CAL_HEIGHT;
	mTreeCtrl.MoveWindow(&mvRct);

	// 右侧目录下文件列表
	mvRct.left   = mvRct.right + 10;
	mvRct.top    = lTop;
	mvRct.right  = rect.Width() - 210;
	mvRct.bottom = rect.bottom - MIN_CAL_HEIGHT;
	mLstCtrl.MoveWindow(&mvRct);
	
	// 显示图片的控件
	mvRct.left   = mvRct.right + 10;
	mvRct.top    = lTop;
	mvRct.right  = rect.Width() - 10;
	LONG dwH = mvRct.right - mvRct.left;
	mvRct.bottom = mvRct.top + dwH;
	mPicCtrl.MoveWindow(&mvRct);

	// 显示文件信息的控件
	mvRct.left   = mvRct.left;
	mvRct.top    = mvRct.bottom + 10;
	mvRct.right  = mvRct.right;
	mvRct.bottom = mvRct.top + dwH;
	mFileCtrl.MoveWindow(&mvRct);


	// 调整按钮位置
	LONG spc = 2, btnH = 25, btnW = 46;
	mvRct.left   = mvRct.left;
	mvRct.top    = mvRct.bottom + 10;	// 离文件信息的控件的像素数
	mvRct.right  = mvRct.left + btnW;
	mvRct.bottom = mvRct.top + btnH;
	mBtn0Ctrl.MoveWindow(&mvRct);
	
	mvRct.left   = mvRct.right + spc;
	mvRct.right  = mvRct.left + btnW;
	mBtn1Ctrl.MoveWindow(&mvRct);

	mvRct.left   = mvRct.right + spc;
	mvRct.right  = mvRct.left + btnW;
	mBtn2Ctrl.MoveWindow(&mvRct);
	
	mvRct.left   = mvRct.right + spc;
	mvRct.right  = mvRct.left + btnW;
	mBtn3Ctrl.MoveWindow(&mvRct);



	// 调整文件列表的列宽
	mLstCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	int nLftW = mLstCtrl.GetColumnWidth(0);
	mLstCtrl.SetColumnWidth(COL_PATH, rect.Width() - nLftW - 10);

}

//---------------------------------------------------------------------------------------------------------------------------
// 改变控制页的按钮的状态
void CAXPPackerDlg::NtfUpdateBtnState()
{
	mBtn0Ctrl.EnableWindow(mCurPg > 1);			// 首页
	mBtn1Ctrl.EnableWindow(mCurPg > 1);			// 上一页
	mBtn2Ctrl.EnableWindow(mCurPg < mAllPg);	// 下一页
	mBtn3Ctrl.EnableWindow(mCurPg < mAllPg);	// 尾页
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::NtfCloseImg()
{
	if(mpImgObj->CloseUserImage())
		Invalidate();
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}

//---------------------------------------------------------------------------------------------------------------------------
// 读AXP文件
void CAXPPackerDlg::OnUnpackAXP()
{
	OPTION_LOCKED();

	CString pzfile = GetRegValueString(_T("UnpackAfx"));
	CFileDialog dlg(TRUE, 0, pzfile, OFN_HIDEREADONLY, "Axp File(*.axp)|*.axp||");
	dlg.m_ofn.lpstrInitialDir = pzfile;
	if(IDOK == dlg.DoModal())
	{
		g_ref = FALSE;
		NtfReset();
		UINT tid = 0;
		mOpenFile = dlg.GetPathName();
		SetRegValueString("UnpackAfx", mOpenFile);
		g_hThread = (HANDLE)_beginthreadex(NULL, 0, UnpackAxpFileThreadProc, this, 0, &tid);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
// 存储选中的文件
void CAXPPackerDlg::OnSaveThisFile()
{
	OPTION_LOCKED();
	mExpLst.clear();

	NtfUpdatePubInfo(_T("正在生成存储列表, ..."));
	CTreeItm* ptr = 0;
	POSITION pos = mLstCtrl.GetFirstSelectedItemPosition();
	while(pos)
	{
		int index = mLstCtrl.GetNextSelectedItem(pos);
		ptr = (CTreeItm*) mLstCtrl.GetItemData(index);
		if(ptr && ptr->mAxpItem) mExpLst.push_back(ptr);
	}
	if(mExpLst.empty())
	{
		NtfDefaultPubInfo();
		::MessageBox(GetSafeHwnd(), _T("没有可存储的文件列表"), _T("提示"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	mSelPath = GetBrowseForFolder();
	if(!mSelPath.IsEmpty())
	{
		UINT tid = 0;
		g_hThread = (HANDLE)_beginthreadex(NULL, 0, SaveAxpFileThreadProc, this, 0, &tid);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
// 保持目录结构不变
void CAXPPackerDlg::OnSaveAll()
{
	OPTION_LOCKED();
	const AxpFVct& pLst = mpAxpArch->GetAxpFileList();
	if(pLst.empty())
	{
		NtfDefaultPubInfo();
		::MessageBox(GetSafeHwnd(), _T("没有可存储的文件"), _T("提示"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	mSelPath = GetBrowseForFolder();
	if(!mSelPath.IsEmpty())
	{
		UINT tid = 0;
		g_hThread = (HANDLE)_beginthreadex(NULL, 0, SaveAllAxpFileThreadProc, this, 0, &tid);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	NotifyOnSize();
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	g_bstp = TRUE;
	ShowWindow(SW_HIDE);
	KillTimer(ID_TIMER_APP);
	NtfCloseFileHanderModel();
	NtfShutDownFMod();

	ClearTreeRoot();
	delete mpImgObj;
	delete mpAxpArch;
	mpAxpArch = 0;
	mpImgObj  = 0;
	CDialog::OnCancel();
}


//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnTvnSelchangedItemTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	if(g_ref)
	{
		CTreeItm* pItm = 0;
		HTREEITEM hItem = mTreeCtrl.GetSelectedItem();
		if(hItem)
		{
			pItm = (CTreeItm*)mTreeCtrl.GetItemData(hItem);
		}

		NtfCloseImg();
		NtfResetShowList(pItm);
		NtfShowTreeItems(0);
		size_t nUsed = 0, nUnu = 0, nSub = 0;
		if(pItm)
		{
			nUsed = pItm->mFileLst.size();
			nSub  = pItm->mSubLst.size();
		}
		CListCtrl* pCtrl = GetListCtrl();
		NotifyModifyColName(COL_FILE, pCtrl, "文件数量:%d, 目录数量:%d", nUsed, nSub);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
CString get_size_desc(UINT fsize)
{
	CString szInf;
	if(fsize > (1024 * 1024))
	{
		float fn = (float)fsize/(1024.0f * 1024.0f);
		szInf.Format("%.2fK(%dB)", fn, fsize);
	}
	else if(fsize > 1024)
	{
		float fn = (float)fsize/1024.0f;
		szInf.Format("%.2fK(%dB)", fn, fsize);
	}
	else szInf.Format("%4dB", fsize);
	return szInf;
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnNMClickItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	LPNM_LISTVIEW pItem = (LPNM_LISTVIEW)pNMHDR;
	if(pItem && (pItem->iItem >= 0))
	{
		CTreeItm* ptr = (CTreeItm*) mLstCtrl.GetItemData(pItem->iItem);
		if(ntf_click_item(ptr))	return;
	}
	NtfDefaultPubInfo();
	NtfUpdateInfo(_T("未选中文件"));
	NtfCloseImg();
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnLvnKeydownItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if(!pLVKeyDow) return;
	switch(pLVKeyDow->wVKey)
	{
		case VK_UP:	// UP ARROW key
		{
			int nCount = mLstCtrl.GetItemCount();
			POSITION pos = mLstCtrl.GetFirstSelectedItemPosition();
			int index = mLstCtrl.GetNextSelectedItem(pos);
			if(index < 0)
			{
				NtfDefaultPubInfo();
				NtfUpdateInfo(_T("未选中文件"));
				NtfCloseImg();
				return;
			}
			if(index > 0) index -= 1;
			CTreeItm* ptr = (CTreeItm*) mLstCtrl.GetItemData(index);
			ntf_click_item(ptr);
			break;
		}
		case VK_DOWN:	// DOWN ARROW key
		{
			int nCount = mLstCtrl.GetItemCount();
			POSITION pos = mLstCtrl.GetFirstSelectedItemPosition();
			int index = mLstCtrl.GetNextSelectedItem(pos) + 1;
			if(nCount > 0) index = __min(nCount - 1, index);
			if(index >= 0)
			{
				CTreeItm* ptr = (CTreeItm*) mLstCtrl.GetItemData(index);
				if(ntf_click_item(ptr)) return;
			}
			
			NtfDefaultPubInfo();
			NtfUpdateInfo(_T("未选中文件"));
			NtfCloseImg();
			break;
		}
		case VK_DELETE:	// DEL key
		{
			break;
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
// 双击则显示保存对话框
void CAXPPackerDlg::OnNMDblclkItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	LPNM_LISTVIEW pItem = (LPNM_LISTVIEW)pNMHDR;
	if(pItem && (pItem->iItem >= 0))
	{
		CTreeItm* ptr = (CTreeItm*) mLstCtrl.GetItemData(pItem->iItem);
		if(ptr)
		{
			char szExt[_MAX_EXT]  = "\0";
			const AxpFile* pAxp = ptr->mAxpItem;
			CString fileNm = ptr->get_name();
			_splitpath_s(fileNm, 0, 0, 0, 0,0, 0, szExt, sizeof(szExt));
			CString flt = szExt + 1;
			CString filet;
			flt.MakeUpper();
			filet.Format(_T("%s File(*%s)|*%s||"), flt, szExt, szExt);
			
			CString pzPth = GetRegValueString(_T("AXPExpPath"));
			CFileDialog dlg(FALSE, 0, fileNm, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, filet);
			dlg.m_ofn.lpstrInitialDir = pzPth;
			if(IDOK == dlg.DoModal())
			{
				CString oflt = szExt;
				oflt.MakeLower();

				CString file = dlg.GetPathName();
				_splitpath_s(file, 0, 0, 0, 0,0, 0, szExt, sizeof(szExt));
				CString nflt = szExt;
				if(nflt.IsEmpty())	file += oflt;

				SetRegValueString("AXPExpPath", file);
				NtfUpdatePubInfo(_T("正在存储文件: ") + file);
				mpAxpArch->SaveToFile(pAxp, file);
				NtfDefaultPubInfo();
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnBnClickedFirstBtn()
{
	NtfShowTreeItems(0);
}

void CAXPPackerDlg::OnBnClickedPreBtn()
{
	NtfShowTreeItems(1);
}

void CAXPPackerDlg::OnBnClickedNxtBtn()
{
	NtfShowTreeItems(2);
}

void CAXPPackerDlg::OnBnClickedLastBtn()
{
	NtfShowTreeItems(3);
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::ClearTreeRoot()
{
	for(size_t i = 0; i < mItmRoot.size(); ++i)
	{
		CTreeItm* ptr = mItmRoot[i];
		ptr->clear_tree_item();
		delete ptr;
	}
	mItmRoot.clear();
}

//---------------------------------------------------------------------------------------------------------------------------
// 根据名字查找
CTreeItm* CAXPPackerDlg::GetTreeRoot(CString szNm)
{
	for(size_t i = 0; i < mItmRoot.size(); ++i)
	{
		CTreeItm* ptr = mItmRoot[i];
		if(ptr->mItmName == szNm)
			return ptr;
	}
	return 0;
}

//---------------------------------------------------------------------------------------------------------------------------
// 构造显示用的树型结构
float g_frate = 0.0f;
void repair_list(StrLst& splt)
{
	if(splt.size() == 1)
	{
		CString sz = splt.front();
		if(sz.Find(".") != 0)
		{
			splt.push_front(DEF_GRP);
		}
	}
}

void CAXPPackerDlg::NtfGenTree()
{
	if(IsThreadOver()) return;
	mLstCtrl.DeleteAllItems();
	mTreeCtrl.DeleteAllItems();
	ClearTreeRoot();

	const AxpFVct& pLst = mpAxpArch->GetAxpFileList();
	if(!pLst.empty())
	{
		int nAll = 0, nproc = 0;
		float fpos = 0.0f;

		NtfUpdatePubInfo(_T("正在拆分, ..."));
		NtfStartProgress();

		StrLst splt;
		float fCount = (float)pLst.size();
		g_frate = 1.0f/fCount * 100.0f;
		for(size_t i = 0; i < pLst.size(); ++i)
		{
			const AxpFile* pItm = pLst[i];
			split_string(pItm->szFile, splt, _T("\\"));
			repair_list(splt);
			NtfAddTreeRoot(splt, pItm, nAll);
			if(IsThreadOver()) return;

			fpos = g_frate * (float)i;
			NtfSetProgress((int)fpos);
		}
		NtfEndProgress();

		g_ref  = TRUE;
		NtfAddToTreeCtrl(nAll);	// 生成控件树
	}
	else this->Invalidate();

}

//---------------------------------------------------------------------------------------------------------------------------
// 逐层加入
void CAXPPackerDlg::NtfAddTreeRoot(StrLst& pLst, const AxpFile* pAxp, int& nAll)
{
	if(pLst.empty()) return;
	CString szNm = pLst.front();
	pLst.pop_front();

	CTreeItm* prt = GetTreeRoot(szNm);
	if(!prt)
	{
		++nAll;
		prt = new CTreeItm;
		prt->mAxpItem = 0;
		prt->mpParent = 0;
		prt->mItmName = szNm;
		mItmRoot.push_back(prt);
	}
	NtfAddTreeItem(prt, pLst, pAxp, nAll);
}

//---------------------------------------------------------------------------------------------------------------------------
// 字节点
void CAXPPackerDlg::NtfAddTreeItem(CTreeItm* ptr, StrLst& pLst, const AxpFile* pAxp, int& nAll)
{
	if(IsThreadOver() || pLst.empty()) return;

	CString szNm = pLst.front();
	pLst.pop_front();
	if(pLst.empty())
	{
		++nAll;
		ptr->add_file_item(szNm, pAxp);
	}
	else
	{
		CTreeItm* pItm = ptr->add_sub_item(szNm);
		NtfAddTreeItem(pItm, pLst, pAxp, nAll);
	}
}

//---------------------------------------------------------------------------------------------------------------------------
// 加到树型控件上
void CAXPPackerDlg::NtfAddToTreeCtrl(int nAll)
{
	//if(!mItmRoot.empty())
	{
		NtfUpdatePubInfo(_T("正在生成显示列表, ..."));
		g_frate = 1.0f/nAll * 100.0f;
		NtfStartProgress();

		CTreeCtrl* pTree = GetTreeCtrl();
		pTree->SetRedraw(FALSE);
		HTREEITEM hRoot = 0;
		CTreeItm* pItm = 0;
		int nCur = 0;
		for(size_t i = 0; i < mItmRoot.size(); ++i)
		{
			if(IsThreadOver()) return;
			++nCur;
			pItm = mItmRoot[i];
			hRoot = pTree->InsertItem(pItm->mItmName, 0, 1, TVI_ROOT, TVI_LAST);
			pTree->SetItemData(hRoot, (DWORD_PTR)pItm);
			NtfSetProgress((int)(g_frate * (float)nCur));

			NtfAddTreeCtrlItem(pItm, hRoot, nCur);

		}
		hRoot = pTree->GetRootItem();
		if(hRoot)
		{
			pTree->Expand(hRoot, TVE_EXPAND);
			pTree->SelectItem(hRoot);
		}
		pTree->SetRedraw(TRUE);
		
		NtfEndProgress();
		NtfDefaultPubInfo();
	}
}

//---------------------------------------------------------------------------------------------------------------------------
// 加自节点
void CAXPPackerDlg::NtfAddTreeCtrlItem(CTreeItm* pRItm, HTREEITEM hPrnt, int& nCur)
{
	if(IsThreadOver()) return;
	HTREEITEM hAfter = hPrnt;
	CTreeCtrl* pTree = GetTreeCtrl();
	for(size_t i = 0; i < pRItm->mSubLst.size(); ++i)
	{
		CTreeItm* pItm = (pRItm->mSubLst[i]);
		if(pItm->mSubLst.empty())
		{
			hAfter = pTree->InsertItem(pItm->mItmName, 0, 1, hPrnt, hAfter);
			pTree->SetItemData(hAfter, (DWORD_PTR)pItm);
		}
		else
		{
			char lpszItem[256] = "";
			_snprintf_s(lpszItem, _countof(lpszItem), "%s\0", pItm->mItmName);
			TV_INSERTSTRUCT tvis;
			ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));
			tvis.hParent			 = hPrnt;
			tvis.hInsertAfter		 = hAfter;
			tvis.item.mask			 = TVIF_CHILDREN|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
			tvis.item.pszText		 = lpszItem;
			tvis.item.iImage		 = 0;
			tvis.item.iSelectedImage = 1;
			tvis.item.cChildren		 = TRUE;
			HTREEITEM hItm = pTree->InsertItem(&tvis);
			pTree->SetItemData(hItm, (DWORD_PTR)pItm);
			NtfAddTreeCtrlItem(pItm, hItm, nCur);
			hAfter = hItm;
		}
		++nCur;
		NtfSetProgress((int)(g_frate * nCur));
		if(IsThreadOver()) return;
	}
}


//-----------------------------------------------------------------------------------------------------------
// 0 -- 第一页 | 1 -- 前一页 | 2 -- 下一页 | 3 -- 最后一页
void change_page(size_t& nCur, size_t nAll, int nType)
{
	if(0 == nType) nCur = 1;
	else if(1 == nType)
	{
		if(nCur > 1) nCur -= 1;
	}
	else if(2 == nType)
	{
		if((nCur + 1) <= nAll) nCur += 1;
	}
	else if(3 == nType)
	{
		nCur = nAll;
	}
}

//------------------------------------------------------------------------------------------------
// 计算总页数
void notify_cal_page(size_t& nCurPg, size_t& nAllPg, size_t nCounts, size_t pg_count)
{
	nAllPg = nCounts/pg_count;
	if((nAllPg * pg_count) < nCounts)	nAllPg += 1;
	if(nCurPg <= 0) nCurPg = 1;
	if(nCurPg > nAllPg) nCurPg = nAllPg;
}

//-----------------------------------------------------------------------------------------------------------
size_t CAXPPackerDlg::GetMaxPageCount()const
{
	return 200;
}

//-----------------------------------------------------------------------------------------------------------
// 重置显示列表
void CAXPPackerDlg::NtfResetShowList(CTreeItm* pItm)
{
	mAllPg = 0;					// 总页数
	mCurPg = 0;					// 当前显示到的页数
	mShowLst.clear();			// 显示列表
	if(pItm)
	{
		TrItmVct& pULst = pItm->mFileLst;
		mShowLst.assign(pULst.begin(), pULst.end());
		notify_cal_page(mCurPg, mAllPg, mShowLst.size(), GetMaxPageCount());
	}
}

//-----------------------------------------------------------------------------------------------------------
// 填充到UI上, 0(第一页)/1(前一页)/2(下一页)/3(最后一页)
void CAXPPackerDlg::NtfShowTreeItems(int nType)
{
	CListCtrl* pCtrl = GetListCtrl();
	pCtrl->SetRedraw(FALSE);
	pCtrl->DeleteAllItems();
	NtfDefaultPubInfo();
	NtfUpdateInfo(_T("未选中文件"));
	change_page(mCurPg, mAllPg, nType);
	size_t nCount = mShowLst.size();
	if((mCurPg > 0) && (nCount > 0))
	{
		size_t nMax = GetMaxPageCount();
		size_t nBegin = (mCurPg - 1) * nMax;
		size_t nEnd   = nBegin + nMax;
		if(nEnd >= nCount) nEnd = nCount;

		int nNum = 0, nIndex = 0;
		const CTreeItm* ptr = 0;
		for(int i = 0; nBegin < nEnd; ++nBegin, ++i)
		{
			ptr    = mShowLst[i];
			nIndex = pCtrl->InsertItem(i, "");
			pCtrl->SetItemText(nIndex, COL_FILE, ptr->mItmName);
			pCtrl->SetItemText(nIndex, COL_PATH, ptr->get_name_path());
			pCtrl->SetItemData(i, (DWORD_PTR)ptr);
		}
	}
	pCtrl->SetRedraw(TRUE);
	NtfUpdateBtnState();
}

//-----------------------------------------------------------------------------------------------------------
// 显示文件信息
void CAXPPackerDlg::NtfUpdateInfoCtrl(const char* szMsg, ...)
{
	char pBf[2048] = "";
	memset(pBf, 0, sizeof(pBf));
	va_list list = 0;
	va_start(list, szMsg);
	vsprintf_s(pBf, sizeof(pBf), szMsg, list);
	va_end( list );
	NtfUpdateInfo(pBf);
}

void CAXPPackerDlg::NtfUpdateInfo(const CString& szInf)
{
	mFileCtrl.SetWindowTextA(szInf);
}

//-----------------------------------------------------------------------------------------------------------
CString get_def_tips(CTreeItm* ptr)
{
	if(ptr)
	{
		const AxpFile* pAxp = ptr->mAxpItem;
		if(pAxp)
		{
			CString szInf;
			CString szDesc = "未知文件类型";
			CString szfs = get_size_desc(pAxp->dwSize);
			szInf.Format(_T("Code: %s\n大小: %s\n类型: %s\n路径: %s"), pAxp->szAddr, szfs, szDesc, pAxp->szFile);
			return szInf;
		}
	}
	return _T("输入错误");
}

//-----------------------------------------------------------------------------------------------------------
// 处理选择的项
bool CAXPPackerDlg::ntf_click_item(CTreeItm* ptr)
{
	if(ptr && ptr->mAxpItem)
	{
		NtfCloseImg();
		const AxpFile* pAxp = ptr->mAxpItem;
		NtfUpdatePubInfo(_T("正在处理: ") + pAxp->szFile);

		CString szInf;
		IFileHdlr* pHdlr = get_file_proc(pAxp->szFile);
		if(pHdlr)
		{
			pHdlr->file_proc(this, ptr);
			szInf = pHdlr->get_tips();
		}
		else szInf = get_def_tips(ptr);
		NtfUpdateInfoCtrl(szInf);
		NtfUpdatePubInfo(_T("已选择: ") + pAxp->szFile);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------
// 导出指定的文件
void CAXPPackerDlg::NtfExportFiles()
{
	if(mExpLst.empty()) return;
	g_blck = TRUE;
	// 创建目录
	if(mExpLst.size() > 1)
	{
		char szName[_MAX_FNAME]  = "\0";
		mSelPath.Replace(_T("/"), _T("\\"));
		if(mSelPath.Right(1) != _T("\\")) mSelPath += _T("\\");
		String szAxpf = mpAxpArch->GetArchiveFile();
		_splitpath_s(szAxpf, 0, 0, 0, 0, szName, sizeof(szName), 0, 0);
		mSelPath += szName;
		mSelPath += _T("\\");
		NotifyMakeDir(mSelPath);
	}

	CString file;
	CTreeItm* pItm = 0;

	NtfStartProgress();
	float fRate = 1.0f/(float)(mExpLst.size()) * 100.0f;
	for(size_t i = 0; i < mExpLst.size(); ++i)
	{
		pItm = mExpLst[i];
		const AxpFile* pAxp = pItm->mAxpItem;
		file = mSelPath + pItm->get_name();
		NtfUpdatePubInfo(_T("正在存储文件: ") + file);
		mpAxpArch->SaveToFile(pAxp, file);
		NtfSetProgress((int)(fRate * i));
		if(IsThreadOver()) return;
	}
	mExpLst.clear();
	NtfEndProgress();
	NtfDefaultPubInfo();
}

//-----------------------------------------------------------------------------------------------------------
// 保持目录结构变而存储
void CAXPPackerDlg::NtfExportAllFiles()
{
	const AxpFVct& pLst = mpAxpArch->GetAxpFileList();
	if(pLst.empty()) return;
	g_blck = TRUE;

	
	char szName[_MAX_FNAME]  = "\0";
	mSelPath.Replace(_T("/"), _T("\\"));
	String szAxpf = mpAxpArch->GetArchiveFile();
	if(mSelPath.Right(1) != _T("\\")) mSelPath += _T("\\");
	_splitpath_s(szAxpf, 0, 0, 0, 0, szName, sizeof(szName), 0, 0);
	mSelPath += szName;
	mSelPath += _T("\\");


	CString file;
	const AxpFile* pAxp = 0;

	NtfStartProgress();
	float fRate = 1.0f/(float)(pLst.size()) * 100.0f;
	for(size_t i = 0; i < pLst.size(); ++i)
	{
		pAxp = pLst[i];
		file = mSelPath + pAxp->szFile;
		NtfUpdatePubInfo(_T("正在存储文件: ") + file);
		NotifyMakeDir(file);							// 创建目录
		mpAxpArch->SaveToFile(pAxp, file);
		NtfSetProgress((int)(fRate * i));
		if(IsThreadOver()) return;
	}
	mExpLst.clear();
	NtfEndProgress();
	NtfDefaultPubInfo();
}

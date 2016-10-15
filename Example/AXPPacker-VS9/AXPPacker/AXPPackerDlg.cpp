
// AXPPackerDlg.cpp : ʵ���ļ�
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

#define ID_TIMER_APP		60000	// Ӧ�ó���ʹ�õ�ʱ��
#define TIMER_SLICE			25		// 25����

#define	MIN_CAL_WIDTH		200
#define	MIN_CAL_HEIGHT		200
#define	LOG_EDIT_HEIGHT		200		// ��ʾLOG����ĸ߶�

#define OPTION_LOCKED()	\
	if(g_blck)			\
	{					\
		::MessageBox(GetSafeHwnd(), _T("����ִ����������"), _T("��ʾ"), MB_OK|MB_ICONEXCLAMATION);\
		return;			\
	}

HANDLE g_hThread = 0;

volatile BOOL g_bstp = FALSE;		// TRUE: �߳̽���
volatile BOOL g_blck = FALSE;		// TRUE: ��������
volatile BOOL g_ref  = FALSE;		// ˢ�²�������, ���ؼ���ɾ��/����ʱ���������ѡ�����Ƶ��ˢ����ʾ

BOOL IsThreadOver(){return g_bstp;}	// TRUE: �߳�ֹͣ����ִ��

//---------------------------------------------------------------------------------------------
// ͳһ���ؼ�
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
// ��ע�����д����
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
// ��ע���, string
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
// ��Logд���ײ���Debug����ͬʱд���ļ���
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
// ���
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
// ��ѡ�е��ļ�
unsigned __stdcall SaveAxpFileThreadProc(void *Param)
{
	CAXPPackerDlg* pDlg = (CAXPPackerDlg*)Param;
	if(pDlg) pDlg->NtfExportFiles();
	g_blck = FALSE;
	g_hThread = 0;
	return 0;
}

//------------------------------------------------------------------------------------------------------------------------------
// �洢������Ŀ¼�ṹ��
unsigned __stdcall SaveAllAxpFileThreadProc(void *Param)
{
	CAXPPackerDlg* pDlg = (CAXPPackerDlg*)Param;
	if(pDlg) pDlg->NtfExportAllFiles();
	g_blck = FALSE;
	g_hThread = 0;
	return 0;
}




//-------------------------------------------------------------------------------------------------------------------------------------------------
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CAXPPackerDlg �Ի���




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


// CAXPPackerDlg ��Ϣ�������

//-----------------------------------------------------------------------------------------------------------
// ���˵����Ӧ�����״̬����
void CAXPPackerDlg::EnableAppMenuItem(UINT nItemID, UINT nEnable)
{
	mpMenu.EnableMenuItem(nItemID, nEnable);
	this->RedrawWindow();
}

//-----------------------------------------------------------------------------------------------------------
BOOL CAXPPackerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	NtfCreateFMODAudioSystem(500);		// �����������
	mpAxpArch = new CAxpArchive();
	mAllPg = mCurPg = 0;
	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	mpMenu.LoadMenu(IDR_MAIN_MENU);
	this->SetMenu(&mpMenu);

	mProgCtrl.SetBarColor(RGB(40, 210, 43));
	mProgCtrl.ShowWindow(SW_HIDE);

	CRect rect;
	
	mLstCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	mLstCtrl.InsertColumn(COL_FILE, "�ļ�", LVCFMT_LEFT, (int)(rect.Width() * 0.4));
	mLstCtrl.InsertColumn(COL_PATH, "·��", LVCFMT_LEFT, (int)(rect.Width() * 0.5));
	NtfFormatListCtrl(&mLstCtrl);

	NtfInitFileHanderModel();
	NtfUpdateInfo(_T("δѡ���ļ�"));
	SetTimer(ID_TIMER_APP, TIMER_SLICE, 0);
	mbInit = TRUE;
	NotifyOnSize();
	NtfUpdateBtnState();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

//-----------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnPaint()
{
	//if (IsIconic())
	//{
	//	CPaintDC dc(this); // ���ڻ��Ƶ��豸������

	//	SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

	//	// ʹͼ���ڹ����������о���
	//	int cxIcon = GetSystemMetrics(SM_CXICON);
	//	int cyIcon = GetSystemMetrics(SM_CYICON);
	//	CRect rect;
	//	GetClientRect(&rect);
	//	int x = (rect.Width() - cxIcon + 1) / 2;
	//	int y = (rect.Height() - cyIcon + 1) / 2;

	//	// ����ͼ��
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
//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CAXPPackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//-----------------------------------------------------------------------------------------------------------
// ����
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
// ���ڴ�С�ı�
void CAXPPackerDlg::NotifyOnSize()
{
	if(!mbInit) return;
	CRect rect;
	GetClientRect(&rect);
	//if(rect.Width() <= MIN_CAL_WIDTH) return;
	//if(rect.Height() <= MIN_CAL_HEIGHT) return;


	const LONG lTop = 40;
	RECT mvRct;
	
	// ������Ϣ��
	mvRct.left   = 10;
	mvRct.top    = 15;
	mvRct.right  = (int)(0.6f * rect.Width());
	mvRct.bottom = mvRct.top + 25;
	mPubInfCtrl.MoveWindow(&mvRct);
	
	// ������
	mvRct.left   = mvRct.right + 10;
	mvRct.top    = 15;
	mvRct.right  = rect.Width() - 10;
	mvRct.bottom = mvRct.top + 18;
	mProgCtrl.MoveWindow(&mvRct);


	// ���Ŀ¼��
	mvRct.left   = 10;
	mvRct.top    = lTop;
	mvRct.right  = 300;		// �̶�
	mvRct.bottom = rect.bottom - MIN_CAL_HEIGHT;
	mTreeCtrl.MoveWindow(&mvRct);

	// �Ҳ�Ŀ¼���ļ��б�
	mvRct.left   = mvRct.right + 10;
	mvRct.top    = lTop;
	mvRct.right  = rect.Width() - 210;
	mvRct.bottom = rect.bottom - MIN_CAL_HEIGHT;
	mLstCtrl.MoveWindow(&mvRct);
	
	// ��ʾͼƬ�Ŀؼ�
	mvRct.left   = mvRct.right + 10;
	mvRct.top    = lTop;
	mvRct.right  = rect.Width() - 10;
	LONG dwH = mvRct.right - mvRct.left;
	mvRct.bottom = mvRct.top + dwH;
	mPicCtrl.MoveWindow(&mvRct);

	// ��ʾ�ļ���Ϣ�Ŀؼ�
	mvRct.left   = mvRct.left;
	mvRct.top    = mvRct.bottom + 10;
	mvRct.right  = mvRct.right;
	mvRct.bottom = mvRct.top + dwH;
	mFileCtrl.MoveWindow(&mvRct);


	// ������ťλ��
	LONG spc = 2, btnH = 25, btnW = 46;
	mvRct.left   = mvRct.left;
	mvRct.top    = mvRct.bottom + 10;	// ���ļ���Ϣ�Ŀؼ���������
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



	// �����ļ��б���п�
	mLstCtrl.GetWindowRect(&rect);
	ScreenToClient(&rect);
	int nLftW = mLstCtrl.GetColumnWidth(0);
	mLstCtrl.SetColumnWidth(COL_PATH, rect.Width() - nLftW - 10);

}

//---------------------------------------------------------------------------------------------------------------------------
// �ı����ҳ�İ�ť��״̬
void CAXPPackerDlg::NtfUpdateBtnState()
{
	mBtn0Ctrl.EnableWindow(mCurPg > 1);			// ��ҳ
	mBtn1Ctrl.EnableWindow(mCurPg > 1);			// ��һҳ
	mBtn2Ctrl.EnableWindow(mCurPg < mAllPg);	// ��һҳ
	mBtn3Ctrl.EnableWindow(mCurPg < mAllPg);	// βҳ
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
	// TODO: �ڴ����ר�ô����/����û���

	//CDialog::OnOK();
}

//---------------------------------------------------------------------------------------------------------------------------
// ��AXP�ļ�
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
// �洢ѡ�е��ļ�
void CAXPPackerDlg::OnSaveThisFile()
{
	OPTION_LOCKED();
	mExpLst.clear();

	NtfUpdatePubInfo(_T("�������ɴ洢�б�, ..."));
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
		::MessageBox(GetSafeHwnd(), _T("û�пɴ洢���ļ��б�"), _T("��ʾ"), MB_OK|MB_ICONEXCLAMATION);
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
// ����Ŀ¼�ṹ����
void CAXPPackerDlg::OnSaveAll()
{
	OPTION_LOCKED();
	const AxpFVct& pLst = mpAxpArch->GetAxpFileList();
	if(pLst.empty())
	{
		NtfDefaultPubInfo();
		::MessageBox(GetSafeHwnd(), _T("û�пɴ洢���ļ�"), _T("��ʾ"), MB_OK|MB_ICONEXCLAMATION);
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

	// TODO: �ڴ˴������Ϣ����������
	NotifyOnSize();
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		NotifyModifyColName(COL_FILE, pCtrl, "�ļ�����:%d, Ŀ¼����:%d", nUsed, nSub);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	LPNM_LISTVIEW pItem = (LPNM_LISTVIEW)pNMHDR;
	if(pItem && (pItem->iItem >= 0))
	{
		CTreeItm* ptr = (CTreeItm*) mLstCtrl.GetItemData(pItem->iItem);
		if(ntf_click_item(ptr))	return;
	}
	NtfDefaultPubInfo();
	NtfUpdateInfo(_T("δѡ���ļ�"));
	NtfCloseImg();
}

//---------------------------------------------------------------------------------------------------------------------------
void CAXPPackerDlg::OnLvnKeydownItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
				NtfUpdateInfo(_T("δѡ���ļ�"));
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
			NtfUpdateInfo(_T("δѡ���ļ�"));
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
// ˫������ʾ����Ի���
void CAXPPackerDlg::OnNMDblclkItemList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
				NtfUpdatePubInfo(_T("���ڴ洢�ļ�: ") + file);
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
// �������ֲ���
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
// ������ʾ�õ����ͽṹ
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

		NtfUpdatePubInfo(_T("���ڲ��, ..."));
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
		NtfAddToTreeCtrl(nAll);	// ���ɿؼ���
	}
	else this->Invalidate();

}

//---------------------------------------------------------------------------------------------------------------------------
// ������
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
// �ֽڵ�
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
// �ӵ����Ϳؼ���
void CAXPPackerDlg::NtfAddToTreeCtrl(int nAll)
{
	//if(!mItmRoot.empty())
	{
		NtfUpdatePubInfo(_T("����������ʾ�б�, ..."));
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
// ���Խڵ�
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
// 0 -- ��һҳ | 1 -- ǰһҳ | 2 -- ��һҳ | 3 -- ���һҳ
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
// ������ҳ��
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
// ������ʾ�б�
void CAXPPackerDlg::NtfResetShowList(CTreeItm* pItm)
{
	mAllPg = 0;					// ��ҳ��
	mCurPg = 0;					// ��ǰ��ʾ����ҳ��
	mShowLst.clear();			// ��ʾ�б�
	if(pItm)
	{
		TrItmVct& pULst = pItm->mFileLst;
		mShowLst.assign(pULst.begin(), pULst.end());
		notify_cal_page(mCurPg, mAllPg, mShowLst.size(), GetMaxPageCount());
	}
}

//-----------------------------------------------------------------------------------------------------------
// ��䵽UI��, 0(��һҳ)/1(ǰһҳ)/2(��һҳ)/3(���һҳ)
void CAXPPackerDlg::NtfShowTreeItems(int nType)
{
	CListCtrl* pCtrl = GetListCtrl();
	pCtrl->SetRedraw(FALSE);
	pCtrl->DeleteAllItems();
	NtfDefaultPubInfo();
	NtfUpdateInfo(_T("δѡ���ļ�"));
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
// ��ʾ�ļ���Ϣ
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
			CString szDesc = "δ֪�ļ�����";
			CString szfs = get_size_desc(pAxp->dwSize);
			szInf.Format(_T("Code: %s\n��С: %s\n����: %s\n·��: %s"), pAxp->szAddr, szfs, szDesc, pAxp->szFile);
			return szInf;
		}
	}
	return _T("�������");
}

//-----------------------------------------------------------------------------------------------------------
// ����ѡ�����
bool CAXPPackerDlg::ntf_click_item(CTreeItm* ptr)
{
	if(ptr && ptr->mAxpItem)
	{
		NtfCloseImg();
		const AxpFile* pAxp = ptr->mAxpItem;
		NtfUpdatePubInfo(_T("���ڴ���: ") + pAxp->szFile);

		CString szInf;
		IFileHdlr* pHdlr = get_file_proc(pAxp->szFile);
		if(pHdlr)
		{
			pHdlr->file_proc(this, ptr);
			szInf = pHdlr->get_tips();
		}
		else szInf = get_def_tips(ptr);
		NtfUpdateInfoCtrl(szInf);
		NtfUpdatePubInfo(_T("��ѡ��: ") + pAxp->szFile);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------
// ����ָ�����ļ�
void CAXPPackerDlg::NtfExportFiles()
{
	if(mExpLst.empty()) return;
	g_blck = TRUE;
	// ����Ŀ¼
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
		NtfUpdatePubInfo(_T("���ڴ洢�ļ�: ") + file);
		mpAxpArch->SaveToFile(pAxp, file);
		NtfSetProgress((int)(fRate * i));
		if(IsThreadOver()) return;
	}
	mExpLst.clear();
	NtfEndProgress();
	NtfDefaultPubInfo();
}

//-----------------------------------------------------------------------------------------------------------
// ����Ŀ¼�ṹ����洢
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
		NtfUpdatePubInfo(_T("���ڴ洢�ļ�: ") + file);
		NotifyMakeDir(file);							// ����Ŀ¼
		mpAxpArch->SaveToFile(pAxp, file);
		NtfSetProgress((int)(fRate * i));
		if(IsThreadOver()) return;
	}
	mExpLst.clear();
	NtfEndProgress();
	NtfDefaultPubInfo();
}

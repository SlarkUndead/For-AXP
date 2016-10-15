
// AXPPackerDlg.h : 头文件
//

#pragma once
//#include "afxcmn.h"
//#include "afxwin.h"
#include "AXPPackerTree.h"

struct AxpFile;
class CAxpArchive;
class CMFCIUserObj;


enum 
{
	COL_FILE = 0,
	COL_PATH,
};



// CAXPPackerDlg 对话框
class CAXPPackerDlg : public CDialog
{
	BOOL			mbInit;
	CMenu			mpMenu;
	CStatic			mPubInfCtrl;			// 公共提示信息框
	CProgressCtrl	mProgCtrl;
	CTreeCtrl		mTreeCtrl;
	CListCtrl		mLstCtrl;

	CStatic			mPicCtrl;				// 显示图片
	CStatic			mFileCtrl;				// 显示文件信息

	// 显示列表里分页显示的
	CButton 		mBtn0Ctrl;				// 首页
	CButton 		mBtn1Ctrl;				// 上一页
	CButton 		mBtn2Ctrl;				// 下一页
	CButton 		mBtn3Ctrl;				// 尾页

	CString			mOpenFile;
	CString			mSelPath;				// 保存到此目录

	CMFCIUserObj*	mpImgObj;				// 显示图片
	CAxpArchive*	mpAxpArch;
	TrItmVct		mItmRoot;				// 跟目录
	TrItmVct		mExpLst;				// 存储列表

	
	// UI显示控制
	size_t			mAllPg;					// 总页数
	size_t			mCurPg;					// 当前显示到的页数
	TrItmVct		mShowLst;				// 显示列表

	void NtfReset();													// 重置
	void NotifyOnSize();												// 窗口大小改变
	void NtfAddTreeRoot(StrLst&, const AxpFile*, int& nAll);			// 逐层加入
	void NtfAddTreeItem(CTreeItm*, StrLst&, const AxpFile*, int& nAll);	// 字节点

	void NtfAddToTreeCtrl(int nAll);									// 加到树型控件上
	void NtfAddTreeCtrlItem(CTreeItm*, HTREEITEM, int&);				// 加自节点

	void NtfUpdateBtnState();											// 改变控制页的按钮的状态

	bool ntf_click_item(CTreeItm*);										// 处理选择的项

	void NtfCloseImg();
public:
	CMenu* GetAppMenu(){return &mpMenu;}
	void EnableAppMenuItem(UINT nItemID, UINT nEnable);					// 主菜单里对应项可用状态控制

	CString GetOpenFile()const{return mOpenFile;}
	CString GetExportPath()const{return mSelPath;}

	CAxpArchive* GetAxpArchive(){return mpAxpArch;}
	CProgressCtrl* GetProgCtrl(){return &mProgCtrl;}
	CStatic* GetPubInfoCtrl(){return &mPubInfCtrl;}
	CTreeCtrl* GetTreeCtrl(){return &mTreeCtrl;}
	CListCtrl* GetListCtrl(){return &mLstCtrl;}
	CMFCIUserObj* GetImgCtrl(){return mpImgObj;}


	TrItmVct& GetExportList(){return mExpLst;}	// 获取导出列表
	TrItmVct& GetTreeRoot(){return mItmRoot;}
	void ClearTreeRoot();

	void NtfGenTree();							// 构造显示用的树型结构
	CTreeItm* GetTreeRoot(CString szNm);		// 根据名字查找

	void NtfExportFiles();						// 导出指定的文件
	void NtfExportAllFiles();					// 保持目录结构变而存储

	
	// 显示控制
	void NtfClearShowList(){mShowLst.clear();}
	void NtfResetShowList(CTreeItm* pItm);		// 重置显示列表
	void NtfShowTreeItems(int nType);			// 填充到UI上, 0(第一页)/1(前一页)/2(下一页)/3(最后一页)
	size_t GetMaxPageCount()const;


	void NtfUpdateInfoCtrl(const char* szMsg, ...);	// 显示文件信息
	void NtfUpdateInfo(const CString& szInf);

// 构造
public:
	CAXPPackerDlg(CWnd* pParent = NULL);	// 标准构造函数
	virtual ~CAXPPackerDlg();

// 对话框数据
	enum { IDD = IDD_AXPPACKER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual void OnOK();
	virtual void OnCancel();


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnUnpackAXP();			// 读AXP文件
	afx_msg void OnSaveThisFile();		// 存储选中的文件
	afx_msg void OnSaveAll();			// 保持目录结构不变
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnTvnSelchangedItemTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickItemList(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedFirstBtn();
	afx_msg void OnBnClickedPreBtn();
	afx_msg void OnBnClickedNxtBtn();
	afx_msg void OnBnClickedLastBtn();
	afx_msg void OnNMDblclkItemList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownItemList(NMHDR *pNMHDR, LRESULT *pResult);
};

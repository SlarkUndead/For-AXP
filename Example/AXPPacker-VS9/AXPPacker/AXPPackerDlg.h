
// AXPPackerDlg.h : ͷ�ļ�
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



// CAXPPackerDlg �Ի���
class CAXPPackerDlg : public CDialog
{
	BOOL			mbInit;
	CMenu			mpMenu;
	CStatic			mPubInfCtrl;			// ������ʾ��Ϣ��
	CProgressCtrl	mProgCtrl;
	CTreeCtrl		mTreeCtrl;
	CListCtrl		mLstCtrl;

	CStatic			mPicCtrl;				// ��ʾͼƬ
	CStatic			mFileCtrl;				// ��ʾ�ļ���Ϣ

	// ��ʾ�б����ҳ��ʾ��
	CButton 		mBtn0Ctrl;				// ��ҳ
	CButton 		mBtn1Ctrl;				// ��һҳ
	CButton 		mBtn2Ctrl;				// ��һҳ
	CButton 		mBtn3Ctrl;				// βҳ

	CString			mOpenFile;
	CString			mSelPath;				// ���浽��Ŀ¼

	CMFCIUserObj*	mpImgObj;				// ��ʾͼƬ
	CAxpArchive*	mpAxpArch;
	TrItmVct		mItmRoot;				// ��Ŀ¼
	TrItmVct		mExpLst;				// �洢�б�

	
	// UI��ʾ����
	size_t			mAllPg;					// ��ҳ��
	size_t			mCurPg;					// ��ǰ��ʾ����ҳ��
	TrItmVct		mShowLst;				// ��ʾ�б�

	void NtfReset();													// ����
	void NotifyOnSize();												// ���ڴ�С�ı�
	void NtfAddTreeRoot(StrLst&, const AxpFile*, int& nAll);			// ������
	void NtfAddTreeItem(CTreeItm*, StrLst&, const AxpFile*, int& nAll);	// �ֽڵ�

	void NtfAddToTreeCtrl(int nAll);									// �ӵ����Ϳؼ���
	void NtfAddTreeCtrlItem(CTreeItm*, HTREEITEM, int&);				// ���Խڵ�

	void NtfUpdateBtnState();											// �ı����ҳ�İ�ť��״̬

	bool ntf_click_item(CTreeItm*);										// ����ѡ�����

	void NtfCloseImg();
public:
	CMenu* GetAppMenu(){return &mpMenu;}
	void EnableAppMenuItem(UINT nItemID, UINT nEnable);					// ���˵����Ӧ�����״̬����

	CString GetOpenFile()const{return mOpenFile;}
	CString GetExportPath()const{return mSelPath;}

	CAxpArchive* GetAxpArchive(){return mpAxpArch;}
	CProgressCtrl* GetProgCtrl(){return &mProgCtrl;}
	CStatic* GetPubInfoCtrl(){return &mPubInfCtrl;}
	CTreeCtrl* GetTreeCtrl(){return &mTreeCtrl;}
	CListCtrl* GetListCtrl(){return &mLstCtrl;}
	CMFCIUserObj* GetImgCtrl(){return mpImgObj;}


	TrItmVct& GetExportList(){return mExpLst;}	// ��ȡ�����б�
	TrItmVct& GetTreeRoot(){return mItmRoot;}
	void ClearTreeRoot();

	void NtfGenTree();							// ������ʾ�õ����ͽṹ
	CTreeItm* GetTreeRoot(CString szNm);		// �������ֲ���

	void NtfExportFiles();						// ����ָ�����ļ�
	void NtfExportAllFiles();					// ����Ŀ¼�ṹ����洢

	
	// ��ʾ����
	void NtfClearShowList(){mShowLst.clear();}
	void NtfResetShowList(CTreeItm* pItm);		// ������ʾ�б�
	void NtfShowTreeItems(int nType);			// ��䵽UI��, 0(��һҳ)/1(ǰһҳ)/2(��һҳ)/3(���һҳ)
	size_t GetMaxPageCount()const;


	void NtfUpdateInfoCtrl(const char* szMsg, ...);	// ��ʾ�ļ���Ϣ
	void NtfUpdateInfo(const CString& szInf);

// ����
public:
	CAXPPackerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CAXPPackerDlg();

// �Ի�������
	enum { IDD = IDD_AXPPACKER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��
	virtual void OnOK();
	virtual void OnCancel();


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnUnpackAXP();			// ��AXP�ļ�
	afx_msg void OnSaveThisFile();		// �洢ѡ�е��ļ�
	afx_msg void OnSaveAll();			// ����Ŀ¼�ṹ����
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

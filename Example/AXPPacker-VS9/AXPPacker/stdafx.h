
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��




#include <functional>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <deque>
#include <list>
#include <map>





#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


#define	USE_MFC			1	// 1: CString | 0: std::string
#if(USE_MFC)
	typedef CString				String;
#else
	typedef std::string			String;
#endif
typedef std::vector<String>		StrVct;
typedef std::list<String>		StrLst;


#define CLR_TXT		RGB(  0, 128, 255)			// ������������ɫ
#define CLR_RED		RGB(255,   0,   0)			// �����������ɫ
#define CLR_YELLOW  RGB(255,   0, 255)			// �����������ɫ
#define CLR_GREEN   RGB(  0, 128,   0)			// ǳ��
#define CLR_ORANGE  RGB(255, 128,  64)			// Orange

void UILogWriter(COLORREF txtColor, const char* szMsg, ...);			// ��Logд���ײ���Debug����ͬʱд���ļ���
void UILogMessage(COLORREF txtColor, const char* msg, bool out_time);

void split_string(const String& pzTxt, StrVct& pLst, const String& szfnd);
void split_string(const String& pzTxt, StrLst& pLst, const String& szfnd);

BOOL IsThreadOver();
CProgressCtrl* GetProgressCtrl();


void NtfUpdatePubInfo(CString);	// ������ʾ��Ϣ
void NtfDefaultPubInfo();		// 
void NtfStartProgress();		// ����������
void NtfSetProgress(int nPos);	// ���ò���[0, 100]
void NtfEndProgress();			// �رս�����

void NotifyModifyColName(int nCol, CListCtrl* pCtrl, const char* szMsg, ...);


bool NotifyMakeDir(const char* str);	// ����Ŀ¼
CString GetBrowseForFolder();				// ��ȡ�ļ���
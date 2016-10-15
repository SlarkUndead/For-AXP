
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持




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


#define CLR_TXT		RGB(  0, 128, 255)			// 基本的文字颜色
#define CLR_RED		RGB(255,   0,   0)			// 警告的文字颜色
#define CLR_YELLOW  RGB(255,   0, 255)			// 警告的文字颜色
#define CLR_GREEN   RGB(  0, 128,   0)			// 浅绿
#define CLR_ORANGE  RGB(255, 128,  64)			// Orange

void UILogWriter(COLORREF txtColor, const char* szMsg, ...);			// 把Log写到底部的Debug窗口同时写到文件里
void UILogMessage(COLORREF txtColor, const char* msg, bool out_time);

void split_string(const String& pzTxt, StrVct& pLst, const String& szfnd);
void split_string(const String& pzTxt, StrLst& pLst, const String& szfnd);

BOOL IsThreadOver();
CProgressCtrl* GetProgressCtrl();


void NtfUpdatePubInfo(CString);	// 公共提示信息
void NtfDefaultPubInfo();		// 
void NtfStartProgress();		// 启动进度条
void NtfSetProgress(int nPos);	// 设置步幅[0, 100]
void NtfEndProgress();			// 关闭进度条

void NotifyModifyColName(int nCol, CListCtrl* pCtrl, const char* szMsg, ...);


bool NotifyMakeDir(const char* str);	// 创建目录
CString GetBrowseForFolder();				// 获取文件夹
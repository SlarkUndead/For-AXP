
// AXPPacker.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

class CAXPPackerDlg;

// CAXPPackerApp:
// �йش����ʵ�֣������ AXPPacker.cpp
//

class CAXPPackerApp : public CWinAppEx
{
public:
	CAXPPackerApp();

	CAXPPackerDlg* GetAXPPackerDlg(){return (CAXPPackerDlg*)m_pMainWnd;}

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CAXPPackerApp theApp;
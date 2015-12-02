
// EVE_LOGIN_MFC.h : main header file for the PROJECT_NAME application
//
#define WM_ENDEDITUSERINFO WM_USER+1001
#define WM_SHOWUSERINFO WM_USER+1002
#define WM_ENDFILTER WM_USER+1003
#define WM_SHOWFILTER WM_USER+1004

#define SW_REFRESH -2
#define SW_CLOSE -1
#define SW_ADDFILTER 0
#define SW_EDITFILTER 1
#define SW_DELETEFILTER 2



#include "UserController.h"


#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CEVE_LOGIN_MFCApp:
// See EVE_LOGIN_MFC.cpp for the implementation of this class
//

class CEVE_LOGIN_MFCApp : public CWinApp
{
public:
	CEVE_LOGIN_MFCApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()

};

extern CEVE_LOGIN_MFCApp theApp;
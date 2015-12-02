
// EVE_LOGIN_MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EVE_LOGIN_MFC.h"
#include "EVE_LOGIN_MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CEVE_LOGIN_MFCDlg dialog



CEVE_LOGIN_MFCDlg::CEVE_LOGIN_MFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CEVE_LOGIN_MFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEVE_LOGIN_MFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, listMain);
	DDX_Control(pDX, IDC_EDIT1, strUserNum);
	DDX_Control(pDX, IDC_PROGRESS1, LogProgress);
	DDX_Control(pDX, IDC_EDIT2, LogProgressText);
	DDX_Control(pDX, IDC_LOCALS, cbLocalSetting);
}

BEGIN_MESSAGE_MAP(CEVE_LOGIN_MFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_AddUser, &CEVE_LOGIN_MFCDlg::OnBnClickedAdduser)
	ON_BN_CLICKED(IDC_EditUser, &CEVE_LOGIN_MFCDlg::OnBnClickedEdituser)
	ON_MESSAGE(WM_ENDEDITUSERINFO, &CEVE_LOGIN_MFCDlg::EndEditUserInfo)
	ON_MESSAGE(WM_ENDFILTER,&CEVE_LOGIN_MFCDlg::EndFilter)
	ON_BN_CLICKED(IDC_DeleteUser, &CEVE_LOGIN_MFCDlg::OnBnClickedDeleteuser)
	ON_BN_CLICKED(IDC_Filter, &CEVE_LOGIN_MFCDlg::OnBnClickedFilter)
	ON_LBN_DBLCLK(IDC_LIST1, &CEVE_LOGIN_MFCDlg::OnLbnDblclkList1)
	ON_COMMAND(ID_32775, &CEVE_LOGIN_MFCDlg::On32775)
	ON_COMMAND(ID_32773, &CEVE_LOGIN_MFCDlg::On32773)
	ON_BN_CLICKED(IDC_LOCALS, &CEVE_LOGIN_MFCDlg::OnBnClickedLocals)
	ON_BN_CLICKED(IDC_MultipleLogin, &CEVE_LOGIN_MFCDlg::OnBnClickedMultiplelogin)
END_MESSAGE_MAP()


// CEVE_LOGIN_MFCDlg message handlers

BOOL CEVE_LOGIN_MFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	//initialize usercontroller

	controller = new UserController();


	//initialize subwindows

	dlgUserInfo = new UserInfoDlg(this);
	dlgUserInfo->Create(MAKEINTRESOURCE(IDD_OLE_UserInfo), this);
	dlgUserInfo->SendMessage(WM_CLOSE);

	dlgFilterInfo = new FilterInfoDlg(this);
	dlgFilterInfo->Create(MAKEINTRESOURCE(IDD_OLE_Filter), this);
	dlgFilterInfo->SendMessage(WM_CLOSE);

	nWindowState = 0;

	tools = new CalcTools();

	//window refresh
	SetTimer(MAIN_TIMER_ID, MAIN_TIMER, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEVE_LOGIN_MFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CEVE_LOGIN_MFCDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CEVE_LOGIN_MFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





void CEVE_LOGIN_MFCDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch(nIDEvent)
	{
	case (MAIN_TIMER_ID) :
	{
		break;
	}
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CEVE_LOGIN_MFCDlg::RefreshList()
{
	INT nUserNum = controller->GetUserNum();
	WCHAR wstrBuffer[PARA_LENGTH];
	wsprintf(wstrBuffer, TEXT("%d"), nUserNum);
	strUserNum.SetWindowTextW(wstrBuffer);

	if (nUserNum == 1)
	{
		nUserNum = 1;
	}

	nUserInListNum = 0;
	listMain.ResetContent();
	for (INT i1 = 0; i1 < nUserNum; i1++)
	{
		UserData userNew;
		if (controller->FilterCheck(i1))
		{
			controller->ShowUserInfo(i1, &userNew);
			nUserInList[nUserInListNum++] = i1;
			ProduceListText(&userNew, bFailed[i1], wstrBuffer);
			listMain.AddString(wstrBuffer);
		}
	}

	cbLocalSetting.SetCheck(controller->UseLocalSetting() ? BST_CHECKED : BST_UNCHECKED);
	controller->UpdateData();
}


void CEVE_LOGIN_MFCDlg::OnBnClickedAdduser()
{
	// TODO: Add your control notification handler code here
	if (nWindowState == 0)
	{
		if (controller->AvailAddUser())
		{
			nWindowState = 1;
			ShowInfoWindow(-1);
			PostStateText(TEXT(TEXT_ADDUSER));
		}
		else
		{
			MessageBoxW(TEXT("账号数量已到达上限"), TEXT("错误"));
		}
	}
}


void CEVE_LOGIN_MFCDlg::OnBnClickedEdituser()
{
	// TODO: Add your control notification handler code here

	if (nWindowState == 0)
	{
		INT nIndex;
		nIndex = listMain.GetCurSel();
		if (nIndex!=LB_ERR)
		{
			nWindowState = 2;
			ShowInfoWindow(nIndex);
			PostStateText(TEXT(TEXT_EDITUSER));
		}
		else
		{
			MessageBoxW(TEXT("请选中一个账号"), TEXT("错误"));
		}
	}
}



void CEVE_LOGIN_MFCDlg::OnBnClickedDeleteuser()
{
	// TODO: Add your control notification handler code here

	if (nWindowState == 0)
	{
		INT nIndex;
		nIndex = listMain.GetCurSel();
		if (nIndex != LB_ERR)
		{
			controller->DeleteUser(nUserInList[nIndex]);
			RefreshList();
			PostStateText(TEXT(TEXT_DELETEUSER));
		}
		else
		{
			MessageBoxW(TEXT("请选中一个账号"), TEXT("错误"));
		}
	}
}



void CEVE_LOGIN_MFCDlg::OnBnClickedFilter()
{
	// TODO: Add your control notification handler code here

	if (nWindowState == 0)
	{
		nWindowState = 3;
		ShowFilterWindow();
	}
}

void CEVE_LOGIN_MFCDlg::ShowFilterWindow()
{
	InfoFilterParam *infoCurrent = (InfoFilterParam*)malloc(sizeof(InfoFilterParam));
	memset(infoCurrent, 0, sizeof(InfoFilterParam));

	infoCurrent->nFilterNum = controller->GetFilterNum();

	controller->GetFilterState(infoCurrent->nFilterState);

	for (INT i1 = 0; i1 < infoCurrent->nFilterNum; i1++)
	{
		controller->ShowFilterLabel(i1, infoCurrent->wstrFilterName[i1]);
	}

	dlgFilterInfo->ShowWindow(SW_SHOW);
	dlgFilterInfo->SendMessage(WM_SHOWFILTER, (WPARAM)infoCurrent, NULL);
	
	delete(infoCurrent);

}

void CEVE_LOGIN_MFCDlg::ShowInfoWindow(const INT nMode)
{
	InfoUserParam *infoCurrent = (InfoUserParam*)malloc(sizeof(InfoUserParam));
	memset(infoCurrent, 0, sizeof(InfoUserParam));

	infoCurrent->nMode = nMode;
	
	if (nMode != -1)
	{
		controller->ShowUserInfo(nMode, &(infoCurrent->userCurrent));
		controller->GetUserFilterState(nMode, infoCurrent->bUserState);
	}


	for (INT i1 = 0; i1 < controller->GetFilterNum(); i1++)
	{
		controller->ShowFilterLabel(i1, infoCurrent->wstrFilterName[i1]);
	}
	infoCurrent->nFilterNum = controller->GetFilterNum();
	dlgUserInfo->ShowWindow(SW_SHOW);
	dlgUserInfo->SendMessage(WM_SHOWUSERINFO, (WPARAM)infoCurrent, NULL);

	delete(infoCurrent);

}

LRESULT CEVE_LOGIN_MFCDlg::EndFilter(WPARAM wParam, LPARAM lParam)
{
	InfoFilterParam *infoCurrent = (InfoFilterParam*)wParam;
	if (infoCurrent != NULL)
	{
		switch (infoCurrent->nMode)
		{
		case (SW_ADDFILTER) :
		{
			controller->AddFilterLabel(infoCurrent->wstrNewFilterName);
			break;
		}
		case(SW_EDITFILTER) :
		{
			controller->EditFilterLabel(infoCurrent->nFilterIndex, infoCurrent->wstrNewFilterName);
			break;
		}
		case(SW_DELETEFILTER) :
		{
			controller->DeleteFilterLabel(infoCurrent->nFilterIndex);
			break;
		}
		case(SW_CLOSE) :
		{
			nWindowState = 0;
		}
		default:
		{
		}
		}
		controller->SetFilterState(infoCurrent->nFilterState);
	}
	RefreshList();
	PostStateText(TEXT(TEXT_EDITFILTER));
	return(1);
}


LRESULT CEVE_LOGIN_MFCDlg::EndEditUserInfo(WPARAM wParam, LPARAM lParam)
{
	InfoUserParam *infoCurrent = (InfoUserParam*)wParam;
	if (infoCurrent != NULL)
	{
		if (infoCurrent->nMode == -1)
		{
			// Add new user

			INT nIndex = controller->AddUser(&(infoCurrent->userCurrent));
			controller->SetUserFilter(nIndex, infoCurrent->bUserState);
		}
		else
		{
			controller->EditUser(infoCurrent->nMode, &(infoCurrent->userCurrent));
			controller->SetUserFilter(infoCurrent->nMode, infoCurrent->bUserState);
			//edit user
		}
	}
	nWindowState = 0;
	RefreshList();
	return(1);
}



void CEVE_LOGIN_MFCDlg::OnLbnDblclkList1()
{
	// TODO: Add your control notification handler code here

	if (!controller->AvailLogon())
	{
		MessageBox(TEXT("请先设置EVE启动路径(点击文件-设置路径-选择bin文件夹下的exefile.exe"), TEXT("错误"));
		return;
	}
	INT nIndex = listMain.GetCurSel();
	if (nIndex != LB_ERR)
	{
		INT nExecResult = controller->LogonUserCN(nUserInList[nIndex]);
		if (nExecResult > 32)
		{
			PostStateText(TEXT("登录成功,正在启动游戏..."));
		}
		else
		{
			if (nExecResult == 0)
			{
				PostStateText(TEXT("登录失败,账号密码错误或者网络故障。"));
			}
			else
			{
				PostStateText(TEXT("游戏启动失败。"));
			}	
		}
	}
}


void CEVE_LOGIN_MFCDlg::On32775()
{
	// TODO: Add your command handler code here

	controller->~UserController();
	this->SendMessage(WM_CLOSE, NULL, NULL);
}


void CEVE_LOGIN_MFCDlg::On32773()
{
	// TODO: Add your command handler code here

	dlgEveExePath = new CFileDialog(TRUE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		TEXT("EVE Execute File|exefile.exe||"),
		this,
		0,
		TRUE);
	OPENFILENAME ofnEveExe = dlgEveExePath->GetOFN();
	if (dlgEveExePath->DoModal() == IDOK)
	{
		controller->SetEVEPath(ofnEveExe.lpstrFile);
	}

	PostStateText(TEXT(TEXT_CHANGEEVEURL));
}

void CEVE_LOGIN_MFCDlg::PostStateText(LPCWSTR wstrStateText)
{
	LogProgressText.SetWindowTextW(wstrStateText);
}

void CEVE_LOGIN_MFCDlg::ProduceListText(UserData *userCurrent,const BOOL bFailed, LPWSTR strOut)
{
	memset((void*)strOut, 0, PARA_LENGTH);
	INT nSkillDay = tools->RemainDays(&(userCurrent->timeSkill));
	INT nTicketDay = tools->RemainDays(&(userCurrent->timeTicket));
	wsprintf(strOut,
		TEXT("%-20s  %s %s  %s"),
		userCurrent->wstrCharacter,
		nSkillDay >= 2 ? TEXT("\0") : TEXT("技能到期警示"),
		nTicketDay >= 7 ? TEXT("\0") : TEXT("月卡到期警示"),
		bFailed ? TEXT("(批量登录失败)") : TEXT(""));
}



void CEVE_LOGIN_MFCDlg::OnBnClickedLocals()
{
	// TODO: Add your control notification handler code here

	if (controller->UseLocalSetting())
	{
		controller->SetLocalSetting(FALSE);
		cbLocalSetting.SetCheck(BST_UNCHECKED);
	}
	else
	{
		controller->SetLocalSetting(TRUE);
		cbLocalSetting.SetCheck(BST_CHECKED);
	}
	controller->UpdateSetting();
}


void CEVE_LOGIN_MFCDlg::OnBnClickedMultiplelogin()
{
	// TODO: Add your control notification handler code here
	if (!controller->AvailLogon())
	{
		MessageBox(TEXT("请先设置EVE启动路径(点击文件-设置路径-选择bin文件夹下的exefile.exe"), TEXT("错误"));
		return;
	}

	memset(bFailed, 0, sizeof(bFailed));
	LogProgress.SetRange(0, 100);
	LogProgress.SetPos(0);
	LogProgress.SetStep(ceil(100 / nUserInListNum));
	for (INT i1 = 0; i1 < nUserInListNum; i1++)
	{
		WCHAR wstrBuffer[PARA_LENGTH];
		wsprintf(wstrBuffer, TEXT("账号登录中%d/%d"), i1 + 1, nUserInListNum);
		PostStateText(wstrBuffer);

		if (controller->LogonUserCN(nUserInList[i1]) <= 32)
		{
			bFailed[nUserInList[i1]] = TRUE;
		}

		LogProgress.StepIt();
	}

}

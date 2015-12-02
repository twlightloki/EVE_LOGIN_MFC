// UserInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EVE_LOGIN_MFC.h"
#include "UserInfoDlg.h"
#include "afxdialogex.h"


// UserInfoDlg dialog

IMPLEMENT_DYNAMIC(UserInfoDlg, CDialogEx)

UserInfoDlg::UserInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(UserInfoDlg::IDD, pParent)
{
	tools = new CalcTools();
}

UserInfoDlg::~UserInfoDlg()
{
}

void UserInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Character, strCharacter);
	DDX_Control(pDX, IDC_UserName, strUserName);
	DDX_Control(pDX, IDC_PassWord, strPassWord);
	DDX_Control(pDX, IDC_Filter, FilterList);
	DDX_Control(pDX, IDC_SKILLDAY, SkillDay);
	DDX_Control(pDX, IDC_TICKETDAY, TicketDay);
}


BEGIN_MESSAGE_MAP(UserInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_Cancel, &UserInfoDlg::OnBnClickedCancel)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_OK, &UserInfoDlg::OnBnClickedOk)
	ON_MESSAGE(WM_SHOWUSERINFO, &UserInfoDlg::OnShowUserInfo)
END_MESSAGE_MAP()


// UserInfoDlg message handlers


void UserInfoDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	this->SendMessage(WM_CLOSE);
}


int UserInfoDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}

void UserInfoDlg::OnOK()
{
	OnBnClickedOk();
}


void UserInfoDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	this->GetParent()->SendMessage(WM_ENDEDITUSERINFO, NULL, NULL);
	CDialogEx::OnClose();
}


void UserInfoDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	InfoUserParam *infoCurrent = (InfoUserParam*)malloc(sizeof(InfoUserParam));
	memset(infoCurrent, 0, sizeof(InfoUserParam));

	if (CollectUserInfo(infoCurrent))
	{
		this->GetParent()->SendMessage(WM_ENDEDITUSERINFO, (WPARAM)infoCurrent, NULL);
		this->SendMessage(WM_CLOSE);
		delete(infoCurrent);
	}
	else
	{
		MessageBoxW(TEXT("输入信息有误,请检查! (每一项长度至少为2)"), TEXT("错误"));
	}

}

BOOL UserInfoDlg::CollectUserInfo(InfoUserParam *infoCurrent)
{
	infoCurrent->nMode = nCurrentMode;

	strCharacter.GetLine(0, infoCurrent->userCurrent.wstrCharacter, sizeof(infoCurrent->userCurrent.wstrCharacter));
	infoCurrent->userCurrent.nCharaterLen = wcslen(infoCurrent->userCurrent.wstrCharacter);

	strUserName.GetLine(0, infoCurrent->userCurrent.wstrUserName, sizeof(infoCurrent->userCurrent.wstrUserName));
	infoCurrent->userCurrent.nUserNameLen = wcslen(infoCurrent->userCurrent.wstrUserName);

	strPassWord.GetLine(0, infoCurrent->userCurrent.wstrPassWord, sizeof(infoCurrent->userCurrent.wstrPassWord));
	infoCurrent->userCurrent.nPassWordLen = wcslen(infoCurrent->userCurrent.wstrPassWord);

	if (infoCurrent->userCurrent.nCharaterLen <= 1 || infoCurrent->userCurrent.nUserNameLen <= 1 || infoCurrent->userCurrent.nPassWordLen <= 1)
	{
		return(FALSE);
	}


	//time convert here!!!

	INT nTemp;
	WCHAR wstrTemp[PARA_LENGTH];
	memset(wstrTemp, 0, sizeof(wstrTemp));
	SkillDay.GetLine(0, wstrTemp, sizeof(wstrTemp));
	tools->IncreasedDays(_tstoi(wstrTemp), &(infoCurrent->userCurrent.timeSkill));

	memset(wstrTemp, 0, sizeof(wstrTemp));
	TicketDay.GetLine(0, wstrTemp, sizeof(wstrTemp));
	tools->IncreasedDays(_tstoi(wstrTemp), &(infoCurrent->userCurrent.timeTicket));



	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		infoCurrent->bUserState[i1] = FilterList.GetSel(i1);
	}

	return(TRUE);
}

LRESULT UserInfoDlg::OnShowUserInfo(WPARAM wParam, LPARAM lParam)
{
	InfoUserParam *infoCurrent = (InfoUserParam*)wParam;
	nCurrentMode = infoCurrent->nMode;
	if (nCurrentMode >= 0)
	{
		strCharacter.SetWindowTextW(infoCurrent->userCurrent.wstrCharacter);
		strUserName.SetWindowTextW(infoCurrent->userCurrent.wstrUserName);
		strPassWord.SetWindowTextW(infoCurrent->userCurrent.wstrPassWord);

		WCHAR wstrBuffer[PARA_LENGTH];

		wsprintf(wstrBuffer, TEXT("%d"), tools->RemainDays(&(infoCurrent->userCurrent.timeSkill)));
		SkillDay.SetWindowTextW(wstrBuffer);

		wsprintf(wstrBuffer, TEXT("%d"), tools->RemainDays(&(infoCurrent->userCurrent.timeTicket)));
		TicketDay.SetWindowTextW(wstrBuffer);
	}
	else
	{
		SYSTEMTIME timeCurrent;
		GetSystemTime(&timeCurrent);
		
		strCharacter.SetWindowTextW(TEXT("\0"));
		strUserName.SetWindowTextW(TEXT("\0"));
		strPassWord.SetWindowTextW(TEXT("\0"));
		SkillDay.SetWindowTextW(TEXT("0"));
		TicketDay.SetWindowTextW(TEXT("0"));

	}





	nFilterNum = infoCurrent->nFilterNum;
	FilterList.ResetContent();
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		FilterList.AddString(infoCurrent->wstrFilterName[i1]);
		if (infoCurrent->bUserState[i1])
		{
			FilterList.SetSel(i1);
		}
	}

	return(1);
}
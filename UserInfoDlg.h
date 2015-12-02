#pragma once

#include "UserController.h"
#include "Tools.h"
#include "afxdtctl.h"
#include "afxwin.h"

// UserInfoDlg dialog

typedef struct InfoUserParam
{
	INT nMode, nFilterNum;
	WCHAR wstrFilterName[MAX_FILTER][PARA_LENGTH];
	UserData userCurrent;
	BOOL bUserState[MAX_FILTER];
};

class UserInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(UserInfoDlg)

public:
	UserInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~UserInfoDlg();

// Dialog Data
	enum { IDD = IDD_OLE_UserInfo };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	CEdit strCharacter;
	CEdit strUserName;
	CEdit strPassWord;
	afx_msg void OnBnClickedOk();

private:
	INT nCurrentMode;
	INT nFilterNum;

	CalcTools *tools;

	BOOL CollectUserInfo(InfoUserParam *infoCurrent);

	void OnOK();
	afx_msg LRESULT OnShowUserInfo(WPARAM wParam, LPARAM lParam);
	CListBox FilterList;
public:
	CEdit SkillDay;
	CEdit TicketDay;
};

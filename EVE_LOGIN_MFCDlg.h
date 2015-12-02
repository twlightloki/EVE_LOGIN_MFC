
// EVE_LOGIN_MFCDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "UserInfoDlg.h"
#include "FilterInfoDlg.h"
#include "Tools.h"
#include "afxcmn.h"



// CEVE_LOGIN_MFCDlg dialog

#define MAIN_TIMER_ID 0
#define MAIN_TIMER 2000

#define TEXT_CHANGEEVEURL "修改EVE主程序路径"
#define TEXT_ADDUSER "添加了一个账号"
#define TEXT_EDITUSER "修改了一个账号"
#define TEXT_DELETEUSER "删除了一个账号"
#define TEXT_EDITFILTER "更新了筛选器"


class CEVE_LOGIN_MFCDlg : public CDialogEx
{
// Construction
public:
	CEVE_LOGIN_MFCDlg(CWnd* pParent = NULL);	// standard constructor


// Dialog Data
	enum { IDD = IDD_EVE_LOGIN_MFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:
	CListBox listMain;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	UserController *controller;

	UserInfoDlg *dlgUserInfo;

	FilterInfoDlg *dlgFilterInfo;

	CFileDialog *dlgEveExePath;

	CalcTools *tools;

	INT nUserInList[MAX_USER];

	BOOL bFailed[MAX_USER];

	INT nUserInListNum;

	INT nWindowState;

	void RefreshList();

	void ShowInfoWindow(const INT nMode);

	void ShowFilterWindow();
	
	afx_msg LRESULT EndEditUserInfo(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT EndFilter(WPARAM wParam, LPARAM lParam);

	

public:
	afx_msg void OnBnClickedAdduser();

	afx_msg void OnBnClickedEdituser();

	afx_msg void OnBnClickedDeleteuser();

//	afx_msg void OnLbnDblclkList1();

	CEdit strUserNum;
	afx_msg void OnBnClickedFilter();
private:
	CProgressCtrl LogProgress;


	void PostStateText(LPCWSTR wstrStateText);

	void ProduceListText(UserData *userCurrent,const BOOL bFailed, LPWSTR strOut);


public:
	CEdit LogProgressText;

	afx_msg void OnLbnDblclkList1();
	afx_msg void On32775();
	afx_msg void On32773();
	afx_msg void OnBnClickedLocals();
	CButton cbLocalSetting;
	afx_msg void OnBnClickedMultiplelogin();
};

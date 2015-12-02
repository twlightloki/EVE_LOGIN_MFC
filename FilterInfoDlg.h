#pragma once

#include "UserController.h"
#include "afxwin.h"


typedef struct InfoFilterParam
{
	INT nFilterState[MAX_FILTER], nFilterNum, nSkillTimeFilter, nTicketTimeFilter, nMode, nFilterIndex;
	WCHAR wstrFilterName[MAX_FILTER][PARA_LENGTH];
	WCHAR wstrNewFilterName[PARA_LENGTH];
};

// FilterInfoDlg dialog

class FilterInfoDlg : public CDialogEx
{
	DECLARE_DYNAMIC(FilterInfoDlg)

public:
	FilterInfoDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~FilterInfoDlg();

// Dialog Data
	enum { IDD = IDD_OLE_Filter };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()


private:

	INT nFilterState[MAX_FILTER], nFilterNum;
	WCHAR wstrFilterName[MAX_FILTER][PARA_LENGTH];
	INT nFilterWindowState;

	BOOL bDataAvail;

	void RefreshFilter(const INT nMode, const INT nFilterIndex, const LPCWSTR wstrNewFilterName);

	void ChangeFilterState(const INT nFilterIndex, const INT nFilterState);

	void RefreshList();

	void RefreshCheckBox();

public:
	afx_msg void OnBnClickedAddfilter();
	afx_msg void OnBnClickedClose();
	afx_msg LRESULT OnShowFilterWindow(WPARAM wParam, LPARAM lParam);

private:
	CEdit NewFilterName;
	CListBox FilterList;
public:
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedAvail();
	CButton cbAvail;
	CButton cbNone;
	CButton cbDisable;
	afx_msg void OnBnClickedNone();
	afx_msg void OnBnClickedDisable();
	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnBnClickedStopall();
	afx_msg void OnClose();
};

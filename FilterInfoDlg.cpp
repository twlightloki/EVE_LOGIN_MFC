// FilterInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EVE_LOGIN_MFC.h"
#include "FilterInfoDlg.h"
#include "afxdialogex.h"


// FilterInfoDlg dialog

IMPLEMENT_DYNAMIC(FilterInfoDlg, CDialogEx)

FilterInfoDlg::FilterInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(FilterInfoDlg::IDD, pParent)
{
	bDataAvail = FALSE;
}

FilterInfoDlg::~FilterInfoDlg()
{
}

void FilterInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FilterLabel, NewFilterName);
	DDX_Control(pDX, IDC_LIST1, FilterList);
	DDX_Control(pDX, IDC_Avail, cbAvail);
	DDX_Control(pDX, IDC_None, cbNone);
	DDX_Control(pDX, IDC_Disable, cbDisable);
}


BEGIN_MESSAGE_MAP(FilterInfoDlg, CDialogEx)
	ON_BN_CLICKED(IDC_AddFilter, &FilterInfoDlg::OnBnClickedAddfilter)
	ON_BN_CLICKED(IDC_Close, &FilterInfoDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_Delete, &FilterInfoDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_Avail, &FilterInfoDlg::OnBnClickedAvail)
	ON_BN_CLICKED(IDC_None, &FilterInfoDlg::OnBnClickedNone)
	ON_BN_CLICKED(IDC_Disable, &FilterInfoDlg::OnBnClickedDisable)
	ON_MESSAGE(WM_SHOWFILTER, &FilterInfoDlg::OnShowFilterWindow)
	ON_LBN_SELCHANGE(IDC_LIST1, &FilterInfoDlg::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_StopAll, &FilterInfoDlg::OnBnClickedStopall)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// FilterInfoDlg message handlers


void FilterInfoDlg::OnBnClickedAddfilter()
{
	// TODO: Add your control notification handler code here

	WCHAR wstrNewFilterName[PARA_LENGTH];
	NewFilterName.GetWindowTextW(wstrNewFilterName, PARA_LENGTH);
	if (wcslen(wstrNewFilterName) > 1)
	{
		INT nListIndex = FilterList.GetCurSel();
		if (nListIndex == LB_ERR)
		{
			RefreshFilter(0,0,wstrNewFilterName);
		}
		else
		{
			RefreshFilter(1, nListIndex, wstrNewFilterName);
		}
	}
	else
	{
		MessageBoxW(TEXT("输入信息有误,请检查! (长度至少为2)"), TEXT("错误"));
	}

}


void FilterInfoDlg::OnBnClickedDelete()
{
	// TODO: Add your control notification handler code here

	INT nListIndex = FilterList.GetCurSel();
	if (nListIndex != LB_ERR)
	{
		RefreshFilter(2, nListIndex, NULL);
	}
	else
	{
		MessageBoxW(TEXT("请选择一个标签!"), TEXT("错误"));
	}

}




void FilterInfoDlg::OnBnClickedClose()
{
	// TODO: Add your control notification handler code here

	this->SendMessage(WM_CLOSE, NULL, NULL);

}

void FilterInfoDlg::RefreshFilter(const INT nMode, const INT nFilterIndex, const LPCWSTR wstrNewFilterName)
{
	InfoFilterParam *infoCurrent = (InfoFilterParam*)malloc(sizeof(InfoFilterParam));
	memset(infoCurrent, 0, sizeof(InfoFilterParam));

	infoCurrent->nMode = nMode;

	infoCurrent->nFilterIndex = nFilterIndex;


	switch (nMode)
	{
	case (SW_ADDFILTER) :
	{
		memcpy(infoCurrent->wstrNewFilterName, wstrNewFilterName, sizeof(WCHAR) * PARA_LENGTH);
		memcpy(wstrFilterName[nFilterNum], wstrNewFilterName, sizeof(WCHAR) * PARA_LENGTH);
		nFilterState[nFilterNum] = -1;
		nFilterNum++;
		break;
	}
	case(SW_EDITFILTER) :
	{
		memcpy(infoCurrent->wstrNewFilterName, wstrNewFilterName, sizeof(WCHAR) * PARA_LENGTH);
		memcpy(wstrFilterName[nFilterIndex], wstrNewFilterName, sizeof(WCHAR) * PARA_LENGTH);
		break;
	}
	case(SW_DELETEFILTER) :
	{
		for (INT i1 = nFilterIndex; i1 < nFilterNum; i1++)
		{
			memcpy(wstrFilterName[i1], wstrFilterName[i1 + 1], sizeof(WCHAR) * PARA_LENGTH);
			memcpy(&nFilterState[i1], &nFilterState[i1 + 1], sizeof(INT));
		}
		nFilterNum--;
		break;
	}
	default:
	{
	}
	}

	memcpy(infoCurrent->nFilterState, nFilterState, sizeof(nFilterState));

	this->GetParent()->SendMessage(WM_ENDFILTER, (WPARAM)infoCurrent, NULL);

	delete(infoCurrent);

	RefreshList();
}

void FilterInfoDlg::RefreshList()
{
	FilterList.ResetContent();
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		FilterList.AddString(wstrFilterName[i1]);
	}
	NewFilterName.SetWindowTextW(TEXT("\0"));
	RefreshCheckBox();
}

void FilterInfoDlg::RefreshCheckBox()
{
	cbAvail.SetCheck(BST_UNCHECKED);
	cbNone.SetCheck(BST_UNCHECKED);
	cbDisable.SetCheck(BST_UNCHECKED);
	INT nFilterIndex = FilterList.GetCurSel();
	if (nFilterIndex != LB_ERR)
	{
		switch (nFilterState[nFilterIndex])
		{
		case (-1) :
		{
			cbNone.SetCheck(BST_CHECKED);
			break;
		}
		case(0) :
		{
			cbDisable.SetCheck(BST_CHECKED);
			break;
		}
		case(1) :
		{
			cbAvail.SetCheck(BST_CHECKED);
			break;
		}
		default:
		{

		}
		}
	}
}



void FilterInfoDlg::OnBnClickedAvail()
{
	// TODO: Add your control notification handler code here
	if (nFilterWindowState == 0)
	{
		nFilterWindowState = 1;
		ChangeFilterState(FilterList.GetCurSel(), 1);
		nFilterWindowState = 0;
	}
}


void FilterInfoDlg::OnBnClickedNone()
{
	// TODO: Add your control notification handler code here
	if (nFilterWindowState == 0)
	{
		nFilterWindowState = 1;
		ChangeFilterState(FilterList.GetCurSel(), -1);
		nFilterWindowState = 0;
	}
}


void FilterInfoDlg::OnBnClickedDisable()
{
	// TODO: Add your control notification handler code here
	if (nFilterWindowState == 0)
	{
		nFilterWindowState = 1;
		ChangeFilterState(FilterList.GetCurSel(), 0);
		nFilterWindowState = 0;
	}
}

void FilterInfoDlg::ChangeFilterState(const INT nFilterIndex, const INT nFilterState)
{
	if (nFilterIndex != LB_ERR)
	{
		this->nFilterState[nFilterIndex] = nFilterState;
		RefreshFilter(SW_REFRESH, 0, NULL);
	}
	else
	{
		MessageBoxW(TEXT("请选择一个标签!"), TEXT("错误"));
	}
	RefreshCheckBox();
}

LRESULT FilterInfoDlg::OnShowFilterWindow(WPARAM wParam, LPARAM lParam)
{
	InfoFilterParam *infoCurrent = (InfoFilterParam*)wParam;
	memcpy(nFilterState, infoCurrent->nFilterState, sizeof(nFilterState));
	memcpy(wstrFilterName, infoCurrent->wstrFilterName, sizeof(wstrFilterName));
	nFilterNum = infoCurrent->nFilterNum;
	nFilterWindowState = 0;
	bDataAvail = TRUE;
	RefreshList();
	return(1);
}

void FilterInfoDlg::OnLbnSelchangeList1()
{
	// TODO: Add your control notification handler code here

	INT nIndex = FilterList.GetCurSel();
	if (nIndex != LB_ERR)
	{
		NewFilterName.SetWindowTextW(wstrFilterName[nIndex]);
		RefreshCheckBox();
	}
}


void FilterInfoDlg::OnBnClickedStopall()
{
	// TODO: Add your control notification handler code here

	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		nFilterState[i1] = -1;
	}
	RefreshFilter(SW_REFRESH, 0, NULL);
}


void FilterInfoDlg::OnClose()
{
	// TODO: Add your message handlercode here and/or call default


	if (bDataAvail)
	{
		RefreshFilter(SW_CLOSE, 0, NULL);
	}
	CDialogEx::OnClose();
}

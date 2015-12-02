#include "stdafx.h"
#include "UserController.h"

UserController::UserController()
{
	nUserNum = 0;
	nFilterNum = 0;
	bLocalSetting = FALSE;
	memset(userCurrent, 0, sizeof(userCurrent));
	memset(fState, 0, sizeof(fState));
	ReadSetting();
	ReadData();
}

UserController::~UserController()
{
	UpdateSetting();
	UpdateData();
}

BOOL UserController::AddUser(const UserData *userNew)
{
	if (userNew->nUserNameLen <= 0 || userNew->nPassWordLen <= 0)
	{
		return(-1);
	}
	memcpy(&userCurrent[nUserNum], userNew, sizeof(UserData));
	return(nUserNum++);
}

BOOL UserController::EditUser(const INT nIndex, const UserData *userNew)
{
	if (nIndex >= nUserNum || userNew->nUserNameLen < 0 || userNew->nPassWordLen < 0)
	{
		return(FALSE);
	}
	memcpy(&userCurrent[nIndex], userNew, sizeof(UserData));
	return(TRUE);
}

BOOL UserController::DeleteUser(const INT nIndex)
{
	if (nIndex >= nUserNum)
	{
		return(FALSE);
	}
	for (INT i1 = nIndex; i1 < nUserNum - 1; i1++)
	{
		memcpy(&userCurrent[i1], &userCurrent[i1 + 1], sizeof(UserData));
	}

	for (INT i1 = 0; i1 < nFilterNum; i1++)
		for (INT i2 = nIndex; i2 < nUserNum; i2++)
			fState[i1].bUserState[i2] = fState[i1].bUserState[i2 + 1];

	nUserNum--;


	return(TRUE);
}

BOOL UserController::ShowUserInfo(const INT nIndex, const UserData *userNew)
{
	if (nIndex >= nUserNum)
	{
		return(FALSE);
	}
	memcpy((void*)userNew, &userCurrent[nIndex], sizeof(UserData));
	return(TRUE);;
}

INT UserController::GetUserNum()
{
	return(nUserNum);
}

INT UserController::GetFilterNum()
{
	return(nFilterNum);
}

BOOL UserController::AvailAddUser()
{
	return(nUserNum < MAX_USER);
}

BOOL UserController::AvailAddFilter()
{
	return(nFilterNum < MAX_FILTER);
}

BOOL UserController::AddFilterLabel(LPCWSTR wstrFilterLabel)
{
	memcpy(fState[nFilterNum].wstrLabelName, wstrFilterLabel, sizeof(WCHAR) * PARA_LENGTH);
	memset(fState[nFilterNum].bUserState, 0, sizeof(fState[nFilterNum].bUserState));
	fState[nFilterNum].nFilterState = -1;
	nFilterNum++;
	return(TRUE);
}

BOOL UserController::DeleteFilterLabel(const INT nIndex)
{
	if (nIndex >= nFilterNum)
	{
		return(FALSE);
	}
	for (INT i1 = nIndex; i1<nFilterNum; i1++)
	{
		memcpy(&fState[i1], &fState[i1 + 1], sizeof(FilterLabel));
	}
}

BOOL UserController::EditFilterLabel(const INT nIndex, LPCWSTR wstrFilterLabel)
{
	if (nIndex >= nFilterNum)
	{
		return(FALSE);
	}
	memcpy(fState[nIndex].wstrLabelName, wstrFilterLabel, sizeof(WCHAR) * PARA_LENGTH);
}

BOOL UserController::SetUserFilter(const INT nUserIndex, LPBOOL bUserState)
{
	if (nUserIndex >= nUserNum)
	{
		return(FALSE);
	}
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		fState[i1].bUserState[nUserIndex] = bUserState[i1];
	}
	return(TRUE);
}




BOOL UserController::ShowFilterLabel(const INT nIndex, LPCWSTR strFilterLabel)
{
	if (nIndex > nFilterNum)
	{
		return(FALSE);
	}
	memset((void*)strFilterLabel, 0, sizeof(WCHAR) * PARA_LENGTH);
	memcpy((void*)strFilterLabel, fState[nIndex].wstrLabelName, sizeof(WCHAR) *PARA_LENGTH);
	return(TRUE);
}

BOOL UserController::FilterCheck(INT nUserIndex)
{
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		if (fState[i1].nFilterState != -1)
		{
			if (fState[i1].bUserState[nUserIndex] != fState[i1].nFilterState)
			{
				return(FALSE);
			}
		}
	}
	return(TRUE);
}

BOOL UserController::GetUserFilterState(const INT nIndex, LPBOOL bUserState)
{
	if (nIndex > nUserNum)
	{
		return(FALSE);
	}
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		bUserState[i1] = fState[i1].bUserState[nIndex];
	}
}

BOOL UserController::GetFilterState(LPINT nFilterState)
{
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		nFilterState[i1] = fState[i1].nFilterState;
	}
	return(TRUE);
}

BOOL UserController::SetFilterState(LPINT nFilterState)
{
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		fState[i1].nFilterState = nFilterState[i1];
	}
	return(TRUE);
}





void UserController::SetEVEPath(LPCWSTR wstrEVEPath)
{
	
	memcpy(this->wstrEVEPath, wstrEVEPath, sizeof(WCHAR) * PARA_LENGTH);
	bEveExist = TRUE;
	UpdateSetting();

}

void UserController::ReadSetting()
{
	//read config setting
	memset(wstrEVEPath, 0, sizeof(wstrEVEPath));
	GetPrivateProfileStringW(TEXT("Controller"), TEXT("EveExecuteFilePath"), TEXT("\0"), wstrEVEPath, PARA_LENGTH, TEXT(STR_CONFIG));
	bEveExist = FileExist(wstrEVEPath);

	GetPrivateProfileStringW(TEXT("Controller"), TEXT("DataBasePath"), TEXT("\0"), wstrDataBasePath, PARA_LENGTH, TEXT(STR_CONFIG));

	bLocalSetting = GetPrivateProfileIntW(TEXT("Controller"), TEXT("DataBasePath"), 0, TEXT(STR_CONFIG));

	if (FileExist(wstrDataBasePath))
	{
		ReadData();
	}
	else
	{
		memset(wstrDataBasePath, 0, sizeof(wstrDataBasePath));
		wsprintf(wstrDataBasePath, TEXT(STR_DATABASE));
	}
}

void UserController::ReadData()
{
	HANDLE fDataBase = CreateFile(wstrDataBasePath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBuffer;
	ReadFile(fDataBase, (void*)&nFilterNum, sizeof(INT), &dwBuffer, NULL);
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		CHAR bFile[LONG_PARA_LENGTH], bDecode[LONG_PARA_LENGTH];
		ReadFile(fDataBase, (void*)bFile, LONG_PARA_LENGTH, &dwBuffer, NULL);
		Decode(bFile, bDecode);
		memcpy((void*)&fState[i1], bDecode, sizeof(FilterLabel));
	}

	ReadFile(fDataBase, (void*)&nUserNum, sizeof(INT), &dwBuffer, NULL);
	for (INT i1 = 0; i1 < nUserNum; i1++)
	{
		CHAR bFile[LONG_PARA_LENGTH], bDecode[LONG_PARA_LENGTH];
		ReadFile(fDataBase, (void*)bFile, LONG_PARA_LENGTH, &dwBuffer, NULL);
		Decode(bFile, bDecode);
		memcpy((void*)&userCurrent[i1], bDecode, sizeof(UserData));
	}

	CloseHandle(fDataBase);
}

void UserController::UpdateSetting()
{

	//update config setting
	WritePrivateProfileStringW(TEXT("Controller"), TEXT("EveExecuteFilePath"), wstrEVEPath, TEXT(STR_CONFIG));

	WritePrivateProfileStringW(TEXT("Controller"), TEXT("DataBasePath"), wstrDataBasePath, TEXT(STR_CONFIG));

	WritePrivateProfileStringW(TEXT("Controller"), TEXT("DataBasePath"), bLocalSetting?TEXT("1"):TEXT("0"), TEXT(STR_CONFIG));



}

void UserController::UpdateData()
{
	mutexFile.lock();

	HANDLE fDataBase = CreateFile(wstrDataBasePath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBuffer;

	WriteFile(fDataBase, (void*)&nFilterNum, sizeof(INT), &dwBuffer, NULL);
	for (INT i1 = 0; i1 < nFilterNum; i1++)
	{
		CHAR bFile[LONG_PARA_LENGTH], bEncode[LONG_PARA_LENGTH];
		memset(bEncode, 0, sizeof(bEncode));
		memcpy(bEncode, (void*)&fState[i1], sizeof(FilterLabel));
		Encode(bEncode, bFile);
		WriteFile(fDataBase, (void*)bFile, LONG_PARA_LENGTH, &dwBuffer, NULL);
	}

	WriteFile(fDataBase, (void*)&nUserNum, sizeof(INT), &dwBuffer, NULL);
	for (INT i1 = 0; i1 < nUserNum; i1++)
	{
		CHAR bFile[LONG_PARA_LENGTH], bEncode[LONG_PARA_LENGTH];
		memset(bEncode, 0, sizeof(bEncode));
		memcpy(bEncode, (void*)&userCurrent[i1], sizeof(UserData));
		Encode(bEncode, bFile);
		WriteFile(fDataBase, (void*)bFile, LONG_PARA_LENGTH, &dwBuffer, NULL);
	}

	CloseHandle(fDataBase);
	mutexFile.unlock();
}



BOOL UserController::FileExist(LPCWSTR wstrFilePath)
{
	if (GetFileAttributes(wstrFilePath) != INVALID_FILE_ATTRIBUTES || (
		GetLastError() != ERROR_FILE_NOT_FOUND &&
		GetLastError() != ERROR_PATH_NOT_FOUND &&
		GetLastError() != ERROR_INVALID_NAME &&
		GetLastError() != ERROR_INVALID_DRIVE &&
		GetLastError() != ERROR_NOT_READY &&
		GetLastError() != ERROR_INVALID_PARAMETER &&
		GetLastError() != ERROR_BAD_PATHNAME))
	{
		return(TRUE);
	}
	else
	{
		return(FALSE);
	}
}

BOOL UserController::Encode(LPCSTR strIn, LPCSTR strOut)
{
	memcpy((void*)strOut, strIn, LONG_PARA_LENGTH); 
	return(TRUE);
}


BOOL UserController::Decode(LPCSTR strIn, LPCSTR strOut)
{
	memcpy((void*)strOut, strIn, LONG_PARA_LENGTH);
	return(TRUE);
}

INT UserController::LogonUserCN(const INT nIndex)
{
	BOOL bTmp;
	WCHAR wstrToken[LONG_PARA_LENGTH],wstrBuffer[LONG_PARA_LENGTH];
	LogonModule *lmPort = new LogonModule();
	INT nReturned = 0;
	bTmp = lmPort->StartLogonConnect(); 
	if (bTmp)
	{
		bTmp = lmPort->GetLoginPageCookie();
	}
	
	if (bTmp)
	{
		bTmp = lmPort->PostUserInfo(userCurrent[nIndex].wstrUserName, userCurrent[nIndex].wstrPassWord);
	}

	if (bTmp)
	{
		bTmp = lmPort->GetToken(wstrToken, LONG_PARA_LENGTH);
	}

	if (bTmp)
	{
		if (bLocalSetting)
		{
			wsprintf(wstrBuffer, TEXT(" /LUA:OFF /noconsole /ssoToken=%s"), wstrToken);
		}
		else
		{
			wsprintf(wstrBuffer, TEXT(" /noconsole /ssoToken=%s"), wstrToken);
		}

		nReturned = (INT)ShellExecuteW(NULL, NULL, wstrEVEPath, wstrBuffer, NULL, SW_SHOW);
	}

	delete(lmPort);
	return(nReturned);
	
}

BOOL UserController::AvailLogon()
{
	return(bEveExist);
}

BOOL UserController::UseLocalSetting()
{
	return(bLocalSetting);
}

void UserController::SetLocalSetting(const BOOL bState)
{
	bLocalSetting = bState;
}


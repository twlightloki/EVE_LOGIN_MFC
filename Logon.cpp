#include "stdafx.h"
#include "Logon.h"




LogonModule::LogonModule()
{
	hSession = 0;
	hConnect = 0;
	hRequest = 0; 
	bAvail = TRUE;
	hSession = WinHttpOpen(TEXT("KOM_Logon"), NULL, NULL, NULL, NULL);
}

LogonModule::~LogonModule()
{
	WinHttpCloseHandle(hConnect);
	WinHttpCloseHandle(hSession);
}

BOOL LogonModule::StartLogonConnect()
{
	return(StartConnect(TEXT(URL_HOST), 443));

}

BOOL LogonModule::GetLoginPageCookie()
{
	WCHAR wstrBuffer[STR_LENGTH];
	memset(wstrBuffer, 0, sizeof(wstrBuffer));
	DWORD dwHeaderLen=STR_LENGTH;
	return(GetRequest(TEXT(URL_LOGIN_PATH), wstrBuffer, &dwHeaderLen));
}

BOOL LogonModule::PostUserInfo(LPCWSTR wstrUserName, LPCWSTR wstrPassWord)
{
	WCHAR wstrBuffer[STR_LENGTH];
	wsprintf(wstrBuffer, TEXT("UserName=%s&Password=%s&CaptchaToken=&Captcha="), wstrUserName, wstrPassWord);
	return(PostRequest(TEXT(URL_LOGIN_PATH_2), wstrBuffer, NULL,NULL));
}

BOOL LogonModule::GetToken(LPWSTR wstrToken,const INT nTokenBufferLen)
{
	WCHAR wstrBuffer[STR_LENGTH];
	memset(wstrBuffer, 0, sizeof(wstrBuffer));
	DWORD dwHeaderLen = STR_LENGTH;
	BOOL bSucc = GetRequest(TEXT(URL_LOGIN_PATH), wstrBuffer, &dwHeaderLen);
	if (!bSucc)
	{
		return(FALSE);
	}
	//use pattern
	INT nSucc = -1;
	for (INT i1 = 0; i1 < dwHeaderLen - 13; i1++)
	{
		WCHAR wstrTemp[14];
		memcpy(wstrTemp, &wstrBuffer[i1], sizeof(WCHAR)*13);
		wstrTemp[13] = TEXT('\0');
		if (StrCmpCW(wstrTemp, TEXT("access_token=")) == 0)
		{
			nSucc = i1;
			break;
		}
	}
	if (nSucc == -1)
	{
		return(FALSE);
	}
	INT nSucc2;
	for (INT i1 = 0; i1 < dwHeaderLen;i1++)
		if (wstrBuffer[i1] == TEXT('&'))
		{
			nSucc2 = i1;
			break; 
		}
	memset(wstrToken, 0, sizeof(WCHAR) * nTokenBufferLen);
	memcpy(wstrToken, &wstrBuffer[nSucc+13], sizeof(WCHAR) * (nSucc2 - nSucc-13));
	
}

BOOL LogonModule::StartConnect(LPCWSTR wstrHostName, INTERNET_PORT nPort)
{
	if (bAvail && hSession!=NULL)
	{
		bAvail = FALSE;
		hConnect = WinHttpConnect(hSession, wstrHostName, nPort, 0);
		if (hConnect != NULL)
		{
			return(TRUE);
		}
	}
	return(FALSE);
}


BOOL LogonModule::GetRequest(LPCWSTR wstrURL, LPWSTR wstrHeader, DWORD *dwHeaderLen)
{
	hRequest = WinHttpOpenRequest(hConnect, TEXT("GET"), wstrURL, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	DWORD flag = WINHTTP_DISABLE_REDIRECTS;
	if (hRequest == NULL || 
		!WinHttpSetOption(hRequest, WINHTTP_OPTION_DISABLE_FEATURE, &flag, sizeof(DWORD)))
	{
		return(FALSE);
	}
	BOOL bSucc = WinHttpSendRequest(hRequest, 0, 0, 0, 0, 0, 0);
	if (!bSucc)
	{
		return(FALSE);
	}
	bSucc = WinHttpReceiveResponse(hRequest, 0);
	if (!bSucc)
	{
		return(FALSE);
	}
	if (wstrHeader != NULL)
	{
		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS, 0, wstrHeader, dwHeaderLen, WINHTTP_NO_HEADER_INDEX);
	}
	WinHttpCloseHandle(hRequest);
	return(TRUE);
}

BOOL LogonModule::PostRequest(LPCWSTR wstrURL, LPCWSTR wstrPostData, LPWSTR wstrHeader, DWORD *dwHeaderLen)
{
	hRequest = WinHttpOpenRequest(hConnect, TEXT("POST"), wstrURL, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	DWORD flag = WINHTTP_DISABLE_REDIRECTS;
	if (hRequest == NULL ||
		!WinHttpSetOption(hRequest, WINHTTP_OPTION_DISABLE_FEATURE, &flag, sizeof(DWORD)))
	{
		return(FALSE);
	}
	WinHttpAddRequestHeaders(hRequest, TEXT("Content-Type: application/x-www-form-urlencoded\r\n"), -1L, WINHTTP_ADDREQ_FLAG_ADD);
	CHAR strPostData[STR_LENGTH];
	memset(strPostData, 0, sizeof(strPostData));
	WideCharToMultiByte(0, 0, wstrPostData, wcslen(wstrPostData), strPostData, STR_LENGTH, NULL, NULL);
	BOOL bSucc = WinHttpSendRequest(hRequest, 0, 0, (WCHAR*)strPostData, strlen(strPostData), strlen(strPostData),0);
	if (!bSucc)
	{
		return(FALSE);
	}
	bSucc = WinHttpReceiveResponse(hRequest, 0);
	if (!bSucc)
	{
		return(FALSE);
	}
	if (wstrHeader != NULL)
	{
		WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_RAW_HEADERS, 0, wstrHeader, dwHeaderLen, WINHTTP_NO_HEADER_INDEX);
	}
	WinHttpCloseHandle(hRequest);
	return(TRUE);
}


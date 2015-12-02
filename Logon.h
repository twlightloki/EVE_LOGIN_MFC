#ifndef LOGON_H
#define LOGON_H

#include <winhttp.h>


#pragma comment (lib, "ws2_32.lib")

#define STR_LENGTH 16384
#define URL_HOST "auth.eve-online.com.cn"
#define URL_LOGIN_PATH "/oauth/authorize?client_id=eveclient&scope=eveClientLogin&response_type=token&redirect_uri=https://auth.eve-online.com.cn/launcher?client_id=eveclient&lang=zh&mac=None"
#define URL_LOGIN_PATH_2 "/Account/LogOn?ReturnUrl=/oauth/authorize?client_id=eveclient&scope=eveClientLogin&response_type=token&redirect_uri=https://auth.eve-online.com.cn/launcher?client_id=eveclient&lang=zh&mac=None"


class LogonModule
{
public:
	LogonModule();
	~LogonModule();

	BOOL StartLogonConnect();

	BOOL GetLoginPageCookie();

	BOOL PostUserInfo(LPCWSTR wstrUserName, LPCWSTR wstrPassWord);

	BOOL GetToken(LPWSTR wstrToken, const INT nTokenBufferLen);




private:
	HINTERNET hSession, hConnect, hRequest;
	BOOL bAvail;


	BOOL StartConnect(LPCWSTR wstrHostName, INTERNET_PORT nPort);
	BOOL GetRequest(LPCWSTR wstrURL, LPWSTR wstrHeader, DWORD *dwHeaderLen);
	BOOL PostRequest(LPCWSTR wstrURL, LPCWSTR wstrPostData, LPWSTR wstrHeader, DWORD *dwHeaderLen);


};


#endif
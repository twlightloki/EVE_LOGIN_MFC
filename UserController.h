#ifndef USERCONTROLLER_H
#define USERCONTROLLER_H

#define PARA_LENGTH 64
#define LONG_PARA_LENGTH 2048
#define MAX_USER 256
#define MAX_FILTER 8

#define STR_CONFIG ".\\config.inf"
#define STR_DATABASE ".\\data.kom"

#include <mutex>
#include "Logon.h"

typedef struct UserData
{

	WCHAR wstrCharacter[PARA_LENGTH], wstrUserName[PARA_LENGTH], wstrPassWord[PARA_LENGTH];
	INT nCharaterLen, nUserNameLen, nPassWordLen;
	SYSTEMTIME timeSkill, timeTicket;

};

typedef struct FilterLabel
{
	WCHAR wstrLabelName[PARA_LENGTH];
	BOOL bUserState[MAX_USER];
	INT nFilterState;
};


class UserController
{
public:
	UserController();
	~UserController();

	BOOL AvailAddUser();

	BOOL AvailLogon();

	BOOL UseLocalSetting();

	INT AddUser(const UserData *userNew);
	
	BOOL EditUser(const INT nIndex, const UserData *userNew);

	BOOL DeleteUser(const INT nIndex);

	BOOL ShowUserInfo(const INT nIndex, const UserData *userNew);

	INT GetUserNum();


	INT GetFilterNum();

	BOOL AvailAddFilter();

	BOOL FilterCheck(INT nUserIndex);

	BOOL AddFilterLabel(LPCWSTR wstrFilterLabel);

	BOOL DeleteFilterLabel(const INT nIndex); 

	BOOL EditFilterLabel(const INT nIndex, LPCWSTR wstrFilterLabel);

	BOOL SetUserFilter(const INT nIndex, LPBOOL bUserState);

	BOOL ShowFilterLabel(const INT nIndex, LPCWSTR strFilterLabel);

	BOOL GetUserFilterState(const INT nIndex, LPBOOL bUserState);

	BOOL GetFilterState(LPINT nFilterState);

	BOOL SetFilterState(LPINT nFilterState);


	void SetEVEPath(LPCWSTR wstrEVEPath);

	INT LogonUserCN(const INT nIndex);

	void SetLocalSetting(const BOOL bState);


	void ReadSetting();
	void ReadData();

	void UpdateSetting();
	void UpdateData();

private:
	INT nUserNum, nFilterNum;

	BOOL bLocalSetting;

	UserData userCurrent[MAX_USER];

	FilterLabel fState[MAX_FILTER];

	std::mutex mutexFile;


	WCHAR wstrEVEPath[LONG_PARA_LENGTH], wstrDataBasePath[LONG_PARA_LENGTH];

	BOOL bEveExist;

	BOOL Encode(LPCSTR strIn, LPCSTR strOut);

	BOOL Decode(LPCSTR strIN, LPCSTR strOut);

	BOOL FileExist(LPCWSTR wstrFilePath);




};

#endif
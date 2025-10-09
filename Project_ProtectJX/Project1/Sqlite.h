#pragma once
#include <stdlib.h>

struct INFO_ACCOUNT_LOGIN
{
	char AccountName[32];
	char RoleName[32];
	char HardwareId[32];
	char MacAddress[18];
	char IpAddress[16];
	char LicenseId[33];
	int LastDetectionTime;
	int IsLogined;
};

struct Container;
typedef struct Container Container;
Container* ac_init();
void ac_dispose(Container* container);
size_t ac_get(Container* container, unsigned char** userBuf);
void ac_put(Container* container, const unsigned char* userData, size_t userDataSz);
bool	Sqlite_Init();
bool Sqlite_ReNew();
bool Sqlite_GetAndInsert_HardwareId_On_EnterGame_Gs(const char* cHardwareId, const char* cMacAddress, const char* cIpAddress);
//bool	Sqlite_Insert_Account_From_Login(const char* CodeCheck, const char* HardwareId, const char* AccountName, const char* MacAddress, const char* IpAddress, const char* LicenseId);
bool	Sqlite_Insert_Account_From_Login(const char* MacAddress, const char* IpAddress, const char* HardwareId, const char* HardwareSerialId, const char* AccountName);
bool	Sqlite_InsertOrUpdate_HardwareId_On_EnterGame(const char* AccountName);
bool	Sqlite_Insert_IpAddr_From_Login(const char* AccountName, const char* IpAddr);
bool	Sqlite_Check_WanAddress_GS(const char* IpAddress);
bool	Sqlite_Remove_HardwareId_On_Outgame(const char* AccountName);
int Sqlite_GetHardwareIdByAccount(const char* AccountName, const void* HardwareId, int nType);
bool	Sqlite_Check_HardwareId_On_EnterGame(const char* AccountName);
int	Sqlite_GetAndInsert_HardwareId_On_EnterGame(const char* AccountName, const void* HardwareId, int nType);
bool	Sqlite_Check_HardwareId(const char* HardwareId, const char* MacAddress, const char* IpAddress);
bool	Sqlite_Update_HardwareId(const char* AccountName, const char* HardwareId, const char* MacAddress, const char* IpAddress);
bool	Sqlite_Update_WanAddress(const char* AccountName, const char* IpAddress);
void	Sqlite_UpdateRoleInfo_Level_On_EnterGame(const char* AccountName, int nLevel);
void	Sqlite_UpdateRoleInfo_On_EnterGame(const char* AccountName, const char* pRoleName, int nLevel, INT64 nPlayerExp);
bool	Sqlite_Check_HardwareId_GS(const char* HardwareId, const char* IpAddress);
void	Sqlite_BackNumber_On_Kichout_ByLimit(const char* cHardwareId, const char* cMacAddress, const char* cIpAddress);
static int callback(void* NotUsed, int argc, char** argv, char** azColName);
static int callback_cound(void* count, int argc, char** argv, char** azColName);
static int callback_Get_HardwareId(void* data, int argc, char** argv, char** azColName);
static int callback_Loop_Row(void* data, int argc, char** argv, char** azColName);
int Sqlite_GetHardwareIdByAccount(const char* AccountName);
int Sqlite_Sort_Top_Data();
void Sqlite_FactionRemove(const char* AccountName);
bool Sqlite_CheckFaction(const char* Account, const char* IpAddress, int nFaction, int nLimit);
bool Sqlite_CheckFaction_IpAddress(const char* IpAddress, int nFaction, int nLimit);
bool Sqlite_Remove_IP_On_Outgame(const char* AccountName);
bool Sqlite_Check_IpAddress(const char* IpAddress);
bool Sqlite_Insert_IpAddr_JoinGS(const char* AccountName, const char* IpAddr);

class CSqlite
{
public:
	CSqlite();
	virtual	~CSqlite();
	void	Init();
};

extern CSqlite gSqlite;
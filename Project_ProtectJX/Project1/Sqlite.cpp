#include "stdafx.h"
#include <stdio.h>
#include <sqlite3.h>
#include <cstdlib>
//#include "KLoginX.h"
#include "Sqlite.h"
//#include <malloc.h>
#include <string.h>
#include "Util.h"
#include <iostream>
#include "Authenticate.h"
#include "KPlayerNew.h"

sqlite3* db;
int ReconnectDb;

#define Sqlite_IpToString		((LPCSTR(__cdecl*)(DWORD dwIp, LPSTR pszIp, size_t nLen)) 0x805A6DE)//0x0063A578 
#define Sqlite_IpFromString		((DWORD(__cdecl*)(LPCSTR pszIp)) 0x805A66A)//0x0063A578 

CSqlite::CSqlite() {
	//int nLength = sizeof(CDataLimitConnects);
	//this->nDataLimitConnect->AccountName = AccountName;
}
// -----------------------------------------------------------------------------

CSqlite::~CSqlite() {
}

void CSqlite::Init() {

}

struct Container
{
	unsigned char* buf;
	size_t dataSz;
	size_t allocSz;
};

/** Create and initialize a struct Container */
struct Container* ac_init()
{
	struct Container* newCont = (Container*)malloc(sizeof(struct Container));
	if (!newCont) { exit(1); } // out of mem
	newCont->dataSz = 0;
	newCont->allocSz = 0;
	newCont->buf = NULL;
	return newCont;
}

void ac_dispose(struct Container* container)
{
	free(container->buf);
	free(container);
}

size_t ac_get(struct Container* container, unsigned char** userBuf)
{
	if (container->dataSz > 0)
	{
		*userBuf = (unsigned char*)malloc(container->dataSz);
		if (!*userBuf) { exit(1); } // out of mem
		memcpy(*userBuf, container->buf, container->dataSz);
	}
	return container->dataSz;
}

void ac_put(struct Container* container, const unsigned char* userData, size_t userDataSz)
{
	if (userDataSz != 0)
	{
		if (container->allocSz < userDataSz)
		{
			free(container->buf);
			container->buf = (unsigned char*)malloc(userDataSz);
			if (!container->buf) { exit(1); } // out of mem
			container->allocSz = userDataSz;
		}
		memcpy(container->buf, userData, userDataSz);
	}
	container->dataSz = userDataSz;
}

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
#if (_DEBUG_SQLITE==1)
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
#endif
	}
#if (_DEBUG_SQLITE==1)
	printf("\n");
#endif
	return 0;
}

static int callback_datalogin(void* data, int argc, char** argv, char** azColName) {
	int i;
	
	//fprintf(stderr, "%s: ", (const char*)data);
	//memset(data,0,sizeof(INFO_ACCOUNT_LOGIN));
	INFO_ACCOUNT_LOGIN* c = (INFO_ACCOUNT_LOGIN*)data;

	for (i = 0; i < argc; i++) {
#if (_DEBUG_SQLITE==1)
		printf("[%d] %s = %s\n", i, azColName[i], argv[i] ? argv[i] : "NULL");
#endif
		if (i == 0)
			strcpy(c->AccountName, argv[0]);
		else if (i == 1)
			strcpy(c->RoleName, argv[1]);
		else if (i == 2)
			strcpy(c->HardwareId, argv[2]);
		else if (i == 3)
			strcpy(c->MacAddress, argv[3]);
		else if (i == 4)
			strcpy(c->IpAddress, argv[4]);
		else if (i == 5)
			strcpy(c->LicenseId, argv[5]);
		else if (i == 6)
			c->LastDetectionTime = atoi(argv[6]);
		else if (i == 7)
			c->IsLogined = atoi(argv[7]);
	}
#if (_DEBUG_SQLITE==1)
	printf("\n");
#endif
	return 0;
}

static int callback_HardwareId_entergame(void* data, int argc, char** argv, char** azColName) {
	int i;

	char* c = (char*)data;

	//fprintf(stderr, "contDataSz: 0\n");
	//*c = *(char*)argv[0];
	//strcpy(&c[0], argv[0]);
	//strcpy(c, argv[0]);
	//c[36] = 0;
	//printf("%s\n", c);
	//strcpy(&c[37], argv[1]);
	//c[54] = '\0';
	//printf("%s\n", c[37]);
	//strcpy(&c[55], argv[2]);
	//sprintf(c, (const char*)"%s%s%s", argv[0], argv[1], argv[2]);
	//c[72] = '\0';
	//printf("%s\n", c);
	//strcpy(&c[23], argv[1]);
	for (i = 0; i < argc; i++)
	{

		if (i == 0)
		{
			strcpy(c, argv[0]);
			c[31] = 0;
		}
		else if (i == 1)
		{
			strcpy(&c[32], argv[1]);
			c[48] = 0;
		}
		else if (i == 2)
		{
			strcpy(&c[49], argv[2]);
			c[67] = 0;
			//c[49] = atoi(argv[2]);
			//c[50] = 0;
			//strcpy(&c[51], argv[2]);
			//c[72] = 0;
			//printf("nWorldStat: %d , %d\n", c[51], atoi(argv[2]));
		}
		else
			break;

		//printf("[%d] %s = %s\n", i, azColName[i], argv[i] ? argv[i] : "NULL");

	}
	//printf("\n");
	return 0;
}

static int callback_Get_HardwareId(void* data, int argc, char** argv, char** azColName) {
	int i;

	char* c = (char*)data;
	//*c = *(char*)argv[0];
	strcpy(c, argv[0]);
	c[36] = 0;
	strcpy(&c[37], argv[1]);
	//for (i = 0; i < argc; i++) {
	//	printf("[%d] %s = %s\n", i, azColName[i], argv[i] ? argv[i] : "NULL");
	//}
	//printf("\n");
	return 0;
}

static int callback_data_entergame(void* data, int argc, char** argv, char** azColName) {
	int i;

	int* c = (int*)data;
	*c = atoi(argv[0]);

	//for (i = 0; i < argc; i++) {
	//	printf("[%d] %s = %s\n", i, azColName[i], argv[i] ? argv[i] : "NULL");
	//}
	//printf("\n");
	return 0;
}

static int callback_byte_entergame(void* data, int argc, char** argv, char** azColName) {
	int i;

	char* c = (char*)data;
	strcpy(c, argv[0]);
	//for (i = 0; i < argc; i++) {
	//	printf("[%d] %s = %s\n", i, azColName[i], argv[i] ? argv[i] : "NULL");
	//}
	//printf("\n");
	return 0;
}

static int callback_cound(void* count, int argc, char** argv, char** azColName) {
	int i;
	int* c = (int*)count;
	*c = atoi(argv[0]);
	return 0;
}

bool Sqlite_Init()
{
	try
	{
		char* zErrMsg = 0;
		int rc;
		char* sqlAccount;
		char* sqlHwid;
		char* sqlSetting;
		char* sqlIpWan;

		rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
			return false;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
	}

		memset(&sqlAccount, 0, sizeof(sqlAccount));

		sqlAccount = "CREATE TABLE IF NOT EXISTS Account("  \
			"AccountName           CHAR(50) PRIMARY KEY   NOT NULL," \
			"RoleName           CHAR(50)   NOT NULL DEFAULT ''," \
			"PlayerLevel            INT DEFAULT 0," \
			"PlayerExp            INT DEFAULT 0," \
			"CurStrength            INT DEFAULT 0," \
			"CurDexterity            INT DEFAULT 0," \
			"CurVitality            INT DEFAULT 0," \
			"CurEngergy            INT DEFAULT 0," \
			"CurLucky            INT DEFAULT 0," \
			"PlayGameTime            INT DEFAULT 0," \
			"WorldStat            INT DEFAULT 0," \
			"Guid           CHAR(50)    NOT NULL DEFAULT ''," \
			"CodeCheck           CHAR(50)    NOT NULL DEFAULT ''," \
			"HardwareId           CHAR(50)    NOT NULL DEFAULT ''," \
			"MacAddress           CHAR(50)    NOT NULL DEFAULT ''," \
			"IpAddress           CHAR(50)    NOT NULL DEFAULT ''," \
			"WanAddress           CHAR(50)    NOT NULL DEFAULT ''," \
			"LicenseId           CHAR(50)    NOT NULL DEFAULT ''," \
			"LastDetectionTime            INT DEFAULT 0," \
			"IsLogined       INT DEFAULT 0);";

		rc = sqlite3_exec(db, sqlAccount, callback, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			return false;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stdout, "Table created successfully\n");
#endif
}

		memset(&sqlHwid, 0, sizeof(sqlHwid));

		sqlHwid = "CREATE TABLE IF NOT EXISTS HardwareId("  \
			"cAccountName           CHAR(50)    NOT NULL DEFAULT ''," \
			"cHardwareId           CHAR(50)    NOT NULL DEFAULT ''," \
			"cMacAddress           CHAR(50)    NOT NULL DEFAULT ''," \
			"cIpAddress           CHAR(50)    NOT NULL DEFAULT ''," \
			"nCount INT     DEFAULT 0);";

		rc = sqlite3_exec(db, sqlHwid, callback, 0, &zErrMsg);

		memset(&sqlHwid, 0, sizeof(sqlHwid));

		sqlHwid = "CREATE TABLE IF NOT EXISTS FactionData("  \
			"AccountName           CHAR(50)    NOT NULL DEFAULT ''," \
			"WanAddress           CHAR(50)   NOT NULL DEFAULT ''," \
			"CurFaction INT     DEFAULT 0);";

		rc = sqlite3_exec(db, sqlHwid, callback, 0, &zErrMsg);

		memset(&sqlHwid, 0, sizeof(sqlHwid));

		sqlHwid = "CREATE TABLE IF NOT EXISTS Option("  \
			"Id           INT PRIMARY KEY   DEFAULT 0," \
			"Name           CHAR(255)   NOT NULL DEFAULT ''," \
			"Value           CHAR(50)   NOT NULL DEFAULT '');";

		rc = sqlite3_exec(db, sqlHwid, callback, 0, &zErrMsg);
		
		memset(&sqlIpWan, 0, sizeof(sqlIpWan));

		sqlIpWan = "CREATE TABLE IF NOT EXISTS IpAddrClient("  \
			"AccountName           CHAR(32) PRIMARY KEY   NOT NULL," \
			"IpAddr           CHAR(50)   NOT NULL DEFAULT '');";

		rc = sqlite3_exec(db, sqlIpWan, callback, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			return false;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stdout, "Table created successfully\n");
#endif
		}

		if (gAuthenticate.m_IsBishop)
		{

			//sqlite3_exec(db, "UPDATE HardwareId set nCount = 0;", callback, 0, &zErrMsg);
			sqlite3_exec(db, "DELETE FROM IpAddrClient;", callback, 0, 0);
			rc = sqlite3_exec(db, "DELETE FROM HardwareId;", callback, 0, &zErrMsg);

			if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
				sqlite3_free(zErrMsg);
				return false;
			}
			else {
#if (_DEBUG_SQLITE==1)
				fprintf(stdout, "Clean table HardwareId successfully\n");
#endif
			}

			int count = 0;
			char sql[512];

			memset(&sql, 0, sizeof(sql));

			sprintf((char*)sql, (const char*)"SELECT COUNT(*) FROM Option WHERE Name = 'GameLimit';");

			rc = sqlite3_exec(db, sql, callback_cound, &count, &zErrMsg);

			if (rc == SQLITE_OK)
			{
				if (count == 0)
				{
					memset(&sql, 0, sizeof(sql));
					sprintf((char*)sql, (const char*)"INSERT INTO Option (Name, Value) VALUES ('GameLimit', '%d');", gUtil.nLimit);
					sqlite3_exec(db, sql, 0, 0, &zErrMsg);
				}
				else
				{
					memset(&sql, 0, sizeof(sql));
					sprintf((char*)sql, (const char*)"UPDATE Option set Value = '%d' WHERE Name = 'GameLimit';", gUtil.nLimit);
					sqlite3_exec(db, sql, 0, 0, &zErrMsg);
				}
			}
		}

		if (gAuthenticate.m_GameServer)
		{
			int count = 0;
			char sql[512];

			rc = sqlite3_exec(db, "SELECT Value FROM Option WHERE Name = 'GameLimit';", callback_data_entergame, &count, &zErrMsg);
			
			if (rc == SQLITE_OK)
			{
				if (count > 0)
				{
					gUtil.nLimit = count;
				}
			}
		}
		

		//==============================================\\

//		memset(&sqlSetting, 0, sizeof(sqlSetting));
//
//		sqlSetting = "CREATE TABLE IF NOT EXISTS Setting("  \
//			"nVersion INT     DEFAULT 0);";
//
//		rc = sqlite3_exec(db, sqlSetting, callback, 0, &zErrMsg);
//
//		if (rc != SQLITE_OK) {
//#if (_DEBUG_SQLITE==1)
//			fprintf(stderr, "SQL error: %s\n", zErrMsg);
//#endif
//			sqlite3_free(zErrMsg);
//			return false;
//		}
//		else {
//#if (_DEBUG_SQLITE==1)
//			fprintf(stdout, "Table created successfully\n");
//#endif
//		}

		//sqlite3_close(db);

		return true;
	}
	catch (...)
	{
		return false;
	}
	
	return true;
}

bool Sqlite_ReNew()
{
	try
	{
		bool result;
		char* zErrMsg = 0;
		int count = 0;
		char sql[512];
		int rc = 0;
		
		memset(&sql, 0, sizeof(sql));

		sprintf((char*)sql, (const char*)"SELECT COUNT(*) FROM Option WHERE Name = 'GameLimit';");

		rc = sqlite3_exec(db, sql, callback_cound, &count, &zErrMsg);

		if (rc == SQLITE_OK)
		{
			if (count == 0)
			{
				memset(&sql, 0, sizeof(sql));
				sprintf((char*)sql, (const char*)"INSERT INTO Option (Name, Value) VALUES ('GameLimit', '%d');", gUtil.nLimit);
				sqlite3_exec(db, sql, 0, 0, &zErrMsg);
			}
			else
			{
				memset(&sql, 0, sizeof(sql));
				sprintf((char*)sql, (const char*)"UPDATE Option set Value = '%d' WHERE Name = 'GameLimit';", gUtil.nLimit);
				sqlite3_exec(db, sql, 0, 0, &zErrMsg);
			}
		}

		return true;
	}
	catch (...)
	{
		return false;
	}

	return true;
}

bool Sqlite_Insert_Account_From_Login(const char* MacAddress, const char* IpAddress, const char* HardwareId, const char* HardwareSerialId, const char* AccountName)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;
		
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[256];
		char sqlInsertAccount[256];
		int rc = 0;
		int count = 0;
		
		//pthread_mutex_lock(&cs_mutex);

		result = false;

		/*rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
		}*/

		//std::cout << "HardwareId: " << HardwareSerialId << std::endl;
		//std::cout << "MacAddress: " << MacAddress << std::endl;
		//std::cout << "IpAddress: " << IpAddress << std::endl;

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM Account WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sqlCount, callback_cound, &count, &zErrMsg);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQLite Rows: %d\n", count);
#endif
			if (count == 0)
			{

				sprintf((char*)sqlInsertAccount, (const char*)"INSERT INTO Account (AccountName,CodeCheck,HardwareId,MacAddress,WanAddress) VALUES ('%s', '%s', '%s', '%s', '%s');", AccountName, HardwareSerialId, HardwareId, MacAddress, IpAddress);

				rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Records created successfully\n");
#endif
					result = true;
				}

			}
			else
			{
				sprintf((char*)sqlInsertAccount, (const char*)"UPDATE Account set CodeCheck = '%s', HardwareId = '%s', MacAddress = '%s', WanAddress = '%s' WHERE AccountName='%s';", HardwareSerialId, HardwareId, MacAddress, IpAddress, AccountName);

				/* Execute SQL statement */
				rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Update done successfully\n");
#endif
					result = true;
				}
			}
			//fprintf(stdout, "Table created successfully\n");
		}
	EndProc:
		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;
	}
	catch (std::exception const& e)
	{
		//pthread_mutex_unlock(&cs_mutex);
		return false;
	}
	
}

bool Sqlite_InsertOrUpdate_HardwareId_On_EnterGame(const char* AccountName)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		int nCount = 0;
		BYTE BuffData[72];
		
		//pthread_mutex_lock(&cs_mutex);

		Container* c = ac_init();

		result = false;

//		rc = sqlite3_open(DATABASE_LIMITNAME, &db);
//
//		if (rc) {
//#if (_DEBUG_SQLITE==1)
//			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
//#endif
//			goto EndProc;
//		}
//		else {
//#if (_DEBUG_SQLITE==1)
//			fprintf(stderr, "Opened database successfully\n");
//#endif
//		}

		if (strlen(AccountName) <= 1 )
			return false;

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT CodeCheck,MacAddress,IpAddress FROM Account WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sqlCount, callback_HardwareId_entergame, (void*)BuffData, &zErrMsg);

		ac_put(c, (unsigned char*)BuffData, sizeof(BuffData) + 1);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			goto EndProc;
		}
		else {

			unsigned char* dataFromContainer;
			size_t contDataSz = ac_get(c, &dataFromContainer);
			if (contDataSz == 0)
			{
#if (_DEBUG_SQLITE==1)
				sqlite3_free(zErrMsg);
#endif
				goto EndProc01;
			}
			else
			{

				dataFromContainer[contDataSz - 1] = 0;
#if (_DEBUG_SQLITE==1)
				printf("HardwareId: %s\n", (const char*)dataFromContainer);
#endif
				memset(&sqlCount, 0, sizeof(sqlCount));

				sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cHardwareId = '%s' and cMacAddress = '%s' and cIpAddress = '%s';", (char*)dataFromContainer, (char*)dataFromContainer + 37, (char*)dataFromContainer + 55);

				rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, &zErrMsg);

				if (rc != SQLITE_OK) {

#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif // (_DEBUG_SQLITE==1)
					sqlite3_free(zErrMsg);
					goto EndProc01;
				}
				else {

#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQLite Rows: %d\n", count);
#endif

					if (count == 0)
					{

						memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

						sprintf((char*)sqlInsertAccount, (const char*)"INSERT INTO HardwareId (cHardwareId, cMacAddress, cIpAddress, nCount) VALUES ('%s', '%s', '%s', %d);", (const char*)dataFromContainer, (char*)dataFromContainer + 37, (char*)dataFromContainer + 55, 1);

						rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

						if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
							fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
							sqlite3_free(zErrMsg);
							goto EndProc01;
						}
						else {
#if (_DEBUG_SQLITE==1)
							fprintf(stdout, "Records created successfully\n");
#endif
						}
						result = true;

					}
					else
					{

						memset(&sqlCount, 0, sizeof(sqlCount));

						sprintf((char*)sqlCount, (const char*)"SELECT nCount FROM HardwareId WHERE cHardwareId = '%s' and cMacAddress = '%s' and cIpAddress = '%s';", (const char*)dataFromContainer, (char*)dataFromContainer + 37, (char*)dataFromContainer + 55);

						rc = sqlite3_exec(db, sqlCount, callback_data_entergame, &nCount, &zErrMsg);
#if (_DEBUG_SQLITE==1)
						fprintf(stderr, "nCount: %d / %d\n", nCount, gUtil.nLimit);
#endif
						
						if (nCount >= gUtil.nLimit)
						{
#if (_DEBUG_SQLITE==1)
							fprintf(stderr, "Limited \n");
#endif
							sqlite3_free(zErrMsg);
							result = false;
							goto EndProc01;
						}
						else
						{

							if (nCount <= 0)
							{

								memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

								sprintf((char*)sqlInsertAccount, (const char*)"UPDATE HardwareId set nCount = 1 WHERE cHardwareId= '%s' and cMacAddress = '%s' and cIpAddress = '%s';", (const char*)dataFromContainer, (char*)dataFromContainer + 37, (char*)dataFromContainer + 55);

								rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

								if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
									fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
									sqlite3_free(zErrMsg);
									goto EndProc01;
								}
								else {
#if (_DEBUG_SQLITE==1)
									fprintf(stdout, "Update login done successfully\n");
#endif
									result = true;
								}

							}
							else
							{

								memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

								sprintf((char*)sqlInsertAccount, (const char*)"UPDATE HardwareId set nCount = nCount + 1 WHERE cHardwareId= '%s' and cMacAddress = '%s' and cIpAddress = '%s';", (const char*)dataFromContainer, (char*)dataFromContainer + 37, (char*)dataFromContainer + 55);

								rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

								if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
									fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
									sqlite3_free(zErrMsg);
									goto EndProc01;
								}
								else {
#if (_DEBUG_SQLITE==1)
									fprintf(stdout, "Update login done successfully\n");
#endif
									result = true;

								}

							}

						}

					}
					//fprintf(stdout, "Table created successfully\n");
				}

			}
		EndProc01:
			free(dataFromContainer);
#if (_DEBUG_SQLITE==1)
			fprintf(stdout, "Operation done successfully\n");
#endif
		}
	EndProc:
		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;
	}
	catch (...)
	{
		//pthread_mutex_unlock(&cs_mutex);
		return false;
	}
	
}

bool Sqlite_Remove_HardwareId_On_Outgame(const char* AccountName)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		BYTE BuffData[72];
		
		//pthread_mutex_lock(&cs_mutex);

		Container* c = ac_init();

		result = false;



		/*rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
		}*/

		//fprintf(stderr, "GameServer Destroy : %s\n", AccountName);

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT HardwareId, WanAddress FROM Account WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sqlCount, callback_HardwareId_entergame, (void*)BuffData, &zErrMsg);

		ac_put(c, (unsigned char*)BuffData, sizeof(BuffData) + 1);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			result = false;
		}
		else {

			unsigned char* dataFromContainer;
			size_t contDataSz = ac_get(c, &dataFromContainer);
			if (contDataSz == 0)
			{
#if (_DEBUG_SQLITE==1)
				printf("[empty]\n");
#endif
				result = false;
			}
			else
			{

				dataFromContainer[contDataSz - 1] = 0;

				memset(&sqlCount, 0, sizeof(sqlCount));

				sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cAccountName = '%s' and cHardwareId = '%s' and cIpAddress = '%s';", AccountName, (const char*)dataFromContainer, (const char*)dataFromContainer + 33);

				rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, &zErrMsg);

				if (rc != SQLITE_OK) {

#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif // (_DEBUG_SQLITE==1)
					sqlite3_free(zErrMsg);
					result = false;
				}
				else
				{
					if (count > 0)
					{
						memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

						sprintf((char*)sqlInsertAccount, (const char*)"DELETE FROM HardwareId WHERE cAccountName = '%s' and cHardwareId = '%s' and cIpAddress = '%s';", AccountName, (const char*)dataFromContainer, (const char*)dataFromContainer + 33);

						rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

						if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
							fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
							sqlite3_free(zErrMsg);
							result = false;
						}
						else {
#if (_DEBUG_SQLITE==1)
							fprintf(stdout, "DELETE done successfully\n");
#endif
							result = true;
						}
					}
				}

			}
			free(dataFromContainer);
		}
		return result;
	}
	catch (...)
	{
		return false;
	}
}

bool Sqlite_Check_HardwareId(const char* HardwareId, const char* MacAddress, const char* IpAddress)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		int nCount = 0;
		//char BuffData[32];

		result = false;

		//pthread_mutex_lock(&cs_mutex);

		Container* c = ac_init();

		/*rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
		}*/

		//fprintf(stderr, "HardwareId : %s\n", HardwareId);
		//fprintf(stderr, "MacAddress : %s\n", MacAddress);
		//fprintf(stderr, "IpAddress : %s\n", IpAddress);

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cHardwareId = '%s';", HardwareId);

		rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, &zErrMsg);

		if (rc != SQLITE_OK) {

#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif // (_DEBUG_SQLITE==1)
			sqlite3_free(zErrMsg);
			goto EndProc;
		}
		else {

#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQLite Rows: %d\n", count);
#endif

			if (count == 0)
			{

				memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));
				//fprintf(stderr, "MacFromString : %d\n", (int)gUtil.string_to_mac(MacAddress));
				//fprintf(stderr, "IpFromString : %d\n", (int)Sqlite_IpFromString(IpAddress));
				//uint32_t IpAddr = (int)Sqlite_IpFromString(IpAddress);
				sprintf((char*)sqlInsertAccount, (const char*)"INSERT INTO HardwareId (cHardwareId, nCount) VALUES ('%s', 0);", HardwareId);

				rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Records created successfully\n");
#endif
					result = true;
				}

			}
			else
			{

				memset(&sqlCount, 0, sizeof(sqlCount));

				sprintf((char*)sqlCount, (const char*)"SELECT nCount FROM HardwareId WHERE cHardwareId = '%s';", HardwareId);

				rc = sqlite3_exec(db, sqlCount, callback_data_entergame, &nCount, &zErrMsg);
#if (_DEBUG_SQLITE==1)
				fprintf(stderr, "nCount: %d / %d\n", nCount, gUtil.nLimit);
#endif
				//fprintf(stderr, "nCount: %d / %d\n", nCount, gUtil.nLimit);

				if (nCount >= gUtil.nLimit)
				{
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "Limited \n");
#endif
					sqlite3_free(zErrMsg);
					return false;
					//goto EndProc;
				}
				else
				{
					return true;
				}

			}
			//fprintf(stdout, "Table created successfully\n");
		}

	EndProc:

		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;

	}
	catch (...)
	{
		//pthread_mutex_unlock(&cs_mutex);
		return true;
	}
	
}

bool Sqlite_Check_HardwareId_On_EnterGame(const char* AccountName)
{
	//pthread_mutex_t waitlock;
	//pthread_mutex_init(&waitlock, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		int nCount = 0;
		BYTE BuffData[72];
		//pthread_mutexattr_t mAttr;

		//pthread_mutex_t cs_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
		//pthread_mutex_init(&cs_mutex, &mAttr);

		//pthread_mutex_lock(&waitlock);

		Container* c = ac_init();

		result = false;

		/*rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
		}*/

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT HardwareId,WanAddress FROM Account WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sqlCount, callback_HardwareId_entergame, (void*)BuffData, &zErrMsg);

		ac_put(c, (unsigned char*)BuffData, sizeof(BuffData) + 1);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			return false;
		}
		else {

			unsigned char* dataFromContainer;
			size_t contDataSz = ac_get(c, &dataFromContainer);
			if (contDataSz == 0)
			{
#if (_DEBUG_SQLITE==1)
				sqlite3_free(zErrMsg);
#endif
			}
			else
			{

				dataFromContainer[contDataSz - 1] = 0;
				//std::cout << "HardwareId: " << (const char*)dataFromContainer << std::endl;
				//std::cout << "cIpAddress: " << (const char*)dataFromContainer + 33 << std::endl;
				//std::cout << "IpAddress: " << (const char*)dataFromContainer + 51 << std::endl;

#if (_DEBUG_SQLITE==1)
				printf("CheckCode: %d %s\n", strlen((const char*)dataFromContainer), (const char*)dataFromContainer);
#endif
				memset(&sqlCount, 0, sizeof(sqlCount));

				sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cHardwareId = '%s' and cIpAddress = '%s';", (const char*)dataFromContainer, (const char*)dataFromContainer + 33);

				rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, &zErrMsg);

				if (rc != SQLITE_OK) {

#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif // (_DEBUG_SQLITE==1)
					sqlite3_free(zErrMsg);
					result = false;
				}
				else {

#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQLite Rows: %d\n", count);
#endif
					if (count >= gUtil.nLimit)
					{
#if (_DEBUG_SQLITE==1)
						fprintf(stderr, "Limited \n");
#endif
						sqlite3_free(zErrMsg);

						result = false;

					}
					else
					{
						result = true;
						/*memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

						sprintf((char*)sqlInsertAccount, (const char*)"UPDATE HardwareId set nCount = nCount + 1 WHERE cHardwareId= '%s' and cMacAddress = '%s' and cIpAddress = '%s';", (const char*)dataFromContainer, (const char*)dataFromContainer + 37, (const char*)dataFromContainer + 55);

						rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

						if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
								fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
								sqlite3_free(zErrMsg);
								return false;
							}
							else {
#if (_DEBUG_SQLITE==1)
								fprintf(stdout, "Update enter game done successfully\n");
#endif
								sqlite3_free(zErrMsg);
								return true;
							}*/
					}
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQLite result: %d\n", result);
#endif
//					if (count <= gUtil.nLimit)
//					{
//
//						//result = false;
//
//						memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));
//
//						sprintf((char*)sqlInsertAccount, (const char*)"INSERT INTO HardwareId (cHardwareId, cIpAddress, nCount) VALUES ('%s', '%s', '%s', %d);", (const char*)dataFromContainer, (const char*)dataFromContainer + 51, 1);
//
//						rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);
//
//						if (rc != SQLITE_OK) {
//	#if (_DEBUG_SQLITE==1)
//							fprintf(stderr, "SQL error: %s\n", zErrMsg);
//	#endif
//							sqlite3_free(zErrMsg);
//							result = false;
//						}
//						else {
//	#if (_DEBUG_SQLITE==1)
//							fprintf(stdout, "Records created successfully\n");
//	#endif
//							sqlite3_free(zErrMsg);
//							result = true;
//						}
//						
//
//					}
//					else
//					{
//
//						memset(&sqlCount, 0, sizeof(sqlCount));
//
//						sprintf((char*)sqlCount, (const char*)"SELECT nCount FROM HardwareId WHERE cHardwareId = '%s' and cIpAddress = '%s';", (const char*)dataFromContainer, (const char*)dataFromContainer + 55);
//
//						rc = sqlite3_exec(db, sqlCount, callback_data_entergame, &nCount, &zErrMsg);
//#if (_DEBUG_SQLITE==1)
//						fprintf(stderr, "nCount: %d / %d\n", nCount, gUtil.nLimit);
//#endif
//
//						//fprintf(stderr, "nCount %s : %d / %d\n", (const char*)dataFromContainer, nCount, gUtil.nLimit);
//
//						if (nCount >= gUtil.nLimit)
//						{
//#if (_DEBUG_SQLITE==1)
//							fprintf(stderr, "Limited \n");
//#endif
//							sqlite3_free(zErrMsg);
//
//							result = false;
//
//						}
//						else
//						{
//							result = true;
//							/*memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));
//
//							sprintf((char*)sqlInsertAccount, (const char*)"UPDATE HardwareId set nCount = nCount + 1 WHERE cHardwareId= '%s' and cMacAddress = '%s' and cIpAddress = '%s';", (const char*)dataFromContainer, (const char*)dataFromContainer + 37, (const char*)dataFromContainer + 55);
//
//							rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);
//
//							if (rc != SQLITE_OK) {
//#if (_DEBUG_SQLITE==1)
//								fprintf(stderr, "SQL error: %s\n", zErrMsg);
//#endif
//								sqlite3_free(zErrMsg);
//								return false;
//							}
//							else {
//#if (_DEBUG_SQLITE==1)
//								fprintf(stdout, "Update enter game done successfully\n");
//#endif
//								sqlite3_free(zErrMsg);
//								return true;
//							}*/
//						}
//
//					}
					//fprintf(stdout, "Table created successfully\n");
				}

			}

			free(dataFromContainer);

		}

		//sqlite3_close(db);

		//pthread_mutex_unlock(&waitlock);

#if (_DEBUG_SQLITE==1)
		fprintf(stdout, "Operation done successfully\n");
#endif

		return result;
	}
	catch (std::exception const& e)//catch (...)
	{
		fprintf(stdout, "Exception\n");
		fprintf(stdout, e.what());
		//pthread_mutex_unlock(&waitlock);
		return false;
	}
	
}

int Sqlite_GetAndInsert_HardwareId_On_EnterGame(const char* AccountName, const void* HardwareId, int nType)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);
	//std::cout << "Opened database successfully" << std::endl;
	try
	{
		int result;
		const char* cHardwareId;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		int count2 = 0;
		int nCount = 0;
		BYTE BuffData[72];

		char* dataType;

		if (nType == 0)
			dataType = "CodeCheck";
		else if(nType == 1)
			dataType = "MacAddress";
		else if(nType == 2)
			dataType = "WanAddress";
		else if (nType == 3)
			dataType = "HardwareId,WanAddress,MacAddress";

		//pthread_mutex_init(&cs_mutex, &mAttr);

		Container* c = ac_init();

		result = 0;

		//pthread_mutex_lock(&cs_mutex);

		/*rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
		}*/
		//fprintf(stderr, "Opened database successfully %s\n", AccountName);

		char sql[512];

		rc = sqlite3_exec(db, "SELECT Value FROM Option WHERE Name = 'GameLimit';", callback_data_entergame, &count, &zErrMsg);

		if (rc == SQLITE_OK)
		{
			if (count > 0)
			{
				gUtil.nLimit = count;
			}
			else
				gUtil.nLimit = 0;
		}

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT %s FROM Account WHERE AccountName = '%s';", dataType, AccountName);

		rc = sqlite3_exec(db, sqlCount, callback_HardwareId_entergame, (void*)BuffData, &zErrMsg);

		ac_put(c, (unsigned char*)BuffData, sizeof(BuffData) + 1);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			result = 0;
		}
		else {

			unsigned char* dataFromContainer;
			size_t contDataSz = ac_get(c, &dataFromContainer);
			if (contDataSz == 0)
			{
#if (_DEBUG_SQLITE==1)
				fprintf(stderr, "contDataSz: 0\n");
				sqlite3_free(zErrMsg);
#endif
				sqlite3_free(zErrMsg);
				result = 0;
			}
			else
			{

				dataFromContainer[contDataSz - 1] = 0;
				memcpy((void*)(HardwareId), dataFromContainer, 72);
				//memcpy((void*)(HardwareId), dataFromContainer + 23, 32);
				//std::cout << "HardwareId: " << (const char*)dataFromContainer << std::endl;
				//std::cout << "MacAddress: " << (const char*)dataFromContainer + 36 << std::endl;
				//std::cout << "IpAddress: " << (const char*)dataFromContainer + 55 << std::endl;
				//strcpy((char*)HardwareId, (const char*)dataFromContainer);
				//std::cout << "HardwareId: " << HardwareId << std::endl;
				//strcpy((char*)pszMacAddress, (const char*)dataFromContainer + 36);

				//strcpy((char*)pszIpAddress, (const char*)dataFromContainer + 55);
				//*(HardwareId) = (char)cHardwareId;
				//memcpy(&HardwareId, (const char*)dataFromContainer, 32);

				//memcpy(&pszMacAddress, (const char*)dataFromContainer + 36, 18);

				//memcpy(&pszIpAddress, (const char*)dataFromContainer + 55, 16);

				if (gUtil.nLimit <= 0)
				{
					result = 1;
					goto EndProc01;
				}

#if (USE_LIMIT==0)

#if (_DEBUG_SQLITE==1)
				printf("HardwareId: %s\n", (const char*)dataFromContainer);
				printf("WanAddress: %s\n", (const char*)dataFromContainer + 33);
				//printf("HardwareId: %s\n", (const char*)dataFromContainer + 51);
				//printf("HardwareId: %s\n", (const char*)dataFromContainer + 23);
#endif
				//gUtil.WriteDatabase("Database - Info: Account: [ %s ], HardwareId: [ %s ], MacAddress: [ %s ], IpAddress: [ %s ]\n", AccountName, (char*)dataFromContainer, (const char*)dataFromContainer + 37, (const char*)dataFromContainer + 55);
				memset(&sqlCount, 0, sizeof(sqlCount));

				//sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cAccountName = '%s' and cHardwareId = '%s';", AccountName, (char*)dataFromContainer);
				sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cAccountName = '%s' and cHardwareId = '%s' and cIpAddress = '%s';", AccountName, (char*)dataFromContainer, (const char*)dataFromContainer + 33);

				rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, &zErrMsg);

				if (rc != SQLITE_OK) {

#if (_DEBUG_SQLITE==1)
					gUtil.WriteDatabase("Database - Error COUNT(*): %s\n",zErrMsg);
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif // (_DEBUG_SQLITE==1)
					sqlite3_free(zErrMsg);
					result = 0;
				}
				else {

#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQLite Rows: %d\n", count);
#endif
					/*if (count >= gUtil.nLimit)
					{
						#if (_DEBUG_SQLITE==1)
							gUtil.WriteDatabase("Database - Error Limited: %s\n", zErrMsg);
							fprintf(stderr, "Limited \n");
						#endif
						sqlite3_free(zErrMsg);
						result = false;
						goto EndProc01;
					}
					else
					{
						result = true;
						goto EndProc01;
					}*/


					if (count <= 0)
					{

						memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

						sprintf((char*)sqlInsertAccount, (const char*)"INSERT INTO HardwareId (cAccountName, cHardwareId, cIpAddress, nCount) VALUES ('%s','%s','%s', 1);", AccountName, (const char*)dataFromContainer, (const char*)dataFromContainer + 33);

						rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

						if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
							gUtil.WriteDatabase("Database - Error INTO: %s\n", zErrMsg);
							fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
							sqlite3_free(zErrMsg);
							result = 0;
						}
						else
						{
							result = 1;
						}
					}
					else
					{
						result = count;
					}
//
//					memset(&sqlCount, 0, sizeof(sqlCount));
//
//					sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cHardwareId = '%s';", (char*)dataFromContainer);
//
//					rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count2, &zErrMsg);
//
//					if (rc != SQLITE_OK) {
//
//#if (_DEBUG_SQLITE==1)
//						gUtil.WriteDatabase("Database - Error COUNT(*): %s\n", zErrMsg);
//						fprintf(stderr, "SQL error: %s\n", zErrMsg);
//#endif // (_DEBUG_SQLITE==1)
//						sqlite3_free(zErrMsg);
//						result = 0;
//						goto EndProc01;
//					}
//					else
//					{
//						if (count > 0)
//							result = count;
//						else
//							result = 0;
//					}
//					else
//					{
//
//						memset(&sqlCount, 0, sizeof(sqlCount));
//
//						sprintf((char*)sqlCount, (const char*)"SELECT nCount FROM HardwareId WHERE AccountName = '%s' and cHardwareId = '%s';", AccountName, (const char*)dataFromContainer);
//
//						rc = sqlite3_exec(db, sqlCount, callback_data_entergame, &nCount, &zErrMsg);
//#if (_DEBUG_SQLITE==1)
//						fprintf(stderr, "nCount: %d / %d\n", nCount, gUtil.nLimit);
//#endif
//						if (nCount >= gUtil.nLimit)
//						{
//#if (_DEBUG_SQLITE==1)
//							gUtil.WriteDatabase("Database - Error Limited: %s\n", zErrMsg);
//							fprintf(stderr, "Limited \n");
//#endif
//							sqlite3_free(zErrMsg);
//							//result = false;
//							return false;
//						}
//						else
//						{
//
//							memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));
//
//							sprintf((char*)sqlInsertAccount, (const char*)"UPDATE HardwareId set nCount = nCount + 1 WHERE cHardwareId = '%s';", (const char*)dataFromContainer);
//
//							rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);
//
//							if (rc != SQLITE_OK) {
//#if (_DEBUG_SQLITE==1)
//								gUtil.WriteDatabase("Database - Error UPDATE: %s\n", zErrMsg);
//								fprintf(stderr, "SQL error: %s\n", zErrMsg);
//#endif
//								sqlite3_free(zErrMsg);
//								return false;
//							}
//							else {
//#if (_DEBUG_SQLITE==1)
//								fprintf(stdout, "Update enter game done successfully\n");
//#endif
//								sqlite3_free(zErrMsg);
//								return true;
//							}
//						}
//
//					}
					//fprintf(stdout, "Table created successfully\n");
				}
#endif
			}
		EndProc01:

			free(dataFromContainer);
#if (_DEBUG_SQLITE==1)
			fprintf(stdout, "Operation enter game done successfully\n");
#endif
		}
	EndProc:
		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;
	}
	catch (...)
	{
		//sqlite3_close(db);
		//pthread_mutex_unlock(&cs_mutex);
		return 0;
	}
	
}

void Sqlite_UpdateRoleInfo_On_EnterGame(const char* AccountName, const char* pRoleName, int nLevel, INT64 nPlayerExp)
{

	bool result;
	char* zErrMsg = 0;
	int rc;
	char sqlUpdateRoleInfo[512];
	int count = 0;
	int nCount = 0;
	BYTE BuffData[36];

	try
	{
		memset(&sqlUpdateRoleInfo, 0, sizeof(sqlUpdateRoleInfo));

		sprintf((char*)sqlUpdateRoleInfo, (const char*)"UPDATE Account set RoleName = '%s', PlayerLevel = %d, PlayerExp = %lld WHERE AccountName= '%s';", pRoleName, nLevel, nPlayerExp, (const char*)AccountName);

		sqlite3_exec(db, sqlUpdateRoleInfo, 0, 0, 0);

	}
	catch (...)
	{
	}

}

void Sqlite_BackNumber_On_Kichout_ByLimit(const char* cHardwareId, const char* cMacAddress, const char* cIpAddress)
{

	bool result;
	char* zErrMsg = 0;
	int rc;
	char sqlUpdateRoleInfo[512];
	int count = 0;
	int nCount = 0;
	BYTE BuffData[36];

	try
	{
		memset(&sqlUpdateRoleInfo, 0, sizeof(sqlUpdateRoleInfo));

		sprintf((char*)sqlUpdateRoleInfo, (const char*)"UPDATE HardwareId set nCount = nCount + 1 WHERE cHardwareId = '%s' and cIpAddress= '%s';", cHardwareId, cIpAddress);

		sqlite3_exec(db, sqlUpdateRoleInfo, 0, 0, 0);

	}
	catch (...)
	{
	}

}

void Sqlite_UpdateRoleInfo_WorldStat(const char* AccountName, int nWorldStat)
{

	bool result;
	char* zErrMsg = 0;
	int rc;
	char sqlUpdateRoleInfo[512];
	int count = 0;
	int nCount = 0;
	BYTE BuffData[36];

	try
	{
		memset(&sqlUpdateRoleInfo, 0, sizeof(sqlUpdateRoleInfo));

		sprintf((char*)sqlUpdateRoleInfo, (const char*)"UPDATE Account set WorldStat = %d WHERE AccountName= '%s';", nWorldStat, (const char*)AccountName);

		sqlite3_exec(db, sqlUpdateRoleInfo, 0, 0, 0);

	}
	catch (...)
	{
	}
}

void Sqlite_UpdateRoleInfo_Level_On_EnterGame(const char* AccountName, int nLevel)
{

	bool result;
	char* zErrMsg = 0;
	int rc;
	char sqlUpdateRoleInfo[512];
	int count = 0;
	int nCount = 0;
	BYTE BuffData[36];

	try
	{
		memset(&sqlUpdateRoleInfo, 0, sizeof(sqlUpdateRoleInfo));

		sprintf((char*)sqlUpdateRoleInfo, (const char*)"UPDATE Account set PlayerLevel = %d WHERE AccountName= '%s';", nLevel, (const char*)AccountName);

		sqlite3_exec(db, sqlUpdateRoleInfo, 0, 0, 0);

	}
	catch (...)
	{
	}
}

bool Sqlite_Check_HardwareId_GS(const char* HardwareId, const char* IpAddress)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		int nCount = 0;
		//char BuffData[32];

		result = false;

		//pthread_mutex_lock(&cs_mutex);

		Container* c = ac_init();

		/*rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
		}*/

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cHardwareId = '%s' and cIpAddress = '%s';", HardwareId, IpAddress);

		rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, &zErrMsg);

		if (rc != SQLITE_OK) {

#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif // (_DEBUG_SQLITE==1)
			sqlite3_free(zErrMsg);
			goto EndProc;
		}
		else {

#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQLite Rows: %d\n", count);
#endif

			if (count == 0)
			{

				memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

				sprintf((char*)sqlInsertAccount, (const char*)"INSERT INTO HardwareId (cHardwareId, cIpAddress, nCount) VALUES ('%s', '%s', %d);", HardwareId, IpAddress, 0);

				rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Records created successfully\n");
#endif
					result = true;
				}

			}
			else
			{

				memset(&sqlCount, 0, sizeof(sqlCount));

				sprintf((char*)sqlCount, (const char*)"SELECT nCount FROM HardwareId WHERE cHardwareId = '%s' and cIpAddress = '%s';", HardwareId, IpAddress);

				rc = sqlite3_exec(db, sqlCount, callback_data_entergame, &nCount, &zErrMsg);
#if (_DEBUG_SQLITE==1)
				fprintf(stderr, "nCount: %d / %d\n", nCount, gUtil.nLimit);
#endif
				//fprintf(stderr, "nCount: %d / %d\n", nCount, gUtil.nLimit);

				if (nCount >= gUtil.nLimit)
				{
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "Limited \n");
#endif
					sqlite3_free(zErrMsg);
					result = false;
					goto EndProc;
				}
				else
				{
					result = true;
				}

			}
			//fprintf(stdout, "Table created successfully\n");
		}

	EndProc:

		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;

	}
	catch (...)
	{
		//pthread_mutex_unlock(&cs_mutex);
		return false;
	}
}

bool Sqlite_GetAndInsert_HardwareId_On_EnterGame_Gs(const char* cHardwareId, const char* cMacAddress, const char* cIpAddress)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);
	//std::cout << "Opened database successfully" << std::endl;
	try
	{
		bool result;
		//const char* cHardwareId;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		int nCount = 0;
		BYTE BuffData[72];

		//pthread_mutex_init(&cs_mutex, &mAttr);

		Container* c = ac_init();

		result = false;

#if (USE_LIMIT==0)

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cHardwareId = '%s' and cIpAddress = '%s';", (char*)cHardwareId, (char*)cIpAddress);

		rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, &zErrMsg);

		if (rc != SQLITE_OK) {

#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif // (_DEBUG_SQLITE==1)
			sqlite3_free(zErrMsg);
			goto EndProc01;
		}
		else {

#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQLite Rows: %d\n", count);
#endif
			if (count == 0)
			{

				memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

				sprintf((char*)sqlInsertAccount, (const char*)"INSERT INTO HardwareId (cHardwareId, cMacAddress, cIpAddress, nCount) VALUES ('%s', '%s', '%s', 1);", (char*)cHardwareId, (char*)cMacAddress, (char*)cIpAddress);

				rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc01;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Records created successfully\n");
#endif
					sqlite3_free(zErrMsg);
					return true;
				}
		}
			else
			{

				memset(&sqlCount, 0, sizeof(sqlCount));

				sprintf((char*)sqlCount, (const char*)"SELECT nCount FROM HardwareId WHERE cHardwareId = '%s' and cIpAddress = '%s';", (char*)cHardwareId, (char*)cIpAddress);

				rc = sqlite3_exec(db, sqlCount, callback_data_entergame, &nCount, &zErrMsg);
#if (_DEBUG_SQLITE==1)
				fprintf(stderr, "nCount: %d / %d\n", nCount, gUtil.nLimit);
#endif
				if (nCount >= gUtil.nLimit)
				{
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "Limited \n");
#endif
					sqlite3_free(zErrMsg);
					return false;
				}
				else
				{

					memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));

					sprintf((char*)sqlInsertAccount, (const char*)"UPDATE HardwareId set nCount = nCount + 1 WHERE cHardwareId= '%s' and cIpAddress = '%s';", (char*)cHardwareId, (char*)cIpAddress);

					rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, &zErrMsg);

					if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
						fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
						sqlite3_free(zErrMsg);
						return false;
					}
					else {
#if (_DEBUG_SQLITE==1)
						fprintf(stdout, "Update enter game done successfully\n");
#endif
						sqlite3_free(zErrMsg);
						return true;
					}
				}

			}
			//fprintf(stdout, "Table created successfully\n");
	}
#endif
		EndProc01:

#if (_DEBUG_SQLITE==1)
			fprintf(stdout, "Operation enter game done successfully\n");
#endif

	EndProc:
		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;
	}
	catch (...)
	{
		//sqlite3_close(db);
		//pthread_mutex_unlock(&cs_mutex);
		return false;
	}

}

bool Sqlite_GetAndInsert_HardwareId_On_LeaveGame_Gs(const char* cHardwareId, const char* cMacAddress, const char* cIpAddress)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);
	//std::cout << "Opened database successfully" << std::endl;
	try
	{
		bool result;
		const char* cHardwareId;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		int nCount = 0;
		BYTE BuffData[72];

		//pthread_mutex_init(&cs_mutex, &mAttr);

		Container* c = ac_init();

		result = false;

#if (USE_LIMIT==0)

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"UPDATE HardwareId set nCount = nCount - 1 WHERE nCount > 0 and cHardwareId= '%s' and cIpAddress = '%s';", (char*)cHardwareId, (char*)cIpAddress);

		rc = sqlite3_exec(db, sqlCount, callback, 0, &zErrMsg);

		if (rc != SQLITE_OK) {
			sqlite3_free(zErrMsg);
			return false;
		}
		else {
			return true;
		}
#endif
		return false;
	}
	catch (...)
	{
		//sqlite3_close(db);
		//pthread_mutex_unlock(&cs_mutex);
		return false;
	}

}

bool Sqlite_Insert_IpAddr_From_Login(const char* AccountName, const char* IpAddr)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;

		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[256];
		char sqlInsertIpAddr[256];
		int rc = 0;
		int count = 0;

		//pthread_mutex_lock(&cs_mutex);

		result = false;

		/*rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
		}*/

		//std::cout << "HardwareId: " << HardwareSerialId << std::endl;
		//std::cout << "MacAddress: " << MacAddress << std::endl;
		//std::cout << "IpAddress: " << IpAddress << std::endl;

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM IpAddrClient WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sqlCount, callback_cound, &count, &zErrMsg);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQLite Rows: %d\n", count);
#endif
			if (count <= 0)
			{
				sprintf((char*)sqlInsertIpAddr, (const char*)"UPDATE Account set WanAddress = '%s' WHERE AccountName='%s';", IpAddr, AccountName);

				rc = sqlite3_exec(db, sqlInsertIpAddr, callback, 0, &zErrMsg);

				sprintf((char*)sqlInsertIpAddr, (const char*)"INSERT INTO IpAddrClient (AccountName,IpAddr) VALUES ('%s', '%s');", AccountName, IpAddr);

				rc = sqlite3_exec(db, sqlInsertIpAddr, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Records created successfully\n");
#endif
					result = true;
				}

			}
			else
			{

				sprintf((char*)sqlInsertIpAddr, (const char*)"UPDATE Account set WanAddress = '%s' WHERE AccountName='%s';", IpAddr, AccountName);

				rc = sqlite3_exec(db, sqlInsertIpAddr, callback, 0, &zErrMsg);

				sprintf((char*)sqlInsertIpAddr, (const char*)"UPDATE IpAddrClient set IpAddr = '%s' WHERE AccountName='%s';", IpAddr, AccountName);

				/* Execute SQL statement */
				rc = sqlite3_exec(db, sqlInsertIpAddr, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Update done successfully\n");
#endif
					result = true;
				}
			}
			//fprintf(stdout, "Table created successfully\n");
		}
	EndProc:
		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;
	}
	catch (...)
	{
		//pthread_mutex_unlock(&cs_mutex);
		return false;
	}

}

bool Sqlite_Insert_IpAddr_JoinGS(const char* AccountName, const char* IpAddr)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;

		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[256];
		char sqlInsertIpAddr[256];
		int rc = 0;
		int count = 0;

		//pthread_mutex_lock(&cs_mutex);

		result = false;

		/*rc = sqlite3_open(DATABASE_LIMITNAME, &db);

		if (rc) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
#endif
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "Opened database successfully\n");
#endif
		}*/

		//std::cout << "HardwareId: " << HardwareSerialId << std::endl;
		//std::cout << "MacAddress: " << MacAddress << std::endl;
		//std::cout << "IpAddress: " << IpAddress << std::endl;

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM IpAddrClient WHERE AccountName = '%s' and IpAddr = '%s';", AccountName, IpAddr);

		rc = sqlite3_exec(db, sqlCount, callback_cound, &count, &zErrMsg);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			goto EndProc;
		}
		else {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQLite Rows: %d\n", count);
#endif
			if (count <= 0)
			{

				sprintf((char*)sqlInsertIpAddr, (const char*)"INSERT INTO IpAddrClient (AccountName,IpAddr) VALUES ('%s', '%s');", AccountName, IpAddr);

				rc = sqlite3_exec(db, sqlInsertIpAddr, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Records created successfully\n");
#endif
					result = true;
				}

			}
			else
			{

				sprintf((char*)sqlInsertIpAddr, (const char*)"UPDATE IpAddrClient set IpAddr = '%s' WHERE AccountName='%s';", IpAddr, AccountName);

				/* Execute SQL statement */
				rc = sqlite3_exec(db, sqlInsertIpAddr, callback, 0, &zErrMsg);

				if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
					sqlite3_free(zErrMsg);
					goto EndProc;
				}
				else {
#if (_DEBUG_SQLITE==1)
					fprintf(stdout, "Update done successfully\n");
#endif
					result = true;
				}
			}
			//fprintf(stdout, "Table created successfully\n");
		}
	EndProc:
		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;
	}
	catch (...)
	{
		//pthread_mutex_unlock(&cs_mutex);
		return false;
	}

}

bool Sqlite_Check_WanAddress_GS(const char* IpAddress)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		//char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[128];
		//char sqlInsertAccount[512];
		int count = 0;
		//int nCount = 0;
		//char BuffData[32];

		result = false;

		Container* c = ac_init();

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM IpAddrClient WHERE IpAddr = '%s';", IpAddress);

		rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, 0);

		if (rc != SQLITE_OK) {

#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif // (_DEBUG_SQLITE==1)
			return false;
		}
		else {

#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQLite Rows: %d\n", count);
#endif

			if (count <= 0)
			{
				return false;
			}
			else
			{
				return true;
			}
			//fprintf(stdout, "Table created successfully\n");
		}

	}
	catch (...)
	{
		//pthread_mutex_unlock(&cs_mutex);
		return false;
	}
}

bool Sqlite_Check_IpAddress(const char* IpAddress)
{
	try
	{
		bool result;
		int rc;
		char sqlCount[128];
		int count = 0;
		result = false;

		Container* c = ac_init();

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM IpAddrClient WHERE IpAddr = '%s';", IpAddress);

		rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, 0);

		if (rc != SQLITE_OK) {
			return false;
		}
		else {
			if (count <= 0)
			{
				return true;
			}
			else
			{
				if (count > gUtil.nLimitIP)
					return false;
				else
				{
					return true;
				}
				return true;
			}
		}
	}
	catch (...)
	{
		return false;
	}
}

bool Sqlite_Remove_IP_On_Outgame(const char* AccountName)
{
	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		char sqlCount[512];
		int count = 0;
		result = false;

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"DELETE FROM IpAddrClient WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sqlCount, 0, 0, 0);

		return true;

	}
	catch (...)
	{
		return false;
	}
}

bool Sqlite_Update_HardwareId(const char* AccountName, const char* HardwareId, const char* MacAddress, const char* IpAddress)
{
	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		char sql[512];
		int count = 0;
		result = false;
		Container* c = ac_init();

		memset(&sql, 0, sizeof(sql));

		sprintf((char*)sql, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cHardwareId = '%s' and cMacAddress = '%s';", AccountName, HardwareId, MacAddress);

		rc = sqlite3_exec(db, (const char*)sql, callback_cound, &count, &zErrMsg);

		if (count <= 0)
		{
			memset(&sql, 0, sizeof(sql));
			sprintf((char*)sql, (const char*)"INSERT INTO HardwareId (cHardwareId, cMacAddress, nCount) VALUES ('%s','%s','%s', 0);", AccountName, HardwareId, MacAddress);
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		}
		/*else
		{
			memset(&sql, 0, sizeof(sql));
			sprintf((char*)sql, (const char*)"UPDATE HardwareId set cHardwareId = '%s', set cMacAddress = '%s' WHERE cAccountName='%s';", HardwareId, MacAddress, AccountName);
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		}*/
		return true;
	}
	catch (...)
	{
		return false;
	}
}

bool Sqlite_Update_WanAddress(const char* AccountName, const char* IpAddress)
{
	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		char sql[256];
		int count = 0;
		BYTE BuffData[72];
		Container* c = ac_init();

		memset(&sql, 0, sizeof(sql));

		sprintf((char*)sql, (const char*)"SELECT HardwareId,MacAddress,WanAddress FROM Account WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sql, callback_HardwareId_entergame, (void*)BuffData, &zErrMsg);

		ac_put(c, (unsigned char*)BuffData, sizeof(BuffData) + 1);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			return false;
		}
		else
		{

			unsigned char* dataFromContainer;
			size_t contDataSz = ac_get(c, &dataFromContainer);
			dataFromContainer[contDataSz - 1] = 0;

			memset(&sql, 0, sizeof(sql));
			sprintf((char*)sql, (const char*)"SELECT COUNT(*) FROM HardwareId WHERE cAccountName = '%s' and cHardwareId = '%s' and cIpAddress = '%s';", AccountName, (const char*)dataFromContainer, (const char*)dataFromContainer + 51);
			rc = sqlite3_exec(db, (const char*)sql, callback_cound, &count, &zErrMsg);

			if (count <= 0)
			{
				memset(&sql, 0, sizeof(sql));
				sprintf((char*)sql, (const char*)"INSERT INTO HardwareId (cAccountName, cHardwareId, cIpAddress, nCount) VALUES ('%s', '%s', '%s', %d);", AccountName, (const char*)dataFromContainer, (const char*)dataFromContainer + 51, 1);
				rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			}
			free(dataFromContainer);
		}

		

		/*result = false;

		memset(&sql, 0, sizeof(sql));

		sprintf((char*)sql, (const char*)"UPDATE HardwareId set cIpAddress = '%s' WHERE cAccountName='%s';", IpAddress, AccountName);

		rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);*/

		return true;

	}
	catch (...)
	{
		return true;
	}
}

int Sqlite_GetHardwareIdByAccount(const char* AccountName, const void* HardwareId, int nType)
{
	try
	{
		int result;
		const char* cHardwareId;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[128];
		char sqlInsertAccount[128];
		int count = 0;
		int count2 = 0;
		int nCount = 0;
		BYTE BuffData[72];

		char* dataType;

		if (nType == 0)
			dataType = "CodeCheck";
		else if (nType == 1)
			dataType = "MacAddress";
		else if (nType == 2)
			dataType = "IpAddress";
		else if (nType == 3)
			dataType = "HardwareId,WanAddress";

		Container* c = ac_init();

		result = 0;

		char sql[512];

		rc = sqlite3_exec(db, "SELECT Value FROM Option WHERE Name = 'GameLimit';", callback_data_entergame, &count, &zErrMsg);

		if (rc == SQLITE_OK)
		{
			if (count > 0)
			{
				gUtil.nLimit = count;
			}
			else
				gUtil.nLimit = 0;
		}

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT %s FROM Account WHERE AccountName = '%s';", dataType, AccountName);

		rc = sqlite3_exec(db, sqlCount, callback_HardwareId_entergame, (void*)BuffData, &zErrMsg);

		ac_put(c, (unsigned char*)BuffData, sizeof(BuffData) + 1);

		if (rc != SQLITE_OK) {
#if (_DEBUG_SQLITE==1)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
#endif
			sqlite3_free(zErrMsg);
			return 0;
		}
		else {

			unsigned char* dataFromContainer;
			size_t contDataSz = ac_get(c, &dataFromContainer);
			if (contDataSz == 0)
			{
#if (_DEBUG_SQLITE==1)
				fprintf(stderr, "contDataSz: 0\n");
				sqlite3_free(zErrMsg);
#endif
				sqlite3_free(zErrMsg);
				return 0;
			}
			else
			{
				dataFromContainer[contDataSz - 1] = 0;
				memcpy((void*)(HardwareId), dataFromContainer, 72);
				return 1;
			}
		EndProc01:

			free(dataFromContainer);
#if (_DEBUG_SQLITE==1)
			fprintf(stdout, "Operation enter game done successfully\n");
#endif
		}
	EndProc:
		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;
	}
	catch (...)
	{
		//sqlite3_close(db);
		//pthread_mutex_unlock(&cs_mutex);
		return 0;
	}

}

static int callback_Loop_Row(void* data, int argc, char** argv, char** azColName) {
	
	
	int i;

	//char* c = (char*)data;

	gKPlayerNew.m_nRankIndex++;

	char* c = (char*)data;

	//fprintf(stderr, "contDataSz: 0\n");
	//*c = *(char*)argv[0];
	//strcpy(&c[0], argv[0]);
	//strcpy(c, argv[0]);
	//c[36] = 0;
	//printf("%s\n", c);
	//strcpy(&c[37], argv[1]);
	//c[54] = '\0';
	//printf("%s\n", c[37]);
	//strcpy(&c[55], argv[2]);
	//sprintf(c, (const char*)"%s%s%s", argv[0], argv[1], argv[2]);
	//c[72] = '\0';
	//printf("%s\n", c);
	//strcpy(&c[23], argv[1]);
	gKPlayerNew.WorldRank_Data.insert(pair<std::string, int>(argv[0], gKPlayerNew.m_nRankIndex));
	Sqlite_UpdateRoleInfo_WorldStat(argv[0], gKPlayerNew.m_nRankIndex);

	//for (i = 0; i < argc; i++)
	//{

		/*if (i == 0)
		{
			strcpy(c, argv[0]);
			c[32] = 0;
		}
		else if (i == 1)
		{
			strcpy(&c[33], argv[1]);
			c[50] = 0;
		}
		else if (i == 2)
		{
			strcpy(&c[51], argv[2]);
			c[72] = 0;
		}
		else
			break;*/

		//printf("[%d] %s = %s\n", i, azColName[i], argv[i] ? argv[i] : "NULL");

	//}

	//printf("[%s] Rank %d \n", argv[0], gKPlayerNew.m_nRankIndex);

	//printf("\n");
	return 0;
}

int Sqlite_Sort_Top_Data()
{
	try
	{
		int result;

		char* zErrMsg = 0;

		int rc;

		char sqlCount[256];

		int nCount = 0;

		BYTE BuffData[72];

		char* dataType;

		Container* c = ac_init();

		result = 0;

		gKPlayerNew.m_nRankIndex = 10;

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT AccountName, RoleName, PlayerLevel, PlayerExp FROM (SELECT AccountName, RoleName, PlayerLevel, PlayerExp FROM Account WHERE PlayerLevel > 0 and RoleName != '' ORDER BY PlayerLevel DESC LIMIT 999) ORDER BY PlayerExp DESC;");

		rc = sqlite3_exec(db, sqlCount, callback_Loop_Row, (void*)BuffData, 0);

		//ac_put(c, (unsigned char*)BuffData, sizeof(BuffData) + 1);

		//if (rc != SQLITE_OK) {
		//}
		//else {

		//	unsigned char* dataFromContainer;

		//	size_t contDataSz = ac_get(c, &dataFromContainer);

		//	if (contDataSz == 0)
		//	{
		//	}
		//	else
		//	{

		//		dataFromContainer[contDataSz - 1] = 0;

		//		//memcpy((void*)(HardwareId), dataFromContainer, 72);
		//		
		//	}
		//free(dataFromContainer);
		//}

	}
	catch (...)
	{
		return 0;
	}
}

int Sqlite_GetHardwareIdByAccount(const char* AccountName)
{
	try
	{
		int rc;

		char sql[128];

		int nCount = 0;

		Container* c = ac_init();

		memset(&sql, 0, sizeof(sql));

		sprintf((char*)sql, (const char*)"SELECT WorldStat FROM Account WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sql, callback_data_entergame, &nCount, 0);

		//printf("caccname: %s\n", AccountName);
		//printf("nCount: %d\n", nCount);

		return nCount;
	}
	catch (...)
	{
		return 0;
	}

}

bool Sqlite_CheckFaction_IpAddress(const char* IpAddress, int nFaction, int nLimit)
{
	try
	{
		bool result;
		int rc;
		char sqlCount[128];
		int count = 0;
		result = false;

		Container* c = ac_init();

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM FactionData WHERE WanAddress = '%s' and CurFaction = %d;", IpAddress, nFaction);

		rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, 0);

		if (rc != SQLITE_OK) {
			return false;
		}
		else {
			if (count <= 0)
			{
				return true;
			}
			else
			{
				if (count > nLimit)
					return false;
				else
				{
					return true;
				}
				return true;
			}
		}
	}
	catch (...)
	{
		return false;
	}
}

bool Sqlite_CheckFaction(const char* Account, const char* IpAddress, int nFaction, int nLimit)
{
	//pthread_mutex_t cs_mutex;
	//pthread_mutex_init(&cs_mutex, NULL);

	try
	{
		bool result;
		char* zErrMsg = 0;
		int rc;
		//char* sqlCount;
		//char* sqlInsertAccount;
		char sqlCount[512];
		char sqlInsertAccount[512];
		int count = 0;
		int nCount = 0;
		//char BuffData[32];

		result = false;

		Container* c = ac_init();

		memset(&sqlCount, 0, sizeof(sqlCount));

		sprintf((char*)sqlCount, (const char*)"SELECT COUNT(*) FROM FactionData WHERE WanAddress = '%s' and CurFaction = %d;", IpAddress, nFaction);

		rc = sqlite3_exec(db, (const char*)sqlCount, callback_cound, &count, 0);

		if (rc != SQLITE_OK) {
			
			result = false;
		}
		else {
			//fprintf(stderr, "Count Faction : %d\n", count);
			if (count < nLimit)
			{

				memset(&sqlInsertAccount, 0, sizeof(sqlInsertAccount));
				//fprintf(stderr, "MacFromString : %d\n", (int)gUtil.string_to_mac(MacAddress));
				//fprintf(stderr, "IpFromString : %d\n", (int)Sqlite_IpFromString(IpAddress));
				//uint32_t IpAddr = (int)Sqlite_IpFromString(IpAddress);
				sprintf((char*)sqlInsertAccount, (const char*)"INSERT INTO FactionData (AccountName, WanAddress, CurFaction) VALUES ('%s','%s', %d);", Account, IpAddress, nFaction);

				rc = sqlite3_exec(db, sqlInsertAccount, callback, 0, 0);

				if (rc != SQLITE_OK) {
					return false;
				}
				else {
					return true;
				}

			}
			else
			{
				return false;
			}
			//fprintf(stdout, "Table created successfully\n");
		}

	EndProc:

		//sqlite3_close(db);

		//pthread_mutex_unlock(&cs_mutex);

		return result;

	}
	catch (...)
	{
		//pthread_mutex_unlock(&cs_mutex);
		return true;
	}

}

void Sqlite_FactionRemove(const char* AccountName)
{
	try
	{
		int rc;

		char sql[128];

		int nCount = 0;

		Container* c = ac_init();

		memset(&sql, 0, sizeof(sql));

		sprintf((char*)sql, (const char*)"DELETE FROM FactionData WHERE AccountName = '%s';", AccountName);

		rc = sqlite3_exec(db, sql, callback, 0, 0);

	}
	catch (...)
	{
	}

}

static int callback_query(void* data, int argc, char** argv, char** azColName) {
	int i;

	char* c = (char*)data;

	std::string mydata = c;

	mydata.append("{");
	
	for (i = 0; i < argc; i++)
	{
		mydata.append("\"");
		mydata.append(azColName[i]);
		mydata.append("\"");
		mydata.append("=");
		mydata.append("\"");
		mydata.append(argv[i]);
		mydata.append("\"");
	}

	mydata.append("}");

	return 0;
}

string Sqlite_Query(const char* szQuery)
{
	try
	{
		int rc;
		int nCount = 0;
		char* zErrMsg = 0;
		BYTE BuffData[1024];
		Container* c = ac_init();
		std::string sQuery = szQuery;

		if (sQuery.find("INSERT") != std::string::npos || sQuery.find("UPDATE") != std::string::npos || sQuery.find("DELETE") != std::string::npos)
		{
			rc = sqlite3_exec(db, szQuery, NULL, NULL, &zErrMsg);
			if (rc != SQLITE_OK)
			{
				sqlite3_free(zErrMsg);
				return std::string("ERROR");
			}
			else
			{
				return std::string("OK");
			}
		}
		else if (sQuery.find("SELECT") != std::string::npos)
		{
			if (sQuery.find("COUNT") != std::string::npos)
			{
				int count = 0;
				rc = sqlite3_exec(db, szQuery, callback_cound, &count, &zErrMsg);
				if (rc != SQLITE_OK)
				{
					sqlite3_free(zErrMsg);
					return std::string("ERROR");
				}
				else
				{
					char nCount[10];
					memset(&nCount, 0, sizeof(nCount));
					sprintf((char*)nCount, (const char*)"%d", nCount);
					return std::string(nCount);
				}
			}
			else
			{
				sqlite3_stmt* result;
				rc = sqlite3_exec(db, szQuery, callback_query, (void*)BuffData, &zErrMsg);
				ac_put(c, (unsigned char*)BuffData, sizeof(BuffData) + 1);
				if (rc != SQLITE_OK)
				{
					sqlite3_free(zErrMsg);
					return std::string("ERROR");
				}
				else
				{
					unsigned char* dataFromContainer;
					size_t contDataSz = ac_get(c, &dataFromContainer);
					if (contDataSz == 0)
					{
						sqlite3_free(zErrMsg);
						return std::string("ERROR");
					}
					else
					{
						dataFromContainer[contDataSz - 1] = 0;
						return std::string((char*)dataFromContainer);
					}
				}
			}
		}
		return std::string("Empty");
	}
	catch (...)
	{
		return std::string("Error");
	}
}
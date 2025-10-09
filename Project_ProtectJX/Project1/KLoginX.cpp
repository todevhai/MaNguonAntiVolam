#include "stdafx.h"
#include "Util.h"
#include <sys/mman.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdint>
#include <unistd.h>
#include "KLoginX.h"
#include <arpa/inet.h>
//#include <boost/asio/ip/tcp.hpp>
#include <iostream>
//#include <boost/algorithm/string/split.hpp>
//#include <boost/algorithm/string.hpp>
//#include <boost/asio/placeholders.hpp>
//#include <sys/mman.h>
//#include <sys/sysctl.h>
//#include <sys/utsname.h>
//#include <sys/stat.h>
//#include <sys/mman.h>
//#include "subhook.c"
#include <stdio.h>
//#include <string.h>
#include <linux/string.h>
#include <unistd.h>
#include <iostream>
#include <cstdarg>
#include <thread>
#include <cassert>
#include <cstring>
#include <assert.h>
#include <fstream>
#include <vector>
#include "Authenticate.h"
#include "Sqlite.h"
//#include "bishop_y.h"
//#include <stdlib.h>

using namespace std;

//void* malloc(size_t size);

KLoginX gKLoginX;
CGamePlayer gGamePlayer;
CIntercessor* vptr_CIntercessor;
_CGamePlayer Execute_CheckLoginPermission = (_CGamePlayer)0x804C1B6;
_VerifyAccountToPlayer ExecuteVerifyAccountToPlayer = (_VerifyAccountToPlayer)0x804D40A;
_IsAccountLocked ExecuteIsAccountLocked = (_IsAccountLocked)0x806802A;
_CheckAccountName ExecuteCheckAccountName = (_CheckAccountName)0x8067E54;
_MsgQueue_Pop ExecuteMsgQueue_Pop = (_MsgQueue_Pop)0x8074636;

pthread_mutex_t waitlock;

//#define KMsgQueue_Pop		((const void*(__cdecl*)(LPVOID This, BYTE byProtocol, size_t *const nLen)) 0x8074636)//0x0063A578 
#define ExecutePrepareLogin		((void*(__cdecl*)(CGamePlayer * This, KAccountUserLoginInfo* pBuffer, DWORD dwIP, DWORD dwPort)) 0x804C1E6)//0x0063A578 
#define ExecuteReconnectableClientSend		((bool(__cdecl*)(LPVOID This, const void * const pData, const size_t &datalength)) 0x8055430)//0x0063A578 
#define Executec2sCheckProtocol		((bool(__cdecl*)(CIntercessor* This, c2s_PROTOCOL nProtocol, const BYTE *pMsg, size_t size)) 0x805A852)//0x0063A578 
#define Execute_GetAccountInMap		((bool(__cdecl*)(LPCSTR pcszAccount, Account *pAccount)) 0x806A7C8)//0x0063A578 
#define Execute_SimpleThreadClose		((void(__cdecl*)(KSimpleThread * This)) 0x80699FC)//0x0063A578 
#define Execute_SyncRoleInfo		((bool(__cdecl*)(CGameServer *This, const void *pData, size_t datalength, void *pvData)) 0x8057020)//0x0063A578 
#define Execute_SendData		((bool(__cdecl*)(CGameServer *This, const void *pData, size_t nLen)) 0x805590A)//0x0063A578 
#define Execute_PushAccount		((bool(__cdecl*)(CGameServer *, const char *)) 0x8056656)//0x0063A578 
#define Execute_PushData		((bool(__cdecl*)(LPCSTR* This, const void* pData, size_t nLen)) 0x80817D0)//0x0063A578 
#define Execute_Account		((void(__cdecl*)(Account *This)) 0x8050C1A)//0x0063A578 
#define Execute_AllocTmpBuffer		((char*(__cdecl*)(LPVOID* This, size_t nLen)) 0x80680B6)//0x0063A578 
#define Execute_KConsole_ProcessCmd		((void(__cdecl*)(KConsole *const This, LPCSTR pszCmd)) 0x806206E)//0x0063A578 
#define Execute_GetServer		((CGameServer*(__cdecl*)(CIntercessor *, size_t)) 0x805AB0A)//0x0063A578 
#define Execute_FreePlayer		((void(__cdecl*)(CGamePlayer *This)) 0x804F338)//0x0063A578 
#define Execute_Shutdown		((void(__cdecl*)(CGamePlayer *This)) 0x804B7F8)//0x0063A578 
#define Execute_SmallPackProcess		((bool(__cdecl*)(CGameServer *const This, BYTE cProtocol, const void *pData, size_t nLen)) 0x8055590)//0x0063A578 
#define Execute_NotifyEnterGame		((bool(__cdecl*)(CGameServer * This, const void *, size_t datalength)) 0x805749A)//0x0063A578 
#define Execute_OfflineRequestAsk		((bool(__cdecl*)(CGameServer *, const void *pData, size_t )) 0x80567D4)//0x0063A578 
#define Execute_AddPlayer		((bool(__cdecl*)(CIntercessor *This, LPCSTR pszRole, CGamePlayer *pPlayer)) 0x805B1A8)//0x0063A578 
#define Execute_SyncRoleInfo_ToGameServer		((bool(__cdecl*)(CGamePlayer *This, const void *pData, size_t dataLength)) 0x804C866)//0x0063A578 
#define Execute_KGoddess_SendLargeData		((bool(__cdecl*)(LPVOID* This, BYTE byProtocol, const void *pData, size_t nLen)) 0x80661F8)//0x0063A578 
#define Execute_ChangeAccountInMap		((BOOL(__cdecl*)(LPCSTR pcszAccount, const Account* pNew, const Account* pMask)) 0x806A508)//0x0063A578 
#define Execute_DispatchPlayerMessage		((bool(__cdecl*)(CIntercessor* This, CGamePlayer* pPlayer, size_t index, const void* pData, size_t datalength)) 0x805A91A)//0x0063A578 
#define Execute_PrepareLogin		((void(__cdecl*)(CGamePlayer *This, KAccountUserLoginInfo *pLogin, DWORD dwIP, DWORD dwPort)) 0x804C1E6)//0x0063A578 
#define Execute_IpToString		((LPCSTR(__cdecl*)(DWORD dwIp, LPSTR pszIp, size_t nLen)) 0x805A6DE)//0x0063A578 
#define Execute_IpFromString		((DWORD(__cdecl*)(LPCSTR pszIp)) 0x805A66A)//0x0063A578 
#define Execute_QueryAccPwd		((UINT(__cdecl*)(CGamePlayer* This)) 0x804E540)//0x0063A578 
#define Execute_CheckAccountMap		((BOOL(__cdecl*)(CGamePlayer* This, LPCSTR pszFunc)) 0x804C6AA)//0x0063A578 
#define Execute_DetachAccount		((BOOL(__cdecl*)(CGameServer* This, LPCSTR pszAccount)) 0x8056C24)//0x0063A578 
#define Execute_DispatchTask		((BOOL(__cdecl*)(CGameServer* This, UINT nTask, const void *pData, size_t datalength, void *pvData)) 0x8057356)//0x0063A578 
#define Execute_CreateServer		((BOOL(__cdecl*)(CIntercessor *This, CGameServer **ppServer, UINT nIdentity)) 0x805B6A2)//0x0063A578 
#define Execute_CGameServer		((void(__cdecl*)(CGameServer *This, UINT nIdentityID, KService *pService, CIntercessor *pIntercessor, const void *pPaysys)) 0x8058EC6)//0x0063A578 
#define Execute_OnGameServerDestroy		((void(__cdecl*)(CIntercessor *This, unsigned int nID)) 0x805CC56)//0x0063A578 
//#define ExecuteVerifyAccountToPlayer		((bool(__cdecl*)(CGamePlayer* This, LPCSTR pszUserName, LPCSTR pszPassword, UINT nQueryResult, unsigned int nLeftTime, unsigned int nLeftTimeOfPoint, DWORD dwLimitTimeFlag, DWORD dwLimitTimeOnlineSeconds, DWORD dwLimitTimeOfflineSeconds)) 0x804D40A)//0x0063A578 
//#define ExecuteCheckLoginPermission		((bool(__cdecl*)(CGamePlayer *, LPCSTR )) 0x804C1B6)//0x0063A578 

KLoginX::KLoginX() {
	//int nLength = sizeof(CDataLimitConnects);
	//this->nDataLimitConnect->AccountName = AccountName;
}
// -----------------------------------------------------------------------------

KLoginX::~KLoginX() {
}

DataLimitConnect KLoginX::LoadData(const char* pAccName)
{
	CDataLimitConnects* Data;
	Data->AccountName = pAccName;
	return Data;
}

void handler_signal(int nSignal)
{
	//printf("Signal ID: %d\n", nSignal);
	signal(SIGSEGV, SIG_IGN);
	//signal(SIGINT, SIG_IGN);
	//printf("Got SIGSEGV at address: %p\n", si->si_addr);
	//exit(EXIT_FAILURE);
	//throw(signal);
	//exit(signal);
}

static void
handler(int signalNum, siginfo_t* si, void* unused)
{
	signal(SIGINT, SIG_DFL);
	printf("Got SIGSEGV at address: %p\n", si->si_addr);
	//exit(EXIT_FAILURE);
	throw(signalNum);
	//exit(signal);
}

bool KLoginX::CheckHardwareId(char* HardwareId)
{
	if (WhiteList_Data.size() <= 0)
	{
		return 0;
	}

	for (DWORD i = 0; i < WhiteList_Data.size(); i++)
	{
		//printf("HardwareId [%d] : %s \n", i, WhiteList_Data[i].HardwareId);

		if (strcmp(WhiteList_Data[i].HardwareId, HardwareId) == 0)
		{
			return 1;
		}
	}

	return 0;
}

void KLoginX::Hook_WaitForAccPwd()
{
	bool SqlInit;
	//CGamePlayer* gGamePlayer = (CGamePlayer*)0xFF003F65;
	//CGamePlayer* GamePlayer = (CGamePlayer*)0x8082740;
	//vptr_CIntercessor = (CIntercessor*)0x8083380;
	//printf("ANTI LOADED\n");
	memset(gKLoginX.m_CurrenSubCommand, 0, 32);
	gKLoginX.m_IsSub = false;;
	//gKLoginX.m_CurrenSubCommand;

	//SqlInit = gSqlite.Init();
	SqlInit = Sqlite_Init();

	if (!SqlInit)
		exit(0);

#if (_DEBUG_SQLITE==1)
	printf("SqlInit: %d\n", SqlInit);
#endif

	//pthread_mutex_init(&waitlock, NULL);

	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start;
	uintptr_t pagestart;
	 
	//start = (uintptr_t)(void*)0x804CAD4;
	//pagestart = start & -pagesize;
	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//}

	//gUtil.SetCompleteHook(0xE8, start, &ProcessRoleInfoMsgQueue);

	//

	/*start = (uintptr_t)(void*)0x805A947;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &c2sCheckProtocol);*/


	
	/*start = (uintptr_t)(void*)0x8057579;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &CGameServer_PushAccount);*/

	//-==================================================================
	//0x804CCEA 0x805739D 0x804CB68

	/*start = (uintptr_t)(void*)0x805739D;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &SyncRoleInfoToGameServer);*/
	
	// DispatchTask

	start = (uintptr_t)(void*)0x804C9B1;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeDispatchTask);

	//-==================================================================

	//gUtil.SetByte(0x8057248, 0xD5);

	//start = (uintptr_t)(void*)0x804F71E;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//}

	//gUtil.SetCompleteHook(0xE8, start, &_FreePlayer);

	//start = (uintptr_t)(void*)0x804C95E;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//}

	//gUtil.SetCompleteHook(0xE8, start, &CIntercessorAddPlayer);
	 
	

	start = (uintptr_t)(void*)0x805AA75;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &GamePlayer_DispatchPlayerMessage);

	//=========================== Get IP Login ==================\\

	/*start = (uintptr_t)(void*)0x804E76B;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &SafePrepareLogin);*/

	//=========================== Check Account nefore login to GameServer ==================\\

	/*start = (uintptr_t)(void*)0x804C7D1;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeCheckAccountMap);*/

	//==========================================================================================//

	//gUtil.SetDword(0x804ED05, (DWORD)&SafeQueryAccPwd);

	//=========================== END Get IP Login ==================\\

#if(BISHOP_VERSION>=2)

	// -======================================================================
	//if (gUtil.nLimit > 0)
	//{
		start = (uintptr_t)(void*)0x8057713; // NotifyLeaveGame
		pagestart = start & -pagesize;

		if (mprotect((void*)pagestart, 0x2000u,
			PROT_READ | PROT_WRITE | PROT_EXEC))
		{
			perror("mprotect");
		}

		gUtil.SetCompleteHook(0xE8, start, &GetAccountInMap);
	//}
	

	// -======================================================================

	/*start = (uintptr_t)(void*)0x804CA05;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &NotifyEnterGameGetAccountInMap);*/
	 
	//start = (uintptr_t)(void*)0x805568E;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//}
		
	//gUtil.SetCompleteHook(0xE8, start, &CGameServer_SmallPackProcess);

	/*start = (uintptr_t)(void*)0x80574E8;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &CGameServer_PushAccount);*/

	//std::cout << "DWORD NotifyEnterGame: " << (DWORD)&NotifyEnterGame << std::endl;
	//std::cout << "DWORD* NotifyEnterGame: " << *(DWORD*)&NotifyEnterGame << std::endl;

	//gUtil.MemorySet(0x8058468, (DWORD)&NotifyEnterGame, sizeof(&NotifyEnterGame));
	//printf("NotifyEnterGame: %p %p %p\n", (DWORD)&NotifyEnterGame, (DWORD)&OfflineRequestAsk, (DWORD)&CGameServer_SmallPackProcess);
	//gUtil.SetDword(0x8058468, (DWORD)&NotifyEnterGame);
	//gUtil.SetDword(0x8058468, (DWORD)&NotifyEnterGame);
	//gUtil.SetDword(0x80585DD, (DWORD)&OfflineRequestAsk);

	// -======================================================================

	start = (uintptr_t)(void*)0x80588E8;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &GameServer_Destroy_Account);
	
	start = (uintptr_t)(void*)0x80629DC;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &KConsole_ProcessCmd);

	start = (uintptr_t)(void*)0x80568BC;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &g_ChangeAccountInMap);

	//==================================================

	gKLoginX.GameServer_List.clear();

	start = (uintptr_t)(void*)0x805B6E6;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &g_CGameServer);
	
	//==================================================

	gKLoginX.GameServer_List.clear();

	start = (uintptr_t)(void*)0x805CE95;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	gUtil.SetCompleteHook(0xE8, start, &g_OnGameServerDestroy);

	gKLoginX.IsOpen = true;
	//-===========================================================================
	// 
	//gUtil.SetByte(0x805D81E, sizeof(KLoginAccountInfo) + 1);
	//gUtil.SetByte(0x805DB78, sizeof(KLoginAccountInfo) + 1);
#endif
	
	//gUtil.SetByte(0x804D4F4, sizeof(KLoginAccountInfo) + 1);

	//gUtil.SetByte(0x804D4C8, sizeof(KLoginAccountInfo));

	//0x8082740
	//0x8082748
}

//void __cdecl _FreePlayer(CGamePlayer* This)
//{
//	printf("__Shutdown!\n");
//	Execute_Shutdown(This);
//}

//bool __cdecl CIntercessorAddPlayer(CIntercessor* This, LPCSTR pszRole, CGamePlayer* pPlayer)
//{
//	printf("pszAccount: %s\n", pPlayer->tmp[0xA8]);
//	Execute_AddPlayer(This, pszRole, pPlayer);
//	//if (gKLoginX.CheckAccountEntergame((char*)pszAccount))
//	//{
//	//	Execute_AddPlayer(This, pszRole, pPlayer);
//	//}
//	//else
//	//	return false;
//	//
//}

void __cdecl g_OnGameServerDestroy(CIntercessor* This, unsigned int nID)
{
	Execute_OnGameServerDestroy(This, nID);

	std::map<int, CGameServer*>::iterator it = gKLoginX.GameServer_List.find(nID);
	
	if (it != gKLoginX.GameServer_List.end())
	{
		gKLoginX.GameServer_List.erase(it);
	}

}

void __cdecl g_CGameServer(CGameServer* This, UINT nIdentityID, KService* pService, CIntercessor* pIntercessor, const void* pPaysys)
{
	Execute_CGameServer(This, nIdentityID, pService, pIntercessor, pPaysys);
	gKLoginX.GameServer_List.insert(pair<int, CGameServer*>((int)nIdentityID, This));
}

BOOL __cdecl g_ChangeAccountInMap(LPCSTR pcszAccount, const Account* pNew, const Account* pMask)
{
	//printf("pszAccount: %s\n", pcszAccount);
	try
	{
		Sqlite_FactionRemove((char*)pcszAccount);

		if (gUtil.nLimitIP > 0)
		{
			Sqlite_Remove_IP_On_Outgame((char*)pcszAccount);
		}

		if (gUtil.nLimit > 0)
		{
			Sqlite_Remove_HardwareId_On_Outgame((char*)pcszAccount);
		}
	}
	catch (...)
	{
	}
	

	return Execute_ChangeAccountInMap(pcszAccount, pNew, pMask);
}

void __cdecl KConsole_ProcessCmd(KConsole* const This, LPCSTR pszCmd)
{

	char* cmd_main;
	cmd_main = (char*)malloc(strlen(pszCmd) + 1);

	strcpy(cmd_main, pszCmd);

	if (!gKLoginX.m_IsSub)
	{
		memset(gKLoginX.m_CurrenSubCommand, 0, 32);
		strcpy(gKLoginX.m_CurrenSubCommand, cmd_main);
		if ((memcmp(cmd_main, "close", 5) == 0))
		{
			gKLoginX.IsOpen = false;
			std::cout << ">> Close gateway!" << std::endl;
			
		}
		else if ((memcmp(cmd_main, "open", 4) == 0))
		{
			gKLoginX.IsOpen = true;
			std::cout << ">> Open gateway!" << std::endl;
		}
		else if ((memcmp(cmd_main, "unlimit", 6) == 0))
		{
			gUtil.nLimit = 0;
			std::cout << ">> Disable limit game!" << std::endl;
		}
		else if ((memcmp(cmd_main, "limit", 5) == 0))
		{
			gKLoginX.m_IsSub = true;
			std::cout << ">> [Game] Input number: " << std::endl;
		}
		else if ((memcmp(cmd_main, "ipconn", 7) == 0))
		{
			gKLoginX.m_IsSub = true;
			std::cout << ">> [IP] Input number: " << std::endl;
		}
		else if ((memcmp(cmd_main, "reloadwl", 8) == 0))
		{
			gKLoginX.WhiteList_Data.clear();
			fstream newfile;
			newfile.open("whitelist.txt", ios::in);
			if (newfile.is_open()) {
				std::string tp;
				while (std::getline(newfile, tp)) {
					//std::string delimiter = "	";
					//std::vector<std::string> results = gUtil.split(tp, delimiter);
					//std::vector<std::string> results;
					//boost::algorithm::split(results, tp, boost::is_any_of(delimiter));
					//cout << nRank << " - " << results[0] << "\n";
					//cout << tp << "\n";
					
					WHITELIST_HARDID_INFO HidInfo;

					memcpy(HidInfo.HardwareId, tp.c_str(), 32);

					HidInfo.HardwareId[31] = '\0';

					gKLoginX.WhiteList_Data.push_back(HidInfo);

					/*if (gKLoginX.WhiteList_Data.size() > 0)
					{
						std::map<std::string, int>::iterator it = gKLoginX.WhiteList_Data.find(std::string(tp.c_str()));
						if (it == gKLoginX.WhiteList_Data.end())
						{
							gKLoginX.WhiteList_Data.insert(pair<std::string, int>(std::string(tp.c_str()), 1));
						}
					}
					else
						gKLoginX.WhiteList_Data.insert(pair<std::string, int>(std::string(tp.c_str()), 1));*/

					//std::this_thread::sleep_for(std::chrono::milliseconds(30));
				}
				newfile.close();
			}

			std::cout << ">> Whitelist allow load " << gKLoginX.WhiteList_Data.size() << " count" << std::endl;
		}
		else if ((memcmp(cmd_main, "now", 3) == 0))
		{
			gKLoginX.m_IsSub = true;
			std::cout << ">> Are you sure to quit?(Y/N)" << std::endl;
		}
		else
			Execute_KConsole_ProcessCmd(This, pszCmd);
	}
	else
	{

		gKLoginX.m_IsSub = false;

		if ((memcmp(gKLoginX.m_CurrenSubCommand, "limit", 5) == 0))
		{
			if (gUtil.is_integer(std::string(cmd_main)))
			{
				gUtil.nLimit = atoi(cmd_main);
				std::cout << ">> Set limit [Game] to: " << gUtil.nLimit << std::endl;
				memset(gKLoginX.m_CurrenSubCommand, 0, 32);
				//Sqlite_ReNew();
				if (!gKLoginX.GameServer_List.empty())
				{
					for (auto it = gKLoginX.GameServer_List.begin(); it != gKLoginX.GameServer_List.end(); ++it)
					{
						if (it->second->tmp[0x85C] == 1)
						{
							BYTE szBuff[10];
							memset(szBuff, 0, 10);
							szBuff[0] = 212;
							gUtil.SetByte((DWORD)&szBuff[5], 1);
							gUtil.SetInt((DWORD)&szBuff[6], gUtil.nLimit);
							szBuff[9] = 0;
							Execute_SendData(it->second, &szBuff, 10u);
							//std::cout << ">> Send to gameserver [" << it->first << "] done" << std::endl;
						}
					}
				}
			}
			else
			{
				std::cout << ">> Please enter the number" << std::endl;
			}
			memset(gKLoginX.m_CurrenSubCommand, 0, 32);
		}
		else if ((memcmp(gKLoginX.m_CurrenSubCommand, "ipconn", 6) == 0))
		{
			if (gUtil.is_integer(std::string(cmd_main)))
			{
				gUtil.nLimitIP = atoi(cmd_main);
				std::cout << ">> Set limit [IP] to: " << gUtil.nLimitIP << std::endl;
				memset(gKLoginX.m_CurrenSubCommand, 0, 32);
				if (!gKLoginX.GameServer_List.empty())
				{
					for (auto it = gKLoginX.GameServer_List.begin(); it != gKLoginX.GameServer_List.end(); ++it)
					{
						if (it->second->tmp[0x85C] == 1)
						{
							BYTE szBuff[10];
							memset(szBuff, 0, 10);
							szBuff[0] = 212;
							gUtil.SetByte((DWORD)&szBuff[5], 2);
							gUtil.SetInt((DWORD)&szBuff[6], gUtil.nLimitIP);
							szBuff[9] = 0;
							Execute_SendData(it->second, &szBuff, 10u);
							//std::cout << ">> Send to gameserver [" << it->first << "] done" << std::endl;
						}
					}
				}
			}
			else
			{
				std::cout << ">> Please enter the number" << std::endl;
			}
			memset(gKLoginX.m_CurrenSubCommand, 0, 32);
		}
		else if ((memcmp(gKLoginX.m_CurrenSubCommand, "now", 3) == 0))
		{
			if ((memcmp(cmd_main, "y", 2) == 0))
			{
				exit(1);
				return;
			}
		}
		else
			Execute_KConsole_ProcessCmd(This, pszCmd);

	}

	free(cmd_main);
}
//Execute_CheckAccountMap
BOOL __cdecl SafeCheckAccountMap(CGamePlayer* This, LPCSTR pszFunc)
{
	//return 0;
	return Execute_CheckAccountMap(This, pszFunc);
}

void __cdecl SafePrepareLogin(CGamePlayer* This, KAccountUserLoginInfo* pLogin, DWORD dwIP, DWORD dwPort)
{
	Execute_PrepareLogin(This, pLogin, dwIP, dwPort);
	try
	{
		//char pAccName[32];
		char pIPWan[32];
		memset(pIPWan, 0, 32);
		Execute_IpToString(dwIP, pIPWan, 32);
		pIPWan[31] = 0;
		//pAccName = &This->tmp[0xA8];
		//pAccName = &pLogin->tmp[0xA];
		//strcpy(pAccName, &This->tmp[0xA8]);
		//memcpy(pAccName, (const void*)((std::string*)&This->tmp[0xA8])->c_str(), 32);
		//printf("PrepareLogin Account: %s\n", pAccName);
		
		//printf("PrepareLogin IP: %s\n", pIPWan);
		//Sqlite_Insert_IpAddr_From_Login(pAccName, pIPWan);
		Sqlite_Insert_IpAddr_From_Login((char*)&pLogin->tmp[0xA], pIPWan);
		//Sqlite_Update_WanAddress(pAccName, pIPWan);
	}
	catch (...)
	{
		/*char pAccName[32];
		char pIPWan[32];
		memset(pIPWan, 0, 32);
		Execute_IpToString(dwIP, pIPWan, 32);
		pIPWan[31] = 0;*/
		//Execute_PrepareLogin(This, pLogin, dwIP, dwPort);
	}
}

BOOL __cdecl GamePlayer_DispatchPlayerMessage(CIntercessor* This, CGamePlayer* pPlayer, size_t index, const void* pData, size_t datalength)
{
	BYTE protocoltype = *(BYTE*)pData;
	KLoginAccountInfo* pLAI;
	//pLAI->dwAllowLimit = 1;
	const BYTE* pMsg;
	int size;
	c2s_PROTOCOL nProtocol;
	const char* pAccName;
	const char* pAccPwd;
	BOOL resul = false;

	/*std::time_t time_result = std::time(nullptr);
	std::string time_res = std::asctime(std::localtime(&time_result));
	std::string::size_type i = time_res.find("\n");
	if (i != std::string::npos)
		time_res.erase(i, time_res.length());*/

	//struct sigaction sa;

	//sa.sa_flags = SA_SIGINFO;
	//sigemptyset(&sa.sa_mask);
	//sa.sa_sigaction = handler;
	//if (sigaction(SIGSEGV, &sa, NULL) == -1)
	//	handle_error("sigaction");

	if (!gKLoginX.IsOpen)
		return 0;

	//#if(_DEBUG==0)
	//	
	//#endif
		// if (protocoltype == 65)
#if(BISHOP_VERSION>=1)	
	if (protocoltype == 65 && datalength == (sizeof(KLoginAccountInfo) + 1))
	{
#endif		

		pMsg = (BYTE*)pData;
		pLAI = (KLoginAccountInfo*)((BYTE*)&pMsg[1]);
		//pLAI->ProtocolVersion = (pLAI->ProtocolVersion - gUtil.nClientVersion);
#if(_DEBUG==1)
		pAccName = (char*)pData + 7;
		pAccPwd = (char*)pData + 39;
		//printf("0: %d, %d, %d\n", datalength, protocoltype, sizeof(KLoginAccountInfo));
		//printf("1: %s, %s\n", pAccName, pAccPwd);
		//printf("2: %d, %d\n", gUtil.nVersionProtocol, pLAI->ProtocolVersion);
		//printf("3: %d, %d\n", pLAI->Size, pLAI->Param);
		//printf("4: %d %s\n", strlen(pLAI->LicenseId), pLAI->LicenseId);
		//printf("5: %s\n", pLAI->MacAddress);
		printf("6: %s\n", pLAI->IpAddress);
		//printf("6: %d %s\n", strlen(pLAI->HardwareSerialId), pLAI->HardwareSerialId);
		printf("7: %d %s\n", strlen(pLAI->HardwareId), pLAI->HardwareId);
		//printf("8: %d %s\n", strlen(gAuthenticate.LicenseId), gAuthenticate.LicenseId);
		//printf("9: %s\n", gAuthenticate.AllowClientSerial);
		//printf("Step check License\n");
#endif
		//printf("8: %d %d %s %s\n", gUtil.nVersionProtocol, pLAI->ProtocolVersion, pLAI->LicenseId, gUtil.AllowClientSerial);
#if(BISHOP_VERSION>=1)
			//if (!(memcmp(pLAI->LicenseId, gUtil.AllowClientSerial, sizeof(pLAI->LicenseId)) == 0))
			//if (!(memcmp(pLAI->LicenseId, gAuthenticate.LicenseId, sizeof(pLAI->LicenseId)) == 0))
			//char LicenseId[32];
			//memset(LicenseId, 0, 32);
			//strncpy(LicenseId, pLAI->LicenseId, 32);
		if (strlen(pLAI->LicenseId) < 32)
		{
			pLAI->ProtocolVersion = 0;
			goto endlogin;
		}

		if (strlen(pLAI->HardwareId) < 31)
		{
			pLAI->ProtocolVersion = 0;
			goto endlogin;
		}

		if (!(memcmp(pLAI->LicenseId, gAuthenticate.AllowClientSerial, 32) == 0))
		{
			pLAI->ProtocolVersion = 0;
			//pLAI->Size = 126;
			goto endlogin;
		}

#endif
		//char HardwareId[32];
		//char HardwareSerialId[36];
		//char* HardwareSerialId;

		//HardwareSerialId = (char*)malloc(sizeof(char) * 5);

#if(_DEBUG==1)
		printf("10: %d %d %s %s\n", gUtil.nVersionProtocol, pLAI->ProtocolVersion, pLAI->LicenseId, gUtil.AllowClientSerial);
#endif
		//strcpy(HardwareId, pLAI->HardwareId);
		//HardwareId[32] = 0;
		//strcpy(HardwareSerialId, pLAI->HardwareSerialId);
		//HardwareSerialId[36] = 0;
		/*if (strlen(pLAI->HardwareSerialId) < 35)
		{
			pLAI->ProtocolVersion = 0;
			goto endlogin;
		}
		else
		{

			memset(HardwareSerialId, 0, 36);

			strncpy(HardwareSerialId, pLAI->HardwareSerialId, 35);

			HardwareId[36] = 0;
		}

		if (strlen(pLAI->HardwareId) < 31)
		{
			pLAI->ProtocolVersion = 0;
			goto endlogin;
		}
		else
		{

			memset(HardwareId, 0, 32);

			strncpy(HardwareId, pLAI->HardwareId, 31);

			HardwareId[32] = 0;
		}*/


		//sprintf((char*)HardwareId, (const char*)"%.21s", pLAI->HardwareId);

#if(_DEBUG==1)
		//printf("HardwareId: %d %s\n", strlen(pLAI->HardwareId), pLAI->HardwareId);
		//printf("MacAddress: %d %s\n", !(memcmp(pLAI->MacAddress, "0", 1) == 0), pLAI->MacAddress);
		//printf("HardwareSerialId: %d %s\n", strlen(pLAI->HardwareSerialId), pLAI->HardwareSerialId);
		//printf("AllowLimit: %s %s %s\n", (char*)pLAI->MacAddress, ((char*)pLAI->MacAddress + 2), ((char*)pLAI->MacAddress + 4));
#endif
		/*bool Query = Sqlite_Insert_Account_From_Login(pLAI->HardwareId, pLAI->Account, pLAI->MacAddress, pLAI->IpAddress, pLAI->LicenseId);

		printf("Sqlite_Insert: %d\n", Query);*/

#if(BISHOP_VERSION>=2)

		try
		{
			/*if (pLAI->dwAllowLimit == NULL)
				pLAI->dwAllowLimit == 1;*/
			Sqlite_Insert_Account_From_Login(((char*)pLAI->MacAddress + 2), pLAI->IpAddress, pLAI->HardwareId, pLAI->HardwareSerialId, pLAI->Account);
			
			if (memcmp(((char*)pLAI->MacAddress + 2), "0", 1) == 0)
			{
				if (gUtil.nLimitIP > 0)
				{
					//Sqlite_Insert_IpAddr_From_Login(pLAI->Account, pLAI->IpAddress);
					if (!Sqlite_Check_IpAddress(pLAI->IpAddress))
					{
						pLAI->ProtocolVersion = 0;
						//pLAI->Size = 126;
						goto endlogin;
					}
				}
			}
		}
		catch (...)
		{
		}
		

		/*gUtil.BishopWriteLog("Login info [%d]\t-\t[%s]\tnPlayerIdx: %d, Account: [ %s ], HardwareId: [ %s ], MacAddress: [ %s ], IpAddress: [ %s ]\n",
			gUtil.nLimit,
			time_res.c_str(),
			index,
			pLAI->Account,
			pLAI->HardwareId,
			pLAI->MacAddress,
			pLAI->IpAddress);*/

		//if (gUtil.nLimit > 0)
		//{
		//	/*if (!Sqlite_Update_HardwareId(pLAI->Account, pLAI->HardwareId, pLAI->MacAddress, pLAI->IpAddress))
		//	{
		//		pLAI->ProtocolVersion = 0;
		//		goto endlogin;
		//	}*/
		//	//bool resual = true;
		//	Sqlite_Update_HardwareId(pLAI->Account, pLAI->HardwareId, pLAI->MacAddress, pLAI->IpAddress);
		//	//printf("Update HardwareId: %d\n", resual);
		//}


		//printf("8: %d %d %s %s\n", gUtil.nVersionProtocol, pLAI->ProtocolVersion, pLAI->LicenseId, gUtil.AllowClientSerial);

		//HARDWARE_ACCOUNT_LOGIN info2;

		////memcpy(&info2.Intercessor, &This, sizeof(info2.Intercessor));

		//memcpy(info2.HardwareId, pLAI->HardwareId, sizeof(info2.HardwareId));

		////memcpy(info2.DriveSerial, pLAI->HardwareId, sizeof(info2.DriveSerial));

		//info2.IsLogined = false;

		//info2.LastDetectionTime = time(0);

		//memcpy(info2.AccountName, pLAI->Account, sizeof(info2.AccountName));

		//memcpy(info2.MacAddress, pLAI->MacAddress, sizeof(info2.MacAddress));

		//memcpy(info2.IpAddress, pLAI->IpAddress, sizeof(info2.IpAddress));

		//info2.nProtocol = 0x41;

		//memcpy(info2.LicenseId, pLAI->LicenseId, sizeof(info2.LicenseId));

		//if (!Sqlite_Insert_Account_From_Login(HardwareId, pLAI->HardwareId, pLAI->Account, pLAI->MacAddress, pLAI->IpAddress, pLAI->LicenseId))

		//if (!gKLoginX.InsertHardwareIdLimit(pLAI->HardwareId, info2))
		//{
		//	pLAI->ProtocolVersion = 0;
		//	//pLAI->Size = 126;
		//}

#endif
			//printf("8: %d %d %s %s\n", gUtil.nVersionProtocol, pLAI->ProtocolVersion, pLAI->LicenseId, gUtil.AllowClientSerial);
			//printf("InsertHardwareIdLimit OK\n");

		if (!gKLoginX.GameServer_List.empty())
		{
			for (auto it = gKLoginX.GameServer_List.begin(); it != gKLoginX.GameServer_List.end(); ++it)
			{
				if (it->second->tmp[0x85C] == 1)
				{
					BYTE szBuff[50];
					memset(szBuff, 0, 10);
					szBuff[0] = 212;
					gUtil.SetByte((DWORD)&szBuff[5], 3);
					gUtil.MemoryCpy(&szBuff[6], pLAI->IpAddress, 16);
					szBuff[49] = 0;
					Execute_SendData(it->second, &szBuff, 50u);
					//std::cout << ">> Send to gameserver [" << it->first << "] done" << std::endl;
				}
			}
		}

	endlogin:
		
		char szBuffer[127];
		//pLAI->Size = 126;
		size = 127;
		memset(szBuffer, 0, 127);
		memcpy(szBuffer, pData, 127);
		//memmove(szBuffer, pData, 127);
		//printf("Execute!\n");
		/*resul = Execute_DispatchPlayerMessage(This, pPlayer, index, szBuffer, size);

		if (resul)
		{
			Sqlite_Insert_Account_From_Login(pLAI->HardwareId, pLAI->Account);
		}*/
		//return resul;
		//printf("return: %d\n", size);
		return Execute_DispatchPlayerMessage(This, pPlayer, index, szBuffer, size);
	}
	else
		return Execute_DispatchPlayerMessage(This, pPlayer, index, pData, datalength);

}

BOOL __cdecl GameServer_Destroy_Account(LPCSTR pcszAccount, Account* pAccount)
{
	//printf("GameServer_Destroy_Account\n");
	//printf("nEnterGameCount: %d\n", This->nEnterGameCount);
	//printf("szAccount: %s\n", pcszAccount);
	//printf("szPassword: %s\n", This->szPassword);
	//printf("szRole: %s\n", This->szRole);
	//gKLoginX.RemoveHardwareIdLimit((char*)pcszAccount);
	try
	{
		if (gUtil.nLimitIP > 0)
		{
			Sqlite_Remove_IP_On_Outgame((char*)pcszAccount);
		}

		if (gUtil.nLimit > 0)
		{
			Sqlite_Remove_HardwareId_On_Outgame((char*)pcszAccount);
		}

		Sqlite_FactionRemove((char*)pcszAccount);
	}
	catch (...)
	{
	}
	
	//Execute_Account(This);
	return Execute_GetAccountInMap(pcszAccount, pAccount);
}

bool __cdecl SyncRoleInfoToGameServerPop(KMsgQueue* This, BYTE byProtocol, size_t* const nLen)
{
	const void* pData;
	TProcessData* pPA;
	TRoleData* pRoleData;
	size_t nNewLen;

	printf("byProtocol: %d\n", byProtocol);

	pData = ExecuteMsgQueue_Pop(This, byProtocol, &nNewLen);

	*(nLen) = nNewLen;

	if (pData > 0)
	{
		pPA = (TProcessData*)pData;

		pRoleData = (TRoleData*)&pPA[1];


		printf("nLen: %d\n", nNewLen);

		nNewLen = nNewLen + 99;

		*(nLen) = nNewLen;

		printf("nLen: %d\n", nNewLen);

		printf("nDataLen1: %d\n", pPA->nDataLen - 1);

		printf("nDataLen2: %d\n", *(DWORD*)((char*)pRoleData));

		printf("nDataLen3: %d\n", pPA->ulIdentity);

		printf("caccname: %s\n", pRoleData->BaseInfo.caccname);

		printf("pBuffer: %d\n", pRoleData->pBuffer);

		printf("dwStateOffset: %d\n", pRoleData->dwStateOffset);

		printf("dwEnergy: %d\n", pRoleData->BaseInfo.dwEnergy);

		printf("imoney: %d\n", pRoleData->BaseInfo.imoney);

		//pRoleData->dwDataLen = pRoleData->dwDataLen + 99;

		//printf("pRoleData: %d\n", pRoleData->dwDataLen);

		//pPA->nDataLen = pPA->nDataLen + 99;

		//printf("nDataLen2: %d\n", pPA->nDataLen - 1);

		HARDWARE_ACCOUNT_LOGIN curreninfo;

		/*if (!gKLoginX.GetDataHwid((char*)pRoleData->BaseInfo.caccname, &curreninfo))
			return 0;*/

		memcpy(&pRoleData->HardwareId, curreninfo.HardwareId, sizeof(pRoleData->HardwareId));

		//memcpy(&pRoleData->MacAddress, curreninfo.MacAddress, sizeof(pRoleData->MacAddress));

		//memcpy(&pRoleData->IpAddress, curreninfo.IpAddress, sizeof(pRoleData->IpAddress));

		//memcpy(&pRoleData->LicenseId, curreninfo.LicenseId, sizeof(pRoleData->LicenseId));

		//pRoleData->dwDataLen = pRoleData->dwDataLen + sizeof(pRoleData->HardwareId) + sizeof(pRoleData->MacAddress) + sizeof(pRoleData->IpAddress) + sizeof(pRoleData->LicenseId);

		//pPA->nDataLen = pPA->nDataLen + sizeof(pRoleData->HardwareId) + sizeof(pRoleData->MacAddress) + sizeof(pRoleData->IpAddress) + sizeof(pRoleData->LicenseId);

		pRoleData->bBaseNeedUpdate = true;

		printf("nDataLen2: %d\n", pPA->nDataLen - 1);

		printf("pRoleData: %d\n", pRoleData->dwDataLen);

		printf("HardwareId: %s\n", pRoleData->HardwareId);
		//printf("MacAddress: %s\n", pRoleData->MacAddress);
		//printf("IpAddress: %s\n", pRoleData->IpAddress);
		//printf("LicenseId: %s\n", pRoleData->LicenseId);

	}

}

bool __cdecl SafeDispatchTask(CGameServer* This, UINT nTask, const void* pData, size_t datalength, void* pvData)
{
	if (nTask == 6u)
	{
#if(_DEBUG==1)
		//printf("pszAccount: %s\n", pszAccount);
		//printf("szName: %s\n", pRoleData->BaseInfo.szName);
		printf("nLimit: %s\n", gUtil.nLimit);
#endif
		//if(!gKLoginX.GetDataHwid((char*)pszAccount, &curreninfo))
		//	return false;

		//if (gKLoginX.CheckAccountEntergame((char*)pszAccount))
		//printf("SyncRoleInfoToGameServer\n");
		if (gUtil.nLimit > 0)
		{

			try
			{
				
				//LPCSTR pszAccount;
				char pszAccount[32];
				sprintf(pszAccount, "%s", pData + 73);
				bool allowLogin = false;
#if(_DEBUG==1)
				printf("SyncRoleInfoToGameServer Account: %s\n", pszAccount);
#endif
				if (strlen(pszAccount) <= 0)
				{
					return false;
				}
				//pszAccount = (char*)pData + 73;
				char AllHardwareId[72];
				char szHardwareId[32];
				memset(&AllHardwareId, 0, 72);
				memset(&szHardwareId, 0, 32);

				Sqlite_GetHardwareIdByAccount((char*)pszAccount, AllHardwareId, 3);

				AllHardwareId[71] = 0;

				strcpy(szHardwareId, AllHardwareId);

				szHardwareId[31] = 0;

				//printf("AllHardwareId : %s\n", szHardwareId);

				//std::map<std::string, int>::iterator it = gKLoginX.WhiteList_Data.find(std::string(szHardwareId));

				//printf("WhiteList_Data : %d\n", gKLoginX.WhiteList_Data.size());
				
				allowLogin = gKLoginX.CheckHardwareId(szHardwareId);

				//printf("allowLogin : %d\n", allowLogin);

				if (allowLogin)
				{
					//allowLogin = true;
					return Execute_DispatchTask(This, nTask, pData, datalength, pvData);
					/*for (auto itr = gKLoginX.WhiteList_Data.find(std::string(szHardwareId)); itr != gKLoginX.WhiteList_Data.end(); itr++)
					{

						printf("Found 2 %s\n", it->first);
						allowLogin = true;
						return Execute_DispatchTask(This, nTask, pData, datalength, pvData);
					}*/

					/*if (it == gKLoginX.WhiteList_Data.end())
					{
						
					}*/
				}
				
				/*if (allowLogin)
					return Execute_DispatchTask(This, nTask, pData, datalength, pvData);*/

				bool result = Sqlite_Check_HardwareId_On_EnterGame((char*)pszAccount);
				//printf("result : %d\n", result);	
				if (!result)
				{
#if(_DEBUG==1)
					printf("SyncRoleInfoToGameServer: %s\n", pszAccount);
#endif
					return false;
				}
				else
					return Execute_DispatchTask(This, nTask, pData, datalength, pvData);
			}
			catch (...)
			{
				return false;
				//return Execute_DispatchTask(This, nTask, pData, datalength, pvData);
			}
		}
		else
		{
			return Execute_DispatchTask(This, nTask, pData, datalength, pvData);
		}
	}
	else
		return Execute_DispatchTask(This, nTask, pData, datalength, pvData);
}

bool __cdecl SyncRoleInfoToGameServer(CGameServer* This, const void* pData, size_t dataLength, void* pvData)
{
	//printf("SyncRoleInfoToGameServer\n");
	//BYTE HardwareId[32];
	//Account sAccount; // [esp+30h] [ebp-118h] BYREF
	//tagGuidableInfo ginfo; // [esp+BBh] [ebp-8Dh] BYREF
	//size_t nPackLen; // [esp+128h] [ebp-20h] BYREF
	//TRoleData* pRoleData; // [esp+12Ch] [ebp-1Ch]
	
	//HARDWARE_ACCOUNT_LOGIN curreninfo;
#if(_DEBUG==1)
	//printf("pszAccount: %s\n", pszAccount);
	//printf("szName: %s\n", pRoleData->BaseInfo.szName);
	printf("nLimit: %s\n", gUtil.nLimit);
#endif
	//if(!gKLoginX.GetDataHwid((char*)pszAccount, &curreninfo))
	//	return false;

	//if (gKLoginX.CheckAccountEntergame((char*)pszAccount))
	//printf("SyncRoleInfoToGameServer\n");
	if (gUtil.nLimit > 0)
	{
		
		try
		{
			//LPCSTR pszAccount;
			char pszAccount[32];
			sprintf(pszAccount, "%s", pData + 73);
#if(_DEBUG==1)
			printf("SyncRoleInfoToGameServer Account: %s\n", pszAccount);
#endif
			if (strlen(pszAccount) <= 0)
			{
				return false;
			}
			//pszAccount = (char*)pData + 73;
			//bool result = Sqlite_Check_HardwareId_On_EnterGame((char*)pszAccount);
			//printf("result : %d\n", result);
			if (!Sqlite_Check_HardwareId_On_EnterGame((char*)pszAccount))
			{
#if(_DEBUG==1)
				printf("SyncRoleInfoToGameServer: %s\n", pszAccount);
#endif
				//Execute_DetachAccount(This, (char*)pszAccount);
				return false;
			}
			else
				return Execute_SyncRoleInfo(This, pData, dataLength, pvData);
		}
		catch (...)
		{
			return Execute_SyncRoleInfo(This, pData, dataLength, pvData);
		}
	}
	else
	{
		return Execute_SyncRoleInfo(This, pData, dataLength, pvData);
	}

	
	
}


bool __cdecl OfflineRequestAsk(CGameServer* This, const void* pData, size_t nDataSize)
{
	try
	{
		if (nDataSize != 73)
			return 0;
		//char pAccName[32];
		//printf("AccountName : %s", pData + 1);

		if (strlen((const char*)pData + 1) <= 1)
		{
			return false;
		}

		Sqlite_FactionRemove((const char*)pData + 1);

		if (gUtil.nLimitIP > 0)
		{
			Sqlite_Remove_IP_On_Outgame((const char*)pData + 1);
		}

		if (gUtil.nLimit > 0)
		{
			//char pAccName[32];
			//memset(&pAccName, 0, 32);
			//memcpy(&pAccName, (char*)pData + 1, 31);
			//pAccName[31] = 0;
			Sqlite_Remove_HardwareId_On_Outgame((const char*)pData + 1);
			return Execute_OfflineRequestAsk(This, pData, nDataSize);
		}
		else
			return Execute_OfflineRequestAsk(This, pData, nDataSize);
		//gKLoginX.RemoveHardwareIdLimit((char*)pcszAccount);
		//Sqlite_InsertOrUpdate_HardwareId_On_EnterGame((char*)pcszAccount);
	}
	catch (...)
	{
		return Execute_OfflineRequestAsk(This, pData, nDataSize);
	}
	
}

bool __cdecl CGameServer_SmallPackProcess(CGameServer* const This, BYTE cProtocol, const void* pData, size_t nLen)
{
	return Execute_SmallPackProcess(This, cProtocol, pData, nLen);
}

bool __cdecl CGameServer_PushAccount(CGameServer* const This, const char* pAccountName)
{
	bool resul;
	int  nHwidCount = 0;
	resul = false;
	resul = Execute_PushAccount(This, pAccountName);
	if (resul)
	{
		BYTE szBuff[105];
		memset(szBuff, 0, 105);
		char AllHardwareId[72];
		//char pszHardwareId[36];
		//char pszMacAddress[18];
		//char pszIpAddress[16];
		memset(&AllHardwareId, 0, 72);
		//memset(&pszHardwareId, 0, 36);
		//memset(&pszMacAddress, 0, 18);
		//memset(&pszIpAddress, 0, 16);
		//pszHardwareId = (char*)malloc(36);
		//strcpy(pszAccount, (char*)pData + 1);
		nHwidCount = Sqlite_GetAndInsert_HardwareId_On_EnterGame(pAccountName, (const void*)AllHardwareId, 3);
		//printf("nHwidCount: %d\n", nHwidCount);
		if (nHwidCount > 0)
		{
			AllHardwareId[72] = 0;
#if(_DEBUG==1)
			printf("HardwareId: %s\n", AllHardwareId);
			printf("MacAddress: %s\n", &AllHardwareId[37]);
			printf("IpAddress: %s\n", &AllHardwareId[55]);
#endif
			if (strlen(AllHardwareId) >= 35)
			{
				szBuff[0] = 211;
				memcpy(&szBuff[1], pAccountName, 32);
				szBuff[31] = 0;
				memcpy(&szBuff[33], AllHardwareId, 72);
				szBuff[105] = 0;
				Execute_SendData(This, &szBuff, 105u);
			}

			return resul;
	}
		else
		{
			return false;
		}
	}

	return resul;

}

bool __cdecl NotifyEnterGame(CGameServer* const This, const void* pData, size_t datalength)
{
	try
	{
		if ((int)datalength != 33)
			return false;


		//printf("NotifyEnterGame\n");
		//const char* pszAccount;
		//LPCSTR pszAccount;
		const BYTE* pMsg;
		//char pszAccount[32];
		//char* HardwareId;
		//BYTE szBuffUpdateMap;

		//LPCSTR pszHardwareId;
		BYTE szBuff[69];
		memset(&szBuff, 0, 69);
		BOOL resul = false;

		//char* pszAccount;

		//pszAccount = (char*)malloc(32);

		//strcpy(pszAccount, (char*)pData + 1);

		char pszAccount[32];
		memset(&pszAccount, 0, 32);
		memcpy(&pszAccount, (char*)pData + 1, 31);
		pszAccount[31] = 0;
		//pszAccount = (LPCSTR)pData + 1;

		//char pszAccount[32];
		//char pszHardwareId[32];

		//memset(&pszAccount, 0, 32);
		//strcpy(pszAccount, (LPCSTR)pData + 1);

		//sprintf(pszAccount, "%s", pData + 1);

#if(_DEBUG==1)
		printf("AccName Join Game: %s\n", pszAccount);
#endif
		//if (Sqlite_GetAndInsert_HardwareId_On_EnterGame(pszAccount, (const void*)pszHardwareId))
			//printf("HardwareId: %s\n", pszHardwareId);

		//memset(&pszHardwareId, 0, 32);

		//szBuffUpdateMap = 210;

		//Execute_SendData(This, &szBuffUpdateMap, 1);

		//if (gKLoginX.GetDataHwid((char*)pAccName, &curreninfo))

		//if (gUtil.nLimit > 0)
		//{
		char pszHardwareId[36];
		//char pszMacAddress[18];
		//char pszIpAddress[16];
		memset(&pszHardwareId, 0, 36);
		//memset(&pszMacAddress, 0, 18);
		//memset(&pszIpAddress, 0, 16);
		//pszHardwareId = (char*)malloc(36);
		//strcpy(pszAccount, (char*)pData + 1);
		int  nHwidCount = 0;
		nHwidCount = Sqlite_GetAndInsert_HardwareId_On_EnterGame(pszAccount, (const void*)pszHardwareId, 0);
		//printf("nHwidCount: %d\n", nHwidCount);
		if (nHwidCount > 0)
		{

#if(_DEBUG==1)
			printf("HardwareId: %s\n", pszHardwareId);
#endif
			if (strlen(pszHardwareId) >= 35)
			{
				szBuff[0] = 211;
				memcpy(&szBuff[1], pszAccount, 31);
				szBuff[31] = 0;
				szBuff[32] = 0;
				memcpy(&szBuff[33], pszHardwareId, 35);
				szBuff[68] = 0;
				szBuff[69] = 0;
				Execute_SendData(This, &szBuff, 69u);
			}

			//free(pszHardwareId);
		}
		else
		{
			return false;
		}
		//}

		//free(pszAccount);

		return Execute_NotifyEnterGame(This, pData, datalength);
	}
	catch (...)
	{
		return Execute_NotifyEnterGame(This, pData, datalength);
	}

}

bool __cdecl NotifyEnterGameGetAccountInMap(LPCSTR pcszAccount, Account* pAccount)
{
	//BYTE HardwareId[32];
	//Sqlite_InsertOrUpdate_HardwareId_On_EnterGame((char*)pcszAccount);
	
	/*if (!gKLoginX.CheckAccountEntergame((char*)pcszAccount))
		return false;
	else
		return Execute_GetAccountInMap(pcszAccount, pAccount);*/
	//printf("NotifyEnterGameGetAccountInMap\n");
	try
	{
		if (gUtil.nLimit > 0)
		{
			if (!Sqlite_Check_HardwareId_On_EnterGame((char*)pcszAccount))
				return false;
			else
				return Execute_GetAccountInMap(pcszAccount, pAccount);
		}
		else
			return Execute_GetAccountInMap(pcszAccount, pAccount);
	}
	catch (...)
	{
		return Execute_GetAccountInMap(pcszAccount, pAccount);
	}	
}

BOOL __cdecl GetAccountInMap(LPCSTR pcszAccount, Account* pAccount)
{
	//printf("GetAccountInMap\n");
	try
	{
		Sqlite_FactionRemove((char*)pcszAccount);

		if (gUtil.nLimitIP > 0)
		{
			Sqlite_Remove_IP_On_Outgame((char*)pcszAccount);
		}

		if (gUtil.nLimit > 0)
		{
			Sqlite_Remove_HardwareId_On_Outgame((char*)pcszAccount);
		}
	}
	catch (...)
	{
	}
	//printf("Account Logout: %s\n", pcszAccount);
	
	//gKLoginX.RemoveHardwareIdLimit((char*)pcszAccount);
	//Sqlite_InsertOrUpdate_HardwareId_On_EnterGame((char*)pcszAccount);
	return Execute_GetAccountInMap(pcszAccount, pAccount);
}

BOOL __cdecl c2sCheckProtocol(CIntercessor* This, c2s_PROTOCOL nProtocol, const BYTE* pMsg, size_t size)
{
	bool resul;
	//KLoginAccountInfoOrigin* pOrigin;

	//if (gAuthenticate.LicenseId != pLAI->LicenseId)
	//	printf("Not found\n");
	//pOrigin = (KLoginAccountInfoOrigin*)((BYTE*)&pMsg[1]);
	//printf("0: %d, %d, %d\n", size, nProtocol, sizeof(pMsg));
	//printf("1: %s, %s\n", pOrigin->Account, pOrigin->Password.szPassword);
	//printf("2: %d, %d\n", pOrigin->nLeftTime, pOrigin->ProtocolVersion);
	//printf("3: %d, %d\n", pOrigin->Size, pOrigin->Param);

	//ExecuteVerifyAccountToPlayer((CGamePlayer*)(void*)0xFF003F65, (const char*)pLAI->Account - 2, (const char*)pLAI->Password.szPassword, 327690, 0, 0, 0, 0, 0);
	if (nProtocol == 65)
	{
		//printf("0: %d, %d, %d\n", size, nProtocol, sizeof(pMsg));
		//KLoginAccountInfo* pLAI;
		//const char* pAccName;
		//const char* pAccPwd;
		////const char* HardwareId;
		////char HardwareId[51];

		//pLAI = (KLoginAccountInfo*)((BYTE*)&pMsg[1]);

		////c2s_PROTOCOL nProtocolCustom;

		////nProtocolCustom = (c2s_PROTOCOL)*pMsg;

		////BYTE cProtocol = (BYTE)pMsg[0];

		////printf("1: %d, %d, %d\n", This->tmp[0x7C], This->tmp[0xC9C], This->tmp[0x90]);
		////This->tmp[0xA8]
		//	//pAccName = (char*)pMsg + 7;
		//	//pAccPwd = (char*)pMsg + 39;
		////pLAI->Size = 126;
		////printf("0: %d, %d, %d\n", size, nProtocol, sizeof(pMsg));
		////printf("1: %s, %s\n", pLAI->Account, pLAI->Password.szPassword);
		////printf("2: %d, %d\n", pLAI->nLeftTime, pLAI->ProtocolVersion);
		////printf("3: %d, %d\n", pLAI->Size, pLAI->Param);
		////printf("4: %s\n", pLAI->LicenseId - 2);
		////printf("5: %s\n", pLAI->MacAddress);
		////printf("6: %s\n", pLAI->IpAddress);
		////printf("7: %s\n", pLAI->HardwareId);
		////printf("8: %s\n", gAuthenticate.LicenseId);
		////printf("9: %d\n", memcmp(gAuthenticate.LicenseId, pLAI->LicenseId, sizeof(gAuthenticate.LicenseId)));

		//if (!(memcmp(pLAI->LicenseId - 2, gAuthenticate.LicenseId, sizeof(pLAI->LicenseId - 2)) == 0))
		//	return false;

		//
		////CIntercessor* iCLient = (CIntercessor*)This;
		//char HardwareId[100];
		////sprintf(HexIp, "%02X:%02X:%02X:%02X:%02X:%02X", val + 24, val+16, val+8, val);
		//sprintf((char*)HardwareId, (const char*)"%s-%s", pLAI->MacAddress - 2, pLAI->IpAddress - 2);

		//HARDWARE_ACCOUNT_LOGIN info2;

		//memcpy(info2.HardwareId, HardwareId, sizeof(info2.HardwareId));

		//memcpy(info2.DriveSerial, pLAI->HardwareId - 2, sizeof(info2.DriveSerial));

		//info2.IsLogined = false;

		//info2.LastDetectionTime = time(0);

		//memcpy(info2.AccountName, pLAI->Account - 2, sizeof(info2.AccountName));

		//memcpy(info2.MacAddress, pLAI->MacAddress - 2, sizeof(info2.MacAddress));

		//memcpy(info2.IpAddress, pLAI->IpAddress - 2, sizeof(info2.IpAddress));

		//info2.nProtocol = nProtocol;

		//memcpy(info2.LicenseId, pLAI->LicenseId - 2, sizeof(info2.LicenseId));
		////info2.LastDetectionTime = time(0);

		////if (!gKLoginX.InsertAccountLogin(info2))
		//	//return false;

		//if (!gKLoginX.InsertHardwareIdLimit(HardwareId, info2))
		//	return false;

		//printf("InsertHardwareIdLimit OK\n");
		//BYTE		Buff[127];
		//memset(&Buff, 0, 127);
		//memmove(&Buff, &pMsg, 127);
		//printf("%s\n", HardwareId);
		//printf("\n\n", HardwareId);
		//size = 127;
		//memset(&gKLoginX.szBuff, 0, 127);
		//memcpy(&pMsg, &pMsg, 127);
		//memcpy(&pData, gKLoginX.pData, 197);
		//return gKLoginX.InsertHardwareIdLimit(HardwareId, info);
		return Executec2sCheckProtocol(This, nProtocol, pMsg, size);
	}
	else
	{
		//BYTE		Buff[127];
		//memset(&Buff, 0, 127);
		//memmove(&Buff, &pMsg, 127);
		//size = 127;
		return Executec2sCheckProtocol(This, nProtocol, pMsg, size);
	}
}

const void* __cdecl ProcessRoleInfoMsgQueue(KMsgQueue* const This, BYTE byProtocol, size_t* const nLen)
{
	const void* pData;
	TProcessData* pPA;
	TRoleData* pRoleData;
	size_t nNewLen;

	pData = ExecuteMsgQueue_Pop(This, byProtocol, &nNewLen);

	if (pData > 0)
	{
		pPA = (TProcessData*)pData;

		pRoleData = (TRoleData*)&pPA[1];

		//printf("nLen: %d\n", nNewLen);

		//nNewLen = nNewLen + 99;

		*(nLen) = nNewLen;

		/*printf("nLen: %d\n", nNewLen);

		printf("nDataLen1: %d\n", pPA->nDataLen - 1);

		printf("nDataLen2: %d\n", pRoleData->dwDataLen);

		printf("nDataLen3: %d\n", *(DWORD*)nLen);

		printf("nDataLen4: %d\n", pRoleData->dwStateOffset);

		printf("nDataLen5: %d\n", pRoleData->nStateCount);

		printf("nDataLen6: %d\n", *(DWORD*)((char*)pRoleData + pRoleData->dwDataLen - 4));*/

		/*printf("caccname: %s\n", pRoleData->BaseInfo.caccname);

		printf("pBuffer: %d\n", pRoleData->pBuffer);

		printf("dwStateOffset: %d\n", pRoleData->dwStateOffset);

		printf("dwEnergy: %d\n", pRoleData->BaseInfo.dwEnergy);

		printf("imoney: %d\n", pRoleData->BaseInfo.imoney);*/

		//pRoleData->dwDataLen = pRoleData->dwDataLen + 99;
		//*(DWORD*)&pRoleData->dwDataLen = *(DWORD*)pRoleData->dwDataLen + 99;

		//printf("pRoleData: %d\n", pRoleData->dwDataLen);
		//pPA->nDataLen = pPA->nDataLen + 99;
		//*(DWORD*)&pPA->nDataLen = *(DWORD*)pPA->nDataLen + 99;

		//printf("nDataLen2: %d\n", pPA->nDataLen - 1);

		//printf("HardwareId: %s\n", pRoleData->HardwareId);

		HARDWARE_ACCOUNT_LOGIN curreninfo;

		/*if (!gKLoginX.GetDataHwid((char*)pRoleData->BaseInfo.caccname, &curreninfo))
			return 0;*/

		//printf("Private HardwareId: %s\n", curreninfo.HardwareId);

		//memcpy(&pRoleData->HardwareId, curreninfo.HardwareId, sizeof(pRoleData->HardwareId));

		//memcpy(&pRoleData->MacAddress, curreninfo.MacAddress, sizeof(pRoleData->MacAddress));

		//memcpy(&pRoleData->IpAddress, curreninfo.IpAddress, sizeof(pRoleData->IpAddress));

		//memcpy(&pRoleData->LicenseId, curreninfo.LicenseId, sizeof(pRoleData->LicenseId));

		//pRoleData->dwDataLen = pRoleData->dwDataLen + sizeof(pRoleData->HardwareId) + sizeof(pRoleData->MacAddress) + sizeof(pRoleData->IpAddress) + sizeof(pRoleData->LicenseId);
		
		//pPA->nDataLen = pPA->nDataLen + sizeof(pRoleData->HardwareId) + sizeof(pRoleData->MacAddress) + sizeof(pRoleData->IpAddress) + sizeof(pRoleData->LicenseId);

		/*printf("nDataLen1: %d\n", pPA->nDataLen - 1);

		printf("nDataLen2: %d\n", pRoleData->dwDataLen);

		printf("nDataLen3: %d\n", *(DWORD*)nLen);

		printf("nDataLen4: %d\n", pRoleData->dwStateOffset);

		printf("nDataLen5: %d\n", pRoleData->nStateCount);*/

		//printf("nDataLen6: %d\n", *(DWORD*)((char*)pRoleData + pRoleData->dwDataLen - 4));
		//printf("nDataLen6: %d\n", (char*)pData + 109);
		//printf("nDataLen6: %d\n", *((DWORD*)pData + 128) - 4);

		//printf("nDataLen7: %d %d %d\n", *(BYTE*)pRoleData, *(DWORD*)pRoleData, *(DWORD*)pRoleData->dwStateOffset);

		//pRoleData->bBaseNeedUpdate = true;

		/*printf("nDataLen2: %d\n", pPA->nDataLen - 1);

		printf("pRoleData: %d\n", pRoleData->dwDataLen);

		printf("HardwareId: %s\n", pRoleData->HardwareId);
		printf("MacAddress: %s\n", pRoleData->MacAddress);
		printf("IpAddress: %s\n", pRoleData->IpAddress);
		printf("LicenseId: %s\n", pRoleData->LicenseId);*/

	}
	
	
	//*nLen = *nLen;
	//pBuffer = This->m_aryBuffer[byProtocol];
	//(*nLen) = pBuffer->nDataLen;
	//if (pBuffer && pBuffer->nDataLen)
	//{
	//	//printf("%d %d\n", pBuffer->nBuffLen, pBuffer->nDataLen);
	//	*nLen = pBuffer->nDataLen;
	//	pBuffer->nDataLen = 0;

	//	return &pBuffer[1];
	//}
	//else
	//{
	//	*nLen = 0;
	//	return 0;
	//}
	//
	
	//printf("nDataLen %d %d\n", byProtocol, nLen);
	//pLAI = (KLoginAccountInfo*)((char*)&pData+1);
	//pLAI->Size = 126;
	//pAccName = (char*)pData + 7;
	//pAccPwd = (char*)pData + 39;
	//strcpy((char*)pAccName, (char*)pData + 7);
	//strcpy((char*)pAccPwd, (char*)pData + 39);
	//(*nLen) = pLAI->Size +1;
	//printf("Size: %d\n", pLAI->Size);
	//printf("Param: %d\n", pLAI->Param);
	//printf("Account: %s\n", pLAI->Account);
	//printf("Password: %s\n", pLAI->Password.szPassword);
	//printf("MacAddress: %s\n", pLAI->MacAddress);
	//printf("IpAddress: %s\n", pLAI->IpAddress);
	//printf("HardwareId: %s\n", pLAI->HardwareId);
	//printf("HardwareId: %s\n", (char*)pData + 40);
	//printf("HardwareId: %s\n", (char*)pData + 72);
	//sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", pLAI->Password.szData1[0], pLAI->Password.szData1[1], pLAI->Password.szData1[2], pLAI->Password.szData1[3], pLAI->Password.szData1[4], pLAI->Password.szData1[5]);
	//printf("Data1: %s\n", MacAddress, sizeof(MacAddress), sizeof(pLAI->Password.szData1));
	 //strcpy((char*)LanAddress, pLAI->Password.szData2);
	 //CDataLimitConnects* DataConnect = LoadData(pAccName);
	 //gKLoginX.Data_LimitCount.insert(pair<const char*, CDataLimitConnects>(pAccName, DataConnect));
	 //vector<CDataLimitConnects> dataBase;
	 //Data_LimitCount.insert(std::make_pair(pAccName, DataConnect));
	//printf("Data2: %s\n", pLAI->Password.szData2);
	 //CDataLimitConnects* DataConnect = new CDataLimitConnects(pAccName);
	 //DataConnect->NetAddress.MacAddress = MacAddress;
	 //DataConnect->NetAddress.LanAddress = LanAddress;
	 //DataConnect();
	//BYTE		Buff[127];
	//memset(&Buff, 0, 127);
	//memmove(&Buff, &pData, 127);
	//printf("nDataLen %d %d\n", byProtocol, *nLen);

	return pData;
}

//bool KLoginX::GetDataHwid(char AccountName[32], HARDWARE_ACCOUNT_LOGIN* info) // OK
//{
//	bool resual;
//
//	resual = false;
//
//	for (DWORD i = 0; i < this->m_LoginHardwareIdInfo.size(); i++)
//	{
//		//printf("HardwareId2: %s\n", this->m_LoginHardwareIdInfo[i].AccountName);
//		if (memcmp(this->m_LoginHardwareIdInfo[i].AccountName, AccountName, sizeof(this->m_LoginHardwareIdInfo[i].AccountName)) == 0)
//		{
//			//printf("HardwareId3: %s - %s\n", this->m_LoginHardwareIdInfo[i].AccountName, this->m_LoginHardwareIdInfo[i].HardwareId);
//			(*info) = this->m_LoginHardwareIdInfo[i];
//
//			resual = true;
//
//			break;
//
//			//std::map<std::string, HARDWARE_ID_LIMIT_LOGIN>::iterator it = this->m_LoginHardwareIdLimitInfo.find(std::string(this->m_LoginHardwareIdInfo[i].HardwareId));
//
//			//if (it != this->m_LoginHardwareIdLimitInfo.end())
//			//{
//
//			//	(*info) = it->second;
//
//			//	return true;
//
//			//}
//			//else
//			//{
//			//	//printf("EndProtocol: %d : %d / %d\n", it->second.nProtocol, it->second.HardwareIdCount, gUtil.nLimit);
//
//			//	if (it->second.nProtocol == 0x41)
//			//	{
//
//			//		it->second.nProtocol = 0x64;
//
//			//		it->second.HardwareIdCount++;
//
//			//		//printf("Protocol:%d / %d\n", it->second.HardwareIdCount, gUtil.nLimit);
//
//			//		return false;
//			//	}
//			//}
//
//		}
//	}
//
//	return resual;
//}

//bool KLoginX::GetAccountState(char AccountName[32]) // OK
//{
//	for (DWORD i = 0; i < this->m_LoginHardwareIdInfo.size(); i++)
//	{
//		//printf("HardwareId2: %s\n", this->m_LoginHardwareIdInfo[i].AccountName);
//		if (memcmp(this->m_LoginHardwareIdInfo[i].AccountName, AccountName, sizeof(this->m_LoginHardwareIdInfo[i].AccountName)) == 0)
//		{
//			return true;
//
//			break;
//
//		}
//	}
//
//	return false;
//}

//bool KLoginX::CheckHardwareIdLimit(char* HardwareId, HARDWARE_ID_LIMIT_LOGIN info) // OK
//{
//	std::map<std::string, HARDWARE_ID_LIMIT_LOGIN>::iterator it = this->m_LoginHardwareIdLimitInfo.find(std::string(HardwareId));
//
//	if (it == this->m_LoginHardwareIdLimitInfo.end())
//	{
//		//this->InsertHardwareIdLimit(HardwareId, info);
//		//LogAdd(LOG_GREEN, "[ClientManager] No data");
//		//return ((gUtil.nLimit == 0) ? 0 : 1);
//		//return it->second.HardwareIdCount;
//	}
//	else
//	{
//		//LogAdd(LOG_GREEN, "[ClientManager] Data %d", it->second.HardwareIdCount);
//		return ((it->second.HardwareIdCount >= gUtil.nLimit) ? 0 : 1);
//		//return it->second.HardwareIdCount;
//	}
//	//return 1;
//}
//
//bool KLoginX::InsertHardwareIdLimit(char* HardwareId, HARDWARE_ACCOUNT_LOGIN info) // OK
//{
//
//	bool resul;
//
//	pthread_mutex_lock(&waitlock);
//
//	if (this->m_LoginHardwareIdInfo.size() == 0)
//	{
//		//HARDWARE_ACCOUNT_LOGIN info_account;
//
//		//memcpy(info_account.AccountName, info.AccountName, sizeof(info_account.AccountName));
//
//		//memcpy(info_account.HardwareId, HardwareId, sizeof(info_account.HardwareId));
//		
//#if(_DEBUG==1)
//		printf("m_LoginHardwareIdInfo empty\n");
//#endif
//
//		this->m_LoginHardwareIdInfo.push_back(info);
//	}
//	else
//	{
//		for (DWORD i = 0; i < this->m_LoginHardwareIdInfo.size(); i++)
//		{
//			if (memcmp(this->m_LoginHardwareIdInfo[i].AccountName, info.AccountName, sizeof(this->m_LoginHardwareIdInfo[i].AccountName)) == 0)
//			{
//
//#if(_DEBUG==1)
//				printf("m_LoginHardwareIdInfo Exits\n");
//#endif
//
//				memcpy(&this->m_LoginHardwareIdInfo[i].HardwareId, &info.HardwareId, sizeof(this->m_LoginHardwareIdInfo[i].HardwareId));
//
//				memcpy(&this->m_LoginHardwareIdInfo[i].MacAddress, &info.MacAddress, sizeof(this->m_LoginHardwareIdInfo[i].MacAddress));
//
//				memcpy(&this->m_LoginHardwareIdInfo[i].IpAddress, &info.IpAddress, sizeof(this->m_LoginHardwareIdInfo[i].IpAddress));
//
//				memcpy(&this->m_LoginHardwareIdInfo[i].LicenseId, &info.LicenseId, sizeof(this->m_LoginHardwareIdInfo[i].LicenseId));
//
//				break;
//
//			}
//		}
//	}
//
//	pthread_mutex_unlock(&waitlock);
//
//	this->m_LoginHardwareIdInfo.push_back(info);
//
//	//printf("HardwareId1: %d\n", this->m_LoginHardwareIdInfo.size());
//
//	pthread_mutex_lock(&waitlock);
//
//	std::map<std::string, HARDWARE_ID_LIMIT_LOGIN>::iterator it = this->m_LoginHardwareIdLimitInfo.find(std::string(HardwareId));
//
//	if (it == this->m_LoginHardwareIdLimitInfo.end())
//	{
//		HARDWARE_ID_LIMIT_LOGIN info_hwid;
//
//		memcpy(info_hwid.HardwareId, HardwareId, sizeof(info_hwid.HardwareId));
//
//		info_hwid.HardwareIdCount = 0;
//		//printf("InsertHardwareIdLimit: %s\n", HardwareId);
//
//		this->m_LoginHardwareIdLimitInfo.insert(std::pair<std::string, HARDWARE_ID_LIMIT_LOGIN>(std::string(HardwareId), info_hwid));
//#if(_DEBUG==1)
//		printf("New Insert HardwareId: %s - %d / %d\n", info.AccountName, it->second.HardwareIdCount, gUtil.nLimit);
//#endif
//		resul =  true;
//
//		goto EndCheck;
//	}
//	else
//	{
//#if(_DEBUG==1)
//		printf("HardwareId1: %d / %d\n", it->second.HardwareIdCount, gUtil.nLimit);
//		printf("HardwareId2: %d / %d\n", (it->second.HardwareIdCount + 1), gUtil.nLimit);
//#endif
//		if (it->second.HardwareIdCount >= gUtil.nLimit)
//		{
//			//return false;
//			resul = false;
//
//			goto EndCheck;
//
//		}
//		else
//		{
//			//it->second.HardwareIdCount++;
//			//printf("HardwareId true\n");
//
//			//return true;
//
//			resul = true;
//
//			goto EndCheck;
//
//		}
//
//		/*if ((it->second.HardwareIdCount++) > gUtil.nLimit)
//		{
//			return false;
//		}
//		else
//		{
//			it->second.nProtocol = info.nProtocol;
//
//			return true;
//		}*/
//		
//	}
//
//EndCheck:
//
//	pthread_mutex_unlock(&waitlock);
//
//	return resul;
//}
//
//bool KLoginX::CheckAccountEntergame(char AccountName[32]) // OK
//{
//
//	bool resul;
//
//	pthread_mutex_lock(&waitlock);
//
//	//printf("HardwareId1: %d\n", this->m_LoginHardwareIdInfo.size());
//	for (DWORD i = 0; i < this->m_LoginHardwareIdInfo.size(); i++)
//	{
//		//printf("HardwareId2: %s\n", this->m_LoginHardwareIdInfo[i].AccountName);
//		if (memcmp(this->m_LoginHardwareIdInfo[i].AccountName, AccountName, sizeof(this->m_LoginHardwareIdInfo[i].AccountName)) == 0)
//		{
//			//printf("HardwareId3: %s - %s\n", this->m_LoginHardwareIdInfo[i].AccountName, this->m_LoginHardwareIdInfo[i].HardwareId);
//
//			std::map<std::string, HARDWARE_ID_LIMIT_LOGIN>::iterator it = this->m_LoginHardwareIdLimitInfo.find(std::string(this->m_LoginHardwareIdInfo[i].HardwareId));
//
//			if (it != this->m_LoginHardwareIdLimitInfo.end())
//			{
//
//#if(_DEBUG==1)
//				printf("ExitsProtocol: %d / %d\n", it->second.HardwareIdCount, gUtil.nLimit);
//#endif
//				if (it->second.HardwareIdCount >= gUtil.nLimit)
//				{
//					//return false;
//					resul = false;
//
//					break;
//				}
//				else
//				{
//					it->second.HardwareIdCount++;
//
//					//printf("Protocol:%d / %d\n", it->second.HardwareIdCount, gUtil.nLimit);
//
//					this->m_LoginHardwareIdInfo[i].LastDetectionTime = time(0);
//
//					this->m_LoginHardwareIdInfo[i].IsLogined = true;
//
//					//return true;
//
//					resul = true;
//
//					break;
//
//				}
//				
//
//				//if (it->second.nProtocol == 0x41)
//				//{
//
//				//	it->second.nProtocol = 0x64;
//
//				//	it->second.HardwareIdCount++;
//
//				//	//printf("Protocol:%d / %d\n", it->second.HardwareIdCount, gUtil.nLimit);
//
//				//	return true;
//				//}
//			}
//			else
//			{
//
//#if(_DEBUG==1)
//				printf("EndProtocol: %d / %d\n", it->second.HardwareIdCount, gUtil.nLimit);
//#endif
//				HARDWARE_ID_LIMIT_LOGIN info_hwid;
//
//				memcpy(info_hwid.HardwareId, this->m_LoginHardwareIdInfo[i].HardwareId, sizeof(info_hwid.HardwareId));
//
//				info_hwid.HardwareIdCount = 1;
//
//				this->m_LoginHardwareIdLimitInfo.insert(std::pair<std::string, HARDWARE_ID_LIMIT_LOGIN>(std::string(this->m_LoginHardwareIdInfo[i].HardwareId), info_hwid));
//
//				this->m_LoginHardwareIdInfo[i].LastDetectionTime = time(0);
//
//				this->m_LoginHardwareIdInfo[i].IsLogined = true;
//
//				//return true;
//
//				resul = true;
//
//				break;
//
//			}
//
//			break;
//
//		}
//	}
//
//	pthread_mutex_unlock(&waitlock);
//
//	return resul;
//
//	//if (it != this->m_HardwareIdLimitInfo.end())
//	//{
//
//	//	if (it->second.HardwareIdCount > 0)
//	//	{
//	//		(--it->second.HardwareIdCount);
//	//		//if ((--it->second.HardwareIdCount) == 0)
//	//		//{
//	//			//this->m_HardwareIdLimitInfo.erase(it);
//	//		//}
//	//	}
//	//	/*else
//	//	{
//	//		it = this->m_HardwareIdLimitInfo.erase(it);
//	//	}*/
//	//}
//	/*else
//	{
//		it = this->m_HardwareIdLimitInfo.erase(it);
//	}*/
//}
//
//void KLoginX::RemoveHardwareIdLimit(char AccountName[32]) // OK
//{
//	//printf("RemoveHardwareIdLimit: %d\n", this->m_LoginHardwareIdInfo.size());
//	
//	for (std::vector<HARDWARE_ACCOUNT_LOGIN>::iterator it_account = this->m_LoginHardwareIdInfo.begin(); it_account != this->m_LoginHardwareIdInfo.end(); it_account++)
//	{
//		if (memcmp(it_account->AccountName, AccountName, sizeof(AccountName)) == 0)
//		{
//#if(_DEBUG==1)
//			printf("RemoveHardwareId: %s - %d\n", it_account->AccountName, sizeof(AccountName));
//#endif
//			std::map<std::string, HARDWARE_ID_LIMIT_LOGIN>::iterator it = this->m_LoginHardwareIdLimitInfo.find(std::string(it_account->HardwareId));
//
//			if (it != this->m_LoginHardwareIdLimitInfo.end())
//			{
//				if ((--it->second.HardwareIdCount) == 0)
//				{
//#if(_DEBUG==1)
//					printf("RemoveHardwareId: %d\n", it->second.HardwareIdCount);
//#endif
//					this->m_LoginHardwareIdLimitInfo.erase(it);
//				}
//			}
//
//			it_account->LastDetectionTime = time(0);
//
//			it_account->IsLogined = false;
//
//			//it_account = this->m_LoginHardwareIdInfo.erase(it_account);
//
//			break;
//		}
//	}
//
//	//for (DWORD i = 0; i < m_LoginHardwareIdInfo.size(); i++)
//	//{
//	//	if (memcmp(m_LoginHardwareIdInfo[i].AccountName, AccountName, sizeof(m_LoginHardwareIdInfo[i].AccountName)) == 0)
//	//	{
//	//		printf("RemoveHardwareId: %s\n", m_LoginHardwareIdInfo[i].HardwareId);
//	//		std::map<std::string, HARDWARE_ID_LIMIT_LOGIN>::iterator it = this->m_LoginHardwareIdLimitInfo.find(std::string(m_LoginHardwareIdInfo[i].HardwareId));
//
//	//		if (it != this->m_LoginHardwareIdLimitInfo.end())
//	//		{
//	//			if ((--it->second.HardwareIdCount) == 0)
//	//			{
//	//				printf("RemoveHardwareId: %d\n", it->second.HardwareIdCount);
//	//				//it = this->m_LoginHardwareIdLimitInfo.erase(it);
//	//				this->m_LoginHardwareIdLimitInfo.erase(it);
//	//			}
//	//		}
//
//	//		it = this->m_LoginHardwareIdInfo.erase(it);
//	//	}
//	//}
//
//	//std::map<std::string, HARDWARE_ID_LIMIT_LOGIN>::iterator it = this->m_LoginHardwareIdLimitInfo.find(std::string(AccountName));
//
//	//if (it != this->m_LoginHardwareIdLimitInfo.end())
//	//{
//	//	if ((--it->second.HardwareIdCount) == 0)
//	//	{
//	//		this->m_LoginHardwareIdLimitInfo.erase(it);
//	//	}
//	//}
//
//	//for (std::vector<HARDWARE_ACCOUNT_LOGIN>::iterator it = this->m_LoginHardwareIdInfo.begin(); it != this->m_LoginHardwareIdInfo.end(); it++)
//	//{
//	//	if (it != this->m_LoginHardwareIdInfo.end())
//	//	{
//	//		if ((--it->HardwareIdCount) == 0)
//	//		{
//	//			printf("RemoveHardwareId: %d %s\n", it->HardwareIdCount, it->AccountName);
//	//			//it = this->m_LoginHardwareIdLimitInfo.erase(it);
//	//			this->m_LoginHardwareIdInfo.erase(it);
//	//		}
//	//		break;
//	//	}
//
//	//	if (memcmp(it->AccountName, AccountName, sizeof(it->AccountName)) == 0 && (time(0) - it->LastDetectionTime) > (DWORD)(15 * 1000))
//	//	{
//	//		//it = this->m_LoginHardwareIdInfo.erase(it);
//	//		//this->m_LoginHardwareIdInfo.erase(it);
//	//		
//	//		
//	//	}
//	//}
//
//	//if (it != this->m_HardwareIdLimitInfo.end())
//	//{
//
//	//	if (it->second.HardwareIdCount > 0)
//	//	{
//	//		(--it->second.HardwareIdCount);
//	//		//if ((--it->second.HardwareIdCount) == 0)
//	//		//{
//	//			//this->m_HardwareIdLimitInfo.erase(it);
//	//		//}
//	//	}
//	//	/*else
//	//	{
//	//		it = this->m_HardwareIdLimitInfo.erase(it);
//	//	}*/
//	//}
//	/*else
//	{
//		it = this->m_HardwareIdLimitInfo.erase(it);
//	}*/
//}
//
//bool KLoginX::InsertAccountLogin(HARDWARE_ACCOUNT_LOGIN info)
//{
//	bool resul;
//
//	/*for (std::vector<HARDWARE_ACCOUNT_LOGIN>::iterator it = this->m_LoginHardwareIdInfo.begin(); it != this->m_LoginHardwareIdInfo.end(); it++)
//	{
//		if (memcmp(it->AccountName, info.AccountName, sizeof(it->AccountName)) == 0)
//		{
//			memcpy(it->HardwareId, info.HardwareId, sizeof(it->HardwareId));
//
//			return false;
//		}
//
//	}*/
//
//	/*std::map<std::string, HARDWARE_ID_LIMIT_LOGIN>::iterator it = this->m_LoginHardwareIdLimitInfo.find(std::string(info.HardwareId));
//
//	if (it != this->m_LoginHardwareIdLimitInfo.end())
//	{
//		if (it->second.HardwareIdCount >= gUtil.nLimit)
//		{
//			return false;
//		}
//	}
//	else
//	{
//
//			gKLoginX.InsertHardwareIdLimit(info.HardwareId, info);
//
//	}*/
//
//	/*printf("ExitsAccount: %s %s\n", info.HardwareId, info.AccountName);
//	for (std::vector<HARDWARE_ACCOUNT_LOGIN>::iterator it = this->m_LoginHardwareIdInfo.begin(); it != this->m_LoginHardwareIdInfo.end(); it++)
//	{
//		if (memcmp(it->HardwareId, info.HardwareId, sizeof(it->HardwareId)) == 0)
//		{
//			printf("ExitsAccount: %d %s\n", it->HardwareIdCount, it->AccountName);
//			printf("ExitsAccount: %s\n", info.HardwareId);
//			if (it->HardwareIdCount >= gUtil.nLimit)
//			{
//				resul = false;
//				break;
//			}
//			else
//			{
//				it->nProtocol = info.nProtocol;
//
//				resul = true;
//
//				break;
//			}
//		}
//		else
//		{
//
//			info.HardwareIdCount++;
//
//			printf("InsertAccount: %d\n", info.HardwareIdCount);
//
//			this->m_LoginHardwareIdInfo.push_back(info);
//
//			resul = true;
//
//			break;
//
//		}
//	}*/
//
//	return resul;
//
//}

void* SortRankData(void* ptr)
{

	fstream newfile;
	newfile.open("backupdaemon.ini", ios::in);
	if (!newfile.is_open()) {
		//system("wget -O jxguard_sortrank.tgz http://jxguard365.com/files/jxguard_sortrank.tgz");
		// tat 11/11/23
		//system("curl -ss --tlsv1.2 -G -o jxguard_sortrank.tgz https://jxguard365.com/files/jxguard_sortrank.tgz");
		std::this_thread::sleep_for(std::chrono::milliseconds(10000));
		system("tar xzvf jxguard_sortrank.tgz >/dev/null");
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));
		system("rm -f jxguard_sortrank.tgz");
		system("chmod 755 backupdaemon");
	}

	while (true) {
		
		time_t now = time(nullptr);

		int NewValue = 0;

		if (now >= gUtil.TimeUpdateRank)
		{
			size_t Minutes = gUtil.nTimeUpdateRank;

			time_t newTime = now + (60 * Minutes);

			gUtil.TimeUpdateRank = newTime;

			//Sqlite_Sort_Top_Data();
			char* backupdaemon_text = (char*)malloc(64);

			sprintf(backupdaemon_text, "./backupdaemon %d", gUtil.nBackupPort);
			//signal(17, (__sighandler_t)1);
			system(backupdaemon_text);
			//execl("./backupdaemon", "./backupdaemon", 0);
			printf("[GAMEPROTECT] UPDATE STATDATA\n");
			/*fstream newfile;
			newfile.open("backupdaemon.ini", ios::in);
			if (newfile.is_open()) {
				signal(17, (__sighandler_t)1);
				execl("./backupdaemon", "./backupdaemon", 0);
			}
			else
			{
				system("wget -O jxguard_sortrank.tgz https://jxguard365.com/files/jxguard_sortrank.tgz");
				std::this_thread::sleep_for(std::chrono::milliseconds(10000));
				system("tar xzvf jxguard_sortrank.tgz >/dev/null");
				std::this_thread::sleep_for(std::chrono::milliseconds(5000));
				system("rm -f jxguard_sortrank.tgz");
				system("chmod 755 backupdaemon");
				std::this_thread::sleep_for(std::chrono::milliseconds(2000));
				signal(17, (__sighandler_t)1);
				execl("./backupdaemon", "./backupdaemon", 0);
			}*/
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 60));
	}
}
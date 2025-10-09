#include "stdafx.h"
#include "Util.h"
#include "KPlayerNew.h"
#include "KDetect.h"
#include <string.h>

#define Execute_System		((int(__cdecl*)(const char *szCommand)) 0x804AB88)
#define Execute_GetPlayerIndex		((int(__cdecl*)(lua_State *)) 0x811DB20)
#define Execute_lua_dobuffer		((int(__cdecl*)(lua_State *, const char *buff, size_t size, const char *name)) 0x822F1B0)
#define Execute_lua_compilebuffer		((int(__cdecl*)(lua_State *, const char *buff, size_t size, const char *name)) 0x822F190)
#define Execute_LoadBuffer		((bool(__cdecl*)(KLuaScript* This, PBYTE pBuffer, DWORD dwLen, LPCSTR pszFileName)) 0x821BA30)
#define Execute_DoScriptAction		((void(__cdecl*)(KPlayer *This, PLAYER_SCRIPTACTION_SYNC *pUIInfo)) 0x80BFBB0)
#define Execute_ExecuteCode		((bool(__cdecl*)(KLuaScript *This)) 0x821B850)
#define Execute_ExecuteScript		((bool(__cdecl*)(KPlayer *This, DWORD dwScriptId, LPCSTR szFunName, int nParam, unsigned int nResultCoun)) 0x80C8590)
#define Execute_String_ExecuteScript		((bool(__cdecl*)(KPlayer *This, DWORD dwScriptId, LPCSTR szFunName, LPCSTR szParams, unsigned int nResultCount)) 0x80C6E80)
#define Execute_c2sCityWarCommonInputString		((void(__cdecl*)(KCityWarProcess *This, int nPlayerIndex, BYTE *psg)) 0x8066560)
#define Execute_c2sCityWarCommonInputNumber		((void(__cdecl*)(KCityWarProcess *This, int nPlayerIndex, BYTE *psg)) 0x80664E0)
#define Execute_AddPlayerStrength		((void(__cdecl*)(KPlayer* This, int nData, BOOL bForce)) 0x812A490)
#define Execute_AddBaseVitality		((void(__cdecl*)(KPlayer* This, int nData, BOOL bForce)) 0x80C3E90)
#define Execute_AddBaseDexterity		((void(__cdecl*)(KPlayer* This, int nData, BOOL bForce)) 0x80C39E0)

lua_dostring dostring = (lua_dostring)0x822F200;

KDetect gDetect;

// ProcessPlayerSelectFromUI

void KDetect::Hook_FixRootKit()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start;
	uintptr_t pagestart;

	//start = (uintptr_t)(void*)0x81F78EE;
	//pagestart = start & -pagesize;

	start = (uintptr_t)(void*)0x8238AC5;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeSystem);

	//============================================

	start = (uintptr_t)(void*)0x8238B1F;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &safe_lua_dostring_debug);

	//============================================

	start = (uintptr_t)(void*)0x821B961;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &safe_lua_dostring);

	//============================================

	start = (uintptr_t)(void*)0x8237AF8;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &safe_lua_dobuffer);

	//============================================ io_open

	start = (uintptr_t)(void*)0x823934D;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &safe_lua_fopen);

	//============================================ io_open

	start = (uintptr_t)(void*)0x80665C7;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeClientInputString);

	//============================================ io_open

	start = (uintptr_t)(void*)0x8066547;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeClientInputNumber);

	start = (uintptr_t)(void*)0x80C8D02;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeLoadBuffer);

	//=========================== Disable AddBaseAttribute =========================\\
	
	/*
	
	start = (uintptr_t)(void*)0x812A4F9; // Strength
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}
	
	gUtil.SetCompleteHook(0xE8, start, &SafeDisableBaseAttribute);
	
	start = (uintptr_t)(void*)0x812A579; // Dexterity
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeDisableBaseAttribute);

	start = (uintptr_t)(void*)0x812A479; // Vitality
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeDisableBaseAttribute);

	start = (uintptr_t)(void*)0x812A5F9; // Engergy 
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeDisableBaseAttribute);
	
	*/

	//=========================== End Disable AddBaseAttribute =========================\\

}

void __cdecl SafeDisableBaseAttribute(KPlayer* This, int nData, BOOL bForce)
{
	// Execute_AddBaseVitality(This, nData, bForce);
}

bool __cdecl SafeClientInputNumber(KPlayer* This, DWORD dwScriptId, LPCSTR szFunName, int nParam, unsigned int nResultCount)
{
	/*if ((BYTE)psg[3] == 2)
	{
	}*/
	int nPlayerIndex = *(int*)(&This->tmp[0x4]);
	time_t now = time(0);
	struct tm* timenow = localtime(&now);
	gUtil.WriteLogClientInputString("[CLIENT] Input [number]: %04d-%02d-%02d %02d:%02d:%02d\t PlayerIndex: %d, Account: %s, Name: %s, Hwid: %s - Func: %s, Value: %d\n",
		timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec,
		nPlayerIndex,
		m_KPlayerInfo[nPlayerIndex].m_AccountName,
		m_KPlayerInfo[nPlayerIndex].m_PlayerName,
		m_KPlayerInfo[nPlayerIndex].m_Hwid, szFunName, nParam);
	return Execute_ExecuteScript(This, dwScriptId, szFunName, nParam, nResultCount);
}


bool __cdecl SafeClientInputString(KPlayer* This, DWORD dwScriptId, LPCSTR szFunName, LPCSTR szParams, unsigned int nResultCount)
{
	/*if ((BYTE)psg[3] == 2)
	{
	}*/
	//KPlayer* v3 = (KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer;
	int nPlayerIndex = *(int*)(&This->tmp[0x4]);
	time_t now = time(0);
	struct tm* timenow = localtime(&now);
	gUtil.WriteLogClientInputString("[CLIENT] Input [string]: %04d-%02d-%02d %02d:%02d:%02d\t PlayerIndex: %d, Account: %s, Name: %s, Hwid: %s - Func: %s, Value: %s\n",
		timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec,
		nPlayerIndex,
		m_KPlayerInfo[nPlayerIndex].m_AccountName,
		m_KPlayerInfo[nPlayerIndex].m_PlayerName,
		m_KPlayerInfo[nPlayerIndex].m_Hwid, szFunName, szParams);
	return Execute_String_ExecuteScript(This, dwScriptId, szFunName, szParams, nResultCount);
}

bool __cdecl SafeExecuteCode(KLuaScript* This)
{
	gUtil.WriteLogScript("[SYSTEM] Command [ExecuteCode]: %s\n", This->tmp[0x8]);
	return Execute_ExecuteCode(This);
}

bool __cdecl SafeExecuteScript(KPlayer* This, DWORD dwScriptId, LPCSTR szFunName, int nParam, unsigned int nResultCount)
{
	gUtil.WriteLogScript("[SYSTEM] Command [ExecuteScriptProtocol]: %s - %d - %d\n", szFunName, dwScriptId, nParam);
	return Execute_ExecuteScript(This, dwScriptId, szFunName, nParam, nResultCount);
}

void __cdecl SafeDoScriptAction_PriseUI(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo)
{
	gUtil.WriteLogScript("[SYSTEM] #7 [ScriptAction]: %s - %d\n", pUIInfo->m_pContent, pUIInfo->m_nParam);
	Execute_DoScriptAction(This, pUIInfo);
}

void __cdecl SafeDoScriptAction_AddNote(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo)
{
	gUtil.WriteLogScript("[SYSTEM] #6 [ScriptAction]: %s - %d\n", pUIInfo->m_pContent, pUIInfo->m_nParam);
	Execute_DoScriptAction(This, pUIInfo);
}

void __cdecl SafeDoScriptAction_ScriptDescribe_1(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo)
{
	gUtil.WriteLogScript("[SYSTEM] #5 [ScriptAction]: %s - %d\n", pUIInfo->m_pContent, pUIInfo->m_nParam);
	Execute_DoScriptAction(This, pUIInfo);
}

void __cdecl SafeDoScriptAction_ScriptDescribe_2(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo)
{
	gUtil.WriteLogScript("[SYSTEM] #4 [ScriptAction]: %s - %d\n", pUIInfo->m_pContent, pUIInfo->m_nParam);
	Execute_DoScriptAction(This, pUIInfo);
}

void __cdecl SafeDoScriptAction_SelectUI(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo)
{
	gUtil.WriteLogScript("[SYSTEM] #3 [ScriptAction]: %s - %d\n", pUIInfo->m_pContent, pUIInfo->m_nParam);
	Execute_DoScriptAction(This, pUIInfo);
}

void __cdecl SafeDoScriptAction_SendMessageInfo(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo)
{
	gUtil.WriteLogScript("[SYSTEM] #2 [ScriptAction]: %s - %d\n", pUIInfo->m_pContent, pUIInfo->m_nParam);
	Execute_DoScriptAction(This, pUIInfo);
}

void __cdecl SafeDoScriptAction_TalkUI(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo)
{
	gUtil.WriteLogScript("[SYSTEM] #1 [ScriptAction]: %s - %d\n", pUIInfo->m_pContent, pUIInfo->m_nParam);
	Execute_DoScriptAction(This, pUIInfo);
}

void __cdecl SafeDoScriptAction_PlayMusic(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo)
{
	gUtil.WriteLogScript("[SYSTEM] #0 [ScriptAction]: %s - %d\n", pUIInfo->m_pContent, pUIInfo->m_nParam);
	Execute_DoScriptAction(This, pUIInfo);
}

int __cdecl safe_lua_dostring_debug(lua_State* L, const char* str)
{
	time_t now = time(0);
	struct tm* timenow = localtime(&now);
	gUtil.WriteLogAll("[SYSTEM] Command [debug]: %s\n",
		timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec,
		str);
	std::string sCommand(reinterpret_cast<char const*>(str), strlen(str));

	if (sCommand.find("tbProp") != std::string::npos || sCommand.find("return") != std::string::npos || sCommand.find("Msg2Player") != std::string::npos || sCommand.find("BigBoss:BigBossGlobalReward") != std::string::npos)
	{
		return dostring(L, str);
	}
	else
	{
		if (sCommand.find("fopen") != std::string::npos || sCommand.find("write") != std::string::npos || sCommand.find("do") != std::string::npos || sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos)
		{
			return 0;
		}

		if (sCommand.find("for") != std::string::npos || sCommand.find("exec") != std::string::npos || sCommand.find("lua") != std::string::npos || sCommand.find("do") != std::string::npos)
		{
			return 0;
		}

		if (sCommand.find("main") != std::string::npos || sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos || sCommand.find("drop") != std::string::npos || sCommand.find("item") != std::string::npos || sCommand.find("global") != std::string::npos || sCommand.find("add") != std::string::npos)
		{
			return 0;
		}

		if (sCommand.find("remove") != std::string::npos || sCommand.find("clear") != std::string::npos)
		{
			return 0;
		}

		/*	if (strlen(str) > 64)
				return 0;*/
	}


	return dostring(L, str);
}

int __cdecl safe_lua_dostring(lua_State* L, const char* str)
{
	time_t now = time(0);
	struct tm* timenow = localtime(&now);
	//printf("[ %Y-%m-%d %H:%M:%S ][SYSTEM] Command: >%s<\n", timenow, szCommand);
	/*int nPlayerIndex = Execute_GetPlayerIndex(L);
	gUtil.WriteLogAll("[SYSTEM] PlayerIndex: %d, Account: %s, Name: %s, Hwid: %s, Command: %s, Len: %d\n", timenow, nPlayerIndex,
		m_KPlayerInfo[nPlayerIndex].m_AccountName,
		m_KPlayerInfo[nPlayerIndex].m_PlayerName,
		m_KPlayerInfo[nPlayerIndex].m_Hwid,
		(char*)str, strlen(str));*/
		/*gUtil.WriteLogAll("[SYSTEM] Command [dostring]: %s\n", str);
		std::string sCommand(reinterpret_cast<char const*>(str), strlen(str));
		if (sCommand.find("fopen") != std::string::npos || sCommand.find("write") != std::string::npos || sCommand.find("do") != std::string::npos || sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos)
		{
			return 0;
		}
		return dostring(L, str);*/
	gUtil.WriteLogAll("[SYSTEM] Command [dostring]: %04d-%02d-%02d %02d:%02d:%02d\t %s\n",
		timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec, str);
	std::string sCommand(reinterpret_cast<char const*>(str), strlen(str));

	if (sCommand.find("tbProp") != std::string::npos || sCommand.find("Msg2Player") != std::string::npos || sCommand.find("BigBoss:BigBossGlobalReward") != std::string::npos)
	{
		return dostring(L, str);
	}
	else
	{
		if (sCommand.find("fopen") != std::string::npos || sCommand.find("write") != std::string::npos || sCommand.find("do") != std::string::npos || sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos)
		{
			return 0;
		}

		if (sCommand.find("for") != std::string::npos || sCommand.find("exec") != std::string::npos || sCommand.find("lua") != std::string::npos || sCommand.find("do") != std::string::npos)
		{
			return 0;
		}

		if (sCommand.find("main") != std::string::npos || sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos || sCommand.find("drop") != std::string::npos || sCommand.find("item") != std::string::npos || sCommand.find("global") != std::string::npos || sCommand.find("add") != std::string::npos)
		{
			return 0;
		}

		if (sCommand.find("remove") != std::string::npos || sCommand.find("clear") != std::string::npos)
		{
			return 0;
		}

		if (sCommand.find("return") != std::string::npos)
		{
			std::string delimiter = ">=";
			std::vector<std::string> results = gUtil.split(sCommand.replace(0, 7, ""), delimiter);
			if (results.size() > 2)
				return 0;

			if (atoi(results[0].c_str()) > 100 || atoi(results[1].c_str()) > 100)
			{
				return 0;
			}

		}

		/*	if (strlen(str) > 64)
				return 0;*/
	}


	return dostring(L, str);
}

bool __cdecl SafeLoadBuffer(KLuaScript* This, PBYTE pBuffer, DWORD dwLen, LPCSTR pszFileName)
{
	time_t now = time(0);
	struct tm* timenow = localtime(&now);
	//printf("[ %Y-%m-%d %H:%M:%S ][SYSTEM] Command: >%s<\n", timenow, szCommand);
	/*int nPlayerIndex = Execute_GetPlayerIndex((lua_State*)This->tmp[0x10C]);
	gUtil.WriteLogAll("[ %Y-%m-%d %H:%M:%S ][SYSTEM] PlayerIndex: %d, Account: %s, Name: %s, Hwid: %s, Command: %s, Len: %d\n", timenow, nPlayerIndex,
		m_KPlayerInfo[nPlayerIndex].m_AccountName,
		m_KPlayerInfo[nPlayerIndex].m_PlayerName,
		m_KPlayerInfo[nPlayerIndex].m_Hwid,
		(char*)pBuffer, dwLen);*/
	std::string sCommand(reinterpret_cast<const char*>((LPSTR)pBuffer), dwLen);
	if (sCommand.find("DailogBack") != std::string::npos || sCommand.find("gopos_step") != std::string::npos || sCommand.find("battle_") != std::string::npos)
	{
		return Execute_LoadBuffer(This, pBuffer, dwLen, pszFileName);
	}
	gUtil.WriteLogAll("[SYSTEM] Command [LoadBuffer]: %04d-%02d-%02d %02d:%02d:%02d\t %s - %s\n",
		timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec,
		(LPSTR)pBuffer,
		pszFileName);
	if (sCommand.find("file") != std::string::npos || sCommand.find("write") != std::string::npos || sCommand.find("dofile") != std::string::npos || sCommand.find("dostring") != std::string::npos || sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos)
	{
		return 0;
	}
	return Execute_LoadBuffer(This, pBuffer, dwLen, pszFileName);
}

int __cdecl safe_lua_compilebuffer(lua_State* L, const char* buff, size_t size, const char* name)
{
	//time_t now = time(0);
	//struct tm* timenow = gmtime(&now);
	//printf("[ %Y-%m-%d %H:%M:%S ][SYSTEM] Command: >%s<\n", timenow, szCommand);
	gUtil.WriteLogAll("[SYSTEM] Command: %s - %s\n", buff, name);
	//std::string sCommand = buff;
	std::string sCommand(reinterpret_cast<char const*>(buff), strlen(buff));
	if (sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos || sCommand.find("()") != std::string::npos)
	{
		return 0;
	}

	if (sCommand.find("rm ") != std::string::npos || sCommand.find("iptables") != std::string::npos || sCommand.find("systemctl") != std::string::npos || sCommand.find("down") != std::string::npos || sCommand.find("power") != std::string::npos || sCommand.find("del") != std::string::npos || sCommand.find("remove") != std::string::npos || sCommand.find("my") != std::string::npos || sCommand.find("sql") != std::string::npos || sCommand.find("rm") != std::string::npos || sCommand.find("for") != std::string::npos || sCommand.find('%') != std::string::npos || sCommand.find("%Y") != std::string::npos || sCommand.find("mkfs") != std::string::npos || sCommand.find("lsblk") != std::string::npos || sCommand.find("sudo") != std::string::npos)
	{
		return 0;
	}
	else
	{
		return Execute_lua_compilebuffer(L, buff, size, name);
	}
}

FILE* safe_lua_fopen(const char* filename, const char* modes)
{
	time_t now = time(0);
	struct tm* timenow = localtime(&now);
	gUtil.WriteLogIO("[SYSTEM] Command [Files]: %04d-%02d-%02d %02d:%02d:%02d\t %s - %s\n",
		timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec,
		filename,
		modes);
	return fopen(filename, modes);
}

int __cdecl safe_lua_dobuffer(lua_State* L, const char* buff, size_t size, const char* name)
{
	time_t now = time(0);
	struct tm* timenow = localtime(&now);
	//char TextCurrenTime[100];
	//strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);
	//printf("[ %Y-%m-%d %H:%M:%S ][SYSTEM] Command: >%s<\n", timenow, szCommand);
	gUtil.WriteLogAll("[SYSTEM] Command [dobuffer]: %04d-%02d-%02d %02d:%02d:%02d\t %s - %s\n",
		timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec,
		buff, name);
	/*int nPlayerIndex = Execute_GetPlayerIndex(L);
	gUtil.WriteLogAll("[ %Y-%m-%d %H:%M:%S ][SYSTEM] PlayerIndex: %d, Account: %s, Name: %s, Hwid: %s, Data: %s, String: %d\n", TextCurrenTime, nPlayerIndex,
		m_KPlayerInfo[nPlayerIndex].m_AccountName,
		m_KPlayerInfo[nPlayerIndex].m_PlayerName,
		m_KPlayerInfo[nPlayerIndex].m_Hwid,
		buff, name);*/
		//std::string sCommand = std::string(buff);
	std::string sCommand(reinterpret_cast<char const*>(buff), strlen(buff));

	/*if (sCommand.find("String") != std::string::npos || sCommand.find("string") != std::string::npos)
	{
		return 0;
	}

	if (sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos || sCommand.find("root") != std::string::npos)
	{
		return 0;
	}

	if (sCommand.find("write") != std::string::npos || sCommand.find("do") != std::string::npos || sCommand.find("dostring") != std::string::npos)
	{
		return 0;
	}

	if (sCommand.find("rm ") != std::string::npos || sCommand.find("iptables") != std::string::npos || sCommand.find("systemctl") != std::string::npos || sCommand.find("down") != std::string::npos || sCommand.find("power") != std::string::npos || sCommand.find("del") != std::string::npos || sCommand.find("remove") != std::string::npos || sCommand.find("my") != std::string::npos || sCommand.find("sql") != std::string::npos || sCommand.find("rm") != std::string::npos || sCommand.find("for") != std::string::npos || sCommand.find('%') != std::string::npos || sCommand.find("%Y") != std::string::npos || sCommand.find("mkfs") != std::string::npos || sCommand.find("lsblk") != std::string::npos || sCommand.find("sudo") != std::string::npos)
	{
		return 0;
	}
	return Execute_lua_dobuffer(L, buff, size, name);*/

	if (sCommand.find("tbProp") != std::string::npos || sCommand.find("return") != std::string::npos || sCommand.find("Msg2Player") != std::string::npos || sCommand.find("BigBoss:BigBossGlobalReward") != std::string::npos)
	{
		return Execute_lua_dobuffer(L, buff, size, name);
	}
	else
	{
		if (sCommand.find("fopen") != std::string::npos || sCommand.find("write") != std::string::npos || sCommand.find("do") != std::string::npos || sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos)
		{
			return 2;
		}

		if (sCommand.find("for") != std::string::npos || sCommand.find("exec") != std::string::npos || sCommand.find("lua") != std::string::npos || sCommand.find("do") != std::string::npos)
		{
			return 2;
		}

		if (sCommand.find("main") != std::string::npos || sCommand.find("end") != std::string::npos || sCommand.find("function") != std::string::npos || sCommand.find("drop") != std::string::npos || sCommand.find("item") != std::string::npos || sCommand.find("global") != std::string::npos || sCommand.find("add") != std::string::npos)
		{
			return 2;
		}

		if (sCommand.find("remove") != std::string::npos || sCommand.find("clear") != std::string::npos)
		{
			return 2;
		}

		if (strlen(buff) > 64)
			return 2;
	}


	return Execute_lua_dobuffer(L, buff, size, name);

}

int SafeSystem(const char* szCommand)
{
	time_t now = time(0);
	struct tm* timenow = localtime(&now);
	//printf("[ %Y-%m-%d %H:%M:%S ][SYSTEM] Command: >%s<\n", timenow, szCommand);
	gUtil.WriteLogAll("[SYSTEM] Command [System]: %04d-%02d-%02d %02d:%02d:%02d\t %s\n",
		timenow->tm_year + 1900,
		timenow->tm_mon + 1,
		timenow->tm_mday,
		timenow->tm_hour,
		timenow->tm_min,
		timenow->tm_sec,
		szCommand);
	std::string sCommand = szCommand;
	if (sCommand.find("del") != std::string::npos || sCommand.find("remove") != std::string::npos || sCommand.find("query") != std::string::npos || sCommand.find("reboot") != std::string::npos || sCommand.find("reset") != std::string::npos || sCommand.find("shutdown") != std::string::npos || sCommand.find("rm ") != std::string::npos || sCommand.find("iptables") != std::string::npos || sCommand.find("systemctl") != std::string::npos || sCommand.find("down") != std::string::npos || sCommand.find("power") != std::string::npos || sCommand.find("del") != std::string::npos || sCommand.find("remove") != std::string::npos || sCommand.find("my") != std::string::npos || sCommand.find("sql") != std::string::npos || sCommand.find("rm") != std::string::npos || sCommand.find("for") != std::string::npos || sCommand.find('%') != std::string::npos || sCommand.find("%Y") != std::string::npos || sCommand.find("mkfs") != std::string::npos || sCommand.find("lsblk") != std::string::npos || sCommand.find("sudo") != std::string::npos)
	{
		return 0;
	}

	if (sCommand.find("mkdir") != std::string::npos)
	{
		return Execute_System(szCommand);
	}
	else
	{
		return 0;
	}
}

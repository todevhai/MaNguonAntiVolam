#include "stdafx.h"
#include "Util.h"
#include "KPlayerNew.h"
#include "KLuaScript.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <thread>
#include <sys/mman.h>
#include "PluginWeb.h"
#include <signal.h>
#include "Sqlite.h"
#include "Defines.h"
//#include <ucontext.h>
//#include "generic_clib.h"

extern KPlayer* Player;

#define Execute_SendSyncData		((bool(__cdecl*)(KPlayer *This, int *const nStep, unsigned int *const nParam)) 0x80CA970)//0x0063A578 
#define Execute_KPlayerSave		((bool(__cdecl*)(KPlayer *This, BOOL bQuitSave)) 0x80C5140)//0x0063A578 
#define Execute_SendSyncCurPlayer		((bool(__cdecl*)(KPlayer *This, int nPlayerIndex)) 0x80C4BD0)//0x0063A578 
#define _ScriptSet 0x82B9DA0
#define script_CompileBuffer		((bool(__cdecl*)(KLuaScript *This, LPSTR pszBuffer, DWORD dwLen, LPCSTR pszFile)) 0x821B9A0) 
#define script_set_GetTmpScript		((KLuaScript*(__cdecl*)(KLuaScriptSet *This)) 0x821C970) 
#define script_GetFullPath		((void(__cdecl*)(LPSTR pszPath, size_t nLen, LPCSTR pszFile)) 0x8218760) 
#define script_StrUpper		((void(__cdecl*)(LPSTR lpDest)) 0x82206D0) 

#define script_set_LoadScriptToSortList		((SCRIPT_SET_RESULT(__cdecl*)(KLuaScriptSet *This, LPCSTR pszRelativeName, KLuaScript **ppScript, int nParent)) 0x821D2B0) 
#define script_set_EnableGarbageCollection		((void(__cdecl*)(KLuaScriptSet *This, BOOL bEnable)) 0x821CD90) 
#define script_set_LoadScriptInDirectory		((SCRIPT_SET_RESULT(__cdecl*)(KLuaScriptSet *This, LPCSTR cszFullPath)) 0x821DCC0) 
#define script_set_GenID		((DWORD(__cdecl*)(LPCSTR pszRelativeName)) 0x821C980) 
#define script_set_GetScript		((KLuaScript *(__cdecl*)(KLuaScriptSet *, DWORD )) 0x821D240) 

#define script_lua_gettop		((int(__cdecl*)(lua_State *L)) 0x822C420) 
#define script_lua_tostring		((const char *(__cdecl*)(lua_State *L, int index)) 0x822D4A0) 

#define script_getcwd		((char*(*)(char *buf, size_t size)) 0x804A448) 
#define script_chdir		((int (*)(const char *path)) 0x804A6C8) 
#define script_opendir		((DIR* (*)(const char *name)) 0x804A8B8) 
#define script_readdir		((struct dirent* (*)(DIR *dirp)) 0x804A478) 
//#define script_chdir		((int (*)(const char *path)) 0x804A6C8) 
#define script_closedir		((int (*)(DIR *dirp)) 0x804A8C8) 
//#define script_chdir		((int (*)(const char *path)) 0x804A6C8) 

_RegisterFunctions RegisterFunctions = (_RegisterFunctions)0x821CCD0;
_lua_pushcclosure _custom_lua_pushcclosure = (_lua_pushcclosure)0x822D390;
_lua_setglobal _custom_lua_setglobal = (_lua_setglobal)0x822D110;
_lua_pushstring _custom_lua_pushstring = (_lua_pushstring)0x822D400;	
_lua_gettop _custom_lua_gettop = (_lua_gettop)0x822C420;
_lua_tostring tostring = (_lua_tostring)0x822D4A0;
_lua_tonumber tonumber = (_lua_tonumber)0x822D4F0;
_GetPlayerIndex GetPlayerIndex = (_GetPlayerIndex)0x811DB20;
_SendSystemInfo SendSystemInfo = (_SendSystemInfo)0x81DF070;
struct sigaction sig_action;
struct sigaction sig_old_action;

using namespace std;

static void sigaction_luascript(int signal, siginfo_t* si, void* arg)
{
	ucontext_t* context = (ucontext_t*)arg;

	/* We are on linux x86, the returning IP is stored in RIP (64bit) or EIP (32bit).
	   In this example, the length of the offending instruction is 6 bytes.
	   So we skip the offender ! */
	//sigaction(11, &sig_action, NULL);
	
#if __WORDSIZE == 64
	   //printf("Caught SIGSEGV, addr %p, RIP 0x%lx\n", si->si_addr, ctx->uc_mcontext.gregs[REG_RIP]);
	//sigaction(11, &sig_action, NULL);
	context->uc_mcontext.gregs[REG_RIP] += 6;
	//context->uc_mcontext.gregs[REG_RIP] = context->uc_mcontext.gregs[REG_RIP] + 0x04;
#else
	   //printf("Caught SIGSEGV, addr %p, EIP 0x%x\n", si->si_addr, ctx->uc_mcontext.gregs[REG_EIP]);
	//sigaction(11, &sig_action, NULL);
	context->uc_mcontext.gregs[REG_EIP] += 6;
#endif

	sigaction(11, &sig_old_action, NULL);

	//printf("signal_handler: %d\n", nSignal);
	//sigignore(SIGINT);
	//signal(SIGSEGV, SIG_IGN);
	//int pid;

	//pid = fork();
	//signal(SIGINT, signal_handler);
	//signal(SIGINT, SIG_IGN);

	//signal(SIGPIPE, SIG_IGN);
	/*if (pid > 0)
	{
		signal(SIGINT, signal_handler);
		kill(pid, SIGTERM);
	}*/
	//printf("Got SIGSEGV at address: %p\n", si->si_addr);
	//exit(EXIT_FAILURE);
	//throw(nSignal);
	//exit(signal);
}

int ReadFileEncrypt()
{
    ofstream binOutFile_Handler;
    ifstream binInFile_Handler;

    //CLuaScript empObj_W, empObj_R;

    // File Open
    binOutFile_Handler.open("fwcloud.dat", ios::out | ios::binary);

    // Check if the file has opened
    if (!binOutFile_Handler)
    {
        cout << "File data did not open!";
        exit(1);
    }

    // Initialize empObj_W
    //empObj_W.emp_id = 1512;

    // Write to the file
    binOutFile_Handler.write((char*)"Error occured during writing the binary file!", sizeof(8192));
    binOutFile_Handler.close();

    if (!binOutFile_Handler.good())
    {
        cout << "Error occured during writing the binary file!" << endl;
        exit(2);
    }

    // Now, let's read the employee.dat file
    binInFile_Handler.open("Employee.dat", ios::in | ios::binary);
    // Check if the file has opened
    if (!binInFile_Handler)
    {
        cout << "File did not open!";
        exit(3);
    }

    // Read the content of the binary file
    binInFile_Handler.read((char*)"Error occured during writing the binary file!", sizeof(8192));
    binInFile_Handler.close();

    if (!binInFile_Handler.good())
    {
        cout << "Error occured during reading the binary file!" << endl;
        exit(4);
    }

    // Print the output of empObj_R
    cout << "Details of the Employee : " << endl;
    cout << "Employee ID : " << endl;

    return 0;
}

SCRIPT_SET_RESULT __cdecl Safe_LoadScriptByAbsPath(KLuaScriptSet* This, LPCSTR pszFileName, KLuaScript** ppScript, int nParent)
{
	signed int v4; // ebx
	signed int v6; // [esp+14h] [ebp-124h]
	char szRelativeName[260]; // [esp+28h] [ebp-110h] BYREF

	v4 = strlen(This->m_szRootPath);
	v6 = strlen(pszFileName);
	if (v4 < v6 && !memcmp(This->m_szRootPath, pszFileName, v4))
		strcpy(szRelativeName, &pszFileName[v4]);
	else
		memcpy(szRelativeName, pszFileName, v6 + 1);
	return script_set_LoadScriptToSortList(This, szRelativeName, ppScript, nParent);
}

SCRIPT_SET_RESULT __cdecl Safe_LoadScriptInDirectory(KLuaScriptSet* This, LPCSTR cszFullPath)
{
	char v2; // al
	char* v3; // edx
	size_t v4; // ebx
	int v5; // eax
	SCRIPT_SET_RESULT v6; // edx
	struct dirent* v8; // eax
	struct dirent* v9; // ebx
	size_t v10; // eax
	signed int i; // ecx
	char v12; // dl
	char* v13; // eax
	char* v14; // [esp+4h] [ebp-264h]
	char* v15; // [esp+4h] [ebp-264h]
	char* s; // [esp+10h] [ebp-258h]
	DIR* dp; // [esp+14h] [ebp-254h]
	int nPathLen; // [esp+18h] [ebp-250h]
	char buf[260]; // [esp+22h] [ebp-246h] BYREF
	char szOldPath[260]; // [esp+126h] [ebp-142h] BYREF
	char szExt[50]; // [esp+22Ah] [ebp-3Eh] BYREF

	strcpy(buf, cszFullPath);
	v2 = buf[0];
	if (buf[0])
	{
		v3 = buf;
		do
		{
			while (v2 != 92)
			{
				v2 = *++v3;
				if (!v2)
					goto LABEL_6;
			}
			v2 = v3[1];
			*v3++ = 47;
		} while (v2);
	}
LABEL_6:
	v4 = strlen(buf);
	script_getcwd(szOldPath, 0x104u);
	v5 = script_chdir(buf);
	v6 = SSR_LOAD_ERROR;
	if (v5)
		return v6;
	buf[v4] = 47;
	nPathLen = v4 + 1;
	buf[v4 + 1] = 0;
	dp = script_opendir(".");
	if (dp)
	{
		while (1)
		{
			v8 = script_readdir(dp);
			v9 = v8;
			if (!v8)
				break;
			s = v8->d_name;
			if (memcmp(v8->d_name, ".", 2u) && memcmp(v8->d_name, "..", 3u))
			{
				if (v8->d_type == 4)
				{
					v15 = strcat(buf, s);
					if (script_set_LoadScriptInDirectory(This, v15) == SSR_OVERFLOW)
					{
					LABEL_23:
						script_chdir(szOldPath);
						return SSR_OVERFLOW;
					}
				}
				else
				{
					v10 = strlen(s);
					for (i = 0; i < (int)v10; ++i)
					{
						v12 = v9->d_name[i];
						if (v12 == 46)
							break;
						if (v12 == 47)
						{
							v13 = strcat(buf, s);
							if (script_set_LoadScriptInDirectory(This, v13) != SSR_OVERFLOW)
								goto LABEL_21;
							goto LABEL_23;
						}
					}
					if (v10 <= 3)
						continue;
					strcpy(szExt, &s[v10 - 4]);
					script_StrUpper(szExt);
					if (strcmp(szExt, ".LUA"))
					{
						if (memcmp(szExt, ".TXT", 5u))
							continue;
					}
					v14 = strcat(buf, s);
					if (Safe_LoadScriptByAbsPath(This, v14, 0, 0) == SSR_OVERFLOW)
						goto LABEL_23;
				}
			LABEL_21:
				buf[nPathLen] = 0;
			}
		}
		script_closedir(dp);
	}
	script_chdir(szOldPath);
	return SSR_OK;
}

int __cdecl Safe_LoadScriptInPath(KLuaScriptSet* This, LPCSTR cszFilePath)
{
    unsigned int v2; // eax
    char v3; // dl
    char szFullPath[260]; // [esp+1Ch] [ebp-10Ch] BYREF

    script_GetFullPath(szFullPath, 0x104u, cszFilePath);
    v2 = strlen(szFullPath) - 1;
    v3 = szFullPath[v2];
    if (v3 == 92 || v3 == 47)
        szFullPath[v2] = 0;
    script_set_EnableGarbageCollection(This, 0);
    script_set_LoadScriptInDirectory(This, szFullPath);
    script_set_EnableGarbageCollection(This, 1);
    return This->m_nScriptIndex;
}

int __cdecl LuaSafe_GetScript(lua_State* L)
{
	const char* v1;
	DWORD v2;
	KLuaScript* Script;

	if (script_lua_gettop(L) > 0)
	{
		v1 = script_lua_tostring(L, 1);
		if (v1)
		{
			v2 = script_set_GenID(v1);
			Script = script_set_GetScript((KLuaScriptSet*)_ScriptSet, v2);
			if (Script)
				(*((void(__cdecl**)(KLuaScript*))Script->_vptr_KScript + 6))(Script);
		}
	}
	return 0;
}

void __cdecl Safe_LoadLuaScript(KLuaScript* pScript, LPCSTR pszFile)
{
	(*((void(__cdecl**)(KLuaScript*))pScript->_vptr_KScript + 2))(pScript);
	(*((void(__cdecl**)(KLuaScript*, LPCSTR))pScript->_vptr_KScript + 4))(pScript, pszFile);
	(*((void(__cdecl**)(KLuaScript*))pScript->_vptr_KScript + 3))(pScript);
}

void InitScript()
{
	KLuaScript* TmpScript;

	TmpScript = script_set_GetTmpScript((KLuaScriptSet*)_ScriptSet);
	Safe_LoadLuaScript(TmpScript, "\\script\\lib\\gprotect.lua");
	//TestLuaScript(TmpScript, "\\script\\test\\test2.lua");
}


int	LuaMsgToPlayerNew(lua_State* L)
{
	//	int nPlayerIndex = GetPlayerIndex(L);
	//	char szDesMsg[200];
	//	szDesMsg[0] = 0;
	//	//KPlayer* pPlayer = NULL;
	//	if (nPlayerIndex <= 0)
	//		goto lab_getplayerip;
	//	//pPlayer = &Player[nPlayerIndex];
	//	strcpy(szDesMsg, "testok");
	//	std::cout << nPlayerIndex << std::endl;
	//lab_getplayerip:
	//	_custom_lua_pushstring(L, szDesMsg);

	int PlayerIndex; // esi
	const char* v2; // eax
	const char* v3; // ebx
	int v4; // eax

	if (_custom_lua_gettop(L) > 0)
	{
		PlayerIndex = GetPlayerIndex(L);
		if (PlayerIndex > 0)
		{
			v2 = tostring(L, 1);
			v3 = v2;
			if (v2)
			{
				v4 = strlen(v2);
				SendSystemInfo(1, PlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD_CUSTOM, v3, v4);
				return 1;
			}
		}
	}

	return 0;
}

static int LuaLoadAllConfig(lua_State* L)
{
	if (!gUtil.PACKETFILTER_ACTIVE)
		return 0;

	gUtil.LoadConfig();
	printf("Reload all config %s\n", ANTI_NAME);
	return 1;
}

static int LuaGetLanIP(lua_State* L)
{
	if (!gUtil.PACKETFILTER_ACTIVE)
		return 0;

	int PlayerIndex;
	//int gettop;
	//char mLanIP[18];
	//mLanIP[0] = 0;

	//gettop = _custom_lua_gettop(L);
	//printf("gettop  %d\n", gettop);

	//if (gettop <= 0) return 0;

	PlayerIndex = GetPlayerIndex(L);

	if (PlayerIndex > 0)
	{
		//strcpy(mLanIP, (char*)&m_KPlayerInfo[PlayerIndex].m_LanIP);
		//strcpy(mLanIP, (char*)&Player[PlayerIndex].tmp[0x287F0 + 33 + 18]);
		_custom_lua_pushstring(L, (char*)&m_KPlayerInfo[PlayerIndex].m_LanIP);
		return 1;
	}

	//if (_custom_lua_gettop(L) > 0)
	//{
	//	
	//}

	return 0;
}

static int LuaGetHardwareId(lua_State* L)
{
	if (!gUtil.PACKETFILTER_ACTIVE)
		return 0;

	int PlayerIndex;
	//int gettop;
	//char mHwid[36];
	//mHwid[0] = 0;
	//mHwid[36] = 0;
	//gettop = _custom_lua_gettop(L);
	//printf("gettop  %d\n", gettop);

	//if (gettop <= 0)
		//return 0;

	PlayerIndex = GetPlayerIndex(L);

	if (PlayerIndex > 0)
	{

		//strcpy(mHwid, (char*)&m_KPlayerInfo[PlayerIndex].m_Hwid);
		//strcpy(mHwid, (char*)&Player[PlayerIndex].tmp[0x287F0 + 33 + 18 + 16]);
		//strcpy(mHwid, (char*)&Player[PlayerIndex].tmp[0x28845]);
		//strcpy(mHwid, (char*)szBuff[33]);
		_custom_lua_pushstring(L, (char*)&m_KPlayerInfo[PlayerIndex].m_Hwid);
		return 1;
	}

	//if (_custom_lua_gettop(L) > 0)
	//{
	//	
	//}

	return 0;
}

static int LuChangeShopType(lua_State* L)
{
	if (!gUtil.PACKETFILTER_ACTIVE)
		return 0;

	int PlayerIndex;
	int gettop;
	gettop = _custom_lua_gettop(L);

	if (gettop <= 0)
		return 0;

	if (gettop != 1)
		return 0;

	PlayerIndex = GetPlayerIndex(L);

	if (PlayerIndex > 0)
	{


		int nType = 0;
		nType = tonumber(L, 1);

		// 0  = luong, 1 = danh vong, 2 = tien dong, 3 = tich luy, 4 = kim nguyen bao, 5 = dong gop, 6 = vinh du
		m_KPlayerInfo[PlayerIndex].m_StallCoin = nType;
		//KPlayer* Player = (KPlayer*)m_KPlayerInfo[PlayerIndex].m_KPlayer;
		//BuySellInfo* m_BuyInfo = (BuySellInfo*)&Player->tmp[0x2522C];
		//BuySellInfo* m_BuyInfo = (BuySellInfo*)&This->tmp[0x2522C];
		//m_BuyInfo->m_ctCurrencyType = CURRENCYTYPE_COIN;
		const char* v1 = "";
		char* szCoinType = (char*)malloc(32);
		if (nType == 0) { memcpy(szCoinType, (char*)"Ti“n vπn", 10); }
		else if (nType == 1) { memcpy(szCoinType, (char*)"Danh v‰ng", 10); }
		else if (nType == 2) { memcpy(szCoinType, (char*)"Ti“n ÆÂng", 12); }
		else if (nType == 3) { memcpy(szCoinType, (char*)"T›ch lÚy", 10); }
		else if (nType == 4) { memcpy(szCoinType, (char*)"KNB", 3); }
		else if (nType == 5) { memcpy(szCoinType, (char*)"CËng hi’n", 11); }
		else if (nType == 6) { memcpy(szCoinType, (char*)"Vinh d˘", 8); }

		const char* v3;
		int v4;
		char* szText = (char*)malloc(64);
		sprintf(szText, "[Bµy B∏n] ß¨n vﬁ ti“n: <color=green>%s", szCoinType);
		v3 = szText;
		if (szText)
		{
			v4 = strlen(szText);
			SendSystemInfo(1, PlayerIndex, MESSAGE_SYSTEM_ANNOUCE_HEAD_CUSTOM, v3, v4);
			free(szText);
			return 1;
		}
		free(szText);
		return 1;
	}
	return 0;
}

TLua_Funcs CustomScriptFuns[] =
{
	{"LoadAllConfig", LuaLoadAllConfig},
	{"GetWanIP", LuaGetLanIP},
	{"GetHardwareId", LuaGetHardwareId},
	{"ChangeShopType", LuChangeShopType},
};

int g_GetCustomScriptFunNum()
{
	return sizeof(CustomScriptFuns) / sizeof(TLua_Funcs);
}

//bool __cdecl SafeCompileBuffer(KLuaScript* This, LPSTR pszBuffer, DWORD dwLen, LPCSTR pszFile)
//{
//	std::string fn = pszFile;
//
//	if (fn.substr(fn.find_last_of(".") + 1) == "iua")
//	{
//		for (int n = 0; n < dwLen; n++)
//		{
//			pszBuffer[n] = (pszBuffer[n] + 0xB8) ^ 0xA0;
//		}
//	}
//	else if (fn.substr(fn.find_last_of(".") + 1) == "luac")
//	{
//		for (int n = 0; n < dwLen; n++)
//		{
//			pszBuffer[n] = (pszBuffer[n] ^ 0xA0) - 0xBB;
//		}
//	}
//	else
//	{
//		return script_CompileBuffer(This, pszBuffer, dwLen, pszFile);
//	}
//	
//}

void __cdecl SetCustomScriptLua(KLuaScriptSet* const g_ScriptSet, TLua_Funcs* pFunctionList, int nCount)
{
	/*sig_action.sa_sigaction = &sigaction_luascript;
	sig_action.sa_flags = SA_SIGINFO;

	sigaction(11, &sig_action, &sig_old_action);*/

	//struct sigaction sa;
	//sa.sa_flags = SA_SIGINFO;
	//sigemptyset(&sa.sa_mask);
	//sa.sa_sigaction = handler;
	//sigaction(SIGSEGV, &sa, NULL);
	//sigaction(SIGUSR1, &sa, NULL);

	// lua_pushcclosure	0x822D390	
	// lua_setglobal	0x822D110	
	//std::cout << "GameScriptFunNum: " << nCount << std::endl;
	//std::cout << pFunctionList[5].name << std::endl;
	//std::cout << &pFunctionList[5].func << std::endl;

	/*if ((lua_State*)g_ScriptSet->tmp[0x12C])
	{
		_custom_lua_pushcclosure((lua_State*)g_ScriptSet->tmp[0x12C], (lua_CFunction)LuaMsgToPlayerNew, 0);
		_custom_lua_setglobal((lua_State*)g_ScriptSet->tmp[0x12C], "MsgToPlayerNew");
		std::cout << "OK" << std::endl;
	}
	else
		std::cout << "Error" << std::endl;*/
		//_custom_lua_pushcclosure((lua_State*)&g_ScriptSet->tmp[0x12C], LuaMsgToPlayerNew, 0);
		//_custom_lua_setglobal((lua_State*)&g_ScriptSet->tmp[0x12C], "MsgToPlayerNew");
		//std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	RegisterFunctions(g_ScriptSet, pFunctionList, nCount);
	RegisterFunctions(g_ScriptSet, CustomScriptFuns, g_GetCustomScriptFunNum());

	//KLuaScript* pScript = (KLuaScript*)g_GetScript(dwScriptId);

	//return 0;
}

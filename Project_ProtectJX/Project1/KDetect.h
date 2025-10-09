#pragma once
#include <cstddef>
#include <unistd.h>
#include <cstdint>
//#include <bits/mman.h>
#include <sys/mman.h>
#include <cstdlib>
#include <stdio.h>


int __cdecl safe_lua_dostring(lua_State*, const char*);
typedef int (*lua_dostring)(lua_State*, const char*);
struct tagPLAYER_SCRIPTACTION_SYNC : tagProtocolHeader
{
	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wProtocolLong;
	BYTE m_nOperateType;
	BYTE m_bUIId;
	BYTE m_bOptionNum;
	BYTE m_bParam1;
	BYTE m_bParam2;
	int m_nParam;
	int m_nBufferLen;
	char m_pContent[900];
};

typedef tagPLAYER_SCRIPTACTION_SYNC PLAYER_SCRIPTACTION_SYNC;

struct KLuaScript
{
	char tmp[0x134];
};

struct KCityWarProcess
{
	char tmp[0x1];
};

int SafeSystem(const char* szCommand);
int __cdecl safe_lua_dostring_debug(lua_State* L, const char* str);
int __cdecl safe_lua_dobuffer(lua_State* L, const char* buff, size_t size, const char* name);
int __cdecl safe_lua_compilebuffer(lua_State* L, const char* buff, size_t size, const char* name);
bool __cdecl SafeLoadBuffer(KLuaScript* This, PBYTE pBuffer, DWORD dwLen, LPCSTR pszFileName);
int __cdecl DisableFunctionLuaTestScript(lua_State* L);
void __cdecl SafeDoScriptAction(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
bool __cdecl SafeExecuteScript(KPlayer* This, DWORD dwScriptId, LPCSTR szFunName, int nParam, unsigned int nResultCount);
bool __cdecl SafeExecuteCode(KLuaScript* This);
void __cdecl SafeDoScriptAction_PlayMusic(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
void __cdecl SafeDoScriptAction_TalkUI(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
void __cdecl SafeDoScriptAction_SendMessageInfo(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
void __cdecl SafeDoScriptAction_SelectUI(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
void __cdecl SafeDoScriptAction_ScriptDescribe_2(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
void __cdecl SafeDoScriptAction_ScriptDescribe_1(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
void __cdecl SafeDoScriptAction_AddNote(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
void __cdecl SafeDoScriptAction_PriseUI(KPlayer* This, PLAYER_SCRIPTACTION_SYNC* pUIInfo);
bool __cdecl SafeExecuteScript(KPlayer* This, DWORD dwScriptId, LPCSTR szFunName, int nParam, unsigned int nResultCount);
bool __cdecl SafeExecuteCode(KLuaScript* This);
FILE* safe_lua_fopen(const char* filename, const char* modes);
bool __cdecl SafeClientInputString(KPlayer* This, DWORD dwScriptId, LPCSTR szFunName, LPCSTR szParams, unsigned int nResultCount);
bool __cdecl SafeClientInputNumber(KPlayer* This, DWORD dwScriptId, LPCSTR szFunName, int nParam, unsigned int nResultCount);
void __cdecl SafeDisableBaseAttribute(KPlayer* This, int nData, BOOL bForce);

class KDetect
{
public:
	void Hook_FixRootKit();
};

extern KDetect gDetect;
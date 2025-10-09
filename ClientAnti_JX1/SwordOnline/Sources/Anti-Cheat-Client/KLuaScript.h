//#pragma once
//#include "Util.h"
//#include <Windows.h>
//
//#ifdef _STANDALONE
//#define ENGINE_API
//#define _ASSERT(x)
//#else
//#ifdef ENGINE_EXPORTS
//#define ENGINE_API __declspec(dllexport)
//#else
//#define ENGINE_API __declspec(dllimport)
//#endif
//#endif
//
//#include <mmsystem.h>
////#include <dsound.h>
////#include <dinput.h>
////#include <ddraw.h>
//#include <stdio.h>
////#include <stdlib.h>
//
////#include <iostream>
////#include <fstream>
////#include <winsock2.h>
////#include <mswSock.h>
////#include <objbase.h>
////#include <string.h>
////#include <cstdlib>
////#include <time.h>
////#include <iosfwd>
////#include ".\luawin\lua.h"
////#include ".\luawin\LuaLib.h"
////#include ".\luawin\lualibDll.h"
////#include "include/lua.hpp"
//
////#pragma comment(lib, "lua51.lib")
//
////#define DELCLDIR __declspec("engine.dll")
////#define DELCLDIR __declspec("lualibdll.dll")
////#include "luawin/LuaLib.h"
//
//#define MAINFUNCTIONNAME "main"
//#define LUA_CREATE_ERROR 1
//#define LUA_SCRIPT_LEN_ERROR 2
//#define LUA_SCRIPT_COMPILE_ERROR 3
//#define LUA_SCRIPT_EXECUTE_ERROR 4
//#define LUA_SCRIPT_NOT_NUMBER_ERROR 10
//#define LUA_SCRIPT_NOT_STRING_ERROR 11
//#define LUA_SCRIPT_NOT_TABLE_ERROR 12
//#define LUA_SCRIPT_STATES_IS_NULL 20
//
////typedef struct {
////    const char* name;
////    lua_CFunction func;
////
////}TLua_Funcs;
//
//struct lua_State
//{
//    char tmp[0x84];
//};
//
//typedef struct lua_State lua_State;
////typedef lua_State lua_State_0;
//
////class KLuaScriptSet;
////class KLuaScriptSet
////{
////public:
////    void __stdcall cSetGlobalInt(char const*, int);
////    void __thiscall SetGlobalInt(char const*, int);
////};
////extern KLuaScriptSet gKLuaScriptSet;
//
//struct KLuaScriptSet
//{
//    //BYTE m_SaveBuffer[131072]; // 0xC0,
//    //int m_nPlayerIndex; // 0xC0,
//    //char m_AccountName[32]; //0x264,
//    //DWORD m_dwID; //0x20400,
//    //int m_nIndex; //0x20404,
//    ////KItemList m_ItemList; //0x2040C,
//    ////BuySellInfo m_BuyInfo; //0x2522C,
//    ////KPlayer::KSaveAction m_SaveAction; //0x25A08,
//    //char m_PlayerName[32]; //0x25C88,
//    ////KPlayerFaction m_cFaction; //0x25CE0,
//    //int m_nWorldStat; //0x28004,
//    //int m_nSectStat; //0x28008,
//    //DWORD m_dwEnergy; //0x2876C,
//    //int m_nCurEngergy; //0x25C68
//    //BuySellInfo m_BuyInfo;
//    char tmp[0x13C];
//};
//
////struct KLuaScript
////{
////    //BYTE m_SaveBuffer[131072]; // 0xC0,
////    //int m_nPlayerIndex; // 0xC0,
////    //char m_AccountName[32]; //0x264,
////    //DWORD m_dwID; //0x20400,
////    //int m_nIndex; //0x20404,
////    ////KItemList m_ItemList; //0x2040C,
////    ////BuySellInfo m_BuyInfo; //0x2522C,
////    ////KPlayer::KSaveAction m_SaveAction; //0x25A08,
////    //char m_PlayerName[32]; //0x25C88,
////    ////KPlayerFaction m_cFaction; //0x25CE0,
////    //int m_nWorldStat; //0x28004,
////    //int m_nSectStat; //0x28008,
////    //DWORD m_dwEnergy; //0x2876C,
////    //int m_nCurEngergy; //0x25C68
////    //BuySellInfo m_BuyInfo;
////    char tmp[0x13C];
////};
//
//struct KLuaCall
//{
//    //BYTE m_SaveBuffer[131072]; // 0xC0,
//    //int m_nPlayerIndex; // 0xC0,
//    //char m_AccountName[32]; //0x264,
//    //DWORD m_dwID; //0x20400,
//    //int m_nIndex; //0x20404,
//    ////KItemList m_ItemList; //0x2040C,
//    ////BuySellInfo m_BuyInfo; //0x2522C,
//    ////KPlayer::KSaveAction m_SaveAction; //0x25A08,
//    //char m_PlayerName[32]; //0x25C88,
//    ////KPlayerFaction m_cFaction; //0x25CE0,
//    //int m_nWorldStat; //0x28004,
//    //int m_nSectStat; //0x28008,
//    //DWORD m_dwEnergy; //0x2876C,
//    //int m_nCurEngergy; //0x25C68
//    //BuySellInfo m_BuyInfo;
//    char tmp[0x134];
//};
//
//typedef int (*lua_CFunction)(lua_State* L);
//
////#pragma pack (push, 1)
//struct TLua_Funcs {
//    const char* name;
//    lua_CFunction func;
//};
////#pragma (pop)
//
//struct PARAMS
//{
//    const char* a1;
//    int a2;
//};
//
//#define d_KLuaScript_Create		((struct KLuaScript*(__cdecl*)(struct lua_State * m_lua, struct KLuaScriptSet * b_ScriptSet)) 0x10013850)
//#define d_KLuaScriptSet_CreateScript		((struct KLuaScript *(__thiscall*)(KLuaScriptSet * This)) 0x0063A8A0)
//#define d_KLuaScriptSet_GetScript		((struct KLuaScript *(__thiscall*)(KLuaScriptSet * This, const char *)) 0x10010F30)
//#define d_KLuaScript_RegisterFunctions		((int(__thiscall*)(KLuaScript * This, struct TLua_Funcs *, int)) 0x10011210)//0x10011210 0x63A8A4
////#define d_RegisterFunctions		((int(__thiscall*)(KLuaScriptSet *This, struct TLua_Funcs*, int)) 0x0063A578)
//#define d_RegisterFunctions		((int(__thiscall*)(struct TLua_Funcs*, int)) 0x1EA3AA0)//0x0063A578
//#define u_GetScript		((struct KLuaScript*(__cdecl*)(unsigned int)) 0x5456F0)//0x0063A578 0x5456F0
//#define c_GetScript		((struct KLuaScript*(__cdecl*)(const char *)) 0x545710)//0x0063A578 
//#define b_GetTmpScript		((class KLuaScript *(__thiscall*)(void *)) 0x10013C10)//0x0063A578 
//#define b_KLuaCall		((KLuaCall *(__thiscall*)(KLuaCall *, struct KLuaScript *, int)) 0x1000BB30)//0x0063A578 
//#define d_FileName2Id		((unsigned int(__cdecl*)(char const *)) 0x10025C60)//0x0063A578
//#define d_lua_pushstring		((int(__cdecl*)(_DWORD, void*))0x100018B0)//0x1003F180 0x63A1DC
////#define d_PushString		((_DWORD(__thiscall*)(KLuaScriptSet *This, struct TLua_Funcs*, int)) 0x1000BBD0)
//#define d_SetGlobalInt		((void(__thiscall*)(KLuaScriptSet* , char const *, int)) 0x10013B10) //
////#define structKLuaScriptSet		*(KLuaScriptSet*)0x10055388 //0x1004005C
//#define structKLuaCall		*(KLuaCall*)0x1000BB60//0x10055388
//#define structKLuaScriptSet		0x6E2170 //0x10055388
//#define classKLuaScript		*(KLuaScript*)0x10042FB8
//#define structKLuaScript		*(KLuaScript*)0x68CFD8//0x68CFD8 0x100400D0
//#define structLua_Funcs		*(TLua_Funcs*)0x6849C0//0x68CFD8 0x100400D0
//#define d_RegisterFunctionKLuaScript		((int(__thiscall*)(KLuaScript *, char const *,void *)) 0x100111d0)
//
////extern "C" __declspec(dllimport) int lua_pushstring(_DWORD);
//void cPrintLoading(const char* const Format, ...);
//
//static int LuaGetMac(int);
//
//typedef int (*lua_pushcclosure)(int, int);
//typedef int (*lua_setglobal)(int*, void* Src);
//typedef int (*lua_pushstring)(int, char const*);
//typedef int (*lua_pushnumber)(int, double);
//typedef int (*lua_tostring)(int, int);
//typedef int (*lua_gettop)(_DWORD*);
////typedef int (*lua_call)(int*, int, int);
//typedef int (*lua_baselibopen)(int);
//typedef int (*lua_dblibopen)(int*);
//typedef int (*lua_iolibopen)(int*);
//typedef int (*lua_mathlibopen)(int*);
//typedef int (*lua_strlibopen)(int*);
////HMODULE hModuleLuaLibDll;
////lua_pushstring pushstring;
////lua_gettop gettop;
////lua_pushnumber pushnumber;
////lua_pushcclosure pushcclosure;
////lua_setglobal setglobal;
//////lua_call luacall;
////lua_baselibopen baselibopen;
////lua_dblibopen dblibopen;
////lua_iolibopen iolibopen;
////lua_mathlibopen mathlibopen;
////lua_strlibopen strlibopen;
//
//class KLuaScript
//{
//public:
//    //KLuaScript();
//    //virtual ~KLuaScript();
//    //KLuaScript(int StackSize);
//
//    //int		Activate() { Execute(); return 1; };
//    //virtual BOOL Init();
//    //virtual void Exit();
//    //virtual BOOL Load(char* FileName);
//    //virtual	BOOL Compile(char* FileName);
//    //virtual	BOOL Execute();
//    //virtual	BOOL CallFunction(LPSTR cFuncName, int nResults, LPSTR cFormat, ...);
//    //virtual BOOL RegisterFunction(LPSTR FuncName, void* Func);
//    //virtual void SetScriptName(LPSTR scpname) { strcpy(m_szScriptName, scpname); };
//
//    //int	 GetUserTag() { return m_UserTag; };
//    //BOOL RegisterFunctions(TLua_Funcs Funcs[], int n = 0);
//    //BOOL LoadBuffer(PBYTE pBuffer, DWORD dwLen);
//    //void SafeCallBegin(int* pIndex);//记录当前堆栈栈顶位置，并保存到pIndex指向的数据中
//    //void SafeCallEnd(int nIndex);//恢复至调用之前栈顶位置。
//    //BOOL GetValuesFromStack(char* cFormat, ...);
//
//    //BOOL Stop();
//    //BOOL Resume();
//    //DWORD CreateTable();
//    //DWORD ModifyTable(LPSTR szTableName);
//    //void SetGlobalName(LPSTR szName);
//    ////inline void SetTableMember(int nIndex, int Id, const char* szString) { Lua_SetTable_StringFromId(m_LuaState, nIndex, Id, szString); };
//    ////inline void SetTableMember(int nIndex, int Id, double nNumber) { Lua_SetTable_DoubleFromId(m_LuaState, nIndex, Id, nNumber); };
//    ////inline void SetTableMember(int nIndex, int Id, int nNumber) { Lua_SetTable_IntFromId(m_LuaState, nIndex, Id, nNumber); };
//    ////inline void SetTableMember(int nIndex, const char* szMemberName, Lua_CFunction CFun) { Lua_SetTable_CFunFromName(m_LuaState, nIndex, szMemberName, CFun); };
//    ////inline void SetTableMember(int nIndex, int nId, Lua_CFunction CFun) { Lua_SetTable_CFunFromId(m_LuaState, nIndex, nId, CFun); };
//    ////inline void SetTableMember(int nIndex, const char* szMemberName, char* szString) { Lua_SetTable_StringFromName(m_LuaState, nIndex, szMemberName, szString); };
//    ////inline void SetTableMember(int nIndex, const char* szMemberName, int Number) { Lua_SetTable_IntFromName(m_LuaState, nIndex, szMemberName, Number); };
//    ////inline void SetTableMember(int nIndex, const char* szMemberName, double Number) { Lua_SetTable_DoubleFromName(m_LuaState, nIndex, szMemberName, Number); };
//    //void ScriptError(int Error);
//    //void ScriptError(int, int);
//    //BOOL ExecuteCode();
//    
//    struct KLuaScript* __thiscall GetTmpScript(KLuaScriptSet* This)
//    {
//        return (struct KLuaScript*)*((_DWORD*)This + 73);
//    };
//    int __cdecl lua_usepack(int a1, int a2, _DWORD* a3)
//    {
//        if (a2 < 0 || a2 >= *(_DWORD*)(a1 + 112))
//            return -1;
//        if (a3)
//            *a3 = *(_DWORD*)(a1 + 116);
//        *(_DWORD*)(a1 + 116) = a2;
//        return 0;
//    };
//    int __cdecl cUsepack(_DWORD a1, _DWORD pack, _DWORD* oldpack)
//    {
//        int result; // eax
//
//        if (pack < 0 || pack >= *(_DWORD*)(a1 + 112))
//            return -1;
//        if (oldpack)
//            *oldpack = *(_DWORD*)(a1 + 116);
//        result = 0;
//        *(_DWORD*)(a1 + 116) = pack;
//        return 0;
//    };
//    //extern "C" ENGINE_API
//
//    //extern "C" ENGINE_API
//    static BOOL LuaGetLanIP(lua_State* L)
//    {
//        //char szDesMsg[200];
//        //szDesMsg[0] = 0;
//        //strcpy(szDesMsg, "0.0.0.0");
//
//        //int PlayerIndex = GetPlayerIndex(L);
//        //unsigned int Player = 165956 * PlayerIndex;
//        //KPlayer* Player = (KPlayer*)Player_Struct;
//        //bool sendok = SendData((KPlayer*)&Player[PlayerIndex], &gethwid, sizeof(G2C_PLAYER_GETHWID));
//        //if (sendok)
//        //    _custom_lua_pushstring(L, MacAddress);
//        //_custom_lua_pushstring(L, IpAddress);
//        return true;
//    };
//
//    //extern "C" ENGINE_API
//    static BOOL LuaGetHardwareId(lua_State* L)
//    {
//        //char szDesMsg[200];
//        //szDesMsg[0] = 0;
//        //strcpy(szDesMsg, "0.0.0.0");
//
//        //int PlayerIndex = GetPlayerIndex(L);
//        //unsigned int Player = 165956 * PlayerIndex;
//        //KPlayer* Player = (KPlayer*)Player_Struct;
//        //bool sendok = SendData((KPlayer*)&Player[PlayerIndex], &gethwid, sizeof(G2C_PLAYER_GETHWID));
//        //if (sendok)
//        //    _custom_lua_pushstring(L, MacAddress);
//        //_custom_lua_pushstring(L, HardwareId);
//        return true;
//    };
//
//    //typedef int (*_ConnectToGameSvr)(int, const unsigned char*, unsigned short, GUID*);
//    //typedef int (*_SendPackToServer)(int, const void* const, const size_t&);
//    //typedef class KLuaScript* (*_GetScript)(KLuaScriptSet*, char const*);
//    //typedef struct KLuaScript* (*_CreateScript)(KLuaScriptSet*);
//    //typedef KLuaScriptSet* (*_KLuaScriptSet)(KLuaScriptSet*, const struct KLuaScriptSet*);
//    //typedef struct KLuaScript* (*_GetTmpScript)(LPVOID);
//    //typedef void (*_Script_PushNumber)(KLuaCall*, double); // 0x1003FF54
//    //typedef char* (*_lua_tostring)(int);
//    //typedef int (*_FormatCallNotReturn)(int, const char*, const char*, const char*, ...);
//    ////typedef void (*_SetGlobalInt)(KLuaScriptSet* , const char* , int );
//    //typedef void (*_SetGlobalInt)(KLuaScriptSet*, const char*, int);
//    //typedef int (*_RegisterFunction)(KLuaScript*, const char*, void*);
//    //typedef int (*_RegisterFunctions)(TLua_Funcs*, int);
//    //typedef int (*_GetPlayerIndex)(lua_State*);
//    //typedef int (*_KLuaCallGetTopIndex)(LPVOID);
//    //typedef void (*_SendSystemInfo)(int, int, const char*, const char*, int);
//    
//    //typedef int (*m_RegisterFunctions)(KLuaScriptSet* This, struct TLua_Funcs*, int);
//
//    //struct KLuaScript* b_TmpScript;
//    //lua_State* b_L;
//
//
//    //_ConnectToGameSvr _Hook_ConnectToGameSvr = (_ConnectToGameSvr)0x528CF0; // lua_tostring	0822D4A0
//    //_SendPackToServer _Hook_SendPackToServer = (_SendPackToServer)0x615DC3; // lua_tostring	0822D4A0
//    //_CreateScript _Hook_CreateScript = (_CreateScript)0x10013BF0; // lua_tostring	0822D4A0
//    //_GetScript _Hook_GetScript = (_GetScript)0x10010F30; // lua_tostring	0822D4A0
//    //_GetTmpScript _Hook_GetTmpScript = (_GetTmpScript)0x63A8D0; // 0x63A8D0 0x10013C10
//    //_Script_PushNumber _Hook_Script_PushNumber = (_Script_PushNumber)0x63A6E0; // lua_tostring	0x63A6E0 0x1000BBB0
//    //_lua_tostring _lualibdll_lua_tostring = (_lua_tostring)0x10001710; // lua_tostring	0822D4A0
//    //_FormatCallNotReturn oFormatCallNotReturn = (_FormatCallNotReturn)0x566F50; // KLuaScriptSet::RegisterFunctions(TLua_Funcs *,int)	0x63A578 0x63A8A4 0x10013AA0 0x63A578 0x63A578  0x1C51210 0x1C53AA0 0x1C33AA0
//    //_SetGlobalInt oSetGlobalInt = (_SetGlobalInt)0x10013B10; // 0x0063A57C 0x10013B10
//    //_KLuaScriptSet oKLuaScriptSet = (_KLuaScriptSet)0x100138D0; // 0x0063A57C 0x10013B10
//    //_RegisterFunction oRegisterFunction = (_RegisterFunction)0x100111D0; // KLuaScriptSet::RegisterFunctions(TLua_Funcs *,int)	0x63A578 0x63A8A4 0x10013AA0 0x63A578 0x63A578  0x1C51210 0x1C53AA0 0x1C33AA0
//    //_RegisterFunctions oRegisterFunctions = (_RegisterFunctions)0x63A578; // KLuaScriptSet::RegisterFunctions(TLua_Funcs *,int)	0x63A578 0x63A8A4 0x10013AA0 0x63A578 0x63A578  0x1C51210 0x1C53AA0 0x1C33AA0
//    //_GetPlayerIndex GetPlayerIndex = (_GetPlayerIndex)0x811DB20; // GetPlayerIndex(lua_State *)	0811DB20	
//    //_SendSystemInfo SendSystemInfo = (_SendSystemInfo)0x81DF070; // KPlayerChat::SendSystemInfo(int,int,char const*,char const*,int)	081DF070	
//    //_lua_pushcclosure _custom_lua_pushcclosure = (_lua_pushcclosure)0x100018F0; // 0x100018F0 0x63A1F4
//    //_lua_setglobal _custom_lua_setglobal = (_lua_setglobal)0x1003F1B0; // 0x10001BC0
//    //_lua_pushstring _custom_lua_pushstring = (_lua_pushstring)0x63A1DC; // lua_pushstring	0822D400	
//    //_lua_pushnumber _custom_lua_pushnumber = (_lua_pushnumber)0x63A1E0; // 0x63A1E0 0x1003F17C
//    //_lua_tostring _custom_lua_tostring_new = (_lua_tostring)0x1671710; // lua_pushstring	0822D400	
//    //_lua_gettop _custom_lua_gettop = (_lua_gettop)0x0063A1EC; // lua_gettop	0822C420	
//    //_KLuaCallGetTopIndex oKLuaCall_GetTopIndex = (_KLuaCallGetTopIndex)0x63A78C; // lua_gettop	0822C420	
//    //_lua_tostring _custom_lua_tostring = (_lua_tostring)0x822D4A0; // lua_tostring	0822D4A0	
//
//    //private:
//    
//    char m_KLuaCall[12];
//    void RegisterStandardFunctions();
//    BOOL m_IsRuning;//是否该脚本有效
//    char m_szScriptName[100];
//    int	 m_UserTag;
//    lua_State* m_LuaState;
//private:
//    BOOL CallFunction(LPSTR cFuncName, int nResults, LPSTR cFormat, va_list vlist);
//};
//
//extern KLuaScript gKLuaScript;
//extern KLuaScriptSet* b_ScriptSet;
////extern KLuaScript* b_Script;
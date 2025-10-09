//#include "stdafx.h"
//#include "KLuaScript.h"
//#include <fstream>
//#include <string>
//#include "Protect.h"
////#include "KUiMissionTime.h"
////#include "luawin/LuaLib.h"
////#include <fstream>
////#include <iosfwd>
//
//using namespace std;
//
//KLuaScript gKLuaScript;
//KLuaScriptSet* b_ScriptSet;
//HMODULE hModuleLuaLibDll = LoadLibrary(TEXT("lualibdll.dll"));
//lua_pushstring pushstring = (lua_pushstring)GetProcAddress(hModuleLuaLibDll, "lua_pushstring");
//lua_gettop gettop = (lua_gettop)GetProcAddress(hModuleLuaLibDll, "lua_gettop");
//lua_pushnumber pushnumber = (lua_pushnumber)GetProcAddress(hModuleLuaLibDll, "lua_pushnumber");
//lua_pushcclosure pushcclosure = (lua_pushcclosure)GetProcAddress(hModuleLuaLibDll, "lua_pushcclosure");
//lua_setglobal setglobal = (lua_setglobal)GetProcAddress(hModuleLuaLibDll, "lua_setglobal");
////lua_call luacall = (lua_call)GetProcAddress(hModuleLuaLibDll, "lua_call");
//lua_baselibopen baselibopen = (lua_baselibopen)GetProcAddress(hModuleLuaLibDll, "lua_baselibopen");
//lua_dblibopen dblibopen = (lua_dblibopen)GetProcAddress(hModuleLuaLibDll, "lua_dblibopen");
//lua_iolibopen iolibopen = (lua_iolibopen)GetProcAddress(hModuleLuaLibDll, "lua_iolibopen");
//lua_mathlibopen mathlibopen = (lua_mathlibopen)GetProcAddress(hModuleLuaLibDll, "lua_mathlibopen");
//lua_strlibopen strlibopen = (lua_strlibopen)GetProcAddress(hModuleLuaLibDll, "lua_strlibopen");
//
////KLuaScript* b_Script;
////TLua_Funcs CustomScriptFuns[] =
////{
////    {"GetMac", LuaGetMac},
////    {"GetLanIP", LuaGetLanIP},
////    {"GetHardwareId", LuaGetHardwareId},
////};
//
////TLua_Funcs CustomScriptFuns[] =
////{
////    {"GetMac", LuaGetMac}
////    //{"GetLanIP", LuaGetLanIP},
////    //{"GetHardwareId", LuaGetHardwareId},
////};
//
////int g_GetCustomScriptFunNum()
////{
////    return sizeof(CustomScriptFuns) / sizeof(TLua_Funcs);
////}
//
//void SendPackToServer(int This, const void* const pData, const size_t& datalength) {
//    //_Hook_SendPackToServer(This, pData, datalength); // 0x19FD50
//}
////extern "C" _declspec(dllexport) void HookPushNumber(KLuaCall * This, double a2) {
//void __stdcall HookPushNumber(KLuaCall* This, double a2) {
//    //lua_pushnumber(Script->m_LuaState, (double)this->m_nPlayerIndex);
//    //lua_pushnumber(*(_DWORD*)(*(_DWORD*)This + 0x10C), LODWORD(a2), HIDWORD(a2));
//    //int TopIndex = 0;
//    ////lua_State* L;
//    //if (!b_ScriptSet)
//    //    b_ScriptSet = (KLuaScriptSet*)0x6E2170;
//
//    //if (!b_TmpScript)
//    //    b_TmpScript = (struct KLuaScript*)0x100400D0;
//
//    //if (!b_L)
//    //    b_L = (lua_State*)0x10013750;
//
//    //TopIndex = lua_gettop(b_L);
//    ////TmpScript = _Hook_GetTmpScript(&b_ScriptSet);
//    ////TmpScript = (struct KLuaScript*)*((DWORD*)b_ScriptSet + 73);
//    ////TopIndex = _custom_lua_gettop(*((DWORD*)TmpScript + 72));
//
//    ////oKLuaCall_GetTopIndex(&TmpScript);
//    //ofstream myfile;
//
//    //myfile.open("E:\\example.txt");
//    //myfile << b_ScriptSet << "\n";
//    //myfile << b_TmpScript << "\n";
//    //myfile << &b_TmpScript + 67 << "\n";
//    //myfile << a2 << "\n";
//
//    //myfile << TopIndex << "\n";
//
//    //myfile.close();
//    //oRegisterFunction((KLuaScript*)&b_TmpScript, "GetMac", LuaGetMac);
//    //lua_pushnumber((lua_State*)&b_TmpScript + 67, a2);
//    //_Hook_Script_PushNumber(This, a2);
//}
////
////void FormatCallNotReturn(int This, const char* pszFile, const char* pszFunction, const char* pszFmt, int nProtolId, int nHandle) {
////    int nTopIndex = 0;
////    int nValueNum = 0;
////    double* pNumber = NULL;
////    const char** pString;
////    int* pInt = NULL;
////    int i = 0;
////    //va_list vlist;
////    //nValueNum = strlen(pszFmt);
////    //va_start(vlist, pszFmt);
////    //ofstream myfile;
////
////    //myfile.open("E:\\example.txt");
////    //myfile << This << "\n";
////    //myfile << pszFmt << "\n";
////    //myfile << nProtolId << "\n";
////    //myfile << nHandle << "\n";
////    //while (pszFmt[i] != '\0')
////    //{
////    //    switch (pszFmt[i])
////    //    {
////    //    case 'n'://返回值为数值形,Number,此时Lua只传递double形的值
////    //    {
////    //        pNumber = va_arg(vlist, double*);
////
////    //        if (pNumber == NULL)
////    //            return;
////
////    //        myfile << pNumber << " n \n";
////    //    }
////    //    break;
////    //    case 'd':
////    //    {
////    //        pInt = va_arg(vlist, int*);
////    //        if (pInt == NULL)
////    //            return;
////    //        myfile << pInt << " d \n";
////
////    //    }
////    //    break;
////    //    case 's'://字符串形
////    //    {
////    //        pString = va_arg(vlist, const char**);
////
////    //        if (pString == NULL)
////    //            return;
////
////    //        myfile << pString << " s \n";
////    //    }
////    //    break;
////
////    //    }
////    //    
////
////    //    i++;
////    //}
////    //
////    //myfile.close();
////    //oFormatCallNotReturn(This, "\\script\\protocol.lua", "ScriptProtocol:ProtocolProcess", "dd", nProtolId, nHandle); // 0x19FD50
////    //oFormatCallNotReturn(This, "\\script\\protocol.lua", "ScriptProtocol:ProtocolProcess", "dd"); // 0x19FD50
////    //va_end(vlist);
////    //return;
////}
//////extern "C" _declspec(dllexport) int cRegisterFunctions(KLuaScript* const g_ScriptSet, TLua_Funcs * pFunctionList, int nCount)
////void __stdcall cSetGlobalInt(const char* a2, int a3, KLuaScriptSet* This) {
////    DWORD OldProtect;
////    DWORD OldProtect1;
////    DWORD OldProtect2;
////    DWORD OldProtect3;
////    DWORD OldProtect4;
////    DWORD OldProtect5;
////
////    lua_State* m_pLua;
////    int TopIndex;
////
////    //ofstream myfile;
////    //myfile.open("E:\\example.txt");
////    //myfile << This << "\n";
////    //myfile << a2 << "\n";
////    //myfile << a3 << "\n";
////    //myfile.close();
////    VirtualProtect((LPVOID)&structKLuaScriptSet, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////
////    if (!b_ScriptSet)
////        b_ScriptSet = (KLuaScriptSet*)&structKLuaScriptSet;
////    VirtualProtect((LPVOID)&b_ScriptSet, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    m_pLua = (lua_State*)((_DWORD*)&b_ScriptSet + 72);
////    TopIndex = lua_gettop((lua_State*)((_DWORD*)&b_ScriptSet + 72));
////    //myfile.open("E:\\example.txt");
////    //myfile << b_ScriptSet << "\n";
////    //myfile << m_pLua << "\n";
////    //myfile << TopIndex << "\n";
////    //myfile.close();
////    VirtualProtect((LPVOID)&gKLuaScript, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    VirtualProtect((LPVOID)&structKLuaScript, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    VirtualProtect((LPVOID)0x63A57C, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    VirtualProtect((LPVOID)0x1003D22E, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    VirtualProtect((LPVOID)0x100111d0, 2000, PAGE_EXECUTE_READWRITE, &OldProtect1);
////    VirtualProtect((LPVOID)0x10042FB8, 2000, PAGE_EXECUTE_READWRITE, &OldProtect2);
////    //VirtualProtect((LPVOID)&oSetGlobalInt, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    VirtualProtect((LPVOID)0x100111d6, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    VirtualProtect((LPVOID)0x1000c244, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////
////    lua_State* v4; // eax
////    const char* v5; // ebx
////    int v6; // [esp-10h] [ebp-24h]
////
////    v4 = (lua_State*)*((_DWORD*)b_ScriptSet + 72);
////    if (v4)
////    {
////        v5 = a2;
////        if (a2)
////        {
////            VirtualProtect((LPVOID)v4, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////            lua_pushnumber(v4, (double)a3);
////            v6 = *((_DWORD*)b_ScriptSet + 72);
////            a2 = 0;
////            gKLuaScript.lua_usepack(v6, 0, (_DWORD*)&a2);
////            lua_setglobal((lua_State*)((_DWORD*)b_ScriptSet + 72), v5);
////            gKLuaScript.lua_usepack(*((_DWORD*)b_ScriptSet + 72), (int)&a2, 0);
////        }
////    }
////
////    if (!gKLuaScript.m_LuaState)
////        *(KLuaScript*)&gKLuaScript = *(KLuaScript*)&structKLuaScript;
////    VirtualProtect((LPVOID)&gKLuaScript, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    m_pLua = (lua_State*)((_DWORD*)&gKLuaScript + 67);
////    //VirtualProtect((LPVOID)m_pLua, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    //TopIndex = lua_gettop((lua_State*)*((_DWORD*)b_Script + 67));
////    TopIndex = lua_gettop((lua_State*)((KLuaScript*)&gKLuaScript + 67));
////    //myfile.open("E:\\example.txt");
////    //myfile << b_Script << "\n";
////    //myfile << m_pLua << "\n";
////    //myfile << TopIndex << "\n";
////    //myfile.close();
////
////    //oKLuaScriptSet
////    //oSetGlobalInt(a2, a3);
////
////    //KLuaScriptSet* m_ScriptSet = oKLuaScriptSet((KLuaScriptSet*)(char*)&structKLuaScriptSet);
////    //oSetGlobalInt(b_ScriptSet, a2, a3);
////    lua_State* result; // eax
////
////    result = (lua_State*)((_DWORD*)&gKLuaScript + 67);
////    if (result)
////    {
////        VirtualProtect((LPVOID)result, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////        //lua_pushcclosure(result, (*(_DWORD*)&LuaGetMac), 0);
////        lua_setglobal((lua_State*)((KLuaScript*)&classKLuaScript + 67), "GetMac");
////        //return 1;
////    }
////    //d_RegisterFunctionKLuaScript((KLuaScript*)&classKLuaScript, "GetMac", LuaGetMac);
////    //d_RegisterFunctionKLuaScript((KLuaScript*)&classKLuaScript, "GetMac", LuaGetMac);
////    //oRegisterFunction((KLuaScript*)&classKLuaScript, "GetMac", LuaGetMac);
////    //d_SetGlobalInt(a2, a3);
////    //d_SetGlobalInt(b_ScriptSet, a2, a3);
////    //typedef void* (__stdcall* KLuaScriptSet)();
////    //typedef void (WINAPI* PKLuaScriptSet) (void);
////    //typedef void (KLuaScriptSet::* SetGlobalInt)(const char*, int);
////    //typedef HRESULT(__stdcall* pfnKLuaScriptSet)(
////    //    KLuaScriptSet This
////    //    );
////    //KLuaScriptSet* b_ScriptSet;
////    //HMODULE hModuleLuaLibDll = LoadLibrary(TEXT("lualibdll.dll"));
////    //pushnumber b_lua_pushnumber = (pushnumber)GetProcAddress(hModuleLuaLibDll, "lua_pushnumber");
////    //setglobal lua_setglobal = (setglobal)GetProcAddress(hModuleLuaLibDll, "lua_setglobal");
////    //SetGlobalInt b_SetGlobalInt = NULL;
////
////    //HMODULE hModuleEngine = LoadLibraryExA(TEXT("engine.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE);
////    //HMODULE hModuleEngine = GetModuleHandle("engine.dll");
////    //VirtualProtect(hModuleEngine, sizeof(hModuleEngine), PAGE_EXECUTE_READWRITE, &OldProtect);
////    //if (hModuleEngine != NULL)
////    //{
////    //    //m_DllEntryPoint DllEntryPoint = (m_DllEntryPoint)GetProcAddress((HMODULE)hModuleEngine, "DllEntryPoint");
////    //    //m_SetGlobalInt b_SetGlobalInt = (m_SetGlobalInt)GetProcAddress((HMODULE)hModuleEngine, "?SetGlobalInt@KLuaScriptSet@@QAEXPBDH@Z");
////    //    //KLuaScriptSet* b_ScriptSet = (KLuaScriptSet*)GetProcAddress((HMODULE)hModuleEngine, "??_7KScriptSet@@6B@");
////    //    //b_SetGlobalInt = (SetGlobalInt)GetProcAddress((HMODULE)hModuleEngine, "?SetGlobalInt@KLuaScriptSet@@QAEXPBDH@Z");
////    //    //DllEntryPoint();
////    //    //void* objptr = pfnCreateMyClass();
////
////    //    //int res = 0;
////
////    //    if (b_ScriptSet == NULL)
////    //        MessageBox(0, "Cannot load b_ScriptSet", "Error", MB_OK | MB_ICONINFORMATION);
////    //    //VirtualProtect((LPVOID)0x6E2170, 2000, PAGE_EXECUTE_READWRITE, &OldProtect1);
////    //    //VirtualProtect((LPVOID)&b_ScriptSet, 2000, PAGE_EXECUTE_READWRITE, &OldProtect2);
////    //    //VirtualProtect((LPVOID)&b_SetGlobalInt, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
////    //    VirtualProtect((LPVOID)0x63A57C, 2000, PAGE_EXECUTE_READWRITE, &OldProtect4);
////    //    if (b_SetGlobalInt != NULL)
////    //    {
////    //        try
////    //        {
////    //            //MessageBox(0, "OK", "Error", MB_OK | MB_ICONINFORMATION);
////    //            //h_KLuaScriptSet b_SetGlobalInt = (h_KLuaScriptSet)GetProcAddress(hModuleEngine, "?SetGlobalInt@KLuaScriptSet@@QAEXPBDH@Z"); // aSetglobalintKl
////
////    //            //VirtualProtect((void*)This, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////
////    //            //VirtualProtect((void*)a3, 2000, PAGE_EXECUTE_READWRITE, &OldProtect2);
////    //            //VirtualProtect((void*)0x63A57C, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
////    //            //VirtualProtect((void*)0x6E2170, 0x13C, PAGE_EXECUTE_READWRITE, &OldProtect4);
////    //            //VirtualProtect((void*)b_ScriptSet, 2000, PAGE_EXECUTE_READWRITE, &OldProtect5);
////    //            //VirtualizeOffset(0x6E2170, 0x13C);
////    //            //if (!b_ScriptSet)
////    //                //b_ScriptSet = (KLuaScriptSet*)(char*)&structKLuaScriptSet;
////
////    //            //m_pLua = (lua_State*)&b_ScriptSet->tmp[72];
////
////    //            //myfile << This->tmp[0x0] << "\n";
////    //            //myfile << m_pLua << "\n";
////    //         /*   PARAMS params;
////    //            params.a1 = a2;
////    //            params.a2 = a3;
////    //            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)d_SetGlobalInt, &params, 0, 0);*/
////    //            b_SetGlobalInt(a2, a3);
////    //            //d_SetGlobalInt(a2, a3);
////    //        }
////    //        catch (std::exception& e)
////    //        {
////    //            //std::cout << e.what();
////    //            myfile << e.what() << "\n";
////    //        }
////    //    }
////    //    else
////    //        MessageBox(0, "Cannot load b_SetGlobalInt", "Error", MB_OK | MB_ICONINFORMATION);
////    //    //VirtualProtect((void*)0x6E2170, 2000, OldProtect1, &OldProtect1);
////    //    //VirtualProtect((void*)&b_ScriptSet, 2000, OldProtect2, &OldProtect2);
////    //    //VirtualProtect((void*)&b_SetGlobalInt, 2000, OldProtect3, &OldProtect3);
////    //}
////    //else
////    //    MessageBox(0, "Cannot load dll", "Error", MB_OK | MB_ICONINFORMATION);
////
////
////    //if (v4)
////    //{
////    //    v5 = a2;
////    //    if (a2)
////    //    {
////    //        //VirtualProtect((void*)v4, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
////    //        //lua_pushnumber(m_pLua, (double)a3);
////
////    //        //_custom_lua_pushnumber(*((_DWORD*)b_ScriptSet + 72), (double)a3, (double)a3);
////    //        b_lua_pushnumber(*((_DWORD*)b_ScriptSet + 72), a3);
////    //        v6 = *((_DWORD*)b_ScriptSet + 72);
////    //        a2 = 0;
////    //        cUsepack(v6, 0, (_DWORD*)&a2);
////    //        lua_setglobal(*((_DWORD*)b_ScriptSet + 72), *((_DWORD*)v5));
////    //        //_custom_lua_setglobal(*((_DWORD*)b_ScriptSet + 72), *((_DWORD*)v5));
////    //        cUsepack(*((_DWORD*)b_ScriptSet + 72), *(_DWORD*)&a2, 0);
////    //    }
////    //}
////    //FreeLibrary(hModuleEngine);
////    //FreeLibrary(hModuleLuaLibDll);
////    //MemoryCpy((DWORD)b_ScriptSet, (void*)0x6E2170, sizeof(b_ScriptSet));
////    //MessageBox(0, "Start", "Error", MB_OK | MB_ICONINFORMATION);
////
////    //oSetGlobalInt(a2, a3);
////    //oSetGlobalInt((KLuaScriptSet*)&b_ScriptSet, a2, a3);
////    //VirtualProtect((void*)This, 2000, OldProtect, &OldProtect);
////
////    //VirtualProtect((void*)a3, 2000, OldProtect2, &OldProtect2);
////    //VirtualProtect((void*)0x63A57C, 2000, OldProtect3, &OldProtect3);
////    //VirtualProtect((void*)0x6E2170, 0x13C, OldProtect4, &OldProtect4);
////    //VirtualProtect((void*)b_ScriptSet, 2000, OldProtect5, &OldProtect5);
////    //oFormatCallNotReturn(This, "\\script\\protocol.lua", "ScriptProtocol:ProtocolProcess", "dd", nProtolId, nHandle); // 0x19FD50
////    //oFormatCallNotReturn(This, "\\script\\protocol.lua", "ScriptProtocol:ProtocolProcess", "dd"); // 0x19FD50
////    //va_end(vlist);
////    //return;
////}
////
//
//void __stdcall cPrintLoading(const char* const Format, ...) {
//    DWORD OldProtect;
//    DWORD OldProtect1;
//    DWORD OldProtect2;
//    DWORD OldProtect3;
//    DWORD OldProtect4;
//    DWORD OldProtect5;
//    lua_State* m_pLua;
//    //int v4;
//    const char* v5;
//    int v6;
//    int nPack;
//    int nOldPack;
//    //ofstream myfile;
//    //myfile.open("E:\\example.txt");
//    //typedef bool (*pushnumber)(int a1, double a2);
//    //typedef bool (*setglobal)(_DWORD a1, _DWORD Src);
//    //myfile << Format << "\n";
//    //myfile.close();
//    //HMODULE hModuleEngine = GetModuleHandle("engine.dll");
//    //VirtualProtect(&hModuleEngine, sizeof(hModuleEngine), PAGE_EXECUTE_READWRITE, &OldProtect);
//    //if (hModuleEngine != NULL)
//    //{
//    //    MessageBox(0, "Cannot load engine.dll", "Error", MB_OK | MB_ICONINFORMATION);
//    //}
//    //if (!b_ScriptSet)
//    //    b_ScriptSet = (KLuaScriptSet*)&structKLuaScriptSet;
//    //if (b_ScriptSet == NULL)
//    //    MessageBox(0, "Cannot load b_ScriptSet", "Error", MB_OK | MB_ICONINFORMATION);
//    //m_pLua = (lua_State*)&b_ScriptSet->tmp[72];
//    //KLuaScriptSet* b_ScriptSet = structKLuaScriptSet();
//    //d_RegisterFunctions((KLuaScriptSet*)&structKLuaScriptSet,CustomScriptFuns, g_GetCustomScriptFunNum());
//    //Sleep(5000);
//    //unsigned int v4;
//    //ofstream myfile;
//    //myfile.open("E:\\example.txt");
//    //VirtualProtect(d_FileName2Id, 2000, PAGE_EXECUTE_READWRITE, &OldProtect1);
//    //VirtualProtect(u_GetScript, 2000, PAGE_EXECUTE_READWRITE, &OldProtect2);
//    //VirtualProtect(d_KLuaScript_RegisterFunctions, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
//    //VirtualProtect(&v4, 2000, PAGE_EXECUTE_READWRITE, &OldProtect4);
//    //VirtualProtect(&gKLuaScript, 2000, PAGE_EXECUTE_READWRITE, &OldProtect5);
//
//    //v4 = d_FileName2Id("\script\protocol.lua");
//    //gKLuaScript = c_GetScript("\script\fwcloud.lua");
//    //VirtualProtect(&gKLuaScript, 2000, PAGE_EXECUTE_READWRITE, &OldProtect5);
//    //myfile << v4 << "\n";
//    //myfile << b_Script << "\n";
//    //myfile << *((DWORD*)&b_Script + 67) << "\n";
//    //myfile.close();
//    //if (gKLuaScript)
//    //{
//    //    //lua_pushcclosure(*((_DWORD*)this + 67), *((_DWORD*)a2 + 2 * i + 1), 0);
//    //    //lua_setglobal(*((_DWORD*)this + 67), *((_DWORD*)a2 + 2 * i));
//    //    //gKLuaScript._custom_lua_pushcclosure(*((DWORD*)&b_Script + 67), *(_DWORD*)&gKLuaScript.LuaGetMac, 0);
//    //    //gKLuaScript._custom_lua_setglobal(*((DWORD*)&b_Script + 67), *(_DWORD*)"GetMacAddr");
//    //    //d_KLuaScript_RegisterFunctions((KLuaScript*)b_Script, (struct TLua_Funcs*)&CustomScriptFuns, g_GetCustomScriptFunNum());
//    //}
//    //b_Script = c_GetScript("\script\protocol.lua");
//    ////VirtualProtect(b_Script, 2000, PAGE_EXECUTE_READWRITE, &OldProtect5);
//    ////myfile << b_Script << "\n";
//    ////myfile.close();
//    //if (b_Script)
//    //{
//    //    //lua_pushcclosure(*((_DWORD*)this + 67), *((_DWORD*)a2 + 2 * i + 1), 0);
//    //    //lua_setglobal(*((_DWORD*)this + 67), *((_DWORD*)a2 + 2 * i));
//    //    _custom_lua_pushcclosure(*((DWORD*)b_Script + 67), *(_DWORD*)&LuaGetMac, 0);
//    //    _custom_lua_setglobal(*((_DWORD*)b_Script + 67), *(_DWORD*)"GetMacAddr");
//    //    //d_KLuaScript_RegisterFunctions((KLuaScript*)b_Script, (struct TLua_Funcs*)&CustomScriptFuns, g_GetCustomScriptFunNum());
//    //}
//    TLua_Funcs* listFuncs[146];//structLua_Funcs* (TLua_Funcs*)
//
//    VirtualProtect((void*)&listFuncs, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//    VirtualProtect((void*)&structLua_Funcs, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//    memcpy((void*)&listFuncs, (void*)&structLua_Funcs, sizeof(listFuncs));
//    //*listFuncs = (TLua_Funcs*)((void*)&structLua_Funcs);
//    //listFuncs[140]->func = &LuaGetMac;
//    //SetDword(*((DWORD*)&listFuncs + 140 + 1), (DWORD)&LuaGetMac);
//    //memcpy(&listFuncs[141]->func, (void*)&LuaGetMac, sizeof(&LuaGetMac));
//    //MemorySet((DWORD)((DWORD*)&listFuncs + 140 + 1), (DWORD)&LuaGetMac, sizeof(&LuaGetMac));
//    MemorySet(*((DWORD*)&listFuncs + 36 + 1), (DWORD)&LuaGetMac, sizeof(&LuaGetMac));
//    memcpy((void*)*((DWORD*)&listFuncs + 36), (char*)"GetHardware", sizeof("GetHardware"));
//    SetByte(0x5D10F1, 0x92);
//
//    //*((char*)&structLua_Funcs) = *(char*)&listFuncs;
//    //char buff[32];
//    //sprintf(buff, "%p", (DWORD)&LuaGetMac); // 36
//    //MessageBox(0, buff, "Error", MB_OK | MB_ICONINFORMATION);
//
//    //sprintf(buff, "%p", *((DWORD*)&listFuncs + 36 + 1)); // 36
//    //MessageBox(0, buff, "Error", MB_OK | MB_ICONINFORMATION);
//    //MessageBox(0, (char*)*((DWORD*)&listFuncs + 36), "Error", MB_OK | MB_ICONINFORMATION);
//
//    memcpy((void*)&structLua_Funcs, &listFuncs, sizeof(listFuncs));
//    //VirtualProtect((void*)&LuaGetMac, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//     
//    SetCompleteHook(0xE9, 0x5D0CD0, &LuaGetMac); // 0x71E14FB0 0x5456BE
//
//    //SetCompleteHook(0xE8, 0x5D0D0B, &LuaGetMac); // 0x71E14FB0 0x5456BE
//    //MemorySet(0x5D0CDB, 0x90, 1);
//     
//    //SetCompleteHook(0xE8, 0x5D0CD6, &LuaGetMac); // 0x71E14FB0 0x5456BE
//    //MemorySet(0x5D0CDB, 0x90, 1);
//
//    //SetRange((void*)0x5D0CDC, 0x34, 0x90);
//
//    //SetCompleteHook(0xE8, 0x5D0CD6, &LuaGetMac); // 0x71E14FB0 0x5456BE
//    //MemorySet(0x5D0CDB, 0x90, 1);
//    //FreeLibrary(hModuleEngine);
//    //typedef void* (__stdcall* KLuaScriptSet)();
//    //typedef void (WINAPI* PKLuaScriptSet) (void);
//    //typedef void (KLuaScriptSet::* SetGlobalInt)(const char*, int);
//    //typedef HRESULT(__stdcall* pfnKLuaScriptSet)(
//    //    KLuaScriptSet This
//    //    );
//    //KLuaScriptSet* b_ScriptSet;
//    //HMODULE hModuleLuaLibDll = LoadLibrary(TEXT("lualibdll.dll"));
//    //pushnumber b_lua_pushnumber = (pushnumber)GetProcAddress(hModuleLuaLibDll, "lua_pushnumber");
//    //setglobal lua_setglobal = (setglobal)GetProcAddress(hModuleLuaLibDll, "lua_setglobal");
//    //SetGlobalInt b_SetGlobalInt = NULL;
//
//    //HMODULE hModuleEngine = LoadLibraryExA(TEXT("engine.dll"), NULL, LOAD_LIBRARY_AS_DATAFILE_EXCLUSIVE);
//    //HMODULE hModuleEngine = GetModuleHandle("engine.dll");
//    //VirtualProtect(hModuleEngine, sizeof(hModuleEngine), PAGE_EXECUTE_READWRITE, &OldProtect);
//    //if (hModuleEngine != NULL)
//    //{
//    //    //m_DllEntryPoint DllEntryPoint = (m_DllEntryPoint)GetProcAddress((HMODULE)hModuleEngine, "DllEntryPoint");
//    //    m_SetGlobalInt b_SetGlobalInt = (m_SetGlobalInt)GetProcAddress((HMODULE)hModuleEngine, "?SetGlobalInt@KLuaScriptSet@@QAEXPBDH@Z");
//    //    KLuaScriptSet* b_ScriptSet = (KLuaScriptSet*)GetProcAddress((HMODULE)hModuleEngine, "??_7KScriptSet@@6B@");
//    //    //b_SetGlobalInt = (SetGlobalInt)GetProcAddress((HMODULE)hModuleEngine, "?SetGlobalInt@KLuaScriptSet@@QAEXPBDH@Z");
//    //    //DllEntryPoint();
//    //    //void* objptr = pfnCreateMyClass();
//
//    //    //int res = 0;
//
//
//    //    VirtualProtect((LPVOID)0x6E2170, 2000, PAGE_EXECUTE_READWRITE, &OldProtect1);
//    //    VirtualProtect((LPVOID)&b_ScriptSet, 2000, PAGE_EXECUTE_READWRITE, &OldProtect2);
//    //    VirtualProtect((LPVOID)&b_SetGlobalInt, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
//    //    VirtualProtect((LPVOID)0x63A57C, 2000, PAGE_EXECUTE_READWRITE, &OldProtect4);
//    //    if (b_SetGlobalInt != NULL)
//    //    {
//    //        try
//    //        {
//    //            //MessageBox(0, "OK", "Error", MB_OK | MB_ICONINFORMATION);
//    //            //h_KLuaScriptSet b_SetGlobalInt = (h_KLuaScriptSet)GetProcAddress(hModuleEngine, "?SetGlobalInt@KLuaScriptSet@@QAEXPBDH@Z"); // aSetglobalintKl
//
//    //            //VirtualProtect((void*)This, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//
//    //            //VirtualProtect((void*)a3, 2000, PAGE_EXECUTE_READWRITE, &OldProtect2);
//    //            //VirtualProtect((void*)0x63A57C, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
//    //            //VirtualProtect((void*)0x6E2170, 0x13C, PAGE_EXECUTE_READWRITE, &OldProtect4);
//    //            //VirtualProtect((void*)b_ScriptSet, 2000, PAGE_EXECUTE_READWRITE, &OldProtect5);
//    //            //VirtualizeOffset(0x6E2170, 0x13C);
//
//
//    //            //myfile << This->tmp[0x0] << "\n";
//    //            //myfile << m_pLua << "\n";
//    //         /*   PARAMS params;
//    //            params.a1 = a2;
//    //            params.a2 = a3;
//    //            CreateThread(0, 0, (LPTHREAD_START_ROUTINE)d_SetGlobalInt, &params, 0, 0);*/
//    //            b_SetGlobalInt(a2, a3);
//    //            //d_SetGlobalInt(a2, a3);
//    //        }
//    //        catch (std::exception& e)
//    //        {
//    //            //std::cout << e.what();
//    //            myfile << e.what() << "\n";
//    //        }
//    //    }
//    //    else
//    //        MessageBox(0, "Cannot load b_SetGlobalInt", "Error", MB_OK | MB_ICONINFORMATION);
//    //    //VirtualProtect((void*)0x6E2170, 2000, OldProtect1, &OldProtect1);
//    //    //VirtualProtect((void*)&b_ScriptSet, 2000, OldProtect2, &OldProtect2);
//    //    //VirtualProtect((void*)&b_SetGlobalInt, 2000, OldProtect3, &OldProtect3);
//    //}
//    //else
//    //    MessageBox(0, "Cannot load dll", "Error", MB_OK | MB_ICONINFORMATION);
//
//    //myfile.close();
//    ////if (v4)
//    ////{
//    ////    v5 = a2;
//    ////    if (a2)
//    ////    {
//    ////        //VirtualProtect((void*)v4, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//    ////        //lua_pushnumber(m_pLua, (double)a3);
//
//    ////        //_custom_lua_pushnumber(*((_DWORD*)b_ScriptSet + 72), (double)a3, (double)a3);
//    ////        b_lua_pushnumber(*((_DWORD*)b_ScriptSet + 72), a3);
//    ////        v6 = *((_DWORD*)b_ScriptSet + 72);
//    ////        a2 = 0;
//    ////        cUsepack(v6, 0, (_DWORD*)&a2);
//    ////        lua_setglobal(*((_DWORD*)b_ScriptSet + 72), *((_DWORD*)v5));
//    ////        //_custom_lua_setglobal(*((_DWORD*)b_ScriptSet + 72), *((_DWORD*)v5));
//    ////        cUsepack(*((_DWORD*)b_ScriptSet + 72), *(_DWORD*)&a2, 0);
//    ////    }
//    ////}
//    ////FreeLibrary(hModuleEngine);
//    ////FreeLibrary(hModuleLuaLibDll);
//    ////MemoryCpy((DWORD)b_ScriptSet, (void*)0x6E2170, sizeof(b_ScriptSet));
//    ////MessageBox(0, "Start", "Error", MB_OK | MB_ICONINFORMATION);
//    //int nTopIndex = 0;
//
//
//    ////oSetGlobalInt(a2, a3);
//    ////oSetGlobalInt((KLuaScriptSet*)&b_ScriptSet, a2, a3);
//    ////VirtualProtect((void*)This, 2000, OldProtect, &OldProtect);
//
//    ////VirtualProtect((void*)a3, 2000, OldProtect2, &OldProtect2);
//    ////VirtualProtect((void*)0x63A57C, 2000, OldProtect3, &OldProtect3);
//    ////VirtualProtect((void*)0x6E2170, 0x13C, OldProtect4, &OldProtect4);
//    ////VirtualProtect((void*)b_ScriptSet, 2000, OldProtect5, &OldProtect5);
//    ////oFormatCallNotReturn(This, "\\script\\protocol.lua", "ScriptProtocol:ProtocolProcess", "dd", nProtolId, nHandle); // 0x19FD50
//    ////oFormatCallNotReturn(This, "\\script\\protocol.lua", "ScriptProtocol:ProtocolProcess", "dd"); // 0x19FD50
//    ////va_end(vlist);
//    //return 0;
//}
//
//int __stdcall cRegisterFunctions(struct TLua_Funcs* pFunctionList, int nCount)
//{
//    DWORD OldProtect;
//    int TopIndex = 0;
//    //ofstream myfile;
//    ////TopIndex = lua_gettop((lua_State*)&g_ScriptSet + 72);
//    //myfile.open("E:\\example.txt");
//    ////myfile << g_ScriptSet << "\n";
//    ////myfile << b_TmpScript << "\n";
//    ////myfile << &b_TmpScript + 67 << "\n";
//    //myfile << nCount << "\n";
//
//    //myfile << TopIndex << "\n";
//
//    //myfile.close();
//    //MessageBox(0, "Start", "Error", MB_OK | MB_ICONINFORMATION);
//    //struct KLuaScript* Script;
//
//    //char numberstring[32];
//    //sprintf(numberstring, "%d", g_ScriptSet);
//    //MessageBox(0, numberstring, "Info", MB_OK | MB_ICONINFORMATION);
//    //////Script = _Hook_CreateScript(g_ScriptSet);
//    //sprintf(numberstring, "%p", &g_ScriptSet->tmp[0x4]);
//    //MessageBox(0, numberstring, "Info", MB_OK | MB_ICONINFORMATION);
//
//    //if (nCount <= 0 || !g_ScriptSet->tmp[0x48])
//    //    return;
//
//    VirtualProtect((void*)0x6E2170, 0x13C, PAGE_EXECUTE_READWRITE, &OldProtect);
//    //VirtualProtect((void*)0x63A578, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//    //m_RegisterFunctions *function = (m_RegisterFunctions*)0x63A578;
//    //function(pFunctionList, nCount);
//    //Script = (struct KLuaScript*)*((_DWORD*)g_ScriptSet + 73);
//    //_custom_lua_pushcclosure((lua_State*)g_ScriptSet->tmp[0x12C], LuaGetMac, 0);
//    //_custom_lua_setglobal((lua_State*)g_ScriptSet->tmp[0x12C], "GetMac");
//    //lua_register((lua_State*)*((DWORD*)g_ScriptSet + 67), "GetMac", LuaGetMac);
//    //oRegisterFunctions(pFunctionList, nCount);
//    //d_RegisterFunctions(pFunctionList, nCount);
//    MessageBox(0, "pFunctionList OK", "Error", MB_OK | MB_ICONINFORMATION);
//    //oRegisterFunctions(g_ScriptSet, CustomScriptFuns, g_GetCustomScriptFunNum());
//    //lua_State* L = lua_open(500);
//    ////(KLuaScriptSet*)&unk_6E2170;
//    ////lua_State *L = (lua_State*)&Script->tmp[0x10C];
//    ////lua_State *L = (lua_State*)((_DWORD*)&g_ScriptSet + 72);
//    //if (L)
//    //{
//    //    //char lszErrMsg[200]; // [esp+1Ch] [ebp-CCh] BYREF
//    //    //sprintf(lszErrMsg, "ScriptError %d. (%s) (%s) (%s) (%s) \n", &g_ScriptSet, &g_ScriptSet->tmp[0x4], &g_ScriptSet->tmp[0x8], &g_ScriptSet->tmp[0x12C], &g_ScriptSet->tmp[0x130]);
//    //    //MessageBox(0, lszErrMsg, "Error", MB_OK | MB_ICONINFORMATION);
//    //    //RegisterFunctions(g_ScriptSet, pFunctionList, nCount);
//    //    //MessageBox(0, "pFunctionList OK", "Error", MB_OK | MB_ICONINFORMATION);
//    //    
//    //    //RegisterFunctions(g_ScriptSet, CustomScriptFuns, g_GetCustomScriptFunNum());
//    //    //MessageBox(0, "Work 1", "Error", MB_OK | MB_ICONINFORMATION);
//    //    //int i; // esi
//    //    //const char* name;       
//    //    //lua_register(L, "GetMac", LuaGetMac);
//    //    //lua_pushcclosure((lua_State*)(*((_DWORD*)g_ScriptSet + 72)), LuaGetMac, 0);
//    //    //lua_pushcclosure(L, LuaGetMac, 1);
//    //    //MessageBox(0, "Work 1", "Error", MB_OK | MB_ICONINFORMATION);
//    //    //lua_setglobal(L, "GetMac");
//    //    //lua_setglobal((lua_State*)(*((_DWORD*)g_ScriptSet + 72)), *(LPCSTR*)"GetMac");
//    //    //MessageBox(0, "Work 2", "Error", MB_OK | MB_ICONINFORMATION);
//    //    //for (i = 0; i < g_GetCustomScriptFunNum(); ++i)
//    //    //{
//    //    //    try
//    //    //    {
//    //    //        //lszErrMsg[200]; // [esp+1Ch] [ebp-CCh] BYREF
//    //    //        //sprintf(lszErrMsg, "ScriptError %d. (%s) \n", &Script->tmp[0x4], &Script->tmp[0x8]);
//    //    //        //MessageBox(0, lszErrMsg, "Error", MB_OK | MB_ICONINFORMATION);
//    //    //        //lua_pushcclosure(L, *(lua_CFunction*)&CustomScriptFuns[i].func, 1);
//
//
//    //    //        //name = CustomScriptFuns[i].name;
//    //    //        //MessageBox(0, name, "Error", MB_OK | MB_ICONINFORMATION);
//    //    //        //sprintf(numberstring, "p %p", &CustomScriptFuns[i].func);
//    //    //        //MessageBox(0, numberstring, "Error", MB_OK | MB_ICONINFORMATION);
//    //    //        
//    //    //        //MessageBox(0, "Work 2", "Error", MB_OK | MB_ICONINFORMATION);
//    //    //        //lua_setglobal(L, name);
//    //    //    }
//    //    //    catch (std::exception& e)
//    //    //    {
//    //    //        //luaL_error(L, e.what());
//    //    //        MessageBox(0, e.what(), "Error", MB_OK | MB_ICONINFORMATION);
//    //    //    }
//    //    //}
//
//    //    //lua_settop((lua_State*)g_ScriptSet->tmp[0x48], 1);
//    //    //MessageBox(0, "Work 1", "Error", MB_OK | MB_ICONINFORMATION);
//    //    //lua_pushcclosure(L, (lua_CFunction)(void*)LuaGetMac, 0);
//    //    //lua_pushcclosure(L, (lua_CFunction)(void*)LuaGetMac, 0);
//    //   
//    //    //lua_setglobal(L, "GetMac");
//    //    //lua_setglobal(L, "GetMac");
//    //    MessageBox(0, "CustomScriptFuns OK", "Error", MB_OK | MB_ICONINFORMATION);
//    //     
//    //    // //MessageBox(0, "Work", "Error", MB_OK | MB_ICONINFORMATION);
//    //    ////lua_settop((lua_State*)g_ScriptSet->tmp[0x48], 1);
//    //    ////MessageBox(0, "Work 1", "Error", MB_OK | MB_ICONINFORMATION);
//    //    ////lua_pushcclosure((lua_State*)((DWORD*)g_ScriptSet->tmp[0x48]), (lua_CFunction)(void*)LuaGetMac, 0);
//    //    //lua_pushcclosure(L, (lua_CFunction)(void*)LuaGetMac, 0);
//    //    //MessageBox(0, "Work 2", "Error", MB_OK | MB_ICONINFORMATION);
//    //    ////lua_setglobal((lua_State*)((DWORD*)g_ScriptSet->tmp[0x48]), *(LPCSTR*)"GetMac");
//    //    //lua_setglobal(L, "GetMac");
//    //    //MessageBox(0, "CustomScriptFuns OK", "Error", MB_OK | MB_ICONINFORMATION);
//    //}
//    //else
//    //    MessageBox(0, "Error!", "Error", MB_OK | MB_ICONINFORMATION);
//    return 1;
//}
//
////KLuaScript::KLuaScript(void)
////{
////    m_LuaState = lua_open(100);
////
////    if (m_LuaState == NULL)
////    {
////        ScriptError(LUA_CREATE_ERROR);
////        m_IsRuning = FALSE;
////        return;
////    }
////
////    m_IsRuning = TRUE;
////    m_szScriptName[0] = '\0';
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::KLuaScript
////// 功能:	
////// 参数:	int StackSize
////// 返回:	
//////---------------------------------------------------------------------------
////KLuaScript::KLuaScript(int StackSize = 0)
////{
////    m_LuaState = Lua_Create(StackSize);
////
////    if (m_LuaState == NULL)
////    {
////        ScriptError(LUA_CREATE_ERROR);
////        m_IsRuning = FALSE;
////        return;
////    }
////    m_IsRuning = TRUE;
////    m_szScriptName[0] = '\0';
////}
////
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::~KLuaScript
////// 功能:	
////// 参数:	void
////// 返回:	
//////---------------------------------------------------------------------------
////KLuaScript::~KLuaScript(void)
////{
////    Exit();
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::LoadBuffer()
////// 功能:	
////// 参数:	PBYTE pBuffer
////// 参数:	DWORD dwLen
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::LoadBuffer(PBYTE pBuffer, DWORD dwLen)
////{
////    if (dwLen < 0)
////    {
////        ScriptError(LUA_SCRIPT_LEN_ERROR);
////        return FALSE;
////    }
////
////    if (Lua_CompileBuffer(m_LuaState, (char*)pBuffer, dwLen, NULL) != 0)
////    {
////        ScriptError(LUA_SCRIPT_COMPILE_ERROR);
////        return FALSE;
////    }
////    return TRUE;
////}
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::Load
////// 功能:	
////// 参数:	LPSTR Filename
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::Load(char* Filename)
////{
////    //KPakFile	File;
////    //DWORD		Size;
////
////
////    //// open file
////    //if (!File.Open(Filename))	return FALSE;
////
////    //// get file size
////    //Size = File.Size();
////
////    //KMemClass Memory;
////    //// alloc memory
////    //if (!Memory.Alloc(Size + 4))
////    //    return FALSE;
////
////    //// read file
////    //if (File.Read(Memory.GetMemPtr(), Size) != Size)
////    //    return FALSE;
////    //char* pszMem = (char*)Memory.GetMemPtr();
////    //pszMem[Size + 1] = 0;
////
////    //File.Close();
////    //try
////    //{
////    //    if (!LoadBuffer((PBYTE)Memory.GetMemPtr(), Size))
////    //    {
////    //        ScriptError(LUA_SCRIPT_COMPILE_ERROR);
////    //        return FALSE;
////    //    }
////    //}
////    //catch (...)
////    //{
////    //    printf("Load Script %s 出现异常，请检查!!\n", Filename);
////    //    return FALSE;
////    //}
////
////    //if (!ExecuteCode()) return FALSE;
////
////    return TRUE;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::Execute
////// 功能:	
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::Execute()
////{
////  /*  if (m_IsRuning && m_LuaState)
////        return this->CallFunction(MAINFUNCTIONNAME, 0, "");*/
////
////    return FALSE;
////}
////
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::ExecuteCode
////// 功能:	
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::ExecuteCode()
////{
////    if (!(m_IsRuning && m_LuaState))
////    {
////        ScriptError(LUA_SCRIPT_EXECUTE_ERROR);
////        //if (!ExecuteCode()) return FALSE; ZHANGPENG 发现这里可能有错
////        return FALSE;
////    }
////
////    int state;
////    if (state = Lua_Execute(m_LuaState) != 0)
////    {
////        ScriptError(LUA_SCRIPT_EXECUTE_ERROR, state);
////        return FALSE;
////    }
////
////    return	TRUE;
////}
////
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::CallFunction
////// 功能:	调用Lua脚本内的函数
////// 参数:	LPSTR cFuncName
////// 参数:	int nResults
////// 参数:	LPSTR cFormat  调用时所传参数的类型 
//////			n:数字型(double) d:整形(int) s:字符串型 f:C函数型  n:Nil v:Value p:Point
//////        v形为Lua支持的，参数为整形的数index，指明将index所指堆栈的变量作为
//////			 该函数的调用参数。
//////	注意：由于该函数有不定参数…,对于数字，系统并不确定数是以double还是以int
//////  存在，两种保存形式是不同的。因此需要注意当传入的数是整形时，格式符应用d
//////  而不能用n,或者强行改变为double形。否则会出现计算的错误。
//////   
////// 参数:	...
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::CallFunction(LPSTR cFuncName, int nResults, LPSTR cFormat, va_list vlist)
////{
////
////    double nNumber;
////    char* cString = NULL;
////    void* pPoint = NULL;
////    Lua_CFunction CFunc;
////    int i = 0;
////    int nArgnum = 0;
////    int nIndex = 0;
////    int nRetcode;		//调用脚本函数后的返回码
////
////    if (!(m_IsRuning && m_LuaState))
////    {
////        ScriptError(LUA_SCRIPT_STATES_IS_NULL);
////        return FALSE;
////    }
////
////    {
////        Lua_GetGlobal(m_LuaState, cFuncName); //在堆栈中加入需要调用的函数名
////
////        while (cFormat[i] != '\0')
////        {
////            switch (cFormat[i])
////            {
////            case 'n'://输入的数据是double形 NUMBER，Lua来说是Double型
////            {
////                nNumber = va_arg(vlist, double);
////                Lua_PushNumber(m_LuaState, nNumber);
////                nArgnum++;
////
////            }
////            break;
////
////            case 'd'://输入的数据为整形
////            {
////                nNumber = (double)(va_arg(vlist, int));
////                Lua_PushNumber(m_LuaState, (double)nNumber);
////                nArgnum++;
////            }
////            break;
////
////            case 's'://字符串型
////            {
////                cString = va_arg(vlist, char*);
////                Lua_PushString(m_LuaState, cString);
////                nArgnum++;
////            }
////            break;
////            case 'N'://NULL
////            {
////                Lua_PushNil(m_LuaState);
////                nArgnum++;
////            }
////            break;
////
////            case 'f'://输入的是CFun形，即内部函数形
////            {
////                CFunc = va_arg(vlist, Lua_CFunction);
////                Lua_PushCFunction(m_LuaState, CFunc);
////                nArgnum++;
////            }
////            break;
////
////            case 'v'://输入的是堆栈中Index为nIndex的数据类型
////            {
////                nNumber = va_arg(vlist, int);
////                int nIndex1 = (int)nNumber;
////                Lua_PushValue(m_LuaState, nIndex1);
////                nArgnum++;
////            }
////            break;
////            case 't'://输入为一Table类型
////            {
////
////
////
////            }
////            break;
////
////            case 'p':
////            {
////                pPoint = va_arg(vlist, void*);
////
////                Lua_PushUserTag(m_LuaState, pPoint, m_UserTag);
////                nArgnum++;
////            }
////            break;
////            }
////
////            i++;
////        }
////
////    }
////
////    nRetcode = Lua_Call(m_LuaState, nArgnum, nResults);
////
////    if (nRetcode != 0)
////    {
////        ScriptError(LUA_SCRIPT_EXECUTE_ERROR, nRetcode);
////        return FALSE;
////    }
////
////
////    return	TRUE;
////}
////
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::CallFunction
////// 功能:	
////// 参数:	LPSTR cFuncName
////// 参数:	int nResults
////// 参数:	LPSTR cFormat
////// 参数:	...
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::CallFunction(LPSTR cFuncName, int nResults, LPSTR cFormat, ...)
////{
////    BOOL bResult = FALSE;
////    va_list vlist;
////    va_start(vlist, cFormat);
////    bResult = CallFunction(cFuncName, nResults, cFormat, vlist);
////    va_end(vlist);
////    return bResult;
////}
////
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::GetValuesFromStack
////// 功能:	从堆栈中获得变量
////// 参数:	char * cFormat
////// 参数:	...
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::GetValuesFromStack(char* cFormat, ...)
////{
////    va_list vlist;
////    double* pNumber = NULL;
////    const char** pString;
////    int* pInt = NULL;
////    int i = 0;
////    int nTopIndex = 0;
////    int nIndex = 0;
////    int nValueNum = 0;//cFormat的字符长度，表示需要取的参数数量
////
////    if (!m_LuaState)
////        return FALSE;
////
////    nTopIndex = Lua_GetTopIndex(m_LuaState);
////    nValueNum = strlen(cFormat);
////
////    if (nTopIndex == 0 || nValueNum == 0)//当堆栈中无数据或不取参数是返回FALSE
////        return FALSE;
////
////    if (nTopIndex < nValueNum)
////        return FALSE;
////
////    nIndex = nTopIndex - nValueNum + 1;
////
////    {
////        va_start(vlist, cFormat);
////
////        while (cFormat[i] != '\0')
////        {
////
////            switch (cFormat[i])
////            {
////            case 'n'://返回值为数值形,Number,此时Lua只传递double形的值
////            {
////                pNumber = va_arg(vlist, double*);
////
////                if (pNumber == NULL)
////                    return FALSE;
////
////                if (Lua_IsNumber(m_LuaState, nIndex))
////                {
////                    *pNumber = Lua_ValueToNumber(m_LuaState, nIndex++);
////
////                }
////                else
////                {
////                    ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
////                    return FALSE;
////                }
////
////
////            }
////            break;
////            case 'd':
////            {
////                pInt = va_arg(vlist, int*);
////                if (pInt == NULL)
////                    return FALSE;
////                if (Lua_IsNumber(m_LuaState, nIndex))
////                {
////                    *pInt = (int)Lua_ValueToNumber(m_LuaState, nIndex++);
////                }
////                else
////                {
////                    ScriptError(LUA_SCRIPT_NOT_NUMBER_ERROR);
////                    return FALSE;
////                }
////
////            }
////            break;
////            case 's'://字符串形
////            {
////                pString = va_arg(vlist, const char**);
////
////                if (pString == NULL)
////                    return FALSE;
////
////                if (Lua_IsString(m_LuaState, nIndex))
////                {
////                    (*pString) = (const char*)Lua_ValueToString(m_LuaState, nIndex++);
////
////                }
////                else
////                {
////                    ScriptError(LUA_SCRIPT_NOT_STRING_ERROR);
////                    return FALSE;
////                }
////            }
////            break;
////
////            }
////
////
////            i++;
////        }
////        va_end(vlist);     		/* Reset variable arguments.      */
////
////    }
////    return	TRUE;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::Init
////// 功能:	初始化脚本对象，注册系统标准函数库
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::Init()
////{
////    if (!m_LuaState)
////    {
////        m_LuaState = Lua_Create(0);
////
////        if (m_LuaState == NULL)
////        {
////            ScriptError(LUA_CREATE_ERROR);
////            m_IsRuning = FALSE;
////            return FALSE;
////        }
////
////        m_IsRuning = TRUE;
////        m_szScriptName[0] = '\0';
////        m_UserTag = lua_newtag(m_LuaState);
////    }
////
////    RegisterStandardFunctions();
////    return	TRUE;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::RegisterFunction
////// 功能:	注册某内部C函数至脚本中
////// 参数:	LPSTR FuncName  在脚本中使用的函数名
////// 参数:	void* Func    实际相应的C函数指针
////// 参数:	int Args = 0 //与KScript接口相容，无用
////// 参数:	int Flag = 0 //与KScript接口相容, 无用
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::RegisterFunction(LPSTR FuncName, void* Func)
////{
////    if (!m_LuaState)
////        return FALSE;
////    Lua_Register(m_LuaState, FuncName, (Lua_CFunction)Func);
////    return TRUE;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::Compile
////// 功能:	
////// 参数:	char *
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::Compile(char*)
////{
////    return TRUE;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::RegisterFunctions
////// 功能:	批量注册Lua的内部C函数，各个函数的信息保存在TLua_Funcs的数据中
////// 参数:	TLua_Funcs *Funcs 数组的指针
////// 参数:	int n 函数数量。可以为零，由系统计算得到。
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::RegisterFunctions(TLua_Funcs Funcs[], int n)
////{
////    const char* name;
////    if (!m_LuaState)	return FALSE;
////    if (n == 0)	n = sizeof(Funcs) / sizeof(Funcs);
////    for (int i = 0; i < n; i++)
////    {
////        lua_pushcclosure(m_LuaState, Funcs[i].func, 0);
////        name = Funcs[i].name;
////        lua_setglobal(m_LuaState, name);
////        //Lua_Register(m_LuaState, Funcs[i].name, Funcs[i].func);
////    }
////    return TRUE;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::RegisterStandardFunctions
////// 功能:	注册Lua系统标准的函数库
////// 返回:	void 
//////---------------------------------------------------------------------------
////void KLuaScript::RegisterStandardFunctions()
////{
////    if (!m_LuaState)		return;
////    Lua_OpenBaseLib(m_LuaState);//Lua基本库
////    Lua_OpenIOLib(m_LuaState);//输入输出库
////    Lua_OpenStrLib(m_LuaState);//字符串处理库
////    Lua_OpenMathLib(m_LuaState);//数值运算库
////    Lua_OpenDBLib(m_LuaState);//调试库
////    return;
////}
////
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::ReleaseScript
////// 功能:	释放该脚本资源。
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////void KLuaScript::Exit()
////{
////
////    if (!m_LuaState)		return;
////    Lua_Release(m_LuaState);
////    m_LuaState = NULL;
////    m_IsRuning = FALSE;
////
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::ScriptError
////// 功能:	
////// 参数:	int Error
////// 返回:	void 
//////---------------------------------------------------------------------------
////void KLuaScript::ScriptError(int Error)
////{
////    char lszErrMsg[200];
////    sprintf(lszErrMsg, "ScriptError %d. (%s) \n", Error, m_szScriptName);
////    lua_outerrmsg(lszErrMsg);
////    return;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::ScriptError
////// 功能:	
////// 参数:	int Error1
////// 参数:	int Error2
////// 返回:	void 
//////---------------------------------------------------------------------------
////void KLuaScript::ScriptError(int Error1, int Error2)
////{
////    char lszErrMsg[200];
////    sprintf(lszErrMsg, "ScriptError %d:[%d] (%s) \n", Error1, Error2, m_szScriptName);
////    lua_outerrmsg(lszErrMsg);
////    return;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::SafeCallBegin
////// 功能:	
////// 参数:	int * pIndex
////// 返回:	void 
//////---------------------------------------------------------------------------
//////---------------------------------------------------------------------------
////// SafeCallBegin与SafeCallEnd两函数应搭配使用，以防止在调用Lua的外部函数之后，
//////有多余数据在堆栈中未被清除。达到调用前与调用后堆栈的占用大小不变。
//////上述情况只需用在调用外部函数时，内部函数不需如此处理。
//////																	Romandou
//////---------------------------------------------------------------------------
////void KLuaScript::SafeCallBegin(int* pIndex)
////{
////    if (!m_LuaState)		return;
////    Lua_SafeBegin(m_LuaState, pIndex);
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::SafeCallEnd
////// 功能:	
////// 参数:	int nIndex
////// 返回:	void 
//////---------------------------------------------------------------------------
////void KLuaScript::SafeCallEnd(int nIndex)
////{
////    if (!m_LuaState)	return;
////    Lua_SafeEnd(m_LuaState, nIndex);
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::StopScript
////// 功能:	中止脚本
////// 参数:	void
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::Stop(void)
////{
////    if (!m_IsRuning)		return TRUE;
////    if (!m_LuaState)		return FALSE;
////    m_IsRuning = FALSE;
////    return TRUE;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::ResumeScript
////// 功能:	恢复已中止的脚本
////// 参数:	void
////// 返回:	BOOL 
//////---------------------------------------------------------------------------
////BOOL KLuaScript::Resume(void)
////{
////    if ((!m_IsRuning) && (m_LuaState))
////    {
////        m_IsRuning = TRUE;
////        return TRUE;
////    }
////    return FALSE;
////}
////
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::CreateTable
////// 功能:	建立一个Lua的Table，在调用该函数并设置Table各个成员之后，必须调用
//////			SetGlobalName()来给这个Table指定一个名字。
////// 返回:	DWORD 
//////---------------------------------------------------------------------------
////DWORD KLuaScript::CreateTable()
////{
////    int nIndex = 0;
////
////    nIndex = Lua_GetTopIndex(m_LuaState);
////    Lua_NewTable(m_LuaState);
////    if (Lua_GetTopIndex(m_LuaState) != ++nIndex)
////        return -1;
////
////    return nIndex;
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::SetGlobalName
////// 功能:	设置Lua堆栈顶部的数据一个名字
////// 参数:	LPSTR szName
////// 返回:	void 
//////---------------------------------------------------------------------------
////void KLuaScript::SetGlobalName(LPSTR szName)
////{
////    if (!szName) return;
////    Lua_SetGlobal(m_LuaState, szName);
////}
////
//////---------------------------------------------------------------------------
////// 函数:	KLuaScript::ModifyTable
////// 功能:	将指定名称的LuaTable置堆栈顶端，并返回顶端Index
////// 参数:	LPSTR szTableName
////// 返回:	DWORD 若Lua中不存在该Table则返回-1
//////---------------------------------------------------------------------------
////DWORD KLuaScript::ModifyTable(LPSTR szTableName)
////{
////    if (!szTableName[0])		return -1;
////
////    int nIndex = Lua_GetTopIndex(m_LuaState);
////
////    Lua_GetGlobal(m_LuaState, szTableName);
////
////    if (Lua_GetTopIndex(m_LuaState) != ++nIndex)		return -1;
////
////    return nIndex;
////}
//static int __cdecl LuaGetMac(int Lua)
//{
//
//    if (gKLuaScript.m_UserTag == 0)
//    {
//        DWORD OldProtect;
//        DWORD OldProtect1;
//        DWORD OldProtect2;
//        DWORD OldProtect3;
//        DWORD OldProtect4;
//        DWORD OldProtect5;
//        DWORD OldProtect6;
//        DWORD OldProtect7;
//        DWORD OldProtect8;
//        DWORD OldProtect9;
//        int nPlayerIndex;
//        int nTop = 0;
//        int nResul = 0;
//        char HardwareInfo2[100];
//
//        char HardwareInfo[100];
//
//        //ofstream myfile;
//        //myfile.open("E:\\example.txt");
//        //myfile << MacAddress << "\n";
//        //myfile << Lua << "\n";
//
//        //char szDesMsg[200];
//        //szDesMsg[0] = 0;
//        //strcpy(szDesMsg, "0.0.0.0");
//
//        //int PlayerIndex = 0x64E5E8;
//        //int PlayerIndex = GetPlayerIndex(*(*(_DWORD*)L));
//        //int PlayerIndex = GetPlayerIndex(L);
//        //unsigned int Player = 165956 * PlayerIndex;
//
//        //char numberstring[32];
//        //sprintf(numberstring, "%d", (int)L);
//
//        //MessageBox(0, numberstring, "Error", MB_OK | MB_ICONINFORMATION);
//        //MessageBox(0, MacAddress, "Error", MB_OK | MB_ICONINFORMATION);
//        //KPlayer* Player = (KPlayer*)Player_Struct;
//        //bool sendok = SendData((KPlayer*)&Player[PlayerIndex], &gethwid, sizeof(G2C_PLAYER_GETHWID));
//        //if (sendok)
//        //    _custom_lua_pushstring(L, MacAddress);
//
//        //_custom_lua_pushstring(L, *((_DWORD*)&MacAddress));
//        //d_lua_pushstring(*((_DWORD*)L), *((_DWORD*)&MacAddress));
//        //d_lua_pushstring(*((_DWORD*)L), *((_DWORD*)MacAddress));
//        //int nPlayerIndex = lua_tointeger((lua_State*)(DWORD*)L, 1);
//        //lua_pushstring((lua_State*)(DWORD*)L, MacAddress);
//        //VirtualProtect((void*)L, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)d_lua_pushstring, 2000, PAGE_EXECUTE_READWRITE, &OldProtect1);
//
//        //int nPlayerIndex = lua_tointeger(L, 1);
//
//        //VirtualProtect((void*)MacAddress, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((void*)lua_pushstring, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//
//        //if (nPlayerIndex == NULL || nPlayerIndex <= 0)
//        //{
//        //    lua_getglobal(L, "PlayerIndex");
//        //    nPlayerIndex = lua_tonumber(L, 1);
//        //}
//        //myfile << nPlayerIndex << "\n";
//        //myfile.close();
//
//        //VirtualProtect((LPVOID)0x10001720, 2000, PAGE_EXECUTE_READWRITE, &OldProtect2);
//        //VirtualProtect((LPVOID)0x100018B0, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
//        //VirtualProtect((LPVOID)0x1FE8C1C2, 2000, PAGE_EXECUTE_READWRITE, &OldProtect4);
//        //VirtualProtect((LPVOID)0x2d0464ea, 2000, PAGE_EXECUTE_READWRITE, &OldProtect5);
//        //VirtualProtect((LPVOID)0x00651fbc, 2000, PAGE_EXECUTE_READWRITE, &OldProtect6);
//        //d_lua_pushstring(*(_DWORD*)L, &MacAddress);//0x2e5f5e12 0x100018b0 0x2e43c522
//        //lua_pushstring(*((DWORD*)L), MacAddress);
//        //const char* v1; // eax
//        //int v2;
//        //struct KLuaScript* Script; // eax
//
//        
//        //VirtualProtect((LPVOID)hModuleLuaLibDll, sizeof(hModuleLuaLibDll), PAGE_EXECUTE_READWRITE, &OldProtect1);
//        //VirtualProtect((LPVOID)Lua, 2000, PAGE_EXECUTE_READWRITE, &OldProtect2);
//        //VirtualProtect((LPVOID)MacAddress, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
//        //VirtualProtect((LPVOID)0x10013754, 2000, PAGE_EXECUTE_READWRITE, &OldProtect4);
//        //VirtualProtect((LPVOID)0x10013754, 2000, PAGE_EXECUTE_READWRITE, &OldProtect5);
//        //VirtualProtect((LPVOID)0x01efc0b6, 2000, PAGE_EXECUTE_READWRITE, &OldProtect6);
//        //VirtualProtect((LPVOID)0x018ac0b6, 2000, PAGE_EXECUTE_READWRITE, &OldProtect7);
//        //VirtualProtect((LPVOID)0x01e9c0b6, 2000, PAGE_EXECUTE_READWRITE, &OldProtect8);
//        //VirtualProtect((LPVOID)0x01e7c0b6, 2000, PAGE_EXECUTE_READWRITE, &OldProtect9);
//
//        //VirtualProtect((LPVOID)0x69727473, 2000, PAGE_EXECUTE_READWRITE, &OldProtect9);
//        //VirtualProtect((LPVOID)0x716a1eeb, 2000, PAGE_EXECUTE_READWRITE, &OldProtect9);
//
//        //lua_pushstring pushstring = (lua_pushstring)GetProcAddress(hModuleLuaLibDll, "lua_pushstring");
//        //lua_gettop gettop = (lua_gettop)GetProcAddress(hModuleLuaLibDll, "lua_gettop");
//        //lua_pushnumber pushnumber = (lua_pushnumber)GetProcAddress(hModuleLuaLibDll, "lua_pushnumber");
//        //lua_pushcclosure pushcclosure = (lua_pushcclosure)GetProcAddress(hModuleLuaLibDll, "lua_pushcclosure");
//        //lua_setglobal setglobal = (lua_setglobal)GetProcAddress(hModuleLuaLibDll, "lua_setglobal");
//        ////lua_call luacall = (lua_call)GetProcAddress(hModuleLuaLibDll, "lua_call");
//        //lua_baselibopen baselibopen = (lua_baselibopen)GetProcAddress(hModuleLuaLibDll, "lua_baselibopen");
//        //lua_dblibopen dblibopen = (lua_dblibopen)GetProcAddress(hModuleLuaLibDll, "lua_dblibopen");
//        //lua_iolibopen iolibopen = (lua_iolibopen)GetProcAddress(hModuleLuaLibDll, "lua_iolibopen");
//        //lua_mathlibopen mathlibopen = (lua_mathlibopen)GetProcAddress(hModuleLuaLibDll, "lua_mathlibopen");
//        //lua_strlibopen strlibopen = (lua_strlibopen)GetProcAddress(hModuleLuaLibDll, "lua_strlibopen");
//
//        //VirtualProtect((LPVOID)pushstring, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)gettop, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)pushnumber, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)pushcclosure, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)setglobal, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        ////VirtualProtect((LPVOID)luacall, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)baselibopen, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)dblibopen, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)iolibopen, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)mathlibopen, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //VirtualProtect((LPVOID)strlibopen, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
//        //0x69727473 0x716a1eeb 0x69727473.0x716a1eeb 0x69727473.0x716a1eeb 
//
//        baselibopen(Lua);
//        dblibopen((int*)Lua);
//        iolibopen((int*)Lua);
//        mathlibopen((int*)Lua);
//        strlibopen((int*)Lua);
//
//        //sprintf(HardwareInfo, "%s|%s|%s", MacAddress, IpAddress, HardwareId);
//
//        //strcpy(HardwareInfo2, HardwareInfo);
//        ////memcpy((void*)&HardwareInfo2, (void*)&HardwareInfo, sizeof(HardwareInfo));
//
//        //pushstring(Lua, HardwareInfo);
//
//        //setglobal((int*)Lua, "szHardwareInfo");
//
//        //pushstring(Lua, MacAddress);
//
//        //setglobal((int*)Lua, "szMacAddress");
//
//        //pushstring(Lua, IpAddress);
//
//        //setglobal((int*)Lua, "szIpAddress");
//
//        //pushstring(Lua, HardwareId);
//
//        //setglobal((int*)Lua, "szHardwareId");
//
//        //luacall((int*)Lua,0,1);
//        //char numberstring[32];
//        //VirtualProtect((LPVOID)L, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
//        //VirtualProtect((LPVOID)lua_tolstring, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
//        //MessageBox(0, MacAddress, "Error", MB_OK | MB_ICONINFORMATION);
//        //myfile << *(_DWORD*)&pushstring << "\n";
//        //myfile << (void*)&gettop << "\n";
//        //nTop = gettop((_DWORD*)(*(_DWORD*)Lua));
//        //myfile << gettop << "\n";
//        //myfile << MacAddress << "\n";
//        //myfile.close();
//        //if (nTop >= 1)
//        //{
//        //    //lua_pushnumber(L, 1);
//        //    
//        //    return 0;
//        //    //myfile << MacAddress << "\n";
//        //    //VirtualProtect((LPVOID)&v1, 2000, PAGE_EXECUTE_READWRITE, &OldProtect3);
//        //    //memcpy((void*)&v1, (void*)lua_tolstring(L, 1, NULL), sizeof(v1));
//        //    ////v1 = lua_tolstring(L, 1, NULL);
//        //    //
//        //    //if (v1)
//        //    //{
//        //    //    sprintf(numberstring, "%s", v1);
//        //    //    MessageBox(0, numberstring, "Error", MB_OK | MB_ICONINFORMATION);
//        //    //    Script = d_KLuaScriptSet_GetScript((KLuaScriptSet*)structKLuaScriptSet, v1);
//        //    //    if (Script)
//        //    //        (*(void(__thiscall**)(struct KLuaScript*))(*(_DWORD*)Script + 20))(Script);
//        //    //}
//        //    //else
//        //    //    MessageBox(0, "NULL", "Error", MB_OK | MB_ICONINFORMATION);
//        //}
//        MessageBox(0, "OK", "Error", MB_OK | MB_ICONINFORMATION);
//        //pushstring(Lua, HardwareInfo2);
//        //pushstring(Lua, gProtect.m_HardwareId);
//        //pushnumber(Lua, 100);
//        //FreeLibrary(hModuleLuaLibDll);
//        gKLuaScript.m_UserTag = 1;
//    }
//    
//    return 2;
//};
#pragma once

typedef unsigned __int64 QWORD;
typedef unsigned int _DWORD;

#define WIN32_LEAN_AND_MEAN

#define _WIN32_WINNT _WIN32_WINNT_WINXP

#define Anti-Cheat-Client_VERSION "JX60"

#define CLIENT_VERSION 9

#define USE_LIMIT_LOCAL 1

#define USE_LIMIT_AUTH 1

#define USE_LIMIT_AUTH_MULTI 1

#define USE_CLIENTV8 0

#define CALL_AUTOUPDATE 0

#define ENCRYPT_LOG 1

#define ICONSKILL 0

#define AUTH_TYPE 1

#define DEBUG 0

#define GAME_LOADER 1

//#define _DEBUG 1
//#define _WIN32_WINNT  0x0400
//#include <windows.h>
//#define DIRECTINPUT_VERSION 0x800
// System Include
#include <windows.h>
#include <iostream>
#include <map>
#include <math.h>
#include <stdlib.h>
#include <winsock2.h>
#include <vector>
#include <Psapi.h>
#include "detours.h"
#include <Shlwapi.h>
#include <Dbghelp.h>
#include <wchar.h>
#include <Rpc.h>
#include <winternl.h>
#include <winioctl.h>
#include <WtsApi32.h>

#pragma comment(lib,"Wtsapi32.lib") 
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Psapi.lib")
#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"detours.lib")
#pragma comment(lib,"Rpcrt4.lib")
#pragma comment(lib,"engine.lib")
#pragma comment(lib,"WinLicenseSDK32.lib")
#pragma comment(lib,"SecureEngineSDK32.lib")
//#pragma comment(lib,"fProtectLib.lib")
//#pragma comment(lib,"d3d8.lib")
//#pragma comment(lib,"d3dx.lib")
//#pragma comment(lib,"d3dx8.lib")
//#pragma comment(lib,"d3dx8d.lib")
//#pragma comment(lib,"d3dx8dt.lib")
//#pragma comment(lib,"dinput8.lib")
//#pragma comment(lib,"GdiPlus.lib")

//typedef int(WINAPI* MBA)(HWND, LPCSTR, LPCSTR, UINT);

//odbc32.lib
//odbccp32.lib
//Winmm.lib
//shlwapi.lib

#if (ICONSKILL==1)
//#pragma comment(lib,"common.lib")
#pragma comment(lib,"lualib.lib")
//#pragma comment(lib,"CoreClient.lib")
//#pragma comment(lib,"Represent.lib")
//#pragma comment(lib,"GdiPlus.lib")
//#pragma comment(lib, "d3d9.lib")
//#pragma comment(lib,"shlwapi.lib")
#endif

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
//#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
//#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
//#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
//#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
//#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
//#pragma comment(lib,"Represent.lib")
//MBA originalMBA = NULL;

// STL
#pragma warning(disable : 4786)

#define _GNU_SOURCE
#define __int8 char
#define __int16 short
#define __int32 int
#define __int64 long long
typedef unsigned __int8 BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef __int64 INT64;
typedef unsigned long ULONG_PTR, * PULONG_PTR;
typedef unsigned int mem_t;
typedef unsigned char byte_t;
typedef unsigned int uint32_t;
typedef unsigned int UINT;
typedef int BOOL;
typedef void* LPVOID;
typedef const void* LPCVOID;
typedef const char* LPCSTR;
typedef unsigned int _DWORD;

#define _DEBUG 0
#define AUTH_TYPE 1
#define USE_RECHECK_AUTH_TRAP 0
#define USE_RELOAD_RANK 0
#define _DEBUG_SQLITE 0
#define GAME_VERSION 3
#define GAME_VERSION_SUB "7.9"
#define S3REPLAY_VERSION 3
#define S3REPLAY_VERSION_SUB 0
#define GODDESS_VERSION 3
#define GODDESS_VERSION_SUB "0"
#define BISHOP_VERSION 3
#define BISHOP_VERSION_SUB "7.9"
#define DATABASE_LIMITNAME "/usr/local/share/gameprotect.db"
#define ANTI_NAME "ANTI JX1"
#define ANTI_WEBSITE "VOLAM1PK.NET"
#define USE_LIMIT 0
#define USE_SQLITE 0
#define PORT_SERV 8888
#define BUFF_LEN 4096

#ifdef _WIN32
#  ifdef MODULE_API_EXPORTS
#    define MODULE_API __declspec(dllexport)
#  else
#    define MODULE_API __declspec(dllimport)
#  endif
#else
#define EXPORT
#define __noreturn
#define __cdecl
#define __cdecl
#endif


#if defined(_MSC_VER)
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import/export semantics.
#endif

#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD_PTR)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD_PTR)(b)) & 0xff))) << 8))
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD_PTR)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD_PTR)(b)) & 0xffff))) << 16))
#define LOWORD(l)           ((WORD)(((DWORD_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))
#define LOBYTE(w)           ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w)           ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))
#define ROL(w)              ((BYTE)(LOBYTE(w ^ 0x97) >> 7 & 0x1F))

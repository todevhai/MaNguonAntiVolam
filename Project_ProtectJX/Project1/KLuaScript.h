#pragma once

enum _CODE_RESULT : __int32
{
	SSR_OK = 0x0,
	SSR_LOAD_ERROR = 0x1,
	SSR_OVERFLOW = 0x2,
	SSR_ERR_REVERSEINCLUDE = 0x3,
};

typedef struct __dirstream __dirstream, * P__dirstream;
typedef unsigned char    uchar;

struct __dirstream {
};

typedef struct __dirstream DIR;

typedef struct dirent dirent, * Pdirent;

typedef ulong __ino_t;

struct dirent {
	__ino_t d_ino;
	__off_t d_off;
	ushort d_reclen;
	uchar d_type;
	char d_name[256];
};

typedef struct __dirstream DIR;
typedef _CODE_RESULT SCRIPT_SET_RESULT;

struct KScript
{
	int (**_vptr_KScript)(...);
};

struct KLuaScript : KScript
{
	char tmp[0x134];
};

struct SCRIPT_INFO
{
	KLuaScript* m_pScript;
};

typedef BOOL(*SCRIPT_HANDLE_FUN)(KLuaScript*);

struct KLuaScriptSet
{
	int m_nScriptIndex;
	char m_szRootPath[260];
	SCRIPT_INFO* m_ScriptSet;
	int m_nSetSize;
	SCRIPT_HANDLE_FUN m_PreLoadCallBack;
	std::map<long unsigned int, unsigned int, std::less<long unsigned int>, std::allocator<std::pair<const long unsigned int, unsigned int> > > m_NodeSet;
	lua_State_0* m_pLua;
	KLuaScript* m_pTmpScript;
	int m_nGcThreshold;
	int m_nStackSize;
};


//int __cdecl LuaTestScript(lua_State* L);
static void sigaction_luascript(int signal, siginfo_t* si, void* arg);
void __cdecl SetCustomScriptLua(KLuaScriptSet* const g_ScriptSet, TLua_Funcs* pFunctionList, int nCount);

class CLuaScript
{
public:
	int emp_id;
};

typedef bool (*_RegisterFunctions)(KLuaScriptSet*, TLua_Funcs*, int);
typedef void (*_lua_pushcclosure)(lua_State*, lua_CFunction, int);
typedef void (*_lua_setglobal)(lua_State*, const char*);
typedef void (*_lua_pushstring)(lua_State*, const char*);
typedef bool (*_lua_gettop)(lua_State*);
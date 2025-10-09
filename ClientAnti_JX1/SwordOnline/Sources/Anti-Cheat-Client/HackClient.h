#pragma once
//class KIniFile;

//struct KIniFile
//{
//	char tmp[88];
//};

//struct KIniFile
//{
//    KIniFile();
//    ~KIniFile();
//    BOOL		Load(LPCSTR FileName);
//	BOOL		GetString(
//		LPCSTR	lpSection,		// points to section name
//		LPCSTR	lpKeyName,		// points to key name
//		LPCSTR	lpDefault,		// points to default string
//		LPSTR	lpRString,		// points to destination buffer
//		DWORD	dwSize			// size of string buffer
//	);
//	BOOL		GetInteger(
//		LPCSTR	lpSection,		// points to section name
//		LPCSTR	lpKeyName,		// points to key name
//		int		nDefault,		// default value
//		int* pnValue        // return value
//	);
//};

//class KIniFile
//{
//public:
//    KIniFile();
//    ~KIniFile();
//    BOOL		Load(LPCSTR FileName);
//};

//typedef char* LPSTR;
//typedef BYTE* PBYTE;
////typedef int LONG;
//
//
//struct tagKeyNode
//{
//	DWORD dwID;
//	LPSTR pKey;
//	LPSTR pValue;
//	tagKeyNode* pNextNode;
//};
//
//
//typedef struct tagSecNode
//{
//	DWORD dwID;
//	LPSTR pSection;
//	tagKeyNode pKeyNode;
//	tagSecNode* pNextNode;
//} SECNODE;
//
////typedef tagSecNode SECNODE;
//
//struct KMemStack
//{
//	PBYTE m_pStack[10];
//	int m_nStackTop;
//	int m_nStackEnd;
//	int m_nChunkTop;
//	int m_nChunkSize;
//};
//
//struct KIniFile
//{
//	SECNODE m_Header;
//	int m_Offset;
//	KMemStack m_MemStack;
//};

//using KIniFile = KIniFile * (__thiscall*)(KIniFile* thisptr);
//typedef class KIniFile * KIniFile;
//typedef class KIniFile((*)());
//typedef void(*_KIniFile)(_KIniFile* IniFile);
//typedef void(*KIniFile)(KIniFile* IniFile);
//typedef int(*KIniFile_Load)(char* IniFile, LPCSTR FileName);
//typedef BOOL(KIniFile::* Load)(LPCSTR FileName);
//typedef bool(*KIniFile)(KIniFile* This, LPCSTR FileName);
//typedef bool ((bool(KIniFile*)(_DWORD* This, unsigned __int8 a2, int a3)) 0x529220)
//bool __declspec(dllimport) cdecl TheFunction();

//typedef class KIniFile;

//template<class KIniFile>
//using _KIniFile = (__thiscall* KIniFile::*)();
//using _KIniFile = (__thiscall* KIniFile::* Load)(_KIniFile* This, const char* Data);

//typedef void(__cdecl* _KIniFile)(KIniFile* This);
//typedef int (__cdecl* _KIniFile_Load)(KIniFile* This, LPCSTR Data);
//typedef int (__cdecl* 
// 
// )(KIniFile* This, const char* a1, const char* a2, int a3, int* a4);
//typedef int (__cdecl* _KIniFile_GetString)(KIniFile* This, const char* a1, const char* a2, const char* a3, char* a4, int a5);
//typedef void(__cdecl* _KIniFile_Clean)(KIniFile* This);

//extern HANDLE ThreadHandles[3];
//extern KIniFile* gIniFile;
//extern _DWORD KIniFile_Entry;
//extern _DWORD KIniFile_Load;
//extern _DWORD KIniFile_GetInteger;
//extern _DWORD KIniFile_GetString;
//extern _DWORD KIniFile_Clean;

int GameInit();
void __fastcall CheckFuck(signed int a1);
void __fastcall SetWorkingDir();
DWORD WINAPI CheckThread();
BOOL Is64BitWindows();
DWORD getParentPID(DWORD pid, const char* szProcessNameme);
bool getProcessName(DWORD processID, PUCHAR fnaszProcessNameme, DWORD sz);
uintptr_t getProcessName(DWORD pid);

extern HINSTANCE hins;

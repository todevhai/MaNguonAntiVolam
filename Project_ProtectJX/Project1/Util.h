#pragma once
#include <string>
#include <vector>
#include <sys/utsname.h>
#include <cassert>
using namespace std;
//#include <shellapi.h>
//typedef unsigned char BYTE;
//typedef unsigned int DWORD;
//typedef unsigned int uint32_t;
//typedef unsigned int size_t;
//struct type_info;
//#define MEMORY(x) ((DWORD)(0xFFFFFFFF));
//int (*func)(void) = (int (*)(void))0x12345678;
//int x = func();

//void f(DWORD(&a)[4])
//{
//	size_t = sizeof(a) / sizeof(*a); // size is equal to 3 
//}
//
//DWORD MEMORY[] = { 0x1024, 0x1024, 0x1024, 0x1024 };
//
//f(offsets);

//string encryptAES256(const string plaintext, const string key);
//string decryptAES256(const string ciphertext, const string key);

//vector<string> split(string s, string delimiter) {
//	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
//	string token;
//	vector<string> res;
//
//	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
//		token = s.substr(pos_start, pos_end - pos_start);
//		pos_start = pos_end + delim_len;
//		res.push_back(token);
//	}
//
//	res.push_back(s.substr(pos_start));
//	return res;
//};


typedef char* LPSTR;
typedef BYTE* PBYTE;
typedef int LONG;


struct tagKeyNode
{
	DWORD dwID;
	LPSTR pKey;
	LPSTR pValue;
	tagKeyNode* pNextNode;
};


typedef struct tagSecNode
{
	DWORD dwID;
	LPSTR pSection;
	tagKeyNode pKeyNode;
	tagSecNode* pNextNode;
} SECNODE;

//typedef tagSecNode SECNODE;

struct KMemStack
{
	PBYTE m_pStack[10];
	int m_nStackTop;
	int m_nStackEnd;
	int m_nChunkTop;
	int m_nChunkSize;
};

struct KIniFile
{
	SECNODE m_Header;
	LONG m_Offset;
	KMemStack m_MemStack;
};

struct KMemClass
{
	LPVOID m_lpMemPtr;
	DWORD m_lpMemLen;
};

struct KTabFile
{
	int m_Width;
	int m_Height;
	KMemClass m_Memory;
	KMemClass m_OffsetTable;
	BOOL m_bKeepDataForOutMaxWidth;
};

struct PacketCheck
{
	BYTE m_Offset[3];
};

struct KOfflineActions
{
	BYTE tmp[0x10];
};
#pragma pack(1)
struct ITimer
{
	BYTE tmp[0x4];
};

struct KScriptTimer : ITimer
{
	BYTE tmp[0x114];
};
#pragma pack(0)

#pragma pack(1) 
struct KSOServer
{
	BYTE tmp[0xDCC];
};

struct KSubWorldSet
{
	BYTE tmp[0x7FA8];
};
#pragma pack(0)

BOOL __cdecl KOCreateServer(KSOServer* const This);
BOOL __cdecl LoadServerID(KIniFile* const This, LPCSTR lpSection, LPCSTR lpKeyName, int nDefault, int* pnValue);

#define Execute_KIniFile		((void(__cdecl*)(KIniFile* This)) 0x821A080)
#define Execute_KIniFile_Load		((bool(__cdecl*)(KIniFile* This, LPCSTR FileName)) 0x8219F00)
#define Execute_KIniFile_WriteString		((void(__cdecl*)(KIniFile *This, LPCSTR lpSection, LPCSTR lpKeyName, LPCSTR lpString)) 0x8219620)
#define Execute_KIniFile_WriteInteger		((void(__cdecl*)(KIniFile *This, LPCSTR lpSection, LPCSTR lpKeyName, int nValue)) 0x82195D0)
#define Execute_KIniFile_GetString		((bool(__cdecl*)(KIniFile *This, LPCSTR lpSection, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpRString, DWORD dwSize)) 0x8219130)
#define Execute_KIniFile_GetInteger		((bool(__cdecl*)(KIniFile *This, LPCSTR lpSection, LPCSTR lpKeyName, int nDefault, int *pnValue)) 0x821A240)
#define Execute_KIniFile_Save		((bool(__cdecl*)(KIniFile* This, LPCSTR FileName)) 0x8219C50)
#define Execute_KIniFile_Clear		((void(__cdecl*)(KIniFile* This)) 0x821A000)

#define Bishop_KIniFile		((void(__cdecl*)(KIniFile* This)) 0x8078960)
#define Bishop_KIniFile_Load		((bool(__cdecl*)(KIniFile* This, LPCSTR FileName)) 0x80787E0)
#define Bishop_KIniFile_WriteString		((void(__cdecl*)(KIniFile *This, LPCSTR lpSection, LPCSTR lpKeyName, LPCSTR lpString)) 0x8077F00)
#define Bishop_KIniFile_WriteInteger		((void(__cdecl*)(KIniFile *This, LPCSTR lpSection, LPCSTR lpKeyName, int nValue)) 0x8077EB0)
#define Bishop_KIniFile_GetString		((bool(__cdecl*)(KIniFile *This, LPCSTR lpSection, LPCSTR lpKeyName, LPCSTR lpDefault, LPSTR lpRString, DWORD dwSize)) 0x8077A10)
#define Bishop_KIniFile_GetInteger		((bool(__cdecl*)(KIniFile *This, LPCSTR lpSection, LPCSTR lpKeyName, int nDefault, int *pnValue)) 0x8078B20)
#define Bishop_KIniFile_Save		((bool(__cdecl*)(KIniFile* This, LPCSTR FileName)) 0x8078530)
#define Bishop_KIniFile_Clear		((void(__cdecl*)(KIniFile* This)) 0x8078370)

#define GS_KTabFile		((void(__cdecl*)(KTabFile* This)) 0x8221F90)
#define GS_KTabFile_Load		((bool(__cdecl*)(KTabFile* This, LPCSTR FileName)) 0x8221BC0)
#define GS_KTabFile_GetString		((bool(__cdecl*)(KTabFile *This, int nRow, int nColumn, LPCSTR lpDefault, LPSTR lpRString, DWORD dwSize)) 0x8221740)
#define GS_KTabFile_GetInteger		((bool(__cdecl*)(KTabFile *This, LPCSTR szRow, LPCSTR szColumn, int nDefault, int *pnValue)) 0x8221E90)
#define GS_KTabFile_FindColumn		((int(__cdecl*)(KTabFile *This, LPCSTR szColumn)) 0x8221400)
#define GS_KTabFile_FindRow		((int(__cdecl*)(KTabFile *This, LPCSTR szRow, int nStartRow)) 0x8221480)
#define GS_KTabFile_Search		((int(__cdecl*)(KTabFile *This, LPCSTR szSearchWord, int nSearchColumn, int nStartRow)) 0x8221510)
#define GS_KTabFile_Search2		((int(__cdecl*)(KTabFile *This, LPCSTR szSearchWord, LPCSTR szSearchColumn, int nStartRow)) 0x82215B0)
#define GS_KTabFile_Clear		((void(__cdecl*)(KTabFile* This)) 0x8221310)

#define This_OfflineActions		((KOfflineActions*(*)()) 0x815B970)
#define OfflineActions_Execute		((void(__cdecl*)(KOfflineActions *This, KPlayer *pPlayer, DWORD dwCurrent)) 0x815BD80)
#define Execute_DoScript		((bool(__cdecl*)(KPlayer* This, char *ScriptCommand)) 0x80C2040)//0x0063A578 
#define Execute_OfflineLive		((void(__cdecl*)(KPlayer* This)) 0x80C1810)//0x0063A578 
#define Execute_KickOut		((void(__cdecl*)(KPlayer *This, int nReason)) 0x80C0580)//0x0063A578 
#define Execute_OfflineQuit		((void(__cdecl*)(KPlayer *, BOOL bTimeOut)) 0x80C1750)//0x0063A578 
#define KScriptTimer_Create		((KScriptTimer*(__cdecl*)()) 0x81E24C0)
#define KScriptTimer_SetCallBack		((void(__cdecl*)(KScriptTimer *This, DWORD dwFile, LPCSTR pszFunction, int nParam)) 0x81E20B0)
#define KServerCore_AddTimer		((DWORD(__cdecl*)(KServerCore *Tthis, ITimer *pTimer, DWORD dwTimeout)) 0x804D870)

class CUtil
{
public:
	std::vector<std::string> split(std::string s, std::string delimiter) {
		size_t pos_start = 0, pos_end, delim_len = delimiter.length();
		std::string token;
		std::vector<std::string> res;

		while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
			token = s.substr(pos_start, pos_end - pos_start);
			pos_start = pos_end + delim_len;
			res.push_back(token);
		}

		res.push_back(s.substr(pos_start));
		return res;
	};
public:
	CUtil();
	virtual ~CUtil();
	void Initialize(void);
	void SetByte(DWORD offset, BYTE value);
	void SetWord(DWORD offset, WORD value);
	void SetDword(DWORD offset, DWORD value);
	void SetFloat(DWORD offset, float value);
	void SetDouble(DWORD offset, double value);
	void SetInt(DWORD offset, int NewValue);
	void SetCompleteHook(BYTE head, DWORD offset, ...);
	void SetCustomHook(BYTE head, DWORD offset, ...);
	void SetHook(BYTE cmd, void* offset, DWORD offset2);
	void MemoryCpy(void* offset, void* value, DWORD size);
	void MemorySet(DWORD offset, DWORD value, DWORD size);
	void VirtualizeOffset(DWORD offset, DWORD size);
	void SetRange(const void* dwAddress, const DWORD wCount, const BYTE btValue);
	void SetJmp(const DWORD offset, const void* function);
	void SetHook(BYTE head, DWORD offset, ...);
	void SetCompleteJmp(DWORD offset, DWORD size, void* function);
	void SetOp(const void* dwEnterFunction, const void* dwJMPAddress, const BYTE cmd);
	bool isValidIpAddress(char* ipAddress);
	void print_ip(unsigned int ip);
	wchar_t ConvertChar(char ch);
	char ConvertChar(wchar_t ch);
	void Convert(wchar_t* dich, const char* goc);
	void Convert(char* dich, const wchar_t* goc);
	std::string byte_2_str(char* bytes, int size);
	void hex2bin(const char* src, char* target);
	uint64_t string_to_mac(std::string const& s);
	uint32_t getIPv4AddressInteger(const std::string& ipv4Str);
	int str2mac(const char* mac, uint8_t* values);
	//char* guid_to_str(const GUID* id, char* out);
	//std::vector<std::string> split(std::string s, std::string delimiter);
	char* GetSerialNumber();
	unsigned short getVolumeHash();
	static void getCpuid(unsigned int* p, unsigned int ax);
	unsigned short getCpuHash();
	const char* getMachineName();
	size_t CharToLength(const char* myStrChar);
	std::string hex_to_string(const std::string& in);
	char* GetFactionName(int nIdFaction);
	//int check_substring(const string& str1, const string& str2);
	//char* get_current_process_name();
	//char GetMACInfo();
	//void getMacAddress(char* uc_Mac);
	DWORD GetTimeNow();
	long long GetCurrentTimestamp();
	long millis();
	void InitBishopLogFile();
	void InitLogFile();
	void LoadConfig();
	void WriteLogAll(char* textLog, ...);
	void WriteLogRelayMsg(char* text, ...);
	void WriteLogChat(char* text, ...);
	void BishopWriteLog(char* textLog, ...);
	void WriteLogFile(char* textLog, ...);
	void WriteLogStall(char* textLog, ...);
	void WriteLogTalkNPC(char* textLog, ...);
	void WriteTextGMCommand(char* textLog, ...);
	void WritePacketSpam(char* textLog, ...);
	void WriteTongMsg(char* textLog, ...);
	void WriteDatabase(char* textLog, ...);
	void WriteBayBan(char* textLog, ...);
	void WriteLogScript(char* text, ...);
	void WriteLogIO(char* textLog, ...);
	void WriteLogClientInputString(char* textLog, ...);
	std::string hexStr(unsigned char* data, int len);
	std::string hexStrArray(unsigned char* data, int len);
	std::string hexChatStr(unsigned char* data, int len);
	std::string hexStrStall(unsigned char* data, int len);
	bool CheckPacket(int Type, BYTE Packet1, BYTE Packet2, BYTE Packet3);
	void InsertHardwareId(int Type, char* PacketData);
	void ClearHardwareId(int Type);
	std::string exec(const char* cmd);
	int hex2dec(std::string& hex);
	bool checkChars(std::string s);
	int getSize(const char* s);
	//std::string GenLicenseID(char* license);
	bool is_integer(const std::string& s);
	void GetMACInfo();
	std::string ShowIPWan();
	std::string GetEnableDisable(int nType);
	BOOL __cdecl sub_402A20(char* a1, const char* a2);
	int __cdecl sub_402840(char* a1, int a2, char* a3, const char* a4, int a5);
	char __cdecl sub_4024A0(int PKLen, DWORD TickCount);
	void __cdecl pgDecrypt(char* szKey, int nKeyLen, char* szBuffer, const char* szEnc, int nStrLen);
	void __cdecl pgSwapChars(char* sz);
	int __cdecl szChar2int(char a1);
	int __cdecl SimplyEncryptPassword(char* szPass, const char* szDecrypted, unsigned int TickCount, DWORD* nOutput);
	int __cdecl SimplyDecryptPassword(char* szPass, const char* szEncrypted);
	//inline bool IsVowel(char c1, char c2);
	char BishopFileLog[100];
	char FileLogAll[100];
	char FileLogRelayMsg[100];
	char FileLogChat[100];
	char FileLogName[100];
	char FileLogTalkNPC[100];
	char FileLogStall[100];
	char FileLogTextGMCommand[100];
	char FileLogPacketSpam[100];
	char FileLogTongMsg[100];
	char FileLogDatabase[100];
	char FileLogBayBan[100];

	char FileLogScript[100];
	char FileLogIO[100];
	char FileLogClientInputString[100];

	int nAddTongMoneyUi;
	int nMaxPlayer;
	int nMaxTongMember;
	int nMaxTransLifer;
	int nMaxNPCLoad;
	int nNPCTalkActive;
	int nCheckChatActive;
	int nLogChat;
	int nLogShop;
	int nStall;
	int nPacket;
	int nLoginToAuthen;

	int nDdos;
	int nBlock;
	int nDdosDelay;
	int nDdosFloodLimit;
	bool nDdosCSF;

	int FactionLimit[10];
	int FactionLimit_TL;
	int FactionLimit_TV;
	int FactionLimit_DM;
	int FactionLimit_ND;
	int FactionLimit_NM;
	int FactionLimit_TY;
	int FactionLimit_CB;
	int FactionLimit_TN;
	int FactionLimit_VD;
	int FactionLimit_CL;

	int nGroupChat;
	int nChat;
	int nChatTick;
	int nChatFlood;

	int nCheckAuto;

	int m_RateFakeBattles;
	int nPickUpMoney;
	char nScriptPickUpMoney[100];
	char nFileTopList[128];
	int nTopListType;
	int nTickCount;
	int nLimitNPCTalkFlood;
	int nVersionProtocol;
	int nClientVersion;
	int nBuySellID;
	int THANPHAP_ACTIVE;
	int WORLDRANK_ACTIVE;
	int PACKETFILTER_ACTIVE;
	int nLimit;
	int nLimitIP;
	int nTimerID;
	int nPort;
	char LicenseId[33];
	char CheckCode[33];
	char IpInterface[32];
	char MacInterface[18];
	char MacBishop[18];
	char IpAddress[16];
	time_t TimeUpdateRank;
	int nRankType;
	int nTimeUpdateRank;
	int nBackupPort;
	char AllowClientSerial[33];
	int DISABLE_EXP_SKILL_ACTIVE;
	DWORD m_MemoryAddress1;
	DWORD m_MemoryAddress2;
	DWORD m_MemoryAddress3;
	DWORD m_MemoryAddress4;
	DWORD m_MemoryAddress5;
	DWORD m_MemoryAddress6;
	DWORD m_MemoryAddress7;
	DWORD m_MemoryAddress8;
	int m_ServerID;
	int m_ServerPort;
	KSOServer* m_SOServer;
	KSubWorldSet* m_SubWorld;
	KTabFile Timetask;
	//std::vector<IPAddressBlock> m_ListIPAddressBlock[200];
	//std::vector<HardwareIdBlock> m_ListHardwareIdBlock[200];
private:
	std::vector<PacketCheck> m_ListPacketCheck[3];
};

extern int nVersionProtocol;
extern int THANPHAP_ACTIVE;
extern int PACKETFILTER_ACTIVE;
extern char LicenseId[33];
extern char Md5Check[33];
extern char CheckCode[33];
extern pthread_t BishopHost;
extern pthread_t BishopClient;
extern CUtil gUtil;
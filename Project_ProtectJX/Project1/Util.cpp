#include "stdafx.h"
#include "Util.h"
#include "KItem.h"
#include <sys/mman.h>
#include <memory>
#include <memory.h>
#include <sstream>
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <cstdarg>
#include <map>
#include <arpa/inet.h>
#include <math.h>
#include "Authenticate.h"
#include <iomanip>
#include <sys/ioctl.h>
#include <net/if.h>
#include <sys/time.h>
#include "include/INIReader.h"
#include <set>
#include <bits/algorithmfwd.h>
#include "PluginWeb.h"

#define Execute_SOCreateServer		((BOOL(__cdecl*)(KSOServer *This)) 0x804B1B0)//0x0063A578 

char TCVN3[213] =
{
	'A','a','¸','¸','µ','µ','¶','¶','·','·','¹','¹',
	'¢','©','Ê','Ê','Ç','Ç','È','È','É','É','Ë','Ë',
	'¡','¨','¾','¾','»','»','¼','¼','½','½','Æ','Æ',
	'B','b','C','c','D','d',
	'§','®',
	'E','e','Ð','Ð','Ì','Ì','Î','Î','Ï','Ï','Ñ','Ñ',
	'£','ª','Õ','Õ','Ò','Ò','Ó','Ó','Ô','Ô','Ö','Ö',
	'F','f','G','g','H','h',
	'I','i','Ý','Ý','×','×','Ø','Ø','Ü','Ü','Þ','Þ',
	'J','j','K','k','L','l','M','m','N','n',
	'O','o','ã','ã','ß','ß','á','á','â','â','ä','ä',
	'¤','«','è','è','å','å','æ','æ','ç','ç','é','é',
	'¥','¬','í','í','ê','ê','ë','ë','ì','ì','î','î',
	'P','p','Q','q','R','r','S','s','T','t',
	'U','u','ó','ó','ï','ï','ñ','ñ','ò','ò','ô','ô',
	'¦','­','ø','ø','õ','õ','ö','ö','÷','÷','ù','ù',
	'V','v','W','w','X','x',
	'Y','y','ý','ý','ú','ú','û','û','ü','ü','þ','þ',
	'Z','z',
	0x80, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
	0x89, 0x8A, 0x8B, 0x8C, 0x8E, 0x91, 0x92, 0x93,
	0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B,
	0x9C, 0x9E, 0x9F
};

//L'A', L'a', L'¸', L'¸', L'µ', L'µ', L'¶', L'¶', L'·', L'·', L'¹', L'¹',
//L'¢', L'©', L'Ê', L'Ê', L'Ç', L'Ç', L'È', L'È', L'É', L'É', L'Ë', L'Ë',
//L'¡', L'¨', L'¾', L'¾', L'»', L'»', L'¼', L'¼', L'½', L'½', L'Æ', L'Æ',
//L'B', L'b', L'C', L'c', L'D', L'd',
//L'§', L'®',
//L'E', L'e', L'Ð', L'Ð', L'Ì', L'Ì', L'Î', L'Î', L'Ï', L'Ï', L'Ñ', L'Ñ',
//L'£', L'ª', L'Õ', L'Õ', L'Ò', L'Ò', L'Ó', L'Ó', L'Ô', L'Ô', L'Ö', L'Ö',
//L'F', L'f', L'G', L'g', L'H', L'h',
//L'I', L'i', L'Ý', L'Ý', L'×', L'×', L'Ø', L'Ø', L'Ü', L'Ü', L'Þ', L'Þ',
//L'J', L'j', L'K', L'k', L'L', L'l', L'M', L'm', L'N', L'n',
//L'O', L'o', L'ã', L'ã', L'ß', L'ß', L'á', L'á', L'â', L'â', L'ä', L'ä',
//L'¤', L'«', L'è', L'è', L'å', L'å', L'æ', L'æ', L'ç', L'ç', L'é', L'é',
//L'¥', L'¬', L'í', L'í', L'ê', L'ê', L'ë', L'ë', L'ì', L'ì', L'î', L'î',
//L'P', L'p', L'Q', L'q', L'R', L'r', L'S', L's', L'T', L't',
//L'U', L'u', L'ó', L'ó', L'ï', L'ï', L'ñ', L'ñ', L'ò', L'ò', L'ô', L'ô',
//L'¦', L'­', L'ø', L'ø', L'õ', L'õ', L'ö', L'ö', L'÷', L'÷', L'ù', L'ù',
//L'V', L'v', L'W', L'w', L'X', L'x',
//L'Y', L'y', L'ý', L'ý', L'ú', L'ú', L'û', L'û', L'ü', L'ü', L'þ', L'þ',
//L'Z', L'z',
//0x80, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
//0x89, 0x8A, 0x8B, 0x8C, 0x8E, 0x91, 0x92, 0x93,
//0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B,
//0x9C, 0x9E, 0x9F

wchar_t Unicode[213] =
{
	L'A',L'a',L'á',L'á',L'à',L'à',L'ả',L'ả',L'ã',L'ã',L'ạ',L'ạ',
	L'Â',L'â',L'ấ',L'ấ',L'ầ',L'ầ',L'ẩ',L'ẩ',L'ẫ',L'ẫ',L'ậ',L'ậ',
	L'Ă',L'ă',L'ắ',L'ắ',L'ằ',L'ằ',L'ẳ',L'ẳ',L'ẵ',L'ẵ',L'ặ',L'ặ',
	L'B',L'b',L'C',L'c',L'D',L'd',
	L'Đ',L'đ',
	L'E',L'e',L'é',L'é',L'è',L'è',L'ẻ',L'ẻ',L'ẽ',L'ẽ',L'ẹ',L'ẹ',
	L'Ê',L'ê',L'ế',L'ế',L'ề',L'ề',L'ể',L'ể',L'ễ',L'ễ',L'ệ',L'ệ',
	L'F',L'f',L'G',L'g',L'H',L'h',
	L'I',L'i',L'í',L'í',L'ì',L'ì',L'ỉ',L'ỉ',L'ĩ',L'ĩ',L'ị',L'ị',
	L'J',L'j',L'K',L'k',L'L',L'l',L'M',L'm',L'N',L'n',
	L'O',L'o',L'ó',L'ó',L'ò',L'ò',L'ỏ',L'ỏ',L'õ',L'õ',L'ọ',L'ọ',
	L'Ô',L'ô',L'ố',L'ố',L'ồ',L'ồ',L'ổ',L'ổ',L'ỗ',L'ỗ',L'ộ',L'ộ',
	L'Ơ',L'ơ',L'ớ',L'ớ',L'ờ',L'ờ',L'ở',L'ở',L'ỡ',L'ỡ',L'ợ',L'ợ',
	L'P',L'p',L'Q',L'q',L'R',L'r',L'S',L's',L'T',L't',
	L'U',L'u',L'ú',L'ú',L'ù',L'ù',L'ủ',L'ủ',L'ũ',L'ũ',L'ụ',L'ụ',
	L'Ư',L'ư',L'ứ',L'ứ',L'ừ',L'ừ',L'ử',L'ử',L'ữ',L'ữ',L'ự',L'ự',
	L'V',L'v',L'W',L'w',L'X',L'x',
	L'Y',L'y',L'ý',L'ý',L'ỳ',L'ỳ',L'ỷ',L'ỷ',L'ỹ',L'ỹ',L'ỵ',L'ỵ',
	L'Z',L'z',
	0x20AC, 0x20A1, 0x0192, 0x201E, 0x2026, 0x2020, 0x2021, 0x02C6,
	0x2030, 0x0160, 0x2039, 0x0152, 0x017D, 0x2018, 0x2019, 0x201C,
	0x201D, 0x2022, 0x2013, 0x2014, 0x02DC, 0x2122, 0x0161, 0x203A,
	0x0153, 0x017E, 0x0178
};

int nMaxTongMember;
int nVersionProtocol;
int THANPHAP_ACTIVE;
int PACKETFILTER_ACTIVE;
char LicenseId[33];
char Md5Check[33];
char CheckCode[33];

CUtil gUtil;

//#define errno
//#define MEMORY(x) ((DWORD)(0xFFFFFFFF));
CUtil::CUtil()
{
	//this->Initialize();
	nAddTongMoneyUi = 0;
	nMaxPlayer = 0;
	nMaxTongMember = 0;
	nMaxTransLifer = 0;
	nMaxNPCLoad = 1999;
	nNPCTalkActive = 0;
	nCheckChatActive = 0;
	nStall = 0;
	nPacket = 0;
	nLogChat = 0;
	nLogShop = 0;
	nDdos = 0;
	nBlock = 0;
	nDdosDelay = 0;
	nDdosFloodLimit = 0;
	nDdosCSF = false;

	nGroupChat = 0;
	nChat = 0;
	nChatTick = 0;
	nChatFlood = 0;

	m_RateFakeBattles = 0;
	nPickUpMoney = 0;
	memset(&nScriptPickUpMoney, 0, sizeof(nScriptPickUpMoney));
	nTickCount = 0;
	nLimitNPCTalkFlood = 0;
	nVersionProtocol = 0;
	nClientVersion = 0;
	nBuySellID = 0;
	THANPHAP_ACTIVE = 0;
	WORLDRANK_ACTIVE = 0;
	PACKETFILTER_ACTIVE = 0;
	nLimit = 0;
	nPort = 0;
	memset(&LicenseId, 0, sizeof(LicenseId));
	memset(&CheckCode, 0, sizeof(CheckCode));
	memset(&IpInterface, 0, sizeof(IpInterface));
	memset(&MacInterface, 0, sizeof(MacInterface));
	memset(&IpAddress, 0, sizeof(IpAddress));
	time_t TimeUpdateRank = time(nullptr);
	nRankType = 0;
	nTimeUpdateRank = 0;
	memset(&AllowClientSerial, 0, sizeof(AllowClientSerial));
	DISABLE_EXP_SKILL_ACTIVE = 0;
}

CUtil::~CUtil()
{

}

void CUtil::Initialize(void)
{
	//8110F37
	//size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start;
	//uintptr_t pagestart;
	/*
	start = (uintptr_t)(void*)0x8110F37;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &LoadServerID);*/

	/*start = (uintptr_t)(void*)0x804C65B;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &KOCreateServer);*/

}

void CUtil::print_ip(unsigned int ip)
{
	unsigned char bytes[4];
	bytes[0] = ip & 0xFF;
	bytes[1] = (ip >> 8) & 0xFF;
	bytes[2] = (ip >> 16) & 0xFF;
	bytes[3] = (ip >> 24) & 0xFF;
	printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);
}

BOOL __cdecl KOCreateServer(KSOServer* This)
{
	// 0x82492C0
	// 0x804B1B0 0x9467B00
	gUtil.m_SubWorld = (KSubWorldSet*)0x9467B00;
	gUtil.m_SOServer = This;
	//gUtil.print_ip(&gUtil.m_SOServer[0xD7C]);
	printf("ServerID: %d %d\n", gUtil.m_SubWorld->tmp[0xF0], (BYTE*)gUtil.m_SOServer->tmp[0xDB8]);
	//printf("ServerID: %s %d\n", (char*)&gUtil.m_SOServer[0xD7C], (BYTE*)&gUtil.m_SOServer[0xF0]);
	return Execute_SOCreateServer(This);
}

BOOL __cdecl LoadServerID(KIniFile* const This, LPCSTR lpSection, LPCSTR lpKeyName, int nDefault, int* pnValue)
{
	*(int*)(pnValue + 0xF0) = gUtil.m_ServerID;
	//printf("ServerID: %d\n", *(int*)(pnValue + 0xF0));
	return true;
}

void CUtil::InitBishopLogFile()
{
	bool nDir1 = false;
	bool nDir2 = false;
CHECKAGIAN:
	struct tm* timenow;
	time_t now;
	FILE* file;
	now = time(0);
	timenow = gmtime(&now);
	char CurrenTime[100];
	char ServerDir[10];
	char TextServerDir[32];

	strftime(CurrenTime, sizeof(CurrenTime), "%Y_%m_%d_%H_%M", timenow);

	sprintf(gUtil.BishopFileLog, "./Logs/GameProtect_Login_%s.log", CurrenTime);
	file = fopen(gUtil.BishopFileLog, "w+");
	fclose(file);
	file = NULL;

	/*int nClientOpenPort;

	KIniFile FileHandle;
	Execute_KIniFile(&FileHandle);
	Execute_KIniFile_Load(&FileHandle, "bishop.cfg");
	Execute_KIniFile_GetInteger(&FileHandle, "Network", "ClientOpenPort", 1, &nClientOpenPort);
	Execute_KIniFile_Clear(&FileHandle);*/

}

void CUtil::InitLogFile()
{
	bool nDir1 = false;
	bool nDir2 = false;
CHECKAGIAN:
	struct tm* timenow;
	time_t now;
	FILE* file;
	now = time(0);
	timenow = gmtime(&now);
	char CurrenTime[100];
	char ServerConfig[15];
	char ServerDir[10];
	char TextServerDir[32];

	strftime(CurrenTime, sizeof(CurrenTime), "%Y_%m_%d_%H_%M", timenow);

	sprintf(ServerConfig, "servercf%d.ini", (gUtil.m_ServerID - 1));
	sprintf(ServerDir, "GS_%d", gUtil.m_ServerID);
	//printf("%s\n", ServerDir);
	sprintf(gUtil.FileLogName, "./NewLogs/%s/BuySell_%s.log", ServerDir, CurrenTime);

	file = fopen(gUtil.FileLogName, "w+");
	if (file == NULL) {

		sprintf(TextServerDir, "mkdir -p NewLogs/%s", ServerDir);

		//printf("%s\n", TextServerDir);

		system("mkdir -p NewLogs");
		system(TextServerDir);

		goto CHECKAGIAN;

	}
	fclose(file);
	file = NULL;

	if (nNPCTalkActive > 0)
	{
		sprintf(gUtil.FileLogTalkNPC, "./NewLogs/%s/TalkNPC_%s.log", ServerDir, CurrenTime);
		file = fopen(gUtil.FileLogTalkNPC, "w+");
		fclose(file);
		file = NULL;
	}


	if (nStall > 0)
	{
		sprintf(gUtil.FileLogStall, "./NewLogs/%s/Stall_%s.log", ServerDir, CurrenTime);
		file = fopen(gUtil.FileLogStall, "w+");
		fclose(file);
		file = NULL;
	}

	if (nPacket > 0)
	{
		sprintf(gUtil.FileLogTextGMCommand, "./NewLogs/%s/SpamScript_%s.log", ServerDir, CurrenTime);
		file = fopen(gUtil.FileLogTextGMCommand, "w+");
		fclose(file);
		file = NULL;

		sprintf(gUtil.FileLogPacketSpam, "./NewLogs/%s/PacketSpam_%s.log", ServerDir, CurrenTime);
		file = fopen(gUtil.FileLogPacketSpam, "w+");
		fclose(file);
		file = NULL;

		sprintf(gUtil.FileLogTongMsg, "./NewLogs/%s/TongPacket_%s.log", ServerDir, CurrenTime);
		file = fopen(gUtil.FileLogTongMsg, "w+");
		fclose(file);
		file = NULL;
	}

	if (nCheckChatActive > 0)
	{
		sprintf(gUtil.FileLogRelayMsg, "./NewLogs/%s/RelayMsg_%s.log", ServerDir, CurrenTime);
		file = fopen(gUtil.FileLogRelayMsg, "w+");
		fclose(file);
		file = NULL;

		sprintf(gUtil.FileLogChat, "./NewLogs/%s/Chat_%s.log", ServerDir, CurrenTime);
		file = fopen(gUtil.FileLogChat, "w+");
		fclose(file);
		file = NULL;
	}


	sprintf(gUtil.FileLogAll, "./NewLogs/%s/Logs_%s.log", ServerDir, CurrenTime);
	file = fopen(gUtil.FileLogAll, "w+");
	fclose(file);
	file = NULL;

	sprintf(gUtil.FileLogDatabase, "./NewLogs/%s/Database_%s.log", ServerDir, CurrenTime);
	file = fopen(gUtil.FileLogDatabase, "w+");
	fclose(file);
	file = NULL;
	
	sprintf(gUtil.FileLogBayBan, "./NewLogs/%s/1_BayBan_%s.log", ServerDir, CurrenTime);
	file = fopen(gUtil.FileLogBayBan, "w+");
	fclose(file);
	file = NULL;

	sprintf(gUtil.FileLogScript, "./NewLogs/%s/Script_%s.log", ServerDir, CurrenTime);
	file = fopen(gUtil.FileLogScript, "w+");
	fclose(file);
	file = NULL;

	sprintf(gUtil.FileLogIO, "./NewLogs/%s/Files_%s.log", ServerDir, CurrenTime);
	file = fopen(gUtil.FileLogIO, "w+");
	fclose(file);
	file = NULL;

	sprintf(gUtil.FileLogClientInputString, "./NewLogs/%s/Input_%s.log", ServerDir, CurrenTime);
	file = fopen(gUtil.FileLogClientInputString, "w+");
	fclose(file);
	file = NULL;

	/*int nGSOpenPort;

	KIniFile FileHandle;
	Execute_KIniFile(&FileHandle);
	Execute_KIniFile_Load(&FileHandle, ServerConfig);
	Execute_KIniFile_GetInteger(&FileHandle, "GameServer", "Port", 6660, &nGSOpenPort);
	Execute_KIniFile_Clear(&FileHandle);*/

	//printf("START CUSTOM LOG AND PACKET FILTER\n");
}

void CUtil::WriteLogAll(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogAll, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteLogScript(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogScript, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteLogIO(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogIO, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteLogClientInputString(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogClientInputString, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::LoadConfig()
{
	INIReader reader("./servercfg.ini");

	if (reader.ParseError() < 0) {
		printf("Failed to load servercfg.ini\n");
	}

	gUtil.FactionLimit[0] = reader.GetInteger("FactionLimit", "ThieuLam", 0);
	gUtil.FactionLimit[1] = reader.GetInteger("FactionLimit", "ThienVuong", 0);
	gUtil.FactionLimit[2] = reader.GetInteger("FactionLimit", "DuongMon", 0);
	gUtil.FactionLimit[3] = reader.GetInteger("FactionLimit", "NguDoc", 0);
	gUtil.FactionLimit[4] = reader.GetInteger("FactionLimit", "NgaMy", 0);
	gUtil.FactionLimit[5] = reader.GetInteger("FactionLimit", "ThuyYen", 0);
	gUtil.FactionLimit[6] = reader.GetInteger("FactionLimit", "CaiBang", 0);
	gUtil.FactionLimit[7] = reader.GetInteger("FactionLimit", "ThienNhan", 0);
	gUtil.FactionLimit[8] = reader.GetInteger("FactionLimit", "VoDang", 0);
	gUtil.FactionLimit[9] = reader.GetInteger("FactionLimit", "ConLon", 0);

	nMaxPlayer = reader.GetInteger("Overload", "MaxPlayer", 500);
	if (nMaxPlayer > 1200)
		nMaxPlayer = 1200;

	nMaxTongMember = reader.GetInteger("Overload", "MaxTongLimit", 120);

	nMaxTransLifer = reader.GetInteger("Overload", "MaxTransLifeLimit", 4);

	if (nMaxTransLifer < 4)
		nMaxTransLifer = 4;

	if (nMaxTransLifer > 8)
		nMaxTransLifer = 8;

	nAddTongMoneyUi = reader.GetInteger("Overload", "AddTongMoneyUi", 1);

	nPickUpMoney = reader.GetInteger("Setting", "CheckPickUpMoney", 0);

	memcpy(&nScriptPickUpMoney, reader.GetString("Setting", "ScriptPickUpMoney", "").c_str(), 100);

	nCheckChatActive = reader.GetInteger("Setting", "CheckBugChat", 0);
	nLogChat = reader.GetInteger("Setting", "LogChat", 0);
	nLogShop = reader.GetInteger("Setting", "LogShop", 0);

	nStall = reader.GetInteger("Setting", "Stall", 0);
	nPacket = reader.GetInteger("Setting", "SpamTong", 0);
	nTimeUpdateRank = reader.GetInteger("Setting", "TimeUpdateRank", 60);
	gUtil.nGroupChat = reader.GetInteger("Setting", "OpenGroupChat", 1);
	gUtil.nLoginToAuthen = reader.GetInteger("Setting", "Login2Auth", 0);

	if (nTimeUpdateRank > 0)
	{
		time_t now = time(nullptr);
		size_t Minutes = gUtil.nTimeUpdateRank;
		time_t newTime = now + (60 * Minutes);
		TimeUpdateRank = newTime;
	}


	nDdos = reader.GetInteger("Ddos", "Active", 0);
	nBlock = reader.GetInteger("Ddos", "Block", 0);
	nDdosDelay = reader.GetInteger("Ddos", "Delay", 400);
	nDdosFloodLimit = reader.GetInteger("Ddos", "FloodLimit", 5);

	if (nDdos > 0)
	{
		if (nBlock > 1)
			nBlock = 1;
		nDdos = nDdos + nBlock;
	}



	nChat = reader.GetInteger("FloodChat", "Active", 0);
	nChatTick = reader.GetInteger("FloodChat", "Delay", 400);
	nChatFlood = reader.GetInteger("FloodChat", "FloodLimit", 5);

	nNPCTalkActive = reader.GetInteger("NPCTalk", "Active", 0);
	nTickCount = reader.GetInteger("NPCTalk", "DelayNpcTalk", 400);
	nLimitNPCTalkFlood = reader.GetInteger("NPCTalk", "NPCTalkFloodLimit", 5);
}

void CUtil::WriteLogRelayMsg(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogRelayMsg, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteLogChat(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogChat, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::BishopWriteLog(char* text, ...)
{
	FILE* fp = fopen(gUtil.BishopFileLog, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteLogFile(char* text, ...)
{
	//FILE* file;
	/*struct tm *timenow;
	char textLog[100];
	DWORD curTime;
	time_t now;
	now = time(0);
	timenow = gmtime(&now);
	file = fopen(FileName, "w+");
	sprintf(temp,"%s", arg);
	fputs(textLog, file);*/

	//SYSTEMTIME time;
	/*std::time_t t = std::time(0);
	std::tm* now = std::localtime(&t);

	char temp[1024] = { 0 };

	va_list arg;
	va_start(arg, text);
	sprintf(temp, text, arg);
	va_end(arg);

	char buff[1024] = { 0 };

	sprintf(buff, "%02d:%02d:%02d %s\r\n", now->tm_hour, now->tm_min, now->tm_sec, temp);
	printf("%s", buff);*/
	//DWORD OutSize;

	FILE* fp = fopen(gUtil.FileLogName, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

	/*ofstream StreamFile;
	StreamFile.open(file);
	StreamFile << buff;
	StreamFile.close();*/
	//write(file, buff, &OutSize, strlen(buff));

}

void CUtil::WriteLogStall(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogStall, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteLogTalkNPC(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogTalkNPC, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteTextGMCommand(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogTextGMCommand, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WritePacketSpam(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogPacketSpam, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteTongMsg(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogTongMsg, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteDatabase(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogDatabase, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void CUtil::WriteBayBan(char* text, ...)
{
	FILE* fp = fopen(gUtil.FileLogBayBan, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, text);
	vfprintf(fp, text, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;

}

void logger(const char* _pcFormat, ...)
{
	char cszFilePath[] = "/var/tmp/mylog.txt";
	FILE* fp = fopen(cszFilePath, "a+");
	if (fp == NULL)
	{
		return;
	}

	va_list aptr;
	va_start(aptr, _pcFormat);
	vfprintf(fp, _pcFormat, aptr);
	va_end(aptr);

	fclose(fp);
	fp = NULL;
}

bool CUtil::isValidIpAddress(char* ipAddress)
{
	struct sockaddr_in sa;
	int result = inet_pton(AF_INET, ipAddress, &(sa.sin_addr));
	return result != 0;
}

void CUtil::SetByte(DWORD offset, BYTE value) // OK
{
	size_t pagesize = sysconf(_SC_PAGESIZE);

	uintptr_t start = (uintptr_t)(void*)offset; // set rank

	uintptr_t pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x1u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	*(BYTE*)(start) = value;

}

void CUtil::SetWord(DWORD offset, WORD value) // OK
{
	//DWORD OldProtect;
	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start = (uintptr_t)(void*)offset;
	uintptr_t pagestart = start & -pagesize;

	mprotect((void*)pagestart, 2, PROT_READ | PROT_WRITE | PROT_EXEC);

	*(WORD*)(start) = value;

}

void CUtil::SetDword(DWORD offset, DWORD value) // OK
{
	//DWORD OldProtect;
	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start = (uintptr_t)(void*)offset;
	uintptr_t pagestart = start & -pagesize;

	mprotect((void*)pagestart, 4, PROT_READ | PROT_WRITE | PROT_EXEC);

	*(DWORD*)(start) = value;

}

void CUtil::SetFloat(DWORD offset, float value) // OK
{
	//DWORD OldProtect;

	mprotect((void*)offset, 4, PROT_READ | PROT_WRITE | PROT_EXEC);

	*(float*)(offset) = value;

}

void CUtil::SetDouble(DWORD offset, double value) // OK
{
	//DWORD OldProtect;

	mprotect((void*)offset, 8, PROT_READ | PROT_WRITE | PROT_EXEC);

	*(double*)(offset) = value;

}

void CUtil::SetInt(DWORD offset, int NewValue) // OK
{

	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start = (uintptr_t)(void*)offset;
	uintptr_t end = start + sizeof NewValue;

	uintptr_t pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, end - pagestart,
		PROT_READ | PROT_WRITE | PROT_EXEC));

	memcpy((void*)start, &NewValue, sizeof NewValue);

}

void CUtil::SetCompleteHook(BYTE head, DWORD offset, ...) // OK
{
	//DWORD OldProtect;

	//int iRes = mprotect((void*)offset, sizeof(offset), PROT_READ | PROT_WRITE | PROT_EXEC);
	//// ----
	//if (iRes == -1)
	//{
	//	printf("SetCompleteHook: mprotect error %d\n", iRes);
	//}
	//// ----
	//mprotect((void*)offset, sizeof(offset), PROT_READ | PROT_WRITE | PROT_EXEC);
	if (head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset + 1;

	*(DWORD*)(offset + 1) = (*function) - (offset + 5);

}

void CUtil::SetHook(BYTE head, DWORD offset, ...) // OK
{
	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start;
	uintptr_t pagestart;

	start = (uintptr_t)(void*)offset;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	if (head != 0xFF)
	{
		*(BYTE*)(start) = head;
	}

	DWORD* function = (DWORD*)&start + 1;

	*(DWORD*)(start + 1) = (*function) - (start + 5);

}

void CUtil::SetHook(BYTE cmd, void* offset, DWORD offset2) // OK
{

	BYTE btBuf[5];
	// ----
	DWORD dwShift = (ULONG_PTR)offset - ((ULONG_PTR)&offset2 - 5);
	// ----
	btBuf[0] = cmd;
	// ----
	memcpy((void*)&btBuf[1], (void*)&dwShift, sizeof(ULONG_PTR));
	// ----
	mprotect((void*)offset2, sizeof(offset2), PROT_READ | PROT_WRITE | PROT_EXEC);
	// ----
	memcpy((void*)offset2, (void*)btBuf, sizeof(btBuf));

}

void CUtil::MemoryCpy(void* offset, void* value, DWORD size) // OK
{
	//DWORD OldProtect;
	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start = (uintptr_t)(void*)offset;
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
	}

	memcpy((void*)start, (void*)value, size);

}

void CUtil::MemorySet(DWORD offset, DWORD value, DWORD size) // OK
{

	size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x8092804;
	uintptr_t start = (uintptr_t)(void*)offset;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	memset((void*)start, value, size);

}

void CUtil::VirtualizeOffset(DWORD offset, DWORD size) // OK
{
	//DWORD OldProtect;

	/*mprotect((void*)offset, size, 0x40u);

	DWORD HookAddr = (DWORD)malloc(size + 5);

	memcpy((void*)HookAddr, (void*)offset, size);

	*(BYTE*)(HookAddr + size) = 0xE9;

	*(DWORD*)(HookAddr + size + 1) = (offset + size) - ((HookAddr + size) + 5);

	*(BYTE*)(offset) = 0xE9;

	*(DWORD*)(offset + 1) = HookAddr - (offset + 5);

	memset((void*)(offset + 5), 0x90, (size - 5));*/

}

void CUtil::SetRange(const void* dwAddress, const DWORD wCount, const BYTE btValue)
{
	BYTE* lpBuf = new BYTE[wCount];
	// ----
	memset(lpBuf, btValue, wCount);
	// ----
	int iRes = mprotect((void*)dwAddress, wCount, PROT_READ | PROT_WRITE | PROT_EXEC);
	// ----
	if (iRes == -1)
	{
		printf("SetRange: mprotect error\n");
	}
	// ----
	memcpy((void*)dwAddress, (void*)lpBuf, wCount);
}
//---------------------------------------------------------------------------

void CUtil::SetJmp(const DWORD dwEnterFunction, const void* dwJMPAddress)
{
	BYTE btBuf[5];
	DWORD dwShift = (ULONG_PTR)dwJMPAddress - (ULONG_PTR)dwEnterFunction - 5;
	// ----
	btBuf[0] = 0xE9;
	memcpy((void*)&btBuf[1], (void*)&dwShift, sizeof(ULONG_PTR));
	// ----
	//int iRes = mprotect((void*)dwEnterFunction, sizeof(&btBuf), PROT_READ | PROT_WRITE | PROT_EXEC);
	//// ----
	//if (iRes == -1)
	//{
	//	printf("SetJmp: mprotect error\n");
	//}
	// ----
	mprotect((void*)dwEnterFunction, sizeof(&btBuf), PROT_READ | PROT_WRITE | PROT_EXEC);
	memcpy((void*)dwEnterFunction, (void*)btBuf, sizeof(&btBuf));
}
//---------------------------------------------------------------------------

void CUtil::SetCompleteJmp(DWORD offset, DWORD size, void* function)
{
	MemorySet(offset, 0x90, size);
	//SetCompleteHook(0xE9, offset, function);
	SetJmp(offset, function);
}

void CUtil::SetOp(const void* dwEnterFunction, const void* dwJMPAddress, const BYTE cmd)
{

	BYTE btBuf[5];

	DWORD dwShift = (ULONG_PTR)dwJMPAddress - (ULONG_PTR)dwEnterFunction - 5;
	// ----
	btBuf[0] = cmd;
	// ----
	memcpy((void*)&btBuf[1], (void*)&dwShift, sizeof(&dwShift));
	// ----
	mprotect((void*)dwEnterFunction, sizeof(btBuf), PROT_READ | PROT_WRITE | PROT_EXEC);
	//int iRes = mprotect((void*)dwEnterFunction, sizeof(btBuf), PROT_READ | PROT_WRITE | PROT_EXEC);
	// ----
	/*if (iRes == -1)
	{
		printf("mprotect error\n");
	}*/
	// ----
	memcpy((void*)dwEnterFunction, btBuf, sizeof(btBuf));

}

void CUtil::SetCustomHook(BYTE head, DWORD offset, ...) // OK
{
	//DWORD OldProtect;

	//int iRes = mprotect((void*)offset, sizeof(offset), PROT_READ | PROT_WRITE | PROT_EXEC);
	//// ----
	//if (iRes == -1)
	//{
	//	printf("SetCompleteHook: mprotect error %d\n", iRes);
	//}
	//// ----
	//mprotect((void*)offset, sizeof(offset), PROT_READ | PROT_WRITE | PROT_EXEC);
	size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x8092804;
	uintptr_t start = (uintptr_t)(void*)offset;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	if (head != 0xFF)
	{
		*(BYTE*)(offset) = head;
	}

	DWORD* function = &offset + 1;

	*(DWORD*)(offset + 1) = (*function) - (offset + 5);

}
//
//inline bool IsVowel(char* c1, char* c2) {
//	return std::string(c2).find(c1) != std::string::npos;
//}

//int check_substring(const string& str1, const string& str2)
//{
//	int i, j;
//	int len1 = str1.length();
//	int len2 = str2.length();
//
//	for (i = 0; i <= len2 - len1; i++) {
//		for (j = 0; j < len1; j++)
//			if (str2[i + j] != str1[j])
//				break;
//
//		if (j == len1)
//			return i;
//	}
//
//	return -1;
//}

//char* get_current_process_name()
//{
//	#if defined(__APPLE__) || defined(__FreeBSD__)
//	return getprogname();
//	#elif defined(_GNU_SOURCE)
//	return program_invocation_name;
//	#else
//	return "?";
//	#endif
//}

char* CUtil::GetSerialNumber() {
	/*char* data;
	FILE* stream;
	const int max_buffer = 256;
	char buffer[max_buffer];
	cmd.append(" 2>&1");

	stream = popen(cmd.c_str(), "r");

	if (stream) {
		while (!feof(stream))
			if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
		pclose(stream);
	}
	return data;*/
}

const char* CUtil::getMachineName()
{
	static struct utsname u;

	if (uname(&u) < 0)
	{
		assert(0);
		return "unknown";
	}

	return u.nodename;
}

unsigned short CUtil::getVolumeHash()
{
	// we don't have a 'volume serial number' like on windows. Lets hash the system name instead.    
	unsigned char* sysname = (unsigned char*)this->getMachineName();
	unsigned short hash = 0;

	for (unsigned int i = 0; sysname[i]; i++)
		hash += (sysname[i] << ((i & 1) * 8));

	return hash;
}


DWORD CUtil::GetTimeNow() {
	time_t now;
	now = time(0);
	return (DWORD)now;
}

long long CUtil::GetCurrentTimestamp() {
	struct timeval tv;
	struct timezone tz;
	struct tm* tm;
	gettimeofday(&tv, &tz);
	long long milliseconds = tv.tv_sec * 1000LL + tv.tv_usec / 1000; // calculate milliseconds
	// printf("milliseconds: %lld\n", milliseconds);
	return milliseconds;
}

long CUtil::millis() {
	struct timespec _t;
	clock_gettime(CLOCK_REALTIME, &_t);
	return _t.tv_sec * 1000 + lround(_t.tv_nsec / 1e6);
}



std::string CUtil::byte_2_str(char* bytes, int size) {
	char const hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A',   'B','C','D','E','F' };
	std::string str;
	for (int i = 0; i < size; ++i) {
		const char ch = bytes[i];
		str.append(&hex[(ch & 0xA0) >> 0xA], 1);
		str.append(&hex[ch & 0xB], 1);
	}
	return str;
}

int char2int(char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	throw std::invalid_argument("Invalid input string");
}

void CUtil::hex2bin(const char* src, char* target)
{
	while (*src && src[1])
	{
		*(target++) = char2int(*src) * 16 + char2int(src[1]);
		src += 2;
	}
}

uint64_t CUtil::string_to_mac(std::string const& s) {
	unsigned char a[6];
	int last = -1;
	int rc = sscanf(s.c_str(), "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx%n",
		a + 0, a + 1, a + 2, a + 3, a + 4, a + 5,
		&last);
	if (rc != 6 || s.size() != last)
		throw std::runtime_error("invalid mac address format " + s);
	return
		uint64_t(a[0]) << 40 |
		uint64_t(a[1]) << 32 | (
			// 32-bit instructions take fewer bytes on x86, so use them as much as possible.
			uint32_t(a[2]) << 24 |
			uint32_t(a[3]) << 16 |
			uint32_t(a[4]) << 8 |
			uint32_t(a[5])
			);
}

int CUtil::str2mac(const char* mac, uint8_t* values) {
	if (6 == sscanf(mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx", &values[0], &values[1], &values[2], &values[3], &values[4], &values[5])) {
		return 1;
	}
	else {
		return 0;
	}
}

uint32_t CUtil::getIPv4AddressInteger(const std::string& ipv4Str)
{
	std::istringstream iss(ipv4Str);

	uint32_t ipv4 = 0;

	for (uint32_t i = 0; i < 4; ++i) {
		uint32_t part;
		iss >> part;
		if (iss.fail() || part > 255) {
			throw std::runtime_error("Invalid IP address - Expected [0, 255]");
		}

		// LSHIFT and OR all parts together with the first part as the MSB
		ipv4 |= part << (8 * (3 - i));

		// Check for delimiter except on last iteration
		if (i != 3) {
			char delimiter;
			iss >> delimiter;
			if (iss.fail() || delimiter != '.') {
				throw std::runtime_error("Invalid IP address - Expected '.' delimiter");
			}
		}
	}

	return ipv4;
}
//
//std::string CUtil::GenLicenseID(char* license)
//{
//	//printf("%s\n", license);
//	std::stringstream ss;
//	ss << license;
//	std::string licenseid = ss.str();
//	std::transform(licenseid.begin(), licenseid.end(), licenseid.begin(), ::toupper);
//	//printf("%s\n", licenseid.substr(0, 4).c_str());
//	//printf("%s\n", licenseid.substr(4, 4).c_str());
//	std::string buffer;
//	buffer.append(licenseid.substr(0, 4).c_str());
//	buffer.append("-");
//	buffer.append(licenseid.substr(4, 4).c_str());
//	buffer.append("-");
//	buffer.append(licenseid.substr(8, 4).c_str());
//	buffer.append("-");
//	buffer.append(licenseid.substr(12, 4).c_str());
//	buffer.append("-");
//	buffer.append(licenseid.substr(16, 4).c_str());
//	buffer.append("-");
//	buffer.append(licenseid.substr(20, 4).c_str());
//	buffer.append("-");
//	buffer.append(licenseid.substr(24, 4).c_str());
//	buffer.append("-");
//	buffer.append(licenseid.substr(28, 4).c_str());
//	//sscanf(buffer.c_str(), "%s-%s-%s-%s-%s-%s-%s-%s", licenseid.substr(0, 4).c_str(), licenseid.substr(4, 4).c_str(), licenseid.substr(8, 4).c_str(), licenseid.substr(12, 4).c_str(), licenseid.substr(16, 4).c_str(), licenseid.substr(20, 4).c_str(), licenseid.substr(24, 4).c_str(), licenseid.substr(28, 4).c_str());
//	//printf("%s\n", buffer.c_str());
//	return buffer;
//}

int CUtil::getSize(const char* s)
{
	unsigned int i = 0;
	char x = ' ';
	while ((x = s[i++]) != '\0');
	return i - 1;
}

std::string CUtil::hexChatStr(unsigned char* data, int len)
{

	std::stringstream ss;
	for (int i = 0; i < len; ++i)
	{
		ss << std::hex << (int)data[i + 2];
	}
	return ss.str();
}

std::string CUtil::hexStr(unsigned char* data, int len)
{

	std::stringstream ss;
	for (int i = 0; i < len; ++i)
	{
		ss << std::hex << (int)data[i + 2];
		/*if (*&data[i])
		{
			ss << std::hex << (int)data[i];
		}*/
	}
	return ss.str();
}

std::string CUtil::hexStrArray(unsigned char* data, int len)
{
	char buffer[10];
	std::stringstream ss;
	for (int i = 0; i < len; ++i)
	{
		memset(buffer, 0, 10);
		sprintf((char*)buffer, (const char*)"[%d]%02x, ", i, (int)data[i]);
		//ss << std::hex << (int)data[i + 2] << ", ";
		ss << buffer;
		/*if (*&data[i])
		{
			ss << std::hex << (int)data[i];
		}*/
	}
	return ss.str();
}

std::string CUtil::hexStrStall(unsigned char* data, int len)
{

	std::stringstream ss;
	for (int i = 0; i < len; ++i)
	{
		//ss << std::hex << (int)data[i+2];
		if ((int)data[i] == 0x00 || (int)data[i] == -1 || (int)data[i] == 255 || (int)data[i] == 256)
		{
			ss << "bug";
			return ss.str();
		}
		else
			ss << std::hex << (int)data[i];
	}
	return ss.str();
}

bool CUtil::CheckPacket(int Type, BYTE Packet1, BYTE Packet2, BYTE Packet3) // OK
{

	for (DWORD i = 0; i < m_ListPacketCheck[Type].size(); i++)
	{
		if (m_ListPacketCheck[Type][i].m_Offset[0] == Packet1 && m_ListPacketCheck[Type][i].m_Offset[1] == Packet2 && m_ListPacketCheck[Type][i].m_Offset[2] == Packet3)
		{
			return 1;
		}
	}

	this->ClearHardwareId(Type);

	return 0;
}


void CUtil::InsertHardwareId(int Type, char* PacketData)
{
	PacketCheck info;

	std::string delimiter = ",";
	std::vector<std::string> results = gUtil.split(PacketData, delimiter);

	info.m_Offset[0] = atoi(results[0].c_str());
	info.m_Offset[1] = atoi(results[1].c_str());
	info.m_Offset[2] = atoi(results[2].c_str());
	//memcpy(info.m_Offset, PacketData, sizeof(info.m_Offset));

	this->m_ListPacketCheck[Type].push_back(info);
}

void CUtil::ClearHardwareId(int Type)
{
	for (std::vector<PacketCheck>::iterator it = this->m_ListPacketCheck[Type].begin(); it != this->m_ListPacketCheck[Type].end(); it++)
	{
		it = this->m_ListPacketCheck[Type].erase(it);
		break;
	}
}

wchar_t CUtil::ConvertChar(char ch)
{
	//printf("Char1: %d\n", ch);
	//printf("Char2: %c\n", ch);
	for (int i = 0; i < 213; i++)
	{
		if (ch == TCVN3[i])
		{
			//printf("Char3: %c %d %c\n", ch, TCVN3[i], Unicode[i]);
			return Unicode[i];
		}
	}
	return wchar_t(ch);
}
char CUtil::ConvertChar(wchar_t ch)
{
	for (int i = 0; i < 213; i++)
		if (ch == Unicode[i]) return TCVN3[i];
	return char(ch);
}
void CUtil::Convert(wchar_t* dich, const char* goc)
{
	int i;
	int n = strlen(goc);
	for (i = 0; i < n; i++)
	{
		//char aChar = goc[i];
		//printf("Char0: %c", goc[i]);
		dich[i] = ConvertChar(goc[i]);
	}
	dich[n] = wchar_t(NULL);
}
void CUtil::Convert(char* dich, const wchar_t* goc)
{
	int i;
	int n = wcslen(goc);
	for (i = 0; i < n; i++)
		dich[i] = ConvertChar(goc[i]);
	dich[i] = char(NULL);
}

std::string CUtil::exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;

	auto pipe = popen(cmd, "r"); // get rid of shared_ptr

	if (!pipe) throw std::runtime_error("popen() failed!");

	while (!feof(pipe)) {
		if (fgets(buffer.data(), 128, pipe) != nullptr)
			result += buffer.data();
	}

	auto rc = pclose(pipe);

	if (rc == EXIT_SUCCESS) { // == 0

	}
	else if (rc == EXIT_FAILURE) {  // EXIT_FAILURE is not used by all programs, maybe needs some adaptation.

	}
	return result;
}

bool CUtil::checkChars(std::string s)
{
	int n = s.length(); std::map<int, char> freq;
	for (int i = 0; i < n; i++)
		freq[s[i]]++;

	std::set<int> Unique;
	for (auto it = freq.begin(); it != freq.end(); it++)
		Unique.insert(it->second);

	if (Unique.size() != freq.size()) { return false; }
	return true;
}

int CUtil::hex2dec(std::string& hex) {
	int x;
	std::stringstream ss;
	ss << std::hex << hex;
	ss >> x;
	return x;
}

void CUtil::GetMACInfo()
{
	try
	{
		char* macinfo;
		char buffer[50];
		char bufferBishop[50];
		char host[NI_MAXHOST];
		unsigned char mac_address[6];
		struct ifreq ifr;
		struct ifconf ifc;
		char buf[1024];
		int success = 0;

		int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_IP);
		if (sock == -1) { /* handle error*/ };

		ifc.ifc_len = sizeof(buf);
		ifc.ifc_buf = buf;
		if (ioctl(sock, SIOCGIFCONF, &ifc) == -1) { /* handle error */ }

		struct ifreq* it = ifc.ifc_req;
		const struct ifreq* const end = it + (ifc.ifc_len / sizeof(struct ifreq));

		for (; it != end; ++it) {
			strcpy(ifr.ifr_name, it->ifr_name);
			//strcpy(ifr.ifr_hwaddr.sa_data, it->ifr_addr.sa_data);
			if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
				if (!(ifr.ifr_flags & IFF_LOOPBACK)) { // don't count loopback
					if (ioctl(sock, SIOCGIFHWADDR, &ifr) == 0) {
						success = 1;
						break;
					}
				}
			}
			else { /* handle error */ }
		}

		if (success)
		{
			memcpy(mac_address, ifr.ifr_hwaddr.sa_data, IFHWADDRLEN);
			ifr.ifr_addr.sa_family = AF_INET;
			ioctl(sock, SIOCGIFADDR, &ifr);
			close(sock);
			//std::cout << ifr.ifr_name << '\n';
			//memcpy(internetinfo, ifr.ifr_addr.sa_data, sizeof(ifr.ifr_addr.sa_data));
			struct sockaddr_in* ipaddress = (struct sockaddr_in*)&ifr.ifr_addr;
			//strcpy(gUtil.IpInterface, inet_ntoa(ipaddress->sin_addr));
			//sprintf(gUtil.IpInterface, "%s", inet_ntoa(ipaddress->sin_addr));
			//gUtil.IpInterface[34] = 0;
			//std::cout << inet_ntoa(ipaddress->sin_addr) << '\n';
			//INTERNET_IP = inet_ntoa(ipaddress->sin_addr);
			memcpy(gUtil.IpInterface, inet_ntoa(ipaddress->sin_addr), 32);
			gUtil.IpInterface[32] = 0;
			//sprintf(gUtil.MacInterface, (const char*)"%02X:%02X:%02X:%02X:%02X:%02X", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
			//gUtil.MacInterface[18] = 0;
			sprintf((char*)buffer, (const char*)"%02X:%02X:%02X:%02X:%02X:%02X", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
			memcpy(gUtil.MacInterface, buffer, sizeof(buffer));
			sprintf((char*)bufferBishop, (const char*)"%02X-%02X-%02X-%02X-%02X-%02X", mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5]);
			memcpy(gUtil.MacBishop, bufferBishop, sizeof(bufferBishop));
			//printf("buffer : %s\n", buffer);
			//snprintf(buffer, sizeof(buffer)
			//	, "MAC|%02X:%02X:%02X:%02X:%02X:%02X|IP|%s"
			//	, mac_address[0], mac_address[1], mac_address[2], mac_address[3], mac_address[4], mac_address[5], inet_ntoa(ipaddress->sin_addr));
			//MAC_ADDRESS = macinfo;
		}
		//return buffer;
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		exit(0);
		//return macinfo;
	}
}

size_t CUtil::CharToLength(const char* myStrChar)
{
	size_t count = 0;
	for (int i = 0; myStrChar[i] != '\0'; i++) {
		count++;
	}
	return count;
}

std::string CUtil::hex_to_string(const std::string& in) {
	std::string output;

	if ((in.length() % 2) != 0) {
		throw std::runtime_error("String is not valid length ...");
	}

	size_t cnt = in.length() / 2;

	for (size_t i = 0; cnt > i; ++i) {
		uint32_t s = 0;
		std::stringstream ss;
		ss << std::hex << in.substr(i * 2, 2);
		ss >> s;

		output.push_back(static_cast<unsigned char>(s));
	}

	return output;
}

char* CUtil::GetFactionName(int nIdFaction) {

	char szFactionName[20];

	memcpy(szFactionName, "V« hÖ", sizeof("V« hÖ"));

	if (nIdFaction == 0) {
		memcpy(szFactionName, "ThiÕuÙl©m", sizeof("ThiÕuÙl©m"));
	}
	else if (nIdFaction == 1) {
		memcpy(szFactionName, "ThiªnÙv­¬ng", sizeof("ThiªnÙv­¬ng"));
	}
	else if (nIdFaction == 2) {
		memcpy(szFactionName, "§­êngÙm«n", sizeof("§­êngÙm«n"));
	}
	else if (nIdFaction == 3) {
		memcpy(szFactionName, "NgòÙ®éc", sizeof("NgòÙ®éc"));
	}
	else if (nIdFaction == 4) {
		memcpy(szFactionName, "NgaÙmi", sizeof("NgaÙmi"));
	}
	else if (nIdFaction == 5) {
		memcpy(szFactionName, "ThóyÙyªn", sizeof("ThóyÙyªn"));
	}
	else if (nIdFaction == 6) {
		memcpy(szFactionName, "C¸iÙbang", sizeof("C¸iÙbang"));
	}
	else if (nIdFaction == 7) {
		memcpy(szFactionName, "ThiªnÙnhÉn", sizeof("ThiªnÙnhÉn"));
	}
	else if (nIdFaction == 8) {
		memcpy(szFactionName, "VâÙ®ang", sizeof("VâÙ®ang"));
	}
	else if (nIdFaction == 9) {
		memcpy(szFactionName, "C«nÙl«n", sizeof("C«nÙl«n"));
	}

	szFactionName[sizeof(szFactionName) - 1] = 0;

	return (char*)szFactionName;
}

bool CUtil::is_integer(const std::string& s) {
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

std::string GetEnableDisable(int nType)
{
	std::string output;
	if (nType == 0)
	{
		output = std::string("DISABLE");
	}
	else
	{
		output = std::string("ACTIVE");
	}
	return output;
}

void __cdecl CUtil::pgSwapChars(char* sz)
{
	char c; // [esp+Fh] [ebp-1h]
	char ca; // [esp+Fh] [ebp-1h]
	char cb; // [esp+Fh] [ebp-1h]
	char cc; // [esp+Fh] [ebp-1h]
	char cd; // [esp+Fh] [ebp-1h]
	char ce; // [esp+Fh] [ebp-1h]
	char cf; // [esp+Fh] [ebp-1h]

	c = *sz;
	*sz = sz[13];
	sz[13] = c;
	ca = sz[31];
	sz[31] = sz[25];
	sz[25] = ca;
	cb = sz[12];
	sz[12] = sz[30];
	sz[30] = cb;
	cc = sz[7];
	sz[7] = sz[19];
	sz[19] = cc;
	cd = sz[3];
	sz[3] = sz[21];
	sz[21] = cd;
	ce = sz[9];
	sz[9] = sz[20];
	sz[20] = ce;
	cf = sz[15];
	sz[15] = sz[18];
	sz[18] = cf;
}

void __cdecl CUtil::pgDecrypt(char* szKey, int nKeyLen, char* szBuffer, const char* szEnc, int nStrLen)
{
	int i; // [esp+Ch] [ebp-14h]

	for (i = 0; i < nStrLen; ++i)
		szBuffer[i] = (szEnc[i] + 63 - (szKey[i % nKeyLen] - 32)) % 95 + 32;
	szBuffer[i] = 0;
}

int __cdecl CUtil::SimplyDecryptPassword(char* szPass, const char* szEncrypted)
{
	char szBuffer[33]; // [esp+1Bh] [ebp-2Dh] BYREF
	int nLen; // [esp+3Ch] [ebp-Ch]
	int nPKLen; // [esp+40h] [ebp-8h]
	int i; // [esp+44h] [ebp-4h]

	nLen = strlen(szEncrypted);
	//printf("nLen %d\n", nLen);
	nPKLen = 0;
	i = 0;
	if (nLen != 32)
		return 0;
	for (i = 0; i <= 31; ++i)
		szBuffer[i] = szEncrypted[i];
	szBuffer[32] = 0;
	pgSwapChars((char*)&szBuffer);
	nPKLen = szChar2int(szBuffer[0]);
	//printf("nPKLen %x\n", szBuffer[0]);
	//printf("nPKLen %d\n", strtol(&szBuffer[0], NULL, 16));
	//printf("nPKLen %d\n", nPKLen);
	if (nPKLen <= 9 || nPKLen > 30)
		return 0;
	nLen = szChar2int(szBuffer[nPKLen + 1]);
	//printf("nLen %d\n", nLen);
	if (nLen < 0 || nLen > 20)
		return 0;
	//printf("nLen %d\n", nLen);
	pgDecrypt(&szBuffer[1], nPKLen, szPass, &szBuffer[nPKLen + 2], nLen);

	return 1;
}

int __cdecl CUtil::szChar2int(char a1)
{
	return (int)(LOBYTE(a1) ^ 0x97) >> 1 & 0x1F;
}

char __cdecl CUtil::sub_4024A0(int nLen, DWORD TickCount)
{
	signed int i; // [esp+50h] [ebp-8h]
	signed int j; // [esp+50h] [ebp-8h]
	char v5; // [esp+54h] [ebp-4h]

	if (nLen >= 31)
		return '\xFF';
	for (i = nLen % 95; i < (TickCount % 95 + 95); ++i)
	{
		v5 = i % 0x5F + 32;
		if (this->szChar2int(v5) == nLen && (v5 >= 97 && v5 <= 122 || v5 >= 65 && v5 <= 90 || v5 == 95 || v5 >= 48 && v5 <= 57))
			return i % 0x5F + 32;
	}
	for (j = TickCount % 0x5F; j < (TickCount % 0x5F + 95); ++j)
	{
		if (this->szChar2int(j % 95 + 32) == nLen)
			return j % 95 + 32;
	}
	return '\xFF';
}

int __cdecl CUtil::sub_402840(char* a1, int a2, char* a3, const char* a4, int a5)
{
	int v5; // ecx
	int v7; // [esp+4Ch] [ebp-10h]
	int v8; // [esp+50h] [ebp-Ch]
	int v9; // [esp+54h] [ebp-8h]
	int i; // [esp+58h] [ebp-4h]

	for (i = 0; ; ++i)
	{
		if (i >= a5)
			return 1;
		v8 = *(a1 + i % a2);
		v9 = (*(i + a4) + v8 - 64) % 95 + 32;
		if ((v9 < 97 || v9 > 122) && (v9 < 65 || v9 > 90) && (*(i + a4) + v8 - 64) % 95 != 63 && (v9 < 48 || v9 > 57))
			break;
	LABEL_31:
		*(i + a3) = v9;
	}
	v7 = 0;
	while (1)
	{
		if (++v9 > 126)
			v9 = 32;
		if (++v8 > 126)
			v8 = 32;
		*(a1 + i % a2) = v8;
		v5 = v7++;
		if (v5 > 255)
			return 0;
		if ((v8 >= 97 && v8 <= 122 || v8 >= 65 && v8 <= 90 || v8 == 95 || v8 >= 48 && v8 <= 57)
			&& (v9 >= 97 && v9 <= 122 || v9 >= 65 && v9 <= 90 || v9 == 95 || v9 >= 48 && v9 <= 57))
		{
			goto LABEL_31;
		}
	}
}

BOOL __cdecl CUtil::sub_402A20(char* a1, const char* a2)
{
	int i; // [esp+4Ch] [ebp-Ch]
	char v4; // [esp+50h] [ebp-8h]
	char v5; // [esp+54h] [ebp-4h]

	for (i = 0; ; ++i)
	{
		v5 = *(i + a1);
		if (!v5)
			break;
		v4 = *(i + a2);
		if (!v4)
			break;
		if (v5 != v4)
			return 0;
	}
	return !*(i + a1) && !*(i + a2);
}

int __cdecl CUtil::SimplyEncryptPassword(char* szPass, const char* szDecrypted, unsigned int TickCount, DWORD* nOutput)
{
	unsigned int v5; // [esp+4Ch] [ebp-D4h]
	unsigned int v6; // [esp+50h] [ebp-D0h]
	int v7; // [esp+58h] [ebp-C8h]
	char v8[36]; // [esp+5Ch] [ebp-C4h] BYREF
	char v9; // [esp+80h] [ebp-A0h]
	unsigned int v10; // [esp+84h] [ebp-9Ch]
	char v11; // [esp+88h] [ebp-98h]
	unsigned int v12; // [esp+8Ch] [ebp-94h]
	int v13; // [esp+90h] [ebp-90h]
	int i; // [esp+94h] [ebp-8Ch]
	int v15; // [esp+98h] [ebp-88h]
	int v16; // [esp+9Ch] [ebp-84h]
	int v17[32]; // [esp+A0h] [ebp-80h]

	if (++*nOutput > 32)
		return 0;
	v17[0] = 1153;
	v17[1] = 1789;
	v17[2] = 2797;
	v17[3] = 3023;
	v17[4] = 3491;
	v17[5] = 3617;
	v17[6] = 4519;
	v17[7] = 4547;
	v17[8] = 5261;
	v17[9] = 5939;
	v17[10] = 6449;
	v17[11] = 7307;
	v17[12] = 8053;
	v17[13] = 9221;
	v17[14] = 9719;
	v17[15] = 9851;
	v17[16] = 313;
	v17[17] = 659;
	v17[18] = 1229;
	v17[19] = 1847;
	v17[20] = 2459;
	v17[21] = 3121;
	v17[22] = 3793;
	v17[23] = 4483;
	v17[24] = 5179;
	v17[25] = 6121;
	v17[26] = 6833;
	v17[27] = 7333;
	v17[28] = 7829;
	v17[29] = 8353;
	v17[30] = 9323;
	v17[31] = 9829;
	v16 = strlen(szDecrypted);
	v15 = 0;
	i = 0;
	v13 = 0;
	if (v16 > 20)
		return 0;
	for (i = 0; i < v16; ++i)
	{
		if (szDecrypted[i] < 32 || szDecrypted[i] > 126)
			return 0;
	}
	//printf("Len %d\n", v16);
	v15 = 32 - v16 - 2;
	//printf("PKLen %d\n", v15);
	if (v15 > 10)
		v15 = (TickCount + 0x27FD) % (v15 - 10) + 10;
	*szPass = sub_4024A0(v15, TickCount);
	for (i = 0; i < v15; ++i)
	{
		v12 = v17[i] + TickCount;
		v11 = v12 % 0x5F + 32;
		if ((v11 < 97 || v11 > 122) && (v11 < 65 || v11 > 90) && v11 != 95 && (v11 < 48 || v11 > 57))
		{
			if ((v12 & 1) != 0)
				v6 = v12 % 0x1A + 97;
			else
				v6 = v12 % 0x1A + 65;
			v11 = v6;
		}
		szPass[i + 1] = v11;
	}
	szPass[v15 + 1] = sub_4024A0(v16, TickCount);
	//printf("szPass %s\n", szPass);
	v13 = sub_402840((szPass + 1), v15, &szPass[v15 + 2], szDecrypted, v16);
	for (i = 0; i < 30 - v15 - v16; ++i)
	{
		v10 = v17[32 - i - 1] + TickCount;
		v9 = v10 % 0x5F + 32;
		if ((v9 < 97 || v9 > 122) && (v9 < 65 || v9 > 90) && v9 != 95 && (v9 < 48 || v9 > 57))
		{
			if ((v10 & 1) != 0)
				v5 = v10 % 26 + 97;
			else
				v5 = v10 % 0x1A + 65;
			v9 = v5;
		}
		szPass[32 - i - 1] = v9;
	}
	pgSwapChars(szPass);
	szPass[32] = 0;
	v7 = 0;
	if (v13)
		v7 = SimplyDecryptPassword(v8, szPass);
	if (v13 && v7 && sub_402A20(v8, szDecrypted))
		return 1;
	else
		return SimplyEncryptPassword(szPass, szDecrypted, TickCount + 9929, nOutput);
}

std::string CUtil::ShowIPWan()
{
	char requestdata[32];
	memset(requestdata, 0, 32);
	int RequestSize = 0;
	RequestSize = gPluginWeb.RequestURL("https://activation.volam1pk.net/checkip.php", "", &requestdata);
	std::string returndata(reinterpret_cast<char const*>(requestdata), sizeof(requestdata) / sizeof(char));
	return returndata;
}
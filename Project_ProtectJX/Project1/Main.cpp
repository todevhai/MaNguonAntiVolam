#include "stdafx.h"
#include "Main.h"
#include "Util.h"
#include "KItem.h"
#include "KPlayerNew.h"
#include <sys/mman.h>
#include <sys/sysctl.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <errno.h>
#include <thread>
#include <iostream>
#include <fstream>
#include "MD5.h"
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "include/INIReader.h"
#include <netdb.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "KLoginX.h"
#include <fcntl.h>
#include "CConnection.h"
#include <sys/resource.h>
#include "TextList.h"
#if AUTH_TYPE == 1
#include "PluginWeb.h"
#else
#include "Authenticate.h"
#endif
#include "Sqlite.h"
#include "KDetect.h"
#include "Goddess.h"

using namespace std;

int m_ServerID = 0;
int m_ServerPort = 0;
extern char IpInterface[32];
extern char MacInterface[18];
extern CAuthenticate gAuthenticate;

#define Execute_GetTenRoleListPlus		((TRoleList*(__cdecl*)(KLadder *This, DWORD nID)) 0x8085A80)//0x0063A578 

std::string getDiskSerialNumber(string cmd)
{
	string data;
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
	return data;
}

static void handler(int signalNum, siginfo_t* si, void* unused)
{
	signal(SIGINT, SIG_DFL);
	printf("Got SIGSEGV at address: %p\n", si->si_addr);
	//exit(EXIT_FAILURE);
	throw(signalNum);
	//exit(signal);
}

EXPORT int Goddess()
{
	gAuthenticate.m_ResCheckAuthen = false;

	printf("=========== %s Goddess v%d.%s ===========\n", ANTI_NAME, GODDESS_VERSION, GODDESS_VERSION_SUB);

	gAuthenticate.m_IsGoddess = true;

	m_Goddess.m_pRoleRank = NULL;

	if (!gPluginWeb.ReconnectCheck())
	{
		printf("LICENSE DISABLE\n");
		printf("===========================================\n");
		printf("Authorization failed.\n");
		printf("%s\n", ANTI_WEBSITE);
		exit(1);
	}

	if (gUtil.PACKETFILTER_ACTIVE == 1)
	{
		INIReader reader("./goddess.cfg");

		if (reader.ParseError() < 0) {
			//std::cout << "Failed to load servercfg.ini'\n";
			printf("Failed to load goddess.cfg\n");
			exit(0);
		}

		gUtil.nTimeUpdateRank = reader.GetInteger("Setting", "BackupTime", 0);

		if (gUtil.nTimeUpdateRank > 0)
		{
			m_Goddess.Hook_RoleRankInitialize();
		}

	}

	return 1;
}

void WorkBishop()
{
	try
	{

		if (!gPluginWeb.ReconnectCheck())
		{
			printf("LICENSE DISABLE\n");
			printf("===========================================\n");
			printf("Authorization failed.\n");
			printf("%s\n", ANTI_WEBSITE);
			exit(1);
		}
		
		if (gUtil.PACKETFILTER_ACTIVE == 1)
		{
			printf(LOAD_LIBRARY.c_str());
			gUtil.InitBishopLogFile();

			gKLoginX.Hook_WaitForAccPwd();

			gUtil.SetByte(0x0804E9CE, gUtil.nVersionProtocol);
			gUtil.SetByte(0x80908F0, 16);
			gKLoginX.WhiteList_Data.clear();

			fstream newfile;
			newfile.open("whitelist.txt", ios::in);
			if (newfile.is_open()) {
				std::string tp;
				while (std::getline(newfile, tp)) {

					if (tp.size() <= 0)
						break;
					WHITELIST_HARDID_INFO HidInfo;

					memcpy(HidInfo.HardwareId, tp.c_str(), 32);

					HidInfo.HardwareId[31] = '\0';

					gKLoginX.WhiteList_Data.push_back(HidInfo);

				}
				newfile.close(); //close the file object.
			}
			else
			{
				newfile.open("whitelist.txt", fstream::out);
				newfile.close();
			}

			INIReader reader("./bishop.cfg");

			if (reader.ParseError() < 0) {
				//std::cout << "Failed to load servercfg.ini'\n";
				printf("Failed to load bishop.cfg\n");
				exit(0);
			}

			int nPort = reader.GetInteger("Network", "ClientOpenPort", 5622);

			//if (gUtil.WORLDRANK_ACTIVE == 2)
			//{

			//	KIniFile FileHandle;
			//	Bishop_KIniFile(&FileHandle);
			//	Bishop_KIniFile_Load(&FileHandle, "goddess.cfg");
			//	Bishop_KIniFile_GetInteger(&FileHandle, "Setting", "BackupTime", 60, &gUtil.nTimeUpdateRank);
			//	Bishop_KIniFile_GetInteger(&FileHandle, "Setting", "BackupPort", 60, &gUtil.nBackupPort);
			//	Bishop_KIniFile_Clear(&FileHandle);

			//	printf(SORTRANK.c_str(), gUtil.nTimeUpdateRank);

			//	pthread_t thread_id;
			//	time_t now = time(nullptr);
			//	size_t Minutes = gUtil.nTimeUpdateRank;
			//	time_t newTime = now + (60 * Minutes);
			//	gUtil.TimeUpdateRank = newTime;
			//	//pthread_create(&thread_id, NULL, &SortRankData, NULL);
			//}
			if(gUtil.nLimit > 0)
				std::cout << "Limit game: " << gUtil.nLimit << std::endl;

			if(gUtil.nLimitIP > 0)
				std::cout << "Limit IpAddress: " << gUtil.nLimitIP << std::endl;

			std::cout << "Protocol Version: " << gUtil.nVersionProtocol << std::endl;

			std::cout << "===========================================" << std::endl;
		}
		else
		{
			printf("LICENSE DISABLE\n");
			printf("===========================================\n");
			exit(0);
		}

	}
	catch (std::exception const& e)
	{
		printf("LICENSE DISABLE\n");
		printf("===========================================\n");
		exit(0);
	}

}

void WorkGameServer()
{

	try
	{
		if (!gPluginWeb.ReconnectCheck())
		{
			printf("LICENSE DISABLE\n");
			printf("===========================================\n");
			printf("Authorization failed.\n");
			printf("%s\n", ANTI_WEBSITE);
			exit(1);
		}

		gAuthenticate.m_ResCheckAuthen = true;
		KIniFile FileHandle;
		char WorldsetName[16];
		char MapsWorldsetPath[16];
		strcpy(WorldsetName, (char*)0x82529AB);
		sprintf(MapsWorldsetPath, "\\maps\\%s", WorldsetName);

		Execute_KIniFile(&FileHandle);
		Execute_KIniFile_Load(&FileHandle, MapsWorldsetPath);
		Execute_KIniFile_GetInteger(&FileHandle, "Init", "ServerID", 1, &gUtil.m_ServerID);
		Execute_KIniFile_Clear(&FileHandle);

		printf("LIBRARY LOAD SUCCESSFULLY\n");

		if (gUtil.THANPHAP_ACTIVE == 1)
			THANPHAP_TYPE = std::string("ACTIVE ALL");
		else if (gUtil.THANPHAP_ACTIVE == 2)
			THANPHAP_TYPE = std::string("ACTIVE MOC");

		/*if (gUtil.WORLDRANK_ACTIVE == 1)
		{
			WORLDRANK_TYPE = std::string("ACTIVE");
		}

		if (gUtil.WORLDRANK_ACTIVE == 2)
		{
			WORLDRANK_TYPE = std::string("AUTO SORT");
		}*/

		if (gUtil.PACKETFILTER_ACTIVE == 1)
		{
			ANTIDUMP_ACTIVE = std::string("ACTIVE");
		}

		gUtil.Initialize();

		if (gUtil.PACKETFILTER_ACTIVE == 1)
		{
			IsInitCoreAfterRelayData = false;

			if (gUtil.THANPHAP_ACTIVE > 0)
			{
				gKPlayerNew.HookAll();
			}

			printf(ANTIDUMP.c_str(), ANTIDUMP_ACTIVE.c_str());
			printf(THANPHAP.c_str(), THANPHAP_TYPE.c_str());
			
			INIReader reader("./servercfg.ini");

			if (reader.ParseError() < 0) {
				printf("Failed to load servercfg.ini\n");
				exit(0);
			}

			/*gUtil.nTopListType = 1;

			memcpy(&gUtil.nFileTopList, reader.GetString("Setting", "PathTopList", "worldrank/toplist.txt").c_str(), 128);

			fstream newfile;

			newfile.open("dulieu/ranklist.txt", ios::in);

			if (newfile.is_open()) {
				memcpy(&gUtil.nFileTopList, reader.GetString("Setting", "PathTopList", "dulieu/ranklist.txt").c_str(), 128);
				newfile.close();
				gUtil.nTopListType = 2;
			}
			else
			{
				memcpy(&gUtil.nFileTopList, reader.GetString("Setting", "PathTopList", "worldrank/toplist.txt").c_str(), 128);
				newfile.close();
				gUtil.nTopListType = 1;
			}*/

			//if (gUtil.WORLDRANK_ACTIVE > 0)
			//{
			//	gKPlayerNew.m_nRankIndex = 0;

			//	time_t now = time(nullptr);
			//	size_t Minutes = 15;
			//	time_t newTime = now + (60 * Minutes);
			//	//time_t newTime = now + 10;
			//	gUtil.TimeUpdateRank = newTime;

			//	/*TRoleList* Rank = Execute_GetTenRoleListPlus(((KLadder*)0x82BCE40), 10270);

			//	for (int i = 0; i < 10; i++)
			//	{
			//		gKPlayerNew.WorldRank_Data.insert(pair<std::string, int>(Rank[i].Name, (i + 1)));
			//	}*/

			//	if (gUtil.WORLDRANK_ACTIVE == 1)
			//	{
			//		int nRank = 1;
			//		if (gUtil.nTopListType == 2)
			//			nRank = 0;
			//		int nBoQua = 0;
			//		fstream newfile;
			//		//printf("FileTopList %s\n", gUtil.nFileTopList);
			//		newfile.open(gUtil.nFileTopList, ios::in);
			//		if (newfile.is_open()) {
			//			std::string tp;
			//			while (std::getline(newfile, tp)) {

			//				if (nBoQua < 10)
			//				{
			//					nBoQua++;
			//					continue;
			//				}

			//				if (nRank == 0)
			//				{
			//					nRank++;
			//					continue;
			//				}

			//				std::string delimiter = "	";
			//				std::vector<std::string> results = gUtil.split(tp, delimiter);
			//				gKPlayerNew.WorldRank_Data.insert(pair<std::string, int>(results[0], nRank));

			//				nRank++;

			//				if (nRank > 1000)
			//					break;

			//			}
			//			newfile.close(); //close the file object.
			//		}
			//		gKPlayerNew.HookRank();
			//	}
			//	else
			//	{
			//		gKPlayerNew.HookRank();
			//	}

			//}

			gUtil.nMaxPlayer = reader.GetInteger("Overload", "MaxPlayer", 500);
			if (gUtil.nMaxPlayer > 1200)
				gUtil.nMaxPlayer = 1198;

			gUtil.nMaxTongMember = reader.GetInteger("Overload", "MaxTongLimit", 120);

			gUtil.nMaxTransLifer = reader.GetInteger("Overload", "MaxTransLifeLimit", 4);

			if (gUtil.nMaxTransLifer < 4)
				gUtil.nMaxTransLifer = 4;

			if (gUtil.nMaxTransLifer > 8)
				gUtil.nMaxTransLifer = 8;

			gUtil.nAddTongMoneyUi = reader.GetInteger("Overload", "AddTongMoneyUi", 1);

			gUtil.nPickUpMoney = reader.GetInteger("Setting", "CheckPickUpMoney", 0);

			memcpy(&gUtil.nScriptPickUpMoney, reader.GetString("Setting", "ScriptPickUpMoney", "").c_str() , 100);

			gUtil.nCheckChatActive = reader.GetInteger("Setting", "CheckBugChat", 0);
			gUtil.nStall = reader.GetInteger("Setting", "Stall", 0);
			gUtil.nPacket = reader.GetInteger("Setting", "SpamTong", 0);
			gUtil.nTimeUpdateRank = reader.GetInteger("Setting", "TimeUpdateRank", 60);
			gUtil.nGroupChat = reader.GetInteger("Setting", "OpenGroupChat", 1);
			gUtil.nLogChat = reader.GetInteger("Setting", "LogChat", 0);
			gUtil.nLogShop = reader.GetInteger("Setting", "LogShop", 0);
			gUtil.nLoginToAuthen = reader.GetInteger("Setting", "Login2Auth", 0);

		/*	KIniFile FileHandle;
			Execute_KIniFile(&FileHandle);
			Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "ThieuLam", 0, &gUtil.FactionLimit[0]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "ThienVuong", 0, &gUtil.FactionLimit[1]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "DuongMon", 0, &gUtil.FactionLimit[2]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "NguDoc", 0, &gUtil.FactionLimit[3]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "NgaMy", 0, &gUtil.FactionLimit[4]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "ThuyYen", 0, &gUtil.FactionLimit[5]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "CaiBang", 0, &gUtil.FactionLimit[6]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "ThienNhan", 0, &gUtil.FactionLimit[7]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "VoDang", 0, &gUtil.FactionLimit[8]);
			Execute_KIniFile_GetInteger(&FileHandle, "FactionLimit", "ConLon", 0, &gUtil.FactionLimit[9]);
			Execute_KIniFile_GetInteger(&FileHandle, "Setting", "CheckAuto", 0, &gUtil.nCheckAuto);
			Execute_KIniFile_Clear(&FileHandle);*/

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
			gUtil.nCheckAuto = reader.GetInteger("Setting", "CheckAuto", 0);

			/*for (int i = 0; i < 10; ++i)
			{
				char* szFactionText = (char*)malloc(256);
				sprintf(szFactionText, "Faction[%d]: %d Account / IP", i, gUtil.FactionLimit[i]);
				printf("%s\n", szFactionText);
				free(szFactionText);
			}*/
			

			/*int nTempNumber = 0;
			nTempNumber = reader.GetInteger("FactionLimit", "ThieuLam", 0);
			gUtil.FactionLimit[0] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "ThienVuong", 0);
			gUtil.FactionLimit[1] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "DuongMon", 0);
			gUtil.FactionLimit[2] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "NguDoc", 0);
			gUtil.FactionLimit[3] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "NgaMy", 0);
			gUtil.FactionLimit[4] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "ThuyYen", 0);
			gUtil.FactionLimit[5] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "CaiBang", 0);
			gUtil.FactionLimit[6] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "ThienNhan", 0);
			gUtil.FactionLimit[7] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "VoDang", 0);
			gUtil.FactionLimit[8] = nTempNumber;
			nTempNumber = (int)reader.GetInteger("FactionLimit", "ConLon", 0);
			gUtil.FactionLimit[9] = nTempNumber;*/

			/*gUtil.FactionLimit_TL = reader.GetInteger("FactionLimit", "ThieuLam", 0);
			gUtil.FactionLimit_TV = reader.GetInteger("FactionLimit", "ThienVuong", 0);
			gUtil.FactionLimit_DM = reader.GetInteger("FactionLimit", "DuongMon", 0);
			gUtil.FactionLimit_ND = reader.GetInteger("FactionLimit", "NguDoc", 0);
			gUtil.FactionLimit_NM = reader.GetInteger("FactionLimit", "NgaMy", 0);
			gUtil.FactionLimit_TY = reader.GetInteger("FactionLimit", "ThuyYen", 0);
			gUtil.FactionLimit_CB = reader.GetInteger("FactionLimit", "CaiBang", 0);
			gUtil.FactionLimit_TN = reader.GetInteger("FactionLimit", "ThienNhan", 0);
			gUtil.FactionLimit_VD = reader.GetInteger("FactionLimit", "VoDang", 0);
			gUtil.FactionLimit_CL = reader.GetInteger("FactionLimit", "ConLon", 0);*/
			//gUtil.nRankType = 1;

			if (gUtil.nTimeUpdateRank > 0)
			{
				time_t now = time(nullptr);
				size_t Minutes = gUtil.nTimeUpdateRank;
				time_t newTime = now + (60 * Minutes);
				gUtil.TimeUpdateRank = newTime;
			}
			

			gUtil.nDdos = reader.GetInteger("Ddos", "Active", 0);
			gUtil.nBlock = reader.GetInteger("Ddos", "Block", 0);
			gUtil.nDdosDelay = reader.GetInteger("Ddos", "Delay", 500);
			gUtil.nDdosFloodLimit = reader.GetInteger("Ddos", "FloodLimit", 4);
			
			/*char ServerConfig[15];
			sprintf(ServerConfig, "servercfx.ini", (gUtil.m_ServerID - 1));
			INIReader readerServerConfig(ServerConfig);

			if (readerServerConfig.ParseError() < 0) {
				printf("Failed to load %s\n", ServerConfig);
				exit(0);
			}

			int nPort = readerServerConfig.GetInteger("GameServer", "Port", 6666);*/

			//printf("nPort: %d\n", nPort);

			if (gUtil.nDdos == 1)
			{
				char buf[128];
				//sprintf(buf, "iptables -A INPUT -p tcp -m tcp --dport $port2 -m conntrack --ctstate NEW -j PortGS", nPort);
				//sprintf(buf, "iptables -A INPUT -p tcp --dport %d -j PortGS", nPort);
				//system(buf);
				//system("iptables -A PortGS -m state --state ESTABLISHED -j ACCEPT");
				//system("iptables -A PortGS -m state --state NEW -m set --match-set filter src -j ACCEPT");
				//system("iptables -A PortGS -p tcp -j REJECT");

				if (gUtil.nBlock == 1)
				{
					gUtil.nDdos = 2;
				}
				//gUtil.nDdos = gUtil.nDdos + gUtil.nBlock;

			}
			else
				gUtil.nDdos = 0;
			/*if (gUtil.nDdos == 3)
			{
				if (!gUtil.nDdosCSF)
					gUtil.nDdos = 2;
			}*/

			gUtil.nChat = reader.GetInteger("FloodChat", "Active", 0);
			gUtil.nChatTick = reader.GetInteger("FloodChat", "Delay", 500);
			gUtil.nChatFlood = reader.GetInteger("FloodChat", "FloodLimit", 5);

			gUtil.nNPCTalkActive = reader.GetInteger("NPCTalk", "Active", 0);
			gUtil.nTickCount = reader.GetInteger("NPCTalk", "DelayNpcTalk", 500);
			gUtil.nLimitNPCTalkFlood = reader.GetInteger("NPCTalk", "NPCTalkFloodLimit", 10);

			gUtil.nTimerID = reader.GetInteger("Setting", "TimerID", 0);

			if(gUtil.nPickUpMoney)
				printf("CHECK NHAT TIEN: ACTIVE\n");

			if (gUtil.DISABLE_EXP_SKILL_ACTIVE == 1)
			{
				gUtil.SetByte(0x804C3BC, 0x99);
				gUtil.SetByte(0x804C3BC + 1, 0x99);
				gUtil.SetByte(0x804C3BC + 2, 0x99);

				gUtil.SetByte(0x804C40E, 0x99);
				gUtil.SetByte(0x804C40E + 1, 0x99);
				gUtil.SetByte(0x804C40E + 2, 0x99);

				gUtil.SetByte(0x80A2657, 0x08);
				gUtil.SetByte(0x80A2659, 0x46);
				gUtil.SetByte(0x80A2659 + 1, 0x24);
				gUtil.SetByte(0x80A2659 + 2, 0x90);
				gUtil.SetByte(0x80A265E, 0x00);
				gUtil.SetByte(0x80A2663, 0x03);
				//HookSkillExp();
				printf(EXPSKILL.c_str(), "ACTIVE");
			}
			else
				printf(EXPSKILL.c_str(), "DISABLE");

			gUtil.SetByte(0x820EA42, 0x7B);
			printf(WORLDRANK.c_str(), WORLDRANK_TYPE.c_str());

			gDetect.Hook_FixRootKit(); // fix xoa server
			gKPlayerNew.Hook_StallCoin(); // bay ban xu
			gKItemBuySell.Hook();
			gKPlayerNew.HookFixAll();
			gKPlayerNew.Hook_LoadRoleData();
			gKPlayerNew.HookCustomScriptLua();

			if (gUtil.nNPCTalkActive > 0)
				gKPlayerNew.Hook_DialogNpc();

			if (gUtil.nPacket > 0)
				gKPlayerNew.Hook_TextGMFilter();

			if (gUtil.nCheckChatActive > 0)
				gKPlayerNew.Hook_ServerSendChat();

			if (gUtil.nPickUpMoney > 0)
				gKPlayerNew.Hook_PickUpMoney();

			if (gUtil.nDdos == 2)
			{
				gKPlayerNew.Hook_CheckProtocolSize();
			}

			gUtil.SetByte(0x80C724B, (BYTE)gUtil.nMaxTransLifer);

			gUtil.InitLogFile();

	

		}
		else
		{
			printf("LICENSE DISABLE\n");
			printf("===========================================\n");
			exit(0);
		}
	}
	catch (std::exception const& e)
	{
		printf("LICENSE DISABLE\n");
		printf("===========================================\n");
		std::cerr << "Error: " << e.what() << std::endl;
		exit(0);
	}

}

EXPORT int Init()
{
	
	gAuthenticate.m_ResCheckAuthen = false;
	if (gAuthenticate.m_AuthReceived == false)
	{
		try
		{
			time_t t = time(NULL);
			struct tm* tm = localtime(&t);
			INIReader reader("package.ini");
			if (reader.ParseError() < 0)
			{
				gUtil.nLimit = 0;
				printf("=========== %s Bishop v%d.%s ===========\n", ANTI_NAME, BISHOP_VERSION, BISHOP_VERSION_SUB);
				const int BUFSIZE = 4096; // should really get PAGESIZE or something instead...
				unsigned char buffer[BUFSIZE]; // dynamic allocation rather than stack/global would be better
				char* name = (char*)malloc(BUFSIZE);
				int fd = open("/proc/self/cmdline", O_RDONLY);
				int nbytesread = read(fd, buffer, BUFSIZE);
				unsigned char* end = buffer + nbytesread;
				int next = 0;

				gUtil.nVersionProtocol = 1;
				gUtil.nClientVersion = 0;
				gUtil.nTimeUpdateRank = 60;

				gUtil.nLimit = 0;
				char args1[16];
				for (unsigned char* p = buffer; p < end; /**/)
				{
					if (next == 1)
					{
						gUtil.nVersionProtocol = atoi((char*)p);
					}

					if (next == 2)
					{
						gUtil.nLimit = atoi((char*)p);
					}

					if (next == 3)
					{
						gUtil.nTimeUpdateRank = atoi((char*)p);
					}

					next++;
					while (*p++);
				}

				if (next == 0)
				{
					printf("Data is correct!\n    -> Run: ./bishop_y <VersionCheck> <GameLimit> <MinTimeSortRank>\n");
					printf("===========================================\n");
					printf("Get help from: %s\n", ANTI_WEBSITE);
					exit(1);
				}

				close(fd);

				gAuthenticate.m_IsBishop = true;
				gAuthenticate.m_GameServer = false;

				WorkBishop();

				return 0;

			}
			else
			{
				printf("=========== %s GameServer v%d.%s ===========\n", ANTI_NAME, GAME_VERSION, GAME_VERSION_SUB);
				const int BUFSIZE = 16; // should really get PAGESIZE or something instead...
				unsigned char buffer[BUFSIZE]; // dynamic allocation rather than stack/global would be better
				int fd = open("/proc/self/cmdline", O_RDONLY);
				int nbytesread = read(fd, buffer, BUFSIZE);
				unsigned char* end = buffer + nbytesread;
				int next = 0;
				KIniFile FileHandle;
				for (unsigned char* p = buffer; p < end; /**/)
				{
					if (next == 1)
					{
						char cIP[32];
						sprintf(cIP, "127.0.0.%s", p);
						gUtil.MemorySet(0x8262207, (DWORD)cIP, strlen(cIP));
						gUtil.SetByte(0x824905F, 0x78);//67 = g 78 = x
						gUtil.SetByte(0x824B214, 0x74);
						gUtil.SetByte(0x82529B2, 0x74);
						gUtil.m_ServerID = atoi((char*)p);
						gUtil.m_ServerPort = 6600 + atoi((char*)p);
						Execute_KIniFile(&FileHandle);
						Execute_KIniFile_Load(&FileHandle, "servercfx.ini");
						Execute_KIniFile_WriteString(&FileHandle, "Transfer", "Ip", cIP);
						Execute_KIniFile_WriteString(&FileHandle, "Chat", "Ip", cIP);
						Execute_KIniFile_WriteString(&FileHandle, "Tong", "Ip", cIP);
						Execute_KIniFile_WriteInteger(&FileHandle, "GameServer", "Port", 6600 + atoi((char*)p));
						Execute_KIniFile_WriteString(&FileHandle, "FixIp", "IntranetIp", cIP);
						Execute_KIniFile_Save(&FileHandle, "servercfx.ini");

						Execute_KIniFile(&FileHandle);
						Execute_KIniFile_Load(&FileHandle, "\\maps\\worldset.ini");
						Execute_KIniFile_WriteInteger(&FileHandle, "Init", "ServerID", atoi((char*)p));
						Execute_KIniFile_Save(&FileHandle, "\\maps\\worldset.ini");
						Execute_KIniFile_Clear(&FileHandle);
					}

					if (next == 2)
					{
						m_ServerPort = atoi((char*)p);
						Execute_KIniFile(&FileHandle);
						Execute_KIniFile_Load(&FileHandle, "servercfx.ini");
						Execute_KIniFile_WriteInteger(&FileHandle, "GameServer", "Port", atoi((char*)p));
						Execute_KIniFile_Save(&FileHandle, "servercfx.ini");
						Execute_KIniFile_Clear(&FileHandle);
					}

					next++;
					while (*p++);
				}
				close(fd);
				gAuthenticate.m_IsBishop = false;
				gAuthenticate.m_GameServer = true;
				WorkGameServer();
				return 0;
			}
		}
		catch (std::exception const& e)
		{
			//return EXIT_FAILURE;
			//printf("LICENSE DISABLE\n");
			//printf("===========================================\n");
			std::cerr << "Error: " << e.what() << std::endl;
			exit(0);
		}

	}
	//else
	//{
	//	return 0;
	//}
}

//void __attribute__((destructor)) Safe_Exit()
//{
//	//execute(format("curl -ss --tlsv1.2 -G -v 'https://taikhoan.volamhaingoai.com/botmess/notice.php' --data \"action=%s\" --data \"account=%s\" --data \"value=%s\" --data-urlencode \"datapost=%s\" &> /dev/null", tostring(cAction), tostring(szParam1), tostring(szParam2), tostring(cData)))
//
//	printf("Safe anti Exit\n");
//}
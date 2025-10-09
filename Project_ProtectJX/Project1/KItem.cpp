#include "stdafx.h"
//#include "Defines.h"
#include "KPlayerNew.h"
#include "KItem.h"
#include "Util.h"
//#include "subhook.c"
//#include <chrono>
//#include <thread>
#include <sys/mman.h>
//#include <string.h>
//#include <cstdio>   // use the C++ versions of the header files
//#include <cstring>
#include <unistd.h>
#include <string.h>
#include <ctime>
#include <iomanip>
#include <iostream>
#include "include/INIReader.h"
#include <execinfo.h>
#include <cstdarg>
#include <chrono>
#include "Authenticate.h"

//#include <csignal>
//#include "../../../../../../Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Include/WinBase.h"
//#include <memory>
//#include <memory.h>
//
//subhook::Hook foo_hook;
//subhook_t func_hook;

#define Execute_ClearAllPrice		((int(__cdecl*)(KPlayerStall* This)) 0x80E1950)//0x0063A578 
#define Execute_SyncStallInfo		((int(__cdecl*)(KPlayerStall* This)) 0x80E1E20)//0x0063A578 
#define Execute_SetStallState		((int(__cdecl*)(KPlayerStall* This, int nStallState)) 0x80E2290)//0x0063A578 
#define Execute_ProcessStallProtocol		((int(__cdecl*)(KPlayerStall* This, BYTE* msg)) 0x80E3810)//0x0063A578 
#define Execute_ShutdownClient		((void(__cdecl*)(KPlayer *This)) 0x80C0230)//0x0063A578 
#define Execute_BuySellPrintf		((void(__cdecl*)(KSG_LogFile* This, const char* pcszFmt, ...)) 0x8217940)//0x0063A578 

//KBuySell* BuySell;
KItemBuySell gKItemBuySell;
_ShutdownClient ShutdownClient = (_ShutdownClient)0x80C0230;
_NewBuyItem oBuyItem = (_NewBuyItem)0x80C6AA0;
//_ProcessStallProtocol oProcessStallProtocol = (_ProcessStallProtocol)0x80E3810;
_BuyItem BuyItem = (_BuyItem)0x81FE950;
extern KPlayer* Player;
extern KPlayerInfo	m_KPlayerInfo[1200];
PlayerStallCheck	m_StallCheck[1200];
int n = 100;

static void sigaction_stall(int signal, siginfo_t* si, void* arg)
{
	ucontext_t* ctx = (ucontext_t*)arg;

	/* We are on linux x86, the returning IP is stored in RIP (64bit) or EIP (32bit).
	   In this example, the length of the offending instruction is 6 bytes.
	   So we skip the offender ! */
#if __WORDSIZE == 64
	   //printf("Caught SIGSEGV, addr %p, RIP 0x%lx\n", si->si_addr, ctx->uc_mcontext.gregs[REG_RIP]);
	ctx->uc_mcontext.gregs[REG_RIP] += 6;
#else
	   //printf("Caught SIGSEGV, addr %p, EIP 0x%x\n", si->si_addr, ctx->uc_mcontext.gregs[REG_EIP]);
	ctx->uc_mcontext.gregs[REG_EIP] += 6;
#endif
	//printf("signal_handler: %d\n", nSignal);
	//sigignore(SIGINT);
	//signal(SIGSEGV, SIG_IGN);
	//int pid;

	//pid = fork();
	//signal(SIGINT, signal_handler);
	//signal(SIGINT, SIG_IGN);

	//signal(SIGPIPE, SIG_IGN);
	/*if (pid > 0)
	{
		signal(SIGINT, signal_handler);
		kill(pid, SIGTERM);
	}*/
	//printf("Got SIGSEGV at address: %p\n", si->si_addr);
	//exit(EXIT_FAILURE);
	//throw(nSignal);
	//exit(signal);
}

static void
player_handler(int signalNum, siginfo_t* si, void* unused)
{
	signal(SIGINT, SIG_IGN);
	//printf("Got SIGSEGV at address: %p\n", si->si_addr);
	printf("Error address: %p\n", si->si_addr);
	//exit(EXIT_FAILURE);
	throw(signalNum);
	//exit(signal);
}
//
//static void
//handler(int signalNum, siginfo_t* si, void* unused)
//{
//	signal(SIGINT, SIG_DFL);
//	//printf("Got SIGSEGV at address: %p\n", si->si_code);
//	//printf("255.255.255.255|255.255.255.255|00:00:00:00:00:00", si->si_addr);
//	//exit(EXIT_FAILURE);
//	void* trace[16];
//	char** messages = (char**)NULL;
//	int i, trace_size = 0;
//
//	if (signalNum == SIGSEGV)
//		printf("Got signal %d, faulty address is %p, "
//			"from %p\n", signalNum, si->si_addr, si->si_call_addr);
//	else
//		printf("Got signal %d\n", signalNum);
//
//	trace_size = backtrace(trace, 16);
//	/* overwrite sigaction with caller's address */
//	trace[1] = (void*)si->si_call_addr;
//	messages = backtrace_symbols(trace, trace_size);
//	/* skip first stack frame (points here) */
//	printf("[bt] Execution path:\n");
//	for (i = 1; i < trace_size; ++i)
//	{
//		printf("[bt] #%d %s\n", i, messages[i]);
//
//		/* find first occurence of '(' or ' ' in message[i] and assume
//		 * everything before that is the file name. (Don't go beyond 0 though
//		 * (string terminator)*/
//		size_t p = 0;
//		while (messages[i][p] != '(' && messages[i][p] != ' '
//			&& messages[i][p] != 0)
//			++p;
//
//		char syscom[256];
//		sprintf(syscom, "Line %p -e %.*s", trace[i], p, messages[i]);
//		//last parameter is the file name of the symbol
//		system(syscom);
//	}
//	throw(signalNum);
//	//exit(signal);
//}

KItemBuySell::KItemBuySell() {
	//Hook();
}
// -----------------------------------------------------------------------------

KItemBuySell::~KItemBuySell() {
}

void KItemBuySell::Hook() {
	//signal(SIGINT, SIG_DFL);
	//signal(SIGINT, SIG_IGN);
	mprotect((void*)0x80C5000, 0x2000u, 7);
	//int iRes = mprotect((void*)0x80C5000, 0x2000u, PROT_READ | PROT_WRITE | PROT_EXEC);
	//int iRes = mprotect((void*)0x80925E9, 0x2000u, PROT_READ | PROT_WRITE | PROT_EXEC);
	// ----
	/*if (iRes == -1)
	{
		printf("KItemBuySell::Hook: mprotect error %d\n", iRes);
	}*/

	//size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x80C5000;
	////uintptr_t end = start + sizeof(GetEngergyDamageHook);

	////  Calculate start of page for mprotect.
	//uintptr_t pagestart = start & -pagesize;

	////  Change memory protection.
	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//*(BYTE*)(0x80C6BC0) = *(BYTE*)BuyItemHook;
	//*(DWORD*) function = (DWORD*)BuyItemHook+1;
	//DWORD HookAddr = (DWORD)malloc(size + 5);
	//printf("info symbol  %p\n", *(BYTE*)HookAddr);
	//printf("info symbol  %p\n", (void*)HookAddr);
	//printf("info symbol  %p\n", (DWORD*)HookAddr);
	//printf("info symbol  %p\n", *(DWORD*)HookAddr);

	/*printf("info symbol  %p\n", (DWORD*)function);
	printf("info symbol  %p\n", (void*)function);*/
	//*(BYTE*)0x80C6BBF = 0x0E8;
	//*(BYTE*)0x80C6BBF = -36;
	//*(BYTE*)0x80C6BC0 = *(BYTE*)(void*)BuyItemHook - 135031748;
	//*(DWORD*)(0x80C6BC0) = (*function) - (0x80C6BC0 + 5);

	*(BYTE*)(0x80C6BBF) = -28;
	
	//gUtil.SetOp((void*)0x80C6BC0, (void*)BuyItemHook, 0xFF);
	//gUtil.SetCompleteHook(0xFF, 0x80C6BC0, (void*)BuyItemHook);
	 
	//gUtil.SetRange((void*)0x80c6ba4, 6, 0x90);
	//gUtil.SetJmp(0x80C6BC0, 12, (char*)gKItemBuySell.BuyItemHook);

	//gUtil.SetJmp(0x80c6aa0, 12, (void*)gKItemBuySell.BuyItemHook);
	//gUtil.SetJmp(0x80C6BC0, 12, (void*)gKItemBuySell.BuyItemHook - *(BYTE*)gKItemBuySell.BuyItemHook);
	//gUtil.SetJmp(0x80C6BBF, 6, (char*)gKItemBuySell.BuyItemHook);
	//gUtil.SetCompleteJmp(0x80C6BBF, 5, (void*)gKItemBuySell.BuyItemHook);
	//gUtil.SetCompleteJmp(0x80c6aa0, 5, (void*)gKItemBuySell.BuyItemHook);
	//gUtil.SetCompleteJmp(0x80C6BC0, 5, (void*)gKItemBuySell.BuyItemHook - *(BYTE*)gKItemBuySell.BuyItemHook);

	//gUtil.SetJmp(0x80C6BC0, (void*)gKItemBuySell.BuyItemHook);
	//gUtil.SetCompleteJmp(0xE9, 0x80C6BBF, (void*)gKItemBuySell.BuyItemHook);

	//foo_hook.Install((void*)0x80C6BC0, (void*)gKItemBuySell.BuyItemHook); // 0x80C6BC0 0x80C6BC4
	//foo_hook.Install((void*)0x80C6BBF, (void*)gKItemBuySell.BuyItemHook); // 0x80C6BC0

	//gUtil.SetCompleteHook(0xE8, 0x80C6BBF, &BuyItemHook); // loi nBuyIdx

	// -------------------------------------------------------

	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start;
	uintptr_t pagestart;
	////uintptr_t start = (uintptr_t)(void*)0x80C4CFB; // set rank

	start = (uintptr_t)(void*)0x80C0479; // set rank

	//uintptr_t start = (uintptr_t)(void*)0x80BF670;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}
	//*(BYTE*)(0x80CAB80) = -4;
	gUtil.SetCompleteHook(0xE9, start, &ProcessStallProtocolHook); // loi nBuyIdx

	// -------------------------------------------------------

	//size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x80D8E7F;
	 
	//uintptr_t start = (uintptr_t)(void*)0x80C4CFB; // set rank

	//start = (uintptr_t)(void*)0x80F2561; // KPlayerBuyItem

	start = (uintptr_t)(void*)0x80C6BBF;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &BuyItemHook);

	//================================================

	/*start = (uintptr_t)(void*)0x81FB4FF;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &BuySellPrintf);*/

	//gUtil.SetCompleteHook(0xE9, start, &KPlayerBuyItem); // loi nBuyIdx

	// -------------------------------------------------------
	// 
	//gUtil.SetByte(0x80927E9, 0x09); // loi nBuyIdx
	//mprotect((void*)0x80927E9, 0x1, 7);
	//memchr((void*)0x80927E9, 0x09, 1);
	//*(BYTE*)(0x80BF2A6) = 0;
	//gUtil.MemorySet(0x080927E9, 0x09, 0x1); // loi nBuyIdx
	//gUtil.SetCompleteHook(0xE9, 0x8092804, &GetEngergyDamageHookNew); // loi nBuyIdx
	//gUtil.SetJmp(0x8092804, (void*)GetEngergyDamageHook);
	//gUtil.SetCompleteHook(0xE8, 0x80C6BBF, (char*)gKItemBuySell.BuyItemHook);
	//gUtil.SetHook(0xE8, (char*)gKItemBuySell.BuyItemHook, 0x80C6BBF);
	//foo_hook.Install((void*)0x7887894, (void*)0x80C6BC0);

	//func_hook = subhook_new((void*)0x80c6aa0, (void*)*(BYTE*)BuyItemHook + 0xf78878a3, subhook_flags_t::SUBHOOK_64BIT_OFFSET);
	//func_hook = subhook_new((void*)0x80C6BBF, (char*)gKItemBuySell.BuyItemHook, subhook_flags_t::SUBHOOK_64BIT_OFFSET);
	//subhook_install(func_hook);
	// 
	//gUtil.SetCompleteHook(0x0E8, 0x80C6BC0, &BuyItemHook - 0x080C6BC4);
	//gUtil.SetCompleteHook(0x0E8, 0x80C6BC0, &BuyItemHook - 0x080C6BC4);
}

int __cdecl ProcessStallProtocolHook(KPlayerStall* This, BYTE* msg)
{
	try
	{
		
		//signal(11, sighandler);

		/*struct sigaction sa;
		struct sigaction old_action;

		memset(&sa, 0, sizeof(sa));
		memset(&old_action, 0, sizeof(old_action));
		sigemptyset(&sa.sa_mask);
		sa.sa_sigaction = sigaction_stall;
		sa.sa_flags = SA_SIGINFO;
		sigaction(SIGSEGV, &sa, NULL);*/
		//sigaction(SIGFPE, &sa, NULL);

		KIniFile FileHandle;
		Execute_KIniFile(&FileHandle);
		Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
		Execute_KIniFile_GetInteger(&FileHandle, "Setting", "Stall", 0, &gUtil.nStall);
		Execute_KIniFile_Clear(&FileHandle);

		if (gUtil.nStall == 1)
		{
			if (gUtil.PACKETFILTER_ACTIVE)
			{
				/*struct sigaction sa;

				memset(&sa, 0, sizeof(struct sigaction));
				sigemptyset(&sa.sa_mask);
				sa.sa_sigaction = player_handler;
				sa.sa_flags = SA_SIGINFO;
				sigaction(SIGSEGV, &sa, NULL);*/
				//DWORD curTime = time(0);

				BYTE nProtocol = msg[3];
				//int nPrice;

				//

				//KPlayer* Player;
				//Player = (KPlayer*)((char*)Player_Struct + 165956 * m_nPlayerIndex);
				//std::allocator<char> __a; // [esp+1Dh] [ebp-Bh] BYREF
				/*KPlayer* cPlayer;
				KPlayer* Player;
				if (!Player)
					Player = (KPlayer*)Player_Struct;
				cPlayer = &Player[m_nPlayerIndex];*/

				//Execute_ShutdownClient(&Player[m_nPlayerIndex]);


				//int pPlayerIndex;
				//const char* pAccName;
				//const char* pRoleName;
				//pPlayerIndex = *(int*)(&This->tmp[0xC0]);
				//pAccName = &This->tmp[0x264];
				//pRoleName = &This->tmp[0x25C88];
				// 
				//size_t lenght = sizeof(msg) / sizeof(BYTE);
				//int resul;
				int v2 = *((DWORD*)msg + 2);
				//int count;
				//
				//char* v3;
				//string str;

				//resul = -1;
				int m_nStallState = This->tmp[0x0];
				//nPrice = *((DWORD*)msg + 5);
				const int siz_ar = sizeof(msg) / sizeof(int);

				time_t now = time(0);
				struct tm* timenow = localtime(&now);
				gUtil.WriteLogStall("Stall Info %x - Time: %04d-%02d-%02d %02d:%02d:%02d, Count %d, %d - %s\n",
					nProtocol, timenow->tm_year + 1900,
					timenow->tm_mon + 1,
					timenow->tm_mday,
					timenow->tm_hour,
					timenow->tm_min,
					timenow->tm_sec, v2, m_nStallState, gUtil.hexStr((unsigned char*)msg, siz_ar).c_str());

				

				//if (nProtocol == 0)
				//{

				//	//printf("Protocol: %d\n", nProtocol);

				//	if (count > 0 && (unsigned int)(count - 1) <= 59)
				//	{
				//		for (int i = 0; i != count; ++i)
				//		{
				//			//gUtil.WriteLogFile("Stall Info 2 - Time: %u, nPlayerIndex: %d, msg: %d - %d %d %d\n",
				//			//	curTime, m_nPlayerIndex, lenght, count, *(_DWORD*)&msg[8 * i + 12], *(_DWORD*)&msg[8 * i + 16]);

				//			if (*(_DWORD*)&msg[8 * i + 12] <= 0 || *(_DWORD*)&msg[8 * i + 16] <= 0)
				//			{
				//				//gUtil.WriteLogStall("Stall Invalid - Time: %u, nPlayerIndex: %d, HardwareId: [ %s ], msg: %d, %d %d\n",
				//					//curTime, m_nPlayerIndex, m_KPlayerInfo[m_nPlayerIndex].m_Hwid, lenght, *(_DWORD*)&msg[8 * i + 12], *(_DWORD*)&msg[8 * i + 16]);
				//				//ShutdownClient(&Player[m_nPlayerIndex]);
				//				return -1;
				//			}
				//		}
				//	}
				//	else
				//	{
				//		//gUtil.WriteLogStall("Stall Overload Count - Time: %u, nPlayerIndex: %d, HardwareId: [ %s ], Count %d, %d %d %d %d %d - %d %d %d\n",
				//			//curTime, m_nPlayerIndex, m_KPlayerInfo[m_nPlayerIndex].m_Hwid, count, msg[0], msg[1], msg[2], msg[3], msg[4], This->tmp[0x0], v2, (unsigned int)(count - 1));
				//		return -1;
				//	}

				//}
				//else if (nProtocol == 1)
				//if (nProtocol == 0u)
				//{
				//	try
				//	{
				//		m_nPlayerIndex = This->tmp[0x4];

				//		/*gUtil.WriteLogStall("Stall Overload [ %d ] - Time: %u, nPlayerIndex: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ] , [ %d ], Count [ %d ], ShopName: [ %s ]\n",
				//			nProtocol, curTime, m_StallCheck[m_nPlayerIndex].m_nPlayerIndex
				//			, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
				//			, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
				//			, m_KPlayerInfo[m_nPlayerIndex].m_Hwid, m_StallCheck[m_nPlayerIndex].m_nError);*/

				//		if (!m_StallCheck[m_nPlayerIndex].m_nError && m_StallCheck[m_nPlayerIndex].m_nPlayerIndex > 0)
				//		{
				//			//Execute_ClearAllPrice(This);
				//			//Execute_
				// llInfo(This);
				//			m_StallCheck[m_nPlayerIndex].m_nPlayerIndex = 0;
				//			m_StallCheck[m_nPlayerIndex].m_nProtocol = 0;
				//			m_StallCheck[m_nPlayerIndex].m_nError = false;
				//			return Execute_ProcessStallProtocol(This, msg);
				//		}
				//		else
				//		{
				//			m_StallCheck[m_nPlayerIndex].m_nPlayerIndex = 0;
				//			m_StallCheck[m_nPlayerIndex].m_nProtocol = 0;
				//			m_StallCheck[m_nPlayerIndex].m_nError = false;
				//			//Execute_SetStallState(This, 0);
				//			return -1;
				//		}
				//	}
				//	catch (...)
				//	{
				//		return -1;
				//	}
				//}
				//else 

				if (msg[3] == 1u)
				{
					//KPlayer* Player = (KPlayer*)((char*)0x88A1A20 + 165956 * m_nPlayerIndex);
							//if (Player->tmp[0x20408] <= -1 && (int)Player->tmp[0x20400] <= 0 && Player->tmp[0x20404] <= 0)
							//{
							//	gUtil.WriteLogStall("Stall Index Error [ %d ] - Time: %u, nPlayerIndex: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ]\n", nProtocol, curTime, m_nPlayerIndex
							//		, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
							//		, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
							//		, m_KPlayerInfo[m_nPlayerIndex].m_Hwid);
							//	//Execute_ClearAllPrice(This);
							//	//Execute_SyncStallInfo(This);
							//	//Execute_SetStallState(This, 0);
							//	m_StallCheck[m_nPlayerIndex].m_nError = true;
							//	Execute_SetStallState(This, 0);
							//	return -1;
							//}
							//printf("Protocol: %d\n", nProtocol);
							//printf("Protocol: %d\n", nProtocol);
							//printf("PlayerIndex: %d\n", m_nPlayerIndex);

					int nCount;
					int result;
					int nSize = *((int*)msg + 2);
					nCount = *((DWORD*)msg + 2);
					//int nShopNameLen = strlen((char*)msg + 12);
					result = *(DWORD*)(msg + 8);

					//char byteData[nSize + 1];
					//memset(byteData, 0, nSize + 1);
					//printf("DATA 1: %d - %d\n", nSize, nShopNameLen);

					/*printf("1: %s\n", msg + 12);

					strcpy((char*)(msg + 12), "ONLY 9VP\001");

					printf("2: %s\n", msg + 12);*/

					////printf("nShopName: %s\n", (char*)msg + 12);
					//printf("nShopName: %s\n", (char*)&msg[nSize + 12]);

					//std::string sPacket = gUtil.hexStrStall((unsigned char*)((char*)msg + 12), strlen((char*)msg + 12));

					//if (sPacket.find("bug") != std::string::npos || sPacket.find("5c") != std::string::npos || sPacket.find("21") != std::string::npos || sPacket.find("26") != std::string::npos || sPacket.find("25") != std::string::npos)
					//{
					//	//printf("BUG 0: %s\n", sPacket.c_str());
					//	return -1;
					//}

					//printf("sPacket: %s\n", sPacket.c_str());

					//printf("nShopName: %s\n", gUtil.hex_to_string(std::string(sPacket.c_str()).c_str()).c_str());

					/*nShopNameLen = gUtil.hex_to_string(std::string(sPacket.c_str()).c_str()).size();

					if (nSize >= nShopNameLen)
						memcpy((char*)(msg + 12), gUtil.hex_to_string(std::string(sPacket.c_str()).c_str()).c_str(), nShopNameLen);
					else
						memcpy((char*)(msg + 12), gUtil.hex_to_string(std::string(sPacket.c_str()).c_str()).c_str(), nSize);*/


						//if (nShopNameLen != nSize || nShopNameLen >= 23 || nSize > 149 || nSize < 0)
						if (nSize > 149 || nSize < 0)
						{
							//printf("BUG 1: %d - %d - %d\n", nSize, nShopNameLen, *((BYTE*)msg + 11));
							//ShutdownClient(&Player[m_nPlayerIndex]);
							//m_StallCheck[m_nPlayerIndex].m_nError = true;
							return -1;
						}

						//if ((char*)msg + 12)
						//{
						//	strcpy((char*)&This->tmp[0x8], (char*)msg + 12);
						//}


						if (msg[0] >= 124 && msg[1] == 12 || msg[1] == 12 && msg[2] == 0)
						{
							//printf("BUG 2: %d - %d - %d\n", msg[0], msg[1], msg[2]);
							//ShutdownClient(&Player[m_nPlayerIndex]);
							//m_StallCheck[m_nPlayerIndex].m_nError = true;
							return -1;
						}

						/*char pShopName[nCount];
						memcpy(pShopName, (char*)msg + 12, nCount);
						pShopName[nCount] = 0;*/
						//printf("Count: %d\n", nCount);
						//printf("ShopName: %s\n", pShopName);
						//printf("Len ShopName: %d\n", strlen(pShopName));
						/*	string str;
							std::string::string<char*>(&str, (char*)msg + 12, (char*)&msg[v2 + 12], &__a);*/
							//

					if (result > 0)
					{
						result = *(DWORD*)(msg + 8);

						/*gUtil.WriteLogStall("Stall Info [ %d ] - Time: %u, nPlayerIndex: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], Count [ %d ], ShopName: [ %s ], Data: [ %s ]\n", nProtocol, curTime, m_nPlayerIndex
							, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
							, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
							, m_KPlayerInfo[m_nPlayerIndex].m_Hwid, *((DWORD*)msg + 2), (char*)msg + 12, (char*)&msg[*((DWORD*)msg + 2) + 12]);*/

						if (result <= 16)
							return Execute_ProcessStallProtocol(This, msg);
					}

					return result;




					//if (*(int*)&msg[nCount + 12] != nCount)
					//{
					//	//m_nPlayerIndex = *(int*)(&This->tmp[0x4]);

					//	//if (m_nPlayerIndex <= 0 || m_nPlayerIndex > gUtil.nMaxPlayer)
					//	//{
					//	//	//gUtil.WriteLogStall("Stall Overload PlayerIndex - Time: %u, nPlayerIndex: %d, MaxPlayer: %d\n",
					//	//		//curTime, m_nPlayerIndex, gUtil.nMaxPlayer);
					//	//	return -1;
					//	//}

					//	gUtil.WriteLogStall("Stall Error - Time: %u, nPlayerIndex: %d, Count [ %d ], ShopName: [ %s ], Data: [ %s ]\n",
					//		nProtocol, curTime, m_nPlayerIndex
					//		, nCount, (char*)msg + 12, (char*)&msg[nCount + 12]);

					//	/*gUtil.WriteLogStall("Stall Error - Time: %u, nPlayerIndex: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], Count [ %d ], ShopName: [ %s ]\n",
					//		nProtocol, curTime, m_nPlayerIndex
					//		, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
					//		, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
					//		, m_KPlayerInfo[m_nPlayerIndex].m_Hwid
					//		, v2, (char*)msg + 12);*/
					//	m_StallCheck[m_nPlayerIndex].m_nError = true;
					//	return -1;
					//}

					//printf("1: %s\n", msg + 12);

					//strcpy((char*)(msg + 12), "ONLY 9VP");

					//printf("2: %s\n", msg + 12);



					/*if (sPacket.substr(sPacket.size() - 2, 2).find("0") != std::string::npos)
					{
						gUtil.WriteLogStall("Stall Error - Time: %u, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ] \n", curTime, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
							, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
							, m_KPlayerInfo[m_nPlayerIndex].m_Hwid);
						return -1;
					}*/

					//int v3 = atoi(sPacket.substr((strlen((char*)msg + 12) * 2) - 2, 2).c_str());

					//printf("v3: %d , %d , %d , %s , %s\n", v3, nCount, sPacket.size(), sPacket.substr((nCount * 2) - 2, 2).c_str(), sPacket.c_str());
					//printf("3: %d\n", (sPacket.size() / 2) < (float)nCount);



					/*((char*)msg + 12)[nCount - 1] = 0;

					if ((sPacket.size() / 2) < (float)nCount)
					{
						gUtil.WriteLogStall("Stall Error - Time: %u, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ] \n", curTime, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
							, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
							, m_KPlayerInfo[m_nPlayerIndex].m_Hwid);
						return -1;
					}*/

					/*if (v3 < 20)
					{
						gUtil.WriteLogStall("Stall Error - Time: %u, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ] \n", curTime, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
							, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
							, m_KPlayerInfo[m_nPlayerIndex].m_Hwid);
						return -1;
					}*/

					/*if (sPacket.find("00") != std::string::npos)
					{
						gUtil.WriteLogStall("Stall Error - Time: %u, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ] \n", curTime, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
							, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
							, m_KPlayerInfo[m_nPlayerIndex].m_Hwid);
						return -1;
					}
					else if (sPacket.find("ff") != std::string::npos)
					{
						gUtil.WriteLogStall("Stall Error - Time: %u, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ] \n", curTime, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
							, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
							, m_KPlayerInfo[m_nPlayerIndex].m_Hwid);
						return -1;
					}*/

					//return Execute_ProcessStallProtocol(This, msg);
					/*else if (strlen(pShopName) <= 6 || strlen(pShopName) > 16)
					{
						gUtil.WriteLogStall("Stall Overload - Time: %u, nPlayerIndex: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], Count [ %d ], ShopName: [ %s ]\n",
							nProtocol, curTime, m_nPlayerIndex
							, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
							, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
							, m_KPlayerInfo[m_nPlayerIndex].m_Hwid, nCount, pShopName);

						return -1;
					}*/
				}
				//else if (nProtocol == 3u)
				//{
				//	if (This->tmp[0x4] != NULL)
				//		m_nPlayerIndex = This->tmp[0x4];

				//	/*gUtil.WriteLogStall("Stall Overload [ %d ] - Time: %u, nPlayerIndex: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ] , [ %d ]\n",
				//		nProtocol, curTime, m_StallCheck[m_nPlayerIndex].m_nPlayerIndex
				//		, m_KPlayerInfo[m_nPlayerIndex].m_AccountName
				//		, m_KPlayerInfo[m_nPlayerIndex].m_PlayerName
				//		, m_KPlayerInfo[m_nPlayerIndex].m_Hwid, m_StallCheck[m_nPlayerIndex].m_nError);*/

				//	if (!m_StallCheck[m_nPlayerIndex].m_nError && m_StallCheck[m_nPlayerIndex].m_nPlayerIndex > 0)
				//	{
				//		//Execute_ClearAllPrice(This);
				//		//Execute_SyncStallInfo(This);
				//		m_StallCheck[m_nPlayerIndex].m_nPlayerIndex = 0;
				//		m_StallCheck[m_nPlayerIndex].m_nProtocol = 0;
				//		m_StallCheck[m_nPlayerIndex].m_nError = false;
				//		return Execute_ProcessStallProtocol(This, msg);
				//	}
				//	else
				//	{
				//		m_StallCheck[m_nPlayerIndex].m_nPlayerIndex = 0;
				//		m_StallCheck[m_nPlayerIndex].m_nProtocol = 0;
				//		m_StallCheck[m_nPlayerIndex].m_nError = false;
				//		return Execute_SetStallState(This, 0);
				//	}
				//}
				else
					Execute_ProcessStallProtocol(This, msg);
			}
			else
			{
				return Execute_ProcessStallProtocol(This, msg);
			}
		}
		else if (gUtil.nStall == 2)
		{
			return Execute_ProcessStallProtocol(This, msg);
		}
		else
			return -1;

		//if (lenght <= 0 || lenght > 27 || msg[4] > 32)
		//{
		//	gUtil.WriteLogFile("Stall Invalid - Time: %u, msg: %d - %d %d %d %d\n",
		//		curTime, lenght, msg[1], msg[2], msg[3], msg[4]);
		//	/*gUtil.WriteLogFile("Protocol Invalid - Time: %u, pProtocol: %p - %d - %d - %d - %d\n",
		//		curTime, (DWORD*)pProtocol, *((DWORD*)pProtocol + 1), pProtocol[3], pProtocol[2], pProtocol[1]);*/
		//	//ShutdownClient(&Player[nPlayerIdx]);
		//	return -1;
		//}
		//else
		//	return oProcessStallProtocol(This, msg);
	}
	catch (...)
	{
		return Execute_ProcessStallProtocol(This, msg);
	}
}

void __cdecl KPlayerBuyItem(KPlayer* This, BYTE* pProtocol)
{
	DWORD curTime = time(0);
	std::time_t result = std::time(nullptr);
	std::string res = std::asctime(std::localtime(&result));
	std::string::size_type i = res.find("\n");
	if (i != std::string::npos)
		res.erase(i, res.length());
	size_t lenght = sizeof(pProtocol) / sizeof(BYTE);
	int resul;
	int nIndex = (int)This->tmp[0xC0];
	int nMaxPlayer;
	//KPlayer* cPlayer;
	//if (!Player)
	//	Player = (KPlayer*)Player_Struct;
	//cPlayer = &Player[This->tmp[0xC0]];
	//KItemList* ItemList = (KItemList*)&This->tmp[0x2040C];
	//KBuySell* pBuySell;
	//gUtil.MemoryCpy(pBuySell, (void*)0x949B1A0,sizeof(KBuySell));
	//if (!pBuySell)
	//	pBuySell = (KBuySell*)0x949B1A0;

	//pthread_t thread_id;
	//pthread_create(&thread_id, NULL, &ThreadWriteLog, NULL);


	//KPlayer* cPlayer;
	//KPlayer* Player;
	//if (!Player)
	//	Player = (KPlayer*)Player_Struct;
	//cPlayer = &Player[This->tmp[0xC0]];

	//gUtil.WriteLogFile("BuyItem\t-\t[%s]\t[%d]\t[%d] - %u, %u, %u - %u - %u, %u, %u, %u, %u, %u, %u, %u, %u\n",
	//	res.c_str(), lenght, nIndex, This->tmp[0x20400], This->tmp[0x20404], This->tmp[0x20408], *((DWORD*)pProtocol), *((DWORD*)pProtocol + 1), pProtocol[0], pProtocol[1], pProtocol[2], pProtocol[3], pProtocol[4], pProtocol[5], pProtocol[6], pProtocol[7]);

	//if (This->tmp[0x20404] < 0)
	//{
	//	//ShutdownClient(&Player[This->tmp[0xC0]]);
	//	return;
	//}

	//if (This->tmp[0x20408] < 0)
	//{
	//	//ShutdownClient(&Player[This->tmp[0xC0]]);
	//	return;
	//}

	//if (This->tmp[0xC0] == NULL || This->tmp[0xC0] <= 0 || This->tmp[0xC0] >= gUtil.nMaxPlayer)
	//{
	//	ShutdownClient(&Player[This->tmp[0xC0]]);
	//	return;
	//}

	try
	{
		oBuyItem(This, pProtocol);
	}
	catch (...)
	{
	}

	//if (nIndex > 0 && nIndex < gUtil.nMaxPlayer)
	//{
	//	oBuyItem(This, pProtocol);
	//	//ShutdownClient(&Player[This->tmp[0xC0]]);
	//	//return;
	//}
	//else
	//{
	//	gUtil.WriteLogFile("Protocol Invalid - Time: %u, pProtocol: %p - %d - %d - %d - %d\n",
	//	curTime, (DWORD*)pProtocol, *((DWORD*)pProtocol + 1), pProtocol[3], pProtocol[2], pProtocol[1]);
	//	return;
	//}
	/*try
	{
		

		
	}
	catch (std::out_of_range& ex)
	{
		gUtil.WriteLogFile("BuyItem Out Of Range\t-\t[%s]\t%d\n",
			res.c_str(), lenght);
		return;
	}*/

	//if (This->tmp[0x20408] > This->tmp[0xC0])
	//{
	//	ShutdownClient(&Player[This->tmp[0xC0]]);
	//	return;
	//}

	//BuyItemHook(BuySell, This->tmp[0xC0], ItemList->tmp[0x4DFC], *((DWORD*)pProtocol + 1), pProtocol[3], pProtocol[2], pProtocol[1]);
	

	//if (lenght <= 0 || lenght > 27 || pProtocol[4] > 32)
	//{
	//	gUtil.WriteLogFile("Protocol Invalid - Time: %u, pProtocol: %p - %d - %d - %d - %d\n",
	//		curTime, (DWORD*)pProtocol, *((DWORD*)pProtocol + 1), pProtocol[3], pProtocol[2], pProtocol[1]);
	//	//ShutdownClient(&Player[nPlayerIdx]);
	//	return;
	//}
	//else
	//	return oBuyItem(This, pProtocol);
}

void __cdecl BuySellPrintf(KSG_LogFile* This, const char* pcszFmt, ...)
{
	//runScript = Execute_ExecuteScript_Name(This, gUtil.nScriptPickUpMoney, "PickUp", 1, "dd", pPlayerIndex, nMoney);
	va_list aptr;
	va_start(aptr, pcszFmt);
	printf(pcszFmt);
	Execute_BuySellPrintf(This, pcszFmt, aptr);
	va_end(aptr);
}

BOOL __cdecl BuyItemHook(KBuySell *pBuySell, int nPlayerIdx, int nBuy, int nBuyIdx, BYTE nPlace, BYTE nX, BYTE nY)
{
	if (gUtil.PACKETFILTER_ACTIVE)
	{
		BOOL result = 0;
		//DWORD curTime = time(0);
		std::time_t time_result = std::time(nullptr);
		std::string time_res = std::asctime(std::localtime(&time_result));
		std::string::size_type i = time_res.find("\n");
		if (i != std::string::npos)
			time_res.erase(i, time_res.length());
		int v8 = nBuy;
		int v9 = nBuyIdx;
		int m_MaxPlayer = 0;
		int curCurrencyType = 0;
		int pPlayerIndex;
		const char* pAccName;
		const char* pRoleName;
		pPlayerIndex = nPlayerIdx;
		/*INIReader reader("./servercfg.ini");

		if (reader.ParseError() < 0)
		{
			m_MaxPlayer = 600;
		}
		else
		{
			m_MaxPlayer = (int)reader.GetInteger("Overload", "MaxPlayer", 600);
		}*/
		//= (KSOServer*)0x82492C0
		//int aMaxplayer = 500;
		//char m_AccountName[32];
		//char m_PlayerName[32];
		//printf("+-%d-%d-+\n", oGetExtPoint(&Player[nPlayerIdx], 1), oGetEngergyDamage(&Player[nPlayerIdx]), *(BYTE*)Player);
		//strcpy((char*)m_AccountName, (std::string*)&Player[nPlayerIdx].tmp[0x264]);
		//strcpy((char*)m_PlayerName, (std::string*)&Player[nPlayerIdx].tmp[0x25C88]);
		//printf("+-%s-%s-+\n", (char*)Player[nPlayerIdx].m_nPlayerIndex, (char*)Player[nPlayerIdx].m_nIndex);
		//printf("+-%s-%s-+\n", *(WORD*)(Player[nPlayerIdx].m_SaveBuffer + 293), *(WORD*)(Player[nPlayerIdx].m_SaveBuffer + 199));
		//*(DWORD*)(Player[nPlayerIdx].m_SaveBuffer + 293) = 1;
		//if (nPlayerIdx == NULL || nBuy == NULL || nBuyIdx == NULL)
		//if (nPlayerIdx == NULL || nBuy == NULL || nBuyIdx == NULL)

		if (nPlayerIdx <= 0 || nPlayerIdx > gUtil.nMaxPlayer)
		{
			gUtil.WriteLogFile("Invalid Overload - Time:%s\tnPlayerIdx:%d, nMaxPlayer:%d\n",
				time_res.c_str(), nPlayerIdx, gUtil.nMaxPlayer);
			return 0;
		}

		/*KPlayer* CurrenPlayer;

		if (!Player)
			Player = (KPlayer*)0x88A1A20;

		CurrenPlayer = (KPlayer*)((char*)(DWORD*)Player + 165956 * pPlayerIndex);

		pAccName = &CurrenPlayer->tmp[0x264];
		pRoleName = &CurrenPlayer->tmp[0x25C88];*/
		
		if (gUtil.nLogShop > 0)
		{
			gUtil.WriteLogFile("Info\t-\t[%s]\tnPlayerIdx: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], nBuy:%d, nBuyIdx:%d, nPlace:%u, nX:%u, nY:%u, m_Width:%u, m_Height:%u\n",
				time_res.c_str(),
				pPlayerIndex,
				m_KPlayerInfo[pPlayerIndex].m_AccountName,
				m_KPlayerInfo[pPlayerIndex].m_PlayerName,
				m_KPlayerInfo[pPlayerIndex].m_Hwid,
				nBuy,
				nBuyIdx,
				nPlace,
				nX,
				nY,
				pBuySell->m_Height,
				pBuySell->m_Width);
		}
		
		 
		//gUtil.WriteLogFile("nPlayerIdx OK \n");
		//KPlayer* cPlayer;
		//KPlayer* Player;
		//if (!Player)
			//Player = (KPlayer*)Player_Struct;
		////cPlayer = &Player[nPlayerIdx];
		//char* cPlayer = (char*)&Player + 165956 * nPlayerIdx;
		//if (&Player[nPlayerIdx] == NULL || !&Player[nPlayerIdx])
		//{
		//	return false;
		//}
		//gUtil.WriteLogFile("KPlayer OK \n");
		//curCurrencyType = 0;
		//curCurrencyType = *((DWORD*)&Player[nPlayerIdx].tmp[152128]);
		//curCurrencyType = *((CurrencyType*)cPlayer + 38032);
		//int curCurrencyType = *(DWORD*)&cPlayer->tmp[38032];
		//int curCurrencyType = (int)&cPlayer->tmp[38032];
		//int curCurrencyType = *(DWORD*)&cPlayer->tmp[0x2522C];
		//gUtil.WriteLogFile("CurrencyType 1 %d\n", &Player[nPlayerIdx].tmp[152128]);
		//if (!&Player[nPlayerIdx].tmp[152128] || &Player[nPlayerIdx].tmp[152128] == NULL)
		//{
		//	gUtil.WriteLogFile("Player Error\n");
		//	/*if (nPlayerIdx < 0 || nPlayerIdx > 2000)
		//	{
		//		ShutdownClient(&Player[nPlayerIdx]);
		//	}*/
		//	//v8 = nBuyIdx >> 20;
		//	//v9 = nBuyIdx << 12 >> 12;
		//	//gUtil.WriteLogFile("Invalid CurrencyType 2\t-\t[%s]\tnPlayerIdx:%d, nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
		//	//	time_res.c_str(), nPlayerIdx, nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
		//	//ShutdownClient(&Player[nPlayerIdx]);
		//	return false;
		//}
		//else
		//{
		//	gUtil.WriteLogFile("Start curCurrencyType\n");
		//	try
		//	{
		//		curCurrencyType = *(DWORD*)&Player[nPlayerIdx].tmp[152128];
		//		gUtil.WriteLogFile("CurrencyType 2 %d\n", curCurrencyType);
		//	}
		//	catch (std::exception const& e)
		//	{
		//		//std::cerr << "Error: " << e.what() << std::endl;
		//		gUtil.WriteLogFile("Error:\t%s\n", e.what());
		//		return false;
		//	}
		//}
		//int v7 = *(DWORD*)&Player->tmp[0x25C88] + 165956 * nPlayerIdx;
		//int m_nBuyIdx1 = *(DWORD*)&cPlayer->tmp[0x2522C + 0x4];
		//int m_nBuyIdx2 = *(DWORD*)&cPlayer->tmp[0x2522C + 0x8];
		//int m_nBuyIdx3 = *(DWORD*)&cPlayer->tmp[0x2522C + 0xC];
		//int m_nBuyIdx1 = *(DWORD*)&cPlayer->tmp[0x2522C + 1];
		//int m_nBuyIdx2 = *(DWORD*)&cPlayer->tmp[0x2522C + 2];
		//int m_nBuyIdx3 = *(DWORD*)&cPlayer->tmp[0x2522C + 3];
		//int m_nBuyIdx = *(DWORD*)&cPlayer->tmp[0x2522C + 0x10];
		//int m_nPrice = *(DWORD*)&cPlayer->tmp[0x2522C + 0x14];
		//printf("+-%d-%d-%d-%d-%d-%d-+\n", m_nBuyIdx1, m_nBuyIdx2, m_nBuyIdx3, curCurrencyType, m_nBuyIdx, m_nPrice);
		//if (curCurrencyType == 3)
		//{
		//	v8 = nBuyIdx >> 20;
		//	v9 = nBuyIdx << 12 >> 12;
		//}
		//gUtil.WriteLogFile("CurrencyType OK \n");
		//gUtil.WriteLogFile("CurrencyType %d\n", curCurrencyType);

		//if (curCurrencyType < 0)
		//{
			//gUtil.WriteLogFile("CurrencyType Error\n");
			/*if (nPlayerIdx < 0 || nPlayerIdx > 2000)
			{
				ShutdownClient(&Player[nPlayerIdx]);
			}*/
			//v8 = nBuyIdx >> 20;
			//v9 = nBuyIdx << 12 >> 12;
			//gUtil.WriteLogFile("Invalid CurrencyType 2\t-\t[%s]\tnPlayerIdx:%d, nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
			//	time_res.c_str(), nPlayerIdx, nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
			//ShutdownClient(&Player[nPlayerIdx]);
			//return false;
		//}

		//if (curCurrencyType == CURRENCYTYPE_COIN)
		//{
		//	v8 = nBuyIdx >> 20;
		//	v9 = nBuyIdx << 12 >> 12;
		//}

		//gUtil.WriteLogFile("CurrencyType OK \n");

		//if (nBuyIdx > 100000000 && nBuy < 141 || nBuy > 141)
		if (nBuyIdx > 100000000)
		{
			v8 = nBuyIdx >> 20;
			v9 = nBuyIdx << 12 >> 12;
			//gUtil.WriteLogFile("Invalid CurrencyType \n");

			//if (nBuy < 100 && nBuy > 102)
			//{
			//	gUtil.WriteLogFile("Invalid CurrencyType\t-\t[%s]\tnPlayerIdx:%d, nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
			//		time_res.c_str(), nPlayerIdx, nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
			//	//ShutdownClient(&Player[nPlayerIdx]);
			//	return 0;
			//}

			//if (nBuy < 199 || nBuy > 202)
			//{
			//	gUtil.WriteLogFile("Invalid CurrencyType\t-\t[%s]\tnPlayerIdx:%d, nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
			//		time_res.c_str(), nPlayerIdx, nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
			//	//ShutdownClient(&Player[nPlayerIdx]);
			//	return 0;
			//}

			//if (nBuy < 139 || nBuy > 143)
			//{
			//	gUtil.WriteLogFile("Invalid CurrencyType\t-\t[%s]\tnPlayerIdx:%d, nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
			//		time_res.c_str(), nPlayerIdx, nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
			//	//ShutdownClient(&Player[nPlayerIdx]);
			//	return 0;
			//}
			
			// huyenthoaivlus
			 //if (nBuy != 182 & nBuy != 140 & nBuy != 151 & nBuy != 144)
			 // 
			// vo lam sieu pham
			if (nBuy < gUtil.nBuySellID - 2 || nBuy > gUtil.nBuySellID + 10)
			{
				gUtil.WriteLogFile("Invalid CurrencyType\t-\t[%s]\tnPlayerIdx: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
					time_res.c_str(), nPlayerIdx, 
					m_KPlayerInfo[nPlayerIdx].m_AccountName,
					m_KPlayerInfo[nPlayerIdx].m_PlayerName,
					m_KPlayerInfo[nPlayerIdx].m_Hwid, 
					nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
				//ShutdownClient(&Player[nPlayerIdx]);
				return 0;
			}

			/*if (nBuy != 200)
			{
				gUtil.WriteLogFile("Invalid CurrencyType - Time:%u\tnPlayerIdx:%d, nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
					curTime, nPlayerIdx, nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
				ShutdownClient(&Player[nPlayerIdx]);
				return false;
			}*/
		}
		/*	else if (nX <= 0 && nY <= 0)
			{
				gUtil.WriteLogFile("Invalid X-Y\t-\t[%s]\tnPlayerIdx:%d, nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
					time_res.c_str(), nPlayerIdx, nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
				ShutdownClient(&Player[nPlayerIdx]);
				return false;
			}*/
			//gUtil.WriteLogFile("Invalid CurrencyType OK \n");
			//v12 = this->m_ShopGoods[v8][v7];
		if (nPlayerIdx < 0
			|| nBuy < 0
			|| nBuyIdx < 0
			|| v8 < 0
			|| v9 < 0
			|| v8 >= pBuySell->m_Height
			|| pBuySell->m_Width <= v9
			|| nX > 7u
			|| nX < 0
			|| nY > 0xCu
			|| nY < 0
			|| nPlace > 0xBBu
			|| nPlace <= 0)
		{
			gUtil.WriteLogFile("Invalid\t-\t[%s], nPlayerIdx: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
				time_res.c_str(), nPlayerIdx, 
				m_KPlayerInfo[pPlayerIndex].m_AccountName,
				m_KPlayerInfo[pPlayerIndex].m_PlayerName, 
				m_KPlayerInfo[nPlayerIdx].m_Hwid, 
				nBuy, nBuyIdx, curCurrencyType, nPlace, nX, nY);
			//ShutdownClient(&Player[nPlayerIdx]);
			//result = 0;
			return 0;
		}
		else
		{
			/*gUtil.WriteLogFile("OK\t-\t[%s]\tnPlayerIdx: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], nBuy:%d, nBuyIdx:%d, CurrencyType:%d, nPlace:%u, nX:%u , nY:%u\n",
				time_res.c_str(),
				nPlayerIdx,
				m_KPlayerInfo[pPlayerIndex].m_AccountName,
				m_KPlayerInfo[pPlayerIndex].m_PlayerName,
				m_KPlayerInfo[nPlayerIdx].m_Hwid,
				nBuy,
				nBuyIdx,
				curCurrencyType,
				nPlace,
				nX,
				nY);*/
			return BuyItem(pBuySell, nPlayerIdx, nBuy, nBuyIdx, nPlace, nX, nY);
		}
	}
	else
	{
		return BuyItem(pBuySell, nPlayerIdx, nBuy, nBuyIdx, nPlace, nX, nY);
	}
}

void* ThreadWriteLog(void* ptr)
{

}

//void g_AssertFailed(LPSTR pFileName, int nLineNum)
//{
//	char szMsg[256];
//	sprintf(szMsg, "Assert failed in %s, line = %i", pFileName, nLineNum);
//	g_DebugLog(szMsg);
//	g_MessageBox(szMsg);
//	exit(1);
//}
#pragma once
#include <map>
#include "stdafx.h"
typedef int BOOL;
enum CurrencyType
{
	CURRENCYTYPE_NONE = 0x0,
	CURRENCYTYPE_MONEY = 0x1,
	CURRENCYTYPE_FUYUAN = 0x2,
	CURRENCYTYPE_COIN = 0x3,
	CURRENCYTYPE_SCORE = 0x4,
	CURRENCYTYPE_GOLDCOIN = 0x5,
	CURRENCYTYPE_SLIVER = 0x6,
	CURRENCYTYPE_TICKET = 0x7,
	CURRENCYTYPE_YUANBAO = 0x8,
	CURRENCYTYPE_GOLDCOIN_NEW = 0x9,
	CURRENCYTYPE_CONTRIBUTION = 0xA,
	CURRENCYTYPE_HONOUR = 0xB,
	CURRENCYTYPE_GOLDCOIN_SCORE = 0xC,
	CURRENCYTYPE_RESPECT = 0xD,
	CURRENCYTYPE_REFINING = 0xE,
	CURRENCYTYPE_ENERGY = 0xF,
	CURRENCYTYPE_ARENA_CREDITS = 0x10,
	CURRENCYTYPE_END = 0x11,
};

struct BuySellInfo
{
	int m_nBuyIdx;
	DWORD m_SubWorldID;
	int m_nMpsX;
	int m_nMpsY;
	int m_nScale;
	CurrencyType m_ctCurrencyType;
	int m_nCurGoodIdx;
	int m_nRestGoodCount;
	int m_nPrice;
	int m_nPlace;
	int m_nX;
	int m_nY;
	int m_nConsumeOldGoldCoin;
	int m_nConsumeNewGoldCoin;
	DWORD m_dwCallBackScriptID;
	char m_szCallBackFunc[100];
	BOOL m_bDynamicShop;
};

typedef struct _SYSTEMTIME
{
	WORD wYear;
	WORD wMonth;
	WORD wDayOfWeek;
	WORD wDay;
	WORD wHour;
	WORD wMinute;
	WORD wSecond;
	WORD wMilliseconds;
}   SYSTEMTIME;

struct KNpcTemplate
{
	char tmp[0x113C];
};

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

//static void
//handler(int signalNum, siginfo_t* si, void* unused)
//{
//	signal(SIGINT, SIG_DFL);
//	//printf("Got SIGSEGV at address: %p\n", si->si_addr);
//	//exit(EXIT_FAILURE);
//	throw(signalNum);
//	//exit(signal);
//}

//int main(int argc, char** argv)
//{
//	int sock, bytes_read, addrlen, i = 0;
//	char recv_data[1024], send_data[1024], ca[1024];
//	struct sockaddr_in server_addr, client_addr;
//	int clients[2] = { 0,0 };
//	struct sockaddr_in clientsAddr[2];
//	int yes = 1;
//
//	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
//	{
//		perror("Socket Creation Error");
//		exit(1);
//	}
//
//	addrlen = sizeof(client_addr);
//
//	server_addr.sin_family = AF_INET;
//	server_addr.sin_port = htons(5000);
//	server_addr.sin_addr.s_addr = INADDR_ANY;
//	bzero(&(server_addr.sin_zero), sizeof(server_addr));
//
//	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (char*)&yes, sizeof(int)) < 0)
//	{
//		perror("Reuse option\n");
//		close(sock);
//		exit(1);
//	}
//
//	if (bind(sock, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) == -1)
//	{
//		perror("Bind Error");
//		close(sock);
//		exit(1);
//	}
//
//	printf("\n UDP Server waiting for client on port 5000 \n");
//	fflush(stdout);
//	while (1)
//	{
//		int sender = -1;
//		int receiver = -1;
//		printf("I : %d\n", i);
//		printf("Before recv\n");
//		bytes_read = recvfrom(sock, ca, 1024, 0, (struct sockaddr*)&client_addr, &addrlen);
//		printf("Received from Client port : %d\nString : %s\n", client_addr.sin_port, ca);
//		if (i < 2)
//		{
//			if ((clientsAddr[0].sin_addr.s_addr == client_addr.sin_addr.s_addr && clientsAddr[0].sin_port == client_addr.sin_port) ||
//				(clientsAddr[1].sin_addr.s_addr == client_addr.sin_addr.s_addr && clientsAddr[1].sin_port == client_addr.sin_port))
//			{
//				continue;
//			}
//			else
//			{
//				if (clients[0] == 0) { clients[0] = 1; memcpy(&clientsAddr[0], &client_addr, sizeof(client_addr)); i++; }
//				else if (clients[1] == 0) { clients[1] = 1; memcpy(&clientsAddr[1], &client_addr, sizeof(client_addr)); i++; }
//				printf("Client Connected\n");
//			}
//		}
//		if ((clientsAddr[0].sin_addr.s_addr == client_addr.sin_addr.s_addr && clientsAddr[0].sin_port == client_addr.sin_port))
//			sender = 0;
//		else
//			sender = 1;
//		receiver = (sender == 0) ? 1 : 0;
//		printf("Sender : %d __ Receiver : %d\n", sender, receiver);
//		if (i == 2 && sender >= 0 && receiver >= 0) // 2 clients connected
//		{
//			if ((strcmp(ca, "q") == 0) || strcmp(ca, "Q") == 0)
//				break;
//			else
//				printf("Received : %s\n", ca);
//			sendto(sock, ca, strlen(ca) + 1, 0, (struct sockaddr*)&clientsAddr[receiver], addrlen);
//			sender = -1; receiver = -1;
//		}
//	}
//	return 0;
//}
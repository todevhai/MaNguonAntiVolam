#pragma once
#include "KPlayerNew.h"
#include <signal.h>

struct KBuySell
{
	char tmp[11];
	int m_ShopGoods;
	int m_Goods;
	int m_Width;
	int m_Height;
	int m_MaxGoods;
	int m_mapGoodsName;
	char tmp2[592];
};

struct PlayerStallCheck
{
	int m_nPlayerIndex;
	BYTE m_nProtocol;
	bool m_nError;
};

//typedef struct __attribute__((packed)) __attribute__((aligned(1))) KItemList

struct KSG_LogFile
{
	//BYTE tmp[0x4E20];
	//BYTE tmp[0x4C28];
	//KInventory m_Room;
	//BYTE tmp2[(0x4E20 - 0x4C2C)];
	char tmp[0x234];
};


class KItemBuySell
{
public:
	KItemBuySell ();
	virtual ~KItemBuySell();
	void Hook();
private:

};
extern KItemBuySell gKItemBuySell;
extern PlayerStallCheck	m_StallCheck[1200];
//ENGINE_API void g_AssertFailed(char* FileName, int LineNum);
void* ThreadWriteLog(void*);
int __cdecl ProcessStallProtocolHook(KPlayerStall* This, BYTE* msg);
void __cdecl KPlayerBuyItem(KPlayer* This, BYTE* pProtocol);
void __cdecl BuySellPrintf(KSG_LogFile* This, const char* pcszFmt, ...);
BOOL __cdecl BuyItemHook(KBuySell* pBuySell, int nPlayerIdx, int nBuy, int nBuyIdx, BYTE nPlace, BYTE nX, BYTE nY);
typedef bool(*_BuyItem)(KBuySell*, int, int, int, BYTE, BYTE, BYTE);
typedef void(*_NewBuyItem)(KPlayer*, BYTE*);
typedef int(*_ProcessStallProtocol)(KPlayerStall*, BYTE*);
time_t GetTickCount();

static void sigaction_stall(int signal, siginfo_t* si, void* arg);
int function_throws_int(int m_nPlayerIndex);
int function_throws_string();
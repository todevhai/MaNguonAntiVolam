#include "Protocol.h"
#include "Util.h"

#define Execute_InitCoreAfterRelayData		((void(__cdecl*)(KTongProcess *This)) 0x81F3780)//0x0063A578 
#define Execute_ProcessGMCommand		((bool(__cdecl*)(int nPlayerIdx, EGameMasterCommand eCommand, const char* pParam, int nLen)) 0x8202DA0)//0x0063A578 

bool IsInitCoreAfterRelayData = false;

//bool __cdecl ProcessGMCommand(int nPlayerIdx, EGameMasterCommand eCommand, const char* pParam, int nLen)
//{
//	//Execute_ProcessGMCommand(nPlayerIdx, eCommand, pParam, nLen);
//	return 0;
//}

bool __cdecl CheckRight(KTongLogic* This, DWORD dwTongID, DWORD dwMemberID, unsigned int RightID)
{
	DWORD curTime = time(0);
	gUtil.WriteLogFile("SPAM Core Server - Time: %u\n", curTime);
	return 0;
}

void __cdecl InitCoreAfterRelayData(KTongProcess* This)
{
	DWORD curTime = time(0);
	//gUtil.WriteLogFile("SPAM Core Server - Time: %u %d\n", curTime, IsInitCoreAfterRelayData);
	if (!IsInitCoreAfterRelayData)
	{
		Execute_InitCoreAfterRelayData(This);
		IsInitCoreAfterRelayData = true;
	}
	else
	{
		gUtil.WriteLogFile("SPAM Core Server - Time: %u\n", curTime);
	}
	//gUtil.WriteLogFile("SPAM Core Server - Time: %u %d\n", curTime, IsInitCoreAfterRelayData);
}
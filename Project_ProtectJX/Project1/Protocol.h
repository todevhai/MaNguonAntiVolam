#pragma once
#include "stdafx.h"
#include <unistd.h>
#include <stdlib.h>
#include "KPlayerNew.h"
#pragma pack(1)
struct HEAD_CENTER
{
	void set(BYTE head, BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->key = rand() % 256;
		this->head = head;
	}

	BYTE type;
	BYTE size;
	BYTE key;
	BYTE head;
};

struct BISHOP_GAMESERVER_INFO_SEND
{
	HEAD_CENTER header; // C1:00
	int m_ServerID;
	int m_Type;
	char m_TextData[32];
	BYTE m_ByteData[256];
};
#pragma pack(0)

//struct KTongLogic
//{
//	char tmp[0x20];
//};

//enum EGameMasterCommand : __int32
//{
//	GMDoScriptAction = 0x0,
//	GMDoWorldScriptAction = 0x1,
//	GMRunScriptFile = 0x2,
//	GMReloadScriptFile = 0x3,
//	GMReloadAllScriptFile = 0x4,
//	GMDoWorldScriptFileAction = 0x5,
//};


struct KTongProcess
{
	char tmp[0x1C];
};

extern bool IsInitCoreAfterRelayData;

bool __cdecl ProcessGMCommand(int nPlayerIdx, EGameMasterCommand eCommand, const char* pParam, int nLen);
bool __cdecl CheckRight(KTongLogic* This, DWORD dwTongID, DWORD dwMemberID, unsigned int RightID);
void __cdecl InitCoreAfterRelayData(KTongProcess* This);

class Protocol
{
};


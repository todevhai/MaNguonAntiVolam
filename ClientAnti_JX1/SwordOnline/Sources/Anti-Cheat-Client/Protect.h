// Protect.h: interface for the CProtect class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "Message.h"

struct MAIN_FILE_INFO
{
	char AuthIpAddress[32];
	char AuthServerSec[32];
	WORD AuthServerPort;
	//char LicenseId[33];
	WORD ServerPort;
	char ServerName[32];
	char AccountWebsite[50];
	char IpAddress[32];
	DWORD ClientVersion;
	char ClientSerial[33];
	DWORD nAntiDump;
	DWORD nThanPhap;
	DWORD nWorldRank;
	DWORD nProtocolVersion;
	DWORD nLimit;
	DWORD Verify;
	char ClientName[32];
	char PluginName[32];
	char VerifyName[32];
	DWORD ClientCRC32;
	DWORD PluginCRC32;
	DWORD VerifyCRC32;
	MESSAGE_INFO EngMessageInfo[MAX_MESSAGE];
	MESSAGE_INFO PorMessageInfo[MAX_MESSAGE];
	MESSAGE_INFO SpnMessageInfo[MAX_MESSAGE];
	//char AuthIpAddressUS[32];
};

class CProtect
{
public:
	CProtect();
	virtual ~CProtect();
	bool ReadMainFile(char* name);
	void CheckClientFile();
	void CheckPluginFile();
	void CheckVerifyFile();
public:
	HANDLE ThreadHandles[3];
	bool m_IsConnectedAuth;
	MAIN_FILE_INFO m_MainInfo;
	DWORD m_MainInfo_OutSize;
	char m_IpAddress[16];
	char m_MacAddress[18];
	char m_HardwareId[32];
	char m_SmallHardwareId[8];
	DWORD m_ClientFileCRC;
	char m_IpAddressSelect[33];
	char m_HardwareSerialId[33];
	DWORD m_BaseAddress;
	bool m_GetInfoClient;
	bool m_IsInGame;
	bool IsInGame;
	bool m_SendSelectServer;
	DWORD m_CheckLimit;
	DWORD m_CheckVPN;
	DWORD m_CheckProcess;
	DWORD m_CheckCount;
	int m_StateSkill;
	char m_WanIpAddress[16];
	DWORD m_Memory;
	int m_Check;
	DWORD m_CheckMayAo;
	DWORD m_AllowLimit;
	DWORD m_GiamHinhAnh;
	DWORD m_NpcLevel1;
	DWORD m_NpcLevel2;
	DWORD m_Resolution;
	DWORD m_GiamCPU;
	DWORD m_FixMauTim;
};

extern CProtect gProtect;

DWORD __fastcall sub_10001300(int nMemory);
DWORD __stdcall sub_10002F40(DWORD* This);
DWORD __stdcall sub_10003670(int a1, int a2);

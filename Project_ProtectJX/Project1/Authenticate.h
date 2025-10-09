#pragma once
#include "stdafx.h"
#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/aes.h>
#include <openssl/err.h>
#include <openssl/md5.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <thread>
#include <map>

#define AUTH_PORT  1311
#define MAX_BUFF_SIZE 8192
//#define AUTH_ADDRESS_A  "103.142.218.222";
//#define AUTH_ADDRESS_A  "15.204.192.229";
//#define AUTH_ADDRESS_B  "jx.b92dt.com";
//#define AUTH_ADDRESS_C  "jx.nguyenbinhdt.com";
#define MAKEWORD(a,b)   ((WORD)(((BYTE)(a))|(((WORD)((BYTE)(b)))<<8)))
#define LOWORD(l) ((WORD)(l))
#define HIWORD(l) ((WORD)(((DWORD)(l) >> 16) & 0xFFFF))
#define LOBYTE(w) ((BYTE)(w))
#define HIBYTE(w) ((BYTE)(((WORD)(w) >> 8) & 0xFF))
#define closesocket close

struct PBMSG_HEAD
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

struct PMSG_AUTH_INFO_RECV
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE status;
	char CodeCheck[33];
	char LicenseId[33];
	char IpAddress[16];
	DWORD m_Port;
	DWORD m_AntiDump;
	DWORD m_ThanPhap;
	DWORD m_WorldRank;
	DWORD m_ProtocolVersion;
	DWORD m_BuySell;
	DWORD m_Limit;
	DWORD m_MaxIpConnect;
	DWORD m_ClientVersion;
	DWORD m_ExpSkillPlayer;
	DWORD m_StateSkill;
	DWORD m_Window1024[8];
	DWORD m_FakeUserBattles;
	DWORD m_ReAuthen;
};

struct PMSG_AUTH_INFO_PROTECT_SEND
{
	PBMSG_HEAD header;
	char LicenseId[33];
	char CheckCode[33];
};

//**********************************************//
//************ FProtect -> Bishop **************//
//**********************************************//

struct BISHOP_SYNC_HWID_RECV
{
	PBMSG_HEAD header; // C1:00
	BYTE result;
	char Character[32];
	char m_IpAddr[16];
	char IpAddress[16];
	char MacAddress[18];
	char HardwareId[32];
};

struct SYNC_HWID_INFO_SEND
{
	PBMSG_HEAD header;
	char LicenseId[33];
	char CheckCode[33];
	DWORD Type;
};

struct HARDWARE_ID_LIMIT_INFO
{
	char HardwareId[32];
	WORD HardwareIdCount;
	char Character[32];
	char m_IpAddr[16];
	char IpAddress[16];
	char MacAddress[18];
};

using namespace std;

void* ReCheckAuth(void* This);
void* GlobalDataRecv(void*);
void GlobalAuthInfoRecv(PMSG_AUTH_INFO_RECV* lpMsg);
int Authenticate(char* LicenseId, char* CheckCode);
int PacketDecryptData(BYTE* lpMsg, int size, BYTE key);
int PacketEncryptData(BYTE* lpMsg, int size, BYTE key);
int EncryptStringProtect(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext);
int DecryptStringProtect(unsigned char* ciphertext, int ciphertext_len, unsigned char* plaintext);

class CAuthenticate
{
//private:
//	int THANPHAP_ACTIVE = 0;
//	int WORLDRANK_ACTIVE = 0;
//	int PACKETFILTER_ACTIVE = 0;
//	int DISABLE_EXP_SKILL_ACTIVE = 0;
//	int FAKE_USER_BATTLE = 0;
public:
	char LicenseId[33];
	char CheckCode[33];
	char AllowClientSerial[33];
	//char LicenseText[40];
public:
	CAuthenticate();
	virtual ~CAuthenticate();
	static void* Init(void*);
	static void* AutoReconnectCheck(void*);
	//static void* __cdecl CheckLicense(void*);
	bool DataRecv();
	bool DataSend(BYTE* lpMsg, int size);
	void AuthInfoRecv(PMSG_AUTH_INFO_RECV* lpMsg);
	int ReconnectCheck();
	static void* __cdecl ClientWorkerThread(LPVOID lpThisParam);
	void ProtocolCore(BYTE head, BYTE* lpMsg, int size);
	bool Connect(char* IpAddress, WORD port);
	void RequestInfoSend();
	void RequestHwidDataSend();
	bool DelayMe(DWORD delay, DWORD value);
	int enable_keepalive(int sock);
	void Disconnect();
	void Disconnected();
	void AuthenticateError();
	void ReAuthenticateError();
	void FProtectSyncHwidRecv(BISHOP_SYNC_HWID_RECV* lpMsg);
public:
	pthread_t thread_id;
	int m_Port;
	char m_HostAuth1[32];
	char m_HostAuth2[32];
	char m_HostAuth3[32];
	char m_CurrenAuthIP[32];
	int m_socket;
	DWORD m_ReconnectStatus;
	BYTE m_RecvBuff[MAX_BUFF_SIZE];
	int m_RecvSize;
	BYTE m_SendBuff[MAX_BUFF_SIZE];
	int m_SendSize;
	int m_Result;
	void (*wsProtocolCore)(BYTE, BYTE*, int);
	bool m_AuthReceived;
	const int timeout = -1;
	const int MAX_EVENTS = 20;
	int m_RateFakeBattles;
	int EventSocket;
	bool m_ClientWorkerThread;
	bool m_IsGoddess;
	bool m_IsBishop;
	bool m_GameServer;
	bool m_S3Replay;
	bool m_RunService;
	DWORD ExpiredTime;
	int m_ConnectError;
	bool m_ResCheckAuthen;
	bool m_DisableReAuthen = false;
	time_t nAuthCheckDelay;
	std::map<std::string, HARDWARE_ID_LIMIT_INFO> m_HardwareIdLimitInfo;
};

extern CAuthenticate gAuthenticate;
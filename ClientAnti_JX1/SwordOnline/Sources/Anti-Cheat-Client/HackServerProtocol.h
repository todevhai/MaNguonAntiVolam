#pragma once

#define SET_NUMBERHB(x) ((BYTE)((DWORD)(x)>>(DWORD)8))
#define SET_NUMBERLB(x) ((BYTE)((DWORD)(x)&0xFF))
#define SET_NUMBERHW(x) ((WORD)((DWORD)(x)>>(DWORD)16))
#define SET_NUMBERLW(x) ((WORD)((DWORD)(x)&0xFFFF))
#define SET_NUMBERHDW(x) ((DWORD)((QWORD)(x)>>(QWORD)32))
#define SET_NUMBERLDW(x) ((DWORD)((QWORD)(x)&0xFFFFFFFF))
#define KSG_PASSWORD_MAX_SIZE   64
#define MAKE_NUMBERW(x,y) ((WORD)(((BYTE)((y)&0xFF))|((BYTE)((x)&0xFF)<<8)))
#define MAKE_NUMBERDW(x,y) ((DWORD)(((WORD)((y)&0xFFFF))|((WORD)((x)&0xFFFF)<<16)))
#define MAKE_NUMBERQW(x,y) ((QWORD)(((DWORD)((y)&0xFFFFFFFF))|((DWORD)((x)&0xFFFFFFFF)<<32)))

//**********************************************//
//************ Packet Base *********************//
//**********************************************//

struct PBMSG_HEAD
{
	void set(BYTE head,BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->key = rand()%256;
		this->head = head;
	}

	BYTE type;
	BYTE size;
	BYTE key;
	BYTE head;
};

struct PSBMSG_HEAD
{
	void set(BYTE head,BYTE subh,BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->key = rand()%256;
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size;
	BYTE key;
	BYTE head;
	BYTE subh;
};

struct PWMSG_HEAD
{
	void set(BYTE head,WORD size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->key = rand()%256;
		this->head = head;
	}

	BYTE type;
	BYTE size[2];
	BYTE key;
	BYTE head;
};

struct PSWMSG_HEAD
{
	void set(BYTE head,BYTE subh,WORD size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->key = rand()%256;
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size[2];
	BYTE key;
	BYTE head;
	BYTE subh;
};

//**********************************************//
//********** HackServer -> Client **************//
//**********************************************//

struct SDHP_CLIENT_INFO_RECV
{
	PBMSG_HEAD header; // C1:00
	BYTE result;
	BYTE EncDecKey1;
	BYTE EncDecKey2;
	BYTE InternalVersion;
	BYTE ReconnectSwitch;
	BYTE MemoryGuardSwitch;
	BYTE MemoryGuardNumber;
	BYTE MacroProtectSwitch;
	BYTE MacroProtectNumber;
	char ClientVersion[8];
	char ClientSerial[17];
	DWORD MemoryAddress1;
	DWORD MemoryAddress2;
	DWORD MemoryAddress3;
	DWORD MemoryAddress4;
	DWORD MemoryAddress5;
	DWORD MemoryAddress6;
	DWORD MemoryAddress7;
	DWORD MemoryAddress8;
	DWORD MaxHardwareIdConnection;
	DWORD nAntiDump;
	DWORD nThanPhap;
	DWORD nWorldRank;
	DWORD nProtocolVersion;
	DWORD nLimit;
	char AllowCode[33];
	DWORD m_StateSkill;
	DWORD m_Window1024[8];
	DWORD m_CheckLimit;
	DWORD m_CheckVPN;
	DWORD m_CheckProcess;
	DWORD m_CheckCount;
	DWORD m_FakeUserTK;
};

struct SDHP_CLIENT_SELECT_SERVER_RECV
{
	PBMSG_HEAD header; // C1:00
	BYTE result;
	char ClientSerial[33];
};

struct SDHP_CONNECTION_STATUS_RECV
{
	PBMSG_HEAD header; // C1:01
};

struct SDHP_DUMP_LIST_RECV
{
	PSWMSG_HEAD header; // C1:02:00
	//PBMSG_HEAD header; // C1:02:00
	WORD MaxCount;
	BYTE count;
};

struct SDHP_CHECKSUM_LIST_RECV
{
	PSWMSG_HEAD header; // C1:02:01
	WORD MaxCount;
	BYTE count;
};

struct SDHP_INTERNAL_LIST_RECV
{
	PSWMSG_HEAD header; // C1:02:02
	WORD MaxCount;
	BYTE count;
};

struct SDHP_WINDOW_LIST_RECV
{
	PSWMSG_HEAD header; // C1:02:03
	WORD MaxCount;
	BYTE count;
};

struct SDHP_CLIENT_DISCONNECT_RECV
{
	PBMSG_HEAD header; // C1:03
	BYTE type;
};


//**********************************************//
//************ Client -> vProtect **************//
//**********************************************//

struct SDHP_CLIENT_INFO_SEND
{
	PBMSG_HEAD header; // C1:00
	DWORD IsReconnect;
	DWORD ClientFileCRC;
	char HackVersion[8];
	char ClientSerial[33];
	//char IpAddress[16];
	//char MacAddress[18];
	//char HardwareId[32];
	char HardwareSerialId[36];
	DWORD Version;
};

struct SDHP_CLIENT_SELECT_SERVER_SEND
{
	PBMSG_HEAD header; // C1:00
	DWORD IsReconnect;
	DWORD ClientFileCRC;
	char HackVersion[8];
	char ClientSerial[33];
	char IpAddress[32];
	//char MacAddress[18];
	char HardwareId[32];
	char IpAddressSelect[33];
	//char HardwareSerialId[36];
	DWORD Version;
	DWORD InGame;
};

struct SDHP_REMOVE_SELECT_SERVER_SEND
{
	PBMSG_HEAD header; // C1:00
	char IpAddressSelect[33];
	char HardwareId[32];
	DWORD Version;
};

struct SDHP_CONNECTION_STATUS_SEND
{
	PBMSG_HEAD header; // C1:01
};

struct SDHP_CLIENT_DISCONNECT_SEND
{
	PBMSG_HEAD header; // C1:03
	BYTE type;
	char account[11];
	char name[11];
	char CaptionName[64];
	char ProcessName[64];
};

struct SDHP_SYNC_HWID_SEND
{
	PSBMSG_HEAD header; // C1:00
	BYTE result;
	char Character[32];
	char IpAddress[16];
	char MacAddress[18];
	char HardwareId[36];
};


//**********************************************//
//**********************************************//
//**********************************************//
//void HCConnectInfoRecv(SDHP_CONNECT_INFO_RECV* lpMsg);

void ExitGameSend();
void HCClientSelectServerRecv(SDHP_CLIENT_SELECT_SERVER_RECV* lpMsg);
void HackServerProtocolCore(BYTE head,BYTE* lpMsg,int size);
void HCClientInfoRecv(SDHP_CLIENT_INFO_RECV* lpMsg);
void HCConnectionStatusRecv(SDHP_CONNECTION_STATUS_RECV* lpMsg);
void HCDumpListRecv(SDHP_DUMP_LIST_RECV* lpMsg);
void HCChecksumListRecv(SDHP_CHECKSUM_LIST_RECV* lpMsg);
void HCInternalListRecv(SDHP_INTERNAL_LIST_RECV* lpMsg);
void HCWindowListRecv(SDHP_WINDOW_LIST_RECV* lpMsg);
void HCClientDisconnectRecv(SDHP_CLIENT_DISCONNECT_RECV* lpMsg);
void CHRemoveSelectServerSend();
void CHClientSelectServerSend(int nProtocol);
void CHConnectInfoSend();
void CHClientInfoSend();
void CHConnectionStatusSend();
void CHClientDisconnectSend(int type,char* text,DWORD pid);

int __stdcall hook_sub_5770F1();

extern DWORD gConnectionStatusTime;
extern DWORD gMaxHardwareIdConnection;
extern DWORD gEncDecKey1;
extern DWORD gEncDecKey2;
extern DWORD gClientInfoOK;
extern DWORD gDumpListOK;
extern DWORD gChecksumListOK;
extern DWORD gInternalListOK;
extern DWORD gWindowListOK;
extern DWORD gClassListOK;
extern DWORD gDumpListMaxCount;
extern DWORD gChecksumListMaxCount;
extern DWORD gInternalListMaxCount;
extern DWORD gWindowListMaxCount;
extern DWORD gClassListMaxCount;
extern DWORD gMemoryGuardSwitch;
extern DWORD gMemoryGuardNumber;
extern DWORD gMacroProtectSwitch;
extern DWORD gMacroProtectNumber;
extern DWORD gReconnectSwitch;
extern DWORD gReconnectStatus;
extern DWORD gDetectCloseTime;
extern DWORD gInternalVersion;
extern DWORD gUserAccount;
extern DWORD gUserStruct;
extern DWORD gWindowHwnd;
extern DWORD gIpAddressAddress;
extern DWORD gClientVersionAddress;
extern DWORD gClientSerialAddress;
extern DWORD gFileProtectAddress1;
extern DWORD gFileProtectAddress2;
extern char gIpAddress[32];
extern BYTE gClientVersion[5];
extern BYTE gClientSerial[17];
extern DWORD gAntiDump;
extern DWORD gThanPhap;
extern DWORD gWorldRank;
extern DWORD gProtocolVersion;
extern DWORD gLimit;
extern DWORD gStateSkill;
extern DWORD gFakeUserBattles;
extern DWORD gIPProtect;
extern DWORD gPortProtect;
extern DWORD gProcessListMaxCount;
extern DWORD gProcessListOK;
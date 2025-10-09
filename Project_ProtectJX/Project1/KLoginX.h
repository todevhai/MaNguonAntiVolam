#pragma once
#include <string>
#include <iostream>
#include <map>
// uncomment to disable assert()
// #define NDEBUG
#include <cassert>
#include "KPlayerNew.h"
#ifdef NDEBUG
#  define assert(condition) ((void)0)
#else
#  define assert(condition) /*implementation defined*/
#endif
//#include "../osinlove/engine.dll.h"
#if __GNUC__ >= 4
#define SWEEP_API __attribute__((visibility("default")))
#define SWEEP_PACKED( class_to_pack ) class_to_pack __attribute__((__packed__))
#else
#if defined _WIN32 || defined __CYGWIN__
#define SWEEP_API __declspec(dllexport)
#define SWEEP_PACKED(class_to_pack) __pragma(pack(push, 1)) class_to_pack __pragma(pack(pop))
#else
#error "Only Clang, GCC and Visual supported at the moment, please open a ticket"
#define SWEEP_API
#define SWEEP_PACKED
#endif
#endif
//0x804E92E

using namespace std;

typedef int LONG;
#define KSG_PASSWORD_MAX_SIZE   64
#define MAX_USERS 100000

// c2s_login
enum c2s_PROTOCOL : __int32
{
	c2s_roleserver_saveroleinfo = 0xA,
	c2s_roleserver_createroleinfo = 0xB,
	c2s_gmsvr2gateway_saverole = 0xC,
	c2s_micropackbegin = 0x1F,
	c2s_accountbegin = 0x20,
	c2s_accountlogin = 0x21,
	c2s_gamelogin = 0x22,
	c2s_accountlogout = 0x23,
	c2s_gatewayverify = 0x24,
	c2s_gatewayverifyagain = 0x25,
	c2s_gatewayinfo = 0x26,
	c2s_gatewayclose = 0x27,
	c2s_account_change_extpoint = 0x28,
	c2s_gateway_kickout = 0x29,
	c2s_tryout_timeout_req = 0x2A,
	c2s_statinfo = 0x2B,
	c2s_cdkey = 0x2C,
	c2s_change_account_state = 0x2D,
	c2s_paysys_ib_item_buy = 0x2E,
	c2s_paysys_ib_item_use = 0x2F,
	c2s_paysys_end = 0x30,
	c2s_multiserverbegin = 0x30,
	c2s_permitplayerlogin = 0x31,
	c2s_updatemapinfo = 0x32,
	c2s_updategameserverinfo = 0x33,
	c2s_entergame = 0x34,
	c2s_leavegame = 0x35,
	c2s_registeraccount = 0x36,
	c2s_requestsvrip = 0x37,
	c2s_roleserver_getrolelist = 0x38,
	c2s_roleserver_getroleinfo = 0x39,
	c2s_roleserver_deleteplayer = 0x3A,
	c2s_transfer_role = 0x3B,
	c2s_gamestatistic = 0x3C,
	c2s_roleserver_lock = 0x3D,
	c2s_change_extpoint = 0x3E,
	c2s_use_spreader_cdkey = 0x3F,
	c2s_dynamicupdatemapinfo = 0x40,
	c2s_gs_ib_item_buy = 0x41,
	c2s_gs_ib_item_use = 0x42,
	c2s_ready_state = 0x43,
	c2s_gameserverbegin = 0x40,
	c2s_login = 0x41,
	c2s_logiclogin = 0x42,
	c2s_syncend = 0x43,
	c2s_loadplayer = 0x44,
	c2s_newplayer = 0x45,
	c2s_removeplayer = 0x46,
	c2s_requestworld = 0x47,
	c2s_requestplayer = 0x48,
	c2s_requestnpc = 0x49,
	c2s_requestobj = 0x4A,
	c2s_npcwalk = 0x4B,
	c2s_npcrun = 0x4C,
	c2s_npcskill = 0x4D,
	c2s_npcjump = 0x4E,
	c2s_npctalk = 0x4F,
	c2s_npchurt = 0x50,
	c2s_npcdeath = 0x51,
	c2s_playertalk = 0x52,
	c2s_team = 0x53,
	c2s_playerapplysetpk = 0x54,
	c2s_playerapplyfactiondata = 0x55,
	c2s_playersendchat = 0x56,
	c2s_playeraddbaseattribute = 0x57,
	c2s_playerapplyaddskillpoint = 0x58,
	c2s_playereatitem = 0x59,
	c2s_playerpickupitem = 0x5A,
	c2s_playermoveitem = 0x5B,
	c2s_playersellitem = 0x5C,
	c2s_playerbuyitem = 0x5D,
	c2s_playerthrowawayitem = 0x5E,
	c2s_playerselui = 0x5F,
	c2s_chatsetchannel = 0x60,
	c2s_chatapplyaddfriend = 0x61,
	c2s_chataddfriend = 0x62,
	c2s_chatrefusefriend = 0x63,
	c2s_dbplayerselect = 0x64,
	c2s_chatapplyresendallfriendname = 0x65,
	c2s_chatapplysendonefriendname = 0x66,
	c2s_chatdeletefriend = 0x67,
	c2s_chatredeletefriend = 0x68,
	c2s_tradeapplystateopen = 0x69,
	c2s_tradeapplystateclose = 0x6A,
	c2s_tradeapplystart = 0x6B,
	c2s_trademovemoney = 0x6C,
	c2s_tradedecision = 0x6D,
	c2s_dialognpc = 0x6E,
	c2s_changeauraskill = 0x6F,
	c2s_ping = 0x70,
	c2s_npcsit = 0x71,
	c2s_objmouseclick = 0x72,
	c2s_storemoney = 0x73,
	c2s_playerrevive = 0x74,
	c2s_tradereplystart = 0x75,
	c2s_pkapplychangenormalflag = 0x76,
	c2s_pkapplyenmity = 0x77,
	c2s_viewequip = 0x78,
	c2s_ladderquery = 0x79,
	c2s_repairitem = 0x7A,
	c2s_itemmask = 0x7B,
	c2s_stallextend = 0x7C,
	c2s_give = 0x7D,
	c2s_notify = 0x7E,
	c2s_enchaseritem = 0x7F,
	c2s_killerextend = 0x80,
	c2s_auctionextend = 0x81,
	c2s_citywarextend = 0x82,
	c2s_throwawayallmedicine = 0x83,
	c2s_boxoperate = 0x84,
	c2s_playerdivideitem = 0x85,
	c2s_bulletincontentquery = 0x86,
	c2s_autoattacknpc = 0x87,
	c2s_requestcityowner = 0x88,
	c2s_giveitemuiresult = 0x89,
	c2s_bot_clientcheck_result = 0x8A,
	c2s_welcome2server = 0x8B,
	c2s_offline_request_req = 0x8C,
	c2s_offline_request_ask = 0x8D,
	c2s_offline_kickout_res = 0x8E,
	c2s_offline_timeout_res = 0x8F,
	c2s_daytime_req = 0x90,
	c2s_requestnpcfeature = 0x91,
	c2s_foundry_request = 0x92,
	c2s_bishop_shutdowngodess = 0x93,
	c2s_replyroleinfo = 0x94,
	c2s_iambishop = 0x95,
	c2s_request_statdata = 0x96,
	c2s_getroledata_request = 0x97,
	c2s_hostexchange = 0x98,
	c2s_spectator = 0x99,
	c2s_partnerextend = 0x9A,
	c2s_closeconnection = 0x9B,
	c2s_queryrolename = 0x9C,
	c2s_lockaccount = 0x9D,
	c2s_unlockaccount = 0x9E,
	c2s_changerolename = 0x9F,
	c2s_changerolename_finish = 0xA0,
	c2s_querytongname = 0xA1,
	c2s_changetongname = 0xA2,
	c2s_sendtextcmd = 0xA3,
	c2s_chatroom = 0xA4,
	c2s_tongexextend = 0xA5,
	c2s_request_npcstate = 0xA6,
	c2s_apply_syncfile = 0xA7,
	c2s_setplayertaskvalue = 0xA8,
	c2s_nationalwar = 0xA9,
	c2s_select_diceitem = 0xAA,
	c2s_script_protocol = 0xAB,
	c2s_stores_change_shop = 0xAC,
	c2s_tripserver = 0xAD,
	c2s_tripclient = 0xAE,
	c2s_get_tags_request = 0xAF,
	c2s_add_tag = 0xB0,
	c2s_del_tag = 0xB1,
	c2s_set_friend_publish_flag = 0xB2,
	c2s_num_check = 0xB3,
	_c2s_begin_relay = 0xFA,
	c2s_extend = 0xFA,
	c2s_extendchat = 0xFB,
	c2s_extendfriend = 0xFC,
	_c2s_end_relay = 0xFC,
	c2s_extendtong = 0xFD,
	c2s_end = 0xFE,
};

enum ServiceType : __int32
{
	SERVICE_NORMAL = 0x0,
	SERVICE_DENIAL = 0x1,
	SERVICE_COUNT = 0x2,
};

//#pragma pack (push, 1)

#define	PROTOCOL_MSG_TYPE	BYTE
#define PROTOCOL_MSG_SIZE	(sizeof(PROTOCOL_MSG_TYPE))

//struct KLoginAccountInfo
//{
//	unsigned short Size;
//	int Param;
//	char Account[32];
//	KSG_PASSWORD Password;
//	unsigned int nLeftTime;
//	unsigned int ProtocolVersion;
//	KAccount_Limittime_Info _anon_0;
//	DWORD dwLimitTimeFlag;
//	DWORD dwLimitTimeOnlineSeconds;
//	DWORD dwLimitTimeOfflineSeconds;
//	char	        MacAddress[18];
//	char	        IpAddress[16];
//	char	        HardwareId[32];
//};

//#pragma once
//#pragma pack(push, 1)
//struct __attribute__((packed)) __attribute__((aligned(2))) KLoginStructHead
//{
//	unsigned __int16 Size;
//	int Param;
//};
//
//struct KLoginStructHead
//{
//	unsigned __int16 Size;
//	int Param;
//};
//typedef struct KLoginStructHead
//{
//	unsigned __int16 Size;
//	int Param;
//} KLoginStructHead;
//
//struct MyStruct
//{
//
//};
//typedef MyStruct MyStruct;

//typedef struct {
//	unsigned __int16 Size;
//	int Param;
//} __attribute__((packed)) __attribute__((aligned(2)))  KLoginStructHead;
//
//typedef struct {
//	char	        Account[32];
//	KSG_PASSWORD    Password;
//	unsigned long   nLeftTime;
//	unsigned    ProtocolVersion;
//	KAccount_Limittime_Info _anon_0;
//	DWORD dwLimitTimeFlag;
//	DWORD dwLimitTimeOnlineSeconds;
//	DWORD dwLimitTimeOfflineSeconds;
//	char MacAddress[18];
//	char IpAddress[16];
//	char HardwareId[32];
//} __attribute__((packed)) __attribute__((aligned(2))) KLoginStructHead,*KLoginAccountInfo;

//#pragma (pop)

//struct __attribute__((packed)) __attribute__((aligned(2))) KLoginStructHead
//{
//	void push(BYTE size, BYTE param) // OK
//	{
//		*(BYTE*)this->Size = size;
//		*(BYTE*)this->Param = param;
//	}
//
//	void set(BYTE head) // OK
//	{
//		*(BYTE*)this->head = head;
//	}
//
//	BYTE Param[2];
//	BYTE Size[2];
//	BYTE head[2];
//
//};

//struct __attribute__((packed)) __attribute__((aligned(2))) KLoginStructHead
//struct __attribute__((packed)) __attribute__((aligned(2))) KLoginStructHead
//{
//	unsigned short Size;
//	int Param;
//};

//struct KLoginAccountInfo
//{
//	unsigned short Size;
//	int Param;
//	char Account[32];
//	KSG_PASSWORD Password;
//	unsigned int nLeftTime;
//	unsigned int ProtocolVersion;
//	KAccount_Limittime_Info _anon_0;
//	DWORD dwLimitTimeFlag;
//	DWORD dwLimitTimeOnlineSeconds;
//	DWORD dwLimitTimeOfflineSeconds;
//	char	        MacAddress[18];
//	char	        IpAddress[16];
//	char	        HardwareId[32];
//};

//__attribute__((aligned(2), packed)) struct KLoginAccountInfo
//{
//	unsigned short Size;
//	int Param;
//	char Account[32];
//	KSG_PASSWORD Password;
//	unsigned int nLeftTime;
//	unsigned int ProtocolVersion;
//	KAccount_Limittime_Info _anon_0;
//	DWORD dwLimitTimeFlag;
//	DWORD dwLimitTimeOnlineSeconds;
//	DWORD dwLimitTimeOfflineSeconds;
//	char	        MacAddress[18];
//	char	        IpAddress[16];
//	char	        HardwareId[37];
//};
//
//struct KLoginStructHead
//{
//	unsigned short Size;
//	struct KLoginAccountInfo Param;
//};

//struct __attribute__((aligned(2), packed)) KLoginStructHead
//{
//	unsigned short Size;
//	int Param;
//	struct KLoginAccountInfo
//	{
//		unsigned short Size;
//		int Param;
//		char Account[32];
//		KSG_PASSWORD Password;
//		unsigned int nLeftTime;
//		unsigned int ProtocolVersion;
//		KAccount_Limittime_Info _anon_0;
//		DWORD dwLimitTimeFlag;
//		DWORD dwLimitTimeOnlineSeconds;
//		DWORD dwLimitTimeOfflineSeconds;
//		char	        MacAddress[18];
//		char	        IpAddress[16];
//		char	        HardwareId[32];
//	};
//};

//struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) KLoginAccountInfo
//{
//	KLoginStructHead super_KLoginStructHead;
//	char Account[32];
//	KSG_PASSWORD Password;
//	unsigned int nLeftTime;
//	unsigned int ProtocolVersion;
//	KAccount_Limittime_Info _anon_0;
//	DWORD dwLimitTimeFlag;
//	DWORD dwLimitTimeOnlineSeconds;
//	DWORD dwLimitTimeOfflineSeconds;
//	char	        MacAddress[18];
//	char	        IpAddress[16];
//	char	        HardwareId[32];
//};

//__attribute__((packed, aligned(2))) __attribute__((aligned(4),packed))  : public KLoginStructHead
//#pragma pack(1)
#pragma pack(push, 1)
typedef struct tagKSG_PASSWORD
{
	char szPassword[KSG_PASSWORD_MAX_SIZE];
	//char szPassword[32];    // 现在采用MD5的字符串，由于是32个字符，加上末尾'\0'，需要至少33个空间，因此使用64
	//unsigned char szData1[8];    // 现在采用MD5的字符串，由于是32个字符，加上末尾'\0'，需要至少33个空间，因此使用64
	//char szData2[24];    // 现在采用MD5的字符串，由于是32个字符，加上末尾'\0'，需要至少33个空间，因此使用64
} KSG_PASSWORD;

union KAccount_Limittime_Info
{
	unsigned int nLeftTimeOfPoint;
	int bKickOutOldAcc;
};

struct KLoginStructHead
{
	unsigned short	Size;		//size of the struct, if the struct is been inherit, the size is ref the the derive struct
	int				Param;		//be one of the LOGIN_R_* define value combin with a LOGIN_A_* value.
};
//__attribute__((packed)) __attribute__((aligned(1)))

struct KLoginAccountInfo : KLoginStructHead
{
	char Account[32];
	KSG_PASSWORD Password;
	unsigned int nLeftTime;
	unsigned int ProtocolVersion;
	KAccount_Limittime_Info _anon_0;
	DWORD dwLimitTimeFlag;
	DWORD dwLimitTimeOnlineSeconds;
	DWORD dwLimitTimeOfflineSeconds;
	char LicenseId[33];
	char HardwareId[32];
	char HardwareSerialId[36];
	char MacAddress[18];
	char IpAddress[16];
};
#pragma (pop)

struct KBUFFER
{
	size_t nBuffLen;
	size_t nDataLen;
};

struct KMsgQueue : KBUFFER
{
	KMsgQueue::KBUFFER* m_aryBuffer[255];
};


struct CIntercessor
{
	char tmp[0xCB8];
};

struct KConsole
{
	char tmp[0x24];
};

//struct KMsgQueue
//{
//	char tmp[0x3FC];
//};

struct IGoddessMsgHandler
{
	int (**_vptr_IGoddessMsgHandler)(...);
};

//struct tagExtPointInfo
//{
//	int nExtPoint[8];
//};
//
//struct KACCOUNT_LIMITTIME_INFO
//{
//	DWORD dwLimitTimeFlag;
//	DWORD dwLimitTimeOnlineSeconds;
//	DWORD dwLimitTimeOfflineSeconds;
//};
//
//struct tagAccountUserInfo
//{
//	tagExtPointInfo ExtPointInfo;
//	DWORD nTotalLeftTime;
//	DWORD nLeftTimeOfPoint;
//	KACCOUNT_LIMITTIME_INFO LimitTimeInfo;
//};

struct tagAccountUserInfoNew
{
	tagExtPointInfo ExtPointInfo;
	DWORD nTotalLeftTime;
	DWORD nLeftTimeOfPoint;
	KACCOUNT_LIMITTIME_INFO LimitTimeInfo;
	char LicenseId[33];
	char MacAddress[18];
	char IpAddress[16];
	char HardwareId[32];
};

struct KAccountUserLoginInfo
{
	char tmp[0x73];
};

struct KAccountUserReturnExt
{
	char tmp[0x62];
};

struct CGamePlayer
{
	char tmp[0xEE8];
};

struct CGameServer
{
	char tmp[0x860];
};

//struct Account
//{
//	enumAccountStatus nStatus;
//	DWORD dwLoginTime;
//	int nIndex;
//	DWORD dwMapId;
//	Exchange sExchange;
//	int nGameServer;
//	int nEnterGameCount;
//	char szRole[32];
//	char szPassword[64];
//};
//
//struct __attribute__((packed)) __attribute__((aligned(1))) TRoleBaseInfo
//{
//	DWORD dwId;
//	char szName[32];
//	bool bSex;
//	char szReserved[16];
//	DWORD dwLastExchangeTime;
//	DWORD dwLastLoginTime;
//	DWORD dwLastOfflineTime;
//	DWORD dwEnergy;
//	char caccname[32];
//	BYTE nLastSect;
//	BYTE nSect;
//	BYTE cFightMode;
//	BYTE cUseRevive;
//	BYTE cIsExchange;
//	BYTE cPkStatus;
//	int ijoincount;
//	int isectrole;
//	int igroupcode;
//	int igrouprole;
//	int irevivalid;
//	int irevivalx;
//	int irevivaly;
//	int ientergameid;
//	int ientergamex;
//	int ientergamey;
//	char cpartnercode[32];
//	int isavemoney;
//	int imoney;
//	int ifiveprop;
//	int iteam;
//	WORD ifightlevel;
//	__int16 fightexpHigh;
//	int fightexp;
//	int ileadlevel;
//	int ileadexp;
//	int iliveexp;
//	int ipower;
//	int iagility;
//	int iouter;
//	int iinside;
//	int iluck;
//	int imaxlife;
//	int imaxstamina;
//	int imaxinner;
//	int icurlife;
//	int icurstamina;
//	int icurinner;
//	int ipkvalue;
//	int ileftprop;
//	int ileftfight;
//	int ileftlife;
//	int ifinishgame;
//	__int16 iarmorres;
//	__int16 iweaponres;
//	__int16 iheadimage;
//	int nSectStat;
//	int nWorldStat;
//	int nKillPeopleNumber;
//	int nBitFlag;
//	DWORD dwTongID;
//	int nRepute;
//	int nVotePoint;
//	DWORD dwLastLogoutTime;
//	__int16 shPhysicsRes;
//	__int16 shColdRes;
//	__int16 shPoisonRes;
//	__int16 shLightingRes;
//	__int16 shFireRes;
//	__int16 shReLiveTime;
//	BYTE byExtBox;
//	BYTE byBoxPasswordParam;
//	BYTE byReserve13;
//	BYTE byReserve14;
//	DWORD dwBoxPassword;
//	unsigned int uCatchTimeForAntiBot;
//	BYTE byRefuseLoginCount;
//	BYTE byHaveRefuseLogin;
//	BYTE byIsExchangeServer;
//	BYTE byRefuseLoginRe2;
//	int nMapCopyIndex;
//	DWORD dwCreateTime;
//	BYTE byDataTransMark;
//	BYTE byRenascenceCount;
//	BYTE bySkillMaxLevelAddons;
//	BYTE byMaxFireResist;
//	BYTE byMaxColdResist;
//	BYTE byMaxPoisResist;
//	BYTE byMaxLighResist;
//	BYTE byMaxPhysResist;
//	unsigned int uReserve9;
//	unsigned int uReserve0;
//};
//
//
//struct TRoleData
//{
//	DWORD dwVersion;
//	TRoleBaseInfo BaseInfo;
//	bool bBaseNeedUpdate;
//	__int16 nFightSkillCount;
//	__int16 nLiveSkillCount;
//	BYTE nTaskCount;
//	__attribute__((packed)) __attribute__((aligned(1))) __int16 nItemCount;
//	__attribute__((packed)) __attribute__((aligned(1))) __int16 nStateCount;
//	__attribute__((packed)) __attribute__((aligned(1))) int dwTaskOffset;
//	__attribute__((packed)) __attribute__((aligned(1))) int dwLSkillOffset;
//	__attribute__((packed)) __attribute__((aligned(1))) int dwFSkillOffset;
//	__attribute__((packed)) __attribute__((aligned(1))) int dwItemOffset;
//	__attribute__((packed)) __attribute__((aligned(1))) int dwStateOffset;
//	__attribute__((packed)) __attribute__((aligned(1))) int dwDataLen;
//	BYTE pBuffer[1];
//};


//typedef struct DataLimitHeader
//{
//	//char	AccountName[32];
//	std::string AccountName;
//};

typedef struct CNetAddress
{
	const char* NetAddress;
	const char* LanAddress;
	const char* MacAddress;
};

#pragma pack(1)
typedef struct CDataLimitConnects
{
	CNetAddress NetAddress;
	const char* AccountName;
	const char* Password;
} *DataLimitConnect;
#pragma pack(0)


struct KSimpleThread
{
	void (*m_pfnFunction)(void*);
	pthread_t m_hThread;
	void* m_pParam;
};

//struct __cppobj __attribute__((packed)) __attribute__((aligned(2))) tagLeaveGame : tagProtocolHeader
//{
//	BYTE cCmdType;
//	int nExtPoint;
//	char szAccountName[32];
//};

struct HARDWARE_ACCOUNT_LOGIN
{
	char HardwareId[32];
	char AccountName[32];
	char RoleName[32];
	char MacAddress[18];
	char IpAddress[16];
	char LicenseId[33];
	BYTE nProtocol;
	DWORD LastDetectionTime;
	bool IsLogined;
	//char DriveSerial[32];
	//CIntercessor* Intercessor;
	//CGameServer* GameServer;
};

struct HARDWARE_ID_LIMIT_LOGIN
{
	char HardwareId[32];
	WORD HardwareIdCount;
};

struct KCurrenDataCommand
{
	char m_MainCommand[32];
};

struct KService
{
	char tmp[0xCB8];
};

//typedef struct CDataLimitConnect* DataConnect[MAX_USERS];
//typedef struct CDataLimitConnect_container container;
//extern std::map<const char*, CDataLimitConnects> Data_LimitCount;

typedef bool __cdecl (*_CGamePlayer)(CGamePlayer*, const char*);
typedef bool __cdecl (*_IsAccountLocked)(LPVOID* , const char*);
typedef bool __cdecl (*_CheckAccountName)(LPVOID*, const char*);
typedef const void* __cdecl (*_MsgQueue_Pop)(LPVOID This, BYTE byProtocol, size_t* const nLen);
typedef bool __cdecl (*_VerifyAccountToPlayer)(CGamePlayer* , const char*, const char*, UINT , unsigned int , unsigned int , DWORD , DWORD , DWORD );

void* SortRankData(void* ptr);
BOOL __cdecl g_ChangeAccountInMap(LPCSTR pcszAccount, const Account* pNew, const Account* pMask);
bool __cdecl CGameServer_PushAccount(CGameServer* const This, const char* pAccountName);
void __cdecl KConsole_ProcessCmd(KConsole* const This, LPCSTR pszCmd);
bool __cdecl CIntercessorAddPlayer(CIntercessor* This, LPCSTR pszRole, CGamePlayer* pPlayer);
void __cdecl _FreePlayer(CGamePlayer* This);
//bool __cdecl CGameServer_PushAccount(CGameServer* const This, const char* pAccountName);
//bool __cdecl CGameServer_PushAccount(CGameServer* const This, BYTE cProtocol, const void* pData, size_t nLen);
bool __cdecl NotifyEnterGame(CGameServer* const This, const void* pData, size_t datalength);
bool __cdecl OfflineRequestAsk(CGameServer* This, const void* pData, size_t nDataSize);
const void* __cdecl KMsgQueuePop(KMsgQueue* const This, BYTE byProtocol, size_t* const nLen);
UINT __cdecl ProcWaitForAccPwd(CGamePlayer* const This);
BOOL __cdecl c2sCheckProtocol(CIntercessor* This, c2s_PROTOCOL nProtocol, const BYTE* pMsg, size_t size);
bool __cdecl SyncRoleInfoToGameServer(CGameServer* This, const void* pData, size_t dataLength, void* pvData);
bool __cdecl SafeDispatchTask(CGameServer* This, UINT nTask, const void* pData, size_t datalength, void* pvData);
bool __cdecl NotifyEnterGameGetAccountInMap(LPCSTR pcszAccount, Account* pAccount);
BOOL __cdecl GetAccountInMap(LPCSTR pcszAccount, Account* pAccount);
void __cdecl GamePlayerShutdown(CGamePlayer* This);
BOOL __cdecl GameServer_Destroy_Account(LPCSTR pcszAccount, Account* pAccount);
BOOL __cdecl GamePlayer_DispatchPlayerMessage(CIntercessor* This, CGamePlayer* pPlayer, size_t index, const void* pData, size_t datalength);
void __cdecl SafePrepareLogin(CGamePlayer* This, KAccountUserLoginInfo* pLogin, DWORD dwIP, DWORD dwPort);
BOOL __cdecl SafeCheckAccountMap(CGamePlayer* This, LPCSTR pszFunc);
const void* __cdecl ProcessRoleInfoMsgQueue(KMsgQueue* const This, BYTE byProtocol, size_t* const nLen);
void __cdecl g_CGameServer(CGameServer* This, UINT nIdentityID, KService* pService, CIntercessor* pIntercessor, const void* pPaysys);
void __cdecl g_OnGameServerDestroy(CIntercessor* This, unsigned int nID);
void __cdecl handler_signal(int signal);

struct WHITELIST_HARDID_INFO
{
	char HardwareId[32];
};

class KLoginX
{
public:
	KCurrenDataCommand m_MainCommand[3];
	char m_CurrenSubCommand[32];
	bool m_IsSub;
	bool IsOpen;
public:
	KLoginX();
	virtual	~KLoginX();
	DataLimitConnect LoadData(const char* AccountName);
	bool CheckHardwareId(char* HardwareId);
	CDataLimitConnects* nDataLimitConnect;
	void	Hook_WaitForAccPwd();
	//std::map<std::string, int> BlackList_Data;
	std::vector<WHITELIST_HARDID_INFO> WhiteList_Data;
	std::map<int, CGameServer*> GameServer_List;
	/*bool InsertAccountLogin(HARDWARE_ACCOUNT_LOGIN info);
	bool CheckAccountEntergame(char AccountName[32]);
	bool CheckHardwareIdLimit(char* HardwareId, HARDWARE_ID_LIMIT_LOGIN info);
	bool InsertHardwareIdLimit(char* HardwareId, HARDWARE_ACCOUNT_LOGIN info);
	void RemoveHardwareIdLimit(char AccountName[32]);*/
	//bool GetDataHwid(char AccountName[32], HARDWARE_ACCOUNT_LOGIN* info);
	//bool GetAccountState(char AccountName[32]);
//private:
//	std::vector<HARDWARE_ACCOUNT_LOGIN> m_LoginHardwareIdInfo;
//	std::map<std::string, HARDWARE_ID_LIMIT_LOGIN> m_LoginHardwareIdLimitInfo;
};

extern KLoginX gKLoginX;

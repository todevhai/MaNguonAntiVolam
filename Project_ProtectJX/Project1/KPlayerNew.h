#pragma once
//#include "Defines.h"
#include "Util.h"
#include "SocketManager.h"
#include <signal.h>

//#include "LuaLib.h"
//#include "lua.h"
//#include "LuaLib.h"
//#define m_ExtPointInfo             0x44;
//#define m_nPlayerIndex             0xC0;
//#define m_AccountName             0x264;
//#define m_SaveBuffer             0x28C;
//#define m_bIsSyncAura             0x20388;
//#define m_dwID             0x20400;
//#define m_nIndex             0x20404;
//#define m_PlayerName             0x25C88;
//#define m_nWorldStat             0x28004;
//#define Offset_Player				0x88A1A20;
//#define PlayerStruct				*(KPlayer*)0x8128AC0
//#define __int8 char
//typedef unsigned __int8 BYTE;
// 0x80CAD90 KPlayer::Active(KPlayer *const this)
// 0x8085830 Data Custom Leader
// 0x8085510 Data Leader KLadder 0x82BCE40
//o	8085527	lea     ecx, [this_0 + KLadder.GameStatData.LevelStat] 0x82BCE40 0x80853E4
//00000000 MoneyStat       TRoleList 10 dup(? )
//000001CC LevelStat       TRoleList 10 dup(? ); XREF: KLadder::GetTopTen(ulong) + 17 / o
//00000398 KillerStat      TRoleList 10 dup(? )
//00000564 MoneyStatBySect TRoleList 110 dup(? )
//00001928 LevelStatBySect TRoleList 110 dup(? )
//00002CEC MoneyStatByCamp TRoleList 50 dup(? )
//000035E8 LevelStatByCamp TRoleList 50 dup(? )
//00003EE4 SectPlayerNum   dd 11 dup(? )
//00003F10 SectMoneyMost   dd 11 dup(? )
//00003F3C SectLevelMost   dd 11 dup(? )
//00003F68 FortuneStat     TRoleList 10 dup(? )
//00004134 BossKillerStat  TRoleList 10 dup(? )
//00004300 LeagueWinnerStat TRoleList 10 dup(? )
//000044CC $5A420B7D2C6DD93458F28826E84662D2 ends
// offset 121h 0xFF0099FC
// offset custom ladder 0xFF0099FC TCustomLadder g_CustomLadder[10]
// 0x82B72F4 all check error
// 0xFF00CE6C offset pet lua hook
// all ham bang hoi script MONEYFUND_ADD_G_1 MONEYFUND_ADD_G_1 uaTONG_GetTongCount
// LuaLG_ApplyRemoveLeague(
// 0x8160260 N16KLibFuncsManager10InitializeEv
// N16KLibFuncsManager10InitializeEv 0x80D7B60 int __cdecl KPlayer::SavePlayerBaseInfo(KPlayer *const this, BYTE *pRoleBuffer, BOOL bQuitSave)
// 0x80681C0 void __cdecl g_InitCore(int nProductRegion, int nProductLanguage)
// 0x8067130 void TestLua() .text:08067130                     public _Z7TestLuav
// 0x8067060  unsigned int g_IniScriptEngine()
//void __cdecl TestLuaScript(KLuaScript* pScript, LPCSTR pszFile)
//{
//	(*((void(__cdecl**)(KLuaScript*))pScript->_vptr_KScript + 2))(pScript);
//	(*((void(__cdecl**)(KLuaScript*, LPCSTR))pScript->_vptr_KScript + 4))(pScript, pszFile);
//	(*((void(__cdecl**)(KLuaScript*))pScript->_vptr_KScript + 3))(pScript);
//} 0x8066BB0
#define KNpc_Struct 0x8320A00
#define Player_Struct 0x88A1A20
#define Player_RankData 0x8085527
#define Player_Rank 0x82BCE40
#define Offset_GetEngergyDamage 0x80BF2A0
#define oOnLogin		((int(__cdecl*)(KPlayer* cPlayer)) 0x80C1A70)
#define oOnQuit		((int(__cdecl*)(KPlayer* cPlayer)) 0x80C5C10)
#define oPay		((int(__cdecl*)(KPlayer* cPlayer, int nMoney)) 0x80C0830)
#define oGetExtPoint		((int(__cdecl*)(KPlayer* cPlayer, int nIndex)) 0x80BF760)
#define oGetEngergyDamage		((int(__cdecl*)(KPlayer* cPlayer)) 0x80BF2A0)
#define oSavePlayerBaseInfo		((int(__cdecl*)(KPlayer* cPlayer, BYTE *pRoleBuffer, BOOL bQuitSave)) 0x80D7B60)
#define oAddDBPlayer		((int(__cdecl*)(KPlayer* cPlayer, char *szPlayerName, int value, int size)) 0x80D5760)
#define oAttachPlayer		((int(__cdecl*)(KPlayerSet*, int, _GUID*)) 0x80DD9A0) // KPlayerSet::AttachPlayer(ulong,_GUID const*)	080DD9A0	
#define		MESSAGE_SYSTEM_ANNOUCE_HEAD_CUSTOM		"Custom"
//KPlayer	Player[1200];
#define TRACE _trace
typedef unsigned char byte;

#ifndef GUID_DEFINED
#define GUID_DEFINED
#if defined(__midl)
typedef struct {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	byte           Data4[8];
} GUID;
#else
typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;
#endif
#endif

enum KE_SERVERTYPE : __int32
{
	emSERVER_GODDESS = 0x0,
	emSERVER_BISHOP = 0x1,
	emSERVER_HOST = 0x2,
	emSERVER_TONG = 0x3,
	emSERVER_CHAT = 0x4,
	emSERVER_COUNT = 0x5,
};


#pragma pack(push, 1)

struct IGameServer
{
	int (**_vptr_IGameServer)(...);
};

struct GameStatus
{
	int nPlayerIndex;
	int nGameStatus;
	int nNetStatus;
	int nExchangeStatus;
	DWORD dwSendPingTime;
	DWORD dwReplyPingTime;
	BYTE mapChannelMsgTimes[12];
};

struct KRecvPack
{
	char* m_pBuffer;
	size_t m_nBufLen;
	size_t m_nDataLen;
	BOOL m_bComplete;
};

struct KTRAN_UNIT
{
	DWORD dwIp;
	DWORD dwRelayID;
	KRecvPack* pRecvPack;
};

struct KEXCHANGING
{
	char szRole[32];
	int nExtPoint;
	DWORD dwLoopTime;
};

typedef struct account_less : std::binary_function<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, bool>
{
};


typedef std::map<std::basic_string<char, std::char_traits<char>, std::allocator<char> >, KEXCHANGING, account_less, std::allocator<std::pair<const std::basic_string<char, std::char_traits<char>, std::allocator<char> >, KEXCHANGING> > > MAP_EXCHANGING;

typedef std::multimap<long unsigned int, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::less<long unsigned int>, std::allocator<std::pair<const long unsigned int, std::basic_string<char, std::char_traits<char>, std::allocator<char> > > > > MULTIMAP_EXCHANGING;

struct IServer;

typedef struct KSendPack KSendPack,* PKSendPack;
typedef struct IServerCore IServerCore,* PIServerCore;
typedef struct KServerCore KServerCore, * PKServerCore;
typedef struct KHostProcess KHostProcess,* PKHostProcess;
typedef struct KClientProcess KClientProcess,* PKClientProcess;
typedef struct KGoddessProcess KGoddessProcess,* PKGoddessProcess;
typedef struct KBishopProcess KBishopProcess,* PKBishopProcess;
typedef struct KProtocolProcess KProtocolProcess,* PKProtocolProcess;
typedef struct IGameServer IGameServer,* PIGameServer;

struct KSendPack
{
	BYTE m_byProtocol;
	unsigned int m_nUserData;
	char* m_pBuffer;
	size_t m_nBufLen;
	size_t m_nOffset;
	size_t m_nDataLen;
	size_t m_nPackIdx;
	char* m_pPack;
	size_t m_nCurPackLen;
	size_t m_nMaxPackLen;
};

struct IServerCore
{
	int (**_vptr_IServerCore)(...);
};

struct KHostProcess
{
	BOOL m_bReady;
	std::map<long unsigned int, KTRAN_UNIT, std::less<long unsigned int>, std::allocator<std::pair<const long unsigned int, KTRAN_UNIT> > > m_mapIp2TransferUnit;
	KSendPack m_cSendPack;
	KRecvPack m_cRecvPack;
	KServerCore* m_pCore;
	IGameServer* m_pServer;
	MAP_EXCHANGING m_mapExchanging;
	MULTIMAP_EXCHANGING m_mmapExchanging;
};

struct KClientProcess
{
	BOOL m_bOpenGm;
	IGameServer* m_pServer;
	KServerCore* m_pCore;
};

struct KGoddessProcess
{
	KRecvPack m_cRecvPack;
	IGameServer* m_pServer;
	KServerCore* m_pCore;
};

struct KBishopProcess
{
	KRecvPack m_cRecvPack;
	IGameServer* m_pServer;
	KServerCore* m_pCore;
	BOOL m_bReady;
};

struct KProtocolProcess
{
	char tmp[0x880];
};

struct KServerCore : IServerCore
{
	GameStatus* m_pGameStatus;
	DWORD m_dwGameLoop;
	DWORD m_dwTickCount;
	int m_nMaxPlayerCount;
	BYTE* m_pTimerList[4];
	IGameServer* m_pServer;
	BYTE m_bySendBuffer[65536];
	KSendPack m_cSendPack;
	KGoddessProcess* m_pGoddessProto;
	KBishopProcess* m_pBishopProto;
	KHostProcess* m_pHostProto;
	KProtocolProcess* m_pProtocol;
	BYTE m_pTongProto[4];
	BYTE m_pChatProto[4];
	KClientProcess* m_pClientProto;
};

#pragma pack(pop)

#pragma pack(1) 
struct DataHeader
{
	BYTE ProtocolID;
	BYTE Size;
};

struct KDataShort : DataHeader
{
	int nPlayerIndex;
	unsigned int lnID;
	int nLevel;
	char m_szAccount[32];
	char m_szRole[32];
};

// 10
struct KGameServerInfo : DataHeader
{
	int m_ServerID;
	char m_Data[256];
};
#pragma pack(0)

#pragma pack(push, 2)
struct EXTEND_HEADER
{
	BYTE ProtocolFamily;
	BYTE ProtocolID;
};

struct tagEnterGame2 : EXTEND_HEADER
{
	char szAccount[32];
	char szRole[32];
	DWORD dwNameID;
	DWORD dwParam;
	unsigned int lnID;
	BYTE byLevel;
};
#pragma pack(pop)

//char LicenseId[33];
//char MacAddress[18];
//char IpAddress[16];
//char HardwareId[32];

enum enumAccountStatus : __int32
{
	enumInNone = 0x0,
	enumInGateway = 0x1,
	enumInGameServer = 0x2,
	enumOfflineLive = 0x3,
	enumOfflineLock_1 = 0x4,
	enumOfflineLock_2 = 0x5,
};

struct Exchange
{
	int nSource;
	int nTarget;
	DWORD dwMapId;
	BOOL bExchanging;
};

struct Account
{
	enumAccountStatus nStatus;
	DWORD dwLoginTime;
	int nIndex;
	DWORD dwMapId;
	Exchange sExchange;
	int nGameServer;
	int nEnterGameCount;
	char szRole[32];
	char szPassword[64];
};

#pragma pack(1) 
struct __attribute__((packed)) __attribute__((aligned(1))) TRoleBaseInfo
{
	DWORD dwId;
	char szName[32];
	bool bSex;
	char szReserved[16];
	DWORD dwLastExchangeTime;
	DWORD dwLastLoginTime;
	DWORD dwLastOfflineTime;
	DWORD dwEnergy;
	char caccname[32];
	BYTE nLastSect;
	BYTE nSect;
	BYTE cFightMode;
	BYTE cUseRevive;
	BYTE cIsExchange;
	BYTE cPkStatus;
	int ijoincount;
	int isectrole;
	int igroupcode;
	int igrouprole;
	int irevivalid;
	int irevivalx;
	int irevivaly;
	int ientergameid;
	int ientergamex;
	int ientergamey;
	char cpartnercode[32];
	int isavemoney;
	int imoney;
	int ifiveprop;
	int iteam;
	WORD ifightlevel;
	__int16 fightexpHigh;
	int fightexp;
	int ileadlevel;
	int ileadexp;
	int iliveexp;
	int ipower;
	int iagility;
	int iouter;
	int iinside;
	int iluck;
	int imaxlife;
	int imaxstamina;
	int imaxinner;
	int icurlife;
	int icurstamina;
	int icurinner;
	int ipkvalue;
	int ileftprop;
	int ileftfight;
	int ileftlife;
	int ifinishgame;
	__int16 iarmorres;
	__int16 iweaponres;
	__int16 iheadimage;
	int nSectStat;
	int nWorldStat;
	int nKillPeopleNumber;
	int nBitFlag;
	DWORD dwTongID;
	int nRepute;
	int nVotePoint;
	DWORD dwLastLogoutTime;
	__int16 shPhysicsRes;
	__int16 shColdRes;
	__int16 shPoisonRes;
	__int16 shLightingRes;
	__int16 shFireRes;
	__int16 shReLiveTime;
	BYTE byExtBox;
	BYTE byBoxPasswordParam;
	BYTE byReserve13;
	BYTE byReserve14;
	DWORD dwBoxPassword;
	unsigned int uCatchTimeForAntiBot;
	BYTE byRefuseLoginCount;
	BYTE byHaveRefuseLogin;
	BYTE byIsExchangeServer;
	BYTE byRefuseLoginRe2;
	int nMapCopyIndex;
	DWORD dwCreateTime;
	BYTE byDataTransMark;
	BYTE byRenascenceCount;
	BYTE bySkillMaxLevelAddons;
	BYTE byMaxFireResist;
	BYTE byMaxColdResist;
	BYTE byMaxPoisResist;
	BYTE byMaxLighResist;
	BYTE byMaxPhysResist;
	unsigned int uReserve9;
	unsigned int uReserve0;
};
#pragma pack(0)

struct tagExtPointInfo
{
	int nExtPoint[8];
};

struct KACCOUNT_LIMITTIME_INFO
{
	DWORD dwLimitTimeFlag;
	DWORD dwLimitTimeOnlineSeconds;
	DWORD dwLimitTimeOfflineSeconds;
};

struct tagAccountUserInfo
{
	tagExtPointInfo ExtPointInfo;
	DWORD nTotalLeftTime;
	DWORD nLeftTimeOfPoint;
	KACCOUNT_LIMITTIME_INFO LimitTimeInfo;
};

struct KACCOUNT_INFO2 : tagAccountUserInfo
{
	tagExtPointInfo ChangePointInfo;
};

struct TRoleData
{
	DWORD dwVersion;
	TRoleBaseInfo BaseInfo;
	bool bBaseNeedUpdate;
	__int16 nFightSkillCount;
	__int16 nLiveSkillCount;
	BYTE nTaskCount;
	__attribute__((packed)) __attribute__((aligned(1))) __int16 nItemCount;
	__attribute__((packed)) __attribute__((aligned(1))) __int16 nStateCount;
	__attribute__((packed)) __attribute__((aligned(1))) int dwTaskOffset;
	__attribute__((packed)) __attribute__((aligned(1))) int dwLSkillOffset;
	__attribute__((packed)) __attribute__((aligned(1))) int dwFSkillOffset;
	__attribute__((packed)) __attribute__((aligned(1))) int dwItemOffset;
	__attribute__((packed)) __attribute__((aligned(1))) int dwStateOffset;
	__attribute__((packed)) __attribute__((aligned(1))) int dwDataLen;
	BYTE pBuffer[1];
	char HardwareId[32];
};

struct tagProtocolHeader
{
	BYTE cProtocol;
	BYTE ProtocolType;
};

struct __attribute__((packed)) __attribute__((aligned(1))) tagProtocolHeader2 : tagProtocolHeader
{
	unsigned int ulIdentity;
};

struct __attribute__((packed)) __attribute__((aligned(1))) tagGuidableInfo : tagProtocolHeader2
{
	GUID guid;
	KACCOUNT_INFO2 sAccountInfo;
	size_t datalength;
	char szData[];
};

struct __attribute__((packed)) __attribute__((aligned(1))) tagProtectInfo : tagProtocolHeader2
{
	char m_Hwid[32];
	char m_LicenseId[33];
};

struct tagEnterGame : tagProtocolHeader
{
	BYTE szAccountName[32];
};

typedef struct _CUSTOM_GUID {
//typedef struct _CUSTOM_GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
	//char MacAddress[18];
	//unsigned char  MacAddress[6];
	//unsigned char  Data6[18];
	//unsigned char  Data7[35];
} CUSTOM_GUID;




struct KPlayerInfo
{
	int m_nIndex;
	char m_AccountName[32];
	char m_PlayerName[32];
	char m_NetIP[17];
	char m_LicenseId[33];
	char m_Mac[18];
	char m_LanIP[17];
	char m_Hwid[32];
	long long m_TickCount;
	int m_NPCTalkFlood;
	long long m_ChatTick;
	int m_ChatFlood;
	int m_Faction;
	char m_FactionName[64];
	DWORD* m_KPlayer;
	_GUID m_Guid[16];
	bool m_AutoCheck;
	int m_AllowLimit;
	int m_StallCoin;
	int m_SBuyType;
	int nWorldStat;
};

struct KClientInfo
{
	int m_NetConnectIdx;
	_GUID m_Guid[16];
	char m_TextGuid[33];
	long long m_TickCount;
	int m_Flood;
	bool m_Checked;
};

struct __attribute__((packed)) __attribute__((aligned(1))) PLAYER_NORMAL_SYNC : tagProtocolHeader
{
	__attribute__((packed)) __attribute__((aligned(1))) DWORD ID;
	BYTE AttackSpeed;
	BYTE RunSpeed;
	BYTE m_btChangeFeatureCount;
	BYTE HelmType;
	BYTE HorseType;
	BYTE WalkSpeed;
	BYTE CastSpeed;
	BYTE ArmorType;
	BYTE WeaponType;
	BYTE byMantleType;
	BYTE m_btSomeFlag1;
	BYTE m_btSomeFlag;
	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wTitleID;
	__attribute__((packed)) __attribute__((aligned(1))) DWORD m_dwTongNameID;
	BYTE nFortuneRank;
};

//typedef tagCURPLAYER_SYNC CURPLAYER_SYNC;
//#pragma pack(push, 1)
//typedef struct tagCURPLAYER_SYNC : tagProtocolHeader
//{
//	__attribute__((packed)) __attribute__((aligned(1))) DWORD m_dwID;
//	BYTE m_btLevel;
//	WORD m_wLifeMax;
//	WORD m_wStaminaMax;
//	BYTE m_btSex;
//	BYTE m_btKind;
//	BYTE m_btSeries;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wManaMax;
//	__attribute__((packed)) __attribute__((aligned(1))) int m_HeadImage;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wAttributePoint;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wEngergy;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wLucky;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wSkillPoint;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wStrength;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wDexterity;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wVitality;
//	__attribute__((packed)) __attribute__((aligned(1))) INT64 m_nExp;
//	__attribute__((packed)) __attribute__((aligned(1))) DWORD m_dwLeadExp;
//	BYTE m_byBoxParam;
//	BYTE m_btCurFaction;
//	__attribute__((packed)) __attribute__((aligned(1))) WORD m_nFactionAddTimes;
//	__attribute__((packed)) __attribute__((aligned(1))) __int16 m_nExpEx;
//	BYTE m_btLastFaction;
//	WORD m_wSectStat;
//	WORD m_wWorldStat;
//	int m_nMoney2;
//	int m_nMoney1;
//	int m_nRenascenceCount;
//	char LicenseId[33];
//	char MacAddress[18];
//	char IpAddress[16];
//	char HardwareId[32];
//} CURPLAYER_SYNC;
//#pragma (pop)

struct KPlayerSet
{
	//BYTE m_SaveBuffer[131072]; // 0xC0,
	//int m_nPlayerIndex; // 0xC0,
	//char m_AccountName[32]; //0x264,
	//DWORD m_dwID; //0x20400,
	//int m_nIndex; //0x20404,
	////KItemList m_ItemList; //0x2040C,
	////BuySellInfo m_BuyInfo; //0x2522C,
	////KPlayer::KSaveAction m_SaveAction; //0x25A08,
	//char m_PlayerName[32]; //0x25C88,
	////KPlayerFaction m_cFaction; //0x25CE0,
	//int m_nWorldStat; //0x28004,
	//int m_nSectStat; //0x28008,
	//DWORD m_dwEnergy; //0x2876C,
	//int m_nCurEngergy; //0x25C68
	//BuySellInfo m_BuyInfo;
	char tmp[0x2C54];
};

struct TONG_CHANGE_MONEY_COMMAND : tagProtocolHeader
{
	WORD m_wLength;
	BYTE m_btMsgId;
	DWORD m_dwMoneyFundChange;
	BYTE m_btSaveFlag;
};

//typedef struct __attribute__((packed)) __attribute__((aligned(1))) KPlayer
struct KPlayer
{
	//BYTE m_SaveBuffer[131072]; // 0xC0,
	//int m_nPlayerIndex; // 0xC0,
	//char m_AccountName[32]; //0x264,
	//DWORD m_dwID; //0x20400,
	//int m_nIndex; //0x20404,
	////KItemList m_ItemList; //0x2040C,
	////BuySellInfo m_BuyInfo; //0x2522C,
	////KPlayer::KSaveAction m_SaveAction; //0x25A08,
	//char m_PlayerName[32]; //0x25C88,
	////KPlayerFaction m_cFaction; //0x25CE0,
	//int m_nWorldStat; //0x28004,
	//int m_nSectStat; //0x28008,
	//DWORD m_dwEnergy; //0x2876C,
	//int m_nCurEngergy; //0x25C68
	//BuySellInfo m_BuyInfo;
	// 0x288D4 0x2A59D
	char tmp[165956];
	//BYTE tmp01[0x2040B];
	//KItemList m_ItemList;
	//BYTE tmp02[0x3618];
};

struct KSkill
{
	char tmp[0x69C];
};

struct KObjSet
{
	//BYTE m_SaveBuffer[131072]; // 0xC0,
	//int m_nPlayerIndex; // 0xC0,
	//char m_AccountName[32]; //0x264,
	//DWORD m_dwID; //0x20400,
	//int m_nIndex; //0x20404,
	////KItemList m_ItemList; //0x2040C,
	////BuySellInfo m_BuyInfo; //0x2522C,
	////KPlayer::KSaveAction m_SaveAction; //0x25A08,
	//char m_PlayerName[32]; //0x25C88,
	////KPlayerFaction m_cFaction; //0x25CE0,
	//int m_nWorldStat; //0x28004,
	//int m_nSectStat; //0x28008,
	//DWORD m_dwEnergy; //0x2876C,
	//int m_nCurEngergy; //0x25C68
	//BuySellInfo m_BuyInfo;
	char tmp[0x2BC];
};

struct KNpc
{
	//BYTE m_SaveBuffer[131072]; // 0xC0,
	//int m_nPlayerIndex; // 0xC0,
	//char m_AccountName[32]; //0x264,
	//DWORD m_dwID; //0x20400,
	//int m_nIndex; //0x20404,
	////KItemList m_ItemList; //0x2040C,
	////BuySellInfo m_BuyInfo; //0x2522C,
	////KPlayer::KSaveAction m_SaveAction; //0x25A08,
	//char m_PlayerName[32]; //0x25C88,
	////KPlayerFaction m_cFaction; //0x25CE0,
	//int m_nWorldStat; //0x28004,
	//int m_nSectStat; //0x28008,
	//DWORD m_dwEnergy; //0x2876C,
	//int m_nCurEngergy; //0x25C68
	//BuySellInfo m_BuyInfo;
	char tmp[0x1924];
};

struct KLadder
{
	char tmp[0x44CC];
};

struct __attribute__((packed)) __attribute__((aligned(2))) TRoleList
{
	char Name[32];
	INT64 nValue;
	int nSect;
	BYTE byGender;
	BYTE bySort;
};

struct CURPLAYER_SYNC
{
	//BYTE m_SaveBuffer[131072]; // 0xC0,
	//int m_nPlayerIndex; // 0xC0,
	//char m_AccountName[32]; //0x264,
	//DWORD m_dwID; //0x20400,
	//int m_nIndex; //0x20404,
	////KItemList m_ItemList; //0x2040C,
	////BuySellInfo m_BuyInfo; //0x2522C,
	////KPlayer::KSaveAction m_SaveAction; //0x25A08,
	//char m_PlayerName[32]; //0x25C88,
	////KPlayerFaction m_cFaction; //0x25CE0,
	//int m_nWorldStat; //0x28004,
	//int m_nSectStat; //0x28008,
	//DWORD m_dwEnergy; //0x2876C,
	//int m_nCurEngergy; //0x25C68
	//BuySellInfo m_BuyInfo;
	char tmp[0x44];
};

struct KMission
{
	char tmp[0x73A0];
};

struct tagProtoHeader
{
	BYTE	cProtocol;
};

struct G2C_PLAYER_GETHWID : public tagProtoHeader
{
	size_t	dataLength;
	char	szData[100];
};

//#pragma pack (push, 1)
struct tagLogicLoginBew : public tagProtoHeader
{
	CUSTOM_GUID guid;
	//char MacAddress[18];
	//unsigned char nMacAddress[6];
	//unsigned int nIpAddress[4];
	//char HardwareId[35];
	//char IpAddress[16];
	//char MacAddress[18];
} __attribute__((packed, aligned(1)));
//#pragma (pop)

//#pragma pack (push, 1)
struct tagLogicLogin : public tagProtoHeader
{
	GUID guid;
} __attribute__((packed, aligned(1)));
//__attribute__((packed));
//#pragma (pop)

struct lua_State
{
	char tmp[0x84];
};

typedef lua_State lua_State_0;

#pragma pack(1)
struct EXTEND_HEADERS3
{
	BYTE ProtocolFamily;
	BYTE ProtocolID;
};


struct S2R_ADD_TAG_REQUEST : EXTEND_HEADERS3
{
	char szSrcPlayer[32];
	char szTargetPlayer[32];
	char szTagTxt[32];
};

struct S2R_GET_TAGS_REQUEST : EXTEND_HEADERS3
{
	char szSrcPlayer[32];
	char szTargetPlayer[32];
};

struct __attribute__((packed)) __attribute__((aligned(2))) S2R_DEL_TAG_REQUEST : EXTEND_HEADERS3
{
	char szPlayerName[32];
	time_t nTagTime;
};


struct KPlayerTask
{
	int nClear[256];
	char szTask[10][33];
	std::map<int, long unsigned int, std::less<int>, std::allocator<std::pair<const int, long unsigned int> > > m_mapTask;
};

struct tagRelaySDBRecord : EXTEND_HEADERS3
{
	unsigned int uKeyParam1;
	unsigned int uKeyParam2;
	unsigned __int8 byKeyStringLen;
	unsigned __int8 byFlag;
	unsigned __int16 uDataBufferLen;
	unsigned __int8 byOperParam;
};

struct __attribute__((packed)) __attribute__((aligned(1))) TProcessData
{
	unsigned __int8 nProtoId;
	size_t nDataLen;
	unsigned int ulIdentity;
	bool bLeave;
	char pDataBuffer[1];
};
struct tagRelaySDBOper : tagRelaySDBRecord
{
	BYTE byData[1];
};
#pragma pack(0)

struct KSubWorld
{
	char tmp[0x63FC8];
};

struct KRegion
{
	char tmp[0xCC4];
};

struct CChatRoomRelayMsgProcess
{
	char tmp[0x90];
};


struct KPlayerTong
{
	char tmp[0x4F8];
};

struct KTongLogic
{
	char tmp[0x20];
};

struct KTongManagerAgent
{
	char tmp[0x26C];
};

struct KTongMember
{
	char tmp[0x100];
};

struct KTongData
{
	char tmp[0xE38];
};

struct CSpectatorMap
{
	char tmp[0x13D8];
};

struct KInventory
{
  int *m_pArray;
  int m_nMoney;
  int m_nWidth;
  int m_nHeight;
  int m_nUseWidth;
  int m_nUseHeight;
  int m_nGainFlag;
};

enum TongExProtocol : __int32
{
	tp_executor_action_begin = 0x0,
	tp_executor_action_tong_begin = 0x0,
	tp_tong_upgrade = 0x1,
	tp_tong_degrade = 0x2,
	tp_tong_changecamp = 0x3,
	tp_tong_moneyfund2buildfund = 0x4,
	tp_tong_buildfund2warfund = 0x5,
	tp_tong_setannouncement = 0x6,
	tp_tong_claimwar = 0x7,
	tp_executor_action_tong_end = 0x8,
	tp_executor_action_workshop_begin = 0x500,
	tp_workshop_learn = 0x501,
	tp_workshop_remove = 0x502,
	tp_workshop_open = 0x503,
	tp_workshop_close = 0x504,
	tp_workshop_upgrade = 0x505,
	tp_workshop_degrade = 0x506,
	tp_workshop_use = 0x507,
	tp_workshop_maintain = 0x508,
	tp_workshop_setuselevelset = 0x509,
	tp_executor_action_workshop_end = 0x50A,
	tp_executor_action_member_begin = 0x1000,
	tp_member_money2buildfund = 0x1001,
	tp_member_contribution2storeoffer = 0x1002,
	tp_member_storeoffer2contribution1 = 0x1003,
	tp_member_storeoffer2contribution2 = 0x1004,
	tp_member_add = 0x1005,
	tp_member_kick = 0x1006,
	tp_member_retire = 0x1007,
	tp_member_setfigure = 0x1008,
	tp_member_right_assign = 0x1009,
	tp_member_right_award = 0x100A,
	tp_member_right_retract = 0x100B,
	tp_executor_action_member_end = 0x100C,
	tp_executor_action_union_begin = 0x1500,
	tp_union_join_apply = 0x1501,
	tp_union_join_refuse = 0x1502,
	tp_executor_action_union_end = 0x1503,
	tp_executor_action_end = 0x1504,
	tp_nonexecutor_action_begin = 0x4000,
	tp_nonexecutor_action_tonglist_begin = 0x4000,
	tp_unionlist_data = 0x4001,
	tp_tonglist_data = 0x4002,
	tp_member_data = 0x4003,
	tp_nonexecutor_action_tonglist_end = 0x4004,
	tp_nonexecutor_action_tong_begin = 0x4500,
	tp_tong_onlinelist = 0x4501,
	tp_tong_settaskvalue = 0x4502,
	tp_tong_appendtaskvalue = 0x4503,
	tp_tong_appendutaskvalue = 0x4504,
	tp_tong_add = 0x4505,
	tp_tong_remove = 0x4506,
	tp_tong_maletitle = 0x4507,
	tp_tong_femaletitle = 0x4508,
	tp_tong_setmoneyfund = 0x4509,
	tp_tong_addmoneyfund = 0x450A,
	tp_tong_addeventrecord = 0x450B,
	tp_tong_addhistoryrecord = 0x450C,
	tp_tong_maintain = 0x450D,
	tp_tong_weekly_maintain = 0x450E,
	tp_tong_changecurcamp = 0x450F,
	tp_tong_creatmap = 0x4510,
	tp_tong_mapcreated = 0x4511,
	tp_tong_deletemap = 0x4512,
	tp_tong_setmap = 0x4513,
	tp_tong_stunt_set = 0x4514,
	tp_tong_msg = 0x4515,
	tp_map_ban = 0x4516,
	tp_tong_finish_claimwar = 0x4517,
	tp_nonexecutor_action_tong_end = 0x4518,
	tp_nonexecutor_action_workshop_begin = 0x5000,
	tp_workshop_settaskvalue = 0x5001,
	tp_workshop_appendtaskvalue = 0x5002,
	tp_workshop_appendutaskvalue = 0x5003,
	tp_workshop_setuselevel = 0x5004,
	tp_nonexecutor_action_workshop_end = 0x5005,
	tp_nonexecutor_action_member_begin = 0x5500,
	tp_member_settaskvalue = 0x5501,
	tp_member_appendtaskvalue = 0x5502,
	tp_member_appendutaskvalue = 0x5503,
	tp_member_settmptaskvalue = 0x5504,
	tp_member_remove = 0x5505,
	tp_member_delete = 0x5506,
	tp_member_setonline = 0x5507,
	tp_member_settitle = 0x5508,
	tp_member_appendoffer = 0x5509,
	tp_member_dispenseoffer = 0x550A,
	tp_member_setoffer = 0x550B,
	tp_member_changefeature = 0x550C,
	tp_member_rename = 0x550D,
	tp_nonexecutor_action_member_end = 0x550E,
	tp_nonexecutor_action_union_begin = 0x6000,
	tp_union_settaskvalue = 0x6001,
	tp_union_appendtaskvalue = 0x6002,
	tp_union_appendutaskvalue = 0x6003,
	tp_union_add = 0x6004,
	tp_union_remove = 0x6005,
	tp_union_addtong = 0x6006,
	tp_union_removetong = 0x6007,
	tp_nonexecutor_action_union_end = 0x6008,
	tp_nonexecutor_action_end = 0x6009,
	tp_client_begin = 0x8000,
	tp_client_sync_tong_head = 0x8001,
	tp_client_sync_tong_synopsis = 0x8002,
	tp_client_sync_tong_announcement = 0x8003,
	tp_client_sync_tong_weekgoal = 0x8004,
	tp_client_sync_tong_eventrecordlist = 0x8005,
	tp_client_sync_tong_historyrecordlist = 0x8006,
	tp_client_sync_tonglist_pagedata = 0x8007,
	tp_client_sync_tonglist_name = 0x8008,
	tp_client_sync_workshoplist = 0x8009,
	tp_client_sync_member_synopsis = 0x800A,
	tp_client_sync_member_mixdata = 0x800B,
	tp_client_sync_member_right = 0x800C,
	tp_client_sync_memberlist_pagedata = 0x800D,
	tp_client_sync_memberlist_name = 0x800E,
	tp_client_sync_memberlist_figurecount = 0x800F,
	tp_client_sync_memberlist_figure = 0x8010,
	tp_client_sync_union_tonglist = 0x8011,
	tp_client_sync_unionlist_pagedata = 0x8012,
	tp_client_sync_unionlist_name = 0x8013,
	tp_client_sync_refresh_page = 0x8014,
	tp_client_entertongmap = 0x8015,
	tp_client_createtongmap = 0x8016,
	tp_client_configuretongmap = 0x8017,
	tp_client_configuretongstunt = 0x8018,
	tp_client_end = 0x8019,
};
//
//struct TONG_MEMBER_PAGE_DATA_REQ
//{
//	char tmp[0x8];
//};

struct TONG_MEMBER_PAGE_DATA_REQ
{
	BYTE m_byFilterFlag;
	BYTE m_bySortType;
	__attribute__((packed)) __attribute__((aligned(1))) DWORD m_dwTongID;
	WORD m_wPageId;
};

enum NPC_RELATION : __int32
{
	relation_none = 0x1,
	relation_self = 0x2,
	relation_ally = 0x4,
	relation_enemy = 0x8,
	relation_dialog = 0x10,
	relation_nonpc = 0x20,
	relation_hide = 0x40,
	relation_num = 0x41,
	relation_all = 0x1F,
};


typedef int (*lua_CFunction)(lua_State_0*);

typedef struct {
	const char* name;
	lua_CFunction func;

}TLua_Funcs;

union
{
	unsigned int integer;
	unsigned char byte[4];
} itoch;

static int LuaGetMacAddress(lua_State* L);
static int LuaGetLanIP(lua_State* L);
static int LuaGetHardwareId(lua_State* L);
static int LuaSendTelegram(lua_State* L);

typedef void (*_ShutdownClient)(KPlayer*);

typedef void (*_SyncGoldCoin)(KPlayer*, int);
typedef bool (*_SendData)(KPlayer*, const void*, int);


//typedef void (*_lua_pushcclosure)(lua_State*, lua_CFunction, int);
//typedef void (*_lua_setglobal)(lua_State*, const char*);
//typedef void (*_lua_pushstring)(lua_State*, const char*);
//typedef bool (*_lua_gettop)(lua_State*);

typedef const char* (*_lua_tostring)(lua_State*, int);
typedef double (*_lua_tonumber)(lua_State*, int);
typedef int (*_GetPlayerIndex)(lua_State*);
typedef void (*_SendSystemInfo)(int, int, const char*, const char*, int);

struct KItem
{
	char tmp[0x278];
};

struct KItemList
{
	//BYTE tmp[0x4E20];
	//BYTE tmp[0x4C28];
	//KInventory m_Room;
	//BYTE tmp2[(0x4E20 - 0x4C2C)];
	char tmp[0x4E20];
	//BYTE tmp01[0x4C2B];
	//KInventory m_Room[15];
	//BYTE tmp02[0x50];
};

struct KITEM_CONSUME
{
	int nItemIdx;
	int nMode;
};

struct KSkillList
{
	char tmp[0xF38];
};

struct KMagicAttrib
{
	int nAttribType;
	int nValue[3];
};

enum EGameMasterCommand : __int32
{
	GMDoScriptAction = 0x0,
	GMDoWorldScriptAction = 0x1,
	GMRunScriptFile = 0x2,
	GMReloadScriptFile = 0x3,
	GMReloadAllScriptFile = 0x4,
	GMDoWorldScriptFileAction = 0x5,
};

struct TGameMaster_Command
{
	char Command[20];
	EGameMasterCommand eCommandId;
};

struct KNewProtocolProcess
{
	char tmp[0x58];
};

struct CClientConnection
{
	char tmp[0x2A4];
};

struct KPlayerChat
{
	char tmp[0x74];
};

struct CHAT_CHANNELCHAT_CMD
{
	char tmp[0xF];
};

struct KChatProcess
{
	char tmp[0x8];
};

struct KNpcAttribModify
{
	char tmp[0x814];
};

struct KPlayerTempInfo
{
	int lnID;
	char WanIpAddress[50];
};

struct KNpcAI
{
	char tmp[0x8];
};

struct KServerCores
{
	char tmp[0x1005C];
};

struct PLAYER_SEND_CHAT_SYNC
{
	char tmp[0x13C];
};

#pragma pack(1)
struct IItemEnv
{
	char tmp[0x4];
};

struct KItemEnvPlayer : IItemEnv
{
	char tmp[0x408];
};

struct KItemEnvCustom : KItemEnvPlayer
{
	char tmp[0x40C];
};
#pragma pack(0)

struct KPlayerStall
{
	char tmp[0x10];
};

#pragma pack(1)
struct SViewItemInfo
{
	int m_nID;
	BYTE m_btPattern;
	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wDetail;
	__attribute__((packed)) __attribute__((aligned(1))) WORD m_wParticur;
	BYTE m_btSeries;
	BYTE m_btLevel;
	BYTE m_btLuck;
	int m_MagicLevel[6];
	BYTE m_wVersion[2];
	__attribute__((packed)) __attribute__((aligned(1))) DWORD m_dwRandomSeed;
	BYTE m_btExParam0;
	BYTE m_btExParam1;
	DWORD m_ExpiredTime;
	DWORD m_dwLeftUsageTime;
	WORD wPlatinaMagicIdx[2];
	DWORD dwExchangeValue;
	DWORD dwFortuneValue;
};

struct StallViewItemInfo : SViewItemInfo
{
	int nX;
	int nY;
	int nWidth;
	int nHeight;
	int nPrice;
	int m_nCurrentDur;
	int m_nMaxDurability;
};
#pragma pack(0)

typedef void (*_KProtocolProcess)(KProtocolProcess* , int, BYTE*); // 81F76CB call    _ZN14KClientProcess20ProcessLoginProtocolEmPKvj ; KClientProcess::ProcessLoginProtocol(ulong,void const*,uint)
typedef void (*_ProcessMessage)(KClientProcess* , const unsigned long , const char* , size_t ); // 81F76CB call    _ZN14KClientProcess20ProcessLoginProtocolEmPKvj ; KClientProcess::ProcessLoginProtocol(ulong,void const*,uint)
typedef int (*_ProcessLoginProtocol)(KClientProcess*, const unsigned long, const char*, size_t); // 81F76CB call    _ZN14KClientProcess20ProcessLoginProtocolEmPKvj ; KClientProcess::ProcessLoginProtocol(ulong,void const*,uint)
typedef int (*_AttachPlayer)(KPlayerSet*, int, GUID*); // 81F76CB call    _ZN14KClientProcess20ProcessLoginProtocolEmPKvj ; KClientProcess::ProcessLoginProtocol(ulong,void const*,uint)
typedef bool (*_CheckProtocolSize)(KServerCore*, int, const char*, int, bool); // 81F76CB call    _ZN14KClientProcess20ProcessLoginProtocolEmPKvj ; KClientProcess::ProcessLoginProtocol(ulong,void const*,uint)

class KPlayerNew
{
private:
	char			m_NetworkInfo[50];
public:
	KPlayerNew();
	virtual ~KPlayerNew();
	void HookFixAll();
	void HookAll();
	void HookMoc();
	void HookRank();
	//void HookSkillExp();
	void HookCustomScriptLua();
	void Hook_AttachPlayer();
	void Hook_ProcessMessage();
	void Hook_CheckProtocolSize();
	void Hook_ProcessLoginProtocol();
	void Hook_LoadRoleData();
	void Hook_DialogNpc();
	void Hook_TextGMFilter();
	void Hook_ServerSendChat();
	void Hook_PickUpMoney();
	void Hook_StallCoin();
public:
	int		m_nRankIndex;
	KPlayerInfo		m_PlayerInfo;
	KBishopProcess		m_BishopProcess;
	std::map<std::string, int> WorldRank_Data;
	std::map<std::string, int> m_PlayerToxicList;
	std::map<std::string, KPlayerInfo> m_PlayerList;
};
extern KClientProcess ClientProcess;
//extern KPlayerSet PlayerSet;
extern KPlayerNew gKPlayerNew;
extern KPlayerInfo	m_KPlayerInfo[1200];
//extern KLuaScript g_CustomScript;
//extern KNpcTemplate	*g_pNpcTemplate[4000][6][120];

static void sigaction_player(int signal, siginfo_t* si, void* arg);
void __cdecl SafeNpcSkillCommand(KProtocolProcess* This, int nIndex, BYTE* pProtocol);
bool __cdecl SafeProcessClientProtocol(KTongManagerAgent* This, int nPlayerIdx, BYTE* pData);
bool __cdecl SafeSyncPlayer(KNpc* This, KPlayer* pReceiver);
void __cdecl SafeSendSyncData(KNpc* This, int nHideMode);
bool __cdecl SafeOnEnterNewMap(KPlayer* This);
bool __cdecl SafeLoadMap(KSubWorld* This, int nId);
bool __cdecl SafeGroupChat(KServerCore* This, BYTE tgtcls, DWORD tgtid, const void* pData, size_t size);
bool __cdecl SafeLoadDBPlayerInfo(KPlayer* This, BYTE* pPlayerInfo, int* const nStep, unsigned int* const nParam);
unsigned int __cdecl SafeSendMemberCount(KMission* This, const char* pBuffer, unsigned int ulBufferSize);
//bool __cdecl SafeSendMemberCount(KPlayer* This, const char* pData, size_t nLen);
int __cdecl SprintfLogsGiaoDich(char* szBuffer, const char* szText, const char* szAccountSell, const char* szRoleNameSell, const char* szItemName, LPCSTR GmCommand, int nCount, const char* szAccountNameBuy, const char* szRoleNameBuy, DWORD nTime);
void __cdecl SafeNotifyHostLeaveGame(KServerCore* This, LPCSTR pszAccount, LPCSTR pszRole, int nType);
void __cdecl SafeProcessPlayerTongMsg(KClientProcess* This,const unsigned int nPlayerIdx,const char* pData,size_t dataLength);
bool __cdecl ProcessGMCommand(int nPlayerIdx, EGameMasterCommand eCommand, const char* pParam, int nLen);
bool __cdecl SafeEarn(KPlayer* This, int nMoney);
bool __cdecl SafeProcessRelayMsg(KClientProcess* This, unsigned int lnID, int nIndex, const char* pData, size_t dataLength);
bool __cdecl ClientConnection_SendData_Host(CClientConnection* This, const void* pData, unsigned int nLen);
bool __cdecl ClientConnection_SendData_Tong(CClientConnection* This, const void* pData, unsigned int nLen);
bool __cdecl ClientConnection_SendData_Chat(CClientConnection* This, const void* pData, unsigned int nLen);
bool __cdecl SafeExecuteRelayScript(KNewProtocolProcess* This, const void* pData, size_t nLen);
bool __cdecl SafeTextMsgProcessGMCmd(int nPlayerIdx, const char* pGMCmd, int nLen);
bool __cdecl TextGMFilter(int nPlayerIdx, const char* pText, int nLen);
void __cdecl ExecDialogNpc(KPlayer* This, BYTE* pProtocol);
int __cdecl LoadRoleBaseInfo(KPlayer* const This, TRoleBaseInfo* pBaseInfo, BOOL bLogin);
int __cdecl LoadRoleBaseInfo(KPlayer* const This, TRoleBaseInfo* pBaseInfo, BOOL bLogin);
void FixGetIP(char* dest, const char* src);
bool __cdecl Fix_AddMoneyFund(KTongLogic* This, DWORD dwExecutorID, DWORD dwTongID, INT64 llValue);
int __cdecl SubAddSkillExp(KSkillList* This, KMagicAttrib* pAttrib, BOOL bPercent);
bool __cdecl KSkillListAddSkillExp(KNpc* This, int nLauncher, int nSeries, BOOL bIsMelee, void* pData, BOOL bUseAR, int nDoHurt, NPC_RELATION eRelation, int nSkillID);
void __cdecl BishopGatewaySmallPackProcess(KBishopProcess* This, const void* pData, size_t dataLength);
void __cdecl HostProcessTransfer(KHostProcess* This, const void* pData, size_t nLen, KTRAN_UNIT* pUnit);
void __cdecl AddPlayerToWorld(KServerCore* This, int nIndex, _DWORD dwInitKey, _DWORD dwProcessId);
void __cdecl NotifyHostEnterGame(KServerCore* This, LPCSTR pszAccount, LPCSTR pszRole, int nPlayerIndex, DWORD dwNameID, unsigned int lnID, int nLevel);
void __cdecl BishopGatewayLargePackProcess(KBishopProcess* This, const void* pData, size_t dataLength);
int __cdecl BishopLoadRoleData(LPCSTR* This, TRoleData* pRoleData, GUID* pGuid, KACCOUNT_INFO2* pAccountInfo);
bool __cdecl CheckProtocolSize_Hook(KServerCore* This, int nPlayerIndex, const char* pChar, int nSize, BOOL* const bShutDown);
int __cdecl AttachPlayer_Hook(KPlayerSet* This, unsigned int lnID, _GUID* pGuid);
void __cdecl ProcessNetMsg_Hook(KProtocolProcess* This, int nIndex, BYTE* pMsg);
void __cdecl ProcessMessage_Hook(KClientProcess* This, const unsigned long lnID, const char* pData, size_t dataLength);
int __cdecl ProcessLoginProtocol_Hook(KClientProcess* This, const unsigned long lnID, const char* pData, size_t dataLength);

void HookSkillExp();
void __cdecl SafeProcessNetMsg(KProtocolProcess* This, int nIndex, BYTE* pMsg);
bool __cdecl SetRankHook(KPlayer* const cPlayer, CURPLAYER_SYNC* const pData, size_t nLen);
int __cdecl SkillExpOnPlayer();
int __cdecl GetRankNum(std::string cPlayerName);
int __cdecl GetEngergyDamageHook(KPlayer* const cPlayer);
int __cdecl SetEngergyDamageAgi(KNpc* const cKNpc);
void __cdecl Safe_SyncAttributes(KPlayer* const cPlayer);
void __cdecl Safe_SetPhysicsDamage(KNpc* const This, int nMinDamage, int nMaxDamage);
void __cdecl Safe_ReCalcEquip(KPlayer* const cPlayer);
void __cdecl Safe_SetNpcPhysicsDamage(KPlayer* const cPlayer);
void __cdecl Safe_ChangeCurDexterity(KPlayer* const cPlayer, int nData);
void __cdecl gInitClientProtocolSize(int* nProtocolSize);
void __cdecl SafeOnLogin(KPlayer* This);
void __cdecl SafeOnQuit(KPlayer* This);
//bool __cdecl TongLogicMember_Add(KTongLogic* This, DWORD dwExecutorID, DWORD dwTongID, LPCSTR szName, DWORD dwSex, DWORD dwLevel, DWORD dwJoinTime, DWORD dwMemberID, DWORD dwOfferBring);
bool __cdecl ProcessProtocolTongExProtoco(KTongLogic* This, TongExProtocol eTongProtocolID, BYTE* pTongProtocolData, DWORD dwTongProtocolSize);
void __cdecl OnJoinTong(KPlayerTong* This, DWORD dwTongID, DWORD dwOffer);
bool __cdecl CheckAddCondition(KPlayerTong* This, int nPlayerIdx);
void __cdecl ProcessApplyJoinTongFromZhaoMu(KPlayerTong* This, DWORD dwTongID);
bool __cdecl SafeAcceptOrRefuseApplyToRelay(KPlayerTong* This, BOOL bAccept, char* pszName, int nPageIndex);
int __cdecl DisableFunctionLua(lua_State* L);
bool __cdecl sub_F644D90C(int a1);
void __cdecl SafeClearHideState(KNpc* This);
bool __cdecl SafeChatScreen(KServerCore* This, DWORD dwIndex, LPCVOID pMsg, size_t size);
void __cdecl SafeSendDataToNearRegion(KNpc* This, void* pBuffer, DWORD dwSize, int nMaxSync, BOOL bForce);
void __cdecl SafeRemoveStateSkillEffect(KNpc* This, int nSkillID, BOOL bSyncClient);
bool __cdecl SafeChatWorld(KServerCore* This, DWORD dwIndex, const void* pMsg, size_t size);
bool __cdecl SafeProcessSyncReplyProtocol(KClientProcess* This, const unsigned int lnID, int nPlayerIndex, const char* pData, size_t dataLength);
bool __cdecl SafeProcessSyncReplyProtocol(KClientProcess* This, const unsigned int lnID, int nPlayerIndex, const char* pData, size_t dataLength);
bool __cdecl SafeDealSellItem(KPlayerStall* This, int nIdx, int nFullPrice, int nTaxMoney);
bool __cdecl SafeDealBuyItem(KPlayerStall* This, int nIdx, int nX, int nY, int nPrice);
bool __cdecl SafeHasEnoughMoney(KPlayerStall* This, int nNeedMoney);
int __cdecl SafeGetPlayerIdxFromID(KPlayerStall* This, DWORD dwPlayerID);
DWORD __cdecl SafeSyncStallInfoGetPlayerIDFromIdx(KPlayerStall* This, int nPlayerIdx);
bool __cdecl SafeSyncStallInfo(KPlayer* This, void* pData, size_t nLen);
char* guid_to_str(const GUID* id, char* out);
typedef int(*_GetEngergyDamage)(KPlayer*);
	//int m_nIndex;
	//char m_AccountName[32];
	//char m_PlayerName[32];
	//int m_nWorldStat;
	//KBuySell m_BuyInfo;
	//BYTE m_SaveBuffer[131072];

////enum KPlayerOffset
//{
//	m_nPlayerIndex = 0xC0,
//	m_AccountName = 0x264,
//	m_dwID = 0x20400,
//	m_nIndex = 0x20404,
//	m_ItemList = 0x2040C,
//	m_BuyInfo = 0x2522C,
//	m_SaveAction = 0x25A08,
//	m_PlayerName = 0x25C88,
//	m_cFaction = 0x25CE0,
//	m_cTong = 0x25DA4,
//	m_nWorldStat = 0x28004,
//	m_nSectStat = 0x28008,
//	m_dwEnergy = 0x2876C,
//	m_listQuitCall = 0x287C0,
//};
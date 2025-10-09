#pragma once
//#pragma pack(push, 2)

enum c2p_PROTOCOL : int
{
	c2p_NEWSERVER = 0,
	c2p_CHANGESERVER = 1,
	c2p_INGAME = 2,
	c2p_RECONECCT = 3,
	c2p_DISSCONECCT = 4,
};

#define KSG_PASSWORD_MAX_SIZE   64
#define	LOGIN_ACCOUNT_MIN_LEN	6
#define LOGIN_ACCOUNT_MAX_LEN	16
#define LOGIN_PASSWORD_MIN_LEN	8
#define LOGIN_PASSWORD_MAX_LEN	16
#define LOGIN_ROLE_NAME_MIN_LEN	6
#define	LOGIN_ROLE_NAME_MAX_LEN	16

//login action return value
//the folloin' value is used in login connect operaion
#define	LOGIN_ACTION_FILTER					0xff0000	//登陆过程中的操作类型过滤数值
#define LOGIN_A_CONNECT						0x010000	//连接
#define	LOGIN_A_NEWACCOUNT					0x020000	//新建账号（账号服务器）
#define	LOGIN_A_SERVERLIST					0x030000	//获取服务器列表（账号服务器）
#define	LOGIN_A_REPORT						0x040000	//通知服务器在线(定期向服务器发送)
#define	LOGIN_A_LOGIN						0x050000	//登陆服务器（账号、游戏服务器）
#define	LOGIN_A_LOGOUT						0x060000	//注销
#define	LOGIN_A_CHARACTERLIST				0x070000	//获取角色列表（游戏服务器）


#define	LOGIN_R_REQUEST						0	//when the login request is send form client to server
#define	LOGIN_R_SUCCESS						1
#define	LOGIN_R_FAILED						2
#define	LOGIN_R_ACCOUNT_OR_PASSWORD_ERROR	3
#define	LOGIN_R_ACCOUNT_EXIST				4
#define	LOGIN_R_TIMEOUT						5
#define	LOGIN_R_IN_PROGRESS					6
#define	LOGIN_R_NO_IN_PROGRESS				7
#define	LOGIN_R_VALID						8    //合法
#define	LOGIN_R_INVALID						9    //非法
#define LOGIN_R_INVALID_PROTOCOLVERSION     10   //协议版本不兼容，详见 KProtocolDef.h's KPROTOCOL_VERSION
#define LOGIN_R_FREEZE						11	//被冻结
//#pragma	pack(1)
#pragma pack(push, 1)
#define	PROTOCOL_MSG_TYPE	BYTE
#define PROTOCOL_MSG_SIZE	(sizeof(PROTOCOL_MSG_TYPE))
#define	MAX_PROTOCOL_NUM	203

union KAccount_Limittime_Info
{
	unsigned int nLeftTimeOfPoint;
	BOOL bKickOutOldAcc;
};

typedef struct tagKSG_PASSWORD_JX
{
	char szPassword[KSG_PASSWORD_MAX_SIZE];
	//char szPassword[32];    // 现在采用MD5的字符串，由于是32个字符，加上末尾'\0'，需要至少33个空间，因此使用64
	//unsigned char szData1[8];    // 现在采用MD5的字符串，由于是32个字符，加上末尾'\0'，需要至少33个空间，因此使用64
	//char szData2[24];    // 现在采用MD5的字符串，由于是32个字符，加上末尾'\0'，需要至少33个空间，因此使用64
} KSG_PASSWORD_JX;

struct KLoginStructHead2
{
	unsigned short	Size;		//size of the struct, if the struct is been inherit, the size is ref the the derive struct
	int				Param;		//be one of the LOGIN_R_* define value combin with a LOGIN_A_* value.
};

struct tagProtoHeader2
{
	BYTE	cProtocol;
};

struct tagLogicLogin2 : public tagProtoHeader2
{
	GUID guid;
	char HardwareId[33];
};

struct KLoginAccountInfo : KLoginStructHead2
{
	char	        Account[32];
	KSG_PASSWORD_JX    Password;
	unsigned long   nLeftTime;
	unsigned    ProtocolVersion;
	KAccount_Limittime_Info _anon_0;
	DWORD dwLimitTimeFlag;
	DWORD dwLimitTimeOnlineSeconds;
	DWORD dwLimitTimeOfflineSeconds;
	char	        LicenseId[33];
	char	        HardwareId[32];
	char	        HardwareSerialId[36];
	char MacAddress[18];
	char IpAddress[16];
	//DWORD dwAllowLimit;
};

#pragma (pop)

void __cdecl signal_handler(int signal);
void LoadSwitch(int This);
int __stdcall LoadLoginLogicInfo(_DWORD* This);
int __stdcall fConnectToGameSvr(_DWORD a1, const unsigned char* pIpAddress, unsigned short uPort, GUID* pGuid);
int __stdcall hook_sub_545730(int a1);
int __stdcall hook_sub_52B950(int a1, const void* a2, int a3, int a4);
int __stdcall hook_sub_528CF0(int a1, const unsigned char* pIpAddress, unsigned short uPort, GUID* pGuid);
int __stdcall hook_sub_529721(int a1);
int hook_sub_528F40(const void* pBuffer, int nSize);
__int16 __stdcall hook_sub_52CB70(WORD* This);
size_t __stdcall thamgia_tongkim(char* This);
int update_songuoi_tongkim(int a1, int a2);
int __stdcall query_rank(char* This);
//__int16 __stdcall safe_sub_52CB70(WORD* This);
//int __stdcall safe_sub_539090(char* Source, int a2, int a3);
int safe_sub_42F510(char* Str, int Size, int a4, int a5, int a6, DWORD* This);
int safe_sub_406230(int a2, char* Str2, void* Src, unsigned __int16 a5, char a6, int a7, DWORD* This);
void StartModule();
int safe_sub_585D30();
int CheckExitGame();
int __stdcall Func_52CB70(WORD* This);
int __stdcall Func_52CB00(int* This);
int __stdcall Func_509250(char* Str, WORD* This);
//int __stdcall HookChatCheck(void* This, int a2, int a3, int a4, char* Str, int a6);
//int __fastcall FastChatCheck(char* Str, int a2, int a3, int a4, int a5, int a6);
DWORD __stdcall hook_sub_55E7D(int a1, int a2);
DWORD __fastcall sub_10001300();
//DWORD* sub_552970(char* Src, size_t Size, DWORD* This);
//DWORD* __stdcall sub_10003570(DWORD* This, char* Src, size_t Size);
_DWORD* __stdcall sub_10003570(char* Src, size_t Size, _DWORD* This);
int __stdcall sub_557280(int a1, int a2);

class KPlayerSafe
{
public:
	bool m_LoadPluginIconSkill;
	bool m_SendSelectServer;
	bool m_IsGameSetOffline;
	bool m_IsExitGame;
	bool m_WaitForRespon;
	_DWORD m_ConnectionID;
	//iRepresentShell* m_RepresentShell;
	char	        HardwareSerialId[36];
};

extern int nFreeMemoryCount;
extern KPlayerSafe gPlayerSafe;
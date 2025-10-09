// Connection.h: interface for the CConnection class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CriticalSection.h"
#include "HackServerProtocol.h"

#define MAX_BUFF_SIZE	8192
#define AUTH_ADDRESS_A	"15.204.195.65"
//#define AUTH_ADDRESS_A	"45.76.190.5"
#define AUTH_ADDRESS_B	"jx.b92dt.com"
#define AUTH_ADDRESS_C	"jx.nguyenbinhdt.com"
#define AUTH_PORT		1992

//**********************************************//
//********** Auth -> Client **************//
//**********************************************//

struct SDHP_CONNECT_INFO_RECV
{
	PBMSG_HEAD header; // C1:00
	BYTE result;
	BYTE status;
	char IpAddress[16];
	DWORD nPort;
	DWORD nSplashScreen;
	DWORD nLimit;
};

struct PMSG_AUTH_INFO_RECV
{
	PBMSG_HEAD header;
	BYTE result;
	BYTE status;
	DWORD version;
};

//**********************************************//
//********** Client -> Auth **************//
//**********************************************//

struct SDHP_CONNECT_INFO_SEND
{
	PBMSG_HEAD header; // C1:00
	DWORD Version;
	char LicenseId[33];
};

struct PMSG_AUTH_INFO_PROTECT_SEND
{
	PBMSG_HEAD header;
	char LicenseId[33];
	DWORD nPort;
};

class CAuthenticate
{
public:
	int m_SplashScreen;
	int m_ProcessCountTemp;
public:
	CAuthenticate();
	virtual ~CAuthenticate();
	bool Init(void* function);
	void Check();
	bool Connect(char* IpAddress, WORD port);
	void Disconnect();
	bool CheckState();
	bool DataRecv();
	bool DataSend(BYTE* lpMsg, int size);
	bool DataSendEx();
	static DWORD WINAPI ClientWorkerThread(CAuthenticate* lpConnection);
	static void ProtocolCore(BYTE head, BYTE* lpMsg, int size);
	void AuthInfoRecv(PMSG_AUTH_INFO_RECV* lpMsg);
	void ConnectInfoRecv(SDHP_CONNECT_INFO_RECV* lpMsg);
	void RequestInfoSend();
	void SetAuthAddressIP(char* IpAddress, int numb);
private:
	SOCKET m_socket;
	WSAEVENT m_event;
	BYTE m_RecvBuff[MAX_BUFF_SIZE];
	int m_RecvSize;
	BYTE m_SendBuff[MAX_BUFF_SIZE];
	int m_SendSize;
	void (*wsProtocolCore)(BYTE, BYTE*, int);
	CCriticalSection m_critical;
	bool m_AuthReceived;
	char LicenseId[33];
protected:
	char Auth_Address_IP_1[50];
	char Auth_Address_IP_2[50];
	char Auth_Address_IP_3[50];
};

extern CAuthenticate gAuthenticate;
// Connection.h: interface for the CConnection class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CriticalSection.h"

#define MAX_BUFF_SIZE 8192


#define b_CloseConnected		((int(__thiscall*)(char* This)) 0x528B00)//0x528F40 
//#define b_Close		((int(__cdecl*)()) 0x639380)//0x528F40 
#define b_Close		((int(__thiscall*)(_DWORD* a1)) 0x528FC0)//0x528F40 
#define b_CloseClient		((int(__thiscall*)(_DWORD* a1)) 0x528C60)//0x528F40 
#define b_CloseGameServer		((int(__thiscall*)(int a1)) 0x528EA0)//0x528F40 
#define b_sub_539C30_Close		((int(__stdcall*)(int a1)) 0x539C30)//0x528F40 
#define b_sub_41B640		((LRESULT(__thiscall*)(_DWORD** a1, BYTE *a2)) 0x41B640)//0x528F40 
#define b_sub_420740_Close		((int(*)()) 0x420740)//0x528F40 

class CConnection
{
public:
	CConnection();
	virtual ~CConnection();
	bool Init(void* function);
	bool Connect(char* IpAddress,WORD port);
	void Disconnect();
	bool CheckState();
	bool DataRecv();
	bool DataSend(BYTE* lpMsg,int size);
	bool DataSendEx();
	static DWORD WINAPI ClientWorkerThread(CConnection* lpConnection);
private:
	SOCKET m_socket;
	WSAEVENT m_event;
	BYTE m_RecvBuff[MAX_BUFF_SIZE];
	int m_RecvSize;
	BYTE m_SendBuff[MAX_BUFF_SIZE];
	int m_SendSize;
	void (*wsProtocolCore)(BYTE,BYTE*,int);
	CCriticalSection m_critical;
	HANDLE m_WorkerThread;
	//HANDLE m_ReconnectThread;
	bool m_active;
};

extern CConnection gConnection;

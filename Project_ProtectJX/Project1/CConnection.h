#pragma once
#include "stdafx.h"
#include "Protocol.h"

#define MAX_BUFF_SIZE 8192

class CConnection
{
public:
	CConnection();
	virtual ~CConnection();
	bool Init(int port);
	bool Connect(char* IpAddress, WORD port);
	void Disconnect();
	bool CheckState();
	//bool DataRecv();
	bool DataSend(BYTE* lpMsg, int size);
	static void* __cdecl SocketClientWorkerThread(LPVOID lpThisParam);
	int Lock()
	{
		pthread_mutex_lock(&m_cLock);
		return true;
	}

	int UnLock()
	{
		pthread_mutex_unlock(&m_cLock);
		return true;
	}

	int DestroyCritical()
	{
		pthread_mutex_destroy(&m_cLock);
		return true;
	}
private:
	int m_socket;
	
	BYTE m_RecvBuff[MAX_BUFF_SIZE];
	int m_RecvSize;

	BYTE m_SendBuff[MAX_BUFF_SIZE];
	int m_SendSize;

	void (*wsProtocolCore)(BYTE, BYTE*, int);
	pthread_mutex_t m_cLock;
	DWORD m_WorkerThread;
	pthread_t m_clientworker;
	bool m_active;
};

extern CConnection gConnection;

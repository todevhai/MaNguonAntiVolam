#include "stdafx.h"
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include "CConnection.h"

using namespace std;

CConnection gConnection;

CConnection::CConnection() // OK
{
	this->m_socket = -1;

	this->m_WorkerThread = 0;

	this->m_active = 0;
}

CConnection::~CConnection() // OK
{
	//this->Disconnect();
}

bool CConnection::Init(int port) // OK
{
	//this->m_critical.lock();

	struct sockaddr_in servaddr;

	if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		cout << "\nError creating socket..." << endl;
		exit(0);
	}

	cout << "Socket created successfully...\n" << endl;

	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	if (connect(m_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) == 0)
		cout << "Connection to the server " << inet_ntoa(servaddr.sin_addr) << "\n" << endl;

	pthread_mutex_init(&this->m_cLock, NULL);

	memset(this->m_RecvBuff, 0, sizeof(this->m_RecvBuff));

	this->m_RecvSize = 0;

	memset(this->m_SendBuff, 0, sizeof(this->m_SendBuff));

	int m_SendSize = 0;

	this->m_WorkerThread = pthread_create(&this->m_clientworker, NULL, &SocketClientWorkerThread, this);

	this->m_active = 1;

	return 1;

}

void* __cdecl CConnection::SocketClientWorkerThread(LPVOID lpThisParam)
{
	while (true)
	{
		gConnection.m_RecvSize = recv(gConnection.m_socket, (char*)&gConnection.m_RecvBuff[gConnection.m_RecvSize], (MAX_BUFF_SIZE - gConnection.m_RecvSize), 0); //MSG_DONTWAIT
		
		if (gConnection.m_RecvSize == -1)
		{
		}
		else if (gConnection.m_RecvSize == 0)
		{
			printf("Server Failed ....\n");
		}
		else
		{
			gConnection.m_RecvBuff[gConnection.m_RecvSize + 1] = 0;
			printf("'%s'", gConnection.m_RecvBuff);
			
		}

		sleep(100);

	}
}

bool CConnection::CheckState() // OK
{
	if (this->m_socket == -1)
	{
		return 0;
	}

	if (this->m_active == 0)
	{
		return 0;
	}

	return 1;
}

bool CConnection::DataSend(BYTE* lpMsg, int size) // OK
{
	this->Lock();

	if (this->CheckState() <= 0)
	{
		this->Disconnect();
		this->UnLock();
		return 0;
	}

	this->m_SendSize = 0;

	BYTE buff[MAX_BUFF_SIZE];

	memcpy(buff, lpMsg, size);

	this->m_SendSize = send(this->m_socket, this->m_SendBuff, sizeof(this->m_SendBuff), 0);
	
	memset(this->m_SendBuff, 0, sizeof(this->m_SendBuff));

	printf("DataSend success ....\n");

	this->UnLock();

	return 1;

}

void CConnection::Disconnect() // OK
{
	if (this->m_WorkerThread != 0)
	{
		this->m_WorkerThread = 0;
		pthread_exit(&this->m_clientworker);
	}

	if (this->m_socket != -1)
	{
		close(this->m_socket);
		this->m_socket = -1;
	}

	this->m_active = 0;

}
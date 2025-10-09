#include "SocketManager.h"
#include "BishopSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

CClientManager m_GameServer[MAX_GAMESERVER_CONNECT];

CClientManager::CClientManager() // OK
{
    this->m_index = -1;

    this->m_state = SERVER_OFFLINE;

    this->m_socket = -1;

    this->m_OnlineTime = 0;

    this->m_ServerID = 0;

	pthread_mutex_init(&this->m_ClientLock, NULL);

    this->m_RecvSize = 0;

	memset(this->m_RecvBuff, 0, sizeof(this->m_RecvBuff));

	this->m_SendSize = 0;

	memset(this->m_SendBuff, 0, sizeof(this->m_SendBuff));

}

CClientManager::~CClientManager() // OK
{

}

bool CClientManager::CheckState() // OK
{
    if (CLIENT_RANGE(this->m_index) == 0 || this->m_state == SERVER_OFFLINE || this->m_socket == -1)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

void CClientManager::AddClient(int index, char* ip, int socket) // OK
{
	this->m_index = index;

	this->m_state = SERVER_ONLINE;

	memset(this->m_IpAddr, 0, sizeof(this->m_IpAddr));

	strcpy(this->m_IpAddr, ip);

	this->m_socket = socket;

	this->m_RecvSize = 0;

	memset(this->m_RecvBuff, 0, sizeof(this->m_RecvBuff));

	this->m_SendSize = 0;

	memset(this->m_SendBuff, 0, sizeof(this->m_SendBuff));

	gSocketManager.gClientCount = ((this->CheckAlloc() == 0) ? (((++gSocketManager.gClientCount) >= MAX_CLIENT) ? 0 : gSocketManager.gClientCount) : gSocketManager.gClientCount);

	this->m_OnlineTime = time(0);

	this->m_ServerID = 0;

	printf("[ClientManager][%d] AddClient (%s)\n", this->m_index, this->m_IpAddr);

}

void CClientManager::DelClient() // OK
{
	
	printf("[ClientManager][%d] DelClient (%s)\n", this->m_index, this->m_IpAddr);

	//gIpManager.RemoveIpAddress(this->m_IpAddr);

	this->m_index = -1;

	this->m_state = SERVER_OFFLINE;

	memset(this->m_IpAddr, 0, sizeof(this->m_IpAddr));

	this->m_socket = -1;

	this->m_OnlineTime = time(0);

	this->m_ServerID = 0;
}

bool CClientManager::CheckAlloc() // OK
{
	if (this->m_RecvSize == 0 || this->m_index <= 0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

void GS_Info_Recv(BISHOP_GAMESERVER_INFO_SEND* lpMsg, int index)
{

}
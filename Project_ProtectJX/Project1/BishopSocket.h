#pragma once
#include "stdafx.h"
#include "Protocol.h"

#define MAX_CLIENT 5000
#define MAX_BUFF_SIZE 1024
#define MAX_GAMESERVER_CONNECT 32
#define CLIENT_RANGE(x) (((x)<0)?0:((x)>=MAX_CLIENT)?0:1)
#define MAKEWORD(a,b)   ((WORD)(((BYTE)(a))|(((WORD)((BYTE)(b)))<<8)))

enum eClientState
{
    SERVER_OFFLINE = 0,
    SERVER_ONLINE = 1,
};

void GS_Info_Recv(BISHOP_GAMESERVER_INFO_SEND* lpMsg, int index);

class CClientManager
{
public:
    CClientManager();
    virtual ~CClientManager();
    bool CheckState();
    void AddClient(int index, char* ip, int socket);
    bool CheckAlloc();
    void DelClient();
    int Lock()
    {
        pthread_mutex_lock(&m_ClientLock);
        return true;
    }

    int UnLock()
    {
        pthread_mutex_unlock(&m_ClientLock);
        return true;
    }

    int DestroyCritical()
    {
        pthread_mutex_destroy(&m_ClientLock);
        return true;
    }
public:

    int m_index;
    eClientState m_state;
    char m_IpAddr[16];
    int m_socket;

    BYTE m_RecvBuff[MAX_BUFF_SIZE];
    int m_RecvSize;

    BYTE m_SendBuff[MAX_BUFF_SIZE];
    int m_SendSize;

    DWORD m_OnlineTime;
    DWORD m_ThreadHandle;

    bool m_IsGameServer;
    bool m_IsReplay;
    bool m_IsGoddess;
    bool m_IsBishop;
    int m_ServerID;
    pthread_mutex_t m_ClientLock;
    pthread_t m_clientworker;
};

extern CClientManager m_GameServer[MAX_GAMESERVER_CONNECT];
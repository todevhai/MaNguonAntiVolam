#pragma once
#include "stdafx.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <map>

//using namespace std;

//void* CreateWorkerThread(void*);
//void* ServerAcceptThread(void*);
int GetFreeClientIndex();
int SearchFreeClientIndex(int* index, int MinIndex, int MaxIndex, DWORD MinTime);
void ServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size);

class CSocketManager
{
public:
    CSocketManager();
    virtual ~CSocketManager();
    bool Start(WORD port);
    bool CreateAcceptThread();
    int		GetSocketServer() { return m_listen; }
    void Disconnect(int index);
    //bool CreateWorkerThread();
    static void* __cdecl SocketManagerCreateWorkerThread(LPVOID lpThisParam);
    static void* __cdecl SocketManagerAcceptThread(LPVOID lpThisParam);
    bool DataSend(int index, BYTE* lpMsg, int size);
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
public:
    int gClientCount;
private:

    //int client, server;
    //int bufsize = 1024;
    //int portNum = 1500;
    //bool isExit = false;
    //char* buffer = new char[bufsize];

    int m_listen;
    int m_CompletionPort;
    WORD m_port;
    DWORD m_ThreadID;
    pthread_t m_ServerAcceptThread;
    struct sockaddr_in direc;
    socklen_t tamano;
    pid_t pid;
    pthread_mutex_t m_cLock;
    
    //std::map<int, ClientInfo> m_GameServer_List;
};

extern CSocketManager gSocketManager;
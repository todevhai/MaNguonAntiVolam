#include "SocketManager.h"
#include "BishopSocket.h"
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <thread>
#include <future>

using namespace std;

CSocketManager gSocketManager;

int gClientCount;

void ServerProtocolCore(int index, BYTE head, BYTE* lpMsg, int size) // OK
{
    m_GameServer[index].m_OnlineTime = time(0);

    switch (head)
    {
    case 0x00:
        //CHClientInfoRecv((SDHP_CLIENT_INFO_RECV*)lpMsg, index);
        break;
    case 0x01:
        //CHConnectionStatusRecv((SDHP_CONNECTION_STATUS_RECV*)lpMsg, index);
        break;
    case 0x03:
        GS_Info_Recv((BISHOP_GAMESERVER_INFO_SEND*)lpMsg, index);
        break;
    //case 0x04:
    //    CHServerInfoRecv((SDHP_SERVER_INFO_RECV*)lpMsg, index);
    //    break;
    //case 0x05:
    //    CHClientSelectServerRecv((SDHP_CLIENT_SELECT_SERVER_RECV*)lpMsg, index);
    //    break;
    //case 0x06:
    //    switch (lpMsg[5])
    //    {
    //    case 0x00:
    //        HCClientSyncHwidRecv((CLIENT_SYNC_HWID_RECV*)lpMsg, index);
    //        break;
    //    case 0x01:
    //        GameServerSyncHwidRecv((GAME_SYNC_HWID_RECV*)lpMsg, index);
    //        break;
    //    }
    //    break;
    //case 0x09:
    //    CHGameServerHwidRecv((SDHP_SERVER_HWID_RECV*)lpMsg, index);
    //    break;
    //case 0x0A:
    //    CHRemoveClientSelectServerRecv((SDHP_REMOVE_SELECT_SERVER_RECV*)lpMsg, index);
    //    break;
    default:
        gSocketManager.Disconnect(index);
        break;
    }
}

CSocketManager::CSocketManager()
{
    this->m_listen = 0;
    this->m_port = 0;
    this->m_ThreadID = 0;
}

CSocketManager::~CSocketManager() // OK
{
	//this->Clean();
}

bool CSocketManager::Start(WORD port)
{

	this->m_port = port;

    if ((this->m_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "\nError establishing socket..." << endl;
        exit(1);
    }

    this->direc.sin_family = AF_INET;
    this->direc.sin_addr.s_addr = htons(INADDR_ANY);
    this->direc.sin_port = htons(port);

    if ((bind(this->m_listen, (struct sockaddr*)&this->direc, sizeof(this->direc))) < 0) {
        cout << "\nError binding connection..." << endl;
        return -1;
    }

    cout << "Looking for clients " << port << endl;

    listen(this->m_listen, 5);

    pthread_mutex_init(&this->m_cLock, NULL);

    this->m_ThreadID = pthread_create(&this->m_ServerAcceptThread, NULL, &SocketManagerAcceptThread, NULL);

    return 1;

}

void* __cdecl CSocketManager::SocketManagerCreateWorkerThread(LPVOID index)
{
    int count = 0, size = 0, result = 0;

    unsigned int nIndex = *((unsigned int*)index);

    CClientManager* lpClientManager = &m_GameServer[nIndex];

    int max_wait_ms = 5;

    printf("Server [%d] thread [%d]\n", nIndex, lpClientManager->m_clientworker);

    while (lpClientManager->CheckState())
    {
        usleep(100000);

        //printf("Server [%d] thread 2\n", lpClientManager->CheckState());

        fd_set s;
        FD_ZERO(&s);
        FD_SET(lpClientManager->m_socket, &s);
        struct timeval timeout;
        timeout.tv_sec = max_wait_ms / 1000;
        timeout.tv_usec = (max_wait_ms % 1000) * 1000;
        result = select(lpClientManager->m_socket + 1, &s, &s, &s, &timeout);

        usleep(100000);

        if (result == -1) {
            gSocketManager.Disconnect(nIndex);
            break;
        }

        if (result == 0) {
            gSocketManager.Disconnect(nIndex);
            break;
        }

        result = recv(lpClientManager->m_socket, (char*)&lpClientManager->m_RecvBuff[lpClientManager->m_RecvSize], (MAX_BUFF_SIZE - lpClientManager->m_RecvSize), 0);

        if (result <= 0) {
            gSocketManager.Disconnect(nIndex);
            break;
        }

        lpClientManager->m_RecvSize += result;

        if (lpClientManager->m_RecvSize < 3)
        {
            gSocketManager.Disconnect(nIndex);
            break;
        }

        BYTE header, head;

        unsigned char decryptedtext[1024];

        memset(&decryptedtext, 0, sizeof(decryptedtext));

        while (true)
        {

            if (lpClientManager->m_RecvBuff[count] == 0xC1)
            {
                header = lpClientManager->m_RecvBuff[count];
                size = lpClientManager->m_RecvBuff[count + 1];
                head = lpClientManager->m_RecvBuff[count + 3];
            }
            else if (lpClientManager->m_RecvBuff[count] == 0xC2)
            {
                header = lpClientManager->m_RecvBuff[count];
                size = MAKEWORD(lpClientManager->m_RecvBuff[count + 2], lpClientManager->m_RecvBuff[count + 1]);
                head = lpClientManager->m_RecvBuff[count + 4];
            }
            else
            {
                printf("[Connection] Protocol header error (header:%x)\n", lpClientManager->m_RecvBuff[count]);
                gSocketManager.Disconnect(nIndex);
                break;
            }

            if (size <= lpClientManager->m_RecvSize)
            {
                if (header == 0xC1)
                {

                    //memmove(&lpClientManager->m_RecvBuff[count], &decryptedtext, decryptedtext_len);

                    head = lpClientManager->m_RecvBuff[count + 3];

                }
                else
                {
                    //memmove(&lpClientManager->m_RecvBuff[count], &decryptedtext, decryptedtext_len);

                    head = lpClientManager->m_RecvBuff[count + 4];

                }

                ServerProtocolCore(nIndex, head, &lpClientManager->m_RecvBuff[count], size);

                count += size;

                lpClientManager->m_RecvSize -= size;

                if (lpClientManager->m_RecvSize <= 0)
                {
                    //printf("\n >m_RecvSize <= 0 \n");
                    break;
                }

            }
            else
            {
                if (count > 0 && lpClientManager->m_RecvSize > 0 && lpClientManager->m_RecvSize < MAX_BUFF_SIZE)
                {
                    memmove(&lpClientManager->m_RecvBuff[lpClientManager->m_RecvSize], &lpClientManager->m_RecvBuff[count], lpClientManager->m_RecvSize);
                }

                //printf("\n break 3 \n");

                break;

            }
  
        }

    }

    gSocketManager.Disconnect(nIndex);

}

bool CSocketManager::CreateAcceptThread()
{

   /* if ((this->m_ThreadID = pthread_create(&this->m_ServerAcceptThread, NULL, &SocketManagerAcceptThread, NULL)) == 0)
    {
        printf("[SocketManager] CreateThread() failed with error: %d\n");
        return 0;
    }*/

    return 1;

}

void* __cdecl CSocketManager::SocketManagerAcceptThread(LPVOID lpThisParam)
{
    
    struct sockaddr_in client_addr;
    socklen_t length = sizeof(client_addr);
    
    while (true)
    {
        int socket = accept(gSocketManager.GetSocketServer(), (struct sockaddr*)&client_addr, &length);

        if (socket == -1)
        {
            sleep(1);

            continue;

        }

        gSocketManager.Lock();

        printf("[GameServer] %s connected!\n", inet_ntoa(client_addr.sin_addr));

        int index = -1;

        unsigned int ints[8];

        if ((index = GetFreeClientIndex()) == -1)
        {
            close(socket);
            gSocketManager.UnLock();
            continue;
        }

        CClientManager* lpClientManager = &m_GameServer[index];

        lpClientManager->AddClient(index, inet_ntoa(client_addr.sin_addr), socket);

        ints[0] = index;

        lpClientManager->m_ThreadHandle = pthread_create(&lpClientManager->m_clientworker, NULL, &SocketManagerCreateWorkerThread, &ints);

        gSocketManager.UnLock();

        sleep(100000);

    }

}

bool CSocketManager::DataSend(int index, BYTE* lpMsg, int size) // OK
{
    this->Lock();

    if (CLIENT_RANGE(index) == 0)
    {
        this->UnLock();
        return 0;
    }

    CClientManager* lpClientManager = &m_GameServer[index];

    if (lpClientManager->CheckState() == 0)
    {
        this->UnLock();
        return 0;
    }

    BYTE datasend[MAX_BUFF_SIZE];

    memcpy(datasend, lpMsg, size);

    unsigned char ciphertext[1024];

    memset(&ciphertext, 0, sizeof(ciphertext));

    if (datasend[0] == 0xC1)
    {
    }
    else
    {
    }

    if (size > MAX_BUFF_SIZE)
    {
        printf("[SocketManager] Max msg size (Type: 1, Index: %d, Size: %d)\n", index, size);

        this->UnLock();
        return 0;
    }

    int count = 0, result = 0;

    while (size > 0)
    {
        if ((result = send(lpClientManager->m_socket, &datasend[size], size, 0)) == -1)
        {
            if ((lpClientManager->m_SendSize + size) >= MAX_BUFF_SIZE)
            {
                this->UnLock();
                return 0;
            }
            else
            {
                memcpy(&lpClientManager->m_SendBuff[lpClientManager->m_SendSize], &ciphertext[count], size);
                lpClientManager->m_SendSize += size;
                this->UnLock();
                return 1;
            }
        }
        else
        {
            count += result;
            size -= result;
        }
    }

    this->UnLock();

    return 1;
}

void CSocketManager::Disconnect(int index) // OK
{

    this->Lock();

    if (CLIENT_RANGE(index) == 0)
    {
        this->UnLock();
        return;
    }

    CClientManager* lpClientManager = &m_GameServer[index];

    lpClientManager->DelClient();

    lpClientManager->m_ThreadHandle = 0;

    if (lpClientManager->CheckState() != 0)
    {
        close(lpClientManager->m_socket);
    }

    lpClientManager->m_index = -1;

    lpClientManager->m_state = SERVER_OFFLINE;

    lpClientManager->m_socket = -1;

    lpClientManager->m_OnlineTime = 0;

    lpClientManager->m_ServerID = 0;
    
    this->UnLock();

    printf("[Disconnect] Index: %d\n", index);

    pthread_cancel(pthread_self());

    return;

}

int GetFreeClientIndex() // OK
{
    int index = -1;

    int count = gSocketManager.gClientCount;

    if (SearchFreeClientIndex(&index, 0, MAX_GAMESERVER_CONNECT, 10000) != 0)
    {
        return index;
    }

    for (int n = 0; n < MAX_GAMESERVER_CONNECT; n++)
    {
        if (m_GameServer[count].CheckState() == 0)
        {
            return count;
        }
        else
        {
            count = (((++count) >= MAX_GAMESERVER_CONNECT) ? 0 : count);
        }
    }

    return -1;
}

int SearchFreeClientIndex(int* index, int MinIndex, int MaxIndex, DWORD MinTime) // OK
{
    DWORD CurOnlineTime = 0;
    DWORD MaxOnlineTime = 0;

    for (int n = MinIndex; n < MaxIndex; n++)
    {
        if (m_GameServer[n].CheckState() == 0 && m_GameServer[n].CheckAlloc() != 0)
        {
            if ((CurOnlineTime = (time(0) - m_GameServer[n].m_OnlineTime)) > MinTime && CurOnlineTime > MaxOnlineTime)
            {
                (*index) = n;
                MaxOnlineTime = CurOnlineTime;
            }
        }
    }

    return (((*index) == -1) ? 0 : 1);
}

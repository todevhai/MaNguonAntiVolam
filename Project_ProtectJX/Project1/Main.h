#pragma once
#include "SocketManager.h"
#define __USE_GNU
#include <sys/ucontext.h>
#include <signal.h>

#ifdef __cplusplus
extern "C" {
#endif

    //EXPORT int main() __attribute__((constructor));
    //EXPORT int __attribute__((constructor)) Init();
    EXPORT int Init();
    EXPORT void sigaction_segv(int signal, siginfo_t* si, void* arg);
    EXPORT int Goddess();
    //void init()__attribute__((constructor));
    //void before_main(void) 
    //void after_main(void) 
#ifdef __cplusplus
}
#endif

#define KBishopMain		((int(__cdecl*)(void)) 0x8049E34)//0x0063A578 
#define KGameServerMain		((int(__cdecl*)(void)) 0x804AD10)//0x0063A578 

//struct KCONNECTION
//{
//    char szIp[16];
//    int nPort;
//    int nBufSize;
//};

//void Main();
void WorkGoddess();
void WorkBishop();
void WorkGameServer();
int __cdecl SimplyDecryptPassword(char* szPass, const char* szEncrypted);
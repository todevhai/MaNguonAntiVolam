#include "stdafx.h"
#include "Util.h"
#include "Main.h"
#include "Authenticate.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <thread>
#include <pthread.h>
#include <time.h>
#include <mutex>
#include <map>

//#include <linux/socket.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
//#include <unistd.h>
//#include <string.h>
//#include <stdlib.h>
//#include <fcntl.h>
//#include <sys/shm.h>
//#include <thread>
//#include <netinet/tcp.h>
//#include <netdb.h>
//
using namespace std;
using namespace std::this_thread;
pthread_mutex_t m;
pthread_mutexattr_t mAttr;
static pthread_mutex_t cs_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
static pthread_mutex_t cs_mutex_reconnect = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
mutex AutoReconnect;
int run_thread_a = 0;
pthread_mutex_t run_lock_a = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t run_cond_a = PTHREAD_COND_INITIALIZER;

int run_thread_b = 0;
pthread_mutex_t run_lock_b = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t run_cond_b = PTHREAD_COND_INITIALIZER;

CAuthenticate gAuthenticate;
pthread_t thread_id_auth;
//bool m_AuthReceived = false;
//int EventSocket = -1;
//char m_HostAuth1[32] = "103.142.218.222";
//char m_HostAuth2[32] = "jx.b92dt.com";
//char m_HostAuth3[32] = "jx.nguyenbinhdt.com";
//char m_CurrenAuthIP[32] = "103.142.218.222";

CAuthenticate::CAuthenticate() // OK
{
	//this->m_socket = INVALID_SOCKET;

	//this->m_event = WSA_INVALID_EVENT;

	//this->m_AuthReceived = false;

	//this->m_ReconnectStatus = 0;
	/*this->m_AuthReceived = false;
	this->EventSocket = -1;
	this->m_HostAuth[0] = "103.142.218.222";
	this->m_HostAuth[1] = "jx.b92dt.com";
	this->m_HostAuth[2] = "jx.nguyenbinhdt.com";
	this->m_CurrenAuthIP = this->m_HostAuth[0];*/
	this->m_socket = -1;
	this->m_ConnectError = 0;
	this->m_ResCheckAuthen = false;
	this->m_ClientWorkerThread = false;
	this->m_ReconnectStatus = 0;
	this->m_AuthReceived = false;
	this->m_DisableReAuthen = false;
	DWORD ExpiredTime = 0;
	int m_ConnectError = 0;

}

CAuthenticate::~CAuthenticate() // OK
{
	gAuthenticate.Disconnect();
}

time_t GetTickCount()
{
	return time(0);
}

void* CAuthenticate::Init(void*) // OK
{


	//printf("Start work!\n");
	gAuthenticate.m_ClientWorkerThread = false;
	gAuthenticate.m_ReconnectStatus = 0;
	gAuthenticate.m_AuthReceived = false;
	//pthread_mutexattr_settype(&mAttr, PTHREAD_MUTEX_RECURSIVE_NP);
	pthread_t theThread;
	pthread_create(&theThread, NULL, &gAuthenticate.ClientWorkerThread, NULL);
	//CB obj;
	////thread t(obj);
	//thread t(std::ref(obj));
	//t.join();
	//return 1;
}

void CAuthenticate::ProtocolCore(BYTE head, BYTE* lpMsg, int size) // OK
{
	switch (head)
	{
	case 0x01:
		gAuthenticate.AuthInfoRecv((PMSG_AUTH_INFO_RECV*)lpMsg);
		break;
	}
}

void CAuthenticate::AuthInfoRecv(PMSG_AUTH_INFO_RECV* lpMsg) // OK
{
	//this->EventSocket = 5;
	//printf("AuthInfoRecv!\n");
	//printf("Revc %d %d %d %d\n", (int)lpMsg->m_ProtocolVersion, (int)lpMsg->m_AntiDump, (int)lpMsg->m_ThanPhap, (int)lpMsg->m_WorldRank);
	//printf("Code %s %s %d\n", lpMsg->CodeCheck, Md5Check, (int)lpMsg->m_AntiDump);
	if (lpMsg->result <= 0)
	{
		gAuthenticate.AuthenticateError();
		//exit(1);
	}
	else
	{
		if (lpMsg->CodeCheck != Md5Check && (int)lpMsg->m_AntiDump <= 0)
		{
			gAuthenticate.AuthenticateError();
			//exit(1);
		}
		else
		{
			gUtil.PACKETFILTER_ACTIVE = (int)lpMsg->m_AntiDump;

			if (gUtil.PACKETFILTER_ACTIVE < 1)
			{
				gAuthenticate.AuthenticateError();
			}
			else
			{

				memcpy(&gAuthenticate.AllowClientSerial, lpMsg->LicenseId, sizeof(gAuthenticate.AllowClientSerial));

				//memcpy(gUtil.AllowClientSerial, lpMsg->LicenseId, sizeof(gUtil.AllowClientSerial));

				memcpy(gUtil.CheckCode, lpMsg->CodeCheck, sizeof(gUtil.CheckCode));

				memcpy(gUtil.IpAddress, lpMsg->IpAddress, sizeof(gUtil.IpAddress));

				gUtil.THANPHAP_ACTIVE = (int)lpMsg->m_ThanPhap;

				gUtil.WORLDRANK_ACTIVE = (int)lpMsg->m_WorldRank;

				gUtil.nClientVersion = (int)lpMsg->m_ProtocolVersion;

				gUtil.nBuySellID = (int)lpMsg->m_BuySell;

				//gUtil.nLimit = (int)lpMsg->m_Limit;

				gUtil.nLimitIP = (int)lpMsg->m_MaxIpConnect;

				gUtil.nPort = (int)lpMsg->m_Port;

				gUtil.DISABLE_EXP_SKILL_ACTIVE = (int)lpMsg->m_ExpSkillPlayer;

				gUtil.m_RateFakeBattles = (int)lpMsg->m_FakeUserBattles;

				//printf("AllowClientSerial %s\n", gUtil.AllowClientSerial);
				//this->m_ResCheckAuthen = (bool)lpMsg->m_ReAuthen;

				gAuthenticate.m_AuthReceived = true;

				this->m_DisableReAuthen = (bool)lpMsg->m_ReAuthen;

				gAuthenticate.Disconnected();

			}

		}

	}
}

//void* CheckLicense(void*)
//{
//	printf("Start work!\n");
//	pthread_t theThread;
//	pthread_create(&theThread, NULL, &gAuthenticate.ClientWorkerThread, NULL);
//}

void* CAuthenticate::AutoReconnectCheck(void*)
{

	//CAuthenticate* This = (CAuthenticate*)&lpThisParam;
	//printf("%s    %s\n", This->LicenseId, This->CheckCode);
	//AutoReconnect.lock();
	//cs_mutex_reconnect
	int nDelay;
	bool nRec;
	pthread_t theThread;
	DWORD ClientInfoTimeOut = GetTickCount();
	//printf("start while!\n");
	while (true)
	{


		if (!gAuthenticate.m_ClientWorkerThread)
		{
			int nDelay;
			gAuthenticate.m_ReconnectStatus = 0;
			gAuthenticate.m_AuthReceived = false;
			gAuthenticate.EventSocket = -1;

			//strcpy(gAuthenticate.m_HostAuth1, "103.142.218.237");/*
			//strcpy(gAuthenticate.m_HostAuth2, "jx.b92dt.com");
			//strcpy(gAuthenticate.m_HostAuth3, "jx.nguyenbinhdt.com");
			//strcpy(gAuthenticate.m_CurrenAuthIP, "103.142.218.237");*/

			bool nRec = false;;

			gAuthenticate.m_ClientWorkerThread = true;
			if (gAuthenticate.m_ReconnectStatus == 0)
				nDelay = 500;

		}

		if (gAuthenticate.m_ReconnectStatus == 0)
		{
			/*if (gAuthenticate.Init(gAuthenticate.ProtocolCore) == 0)
			{
				gAuthenticate.AuthenticateError();
			}*/
			if (gAuthenticate.Connect(gAuthenticate.m_HostAuth1, AUTH_PORT) == 0)
			{
				if (gAuthenticate.Connect(gAuthenticate.m_HostAuth2, AUTH_PORT) == 0)
				{
					if (gAuthenticate.Connect(gAuthenticate.m_HostAuth3, AUTH_PORT) == 0)
					{
						gAuthenticate.Disconnect();
						gAuthenticate.AuthenticateError();
					}
				}
			}

			if (gAuthenticate.m_socket > 0)
				gAuthenticate.EventSocket = 1;

			memset(gAuthenticate.m_SendBuff, 0, sizeof(gAuthenticate.m_SendBuff));

			gAuthenticate.m_SendSize = 0;

			memset(gAuthenticate.m_RecvBuff, 0, sizeof(gAuthenticate.m_RecvBuff));

			gAuthenticate.m_RecvSize = 0;

			//printf("RequestInfoSend!\n");

			pthread_create(&theThread, NULL, &GlobalDataRecv, NULL);

			//printf("theThread %d!\n", theThread);

			gAuthenticate.RequestInfoSend();

			//printf("RequestInfoSend OK!\n");

		/*	if (gAuthenticate.m_AuthReceived == false)
			{
				gAuthenticate.RequestInfoSend();
			}*/

			//gAuthenticate.DataRecv();

			while (gAuthenticate.m_AuthReceived == false)
			{
				/*printf("GetTickCount %d %d %d\n", GetTickCount(), ClientInfoTimeOut, (GetTickCount() - ClientInfoTimeOut));
				if ((GetTickCount() - ClientInfoTimeOut) > 5000)
				{
					gAuthenticate.AuthenticateError();
				}*/
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}

			/*if (gAuthenticate.EventSocket <= 0)
			{

			}
			else if (gAuthenticate.EventSocket < 1)
			{


			}
			else if (gAuthenticate.EventSocket < 2)
			{

			}
			else if (gAuthenticate.EventSocket < 3)
			{


			}*/


			/*CAuthenticate Authen;
			std::thread t(&CAuthenticate::DataRecv, &Authen);
			t.join();*/


			// 


			//std::thread cThread(&WorkBishop);
			//cThread.join();


			if (gAuthenticate.m_AuthReceived == true)
			{

				gAuthenticate.m_ReconnectStatus = 2;

				nDelay = 10000;

				nRec = false;

				printf("LIBRARY LOAD SUCCESSFULLY\n");
				printf("Protocol Version: %d \n", gUtil.nVersionProtocol);
				printf("===========================================\n");
				if (gUtil.PACKETFILTER_ACTIVE == 1)
				{
					gUtil.nVersionProtocol = (65 + gUtil.nVersionProtocol);

					gUtil.SetByte(0x0804E9CE, gUtil.nVersionProtocol);
				}

			}

			//sleep(5);
			std::this_thread::sleep_for(std::chrono::milliseconds(nDelay));

		}
		else if (gAuthenticate.m_ReconnectStatus == 1)
		{
			if (!nRec)
				printf("[Connection] Reconnecting...");

			nRec = true;

			if (gAuthenticate.Connect(gAuthenticate.m_HostAuth1, AUTH_PORT) == 0)
			{
				if (gAuthenticate.Connect(gAuthenticate.m_HostAuth2, AUTH_PORT) == 0)
				{
					if (gAuthenticate.Connect(gAuthenticate.m_HostAuth3, AUTH_PORT) == 0)
					{
						gAuthenticate.Disconnect();
						gAuthenticate.AuthenticateError();
					}
				}
			}

			memset(gAuthenticate.m_SendBuff, 0, sizeof(gAuthenticate.m_SendBuff));

			gAuthenticate.m_SendSize = 0;

			memset(gAuthenticate.m_RecvBuff, 0, sizeof(gAuthenticate.m_RecvBuff));

			gAuthenticate.m_RecvSize = 0;
			//printf("RequestInfoSend!\n");

			pthread_create(&theThread, NULL, &GlobalDataRecv, NULL);

			//printf("theThread %d!\n", theThread);

			gAuthenticate.RequestInfoSend();

			while (gAuthenticate.m_AuthReceived == false)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(500));
			}

			/*if (gAuthenticate.m_AuthReceived == false)
			{
				gAuthenticate.RequestInfoSend();
			}

			printf("RequestInfoSend OK!\n");

			gAuthenticate.DataRecv();*/

			//printf("ClientWorkerThread %d!\n", gAuthenticate.m_RecvSize);


			//while (gAuthenticate.m_AuthReceived == false)
			//{
			//	std::this_thread::sleep_for(std::chrono::milliseconds(500));
			//}


			printf("[Connection] Reconnected successfully!\n");

			if (gAuthenticate.m_AuthReceived == true)
			{
				gAuthenticate.m_ReconnectStatus = 2;

				nDelay = 10000;

				nRec = false;

				if (gUtil.PACKETFILTER_ACTIVE == 1)
				{
					gUtil.nVersionProtocol = (65 + gUtil.nVersionProtocol);

					gUtil.SetByte(0x0804E9CE, gUtil.nVersionProtocol);

				}
				else
				{

					gUtil.SetByte(0x0804E9CE, 65);

				}

			}

			//sleep(5);
			std::this_thread::sleep_for(std::chrono::milliseconds(nDelay));

		}
		else
		{
			//printf("ClientWorkerThread in ide\n");

			gAuthenticate.DataRecv();

			//sleep(5);
			std::this_thread::sleep_for(std::chrono::milliseconds(500));
		}

	}

	//AutoReconnect.unlock();

}

void* ReCheckAuth(void* arg)
{
	CAuthenticate* This = (CAuthenticate*)arg;

	if (This->m_DisableReAuthen)
	{
		//printf("\nDISABLE-AUTHENTICATION %d\n", This->m_DisableReAuthen);
		pthread_exit(NULL);
	}

	time_t now = time(nullptr);
	int random = 60 + (rand() % 180);
	//int random = 5;
	size_t Minutes = random;
	time_t newTime = now + (60 * Minutes);
	This->nAuthCheckDelay = newTime;

	try
	{
		//printf("\nDISABLE-AUTHENTICATION %d\n", This->m_DisableReAuthen);

		while (true)
		{
			if (This->m_DisableReAuthen)
			{
				//printf("\nDISABLE-AUTHENTICATION %d\n", This->m_DisableReAuthen);
				break;
			}

			if (now >= This->nAuthCheckDelay)
			{

				/*if (This->m_ResCheckAuthen == false)
					break;*/

				random = 60 + (rand() % 180);
				//random = 5;
				Minutes = random;
				time_t newTime = now + (60 * Minutes);
				This->nAuthCheckDelay = newTime;

				printf("\n[JXGUARD] RE-AUTHENTICATION...\n");

//#if AUTH_TYPE == 0
				if (This->ReconnectCheck() != 0)
				{
					This->ReAuthenticateError();
				}
				else
				{
					printf("[JXGUARD] AUTHENTICATION SUCCESSFULLY!\n");
				}
//#else
//				if (!gPluginWeb.ReconnectCheck())
//				{
//					gPluginWeb.ReAuthenticateError();
//				}
//#endif
			}
			else
				now = time(nullptr);

			std::this_thread::sleep_for(std::chrono::milliseconds(5000));

			//printf("m_ResCheckAuthen : %d\n", This->m_ResCheckAuthen);

		}
	}
	catch (...)
	{
	}

	This->m_RunService = false;

	pthread_exit(NULL);

}

int CAuthenticate::ReconnectCheck()
{
	//CAuthenticate* This = this;

	//printf("ReconnectCheck!\n");
	//printf("%s    %s\n", this->LicenseId, this->CheckCode);

	int nDelay;
	bool nRec;
	pthread_t theThread;

	if (!this->m_ClientWorkerThread)
	{
		int nDelay;
		this->m_ReconnectStatus = 0;
		this->m_AuthReceived = false;
		this->m_RunService = false;
		this->m_DisableReAuthen = false;
		this->m_ResCheckAuthen = true;
		this->EventSocket = -1;
		this->m_Port = 1311;
		strcpy(this->m_HostAuth1, "45.32.106.174");
		strcpy(this->m_HostAuth2, "45.32.106.174");
		strcpy(this->m_HostAuth3, "15.204.195.65");
		strcpy(this->m_CurrenAuthIP, "45.32.106.174");

		bool nRec = false;;

		this->m_ClientWorkerThread = true;
		if (this->m_ReconnectStatus == 0)
			nDelay = 500;
	}

	/*if (!this->m_RunService)
	{
		this->m_RunService = true;
		pthread_create(&thread_id_auth, NULL, &ReCheckAuth, this);
	}*/

	this->m_ReconnectStatus = 0;

	try
	{
		while (true)
		{

			if (this->m_ReconnectStatus == 0)
			{
				/*if (gAuthenticate.Init(This->ProtocolCore) == 0)
				{
					This->AuthenticateError();
				}*/
				while (true)
				{
					if (gAuthenticate.Connect(this->m_HostAuth1, this->m_Port) == 0)
					{
						nDelay = 2000;
						std::this_thread::sleep_for(std::chrono::milliseconds(3000));
						if (gAuthenticate.Connect(this->m_HostAuth2, this->m_Port) == 0)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(3000));
							if (gAuthenticate.Connect(this->m_HostAuth3, this->m_Port) == 0)
							{
								if (this->m_ConnectError >= 10)
								{
									if (gAuthenticate.m_AuthReceived)
									{

										gAuthenticate.m_AuthReceived = false;

										memset(&gAuthenticate.AllowClientSerial, 0, sizeof(gAuthenticate.AllowClientSerial));

										//memcpy(gUtil.AllowClientSerial, lpMsg->LicenseId, sizeof(gUtil.AllowClientSerial));

										memset(gUtil.CheckCode, 0, sizeof(gUtil.CheckCode));

										memset(gUtil.IpAddress, 0, sizeof(gUtil.IpAddress));

										gUtil.PACKETFILTER_ACTIVE = 0;

										gUtil.THANPHAP_ACTIVE = 0;

										gUtil.WORLDRANK_ACTIVE = 0;

										gUtil.nBuySellID = 0;

										gUtil.nPort = 0;

										gUtil.DISABLE_EXP_SKILL_ACTIVE = 0;

										return 1;

									}
									else
									{
										//printf("Connect error!\n");
										gAuthenticate.Disconnect();
										this->AuthenticateError();
										return 1;
									}

								}
								std::this_thread::sleep_for(std::chrono::milliseconds(2000));
							}
							else
								break;
						}
						else
							break;
					}
					else
						break;
				}

				memset(this->m_SendBuff, 0, sizeof(this->m_SendBuff));

				this->m_SendSize = 0;

				memset(this->m_RecvBuff, 0, sizeof(this->m_RecvBuff));

				this->m_RecvSize = 0;

				//printf("RequestInfoSend!\n");

				//pthread_create(&theThread, NULL, &gAuthenticate.ClientWorkerThread, this);

				//printf("theThread %d!\n", theThread);
				//pthread_create(&theThread, NULL, &GlobalDataRecv, NULL);

				//printf("theThread %d!\n", theThread);

				gAuthenticate.RequestInfoSend();

				gAuthenticate.DataRecv();

				//printf("RequestInfoSend OK!\n");

		/*		if (this->m_AuthReceived == false)
				{
					gAuthenticate.RequestInfoSend();
				}

				gAuthenticate.DataRecv();*/

				while (gAuthenticate.m_AuthReceived == false)
				{
					/*printf("GetTickCount %d %d %d\n", GetTickCount(), ClientInfoTimeOut, (GetTickCount() - ClientInfoTimeOut));
					if ((GetTickCount() - ClientInfoTimeOut) > 5000)
					{
						gAuthenticate.AuthenticateError();
					}*/
					std::this_thread::sleep_for(std::chrono::milliseconds(500));
				}

				/*CAuthenticate Authen;
				std::thread t(&CAuthenticate::DataRecv, &Authen);
				t.join();*/


				// 


				//std::thread cThread(&WorkBishop);
				//cThread.join();

				if (this->m_AuthReceived == true)
				{

					close(this->m_socket);

					this->m_socket = -1;

					this->m_ReconnectStatus = 1;

					this->m_ConnectError = 0;

					//this->m_ReconnectStatus = 2;

					nDelay = 10000;

					nRec = false;

					return 0;
					//if (this->m_RunService == false)
					//{
					//	if (this->m_IsBishop == true)
					//	{

					//		//pthread_create(&this->thread_id, NULL, &WorkBishop, NULL);
					//		//WorkBishop(NULL);
					//		this->m_RunService = true;
					//		close(this->m_socket);
					//		//pthread_exit(NULL);
					//		return 0;

					//	}
					//	else if (this->m_GameServer == true)
					//	{

					//		//pthread_create(&this->thread_id, NULL, &WorkGameServer, NULL);
					//		//WorkGameServer(NULL);
					//		close(this->m_socket);
					//		this->m_RunService = true;
					//		//pthread_exit(NULL);
					//		return 0;
					//	}

					//	//this->m_Port = gUtil.nPort;

					//	//strcpy(this->m_HostAuth1, gUtil.IpAddress);

					//	//gAuthenticate.Disconnect();

					//}

					/*printf("LIBRARY LOAD SUCCESSFULLY\n");
					printf("Protocol Version: %d \n", gUtil.nVersionProtocol);
					printf("===========================================\n");
					if (gUtil.PACKETFILTER_ACTIVE == 1)
					{
						gUtil.nVersionProtocol = (65 + gUtil.nVersionProtocol);

						gUtil.SetByte(0x0804E9CE, gUtil.nVersionProtocol);
					}*/

					//pthread_exit(NULL);
					//pthread_join(NULL);

				}
				else
					return 1;
				//cThread.detach();
				//sleep(10);
				//printf("cThread.detach %d %d %d!\n", theThread, this->m_AuthReceived, this->m_ReconnectStatus);

				std::this_thread::sleep_for(std::chrono::milliseconds(nDelay));

			}
			else if (this->m_ReconnectStatus == 1)
			{

				printf("RE-AUTHENTICATION...\n");

				while (true)
				{
					if (gAuthenticate.Connect(this->m_HostAuth1, this->m_Port) == 0)
					{
						std::this_thread::sleep_for(std::chrono::milliseconds(2000));
						if (gAuthenticate.Connect(this->m_HostAuth2, this->m_Port) == 0)
						{
							std::this_thread::sleep_for(std::chrono::milliseconds(2000));
							if (gAuthenticate.Connect(this->m_HostAuth3, this->m_Port) == 0)
							{
								if (this->m_ConnectError >= 10)
								{
									if (this->m_AuthReceived = false)
									{

										this->m_AuthReceived = false;

										memset(&gAuthenticate.AllowClientSerial, 0, sizeof(gAuthenticate.AllowClientSerial));

										//memcpy(gUtil.AllowClientSerial, lpMsg->LicenseId, sizeof(gUtil.AllowClientSerial));

										memset(gUtil.CheckCode, 0, sizeof(gUtil.CheckCode));

										memset(gUtil.IpAddress, 0, sizeof(gUtil.IpAddress));

										gUtil.PACKETFILTER_ACTIVE = 0;

										gUtil.THANPHAP_ACTIVE = 0;

										gUtil.WORLDRANK_ACTIVE = 0;

										gUtil.nBuySellID = 0;

										gUtil.nPort = 0;

										gUtil.DISABLE_EXP_SKILL_ACTIVE = 0;

										return 1;

									}
									else
									{
										std::this_thread::sleep_for(std::chrono::milliseconds(5000));
										continue;
										//printf("Connect error!\n");
										//gAuthenticate.Disconnect();
										//this->AuthenticateError();
										//return 1;
									}
								}

								std::this_thread::sleep_for(std::chrono::milliseconds(2000));
							}
							else
								break;
						}
						else
							break;
					}
					else
						break;
				}

				memset(this->m_SendBuff, 0, sizeof(this->m_SendBuff));

				this->m_SendSize = 0;

				memset(this->m_RecvBuff, 0, sizeof(this->m_RecvBuff));

				this->m_RecvSize = 0;

				//printf("RequestInfoSend!\n");

				//pthread_create(&theThread, NULL, &gAuthenticate.ClientWorkerThread, this);

				//printf("theThread %d!\n", theThread);
				//pthread_create(&theThread, NULL, &GlobalDataRecv, NULL);

				//printf("theThread %d!\n", theThread);

				gAuthenticate.RequestInfoSend();

				gAuthenticate.DataRecv();

				while (this->m_AuthReceived == false)
				{
					/*printf("GetTickCount %d %d %d\n", GetTickCount(), ClientInfoTimeOut, (GetTickCount() - ClientInfoTimeOut));
					if ((GetTickCount() - ClientInfoTimeOut) > 5000)
					{
						gAuthenticate.AuthenticateError();
					}*/
					std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				}

				if (this->m_AuthReceived == true)
				{

					//printf("Authenticate successfully!\n");

					close(this->m_socket);

					this->m_socket = -1;

					this->m_ConnectError = 0;

					this->m_ReconnectStatus = 1;

					nDelay = 10000;

					nRec = false;

					//if (this->m_RunService == false)
					//{
					//	if (this->m_IsBishop == true)
					//	{

					//		//pthread_create(&this->thread_id, NULL, &WorkBishop, NULL);
					//		//WorkBishop(NULL);
					//		this->m_RunService = true;
					//		close(this->m_socket);
					//		//pthread_exit(NULL);
					//		return 0;

					//	}
					//	else if (this->m_GameServer == true)
					//	{

					//		//pthread_create(&this->thread_id, NULL, &WorkGameServer, NULL);
					//		//WorkGameServer(NULL);
					//		close(this->m_socket);
					//		this->m_RunService = true;
					//		//pthread_exit(NULL);
					//		return 0;
					//	}

					//	//this->m_Port = gUtil.nPort;

					//	//strcpy(this->m_HostAuth1, gUtil.IpAddress);

					//	//gAuthenticate.Disconnect();

					//}

					/*printf("LIBRARY LOAD SUCCESSFULLY\n");
					printf("Protocol Version: %d \n", gUtil.nVersionProtocol);
					printf("===========================================\n");
					if (gUtil.PACKETFILTER_ACTIVE == 1)
					{
						gUtil.nVersionProtocol = (65 + gUtil.nVersionProtocol);

						gUtil.SetByte(0x0804E9CE, gUtil.nVersionProtocol);
					}*/

					//pthread_exit(NULL);
					//pthread_join(NULL);
					return 0;
				}
				else
					return 1;

			}
			else
			{
				gAuthenticate.DataRecv();
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(nDelay));
		}
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		printf("Error: %s \n", e.what());
		close(this->m_socket);
		this->AuthenticateError();
		return 1;
	}


	close(this->m_socket);
	printf("DISCONNECTED\n");
	this->AuthenticateError();
	return 1;
}

bool CAuthenticate::Connect(char* IpAddress, WORD port) // OK
{

	if (this->m_socket <= 0)
	{
		this->m_AuthReceived = false;

		//printf("INVALID_SOCKET %d!\n");

		this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

		struct sockaddr_in servaddr;
		memset(&servaddr, 0, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = htons(AUTH_PORT);  /// Server Port
		servaddr.sin_addr.s_addr = inet_addr(this->m_HostAuth1);  /// server ip

		if (servaddr.sin_addr.s_addr == INADDR_NONE)
		{
			//printf("INADDR_NONE %d!\n");
			servaddr.sin_addr.s_addr = inet_addr(this->m_HostAuth2);
		}

		if (connect(this->m_socket, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		{
			this->m_ConnectError++;
			return 0;
		}

		//printf("CONNECTED %d!\n");

	}

	return 1;
}

void* __cdecl CAuthenticate::ClientWorkerThread(LPVOID lpThisParam) // OK
{

	CAuthenticate* This = (CAuthenticate*)lpThisParam;
	int nCount = 0;
	int nThread;
	pthread_t theThread;
	while (gAuthenticate.m_socket <= 0)
	{

		nCount++;

		//printf("ClientWorkerThread %d\n", nCount);
		//(void*)gAuthenticate.AutoReconnectCheck(This);
		//pthread_mutex_lock(&cs_mutex_reconnect);
		pthread_create(&theThread, NULL, &gAuthenticate.AutoReconnectCheck, NULL);
		//pthread_mutex_unlock(&cs_mutex_reconnect);
		//if (This->m_socket > 0)
		//{
		//	//pthread_create(&Thread1, NULL, &gAuthenticate.DataRecv, this);
		//	//gAuthenticate.DataRecv();
		//	//printf("gAuthenticate.DataRecv = 1 - %d\n", This->m_ClientWorkerThread);
		//}
		//else
		//{
		//	printf("This->m_socket < 0!\n");
		//	gAuthenticate.Disconnect();
		//	break;
		//}

		//sleep(5);
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}

	//pthread_exit(NULL);

}

bool CAuthenticate::DataRecv() // OK
{
	int count = 0, size = 0, result = 0;

	//printf("\n DataRecv \n");
	//this->EventSocket = 4;
	if ((result = recv(this->m_socket, (char*)&this->m_RecvBuff[this->m_RecvSize], (MAX_BUFF_SIZE - this->m_RecvSize), 0)) == EOF)
	{
		this->m_ReconnectStatus = 1;
		this->Disconnect();
		return 0;
	}

	this->m_RecvSize += result;

	if (this->m_RecvSize < 3)
	{
		this->Disconnect();
		return 0;
	}

	BYTE header, head;
	unsigned char decryptedtext[1024];
	memset(&decryptedtext, 0, sizeof(decryptedtext));

	while (true)
	{
		//printf("Data %d %x %d %x\n", size, (BYTE)m_RecvBuff[count], (DWORD)m_RecvBuff[count + 1], (BYTE)m_RecvBuff[count + 2]);

		if (this->m_RecvBuff[count] == 0xC1)
		{
			header = this->m_RecvBuff[count];
			size = this->m_RecvBuff[count + 1];
			head = this->m_RecvBuff[count + 3];
		}
		else if (this->m_RecvBuff[count] == 0xC2)
		{
			header = this->m_RecvBuff[count];
			size = MAKEWORD(this->m_RecvBuff[count + 2], this->m_RecvBuff[count + 1]);
			head = this->m_RecvBuff[count + 4];
		}
		else
		{
			printf("[Connection] Protocol header error (header: %x)", this->m_RecvBuff[count]);
			this->AuthenticateError();
			return 0;
		}

		if (size <= 0 || size >= MAX_BUFF_SIZE)
		{
			printf("[Connection] Protocol size error (size:%d)", size);
			this->AuthenticateError();
			return 0;
		}

		if (size <= this->m_RecvSize)
		{

			if (header == 0xC1)
			{
				(*(BYTE*)&decryptedtext) = *(BYTE*)(&this->m_RecvBuff[count]);

				//*(BYTE*)(decryptedtext[2]) = (BYTE)(this->m_RecvBuff[count + 2]);

				memcpy((BYTE*)&decryptedtext[2], (BYTE*)&m_RecvBuff[count + 2], 1);

				int decryptedtext_len;

				decryptedtext_len = DecryptStringProtect((unsigned char*)&this->m_RecvBuff[count + 3], (size - 3), (unsigned char*)&decryptedtext[3]) + 3;

				decryptedtext[decryptedtext_len] = '\0';

				decryptedtext[1] = LOBYTE(decryptedtext_len);

				memmove(&this->m_RecvBuff[count], &decryptedtext, decryptedtext_len);

				PacketDecryptData((unsigned char*)&this->m_RecvBuff[count + 3], (size - 3), this->m_RecvBuff[count + 2]);
				head = m_RecvBuff[count + 3];
			}
			else
			{
				(*(BYTE*)&decryptedtext) = *(BYTE*)(&this->m_RecvBuff[count]);

				//*(BYTE*)(decryptedtext[3]) = (BYTE)(this->m_RecvBuff[count + 3]);

				memcpy((BYTE*)&decryptedtext[3], (BYTE*)&m_RecvBuff[count + 3], 1);

				int decryptedtext_len;

				decryptedtext_len = DecryptStringProtect((unsigned char*)&this->m_RecvBuff[count + 4], (size - 4), (unsigned char*)&decryptedtext[4]) + 4;

				decryptedtext[decryptedtext_len] = '\0';

				decryptedtext[2] = LOBYTE(decryptedtext_len);

				memmove(&this->m_RecvBuff[count], &decryptedtext, decryptedtext_len);

				PacketDecryptData((unsigned char*)&this->m_RecvBuff[count + 4], (size - 4), this->m_RecvBuff[count + 3]);
				head = this->m_RecvBuff[count + 4];
			}

			//this->ProtocolCore(head, (BYTE*)&this->m_RecvBuff[count], size);
			switch (head)
			{
			case 0x09:
				gAuthenticate.AuthInfoRecv((PMSG_AUTH_INFO_RECV*)&this->m_RecvBuff[count]);
				break;
			}

			count += size;
			this->m_RecvSize -= size;

			if (this->m_RecvSize <= 0)
			{
				//printf("\n >m_RecvSize <= 0 \n");
				break;
			}
		}
		else
		{
			if (count > 0 && this->m_RecvSize > 0 && this->m_RecvSize < MAX_BUFF_SIZE)
			{
				memmove(this->m_RecvBuff, &this->m_RecvBuff[count], this->m_RecvSize);
			}

			//printf("\n break 3 \n");

			break;
		}
	}

	return 1;
}

//int CAuthenticate::enable_keepalive(int sock) {
//	int yes = 1;
//
//	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(int)) == -1)
//		return -1;
//
//	int idle = 1;
//
//	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(int)) == -1)
//		return -1;
//
//	int interval = 1;
//
//	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(int)) == -1)
//		return -1;
//
//	int maxpkt = 10;
//
//	if (setsockopt(sock, IPPROTO_TCP, TCP_KEEPCNT, &maxpkt, sizeof(int)) == -1)
//		return -1;
//
//	return 0;
//}

bool CAuthenticate::DataSend(BYTE* lpMsg, int size) // OK
{

	AutoReconnect.lock();

	//pthread_mutex_lock(&cs_mutex);

	if (this->m_socket < 1)
	{
		return 0;
	}

	BYTE buff[MAX_BUFF_SIZE];

	memcpy(buff, lpMsg, size);

	unsigned char ciphertext[1024];

	memset(&ciphertext, 0, sizeof(ciphertext));

	if (buff[0] == 0xC1)
	{

		(*(BYTE*)&ciphertext) = (BYTE)(buff[0]);

		//*(BYTE*)(ciphertext[2]) = (BYTE)(buff[2]);
		memcpy((BYTE*)&ciphertext[2], (BYTE*)&buff[2], 1);

		PacketEncryptData(&buff[3], (size - 3), buff[2]);

		size = EncryptStringProtect((unsigned char*)&buff[3], (size - 3), (unsigned char*)&ciphertext[3]) + 3;

		ciphertext[1] = LOBYTE(size);

	}
	else
	{

		(*(BYTE*)&ciphertext) = (BYTE)(buff[0]);

		//*(BYTE*)(ciphertext[3]) = (BYTE)(buff[3]);
		memcpy((BYTE*)&ciphertext[3], (BYTE*)&buff[3], 1);

		PacketEncryptData(&buff[3], (size - 3), buff[2]);

		size = EncryptStringProtect((unsigned char*)&buff[4], (size - 4), (unsigned char*)&ciphertext[4]) + 4;

		ciphertext[2] = LOBYTE(size);

	}

	if (this->m_SendSize > 0)
	{
		if ((this->m_SendSize + size) >= MAX_BUFF_SIZE)
		{
			this->AuthenticateError();
			return 0;
		}
		else
		{
			memcpy(&this->m_SendBuff[this->m_SendSize], ciphertext, size);
			this->m_SendSize += size;
			return 1;
		}
	}

	int count = 0, result = 0;

	while (size > 0)
	{
		if ((result = send(this->m_socket, (char*)&ciphertext[count], size, 0)) == -1)
		{
			if ((this->m_SendSize + size) >= MAX_BUFF_SIZE)
			{
				this->AuthenticateError();
				return 0;
			}
			else
			{
				memcpy(&this->m_SendBuff[this->m_SendSize], &ciphertext[count], size);
				this->m_SendSize += size;
				return 1;
			}
		}
		else
		{
			count += result;
			size -= result;
		}
	}

	//printf("\n Send done 3 \n");

	AutoReconnect.unlock();

	//pthread_mutex_unlock(&cs_mutex);

	return 1;
}

void CAuthenticate::RequestInfoSend() // OK
{

	PMSG_AUTH_INFO_PROTECT_SEND pMsg;

	int size = 0;

	pMsg.header.set(0x09, sizeof(pMsg));

	memcpy(&pMsg.LicenseId, LicenseId, sizeof(pMsg.LicenseId));

	memcpy(&pMsg.CheckCode, CheckCode, sizeof(pMsg.CheckCode));

	//printf("[Connection] (header: %x %d %x)", pMsg.header, pMsg.header.size, pMsg.header.head);

	this->DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CAuthenticate::Disconnect() // OK
{
	close(this->m_socket);
	this->m_socket = -1;
	this->m_ReconnectStatus = 1;
	exit(1);
	//pthread_exit(NULL);
	//pthread_detach(NULL);
}

void CAuthenticate::Disconnected() // OK
{
	close(this->m_socket);
	this->m_socket = -1;
	this->m_ReconnectStatus = 1;
	//pthread_exit(NULL);
	//pthread_detach(NULL);
}

//int hostname_to_ip(char* hostname, char* ip)
//{
//	struct hostent* he;
//	struct in_addr** addr_list;
//	int i;
//
//	if ((he = gethostbyname(hostname)) == NULL)
//	{
//		// get the host info
//		herror("gethostbyname");
//		return 1;
//	}
//
//	addr_list = (struct in_addr**)he->h_addr_list;
//
//	for (i = 0; addr_list[i] != NULL; i++)
//	{
//		//Return the first one;
//		strcpy(ip, inet_ntoa(*addr_list[i]));
//		return 0;
//	}
//
//	return 1;
//}
//
bool CAuthenticate::DelayMe(DWORD delay, DWORD value) // OK
{
	DWORD tick = GetTickCount();

	while (true)
	{
		if ((GetTickCount() - tick) >= delay)
		{
			break;
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(value));
		}
	}

	return 0;
}

int PacketDecryptData(BYTE* lpMsg, int size, BYTE key) // OK
{

	for (int n = 0; n < size; n++)
	{
		lpMsg[n] = (lpMsg[n] + key) ^ 0xA0;
	}

	return 0;
}

int PacketEncryptData(BYTE* lpMsg, int size, BYTE key) // OK
{

	for (int n = 0; n < size; n++)
	{
		lpMsg[n] = (lpMsg[n] ^ 0xA0) - key;
	}

	return 0;

}

int EncryptStringProtect(unsigned char* plaintext, int plaintext_len, unsigned char* ciphertext)
{
	EVP_CIPHER_CTX* ctx;

	int len;

	int ciphertext_len;

	unsigned char* key = (unsigned char*)"44da4@f559cfd628586$894@e86f7e2$eb09";
	//std::cout << key << "\n";
	unsigned char* iv = (unsigned char*)"98@74a$7c7@f9a4$4da9";

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
	{
		//handleErrors();
		return 0;
	}

	/*
	 * Initialise the encryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
	{
		//handleErrors();
		return 0;
	}

	/*
	 * Provide the message to be encrypted, and obtain the encrypted output.
	 * EVP_EncryptUpdate can be called multiple times if necessary
	 */
	if (1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
	{
		//handleErrors();
		return 0;
	}
	ciphertext_len = len;

	/*
	 * Finalise the encryption. Further ciphertext bytes may be written at
	 * this stage.
	 */
	if (1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
	{
		//handleErrors();
		return 0;
	}
	ciphertext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return ciphertext_len;
}

int DecryptStringProtect(unsigned char* ciphertext, int ciphertext_len, unsigned char* plaintext)
{
	EVP_CIPHER_CTX* ctx;

	int len;

	int plaintext_len;

	unsigned char* key = (unsigned char*)"44da4@f559cfd628586$894@e86f7e2$eb09";
	//std::cout << key << "\n";
	unsigned char* iv = (unsigned char*)"98@74a$7c7@f9a4$4da9";

	/* Create and initialise the context */
	if (!(ctx = EVP_CIPHER_CTX_new()))
	{
		//handleErrors();
		return 0;
	}

	/*
	 * Initialise the decryption operation. IMPORTANT - ensure you use a key
	 * and IV size appropriate for your cipher
	 * In this example we are using 256 bit AES (i.e. a 256 bit key). The
	 * IV size for *most* modes is the same as the block size. For AES this
	 * is 128 bits
	 */
	if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
	{
		//handleErrors();
		return 0;
	}

	/*
	 * Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary.
	 */
	if (1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
	{
		//handleErrors();
		return 0;
	}
	plaintext_len = len;

	/*
	 * Finalise the decryption. Further plaintext bytes may be written at
	 * this stage.
	 */
	if (1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
	{
		//handleErrors();
		return 0;
	}
	plaintext_len += len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	return plaintext_len;
}

void CAuthenticate::ReAuthenticateError()
{
	//memset(&this->AllowClientSerial, 0, sizeof(gAuthenticate.AllowClientSerial));

	//memcpy(gUtil.AllowClientSerial, lpMsg->LicenseId, sizeof(gUtil.AllowClientSerial));

	//memset(gUtil.CheckCode, 0, sizeof(gUtil.CheckCode));

	//memset(gUtil.IpAddress, 0, sizeof(gUtil.IpAddress));

	gUtil.THANPHAP_ACTIVE = 0;

	gUtil.WORLDRANK_ACTIVE = 0;

	gUtil.nClientVersion = 0;

	gUtil.nBuySellID = 0;

	gUtil.nPort = 0;

	gUtil.DISABLE_EXP_SKILL_ACTIVE = 0;

	gUtil.m_RateFakeBattles = 0;

	//printf("DISABLE: %s\n", gUtil.GenLicenseID(gAuthenticate.LicenseId).c_str());
	printf("===========================================\n");
	printf("Authorization failed.\n");
	printf("%s\n", ANTI_WEBSITE);

}

void CAuthenticate::AuthenticateError()
{
	//memset(&gAuthenticate.AllowClientSerial, 0, sizeof(gAuthenticate.AllowClientSerial));

	//memcpy(gUtil.AllowClientSerial, lpMsg->LicenseId, sizeof(gUtil.AllowClientSerial));

	//memset(gUtil.CheckCode, 0, sizeof(gUtil.CheckCode));

	//memset(gUtil.IpAddress, 0, sizeof(gUtil.IpAddress));

	gUtil.THANPHAP_ACTIVE = 0;

	gUtil.WORLDRANK_ACTIVE = 0;

	gUtil.nClientVersion = 0;

	gUtil.nBuySellID = 0;

	gUtil.nLimit = 0;

	gUtil.nLimitIP = 0;

	gUtil.nPort = 0;

	gUtil.DISABLE_EXP_SKILL_ACTIVE = 0;

	gUtil.m_RateFakeBattles = 0;

	//printf("DISABLE: %s\n", gUtil.GenLicenseID(gAuthenticate.LicenseId).c_str());
	printf("DISABLE: %s\n", gUtil.ShowIPWan().c_str());
	printf("===========================================\n");
	printf("Authorization failed.\n");
	printf("%s\n", ANTI_WEBSITE);
	exit(1);
}

void CAuthenticate::RequestHwidDataSend() // OK
{

	SYNC_HWID_INFO_SEND pMsg;

	int size = 0;

	pMsg.header.set(0x09, sizeof(pMsg));

	memcpy(&pMsg.LicenseId, LicenseId, sizeof(pMsg.LicenseId));

	memcpy(&pMsg.CheckCode, CheckCode, sizeof(pMsg.CheckCode));

	if (this->m_IsBishop == true)
	{
		pMsg.Type = 1;
	}
	else if (this->m_GameServer == true)
	{
		pMsg.Type = 2;
	}

	//printf("[Connection] (header: %x %d %x)", pMsg.header, pMsg.header.size, pMsg.header.head);

	this->DataSend((BYTE*)&pMsg, pMsg.header.size);
}

//void ErrorHandler(char* errorMessage) {
//
//	printf(errorMessage);
//}

//int Authenticate(char* LicenseId, char* CheckCode) {
//	
//	m_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//	if (m_socket < 0) {
//		ErrorHandler("socket creation failed.\n");
//		closesocket(m_socket);
//		return 0;
//	}
//	// Server address construction
//	struct sockaddr_in sad;
//	memset(&sad, 0, sizeof(sad));
//	sad.sin_family = AF_INET;
//	sad.sin_addr.s_addr = inet_addr("103.142.218.222"); // server IP
//	sad.sin_port = htons(1996);
//
//	// Connection to the server
//
//	if (connect(m_socket, (struct sockaddr*)&sad, sizeof(sad)) < 0) {
//		ErrorHandler("Failed to connect.\n");
//		closesocket(m_socket);
//		return 0;
//	}
//	printf("Connected %d\n", m_socket);
//	//char* LicenseId = "082f2d43c37fc122b4f940534cf6a865"; // String to send
//	//char* CheckCode = "1de05dbb1b44e8bd912b84ef8b05fdf7"; // String to send
//	//int stringLen = strlen(LicenseId);
//	BYTE m_RecvBuff[MAX_BUFF_SIZE];
//	int m_RecvSize;
//	BYTE m_SendBuff[MAX_BUFF_SIZE];
//	int m_SendSize;
//
//	PMSG_AUTH_INFO_PROTECT_SEND pMsg;
//	
//	int size = 0;
//
//	pMsg.header.set(0x01, sizeof(pMsg));
//
//	memcpy(&pMsg.LicenseId, LicenseId, sizeof(pMsg.LicenseId));
//
//	memcpy(&pMsg.CheckCode, CheckCode, sizeof(pMsg.CheckCode));
//
//	//this->DataSend((BYTE*)&pMsg, pMsg.header.size);
//
//	printf("size %d\n", pMsg.header.size);
//
//	BYTE buff[MAX_BUFF_SIZE];
//	//size = pMsg.header.size;
//	memcpy(&buff, (BYTE*)&pMsg, pMsg.header.size);
//
//	printf("Data %d %x %d %x\n", pMsg.header.size, (BYTE)buff[0], (DWORD)buff[1], (BYTE)buff[2]);
//
//	unsigned char ciphertext[MAX_BUFF_SIZE];
//
//	memset(ciphertext, 0, sizeof(ciphertext));
//
//	(*(BYTE*)&ciphertext) = (BYTE)(buff[0]);
//
//	//*(BYTE*)(ciphertext[2]) = (BYTE)(buff[2]);
//
//	memcpy((BYTE*)&ciphertext[2], (BYTE*)&buff[2], 1);
//
//	printf("Data %d %x %x\n", pMsg.header.size, (BYTE)ciphertext[0], (BYTE)ciphertext[2]);
//
//	PacketEncryptData(&buff[3], (pMsg.header.size - 3), buff[2]);
//
//	size = EncryptStringProtect((unsigned char*)&buff[3], (pMsg.header.size - 3), (unsigned char*)&ciphertext[3]) + 3;
//
//	printf("size %d\n", size);
//
//	ciphertext[1] = LOBYTE(size);
//
//	printf("Data %d %x %d %x\n", size, (BYTE)ciphertext[0], (DWORD)ciphertext[1], (BYTE)ciphertext[2]);
//
//	if (m_SendSize > 0)
//	{
//		if ((m_SendSize + size) >= MAX_BUFF_SIZE)
//		{
//			printf("[Connection] Max msg size (size:%d)");
//			return 0;
//		}
//		else
//		{
//			memcpy(&m_SendBuff[m_SendSize], ciphertext, size);
//			return 1;
//		}
//	}
//
//	int count = 0, result = 0;
//
//	while (size > 0)
//	{
//		if ((result = send(m_socket, (char*)&ciphertext[count], size, 0)) < 0)
//		{
//			if ((m_SendSize + size) >= MAX_BUFF_SIZE)
//			{
//				printf("[Connection] Max msg size (size:%d)");
//				return 0;
//			}
//			else
//			{
//				memcpy(&m_SendBuff[m_SendSize], &ciphertext[count], size);
//				m_SendSize += size;
//				return 1;
//			}
//		}
//		else
//		{
//			count += result;
//			size -= result;
//		}
//	}
//
//	/*if (send(m_socket, ciphertext, size, 0) != stringLen) {
//		ErrorHandler("send() sent a different number of bytes than expected");
//		closesocket(m_socket);
//		ClearWinSock();
//		return 0;
//	}*/
//
//	//printf("Received: "); // Setup to print the echoed string
//
//	//while (true) {
//	//	if ((r_count = recv(m_socket, (char*)&m_RecvBuff[m_RecvSize], (MAX_BUFF_SIZE - m_RecvSize), 0)) < 0) {
//	//		ErrorHandler("recv() failed or connection closed prematurely");
//	//		closesocket(m_socket);
//	//		//ClearWinSock();
//	//		return 0;
//	//	}
//	//	m_RecvSize += r_count; // Keep tally of total bytes
//	//	m_RecvBuff[r_count] = '\0'; // Add \0 so printf knows where to stop
//	//	printf("%s", m_RecvBuff); // Print the echo buffer
//	//}
//	DataRecv();
//	
//
//	printf("\nReceived done\n");
//
//	//while (true)
//	//{
//	//	printf("Data %d %x %d %x\n", r_size, (BYTE)m_RecvBuff[r_count], (DWORD)m_RecvBuff[r_count + 1], (BYTE)m_RecvBuff[r_count + 2]);
//	//	if (m_RecvBuff[r_count] == 0xC1)
//	//	{
//	//		header = m_RecvBuff[r_count];
//	//		r_size = m_RecvBuff[r_count + 1];
//	//		head = m_RecvBuff[r_count + 3];
//	//	}
//	//	else if (m_RecvBuff[r_count] == 0xC2)
//	//	{
//	//		header = m_RecvBuff[r_count];
//	//		r_size = MAKEWORD(m_RecvBuff[r_count + 2], m_RecvBuff[r_count + 1]);
//	//		head = m_RecvBuff[r_count + 4];
//	//	}
//	//	else
//	//	{
//	//		//gLog.Output(LOG_CONNECT,"[Connection] Protocol header error (header:%x)",this->m_RecvBuff[count]);
//	//		printf("[Connection] Protocol header error");
//	//		if (m_socket > 0)
//	//		{
//	//			close(m_socket);
//	//			m_socket = -1;
//	//		}
//	//		return 0;
//	//	}
//
//	//	if (r_size <= 0 || r_size >= MAX_BUFF_SIZE)
//	//	{
//	//		printf("[Connection] recv() failed with error:");
//	//		if (m_socket > 0)
//	//		{
//	//			close(m_socket);
//	//			m_socket = -1;
//	//		}
//	//		return 0;
//	//	}
//
//	//	if (r_size <= m_RecvSize)
//	//	{
//
//	//		if (header == 0xC1)
//	//		{
//
//	//			printf("Data %d %x %x\n", r_size, (BYTE)m_RecvBuff[r_count], (BYTE)m_RecvBuff[r_count + 2]);
//
//	//			(*(BYTE*)&decryptedtext) = *(BYTE*)(&m_RecvBuff[r_count]);
//
//	//			//*(BYTE*)(decryptedtext[2]) = (BYTE)(m_RecvBuff[r_count + 2]);
//
//	//			memcpy((BYTE*)decryptedtext[2], (BYTE*)m_RecvBuff[r_count + 2], 1);
//
//	//			printf("Data %d %x %x\n", r_size, (BYTE)decryptedtext[0], (BYTE)decryptedtext[2]);
//
//	//			int decryptedtext_len;
//
//	//			decryptedtext_len = DecryptStringProtect(&m_RecvBuff[r_count + 3], (r_size - 3), (unsigned char*)&decryptedtext[3]) + 3;
//
//	//			decryptedtext[decryptedtext_len] = '\0';
//
//	//			decryptedtext[1] = LOBYTE(decryptedtext_len);
//
//	//			printf("Data %d %x %x\n", decryptedtext_len, (BYTE)decryptedtext[0], (DWORD)decryptedtext[1], (BYTE)decryptedtext[2]);
//
//	//			memmove(&m_RecvBuff[r_count], &decryptedtext, decryptedtext_len);
//
//	//			PacketDecryptData(&m_RecvBuff[r_count + 3], (r_size - 3), m_RecvBuff[r_count + 2]);
//	//			head = m_RecvBuff[r_count + 3];
//	//		}
//	//		else
//	//		{
//	//			(*(BYTE*)&decryptedtext) = *(BYTE*)(&m_RecvBuff[r_count]);
//
//	//			//*(BYTE*)(decryptedtext[3]) = (BYTE)(m_RecvBuff[r_count + 3]);
//	//			memcpy((BYTE*)&decryptedtext[3], (BYTE*)&m_RecvBuff[r_count + 3], 1);
//	//			int decryptedtext_len;
//
//	//			decryptedtext_len = DecryptStringProtect(&m_RecvBuff[r_count + 4], (size - 4), (unsigned char*)&decryptedtext[4]) + 4;
//
//	//			decryptedtext[decryptedtext_len] = '\0';
//
//	//			decryptedtext[2] = LOBYTE(decryptedtext_len);
//
//	//			memmove(&m_RecvBuff[r_count], &decryptedtext, decryptedtext_len);
//
//	//			PacketDecryptData(&m_RecvBuff[r_count + 4], (r_size - 4), m_RecvBuff[r_count + 3]);
//	//			head = m_RecvBuff[r_count + 4];
//	//		}
//
//	//		printf("Data %x %d %d\n", head, r_count, r_size);
//
//	//		//wsProtocolCore(head, &m_RecvBuff[count], size);
//	//		switch (head)
//	//		{
//	//		case 0x01:
//	//			AuthInfoRecv((PMSG_AUTH_INFO_RECV*)&m_RecvBuff[r_count]);
//	//			break;
//	//		}
//
//	//		r_count += r_size;
//	//		m_RecvSize -= r_size;
//
//	//		if (m_RecvSize <= 0)
//	//		{
//	//			break;
//	//		}
//	//	}
//	//	else
//	//	{
//	//		if (r_count > 0 && m_RecvSize > 0 && m_RecvSize < MAX_BUFF_SIZE)
//	//		{
//	//			memmove(m_RecvBuff, &m_RecvBuff[r_count], m_RecvSize);
//	//		}
//
//	//		break;
//	//	}
//	//}
//
//	// Closing connection
//	closesocket(m_socket);
//	//ClearWinSock();
//	printf("\n");
//	return 1;
//}
//
void* GlobalDataRecv(void*)
{
	int count = 0, size = 0, result = 0;

	if ((result = recv(gAuthenticate.m_socket, (char*)&gAuthenticate.m_RecvBuff[gAuthenticate.m_RecvSize], (MAX_BUFF_SIZE - gAuthenticate.m_RecvSize), 0)) < 0)
	{
		printf("recv() failed or connection closed prematurely");
		gAuthenticate.Disconnect();
	}

	gAuthenticate.m_RecvSize += result;
	//printf("%s", gAuthenticate.m_RecvBuff); // Print the echo buffer
	if (gAuthenticate.m_RecvSize < 3)
	{
		printf("m_RecvSize < 3");
		gAuthenticate.Disconnect();
	}

	BYTE header, head;
	unsigned char decryptedtext[1024];
	//gUtil.MemorySet((DWORD)&decryptedtext, 0, sizeof(decryptedtext));
	memset(decryptedtext, 0, sizeof(decryptedtext));
	//printf("Data %x %d %x\n", (BYTE)gAuthenticate.m_RecvBuff[count], (DWORD)gAuthenticate.m_RecvBuff[count + 1], (BYTE)gAuthenticate.m_RecvBuff[count + 3]);

	while (true)
	{
		if (gAuthenticate.m_RecvBuff[count] == 0xC1)
		{
			header = gAuthenticate.m_RecvBuff[count];
			size = gAuthenticate.m_RecvBuff[count + 1];
			head = gAuthenticate.m_RecvBuff[count + 3];
		}
		else if (gAuthenticate.m_RecvBuff[count] == 0xC2)
		{
			header = gAuthenticate.m_RecvBuff[count];
			size = MAKEWORD(gAuthenticate.m_RecvBuff[count + 2], gAuthenticate.m_RecvBuff[count + 1]);
			head = gAuthenticate.m_RecvBuff[count + 4];
		}
		else
		{
			//gLog.Output(LOG_CONNECT,"[Connection] Protocol header error (header:%x)",this->m_RecvBuff[count]);
			printf("[Connection] Protocol header error");
			gAuthenticate.Disconnect();
			return 0;
		}

		if (size <= 0 || size >= MAX_BUFF_SIZE)
		{
			printf("[Connection] recv() failed with error:");
			gAuthenticate.Disconnect();
			return 0;
		}

		if (size <= gAuthenticate.m_RecvSize)
		{

			if (header == 0xC1)
			{

				//printf("Data %d %x %x\n", size, (BYTE)gAuthenticate.m_RecvBuff[count], (BYTE)gAuthenticate.m_RecvBuff[count + 2]);

				(*(BYTE*)&decryptedtext) = *(BYTE*)(&gAuthenticate.m_RecvBuff[count]);

				//*(BYTE*)(decryptedtext[2]) = (BYTE)(m_RecvBuff[count + 2]);

				memcpy((BYTE*)&decryptedtext[2], (BYTE*)&gAuthenticate.m_RecvBuff[count + 2], 1);

				//printf("Data %d %x %x\n", size, (BYTE)decryptedtext[0], (BYTE)decryptedtext[2]);

				int decryptedtext_len;

				decryptedtext_len = DecryptStringProtect(&gAuthenticate.m_RecvBuff[count + 3], (size - 3), (unsigned char*)&decryptedtext[3]) + 3;

				decryptedtext[decryptedtext_len] = '\0';

				decryptedtext[1] = LOBYTE(decryptedtext_len);

				//printf("Data %d %x %x\n", decryptedtext_len, (BYTE)decryptedtext[0], (DWORD)decryptedtext[1], (BYTE)decryptedtext[2]);

				memmove(&gAuthenticate.m_RecvBuff[count], &decryptedtext, decryptedtext_len);

				PacketDecryptData(&gAuthenticate.m_RecvBuff[count + 3], (size - 3), gAuthenticate.m_RecvBuff[count + 2]);
				head = gAuthenticate.m_RecvBuff[count + 3];
			}
			else
			{
				(*(BYTE*)&decryptedtext) = *(BYTE*)(&gAuthenticate.m_RecvBuff[count]);

				//*(BYTE*)(decryptedtext[3]) = (BYTE)(m_RecvBuff[count + 3]);
				memcpy((BYTE*)&decryptedtext[3], (BYTE*)&gAuthenticate.m_RecvBuff[count + 3], 1);
				int decryptedtext_len;

				decryptedtext_len = DecryptStringProtect(&gAuthenticate.m_RecvBuff[count + 4], (size - 4), (unsigned char*)&decryptedtext[4]) + 4;

				decryptedtext[decryptedtext_len] = '\0';

				decryptedtext[2] = LOBYTE(decryptedtext_len);

				memmove(&gAuthenticate.m_RecvBuff[count], &decryptedtext, decryptedtext_len);

				PacketDecryptData(&gAuthenticate.m_RecvBuff[count + 4], (size - 4), gAuthenticate.m_RecvBuff[count + 3]);
				head = gAuthenticate.m_RecvBuff[count + 4];
			}

			printf("Data %x %d %d\n", head, count, size);

			//wsProtocolCore(head, &m_RecvBuff[count], size);
			switch (head)
			{
			case 0x06:
				switch (gAuthenticate.m_RecvBuff[5])
				{
				case 0x00:
					gAuthenticate.FProtectSyncHwidRecv((BISHOP_SYNC_HWID_RECV*)&gAuthenticate.m_RecvBuff[count]);
					break;
				}
				break;
			case 0x09:
				GlobalAuthInfoRecv((PMSG_AUTH_INFO_RECV*)&gAuthenticate.m_RecvBuff[count]);
				break;
			}

			count += size;
			gAuthenticate.m_RecvSize -= size;

			if (gAuthenticate.m_RecvSize <= 0)
			{
				break;
			}
		}
		else
		{
			if (count > 0 && gAuthenticate.m_RecvSize > 0 && gAuthenticate.m_RecvSize < MAX_BUFF_SIZE)
			{
				memmove(gAuthenticate.m_RecvBuff, &gAuthenticate.m_RecvBuff[count], gAuthenticate.m_RecvSize);
			}

			break;
		}
	}

	//printf("[Connection] Protocol header OK");
	/*printf("Data %x %d %x\n", (BYTE)m_RecvBuff[count], (DWORD)m_RecvBuff[count + 1], (BYTE)m_RecvBuff[count + 3]);*/
}

void GlobalAuthInfoRecv(PMSG_AUTH_INFO_RECV* lpMsg) // OK
{
	//printf("Code %d %s %s %d\n", lpMsg->result, lpMsg->CodeCheck, Md5Check, (int)lpMsg->m_AntiDump);
	if (lpMsg->result <= 0)
	{
		gAuthenticate.AuthenticateError();
		//exit(1);
	}
	else
	{
		if (lpMsg->CodeCheck != Md5Check && (int)lpMsg->m_AntiDump <= 0)
		{
			gAuthenticate.AuthenticateError();
			//exit(1);
		}
		else
		{
			if ((int)lpMsg->m_AntiDump < 1)
			{
				gAuthenticate.AuthenticateError();
			}
			else
			{

				memcpy(&gAuthenticate.AllowClientSerial, lpMsg->LicenseId, sizeof(gAuthenticate.AllowClientSerial));

				//memcpy(gUtil.AllowClientSerial, lpMsg->LicenseId, sizeof(gUtil.AllowClientSerial));

				memcpy(gUtil.CheckCode, lpMsg->CodeCheck, sizeof(gUtil.CheckCode));

				memcpy(gUtil.IpAddress, lpMsg->IpAddress, sizeof(gUtil.IpAddress));

				gUtil.THANPHAP_ACTIVE = (int)lpMsg->m_ThanPhap;

				gUtil.WORLDRANK_ACTIVE = (int)lpMsg->m_WorldRank;

				gUtil.nClientVersion = (int)lpMsg->m_ProtocolVersion;

				gUtil.nBuySellID = (int)lpMsg->m_BuySell;

				//gUtil.nLimit = (int)lpMsg->m_Limit;

				gUtil.nLimitIP = (int)lpMsg->m_MaxIpConnect;

				gUtil.nPort = (int)lpMsg->m_Port;

				gUtil.DISABLE_EXP_SKILL_ACTIVE = (int)lpMsg->m_ExpSkillPlayer;

				//printf("m_FakeUserBattles %d\n", lpMsg->m_FakeUserBattles);

				//gAuthenticate.m_RateFakeBattles = (int)lpMsg->m_FakeUserBattles;

				/*printf("AllowClientSerial %s\n", gUtil.AllowClientSerial);

				printf("m_FakeUserBattles %d\n", gUtil.DISABLE_EXP_SKILL_ACTIVE);
				printf("m_FakeUserBattles %d\n", gUtil.nBuySellID);
				printf("m_FakeUserBattles %d\n", gUtil.WORLDRANK_ACTIVE);
				printf("m_FakeUserBattles %d\n", gUtil.THANPHAP_ACTIVE);*/
				gUtil.PACKETFILTER_ACTIVE = (int)lpMsg->m_AntiDump;

				gAuthenticate.m_AuthReceived = true;
			}

		}

	}
}

void CAuthenticate::FProtectSyncHwidRecv(BISHOP_SYNC_HWID_RECV* lpMsg)
{
	HARDWARE_ID_LIMIT_INFO info;

	memcpy(info.HardwareId, lpMsg->HardwareId, sizeof(info.HardwareId));

	info.HardwareIdCount = 1;

	memcpy(info.Character, lpMsg->Character, sizeof(info.Character));

	memcpy(info.m_IpAddr, lpMsg->m_IpAddr, sizeof(info.m_IpAddr));

	memcpy(info.IpAddress, lpMsg->IpAddress, sizeof(info.IpAddress));

	memcpy(info.MacAddress, lpMsg->MacAddress, sizeof(info.MacAddress));

	std::map<std::string, HARDWARE_ID_LIMIT_INFO>::iterator it = this->m_HardwareIdLimitInfo.find(std::string(lpMsg->HardwareId));

	if (it == this->m_HardwareIdLimitInfo.end())
	{
		this->m_HardwareIdLimitInfo.insert(std::pair<std::string, HARDWARE_ID_LIMIT_INFO>(std::string(lpMsg->HardwareId), info));
	}
	else
	{
		it->second.HardwareIdCount++;
	}
}
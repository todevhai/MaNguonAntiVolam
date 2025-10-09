#include "stdafx.h"
#include "Protect.h"
#include "HackServerProtocol.h"
#include "Util.h"
//#include <KRepresentShell2.h>
#include "KPlayerSafe.h"
#include <fstream>
#include "INIReader.h"
#include "Connection.h"
#include "Anti-Cheat-Client.h"
#include "SkillState.h"
#include <IPTypes.h>
#include <IPHlpApi.h>
#include <cstdarg>
#include <csignal>
#include <memory.h>

using namespace std;

KPlayerSafe gPlayerSafe;
bool IsInGame = false;

#define b_sub_529540		((int(__cdecl*)(int a1, unsigned char *a2)) 0x529540)//0x0063A578 
#define b_sub_53BF30_v8		((int(__cdecl*)(int a1, unsigned char *a2)) 0x53BF30)//0x0063A578 

#define b_sub_4F18E0		((int(__cdecl*)(int a1, int a2)) 0x4F18E0)//0x528F40 
#define b_sub_4F2E40		((size_t(__stdcall*)(char* This)) 0x4F2E40)//0x528F40 

#define b_sub_52CB70		((__int16(__thiscall*)(WORD* This)) 0x52CB70)//0x528F40 
#define b_sub_528F40		((int(__thiscall*)(_DWORD*This, int a2, int a3)) 0x528F40)//0x528F40 
#define b_sub_52BA50		((int(__stdcall*)(int a1)) 0x52BA50)//0x528F40 
#define b_sub_545730		((int(__cdecl*)(int a1)) 0x545730)//0x528F40 
#define b_sub_539C30		((int(__stdcall*)(int a1)) 0x539C30)//0x528F40 
#define b_sub_528CF0		((int(__thiscall*)(int This, int a2, __int16 a3, void *Src)) 0x528CF0)//0x528F40 
#define b_sub_53E340_v8		((int(__thiscall*)(_DWORD*This, int a2, int a3)) 0x53E340)//0x528F40 
#define b_sub_53B900_v8		((int(__thiscall*)(_DWORD *This, int a2, int a3)) 0x53B900)//0x528F40 

#define execute_sub_528F40		((int(__thiscall*)(_DWORD*This, int a2, int a3)) 0x528F40)//0x528F40 
#define b_sub_52A020		((unsigned int(__thiscall*)(unsigned __int16* a2)) 0x52A020)//0x52A020 
//#define b_sub_529270		((int(__fastcall*)(char a3, int a4)) 0x529270)//0x52A020 

#define b_sub_529270		((int(__fastcall*)(int a1, int a2, char a3, int a4)) 0x529270)//0x0063A578 
#define b_sub_53BC60_v8		((int(__fastcall*)(int a1, int a2, char a3, int a4)) 0x53BC60)//0x0063A578 

#define b_sub_529220		((_DWORD(__thiscall*)(_DWORD *This, unsigned __int8 a2, int a3)) 0x529220)//0x0063A578 
#define b_sub_528CF0		((int(__thiscall*)(_DWORD This, const unsigned char* pIpAddress, unsigned short uPort, GUID* pGuid)) 0x528CF0)//0x0063A578 
#define b_sub_420740		((int(*)()) 0x420740)//0x528F40 
#define b_sub_420740		((int(*)()) 0x420740)//0x528F40 
#define b_sub_52CB70		((__int16(*)(WORD *This)) 0x52CB70)//0x528F40 

#define b_sub_526CB0		((int(__thiscall*)(DWORD *This)) 0x526CB0)
#define b_sub_55E7D0		((int(__stdcall*)(int a1, int a2)) 0x55E7D0)
#define b_sub_552970		((_DWORD*(__thiscall*)(_DWORD *This, char *Str, size_t Size)) 0x552970)

#define b_sub_539090		((int(*)(char *Source, int, int)) 0x539090)//0x528F40 
//#define b_sub_42F510		((int(*)(char *Source, int, int)) 0x42F510)//0x528F40 
#define b_sub_528B00		((int(__thiscall*)(int This)) 0x528B00)//0x528F40 
#define b_sub_406230		((int(__thiscall*)(DWORD *This, int a2, char* Str2, void* Src, unsigned __int16 a5, char a6, int a7)) 0x406230)//0x528F40 
#define b_sub_42F510		((int(__thiscall*)(char *Str, int Size, int a4, int a5, int a6, DWORD *This)) 0x42F510)//0x528F40 
#define b_CloseGameServer		((int(__thiscall*)(int a1)) 0x528EA0)//0x528F40 
#define b_sub_4305A0		((int(__thiscall*)(DWORD* This, int a1)) 0x4305A0)//0x528F40 
#define b_IsConnecting		((int(__thiscall*)(_DWORD This, int a1)) 0x5292F0)//0x528F40 
#define b_TobeDisconnect		((BYTE*(__thiscall*)(BYTE* This)) 0x529250)//0x528F40 
#define b_LoginLogicInfo		((int(__thiscall*)(_DWORD* This)) 0x52A920)//0x528F40 
#define b_52CB70		((int(__thiscall*)(WORD* This)) 0x52CB70)//0x528F40 
#define b_52CB00		((int(__thiscall*)(int This)) 0x52CB00)//0x528F40 
#define b_509250		((int(__thiscall*)(WORD* This, char *Str)) 0x509250)//0x528F40 
#define b_sub_557280		((int(__stdcall*)(int a1, int a2)) 0x557280)

//char pAccName[32];
//char pAccPwd[64];
//iRepresentShell* m_RepresentShell;
#define  PHYSICSSKILLICON "\\spr\\Ui\\����ͼ��\\icon_sk_wa_23.spr"

bool m_LoadPluginIconSkill = false;
int nFreeMemoryCount = 20;
static unsigned gs_holdrand = time(NULL);

DWORD m_BassAddress = 0;

void signal_handler(int signal)
{
	//printf("signal_handler: %d\n", signal);
	//signal(SIGINT, signal_handler);
	//printf("Got SIGSEGV at address: %p\n", si->si_addr);
	//exit(EXIT_FAILURE);
	throw(signal);
	//exit(signal);
}

static inline unsigned _Rand()
{
	gs_holdrand = gs_holdrand * 244213L + 1541021L;

	return gs_holdrand;
}

void _RandMemSet(int nSize, unsigned char* pbyBuffer)
{
	
	_ASSERT(pbyBuffer);

	while (nSize--)
	{
		*pbyBuffer++ = (unsigned char)_Rand();
	}
}

int safe_sub_42F510(char* Str, int Size, int a4, int a5, int a6, DWORD* This)
{
	//signal(11, signal_handler);
	//int v3;
	//v3 = *((DWORD*)Source + 17);
	/*ofstream myfile;
	myfile.open("C:\\safe_sub_494F92.txt");
	myfile << This << "\n";
	myfile << Str << "\n";
	myfile << Size << "\n";
	myfile << a4 << "\n";
	myfile << a5 << "\n";
	myfile << a6 << "\n";
	myfile.close();*/

	//free(((DWORD*)This + 65));
	//free(((DWORD*)This + 66));
	//free(&((DWORD*)This + 66)[65]);
	//free(&((DWORD*)This + 66)[66]);

	//::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

	//SetUnhandledExceptionFilter(NULL);

	//return b_sub_42F510(This, Str, Size, a4, a5, a6);

	//return b_sub_42F510(This, Str, Size, a4, a5, a6);

	return b_sub_42F510(Str, Size, a4, a5, a6, This);

	//try
	//{
	//	//_ASSERT(*((DWORD*)This + 65));
	//	//_ASSERT(*((DWORD*)This + 66));

	//	//return b_sub_42F510(Str, Size, a4, a5, a6);

	//	return b_sub_42F510(Str, Size, a4, a5, a6, This);
	//	 
	//	//if (This != NULL && Str != NULL)
	//	//{
	//	//	//return b_sub_4305A0((DWORD*)This, Size);
	//	//	return b_sub_42F510(This, Str, Size, a4, a5, a6);
	//	//}
	//	//else
	//	//	return 0;
	//}
	//catch (const std::exception& ex) {
	//	return 0;
	//}
	//catch (const std::string& ex) {
	//	return 0;
	//}
	//catch (const std::out_of_range& e) {
	//	return 0;
	//}
	//catch (...) {
	//	return 0;
	//}
}

int safe_sub_585D30()
{
	return 0;
}

int safe_sub_406230(int a2, char* Str2, void* Src, unsigned __int16 a5, char a6, int a7, DWORD* This)
{
	DWORD lpOldMode;
	::SetThreadErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX, &lpOldMode);
	try
	{
		return b_sub_406230(This, a2, Str2, Src, a5, a6, a7);
	}
	catch (const std::exception& ex) {
		return 0;
	}
	catch (const std::string& ex) {
		return 0;
	}
	catch (const std::out_of_range& e) {
		return 0;
	}
	catch (...) {
		return 0;
	}
}


//int __stdcall safe_sub_539090(char* Source, int a2, int a3)
//{
//	int v3;
//	v3 = *((DWORD*)Source + 17);
//	ofstream myfile;
//	myfile.open("C:\\safe_sub_539090.txt");
//	myfile << Source << "\n";
//	myfile << (char*)a2 << "\n";
//	myfile << *(DWORD*)a3 << "\n";
//	myfile << v3 << "\n";
//	myfile << *((DWORD*)Source + 16) << "\n";
//	myfile.close();
//	return b_sub_539090(Source, a2, a3);
//}


//__int16 __stdcall safe_sub_52CB70(WORD* This)
//{
//	signal(11, signal_handler);
//
//	__int16 v2;
//	v2 = 0;
//
//	ofstream myfile;
//	myfile.open("C:\\safe_sub_52CB70.txt");
//	myfile << v2 << "\n";
//	myfile.close();
//
//	try {
//		_ASSERT(This[218]);
//		if (This[218] != NULL)
//		{
//			v2 = This[218];
//			This[218] = v2 + 1;
//		}
//	}
//	catch (...) {
//		//throw;
//	}
//	
//	return v2;
//}

int __stdcall query_rank(char* This)
{
	return 1;
}

int CheckExitGame()
{
	IsInGame = false;
	//MessageBox(0, "CheckExitGame", "Info", MB_OK | MB_ICONINFORMATION);
	gPlayerSafe.m_IsGameSetOffline = true;

	Sleep(300);

	/*int nCountExit = 0;

	while (gPlayerSafe.m_IsExitGame == false)
	{
		nCountExit++;

		Sleep(300);

		if (nCountExit == 30)
			break;
	}*/

	if (gProtect.m_CheckCount == 1)
	{
		ExitGameSend();
		Sleep(300);
	}

	TerminateProcess(GetCurrentProcess(), 0);

	return b_sub_420740();
}

void StartModule() {
    //int nProtocolVersion = 0;
    //std::string temp;
    //SetCompleteHook(0xE8, 0x52BA17, &hook_sub_528F40); // 0x71E14FB0
	//SetCompleteHook(0xE8, 0x52980A, &hook_sub_52B950);
}

void LoadSwitch(int This)
{
	//ofstream myfile;
	//myfile.open("E:\\example.txt");
	//myfile << This << "\n";
	//myfile.close();
	//void (*gsv)(int This) = (void (*)(int This))0x459B40;
	//gsv(This);
}

int __stdcall Func_52CB70(WORD* This)
{
	This[4] = *This;
	This[7] = 0;

	return b_52CB70(This);
}

int __stdcall Func_52CB00(int* This)
{
	This[4] = *This;
	This[7] = 0;

	return b_52CB00(*This);
}

int __stdcall Func_509250(char* Str, WORD* This)
{
	This[4] = *This;
	This[7] = 0;

	return b_509250(This, Str);
}

int __stdcall LoadLoginLogicInfo(_DWORD* This)
{
	int v2;
	char Buffer[8];

	//v2 = b_LoginLogicInfo(This);
	//0x68F9F8
	//ofstream myfile;
	//myfile.open("E:\\example.txt");
	////myfile << itoa(v2, Buffer, 10) << "\n";
	//myfile << &This[3] << "\n";
	//myfile.close();

	return b_LoginLogicInfo(This);

	//if (v2)
	//{

	//}
	//else
	//{
	//	return v2;
	//}

	//return b_LoginLogicInfo(This);
}

int __stdcall hook_sub_545730(int a1)
{
	if (a1 != 0)
	{
		gPlayerSafe.m_ConnectionID = a1;
		gProtect.m_IsInGame = true;
		if (gProtect.m_CheckCount == 1)
			CHClientSelectServerSend(c2p_INGAME);
	}
	else
	{
		gProtect.IsInGame = false;
		gProtect.m_IsInGame = false;
		if (gProtect.m_CheckCount == 1)
			CHRemoveSelectServerSend();
	}
	//return b_sub_545730(*(_DWORD*)((void*)a1));
	return b_sub_539C30(a1);

}

int __stdcall fConnectToGameSvr(_DWORD a1, const unsigned char* pIpAddress, unsigned short uPort, GUID* pGuid)
{

	gProtect.m_IsInGame = true;

	CHClientSelectServerSend(c2p_INGAME);

	return b_sub_528CF0(a1, pIpAddress, uPort, pGuid);

}

int __stdcall hook_sub_529721(int a1) {
	//char pIpServer[32];

	if (gProtect.m_CheckCount == 1)
	{
		if (gConnection.CheckState() != 0)
		{

			//if (b_IsConnecting(0x6DE818, 0))
			if (!CheckInGame())
			{
				gProtect.m_SendSelectServer = true;
				char	pIpServer[33];
				const unsigned char* IpAddress = (const unsigned char*)&a1;
				sprintf(pIpServer, "%d.%d.%d.%d", IpAddress[0], IpAddress[1],
					IpAddress[2], IpAddress[3]);

				//ofstream myfile;
				//myfile.open("E:\\example3.txt");
				//myfile << pIpServer << "\n";
				//myfile.close();

				if (strlen(pIpServer) < 10)
					return 0;

				if (memcmp(gProtect.m_IpAddressSelect, pIpServer, 33) != 0)
				{
					memcpy(gProtect.m_IpAddressSelect, pIpServer, sizeof(gProtect.m_IpAddressSelect));

					gProtect.m_IpAddressSelect[33] = 0;

					CHClientSelectServerSend(c2p_CHANGESERVER);

				}
				else
					CHClientSelectServerSend(c2p_NEWSERVER);

				/*if (!gProtect.m_SendSelectServer)
				{
					gProtect.m_SendSelectServer = true;
					CHClientSelectServerSend();
				}*/
			}

			return b_sub_52BA50(a1);
		}
		else
			return 0;
	}
	else
		return b_sub_52BA50(a1);
	//if (memcmp(gProtect.m_IpAddressSelect, pIpServer, strlen(gProtect.m_IpAddressSelect)))
	//if (!gProtect.m_SendSelectServer)
	//{
	//	//memcpy(gProtect.m_IpAddressSelect, pIpServer, sizeof(gProtect.m_IpAddressSelect));
	//	CHClientSelectServerSend();
	//	gProtect.m_SendSelectServer = true;
	//}

	/*HMODULE hModule = GetModuleHandle(NULL);

	int mapid;

	gProtect.m_BaseAddress = (DWORD)((void*)hModule);

	*(BYTE*)(mapid) = *(BYTE*)&gProtect.m_BaseAddress + 0xEFA5C0;

	ofstream myfile;
	myfile.open("E:\\example1.txt");
	myfile << gProtect.m_BaseAddress << "\n";
	myfile << *(BYTE*)gProtect.m_BaseAddress + 0xEFA5C0 << "\n";
	myfile << mapid << "\n";
	myfile.close();*/

	//unsinge int uint_2 = this.class36_0.method_3(9324788U);
	//this.uint_3 = this.uint_2 + 35632U;
	//this.uint_4 = this.uint_3 + 276U;
	
}

int __stdcall hook_sub_528CF0(int a1, const unsigned char* pIpAddress, unsigned short uPort, GUID* pGuid) {

	int v5;
	int v7;
	int* v8;
	char v9;
	char v10[16];

	b_CloseGameServer(a1);
	if (!pGuid || *(_DWORD*)(a1 + 4))
		return 0;
	v8 = (int*)(a1 + 8);

	if (a1 != -4 && 0x6DE3E8)
	{
		if (a1 == -8)
			v8 = (int*)(void*)0x6DE3E8;
		v7 = (*(int(__thiscall**)(int, int, int, int*))(*(_DWORD*)0x6DE3E8 + 12))(
			0x6DE3E8,
			0x80000,
			a1 + 4,
			v8);
	}
	else
	{
		v7 = 0;
	}

	char	Address[128];
	sprintf(Address, "%d.%d.%d.%d", pIpAddress[0], pIpAddress[1],
		pIpAddress[2], pIpAddress[3]);

	if ((*(int(__thiscall**)(_DWORD, const char* const, unsigned short))(**(_DWORD**)(a1 + 4) + 8))(*(_DWORD*)(a1 + 4), Address, uPort))
	{

		*(BYTE*)(a1 + 1037) = 1;
		tagLogicLogin2 ll;
		ll.cProtocol = 66;
		memcpy(&ll.guid, pGuid, sizeof(ll.guid));
		memcpy(&ll.HardwareId, gProtect.m_HardwareId, sizeof(ll.HardwareId));
		ll.HardwareId[33] = 0;

		if ((*(int(__thiscall**)(_DWORD, const void* const, const size_t))(**(_DWORD**)(a1 + 4) + 16))(*(_DWORD*)(a1 + 4), &ll, 50))
		{
			if (0x6DE314)
				b_sub_539C30(*(_DWORD*)(a1 + 4));
			return 1;
		}
		else
		{
			(*(void(__thiscall**)(_DWORD))(**(_DWORD**)(a1 + 4) + 4))(*(_DWORD*)(a1 + 4));
			(***(void(__thiscall****)(_DWORD))(a1 + 4))(*(_DWORD*)(a1 + 4));
			*(_DWORD*)(a1 + 4) = 0;
			return 0;
		}
	}
	else
	{
		(***(void(__thiscall****)(_DWORD))(a1 + 4))(*(_DWORD*)(a1 + 4));
		*(_DWORD*)(a1 + 4) = 0;
		return 0;
	}
}
//
//bool print_adapter(PIP_ADAPTER_ADDRESSES aa)
//{
//	char buf[BUFSIZ];
//	memset(buf, 0, BUFSIZ);
//	WideCharToMultiByte(CP_ACP, 0, aa->Description, wcslen(aa->Description), buf, BUFSIZ, NULL, NULL);
//
//	const IF_OPER_STATUS& Status = aa->OperStatus;
//	if (Status) {
//		//printf("adapter_name:%s\n", std::string(buf));
//	}
//	string des = "";
//	for (int i = 0; i < sizeof(buf); i++) {
//		des = des + buf[i];
//	}
//
//	if (des.find("TAP-") != std::string::npos || des.find("VPN") != std::string::npos) {
//		std::cout << "found!" << '\n';
//		return true;
//	}
//	return false;
//}
//
//bool isVPN()
//{
//
//	DWORD rv, size;
//	PIP_ADAPTER_ADDRESSES adapter_addresses, aa;
//	PIP_ADAPTER_UNICAST_ADDRESS ua;
//	bool isVPNOn = false;
//	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &size);
//	if (rv != ERROR_BUFFER_OVERFLOW) {
//
//	}
//	adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);
//
//	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &size);
//	if (rv != ERROR_SUCCESS) {
//		free(adapter_addresses);
//
//	}
//
//	for (aa = adapter_addresses; aa != NULL; aa = aa->Next) {
//		if (print_adapter(aa)) {
//			isVPNOn = true;
//		}
//	}
//
//	free(adapter_addresses);
//	if (isVPNOn) { // check VPN       
//		MessageBox(0, "Phat hien fake IP, disable VPN Adapter trong Control Panel->Network and Internet->Network Connections roi vao Game.", "Check Fake IP", MB_ICONINFORMATION);
//		exit(0);
//	}
//	return TRUE;
//}

int __stdcall hook_sub_52B950(int a1, const void* a2, int a3, int a4) {

	//try
	//{

	//	/*m_RepresentShell = CreateRepresentShell();
	//	KRUImage RUIconImage;
	//	RUIconImage.nType = ISI_T_SPR;
	//	RUIconImage.Color.Color_b.a = 255;
	//	RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	//	RUIconImage.uImage = 0;
	//	RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	//	RUIconImage.bRenderFlag = 0;
	//	strcpy(RUIconImage.szImage, PHYSICSSKILLICON);
	//	RUIconImage.oPosition.nX = 10;
	//	RUIconImage.oPosition.nY = 20;
	//	RUIconImage.oPosition.nZ = 0;
	//	RUIconImage.nFrame = 0;

	//	m_RepresentShell->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 1);*/

	//	if (!m_LoadPluginIconSkill)
	//	{
	//		HMODULE hModule = LoadLibraryA("CoreClient.dll");

	//		//void (*EntryProc)(char*) = ((void(*)(char*))GetProcAddress(hModule, "EntryProc"));
	//		void (*EntryProc)() = (void(*)())GetProcAddress(hModule, "EntryProc");

	//		if (EntryProc)
	//		{
	//			//HANDLE ThreadHandles1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryProc, 0, 0, NULL);
	//			//choose one
	//			m_LoadPluginIconSkill = true;
	//			EntryProc();
	//			//EntryProc("SS2");
	//		}
	//	}
	//	
	//}
	//catch (...)
	//{

	//}

	//gProtect.m_IsInGame = false;

	//gProtect.m_SendSelectServer = false;

	//CHClientSelectServerSend();
	try
	{


		//unsigned char mac[6] = { '\0' };
			//char cIpAddressID[8];
			//char cMacAddressID[8];
			//char textmix[100];
			//char MixMacAddress[50];
			//char local_IpAddress[16];
			//char local_MacAddress[18];
			//char local_HardwareId[32];
			//char local_SmallHardwareId[8];

			//memcpy(local_SmallHardwareId, SmallHardwareId(), sizeof(local_SmallHardwareId));

			//sprintf(local_IpAddress, "%s", GetLocalIpAddressNew().c_str());

			//get_mac(mac, local_IpAddress);

			////sprintf(gProtect.m_MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

			//DWORD IPId = *(DWORD*)(&local_IpAddress[0x00]) ^ *(DWORD*)(&local_IpAddress[0x16]) ^ 0x68BBBB79;

			//sprintf(MixMacAddress, "%08X%02X%02X%02X%02X%02X%s", IPId, mac[1], mac[2], mac[3], mac[4], mac[5], local_SmallHardwareId);

			//std::string newhwid = licenseKeyFormatting(MixMacAddress, 5, 4);

			//memcpy(local_HardwareId, newhwid.c_str(), sizeof(local_HardwareId));
			/*gPlayerSafe.m_WaitForRespon = false;

			CHClientSelectServerSend();

			int EndWait = 0;

			while (gPlayerSafe.m_WaitForRespon == false)
			{
				Sleep(500);
				EndWait++;
				if (EndWait == 10)
					break;
			}*/

		/*if (strlen(gPlayerSafe.HardwareSerialId) < 1)
		{
			memcpy(gPlayerSafe.HardwareSerialId, GetHardwareId(), sizeof(gPlayerSafe.HardwareSerialId));
		}*/
		//isVPN();

		INIReader reader("version.ini");

		if (reader.ParseError() != 0)
			return 0;

		if (!a1)
			return 0;
		if (!a2)
			return 0;

		//b_sub_529540(sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE, szBuff);
		//b_sub_53BF30_v8(sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE, szBuff);
		//RandMemSet(sizeof(szBuff), (BYTE*)szBuff);
		BYTE szBuff[sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE];

		KLoginAccountInfo* pProto;
		szBuff[0] = 65;
		pProto = (KLoginAccountInfo*)&szBuff[1];
		memset(&szBuff[1], 0, sizeof(KLoginAccountInfo) + 1);
		pProto->Param = a3;
		pProto->Size = 126;
		//pProto->Size = sizeof(KLoginAccountInfo);

		memcpy(c_AccountName, (char*)(void*)a1, 32);
		c_AccountName[31] = 0;
		strncpy(pProto->Account, (char*)(void*)a1, 32);
		pProto->Account[31] = 0;

		memcpy(pProto->Password.szPassword, (const void*)a2, sizeof(pProto->Password.szPassword));
		//pProto->Password.szPassword[63] = 0;

		//pProto->nLeftTime = pLAI->nLeftTime;

		pProto->nLeftTime = a4;
		
		//pProto->dwAllowLimit = gProtect.m_MainInfo.nLimit;

		//#if (USE_LIMIT_AUTH == 1)
		//			if (gProtocolVersion == 0)
		//				pProto->ProtocolVersion = gProtocolVersion;
		//			else
		//				pProto->ProtocolVersion = reader.GetInteger("FWCLD", "Version", 0);
		//#else
		//			pProto->ProtocolVersion = reader.GetInteger("FWCLD", "Version", 0);
		//			//SetByte(0x53E3E9, pProto->ProtocolVersion); //v8
		//#endif // (USE_LIMIT_AUTH == 1)

		pProto->ProtocolVersion = reader.GetInteger("GENERAL", "Version", 1);

		SetByte(0x52B9F9, pProto->ProtocolVersion);

		//pProto->ProtocolVersion = pProto->ProtocolVersion;

		//memcpy(c_ClientSerial, gProtect.m_MainInfo.ClientSerial, 32);
		//c_ClientSerial[32] = 0;

		memcpy(pProto->LicenseId, gProtect.m_MainInfo.ClientSerial, sizeof(pProto->LicenseId));

		//strncpy(pProto->LicenseId, "6f543b6329956d9a6fa10e43459b568e", sizeof(pProto->LicenseId));
		//strncpy(pProto->LicenseId, gProtect.m_MainInfo.LicenseId, sizeof(pProto->LicenseId) -1);

		pProto->LicenseId[sizeof(pProto->LicenseId) - 1] = 0;

		/*ofstream myfile;
		myfile.open("E:\\example.txt");
		myfile << GetHardwareIdNow() << "\n";
		myfile.close();*/

		//strncpy(pProto->HardwareId, gProtect.m_HardwareId, sizeof(pProto->HardwareId));
		strncpy(pProto->HardwareId, GetHardwareIdNow(), sizeof(pProto->HardwareId));
		pProto->HardwareId[sizeof(pProto->HardwareId)] = 0;

		//memcpy(c_HardwareId, pProto->HardwareId, 32);
		//c_HardwareId[31] = 0;
		//char	pHardwareSerialId[36];

		//sprintf(pHardwareSerialId, "%s", GetHardwareId());
		////sprintf(pHardwareSerialId, "%s", randomGet(35));

		//while (true)
		//{
		//	Sleep(100);
		//	if (strlen(pHardwareSerialId) > 30)
		//		break;
		//	else
		//		sprintf(pHardwareSerialId, "%s", GetHardwareId());
		//}

		//memcpy(pProto->HardwareSerialId, gPlayerSafe.HardwareSerialId, sizeof(pProto->HardwareSerialId));
		memcpy(pProto->HardwareSerialId, GetHardwareId(), sizeof(pProto->HardwareSerialId));
		pProto->HardwareSerialId[sizeof(pProto->HardwareSerialId)] = 0;

		memset(gProtect.m_MacAddress, 0, sizeof(gProtect.m_MacAddress));
		//SetInt((DWORD)((char*)&gProtect.m_MacAddress), gProtect.m_CheckLimit);
		//SetInt((DWORD)((char*)&gProtect.m_MacAddress[4]), gProtect.m_AllowLimit);
		//SetInt((DWORD)((char*)&gProtect.m_MacAddress[8]), gProtect.m_MainInfo.nLimit);
		sprintf((char*)&gProtect.m_MacAddress[0], "%d", gProtect.m_CheckLimit);
		sprintf((char*)&gProtect.m_MacAddress[2], "%d", gProtect.m_AllowLimit);
		sprintf((char*)&gProtect.m_MacAddress[4], "%d", gProtect.m_MainInfo.nLimit);

		memcpy(pProto->MacAddress, gProtect.m_MacAddress, sizeof(pProto->MacAddress));
		pProto->MacAddress[sizeof(pProto->MacAddress) - 1] = 0;

		strncpy(pProto->IpAddress, gProtect.m_WanIpAddress, sizeof(pProto->IpAddress));
		pProto->IpAddress[sizeof(pProto->IpAddress) - 1] = 0;

		/*ofstream myfile;
		myfile.open("E:\\example.txt");
		myfile << (char*)(void*)a1 << "\n";
		myfile << (char*)(void*)a2 << "\n";
		myfile << a3 << "\n";
		myfile << a4 << "\n";
		myfile.close();*/

#if (USE_CLIENTV8 == 1)
		if (!b_sub_53B900_v8((_DWORD*)(0x716AE8), (int)szBuff, sizeof(KLoginAccountInfo) + 1))
			return 0;
		b_sub_53BC60_v8((int)(DWORD*)(0x716AE8), (int)a2, 0, 60000);
#else
		if (!b_sub_528F40((_DWORD*)(0x6DE818), (int)szBuff, sizeof(KLoginAccountInfo) + 1))
			return 0;
		b_sub_529270((int)(_DWORD*)(0x6DE818), (int)a2, 0, 60000);
#endif


		//b_sub_53BC60_v8((int)(DWORD*)(0x716AE8), (int)a2, 0, 60000);
		return 1;

	}
	catch (...)
	{
		return 0;
	}
	
	//return b_sub_528F40((_DWORD*)0x6DE818, (int)szBuff, 230);
	//results = b_sub_528F40((_DWORD*)0x6DE818, (int)szBuff, 230);
	//ofstream myfile;
	//myfile.open("E:\\example.txt");
	//myfile << results << "\n";
	//myfile.close();
	//return results;
	//return b_sub_52A020(a2);
}

//int __stdcall HookChatCheck(void* This, int a2, int a3, int a4, char* Str, int a6)
//{
//	return FastChatCheck(Str, a6, (int)This, a2, a3, a4);
//}
//
//int __fastcall FastChatCheck(char* Str, int a2, int a3, int a4, int a5, int a6)
//{
//
//}

size_t __stdcall thamgia_tongkim(char* This)
{
	
	size_t result = std::strlen((const char*)0x69763C);
	if (result)
	{
		//if (memcmp((const char*)0x69763C, "Tr�n chi�n", 10) == 0)
	//if (memcmp((const char*)0x69763C, "Thi�n t�", 10) == 0)
	//const char* m_BattleName = ((const char*)0x69763C) - 1;
		//const char* m_BattleName = "Thi�n t�\0";
		//const char* m_BattleName = "Tr�n chi�n";
		//char m_BattleName2[8];
		//memset(m_BattleName2, 0, 8);
		//auto len = std::strlen((const char*)0x69763C);
		//auto len = std::strlen(m_BattleName);
		//memcpy(m_BattleName2, m_BattleName, 7);

		//ofstream myfile;
		//myfile.open("E:\\example1.txt");
		////myfile << (int)len << "\n";
		//myfile << m_BattleName << "\n";
		//myfile << m_BattleName2 << "\n";
		//myfile << *(int*)0x697624 << "\n";
		//myfile << *(int*)0x697628 << "\n";
		//myfile.close();

		//if ((int)len >= 9)
		if (result >= 9)
		{
			//SetByte(0x697624, nTong);
			//SetByte(0x697628, nKim);
			if (*(DWORD*)0x697624 > 0)
			{
				*(DWORD*)0x697624 = *(DWORD*)0x697624 + floor(*(DWORD*)0x697624 * 0.5);
			}

			if (*(DWORD*)0x697628 > 0)
			{
				*(DWORD*)0x697628 = *(DWORD*)0x697628 + floor(*(DWORD*)0x697628 * 0.5);
			}
		}

		return b_sub_4F2E40(This);
	}
	else
		return result;
	//ofstream myfile;
	//myfile.open("E:\\example.txt");
	//myfile << (int)len << "\n";
	//myfile << (const char*)0x69763C << "\n";
	//myfile.close();
}

int update_songuoi_tongkim(int a1, int a2)
{
	/*ofstream myfile;
	myfile.open("E:\\example.txt");
	myfile << a1 << "\n";
	myfile << a2 << "\n";
	myfile << memcmp((const char*)0x69763C, "Tr�n chi�n", 10) << "\n";
	myfile << memcmp((const char*)0x69763C, "Tr�n chi�n", 11) << "\n";
	myfile << (const char*)0x69763C << "\n";
	myfile << (const char*)0x6DF7E8 << "\n";
	myfile << (const char*)0x6966D8 << "\n";
	myfile.close();*/

	if (memcmp((const char*)0x69763C, "Tr�n chi�n", 10) == 0)
	{
		if (a1 >= 0 && a2 >= 0)
		{
			//int nBalance1 = floor(a1 * 0.5);
			//int nBalance2 = floor(a2 * 0.5);

			//int nTong = 0;

			//int nKim = 0;

			int nTong = a1 + floor(a1 * 0.5);

			int nKim = a2 + floor(a2 * 0.5);

			return b_sub_4F18E0(nTong, nKim);
		}
		else
			return a2;
	}
	else
		return b_sub_4F18E0(a1, a2);
}

int hook_sub_528F40(const void* pBuffer, int nSize)
{
	
	//HANDLE v1;
	//HANDLE v2;
	//v1 = GetCurrentProcess();
	//SetProcessWorkingSetSize(v1, 0xFFFFFFFF, 0xFFFFFFFF);
	//v2 = GetCurrentProcess();
	//SetThreadPriority(v2, -2);
	int results;
	char pAccName[32];
	char pAccPwd[64];

	KLoginAccountInfo* pLAI;

	pLAI = (KLoginAccountInfo*)((char*)pBuffer + 1);

	sprintf(pAccName, "%s", (char*)pBuffer + 7);
	sprintf(pAccPwd, "%s", (char*)pBuffer + 39);

	BYTE szBuff[sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE];
	_RandMemSet(sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE, (BYTE*)szBuff);
	KLoginAccountInfo* pProto;
	szBuff[0] = 65;
	pProto = (KLoginAccountInfo*)&szBuff[1];
	pProto->Size = sizeof(KLoginAccountInfo);
	pProto->Param = pLAI->Param;
	/*memset(&szBuff[1], 0, 230);*/

	strncpy(pProto->Account, pAccName, 32);
	pProto->Account[31] = 0;

	pProto->Password = *(KSG_PASSWORD_JX*)pAccPwd;
	//memcpy(pProto->Password.szPassword, pAccPwd, sizeof(pProto->Password.szPassword));
	//pProto->Password.szPassword[63] = 0;

	pProto->nLeftTime = pLAI->nLeftTime;
	pProto->ProtocolVersion = gProtocolVersion;
	pProto->_anon_0.nLeftTimeOfPoint = pLAI->_anon_0.nLeftTimeOfPoint;
	pProto->dwLimitTimeFlag = pLAI->dwLimitTimeFlag;
	pProto->dwLimitTimeOnlineSeconds = pLAI->dwLimitTimeOnlineSeconds;
	pProto->dwLimitTimeOfflineSeconds = pLAI->dwLimitTimeOfflineSeconds;
	memcpy(pProto->LicenseId, gProtect.m_MainInfo.ClientSerial, sizeof(pProto->LicenseId));
	//strncpy(pProto->LicenseId, "6f543b6329956d9a6fa10e43459b568e", sizeof(pProto->LicenseId) -1);
	//strncpy(pProto->LicenseId, gProtect.m_MainInfo.LicenseId, sizeof(pProto->LicenseId) -1);

	//pProto->LicenseId[sizeof(pProto->LicenseId) - 1] = 0;

	strncpy(pProto->HardwareId, gProtect.m_HardwareId, sizeof(pProto->HardwareId));
	//pProto->HardwareId[sizeof(pProto->HardwareId) - 1] = 0;

	//strncpy(pProto->MacAddress, gProtect.m_MacAddress, sizeof(pProto->MacAddress));
	//pProto->MacAddress[sizeof(pProto->MacAddress) - 1] = 0;

	//strncpy(pProto->IpAddress, gProtect.m_IpAddress, sizeof(pProto->IpAddress));
	//pProto->IpAddress[sizeof(pProto->IpAddress) - 1] = 0;
	/*ofstream myfile;
	myfile.open("E:\\example.txt");
	myfile << szBuff[0] << "\n";
	myfile << pProto->Size << "\n";
	myfile << pProto->Param << "\n";
	myfile << pProto->Account << "\n";
	myfile << pProto->Password.szPassword << "\n";
	myfile << pLAI->nLeftTime - 1 << "\n";
	myfile << pLAI->nLeftTime - 2 << "\n";
	myfile << pProto->ProtocolVersion << "\n";
	myfile << pProto->LicenseId << "\n";
	myfile << pProto->HardwareId << "\n";
	myfile << pProto->MacAddress << "\n";
	myfile << pProto->IpAddress << "\n";
	myfile << sizeof(KLoginAccountInfo) << "\n";
	myfile << sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE << "\n";
	myfile.close();*/
	//return b_sub_528F40((_DWORD*)0x6DE818, (int)szBuff, 230);
	//results = b_sub_528F40((_DWORD*)0x6DE818, (int)szBuff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE);
	//results = execute_sub_528F40((_DWORD*)(0x6DE818), (int)szBuff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE);
	results = execute_sub_528F40((_DWORD*)b_sub_529220((_DWORD*)(0x6DE818), 65, 0), (int)szBuff, sizeof(KLoginAccountInfo) + PROTOCOL_MSG_SIZE);
	
	return results;
	//if (results)
	//	return results;
	//else
	//	return 0;

}

__int16 __stdcall hook_sub_52CB70(WORD* This)
{
	__int16 v2;

	try
	{
		if (This[218] != NULL)
		{
			/*v2 = This[218];
			This[218] = v2 + 1;
			return v2;*/
			return b_sub_52CB70(This);
		}
		else
			return 0;
		//return b_sub_52CB70(This);
	}
	catch (...)
	{
		return 0;
	}
}

DWORD __stdcall hook_sub_55E7D(int a1, int a2)
{
	int result;
	result = b_sub_55E7D0(a1, a2);
	if (result)
	{
		/*if (nFreeMemoryCount > 0)
		{
			nFreeMemoryCount--;
		}
		else
			sub_10001300();*/
		sub_10001300();
	}
	return result;
}

DWORD __fastcall sub_10001300()
{
	DWORD result; // eax
	HANDLE CurrentProcess; // eax
	HANDLE v6; // eax
	PROCESS_MEMORY_COUNTERS_EX pmc;

	nFreeMemoryCount = 20;

	result = GetCurrentProcessId();

	if (result)
	{
		HANDLE hProc = OpenProcess(0x410u, 0, result);

		if (hProc)
		{
			if (GetProcessMemoryInfo(hProc, (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc)))
			{
				CurrentProcess = GetCurrentProcess();
				SetProcessWorkingSetSize(CurrentProcess, 0xFFFFFFFF, 0xFFFFFFFF);
				v6 = GetCurrentProcess();
				SetThreadPriority(v6, -2);
			}
			return CloseHandle(hProc);
		}
	}
	return result;
}

_DWORD* __stdcall sub_10003570(char* Src, size_t Size, _DWORD* This)
{
	_DWORD* v3; // ebx
	unsigned __int8 v4; // al
	size_t v5; // edi
	unsigned int v6; // kr00_4
	int result; // eax
	int v8; // [esp+4h] [ebp-408h]
	char v9[1024]; // [esp+8h] [ebp-404h] BYREF
	

	v3 = This;

	if (!(DWORD*)(0x6E2418))
		return b_sub_552970(This, Src, Size);
	if (!Src)
		return b_sub_552970(This, Src, Size);
	v4 = *(BYTE*)Src;
	if (*(BYTE*)Src <= 0x40u || v4 == 0xFF || !*(DWORD*)(This + 4 * v4) || v4 != 0xFB)
		return b_sub_552970(This, Src, Size);
	if (Src[*(int*)((char*)0x6E2418) - 1] == 8 || Src[Src[46] + 46] == 8)
	{
		Src[*(int*)(0x6E2418) - 1] = 0;
		v3 = This - 1;
		Src[Src[46]-- + 46] = 0;
	}
	if ((memset(v9, 0, sizeof(v9)),
		v5 = Src[46],
		memcpy(v9, Src + 47, v5),
		v9[v5] = 0,
		v6 = strlen(v9),
		result = v6,
		Src[46] > 1u)
		&& v6 != 1
		|| v9[0] != 27)
	{
		if (v9[v6 - 1] != 8)
			return b_sub_552970(This, Src, Size);
	}
	return (_DWORD*)result;
}

int __stdcall sub_557280(int a1, int a2)
{
	const char* szMoney = "l��ng";
	const char* szMoneyFlag = "%d l��ng";
	const char* szCoin = "Ti�n ��ng";
	const char* szCoinFlag = "%d Ti�n ��ng";

	if (*(BYTE*)(a1 + 3) > 7)
	{
		SetByte((DWORD)(void*)a1 + 3, 2);
		SetDword(0x6DF5F4, (DWORD)szCoinFlag);
		SetDword(0x6DFA74, (DWORD)szCoin);
		SetDword(0x6E0304, (DWORD)szCoin);
		SetDword(0x6E0174, (DWORD)szCoin);
	}
	else
	{
		SetDword(0x6DF5F4, (DWORD)szMoneyFlag);
		SetDword(0x6DFA74, (DWORD)szMoney);
		SetDword(0x6E0304, (DWORD)szMoney);
		SetDword(0x6E0174, (DWORD)szMoney);
	}

	return b_sub_557280(a1, a2);
}
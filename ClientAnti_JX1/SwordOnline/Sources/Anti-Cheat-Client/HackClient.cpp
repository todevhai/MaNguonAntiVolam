#include "stdafx.h"
//#include <KRepresentShell2.h>
#include "Anti-Cheat-Client.h"
#include "Connection.h"
#include "FileProtect.h"
#include "HackCheck.h"
#include "HackServerProtocol.h"
#include "ListManager.h"
#include "Log.h"
#include "Message.h"
#include "ProcessManager.h"
#include "Protect.h"
#include "SplashScreen.h"
#include "resource.h"
#include "Authenticate.h"
#include "Util.h"
//Modules
#include "DumpCheck.h"
//#include "ExecutableCheck.h"
#include "FileCheck.h"
//#include "FileMappingCheck.h"
//#include "LibraryCheck.h"
//#include "MacroCheck.h"
//#include "ProcessCheck.h"
//#include "RegistryCheck.h"
//#include "SimpleModules.h"
#include "ThreadCheck.h"
#include "WindowCheck.h"
#include "StatusDisplayer.h"
#include <TlHelp32.h>
#include <fstream>
#include "KPlayerSafe.h"
#include "KLuaScript.h"
#include <tchar.h>
#include "INIReader.h"
#include <IPTypes.h>
#include <IPHlpApi.h>
#include <Psapi.h>
#if (AUTH_TYPE==1)
#include "SkillState.h"
//#include <KIniFile.h>
#endif
#include "SimpleModules.h"
#include <signal.h>
#include <exception>
#include "WinlicenseSDK.h"
//#include "DetectVM.h"
#include <Shlobj.h>
//#include "iRepresent/D3D_Shell.h"
//#include "iRepresent/D3D_Device.h"


using namespace std;
//#include "KPlayerSafe.h"
//#include "KPlayerSafe.h"

#define b_sub_529270		(*(unsigned __int8(__thiscall**)(int, int, int, int))(*(_DWORD*)0x6DE30C + 4))(dword_6DE30C, a2, a3, v3);
#define b_KIniFile		((int(__thiscall*)(KIniFile* This)) 0x63A8F8)// 0x63A8F8 KIniFile_Entry (DWORD)GetModuleHandleA("Borderlands2.exe") + 0xA18940
#define b_KLoad		((int(__thiscall*)(KIniFile* This, const char *Data)) 0x63A8E4)//0x63A8E4 KIniFile_Load
#define b_GetInteger		((int(__thiscall*)(KIniFile* This, const char* a1, const char* a2, int a3, int * a4)) 0x63A8F4)//0x63A8F4 KIniFile_GetInteger
#define b_GetString		((int(__thiscall*)(KIniFile* This, const char* a1, const char* a2, const char* a3, const char* a4, int a5)) 0x63A8E0)//0x63A8E0 KIniFile_GetString
#define b_CleanIniFile		((void(__thiscall*)(KIniFile* This)) 0x678E88)//0x63A8DC 0x5E1A70 KIniFile_Clean
#define b_g_HashString2Id		((unsigned long(__cdecl*)(char const *)) 0x63A8EC)//0x63A8DC 
#define b_SendScriptData		((void(__cdecl*)(WORD wProtocol, const void* pData, size_t nLen)) 0x5CD7D0)//0x63A8DC 
#define b_SetMultiGame		((void(__thiscall*)(KWin32App *This, int a2)) 0x613AF4)//0x63A8DC 
#define Func_420820		((void(*)(signed int a1)) 0x420820)//0x63A8DC 

#define	SCHEME_INI_MINI		"SkillState.ini"

HINSTANCE hins;
HANDLE ThreadHandles[3];
HWND hWnd;
HANDLE ReconnectThread;
LARGE_INTEGER Frequency;
LARGE_INTEGER InitCounter;
LARGE_INTEGER NextCounter;
LARGE_INTEGER ElapsedMicroseconds;
DWORD CycleExecutionDelay = 500;
unsigned int MainAddressOffsetMap1 = NULL;
int					m_MainMapId = NULL;

std::vector<std::string> split(std::string s, std::string delimiter) {
	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
	std::string token;
	std::vector<std::string> res;

	while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
		token = s.substr(pos_start, pos_end - pos_start);
		pos_start = pos_end + delim_len;
		res.push_back(token);
	}

	res.push_back(s.substr(pos_start));
	return res;
};

bool print_adapter(PIP_ADAPTER_ADDRESSES aa)
{
	char buf[BUFSIZ];
	memset(buf, 0, BUFSIZ);
	WideCharToMultiByte(CP_ACP, 0, aa->Description, wcslen(aa->Description), buf, BUFSIZ, NULL, NULL);

	const IF_OPER_STATUS& Status = aa->OperStatus;
	if (Status) {
		//printf("adapter_name:%s\n", std::string(buf));
	}
	string des = "";
	for (int i = 0; i < sizeof(buf); i++) {
		des = des + buf[i];
	}

	if (des.find("TAP-") != std::string::npos || des.find("VPN") != std::string::npos) {
	//if (des.find("VPN") != std::string::npos) {
		std::cout << "found!" << '\n';
		return true;
	}
	return false;
}

bool isVPN()
{

	DWORD rv, size;
	PIP_ADAPTER_ADDRESSES adapter_addresses, aa;
	PIP_ADAPTER_UNICAST_ADDRESS ua;
	bool isVPNOn = false;
	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &size);
	if (rv != ERROR_BUFFER_OVERFLOW) {

	}
	adapter_addresses = (PIP_ADAPTER_ADDRESSES)malloc(size);

	rv = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapter_addresses, &size);
	if (rv != ERROR_SUCCESS) {
		free(adapter_addresses);

	}

	for (aa = adapter_addresses; aa != NULL; aa = aa->Next) {
		if (print_adapter(aa)) {
			isVPNOn = true;
		}
	}

	free(adapter_addresses);

	if (isVPNOn) {

		if (gAuthenticate.m_SplashScreen == 1)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(8), 3000);
			//SplashInit.CloseSplash();
		}
		else
		{
			MessageBox(0, "Phat hien fake IP, disable VPN Adapter trong Control Panel->Network and Internet->Network Connections roi vao Game.", "Check Fake IP", MB_ICONINFORMATION);

		}

		exit(0);
	}
	return TRUE;
}

DWORD getParentPID(DWORD pid, const char* szProcessNameme)
{
	HANDLE h = NULL;
	PROCESSENTRY32 pe = { 0 };
	DWORD ppid = 0;
	pe.dwSize = sizeof(PROCESSENTRY32);
	h = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (Process32First(h, &pe))
	{
		do
		{
			if (pe.th32ProcessID == pid)
			{
				//pe.szExeFile
				//strcpy((char*)szProcessNameme, pe.szExeFile);
				ppid = pe.th32ParentProcessID;
				break;
			}
		} while (Process32Next(h, &pe));
	}
	CloseHandle(h);
	return (ppid);
}

bool getProcessName(DWORD processID, LPSTR szProcessName, DWORD sz)
{
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS |
		PROCESS_VM_READ,
		FALSE, processID);

	if (NULL != hProcess)
	{
		HMODULE hMod;
		DWORD cbNeeded;

		if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
			&cbNeeded))
		{
			GetModuleBaseName(hProcess, hMod, szProcessName, sz);
			return true;
		}
	}
	return false;
}

BOOL CALLBACK EnumWindowsProcMy(HWND hwnd, LPARAM lParam)
{
	DWORD lpdwProcessId;
	GetWindowThreadProcessId(hwnd, &lpdwProcessId);
	if (lpdwProcessId == lParam)
	{
		hWnd = hwnd;
		return FALSE;
	}
	return TRUE;
}

bool IsProcessRunning(const TCHAR* const executableName) {
	PROCESSENTRY32 entry;
	entry.dwSize = sizeof(PROCESSENTRY32);

	const auto snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

	if (!Process32First(snapshot, &entry)) {
		CloseHandle(snapshot);
		return false;
	}

	do {
		if (!_tcsicmp(entry.szExeFile, executableName)) {
			CloseHandle(snapshot);
			return true;
		}
	} while (Process32Next(snapshot, &entry));

	CloseHandle(snapshot);
	return false;
}

DWORD_PTR GetProcessBaseAddress(DWORD processID)
{
	DWORD_PTR   baseAddress = 0;
	HANDLE      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processID);
	HMODULE* moduleArray;
	LPBYTE      moduleArrayBytes;
	DWORD       bytesRequired;

	if (processHandle)
	{
		if (EnumProcessModules(processHandle, NULL, 0, &bytesRequired))
		{
			if (bytesRequired)
			{
				moduleArrayBytes = (LPBYTE)LocalAlloc(LPTR, bytesRequired);

				if (moduleArrayBytes)
				{
					unsigned int moduleCount;

					moduleCount = bytesRequired / sizeof(HMODULE);
					moduleArray = (HMODULE*)moduleArrayBytes;

					if (EnumProcessModules(processHandle, moduleArray, bytesRequired, &bytesRequired))
					{
						baseAddress = (DWORD_PTR)moduleArray[0];
					}

					LocalFree(moduleArrayBytes);
				}
			}
		}

		CloseHandle(processHandle);
	}

	return baseAddress;
}

DWORD GetModuleBase(DWORD processId, TCHAR* szModuleName)
{
	DWORD moduleBase = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
	if (hSnapshot != INVALID_HANDLE_VALUE) {
		MODULEENTRY32 moduleEntry;
		moduleEntry.dwSize = sizeof(MODULEENTRY32);
		if (Module32First(hSnapshot, &moduleEntry)) {
			do {
				if (wcscmp((const wchar_t*)moduleEntry.szModule, (const wchar_t*)szModuleName) == 0) {
					moduleBase = (DWORD)moduleEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnapshot, &moduleEntry));
		}
		CloseHandle(hSnapshot);
	}
	return moduleBase;
}

uintptr_t GetModuleBaseAddress(DWORD procId, const char* modName)
{
	uintptr_t modBaseAddr = 0;
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, procId);
	if (hSnap != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 modEntry;
		modEntry.dwSize = sizeof(modEntry);
		if (Module32First(hSnap, &modEntry))
		{
			do
			{
				if (!memcmp(modEntry.szModule, modName, strlen(modName)))
				{
					modBaseAddr = (uintptr_t)modEntry.modBaseAddr;
					break;
				}
			} while (Module32Next(hSnap, &modEntry));
		}
	}
	CloseHandle(hSnap);
	return modBaseAddr;
}

bool CheckProcess(DWORD processID, DWORD flag)
{
	//TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	char szProcessName[32];

	memset(szProcessName, 0, 32);

	if (flag == 1)
	{
		GetModuleBaseName(GetCurrentProcess(), GetModuleHandle(NULL), szProcessName,
			sizeof(szProcessName) / sizeof(TCHAR));
		if (strcmp(gProtect.m_MainInfo.ClientName, szProcessName) != 0)
		{
			return false;
		}
		else
		{
			//memcpy(gProtect.m_MainInfo.ClientName, szProcessName, sizeof(gProtect.m_MainInfo.ClientName));
			return true;
		}
	}
	else if (flag == 2)
	{
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS |
			PROCESS_VM_READ,
			FALSE, processID);

		if (NULL != hProcess)
		{
			HMODULE hMod;
			DWORD cbNeeded;

			if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
				&cbNeeded))
			{
				GetModuleBaseName(hProcess, hMod, szProcessName,
					sizeof(szProcessName) / sizeof(TCHAR));
			}
		}

		if (strcmp(gProtect.m_MainInfo.ClientName, szProcessName) == 0)
		{
			return true;
		}
		else
			return false;
	}
	// Print the process name and identifier.

	//_tprintf(TEXT("%s  (PID: %u)\n"), szProcessName, processID);

	// Release the handle to the process.

	CloseHandle(hProcess);
}

DWORD WINAPI ConnectionReconnectThread() // OK
{
	int ProtocolServer;

	//while (!DelayMe(5000, 500))
	while (true)
	{

		Sleep(500);
		/*	std::ofstream myfile;
			myfile.open("E:\\example.txt");
			myfile << gProtect.m_IsInGame << "\n";
			myfile.close();*/
		if (gPlayerSafe.m_IsGameSetOffline)
			break;

		if (gReconnectStatus == 1)
		{

			if (gConnection.Init(HackServerProtocolCore) == 0)
			{
				continue;
			}

			if (gConnection.Connect(gProtect.m_MainInfo.AuthIpAddress, gProtect.m_MainInfo.ServerPort) == 0)
			{
				//gConnection.Disconnect();
				gConnectionStatusTime = GetTickCount();

				CHClientInfoSend();

			}

			if (gProtect.m_IsConnectedAuth)
				CHClientSelectServerSend(c2p_RECONECCT);

			//if (gProtect.m_IsInGame == true)
			//	CHClientSelectServerSend();
			//else
			//{
			//	/*if(!gProtect.m_SendSelectServer)
			//		CHClientInfoSend();
			//	else
			//		CHConnectionStatusSend();*/
			//	CHConnectionStatusSend();
			//}
			//CHClientSelectServerSend();
			//CHClientInfoSend();

			gReconnectStatus = 2;

			gConnectionStatusTime = GetTickCount();

			//Sleep(2000);

		}
		//else
			//Sleep(2000);

	}

	//CHRemoveSelectServerSend();

	gPlayerSafe.m_IsExitGame = true;

	return 0;
}

DWORD WINAPI ConnectionStatusThread() // OK
{
	try
	{

		//gReconnectStatus = 0;

		//gReconnectStatus = 2;

		DWORD ClientInfoTimeOut = GetTickCount();

		//gReconnectStatus = 0;

		//while (!DelayMe(5000, 500))
#if AUTH_TYPE == 0
		while (true)
		{
			Sleep(500);

			if (gPlayerSafe.m_IsGameSetOffline)
				break;

			if (gProtect.m_CheckLimit == 1)
			{
				if ((GetTickCount() - ClientInfoTimeOut) > 5000)
				{
					//gConnection.Disconnect();
					if (gConnection.CheckState() == 0)
					{
						if (gAuthenticate.m_SplashScreen == 1)
							SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(1), 2000);
						else
						{
							MessageBox(0, gMessage.GetMessage(1), "Game", MB_ICONINFORMATION);
							SafeExitProcess();
						}
						return 0;
					}
				}
			}


			//if(gConnection.CheckState() == 0 && gConnection.Init(HackServerProtocolCore) != 0)
			//{
			//	if(gConnection.Connect(gProtect.m_MainInfo.AuthIpAddress, gProtect.m_MainInfo.ServerPort) != 0)
			//	{
			//		CHClientInfoSend();
			//		continue;;
			//	}
			//	//else
			//	//{
			//	//	gConnection.Disconnect();
			//	//	continue;
			//	//	//if (gAuthenticate.m_SplashScreen == 0)
			//	//		//SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(1), 5000);
			//	//	//MessageBox(0, "Connect failed", "Info", MB_OK | MB_ICONINFORMATION);
			//	//	//ExitProcess(0);
			//	//}
			//}

			if (gConnection.CheckState() == 0 && gConnection.Init(HackServerProtocolCore) != 0)
			{
				if (gConnection.Connect(gProtect.m_MainInfo.AuthIpAddress, gProtect.m_MainInfo.ServerPort) != 0)
				{
					gConnectionStatusTime = GetTickCount();
					CHClientInfoSend();
					//continue;
					//break;
				}
			}

			if (gProtect.m_IsConnectedAuth)
				break;

			/*if (gClientInfoOK != 0 && gDumpListOK != 0 && gChecksumListOK != 0 && gInternalListOK != 0 && gWindowListOK != 0)
			{
				DWORD CurProgress = gListManager.gDumpListInfo.size() + gListManager.gChecksumListInfo.size() + gListManager.gInternalListInfo.size() + gListManager.gWindowListInfo.size();

				DWORD MaxProgress = gDumpListMaxCount + gChecksumListMaxCount + gInternalListMaxCount + gWindowListMaxCount;

				if (CurProgress == MaxProgress)
				{
					break;
				}
			}*/
		}
#endif
		ReadProcessMemory(hProcess, (void*)(unsigned int)MainAddressOffsetMap1, &m_MainMapId, sizeof(m_MainMapId), NULL);

		if (m_MainMapId != 380 && m_MainMapId != 900)
		{
			if (gProtect.m_CheckLimit == 1)
			{

				DWORD aProcesses[1024], cbNeeded, cProcesses;
				unsigned int i;

				if (!EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
				{
					/*if (gAuthenticate.m_SplashScreen == 1)
						SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(5), 3000);
					else
					{
						MessageBox(0, gMessage.GetMessage(5), "Game", MB_ICONINFORMATION);
						exit(0);
					}*/
					SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(5), 3000);
					return 0;
				}

				cProcesses = cbNeeded / sizeof(DWORD);

				for (i = 0; i < cProcesses; i++)
				{
					Sleep(50);
					if (aProcesses[i] != 0)
					{
						if (CheckProcess(aProcesses[i], 2))
						{
#ifdef USE_LIMIT_LOCAL == 1
							if (gAuthenticate.m_ProcessCountTemp >= gProtect.m_MainInfo.nLimit)
							{
								char buff[100];
								wsprintf(buff, gMessage.GetMessage(4), gProtect.m_MainInfo.nLimit);
								SplashScreen(&SplashError, 2, 1, buff, 3000);
								/*if (gAuthenticate.m_SplashScreen == 1)
									SplashScreen(&SplashError, 2, 1, buff, 3000);
								else
								{
									MessageBox(3000, buff, "Game", MB_ICONINFORMATION);
									exit(0);
								}*/
								return 0;
							}
#endif // USE_LIMIT_LOCAL == 1
							gAuthenticate.m_ProcessCountTemp++;
						}
					}
				}
			}
		}

#if AUTH_TYPE == 0
		if (gProtect.m_CheckCount == 1)
		{
			gProtect.ThreadHandles[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConnectionReconnectThread, 0, 0, (DWORD*)&gThreadCheck.m_CheckThreadID[3]);

			while (true)
			{
				Sleep(500);
				//std::ofstream myfile;
				//myfile.open("E:\\example.txt");
				//myfile << (_DWORD*)(0x6E1DA4) << "\n";
				//myfile.close();
				/*if (gPlayerSafe.m_IsGameSetOffline)
					break;*/

				if (gPlayerSafe.m_IsGameSetOffline)
					break;

				if (gConnection.CheckState() == 0)
				{
					//if(gReconnectSwitch == 0)
					//{
					//	//SplashScreen(&SplashError,2,1,gMessage.GetMessage(2),5000);
					//	continue;
					//}

					if (gReconnectStatus == 1)
					{
						gConnectionStatusTime = GetTickCount();
						continue;
					}

					if (gReconnectStatus == 0 || gReconnectStatus == 2)
					{
						//gConnection.Disconnect();
						gReconnectStatus = 1;

						gProtect.m_IsConnectedAuth = false;

						continue;
					}

					//Sleep(2000);
				}
				else
				{

					if ((GetTickCount() - gConnectionStatusTime) > 5000)
					{
						//SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(1), 5000);
						//SafeExitProcess();
						//gConnection.Disconnect();
						//Sleep(500);
						gReconnectStatus = 1;

						gProtect.m_IsConnectedAuth = false;

						continue;
					}
					else
					{
						Sleep(500);
						CHConnectionStatusSend();
						continue;
					}

					//Sleep(2000);
					//if((GetTickCount()-gConnectionStatusTime) > 5000)
					//{
					//	gConnection.Disconnect();
					//	//SplashScreen(&SplashError,2,1,gMessage.GetMessage(3),5000);
					//	//continue;
					//}
					//else
					//{
					//	
					//	//continue;
					//}
				}
			}
		}
		else
		{
			gConnection.~CConnection();
		}
#endif

		//Sleep(1000);

		//SplashInit.CloseSplash();

	//#if (USE_LIMIT_AUTH == 1)
		//while (true)
		//while(!DelayMe(5000,500))


	//#endif

		//CHRemoveSelectServerSend();

		gPlayerSafe.m_IsExitGame = true;

		return 0;
	}
	catch (...)
	{
		return 0;
	}
}

DWORD WINAPI MainThread() // OK
{
	try
	{
	
		DWORD gDetectTime = 0;

		DWORD CycleCount = 0;

		DWORD CycleExecutionDelay = 500;

		LARGE_INTEGER Frequency;

		LARGE_INTEGER InitCounter;

		LARGE_INTEGER NextCounter;

		LARGE_INTEGER ElapsedMicroseconds;

		QueryPerformanceFrequency(&Frequency);

		QueryPerformanceCounter(&InitCounter);

		gDetectTime = GetTickCount();

		HMODULE hModule = GetModuleHandle(NULL);

		unsigned int m_MainBaseAddress = (DWORD)((void*)hModule);

		MainAddressOffsetMap1 = (m_MainBaseAddress + 0xEFA5C0);

		//while (!DelayMe(((CycleExecutionDelay > 500) ? 0 : (500 - CycleExecutionDelay)), 300))
		while (!DelayMe(((CycleExecutionDelay > 500) ? 0 : (500 - CycleExecutionDelay)), 150))
		{
			//Sleep(300);

			/*if (gPlayerSafe.m_IsGameSetOffline)
			{
				Sleep(200);
				continue;
			}*/
			ReadProcessMemory(hProcess, (void*)(unsigned int)MainAddressOffsetMap1, &m_MainMapId, sizeof(m_MainMapId), NULL);

			QueryPerformanceCounter(&NextCounter);

			ElapsedMicroseconds.QuadPart = ((NextCounter.QuadPart - InitCounter.QuadPart) * 1000000) / Frequency.QuadPart;

			if ((ElapsedMicroseconds.QuadPart / 1000) > 10000)
			{
				MessageBox(0, "He thong tro choi khong dong bo", "Error", MB_OK | MB_ICONINFORMATION);
				//SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(1), 5000);
				SafeExitProcess();
				return 0;
				//Sleep(500);
				//continue;
			}

			QueryPerformanceFrequency(&Frequency);

			QueryPerformanceCounter(&InitCounter);

			if(gDetectTime != 0)
			{
				if((GetTickCount()- gDetectTime) > 10000)
				{
					MessageBox(0, "He thong tro choi khong dong bo", "Error", MB_OK | MB_ICONINFORMATION);
					SafeExitProcess();
					return 0;
				}
			}

			/*
			if(gIpAddressAddress != 0)
			{
				MemoryCpy(gIpAddressAddress,gIpAddress,sizeof(gIpAddress));
			}

			if(gClientVersionAddress != 0)
			{
				MemoryCpy(gClientVersionAddress,gClientVersion,sizeof(gClientVersion));
			}

			if(gClientSerialAddress != 0)
			{
				MemoryCpy(gClientSerialAddress,gClientSerial,sizeof(gClientSerial));
			}*/

		
			//char textoutput[100];
			//sprintf_s(textoutput, "%s", CycleCount++);
			//sprintf(textoutput, "%s", CycleCount);
			//MessageBox(0, textoutput, "Info", MB_OK | MB_ICONINFORMATION);
			//gLog.Output(LOG_DEBUG, textoutput);
		
			/*ofstream myfile;
			myfile.open("E:\\CycleCount.txt");
			myfile << CycleCount << "\n";
			myfile << ((CycleCount++) % 10) << "\n";
			myfile.close();*/

			if (m_MainMapId != 380 && m_MainMapId != 900)
			{
				switch (((CycleCount++) % 30))
				{
				case 0:
					//API_SCAN();
					//MEMORY_PROTECTION_SCAN();
					//gWindowCheck.Scan();
					//gFileCheck.Scan();
					gProcessManager.CheckProcess(15);

					break;
				case 1:
					gWindowCheck.Scan();
					//gFileCheck.Scan();
					//gProcessManager.CheckProcess(15);
					//CheckDetourIntegrity();
					//MEMORY_PROTECTION_SCAN();
				//	gRegistryCheck.Scan();

					if (gProtect.m_CheckVPN == 1)
					{
						isVPN();
					}

					Sleep(100);
					break;
				case 2:
					//API_SCAN();
					//gWindowCheck.Scan();
					gProcessManager.CheckProcess(15);

					//gFileMappingCheck.Scan();
					break;
				case 3:
					//gFileCheck.Scan();
					//gProcessManager.CheckProcess(15);
					//	//DEBUGGER_SCAN();
					//MEMORY_PROTECTION_SCAN();
					 //HANDLE_PROTECTION_SCAN();
					//	gFileCheck.Scan();
					gWindowCheck.Scan();
					Sleep(100);
					break;
				case 4:
					//API_SCAN();
					//gWindowCheck.Scan();
					gProcessManager.CheckProcess(15);
					break;
				case 5:
					//gWindowCheck.Scan();
					//CheckDetourIntegrity();
					//MEMORY_PROTECTION_SCAN();
					//HANDLE_PROTECTION_SCAN();
					gWindowCheck.Scan();
					Sleep(100);
					//gProcessManager.CheckProcess(15);
					break;
				case 6:
					//gFileCheck.Scan();
					//API_SCAN();
					//gWindowCheck.Scan();
					gProcessManager.CheckProcess(15);
					break;
				case 7:
					//gWindowCheck.Scan();
					//gProcessManager.CheckProcess(15);
				//	DEBUGGER_SCAN();
					//MEMORY_PROTECTION_SCAN();
					//HANDLE_PROTECTION_SCAN();
				//	gFileMappingCheck.Scan();
					gWindowCheck.Scan();
					Sleep(100);
					break;
				case 8:
					//gFileCheck.Scan();
					//API_SCAN();
					//gWindowCheck.Scan();
					gProcessManager.CheckProcess(15);
					//Sleep(100);
					break;
				case 9:
					//gFileCheck.Scan();
				//	CheckDetourIntegrity();
					//MEMORY_PROTECTION_SCAN();
					gProcessManager.ClearProcessCache();
					Sleep(100);
					break;
				default:
					break;
				}
			}
			else
			{
				Sleep(1000);
			}
			
			gDetectTime = GetTickCount();

			QueryPerformanceCounter(&NextCounter);

			ElapsedMicroseconds.QuadPart = ((NextCounter.QuadPart - InitCounter.QuadPart) * 1000000) / Frequency.QuadPart;

			CycleExecutionDelay = (DWORD)(ElapsedMicroseconds.QuadPart / 1000);

			QueryPerformanceFrequency(&Frequency);

			QueryPerformanceCounter(&InitCounter);
		}

		return 0;
	}
	catch (...)
	{
		return 0;
	}

}

void CheckClassGame()
{

	//MessageBox(0, "CheckClassGame", "Info", MB_OK | MB_ICONINFORMATION);
	//unsigned char ckey[] = "thiskeyisverybad";
	//unsigned char ivec[] = "dontusethisinput";
	//FILE* fIN, * fOUT;
	//// First encrypt the file

	//fIN = fopen("./decrypted.txt", "rb"); //File to be encrypted; plain text
	//fOUT = fopen("./cyphertext.txt", "wb"); //File to be written; cipher text
	//if (fIN == NULL)
	//{
	//	MessageBox(0, "Cannot open file fIN", "Info", MB_OK | MB_ICONINFORMATION);
	//	return;
	//}
	//if (fOUT == NULL)
	//{
	//	MessageBox(0, "Cannot open file fOUT", "Info", MB_OK | MB_ICONINFORMATION);
	//	return;
	//}

	//DecryptEncryptFileProtect(TRUE, fIN, fOUT, ckey, ivec);

	//fclose(fIN);
	//fclose(fOUT);

	//Decrypt file now

	//fIN = fopen("cyphertext.txt", "rb"); //File to be read; cipher text
	//fOUT = fopen("decrypted.txt", "wb"); //File to be written; cipher text

	//DecryptEncryptFileProtect(FALSE, fIN, fOUT, ckey, ivec);

	//fclose(fIN);
	//fclose(fOUT);

	//FILE* fIN, * fOUT;
	//fIN = fopen("decrypted.txt", "r+b");//File to be encrypted; plain text
	//fOUT = fopen("cyphertext.txt", "w+b");//File to be written; cipher text

	//EncryptFileProtect(fIN, fOUT);
	//fclose(fIN);
	//fclose(fOUT);
	//Decrypt file now
	//fIN = fopen("cyphertext.txt", "rb");//File to be written; cipher text
	//fOUT = fopen("decrypted.txt", "wb");//File to be written; cipher text
	//DecryptFileProtect(fIN, fOUT);
	//fclose(fIN);
	//fclose(fOUT);

	//mahoafile("./decrypted.txt", "./cyphertext.txt");
	//giaimafile("./cyphertext.txt", "./cyphertext1.txt");
	Sleep(1000);

	SplashInit.CloseSplash();

	//gConnection.Disconnect();
	CloseHandle(GetCurrentThread());

}

//void __cdecl SendScriptDataToServe(WORD wProtocol, const void* pData, size_t nLen)
//{
//	ofstream myfile;
//	myfile.open("E:\\SendScriptDataToServe.txt");
//	myfile << wProtocol << "\n";
//	myfile << pData << "\n";
//	myfile << &pData + 1 << "\n";
//	myfile << nLen << "\n";
//	myfile.close();
//	b_SendScriptData(wProtocol, pData, nLen);
//}

//int GameInit()
//{
//	int result; // eax
//	int v1; // esi
//
//	result = *((DWORD*)0x526CB0);
//	v1 = result;
//	if (result)
//	{
//		SetWorkingDir();
//		return v1;
//	}
//	return result;
//}

//void __fastcall SetWorkingDir()
//{
//	DWORD Process; // eax
//	HANDLE hProcess; // eax
//	void* v3; // edi
//	unsigned int v4; // eax
//	HANDLE CurrentProcess; // eax
//	HANDLE v6; // eax
//	PPROCESS_MEMORY_COUNTERS v7;
//	unsigned int v8;
//
//	Process = GetCurrentProcessId();
//	if (Process)
//	{
//		hProcess = OpenProcess(0x410u, 0, Process);
//		v3 = (void*)hProcess;
//		if (hProcess)
//		{
//			if (GetProcessMemoryInfo(hProcess, v7, 40))
//			{
//				CurrentProcess = GetCurrentProcess();
//				SetProcessWorkingSetSize(CurrentProcess, 0xFFFFFFFF, 0xFFFFFFFF);
//				v6 = GetCurrentProcess();
//				SetThreadPriority(v6, -2);
//			}
//			CloseHandle(v3);
//		}
//	}
//}

void __fastcall CheckFuck(signed int a1)
{
	//DWORD gDetectTime = 0;

	//gDetectTime = GetTickCount();

	if (*((BYTE*)0x68D298) == 2)
	{
		QueryPerformanceCounter(&NextCounter);

		ElapsedMicroseconds.QuadPart = ((NextCounter.QuadPart - InitCounter.QuadPart) * 1000000) / Frequency.QuadPart;

		if ((ElapsedMicroseconds.QuadPart / 1000) > 3000)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
			SafeExitProcess();
		}

		QueryPerformanceFrequency(&Frequency);

		QueryPerformanceCounter(&InitCounter);

		/*if (gDetectTime != 0)
		{
			if ((GetTickCount() - gDetectTime) > 5000)
			{
				SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
				SafeExitProcess();
			}
		}*/

		//gDetectTime = GetTickCount();

		DWORD flOldProtect;
		DWORD v6;

		VirtualProtect((LPVOID)0x44E4BB, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44E4BB) != (DWORD*)0xFFEF30E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x44E4BB, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x406804, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x406804) != (DWORD*)0x046E37E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x406804, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x40E78C, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x40E78C) != (DWORD*)0x03EEAFE8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x40E78C, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x44D089, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44D089) != (DWORD*)0x0005B2E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x44D089, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x4A4649, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x4A4649) != (DWORD*)0xFA8FF2E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x4A4649, 5u, flOldProtect, &v6);

		/*VirtualProtect((LPVOID)0x5281F8, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x5281F8) != (DWORD*)0x04C4837B)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x5281F8, 5u, flOldProtect, &v6);*/

		VirtualProtect((LPVOID)0x44E4BF, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44E4BF) != (DWORD*)0xFC4D8BFF)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x44E4BF, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x404162, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x404162) != (DWORD*)0xFFEC79E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x404162, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x405607, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x405607) != (DWORD*)0xFFD7D4E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x405607, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x468820, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x468820) != (DWORD*)0xF9A5BBE8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x468820, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x49B5F2, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x49B5F2) != (DWORD*)0xF677E9E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x49B5F2, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x4ED789, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x4ED789) != (DWORD*)0xF15652E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x4ED789, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x530188, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x530188) != (DWORD*)0xFFC9E3E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x530188, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x5300D3, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x5300D3) != (DWORD*)0xFFCA28E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x5300D3, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x508BE8, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x508BE8) != (DWORD*)0x000663E8)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x508BE8, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x44CBD3, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44CBD3) != (DWORD*)0x8D8B046A)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x44CBD3, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x44CD44, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44CD44) != (DWORD*)0x60A13A75)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
		}
		VirtualProtect((LPVOID)0x44CD44, 5u, flOldProtect, &v6);

		QueryPerformanceCounter(&NextCounter);

		ElapsedMicroseconds.QuadPart = ((NextCounter.QuadPart - InitCounter.QuadPart) * 1000000) / Frequency.QuadPart;

		QueryPerformanceFrequency(&Frequency);

		QueryPerformanceCounter(&InitCounter);

	}
	Func_420820(a1);
}

DWORD WINAPI CheckThread() // OK
{
	DWORD gDetectTime = 0;

	DWORD CycleCount = 0;

	

	LARGE_INTEGER Frequency;

	LARGE_INTEGER InitCounter;

	LARGE_INTEGER NextCounter;

	LARGE_INTEGER ElapsedMicroseconds;

	QueryPerformanceFrequency(&Frequency);

	QueryPerformanceCounter(&InitCounter);

	gDetectTime = GetTickCount();
	//while (!DelayMe(((CycleExecutionDelay > 500) ? 0 : (500 - CycleExecutionDelay)), 1))
	while (!DelayMe(((CycleExecutionDelay > 500) ? 0 : (500 - CycleExecutionDelay)), 300))
	{
		//Sleep(300);

		/*if (gPlayerSafe.m_IsGameSetOffline)
		{
			Sleep(200);
			continue;
		}*/

		QueryPerformanceCounter(&NextCounter);

		ElapsedMicroseconds.QuadPart = ((NextCounter.QuadPart - InitCounter.QuadPart) * 1000000) / Frequency.QuadPart;

		if ((ElapsedMicroseconds.QuadPart / 1000) > 3000)
		{
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
			//SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(1), 5000);
			SafeExitProcess();
			return 0;
			//Sleep(500);
			//continue;
		}

		QueryPerformanceFrequency(&Frequency);

		QueryPerformanceCounter(&InitCounter);

		if(gDetectTime != 0)
		{
			if((GetTickCount()- gDetectTime) > 5000)
			{
				SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);
				SafeExitProcess();
				return 0;
			}
		}

		DWORD flOldProtect;
		DWORD v6;

		VirtualProtect((LPVOID)0x44E4BB, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44E4BB) != (DWORD*)0xFFEF30E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x44E4BB, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x406804, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x406804) != (DWORD*)0x046E37E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x406804, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x40E78C, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x40E78C) != (DWORD*)0x03EEAFE8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x40E78C, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x44D089, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44D089) != (DWORD*)0x0005B2E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x44D089, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x4A4649, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x4A4649) != (DWORD*)0xFA8FF2E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x4A4649, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x44E4BF, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44E4BF) != (DWORD*)0xFC4D8BFF)
		{
			break;
		}
		VirtualProtect((LPVOID)0x44E4BF, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x404162, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x404162) != (DWORD*)0xFFEC79E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x404162, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x405607, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x405607) != (DWORD*)0xFFD7D4E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x405607, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x468820, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x468820) != (DWORD*)0xF9A5BBE8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x468820, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x49B5F2, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x49B5F2) != (DWORD*)0xF677E9E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x49B5F2, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x4ED789, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x4ED789) != (DWORD*)0xF15652E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x4ED789, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x5281F4, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x5281F4) != (DWORD*)0xEF8627E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x5281F4, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x530188, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x530188) != (DWORD*)0xFFC9E3E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x530188, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x5300D3, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x5300D3) != (DWORD*)0xFFCA28E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x5300D3, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x508BE8, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x508BE8) != (DWORD*)0x000663E8)
		{
			break;
		}
		VirtualProtect((LPVOID)0x508BE8, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x44CBD3, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44CBD3) != (DWORD*)0x8D8B046A)
		{
			break;
		}
		VirtualProtect((LPVOID)0x44CBD3, 5u, flOldProtect, &v6);

		VirtualProtect((LPVOID)0x44CD44, 5u, 0x40u, &flOldProtect);
		if ((DWORD*)*((DWORD*)0x44CD44) != (DWORD*)0x60A13A75)
		{
			break;
		}
		VirtualProtect((LPVOID)0x44CD44, 5u, flOldProtect, &v6);

		gDetectTime = GetTickCount();

		QueryPerformanceCounter(&NextCounter);

		ElapsedMicroseconds.QuadPart = ((NextCounter.QuadPart - InitCounter.QuadPart) * 1000000) / Frequency.QuadPart;

		CycleExecutionDelay = (DWORD)(ElapsedMicroseconds.QuadPart / 1000);

		QueryPerformanceFrequency(&Frequency);

		QueryPerformanceCounter(&InitCounter);

	}

	SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(6), 5000);

	SafeExitProcess();

	return 0;
}

extern "C" _declspec(dllexport) void EntryProc() // OK
{
	DWORD pid = GetCurrentProcessId();

	gProtect.m_Check = 0;
	gProtect.m_Memory = 50;
	
	//DWORD dllBase = GetModuleBase(pId, "hw.dll");
	//SplashScreen(&SplashInit, 0, 1, gMessage.GetMessage(7), 2000);

	if (!IsUserAnAdmin())
	{
		MessageBox(0, "Vui long chay game voi quyen Administrator. Please Run as Administrator !", "Game", MB_ICONINFORMATION);
		SafeExitProcess();
		return;
	}
	
	if (SetAdminPrivilege(SE_AUDIT_NAME) == 0)
	{
		MessageBox(0, "Vui long chay game voi quyen Administrator. Please Run as Administrator !", "Game", MB_ICONINFORMATION);
		SafeExitProcess();
		return;
	}

	DEVMODE DevMode;

	EnumDisplaySettings(NULL, 0xFFFFFFFF, &DevMode);

	//DevMode.dmFields |= 262144u;
	//DevMode.dmBitsPerPel = 16;
	//ChangeDisplaySettingsA(&DevMode, 0);

	if (DevMode.dmBitsPerPel != 16)
	{
		DevMode.dmFields |= 262144u;
		DevMode.dmBitsPerPel = 16;
		ChangeDisplaySettingsA(&DevMode, 0);
	}

	//g_D3DShell.m_pD3D->QueryInterface();

	//g_Device.g_hWnd = gGetMainHWnd();

	//D3DAdapterInfo* pAdapterInfo = NULL;
	//D3DDeviceInfo* pDeviceInfo = NULL;
	//D3DModeInfo* pModeInfo = NULL;

	////g_D3DShell.m_pD3D->Release();

	//pDeviceInfo = g_D3DShell.PickDefaultDev(&pAdapterInfo);
	//pModeInfo = g_D3DShell.PickDefaultMode(pDeviceInfo, 16);
	//g_Device.FreeAll();
	//g_Device.CreateDevice(pAdapterInfo, pDeviceInfo, pModeInfo);

	//pAdapterInfo = g_Device.GetAdapterInfo();
	//pDeviceInfo = g_Device.GetDeviceInfo();
	//
	//pModeInfo = g_Device.SetPresentationParams();

	

	//g_Device.SetMode(pModeInfo);

	//SetDword(0x646BC4, (DWORD)&sub_10002F40);
	//SetDword(0x55F046, (DWORD)&sub_10003670);

	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//HMODULE hKernel32 = GetModuleHandle("kernel32");

	//FARPROC pSetProcessDEPPolicy = GetProcAddress(hKernel32, "SetProcessDEPPolicy");

	//HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)pSetProcessDEPPolicy, 0 /* disable DEP */, 0, NULL);

	//WaitForSingleObject(hThread, INFINITE);

	//CloseHandle(hProcess);

	//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	/*SetProcessWorkingSetSizeEx(hProcess,
		(60 * 1024 * 1024),
		(100 * 1024 * 1024),
		QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE
		);*/

	/*SetProcessWorkingSetSizeEx(hProcess,
		0xFFFFFFFF,
		0xFFFFFFFF,
		QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE
		);*/

	/*SetProcessWorkingSetSizeEx(
		hProcess,
		(50 * 1024 * 1024),
		(1024 * 1024 * 1024),
		QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE
	);*/

	SetProcessWorkingSetSize(hProcess, 0xFFFFFFFF, 0xFFFFFFFF);

	//SetThreadPriority(hProcess, -2);

	//WaitForSingleObject(hThread, INFINITE);

	//CloseHandle(hProcess);

	// 0x63A508
	// 0x6DE340
	//
	//
	//ofstream myfile;
	//myfile.open("E:\\EntryProc.txt");
	//myfile << (DWORD*)(void*)CheckFuck << "\n";
	//myfile << (DWORD*)(&CheckFuck) << "\n";
	//myfile << (DWORD*)*((DWORD*)((DWORD*)0x5281F4)) << "\n";
	////myfile << fname << "\n";
	////myfile << strcmp(ParentName, fname) << "\n";
	//myfile.close();

	/*if (e <= 0)
	{
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		CreateProcess("Game.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		SafeExitProcess();
		return;
	}*/
	/*ofstream myfile;
	myfile.open("E:\\EntryProc.txt");
	myfile << pid << "\n";
	myfile << ppid << "\n";
	myfile << e << "\n";
	myfile << fname << "\n";
	myfile.close();*/

	////GetCurrentProcess(),
	//SetProcessWorkingSetSize(hProcess, 0xFFFFFFFF, 0xFFFFFFFF);
	//SetProcessWorkingSetSizeEx(
	//	hProcess,
	//	(100 * 1024 * 1024), // dwMinimumWorkingSetSize
	//	(100 * 1024 * 1024),  // dwMaximumWorkingSetSize,
	//	QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE
	//);

	//SetThreadPriority(hProcess, THREAD_PRIORITY_HIGHEST);
	//SetThreadPriority(hProcess, -2);

	/*DEVMODEA DevMode;

	if (GetPrivateProfileIntA("Client", "Represent", 2, ".\\config.ini") == 2
		&& EnumDisplaySettingsA(0, 0xFFFFFFFF, &DevMode)
		&& DevMode.dmBitsPerPel != 16)
	{
		DevMode.dmFields |= 262144u;
		DevMode.dmBitsPerPel = 16;
		ChangeDisplaySettingsA(&DevMode, 0);
	}*/

	//SetCompleteHook(0xE8, 0x5281F4, &CheckFuck);

	try
	{
		char* l = setlocale(LC_ALL, "");
		char buffer[256];
		GetModuleFileName(NULL, buffer, MAX_PATH);
		HKEY key;
		char Language[256];
		LPCTSTR value = TEXT("~ RUNASADMIN 16BITCOLOR");
		if (RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows NT\\CurrentVersion\\AppCompatFlags\\Layers", 0, KEY_ALL_ACCESS, &key) == ERROR_SUCCESS)
		{
			DWORD type = REG_SZ, size = sizeof(Language);

			if (RegQueryValueEx(key, TEXT(buffer), 0, NULL, (LPBYTE)Language, &size) == ERROR_SUCCESS)
			{
				std::string regvalue = Language;
				
				if (regvalue.find("RUNASADMIN") != std::string::npos) {
					RegCloseKey(key);
					/*std::ofstream myfile;
					myfile.open("E:\\Layers.txt");
					myfile << Language << "\n";
					myfile << buffer << "\n";
					myfile.close();*/
				}
				else {
					RegSetValueEx(key, TEXT(buffer), 0, REG_SZ, (LPBYTE)value, (strlen(value) + 1) * sizeof(value));
					RegCloseKey(key);
					/*STARTUPINFO si = { sizeof(STARTUPINFO) };
					si.cb = sizeof(si);
					si.dwFlags = STARTF_USESHOWWINDOW;
					si.wShowWindow = SW_SHOWNORMAL;
					PROCESS_INFORMATION pi;*/
					//CreateProcess(gProtect.m_MainInfo.ClientName, gProtect.m_MainInfo.ClientName, NULL, NULL, FALSE, REALTIME_PRIORITY_CLASS, NULL, NULL, &si, &pi);
					//CreateProcess(buffer, buffer, NULL, NULL, FALSE, REALTIME_PRIORITY_CLASS, NULL, NULL, &si, &pi);

					if (gProtect.m_MainInfo.Verify == 1) {
						STARTUPINFO si = { sizeof(STARTUPINFO) };
						si.cb = sizeof(si);
						si.dwFlags = STARTF_USESHOWWINDOW;
						si.wShowWindow = SW_NORMAL;
						PROCESS_INFORMATION pi;
						CreateProcess("Game.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
						SafeExitProcess();
						return;
					}
					else
					{
						STARTUPINFO si = { sizeof(STARTUPINFO) };
						si.cb = sizeof(si);
						si.dwFlags = STARTF_USESHOWWINDOW;
						si.wShowWindow = SW_NORMAL;
						PROCESS_INFORMATION pi;
						CreateProcess("game_y.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
						SafeExitProcess();
						return;
					}

					

					//SafeExitProcess();
				}
				//if (strcmp(Language, "ADMIN") == 0)
				//{
				//}
				//regvalue.c_str
				/*strcpy_s(Language, "~ RUNASADMIN 16BITCOLOR");*/
				
				//RegCloseKey(key);
			}
			else
			{
				RegSetValueEx(key, TEXT(buffer), 0, REG_SZ, (LPBYTE)value, (strlen(value) + 1) * sizeof(value));
				RegCloseKey(key);
				//STARTUPINFO si = { sizeof(STARTUPINFO) };
				//si.cb = sizeof(si);
				//si.dwFlags = STARTF_USESHOWWINDOW;
				//si.wShowWindow = SW_SHOWNORMAL;
				//PROCESS_INFORMATION pi;
				//CreateProcess(gProtect.m_MainInfo.ClientName, NULL, NULL, NULL, FALSE, REALTIME_PRIORITY_CLASS, NULL, NULL, &si, &pi);
				//CreateProcess(buffer, NULL, NULL, NULL, FALSE, REALTIME_PRIORITY_CLASS, NULL, NULL, &si, &pi);
				//SafeExitProcess();
				if (gProtect.m_MainInfo.Verify == 1) {
					STARTUPINFO si = { sizeof(STARTUPINFO) };
					si.cb = sizeof(si);
					si.dwFlags = STARTF_USESHOWWINDOW;
					si.wShowWindow = SW_NORMAL;
					PROCESS_INFORMATION pi;
					CreateProcess("Game.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
					SafeExitProcess();
					return;
				}
				else {
					STARTUPINFO si = { sizeof(STARTUPINFO) };
					si.cb = sizeof(si);
					si.dwFlags = STARTF_USESHOWWINDOW;
					si.wShowWindow = SW_NORMAL;
					PROCESS_INFORMATION pi;
					CreateProcess("game_y.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
					SafeExitProcess();
					return;
				}

			}
		}
		//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	}
	catch (...)
	{
	}

	/*if (FileExists("jxguard.bmp"))
	{
		SplashInit.SetBitmap("jxguard.bmp");
		SplashAlert.SetBitmap("jxguard.bmp");
		SplashError.SetBitmap("jxguard.bmp");
	}*/

	//SetDword(0x646BC4, (DWORD)&SetWorkingDir);
	//SetWorkingDir();

	if (gProtect.m_MainInfo.Verify == 1) {

		try
		{
			DWORD pid, ppid;
			int e;
			const char* ParentName = "Game.exe";
			char fname[MAX_PATH] = { 0 };
			pid = GetCurrentProcessId();

			//try
			//{
			//	HANDLE      processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

			//	HMODULE hKernel32 = GetModuleHandle("kernel32");

			//	FARPROC pSetProcessDEPPolicy = GetProcAddress(hKernel32, "SetProcessDEPPolicy");

			//	HANDLE hThread = CreateRemoteThread(processHandle, NULL, 0, (LPTHREAD_START_ROUTINE)pSetProcessDEPPolicy, 0 /* disable DEP */, 0, NULL);

			//	WaitForSingleObject(hThread, INFINITE);

			//	CloseHandle(hThread);

			//	int errorCode = SetProcessWorkingSetSizeEx(
			//		processHandle,
			//		(100 * 1024 * 1024), // dwMinimumWorkingSetSize
			//		(100 * 1024 * 1024),  // dwMaximumWorkingSetSize,
			//		QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE
			//	);
			//}
			//catch (...)
			//{
			//}

			ppid = getParentPID(pid, fname);
			getProcessName(ppid, fname, MAX_PATH);
			e = GetModuleBaseAddress(ppid, fname);
			e = GetProcessBaseAddress(ppid);

			if (strncmp(ParentName, fname, 9) != 0)
			{
				STARTUPINFO si = { sizeof(STARTUPINFO) };
				si.cb = sizeof(si);
				si.dwFlags = STARTF_USESHOWWINDOW;
				si.wShowWindow = SW_NORMAL;
				PROCESS_INFORMATION pi;
				CreateProcess("Game.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
				SafeExitProcess();
				return;
			}

		}
		catch (...)
		{
		}
	}

	//SetCompleteHook(0xE8, 0x52718B, &SetRepresentShell); // Render 2  sub_420820();

	//SetCompleteHook(0xE8, 0x420866, &Main_RenderWindows); // Render 2  sub_420820();

	//g_pRepresentShell = (iRepresentShell*)0x6DE30C;
	//b_sub_529270(g_pRepresentShell, );
	//KRepresentShell2* m_RepresentShell;
	//if(m_RepresentShell->Create(800, 600, false))
	//	MessageBox(0, "OK", "Info", MB_OK | MB_ICONINFORMATION);
	//else
	//	MessageBox(0, "Error", "Info", MB_OK | MB_ICONINFORMATION);
	//TCHAR buffer[80];
	//sprintf(buffer, "Test %d", 80);
	//m_RepresentShell->OutputText(12, buffer, KRF_ZERO_END, 10, 20, 0xffffffff, 3, 5);

	//DWORD dwType = REG_SZ;
	//char result[MAX_PATH];
	//char pBuf[256];
	//DWORD dwRet;
	//size_t len = sizeof(pBuf);
	//int bytes = GetModuleFileName(NULL, pBuf, len);


	//HKEY hkey = NULL;
	//LONG lret = RegOpenKeyExW(HKEY_CURRENT_USER, L"SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers", NULL, KEY_QUERY_VALUE, &hkey);
	//if (lret == ERROR_SUCCESS)
	//{
	//	CHAR value[1024] = {};
	//	DWORD dwvalue = sizeof(value);
	//	DWORD type = REG_SZ;

	//	/*lret = RegQueryValueExA(hkey, TEXT(pBuf), NULL, &type, (LPBYTE)&value, &dwvalue);
	//	if (lret == ERROR_SUCCESS)
	//	{
	//		
	//	}*/

	//	RegSetValueEx(hkey, TEXT(pBuf), 0, dwType, (LPBYTE)"~ 16BITCOLOR", strlen("~ 16BITCOLOR") * sizeof(char));
	//	RegCloseKey(hkey);
	//	hkey = NULL;
	//}

	//unsigned char ciphertext[1024];

	//memset(&ciphertext, 0, sizeof(ciphertext));

	//int cipherlen = EncryptStringProtect((unsigned char*)"45.76.190.5", 12, (unsigned char*)&ciphertext);
	//std::string buff_base64_1 = base64_encode((unsigned char*)&ciphertext, cipherlen, false);

	//memset(&ciphertext, 0, sizeof(ciphertext));

	//cipherlen = EncryptStringProtect((unsigned char*)"15.204.195.65", 14, (unsigned char*)&ciphertext);
	//std::string buff_base64_2 = base64_encode((unsigned char*)&ciphertext, cipherlen, false);

	//memset(&ciphertext, 0, sizeof(ciphertext));

	//cipherlen = EncryptStringProtect((unsigned char*)"14.225.254.30", 14, (unsigned char*)&ciphertext);
	//std::string buff_base64_3 = base64_encode((unsigned char*)&ciphertext, cipherlen, false);
	 
	//std::string buff_decodebase64_1 = base64_decode("C+vFUaybPHWaOGaKv/GRIA==", false);

	//ofstream myfile;
	//myfile.open("E:\\countryCode.txt");
	//myfile << dataread.c_str() << "\n";
	//myfile << dataread.size() << "\n";
	//myfile << info.dwMajorVersion << "\n";
	//myfile << ciphertext << "\n";
	//myfile << buff_base64_1.c_str() << "\n";
	//myfile << buff_base64_2.c_str() << "\n";
	//myfile << buff_base64_3.c_str() << "\n";
	//myfile << buff_decodebase64_1.c_str() << "\n";
	//myfile.close();

	/*try {
		remove("ddraw.dll");
		std::ifstream ifile("ddraw.dll");
		if (ifile)
		{
			DeleteFile("ddraw.dll");
		}
	}
	catch (...) {
		
	}*/
	
	gLog.AddLog(1, "Logs");

	//CheckSystemInformation();

	if (gProtect.ReadMainFile("GameProtect.sys") == 0)
	{
		//gLog.Output(LOG_DEBUG, GetEncryptedString(18));
		MessageBox(0, "Khong the phan tich GameProtect.sys cap nhat lai game", "Game", MB_ICONINFORMATION);
		SafeExitProcess();
		return;
	}

	/*ofstream myfile;
	myfile.open("E:\\GameProtect.txt");
	myfile << gProtect.m_MainInfo.ServerName << "\n";
	myfile << gProtect.m_MainInfo.AccountWebsite << "\n";
	myfile << gProtect.m_MainInfo.ClientName << "\n";
	myfile << gProtect.m_MainInfo.ClientSerial << "\n";
	myfile << gProtect.m_MainInfo.ClientVersion << "\n";
	myfile << gProtect.m_MainInfo.AuthServerPort << "\n";
	myfile << gProtect.m_MainInfo.AuthIpAddress << "\n";
	myfile.close();*/

#if AUTH_TYPE == 1

	/*OSVERSIONINFOEX info;
	ZeroMemory(&info, sizeof(OSVERSIONINFOEX));
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((LPOSVERSIONINFO)&info);*/
	bool nCheckLai = false;
	//std::string authurl = "https://geo.fwprotect.com/?type=countryCode";
	//gProtect.m_MainInfo.ClientSerial
	int MaxCheck = 0;
	int AuthCount = 1;
	int MaxAuthCount = 2;
	char buffURL[128];
	char HardwareId[32];
	char buffText[512];

	unsigned char ciphertext[8096];

	memset(&ciphertext, 0, sizeof(ciphertext));
	memset(&buffText, 0, sizeof(buffText));
	memcpy(&HardwareId, GetHardwareIdMain(), sizeof(HardwareId));
	strcpy(gProtect.m_HardwareId, HardwareId);
	Sleep(300);

CheckLai:

	if (strlen(HardwareId) <= 0)
	{
		memcpy(&HardwareId, GetHardwareIdMain(), sizeof(HardwareId));
		
		Sleep(300);
		goto CheckLai;
	}

	HardwareId[sizeof(HardwareId)] = '\0';
	strcpy(c_HardwareId, HardwareId);
	int count = sprintf(buffText, "%d|%s", gProtect.m_MainInfo.ClientVersion, HardwareId);

	int cipherlen = EncryptStringProtect((unsigned char*)&buffText, count, (unsigned char*)ciphertext);

	std::string buff_base64_1 = base64_encode((unsigned char*)&ciphertext, cipherlen, false);
	/*if (info.dwMajorVersion >= 10)
		sprintf(buffURL, "https://auth%d.jxguard365.xyz/client-%s-%s.nnb", AuthCount, gProtect.m_MainInfo.ClientSerial, buff_base64_1.c_str());
	else
		sprintf(buffURL, "http://auth%d.jxguard365.xyz/client-%s-%s.nnb", AuthCount, gProtect.m_MainInfo.ClientSerial, buff_base64_1.c_str());*/
	sprintf(buffURL, "http://auth.volam1pk.net/client-%d-%s-%s.nnb", gProtect.m_MainInfo.ClientVersion, gProtect.m_MainInfo.ClientSerial, HardwareId);
	std::string authurl = buffURL;
	std::string dataread = DownloadString(authurl);
	Sleep(500);
	//char requestdata[1024];
	//memcpy(requestdata, dataread.c_str(), dataread.size());
	unsigned char decryptedtext[8096];

	memset(&decryptedtext, 0, sizeof(decryptedtext));

	int decryptedtext_len;

	decryptedtext_len = DecryptStringProtect((unsigned char*)dataread.c_str(), dataread.size(), decryptedtext);

	decryptedtext[decryptedtext_len] = '\0';

	std::string delimiter = "|";
	std::string tp(reinterpret_cast<char const*>(decryptedtext), decryptedtext_len);
	std::vector<std::string> results = split(tp, delimiter);
	//dataread.clear();

	/*ofstream myfile2;
	myfile2.open("E:\\dataread2.txt");
	myfile2 << buffText << "\n";
	myfile2 << count << "\n";
	myfile2 << ciphertext << "\n";
	myfile2 << cipherlen << "\n";
	myfile2 << buff_base64_1.c_str() << "\n";
	myfile2 << buff_base64_1.size() << "\n";
	myfile2 << decryptedtext << "\n";
	myfile2 << decryptedtext_len << "\n";
	myfile2 << results.size() << "\n";
	myfile2.close();*/
	Sleep(500);

	if (sizeof(decryptedtext) > 32 && results.size() > 30 && atoi(results[7].c_str()) > 0)
	{
		gProtect.m_IsConnectedAuth = true;

		gProtect.m_MainInfo.nLimit = atoi(results[31].c_str());

		gProtect.m_CheckLimit = atoi(results[29].c_str());

		gProtect.m_CheckVPN = atoi(results[25].c_str());

		gProtect.m_CheckProcess = atoi(results[27].c_str());

		gProtect.m_CheckCount = 0;

		gProtect.m_AllowLimit = atoi(results[45].c_str());

		gAuthenticate.m_SplashScreen = atoi(results[33].c_str());

		memcpy(gProtect.m_MainInfo.ClientSerial, results[5].c_str(), sizeof(gProtect.m_MainInfo.ClientSerial));

		gThanPhap = atoi(results[9].c_str());

		gFakeUserBattles = atoi(results[23].c_str());

		gStateSkill = atoi(results[19].c_str());

		gProtect.m_CheckMayAo = atoi(results[39].c_str());

		memcpy(gProtect.m_WanIpAddress, results[35].c_str(), sizeof(gProtect.m_WanIpAddress));

		char buffSplashInit[128];

		wsprintf(buffSplashInit, gMessage.GetMessage(0), gProtect.m_MainInfo.ServerName);
		//gAuthenticate.m_SplashScreen = 1;
		if (gAuthenticate.m_SplashScreen == 1)
			SplashScreen(&SplashInit, 0, 1, buffSplashInit, 1500);

		if (results[37].size() > 0)
		{
			std::string delimiter = "$";

			std::vector<std::string> dumplists = split(results[37], delimiter);

			gDumpListMaxCount = dumplists.size() - 1;

			//ofstream myfile2;

			//myfile2.open("E:\\dumplist.txt");

			//myfile2 << gDumpListMaxCount << "\n";

			for (int n = 0; n < gDumpListMaxCount; n++)
			{

				std::string delimiter1 = "	";

				std::vector<std::string> dump = split(dumplists[n], delimiter1);

				DUMP_LIST_INFO lpInfo;

				lpInfo.type = ((BYTE)atoi(dump[0].c_str()));

				char TempHex[11];

				strcpy_s(TempHex, dump[1].c_str());

				sscanf_s(TempHex, "%X", &lpInfo.offset);

				for (int i = 0; i < 32; i++)
				{
					lpInfo.dump[i] = ((BYTE)atoi(dump[2 + i].c_str()));
				}

				char TempName[255];

				strcpy_s(TempName, dump[dump.size() - 1].c_str());

				TempName[31] = ((strlen(TempName) > 31) ? 0 : TempName[31]);

				strcpy_s(lpInfo.name, TempName);

				//strcpy_s(lpInfo.name, dump[dump.size() - 1].c_str());

				//myfile2 << lpInfo.type << "\n";
				//myfile2 << lpInfo.offset << "\n";
				//myfile2 << lpInfo.name << "\n";

				gListManager.gDumpListInfo.push_back(lpInfo);
			}

			//myfile2.close();

			gDumpListOK = 1;

		}

		if (results[41].size() > 0)
		{
			std::string delimiter = "$";

			std::vector<std::string> processlists = split(results[41], delimiter);

			gProcessListMaxCount = processlists.size() - 1;

			if (gProcessListMaxCount > 0)
			{
				for (int n = 0; n < gProcessListMaxCount; n++)
				{
					std::string delimiter1 = processlists[n];

					gListManager.gProcessListInfo.push_back(delimiter1);

				}

				gProcessListOK = 1;
			}
		}
		
		if (results[43].size() > 0)
		{
			std::string delimiter = "$";

			std::vector<std::string> processlists = split(results[43], delimiter);

			gWindowListMaxCount = processlists.size() - 1;

			if (gWindowListMaxCount > 0)
			{
				for (int n = 0; n < gWindowListMaxCount; n++)
				{

					WNDW_LIST_INFO winproc;

					winproc.type = 0;

					strcpy_s(winproc.text, processlists[n].c_str());

					gListManager.gWindowListInfo.push_back(winproc);

				}

				gWindowListOK = 1;
			}
		}

		if (results[47].size() > 0)
		{
			std::string delimiter = "$";

			std::vector<std::string> processlists = split(results[47], delimiter);

			gClassListMaxCount = processlists.size() - 1;

			if (gClassListMaxCount > 0)
			{
				for (int n = 0; n < gClassListMaxCount; n++)
				{
					WNDW_LIST_INFO winproc;

					winproc.type = 1;

					strcpy_s(winproc.text, processlists[n].c_str());

					gListManager.gWindowListInfo.push_back(winproc);
				}

				gClassListOK = 1;
			}
		}

		//Sleep(300);
		

		/*if (memcmp(dataread.c_str(), "VN", 2) == 0)
		{
			gAuthenticate.SetAuthAddressIP("C+vFUaybPHWaOGaKv/GRIA==", 1);
			gAuthenticate.SetAuthAddressIP("tRpKguL3KeGNRNAzIqlnzg==", 2);
			gAuthenticate.SetAuthAddressIP("RubaYzPsyi/TaJjHa76W2A==", 3);
		}
		else if (memcmp(dataread.c_str(), "US", 2) == 0)
		{
			gAuthenticate.SetAuthAddressIP("RubaYzPsyi/TaJjHa76W2A==", 1);
			gAuthenticate.SetAuthAddressIP("tRpKguL3KeGNRNAzIqlnzg==", 2);
			gAuthenticate.SetAuthAddressIP("C+vFUaybPHWaOGaKv/GRIA==", 3);
		}
		else
		{
			gAuthenticate.SetAuthAddressIP("tRpKguL3KeGNRNAzIqlnzg==", 1);
			gAuthenticate.SetAuthAddressIP("C+vFUaybPHWaOGaKv/GRIA==", 2);
			gAuthenticate.SetAuthAddressIP("RubaYzPsyi/TaJjHa76W2A==", 3);
		}*/
	}
	else
	{
		if (AuthCount == MaxAuthCount)
		{
			gProtect.m_IsConnectedAuth = false;
			gThanPhap = 0;
			gFakeUserBattles = 0;
			gStateSkill = 0;
			SetByte(0x52B9F9, 65);
			memset(gProtect.m_MainInfo.ClientSerial, 0, sizeof(gProtect.m_MainInfo.ClientSerial) - 1);
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(1), 2000);
			return;
		}
		else
		{
			//authurl = "http://geo.fwprotect.com/?type=countryCode";
			if (MaxCheck >= 5)
			{
				MaxCheck = 0;
				AuthCount++;
			}
			
			memset(buffURL, 0, sizeof(buffURL));
			sprintf(buffURL, "http://auth.volam1pk.net/client-%d-%s-%s.nnb", gProtect.m_MainInfo.ClientVersion, gProtect.m_MainInfo.ClientSerial, HardwareId);
			authurl = buffURL;
			MaxCheck++;
			goto CheckLai;
		}
		/*else
		{
			gAuthenticate.SetAuthAddressIP("tRpKguL3KeGNRNAzIqlnzg==", 1);
			gAuthenticate.SetAuthAddressIP("C+vFUaybPHWaOGaKv/GRIA==", 2);
			gAuthenticate.SetAuthAddressIP("RubaYzPsyi/TaJjHa76W2A==", 3);
		}*/
	}
	

#endif

	INIReader reader("AntiVolam.ini");

	if (reader.ParseError() != 0)
		return;

	gProtect.m_StateSkill = (int)reader.GetInteger("GENERAL", "DisableStateSkill", 0);
	gProtect.m_GiamCPU = (int)reader.GetInteger("GENERAL", "GiamCPU", 0);
	gProtect.m_GiamHinhAnh = (int)reader.GetInteger("GENERAL", "GiamHinhAnh", 0);
	gProtect.m_NpcLevel1 = (int)reader.GetInteger("GENERAL", "NpcLevel1", 0);
	gProtect.m_NpcLevel2 = (int)reader.GetInteger("GENERAL", "NpcLevel2", 0);
	gProtect.m_Resolution = (int)reader.GetInteger("GENERAL", "Resolution", 0);
	gProtect.m_FixMauTim = (int)reader.GetInteger("GENERAL", "FixMau", 0);

	if (gProtect.m_Resolution > 0)
	{
		SetWord(0x526F2A, 0x300);
		SetWord(0x526F2F, 0x400);
		SetWord(0x5270D5, 0x300);
		SetWord(0x5270DA, 0x400);
		SetWord(0x5271BF, 0x300);
		SetWord(0x5271C4, 0x400);
		//const char* package1024 = "package1.ini";
		DWORD OldProtect;

		VirtualProtect((void*)0x646D9C, 2, PAGE_EXECUTE_READWRITE, &OldProtect);

		memcpy((void*)0x646D9C, "\package1.ini", sizeof("\package1.ini"));

		VirtualProtect((void*)0x646D9C, 2, OldProtect, &OldProtect);
	}

	//SetByte(0x5C45B3, 0x6); // Translife 6

#if (CALL_AUTOUPDATE==1)

	if (!IsProcessRunning("autoupdate.exe"))
	{
		STARTUPINFO si = { sizeof(STARTUPINFO) };
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		PROCESS_INFORMATION pi;
		CreateProcess("autoupdate.exe", NULL, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi);
		SafeExitProcess();
		return;
	}

	/*HMODULE hModule = LoadLibraryA("CoreClient.dll");

	void (*EntryProc)() = (void(*)())GetProcAddress(hModule, "CheckAutoUpdate");

	if (EntryProc)
	{
		EntryProc();
	}*/

#endif // (CALL_AUTOUPDATE==1)

	/*
	

	
	
	

	*/

	/*if(gConnection.Init(HackServerProtocolCore) == 0)
	{
		SplashScreen(&SplashError,2,1,gMessage.GetMessage(4),5000);
		return;
	}*/

	//========= Get New HardwareId ===========\\

	gAuthenticate.m_ProcessCountTemp = 0;
	//GetCurrentProcessId()
#if DEBUG == 0

	if (gProtect.m_CheckMayAo == 1)
	{
		if (gProtect.m_AllowLimit == 0)
		{
			if (WLCheckVirtualPC())
			{
				TCHAR buffer[256];
				sprintf(buffer, TEXT("Tr� ch�i kh�ng th� ch�y trong m�y �o"));
				SplashScreen(&SplashError, 2, 1, buffer, 5000);
				SafeExitProcess();
				return;
			}

			/*if (IsInsideVPC())
			{
				TCHAR buffer[256];
				sprintf(buffer, TEXT("Tr� ch�i kh�ng th� ch�y trong m�y �o"));
				SplashScreen(&SplashError, 2, 1, buffer, 5000);
				SafeExitProcess();
				return;
			}

			if (IsInsideVMWare())
			{
				TCHAR buffer[256];
				sprintf(buffer, TEXT("Tr� ch�i kh�ng th� ch�y trong m�y �o"));
				SplashScreen(&SplashError, 2, 1, buffer, 5000);
				SafeExitProcess();
				return;
			}*/
		}
	}

	if (gProtect.m_CheckVPN == 1)
	{
		if (gProtect.m_AllowLimit == 0)
			isVPN();
	}

	if (!CheckProcess(0, 1))
	{
		/*if (gAuthenticate.m_SplashScreen == 1)
			SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(3), 3000);
		else
		{
			MessageBox(0, gMessage.GetMessage(3), "Game", MB_ICONINFORMATION);
			exit(0);
			return;
		}*/
		SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(3), 3000);
	}

#endif // DEBUG == 0

	

	/*unsigned char mac[6] = { '\0' };
	char cIpAddressID[8];
	char cMacAddressID[8];
	char textmix[100];
	char MixMacAddress[50];

	memcpy(gProtect.m_SmallHardwareId, SmallHardwareId(), sizeof(gProtect.m_SmallHardwareId));

	sprintf(gProtect.m_IpAddress, "%s", GetLocalIpAddressNew().c_str());

	get_mac(mac, gProtect.m_IpAddress);

	sprintf(gProtect.m_MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	DWORD IPId = *(DWORD*)(&gProtect.m_IpAddress[0x00]) ^ *(DWORD*)(&gProtect.m_IpAddress[0x16]) ^ 0x68BBBB79;

	sprintf(MixMacAddress, "%08X%02X%02X%02X%02X%02X%s", IPId, mac[1], mac[2], mac[3], mac[4], mac[5], gProtect.m_SmallHardwareId);

	std::string newhwid = licenseKeyFormatting(MixMacAddress, 5, 4);

	memcpy(gProtect.m_HardwareId, newhwid.c_str(), sizeof(gProtect.m_HardwareId));*/

	//========= END Get New HardwareId ===========\\
	// 
	//memcpy(gPlayerSafe.HardwareSerialId, GetHardwareId(), sizeof(gPlayerSafe.HardwareSerialId));

	//DWORD dwOldProtect = 0;
	// ----
	//VirtualProtect((LPVOID)0x1607BA0, 1000, PAGE_EXECUTE_READWRITE, &dwOldProtect);
	
	//ofstream myfile;
	//myfile.open("c:\\npcbobo.txt");
	////myfile << dword_6AE930 << "\n";
	////myfile << GetHardwareId() << "\n";
	//myfile << (int)(DWORD*)0x1607BA0 << "\n";
	//myfile << (int)(DWORD*)0x1606920 << "\n";
	//myfile << *(int*)0x1607BA0 << "\n";
	//myfile << *(int*)0x1606920 << "\n";
	//myfile << *(int*)(DWORD*)0x68F9D0 << "\n";
	//myfile << (char*)(DWORD*)0x1607844 << "\n";
	//myfile.close();

	//VirtualProtect((LPVOID)0x1607BA0, 1000, dwOldProtect, 0);

//#if (ICONSKILL==1)
	/*HANDLE v1;

	v1 = GetCurrentProcess();

	EnumWindows(EnumWindowsProcMy, GetProcessId(v1));

	SetThreadPriority(v1, THREAD_PRIORITY_HIGHEST);*/

//#endif

#if (USE_CLIENTV8 == 1)
	SetCompleteHook(0xE8, 0x53C1FA, &hook_sub_52B950);
	//SetByte((DWORD)0x1919030, 0);
#else
	SetCompleteHook(0xE8, 0x52980A, &hook_sub_52B950);
	//SetCompleteHook(0xE8, 0x52A80F, &hook_sub_528CF0);
#endif

#if (USE_LIMIT_AUTH == 1)

	#if (USE_LIMIT_AUTH_MULTI == 1)

		#if (USE_CLIENTV8 == 1)
			//SetCompleteHook(0xE8, 0x53C1FA, &hook_sub_52B950);
			//SetByte((DWORD)0x1919030, 1);
		#else
			SetCompleteHook(0xE8, 0x529721, &hook_sub_529721); // Select Server
			SetCompleteHook(0xE8, 0x528EEA, &hook_sub_545730); // Leave Game
			//SetCompleteHook(0xE9, 0x539C35, &hook_sub_545730); // v6
			SetCompleteHook(0xE8, 0x528E7A, &hook_sub_545730); // Enter game
			//SetCompleteHook(0xE8, 0x459AF5, &LoadLoginLogicInfo); // v6
			//SetCompleteHook(0xFF, 0x459B40, &LoadSwitch); // v6
			//SetCompleteHook(0xE8, 0x52A80F, &fConnectToGameSvr); // v6
			//SetCompleteHook(0xE8, 0x54568D, &cPrintLoading);
			//SetCompleteHook(0xE8, 0x52C050, &fun_TobeDisconnect);
			//SetCompleteHook(0xE8, 0x530188, &hook_sub_52CB70); // Enter game
			SetDword((DWORD)0x55F3AC, (DWORD)&sub_557280);
		#endif
		
		gPlayerSafe.m_ConnectionID = 0;
		gProtect.m_SendSelectServer = false;
		gPlayerSafe.m_IsGameSetOffline = false;

	#endif // (USE_LIMIT_AUTH == 1)

#endif // (USE_LIMIT_AUTH == 1)
		// Ngoi sao trung sinh
		SetByte((DWORD)0x5C4582, 96);
		SetByte((DWORD)0x5C4952, 96);
		SetByte((DWORD)0x5C4937, 7);
		SetByte((DWORD)0x5783A7, 7);

		// fix mau tim
		SetByte((DWORD)0x52A0B1, 0xF0);
		SetByte((DWORD)0x52A0B3, 0xF0);
		SetByte((DWORD)0x53801F, 0xF0);
		SetByte((DWORD)0x538021, 0xF0);

		SetByte((DWORD)0x578071, 0xF0);
		SetByte((DWORD)0x578073, 0xF0);
		SetByte((DWORD)0x5785DF, 0xF0);
		SetByte((DWORD)0x5785E1, 0xF0);

		SetByte((DWORD)0x578965, 0xF0);
		SetByte((DWORD)0x57897D, 0xF0);
		SetByte((DWORD)0x57899B, 0xF0);
		SetByte((DWORD)0x57899F, 0xF0);

		// 0052A0B1	0x1	FF 	F0
		// 0053801F	0x1	FF 	F0 
		// 00538021	0x1	E7 	F0 
		//SetByte((DWORD)0x5789A4, 0xF0);
		//SetByte((DWORD)0x5789B3, 0xF0);
		SetByte((DWORD)0x67D37D, 0xF0);
		SetByte((DWORD)0x67D37F, 0xF0);

	/*	SetByte((DWORD)0x66F5DF, 0xF0);
		SetByte((DWORD)0x66F5E1, 0xF0);

		SetByte((DWORD)0x67DEB5, 0xF0);
		SetByte((DWORD)0x67DEB7, 0xF0);
		SetByte((DWORD)0x67DEBB, 0xF0);
		SetByte((DWORD)0x67DEBD, 0xF0);*/
		
		//SetByte((DWORD)0x641FBF, 20);
		//SetByte((DWORD)0x63E27A, 00);
		//SetByte((DWORD)0x63E27B, 00);
		//SetByte((DWORD)0x63B999, 34);

		DWORD OldProtectAW;

		VirtualProtect((void*)0x640D60, 2, PAGE_EXECUTE_READWRITE, &OldProtectAW);

		memcpy((void*)0x640D60, &gProtect.m_MainInfo.AccountWebsite, sizeof(gProtect.m_MainInfo.AccountWebsite));

		VirtualProtect((void*)0x640D60, 2, OldProtectAW, &OldProtectAW);

		// END Ngoi sao trung sinh
		SetCompleteHook(0xE8, 0x527569, &CheckExitGame); // v6

		//SetCompleteHook(0xE8, 0x530188, &Func_52CB70); // fix chat
		/*BYTE v1[5];
		v1[0] = -24;
		*(DWORD*)&v1[1] = (DWORD)&Func_52CB00;
		*(unsigned int*)(0x530188) = *(DWORD*)v1;*/
		//*(unsigned int*)(0x53018C) = ((unsigned int)&Func_52CB70) >> 0x18;

		//SetCompleteHook(0xE8, 0x5300D3, &Func_52CB00); // fix chat
		//SetCompleteHook(0xE8, 0x508BE8, &Func_509250); // fix chat

		//SetCompleteHook(0xE8, 0x4BCD8B, &query_rank); // v6

		//SetCompleteHook(0xFF, 0x4BCBD6, &hook_sprintf); // v6

//#if (ICONSKILL==0)
	//DetourRestoreAfterWith();
	//DetourTransactionBegin();
	//DetourUpdateThread(GetCurrentThread());

	////pCreate = (fnCreateRepresentShell)DetourFindFunction("represent2.dll", "CreateRepresentShell");//Pointer the the original MBA (MessageBoxA) function.
	//DetourAttach((PVOID*)0x5565EA, (PVOID)SyncStateSkill);
	////SetCompleteHook(0xE8, 0x5565EA, &SyncStateSkill); // v6
	//DetourAttach(&(PVOID&)*(DWORD*)0x5565EA, (PVOID)SyncStateSkill);
	//DetourTransactionCommit();

	//SetCompleteHook(0xE9, 0x420C64, &UiSkillStateInitialize); // v6
	//SetCompleteHook(0xE8, 0x459A7B, &InGame); // v6
	//SetCompleteHook(0xE8, 0x420FA4, &StartGame); // v6
	//SetCompleteHook(0xE8, 0x5565B4, &GetPlayerIndex); // v6

	//SetCompleteHook(0xE8, 0x5565EA, &SyncStateSkill); // v6
	//SetCompleteHook(0xE8, 0x57B176, &SyncStateSkill); // v6
	//SetCompleteHook(0xE8, 0x57B1BB, &SyncStateSkill); // v6

	//SetCompleteHook(0xE8, 0x5565B4, &DetectSynSkill); // v6
	//SetCompleteHook(0xE9, 0x5668E2, &CleanSynSkill); // v6
	//DWORD oldProtection2;
	//VirtualProtect((LPVOID)0x57B080, 2048, PAGE_EXECUTE_READWRITE, &oldProtection2);
	 
	//SetCompleteHook(0xE8, 0x57B440, &SetSkillStateAddTail); // v6
	 
	//SetCompleteHook(0xE8, 0x420C46, &UiSkillStateInitialize); // v6
	//SetCompleteHook(0xE8, 0x420CC1, &StartGame); // v6
	//SetCompleteHook(0xE8, 0x52729F, &StartGame); // v6
	//SetCompleteHook(0xE8, 0x41A2A0, &Main_LoadScheme); // v6
	//SetCompleteHook(0xE8, 0x526F39, &InitRepresentShell); // v6
	
	//std::shared_ptr<KList> ClassToHook(new KList);
	
	//SetCompleteHook(0xE8, 0x52818B, &ByPass); // Render 1 sub_420880();
	
	//SetCompleteHook(0xFF, 0x5281F4, &UiPaint); // Render 2  sub_420820();

	//SetCompleteHook(0xE8, 0x52718B, &SetRepresentShell); // Render 2  sub_420820();

	//SetCompleteHook(0xE8, 0x420866, &Main_RenderWindows); // Render 2  sub_420820();
	
	//SetCompleteHook(0xFF, 0x5272BF, &ByPass); // v6
	//SetCompleteHook(0xE8, 0x585D7A, &ByPass); // v6
	//SetCompleteHook(0xE8, 0x680F5C, &ByPass); // v6
	//SetCompleteHook(0xE8, 0x680F14, &ByPass); // v6
	//SetCompleteHook(0xE8, 0x539251, &ByPass); // v6
	//SetCompleteHook(0xE8, 0x5398E3, &ByPass); // v6
	//SetByte(0x5C6F6B, 6); // v6
//#endif

	//SetCompleteHook(0xFF, 0x5D4241, &SendScriptDataToServe); // v6
	//g_UiBase = (KUiBase*)0x68D090;
	//g_pRepresentShell = (iRepresentShell*)0x6DE30C;
	//g_pCoreShell = (iCoreShell*)0x646C60;

	//hModuleEngine = LoadLibraryA("engine.dll");

	//if (EntryProc)
	//{
	//	//HANDLE ThreadHandles1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryProc, 0, 0, NULL);
	//	//choose one
	//	EntryProc();
	//	//EntryProc("SS2");
	//}

	//ofstream myfile;
	//myfile.open("E:\\example.txt");
	////myfile << v4 << "\n";
	////myfile << (const char*)(void*)0x68D090 << "\n";
	//myfile << (void*)g_UiBase << "\n";
	//myfile << (void*)g_pRepresentShell << "\n";
	//myfile << (void*)g_pCoreShell << "\n";
	////myfile << Buffer << "\n";
	//myfile.close();
	
	//ByPass();

//#if (USE_CLIENTV8 == 0)

	//MessageBox(0, "SplashInit", "Info", MB_OK | MB_ICONINFORMATION);
	//gLog.Output(LOG_DEBUG,GetEncryptedString(24),gDumpListMaxCount,gChecksumListMaxCount,gInternalListMaxCount,gWindowListMaxCount);

	/*if(MEMORY_CHECK_DETACH() == 0)
	{
		SplashScreen(&SplashError,2,1,gMessage.GetMessage(20),5000);
		return;
	}*/

	//InitHackCheck();

	//gProtect.CheckClientFile();

	//gProtect.CheckVerifyFile();

	gProtect.m_IsConnectedAuth = false;

	//SetWord(0x680490, 20);

#if AUTH_TYPE == 0
	gAuthenticate.Check();

	char buff[256];

	wsprintf(buff, gMessage.GetMessage(0), gProtect.m_MainInfo.ServerName);
	//gAuthenticate.m_SplashScreen = 1;
	if (gAuthenticate.m_SplashScreen == 1)
		SplashScreen(&SplashInit, 0, 1, buff, 1000);
#endif

#if AUTH_TYPE == 1

	if (gThanPhap == 1)
	{
		SetCompleteHook(0xE9, 0x576F1E, &hook_sub_5770F1);
	}
	else if (gThanPhap == 2)
	{
		SetCompleteHook(0xE8, 0x5770F1, &hook_sub_5770F1);
	}

	if (gFakeUserBattles == 1)
	{
		//SetCompleteHook(0xE8, 0x4F2D7C, &update_songuoi_tongkim); // cong them so nguoi trong tong kim
		SetCompleteHook(0xE8, 0x4045E3, &update_songuoi_tongkim); // cong them so nguoi trong tong kim
		SetCompleteHook(0xE8, 0x4F2D7C, &thamgia_tongkim); // cong them so nguoi trong tong kim
	}

	if (gStateSkill == 1)
	{
		SetCompleteHook(0xE8, 0x52729F, &StartGame);

		SetCompleteHook(0xE8, 0x459A7B, &InGame); // v6

		//SetCompleteHook(0xE8, 0x52718B, &SetRepresentShell); // Render 2  sub_420820();

		SetCompleteHook(0xE8, 0x420866, &Main_RenderWindows); // Render 2  sub_420820();

		SetCompleteHook(0xE8, 0x420FB4, &CloseAllWindowOutGame); // Render 2  sub_420820();
	}

	//DWORD flOldProtect = 0;
	//DWORD v5 = 0;
	//VirtualProtect((LPVOID)0x55F046, 4u, 0x40u, &flOldProtect);
	//SetCompleteHook(0xE8, 0x55F046, &hook_sub_55E7D);
	//SetCompleteHook(0xE9, 0x5389C5, &sub_10003570);
	//SetCompleteHook(0xE9, 0x5291FC, &sub_10003570);
	//SetCompleteHook(0xE8, 0x5389C9, &sub_10003570);
	//SetCompleteHook(0xE8, 0x554981, &sub_10003570);
	//SetDword(0x55F4D4, (DWORD)(&sub_554940));
	//SetCompleteHook(0xE9, 0x5389C9, &sub_10003570);
	//SetDword(0x55F046, (DWORD)(&hook_sub_55E7D));
	//VirtualProtect((LPVOID)0x55F046, 4u, flOldProtect, &v5);

#endif

	SplashInit.CloseSplash();
	//gStatusDisplayer.Init(hWnd);
	//GetModuleBaseAddress
	

	//HANDLE ThreadHandles1 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&ConnectionReconnectThread, 0, 0, 0);

	//HANDLE ThreadHandles2 = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&ConnectionStatusThread, 0, 0, 0);

	//WaitForSingleObject(ThreadHandles1, 2000);

	//QueryPerformanceFrequency(&Frequency);

	//QueryPerformanceCounter(&InitCounter);

	//gProtect.ThreadHandles[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckThread, 0, 0, (DWORD*)&gThreadCheck.m_CheckThreadID[2]);

	gProtect.ThreadHandles[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ConnectionStatusThread, 0, 0, (DWORD*)&gThreadCheck.m_CheckThreadID[1]);

	//SetThreadPriority(gProtect.ThreadHandles[0], THREAD_PRIORITY_HIGHEST);

	//WaitForSingleObject(gProtect.ThreadHandles[0], 3000);

#if AUTH_TYPE == 0
	while (true)
	{
		Sleep(300);

		if (gProtect.m_IsConnectedAuth)
			break;
	}
#endif

	//if (gProtect.m_FixMauTim >= 1)
	//{
		DWORD oldProtect1, oldProtect2;

		uintptr_t serverdllBaseAddress = GetModuleBaseAddress(pid, "engine.dll");

		HMODULE hModuleEngine = (HMODULE)(serverdllBaseAddress);

		VirtualProtect((LPVOID)(serverdllBaseAddress + 0x41246), 6, PAGE_EXECUTE_READWRITE, &oldProtect1);
		VirtualProtect((LPVOID)(serverdllBaseAddress + 0x41248), 6, PAGE_EXECUTE_READWRITE, &oldProtect2);

		SetByte((DWORD)(serverdllBaseAddress + 0x41246), 0xF0);
		SetByte((DWORD)(serverdllBaseAddress + 0x41248), 0xF0);

		VirtualProtect((LPVOID)(serverdllBaseAddress + 0x41246), 6, PAGE_EXECUTE_READWRITE, &oldProtect1);
		VirtualProtect((LPVOID)(serverdllBaseAddress + 0x41248), 6, PAGE_EXECUTE_READWRITE, &oldProtect2);
	//}

	if (gProtect.m_CheckProcess == 1)
	{
		//HANDLE ThreadHandles1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, (DWORD*)&gThreadCheck.m_CheckThreadID[1]);
		gProtect.ThreadHandles[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, (DWORD*)&gThreadCheck.m_CheckThreadID[1]);

		SetThreadPriority(gProtect.ThreadHandles[1], THREAD_PRIORITY_HIGHEST);

		//WaitForSingleObject(gProtect.ThreadHandles[1], 3000);
	}

	//SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
	/*SetProcessWorkingSetSizeEx(hProcess,
		0xFFFFFFFF,
		0xFFFFFFFF,
		QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE
	);*/
	//SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

	gPlayerSafe.m_IsExitGame = false;

	DWORD lpOldMode;

	//::SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

	//if (MEMORY_PROTECTION_INIT() == 0)
	//{
	//	//gLog.Output(LOG_DEBUG, GetEncryptedString(45));
	//	//MessageBox(0, gMessage.GetMessage(6), "MEMORY_PROTECTION", MB_ICONINFORMATION);
	//	SafeExitProcess();
	//	return;
	//}

	//if (MEMORY_CHECK_DETACH() == 0)
	//{
	//	//gLog.Output(LOG_DEBUG, GetEncryptedString(20));
	//	//MessageBox(0, gMessage.GetMessage(6), "MEMORY_CHECK", MB_ICONINFORMATION);
	//	SafeExitProcess();
	//	return;
	//}

	//if (LIBRARY_LOAD_DETACH() == 0)
	//{
	//	//gLog.Output(LOG_DEBUG, GetEncryptedString(19));
	//	MessageBox(0, gMessage.GetMessage(6), "LIBRARY_LOAD", MB_ICONINFORMATION);
	//	SafeExitProcess();
	//	return;
	//}

	//if (HANDLE_PROTECTION_INIT() == 0)
	//{
	//	//gLog.Output(LOG_DEBUG, GetEncryptedString(46));
	//	SafeExitProcess();
	//	return;
	//}

	//if (API_INIT() == 0)
	//{
	//	//gLog.Output(LOG_DEBUG, GetEncryptedString(22));
	//	//MessageBox(0, gMessage.GetMessage(6), "API_INIT", MB_ICONINFORMATION);
	//	SafeExitProcess();
	//	return;
	//}

	/*if (gProcessManager.Init() == 0)
	{
		MessageBox(0, gMessage.GetMessage(5), "Game", MB_ICONINFORMATION);
		SafeExitProcess();
		return;
	}*/

	//HMODULE hModule = GetModuleHandle(NULL);

	//LoadReferenceAddressTable((HMODULE)hModule, MAKEINTRESOURCE(IDR_BIN1), (DWORD)&NewAddressData1);

	//LoadReferenceAddressTable((HMODULE)hModule, MAKEINTRESOURCE(IDR_BIN2), (DWORD)&NewAddressData2);

	//LoadReferenceAddressTable((HMODULE)hModule, MAKEINTRESOURCE(IDR_BIN3), (DWORD)&NewAddressData3);

	//SetThreadPriority(gProtect.ThreadHandles[1], THREAD_PRIORITY_HIGHEST);

	//WaitForMultipleObjects(3, ThreadHandles, 1, 1000);

	 // (USE_LIMIT_AUTH == 1)
		
		//DWORD oldProtection;
		//VirtualProtect((LPVOID)0x63A8F8, 1024, PAGE_EXECUTE_READWRITE, &oldProtection);
		//VirtualProtect((LPVOID)0x63A8E4, 1024, PAGE_EXECUTE_READWRITE, &oldProtection);
		//VirtualProtect((LPVOID)0x63A8F4, 1024, PAGE_EXECUTE_READWRITE, &oldProtection);
		//VirtualProtect((LPVOID)0x63A8DC, 1024, PAGE_EXECUTE_READWRITE, NULL);
		//VirtualProtect((LPVOID)0x63A8EC, 1024, PAGE_EXECUTE_READWRITE, NULL);

		//HINSTANCE hModuleEngine = LoadLibraryA("engine.dll");

		//0x63A8F8
		//VirtualProtect((LPVOID)hModuleEngine, sizeof(hModuleEngine), PAGE_EXECUTE_READWRITE, NULL);
		//	//dllFunction = (testFunction)GetProcAddress(hModuleEngine, "DllEntryPoint");

		//KIniFile Ini;
		//KIniFile gIniFile;
		//gIniFile = (KIniFile*)GetProcAddress(hModuleEngine, "??0KIniFile@@QAE@XZ");
		//int(*pM1)() = (KIniFile(*)()) ::GetProcAddress(hLib, "c_M1");
		//Ini("\\ui\\ui3\\SkillState.ini");
		//void (*KIniFile)(char*) = ((void(*)(char*))GetProcAddress(hModuleEngine, "??0KIniFile@@QAE@XZ"));
		//VirtualProtect((LPVOID)Ini, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//KIniFile_Load KIniFile_Load = ((KIniFile_Load)GetProcAddress(hModuleEngine, "Load@KIniFile@@QAEHPBD@Z"));
		//bool (*KIniFile_Load)(KIniFile* This, LPCSTR Data) = ((bool(*)(KIniFile * This, LPCSTR Data))GetProcAddress(hModuleEngine, "Load@KIniFile@@QAEHPBD@Z"));
		//VirtualProtect((LPVOID)KIniFile_Load, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//int (*KIniFile_GetInteger)(KIniFile* This, const char* a1, const char* a2, int a3, int* a4) = ((int(*)(KIniFile * This, const char* a1, const char* a2, int a3, int* a4))GetProcAddress(hModuleEngine, "GetInteger@KIniFile@@QAEHPBD0HPAH@Z"));
		//VirtualProtect((LPVOID)KIniFile_GetInteger, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//int (*KIniFile_GetString)(KIniFile* This, const char* a1, const char* a2, const char* a3, char* a4, int a5) = ((int(*)(KIniFile * This, const char* a1, const char* a2, const char* a3, char* a4, int a5))GetProcAddress(hModuleEngine, "GetString@KIniFile@@QAEHPBD00PADK@Z"));
		//VirtualProtect((LPVOID)KIniFile_GetString, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//void (*KIniFile_Clean)(KIniFile*) = ((void(*)(KIniFile*))GetProcAddress(hModuleEngine, "1KIniFile@@QAE@XZ"));
		//VirtualProtect((LPVOID)KIniFile_Clean, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//
		//void (*KIniFile)(char*) = ((void(*)(char*)) 0x63A8F8);
		////VirtualProtect((LPVOID)KIniFile, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//bool (*KIniFile_Load)(char* This, const char* Data) = ((bool(*)(char* This, const char* Data))0x63A8E4);
		////VirtualProtect((LPVOID)KIniFile_Load, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//int (*KIniFile_GetInteger)(char* This, const char* a1, const char* a2, int a3, int* a4) = ((int(*)(char* This, const char* a1, const char* a2, int a3, int* a4))0x63A8F4);
		////VirtualProtect((LPVOID)KIniFile_GetInteger, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//int (*KIniFile_GetString)(char* This, const char* a1, const char* a2, const char* a3, char* a4, int a5) = ((int(*)(char* This, const char* a1, const char* a2, const char* a3, char* a4, int a5))0x63A8E0);
		////VirtualProtect((LPVOID)KIniFile_GetString, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//void (*KIniFile_Clean)(char*) = ((void(*)(char*))0x63A8DC);
		////VirtualProtect((LPVOID)KIniFile_Clean, 2000u, PAGE_EXECUTE_READWRITE, NULL);
		//VirtualClass* myClass = new VirtualClass();
	//KIniFile IniFile;
		//char Ini[88];
		//DWORD oldProtection;
		//VirtualProtect((LPVOID)0x63A8F8, 4, PAGE_EXECUTE_READWRITE, &oldProtection);
		//b_KIniFile(Ini);
		//VirtualProtect((LPVOID)0x63A8F8, 4, oldProtection, 0);

		//VirtualProtect((LPVOID)0x63A8F4, 4, PAGE_EXECUTE_READWRITE, &oldProtection);
		//b_KLoad(Ini, "\\ui\\ui3\\����״̬�б�.ini");
		//VirtualProtect((LPVOID)0x63A8F4, 4, oldProtection, 0);

	//	HMODULE hModuleEngine = LoadLibraryA("engine.dll");

	//	//void (*EntryProc)() = (void(*)())GetProcAddress(hModuleEngine, "KIniFile");
	//	KIniFile_Entry = (_KIniFile)(DWORD)GetProcAddress(hModuleEngine, "??0KIniFile@@QAE@XZ");
	//	VirtualProtect((LPVOID)KIniFile_Entry, 6, PAGE_EXECUTE_READWRITE, &oldProtection);
	//	//KIniFile test;
	///*	ofstream myfile;
	//	myfile.open("E:\\example.txt");
	//	myfile << (_DWORD)KIniFile_Entry << "\n";
	//	myfile.close();*/

	//	KIniFile_Load = (_KIniFile_Load)(DWORD)GetProcAddress(hModuleEngine, "?Load@KIniFile@@QAEHPBD@Z");
	//	VirtualProtect((LPVOID)KIniFile_Load, 6, PAGE_EXECUTE_READWRITE, &oldProtection);

	//	KIniFile_GetInteger = (_KIniFile_GetInteger)(DWORD)GetProcAddress(hModuleEngine, "?GetInteger@KIniFile@@QAEHPBD0HPAH@Z");
	//	VirtualProtect((LPVOID)KIniFile_GetInteger, 6, PAGE_EXECUTE_READWRITE, &oldProtection);

	//	KIniFile_GetString = (_KIniFile_GetString)(DWORD)GetProcAddress(hModuleEngine, "?GetString@KIniFile@@QAEHPBD00PADK@Z");
	//	VirtualProtect((LPVOID)KIniFile_GetString, 6, PAGE_EXECUTE_READWRITE, &oldProtection);

	//	KIniFile_Clean = (_KIniFile_Clean)(DWORD)GetProcAddress(hModuleEngine, "??1KIniFile@@QAE@XZ");
	//	VirtualProtect((LPVOID)KIniFile_Clean, 6, PAGE_EXECUTE_READWRITE, &oldProtection);

	//	//void** vTablePtr = *reinterpret_cast<void***>(myClass);

	//	ofstream myfile1;
	//	myfile1.open("E:\\example.txt");
	//	myfile1 << (_DWORD)KIniFile_Entry << "\n";
	//	myfile1 << (_DWORD)KIniFile_Load << "\n";
	//	myfile1 << (_DWORD)KIniFile_GetInteger << "\n";
	//	myfile1 << (_DWORD)KIniFile_GetString << "\n";
	//	myfile1 << (_DWORD)KIniFile_Clean << "\n";
	//	myfile1.close();

		//int IsOpenl;
		//char txtImage[512];
		////_KIniFile* Ini = new _KIniFile;
		//////ofstream myfile2;
		////myfile2.open("E:\\example1.txt");
		////myfile2 << (void*)Ini << "\n";
		////myfile2.close();
		////_KIniFile(Ini);
		//char		Buff[128];
		//KIniFile	Ini;
		//sprintf(Buff, "\\Ui\\ui3\\%s", SCHEME_INI_MINI);
		//Ini.Load(Buff);
		//Ini.GetInteger("Main", "Moveable", 0, &IsOpenl);
		//Ini.GetString("Main", "Image", (char*)0x63ABB8, txtImage, 512);
		////b_KIniFile(&IniFile);
		//////KIniFile_Load(Ini, "\\Ui\\ui3\\����״̬�б�.ini");

		////ofstream myfile1;
		////myfile1.open("E:\\example1.txt");
		////myfile1 << (_DWORD)&IniFile << "\n";
		////myfile1.close();

		////b_KLoad(&IniFile, "\\Ui\\ui3\\����״̬�б�.ini");
		//////b_GetInteger(Ini, "Main", "IsOpen", 0, &IsOpenl);
		// 
		//ofstream myfile2;
		//myfile2.open("E:\\example2.txt");
		//myfile2 << Buff << "\n";
		//myfile2 << txtImage << "\n";
		//myfile2 << IsOpenl << "\n";
		//myfile2.close();

		//VirtualProtect((LPVOID)KIniFile_Entry, 6, oldProtection, 0);
		//VirtualProtect((LPVOID)KIniFile_Load, 6, oldProtection, 0);
		//VirtualProtect((LPVOID)KIniFile_GetInteger, 6, oldProtection, 0);
		//VirtualProtect((LPVOID)KIniFile_GetString, 6, oldProtection, 0);
		//VirtualProtect((LPVOID)KIniFile_Clean, 6, oldProtection, 0);

		//b_GetInteger(Ini, "Main", "IsOpen", 0, &IsOpenl);
		//KIniFile_GetInteger(Ini, "Main", "IsOpen", 0, &IsOpenl);
		//VirtualProtect((LPVOID)0x63A8F8, 1024, oldProtection, 0);
		//VirtualProtect((LPVOID)0x63A8E4, 1024, oldProtection, 0);
		//VirtualProtect((LPVOID)0x63A8F4, 1024, oldProtection, 0);
		//VirtualProtect((LPVOID)0x63A8DC, 1024, PAGE_EXECUTE_READWRITE, NULL);
		//VirtualProtect((LPVOID)0x63A8EC, 1024, PAGE_EXECUTE_READWRITE, NULL);
		// 
		//KIniFile	pgIni;
	//HMODULE hModule = LoadLibraryA("engine.dll");

	////void (*EntryProc)(char*) = ((void(*)(char*))GetProcAddress(hModule, "EntryProc"));
	//void (*EntryProc)() = (void(*)())GetProcAddress(hModule, "EntryProc");

	//if (EntryProc)
	//{
	//	//HANDLE ThreadHandles1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryProc, 0, 0, NULL);
	//	//choose one
	//	EntryProc();
	//	//EntryProc("SS2");
	//}

	//char Offset[100];
	//DWORD oldProtect;
	//HMODULE hModule = LoadLibraryA("Client.dll");

	////uintptr_t serverdllBaseAddress = GetModuleBaseAddress(GetProcessId(v1), "engine.dll");

	////HMODULE hModuleEngine = (HMODULE)serverdllBaseAddress;
	//hModuleEngine = hModule;
	//VirtualProtect((LPVOID)hModuleEngine, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

	//void (*EntryProc)() = (void(*)())GetProcAddress(hModuleEngine, "DllMain");

	//if (EntryProc)
	//{

	//	EntryProc();

	//	KIniFile_Entry = (DWORD)GetProcAddress(hModuleEngine, "??0KIniFile@@QAE@XZ");
	//	//VirtualProtect((LPVOID)Ex_KIniFile_Entry, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

	//	KIniFile_Load = (DWORD)GetProcAddress(hModuleEngine, "?Load@KIniFile@@QAEHPBD@Z");
	//	//VirtualProtect((LPVOID)Ex_KIniFile_Load, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

	//	KIniFile_GetInteger = (DWORD)GetProcAddress(hModuleEngine, "?GetInteger@KIniFile@@QAEHPBD0HPAH@Z");
	//	//VirtualProtect((LPVOID)Ex_KIniFile_GetInteger, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

	//	KIniFile_GetString = (DWORD)GetProcAddress(hModuleEngine, "?GetString@KIniFile@@QAEHPBD00PADK@Z");
	//	//VirtualProtect((LPVOID)Ex_KIniFile_GetString, 6, PAGE_EXECUTE_READWRITE, &oldProtect);

	//	KIniFile_Clean = (DWORD)GetProcAddress(hModuleEngine, "??1KIniFile@@QAE@XZ");
	//	//VirtualProtect((LPVOID)Ex_KIniFile_Clean, 6, PAGE_EXECUTE_READWRITE, &oldProtect);
	//}

	//char Buffer[132];
	//int v4;
	//KIniFile Ini;

	//Ini.Load("\\ui\\ui3\\SkillState.ini");

	//Ini.GetInteger("Main", "IsOpen", 0, &v4);

	//ofstream myfile1;
	//myfile1.open("E:\\example1.txt");
	////myfile1 << (void*)serverdllBaseAddress << "\n";
	////myfile1 << (DWORD)serverdllBaseAddress << "\n";
	////myfile1 << (void*)hModule << "\n";
	////myfile1 << (DWORD)hModule << "\n";
	//myfile1 << v4 << "\n";
	////myfile1 << (DWORD)hModuleEngine << "\n";
	//myfile1 << (void*)KIniFile_Entry << "\n";
	//myfile1 << (void*)KIniFile_Load << "\n";
	//myfile1 << (void*)KIniFile_GetInteger << "\n";
	//myfile1 << (void*)KIniFile_GetString << "\n";
	//myfile1 << (void*)KIniFile_Clean << "\n";
	//myfile1.close();
	 
	//sprintf(Offset, "%x %d - %x %d", (void*)hModule, *(DWORD*)hModule, (void*)serverdllBaseAddress, *(DWORD*)serverdllBaseAddress);

	//MessageBox(0, Offset, "Testing", MB_OK);

	//void (*EntryProc)(char*) = ((void(*)(char*))GetProcAddress(hModule, "EntryProc"));
	//void (*EntryProc)() = (void(*)())GetProcAddress(hModule, "EntryProc");

	//if (EntryProc)
	//{
	//	//HANDLE ThreadHandles1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryProc, 0, 0, NULL);
	//	//choose one
	//	EntryProc();
	//	//EntryProc("SS2");
	//}

	//WaitForMultipleObjects(3,ThreadHandles,1,2000);

	//gMacroCheck.Init(hins);

	//gThreadCheck.Init();

	//gFileProtect.Init();

	/*while (true)
	{
		Sleep(300);
		if (gProtect.m_IsConnectedAuth)
			break;
	}*/

//#if (ICONSKILL==1)
//	DynamicLib(hWnd);
//#endif
	/*while (!DelayMe(5000, 1))
	{
		if (gConnection.CheckState() != 0)
			break;
	}*/
	
	//if (gAuthenticate.m_SplashScreen == 1)
	//{
	//	SplashInit.CloseSplash();
	//	//CreateThread(0, 0, (LPTHREAD_START_ROUTINE)&CheckClassGame, 0, 0, 0);
	//}
//#endif
	//HINSTANCE hModuleEngine = LoadLibraryA("engine.dll");
	////dllFunction = (testFunction)GetProcAddress(hModuleEngine, "DllEntryPoint");
	//Ex_KIniFile_Entry = (_KIniFile)GetProcAddress(hModuleEngine, "0KIniFile@@QAE@XZ");
	//Ex_KIniFile_Load = (_KIniFile_Load)GetProcAddress(hModuleEngine, "Load@KIniFile@@QAEHPBD@Z");
	//Ex_KIniFile_GetInteger = (_KIniFile_GetInteger)GetProcAddress(hModuleEngine, "GetInteger@KIniFile@@QAEHPBD0HPAH@Z");
	//Ex_KIniFile_GetString = (_KIniFile_GetString)GetProcAddress(hModuleEngine, "GetString@KIniFile@@QAEHPBD00PADK@Z");
	//Ex_KIniFile_Clean = (_KIniFile_Clean)GetProcAddress(hModuleEngine, "1KIniFile@@QAE@XZ");
	//
	///*KIniFile_Entry = (DWORD)&Module_KIniFile;
	//KIniFile_Load = (DWORD)&Module_KIniFile_load;
	//KIniFile_GetInteger = (DWORD)&Module_KIniFile_GetInteger;
	//KIniFile_GetString = (DWORD)&Module_KIniFile_GetString;
	//KIniFile_Clean = (DWORD)&Module_KIniFile_EndKIniFile;*/

	//ofstream myfile;
	//myfile.open("E:\\example0.txt");
	////myfile << (DWORD)dllFunction << "\n";
	//myfile << (DWORD*)&Ex_KIniFile_Entry << "\n";
	//myfile << (DWORD*)&Ex_KIniFile_Load << "\n";
	//myfile << (DWORD*)&Ex_KIniFile_GetInteger << "\n";
	//myfile << (DWORD*)&Ex_KIniFile_GetString << "\n";
	//myfile << (DWORD*)&Ex_KIniFile_Clean << "\n";
	//myfile.close();

	//dllFunction(hModuleEngine, 1, 0);

	//void (*EntryProc)(char*) = ((void(*)(char*))GetProcAddress(hModule, "EntryProc"));
	//void (*EntryProc)() = (void(*)())GetProcAddress(hModuleEngine, "KIniFile");

	//if (EntryProc)
	//{
	//	//HANDLE ThreadHandles1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryProc, 0, 0, NULL);
	//	//choose one
	//	EntryProc();
	//	//EntryProc("SS2");
	//}

	/*else
		gConnection.Disconnect();*/

		//gProtect.CheckPluginFile();

		//gLog.Output(LOG_DEBUG,GetEncryptedString(44));
}

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) // OK
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		hins = (HINSTANCE)hModule;
		//g_UiSkillState.hins = hins;
		//LIBRARY_LOAD_ATTACH();
		//MEMORY_CHECK_ATTACH();
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}

	return 1;
}

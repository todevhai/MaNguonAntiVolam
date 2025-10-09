#include "stdafx.h"
#include "HackServerProtocol.h"
#include "Connection.h"
#include "ListManager.h"
#include "Log.h"
#include "Message.h"
#include "Protect.h"
#include "Util.h"
#include "KPlayerSafe.h"
#include "INIReader.h"
#include "SkillState.h"
#include "SimpleModules.h"
//#include "KLuaScript.h"
//#include "NewUiShell.h"
//#include <fstream>

using namespace std;

DWORD gConnectionStatusTime = 0;
DWORD gMaxHardwareIdConnection = 0;
DWORD gEncDecKey1 = 0;
DWORD gEncDecKey2 = 0;
DWORD gClientInfoOK = 0;
DWORD gDumpListOK = 0;
DWORD gChecksumListOK = 0;
DWORD gInternalListOK = 0;
DWORD gWindowListOK = 0;
DWORD gClassListOK = 0;
DWORD gDumpListMaxCount = 0;
DWORD gChecksumListMaxCount = 0;
DWORD gInternalListMaxCount = 0;
DWORD gWindowListMaxCount = 0;
DWORD gClassListMaxCount = 0;
DWORD gMemoryGuardSwitch = 0;
DWORD gMemoryGuardNumber = 0;
DWORD gMacroProtectSwitch = 0;
DWORD gMacroProtectNumber = 0;
DWORD gReconnectSwitch = 0;
DWORD gReconnectStatus = 0;
DWORD gDetectCloseTime = 0;
DWORD gInternalVersion = 0;
DWORD gUserAccount = 0;
DWORD gUserStruct = 0;
DWORD gWindowHwnd = 0;
DWORD gIpAddressAddress = 0;
DWORD gClientVersionAddress = 0;
DWORD gClientSerialAddress = 0;
DWORD gFileProtectAddress1 = 0;
DWORD gFileProtectAddress2 = 0;
char gIpAddress[32];
BYTE gClientVersion[5];
BYTE gClientSerial[17];
DWORD gAntiDump;
DWORD gThanPhap;
DWORD gWorldRank;
DWORD gProtocolVersion;
DWORD gLimit;
DWORD gStateSkill;
DWORD gFakeUserBattles;
DWORD gIPProtect;
DWORD gPortProtect;
DWORD gProcessListMaxCount = 0;
DWORD gProcessListOK = 0;

void HackServerProtocolCore(BYTE head,BYTE* lpMsg,int size) // OK
{
	switch(head)
	{
		case 0x00:
			HCClientInfoRecv((SDHP_CLIENT_INFO_RECV*)lpMsg);
			break;
		case 0x01:
			HCConnectionStatusRecv((SDHP_CONNECTION_STATUS_RECV*)lpMsg);
			break;
		case 0x02:
			switch(lpMsg[5])
			{
				case 0x00:
					HCDumpListRecv((SDHP_DUMP_LIST_RECV*)lpMsg);
					break;
				case 0x01:
					HCChecksumListRecv((SDHP_CHECKSUM_LIST_RECV*)lpMsg);
					break;
				case 0x02:
					HCInternalListRecv((SDHP_INTERNAL_LIST_RECV*)lpMsg);
					break;
				case 0x03:
					HCWindowListRecv((SDHP_WINDOW_LIST_RECV*)lpMsg);
					break;
			}
			break;
		case 0x03:
			HCClientDisconnectRecv((SDHP_CLIENT_DISCONNECT_RECV*)lpMsg);
			break;
		case 0x05:
			HCClientSelectServerRecv((SDHP_CLIENT_SELECT_SERVER_RECV*)lpMsg);
			break;
		case 0x06:
			switch (lpMsg[5])
			{
			case 0x00:
				HCClientInfoRecv((SDHP_CLIENT_INFO_RECV*)lpMsg);
				break;
			case 0x01:
				HCChecksumListRecv((SDHP_CHECKSUM_LIST_RECV*)lpMsg);
				break;
			case 0x02:
				HCInternalListRecv((SDHP_INTERNAL_LIST_RECV*)lpMsg);
				break;
			case 0x03:
				HCWindowListRecv((SDHP_WINDOW_LIST_RECV*)lpMsg);
				break;
			}
			break;
		
		default:
			gLog.Output(LOG_DEBUG,GetEncryptedString(25),head,size);
			gConnection.Disconnect();
			break;
	}
}

void HCClientSelectServerRecv(SDHP_CLIENT_SELECT_SERVER_RECV* lpMsg) // OK
{

	gPlayerSafe.m_WaitForRespon = true;

	if (lpMsg->result == 0)
	{
		if (!CheckInGame())
		{
			//b_sub_539C30_Close(gPlayerSafe.m_ConnectionID);
			memcpy(gProtect.m_MainInfo.ClientSerial, lpMsg->ClientSerial, sizeof(gProtect.m_MainInfo.ClientSerial));
		}
		//else
		//{
		//	//INIReader reader("AntiFwcld.ini");
		//	//SetByte(0x52B9F9, 0x1);
		//	//gProtocolVersion = 0x1;
		//	memcpy(gProtect.m_MainInfo.ClientSerial, lpMsg->ClientSerial, sizeof(gProtect.m_MainInfo.ClientSerial));
		//}
	}
	else
	{

		if (CheckInGame())
		{
			SafeExitProcess();
			//b_sub_539C30_Close(0);
			//b_sub_41B640((_DWORD**)0x68D090, (BYTE*)"ExitGame()");
		}
		else
			memset(gProtect.m_MainInfo.ClientSerial, 0, sizeof(gProtect.m_MainInfo.ClientSerial));

	}
}

void HCClientInfoRecv(SDHP_CLIENT_INFO_RECV* lpMsg) // OK
{
	//gLog.Output(LOG_DEBUG,GetEncryptedString(26),lpMsg->result,lpMsg->EncDecKey1,lpMsg->EncDecKey2,lpMsg->InternalVersion,lpMsg->ReconnectSwitch,lpMsg->MemoryGuardSwitch,lpMsg->MemoryGuardNumber,lpMsg->MacroProtectSwitch,lpMsg->MacroProtectNumber,lpMsg->MemoryAddress1,lpMsg->MemoryAddress2,lpMsg->MemoryAddress3,lpMsg->MemoryAddress4,lpMsg->MemoryAddress5,lpMsg->MemoryAddress6,lpMsg->MemoryAddress7,lpMsg->MemoryAddress8);

	if(lpMsg->result == 0)
	{
		gClientInfoOK = 1;

		gConnectionStatusTime = GetTickCount();

		if (gReconnectStatus == 0)
		{
			gMaxHardwareIdConnection = lpMsg->MaxHardwareIdConnection;

			gAntiDump = lpMsg->nAntiDump;

			gThanPhap = lpMsg->nThanPhap;

			//gWorldRank = lpMsg->nWorldRank;

			gProtocolVersion = lpMsg->nProtocolVersion;

			//gLimit = lpMsg->nLimit;			

			if (gAntiDump == 1)
			{
				gProtect.m_IsConnectedAuth = true;

				gProtect.m_MainInfo.nLimit = lpMsg->nLimit;

				gProtect.m_CheckLimit = lpMsg->m_CheckLimit;

				gProtect.m_CheckVPN = lpMsg->m_CheckVPN;

				gProtect.m_CheckProcess = lpMsg->m_CheckProcess;

				gProtect.m_CheckCount = lpMsg->m_CheckCount;

				//int nCurProtocolVersion = *(BYTE*)lpMsg->MemoryAddress1;

				//gProtocolVersion = (nCurProtocolVersion + lpMsg->nProtocolVersion);

				//SetByte(lpMsg->MemoryAddress1, gProtocolVersion);

				//SetCompleteHook(0xE8, 0x54568D, &cPrintLoading);

				//MemorySet(0x545692, 0x90, 1);

				//SetCompleteHook(0xFF, 0x528E0A, &NewData); // 0x71E14FB0

				//SetCompleteHook(0xFF, 0x52BA17, &hook_sub_528F40); // 0x71E14FB0

				//SetCompleteHook(0xFF, 0x63A7EC, &hook_StringToMD5String); // 0x71E14FB0

				if (gThanPhap == 1)
				{
					SetCompleteHook(0xE9, lpMsg->MemoryAddress2, &hook_sub_5770F1);
				}
				else if (gThanPhap == 2)
				{
					SetCompleteHook(0xE8, lpMsg->MemoryAddress3, &hook_sub_5770F1);
				}

				//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&StartModule, NULL, 0, NULL);
				//SetByte(0x52B9F9, gProtocolVersion); //v6
				//INIReader reader("AntiFwcld.ini");
				//SetByte(0x53E3E9, 1); //v8
				
				if (lpMsg->m_FakeUserTK == 1)
				{
					//SetCompleteHook(0xE8, 0x4F2D7C, &update_songuoi_tongkim); // cong them so nguoi trong tong kim
					SetCompleteHook(0xE8, 0x4045E3, &update_songuoi_tongkim); // cong them so nguoi trong tong kim
					SetCompleteHook(0xE8, 0x4F2D7C, &thamgia_tongkim); // cong them so nguoi trong tong kim
				}

				//SetCompleteHook(0xFF, 0x52BA17, &hook_sub_528F40);
				// 
				//DWORD OldProtect;

				//VirtualProtect((void*)0x52BA17, 2000, PAGE_EXECUTE_READWRITE, &OldProtect);
				//SetCompleteHook(0xFF, 0x52BA17, &hook_sub_528F40); // 0x71E14FB0

				//if (gLimit > 1)
				//{
				//	//std::wstringstream ss;
				//	//ss << nGameLimit;
				//	//std::string message = std::to_string(nGameLimit);
				//	//MessageBox(hwnd, buf, "Testing", MB_OK);
				//	//MessageBox(0, message.c_str(), "Error", MB_OK | MB_ICONINFORMATION);
				//	//CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&CheckLimit, NULL, 0, NULL);

				//	//return;
				//}

				if (lpMsg->m_StateSkill > 0)
				{

					//gProtect.m_StateSkill = lpMsg->m_StateSkill;

					//SetDword(0x55F2B6, (DWORD)&exec_sub_556590); // v6

					//SetCompleteHook(0xE8, 0x5565B4, &DetectSynSkill); // v6

					//SetCompleteHook(0xFF, 0x585D7A, &safe_sub_585D30); // v6

					//SetCompleteHook(0xE8, 0x406140, &safe_sub_406230); // v6

					//SetCompleteHook(0xE8, 0x494F92, &safe_sub_42F510); // v6

					//SetCompleteHook(0xE8, 0x4063FA, &safe_sub_539090); // v6

					//SetCompleteHook(0xE8, 0x530188, &safe_sub_52CB70); // v6

					SetCompleteHook(0xE8, 0x459A7B, &InGame); // v6

					//SetCompleteHook(0xE8, 0x5565B4, &DetectSynSkill); // v6

					//SetCompleteHook(0xE8, 0x527227, &NetConnectAgentInitialize); // v6

					//SetCompleteHook(0xE8, 0x52729F, &StartGame); // v6
					 
					//SetCompleteHook(0xE8, 0x420805, &HookUiStart); // UiStart // 0x526F6D

					//SetCompleteHook(0xE8, 0x4265A4, &HookOnLBtnDown); // OnLBtnDown single click // 0x526F6D

					//SetCompleteHook(0xE8, 0x426964, &HookOnLBtnDown); // OnLBtnDown // 0x526F6D

					//SetCompleteHook(0xE8, 0x426C1D, &HookOnButtonClick); // OnButtonClick // 0x526F6D

					//SetCompleteHook(0xE8, 0x426D11, &HookOnButtonClick); // OnButtonClick // 0x526F6D

					//SetCompleteHook(0xE8, 0x426CF9, &HookOnButtonClick); // OnButtonClick // 0x526F6D

					//SetCompleteHook(0xE8, 0x426CBB, &HookOnButtonClick); // OnButtonClick // 0x526F6D

					//SetCompleteHook(0xE8, 0x426CA3, &HookOnButtonClick); // OnButtonClick // 0x526F6D

					//SetCompleteHook(0xFF, 0x426D21, &HookOnButtonClick); // OnButtonClick // 0x526F6D

					//SetCompleteHook(0xE8, 0x420C02, &HookUiArenaIcon); // OnButtonClick

					//SetCompleteHook(0xE8, 0x41A0DE, &HookLoadSchemeForEachWnd); // OnButtonClick

					/*DWORD oldProtection1, oldProtection2, oldProtection3, oldProtection4;

					VirtualProtect((LPVOID)0x4265A4, 4, PAGE_EXECUTE_READWRITE, &oldProtection1);
					VirtualProtect((LPVOID)0x426964, 4, PAGE_EXECUTE_READWRITE, &oldProtection2);
					VirtualProtect((LPVOID)0x420BC6, 4, PAGE_EXECUTE_READWRITE, &oldProtection3);
					VirtualProtect((LPVOID)0x42C596, 2000, PAGE_EXECUTE_READWRITE, &oldProtection4);

					VirtualProtect((LPVOID)0x4265A4, 4, oldProtection1, 0);
					VirtualProtect((LPVOID)0x426964, 4, oldProtection2, 0);
					VirtualProtect((LPVOID)0x420BC6, 4, oldProtection3, 0);
					VirtualProtect((LPVOID)0x42C596, 2000, oldProtection4, 0);*/
	 
					//SetCompleteHook(0xE8, 0x43E318, &HookAddOneWindow); // HookAddOneWindow // 0x526F6D

					//SetCompleteHook(0xE8, 0x420697, &HookRegisterComClass); // kRegisterComClass InUse // 0x420697

					//SetCompleteHook(0xE8, 0x420A09, &HookUiGameMain); // InUse

					//SetDword(0x6413BC, (DWORD)&HookWindowWndProc); // v6

					//SetDword(0x63E0AC, (DWORD)&HookWindowWndProc); // KUiToolsControlBar

					//SetDword(0x646414, (DWORD)&HookButtonWndProc); // KUiArena InUse
					 
					//SetCompleteHook(0xE8, 0x465D0E, &HookUiInitProc); // v6

					//SetCompleteHook(0xE8, 0x4264D9, &HookWindowWndProc); // v6

					//SetCompleteHook(0xE8, 0x4267EB, &HookWindowWndProc); // v6

					//SetCompleteHook(0xE8, 0x4264D9, &HookWindowWndProc); // v6

					//SetCompleteHook(0xE8, 0x427894, &HookButtonWndProc); // v6
					 
					//SetDword(0x6461E4, (DWORD)&HookButtonWndProc); // KUiActivityGuide

					//SetCompleteHook(0xE8, 0x442A08, &HookOnLBtnDown); // Button WM_LBUTTONDOWN

					//SetCompleteHook(0xE8, 0x4A57BD, &HookButtonWndProc); // v6

					SetCompleteHook(0xE8, 0x52718B, &SetRepresentShell); // Render 2  sub_420820();

					SetCompleteHook(0xE8, 0x420866, &Main_RenderWindows); // Render 2  sub_420820();

					SetCompleteHook(0xE8, 0x420FB4, &CloseAllWindowOutGame); // Render 2  sub_420820();

					//SetCompleteHook(0xE8, 0x44E4BB, &HookChatCheck); // Render 2  sub_420820();
				}

				memcpy(gProtect.m_MainInfo.ClientSerial, lpMsg->AllowCode, sizeof(gProtect.m_MainInfo.ClientSerial));
				

				/*if (lpMsg->m_Window1024[0] == 1)
				{
					SetWord(lpMsg->m_Window1024[1], 0x300);
					SetWord(lpMsg->m_Window1024[2], 0x400);
					SetWord(lpMsg->m_Window1024[3], 0x300);
					SetWord(lpMsg->m_Window1024[4], 0x400);
					SetWord(lpMsg->m_Window1024[5], 0x300);
					SetWord(lpMsg->m_Window1024[6], 0x400);
				}*/

				//MessageBox(0, "Icon OK", "Error", MB_OK | MB_ICONINFORMATION);
				//LIBRARY_LOAD_ATTACH();
				//MEMORY_CHECK_ATTACH();
			}
			else
			{
				SetByte(lpMsg->MemoryAddress1, 65);
			}
		}
	}
	else
	{
		SetByte(lpMsg->MemoryAddress1, 65);
		gProtocolVersion = 0;
		lpMsg->MemoryAddress2 = 0;
		lpMsg->MemoryAddress3 = 0;

		switch(lpMsg->result)
		{
			case 1:
				SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(3), 5000);
				break;
			case 2:
				SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(2), 5000);
				break;
			case 3:
				SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(2), 5000);
				break;
			default:
				SplashScreen(&SplashError, 2, 1, gMessage.GetMessage(1), 5000);
				break;
		}
	}
}

void HCConnectionStatusRecv(SDHP_CONNECTION_STATUS_RECV* lpMsg) // OK
{
	gConnectionStatusTime = GetTickCount();
}

void HCDumpListRecv(SDHP_DUMP_LIST_RECV* lpMsg) // OK
{
	//gLog.Output(LOG_DEBUG,GetEncryptedString(27),lpMsg->count,lpMsg->MaxCount);

	gDumpListMaxCount = lpMsg->MaxCount;

	for(int n=0;n < lpMsg->count;n++)
	{
		DUMP_LIST_INFO* lpInfo = (DUMP_LIST_INFO*)(((BYTE*)lpMsg)+sizeof(SDHP_DUMP_LIST_RECV)+(sizeof(DUMP_LIST_INFO)*n));

		gListManager.gDumpListInfo.push_back((*lpInfo));
	}

	gDumpListOK = 1;

}

void HCChecksumListRecv(SDHP_CHECKSUM_LIST_RECV* lpMsg) // OK
{
	//gLog.Output(LOG_DEBUG,GetEncryptedString(28),lpMsg->count,lpMsg->MaxCount);

	gChecksumListMaxCount = lpMsg->MaxCount;

	for(int n=0;n < lpMsg->count;n++)
	{
		CSUM_LIST_INFO* lpInfo = (CSUM_LIST_INFO*)(((BYTE*)lpMsg)+sizeof(SDHP_CHECKSUM_LIST_RECV)+(sizeof(CSUM_LIST_INFO)*n));

		gListManager.gChecksumListInfo.push_back((*lpInfo));
	}

	gChecksumListOK = 1;
}

void HCInternalListRecv(SDHP_INTERNAL_LIST_RECV* lpMsg) // OK
{
	//gLog.Output(LOG_DEBUG,GetEncryptedString(29),lpMsg->count,lpMsg->MaxCount);

	gInternalListMaxCount = lpMsg->MaxCount;

	for(int n=0;n < lpMsg->count;n++)
	{
		ITRN_LIST_INFO* lpInfo = (ITRN_LIST_INFO*)(((BYTE*)lpMsg)+sizeof(SDHP_INTERNAL_LIST_RECV)+(sizeof(ITRN_LIST_INFO)*n));

		gListManager.gInternalListInfo.push_back((*lpInfo));
	}

	gInternalListOK = 1;
}

void HCWindowListRecv(SDHP_WINDOW_LIST_RECV* lpMsg) // OK
{
	//gLog.Output(LOG_DEBUG,GetEncryptedString(43),lpMsg->count,lpMsg->MaxCount);

	/*gWindowListMaxCount = lpMsg->MaxCount;

	for(int n=0;n < lpMsg->count;n++)
	{
		WNDW_LIST_INFO* lpInfo = (WNDW_LIST_INFO*)(((BYTE*)lpMsg)+sizeof(SDHP_WINDOW_LIST_RECV)+(sizeof(WNDW_LIST_INFO)*n));

		gListManager.gWindowListInfo.push_back((*lpInfo));
	}

	gWindowListOK = 1;*/
}

void HCClientDisconnectRecv(SDHP_CLIENT_DISCONNECT_RECV* lpMsg) // OK
{
	//gLog.Output(LOG_DEBUG,GetEncryptedString(30),lpMsg->type);

	TCHAR buffer[256];

	switch(lpMsg->type)
	{
		case CLIENT_DISCONNECT_NONE:
			break;
		case CLIENT_DISCONNECT_DUMP_DETECTION:
			//SplashScreen(&SplashError,2,1,gMessage.GetMessage(14),5000);
			sprintf(buffer, TEXT("Ph¸t hiÖn phÒn mÒm kh«ng hîp lÖ [ %s ]"), gMessage.m_CaptionNameProcess);
			SplashScreen(&SplashError,2,1,buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_EXECUTABLE_DETECTION:
			sprintf(buffer, TEXT("Ph¸t hiÖn phÒn mÒm kh«ng hîp lÖ"), gMessage.m_CaptionNameProcess);
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_FILE_DETECTION:
			sprintf(buffer, TEXT("TiÖp game bÞ thay ®æi vui lßng cËp nhËt l¹i"), gMessage.m_CaptionNameProcess);
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_FILE_MAPPING_DETECTION:
			sprintf(buffer, TEXT("Ph¸t hiÖn can thiÖp hÖ bé nhí trß ch¬i"), gMessage.m_CaptionNameProcess);
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_LIBRARY_DETECTION:
			sprintf(buffer, TEXT("Ph¸t hiÖn can thiÖp hÖ bé nhí trß ch¬i"));
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_REGISTRY_DETECTION:
			sprintf(buffer, TEXT("Ph¸t hiÖn can thiÖp hÖ bé nhí trß ch¬i"));
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_MEMORY_DETECTION:
			sprintf(buffer, TEXT("Ph¸t hiÖn can thiÖp hÖ bé nhí trß ch¬i"));
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_WINDOW_DETECTION:
			sprintf(buffer, TEXT("Ph¸t hiÖn phÒn mÒm kh«ng hîp lÖ"));
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_MACRO_DETECTION:
			sprintf(buffer, TEXT("Ph¸t hiÖn can thiÖp hÖ bé nhí trß ch¬i"));
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		case CLIENT_DISCONNECT_DEBUGGER_DETECTION:
			sprintf(buffer, TEXT("Ph¸t hiÖn can thiÖp hÖ bé nhí trß ch¬i"));
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
		default:
			sprintf(buffer, TEXT("HÖ thèng gÆp sî cè vui lßng cËp nhËt l¹i"));
			SplashScreen(&SplashError,2,1, buffer,5000);
			//SafeExitProcess();
			break;
	}
}

void ExitGameSend() // OK
{

	//char	pSelectServer[65];

	//sprintf(pSelectServer, "%s-%s", GetHardwareId(), gProtect.m_IpAddressSelect);
	//sprintf(pSelectServer, "%s-%s", GetHardwareIdNow(), gProtect.m_IpAddressSelect);

	SDHP_REMOVE_SELECT_SERVER_SEND pMsg;

	pMsg.header.set(0x0B, sizeof(pMsg));

	pMsg.Version = 0;

	memcpy(pMsg.IpAddressSelect, gProtect.m_IpAddressSelect, sizeof(pMsg.IpAddressSelect));

	memcpy(pMsg.HardwareId, GetHardwareIdNow(), sizeof(pMsg.HardwareId));
	//memcpy(pMsg.IpAddressSelect, GetHardwareIdNow(), sizeof(pMsg.IpAddressSelect));

	gConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	//gProtect.m_IsInGame = false;

	//CloseHandle(GetCurrentThread());

	TerminateThread(gProtect.ThreadHandles[0], 0);

	CloseHandle(gProtect.ThreadHandles[0]);

	if (gProtect.m_CheckProcess == 1)
	{
		TerminateThread(gProtect.ThreadHandles[1], 0);
		CloseHandle(gProtect.ThreadHandles[1]);
	}
	
	if (gProtect.m_CheckLimit == 1)
	{
		TerminateThread(gProtect.ThreadHandles[2], 0);
		CloseHandle(gProtect.ThreadHandles[2]);
	}

}

void CHRemoveSelectServerSend() // OK
{

	//char	pSelectServer[65];

	//sprintf(pSelectServer, "%s-%s", GetHardwareId(), gProtect.m_IpAddressSelect);

	SDHP_REMOVE_SELECT_SERVER_SEND pMsg;

	pMsg.header.set(0x0A, sizeof(pMsg));

	//pMsg.Version = (int)CLIENT_VERSION;
	pMsg.Version = (int)gProtect.m_MainInfo.ClientVersion;

	memcpy(pMsg.IpAddressSelect, gProtect.m_IpAddressSelect, sizeof(pMsg.IpAddressSelect));

	memcpy(pMsg.HardwareId, GetHardwareIdNow(), sizeof(pMsg.HardwareId));

	gConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

	//gProtect.m_IsInGame = false;

	//CloseHandle(GetCurrentThread());

}

void CHClientSelectServerSend(int nProtocol) // OK
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

	//DWORD IPId = *(DWORD*)(&gProtect.m_IpAddressSelect[0x00]) ^ *(DWORD*)(&gProtect.m_IpAddressSelect[0x20]) ^ 0x68BBBB79;

	//char	pSelectServer[65];

	char m_IpAddress[16];

	//sprintf(pSelectServer, "%s-%s", GetHardwareId(), gProtect.m_IpAddressSelect);
	//sprintf(pSelectServer, "%s-%s", GetHardwareIdNow(), gProtect.m_IpAddressSelect);

	//sprintf(m_IpAddress, "%s", GetLocalIpAddressNew().c_str());

	SDHP_CLIENT_SELECT_SERVER_SEND pMsg;

	pMsg.header.set(0x05, sizeof(pMsg));

	pMsg.IsReconnect = gReconnectStatus;

	pMsg.ClientFileCRC = gProtect.m_ClientFileCRC;

	//pMsg.Version = (int)CLIENT_VERSION;
	pMsg.Version = (int)gProtect.m_MainInfo.ClientVersion;

	if (nProtocol == 2 || gProtect.m_IsInGame)
		pMsg.InGame = 1;
	else
		pMsg.InGame = (int)((gProtect.m_IsInGame) ? 1 : 0);

	//pMsg.InGame = CheckInGame();

	//memcpy(pMsg.IpAddressSelect, pSelectServer, sizeof(pMsg.IpAddressSelect));
	memcpy(pMsg.IpAddressSelect, gProtect.m_IpAddressSelect, sizeof(pMsg.IpAddressSelect));

	//memcpy(pMsg.IpAddressSelect, GetHardwareIdNow(), sizeof(pMsg.IpAddressSelect));

	pMsg.IpAddressSelect[33] = 0;
	//pMsg.IpAddressSelect[sizeof(pMsg.IpAddressSelect) - 1] = 0;

	memcpy(pMsg.IpAddress, m_IpAddress, sizeof(pMsg.IpAddress));

	pMsg.IpAddress[sizeof(pMsg.IpAddress) - 1] = 0;

	memcpy(pMsg.HackVersion, Anti-Cheat-Client_VERSION, sizeof(pMsg.HackVersion));

	//memcpy(pMsg.ClientSerial, gProtect.m_MainInfo.ClientSerial, sizeof(pMsg.ClientSerial));
	memset(pMsg.ClientSerial, 0, sizeof(pMsg.ClientSerial));

	//memcpy(pMsg.IpAddress, gProtect.m_IpAddress, sizeof(pMsg.IpAddress));

	//memcpy(pMsg.MacAddress, gProtect.m_MacAddress, sizeof(pMsg.MacAddress));

	memcpy(pMsg.HardwareId, GetHardwareIdNow(), sizeof(pMsg.HardwareId));

	//memcpy(pMsg.HardwareSerialId, GetHardwareId(), sizeof(pMsg.HardwareSerialId));

	gConnection.DataSend((BYTE*)&pMsg, pMsg.header.size);

}

void CHClientInfoSend() // OK
{

	//unsigned char mac[6] = { '\0' };
	//char cIpAddressID[8];
	//char cMacAddressID[8];
	//char textmix[100];
	//char MixMacAddress[50];
	////char MixIpAddress[16];
	//
	//memcpy(gProtect.m_SmallHardwareId, SmallHardwareId(), sizeof(gProtect.m_SmallHardwareId));

	//sprintf(gProtect.m_IpAddress, "%s", GetLocalIpAddressNew().c_str());

	//get_mac(mac, gProtect.m_IpAddress);

	//sprintf(gProtect.m_MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
	//
	////DWORD addr1 = inet_addr(gProtect.m_IpAddress);

	//DWORD IPId = *(DWORD*)(&gProtect.m_IpAddress[0x00]) ^ *(DWORD*)(&gProtect.m_IpAddress[0x16]) ^ 0x68BBBB79;

	//sprintf(MixMacAddress, "%08X%02X%02X%02X%02X%02X%s", IPId, mac[1], mac[2], mac[3], mac[4], mac[5], gProtect.m_SmallHardwareId);

	//std::string newhwid = licenseKeyFormatting(MixMacAddress, 5, 4);

	//memcpy(gProtect.m_HardwareId, newhwid.c_str(), sizeof(gProtect.m_HardwareId));

	// AABB-CCDD-ABCDE-ABCDE-EEFF-AABB
	// <-IPLAN-><MACNETWORK><DISKSERI>
	//sprintf(textmix, "%s - %08X - %s - %08X - %08X", newhwid.c_str(), IPId, gProtect.m_SmallHardwareId, addr1, addr2);
	//sprintf(textmix, "%s", newhwid.c_str());
	//MessageBox(0, textmix, "Testing", MB_OK);
	//DWORD IpAddressId = *(DWORD*)(&IpAddress[0x00]) ^ *(DWORD*)(&IpAddress[0x10]) ^ 0x3AD3B768;
	//sprintf(cIpAddressID, "%08X", IpAddressId);

	//DWORD MacAddressId = *(DWORD*)(&MacAddress[0x00]) ^ *(DWORD*)(&MacAddress[0x10]) ^ 0x3AD3B779;
	//sprintf(cMacAddressID, "%08X", MacAddressId);
	//MessageBox(0, IpAddress, "Testing", MB_OK);
	//MessageBox(0, MacAddress, "Testing", MB_OK);
	if(gReconnectStatus != 1)
	{
		gClientInfoOK = 0;

		gDumpListOK = 0;

		gChecksumListOK = 0;

		gInternalListOK = 0;

		gWindowListOK = 0;

		gDumpListMaxCount = 0;

		gChecksumListMaxCount = 0;

		gInternalListMaxCount = 0;

		gWindowListMaxCount = 0;

		gListManager.gDumpListInfo.clear();

		gListManager.gChecksumListInfo.clear();

		gListManager.gInternalListInfo.clear();

		gListManager.gWindowListInfo.clear();

		gListManager.gWindowListStringInfo.clear();
	}

	SDHP_CLIENT_INFO_SEND pMsg;

	pMsg.header.set(0x00,sizeof(pMsg));

	pMsg.IsReconnect = gReconnectStatus;

	pMsg.ClientFileCRC = gProtect.m_ClientFileCRC;

	//pMsg.Version = (int)CLIENT_VERSION;
	pMsg.Version = (int)gProtect.m_MainInfo.ClientVersion;

	memcpy(pMsg.HackVersion, Anti-Cheat-Client_VERSION,sizeof(pMsg.HackVersion));

	memcpy(pMsg.ClientSerial, gProtect.m_MainInfo.ClientSerial, sizeof(pMsg.ClientSerial));

#if (USE_LIMIT_AUTH_MULTI==0)

	memcpy(pMsg.IpAddress, gProtect.m_IpAddress, sizeof(pMsg.IpAddress));

	memcpy(pMsg.MacAddress, gProtect.m_MacAddress, sizeof(pMsg.MacAddress));

	memcpy(pMsg.HardwareId, gProtect.m_HardwareId, sizeof(pMsg.HardwareId));

#endif // (USE_LIMIT_AUTH_MULTI==0)

	memcpy(pMsg.HardwareSerialId, GetHardwareIdNow(), sizeof(pMsg.HardwareSerialId));
	//gLog.Output(LOG_DEBUG,GetEncryptedString(31),pMsg.IsReconnect,pMsg.ClientFileCRC,pMsg.HackVersion,pMsg.HardwareId);

	gConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);

	//if (gProtect.m_IsInGame)
	//	CHClientSelectServerSend();
}

void CHConnectionStatusSend() // OK
{
	SDHP_CONNECTION_STATUS_SEND pMsg;

	pMsg.header.set(0x01,sizeof(pMsg));

	gConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

void CHClientDisconnectSend(int type,char* text,DWORD pid) // OK
{
	if(gDetectCloseTime == 0)
	{
		gDetectCloseTime = GetTickCount();

		SDHP_CLIENT_DISCONNECT_RECV pMsg;

		pMsg.header.set(0x03,sizeof(pMsg));

		pMsg.type = type;

		if(gConnection.CheckState() == 0){HCClientDisconnectRecv(&pMsg);return;}
	}
	else
	{
		return;
	}

	SDHP_CLIENT_DISCONNECT_SEND pMsg;

	pMsg.header.set(0x03,sizeof(pMsg));

	pMsg.type = type;

	if(gUserAccount == 0)
	{
		memset(pMsg.account,0,sizeof(pMsg.account));
	}
	else
	{
		memcpy(pMsg.account,(void*)gUserAccount,sizeof(pMsg.account));
	}

	if(gUserStruct == 0 || (*(DWORD*)(gUserStruct)) == 0)
	{
		memset(pMsg.name,0,sizeof(pMsg.name));
	}
	else
	{
		memcpy(pMsg.name,(void*)(*(DWORD*)(gUserStruct)+0x00),sizeof(pMsg.name));
	}

	if(text == 0)
	{
		memset(pMsg.CaptionName,0,sizeof(pMsg.CaptionName));
	}
	else
	{
		memcpy(pMsg.CaptionName,text,sizeof(pMsg.CaptionName));
	}

	if(pid == 0)
	{
		memset(pMsg.ProcessName,0,sizeof(pMsg.ProcessName));
	}
	else
	{
		memset(pMsg.ProcessName,0,sizeof(pMsg.ProcessName));

		HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,0,pid);

		if(hProcess != 0)
		{
			wchar_t szProcessName[MAX_PATH];

			if(GetProcessImageFileNameW(hProcess,szProcessName,MAX_PATH) != 0)
			{
				wsprintf(pMsg.ProcessName,"%s",ConvertModuleFileName(szProcessName));
			}

			CloseHandle(hProcess);
		}
	}

	gLog.Output(LOG_DEBUG,GetEncryptedString(32),pMsg.type,pMsg.account,pMsg.name,pMsg.CaptionName,pMsg.ProcessName);

	gConnection.DataSend((BYTE*)&pMsg,pMsg.header.size);
}

int __stdcall hook_sub_5770F1() {;
	return 1;
}


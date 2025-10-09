#include "stdafx.h"
#include "ProcessCheck.h"
#include "HackServerProtocol.h"
#include "ListManager.h"
//Modules
#include "DumpCheck.h"
#include "ExecutableCheck.h"
#include "LibraryCheck.h"
#include "Message.h"

CProcessCheck gProcessCheck;

CProcessCheck::CProcessCheck()
{

}

CProcessCheck::~CProcessCheck()
{

}

bool CProcessCheck::CheckProcess(DWORD processID,DWORD flag)
{
	if((flag & 2) == 0 && (flag & 4) == 0 && (flag & 8) == 0)
	{
		return 1;
	}

	char CaptionName[64];
	TCHAR buffer[256];
	wchar_t szProcessName[MAX_PATH], sTemp[MAX_PATH];

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE, FALSE, processID);

	if (hProcess != NULL)
	{
		if(GetProcessImageFileNameW(hProcess,szProcessName,MAX_PATH) != 0)
		{
			if(ConvertProcessImageFileNameW(szProcessName,sTemp,MAX_PATH) != 0)
			{
				if (gProcessListOK == 1)
				{
					if (gDumpCheck.CheckProcessName(sTemp) == 0)
					{
						//strcpy(gMessage.m_CaptionNameProcess, CaptionName);
						TerminateProcess(hProcess, 1);
						CloseHandle(hProcess);
						sprintf(buffer, TEXT("Ph∏t hi÷n ph“n m“m [ %s ] kh´ng hÓp l÷"), sTemp);
						SplashScreen(&SplashError, 2, 1, buffer, 5000);
						//CHClientDisconnectSend(CLIENT_DISCONNECT_DUMP_DETECTION,CaptionName,processID);
						return 0;
					}
				}
				
				//if (gWindowListOK == 1)
				//{
				//	if (gDumpCheck.CheckWindowName(CaptionName) == 0)
				//	{
				//		//strcpy(gMessage.m_CaptionNameProcess, CaptionName);
				//		TerminateProcess(hProcess, 1);
				//		CloseHandle(hProcess);
				//		sprintf(buffer, TEXT("Ph∏t hi÷n ph“n m“m [ %s ] kh´ng hÓp l÷"), CaptionName);
				//		SplashScreen(&SplashError, 2, 1, buffer, 5000);
				//		//CHClientDisconnectSend(CLIENT_DISCONNECT_DUMP_DETECTION,CaptionName,processID);
				//		return 0;
				//	}
				//}

				if ((flag & 2) != 0 && gDumpCheck.Check(sTemp, CaptionName) == 0)
				{
					//strcpy(gMessage.m_CaptionNameProcess, CaptionName);
					TerminateProcess(hProcess, 1);
					CloseHandle(hProcess);
					sprintf(buffer, TEXT("Ph∏t hi÷n ph“n m“m [ %s ]  kh´ng hÓp l÷"), sTemp);
					SplashScreen(&SplashError, 2, 1, buffer, 5000);
					//CHClientDisconnectSend(CLIENT_DISCONNECT_DUMP_DETECTION,CaptionName,processID);
					return 0;
				}

				if ((flag & 4) != 0 && gExecutableCheck.Check(sTemp, CaptionName) == 0)
				{
					//strcpy(gMessage.m_CaptionNameProcess, CaptionName);
					TerminateProcess(hProcess, 1);
					CloseHandle(hProcess);
					sprintf(buffer, TEXT("Ph∏t hi÷n ph“n m“m [ %s ]  kh´ng hÓp l÷"), sTemp);
					SplashScreen(&SplashError, 2, 1, buffer, 5000);
					//CHClientDisconnectSend(CLIENT_DISCONNECT_EXECUTABLE_DETECTION,CaptionName,processID);
					return 0;
				}

				if ((flag & 8) != 0 && gLibraryCheck.Check(sTemp, CaptionName) == 0)
				{
					//strcpy(gMessage.m_CaptionNameProcess, CaptionName);
					TerminateProcess(hProcess, 1);
					CloseHandle(hProcess);
					sprintf(buffer, TEXT("Ph∏t hi÷n ph“n m“m [ %s ] kh´ng hÓp l÷"), sTemp);
					SplashScreen(&SplashError, 2, 1, buffer, 5000);
					//CHClientDisconnectSend(CLIENT_DISCONNECT_LIBRARY_DETECTION,CaptionName,processID);
					return 0;
				}
			}
		}

		CloseHandle(hProcess);
	}

	return 1;
}

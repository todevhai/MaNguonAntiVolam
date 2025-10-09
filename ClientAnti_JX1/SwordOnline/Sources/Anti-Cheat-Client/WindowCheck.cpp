#include "stdafx.h"
#include "WindowCheck.h"
#include "HackServerProtocol.h"
#include "ListManager.h"

CWindowCheck gWindowCheck;

CWindowCheck::CWindowCheck()
{

}

CWindowCheck::~CWindowCheck()
{

}

void CWindowCheck::Scan()
{
	HWND WindowHwnd;

	for(std::vector<WNDW_LIST_INFO>::iterator it=gListManager.gWindowListInfo.begin();it != gListManager.gWindowListInfo.end();it++)
	{
		//Sleep(30);

		switch(it->type)
		{
			case WINDOW_NAME:
				if((WindowHwnd=FindWindowExA(0,0,0,it->text)) != 0)
				{
					DWORD pid = 0;
					TCHAR buffer[256];
					char wnd_title[64];
					GetWindowText(WindowHwnd, wnd_title, sizeof(wnd_title));
					std::string excludeProcess = std::string(wnd_title);

					if (excludeProcess.find("365") != std::string::npos)
						continue;

					sprintf(buffer, TEXT("Ph¸t hiÖn phÒn mÒm [ %s ] kh«ng hîp lÖ"), wnd_title);
					SplashScreen(&SplashError, 2, 1, buffer, 5000);
					GetWindowThreadProcessId(WindowHwnd,&pid);
					HANDLE tmpHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
					if (NULL != tmpHandle)
					{
						TerminateProcess(tmpHandle, 1);
						CloseHandle(tmpHandle);
					}
					//CHClientDisconnectSend(CLIENT_DISCONNECT_WINDOW_DETECTION,it->text,pid);
					return;
				}
				break;
			case WINDOW_CLASS:
				if((WindowHwnd=FindWindowExA(0,0,it->text,0)) != 0)
				{
					DWORD pid = 0;
					TCHAR buffer[256];
					char wnd_title[64];
					GetWindowText(WindowHwnd, wnd_title, sizeof(wnd_title));
					std::string excludeProcess = std::string(wnd_title);
					if (excludeProcess.find("365") != std::string::npos)
						continue;
					sprintf(buffer, TEXT("Ph¸t hiÖn phÒn mÒm [ %s ] kh«ng hîp lÖ"), wnd_title);
					SplashScreen(&SplashError, 2, 1, buffer, 5000);
					GetWindowThreadProcessId(WindowHwnd, &pid);
					HANDLE tmpHandle = OpenProcess(PROCESS_ALL_ACCESS, TRUE, pid);
					if (NULL != tmpHandle)
					{
						TerminateProcess(tmpHandle, 1);
						CloseHandle(tmpHandle);
					}
					//CHClientDisconnectSend(CLIENT_DISCONNECT_WINDOW_DETECTION,it->text,pid);
					return;
				}
				break;
		}
		Sleep(30);
	}
}

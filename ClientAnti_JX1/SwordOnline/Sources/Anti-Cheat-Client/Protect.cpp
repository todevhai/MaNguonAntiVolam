// Protect.cpp: implementation of the CProtect class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Protect.h"
#include "CCRC32.H"
#include "Log.h"
#include "Util.h"
#include <string>
#include<iostream>

#define Func_526CB0		((int(__thiscall*)(DWORD *This)) 0x526CB0)
#define Func_55E7D0		((int(__stdcall*)(int a1, int a2)) 0x55E7D0)

using namespace std;

CProtect gProtect;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProtect::CProtect() // OK
{
	m_Memory = 50;
	m_Check = 0;
}

CProtect::~CProtect() // OK
{

}

bool CProtect::ReadMainFile(char* name) // OK
{
	CCRC32 CRC32;

	//bool waitgetdata = false;

	m_MainInfo_OutSize = 0;

	if(CRC32.FileCRC(name,(DWORD*)&this->m_ClientFileCRC,1024) == 0)
	{
		return 0;
	}
	
	//HANDLE m_file = CreateFile(name, GENERIC_WRITE | GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,0);

	////giaimafile("fwcld.temp", "fwcld.bak", true);

	//if(m_file == INVALID_HANDLE_VALUE)
	//{
	//	MessageBox(0, "INVALID_HANDLE_VALUE", "Info", MB_OK | MB_ICONINFORMATION);
	//	return 0;
	//}

	//if(GetFileSize(m_file,0) != sizeof(MAIN_FILE_INFO))
	//{
	//	MessageBox(0, "GetFileSize", "Info", MB_OK | MB_ICONINFORMATION);
	//	CloseHandle(m_file);
	//	return 0;
	//}

	/*if(ReadFile(m_file,&this->m_MainInfo,sizeof(MAIN_FILE_INFO),&m_MainInfo_OutSize,0) == 0)
	{
		MessageBox(0, "ReadFile", "Info", MB_OK | MB_ICONINFORMATION);
		CloseHandle(m_file);
		return 0;
	}*/

	DWORD OutSize = 0;

	MAIN_FILE_INFO info;

	memset(&info, 0, sizeof(info));

	//HANDLE file = CreateFile("fwcld.bak", GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_ARCHIVE, 0);

	//if (file == INVALID_HANDLE_VALUE)
	//{
	//	return 0;
	//}

	char datarequest[200];
	//sprintf(datarequest, "data 1 %d", sizeof(this->m_MainInfo));
	//MessageBox(0, datarequest, "Info", MB_OK | MB_ICONINFORMATION);

	unsigned char* BufferOut;

	BufferOut = DecryptFileProtect(name);

	memcpy(&this->m_MainInfo, BufferOut, sizeof(MAIN_FILE_INFO));

	free(BufferOut);

	//memcpy(&info, BufferOut, sizeof(MAIN_FILE_INFO));
	
	if(sizeof(this->m_MainInfo) != sizeof(MAIN_FILE_INFO))
	{
		return 0;
	}

	/*for (int n = 0; n < sizeof(MAIN_FILE_INFO); n++)
	{
		((BYTE*)&this->m_MainInfo)[n] += 0x78;
		((BYTE*)&this->m_MainInfo)[n] ^= 0xB3;
	}*/

	
	//memcpy(&info, BufferOut, sizeof(MAIN_FILE_INFO));

	//if (WriteFile(file, &info, sizeof(MAIN_FILE_INFO), &OutSize, 0) == 0)
	//{
	//	CloseHandle(file);
	//	return 0;
	//}

	for (int n = 0; n < sizeof(MAIN_FILE_INFO); n++)
	{
		((BYTE*)&this->m_MainInfo)[n] += 0x78;
		((BYTE*)&this->m_MainInfo)[n] ^= 0xB3;
	}

	//gMessage.GetLanguage();

	//gMessage.LoadEng(this->m_MainInfo.EngMessageInfo);

	//gMessage.LoadPor(this->m_MainInfo.PorMessageInfo);

	//gMessage.LoadSpn(this->m_MainInfo.SpnMessageInfo);

	//sprintf(datarequest, "data %s", &this->m_MainInfo.ClientSerial);

	//MessageBox(0, datarequest, "Info", MB_OK | MB_ICONINFORMATION);
	//CloseHandle(m_file);

	return 1;
}

void CProtect::CheckClientFile() // OK
{
	if(this->m_MainInfo.ClientCRC32 == 0)
	{
		return;
	}

	char name[MAX_PATH] = {0};

	if(GetModuleFileName(0,name,sizeof(name)) == 0)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(33));
		SafeExitProcess();
	}

	if(_stricmp(ConvertModuleFileName(name),this->m_MainInfo.ClientName) != 0)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(34));
		SafeExitProcess();
	}

	CCRC32 CRC32;

	DWORD ClientCRC32;

	if(CRC32.FileCRC(this->m_MainInfo.ClientName,&ClientCRC32,1024) == 0)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(35));
		SafeExitProcess();
	}

	if(this->m_MainInfo.ClientCRC32 != ClientCRC32)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(36));
		SafeExitProcess();
	}
}

void CProtect::CheckPluginFile() // OK
{
	if(this->m_MainInfo.PluginCRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD PluginCRC32;

	if(CRC32.FileCRC(this->m_MainInfo.PluginName,&PluginCRC32,1024) == 0)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(37));
		SafeExitProcess();
	}

	if(this->m_MainInfo.PluginCRC32 != PluginCRC32)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(38));
		SafeExitProcess();
	}

	HMODULE module = LoadLibrary(this->m_MainInfo.PluginName);

	if(module == 0)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(39));
		SafeExitProcess();
	}

	void (*EntryProc)() = (void(*)())GetProcAddress(module,"EntryProc");

	if(EntryProc != 0){EntryProc();}
}

void CProtect::CheckVerifyFile() // OK
{
	if(this->m_MainInfo.VerifyCRC32 == 0)
	{
		return;
	}

	CCRC32 CRC32;

	DWORD VerifyCRC32;

	if(CRC32.FileCRC(this->m_MainInfo.VerifyName,&VerifyCRC32,1024) == 0)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(40));
		SafeExitProcess();
	}

	if(this->m_MainInfo.VerifyCRC32 != VerifyCRC32)
	{
		gLog.Output(LOG_DEBUG,GetEncryptedString(41));
		SafeExitProcess();
	}
}

DWORD __fastcall sub_10001300(int nMemory)
{
	DWORD result;
	void* v3;
	unsigned int v4;
	HANDLE CurrentProcess;
	HANDLE v6;
	char v7[12];
	unsigned int v8;
	PROCESS_MEMORY_COUNTERS_EX info;
	ZeroMemory(&info, sizeof(info));
	result = GetCurrentProcessId();
	if (result)
	{
		result = (DWORD)OpenProcess(0x410u, 0, result);
		v3 = (void*)result;
		if (result)
		{
			if (GetProcessMemoryInfo((HANDLE)result, (PROCESS_MEMORY_COUNTERS*)&info, sizeof(info)))
			{
				v4 = gProtect.m_Memory << 20;
				if (nMemory > 0)
					v4 = nMemory << 20;
				if (v8 >= v4)
				{
					CurrentProcess = GetCurrentProcess();
					SetProcessWorkingSetSize(CurrentProcess, 0xFFFFFFFF, 0xFFFFFFFF);
					v6 = GetCurrentProcess();
					SetThreadPriority(v6, -2);
				}
			}
			return CloseHandle(v3);
		}
	}
	return result;
}

DWORD __stdcall sub_10002F40(DWORD* This)
{
	/*int result;
	int v1;
	result = 0x526CB0;
	v1 = result;
	if (result)
	{
		if (gProtect.m_Memory > 0 && !gProtect.m_Check)
		{
			sub_10001300(gProtect.m_Memory);
			gProtect.m_Check = 1;
		}
		return v1;
	}*/
	//sub_10001300(gProtect.m_Memory);
	void* v3;
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());
	v3 = (void*)hProcess;
	SetProcessWorkingSetSize(GetCurrentProcess(), 0xFFFFFFFF, 0xFFFFFFFF);
	CloseHandle(v3);
	return Func_526CB0((DWORD*)0x6DE340);
}

DWORD __stdcall sub_10003670(int a1, int a2)
{
	DWORD result;

	result = Func_55E7D0(a1, a2);
	if (gProtect.m_Memory > 0)
		return sub_10001300(gProtect.m_Memory);
	return result;
}
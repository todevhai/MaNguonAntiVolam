#include "StdAfx.h"
#include <Windows.h>
#include <CommDlg.h>
#include <WinUser.h>
#include "StatusDisplayer.h"

StatusDisplayer gStatusDisplayer;
#define pRepresent				((LPVOID(*)()) 0x006DE30C)

StatusDisplayer::StatusDisplayer() // OK
{
	//for (int n = 0; n < MAX_LOG_TEXT_LINE; n++)
	//{
	//	memset(&this->m_log[n], 0, sizeof(this->m_log[n]));
	//}

	//this->m_font = CreateFont(50, 0, 0, 0, FW_THIN, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Times");

	//this->m_brush[0] = CreateSolidBrush(RGB(105, 105, 105));
	//this->m_brush[1] = CreateSolidBrush(RGB(110, 240, 120));

	//strcpy_s(this->m_DisplayerText[0], "STANDBY MODE");
	//strcpy_s(this->m_DisplayerText[1], "ACTIVE MODE");
}

StatusDisplayer::~StatusDisplayer() // OK
{
	//ID_VIEW_SMALLICON
	//DeleteObject(this->m_font);
	//DeleteObject(this->m_brush[0]);
	//DeleteObject(this->m_brush[1]);
}

void StatusDisplayer::Init(HWND hWnd) // OK
{
	HMODULE hModule = LoadLibraryA("engine.dll");

	m_EngineBaseAddress = (DWORD)((void*)hModule);

	this->m_hwnd = hWnd;

	//char Offset[100];
	
	//HMODULE hModule = LoadLibraryA("engine.dll");

	//uintptr_t serverdllBaseAddress = GetModuleBaseAddress(GetProcessId(v1), "engine.dll");

	//sprintf(Offset, "%x - %x - %s", (void*)m_EngineBaseAddress, m_EngineBaseAddress, (char*)LOBYTE(m_EngineBaseAddress));

	//MessageBox(0, Offset, "Testing", MB_OK);
}

void StatusDisplayer::Run() // OK
{
	//this->SetWindowName();
	//this->PaintAllInfo();
	//this->LogTextPaint();
}

//HWND StatusDisplayer::CreateListView(HWND hwndParent)
//{
//
//}
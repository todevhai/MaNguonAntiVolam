#pragma once

#define MAX_LOADSTRING 100

class StatusDisplayer
{
public:
	StatusDisplayer();
	virtual ~StatusDisplayer();
	void Init(HWND hWnd);
	void Run();
	//HWND CreateListView(HWND hwndParent);
private:
	HWND m_hwnd;
	HFONT m_font;
	HBRUSH m_brush[2];
	DWORD m_EngineBaseAddress;
};

extern StatusDisplayer gStatusDisplayer;
#pragma once
#include <KWin32.h>
#include <KIniFile.h>
#include <KIniFile.h>
#include "iRepresentShell.h"
#include "KRepresentUnit.h"
#include "Import.h"

class KIniFile;
//typedef void CallWindow(TempWindow*);

class TempWindow {
protected:
	BYTE vfptr[252];
public:
	TempWindow() {
		//CreateWndWindow2((BYTE*)this);
		//CreateWndWindow(((char*)this));
		//*this = *(TempWindow*)CreateWndWindow(new TempWindow);
		/*CallWindow fp = *((CallWindow*)0x441A60);
		(TempWindow(*this).*fp)();
		fp(this);*/
		//CreateWndWindow(new TempWindow);
		//*this = *(TempWindow*)*((unsigned char**)0x659078);
		//*this = *(TempWindow*)CreateWndWindow(this);
		//BYTE* (__thiscall** fp)() = (BYTE * (__thiscall**)())0x441A60;
		//(**fp)();
		// *((char**)0x659078)
		//*this = *static_cast<TempWindow*>((TempWindow*)*((unsigned char**)0x659078));
		//CreateWndWindow2((BYTE*)reinterpret_cast<TempWindow*>(this));
		//char* (TempWindow::**fnTempWindow)() = (char* (TempWindow::**)())0x441A60;
		//(this->**fnTempWindow)();
		//void* baseClass_WndWindow = (void*)0x63DB6C;
		//void* baseClass_WndWindow = (void**)0x6590A4;
		//baseClass_WndWindow = this;
		//*this = *(TempWindow*)*((unsigned char**)0x659040);
		//*this = *(TempWindow*)*((unsigned char**)0x659078);
		//(*(TempWindow*)this) = (*(TempWindow*)(void*)0x659078);

		//*((void**)this) = +*(static_cast<void**>(*((void**)0x659078)));
		//*((void**)this) = &*(static_cast<void**>(*((void**)0x63DB6C)));

		//*this = *(TempWindow*)CreateWndWindow((DWORD*)baseClass_WndWindow);
		//CreateWndWindow((BYTE*)this);
		//CreateWndWindow(baseClass_WndWindow);
		//*this = *static_cast<TempWindow*>(baseClass_WndWindow); // dynamic_cast
		//*(this) = **((TempWindow**)CreateWndWindow(this)); // dynamic_cast
		//*(char*)this = +*(static_cast<char*>((char*)baseClass_WndWindow));
		//*(void**)this = (static_cast<void*>((unsigned char**)0x63DB6C));
		//void* vtable = (void*)((char*)this);
		//TempWindow vtable = *(TempWindow*)((void*)0x659078);
		//baseClass_WndWindow = (void*)((char*)0x6590A4);
		//CreateWndWindow((char*)&vtable);
		//CreateWndWindow(&(*this));
		//* this = *(TempWindow*)CreateWndWindow((void*)0x659078);
		//*(unsigned char**)this = (unsigned char*)CreateWndWindow((unsigned char**)this);
	}
	virtual ~TempWindow() {
	/*	void (TempWindow:: * *fnWndProc)() = (void (TempWindow::**)())0x441B80;
		(this->* * fnWndProc)();*/
	}
	//virtual void Func() {}
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return KWndWindow_Init(this, pIniFile, pSection); }
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return static_cast<TempWindow*>(this)->WndProc(uMsg, uParam, nParam); }
	virtual int		PtInWindow(int x, int y) { return static_cast<TempWindow*>(this)->PtInWindow(x, y); }

	/*virtual int		Init(KIniFile* pIniFile, const char* pSection) {
		int (TempWindow:: * *fnInit)(KIniFile*, const char*) = (int (TempWindow::**)(KIniFile*, const char*))0x442060;
		return (this->* * fnInit)(pIniFile, pSection);
	}*/
	/*virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) {
		int (TempWindow:: * *fnWndProc)(unsigned int, unsigned int, int) = (int (TempWindow::**)(unsigned int, unsigned int, int))0x4428D0;
		return (this->* * fnWndProc)(uMsg, uParam, nParam);
	}*/
	/*virtual int		PtInWindow(int x, int y) { 
		int (TempWindow:: * *fnWndProc)(int, int) = (int (TempWindow::**)(int, int))0x442490;
		return (this->* * fnWndProc)(x, y);
	}*/
	virtual void	PaintWindow() { static_cast<TempWindow*>(this)->PaintWindow(); }
	virtual void	SetSize(int nWidth, int nHeight) { KWndWindow_SetSize((unsigned int*)this, nWidth, nHeight); }
	virtual void	Show() { KWndWindow_Show((unsigned int*)this); }
	virtual void	Hide() { KWndWindow_Hide((unsigned int*)this); }
	virtual	void	Enable(int bEnable) { KWndWindow_Enable(this, bEnable); }
	virtual TempWindow* TopChildFromPoint(int x, int y) { return NULL; }
private:
	virtual void	Breathe() {}
	virtual void	OnLBtnDown(int x, int y) {}
};

extern TempWindow g_TempWindow;
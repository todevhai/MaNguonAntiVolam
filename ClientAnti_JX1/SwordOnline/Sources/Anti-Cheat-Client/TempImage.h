#pragma once
#include <KWin32.h>
#include <KIniFile.h>
#include <KIniFile.h>
#include "iRepresentShell.h"
#include "KRepresentUnit.h"
#include "TempWindow.h"

class KIniFile;

class TempImage : public TempWindow {
protected:
	char vfptr[184];
public:
	
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return static_cast<TempImage*>(this)->Init(pIniFile, pSection); }
	virtual int		PtInWindow(int x, int y) { return KWndImage_PtInWindow(this, x, y); }
	virtual void	PaintWindow() { KWndImage_PaintWindow(this); }
	TempImage() {
		/*void (TempImage:: * *TempImage)() = (void (TempImage::**)())0x42C010;
		(this->* * TempImage)();*/
		//CreateWndImage(new TempImage);
		//CreateWndImage(this);
		//*this = *(TempImage*)CreateWndImage(this);
		//TempImage vtable = *(TempImage*)((void*)this);
		//*this = *(TempImage*)*((unsigned int**)0x65906C);
		//*(void**)this = (static_cast<void*>(((void**)0x63CF4C)));
		//*(void**)this = +*(static_cast<void**>(*((void**)0x659078)));
		//*((void**)this) = &*(static_cast<void**>(*((void**)0x63CF4C)));
		//**((TempImage**)this) = **((TempImage**)(void*)0x659040);
		//int** ivtable = (int**)vtable;
		//CreateWndImage((int*)&vtable);
	}
};

extern TempImage g_TempImage;
#pragma once
#include <KWin32.h>
#include <KIniFile.h>
#include <KIniFile.h>
#include "iRepresentShell.h"
#include "KRepresentUnit.h"
#include "TempImage.h"

class KIniFile;
class TempWindow;
//typedef void* (__thiscall** ptnButton)(void*) = (void* (__thiscall**)(void*))0x425E20;

template <typename DELEGATE>
struct ButtonAction
{
	typedef int (DELEGATE::* Type)(int);
};

template <typename DELEGATE>
void newButton(DELEGATE* obj, typename ButtonAction<DELEGATE>::Type method)
{
	std::function<int(int)> callback = std::bind(
		method, obj, std::placeholders::_1);
}

class TempButton : public TempImage {
protected:
	char vfptr1[1];
private:
	char vfptr2[149];
public:
	TempButton() {
		//CreateWndButton(new TempButton);
		//CreateWndButton(this);
		//*this = *(TempButton*)CreateWndButton(this);
		/*void (TempButton:: * *TempButton)() = (void (TempButton::**)())0x425E20;
		(this->**TempButton)();*/
		//void** vtable = *((void***)((char*)this));
		//int** ivtable = (int**)vtable;
		//void* firstFunction = (void*)ivtable[0];
		//CreateWndButton(vtable);
		//**((TempButton**)this) = **((TempButton**)(void*)0x659434);
		//*this = *(TempButton*)*((short**)0x659460);
		//*((void**)this) = &*(static_cast<void**>(*((void**)0x63CBCC)));
		//* ((void**)this) = &*(static_cast<void**>(*((void**)0x63CBCC)));
	}
	virtual int		Init(KIniFile* pIniFile, const char* pSection) {
		//int (TempButton:: * *CastInit)(KIniFile*, const char*) = (int(TempButton::**)(KIniFile*, const char*))0x425FB0;
		//return (this->**CastInit)(pIniFile, pSection);
		//return reinterpret_cast<TempButton*>((this->* * CastInit)(pIniFile, pSection))->Init(pIniFile, pSection);
		return KWndButton_Init(this, pIniFile, pSection);
		//return static_cast<TempButton*>(this)->Init(pIniFile, pSection);
	}

	//virtual int		Init(KIniFile* pIniFile, const char* pSection) {
	//	int (TempButton::**CastInit)(KIniFile*, const char*) = (int(TempButton::**)(KIniFile*, const char*))0x425FB0;
	//	return (this->**CastInit)(pIniFile, pSection);
	//	//return static_cast<TempButton*>(this)->Init(pIniFile, pSection);
	//}
	virtual const char* GetShortKey() { return NULL; }
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return KWndButton_WndProc(this, uMsg, uParam, nParam); }
	virtual void	PaintWindow() {}
	virtual void	OnButtonClick() {}
	virtual int		GetToolTipInfo(char* szTip, int nMax) { return 0; }
};

extern TempButton g_TempButton;
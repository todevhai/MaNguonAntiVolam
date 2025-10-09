#pragma once
#include <KWin32.h>
#include "KCanvas.h"
#include "KIniFile.h"
//#include "Ui/Elem/Wnds.h"
//#include <KIniFile.h>
//#include <KList.h>
//#include "KWin32App.h"
//#include "TempClass.h"
#include "Import.h"
//#include "Ui/Elem/WndWindow.h"
//#include "Ui/Elem/WndImage.h"
//#include "Ui/Elem/WndButton.h"
#include "TempWindow.h"
#include "TempButton.h"
//#include "NewUiWindow.h"
//#include "NewUiImage.h"
//#include "NewUiButton.h"
//#include "NewUiShowAnimate.h"
//#ifndef UIAUTO_H
//#define UIAUTO_H
#include "NewComWindow.h"
//#include "Elem/ComWindow.h"
//#include "NewUiText.h"
#include <vector>

//Offset size KWndWindow : 340, 
//Offset size KWndButton Image : 592 , ImagePart 1056
//Offset size KWndLabeledTextButton : 812
//Offset size KWndLabeledButton : 768
//Offset size KWndImage ImagePart : 480, 440
//Offset size KWndImage : 480, 440
// Offset size KWndText32 : 504, 
//DWORD pid;
//#include "Elem/WndButton.h"
//void InitWindow(KWndWindow* ptr);
//void InitImage(KWndImage* ptr);
//void InitButton(KWndButton* ptr);

void HookLoadSchemeForEachWnd(const char* This);
int HookUiArenaIcon();
void __fastcall HookOnButtonClick(short* This);
int HookAddOneWindow(const void* This, KIniFile* pIniFile, const char* pSection);
int __cdecl HookRegisterComClass(char* Str1);
int __cdecl HookUiGameMain(unsigned int a1, int a2, int a3);
int	HookUiStart(bool bStartMusic, bool bJustLaunched);
void HookUiInitProc(unsigned int* pWnd, unsigned int* This);
int HookWindowWndProc(unsigned int uMsg, unsigned int uParam, int nParam, unsigned int* This);
//int HookWindowWndProc(unsigned int uMsg, unsigned int uParam, int nParam, unsigned int* This);
//int HookWindowWndProc(unsigned int uMsg, int uParam, int nParam, unsigned int* This);
int __fastcall HookButtonWndProc(unsigned int uMsg, int uParam, int nParam, unsigned int* This);
//char HookOnLBtnDown(int x, int y, unsigned int* This);
void InstallObject(void* base, KWndWindow* object);
int			NewUiInit();
void		UiExit();
void* DisMember(size_t size, ...);

class KIniFile;
//class WorldObject;
//class KWndWindow;
//class KWndImage;
//class KWndButton;


//inline ~KWndWindow() { }


// 0x637C90 UiGameSpace
// 812 KWndLabeledTextButton
// 592 KWndButton
// 340 KWndWindow
//KWndAutoPlay: 3944
//Player_AutoPlay : 2104
//KWndWindow : 1840
//KWndButton : 2104
// dword_63BF64		1C8		456
//typedef KWndButton* (*KWndButton)(); override
// 1312
//void (KWndWindow::* pfn)(void);
 
//struct WndWindow
//{
//protected:
//	int			vptr_Window[340];
//public:
//	WndWindow() {}
//	virtual ~WndWindow() {}
//};
//
//
//struct WndImage : public WndWindow
//{
//protected:
//	int			vptr_Window[340];
//public:
//	WndImage() {}
//	virtual ~WndImage() {}
//};
//
//
//struct WndButton : public WndImage
//{
//protected:
//	unsigned short	m_Flag;
//public:
//	WndButton() { CreateWndButton((unsigned int*)this); }
//	virtual ~WndButton() {}
//	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return 0; }
//	virtual void	OnButtonClick() {}
//	virtual const char* GetShortKey() { return NULL; }
//private:
//	void			OnLBtnDown(bool bDoubleClick);	//响应鼠标左键在此按下
//	void			OnLBtnUp();						//响应鼠标左键在此放开
//	void			OnLBtnDownMove();				//响应鼠标左键按下时的移动
//};

//CreateWndButton((unsigned int*)this);

//class Player_AutoPlay : public KWndButton
class Player_AutoPlay : TempButton
{
public:
	//DECLARE_COMCLASS(Player_AutoPlay)	
	//Player_AutoPlay();
	//Player_AutoPlay() {
	//	//*(Player_AutoPlay*)&NewAddressData1 = *dynamic_cast<Player_AutoPlay*>((Player_AutoPlay*)this);
	//	//BYTE* vtableWindow = (BYTE*)((unsigned int*)((void*)this));
	//	//BYTE* vtableWindow = (BYTE*)((unsigned int*)((void*)this));
	//	//void* vptr_WndButton = (void*)0x63CBCC;
	//	//void* vptr_WndWindow = (void*)0x63DB6C;

	//	//vptr_WndWindow = (BYTE*)((unsigned int*)((void*)this));
	//	//vptr_WndButton = (void*)this;
	//	RefCountBaseClass(this, 340, 1, (void*)RefWndWindow, (void*)RefWndBase);
	//	//CreateWndWindow((BYTE*)((unsigned int*)((void*)this)));
	//	//CreateWndButton((void*)this);
	//}
	void OnButtonClick();
	//int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	const char* GetShortKey();
};

class Player_AutoPlay2 : protected TempButton
{
public:
	//DECLARE_COMCLASS(Player_AutoPlay2)
	//Player_AutoPlay2();
	/*Player_AutoPlay2()
	{ 
		*this = *new Player_AutoPlay2;
		KWndWindow* m_window = this;
		CreateWndButton(m_window);
	}*/
	//Player_AutoPlay2() {
	//	//*(Player_AutoPlay*)&NewAddressData1 = *dynamic_cast<Player_AutoPlay*>((Player_AutoPlay*)this);
	//	//BYTE* vtableWindow = (BYTE*)((unsigned int*)((void*)this));
	//	//BYTE* vtableWindow = (BYTE*)((unsigned int*)((void*)this));
	//	//void* vptr_WndButton = (void*)0x63CBCC;
	//	//void* vptr_WndWindow = (void*)0x63DB6C;

	//	//vptr_WndWindow = (BYTE*)((unsigned int*)((void*)this));
	//	//vptr_WndButton = (void*)this;
	//	//RefCountBaseClass(this, 340, 1, (void*)RefWndWindow, (void*)RefWndBase);
	//	//CreateWndWindow((BYTE*)((unsigned int*)((void*)this)));
	//	//CreateWndButton(new TempButton);
	//}
	virtual int		Init(KIniFile* pIniFile, const char* pSection);
	void	OnLBtnDown(int x, int y);
	void OnButtonClick();
	const char* GetShortKey();
};

//extern Player_AutoPlay* gAutoPlay;

//class __declspec(novtable) 
// 
//{
//protected:
//	int			vptr_Window[340];
//public:
//	KWndWindow() {}
//	virtual ~KWndWindow() {}
//};
//
//class __declspec(novtable) KWndImage : public KWndWindow
//{
//protected:
//	int			vptr_KWndImage[440];
//public:
//	KWndImage() {}
//	virtual ~KWndImage() {}
//};
//
//class __declspec(novtable) KWndButton : public KWndImage
//{
//protected:
//	int			vptr_KWndButton[592];
//public:
//	KWndButton() {}
//	virtual ~KWndButton() {}
//};

//typedef void* (__thiscall** ptnWndWindow)(void*);

//public KWndWindow KWndImage

//class KWndAutoPlay : public KWndWindow
//template <typename KWndWindow>
//template <typename KWndWindow, class Allocator = std::allocator<KWndWindow>>

class KWndAutoPlay : public TempWindow
{
public:
	//using KWndWindowBase = KWndWindow; // declare Type here
	//DECLARE_COMCLASS(KWndAutoPlay)
	/*KWndAutoPlay() {
		RefCountBaseClass(this, 340u, 1, fnRefWndWindow, fnRefWndBase);
	}*/
	//~KWndAutoPlay() {}
	//m_BtnAutoPlay = *(Player_AutoPlay*)new Player_AutoPlay;
	KWndAutoPlay() {
		//CreateWndWindow2((BYTE*)this);
		//CreateWndWindow2(*((BYTE**)this));
		//TempWindow::TempWindow();
		//this->CreateWndWindow2((TempWindow**)this);
		//m_pSelf = (KWndAutoPlay*)CreateWndWindow(&TempWindow::TempWindow());
		//m_pSelf = (KWndAutoPlay*)CreateWndWindow2(*(new TempWindow()));
		//CreateWndWindow(TempWindow::vfptr);
		//TempWindow m_pSelf = *((TempWindow*)CreateWndWindow(new TempWindow));
		//void* (TempWindow::**fnTempWindow)(BYTE*) = (void* (TempWindow::**)(BYTE*))0x441A60;
		//(this->**fnTempWindow)(this->vfptr);
		//CreateWndWindow((DWORD*)this);
		//CreateWndWindow((*(unsigned char**)this));
		//*((void**)this) = +*(static_cast<void**>(*((void**)0x63DB6C)));
		//*((void**)this) = &*(static_cast<void**>(*((void**)0x63CBCC)));
	}
	//virtual ~KWndAutoPlay() { ReleaseWndButton((DWORD*)this); }
	//virtual ~KWndAutoPlay() { DestroyWndWindow((DWORD*)this); }
	~KWndAutoPlay() { DestroyWndWindow((DWORD*)this); }
	static KWndAutoPlay*	OpenWindow();
	static KWndAutoPlay*	GetIfVisible();
	static void				CloseWindow(bool bDestroy);
	static void				LoadScheme(const char* pScheme);
	//int		Init(KIniFile* pIniFile, const char* pSection);// { return 0; }	
private:
	//KWndAutoPlay() {

	//	//RefCountBaseClass(this, 340, 1, fnRefWndWindow, fnRefWndBase);

	//	//CreateWndWindow((DWORD*)this);

	//	//void* baseClass_WndWindow = (void*)0x659078;

	//	//baseClass_WndWindow = this;

	//	//*this = *static_cast<KWndAutoPlay*>(baseClass_WndWindow); // dynamic_cast

	//	//void* baseClass_WndButton = (void*)0x659434;

	//	//baseClass_WndButton = &this->m_BtnAutoPlay;

	//	//*(&this->m_BtnAutoPlay) = (KWndButton&)static_cast<void*>(baseClass_WndButton);
	//}
	
	//KWndAutoPlay() { 
	//	//*this = *new KWndAutoPlay;
	//	//KWndWindow* m_window = this; 
	//	//CreateWndWindow((DWORD*)this);
	//	RefCountBaseClass(this, 340, 1, (BaseWndWindow)0x441A60, (RefWndBase)0x44ADC0);
	//}
	//~KWndAutoPlay() {}
	//KWndAutoPlay();
	
	void	Initialize();
	void	LoadScheme(KIniFile* pIni);
	void KWndAutoPlay::OnLBtnDown(bool bDoubleClick);
	void			OnButtonClick();
	const char* GetShortKey();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	//void	PaintWindow();
	//void		Hide();// {}
	//void		Show();// {}
	//int		PtInWindow(int x, int y) { return 0; }
	//void		SetSize(int nWidth, int nHeight) {}
	//void		Enable(int bEnable) {}
	//KWndButton* GetActiveBtn();
private:
	static KWndAutoPlay* m_pSelf;
	//KWndButton m_BtnAutoPlay;
	//Player_AutoPlay2 m_BtnAutoPlay;
	//TempButton m_BtnAutoPlay;
public:
	//Player_AutoPlay2 m_BtnAutoPlay;
	TempButton m_BtnAutoPlay;
};

extern KWndAutoPlay	g_WndAutoPlay;

//#endif

#include "Util.h"
//#include "AutoPlay.h"
#include <fstream>
//#include <iRepresent/iRepresentShell.h>
//#include "KRepresentUnit.h"
//#include "Ui/Elem/WndMessage.h"
#include "NewUiShell.h"
#include "TempClass.h"
//#include "AutoPlay.h"
//#include <Elem/WndMessage.h>
//#include "NewComWindow.h"
//#include "NewUiButton.h"
//#include "NewUiShowAnimate.h"
//#include <Elem/WndWindow.h>
//#include <Elem/WndImage.h>
//#include <Elem/WndButton.h>
//#include "Elem/WndWindow.hpp"
//#include "Elem/WndImage.hpp"
//#include "Elem/WndWindow.h"
//#include "Elem/WndImage.h"
//#include "Elem/WndButton.h"
//#include "Ui/Elem/WndWindow.cpp"
//#include "Ui/Elem/WndImage.cpp"
//#include "Ui/Elem/WndButton.cpp"

using namespace std;

class KIniFile;
//class KWndWindow;
//iRepresentShell* g_pRepresentUiShell;
//extern iRepresentShell* g_pRepresentShell;
//class KWndWindow;
//KWndAutoPlay gAutoPlay;
KWndAutoPlay g_WndAutoPlay = *new KWndAutoPlay;
//KWndAutoPlay g_WndAutoPlay = *((KWndAutoPlay*)CreateWndWindow(new TempWindow));
//KUiAutoPlay* m_AutoPlay;
Player_AutoPlay gAutoPlay;
//KWndWindow KWndWindow;
//unsigned int RefWndBase = 0x44ADC0;
//unsigned int RefWndImage = 0x42C010;
//unsigned int RefWndImage2 = 0x412230;
//unsigned int RefWndButton = 0x425E20;
//unsigned int RefWndButton2 = 0x425E32;
//unsigned int RefWndText32 = 0x43DCE0;
//unsigned int RefWndWindow = 0x432600;

RefWndBase fnRefWndBase = (RefWndBase)(void**)0x44ADC0;
BaseWndWindow fnBaseWndWindow = (BaseWndWindow)(void**)0x441A60;
VtableWndWindow fnVtableWndWindow = (VtableWndWindow)(void**)0x441B80;
RefWndWindow fnRefWndWindow = (RefWndWindow)(void**)0x432600;
RefWndImage fnRefWndImage = (RefWndImage)(void**)0x42C010;
RefWndButton fnRefWndButton = (RefWndButton)(void**)0x425E20;

//KWndAutoPlay* Address1;
//KWndWindow* gWndWindow = (KWndWindow*)0x63DB6C;
//KWndImage* gWndImage = (KWndImage*)0x63CF4C;
//KWndButton* gWndButton = (KWndButton*)0x63CBCC;
//using KWndWindowBase = 0x67C03C;
//KWndWindow gWndWindow;
//KWndImage gWndImage;
//KWndButton gWndButton;

TempWindow g_TempWindow = *((TempWindow*)CreateWndWindow(new TempWindow));
TempImage g_TempImage = *((TempImage*)CreateWndImage(new TempImage));
TempButton g_TempButton = *((TempButton*)CreateWndButton(new TempButton));
 
//Offset size KWndWindow : 340, 
//Offset size KWndButton : 592
//Offset size KWndLabeledTextButton : 812
//Offset size KWndLabeledButton : 768
//Offset size KWndImage : 480, 440
// Offset size KWndText32 : 504, 

//using gWndWindow = KWndWindow;

//#define DYNAMIC_CALL_DO_STH(Type) \
//    if(auto *obj = dynamic_cast<Type const *>(uObj.get())) \
//        T_obj.do_sth(*obj)
//
//#undef DYNAMIC_CALL_DO_STH

//DWORD* gAutoPlay;
//struct WndWindow
//{
//	//WndWindow();
//	virtual ~WndWindow() = 0;
//};
//
//struct KWndImage
//{
//	//KWndImage();
//	virtual ~KWndImage() = 0;
//};
//
//struct KWndButton
//{
//	//KWndButton();
//	virtual ~KWndButton() = 0;
//};

void InstallObject(void* base, const KWndWindow& object)
{
	base = (void*)&object;
	unsigned int baseVTable = *(unsigned int*)base;
	unsigned int baseVTable2 = baseVTable + 0x4;
	unsigned int objectVTable = *(unsigned int*)&object;
	unsigned int objectVTable2 = objectVTable + 0x4;
	*((DWORD*)&object) = *reinterpret_cast<DWORD*>(base);
	//(*(KWndWindow*)(object->first)()) = *(KWndWindow*)(*(unsigned int*)(objectVTable2));
	*((unsigned int**)baseVTable2) = &objectVTable2;
	//secondFun bSecond = (secondFun)(*(unsigned int*)(adressVTable2));
	//int f = bFirst(object);
	//int s = bSecond(object, x);
	//printf("First: %d, second: %d", f, s);
}

void InstallObject2(void* base, KWndWindow* object)
{
	//auto* object = dynamic_cast<KWndWindow const*>((KWndWindow*)base);
}

//int makeObject(void** obj) {
//	*obj = new KWndWindow;
//	if (*obj == nullptr) {
//		return 1;
//	}
//	return 0;
//}

//KWndWindow* gWndWindow = (KWndWindow*)0x63DB6C;
//KWndImage* gWndImage = (KWndImage*)0x63CF4C;
//KWndButton* gWndButton = (KWndButton*)0x63A66C;
//KWndWindow* gWndWindow = (KWndWindow*)0x63DB6C;
//KWndWindow gWndWindow;
//KWndImage gWndImage;
//KWndButton gWndButton;
//void (*KWndWindow::* KWndWindow)(void);
//typedef KWndWindow* (*KWndWindow)(void);
//KWndWindow = 0x57B080;
//pfn = (KWndWindow*)0x441A60;
//void* KWndWindow = (void*)0x63DB6C;

//void* DisMember(size_t size, ...)
//{
//	if (size != sizeof(void*)) return NULL;
//	va_list args;
//	va_start(args, size);
//	void* res = va_arg(args, void*);
//	va_end(args);
//	return res;
//}
//
//void* anything = DisMember(sizeof((void*)0x63DB6C), &gWndWindow);

//void InitWindow(KWndWindow* ptr) {
//	//ptr = &g_WndAutoPlay; // Legal.
//	gWndWindow = dynamic_cast<KWndWindow*>(ptr);
//}
//
//void InitImage(KWndImage* ptr) {
//	//ptr = &g_WndAutoPlay.m_BtnAutoPlay; // Legal.
//	gWndImage = dynamic_cast<KWndImage*>(ptr);
//}
//
//void InitButton(KWndButton* ptr) {
//	//ptr = &g_WndAutoPlay.m_BtnAutoPlay; // Legal.
//	gWndButton = dynamic_cast<KWndButton*>(ptr);
//}

int	NewUiInit()
{
	//KUiGameOption::OpenWindow();
	//g_pRepresentShell = (iRepresentShell*)0x6DE30C;
	//DWORD pid = GetCurrentProcessId();

	//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//unsigned int* gButton1 = (unsigned int*)0x161ECB0;

	//DWORD pid = GetCurrentProcessId();

	//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//g_WndAutoPlay = new KWndAutoPlay;
	//KWndWindow* g_WndAutoPlay = new KWndAutoPlay;

	//KWndWindow* m_WndAutoPlay = &g_WndAutoPlay;

	//Address2 = (Player_AutoPlay2*)VirtualAllocEx(hProcess, NULL, 1024, 1052672, PAGE_EXECUTE_READWRITE);
	 
	//======================================================================

	//KWndWindow* base_Window = new KWndWindow;

	////RefCountBaseClass(base_Window, 340, 1, fnRefWndWindow, fnRefWndBase);

	//KWndImage* base_Image = new KWndImage;
	//base_Window = base_Image;

	////RefCountBaseClass(base_Window, 440, 1, fnRefWndImage, fnRefWndBase);
	//
	//KWndButton* base_Button = new KWndButton;
	//base_Window = base_Button;

	////RefCountBaseClass(base_Window, 592, 1, fnRefWndButton, fnRefWndBase);

	//delete base_Window;

	//delete base_Image;

	//delete base_Button;

	//KWndWindow* gWndWindow = (KWndWindow*)CreateWndWindow((DWORD*)gWndWindow);
	//CreateWndImage((DWORD*)gWndImage);
	//CreateWndButton((DWORD*)gWndButton);

	//void* vptr_WndButton = (BYTE*)(unsigned int*)(void*)0x63CBCC;

	//vptr_WndWindow = static_cast<KWndWindow*>(gWndWindow);

	//Player_AutoPlay2* Address1 = (Player_AutoPlay2*)operator new(1024);

	//DWORD oldProtection1, oldProtection2, oldProtection3;

	//void* vptr_WndButton = (BYTE*)(unsigned int*)(void*)0x63CBCC;

	//VirtualProtect(Address1, 1024, PAGE_EXECUTE_READWRITE, &oldProtection3);

	//*(void**)Address1 = (void*)operator new(1024);

	////vptr_WndButton = (void*)0x161ECB0;

	////gWndImage = *(KWndImage*)VirtualAllocEx(hProcess, NULL, 440, 1052672, PAGE_EXECUTE_READWRITE);

	////gWndButton = *(KWndButton*)VirtualAllocEx(hProcess, NULL, 592, 1052672, PAGE_EXECUTE_READWRITE);

	////CreateWndWindow((BYTE*)(void*)0x161ECB0);
	////CreateWndImage((unsigned int*)(void*)0x161ECB0);

	//Address2 = new Player_AutoPlay2;

	////RefCountBaseClass((void*)m_gButton1, 592, 1, (void*)RefWndButton, (void*)RefWndBase);

	//KIniFile	Ini;

	//if (Ini.Load("\\Ui\\Ui3\\工具控制条.ini"))
	//{
	//	KWndButton_Init(Address1, &Ini, "AutoPlay2");
	//}
	////LoadScheme("\\Ui\\Ui3\\AutoPlay_icon.ini");
	////LoadScheme("\\Ui\\Ui3\\AutoPlay_icon.ini");
	////*(DWORD*)(this + 4) &= ~WND_S_VISIBLE;

	//b_Wnd_AddWindow(Address1, 1);

	//VirtualProtect((void*)0x161ECB0, 2000, oldProtection3, 0);

	//======================================================================
	 
	//void* vptr_UiAutoPlayIcon = (void*)(unsigned int*)0x6DE0EC;
	//void* vptr_UiAutoPlayIcon = (void*)(unsigned int*)0x161ECB0;
	//void* vptr_WndImage = (void*)(unsigned int*)0x659D40;
	//void* vptr_WndButton = (void*)(unsigned int*)0x659B94;
	//void* vptr_WndWindow = (void*)(unsigned int*)0x65A50C;
	//using KWndWindow = decltype([] {});
	//void* vptr_WndWindow = (void*)0x67C03C;

	//void* vptr_WndWindow = (void*)0x65A50C;

	//static_cast<KWndWindow&>(*((KWndWindow*)vptr_WndWindow));

	//InstallObject(vptr_WndWindow, (const KWndWindow&)g_WndAutoPlay);

	//vptr_WndWindow = &g_WndAutoPlay;

	//void* vptr_WndImage = (void*)0x63CF4C;

	//vptr_WndImage = gWndImage;

	//void* vptr_WndButton = (void*)0x63CBCC;

	//vptr_WndButton = &g_WndAutoPlay->m_BtnAutoPlay;
	

	//using KWndWindow = KWndWindow;

	//*g_WndAutoPlay = *dynamic_cast<KWndAutoPlay*>((KWndWindow*)vptr_WndWindow);
	//g_WndAutoPlay->m_BtnAutoPlay = dynamic_cast<KWndButton&>((KWndButton&)vptr_WndButton);
	 
	//gWndWindow = static_cast<KWndWindow*>((KWndWindow*)0x65A50C);

	//gWndImage = static_cast<KWndImage*>((KWndImage*)0x659D40);

	//gWndButton = static_cast<KWndButton*>((KWndButton*)0x659B94);

	/*CreateWndWindow((unsigned int*)gAutoPlay);*/
	//KWndWindow* gWndWindow = static_cast<KWndWindow*>(gWndWindow);
	//void* anything = DisMember(sizeof(gWndWindow), (void*)0x63DB6C);
	//void* anything1 = HookVTableFunction((void*)0x63DB6C, gWndWindow, 0);
	//void* anything2 = HookVTableFunction((void*)0x63DB6C, gWndWindow + 8, 8);
	//void* anything3 = HookVTableFunction((void*)0x63DB6C, gWndWindow->Init, 8);
	//void* anything4 = HookVTableFunction((void*)0x63DB6C, gWndWindow->WndProc, 12);
	//void* anything5 = HookVTableFunction((void*)0x63DB6C, gWndWindow->PaintWindow, 16);
	// 0x67C044 0x67C03C
	
	//ofstream myfile;
	//myfile.open("E:\\Size.txt");
	//myfile << "anything1 " << anything1 << "\n"; // skill id
	//myfile << "anything2 " << anything1 << "\n"; // skill id
	//myfile.close();
	//void* vptr_KWndWindow_Base = **(BYTE***)0x63DB6C; // vtable

	//void* vptr_KWndImage_Base = *(unsigned int**)0x63CF4C; // vtable

	//void* vptr_KWndButton_Base = (void*)0x63CBCC; // vtable

	//void* vptr_KWndWindow = new KWndWindow;

	//void* vptr_KWndImage = new KWndImage;

	//void* vptr_KWndButton = new KWndButton;

	//void* m_KWndWindow = dynamic_cast<KWndWindow*>((KWndWindow*)vptr_KWndImage);

	//void* m_KWndImage = dynamic_cast<KWndImage*>((KWndImage*)vptr_KWndButton);

	//vptr_KWndButton_Base = dynamic_cast<KWndButton*>((KWndButton*)vptr_KWndButton);

	//unsigned int* vptr_Window = (unsigned int*)(void*)0x659078;
	//vptr_Window = reinterpret_cast<unsigned int*>(&gWndWindow);

	//unsigned int* vptr_Image = (unsigned int*)(void*)0x659040;
	//vptr_Image = reinterpret_cast<unsigned int*>(&gWndImage);

	//unsigned int* vptr_Button = (unsigned int*)(void*)0x659434;
	//vptr_Button = reinterpret_cast<unsigned int*>(&gWndButton);

	//InitWindow((KWndWindow*)(void*)0x659078); // 171
	//InitImage((KWndImage*)(void*)0x659040); // 136
	//InitButton((KWndButton*)(void*)0x659434); // 27
	//HMODULE hModule = GetModuleHandle(NULL);

	//void* vptr_WndButton = (BYTE*)(unsigned int*)(void*)0x63CBCC;

	//void* vptr_WndImage = (unsigned int*)(void*)0x63CF4C;

	//void* vptr_WndWindow = (void*)0x63DB6C;

	//vptr_UiAutoPlayIcon = new UiAutoPlayIcon;

	//vptr_WndWindow = &vptr_UiAutoPlayIcon;

	//g_WndAutoPlay = (KWndAutoPlay*)NewAddressData3;

	//gAutoPlay = (Player_AutoPlay*)NewAddressData3;

	//CreateWndWindow((BYTE*)&NewAddressData3);

	//CreateWndButton((unsigned int*)&gAutoPlay);

	//vptr_KWndWindow_Base = dynamic_cast<KWndWindow*>((KWndWindow*)new KWndWindow);

	//vptr_KWndImage_Base = dynamic_cast<KWndImage*>((KWndImage*)new KWndImage);

	//vptr_KWndButton_Base = dynamic_cast<KWndButton*>((KWndButton*)new KWndButton);

	//ReleaseWndWindow((unsigned int*)(void*)gWndWindow);

	//ReleaseWndWindow((unsigned int*)(void*)gWndImage);

	//ReleaseWndWindow((unsigned int*)(void*)gWndButton);

	//RefCountBaseClass((BYTE*)(unsigned int*)(void*)gWndWindow, 340, 1, (void*)RefWndWindow, (void*)RefWndBase);

	//RefCountBaseClass((unsigned int*)(void*)gWndImage, 440, 1, (void*)RefWndImage, (void*)RefWndBase);

	//RefCountBaseClass((void*)gWndButton, 592, 1, (void*)RefWndButton, (void*)RefWndBase);

	//vptr_WndWindow = &gWndWindow;

	//vptr_WndImage = &gWndImage;

	//vptr_WndButton = &gWndButton; // 0x43CB10
	 
	//vptr_WndWindow = &m_pSelf;

	//vptr_WndWindow = &NewAddressData3;

	//vptr_WndButton = &m_pSelf->m_BtnAutoPlay;

	//vptr_WndButton = &gAutoPlay;

	//gAutoPlay = dynamic_cast<Player_AutoPlay*>((Player_AutoPlay*)(void*)0x63CBCC);

	//Foo2(*(void**)&vptr_Button);

	//window_ptr = image_ptr;
	//image_ptr = button_ptr;

	//Player_AutoPlay::RegisterSelfClass();

	//void (*EntryProc)(void) = ((void(*)(void)) 0x659078);

	//void* vptr_KWndWindow_Base = (void*)0x659078; // base class

	//0x63CBCC

	//Player_AutoPlay myClass;

	//Player_AutoPlay myClass;
	
	//vptr_KWndButton_Base = &myClass;

	//CreateWndButton((unsigned int*)&m_pSelf->m_BtnAutoPlay);
	 
	//CreateWndWindow((BYTE*)((unsigned int*)(&gAutoPlay)));

	//CreateWndButton(&gAutoPlay);

	//ofstream myfile;
	//myfile.open("E:\\NewUiInit.txt");
	//myfile << "EntryProc " << EntryProc << "\n"; // skill id
	//myfile << "myClass " << myClass << "\n"; // skill id
	////myfile << "pWnd " << EntryProc << "\n"; // skill id
	////myfile << "pWnd " << anything4 << "\n"; // skill id
	////myfile << "pWnd " << anything5 << "\n"; // skill id
	//myfile.close();

	//void* vTableClass = reinterpret_cast<void*>(&myClass);

	//void* vTablePtr = reinterpret_cast<void**>(EntryProc);

	//DWORD oldProtection1, oldProtection2, oldProtection3;

	//*((KWndWindow*)&gWndImage) = *reinterpret_cast<KWndWindow*>(&gWndWindow); // reinterpret_cast static_cast dynamic_cast

	//*((KWndImage*)&gWndButton) = *reinterpret_cast<KWndImage*>(&gWndImage);

	//CreateWndButton((unsigned int*)&gWndButton);

	//vptr_WndButton = reinterpret_cast<KWndButton*>(&gWndButton);

	//VirtualProtect(gWndWindow, 2000, PAGE_EXECUTE_READWRITE, &oldProtection1);

	//VirtualProtect(gWndImage, 2000, PAGE_EXECUTE_READWRITE, &oldProtection2);

	//VirtualProtect(gWndButton, 2000, PAGE_EXECUTE_READWRITE, &oldProtection3);

	//vTableClass = (Player_AutoPlay*)reinterpret_cast<Player_AutoPlay*>(vptr_KWndButton_Base);

	//vptr_KWndButton_Base = &vTableClass;

	//vptr_WndWindow = g_WndAutoPlay;

	//vptr_WndButton = &g_WndAutoPlay->m_BtnAutoPlay;

	//vptr_WndWindow = &g_WndAutoPlay->m_BtnAutoPlay;

	//VirtualProtect(gWndWindow, 2000, oldProtection1, 0);

	//VirtualProtect(gWndImage, 2000, oldProtection2, 0);

	//VirtualProtect(gWndButton, 2000, oldProtection3, 0);
	
	

	//HookVTableFunction(gWndWindow + 40, (void*)0x63DB6C, 40);
	//HookVTableFunction(gWndWindow + 44, (void*)0x63DB6C, 44);
	//HookVTableFunction(gWndWindow + 48, (void*)0x63DB6C, 48);
	//HookVTableFunction(gWndImage + 8, (void*)0x63CF4C, 8);
	//HookVTableFunction(gWndButton, (void*)(void*)(void*)0x63A66C, 0);
	//HookVTableFunction(gWndButton + 8, (void*)(void*)(void*)0x63A66C, 8);
	//HookFunction(anything, (void*)0x63A66C);

	//KComClassInfo m_one;
	//strcpy(m_one.szClassType, "Player_AutoPlay")

	//KComClassFactory* This = _ComClassFactory();

	//_CreateComObject(This, "KWndAutoPlay")->pfnCreateObject;

	//ReleaseWndWindow((unsigned int*)(KWndWindow*)Player_AutoPlay::CreateObject);
	 
	//CreateWndImage((unsigned int*)&Player_AutoPlay::CreateObject);
	//CreateWndButton((unsigned int*)&Player_AutoPlay::CreateObject);

	//void* vptr_WndWindow = (void*)0x63DB6C;

	//gWndWindow = new KWndWindow;

	//vptr_WndWindow = gWndWindow/*;*/

	//*((KWndWindow*)gWndWindow) = *dynamic_cast<KWndWindow*>((KWndWindow*)0x63DB6C);
	//(*(KWndWindow*)&gWndWindow) = *dynamic_cast<KWndWindow*>((KWndWindow*)0x63DB6C);
	//dynamic_cast<KWndWindow*>((KWndWindow*)(void*)0x63DB6C)->~KWndWindow();

	//*(this) = *static_cast<KWndWindow*>((KWndWindow*)vptr_WndWindow);

	//BYTE* window_ptr = (BYTE*)0x63DB6C;
	//window_ptr = (BYTE*)this;
	//gWndWindow = reinterpret_cast<KWndWindow*>(window_ptr);

	//Player_AutoPlay* button1 = (Player_AutoPlay*)0x659434;

	//CreateWndButton((DWORD*)new Player_AutoPlay);

	/*KComClassInfo pComAutoPlay;
	strcpy(pComAutoPlay.szClassType, "Player_AutoPlay");
	pComAutoPlay.szClassType[32] = 0;
	pComAutoPlay.pfnCreateObject = Player_AutoPlay::CreateObject;
	_RegisterComClass(&pComAutoPlay);*/

	////size_t* vptr = *(size_t**)(void*)0x63CBCC;
	////((void (*)()) vptr[0])();
	ofstream myfile;
	myfile.open("E:\\Size.txt");
	/*myfile << "Size KWndWindow " << sizeof(KWndWindow) << "\n";
	myfile << "Size KWndImage " << sizeof(KWndImage) << "\n";
	myfile << "Size KWndButton " << sizeof(KWndButton) << "\n";
	myfile << "Size KWndButton " << sizeof(KWndText32) << "\n";*/

	//myfile << "Size KWndWindow " << sizeof(WndWindow) << "\n";
	//myfile << "Size KWndImage " << sizeof(WndImage) << "\n";
	//myfile << "Size KWndButton " << sizeof(WndButton) << "\n";
	//myfile << "Size KWndButton " << sizeof(KWndText32) << "\n";

	//myfile << "Size Player_AutoPlay " << sizeof(Player_AutoPlay) << "\n"; // skill id
	//myfile << "Size Player_AutoPlay " << sizeof(Player_AutoPlay2) << "\n"; // skill id
	//myfile << "Size unsigned int " << sizeof(unsigned int) << "\n"; // skill id
	//myfile << "Size int " << sizeof(int) << "\n"; // skill id
	//myfile << "Size unsigned char " << sizeof(unsigned char) << "\n"; // skill id
	//myfile << "Size char " << sizeof(char) << "\n"; // skill id
	//myfile << "Size Player_AutoPlay " << sizeof(vptr) << "\n"; // skill id
	//myfile << "Size Player_AutoPlay " << vptr << "\n"; // skill id
	//myfile << "Size Player_AutoPlay " << sizeof(*(DWORD**)((void*)0x63CBCC)) << "\n"; // skill id
	//myfile << "Size Player_AutoPlay " << *(DWORD**)((void*)0x63CBCC) << "\n"; // skill id
	//myfile << "Size Player_AutoPlay " << sizeof((void*)0x63CBCC) << "\n"; // skill id
	myfile.close();

	//KWndAutoPlay::OpenWindow();

	//CreateWndButton((unsigned int*)gAutoPlay);
	//Player_AutoPlay::RegisterSelfClass();
	//Player_AutoPlay* pMyClass = const_cast<Player_AutoPlay*>(static_cast<const Player_AutoPlay*>(&Player_AutoPlay::KWndWindow()));
	//_RegisterComClass(pMyClass);
	//Player_AutoPlay::RegisterSelfClass();
	//Player_AutoPlay::OpenWindow();
	//KUiAutoPlay::OpenWindow();
	//KAutoPlay::OpenWindow();
	//KWndAutoPlay::OpenWindow();
	//KUiAutoPlayButton::OpenWindow();
	//KUiAutoPlayButton::RegisterSelfClass();
	//This()->CreateComObject("Player_AutoPlay");
	//This->CreateComObject("Player_AutoPlay");
	
	
	return true;
}

//void __fastcall HookOnButtonClick(short* This)
//{
//	
//	ofstream myfile;
//	myfile.open("E:\\KWndButtonAutoPlay.txt");
//	myfile << "This " << (WORD*)This << "\n"; // skill id
//	//myfile << "This " << szNumber << "\n"; // skill id
//	myfile.close();
//
//	//short* result;
//
//	//result = This;
//	//This[211] = szNumber;
//	//return result;
//
//	//return This;
//
//	//return SafeCallOnButtonClick(This, szNumber);
//
//}

void HookLoadSchemeForEachWnd(const char* This)
{
	KWndAutoPlay::LoadScheme("\\Ui\\Ui3\\AutoPlay_icon.ini");
	SafeCallLoadSchemeForEachWnd(This);
}

int HookUiArenaIcon()
{

	//DWORD pid = GetCurrentProcessId();

	//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	 
	//*((KWndAutoPlay***)g_WndAutoPlay) = (KWndAutoPlay**)VirtualAllocEx(hProcess, NULL, 848, 1052672, MEM_COMMIT);

	//CreateWndWindow(g_WndAutoPlay);

	//42652C 420A0E
	//KWndButton button_base = *(KWndButton*)0x63CBCC;
	//SafeCallOnLBtnDown((int)0x42652C, bDoubleClick);
	//ofstream myfile;
	//myfile.open("E:\\KWndButtonAutoPlay.txt");
	//myfile << "This " << (WORD*)This << "\n"; // skill id
	//myfile << "This " << bDoubleClick << "\n"; // skill id
	//myfile << "This " << *((DWORD*)This + 24) << "\n"; // skill id
	//myfile << "This " << This[220] << "\n"; // skill id
	//myfile.close();

	/*void* baseClass_WndWindow = (void*)0x659078;

	baseClass_WndWindow = (void*)gWndWindow;

	void* baseClass_WndImage = (void*)0x659040;

	baseClass_WndImage = (void*)gWndImage;

	void* baseClass_WndButton = (void*)0x659434;

	baseClass_WndButton = (void*)gWndButton;*/

	//gWndWindow = **((KWndWindow**)0x659078);
	//gWndImage = **((KWndImage**)0x659040);
	//gWndButton = **((KWndButton**)0x659434);

	//RefCountBaseClass(gWndWindow, 340, 1, fnRefWndWindow, fnRefWndBase);

	//KWndImage* base_Image = new KWndImage;
	//base_Window = base_Image;

	//RefCountBaseClass(gWndImage, 440, 1, fnRefWndImage, fnRefWndBase);
	//
	//KWndButton* base_Button = new KWndButton;
	//base_Window = base_Button;

	//RefCountBaseClass(gWndButton, 592, 1, fnRefWndButton, fnRefWndBase);

	ofstream myfile;
	myfile.open("E:\\Size.txt");
	//myfile << "Size KWndWindow " << sizeof(KWndWindow) << "\n"; // skill id
	//myfile << "Size KWndImage " << sizeof(KWndImage) << "\n"; // skill id
	//myfile << "Size KWndButton " << sizeof(KWndButton) << "\n"; // skill id

	//myfile << "Size KWndWindow " << sizeof(WndWindow) << "\n"; // skill id
	//myfile << "Size KWndImage " << sizeof(WndImage) << "\n"; // skill id
	//myfile << "Size KWndButton " << sizeof(WndButton) << "\n"; // skill id
	

	myfile << "Size KWndWindow " << sizeof(TempWindow) << "\n"; // skill id
	myfile << "Size KWndImage " << sizeof(TempImage) << "\n"; // skill id
	myfile << "Size KWndButton " << sizeof(TempButton) << "\n"; // skill id

	//myfile << "Size Player_AutoPlay " << sizeof(KWndWindow) << "\n"; // skill id
	//myfile << "Size Player_AutoPlay " << sizeof(KWndImage) << "\n"; // skill id
	//myfile << "baseClass_WndWindow " << (DWORD*)baseClass_WndWindow << "\n"; // skill id
	//myfile << "baseClass_WndImage " << (DWORD*)baseClass_WndImage << "\n"; // skill id
	//myfile << "baseClass_WndButton " << (DWORD*)baseClass_WndButton << "\n"; // skill id

	//myfile << "baseClass_WndWindow " << (DWORD*)new KWndWindow() << "\n"; // skill id
	//myfile << "baseClass_WndImage " << (DWORD*)new KWndImage() << "\n"; // skill id
	//myfile << "baseClass_WndButton " << (DWORD*)new KWndButton() << "\n"; // skill id

	//myfile << "gWndWindow " << (DWORD*)&gWndWindow << "\n"; // skill id
	//myfile << "gWndImage " << (DWORD*)&gWndImage << "\n"; // skill id
	//myfile << "gWndButton " << (DWORD*)&gWndButton << "\n"; // skill id
	 
	myfile << "gWndWindow " << (DWORD*)&g_TempWindow << "\n"; // skill id
	myfile << "gWndImage " << (DWORD*)&g_TempImage << "\n"; // skill id
	myfile << "gWndButton " << (DWORD*)&g_TempButton << "\n"; // skill id

	myfile.close();
	//makeObject(((void**)0x67C03C));
	//delete baseClass_WndWindow;
	//delete baseClass_WndImage;
	//delete baseClass_WndButton;

	//void** vtable = *((void***)(((char*)&g_TempWindow) + 0));

	//CreateWndWindow2(&g_TempWindow);
	//CreateWndImage(&g_TempImage);
	//CreateWndButton(&g_TempButton);
	 
	//RefCountBaseClass(&g_TempWindow, 256, 1, (void(__thiscall*)(int))0x441A60, (void(__thiscall*)(int))0x44ADC0);
	//RefCountBaseClass(&g_TempImage, 440, 1, (void(__thiscall*)(int))0x42C010, (void(__thiscall*)(int))0x44ADC0);
	//RefCountBaseClass(&g_TempButton, 592, 1, (void(__thiscall*)(int))0x425E20, (void(__thiscall*)(int))0x44ADC0);

	//*((void**)&g_TempWindow) = &*(static_cast<void**>(*((void**)0x63DB6C)));
	//*((void**)&g_TempImage) = &*(static_cast<void**>(*((void**)0x63CF4C)));
	//*((void**)&g_TempButton) = &*(static_cast<void**>(*((void**)0x63CBCC)));

	g_TempWindow = static_cast<TempWindow&>((TempWindow&)*((BYTE**)0x63DB6C)); // 0x659078
	g_TempImage = static_cast<TempImage&>((TempImage&)*((DWORD**)0x63CF4C)); // 0x659040
	g_TempButton = static_cast<TempButton&>((TempButton&)*((WORD**)0x63CBCC)); // 0x659434

	//reinterpret_cast<TempWindow*>(*((void**)0x441A60));
	
	KWndAutoPlay::OpenWindow();

	return SafeCallUiArenaIcon();
}

void HookOnLBtnDown(bool bDoubleClick, DWORD* This)
{
	//42652C 420A0E
	//KWndButton button_base = *(KWndButton*)0x63CBCC;
	//SafeCallOnLBtnDown((int)0x42652C, bDoubleClick);
	//ofstream myfile;
	//myfile.open("E:\\KWndButtonAutoPlay.txt");
	//myfile << "This " << (WORD*)This << "\n"; // skill id
	//myfile << "This " << bDoubleClick << "\n"; // skill id
	//myfile << "This " << *((DWORD*)This + 24) << "\n"; // skill id
	//myfile << "This " << This[220] << "\n"; // skill id
	//myfile.close();
}

int HookAddOneWindow(const void* This, KIniFile* pIniFile, const char* pSection)
{
	int result = SafeAddOneWindow(This, pIniFile, pSection);

	/*KIniFile	Ini;

	if (Ini.Load("\\Ui\\Ui3\\AutoPlay_icon.ini"))
	{
		gAutoPlay.Init(&Ini, "AutoPlay");
		gAutoPlay.AddBrother((KWndWindow*)0x67D6E0);
	}

	Ini.Clear();*/

	return result;
}

int HookUiStart(bool bStartMusic, bool bJustLaunched)
{
	int result = HookUiInitOpenWindow(bStartMusic, bJustLaunched);

	/*KWndWindow* vptr_WndWindow = ((KWndWindow*)0x65A50C);

	vptr_WndWindow = gWndWindow;

	vptr_WndWindow = static_cast<KWndWindow*>(gWndWindow);*/

	NewUiInit();

	return result;
}

int __cdecl HookRegisterComClass(char* Str1)
{
	return _RegisterComClass((void*)Str1);
}

int __cdecl HookUiGameMain(unsigned int uMsg, int uParam, int nParam)
{
	int nRet = 0;

		ofstream myfile;
		myfile.open("E:\\HookUiGameMain.txt");
		myfile << "uMsg " << uMsg << "\n"; // skill id
		myfile << "uParam " << (int*)uParam << "\n"; // skill id
		myfile << "nParam " << nParam << "\n"; // skill id
		//myfile << "result " << result << "\n"; // skill id
		myfile.close();
		
	//HookWindowProc((unsigned int*)gWndWindow, uMsg, uParam, nParam);
	//HookButtonProc((unsigned int*)(KWndButton*)0x63A66C, uMsg, uParam, nParam);
		//gWndButton->WndProc(uMsg, uParam, nParam);

	switch (uMsg)
	{
	case 132:
		//nRet = CallUiGameSpaceProc(uMsg, uParam, nParam);
		//KWndAutoPlay::GetIfVisible()->OnClickButton((KWndButton*)(KWndWindow*)&This);
		//nRet = KWndAutoPlay::GetIfVisible()->WndProc(uMsg, uParam, nParam);
		break;
	default:
		nRet = CallUiGameSpaceProc(uMsg, uParam, nParam);
		//gWndWindow->WndProc(uMsg, uParam, nParam);
		//g_WndAutoPlay.m_BtnAutoPlay.WndProc(uMsg, uParam, nParam);
		//g_WndAutoPlay.WndProc(uMsg, uParam, nParam);
		break;
	}
	//int result = HookGlobalProc(This, uMsg, uParam, nParam);
	//
	//int result = HookButtonProc(This, uMsg, uParam, nParam);

	return nRet;
}

void HookUiInitProc(unsigned int* pWnd, unsigned int* This)
{
	//ofstream myfile;
	//myfile.open("E:\\KWndWindowAutoPlay.txt");
	//myfile << "This " << (DWORD*)This << "\n"; // skill id
	//myfile << "uMsg " << *((DWORD*)pWnd) << "\n"; // skill id
	//myfile << "uMsg " << (KWndButton*)(KWndWindow*)pWnd << "\n"; // skill id
	//myfile.close();

	CallUiInitProc(This, pWnd);
}

int HookWindowWndProc(unsigned int uMsg, unsigned int uParam, int nParam, unsigned int* This)
{
	int nRet = 0;
	//ofstream myfile;
	//myfile.open("E:\\KWndWindowAutoPlay.txt");
	//myfile << "This " << (DWORD*)This << "\n"; // skill id
	//myfile << "uMsg " << uMsg << "\n"; // skill id
	//myfile << "uParam " << uParam << "\n"; // skill id
	//myfile << "nParam " << nParam << "\n"; // skill id
	////myfile << "result " << result << "\n"; // skill id
	//myfile.close();

	//int result = CallUiSysMsgCentreProc(This, uMsg, uParam, nParam);

	//int result = HookWindowProc(This, uMsg, uParam, nParam);

	switch (uMsg)
	{
		case 132:
			//KWndAutoPlay::GetIfVisible()->OnClickButton((KWndButton*)(KWndWindow*)This);
			break;
		default:
			//nRet = KWndAutoPlay::GetIfVisible()->WndProc(uMsg, uParam, nParam);
			break;
	}

	//nRet = CallUiToolsControlBarProc(This, uMsg, uParam, nParam);

	//if ((DWORD)This == 0x10A0E)
	//{
	//	if (uMsg == WND_N_BUTTON_CLICK)
	//	{
	//		KWndAutoPlay::OnClickButton((KWndButton*)This);
	//	}
	//	//HookButtonWndProc(uMsg, uParam, nParam, This);
	//}

	//if ((DWORD)This == 0x10A0E)
	//{
	//	KWndAutoPlay::OnClickButton((KWndButton*)This);
	//	//HookButtonWndProc(uMsg, uParam, nParam, This);
	//}
	
	return nRet;
}

int __fastcall HookButtonWndProc(unsigned int uMsg, int uParam, int nParam, unsigned int* This)
{
	int nRet = 0;

	//if (uMsg == 132)
	//{
	//	//ofstream myfile;
	//	//myfile.open("E:\\KWndButtonAutoPlay.txt");
	//	//myfile << "This " << (DWORD*)This << "\n"; // skill id
	//	//myfile << "This " << *(DWORD*)This << "\n"; // skill id
	//	//myfile << "This " << This + 64 << "\n"; // skill id
	//	//myfile << "uMsg " << uMsg << "\n"; // skill id
	//	//myfile << "uParam " << (int*)uParam << "\n"; // skill id
	//	//myfile << "nParam " << nParam << "\n"; // skill id
	//	////myfile << "result " << result << "\n"; // skill id
	//	//myfile.close();

	//	//if (*(DWORD*)This == 0x3A22650)
	//	//if (*(DWORD*)This == 0x694DB8)
	//	//{
	//	//	if (KUiAutoPlay::GetIfVisible())
	//	//		KUiAutoPlay::CloseWindow(false);
	//	//	else
	//	//		KUiAutoPlay::OpenWindow();
	//	//	return 1;
	//	//	//KWndAutoPlay::OnClickButton((KWndButton*)(KWndWindow*)This);
	//	//}
	//	
		//nRet = KWndAutoPlay::GetIfVisible()->WndProc(uMsg, uParam, nParam);
	//}
	//BA02C9
	//9F02C5
	ofstream myfile;
	myfile.open("E:\\KWndButtonAutoPlay1.txt");
	//myfile << "This " << (KWndButton*)(KWndImage*)(KWndWindow*)&This << "\n"; // skill id
	//myfile << "This " << (DWORD*)This << "\n"; // skill id
	//myfile << "uMsg " << uMsg << "\n"; // skill id
	//myfile << "nParam " << (KWndButton*)(KWndImage*)(KWndWindow*)nParam << "\n";
	//myfile << "nParam " << (DWORD*)(KWndButton*)(KWndImage*)(KWndWindow*)nParam << "\n";
	////unsigned int nNumber2 = (*(DWORD*)&This + 0x1C8);
	////myfile << "pWnd " << *(DWORD**)nNumber2 << "\n"; // skill i
	//myfile << "pWnd " << *(DWORD**)&This << "\n"; // skill i
	//myfile << "uParam " << (DWORD*)uParam << "\n";
	//myfile << "result " << (KWndButton*)(KWndImage*)(KWndWindow*)&nParam << "\n";
	//myfile << "result " << (DWORD*)(KWndButton*)(KWndImage*)(KWndWindow*)&nParam << "\n";
	myfile << "This 1 " << uMsg << "\n";
	myfile << "This 1 " << uParam << "\n";
	myfile << "This 1 " << nParam << "\n";
	//myfile << "This 1 " << *(DWORD*)This[125] << "\n";
	//myfile << "This 2 " << (DWORD*)(int)This << "\n";
	//myfile << "This 3 " << *(DWORD*)This[24] << "\n";
	myfile.close();

	//KWndAutoPlay::GetIfVisible()->OnClickButton((KWndButton*)(KWndWindow*)&This);

	switch (uMsg)
	{
	//case 132:
		//nRet = CallUiArenaProc(This, uMsg, uParam, nParam);
		//KWndAutoPlay::GetIfVisible()->OnClickButton((KWndButton*)(KWndWindow*)&This);
		//nRet = KWndAutoPlay::GetIfVisible()->WndProc(uMsg, uParam, nParam);
		//break;
	default:
		//nRet = CallUiArenaProc(This, uMsg, uParam, nParam);
		//nRet = CallUiActivityGuide(This, uMsg, uParam, nParam);
		//nRet = KWndWindow_WndProc(This, uMsg, uParam, nParam);
		//nRet = KWndWindow_WndProc(This, uMsg, uParam, nParam);
		break;
	}
	//int result = HookGlobalProc(This, uMsg, uParam, nParam);
	//
	//int result = HookButtonProc(This, uMsg, uParam, nParam);

	return nRet;
}

char HookOnLBtnDown(int x, int y, unsigned int* This)
{
	ofstream myfile;
	myfile.open("E:\\HookOnLBtnDown.txt");
	myfile << "This " << (DWORD*)This << "\n";
	myfile << "This " << (DWORD*)&This[24] << "\n";
	myfile.close();

	char result;

	result = (char)This;

	/*if (This[1] & WND_S_MOVEALBE)
	{
		This[16] = 1;
		This[17] = x;
		This[18] = y;
		return CallWnd_SetCapture((int)This, 1);
	}*/

	return result;
	//return CallOnLBtnDown((unsigned int*)This, x, y);
}

void	NewUiExit()
{

}

KWndAutoPlay* KWndAutoPlay::m_pSelf = NULL;

//Player_AutoPlay* m_BtnAutoPlay = (Player_AutoPlay*)0x612c87;
//Player_AutoPlay::KWndButton Player_AutoPlay::KWndButton = 0x425E20;

//KWndAutoPlay::KWndAutoPlay()
//{
//	//RefCountBaseClass(this, 340, 1, (BaseWndWindow)0x441A60, (RefWndBase)0x44ADC0);
//	//memset(&m_BtnAutoPlay, 0, sizeof(KWndButton));
//	//*((BYTE**)g_WndAutoPlay) = CreateWndWindow2(*((BYTE**)new KWndAutoPlay));
//	//*this = *static_cast<KWndAutoPlay*>(*((KWndWindow**)0x65A50C));
//
//	//*(void**)this = +((char**)((void***)0x63DB6C));
//	//gWndWindow = this;
//	//gWndButton = &this->m_BtnAutoPlay;
//	//KWndWindow* window = (KWndAutoPlay*)VirtualAllocEx(hProcess, NULL, 848, 1052672, MEM_COMMIT);
//	//*((BYTE*)this) = *CreateWndWindow((DWORD*)this);
//	//CreateWndWindow(&g_WndAutoPlay);
//	//CreateWndButton((DWORD*)(&this->m_BtnAutoPlay));
//	//*(this) = *dynamic_cast<KWndAutoPlay*>(window);
//	//void* vptr_WndWindow = (void*)0x659078;
//	//void* vptr_WndWindow = (void*)0x63DB6C;
//	//CreateWndWindow(*((char**)this));
//	//void* vtable_WndWindow = *((void**)0x63DB6C);
//
//	////void (*EntryProc)(void*) = ((void(*)(void*)) reinterpret_cast<void*>((void*)0x441A60));
//
//	////EntryProc(this);
//
//	//vtable_WndWindow = this;
//
//	//*vtable_WndWindow = static_cast<KWndAutoPlay>(*((KWndAutoPlay*)(void*)0x65A50C));
//
//	//*((KWndWindow*)this) = *dynamic_cast<KWndWindow*>((KWndWindow*)vptr_WndWindow);
//
//	//* (void**)this = +((char**)((void***)0x63DB6C));
//
//	//CreateWndImage((DWORD*)this);
//	//*((BYTE*)this) = *CreateWndWindow((DWORD*)gWndWindow);
//	//CreateWndWindow((DWORD*)gWndWindow);
//	//m_pSelf = NULL;
//
//	//void* vptr_WndButton = (unsigned char*)(void*)0x659434;
//
//	//void* vptr_WndWindow = (unsigned char*)(void*)0x659078;
//
//	//vptr_WndWindow = &m_pSelf;
//
//	//vptr_WndWindow = &m_pSelf;
//
//	//vptr_WndButton = &m_pSelf->m_BtnAutoPlay;
//
//	//vptr_WndButton = &m_pSelf + 428;
//
//	//m_pSelf = (KWndAutoPlay*)NewAddressData3;
//	//m_pSelf = this;
//
//	//vptr_WndWindow = dynamic_cast<KWndAutoPlay*>(this);
//
//	//m_pSelf = new KWndAutoPlay;
//
//	//gWndWindow = m_pSelf;
//
//	//vptr_WndButton = dynamic_cast<KWndButton*>(&m_BtnAutoPlay);
//	//CreateWndWindow((BYTE*)m_pSelf);
//	//CreateWndWindow((BYTE*)((unsigned int*)((void*)this)));
//	//*((DWORD*)&m_BtnAutoPlay) = *CreateWndButton((DWORD*)gWndButton);
//	//CreateWndWindow((unsigned int*)(KWndWindow*)this);
//	//CreateWndImage((unsigned int*)this);
//
//	
//	//Player_AutoPlay2* button_base = (Player_AutoPlay2*)0x659434;
//
//	//m_BtnAutoPlay = *(Player_AutoPlay2*)0x659434;
//
//	//KWndButton* button_array = (KWndButton*)0x659460;
//
//	//KWndButton* button_vtable = (KWndButton*)0x63CBCC;
//
//	////window_vtable = button_vtable;
//
//	//KWndWindow* window_base = (KWndWindow*)0x659078;
//
//	//KWndWindow* window_vtable = (KWndWindow*)0x63DB6C;
//
//	//KWndButton* button_base = (KWndButton*)0x659434;
//
//	//KWndButton* button_array = (KWndButton*)0x659460;
//
//	//KWndButton* button_vtable = (KWndButton*)0x63CBCC;
//
//	////window_vtable = button_vtable;
//
//	//button_base = &this->m_BtnAutoPlay;
//
//	//window_vtable = this;
//
//	//CreateWndButton((DWORD*)&m_BtnAutoPlay);
//
//	//((Player_AutoPlay2&)*((void**)0x659434)) = m_BtnAutoPlay;
//
//	//*(void**)this = reinterpret_cast<void*>(button_vtable);
//	//this = static_cast<void*>(button_array);
//
//	//Address2 = new Player_AutoPlay2;
//
//	//KWndButton* button_vtable = (KWndButton*)0x63CBCC;
//
//	//button_vtable = this;
//
//	//CreateWndWindow(*((DWORD**)this));
//
//	//CreateWndButton((DWORD*)this);
//
//	//*(void**)this = +((char**)((void**)0x63CBCC));
//
//	//void* vptr_WndWindow = (void*)0x63DB6C;
//	//void* button_ptr = (void*)0x63CBCC;
//
//	//*((Player_AutoPlay2**)0x659434) = reinterpret_cast<Player_AutoPlay2*>(&m_BtnAutoPlay);
//
//	//KWndWindow* vtable_WndWindow = (KWndWindow*)0x63DB6C;
//
//	//vtable_WndWindow = this;
//
//}

//KWndAutoPlay::~KWndAutoPlay()
//{
//	ReleaseWndWindow((unsigned int*)this);
//	//KWndWindow::~KWndWindow();
//	//this->~KWndAutoPlay();
//	//KWndWindow::~KWndWindow();
//	//KWndButton::~KWndButton();
//}

KWndAutoPlay* KWndAutoPlay::GetIfVisible()
{
	/*if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;*/
	return NULL;
}

KWndAutoPlay* KWndAutoPlay::OpenWindow()
{
	/*void* vptr_WndImage = (void*)0x63CF4C;
	KWndImage* WndImage;
	WndImage = (KWndImage*)vptr_WndImage;*/
	//void* vptr_WndImage = (DWORD (**)(...))(void*)0x63CF4C;

	if (m_pSelf == NULL)
	{
		//KWndAutoPlay*  m_pSelf1 = new KWndAutoPlay;
		//m_pSelf = (KWndAutoPlay*)operator new(340) + sizeof(KWndAutoPlay);
		//memset(m_pSelf, 0, sizeof(m_pSelf));
		
		//void* vptr_WndWindow = (unsigned char*)0x659078;

		//void* vptr_WndButton = (unsigned char*)0x659434;
		 
		//void* vptr_WndWindow = (void*)(unsigned char*)0x63DB6C;

		//KWndAutoPlay* v2;

		//CreateWndWindow((unsigned int*)&gAutoPlay);

		//CreateWndWindow((unsigned int*)gAutoPlay);

		/*auto vptr_WndWindow = &m_pSelf;

		unsigned int* objPtr = reinterpret_cast<unsigned int*>(&m_pSelf);

		auto derivedVTablePtr = reinterpret_cast<unsigned int*>(*objPtr);

		auto derivedFuncPtr = reinterpret_cast<void(*)(KWndAutoPlay*)>(*derivedVTablePtr);

		derivedFuncPtr(m_pSelf);*/

		//gAutoPlay = (DWORD*)new KWndAutoPlay;

		//m_pSelf = (KWndAutoPlay*)&NewAddressData3;

		//void* baseClass_Image = (void*)0x659040;

		//baseClass_Image = &m_pSelf->m_BtnAutoPlay;;

		//baseClass_Image = reinterpret_cast<unsigned int*>(&gWndImage);

		//void* baseClass_WndButton = (void*)0x659434;

		//baseClass_WndButton = &m_pSelf->m_BtnAutoPlay;

		//*(&m_pSelf->m_BtnAutoPlay) = (KWndButton&)static_cast<void*>(baseClass_WndButton);

		//CreateWndWindow(new TempWindow());

		//g_WndAutoPlay = new KWndAutoPlay();

		//m_pSelf = g_WndAutoPlay;

		//void* baseClass_WndWindow = (void*)0x659078;

		//baseClass_WndWindow = m_pSelf;

		//*m_pSelf = *static_cast<KWndAutoPlay*>(baseClass_WndWindow); // dynamic_cast

		//g_TempWindow = new TempWindow();
		
		 
		//g_WndAutoPlay = new KWndAutoPlay();

		//void** vtable = *((void***)(((char*)m_pSelf) + 0));
		//void* vtable = (void*)((char*)m_pSelf);

		//CreateWndWindow(*((char**)m_pSelf));

		//void* vptr_Window = ((void*)0x67C03C);

		//vptr_Window = m_pSelf;

		//auto m_pSelf = reinterpret_cast<void**>(vptr_Window);

		
		//CreateWndWindow(*((DWORD**)m_pSelf));

		//*((void**)m_pSelf) = &*(static_cast<void**>(*((void**)0x63CBCC)));

		//m_pSelf = new TempWindow();

		//m_pSelf = new KWndAutoPlay();

		//BYTE* baseClass_WndWindow = (BYTE*)0x659078;

		//baseClass_WndWindow = (BYTE*)new KWndAutoPlay();

		m_pSelf = new KWndAutoPlay();

		//CreateWndWindow(m_pSelf);
		//*((BYTE**)m_pSelf) = (BYTE*)CreateWndWindow(baseClass_WndWindow);
		//*((void**)m_pSelf) = static_cast<BYTE*>(baseClass_WndWindow);

		//m_pSelf = &g_WndAutoPlay;

		//m_pSelf = &g_WndAutoPlay;

		//memcpy(m_pSelf + 1, (BYTE*)CreateWndWindow(new TempWindow) + 1, 252);

		//*m_pSelf = *(KWndAutoPlay*)CreateWndWindow(new TempWindow);

		//*(TempWindow*)m_pSelf = *((TempWindow*)CreateWndWindow(new TempWindow));

		//g_WndAutoPlay = new KWndAutoPlay();
		
		//memcpy(&g_WndAutoPlay, &m_pSelf, sizeof(KWndAutoPlay));

		//SetDword(g_WndAutoPlay->vfptr[4], *(DWORD*)m_pSelf);

		//ofstream myfile;
		//myfile.open("E:\\KWndAutoPlay1.txt");
		////myfile << "This " << (DWORD*)vtable_WndWindow << "\n";
		//myfile << "(DWORD*)this " << (DWORD*)g_WndAutoPlay << "\n";
		//myfile << "g_WndAutoPlay->vfptr[0] " << (DWORD*)g_WndAutoPlay->vfptr[0] << "\n";
		//myfile << "*((DWORD**)g_WndAutoPlay) " << ((DWORD**)g_WndAutoPlay) << "\n";
		//myfile << "*((DWORD**)g_WndAutoPlay) " << *((DWORD**)g_WndAutoPlay) << "\n";
		//myfile << "(DWORD*)m_pSelf " << (DWORD*)m_pSelf << "\n";
		//myfile << "(DWORD*)m_pSelf " << *(DWORD*)m_pSelf << "\n";
		//myfile << "*((DWORD**)m_pSelf) " << *((DWORD**)m_pSelf) << "\n";
		//myfile << "(DWORD*)&m_pSelf " << (DWORD*)&m_pSelf << "\n";
		//myfile.close();

		//CreateWndWindow(&g_TempWindow);

		//memcpy(*((DWORD**)g_WndAutoPlay), *((DWORD**)m_pSelf), 4);

		//SetDword((DWORD)((DWORD**)m_pSelf), (DWORD)*((DWORD**)g_WndAutoPlay));

		//ofstream myfile2;
		//myfile2.open("E:\\KWndAutoPlay2.txt");
		////myfile << "This " << (DWORD*)vtable_WndWindow << "\n";
		//myfile2 << "(DWORD*)this " << (DWORD*)g_WndAutoPlay << "\n";
		//myfile2 << "g_WndAutoPlay->vfptr[0] " << (DWORD*)g_WndAutoPlay->vfptr[0] << "\n";
		//myfile2 << "*((DWORD**)g_WndAutoPlay) " << ((DWORD**)g_WndAutoPlay) << "\n";
		//myfile2 << "*((DWORD**)g_WndAutoPlay) " << *((DWORD**)g_WndAutoPlay) << "\n";
		//myfile2 << "(DWORD*)m_pSelf " << (DWORD*)m_pSelf << "\n";
		//myfile2 << "(DWORD*)m_pSelf " << *(DWORD*)m_pSelf << "\n";
		//myfile2 << "*((DWORD**)m_pSelf) " << *((DWORD**)m_pSelf) << "\n";
		//myfile2 << "(DWORD*)&m_pSelf " << (DWORD*)&m_pSelf << "\n";
		//myfile2.close();

		//CreateWndWindow(m_pSelf);

		//m_pSelf->m_BtnAutoPlay = *(TempButton*)CreateWndButton(new TempButton());
		//*((DWORD*)&NewAddressData3) = *((DWORD*)new KWndAutoPlay);

		//m_pSelf = (KWndAutoPlay*)&NewAddressData3;

		//RefCountBaseClass(&NewAddressData3, 592u, 1, fnVtableWndWindow, fnRefWndBase);

		//DWORD pid = GetCurrentProcessId();

		//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

		//m_pSelf = (KWndAutoPlay*)VirtualAllocEx(hProcess, (KWndAutoPlay*)&NewAddressData3, 848, 1052672, MEM_COMMIT);

		//m_pSelf = (KWndAutoPlay*)VirtualAllocEx(hProcess, NULL, 848, 1052672, PAGE_EXECUTE_READWRITE);

		//*(&m_pSelf + 256) = (KWndAutoPlay*)VirtualAllocEx(hProcess, NULL, 848, 1052672, PAGE_EXECUTE_READWRITE);

		//RefCountBaseClass(&m_pSelf, 340u, 1, fnRefWndWindow, fnRefWndBase);

		//g_WndAutoPlay = new KWndAutoPlay;

		//CreateWndImage((DWORD*)g_WndAutoPlay);

		//m_pSelf = g_WndAutoPlay;

		//*(KWndAutoPlay*)NewAddressData3 = *new KWndAutoPlay;

		//m_pSelf = (KWndAutoPlay*)&NewAddressData3;
		 
		//m_pSelf = new KWndAutoPlay;

		//m_pSelf = (KWndAutoPlay*)gWndWindow;
		 
		//m_pSelf = (KWndAutoPlay*)&gAutoPlay;
		
		if (m_pSelf)
		{
			//0x659078
			/*KComClassInfo pComAutoPlay1;
			strcpy(pComAutoPlay1.szClassType, "Player_AutoPlay2");
			pComAutoPlay1.szClassType[32] = 0;
			pComAutoPlay1.pfnCreateObject = &m_pSelf->m_BtnAutoPlay.CreateObject;
			_RegisterComClass(&pComAutoPlay1);*/

			//RefCountBaseClass(m_pSelf, 256u, 1, (void(__thiscall*)(int))0x441A60, (void(__thiscall*)(int))0x44ADC0);

			//VoidMemberFn* fn = (VoidMemberFn*)GetVTable<KWndWindow>((KWndWindow*)0x63DB6C)[0];

			//fn((KWndWindow*)0x63DB6C);

			//CreateWndWindow(m_pSelf);

			//CreateWndImage((DWORD*)m_pSelf);
				
			CreateWndButton(&m_pSelf->m_BtnAutoPlay);

			//g_TempWindow

			//delete baseClass_WndWindow;

			//void* baseClass_WndImage = (void*)0x659040;

			//baseClass_WndImage = new KWndImage();

			//delete baseClass_WndImage;

			//void* baseClass_WndButton = (void*)0x659434;

			//baseClass_WndButton = new KWndButton();

			//void* baseClass_WndWindow = (void*)0x659078;
			//reinterpret_cast<KWndButton*>(baseClass_WndWindow);

			//void* baseClass_WndWindow = (void*)0x659078;

			//baseClass_WndWindow = m_pSelf;

			//*m_pSelf = *static_cast<KWndAutoPlay*>(baseClass_WndWindow); // dynamic_cast

			//void* baseClass_WndButton = (void*)0x659434;

			//baseClass_WndWindow = (BYTE*)&m_pSelf->m_BtnAutoPlay;

			//baseClass_WndButton = &m_pSelf->m_BtnAutoPlay;

			//*(&m_pSelf->m_BtnAutoPlay) = (KWndButton&)static_cast<void*>(baseClass_WndButton);

			//KWndAutoPlay* vtable_WndWindow = (KWndAutoPlay*)0x63DB6C;

			//void (*EntryProc)(void*) = ((void(*)(void*)) reinterpret_cast<void*>((void*)0x441A60));

			//EntryProc(this);

			//vtable_WndWindow = m_pSelf;

			//*vtable_WndWindow = static_cast<KWndAutoPlay>(*((KWndAutoPlay*)(void*)0x65A50C));

			//gWndWindow = new KWndAutoPlay;

			//m_pSelf = vtable_WndWindow;

			//void* vtable_WndWindow = (void*)0x65A50C;

			//m_pSelf = reinterpret_cast<KWndAutoPlay*>((KWndAutoPlay*)gWndWindow);

			//vtable_WndWindow = m_pSelf;

			//void* vptr_WndImage = (void*)0x659D40;

			//void* vptr_WndWindow = new KWndWindow;

			//void* vptr_WndImage = new KWndImage;

			//void* vptr_WndButton = new KWndButton;
			 
			//m_pSelf = (KWndAutoPlay*)CreateWndWindow((DWORD*)vtable_WndWindow);

			//CreateWndImage((unsigned int*)this);
			//vptr_WndWindow = &m_pSelf;

			//vptr_WndButton = &m_pSelf->m_BtnAutoPlay;

			//vptr_WndImage = &vptr_WndButton;

			//KWndAutoPlay ptr1;

			//m_pSelf = (KWndAutoPlay*)0x67C03C;

			//VirtualProtect(vptr_WndWindow, 2048, PAGE_EXECUTE_READWRITE, &oldProtection);

			//vptr_WndWindow = &ptr1;

			//VirtualProtect(vptr_WndWindow, 2048, oldProtection, 0);

			//m_pSelf = (KWndAutoPlay*)vptr_WndWindow;

			
			 
			//Foo1(*(void**)&vptr_Window);

			//void* vptr_Button = (void*)0x67D0C4;

			//vptr_Button = &m_pSelf->m_BtnAutoPlay;
			
			
			/*KWndWindow* vtable_WndWindow = (KWndWindow*)0x63DB6C;

			vtable_WndWindow = m_pSelf;

			CreateWndWindow((DWORD*)m_pSelf);

			KWndButton* button_vtable = (KWndButton*)0x63CBCC;

			button_vtable = &m_pSelf->m_BtnAutoPlay;

			CreateWndButton((DWORD*)&m_pSelf->m_BtnAutoPlay);*/

			//exit(9);

			//CreateWndWindow((BYTE*)((unsigned int*)((void*)m_pSelf)));

			//ReleaseWndWindow((unsigned int*)m_pSelf);

			//vptr_WndWindow = &m_pSelf;

		//	ofstream myfile;
		//myfile.open("E:\\m_BtnAutoPlay.txt");
		//myfile << "KWndAutoPlay : " << sizeof(m_pSelf->m_BtnAutoPlay) << "\n"; // skill id
		//myfile << "KWndAutoPlay : " << &m_pSelf->m_BtnAutoPlay << "\n"; // skill id
		//myfile << "Player_AutoPlay 2 : " << sizeof(Player_AutoPlay) << "\n"; // skill id
		// 
		//	//CreateWndWindow((BYTE*)&m_pSelf->m_BtnAutoPlay);
		//	//CreateWndImage((unsigned int*)&m_pSelf->m_BtnAutoPlay);
		//	//CreateWndButton((void*)&m_pSelf->m_BtnAutoPlay);
		//myfile << "Player_AutoPlay 2 : " << sizeof(m_pSelf->m_BtnAutoPlay) << "\n"; // skill id
		//myfile << "Player_AutoPlay 2 : " << &m_pSelf->m_BtnAutoPlay << "\n"; // skill id
		//myfile << "Player_AutoPlay : " << (DWORD*)Player_AutoPlay::CreateObject << "\n"; // skill id
		//myfile << "m_pSelf : " << (DWORD*)m_pSelf << "\n"; // skill id
		//myfile << "Address1 : " << (DWORD*)Address1 << "\n"; // skill id
		//myfile << "Address2 : " << (DWORD*)Address2 << "\n"; // skill id
		//myfile << "NewAddressData1 : " << (DWORD*)NewAddressData1 << "\n"; // skill id
		//myfile << "NewAddressData2 : " << (DWORD*)NewAddressData2 << "\n"; // skill id
		//myfile << "NewAddressData3 : " << (DWORD*)NewAddressData3<< "\n"; // skill id
		//myfile.close();

			//m_pSelf = reinterpret_cast<KWndAutoPlay*>((void*)0x659078);

			//KWndWindow* ptrWindow2;

			//m_pSelf = (KWndAutoPlay*)&gWndWindow;	

			//KWndWindow* ptrWindow = *(&m_pSelf);

			//KWndButton* ptrButton = &m_pSelf->m_BtnAutoPlay;

			//gWndWindow = &ptr1;

			//(obj.*vptr_WndWindow)();
			
			//CreateWndWindow((BYTE*)m_pSelf);

			//m_pSelf = (KWndAutoPlay*)gWndWindow;

			//void* vptr = (void*)ptrWindow;

			//KWndButton ptr2;

			//gWndButton = &ptr2;

			//m_pSelf->m_BtnAutoPlay = *(KWndButton*)gWndButton;
			 
			//m_pSelf = (KWndAutoPlay*)(void*)vptr;

			//m_pSelf = (KWndAutoPlay*)NewAddressData3;

			//*(unsigned int*)&m_pSelf->m_BtnAutoPlay = *CreateWndButton((unsigned int*)ptrButton);

			//void* vptrbt = (void*)ptrButton;

			//*&m_pSelf->m_BtnAutoPlay = *(KWndButton*)(void*)vptrbt;

			//*&m_pSelf->m_BtnAutoPlay = *gWndButton;
			//*(KWndAutoPlay**)&gAutoPlay = new KWndAutoPlay;
			//CreateWndWindow((unsigned int*)m_pSelf);
			//CreateWndButton((unsigned int*)&m_pSelf->m_BtnAutoPlay);
			//CreateWndWindow((unsigned int*)m_pSelf);
		}
		 
		//CreateWndButton((unsigned int*)&m_pSelf->m_BtnAutoPlay);

		//gAutoPlay = m_pSelf;
		//m_pSelf = reinterpret_cast<KWndAutoPlay*>(&vptr_WndWindow);
		//m_pSelf = new KWndAutoPlay;

		//CreateWndShowAnimate((unsigned int*)m_pSelf);
		//m_pSelf = (KWndAutoPlay*)CreateWndWindow((unsigned int*)m_pSelf);
		//m_pSelf = new KWndAutoPlay;
		//m_pSelf = *(KWndAutoPlay**)CreateWndButton((unsigned int*)m_pSelf);
		//m_pSelf = (KWndAutoPlay*)CreateWndImage((unsigned int*)m_pSelf);
		//m_pSelf = (KWndAutoPlay*)m_pSelf2;
		//m_pSelf = (KWndAutoPlay*)vptr_WndWindow;

		

		if (m_pSelf)
			m_pSelf->Initialize();
	}

	if (m_pSelf)
	{
		//m_pSelf->Show();
		//*(DWORD*)((unsigned int)m_pSelf + 4) |= WND_S_VISIBLE;
		//if (*(DWORD*)((unsigned int)m_pSelf + 4) != WND_S_VISIBLE)
		//{
			//m_pSelf->Show();
			//KWndButton_Enable(m_pSelf, 1);
		//}

		//m_pSelf->Enable(1);
		//m_pSelf->m_BtnAutoPlay.Enable(1);
		b_ShowWndWindow(m_pSelf);

		//b_ShowWndWindow(&m_pSelf->m_BtnAutoPlay);

		//KWndButton_Enable(&m_pSelf->m_BtnAutoPlay, 1);
		//m_pSelf->m_BtnAutoPlay.Enable(1);
		//if (m_pSelf->IsVisible())
		//{
		//	//b_ShowWndWindow(&m_pSelf);
		//	m_pSelf->Show();
		//}
		//{
			//m_pSelf->Show();
			//ofstream myfile;
			//myfile.open("E:\\KWndAutoPlay_OpenWindow.txt");
			//myfile << "WndProc : " << *((DWORD*)((DWORD*)((DWORD*)m_pSelf))) << "\n"; // skill id
			//myfile << "WndProc : " << m_pSelf->m_Flag << "\n"; // skill id
			//myfile << "WndProc : " << (DWORD*)m_pSelf->m_Style << "\n"; // skill id
			//myfile << "WndProc : " << *(m_pSelf->vtable + 1) << "\n"; // skill id
			//myfile << "WndProc : " << (DWORD*)((DWORD*)m_pSelf + 1) << "\n"; // skill id
			//myfile.close();
		//}
	}

	return m_pSelf;
}

//int	KWndAutoPlay::Init(KIniFile* pIniFile, const char* pSection)
//{
//	/*ofstream myfile;
//	myfile.open("E:\\KWndAutoPlay_Init.txt");
//	myfile << "m_Width : " << pIniFile << "'\n";
//	myfile << "m_Height : " << pSection << "'\n";
//	myfile.close();*/
//
//	/*if (KWndWindow::Init(pIniFile, pSection))
//	{
//		m_BtnAutoPlay.Init(pIniFile, "btnOpen");
//	}*/
//
//	//if (KWndWindow_Init(this, pIniFile, pSection))
//	//{
//	//	//m_BtnAutoPlay.Init(pIniFile, "btnOpen");
//	//	KWndButton_Init(this, pIniFile, "btnOpen")
//	//}
//
//	//return KWndButton::Init(pIniFile, pSection);
//	/*if (KWndButton_Init(this, pIniFile, pSection))
//	{
//
//	}*/
//
//	//if (KWndImage_Init(this, pIniFile, pSection))
//	//{
//	//	/*if (KWndButton::Init(pIniFile, pSection))
//	//	{
//
//	//	}*/
//	//
//	//}
//	
//	/*if (KWndImage::Init(pIniFile, pSection))
//	{
//
//	}*/
//	//if (KWndButton::Init(pIniFile, pSection))
//	//{
//	//	//*(void**)this = +((char**)((void**)0x63DB6C));
//	//}
//	return 1;
//}

int KWndAutoPlay::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	/*try
	{

	}
	catch (..)
	{
	}*/
	ofstream myfile;
	myfile.open("E:\\WndProc_KWndWindowAutoPlay2.txt");
	myfile << "This " << (DWORD*)this << "\n"; // skill id
	myfile << "uMsg " << uMsg << "\n"; // skill id
	myfile << "uParam " << uParam << "\n"; // skill id
	myfile << "nParam " << nParam << "\n"; // skill id
	//myfile << "m_pParentWnd " << (DWORD*)this->m_pParentWnd << "\n"; // skill id
	//myfile << "m_pParentWnd " << (DWORD*)this->m_pFirstChild << "\n"; // skill id
	myfile << "m_pParentWnd2 " << (DWORD*)this + 24 << "\n"; // skill id
	myfile << "m_pParentWnd2 " << (DWORD*)this + 23 << "\n"; // skill id
	myfile.close();
	//nRet = KWndWindow_WndProc(this, uMsg, uParam, nParam);
	//OnClickButton((KWndButton*)(KWndWindow*)uParam);
	switch (uMsg)
	{
		//case 1381:
		//	if (uParam == (unsigned int)(KWndWindow*)&m_BtnAutoPlay)
		//	{
		//		ofstream myfile2;
		//		myfile2.open("E:\\KWndAutoPlay_OnButtonClick_1381.txt");
		//		myfile2 << "This " << (DWORD*)this << "\n";
		//		myfile2 << "m_BtnAutoPlay " << (DWORD*)&m_BtnAutoPlay << "\n";
		//		myfile2 << "uMsg " << uMsg << "\n";
		//		myfile2 << "uParam " << uParam << "\n";
		//		myfile2 << "nParam " << nParam << "\n";
		//		myfile2.close();
		//	}
		//	//OnButtonClick();
		//	break;
		default:
			//nRet = KWndWindow::WndProc(uMsg, uParam, nParam);
			//nRet = WndWindow::WndProc(uMsg, uParam, nParam);
			nRet = TempWindow::WndProc(uMsg, uParam, nParam);
			//
			//break;
	}
	return nRet;
}

//void KWndAutoPlay::PaintWindow()
//{
//	ofstream myfile;
//	myfile.open("E:\\PaintWindow.txt");
//	myfile << "This " << (DWORD*)this << "\n"; // skill id
//	myfile.close();
//	KWndImage_PaintWindow(this);
//	//KWndWindow::PaintWindow();
//}

void KWndAutoPlay::Initialize()
{
	//*&m_BtnAutoPlay = *(Player_AutoPlay2*)CreateWndButton(new TempButton());
	//RefCountBaseClass(&m_BtnAutoPlay, 592u, 1, (void(__thiscall*)(int))0x425E20, (void(__thiscall*)(int))0x44ADC0);
	//*&m_BtnAutoPlay = *reinterpret_cast<KWndButton**>((void*)0x67D0C4);
	//CreateWndWindow((void*)this);
	//void* (__thiscall**ptnButton)(void*) = (void* (__thiscall**)(void*))0x425E20;
	//*(unsigned int*)&m_BtnAutoPlay = *CreateWndButton((void*)&m_BtnAutoPlay);
	//*(unsigned int*)&m_pSelf->m_BtnAutoPlay = *CreateWndButton((unsigned int*)&m_pSelf->m_BtnAutoPlay);
	//KWndButton* m_BtnAutoPlay;
	//m_BtnAutoPlay = &m_pSelf->m_BtnAutoPlay;
	//*&m_pSelf->m_BtnAutoPlay = m_BtnAutoPlay;
	//CreateWndButton(&m_BtnAutoPlay);
	//void (TempButton:: * *TempButton)(void*) = (void (TempButton::**)(void*))0x425E20;
	//(&m_BtnAutoPlay->**TempButton)(&m_BtnAutoPlay);
	//*(&m_BtnAutoPlay) = *(Player_AutoPlay2*)CreateWndButton(new Player_AutoPlay2());
	//KComClassFactory* This = _ComClassFactory();
	//This->RegisterComClass(this->m_one);
	//CreateWndImage((unsigned int*)&m_BtnAutoPlay);
	//CreateWndButton((unsigned int*)(unsigned int*)&m_BtnAutoPlay);
	//KWndButton m_BtnAutoPlay = *new KWndButton;
	b_AddChild(this, &m_BtnAutoPlay);
	//b_AddChild(m_pSelf, &m_pSelf->m_BtnAutoPlay);
	//AddChild(&m_BtnAutoPlay);
	//AddChild(m_pButton);
	//AddChild(&m_BtnAutoPlay);

	//char Scheme[256];

	//b_sub_41A4A0((const char*)0x68D090, Scheme, 256);

	KIniFile	Ini;

	if (Ini.Load("\\Ui\\Ui3\\AutoPlay_icon.ini"))
	{
		LoadScheme(&Ini);
		
	//	//this->Init(&Ini, "Main");
	//	//this->Init(&Ini, "Main");
	//	//KWndButton_Init(&m_ButtonAutoPlay, &Ini, "btnOpen");

	//	//m_ButtonAutoPlay.Enable(1);
	//	//m_pSelf->m_Button.Init(&Ini, "btnOpen");
	//	//m_pSelf->m_Button.Show();
	//	//b_ShowWndWindow(&m_pSelf->m_Button);
	//	//b_HideWndWindow(&m_pSelf);
	}
	//LoadScheme("\\Ui\\Ui3\\AutoPlay_icon.ini");
	//LoadScheme("\\Ui\\Ui3\\AutoPlay_icon.ini");
	//*(DWORD*)(this + 4) &= ~WND_S_VISIBLE;
	//m_pSelf->LoadScheme("\\Ui\\Ui3\\AutoPlay_icon.ini");

	ofstream myfile;
	myfile.open("E:\\KWndAutoPlay.txt");
	//myfile << "This " << (DWORD*)vtable_WndWindow << "\n";
	myfile << "(DWORD*)this " << (DWORD*)this << "\n";
	myfile << "*((DWORD**)this) " << *((DWORD**)this) << "\n";
	myfile << "*(DWORD*)&m_BtnAutoPlay " << *(DWORD*)&m_BtnAutoPlay << "\n";
	myfile << "(DWORD*)&m_BtnAutoPlay " << (DWORD*)&m_BtnAutoPlay << "\n";
	myfile << "*((DWORD**)&m_BtnAutoPlay) " << *((DWORD**)&m_BtnAutoPlay) << "\n";
	myfile << "(DWORD*)m_pSelf " << (DWORD*)m_pSelf << "\n";
	myfile << "*((DWORD**)m_pSelf) " << *((DWORD**)m_pSelf) << "\n";
	myfile << "(DWORD*)&m_pSelf " << (DWORD*)&m_pSelf << "\n";
	myfile << "Button reference 1 " << (DWORD*)&m_pSelf->m_BtnAutoPlay << "\n";
	myfile << "Button reference 2 " << (DWORD*)&m_pSelf->m_BtnAutoPlay << "\n";
	myfile << "This " << (DWORD*)m_pSelf << "\n";
	myfile.close();

	b_Wnd_AddWindow(m_pSelf, 1);
	//Call_Wnd_AddWindow(this, 1);

	
	//b_AddChild(gAutoPlay, &gAutoPlay + 428);
	//ofstream myfile;
	//myfile.open("E:\\OpenWindow.txt");
	//myfile << "1 " << **(DWORD***)m_pSelf << "\n"; // skill id
	//myfile << "2 " << **(DWORD***)&m_pSelf << "\n"; // skill id
	//myfile.close();
}

void KWndAutoPlay::LoadScheme(const char* pScheme)
{
	KIniFile	Ini;

	if (m_pSelf && Ini.Load("\\Ui\\Ui3\\AutoPlay_icon.ini"))
	{
		m_pSelf->LoadScheme(&Ini);
		//m_pSelf->Init(&Ini, "Main");
		//m_pSelf->m_BtnAutoPlay.Init(&Ini, "btnOpen");
		//KWndWindow_Init(&m_pSelf, &Ini, "Main");
		//KWndButton_Init(&m_pSelf->m_BtnAutoPlay, &Ini, "btnOpen");
		//this->Init(&Ini, "Main");
		//this->Init(&Ini, "Main");
		//KWndButton_Init(&m_ButtonAutoPlay, &Ini, "btnOpen");
		
		//m_ButtonAutoPlay.Enable(1);
		//m_pSelf->m_Button.Init(&Ini, "btnOpen");
		//m_pSelf->m_Button.Show();
		//b_ShowWndWindow(&m_pSelf->m_Button);
		//b_HideWndWindow(&m_pSelf);
	}

	Ini.Clear();
}

//int KWndAutoPlay::Init(KIniFile* pIniFile, const char* pSection)
//{
//	if (KWndWindow::Init(pIniFile, pSection))
//	{
//	}
//	return false;
//}

void KWndAutoPlay::LoadScheme(KIniFile* pIni)
{
	
	//Init(pIni, "Main");
	//m_pSelf->m_BtnAutoPlay.Init(pIni, "btnOpen");
	//Init(pIni, "Main");
	//TempWindow::Init(pIni, "Main");
	//KWndWindow::Init(pIni, "btnOpen");
	//KWndImage::Init(pIni, "Main");
	//KWndButton::Init(pIni, "btnOpen");
	//this->Enable(1);
	
	/*int (__thiscall** CastInit)(KWndAutoPlay*, KIniFile*, const char*) = (int(__thiscall**)(KWndAutoPlay*, KIniFile*, const char*))0x425FB0;
	(**CastInit)(this, pIni, "Main");*/
	 
	//KWndWindow_Enable(this, 1);
	KWndWindow_Init(this, pIni, "Main");
	//KWndImage_Init(&m_pSelf, pIni, "Main");
	//memset(&m_BtnAutoPlay, 0, sizeof(m_BtnAutoPlay));
	//void* vptr_WndButton = (void*)(unsigned int*)0x659B94;
	//m_BtnAutoPlay = *(Player_AutoPlay*)&vptr_WndButton;
	/**(BYTE*)&m_BtnAutoPlay = *CreateWndWindow((BYTE*)&m_BtnAutoPlay);*/
	//CreateWndButton((unsigned int*)&m_BtnAutoPlay);
	/*Player_AutoPlay* m_BtnAutoPlay1 = new Player_AutoPlay;
	Player_AutoPlay* m_BtnAutoPlay2;
	m_BtnAutoPlay2 = (Player_AutoPlay*)CreateWndButton((unsigned int*)m_BtnAutoPlay1);
	m_BtnAutoPlay = *(Player_AutoPlay*)m_BtnAutoPlay2;*/

	//int (TempButton:: **CastInit)(KIniFile*, const char*) = (int(TempButton::**)(KIniFile*, const char*))0x425FB0;
	//(&m_BtnAutoPlay->**CastInit)(pIni, "btnOpen");
	KWndButton_Init(&m_BtnAutoPlay, pIni, "btnOpen");
	//KWndButton_Init(*((DWORD**)&m_BtnAutoPlay), pIni, "btnOpen");
	//m_BtnAutoPlay.Init(pIni, "btnOpen");
	//((int(TempButton::* Init)(pIni, "btnOpen")) 0x425FB0);

	//int (*Init)(KIniFile*, const char*) = (int(*)(KIniFile*, const char*))0x425FB0;
	//Init();
	//m_BtnAutoPlay.Init(pIni, "btnOpen");

	//KWndButton_Init(&m_BtnAutoPlay, pIni, "btnOpen");
	
	//KWndWindow_Init(this, pIni, "Main");
	//KWndImage_Init(this, pIni, "Main");
	//KWndWindow_Init(&m_BtnAutoPlay, pIni, "btnOpen");
	//m_BtnAutoPlay.Init(pIni, "btnOpen");
	//KWndButton_Init(&m_BtnAutoPlay, pIni, "btnOpen");

	//ofstream myfile;
	//myfile.open("E:\\LoadScheme.txt");
	//myfile << "GetParent : " << *(DWORD*)this << "\n"; // skill id
	//myfile << "GetParent : " << *((DWORD*)this + 23) << "\n"; // skill id
	//myfile << "GetParent : " << *((DWORD*)this + 24) << "\n"; // skill id
	//myfile << "GetParent : " << (DWORD*)&m_BtnAutoPlay << "\n"; // skill id
	//myfile << "GetParent : " << *((DWORD*)&m_BtnAutoPlay + 23) << "\n"; // skill id
	//myfile << "GetParent : " << *((DWORD*)&m_BtnAutoPlay + 24) << "\n"; // skill id
	//myfile.close();

	//KWndWindow_Init(gAutoPlay, pIni, "Main");
	//KWndButton_Init(&gAutoPlay + 428, pIni, "btnOpen");
	//b_AddChild(&m_pSelf, &this->m_BtnAutoPlay);
	//m_BtnAutoPlay.CreateObject()->Init(pIni, "btnOpen");
	//m_BtnAutoPlay.Init(pIni, "btnOpen");
	//m_BtnAutoPlay.Init(pIni, "btnOpen");
	//AddChild(&m_BtnAutoPlay);

	pIni->Clear();
}

void KWndAutoPlay::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
		{
			//m_pSelf->Hide();
		}
		else
		{
			//m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

//IMPLEMENT_COMCLASS(KWndAutoPlay)
void KWndAutoPlay::OnButtonClick()
{
	unsigned int KPlayer_Offset1 = NULL;
	int					m_PlayerIndex = NULL;

	KPlayer_Offset1 = *(unsigned int*)(0x8E48F4);

	m_PlayerIndex = *(unsigned int*)(KPlayer_Offset1 + 0x8B30 + 0x114);

	//ofstream myfile;
	//myfile.open("E:\\KAutoPlay_OnButtonClick.txt");
	//myfile << "m_PlayerIndex : " << m_PlayerIndex << "\n"; // skill id
	//myfile << "KWndWindow : " << &pWnd << "\n"; // skill id
	//myfile.close();

	
	//if (*(DWORD**)&pWnd + 0x1C8 == *(DWORD**)m_pSelf)
	if (m_PlayerIndex > 0)
	{
		ofstream myfile;
		myfile.open("E:\\KAutoPlay_OnClickButton.txt");
		//myfile << "pWnd " << (DWORD*)pWnd << "\n"; // skill id
		//myfile << "pWnd " << *(DWORD*)pWnd << "\n"; // skill id
		//myfile << "pWnd " << *(DWORD**)pWnd << "\n"; // skill id
		//myfile << "pWnd " << *(DWORD**)(pWnd + 0x1C8) << "\n"; // skill id
		//unsigned int nNumber1 = ;
		//unsigned int nNumber2 = (*(DWORD*)pWnd + 0x1C8);
		//myfile << "pWnd " << *(DWORD**)nNumber2 << "\n"; // skill id
		//myfile << "pWnd " << *(DWORD*)(*(DWORD**)pWnd) + 0x1C8 << "\n"; // skill id


		//myfile << "This " << ((KWndButton*)(KWndImage*)(KWndWindow*)m_pSelf) << "\n"; // skill id
		//myfile << "This " << *(DWORD*)m_pSelf << "\n"; // skill id
		//myfile << "This " << *(DWORD**)m_pSelf << "\n"; // skill id
		//myfile << "This " << (KWndButton*)(KWndImage*)(KWndWindow*)&m_pSelf << "\n"; // skill id
		//myfile << "This " << (DWORD*)(KWndButton*)(KWndImage*)(KWndWindow*)&m_pSelf << "\n"; // skill id
		myfile << "m_PlayerIndex " << m_PlayerIndex << "\n"; // skill id
		myfile.close();

		/*if (KUiAutoPlay::GetIfVisible())
			KUiAutoPlay::CloseWindow(false);
		else
			KUiAutoPlay::OpenWindow();*/
	}

}

const char* KWndAutoPlay::GetShortKey()
{
	/*return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
			KShortcutKeyCentre::FindCommandByScript("Open([[auto]])")));*/
	return NULL;
}

void KWndAutoPlay::OnLBtnDown(bool bDoubleClick)
{
	BYTE* Curren = (BYTE*)this;

	ofstream myfile;
	myfile.open("E:\\KWndAutoPlay_OnLBtnDown.txt");
	//myfile << "m_pParentWnd : " << this->m_pParentWnd << "'\n";
	myfile << "This  : " << (DWORD*)this << "'\n";
	myfile << "bDoubleClick : " << bDoubleClick << "'\n";
	//myfile << "IsVisible : " << this->m_Style << "'\n";
	//myfile << "IsVisible : " << this->IsVisible() << "'\n";
	//myfile << "IsVisible : " << (this->m_Style == WND_S_VISIBLE) << "'\n";
	myfile.close();

	/*if (!this->IsVisible())
		KUiAutoPlay::OpenWindow();
	else
		KUiAutoPlay::CloseWindow(false);*/
}

//IMPLEMENT_COMCLASS(Player_AutoPlay)
void Player_AutoPlay::OnButtonClick()
{
	//BYTE* Curren = (BYTE*)this;

	//ofstream myfile;
	//myfile.open("E:\\Player_AutoPlay_OnButtonClick.txt");
	//myfile << "IsVisible : " << this->m_Style << "'\n";
	//myfile << "IsVisible : " << this->IsVisible() << "'\n";
	//myfile << "IsVisible : " << (this->m_Style == WND_S_VISIBLE) << "'\n";
	//myfile.close();

	//if (this->IsVisible())
	//	KUiAutoPlay::OpenWindow();
	//else
	//	KUiAutoPlay::CloseWindow(false);
}

//Player_AutoPlay::Player_AutoPlay()
//{
//	CreateWndButton((DWORD*)this);
//	//*(void**)this = +((char**)((void**)0x63CBCC));
////	//RefCountBaseClass((void*)this, 440, 1, (void*)RefWndImage, (void*)RefWndBase);
////	//RefCountBaseClass((void*)this, 592, 1, (void*)RefWndButton, (void*)RefWndBase);
//	//CreateWndWindow((BYTE*)((unsigned int*)((void*)this)));
//	//*((DWORD*)this) = *CreateWndImage((DWORD*)gWndImage);
//	//CreateWndImage((unsigned int*)((void*)this));
//	//CreateWndButton((DWORD*)this);
////	//ReleaseWndButton((unsigned int*)this);
//}

const char* Player_AutoPlay::GetShortKey()
{
	/*return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
			KShortcutKeyCentre::FindCommandByScript("Open([[auto]])")));*/
	return NULL;
}

//Player_AutoPlay2::Player_AutoPlay2()
//{
//	//CreateWndWindow(*(DWORD**)this);
//
//	//KWndButton* button_base = (KWndButton*)0x659434;
//
//	//KWndButton* button_array = (KWndButton*)0x659460;
//
//	//KWndButton* button_vtable = (KWndButton*)0x63CBCC;
//
//	////window_vtable = button_vtable;
//	 
//	//KWndWindow* window_base = (KWndWindow*)0x659078;
//
//	//KWndWindow* window_vtable = (KWndWindow*)0x63DB6C;
//
//	//KWndButton* button_base = (KWndButton*)0x659434;
//
//	//KWndButton* button_array = (KWndButton*)0x659460;
//	 
//	//KWndButton* button_vtable = (KWndButton*)0x63CBCC;
//
//	////window_vtable = button_vtable;
//
//	//button_base = this;
//
//	//window_vtable = this;
//
//	//button_base = this;
//
//	//CreateWndButton((DWORD*)this);
//
//	//*(void**)this = reinterpret_cast<void*>(button_vtable);
//	//this = static_cast<void*>(button_array);
//
//	//Address2 = new Player_AutoPlay2;
//
//	//KWndButton* button_vtable = (KWndButton*)0x63CBCC;
//
//	//button_vtable = this;
//	//CreateWndWindow(*((DWORD**)this));
//	//CreateWndButton((DWORD*)this);
//
//	//*(void**)this = +((char**)((void**)0x63CBCC));
//
//	//void* vptr_WndWindow = (void*)0x63DB6C;
//	//void* button_ptr = (void*)0x63CBCC;
//
//	//button_base = reinterpret_cast<KWndButton*>(this);
//	//void (*EntryProc)(void*) = ((void(*)(void*)) reinterpret_cast<void*>((void*)0x441A60));
//
//	//EntryProc(this);
//
//	//button_ptr = this;
//
//	//*((KWndWindow*)this) = *dynamic_cast<KWndWindow*>((KWndWindow*)vptr_WndWindow);
//	/*void* vptr_WndWindow = (void*)0x63DB6C;
//	void* image_ptr = (void*)0x63CF4C;
//	unsigned int* button_ptr = (unsigned int*)0x63CBCC;
//
//	vptr_WndWindow = this;
//	image_ptr = ((void*)this);
//	button_ptr = ((unsigned int*)this);*/
//
//	//gWndButton = dynamic_cast<KWndButton*>((KWndButton*)&button_ptr);
//
//	//*(this) = *dynamic_cast<Player_AutoPlay2*>((Player_AutoPlay2*)vptr_WndWindow);
//	//*(this) = *dynamic_cast<Player_AutoPlay2*>((Player_AutoPlay2*)image_ptr);
//	//*(this) = *dynamic_cast<Player_AutoPlay2*>((Player_AutoPlay2*)button_ptr);
//
//	//*(this) = *(Player_AutoPlay2*)new KWndButton;
//
//	//unsigned int* button_ptr = (unsigned int*)0x63CBCC;
//
//	//button_ptr = ((unsigned int*)this);
//
//	//gWndButton = dynamic_cast<KWndButton*>((KWndButton*)&button_ptr);
//
//	//*(this) = *dynamic_cast<Player_AutoPlay2*>((Player_AutoPlay2*)button_ptr);
//
//	
//	//void* window_ptr = new KWndWindow;
//	//void* image_ptr = new KWndImage;
//	//void* button_ptr = new KWndButton;
//	
//	//CreateWndImage((unsigned int*)image_ptr);
//	//image_ptr = this;
//	//void* window_ptr = (void*)0x63DB6C; // bass class 0x67C03C, 
//	//void** image_ptr = (void**)0x67C024;
//	//void* button_ptr = (void*)0x63CC74; // 0x63CC74 vtable, 0x67D0C4 bass class
//	//window_ptr = static_cast<KWndWindow*>((KWndWindow*)this);
//	//window_ptr = reinterpret_cast<KWndWindow*>((KWndWindow*)this);
//	//image_ptr = reinterpret_cast<KWndImage*>((KWndImage*)this);
//	//button_ptr = reinterpret_cast<KWndButton*>((KWndButton*)this);
//	//*(unsigned int*)window_ptr = *(unsigned int*)this;
//	//*(BYTE*)button_ptr = *(BYTE*)this;
//	//CreateWndWindow((BYTE*)window_ptr);
//	//RefCountBaseClass((BYTE*)this, 340, 1, (void*)RefWndWindow, (void*)RefWndBase);
//	//RefCountBaseClass((BYTE*)this, 340, 1, (void*)RefWndWindow, (void*)RefWndBase);
//	//RefCountBaseClass((unsigned int*)this, 480, 1, (void*)RefWndImage2, (void*)RefWndBase);
//	//RefCountBaseClass((BYTE*)this, 592, 1, (void*)RefWndButton, (void*)RefWndBase);
//	
//	//ofstream myfile;
//	//myfile.open("E:\\Player_AutoPlay2.txt");
//	//myfile << "GetParent : " << (DWORD*)this << "\n"; // skill id
//	//myfile.close();
//}

//IMPLEMENT_COMCLASS(Player_AutoPlay2)
void Player_AutoPlay2::OnLBtnDown(int x, int y)
{
	BYTE* Curren = (BYTE*)this;

	ofstream myfile;
	myfile.open("E:\\Player_AutoPlay2_OnLBtnDown.txt");
	//myfile << "m_pParentWnd : " << this->m_pParentWnd << "'\n";
	myfile << "This  : " << (DWORD*)this << "'\n";
	myfile << "x : " << x << "'\n";
	myfile << "y : " << y << "'\n";
	//myfile << "IsVisible : " << this->m_Style << "'\n";
	//myfile << "IsVisible : " << this->IsVisible() << "'\n";
	//myfile << "IsVisible : " << (this->m_Style == WND_S_VISIBLE) << "'\n";
	myfile.close();

	/*if (!this->IsVisible())
		KUiAutoPlay::OpenWindow();
	else
		KUiAutoPlay::CloseWindow(false);*/
}

void Player_AutoPlay2::OnButtonClick()
{
	BYTE* Curren = (BYTE*)this;

	ofstream myfile;
	myfile.open("E:\\Player_AutoPlay2_OnButtonClick.txt");
	//myfile << "m_pParentWnd : " << this->m_pParentWnd << "'\n";
	myfile << "This size : " << *((unsigned int*)((unsigned int*)Curren) + 1) << "'\n";
	myfile << "IsVisible : " << Curren << "'\n";
	//myfile << "IsVisible : " << this->m_Style << "'\n";
	//myfile << "IsVisible : " << this->IsVisible() << "'\n";
	//myfile << "IsVisible : " << (this->m_Style == WND_S_VISIBLE) << "'\n";
	myfile.close();

	/*if (!this->IsVisible())
		KUiAutoPlay::OpenWindow();
	else
		KUiAutoPlay::CloseWindow(false);*/
}

int	Player_AutoPlay2::Init(KIniFile* pIniFile, const char* pSection)
{
	ofstream myfile;
	myfile.open("E:\\Player_AutoPlay2_Init.txt");
	myfile << "m_Width : " << pIniFile << "'\n";
	myfile << "m_Height : " << pSection << "'\n";
	myfile.close();

	return KWndButton_Init(this, pIniFile, pSection);

	/*int (TempButton::**CastInit)(KIniFile*, const char*) = (int(TempButton::**)(KIniFile*, const char*))0x425FB0;
	return (this->**CastInit)(pIniFile, pSection);*/
	//return TempButton::Init(pIniFile, pSection);
	/*if (KWndButton_Init(this, pIniFile, pSection))
	{

	}*/

	//if (KWndImage_Init(this, pIniFile, pSection))
	//{
	//	/*if (KWndButton::Init(pIniFile, pSection))
	//	{

	//	}*/
	//
	//}
	/*if (KWndWindow_Init(this, pIniFile, pSection))
	{
		if (KWndImage::Init(pIniFile, pSection))
		{

		}
	}*/
	/*if (KWndImage::Init(pIniFile, pSection))
	{

	}*/
	//if (KWndButton::Init(pIniFile, pSection))
	//{
	//	//*(void**)this = +((char**)((void**)0x63DB6C));
	//}
	return 1;
}

const char* Player_AutoPlay2::GetShortKey()
{
	/*return KShortcutKeyCentre::GetKeyName(
		KShortcutKeyCentre::GetCommandKey(
			KShortcutKeyCentre::FindCommandByScript("Open([[auto]])")));*/
	return NULL;
}

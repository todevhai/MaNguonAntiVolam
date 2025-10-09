#include "NewUiWindow.h"
#include "NewUiImage.h"
#include "NewUiButton.h"
#include <Elem/WndMessage.h>
//#include <Elem/Wnds.h>
#include <fstream>
#include <Text.h>
#include "KIniFile.h"
#include "Util.h"
#include <memory>

using namespace std;
//#include "NewMouseHover.h"

//using namespace std;

//extern iRepresentShell* g_pRepresentShell;

//KWndWindow gWndWindow;

int WND_SHOW_DEBUG_FRAME_TEXT = false;

#define	MOSE_HOVER_TIME_VALVE		1000	
//void CallWindow(KWndWindow* gWndWindow) {
//
//	void* vptr_WndWindow = (void*)0x63DB6C;
//
//	vptr_WndWindow = new KWndWindow;
//
//	gWndWindow = dynamic_cast<KWndWindow*>((KWndWindow*)vptr_WndWindow);
//}
//typedef void (*KWndWindow)();
//extern KWndWindow* gWndWindow;
//KWndWindow* gWndWindow;
//KWndWindow* KWndWindow::* = (*0x63DB6C)();
//KWndWindow* gWndWindow = (KWndWindow*)CreateWndWindow((BYTE*)(KWndWindow*)gWndWindow);
//template <typename KWndWindow>
//void call_Install(base& obj)
class KIniFile;
//IMPLEMENT_COMCLASS(KWndWindow)
//RefWndBase fnRefWndBase = (RefWndBase)0x44ADC0;
//BaseWndWindow fnBaseWndWindow = (BaseWndWindow)0x441A60;
//VtableWndWindow fnVtableWndWindow = (VtableWndWindow)0x441B80;

KWndWindow::KWndWindow()
{
	//void* vptr_WndWindow = (void*)0x65A50C;
	//void* vptr_WndWindow = *((void**)0x63DB6C);
	//*(void**)this = (static_cast<void*>((void**)0x63DB6C));
	//void* vptr_WndWindow = *((void**)0x659078);
	//vptr_WndWindow = ((void**)this);

	//*(void**)this = +*(static_cast<void**>(*((void**)0x659078)));
	//*(void**)this = +*(static_cast<void**>(*((void**)0x63DB68)));

	//*(void**)this = +(static_cast<void*>(((void**)0x659078)));
	//KWndWindow::Init = (ptnInit)0x442060;
	//*(void**)this = +*(static_cast<void**>(*((void**)0x63DB6C)));
	//SetCompleteHook(0xE8, (DWORD)&KWndWindow::Init, 0x442060);
	//*this = **((KWndWindow**)0x659078);


	//*this = *dynamic_cast<KWndWindow*>(*((void**)0x659078);


	//*this = *dynamic_cast<KWndWindow*>(*(KWndWindow**)0x659078);
	//*this = *reinterpret_cast<KWndWindow*>(*(KWndWindow**)0x659078);
	//CreateWndWindow(*(BYTE**)this);
	//*this = *static_cast<KWndWindow*>(*((KWndWindow**)0x659078));
	//*this = *static_cast<KWndWindow*>(*((KWndWindow**)0x65A50C));
	//*this = *static_cast<KWndWindow*>(*(KWndWindow**)0x63DB68);
	//*this = *static_cast<KWndWindow*>(*(KWndWindow**)vptr_WndWindow);
	//RefCountBaseClass(this, 340, 1, fnBaseWndWindow, fnRefWndBase);
	/*unsigned int baseVTable = *(unsigned int*)vptr_WndWindow;
	unsigned int baseVTable2 = baseVTable + 0x4;
	unsigned int objectVTable = *(unsigned int*)this;
	unsigned int objectVTable2 = objectVTable + 0x4;*/
	//*((KWndWindow*)vptr_WndWindow) = *dynamic_cast<KWndWindow*>(this);
	//*(unsigned int**)(baseVTable2) = &objectVTable2;
	//void* window_ptr = (void*)0x63DB6C;
	//void** image_ptr = (void**)0x67C024;
	//void* button_ptr = (void*)0x67D0C4;
	//window_ptr = this;
	//window_ptr = dynamic_cast<KWndWindow*>((KWndWindow*)this);
	//void* window_ptr = (BYTE*)0x63DB6C;
	//void* image_ptr = (unsigned int*)0x63CF4C;
	//void* button_ptr = (void*)0x63CBCC;
	//image_ptr = this;
	//image_ptr = static_cast<void**>(window_ptr);
	//window_ptr = static_cast<KWndWindow*>(this);

	//CreateWndWindow((DWORD*)this);
	
	//KWndWindow* base_WndWindow = (KWndWindow*)0x659078; // 0x67D0C4
	//KWndWindow* array_WndWindow = (KWndWindow*)0x6590A4; // 0x67D0C4
	//KWndWindow* vptr_WndWindow = (KWndWindow*)0x65A50C; // 0x67D0C4

	//KWndWindow& vptr_WndWindow_4 = *(KWndWindow*)0x441A60; // Base Class Descriptor at (0, -1, 0, 64)
	//KWndWindow& vptr_WndWindow_3 = *(KWndWindow*)0x65B36C; // Base Class Descriptor at (0, -1, 0, 64)

	//KWndWindow* vptr_WndWindow = (KWndWindow*)0x659078; // Base Class Descriptor at (0, -1, 0, 64)

	//KWndWindow& vtable_WndWindow = *(KWndWindow*)0x63DB6C;
	 
	//KWndWindow* vtable_WndWindow = (KWndWindow*)0x63DB6C;
	
	//KWndWindow* vptr_WndWindow = (KWndWindow*)0x67C03C;
	
	//KWndWindow* vptr_WndWindow = (KWndWindow*)0x65A50C;

	//KWndWindow* vtable_WndWindow = (KWndWindow*)0x67C03C;

	//reinterpret_cast<KWndWindow*>((KWndWindow*)this);

	//vtable_WndWindow = ((void*)this);
	//vtable_WndWindow = this;

	//KWndWindow* vptr_WndWindow = new KWndWindow;

	//base_WndWindow = this;

	//dynamic_cast<KWndWindow*>((KWndWindow*)0x65A50C);

	//*this = *static_cast<KWndWindow*>(array_WndWindow);
	//CreateWndWindow((DWORD*)base_WndWindow);

	//void (*EntryProc)(void*) = ((void(*)(void*)) reinterpret_cast<void*>((void*)0x441A60));

	//EntryProc(this);

	//vptr_WndWindow = new KWndImage;
 
	//KWndWindow instance;

	//vptr_WndWindow = instance;

	//*((KWndWindow*)(DWORD*)((BYTE*)this)) = *static_cast<KWndWindow*>((KWndWindow*)(void*)0x63DB6C);
	//CreateWndWindow((DWORD*)((DWORD*)this));
	
	//*((KWndWindow*)0x65A50C)  = *static_cast<KWndWindow*>(this);

	//* (void**)gWndWindow = +((char**)((void**)0x63DB6C));
	//* (void**)gWndWindow = +((char**)((void***)0x63DB6C));
	//* (void**)gWndWindow = +((char**)((void***)0x65A50C));
	
	//*this = *static_cast<KWndWindow*>((KWndWindow*)CreateWndWindow((DWORD*)this));
	//*(void**)this = static_cast<void**>(+((void**)((void**)0x63DB6C)));
	//*(DWORD**)this = +*((DWORD**)((void**)0x63DB6C));
	

	//*(this) = **static_cast<KWndWindow**>((KWndWindow**)((void*)0x63DB6C));
	//*(this) = reinterpret_cast<KWndWindow&>(vptr_WndWindow_3);

	//*(this) = *dynamic_cast<KWndWindow*>((KWndWindow*)((void*)0x63DB6C) + 8);
	//*(this) = *static_cast<KWndWindow*>((KWndWindow*)((void*)0x63DB6C));
	//*(this) = reinterpret_cast<KWndWindow&>(vptr_WndWindow_3);
	//*(this) = *static_cast<KWndWindow*>((KWndWindow*)(CreateWndWindow((DWORD*)vptr_WndWindow)));
	//static_cast<KWndWindow**>((KWndWindow**)((void*)0x441A60));
	//*(void**)this = +*(static_cast<void**>((void**)((void*)0x441A60)));

	//*(this) = static_cast<KWndWindow&>((KWndWindow&)vptr_WndWindow);

	//*(KWndWindow*)this = *static_cast<const KWndWindow*>((const KWndWindow*)0x65A50C);

	//**((KWndWindow**)this) = *static_cast<KWndWindow*>(*(KWndWindow**)0x65A50C);
	//**((KWndWindow**)this) = *static_cast<KWndWindow*>(*(KWndWindow**)0x65A50C);
	//*(this) = *static_cast<KWndWindow*>((KWndWindow*)((void*)0x65A50C));
	//*(KWndWindow*)(*(DWORD*)this) = *static_cast<KWndWindow*>((KWndWindow*)*(void***)(((char*)0x63DB6C) + 1));

	//void* image_ptr = new KWndImage;
	//void* button_ptr = new KWndButton;
	//*(void**)this = dynamic_cast<KWndImage*>((KWndImage*)image_ptr);
	//*(void**)this = dynamic_cast<KWndImage*>((KWndImage*)button_ptr);

	//*(KWndWindow*)(*(DWORD*)this) = *static_cast<KWndWindow*>((KWndWindow*)((char*)((void*)0x63DB6C)));
	//*((DWORD*)this) = *static_cast<DWORD*>((void*)0x63DB6C);
	//*((KWndWindow*)this) = *dynamic_cast<KWndWindow*>((KWndWindow*)(void*)0x63DB6C);
	//*((KWndWindow*)this + 8) = *dynamic_cast<KWndWindow*>((KWndWindow*)((void*)0x63DB6C) + 8);

	//static_cast<KWndWindow&>(*((KWndWindow*)0x67C03C));

	//BYTE* window_ptr = (BYTE*)0x63DB6C;
	//window_ptr = (BYTE*)this;
	//gWndWindow = reinterpret_cast<KWndWindow*>(window_ptr);

	////*((BYTE*)&m_Caption[0]) = 0;
	////*(m_Caption[0]) = 0;
	////*(&m_Caption[0]) = 0;
	//m_Caption[0] = 0;
	////m_Caption[31] = 0;
	//m_Left = 0;
	//m_Top = 0;
	//m_Width = 0;
	//m_Height = 0;
	//m_nAbsoluteLeft = 0;
	//m_nAbsoluteTop = 0;
	//m_bMoving = false;
	////*((BYTE*)&m_bMoving) = false;
	//m_nLastMouseHoldPosX = m_nLastMouseHoldPosY = 0;
	////*&(m_pData19[0]) = -1;
	////*&(m_pData19[1]) = -1;
	////*((unsigned int*)&m_pData19[0]) = -1;
	////*((unsigned int*)&m_pData19[1]) = -1;
	////*(DWORD*)&m_pPreviousWnd->m_Caption = -1;
	////*(DWORD*)&m_pNextWnd->m_Caption = -1;
	//m_pPreviousWnd = NULL;
	//m_pNextWnd = NULL;
	//m_pFirstChild = NULL;
	//m_pParentWnd = NULL;
	//m_Style = WND_S_VISIBLE;
	//for (int i = 0; i < 2; ++i)
	//	*(DWORD*)&vptr[4 * i + 76] = -1;

	//*((DWORD*)m_Caption + 55) = 0;

	////memset(&m_szTip, 0, 64);
	//////memset(((unsigned int*)&m_Caption[0]), 0, 32);
	//////strncpy(&m_Caption[31], 0, 1);
	//////m_Caption[31] = 0;
	//////memset(m_Caption, 0, sizeof(m_Caption) - 1);
	//////m_Caption[sizeof(m_Caption) - 1] = 0;
	//////*((unsigned int*)((BYTE*)&m_Caption[31])) = 0;
	/////*for (int i = 0; i < 32; ++i)
	////	*((unsigned int*)&m_Caption[i]) = 0;*/
	////
	//////strncpy(m_Caption, 0, 31);
	//////for (int i = 0; i < 2; ++i)
	//////	*((unsigned int*)&m_pData19[4 * i + 76]) = -1;

	//
	////m_Caption[31] = 0;
	//////*(DWORD*)m_pPreviousWnd->m_pPreviousWnd = -1;
	//////*(DWORD*)m_pPreviousWnd->m_pNextWnd = -1;
	//// 
	//////**(unsigned int**)&m_pData19[0] = -1;
	//////**(unsigned int**)&m_pData19[1] = -1;
	//
	////*((DWORD*)m_Caption[31]) = 0;
	////*(DWORD*)(m_Caption[31]) = 0;
	////*(&m_Caption[0]) = 0;
	////*((DWORD*)((BYTE*)&m_Caption[31])) = 0;
	////*((unsigned int*)&m_Caption[31]) = 0;
	////*(unsigned int*)m_Caption[31] = 0;
}

//KWndWindow::~KWndWindow()
//{
//	//static_cast<KWndWindow*>((KWndWindow*)((void*)((DWORD*)0x63DB6C)))->~KWndWindow();
//	//dynamic_cast<KWndWindow*>((KWndWindow*)0x63DB6C)->~KWndWindow();
//	//*((KWndWindow*)this) = *static_cast<KWndWindow*>(((void*)0x63DB6C));
//	//*((KWndWindow*)this + 1) = *dynamic_cast<KWndWindow*>((KWndWindow*)((void*)0x63DB6C) + 1);
//	
//	//static_cast<void**>(*((void***)(((char*)0x63DB6C) + 0)));
//	//*(KWndWindow*)(*(DWORD*)this) = *static_cast<KWndWindow*>((KWndWindow*)((char*)((void*)0x63DB6C)));
//	//ReleaseWndWindow((DWORD*)this);
//	//LeaveAlone();
//	//Wnd_OnWindowDelete(this);
//	//g_MouseOver.OnWndClosed((void*)this);
//}

//void KWndWindow::Destroy()
//{
//	m_Style |= WND_S_TOBEDESTROY;
//}

void KWndWindow::Clone(KWndWindow* pCopy)
{
	if (pCopy)
	{
		pCopy->m_Width = m_Width;
		pCopy->m_Height = m_Height;
		pCopy->m_Style = m_Style;
		pCopy->SetPosition(m_Left, m_Top);
	}
}

//int KWndWindow::Init(KIniFile* pIniFile, const char* pSection)
//{
//	//pIniFile, pSection
//	
//	//int result = dynamic_cast<KWndWindow*>(this)->Init(pIniFile, pSection);
//	return static_cast<int>(this, KWndWindow::Init(pIniFile, pSection), Init(pIniFile, pSection));
//	//return reinterpret_cast<int>(this->Init(pIniFile, pSection), pIniFile, pSection);
//	//return result;
//	//*((KWndWindow*)this) = *static_cast<KWndWindow*>(((void*)0x63DB6C));
//	//*((KWndWindow*)((DWORD*)this + 3)) = *dynamic_cast<KWndWindow*>((KWndWindow*)((void*)((DWORD*)0x63DB6C + 3)));
//	//*((DWORD*)(KWndWindow*)this) = static_cast<KWndWindow*>((KWndWindow*)((void*)((DWORD*)0x63DB6C + 3)))->Init(pIniFile, pSection);
//	
//	/*ofstream myfile;
//	myfile.open("E:\\KWndWindow_Init.txt");
//	myfile << "This : " << (DWORD*)this << "'\n";
//	myfile << "This : " << sizeof(KWndWindow) << "'\n";
//	myfile << "This size : " << pIniFile << "'\n";
//	myfile.close();*/
//	//*(int*)(this)->Init(pIniFile, pSection) = dynamic_cast<KWndWindow*>((KWndWindow*)((void*)((DWORD*)0x63DB6C)))->Init(pIniFile, pSection);
//	//return Init(pIniFile, pSection);
//	//*(KWndWindow*)(*(DWORD*)this) = *static_cast<KWndWindow*>((KWndWindow*)((void*)0x63DB6C) + 3);
//	//static_cast<void**>(*((void***)(((char*)0x63DB6C) + 3)));
//	//DWORD oldProtection1, oldProtection2, oldProtection3;
//	//int		nValue1, nValue2;
//	//unsigned int value; // edx
//	//int				m_nTipLen;
//	//if (pIniFile && pSection)
//	//{
//		//return KWndWindow_Init(this, pIniFile, pSection);
//	//	strncpy(m_Caption, pSection, 32);
//	//	pIniFile->GetInteger(pSection, "Width", 0, &m_Width);
//	//	pIniFile->GetInteger(pSection, "Height", 0, &m_Height);
//	//	pIniFile->GetInteger(pSection, "Left", 0, &nValue1);
//	//	pIniFile->GetInteger(pSection, "Top", 0, &nValue2);
//	//	SetPosition(nValue1, nValue2);
//	//	pIniFile->GetInteger(pSection, "Disable", 0, &nValue1);
//	//	pIniFile->GetInteger(pSection, "Moveable", 0, &nValue2);
//
//	//	//VirtualProtect((void*)m_Style, 4, PAGE_EXECUTE_READWRITE, &oldProtection1);
//	//	//VirtualProtect((void*)0x63A808, 2000, PAGE_EXECUTE_READWRITE, &oldProtection2);
//
//	//	if (nValue1)
//	//		value = m_Style | WND_S_DISABLE;
//	//	else
//	//		value = m_Style & 0xFBFFFFFF;
//
//	//	m_Style = value;
//
//	//	value = 0;
//
//	//	if (nValue2)
//	//		value = m_Style | WND_S_MOVEALBE;
//	//	else
//	//		value = m_Style & 0xEFFFFFFF;
//
//	//	myfile << "Disable : " << nValue1 << "'\n";
//
//	//	myfile << "value : " << value << "'\n";
//
//	//	m_Style = value;
//
//	//	nValue1 = nValue2 = 0;
//
//	//	value = 0;
//	//	//pIniFile->GetInteger2(pSection, "FollowMove", &nValue1, &nValue2);
//	//	//b_GetInteger2(pIniFile, pSection, "FollowMove", &nValue1, &nValue2);
//	//	//
//	//	//if (nValue1)
//	//	//	value = (unsigned int)0x1000000 | m_Style;
//	//	//else
//	//	//	value = m_Style & 0xFEFFFFFF;
//
//	//	//m_Style = value;
//	//	//value = 0;
//	//	//if (nValue2)
//	//	//	value = (unsigned int)0x00800000 | m_Style;
//	//	//else
//	//	//	value = m_Style & 0xFF7FFFFF;
//
//	//	//m_Style = value;
//	//	//value = 0;
//	//	//nValue1 = nValue2 = 0;
//	//	////pIniFile->GetInteger2(pSection, "FollowSize", &nValue1, &nValue2);
//	//	//b_GetInteger2(pIniFile, pSection, "FollowSize", &nValue1, &nValue2);
//	//	//myfile.close();
//	//	//if (nValue1)
//	//	//	value = (unsigned int)0x00400000 | m_Style;
//	//	//else
//	//	//	value = m_Style & 0xFFBFFFFF;
//
//	//	//m_Style = value;
//	//	//value = 0;
//	//	//if (nValue2)
//	//	//	value = m_Style | WND_S_SIZE_WITH_B_EDGE;
//	//	//else
//	//	//	value = m_Style & 0xFFDFFFFF;
//
//	//	//m_Style = value;
//	//	//m_Style = m_Style & 0xFEFFFFFF;
//	//	//m_Style = m_Style & 0xFF7FFFFF;
//	//	//m_Style = m_Style & 0xFFBFFFFF;
//	//	//m_Style = m_Style & 0xFFDFFFFF;
//
//	//	value = 0;
//
//	//	pIniFile->GetInteger(pSection, "DummyWnd", 0, &nValue1);
//
//	//	if (nValue1 == 0)
//	//		value = m_Style & WND_S_SIZE_WITH_ALL_CHILD;
//	//	else
//	//		value = m_Style | 0xFFEFFFFF;
//
//	//	m_Style = value;
//
//	//	value = 0;
//
//	//	/*if (!m_nOffset26)
//	//		m_nOffset26 = m_pParentWnd;
//	//	if (!m_nOffset27)
//	//		m_nOffset27 = m_nOffset28;
//	//	if (!m_nOffset28)
//	//		m_nOffset28 = m_bMoving + m_pParentWnd;
//	//	if (!m_nOffset29)
//	//		m_nOffset29 = m_pData19[1] + m_nOffset28;*/
//
//	///*	if (!m_nOffset104)
//	//		m_nOffset104 = m_pParentWnd;
//	//	if (!m_nOffset108)
//	//		m_nOffset108 = m_nOffset28;
//	//	if (!m_nOffset112)
//	//		m_nOffset112 = m_bMoving + m_pParentWnd;
//	//	if (!m_nOffset116)
//	//		m_nOffset116 = m_pData19[1] + m_nOffset28;*/
//
//	///*	m_nTipLen = 0;
//
//	//	if (pIniFile->GetString(pSection, "Tip", "", m_szTip, sizeof(m_szTip)))
//	//	{
//	//		m_nTipLen = TEncodeText(m_szTip, strlen(m_szTip));
//	//	}
//	//	char m_szTipColor[260];
//	//	pIniFile->GetString(pSection, "TipColor", "255,255,255", m_szTipColor, 16);
//
//	//	m_nTipColor = _GetColorTip(m_szTipColor);*/
//
//	//	//VirtualProtect(&m_Style, 4, oldProtection1, 0);
//	//	//VirtualProtect((void*)0x63A808, 2000, oldProtection2, 0);
//
//	//	return true;
//	//}
//	
//	//return false;
//}

//int KWndWindow::PtInWindow(int x, int y)
//{
//	/*int nRet = 0;
//	if (m_Style & WND_S_VISIBLE)
//	{
//		if ((m_Style & WND_S_SIZE_WITH_ALL_CHILD) == 0)
//		{
//			nRet = (x >= m_nAbsoluteLeft && y >= m_nAbsoluteTop &&
//				x < m_nAbsoluteLeft + m_Width && y < m_nAbsoluteTop + m_Height);
//		}
//		else
//		{
//			KWndWindow* pChild = m_pFirstChild;
//			while (pChild)
//			{
//				if (pChild->PtInWindow(x, y))
//				{
//					nRet = 1;
//					break;
//				}
//				pChild = pChild->m_pNextWnd;
//			};
//		}
//	}
//	return nRet;*/
//	return KWndWindow_PtInWindow((unsigned int*)this, x, y);
//}

//void KWndWindow::AbsoluteMove(int dx, int dy)
//{
//	m_nAbsoluteLeft += dx;
//	m_nAbsoluteTop += dy;
//	KWndWindow* pChild = m_pFirstChild;
//	while (pChild)
//	{
//		pChild->AbsoluteMove(dx, dy);
//		pChild = pChild->m_pNextWnd;
//	}
//}

//void KWndWindow::GetPosition(int* pLeft, int* pTop)
//{
//	if (pLeft)
//		*pLeft = m_Left;
//	if (pTop)
//		*pTop = m_Top;
//}
//
//void KWndWindow::SetCursorAbove()
//{
//	Wnd_SetCursorPos(m_nAbsoluteLeft + m_Width / 2,
//		m_nAbsoluteTop + m_Height / 2);
//}

void KWndWindow::Show()
{
	KWndWindow_Show((unsigned int*)this);
	//m_Style |= WND_S_VISIBLE;
}

void KWndWindow::Hide()
{
	KWndWindow_Hide((unsigned int*)this);
	//m_Style &= ~WND_S_VISIBLE;
	////g_MouseOver.OnWndClosed((void*)this);
	//KWndWindow* pFocus = Wnd_GetFocusWnd();
	//while (pFocus)
	//{
	//	if (pFocus != this)
	//		pFocus = pFocus->m_pParentWnd;
	//	else
	//	{
	//		Wnd_SetFocusWnd(NULL);
	//		break;
	//	}
	//}
	//Wnd_ReleaseExclusive(this);
}

void KWndWindow::SetSize(int nWidth, int nHeight)
{
	KWndWindow_SetSize((unsigned int*)this, nWidth, nHeight);
	//if (nWidth < 0)
	//	nWidth = 0;
	//if (nHeight < 0)
	//	nHeight = 0;
	//int nDX = nWidth - m_Width;
	//int nDY = nHeight - m_Height;
	//if (nDX == 0 && nDY == 0)
	//	return;
	//m_Width = nWidth;
	//m_Height = nHeight;

	//KWndWindow* pChild = m_pFirstChild;
	//while (pChild)
	//{
	//	int	x, y, w, h;
	//	pChild->GetPosition(&x, &y);
	//	pChild->GetSize(&w, &h);
	//	if (nDX)
	//	{
	//		if (pChild->m_Style & WND_S_SIZE_WITH_R_EDGE)
	//			pChild->SetSize(w + nDX, h);
	//		if (pChild->m_Style & WND_S_MOVE_WITH_R_EDGE)
	//			pChild->SetPosition(x + nDX, y);
	//	}
	//	if (nDY)
	//	{
	//		if (pChild->m_Style & WND_S_SIZE_WITH_B_EDGE)
	//			pChild->SetSize(w, h + nDY);
	//		if (pChild->m_Style & WND_S_MOVE_WITH_B_EDGE)
	//			pChild->SetPosition(x, y + nDY);
	//	}
	//	pChild = pChild->m_pNextWnd;
	//};

	////to be check.!!!!!
	////一些窗口类可能会有根据窗口大小计算出一些变量保存下来供后继运算使用，
	////目前没有提供大小改变的通知消息，可能那些窗口的行为会有未知的结果。
	////但是目前情况来看那样的窗口一般不会被调用SetSize。
}

//void KWndWindow::SetPosition(int nLeft, int nTop)
//{
//	AbsoluteMove(nLeft - m_Left, nTop - m_Top);
//	m_Left = nLeft;
//	m_Top = nTop;
//}
//
//void KWndWindow::GetAbsolutePos(int* pLeft, int* pTop)
//{
//	if (pLeft)
//		*pLeft = m_nAbsoluteLeft;
//	if (pTop)
//		*pTop = m_nAbsoluteTop;
//}

//void KWndWindow::GetSize(int* pWidth, int* pHeight)
//{
//	if (pWidth)
//		*pWidth = m_Width;
//	if (pHeight)
//		*pHeight = m_Height;
//}

void KWndWindow::Paint()
{
	KWndWindow_Paint(*(LPVOID**)this);
	/*if (m_Style & WND_S_VISIBLE)
	{
		PaintWindow();
		if (m_pFirstChild)
			m_pFirstChild->Paint();
	}
	if (m_pNextWnd)
		m_pNextWnd->Paint();*/
}

void KWndWindow::LetMeBreathe()
{
	if (m_pNextWnd)
		m_pNextWnd->LetMeBreathe();

	if (m_Style & WND_S_TOBEDESTROY)
		delete this;
	/*else if ((m_Style & (WND_S_VISIBLE | WND_S_DISABLE)) == WND_S_VISIBLE)
		Breathe();*/
}

//void KWndWindow::BringToTop()
//{
//	if (m_pNextWnd)
//	{
//		KWndWindow* pLast = m_pNextWnd;
//		while (pLast->m_pNextWnd)
//			pLast = pLast->m_pNextWnd;
//
//		pLast->m_pNextWnd = this;
//		m_pNextWnd->m_pPreviousWnd = m_pPreviousWnd;
//		if (m_pPreviousWnd)
//			m_pPreviousWnd->m_pNextWnd = m_pNextWnd;
//		else if (m_pParentWnd && m_pParentWnd->m_pFirstChild == this)
//			m_pParentWnd->m_pFirstChild = m_pNextWnd;
//		m_pNextWnd = NULL;
//		m_pPreviousWnd = pLast;
//	}
//}

//int KWndWindow::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
//{
//	//*(KWndWindow*)(*(DWORD*)this) = *static_cast<KWndWindow*>((KWndWindow*)((char*)((void**)0x63DB6C) + 4));
//	ofstream myfile;
//	myfile.open("E:\\KWndWindow_WndProc.txt");
//	myfile << "This " << (DWORD*)this << "\n"; // skill id
//	myfile << "uMsg " << uMsg << "\n"; // skill id
//	myfile << "uParam " << uParam << "\n"; // skill id
//	myfile << "nParam " << nParam << "\n"; // skill id
//	//myfile << "result " << result << "\n"; // skill id
//	myfile.close();
//
//	/*int	nRet = 0;
//	switch (uMsg)
//	{
//	case WM_LBUTTONDOWN:
//		OnLBtnDown(LOWORD(nParam), HIWORD(nParam));
//		nRet = 1;
//		break;
//	case WM_RBUTTONDOWN:
//		if (m_pParentWnd)
//		{
//			m_pParentWnd->WndProc(WND_M_POPUPMENU, (unsigned int)(KWndWindow*)this, nParam);
//			nRet = 1;
//		}
//		break;
//	case WM_MOUSEMOVE:
//		if (m_bMoving)
//		{
//			OnMoveWnd();
//			nRet = 1;
//		}
//		break;
//	case WM_LBUTTONUP:
//		if (m_bMoving)
//		{
//			Wnd_ReleaseCapture();
//			m_bMoving = false;
//			if (m_pParentWnd)
//			{
//				m_pParentWnd->WndProc(WND_N_CHILD_MOVE, (unsigned int)(KWndWindow*)this, 0);
//			}
//		}
//		nRet = 1;
//		break;
//	case WND_M_CAPTURECHANGED:
//		m_bMoving = false;
//		nRet = 1;
//		break;
//	}
//
//	return nRet;*/
//	return KWndWindow_WndProc(this, uMsg, uParam, nParam);
//}

//void KWndWindow::OnLBtnDown(int x, int y)
//{
//	if (m_Style & WND_S_MOVEALBE)
//	{
//		m_bMoving = TRUE;
//		m_nLastMouseHoldPosX = x;
//		m_nLastMouseHoldPosY = y;
//		Wnd_SetCapture(this);
//	}
//}
//
//void KWndWindow::OnMoveWnd()
//{
//	int x, y;
//	Wnd_GetCursorPos(&x, &y);
//	if (x != m_nLastMouseHoldPosX || y != m_nLastMouseHoldPosY)
//	{
//		x -= m_nLastMouseHoldPosX;
//		y -= m_nLastMouseHoldPosY;
//		SetPosition(x + m_Left, y + m_Top);
//		m_nLastMouseHoldPosX += x;
//		m_nLastMouseHoldPosY += y;
//
//		x = ((x & 0xffff) | ((y & 0xffff) << 16));
//		if (x && m_pParentWnd)
//		{
//			m_pParentWnd->WndProc(WND_N_CHILD_MOVE, (unsigned int)(KWndWindow*)this, x);
//		}
//	}
//}

int KWndWindow::IsVisible()
{
	return (m_Style & WND_S_VISIBLE);
}

//void KWndWindow::LeaveAlone()
//{
//	SplitSmaleFamily();
//	while (m_pFirstChild)
//	{
//		m_pFirstChild->m_pParentWnd = NULL;
//		m_pFirstChild = m_pFirstChild->m_pNextWnd;
//	}
//}

KWndWindow* KWndWindow::GetOwner()
{
	KWndWindow* pWnd = this;
	while (pWnd->m_pParentWnd)
		pWnd = pWnd->m_pParentWnd;
	return pWnd;
}

//#ifdef _DEBUG
//void KWndWindow::SetCaption(char* pszCaption)
//{
//	if (pszCaption)
//		strncpy(m_Caption, pszCaption, 31);
//}
//#endif

//void KWndWindow::SplitSmaleFamily()
//{
//	if (IsVisible())
//	{
//		KWndWindow::Hide();
//		KWndWindow::Show();
//	}
//	if (m_pPreviousWnd)
//		m_pPreviousWnd->m_pNextWnd = m_pNextWnd;
//	else if (m_pParentWnd)
//		m_pParentWnd->m_pFirstChild = m_pNextWnd;
//	if (m_pNextWnd)
//		m_pNextWnd->m_pPreviousWnd = m_pPreviousWnd;
//	m_pPreviousWnd = NULL;
//	m_pNextWnd = NULL;
//	m_pParentWnd = NULL;
//}

//void KWndWindow::AddChild(KWndWindow* pChild)
//{
//	if (pChild)
//	{
//		pChild->SplitSmaleFamily();
//
//		//调整绝对坐标
//		pChild->AbsoluteMove(m_nAbsoluteLeft + pChild->m_Left - pChild->m_nAbsoluteLeft,
//			m_nAbsoluteTop + pChild->m_Top - pChild->m_nAbsoluteTop);
//
//		//调整级连关系
//		pChild->m_pParentWnd = this;
//		if (m_pFirstChild == NULL)
//			m_pFirstChild = pChild;
//		else
//		{
//			KWndWindow* pBrother = m_pFirstChild;
//			while (pBrother->m_pNextWnd)
//				pBrother = pBrother->m_pNextWnd;
//			pBrother->m_pNextWnd = pChild;
//			pChild->m_pPreviousWnd = pBrother;
//		}
//	}
//}
//
//void KWndWindow::AddBrother(KWndWindow* pBrother)
//{
//	if (pBrother)
//	{
//		pBrother->SplitSmaleFamily();
//
//		//调整绝对坐标
//		if (m_pParentWnd)
//		{
//			pBrother->AbsoluteMove(m_pParentWnd->m_nAbsoluteLeft + pBrother->m_Left - pBrother->m_nAbsoluteLeft,
//				m_pParentWnd->m_nAbsoluteTop + pBrother->m_Top - pBrother->m_nAbsoluteTop);
//		}
//
//		//调整级连关系
//		pBrother->m_pParentWnd = m_pParentWnd;
//		if (m_pNextWnd == NULL)
//		{
//			m_pNextWnd = pBrother;
//			pBrother->m_pPreviousWnd = this;
//		}
//		else
//		{
//			KWndWindow* pWnd = m_pNextWnd;
//			while (pWnd->m_pNextWnd)
//				pWnd = pWnd->m_pNextWnd;
//			pWnd->m_pNextWnd = pBrother;
//			pBrother->m_pPreviousWnd = pWnd;
//		}
//	}
//}

void KWndWindow::Enable(int bEnable)
{
	KWndWindow_Enable(this, bEnable);
	/*if (bEnable)
		m_Style &= ~WND_S_DISABLE;
	else
		m_Style |= WND_S_DISABLE;*/
}

//void KWndWindow::Enable(int bEnable)
//{
//	KWndWindow_Enable(this, bEnable);
//}

//void KWndWindow::PaintWindow()
//{
//	KWndWindow_PaintWindow(this);
//}

//void KWndWindow::PaintWindow()
//{
//	KWndWindow_PaintWindow(this);
//	/*if (m_bMoving)
//		OnMoveWnd();
//
//	if (g_pRepresentShell && WND_SHOW_DEBUG_FRAME_TEXT)
//	{
//		KRULine		Line[4];
//
//		for (int i = 0; i < 4; i++)
//		{
//			Line[i].Color.Color_dw = 0xff0000ff;
//			Line[i].oPosition.nX = Line[i].oEndPos.nX = m_nAbsoluteLeft;
//			Line[i].oPosition.nY = Line[i].oEndPos.nY = m_nAbsoluteTop;
//		}
//
//		Line[0].oEndPos.nX += m_Width;
//		Line[1].oPosition.nY += m_Height;
//		Line[1].oEndPos.nX += m_Width;
//		Line[1].oEndPos.nY += m_Height;
//		Line[2].oEndPos.nY += m_Height;
//		Line[3].oPosition.nX += m_Width;
//		Line[3].oEndPos.nX += m_Width;
//		Line[3].oEndPos.nY += m_Height;
//
//		g_pRepresentShell->DrawPrimitives(4, Line, RU_T_LINE, true);
//
//		if (m_Style & WND_S_SIZE_WITH_ALL_CHILD)
//		{
//			for (int i = 0; i < 4; i++)
//				Line[i].Color.Color_dw = 0xffff0000;
//			RECT	rc;
//			GetAllChildLayoutRect(&rc);
//			Line[0].oPosition.nX = Line[1].oPosition.nX = rc.left;
//			Line[0].oEndPos.nX = Line[1].oEndPos.nX = rc.right;
//			Line[0].oPosition.nY = Line[0].oEndPos.nY = rc.top;
//			Line[1].oPosition.nY = Line[1].oEndPos.nY = rc.bottom;
//
//			Line[0].oPosition.nY = Line[1].oPosition.nY = rc.top;
//			Line[0].oEndPos.nY = Line[1].oEndPos.nY = rc.bottom;
//			Line[0].oPosition.nX = Line[0].oEndPos.nX = rc.left;
//			Line[1].oPosition.nX = Line[1].oEndPos.nX = rc.right;
//
//			g_pRepresentShell->DrawPrimitives(4, Line, RU_T_LINE, true);
//		}
//
//#ifdef _DEBUG
//		g_pRepresentShell->OutputText(12, m_Caption, -1, m_nAbsoluteLeft, m_nAbsoluteTop, 0xFFFFFFFF);
//#endif
//	}*/
//}

//#ifdef _DEBUG
//void KWndWindow::SetCaption(char* pszCaption)
//{
//	if (pszCaption)
//		strncpy((char*)m_Caption, pszCaption, 31);
//}
//#endif

//void KWndWindow::PaintDebugInfo()
//{
//	char	szInfo[128];
//	szInfo[0] = 0;
//	int nInfoLen = 0;
//
//	int nOld = WND_SHOW_DEBUG_FRAME_TEXT;
//	WND_SHOW_DEBUG_FRAME_TEXT = true;
//	KWndWindow::PaintWindow();
//	WND_SHOW_DEBUG_FRAME_TEXT = nOld;
//
//#ifdef _DEBUG
//	sprintf(szInfo, "Name:%s, Pos:%d,%d,Size:%d,%d", m_Caption,
//		m_nAbsoluteLeft, m_nAbsoluteTop, m_Width, m_Height);
//	nInfoLen = strlen(szInfo);
//#else
//	sprintf(szInfo, "Pos:%d,%d,Size:%d,%d", m_nAbsoluteLeft, m_nAbsoluteTop, m_Width, m_Height);
//	nInfoLen = strlen(szInfo);
//#endif
//	if (m_Style & WND_S_SIZE_WITH_ALL_CHILD)
//	{
//		RECT	rc;
//		GetAllChildLayoutRect(&rc);
//		sprintf(&szInfo[nInfoLen], ",Rect:%d,%d-%d,%d", rc.left, rc.top,
//			rc.right, rc.bottom);
//		nInfoLen = strlen(szInfo);
//	}
//	int x, y, w, h;
//	Wnd_GetCursorPos(&x, &y);
//	Wnd_GetScreenSize(w, h);
//	if (x + nInfoLen * 6 + 24 > w)
//		x = w - nInfoLen * 6 - 24;
//	if (y + 25 > h)
//		y = h - 13;
//	else
//		y += 12;
//	g_pRepresentShell->OutputText(12, szInfo, nInfoLen, x, y, 0xFFFF0000,
//		0, TEXT_IN_SINGLE_PLANE_COORD, 0xffffffff);
//}

//void KWndWindow::GetAllChildLayoutRect(RECT* pRect)
//{
//	if (pRect == NULL)
//		return;
//	if (m_pFirstChild == NULL)
//	{
//		pRect->left = pRect->right = m_nAbsoluteLeft;
//		pRect->top = pRect->bottom = m_nAbsoluteTop;
//	}
//	else
//	{
//		m_pFirstChild->GetAbsolutePos((int*)&pRect->left, (int*)&pRect->top);
//		pRect->right = pRect->left;
//		pRect->bottom = pRect->top;
//
//		KWndWindow* pChild = m_pFirstChild;
//		do
//		{
//			RECT	rc;
//			pChild->GetAllChildLayoutRect(&rc);
//			int		x, y, w, h;
//			pChild->GetAbsolutePos(&x, &y);
//			pChild->GetSize(&w, &h);
//
//			if (rc.left > x)
//				rc.left = x;
//			if (rc.top > y)
//				rc.top = y;
//			if (rc.right < x + w)
//				rc.right = x + w;
//			if (rc.bottom < y + h)
//				rc.bottom = y + h;
//
//			if (pRect->left > rc.left)
//				pRect->left = rc.left;
//			if (pRect->top > rc.top)
//				pRect->top = rc.top;
//			if (pRect->right < rc.right)
//				pRect->right = rc.right;
//			if (pRect->bottom < rc.bottom)
//				pRect->bottom = rc.bottom;
//		} while (pChild = pChild->m_pNextWnd);
//	}
//}
//
//unsigned int GetColor(LPCTSTR pString)
//{
//	if (pString == NULL)
//		return 0;
//
//	unsigned int Color = 0xFF000000;
//
//	char Buf[16] = "";
//	int  i = 0;
//	int  n = 0;
//	while (pString[i] != ',')
//	{
//		if (pString[i] == 0 || n >= 15)
//			return Color;
//		Buf[n++] = pString[i++];
//	}
//
//	Buf[n] = 0;
//	Color += ((atoi(Buf) & 0xFF) << 16);
//	n = 0;
//	i++;
//	while (pString[i] != ',')
//	{
//		if (pString[i] == 0 || n >= 15)
//			return Color;
//		Buf[n++] = pString[i++];
//	}
//	Buf[n] = 0;
//	Color += ((atoi(Buf) & 0xFF) << 8);
//	n = 0;
//	i++;
//	while (pString[i] != 0)
//	{
//		if (n >= 15)
//			return Color;
//		Buf[n++] = pString[i++];
//	}
//	Buf[n] = 0;
//	Color += (atoi(Buf) & 0xFF);
//	return Color;
//}
//
//const char* GetColorString(unsigned int nColor)
//{
//	static char szColor[12];
//	KRColor c;
//	c.Color_dw = nColor;
//	sprintf(szColor, "%d,%d,%d", c.Color_b.r, c.Color_b.g, c.Color_b.b);
//	szColor[11] = 0;
//
//	return szColor;
//}
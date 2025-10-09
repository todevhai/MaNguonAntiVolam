#pragma once
#include <KWin32.h>
//#include "KCanvas.h"
#include <KIniFile.h>
////#include <KList.h>
//#include "iRepresentShell.h"
//#include "KRepresentUnit.h"
#include "Import.h"
//#include <vector>
////#pragma pack(push, 1)

#define WND_S_VISIBLE				0x80000000	//可见
#define WND_S_DISABLED				0x40000000	//不可操作
#define	WND_S_TOPMOST				0x20000000	//置顶窗口
#define	WND_S_MOVEALBE				0x10000000	//可被拖动
#define	WND_S_DISABLE				0x04000000	//窗口不可被操作
#define	WND_S_TOBEDESTROY			0x02000000	//window is to be destroy
#define	WND_S_MOVE_WITH_R_EDGE		0x01000000	//跟随父窗口右下边界的变化移动窗口
#define	WND_S_MOVE_WITH_B_EDGE		0x00800000	//跟随父窗口右下边界的变化移动窗口
#define	WND_S_SIZE_WITH_R_EDGE		0x00400000	//跟随父窗口大小的变化调整窗口大小
#define	WND_S_SIZE_WITH_B_EDGE		0x00200000	//跟随父窗口大小的变化调整窗口大小
#define	WND_S_SIZE_WITH_ALL_CHILD	0x00100000	//以子窗口的范围测试的集合作为自己的范围测试的集合

extern	int WND_SHOW_DEBUG_FRAME_TEXT;

class KIniFile;

//class KWndWindow;
//class KWndButton;

//struct many_vtable {
//	void(*dtor)(void*);
//	void(*print)(void const*, std::ostream&);
//};
typedef void (*caller)(void);
typedef void (*ptnUpdateData)(void);
typedef int (*ptnCloseWindow)(bool bDestory);
typedef int (*ptnInit)(KIniFile* pIniFile, const char* pSection);
typedef int (*ptnWndProc)(unsigned int uMsg, unsigned int uParam, int nParam);
typedef int (*ptnPtInWindow)(int x, int y);
typedef void (*ptnPaintWindow)(void);
typedef void (*ptnSetSize)(int nWidth, int nHeight);
typedef void (*ptnShow)(void);
typedef void (*ptnHide)(void);
typedef void (*ptnEnable)(int bEnable);
typedef void* (*ptnTopChildFromPoint)(int x, int y);
typedef void (*ptnBreathe)(void);
//typedef void (*KWndWindow)(void);
//typedef void(__thiscall* KWndWindow)(void);
//typedef KWndWindow* (*KWndWindow)();
// 340 428
//typedef KWndWindow* (KWndWindow::* (*KWndWindow)())(void*);

//template <class T>
//typedef KWndWindow::KWndWindow AnotherClass;
typedef struct KWndWindow KWndWindow;
typedef KWndWindow const* KWndWindowBase;
//template <typename KWndWindow>
//struct KWndWindow {
//public:
//	KWndWindow() {}
//};
//DECLARE_COMCLASS(KWndWindow)
struct IObjectVtable {
	int (*getValue)(KWndWindowBase*);
};

int makeObject(void**);

//template <typename KWndWindowBase>
class KWndWindow
{
	//ptnInit CastInit;
//public:
//	using KWndWindowBase = gWndWindow; // declare Type here
protected:
	unsigned int m_Style; // 1
	int			m_Left; // 2
	int			m_Top; // 3
	int			m_Width; // 4
	int			m_Height; // 5
	int			m_nAbsoluteLeft; // 6
	int			m_nAbsoluteTop; // 7
	char		m_Caption[32]; // 8
	int			m_bMoving; // 16
	int			m_nLastMouseHoldPosX; // 17
	int			m_nLastMouseHoldPosY; // 18
	int				vptr[41];
	KWndWindow* m_pPreviousWnd; // 21
	KWndWindow* m_pNextWnd; // 22
	KWndWindow* m_pFirstChild; // 23
	KWndWindow* m_pParentWnd; // 24
	
	//int				vptr[592];
//private:
//	int				vptr2[21];
public:
	//typedef int (*fnInit)(KIniFile* pIniFile, const char* pSection);
	KWndWindow();
	//KWndWindow() {}
	//virtual KWndWindow* first() { return static_cast<KWndWindow*>(this); }
	//virtual void* first() { return this; }
	//KWndWindow() { CreateWndWindow((BYTE*)this);}
	//KWndWindow() { RefCountBaseClass(this, 340, 1, (BaseWndWindow)0x441A60, (RefWndBase)0x44ADC0); }
	//virtual void Method1() {}
	//~KWndWindow() { DestroyWndWindow((DWORD*)this); }
	//virtual ~KWndWindow() { DestroyWndWindow((DWORD*)this); }
	virtual ~KWndWindow() {}
	//void	Destroy();
	virtual void	UpdateData() {}
	virtual int		CloseWindow(bool bDestory) { return 0; }
	//ptnInit Init;
	//virtual int		Init(KIniFile* pIniFile, const char* pSection);
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return KWndWindow_Init(this, pIniFile, pSection); }
	//virtual int		Init(KIniFile* pIniFile, const char* pSection) { return static_cast<KWndWindow*>(static_cast<KWndWindow*>(this))->Init(pIniFile, pSection); }
	//virtual int		Init(KIniFile* pIniFile, const char* pSection) { return dynamic_cast<KWndWindow*>(this)->Init(pIniFile, pSection); }
	//virtual int		Init(KIniFile* pIniFile, const char* pSection) { return dynamic_cast<KWndWindow*>(this)->Init(pIniFile, pSection); }
	//virtual int		Init(KIniFile* pIniFile, const char* pSection) { return CastInit(this, pIniFile, pSection); };
	/*virtual int		Init(KIniFile* pIniFile, const char* pSection) {
		int (__thiscall* CallInit)(void*, KIniFile*, const char*) = ((int(__thiscall*)(void*, KIniFile*, const char*))0x442060);
		return CallInit(this, pIniFile, pSection);
	}*/
	//virtual fnInit		Init(KIniFile* pIniFile, const char* pSection) const = 0;
	//virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return KWndWindow_WndProc(this, uMsg, uParam, nParam); }
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return static_cast<KWndWindow*>(this)->WndProc(uMsg, uParam, nParam); }
	//virtual int		PtInWindow(int x, int y) { return KWndWindow_PtInWindow(this, x, y); }
	virtual int		PtInWindow(int x, int y) { return static_cast<KWndWindow*>(this)->PtInWindow(x, y); }
	//virtual void	PaintWindow() { KWndWindow_PaintWindow(this); }
	virtual void	PaintWindow() { static_cast<KWndWindow*>(this)->PaintWindow(); }
	void			PaintDebugInfo() {}
	virtual void	SetSize(int nWidth, int nHeight);// {}
	void			SetPosition(int nLeft, int nTop) {}
	//void			GetAllChildLayoutRect(RECT* pRect);// {}
	//void			GetAbsolutePos(int* pLeft, int* pTop);// {}
	//void			GetSize(int* pWidth, int* pHeight);// {}
	void			BringToTop() { KWndWindow_BringToTop(this); }
	//void			GetPosition(int* pLeft, int* pTop);
	//void			SetCursorAbove();// {}
	virtual void	Show();// {}
	virtual void	Hide();// {}
	int				IsVisible();// { return 0; }
	virtual	void	Enable(int bEnable);// { KWndWindow_Enable(this, bEnable); }
	void			AddChild(KWndWindow* pChild) { KWndWindow_AddChild(this, pChild); }
	void			AddBrother(KWndWindow* pBrother) { KWndWindow_AddBrother(this, pBrother); }

	virtual KWndWindow* TopChildFromPoint(int x, int y) { return NULL; }

	KWndWindow* GetPreWnd() const { return m_pPreviousWnd; }	//得到前一个兄弟窗口
	KWndWindow* GetNextWnd() const { return m_pNextWnd; }		//得到后一个兄弟窗口
	KWndWindow* GetParent() const { return m_pParentWnd; }		//得到父窗口
	KWndWindow* GetFirstChild() const { return m_pFirstChild; }	//得到第一个子窗口
	KWndWindow* GetOwner();								//获得不再有父窗口的祖先窗口

	void			Paint();// {}
	void			LetMeBreathe();// {}

	//void			SplitSmaleFamily() {}
	//void			LeaveAlone();// {}
//#ifdef _DEBUG
	//void			SetCaption(char* pszCaption);
//#endif
	int				IsDisable() { return (m_Style & WND_S_DISABLE); }
	//int				GetStyle() { return m_Style; }
	/*int				SetStyle(unsigned int nStyle)
	{
		m_Style = nStyle;
		return m_Style;
	}*/
protected:
	void			Clone(KWndWindow* pCopy);

private:
	virtual void	Breathe() {}
//	void			OnLBtnDown(int x, int y);
//	void			OnMoveWnd();// {}
//
//private:
//	void			AbsoluteMove(int dx, int dy) {}

};

//typedef KWndWindow* KWndWindow;

//unsigned int	GetColor(const char* pString);

//const char* GetColorString(unsigned int nColor);

//#pragma pack(pop)

//extern KWndWindow gWndWindow;

//#pragma pack(push, 1)
//template <typename T, class Allocator = std::allocator<T>>

//typedef class KWndWindow
//{
//protected:
//	unsigned int m_Style;
//	int			m_Left; // 2
//	int			m_Top; // 3
//	int			m_Width; // 4
//	int			m_Height; // 5
//	int			m_nAbsoluteLeft; // 6
//	int			m_nAbsoluteTop; // 7
//	char		m_Caption[32]; // 8
//	int			m_bMoving; // 16
//	int			m_nLastMouseHoldPosX; // 17
//	int			m_nLastMouseHoldPosY; // 18
//	KWndWindow* m_TopChildFromPoint; // 19
//	KWndWindow* m_pPreviousWnd; // 21
//	KWndWindow* m_pNextWnd; // 22
//	KWndWindow* m_pFirstChild; // 23
//	KWndWindow* m_pParentWnd; // 24
//	BYTE				vptr[160];
//	//int				vptr[592];
////private:
////	int				vptr2[21];
//public:
//	//typedef KWndWindow ptKWndWindow;
//	//typedef KWndWindow KWndWindow;
//	KWndWindow() { CreateWndWindow((DWORD*)this); }
//	//virtual KWndWindow* first() { return static_cast<KWndWindow*>(this); }
//	//virtual void* first() { return this; }
//	//KWndWindowTemp() { }
//	//BaseWndWindow fnBaseWndWindow();
//	//BaseWndWindow fnBaseWndWindow();
//	//KWndWindowTemp() {};
//	//KWndWindowTemp(const KWndWindowTemp& ref);
//	//KWndWindowTemp& operator=(const KWndWindowTemp& ref);
//	~KWndWindow() { ReleaseWndWindow(this); }
//	void	Destroy() {}
//	void	UpdateData() {}
//	int		CloseWindow(bool bDestory) { return 0; }
//	int		Init(KIniFile* pIniFile, const char* pSection) { return 0; }
//	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return 0; }
//	int		PtInWindow(int x, int y) { return 0; }
//	void	PaintWindow() {}
//	void			PaintDebugInfo() {}
//	void	SetSize(int nWidth, int nHeight) {}
//	void			SetPosition(int nLeft, int nTop) {}
//	void			GetAllChildLayoutRect(RECT* pRect) {}
//	void			GetAbsolutePos(int* pLeft, int* pTop) {}
//	void			GetSize(int* pWidth, int* pHeight) {}
//	void			BringToTop() {}
//	void			GetPosition(int* pLeft, int* pTop) {}
//	void			SetCursorAbove() {}
//	void	Show() {}
//	void	Hide() {}
//	int				IsVisible() { return 0; }
//	void	Enable(int bEnable) {}
//	void			AddChild(KWndWindow* pChild) { }
//	void			AddBrother(KWndWindow* pBrother) { }
//
//	KWndWindow* TopChildFromPoint(int x, int y) { return NULL; }
//
//	KWndWindow* GetPreWnd() { return NULL; }	//得到前一个兄弟窗口
//	KWndWindow* GetNextWnd() { return NULL; }		//得到后一个兄弟窗口
//	KWndWindow* GetParent() { return NULL; }		//得到父窗口
//	KWndWindow* GetFirstChild() { return NULL; }	//得到第一个子窗口
//	KWndWindow* GetOwner() { return NULL; }							//获得不再有父窗口的祖先窗口
//
//	void			Paint() {}
//	void			LetMeBreathe() {}
//
//	void			SplitSmaleFamily() {}
//	void			LeaveAlone() {}
//	//#ifdef _DEBUG
//	void			SetCaption(char* pszCaption) {}
//	//#endif
//	int				IsDisable() { }
//	int				GetStyle() {}
//	int				SetStyle(unsigned int nStyle) {}
//	void			Clone(KWndWindow* pCopy) {}
//	void	Breathe() {}
//	void			OnLBtnDown(int x, int y) {}
//	void			OnMoveWnd() {}
//	void			AbsoluteMove(int dx, int dy) {}
//} ptKWndWindow;

//#pragma pack(pop)
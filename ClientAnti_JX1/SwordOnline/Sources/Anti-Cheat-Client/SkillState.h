#pragma once
//#include "stdafx.h"
typedef unsigned __int64 _QWORD;
typedef unsigned int _DWORD;
//#include <KWin32.h>
//#include "KCanvas.h"
//#include "KIniFile.h"
//#include <KIniFile.h>
//#include <KList.h>
//#include "KWin32App.h"
//#include <windows.h>
//#include "NewUiShell.h"
#include "NewUiText.h"
//#include <mmsystem.h>
//#include <dsound.h>
//#include <dinput.h>
//#include <ddraw.h>
#include <stdarg.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <map>
#include "NewUiImage.h"



//#pragma once
//#include "NewUiText.h"
//#include "Elem/WndWindow.h"
//#include "Elem/WndImage.h"
//#include "Elem/WndButton.h"
//#include "Ui/Elem/WndWindow.h"
//#include "Ui/Elem/WndImage.h"
//#include "Ui/Elem/WndButton.h"
//#pragma once

//class KIniFile;

//#include "ComWindow.h"
//#include <KList.h>
//#pragma pack(push, 1)
//#include "TextPic.h"
//#include "Wnds.h"
//#include "UiInit.h"
//#include "NewUiWindow.h"
//#include "NewUiImage.h"
//#include "NewUiText.h"
//#include "KImageStore2.h"
//#include "WndImage.h"
//#include "WndText.h"
//#include "WndWindow.h"
//#include "MouseHover.h"
//#include "Text.h"
//#include "crtdbg.h"
//#include "KImageStore2.h"
//#include <iRepresent/iRepresentShell.h>
//#include "Elem/WndButton.h"
//#include "Ui/Elem/WndImage.h"
//#include "wndbutton.h"
//#include "../Elem/WndWindow.h"
//#include "Ui/Elem/WndText.h"
//#include "Ui/Elem/Wnds.h"
//#include "WndMessage.h"
//#include "../Elem/MouseHover.h"
//#include "../core/src/gamedatadef.h"
//#include "../core/src/CoreShell.h"
//#include "KEngine.h"
//class KIniFile;
//typedef void (__thiscall KIniFile)(void);
//typedef KIniFile* (*KIniFile)();

//struct KWndImage{ int (**_vptr_WndImage)(...); };
//#ifndef STATESKILL_H
//#define STATESKILL_H

struct KNpcInfo
{
	char tmp[3720];
};

//#include "KEngine.h"
//struct CoreShell; // 0x659D40
//struct iRepresentShell; // 0x659D40
//typedef iCoreShell* (*fnCreateCoreShell)(const char* a1, const char* Source);
//typedef int (*_SetStateSkillEffect)(const void* This); // 0x57E07F
typedef int(__thiscall* _KListGetHead)(_DWORD* This); // 0x57E07F
typedef int(__thiscall* _DetectSynSkill)(const char* This, int a2); // 0x57E07F
typedef int(__thiscall* _SetStateSkillEffect)(_DWORD* This, int Index, int SkillID, int nLevel, int Data, int nDataNum, int nTime, int a7, int a8, int a9, int a10, int a11); // 0x57E07F
//extern DWORD* m_CoreShell;
//extern DWORD* m_RepresentShell;
//extern unsigned int* g_pAdjustColorTab;
//extern unsigned int g_ulAdjustColorCount;
//extern SIZE m_RepresentExactHalfSize;
#define SKILL_MAX_COUNT			200
#define STATE_SKILL_MAX_COUNT			15
//struct IServerCore
//{
//	int (**_vptr_IServerCore)(...);
//};
//g_pCoreServerShell = (KServerCore*)(LPVOID*)0x82B8830;
//KServerCore* g_pCoreServerShell;

struct SkillStateLive
{
	int		nIndex;
	int		nSkillID;
	int		nSkillLevel;
	int		nSkillTime;
	int		nFloatTime;
	int		m_LeftImage;
	int		m_TopImage;
	int		m_LeftText;
	int		m_TopText;
	int		m_TickTime;
	char	m_TextTime[10];
};

struct KUiSkillStateData
{
	int		nIndex;
	int		nSkillID;
	int		nSkillTime;
	char	m_Time[32];
	char	m_Name[128];
	char	m_Image[128];
	char	m_Desc[256];;
	int		m_IsAura;
	int		m_IsDebuff;
	int		m_IsState;
};

void __fastcall Main_RenderWindows();
void InsertSkillStateData(int SkillID, KUiSkillStateData SkillStateInfo);
void RunSkillState();
void CloseAllWindowOutGame();

int __stdcall NetConnectAgentInitialize(BYTE* This);
int __stdcall exec_sub_556590(int a1, int a2);
static DWORD WINAPI SyncSkillState(void*);
int WINAPI CleanSynSkill(_DWORD* This);
static DWORD WINAPI CheckSkillState(void*);
int WINAPI DetectSynSkill(int a2);
int __stdcall SyncStateSkill(int Index, int SkillID, int nLevel, int Data, int nDataNum, int nTime, int a7, int a8, int a9, int a10, int a11);

int InGame();
BOOL StartGame();
void SetRepresentShell(struct iRepresentShell* pRepresent);

//class KWndWindow
//{
//public:
//	KWndWindow() { CreateWndWindow(this); }
//	virtual ~KWndWindow() {}
//	//const int* vptr_WndImage[580];
//	//int			(**vptr_WndImage[580]);
//	//int				(**vptr_WndImage[580])(...);
////public:
//	//int				(**vptr_WndImage[440]);
////protected:
//	////----窗口参数----
//	//unsigned int m_Style;			//窗口风格,见Wnd.h中窗口风格的定义
//	//int			m_Left;				//左上角X坐标，相对于父窗口
//	//int			m_Top;				//左上角Y坐标，相对于父窗口
//	//int			m_Width;			//宽度
//	//int			m_Height;			//高度
//	//int			m_nAbsoluteLeft;	//窗口左上角的绝对坐标x
//	//int			m_nAbsoluteTop;		//窗口左上角的绝对坐标y
//	//int			m_bMoving;
//	//int			m_nLastMouseHoldPosX;
//	//int			m_nLastMouseHoldPosY;
//
//	////----与其他窗口的级连关系----
//	//KWndWindow* m_pPreviousWnd;		//前一个同级窗口
//	//KWndWindow* m_pNextWnd;			//后一个同级窗口
//	//KWndWindow* m_pFirstChild;		//第一个子窗口
//	//KWndWindow* m_pParentWnd;		//父窗口
//	//int			vptr_KWndWindow[440 - sizeof(KUiImageRef)];
//	//int			vptr_KWndWindow[440];
//};

//virtual

//class KWndImage : public KWndWindow
//{
//public:
//	KWndImage() { CreateWndImage(this); }
//	//struct vtable { int			vptr[440]; };
////protected:
////	int		(**vptr_KWndImage[440]);
//protected:
//	//vtable*			vptr_KWndImage;
//	int			vptr_KWndImage[440];
//	//KUiImageRef		m_Image;
//};

//class KWndText : public KWndWindow
//{
//public:
//	KWndText() {}
//	~KWndText() {}
//private:
//	//int				m_nFontSize;		//字体大小
//	//unsigned int	m_TextColor;		//文字颜色
//	//unsigned int	m_BorderColor;		//文字边缘颜色
//
//	//char* m_pText;			//文本缓冲区
//	//int				m_nBuffLen;			//文本缓冲区大小
//	//int				m_nTextLen;			//字符串的存储长度	
//	//int				m_nTopLine;			//显示的头一行的行号，从0开始
//	//int				m_nLineCount;		//文本总的行数目
//	int			vptr_KWndText[471];
//};
//
//class KWndText32 : public KWndText
//{
//private:
//	int			vptr_KWndText[32];
//public:
//	KWndText32() {}
//	~KWndText32() {}
//};

// 1000A160 1000A1A0






//struct KWndText : public KWndWindow
//{
//public:
//	KWndText() {}
//private:
//	int			vptr_KWndText[504];
//};

//struct KWndText32
//{
//public:
//	KWndText32() {}
//private:
//	int			vptr_KWndText[504];
//};

//struct KWndImage
//{
//	int			vtable_image[440];
//};
 
//struct KWndButton
//{
//	int			vtable_Button[592];
//};
 
//struct KWndLabeledButton
//{
//	int			vtable_LabeledButton[768];
//};

class KUiStateSkill : public KWndImage
{
public:
	static KUiStateSkill* OpenWindow();
	void	LoadScheme(const char* pScheme);
	static void __fastcall		UpdateData();
	static void		CloseWindow(bool bDestroy);
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return 0; }
	KUiStateSkill();
	//~KUiStateSkill();
private:
	void				Initialize();
	void				LoadScheme(KIniFile* pIni);
	int			WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	 //void	OnClickButton(KWndWindow* pBtn);
	int		PtInWindow(int, int) { return 0; }
	void		PaintWindow();
	//void		Destroy();
	void		Hide();
	void		Show();
	void		SetSize(int nWidth, int nHeight) {}
	void		Enable(int bEnable) {}
	virtual void	Breathe() {}
private:
	static KUiStateSkill* m_pSelf;
private:
	KWndImage				m_ImageSkill[STATE_SKILL_MAX_COUNT];
	KWndText32				m_Text32Skill[STATE_SKILL_MAX_COUNT];
	int				m_IsShow;
	int				m_nNumSkills;
	int				m_nLiveSkills;
	//KWndAutoPlay			m_AutoPlay;
	//KWndWindow			m_WndWindowAutoPlay;
	//Player_AutoPlay			m_ButtonAutoPlay;
};

extern char c_AccountName[32];
extern char c_ClientSerial[33];
extern char c_HardwareId[32];
//extern KUiStateSkill	g_UiStateSkill;

//extern HANDLE ThreadCheckState1;

//#endif

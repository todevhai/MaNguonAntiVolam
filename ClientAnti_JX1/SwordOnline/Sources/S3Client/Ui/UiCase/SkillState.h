#pragma once
//#include "stdafx.h"
//#include "KWin32.h"
//#include "KIniFile.h"
#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <dinput.h>
#include <ddraw.h>
#include <stdarg.h>
//#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include "Text.h"
//#include "crtdbg.h"
//#include "../Elem/WndImage.h"
//#include "wndbutton.h"
//#include "../Elem/WndWindow.h"
//#include "../Elem/WndText.h"
//#include "../Elem/Wnds.h"
//#include "WndMessage.h"
//#include "../Elem/MouseHover.h"
//#include "../core/src/gamedatadef.h"
//#include "../Represent/iRepresent/KRepresentUnit.h"
//#include "../Represent/iRepresent/iRepresentShell.h"
typedef unsigned __int64 QWORD;
typedef unsigned int _DWORD;
//struct iCoreShell; // 0x659D40
//struct iRepresentShell; // 0x659D40

//extern iCoreShell* g_pCoreShell;
//extern iRepresentShell* g_pRepresentShell;

#define SKILL_MAX_COUNT			200
#define STATE_SKILL_MAX_COUNT			10

//typedef class KIniFile;
//typedef class KWndWindow;
//typedef class KWndImage;
//typedef class KWndText32;


//struct KWndWindow
//{
//	BYTE tmp[27637];
//};
//
//
//struct KWndImage
//{
//	BYTE tmp[27637];
//};

struct KUiSkillState
{
	BYTE tmp[27637];
};

class KIniFile; // 0x659D40

//extern KWndImage* KWndImage;
//extern KWndText* KWndText;
//extern iCoreShell* g_pCoreShell;
//extern iRepresentShell* g_pRepresentShell;

//typedef void (*KWndText32)(void); // 0x63D8C4
//typedef void (*KWndText80)(void); // 0x63D904
//typedef void (*KWndImage)(void); // 0x63CF4C
//typedef void (*KWndWindow)(void); // 0x63DB6C

//typedef void (*_KIniFile)(char* This);
//typedef bool (*_KIniFile_Load)(char* This, const char* Data);
//typedef int (*_KIniFile_GetInteger)(char* This, const char* a1, const char* a2, int a3, int* a4);
//typedef int (*_KIniFile_GetString)(char* This, const char* a1, const char* a2, const char* a3, char* a4, int a5);
//typedef void (*_KIniFile_Clean)(char* This);

//extern _KIniFile Ex_KIniFile_Entry;
//extern _KIniFile_Load Ex_KIniFile_Load;
//extern _KIniFile_GetInteger Ex_KIniFile_GetInteger;
//extern _KIniFile_GetString Ex_KIniFile_GetString;
//extern _KIniFile_Clean Ex_KIniFile_Clean;

//extern DWORD KIniFile_Entry;
//extern DWORD KIniFile_Load;
//extern DWORD KIniFile_GetInteger;
//extern DWORD KIniFile_GetString;
//extern DWORD KIniFile_Clean;

int UiSkillStateInitialize();
//KUiSkillState* UiSkillState();
int UiSkillState();
void ByPass();
DWORD* CreateSkillState(DWORD* Data);
DWORD* SkillState_sub_4039A0(KUiSkillState* m_KUiSkillState);
void SkillState_sub_403A70(const char* a1);
int SkillState_sub_404AB0(DWORD* Data, int a2, DWORD* a3);
DWORD* SkillState_sub_404FA0(DWORD* Data, DWORD* a2, char a3, DWORD* a4, int a5);
void* SkillState_sub_405FD0(void* Data, int a2, int a3, int a4, const void* a5, char a6);
int SkillState_sub_405D20(DWORD* Data, int a2);
int SkillState_sub_405DD0(DWORD* Data, DWORD* a2);
BYTE* SkillState_sub_445720(BYTE* m_KUiSkillState);
void SkillState_sub_42FC60(DWORD* m_KUiSkillState);
int SkillState_sub_404670(void* a1, const char* a2, int a3);
void* SkillState_sub_404F30(void* This);
void* SkillState_sub_405E80(void* This);
void* __stdcall SkillState_sub_406110(int a1);
void* __cdecl SkillState_sub_406230(unsigned int a1);
int SkillState_sub_4038B0(DWORD* This);
int __stdcall sub_62DA72(int a1, int a2, int a3, int(__thiscall* a4)(int));

void ALW_GetWndPosition(int& nX, int& nY, int nWidth, int nHeight, bool bUseInputParamPos = false, bool bIgnoreCursorPos = false);

struct KOutputTextParam;
//输出包含tab分格的字符串，可以是多行，返回值表示输出内容共几行
//nLineWidth 的单位是屏幕像素点
//KOutputTextParam::nSkipLine不支持
int OutputTabSplitText(const char* pBuffer, int nCount, int nLineWidth,
	int nFontId, KOutputTextParam* pParam);
//eType:文件类型（bmp,jpg）
//nQuality:图象质量(0-100)，只对jpg有意义
//void SaveScreenToFile(ScreenFileType eType = SCRFILETYPE_JPG, unsigned int nQuality = 100);
bool SetScrPicPath(const char* szPath);

struct KSkillObject
{
	unsigned int uGenre;	//对象类属
	unsigned int uId;		//对象id
//	int			 nData;		//与对象实例相关的某数据
};

struct KUiSkillStateData
{
	int		nIndex;
	int		nID;
	int		nTime;
	char	m_Time[32];
	char	m_Name[32];
	char	m_Image[128];
	char	m_Desc[256];;
	int				m_Left;
	int				m_Top;
	int				m_Width;
	int				m_Height;
};

struct KSkillListInfo : public KSkillObject
{
	int		nIndex;
	int		nID;
	char	m_Name[63];
	char	m_Image[128];
	char	m_Desc[80];
};

struct KNpcInfo
{
	char tmp[3720];
};

//typedef int (*_KNpcInfo)(KNpcInfo*, int);

//_DWORD* m_NpcIndex;
//struct KUiDraggedObject;
//struct KPopupMenuData;
//class KIniFile;
static DWORD WINAPI CheckSkillState(void*);
int __stdcall SyncStateSkill(int This, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11);
int __stdcall GetPlayerIndex(KNpcInfo* , int);

class CSkillState
{
public:
	CSkillState();
	virtual ~CSkillState();
	void Initialize();
	void LoadScheme(const char* pScheme);
	//void LoadScheme(KIniFile* pIni);
	int	LoadSkillItems(KIniFile* pIniFile, const char* pSection);
	static CSkillState* OpenWindow();
	static CSkillState* GetIfVisible();
	static void		 CloseWindow(bool bDestroy);
	int WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	int	GetSkillAtPos(int x, int y);
	void	PaintWindow();
public:
	static CSkillState* m_pSelf;
public:
	int				m_nOpen;
	int				m_nFont;
	unsigned int	m_uColor;
	unsigned int	m_bLeft;
	int				m_nLeft, m_nTop;
	int				m_nRightLeft, m_nRightTop;
	int				m_nWidth, m_nHeight;
	int				m_nNumSkills;
	int				m_nSkillsCount;
	//KWndWindow m_WndBuff; // 0x65AF94
	//KWndText32 m_txtBuffName[STATE_SKILL_MAX_COUNT]; // 0x65ACB4
	//KWndText32 m_txtBuffTime[STATE_SKILL_MAX_COUNT]; // 0x65ACB4
	//KWndImage m_BuffImage[STATE_SKILL_MAX_COUNT]; // 0x65A2DC
	//KWndText80 m_BuffDesc[STATE_SKILL_MAX_COUNT]; // 0x65A2DC
	static KUiSkillStateData	ms_ListSkillsState[STATE_SKILL_MAX_COUNT];
	static KSkillListInfo	ms_ListInfo[SKILL_MAX_COUNT];
	KUiSkillStateData				m_StateSkillList;
};
//
extern CSkillState gSkillState;

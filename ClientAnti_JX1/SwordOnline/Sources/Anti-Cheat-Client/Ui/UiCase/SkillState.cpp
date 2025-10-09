#include "KWin32.h"
#include "KCore.h"
#include "KPakList.h"
#include "SkillState.h"
#include "KIniFile.h"
#include <fstream>

////#include "../Represent/iRepresent/iRepresentShell.h"
//#include "StdAfx.h"
//#include "SkillState.h"
//#include <fstream>
//#include "Util.h"
//#include "Log.h"
////#include <Elem/WndObjContainer.h>
////#include <Elem/WndShowAnimate.h>
////#include <Kime.h>
////#include <Elem/WndShowAnimate.h>
////#include <Elem/WndObjContainer.h>
////#include "Wnds.h"
////#include "WndMessage.h"
////#include "MouseHover.h"
//
////#include <Elem/TextPic.h>
////#include "../core/src/coreshell.h"
////typedef class KIniFile;
////typedef void KIniFile ((void(KIniFile*)(KIniFile* a1)));
////extern KIniFile* gIniFile;
////iCoreShell* g_pCoreShell;
////iRepresentShell* g_pRepresentShell;
////KUiBase*	g_UiBase;
//
//#define	UI_INFO_FILE_PATH			"\\Ui"				//界面信息文件存放的根目录
//#define	UI_COMMON_CONFIG_FILE		"\\Ui\\Setting.ini"
//#define	UI_DEF_CONFIG_FILE			"\\Ui\\DefConfig.Ini"//界面配置纪录文件名
//#define	UI_PUBLIC_SETTING			"PublicSetting.ini"			//某个界面方案的设定文件
//
//#define	UI_USER_DATA_FOLDER			"\\UserData"		//玩家数据的存盘目录位置
//#define	UI_USER_DATA_TEMP_FOLDER	"\\UserData\\Temp"	//玩家数据的临时存盘目录位置
//#define	UI_COMMON_SETTING_FILE		"\\UserData\\UiCommon.ini"//界面公共设置的文件的名称
//#define UI_PRIVATE_SETTING_FILE		"UiConfig.ini"		//界面个人数据的存储文件
//
//#define	THEME_SECTION			"Theme"
//#define	FONT_SECTION			"FontList"
//int WND_SHOW_MOUSE_OVER_WND = false;
//
//const char* $Main = "Main";
//
//
//
#define SCHEME_INI						"SkillState.ini"
//
//#define SKILLTREE_SHORTCUT_SKILL_SAVE	"DataSkillState"
//
//int WND_SHOW_DEBUG_FRAME_TEXT = false;
//
//#define	MOSE_HOVER_TIME_VALVE		1000	
//
//unsigned int	l_Time = 0;
//
//#define	CURSOR_HEIGHT		32
//
//#define	SCREEN_PIC_SAVE_PATH	"\\UserData\\PrintScreen\\"
//static char szScrPicPath[MAX_PATH] = "";
//
using namespace std;
//
////CSkillState gSkillState;
////int m_pSelf[0x6BF5];
////int _DWORD_68CD7C[0x6BF5];
////int m_pSelf = (int)0x68CD7C;
////int m_pSelf;
////BYTE m_KUiSkillState[0x6BF5];
////int m_KUiSkillState = (int)(void*)0x6AE930;
////char m_KUiSkillState[27637];
//
//CSkillState gSkillState;
//KUiSkillStateData	CSkillState::ms_ListSkillsState[STATE_SKILL_MAX_COUNT];
//KSkillListInfo	CSkillState::ms_ListInfo[SKILL_MAX_COUNT];
////KWndWindow			m_WndBuffList;
////KWndText32			m_txtBuffTime;
////KWndImage		m_BuffImage;
////KWndList		m_RawImage;
////KWndImage* m_BuffImage = (KWndImage*)0x80BFA80;
////KWndText32* m_txtBuffTime = (KWndText32*)0x63D8C4;
//
KUiSkillState* m_DataState;
////KUiSkillData* m_SkillList[10];
//
//char byte_1414141[3];
//char byte_68D090[484];
//HMODULE hModuleEngine;
//bool m_IsActiveSkillState = false;
//
//DWORD KIniFile_Entry;
//DWORD KIniFile_Load;
//DWORD KIniFile_GetInteger;
//DWORD KIniFile_GetString;
//DWORD KIniFile_Clean;
//
//_KIniFile Ex_KIniFile_Entry;
//_KIniFile_Load Ex_KIniFile_Load;
//_KIniFile_GetInteger Ex_KIniFile_GetInteger;
//_KIniFile_GetString Ex_KIniFile_GetString;
//_KIniFile_Clean Ex_KIniFile_Clean;
//
//#define b_Optimize		((void(__cdecl*)()) 0x639110)//0x528F40 
//#define b_sub_420B90		((int(*)()) 0x420B90)//0x528F40 
//#define b_sub_491290		((int(*)()) 0x491290)//0x528F40 
#define b_sub_4A50F0		((int(*)()) 0x4A50F0)//0x528F40 
////#define b_sub_441B80		((_DWORD*(__thiscall*)(_DWORD* a1)) 0x441B80)//0x528F40 
//
////#define b_sub_42C0E0		((int(__thiscall*)(int, char*, char*)) 0x42C0E0)//0x528F40 
//#define b_sub_42C010		((void(__thiscall*)(void* a1)) 0x42C010)//0x528F40 
//#define b_sub_43DCE0		((int*(__thiscall*)(void* a1)) 0x43DCE0)//0x528F40 
//#define b_sub_44ADC0		((int*(__thiscall*)(_DWORD* a1)) 0x44ADC0)//0x528F40 
//#define b_AddChild		((_DWORD*(__thiscall*)(_DWORD *, _DWORD *)) 0x442690)//0x528F40 
//#define b_sub_442730		((_DWORD*(__thiscall*)(_DWORD *, _DWORD *)) 0x442730)//0x528F40 
//#define b_sub_41A4A0		((int(__thiscall*)(const char *This, char* Data, int a3)) 0x41A4A0)//0x528F40 
//#define b_sub_442690_1		((_DWORD*(__thiscall*)(_DWORD *)) 0x442690)//0x528F40 
//#define b_SetPosition		((_DWORD*(__thiscall*)(_DWORD *, int , int )) 0x441CA0)//0x528F40 
//#define b_sub_43AD60		((_DWORD*(__cdecl*)(int a1, int a2)) 0x43AD60)//0x528F40 
//#define b_sub_441BC0		((_DWORD*(__thiscall*)(_DWORD *)) 0x441BC0)//0x528F40 
//#define b_sub_4255E0		((int(__cdecl*)(int )) 0x4255E0)//0x528F40 
//#define b_sub_40B060		((_DWORD*(__thiscall*)(_DWORD *)) 0x40B060)//0x528F40 
#define b_Get_NPC_nIndex		((int(__thiscall*)(KNpcInfo *This, int a2)) 0x580850)//0x528F40 

//#define b_Get_NPC_nIndex		((int(__thiscall*)(char *This, int a2)) 0x63A66C)//0x528F40 
#define b_SyncStateSkill		((int(__thiscall*)(_DWORD* This,int a1,int a2,int a3,int a4,int a5,int a6,int a7,int a8,int a9,int a10,int a11)) 0x57B080)//0x528F40 
//#define b_GetIndexPlayer		((int(__thiscall*)(char *This,int a2)) 0x580850)//0x528F40 

KNpcInfo* m_NpcInfo;
HANDLE ThreadHandlesCheckSkill;
//KList				m_StateSkillList;

//#define b_KIniFile		((void*(__thiscall*)(void *This)) 0x678EE2)// 0x63A8F8 KIniFile_Entry
//#define b_KLoad		((bool(__thiscall*)(void *This, const char *Data)) 0x678EC6)//0x63A8E4 KIniFile_Load
//#define b_GetInteger		((int(__thiscall*)(void *This, const char* a1, const char* a2, int a3, int * a4)) 0x678EF8)//0x63A8F4 KIniFile_GetInteger
//#define b_GetString		((int(__thiscall*)(void *This, const char* a1, const char* a2, const char* a3, const char* a4, int a5)) 0x678E9E)//0x63A8E0 KIniFile_GetString
//#define b_CleanIniFile		((void(__thiscall*)(void *This)) 0x678E88)//0x63A8DC 0x5E1A70 KIniFile_Clean
//#define b_g_HashString2Id		((unsigned long(__cdecl*)(char const *)) 0x63A8EC)//0x63A8DC 
//
//#define b_sub_612CD0		((void(__cdecl*)(void*)) 0x612CD0)//0x528F40 
//#define b_sub_404030		((int(__fastcall*)(int)) 0x404030)//0x528F40 
//#define b_sub_408C40		((_DWORD*(*)()) 0x408C40)//0x528F40 
//#define b_sub_420AB0		((void(*)()) 0x420AB0)//0x528F40 
//#define b_sub_63A3E4		((void(__cdecl*)()) 0x63A3E4)//0x528F40 
//#define b_sub_402370		((_DWORD*(__thiscall*)(_DWORD *, int )) 0x402370)//0x528F40 
//#define b_invalid_parameter_noinfo		((void(__cdecl*)()) 0x6573E8)//0x528F40 
//#define b_sub_42C010_void		((void*(__thiscall*)(void* a1)) 0x42C010)//0x528F40 
//#define b_sub_612F01		((int*(__stdcall*)(int a1, int a2, int a3, void (__thiscall *a4)(int), int a5)) 0x612F01)//0x528F40 
//#define b_sub_612E9E		((int*(__stdcall*)(int a1, int a2, int a3, int (__thiscall *a4)(int))) 0x612E9E)//0x528F40 
//#define b_sub_40ADA0		((_DWORD*(__thiscall*)(_DWORD* This)) 0x40ADA0)//0x528F40 
//#define b_sub_441B80		((int(__thiscall*)(_DWORD* This)) 0x441B80)//0x528F40 
//#define b_sub_441F00		((int(__thiscall*)(_DWORD* This)) 0x441F00)//0x528F40 
//#define b_sub_42C0E0		((int(__thiscall*)(int This, KIniFile* a2, char *a3)) 0x42C0E0)//0x528F40 
//#define b_sub_4428D0		((int(__thiscall*)(_DWORD *this, unsigned int a2, int a3, int a4)) 0x4428D0)//0x528F40 
//#define b_sub_43D5C0		((int(__thiscall*)(int This, KIniFile* a2, char *a3)) 0x43D5C0)//0x528F40 
//#define b_sub_442060		((int(__thiscall*)(int This, KIniFile* a2, char *a3)) 0x442060)//0x528F40 
//
CSkillState* CSkillState::m_pSelf = NULL;

CSkillState::CSkillState()
{
	//m_pSelf = NULL;
	m_nNumSkills = 0;

	/*for (int i = 0; i < STATE_SKILL_MAX_COUNT; i++)
	{
		 memset(&m_pSelf->ms_ListSkillsState[i].BuffImage, 0, 128);
		 memset(&m_pSelf->ms_ListSkillsState[i].BuffTime, 0, 128);
		 memset(&m_pSelf->ms_ListSkillsState[i].m_Time, 0, 32); 
	}*/

	m_nOpen = 0;

	m_nSkillsCount = 0;
	//m_DataState = NULL;
	//g_UiBase.SetUiBase((_DWORD*)0x68D090);
}
 
//CSkillState* CSkillState::GetIfVisible()
//{
//	//if (m_pSelf && m_pSelf->IsVisible())
//		//return m_pSelf;
//	return NULL;
//}
//
//CSkillState* CSkillState::OpenWindow()
//{
//	//if (m_DataState == NULL)
//	//{
//	//	m_pSelf = new CSkillState;
//	//	if (m_pSelf)
//	//		Initialize();
//	//}
//	//if (m_pSelf)
//	//{
//	//	//m_pSelf->UpdateData();
//	//	//m_pSelf->Show();
//	//	//m_pSelf->m_Market.SetMaxFrame(pImage->MaxFrame); 
//	//}
//	_DWORD* v2 = (_DWORD*)operator new(27636);
//	_DWORD* v1 = CreateSkillState(v2);
//	m_DataState = (KUiSkillState*)v1;
//	//KWndImage::KWndImage();
//	return (CSkillState*)m_DataState;
//}
//
//void CSkillState::CloseWindow(bool bDestroy)
//{
//	/*if (m_pSelf)
//	{
//		if (bDestroy == false)
//			m_pSelf->Hide();
//		else
//		{
//			m_pSelf->Destroy();
//			m_pSelf = NULL;
//		}
//	}*/
//}
//
//void CSkillState::PaintWindow()
//{
//	//KWndImage::PaintWindow();
//}

void ByPass()
{
	int IsOpenl;
	int IsOpen2;
	char txtImage[512];
	g_SetRootPath(NULL);
	g_SetFilePath("\\");
	//INIReader reader("\\Ui\\ui3\\SkillState.ini");
	//IsOpenl = reader.GetInteger("Main", "IsOpen", 0);
	//memcpy(txtImage, &reader.GetString("Main", "Image", 0), 128);
	//txtImage = reader.GetString("Main", "Image", 0);
		//_KIniFile* Ini = new _KIniFile;
		////ofstream myfile2;
		//myfile2.open("E:\\example1.txt");
		//myfile2 << (void*)Ini << "\n";
		//myfile2.close();
		//_KIniFile(Ini);
		//char		Buff[128];
		KIniFile	Ini;
		//sprintf(Buff, "\\Ui\\ui3\\SkillState.ini", SCHEME_INI_MINI);
		Ini.Load("SkillState.ini");
		Ini.GetInteger("Main", "IsOpen", 0, &IsOpenl);
		Ini.GetString("Main", "Image", (char*)0x63ABB8, txtImage, 128);
		//b_KIniFile(&IniFile);
		////KIniFile_Load(Ini, "\\Ui\\ui3\\技能状态列表.ini");

		//ofstream myfile1;
		//myfile1.open("E:\\example1.txt");
		//myfile1 << (_DWORD)&IniFile << "\n";
		//myfile1.close();

		//b_KLoad(&IniFile, "\\Ui\\ui3\\技能状态列表.ini");
		////b_GetInteger(Ini, "Main", "IsOpen", 0, &IsOpenl);

	//INIReader reader2("SkillState.ini");
	//IsOpen2 = reader2.GetInteger("Main", "IsOpen", 0);

	ofstream myfile2;
	myfile2.open("E:\\example2.txt");
	//myfile2 << Buff << "\n";
	myfile2 << IsOpenl << "\n";
	myfile2 << txtImage << "\n";
	myfile2.close();
	//CSkillState::m_pSelf = new CSkillState;
	//CSkillState::m_pSelf->Initialize();

	//ofstream myfile1;
	//char Buffer[65];
	//char		Buff[32];
	//KIniFile	pIni;
	//int v4;
	////CSkillState::m_pSelf->AddChild(&m_RawImage);
	////CSkillState::m_pSelf->AddChild(&m_BuffImage);
	////CSkillState::m_pSelf->AddChild(&m_txtBuffTime);
	////ms_ListSkillsState

	//myfile1.open("E:\\example4.txt");
	//myfile1 << (void*)&pIni << "\n";
	////myfile << (const char*)(void*)0x68D090 << "\n";
	////myfile << (_DWORD*)&g_UiBase << "\n";
	////myfile << (void*)g_pRepresentShell << "\n";
	////myfile << (void*)g_pCoreShell << "\n";
	//myfile1 << pIni.Load("\\Ui\\ui3\\SkillState.ini") << "\n";
	//myfile1.close();

	//ofstream myfile;
	//myfile.open("E:\\example5.txt");
	//if (pIni.Load("\\Ui\\ui3\\SkillState.ini"))
	//{
	//	myfile << "Load OK" << "\n";
	//	
	//	//m_RawImage.Init(&Ini, "Main");

	//	for (int i = 0; i < 10; ++i)
	//	{

	//		
	//		myfile << i << "\n";
	//		//myfile << (const char*)(void*)0x68D090 << "\n";
	//		//myfile << (_DWORD*)&g_UiBase << "\n";
	//		//myfile << (void*)g_pRepresentShell << "\n";
	//		//myfile << (void*)g_pCoreShell << "\n";
	//		//myfile << Buffer << "\n";
	//		//myfile.close();

	//		//m_BuffImage.Init(&Ini, "BuffImage");
	//		//m_txtBuffTime.Init(&Ini, "txtBuffTime");

	//		//m_BuffImage.SetPosition(24 * i, 0);
	//		//m_BuffImage.AddChild(&m_txtBuffTime);

	//		sprintf(Buffer, "Buff_%d_Name", i);
	//		pIni.GetString("BuffList", Buffer, "N/A", CSkillState::m_pSelf->ms_ListSkillsState[i].m_Name, 63);
	//		myfile << CSkillState::m_pSelf->ms_ListSkillsState[i].m_Name << "\n";
	//		sprintf(Buffer, "Buff_%d_Image", i);
	//		pIni.GetString("BuffList", Buffer, "N/A", CSkillState::m_pSelf->ms_ListSkillsState[i].m_Image, 128);
	//		myfile << CSkillState::m_pSelf->ms_ListSkillsState[i].m_Image << "\n";
	//		sprintf(Buffer, "Buff_%d_Desc", i);
	//		pIni.GetString("BuffList", Buffer, "N/A", CSkillState::m_pSelf->ms_ListSkillsState[i].m_Desc, 256);
	//		myfile << CSkillState::m_pSelf->ms_ListSkillsState[i].m_Desc << "\n";
	//		strcpy(CSkillState::m_pSelf->ms_ListSkillsState[i].m_Time, "N/A");
	//		myfile << CSkillState::m_pSelf->ms_ListSkillsState[i].m_Time << "\n";
	//		//m_BuffImage.SetImage(ISI_T_BITMAP16, CSkillState::m_pSelf->ms_ListSkillsState[i].m_Image);
	//		//m_txtBuffTime.SetText(CSkillState::m_pSelf->ms_ListSkillsState[i].m_Time, strlen(CSkillState::m_pSelf->ms_ListSkillsState[i].m_Time));
	//		//m_BuffImage.Show();
	//	}

	//	//gSkillState.m_txtBuffTime.Show();
	//	//gSkillState.m_RawImage.PaintWindow();
	//	//Ini.GetInteger("Main", "IsOpen", 0, &v4);

	//}
	//myfile.close();
}


int __stdcall SyncStateSkill(int a1, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9, int a10, int a11)
{
	
	if (a4 <= 0 || a3 <= 0)
		return -1;

	if (a6 >= 20)
		a6 = 20;
	
	//char DataSkill[100];
	//int result;

	ofstream myfile2;
	myfile2.open("E:\\example.txt");
	myfile2 << a1 << "\n"; // skill id
	myfile2 << a2 << "\n"; // skill id
	myfile2 << a3 << "\n"; // skill level
	myfile2 << a4 << "\n"; // data
	myfile2 << a6 << "\n"; // skill time
	myfile2 << a7 << "\n";
	myfile2 << a11 << "\n";
	myfile2.close();

	return b_SyncStateSkill((_DWORD*)(0x12C5540 + 70584 * a1),a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11);
}

int __stdcall GetPlayerIndex(KNpcInfo* m_NpcInfo, int a2)
{
	//int result;
	//result = b_GetIndexPlayer(This, a2);

	//m_NpcInfo = (KNpcInfo*)This;

	return b_Get_NPC_nIndex(m_NpcInfo, a2);
}

DWORD WINAPI CheckSkillState(void*)
{
	int nCount;
	int result;
	ofstream myfile2;
	nCount = 0;
	while (true)
	{

		result = b_Get_NPC_nIndex(m_NpcInfo, *(_DWORD*)(0x6E2418 + 3));

		ofstream myfile2;
		myfile2.open("E:\\example3.txt");
		myfile2 << nCount << "\n";
		myfile2 << (_DWORD)m_NpcInfo << "\n";
		myfile2 << *(_DWORD*)(0x6E2418 + 3) << "\n";
		myfile2.close();
		
		Sleep(1000);

		nCount++;
	}
	
}

int UiSkillStateInitialize()
{
	int result;

	g_SetRootPath(NULL);
	g_SetFilePath("\\");

	//m_NpcInfo = (KNpcInfo*)(void*)0x12C5598;

	//ThreadHandlesCheckSkill = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckSkillState, 0, 0, NULL);
	
	//SetThreadPriority(ThreadHandlesCheckSkill, THREAD_PRIORITY_HIGHEST);

	//WaitForSingleObject(ThreadHandlesCheckSkill, 1000);

	//UiSkillState();
	//return Number;
	//return b_sub_4A50F0();
	return b_sub_4A50F0();

}

//int UiSkillState()
//{
//	_DWORD* v1;
//	_DWORD* v2;
//
//	//ofstream myfile2;
//	//myfile2.open("E:\\example1.txt");
//	//myfile2 << m_pSelf << "\n";
//	//myfile2.close();
//
//	if (!m_IsActiveSkillState)
//	{
//		//m_pSelf = new KUiSkillState;
//		//0x12C80D0
//		//VirtualProtect((void*)m_KUiSkillState, 0x6BF4u, PAGE_EXECUTE_READWRITE, &OldProtect);
//		//MemorySet((_DWORD)m_pSelf, 0, 27636);
//
//		v2 = (_DWORD*)operator new(27636);
//		//BYTE szBuff[0x6BF4];
//		v1 = CreateSkillState(v2);
//		//m_DataState = (KUiSkillState*)v1;
//		//m_IsActiveSkillState = true;
//
//		//memcpy(&m_pSelf, v1, 27636);
//		//VirtualProtect(&m_KUiSkillState, 27636, PAGE_EXECUTE_READWRITE, &OldProtect);
//		//VirtualProtect(&m_pSelf, sizeof(m_pSelf), PAGE_EXECUTE_READWRITE, &OldProtect);
//		//VirtualProtect((void*)0x12C80D0, sizeof(0x12C80D0), PAGE_EXECUTE_READWRITE, &OldProtect);
//		//*(this->m_KUiSkillState) = (char)&m_KUiSkillState;
//
//	/*	ofstream myfile;
//		myfile.open("E:\\example2.txt");
//		myfile << v1 << "\n";
//		myfile << m_pSelf << "\n";
//		myfile.close();*/
//
//		if (m_IsActiveSkillState)
//		{
//			SkillState_sub_4039A0(m_DataState);
//			
//		}
//
//	}
//
//	
//
//	//if (m_pSelf)
//	//{
//	//	b_sub_441BC0((_DWORD*)m_pSelf);
//	//	ofstream myfile4;
//	//	myfile4.open("E:\\example4.txt");
//	//	myfile4 << m_pSelf << "\n";
//	//	//myfile << IniFile << "\n";
//	//	//myfile << Buffer << "\n";
//	//	myfile4.close();
//	//	(*(void(**)(void))((_DWORD*)m_pSelf + 32))();
//	//	ofstream myfile5;
//	//	myfile5.open("E:\\example5.txt");
//	//	myfile5 << m_pSelf << "\n";
//	//	//myfile << IniFile << "\n";
//	//	//myfile << Buffer << "\n";
//	//	myfile5.close();
//	//	//b_sub_40B060((_DWORD*)m_pSelf);
//	//	return (int)m_pSelf;
//	//}
//		/*ofstream myfile4;
//		myfile4.open("E:\\example4.txt");
//		myfile4 << m_pSelf << "\n";
//		myfile4.close();*/
//		if (m_IsActiveSkillState)
//		{
//			b_sub_441BC0((_DWORD*)&m_DataState);
//			//(*(void(**)(void))((_DWORD*)&m_pSelf[32]))();
//			//b_sub_404030((_DWORD*)&m_pSelf[32]);
//			//SkillState_sub_4038B0((_DWORD*)&m_pSelf[32]);
//			b_sub_441F00((_DWORD*)m_DataState + 32);
//			//b_sub_408C40();
//			return (int)m_DataState;
//		}
//
//	return NULL;
//}

////int __thiscall sub_4038B0(_DWORD* This)
////{
////	*this = &KUiSkillState::`vftable';
////	b_sub_612E9E((int)(this + 5299), 644, 10, (int(__thiscall*)(int))sub_4156C0);
////	b_sub_612E9E((int)(this + 3849), 580, 10, (int(__thiscall*)(int))sub_4156C0);
////	b_sub_612E9E((int)(this + 2239), 644, 10, (int(__thiscall*)(int))sub_4156C0);
////	b_sub_612E9E((int)(this + 789), 580, 10, (int(__thiscall*)(int))sub_4156C0);
////	sub_404CF0(this + 146);
////	return sub_445870(this);
////}
//
////int CSkillState::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
////{
////	return NULL;
////}

//CSkillState* CSkillState::m_pSelf = NULL;


CSkillState* CSkillState::OpenWindow()
{

	if (m_pSelf == NULL)
	{
		m_pSelf = new CSkillState;
		if (m_pSelf)
			m_pSelf->Initialize();
	}

	//if (m_pSelf == NULL)
	//{
	//	m_pSelf = new KUiPlayerBar;
	//	if (m_pSelf)
	//		m_pSelf->Initialize();
	//	//m_pSelf->m_Market.SetMaxFrame(pImage->MaxFrame); 
	//}
	//if (m_pSelf)
	//{
	//	m_pSelf->UpdateData();
	//	m_pSelf->Show();
	//	//m_pSelf->m_Market.SetMaxFrame(pImage->MaxFrame); 
	//}

	//m_pSelf->m_nCurChannel = -1;
	//m_pSelf->m_nRecentPlayerName = 0;

	return m_pSelf;
}

void CSkillState::Initialize()
{

	//KWndImage::PaintWindow();

	//char Scheme[256];
	//g_UiBase.GetCurSchemePath(Scheme, 256);
	//m_pSelf->LoadScheme(Scheme);
	//Wnd_AddWindow(this, WL_NORMAL);

}

//int CSkillState::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
//{
//	/*int x, y, nSkill;
//	int nRet = 0;
//	switch (uMsg)
//	{
//	case WM_MOUSEHOVER:
//		x = LOWORD(nParam);
//		y = HIWORD(nParam);
//		nSkill = GetSkillAtPos(x, y);
//		if (nSkill >= 0)
//		{
//			if (g_MouseOver.IsMoseHoverWndObj(this, nSkill) == 0)
//			{
//				SetMouseHoverObjectDesc(this, nSkill, ms_ListSkillsState[nSkill].uGenre,
//					ms_ListSkillsState[nSkill].uId, UOC_SKILL_LIST, x, y);
//			}
//		}
//		else
//			g_MouseOver.CancelMouseHoverInfo();
//		break;
//	default:
//		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
//	}
//	return nRet;*/
//	return 0;
//}
//
//int	CSkillState::GetSkillAtPos(int x, int y)
//{
//	//int nMaxColu = m_Width / m_nWidthPerSkill;
//	//int nRow = (y - m_nAbsoluteTop) / m_nHeightPerSkill;
//	//int nColu, nIndex;
//
//	////	if (m_bLeft)
//	////		nColu = (x - m_nAbsoluteLeft) / m_nWidthPerSkill;
//	////	else
//	//nColu = nMaxColu - 1 - (x - m_nAbsoluteLeft) / m_nWidthPerSkill;
//
//	//nIndex = 0;
//	//while (nIndex < m_nNumSkills && ms_ListSkillsState[nIndex].nID < nRow)
//	//	nIndex++;
//
//	//int	nNumColu = 0;
//	//while (nIndex < m_nNumSkills && ms_ListSkillsState[nIndex].nID == nRow)
//	//{
//	//	if (nNumColu == nColu)
//	//		return nIndex;
//	//	nIndex++;
//	//	nNumColu++;
//	//}
//
//	return -1;
//}
//
//_DWORD* CreateSkillState(_DWORD* szBuff)
//{
//	char v3; // [esp+11h] [ebp-2Fh] BYREF
//	char v4; // [esp+12h] [ebp-2Eh] BYREF
//	char v5; // [esp+13h] [ebp-2Dh] BYREF
//	char* v6; // [esp+14h] [ebp-2Ch]
//	char* v7; // [esp+1Ch] [ebp-24h]
//	char* v8; // [esp+24h] [ebp-1Ch]
//	int i; // [esp+30h] [ebp-10h]
//	int v10; // [esp+3Ch] [ebp-4h]
//	//KUiSkillState* m_KUiSkillState;
//	//BYTE szBuff[0x6BF4];
//	//memset(&szBuff, 0, sizeof(szBuff));
//
//	//v2 = operator new(0x6BF4u);
//		//v1 = (_DWORD*)CreateSkillState((_DWORD*)m_KUiSkillState);
//	b_sub_42C010(szBuff);
//	//b_sub_42C010();
//	//SkillState_sub_42FC60(szBuff);
//	*szBuff = (_DWORD)&m_DataState;
//	v10 = 0;
//	v8 = &v3;
//	v7 = &v4;
//	v6 = &v5;
//	SkillState_sub_404F30(szBuff + 146);
//	//sub_404F30(&szBuff[146]);
//	//m_KUiSkillState = (KUiSkillState*)&szBuff;
//	//sub_44ADC0
//	v10 = 1;
//	//b_sub_612F01((int)szBuff + 789, 0x244u, 10, (void(__thiscall*)(int))0x42C010, 0x44ADC0);
//	b_sub_612F01((int)szBuff + 789, 0x244u, 10, (void(__thiscall*)(int))0x42C010, 0x44ADC0);
//	v10 = 2;
//	//b_sub_612F01((int)szBuff + 2239, 0x284u, 10, (void(__thiscall*)(int))0x43DCE0, 0x44ADC0);
//	b_sub_612F01((int)szBuff + 2239, 0x284u, 10, (void(__thiscall*)(int))0x43DCE0, 0x44ADC0);
//	v10 = 3;
//	//b_sub_612F01((int)szBuff + 3849, 0x244u, 10, (void(__thiscall*)(int))0x42C010, 0x44ADC0);
//	b_sub_612F01((int)szBuff + 3849, 0x244u, 10, (void(__thiscall*)(int))0x42C010, 0x44ADC0);
//	v10 = 4;
//	//b_sub_612F01((int)szBuff + 5299, 0x284u, 10, (void(__thiscall*)(int))0x43DCE0, 0x44ADC0);
//	b_sub_612F01((int)szBuff + 5299, 0x284u, 10, (void(__thiscall*)(int))0x43DCE0, 0x44ADC0);
//
//	for (i = 0; i < 10; ++i)
//	{
//		memset(&szBuff[32 * i + 149], 0, 0x80u);
//		memset(&szBuff[32 * i + 469], 0, 0x80u);
//	}
//	*(szBuff +145) = 0;
//
//	return szBuff;
//
//	//if (m_KUiSkillState == NULL)
//	//{
//	//	
//	//}
//	//return (_DWORD*)&m_KUiSkillState;
//}
//
//int SkillState_sub_4038B0(_DWORD* This)
//{
//		*(This) = (_DWORD)&gSkillState.m_pSelf;
//		b_sub_612F01((int)This + 789, 0x244u, 10, (void(__thiscall*)(int))0x42C010, 0x44ADC0);
//		b_sub_612F01((int)This + 2239, 0x284u, 10, (void(__thiscall*)(int))0x43DCE0, 0x44ADC0);
//		b_sub_612F01((int)This + 3849, 0x244u, 10, (void(__thiscall*)(int))0x42C010, 0x44ADC0);
//		b_sub_612F01((int)This + 5299, 0x284u, 10, (void(__thiscall*)(int))0x43DCE0, 0x44ADC0);
//		b_sub_40ADA0(This + 146);
//	return (int)b_sub_441B80(This);
//}
//
////_DWORD* sub_404CF0(_DWORD* This)
////{
////	int v1; // ecx
////	int v2; // ecx
////	_DWORD* result; // eax
////	int v5; // [esp+4Ch] [ebp-18h] BYREF
////	_DWORD* v6; // [esp+54h] [ebp-10h]
////	int v7; // [esp+58h] [ebp-Ch]
////	_DWORD* v8; // [esp+5Ch] [ebp-8h]
////	int v9; // [esp+60h] [ebp-4h]
////
////	v1 = This[1];
////	v8 = 0;
////	v9 = v1;
////	if (!This)
////		b_sub_63A3E4();
////	v8 = This;
////	v2 = *(_DWORD*)This[1];
////	v6 = 0;
////	v7 = v2;
////	if (!This)
////		b_sub_63A3E4();
////	v6 = This;
////	sub_40ADA0(This, &v5, (int)This, v7, (int)v8, v9);
////	b_sub_612CD0(*(void**)This[1]);
////	This[1] = 0;
////	result = This;
////	This[2] = 0;
////	return result;
////}
//
////int __thiscall sub_404CF0(int this)
////{
////	int result; // eax
////	int v3; // [esp+40h] [ebp-24h]
////	int v4; // [esp+48h] [ebp-1Ch]
////	int v5; // [esp+4Ch] [ebp-18h] BYREF
////	int v6; // [esp+54h] [ebp-10h]
////	int v7; // [esp+58h] [ebp-Ch]
////	_DWORD** v8; // [esp+5Ch] [ebp-8h]
////	int v9; // [esp+60h] [ebp-4h]
////
////	v4 = *(_DWORD*)(this + 4);
////	v8 = 0;
////	v9 = v4;
////	if (!this)
////		invalid_parameter_noinfo();
////	v8 = (_DWORD**)this;
////	v3 = **(_DWORD**)(this + 4);
////	v6 = 0;
////	v7 = v3;
////	if (!this)
////		invalid_parameter_noinfo();
////	v6 = this;
////	sub_404DB0((_DWORD**)this, &v5, this, v7, v8, v9);
////	operator delete(*(void**)(this + 4));
////	*(_DWORD*)(this + 4) = 0;
////	result = this;
////	*(_DWORD*)(this + 8) = 0;
////	return result;
////}
//
//void SkillState_sub_42FC60(_DWORD* m_KUiSkillState)
//{
//	SkillState_sub_445720((BYTE*)m_KUiSkillState);
//	*(_DWORD*)m_KUiSkillState = 0x63CF4C;
//	*((_DWORD*)m_KUiSkillState + 141) = 0;
//	//b_sub_4255E0((int)m_KUiSkillState + 256);
//	b_sub_4255E0((int)m_KUiSkillState + 396);
//}
//
//BYTE* SkillState_sub_445720(BYTE* m_KUiSkillState)
//{
//	int i; // [esp+4h] [ebp-4h]
//
//	*(_DWORD*)m_KUiSkillState = 0x63DB6C;
//	m_KUiSkillState[32] = 0;
//	*((_DWORD*)m_KUiSkillState + 2) = 0;
//	*((_DWORD*)m_KUiSkillState + 3) = 0;
//	*((_DWORD*)m_KUiSkillState + 4) = 0;
//	*((_DWORD*)m_KUiSkillState + 5) = 0;
//	*((_DWORD*)m_KUiSkillState + 6) = 0;
//	*((_DWORD*)m_KUiSkillState + 7) = 0;
//	*((_DWORD*)m_KUiSkillState + 16) = 0;
//	*((_DWORD*)m_KUiSkillState + 18) = 0;
//	*((_DWORD*)m_KUiSkillState + 17) = 0;
//	*((_DWORD*)m_KUiSkillState + 21) = 0;
//	*((_DWORD*)m_KUiSkillState + 22) = 0;
//	*((_DWORD*)m_KUiSkillState + 23) = 0;
//	*((_DWORD*)m_KUiSkillState + 24) = 0;
//	*((_DWORD*)m_KUiSkillState + 1) = 0x80000000;
//	*((_DWORD*)m_KUiSkillState + 25) = 0;
//	*((_DWORD*)m_KUiSkillState + 26) = 0;
//	*((_DWORD*)m_KUiSkillState + 27) = 0;
//	*((_DWORD*)m_KUiSkillState + 28) = 0;
//	*((_DWORD*)m_KUiSkillState + 29) = 0;
//	for (i = 0; i < 2; ++i)
//		*(_DWORD*)&m_KUiSkillState[4 * i + 76] = -1;
//	*((_DWORD*)m_KUiSkillState + 95) = 0;
//	*((_DWORD*)m_KUiSkillState + 96) = 0;
//	*((_DWORD*)m_KUiSkillState + 97) = 0;
//	*((_DWORD*)m_KUiSkillState + 98) = 0;
//	m_KUiSkillState[120] = 0;
//	return m_KUiSkillState;
//}
//
//int __stdcall sub_62DA72(int a1, int a2, int a3, int(__thiscall* a4)(int))
//{
//	int result; // eax
//	int i; // [esp+34h] [ebp+8h]
//
//	result = a3 * a2;
//	for (i = a3 * a2 + a1; --a3 >= 0; result = a4(i))
//		i -= a2;
//	return result;
//}
//
//_DWORD* SkillState_sub_4039A0(KUiSkillState* This)
//{
//	int i; // [esp+4h] [ebp-10Ch]
//	char Destination[260]; // [esp+8h] [ebp-108h] BYREF
//	// 0x68D090 char[484]
//	//ofstream myfile;
//	//myfile.open("E:\\example.txt");
//	for (i = 0; i < 10; ++i)
//	{
//		//b_sub_442730((_DWORD*)m_KUiSkillState, (_DWORD*)(void*)((_DWORD*)m_KUiSkillState + 145 * i + 789));
//		//b_sub_442730((_DWORD*)m_KUiSkillState, (_DWORD*)(void*)((_DWORD*)m_KUiSkillState + 145 * i + 3849));
//		//VirtualProtect((void*)This[145 * i + 789], 1024, PAGE_EXECUTE_READWRITE, NULL);
//		//
//		//myfile << i << "\n";
//		//myfile << (_DWORD*)This << "\n";
//		////myfile << (_DWORD*)This[145 * i + 789] << "\n";
//		////myfile << (_DWORD*)This[145 * i + 3849] << "\n";
//
//		////myfile << (_DWORD*)(This[145 * i + 789]) << "\n";
//		////myfile << (_DWORD*)(This[145 * i + 3849]) << "\n";
//
//		////myfile << (_DWORD*)&This[145 * i + 789] << "\n";
//		////myfile << (_DWORD*)&This[145 * i + 3849] << "\n";
//
//		//myfile << (_DWORD*)(&This->tmp[145 * i + 789]) << "\n";
//		//myfile << (_DWORD*)(&This->tmp[145 * i + 3849]) << "\n";
//
//		//myfile << *(_DWORD*)&This[145 * i + 789] << "\n";
//		//myfile << *(_DWORD*)&This[145 * i + 3849] << "\n";
//		
//		//VirtualProtect(&This[145 * i + 789], 1024, PAGE_EXECUTE_READWRITE, NULL);
//		b_AddChild((_DWORD*)This, (_DWORD*)&This->tmp[145 * i + 789]);
//		//b_sub_442690_1((_DWORD*)&This[145 * i + 789]);
//		//VirtualProtect(&This[145 * i + 3849], 1024, PAGE_EXECUTE_READWRITE, NULL);
//		b_AddChild((_DWORD*)This, (_DWORD*)&This->tmp[145 * i + 3849]);
//		//b_sub_442690_1((_DWORD*)&This[145 * i + 3849]);
//		//b_sub_442690_1(&This[145 * i + 789]);
//		//b_sub_442690_1(&This[145 * i + 3849]);
//	}
//	//g_UiBase = 0x68D090
//	b_sub_41A4A0((const char*)(void*)0x68D090, Destination, 256);
//	//b_sub_41A4A0((const char*)(void*)0x68D090, (int)Destination, 256);
//	//myfile << This << "\n";
//	//myfile.close();
//	SkillState_sub_403A70(Destination);
//	//LoadScheme(Destination);
//	return b_sub_43AD60((int)This, 1);
//}
//

void CSkillState::LoadScheme(const char* pScheme)
{
	char		Buff[32];
	KIniFile	pgIni;
	int v4;
	char Buffer[132];

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);

	v4 = 0;

	ofstream myfile1;
	myfile1.open("E:\\example4.txt");

	if (m_pSelf && pgIni.Load(Buff))
	{
		Init(&pgIni, "Main");

		pgIni.GetInteger("Main", "IsOpen", 0, &m_nOpen);
		//pgIni.GetInteger("Main", "Left", 0, &m_nLeft);
		//pgIni.GetInteger("Main", "Top", 0, &m_nTop);
		//pgIni.GetInteger("Main", "Width", 0, &m_nWidth);
		//pgIni.GetInteger("Main", "Height", 0, &m_nHeight);

		for (int i = 0; i < 10; ++i)
		{
			//ms_ListSkillsState[i].Init(&pgIni, "BuffImage");
			//ms_ListSkillsState[i].m_txtBuffTime.Init(&pgIni, "txtBuffTime");
			//ms_ListSkillsState[i].SetPosition(24 * i, 0);
			//ms_ListSkillsState[i].AddChild(&ms_ListSkillsState[i].m_txtBuffTime);

			//m_pSelf->ms_ListSkillsState[i].BuffImage.Init(&pgIni, "BuffImage");

			//m_pSelf->ms_ListSkillsState[i].BuffImage.SetPosition(24 * i, 0);
			//m_pSelf->ms_ListSkillsState[i].BuffImage.AddChild(&m_pSelf->ms_ListSkillsState[i].BuffTime);
			//m_pSelf->ms_ListSkillsState[i].BuffImage.Show();
			////m_pSelf->ms_ListSkillsState[i].BuffImage.Hide();

			//m_pSelf->ms_ListSkillsState[i].DebuffTime.Init(&pgIni, "txtDebuffTime");
			//m_pSelf->ms_ListSkillsState[i].DebuffImage.Init(&pgIni, "DebuffImage");

			//m_pSelf->ms_ListSkillsState[i].DebuffImage.SetPosition(24 * i, 36);
			//m_pSelf->ms_ListSkillsState[i].DebuffImage.AddChild(&m_pSelf->ms_ListSkillsState[i].DebuffTime);
			//m_pSelf->ms_ListSkillsState[i].DebuffImage.Show();
			////m_pSelf->ms_ListSkillsState[i].DebuffImage.Hide();
			
			sprintf(Buffer, "Buff_%d_ID", i);
			myfile1 << Buffer << "\n";
			sprintf(Buffer, "Buff_%d_Name", i);
			myfile1 << (void*)Buffer << "\n";
			sprintf(Buffer, "Buff_%d_Image", i);
			myfile1 << (void*)Buffer << "\n";
			sprintf(Buffer, "Buff_%d_Desc", i);
			myfile1 << (void*)Buffer << "\n";
			
		}
		myfile1.close();
		m_pSelf->LoadSkillItems(&pgIni, "BuffList");
	}
	v4 = -1;
	pgIni.~KIniFile();
}

//void CSkillState::LoadScheme(KIniFile* pIni)
//{
//	//Init(pIni, $Main);
//	//m_BuffImage.Init(pIni, "BuffImage");
//	//m_txtBuffTime.Init(pIni, "txtBuffTime");
//}
//
int	CSkillState::LoadSkillItems(KIniFile* pIniFile, const char* pSection)
{
	//int nSkillID;
	//bool result;
	//char Buffer[65];
	//int Data[50];

	//pIniFile->GetInteger(pSection, "BuffCount", 0, &m_nNumSkills);
	//ofstream myfile1;
	//myfile1.open("E:\\example4.txt");
	//for (int i = 0; i < 10; ++i)
	//{
	//	nSkillID = 0;
	//	sprintf(Buffer, "Buff_%d_ID", i);
	//	result = pIniFile->GetInteger(pSection, Buffer, 0, &m_pSelf->ms_ListInfo[i].nID);
	//	if (nSkillID > 0)
	//	{
	//		//memset(m_pSelf->ms_ListInfo[i].m_Name, 0, sizeof(m_pSelf->ms_ListInfo[i].m_Name));
	//		//memset(m_pSelf->ms_ListInfo[i].m_Image, 0, sizeof(m_pSelf->ms_ListInfo[i].m_Image));
	//		//memset(m_pSelf->ms_ListInfo[i].m_Desc, 0, sizeof(m_pSelf->ms_ListInfo[i].m_Desc));
	//		sprintf(Buffer, "Buff_%d_Name", i);
	//		pIniFile->GetString(pSection, Buffer, "N/A", m_pSelf->ms_ListInfo[i].m_Name, 63);
	//		sprintf(Buffer, "Buff_%d_Image", i);
	//		pIniFile->GetString(pSection, Buffer, "N/A", m_pSelf->ms_ListInfo[i].m_Image, 128);
	//		sprintf(Buffer, "Buff_%d_Desc", i);
	//		pIniFile->GetString(pSection, Buffer, "N/A", m_pSelf->ms_ListInfo[i].m_Desc, 80);
	//		myfile1 << m_pSelf->ms_ListInfo[i].nID << "\n";
	//		myfile1 << m_pSelf->ms_ListInfo[i].m_Name << "\n";
	//		myfile1 << m_pSelf->ms_ListInfo[i].m_Image << "\n";
	//		myfile1 << m_pSelf->ms_ListInfo[i].m_Desc << "\n";
	//		KRUImage RUIconImage;
	//		RUIconImage.nType = ISI_T_SPR;
	//		RUIconImage.Color.Color_b.a = 255;
	//		RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	//		RUIconImage.uImage = 0;
	//		RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
	//		RUIconImage.bRenderFlag = 0;
	//		strcpy(RUIconImage.szImage, m_pSelf->ms_ListInfo[i].m_Image);
	//		//m_pSelf->ms_ListSkillsState[i].BuffImage.GetPosition(&RUIconImage.oPosition.nX, &RUIconImage.oPosition.nY);
	//		//RUIconImage.oPosition.nX = x;
	//		//RUIconImage.oPosition.nY = y;
	//		RUIconImage.oPosition.nZ = 0;
	//		RUIconImage.nFrame = 0;
	//		g_pRepresentShell->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 1);
	//	}
	//}
	//myfile1.close();
	//int nRet = KWndObjectBox::Init(pIniFile, pSection);
	//if (nRet)
	//{
	//	char ChName[64];
	//	pIniFile->GetString(pSection, "TextColor", "255,255,0", ChName, sizeof(ChName));
	//	m_TextColor = GetColor(ChName);
	//	return true;
	//}
	/*pIniFile->GetInteger(pSection, "SmoothPing", 0, &m_nSmoothPing);
	pIniFile->GetInteger(pSection, "CrowdPing", 0, &m_nCrowdPing);
	pIniFile->GetInteger(pSection, "BlockPing", 0, &m_nBlockPing);

	pIniFile->GetString(pSection, "SmoothMsg", "", m_szSmoothMsg, sizeof(m_szSmoothMsg));
	pIniFile->GetString(pSection, "CrowdMsg", "", m_szCrowdMsg, sizeof(m_szCrowdMsg));
	pIniFile->GetString(pSection, "BlockMsg", "", m_szBlockMsg, sizeof(m_szBlockMsg));
	pIniFile->GetString(pSection, "GameLogo", GAME_LOGO, m_szLogo, sizeof(m_szLogo));*/
	return 0;
}

//void SkillState_sub_403A70(const char* a1)
//{
//	int i; // [esp+4h] [ebp-F0h]
//	//void* IniFile; // [esp+8h] [ebp-ECh] BYREF
//	//char IniFile[88]; // [esp+8h] [ebp-ECh] BYREF
//	char Buffer[132]; // [esp+60h] [ebp-94h] BYREF
//	int v4; // [esp+F0h] [ebp-4h]
//	char aS_6[32]; // { '\0' }
//	sprintf(Buffer, "%s\\SkillState.ini", a1);
//	//sprintf(Buffer, "\\ui\\ui3\\技能状态列表.ini\x00\x00\x00\x00餦x8Fe");
//	//strcpy(Buffer, "\\Ui\\ui3\\技能状态列表.ini");
//	//sprintf(Buffer, "%s\\技能状态列表.ini", a1);
//	//sprintf(Buffer, "%s\\技能状态列表.ini", a1);
//
//	/*ofstream myfile1;
//	myfile1.open("E:\\example1.txt");
//	myfile1 << (void*)IniFile << "\n";
//	myfile1 << (void*)Ex_KIniFile_Entry << "\n";
//	myfile1 << (void*)Ex_KIniFile_Load << "\n";
//	myfile1 << (void*)Ex_KIniFile_GetInteger << "\n";
//	myfile1 << (void*)Ex_KIniFile_GetString << "\n";
//	myfile1 << (void*)Ex_KIniFile_Clean << "\n";
//	myfile1.close();*/
//
//	//char Buffer[132];
//	//int v4;
//	//KIniFile IniFile;
//
//	//Ini.Load("\\ui\\ui3\\SkillState.ini");
//
//	//Ini.GetInteger("Main", "IsOpen", 0, &v4);
//
//	//ofstream myfile1;
//	//myfile1.open("E:\\example1.txt");
//	////myfile1 << (void*)serverdllBaseAddress << "\n";
//	////myfile1 << (DWORD)serverdllBaseAddress << "\n";
//	////myfile1 << (void*)hModule << "\n";
//	////myfile1 << (DWORD)hModule << "\n";
//	//myfile1 << v4 << "\n";
//	////myfile1 << (DWORD)hModuleEngine << "\n";
//	//myfile1 << (void*)KIniFile_Entry << "\n";
//	//myfile1 << (void*)KIniFile_Load << "\n";
//	//myfile1 << (void*)KIniFile_GetInteger << "\n";
//	//myfile1 << (void*)KIniFile_GetString << "\n";
//	//myfile1 << (void*)KIniFile_Clean << "\n";
//	//myfile1.close();
//
//	//if (m_IsActiveSkillState)
//	//{
//	//	
//	//	//Ex_KIniFile_Entry(IniFile);
//	//	//VirtualProtect((LPVOID)0x63A8F8, 1024, PAGE_EXECUTE_READWRITE, NULL);
//	//	//VirtualProtect((LPVOID)0x63A8E4, 1024, PAGE_EXECUTE_READWRITE, NULL);
//	//	//VirtualProtect((LPVOID)0x63A8F4, 1024, PAGE_EXECUTE_READWRITE, NULL);
//	//	//VirtualProtect((LPVOID)0x63A8E0, 1024, PAGE_EXECUTE_READWRITE, NULL);
//	//	//VirtualProtect((LPVOID)0x63A8DC, 1024, PAGE_EXECUTE_READWRITE, NULL);
//	//	//VirtualProtect((LPVOID)0x63A8EC, 1024, PAGE_EXECUTE_READWRITE, NULL);
//	//	//	HINSTANCE hModuleEngine = LoadLibraryA("engine.dll");
//	//	////dllFunction = (testFunction)GetProcAddress(hModuleEngine, "DllEntryPoint");
//	//	//Ex_KIniFile_Entry = (_KIniFile)GetProcAddress(hModuleEngine, "0KIniFile@@QAE@XZ");
//	//	//Ex_KIniFile_Load = (_KIniFile_Load)GetProcAddress(hModuleEngine, "Load@KIniFile@@QAEHPBD@Z");
//	//	//Ex_KIniFile_GetInteger = (_KIniFile_GetInteger)GetProcAddress(hModuleEngine, "GetInteger@KIniFile@@QAEHPBD0HPAH@Z");
//	//	//Ex_KIniFile_GetString = (_KIniFile_GetString)GetProcAddress(hModuleEngine, "GetString@KIniFile@@QAEHPBD00PADK@Z");
//	//	//Ex_KIniFile_Clean = (_KIniFile_Clean)GetProcAddress(hModuleEngine, "1KIniFile@@QAE@XZ");
//	//	//memset(IniFile,0, 88);
//	//	//IniFile = (void*)0x63A8F8;
//	//	//b_KIniFile(IniFile);
//	//	//Ex_KIniFile_Entry(IniFile);
//
//	///*	ofstream myfile2;
//	//	myfile2.open("E:\\example2.txt");
//	//	myfile2 << "m_IsActiveSkillState true\n";
//	//	myfile2 << Buffer << "\n";
//	//	myfile2 << (void*)m_pSelf << "\n";
//	//	myfile2 << (void*)IniFile << "\n";
//	//	myfile2.close();*/
//
//	//	//INIReader reader(Buffer);
//	//	int test = 0;
//	//	v4 = 0;
//	//	//GetPrivateProfileString("ClientInfo", "ServerName", "", info.ServerName, sizeof(info.ServerName), ".\\ClientInfo.ini");
//	//	//if (Ex_KIniFile_Load(IniFile, Buffer))
//	//	//if (reader.ParseError() == 0)
//
//	//	//if (b_KLoad(IniFile, "\\ui\\ui3\\技能状态列表.ini"))
//	//	//if (Ex_KIniFile_Load(IniFile, Buffer))
//	//	ofstream myfile1;
//	//	myfile1.open("E:\\example2.txt");
//	//	if (IniFile.Load(Buffer))
//	//	{
//	//		
//	//		//(*(void(__thiscall**)(int, const void*, char*))((_DWORD*)m_pSelf + 12))((int)m_pSelf, &IniFile, "Main");
//	//		//b_sub_42C0E0((int)m_pSelf, &IniFile, "Main");
//
//	//		//b_sub_442060((int)m_pSelf, &IniFile, "Main");
//	//		b_sub_42C0E0((int)&m_DataState, &IniFile, "Main");
//	//		//b_GetInteger(IniFile, "Main", "IsOpen", 0, (int*)&m_pSelf->tmp[580]);
//	//		IniFile.GetInteger("Main", "IsOpen", 0, (int*)&m_DataState->tmp[580]);
//	//		//IniFile.GetInteger("Main", "IsOpen", 0, &test);
//	//		//Ex_KIniFile_GetInteger(IniFile, "Main", "IsOpen", 0, (int*)&m_pSelf->tmp[580]);
//	//		ofstream myfile3;
//	//		myfile3.open("E:\\example3.txt");
//	//		myfile3 << (WORD)m_DataState->tmp[580] << "\n";
//	//		myfile3 << (int)m_DataState->tmp[580] << "\n";
//	//		myfile3.close();
//	//		//ofstream myfile4;
//	//		//myfile4.open("E:\\example4.txt");
//	//		//m_pSelf->tmp[580] = reader.GetInteger("Main", "IsOpen", 0);
//	//		for (i = 0; i < 10; ++i)
//	//		{
//	//			//myfile4 << i << "\n";
//	//	/*		(*(void(__thiscall**)(int, KIniFile*, char*))(*(_DWORD*)(&m_pSelf->tmp[644 * i + 8956]) + 12))(
//	//				(int)&m_pSelf->tmp[644 * i + 8956],
//	//				&IniFile,
//	//				"txtBuffTime");*/
//
//	//			b_sub_43D5C0((int)m_DataState->tmp[644 * i + 8956], &IniFile, "txtBuffTime");
//
//	//			ofstream myfile3;
//	//			myfile3.open("E:\\example4.txt");
//	//			myfile3 << (int)m_DataState->tmp[644 * i + 8956] << "\n";
//	//			myfile3 << m_DataState->tmp[580] << "\n";
//	//			myfile3.close();
//	//		/*	(*(void(__thiscall**)(int, KIniFile*, char*))(*(_DWORD*)(&m_pSelf->tmp[580 * i + 3156]) + 12))(
//	//				(int)&m_pSelf->tmp[580 * i + 3156],
//	//				&IniFile,
//	//				"BuffImage");*/
//	////			b_sub_42C0E0((int)m_pSelf->tmp[580 * i + 3156], &IniFile, "BuffImage");
//
//	////			b_SetPosition((_DWORD*)&m_pSelf->tmp[580 * i + 3156], 24 * i, 0);
//	////			b_AddChild((_DWORD*)&m_pSelf->tmp[580 * i + 3156], (_DWORD*)&m_pSelf->tmp[644 * i + 8956]);
//
//	////			b_sub_441F00((_DWORD*)&m_pSelf->tmp[580 * i + 3156]);
//	////			//(*(void(__thiscall**)(int))((_DWORD*)&m_pSelf->tmp[580 * i + 3156 + 36]))((int)&m_pSelf->tmp[580 * i + 3156]);
//	////			//b_sub_42C0E0((int)&m_pSelf->tmp[644 * i + 21196], IniFile, "txtDebuffTime");
//	////			
//	////		/*	(*(void(__thiscall**)(int, KIniFile*, char*))(*(_DWORD*)(&m_pSelf->tmp[644 * i + 21196]) + 12))(
//	////				(int)&m_pSelf->tmp[644 * i + 21196],
//	////				&IniFile,
//	////				"txtDebuffTime");*/
//	//			myfile1 << i << "\n";
//	//			myfile1 << m_DataState->tmp[580 * i + 8956] << "\n";
//	//			//myfile1 << m_pSelf->tmp[580 * i + 3156] << "\n";
//
//	////			b_sub_43D5C0((int)&m_pSelf->tmp[644 * i + 21196], &IniFile, "txtDebuffTime");
//
//	/////*			(*(void(__thiscall**)(int, KIniFile*, char*))(*(_DWORD*)(&m_pSelf->tmp[580 * i + 15396]) + 12))(
//	////				(int)&m_pSelf->tmp[580 * i + 15396],
//	////				&IniFile,
//	////				"DebuffImage");*/
//
//	////			b_sub_42C0E0((int)m_pSelf->tmp[580 * i + 15396], &IniFile, "DebuffImage");
//
//	////			b_SetPosition((_DWORD*)&m_pSelf->tmp[580 * i + 15396], 24 * i, 36);
//	////			b_AddChild((_DWORD*)&m_pSelf->tmp[580 * i + 15396], (_DWORD*)&m_pSelf->tmp[644 * i + 21196]);
//
//	////			b_sub_441F00((_DWORD*)&m_pSelf->tmp[580 * i + 15396]);
//	//			//(*(void(__thiscall**)(int))(*(_DWORD*)(&m_pSelf->tmp[580 * i + 15396]) + 36))((int)&m_pSelf->tmp[580 * i + 15396]);
//	//		}
//	//		//myfile4.close();
//	//		//SkillState_sub_404670(IniFile, "BuffList", m_pSelf + 584);
//	//		//SkillState_sub_404670(&IniFile, "BuffList", (int)&m_pSelf->tmp[584]); // LoadConfig
//	//		//SkillState_sub_404670(reader, "BuffList", (int)&m_pSelf->tmp[584]);
//	//	}
//	//	myfile1.close();
//	//	v4 = -1;
//	//	IniFile.Clear();
//		//b_CleanIniFile(IniFile);
//		//Ex_KIniFile_Clean(IniFile);
//	//}
//	
//}
//
//// LoadConfig
//int SkillState_sub_404670(KIniFile* a1, const char* a2, int a3)
//{
//	int result; // eax
//	//int* v4; // eax
//	//int v5; // edx
//	//char v6[12]; // [esp+68h] [ebp-2A8h] BYREF
//	//int v7; // [esp+74h] [ebp-29Ch]
//	//int v8; // [esp+78h] [ebp-298h]
//	//int v9; // [esp+7Ch] [ebp-294h] BYREF
//	//char v10[200]; // [esp+80h] [ebp-290h] BYREF
//	//int v11; // [esp+148h] [ebp-1C8h]
//	//char v12[200]; // [esp+14Ch] [ebp-1C4h] BYREF
//	//int v13[50]; // [esp+218h] [ebp-F8h] BYREF
//	//int v14; // [esp+2E0h] [ebp-30h] BYREF
//	//int i; // [esp+2E4h] [ebp-2Ch]
//	//char Buffer; // [esp+2E8h] [ebp-28h] BYREF
//	//int v17; // [esp+2E9h] [ebp-27h]
//	//int v18; // [esp+2EDh] [ebp-23h]
//	//int v19; // [esp+2F1h] [ebp-1Fh]
//	//int v20; // [esp+2F5h] [ebp-1Bh]
//	//int v21; // [esp+2F9h] [ebp-17h]
//	//int v22; // [esp+2FDh] [ebp-13h]
//	//int v23; // [esp+301h] [ebp-Fh]
//	//__int16 v24; // [esp+305h] [ebp-Bh]
//	//char v25; // [esp+307h] [ebp-9h]
//	//int v26; // [esp+30Ch] [ebp-4h] BYREF
//
//	//v26 = 0;
//	////b_GetInteger(reader, (const char*)a2, "BuffCount", 0, &v26);
//	//a1->GetInteger("BuffList", "BuffCount", 0, &v26);
//	////Ex_KIniFile_GetInteger(reader, (const char*)a2, "BuffCount", 0, &v26);
//	////v26 = reader.GetInteger(a2, "BuffCount", 0);
//	//Buffer = (char)(void*)0x63ABB8;
//	//result = 0;
//	//v17 = 0;
//	//v18 = 0;
//	//v19 = 0;
//	//v20 = 0;
//	//v21 = 0;
//	//v22 = 0;
//	//v23 = 0;
//	//v24 = 0;
//	//v25 = 0;
//	//for (i = 0; i < v26; ++i)
//	//{
//	//	v14 = 0;
//	//	sprintf(&Buffer, "Buff_%d_ID", i);
//	//	//v14 = reader.GetInteger(a2, &Buffer, 0);
//	//	//result = b_GetInteger(reader, (const char*)a2, &Buffer, 0, &v14);
//	//	a1->GetInteger("BuffList", "BuffCount", 0, &v14);
//	//	//result = Ex_KIniFile_GetInteger(reader, (const char*)a2, (const char*)Buffer, 0, &v14);
//	//	ofstream myfile3;
//	//	myfile3.open("E:\\example5.txt");
//	//	myfile3 << &v14 << "\n";
//	//	//myfile3 << m_pSelf << "\n";
//	//	//myfile3 << Buffer << "\n";
//	//	myfile3.close();
//	//	if (v14 > 0)
//	//		result = 1;
//	//	if (v14 > 0)
//	//	{
//	//		memset(v13, 0, sizeof(v13));
//	//		sprintf(&Buffer, "Buff_%d_Level", i);
//	//		//b_GetInteger(reader, (char*)a2, &Buffer, -1, &v13[48]);
//	//		a1->GetInteger("BuffList", &Buffer, -1, &v13[48]);
//	//		//Ex_KIniFile_GetInteger(reader, (const char*)a2, (const char*)Buffer, -1, &v13[48]);
//	//		//v13[48] = reader.GetInteger(a2, &Buffer, -1);
//	//		sprintf(&Buffer, "Buff_%d_IsDebuff", i);
//	//		//b_GetInteger(reader, (char*)a2, &Buffer, 0, &v13[49]);
//	//		a1->GetInteger("BuffList", &Buffer, 0, &v13[49]);
//	//		//Ex_KIniFile_GetInteger(reader, (const char*)a2, (const char*)Buffer, 0, &v13[49]);
//	//		//v13[49] = reader.GetInteger(a2, &Buffer, -1);
//	//		sprintf(&Buffer, "Buff_%d_Name", i);
//	//		//b_GetString(reader, (char*)a2, &Buffer, (char*)0x63ABB8, (char*)&v13, 63);
//	//		a1->GetString("BuffList", &Buffer, (char*)0x63ABB8, (char*)&v13, 63);
//	//		//Ex_KIniFile_GetString(reader, (const char*)a2, (const char*)Buffer, (char*)0x63ABB8, (char*)&v13, 63);
//	//		//strcpy((char*)&v13, reader.GetString(a2, &Buffer, 0).c_str());
//
//	//		sprintf(&Buffer, "Buff_%d_Image", i);
//	//		//b_GetString(reader, (char*)a2, &Buffer, (char*)0x63ABB8, (char*)&v13[16], 63);
//	//		a1->GetString("BuffList", &Buffer, (char*)0x63ABB8, (char*)&v13[16], 63);
//	//		//Ex_KIniFile_GetString(reader, (const char*)a2, (const char*)Buffer, (char*)0x63ABB8, (char*)&v13[16], 63);
//	//		//strcpy((char*)&v13[16], reader.GetString(a2, &Buffer, 0).c_str());
//
//	//		sprintf(&Buffer, "Buff_%d_Desc", i);
//	//		//b_GetString(reader, (char*)a2, &Buffer, (char*)0x63ABB8, (char*)&v13[32], 63);
//	//		a1->GetString("BuffList", &Buffer, (char*)0x63ABB8, (char*)&v13[32], 63);
//	//		//Ex_KIniFile_GetString(reader, (const char*)a2, (const char*)Buffer, (char*)0x63ABB8, (char*)&v13[32], 63);
//	//		//strcpy((char*)&v13[32], reader.GetString(a2, &Buffer, 0).c_str());
//
//	//		v11 = v14;
//	//		memcpy(v12, v13, sizeof(v12));
//	//		v9 = v14;
//	//		memcpy(v10, v12, sizeof(v10));
//	//		v4 = (int*)SkillState_sub_404AB0((_DWORD*)a3, (int)v6, (_DWORD*)&v9);
//	//		v5 = *v4;
//	//		result = v4[1];
//	//		v7 = v5;
//	//		v8 = result;
//	//	}
//	//}
//	return result;
//}
//
//int SkillState_sub_404AB0(_DWORD* Data, int a2, _DWORD* a3)
//{
//	int v3; // ecx
//	int v5; // [esp+0h] [ebp-B0h]
//	_DWORD* v6; // [esp+1Ch] [ebp-94h]
//	char v7[11]; // [esp+90h] [ebp-20h] BYREF
//	char v8; // [esp+9Bh] [ebp-15h]
//	int v9; // [esp+A4h] [ebp-Ch]
//	_DWORD* v10; // [esp+A8h] [ebp-8h]
//	bool v11; // [esp+AFh] [ebp-1h]
//
//	v9 = *(_DWORD*)(&Data[1] + 4);
//	v10 = (_DWORD*)&Data[1];
//	v11 = 1;
//	while (!*(BYTE*)(v9 + 217))
//	{
//		v10 = (_DWORD*)v9;
//		v11 = *a3 < *(_DWORD*)(v9 + 12);
//		if (v11)
//			v5 = *(_DWORD*)v9;
//		else
//			v5 = *(_DWORD*)(v9 + 8);
//		v9 = v5;
//	}
//	v8 = 1;
//	v6 = SkillState_sub_404FA0(Data, (_DWORD*)v7, v11, v10, (int)a3);
//	v3 = v6[1];
//	*(_DWORD*)a2 = *v6;
//	*(_DWORD*)(a2 + 4) = v3;
//	*(BYTE*)(a2 + 8) = v8;
//	return a2;
//}
//
//_DWORD* SkillState_sub_404FA0(_DWORD* Data, _DWORD* a2, char a3, _DWORD* a4, int a5)
//{
//	int v6; // [esp+4h] [ebp-C4h]
//	char v8[28]; // [esp+70h] [ebp-58h] BYREF
//	int pExceptionObject[10]; // [esp+8Ch] [ebp-3Ch] BYREF
//	_DWORD* v10; // [esp+B4h] [ebp-14h]
//	_DWORD* v11; // [esp+B8h] [ebp-10h]
//	int v12; // [esp+C4h] [ebp-4h]
//	int v13; // [esp+D8h] [ebp+10h]
//	int v14; // [esp+D8h] [ebp+10h]
//
//	if (byte_1414141)
//		v6 = (int)byte_1414141;
//	else
//		v6 = 1;
//	if ((unsigned int)(v6 - 1) <= Data[2])
//	{
//		sprintf(v8, "map/set<T> too long");
//		//std::string::string(v8, "map/set<T> too long");
//		v12 = 0;
//		b_sub_402370((_DWORD*)(std::exception*)pExceptionObject, (int)v8);
//		//pExceptionObject[0] = (int)&off_657C6C;
//		//CxxThrowException(pExceptionObject, &_TI3_AVlength_error_std__);
//	}
//	v11 = (_DWORD*)SkillState_sub_405FD0(Data, Data[1], (int)a4, Data[1], &a5, 0);
//	++Data[2];
//	if (a4 == (_DWORD*)Data[1])
//	{
//		*(_DWORD*)(Data[1] + 4) = *v11;
//		*(_DWORD*)Data[1] = *v11;
//		*(_DWORD*)(Data[1] + 8) = *v11;
//	}
//	else if (a3)
//	{
//		*a4 = *v11;
//		if (a4 == *(_DWORD**)Data[1])
//			*(_DWORD*)Data[1] = *v11;
//	}
//	else
//	{
//		a4[2] = *v11;
//		if (a4 == *(_DWORD**)(Data[1] + 8))
//			*(_DWORD*)(Data[1] + 8) = *v11;
//	}
//	v10 = v11;
//	while (!*(BYTE*)(v10[1] + 216))
//	{
//		if (v10[1] == **(_DWORD**)(v10[1] + 4))
//		{
//			v13 = *(_DWORD*)(*(_DWORD*)(v10[1] + 4) + 8);
//			if (*(BYTE*)(v13 + 216))
//			{
//				if (v10 == *(_DWORD**)(v10[1] + 8))
//				{
//					v10 = (_DWORD*)v10[1];
//					SkillState_sub_405D20(Data, (int)v10);
//				}
//				*(BYTE*)(v10[1] + 216) = 1;
//				*(BYTE*)(*(_DWORD*)(v10[1] + 4) + 216) = 0;
//				SkillState_sub_405DD0(Data, *(_DWORD**)(v10[1] + 4));
//			}
//			else
//			{
//				*(BYTE*)(v10[1] + 216) = 1;
//				*(BYTE*)(v13 + 216) = 1;
//				*(BYTE*)(*(_DWORD*)(v10[1] + 4) + 216) = 0;
//				v10 = *(_DWORD**)(v10[1] + 4);
//			}
//		}
//		else
//		{
//			v14 = **(_DWORD**)(v10[1] + 4);
//			if (*(BYTE*)(v14 + 216))
//			{
//				if (v10 == *(_DWORD**)v10[1])
//				{
//					v10 = (_DWORD*)v10[1];
//					SkillState_sub_405DD0(Data, v10);
//				}
//				*(BYTE*)(v10[1] + 216) = 1;
//				*(BYTE*)(*(_DWORD*)(v10[1] + 4) + 216) = 0;
//				SkillState_sub_405D20(Data, *(_DWORD*)(v10[1] + 4));
//			}
//			else
//			{
//				*(BYTE*)(v10[1] + 216) = 1;
//				*(BYTE*)(v14 + 216) = 1;
//				*(BYTE*)(*(_DWORD*)(v10[1] + 4) + 216) = 0;
//				v10 = *(_DWORD**)(v10[1] + 4);
//			}
//		}
//	}
//	*(BYTE*)(*(_DWORD*)(Data[1] + 4) + 216) = 1;
//	*a2 = 0;
//	a2[1] = *v11;
//	if (!Data)
//		b_invalid_parameter_noinfo();
//	*a2 = *Data;
//	return a2;
//}
//
//void* SkillState_sub_405FD0(void* Data, int a2, int a3, int a4, const void* a5, char a6)
//{
//	int v7; // [esp+0h] [ebp-44h] BYREF
//	void* v8; // [esp+10h] [ebp-34h]
//	void* v9; // [esp+14h] [ebp-30h]
//	_DWORD* v10; // [esp+28h] [ebp-1Ch]
//	void* v11; // [esp+30h] [ebp-14h]
//	int* v12; // [esp+34h] [ebp-10h]
//	int v13; // [esp+40h] [ebp-4h]
//
//	v12 = &v7;
//	v9 = Data;
//	v11 = SkillState_sub_406230(1u);
//	v13 = 0;
//	v10 = (_DWORD*)v11;
//	if (v11)
//	{
//		*v10 = a2;
//		v10[1] = a3;
//		v10[2] = a4;
//		memcpy(v10 + 3, a5, 0xCCu);
//		*((BYTE*)v10 + 216) = a6;
//		*((BYTE*)v10 + 217) = 0;
//		v8 = v10;
//	}
//	else
//	{
//		v8 = 0;
//	}
//	return v11;
//}
//
//int SkillState_sub_405D20(_DWORD* Data, int a2)
//{
//	int result; // eax
//	int* v3; // [esp+Ch] [ebp-4h]
//
//	v3 = *(int**)(a2 + 8);
//	*(_DWORD*)(a2 + 8) = *v3;
//	if (!*(BYTE*)(*v3 + 217))
//		*(_DWORD*)(*v3 + 4) = a2;
//	v3[1] = *(_DWORD*)(a2 + 4);
//	if (a2 == *(_DWORD*)(Data[1] + 4))
//	{
//		*(_DWORD*)(Data[1] + 4) = *v3;
//	}
//	else if (a2 == **(_DWORD**)(a2 + 4))
//	{
//		**(_DWORD**)(a2 + 4) = *v3;
//	}
//	else
//	{
//		*(_DWORD*)(*(_DWORD*)(a2 + 4) + 8) = *v3;
//	}
//	*v3 = a2;
//	result = a2;
//	*(_DWORD*)(a2 + 4) = *v3;
//	return result;
//}
//
//int SkillState_sub_405DD0(_DWORD* Data, _DWORD* a2)
//{
//	int result; // eax
//	int v3; // [esp+8h] [ebp-4h]
//
//	v3 = *a2;
//	*a2 = *(_DWORD*)(*a2 + 8);
//	if (!*(BYTE*)(*(_DWORD*)(v3 + 8) + 217))
//		*(_DWORD*)(*(_DWORD*)(v3 + 8) + 4) = *a2;
//	*(_DWORD*)(v3 + 4) = a2[1];
//	if (a2 == *(_DWORD**)(Data[1] + 4))
//	{
//		*(_DWORD*)(Data[1] + 4) = v3;
//	}
//	else if (a2 == *(_DWORD**)(a2[1] + 8))
//	{
//		*(_DWORD*)(a2[1] + 8) = v3;
//	}
//	else
//	{
//		*(_DWORD*)a2[1] = v3;
//	}
//	*(_DWORD*)(v3 + 8) = *a2;
//	result = v3;
//	a2[1] = v3;
//	return result;
//}
//
//
//void* SkillState_sub_404F30(void* This)
//{
//	try
//	{
//		void* result; // eax
//
//		*((_DWORD*)This + 1) = *(_DWORD*)SkillState_sub_405E80(&This);
//		*((_DWORD*)This + 218) = 0;
//		*((_DWORD*)This + 5) = *((_DWORD*)This + 1);
//		*((_DWORD*)This + 1) = *((_DWORD*)This + 1);
//		result = This;
//		*((_DWORD*)This + 9) = *((_DWORD*)This + 1);
//		*((_DWORD*)This + 2) = 0;
//		return result;
//	}
//	catch (std::bad_alloc) {
//		MessageBox(0, "sub_404F30", "Testing", MB_OK);
//	}
//	
//}
//
//void* SkillState_sub_405E80(void* This)
//{
//	try
//	{
//		int v2; // [esp+0h] [ebp-70h] BYREF
//		_DWORD* v3; // [esp+10h] [ebp-60h]
//		_DWORD* v4; // [esp+14h] [ebp-5Ch]
//		_DWORD* v5; // [esp+18h] [ebp-58h]
//		void* v6; // [esp+1Ch] [ebp-54h]
//		_DWORD* v7; // [esp+20h] [ebp-50h]
//		int v8; // [esp+24h] [ebp-4Ch]
//		_DWORD* v9; // [esp+28h] [ebp-48h]
//		int v10; // [esp+2Ch] [ebp-44h]
//		_DWORD* v11; // [esp+30h] [ebp-40h]
//		void* v12; // [esp+34h] [ebp-3Ch]
//		int v13; // [esp+4Ch] [ebp-24h]
//		int v14; // [esp+50h] [ebp-20h]
//		int v15; // [esp+54h] [ebp-1Ch]
//		void* v16; // [esp+58h] [ebp-18h]
//		int v17; // [esp+5Ch] [ebp-14h]
//		int* v18; // [esp+60h] [ebp-10h]
//		int v19; // [esp+6Ch] [ebp-4h]
//
//		v18 = &v2;
//		v6 = &This;
//		v16 = SkillState_sub_406110(1);
//		v17 = 0;
//		v19 = 0;
//		v15 = 0;
//		v12 = v16;
//		v11 = (_DWORD*)v16;
//		if (v16)
//		{
//			*v11 = v15;
//			v5 = v11;
//		}
//		else
//		{
//			v5 = 0;
//		}
//		++v17;
//		v14 = 0;
//		v10 = (int)v16 + 4;
//		v9 = (_DWORD*)(char*)v16 + 4;
//		if (v16 == (void*)-4)
//		{
//			v4 = 0;
//		}
//		else
//		{
//			*v9 = v14;
//			v4 = v9;
//		}
//		++v17;
//		v13 = 0;
//		v8 = (int)v16 + 8;
//		v7 = (_DWORD*)(char*)v16 + 8;
//		if (v16 == (void*)-8)
//		{
//			v3 = 0;
//		}
//		else
//		{
//			*v7 = v13;
//			v3 = v7;
//		}
//		*((BYTE*)v16 + 216) = 1;
//		*((BYTE*)v16 + 217) = 0;
//		return v16;
//	}
//	catch (std::bad_alloc) {
//		MessageBox(0, "sub_405E80", "Testing", MB_OK);
//	}
//	
//}
//
//void* __stdcall SkillState_sub_406110(int a1)
//{
//	return SkillState_sub_406230(a1);
//}
//
//void* __cdecl SkillState_sub_406230(unsigned int a1)
//{
//	const char* v2; // [esp+0h] [ebp-10h] BYREF
//	int pExceptionObject[3]; // [esp+4h] [ebp-Ch] BYREF
//
//	if (a1)
//	{
//		try
//		{
//			if (0xFFFFFFFF / a1 < 0xDC)
//			{
//				v2 = 0;
//			}
//		}
//		catch (std::bad_alloc) {
//			MessageBox(0, "sub_406230", "Testing", MB_OK);
//		}
//	}
//	else
//	{
//		a1 = 0;
//	}
//	return operator new(220 * a1);
//}

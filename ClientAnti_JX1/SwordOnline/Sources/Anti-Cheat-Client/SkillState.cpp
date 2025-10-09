//#include <KWin32.h>
//#include "KWin32App.h"
//#include <KWin32Wnd.h>
//#include <KPakList.h>
#include "Util.h"
//#include <KWin32.h>
//#include "Windows.h"
//#include "KCanvas.h"
//#include <KIniFile.h>
//#include <KList.h>
//#include <iRepresent/iRepresentShell.h>
//#include "KRepresentUnit.h"
//#include <Elem/WndMessage.h>
//#include "INIReader.h"
#include "SkillState.h"
//#include <KIniFile.h>
#include "KPlayerSafe.h"
#include "Protect.h"
//#include "KTabFile.h"
//#include "WndImage.cpp"
//#include "WndWindow.cpp"
//#include "WndText.cpp"
//#include "WndImage.h"
#include <shlwapi.h>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <csignal>
//#include <iRepresent/iRepresentShell.h>
//#include "KRepresentUnit.h"
//#include "../../Engine/src/Text.h"
#include "Import.h"
#include <Text.h>
#include "KUiMissionTime.h"
#include "KRepresentShell2.h"
//#include "NewUiShell.h"
//#include "NewUiShell.h"
//#include "AutoPlay.h"
//#include <Text.h>
#include <math.h>
//#include <chrono>
//#include <mutex>
//#include "WndShowAnimate.h" 
//#include "Image/ImageOperation.h"
//#include "RepresentUtility.h"
//#include <Gdiplus.h>
//#include "Wnds.h"

//#include "detours.h"

//#include <iRepresent/iRepresentShell.h>
//#include "KRepresentUnit.h"
//#include "RepresentUtility.h"
//#include "Image/ImageOperation.h"
//#include "Font/KFont2.h"

using namespace std;

//extern KWndImage* g_WndImage;
//extern KWndText32* g_WndText32;
//KUiStateSkill*	g_UiStateSkill;
extern iRepresentShell* g_pRepresentShell = NULL;
//iRepresentShell* g_pRepresentShell = NULL;
//struct iRepresentShell* g_pRepresentShell = NULL;
iRepresentShell* g_pRepresent = 0;
IInlinePicEngineSink* g_pIInlinePicSinkRP = NULL;

//KWndAutoPlay g_WndAutoPlay;

HWND MainHWnd;
HWND DrawHWnd;
HMODULE	hRepresentModule;
HMODULE	hCoreShellModule;
HINSTANCE hModuleEngine;

HANDLE ThreadCheckState1;
HANDLE ThreadHandles2;
HANDLE WINAPI GetCurrentProcess(void);
std::map<int, SkillStateLive> StateSkill_Data;

HANDLE ThreadHandlesCheckSkill;
HANDLE hProcess;

#define SCHEME_INI						"SkillState.ini"
#define WND_S_VISIBLE				0x80000000	//¿É¼û

unsigned int m_BaseAddress = NULL;
unsigned int AddressOffsetNpcCount = NULL;
unsigned int AddressOffsetMap1 = NULL;
unsigned int AddressOffset1 = NULL;
unsigned int AddressOffset2 = NULL;
unsigned int AddressOffset3 = NULL;
unsigned int AddressOffset4 = NULL;
unsigned int AddressOffset5 = NULL;
unsigned int AddressOffset6 = NULL;
unsigned int SkillInfoOffset1 = NULL;
unsigned int SkillInfoOffset2 = NULL;
unsigned int SkillInfoOffset3 = NULL;
unsigned int SkillInfoOffset4 = NULL;

unsigned int KPlayer_Offset1 = NULL;
unsigned int RoleName_Offset1 = NULL;
unsigned int PlayerIndex_Offset1 = NULL;
unsigned int nPlayerIndex_Offset1 = NULL;

unsigned int GameData_Offset1 = NULL;
unsigned int GameData_Skill_Offset1 = NULL;
unsigned int GameData_Skill_Offset2 = NULL;
unsigned int GameData_Skill_Offset3 = NULL;

bool					IsActivePlayerNpc = false;
bool					IsActiveMissleSame = false;
bool					IsActiveVisiblePlayer = false;

bool					InitStart = false;
bool					InitUiState = false;
int					m_PlayerIndex = NULL;
int					m_NpcCount = NULL;
int					m_MapId = 0;
int					m_MapIdCurren = 0;
int					m_StateList = NULL;
int					m_StateCount = 0;
int					m_SkillCount = 0;
char c_AccountName[32];
char c_ClientSerial[33];
char c_HardwareId[32];
unsigned int					GameData = (unsigned int)0x12C5540;
unsigned int					SkillInfo = NULL;
SkillStateLive	ms_ListStateSkill[STATE_SKILL_MAX_COUNT];
int	ms_ListSkill[512];
int	szNumBuffShow = 0;
KUiSkillStateData	ms_ListInfo[1999];
_KListGetHead KListGetHead = (_KListGetHead)0x63A66C;
_DetectSynSkill CallDetectSynSkill = (_DetectSynSkill)0x580850;
_SetStateSkillEffect SetStateSkillEffect = (_SetStateSkillEffect)0x57B080;
KUiStateSkill* KUiStateSkill::m_pSelf = NULL;
KUiStateSkill	g_UiStateSkill;
//KWndWindow* gWndWindow;
//KWndImage* gWndImage;
//KWndButton* gWndButton;
//KUiStateSkill _KUiStateSkill;
//extern unsigned int m_StateSkill = (unsigned int)0x6AE930;
//_SetStateSkillEffect UiStateSkill = (_SetStateSkillEffect)0x6AE930;
// 0x68E0E0

void SetRepresentShell(struct iRepresentShell* pRepresent)
{
	Core_SetRepresentShell(pRepresent);
	g_pRepresentShell = pRepresent;
	KRepresentShell2* g_pRepresentShell;
}

void __fastcall Main_RenderWindows()
{

	_Wnd_RenderWindows();
	
	

	/*ReadProcessMemory(hProcess, (void*)(unsigned int)0x8E48F4, &AddressOffset1, sizeof(AddressOffset1), NULL);

	AddressOffset2 = (AddressOffset1 + 0x8B30);

	AddressOffset3 = (AddressOffset2 + 0x114);

	ReadProcessMemory(hProcess, (void*)(unsigned int)AddressOffset3, &m_PlayerIndex, sizeof(m_PlayerIndex), NULL);*/

	//ReadProcessMemory(hProcess, (void*)(unsigned int)GameData, &SkillInfoOffset1, sizeof(SkillInfoOffset1), NULL);

	gUtil.KPlayer_Offset1 = *(unsigned int*)(0x8E48F4);

	gUtil.m_PlayerIndex = *(unsigned int*)(gUtil.KPlayer_Offset1 + 0x8B30 + 0x114);

	// Offset01 = (KPlayer_Offset1 + 0x8C44) * 0x113B8
	// Offset02 = 0x12C5540 + Offset01
	// Offset03 = Offset02 + 0x130D

	//RoleName_Offset1 = *(unsigned int*)(*(unsigned int*)(0x12C5540 + (*(unsigned int*)(*(unsigned int*)(0x8E48F4) + 0x8C44) * 0x113B8)) + 0x130D);
	
	if (gUtil.m_PlayerIndex > 0)
	{
		//if (gProtect.m_GiamHinhAnh > 0 || gProtect.m_GiamCPU > 0)
		//{
		//	m_NpcCount = *(unsigned int*)(m_BaseAddress + 0xEC5598 + 0x1E94 + 4);
		//	if (gProtect.m_GiamHinhAnh > 0)
		//	{
		//		if (gProtect.m_NpcLevel1 > 0 && gProtect.m_NpcLevel2 > 0)
		//		{
		//			if (m_NpcCount >= gProtect.m_NpcLevel1 && m_NpcCount < gProtect.m_NpcLevel2)
		//			{
		//				if (gProtect.m_GiamHinhAnh == 2)
		//				{
		//					if (!IsActivePlayerNpc)
		//					{
		//						IsActivePlayerNpc = true;
		//						b_ExecuteScriptFunc("Open([[PlayerNpc]])");
		//					}
		//					if (!IsActiveMissleSame)
		//					{
		//						IsActiveMissleSame = true;
		//						b_ExecuteScriptFunc("Open([[MissleSame]])");
		//					}
		//				}
		//				else
		//				{
		//					if (*(BYTE*)(m_BaseAddress + 0x185471) == 117 && *(BYTE*)(m_BaseAddress + 0x185471 + 1) == 237)
		//					{
		//						WORD num1 = MAKEWORD(144, 144);
		//						SetWord((m_BaseAddress + 0x185471), num1);
		//					}

		//					if (*(BYTE*)(m_BaseAddress + 0x1853ED) == 144 && *(BYTE*)(m_BaseAddress + 0x1853ED + 1) == 233)
		//					{
		//						WORD num2 = MAKEWORD(15, 132);
		//						SetWord((m_BaseAddress + 0x1853ED), num2);
		//					}
		//				}
		//				//b_SetGraphic(*((DWORD*)0x68FA30), 1);
		//			}
		//			else if (m_NpcCount >= gProtect.m_NpcLevel2)
		//			{
		//				if (gProtect.m_GiamHinhAnh == 2)
		//				{
		//					if (!IsActiveVisiblePlayer)
		//					{
		//						IsActiveVisiblePlayer = true;
		//						b_ExecuteScriptFunc("Open([[VisiblePlayer]])");
		//					}
		//				}
		//				else
		//				{
		//					if (*(BYTE*)(m_BaseAddress + 0x185471) == 144 && *(BYTE*)(m_BaseAddress + 0x185471 + 1) == 144)
		//					{
		//						WORD num1 = MAKEWORD(117, 237);
		//						SetWord((m_BaseAddress + 0x185471), num1);
		//					}

		//					if (*(BYTE*)(m_BaseAddress + 0x1853ED) == 144 && *(BYTE*)(m_BaseAddress + 0x1853ED + 1) == 132)
		//					{
		//						WORD num2 = MAKEWORD(144, 233);
		//						SetWord((m_BaseAddress + 0x1853ED), num2);
		//					}
		//				}
		//			}
		//			else
		//			{
		//				if (gProtect.m_GiamHinhAnh == 2)
		//				{
		//					if (IsActivePlayerNpc)
		//					{
		//						IsActivePlayerNpc = false;
		//						b_ExecuteScriptFunc("Open([[PlayerNpc]])");
		//					}

		//					if (IsActiveMissleSame)
		//					{
		//						IsActiveMissleSame = false;
		//						b_ExecuteScriptFunc("Open([[MissleSame]])");
		//					}

		//					if (IsActiveVisiblePlayer)
		//					{
		//						IsActiveVisiblePlayer = false;
		//						b_ExecuteScriptFunc("Open([[VisiblePlayer]])");
		//					}

		//				}
		//				else
		//				{
		//					if (*(BYTE*)(m_BaseAddress + 0x185471) == 144 && *(BYTE*)(m_BaseAddress + 0x185471 + 1) == 144)
		//					{
		//						WORD num1 = MAKEWORD(117, 237);
		//						SetWord((m_BaseAddress + 0x185471), num1);
		//					}

		//					if (*(BYTE*)(m_BaseAddress + 0x1853ED) == 144 && *(BYTE*)(m_BaseAddress + 0x1853ED + 1) == 233)
		//					{
		//						WORD num2 = MAKEWORD(15, 132);
		//						SetWord((m_BaseAddress + 0x1853ED), num2);
		//					}
		//				}
		//			}
		//		}
		//	}
		//}
		
		//ofstream myfile1;
		//myfile1.open("C:\\mapid.txt");
		//myfile1 << m_PlayerIndex << "\n";
		//myfile1 << InitUiState << "\n";
		//myfile1 << m_MapId << "\n";
		//myfile1 << *(unsigned int*)(&m_pSelf[1]) << "\n";
		//myfile1 << WND_S_VISIBLE << "\n";
		//myfile1.close();

		//KRUImage	Image;
		//Image.nType = ISI_T_SPR;
		//Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		//Image.uImage = 0;
		//Image.nISPosition = -1;
		//strcpy(Image.szImage, "\\spr\\Ui\\×´Ì¬Í¼±ê\\ÂÞººÕó.spr");
		////Image.szImage[128] = 0;
		////myfile2 << ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Image << "\n";
		////myfile2 << Image.szImage << "\n";
		//Image.Color.Color_b.a = 255;
		//Image.bRenderFlag = 0;
		//Image.nFrame = 0;
		//Image.oPosition.nX = 39;
		//Image.oPosition.nY = 28;
		//Image.oPosition.nZ = 0;
		//Image.nNew1 = 0;

	//	KRUImage	Image;
	//Image.nType = ISI_T_SPR;
	//Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	//Image.uImage = 0;
	//Image.nISPosition = -1;
	//strcpy(Image.szImage, "\\spr\\Ui\\×´Ì¬Í¼±ê\\ÂÞººÕó.spr");
	////Image.szImage[128] = 0;
	////myfile2 << ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Image << "\n";
	////myfile2 << Image.szImage << "\n";
	//Image.Color.Color_b.a = 255;
	//Image.bRenderFlag = 0;
	//Image.nFrame = 0;
	//Image.oPosition.nX = 39;
	//Image.oPosition.nY = 28;
	//Image.oPosition.nZ = 0;
	//Image.nNew1 = 0;

	//g_pRepresentShell->DrawPrimitives(1, &Image, RU_T_IMAGE, TRUE);

		/*if (!InitUiState)
		{
			KUiStateSkill::OpenWindow();
			InitUiState = true;
		}
		else
			KUiStateSkill::UpdateData();*/

		try
		{
			if (!gUtil.IsInGame)
			{
				gUtil.IsInGame = true;
				KUiStateSkill::OpenWindow();
				//KUiMissionTime::OpenWindow();
				//KUiAutoPlay::OpenWindow();
				//KWndAutoPlay::OpenWindow();

			}
			else
			{
				if (InitStart)
				{
					//Player_AutoPlay::OpenWindow();
					if (m_MapId != gProtect.m_StateSkill)
					{
						KUiStateSkill::OpenWindow();
						//KUiMissionTime::OpenWindow();
						//KUiAutoPlay::OpenWindow();
						//KWndAutoPlay::OpenWindow();
						//KUiAutoPlay::PaintWindow(g_pRepresentShell);
					}
					else
						KUiStateSkill::CloseWindow(false);
				}
			}
		}
		catch (...)
		{ }

		//g_pRepresentShell->DrawPrimitives(1, &Image, RU_T_IMAGE, TRUE);
		//ofstream myfile2;
		//myfile2.open("E:\\RenderWindows.txt");
		
		//for (int i = 0; i < STATE_SKILL_MAX_COUNT; i++)
	}
	else
	{
		IsActivePlayerNpc = false;
		IsActiveMissleSame = false;
		IsActiveVisiblePlayer = false;

		if (!InitStart)
		{
			InitStart = true;

			DWORD pid = GetCurrentProcessId();

			hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, FALSE, pid);

			//hProcess = GetCurrentProcess();

		/*	ofstream myfile2;
			myfile2.open("C:\\StartGame.txt");
			myfile2 << GetCurrentProcessId() << "\n";
			myfile2 << hProcess << "\n";
			myfile2.close();*/

			//RegisterComClass("KUiAutoPlayButton");

			KIniFile	Ini;

			BOOL result;

			result = b_sub_4207E0();

			/*char IDBuffer[32];
			char TextBuffer[128];
			char TextNameTemp[128];
			char szTipTemp[256];
			char szTip[256];*/

			char* IDBuffer = (char*)malloc(32);
			char* TextBuffer = (char*)malloc(128);
			char* TextNameTemp = (char*)malloc(128);
			char* szTipTemp = (char*)malloc(256);
			char* szTip = (char*)malloc(256);

			int szTipLen;
			int szTipMaxtrix;
			int szTipLenCenter;
			int szSkillNameLen;
			float abc;

			HMODULE hModule = GetModuleHandle(NULL);

			m_BaseAddress = (DWORD)((void*)hModule);

			if (Ini.Load("\\Ui\\ui3\\SkillState.ini"))
			{

				Ini.GetInteger("BuffList", "BuffCount", 0, &szNumBuffShow);

				for (int i = 0; i < szNumBuffShow; ++i)
				{
					szTipLen = 0;
					szSkillNameLen = 0;
					szTipLenCenter = 0;
					szTipMaxtrix = 0;
					abc = 0;
					int SkillID = 0;

					memset(IDBuffer, 0, 32);

					sprintf(IDBuffer, "Buff_%d_ID", i);

					//SkillID = reader.GetInteger("BuffList", IDBuffer, 0);

					Ini.GetInteger("BuffList", IDBuffer, 0, &SkillID);
					ms_ListInfo[SkillID].nSkillID = SkillID;
					ms_ListSkill[i] = SkillID;
					memset(TextBuffer, 0, 128);
					memset(TextNameTemp, 0, 128);
					sprintf(TextBuffer, "Buff_%d_Name", i);

					Ini.GetString("BuffList", TextBuffer, "N/A", ms_ListInfo[SkillID].m_Name, 32);

					//memset(TextBuffer, 0, 128);
					//sprintf(TextBuffer, "<color=yellow>%s%%<color>\n\n", ms_ListInfo[SkillID].m_Name);
					//strcpy(ms_ListInfo[SkillID].m_Name, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
					//myfile2 << ms_ListInfo[SkillID].m_Name << "\n";

					memset(TextBuffer, 0, 128);
					sprintf(TextBuffer, "Buff_%d_Image", i);

					Ini.GetString("BuffList", TextBuffer, "N/A", ms_ListInfo[SkillID].m_Image, 128);
					//strcpy(ms_ListInfo[SkillID].m_Image, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
					//myfile2 << ms_ListInfo[SkillID].m_Image << "\n";

					memset(TextBuffer, 0, 128);
					sprintf(TextBuffer, "Buff_%d_Desc", i);

					memset(szTip, 0, 256);
					memset(szTipTemp, 0, 256);

					Ini.GetString("BuffList", TextBuffer, "N/A", szTip, 256);
					//strcpy(szTip, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
					//myfile2 << ms_ListInfo[SkillID].m_Desc << "\n";

					szTipLen = strlen(szTip);
					szSkillNameLen = strlen(ms_ListInfo[SkillID].m_Name);
					//szTipLenCenter = (szTipLen / 2) - (szSkillNameLen + 1);
					if (szTipLen > szSkillNameLen)
					{
						if (float((szTipLen / 2) - (szSkillNameLen + (szSkillNameLen / 2))) > 1)
						{

							abc = float((szTipLen / 2) - (szSkillNameLen + (szSkillNameLen * 2) / 3));
						}
						else
							abc = 1;
					}

					szTipLenCenter = (int)(abc);


					//myfile2 << "szTipLen " << szTip << "\n"; // skill id
					//myfile2 << "szTipLen " << szTipLen << "\n"; // skill id
					//myfile2 << "szTipLen " << ms_ListInfo[SkillID].m_Name << "\n"; // skill id
					//myfile2 << "szSkillNameLen " << szSkillNameLen << "\n"; // skill id
					//myfile2 << "szTipLenCenter " << szTipLenCenter << "\n"; // skill id
					
					memset(szTipTemp, *(const char*)"Ù", szTipLenCenter);
					strcat(szTipTemp, "<color=yellow>");
					//strcat(ms_ListInfo[SkillID].m_Name, TextNameTemp);
					//strcat(ms_ListInfo[SkillID].m_Name, "<color>\n");
					//strcpy(&szTipTemp[szTipLenCenter], ms_ListInfo[SkillID].m_Name);
					strcat(szTipTemp, ms_ListInfo[SkillID].m_Name);
					strcat(szTipTemp, "<color>\n\n");
					strcat(szTipTemp, szTip);
					strcat(szTipTemp, "\n\n");
					strcat(szTipTemp, "<color=yellow>CÊp ®é:<color> ");
					//szTipTemp[szTipLenCenter + szSkillNameLen] = '\n';
					//strcpy(&szTipTemp[szTipLenCenter + szSkillNameLen + 1], szTip);
					strcpy(ms_ListInfo[SkillID].m_Desc, szTipTemp);

					memset(TextBuffer, 0, 128);
					sprintf(TextBuffer, "Buff_%d_IsAura", i);

					Ini.GetInteger("BuffList", TextBuffer, 0, &ms_ListInfo[SkillID].m_IsAura);

					memset(TextBuffer, 0, 128);
					sprintf(TextBuffer, "Buff_%d_IsState", i);

					Ini.GetInteger("BuffList", TextBuffer, 0, &ms_ListInfo[SkillID].m_IsState);
					//ms_ListInfo[SkillID].m_IsAura = reader.GetInteger("BuffList", TextBuffer, 0);
					//myfile2 << ms_ListInfo[SkillID].m_IsAura << "\n";

					memset(TextBuffer, 0, 128);
					sprintf(TextBuffer, "Buff_%d_IsDebuff", i);

					Ini.GetInteger("BuffList", TextBuffer, 0, &ms_ListInfo[SkillID].m_IsDebuff);
					Ini.GetInteger("BuffList", TextBuffer, 0, &ms_ListInfo[SkillID].m_IsDebuff);
					//ms_ListInfo[SkillID].m_IsDebuff = reader.GetInteger("BuffList", TextBuffer, 0);
					//myfile2 << ms_ListInfo[SkillID].m_IsDebuff << "\n";
				}
			}

			Ini.Clear();
			//myfile2.close();
			free(IDBuffer);
			free(TextBuffer);
			free(TextNameTemp);
			free(szTipTemp);
			free(szTip);

			for (int i = 0; i < STATE_SKILL_MAX_COUNT; i++)
			{

				ms_ListStateSkill[i].nIndex = i;

				ms_ListStateSkill[i].nSkillID = 0;

				ms_ListStateSkill[i].nSkillTime = -1;

				ms_ListStateSkill[i].m_LeftImage = 39 + (28 * i);

				ms_ListStateSkill[i].m_TopImage = 32;

				ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);

				ms_ListStateSkill[i].m_TopText = 56;

			}

		/*	ReadProcessMemory(hProcess, (void*)(unsigned int)0x8E48F4, &AddressOffset1, sizeof(AddressOffset1), NULL);

			AddressOffset2 = (AddressOffset1 + 0x8B30);

			AddressOffset3 = (AddressOffset2 + 0x114);

			ReadProcessMemory(hProcess, (void*)(unsigned int)GameData, &SkillInfoOffset1, sizeof(SkillInfoOffset1), NULL);*/

			
			//WaitForSingleObject(ThreadCheckState1, 666);
			
			//Player_AutoPlay::RegisterSelfClass();
		}
		else
		{
			if (gUtil.IsInGame)
			{
				gUtil.IsInGame = false;
				KUiStateSkill::CloseWindow(false);
				//KWndAutoPlay::CloseWindow(false);
				//KUiAutoPlay::CloseWindow(false);
			}
		}
	}
}

int __stdcall exec_sub_556590(int a1, int a2)
{
	/*ofstream myfile;
	myfile.open("C:\\exec_sub_556590.txt");
	myfile << "a1 " << a1 << "\n";
	myfile << "a2 " << a2 << "\n";
	myfile << "m_SkillCount " << m_SkillCount << "\n";
	myfile << "SkillID " << *(DWORD*)(a1 + 7) << "\n";
	myfile << "nLevel " << *(DWORD*)(a1 + 11) << "\n";
	myfile << "nTime " << *(DWORD*)(a1 + 15) << "\n";
	

	m_SkillCount++;

	int skilltime_value = NULL;
	SkillStateLive skillinfo;
	skillinfo.nSkillID = *(DWORD*)(a1 + 7);
	skillinfo.nFloatTime = *(DWORD*)(a1 + 15);
	skilltime_value = static_cast<int>((*(DWORD*)(a1 + 15) * 60 / 1000) + 0.5);
	skillinfo.m_TickTime = GetTickCount();

	std::map<int, SkillStateLive>::iterator it = StateSkill_Data.find(skillinfo.nSkillID);

	char	textTime[10];

	if ((int)skilltime_value <= 1)
	{
		if (ms_ListInfo[skillinfo.nSkillID].m_IsAura == 1)
		{
			strcpy(textTime, "N/A");
		}
		else
		{
			sprintf(textTime, "%ds", (int)skilltime_value);

		}
	}
	else if ((int)skilltime_value > 60 && (int)skilltime_value < 3600)
	{
		sprintf(textTime, "%dm", ((int)skilltime_value / 60));
	}
	else if ((int)skilltime_value >= 3600)
	{
		sprintf(textTime, "%dh", ((int)skilltime_value / 3600));
	}
	else
	{
		sprintf(textTime, "%ds", (int)skilltime_value);
	}

	memcpy(&skillinfo.m_TextTime, textTime, 10);

	skillinfo.m_TextTime[10] = 0;

	if (it == StateSkill_Data.end())
	{
		StateSkill_Data.insert(std::pair<int, SkillStateLive>(*(DWORD*)(a1 + 7), skillinfo));
	}
	else
	{

		memcpy(&it->second, &skillinfo, sizeof(SkillStateLive));

	}

	myfile << "StateSkill_Data Size " << StateSkill_Data.size() << "\n";
	myfile.close();
	*/
	return b_sub_556590(a1, a2);
}

int WINAPI DetectSynSkill(int a2)
{

	// ms_ListStateSkill
	// 0x44EE
	// 0x113B8
	// 0x12C5540
	// 0x907A04
	// 0x204F90
	// 0x1AD9780

	m_PlayerIndex = -1;

	int result;
	int data1 = -1;
	int data2 = -1;
	int data3 = -1;

	result = 0;
	
	result = CallDetectSynSkill((const char*)0x12C5598, a2);

	// 0x6E2418

	/*DWORD* This = (DWORD*)0x6E2418;

	ofstream myfile;
	myfile.open("E:\\DetectSynSkill.txt");
	myfile << "result " << result << "\n";
	myfile << "SkillID " << *(int*)((DWORD*)(This[135]) + 7) << "\n";
	myfile << "nLevel " << *(int*)((DWORD*)This[135] + 11) << "\n";
	myfile << "nTime " << *(int*)((DWORD*)This[135] + 15) << "\n";*/
	//myfile.close();

	if (result)
	{
		gUtil.IsInGame = true;
		int SkillID = NULL;
		int SkillTime = NULL;
		gUtil.m_PlayerIndex = result;

		/*ofstream myfile2;
		myfile2.open("C:\\DetectSynSkill.txt");
		myfile2 << m_PlayerIndex << "\n";
		myfile2 << InitUiState << "\n";
		myfile2 << ThreadCheckState1 << "\n";
		myfile2 << StateSkill_Data.size() << "\n";
		myfile2.close();*/

	}

	return result;
}

int __stdcall SyncStateSkill(int Index, int SkillID, int nLevel, int Data, int nDataNum, int nTime, int a7, int a8, int a9, int a10, int a11)
{
	//ofstream myfile;
	//myfile.open("E:\\SyncStateSkill_1.txt");
	//myfile << "Index " << This << "\n"; // skill id
	//myfile.close();

	/*if (nLevel <= 0 || Data <= 0)
		return 0;

	if (nTime >= 20)
		nTime = 20;*/
	
	//char DataSkill[100];
	int result;

	//int result;

	//result = 0;

	//_DWORD* Head;

	//Head = (_DWORD*)0x12C5540 + 70584 * Index;

	//KListGetHead();

	//ofstream myfile2;
	//myfile2.open("E:\\SyncStateSkill.txt");
	//myfile2 << "Index " << Index << "\n"; // skill id
	//myfile2 << "SkillID " << SkillID << "\n"; // skill id
	//myfile2 << "nLevel " << nLevel << "\n"; // skill level
	//myfile2 << "Data " << Data << "\n"; // data
	//myfile2 << "nDataNum " << nDataNum << "\n"; // data
	//myfile2 << "nTime " << nTime << "\n"; // skill time
	//myfile2 << "a7 " << a7 << "\n";
	//myfile2 << "a8 " << a8 << "\n";
	//myfile2 << "a9 " << a9 << "\n";
	//myfile2 << "a10 " << a10 << "\n";
	//myfile2 << "a11 " << a11 << "\n";
	//myfile2 << "this[1144] " << ((DWORD*)0x12C5540 + 70584 * Index)[1144] << "\n";
	//myfile2 << "this[1] " << ((DWORD*)0x12C5540 + 70584 * Index)[1] << "\n";
	//myfile2 << "this[1187] " << ((DWORD*)0x12C5540 + 70584 * Index)[1187] << "\n";
	//myfile2 << "call_sub_5757F0 " << call_sub_5757F0(((DWORD*)0x12C5540 + 70584 * Index)[1187]) << "\n";
	//myfile2 << "a12 " << (DWORD*)(0x12C5540 + (17646 * Index)) << "\n";
	//myfile2 << "Head " << Head + 63 << "\n";
	//myfile2 << "result " << *((_DWORD*)Head + 4) << "\n";
	//myfile2 << "result " << *((_DWORD*)Head + 6) << "\n";
	//myfile2 << "result " << (_DWORD*)((char*)Head + 40) << "\n";
	//myfile2.close();

	//ThreadHandles1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckSkillState, 0, 0, 0);

	//SetThreadPriority(ThreadHandles1, THREAD_PRIORITY_HIGHEST);

	//return ((int(__stdcall*)(DWORD*, int, int, int, int, int, int, int, int, int, int, int))0x57B080)(
		//(DWORD*)0x12C5540 + 17646 * Index, Index, SkillID, nLevel, Data, nDataNum, nTime, a7, a8, a9, a10, a11);

	//SetSkillState(Index, SkillID, nTime, nLevel, Data);
	//result = b_SyncStateSkill((_DWORD*)0x12C5540 + 70584 * Index, Index, SkillID, nLevel, Data, nDataNum, nTime, a7, a8, a9, a10, a11); // 0x6E2418	
	//result = SetStateSkillEffect((_DWORD*)0x12C5540 + 17646 * Index, Index, SkillID, nLevel, Data, nDataNum, nTime, a7, a8, a9, a10, a11); // 0x6E2418	
	//ofstream myfile3;
	//myfile3.open("E:\\SetSkillState3.txt");
	//myfile3 << result << "\n";
	//myfile3.close();
	//return nTime;
	//return result;
	//return SetStateSkillEffect(void*(*(DWORD*)0x12C5540 + 17646 * Index), Index, SkillID, nLevel, Data, nDataNum, nTime, a7, a8, a9, a10, a11); // 0x6E2418	
	return b_SyncStateSkill((_DWORD*)0x12C5540 + 70584 * Index, Index, SkillID, nLevel, Data, nDataNum, nTime, a7, a8, a9, a10, a11); // 0x6E2418	
	//return _SyncStateSkill((DWORD*)(0x12C5540 + 17646 * Index), Index, SkillID, nLevel, Data, nDataNum, nTime, a7, a8, a9, a10, a11); // 0x6E2418
	//b_SyncStateSkill_codecl(Index, SkillID, nLevel, Data, a5, nTime, a7, a8, a9, a10, a11); // 0x6E2418
	//return result;
}

DWORD WINAPI SyncSkillState(void*)
{
	//ReadProcessMemory(hProcess, (void*)(unsigned int)0x8E48F4, &AddressOffset1, sizeof(AddressOffset1), NULL);

	//AddressOffset2 = (AddressOffset1 + 0x8B30);

	//AddressOffset3 = (AddressOffset2 + 0x114);

	//ReadProcessMemory(hProcess, (void*)(unsigned int)AddressOffset3, &m_PlayerIndex, sizeof(m_PlayerIndex), NULL);

	//ofstream myfile;
	//myfile.open("E:\\DetectSynSkill.txt");
	//myfile << "m_PlayerIndex " << m_PlayerIndex << "\n";
	//if (m_PlayerIndex)
	//{
	//	IsInGame = true;
	//	int SkillID = NULL;
	//	int SkillTime = NULL;

	//	ReadProcessMemory(hProcess, (void*)(unsigned int)GameData, &SkillInfoOffset1, sizeof(SkillInfoOffset1), NULL);

	//	SkillInfoOffset2 = SkillInfoOffset1 + 0x113B8 * m_PlayerIndex;

	//	SkillInfoOffset3 = SkillInfoOffset2 + 0x100;

	//	ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset3 + 4), &m_StateList, sizeof(m_StateList), NULL);
	//	myfile << "m_StateList " << m_StateList << "\n";
	//	if (m_StateList > 0)
	//	{
	//		ReadProcessMemory(hProcess, (void*)SkillInfoOffset3, &SkillInfoOffset4, sizeof(SkillInfoOffset4), NULL);

	//		char textnum[16];

	//		for (int i = 0; i < (m_StateList - 1); i++)
	//		{
	//			//if (m_PlayerIndex <= 0)
	//				//break;

	//			float skilltime_value = -1;

	//			if (i < (m_StateList - 1))
	//			{
	//				ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 16), &SkillID, sizeof(SkillID), NULL);

	//				ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 24), &SkillTime, sizeof(SkillTime), NULL);

	//				ms_ListStateSkill[i].nIndex = i;

	//				ms_ListStateSkill[i].nSkillID = SkillID;

	//				skilltime_value = static_cast<int>(SkillTime * 60 / 1000);

	//				ms_ListStateSkill[i].nSkillTime = (int)skilltime_value;

	//				myfile << SkillID << "\n";
	//				myfile << SkillTime << "\n";
	//				myfile << skilltime_value << "\n";

	//				char	textTime[32];

	//				/*if ((int)skilltime_value / 3600 > 10)
	//					continue;*/

	//					//myfile2 << ms_ListStateSkill[i].nSkillTime << "\n";
	//					//myfile2 << skilltime_value << "\n";
	//					//myfile2 << ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_IsAura << "\n";

	//				if ((int)skilltime_value <= 1)
	//				{
	//					if (ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_IsAura == 1)
	//					{
	//						strcpy(textTime, "N/A");
	//						ms_ListStateSkill[i].m_LeftText = 42 + (29 * i);
	//					}
	//					else
	//					{
	//						sprintf(textTime, "%ds", (int)skilltime_value);
	//						if ((int)skilltime_value < 10)
	//							ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
	//						else
	//							ms_ListStateSkill[i].m_LeftText = 42 + (29 * i);
	//					}
	//				}
	//				else if ((int)skilltime_value > 60 && (int)skilltime_value < 3600)
	//				{
	//					sprintf(textTime, "%dm", ((int)skilltime_value / 60));
	//					if ((int)skilltime_value < 10)
	//						ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
	//					else
	//						ms_ListStateSkill[i].m_LeftText = 42 + (29 * i);
	//				}
	//				else if ((int)skilltime_value >= 3600)
	//				{
	//					sprintf(textTime, "%dh", ((int)skilltime_value / 3600));
	//					if ((int)skilltime_value < 10)
	//						ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
	//					else
	//						ms_ListStateSkill[i].m_LeftText = 42 + (29 * i);
	//				}
	//				else
	//				{
	//					sprintf(textTime, "%ds", (int)skilltime_value);
	//					if ((int)skilltime_value < 10)
	//						ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
	//					else
	//						ms_ListStateSkill[i].m_LeftText = 42 + (29 * i);
	//				}

	//				memcpy(&ms_ListStateSkill[i].m_TextTime, textTime, 10);

	//				ms_ListStateSkill[i].m_TextTime[10] = 0;

	//				ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 8), &SkillInfoOffset4, sizeof(SkillInfoOffset4), NULL);

	//			}
	//			else
	//			{

	//				ms_ListStateSkill[i].nIndex = i;

	//				ms_ListStateSkill[i].nSkillID = 0;

	//				ms_ListStateSkill[i].nSkillTime = skilltime_value;

	//				memset(ms_ListStateSkill[i].m_TextTime, 0, 10);

	//				ms_ListStateSkill[i].m_LeftText = 42 + (28 * i);

	//			}
	//		}


	//	}
	//	myfile.close();
	//}

	//return 1;
}

DWORD WINAPI CheckSkillState(void*)
{

	// BUFF_BASE_ADDR 0x8E4D00
	// BUFF_BASE_SIZE 0x4AC 1196
	// BUFF_BASE_OFFSET 0x2C 11 44
	// 
	// BUFF_DOING_OFFSET 0x1718 5912
	// BUFF_SKILL_ID_OFFSET 0x171C
	// BUFF_COUNT_OFFSET 0x38 56
	// BUFF_COMPARE_SIZE 0x8
	// BUFF_TIME_OFFSET 0x12C5540
	// BUFF_TIME_SIZE 0x0FC

	/*ReadProcessMemory(hProcess, (void*)(unsigned int)0x8E48F4, &AddressOffset1, sizeof(AddressOffset1), NULL);

	AddressOffset2 = (AddressOffset1 + 0x8B30);

	AddressOffset3 = (AddressOffset2 + 0x114);*/

	HMODULE hModule = GetModuleHandle(NULL);

	m_BaseAddress = (DWORD)((void*)hModule);

	AddressOffsetNpcCount = (m_BaseAddress + 0xEC5598 + 0x1E94 + 4);

	AddressOffsetMap1 = (m_BaseAddress + 0xEFA5C0);

	KIniFile	Ini;

	if (Ini.Load("AntiVolam.ini"))
	{
		Ini.GetInteger("GENERAL", "DisableStateSkill", 0, &gProtect.m_StateSkill);
	}

	Ini.Clear();
	//ofstream myfile1;
	//	myfile1.open("C:\\BaseAddress.txt");
	//	myfile1 << m_PlayerIndex << "\n";
	//	//myfile2 << InitUiState << "\n";
	//	//myfile2 << ThreadCheckState1 << "\n";
	//	myfile1.close();

	//ReadProcessMemory(hProcess, (void*)(unsigned int)hModule, &m_BaseAddress, sizeof(m_BaseAddress), NULL);

	ReadProcessMemory(hProcess, (void*)(unsigned int)GameData, &SkillInfoOffset1, sizeof(SkillInfoOffset1), NULL);

	//KPlayer_Offset1 = *(unsigned int*)(0x8E48F4);
	// Offset01 = (KPlayer_Offset1 + 0x8C44) * 0x113B8
	// Offset02 = 0x12C5540 + Offset01
	// Offset03 = Offset02 + 0x130D
	// PlayerName = GetString Offset03
	// m_PlayerIndex = *(unsigned int*)(KPlayer_Offset1 + 0x8B30 + 0x114);
	// strcpy(ms_ListInfo[SkillID].m_Desc, szTipTemp);
	BYTE nPaintFps = 1;
	int nTimeUpdatePaint = 900;

	/*if (*(int*)0x680490 > 20)
		SetWord(0x680490, 20);*/

	while (true)
	{
		try
		{
			/*if (!IsInGame && m_PlayerIndex == 0)
			{
				KUiStateSkill::CloseWindow(false);
				Sleep(300);
				continue;
			}*/

			//m_PlayerIndex = *(unsigned int*)(KPlayer_Offset1 + 0x8B30 + 0x114);

			if (gProtect.m_GiamCPU > 0)
				m_NpcCount = *(unsigned int*)(AddressOffsetNpcCount);

			//m_StateCount = 0;

			AddressOffsetMap1 = (m_BaseAddress + 0xEFA5C0);

			ReadProcessMemory(hProcess, (void*)(unsigned int)AddressOffsetMap1, &m_MapId, sizeof(m_MapId), NULL);

			if (m_MapId > 0)
			{
				if (m_MapId != m_MapIdCurren)
				{
					//SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);
					SetProcessWorkingSetSizeEx(hProcess,
						0xFFFFFFFF,
						0xFFFFFFFF,
						QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE
					);
					m_MapIdCurren = m_MapId;
				}
			}

			//break;
			/*if (IsInGame)
			{
				if (InitStart)
				{
					if (m_MapId == gProtect.m_StateSkill)
					{
						KUiStateSkill::CloseWindow(false);
						Sleep(300);
						continue;
					}
				}
			}*/

			//ReadProcessMemory(hProcess, (void*)(unsigned int)AddressOffsetNpcCount, &m_NpcCount, sizeof(m_NpcCount), NULL);

			//ReadProcessMemory(hProcess, (void*)(unsigned int)AddressOffset3, &m_PlayerIndex, sizeof(m_PlayerIndex), NULL);

			//KPlayer_Offset1 = *(unsigned int*)(0x8E48F4);

			//m_PlayerIndex = *(unsigned int*)(KPlayer_Offset1 + 0x8B30 + 0x114);

			//myfile2 << hProcess << "\n";
			//myfile2 << (DWORD*)AddressOffset1 << "\n";
			//myfile2 << (DWORD*)AddressOffset3 << "\n";
			//myfile2 << "PlayerIndex: " << (DWORD)m_PlayerIndex << "\n";
			//myfile2 << *(int*)(DWORD*)AddressOffset3 << "\n";
		
			//ofstream myfile1;
			//myfile1.open("C:\\data1.txt");
			//myfile1 << m_PlayerIndex << "\n";
			////myfile2 << InitUiState << "\n";
			////myfile2 << ThreadCheckState1 << "\n";
			//myfile1.close();

			if (m_PlayerIndex > 0)
			{
				/*ReadProcessMemory(hProcess, (void*)(unsigned int)AddressOffsetMap1, &m_MapId, sizeof(m_MapId), NULL);

				if (m_MapId != gProtect.m_StateSkill)
				{*/
					//SkillInfoOffset2 = (unsigned int)(SkillInfoOffset1 + 0x113B8 * m_PlayerIndex);

				//SkillInfoOffset3 = SkillInfoOffset2 + 0x100;

				//ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset3 + 4), &m_StateList, sizeof(m_StateList), NULL);

				//ofstream myfile2;
				//myfile2.open("C:\\data2.txt");
				//myfile2 << m_StateList << "\n";
				////myfile2 << InitUiState << "\n";
				////myfile2 << ThreadCheckState1 << "\n";
				//myfile2.close();

					//SkillInfoOffset2 = (unsigned int)(SkillInfoOffset1 + 0x113B8 * m_PlayerIndex);

					//SkillInfoOffset3 = SkillInfoOffset2 + 0x100;

					////ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset3 + 4), &m_StateList, sizeof(m_StateList), NULL);

					//GameData_Offset1 = *(unsigned int*)(GameData);

					//GameData_Skill_Offset1 = (unsigned int)(GameData_Offset1 + 0x113B8 * m_PlayerIndex + 0x100);

					//m_StateList = *(unsigned int*)(GameData_Skill_Offset1 + 4);

					////ofstream myfile2;
					////myfile2.open("C:\\data2.txt");
					////myfile2 << m_StateList << "\n";
					//////myfile2 << InitUiState << "\n";
					//////myfile2 << ThreadCheckState1 << "\n";
					////myfile2.close();

					//if (m_StateList > 0)
					//{
					//	int SkillID = NULL;
					//	int SkillLevel = NULL;
					//	int SkillTime = NULL;

					//	ReadProcessMemory(hProcess, (void*)SkillInfoOffset3, &SkillInfoOffset4, sizeof(SkillInfoOffset4), NULL);

					//	for (int i = 0; i < STATE_SKILL_MAX_COUNT; i++)
					//	{

					//		//Sleep(30);

					//		int skilltime_value = NULL;

					//		ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 16), &SkillID, sizeof(SkillID), NULL);

					//		ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 20), &SkillLevel, sizeof(SkillLevel), NULL);

					//		ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 24), &SkillTime, sizeof(SkillTime), NULL);

					//		if (SkillTime > 0 && i < m_StateList)
					//		{

					//			char	textTime[10];

					//			skilltime_value = static_cast<int>((SkillTime * 60 / 1000) + 0.5);
					//			//myfile2 << "skilltime_value: " << skilltime_value << "\n";
					//			//if (skilltime_value / 3600 > 8 || SkillID >= 661 && SkillID <= 667)
					//			if (SkillID >= 661 && SkillID <= 667)
					//			{
					//				continue;
					//			}

					//			ms_ListStateSkill[i].nIndex = i;

					//			ms_ListStateSkill[i].nSkillID = SkillID;

					//			ms_ListStateSkill[i].nSkillLevel = SkillLevel;

					//			ms_ListStateSkill[i].nFloatTime = SkillTime;

					//			ms_ListStateSkill[i].nSkillTime = (int)skilltime_value;

					//			if ((int)SkillTime <= 18 && (int)SkillTime > 0)
					//			{
					//				if (ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_IsAura == 1)
					//				{
					//					strcpy(textTime, "N/A");
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
					//				}
					//				else
					//				{
					//					sprintf(textTime, "%ds", (int)skilltime_value);
					//					if ((int)skilltime_value < 10)
					//						ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
					//					else
					//						ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
					//				}
					//			}
					//			else if ((int)skilltime_value > 60 && (int)skilltime_value < 3600)
					//			{
					//				sprintf(textTime, "%dm", ((int)skilltime_value / 60));
					//				if (((int)skilltime_value / 60) < 10)
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
					//				else
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
					//			}
					//			else if ((int)skilltime_value >= 3600 && (int)skilltime_value < 86400)
					//			{
					//				sprintf(textTime, "%dh", ((int)skilltime_value / 3600));
					//				if (((int)skilltime_value / 3600) < 10)
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 1;
					//				else
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
					//			}
					//			else if ((int)skilltime_value >= 86400)
					//			{
					//				sprintf(textTime, "%dN", ((int)skilltime_value / 86400));
					//				if (((int)skilltime_value / 86400) < 10)
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 1;
					//				else
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
					//			}
					//			else
					//			{
					//				sprintf(textTime, "%ds", (int)skilltime_value);
					//				if ((int)skilltime_value < 10)
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
					//				else
					//					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
					//			}

					//			memcpy(&ms_ListStateSkill[i].m_TextTime, textTime, 10);

					//			ms_ListStateSkill[i].m_TextTime[10] = 0;

					//			m_StateCount++;

					//		}
					//		else
					//		{
					//			ms_ListStateSkill[i].nIndex = i;

					//			ms_ListStateSkill[i].nSkillID = 0;

					//			ms_ListStateSkill[i].nSkillLevel = 0;

					//			ms_ListStateSkill[i].nFloatTime = skilltime_value;

					//			ms_ListStateSkill[i].nSkillTime = skilltime_value;

					//			memset(ms_ListStateSkill[i].m_TextTime, 0, 10);

					//			ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);

					//		}

					//		ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 8), &SkillInfoOffset4, sizeof(SkillInfoOffset4), NULL);

					//		Sleep(10);

					//	}

					//	//nPaintFps = nPaintFps + (m_StateCount * 2);
					//	// 0x128102
					//		// 0x119A17
					//		// 0x1281FE
					//		// 0x185471
					//		// 0x1853ED
					//		// 
					//		// 0x528102
					//		// 0x519A17
					//		// 0x5281FE
					//		// 0x5281FE

					//	/*ofstream myfile1;
					//	myfile1.open("C:\\Windows\\count.txt");
					//	myfile1 << m_NpcCount << "\n";
					//	myfile1 << m_PlayerIndex << "\n";
					//	myfile1.close();*/

					//

					//	if (gProtect.m_GiamCPU == 1)
					//	{
					//		if (m_NpcCount >= 30 && m_NpcCount < 100)
					//		{
					//			nPaintFps = 20;
					//			/*SetByte((m_BaseAddress + 0x185471), 144);
					//			SetByte((m_BaseAddress + 0x185471 + 1), 144);
					//			SetByte((m_BaseAddress + 0x1853ED), 15);
					//			SetByte((m_BaseAddress + 0x1853ED + 1), 132);*/

					//		}
					//		else if (m_NpcCount >= 100)
					//		{
					//			nPaintFps = 40;
					//			/*SetByte((m_BaseAddress + 0x185471), 117);
					//			SetByte((m_BaseAddress + 0x185471 + 1), 237);
					//			SetByte((m_BaseAddress + 0x1853ED), 144);
					//			SetByte((m_BaseAddress + 0x1853ED + 1), 233);*/
					//		}
					//		else
					//		{
					//			nPaintFps = 1;
					//			/*SetByte((m_BaseAddress + 0x185471), 117);
					//			SetByte((m_BaseAddress + 0x185471 + 1), 237);
					//			SetByte((m_BaseAddress + 0x1853ED), 15);
					//			SetByte((m_BaseAddress + 0x1853ED + 1), 132);*/
					//		}

					//		SetByte((m_BaseAddress + 0x128102) + 1, nPaintFps);
					//		SetByte((m_BaseAddress + 0x119A17) + 1, nPaintFps);
					//		SetByte((m_BaseAddress + 0x1281FE) + 1, nPaintFps);
					//	}

					//

					//	//SetByte(0x528102, nPaintFps);
					//	//SetByte(0x519A17, nPaintFps);
					//	//SetByte(0x5281FE, nPaintFps);

						//if (nTimeUpdatePaint <= 0)
						//{
						//	nTimeUpdatePaint = 900;
						//	//SetWord(0x680490, nPaintFps);

						//	//SetByte(0x680490, nPaintFps);
						//	/*SetProcessWorkingSetSizeEx(hProcess,
						//		0xFFFFFFFF,
						//		0xFFFFFFFF,
						//		QUOTA_LIMITS_HARDWS_MIN_ENABLE | QUOTA_LIMITS_HARDWS_MAX_ENABLE
						//	);*/
						//	SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

						//}
						//else
						//	nTimeUpdatePaint--;

					//	//ofstream myfile3;
					//	//myfile3.open("C:\\data3.txt");
					//	//myfile3 << m_StateCount << "\n";
					//	////myfile2 << InitUiState << "\n";
					//	////myfile2 << ThreadCheckState1 << "\n";
					//	//myfile3.close();
					//	//KUiStateSkill::UpdateData();

					//	//SetProcessWorkingSetSize(GetCurrentProcess(), -1, -1);

					//	//if (!IsInGame)
					//	//{
					//	//	IsInGame = true;
					//	//	KUiStateSkill::OpenWindow();
					//	//	//KUiMissionTime::OpenWindow();
					//	//	//KUiAutoPlay::OpenWindow();
					//	//	//KWndAutoPlay::OpenWindow();
					//	//}
					//	//else
					//	//{
					//	//	if (InitStart)
					//	//	{
					//	//		//Player_AutoPlay::OpenWindow();
					//	//		if (m_MapId != gProtect.m_StateSkill)
					//	//		{
					//	//			KUiStateSkill::OpenWindow();
					//	//			//KUiMissionTime::OpenWindow();
					//	//			//KUiAutoPlay::OpenWindow();
					//	//			//KWndAutoPlay::OpenWindow();
					//	//			//KUiAutoPlay::PaintWindow(g_pRepresentShell);
					//	//		}
					//	//		else
					//	//			KUiStateSkill::CloseWindow(false);
					//	//	}
					//	//}
					//}
				//}
				//else
				//{
				//	Sleep(300);

				///*	if (*(int*)0x680490 < 30)
				//		SetWord(0x680490, 30);*/

				//}
			}

			Sleep(666);

			//myfile2.close();
			/*if(gPlayerSafe.m_IsGameSetOffline)
			{
				break;
			}*/
			/*else
			{
				for (int i = m_StateList; i < STATE_SKILL_MAX_COUNT; i++)
				{
					ms_ListStateSkill[i].nIndex = i;

					ms_ListStateSkill[i].nSkillID = 0;

					ms_ListStateSkill[i].nSkillTime = -1;

					memset(ms_ListStateSkill[i].m_TextTime, 0, 10);

					ms_ListStateSkill[i].m_LeftText = 42 + (28 * i);

				}

				Sleep(888);
			}*/
		
			//nCount++;
		}
		catch (...)
		{

		}
	}

	return 1;

}

int __stdcall NetConnectAgentInitialize(BYTE* This)
{
	DWORD pid = GetCurrentProcessId();

	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//int szNumBuffShow;

	KIniFile	Ini;

	BOOL result;

	result = b_sub_4207E0();

	//char IDBuffer[32];
	//char TextBuffer[128];
	//char szTipTemp[256];
	//char szTip[256];

	char* IDBuffer = (char*)malloc(32);
	char* TextBuffer = (char*)malloc(128);
	char* szTipTemp = (char*)malloc(256);
	char* szTip = (char*)malloc(256);

	int szTipLen;
	int szTipMaxtrix;
	int szTipLenCenter;
	int szSkillNameLen;
	float abc;

	//INIReader reader("SkillState.ini");

	//if (reader.ParseError() != 0)
		//exit(1); //MessageBox(0, "ParseError!", "Info", MB_OK | MB_ICONINFORMATION); //

	//ofstream myfile2;
	//myfile2.open("E:\\StartGame.txt");
	//myfile2 << hProcess << "\n";

	if (Ini.Load("\\Ui\\ui3\\SkillState.ini"))
	{

		Ini.GetInteger("BuffList", "BuffCount", 0, &szNumBuffShow);

		for (int i = 0; i < szNumBuffShow; ++i)
		{
			szTipLen = 0;
			szSkillNameLen = 0;
			szTipLenCenter = 0;
			szTipMaxtrix = 0;
			abc = 0;
			int SkillID = 0;

			memset(IDBuffer, 0, 32);

			sprintf(IDBuffer, "Buff_%d_ID", i);

			//SkillID = reader.GetInteger("BuffList", IDBuffer, 0);

			Ini.GetInteger("BuffList", IDBuffer, 0, &SkillID);
			ms_ListInfo[SkillID].nSkillID = SkillID;

			memset(TextBuffer, 0, 128);
			sprintf(TextBuffer, "Buff_%d_Name", i);

			Ini.GetString("BuffList", TextBuffer, "N/A", ms_ListInfo[SkillID].m_Name, 32);
			//strcpy(ms_ListInfo[SkillID].m_Name, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
			//myfile2 << ms_ListInfo[SkillID].m_Name << "\n";

			memset(TextBuffer, 0, 128);
			sprintf(TextBuffer, "Buff_%d_Image", i);

			Ini.GetString("BuffList", TextBuffer, "N/A", ms_ListInfo[SkillID].m_Image, 128);
			//strcpy(ms_ListInfo[SkillID].m_Image, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
			//myfile2 << ms_ListInfo[SkillID].m_Image << "\n";

			memset(TextBuffer, 0, 128);
			sprintf(TextBuffer, "Buff_%d_Desc", i);

			memset(szTip, 0, 256);
			memset(szTipTemp, 0, 256);

			Ini.GetString("BuffList", TextBuffer, "N/A", szTip, 256);
			//strcpy(szTip, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
			//myfile2 << ms_ListInfo[SkillID].m_Desc << "\n";

			szTipLen = strlen(szTip);
			szSkillNameLen = strlen(ms_ListInfo[SkillID].m_Name);
			//szTipLenCenter = (szTipLen / 2) - (szSkillNameLen + 1);
			if (szTipLen > szSkillNameLen)
			{
				if (float((szTipLen / 2) - (szSkillNameLen + (szSkillNameLen / 2))) > 1)
				{

					abc = float((szTipLen / 2) - (szSkillNameLen + (szSkillNameLen * 2) / 3));
				}
				else
					abc = 1;
			}

			szTipLenCenter = (int)(abc);


			//myfile2 << "szTipLen " << szTip << "\n"; // skill id
			//myfile2 << "szTipLen " << szTipLen << "\n"; // skill id
			//myfile2 << "szTipLen " << ms_ListInfo[SkillID].m_Name << "\n"; // skill id
			//myfile2 << "szSkillNameLen " << szSkillNameLen << "\n"; // skill id
			//myfile2 << "szTipLenCenter " << szTipLenCenter << "\n"; // skill id

			memset(&szTipTemp[0], *(const char*)"Ù", szTipLenCenter);
			strcpy(&szTipTemp[szTipLenCenter], ms_ListInfo[SkillID].m_Name);
			szTipTemp[szTipLenCenter + szSkillNameLen] = '\n';
			strcpy(&szTipTemp[szTipLenCenter + szSkillNameLen + 1], szTip);
			strcpy(ms_ListInfo[SkillID].m_Desc, szTipTemp);

			memset(TextBuffer, 0, 128);
			sprintf(TextBuffer, "Buff_%d_IsAura", i);

			Ini.GetInteger("BuffList", TextBuffer, 0, &ms_ListInfo[SkillID].m_IsAura);
			//ms_ListInfo[SkillID].m_IsAura = reader.GetInteger("BuffList", TextBuffer, 0);
			//myfile2 << ms_ListInfo[SkillID].m_IsAura << "\n";

			memset(TextBuffer, 0, 128);
			sprintf(TextBuffer, "Buff_%d_IsDebuff", i);

			Ini.GetInteger("BuffList", TextBuffer, 0, &ms_ListInfo[SkillID].m_IsDebuff);
			//ms_ListInfo[SkillID].m_IsDebuff = reader.GetInteger("BuffList", TextBuffer, 0);
			//myfile2 << ms_ListInfo[SkillID].m_IsDebuff << "\n";
		}
	}

	Ini.Clear();
	//myfile2.close();
	free(IDBuffer);
	free(TextBuffer);
	free(szTipTemp);
	free(szTip);


	for (int i = 0; i < STATE_SKILL_MAX_COUNT; i++)
	{

		ms_ListStateSkill[i].nIndex = i;

		ms_ListStateSkill[i].nSkillID = 0;

		ms_ListStateSkill[i].nSkillTime = -1;

		ms_ListStateSkill[i].m_LeftImage = 39 + (28 * i);

		ms_ListStateSkill[i].m_TopImage = 32;

		ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);

		ms_ListStateSkill[i].m_TopText = 56;

	}

	//ThreadCheckState1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckSkillState, 0, 0, NULL);

	//SetThreadPriority(ThreadHandles1, THREAD_PRIORITY_HIGHEST);

	//WaitForSingleObject(ThreadCheckState1, 333);

	return b_sub_528AE0(This);
}

BOOL StartGame()
{

	//DWORD pid = GetCurrentProcessId();

	//hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	//int szNumBuffShow;

	//KIniFile	Ini;

	BOOL result;

	KPlayer_Offset1 = *(unsigned int*)(0x8E48F4);

	result = b_sub_4207E0();

	//BYTE* window_ptr = (BYTE*)new KWndWindow;
	//unsigned int* image_ptr = (unsigned int*)new KWndImage;
	//unsigned int* button_ptr = (unsigned int*)new KWndButton;

	//void* window_ptr = (void*)0x63DB6C; // vtable 0x63DB6C;
	//gWndWindow = new KWndWindow;
	//window_ptr = gWndWindow;

	//void* image_ptr = (unsigned int*)0x63CF4C;
	//gWndImage = new KWndImage;
	//image_ptr = gWndImage;

	//void* button_ptr = (void*)0x63CBCC;
	//gWndButton = new KWndButton;
	//button_ptr = gWndButton;


	//(*((void**)(*(void**)window_ptr) + 8)) = gWndWindow->Install();
	//window_ptr = gWndWindow;
	//gWndWindow = dynamic_cast<KWndWindow*>(window_ptr);

	/*KWndWindow* base1 = new KWndImage;
	KWndImage* image = dynamic_cast<KWndImage*>(base1);
	image = gWndImage;

	KWndImage* base2 = new KWndButton;
	KWndButton* button = dynamic_cast<KWndButton*>(base2);
	button = gWndButton;*/

	//RefCountBaseClass(&gWndWindow, 340u, 1, (void(__thiscall*)(void*))RefWndWindow, (void(__thiscall*)(void*))RefWndBase);
	//RefCountBaseClass(&gWndImage, 440u, 1, (void(__thiscall*)(void*))RefWndImage, (void(__thiscall*)(void*))RefWndBase);
	//RefCountBaseClass(&gWndButton, 592u, 1, (void(__thiscall*)(void*))RefWndButton, (void(__thiscall*)(void*))RefWndBase);

	//char IDBuffer[32];
	//char TextBuffer[128];
	//char szTipTemp[256];
	//char szTip[256];
	//int szTipLen;
	//int szTipMaxtrix;
	//int szTipLenCenter;
	//int szSkillNameLen;
	//float abc;

	//IsInGame = false;

	////INIReader reader("SkillState.ini");

	////if (reader.ParseError() != 0)
	//	//exit(1); //MessageBox(0, "ParseError!", "Info", MB_OK | MB_ICONINFORMATION); //

	////ofstream myfile2;
	////myfile2.open("E:\\StartGame.txt");
	////myfile2 << hProcess << "\n";

	//if (Ini.Load("\\Ui\\ui3\\SkillState.ini"))
	//{

	//	Ini.GetInteger("BuffList", "BuffCount", 0, &szNumBuffShow);
	//	
	//	for (int i = 0; i < szNumBuffShow; ++i)
	//	{
	//		szTipLen = 0;
	//		szSkillNameLen = 0;
	//		szTipLenCenter = 0;
	//		szTipMaxtrix = 0;
	//		abc = 0;
	//		int SkillID = 0;

	//		memset(IDBuffer, 0, 32);

	//		sprintf(IDBuffer, "Buff_%d_ID", i);

	//		//SkillID = reader.GetInteger("BuffList", IDBuffer, 0);

	//		Ini.GetInteger("BuffList", IDBuffer, 0, &SkillID);

	//		if (SkillID == 0)
	//		{
	//			ms_ListInfo[SkillID].nSkillID = SkillID;

	//			continue;
	//		}

	//		ms_ListInfo[SkillID].nSkillID = SkillID;

	//		memset(TextBuffer, 0, 128);
	//		sprintf(TextBuffer, "Buff_%d_Name", i);

	//		Ini.GetString("BuffList", TextBuffer, "N/A", ms_ListInfo[SkillID].m_Name, 32);
	//		//strcpy(ms_ListInfo[SkillID].m_Name, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
	//		//myfile2 << ms_ListInfo[SkillID].m_Name << "\n";

	//		memset(TextBuffer, 0, 128);
	//		sprintf(TextBuffer, "Buff_%d_Image", i);

	//		Ini.GetString("BuffList", TextBuffer, "N/A", ms_ListInfo[SkillID].m_Image, 128);
	//		//strcpy(ms_ListInfo[SkillID].m_Image, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
	//		//myfile2 << ms_ListInfo[SkillID].m_Image << "\n";

	//		memset(TextBuffer, 0, 128);
	//		sprintf(TextBuffer, "Buff_%d_Desc", i);

	//		memset(szTip, 0, 256);
	//		memset(szTipTemp, 0, 256);

	//		Ini.GetString("BuffList", TextBuffer, "N/A", szTip, 256);
	//		//strcpy(szTip, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
	//		//myfile2 << ms_ListInfo[SkillID].m_Desc << "\n";

	//		szTipLen = strlen(szTip);
	//		szSkillNameLen = strlen(ms_ListInfo[SkillID].m_Name);
	//		//szTipLenCenter = (szTipLen / 2) - (szSkillNameLen + 1);
	//		if (szTipLen > szSkillNameLen)
	//		{
	//			if (float((szTipLen / 2) - (szSkillNameLen + (szSkillNameLen / 2))) > 1)
	//			{

	//				abc = float((szTipLen / 2) - (szSkillNameLen + (szSkillNameLen * 2) / 3));
	//			}
	//			else
	//				abc = 1;
	//		}

	//		szTipLenCenter = (int)(abc);


	//		//myfile2 << "szTipLen " << szTip << "\n"; // skill id
	//		//myfile2 << "szTipLen " << szTipLen << "\n"; // skill id
	//		//myfile2 << "szTipLen " << ms_ListInfo[SkillID].m_Name << "\n"; // skill id
	//		//myfile2 << "szSkillNameLen " << szSkillNameLen << "\n"; // skill id
	//		//myfile2 << "szTipLenCenter " << szTipLenCenter << "\n"; // skill id

	//		memset(&szTipTemp[0], *(const char*)"Ù", szTipLenCenter);
	//		strcpy(&szTipTemp[szTipLenCenter], ms_ListInfo[SkillID].m_Name);
	//		szTipTemp[szTipLenCenter + szSkillNameLen] = '\n';
	//		strcpy(&szTipTemp[szTipLenCenter + szSkillNameLen + 1], szTip);
	//		strcpy(ms_ListInfo[SkillID].m_Desc, szTipTemp);

	//		memset(TextBuffer, 0, 128);
	//		sprintf(TextBuffer, "Buff_%d_IsAura", i);

	//		Ini.GetInteger("BuffList", TextBuffer, 0, &ms_ListInfo[SkillID].m_IsAura);
	//		//ms_ListInfo[SkillID].m_IsAura = reader.GetInteger("BuffList", TextBuffer, 0);
	//		//myfile2 << ms_ListInfo[SkillID].m_IsAura << "\n";

	//		memset(TextBuffer, 0, 128);
	//		sprintf(TextBuffer, "Buff_%d_IsDebuff", i);

	//		Ini.GetInteger("BuffList", TextBuffer, 0, &ms_ListInfo[SkillID].m_IsDebuff);
	//		//ms_ListInfo[SkillID].m_IsDebuff = reader.GetInteger("BuffList", TextBuffer, 0);
	//		//myfile2 << ms_ListInfo[SkillID].m_IsDebuff << "\n";
	//	}
	//}

	//Ini.Clear();
	////myfile2.close();

	//for (int i = 0; i < STATE_SKILL_MAX_COUNT; i++)
	//{

	//	ms_ListStateSkill[i].nIndex = i;

	//	ms_ListStateSkill[i].nSkillID = 0;

	//	ms_ListStateSkill[i].nSkillTime = -1;

	//	ms_ListStateSkill[i].nFloatTime = -1;

	//	ms_ListStateSkill[i].m_LeftImage = 39 + (28 * i);

	//	ms_ListStateSkill[i].m_TopImage = 32;

	//	ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);

	//	ms_ListStateSkill[i].m_TopText = 56;

	//}

	//ThreadCheckState1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckSkillState, 0, 0, NULL);

	//SetThreadPriority(ThreadHandles1, THREAD_PRIORITY_HIGHEST);

	//WaitForSingleObject(ThreadCheckState1, 333);

	return result;

}

int InGame()
{
	//int number = 2;
	//int resultscript1 = b_ExecuteScriptFile_1((DWORD*)0x6DE314, "\\script\\fwcloud.lua", "process_protocol", "d", number);
	//int resultscript1 = b_GetScriptFile("\\script\\allowauto.lua");
	//int resultscript2 = b_ExecuteScriptFile("\\script\\allowauto.lua");

	//ofstream myfile;
	//myfile.open("E:\\SendScriptDataToServe.txt");
	//myfile << resultscript1 << "\n";
	////myfile << resultscript2 << "\n";
	//myfile.close();

	int result;
	//bool result;
	//char szAccount[68];
	////0x6DEC30
	//result = gGetLoginAccount((DWORD*)0x6DEC30, szAccount, 64);

	//memcpy(pszAccount, szAccount, 64);

	result = b_sub_420B90();
	//result = b_sub_5232C0();
	//_RegisterComClass("KUiStateSkill");
	//KUiStateSkill::OpenWindow();
	gUtil.InitUiState = true;
	gUtil.IsInGame = true;
	gProtect.m_IsInGame = true;
	gUtil.IsActivePlayerNpc = false;
	gUtil.IsActiveMissleSame = false;
	gUtil.IsActiveVisiblePlayer = false;
	
	if (GetThreadId(gUtil.ThreadCheckState1) <= 0)
	{
		gUtil.ThreadCheckState1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CheckSkillState, 0, 0, NULL);
		//SetThreadPriority(ThreadCheckState1, THREAD_PRIORITY_HIGHEST);
		//WaitForSingleObject(ThreadCheckState1, 3000);
	}

	//char* RoleName = (char*)malloc(32);
	//char* HardwareId = (char*)malloc(32);

	//unsigned int PlayerOffset1;
	//unsigned int PlayerOffset2;
	//unsigned int PlayerOffset3;
	//unsigned int PlayerOffset4;
	//unsigned int PlayerOffset5;

	//ReadProcessMemory(hProcess, (void*)(unsigned int)(KPlayer_Offset1 + 0x8C44), &PlayerOffset1, sizeof(PlayerOffset1), NULL);

	//PlayerOffset2 = (PlayerOffset1 * 0x113B8);

	//ReadProcessMemory(hProcess, (void*)(unsigned int)(0x12C5540), &PlayerOffset3, sizeof(PlayerOffset3), NULL);

	//PlayerOffset4 = PlayerOffset3 + PlayerOffset2;

	//PlayerOffset5 = PlayerOffset4 + 0x130D;

	//ReadProcessMemory(hProcess, (void*)(unsigned int)(PlayerOffset5), &RoleName, sizeof(RoleName), NULL);

	//RoleName_Offset1 = *(unsigned int*)(0x12C5540 + *(unsigned int*)(*(unsigned int*)(*(unsigned int*)(0x8E48F4) + 0x8C44) * 0x113B8)) + 0x130D;
	//strcpy(RoleName, (char*)(PlayerOffset5));

	//ofstream myfile;
	//myfile.open("E:\\Initialize.txt");
	//myfile << "PlayerOffset2 " << (DWORD*)PlayerOffset1 << "\n"; // skill id
	//myfile << "PlayerOffset2 " << (DWORD*)PlayerOffset2 << "\n"; // skill id
	//myfile << "PlayerOffset3 " << (DWORD*)PlayerOffset3 << "\n"; // skill id
	//myfile << "PlayerOffset4 " << (DWORD*)PlayerOffset4 << "\n"; // skill id
	//myfile << "PlayerOffset5 " << (DWORD*)PlayerOffset5 << "\n"; // skill id
	//myfile << "RoleName " << RoleName << "\n"; // skill id
	//myfile << "ClientSerial " << c_AccountName << "\n"; // skill id
	//myfile << "ClientSerial " << c_ClientSerial << "\n"; // skill id
	//myfile << "ClientSerial " << c_HardwareId << "\n"; // skill id
	//myfile.close();
	

	//char* RoleName = (char*)malloc(32);
	//RoleName_Offset1 = *(unsigned int*)(0x12C5540 + *(unsigned int*)(*(unsigned int*)(*(unsigned int*)(0x8E48F4) + 0x8C44) * 0x113B8)) + 0x130D;
	//strcpy(RoleName, (char*)(RoleName_Offset1));

	//ofstream myfile;
	//myfile.open("E:\\Initialize.txt");
	//	myfile << "RoleName " << RoleName << "\n"; // skill id
	//myfile.close();
	//free(RoleName);

	//KPlayer_Offset1 = *(unsigned int*)(0x8E48F4);
	// Offset01 = (KPlayer_Offset1 + 0x8C44) * 0x113B8
	// Offset02 = 0x12C5540 + Offset01
	// Offset03 = Offset02 + 0x130D
	// PlayerName = GetString Offset03
	// m_PlayerIndex = *(unsigned int*)(KPlayer_Offset1 + 0x8B30 + 0x114);
	// strcpy(ms_ListInfo[SkillID].m_Desc, szTipTemp);

	//memcpy(&HardwareId, GetHardwareIdMain(), sizeof(HardwareId));

	/*char* buffURL = (char*)malloc(256);
	sprintf(buffURL, "http://auth.antithailan.xyz/login-%s-%s-%s-%s.nnb", c_AccountName, RoleName, c_ClientSerial, c_HardwareId);
	std::string authurl = buffURL;
	std::string dataread = DownloadString(authurl);
	free(buffURL);
	free(RoleName);*/

	gUtil.SendLoginInfo();
	//free(HardwareId);

	/*KComClassInfo pComAutoPlay;
	strcpy(pComAutoPlay.szClassType, "Player_AutoPlay");
	pComAutoPlay.szClassType[32] = 0;
	pComAutoPlay.pfnCreateObject = Player_AutoPlay::CreateObject;
	_RegisterComClass(&pComAutoPlay);*/

	//KUiAutoPlayButton::RegisterSelfClass();
	//KUiAutoPlay::OpenWindow();
	//KAutoPlay::OpenWindow();
	//Player_AutoPlay::OpenWindow();
	//NewUiInit();
	//int number = 2;
	//int resultscript1 = b_ExecuteScriptFile_1((DWORD*)0x6DE314, "\\script\\allowauto.lua", "SendAuth", "d", number);
	//KWndAutoPlay::OpenWindow();
	return b_sub_420B90();;
}

void CloseAllWindowOutGame()
{
	//TerminateThread(ThreadCheckState1, 1);
	m_PlayerIndex = 0;
	_HookCallHideWindow();
	KUiStateSkill::CloseWindow(true);
	//InitUiState = false;
	IsInGame = false;
}

//KWndImage::KWndImage()
//{
//	//void* vptr_WndImage = (void*)(unsigned int*)0x63CF4C;
//	//this->~KWndWindow = reinterpret_cast<KUiStateSkill*>(&vptr_WndImage);
//	//CreateWndImage(m_pSelf);
//}

KUiStateSkill::KUiStateSkill()
{
	//void* vptr_WndWindow = (void*)(BYTE*)0x63DB6C;
	//void* vptr_WndImage = (void*)0x63CF4C;
	//KUiStateSkill* vptr;
	//*(void**)(BYTE*)this = reinterpret_cast<BYTE*>(&vptr_WndWindow);
	//this[0] = static_cast<KUiStateSkill>(*(KUiStateSkill*)&vptr_WndWindow);
	//vptr_WndWindow = static_cast<void*>(&vptr[0]);
	//vptr_WndImage = static_cast<void*>(&vptr[1]);
	m_IsShow = 0;
	m_nNumSkills = 0;
	m_nLiveSkills = 0;
}

//KUiStateSkill::~KUiStateSkill()
//{
//	//KWndWindow::~KWndWindow();
//}

KUiStateSkill* KUiStateSkill::OpenWindow()
{
	//void* vptr_WndImage = (void*)(unsigned int*)0x42C010;
	/*void* vptr_WndImage = (void*)0x63CF4C;
	KWndImage* WndImage;
	WndImage = (KWndImage*)vptr_WndImage;*/
	//void* vptr_WndImage = (DWORD (**)(...))(void*)0x63CF4C;
	
	if (m_pSelf == NULL)
	{
		//void* vptr_WndWindow = (void*)(unsigned char*)0x63DB6C;
		//void* vptr_WndImage = (void*)(unsigned int*)0x63CF4C;

		//m_pSelf = (KUiStateSkill*)0x6AE930;
		//m_pSelf->_vptr_WndImage = (int (**)(...))(void*)0x63CF4C;
		
		//m_pSelf = (KUiStateSkill*)operator new(0x6BF4u);
		//b_sub_42C010((unsigned int*)m_pSelf
		//b_sub_42C010((unsigned int*)&m_pSelf);
		//m_pSelf = (KUiStateSkill*)CreateWndImage(new KUiStateSkill);
		m_pSelf = new KUiStateSkill;
		//m_pSelf = reinterpret_cast<KUiStateSkill*>(&vptr_WndWindow);
		//m_pSelf = reinterpret_cast<KUiStateSkill*>(&vptr_WndImage);

		//vptr_WndWindow = reinterpret_cast<void**>(&vptr_WndImage);
		//vptr_WndImage = reinterpret_cast<void**>(&m_pSelf);
		
		CreateWndImage((DWORD*)m_pSelf);
		//HookVTableFunction(&vptr_WndWindow, &KWndWindow(), 0);
		//HookVTableFunction(&vptr_WndImage, &KWndImage(), 0);

		//CreateWndImage(m_pSelf = new KUiStateSkill);
		//CreateWndWindow(m_pSelf = new KUiStateSkill);
		//m_pSelf = (KUiStateSkill*)CreateWndImage(new KUiStateSkill);
		
		//m_pSelf = reinterpret_cast<KUiStateSkill*>((DWORD(**)(...))(void*)0x63CF4C);
		
		if (m_pSelf)
			m_pSelf->Initialize();
	}

	if (m_pSelf)
	{
		if (*(DWORD*)((unsigned int)m_pSelf + 4) != WND_S_VISIBLE)
			m_pSelf->Show();

		try
		{
			m_pSelf->UpdateData();
		}
		catch (...)
		{
			return m_pSelf;
		}
	}

	return m_pSelf;
}

void KUiStateSkill::Initialize()
{
	
	/*void** vptr_WndImage = (void**)0x63CF4C;
	*(void**)(unsigned int**)this = reinterpret_cast<unsigned int*>(&vptr_WndImage);*/
	//ofstream myfile;
	//myfile.open("E:\\Initialize.txt");
	//	myfile << "b_KLoad " << (unsigned int*)m_pSelf << "\n"; // skill id
	//	myfile << "b_KLoad " << (unsigned int*)(BYTE*)((unsigned int*)m_pSelf) << "\n"; // skill id
	//	myfile << "b_KLoad " << (unsigned int)(BYTE*)((unsigned int*)m_pSelf->_vptr_WndImage) << "\n"; // skill id
	//myfile.close();
	//CreateWndButton((unsigned int*)&m_ButtonAutoPlay);

	//KWndWindow* ButtonAutoPlay = gGetComClassFactory()->CreateComObject("Player_AutoPlay");

	//CreateWndImage((unsigned int*)&m_ButtonAutoPlay);

	//CreateWndWindow((unsigned int*)&m_AutoPlay);

	//CreateWndButton((unsigned int*)&m_AutoPlay);
	 
	//CreateWndButton((unsigned int*)&m_ButtonAutoPlay);

	//b_AddChild(&m_AutoPlay, &m_ButtonAutoPlay);
	// 
	//b_AddChild(this, &m_WndWindowAutoPlay);

	//b_AddChild(&m_WndWindowAutoPlay, &m_AutoPlay);

	//KIniFile	Ini;

	//if (Ini.Load("\\Ui\\ui3\\AutoPlay_icon.ini"))
	//{
	//	//KWndImage_Init(&m_ButtonAutoPlay, &Ini, "Main");
	//	//KWndButton_Init(&m_ButtonAutoPlay, &Ini, "Main");
	//	//m_ButtonAutoPlay.Init(&Ini, "Main");
	//	//m_AutoPlay.Init(&Ini, "Main");
	//	//m_AutoPlay.Init(&Ini, "btnOpen");
	//	//m_ButtonAutoPlay.Enable(1);
	//	//m_ButtonAutoPlay.BringToTop();
	//}

	//Ini.Clear();

	
	//m_AutoPlay.LeaveAlone();
	//b_Wnd_AddWindow(&m_WndWindowAutoPlay, 1);
	//b_Wnd_AddWindow(&m_ButtonAutoPlay, 1);
	//b_Wnd_AddWindow(&m_AutoPlay, 1);

	//b_AddChild(this, &m_AutoPlay);
	//KWndAutoPlay::OpenWindow();
	//KWndAutoPlay::OpenWindow();

	for (int i = 0; i < 15; ++i)
	{
		CreateWndImage((DWORD*)&m_ImageSkill[i]);
		b_AddChild(this, &m_ImageSkill[i]);

		SetObjectWndText32((unsigned int*)&m_Text32Skill[i]);
		b_AddChild(this, &m_Text32Skill[i]);
	}
	
	char Scheme[256];
	b_sub_41A4A0((const char*)0x68D090, Scheme, 256);
	this->LoadScheme(Scheme);
	//(*(DWORD*)this + 4) &= ~WND_S_VISIBLE;
	b_Wnd_AddWindow(this, 1);
}

void KUiStateSkill::LoadScheme(const char* pScheme)
{
	int	nValue;
	char		Buff[128];
	KIniFile	Ini;

	if (Ini.Load("\\Ui\\ui3\\SkillState.ini"))
	{
		m_pSelf->LoadScheme(&Ini);
	}
}

void KUiStateSkill::LoadScheme(KIniFile* pIni)
{
	int	nValue;
	char		Buff[128];
	char		TextBuffer[128];
	char		szTip[128];
	char		TipColor[260];
	pIni->GetInteger("Main", "IsOpen", 0, &this->m_IsShow);

	KWndImage_Init(this, pIni, "Main");

	for (int i = 0; i < 15; ++i)
	{

		KWndImage_Init(&m_ImageSkill[i], pIni, "BuffImage");

		_SetPosition(&m_ImageSkill[i], 28 * i, 0);
		
		KWndText32_Init(&m_Text32Skill[i], pIni, "txtBuffTime");

		_SetPosition(&m_Text32Skill[i], 28 * i, 24);

		//int SkillID = 0;
		
		//memset(IDBuffer, 0, 32);

		//sprintf(IDBuffer, "Buff_%d_ID", i);

		//SkillID = reader.GetInteger("BuffList", IDBuffer, 0);

		//ms_ListInfo[SkillID].nSkillID = SkillID;

		//sprintf(TextBuffer, "Buff_%d_Name", i);

		//strcpy(ms_ListInfo[SkillID].m_Name, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
		////myfile2 << ms_ListInfo[SkillID].m_Name << "\n";

		//memset(TextBuffer, 0, 128);

		/*sprintf(Buff, "Buff_%d_Image", i);
		pIni->GetString("BuffList", Buff, "N/A", TextBuffer, 128);
		_SetImage(&m_ImageSkill[i], 1, TextBuffer, 0, 0);*/

		b_HideWndWindow(&m_ImageSkill[i]);
		b_HideWndWindow(&m_Text32Skill[i]);

		//pIni->GetString("BuffImage", "TipColor", "255,255,255", TipColor, 16);
		//*(int*)(void*)((&m_ImageSkill[i]) + 252) = _GetColorTip(TipColor);

		//memset(Buff, 0, 128);

		/*sprintf(Buff, "Buff_%d_Desc", i);
		pIni->GetString("BuffList", Buff, "N/A", szTip, 128);
		strcpy((char*)(&m_ImageSkill[i]) + 120, szTip);
		*(int*)(void*)((&m_ImageSkill[i]) + 248) = strlen(szTip);*/
		//_SetMouseHoverInfo((void*)0x68E0E8, &m_ImageSkill[i], 0, 39 + (28 * i), 36, false, true);
		//nLenTitle = _TEncodeText(szTip, strlen(szTip), 0);
		//szTip[nLenTitle] = 0;
		//_SetMouseHoverTitle((void*)0x68E0E8, szTip, nLenTitle, -1, 0);
		//////_SetMouseHoverUpdate((void*)0x68E0E8, 39 + (28 * i), 36);
		//_SetMouseHoverUpdate((void*)0x68E0E8, 0x68F328, 0x68F32C);
		//strcpy(ms_ListInfo[SkillID].m_Image, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
		////myfile2 << ms_ListInfo[SkillID].m_Image << "\n";

		//pIni->GetString("BuffList", (const char*)0x63CC18, "N/A", (char*)(this + 120), 128);

		/*char szTip[128];
		strcpy(szTip, "T¨ng kü n¨ng");
		int nTipLen = _TEncodeText(szTip, strlen(szTip), 0);
		szTip[nTipLen] = 0;
		_SetMouseHoverInfo((void*)0x68E0E8, this, 0, 39 + (28 * i), 36, false, true);
		_SetMouseHoverTitle((void*)0x68E0E8, szTip, nTipLen, -1, 0);
		_SetMouseHoverUpdate((void*)0x68E0E8, 0x68F328, 0x68F32C);
		*(DWORD*)0x68F344 = 1;*/

		//memset(TextBuffer, 0, 128);
		//sprintf(TextBuffer, "Buff_%d_Desc", i);

		//strcpy(ms_ListInfo[SkillID].m_Desc, reader.GetString("BuffList", TextBuffer, "N/A").c_str());
		////myfile2 << ms_ListInfo[SkillID].m_Desc << "\n";

		//memset(TextBuffer, 0, 128);
		//sprintf(TextBuffer, "Buff_%d_IsAura", i);

		//ms_ListInfo[SkillID].m_IsAura = reader.GetInteger("BuffList", TextBuffer, 0);
		////myfile2 << ms_ListInfo[SkillID].m_IsAura << "\n";

		//memset(TextBuffer, 0, 128);
		//sprintf(TextBuffer, "Buff_%d_IsDebuff", i);

		//ms_ListInfo[SkillID].m_IsDebuff = reader.GetInteger("BuffList", TextBuffer, 0);
		////myfile2 << ms_ListInfo[SkillID].m_IsDebuff << "\n";
	}
	pIni->Clear();
}

void KUiStateSkill::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			if (*(DWORD*)((unsigned int)m_pSelf + 4) == WND_S_VISIBLE)
				m_pSelf->Hide();

			for (int i = 0; i < 15; ++i)
			{
				//m_pSelf->m_ImageSkill[i].Destroy();
			}

			//m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
		{
			if (*(DWORD*)((unsigned int)m_pSelf + 4) == WND_S_VISIBLE)
				m_pSelf->Hide();
		}
		//m_pSelf->Hide();
	}
}

//void KUiStateSkill::Destroy()
//{
//	if (m_pSelf)
//	{
//		b_HideWndWindow(this);
//		*(unsigned int*)(this + 4) |= 0x2000000;
//	}
//}

void KUiStateSkill::Hide()
{
	if (m_pSelf)
	{
		//if (*(DWORD*)((unsigned int)m_pSelf + 4) == WND_S_VISIBLE)
			//KWndWindow::Hide();
		//b_HideWndWindow(this);
		//b_HideWndWindow(this);
	}
}

void KUiStateSkill::Show()
{
	if (m_pSelf)
	{
		//if (*(DWORD*)((unsigned int)m_pSelf + 4) != WND_S_VISIBLE)
			//KWndWindow::Show();
		b_ShowWndWindow(this);
		//b_ShowWndWindow(this);
	}
}

void KUiStateSkill::PaintWindow()
{
	//KWndWindow test;
	if (g_pRepresentShell == NULL)
		return;
	_PaintWindow(this);
	//KRUImage	Image;
	//	Image.nType = ISI_T_SPR;
	//	Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	//	Image.uImage = 0;
	//	Image.nISPosition = -1;
	//	strcpy(Image.szImage, "\\spr\\Ui\\×´Ì¬Í¼±ê\\ÂÞººÕó.spr");
	//	//Image.szImage[128] = 0;
	//	//myfile2 << ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Image << "\n";
	//	//myfile2 << Image.szImage << "\n";
	//	Image.Color.Color_b.a = 255;
	//	Image.bRenderFlag = 0;
	//	Image.nFrame = 0;
	//	Image.oPosition.nX = 39;
	//	Image.oPosition.nY = 28;
	//	Image.oPosition.nZ = 0;
	//	Image.nNew1 = 0;

	//	g_pRepresentShell->DrawPrimitives(1, &Image, RU_T_IMAGE, TRUE);
}

void __fastcall KUiStateSkill::UpdateData()
{
	

	int StateCount = 0;

	/*unsigned int KPlayer_Offset1 = NULL;
	unsigned int PlayerIndex_Offset1 = NULL;
	unsigned int nPlayerIndex_Offset1 = NULL;

	unsigned int GameData_Offset1 = NULL;
	unsigned int GameData_Skill_Offset1 = NULL;*/

	//ReadProcessMemory(hProcess, (void*)(unsigned int)0x8E48F4, &AddressOffset1, sizeof(AddressOffset1), NULL);

	//GameData_Offset1 = *(unsigned int*)(GameData);

	//StateCount = *(unsigned int*)(GameData_Skill_Offset1 + 4);

	//Test_AddressOffset3 = (Test_AddressOffset2 + 0x114);

	//Test_AddressOffset4 = ((unsigned int)GameData + 0x113B8 * m_PlayerIndex + 0x100);

	//ReadProcessMemory(hProcess, (void*)(unsigned int)GameData, &SkillInfoOffset1, sizeof(SkillInfoOffset1), NULL);

	//GameData_Skill_Offset3 = (unsigned int)((unsigned int*)GameData_Skill_Offset2 + 8);

	//ofstream myfile1;
	//		myfile1.open("C:\\data1.txt");
	//		myfile1 << m_PlayerIndex << "\n";
	//		myfile1 << IsInGame << "\n";
	//		myfile1 << m_pSelf << "\n";
	//		myfile1 << InitUiState << "\n";
	//		myfile1 << m_StateList << "\n";
	//		//myfile1 << (DWORD*)((unsigned int)m_pSelf + 4) << "\n";
	//		myfile1 << *(DWORD*)((unsigned int)m_pSelf + 4) << "\n";

	//		//myfile1 << KPlayer_Offset1 << "\n";
	//		//myfile1 << nPlayerIndex_Offset1 << "\n";

	//		myfile1 << GameData_Offset1 << "\n";
	//		myfile1 << GameData_Skill_Offset1 << "\n";
	//		myfile1 << GameData_Skill_Offset2 << "\n";
	//		//myfile1 << GameData_Skill_Offset3 << "\n";
	//		//myfile1 << StateCount << "\n";
	//		//myfile1 << 0x80000000 << "\n";
	//		//myfile1 << ((unsigned int)m_pSelf + 4 == WND_S_VISIBLE) << "\n";
	//		//myfile1 << ((unsigned int)m_pSelf + 4 < WND_S_VISIBLE) << "\n";
	//		myfile1.close();

	try
	{
		//int i = 0;
		//std::map<int, SkillStateLive>::iterator it;
		//for (it = StateSkill_Data.begin(); it != StateSkill_Data.end(); ++it) {

		//	myfile << "Index " << i << "\n"; // skill id
		//	myfile << "SkillID " << it->second.nSkillID << "\n"; // skill id
		//	myfile << "FloatTime " << it->second.nFloatTime << "\n"; // skill id
		//	myfile << "SkillTime " << it->second.nSkillTime << "\n"; // skill id

		//	if ((GetTickCount() - it->second.m_TickTime) < it->second.nSkillTime && i < 15 && ms_ListInfo[it->second.nSkillID].nSkillID == it->second.nSkillID)
		//	{
		//		_SetImage(&m_pSelf->m_ImageSkill[i], 1, ms_ListInfo[it->second.nSkillID].m_Image, 0, 0);
		//		strcpy((char*)(&m_pSelf->m_ImageSkill[i]) + 120, ms_ListInfo[it->second.nSkillID].m_Desc);
		//		*((BYTE*)(&m_pSelf->m_ImageSkill[i]) + 248) = strlen(ms_ListInfo[it->second.nSkillID].m_Desc);
		//		_SetText(&m_pSelf->m_Text32Skill[i], it->second.m_TextTime, -1);
		//		b_ShowWndWindow(&m_pSelf->m_ImageSkill[i]);
		//		b_ShowWndWindow(&m_pSelf->m_Text32Skill[i]);
		//	}
		//	else
		//	{
		//		_SetImage(&m_pSelf->m_ImageSkill[i], 1, "", 0, 0);
		//		b_HideWndWindow(&m_pSelf->m_ImageSkill[i]);
		//		_SetText(&m_pSelf->m_Text32Skill[i], "", -1);
		//		b_HideWndWindow(&m_pSelf->m_Text32Skill[i]);
		//		StateSkill_Data.erase(it);
		//	}
		//	i++;
		//}
		//ofstream myfile3;
		//myfile3.open("C:\\data4.txt");
		//myfile3 << m_StateCount << "\n";
		////myfile2 << InitUiState << "\n";
		////myfile2 << ThreadCheckState1 << "\n";
		//myfile3.close();
		/*AddressOffset1 = NULL;
		AddressOffset2 = NULL;
		AddressOffset3 = NULL;
		SkillInfoOffset2 = NULL;
		SkillInfoOffset3 = NULL;
		SkillInfoOffset4 = NULL;*/

		//int nCount = 0;



		//myfile2 << hProcess << "\n";
		//myfile2 << (DWORD*)AddressOffset1 << "\n";
		//myfile2 << (DWORD*)AddressOffset3 << "\n";
		//myfile2 << "PlayerIndex: " << (DWORD)m_PlayerIndex << "\n";
		//myfile2 << *(int*)(DWORD*)AddressOffset3 << "\n";

		//ofstream myfile1;
		//myfile1.open("C:\\data1.txt");
		//myfile1 << m_PlayerIndex << "\n";
		//myfile1 << m_pSelf << "\n";
		//myfile1 << InitUiState << "\n";
		//myfile1 << (unsigned int)m_pSelf + 4 << "\n";
		////myfile1 << *(unsigned int*)(&m_pSelf[1]) << "\n";
		////myfile1 << WND_S_VISIBLE << "\n";
		//myfile1 << ((unsigned int)m_pSelf + 4 == WND_S_VISIBLE) << "\n";
		//myfile1 << ((unsigned int)m_pSelf + 4 < WND_S_VISIBLE) << "\n";
		//myfile1.close();

		/*if((unsigned int)m_pSelf + 4 < WND_S_VISIBLE)
			m_pSelf->Show();*/

		if (*(DWORD*)((unsigned int)m_pSelf + 4) == WND_S_VISIBLE)
		{
			if (gUtil.m_PlayerIndex > 0)
			{
				int nCountData = 0;

				m_StateCount = 0;

				SkillInfoOffset2 = (unsigned int)(SkillInfoOffset1 + 0x113B8 * gUtil.m_PlayerIndex);

				SkillInfoOffset3 = SkillInfoOffset2 + 0x100;

				//ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset3 + 4), &m_StateList, sizeof(m_StateList), NULL);

				GameData_Offset1 = *(unsigned int*)(GameData);

				GameData_Skill_Offset1 = (unsigned int)(GameData_Offset1 + 0x113B8 * gUtil.m_PlayerIndex + 0x100);

				m_StateList = *(unsigned int*)(GameData_Skill_Offset1 + 4);

				//ofstream myfile2;
				//myfile2.open("C:\\data2.txt");
				//myfile2 << m_StateList << "\n";
				////myfile2 << InitUiState << "\n";
				////myfile2 << ThreadCheckState1 << "\n";
				//myfile2.close();

				for (int i = 0; i < STATE_SKILL_MAX_COUNT; ++i)
				{
					ms_ListStateSkill[i].nIndex = i;

					ms_ListStateSkill[i].nSkillID = 0;

					ms_ListStateSkill[i].nSkillLevel = 0;

					ms_ListStateSkill[i].nFloatTime = 0;

					ms_ListStateSkill[i].nSkillTime = 0;

					memset(ms_ListStateSkill[i].m_TextTime, 0, 10);

					ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
				}

				if (m_StateList > 0)
				{
					int SkillID = 0;
					int SkillLevel = 0;
					int SkillTime = 0;

					ReadProcessMemory(hProcess, (void*)SkillInfoOffset3, &SkillInfoOffset4, sizeof(SkillInfoOffset4), NULL);

					for (int i = 0; i < STATE_SKILL_MAX_COUNT; i++)
					{
						SkillID = 0;
						SkillLevel = 0;
						SkillTime = 0;
						//Sleep(30);
					boqua:
						int skilltime_value = NULL;
						bool IsSkillExits = false;

						if (gUtil.m_PlayerIndex <= 0)
							break;

						ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 16), &SkillID, sizeof(SkillID), NULL);

						if (gUtil.m_PlayerIndex <= 0)
							break;

						ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 20), &SkillLevel, sizeof(SkillLevel), NULL);

						if (gUtil.m_PlayerIndex <= 0)
							break;

						ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 24), &SkillTime, sizeof(SkillTime), NULL);

						if (gUtil.m_PlayerIndex <= 0)
							break;

						ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 8), &SkillInfoOffset4, sizeof(SkillInfoOffset4), NULL);

						/*
						for (int h = 0; h < STATE_SKILL_MAX_COUNT; h++)
						{
							if (ms_ListStateSkill[h].nSkillID == SkillID)
							{
								IsSkillExits = true;
								break;
							}
						}

						if (IsSkillExits)
						{
							ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 8), &SkillInfoOffset4, sizeof(SkillInfoOffset4), NULL);
							continue;
						}*/

						if (SkillTime > 0 && i < m_StateList)
						{

							//char	textTime[10];
							//memset(textTime, 0, 10);

							skilltime_value = static_cast<int>((SkillTime * 60 / 1000) + 0.5);
							//myfile2 << "skilltime_value: " << skilltime_value << "\n";
							//if (skilltime_value / 3600 > 8 || SkillID >= 661 && SkillID <= 667)



							for (int k = 0; k < szNumBuffShow; ++k)
							{
								if (ms_ListSkill[k] == SkillID)
								{
									ms_ListStateSkill[i].nIndex = i;

									ms_ListStateSkill[i].nSkillID = SkillID;

									ms_ListStateSkill[i].nSkillLevel = SkillLevel;

									ms_ListStateSkill[i].nFloatTime = SkillTime;

									ms_ListStateSkill[i].nSkillTime = (int)skilltime_value;

									break;
								}
							}

							/*if (ms_ListStateSkill[i].nSkillID <= 0)
							{
								continue;
							}*/

							/*if (ms_ListStateSkill[i].nSkillID >= 661 && ms_ListStateSkill[i].nSkillID <= 667)
							{
								continue;
							}*/

							char* textTime = (char*)malloc(10);

							if ((int)SkillTime >= 1555200)
							{
								int daySkillTime = floor((float)(SkillTime / 1555200));
								if (daySkillTime > 0)
								{
									sprintf(textTime, "%dd", daySkillTime);
									if ((int)daySkillTime < 10)
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
									else if ((int)daySkillTime >= 100)
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 3;
									else
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
								}
							}
							else
							{
								if ((int)SkillTime <= 18)
								{

									if (ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_IsAura == 1)
									{
										strcpy(textTime, "N/A");
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
									}
									else
									{
										strcpy(textTime, "N/A");
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
										/*if (skilltime_value > 0)
										{
											sprintf(textTime, "%ds", skilltime_value);
											if ((int)skilltime_value < 10)
												ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
											else
												ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
										}*/
									}
								}
								else if ((int)SkillTime > 1080 && (int)SkillTime < 64800)
								{
									sprintf(textTime, "%dm", (int)floor((float)((int)SkillTime / 1080)));
									if (((int)SkillTime / 1080) < 10)
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
									else
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
								}
								else if ((int)SkillTime >= 64800 && (int)SkillTime < 1555200)
								{
									sprintf(textTime, "%dh", (int)floor((float)((int)SkillTime / 64800)));
									if (((int)SkillTime / 64800) < 10)
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 1;
									else
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
								}
								else
								{
									sprintf(textTime, "%ds", skilltime_value);
									if ((int)skilltime_value < 10)
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
									else
										ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
									/*if ((int)skilltime_value > 0)
									{
										sprintf(textTime, "%ds", skilltime_value);
											if ((int)skilltime_value < 10)
												ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
											else
												ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
									}*/
								}
							}



							/*	else if (skilltime_value >= 86400)
					{
						int daySkillTime = floor(floor((float)(SkillTime / 86400)) / 18);
						if (daySkillTime > 0)
						{
							sprintf(textTime, "%dd", daySkillTime);
							if ((int)daySkillTime < 10)
								ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);
							else if ((int)daySkillTime >= 100)
								ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 3;
							else
								ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
						}
					}*/
					/*else if ((int)skilltime_value >= 86400)
					{
						sprintf(textTime, "%dDay", floor((float)floor((float)((int)skilltime_value / 86400)) / 18));
						if (((int)skilltime_value / 86400) < 10)
							ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 1;
						else
							ms_ListStateSkill[i].m_LeftText = 44 + (28 * i) - 2;
					}*/

							if (strlen(textTime) > 0)
							{
								memcpy(&ms_ListStateSkill[i].m_TextTime, textTime, sizeof(ms_ListStateSkill[i].m_TextTime));

								ms_ListStateSkill[i].m_TextTime[sizeof(ms_ListStateSkill[i].m_TextTime) - 1] = 0;

								m_StateCount++;
							}
							else
							{
								ms_ListStateSkill[i].nSkillID = 0;
								//i--;
							}
							free(textTime);
							//int nWidth, nHeight;
							//char szTipTemp[256];
							//memset(szTipTemp, 0, 256);
							//_SetImage(&m_pSelf->m_ImageSkill[i - nCountData], 1, ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Image, 0, 0);
							//KWndWindow_GetSize(&m_pSelf->m_ImageSkill[i - nCountData], &nWidth, &nHeight);

							//if (nWidth > 0 && nHeight > 0)
							//{
							//	sprintf(szTipTemp, "%s%d\nÙ", ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Desc, ms_ListStateSkill[i].nSkillLevel);
							//	//strcat(ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Desc, textLevel);
							//	strcpy((char*)(&m_pSelf->m_ImageSkill[i - nCountData]) + 120, szTipTemp);
							//	*((BYTE*)(&m_pSelf->m_ImageSkill[i - nCountData]) + 248) = TEncodeText((char*)(&m_pSelf->m_ImageSkill[i - nCountData]) + 120, strlen(szTipTemp));
							//	//*((BYTE*)(&m_pSelf->m_ImageSkill[i]) + 248) = strlen(ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Desc);
							//	_SetText(&m_pSelf->m_Text32Skill[i - nCountData], ms_ListStateSkill[i].m_TextTime, -1);
							//	b_ShowWndWindow(&m_pSelf->m_ImageSkill[i - nCountData]);
							//	b_ShowWndWindow(&m_pSelf->m_Text32Skill[i - nCountData]);
							//}
							//else
							//{
							//	//_SetImage(&m_pSelf->m_ImageSkill[i], 1, "", 0, 0);
							//	b_HideWndWindow(&m_pSelf->m_ImageSkill[i]);
							//	//m_pSelf->m_ImageSkill[i].Hide()
							//	//_SetText(&m_pSelf->m_Text32Skill[i], "", -1);
							//	b_HideWndWindow(&m_pSelf->m_Text32Skill[i]);
							//	nCountData++;
							//}

						}
						else
						{
							ms_ListStateSkill[i].nIndex = i;

							ms_ListStateSkill[i].nSkillID = 0;

							ms_ListStateSkill[i].nSkillLevel = 0;

							ms_ListStateSkill[i].nFloatTime = 0;

							ms_ListStateSkill[i].nSkillTime = 0;

							memset(ms_ListStateSkill[i].m_TextTime, 0, 10);

							ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);

							////_SetImage(&m_pSelf->m_ImageSkill[i], 1, "", 0, 0);
							//b_HideWndWindow(&m_pSelf->m_ImageSkill[i]);
							////m_pSelf->m_ImageSkill[i].Hide()
							////_SetText(&m_pSelf->m_Text32Skill[i], "", -1);
							//b_HideWndWindow(&m_pSelf->m_Text32Skill[i]);
							//nCountData++;

						}

						/*if (IsSkillExits)
						{
							//i--;
						}*/

						//ReadProcessMemory(hProcess, (void*)(unsigned int)(SkillInfoOffset4 + 8), &SkillInfoOffset4, sizeof(SkillInfoOffset4), NULL);

						//Sleep(10);

					}



				}

				for (int i = 0; i < STATE_SKILL_MAX_COUNT; i++)
				{
					//bool StateShow = false;
					//Sleep(30);
					/*if (((unsigned int)&m_pSelf[1] & WND_S_VISIBLE) != 0)
					{
						ofstream myfile10;
						myfile10.open("C:\\data10.txt");
						myfile10 << m_PlayerIndex << "\n";
						myfile10 << WND_S_VISIBLE << "\n";
						myfile10.close();
					}*/

					// m_StateCount
					//myfile << "Index " << i << "\n"; // skill id
					//myfile << "SkillID " << ms_ListStateSkill[i].nSkillID << "\n"; // skill id
					//myfile << "FloatTime " << ms_ListStateSkill[i].nFloatTime << "\n"; // skill id
					//myfile << "SkillTime " << ms_ListStateSkill[i].nSkillTime << "\n"; // skill id
					if (ms_ListStateSkill[i].nFloatTime > 0 && (i - nCountData) < m_StateCount && ms_ListStateSkill[i].nSkillID > 0)
					{
						int nWidth, nHeight;
						//char	textLevel[10];
						//char szTipTemp[256];
						char* szTipTemp = (char*)malloc(256);
						//memset(szTipTemp, 0, 256);
						_SetImage(&m_pSelf->m_ImageSkill[i - nCountData], 1, ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Image, 0, 0);
						KWndWindow_GetSize(&m_pSelf->m_ImageSkill[i - nCountData], &nWidth, &nHeight);

						if (nWidth > 0 && nHeight > 0)
						{
							sprintf(szTipTemp, "%s%d\nÙ", ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Desc, ms_ListStateSkill[i].nSkillLevel);
							//szTipTemp[sizeof(szTipTemp) - 1] = 0;
							//strcat(ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Desc, textLevel);
							strcpy((char*)(&m_pSelf->m_ImageSkill[i - nCountData]) + 120, szTipTemp);
							*((BYTE*)(&m_pSelf->m_ImageSkill[i - nCountData]) + 248) = TEncodeText((char*)(&m_pSelf->m_ImageSkill[i - nCountData]) + 120, strlen(szTipTemp));
							//*((BYTE*)(&m_pSelf->m_ImageSkill[i]) + 248) = strlen(ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Desc);
							_SetText(&m_pSelf->m_Text32Skill[i - nCountData], ms_ListStateSkill[i].m_TextTime, -1);
							b_ShowWndWindow(&m_pSelf->m_ImageSkill[i - nCountData]);
							b_ShowWndWindow(&m_pSelf->m_Text32Skill[i - nCountData]);
							//StateShow = true;
						}
						else
						{
							//_SetImage(&m_pSelf->m_ImageSkill[i], 1, "", 0, 0);
							b_HideWndWindow(&m_pSelf->m_ImageSkill[i]);
							//m_pSelf->m_ImageSkill[i].Hide()
							//_SetText(&m_pSelf->m_Text32Skill[i], "", -1);
							b_HideWndWindow(&m_pSelf->m_Text32Skill[i]);
							nCountData++;
							//StateShow = false;
						}

						free(szTipTemp);

						/*KRUImage	Image;
						Image.nType = ISI_T_SPR;
						Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
						Image.uImage = 0;
						Image.nISPosition = -1;
						strcpy(Image.szImage, ms_ListInfo[ms_ListStateSkill[i].nSkillID].m_Image);
						Image.Color.Color_b.a = -1;
						Image.bRenderFlag = 0;
						Image.nFrame = 0;
						Image.oPosition.nX = ms_ListStateSkill[i].m_LeftImage;
						Image.oPosition.nY = ms_ListStateSkill[i].m_TopImage;
						Image.oPosition.nZ = 0;
						Image.nNew1 = 0;

						g_pRepresentShell->DrawPrimitives(1, &Image, RU_T_IMAGE, TRUE);

						g_pRepresentShell->OutputText(12, ms_ListStateSkill[i].m_TextTime, -1, ms_ListStateSkill[i].m_LeftText, ms_ListStateSkill[i].m_TopText, 0x00EE00, 0);*/

					}
					else
					{
						//_SetImage(&m_pSelf->m_ImageSkill[i], 1, "", 0, 0);
						b_HideWndWindow(&m_pSelf->m_ImageSkill[i]);
						//m_pSelf->m_ImageSkill[i].Hide()
						//_SetText(&m_pSelf->m_Text32Skill[i], "", -1);
						b_HideWndWindow(&m_pSelf->m_Text32Skill[i]);
						nCountData++;
						/*ms_ListStateSkill[i].nIndex = i;

						ms_ListStateSkill[i].nSkillID = 0;

						ms_ListStateSkill[i].nFloatTime = -1;

						ms_ListStateSkill[i].nSkillTime = -1;

						memset(ms_ListStateSkill[i].m_TextTime, 0, 10);

						ms_ListStateSkill[i].m_LeftText = 44 + (28 * i);*/

					}
					//nCount++;

					/*if (!(memcmp(ms_ListStateSkill[i].m_TextTime, "0s", 2) != 0) || !(memcmp(ms_ListStateSkill[i].m_TextTime, "0m", 2) != 0) || !(memcmp(ms_ListStateSkill[i].m_TextTime, "0h", 2) != 0) || (memcmp(ms_ListStateSkill[i].m_TextTime, "0d", 2) != 0))
					{
						b_HideWndWindow(&m_pSelf->m_ImageSkill[i]);
						b_HideWndWindow(&m_pSelf->m_Text32Skill[i]);
					}*/
					/*if (!StateShow)
					{
						b_HideWndWindow(&m_pSelf->m_ImageSkill[i]);
						m_pSelf->m_ImageSkill[i].Hide();
						b_HideWndWindow(&m_pSelf->m_Text32Skill[i]);
					}*/
				}
			}
		}

		//ofstream myfile4;
		//myfile4.open("C:\\data5.txt");
		//myfile4 << nCount << "\n";
		////myfile2 << InitUiState << "\n";
		////myfile2 << ThreadCheckState1 << "\n";
		//myfile4.close();

	}
	catch (...)
	{
	}
	
	//myfile.close();
}

//void KUiStateSkill::Breathe()
//{
//	if (m_Sign.IsVisible())
//		m_Sign.NextFrame();
//}

int KUiStateSkill::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	/*if (IsDisable())
		return nRet;*/
	*(BYTE*)(void*)(unsigned int*)(this) = (int)this;

	//ofstream myfile;
	//myfile.open("E:\\OnClickButton.txt");
	//myfile << "uMsg " << uMsg << "\n"; // skill id
	//myfile << "KWndWindow " << uParam << "\n"; // skill id
	//myfile.close();

	switch (uMsg)
	{
	//case WND_N_BUTTON_CLICK:
	//	OnClickButton((KWndWindow*)uParam);
	//	break;
	//case 0x507:
	//	/*if (_IsMoseHoverWndObj((void*)0x68E0E8, this, 0))
	//		_CancelMouseHoverInfo((void*)0x68E0E8);*/
	//	nRet = 1;
	//	break;
	//case WM_MOUSEHOVER:
	//	/*if (!_IsMoseHoverWndObj((void*)0x68E0E8, this, 0))
	//	{
	//		char szTip[128];
	//		strcpy(szTip, "T¨ng kü n¨ng");
	//		int nTipLen = _TEncodeText(szTip, strlen(szTip), 0);
	//		szTip[nTipLen] = 0;
	//		_SetMouseHoverInfo((void*)0x68E0E8, this, 0, LOWORD(nParam), HIWORD(nParam), false, true);
	//		_SetMouseHoverTitle((void*)0x68E0E8, szTip, nTipLen, -1, 0);
	//		_SetMouseHoverUpdate((void*)0x68E0E8, 0x68F328, 0x68F32C);
	//		*(int*)0x68F344 = 1;
	//		nRet = 1;
	//	}*/
	//	nRet = 1;
	//	break;
	default:
		nRet = KWndWindow_WndProc(this, uMsg, uParam, nParam);
	}
	return nRet;
}

//void KUiStateSkill::OnClickButton(KWndWindow* pWnd)
//{
//	ofstream myfile;
//	myfile.open("E:\\OnClickButton.txt");
//	myfile << "KWndWindow " << pWnd << "\n"; // skill id
//	myfile.close();
//
//	/*if (pWnd == (KWndWindow*)&m_ScrollUpBtn)
//		OnScrollList(true);
//	else if (pWnd == (KWndWindow*)&m_ScrollDownBtn)
//		OnScrollList(false);
//	else if (pWnd == (KWndWindow*)&m_FindBtn)
//		KUiSelPlayerNearby::OpenWindow();
//	else if (pWnd == (KWndWindow*)&m_CloseBtn)
//		CloseWindow(false);
//	else if (pWnd == (KWndWindow*)&m_UnitBtnFriend)
//	{
//		OnSelUnit(FRIEND_INDEX);
//	}
//	else if (pWnd == (KWndWindow*)&m_UnitBtnBrother)
//	{
//		OnSelUnit(BROTHER_INDEX);
//	}
//	else if (pWnd == (KWndWindow*)&m_UnitBtnEnemy)
//	{
//		OnSelUnit(ENEMY_INDEX);
//	}
//	else if (pWnd == (KWndWindow*)&m_UnitBtnOther)
//	{
//		PopupSelUnitMenu();
//	}
//	else
//		OnClickGroupBtn(pWnd, true);*/
//}
//
//int sub_552970(int a1, int a2, unsigned __int8* a3)
//{
//	int v3; // ebx
//	unsigned __int8 v4; // al
//	size_t v5; // edi
//	unsigned int v6; // kr00_4
//	int result; // eax
//	int v8; // [esp+4h] [ebp-408h]
//	char v9[1024]; // [esp+8h] [ebp-404h] BYREF
//
//	v3 = a1;
//	HIBYTE(v8) = HIBYTE(a2);
//	if (!a1)
//		return MEMORY[0x552970](a3, v3);
//	if (!a3)
//		return MEMORY[0x552970](a3, v3);
//	v4 = *a3;
//	if (*a3 <= 0x40u || v4 == 0xFF || !*(_DWORD*)(a2 + 4 * v4) || v4 != 0xFB)
//		return MEMORY[0x552970](a3, v3);
//	if (a3[a1 - 1] == 8 || a3[a3[46] + 46] == 8)
//	{
//		a3[a1 - 1] = 0;
//		v3 = a1 - 1;
//		a3[a3[46]-- + 46] = 0;
//	}
//	if ((memset(v9, 0, sizeof(v9)),
//		v5 = a3[46],
//		memcpy(v9, a3 + 47, v5),
//		v9[v5] = 0,
//		v6 = strlen(v9),
//		result = v6,
//		a3[46] > 1u)
//		&& v6 != 1
//		|| v9[0] != 27)
//	{
//		if (v9[v6 - 1] != 8)
//			return MEMORY[0x552970](a3, v3);
//	}
//	return result;
//}
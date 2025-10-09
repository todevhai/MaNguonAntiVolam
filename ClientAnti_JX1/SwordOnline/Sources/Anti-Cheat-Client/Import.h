#pragma once
//#include "stdafx.h"
//#include <gl\GL.h>
//#pragma comment(lib, "Opengl32.lib")
//#pragma comment(lib, "Ws2_32.lib")

//Offset size KWndWindow : 340, 
//Offset size KWndButton : 592
//Offset size KWndLabeledTextButton : 812
//Offset size KWndLabeledButton : 768
//Offset size KWndImage : 480, 440
// Offset size KWndText32 : 504, 

//extern unsigned int RefWndBase;
//extern unsigned int RefWndImage;
//extern unsigned int RefWndButton;
//extern unsigned int RefWndText32;
//extern unsigned int RefWndWindow;
typedef void(__thiscall* RefWndBase)(void*);
typedef void(__thiscall* BaseWndWindow)(void*);
typedef void(__thiscall* VtableWndWindow)(void*);
typedef void(__thiscall* RefWndWindow)(void*);
typedef void(__thiscall* RefWndImage)(void*);
typedef void(__thiscall* RefWndButton)(void*);
typedef void(__thiscall* RefWndText32)(void*);
typedef void* (__thiscall* fnWndWindow)(void*);

#define NewRegisterComClass		((int(__stdcall*)(char* This)) 0x4227B0)//0x528F40 0x441A60 0x441B80
#define CreateWndWindow		((void*(__thiscall*)(void* This)) 0x441A60)//0x528F40 0x441A60 0x441B80
#define CreateWndWindow2		((BYTE*(__thiscall*)(BYTE* This)) 0x441A60)//0x528F40 0x441A60 0x441B80
//#define CreateWndWindow2		((BYTE*(__thiscall*)(const TempWindow& This)) 0x441A60)//0x528F40 0x441A60 0x441B80
#define ReleaseWndWindow		((int(__thiscall*)(void* This)) 0x441B80)//0x528F40 0x441A60 0x441B80
#define DestroyWndWindow		((int(__thiscall*)(DWORD* This)) 0x44ADC0)//0x528F40 0x441A60
#define CreateWndImage		((void*(__thiscall*)(void* This)) 0x42C010)//0x528F40 
#define ReleaseWndImage		((LPVOID(__thiscall*)(DWORD* This)) 0x43DF00)//0x528F40 
#define WndfirstFunction		((LPVOID(__thiscall*)(LPVOID This)) 0x43DF00)//0x528F40 
#define CreateWndPageSet		((LPVOID(__thiscall*)(LPVOID This)) 0x437F20)//0x528F40 
#define CreateWndShowAnimate		((LPVOID(__thiscall*)(LPVOID This)) 0x43CAD0)//0x528F40 
#define CreateWndShowAnimate_Init		((LPVOID(__thiscall*)(LPVOID This, KIniFile* pIniFile, const char* pSection)) 0x43CB10)//0x528F40 
#define CreateWndShowAnimate_WndProc		((LPVOID(__thiscall*)(LPVOID This, unsigned int uMsg, unsigned int uParam, int nParam)) 0x4428D0)//0x528F40 
#define CreateWndButton		((void*(__thiscall*)(void* This)) 0x425E20)//0x528F40 
#define ReleaseWndButton		((LPVOID(__thiscall*)(void* This)) 0x43DF00)//0x528F40 
#define CreateWndToolBar		((LPVOID(__thiscall*)(LPVOID This)) 0x43E1D0)//0x528F40 
#define SafeCallOnLBtnDown		((void(__thiscall*)(WORD* This, bool bDoubleClick)) 0x426BA0) 
#define SafeCallOnButtonClick		((void(__fastcall*)(short* This)) 0x42C3F0) 
#define SafeAddOneWindow		((int(__thiscall*)(const void* This, KIniFile* pIniFile, const char* pSection)) 0x43E3D0)//0x528F40 

#define SafeCallUiArenaIcon		((int(*)()) 0x523370) 
#define SafeCallLoadSchemeForEachWnd		((void(__thiscall*)(const char* This)) 0x41A160) 

#define HookUiInitOpenWindow		((int(__cdecl*)(bool bStartMusic, bool bJustLaunched)) 0x4652E0)//0x528F40 
#define CallUiSysMsgCentreProc		((int(__thiscall*)(LPVOID This, unsigned int uMsg, int uParam, int nParam)) 0x4A5640)//0x528F40 
#define CallUiInitProc		((void(__thiscall*)(LPVOID This, LPVOID pWnd)) 0x4660D0)//0x528F40 
#define HookWindowProc		((int(__thiscall*)(LPVOID This, unsigned int uMsg, int uParam, int nParam)) 0x4428D0)//0x528F40 
#define HookButtonProc		((int(__thiscall*)(LPVOID This, unsigned int uMsg, int uParam, int nParam)) 0x4263A0)//0x528F40 
#define HookGlobalProc		((int(__thiscall*)(LPVOID This, unsigned int uMsg, int uParam, int nParam)) 0x4428D0)//0x528F40 
#define CallUiToolsControlBarProc		((int(__thiscall*)(LPVOID This, unsigned int uMsg, unsigned int uParam, int nParam)) 0x44BB00)//0x528F40 
#define CallUiArenaProc		((int(__thiscall*)(LPVOID This, unsigned int uMsg, unsigned int uParam, int nParam)) 0x5236D0)//0x523200
#define CallUiGameSpaceProc		((int(__cdecl*)(unsigned int uMsg, int uParam, int nParam)) 0x43AFC0)//0x523200
#define CallUiActivityGuide		((int(__thiscall*)(LPVOID This, unsigned int uMsg, int uParam, int nParam)) 0x5220F0)//0x523200
#define CallOnLBtnDown		((char(__thiscall*)(LPVOIDThis, int a2, int a3)) 0x442C80)//0x523200
#define CallWnd_SetCapture		((char(__cdecl*)(int a1, char a2)) 0x43B8B0)//0x523200

////#define b_Optimize		((void(__cdecl*)()) 0x639110)//0x528F40 
#define b_sub_420B90		((int(*)()) 0x420B90)//0x528F40 
//#define b_sub_5232C0		((int(*)()) 0x5232C0)//0x528F40 
////#define b_sub_491290		((int(*)()) 0x491290)//0x528F40 
//#define b_sub_4A50F0		((int(*)()) 0x4A50F0)//0x528F40 
////#define b_sub_441B80		((DWORD*(__thiscall*)(DWORD* a1)) 0x441B80)//0x528F40 
//
//
//#define KWndImage_Init_2		((int(__thiscall*)(int, KIniFile*, char*)) 0x42C0E0)//0x528F40 
//#define b_sub_42C010_new		((DWORD*(__thiscall*)(DWORD* a1)) 0x42C010)//0x528F40 
#define b_sub_42C010		((LPVOID(__thiscall*)(LPVOID This)) 0x42C010)//0x528F40 
//#define b_sub_43DCE0		((int*(__thiscall*)(void* a1)) 0x43DCE0)//0x528F40 
//#define b_sub_44ADC0		((int*(__thiscall*)(DWORD* a1)) 0x44ADC0)//0x528F40 
//
//#define b_sub_442730		((DWORD*(__thiscall*)(DWORD *, DWORD *)) 0x442730)//0x528F40 
#define b_sub_41A4A0		((int(__thiscall*)(const char *This, char* Data, int a3)) 0x41A4A0)//0x528F40 
//#define b_sub_442690_1		((DWORD*(__thiscall*)(DWORD *)) 0x442690)//0x528F40 
#define b_SetPosition		((LPVOID(__thiscall*)(unsigned int *, int , int )) 0x441CA0)  
#define b_Wnd_AddWindow		((LPVOID(__cdecl*)(void* This, int a2)) 0x43AD60)//0x528F40 
#define Call_Wnd_AddWindow		((void(__thiscall*)(KWndWindow* This, int a2)) 0x442730)//0x528F40 
//#define b_sub_442730		((LPVOID(__cdecl*)(LPVOID a1, int a2)) 0x442730)//0x528F40 
//#define b_sub_441BC0		((DWORD*(__thiscall*)(DWORD *)) 0x441BC0)//0x528F40 
//#define b_SetRepresentShell		((DWORD*(__stdcall*)(DWORD *)) 0x539AC0)//0x528F40 
//#define b_UiSetScreenSize		((void(__stdcall*)(int a1, int a2)) 0x539C20)//0x528F40 
#define b_sub_4255E0		((void(__cdecl*)(KUiImageRef& Img)) 0x4255E0)//0x528F40 
#define b_sub_4207E0		((BOOL(*)()) 0x4207E0)//0x528F40 
//#define b_sub_40B060		((DWORD*(__thiscall*)(DWORD *)) 0x40B060)//0x528F40 
//#define b_Get_NPC_nIndex		((int(__thiscall*)(KNpcInfo *This, int a2)) 0x580850)//0x528F40 
//#define b_sub_612F01_new		((int(__stdcall*)(int a1, int a2, int a3, void (__thiscall *a4)(int), int a5)) 0x612F01)//0x528F40 

//#define b_Get_NPC_nIndex		((int(__thiscall*)(char *This, int a2)) 0x63A66C)//0x528F40 
//#define HookOutputText2		((int(__stdcall*)(int, int nFontId, const char* psText, int nCount, int nX, int nY, unsigned int Color, int nLineWidth)) (0x6DE30C + 16)) //100055B0 
//#define HookOutputText		(*(int (__stdcall **)(void *, int nFontId, const char* psText, int nCount, int nX, int nY, unsigned int Color, int nLineWidth, int nZ, unsigned int BorderColor))(*(DWORD *)0x6DE30C + 16)) //100055B0 
//#define HookGetImageParam		(*(bool(__stdcall**)(void *, const char* pszImage, KImageParam* pImageData, int nType))(*(DWORD*)0x6DE30C + 52)) //100047B0 
//#define HookDrawPrimitives		(*(void(__stdcall**)(void *, int, void *, int, int))(*(DWORD*)0x6DE30C + 72)) //100047B0 
//#define HookCreateCoreShell		((DWORD*(__cdecl*)(DWORD *)) 0x5388C0) //100047B0 
//#define InitEngine		((int(__cdecl*)(const char*, DWORD *)) 0x63A530) //100047B0 
//#define gGetDrawHWnd		((HWND(__cdecl*)()) 0x63A800) //100047B0 
#define gGetMainHWnd		((HWND(__cdecl*)()) 0x613A52) //100047B0 
//#define gGetLoginAccount		((bool(__thiscall*)(DWORD* This,char* pszAccount, int nSize)) 0x52A9D0) //100047B0 
//#define exec_sub_424560		((void(__cdecl*)(const char *a1)) 0x424560) //100047B0 

#define b_KIniFile		((void*(__thiscall *)(void* This)) 0x63A8F8)// 0x63A8F8 KIniFile_Entry
#define b_KLoad		((bool(__thiscall*)(void* This, const char *Data)) 0x63A8E4)//0x63A8E4 KIniFile_Load
#define b_GetInteger		((int(__thiscall*)(void* This, const char* a1, const char* a2, int a3, int * a4)) 0x63A8F4)//0x63A8F4 KIniFile_GetInteger
#define b_GetInteger2		((int(__thiscall*)(KIniFile* This, LPCSTR	lpSection, LPCSTR	lpKeyName, int* pnValue1, int* pnValue2)) 0x63A808)//0x63A8F4 KIniFile_GetInteger
//#define b_GetString		((int(__thiscall*)(void *This, const char* a1, const char* a2, const char* a3, const char* a4, int a5)) 0x63A8E0)//0x63A8E0 KIniFile_GetString
//#define b_CleanIniFile		((void(__thiscall*)(void *This)) 0x63A8DC)//0x63A8DC 0x5E1A70 KIniFile_Clean
//#define b_g_HashString2Id		((unsigned long(__cdecl*)(char const *)) 0x63A8EC)//0x63A8DC 
//
//#define b_sub_612CD0		((void(__cdecl*)(void*)) 0x612CD0)//0x528F40 
//#define b_sub_404030		((int(__fastcall*)(int)) 0x404030)//0x528F40 
//#define b_sub_408C40		((DWORD*(*)()) 0x408C40)//0x528F40 
//#define b_sub_420AB0		((void(*)()) 0x420AB0)//0x528F40 
//#define b_sub_420820		((void(*)()) 0x420820)//0x528F40 
//#define b_sub_63A3E4		((void(__cdecl*)()) 0x63A3E4)//0x528F40 
//#define b_sub_402370		((DWORD*(__thiscall*)(DWORD *, int )) 0x402370)//0x528F40 
//#define b_invalid_parameter_noinfo		((void(__cdecl*)()) 0x6573E8)//0x528F40 
//
#define b_sub_528AE0		((int(__thiscall*)(BYTE *This)) 0x528AE0)//0x528F40 
#define b_sub_556590		((int(__stdcall*)(int a1, int a2)) 0x556590)//0x528F40 
// Loai tru 0x612E9E sub_401320
#define RefCountBaseClass		((int*(__stdcall*)(void *This, int nSize, int nCount, void(__thiscall* Offset)(int), void(__thiscall* vBase)(int))) 0x612F01)//0x528F40 0x61345F
//#define RefCountBaseClass		((int(__stdcall*)(void *This, int nSize, int nCount, void* Offset, void* vBase)) 0x612F01)//0x528F40 0x61345F
//#define RefCountBaseClass		((int*(__stdcall*)(void *This, int nSize, int nCount, void (__thiscall *Offset)(void *), DWORD vBase)) 0x612F01)//0x528F40 0x61345F
//#define RefCountBaseClass1		((void*(__stdcall*)(void *, int a2, int a3, void (__thiscall *a4)(int), int a5)) 0x401320)//0x528F40 
//#define b_sub_612E9E		((int*(__stdcall*)(int a1, int a2, int a3, int (__thiscall *a4)(int))) 0x612E9E)//0x528F40 
//#define b_sub_40ADA0		((DWORD*(__thiscall*)(DWORD* This)) 0x40ADA0)//0x528F40 
//#define b_sub_441B80		((int(__thiscall*)(DWORD* This)) 0x441B80)//0x528F40 
#define b_ShowWndWindow		((void(__thiscall*)(void* This)) 0x441EE0)//0x528F40 
#define b_HideWndWindow		((void(__thiscall*)(void* This)) 0x441F00)//0x528F40 
//#define b_sub_42C0E0		((int(__thiscall*)(int This, KIniFile* a2, char *a3)) 0x42C0E0)//0x528F40 
//#define b_sub_4428D0		((int(__thiscall*)(DWORD *this, unsigned int a2, int a3, int a4)) 0x4428D0)//0x528F40 
//#define b_sub_43D5C0		((int(__thiscall*)(int This, KIniFile* a2, char *a3)) 0x43D5C0)//0x528F40 
//
#define _TEncodeText		((int(__cdecl*)(char* pBuffer, int nCount, int nNew)) 0x613A46)//0x528F40 0x441A60 0x441B80
#define _IsMoseHoverWndObj		((int(__thiscall*)(void* This, void* pWnd, int nObj)) 0x422DC0)//0x528F40 0x441A60 0x441B80
#define _CancelMouseHoverInfo		((void(__thiscall*)(void* This)) 0x422E00)//0x528F40 0x441A60 0x441B80
#define _SetMouseHoverInfo		((void(__thiscall*)(void* This, void* pWnd, int nObj, int x, int y, bool bHeadTailImg, bool bFollowCursor)) 0x422E60)//0x528F40 0x441A60 0x441B80
#define _SetMouseHoverTitle		((void(__thiscall*)(void* This, const char *pTitleText, int nTitleTextLen, UINT uColor, int nNew1)) 0x422EE0)//0x528F40 0x441A60 0x441B80
#define _SetMouseHoverUpdate		((void(__thiscall*)(void* This, int x, int y)) 0x422FB0)//0x528F40 0x441A60 0x441B80
#define _SetPosition		((void(__thiscall*)(void* This, int nLeft, int nTop)) 0x441CA0)//0x528F40 0x441A60 0x441B80
#define _SetImage		((void(__thiscall*)(void* This, short nType, const char* pszImgName, bool bAdjustWndSize, short nNew1)) 0x42C4A0)//0x528F40 0x441A60 0x441B80
#define _SetText		((void(__thiscall*)(void* This, const char* pText, int nLen)) 0x43D280)//0x528F40 
#define _GetColorTip		((int(__cdecl*)(const char* pText)) 0x443230)//0x528F40 
//#define CreateWndWindow		((void*(__thiscall*)(void* This)) 0x441A60)//0x528F40 0x441A60 0x441B80
//#define DestroyWndWindow		((void*(__thiscall*)(void* This)) 0x44ADC0)//0x528F40 0x441A60
//#define CreateWndImage		((void*(__thiscall*)(void* This)) 0x42C010)//0x528F40 
#define SetObjectWndText32		((LPVOID(__thiscall*)(LPVOID a1)) 0x43DCE0)//0x528F40 
//#define This_AddChild		((DWORD*(__thiscall*)(void* child)) 0x442690)//0x528F40 
#define b_AddChild		((DWORD*(__thiscall*)(void* This, void* child)) 0x442690)//0x528F40 
#define b_AddBrother		((DWORD*(__thiscall*)(void* This, void* child)) 0x442730)//0x528F40 
#define b_SetTextPtr		((void(__thiscall*)(void* This, char* pText, int nMaxLen)) 0x43D510)//0x528F40 
//
#define KWndWindow_CloseWindow		((int(__thiscall*)(void* This, bool bDestory)) 0x401320)//0x528F40 
#define KWndWindow_Init		((int(__thiscall*)(void* This, KIniFile* a2, const char *a3)) 0x442060)//0x528F40 
#define KWndWindow_WndProc		((int(__thiscall*)(void* This, unsigned int uMsg, unsigned int uParam, int nParam)) 0x4428D0)//0x528F40 
#define KWndWindow_Paint		((LPVOID*(__thiscall*)(LPVOID* This)) 0x4425B0)//0x528F40 
#define KWndWindow_PtInWindow		((int(__thiscall*)(LPVOID This, int x, int y)) 0x442490)//0x528F40 
#define KWndWindow_PaintWindow		((void(__thiscall*)(void* This)) 0x443000)//0x528F40 
#define KWndWindow_GetSize		((void(__thiscall*)(void* This, int* pWidth, int* pHeight)) 0x441D10)//0x528F40 
#define KWndWindow_SetSize		((void(__thiscall*)(void* This, int nWidth, int nHeight)) 0x441D40)//0x528F40 
#define KWndWindow_AddChild		((void(__thiscall*)(void* This, void* child)) 0x442690)//0x528F40 
#define KWndWindow_AddBrother		((void(__thiscall*)(void* This, void* child)) 0x442730)//0x528F40 
#define KWndWindow_Show		((void(__thiscall*)(LPVOID This)) 0x441EE0)//0x528F40 
#define KWndWindow_Hide		((void(__thiscall*)(LPVOID This)) 0x441F00)//0x528F40 
#define KWndWindow_Enable		((void(__thiscall*)(void* This, int bEnable)) 0x441F70)//0x528F40 
#define KWndWindow_IsVisible		((LPVOID(__thiscall*)(void* This)) 0x441EE0)//0x528F40 
#define KWndWindow_BringToTop		((DWORD*(__thiscall*)(void* This)) 0x441BC0)//0x528F40 
#define KWndWindow_TopChildFromPoint		((KWndWindow*(void*, int x, int y)(void* This)) 0x442DA0)//0x528F40 

// show hide 0x441F70
#define KWndImage_Init		((int(__thiscall*)(void* This, KIniFile* a2, const char* a3)) 0x42C0E0)//0x528F40 
#define KWndImage_WndProc		((int(__thiscall*)(unsigned int uMsg, unsigned int uParam, int nParam)) 0x4428D0)//0x528F40 
#define KWndImage_PtInWindow		((int(__thiscall*)(void* This, int x, int y)) 0x42C320)//0x528F40 
#define KWndImage_PaintWindow		((void(__thiscall*)(void* This)) 0x42C5B0)//0x528F40 

#define KWndButton_new		((void(__thiscall*)(void)) 0x4d12d0)//0x401320
#define KWndButton_Init		((int(__thiscall*)(void* This, KIniFile* a2, const char* a3)) 0x425FB0)//0x528F40 
#define KWndButton_Enable		((void(__thiscall*)(void* This, int bEnable)) 0x426E60)//0x528F40 
#define KWndButton_WndProc		((int(__thiscall*)(void* This, unsigned int uMsg, unsigned int uParam, int nParam)) 0x4263A0)
#define KWndButton_PaintWindow		((void(__thiscall*)(void* This)) 0x426B30)
//#define KWndButton_OnButtonClick		((void(__thiscall*)(void* This, int bEnable)) 0x426E60)//0x4D12D0 

#define KWndText32_Init		((int(__thiscall*)(void* This, KIniFile* a2, const char* a3)) 0x43D5C0)//0x528F40 

#define KWndPageSet_WndProc		((int(__thiscall*)(void* This, unsigned int uMsg, unsigned int uParam, int nParam)) 0x438000)//0x528F40 
#define KWndPageSet_AddPage		((bool(__thiscall*)(void* This, KWndPage* pPage, KWndButton* pPageBtn)) 0x438040)//0x528F40 
#define KWndPageSet_ActivePage		((bool(__thiscall*)(void* This, int nPageIndex)) 0x438150)//0x528F40 
#define KWndPageSet_OnPageBtnClick		((bool(__thiscall*)(void* This, KWndWindow* pBtn)) 0x4382A0)//0x528F40 

//#define KWndImage_SetImage		((int(__thiscall*)(int This, short nType, const char* pszImgName)) 0x44F410)//0x528F40 
//#define KWndObjectBox_Init		((int(__thiscall*)(int This, KIniFile* a2, const char* a3)) 0x441d40)//0x528F40 
//#define KWndImage_This			((void*(__cdecl*)()) 0x0063cf4c)
//#define KWndText32_This			((void*(__cdecl*)()) 0x0063d8c4)
//#define KWndWindow_This			((void*(__cdecl*)()) 0x0063db6c)
//#define _GetMainHWnd			((HWND(__cdecl*)()) 0x613A52)
#define _ComClassFactory			((KComClassFactory*(*)()) 0x422720)
//#define _RegisterComClass			((int(__cdecl*)(const KComClassInfo& one)) 0x4227B0)
#define _Wnd_RenderWindows			((void(*)()) 0x43AC40)
#define _LoadScheme			((void(__cdecl*)(const char * This)) 0x4A7A30)
#define _PaintWindow			((void(__thiscall*)(void* This)) 0x42C5B0)
#define _RegisterComClass			((int(__stdcall*)(const void* one)) 0x4227B0)
#define _CreateComObject			(((KComClassFactory* (__thiscall*)(KComClassFactory *This, char *Str1)) 0x422810)
#define _HookCallHideWindow			((void(*)()) 0x51B2A0)
//#define ScenePlace_Paint			((void(__thiscall*)(int This)) 0x5853E0)
//#define aCreaterepresen			((BOOL(__cdecl*)(BOOL bFullScreen, int nWidth, int nHeight)) 0x526B30)
//#define aDrawGameObj			((unsigned int(__stdcall*)(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam, int nParam2)) 0x539A50)
#define b_SyncStateSkill		((int(__thiscall*)(_DWORD* This, int Index, int SkillID, int nLevel, int Data, int nDataNum, int nTime, int a7, int a8, int a9, int a10, int a11)) 0x57B080)//0x528F40 
//#define b_AddTail		((void(__thiscall*)(void* This, DWORD* Node)) 0x63A678)//0x528F40 
//
//#define pKFontThis				((void*(*)()) pFontThis)
//#define pDrawTextA				((void*(__thiscall*)(void* This, int nX, int nY, const char* psText)) (*(DWORD*)GetProcAddress(hModuleEngine, "?DrawTextA@KFont@@QAEXHHPAD@Z")))
//
//#define HookOutputText2		((int(__thiscall**)(int This, int nFontId, const char* psText, int nCount, int nX, int nY, unsigned int Color, int nLineWidth)) (g_pRepresentShell + 16)) //100055B0 
//#define HookOutputText		(*(int(__thiscall**)(int This, int nFontId, const char* psText, int nCount, int nX, int nY, unsigned int Color, int nLineWidth, int nZ, unsigned int BorderColor))  (g_pRepresentShell + 16)) //100055B0 
//#define HookGetImageParam		(*(bool(__thiscall**)(int This, const char* pszImage, KImageParam* pImageData, int nType))(g_pRepresentShell + 52)) //100047B0 
//#define HookDrawPrimitives		(*(void(__thiscall**)(int This, int, void *, int, int))(g_pRepresentShell + 72)) //100047B0 
#define Core_SetRepresentShell		((void(__stdcall*)(struct iRepresentShell* pRepresent)) 0x539AC0) //100047B0 
//#define call_sub_597DA0		((int*(__thiscall*)(_DWORD*This, int *a2, _DWORD *a3)) 0x597DA0) //100047B0 
//#define call_sub_576350		((const char*(__thiscall*)(_DWORD*This)) 0x576350) //100047B0 
//#define call_sub_5757F0		((BOOL(__cdecl*)(int a1)) 0x5757F0) //100047B0 
#define b_LoadBuffer		((int(__thiscall*)(DWORD *This, char *a2, unsigned int a3, const char *a4)) 0x63A894)//0x63A8DC 
#define b_ExecuteScriptFile		((int(__cdecl*)(const char*)) 0x4143D0)//0x63A8DC 
#define b_ExecuteScriptFile_1		((int(__cdecl*)(DWORD *This, LPCSTR ScriptFileName, LPCSTR szFunName, LPCSTR cFormat, ...)) 0x538D00)//0x63A8DC 0x538C70 0x538D00
#define b_GetScriptFile		((int(__cdecl*)(const char*)) 0x538C10)//0x63A8DC 
#define b_SetGraphic		((DWORD*(__thiscall*)(DWORD *This, __int16 a2)) 0x42C3F0) 
#define b_ExecuteScriptFunc		((int(__cdecl*)(const char* Func)) 0x414240) 
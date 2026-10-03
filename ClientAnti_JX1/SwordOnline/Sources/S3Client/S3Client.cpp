 // S3Client.cpp : Defines the entry point for the application.
//

#include "KWin32.h"
#include "KDebug.h"
#include "KCore.h"
#include "S3Client.h"
#include "KWin32Wnd.h"
#include "../../Represent/iRepresent/iRepresentShell.h"
/* Ban header duoc include dat ten ham khoi tao la CreateRepresentDirect va bo
   typedef nay; file nay goi CreateRepresentShell() khong tham so (xem
   CREATE_REPRESENT_SHELL_FUN ben duoi). Hai ban chi khac o ham khoi tao. */
typedef struct iRepresentShell* (*fnCreateRepresentShell)();
#include "Ui/UiShell.h"
#include "NetConnect/NetConnectAgent.h"
#include "TextCtrlCmd/TextCtrlCmd.h"
#include "KPakList.h"
#include "Ui/Elem/TextPic.h"
#include "Ui/Elem/UiCursor.h"
#include "Ui/Elem/SpecialFuncs.h"
#include "Ui/FilterTextLib.h"
#include "Ui/ChatFilter.h"
#include "Ui/uibase.h"
#include "ErrorCode.h"

#define ClientVersion
KMyApp		MyApp;
HINSTANCE	hInst;
KPakList	g_PakList;
CFilterTextLib g_libFilterText;
CChatFilter g_ChatFilter;


#define	QUIT_QUESTION_ID	"22"
#define	GAME_TITLE			"23"

#define REPRESENT_MODULE_2			"Represent2.dll"
#define REPRESENT_MODULE_3			"Represent3.dll"
#define CREATE_REPRESENT_SHELL_FUN	"CreateRepresentShell"
#define	GAME_FPS			18

/* Tran nhip VE, doc tu [Client] FPS trong config.ini. 0 = khong gioi han. */
int	g_nTranNhipVe = 0;
//RepresentÄ£¿é½Ó¿ÚµÄÖ¸Õë
struct iRepresentShell*	g_pRepresentShell = NULL;
struct IInlinePicEngineSink* g_pIInlinePicSink = NULL;
iCoreShell*				g_pCoreShell = NULL;
KMusic*					g_pMusic = NULL;

#define	DYNAMIC_LINK_REPRESENT_LIBRARY

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	static HMODULE		l_hRepresentModule = NULL;
	int					g_bRepresent3 = false;
#endif
	
int					g_bScreen = true;
char				g_szGameName[32] = "½£ÏÀÇéÔµ¡¤ÍøÂç°æ";


KClientCallback g_ClientCallback;

/* Khong con la hang so: doc tu [General] Resolution trong config.ini.
   Dat mac dinh 800x600 de khong doi hanh vi khi thieu khoa. */
int	g_nBeNgangManHinh = 800;
int	g_nBeDocManHinh = 600;
#define	SCREEN_WIDTH	g_nBeNgangManHinh
#define SCREEN_HEIGHT	g_nBeDocManHinh

/*
 * Add this macro by liupeng on 2003.3.20
 * This macro is helper that can judge some legal character
 */
#define _private_IS_SPACE(c)   ((c) == ' ' || (c) == '\r' || (c) == '\n' || (c) == '\t' || (c) == 'x')
#define IS_SPACE(c)	_private_IS_SPACE(c)

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

	/*
	 * Add this funtion by liupeng on 2003.3.20
	 * We can find some error when start a console tracer
	 */
#ifdef	TRUE

	bool bOpenTracer = false;

    while( lpCmdLine[0] == '-' || lpCmdLine[0] == '/' )
    {
        lpCmdLine++;
		
        switch ( *lpCmdLine++ )
        {
		case 'c':
        case 'C':
            bOpenTracer = true;
            break;
        }
		
        while( IS_SPACE( *lpCmdLine ) )
        {
            lpCmdLine++;
        }
    }
	
	if ( bOpenTracer ) 
	{
		AllocConsole();
	}

#endif // End of this function

	hInst = hInstance;
	if (MyApp.Init(hInstance))
		MyApp.Run();

#ifdef TRUE

	if ( bOpenTracer )
	{
		FreeConsole();
	}

#endif
	Error_Box();

	return 0;
}

KMyApp::KMyApp()
{
	m_pInlinePicSink = NULL;
}

BOOL InitRepresentShell(BOOL bFullScreen, int nWidth, int nHeight)
{
	Error_SetErrorString(g_bRepresent3 ? REPRESENT_MODULE_3 : REPRESENT_MODULE_2);
	if (g_pRepresentShell == NULL)
	{
#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
		if (l_hRepresentModule == NULL &&
			(l_hRepresentModule = LoadLibrary(g_bRepresent3 ? REPRESENT_MODULE_3 : REPRESENT_MODULE_2)) == NULL)
		{
			Error_SetErrorCode(ERR_T_LOAD_MODULE_FAILED);
			return FALSE;
		}
		fnCreateRepresentShell pCreate = (fnCreateRepresentShell)GetProcAddress(
			l_hRepresentModule, CREATE_REPRESENT_SHELL_FUN);
		if (pCreate == NULL || 
			(g_pRepresentShell = pCreate()) == NULL)
		{
			Error_SetErrorCode((pCreate == NULL) ? ERR_T_MODULE_UNCORRECT : ERR_T_MODULE_INIT_FAILED);
			return FALSE;
		}
#else
		g_pRepresentShell = CreateRepresentShell();
#endif
	}
	if(g_pRepresentShell->Create(nWidth, nHeight, bFullScreen != 0))
	{
		return TRUE;
	}
	else
	{
		Error_SetErrorCode(g_bRepresent3 ? ERR_T_REPRESENT3_INIT_FAILED : ERR_T_REPRESENT2_INIT_FAILED);
		return FALSE;
	}
}

BOOL KMyApp::GameInit()
{
	Error_SetErrorString("KMyApp::GameInit");
    #ifdef KUI_USE_HARDWARE_MOUSE
	
    ShowMouse(TRUE);
    
    #else   // KUI_USE_HARDWARE_MOUSE
	
    ShowMouse(FALSE);
    
    #endif

	g_SetRootPath(NULL);
	g_SetFilePath("\\");

	KIniFile*	pSetting = g_UiBase.GetCommConfigFile();
	if (pSetting)
	{
		pSetting->GetString("Main", "GameName", "½£ÏÀÇéÔµ¡¤ÍøÂç°æ", g_szGameName, sizeof(g_szGameName));
        SetWindowText(g_GetMainHWnd(), g_szGameName);
	}

#ifdef _DEBUG
	g_FindDebugWindow("#32770","DebugWin");
#endif

	KIniFile	IniFile;
	if (!IniFile.Load("\\config.ini"))
	{
		Error_SetErrorCode(ERR_T_FILE_NO_FOUND);
		Error_SetErrorString("\\config.ini");
		return FALSE;
	}

#ifdef _DEBUG
	BOOL		bCursor = FALSE;
	if (IniFile.GetInteger("Client", "ShowCursor", 0, &bCursor))
		ShowMouse(TRUE);
#endif

	IniFile.GetInteger("Client", "FullScreen", FALSE, &g_bScreen);
	{
		/* Doc ca hai cho: [Client] la muc da chac chan doc duoc (FullScreen
		   ngay tren dung no), [General] la cho ban 8.x quen dat. */
		int nDoPhanGiai = 0;
		IniFile.GetInteger("General", "Resolution", 0, &nDoPhanGiai);
		if (nDoPhanGiai != 1)
			IniFile.GetInteger("Client", "Resolution", nDoPhanGiai, &nDoPhanGiai);
		if (nDoPhanGiai == 1)
		{
			g_nBeNgangManHinh = 1024;
			g_nBeDocManHinh = 768;
		}
		g_DebugLog("[man hinh] Resolution=%d -> %dx%d", nDoPhanGiai,
			g_nBeNgangManHinh, g_nBeDocManHinh);
	}
	IniFile.GetInteger("Client", "FPS", 0, &g_nTranNhipVe);

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	IniFile.GetInteger("Client", "Represent", 2, &g_bRepresent3);
	g_bRepresent3 = (g_bRepresent3 == 3);
#endif

	g_PakList.Open("\\config.ini");

	char	szPath[MAX_PATH];
	if (IniFile.GetString("Client", "CapPath", "", szPath, sizeof(szPath)))
	{
		if (szPath[0])
			SetScrPicPath(szPath);
	}

	IniFile.Clear();

	if (!g_libFilterText.Initialize()
		|| !g_ChatFilter.Initialize())
		return FALSE;

	if (!InitRepresentShell(g_bScreen, SCREEN_WIDTH, SCREEN_HEIGHT))
	{
		return FALSE;
	}

	if (!UiInit())
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("UiInit");
		return FALSE;
	}

	//[wxb 2003-6-23]
	m_pInlinePicSink = new KInlinePicSink;
    if (m_pInlinePicSink)
	{
		m_pInlinePicSink->Init(g_pRepresentShell);
		_ASSERT(NULL == g_pIInlinePicSink);
		g_pIInlinePicSink = m_pInlinePicSink;
	}

	UiSetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	UiPaint(0);

	// init dsound
	m_Sound.Init();

	SetMultiGame(TRUE);

	if ((g_pCoreShell = CoreGetShell()) == NULL)
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("CoreGetShell");
		return false;
	}
	g_pCoreShell->SetRepresentShell(g_pRepresentShell);
	g_pCoreShell->SetMusicInterface((KMusic*)&m_Music);
	g_pCoreShell->SetCallDataChangedNofify(&g_ClientCallback);
	g_pCoreShell->SetRepresentAreaSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	
	g_pMusic = &m_Music;

	if (g_NetConnectAgent.Initialize() == 0)
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("NetConnectAgent");
		return FALSE;
	}

	m_GameCounter = 0;
	m_Timer.Start();
	
	SetMouseHoverTime(400);

	if(UiStart())
	{
		return TRUE;
	}
	else
	{
		Error_SetErrorCode(ERR_T_MODULE_INIT_FAILED);
		Error_SetErrorString("UiStart");
		return FALSE;
	}
}

BOOL KMyApp::GameExit()
{
	if (m_pInlinePicSink)
	{
		//[wxb 2003-6-23]
		m_pInlinePicSink->UnInit();
		delete m_pInlinePicSink;
		m_pInlinePicSink = NULL;
		g_pIInlinePicSink = NULL;
	}

	UiExit();

	g_pMusic = NULL;
	if (g_pCoreShell)
	{
		g_pCoreShell->SetRepresentShell(NULL);
		g_pCoreShell->SetClient(NULL);
		g_pCoreShell->SetMusicInterface(NULL);
		g_pCoreShell->Release();
		g_pCoreShell = NULL;
	}

	if (g_pRepresentShell)
	{
		g_pRepresentShell->Release();
		g_pRepresentShell = NULL;
	}

	g_NetConnectAgent.Exit();

	m_Music.Close();
	m_Sound.Exit();

#ifdef DYNAMIC_LINK_REPRESENT_LIBRARY
	if (l_hRepresentModule)
	{
		FreeLibrary(l_hRepresentModule);
		l_hRepresentModule = NULL;
	}
#endif

	::ShowCursor(TRUE);

	g_ChatFilter.Uninitialize();
	g_libFilterText.Uninitialize();

	return TRUE;
}

BOOL KMyApp::GameLoop()
{
	static int nGameFps = 0;
	g_NetConnectAgent.Breathe();
	
	if (m_GameCounter * 1000 <= m_Timer.GetElapse() * GAME_FPS)
	{
		if (g_pCoreShell->Breathe() && UiHeartBeat())
		{
			m_GameCounter++;
			int	nElapse = m_Timer.GetElapse();
			if (nElapse)
				nGameFps = m_GameCounter * 1000 / nElapse;
		}
		else
		{
			return false;
		}
	}
	if (m_GameCounter * 1000 >= m_Timer.GetElapse() * GAME_FPS)
	{
		if (g_nTranNhipVe <= 0)
		{
			UiPaint(nGameFps);
		}
		else
		{
			/* GetTickCount quay vong sau 49 ngay; hieu hai DWORD van dung. */
			static DWORD	s_dwVeLanTruoc = 0;
			DWORD		dwBayGio = GetTickCount();
			if ((DWORD)(dwBayGio - s_dwVeLanTruoc) >= (DWORD)(1000 / g_nTranNhipVe))
			{
				s_dwVeLanTruoc = dwBayGio;
				UiPaint(nGameFps);
			}
		}
		Sleep(1);
	}
	else if ((m_GameCounter % 8) == 0)
	{
		Sleep(1);
	}

	return true;
}

int KMyApp::HandleInput(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nRet = 0;
	{
		extern unsigned int g_uAutoGiuChuotDen;	/* Auto/KAutoControl.cpp */
		if ((uMsg == WM_MOUSEMOVE || uMsg == 0x02A1 /* WM_MOUSEHOVER */) && GetTickCount() < g_uAutoGiuChuotDen)
			return 0;
	}
	if (uMsg != WM_CLOSE)
	{
		UiProcessInput(uMsg, wParam, lParam);
	}
	else if (g_bScreen == false && UiIsAlreadyQuit() == false)
	{
		KIniFile*	pSetting = g_UiBase.GetCommConfigFile();
		if (pSetting)
		{
			char	szMsg[128], szTitle[64];
			pSetting->GetString("InfoString", QUIT_QUESTION_ID, "", szMsg, sizeof(szMsg));
			pSetting->GetString("InfoString", GAME_TITLE, "", szTitle, sizeof(szTitle));
			if (szMsg[0] && szTitle[0])
			{
				nRet = (MessageBox(g_GetMainHWnd(), szMsg, szTitle,
					MB_YESNO | MB_ICONQUESTION) != IDYES);
			}
		}
	}
	return nRet;
}

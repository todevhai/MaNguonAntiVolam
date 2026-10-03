// -------------------------------------------------------------------------
//	文件名		：	UiInit.cpp
//	创建者		：	彭建波
//	创建时间	：	2002-9-10 11:27:04
//	功能描述	：	初始界面
//
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "KDebug.h"
#include "KSG_MD5_String.h"
#include "KFilePath.h"
#include "KMusic.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "../UiShell.h"
#include "UiInit.h"
#include "UiSelServer.h"
#include "UiNotice.h"
#include "UiLoginBg.h"
#include "UiOptions.h"
#include "UiConnectInfo.h"
#include "UiPlayVideo.h"
#include "../UiSoundSetting.h"
//#include "UiReconnect.h"

extern KMusic*		g_pMusic;

#define	SCHEME_INI_INIT 	"UiInit.ini"
#define	LAUNCH_GAME_INI		"\\Ui\\UiLauch.ini"


KUiInit* KUiInit::m_pSelf = NULL;

enum UIINT_BUTTON
{
	UIB_NONE,
	UIB_ENTER_GAME,
	UIB_AUTO_LOGIN,
	UIB_GMAE_CONFIG,
	UIB_DESIGNER_LIST,
	UIB_EXIT_GAME,
};

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiInit* KUiInit::OpenWindow(bool bStartMusic, bool bJustLaunched)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiInit;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		if (bJustLaunched)
		{
			Wnd_ShowCursor(false);
			KUiOptions::LoadSetting(true, true);//音量只有在打开音乐才能使用。
			m_pSelf->m_nCurrentMovieIndex = 0;
			KUiLoginBackGround::CloseWindow(false);
			m_pSelf->PlayStartMovie();
		}
		else
		{
			UiSoundPlay(UI_SI_POPUP_OUTGAME_WND);
			KUiLoginBackGround::OpenWindow(m_pSelf->m_szLoginBg);
			m_pSelf->Show();
			if (bStartMusic)
				PlayTitleMusic();
		}
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：关闭窗口，同时可以选则是否删除对象实例
//--------------------------------------------------------------------------
void KUiInit::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_ShowCursor(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}	
}

void KUiInit::PlayStartMovie()
{
	KIniFile	Ini;

	char	szMovieIndex[32], szFile[128];
	char	szPathFile[MAX_PATH];

	if (Ini.Load(LAUNCH_GAME_INI))
	{
		int		nSkipable;
		sprintf(szMovieIndex, "Movie_%d", m_nCurrentMovieIndex);
		Ini.GetString("JustLaunched", szMovieIndex, "", szFile, sizeof(szFile));
		strcat(szMovieIndex, "_Skipable");
		Ini.GetInteger("JustLaunched", szMovieIndex, 0, &nSkipable);

		if (szFile[0])
		{			
			KUiPlayVideo* pPlayer = KUiPlayVideo::OpenWindow();
			if (pPlayer)
			{
				pPlayer->SetPosition(0, 0);
				int nWidth, nHeight;
				Wnd_GetScreenSize(nWidth, nHeight);
				pPlayer->SetSize(nWidth, nHeight);
				pPlayer->Setting(nSkipable != 0, false, this, 0);
		
				g_GetFullPath(szPathFile, szFile);
				if (pPlayer->OpenVideo(szPathFile))
				{	//成功播放影片
					m_nCurrentMovieIndex++;
					return;
				}
			}
		}
	}
	KUiPlayVideo::CloseWindow(true);
	Wnd_ShowCursor(true);
	OpenWindow(true, false);
}

void KUiInit::PlayTitleMusic()
{
	char	szMusic[128] = "";
	KIniFile	Ini;
	if (Ini.Load(LAUNCH_GAME_INI))
	{
		int	nCount = 0;
		Ini.GetInteger("JustLaunched", "TitleMusicCount", 0, &nCount);
		if (nCount > 0)
		{
			char	szKey[16];
			sprintf(szKey, "TitleMusic_%d", rand() % nCount);
			Ini.GetString("JustLaunched", szKey, "", szMusic, sizeof(szMusic));
			if (szMusic[0])
			{
				g_pMusic->Stop();
				g_pMusic->Open((char*)szMusic);
				g_pMusic->Play(true);
			}
		}
	}
}

void KUiInit::StopTitleMusic()
{
	if (g_pMusic)
	{
		g_pMusic->Stop();
		g_pMusic->Close();
	}
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiInit::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_INIT);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		m_EnterGameBorder.Init(&Ini, "EnterGameBorder");
		m_EnterGame.	Init(&Ini, "EnterGame");
		m_GameConfigBorder.Init(&Ini, "GameConfigBorder");
		m_GameConfig.	Init(&Ini, "GameConfig");
		m_OpenRepBorder.	Init(&Ini, "OpenRepBorder");
		m_OpenRep.	Init(&Ini, "OpenRep");
		m_KingSoft.	Init(&Ini, "KingSoft");
		m_ExitGameBorder.Init(&Ini, "ExitGameBorder");
		m_ExitGame.		Init(&Ini, "ExitGame");

		Ini.GetString("Main", "LoginBg", "", m_szLoginBg, sizeof(m_szLoginBg));
	}	
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
void KUiInit::Initialize()
{
	AddChild(&m_EnterGameBorder);
	AddChild(&m_EnterGame);
	AddChild(&m_KingSoft);
	AddChild(&m_GameConfigBorder);
	AddChild(&m_GameConfig);
	AddChild(&m_OpenRepBorder);
	AddChild(&m_OpenRep);
	AddChild(&m_ExitGameBorder);
	AddChild(&m_ExitGame);

	m_szLoginBg[0] = 0;

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this, WL_TOPMOST);
}

//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
/* Dang nhap tu dong theo cau hinh cua ta. Dat o day chu khong o ShowCompleted
   vi ShowCompleted chay ngay trong OpenWindow: dong cua so o do se lam
   UiStart() thay NULL. */
bool g_bXinDangNhapTuDong = false;

static bool DangNhapTuDongTheoCauHinh()
{
    char szTepCauHinh[] = "\\Ui\\Setting.ini";
    KIniFile Ini;
    if (!Ini.Load(szTepCauHinh))
        return false;

    int nBat = 0;
    Ini.GetInteger("AutoLogin", "Enable", 0, &nBat);
    if (!nBat)
        return false;

    char szTaiKhoan[32];
    KSG_PASSWORD MatKhau;
    szTaiKhoan[0] = 0;
    memset(&MatKhau, 0, sizeof(MatKhau));
    Ini.GetString("AutoLogin", "Account", "", szTaiKhoan, sizeof(szTaiKhoan));
    char szMatKhauTho[KSG_PASSWORD_MAX_SIZE];
    szMatKhauTho[0] = 0;
    Ini.GetString("AutoLogin", "Password", "", szMatKhauTho, sizeof(szMatKhauTho));
    /* Phai di DUNG duong ma giao dien di (UiLogin.cpp:316), khong thi
       tu dang nhap gui mat khau THO con go tay gui MD5 -> cung mot tai
       khoan ma hai bi mat khac nhau, duong nao dang ky truoc thi duong
       kia bi khoa ngoai. Chi lo ra khi may chu bat kiem mat khau. */
#ifdef SWORDONLINE_USE_MD5_PASSWORD
    KSG_StringToMD5String(MatKhau.szPassword, szMatKhauTho);
#else
    strncpy(MatKhau.szPassword, szMatKhauTho, sizeof(MatKhau.szPassword) - 1);
#endif
    memset(szMatKhauTho, 0, sizeof(szMatKhauTho));
    g_DebugLog("[TuDong] Setting.ini Enable=%d Account=\"%s\"", nBat, szTaiKhoan);
    if (!szTaiKhoan[0])
        return false;

    KUiInit::CloseWindow();
    KUiConnectInfo::OpenWindow(CI_MI_CONNECTING, LL_S_IN_GAME);
    return g_LoginLogic.DangNhapTuDongTheoCauHinh(szTaiKhoan, MatKhau) != 0;
}

void KiemDangNhapTuDong()
{
    if (!g_bXinDangNhapTuDong)
        return;
    g_bXinDangNhapTuDong = false;
    DangNhapTuDongTheoCauHinh();
}

int KUiInit::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		OnClickButton((KWndButton*)(KWndWindow*)uParam);
		break;
	case WM_KEYDOWN:
		nRet = OnKeyDown(uParam);
		break;
	case WM_SYSKEYDOWN:
		if (uParam == 'A')
			OnAutoLogin();
		break;
	case WND_M_OTHER_WORK_RESULT:
		PlayStartMovie();
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);	
	}
	return nRet;
}

int KUiInit::OnKeyDown(unsigned int uKey)
{
	int	nRet = 1;
	KWndButton* pActive = NULL;
	KWndButton* pToActive = NULL;
	if (uKey == VK_RETURN)
	{
		if (pActive = GetActiveBtn())
			OnClickButton(pActive);
	}
	else if (uKey == VK_UP)
	{
		pActive = GetActiveBtn();
		if (pActive == &m_ExitGame)
			pToActive = &m_OpenRep;
		else if (pActive == &m_OpenRep)
			pToActive = &m_GameConfig;
		else if (pActive == &m_EnterGame)
			pToActive = &m_ExitGame;
		else
			pToActive = &m_EnterGame;
	}
	else if (uKey == VK_DOWN)
	{
		pActive = GetActiveBtn();
		if (pActive == &m_EnterGame)
			pToActive = &m_GameConfig;
		else if (pActive == &m_GameConfig)
			pToActive = &m_OpenRep;
		else if (pActive == &m_OpenRep)
			pToActive = &m_ExitGame;
		else
			pToActive = &m_EnterGame;
	}
	else
		nRet = 0;
	if (pToActive)
		pToActive->SetCursorAbove();
	return nRet;
}

//--------------------------------------------------------------------------
//	功能：响应点击按钮
//--------------------------------------------------------------------------
void KUiInit::OnClickButton(KWndButton* pWnd)
{
	if (pWnd == &m_EnterGame)
	{
		if (KUiNotice::OpenWindow())
			CloseWindow();
	}
	else if (pWnd == &m_GameConfig)
	{
		if (KUiOptions::OpenWindow(this))
			Hide();
	}
	else if (pWnd == &m_OpenRep)
	{
		//to do: write load designer list here...
	}
	else if (pWnd == &m_ExitGame)
	{
		CloseWindow();
		UiPostQuitMsg();
	}
}

void KUiInit::OnAutoLogin()
{
	g_LoginLogic.LoadLoginChoice();
	if (g_LoginLogic.IsAutoLoginEnable())
	{
		KIniFile*	pSetting = g_UiBase.GetCommConfigFile();
		int nAutoLogin = false;
		if (pSetting)
		{
			pSetting->GetInteger("Main", "AutoLogin", 0, &nAutoLogin);
			g_UiBase.CloseCommConfigFile();
		}
		if (nAutoLogin == 6323)
		{
			CloseWindow();
			KUiConnectInfo::OpenWindow(CI_MI_CONNECTING, LL_S_IN_GAME);
			g_LoginLogic.AutoLogin();
		}
	}
}

KWndButton*	KUiInit::GetActiveBtn()
{
	KWndButton* pBtn = NULL;
	if (m_EnterGame.IsButtonActive())
		pBtn = &m_EnterGame;
//	else if (m_AutoLogin.IsButtonActive())
//		pBtn = &m_AutoLogin;
	else if (m_GameConfig.IsButtonActive())
		pBtn = &m_GameConfig;
	else if (m_OpenRep.IsButtonActive())
		pBtn = &m_OpenRep;
	else if (m_ExitGame.IsButtonActive())
		pBtn = &m_ExitGame;

	return pBtn;
}

void KUiInit::ShowCompleted()
{
    m_EnterGame.SetCursorAbove();
    g_bXinDangNhapTuDong = true;
}

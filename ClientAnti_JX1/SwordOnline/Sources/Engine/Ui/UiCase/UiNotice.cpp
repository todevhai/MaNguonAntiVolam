/*****************************************************************************************
//	½çÃæ--Ñ¡ÔñÓÎÏ··þÎñÆ÷´°¿Ú
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-12
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiShell.h"
#include "UiInit.h"
#include "UiLoginBg.h"
#include "UiConnectInfo.h"
#include "UiSelServer.h"
#include "UiNotice.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "../../Login/Login.h"
#include <crtdbg.h>

#define	SCHEME_INI_NOTICE				"UiNotice.ini"


KUiNotice* KUiNotice::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹¹Ôìº¯Êý
//--------------------------------------------------------------------------
KUiNotice::KUiNotice()
{
	m_szLoginBg[0] = 0;
	
}

KUiNotice::~KUiNotice()
{
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´ò¿ª´°¿Ú£¬·µ»ØÎ¨Ò»µÄÒ»¸öÀà¶ÔÏóÊµÀý
//--------------------------------------------------------------------------
KUiNotice* KUiNotice::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiNotice;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_POPUP_OUTGAME_WND);
		m_pSelf->Show();
		m_pSelf->UpdateNotice();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹Ø±Õ´°¿Ú
//--------------------------------------------------------------------------
void KUiNotice::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
		{
			m_pSelf->Hide();
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
void KUiNotice::Initialize()
{
	AddChild(&m_List);
	AddChild(&m_Notice);
	AddChild(&m_LoveGame);
	AddChild(&m_Close);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this, WL_TOPMOST);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë´°¿ÚµÄ½çÃæ·½°¸
//--------------------------------------------------------------------------
void KUiNotice::LoadScheme(const char* pScheme)
{
	char		Buff[255];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_NOTICE);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		m_List     .Init(&Ini, "List");
		m_Notice.Init(&Ini, "Notice");
		m_LoveGame    .Init(&Ini, "LoveGame");
		m_Close   .Init(&Ini, "Close");
		Ini.GetString("Main", "LoginBg", "", m_szLoginBg, sizeof(m_szLoginBg));
		

	}
}


void KUiNotice::UpdateNotice()
{
	m_List.SetText("Phiªn b¶n ®­îc ph¸t triÓn tõ mét nhãm ®am mª game Vâ L©m TruyÒn K× !. Kh«ng ph¶i chÝnh thøc tõ nhµ ph¸t hµnh Vinagame t¹i ViÖt Nam.");
	m_Notice.SetText("Phiªn b¶n hiÖn t¹i: \nTrang chñ: §ang cËp nhËt\nThµnh viªn ph¸t triÓn: Hoµng");
	m_LoveGame.SetText("S¬n Hµ X· T¾c");
	
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´°¿Úº¯Êý
//--------------------------------------------------------------------------
int KUiNotice::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		OnClickButton((KWndButton*)(KWndWindow*)uParam);
		break;
		case WM_KEYDOWN:
		nRet = OnKeyDown(uParam);
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}


void KUiNotice::OnClickButton(KWndButton* pBtn)
{
	if (pBtn == &m_Close)
		{
		if (KUiSelServer::OpenWindow())
			CloseWindow(true);
		}
	
}


int KUiNotice::OnKeyDown(unsigned int uKey)
{
	int	nRet = 1;
	KWndWindow* pToActive = NULL;
	switch (uKey)
	{
	case VK_RETURN:
		if (m_Close.IsButtonActive())
			if (KUiSelServer::OpenWindow())
				CloseWindow(true);
		break;
	case VK_LEFT:
		if (m_Close.IsButtonActive() == 0)
			pToActive = &m_Close;
		break;
	case VK_RIGHT:
		if (m_Close.IsButtonActive() == 0)
			pToActive = &m_Close;
		break;
	default:
		nRet = 0;
	break;
	}
	if (pToActive)
		pToActive->SetCursorAbove();
	return nRet;
}

void KUiNotice::ShowCompleted()
{
    m_Close.SetCursorAbove();
}


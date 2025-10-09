#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiShell.h"
#include "../../../core/src/coreshell.h"
#include "UiInit.h"
#include "UiLoginBg.h"
#include "../UiSoundSetting.h"
#include <crtdbg.h>
#include "UiQuestDT.h"
#include "../UiBase.h"
extern iCoreShell*	g_pCoreShell;
#define Quest1							"QuestDaTau.ini"

KUiDaTau* KUiDaTau::m_pSelf = NULL;



KUiDaTau* KUiDaTau::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiDaTau;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_POPUP_OUTGAME_WND);
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiDaTau::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_ShowCursor(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}	
}

void KUiDaTau::Initialize()
{
	AddChild(&EXP);
	AddChild(&random);
	AddChild(&money);
	AddChild(&text);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this, WL_TOPMOST);
}

void KUiDaTau::LoadScheme(const char* pScheme)
{
	char		Buff[64];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, Quest1);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		EXP	.Init(&Ini, "ExpBtn");
		random.Init(&Ini, "RandomBtn");
		money.Init(&Ini, "MoneyBtn");
		text.Init(&Ini, "Text");
		Ini.GetString("StringQuest", "GetGift", "", Buff, sizeof(Buff));
		text.AddOneMessage(Buff,sizeof(Buff));
	}
}

int KUiDaTau::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&EXP)
		{
			Hide();
			g_pCoreShell->OperationRequest(GOI_DATAU, 1, 0);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&money)
		{
			Hide();
			g_pCoreShell->OperationRequest(GOI_DATAU, 2, 0);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&random)
		{
			Hide();
			g_pCoreShell->OperationRequest(GOI_DATAU, 3, 0);
		}
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}



#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiShell.h"
#include "../../../core/src/coreshell.h"
#include "../Elem/PopupMenu.h"
#include "../UiSoundSetting.h"
#include <crtdbg.h>
#include "UiMarketPage.h"
#include "../UiBase.h"
extern iCoreShell*	g_pCoreShell;
#define MARKETPAGE_UI							"UiMarketPage.ini"

KUiMarketPage* KUiMarketPage::m_pSelf = NULL;

KUiMarketPage::KUiMarketPage()
{
m_pSkinMenu = NULL;
}

KUiMarketPage* KUiMarketPage::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMarketPage;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiMarketPage* KUiMarketPage::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiMarketPage::CloseWindow()
{
	if (m_pSelf)
	{
		m_pSelf->CancelMenu();
		Wnd_ShowCursor(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}	
}

void KUiMarketPage::CancelMenu()
{
	if (m_pSkinMenu)
	{
		KPopupMenu::Cancel();
		free(m_pSkinMenu);
		m_pSkinMenu = NULL;
	}
}

void KUiMarketPage::Initialize()
{
	AddChild(&VP);
	AddChild(&BK);
	AddChild(&DT);
	AddChild(&VatPham);
	AddChild(&BiKip);
	AddChild(&DacThu);
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this, WL_TOPMOST);
}

void KUiMarketPage::LoadScheme(const char* pScheme)
{
	char Buff[32];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, MARKETPAGE_UI);
	if (Ini.Load(Buff))
	{
		KWndShowAnimate::Init(&Ini, "Main");
		VP		.Init(&Ini, "VPImage");
		BK		.Init(&Ini, "BKImage");
		DT		.Init(&Ini, "DTImage");
		VatPham	.Init(&Ini, "VatPham");
		VatPham	.SetText("VËt phÈm");
		BiKip	.Init(&Ini, "BiKip");
		BiKip	.SetText("BÝ kÝp");
		DacThu		.Init(&Ini, "DacThu");
		DacThu		.SetText("§Æc thï");
	}
}

int KUiMarketPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&VatPham)
		{
			g_pCoreShell->OperationRequest(GOI_MARKET, 0, 0);
						Hide();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&BiKip)
		{
			g_pCoreShell->OperationRequest(GOI_MARKET, 1, 0);
						Hide();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&DacThu)
		{
			g_pCoreShell->OperationRequest(GOI_MARKET, 2, 0);
				Hide();		
		}
		break;
	case WND_M_OTHER_WORK_RESULT:
		Hide();
		break;		
	default:
		nRet = KWndWindow::WndProc(uMsg, uParam, nParam);
		break;
	}
	return nRet;
}



/*****************************************************************************************
//	Hop xac nhan xung huyet. Xem UiMeridianConfirm.h.
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "KEngine.h"							// g_DebugLog
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"
#include "UiMeridianConfirm.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI_CONFIRM		"UiMeridianConfirm.ini"
#define	WAY_NORMAL				0			// MERIDIAN_LEVELUP_WAY_NOMAL
#define	WAY_DAHUMAIDAN			3			// MERIDIAN_LEVELUP_WAY_USE_DAHUMAIDAN: mach 3-4

// chi so = cach may chu (0 Pho thong, 1 Huyet vi bao ve, 2 Long Hon Ho The); tren hop jx9tn chung
// xep nguoc lai tu trai sang phai (Long Hon, Bao ve, Pho thong) - vi tri nam o ini.
static const char* s_pszWaySection[MERIDIAN_CONFIRM_WAYS] = { "cbNormalWay", "cbProtectionWay", "cbEnhanceRateWay" };

KUiMeridianConfirm* KUiMeridianConfirm::m_pSelf = NULL;

KUiMeridianConfirm::KUiMeridianConfirm()
{
	m_nMeridian = 1;
	m_nLevel = 1;
	m_nWay = WAY_NORMAL;
	m_nVersion = -1;
	m_szFormatTitle[0] = 0;
}

KUiMeridianConfirm* KUiMeridianConfirm::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

KUiMeridianConfirm* KUiMeridianConfirm::OpenWindow(int nMeridian, int nLevel)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMeridianConfirm;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->SetTarget(nMeridian, nLevel);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiMeridianConfirm::CloseWindow()
{
	if (m_pSelf)
	{
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

void KUiMeridianConfirm::Initialize()
{
	AddChild(&m_Title);
	AddChild(&m_Info);
	for (int i = 0; i < MERIDIAN_CONFIRM_WAYS; i++)
	{
		AddChild(&m_WayBox[i]);
		AddChild(&m_WayLabel[i]);
	}
	AddChild(&m_BtnOk);
	AddChild(&m_BtnOkOnlyOneWay);
	AddChild(&m_BtnClose);

	char szScheme[256];
	g_UiBase.GetCurSchemePath(szScheme, 256);
	LoadScheme(szScheme);
	Wnd_AddWindow(this);
}

void KUiMeridianConfirm::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_CONFIRM);
	if (!Ini.Load(Buff))
		return;
	Init(&Ini, "Main");
	m_Title.Init(&Ini, "txtTitle");
	m_Info.Init(&Ini, "ListInfo");
	for (int i = 0; i < MERIDIAN_CONFIRM_WAYS; i++)
	{
		char szSection[40];
		m_WayBox[i].Init(&Ini, s_pszWaySection[i]);
		sprintf(szSection, "%s_Label", s_pszWaySection[i]);
		m_WayLabel[i].Init(&Ini, szSection);
	}
	m_BtnOk.Init(&Ini, "btnOk");
	m_BtnOkOnlyOneWay.Init(&Ini, "btnOk_OnlyOneWay");
	m_BtnClose.Init(&Ini, "btnClose");
	// Chu co dau nam o ini (TCVN3); nguon chi giu ASCII.
	Ini.GetString("txtTitle", "Format", "%s", m_szFormatTitle, sizeof(m_szFormatTitle));
}

void KUiMeridianConfirm::SetTarget(int nMeridian, int nLevel)
{
	m_nMeridian = nMeridian;
	m_nLevel = nLevel;

	KUiMeridianAcupDesc Desc;
	memset(&Desc, 0, sizeof(Desc));
	Desc.nMeridian = nMeridian;
	Desc.nLevel = nLevel;
	char szTitle[96];
	szTitle[0] = 0;
	if (g_pCoreShell && g_pCoreShell->GetGameData(GDI_MERIDIAN_ACUP_DESC, (unsigned int)&Desc, 0))
		sprintf(szTitle, m_szFormatTitle, Desc.szName);
	m_Title.SetText(szTitle);

	// Mach 3-4 may chu luon dung Dai Ho Mach Don (jx9tn GetDefaultWay): mot cach, nut ra giua
	bool bOneWay = (nMeridian == 3 || nMeridian == 4);
	for (int i = 0; i < MERIDIAN_CONFIRM_WAYS; i++)
	{
		if (bOneWay)
		{
			m_WayBox[i].Hide();
			m_WayLabel[i].Hide();
		}
		else
		{
			m_WayBox[i].Show();
			m_WayLabel[i].Show();
		}
	}
	if (bOneWay)
	{
		m_BtnOk.Hide();
		m_BtnOkOnlyOneWay.Show();
	}
	else
	{
		m_BtnOk.Show();
		m_BtnOkOnlyOneWay.Hide();
	}
	m_Info.SetText("");
	SelectWay(bOneWay ? WAY_DAHUMAIDAN : WAY_NORMAL);
}

void KUiMeridianConfirm::SelectWay(int nWay)
{
	m_nWay = nWay;
	for (int i = 0; i < MERIDIAN_CONFIRM_WAYS; i++)
		m_WayBox[i].CheckButton(i == nWay);
	RequestTips();
}

void KUiMeridianConfirm::RequestTips()
{
	m_nVersion = -1;
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_MERIDIAN, m_nMeridian, m_nWay | UI_MERIDIAN_TIPS_ONLY);
}

int KUiMeridianConfirm::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_BUTTON_CLICK)
	{
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnClose)
		{
			CloseWindow();
			return 0;
		}
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnOk || uParam == (unsigned int)(KWndWindow*)&m_BtnOkOnlyOneWay)
		{
			g_DebugLog("[kinh mach] gui xung mach %d cach %d", m_nMeridian, m_nWay);
			if (g_pCoreShell)
				g_pCoreShell->OperationRequest(GOI_MERIDIAN, m_nMeridian, m_nWay);
			CloseWindow();
			return 0;
		}
		for (int i = 0; i < MERIDIAN_CONFIRM_WAYS; i++)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_WayBox[i])
			{
				SelectWay(i);
				return 0;
			}
		}
	}
	return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
}

// Chu goi y tu goi s2c_meridian: chi nhan goi dung mach + cach dang hien (goi cu co the ve sau).
void KUiMeridianConfirm::PaintWindow()
{
	KUiMeridianInfo Info;
	memset(&Info, 0, sizeof(Info));
	int nVersion = g_pCoreShell ? g_pCoreShell->GetGameData(GDI_MERIDIAN_INFO, (unsigned int)&Info, 0) : 0;
	if (nVersion != m_nVersion && Info.nMeridian == m_nMeridian && Info.nWay == m_nWay)
	{
		m_nVersion = nVersion;
		m_Info.SetText(Info.szTips);
	}
	KWndShowAnimate::PaintWindow();
}

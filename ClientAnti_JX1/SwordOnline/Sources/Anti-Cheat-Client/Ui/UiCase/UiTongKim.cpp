/*****************************************************************************************
//UiTongKim
//Author: Hoang
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiTongKim.h"
#include "../ShortcutKey.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/gamedatadef.h"
#include "../UiBase.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI_TK		"UiTongKim.ini"

KUiTongKim* KUiTongKim::m_pSelf = NULL;


KUiTongKim* KUiTongKim::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


KUiTongKim* KUiTongKim::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTongKim;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiTongKim::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

void KUiTongKim::Initialize()
{
	AddChild(&m_TimeText);
	AddChild(&m_DiemText);
	AddChild(&m_PlayerText);
	AddChild(&m_NpcText);
	AddChild(&m_XepHangText);

	AddChild(&m_Time);
	AddChild(&m_Diem);
	AddChild(&m_Player);
	AddChild(&m_Npc);
	AddChild(&m_XepHang);


	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

void KUiTongKim::LoadScheme(const char* pScheme)
{
	if(m_pSelf)
	{
    	char		Buff[128];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_TK);
    	if (Ini.Load(Buff))
    	{
	m_pSelf->Init(&Ini, "Main");
	m_pSelf->m_TimeText   .Init(&Ini, "TimeText");
	m_pSelf->m_DiemText  .Init(&Ini, "DiemText");
	m_pSelf->m_PlayerText    .Init(&Ini, "PlayerText");
	m_pSelf->m_NpcText   .Init(&Ini, "NpcText");
	m_pSelf->m_XepHangText  .Init(&Ini, "XepHangText");

	m_pSelf->m_TimeText   .SetText("Thêi gian cßn:");
	m_pSelf->m_DiemText  .SetText("TÝch lòy:");
	m_pSelf->m_PlayerText    .SetText("GiÕt ng­êi:");
	m_pSelf->m_NpcText   .SetText("GiÕt npc:");
	m_pSelf->m_XepHangText  .SetText("XÕp h¹ng:");

	m_pSelf->m_Time   .Init(&Ini, "Time");
	m_pSelf->m_Diem  .Init(&Ini, "Diem");
	m_pSelf->m_Player    .Init(&Ini, "Player");
	m_pSelf->m_Npc   .Init(&Ini, "Npc");
	m_pSelf->m_XepHang  .Init(&Ini, "XepHang");
		}
	}

}


void KUiTongKim::UpdateData()
{
	m_nTime = g_pCoreShell->GetGameData(GDI_PLAYER_TK_TIME, 0, 0);
	m_Time.SetIntText(m_nTime);
	m_nDiem = g_pCoreShell->GetGameData(GDI_PLAYER_TK_POINT, 0, 0);
	m_Diem.SetIntText(m_nDiem);
	m_nPlayer = g_pCoreShell->GetGameData(GDI_PLAYER_TK_PLAYER, 0, 0);
	m_Player.SetIntText(m_nPlayer);
	m_nNpc = g_pCoreShell->GetGameData(GDI_PLAYER_TK_NPC, 0, 0);
	m_Npc.SetIntText(m_nNpc);
	m_nXepHang = g_pCoreShell->GetGameData(GDI_PLAYER_TK_XEPHANG, 0, 0);
	m_XepHang.SetIntText(m_nXepHang);
}

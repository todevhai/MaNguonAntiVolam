#include "Util.h"
#include "SkillState.h"
#include "KPlayerSafe.h"
#include "Protect.h"
#include "../../Engine/src/Text.h"
#include "Elem/WndMessage.h"
#include "AutoPlay.h"
#include "Import.h"

#define 	SCHEME_INI_AUTO_SHEET			"\\Ui\\Ui3\\AutoPlay.ini"
#define		LIFE_TIME_DISTANCE_DEFAULT		50

using namespace std;

extern iRepresentShell* g_pRepresentShell;

void KUiFighting::Initialize()
{

	/*AddChild(&m_EatLife);
	AddChild(&m_EatMana);
	AddChild(&m_EatTownPortal);
	AddChild(&m_AttRadius);
	AddChild(&m_Distance);*/
	//AddChild(&m_Life);
	//AddChild(&m_Mana);
	//AddChild(&m_Portal);
	//AddChild(&m_Radius);
	//AddChild(&m_Dstnce);
	/*AddChild(&m_NhatTienText);
	AddChild(&m_NhatTien);
	AddChild(&m_NhatItemText);
	AddChild(&m_NhatItem);
	AddChild(&m_NhatTaskText);
	AddChild(&m_NhatTask);*/
	m_uLifeTime = 0;
	m_uManaTime = 0;
}
//Cai` dat. file .ini
void KUiFighting::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		KWndPage::Init(&Ini, "Fighting");
		m_EatLife.Init(&Ini, "LifeText");
		m_EatMana.Init(&Ini, "ManaText");
		m_EatTownPortal.Init(&Ini, "PortalText");
		m_AttRadius.Init(&Ini, "RadiusText");
		m_Distance.Init(&Ini, "DistanceText");
	/*	m_Life.Init(&Ini, "LifeInput");
		m_Mana.Init(&Ini, "ManaInput");
		m_Portal.Init(&Ini, "PortalInput");
		m_Radius.Init(&Ini, "RadiusInput");
		m_Dstnce.Init(&Ini, "DistanceInput");*/

	/*	m_Life.SetIntText(500);
		m_Mana.SetIntText(100);
		m_Portal.SetIntText(100);
		m_Radius.SetIntText(1024);
		m_Dstnce.SetIntText(75);*/
		m_EatLife.SetText("Dïng m¸u khi hp d­íi");
		m_EatMana.SetText("Dïng mana khi mp d­íi");
		m_EatTownPortal.SetText("Dïng t®p khi hp d­íi");
		m_AttRadius.SetText("Ph¹m vi tÊn c«ng");
		m_Distance.SetText("Kho¶ng c¸ch");
		m_NhatTienText.Init(&Ini, "NhatTienText");
		m_NhatTienText.SetText("NhÆt tiÒn");
		m_NhatTien.Init(&Ini, "NhatTien");
		m_NhatItemText.Init(&Ini, "NhatItemText");
		m_NhatItemText.SetText("NhÆt vËt phÈm");
		m_NhatItem.Init(&Ini, "NhatItem");
		m_NhatTaskText.Init(&Ini, "NhatTaskText");
		m_NhatTaskText.SetText("NhÆt task");
		m_NhatTask.Init(&Ini, "NhatTask");
	}

}
//Ham` chay. lien tuc. (nhu hoi tho~)
void KUiFighting::Breathe()
{
	/*if (!g_pCoreShell)
		return;*/
	if (!m_bIsActived)
		return;
	if (m_uLifeTime) m_uLifeTime--;
	//--------------------------------------------------------------------------
	//int nLife = m_Life.GetIntNumber();
	//if (nLife > 0 && !m_uLifeTime)
	//{
	//	int nRet;
	//	KUiPlayerRuntimeInfo	Info;
	//	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
	//	g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);
	//	if (Info.nLife <= nLife && Info.nLifeFull > nLife)
	//		nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND, 0, false); //gui~ lenh. bom mau'
	//	else nRet = 0;
	//	if (nRet)
	//		m_uLifeTime = LIFE_TIME_DISTANCE_DEFAULT;
	//}
	//if (m_uManaTime) m_uManaTime--;
	//--------------------------------------------------------------------------
	//int nMana = m_Mana.GetIntNumber();
	//if (nMana > 0 && !m_uManaTime)
	//{
	//	int nRet;
	//	KUiPlayerRuntimeInfo	Info;
	//	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
	//	g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);
	//	if (Info.nMana <= nMana && Info.nManaFull > nMana)
	//		nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND, 1, false); //gui~ lenh. bom mana'
	//	else nRet = 0;
	//	if (nRet)
	//		m_uManaTime = LIFE_TIME_DISTANCE_DEFAULT;
	//}
	//if (m_uLifeTime) m_uLifeTime--;
	//--------------------------------------------------------------------------
	//int nPortal = m_Portal.GetIntNumber();
	//if (nPortal > 0 && !m_uLifeTime)
	//{
	//	int nRet;
	//	KUiPlayerRuntimeInfo	Info;
	//	memset(&Info, 0, sizeof(KUiPlayerRuntimeInfo));
	//	g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);
	//	if (Info.nLife <= nPortal && Info.nLifeFull > nPortal)
	//		nRet = g_pCoreShell->OperationRequest(GOI_AUTO_COMMAND, 2, false); //gui~ lenh. TDP'
	//	else nRet = 0;
	//	if (nRet)
	//		m_uLifeTime = LIFE_TIME_DISTANCE_DEFAULT;
	//}
	// more ...
}
// k¨ªch hoat
void KUiFighting::Active(BOOL bAct)
{
	m_bIsActived = bAct;
}

int KUiFighting::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		//if (uParam == (unsigned int)(KWndWindow*)&empty_sample)
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

//--------------------------------------------------------------------------
//	Chuc nang: Tu nhat do
//--------------------------------------------------------------------------
void KUiSetting::Initialize()
{
	/*AddChild(&m_AnPlayerText);
	AddChild(&m_AnNpcText);
	AddChild(&m_AnMissleText);
	AddChild(&m_AnPlayer);
	AddChild(&m_AnNpc);
	AddChild(&m_AnMissle);*/
}

void KUiSetting::LoadScheme()
{
	KIniFile	Ini;
	if (Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		KWndPage::Init(&Ini, "Setting");
		m_AnPlayerText.Init(&Ini, "AnPlayerText");
		m_AnPlayerText.SetText("Èn ng­êi ch¬i");
		m_AnPlayer.Init(&Ini, "AnPlayer");
		m_AnNpcText.Init(&Ini, "AnNpcText");
		m_AnNpcText.SetText("Èn npc");
		m_AnNpc.Init(&Ini, "AnNpc");
		m_AnMissleText.Init(&Ini, "AnMissleText");
		m_AnMissleText.SetText("Èn hiÖu øng kÜ n¨ng");
		m_AnMissle.Init(&Ini, "AnMissle");
	}
}

int KUiSetting::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_AnPlayer)
		{
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_AnNpc)
		{
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_AnMissle)
		{
		}
		break;
	default:
		return KWndPage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

KUiAutoPlay* KUiAutoPlay::m_pSelf = NULL;

KUiAutoPlay* KUiAutoPlay::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiAutoPlay;

		CreateWndPageSet((unsigned int*)m_pSelf);

		if (m_pSelf)
		{
			m_pSelf->Initialize();
			m_pSelf->m_FightPad.Show();
		}
	}
	if (m_pSelf)
	{
		//UiSoundPlay(0);
		if (*(DWORD*)((unsigned int)m_pSelf + 4) != WND_S_VISIBLE)
		{
			//m_pSelf->BringToTop();
			m_pSelf->Show();
		}
		//m_pSelf->PaintWindow();
	}
	return m_pSelf;
}

KUiAutoPlay* KUiAutoPlay::GetIfVisible()
{
	/*if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;*/
	return NULL;
}

void KUiAutoPlay::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_Style &= ~WND_S_VISIBLE;
		m_pSelf->CloseWindow(bDestroy);
	}
}

void KUiAutoPlay::Initialize()
{
	//m_BtnAutoPlay.Initialize();
	m_FightPad.Initialize();
	AddPage(&m_FightPad, &m_FightingBtn);
	m_SettingPad.Initialize();
	AddPage(&m_SettingPad, &m_SettingBtn);
	/*AddChild(&m_Close);
	AddChild(&m_Active);
	AddChild(&m_ActiveText);
	AddChild(&m_FightText);
	AddChild(&m_SettingText);
	AddChild(&m_Title);*/
	//SetObjectWndText32((unsigned int*)&m_Title);
	//b_AddChild(this, &m_Title);
	LoadScheme();
	b_Wnd_AddWindow(this, 1);
	//memset(m_szTitle, 0, sizeof(m_szTitle));
	//strcpy(m_szTitle, "Autoplay Ingame");
}

void KUiAutoPlay::LoadScheme()
{
	KIniFile	Ini;
	if (m_pSelf && Ini.Load(SCHEME_INI_AUTO_SHEET))
	{
		m_pSelf->Init(&Ini, "Main");
		//KWndText32_Init(&m_pSelf->m_Title, &Ini, "txtTitle");
		m_pSelf->m_Title.Init(&Ini, "txtTitle");
		/*m_pSelf->m_Close.Init(&Ini, "CloseBtn");
		m_pSelf->m_Active.Init(&Ini, "ActiveBtn");
		m_pSelf->m_FightingBtn.Init(&Ini, "FightingBtn");
		m_pSelf->m_SettingBtn.Init(&Ini, "SettingBtn");
		m_pSelf->m_ActiveText.Init(&Ini, "ActiveText");
		m_pSelf->m_FightText.Init(&Ini, "FightText");
		m_pSelf->m_SettingText.Init(&Ini, "SettingText");

		m_pSelf->m_FightText.SetText(" Attack");
		m_pSelf->m_SettingText.SetText("Setting");
		m_pSelf->m_ActiveText.SetText("");
		m_pSelf->m_FightPad.LoadScheme();
		m_pSelf->m_SettingPad.LoadScheme();*/
		b_ShowWndWindow(&m_pSelf->m_Title);
	}
}


void KUiAutoPlay::PaintWindow()
{
	KWndPageSet::PaintWindow();

	/*if (g_pRepresentShell)
	{
		KRUImage RUIconImage;
		RUIconImage.nType = ISI_T_SPR;
		RUIconImage.Color.Color_b.a = 255;
		RUIconImage.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
		RUIconImage.uImage = 0;
		RUIconImage.nISPosition = IMAGE_IS_POSITION_INIT;
		RUIconImage.bRenderFlag = 0;
		RUIconImage.oPosition.nX = m_pSelf->m_Left + 28;
		RUIconImage.oPosition.nY = m_pSelf->m_Top + 6;
		RUIconImage.oPosition.nZ = 0;
		RUIconImage.nFrame = 0;
		g_pRepresentShell->DrawPrimitives(1, &RUIconImage, RU_T_IMAGE, 1);

		

		if (m_pSelf->m_szTitle[0])
		{
			KOutputTextParam param;
			param.Color = 0xfffffcb2;
			param.BorderColor = 0xff000000;
			param.nX = m_pSelf->m_Left + 34;
			param.nY = m_pSelf->m_Top + 10;
			param.nZ = TEXT_IN_SINGLE_PLANE_COORD;
			param.nSkipLine = 0;
			param.nNumLine = 1;
			g_pRepresentShell->OutputRichText(16, &param, m_pSelf->m_szTitle, strlen(m_pSelf->m_szTitle));
		}
	}*/
}
int KUiAutoPlay::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		//-----------------------------------------------------------------------------------
		if (uParam == (unsigned int)(KWndWindow*)&m_Close)
		{
			CloseWindow(false);
			return 0;
		}
		//-----------------------------------------------------------------------------------
		else if (uParam == (unsigned int)(KWndWindow*)&m_Active || uParam == (unsigned int)(KWndWindow*)&m_ActiveText)
		{
			//				int nLife = m_Life.GetIntNumber();
			//				int nMana = m_Mana.GetIntNumber();
			//				int nPortal = m_Portal.GetIntNumber();
			//		if (Info.nLife <= nLife && Info.nLifeFull > nLife || Info.nMana <= nMana && Info.nManaFull > nMana || Info.nLife <= nPortal && Info.nLifeFull > nPortal)
			//		{
			m_ActiveText.CheckButton(!m_ActiveText.IsChecked());
			/*if (m_ActiveText.IsChecked())
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, MO_AUTO, 0);
			else
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, TAT_AUTO, 0);*/
			return 0;
			//		else
			//				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, ERORR_AUTO, 0);
		}
		//-----------------------------------------------------------------------------------
		break;
	}
	//return KWndPageSet::WndProc(uMsg, uParam, nParam);
	return 0;
}

void KUiAutoPlay::Breathe()
{
	BOOL bAct = m_ActiveText.IsChecked();
	m_FightPad.Breathe();
	//m_Active.CheckButton(bAct);
	m_FightPad.Active(bAct);
	//m_FightText.BringToTop();
	//m_SettingText.BringToTop();
}
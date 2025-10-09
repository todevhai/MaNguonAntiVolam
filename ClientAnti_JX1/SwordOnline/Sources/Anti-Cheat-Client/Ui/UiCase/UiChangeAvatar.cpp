/********************************************************************
File        : UiCAvatareAvatar.cpp
*********************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"
#include "GameDataDef.h"

#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"

#include "../UiBase.h"
#include "../UiSoundSetting.h"

#include "UiMarket.h"
#include "UiItem.h"
#include "UiChangeAvatar.h"


#define CHANGE_AVATAR "UiChangeAvatar.ini"

KUiChangeAvatar* KUiChangeAvatar::m_pSelf = NULL;

extern iCoreShell *g_pCoreShell;


KUiChangeAvatar* KUiChangeAvatar::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiChangeAvatar;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}


/*********************************************************************
**********************************************************************/
KUiChangeAvatar* KUiChangeAvatar::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


/*********************************************************************
**********************************************************************/
void KUiChangeAvatar::CloseWindow(bool bDestory)
{
	if (m_pSelf)
	{
		m_pSelf->Hide();
		if (bDestory)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}


/*********************************************************************
**********************************************************************/
void KUiChangeAvatar::Initialize()
{
	AddChild(&m_DoneBtn);
	AddChild(&m_DongBtn);
	AddChild(&m_Avatar1);
	AddChild(&m_Avatar2);
	AddChild(&m_Avatar3);
	AddChild(&m_Avatar4);
	AddChild(&m_Avatar5);
	AddChild(&m_Avatar6);
	AddChild(&m_Avatar7);
	AddChild(&m_Avatar8);
	AddChild(&m_Avatar9);
	AddChild(&m_Avatar10);
	AddChild(&m_Avatar11);
	AddChild(&m_Avatar12);
	AddChild(&m_Avatar13);
	AddChild(&m_Avatar14);
	AddChild(&m_Avatar15);


	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

void KUiChangeAvatar::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, CHANGE_AVATAR);
		if (Ini.Load(Buff))
			m_pSelf->LoadScheme(&Ini);	
	}
}
/*********************************************************************
**********************************************************************/

void KUiChangeAvatar::LoadScheme(class KIniFile* pIni)
{
		    Init(pIni, "Main");
			m_DongBtn.Init(pIni, "DongBtn");
			m_DoneBtn.Init(pIni, "DoneBtn");			
	if (g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, 0))
	{	
			m_Avatar1.Init(pIni, "AvatarNam1");
			m_Avatar2.Init(pIni, "AvatarNam2");
			m_Avatar3.Init(pIni, "AvatarNam3");
			m_Avatar4.Init(pIni, "AvatarNam4");
			m_Avatar5.Init(pIni, "AvatarNam5");
			m_Avatar6.Init(pIni, "AvatarNam6");
			m_Avatar7.Init(pIni, "AvatarNam7");
			m_Avatar8.Init(pIni, "AvatarNam8");
			m_Avatar9.Init(pIni, "AvatarNam9");
			m_Avatar10.Init(pIni, "AvatarNam10");
			m_Avatar11.Init(pIni, "AvatarNam11");
			m_Avatar12.Init(pIni, "AvatarNam12");
	}
	else
	{
			m_Avatar1.Init(pIni, "AvatarNu1");
			m_Avatar2.Init(pIni, "AvatarNu2");
			m_Avatar3.Init(pIni, "AvatarNu3");
			m_Avatar4.Init(pIni, "AvatarNu4");
			m_Avatar5.Init(pIni, "AvatarNu5");
			m_Avatar6.Init(pIni, "AvatarNu6");
			m_Avatar7.Init(pIni, "AvatarNu7");
			m_Avatar8.Init(pIni, "AvatarNu8");
			m_Avatar9.Init(pIni, "AvatarNu9");
			m_Avatar10.Init(pIni, "AvatarNu10");
			m_Avatar11.Init(pIni, "AvatarNu11");
			m_Avatar12.Init(pIni, "AvatarNu12");
			m_Avatar13.Init(pIni, "AvatarNu13");
			m_Avatar14.Init(pIni, "AvatarNu14");
			m_Avatar15.Init(pIni, "AvatarNu15");
		}
	}


int KUiChangeAvatar::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)&m_DongBtn)
			Hide();
		else if (uParam == (unsigned int)&m_DoneBtn)
			Hide();
			break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
    return 1;
}

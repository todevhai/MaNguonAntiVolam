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
	for (int i = 0; i < defMAX_AVATAR; i++)
		AddChild(&m_Cell[i]);


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
	// Mot muc [Avatar1]..[AvatarNN] cho moi anh. Bo anh CHUNG cho nam va nu nen
	// khong con tach hai bo muc theo phai nhu ban cu.
	for (int i = 0; i < defMAX_AVATAR; i++)
	{
		char szSect[32];
		sprintf(szSect, "Avatar%d", i + 1);
		m_Cell[i].Init(pIni, szSect);
	}
}


int KUiChangeAvatar::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)&m_DongBtn || uParam == (unsigned int)&m_DoneBtn)
		{
			Hide();
			break;
		}
		// Bam vao mot o chan dung: gui len may chu, KHONG tu doi tai cho.
		// May chu luu roi bao lai bang s2c_setavatar, bang F3 ve theo cai do -
		// nen thu hien tren man hinh luon la thu that su duoc luu.
		for (int i = 0; i < defMAX_AVATAR; i++)
		{
			if (uParam == (unsigned int)&m_Cell[i])
			{
				if (g_pCoreShell)
					g_pCoreShell->OperationRequest(GOI_SET_AVATAR, (unsigned int)(i + 1), 0);
				Hide();
				break;
			}
		}
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
    return 1;
}

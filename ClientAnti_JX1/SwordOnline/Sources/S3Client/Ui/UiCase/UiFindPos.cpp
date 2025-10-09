/*****************************************************************************************
//	Hop thoai dung mo khoa pass ruong
//	Copyright : PTTK
//	Author	:   Dra (NMT)
//	CreateTime:	2012/8/23
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiFindPos.h"
#include "UiItem.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include <crtdbg.h>

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_ITEM	"UiFindPos.ini"

KUiFindPos* KUiFindPos::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	Kiem tra xem hop thoai co dang visible hay khong, neu visible thi tra ve con tro hop thoai
//--------------------------------------------------------------------------
KUiFindPos* KUiFindPos::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	Mo hop thoai, tra ve con tro hop thoai
//--------------------------------------------------------------------------
KUiFindPos* KUiFindPos::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiFindPos;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
		Wnd_GameSpaceHandleInput(false);
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
// Dong hop thoai 
//--------------------------------------------------------------------------
void KUiFindPos::CloseWindow()
{
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

// -------------------------------------------------------------------------
// Khoi tao hop thoai
// -------------------------------------------------------------------------
void KUiFindPos::Initialize()
{
	AddChild(&m_InfoText);
	AddChild(&m_Text);
	AddChild(&m_X);
	AddChild(&m_Y);
	AddChild(&m_OKBtn);
	AddChild(&m_CancelBtn);
	
	char schemePath[256];
	g_UiBase.GetCurSchemePath(schemePath, 256);
	LoadScheme(schemePath);

	//pass = "";
	Wnd_AddWindow(this);
}

// -------------------------------------------------------------------------
// Tao layout hop thoai
// -------------------------------------------------------------------------
void KUiFindPos::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ITEM);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_X.Init(&Ini, "PosXInput");
		m_pSelf->m_Y.Init(&Ini, "PosYInput");
		m_pSelf->m_OKBtn.Init(&Ini, "OkBtn");
		m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
		m_pSelf->m_Text.Init(&Ini, "Text");
		m_pSelf->m_Text.SetText("/");
		m_pSelf->m_InfoText.Init(&Ini, "InfoText");
		m_pSelf->m_InfoText.SetText("Nh藀 t鋋 ");
	}
}
/*********************************************************************
* 功能：窗口函数
**********************************************************************/
int KUiFindPos::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_CancelBtn)
		{
			CloseWindow();
		}
		else if(uParam == (unsigned int)&m_OKBtn)
		{
			OnDone();
		}
		break;

	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
    return 1;
}


/*********************************************************************
* 功能：正中邪CheckBox的管理函数
**********************************************************************/


/*********************************************************************
* 功能：响应确认按钮被按下
**********************************************************************/
void KUiFindPos::OnDone()
{
	CloseWindow();
}
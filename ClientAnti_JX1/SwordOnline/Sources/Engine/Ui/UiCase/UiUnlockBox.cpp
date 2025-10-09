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
#include "UiUnlockBox.h"
#include "UiItem.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include <crtdbg.h>

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_ITEM	"UiUnlockBox.ini"

KUiUnlockBox* KUiUnlockBox::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	Kiem tra xem hop thoai co dang visible hay khong, neu visible thi tra ve con tro hop thoai
//--------------------------------------------------------------------------
KUiUnlockBox* KUiUnlockBox::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	Mo hop thoai, tra ve con tro hop thoai
//--------------------------------------------------------------------------
KUiUnlockBox* KUiUnlockBox::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiUnlockBox;
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
void KUiUnlockBox::CloseWindow()
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
void KUiUnlockBox::Initialize()
{
//	AddChild(&m_Text);
	AddChild(&m_Password);
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
void KUiUnlockBox::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ITEM);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_Password.Init(&Ini, "PWfield");
		m_pSelf->m_OKBtn.Init(&Ini, "OKButton");
		m_pSelf->m_CancelBtn.Init(&Ini, "CloseButton");
		m_pSelf->m_Text.Init(&Ini, "Text");
//		m_pSelf->m_Text.SetText("NhËp mËt khÈu");
	}
}

// -------------------------------------------------------------------------------
// Gui goi tin yeu cau mo khoa ruong len server
// -------------------------------------------------------------------------------
void KUiUnlockBox::OnOK()
	{
		OnCheckInput();
		int inputedPassword = m_Password.GetIntNumber();
		g_pCoreShell->OperationRequest(GOI_CP_UNLOCK, inputedPassword, 0);
		
		CloseWindow();
		return;
	}

// -------------------------------------------------------------------------
// Xu ly khi hop thoai dang mo
// -------------------------------------------------------------------------
int KUiUnlockBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OKBtn)
			OnOK();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			CloseWindow();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOK();
		}
		else if (uParam == VK_ESCAPE)
		{
			CloseWindow();
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN &&
			uParam == (unsigned int)(KWndWindow*)&m_Password)
		{
			OnOK();
			return 1;
		}
		break;
	default:

		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
    return 1;
}

// -------------------------------------------------------------------------
// Kiem tra gia tri mat khau ruong nguoi dung nhap vao (Bat buoc phai la int)
// -------------------------------------------------------------------------
void KUiUnlockBox::OnCheckInput()
{
	int nPass = m_Password.GetIntNumber();
	if (nPass < 0)
		nPass = 0;
	else if (nPass > CHEST_PW_MAX_VALUE)
		nPass = CHEST_PW_MAX_VALUE;
	char	szBuff1[16], szBuff2[16];
	itoa(nPass, szBuff1, 10);
	m_Password.GetText(szBuff2, sizeof(szBuff2), true);
	if (strcmp(szBuff1, szBuff2))
		m_Password.SetIntText(nPass);
}

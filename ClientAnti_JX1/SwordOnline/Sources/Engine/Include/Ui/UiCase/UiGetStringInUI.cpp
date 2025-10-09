// -------------------------------------------------------------------------
//	UiGetStringInUI.cpp
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiGetStringInUI.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*	g_pCoreShell;

#define	SCHEME_INI 	"UiGetStringInUI.ini"


KUiGetStringInUI* KUiGetStringInUI::m_pSelf = NULL;

KUiGetStringInUI::KUiGetStringInUI()
{
	m_pRequester = NULL;
}

//--------------------------------------------------------------------------
//
//--------------------------------------------------------------------------
KUiGetStringInUI* KUiGetStringInUI::OpenWindow(const char* pszTitle,
						const char* pszInitString, unsigned int uParam,
 						int nMinLen, int nMaxLen, BOOL bNum)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiGetStringInUI;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_nMinLen = nMinLen;
		m_pSelf->m_nMaxLen = nMaxLen;
		m_pSelf->m_uRequesterParam = uParam;
		if (pszTitle)
			m_pSelf->m_Title.SetText(pszTitle);
		else
			m_pSelf->m_Title.SetText("");
		if (pszInitString)
			m_pSelf->m_StringEdit.SetText(pszInitString);
		else
			m_pSelf->m_StringEdit.ClearText();

		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	m_pSelf->m_bNum = bNum;
	return m_pSelf;
}

void KUiGetStringInUI::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_pRequester = NULL;
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

KUiGetStringInUI*	KUiGetStringInUI::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

void KUiGetStringInUI::Show()
{
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetFocusWnd(&m_StringEdit);
	Wnd_SetExclusive((KWndWindow*)this);
}

void KUiGetStringInUI::Hide()
{
	Wnd_SetFocusWnd(NULL);
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiGetStringInUI::Initialize()
{
	AddChild(&m_Title);
	AddChild(&m_StringEdit);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiGetStringInUI::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_Title.Init(&Ini, "Title");
			m_pSelf->m_StringEdit.Init(&Ini, "StringInput");
			m_pSelf->m_OkBtn.Init(&Ini, "OkBtn");
			m_pSelf->m_CancelBtn.Init(&Ini, "CancelBtn");
		}
	}
}

//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
int KUiGetStringInUI::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	case WND_N_EDIT_SPECIAL_KEY_DOWN:
		if (nParam == VK_RETURN &&
			uParam == (unsigned int)(KWndWindow*)&m_StringEdit)
		{
			OnOk();
			return 1;
		}
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}
//--------------------------------------------------------------------------
//	
//--------------------------------------------------------------------------
void KUiGetStringInUI::OnOk()
{
		int inputedPassword = m_StringEdit.GetIntNumber();
		g_pCoreShell->OperationRequest(GOI_STRINGBOX, inputedPassword, 0);
		CloseWindow(true);
		return;
}


void KUiGetStringInUI::OnCancel()
{
	CloseWindow(true);
}

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
#include "UiMiniMap.h"	/* dat hop nhap ngay duoi ban do nho */
#include "UiItem.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include <crtdbg.h>
#include <stdlib.h>	/* atoi */

extern iCoreShell*		g_pCoreShell;

/* Diem dang cam co tren ban do nho, theo toa do KHONG GIAN.
   Dinh nghia trong UiMiniMap.cpp. */
extern int	g_nDichSpaceX;
extern int	g_nDichSpaceY;

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
		KUiSceneTimeInfo Info;
		memset(&Info, 0, sizeof(Info));
		if (g_pCoreShell)
		{
			g_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Info, 0);
			m_pSelf->m_X.SetIntText(Info.nScenePos0 / 8);
			m_pSelf->m_Y.SetIntText(Info.nScenePos1 / 8);
		}
		m_pSelf->m_InfoText.SetText("Xin nh藀 t鋋  mu鑞 n");
		KUiMiniMap* pMap = KUiMiniMap::GetIfVisible();
		if (pMap)
		{
			int nMapX, nMapY, nMapW, nMapH, nW, nH;
			pMap->GetAbsolutePos(&nMapX, &nMapY);
			pMap->GetSize(&nMapW, &nMapH);
			m_pSelf->GetSize(&nW, &nH);
			m_pSelf->SetPosition(nMapX + (nMapW - nW) / 2, nMapY + nMapH + 2);
		}
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
			OnOK();
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
/* Nguoi choi nhap theo O BAN DO - dung don vi hien o dong "Dang o" khi mo
   hop thoai - con GotoWhere nhan toa do khong gian: x/256 va y/512.
   mode 20 = da la toa do khong gian VA phai tim duong tranh vat can. */
void KUiFindPos::OnOK()
{
	char szX[16], szY[16];
	szX[0] = 0;
	szY[0] = 0;
	m_X.GetText(szX, sizeof(szX), true);
	m_Y.GetText(szY, sizeof(szY), true);
	int nO_X = atoi(szX);
	int nO_Y = atoi(szY);
	if (nO_X > 0 && nO_Y > 0 && g_pCoreShell)
	{
		g_nDichSpaceX = nO_X * 256;
		g_nDichSpaceY = nO_Y * 512;
		g_pCoreShell->GotoWhere(g_nDichSpaceX, g_nDichSpaceY, 10);
	}
}

void KUiFindPos::OnDone()
{
	CloseWindow();
}
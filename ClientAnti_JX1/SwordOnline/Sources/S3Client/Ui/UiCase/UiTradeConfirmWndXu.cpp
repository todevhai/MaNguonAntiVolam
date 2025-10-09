// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiTradeConfirmWndXu.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiTradeConfirmWnd.ini"

KUiTradeConfirmXu* KUiTradeConfirmXu::m_pSelf = NULL;

//--------------------------------------------------------------------------

//--------------------------------------------------------------------------
KUiTradeConfirmXu* KUiTradeConfirmXu::OpenWindow(int nNumber,KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo, TRADE_CONFIRM_ACTION eAction)
{
	if (pObj == NULL || pPriceInfo == NULL)
		return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTradeConfirmXu;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->m_BuyImg.Hide();

			if (eAction == TCA_BUY)
				m_pSelf->m_BuyImg.Show();
			int nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
			m_pSelf->m_OkBtn.Enable(nHoldMoney >= pPriceInfo->nPriceXu);
			m_pSelf->m_Money.SetTextColor((nHoldMoney >= pPriceInfo->nPriceXu) ?
				m_pSelf->m_uNormalPriceColor : m_pSelf->m_uNotEnoughMoneyPriceColor);
		
		m_pSelf->m_ItemInfo = *pObj;
		m_pSelf->m_PriceInfo = *pPriceInfo;
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

KUiTradeConfirmXu::KUiTradeConfirmXu()
{
	m_PriceInfo.szItemName[0] = 0;
	m_PriceInfo.nPrice = 0;
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
}


KUiTradeConfirmXu* KUiTradeConfirmXu::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：关闭销毁窗口
//--------------------------------------------------------------------------
void KUiTradeConfirmXu::CloseWindow(bool bDestroy)
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


//显示窗口
void KUiTradeConfirmXu::Show()
{
	m_ItemName.SetText(m_PriceInfo.szItemName);
	m_Money.SetXuText(m_PriceInfo.nPriceXu);
	int Left, Top;
	ALW_GetWndPosition(Left, Top, m_Width, m_Height);
	SetPosition(Left, Top);
	KWndImage::Show();
	Wnd_SetExclusive((KWndWindow*)this);
}

//隐藏窗口
void KUiTradeConfirmXu::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
int KUiTradeConfirmXu::Initialize()
{
	AddChild(&m_ItemName);
	//AddChild(&m_BuyNumber);
	//AddChild(&m_Increase);
	//AddChild(&m_Decrease);
	AddChild(&m_Money);
	AddChild(&m_BuyImg);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	return true;
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiTradeConfirmXu::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_ItemName.Init(&Ini, "ItemName");

		m_Money.Init(&Ini, "Price");			
		Ini.GetString("Price", "Color", "", Buff, sizeof(Buff));
		m_uNormalPriceColor = GetColor(Buff);
		Ini.GetString("Price", "CantBuyColor", "", Buff, sizeof(Buff));
		m_uNotEnoughMoneyPriceColor = GetColor(Buff);

		m_BuyImg.Init(&Ini, "BuyImg");
		m_OkBtn.Init(&Ini, "OkBtn");
		m_CancelBtn.Init(&Ini, "CancelBtn");
		m_Money.SetXuText(m_PriceInfo.nPriceXu);
		m_BuyNumber.Init(&Ini, "BuyNumber");
		m_Increase.Init(&Ini, "Increase");
		m_Decrease.Init(&Ini, "Decrease");
	}
}

//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
int KUiTradeConfirmXu::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
		{
			if (m_BuyImg.IsVisible())
				OnBuy();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		else if (uParam == (unsigned int)(KWndWindow*)&m_Increase && (m_BuyImg.IsVisible() || m_BreakImg.IsVisible()) && !KUiPlayerShop::GetIfVisible())
		{
			m_BuyNumber++;
			if (m_BuyNumber > 30 || m_BuyNumber <= 1)
				m_BuyNumber = 1;
			m_Money.SetXuText(m_PriceInfo.nPrice * m_BuyNumber);
			int nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
			m_Money.SetTextColor((nHoldMoney >= m_PriceInfo.nPrice * m_BuyNumber) ?
				m_uNormalPriceColor : m_uNotEnoughMoneyPriceColor);
			m_Number.SetIntText(m_BuyNumber);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Decrease && (m_BuyImg.IsVisible() || m_BreakImg.IsVisible()) && !KUiPlayerShop::GetIfVisible())
		{
			m_BuyNumber--;
			if (m_BuyNumber > 30 || m_BuyNumber <= 1)
				m_BuyNumber = 30;
			m_Money.SetXuText(m_PriceInfo.nPrice * m_BuyNumber);
			int nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
			m_Money.SetTextColor((nHoldMoney >= m_PriceInfo.nPrice * m_BuyNumber) ?
				m_uNormalPriceColor : m_uNotEnoughMoneyPriceColor);
			m_Number.SetIntText(m_BuyNumber);
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			if (m_BuyImg.IsVisible())
			{
				if (m_BuyImg.IsDisable())
					OnCancel();
				else
					OnBuy();
			}
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	功能：响应点击买按钮
//--------------------------------------------------------------------------
void KUiTradeConfirmXu::OnBuy()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}

//--------------------------------------------------------------------------
//	功能：响应点击卖按钮
//-------------------------------------------------------------------------


void KUiTradeConfirmXu::OnCancel()
{
	CloseWindow(false);
}
//so luong mua ban

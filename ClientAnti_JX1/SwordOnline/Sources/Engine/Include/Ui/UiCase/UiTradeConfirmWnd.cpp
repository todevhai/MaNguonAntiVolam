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
#include "UiTradeConfirmWnd.h"
#include "../UiSoundSetting.h"
#include "UiPlayerShop.h"
#include "../Elem/WndText.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiTradeConfirmWnd.ini"

KUiTradeConfirm* KUiTradeConfirm::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiTradeConfirm* KUiTradeConfirm::OpenWindow(KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo, TRADE_CONFIRM_ACTION eAction,int uId)
{
	if (pObj == NULL || pPriceInfo == NULL)
		return NULL;
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiTradeConfirm;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->m_BuyImg.Hide();
		m_pSelf->m_SaleImg.Hide();
		m_pSelf->m_RepairImg.Hide();
		m_pSelf->m_BreakImg.Hide();
		if (eAction == TCA_SALE)
		{
			m_pSelf->m_SaleImg.Show();
			m_pSelf->m_OkBtn.Enable(true);
			m_pSelf->m_Money.SetTextColor(m_pSelf->m_uNormalPriceColor);
		}
		else if (eAction == TCA_BREAK)
		{
			m_pSelf->m_BreakImg.Show();
			m_pSelf->m_OkBtn.Enable(true);
		}
		else
		{
			if (eAction == TCA_BUY)
			{
				m_pSelf->m_BuyImg.Show();
			}
			else
				m_pSelf->m_RepairImg.Show();
				

			int nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
			m_pSelf->m_OkBtn.Enable(nHoldMoney >= pPriceInfo->nPrice);
			m_pSelf->m_Money.SetTextColor((nHoldMoney >= pPriceInfo->nPrice) ?
				m_pSelf->m_uNormalPriceColor : m_pSelf->m_uNotEnoughMoneyPriceColor);
		}
		m_pSelf->m_ItemInfo = *pObj;
		m_pSelf->m_PriceInfo = *pPriceInfo;
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	m_pSelf->m_uId = uId;
	return m_pSelf;
}

KUiTradeConfirm::KUiTradeConfirm()
{
	m_PriceInfo.szItemName[0] = 0;
	m_PriceInfo.nPrice = 0;
	m_uId = 0;
	m_ItemInfo.Obj.uGenre = CGOG_NOTHING;
}

//如果窗口正被显示，则返回实例指针
KUiTradeConfirm* KUiTradeConfirm::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：关闭销毁窗口
//--------------------------------------------------------------------------
void KUiTradeConfirm::CloseWindow(bool bDestroy)
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
void KUiTradeConfirm::Show()
{
    m_Increase.IsVisible();
    m_ItemName.SetText(m_PriceInfo.szItemName);
    m_Money.SetMoneyText(m_PriceInfo.nPrice);
    m_BuyNumber = 1;
    m_Mark = 0;
    m_Number.SetIntText(m_BuyNumber);
    int Left, Top;
    ALW_GetWndPosition(Left, Top, m_Width, m_Height);
    SetPosition(Left, Top);
    KWndImage::Show();
    Wnd_SetExclusive((KWndWindow*)this);

}

//隐藏窗口
void KUiTradeConfirm::Hide()
{
	Wnd_ReleaseExclusive((KWndWindow*)this);
	KWndImage::Hide();
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
int KUiTradeConfirm::Initialize()
{
	AddChild(&m_ItemName);
	AddChild(&m_Money);
	AddChild(&m_BuyImg);
	AddChild(&m_SaleImg);
	AddChild(&m_RepairImg);
	AddChild(&m_BreakImg);
	AddChild(&m_Increase);
	AddChild(&m_Decrease);
	AddChild(&m_Number);
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
void KUiTradeConfirm::LoadScheme(const char* pScheme)
{
    char        Buff[128];
    KIniFile    Ini;
    sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
    if (Ini.Load(Buff))
    {
        Init(&Ini, "Main");
        m_ItemName.Init(&Ini, "ItemName");
        m_Number.Init(&Ini, "BuyNumber");
        m_Money.Init(&Ini, "Price");            
        Ini.GetString("Price", "Color", "", Buff, sizeof(Buff));
        m_uNormalPriceColor = GetColor(Buff);
        Ini.GetString("Price", "CantBuyColor", "", Buff, sizeof(Buff));
        m_uNotEnoughMoneyPriceColor = GetColor(Buff);
        m_Increase.Init(&Ini,"Increase");
        m_Decrease.Init(&Ini,"Decrease");
        m_BuyImg.Init(&Ini, "BuyImg");
        m_SaleImg.Init(&Ini, "SaleImg");
        m_RepairImg.Init(&Ini, "RepairImg");
        m_OkBtn.Init(&Ini, "OkBtn");
        m_CancelBtn.Init(&Ini, "CancelBtn");
        m_Money.SetMoneyText(m_PriceInfo.nPrice);
  
    }
}
//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
int KUiTradeConfirm::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
		{
			if (m_BuyImg.IsVisible())
				OnBuy();
			else if (m_SaleImg.IsVisible())
				OnSale();
			else if (m_RepairImg.IsVisible())
				OnRepair();
			else
				OnBreak();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		else if (uParam == (unsigned int)(KWndWindow*)&m_Increase && (m_BuyImg.IsVisible() || m_BreakImg.IsVisible()) && !KUiPlayerShop::GetIfVisible())
		{
			m_BuyNumber++;
			if (m_BuyNumber > 30 || m_BuyNumber <= 1)
				m_BuyNumber = 1;
			m_Money.SetMoneyText(m_PriceInfo.nPrice * m_BuyNumber);
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
			m_Money.SetMoneyText(m_PriceInfo.nPrice * m_BuyNumber);
			int nHoldMoney = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_MONEY, 0, 0);
			m_Money.SetTextColor((nHoldMoney >= m_PriceInfo.nPrice * m_BuyNumber) ?
				m_uNormalPriceColor : m_uNotEnoughMoneyPriceColor);
			m_Number.SetIntText(m_BuyNumber);
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			if (m_SaleImg.IsVisible())
			{
				OnSale();
			}
			else if (m_BuyImg.IsVisible())
			{
				if (m_BuyImg.IsDisable())
					OnCancel();
				else
					OnBuy();
			}
			else if (m_RepairImg.IsDisable())
			{
				OnCancel();
			}
			else
			{
				OnRepair();
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
//	功能：我要呼吸
//--------------------------------------------------------------------------
void KUiTradeConfirm::Breathe()
{
m_BuyNumber=m_Number.GetIntNumber();
m_Money.SetMoneyText(m_PriceInfo.nPrice * m_BuyNumber);
	if (m_ItemInfo.Obj.uGenre == CGOG_NPCSELLITEM)
	{
		if (m_Mark > 0)
		{
			if (g_pCoreShell && (m_Mark % 3 == 0))
			{
				g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY,
					(unsigned int)(&m_ItemInfo), 0);
			}
			m_Mark--;
			if (m_Mark == 0)
			{
				CloseWindow(false);
			}
		}
		else if (m_BuyNumber <= 1)
		{
			m_Mark = 0;
		}
	}
}

//--------------------------------------------------------------------------
//	功能：响应点击买按钮
//--------------------------------------------------------------------------
void KUiTradeConfirm::OnBuy()
{
// 			if (g_pCoreShell)
// 			{
// 				g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY,
// 					(unsigned int)(&m_ItemInfo), 0);
// 			}
	if (m_ItemInfo.Obj.uGenre == CGOG_NPCSELLITEM)
			m_Mark = m_Number.GetIntNumber() * 3;
	else if (m_ItemInfo.Obj.uGenre == CGOG_PLAYERSELLITEM)
		{
			if (g_pCoreShell)
			{
				g_pCoreShell->OperationRequest(GOI_TRADE_PLAYER_BUY,
					(unsigned int)(&m_ItemInfo), m_uId);
			}
			CloseWindow(false);
		}
	
//	CloseWindow(false);
}

//--------------------------------------------------------------------------
//	功能：响应点击卖按钮
//--------------------------------------------------------------------------
void KUiTradeConfirm::OnSale()
{
//	if (m_ItemInfo.Obj.uGenre == CGOG_NPCSELLITEM)
//			m_Mark = m_Number.GetIntNumber() * 3;
//	else if (m_ItemInfo.Obj.uGenre == CGOG_PLAYERSELLITEM)
//		{
			if (g_pCoreShell)
			{
				g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
					(unsigned int)(&m_ItemInfo), m_uId);
			}
			CloseWindow(false);
//		}
}


//--------------------------------------------------------------------------
//	功能：响应点修理按钮
//--------------------------------------------------------------------------
void KUiTradeConfirm::OnRepair()
{
	if (g_pCoreShell)
	{
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,
			(unsigned int)(&m_ItemInfo), 0);
	}
	CloseWindow(false);
}


void KUiTradeConfirm::OnCancel()
{
	CloseWindow(false);
}
// fix luon cho
void KUiTradeConfirm::OnBreak()
{
	if (m_BuyNumber <= 0)
	{
		CloseWindow(false);
	}
	else
	{
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_NPC_ITEM_BREAK,
			(unsigned int)(&m_ItemInfo), m_BuyNumber);
		}
	}
	CloseWindow(false);
}
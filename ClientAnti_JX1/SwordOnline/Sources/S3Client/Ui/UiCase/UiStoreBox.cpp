/*****************************************************************************************
//	界面--储物箱界面
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-21
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiStoreBox.h"
#include "UiUnlockBox.h"
#include "UiChangePWBox.h"
#include "UiGetMoney.h"
#include "UiItem.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "UiExBox1.h"
#include "UiExBox2.h"
#include "UiExBox3.h"
#include <crtdbg.h>

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_ITEM	"UiStoreBox.ini"

KUiStoreBox* KUiStoreBox::m_pSelf = NULL;

enum WAIT_OTHER_WND_OPER_PARAM
{
	UISTOREBOX_WAIT_GETMONEY,
};

//--------------------------------------------------------------------------
//	功能：如果窗口正被显示，则返回实例指针
//--------------------------------------------------------------------------
KUiStoreBox* KUiStoreBox::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiStoreBox* KUiStoreBox::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiStoreBox;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		if (KUiItem::GetIfVisible() == NULL)
			KUiItem::OpenWindow();
		else
			UiSoundPlay(UI_SI_WND_OPENCLOSE);

		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
		Wnd_GameSpaceHandleInput(false);

	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：关闭窗口
//--------------------------------------------------------------------------
void KUiStoreBox::CloseWindow()
{
	KUiExBox1::CloseWindow();	/* roi ruong (di xa, UiShell) cung dong trang mo rong */
	if (m_pSelf)
	{
		Wnd_GameSpaceHandleInput(true);
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

// -------------------------------------------------------------------------
// 功能	: 初始化
// -------------------------------------------------------------------------
void KUiStoreBox::Initialize()
{
	AddChild(&m_Money);
	AddChild(&m_GetMoneyBtn);
	AddChild(&m_CloseBtn);
	AddChild(&m_ItemBox);
	AddChild(&m_UnlockBtn);
	AddChild(&m_ChangePWBtn);
	AddChild(&m_BtnBox);
	m_ItemBox.SetContainerId((int)UOC_STORE_BOX);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	m_nMoney = 0;
	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	功能：构造函数
//--------------------------------------------------------------------------
void KUiStoreBox::UpdateData()
{
	m_ItemBox.Clear();
	m_nMoney = 0;
	m_Money.SetText("0");

	KUiObjAtRegion* pObjs = NULL;

	int nCount = g_pCoreShell->GetGameData(GDI_ITEM_IN_STORE_BOX, 0, 0);
	if (nCount == 0)
		return;

	if (pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount))
	{
		g_pCoreShell->GetGameData(GDI_ITEM_IN_STORE_BOX, (unsigned int)pObjs, nCount);//单线程执行，nCount值不变
		for (int i = 0; i < nCount; i++)
			if (pObjs[i].Obj.uGenre == CGOG_MONEY || pObjs[i].Region.v < REPOSITORY_ROOM_HEIGHT)	/* chi trang 0 */
				UpdateItem(&pObjs[i], true);
		free(pObjs);
		pObjs = NULL;
	}
}

// -------------------------------------------------------------------------
// 功能	: 物品变化更新
// -------------------------------------------------------------------------
void KUiStoreBox::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem && pItem->Obj.uGenre != CGOG_MONEY && pItem->Region.v >= REPOSITORY_ROOM_HEIGHT)
	{	/* mon o trang mo rong */
		if (KUiExBox1::GetIfVisible())
			KUiExBox1::GetIfVisible()->UpdateItem(pItem, bAdd);
		return;
	}
	if (pItem == NULL && KUiExBox1::GetIfVisible())
		KUiExBox1::GetIfVisible()->UpdateItem(NULL, bAdd);
	if (pItem)
	{
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
		if (pItem->Obj.uGenre != CGOG_MONEY)
		{
			KUiDraggedObject Obj;
			Obj.uGenre = pItem->Obj.uGenre;
			Obj.uId = pItem->Obj.uId;
			Obj.DataX = pItem->Region.h;
			Obj.DataY = pItem->Region.v;
			Obj.DataW = pItem->Region.Width;
			Obj.DataH = pItem->Region.Height;
			if (bAdd)
				m_ItemBox.AddObject(&Obj, 1);
			else
				m_ItemBox.RemoveObject(&Obj);
		}
		else
		{
			m_nMoney = pItem->Obj.uId;
			m_Money.SetMoneyText(m_nMoney);
		}
	}
	else
		UpdateData();
}

// -------------------------------------------------------------------------
// 功能	: 载入界面方案
// -------------------------------------------------------------------------
void KUiStoreBox::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ITEM);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_Money.Init(&Ini, "Money");
		m_pSelf->m_GetMoneyBtn.Init(&Ini, "GetMoneyBtn");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
		m_pSelf->m_ItemBox.Init(&Ini, "ItemBox");
		m_pSelf->m_UnlockBtn.Init(&Ini, "BtnLock");
		m_pSelf->m_ChangePWBtn.Init(&Ini, "BtnPassword");
		m_pSelf->m_BtnBox.Init(&Ini, "BtnBox");
		m_pSelf->m_ItemBox.EnableTracePutPos(true);
	}
}

// -------------------------------------------------------------------------
// 功能	: 窗口函数
// -------------------------------------------------------------------------
void KUiStoreBox::PaintWindow()
{
	if (g_pCoreShell)
		m_UnlockBtn.CheckButton(!g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0));
	KWndShowAnimate::PaintWindow();
}

int KUiStoreBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_RIGHT_CLICK_ITEM:
		WithdrawBoxItem((KUiDraggedObject*)uParam);	// #3b rclick trong ruong -> lay ve tui
		break;
	case WND_N_ITEM_PICKDROP:
		OnItemPickDrop((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
		{	CloseWindow();
			KUiExBox1::CloseWindow();
			KUiExBox2::CloseWindow();
			KUiExBox3::CloseWindow();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_GetMoneyBtn)
		{
			KUiGetMoney::OpenWindow(0, m_nMoney, this, UISTOREBOX_WAIT_GETMONEY, &m_Money);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_BtnBox)
		{	/* bat/tat cua so Mo rong ruong - moi trang deu mo san */
			if (KUiExBox1::GetIfVisible())
				KUiExBox1::CloseWindow();
			else
				KUiExBox1::OpenWindow();
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_UnlockBtn)
		{
			if (g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0))
			{
				// Ruong dang mo - Khoa ruong lai
				g_pCoreShell->OperationRequest(GOI_CP_LOCK, 0, 0);
			}
			else
				
			{   //Ruong dang khoa ~> Mo cua so nhap pass ruong
				 KUiUnlockBox::OpenWindow();
			}
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChangePWBtn)
			{
				if (g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0))
				{
					KUiChangePWBox::OpenWindow();
				}
				else
				{
					g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, CN_GH, 0);
				}
			}
		break;
	case WND_M_OTHER_WORK_RESULT:
		if (uParam == UISTOREBOX_WAIT_GETMONEY)
			OnGetMoney(nParam);
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiStoreBox::OnGetMoney(int nMoney)
{
	if (nMoney > 0)
	{
		g_pCoreShell->OperationRequest(GOI_MONEY_INOUT_STORE_BOX,
			false, nMoney);
	}
}

BOOL KUiStoreBox::DepositBagItem(KUiDraggedObject* pBagItem)
{
	if (KUiExBox1::GetIfVisible())	/* dang xem trang mo rong: cat vao trang do */
		return KUiExBox1::GetIfVisible()->DepositBagItem(pBagItem);
	if (pBagItem == NULL || pBagItem->uGenre == CGOG_NOTHING || g_pCoreShell == NULL)
		return FALSE;
	int iw = pBagItem->DataW > 0 ? pBagItem->DataW : 1;
	int ih = pBagItem->DataH > 0 ? pBagItem->DataH : 1;
	int fx = -1, fy = -1;
	if (!m_ItemBox.FindBlankCell(iw, ih, &fx, &fy))
		return FALSE;	// ruong het cho
	KUiObjAtContRegion Pick, Drop;
	Pick.Obj.uGenre = pBagItem->uGenre;
	Pick.Obj.uId = pBagItem->uId;
	Pick.Region.Width = pBagItem->DataW;
	Pick.Region.Height = pBagItem->DataH;
	Pick.Region.h = pBagItem->DataX;
	Pick.Region.v = pBagItem->DataY;
	Pick.eContainer = UOC_ITEM_TAKE_WITH;
	Drop.Obj.uGenre = pBagItem->uGenre;
	Drop.Obj.uId = pBagItem->uId;
	Drop.Region.Width = pBagItem->DataW;
	Drop.Region.Height = pBagItem->DataH;
	Drop.Region.h = fx;
	Drop.Region.v = fy;
	Drop.eContainer = UOC_STORE_BOX;
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, (unsigned int)&Pick, (int)&Drop);
	return TRUE;
}

BOOL KUiStoreBox::WithdrawBoxItem(KUiDraggedObject* pBoxItem)
{
	if (pBoxItem == NULL || pBoxItem->uGenre == CGOG_NOTHING || g_pCoreShell == NULL)
		return FALSE;
	KUiItem* pBag = KUiItem::GetIfVisible();
	if (pBag == NULL) return FALSE;	// tui phai dang mo
	int iw = pBoxItem->DataW > 0 ? pBoxItem->DataW : 1;
	int ih = pBoxItem->DataH > 0 ? pBoxItem->DataH : 1;
	int fx = -1, fy = -1;
	if (!pBag->FindBlankBagCell(iw, ih, &fx, &fy))
		return FALSE;	// tui het cho
	KUiObjAtContRegion Pick, Drop;
	Pick.Obj.uGenre = pBoxItem->uGenre;
	Pick.Obj.uId = pBoxItem->uId;
	Pick.Region.Width = pBoxItem->DataW;
	Pick.Region.Height = pBoxItem->DataH;
	Pick.Region.h = pBoxItem->DataX;
	Pick.Region.v = pBoxItem->DataY;
	Pick.eContainer = UOC_STORE_BOX;
	Drop.Obj.uGenre = pBoxItem->uGenre;
	Drop.Obj.uId = pBoxItem->uId;
	Drop.Region.Width = pBoxItem->DataW;
	Drop.Region.Height = pBoxItem->DataH;
	Drop.Region.h = fx;
	Drop.Region.v = fy;
	Drop.eContainer = UOC_ITEM_TAKE_WITH;
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, (unsigned int)&Pick, (int)&Drop);
	return TRUE;
}

void KUiStoreBox::OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	if (g_UiBase.GetStatus() != UIS_S_IDLE)
		return;
	KUiObjAtContRegion	Pick, Drop;
	KUiDraggedObject	Obj;

	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);		
		((KWndObjectMatrix*)(pPickPos->pWnd))->GetObject(
			Obj, pPickPos->h, pPickPos->v);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = Obj.DataX;
		Pick.Region.v = Obj.DataY;
		Pick.eContainer = UOC_STORE_BOX;
	}

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = pDropPos->h;
		Drop.Region.v = pDropPos->v;
		Drop.eContainer = UOC_STORE_BOX;
	}
	
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}
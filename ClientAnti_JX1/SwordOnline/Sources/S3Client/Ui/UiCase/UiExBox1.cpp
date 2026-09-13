// "Mo rong ruong" window - see UiExBox1.h for how pages map to chest rows.
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "UiExBox1.h"
#include "UiItem.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include <crtdbg.h>

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI_ITEM	"UiStoreBoxEx.ini"

KUiExBox1* KUiExBox1::m_pSelf = NULL;

KUiExBox1* KUiExBox1::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

KUiExBox1* KUiExBox1::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiExBox1;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->SelectPage(m_pSelf->m_nPage);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

// Game-space input stays with the chest window, which owns it while open.
void KUiExBox1::CloseWindow()
{
	if (m_pSelf)
	{
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

void KUiExBox1::Initialize()
{
	AddChild(&m_CloseBtn);
	AddChild(&m_ItemBox);
	for (int i = 0; i < EXBOX_PAGE_COUNT; i++)
		AddChild(&m_PageBtn[i]);
	m_ItemBox.SetContainerId((int)UOC_STORE_BOX);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

void KUiExBox1::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ITEM);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_CloseBtn.Init(&Ini, "CloseBtn");
		m_pSelf->m_ItemBox.Init(&Ini, "ItemBox");
		m_pSelf->m_ItemBox.EnableTracePutPos(true);
		for (int i = 0; i < EXBOX_PAGE_COUNT; i++)
		{
			char szSection[16];
			sprintf(szSection, "BtnPage%d", i + 1);
			m_pSelf->m_PageBtn[i].Init(&Ini, szSection);
		}
	}
}

int KUiExBox1::RowOffset() const
{
	return m_nPage * REPOSITORY_ROOM_HEIGHT;
}

void KUiExBox1::SelectPage(int nPage)
{
	if (nPage < 1 || nPage > EXBOX_PAGE_COUNT)
		nPage = 1;
	m_nPage = nPage;
	for (int i = 0; i < EXBOX_PAGE_COUNT; i++)
		m_PageBtn[i].CheckButton(i + 1 == m_nPage);
	UpdateData();
}

void KUiExBox1::UpdateData()
{
	m_ItemBox.Clear();
	int nCount = g_pCoreShell->GetGameData(GDI_ITEM_IN_STORE_BOX, 0, 0);
	if (nCount <= 0)
		return;
	KUiObjAtRegion* pObjs = (KUiObjAtRegion*)malloc(sizeof(KUiObjAtRegion) * nCount);
	if (pObjs == NULL)
		return;
	g_pCoreShell->GetGameData(GDI_ITEM_IN_STORE_BOX, (unsigned int)pObjs, nCount);	// fills nCount entries, same as KUiStoreBox::UpdateData
	int nOff = RowOffset();
	for (int i = 0; i < nCount; i++)
	{
		KUiObjAtRegion* p = &pObjs[i];
		if (p->Obj.uGenre == CGOG_MONEY || p->Region.v < nOff || p->Region.v >= nOff + REPOSITORY_ROOM_HEIGHT)
			continue;
		KUiDraggedObject Obj;
		Obj.uGenre = p->Obj.uGenre;
		Obj.uId = p->Obj.uId;
		Obj.DataX = p->Region.h;
		Obj.DataY = p->Region.v - nOff;
		Obj.DataW = p->Region.Width;
		Obj.DataH = p->Region.Height;
		m_ItemBox.AddObject(&Obj, 1);
	}
	free(pObjs);
}

void KUiExBox1::UpdateItem(KUiObjAtRegion* pItem, int bAdd)
{
	if (pItem == NULL)
	{
		UpdateData();
		return;
	}
	int nOff = RowOffset();
	if (pItem->Obj.uGenre == CGOG_MONEY || pItem->Region.v < nOff || pItem->Region.v >= nOff + REPOSITORY_ROOM_HEIGHT)
		return;		// another page
	UiSoundPlay(UI_SI_PICKPUT_ITEM);
	KUiDraggedObject Obj;
	Obj.uGenre = pItem->Obj.uGenre;
	Obj.uId = pItem->Obj.uId;
	Obj.DataX = pItem->Region.h;
	Obj.DataY = pItem->Region.v - nOff;
	Obj.DataW = pItem->Region.Width;
	Obj.DataH = pItem->Region.Height;
	if (bAdd)
		m_ItemBox.AddObject(&Obj, 1);
	else
		m_ItemBox.RemoveObject(&Obj);
}

int KUiExBox1::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_ITEM_PICKDROP:
		if (g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0))
			OnItemPickDrop((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		else
			g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, CN_GH, 0);	// "needs unlocking"
		break;
	case WND_N_RIGHT_CLICK_ITEM:
		WithdrawBoxItem((KUiDraggedObject*)uParam);
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_CloseBtn)
		{
			CloseWindow();
			return 0;
		}
		for (int i = 0; i < EXBOX_PAGE_COUNT; i++)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_PageBtn[i])
			{
				SelectPage(i + 1);
				break;
			}
		}
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiExBox1::OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	if (g_UiBase.GetStatus() != UIS_S_IDLE)
		return;
	KUiObjAtContRegion	Pick, Drop;
	KUiDraggedObject	Obj;
	int nOff = RowOffset();

	if (pPickPos)
	{
		_ASSERT(pPickPos->pWnd);
		((KWndObjectMatrix*)(pPickPos->pWnd))->GetObject(Obj, pPickPos->h, pPickPos->v);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = Obj.DataX;
		Pick.Region.v = Obj.DataY + nOff;
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
		Drop.Region.v = pDropPos->v + nOff;
		Drop.eContainer = UOC_STORE_BOX;
	}
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}

// Right click in the bag while this window is open: put the item on the page being viewed.
BOOL KUiExBox1::DepositBagItem(KUiDraggedObject* pBagItem)
{
	if (pBagItem == NULL || pBagItem->uGenre == CGOG_NOTHING || g_pCoreShell == NULL)
		return FALSE;
	int iw = pBagItem->DataW > 0 ? pBagItem->DataW : 1;
	int ih = pBagItem->DataH > 0 ? pBagItem->DataH : 1;
	int fx = -1, fy = -1;
	if (!m_ItemBox.FindBlankCell(iw, ih, &fx, &fy))
		return FALSE;	// page full
	KUiObjAtContRegion Pick, Drop;
	Pick.Obj.uGenre = Drop.Obj.uGenre = pBagItem->uGenre;
	Pick.Obj.uId = Drop.Obj.uId = pBagItem->uId;
	Pick.Region.Width = Drop.Region.Width = pBagItem->DataW;
	Pick.Region.Height = Drop.Region.Height = pBagItem->DataH;
	Pick.Region.h = pBagItem->DataX;
	Pick.Region.v = pBagItem->DataY;
	Pick.eContainer = UOC_ITEM_TAKE_WITH;
	Drop.Region.h = fx;
	Drop.Region.v = fy + RowOffset();
	Drop.eContainer = UOC_STORE_BOX;
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, (unsigned int)&Pick, (int)&Drop);
	return TRUE;
}

// Right click on an item here: move it to the first free bag cell.
BOOL KUiExBox1::WithdrawBoxItem(KUiDraggedObject* pBoxItem)
{
	if (pBoxItem == NULL || pBoxItem->uGenre == CGOG_NOTHING || g_pCoreShell == NULL)
		return FALSE;
	KUiItem* pBag = KUiItem::GetIfVisible();
	if (pBag == NULL)
		return FALSE;
	int iw = pBoxItem->DataW > 0 ? pBoxItem->DataW : 1;
	int ih = pBoxItem->DataH > 0 ? pBoxItem->DataH : 1;
	int fx = -1, fy = -1;
	if (!pBag->FindBlankBagCell(iw, ih, &fx, &fy))
		return FALSE;	// bag full
	KUiObjAtContRegion Pick, Drop;
	Pick.Obj.uGenre = Drop.Obj.uGenre = pBoxItem->uGenre;
	Pick.Obj.uId = Drop.Obj.uId = pBoxItem->uId;
	Pick.Region.Width = Drop.Region.Width = pBoxItem->DataW;
	Pick.Region.Height = Drop.Region.Height = pBoxItem->DataH;
	Pick.Region.h = pBoxItem->DataX;
	Pick.Region.v = pBoxItem->DataY + RowOffset();
	Pick.eContainer = UOC_STORE_BOX;
	Drop.Region.h = fx;
	Drop.Region.v = fy;
	Drop.eContainer = UOC_ITEM_TAKE_WITH;
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, (unsigned int)&Pick, (int)&Drop);
	return TRUE;
}

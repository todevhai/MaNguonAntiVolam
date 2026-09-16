// -------------------------------------------------------------------------
//	UiMarket.cpp - cua so Ky Tran Cac theo bo cuc jx9tn.
//	Luong: bam tab -> GOI_MARKET(tab) -> may chu doi tab ra shop -> s2c_openmarketbox ->
//	GDCNI_OPEN_MARKET -> OpenWindow -> UpdateData. Mua: nut Mua bo mon vao gio hang,
//	gio hang (KUiMarketCart) moi gui lenh mua.
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "KTabFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiMarket.h"
#include "UiMarketCart.h"
#include "UiItem.h"
#include "UiMsgCentrePad.h"
#include "UiTradeConfirmWnd.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../core/src/coreshell.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"
#include "../../../Engine/src/KDebug.h"

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI		"UiMarket.ini"
#define KTC_TYPE_FILE	"\\settings\\shop\\type.txt"

KUiMarket* KUiMarket::m_pSelf = NULL;
static char s_szTab[KTC_MAX_TAB][32];

KUiMarket::KUiMarket()
{
	m_pObjsList = NULL;
	m_nObjCount = 0;
	m_nPageCount = 0;
	m_nCurrentPage = 0;
	m_nTabCount = 0;
	m_nCurTab = 0;
	m_nXu = -1;
	m_szNapThe[0] = 0;
}

KUiMarket* KUiMarket::GetIfVisible()
{
	return m_pSelf;
}

KUiMarket* KUiMarket::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMarket;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		g_UiBase.SetStatus(UIS_S_TRADE_NPC);
		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
		if (KUiItem::GetIfVisible() == NULL)
			KUiItem::OpenWindow();
		else
			UiSoundPlay(UI_SI_WND_OPENCLOSE);
		KUiItem::OnNpcTradeMode(true);
	}
	return m_pSelf;
}

void KUiMarket::CloseWindow()
{
	if (m_pSelf)
	{
		KUiMarketCart::CloseWindow();
		KUiItem::OnNpcTradeMode(false);
		KUiTradeConfirm::CloseWindow(true);
		g_UiBase.SetStatus(UIS_S_IDLE);
		m_pSelf->Clear();
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

void KUiMarket::CancelTrade()
{
	CloseWindow();
}

void KUiMarket::Initialize()
{
	g_DebugLog("[KTC] Initialize: bat dau");
	DocTab();
	g_DebugLog("[KTC] Initialize: DocTab xong, %d tab", m_nTabCount);
	int i;
	for (i = 0; i < KTC_MAX_TAB; i++)
		AddChild(&m_Tab[i]);
	for (i = 0; i < KTC_MAX_O; i++)
	{
		// Nen o them TRUOC de ve nam duoi cac thanh phan cua o.
		AddChild(&m_O[i].Nen);
		AddChild(&m_O[i].Mon);
		AddChild(&m_O[i].Ten);
		AddChild(&m_O[i].NhanGiaGoc);
		AddChild(&m_O[i].NhanGia);
		AddChild(&m_O[i].GiaGoc);
		AddChild(&m_O[i].Gia);
		AddChild(&m_O[i].Mua);
		AddChild(&m_O[i].GiamGia);
		m_O[i].Mon.SetContainerId((int)UOC_MARKET);
		m_O[i].Mon.EnablePickPut(false);
	}
	AddChild(&m_NapThe);
	AddChild(&m_GioHang);
	AddChild(&m_PreBtn);
	AddChild(&m_NextBtn);
	AddChild(&m_PageInfo);
	AddChild(&m_Xu);
	AddChild(&m_CloseBtn);

	Wnd_AddWindow(this);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	g_DebugLog("[KTC] Initialize: nap so do %s", Scheme);
	LoadScheme(Scheme);
	g_DebugLog("[KTC] Initialize: xong");
}

/* Tab = moi dong cua type.txt (TypeName, SellID). Thu tu dong la so tab gui len may chu;
   may chu doc CUNG tep va tu doi ra shop, nen o day chi can ten. */
void KUiMarket::DocTab()
{
	m_nTabCount = 0;
	KTabFile	File;
	char		szTen[] = "TypeName";
	char		szId[] = "SellID";
	char		szRong[] = "";
	if (!File.Load(KTC_TYPE_FILE))
		return;
	int nCotTen = File.FindColumn(szTen);
	int nCotId = File.FindColumn(szId);
	for (int t = 0; nCotTen > 0 && nCotId > 0 && t < File.GetHeight() - 1 && m_nTabCount < KTC_MAX_TAB; t++)
	{
		int nSellID = 0;
		File.GetInteger(t + 2, nCotId, 0, &nSellID);
		if (nSellID < 1)
			continue;
			// GetValue ghi lpRString[dwSize] = 0 -> truyen sizeof-1 keo la tran mot byte.
		File.GetString(t + 2, nCotTen, szRong, s_szTab[m_nTabCount], sizeof(s_szTab[0]) - 1);
		m_nTabCount++;
	}
}

void KUiMarket::LoadScheme(const char* pScheme)
{
	if (m_pSelf == NULL)
		return;
	char		Buff[256];
	KIniFile	Ini;
	sprintf(Buff, "%s\\" SCHEME_INI, pScheme);
	if (!Ini.Load(Buff))
		return;

	KUiMarket* p = m_pSelf;
	p->KWndShowAnimate::Init(&Ini, "Main");
	g_DebugLog("[KTC] LoadScheme: Main xong");

	// Tab: moi tab dat noi tiep ben phai tab truoc, cach nhau [SellType] Gap diem.
	int nTabX, nTabY, nTabW, nTabH, nGap, i;
	Ini.GetInteger("SellType", "Gap", 0, &nGap);
	for (i = 0; i < KTC_MAX_TAB; i++)
	{
		p->m_Tab[i].Init(&Ini, "SellType");
		p->m_Tab[i].GetPosition(&nTabX, &nTabY);
		p->m_Tab[i].GetSize(&nTabW, &nTabH);
		p->m_Tab[i].SetPosition(nTabX + i * (nTabW + nGap), nTabY);
		p->m_Tab[i].SetLabel(s_szTab[i]);
		if (i < p->m_nTabCount)
			p->m_Tab[i].Show();
		else
			p->m_Tab[i].Hide();
	}

	g_DebugLog("[KTC] LoadScheme: tab xong");
	/* TEN MUC INI TOI DA 29 KY TU: KIniFile::GetKeyValue dung "[" + ten + "]" vao mot
	   char szSection[32] roi g_StrCat - ten dai hon la TRAN NGAN XEP, client tat ngay
	   (c0000409) khong mot dong log. Vi vay muc so gia ten ngan (GiaGocSo/GiaSo). */
	// Luoi o mon: Start = goc o dau, Diff = khoang cach giua hai o, so cot tinh tu be rong nen.
	int nStartX = 0, nStartY = 0, nDiffX = 0, nDiffY = 0, nOW = 0, nOH = 0;
	Ini.GetString("MarketGoods", "Start", "0,0", Buff, sizeof(Buff));
	sscanf(Buff, "%d,%d", &nStartX, &nStartY);
	Ini.GetString("MarketGoods", "Diff", "0,0", Buff, sizeof(Buff));
	sscanf(Buff, "%d,%d", &nDiffX, &nDiffY);
	Ini.GetInteger("MarketGoods", "Width", 171, &nOW);
	Ini.GetInteger("MarketGoods", "Height", 75, &nOH);
	int nCot = (p->m_Width - nStartX + nDiffX) / (nOW + nDiffX);
	if (nCot < 1)
		nCot = 1;
	for (i = 0; i < KTC_MAX_O; i++)
	{
		int nX = nStartX + (i % nCot) * (nOW + nDiffX);
		int nY = nStartY + (i / nCot) * (nOH + nDiffY);
		KUiKtcO& o = p->m_O[i];
		KtcDatTrongO(&o.Nen, &Ini, "MarketGoods", 0, 0);
		o.Nen.SetPosition(nX, nY);
		KtcDatTrongO(&o.Mon, &Ini, "MarketGoods_ItemBox", nX, nY);
		KtcDatTrongO(&o.Ten, &Ini, "MarketGoods_GoodsName", nX, nY);
		KtcDatTrongO(&o.NhanGiaGoc, &Ini, "MarketGoods_OriginalPrice", nX, nY);
		KtcDatTrongO(&o.NhanGia, &Ini, "MarketGoods_Price", nX, nY);
		KtcDatTrongO(&o.GiaGoc, &Ini, "MarketGoods_GiaGocSo", nX, nY);
		KtcDatTrongO(&o.Gia, &Ini, "MarketGoods_GiaSo", nX, nY);
		KtcDatTrongO(&o.Mua, &Ini, "MarketGoods_Buy", nX, nY);
		KtcDatTrongO(&o.GiamGia, &Ini, "MarketGoods_DisCount", nX, nY);
	}

	g_DebugLog("[KTC] LoadScheme: luoi o xong (%d cot)", nCot);
	p->m_NapThe.Init(&Ini, "PrePaid");
	Ini.GetString("PrePaid", "Msg", "", p->m_szNapThe, sizeof(p->m_szNapThe));
	p->m_GioHang.Init(&Ini, "ShoppingCart");
	p->m_PreBtn.Init(&Ini, "LeftBtn");
	p->m_NextBtn.Init(&Ini, "RightBtn");
	p->m_PageInfo.Init(&Ini, "PageInfo");
	p->m_Xu.Init(&Ini, "Xu");
	p->m_CloseBtn.Init(&Ini, "CloseBtn");
	p->m_nXu = -1;
	g_DebugLog("[KTC] LoadScheme: nut xong, dat trang");
	p->SetPage(p->m_nCurrentPage);
}

void KUiMarket::ChonTab(int nTab)
{
	int i;
	for (i = 0; i < m_nTabCount; i++)
		m_Tab[i].CheckButton(i == nTab);
	if (nTab == m_nCurTab || nTab < 0 || nTab >= m_nTabCount)
		return;
	m_nCurTab = nTab;
	// Gio hang giu vi tri mon trong shop dang mo; doi shop thi vi tri do thanh mon khac.
	KUiMarketCart::XoaHet();
	g_pCoreShell->OperationRequest(GOI_MARKET, nTab, 0);
}

void KUiMarket::UpdateData()
{
	Clear();
	m_nObjCount = g_pCoreShell->GetGameData(GDI_TRADE_NPC_ITEM, 0, 0);
	if (m_nObjCount > 0)
	{
		m_pObjsList = (KUiObjAtContRegion*)malloc(sizeof(KUiObjAtContRegion) * m_nObjCount);
		if (m_pObjsList)
		{
			g_pCoreShell->GetGameData(GDI_TRADE_NPC_ITEM, (unsigned int)m_pObjsList, m_nObjCount);
			// nContainer dang la so trang cua kho 6x10 cu; o day tu chia trang nen ghi de.
			for (int i = 0; i < m_nObjCount; i++)
				m_pObjsList[i].eContainer = UOC_MARKET;
		}
		else
			m_nObjCount = 0;
	}
	g_DebugLog("[KTC] UpdateData: %d mon", m_nObjCount);
	m_nPageCount = (m_nObjCount + KTC_MAX_O - 1) / KTC_MAX_O;
	if (m_nPageCount < 1)
		m_nPageCount = 1;
	for (int t = 0; t < m_nTabCount; t++)
		m_Tab[t].CheckButton(t == m_nCurTab);
	SetPage(0);
	m_nXu = -1;
}

void KUiMarket::SetPage(int nPage)
{
	if (nPage >= m_nPageCount)
		nPage = m_nPageCount - 1;
	if (nPage < 0)
		nPage = 0;
	m_nCurrentPage = nPage;
	for (int i = 0; i < KTC_MAX_O; i++)
	{
		KUiKtcO& o = m_O[i];
		int nIdx = nPage * KTC_MAX_O + i;
		if (m_pObjsList == NULL || nIdx >= m_nObjCount)
		{
			o.Nen.Hide(); o.Mon.Hide(); o.Ten.Hide(); o.NhanGiaGoc.Hide(); o.NhanGia.Hide();
			o.GiaGoc.Hide(); o.Gia.Hide(); o.Mua.Hide(); o.GiamGia.Hide();
			o.Mon.Celar();
			continue;
		}
		KUiObjAtContRegion& Obj = m_pObjsList[nIdx];
		KUiItemBuySelInfo	Gia = { 0 };
		g_pCoreShell->GetGameData(GDI_TRADE_ITEM_PRICE, (unsigned int)&Obj, (int)&Gia);
		o.Mon.HoldObject(Obj.Obj.uGenre, Obj.Obj.uId, Obj.Region.Width, Obj.Region.Height);
		o.Ten.SetText(Gia.szItemName);
		o.GiaGoc.SetIntText(Gia.nPrice);
		o.Gia.SetIntText(Gia.nPriceXu);
		o.Nen.Show(); o.Mon.Show(); o.Ten.Show(); o.NhanGiaGoc.Show(); o.NhanGia.Show();
		o.GiaGoc.Show(); o.Gia.Show(); o.Mua.Show();
		if (Gia.nPriceXu > 0 && Gia.nPriceXu < Gia.nPrice)
			o.GiamGia.Show();
		else
			o.GiamGia.Hide();
	}
	g_DebugLog("[KTC] SetPage %d/%d xong", nPage, m_nPageCount);
	char szTrang[32];
	sprintf(szTrang, "%d/%d", m_nCurrentPage + 1, m_nPageCount);
	m_PageInfo.SetText(szTrang);
	m_PreBtn.Enable(m_nCurrentPage > 0);
	m_NextBtn.Enable(m_nCurrentPage + 1 < m_nPageCount);
}

void KUiMarket::OnMua(int nO)
{
	int nIdx = m_nCurrentPage * KTC_MAX_O + nO;
	if (m_pObjsList == NULL || nO < 0 || nO >= KTC_MAX_O || nIdx >= m_nObjCount)
		return;
	KUiItemBuySelInfo	Gia = { 0 };
	g_pCoreShell->GetGameData(GDI_TRADE_ITEM_PRICE, (unsigned int)&m_pObjsList[nIdx], (int)&Gia);
	KUiMarketCart::ThemMon(m_pObjsList[nIdx], Gia);
	KUiMarketCart::OpenWindow();
}

// So xu doi khi mua xong (s2c_syncxu) ma khong co thong bao rieng cho UI -> doc lai moi nhip.
void KUiMarket::Breathe()
{
	int nXu = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	if (nXu != m_nXu)
	{
		m_nXu = nXu;
		m_Xu.SetXuText(nXu);
	}
}

int KUiMarket::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int i;
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		{
			KWndWindow* pWnd = (KWndWindow*)uParam;
			if (Wnd_GetDragObj(NULL))
				break;
			for (i = 0; i < m_nTabCount; i++)
				if (pWnd == (KWndWindow*)&m_Tab[i])
				{
					ChonTab(i);
					return 0;
				}
			for (i = 0; i < KTC_MAX_O; i++)
				if (pWnd == (KWndWindow*)&m_O[i].Mua)
				{
					OnMua(i);
					return 0;
				}
			if (pWnd == (KWndWindow*)&m_NapThe)
			{
				if (m_szNapThe[0])
					KUiMsgCentrePad::SystemMessageArrival(m_szNapThe, (unsigned short)strlen(m_szNapThe));
			}
			else if (pWnd == (KWndWindow*)&m_GioHang)
				KUiMarketCart::OpenWindow();
			else if (pWnd == (KWndWindow*)&m_PreBtn)
				SetPage(m_nCurrentPage - 1);
			else if (pWnd == (KWndWindow*)&m_NextBtn)
				SetPage(m_nCurrentPage + 1);
			else if (pWnd == (KWndWindow*)&m_CloseBtn)
				CloseWindow();
		}
		break;
	case WND_N_RIGHT_CLICK_ITEM:
		// Phai chuot vao hinh mon = bam Mua.
		for (i = 0; i < KTC_MAX_O; i++)
			if (nParam == (int)(KWndWindow*)&m_O[i].Mon)
				OnMua(i);
		break;
	case WM_KEYDOWN:
		if (uParam == VK_ESCAPE)
			CloseWindow();
		break;
	default:
		return KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

void KUiMarket::Clear()
{
	m_nObjCount = 0;
	m_nPageCount = 0;
	if (m_pObjsList)
	{
		free(m_pObjsList);
		m_pObjsList = NULL;
	}
}

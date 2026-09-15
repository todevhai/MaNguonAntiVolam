// -------------------------------------------------------------------------
//	UiMarketCart.cpp - gio hang Ky Tran Cac theo bo cuc jx9tn.
//	Xac nhan = moi dong mot lenh GOI_TRADE_NPC_BUY(so luong). May chu tu tim o, tru xu
//	tung mon; client khong khoa thao tac cho lenh nay nen gui lien ca gio duoc.
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiMarketCart.h"
#include "UiMsgCentrePad.h"
#include "../../../core/src/CoreObjGenreDef.h"
#include "../../../core/src/coreshell.h"
#include "../UiSoundSetting.h"
#include "../UiBase.h"

extern iCoreShell*		g_pCoreShell;

#define SCHEME_INI		"UiMarketCart.ini"

KUiMarketCart*	KUiMarketCart::m_pSelf = NULL;
KKtcMonTrongGio	KUiMarketCart::ms_Mon[KTC_GIO_MAX];
int				KUiMarketCart::ms_nMon = 0;

KUiMarketCart::KUiMarketCart()
{
	m_nXu = -1;
	m_szNapThe[0] = 0;
}

KUiMarketCart* KUiMarketCart::GetIfVisible()
{
	return m_pSelf;
}

KUiMarketCart* KUiMarketCart::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMarketCart;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		m_pSelf->CapNhat();
		m_pSelf->BringToTop();
		m_pSelf->Show();
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
	}
	return m_pSelf;
}

void KUiMarketCart::CloseWindow()
{
	if (m_pSelf)
	{
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

void KUiMarketCart::ThemMon(const KUiObjAtContRegion& Obj, const KUiItemBuySelInfo& Gia)
{
	int i;
	for (i = 0; i < ms_nMon; i++)
	{
		if (ms_Mon[i].Obj.Obj.uId == Obj.Obj.uId)
		{
			if (ms_Mon[i].nSoLuong < KTC_GIO_SL_MAX)
				ms_Mon[i].nSoLuong++;
			break;
		}
	}
	if (i == ms_nMon && ms_nMon < KTC_GIO_MAX)
	{
		ms_Mon[ms_nMon].Obj = Obj;
		ms_Mon[ms_nMon].Gia = Gia;
		ms_Mon[ms_nMon].nSoLuong = 1;
		ms_nMon++;
	}
	if (m_pSelf)
		m_pSelf->CapNhat();
}

void KUiMarketCart::XoaHet()
{
	ms_nMon = 0;
	if (m_pSelf)
		m_pSelf->CapNhat();
}

void KUiMarketCart::Initialize()
{
	for (int i = 0; i < KTC_GIO_MAX; i++)
	{
		AddChild(&m_Dong[i].Nen);
		AddChild(&m_Dong[i].Mon);
		AddChild(&m_Dong[i].Ten);
		AddChild(&m_Dong[i].TongGia);
		AddChild(&m_Dong[i].SoLuong);
		AddChild(&m_Dong[i].Them);
		AddChild(&m_Dong[i].Bot);
		AddChild(&m_Dong[i].Xoa);
		m_Dong[i].Mon.SetContainerId((int)UOC_MARKET);
		m_Dong[i].Mon.EnablePickPut(false);
	}
	AddChild(&m_TieuDe);
	AddChild(&m_NhanTong);
	AddChild(&m_NhanTietKiem);
	AddChild(&m_NhanSoXu);
	AddChild(&m_TongGia);
	AddChild(&m_TietKiem);
	AddChild(&m_SoXu);
	AddChild(&m_NapThe);
	AddChild(&m_XacNhan);
	AddChild(&m_CloseBtn);

	Wnd_AddWindow(this);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

void KUiMarketCart::LoadScheme(const char* pScheme)
{
	char		Buff[256];
	KIniFile	Ini;
	sprintf(Buff, "%s\\" SCHEME_INI, pScheme);
	if (!Ini.Load(Buff))
		return;
	KWndImage::Init(&Ini, "Main");

	// Dong mon: goc [DynamicsGoodsWnd] + [GoodsInfo]; dong sau duoi dong truoc.
	int nKhungX = 0, nKhungY = 0, nDongX = 0, nDongY = 0, nDongH = 54;
	Ini.GetInteger("DynamicsGoodsWnd", "Left", 0, &nKhungX);
	Ini.GetInteger("DynamicsGoodsWnd", "Top", 0, &nKhungY);
	Ini.GetInteger("GoodsInfo", "Left", 0, &nDongX);
	Ini.GetInteger("GoodsInfo", "Top", 0, &nDongY);
	Ini.GetInteger("GoodsInfo", "Height", 54, &nDongH);
	for (int i = 0; i < KTC_GIO_MAX; i++)
	{
		int nX = nKhungX + nDongX;
		int nY = nKhungY + nDongY + i * (nDongH + nDongY);
		KUiKtcDong& d = m_Dong[i];
		KtcDatTrongO(&d.Nen, &Ini, "GoodsInfo", 0, 0);
		d.Nen.SetPosition(nX, nY);
		KtcDatTrongO(&d.Mon, &Ini, "GoodsInfo_ItemBox", nX, nY);
		KtcDatTrongO(&d.Ten, &Ini, "GoodsInfo_GoodsName", nX, nY);
		KtcDatTrongO(&d.TongGia, &Ini, "GoodsInfo_TotalPrice", nX, nY);
		KtcDatTrongO(&d.SoLuong, &Ini, "GoodsInfo_CountEdit", nX, nY);
		KtcDatTrongO(&d.Them, &Ini, "GoodsInfo_AddCount", nX, nY);
		KtcDatTrongO(&d.Bot, &Ini, "GoodsInfo_DelCount", nX, nY);
		KtcDatTrongO(&d.Xoa, &Ini, "GoodsInfo_DelItem", nX, nY);
	}
	m_TieuDe.Init(&Ini, "Title");
	m_NhanTong.Init(&Ini, "TotalCostTitleText");
	m_NhanTietKiem.Init(&Ini, "TotalSaveTitleText");
	m_NhanSoXu.Init(&Ini, "OwnTitleText");
	m_TongGia.Init(&Ini, "TotalCostValueText");
	m_TietKiem.Init(&Ini, "TotalSaveValueText");
	m_SoXu.Init(&Ini, "OwnValueText");
	m_NapThe.Init(&Ini, "PrePaid");
	Ini.GetString("PrePaid", "Msg", "", m_szNapThe, sizeof(m_szNapThe));
	m_XacNhan.Init(&Ini, "ConfirmBuy");
	m_CloseBtn.Init(&Ini, "CloseBtn");
	CapNhat();
}

void KUiMarketCart::CapNhat()
{
	int nTong = 0, nTietKiem = 0;
	for (int i = 0; i < KTC_GIO_MAX; i++)
	{
		KUiKtcDong& d = m_Dong[i];
		if (i >= ms_nMon)
		{
			d.Nen.Hide(); d.Mon.Hide(); d.Ten.Hide(); d.TongGia.Hide();
			d.SoLuong.Hide(); d.Them.Hide(); d.Bot.Hide(); d.Xoa.Hide();
			d.Mon.Celar();
			continue;
		}
		KKtcMonTrongGio& m = ms_Mon[i];
		int nGia = m.Gia.nPriceXu * m.nSoLuong;
		nTong += nGia;
		if (m.Gia.nPrice > m.Gia.nPriceXu)
			nTietKiem += (m.Gia.nPrice - m.Gia.nPriceXu) * m.nSoLuong;
		d.Mon.HoldObject(m.Obj.Obj.uGenre, m.Obj.Obj.uId, m.Obj.Region.Width, m.Obj.Region.Height);
		d.Ten.SetText(m.Gia.szItemName);
		d.TongGia.SetIntText(nGia);
		d.SoLuong.SetIntText(m.nSoLuong);
		d.Nen.Show(); d.Mon.Show(); d.Ten.Show(); d.TongGia.Show();
		d.SoLuong.Show(); d.Them.Show(); d.Bot.Show(); d.Xoa.Show();
	}
	m_TongGia.SetXuText(nTong);
	m_TietKiem.SetXuText(nTietKiem);
	m_XacNhan.Enable(ms_nMon > 0);
	m_nXu = -1;
}

void KUiMarketCart::DoiSoLuong(int nDong, int nThem)
{
	if (nDong < 0 || nDong >= ms_nMon)
		return;
	int n = ms_Mon[nDong].nSoLuong + nThem;
	if (n < 1)
		n = 1;
	if (n > KTC_GIO_SL_MAX)
		n = KTC_GIO_SL_MAX;
	ms_Mon[nDong].nSoLuong = n;
	CapNhat();
}

void KUiMarketCart::XacNhan()
{
	int i, nTong = 0;
	for (i = 0; i < ms_nMon; i++)
		nTong += ms_Mon[i].Gia.nPriceXu * ms_Mon[i].nSoLuong;
	// Chan ca gio khi thieu xu: mua tung dong thi de mua do dang nua gio.
	if (nTong <= 0 || nTong > g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0))
	{
		char szThieu[128];
		KIniFile Ini;
		char Scheme[256], Buff[256];
		szThieu[0] = 0;
		g_UiBase.GetCurSchemePath(Scheme, 256);
		sprintf(Buff, "%s\\" SCHEME_INI, Scheme);
		if (Ini.Load(Buff))
			Ini.GetString("ConfirmBuy", "MsgThieuXu", "", szThieu, sizeof(szThieu));
		if (szThieu[0])
			KUiMsgCentrePad::SystemMessageArrival(szThieu, (unsigned short)strlen(szThieu));
		return;
	}
	for (i = 0; i < ms_nMon; i++)
		g_pCoreShell->OperationRequest(GOI_TRADE_NPC_BUY, (unsigned int)&ms_Mon[i].Obj, ms_Mon[i].nSoLuong);
	ms_nMon = 0;
	CloseWindow();
}

void KUiMarketCart::Breathe()
{
	int nXu = g_pCoreShell->GetGameData(GDI_PLAYER_HOLD_XU, 0, 0);
	if (nXu != m_nXu)
	{
		m_nXu = nXu;
		m_SoXu.SetXuText(nXu);
	}
}

int KUiMarketCart::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch (uMsg)
	{
	case WND_N_BUTTON_CLICK:
		{
			KWndWindow* pWnd = (KWndWindow*)uParam;
			for (int i = 0; i < KTC_GIO_MAX; i++)
			{
				if (pWnd == (KWndWindow*)&m_Dong[i].Them)
				{
					DoiSoLuong(i, 1);
					return 0;
				}
				if (pWnd == (KWndWindow*)&m_Dong[i].Bot)
				{
					DoiSoLuong(i, -1);
					return 0;
				}
				if (pWnd == (KWndWindow*)&m_Dong[i].Xoa && i < ms_nMon)
				{
					for (int j = i; j + 1 < ms_nMon; j++)
						ms_Mon[j] = ms_Mon[j + 1];
					ms_nMon--;
					CapNhat();
					return 0;
				}
			}
			if (pWnd == (KWndWindow*)&m_XacNhan)
				XacNhan();
			else if (pWnd == (KWndWindow*)&m_NapThe)
			{
				if (m_szNapThe[0])
					KUiMsgCentrePad::SystemMessageArrival(m_szNapThe, (unsigned short)strlen(m_szNapThe));
			}
			else if (pWnd == (KWndWindow*)&m_CloseBtn)
				CloseWindow();
		}
		break;
	case WM_KEYDOWN:
		if (uParam == VK_ESCAPE)
			CloseWindow();
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}

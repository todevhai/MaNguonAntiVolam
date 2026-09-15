// -------------------------------------------------------------------------
//	UiMarketCart.h - gio hang Ky Tran Cac, bo cuc theo jx9tn (\ui\ui3\UiMarketCart.ini).
//	Danh sach mon giu o bien tinh: them mon khi cua so dang dong van giu.
//	Moi dong = mot mon cua shop DANG MO (vi tri trong shop); doi tab thi xoa het.
// -------------------------------------------------------------------------
#pragma once

#include "../Elem/WndImage.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

// Dat mot thanh phan theo ini: toa do trong ini tinh tu goc (nX, nY) cua o/dong chua no.
inline void KtcDatTrongO(KWndWindow* pWnd, KIniFile* pIni, const char* pszMuc, int nX, int nY)
{
	pWnd->Init(pIni, pszMuc);
	int nLeft, nTop;
	pWnd->GetPosition(&nLeft, &nTop);
	pWnd->SetPosition(nX + nLeft, nY + nTop);
}

#define KTC_GIO_MAX		5		// so dong hien vua khung 297 diem cua gio (moi dong 54+3)
#define KTC_GIO_SL_MAX	100		// khop KTC_MAX_MUA phia may chu

struct KKtcMonTrongGio
{
	KUiObjAtContRegion	Obj;
	KUiItemBuySelInfo	Gia;	// nPriceXu = gia phai tra, nPrice = gia goc
	int					nSoLuong;
};

struct KUiKtcDong
{
	KWndImage			Nen;
	KWndObjectBox		Mon;
	KWndText80			Ten;
	KWndText32			TongGia;
	KWndText32			SoLuong;
	KWndButton			Them;
	KWndButton			Bot;
	KWndLabeledButton	Xoa;
};

class KUiMarketCart : protected KWndImage
{
public:
	static KUiMarketCart*	OpenWindow();
	static KUiMarketCart*	GetIfVisible();
	static void				CloseWindow();
	static void				ThemMon(const KUiObjAtContRegion& Obj, const KUiItemBuySelInfo& Gia);
	static void				XoaHet();
	int						WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void					Breathe();
private:
	KUiMarketCart();
	~KUiMarketCart() {}
	void	Initialize();
	void	LoadScheme(const char* pScheme);
	void	CapNhat();
	void	XacNhan();
	void	DoiSoLuong(int nDong, int nThem);
private:
	static KUiMarketCart*	m_pSelf;
	static KKtcMonTrongGio	ms_Mon[KTC_GIO_MAX];
	static int				ms_nMon;
	int						m_nXu;
	char					m_szNapThe[256];
	KUiKtcDong				m_Dong[KTC_GIO_MAX];
	KWndText256				m_TieuDe;
	KWndText32				m_NhanTong;
	KWndText32				m_NhanTietKiem;
	KWndText32				m_NhanSoXu;
	KWndText256				m_TongGia;
	KWndText256				m_TietKiem;
	KWndText256				m_SoXu;
	KWndLabeledButton		m_NapThe;
	KWndLabeledButton		m_XacNhan;
	KWndButton				m_CloseBtn;
};

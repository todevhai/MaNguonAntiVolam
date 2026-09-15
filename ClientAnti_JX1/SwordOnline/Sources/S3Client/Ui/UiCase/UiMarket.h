// -------------------------------------------------------------------------
//	UiMarket.h - cua so Ky Tran Cac, bo cuc theo jx9tn (\ui\ui3\UiMarket.ini, ban goc
//	la "qizhenge jiemian" trong pak jx9tn).
//	Tab doc tu \settings\shop\type.txt; moi tab la mot shop buysell. Luoi o mon co ten,
//	gia goc, gia phai tra, dau giam gia va nut Mua (bo vao gio hang KUiMarketCart).
// -------------------------------------------------------------------------
#pragma once

#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndImage.h"
#include "../Elem/WndObjContainer.h"
#include "../../../core/src/gamedatadef.h"

#define KTC_MAX_TAB		8		// khop KTC_MAX_TAB phia may chu (KBuySell.h)
#define KTC_MAX_O		15		// o mon moi trang (3 cot x 5 hang cua nen 579x524)

struct KUiKtcO
{
	KWndImage			Nen;
	KWndObjectBox		Mon;
	KWndText80			Ten;
	KWndText32			NhanGiaGoc;
	KWndText32			NhanGia;
	KWndText32			GiaGoc;
	KWndText32			Gia;
	KWndLabeledButton	Mua;
	KWndImage			GiamGia;
};

class KUiMarket : protected KWndShowAnimate
{
public:
	static KUiMarket*	OpenWindow();
	static KUiMarket*	GetIfVisible();
	static void			CloseWindow();
	static void			LoadScheme(const char* pScheme);
	static void			CancelTrade();
	void				UpdateData();
	int					WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void				Breathe();
private:
	KUiMarket();
	~KUiMarket() {}
	void	Initialize();
	void	DocTab();
	void	ChonTab(int nTab);
	void	SetPage(int nPage);
	void	OnMua(int nO);
	void	Clear();
private:
	static KUiMarket*	m_pSelf;
	KUiObjAtContRegion*	m_pObjsList;
	int					m_nObjCount;
	int					m_nPageCount;
	int					m_nCurrentPage;
	int					m_nTabCount;
	int					m_nCurTab;
	int					m_nXu;
	char				m_szNapThe[256];	// [PrePaid] Msg - tin hien khi bam Nap the
	KWndLabeledButton	m_Tab[KTC_MAX_TAB];
	KUiKtcO				m_O[KTC_MAX_O];
	KWndLabeledButton	m_NapThe;
	KWndLabeledButton	m_GioHang;
	KWndLabeledButton	m_PreBtn;
	KWndLabeledButton	m_NextBtn;
	KWndText32			m_PageInfo;
	KWndText256			m_Xu;
	KWndButton			m_CloseBtn;
};

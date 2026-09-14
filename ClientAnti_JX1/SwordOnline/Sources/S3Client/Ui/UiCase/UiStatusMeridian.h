/*****************************************************************************************
//	Bang F3 - trang Kinh mach, theo jx9tn (lop KUiStatusPageMeridian, ini trang kinh mach).
------------------------------------------------------------------------------------------
	Trai: danh sach chon - Khi Doanh Dan Dien + 8 mach (muc dang chon chu vang).
	Giua: chon mach -> hinh nguoi voi 16 huyet, vi tri doc tu \ui\ui3\Meridian\<mach>.ini (co san
	trong pak), huyet da xung sang; chon Khi Doanh -> 8 diem mach xep vong (sang khi mach du tang 16),
	dong trang thai va nut 1/7/30 ngay.
	Duoi: "Chan Nguyen hien tai: N diem".

	Bam huyet: huyet da xung -> tin "da duoc xung kai"; bo qua huyet -> tin "theo tuan tu";
	dung huyet ke tiep -> hop xac nhan KUiMeridianConfirm (chon cach, xem chi phi, Xung Huyet).
	Moi luat va chu chi phi do may chu (meridian.lua) gui; trang chi hien thi.
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndPage.h"

#define MERIDIAN_PAGE_COUNT		8
#define MERIDIAN_PAGE_ACUPS		16
#define MERIDIAN_BREATH_DAYS	3

class KUiStatusMeridianPage : public KWndPage
{
public:
	KUiStatusMeridianPage();
	void	Initialize();
	void	LoadScheme(const char* pScheme);
	virtual void	Show();								// trang vua duoc chon: xin dong bo
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	PaintWindow();
private:
	void	Select(int nMeridian);						// 0 = Khi Doanh Dan Dien, 1..8 = mach
	void	LoadAcupointLayout(int nMeridian);
	void	PaintAcupointLines();						// duong noi huyet + vach dan ten
	void	Refresh();									// dua trang theo goi s2c_meridian moi nhat
	void	OnAcupointClick(int nIndex);				// 0..15
	void	ShowMessage(const char* pszKey);			// tin he thong, chu lay tu ini [Messages]
private:
	KWndLabeledButton	m_BtnFullBreath;
	KWndLabeledButton	m_BtnMeridian[MERIDIAN_PAGE_COUNT];
	KWndButton			m_Acup[MERIDIAN_PAGE_ACUPS];
	KWndText32			m_AcupName[MERIDIAN_PAGE_ACUPS];	// ten huyet, trai/phai theo [AcupPos] cua ini mach
	int					m_nAcupRight[MERIDIAN_PAGE_ACUPS];	// 1 = nhan ben phai huyet
	KWndImage			m_BreathPoint[MERIDIAN_PAGE_COUNT];
	KWndLabeledButton	m_BtnBreathDays[MERIDIAN_BREATH_DAYS];
	KWndText256			m_BreathInfo;
	KWndText256			m_CurZhenYuan;
	char				m_szScheme[128];
	char				m_szLayout[MERIDIAN_PAGE_COUNT][80];	// ini vi tri huyet cua tung mach
	char				m_szFormatZhenYuan[64];			// chu TCVN3 lay tu ini
	char				m_szFormatAcupTip[64];
	int					m_nMeridian;					// 0 Khi Doanh, 1..8 mach dang chon
	int					m_nVersion;						// GDI_MERIDIAN_INFO da ve
	int					m_nLevel[MERIDIAN_PAGE_COUNT];	// cap 8 mach theo goi moi nhat
};

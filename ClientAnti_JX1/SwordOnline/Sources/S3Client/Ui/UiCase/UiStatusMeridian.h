/*****************************************************************************************
//	Bang F3 - trang Kinh mach (bo cuc va anh cua jx9tn).
------------------------------------------------------------------------------------------
	Trai: nut Xung huyet, 8 nut chon mach, 3 nut cach xung (thuong / Huyet Long Dang /
	Long Hon Ho The; mach 3-4 may chu luon dung Dai Ho Mach Don). Giua: hinh nguoi voi 16 huyet
	cua mach dang chon, vi tri doc tu \ui\ui3\Meridian\<mach>.ini (co san trong pak); huyet da
	xung sang (khung 1 cua anh diem). Duoi: vat lieu, huyet dang chon va chu goi y chi phi / ti le.

	Moi luat (chi phi, ti le, bao ho, phi 50 van) nam o may chu (meridian.lua): trang chi gui
	GOI_MERIDIAN va ve lai goi s2c_meridian (GDI_MERIDIAN_INFO), chu goi y cung do may chu soan.
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndPage.h"

#define MERIDIAN_PAGE_COUNT		8
#define MERIDIAN_PAGE_ACUPS		16
#define MERIDIAN_PAGE_WAYS		3

class KIniFile;

class KUiStatusMeridianPage : public KWndPage
{
public:
	KUiStatusMeridianPage();
	void	Initialize();
	void	LoadScheme(const char* pScheme);
	virtual void	Show();								// trang vua duoc chon: xin goi y moi
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	PaintWindow();
private:
	void	SelectMeridian(int nMeridian);				// 1..8
	void	SelectWay(int nWay);						// 0..2
	void	LoadAcupointLayout(int nMeridian);
	void	RequestTips();
	void	Refresh();									// dua trang theo goi s2c_meridian moi nhat
	void	ShowAcupoint(int nLevel);					// 1..16: dong thong tin huyet
private:
	KWndLabeledButton	m_BtnLevelUp;
	KWndLabeledButton	m_BtnMeridian[MERIDIAN_PAGE_COUNT];
	KWndLabeledButton	m_BtnWay[MERIDIAN_PAGE_WAYS];
	KWndButton			m_Acup[MERIDIAN_PAGE_ACUPS];
	KWndText256			m_Material;
	KWndText256			m_AcupInfo;
	KWndText256			m_Tips;
	char				m_szScheme[128];
	char				m_szFormatMaterial[96];			// chu TCVN3 lay tu ini
	char				m_szFormatAcup[64];
	char				m_szLayout[MERIDIAN_PAGE_COUNT][80];	// ini vi tri huyet cua tung mach
	int					m_nMeridian;					// mach dang chon 1..8
	int					m_nWay;
	int					m_nVersion;						// GDI_MERIDIAN_INFO da ve
	int					m_nShowAcup;					// huyet dang hien thong tin, 0 = huyet ke tiep
};

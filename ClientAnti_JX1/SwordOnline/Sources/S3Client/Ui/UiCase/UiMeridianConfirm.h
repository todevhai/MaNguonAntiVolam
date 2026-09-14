/*****************************************************************************************
//	Hop xac nhan xung huyet (kinh mach) - theo KUiMeridianConfirm cua jx9tn.
------------------------------------------------------------------------------------------
	Mo khi bam dung huyet ke tiep o trang Kinh mach cua F3. Tieu de = ten huyet; khung chu = chi
	phi, ti le, hong the nao (may chu soan bang GetLevelUpTips trong meridian.lua, cung luat voi
	lan xung that); ba o chon cach: Long Hon Ho The / Huyet vi bao ve / Pho thong. Mach 3-4 chi co
	mot cach (Dai Ho Mach Don): an ba o, nut Xung Huyet ra giua. Doi cach -> xin chu goi y moi.
	Xung Huyet -> GOI_MERIDIAN(mach, cach), dong hop; ket qua ve bang tin chu + trang Kinh mach.
	Bo cuc: UiMeridianConfirm.ini (tu ini jx9tn), anh nen + nut co san trong pak.
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShowAnimate.h"

#define MERIDIAN_CONFIRM_WAYS	3

class KUiMeridianConfirm : protected KWndShowAnimate
{
public:
	static KUiMeridianConfirm*	OpenWindow(int nMeridian, int nLevel);	// mach 1..8, huyet 1..16
	static KUiMeridianConfirm*	GetIfVisible();
	static void					CloseWindow();
private:
	KUiMeridianConfirm();
	~KUiMeridianConfirm() {}
	void	Initialize();
	void	LoadScheme(const char* pScheme);
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	virtual void	PaintWindow();
	void	SetTarget(int nMeridian, int nLevel);
	void	SelectWay(int nWay);						// MERIDIAN_LEVELUP_WAY_* 0..2
	void	RequestTips();
private:
	static KUiMeridianConfirm*	m_pSelf;
private:
	KWndText80			m_Title;
	KWndText512			m_Info;
	KWndButton			m_WayBox[MERIDIAN_CONFIRM_WAYS];	// o chon, chi so = cach
	KWndText80			m_WayLabel[MERIDIAN_CONFIRM_WAYS];
	KWndLabeledButton	m_BtnOk;
	KWndLabeledButton	m_BtnOkOnlyOneWay;
	KWndButton			m_BtnClose;
	int					m_nMeridian;
	int					m_nLevel;
	int					m_nWay;
	int					m_nVersion;
	char				m_szFormatTitle[64];
};

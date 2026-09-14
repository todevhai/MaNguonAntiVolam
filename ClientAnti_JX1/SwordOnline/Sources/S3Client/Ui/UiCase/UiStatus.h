/*****************************************************************************************
//	Bang F3: trang thai nhan vat, dang TAB nhu ban jx9tn.
//	Copyright : Kingsoft 2002 (ban goc Wooy); chia tab 14/09/2026.
------------------------------------------------------------------------------------------
	Khung (UiStatus.ini) giu nut tab + nut Dong; ba trang la cua so con:
	  Thuoc tinh  UiStatusAttrib.ini   chan dung, ten, sinh/noi/the luc, 4 thuoc tinh + nut cong diem,
	                                   sat thuong, khang
	  Trang bi    UiStatusEquip.ini    anh nguoi nam/nu, 12 o trang bi, nut Khoa
	  Kinh mach   UiStatusMeridian.ini xem UiStatusMeridian.h
	Moi control van la thanh vien cua KUiStatus nhu truoc (logic giu nguyen), chi doi CHA cua
	no sang trang tuong ung; trang chuyen tiep thong bao cua control len KUiStatus.
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndLabeledButton.h"
#include "../Elem/WndText.h"
#include "../Elem/WndPage.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndImage.h"
#include "UiStatusMeridian.h"

enum UI_PLAYER_ATTRIBUTE;
struct KUiPlayerRuntimeInfo;
struct KUiObjAtRegion;
struct KUiPlayerAttribute;

#define _ITEM_COUNT 15

// Trang cua F3: KWndPage chi chuyen tiep bam nut len cha; o trang bi con bao nhac/tha do
// (WND_N_ITEM_PICKDROP) va bam chuot vao do - thieu la keo tha do mac khong ai nhan.
class KUiStatusPage : public KWndPage
{
public:
	virtual int	WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
};

class KUiStatus : public KWndPageSet
{
public:
	//----giao dien chung cua cac bang----
	static KUiStatus*	OpenWindow();					// mo bang, tra ve doi tuong duy nhat
	static KUiStatus*	GetIfVisible();					// dang hien thi thi tra ve doi tuong
	static void			CloseWindow(bool bDestroy);		// dong bang, co the huy doi tuong
	static void			LoadScheme(const char* pScheme);// nap bo cuc giao dien

	void	UpdateBaseData();							// ten, danh hieu - it doi
	void	UpdateData();
	void	UpdateRuntimeInfo(KUiPlayerRuntimeInfo* pInfo);
	void	UpdateAllEquips();
	void	UpdateRuntimeAttribute(KUiPlayerAttribute* pInfo);

	void	UpdateEquip(KUiObjAtRegion* pEquip, int bAdd);	// mot o trang bi doi

private:
	KUiStatus() {}
	~KUiStatus() {}
	void	Initialize();
	void	UseRemainPoint(UI_PLAYER_ATTRIBUTE type);	// cong mot diem tiem nang
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	LoadPages(const char* pScheme);				// nap khung + ba trang
	void	UpdateAvatar();								// doi anh chan dung khi lua chon thay doi
	virtual void	PaintWindow();
	void	OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
private:
	static KUiStatus*	m_pSelf;

private:
	KWndLabeledButton	m_BtnAttribPage, m_BtnEquipPage, m_BtnMeridianPage;
	KUiStatusPage		m_AttribPage;
	KUiStatusPage		m_EquipPage;
	KUiStatusMeridianPage	m_MeridianPage;
	KWndButton	m_Close;

	// ---- trang Thuoc tinh ----
	KWndImage	m_Face;		// o ve chan dung (anh doi theo lua chon)
	int			m_nAvatarVe;	// chan dung dang ve, de khoi doi anh moi khung
	KWndButton		m_Avatar;
	KWndText512		m_AvatarText;
	KWndText32	m_Name;
	KWndText32	m_Title;
	KWndText32	m_Life;
	KWndText32	m_Mana;
	KWndText32	m_Experience;
	KWndText32	m_Stamina;
	int			m_nRemainPoint;
	KWndText32	m_RemainPoint;
	KWndText32	m_Strength, m_Dexterity, m_Vitality, m_Energy;
	KWndButton	m_AddStrength, m_AddDexterity, m_AddVitality, m_AddEnergy;
	KWndText32	m_LeftDamage, m_RightDamage, m_Attack, m_Defence, m_MoveSpeed, m_AttackSpeed;
	KWndText32	m_PhyDef, m_CoolDef, m_LightDef, m_FireDef, m_PoisonDef;
	KWndText32	m_Level, m_StatusDesc;
	KWndText32	m_nRepute;
	KWndText32	m_nFuYuan;
	KWndText32  m_nPaiMing;

	// ---- trang Trang bi ----
	KWndText32	m_EquipName;
	KWndText32	m_nPk;
	KWndText32  m_ReBorn;
	KWndButton	m_UnlockBtn;
	KWndObjectBox	m_EquipBox[_ITEM_COUNT];
};

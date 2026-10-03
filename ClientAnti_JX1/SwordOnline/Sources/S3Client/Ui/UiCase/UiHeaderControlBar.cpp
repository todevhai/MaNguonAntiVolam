/*****************************************************************************************
//	界面--屏幕顶控制操作条
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-22
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"
#include "../UiBase.h"
#include "UiHeaderControlBar.h"
#include "UiChatCentre.h"

#include "../ShortcutKey.h"
#include "../../../core/src/gamedatadef.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI		"UiHeaderControlBar.ini"

KUiHeaderControlBar* KUiHeaderControlBar::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiHeaderControlBar* KUiHeaderControlBar::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiHeaderControlBar;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
		m_pSelf->Show();
		m_pSelf->UpdateData1();
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：关闭窗口
//--------------------------------------------------------------------------
void KUiHeaderControlBar::CloseWindow()
{
	if (m_pSelf)
	{
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

//初始化
void KUiHeaderControlBar::Initialize()
{
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	m_Style &= ~WND_S_VISIBLE;
	Wnd_AddWindow(this, WL_TOPMOST);
}

//载入界面方案
void KUiHeaderControlBar::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	if (m_pSelf)
	{
		sprintf(Buff, "%s\\" SCHEME_INI, pScheme);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_LevelText.Init(&Ini,"LevelText");
			m_pSelf->m_RankWorldText.Init(&Ini,"RankWorldText");
			m_pSelf->m_Stamina.Init(&Ini, "Stamina");
			m_pSelf->m_Life   .Init(&Ini, "Life");
			m_pSelf->m_Mana   .Init(&Ini, "Mana");
			m_pSelf->m_Exp    .Init(&Ini, "Exp");
			m_pSelf->m_StaminaText.Init(&Ini, "StaminaText");
			m_pSelf->m_LifeText   .Init(&Ini, "LifeText");
			m_pSelf->m_ManaText   .Init(&Ini, "ManaText");
			m_pSelf->m_ExpText    .Init(&Ini, "ExpText");
			/* Khong gan con thi cua so co ton tai cung khong ai ve.
			   Hai o chu cu cung chua tung duoc gan - gan luon o day. */
			m_pSelf->AddChild(&m_pSelf->m_LevelText);
			m_pSelf->AddChild(&m_pSelf->m_RankWorldText);
			m_pSelf->AddChild(&m_pSelf->m_Stamina);
			m_pSelf->AddChild(&m_pSelf->m_Life);
			m_pSelf->AddChild(&m_pSelf->m_Mana);
			m_pSelf->AddChild(&m_pSelf->m_Exp);
			m_pSelf->AddChild(&m_pSelf->m_StaminaText);
			m_pSelf->AddChild(&m_pSelf->m_LifeText);
			m_pSelf->AddChild(&m_pSelf->m_ManaText);
			m_pSelf->AddChild(&m_pSelf->m_ExpText);
		}
	}
}

//重新初始化界面
void KUiHeaderControlBar::DefaultScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	if (m_pSelf)
	{
		sprintf(Buff, "%s\\" SCHEME_INI, pScheme);
		if (Ini.Load(Buff))
		{
			int	nValue1, nValue2;
			Ini.GetInteger("Main", "Left",  0, &nValue1);
			Ini.GetInteger("Main", "Top",   0, &nValue2);
			m_pSelf->SetPosition(nValue1, nValue2);
		}
	}
}

void KUiHeaderControlBar::UpdateData1()
{
		/* chu "Cap" da co san tren anh nen, khong ve de len */
		/* chu "Hang" da co san tren anh nen, khong ve de len */
}

/* Cap nhat bon thanh moi nhip tho. GDI_PLAYER_RT_INFO la duong ma
   KUiPlayerBar da dung san de lay mau va noi luc, dung lai o day.
   Kinh nghiem: m_nExp la phan da di trong cap HIEN TAI (LevelUp dat ve 0),
   nExperienceFull = m_nNextLevelExp la nguong cua cap nay. Truoc day tru
   nExperienceFull (tuong la moc cap truoc) -> mau so 0 -> luon hien 0/1. */
static void DatThanh(KWndImagePart& Thanh, KWndText32& Chu, int nHienTai, int nDay)
{
	if (nDay <= 0)
		nDay = 1;
	if (nHienTai < 0)
		nHienTai = 0;
	if (nHienTai > nDay)
		nHienTai = nDay;
	Thanh.SetPart(nHienTai, nDay);
	char szSo[48];
	sprintf(szSo, "%d/%d", nHienTai, nDay);
	Chu.SetText(szSo);
}

/* Kinh nghiem hien PHAN TRAM (2 so le): nguong cap cao hang chuc trieu,
   dang "hien tai/nguong" khong vua o chu. Nhan bang so 64 bit. */
static void DatThanhPhanTram(KWndImagePart& Thanh, KWndText32& Chu, int nHienTai, int nDay)
{
	DatThanh(Thanh, Chu, nHienTai, nDay);
	if (nDay <= 0)
		nDay = 1;
	if (nHienTai < 0)
		nHienTai = 0;
	if (nHienTai > nDay)
		nHienTai = nDay;
	int nPhanVan = (int)((__int64)nHienTai * 10000 / nDay);
	char szSo[24];
	sprintf(szSo, "%d.%02d%%", nPhanVan / 100, nPhanVan % 100);
	Chu.SetText(szSo);
}

void KUiHeaderControlBar::Breathe()
{
	UpdateData();
	KUiPlayerRuntimeInfo Info;
	memset(&Info, 0, sizeof(Info));
	g_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);
	DatThanh(m_Stamina, m_StaminaText, Info.nStamina, Info.nStaminaFull);
	DatThanh(m_Life,    m_LifeText,    Info.nLife,    Info.nLifeFull);
	DatThanh(m_Mana,    m_ManaText,    Info.nMana,    Info.nManaFull);
	DatThanhPhanTram(m_Exp, m_ExpText, Info.nExperience, Info.nExperienceFull);
	/* Cap va hang xep the gioi nam o KUiPlayerAttribute, khong phai
	   KUiPlayerRuntimeInfo - phai hoi rieng. */
	KUiPlayerAttribute ThuocTinh;
	memset(&ThuocTinh, 0, sizeof(ThuocTinh));
	g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (int)&ThuocTinh, 0);
	char szSo[32];
	sprintf(szSo, "%d", ThuocTinh.nLevel);
	m_LevelText.SetText(szSo);
	sprintf(szSo, "%d", ThuocTinh.nRankInWorld);
	m_RankWorldText.SetText(szSo);
}


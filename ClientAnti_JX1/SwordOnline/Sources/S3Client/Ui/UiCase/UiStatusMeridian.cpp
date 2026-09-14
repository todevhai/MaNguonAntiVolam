/*****************************************************************************************
//	Bang F3 - trang Kinh mach. Xem UiStatusMeridian.h.
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "KEngine.h"					// g_DebugLog
#include "UiStatusMeridian.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI_MERIDIAN	"UiStatusMeridian.ini"
#define	MAU_CHON			0xffe1d52b	// chu nut dang chon: SelectColor 225,213,43 cua jx9tn
#define	MAU_THUONG			0xffffffff
#define	CONFIRM_MS			5000	// bam Xung huyet lan hai trong 5 giay moi gui (moi lan ton 50 van)

KUiStatusMeridianPage::KUiStatusMeridianPage()
{
	m_szScheme[0] = 0;
	memset(m_szLayout, 0, sizeof(m_szLayout));
	memset(m_szFormatMaterial, 0, sizeof(m_szFormatMaterial));
	memset(m_szFormatAcup, 0, sizeof(m_szFormatAcup));
	memset(m_szFormatAcupNone, 0, sizeof(m_szFormatAcupNone));
	memset(m_szLabelLevelUp, 0, sizeof(m_szLabelLevelUp));
	memset(m_szLabelConfirm, 0, sizeof(m_szLabelConfirm));
	m_uConfirmUntil = 0;
	m_nMeridian = 1;
	m_nWay = 0;
	m_nVersion = -1;
	m_nShowAcup = 0;
}

void KUiStatusMeridianPage::Initialize()
{
	int i;
	AddChild(&m_BtnLevelUp);
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
		AddChild(&m_BtnMeridian[i]);
	for (i = 0; i < MERIDIAN_PAGE_WAYS; i++)
		AddChild(&m_BtnWay[i]);
	for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
		AddChild(&m_Acup[i]);
	AddChild(&m_Material);
	AddChild(&m_AcupInfo);
	AddChild(&m_Tips);
}

void KUiStatusMeridianPage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	int			i;

	strncpy(m_szScheme, pScheme, sizeof(m_szScheme) - 1);
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_MERIDIAN);
	if (!Ini.Load(Buff))
		return;

	Init(&Ini, "Main");
	m_BtnLevelUp.Init(&Ini, "BtnLevelUp");
	Ini.GetString("BtnLevelUp", "Label", "", m_szLabelLevelUp, sizeof(m_szLabelLevelUp));
	Ini.GetString("BtnLevelUp", "LabelConfirm", "OK?", m_szLabelConfirm, sizeof(m_szLabelConfirm));

	// 8 nut mach dung chung mot mau [MeridianConfig], xep doc cach nhau Step diem.
	int nLeft = 0, nTop = 0, nStep = 24;
	Ini.GetInteger("MeridianConfig", "Left", 0, &nLeft);
	Ini.GetInteger("MeridianConfig", "Top", 0, &nTop);
	Ini.GetInteger("MeridianConfig", "Step", 24, &nStep);
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
	{
		char szKey[16], szLabel[32];
		m_BtnMeridian[i].Init(&Ini, "MeridianConfig");
		m_BtnMeridian[i].SetPosition(nLeft, nTop + i * nStep);
		sprintf(szKey, "Label_%d", i);
		Ini.GetString("MeridianConfig", szKey, "", szLabel, sizeof(szLabel));
		m_BtnMeridian[i].SetLabel(szLabel);
		// vi tri 16 huyet cua mach nay: tep ini trong pak, duong dan tuong doi voi bo giao dien
		sprintf(szKey, "Meridian_%d", i);
		Ini.GetString("MeridianConfig", szKey, "", Buff, sizeof(Buff));
		sprintf(m_szLayout[i], "%s%s", pScheme, Buff);
	}
	for (i = 0; i < MERIDIAN_PAGE_WAYS; i++)
	{
		char szSection[16];
		sprintf(szSection, "BtnWay%d", i);
		m_BtnWay[i].Init(&Ini, szSection);
	}
	m_Material.Init(&Ini, "Material");
	m_AcupInfo.Init(&Ini, "AcupInfo");
	m_Tips.Init(&Ini, "Tips");
	// Chu co dau nam o ini (TCVN3); nguon chi giu ASCII.
	Ini.GetString("Material", "Format", "%d %d %d", m_szFormatMaterial, sizeof(m_szFormatMaterial));
	Ini.GetString("AcupInfo", "Format", "%s %d/16: %s", m_szFormatAcup, sizeof(m_szFormatAcup));
	Ini.GetString("AcupInfo", "FormatNone", "%s %d/16", m_szFormatAcupNone, sizeof(m_szFormatAcupNone));

	LoadAcupointLayout(m_nMeridian);
	SelectWay(m_nWay);
	m_nVersion = -1;
}

void KUiStatusMeridianPage::LoadAcupointLayout(int nMeridian)
{
	KIniFile Ini;
	if (nMeridian < 1 || nMeridian > MERIDIAN_PAGE_COUNT || !Ini.Load(m_szLayout[nMeridian - 1]))
		return;
	for (int i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
	{
		char szSection[32];
		sprintf(szSection, "btnAcupuncturePoint_%d", i);
		m_Acup[i].Init(&Ini, szSection);
	}
}

// Trang vua duoc chon (KWndPageSet::ActivePage): xin may chu cap + goi y cua mach dang chon.
void KUiStatusMeridianPage::Show()
{
	KWndPage::Show();
	RequestTips();
}

void KUiStatusMeridianPage::RequestTips()
{
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_MERIDIAN, m_nMeridian, m_nWay | UI_MERIDIAN_TIPS_ONLY);
}

void KUiStatusMeridianPage::SelectMeridian(int nMeridian)
{
	if (nMeridian < 1 || nMeridian > MERIDIAN_PAGE_COUNT)
		return;
	CancelConfirm();
	if (nMeridian != m_nMeridian)
	{
		m_nMeridian = nMeridian;
		m_nShowAcup = 0;
		LoadAcupointLayout(nMeridian);
	}
	m_nVersion = -1;
	RequestTips();
}

void KUiStatusMeridianPage::SelectWay(int nWay)
{
	if (nWay < 0 || nWay >= MERIDIAN_PAGE_WAYS)
		return;
	if (nWay != m_nWay)
		CancelConfirm();
	m_nWay = nWay;
	for (int i = 0; i < MERIDIAN_PAGE_WAYS; i++)
	{
		m_BtnWay[i].CheckButton(i == nWay);
		m_BtnWay[i].SetLabelColor(i == nWay ? MAU_CHON : MAU_THUONG);
	}
}

int KUiStatusMeridianPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_BUTTON_CLICK)
	{
		int i;
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnLevelUp)
		{
			// Hai buoc nhu hop xac nhan cua jx9tn: lan dau doi nhan thanh "Xac nhan", lan hai
			// trong CONFIRM_MS moi gui. Doi mach / cach thi huy (xem CancelConfirm).
			unsigned int uNow = GetTickCount();
			if (m_uConfirmUntil && uNow < m_uConfirmUntil)
			{
				CancelConfirm();
				g_DebugLog("[kinh mach] gui xung mach %d cach %d", m_nMeridian, m_nWay);
				if (g_pCoreShell)
					g_pCoreShell->OperationRequest(GOI_MERIDIAN, m_nMeridian, m_nWay);
			}
			else
			{
				m_uConfirmUntil = uNow + CONFIRM_MS;
				m_BtnLevelUp.SetLabel(m_szLabelConfirm);
			}
			return 0;
		}
		for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_BtnMeridian[i])
			{
				SelectMeridian(i + 1);
				return 0;
			}
		}
		for (i = 0; i < MERIDIAN_PAGE_WAYS; i++)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_BtnWay[i])
			{
				SelectWay(i);
				m_nVersion = -1;
				RequestTips();
				return 0;
			}
		}
		for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_Acup[i])
			{
				m_nShowAcup = i + 1;
				m_nVersion = -1;		// ve lai: bam lam tat khung sang cua huyet
				return 0;
			}
		}
	}
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

void KUiStatusMeridianPage::ShowAcupoint(int nLevel)
{
	KUiMeridianAcupDesc Desc;
	memset(&Desc, 0, sizeof(Desc));
	Desc.nMeridian = m_nMeridian;
	Desc.nLevel = nLevel;
	if (!g_pCoreShell || !g_pCoreShell->GetGameData(GDI_MERIDIAN_ACUP_DESC, (unsigned int)&Desc, 0))
	{
		m_AcupInfo.SetText("");
		return;
	}
	char szText[384];
	if (Desc.szDesc[0])
		sprintf(szText, m_szFormatAcup, Desc.szName, nLevel, Desc.szDesc);
	else
		sprintf(szText, m_szFormatAcupNone, Desc.szName, nLevel);
	m_AcupInfo.SetText(szText);
}

void KUiStatusMeridianPage::Refresh()
{
	KUiMeridianInfo Info;
	memset(&Info, 0, sizeof(Info));
	int nVersion = g_pCoreShell ? g_pCoreShell->GetGameData(GDI_MERIDIAN_INFO, (unsigned int)&Info, 0) : 0;
	if (nVersion == m_nVersion)
		return;
	m_nVersion = nVersion;

	int i, nLevel = Info.nLevel[m_nMeridian - 1];
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
	{
		m_BtnMeridian[i].CheckButton(i + 1 == m_nMeridian);
		m_BtnMeridian[i].SetLabelColor(i + 1 == m_nMeridian ? MAU_CHON : MAU_THUONG);
	}
	for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
		m_Acup[i].CheckButton(i < nLevel);		// khung 1 = huyet da xung (sang)
	SelectWay(m_nWay);

	char szText[256];
	sprintf(szText, m_szFormatMaterial, Info.nZhenYuan, Info.nHuMaiDan, Info.nDaHuMaiDan);
	m_Material.SetText(szText);

	// Huyet bam chon; mac dinh huyet ke tiep (hoac huyet cuoi khi mach da day)
	int nShow = m_nShowAcup;
	if (nShow <= 0)
		nShow = nLevel < MERIDIAN_PAGE_ACUPS ? nLevel + 1 : MERIDIAN_PAGE_ACUPS;
	ShowAcupoint(nShow);

	// Chu goi y chi dung cho mach dang chon; goi dong bo luc vao game khong co chu
	if (Info.nMeridian == m_nMeridian)
		m_Tips.SetText(Info.szTips);
	else
		m_Tips.SetText("");
}

void KUiStatusMeridianPage::CancelConfirm()
{
	if (m_uConfirmUntil)
	{
		m_uConfirmUntil = 0;
		m_BtnLevelUp.SetLabel(m_szLabelLevelUp);
	}
}

void KUiStatusMeridianPage::PaintWindow()
{
	if (m_uConfirmUntil && GetTickCount() >= m_uConfirmUntil)
		CancelConfirm();
	Refresh();
	KWndPage::PaintWindow();
}

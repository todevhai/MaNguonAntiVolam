/*****************************************************************************************
//	Bang F3 - trang Kinh mach. Xem UiStatusMeridian.h.
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../UiBase.h"
#include "UiStatusMeridian.h"
#include "UiMeridianConfirm.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/gamedatadef.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI_MERIDIAN	"UiStatusMeridian.ini"
#define	MAU_CHON			0xffe1d52b	// chu muc dang chon: SelectColor 225,213,43 cua jx9tn
#define	MAU_THUONG			0xffffffff
#define	TANG_KHI_DOANH		16			// jx9tn MIN_MERIDIAN_LEVEL: 8 mach du tang nay moi Khi Doanh

KUiStatusMeridianPage::KUiStatusMeridianPage()
{
	m_szScheme[0] = 0;
	memset(m_szLayout, 0, sizeof(m_szLayout));
	memset(m_szFormatZhenYuan, 0, sizeof(m_szFormatZhenYuan));
	memset(m_szFormatAcupTip, 0, sizeof(m_szFormatAcupTip));
	memset(m_nLevel, 0, sizeof(m_nLevel));
	m_nMeridian = 0;
	m_nVersion = -1;
}

void KUiStatusMeridianPage::Initialize()
{
	int i;
	AddChild(&m_BtnFullBreath);
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
		AddChild(&m_BtnMeridian[i]);
	for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
		AddChild(&m_Acup[i]);
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
		AddChild(&m_BreathPoint[i]);
	for (i = 0; i < MERIDIAN_BREATH_DAYS; i++)
		AddChild(&m_BtnBreathDays[i]);
	AddChild(&m_BreathInfo);
	AddChild(&m_CurZhenYuan);
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
	m_BtnFullBreath.Init(&Ini, "BtnFullBreath");

	// 8 nut mach dung chung mau [MeridianConfig], xep doc cach nhau Step diem.
	int nLeft = 0, nTop = 0, nStep = 24;
	Ini.GetInteger("MeridianConfig", "Left", 0, &nLeft);
	Ini.GetInteger("MeridianConfig", "Top", 0, &nTop);
	Ini.GetInteger("MeridianConfig", "Step", 24, &nStep);
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
	{
		char szKey[32], szLabel[32];
		m_BtnMeridian[i].Init(&Ini, "MeridianConfig");
		m_BtnMeridian[i].SetPosition(nLeft, nTop + i * nStep);
		sprintf(szKey, "Label_%d", i);
		Ini.GetString("MeridianConfig", szKey, "", szLabel, sizeof(szLabel));
		m_BtnMeridian[i].SetLabel(szLabel);
		// vi tri 16 huyet cua mach nay: tep ini trong pak, duong dan tuong doi voi bo giao dien
		sprintf(szKey, "Meridian_%d", i);
		Ini.GetString("MeridianConfig", szKey, "", Buff, sizeof(Buff));
		sprintf(m_szLayout[i], "%s%s", pScheme, Buff);

		sprintf(szKey, "imgBreathPoint_%d", i);
		m_BreathPoint[i].Init(&Ini, szKey);
	}
	m_BtnBreathDays[0].Init(&Ini, "Btn1DayBreath");
	m_BtnBreathDays[1].Init(&Ini, "Btn7DaysBreath");
	m_BtnBreathDays[2].Init(&Ini, "Btn30DaysBreath");
	m_BreathInfo.Init(&Ini, "BreathBuffInfo");
	{
		// ini khong chua duoc xuong dong: cac dong Line_0.. ghep bang '\n' (chu jx9tn BUFFINFO1)
		char szAll[512], szLine[128], szKey[16];
		szAll[0] = 0;
		for (i = 0; i < 8; i++)
		{
			sprintf(szKey, "Line_%d", i);
			if (!Ini.GetString("BreathBuffInfo", szKey, "", szLine, sizeof(szLine)))
				break;
			if (i)
				strcat(szAll, "\n");
			if (strlen(szAll) + strlen(szLine) + 2 < sizeof(szAll))
				strcat(szAll, szLine);
		}
		m_BreathInfo.SetText(szAll);
	}
	m_CurZhenYuan.Init(&Ini, "txtCurZYCount");
	// Chu co dau nam o ini (TCVN3); nguon chi giu ASCII.
	Ini.GetString("txtCurZYCount", "Format", "%d", m_szFormatZhenYuan, sizeof(m_szFormatZhenYuan));
	Ini.GetString("Messages", "AcupTip", "%s", m_szFormatAcupTip, sizeof(m_szFormatAcupTip));

	Select(m_nMeridian);
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

// Trang vua duoc chon (KWndPageSet::ActivePage): xin may chu cap 8 mach + Chan Nguyen moi nhat.
void KUiStatusMeridianPage::Show()
{
	KWndPage::Show();
	if (g_pCoreShell)
		g_pCoreShell->OperationRequest(GOI_MERIDIAN, 0, 0);
	Select(m_nMeridian);		// Show() hien lai MOI con: an phan khong thuoc muc dang chon
}

void KUiStatusMeridianPage::Select(int nMeridian)
{
	if (nMeridian < 0 || nMeridian > MERIDIAN_PAGE_COUNT)
		return;
	m_nMeridian = nMeridian;
	int i;
	m_BtnFullBreath.SetLabelColor(nMeridian == 0 ? MAU_CHON : MAU_THUONG);
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
		m_BtnMeridian[i].SetLabelColor(i + 1 == nMeridian ? MAU_CHON : MAU_THUONG);
	// Khi Doanh: vong 8 diem + dong trang thai + nut ngay; mach: 16 huyet
	for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
	{
		if (nMeridian)
			m_Acup[i].Show();
		else
			m_Acup[i].Hide();
	}
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
	{
		if (nMeridian)
			m_BreathPoint[i].Hide();
		else
			m_BreathPoint[i].Show();
	}
	for (i = 0; i < MERIDIAN_BREATH_DAYS; i++)
	{
		if (nMeridian)
			m_BtnBreathDays[i].Hide();
		else
			m_BtnBreathDays[i].Show();
	}
	if (nMeridian)
	{
		m_BreathInfo.Hide();
		LoadAcupointLayout(nMeridian);
	}
	else
		m_BreathInfo.Show();
	m_nVersion = -1;
}

void KUiStatusMeridianPage::ShowMessage(const char* pszKey)
{
	KSystemMessage	Msg;
	Msg.byConfirmType = SMCT_NONE;
	Msg.byParamSize = 0;
	Msg.byPriority = 0;
	Msg.eType = SMT_NORMAL;
	Msg.uReservedForUi = 0;
	Msg.szMessage[0] = 0;
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", m_szScheme, SCHEME_INI_MERIDIAN);
	if (Ini.Load(Buff) && Ini.GetString("Messages", pszKey, "", Msg.szMessage, sizeof(Msg.szMessage)) &&
		Msg.szMessage[0])
		KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
}

void KUiStatusMeridianPage::OnAcupointClick(int nIndex)
{
	if (m_nMeridian < 1)
		return;
	int nLevel = m_nLevel[m_nMeridian - 1];
	if (nIndex < nLevel)
		ShowMessage("AcupExist");			// jx9tn G_MERIDIAN_ACU_EXIST
	else if (nIndex > nLevel)
		ShowMessage("WrongOrder");			// jx9tn G_MERIDIAN_WRONG_ORDER
	else
		KUiMeridianConfirm::OpenWindow(m_nMeridian, nIndex + 1);
}

int KUiStatusMeridianPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_BUTTON_CLICK)
	{
		int i;
		if (uParam == (unsigned int)(KWndWindow*)&m_BtnFullBreath)
		{
			Select(0);
			return 0;
		}
		for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_BtnMeridian[i])
			{
				Select(i + 1);
				return 0;
			}
		}
		for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_Acup[i])
			{
				m_nVersion = -1;		// bam lam tat khung sang cua huyet: ve lai
				OnAcupointClick(i);
				return 0;
			}
		}
		for (i = 0; i < MERIDIAN_BREATH_DAYS; i++)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_BtnBreathDays[i])
			{
				ShowMessage("BreathNotOpen");
				return 0;
			}
		}
	}
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

void KUiStatusMeridianPage::Refresh()
{
	KUiMeridianInfo Info;
	memset(&Info, 0, sizeof(Info));
	int nVersion = g_pCoreShell ? g_pCoreShell->GetGameData(GDI_MERIDIAN_INFO, (unsigned int)&Info, 0) : 0;
	if (nVersion == m_nVersion)
		return;
	m_nVersion = nVersion;

	int i;
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
	{
		m_nLevel[i] = Info.nLevel[i];
		m_BreathPoint[i].SetFrame(Info.nLevel[i] >= TANG_KHI_DOANH ? 1 : 0);
	}

	if (m_nMeridian >= 1)
	{
		int nLevel = m_nLevel[m_nMeridian - 1];
		for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
		{
			m_Acup[i].CheckButton(i < nLevel);		// khung 1 = huyet da xung (sang)
			// chu khi re chuot: ten huyet
			KUiMeridianAcupDesc Desc;
			memset(&Desc, 0, sizeof(Desc));
			Desc.nMeridian = m_nMeridian;
			Desc.nLevel = i + 1;
			if (g_pCoreShell && g_pCoreShell->GetGameData(GDI_MERIDIAN_ACUP_DESC, (unsigned int)&Desc, 0))
			{
				char szTip[128];
				sprintf(szTip, m_szFormatAcupTip, Desc.szName);
				m_Acup[i].SetToolTipInfo(szTip, (int)strlen(szTip));
			}
		}
	}

	char szText[128];
	sprintf(szText, m_szFormatZhenYuan, Info.nZhenYuan);
	m_CurZhenYuan.SetText(szText);
}

void KUiStatusMeridianPage::PaintWindow()
{
	Refresh();
	KWndPage::PaintWindow();
}

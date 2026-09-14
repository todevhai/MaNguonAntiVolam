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
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"

extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;

#define	SCHEME_INI_MERIDIAN	"UiStatusMeridian.ini"
#define	MAU_CHON			0xffe1d52b	// chu muc dang chon: SelectColor 225,213,43 cua jx9tn
#define	MAU_THUONG			0xffffffff
#define	MAU_DUONG_NOI		0xff101010	// duong noi cac huyet
#define	MAU_VACH_DAN		0xffe8e8e8	// vach tu ten toi huyet
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
	memset(m_szBreathLine, 0, sizeof(m_szBreathLine));
	memset(m_szUnit, 0, sizeof(m_szUnit));
	memset(m_szTarget, 0, sizeof(m_szTarget));
	m_uBreathPainted = 0;
}

void KUiStatusMeridianPage::Initialize()
{
	int i;
	AddChild(&m_BtnFullBreath);
	for (i = 0; i < MERIDIAN_PAGE_COUNT; i++)
		AddChild(&m_BtnMeridian[i]);
	for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
	{
		AddChild(&m_Acup[i]);
		AddChild(&m_AcupName[i]);
		m_nAcupRight[i] = 0;
	}
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
		// Chu jx9tn G_STR_FULL_BREATH_BUFFINFO*, tach dong vi ini khong chua duoc xuong dong
		static const char* s_pszKey[4] = { "Duration", "Add", "Minus", "None" };
		static const char* s_pszUnit[3] = { "UnitDay", "UnitHour", "UnitMinute" };
		char szKey[16];
		for (i = 0; i < 4; i++)
			Ini.GetString("BreathBuffInfo", s_pszKey[i], "", m_szBreathLine[i], sizeof(m_szBreathLine[i]));
		for (i = 0; i < 3; i++)
			Ini.GetString("BreathBuffInfo", s_pszUnit[i], "", m_szUnit[i], sizeof(m_szUnit[i]));
		for (i = 0; i < 5; i++)
		{
			sprintf(szKey, "%d_Target", i);
			Ini.GetString("EffectTarget", szKey, "", m_szTarget[i], sizeof(m_szTarget[i]));
		}
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
	// [AcupTxt]: kich thuoc o ten + LeftX / RightX; [AcupPos] Acup_N: 0 ten ben trai, 1 ben phai
	int nTxtW = 70, nTxtH = 16, nLeftX = 0, nRightX = 0;
	Ini.GetInteger("AcupTxt", "Width", 70, &nTxtW);
	Ini.GetInteger("AcupTxt", "Height", 16, &nTxtH);
	Ini.GetInteger("AcupTxt", "LeftX", 0, &nLeftX);
	Ini.GetInteger("AcupTxt", "RightX", 0, &nRightX);
	for (int i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
	{
		char szSection[32];
		sprintf(szSection, "btnAcupuncturePoint_%d", i);
		m_Acup[i].Init(&Ini, szSection);
		sprintf(szSection, "Acup_%d", i);
		Ini.GetInteger("AcupPos", szSection, 0, &m_nAcupRight[i]);
		m_AcupName[i].Init(&Ini, "AcupTxt");		// co chu, mau cua jx9tn
		m_AcupName[i].SetHAlign(!m_nAcupRight[i]);
		int nX, nY, nW, nH;
		m_Acup[i].GetPosition(&nX, &nY);
		m_Acup[i].GetSize(&nW, &nH);
		m_AcupName[i].SetPosition(m_nAcupRight[i] ? nRightX : nLeftX, nY + (nH - nTxtH) / 2);
		KUiMeridianAcupDesc Desc;
		memset(&Desc, 0, sizeof(Desc));
		Desc.nMeridian = nMeridian;
		Desc.nLevel = i + 1;
		if (g_pCoreShell && g_pCoreShell->GetGameData(GDI_MERIDIAN_ACUP_DESC, (unsigned int)&Desc, 0))
			m_AcupName[i].SetText(Desc.szName);
		else
			m_AcupName[i].SetText("");
	}
}

// Duong noi 16 huyet (den, day 2 diem) va vach tu o ten toi huyet (sang) nhu jx9tn. RU_T_LINE dung
// toa do man hinh; ve sau nen trang, truoc cac cua so con (cham huyet, chu) nen nam duoi chung.
void KUiStatusMeridianPage::PaintAcupointLines()
{
	if (!g_pRepresentShell || m_nMeridian < 1)
		return;
	KRULine	Lines[MERIDIAN_PAGE_ACUPS * 3];
	int		nCount = 0, i, nX0, nY0, nX, nY, nW, nH, nPrevX = 0, nPrevY = 0;
	GetAbsolutePos(&nX0, &nY0);
	for (i = 0; i < MERIDIAN_PAGE_ACUPS; i++)
	{
		m_Acup[i].GetPosition(&nX, &nY);
		m_Acup[i].GetSize(&nW, &nH);
		int nCX = nX0 + nX + nW / 2, nCY = nY0 + nY + nH / 2;
		if (i > 0)
		{
			for (int t = 0; t < 2; t++)
			{
				KRULine& l = Lines[nCount++];
				l.Color.Color_dw = MAU_DUONG_NOI;
				l.oPosition.nX = nPrevX + t;	l.oPosition.nY = nPrevY;	l.oPosition.nZ = 0;
				l.oEndPos.nX = nCX + t;			l.oEndPos.nY = nCY;			l.oEndPos.nZ = 0;
			}
		}
		nPrevX = nCX;
		nPrevY = nCY;
		int nTX, nTY, nTW, nTH;
		m_AcupName[i].GetPosition(&nTX, &nTY);
		m_AcupName[i].GetSize(&nTW, &nTH);
		KRULine& v = Lines[nCount++];
		v.Color.Color_dw = MAU_VACH_DAN;
		v.oPosition.nX = nX0 + (m_nAcupRight[i] ? nTX : nTX + nTW);
		v.oPosition.nY = nCY;	v.oPosition.nZ = 0;
		v.oEndPos.nX = nX0 + (m_nAcupRight[i] ? nX + nW : nX);
		v.oEndPos.nY = nCY;		v.oEndPos.nZ = 0;
	}
	g_pRepresentShell->DrawPrimitives(nCount, Lines, RU_T_LINE, true);
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
		{
			m_Acup[i].Show();
			m_AcupName[i].Show();
		}
		else
		{
			m_Acup[i].Hide();
			m_AcupName[i].Hide();
		}
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
				static const int s_nDays[MERIDIAN_BREATH_DAYS] = { 1, 7, 30 };
				if (g_pCoreShell)
					g_pCoreShell->OperationRequest(GOI_MERIDIAN, UI_MERIDIAN_BREATH, s_nDays[i]);
				m_uBreathPainted = 0;
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

// jx9tn BUFFINFO1..4: "duy tri: Khong" / "%d ngay|gio|phut" + he bi khac tang/giam 15%.
void KUiStatusMeridianPage::RefreshBreathInfo()
{
	unsigned int uNow = GetTickCount();
	if (m_uBreathPainted && uNow - m_uBreathPainted < 1000)
		return;
	m_uBreathPainted = uNow ? uNow : 1;
	KUiMeridianInfo Info;
	memset(&Info, 0, sizeof(Info));
	if (g_pCoreShell)
		g_pCoreShell->GetGameData(GDI_MERIDIAN_INFO, (unsigned int)&Info, 0);
	const char* pszTarget = (Info.nSeries >= 0 && Info.nSeries < 5) ? m_szTarget[Info.nSeries] : "";
	char szTime[32], szAdd[96], szMinus[96], szText[256];
	if (Info.nBreathSeconds > 0)
	{
		// lam tron len: vua them 7 ngay thi hien 7, khong phai 6 ngay 23 gio
		int nDays = (Info.nBreathSeconds + 86399) / 86400, nHours = (Info.nBreathSeconds + 3599) / 3600;
		if (Info.nBreathSeconds >= 86400)
			sprintf(szTime, "%d %s", nDays, m_szUnit[0]);
		else if (Info.nBreathSeconds >= 3600)
			sprintf(szTime, "%d %s", nHours, m_szUnit[1]);
		else
			sprintf(szTime, "%d %s", (Info.nBreathSeconds + 59) / 60, m_szUnit[2]);
		char szPercent[16];
		sprintf(szPercent, "%d%%", UI_MERIDIAN_BREATH_PERCENT);
		sprintf(szAdd, m_szBreathLine[1], pszTarget, szPercent);
		sprintf(szMinus, m_szBreathLine[2], pszTarget, szPercent);
	}
	else
	{
		strcpy(szTime, m_szBreathLine[3]);
		sprintf(szAdd, m_szBreathLine[1], pszTarget, m_szBreathLine[3]);
		sprintf(szMinus, m_szBreathLine[2], pszTarget, m_szBreathLine[3]);
	}
	char szDuration[96];
	sprintf(szDuration, m_szBreathLine[0], szTime);
	sprintf(szText, "%s\n\n%s\n\n%s", szDuration, szAdd, szMinus);
	m_BreathInfo.SetText(szText);
}

void KUiStatusMeridianPage::PaintWindow()
{
	if (m_nMeridian == 0)
		RefreshBreathInfo();
	Refresh();
	KWndPage::PaintWindow();
	PaintAcupointLines();
}

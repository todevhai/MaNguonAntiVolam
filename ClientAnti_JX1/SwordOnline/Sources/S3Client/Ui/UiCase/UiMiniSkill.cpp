/*****************************************************************************************
//	Thanh mini skill - xem UiMiniSkill.h
*****************************************************************************************/
#include "KWin32.h"
#include "KEngine.h"					// g_DebugLog
#include "KIniFile.h"
#include "../Elem/Wnds.h"
#include "../Elem/WndMessage.h"
#include "UiMiniSkill.h"
#include "../UiBase.h"
#include "../../../core/src/coreshell.h"

extern iCoreShell*	g_pCoreShell;

#define	MINI_SKILL_INI			"\\Ui\\MiniSkill.ini"
#define	MINI_SKILL_FPS			18		// nhip logic client (S3Client.cpp GAME_FPS)
#define	MINI_SKILL_MAX_FETCH	64		// so trang thai xin tu Core moi nhip
#define	MINI_SKILL_WARNING_SEC	10		// con it hon so giay nay thi chu doi mau canh bao

KUiMiniSkill* KUiMiniSkill::m_pSelf = NULL;

KUiMiniSkill::KUiMiniSkill()
{
	m_nShown = 0;
	m_nBuffCount = 0;
	m_nIconWidth = 24;
	m_nIconHeight = 24;
	m_nTimeHeight = 12;
	m_uBuffColor = m_uDebuffColor = m_uWarningColor = 0xff00ff00;
	for (int i = 0; i < UI_MAX_STATE_SKILL; i++)
	{
		m_nSlotSkill[i] = 0;
		m_nSlotSecond[i] = -2;
	}
}

KUiMiniSkill* KUiMiniSkill::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMiniSkill;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
		m_pSelf->Show();
	return m_pSelf;
}

void KUiMiniSkill::CloseWindow(bool bDestroy)
{
	if (m_pSelf == NULL)
		return;
	if (bDestroy)
	{
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
	else
		m_pSelf->Hide();
}

void KUiMiniSkill::Initialize()
{
	for (int i = 0; i < UI_MAX_STATE_SKILL; i++)
	{
		AddChild(&m_Icon[i]);
		AddChild(&m_Time[i]);
	}
	LoadScheme();
	Wnd_AddWindow(this, WL_TOPMOST);
}

void KUiMiniSkill::LoadScheme()
{
	KIniFile Ini;
	if (!Ini.Load(MINI_SKILL_INI))
		return;

	int nLeft = 120, nTop = 48;
	Ini.GetInteger("Main", "Left", nLeft, &nLeft);
	Ini.GetInteger("Main", "Top", nTop, &nTop);
	SetPosition(nLeft, nTop);

	char szColor[32];
	Ini.GetString("txtBuffTime", "Color", "0,255,0", szColor, sizeof(szColor));
	m_uBuffColor = GetColor(szColor);
	Ini.GetString("txtDebuffTime", "Color", "255,140,0", szColor, sizeof(szColor));
	m_uDebuffColor = GetColor(szColor);
	Ini.GetString("txtWarningTime", "Color", "255,0,0", szColor, sizeof(szColor));
	m_uWarningColor = GetColor(szColor);

	for (int i = 0; i < UI_MAX_STATE_SKILL; i++)
	{
		m_Icon[i].Init(&Ini, "BuffImage");
		m_Time[i].Init(&Ini, "txtBuffTime");
		m_Icon[i].Hide();
		m_Time[i].Hide();
	}
	int nW = 0, nH = 0;
	m_Icon[0].GetSize(&nW, &nH);
	m_nIconWidth = nW > 0 ? nW : 24;
	m_nIconHeight = nH > 0 ? nH : 24;
	m_Time[0].GetSize(&nW, &nH);
	m_nTimeHeight = nH > 0 ? nH : 12;

	// Muc co the thieu so (tep khai BuffCount=210 nhung chi co 206 muc): muc khong co ID thi bo.
	int nCount = 0;
	Ini.GetInteger("BuffList", "BuffCount", 0, &nCount);
	m_nBuffCount = 0;
	for (int i = 0; i < nCount && m_nBuffCount < MINI_SKILL_MAX_BUFF; i++)
	{
		char szKey[32];
		KMiniSkillBuff& Buff = m_Buff[m_nBuffCount];
		sprintf(szKey, "Buff_%d_ID", i);
		Ini.GetInteger("BuffList", szKey, 0, &Buff.nSkillId);
		if (Buff.nSkillId <= 0)
			continue;
		sprintf(szKey, "Buff_%d_Name", i);
		Ini.GetString("BuffList", szKey, "", Buff.szName, sizeof(Buff.szName));
		sprintf(szKey, "Buff_%d_Image", i);
		Ini.GetString("BuffList", szKey, "", Buff.szImage, sizeof(Buff.szImage));
		sprintf(szKey, "Buff_%d_Desc", i);
		Ini.GetString("BuffList", szKey, "", Buff.szDesc, sizeof(Buff.szDesc));
		sprintf(szKey, "Buff_%d_IsDebuff", i);
		Ini.GetInteger("BuffList", szKey, 0, &Buff.bDebuff);
		sprintf(szKey, "Buff_%d_IsAura", i);
		Ini.GetInteger("BuffList", szKey, 0, &Buff.bAura);
		m_nBuffCount++;
	}
	SetSize(0, m_nIconHeight + m_nTimeHeight);
}

const KMiniSkillBuff* KUiMiniSkill::FindBuff(int nSkillId) const
{
	for (int i = 0; i < m_nBuffCount; i++)
	{
		if (m_Buff[i].nSkillId == nSkillId)
			return &m_Buff[i];
	}
	return NULL;
}

void KUiMiniSkill::UpdateSlot(int nSlot, const KUiStateSkill& State, const KMiniSkillBuff& Buff)
{
	if (m_nSlotSkill[nSlot] != State.nSkillId)
	{
		m_nSlotSkill[nSlot] = State.nSkillId;
		m_nSlotSecond[nSlot] = -2;
		m_Icon[nSlot].SetImage(ISI_T_SPR, Buff.szImage);
		char szTip[300];
		sprintf(szTip, "%s\n%s", Buff.szName, Buff.szDesc);
		m_Icon[nSlot].SetToolTipInfo(szTip, (int)strlen(szTip));
		m_Icon[nSlot].SetPosition(nSlot * m_nIconWidth, 0);
		m_Time[nSlot].SetPosition(nSlot * m_nIconWidth, m_nIconHeight);
		g_DebugLog("[MiniSkill] o %d: chieu %d cap %d con %d nhip", nSlot, State.nSkillId, State.nLevel, State.nLeftFrames);
	}

	// Chi dem nguoc khi con TREN 1 giay. Trang thai do vong sang tha (La Han Tran...) duoc lam
	// moi lien tuc nen luon con ~1 giay - ghi "1s" dung yen vo nghia. Khong han (-1) / co
	// IsAura trong ini cung khong ghi so.
	int nSecond = -1;
	if (State.nLeftFrames >= 0 && !Buff.bAura)
	{
		nSecond = (State.nLeftFrames + MINI_SKILL_FPS - 1) / MINI_SKILL_FPS;
		if (nSecond <= 1)
			nSecond = -1;
	}
	if (nSecond == m_nSlotSecond[nSlot])
		return;
	m_nSlotSecond[nSlot] = nSecond;

	char szTime[16];
	szTime[0] = 0;
	if (nSecond >= 3600)
		sprintf(szTime, "%dh", nSecond / 3600);
	else if (nSecond >= 60)
		sprintf(szTime, "%d:%02d", nSecond / 60, nSecond % 60);	// phut:giay de thay giay chay
	else if (nSecond >= 0)
		sprintf(szTime, "%ds", nSecond);
	m_Time[nSlot].SetText(szTime);

	unsigned int uColor = Buff.bDebuff ? m_uDebuffColor : m_uBuffColor;
	if (nSecond >= 0 && nSecond <= MINI_SKILL_WARNING_SEC)
		uColor = m_uWarningColor;
	m_Time[nSlot].SetTextColor(uColor);
}

void KUiMiniSkill::Breathe()
{
	if (g_pCoreShell == NULL)
		return;
	// Lay nhieu hon so o hien thi: trang thai khong khai trong ini se bi loc bo ben duoi.
	KUiStateSkill State[MINI_SKILL_MAX_FETCH];
	int nCount = g_pCoreShell->GetGameData(GDI_PLAYER_STATE_SKILLS, (unsigned int)State, MINI_SKILL_MAX_FETCH);
	if (nCount < 0)
		nCount = 0;
	if (nCount > MINI_SKILL_MAX_FETCH)
		nCount = MINI_SKILL_MAX_FETCH;

	// Chi hien buff khai trong MiniSkill.ini (chieu tay phai tac len nguoi choi - cua minh
	// hay cua nguoi khac). Trang thai noi bo khac (chieu con, hieu ung trang bi...) bo qua:
	// icon cua chung trong skills.txt la anh 36 diem, ve vao o 24 thi de len nhau.
	int nShow = 0;
	for (int i = 0; i < nCount && nShow < UI_MAX_STATE_SKILL; i++)
	{
		const KMiniSkillBuff* pBuff = FindBuff(State[i].nSkillId);
		if (pBuff == NULL)
			continue;
		UpdateSlot(nShow, State[i], *pBuff);
		nShow++;
	}
	nCount = nShow;

	for (int i = 0; i < nCount; i++)
	{
		if (i >= m_nShown)
		{
			m_Icon[i].Show();
			m_Time[i].Show();
		}
	}
	for (int i = nCount; i < m_nShown; i++)
	{
		m_Icon[i].Hide();
		m_Time[i].Hide();
		m_nSlotSkill[i] = 0;
	}
	if (nCount != m_nShown)
	{
		m_nShown = nCount;
		// Chi chiem dung phan co icon, khong nuot cu bam xuong mat dat ben canh.
		SetSize(nCount * m_nIconWidth, m_nIconHeight + m_nTimeHeight);
	}
}

int KUiMiniSkill::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	// Icon chi de xem (tooltip), bam vao khong lam gi.
	if (uMsg == WND_N_BUTTON_CLICK)
		return 0;
	return KWndWindow::WndProc(uMsg, uParam, nParam);
}

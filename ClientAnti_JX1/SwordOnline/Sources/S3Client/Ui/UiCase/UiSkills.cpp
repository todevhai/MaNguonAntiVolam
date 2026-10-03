/*****************************************************************************************
//	½çÃæ--ÁÄÌì´°¿Ú
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-27
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiSkills.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"
#include "crtdbg.h"
#include "../UiSoundSetting.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

extern iCoreShell*		g_pCoreShell;

#define 	SCHEME_INI_SHEET			"UiSkillSheet.ini"
#define 	SCHEME_INI_LIVE				"UiSkillLive.ini"
#define 	SCHEME_INI_FIGHT			"UiSkillFight.ini"
#define 	SCHEME_INI_FIGHT_SUB_PAGE	"UiSkillFightSub.ini"
#define		SET_NEW_SKILL_TO_IMMED_SKILL_LEVEL_RANGE	9
#define		AUTO_SET_IMMED_SKILL_MSG_ID					"24"

KUiFightSkillSubPage::KUiFightSkillSubPage()
{
	m_nRemainSkillPoint = 0;
//	m_nSubPagIndex = 0;
}

//³õÊ¼»¯
void KUiFightSkillSubPage::Initialize(/*int nSubPageIndex*/)
{
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i ++)
	{
		AddChild(&m_FightSkills[i]);
		m_FightSkills[i].Celar();
		m_FightSkills[i].SetContainerId((int)UOC_SKILL_LIST);
		AddChild(&m_ConDiemBtn[i]);
		m_ConDiemBtn[i].SetText("+");
	}
//	m_nSubPagIndex = nSubPageIndex;
}

//ÔØÈë½çÃæ·½°¸
void KUiFightSkillSubPage::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\" SCHEME_INI_FIGHT_SUB_PAGE, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
		{
			sprintf(Buff, "Skill_%d", i);
			m_FightSkills[i].Init(&Ini, Buff);
			m_FightSkills[i].EnablePickPut(false);
			m_ConDiemBtn[i].Init(&Ini, "ConDiemBtn");
			m_ConDiemBtn[i].SetText("+");
			{
				int nL = 0, nT = 0, nW = 0, nH = 0;
				m_FightSkills[i].GetPosition(&nL, &nT);
				m_FightSkills[i].GetSize(&nW, &nH);
				/* O vuong 13x13 de len GOC DUOI PHAI icon chieu. Ban dau
				   11x11 khong nen, chi mot dau + vang lot thom: kho thay va
				   kho bam. Nen do + vien den do KUiFightSkillSubPage::
				   PaintWindow ve (xem ban va ben duoi) vi KWndPureTextBtn chi
				   biet ve chu con KWndWindow khong co nen mau. */
				m_ConDiemBtn[i].SetSize(13, 13);
				m_ConDiemBtn[i].SetPosition(nL + nW - 13, nT + nH - 13);
			}
		}

		Ini.GetInteger("SkillText", "Font", 12, &m_SkillTextParam.nFont);
		Ini.GetInteger2("SkillText", "Offset",
			(int*)&m_SkillTextParam.Offset.cx, (int*)&m_SkillTextParam.Offset.cy);
		Ini.GetString("SkillText", "Color", "", Buff, 16);
		m_SkillTextParam.Color = GetColor(Buff);
	}
}

//¸üÐÂÉý¼¶µãÊý
void KUiFightSkillSubPage::UpdateRemainPoint(int nPoint)
{
	m_nRemainSkillPoint = nPoint;
}

//¸üÐÂÉý¼¶µãÊý
void KUiFightSkillSubPage::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	_ASSERT(pSkill && nIndex >= 0 && nIndex < FIGHT_SKILL_COUNT_PER_PAGE);
	m_FightSkills[nIndex].HoldObject(pSkill->uGenre, pSkill->uId, pSkill->nLevel, pSkill->nThem);
}

//¸üÐÂÊý¾Ý
void KUiFightSkillSubPage::UpdateData(KUiSkillData* pSkills)
{
	_ASSERT(pSkills);
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
		m_FightSkills[i].HoldObject(pSkills[i].uGenre, pSkills[i].uId, pSkills[i].nLevel, pSkills[i].nThem);
}

//´°¿Úº¯Êý
/* Ve chieu dang cam theo con tro. Wnd_RenderWindows goi moi khung hinh. */
static int VeChieuDangCam(int x, int y, const KUiDraggedObject& Obj, int nDropQueryResult)
{
	if (g_pCoreShell && Obj.uGenre != CGOG_NOTHING)
		g_pCoreShell->DrawGameObj(Obj.uGenre, Obj.uId, x - 16, y - 16, 32, 32, 0);
	return 1;
}

int	KUiFightSkillSubPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	/* Nut "+" cua o nao thi cong diem cho chieu dang nam o do. */
	if (uMsg == WND_N_BUTTON_CLICK && uParam)
	{
		for (int nO = 0; nO < FIGHT_SKILL_COUNT_PER_PAGE; nO++)
		{
			if ((KWndWindow*)uParam != (KWndWindow*)&m_ConDiemBtn[nO])
				continue;
			KUiDraggedObject Obj;
			m_FightSkills[nO].GetObject(Obj);
			if (Obj.uGenre == CGOG_NOTHING || m_nRemainSkillPoint <= 0)
				return 0;
			m_nRemainSkillPoint--;
			g_pCoreShell->OperationRequest(GOI_TONE_UP_SKILL, CGOG_SKILL_FIGHT, Obj.uId);
			return 0;
		}
		return 0;
	}
	if (0)	/* cong diem chuyen sang nut dau cong rieng, xem ghi chu tren */
	{
		KUiDraggedObject* pObj = (KUiDraggedObject*)uParam;
		if (pObj->uGenre != CGOG_NOTHING)
		{
			m_nRemainSkillPoint --;	// Ê¹ÓÃ¼¼ÄÜµãÊý
			g_pCoreShell->OperationRequest(GOI_TONE_UP_SKILL, CGOG_SKILL_FIGHT, pObj->uId);
		}
		return 0;
	}

	/* Click vao chieu: NHAC len con tro de dat vao o phim tat.
	   Cong diem la viec cua nut dau cong rieng, khong phai cua cu click nay. */
	if (uMsg == WND_N_LEFT_CLICK_ITEM && uParam)
	{
		KUiDraggedObject* pChieu = (KUiDraggedObject*)uParam;
		if (pChieu->uGenre != CGOG_NOTHING)
			Wnd_DragBegin(pChieu, VeChieuDangCam);
		return 0;
	}
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

//»æÖÆ´°¿Ú
/* O vuong DO vien DEN, dau "+" TRANG canh giua, ve o goc duoi phai
   icon chieu. Ke bang KRULine chu khong dung anh: khong phai them tai
   nguyen vao pak.

   Tu ve chu chu khong goi KWndPureTextBtn::PaintWindow: ham do dat chu
   o DINH cua so (param.nY = m_nAbsoluteTop) va tinh be ngang theo
   nFontSize/2, nen dau + luon lech len tren va sang trai. Do tren anh
   chup 07/09/2026: glyph "+" font 12 rong 5px cao 7px va co san 1px dem
   phia tren -> lech 1px moi chieu trong o 13x13. */
void KNutCongDiem::PaintWindow()
{
	if (!m_bCoChieu || g_pRepresentShell == NULL || m_Width <= 4 || m_Height <= 4)
		return;
	KRULine To[40];
	int nSo = 0;
	/* Nen do: chua tu hang 2 de danh cho vien DAY 2px. */
	for (int nY = 2; nY < m_Height - 2 && nSo < 36; nY++, nSo++)
	{
		To[nSo].Color.Color_dw = 0xffb02020;
		To[nSo].oPosition.nX = m_nAbsoluteLeft + 2;
		To[nSo].oEndPos.nX = m_nAbsoluteLeft + m_Width - 2;
		To[nSo].oPosition.nY = To[nSo].oEndPos.nY = m_nAbsoluteTop + nY;
	}
	if (nSo > 0)
		g_pRepresentShell->DrawPrimitives(nSo, To, RU_T_LINE, true);
	/* Vien den DAY 2px: hai khung long nhau. */
	nSo = 0;
	for (int nV = 0; nV < 2; nV++)
	{
		int nL = m_nAbsoluteLeft + nV, nT = m_nAbsoluteTop + nV;
		int nR = m_nAbsoluteLeft + m_Width - 1 - nV;
		int nB = m_nAbsoluteTop + m_Height - 1 - nV;
		To[nSo].oPosition.nX = To[nSo].oEndPos.nX = nL;
		To[nSo].oPosition.nY = nT; To[nSo].oEndPos.nY = nB + 1; nSo++;
		To[nSo].oPosition.nX = To[nSo].oEndPos.nX = nR;
		To[nSo].oPosition.nY = nT; To[nSo].oEndPos.nY = nB + 1; nSo++;
		To[nSo].oPosition.nY = To[nSo].oEndPos.nY = nT;
		To[nSo].oPosition.nX = nL; To[nSo].oEndPos.nX = nR + 1; nSo++;
		To[nSo].oPosition.nY = To[nSo].oEndPos.nY = nB;
		To[nSo].oPosition.nX = nL; To[nSo].oEndPos.nX = nR + 1; nSo++;
	}
	for (int nK = 0; nK < nSo; nK++)
		To[nK].Color.Color_dw = 0xff100808;
	g_pRepresentShell->DrawPrimitives(nSo, To, RU_T_LINE, true);
	/* Dau "+": lay dung be rong/cao THAT cua glyph (5x7 o font 12) roi
	   canh giua, tru 1px dem san phia tren. */
	g_pRepresentShell->OutputText(12, "+", 1,
		m_nAbsoluteLeft + (m_Width - 5) / 2,
		m_nAbsoluteTop + (m_Height - 7) / 2 - 1,
		0xffffffff, 0xff000000);
}

void KUiFightSkillSubPage::PaintWindow()
{
	KWndPage::PaintWindow();
	for (int i = 0; i < FIGHT_SKILL_COUNT_PER_PAGE; i++)
	{
		KUiDraggedObject	Obj;
		m_FightSkills[i].GetObject(Obj);
		if (Obj.uGenre != CGOG_NOTHING)
		{				
			int nLeft, nTop, nWidth, nHeight;
			m_FightSkills[i].GetAbsolutePos(&nLeft, &nTop);
			m_FightSkills[i].GetSize(&nWidth, &nHeight);
			g_pCoreShell->DrawGameObj(CGOG_SKILL_LIVE, Obj.uId, nLeft, nTop, nWidth, nHeight, Obj.DataW);
			if (Obj.DataW)
			{
				char	szLevel[8];
				int		nLen;
				itoa(Obj.DataW, szLevel, 10);
				nLen = strlen(szLevel);
				nLeft += m_SkillTextParam.Offset.cx;
				nTop += m_SkillTextParam.Offset.cy;
				nLeft += (nWidth - nLen * m_SkillTextParam.nFont / 2) / 2;
				unsigned int dwColor; 
				/* Cap co phan do cong them (allskill_v) thi so mau xanh, cap goc thuan mau trang
				   (user chot 19/09/2026). DataH = KUiSkillData::nThem. */
				dwColor = (Obj.DataH > 0) ? 0xff64b4ff : m_SkillTextParam.Color;

				g_pRepresentShell->OutputText(m_SkillTextParam.nFont, szLevel, nLen, nLeft, nTop,
					dwColor, 0);
			}
		}
	}
	/* Nen cho nut "+": o vuong DO, vien DEN, o goc duoi phai moi o chieu.
	   Ve o day chu khong trong nut vi KWndPureTextBtn chi ve duoc CHU va
	   KWndWindow khong co nen mau. Trang duoc ve TRUOC cac cua so con nen
	   dau "+" trang cua nut nam de len nen nay. Ke bang KRULine ngang chu
	   khong dung anh: khong phai them tai nguyen vao pak. */
	/* O chieu TRONG thi giau han nut: khong dau "+" thua, va bam vao
	   cung khong cong duoc gi. Dat moi khung ve chu khong trong
	   UpdateData - khoi phai bat het cac duong lam doi noi dung o. */
	for (int nN = 0; nN < FIGHT_SKILL_COUNT_PER_PAGE; nN++)
	{
		KUiDraggedObject ObjN;
		m_FightSkills[nN].GetObject(ObjN);
		/* O DON DANH THUONG khong co nut "+": no la don co ban cua vu khi
		   (settings/vukhi-kynang-vatly.txt tra ve 53 cho moi loai), khong
		   phai chieu de nuoi diem ky nang. */
		int bCo = (ObjN.uGenre != CGOG_NOTHING) && ((int)ObjN.uId != 53);
		m_ConDiemBtn[nN].DatCoChieu(bCo);
		m_ConDiemBtn[nN].SetText(bCo ? "+" : "");
	}
}

//³õÊ¼»¯
void KUiFightSkillSub::Initialize()
{
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
	{
		m_SubPages[i].Initialize();
		AddPage(&m_SubPages[i], &m_SubPageBtn[i]);
	}
}

//ÔØÈë½çÃæ·½°¸
void KUiFightSkillSub::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\" SCHEME_INI_FIGHT, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPageSet::Init(&Ini, "Main");
		m_oFixPos.x = 0;
		m_oFixPos.y = 0;
		SetPosition(0, 0);
		for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		{
			m_SubPages[i].LoadScheme(pScheme);
			sprintf(Buff, "SubPageBtn_%d", i);
			m_SubPageBtn[i].Init(&Ini, Buff);
		}
		Show();
	}
}

//¸üÐÂÉý¼¶µãÊý
void KUiFightSkillSub::UpdateRemainPoint(int nPoint)
{
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		m_SubPages[i].UpdateRemainPoint(nPoint);
}

//¸üÐÂ¼¼ÄÜ
void KUiFightSkillSub::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	_ASSERT(pSkill);
	int nPage = nIndex / FIGHT_SKILL_COUNT_PER_PAGE;
	nIndex = nIndex % FIGHT_SKILL_COUNT_PER_PAGE;
	_ASSERT(nPage >= 0 && nPage < FIGHT_SKILL_SUB_PAGE_COUNT);
	m_SubPages[nPage].UpdateSkill(pSkill, nIndex);
}

//¸üÐÂÊý¾Ý
void KUiFightSkillSub::UpdateData()
{
	KUiSkillData	Skills[FIGHT_SKILL_COUNT];
	g_pCoreShell->GetGameData(GDI_FIGHT_SKILLS, (unsigned int)Skills, 0);
	for (int i = 0; i < FIGHT_SKILL_SUB_PAGE_COUNT; i++)
		m_SubPages[i].UpdateData(&Skills[i * FIGHT_SKILL_COUNT_PER_PAGE]);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
void KUiFightSkill::Initialize()
{
	m_InternalPad.Initialize();
	AddChild(&m_InternalPad);
	AddChild(&m_RemainSkillPoint);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë´°¿ÚµÄ½çÃæ·½°¸
//--------------------------------------------------------------------------
void KUiFightSkill::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\" SCHEME_INI_FIGHT, pScheme);
	if (Ini.Load(Buff))
	{
		KWndPage::Init(&Ini, "Main");
		m_InternalPad.LoadScheme(pScheme);
		m_RemainSkillPoint.Init(&Ini, "RemainPoint");
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂÊý¾Ý
//--------------------------------------------------------------------------
void KUiFightSkill::UpdateData()
{
	int nRemainSkillPoint = g_pCoreShell->GetGameData(GDI_FIGHT_SKILL_POINT, 0, 0);
	m_InternalPad.UpdateRemainPoint(nRemainSkillPoint);
	m_RemainSkillPoint.SetIntText(nRemainSkillPoint);
	m_InternalPad.UpdateData();
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂ¼¼ÄÜ
//--------------------------------------------------------------------------
void KUiFightSkill::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	m_InternalPad.UpdateSkill(pSkill, nIndex);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂÉý¼¶µãÊý
//--------------------------------------------------------------------------
void KUiFightSkill::UpdateRemainPoint(int nPoint)
{
	m_RemainSkillPoint.SetIntText(nPoint);
	m_InternalPad.UpdateRemainPoint(nPoint);

}

KUiLiveSkill::KUiLiveSkill()
{
	m_nRemainSkillPoint = 0;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
void KUiLiveSkill::Initialize()
{
	AddChild(&m_RemainSkillPoint);

	for (int i = 0; i < LIVE_SKILL_COUNT; i++)
	{
		m_LiveSkill[i].Celar();
		AddChild(&m_LiveSkill[i]);
		m_LiveSkill[i].SetContainerId((int)UOC_SKILL_LIST);
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë´°¿ÚµÄ½çÃæ·½°¸
//--------------------------------------------------------------------------
void KUiLiveSkill::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_LIVE);
	if (Ini.Load(Buff))
	{
		KWndImage::Init(&Ini, "Main");
		m_RemainSkillPoint.	Init(&Ini, "RemainPoint");

		for (int i = 0; i < LIVE_SKILL_COUNT; i++)
		{
			sprintf(Buff, "Skill_%d", i);
			m_LiveSkill[i].Init(&Ini, Buff);
			m_LiveSkill[i].EnablePickPut(false);
		}

		Ini.GetInteger("SkillText", "Font", 12, &m_SkillTextParam.nFont);
		Ini.GetInteger2("SkillText", "Offset",
			(int*)&m_SkillTextParam.Offset.cx, (int*)&m_SkillTextParam.Offset.cy);
		Ini.GetString("SkillText", "Color", "", Buff, 16);
		m_SkillTextParam.Color = GetColor(Buff);
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´°¿Úº¯Êý
//--------------------------------------------------------------------------
int KUiLiveSkill::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (0)	/* cong diem chuyen sang nut dau cong rieng, xem ghi chu tren */
	{
		KUiDraggedObject* pObj = (KUiDraggedObject*)uParam;
		if (pObj->uGenre != CGOG_NOTHING)
		{
			m_nRemainSkillPoint--;	// Ê¹ÓÃ¼¼ÄÜµãÊý
			g_pCoreShell->OperationRequest(GOI_TONE_UP_SKILL, CGOG_SKILL_LIVE, pObj->uId);
		}
		return 0;
	}
	/* Click vao chieu: NHAC len con tro de dat vao o phim tat.
	   Cong diem la viec cua nut dau cong rieng, khong phai cua cu click nay. */
	if (uMsg == WND_N_LEFT_CLICK_ITEM && uParam)
	{
		KUiDraggedObject* pChieu = (KUiDraggedObject*)uParam;
		if (pChieu->uGenre != CGOG_NOTHING)
			Wnd_DragBegin(pChieu, VeChieuDangCam);
		return 0;
	}
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»æÖÆ´°¿Ú
//--------------------------------------------------------------------------
void KUiLiveSkill::PaintWindow()
{
	KWndPage::PaintWindow();
	for (int i = 0; i < LIVE_SKILL_COUNT; i++)
	{
		KUiDraggedObject	Obj;
		m_LiveSkill[i].GetObject(Obj);
		if (Obj.uGenre != CGOG_NOTHING)
		{
			int nLeft, nTop, nWidth, nHeight;
			m_LiveSkill[i].GetAbsolutePos(&nLeft, &nTop);
			m_LiveSkill[i].GetSize(&nWidth, &nHeight);
			g_pCoreShell->DrawGameObj(CGOG_SKILL_LIVE, Obj.uId, nLeft, nTop, nWidth, nHeight, Obj.DataW);
			if (Obj.DataW)
			{
				char	szLevel[8];
				int		nLen;
				itoa(Obj.DataW, szLevel, 10);
				nLen = strlen(szLevel);
				nLeft += m_SkillTextParam.Offset.cx;
				nTop += m_SkillTextParam.Offset.cy;
				nLeft += (nWidth - nLen * m_SkillTextParam.nFont / 2) / 2;
				g_pRepresentShell->OutputText(m_SkillTextParam.nFont, szLevel, nLen, nLeft, nTop,
					m_SkillTextParam.Color, 0);
			}
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂ¼¼ÄÜ¹«¹²Êý¾Ý
//--------------------------------------------------------------------------
void KUiLiveSkill::UpdateBaseData()
{
	//´ÓÓÎÏ·ÊÀ½ç»ñÈ¡Éú»î¼¼ÄÜÊý¾Ý
	KUiPlayerLiveSkillBase	Info;
	g_pCoreShell->GetGameData(GDI_LIVE_SKILL_BASE, (unsigned int)&Info, 0);
	m_RemainSkillPoint.SetIntText(m_nRemainSkillPoint = Info.nRemainPoint);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂ¼¼ÄÜ
//--------------------------------------------------------------------------
void KUiLiveSkill::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	if (pSkill && nIndex >= 0 && nIndex < LIVE_SKILL_COUNT)
	{
		m_LiveSkill[nIndex].HoldObject(pSkill->uGenre, pSkill->uId, pSkill->nLevel, 0);
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂÊý¾Ý
//--------------------------------------------------------------------------
void KUiLiveSkill::UpdateData()
{
	UpdateBaseData();
	KUiSkillData	Skills[LIVE_SKILL_COUNT];
	g_pCoreShell->GetGameData(GDI_LIVE_SKILLS, (unsigned int)Skills, 0);
	for (int i = 0; i < LIVE_SKILL_COUNT; i++)
		m_LiveSkill[i].HoldObject(Skills[i].uGenre, Skills[i].uId, Skills[i].nLevel, 0);
}

KUiSkills* KUiSkills::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´ò¿ª´°¿Ú£¬·µ»ØÎ¨Ò»µÄÒ»¸öÀà¶ÔÏóÊµÀý
//--------------------------------------------------------------------------
KUiSkills* KUiSkills::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiSkills;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_FightSkillPad.UpdateData();
		m_pSelf->m_LiveSkillPad.UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÈç¹û´°¿ÚÕý±»ÏÔÊ¾£¬Ôò·µ»ØÊµÀýÖ¸Õë
//--------------------------------------------------------------------------
KUiSkills* KUiSkills::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¹Ø±Õ´°¿Ú£¬Í¬Ê±¿ÉÒÔÑ¡ÔòÊÇ·ñÉ¾³ý¶ÔÏóÊµÀý
//--------------------------------------------------------------------------
void KUiSkills::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy == false)
			m_pSelf->Hide();
		else
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º³õÊ¼»¯
//--------------------------------------------------------------------------
void KUiSkills::Initialize()
{
	m_FightSkillPad.Initialize();
	AddPage(&m_FightSkillPad, &m_FightSkillPadBtn);
	m_LiveSkillPad.Initialize();
	AddPage(&m_LiveSkillPad, &m_LiveSkillPadBtn);
//	AddChild(&m_FightText);
//	AddChild(&m_LiveText);
	AddChild(&m_Close);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÔØÈë´°¿ÚµÄ½çÃæ·½°¸
//--------------------------------------------------------------------------
void KUiSkills::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SHEET);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_FightSkillPadBtn.Init(&Ini, "FightBtn");
		m_pSelf->m_LiveSkillPadBtn .Init(&Ini, "LiveBtn");
		m_pSelf->m_Close           .Init(&Ini, "CloseBtn");
		m_pSelf->m_FightText.Init(&Ini, "FightText");
		m_pSelf->m_LiveText.Init(&Ini, "LiveText");
		m_pSelf->m_FightText.SetText("Vâ c«ng");
		m_pSelf->m_LiveText.SetText("K.n sèng");
		m_pSelf->m_LiveSkillPad.LoadScheme(pScheme);
		m_pSelf->m_FightSkillPad.LoadScheme(pScheme);
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂ¼¼ÄÜ
//--------------------------------------------------------------------------
void KUiSkills::UpdateSkill(KUiSkillData* pSkill, int nIndex)
{
	if (pSkill)
	{
		if (m_pSelf)
		{
			if (pSkill->uGenre == CGOG_SKILL_LIVE)
				m_pSelf->m_LiveSkillPad.UpdateSkill(pSkill, nIndex);
			else if (pSkill->uGenre == CGOG_SKILL_FIGHT)
				m_pSelf->m_FightSkillPad.UpdateSkill(pSkill, nIndex);
		}
		if (g_pCoreShell)
		{
			KUiPlayerAttribute	Info;
			memset(&Info, 0, sizeof(KUiPlayerAttribute));
			g_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (unsigned int)&Info, 0);
			if (Info.nLevel <= SET_NEW_SKILL_TO_IMMED_SKILL_LEVEL_RANGE)
			{
				g_pCoreShell->OperationRequest(GOI_SET_IMMDIA_SKILL, (unsigned int)pSkill, 1);
				KSystemMessage	Msg;
				Msg.byConfirmType = SMCT_NONE;
				Msg.byParamSize = 0;
				Msg.byPriority = 0;
				Msg.eType = SMT_NORMAL;
				Msg.uReservedForUi = 0;

				KIniFile*	pIni = g_UiBase.GetCommConfigFile();
				if (pIni)
				{
					if (pIni->GetString("InfoString", AUTO_SET_IMMED_SKILL_MSG_ID,
						"", Msg.szMessage, sizeof(Msg.szMessage)))
					{
						KUiSysMsgCentre::AMessageArrival(&Msg, NULL);
					}
					g_UiBase.CloseCommConfigFile();
				}
			}
		}
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂÕ½¶·¼¼ÄÜÉý¼¶µãÊý
//--------------------------------------------------------------------------
void KUiSkills::UpdateFightRemainPoint(int nPoint)
{
	m_FightSkillPad.UpdateRemainPoint(nPoint);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º¸üÐÂÉú»î¼¼ÄÜ¹«¹²Êý¾Ý
//--------------------------------------------------------------------------
void KUiSkills::UpdateLiveBaseData()
{
	m_LiveSkillPad.UpdateBaseData();
}

//´°¿Úº¯Êý
int KUiSkills::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if (uMsg == WND_N_BUTTON_CLICK && (KWndWindow*)uParam == (KWndWindow*)&m_Close)
	{
		Hide();	// ¹Ø±Õ×°±¸¿ò
		return 0;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}
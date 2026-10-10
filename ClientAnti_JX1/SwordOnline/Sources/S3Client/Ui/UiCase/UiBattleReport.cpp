/*****************************************************************************************
//	Chien bao Tong Kim (phim `) - ban6 KBattleReport, ini UiBattleReport.ini (= ban6 ????????.ini).
//	Ba vung: dong dau (ten tran, cap, phuong thuc, so nguoi hai phe, con phut), thanh tich ca nhan,
//	thap dai cao thu (cot = loai co hien BT_SetView, ten cot lay [ItemName] cua ini).
//	Du lieu: KChienBaoDuLieu cua core (GetGameData(GDI_CHIEN_BAO)), doc moi lan ve nen thoi gian tu dem lui.
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiBattleReport.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/gamedatadef.h"
#include "../UiBase.h"
#include "../../../Represent/iRepresent/iRepresentShell.h"
#include "../../../Represent/iRepresent/KRepresentUnit.h"
#include <stdio.h>
#include <string.h>

extern iCoreShell*		g_pCoreShell;
extern iRepresentShell*	g_pRepresentShell;

#define	SCHEME_INI_CHIEN_BAO	"UiBattleReport.ini"
#define	CB_RONG_CHU				6		// font 12: chu nua o rong 6 diem
#define	CB_CAO_DONG				16

KUiBattleReport* KUiBattleReport::m_pSelf = NULL;

KUiBattleReport* KUiBattleReport::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

KUiBattleReport* KUiBattleReport::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiBattleReport;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

void KUiBattleReport::CloseWindow(bool bDestroy)
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

// Core bao vua nhan goi chien bao: roi tran (loai 5) thi dong nhu ban6.
void KUiBattleReport::OnCoreData(unsigned int uLoai)
{
	if (uLoai == 5)		// chienbao_roi (Headers/KProtocol.h)
		CloseWindow(false);
}

void KUiBattleReport::Initialize()
{
	memset(m_szTenLoai, 0, sizeof(m_szTenLoai));
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	Wnd_AddWindow(this);
}

static unsigned int MauIni(KIniFile & Ini, const char* szKhoa, const char* szMacDinh)
{
	char Buff[32];
	Ini.GetString("Main", szKhoa, szMacDinh, Buff, sizeof(Buff));
	return GetColor(Buff) | 0xff000000;
}

void KUiBattleReport::LoadScheme(const char* pScheme)
{
	if (m_pSelf == NULL)
		return;
	char		Buff[256];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_CHIEN_BAO);
	if (!Ini.Load(Buff))
		return;
	m_pSelf->Init(&Ini, "Main");
	m_pSelf->m_uMauTieuDeMuoi = MauIni(Ini, "TopTenTitleColor", "235,231,148");
	m_pSelf->m_uMauLe = MauIni(Ini, "OddColor", "255,255,255");
	m_pSelf->m_uMauChan = MauIni(Ini, "EvenColor", "255,255,80");
	m_pSelf->m_uMauTieuDeBan = MauIni(Ini, "SelfTitleColor", "235,231,148");
	m_pSelf->m_uMauBan = MauIni(Ini, "SelfInfoColor", "255,255,255");
	m_pSelf->m_uMauDau = MauIni(Ini, "TitleTextColor", "235,231,148");
	m_pSelf->m_uMauVach = MauIni(Ini, "LineColor", "0,255,255");
	Ini.GetInteger("Main", "Left_Right", 2, &m_pSelf->m_nLe);
	Ini.GetInteger("TopPos", "TopInfoPos", 3, &m_pSelf->m_nYDau);
	Ini.GetInteger("TopPos", "LinePos1", 17, &m_pSelf->m_nYVach1);
	Ini.GetInteger("TopPos", "SelfInfoPos", 19, &m_pSelf->m_nYBan);
	Ini.GetInteger("TopPos", "SelfGradePos", 158, &m_pSelf->m_nYChan);
	Ini.GetInteger("TopPos", "LinePos2", 172, &m_pSelf->m_nYVach2);
	Ini.GetInteger("TopPos", "TopTenPos", 174, &m_pSelf->m_nYMuoi);
	const char* aKhoaDau[5] = { "BattleWar", "Level", "WinCondition", "Proportion", "LeftTime" };
	const int aDau[5] = { 28, 8, 24, 16, 12 };
	for (int i = 0; i < 5; ++i)
		Ini.GetInteger("TopTypeSet", aKhoaDau[i], aDau[i], &m_pSelf->m_nCotDau[i]);
	const char* aKhoaBan[2] = { "ReportItem", "ObjNum" };
	const int aBan[2] = { 18, 36 };
	for (int i = 0; i < 2; ++i)
		Ini.GetInteger("SelfTypeSet", aKhoaBan[i], aBan[i], &m_pSelf->m_nCotBan[i]);
	Ini.GetInteger("TenTypeSet", "PlayerName", 10, &m_pSelf->m_nCotTen);
	for (int id = 0; id < CHIEN_BAO_LOAI; ++id)
	{
		char szId[8];
		sprintf(szId, "%d", id);
		Ini.GetString("ItemName", szId, "", m_pSelf->m_szTenLoai[id], sizeof(m_pSelf->m_szTenLoai[id]));
	}
}

void KUiBattleReport::Chu(const char* sz, int nCot, int y, unsigned int uMau)
{
	g_pRepresentShell->OutputText(12, sz, -1, m_nAbsoluteLeft + m_nLe + nCot * CB_RONG_CHU,
		m_nAbsoluteTop + y, uMau, 0, TEXT_IN_SINGLE_PLANE_COORD, 0xff000000);
}

// Chu trong mot cot rong nRong chu nua o: cat bot de khong de len cot ke (TCVN3 mot byte mot chu).
void KUiBattleReport::ChuCot(const char* sz, int nCot, int nRong, int y, unsigned int uMau)
{
	char szCat[256];
	int n = (int)strlen(sz);
	if (n > nRong - 1)
		n = nRong - 1;
	if (n < 0)
		n = 0;
	if (n > (int)sizeof(szCat) - 1)
		n = sizeof(szCat) - 1;
	memcpy(szCat, sz, n);
	szCat[n] = 0;
	Chu(szCat, nCot, y, uMau);
}

void KUiBattleReport::Vach(int y)
{
	KRULine Line;
	Line.Color.Color_dw = m_uMauVach;
	Line.oPosition.nX = m_nAbsoluteLeft + m_nLe;
	Line.oEndPos.nX = m_nAbsoluteLeft + m_Width - m_nLe;
	Line.oPosition.nY = Line.oEndPos.nY = m_nAbsoluteTop + y;
	Line.oPosition.nZ = Line.oEndPos.nZ = 0;
	g_pRepresentShell->DrawPrimitives(1, &Line, RU_T_LINE, true);
}

void KUiBattleReport::PaintWindow()
{
	KWndShadow::PaintWindow();
	if (!g_pRepresentShell || !g_pCoreShell)
		return;
	const KChienBaoDuLieu* c = (const KChienBaoDuLieu*)g_pCoreShell->GetGameData(GDI_CHIEN_BAO, 0, 0);
	if (!c)
		return;
	char sz[256];

	// Dong dau: ten tran | cap | phuong thuc | Tong N : Kim M | con X phut.
	int x = 0;
	ChuCot(c->szTenTran, x, m_nCotDau[0], m_nYDau, m_uMauDau);
	x += m_nCotDau[0];
	const char* aCap[4] = { "", "S\254 c\312p", "Trung c\312p", "Cao c\312p" };
	int nCap = c->nTran[9];
	ChuCot((nCap >= 1 && nCap <= 3) ? aCap[nCap] : "", x, m_nCotDau[1], m_nYDau, m_uMauDau);
	x += m_nCotDau[1];
	ChuCot(c->szPhuongThuc, x, m_nCotDau[2], m_nYDau, m_uMauDau);
	x += m_nCotDau[2];
	sprintf(sz, "T\350ng %d : Kim %d", c->nPhe1, c->nPhe2);
	ChuCot(sz, x, m_nCotDau[3], m_nYDau, m_uMauDau);
	x += m_nCotDau[3];
	int nCon = c->nTran[8] - (int)((GetTickCount() - c->uNhanGio) / 1000);
	sprintf(sz, "C\337n %d ph\363t", (c->uNhanGio && nCon > 0) ? nCon / 60 + 1 : 0);
	Chu(sz, x, m_nYDau, m_uMauDau);
	Vach(m_nYVach1);

	// Thanh tich ca nhan (ban6: Tong PK, NPC, lien tram hien tai, thang don dau, bao vat, doat co).
	const int* v = c->BanThan.v;
	Chu("Th\265nh t\335ch c\270 nh\251n", 0, m_nYBan, m_uMauTieuDeBan);
	int xGiaTri = m_nCotBan[0] + 2;		// ban6 noi cot bang '|': chua khoang cach sau nhan dai nhat (18 chu)
	Chu("T\346ng", xGiaTri, m_nYBan, m_uMauTieuDeBan);
	const char* aMuc[6] = { "T\346ng PK", "NPC", "Li\252n tr\266m hi\326n t\271i", "Th\276ng \256\254n \256\312u", "B\270u v\313t", "\247o\271t c\352" };
	const int aId[6] = { 2, 3, 14, 16, 17, 5 };
	for (int i = 0; i < 6; ++i)
	{
		int y = m_nYBan + (i + 1) * CB_CAO_DONG + 4;
		Chu(aMuc[i], 0, y, m_uMauBan);
		sprintf(sz, "%d", v[aId[i]]);
		Chu(sz, xGiaTri, y, m_uMauBan);
	}
	sprintf(sz, "\247i\323m t\335ch l\362y: %d    T\366 vong: %d    Li\252n tr\266m cao nh\312t: %d    (\312n ph\335m ~ \256\323 b\313t/t\276t)", v[1], v[4], v[13]);
	Chu(sz, 0, m_nYChan, m_uMauTieuDeBan);
	Vach(m_nYVach2);

	// Thap dai cao thu: ten + moi loai dang hien (tang dan theo id), cot rong theo ten cot.
	int aCot[CHIEN_BAO_LOAI], nCot = 0;
	for (int id = 0; id < CHIEN_BAO_LOAI; ++id)
		if (c->Xem[id])
			aCot[nCot++] = id;
	int nYTieuDe = m_nYMuoi + 2;
	// Cot ten rong theo ten dai nhat ("10. " + ten), it nhat bang tieu de va PlayerName cua ini.
	int nRongTen = m_nCotTen;
	for (int i = 0; i < c->nSoDong && i < CHIEN_BAO_DONG; ++i)
	{
		int n = (int)strlen(c->Bang[i].szTen) + 6;
		if (n > nRongTen)
			nRongTen = n;
	}
	const char* szTieuDeMuoi = "Th\313p \256\271i cao th\361";
	if ((int)strlen(szTieuDeMuoi) + 2 > nRongTen)
		nRongTen = (int)strlen(szTieuDeMuoi) + 2;
	Chu(szTieuDeMuoi, 0, nYTieuDe, m_uMauTieuDeMuoi);
	x = nRongTen;
	int aX[CHIEN_BAO_LOAI];
	for (int k = 0; k < nCot; ++k)
	{
		const char* szTen = m_szTenLoai[aCot[k]];
		char szMacDinh[16];
		if (!szTen[0])
		{
			sprintf(szMacDinh, "Data%d", aCot[k]);
			szTen = szMacDinh;
		}
		aX[k] = x;
		Chu(szTen, x, nYTieuDe, m_uMauTieuDeMuoi);
		x += (int)strlen(szTen) + 2;
	}
	for (int i = 0; i < c->nSoDong && i < CHIEN_BAO_DONG; ++i)
	{
		int y = nYTieuDe + (i + 1) * CB_CAO_DONG;
		unsigned int uMau = (i % 2) ? m_uMauChan : m_uMauLe;
		sprintf(sz, "%d. %s", i + 1, c->Bang[i].szTen);
		Chu(sz, 0, y, uMau);
		for (int k = 0; k < nCot; ++k)
		{
			sprintf(sz, "%d", c->Bang[i].v[aCot[k]]);
			Chu(sz, aX[k], y, uMau);
		}
	}
}

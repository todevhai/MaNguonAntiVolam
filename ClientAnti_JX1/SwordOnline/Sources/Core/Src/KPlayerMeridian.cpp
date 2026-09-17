// Kinh mach phia client: cong thuoc tinh huyet vao nhan vat nhu may chu
// (server/linux-server/Core/KPlayerMeridian.cpp), de bang F3 va moi phep tinh dua tren
// suc manh / than phap / sinh khi / noi cong cua client khop may chu.
//
// UpdataCurData dung lai thuoc tinh tu goc moi khi thay do, len cap, nhan goi thuoc tinh;
// khong cong huyet o day thi phan huyet mat ngay lan dung lai dau tien. Cap 8 mach lay tu
// goi s2c_meridian (KPlayer::m_Meridian). Bang giong may chu:
//   settings/meridian/meridian_level.txt  cot 2 mach, cot 3 cap, cot 7..9 ma thuoc tinh
//   settings/item/000/magicattrib_ge.txt  ma thuoc tinh = so dong tinh tu 0 (dong 0 la tieu de)
// Chi ap loai 97..100 (magic_strength_v..magic_energy_v) nhu may chu.

#include "KCore.h"
#ifndef _SERVER
#include "KEngine.h"
#include "KNpc.h"
#include "KPlayer.h"
#include "KTabFile.h"
#include "KMagicAttrib.h"
#include "KMagicDesc.h"
#include "CoreShell.h"

#define MERIDIAN_MAX_ATTRIB	3

struct KMeridianAcupointAttrib
{
	char			szName[32];		// ten huyet (cot 1, TCVN3)
	int				nCount;
	KMagicAttrib	aAttrib[MERIDIAN_MAX_ATTRIB];
};

static KMeridianAcupointAttrib	s_Attrib[MERIDIAN_COUNT + 1][MERIDIAN_MAX_LEVEL + 1];
static int						s_nLoaded = 0;	// 0 chua nap, 1 nap xong, -1 hong (khong thu lai)

static BOOL LoadMeridianAttribs()
{
	if (s_nLoaded)
		return s_nLoaded > 0;
	s_nLoaded = -1;
	KTabFile Level, Attrib;
	if (!Level.Load((LPSTR)"\\settings\\meridian\\meridian_level.txt") ||
		!Attrib.Load((LPSTR)"\\settings\\item\\000\\magicattrib_ge.txt"))
		return FALSE;
	memset(s_Attrib, 0, sizeof(s_Attrib));
	for (int r = 2; r <= Level.GetHeight(); r++)
	{
		int nMeridian = 0, nLevel = 0;
		Level.GetInteger(r, 2, 0, &nMeridian);
		Level.GetInteger(r, 3, 0, &nLevel);
		if (nMeridian < 1 || nMeridian > MERIDIAN_COUNT || nLevel < 1 || nLevel > MERIDIAN_MAX_LEVEL)
			continue;
		KMeridianAcupointAttrib& a = s_Attrib[nMeridian][nLevel];
		Level.GetString(r, 1, (LPSTR)"", a.szName, sizeof(a.szName));
		for (int i = 0; i < MERIDIAN_MAX_ATTRIB; i++)
		{
			int nId = 0;
			Level.GetInteger(r, 7 + i, 0, &nId);
			if (nId <= 0)
				continue;
			KMagicAttrib& m = a.aAttrib[a.nCount];
			Attrib.GetInteger(nId + 1, 5, 0, &m.nAttribType);
			Attrib.GetInteger(nId + 1, 6, 0, &m.nValue[0]);
			if (m.nAttribType > 0)
				a.nCount++;
		}
	}
	s_nLoaded = 1;
	return TRUE;
}

void KPlayer::ReCalcMeridian()
{
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC || !LoadMeridianAttribs())
		return;
	for (int m = 1; m <= MERIDIAN_COUNT; m++)
	{
		int nLevel = m_Meridian.btLevel[m - 1];
		if (nLevel > MERIDIAN_MAX_LEVEL)
			nLevel = MERIDIAN_MAX_LEVEL;
		for (int l = 1; l <= nLevel; l++)
		{
			const KMeridianAcupointAttrib& a = s_Attrib[m][l];
			for (int i = 0; i < a.nCount; i++)
			{
				int nType = a.aAttrib[i].nAttribType;
				if (nType >= magic_strength_v && nType <= magic_energy_v)
					Npc[m_nIndex].ModifyAttrib(m_nIndex, (void*)&a.aAttrib[i]);
			}
		}
	}
}
// Ten huyet va mo ta thuoc tinh cho trang Kinh mach (GDI_MERIDIAN_ACUP_DESC). Mo ta dung
// g_MagicDesc nhu dong thuoc tinh vat pham; loai 8.x khong co mo ta thi bo qua dong do.
BOOL MeridianAcupDesc(KUiMeridianAcupDesc* pDesc)
{
	if (!pDesc || !LoadMeridianAttribs() || pDesc->nMeridian < 1 || pDesc->nMeridian > MERIDIAN_COUNT ||
		pDesc->nLevel < 1 || pDesc->nLevel > MERIDIAN_MAX_LEVEL)
		return FALSE;
	const KMeridianAcupointAttrib& a = s_Attrib[pDesc->nMeridian][pDesc->nLevel];
	strncpy(pDesc->szName, a.szName, sizeof(pDesc->szName) - 1);
	pDesc->szName[sizeof(pDesc->szName) - 1] = 0;
	pDesc->szDesc[0] = 0;
	for (int i = 0; i < a.nCount; i++)
	{
		const char* pszLine = g_MagicDesc.GetDesc((void*)&a.aAttrib[i]);
		if (!pszLine || !pszLine[0])
			continue;
		int nUsed = (int)strlen(pDesc->szDesc);
		if (nUsed + (int)strlen(pszLine) + 2 >= (int)sizeof(pDesc->szDesc))
			break;
		if (nUsed)
			strcat(pDesc->szDesc, " ");
		strcat(pDesc->szDesc, pszLine);
	}
	return a.szName[0] != 0;
}
#endif

// Hoang Kim suits and extended suits.
//
// Client copy of VLTK-Origin server/linux-server/Core/KItemGoldSuit.cpp (same tables and rules).
// The client only keeps the suit state: every equip/unequip ends in KPlayer::UpdataCurData, whose
// ReCalcEquip applies GetEquipEnhance (3 while a suit is active) and BatLaiBoMoRong.
//
// Rules read from the ban6/voz2 server binaries (symbols kept; same as jx9tn):
//   - suit table: every goldequip row, then every platinaequip row, goes to its suit (col 53 / 59)
//     under an index: the equipment slot of its DetailType, or for a ring 14 + level + 10 * series.
//     A suit's part count is the number of distinct indexes.
//   - active suit: worn gold items grouped by suit; count = items (a ring only when its index is
//     still empty). Suits are tried by increasing id; need = suite_activate_count[S] if > 0, else
//     the part count; skipped when count < need; matched = indexes whose first worn row is listed
//     under that index in the suit (row number only). matched >= need -> S. Only one suit.
//   - with a suit active every worn item (any genre) uses 3 hidden attributes; slots 10..14 always do.
//     The suit turning on opens the missing hidden attributes of every worn item; turning off
//     closes them back to the wuxing chain.
//   - extended suit: count per col 54 value over worn gold items. Level = highest k + 1 (k = 0, 1)
//     such that some count >= ActivateCount_{k+1} (no row or <= 0 -> 10). Level k opens hidden
//     attribute k (cols 56/57) of every worn gold item.

#include "KCore.h"
#include <algorithm>
#include "MyAssert.H"
#include "KTabFile.h"
#include "KItem.h"
#include "KItemSet.h"
#include "KItemGenerator.h"
#include "KNpc.h"
#include "KPlayer.h"
#include "KItemList.h"

#define TABFILE_GOLD_DIR		"\\settings\\item\\004"

//=============================================================================
// Suit table

static void ThemDongVaoBo(std::map<int, KGOLD_SUITE>& mapBo, int nDong, int nDetail, int nSeries, int nLevel, int nBo)
{
	int nO = KItemList::GetEquipPlace(nDetail);
	if (nO == itempart_ring1 || nO == itempart_ring2)
		nO = 14 + nLevel + 10 * nSeries;
	if (nBo <= 0 || nO < 0 || nO >= MAX_O_BO_TRANG_BI)
		return;
	std::map<int, KGOLD_SUITE>::iterator it = mapBo.find(nBo);
	if (it == mapBo.end())
	{
		it = mapBo.insert(std::make_pair(nBo, KGOLD_SUITE())).first;
		it->second.nSoBoPhan = 0;
	}
	it->second.aryDong[nO].push_back(nDong);
	if (it->second.aryDong[nO].size() == 1)
		it->second.nSoBoPhan++;
}

static BOOL NapBang(KTabFile& Tab, const char* szTen)
{
	char szDuongDan[FILE_NAME_LENGTH];
	g_SetRootPath(NULL);
	g_UnitePathAndName((char*)TABFILE_GOLD_DIR, (char*)szTen, szDuongDan);
	return Tab.Load(szDuongDan) && Tab.GetHeight() > 1;
}

void KLibOfBPT::InitGoldSuite()
{
	m_GoldSuit.clear();
	m_SuitActivate.clear();
	m_ExtSuitActivate[0].clear();
	m_ExtSuitActivate[1].clear();

	for (int i = 0; i < m_GoldItem.GetRecordCount(); i++)
	{
		const KBASICPROP_EQUIPMENT_GOLD* p = m_GoldItem.GetRecord(i);
		ThemDongVaoBo(m_GoldSuit, i, p->m_nDetailType, p->m_nSeries, p->m_nLevel, p->m_nSuit);
	}

	// Platina (quality 4) items are not made yet, but their rows are part of the same suits.
	KTabFile Tab;
	int nPlatina = 0;
	if (NapBang(Tab, "platinaequip.txt"))
	{
		for (int r = 2; r <= Tab.GetHeight(); r++, nPlatina++)
		{
			int nDetail, nSeries, nLevel, nBo;
			Tab.GetInteger(r, 3, -1, &nDetail);
			Tab.GetInteger(r, 10, 0, &nSeries);
			Tab.GetInteger(r, 12, 1, &nLevel);
			Tab.GetInteger(r, 59, -1, &nBo);
			if (nSeries < 0) nSeries = 0;
			if (nLevel <= 0) nLevel = 1;
			ThemDongVaoBo(m_GoldSuit, r - 2, nDetail, nSeries, nLevel, nBo);
		}
	}

	KTabFile TabBo;
	if (NapBang(TabBo, "suite_activate_count.txt"))
		for (int r = 2; r <= TabBo.GetHeight(); r++)
		{
			int nBo, nCan;
			TabBo.GetInteger(r, 1, 0, &nBo);
			TabBo.GetInteger(r, 2, 0, &nCan);
			m_SuitActivate[nBo] = nCan;
		}

	KTabFile TabMoRong;
	if (NapBang(TabMoRong, "ext_suite_activate_count.txt"))
		for (int r = 2; r <= TabMoRong.GetHeight(); r++)
		{
			int nBo, nCan1, nCan2;
			TabMoRong.GetInteger(r, 1, 0, &nBo);
			TabMoRong.GetInteger(r, 2, 0, &nCan1);
			TabMoRong.GetInteger(r, 3, 0, &nCan2);
			m_ExtSuitActivate[0][nBo] = nCan1;
			m_ExtSuitActivate[1][nBo] = nCan2;
		}

	g_DebugLog("[hoang kim] bo: %d bo (them %d dong platina), %d nguong bo, %d nguong bo mo rong",
		(int)m_GoldSuit.size(), nPlatina, (int)m_SuitActivate.size(), (int)m_ExtSuitActivate[0].size());
}

// One suit of the worn items (C++98: must not be a local type to sit in std::map).
struct KBoDangMac
{
	int		nDem;
	int		aryDong[MAX_O_BO_TRANG_BI];	// first worn row per index, -1 = none
};

int KLibOfBPT::FindGoldSuite(const KMON_TRONG_BO* pMon, int nSoMon) const
{
	std::map<int, KBoDangMac> mapMac;
	for (int i = 0; i < nSoMon; i++)
	{
		const KBASICPROP_EQUIPMENT_GOLD* pRec = m_GoldItem.GetRecord(pMon[i].nDong);
		int nO = pMon[i].nO;
		if (NULL == pRec || pRec->m_nSuit <= 0 || nO < 0 || nO >= MAX_O_BO_TRANG_BI)
			continue;
		std::map<int, KBoDangMac>::iterator it = mapMac.find(pRec->m_nSuit);
		if (it == mapMac.end())
		{
			KBoDangMac sMoi;
			sMoi.nDem = 0;
			for (int k = 0; k < MAX_O_BO_TRANG_BI; k++)
				sMoi.aryDong[k] = -1;
			it = mapMac.insert(std::make_pair(pRec->m_nSuit, sMoi)).first;
		}
		if (!pMon[i].bNhan || it->second.aryDong[nO] < 0)
			it->second.nDem++;
		if (it->second.aryDong[nO] < 0)
			it->second.aryDong[nO] = pMon[i].nDong;
	}

	for (std::map<int, KBoDangMac>::const_iterator it = mapMac.begin(); it != mapMac.end(); ++it)
	{
		std::map<int, KGOLD_SUITE>::const_iterator itBo = m_GoldSuit.find(it->first);
		if (itBo == m_GoldSuit.end())
			continue;
		std::map<int, int>::const_iterator itCan = m_SuitActivate.find(it->first);
		int nCan = (itCan != m_SuitActivate.end() && itCan->second > 0) ? itCan->second : itBo->second.nSoBoPhan;
		if (it->second.nDem < nCan)
			continue;
		int nKhop = 0;
		for (int k = 0; k < MAX_O_BO_TRANG_BI; k++)
		{
			const std::vector<int>& v = itBo->second.aryDong[k];
			if (it->second.aryDong[k] >= 0 && std::find(v.begin(), v.end(), it->second.aryDong[k]) != v.end())
				nKhop++;
		}
		if (nKhop >= nCan)
			return it->first;
	}
	return -1;
}

int KLibOfBPT::GetExtSuitActivate(int nExtSuit, int nCap) const
{
	std::map<int, int>::const_iterator it = m_ExtSuitActivate[nCap].find(nExtSuit);
	return (it != m_ExtSuitActivate[nCap].end() && it->second > 0) ? it->second : 10;
}

//=============================================================================
// Item

void KItem::ApplyExtSuitAttribToNPC(IN KNpc* pNPC, IN int nCap, IN BOOL bBat) const
{
	if (nCap < 0 || nCap > 1 || m_aryExtSuitAttrib[nCap].nAttribType <= 0)
		return;
	KItemNormalAttrib sDoi = m_aryExtSuitAttrib[nCap];
	if (!bBat)
		for (int k = 0; k < 3; k++)
			sDoi.nValue[k] = -sDoi.nValue[k];
	pNPC->ModifyAttrib(pNPC->m_Index, (void*)&sDoi);
}

//=============================================================================
// Item list

int KItemList::TimBoKichHoat()
{
	KMON_TRONG_BO aryMon[itempart_num];
	int nSoMon = 0;
	for (int i = 0; i < itempart_num; i++)
	{
		int nIdx = m_EquipItem[i];
		if (nIdx <= 0 || Item[nIdx].GetGenre() != item_equip || Item[nIdx].GetGoldId() <= 0)
			continue;
		KMON_TRONG_BO* p = &aryMon[nSoMon++];
		p->nDong = Item[nIdx].GetGoldId() - 1;
		p->bNhan = Item[nIdx].GetDetailType() == equip_ring && (i == itempart_ring1 || i == itempart_ring2);
		p->nO = p->bNhan ? 14 + Item[nIdx].GetLevel() + 10 * Item[nIdx].GetSeries() : i;
	}
	return nSoMon > 0 ? ItemGen.FindGoldSuite(aryMon, nSoMon) : -1;
}

int KItemList::CapBoMoRong()
{
	int nCap = 0;
	for (std::map<int, int>::const_iterator it = m_DemBoMoRong.begin(); it != m_DemBoMoRong.end(); ++it)
		for (int k = 0; k < 2; k++)
		{
			if (it->second >= ItemGen.GetExtSuitActivate(it->first, k) && nCap < k + 1)
				nCap = k + 1;
			if (nCap > 1)
				return 2;
		}
	return nCap;
}

// Moves one item's extended hidden attributes from level nTu to nDen (0..2).
void KItemList::DoiCapMoRong(int nIdx, int nTu, int nDen)
{
	KNpc* pNpc = &Npc[Player[m_PlayerIdx].m_nIndex];
	if (nTu < nDen)
		for (int c = nTu; c <= 1 && c != nDen; c++)
			Item[nIdx].ApplyExtSuitAttribToNPC(pNpc, c, TRUE);
	else
		for (int c = nTu - 1; c >= 0 && c != nDen - 1; c--)
			Item[nIdx].ApplyExtSuitAttribToNPC(pNpc, c, FALSE);
}

// Rebuild of all attributes (KPlayer::ReCalcEquip): extended hidden attributes are not part of
// KItem::ApplyMagicAttribToNPC.
void KItemList::BatLaiBoMoRong()
{
	int nCap = CapBoMoRong();
	for (int i = 0; i < itempart_num; i++)
		if (m_EquipItem[i] && Item[m_EquipItem[i]].GetGoldId() > 0 && m_DemBoMoRong[Item[m_EquipItem[i]].GetExtSuit()] > 0)
			DoiCapMoRong(m_EquipItem[i], 0, nCap);
}

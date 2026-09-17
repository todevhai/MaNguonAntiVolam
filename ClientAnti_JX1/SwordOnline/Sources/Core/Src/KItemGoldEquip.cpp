// Hoang Kim equipment (quality 1), client side (settings/item/004/goldequip.txt).
//
// Same tables and rules as the server (VLTK-Origin server/linux-server/Core/KItemGoldEquip.cpp):
//   - goldequip.txt read by column POSITION (62 columns), magicattrib_ge.txt column 5 = type,
//     6..11 = min/max of the 3 parameters. Both from settings/item/004.
//   - goldId = goldequip row + 1 (0 = not a gold item); the server sends it in ITEM_SYNC::m_GoldId.
//   - numbers are rolled from the item's random seed, in the server's order: 7 base attributes,
//     then 3 parameters for each magic slot whose row exists and has a type > 0. Any change here
//     must be made on the server too, or the tooltip shows numbers the server does not use.
// Suits (column 53) are not applied yet on either side: nSet/nSetNum stay 0.

#include "KCore.h"
#include "KTabFile.h"
#include "MyAssert.H"
#include "KBasPropTbl.h"
#include "KItem.h"
#include "KItemGenerator.h"
#include "../../Represent/iRepresent/iRepresentshell.h"	// ISI_T_SPR, as in KItem.cpp

#define TABFILE_GOLD_DIR		"\\settings\\item\\004"
#define TABFILE_GOLDEQUIP		"goldequip.txt"
#define TABFILE_GOLDMAGIC		"magicattrib_ge.txt"

BOOL LoadRecord(IN KTabFile* pTF, IN int nRow, IN OUT const PROPINFO* pPI, IN int cbFields);	// KBasPropTbl.CPP

static void CotSo(PROPINFO* pPI, int& n, int* pnData)
{
	pPI[n].m_nType = PI_VARTYPE_INT;
	pPI[n].m_pData.m_pnData = pnData;
	pPI[n].m_nBufSize = 0;
	n++;
}

static void CotChu(PROPINFO* pPI, int& n, char* pszBuf, int nBufSize)
{
	pPI[n].m_nType = PI_VARTYPE_CHAR;
	pPI[n].m_pData.m_pszBuf = pszBuf;
	pPI[n].m_nBufSize = nBufSize;
	n++;
}

//=============================================================================
// goldequip.txt

KBPT_Equipment_Gold::KBPT_Equipment_Gold()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_EQUIPMENT_GOLD);
	::strcpy(m_szTabFile, TABFILE_GOLDEQUIP);
	m_pszThuMuc = TABFILE_GOLD_DIR;
}

KBPT_Equipment_Gold::~KBPT_Equipment_Gold()
{
}

BOOL KBPT_Equipment_Gold::LoadRecord(int i, KTabFile* pTF)
{
	_ASSERT(pTF != NULL);
	_ASSERT(i >= 0 && i < m_nNumOfEntries);

	KBASICPROP_EQUIPMENT_GOLD* pBuf = ((KBASICPROP_EQUIPMENT_GOLD*)m_pBuf) + i;
	int			nBo;
	PROPINFO	aryPI[57];
	int			n = 0, k;

	CotChu(aryPI, n, pBuf->m_szName, sizeof(pBuf->m_szName));			// 1
	CotSo(aryPI, n, &pBuf->m_nItemGenre);								// 2
	CotSo(aryPI, n, &pBuf->m_nDetailType);								// 3
	CotSo(aryPI, n, &pBuf->m_nParticularType);							// 4
	CotChu(aryPI, n, pBuf->m_szImageName, sizeof(pBuf->m_szImageName));	// 5
	CotSo(aryPI, n, &pBuf->m_nObjIdx);									// 6
	CotSo(aryPI, n, &pBuf->m_nWidth);									// 7
	CotSo(aryPI, n, &pBuf->m_nHeight);									// 8
	CotChu(aryPI, n, pBuf->m_szIntro, sizeof(pBuf->m_szIntro));			// 9
	CotSo(aryPI, n, &pBuf->m_nSeries);									// 10
	CotSo(aryPI, n, &pBuf->m_nPrice);									// 11
	CotSo(aryPI, n, &pBuf->m_nLevel);									// 12
	CotSo(aryPI, n, &nBo);												// 13 stack flag
	for (k = 0; k < 7; k++)												// 14..34
	{
		CotSo(aryPI, n, &pBuf->m_aryPropBasic[k].nType);
		CotSo(aryPI, n, &pBuf->m_aryPropBasic[k].sRange.nMin);
		CotSo(aryPI, n, &pBuf->m_aryPropBasic[k].sRange.nMax);
	}
	for (k = 0; k < 6; k++)												// 35..46
	{
		CotSo(aryPI, n, &pBuf->m_aryPropReq[k].nType);
		CotSo(aryPI, n, &pBuf->m_aryPropReq[k].nPara);
	}
	for (k = 0; k < 6; k++)												// 47..52
		CotSo(aryPI, n, &pBuf->m_aryMagicIdx[k]);
	CotSo(aryPI, n, &pBuf->m_nSuit);									// 53
	CotSo(aryPI, n, &pBuf->m_nExtSuit);									// 54
	CotSo(aryPI, n, &pBuf->m_nExtSuitNo);								// 55
	CotSo(aryPI, n, &pBuf->m_aryExtSuitHidden[0]);						// 56
	CotSo(aryPI, n, &pBuf->m_aryExtSuitHidden[1]);						// 57

	if (!::LoadRecord(pTF, i, aryPI, n))
		return FALSE;

	if (pBuf->m_nSeries < 0)	pBuf->m_nSeries = 0;
	if (pBuf->m_nPrice < 0)		pBuf->m_nPrice = 0;
	if (pBuf->m_nLevel <= 0)	pBuf->m_nLevel = 1;
	return TRUE;
}

const KBASICPROP_EQUIPMENT_GOLD* KBPT_Equipment_Gold::GetRecord(int i) const
{
	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_EQUIPMENT_GOLD*)m_pBuf) + i) : NULL;
}

//=============================================================================
// magicattrib_ge.txt

KBPT_GoldMagic::KBPT_GoldMagic()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_GOLDMAGIC);
	::strcpy(m_szTabFile, TABFILE_GOLDMAGIC);
	m_pszThuMuc = TABFILE_GOLD_DIR;
}

KBPT_GoldMagic::~KBPT_GoldMagic()
{
}

BOOL KBPT_GoldMagic::LoadRecord(int i, KTabFile* pTF)
{
	_ASSERT(pTF != NULL);
	_ASSERT(i >= 0 && i < m_nNumOfEntries);

	KBASICPROP_GOLDMAGIC* pBuf = ((KBASICPROP_GOLDMAGIC*)m_pBuf) + i;
	char		szBo[256];
	int			nBo;
	PROPINFO	aryPI[11];
	int			n = 0;

	CotChu(aryPI, n, szBo, sizeof(szBo));	// 1 name
	CotSo(aryPI, n, &nBo);					// 2 prefix
	CotSo(aryPI, n, &nBo);					// 3 series requirement
	CotSo(aryPI, n, &nBo);					// 4 level requirement
	CotSo(aryPI, n, &pBuf->m_nType);		// 5
	for (int k = 0; k < 3; k++)				// 6..11
	{
		CotSo(aryPI, n, &pBuf->m_aryRange[k].nMin);
		CotSo(aryPI, n, &pBuf->m_aryRange[k].nMax);
	}
	return ::LoadRecord(pTF, i, aryPI, n);
}

const KBASICPROP_GOLDMAGIC* KBPT_GoldMagic::GetRecord(int i) const
{
	return (i >= 0 && i < m_nNumOfEntries) ? (((KBASICPROP_GOLDMAGIC*)m_pBuf) + i) : NULL;
}

//=============================================================================
// Library

// Loaded apart from the main table list so a missing table only turns gold items off.
BOOL KLibOfBPT::InitGoldEquip()
{
	BOOL bOk = m_GoldItem.Load() && m_GoldMagic.Load();
	g_DebugLog("[hoang kim] nap %s: %s, %d mon, %d thuoc tinh", TABFILE_GOLD_DIR,
		bOk ? "xong" : "HONG", m_GoldItem.NumOfEntries(), m_GoldMagic.NumOfEntries());
	return bOk;
}

const KBASICPROP_GOLDMAGIC* KLibOfBPT::GetGoldMagicRecord(IN int nIndex) const
{
	return m_GoldMagic.GetRecord(nIndex);
}

const int KLibOfBPT::GetGoldMagicNumber() const
{
	return m_GoldMagic.NumOfEntries();
}

//=============================================================================
// Item

void KItem::operator = (const KBASICPROP_EQUIPMENT_GOLD& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nSet			 = 0;	// suits not applied yet (see top of file)
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->bStack			 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	::strncpy(pCA->szItemName, sData.m_szName, sizeof(pCA->szItemName) - 1);
	pCA->szItemName[sizeof(pCA->szItemName) - 1] = 0;
	pCA->szScript[0] = 0;
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
	pCA->uPrice = 0;
	::strncpy(pCA->szImageName, sData.m_szImageName, sizeof(pCA->szImageName) - 1);
	pCA->szImageName[sizeof(pCA->szImageName) - 1] = 0;
	::strncpy(pCA->szIntro, sData.m_szIntro, sizeof(pCA->szIntro) - 1);
	pCA->szIntro[sizeof(pCA->szIntro) - 1] = 0;
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
}

// Rebuilds the item from its row and pItem->m_GeneratorParam.uRandomSeed (set by the caller).
BOOL KItemGenerator::Gen_GoldEquipment(IN int nGoldId, IN OUT KItem* pItem)
{
	const KBASICPROP_EQUIPMENT_GOLD* pRec = m_BPTLib.GetGoldItemRecord(nGoldId - 1);
	if (NULL == pItem || NULL == pRec || !pRec->m_szName[0] || m_BPTLib.GetGoldMagicNumber() <= 0)
	{
		g_DebugLog("[hoang kim] khong dung duoc mon %d (bang co %d mon)", nGoldId, m_BPTLib.GetGoldItemNumber());
		return FALSE;
	}

	UINT uHatCu = g_GetRandomSeed();
	g_RandomSeed(pItem->m_GeneratorParam.uRandomSeed);
	ZeroMemory(pItem->m_GeneratorParam.nGeneratorLevel, sizeof(pItem->m_GeneratorParam.nGeneratorLevel));

	*pItem = *pRec;
	pItem->m_CommonAttrib.nGoldId = nGoldId;
	pItem->m_nCurrentDur = 0;	// SetAttrib_Base sets it from a durability attribute, else -1
	pItem->SetAttrib_Base(pRec->m_aryPropBasic);
	pItem->SetAttrib_Req(pRec->m_aryPropReq);

	int nSoThuocTinh = m_BPTLib.GetGoldMagicNumber();
	for (int i = 0; i < 6; i++)
	{
		KItemNormalAttrib* pDst = &pItem->m_aryMagicAttrib[i];
		pDst->nAttribType = 0;
		pDst->nValue[0] = pDst->nValue[1] = pDst->nValue[2] = 0;

		int nIdx = pRec->m_aryMagicIdx[i];
		const KBASICPROP_GOLDMAGIC* pMA = (nIdx > 0 && nIdx <= nSoThuocTinh) ? m_BPTLib.GetGoldMagicRecord(nIdx - 1) : NULL;
		if (NULL == pMA || pMA->m_nType <= 0)
			continue;
		pDst->nAttribType = pMA->m_nType;
		for (int k = 0; k < 3; k++)
			pDst->nValue[k] = ::GetRandomNumber(pMA->m_aryRange[k].nMin, pMA->m_aryRange[k].nMax);
		if (pDst->nAttribType == magic_indestructible_b)
			pItem->SetDurability(-1);
	}

	g_RandomSeed(uHatCu);

	// Same line as the server's VLTK_LOG_HOANG_KIM: compare the two to check both halves roll alike.
	char szDong[512];
	int nViet = sprintf(szDong, "[hoang kim] dung mon %d hat %u:", nGoldId, pItem->m_GeneratorParam.uRandomSeed);
	for (int b = 0; b < 7; b++)
		if (pItem->m_aryBaseAttrib[b].nAttribType > 0)
			nViet += sprintf(szDong + nViet, " co%d=%d", pItem->m_aryBaseAttrib[b].nAttribType, pItem->m_aryBaseAttrib[b].nValue[0]);
	for (int m = 0; m < 6; m++)
		if (pItem->m_aryMagicAttrib[m].nAttribType > 0)
			nViet += sprintf(szDong + nViet, " ma%d=%d/%d/%d", pItem->m_aryMagicAttrib[m].nAttribType, pItem->m_aryMagicAttrib[m].nValue[0],
				pItem->m_aryMagicAttrib[m].nValue[1], pItem->m_aryMagicAttrib[m].nValue[2]);
	sprintf(szDong + nViet, " ben=%d", pItem->GetDurability());
	g_DebugLog("%s", szDong);
	return TRUE;
}

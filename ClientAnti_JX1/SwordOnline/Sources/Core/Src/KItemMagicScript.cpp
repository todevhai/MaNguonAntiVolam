// Item genre 6 - "magic or script" items, client side (settings/item/004/magicscript.txt).
//
// Same table and row lookup as the server (VLTK-Origin server/linux-server/Core/KItemMagicScript.cpp):
// columns read by POSITION, row found by (DetailType, ParticularType), stackable when column 21 > 0.
// The client only needs what it shows: name, icon (column 5), description (column 9), size, price.
// Using the item is decided by the server; the client sends the request and waits for the sync
// (KItemList::EatMecidine does not take one off the stack locally for this genre).

#include "KCore.h"
#include "KTabFile.h"
#include "MyAssert.H"
#include "KBasPropTbl.h"
#include "KItem.h"
#include "KItemGenerator.h"
#include "../../Represent/iRepresent/iRepresentshell.h"	// ISI_T_SPR, as in KItem.cpp

#define TABFILE_MAGICSCRIPT_DIR		"\\settings\\item\\004"
#define TABFILE_MAGICSCRIPT			"magicscript.txt"

BOOL LoadRecord(IN KTabFile* pTF, IN int nRow, IN OUT const PROPINFO* pPI, IN int cbFields);	// KBasPropTbl.CPP

KBPT_MagicScript::KBPT_MagicScript()
{
	m_nSizeOfEntry = sizeof(KBASICPROP_MAGICSCRIPT);
	::strcpy(m_szTabFile, TABFILE_MAGICSCRIPT);
	m_pszThuMuc = TABFILE_MAGICSCRIPT_DIR;
}

KBPT_MagicScript::~KBPT_MagicScript()
{
}

BOOL KBPT_MagicScript::LoadRecord(int i, KTabFile* pTF)
{
	KBASICPROP_MAGICSCRIPT* pBuf = ((KBASICPROP_MAGICSCRIPT*)m_pBuf) + i;
	char	szBo[256];
	int		nBo;
	const PROPINFO aryPI[] =
	{
		{ PI_VARTYPE_CHAR,	pBuf->m_szName, sizeof(pBuf->m_szName) },			// 1
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nItemGenre, 0 },						// 2
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nDetailType, 0 },						// 3
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nParticularType, 0 },					// 4
		{ PI_VARTYPE_CHAR,	pBuf->m_szImageName, sizeof(pBuf->m_szImageName) },	// 5 icon
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nObjIdx, 0 },							// 6
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nWidth, 0 },							// 7
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nHeight, 0 },							// 8
		{ PI_VARTYPE_CHAR,	pBuf->m_szIntro, sizeof(pBuf->m_szIntro) },			// 9 description
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nSeries, 0 },							// 10
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nPrice, 0 },							// 11
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nLevel, 0 },							// 12
		{ PI_VARTYPE_INT,	(char*)&nBo, 0 },										// 13 ignored
		{ PI_VARTYPE_CHAR,	szBo, sizeof(szBo) },									// 14 script
		{ PI_VARTYPE_INT,	(char*)&nBo, 0 },										// 15 skill id
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nParam[0], 0 },							// 16
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nParam[1], 0 },							// 17
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nParam[2], 0 },							// 18
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nParam[3], 0 },							// 19 shortcut bar
		{ PI_VARTYPE_INT,	(char*)&nBo, 0 },										// 20 target
		{ PI_VARTYPE_INT,	(char*)&pBuf->m_nMaxStack, 0 },							// 21 max stack
	};
	if (!::LoadRecord(pTF, i, aryPI, sizeof(aryPI) / sizeof(aryPI[0])))
		return FALSE;
	if (pBuf->m_nSeries < 0)	pBuf->m_nSeries = 0;
	if (pBuf->m_nPrice < 0)		pBuf->m_nPrice = 0;
	if (pBuf->m_nLevel <= 0)	pBuf->m_nLevel = 1;
	if (pBuf->m_nMaxStack < 0)	pBuf->m_nMaxStack = 0;
	return TRUE;
}

const KBASICPROP_MAGICSCRIPT* KBPT_MagicScript::FindRecord(IN int nDetailType, IN int nParticularType) const
{
	for (int i = 0; i < m_nNumOfEntries; i++)
	{
		const KBASICPROP_MAGICSCRIPT* p = ((KBASICPROP_MAGICSCRIPT*)m_pBuf) + i;
		if (p->m_nDetailType == nDetailType && p->m_nParticularType == nParticularType)
			return p;
	}
	return NULL;
}

// Loaded apart from the main table list so a missing table only turns genre 6 off.
BOOL KLibOfBPT::InitMagicScript()
{
	BOOL bOk = m_BPTMagicScript.Load();
	g_DebugLog("[vat pham kich ban] nap %s\\%s: %s, %d dong", TABFILE_MAGICSCRIPT_DIR, TABFILE_MAGICSCRIPT,
		bOk ? "xong" : "HONG", m_BPTMagicScript.NumOfEntries());
	return bOk;
}

const KBASICPROP_MAGICSCRIPT* KLibOfBPT::FindMagicScript(IN int nDetailType, IN int nParticularType) const
{
	return m_BPTMagicScript.FindRecord(nDetailType, nParticularType);
}

void KItem::operator = (const KBASICPROP_MAGICSCRIPT& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	ZeroMemory(pCA, sizeof(KItemCommonAttrib));
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_nMaxStack > 0;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nStackNum		 = 1;
	pCA->nIsSell		 = 1;
	pCA->nIsTrade		 = 1;
	pCA->nIsDrop		 = 1;
	::strcpy(pCA->szItemName, sData.m_szName);
#ifndef _SERVER
	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro, sData.m_szIntro);
#endif
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryRequireAttrib));
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryMagicAttrib));
#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

// pnParam: the six item params the server sends in ITEM_SYNC::m_MagicLevel (scripts keep uses
// left, expiry... there). NULL keeps them zero.
BOOL KItemGenerator::Gen_MagicScript(IN int nDetailType, IN int nParticularType, IN int nLevel,
									 IN const int* pnParam, IN OUT KItem* pItem, IN int nStackNum)
{
	ZeroMemory(&pItem->m_GeneratorParam, sizeof(pItem->m_GeneratorParam));
	const KBASICPROP_MAGICSCRIPT* pRec = m_BPTLib.FindMagicScript(nDetailType, nParticularType);
	if (NULL == pRec)
		return FALSE;
	*pItem = *pRec;
	if (nLevel > 0)
		pItem->SetLevel(nLevel);
	if (nStackNum > 0)
		pItem->SetStackNum(nStackNum);
	if (pnParam)
		for (int i = 0; i < 6; i++)
			pItem->m_GeneratorParam.nGeneratorLevel[i] = pnParam[i];
	return TRUE;
}

BOOL KItemGenerator::CoTheDatPhimTat(IN int nDetailType, IN int nParticularType) const
{
	const KBASICPROP_MAGICSCRIPT* pRec = m_BPTLib.FindMagicScript(nDetailType, nParticularType);
	return pRec && pRec->m_nParam[3] == 1;
}

#include "KCore.h"
#include "MyAssert.H"
#include "KTabFile.h"
#include "KNpc.h"
#include "KItem.h"
#include "KItemSet.h"
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif

#ifndef _SERVER
#include "ImgRef.h"
#include "KPlayer.h"
#include "../../Represent/iRepresent/iRepresentshell.h"
#include "KMagicDesc.h"
#include <time.h>
#endif

KItem	Item[MAX_ITEM];
int GetRandomNumber(int nMin, int nMax);

KItem::KItem()
{
	::memset(&m_CommonAttrib,    0, sizeof(m_CommonAttrib));
	::memset(m_aryBaseAttrib,    0, sizeof(m_aryBaseAttrib));
	::memset(m_aryRequireAttrib, 0, sizeof(m_aryRequireAttrib));
	::memset(m_aryMagicAttrib,   0, sizeof(m_aryMagicAttrib));
	::memset(&m_GeneratorParam,	 0, sizeof(m_GeneratorParam));
	m_nCurrentDur = -1;
#ifndef _SERVER
	::memset(&m_Image,   0, sizeof(KRUImage));
#endif
	m_nIndex = 0;
}

KItem::~KItem()
{
}

void* KItem::GetRequirement(IN int nReq)
{
	int i = sizeof(m_aryRequireAttrib)/sizeof(m_aryRequireAttrib[0]);
	if (nReq >= i)
		return NULL;

	return &m_aryRequireAttrib[nReq];
}
/******************************************************************************
¹¦ÄÜ:	½«itemÉÏµÄÄ§·¨Ó¦ÓÃµ½NPCÉíÉÏ
Èë¿Ú£º	pNPC: Ö¸ÏòNPCµÄÖ¸Õë£¬nMagicAcive£º´ò¿ªµÄÒþ²ØÊôÐÔÊýÄ¿
³ö¿Ú:	Ä§·¨±»Ó¦ÓÃ¡£
		¾ßÌå¹¤×÷ÓÉKNpcµÄ³ÉÔ±º¯ÊýÍê³É¡£
		KItem ¶ÔÏó±¾ÉíÃ»ÓÐ³ÉÔ±±äÁ¿±»ÐÞ¸Ä
******************************************************************************/
void KItem::ApplyMagicAttribToNPC(IN KNpc* pNPC, IN int nMagicActive /* = 0 */) const
{
	_ASSERT(this != NULL);
	_ASSERT(nMagicActive >= 0);

	int nCount = nMagicActive;
	int i;

	// »ù´¡ÊôÐÔµ÷ÕûNPC
	for (i = 0; i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryBaseAttrib[i]);
		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
		}
	}
	// Ä§·¨ÊôÐÔµ÷ÕûNPC
	for (i = 0; i < sizeof(m_aryMagicAttrib)/sizeof(m_aryMagicAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			if (i & 1)						// ÎªÆæÊý£¬ÊÇºó×º£¨i´ÓÁã¿ªÊ¼£©
			{
				if (nCount > 0)
				{
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
					nCount--;
				}
			}
			else
			{
				pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
			}
		}
	}
}

/******************************************************************************
¹¦ÄÜ:	½«itemÉÏµÄÄ§·¨´ÓNPCÉíÉÏÒÆ³ý
Èë¿Ú£º	pNPC: Ö¸ÏòNPCµÄÖ¸Õë£¬nMagicAcive£º´ò¿ªµÄÒþ²ØÊôÐÔÊýÄ¿
³ö¿Ú:	Ä§·¨±»Ó¦ÓÃ¡£
		¾ßÌå¹¤×÷ÓÉKNpcµÄ³ÉÔ±º¯ÊýÍê³É¡£
		KItem ¶ÔÏó±¾ÉíÃ»ÓÐ³ÉÔ±±äÁ¿±»ÐÞ¸Ä
******************************************************************************/
void KItem::RemoveMagicAttribFromNPC(IN KNpc* pNPC, IN int nMagicActive /* = 0 */) const
{
	_ASSERT(this != NULL);
	_ASSERT(nMagicActive >= 0);

	int nCount = nMagicActive;
	int	i;
	
	// »ù´¡ÊôÐÔµ÷ÕûNPC
	for (i = 0; i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryBaseAttrib[i]);
		if (INVALID_ATTRIB != pAttrib->nAttribType)
		{
			KItemNormalAttrib RemoveAttrib;
			RemoveAttrib.nAttribType = pAttrib->nAttribType;
			RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
			RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
			RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
			pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
		}
	}

	for (i = 0; i < sizeof(m_aryMagicAttrib)/sizeof(m_aryMagicAttrib[0]); i++)
	{
		const KItemNormalAttrib* pAttrib;
		pAttrib = &(m_aryMagicAttrib[i]);

		if (INVALID_ATTRIB != pAttrib->nAttribType)		// TODO: Îª -1 ¶¨ÒåÒ»¸ö³£Á¿?
		{
			KItemNormalAttrib RemoveAttrib;
			if (i & 1)						// ÎªÆæÊý£¬ÊÇºó×º£¨i´ÓÁã¿ªÊ¼£©
			{
				if (nCount > 0)
				{
					RemoveAttrib.nAttribType = pAttrib->nAttribType;
					RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
					RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
					RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
					pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
					nCount--;
				}
			}
			else
			{
				RemoveAttrib.nAttribType = pAttrib->nAttribType;
				RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
				RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
				RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
				pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
			}
		}
	}
}

/******************************************************************************
¹¦ÄÜ:	½«itemÉÏµÄµÚNÏîÒþ²ØÄ§·¨ÊôÐÔÓ¦ÓÃµ½NPCÉíÉÏ
Èë¿Ú£º	pNPC: Ö¸ÏòNPCµÄÖ¸Õë
³ö¿Ú:	Ä§·¨±»Ó¦ÓÃ¡£
		¾ßÌå¹¤×÷ÓÉKNpcµÄ³ÉÔ±º¯ÊýÍê³É¡£
		KItem ¶ÔÏó±¾ÉíÃ»ÓÐ³ÉÔ±±äÁ¿±»ÐÞ¸Ä
******************************************************************************/
void KItem::ApplyHiddenMagicAttribToNPC(IN KNpc* pNPC, IN int nMagicActive) const
{
	_ASSERT(this != NULL);
	if (nMagicActive <= 0)
		return;

	const KItemNormalAttrib* pAttrib;
	pAttrib = &(m_aryMagicAttrib[(nMagicActive << 1) - 1]);	// ºó×ºÎªÒþ²ØÊôÐÔËùÒÔ³Ë2¼õÒ»
	if (-1 != pAttrib->nAttribType)
	{
		pNPC->ModifyAttrib(pNPC->m_Index, (void *)pAttrib);
	}
}

/******************************************************************************
¹¦ÄÜ:	½«itemÉÏµÄµÚNÏîÒþ²ØÄ§·¨ÊôÐÔ´ÓNPCÉíÉÏÒÆ³ý
Èë¿Ú£º	pNPC: Ö¸ÏòNPCµÄÖ¸Õë£¬nMagicActive£ºµÚnÏîÄ§·¨ÊôÐÔ
³ö¿Ú:	Ä§·¨±»ÒÆ³ý¡£
		¾ßÌå¹¤×÷ÓÉKNpcµÄ³ÉÔ±º¯ÊýÍê³É¡£
		KItem ¶ÔÏó±¾ÉíÃ»ÓÐ³ÉÔ±±äÁ¿±»ÐÞ¸Ä
******************************************************************************/
void KItem::RemoveHiddenMagicAttribFromNPC(IN KNpc* pNPC, IN int nMagicActive) const
{
	_ASSERT(this != NULL);
	if (nMagicActive <= 0)
		return;

	const KItemNormalAttrib* pAttrib;
	pAttrib = &(m_aryMagicAttrib[(nMagicActive << 1) - 1]);	// ºó×ºÎªÒþ²ØÊôÐÔËùÒÔ³Ë2¼õÒ»
	if (-1 != pAttrib->nAttribType)
	{
		KItemNormalAttrib RemoveAttrib;
		RemoveAttrib.nAttribType = pAttrib->nAttribType;
		RemoveAttrib.nValue[0] = -pAttrib->nValue[0];
		RemoveAttrib.nValue[1] = -pAttrib->nValue[1];
		RemoveAttrib.nValue[2] = -pAttrib->nValue[2];
		pNPC->ModifyAttrib(pNPC->m_Index, (void *)&RemoveAttrib);
	}
}

/******************************************************************************
¹¦ÄÜ:	¸ù¾ÝÅäÖÃÎÄ¼þÖÐµÄÊý¾Ý,ÎªitemµÄ¸÷Ïî¸³³õÖµ
Èë¿Ú£º	pData: ¸ø³öÀ´×ÔÅäÖÃÎÄ¼þµÄÊý¾Ý
³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, ÒÔÏÂ³ÉÔ±±äÁ¿±»Öµ:
			m_CommonAttrib,m_aryBaseAttrib,m_aryRequireAttrib
		Ê§°ÜÊ±·µ»ØÁã
ËµÃ÷:	CBR: Common,Base,Require
******************************************************************************/
BOOL KItem::SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT* pData)
{
	_ASSERT(pData != NULL);
	
	BOOL bEC = FALSE;
	if (pData)
	{
		//SetAttrib_Common(pData);
		*this = *pData;		// ÔËËã·ûÖØÔØ
		SetAttrib_Base(pData->m_aryPropBasic);
		SetAttrib_Req(pData->m_aryPropReq);
		bEC = TRUE;
	}
	return bEC;
}

BOOL KItem::SetAttrib_CBR(IN const KBASICPROP_EQUIPMENT_GOLD* pData)
{
	_ASSERT(pData != NULL);
	
	BOOL bEC = FALSE;
	if (pData)
	{
		//SetAttrib_Common(pData);
		*this = *pData;		// ÔËËã·ûÖØÔØ
		SetAttrib_Base(pData->m_aryPropBasic);
		SetAttrib_Req(pData->m_aryPropReq);
		bEC = TRUE;
	}
	return bEC;
}

BOOL KItem::SetAttrib_Base(const KEQCP_BASIC* pBasic)
{
	for (int i = 0;
		 i < sizeof(m_aryBaseAttrib)/sizeof(m_aryBaseAttrib[0]); i++)
	{
		KItemNormalAttrib* pDst;
		const KEQCP_BASIC* pSrc;
		pDst = &(m_aryBaseAttrib[i]);
		pSrc = &(pBasic[i]);
		pDst->nAttribType = pSrc->nType;
		pDst->nValue[0] = ::GetRandomNumber(pSrc->sRange.nMin, pSrc->sRange.nMax);
		pDst->nValue[1] = 0;	// RESERVED
		pDst->nValue[2] = 0;	// RESERVED
		if (pDst->nAttribType == magic_durability_v)
			SetDurability(pDst->nValue[0]);
	}
	if (m_nCurrentDur == 0)	// ËµÃ÷Ã»ÓÐÄÍ¾Ã¶ÈÊôÐÔ
		m_nCurrentDur = -1;
	return TRUE;
}

BOOL KItem::SetAttrib_Req(const KEQCP_REQ* pReq)
{
	for (int i = 0;
		 i < sizeof(m_aryRequireAttrib)/sizeof(m_aryRequireAttrib[0]); i++)
	{
		KItemNormalAttrib* pDst;
		pDst = &(m_aryRequireAttrib[i]);
		pDst->nAttribType = pReq[i].nType;
		pDst->nValue[0] = pReq[i].nPara;
		pDst->nValue[1] = 0;	// RESERVED
		pDst->nValue[2] = 0;	// RESERVED
	}
	return TRUE;
}

/******************************************************************************
¹¦ÄÜ:	¸ù¾Ý´«ÈëµÄÊý¾Ý, ÎªitemµÄÄ§·¨ÊôÐÔ¸³³õÖµ
Èë¿Ú£º	pMA: ¸ø³öÊý¾Ý
³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, ÒÔÏÂ³ÉÔ±±äÁ¿±»Öµ:
			m_aryMagicAttrib
		Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItem::SetAttrib_MA(IN const KItemNormalAttrib* pMA)
{
	if (NULL == pMA)
		{ _ASSERT(FALSE); return FALSE; }

	for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); i++)
	{
		m_aryMagicAttrib[i] = pMA[i];
		// ÓÀ²»Ä¥Ëð
		if (m_aryMagicAttrib[i].nAttribType == magic_indestructible_b)
		{
			SetDurability(-1);
		}
	}
	return TRUE;
}

/******************************************************************************
¹¦ÄÜ:	¸ù¾Ý´«ÈëµÄÊý¾Ý, ÎªitemµÄÄ§·¨ÊôÐÔ¸³³õÖµ
Èë¿Ú£º	pMA: ¸ø³öÊý¾Ý
³ö¿Ú:	³É¹¦Ê±·µ»Ø·ÇÁã, ÒÔÏÂ³ÉÔ±±äÁ¿±»Öµ:
			m_aryMagicAttrib
		Ê§°ÜÊ±·µ»ØÁã
******************************************************************************/
BOOL KItem::SetAttrib_MA(IN const KMACP* pMA)
{
	if (NULL == pMA)
		{ _ASSERT(FALSE); return FALSE; }

	for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); i++)
	{
		const KMACP* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pMA[i]);
		pDst = &(m_aryMagicAttrib[i]);

		pDst->nAttribType = pSrc->nPropKind;
		pDst->nValue[0] =  ::GetRandomNumber(pSrc->aryRange[0].nMin, pSrc->aryRange[0].nMax);
		pDst->nValue[1] =  ::GetRandomNumber(pSrc->aryRange[1].nMin, pSrc->aryRange[1].nMax);
		pDst->nValue[2] =  ::GetRandomNumber(pSrc->aryRange[2].nMin, pSrc->aryRange[2].nMax);
	}
	return TRUE;
}

BOOL KItem::SetAttrib_MA(IN const int* pMA)
{
	if (NULL == pMA)
	{ _ASSERT(FALSE); return FALSE; }
	
	KTabFile MagicTab;
	MagicTab.Load("\\Settings\\Item\\GoldMagic.txt");
	for (int i = 0; i < sizeof(m_aryMagicAttrib) / sizeof(m_aryMagicAttrib[0]); i++)
	{
		const int* pSrc;
		KItemNormalAttrib* pDst;
		pSrc = &(pMA[i]);
		pDst = &(m_aryMagicAttrib[i]);
		
		int nType,nLow,nHigh;
		MagicTab.GetInteger(*pSrc + 1,"ÊôÐÔµ÷ÕûÀà±ð",0,&nType);
		pDst->nAttribType = nType;
		MagicTab.GetInteger(*pSrc + 1,"²ÎÊý1×îÐ¡Öµ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"²ÎÊý1×î´óÖµ",0,&nHigh);
		pDst->nValue[0] =  ::GetRandomNumber(nLow, nHigh);
		MagicTab.GetInteger(*pSrc + 1,"²ÎÊý2×îÐ¡Öµ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"²ÎÊý2×î´óÖµ",0,&nHigh);
		pDst->nValue[1] =  ::GetRandomNumber(nLow, nHigh);
		MagicTab.GetInteger(*pSrc + 1,"²ÎÊý3×îÐ¡Öµ",0,&nLow);
		MagicTab.GetInteger(*pSrc + 1,"²ÎÊý3×î´óÖµ",0,&nHigh);
		pDst->nValue[2] =  ::GetRandomNumber(nLow, nHigh);
	}
	MagicTab.Clear();
	return TRUE;
}

void KItem::operator = (const KBASICPROP_EQUIPMENT& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = 0;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	::strcpy(pCA->szScript,	   "");
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
#ifndef _SERVER
	pCA->uPrice = 0; 

    ::strcpy(pCA->szImageName, sData.m_szImageName);
    //::strcpy(pCA->szIntro,       sData.m_szIntro);
    //Ken Nguyen chia dong` chu' thich'
    char szIntro[516];
    sprintf(szIntro, "%s", sData.m_szIntro);
    int offset = 0, nL = 0, nS = 0;
    int nStrL = strlen(szIntro);
    while(szIntro[offset] != 0)
    {
        if(szIntro[offset] == '<')
        {
        if(szIntro[offset+1] == 'e' &&
        szIntro[offset+2] == 'n' &&
        szIntro[offset+3] == 't' &&
        szIntro[offset+4] == 'e' &&
        szIntro[offset+5] == 'r' &&
        szIntro[offset+6] == '>')
        nL = -6;
        if(szIntro[offset+1] == 'c' &&
        szIntro[offset+2] == 'o' &&
        szIntro[offset+3] == 'l' &&
        szIntro[offset+4] == 'o' &&
        szIntro[offset+5] == 'r' &&
        szIntro[offset+6] == '>')
        nL -= 6;
        if(szIntro[offset+1] == 'c' &&
        szIntro[offset+2] == 'o' &&
        szIntro[offset+3] == 'l' &&
        szIntro[offset+4] == 'o' &&
        szIntro[offset+5] == 'r' &&
        szIntro[offset+6] == '=')
            {
            int k;
            for(k = 0; k<10; k++)
            {
            if(szIntro[offset+6+k] == '>') break;
            }
            nL -= 6+k;
            }
        }
        if(nL == 40) //doan. dai` hay ngan' tuy` vao` so' nay`
        {
        while(szIntro[offset+nS] < 32 || szIntro[offset+nS] > 126)
            {
            nS++;
            }
            memmove(&szIntro[offset+nS+8], &szIntro[offset+nS], nStrL-(offset+nS)+1);
            memcpy(&szIntro[offset+nS]," <enter>",8);
            offset += nS;
            nStrL += 8;
            nS = 0;
            nL = 0;
        }
        offset++;
        nL++;
    }
    if(strlen(szIntro) > 255)
    szIntro[255] = 0;
    ::memcpy(pCA->szIntro, szIntro, 256);
    ///////////////////////////////////////////
    m_Image.Color.Color_b.a = 255;
    m_Image.nFrame = 0;
    m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
    m_Image.nType = ISI_T_SPR;
    ::strcpy(m_Image.szImage, pCA->szImageName);
    m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_MEDMATERIAL& sData)
{
	// ¸³Öµ: ¹²Í¬ÊôÐÔ²¿·Ö
	KItemCommonAttrib* pCA = &(m_CommonAttrib);
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	::strcpy(pCA->szScript,	   "");
	
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
#ifndef _SERVER
	pCA->uPrice = 0; 

	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	// ¸³Öµ: »ù±¾ÊôÐÔ²¿·Ö
	KItemNormalAttrib* pBA = m_aryBaseAttrib;
	pBA[0].nAttribType = sData.m_nAttrib1_Type;
	pBA[0].nValue[0]   = sData.m_nAttrib1_Para;
	pBA[1].nAttribType = sData.m_nAttrib2_Type;
	pBA[1].nValue[0]   = sData.m_nAttrib2_Para;
	pBA[2].nAttribType = sData.m_nAttrib3_Type;
	pBA[2].nValue[0]   = sData.m_nAttrib3_Para;
	
	// ¸³Öµ: ÐèÇóÊôÐÔ²¿·Ö: ÎÞ
	// ¸³Öµ: Ä§·¨ÊôÐÔ²¿·Ö: ÎÞ
#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_MINE& sData)
{
	// ¸³Öµ: ¹²Í¬ÊôÐÔ²¿·Ö
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = sData.m_nDelet;
	pCA->nGoldId		 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	::strcpy(pCA->szScript,	   sData.m_szScript);
	
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
#ifndef _SERVER
	pCA->uPrice = 0; 

	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	// ¸³Öµ: ÐèÇóÊôÐÔ²¿·Ö: ÎÞ
	// ¸³Öµ: Ä§·¨ÊôÐÔ²¿·Ö: ÎÞ
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

void KItem::operator = (const KBASICPROP_QUEST& sData)
{
	// ¸³Öµ: ¹²Í¬ÊôÐÔ²¿·Ö
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = 0;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = 0;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nPriceXu		 = sData.m_nPriceXu;
	pCA->nLevel			 = 1;
	pCA->nSeries		 = -1;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = sData.m_nDelet;
	pCA->nGoldId		 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	pCA->nIsSell		= sData.m_nIsSell;
	pCA->nIsTrade		= sData.m_nIsTrade;
	pCA->nIsDrop		= sData.m_nIsDrop;
	::strcpy(pCA->szItemName,  sData.m_szName);
	::strcpy(pCA->szScript,	   sData.m_szScript);
	
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
#ifndef _SERVER
	pCA->uPrice = 0; 

	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	// ¸³Öµ: ÐèÇóÊôÐÔ²¿·Ö: ÎÞ
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryBaseAttrib));
	// ¸³Öµ: Ä§·¨ÊôÐÔ²¿·Ö: ÎÞ
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryBaseAttrib));
#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_TOWNPORTAL& sData)
{
	// ¸³Öµ: ¹²Í¬ÊôÐÔ²¿·Ö
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = 0;
	pCA->nParticularType = 0;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = 0;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = 1;
	pCA->nSeries		 = -1;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	::strcpy(pCA->szScript,	   "");
	
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
#ifndef _SERVER
	pCA->uPrice = 0; 

	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	// ¸³Öµ: ÐèÇóÊôÐÔ²¿·Ö: ÎÞ
	ZeroMemory(m_aryRequireAttrib, sizeof(m_aryBaseAttrib));
	// ¸³Öµ: Ä§·¨ÊôÐÔ²¿·Ö: ÎÞ
	ZeroMemory(m_aryMagicAttrib, sizeof(m_aryBaseAttrib));
#ifndef _SERVER
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

void KItem::operator = (const KBASICPROP_MEDICINE& sData)
{
	// ¸³Öµ: ¹²Í¬ÊôÐÔ²¿·Ö
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->bStack			 = sData.m_bStack;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	::strcpy(pCA->szScript,	   "");
	
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
#ifndef _SERVER
	pCA->uPrice = 0; 

	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
#endif
	// ¸³Öµ: »ù±¾ÊôÐÔ²¿·Ö
	ZeroMemory(m_aryBaseAttrib, sizeof(m_aryBaseAttrib));
	KItemNormalAttrib* pBA = m_aryBaseAttrib;
	pBA[0].nAttribType = sData.m_aryAttrib[0].nAttrib;
	pBA[0].nValue[0]   = sData.m_aryAttrib[0].nValue;
	pBA[0].nValue[1]   = sData.m_aryAttrib[0].nTime;
	pBA[1].nAttribType = sData.m_aryAttrib[1].nAttrib;
	pBA[1].nValue[0]   = sData.m_aryAttrib[1].nValue;
	pBA[1].nValue[1]   = sData.m_aryAttrib[1].nTime;
	
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

void KItem::operator = (const KBASICPROP_EQUIPMENT_UNIQUE& sData)
{
	// ¸³Öµ: ¹²Í¬ÊôÐÔ²¿·Ö
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->bStack			 = 0;
	pCA->nSet			 = 0;
	pCA->nSetId			 = 0;
	pCA->nSetNum		 = 0;
	pCA->nBigSet		 = 0;
	pCA->nGoldId		 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	::strcpy(pCA->szScript,	   "");
	
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
#ifndef _SERVER
	pCA->uPrice = 0; 

	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);

	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

//flying add this overloaded operator to generate a gold item.
void KItem::operator = (const KBASICPROP_EQUIPMENT_GOLD& sData)
{
	KItemCommonAttrib* pCA = &m_CommonAttrib;	
	pCA->nItemGenre		 = sData.m_nItemGenre;
	pCA->nDetailType	 = sData.m_nDetailType;
	pCA->nParticularType = sData.m_nParticularType;
	pCA->nObjIdx		 = sData.m_nObjIdx;
	pCA->nPrice			 = sData.m_nPrice;
	pCA->nLevel			 = sData.m_nLevel;
	pCA->nSeries		 = sData.m_nSeries;
	pCA->nWidth			 = sData.m_nWidth;
	pCA->nHeight		 = sData.m_nHeight;
	pCA->nSet			 = sData.m_nSet;
	pCA->nSetId			 = sData.m_nSetId;
	pCA->nSetNum		 = sData.m_nSetNum;
	pCA->nBigSet		 = sData.m_nUpSet;
	pCA->nGoldId		 = sData.m_nId;
	pCA->bStack			 = 0;
	pCA->nStackNum		 = 1;
	pCA->nEnChance		 = 0;
	pCA->nPoint			 = 0;
	::strcpy(pCA->szItemName,  sData.m_szName);
	::strcpy(pCA->szScript,	   "");
	
	pCA->LimitTime.bYear = 0;
	pCA->LimitTime.bMonth = 0;
	pCA->LimitTime.bDay = 0;
	pCA->LimitTime.bHour = 0;
#ifndef _SERVER
	pCA->uPrice = 0; 

	::strcpy(pCA->szImageName, sData.m_szImageName);
	::strcpy(pCA->szIntro,	   sData.m_szIntro);
	m_Image.Color.Color_b.a = 255;
	m_Image.nFrame = 0;
	m_Image.nISPosition = IMAGE_IS_POSITION_INIT;
	m_Image.nType = ISI_T_SPR;
	::strcpy(m_Image.szImage, pCA->szImageName);
	m_Image.uImage = 0;
#endif
}

BOOL KItem::Gen_Equipment_Unique(const KBASICPROP_EQUIPMENT* pEqu,
								 const KBASICPROP_EQUIPMENT_UNIQUE* pUni)
{
	_ASSERT(this != NULL);
	_ASSERT(pEqu != NULL);
	_ASSERT(pUni != NULL);

	if (NULL == pEqu || NULL == pUni)
		{ _ASSERT(FALSE); return FALSE; }

	// ¸³Öµ: ¹²Í¬ÊôÐÔ²¿·Ö
	*this = *pUni;		// ÔËËã·ûÖØÔØ
	KItemCommonAttrib* pCA = &m_CommonAttrib;
	pCA->bStack  = pEqu->m_bStack;
	pCA->nWidth  = pEqu->m_nWidth;
	pCA->nHeight = pEqu->m_nHeight;

	SetAttrib_Base(pEqu->m_aryPropBasic);		// ¸³Öµ: »ù±¾ÊôÐÔ²¿·Ö
	SetAttrib_Req(pUni->m_aryPropReq);			// ¸³Öµ: ÐèÇóÊôÐÔ²¿·Ö
	SetAttrib_MA(pUni->m_aryMagicAttribs);		// ¸³Öµ: Ä§·¨ÊôÐÔ²¿·Ö

	return TRUE;
}

void KItem::Remove()
{
	m_nIndex = 0;
}

BOOL KItem::SetBaseAttrib(IN const KItemNormalAttrib* pAttrib)
{
	if (!pAttrib)
		return FALSE;

	for (int i = 0; i < sizeof(m_aryBaseAttrib) / sizeof(m_aryBaseAttrib[0]); i++)
	{
		m_aryBaseAttrib[i] = pAttrib[i];
	}
	return TRUE;
}

BOOL KItem::SetRequireAttrib(IN const KItemNormalAttrib* pAttrib)
{
	if (!pAttrib)
		return FALSE;

	for (int i = 0; i < sizeof(m_aryRequireAttrib) / sizeof(m_aryRequireAttrib[0]); i++)
	{
		m_aryRequireAttrib[i] = pAttrib[i];
	}
	return TRUE;
}

BOOL KItem::SetMagicAttrib(IN const KItemNormalAttrib* pAttrib)
{
	return SetAttrib_MA(pAttrib);
}

//------------------------------------------------------------------
//	Ä¥Ëð£¬·µ»ØÖµ±íÊ¾Ê£ÓàÄÍ¾Ã¶È
//------------------------------------------------------------------
int KItem::Abrade(IN const int nRandRange)
{
	if (m_nCurrentDur == -1 || nRandRange == 0)	// ÓÀ²»Ä¥Ëð
		return -1;

	if (g_Random(nRandRange) == 0)	// nRandRange·ÖÖ®Ò»µÄ¸ÅÂÊ
	{
		m_nCurrentDur--;
		if (m_nCurrentDur == 0)
		{
			return 0;
		}
	}
	return m_nCurrentDur;
}

#ifndef _SERVER
void KItem::Paint(int nX, int nY,BOOL bStack/* = TRUE*/)
{
	m_Image.oPosition.nX = nX;
	m_Image.oPosition.nY = nY;
	m_Image.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;
	g_pRepresent->DrawPrimitives(1, &m_Image, RU_T_IMAGE, TRUE);

	
	if (m_CommonAttrib.bStack && bStack)
	{
		int nNum = m_CommonAttrib.nStackNum;
		if (nNum > 1 && nNum < 1000)
		{
			int nFontSize = 12;
			char szNum[4];
			int nLen = sprintf(szNum, "%d", nNum);
			szNum[3] = 0;
			g_pRepresent->OutputText(nFontSize, szNum, KRF_ZERO_END,
				nX + (m_CommonAttrib.nWidth * 27) - nLen * (nFontSize ) / 2,
				nY + (m_CommonAttrib.nHeight) + nFontSize + 1, 0xFFFFFF00);
		}
	}
}

void KItem::GetDesc(char* pszMsg, bool bShowPrice, int nPriceScale, int nActiveAttrib)
{
	char	szColor[item_number][32] = 
	{
		"",
		"<color=White>",
		"",
		"",
		"<color=Yellow>",
	};
	
	if (m_CommonAttrib.nItemGenre == item_equip)
	{
		if (m_CommonAttrib.nGoldId)	// »Æ½ð×°±¸
		{
			strcpy(szColor[item_equip], "<color=Yellow>");
		}
		else if (m_CommonAttrib.nPoint > 0)
		{
			strcpy(szColor[item_equip], "<color=Purple>");
		}
		else if (m_aryMagicAttrib[0].nAttribType)	// Ä§·¨×°±¸
		{
			strcpy(szColor[item_equip], "<color=Blue>");
		}
		else
		{
			strcpy(szColor[item_equip], "<color=White>");
		}
	}
	if (m_CommonAttrib.nEnChance)
	{
		char sItemName[64];
		sprintf(sItemName,"%s + %d",m_CommonAttrib.szItemName,m_CommonAttrib.nEnChance);
		strcpy(pszMsg, szColor[m_CommonAttrib.nItemGenre]);
		if (m_CommonAttrib.nItemGenre == 0)
		{
			char    TextLevel[10];
			int        LevelItem = m_CommonAttrib.nLevel;

			if(LevelItem > 10)
			{
				if (LevelItem < 100)
				{
					if (LevelItem%10 == 0)
					{
						sprintf(TextLevel, "%s [CÊp %d]", sItemName, LevelItem/(LevelItem/10));
					}
					else
					{
						sprintf(TextLevel, "%s [CÊp %d]", sItemName, LevelItem%10);
					}
				}
				else if (LevelItem < 1000)
				{
					if (LevelItem%100 == 0)
					{
						sprintf(TextLevel, "%s [CÊp %d]", sItemName, LevelItem/(LevelItem/100));
					}
					else
					{
						sprintf(TextLevel, "%s [CÊp %d]", sItemName, LevelItem%100);
					}
				}
			}
			else
			{
				sprintf(TextLevel, "%s [CÊp %d]", sItemName, LevelItem);
			}
			strcat(pszMsg, TextLevel);
		}
		else
		{
			strcat(pszMsg, sItemName);
		}
	}
	else
	{
		strcpy(pszMsg, szColor[m_CommonAttrib.nItemGenre]);
		if (m_CommonAttrib.nItemGenre == 0)
		{
			char    TextLevel[10];
			int        LevelItem = m_CommonAttrib.nLevel;

			if(LevelItem > 10)
			{
				if (LevelItem < 100)
				{
					if (LevelItem%10 == 0)
					{
						sprintf(TextLevel, "%s [CÊp %d]", m_CommonAttrib.szItemName, LevelItem/(LevelItem/10));
					}
					else
					{
						sprintf(TextLevel, "%s [CÊp %d]", m_CommonAttrib.szItemName, LevelItem%10);
					}
				}
				else if (LevelItem < 1000)
				{
					if (LevelItem%100 == 0)
					{
						sprintf(TextLevel, "%s [CÊp %d]", m_CommonAttrib.szItemName, LevelItem/(LevelItem/100));
					}
					else
					{
						sprintf(TextLevel, "%s [CÊp %d]", m_CommonAttrib.szItemName, LevelItem%100);
					}
				}
			}
			else
			{
				sprintf(TextLevel, "%s [CÊp %d]", m_CommonAttrib.szItemName, LevelItem);
			}
			strcat(pszMsg, TextLevel);
		}
		else
		{
			strcat(pszMsg, m_CommonAttrib.szItemName);
		}
	}

	strcat(pszMsg, "\n");
	if (bShowPrice && nPriceScale > 0)
	{
		if (m_CommonAttrib.nItemGenre == item_task)
		{
			char szPrice[32];
			sprintf(szPrice, "Gi¸ c¶ : %d xu", m_CommonAttrib.nPriceXu);
			strcat(pszMsg, szPrice);
			strcat(pszMsg, "\n<bclr=Black>");
		}
		else
		{
			char szPrice[32];
			if (m_CommonAttrib.nPrice / nPriceScale < 10000)
			{
			sprintf(szPrice, "Gi¸ c¶: %d l­îng", m_CommonAttrib.nPrice / nPriceScale);
			}
			else
			{

					int vPrice = (m_CommonAttrib.nPrice / nPriceScale) / 10000;
					int lPrice = (m_CommonAttrib.nPrice / nPriceScale) % 10000;
					if (lPrice == 0)
					{
						sprintf(szPrice, "Gi¸ c¶: %d v¹n l­îng", vPrice);
					}
					else
					{
						sprintf(szPrice, "Gi¸ c¶: %d v¹n %d l­îng", vPrice, lPrice);
					}
			}
		strcat(pszMsg, szPrice);
		strcat(pszMsg, "\n");			
		}
	}
	
	if (m_CommonAttrib.nItemGenre == item_task)
	{	
		if (m_CommonAttrib.nIsSell == 1)
		{	
			strcat(pszMsg, "<color=Green>VËt phÈm cã thÓ b¸n t¹i cöa hµng<color>");
			strcat(pszMsg, "\n");
		}
		else
		{	strcat(pszMsg, "<color=Red>VËt phÈm kh«ng thÓ b¸n t¹i cöa hµng<color>");
			strcat(pszMsg, "\n");
		}

/*		if(m_CommonAttrib.nIsTrade == 1)
			{	strcat(pszMsg, "<color=Green>Cã thÓ giao dÞch<color>");
				strcat(pszMsg, "\n");
			}
		else 
		{	
			strcat(pszMsg, "<color=Red>Kh«ng thÓ giao dÞch<color>");
				strcat(pszMsg, "\n");
		}
*/
		

	}
	
	if (m_CommonAttrib.nItemGenre == item_equip ||
		(m_CommonAttrib.nItemGenre == item_mine &&
		(m_CommonAttrib.nParticularType == 150 ||
		m_CommonAttrib.nParticularType == 152 ||
		m_CommonAttrib.nParticularType == 154 ||
		m_CommonAttrib.nParticularType == 201 ||
		m_CommonAttrib.nParticularType == 203 ||
		m_CommonAttrib.nParticularType == 205 )
		))
	{
		switch(m_CommonAttrib.nSeries)
		{
		case series_metal:
		strcat(pszMsg, "<color=White>Thuéc tÝnh ngò hµnh trang bÞ: <color=Metal>HÖ Kim");
		break;
		case series_wood:
		strcat(pszMsg, "<color=White>Thuéc tÝnh ngò hµnh trang bÞ: <color=Wood>HÖ Méc");
		break;
		case series_water:
		strcat(pszMsg, "<color=White>Thuéc tÝnh ngò hµnh trang bÞ: <color=Water>HÖ Thñy");
		break;
		case series_fire:
		strcat(pszMsg, "<color=White>Thuéc tÝnh ngò hµnh trang bÞ: <color=Fire>HÖ Háa");
		break;
		case series_earth:
		strcat(pszMsg, "<color=White>Thuéc tÝnh ngò hµnh trang bÞ: <color=Earth>HÖ Thæ\n");
		break;
		}
	}

	strcat(pszMsg, "\n");
	strcat(pszMsg, "<color=White>");
	strcat(pszMsg, m_CommonAttrib.szIntro);
	strcat(pszMsg, "\n");

	if (m_CommonAttrib.nItemGenre == item_mine && 
		m_CommonAttrib.nParticularType >= 200 && 
		m_CommonAttrib.nParticularType <= 205)
	{
		char szIntor[64] = "!";
		
		KTabFile MagicTab;
		
		MagicTab.Load("\\Settings\\Item\\magicattriblevel_index.txt");

		char szTmp[8];
		sprintf(szTmp,"%d",m_aryBaseAttrib[0].nValue[0]);

		MagicTab.GetString(szTmp,"DESC","",szIntor,64);
		strcat(pszMsg, "<color=Yellow>Thuéc tÝnh ma ph¸p: <color><color=Cyan>");
		strcat(pszMsg, szIntor);
		strcat(pszMsg, "\n");
		MagicTab.GetString(szTmp,"FIT_EQUIP","",szIntor,64);
		strcat(pszMsg, "<color=Yellow>Trang bÞ cã thÓ kh¶m n¹m: <color><color=Cyan>");
		strcat(pszMsg, szIntor);
		strcat(pszMsg, "\n");
		strcat(pszMsg, "\n");
	}

	if (m_CommonAttrib.nItemGenre == item_mine && (m_CommonAttrib.nParticularType == 147 ||
		(m_CommonAttrib.nParticularType >= 200 && m_CommonAttrib.nParticularType <= 205)
		))
	{
		char szLevel[32];
		sprintf(szLevel, "<color=Blue>PhÈm chÊt thuéc tÝnh: %d<color=White>", m_CommonAttrib.nLevel);
		strcat(pszMsg, szLevel);
		strcat(pszMsg, "\n");	
	}

	for (int i = 0; i < 7; i++)
	{
		if (!m_aryBaseAttrib[i].nAttribType)
		{
			continue;
		}
		if (m_aryBaseAttrib[i].nAttribType == magic_durability_v)
		{
			char	szDurInfo[32];
			if (m_nCurrentDur == -1)
				sprintf(szDurInfo, "<color=Yellow>Kh«ng thÓ ph¸ hñy<color=White>");
			else
				sprintf(szDurInfo, "§é bÒn trang bÞ: %3d / %3d", GetDurability(), GetMaxDurability());
			strcat(pszMsg, szDurInfo);
		}
		else
		{
			char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryBaseAttrib[i]);
			if (!pszInfo || !pszInfo[0])
				continue;
			strcat(pszMsg, pszInfo);
		}
		strcat(pszMsg, "\n");
	}
	for (i = 0; i < 6; i++)
	{
		if (!m_aryRequireAttrib[i].nAttribType)
		{
			continue;
		}
		char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryRequireAttrib[i]);
		if (!pszInfo || !pszInfo[0])
			continue;
		if (Player[CLIENT_PLAYER_INDEX].m_ItemList.EnoughAttrib(&m_aryRequireAttrib[i]))
		{
			strcat(pszMsg, "<color=White>");
		}
		else
		{
			strcat(pszMsg, "<color=Red>");
		}
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, "\n");
	}

	for (i = 0; i < 6; i++)
	{
		if (!m_aryMagicAttrib[i].nAttribType)
		{
			if (i < m_CommonAttrib.nPoint)
			{
				strcat(pszMsg, "<color=Yellow>Ch­a kh¶m n¹m<color=white>");
				strcat(pszMsg, "\n");
			}
			continue;
		}
		char* pszInfo = (char *)g_MagicDesc.GetDesc(&m_aryMagicAttrib[i]);
		if (!pszInfo || !pszInfo[0])
			continue;
		if ((i & 1) == 0)
		{
			if (m_CommonAttrib.nGoldId > 0)
				strcat(pszMsg, "<color=HYellow>");
			else if (m_CommonAttrib.nPoint)
					strcat(pszMsg, "<color=Purple>");
			else
				strcat(pszMsg, "<color=HBlue>");
		}
		else
		{
			if ((i>>1) < nActiveAttrib)
			{
				if (m_CommonAttrib.nGoldId > 0)
					strcat(pszMsg, "<color=HYellow>");
				else if (m_CommonAttrib.nPoint)
					strcat(pszMsg, "<color=Purple>");
				else
					strcat(pszMsg, "<color=HBlue>");
			}
			else
			{
				if (m_CommonAttrib.nGoldId > 0)
					strcat(pszMsg, "<color=DYellow>");
				else
					strcat(pszMsg, "<color=DBlue>");
			}
		}
		strcat(pszMsg, pszInfo);
		strcat(pszMsg, "\n");
	}

		if (m_CommonAttrib.LimitTime.bYear)
	{
		time_t rawtime;
		struct tm * timeinfo;
		
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );

		char sTmp[64];
		sprintf(sTmp,"<color=Green>VËt phÈm sÏ hÕt h¹n vµo lóc: %d giê ngµy %d th¸ng %d n¨m %d",
			m_CommonAttrib.LimitTime.bHour,
			m_CommonAttrib.LimitTime.bDay,
			m_CommonAttrib.LimitTime.bMonth,
			m_CommonAttrib.LimitTime.bYear);
		strcat(pszMsg, " \n");
		strcat(pszMsg,sTmp);
		strcat(pszMsg, " \n");
	}

	if (m_CommonAttrib.nColor==2|| m_aryMagicAttrib[0].nAttribType)
	{
		strcat(pszMsg, "\n");
        switch(m_CommonAttrib.nDetailType)
        {
        case equip_meleeweapon:
            {
                switch(m_CommonAttrib.nSeries)
                {
                case series_metal:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                    break;
                case series_wood:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                    break;
                case series_water:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                    break;
                case series_fire:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                    break;
                case series_earth:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                    break;
                }
                strcat(pszMsg, " cña <color=Cyan>d©y chuyÒn<color> vµ <color=Cyan>y phôc<color> ®Ó kÝch ho¹t thuéc tÝnh ©m<color> ");
            break;
            
            }
        case equip_rangeweapon:
            {
                switch(m_CommonAttrib.nSeries)
                {
                case series_metal:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                    break;
                case series_wood:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                    break;
                case series_water:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                    break;
                case series_fire:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                    break;
                case series_earth:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                    break;
                }
                        strcat(pszMsg, " cña <color=Cyan>d©y chuyÒn<color> vµ <color=Cyan>y phôc<color> ®Ó kÝch ho¹t thuéc tÝnh ©m<color>");
                        break;
                    
            }
        case  equip_armor:
            {
                switch(m_CommonAttrib.nSeries)
                {
                case series_metal:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                    break;
                case series_wood:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                    break;
                case series_water:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                    break;
                case series_fire:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                    break;
                case series_earth:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                    break;
                }
                    strcat(pszMsg, " cña <color=Cyan>nhÉn (d­íi)<color> vµ <color=Cyan>th¾t l­ng <color>®Ó kÝch ho¹t thuéc tÝnh ©m<color>");
                    break;
            
            }
        case equip_ring:
            {
                switch(m_CommonAttrib.nSeries)
                {
                case series_metal:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                    break;
                case series_wood:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                    break;
                case series_water:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                    break;
                case series_fire:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                    break;
                case series_earth:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                    break;
                }
                    strcat(pszMsg, " cña <color=Cyan>ngäc béi<color> vµ <color=Cyan>bao tay <color>®Ó kÝch ho¹t thuéc tÝnh ©m<color> ");
                    break;
                
            }
        case equip_amulet:
            {
                switch(m_CommonAttrib.nSeries)
                {
                case series_metal:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                    break;
                case series_wood:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                    break;
                case series_water:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                    break;
                case series_fire:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                    break;
                case series_earth:
                    strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                    break;
                }
                    strcat(pszMsg, " cña <color=Cyan>nhÉn (d­íi)<color> vµ <color=Cyan>th¾t l­ng <color>®Ó kÝch ho¹t thuéc tÝnh ©m<color> ");
                    break;
                
            }
                case equip_boots:
                    {
                        switch(m_CommonAttrib.nSeries)
                        {
                        case series_metal:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                            break;
                        case series_wood:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                            break;
                        case series_water:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                            break;
                        case series_fire:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                            break;
                        case series_earth:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                            break;
                        }
                    strcat(pszMsg, " cña <color=Cyan>nãn<color> vµ <color=Cyan>vò khÝ <color>®Ó kÝch ho¹t thuéc tÝnh ©m<color> ");
                    break;
            
                    }
                case equip_belt:
                    {
                        switch(m_CommonAttrib.nSeries)
                        {
                        case series_metal:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                            break;
                        case series_wood:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                            break;
                        case series_water:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                            break;
                        case series_fire:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                            break;
                        case series_earth:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                            break;
                        }
                        
                    strcat(pszMsg, " cña <color=Cyan>ngäc béi<color> vµ <color=Cyan>bao tay<color> ®Ó kÝch ho¹t thuéc tÝnh ©m<color> ");
                    break;
            
                    }
                case equip_helm:
                    {
                        switch(m_CommonAttrib.nSeries)
                        {
                        case series_metal:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                            break;
                        case series_wood:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                            break;
                        case series_water:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                            break;
                        case series_fire:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                            break;
                        case series_earth:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                            break;
                        }
                        
                    strcat(pszMsg, " cña<color=Cyan> d©y chuyÒn<color> vµ <color=Cyan>y phôc<color> ®Ó kÝch ho¹t thuéc tÝnh ©m<color>");
                    break;
                
                    }
                case equip_cuff:
                    {
                        switch(m_CommonAttrib.nSeries)
                        {
                        case series_metal:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                            break;
                        case series_wood:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                            break;
                        case series_water:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                            break;
                        case series_fire:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                            break;
                        case series_earth:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                            break;
                        }
                        
                    strcat(pszMsg, " cña <color=Cyan>giµy<color> vµ <color=Cyan>nhÉn (trªn) <color>®Ó kÝch ho¹t thuéc tÝnh ©m<color> ");
                    break;
            
                    }
                case equip_pendant:
                    {
                        switch(m_CommonAttrib.nSeries)
                        {
                        case series_metal:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Earth>Thæ <color>");
                            break;
                        case series_wood:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Water>Thñy<color>");
                            break;
                        case series_water:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Metal>Kim<color>");
                            break;
                        case series_fire:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Wood>Méc<color>");
                            break;
                        case series_earth:
                            strcat(pszMsg, "<color=Yellow>CÇn hÖ <color=Fire>Háa<color>");
                            break;
                        }
                        strcat(pszMsg, " cña <color=Cyan>nhÉn (trªn)<color> vµ <color=Cyan>giµy <color>®Ó kÝch ho¹t thuéc tÝnh ©m<color>");
                        break;
                    
                    }
                    strcat(pszMsg, "\n");
                break;  
				}
        }
	strcat(pszMsg, "\n");
	if (m_CommonAttrib.nGoldId)
	{
		strcat(pszMsg, " \n ");
		KTabFile GoldTab;

		char szGoldName[96];
		GoldTab.Load("\\Settings\\Item\\GoldItem.txt");
		for (int k = 0;k < m_CommonAttrib.nSetNum;k++)
		{
			GoldTab.GetString(m_CommonAttrib.nGoldId + 3 - m_CommonAttrib.nSetId + k,"","",szGoldName,96);
			int ncolor = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetGoldColor(m_CommonAttrib.nSet,k + 1);
			if (m_CommonAttrib.nSetId == k + 1)
			{
				strcat(pszMsg, " <color=Yellow>");
			}
			else if (ncolor == 2)
			{
				strcat(pszMsg, " <color=Green>");
			}
			else if (ncolor == 1)
			{
				strcat(pszMsg, " <color=HGreen>");
			}
			else if (ncolor == 0)
			{
				strcat(pszMsg, " <color=DGreen>");
			}
			strcat(pszMsg, szGoldName);
			strcat(pszMsg, " \n ");
		}
	}

	if (m_CommonAttrib.uPrice > 0)
	{
		strcat(pszMsg,"\n");
		strcat(pszMsg, "<color=Yellow>");
		char sOrice[16];
		int nN = m_CommonAttrib.uPrice / 100000000;
		int nW = m_CommonAttrib.uPrice % 100000000 / 10000;
		int nG = m_CommonAttrib.uPrice % 10000;
	if (nN > 0)
	{
		sprintf(sOrice,"Gi¸ niªm yÕt: %d ngh×n %d v¹n %d l­îng",m_CommonAttrib.uPrice / 100000000,nW,nG);
	}
	else
	{
		if (nW == 0)
		sprintf(sOrice,"Gi¸ niªm yÕt: %d l­îng",nG);
		else
		sprintf(sOrice,"Gi¸ niªm yÕt: %d v¹n %d l­îng",nW,nG);
	}
		strcat(pszMsg,sOrice);
		strcat(pszMsg,"\n");
	}
}
#endif

int KItem::GetMaxDurability()
{
	for (int i = 0; i < 7; i++)
	{
		if (m_aryBaseAttrib[i].nAttribType == magic_durability_v)
		{
			return m_aryBaseAttrib[i].nValue[0];
		}
	}
	return -1;
}

int KItem::GetTotalMagicLevel()
{
	int nRet = 0;
	for (int i = 0; i < 6; i++)
	{
		nRet += m_GeneratorParam.nGeneratorLevel[i];
	}
	return nRet;
}

int KItem::GetRepairPrice()
{
	if (ItemSet.m_sRepairParam.nMagicScale == 0)
		return 0;

	if (GetGenre() != item_equip)
		return 0;

	if (m_nCurrentDur == -1)
		return 0;

	int nMaxDur = GetMaxDurability();
	int nSumMagic = GetTotalMagicLevel();

	if (nMaxDur <= 0)
		return 0;


	return m_CommonAttrib.nPrice * ItemSet.m_sRepairParam.nPriceScale / 100 * (nMaxDur - m_nCurrentDur) / nMaxDur * (ItemSet.m_sRepairParam.nMagicScale + nSumMagic) / ItemSet.m_sRepairParam.nMagicScale;
}

BOOL KItem::CanBeRepaired()
{
	if (GetGenre() != item_equip)
		return FALSE;

	if (m_nCurrentDur == -1)
		return FALSE;

	int nMaxDur = GetMaxDurability();
	if (m_nCurrentDur == nMaxDur)
		return FALSE;

	return TRUE;
}

BOOL KItem::CanStack( int nOldIdx )
{
	if (m_CommonAttrib.bStack)
	{
		if (m_CommonAttrib.nItemGenre == Item[nOldIdx].GetGenre()
			&& m_CommonAttrib.nDetailType == Item[nOldIdx].GetDetailType()
			&& m_CommonAttrib.nParticularType == Item[nOldIdx].GetParticular()
			&& m_CommonAttrib.nSeries == Item[nOldIdx].GetSeries()
			&& m_CommonAttrib.nItemGenre != 1
			&&Item[nOldIdx].GetStackNum() < 50)
		{
			if (m_CommonAttrib.nItemGenre == item_mine && m_CommonAttrib.nParticularType == 147 && m_CommonAttrib.nLevel != Item[nOldIdx].GetLevel())
			{
				return FALSE;
			}

			if (m_CommonAttrib.nItemGenre == item_mine && m_CommonAttrib.nParticularType >= 149 && m_CommonAttrib.nParticularType <= 155
				&& m_CommonAttrib.nParticularType % 2 == 0 && m_CommonAttrib.nSeries != Item[nOldIdx].GetSeries())
			{
				return FALSE;
			}

			if (m_CommonAttrib.nItemGenre == item_mine && m_CommonAttrib.nParticularType >= 2 && m_CommonAttrib.nParticularType <= 155
				&& m_CommonAttrib.nParticularType % 2 == 1 && m_CommonAttrib.nSeries != Item[nOldIdx].GetSeries())
			{
				return FALSE;
			}
			return TRUE;
		}
	}
	
	return FALSE;
}

int KItem::Stack( int nIdx )
{
	m_CommonAttrib.nStackNum += nIdx;
	
	if (m_CommonAttrib.nStackNum > 50)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int KItem::EnChance( int nEnChance /*= 1*/ )
{
// #ifdef _SERVER
// 	if (GetRandomNumber(0,10) <= m_CommonAttrib.nEnChance)
// 	{
// 		return 0;
// 	}
// #endif

		if (nEnChance < 10)
		{
			m_CommonAttrib.nEnChance = nEnChance;
			for (int i = 0;i < 6;i++)
			{
				if (m_aryMagicAttrib[i].nAttribType)
				{
					if (m_aryMagicAttrib[i].nValue[0] >= 5)
					{
						m_aryMagicAttrib[i].nValue[0] += m_aryMagicAttrib[i].nValue[0] * nEnChance / 18;
					}
				}
			}
		}
		else
		{
			m_CommonAttrib.nEnChance = 9;
			for (int i = 0;i < 6;i++)
			{
				if (m_aryMagicAttrib[i].nAttribType)
				{
					if (m_aryMagicAttrib[i].nValue[0] >= 5)
					{
						m_aryMagicAttrib[i].nValue[0] += m_aryMagicAttrib[i].nValue[0] * nEnChance / 18;
					}
				}
			}
		}
	
	return 1;
}

int KItem::IsPurple()
{
	return m_CommonAttrib.nPoint;
}

void KItem::SetTime( int bYear,BYTE bMonth,BYTE bDay,BYTE bHour )
{
	if (bYear)
	{
		while (bHour > 24)
		{
			bDay++;
			bHour -= 24;
		}
		while (bDay > 30)
		{
			if (((bMonth <= 7) && !(bMonth % 2)) || ((bMonth > 7) && !(bMonth % 2)))
			{
				bMonth++;
				bDay -= 30;
			}
			else if (((bMonth <= 7) && !(bMonth % 2) && bDay > 31 )||((bMonth > 7) && (bMonth % 2) && bDay > 31))
			{
				bMonth++;
				bDay -= 31;
			}
			else if (((bMonth <= 7) && !(bMonth % 2) && bDay == 31 )||((bMonth > 7) && (bMonth % 2) && bDay == 31))
			{
				break;
			}
		}
		while (bMonth > 12)
		{
			bYear++;
			bMonth -= 12;
		}
		m_CommonAttrib.LimitTime.bYear = bYear;
		m_CommonAttrib.LimitTime.bMonth = bMonth;
		m_CommonAttrib.LimitTime.bDay = bDay;
		m_CommonAttrib.LimitTime.bHour = bHour;
	}
}

BOOL KItem::HaveMaigc( int nAttribe,int nValue1Min,int nValue1Max,int nValue2Min,int nValue2Max,int nValue3Min,int nValue3Max )
{
	for (int i = 0;i < 6;i++)
	{
		if (m_aryMagicAttrib[i].nAttribType == nAttribe)
		{
			if (nValue1Min != -1)
			{
				if (m_aryMagicAttrib[i].nValue[0] < nValue1Min)
					return FALSE;
			}
			if (nValue1Max != -1)
			{
				if (m_aryMagicAttrib[i].nValue[0] > nValue1Max)
					return FALSE;
			}
			if (nValue2Min != -1)
			{
				if (m_aryMagicAttrib[i].nValue[1] < nValue2Min)
					return FALSE;
			}
			if (nValue2Max != -1)
			{
				if (m_aryMagicAttrib[i].nValue[1] > nValue2Max)
					return FALSE;
			}
			if (nValue3Min != -1)
			{
				if (m_aryMagicAttrib[i].nValue[2] < nValue3Min)
					return FALSE;
			}
			if (nValue3Max != -1)
			{
				if (m_aryMagicAttrib[i].nValue[2] > nValue3Max)
					return FALSE;
			}
			return TRUE;
		}
	}
	return FALSE;
}
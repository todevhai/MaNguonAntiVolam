#include	"KCore.h"
#include	"MyAssert.H"
#include	"KItem.h"
#include	"KItemSet.h"
#include	"KNpc.h"
#include	"KMath.h"
#include	"KPlayer.h"
#include	"KItemList.h"
#include	"KItemChangeRes.h"
#include	<time.h>
#ifdef _SERVER
//#include	"../../Headers/IServer.h"
#include	"KObjSet.h"
#endif
#ifndef _SERVER
#include	"CoreShell.h"
#include	"../../Headers/IClient.h"
#endif

// PK�����ͷ�������װ������װ�������Ȩֵ
#define		defEQUIP_POWER
#ifdef defEQUIP_POWER
	int		g_nEquipPower[itempart_num] =
	{2, 4, 2, 4, 1, 1, 1, 1, 1, 1, 0};
#endif

int KItemList::ms_ActiveEquip[itempart_num][MAX_ITEM_ACTIVE] = 
{
	{ itempart_foot, itempart_ring1	},	//	itempart_head = 0,
	{ itempart_weapon, itempart_head },	//	itempart_body,
	{ itempart_body, itempart_amulet },	//	itempart_belt,
	{ itempart_ring1, itempart_foot },	//	itempart_weapon,
	{ itempart_cuff, itempart_pendant },//	itempart_foot,
	{ itempart_belt, itempart_ring2 },	//	itempart_cuff,
	{ itempart_head, itempart_weapon },	//	itempart_amulet,
	{ itempart_cuff, itempart_pendant },//	itempart_ring1,
	{ itempart_amulet, itempart_body },	//	itempart_ring2,
	{ itempart_belt, itempart_ring2 },	//	itempart_pendant,
	{ itempart_horse, itempart_horse },	//	itempart_horse,
};

int KItemList::ms_ActivedEquip[itempart_num][MAX_ITEM_ACTIVE] =
{
	{ itempart_body, itempart_amulet },	//	itempart_head = 0,
	{ itempart_ring2, itempart_belt },	//	itempart_body,
	{ itempart_pendant, itempart_cuff },//	itempart_belt,
	{ itempart_amulet, itempart_body },	//	itempart_weapon,
	{ itempart_weapon, itempart_head },	//	itempart_foot,
	{ itempart_foot, itempart_ring1 },	//	itempart_cuff,
	{ itempart_belt, itempart_ring2 },	//	itempart_amulet,
	{ itempart_weapon, itempart_head },	//	itempart_ring1,
	{ itempart_cuff, itempart_pendant },//	itempart_ring2,
	{ itempart_foot, itempart_ring1 },	//	itempart_pendant,
	{ itempart_horse, itempart_horse },	//	itempart_horse,
};

KItemList::KItemList()
{
	m_PlayerIdx = 0;
	m_nListCurIdx = 0;
}

KItemList::~KItemList()
{

}


/*!*****************************************************************************
// Function		: KItemList::GetWeaponType
// Purpose		: ȡ�����װ������������
// Return		: int 
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemList::GetWeaponType()
{
	if (m_EquipItem[itempart_weapon])
		return Item[m_EquipItem[itempart_weapon]].GetDetailType();
	else
		return -1;
}

void KItemList::GetWeaponDamage(int* nMin, int* nMax)
{
	int nWeaponIdx = m_EquipItem[itempart_weapon];
	if (nWeaponIdx)
	{
		_ASSERT(Item[nWeaponIdx].m_CommonAttrib.nItemGenre == item_equip 
			&& (Item[nWeaponIdx].m_CommonAttrib.nDetailType == equip_meleeweapon
			|| Item[nWeaponIdx].m_CommonAttrib.nDetailType == equip_rangeweapon));
		int nMinDamage, nMaxDamage, nEnhance;
		int nDamageMinBase = Item[nWeaponIdx].m_aryBaseAttrib[0].nValue[0];
		int	nDamageMaxBase = Item[nWeaponIdx].m_aryBaseAttrib[1].nValue[0];
		nMinDamage = 0;
		nMaxDamage = 0;
		nEnhance = 0;
		for (int i = 0; i < 6; i++)
		{
			switch(Item[nWeaponIdx].m_aryMagicAttrib[i].nAttribType)
			{
			case magic_weapondamagemin_v:
				nMinDamage += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			case magic_weapondamagemax_v:
				nMaxDamage += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			case magic_weapondamageenhance_p:
				nEnhance += Item[nWeaponIdx].m_aryMagicAttrib[i].nValue[0];
				break;
			default:
				break;
			}
		}
		*nMin = (nDamageMinBase + nMinDamage) * (100 + nEnhance) / 100;
		*nMax = (nDamageMaxBase + nMaxDamage) * (100 + nEnhance) / 100;
	}
	else	// ����
	{
		/*
		int nDamageBase = Player[m_PlayerIdx].m_nCurStrength * Player[m_PlayerIdx].m_nCurDexterity;
		*nMin = nDamageBase >> 9;
		*nMax = nDamageBase >> 8;
		*/
		// ��ֵ���㷽���޸ģ���ӢҪ�� by Spe 03/06/11
		_ASSERT(STRENGTH_SET_DAMAGE_VALUE > 0);
		*nMin = Player[m_PlayerIdx].m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
		*nMax = Player[m_PlayerIdx].m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
	}
}
/*!*****************************************************************************
// Function		: KItemList::Add
// Purpose		: ��ҵõ�һ��װ��
// Return		: int 
// Argumant		: int nIdx		Item�����idx
// Argumant		: int nPlace
// Argumant		: int nX
// Argumant		: int nY
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemList::Add(int nIdx, int nPlace, int nX, int nY, BOOL bInit)
{
	if (nIdx <= 0)
		return 0;

	if (Item[nIdx].GetID() == 0)
		return 0;

	int i = FindFree();
	if (!i)
		return 0;

	switch(nPlace)
	{
	case pos_hand:
		if (m_Hand)
			return 0;
		m_Items[i].nPlace = pos_hand;
		m_Items[i].nX = 0;
		m_Items[i].nY = 0;
		m_Hand = nIdx;
		break;
	case pos_equip:
		if (nX < 0 || nX >= itempart_num)
			return 0;
		if (m_EquipItem[nX])
			return 0;
//	�п���������������������װ�����ϵġ��Ƿ���װ��Ӧ�÷ŵ�Equipʱ���
//		if (!CanEquip(nIdx, nX))
//			return 0;
		m_Items[i].nPlace = pos_equip;
		m_Items[i].nX = nX;
		m_Items[i].nY = 0;
		break;
	case pos_equiproom:
		if (!m_Room[room_equipment].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_equiproom;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;
		break;
#ifndef _SERVER
	case pos_trade1:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			return 0;
		if (!m_Room[room_trade1].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_trade1;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;
		break;
#endif
	case pos_repositoryroom:
		if (!m_Room[room_repository].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_repositoryroom;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_exbox1room: // ruong mo rong 1
		if (!m_Room[room_exbox1].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_exbox1room;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_exbox2room: // ruong mo rong 2
		if (!m_Room[room_exbox2].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_exbox2room;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_exbox3room: // ruong mo rong 3
		if (!m_Room[room_exbox3].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_exbox3room;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	case pos_equiproomex:  // mo rong hanh trang
		if (!m_Room[room_equipmentex].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_equiproomex;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;
		break;
	case pos_immediacy:
		if (!m_Room[room_immediacy].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_immediacy;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;		
	case pos_give:
		if (!m_Room[pos_give].PlaceItem(nX, nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
			return 0;
		m_Items[i].nPlace = pos_give;
		m_Items[i].nX = nX;
		m_Items[i].nY = nY;		
		break;
	default:
		return 0;
	}

	m_Items[i].nIdx = nIdx;
	m_Items[i].nPrice = 0;
	m_FreeIdx.Remove(i);
	m_UseIdx.Insert(i);

	if (m_Items[i].nPlace == pos_equip)
	{
		Equip(m_Items[i].nIdx, nX);
	}
#ifdef _SERVER
	ITEM_SYNC	sItem;
	sItem.ProtocolType = s2c_syncitem;
	sItem.m_Point = Item[nIdx].IsPurple();
	sItem.m_EnChance = Item[nIdx].GetEnChance();
	sItem.m_GoldId = Item[nIdx].GetGoldId();
	sItem.m_StackNum = Item[nIdx].GetStackNum();
	sItem.m_ID = Item[nIdx].GetID();
	sItem.m_Genre = Item[nIdx].GetGenre();
	sItem.m_Detail = Item[nIdx].GetDetailType();
	sItem.m_Particur = Item[nIdx].GetParticular();
	sItem.m_Series = Item[nIdx].GetSeries();
	sItem.m_Level = Item[nIdx].GetLevel();
	sItem.m_Luck = Item[nIdx].m_GeneratorParam.nLuck;
	sItem.m_btPlace = m_Items[i].nPlace;
	sItem.m_btX = m_Items[i].nX;
	sItem.m_btY = m_Items[i].nY;
	sItem.m_Time.bYear = Item[nIdx].GetTime()->bYear;
	sItem.m_Time.bMonth = Item[nIdx].GetTime()->bMonth;
	sItem.m_Time.bDay = Item[nIdx].GetTime()->bDay;
	sItem.m_Time.bHour = Item[nIdx].GetTime()->bHour;
	for (int j = 0; j < 6; j++)
		sItem.m_MagicLevel[j] = Item[nIdx].m_GeneratorParam.nGeneratorLevel[j];
	sItem.m_RandomSeed = Item[nIdx].m_GeneratorParam.uRandomSeed;
	sItem.m_Version = Item[nIdx].m_GeneratorParam.nVersion;
	sItem.m_Durability = Item[nIdx].GetDurability();
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sItem, sizeof(ITEM_SYNC));
	if (!bInit)
		Player[m_PlayerIdx].m_uMustSave = SAVE_REQUEST;
#endif

#ifndef _SERVER
	KUiObjAtContRegion	pInfo;

	int PartConvert[itempart_num] = 
	{
		UIEP_HEAD,
		UIEP_BODY,
		UIEP_WAIST,
		UIEP_HAND,
		UIEP_FOOT,
		UIEP_FINESSE,
		UIEP_NECK,
		UIEP_FINGER1,
		UIEP_FINGER2,
		UIEP_WAIST_DECOR,
		UIEP_HORSE,
		UIEP_MASK,	// mat na
	};

	pInfo.Obj.uGenre = CGOG_ITEM;	//Դװ��
	pInfo.Obj.uId = nIdx;
	pInfo.Region.Width = Item[nIdx].GetWidth();
	pInfo.Region.Height = Item[nIdx].GetHeight();

	switch(nPlace)
	{
	case pos_immediacy:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_IMMEDIA_ITEM;
		break;
	case pos_hand:
		pInfo.eContainer = UOC_IN_HAND;
		break;
	case pos_equip:
		pInfo.Region.h = 0;
		pInfo.Region.v = PartConvert[nX];
		pInfo.eContainer = UOC_EQUIPTMENT;
		break;
	case pos_equiproom:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_ITEM_TAKE_WITH;
		break;
	case pos_repositoryroom:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_STORE_BOX;
		break;
	case pos_exbox1room: // ruong mo rong 1
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_EX_BOX1;
		break;
	case pos_exbox2room: // ruong mo rong 2
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_EX_BOX2;
		break;
	case pos_exbox3room: // ruong mo rong 3
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_EX_BOX3;
		break;
	case pos_equiproomex: // mo rong hanh trang
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		pInfo.eContainer = UOC_ITEM_EX;
		break;
	case pos_trade1:
		pInfo.Region.h = nX;
		pInfo.Region.v = nY;
		break;
	}
	if (nPlace != pos_trade1)
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo, 1);
	else
	{
		CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (DWORD)&pInfo, 1);
	}

#ifdef _DEBUG
	int nLoopIdx = 0;
	nLoopIdx = m_UseIdx.GetNext(nLoopIdx);
	g_DebugLog("[ITEM]Item Begin");
	while(nLoopIdx)
	{
		g_DebugLog("[ITEM]ItemListIdx:%d, Item:%d, ItemId:%d", nLoopIdx, m_Items[nLoopIdx].nIdx, Item[m_Items[nLoopIdx].nIdx].GetID());
		nLoopIdx = m_UseIdx.GetNext(nLoopIdx);
	}
#endif
#endif
	return i;
}

/*!*****************************************************************************
// Function		: KItemList::Remove
// Purpose		: ���ʧȥһ��װ��
// Return		: int 
// Argumant		: int nGameIdxΪ��Ϸ�����е�������ı��
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::Remove(int nGameIdx)
{
	if (!nGameIdx)
		return FALSE;

	int nIdx = FindSame(nGameIdx);

	if (!nIdx)
		return FALSE;

#ifdef _SERVER
	ITEM_REMOVE_SYNC	sRemove;
	sRemove.ProtocolType = s2c_removeitem;
	sRemove.m_ID = Item[nGameIdx].m_dwID;
#endif

	switch(m_Items[nIdx].nPlace)
	{
	case pos_hand:
		m_Hand = 0;
		break;
	case pos_equip:
		UnEquip(m_Items[nIdx].nIdx);
		break;
	case pos_immediacy:
		m_Room[room_immediacy].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_equiproom:
		m_Room[room_equipment].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_repositoryroom:
		m_Room[room_repository].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_exbox1room: // ruong mo rong 1
		m_Room[room_exbox1].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_exbox2room: // ruong mo rong 2
		m_Room[room_exbox2].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_exbox3room: // ruong mo rong 3
		m_Room[room_exbox3].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_equiproomex: // mo rong hanh trang
		m_Room[room_equipmentex].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_traderoom:
		m_Room[room_trade].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
	case pos_give:
		m_Room[room_give].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
#ifndef _SERVER
	case pos_trade1:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
		{
			_ASSERT(0);
		}
		m_Room[room_trade1].PickUpItem(
			nGameIdx,
			m_Items[nIdx].nX,
			m_Items[nIdx].nY,
			Item[m_Items[nIdx].nIdx].GetWidth(),
			Item[m_Items[nIdx].nIdx].GetHeight());
		break;
#endif
	default:
		return FALSE;
	}
#ifndef _SERVER
	// �ͻ��˴��������ȥ��װ����Ӧ�ô�װ������ȥ������
	ItemSet.Remove(m_Items[nIdx].nIdx);

	// ���洦��
	KUiObjAtContRegion pInfo;

	int PartConvert[itempart_num] = 
	{
		UIEP_HEAD,
		UIEP_BODY,
		UIEP_WAIST,
		UIEP_HAND,
		UIEP_FOOT,
		UIEP_FINESSE,
		UIEP_NECK,
		UIEP_FINGER1,
		UIEP_FINGER2,
		UIEP_WAIST_DECOR,
		UIEP_HORSE,
		UIEP_MASK,	// mat na
	};

	pInfo.Obj.uGenre = CGOG_ITEM;
	pInfo.Obj.uId = m_Items[nIdx].nIdx;
	pInfo.Region.Width = Item[m_Items[nIdx].nIdx].GetWidth();
	pInfo.Region.Height = Item[m_Items[nIdx].nIdx].GetHeight();

	switch(m_Items[nIdx].nPlace)
	{
	case pos_hand:
		pInfo.Obj.uGenre = CGOG_NOTHING;
		pInfo.Region.h = 0;
		pInfo.Region.v = 0;
		pInfo.eContainer = UOC_IN_HAND;
		break;
	case pos_equiproom:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_ITEM_TAKE_WITH;
		break;
	case pos_repositoryroom:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_STORE_BOX;
		break;
	case pos_exbox1room: // ruong mo rong 1
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_EX_BOX1;
		break;
	case pos_exbox2room: // ruong mo rong 2
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_EX_BOX2;
		break;
	case pos_exbox3room: // ruong mo rong 3
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_EX_BOX3;
		break;
	case pos_equiproomex: // mo rong hanh trang
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_ITEM_EX;
		break;
	case pos_immediacy:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		pInfo.eContainer = UOC_IMMEDIA_ITEM;
		break;
	case pos_equip:
		pInfo.Region.h = 0;
		pInfo.Region.v = PartConvert[m_Items[nIdx].nX];
		pInfo.eContainer = UOC_EQUIPTMENT;
		break;
	case pos_trade1:
		pInfo.Region.h = m_Items[nIdx].nX;
		pInfo.Region.v = m_Items[nIdx].nY;
		break;
	}
	if (m_Items[nIdx].nPlace != pos_trade1)
	{
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo, 0);
	}
	else
	{
		CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (DWORD)&pInfo, 0);
	}
#endif
	m_Items[nIdx].nIdx = 0;
	m_Items[nIdx].nPlace = 0;
	m_Items[nIdx].nX = 0;
	m_Items[nIdx].nY = 0;
	m_Items[nIdx].nPrice = 0;
	m_FreeIdx.Insert(nIdx);
	m_UseIdx.Remove(nIdx);

#ifdef _SERVER
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));
	Player[m_PlayerIdx].m_uMustSave = SAVE_REQUEST;
#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KItemList::FindFree
// Purpose		: ���ҿ��ÿ�����
// Return		: int 
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemList::FindFree()
{
	return m_FreeIdx.GetNext(0);
}

/*!*****************************************************************************
// Function		: KItemList::FindSame
// Purpose		: 
// Return		: int 
// Argumant		: int nGameIdx
// Comments		:
// Author		: Spe
*****************************************************************************/
int KItemList::FindSame(int nGameIdx)
{
	int nIdx = 0;
	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;
		
		if (m_Items[nIdx].nIdx == nGameIdx)
			return nIdx;
	}
	return 0;
}
/*!*****************************************************************************
// Function		: KItemList::Init
// Purpose		: ��ʼ�����װ���б�
// Return		: BOOL
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::Init(int nPlayerIdx)
{
	m_PlayerIdx = nPlayerIdx;
	m_Hand = 0;
	m_nBackHand = 0;
	m_nMaskLock = FALSE;	// mat na
	// TODO: Maybe we can read size from ini file.
//	int nWidth = 6;
//	int nHeight = 10;
//	m_Room[room_equipment].Init(nWidth, nHeight);
//	m_Room[room_repository].Init(nWidth, nHeight);
//	m_Room[room_trade].Init(nWidth, nHeight);
//	m_Room[room_tradeback].Init(nWidth, nHeight);
	m_Room[room_equipment].Init(EQUIPMENT_ROOM_WIDTH, EQUIPMENT_ROOM_HEIGHT);
	m_Room[room_repository].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT);
	m_Room[room_trade].Init(TRADE_ROOM_WIDTH, TRADE_ROOM_HEIGHT);
	m_Room[room_exbox1].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT); // ruong mo rong 1
	m_Room[room_exbox2].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT); // ruong mo rong 2
	m_Room[room_exbox3].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT); // ruong mo rong 3
	m_Room[room_equipmentex].Init(REPOSITORY_ROOM_WIDTH, REPOSITORY_ROOM_HEIGHT); // ruong hanh trang
	m_Room[room_tradeback].Init(EQUIPMENT_ROOM_WIDTH, EQUIPMENT_ROOM_HEIGHT);
	m_Room[room_immediacy].Init(IMMEDIACY_ROOM_WIDTH, IMMEDIACY_ROOM_HEIGHT);
	m_Room[room_give].Init(EQUIPMENT_ROOM_WIDTH, TRADE_ROOM_HEIGHT);
	m_Room[room_giveback].Init(EQUIPMENT_ROOM_WIDTH, EQUIPMENT_ROOM_HEIGHT);
#ifndef _SERVER
	m_Room[room_trade1].Init(TRADE_ROOM_WIDTH, TRADE_ROOM_HEIGHT);		// ����Ĵ�С������ room_trade �Ĵ�Сһ��
#endif
	ZeroMemory(m_EquipItem, sizeof(m_EquipItem));				// ���װ���ĵ��ߣ���Ӧ��Ϸ�����е��������������
	ZeroMemory(m_Items, sizeof(m_Items));						// ���ӵ�е����е��ߣ�����װ���ŵĺ�������ŵģ���Ӧ��Ϸ�����е��������������
	m_nListCurIdx = 0;											// ���� GetFirstItem �� GetNextItem
	
	m_FreeIdx.Init(MAX_PLAYER_ITEM);
	m_UseIdx.Init(MAX_PLAYER_ITEM);

	for (int i = MAX_PLAYER_ITEM - 1; i > 0 ; i--)
	{
		m_FreeIdx.Insert(i);
	}
	return TRUE;
}

/*!*****************************************************************************
// Function		: KItemList::CanEquip
// Purpose		: 
// Return		: BOOL 
// Argumant		: int nIdx
// Argumant		: int nPlace
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::CanEquip(int nIdx, int nPlace)
{
	if (m_PlayerIdx <= 0 || nIdx <= 0 || nIdx >= MAX_ITEM || Item[nIdx].GetGenre() != item_equip)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	KMagicAttrib* pData = NULL;

	if (nPlace != -1 && !Fit(nIdx, nPlace))
	{
		return FALSE;
	}

	int nCount = 0;
	do
	{
		pData = (KMagicAttrib*)Item[nIdx].GetRequirement(nCount);
		if (pData && !EnoughAttrib(pData))
			return FALSE;
		nCount++;
	} while (pData != NULL);
	return TRUE;
}

BOOL KItemList::CanEquip(KItem* pItem, int nPlace /* = -1 */)
{
	if (m_PlayerIdx <= 0 || !pItem)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	KMagicAttrib* pData = NULL;

	if (nPlace != -1 && !Fit(pItem, nPlace))
	{
		return FALSE;
	}

	int nCount = 0;
	do
	{
		pData = (KMagicAttrib*)pItem->GetRequirement(nCount);
		if (pData && !EnoughAttrib(pData))
			return FALSE;
		nCount++;
	} while (pData != NULL);
	return TRUE;
}

BOOL KItemList::EnoughAttrib(void* pAttrib)
{
	KMagicAttrib*	pData = (KMagicAttrib *)pAttrib;
	_ASSERT(pData);
	switch(pData->nAttribType)
	{
	case magic_requirestr:
		if (Player[m_PlayerIdx].m_nCurStrength < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiredex:
		if (Player[m_PlayerIdx].m_nCurDexterity < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requirevit:
		if (Player[m_PlayerIdx].m_nCurVitality < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requireeng:
		if (Player[m_PlayerIdx].m_nCurEngergy < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requirelevel:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_Level < pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiremenpai:
		if (Player[m_PlayerIdx].m_cFaction.m_nFirstAddFaction != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requireseries:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_Series != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	case magic_requiresex:
		if (Npc[Player[m_PlayerIdx].m_nIndex].m_nSex != pData->nValue[0])
		{
			return FALSE;
		}
		break;
	default:
		break;
	}
	return TRUE;
}
/*!*****************************************************************************
// Function		: KItemList::Equip
// Purpose		: 
// Return		: BOOL
// Argumant		: int nIdx����Ϸ�����еĵ��������ţ�Ҫ��һ����װ�������
// Argumant		: int nPlace������װ����λ�ã�-1�Զ���λ��
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::Equip(int nIdx, int nPlace /* = -1 */)
{
	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	if (m_PlayerIdx <= 0 || nIdx <= 0 || nNpcIdx <= 0 || item_equip != Item[nIdx].GetGenre())
		return FALSE;

	int nItemListIdx = FindSame(nIdx);
	if (!nItemListIdx)
	{
		_ASSERT(0);
		return FALSE;
	}

	int nEquipPlace = nPlace;
	if (-1 == nEquipPlace)
	{
		nEquipPlace = GetEquipPlace(Item[nIdx].GetDetailType());
	}
	else if (!Fit(nIdx, nEquipPlace))
	{
		return FALSE;
	}

	int nType = 0;
	// ��װ
	switch(nEquipPlace)
	{
	case itempart_head:
		if (Item[nIdx].GetGoldId())
		{
			nType = g_ItemChangeRes.GetGoldItemRes(Item[nIdx].GetGoldId());
		}

		if (nType)
			Npc[nNpcIdx].m_HelmType = nType;
		else
			Npc[nNpcIdx].m_HelmType = g_ItemChangeRes.GetHelmRes(Item[nIdx].GetParticular(), Item[nIdx].GetLevel());
		break;
	case itempart_body:
		if (Item[nIdx].GetGoldId())
		{
			nType = g_ItemChangeRes.GetGoldItemRes(Item[nIdx].GetGoldId());
		}

		if (nType)
			Npc[nNpcIdx].m_ArmorType = nType;
		else
			Npc[nNpcIdx].m_ArmorType = g_ItemChangeRes.GetArmorRes(Item[nIdx].GetParticular(), Item[nIdx].GetLevel());
		break;
	case itempart_weapon:
		if (Item[nIdx].GetGoldId())
		{
			nType = g_ItemChangeRes.GetGoldItemRes(Item[nIdx].GetGoldId());
		}

		if (nType)
			Npc[nNpcIdx].m_WeaponType = nType;
		else
			Npc[nNpcIdx].m_WeaponType = g_ItemChangeRes.GetWeaponRes(Item[nIdx].GetDetailType(), Item[nIdx].GetParticular(), Item[nIdx].GetLevel());
		break;
	case itempart_horse:
		if (Item[nIdx].GetGoldId())
		{
			nType = g_ItemChangeRes.GetGoldItemRes(Item[nIdx].GetGoldId());
		}

		if (nType)
			Npc[nNpcIdx].m_HorseType = nType;
		else
			Npc[nNpcIdx].m_HorseType = g_ItemChangeRes.GetHorseRes(Item[nIdx].GetParticular(), Item[nIdx].GetLevel());

		if (Npc[nNpcIdx].m_HorseType >= 0)
		{
			Npc[nNpcIdx].m_bRideHorse = TRUE;
		}
		else
		{
			Npc[nNpcIdx].m_bRideHorse = FALSE;
		}
		break;
	case itempart_mask:	// mat na
		if (!m_nMaskLock)
			Npc[nNpcIdx].m_MaskType = Item[nIdx].GetBaseMagic();
		break;
	default:
		break;
	}
	// ����װ����������
	m_EquipItem[nEquipPlace] = nIdx;
	m_Items[nItemListIdx].nPlace = pos_equip;
	m_Items[nItemListIdx].nX = nEquipPlace;
	m_Items[nItemListIdx].nY = 0;

//#ifdef _SERVER	because client need to show to menu
	int nActive = GetEquipEnhance(nEquipPlace);
	Item[nIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], nActive);
	InfectionNextEquip(nEquipPlace, TRUE);

	if (itempart_weapon == nEquipPlace)
	{		
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].UpdateWeaponSkill();
#endif
		Player[m_PlayerIdx].SetNpcDamageAttrib();
	}
	
	GetIfActive();
	Player[m_PlayerIdx].UpdataCurData();

//#endif
	return TRUE;
}

/*!*****************************************************************************
// Function		: KItemList::InfectionNextEquip
// Purpose		: װ��Ӱ������װ��
// Return		: void 
// Argumant		: int nEquipPlace
// Argumant		: BOOL bEquipUp ��װ�ϣ�TRUE������ж�£�FALSE��
// Comments		:
// Author		: Spe
*****************************************************************************/
void KItemList::InfectionNextEquip(int nEquipPlace, BOOL bEquip/* = FALSE */)
{
	if (m_PlayerIdx <= 0)
		return;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	for (int i = 0; i < MAX_ITEM_ACTIVE; i++)
	{
		int nInfectionEquip = ms_ActiveEquip[nEquipPlace][i];
		if (!m_EquipItem[nInfectionEquip])
			continue;
		if (g_IsAccrue(Item[m_EquipItem[nEquipPlace]].GetSeries(),
			Item[m_EquipItem[nInfectionEquip]].GetSeries()))
		{
			int nActive = GetEquipEnhance(nInfectionEquip);
			_ASSERT(nActive > 0);
			if (bEquip)
				Item[m_EquipItem[nInfectionEquip]].ApplyHiddenMagicAttribToNPC(&Npc[nNpcIdx], nActive);
			else
				Item[m_EquipItem[nInfectionEquip]].RemoveHiddenMagicAttribFromNPC(&Npc[nNpcIdx], nActive);
		}
	}
}
/*!*****************************************************************************
// Function		: KItemList::UnEquip
// Purpose		: �Ƴ�װ��
// Return		: BOOL 
// Argumant		: int nIdx ��Ϸ�����еĵ�����������
// Comments		: 
// Author		: Spe
*****************************************************************************/
BOOL KItemList::UnEquip(int nIdx, int nPos/* = -1*/)
{
	int i = 0;
	if (m_PlayerIdx <= 0)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	if (nIdx <= 0)
		return FALSE;

	if (nPos <= 0)
	{
		for (i = 0; i < itempart_num; i++)
		{
			if (m_EquipItem[i] == nIdx)
			{
				break;
			}
		}
		// û�з������������װ��
		if (i == itempart_num)
			return FALSE;

	}
	else
	{
		if (m_EquipItem[nPos] != nIdx)	// ��������
			return FALSE;
		i = nPos;
	}
	// �Ƴ���װ����NPC�����Ե���
	int nActive = GetEquipEnhance(i);
	if (m_bActiveSet)
		nActive = 3;
	Item[nIdx].RemoveMagicAttribFromNPC(&Npc[nNpcIdx], nActive);
	// �Ƴ���װ�����ܶ��������װ����Ӱ�졣
	InfectionNextEquip(i, FALSE);
	// ��仰һ��Ҫ������һ��󣬱�֤�����װ�������װ���������Ը����������ȷ��
	m_EquipItem[i] = 0;
	// ��װ
	switch(i)
	{
	case itempart_head:
		Npc[nNpcIdx].m_HelmType = g_ItemChangeRes.GetHelmRes(0, 0);
		break;
	case itempart_body:
		Npc[nNpcIdx].m_ArmorType = g_ItemChangeRes.GetArmorRes(0, 0);
		break;
	case itempart_weapon:
		Npc[nNpcIdx].m_WeaponType = g_ItemChangeRes.GetWeaponRes(0, 0, 0);
		break;
	case itempart_horse:
		Npc[nNpcIdx].m_HorseType = -1;
		Npc[nNpcIdx].m_bRideHorse = FALSE;
		break;
	case itempart_mask:	// mat na
		if (!m_nMaskLock)
			Npc[nNpcIdx].m_MaskType = 0;
		break;
	default:
		break;
	}

	if (itempart_weapon == i)
	{
#ifndef _SERVER
		Player[CLIENT_PLAYER_INDEX].UpdateWeaponSkill();
#endif
		Player[m_PlayerIdx].SetNpcDamageAttrib();
	}	

	if (!GetIfActive())
			Player[m_PlayerIdx].UpdataCurData();
#ifdef _SERVER
			for (i = 1;i < MAX_NPCSKILL;i++)
			{
				if (Npc[nNpcIdx].m_SkillList.GetCurrentLevel(Npc[nNpcIdx].m_SkillList.GetSkillId(i)) > 0)
				{
					// �ѵ�ǰ�˼��ܵĵ�����ʣ�༼�ܵ㷢���ͻ���
					PLAYER_SKILL_LEVEL_SYNC	sSkill;
					sSkill.ProtocolType = s2c_playerskilllevel;
					sSkill.m_nSkillID = Npc[nNpcIdx].m_SkillList.GetSkillId(i);
					sSkill.m_nSkillLevel = Npc[nNpcIdx].m_SkillList.GetCurrentLevel(Npc[nNpcIdx].m_SkillList.GetSkillId(i));
					sSkill.m_nLeavePoint = Player[m_PlayerIdx].m_nSkillPoint;
					sSkill.m_nAddPoint = Npc[nNpcIdx].m_SkillList.GetAddPoint(i);
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
						
				}
			}
#endif


	return TRUE;
}

int KItemList::GetEquipPlace(int nType)
{
	int nRet = -1;
	switch(nType)
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		nRet = itempart_weapon;
		break;
	case equip_armor:
		nRet = itempart_body;
		break;
	case equip_helm:
		nRet = itempart_head;
		break;
	case equip_boots:
		nRet = itempart_foot;
		break;
	case equip_ring:
		nRet = itempart_ring1;
		break;
	case equip_amulet:
		nRet = itempart_amulet;
		break;
	case equip_belt:
		nRet = itempart_belt;
		break;
	case equip_cuff:
		nRet = itempart_cuff;
		break;
	case equip_pendant:
		nRet = itempart_pendant;
		break;
	case equip_horse:
		nRet = itempart_horse;
		break;
	case equip_mask:	// mat na
		nRet = itempart_mask;
		break;
	default:
		break;
	}
	return nRet;
}

/*!*****************************************************************************
// Function		: KItemList::Fit
// Purpose		: 
// Return		: BOOL 
// Argumant		: int nIdx
// Argumant		: int nPlace
// Comments		:
// Author		: Spe
*****************************************************************************/
BOOL KItemList::Fit(int nIdx, int nPlace)
{
	BOOL	bRet = FALSE;
	_ASSERT(Item[nIdx].GetGenre() == item_equip);
	switch(Item[nIdx].GetDetailType())
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		if (nPlace == itempart_weapon)
			bRet = TRUE;
		break;
	case equip_armor:
		if (nPlace == itempart_body)
			bRet = TRUE;
		break;
	case equip_belt:
		if (nPlace == itempart_belt)
			bRet = TRUE;
		break;
	case equip_boots:
		if (nPlace == itempart_foot)
			bRet = TRUE;
		break;
	case equip_cuff:
		if (nPlace == itempart_cuff)
			bRet = TRUE;
		break;
	case equip_ring:
		if (nPlace == itempart_ring1 || nPlace == itempart_ring2)
			bRet = TRUE;
		break;
	case equip_amulet:
		if (nPlace == itempart_amulet)
			bRet = TRUE;
		break;
	case equip_pendant:
		if (nPlace == itempart_pendant)
			bRet = TRUE;
		break;
	case equip_helm:
		if (nPlace == itempart_head)
			bRet = TRUE;
		break;
	case equip_horse:
		if (nPlace == itempart_horse)
			bRet = TRUE;
		break;
	case equip_mask:	// mat na
		if (nPlace == itempart_mask)
			bRet = TRUE;
		break;
	}
	return bRet;
}

BOOL KItemList::Fit(KItem* pItem, int nPlace)
{
	BOOL	bRet = FALSE;
	_ASSERT(pItem->GetGenre() == item_equip);
	switch(pItem->GetDetailType())
	{
	case equip_meleeweapon:
	case equip_rangeweapon:
		if (nPlace == itempart_weapon)
			bRet = TRUE;
		break;
	case equip_armor:
		if (nPlace == itempart_body)
			bRet = TRUE;
		break;
	case equip_belt:
		if (nPlace == itempart_belt)
			bRet = TRUE;
		break;
	case equip_boots:
		if (nPlace == itempart_foot)
			bRet = TRUE;
		break;
	case equip_cuff:
		if (nPlace == itempart_cuff)
			bRet = TRUE;
		break;
	case equip_ring:
		if (nPlace == itempart_ring1 || nPlace == itempart_ring2)
			bRet = TRUE;
		break;
	case equip_amulet:
		if (nPlace == itempart_amulet)
			bRet = TRUE;
		break;
	case equip_pendant:
		if (nPlace == itempart_pendant)
			bRet = TRUE;
		break;
	case equip_helm:
		if (nPlace == itempart_head)
			bRet = TRUE;
		break;
	case equip_horse:
		if (nPlace == itempart_horse)
			bRet = TRUE;
		break;
	case equip_mask:	// mat na
		if (nPlace == itempart_mask)
			bRet = TRUE;
		break;
	}
	return bRet;
}

int KItemList::GetEquipEnhance(int nPlace)
{
	if (m_PlayerIdx <= 0)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	if (nPlace < 0 || nPlace >= itempart_num)
	{
		_ASSERT(0);
		return 0;
	}
	int nRet = 0;

	int nSeries = Item[m_EquipItem[nPlace]].GetSeries();
	if (g_IsAccrue(Npc[nNpcIdx].m_Series, nSeries))
		nRet ++;
	for (int i = 0; i < MAX_ITEM_ACTIVE; i++)
	{
		// �����������װ����װ�����Ƿ��������װ��
		int nActivedEquip = m_EquipItem[ms_ActivedEquip[nPlace][i]];
		if (nActivedEquip)
		{
			if (g_IsAccrue(Item[nActivedEquip].GetSeries(), nSeries))
				nRet++;
		}
	}
	return nRet;
}

BOOL KItemList::EatMecidine(int nIdx)
{
	if (m_PlayerIdx <= 0)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;
	int nGenre = Item[nIdx].GetGenre();

	_ASSERT(nGenre == item_medicine || nGenre == item_townportal);
	if (nGenre == item_medicine)
		Item[nIdx].ApplyMagicAttribToNPC(&Npc[nNpcIdx], 3);
	if (nGenre == item_townportal)
	{
		if (!Npc[Player[m_PlayerIdx].m_nIndex].m_FightMode)
		{
#ifndef _SERVER	// �ͻ����ȿ����ǲ��Ƿ�ս����������ǵĻ��ͱ𷢸�Server�ˡ�
			// ϵͳ��Ϣ
			KSystemMessage Msg;
			
			Msg.byConfirmType = SMCT_CLICK;
			Msg.byParamSize = 0;
			Msg.byPriority = 1;
			Msg.eType = SMT_PLAYER;
			strcpy(Msg.szMessage, "Kh�ng th� s� d�ng th� ��a ph� � ��y!");
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
#endif
			return FALSE;
		}
#ifdef _SERVER
		Player[m_PlayerIdx].UseTownPortal();
#endif
	}
	if (nGenre == item_task || nGenre == item_mine)
	{
#ifdef _SERVER
		if (Player[m_PlayerIdx].ExecuteScript(Item[nIdx].GetScript(),"main",""))
		{
			if (Item[nIdx].GetBigSet())
			{
				Remove(nIdx);
				ItemSet.Remove(nIdx);			
			}
		}
#endif
		return TRUE;
	}

	if (Item[nIdx].GetStackNum() > 1)
	{
		Item[nIdx].SetStackNum(Item[nIdx].GetStackNum() - 1);
	}
	else
	{
#ifdef _SERVER
		Remove(nIdx);
		ItemSet.Remove(nIdx);
#endif
	}

	if (Npc[Player[m_PlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[m_PlayerIdx].m_nIndex].SendCommand(do_stand);
	}
	return TRUE;
}

#ifndef _SERVER
int KItemList::UseItem(int nIdx)
{
	if (m_PlayerIdx <= 0)
		return FALSE;

	int nNpcIdx = Player[m_PlayerIdx].m_nIndex;

	if (0 == FindSame(nIdx))
	{
		return 0;
	}

	int		nRet = 0;
	switch(Item[nIdx].GetGenre())
	{
	case item_equip:
		break;
/*		if (Equip(nNpcIdx, nIdx))
			nRet = REQUEST_EQUIP_ITEM;*/
		break;
	case item_townportal:
	case item_medicine:
		if (EatMecidine(nIdx))
			nRet = REQUEST_EAT_MEDICINE;
		break;
	case item_task:
		if (EatMecidine(nIdx))
			nRet = REQUEST_EAT_MEDICINE;
		break;
	case item_mine:
		if (EatMecidine(nIdx))
			nRet = REQUEST_EAT_MEDICINE;
		break;
	default:
		break;
	}
	return nRet;
}
#endif

#ifndef _SERVER
BOOL KItemList::SearchPosition(int nWidth, int nHeight, ItemPos* pPos)
{
	if (nWidth <= 0 || nHeight <= 0 || NULL == pPos)
	{
		return FALSE;
	}

	POINT	pPt;
	if (!m_Room[room_equipment].FindRoom(nWidth, nHeight, &pPt))
	{

		if (0 != m_Hand)
		{
			return FALSE;
		}
		pPos->nPlace = pos_hand;
		pPos->nX = 0;
		pPos->nY = 0;
	}
	else
	{
		pPos->nPlace = pos_equiproom;
		pPos->nX = pPt.x;
		pPos->nY = pPt.y;
	}
	return TRUE;
}
#endif

int	KItemList::SearchID(int nID)
{
	if (m_PlayerIdx <= 0)
		return 0;
	int nIdx = 0;
	while(1)
	{
		nIdx = m_UseIdx.GetNext(nIdx);
		if (!nIdx)
			break;
		if (Item[m_Items[nIdx].nIdx].GetID() == (DWORD)nID)
			return m_Items[nIdx].nIdx;
	}
	return 0;
}

void KItemList::ExchangeMoney(int pos1, int pos2, int nMoney)
{
	if (pos1 < 0 || pos2 < 0 || pos1 > room_trade || pos2 > room_trade)
		return;
	if (pos1 == room_repository && !Player[m_PlayerIdx].m_CUnlocked)
	{
		return;
	}
	if (m_Room[pos1].AddMoney(-nMoney))		// Դλ�����ó���ô��Ǯ��
	{
		if (!m_Room[pos2].AddMoney(nMoney))	// Ŀ�ĵ��ܷŲ���ȥ
		{
			m_Room[pos1].AddMoney(nMoney);	// ��ԭԴλ�õ�Ǯ
		}
	}
	else
	{
		return;
	}

#ifndef _SERVER
	if (pos1 == room_equipment && pos2 == room_repository)
		SendClientCmdStoreMoney(0, nMoney);
	else if (pos1 == room_repository && pos2 == room_equipment)
		SendClientCmdStoreMoney(1, nMoney);
#endif
#ifdef _SERVER
	SendMoneySync();
#endif	
}

//----------------------------------------------------------------------
//	���ܣ��õ���Ʒ���ʹ��������Ǯ��
//----------------------------------------------------------------------
int KItemList::GetMoneyAmount()
{
	return (m_Room[room_equipment].GetMoney() + m_Room[room_repository].GetMoney());
}
int KItemList::GetXuAmount()
{
	return (m_Room[room_equipment].GetXu());
}
//----------------------------------------------------------------------
//	���ܣ��õ���Ʒ���ʹ������Ǯ��
//----------------------------------------------------------------------
int KItemList::GetEquipmentMoney()
{
	return m_Room[room_equipment].GetMoney();
}

int KItemList::GetEquipmentXu()
{
	return m_Room[room_equipment].GetXu();
}


BOOL KItemList::AddMoney(int nRoom, int nMoney)
{
	if (nRoom < 0 || nRoom >= room_num)
		return FALSE;

	if ( !m_Room[nRoom].AddMoney(nMoney) )
		return FALSE;

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

BOOL KItemList::AddXu(int nRoom, int nXu)
{
	if (nRoom < 0 || nRoom >= room_num)
		return FALSE;

	if ( !m_Room[nRoom].AddXu(nXu) )
		return FALSE;

#ifdef _SERVER
	SendXuSync();
#endif

	return TRUE;
}

BOOL KItemList::CostMoney(int nMoney)
{
	if (nMoney > GetEquipmentMoney())
		return FALSE;

	if ( !m_Room[room_equipment].AddMoney(-nMoney) )
		return FALSE;

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

BOOL KItemList::CostXu(int nXu)
{
	if (nXu > GetEquipmentXu())
		return FALSE;

	if ( !m_Room[room_equipment].AddXu(-nXu) )
		return FALSE;

#ifdef _SERVER
	SendXuSync();
#endif

	return TRUE;
}

BOOL KItemList::DecMoney(int nMoney)
{
	if (nMoney < 0)
		return FALSE;

	if (nMoney > m_Room[room_equipment].GetMoney())
	{
		nMoney -= m_Room[room_equipment].GetMoney();
		SetRoomMoney(room_equipment, 0);
		if (nMoney > m_Room[room_repository].GetMoney())
			SetRoomMoney(room_repository, 0);
		else
			AddMoney(room_repository, -nMoney);
	}
	else
	{
		AddMoney(room_equipment, -nMoney);
	}

#ifdef _SERVER
	SendMoneySync();
#endif

	return TRUE;
}

BOOL KItemList::DecXu(int nXu)
{
	if (nXu < 0)
		return FALSE;

	if (nXu > m_Room[room_equipment].GetXu())
	{
		nXu -= m_Room[room_equipment].GetXu();
		SetRoomXu(room_equipment, 0);
	}
	else
	{
		AddXu(room_equipment, -nXu);
	}

#ifdef _SERVER
	SendXuSync();
#endif

	return TRUE;
}

#ifdef _SERVER
//----------------------------------------------------------------------------------
//	���ܣ����ô˽ӿڱ��뱣֤�����nMoney��һ����Ч��(�����Ҳ���������Ǯ��)
//----------------------------------------------------------------------------------
void	KItemList::TradeMoveMoney(int nMoney)
{
	// �Լ�Ǯ�Ĵ���
	m_Room[room_trade].SetMoney(nMoney);
	SendMoneySync();

	// ���Է�����Ϣ
	TRADE_MONEY_SYNC	sMoney;
	sMoney.ProtocolType = s2c_trademoneysync;
	sMoney.m_nMoney = nMoney;
	g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sMoney, sizeof(TRADE_MONEY_SYNC));
}
#endif

#ifdef _SERVER
//----------------------------------------------------------------------------------
//	���ܣ���������moneyͬ����Ϣ���ͻ���
//----------------------------------------------------------------------------------
void	KItemList::SendMoneySync()
{
	PLAYER_MONEY_SYNC	sMoney;
	sMoney.ProtocolType = s2c_syncmoney;
	sMoney.m_nMoney1 = m_Room[room_equipment].GetMoney();
	sMoney.m_nMoney2 = m_Room[room_repository].GetMoney();
	sMoney.m_nMoney3 = m_Room[room_trade].GetMoney();
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMoney, sizeof(PLAYER_MONEY_SYNC));
}
void	KItemList::SendXuSync()
{
	PLAYER_XU_SYNC	sXu;
	sXu.ProtocolType = s2c_syncxu;
	sXu.m_nXu = m_Room[room_equipment].GetXu();
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sXu, sizeof(PLAYER_XU_SYNC));
}
#endif

void KItemList::SetMoney(int nMoney1, int nMoney2, int nMoney3)
{
	m_Room[room_equipment].SetMoney(nMoney1);
	m_Room[room_repository].SetMoney(nMoney2);
	m_Room[room_trade].SetMoney(nMoney3);
#ifndef _SERVER
	KUiObjAtContRegion	sMoney;
	sMoney.Obj.uGenre = CGOG_MONEY;
	sMoney.Obj.uId = nMoney2;
	sMoney.eContainer = UOC_STORE_BOX;
	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sMoney, 1);
#endif
}

void KItemList::SetXu(int nXu)
{
	m_Room[room_equipment].SetXu(nXu);
}

void KItemList::SetRoomMoney(int nRoom, int nMoney)
{
	if (nRoom >= 0 && nRoom < room_num)
		m_Room[nRoom].SetMoney(nMoney);
}

void KItemList::SetRoomXu(int nRoom, int nXu)
{
	if (nRoom >= 0 && nRoom < room_num)
		m_Room[nRoom].SetXu(nXu);
}

void KItemList::ExchangeItem(ItemPos* SrcPos, ItemPos* DesPos)
{
	if (SrcPos->nPlace != DesPos->nPlace)
		return;

	int nTempHand = m_Hand;
	int	nEquipIdx1 = 0;

#ifdef _SERVER
	PLAYER_MOVE_ITEM_SYNC	sMove;
	sMove.ProtocolType = s2c_playermoveitem;
	sMove.m_btDownPos = SrcPos->nPlace;
	sMove.m_btDownX = SrcPos->nX;
	sMove.m_btDownY = SrcPos->nY;
	sMove.m_btUpPos = DesPos->nPlace;
	sMove.m_btUpX = DesPos->nX;
	sMove.m_btUpY = DesPos->nY;
#endif

	// Դװ����SrcPos�����ϣ����ϵ�װ����DesPos
	switch(SrcPos->nPlace)
	{
	case pos_hand:
		g_DebugLog("%s exchange item error", Npc[Player[m_PlayerIdx].m_nIndex].Name);
		return;
		break;
	case pos_equip:
		if (Player[this->m_PlayerIdx].CheckTrading())	// ������ڽ���
			return;
		if (SrcPos->nX < 0 || SrcPos->nX >= itempart_num || DesPos->nX < 0 || DesPos->nX >= itempart_num)
			return;
		nEquipIdx1 = m_EquipItem[SrcPos->nX];
		if (nEquipIdx1)
		{
			UnEquip(nEquipIdx1, SrcPos->nX);
		}
		if (m_Hand)
		{
			if (CanEquip(m_Hand, DesPos->nX))
			{
				Equip(m_Hand, DesPos->nX);
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			else if (nEquipIdx1)
			{
				Equip(nEquipIdx1, SrcPos->nX);
			}
		}
		else
		{
			m_Hand = nEquipIdx1;
			m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
	case pos_immediacy:
		if (Player[m_PlayerIdx].CheckTrading())	// ������ڽ���
			return;
		// �ж��Ƿ���ͬ���͵���Ʒ���еĻ����÷���ȥ
		if (m_Hand)
		{
			if (m_Room[room_immediacy].CheckSameDetailType(Item[m_Hand].GetGenre(), Item[m_Hand].GetDetailType()))
			{
#ifdef _SERVER
				BYTE	byFinished = s2c_itemexchangefinish;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &byFinished, sizeof(BYTE));
#endif
#ifndef _SERVER
				KSystemMessage	sMsg;
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sprintf(sMsg.szMessage, MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
#endif
				return;
			}
		}
		nEquipIdx1 = m_Room[room_immediacy].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;
		
		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_immediacy].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}
		if (m_Hand)
			if (Item[m_Hand].CanStack(nEquipIdx1))
			{
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
				if (Item[nEquipIdx1].Stack(Item[m_Hand].GetStackNum()))
				{
					Item[m_Hand].Remove();
					Remove(m_Hand);
				}
				else
				{
					Item[m_Hand].SetStackNum(Item[nEquipIdx1].GetStackNum() - 50);
					Item[nEquipIdx1].SetStackNum(50);
				}
				m_Room[room_immediacy].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
				
				return;
			}
		// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
		if (m_Hand)
		{
			if (m_Room[room_immediacy].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight())
				&& (Item[m_Hand].GetGenre() == item_medicine || Item[m_Hand].GetGenre() == item_townportal || Item[m_Hand].GetGenre() == item_task))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_immediacy;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_immediacy].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
	case pos_repositoryroom:
		if (Player[m_PlayerIdx].CheckTrading())	// ������ڽ���
			return;
		nEquipIdx1 = m_Room[room_repository].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_repository].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}
		if (m_Hand)
			if (Item[m_Hand].CanStack(nEquipIdx1))
			{
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
				if (Item[nEquipIdx1].Stack(Item[m_Hand].GetStackNum()))
				{
					Item[m_Hand].Remove();
					Remove(m_Hand);
				}
				else
				{
					Item[m_Hand].SetStackNum(Item[nEquipIdx1].GetStackNum() - 50);
					Item[nEquipIdx1].SetStackNum(50);
				}
				m_Room[room_repository].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
				
				return;
			}
			

			// ǿ��
			if (Item[m_Hand].GetGenre() == item_task &&
				Item[m_Hand].GetDetailType() == 238)
			{		
				if (nEquipIdx1 && Item[nEquipIdx1].GetGenre() == item_equip && (Item[nEquipIdx1].GetGoldId() > 0 || Item[nEquipIdx1].IsPurple()))
				{
#ifdef _SERVER
					int nItemClass		= Item[nEquipIdx1].GetGenre();
					int nDetailType		= Item[nEquipIdx1].GetDetailType();
					int nParticularType	= Item[nEquipIdx1].GetParticular();
					int nLevel			= Item[nEquipIdx1].GetLevel();
					int nSeries			= Item[nEquipIdx1].GetSeries();
					int nLuck			= Item[nEquipIdx1].GetItemParam()->nLuck;
					int nEnChance		= Item[nEquipIdx1].GetEnChance();
					int nGoldId			= Item[nEquipIdx1].GetGoldId();	
					int nVersion		= Item[nEquipIdx1].GetItemParam()->nVersion;
					int	nGndrom			= Item[nEquipIdx1].GetItemParam()->uRandomSeed;
					int nPoint			= Item[nEquipIdx1].IsPurple();
						
					if (nEnChance >= 9)
					{
						return;
					}

					if (GetRandomNumber(1,10) < nEnChance + 1)
					{
						if (nEnChance >= 2)
						{
							nEnChance -= 2;
						}	
					}

					int nItemLevel[6];
					
					ZeroMemory(nItemLevel, sizeof(nItemLevel));
					
					for (int i = 0; i < 6; i ++)
						nItemLevel[i] = Item[nEquipIdx1].GetItemParam()->nGeneratorLevel[i];
					
					int	nIndex;

					if (nGoldId)
					{
						nIndex = ItemSet.Add(nGoldId,nItemLevel,nSeries,nEnChance + 1);
					} 
					else
					{
						nIndex = ItemSet.Add(nItemClass,nSeries,nLevel,nLuck,nDetailType,nParticularType,nItemLevel,
							nVersion,nGndrom,1,nEnChance + 1,nPoint);
					}
					
					
					if (nIndex <= 0)
					{
						return;
					}
					
					Remove(m_Hand);
					ItemSet.Remove(m_Hand);
					Remove(nEquipIdx1);
					ItemSet.Remove(nEquipIdx1);
					int		x, y;
					if (CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
					{
						Add(nIndex, pos_equiproom, x, y);
					}
					else
					{
						int		nIdx = Hand();
						if (nIdx)
						{
							Remove(nIdx);
							
							KMapPos sMapPos;
							KObjItemInfo	sInfo;
							
							Player[m_PlayerIdx].GetAboutPos(&sMapPos);
							
							sInfo.m_nItemID = nIdx;
							sInfo.m_nItemWidth = Item[nIdx].GetWidth();
							sInfo.m_nItemHeight = Item[nIdx].GetHeight();
							sInfo.m_nMoneyNum = 0;
							strcpy(sInfo.m_szName, Item[nIdx].GetName());
							sInfo.m_nColorID = 0;
							sInfo.m_nMovieFlag = 1;
							sInfo.m_nSoundFlag = 1;
							
							int nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), sMapPos, sInfo);
							if (nObj >= 0)
							{
								if (Item[nIdx].GetGenre() == item_task)
								{
									Object[nObj].SetEntireBelong(m_PlayerIdx);
								}
								else
								{
									Object[nObj].SetItemBelong(m_PlayerIdx);
								}
							}
						}
						Add(nIndex, pos_hand, 0 ,0);
					}
					return;
#endif
				}				
			}

			// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
			if (m_Hand)
			{
				if (m_Room[room_repository].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_repositoryroom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_repository].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
	//====================================================================================
	case pos_exbox1room: // ruong mo rong 1
		if (Player[m_PlayerIdx].CheckTrading())	// ������ڽ���
			return;
		nEquipIdx1 = m_Room[room_exbox1].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_exbox1].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}

		// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
		if (m_Hand)
		{
			if (m_Room[room_exbox1].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_exbox1room;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_exbox1].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
//---------------------------------------------------
	case pos_exbox2room: // ruong mo rong 2
		if (Player[m_PlayerIdx].CheckTrading())	// ������ڽ���
			return;
		nEquipIdx1 = m_Room[room_exbox2].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_exbox2].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}

		// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
		if (m_Hand)
		{
			if (m_Room[room_exbox2].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_exbox2room;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_exbox2].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
//----------------------------------------------------------------------
	case pos_exbox3room: // ruong mo rong 3
		if (Player[m_PlayerIdx].CheckTrading())	// ������ڽ���
			return;
		nEquipIdx1 = m_Room[room_exbox3].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_exbox3].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}

		// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
		if (m_Hand)
		{
			if (m_Room[room_exbox3].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_exbox3room;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_exbox3].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
//--------------------------------------------------------------------
	// add new
	case pos_equiproomex: // mo rong hanh trang
		nEquipIdx1 = m_Room[room_equipmentex].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_equipmentex].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}
		// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
		if (m_Hand)
		{
			if (m_Room[room_equipmentex].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_equiproomex;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_equipmentex].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}

#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
	//--------------------------------------------------------------------------
	case pos_equiproom:
		nEquipIdx1 = m_Room[room_equipment].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_equipment].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}
		if (m_Hand)
			if (Item[m_Hand].CanStack(nEquipIdx1))
			{
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
				if (Item[nEquipIdx1].Stack(Item[m_Hand].GetStackNum()))
				{
					Item[m_Hand].Remove();
					Remove(m_Hand);
				}
				else
				{
					Item[m_Hand].SetStackNum(Item[nEquipIdx1].GetStackNum() - 50);
					Item[nEquipIdx1].SetStackNum(50);
				}
				m_Room[room_equipment].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
				
				return;
			}

			// ǿ��
			if (Item[m_Hand].GetGenre() == item_task &&
				Item[m_Hand].GetDetailType() == 238)
			{		
				if (nEquipIdx1 && Item[nEquipIdx1].GetGenre() == item_equip && (Item[nEquipIdx1].GetGoldId() > 0 || Item[nEquipIdx1].IsPurple()))
				{
#ifdef _SERVER
					int nItemClass		= Item[nEquipIdx1].GetGenre();
					int nDetailType		= Item[nEquipIdx1].GetDetailType();
					int nParticularType	= Item[nEquipIdx1].GetParticular();
					int nLevel			= Item[nEquipIdx1].GetLevel();
					int nSeries			= Item[nEquipIdx1].GetSeries();
					int nLuck			= Item[nEquipIdx1].GetItemParam()->nLuck;
					int nEnChance		= Item[nEquipIdx1].GetEnChance();
					int nGoldId			= Item[nEquipIdx1].GetGoldId();	
					int nVersion		= Item[nEquipIdx1].GetItemParam()->nVersion;
					int	nGndrom			= Item[nEquipIdx1].GetItemParam()->uRandomSeed;
					int nPoint			= Item[nEquipIdx1].IsPurple();
					int nItemLevel[6];
					if (nEnChance >= 9)
					{
						return;
					}

					if (GetRandomNumber(1,10) < nEnChance + 1)
					{
						if (nEnChance >= 2)
						{
							nEnChance -= 2;
						}	
					}
					ZeroMemory(nItemLevel, sizeof(nItemLevel));
					
					for (int i = 0; i < 6; i ++)
						nItemLevel[i] = Item[nEquipIdx1].GetItemParam()->nGeneratorLevel[i];
					
					int	nIndex;
					
					if (nGoldId)
					{
						nIndex = ItemSet.Add(nGoldId,nItemLevel,nSeries,nEnChance + 1);
					} 
					else
					{
						nIndex = ItemSet.Add(nItemClass,nSeries,nLevel,nLuck,nDetailType,nParticularType,nItemLevel,
							nVersion,nGndrom,1,nEnChance + 1,nPoint);
					}
					
					
					if (nIndex <= 0)
					{
						return;
					}
					
					Remove(m_Hand);
					ItemSet.Remove(m_Hand);
					Remove(nEquipIdx1);
					ItemSet.Remove(nEquipIdx1);
					int		x, y;
					if (CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
					{
						Add(nIndex, pos_equiproom, x, y);
					}
					else
					{
						int		nIdx = Hand();
						if (nIdx)
						{
							Remove(nIdx);
							
							KMapPos sMapPos;
							KObjItemInfo	sInfo;
							
							Player[m_PlayerIdx].GetAboutPos(&sMapPos);
							
							sInfo.m_nItemID = nIdx;
							sInfo.m_nItemWidth = Item[nIdx].GetWidth();
							sInfo.m_nItemHeight = Item[nIdx].GetHeight();
							sInfo.m_nMoneyNum = 0;
							strcpy(sInfo.m_szName, Item[nIdx].GetName());
							sInfo.m_nColorID = 0;
							sInfo.m_nMovieFlag = 1;
							sInfo.m_nSoundFlag = 1;
							
							int nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), sMapPos, sInfo);
							if (nObj >= 0)
							{
								if (Item[nIdx].GetGenre() == item_task)
								{
									Object[nObj].SetEntireBelong(m_PlayerIdx);
								}
								else
								{
									Object[nObj].SetItemBelong(m_PlayerIdx);
								}
							}
						}
						Add(nIndex, pos_hand, 0 ,0);
					}
					return;
#endif
				}				
			}

		// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
		if (m_Hand)
		{
			if (m_Room[room_equipment].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_equiproom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_equipment].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}

#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
	case pos_traderoom:
		if ( !Player[m_PlayerIdx].CheckTrading() )	// ���ڽ���
			return;
#ifdef _SERVER
		if (m_Hand && Item[m_Hand].GetGenre() == item_task)
		{
			// ֪ͨ���棬������ߣ����ܽ���
			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_TRADE_TASK_ITEM;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);

			return;
		}
#endif
		nEquipIdx1 = m_Room[room_trade].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_trade].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}
		// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
		if (m_Hand)
		{
			if (m_Room[room_trade].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_traderoom;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
#ifdef _SERVER
				// ����Ϣ�����׶Է�
				if (nEquipIdx1)	// �������ж���������
				{
					ITEM_REMOVE_SYNC	sRemove;
					sRemove.ProtocolType = s2c_removeitem;
					sRemove.m_ID = Item[nEquipIdx1].m_dwID;
					g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));
				}
				// ���ϵĶ��������˽�����
				ITEM_SYNC	sItem;
				sItem.ProtocolType = s2c_syncitem;
				sItem.m_Point = Item[m_Hand].IsPurple();
				sItem.m_GoldId = Item[m_Hand].GetGoldId();
				sItem.m_EnChance = Item[m_Hand].GetEnChance();
				sItem.m_StackNum = Item[m_Hand].GetStackNum();
				sItem.m_ID = Item[m_Hand].GetID();
				sItem.m_Genre = Item[m_Hand].GetGenre();
				sItem.m_Detail = Item[m_Hand].GetDetailType();
				sItem.m_Particur = Item[m_Hand].GetParticular();
				sItem.m_Series = Item[m_Hand].GetSeries();
				sItem.m_Level = Item[m_Hand].GetLevel();
				sItem.m_Luck = Item[m_Hand].m_GeneratorParam.nLuck;
				sItem.m_btPlace = pos_trade1;
				sItem.m_btX = DesPos->nX;
				sItem.m_btY = DesPos->nY;
				sItem.m_Time.bYear = Item[m_Hand].GetTime()->bYear;
				sItem.m_Time.bMonth = Item[m_Hand].GetTime()->bMonth;
				sItem.m_Time.bDay = Item[m_Hand].GetTime()->bDay;
				sItem.m_Time.bHour = Item[m_Hand].GetTime()->bHour;
				for (int j = 0; j < 6; j++)
					sItem.m_MagicLevel[j] = Item[m_Hand].m_GeneratorParam.nGeneratorLevel[j];
				sItem.m_RandomSeed = Item[m_Hand].m_GeneratorParam.uRandomSeed;
				sItem.m_Version = Item[m_Hand].m_GeneratorParam.nVersion;
				sItem.m_Durability = Item[m_Hand].GetDurability();
				g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sItem, sizeof(ITEM_SYNC));

				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
				m_Hand = nEquipIdx1;
				if (FindSame(nEquipIdx1))
					m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_trade].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
#ifdef _SERVER
			// ����Ϣ�����׶Է�
			if (nEquipIdx1)	// �������ж���������
			{
				ITEM_REMOVE_SYNC	sRemove;
				sRemove.ProtocolType = s2c_removeitem;
				sRemove.m_ID = Item[nEquipIdx1].m_dwID;
				g_pServer->PackDataToClient(Player[Player[m_PlayerIdx].m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sRemove, sizeof(ITEM_REMOVE_SYNC));
			}
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}
		}
		break;
	case pos_give:
		nEquipIdx1 = m_Room[room_give].FindItem(SrcPos->nX, SrcPos->nY);
		if (nEquipIdx1 < 0)
			return;

		// �ȰѶ���������
		if (nEquipIdx1)
		{
			if (!m_Room[room_give].PickUpItem(nEquipIdx1, SrcPos->nX, SrcPos->nY, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight()))
				return;
		}
		if (m_Hand)
			if (Item[m_Hand].CanStack(nEquipIdx1))
			{
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
				if (Item[nEquipIdx1].Stack(Item[m_Hand].GetStackNum()))
				{
					Item[m_Hand].Remove();
					Remove(m_Hand);
				}
				else
				{
					Item[m_Hand].SetStackNum(Item[nEquipIdx1].GetStackNum() - 50);
					Item[nEquipIdx1].SetStackNum(50);
				}
				m_Room[room_give].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
				
				return;
			}

		// ���������ж������������ܲ��ܰ�������ȥ��������еĻ������ð�ԭ���Ķ����Ż�ȥ
		if (m_Hand)
		{
			if (m_Room[room_give].PlaceItem(DesPos->nX, DesPos->nY, m_Hand, Item[m_Hand].GetWidth(), Item[m_Hand].GetHeight()))
			{
				int nListIdx = FindSame(m_Hand);
				m_Items[nListIdx].nPlace = pos_give;
				m_Items[nListIdx].nX = DesPos->nX;
				m_Items[nListIdx].nY = DesPos->nY;
				m_Hand = nEquipIdx1;
				m_Items[FindSame(nEquipIdx1)].nPlace = pos_hand;
#ifdef _SERVER
				g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
			}
			// ��ԭ���Ķ����Ż�ȥ
			else
			{
				m_Room[room_give].PlaceItem(SrcPos->nX, SrcPos->nY, nEquipIdx1, Item[nEquipIdx1].GetWidth(), Item[nEquipIdx1].GetHeight());
			}
		}
		// ��������û�ж�����ֻ��Ҫ�Ѽ������Ķ����ŵ������
		else
		{
			int nListIdx = FindSame(nEquipIdx1);
			if (nEquipIdx1 && nListIdx)
			{
				m_Items[nListIdx].nPlace = pos_hand;
				m_Hand = nEquipIdx1;
			}

#ifdef _SERVER
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(PLAYER_MOVE_ITEM_SYNC));
#endif
		}
		break;
	}


#ifndef _SERVER
	// Notify to Menu
	if (m_Hand != nTempHand)	// �����ɹ���
	{
	//uParam = (KUiObjAtRegion*)pInfo -> ��Ʒ���ݼ����������λ����Ϣ
	//nParam = bAdd -> 0ֵ��ʾ���������Ʒ����0ֵ��ʾ���������Ʒ
		KUiObjAtContRegion pInfo1, pInfo2;
		if (nTempHand == 0)
		{
			pInfo2.Obj.uGenre = CGOG_NOTHING;
			pInfo2.Obj.uId = 0;
			pInfo2.Region.Width = 0;
			pInfo2.Region.Height = 0;
		}
		else
		{
			pInfo2.Obj.uGenre = CGOG_ITEM;	//Դװ��
			pInfo2.Obj.uId = nTempHand;
			pInfo2.Region.Width = Item[nTempHand].GetWidth();
			pInfo2.Region.Height = Item[nTempHand].GetHeight();
		}
		
		if (m_Hand == 0)
		{
			pInfo1.Obj.uGenre = CGOG_NOTHING;
			pInfo1.Obj.uId = 0;
			pInfo1.Region.Width = 0;
			pInfo1.Region.Height = 0;
		}
		else
		{
			pInfo1.Obj.uGenre = CGOG_ITEM;
			pInfo1.Obj.uId = m_Hand;
			pInfo1.Region.Width = Item[m_Hand].GetWidth();
			pInfo1.Region.Height = Item[m_Hand].GetHeight();
		}

		int PartConvert[itempart_num] = 
		{
			UIEP_HEAD,
			UIEP_BODY,
			UIEP_WAIST,
			UIEP_HAND,
			UIEP_FOOT,
			UIEP_FINESSE,
			UIEP_NECK,
			UIEP_FINGER1,
			UIEP_FINGER2,
			UIEP_WAIST_DECOR,
			UIEP_HORSE,
			UIEP_MASK	// mat na
		};

		switch(SrcPos->nPlace)
		{
		case pos_immediacy:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_IMMEDIA_ITEM;
			pInfo2.eContainer = UOC_IMMEDIA_ITEM;
			break;
		case pos_equiproom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_ITEM_TAKE_WITH;
			pInfo2.eContainer = UOC_ITEM_TAKE_WITH;
			break;
		case pos_equip:
			pInfo1.Region.h = 0;
			pInfo1.Region.v = PartConvert[SrcPos->nX];
			pInfo2.Region.h = 0;
			pInfo2.Region.v = PartConvert[DesPos->nX];
			pInfo1.eContainer = UOC_EQUIPTMENT;
			pInfo2.eContainer = UOC_EQUIPTMENT;
			break;
		case pos_repositoryroom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_STORE_BOX;
			pInfo2.eContainer = UOC_STORE_BOX;
			break;
		case pos_exbox1room: // ruong mo rong 1
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_EX_BOX1;
			pInfo2.eContainer = UOC_EX_BOX1;
			break;
		case pos_exbox2room: // ruong mo rong 2
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_EX_BOX2;
			pInfo2.eContainer = UOC_EX_BOX2;
			break;
		case pos_exbox3room: // ruong mo rong 3
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_EX_BOX3;
			pInfo2.eContainer = UOC_EX_BOX3;
			break;
		case pos_equiproomex: // mo rong hanh trang
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_ITEM_EX;
			pInfo2.eContainer = UOC_ITEM_EX;
			break;
		case pos_traderoom:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_TO_BE_TRADE;
			pInfo2.eContainer = UOC_TO_BE_TRADE;
			break;
		case pos_give:
			pInfo1.Region.h = SrcPos->nX;
			pInfo1.Region.v = SrcPos->nY;
			pInfo2.Region.h = DesPos->nX;
			pInfo2.Region.v = DesPos->nY;
			pInfo1.eContainer = UOC_ITEM_GIVE;
			pInfo2.eContainer = UOC_ITEM_GIVE;
			break;
		}
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo1, 0);
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&pInfo2, 0);
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&pInfo1, 1);
		CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo2, 1);
	}
#endif
	if (Npc[Player[m_PlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[m_PlayerIdx].m_nIndex].SendCommand(do_stand);
	}
}

#ifndef	_SERVER
//---------------------------------------------------------------------
//	���ܣ���Ʒ��һ���ط�ֱ���ƶ�����һ���ط����������������м����
//---------------------------------------------------------------------
BOOL	KItemList::AutoMoveItem(ItemPos SrcPos,ItemPos DesPos)
{
	if (Player[this->m_PlayerIdx].CheckTrading())	// ������ڽ���
		return FALSE;

	BOOL	bMove = FALSE;
	int		nIdx, nListIdx;

	// Ŀǰֻ֧�ִ�room_equipment��room_immediacy
	switch (SrcPos.nPlace)
	{
	case pos_equiproom:
		{
			switch (DesPos.nPlace)
			{
			case pos_immediacy:
				{
					nIdx = m_Room[room_equipment].FindItem(SrcPos.nX, SrcPos.nY);
					if (nIdx <= 0)
						return FALSE;
					if (Item[nIdx].GetGenre() != item_medicine)
					{
						_ASSERT(0);
						return FALSE;
					}
					if (!m_Room[room_equipment].PickUpItem(nIdx, SrcPos.nX, SrcPos.nY, Item[nIdx].GetWidth(), Item[nIdx].GetHeight()))
						return FALSE;
					m_Room[room_immediacy].PlaceItem(DesPos.nX, DesPos.nY, nIdx, Item[nIdx].GetWidth(), Item[nIdx].GetHeight());
					nListIdx = FindSame(nIdx);
					if (nListIdx <= 0)
						return FALSE;
					m_Items[nListIdx].nPlace = pos_immediacy;
					m_Items[nListIdx].nX = DesPos.nX;
					m_Items[nListIdx].nY = DesPos.nY;
					bMove = TRUE;
				}
				break;
			}
		}
		break;
	}

	if (!bMove)
		return bMove;

	// ֪ͨ����
	KUiObjAtContRegion sSrcInfo, sDestInfo;

	sSrcInfo.Obj.uGenre		= CGOG_ITEM;
	sSrcInfo.Obj.uId		= nIdx;
	sSrcInfo.Region.Width	= Item[nIdx].GetWidth();
	sSrcInfo.Region.Height	= Item[nIdx].GetHeight();
	sSrcInfo.Region.h		= SrcPos.nX;
	sSrcInfo.Region.v		= SrcPos.nY;
	sSrcInfo.eContainer		= UOC_ITEM_TAKE_WITH;

	sDestInfo.Obj.uGenre	= CGOG_ITEM;
	sDestInfo.Obj.uId		= nIdx;
	sDestInfo.Region.Width	= Item[nIdx].GetWidth();
	sDestInfo.Region.Height	= Item[nIdx].GetHeight();
	sDestInfo.Region.h		= DesPos.nX;
	sDestInfo.Region.v		= DesPos.nY;
	sDestInfo.eContainer	= UOC_IMMEDIA_ITEM;

	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sSrcInfo, 0);
	CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&sDestInfo, 1);

	return bMove;
}
#endif

#ifndef	_SERVER
//---------------------------------------------------------------------
//	���ܣ���Ʒ��һ���ط�ֱ���ƶ�����һ���ط����������������м����
//---------------------------------------------------------------------
void	KItemList::MenuSetMouseItem()
{
	KUiObjAtContRegion	sInfo;
	if (!m_Hand)
	{
		CoreDataChanged(GDCNI_HOLD_OBJECT, 0, 0);
	}
	else
	{
		sInfo.Obj.uGenre = CGOG_ITEM;
		sInfo.Obj.uId = m_Hand;
		sInfo.Region.Width = Item[m_Hand].GetWidth();
		sInfo.Region.Height = Item[m_Hand].GetHeight();
		sInfo.Region.h = 0;
		sInfo.Region.v = 0;
		sInfo.eContainer = UOC_IN_HAND;
		CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&sInfo, 0);
	}
}
#endif

#ifdef _SERVER
BOOL KItemList::EatMecidine(int nPlace, int nX, int nY)
{
	int nItemIdx = 0;
	switch(nPlace)
	{
	case pos_equiproom:
		nItemIdx = m_Room[room_equipment].FindItem(nX, nY);
		if (nItemIdx > 0)
			return EatMecidine(nItemIdx);
		break;
	case pos_immediacy:
		nItemIdx = m_Room[room_immediacy].FindItem(nX, nY);
		if (nItemIdx > 0)
		{
			if (Item[nItemIdx].GetGenre() != item_medicine)
				return EatMecidine(nItemIdx);
			int		nGenre, nDetailType, nIdx, nXpos, nYpos;
			BOOL	bEat;

			nGenre = item_medicine;
			nDetailType = Item[nItemIdx].GetDetailType();

			bEat = EatMecidine(nItemIdx);
			if (bEat == FALSE)
				return bEat;

			if (FALSE == FindSameDetailTypeInEquipment(nGenre, nDetailType, &nIdx, &nXpos, &nYpos))
				return bEat;

			this->AutoMoveMedicine(nIdx, nXpos, nYpos, nX, nY);

			return bEat;
		}
		break;
	default:
		break;
	}

	return FALSE;
}
#endif

PlayerItem* KItemList::GetFirstItem()
{
	m_nListCurIdx = m_UseIdx.GetNext(0);
	return &m_Items[m_nListCurIdx];
}

PlayerItem* KItemList::GetNextItem()
{
	if ( !m_nListCurIdx )
		return NULL;
	m_nListCurIdx = m_UseIdx.GetNext(m_nListCurIdx);
	return &m_Items[m_nListCurIdx];
}

void	KItemList::ClearRoom(int nRoom)
{
	if (nRoom >= 0 && nRoom < room_num)
		this->m_Room[nRoom].Clear();
}


void	KItemList::BackupTrade()
{
	if ( !m_Room[room_tradeback].m_pArray )
		m_Room[room_tradeback].Init(m_Room[room_equipment].m_nWidth, m_Room[room_equipment].m_nHeight);
	memcpy(m_Room[room_tradeback].m_pArray, m_Room[room_equipment].m_pArray, sizeof(int) * m_Room[room_tradeback].m_nWidth * m_Room[room_tradeback].m_nHeight);

	memcpy(this->m_sBackItems, this->m_Items, sizeof(PlayerItem) * MAX_PLAYER_ITEM);

	m_nBackHand = m_Hand;
}

void	KItemList::RecoverTrade()
{
	memcpy(m_Room[room_equipment].m_pArray, m_Room[room_tradeback].m_pArray, sizeof(int) * m_Room[room_tradeback].m_nWidth * m_Room[room_tradeback].m_nHeight);

#ifndef _SERVER
	int nIdx = 0;
	while((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (m_Items[nIdx].nPlace == pos_trade1)
			Remove(m_Items[nIdx].nIdx);
	}
/*	for (int i = 0; i < MAX_PLAYER_ITEM; i++)
	{
		if (m_Items[i].nIdx && m_Items[i].nPlace == pos_trade1)
			Remove(m_Items[i].nIdx);
	}*/
#endif
	memcpy(m_Items, m_sBackItems, sizeof(PlayerItem) * MAX_PLAYER_ITEM);
	m_Hand = m_nBackHand;
}

void	KItemList::StartTrade()
{
	BackupTrade();
	ClearRoom(room_trade);
	ClearRoom(room_trade1);
}

/*!*****************************************************************************
// Function		: KItemList::RemoveAll
// Purpose		: �˳�ʱ������е�װ��
// Return		: void
// Comments		: ��ʵ�ʵش���Ϸ�����еĵ���������ȥ��
// Author		: Spe
*****************************************************************************/
void KItemList::RemoveAll()
{
	int nIdx = m_UseIdx.GetNext(0);
	int nIdx1 = 0;
	while(nIdx)
	{
		nIdx1 = m_UseIdx.GetNext(nIdx);
		int nGameIdx = m_Items[nIdx].nIdx;
		Remove(m_Items[nIdx].nIdx);
#ifdef _SERVER
		// �ͻ���������KItemList::Remove()�Ѿ�����ItemSet.Remove()
		ItemSet.Remove(nGameIdx);
#endif
		nIdx = nIdx1;
	}
}

int KItemList::GetWeaponParticular()
{
	if (m_EquipItem[itempart_weapon])
		return Item[m_EquipItem[itempart_weapon]].GetParticular();
	return -1;
}

#ifdef _SERVER
BOOL KItemList::IsTaskItemExist(int nDetailType,BYTE bType)
{
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (bType == 1)
		{
			int nGameIdx = m_Items[nIdx].nIdx;
			if (item_task != Item[nGameIdx].GetGenre())
				continue;
			if (nDetailType == Item[nGameIdx].GetDetailType())
			{
				return TRUE;
			}
		}
		else if (bType == 2)
		{
			int nGameIdx = m_Items[nIdx].nIdx;
			if (item_mine != Item[nGameIdx].GetGenre())
				continue;
			if (nDetailType == Item[nGameIdx].GetParticular())
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}
#endif

#ifdef _SERVER
int		KItemList::GetTaskItemNum(int nDetailType,BYTE bType)
{
	int		nNo = 0;
	int		nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (bType == 1)
		{
			if (item_task != Item[m_Items[nIdx].nIdx].GetGenre())
				continue;
			if (nDetailType == Item[m_Items[nIdx].nIdx].GetDetailType())
			{
				nNo++;
			}
		}
		else if (bType == 2)
		{
			if (item_mine != Item[m_Items[nIdx].nIdx].GetGenre())
				continue;
			if (nDetailType == Item[m_Items[nIdx].nIdx].GetParticular())
			{
				nNo++;
			}
		}
	}
	return nNo;
}
#endif

#ifdef _SERVER
BOOL KItemList::RemoveTaskItem(int nDetailType)
{
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		if (item_task != Item[nGameIdx].GetGenre())
			continue;
		if (nDetailType == Item[nGameIdx].GetDetailType())
		{
			Remove(nGameIdx);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL KItemList::RemoveMineItem(int nDetailType)
{
	int nIdx = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		int nGameIdx = m_Items[nIdx].nIdx;
		if (item_mine != Item[nGameIdx].GetGenre())
			continue;
		if (nDetailType == Item[nGameIdx].GetParticular())
		{
			Remove(nGameIdx);
			return TRUE;
		}
	}
	return FALSE;
}
#endif

#ifdef _SERVER
//--------------------------------------------------------------------------
//	���ܣ������а� trade room �е� item �� idx width height ��Ϣд�� itemset �е� m_psItemInfo ��ȥ
//--------------------------------------------------------------------------
void	KItemList::GetTradeRoomItemInfo()
{
	_ASSERT(ItemSet.m_psItemInfo);
//	if (!ItemSet.m_psItemInfo)
//	{
//		ItemSet.m_psItemInfo = new TRADE_ITEM_INFO[TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT];
//	}
	memset(ItemSet.m_psItemInfo, 0, sizeof(TRADE_ITEM_INFO) * TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	while (1)
	{
		nItemIdx = m_Room[room_trade].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;
		_ASSERT(nPos < TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT);

		ItemSet.m_psItemInfo[nPos].m_nIdx = nItemIdx;
		ItemSet.m_psItemInfo[nPos].m_nWidth = Item[nItemIdx].GetWidth();
		ItemSet.m_psItemInfo[nPos].m_nHeight = Item[nItemIdx].GetHeight();
		nPos++;
	}

	// �Ӵ�С����
	TRADE_ITEM_INFO	sTemp;
	for (int i = nPos - 1; i >= 0; i--)
	{
		for (int j = 0; j < i; j++)
		{
			if (ItemSet.m_psItemInfo[j].m_nWidth * ItemSet.m_psItemInfo[j].m_nHeight < 
				ItemSet.m_psItemInfo[j + 1].m_nWidth * ItemSet.m_psItemInfo[j + 1].m_nHeight)
			{
				sTemp = ItemSet.m_psItemInfo[j];
				ItemSet.m_psItemInfo[j] = ItemSet.m_psItemInfo[j + 1];
				ItemSet.m_psItemInfo[j + 1] = sTemp;
			}
		}
	}
}
#endif

#ifdef _SERVER
//--------------------------------------------------------------------------
//	���ܣ��������ж��������Ʒ�ܲ�����ȫ�Ž��Լ�����Ʒ��
//--------------------------------------------------------------------------
BOOL	KItemList::TradeCheckCanPlace()
{
	LPINT	pnTempRoom;
	pnTempRoom = new int[EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT];
	memcpy(pnTempRoom, m_Room[room_equipment].m_pArray, sizeof(int) * EQUIPMENT_ROOM_WIDTH * EQUIPMENT_ROOM_HEIGHT);

	int		nPos, i, j, a, b, nFind, nNext;
	for (nPos = 0; nPos < TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT; nPos++)
	{
		if (!ItemSet.m_psItemInfo[nPos].m_nIdx)
			break;
		nFind = 0;
		for (i = 0; i < EQUIPMENT_ROOM_HEIGHT - ItemSet.m_psItemInfo[nPos].m_nHeight + 1; i++)
		{
			for (j = 0; j < EQUIPMENT_ROOM_WIDTH - ItemSet.m_psItemInfo[nPos].m_nWidth + 1; j++)
			{
				nNext = 0;
				for (a = 0; a < ItemSet.m_psItemInfo[nPos].m_nHeight; a++)
				{
					for (b = 0; b < ItemSet.m_psItemInfo[nPos].m_nWidth; b++)
					{
						if (pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b])
						{
							nNext = 1;
							break;
						}
					}
					if (nNext)
						break;
				}
				// �ҵ�һ��λ��
				if (!nNext)
				{
					// ���ݴ���
					ItemSet.m_psItemInfo[nPos].m_nX = j;
					ItemSet.m_psItemInfo[nPos].m_nY = i;
					for (a = 0; a < ItemSet.m_psItemInfo[nPos].m_nHeight; a++)
					{
						for (b = 0; b < ItemSet.m_psItemInfo[nPos].m_nWidth; b++)
							pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b] = ItemSet.m_psItemInfo[nPos].m_nIdx;
					}

					nFind = 1;
					break;
				}
			}
			if (nFind)
				break;
		}
		if (!nFind)
		{
			delete []pnTempRoom;
			return FALSE;
		}
	}

	delete []pnTempRoom;
	return TRUE;
}
#endif

#ifdef _SERVER
//--------------------------------------------------------------------------
//	���ܣ��ж�һ����������Ʒ�ܷ�Ž���Ʒ�� (Ϊ�˷�����Ч�ʣ�����������û�е�����������)
//--------------------------------------------------------------------------
BOOL	KItemList::CheckCanPlaceInEquipment(int nWidth, int nHeight, int *pnX, int *pnY)
{
	if (nWidth <= 0 || nHeight <= 0 || !pnX || !pnY)
		return FALSE;

	_ASSERT(m_Room[room_equipment].m_pArray);

	LPINT	pnTempRoom;
	int		i, j, a, b, nNext;

	pnTempRoom = m_Room[room_equipment].m_pArray;

	for (i = 0; i < EQUIPMENT_ROOM_HEIGHT - nHeight + 1; i++)
	{
		for (j = 0; j < EQUIPMENT_ROOM_WIDTH - nWidth + 1; j++)
		{
			nNext = 0;
			for (a = 0; a < nHeight; a++)
			{
				for (b = 0; b < nWidth; b++)
				{
					if (pnTempRoom[(i + a) * EQUIPMENT_ROOM_WIDTH + j + b])
					{
						nNext = 1;
						break;
					}
				}
				if (nNext)
					break;
			}
			if (!nNext)
			{
				*pnX = j;
				*pnY = i;
				return TRUE;
			}
		}
	}

	return FALSE;
}
#endif

//------------------------------------------------------------------------------
//	���ܣ���room_equipment�в���ָ��Genre��DetailType����Ʒ���õ�ItemIdx��λ��
//------------------------------------------------------------------------------
BOOL	KItemList::FindSameDetailTypeInEquipment(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY)
{
	return m_Room[room_equipment].FindSameDetailType(nGenre, nDetail, pnIdx, pnX, pnY);
}

#ifdef _SERVER
//------------------------------------------------------------------------------
//	���ܣ��Զ���һ��ҩƷ��room_equipment�ƶ���room_immediacy
//------------------------------------------------------------------------------
BOOL	KItemList::AutoMoveMedicine(int nItemIdx, int nSrcX, int nSrcY, int nDestX, int nDestY)
{
	if (!m_Room[room_equipment].m_pArray || !m_Room[room_immediacy].m_pArray)
		return FALSE;
	if (nSrcX < 0 || nSrcX >= m_Room[room_equipment].m_nWidth || nSrcY < 0 || nSrcY >= m_Room[room_equipment].m_nHeight)
		return FALSE;
	if (nDestX < 0 || nDestX >= m_Room[room_immediacy].m_nWidth || nDestY < 0 || nDestY >= m_Room[room_immediacy].m_nHeight)
		return FALSE;
	if (nItemIdx != m_Room[room_equipment].m_pArray[nSrcY * m_Room[room_equipment].m_nWidth + nSrcX] ||
		0 != m_Room[room_immediacy].m_pArray[nDestY * m_Room[room_immediacy].m_nWidth + nDestX])
		return FALSE;

	_ASSERT(Item[nItemIdx].GetWidth() == 1 && Item[nItemIdx].GetHeight() == 1);
	if (!m_Room[room_equipment].PickUpItem(nItemIdx, nSrcX, nSrcY, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight()))
		return FALSE;
	if (!m_Room[room_immediacy].PlaceItem(nDestX, nDestY, nItemIdx, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight()))
	{
		m_Room[room_equipment].PlaceItem(nSrcX, nSrcY, nItemIdx, Item[nItemIdx].GetWidth(), Item[nItemIdx].GetHeight());
		return FALSE;
	}

	int nListIdx = FindSame(nItemIdx);
	_ASSERT(nListIdx > 0);
	m_Items[nListIdx].nPlace = pos_immediacy;
	m_Items[nListIdx].nX = nDestX;
	m_Items[nListIdx].nY = nDestY;

	ITEM_AUTO_MOVE_SYNC	sMove;
	sMove.ProtocolType = s2c_ItemAutoMove;
	sMove.m_btSrcPos = pos_equiproom;
	sMove.m_btSrcX = nSrcX;
	sMove.m_btSrcY = nSrcY;
	sMove.m_btDestPos = pos_immediacy;
	sMove.m_btDestX = nDestX;
	sMove.m_btDestY = nDestY;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&sMove, sizeof(ITEM_AUTO_MOVE_SYNC));

	return TRUE;
}
#endif

#ifndef	_SERVER
void	KItemList::RemoveAllInOneRoom(int nRoom)
{
	if (nRoom < 0 || nRoom >= room_num)
		return;

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	while (1)
	{
		nItemIdx = m_Room[nRoom].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;
		_ASSERT(nPos < m_Room[nRoom].m_nWidth * m_Room[nRoom].m_nHeight);

		Remove(nItemIdx);

		nPos++;
	}
}
#endif

#ifndef _SERVER
void KItemList::LockOperation()
{
	if (IsLockOperation())
	{
		_ASSERT(0);
		return;
	}
	m_bLockOperation = TRUE;
}
#endif

#ifndef _SERVER
void KItemList::UnlockOperation()
{
	if (!IsLockOperation())
	{
		return;
	}
	m_bLockOperation = FALSE;
}
#endif

int KItemList::GetActiveAttribNum(int nIdx)
{
	for (int i = 0; i < itempart_num; i++)
	{
		if (nIdx == m_EquipItem[i])
		{
			return GetEquipEnhance(i);
		}
	}
	return 0;
}

#ifdef _SERVER
//-------------------------------------------------------------------------------
//	���ܣ���ʧ������Ʒ
//-------------------------------------------------------------------------------
void	KItemList::AutoLoseItemFromEquipmentRoom(int nRate)
{
	if (Player[m_PlayerIdx].CheckTrading())
		return;
	if (nRate <= 0 || nRate > 100)
		return;

	int		nItemIdx, nXpos, nYpos, nPos;

	nItemIdx = 0;
	nXpos = 0;
	nYpos = 0;
	nPos = 0;

	// ��ͳ��������Ʒ���ų�������Ʒ
	while (1)
	{
		nItemIdx = m_Room[room_equipment].GetNextItem(nItemIdx, nXpos, nYpos, &nXpos, &nYpos);
		if (nItemIdx == 0)
			break;
		if (item_task == Item[nItemIdx].GetGenre())
			continue;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nIdx = nItemIdx;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nPlace = pos_equiproom;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nX = nXpos;
		ItemSet.m_sLoseItemFromEquipmentRoom[nPos].nY = nYpos;
		nPos++;
	}
	if (nPos == 0)
		return;

	KMapPos			sMapPos;
	int				nSelect;
	int				nObj;
	KObjItemInfo	sInfo;

	for (int i = 0; i < nPos; i++)
	{
		if (g_Random(100) >= nRate)
			continue;
		nItemIdx = ItemSet.m_sLoseItemFromEquipmentRoom[i].nIdx;
		Player[m_PlayerIdx].GetAboutPos(&sMapPos);
		// ����
		if (Remove(nItemIdx))
		{
			sInfo.m_nItemID = nItemIdx;
			sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			strcpy(sInfo.m_szName, Item[nItemIdx].m_CommonAttrib.szItemName);
			sInfo.m_nColorID = 0;
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;

			nObj = ObjSet.Add(Item[nItemIdx].GetObjIdx(), sMapPos, sInfo);
			if (nObj >= 0)
			{
				Object[nObj].SetItemBelong(-1);
			}

			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_DEATH_LOSE_ITEM;
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + sizeof(sInfo.m_szName);
			sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
			memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
			memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), sInfo.m_szName, sizeof(sInfo.m_szName));
			g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
		}
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------------
//	���ܣ���ʧһ���������ϵ�װ��(�������)
//-------------------------------------------------------------------------------
void	KItemList::AutoLoseEquip()
{
#ifndef defEQUIP_POWER
	int		i, nPos = 0;

	for (i = 0; i < itempart_num; i++)
	{
		if (m_EquipItem[i] <= 0)
			continue;
		if (i == itempart_horse)
			continue;
		ItemSet.m_sLoseEquipItem[nPos].nIdx = m_EquipItem[i];
		ItemSet.m_sLoseEquipItem[nPos].nPlace = nPos;
		nPos++;
	}
	if (nPos == 0)
		return;

	int		nSelect = g_Random(nPos);
#endif

#ifdef defEQUIP_POWER
	int		i, nPos = 0, nTotalPower = 0;

	for (i = 0; i < itempart_num; i++)
	{
		if (m_EquipItem[i] <= 0)
			continue;
		nTotalPower += g_nEquipPower[i];
		ItemSet.m_sLoseEquipItem[nPos].nIdx = m_EquipItem[i];
		ItemSet.m_sLoseEquipItem[nPos].nPlace = nPos;
		ItemSet.m_sLoseEquipItem[nPos].nX = nTotalPower;	// nX ����һ��
		nPos++;
	}
	if (nTotalPower == 0)
		return;
	int		nSelect = g_Random(nTotalPower);
	for (i = 0; i < nPos; i++)
	{
		if (ItemSet.m_sLoseEquipItem[i].nX > nSelect)
		{
			nSelect = i;
			break;
		}
	}
	if (i >= nPos)
		return;
#endif

	int			 nItemIdx;
	KMapPos		sMapPos;

	nItemIdx = ItemSet.m_sLoseEquipItem[nSelect].nIdx;
	Player[m_PlayerIdx].GetAboutPos(&sMapPos);
	if (Remove(nItemIdx))
	{
		int		nObj;
		KObjItemInfo	sInfo;
		sInfo.m_nItemID = nItemIdx;
		sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		strcpy(sInfo.m_szName, Item[nItemIdx].m_CommonAttrib.szItemName);
		sInfo.m_nColorID = 0;
		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;

		nObj = ObjSet.Add(Item[nItemIdx].GetObjIdx(), sMapPos, sInfo);
		if (nObj >= 0)
		{
			Object[nObj].SetItemBelong(-1);
		}

		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_DEATH_LOSE_ITEM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + sizeof(sInfo.m_szName);
		sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
		memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
		memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), sInfo.m_szName, sizeof(sInfo.m_szName));
		g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------------
//	���ܣ�
//-------------------------------------------------------------------------------
int		KItemList::GetSameDetailItemNum(int nImmediatePos)
{
	if (nImmediatePos < 0 || nImmediatePos >= IMMEDIACY_ROOM_WIDTH * IMMEDIACY_ROOM_HEIGHT)
		return 0;
	int		nIdx = m_Room[room_immediacy].FindItem(nImmediatePos, 0);
	if (nIdx <= 0)
		return 0;
	return m_Room[room_equipment].CalcSameDetailType(Item[nIdx].GetGenre(), Item[nIdx].GetDetailType()) + Item[nIdx].GetStackNum();
}
#endif
#ifdef _SERVER
void KItemList::Abrade(int nType)
{
	int nItemIdx = 0;
	for (int i = 0; i < itempart_num; i++)
	{
		nItemIdx = m_EquipItem[i];
		if (nItemIdx)
		{
			int nOldDur = Item[nItemIdx].GetDurability();
			int nDur = Item[nItemIdx].Abrade(ItemSet.GetAbradeRange(nType, i));
			if (nDur == 0)
			{
				// ���ͻ��˷���Ϣ
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				sMsg.m_wMsgID = enumMSG_ID_ITEM_DAMAGED;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
				sMsg.m_lpBuf = (void *)Item[nItemIdx].m_dwID;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				sMsg.m_lpBuf = 0;

				Remove(nItemIdx);
				ItemSet.Remove(nItemIdx);
			}
			else if (nOldDur != nDur && nDur != -1)
			{
				ITEM_DURABILITY_CHANGE sIDC;
				sIDC.ProtocolType = s2c_itemdurabilitychange;
				sIDC.dwItemID = Item[nItemIdx].GetID();
				sIDC.nChange = nDur - nOldDur;
				if (g_pServer)
					g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, &sIDC, sizeof(ITEM_DURABILITY_CHANGE));
			}
		}
	}
}
#endif

BOOL KItemList::GetIfActive()
{
	int nSet[9][2],bIfGet,nNum = 0;
	ZeroMemory(nSet,sizeof(nSet));
	for (int i = 0;i < itempart_num;i++)
	{
		bIfGet = 0;
		if (Item[m_EquipItem[i]].GetGoldId())
		{
			for (int j = 0;j < 9;j++)
			{
				if (nSet[j][0] == Item[m_EquipItem[i]].GetSet())
				{
					nSet[j][1]++;
					bIfGet = 1;
					if (nSet[j][1] >= Item[m_EquipItem[i]].GetSetNum())
					{
						m_bActiveSet = TRUE;
						return TRUE;
					}
				}
			}
			if (!bIfGet)
			{
				nSet[nNum][0] = Item[m_EquipItem[i]].GetSet();
				nSet[nNum][1] = 1;
				
				if (nSet[nNum][1] >= Item[m_EquipItem[i]].GetSetNum())
				{
					m_bActiveSet = TRUE;
					return TRUE;
				}
				nNum++;
			}
		}
	}
	m_bActiveSet = FALSE;
	return FALSE;


}
#ifndef _SERVER
int KItemList::GetGoldColor(int nSet,int nId )
{
	for (int i = 0;i < itempart_num;i++)
	{
		if (Item[m_EquipItem[i]].GetSet() == nSet && Item[m_EquipItem[i]].GetSetId() == nId)
		{
			return 2;
		}
	}

	for (i = 0;i < MAX_PLAYER_ITEM;i++)
	{
		if (Item[m_Items[i].nIdx].GetSet() == nSet && Item[m_Items[i].nIdx].GetSetId() == nId)
		{
			return 1;
		}
	}

	return 0;
}
#endif

#ifdef _SERVER
void KItemList::SetPrice( int nIdx, int nPrice )
{
	int nGameId = SearchID(nIdx);
	int nId = FindSame(nGameId);
	if (nPrice)
	{
		m_Items[nId].nPrice = nPrice;
	} 
	else
	{
		m_Items[nId].nPrice = 0;
	}
}

int KItemList::GetPrice( int nGameIdx )
{
	int nId = FindSame(nGameIdx);
	if (nId)
		return m_Items[nId].nPrice;
	else
		return 0;
}

void KItemList::CheckItemTime()
{

	time_t rawtime;
	struct tm * timeinfo;
	
	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	for (int i = 0;i < MAX_PLAYER_ITEM;i++)
	{

		if (Item[m_Items[i].nIdx].GetTime()->bYear)
		if (Item[m_Items[i].nIdx].GetTime()->bYear - 1900 < timeinfo->tm_year)
		{
			Remove(m_Items[i].nIdx);
			ItemSet.Remove(m_Items[i].nIdx);
		}
		else if (Item[m_Items[i].nIdx].GetTime()->bYear - 1900 == timeinfo->tm_year)
			if (Item[m_Items[i].nIdx].GetTime()->bMonth < timeinfo->tm_mon + 1)
			{
				Remove(m_Items[i].nIdx);
				ItemSet.Remove(m_Items[i].nIdx);
			}
			else if (Item[m_Items[i].nIdx].GetTime()->bMonth == timeinfo->tm_mon + 1)
				if (Item[m_Items[i].nIdx].GetTime()->bDay < timeinfo->tm_mday)
				{
					Remove(m_Items[i].nIdx);
					ItemSet.Remove(m_Items[i].nIdx);
				}
				else if (Item[m_Items[i].nIdx].GetTime()->bDay == timeinfo->tm_mday)
					if (Item[m_Items[i].nIdx].GetTime()->bHour <= timeinfo->tm_hour)
					{
						Remove(m_Items[i].nIdx);
						ItemSet.Remove(m_Items[i].nIdx);
					}
	}
}

BOOL KItemList::RemoveGoldItem( int nDetailType )
{
	if (nDetailType <= 0)
	{
		return FALSE;
	}
	for (int i = 0;i < MAX_PLAYER_ITEM;i++)
	{
		if (m_Items[i].nPlace != room_equipment)
		{
			continue;
		}
		if (Item[m_Items[i].nIdx].GetGoldId() == nDetailType)
		{
			Remove(m_Items[i].nIdx);
			ItemSet.Remove(m_Items[i].nIdx);
			return TRUE;
		}
	}
	return FALSE;
}

BOOL KItemList::IsGoldItemExist( int nDetailType )
{
	if (nDetailType <= 0)
	{
		return FALSE;
	}
	for (int i = 0;i < MAX_PLAYER_ITEM;i++)
	{
		if (m_Items[i].nPlace != room_equipment)
		{
			continue;
		}
		if (Item[m_Items[i].nIdx].GetGoldId() == nDetailType)
		{
			return TRUE;
		}
	}
	return FALSE;
}
#endif

// mat na
void KItemList::SetMaskLock( BOOL bFlag )
{
	m_nMaskLock = bFlag;
#ifdef _SERVER
	NPC_SIT_SYNC Sync;
	Sync.ProtocolType = s2c_syncmasklock;
	Sync.ID = m_nMaskLock;
	g_pServer->PackDataToClient(Player[m_PlayerIdx].m_nNetConnectIdx, (BYTE*)&Sync, sizeof(NPC_SIT_SYNC));
#endif
}
//
#ifdef _SERVER
BOOL KItemList::IsItemExist( int nGern,int nDetailType,int nPar,int nSerise,int nLevel )
{
	int nIdx = 0;
	while((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (Item[m_Items[nIdx].nIdx].GetGenre() != nGern ||
			Item[m_Items[nIdx].nIdx].GetDetailType() != nDetailType ||
			Item[m_Items[nIdx].nIdx].GetParticular() != nPar
			|| Item[m_Items[nIdx].nIdx].GetGoldId() || Item[m_Items[nIdx].nIdx].IsPurple())
			continue;		

		if (nSerise < 5)
			if (Item[m_Items[nIdx].nIdx].GetSeries() != nSerise)
				continue;

		if (nLevel != 0)
			if (Item[m_Items[nIdx].nIdx].GetLevel() != nLevel)
				continue;

		return TRUE;
	}
	return FALSE;
}

BOOL KItemList::DelExistItem( int nGern,int nDetailType,int nPar,int nSerise /*= 5*/,int nLevel /*= 0*/ )
{
	int nIdx = 0;
	while((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		if (Item[m_Items[nIdx].nIdx].GetGenre() != nGern ||
			Item[m_Items[nIdx].nIdx].GetDetailType() != nDetailType ||
			Item[m_Items[nIdx].nIdx].GetParticular() != nPar
			|| Item[m_Items[nIdx].nIdx].GetGoldId() || Item[m_Items[nIdx].nIdx].IsPurple())
			continue;		
		
		if (nSerise < 5)
			if (Item[m_Items[nIdx].nIdx].GetSeries() != nSerise)
				continue;
			
			if (nLevel != 0)
				if (Item[m_Items[nIdx].nIdx].GetLevel() != nLevel)
					continue;
				
		Remove(m_Items[nIdx].nIdx);
		ItemSet.Remove(m_Items[nIdx].nIdx);
		return TRUE;
	}
	return FALSE;
}
#endif


void KItemList::BackupGive()
{
	if ( !m_Room[room_giveback].m_pArray )
		m_Room[room_giveback].Init(m_Room[room_equipment].m_nWidth, m_Room[room_equipment].m_nHeight);
	memcpy(m_Room[room_giveback].m_pArray, m_Room[room_equipment].m_pArray, sizeof(int) * m_Room[room_giveback].m_nWidth * m_Room[room_giveback].m_nHeight);
	
	memcpy(this->m_sBackItems, this->m_Items, sizeof(PlayerItem) * MAX_PLAYER_ITEM);
	
	m_nBackHand = m_Hand;
}

void KItemList::RecoverGive()
{
	memcpy(m_Room[room_equipment].m_pArray, m_Room[room_giveback].m_pArray, sizeof(int) * m_Room[room_giveback].m_nWidth * m_Room[room_giveback].m_nHeight);

	memcpy(m_Items, m_sBackItems, sizeof(PlayerItem) * MAX_PLAYER_ITEM);
	m_Hand = m_nBackHand;
}

void KItemList::StartGive()
{
	BackupGive();
	ClearRoom(room_give);
}

//--------------------------- dem so item cung loai trong hanh trang
int		KItemList::GetItemNum(int nGenre, int nDetailType, int nParticular, int nLevel)
{
	int		nNo = 0;
	int		nIdx = 0, nX = 0, nY = 0;
	while ((nIdx = m_UseIdx.GetNext(nIdx)))
	{
		{	
				if (nGenre == Item[m_Items[nIdx].nIdx].GetGenre() && 
					nDetailType == Item[m_Items[nIdx].nIdx].GetDetailType() && 
					nParticular == Item[m_Items[nIdx].nIdx].GetParticular() &&
					nLevel == Item[m_Items[nIdx].nIdx].GetLevel())
						nNo++;
		}
	}
	return nNo;
}
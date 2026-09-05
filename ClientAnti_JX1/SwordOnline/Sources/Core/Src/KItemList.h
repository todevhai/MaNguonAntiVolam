#ifndef	KItemListH
#define	KItemListH

#include "KLinkArray.h"
#include "KItem.h"
#define	MAX_ITEM_ACTIVE	2


//#define		EQUIPMENT_ROOM_WIDTH		6
//#define		EQUIPMENT_ROOM_HEIGHT		10
//#define		REPOSITORY_ROOM_WIDTH		6
//#define		REPOSITORY_ROOM_HEIGHT		10
//#define		TRADE_ROOM_WIDTH			6
//#define		TRADE_ROOM_HEIGHT			10


#define	REQUEST_EQUIP_ITEM		1
#define	REQUEST_EAT_MEDICINE	2

typedef struct
{
	int		nPlace;
	int		nX;
	int		nY;
} ItemPos;


class CORE_API KItemList
{
private:
	int			m_PlayerIdx;
	int			m_Hand;
	int			m_nBackHand;
	int			m_EquipItem[itempart_num];						// ���װ���ĵ��ߣ���Ӧ��Ϸ�����е��������������
	PlayerItem	m_Items[MAX_PLAYER_ITEM];						// ���ӵ�е����е��ߣ�����װ���ŵĺ�������ŵģ���Ӧ��Ϸ�����е��������������
	PlayerItem	m_sBackItems[MAX_PLAYER_ITEM];					// ���׹����� m_Items �ı���
	static int	ms_ActiveEquip[itempart_num][MAX_ITEM_ACTIVE];	// ĳ�����ֵ�װ���������װ��
	static int	ms_ActivedEquip[itempart_num][MAX_ITEM_ACTIVE];	// ĳ�����ֵ�װ����ʲôװ������
	KLinkArray	m_FreeIdx;
	KLinkArray	m_UseIdx;
	int			m_nListCurIdx;									// ���� GetFirstItem �� GetNextItem

	BOOL		m_bActiveSet;
	BOOL		m_nMaskLock;	// mat na
#ifndef _SERVER
	BOOL		m_bLockOperation;
#endif
public:
	KInventory	m_Room[room_num];

private:
	int			FindFree();
	int			FindSame(int nGameIdx);							// nGameIdxָ��Ϸ�����е�������ı��
	int			GetEquipPlace(int nType);						// ȡ��ĳ����װ��Ӧ�÷ŵ�λ��
	BOOL		Fit(int nIdx, int nPlace);						// ����Ƿ����װ����
	BOOL		Fit(KItem* pItem, int nPlace);
	int			GetEquipEnhance(int nPlace);					// ȡ��װ���ļ�����������
	int			GetActiveEquipPlace(int nPlace, int nCount);	// ȡ��nPlace��װ������ĵ�nCount��װ��λ��
	void		InfectionNextEquip(int nPlace, BOOL bEquip = FALSE);
	// ��room_equipment�в���ָ��Genre��DetailType����Ʒ���õ�ItemIdx��λ��
	BOOL		FindSameDetailTypeInEquipment(int nGenre, int nDetail, int *pnIdx, int *pnX, int *pnY);
	friend	class KPlayer;

public:
	KItemList();
	~KItemList();
	int			Init(int nIdx);
	int			GetEquipment(int nIdx) { return m_EquipItem[nIdx]; }
	int			GetActiveAttribNum(int nIdx);					// nIdxָ��Ϸ�����е�������ı��
	int			GetWeaponType();								// ȡ�����������ͣ���������(equip_meleeweapon)����Զ������(equip_rangeweapon)
	int			GetWeaponParticular();							// ȡ�������ľ������ͣ����ڼ��㲻ͬ���˺���ǿ
	void		GetWeaponDamage(int* nMin, int* nMax);			// ȡ���������˺�
	int			Add(int nIdx, int nPlace, int nX, int nY, BOOL bInit = FALSE);		// nIdxָ��Ϸ�����е�������ı��
	BOOL		Remove(int nIdx);								// nIdxָ��Ϸ�����е�������ı��
	void		RemoveAll();
	BOOL		CanEquip(int nIdx, int nPlace = -1);			// nIdxָ��Ϸ�����е�������ı��
	BOOL		CanEquip(KItem* pItem, int nPlace = -1);
	BOOL		EnoughAttrib(void* pData);
	BOOL		Equip(int nIdx, int nPlace = -1);				// nIdxָ��Ϸ�����е�������ı��
	BOOL		UnEquip(int nIdx, int nPlace = -1);				// nIdxָ��Ϸ�����е�������ı��
	BOOL		EatMecidine(int nIdx);							// nIdxָ��Ϸ�����е�������ı��
	PlayerItem*	GetFirstItem();
	PlayerItem*	GetNextItem();
	int			SearchID(int nID);
	void		ExchangeMoney(int nSrcRoom, int DesRoom, int nMoney);
	void		ExchangeItem(ItemPos* SrcPos,ItemPos* DesPos);
	int			GetMoneyAmount();					// �õ���Ʒ���ʹ��������Ǯ��
	int			GetEquipmentMoney();				// �õ���Ʒ���ʹ������Ǯ��
	BOOL		AddMoney(int nRoom, int nMoney);
	BOOL		CostMoney(int nMoney);
	BOOL		DecMoney(int nMoney);
	void		SetMoney(int nMoney1, int nMoney2, int nMoney3);
	void		SetRoomMoney(int nRoom, int nMoney);
	int			GetMoney(int nRoom) { return m_Room[nRoom].GetMoney(); }	// ȡ�ÿռ��Ǯ
	//Xu
	int			GetXu(int nRoom) { return m_Room[nRoom].GetXu(); }
	void		SetXu(int nXu);
	void		SetRoomXu(int nRoom, int nXu);
	BOOL		AddXu(int nRoom, int nXu);
	BOOL		CostXu(int nXu);
	BOOL		DecXu(int nXu);
	int			GetXuAmount();					
	int			GetEquipmentXu();
	/////////////////////////////////
	void		SetPlayerIdx(int nIdx);				// �趨�������
	int			Hand() { return m_Hand; };
	void		ClearRoom(int nRoom);
	void		BackupTrade();
	void		RecoverTrade();
	void		StartTrade();
	int			GetItemNum(int nGenre, int nDetailType, int nParticular, int nLevel); // dem so item cung loai


	void		BackupGive();
	void		RecoverGive();
	void		StartGive();

	BOOL		GetIfActive();
	BOOL		GetMaskLock() {return m_nMaskLock;};	// mat na
	void		SetMaskLock(BOOL bFlag);// mat na
#ifdef	_SERVER
	void		Abrade(int nType);
	void		TradeMoveMoney(int nMoney);			// ���ô˽ӿڱ��뱣֤�����nMoney��һ����Ч��(�����Ҳ���������Ǯ��)
	void		SendMoneySync();					// ��������moneyͬ����Ϣ���ͻ���
	void		SendXuSync();
	BOOL		IsItemExist(int nGern,int nDetailType,int nPar,int nSerise = 5,int nLevel = 0);
	BOOL		DelExistItem(int nGern,int nDetailType,int nPar,int nSerise = 5,int nLevel = 0);
	BOOL		IsTaskItemExist(int nDetailType, BYTE bType = 1);
	int			GetTaskItemNum(int nDetailType, BYTE bType = 1 );
	BOOL		RemoveTaskItem(int nDetailType);
	BOOL		RemoveMineItem(int nDetailType);
	BOOL		RemoveGoldItem(int nDetailType);
	BOOL		IsGoldItemExist(int nDetailType);
	void		GetTradeRoomItemInfo();			// �����а� trade room �е� item �� idx width height ��Ϣд�� itemset �е� m_psItemInfo ��ȥ
	BOOL		TradeCheckCanPlace();				// �������ж��������Ʒ�ܲ�����ȫ�Ž��Լ�����Ʒ��
	BOOL		CheckCanPlaceInEquipment(int nWidth, int nHeight, int *pnX, int *pnY);// �ж�һ����������Ʒ�ܷ�Ž���Ʒ��
	BOOL		EatMecidine(int nPlace, int nX, int nY);		// ��ʲô�ط���ҩ
	// �Զ���һ��ҩƷ��room_equipment�ƶ���room_immediacy
	BOOL		AutoMoveMedicine(int nItemIdx, int nSrcX, int nSrcY, int nDestX, int nDestY);
	// ��ʧ������Ʒ
	void		AutoLoseItemFromEquipmentRoom(int nRate);
	// ��ʧһ���������ϵ�װ��(�������)
	void		AutoLoseEquip();

	void		SetPrice(int nIdx, int nPrice);
	int			GetPrice(int nGameIdx);

	void		CheckItemTime();
#endif

#ifndef	_SERVER
	int			UseItem(int nIdx);					// nIdxָ��Ϸ�����е�������ı��
	BOOL		SearchPosition(int nWidth, int nHeight, ItemPos* pPos);
	BOOL		AutoMoveItem(ItemPos SrcPos,ItemPos DesPos);
	// Tra mon dang cam tren tay (m_Hand) ve o trong dau tien cua tui. Dung khi
	// mot thao tac bi tu choi (vd tha trung loai vao o phim tat) de mon khong ket
	// vo hinh tren con tro. FALSE neu tui khong con cho (giu nguyen tren tay).
	BOOL		ReturnHandToBag();
	void		MenuSetMouseItem();
	void		RemoveAllInOneRoom(int nRoom);
	void		LockOperation();										// �����ͻ��˶�װ���Ĳ���
	void		UnlockOperation();
	BOOL		IsLockOperation() { return m_bLockOperation; };
	int			GetSameDetailItemNum(int nImmediatePos);
	int			GetGoldColor(int nSet,int nId);
#endif
};
#endif

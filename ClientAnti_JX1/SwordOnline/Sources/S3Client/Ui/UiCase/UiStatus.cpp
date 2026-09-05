/*****************************************************************************************
//	����--״̬����
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-9-2
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "KEngine.h"				// g_DebugLog (chan doan tam)
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiStatus.h"
#include "UiPlayerBar.h"
#include "UiUnlockBox.h"
#include "UiChangeAvatar.h"
#include "../ShortcutKey.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/gamedatadef.h"
#include "../UiBase.h"
#include "UiTradeConfirmWnd.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI		"UiStatus.ini"

KUiStatus* KUiStatus::m_pSelf = NULL;

// -------------------------------------------------------------------------
// ---> �����ؼ���UIEP_*�Լ��ɽ�����Ʒ�����͵Ķ�Ӧ��ϵ
static struct UE_CTRL_MAP
{
	int				nPosition;
	const char*		pIniSection;
}CtrlItemMap[_ITEM_COUNT] =
{
	{ UIEP_HEAD,		"Cap"		},	//װ��-ñ��
	{ UIEP_HAND,		"Weapon"	},	//װ��-����
	{ UIEP_NECK,		"Necklace"	},	//װ��-����
	{ UIEP_FINESSE,		"Bangle"	},	//װ��-����
	{ UIEP_BODY,		"Cloth"		},	//װ��-�·�
	{ UIEP_WAIST,		"Sash"		},	//װ��-����
	{ UIEP_FINGER1,		"Ring1"		},	//װ��-��ָ
	{ UIEP_FINGER2,		"Ring2"		},	//װ��-��ָ
	{ UIEP_WAIST_DECOR,	"Pendant"	},	//װ��-��׺
	{ UIEP_FOOT,		"Shoes"		},	//װ��-Ь��
	{ UIEP_HORSE,		"Horse"		},	//װ��-��
	{ UIEP_MASK,		"Mask"		}	//װ��-���	// mat na
//	{ UIEP_QUANAN,		"QuanAn"	},	//quan an
//	{ UIEP_TRANGSUC,	"TrangSuc"	},	//trang suc
//	{ UIEP_PHIPHONG,	"PhiPhong"	}	//phi phong
};


//--------------------------------------------------------------------------
//	���ܣ��������������ʾ���򷵻�ʵ��ָ��
//--------------------------------------------------------------------------
KUiStatus* KUiStatus::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	���ܣ��򿪴��ڣ�����Ψһ��һ�������ʵ��
//--------------------------------------------------------------------------
KUiStatus* KUiStatus::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiStatus;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	���ܣ��رմ��ڣ�ͬʱ����ѡ���Ƿ�ɾ������ʵ��
//--------------------------------------------------------------------------
void KUiStatus::CloseWindow(bool bDestroy)
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
//	���ܣ���ʼ��
//--------------------------------------------------------------------------
void KUiStatus::Initialize()
{
	AddChild(&m_Agname);
	AddChild(&m_Name);
	AddChild(&m_Title);

	//AddChild(&m_Money);

	AddChild(&m_Life);
	AddChild(&m_Mana);
	AddChild(&m_Stamina);
	AddChild(&m_Angry);
	AddChild(&m_Experience);

	AddChild(&m_RemainPoint);
	AddChild(&m_Strength);
	AddChild(&m_Dexterity);
	AddChild(&m_Vitality);
	AddChild(&m_Energy);

	AddChild(&m_AddStrength);
	AddChild(&m_AddDexterity);
	AddChild(&m_AddVitality);
	AddChild(&m_AddEnergy);

	AddChild(&m_LeftDamage);
	AddChild(&m_RightDamage);
	AddChild(&m_Attack);
	AddChild(&m_Defence);
	AddChild(&m_MoveSpeed);
	AddChild(&m_AttackSpeed);

	AddChild(&m_PhyDef);
	AddChild(&m_CoolDef);
	AddChild(&m_LightDef);
	AddChild(&m_FireDef);
	AddChild(&m_PoisonDef);
	AddChild(&m_Level);
	AddChild(&m_StatusDesc);

	AddChild(&m_nPk);
	AddChild(&m_nRepute);
	AddChild(&m_nFuYuan);
	AddChild(&m_nPaiMing);
	AddChild(&m_AvatarText);	
	AddChild(&m_Avatar);
	AddChild(&m_UnlockBtn);
	AddChild(&Dinh);
	AddChild(&Thao);
	AddChild(&m_ReBorn);

	for (int i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].SetObjectGenre(CGOG_ITEM);
		AddChild(&m_EquipBox[i]);
		m_EquipBox[i].SetContainerId((int)UOC_EQUIPTMENT);
	}

	AddChild(&m_OpenItemPad);
	AddChild(&m_Close);

	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
}

//--------------------------------------------------------------------------
//	���ܣ����봰�ڵĽ��淽��
//--------------------------------------------------------------------------
void KUiStatus::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
			m_pSelf->LoadScheme(&Ini);	
	}
}

//������淽��
void KUiStatus::LoadScheme(class KIniFile* pIni)
{
	if (g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, 0))
	{
		Init(pIni, "Male");
	}
	else
	{	Init(pIni, "Female");
	}
	
	m_AvatarText  .Init(pIni, "AvatarText");	
	m_AvatarText  .SetText("Nh�n v�o ��y �� ch�n ch�n dung nh�n v�t");
	m_Avatar		.Init(pIni, "Avatar");
	m_Face    .Init(pIni, "Face");
	m_Agname  .Init(pIni, "Agname");
	m_Name    .Init(pIni, "Name");
	m_Title   .Init(pIni, "Title");

	m_Money  .Init(pIni, "Money");

	m_Life		.Init(pIni, "Life");
	m_Mana		.Init(pIni, "Mana");
	m_Stamina	.Init(pIni, "Stamina");
	m_Angry		.Init(pIni, "Angry");		
	m_Experience.Init(pIni, "Exp");

	m_RemainPoint.Init(pIni, "RemainPoint");
	m_Strength   .Init(pIni, "Strength");
	m_Dexterity  .Init(pIni, "Dexterity");
	m_Vitality   .Init(pIni, "Vitality");
	m_Energy     .Init(pIni, "Energy");
		
	m_AddStrength .Init(pIni, "AddStrength");
	m_AddDexterity.Init(pIni, "AddDexterity");
	m_AddVitality .Init(pIni, "AddVitality");
	m_AddEnergy   .Init(pIni, "AddEnergy");
		
	m_LeftDamage .Init(pIni, "LeftDamage");
	m_RightDamage.Init(pIni, "RightDamage");
	m_Attack     .Init(pIni, "Attack");
	m_Defence    .Init(pIni, "Defense");
	m_MoveSpeed  .Init(pIni, "MoveSpeed");
	m_AttackSpeed.Init(pIni, "AttackSpeed");

	m_PhyDef	 .Init(pIni, "ResistPhy");
	m_CoolDef    .Init(pIni, "ResistCold");
	m_LightDef   .Init(pIni, "ResistLighting");
	m_FireDef    .Init(pIni, "ResistFire");
	m_PoisonDef  .Init(pIni, "ResistPoison");

	m_Level		 .Init(pIni, "Level");
	m_StatusDesc .Init(pIni, "Status");
	m_Close	 .Init(pIni, "Close");

	m_nPk        .Init(pIni, "PKValue");
	m_nRepute    .Init(pIni, "Prestige");
	m_nFuYuan    .Init(pIni, "FuYuan");
	m_nPaiMing   .Init(pIni, "WorldRank");
	m_UnlockBtn.Init(pIni, "BtnLock");
	Dinh.Init(pIni, "DinhBtn");
	Thao.Init(pIni, "ThaoBtn");
	m_ReBorn    .Init(pIni, "TrungSinh");

	m_OpenItemPad.Init(pIni, "Item");

	for (int i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].Init(pIni, CtrlItemMap[i].pIniSection);
	}
}

//--------------------------------------------------------------------------
//	���ܣ����ں���
//--------------------------------------------------------------------------
int KUiStatus::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_Close)
			Hide();
		else if (uParam == (unsigned int)(KWndWindow*)&m_OpenItemPad)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_ITEMS);
		else if (uParam == (unsigned int)(KWndWindow*)&m_UnlockBtn)
		{
			if (g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0))
			{
				// Ruong dang mo - Khoa ruong lai
				g_pCoreShell->OperationRequest(GOI_CP_LOCK, 0, 0);
			}
			else
			{
				// Ruong dang khoa ~> Mo cua so nhap pass ruong
				KUiUnlockBox::OpenWindow();
			}
		}	
		else if (uParam == (unsigned int)(KWndWindow*)&Dinh)
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, HT_CN, 0);
		else if (uParam == (unsigned int)(KWndWindow*)&Thao)
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, HT_CN, 0);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Avatar)
				KUiChangeAvatar::OpenWindow();
		else if (m_nRemainPoint > 0)
		{
			if (uParam == (unsigned int)(KWndWindow*)&m_AddStrength)
				UseRemainPoint(UIPA_STRENGTH);
			else if (uParam == (unsigned int)(KWndWindow*)&m_AddDexterity)
				UseRemainPoint(UIPA_DEXTERITY);
			else if (uParam == (unsigned int)(KWndWindow*)&m_AddVitality)
				UseRemainPoint(UIPA_VITALITY);
			else if (uParam == (unsigned int)(KWndWindow*)&m_AddEnergy)
				UseRemainPoint(UIPA_ENERGY);
		}
		break;
	case WND_N_ITEM_PICKDROP:
		g_DebugLog("[EQUIP-GATE] chest=%d status=%d moveEnable=%d pick=%d drop=%d",
			g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0),
			(int)g_UiBase.GetStatus(),
			(int)g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM),
			uParam ? 1 : 0, nParam ? 1 : 0);
		if (g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0))
		{
		if (g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) || g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR)
			OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
		}
		g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, CN_GH, 0);
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	���ܣ�����ĳ������
//--------------------------------------------------------------------------
void KUiStatus::UseRemainPoint(UI_PLAYER_ATTRIBUTE type)
{
	g_pCoreShell->OperationRequest(GOI_TONE_UP_ATTRIBUTE, type, 0);
	m_nRemainPoint --;
	m_RemainPoint.SetIntText(m_nRemainPoint);
	m_AddStrength.Enable(m_nRemainPoint);
	m_AddDexterity.Enable(m_nRemainPoint);
	m_AddVitality.Enable(m_nRemainPoint);
	m_AddEnergy.Enable(m_nRemainPoint);
}

//--------------------------------------------------------------------------
//	���ܣ����»������ݣ������Ȳ��ױ����ݣ�
//--------------------------------------------------------------------------
void KUiStatus::UpdateBaseData()
{
	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, 0);
	m_Agname.SetText(Info.Agname);
	m_Name  .SetText(Info.Name);
	m_Title .SetText(Info.Title);
}

void KUiStatus::UpdateRuntimeInfo(KUiPlayerRuntimeInfo* pInfo)
{
	if (pInfo)
	{
		m_Life		.Set2IntText(pInfo->nLife, pInfo->nLifeFull, '/');
		m_Mana		.Set2IntText(pInfo->nMana, pInfo->nManaFull, '/');
		m_Stamina	.Set2IntText(pInfo->nStamina, pInfo->nStaminaFull, '/');
		m_Angry		.Set2IntText(pInfo->nAngry, pInfo->nAngryFull, '/');
		m_Experience.Set2IntText(pInfo->nExperience, pInfo->nExperienceFull, '/');
		//Info.byAction & PA_RIDE
	}
}

//--------------------------------------------------------------------------
//	���ܣ���������
//--------------------------------------------------------------------------
void KUiStatus::UpdateData()
{
	UpdateAllEquips();
	UpdateBaseData();
}

void KUiStatus::UpdateAllEquips()
{
	KUiObjAtRegion	Equips[_ITEM_COUNT];
	int nCount = g_pCoreShell->GetGameData(GDI_EQUIPMENT, (unsigned int)&Equips, 0);
	int	i;
	for (i = 0; i < _ITEM_COUNT; i++)
		m_EquipBox[i].Celar();
	for (i = 0; i < nCount; i++)
	{
		if (Equips[i].Obj.uGenre != CGOG_NOTHING)
			UpdateEquip(&Equips[i], true);
	}
}

void KUiStatus::UpdateRuntimeAttribute(KUiPlayerAttribute* pInfo)
{
	if (pInfo)	
	{
		//m_Money.SetMoneyText(pInfo->nMoney);

		m_nRemainPoint = pInfo->nBARemainPoint;
		m_AddStrength.Enable(m_nRemainPoint);
		m_AddDexterity.Enable(m_nRemainPoint);
		m_AddVitality.Enable(m_nRemainPoint);
		m_AddEnergy.Enable(m_nRemainPoint);
		m_RemainPoint.SetIntText(pInfo->nBARemainPoint);
		m_Strength   .SetIntText(pInfo->nStrength);
		m_Dexterity  .SetIntText(pInfo->nDexterity);
		m_Vitality   .SetIntText(pInfo->nVitality);
		m_Energy     .SetIntText(pInfo->nEnergy);

		m_LeftDamage.Set2IntText(pInfo->nKillMIN,pInfo->nKillMAX,'/');
		m_RightDamage.Set2IntText(pInfo->nRightKillMin, pInfo->nRightKillMax, '/');
		m_Attack.SetIntText(pInfo->nAttack);
		m_Defence.SetIntText(pInfo->nDefence);
		m_MoveSpeed.SetIntText(pInfo->nMoveSpeed);
		m_AttackSpeed.Set2IntText(pInfo->nAttackSpeed,pInfo->nCastSpeed, '/');

		char	TextInfo[32];
		sprintf(TextInfo, "%d%%", pInfo->nPhyDef);
		m_PhyDef	.SetText(TextInfo);
		sprintf(TextInfo, "%d%%", pInfo->nCoolDef);
		m_CoolDef  .SetText(TextInfo);
		sprintf(TextInfo, "%d%%", pInfo->nLightDef);
		m_LightDef .SetText(TextInfo);
		sprintf(TextInfo, "%d%%", pInfo->nFireDef);
		m_FireDef  .SetText(TextInfo);
		sprintf(TextInfo, "%d%%", pInfo->nPoisonDef);
		m_PoisonDef.SetText(TextInfo);

		m_Level.SetIntText(pInfo->nLevel);			//�ȼ�
		m_StatusDesc.SetText(pInfo->StatusDesc);
		m_nPk.SetIntText(pInfo->nPKValue);
		m_nRepute.SetIntText(pInfo->nRepute);
		m_nFuYuan.SetIntText(pInfo->nFuYuan);
		m_nPaiMing.Set5IntText(pInfo->nRankInWorld);
		m_ReBorn .Set4IntText(pInfo->nReBorn);
	}
}

//--------------------------------------------------------------------------
//	���ܣ���Ӧ�����������װ���ĸı�
//--------------------------------------------------------------------------
void KUiStatus::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	g_DebugLog("[EQUIP-DBG] OnEquiptChanged eStatus=%d pick=%d drop=%d",
		(int)eStatus, pPickPos ? 1 : 0, pDropPos ? 1 : 0);
	if (pPickPos)
	{
		//_ASSERT(pPickPos->pWnd);
		((KWndObjectBox*)(pPickPos->pWnd))->GetObject(Obj);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = 0;
		Pick.eContainer = UOC_EQUIPTMENT;
		pWnd = pPickPos->pWnd;

	}
	else if (pDropPos)
	{
		pWnd = pDropPos->pWnd;
	}
	else
		return;

	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = 0;
		Drop.eContainer = UOC_EQUIPTMENT;
	}

	for (int i = 0; i < _ITEM_COUNT; i++)
	{
		if (pWnd == (KWndWindow*)&m_EquipBox[i])
		{
			Drop.Region.v = Pick.Region.v = CtrlItemMap[i].nPosition;
			break;
		}
	}
	if (eStatus == UIS_S_TRADE_REPAIR) // doan da fix
	{
		KUiItemBuySelInfo	Price = { 0 };
		{
			if (g_pCoreShell->GetGameData(GDI_REPAIR_ITEM_PRICE,
				(unsigned int)(&Pick), (int)(&Price)))
			{
				KUiTradeConfirm::OpenWindow(&Pick, &Price, TCA_REPAIR);
			}
		}
		
	}
	else if (eStatus == UIS_S_TRADE_SALE && pPickPos && !pDropPos)
	{
		// Ban: KHONG ban truc tiep trang bi DANG MAC - phai thao ra truoc. Truoc
		// day click do dang mac o che do Ban roi vao SWITCH_OBJECT -> thao ra
		// dinh len con tro (lo do). Chan tai day: khong nhac len.
	}
	else
	{
		//_ASSERT(i < _ITEM_COUNT);
		g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
	}

}

//--------------------------------------------------------------------------
//	���ܣ�װ���仯����
//--------------------------------------------------------------------------
void KUiStatus::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < _ITEM_COUNT; i++)
		{
			if (CtrlItemMap[i].nPosition == pEquip->Region.v)
			{
				if (bAdd)
					m_EquipBox[i].HoldObject(pEquip->Obj.uGenre, pEquip->Obj.uId,
						pEquip->Region.Width, pEquip->Region.Height);
				else
					m_EquipBox[i].HoldObject(CGOG_NOTHING, 0, 0, 0);
				break;
			}
		}
	}
}

/*****************************************************************************************
//	Bang F3: trang thai nhan vat dang tab. Xem UiStatus.h.
//	Copyright : Kingsoft 2002 (ban goc Wooy); chia tab 14/09/2026.
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
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

#define	SCHEME_INI			"UiStatus.ini"
#define	SCHEME_INI_ATTRIB	"UiStatusAttrib.ini"
#define	SCHEME_INI_EQUIP	"UiStatusEquip.ini"

KUiStatus* KUiStatus::m_pSelf = NULL;

// O trang bi <-> vi tri trang bi cua core (UIEP_*) <-> muc trong UiStatusEquip.ini
static struct UE_CTRL_MAP
{
	int				nPosition;
	const char*		pIniSection;
}CtrlItemMap[_ITEM_COUNT] =
{
	{ UIEP_HEAD,		"Cap"		},
	{ UIEP_HAND,		"Weapon"	},
	{ UIEP_NECK,		"Necklace"	},
	{ UIEP_FINESSE,		"Bangle"	},
	{ UIEP_BODY,		"Cloth"		},
	{ UIEP_WAIST,		"Sash"		},
	{ UIEP_FINGER1,		"Ring1"		},
	{ UIEP_FINGER2,		"Ring2"		},
	{ UIEP_WAIST_DECOR,	"Pendant"	},
	{ UIEP_FOOT,		"Shoes"		},
	{ UIEP_HORSE,		"Horse"		},
	{ UIEP_MASK,		"Mask"		},
	{ UIEP_MANTLE,		"Mantle"	},	// phi phong
	{ UIEP_SIGNET,		"Signet"	},	// an
	{ UIEP_SHIPIN,		"Shipin"	}	// trang suc
};

int KUiStatusPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	if ((uMsg == WND_N_ITEM_PICKDROP || uMsg == WND_N_LEFT_CLICK_ITEM ||
		uMsg == WND_N_RIGHT_CLICK_ITEM) && m_pParentWnd)
		return m_pParentWnd->WndProc(uMsg, uParam, nParam);
	return KWndPage::WndProc(uMsg, uParam, nParam);
}

KUiStatus* KUiStatus::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

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
		// HOI may chu chan dung dang luu (so 0 = hoi, xem GOI_SET_AVATAR).
		// Khong the chi trong cho cu day luc vao the gioi: luc do client con
		// dang dung canh, goi den thuong bi bo (xem m_nConBaoNpc ben may chu).
		if (g_pCoreShell)
			g_pCoreShell->OperationRequest(GOI_SET_AVATAR, 0, 0);
		m_pSelf->UpdateData();
		m_pSelf->BringToTop();
		m_pSelf->Show();
	}
	return m_pSelf;
}

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

void KUiStatus::Initialize()
{
	// ---- trang Thuoc tinh ----
	m_AttribPage.AddChild(&m_Face);		// phai them thi moi duoc ve
	m_nAvatarVe = -1;
	m_AttribPage.AddChild(&m_AvatarText);
	m_AttribPage.AddChild(&m_Avatar);
	m_AttribPage.AddChild(&m_Name);
	m_AttribPage.AddChild(&m_Title);
	m_AttribPage.AddChild(&m_nRepute);
	m_AttribPage.AddChild(&m_nFuYuan);
	m_AttribPage.AddChild(&m_Level);
	m_AttribPage.AddChild(&m_nPaiMing);
	m_AttribPage.AddChild(&m_Life);
	m_AttribPage.AddChild(&m_Mana);
	m_AttribPage.AddChild(&m_Stamina);
	m_AttribPage.AddChild(&m_StatusDesc);
	m_AttribPage.AddChild(&m_Strength);
	m_AttribPage.AddChild(&m_Dexterity);
	m_AttribPage.AddChild(&m_Vitality);
	m_AttribPage.AddChild(&m_Energy);
	m_AttribPage.AddChild(&m_AddStrength);
	m_AttribPage.AddChild(&m_AddDexterity);
	m_AttribPage.AddChild(&m_AddVitality);
	m_AttribPage.AddChild(&m_AddEnergy);
	m_AttribPage.AddChild(&m_Experience);
	m_AttribPage.AddChild(&m_LeftDamage);
	m_AttribPage.AddChild(&m_RightDamage);
	m_AttribPage.AddChild(&m_Attack);
	m_AttribPage.AddChild(&m_Defence);
	m_AttribPage.AddChild(&m_MoveSpeed);
	m_AttribPage.AddChild(&m_AttackSpeed);
	m_AttribPage.AddChild(&m_RemainPoint);
	m_AttribPage.AddChild(&m_PhyDef);
	m_AttribPage.AddChild(&m_CoolDef);
	m_AttribPage.AddChild(&m_LightDef);
	m_AttribPage.AddChild(&m_FireDef);
	m_AttribPage.AddChild(&m_PoisonDef);

	// ---- trang Trang bi ----
	m_EquipPage.AddChild(&m_EquipName);
	m_EquipPage.AddChild(&m_nPk);
	m_EquipPage.AddChild(&m_ReBorn);
	for (int i = 0; i < _ITEM_COUNT; i ++)
	{
		m_EquipBox[i].SetObjectGenre(CGOG_ITEM);
		m_EquipPage.AddChild(&m_EquipBox[i]);
		m_EquipBox[i].SetContainerId((int)UOC_EQUIPTMENT);
	}
	m_EquipPage.AddChild(&m_UnlockBtn);

	m_MeridianPage.Initialize();

	AddPage(&m_AttribPage, &m_BtnAttribPage);
	AddPage(&m_EquipPage, &m_BtnEquipPage);
	AddPage(&m_MeridianPage, &m_BtnMeridianPage);
	AddChild(&m_Close);

	Wnd_AddWindow(this);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadPages(Scheme);
}

void KUiStatus::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
		m_pSelf->LoadPages(pScheme);
}

void KUiStatus::LoadPages(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
	if (Ini.Load(Buff))
	{
		Init(&Ini, "Main");
		m_BtnAttribPage.Init(&Ini, "BtnAttribPage");
		m_BtnEquipPage.Init(&Ini, "BtnEquipPage");
		m_BtnMeridianPage.Init(&Ini, "BtnMeridianPage");
		m_Close.Init(&Ini, "Close");
		Ini.Clear();
	}

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_ATTRIB);
	if (Ini.Load(Buff))
	{
		m_AttribPage.Init(&Ini, "Main");
		m_Face		.Init(&Ini, "Face");
		m_Avatar	.Init(&Ini, "Avatar");
		m_AvatarText.Init(&Ini, "AvatarText");
		// Chu co dau de ben .ini; nguon chi giu ASCII.
		m_AvatarText.SetText("Bam vao day de chon chan dung");
		m_Name		.Init(&Ini, "Name");
		m_Title		.Init(&Ini, "Title");
		m_nRepute	.Init(&Ini, "Prestige");
		m_nFuYuan	.Init(&Ini, "Luck");
		m_Level		.Init(&Ini, "Level");
		m_nPaiMing	.Init(&Ini, "WorldRank");
		m_Life		.Init(&Ini, "Life");
		m_Mana		.Init(&Ini, "Mana");
		m_Stamina	.Init(&Ini, "Stamina");
		m_StatusDesc.Init(&Ini, "Status");
		m_Strength	.Init(&Ini, "Strength");
		m_Vitality	.Init(&Ini, "Vitality");
		m_Dexterity	.Init(&Ini, "Dexterity");
		m_Energy	.Init(&Ini, "Energy");
		m_AddStrength .Init(&Ini, "AddStrength");
		m_AddVitality .Init(&Ini, "AddVitality");
		m_AddDexterity.Init(&Ini, "AddDexterity");
		m_AddEnergy	  .Init(&Ini, "AddEnergy");
		m_Experience.Init(&Ini, "Exp");
		m_LeftDamage.Init(&Ini, "LeftDamage");
		m_RightDamage.Init(&Ini, "RightDamage");
		m_Attack	.Init(&Ini, "Attack");
		m_Defence	.Init(&Ini, "Defense");
		m_MoveSpeed	.Init(&Ini, "MoveSpeed");
		m_AttackSpeed.Init(&Ini, "AttackSpeed");
		m_RemainPoint.Init(&Ini, "RemainPoint");
		m_PhyDef	.Init(&Ini, "ResistPhy");
		m_CoolDef	.Init(&Ini, "ResistCold");
		m_LightDef	.Init(&Ini, "ResistLighting");
		m_FireDef	.Init(&Ini, "ResistFire");
		m_PoisonDef	.Init(&Ini, "ResistPoison");
		Ini.Clear();
	}

	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_EQUIP);
	if (Ini.Load(Buff))
	{
		// Nen nguoi nam / nu cua trang trang bi
		if (g_pCoreShell && g_pCoreShell->GetGameData(GDI_PLAYER_IS_MALE, 0, 0))
			m_EquipPage.Init(&Ini, "Male");
		else
			m_EquipPage.Init(&Ini, "Female");
		m_EquipName	.Init(&Ini, "Name");
		m_nPk		.Init(&Ini, "PKValue");
		m_ReBorn	.Init(&Ini, "TransLife");
		m_UnlockBtn	.Init(&Ini, "BtnLock");
		for (int i = 0; i < _ITEM_COUNT; i ++)
		{
			if (CtrlItemMap[i].pIniSection)
				m_EquipBox[i].Init(&Ini, CtrlItemMap[i].pIniSection);
		}
		Ini.Clear();
	}

	m_MeridianPage.LoadScheme(pScheme);
}

int KUiStatus::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_Close)
		{
			Hide();
			return 0;
		}
		if (uParam == (unsigned int)(KWndWindow*)&m_UnlockBtn)
		{
			if (g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0))
				g_pCoreShell->OperationRequest(GOI_CP_LOCK, 0, 0);	// dang mo -> khoa lai
			else
				KUiUnlockBox::OpenWindow();							// dang khoa -> hop nhap mat ma
			return 0;
		}
		if (uParam == (unsigned int)(KWndWindow*)&m_Avatar)
		{
			KUiChangeAvatar::OpenWindow();
			return 0;
		}
		if (uParam == (unsigned int)(KWndWindow*)&m_AddStrength ||
			uParam == (unsigned int)(KWndWindow*)&m_AddDexterity ||
			uParam == (unsigned int)(KWndWindow*)&m_AddVitality ||
			uParam == (unsigned int)(KWndWindow*)&m_AddEnergy)
		{
			if (m_nRemainPoint > 0)
			{
				if (uParam == (unsigned int)(KWndWindow*)&m_AddStrength)
					UseRemainPoint(UIPA_STRENGTH);
				else if (uParam == (unsigned int)(KWndWindow*)&m_AddDexterity)
					UseRemainPoint(UIPA_DEXTERITY);
				else if (uParam == (unsigned int)(KWndWindow*)&m_AddVitality)
					UseRemainPoint(UIPA_VITALITY);
				else
					UseRemainPoint(UIPA_ENERGY);
			}
			return 0;
		}
		break;		// nut tab: KWndPageSet doi trang
	case WND_N_ITEM_PICKDROP:
		if (g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0))
		{
			// Chi khi ranh (IDLE) moi duoc thao/xep do dang mac. Rieng che do SUA van
			// chay de con hoi gia sua. Cac che do giao dich khac (ban, mua, doi, dat
			// gia) khong dung toi o trang bi.
			bool bRanh = g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) != 0;
			if (bRanh || g_UiBase.GetStatus() == UIS_S_TRADE_REPAIR)
				OnEquiptChanged((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
			// O trang bi da nhac mon len con tro o tang UI (KWndObjectBox) TRUOC khi
			// bao len day. Ngoai luc ranh thi khong ai nhan mon do nua, phai nha ra -
			// khong nha thi mon ket tren con tro.
			// Chi xet cu NHAC thuan (uParam co, nParam khong): luc dang cam san mot mon
			// thi mon do nam trong tay cua core, nha hinh ve di se thanh ket vo hinh.
			if (!bRanh && uParam && !nParam)
				Wnd_DragFinished();
		}
		else
		{
			// Dang khoa: o trang bi da nhac mon len con tro truoc khi bao len; khong ai
			// nhan nen phai nha hinh keo, khong thi mon dinh con tro. Chi cu nhac thuan.
			if (uParam && !nParam)
				Wnd_DragFinished();
			g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, CN_GH, 0);
		}
		return 0;
	}
	return KWndPageSet::WndProc(uMsg, uParam, nParam);
}

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

void KUiStatus::UpdateBaseData()
{
	KUiPlayerBaseInfo	Info;
	memset(&Info, 0, sizeof(KUiPlayerBaseInfo));
	g_pCoreShell->GetGameData(GDI_PLAYER_BASE_INFO, (int)&Info, 0);
	m_Name     .SetText(Info.Name);
	m_EquipName.SetText(Info.Name);
	m_Title    .SetText(Info.Title);
}

void KUiStatus::UpdateRuntimeInfo(KUiPlayerRuntimeInfo* pInfo)
{
	if (pInfo)
	{
		m_Life		.Set2IntText(pInfo->nLife, pInfo->nLifeFull, '/');
		m_Mana		.Set2IntText(pInfo->nMana, pInfo->nManaFull, '/');
		m_Stamina	.Set2IntText(pInfo->nStamina, pInfo->nStaminaFull, '/');
		m_Experience.Set2IntText(pInfo->nExperience, pInfo->nExperienceFull, '/');
	}
}

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

		m_Level.SetIntText(pInfo->nLevel);
		m_StatusDesc.SetText(pInfo->StatusDesc);
		m_nPk.SetIntText(pInfo->nPKValue);
		m_nRepute.SetIntText(pInfo->nRepute);
		m_nFuYuan.SetIntText(pInfo->nFuYuan);
		m_nPaiMing.Set5IntText(pInfo->nRankInWorld);
		m_ReBorn .Set4IntText(pInfo->nReBorn);
	}
}

// Doc chan dung may chu dang luu va doi anh khi khac cai dang ve.
// So 0 = chua chon: khong ve gi, chi con dong chu "bam vao day de chon".
void KUiStatus::UpdateAvatar()
{
	int nAvatar = 0;
	if (g_pCoreShell)
		nAvatar = g_pCoreShell->GetGameData(GDI_PLAYER_AVATAR, 0, 0);
	if (nAvatar < 0 || nAvatar > defMAX_AVATAR)
		nAvatar = 0;
	if (nAvatar == m_nAvatarVe)
		return;
	m_nAvatarVe = nAvatar;

	char szAnh[128];
	if (nAvatar > 0)
		sprintf(szAnh, "\\spr\\ui3\\chan-dung\\%03d.spr", nAvatar);
	else
		szAnh[0] = 0;
	m_Face.SetImage(ISI_T_SPR, szAnh);
	// Con chua chon thi moi hien loi moi; da co chan dung thi de anh noi thay.
	if (nAvatar == 0)
		m_AvatarText.Show();
	else
		m_AvatarText.Hide();
}

// May chu tra chan dung ve bang goi rieng (s2c_setavatar) chu khong nam trong
// goi thong tin nhan vat, nen khong co cho nao "bao cho bang F3 biet". Doi chieu
// moi khung khi ve: chi la so sanh mot so nguyen, chi doi anh khi that su khac.
void KUiStatus::PaintWindow()
{
	UpdateAvatar();
	if (g_pCoreShell)	// nhan nut Khoa theo trang thai that, xem ruong
		m_UnlockBtn.CheckButton(!g_pCoreShell->GetGameData(GDI_IS_CHEST_UNLOCKED, 0, 0));
	KWndPageSet::PaintWindow();
}

void KUiStatus::OnEquiptChanged(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	KUiObjAtContRegion	Drop, Pick;
	KUiDraggedObject	Obj;
	KWndWindow*			pWnd = NULL;

	UISYS_STATUS	eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
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
	if (eStatus == UIS_S_TRADE_REPAIR)
	{
		// SUA do DANG MAC: mo hop xac nhan CHI khi con sua duoc (do ben da hao).
		// GetGameData tra ve CanBeRepaired(); do ben con day -> im lang.
		KUiItemBuySelInfo	Price = { 0 };
		if (g_pCoreShell->GetGameData(GDI_REPAIR_ITEM_PRICE,
			(unsigned int)(&Pick), (int)(&Price)))
		{
			KUiTradeConfirm::OpenWindow(&Pick, &Price, TCA_REPAIR);
		}
	}
	else
	{
		// Chi den day khi ranh: thao do ra / xep lai (cong o WndProc da loc).
		g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
	}
}

void KUiStatus::UpdateEquip(KUiObjAtRegion* pEquip, int bAdd)
{
	if (pEquip)
	{
		for (int i = 0; i < _ITEM_COUNT; i++)
		{
			if (CtrlItemMap[i].pIniSection && CtrlItemMap[i].nPosition == pEquip->Region.v)
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

#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../elem/wnds.h"
#include "UiItem.h"
#include "UiCompoundItem.h"
#include "UiSysMsgCentre.h"
#include "../../../core/src/coreshell.h"
#include "../UiBase.h"
#include "crtdbg.h"
#include "../UiSoundSetting.h"

#include "../../../Represent/iRepresent/iRepresentShell.h"
extern iRepresentShell*	g_pRepresentShell;

extern iCoreShell*		g_pCoreShell;

KUiComItem* KUiComItem::m_pSelf = NULL;

#define 	SCHEME_INI_SHEET			"Khamnammain.ini"
#define 	SCHEME_INI_COMPE			"Tinhluyen.ini"
#define 	SCHEME_INI_BUILD			"Chetao.ini"
#define 	SCHEME_INI_OUTIN			"Lay.ini"
#define 	SCHEME_INI_DOPHO			"chetaodohk.ini"

void KUiComItem::LoadScheme( const char* pScheme )
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_SHEET);
	if (m_pSelf && Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_CompoundPadBtn.Init(&Ini, "CompoundBtn");
		m_pSelf->m_DistillPadBtn.Init(&Ini, "DistillBtn");
		m_pSelf->m_ForgePadBtn.Init(&Ini, "ForgeBtn");
		m_pSelf->m_EnchasePadBtn.Init(&Ini, "EnchaseBtn");
		m_pSelf->m_AtlasPadBtn.Init(&Ini, "AtlasBtn");
		m_pSelf->m_UpCryoliteBtn.Init(&Ini, "CryoliteBtn");
		m_pSelf->m_UpPropMineBtn.Init(&Ini, "PropMineBtn");

		m_pSelf->m_Close           .Init(&Ini, "CloseBtn");
		
		m_pSelf->m_CompoundPad.LoadScheme(pScheme);
	}	
}

void KUiComItem::Initialize()
{
	AddChild(&m_CompoundPadBtn);
	AddChild(&m_DistillPadBtn);
	AddChild(&m_EnchasePadBtn);
	AddChild(&m_ForgePadBtn);
	AddChild(&m_AtlasPadBtn);
	AddChild(&m_UpCryoliteBtn);
	AddChild(&m_UpPropMineBtn);

	AddChild(&m_Close);
	
	m_CompoundPad.Initialize();
	AddPage(&m_CompoundPad,&m_CompoundPadBtn);

	m_DistillPad.Initialize();
	AddPage(&m_DistillPad,&m_DistillPadBtn);

	m_ForgePad.Initialize();
	AddPage(&m_ForgePad,&m_ForgePadBtn);

	m_EnchasePad.Initialize();
	AddPage(&m_EnchasePad,&m_EnchasePadBtn);

	m_AtlasPad.Initialize();
	AddPage(&m_AtlasPad,&m_AtlasPadBtn);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	Wnd_AddWindow(this);
}

/*********************************************************************
* ¹¦ÄÜ£º´ò¿ª´°¿Ú
**********************************************************************/
KUiComItem* KUiComItem::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiComItem;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->m_nStatus = STATUS_WAITING_MATERIALS;
		m_pSelf->m_nNum = WINDOWS_COMP;
		m_pSelf->m_CompoundPadBtn.CheckButton(TRUE);
		m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
		m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
		m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
		m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
		m_pSelf->m_UpCryoliteBtn.Hide();
		m_pSelf->m_UpPropMineBtn.Hide();
		m_pSelf->BringToTop();
		m_pSelf->Show();

		if (KUiItem::GetIfVisible() == NULL)
			KUiItem::OpenWindow();
		else
			UiSoundPlay(UI_SI_WND_OPENCLOSE);
		KUiItem::OnNpcTradeMode(true);
	}
	return m_pSelf;
}


/*********************************************************************
* ¹¦ÄÜ£ºÈç¹û´°¿ÚÕý±»ÏÔÊ¾£¬Ôò·µ»ØÊµÀýÖ¸Õë
**********************************************************************/
KUiComItem* KUiComItem::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}


/*********************************************************************
* ¹¦ÄÜ£º¹Ø±Õ´°¿Ú£¬Í¬Ê±¿ÉÒÔÑ¡ÔòÊÇ·ñÉ¾³ý¶ÔÏóÊµÀý
**********************************************************************/
void KUiComItem::CloseWindow(bool bDestory)
{
	if (m_pSelf)
	{
		m_pSelf->Hide();
		if (bDestory)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
	}
	KUiItem::OnNpcTradeMode(false);
}

int KUiComItem::WndProc( unsigned int uMsg, unsigned int uParam, int nParam )
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_Close)
		{
			CloseWindow();
		}
		else if(uParam == (unsigned int)&m_UpPropMineBtn && m_UpCryoliteBtn.IsVisible())
		{
			m_nNum = WINDOWS_COMP3;
			ShowWindow(0);
			m_CompoundPad.SetPosText(3);
		}
		else if(uParam == (unsigned int)&m_UpCryoliteBtn && m_UpCryoliteBtn.IsVisible())
		{
			m_nNum = WINDOWS_COMP2;
			ShowWindow(0);
			m_CompoundPad.SetPosText(2);
		}
		else if(uParam == (unsigned int)&m_CompoundPadBtn && m_UpCryoliteBtn.IsVisible())
		{
			m_nNum = WINDOWS_COMP;
			ShowWindow(0);
			m_CompoundPad.SetPosText(1);
		}
		else if (uParam == (unsigned int)&m_CompoundPadBtn && !m_UpCryoliteBtn.IsVisible())
		{
			m_CompoundPadBtn.CheckButton(TRUE);
			int nX,nY;
			m_CompoundPadBtn.GetPosition(&nX,&nY);
			m_UpCryoliteBtn.SetPosition(nX,nY + 19);
			m_UpPropMineBtn.SetPosition(nX,nY + 38);
			m_UpCryoliteBtn.BringToTop();
			m_UpPropMineBtn.BringToTop();
			m_UpCryoliteBtn.Show();
			m_UpPropMineBtn.Show();
		}
		else if (uParam == (unsigned int)&m_DistillPadBtn)
		{
			m_nNum = WINDOWS_DISTill;
			ShowWindow(1);
		}
		else if (uParam == (unsigned int)&m_ForgePadBtn)
		{
			m_nNum = WINDOWS_FORG;
			ShowWindow(2);
		}
		else if (uParam == (unsigned int)&m_EnchasePadBtn)
		{
			m_nNum = WINDOWS_ENCHASE;
			ShowWindow(3);
		}
		else if (uParam == (unsigned int)&m_AtlasPadBtn)
		{
			m_nNum = WINDOWS_ATLAS;
			ShowWindow(4);
		}
		break;
		
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 1;
}

void KUiComItem::ShowWindow( int nNum /*= 0*/ )
{
	m_pSelf->CleanItem();
	switch(nNum)
	{
	case 0:
		m_pSelf->m_CompoundPad.Show();
		m_pSelf->m_CompoundPadBtn.CheckButton(TRUE);
		m_pSelf->m_UpCryoliteBtn.CheckButton(FALSE);
		m_pSelf->m_UpPropMineBtn.CheckButton(FALSE);
		m_pSelf->m_UpCryoliteBtn.Hide();
		m_pSelf->m_UpPropMineBtn.Hide();
		m_pSelf->m_DistillPad.Hide();
		m_pSelf->m_EnchasePad.Hide();
		m_pSelf->m_ForgePad.Hide();
		m_pSelf->m_AtlasPad.Hide();
		m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
		m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
		m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
		m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
		break;
	case 1:
		m_pSelf->m_DistillPad.Show();
		m_pSelf->m_DistillPadBtn.CheckButton(TRUE);
		m_pSelf->m_UpCryoliteBtn.Hide();
		m_pSelf->m_UpPropMineBtn.Hide();
		m_pSelf->m_CompoundPad.Hide();
		m_pSelf->m_EnchasePad.Hide();
		m_pSelf->m_ForgePad.Hide();
		m_pSelf->m_AtlasPad.Hide();
		m_pSelf->m_CompoundPadBtn.CheckButton(FALSE);
		m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
		m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
		m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
		break;
	case 2:
		m_pSelf->m_ForgePad.Show();
		m_pSelf->m_ForgePadBtn.CheckButton(TRUE);
		m_pSelf->m_UpCryoliteBtn.Hide();
		m_pSelf->m_UpPropMineBtn.Hide();
		m_pSelf->m_DistillPad.Hide();
		m_pSelf->m_EnchasePad.Hide();
		m_pSelf->m_CompoundPad.Hide();
		m_pSelf->m_AtlasPad.Hide();
		m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
		m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
		m_pSelf->m_CompoundPadBtn.CheckButton(FALSE);
		m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
		break;
	case 3:
		m_pSelf->m_EnchasePad.Show();
		m_pSelf->m_EnchasePadBtn.CheckButton(TRUE);
		m_pSelf->m_UpCryoliteBtn.Hide();
		m_pSelf->m_UpPropMineBtn.Hide();
		m_pSelf->m_DistillPad.Hide();
		m_pSelf->m_CompoundPad.Hide();
		m_pSelf->m_ForgePad.Hide();
		m_pSelf->m_AtlasPad.Hide();
		m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
		m_pSelf->m_CompoundPadBtn.CheckButton(FALSE);
		m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
		m_pSelf->m_AtlasPadBtn.CheckButton(FALSE);
		break;
	case 4:
		m_pSelf->m_AtlasPad.Show();
		m_pSelf->m_AtlasPadBtn.CheckButton(TRUE);
		m_pSelf->m_UpCryoliteBtn.Hide();
		m_pSelf->m_UpPropMineBtn.Hide();
		m_pSelf->m_DistillPad.Hide();
		m_pSelf->m_EnchasePad.Hide();
		m_pSelf->m_ForgePad.Hide();
		m_pSelf->m_CompoundPad.Hide();
		m_pSelf->m_DistillPadBtn.CheckButton(FALSE);
		m_pSelf->m_EnchasePadBtn.CheckButton(FALSE);
		m_pSelf->m_ForgePadBtn.CheckButton(FALSE);
		m_pSelf->m_CompoundPadBtn.CheckButton(FALSE);
		break;
	}
}

void KUiComItem::UpdateItem( KUiObjAtRegion* pItem, int bAdd )
{

	KUiDraggedObject	obj;
	obj.uGenre = pItem->Obj.uGenre;
	obj.uId = pItem->Obj.uId;
	obj.DataX = pItem->Region.h;
	obj.DataY = pItem->Region.v;
	obj.DataW = pItem->Region.Width;
	obj.DataH = pItem->Region.Height;

	switch(m_nNum)
	{
	case WINDOWS_COMP:
		{
			m_CompoundPad.UpdateItem(&obj,bAdd);
		}
		break;
	case WINDOWS_COMP2:
		{
			m_CompoundPad.UpdateItem(&obj,4);
		}
		break;
	case WINDOWS_COMP3:
		{
			m_CompoundPad.UpdateItem(&obj,4);
		}
		break;
	case WINDOWS_DISTill:
		{
			m_DistillPad.UpdateItem(&obj,bAdd);
		}
		break;
	case WINDOWS_FORG:
		{
			m_ForgePad.UpdateItem(&obj,bAdd);
		}
		break;
	case WINDOWS_ENCHASE:
		{
			m_EnchasePad.UpdateItem(&obj,bAdd);
		}
		break;
	case WINDOWS_ATLAS:
		{
			m_AtlasPad.UpdateItem(&obj,bAdd);
		}
		break;
	}
}

int KUiComItem::GetWindowsNum()
{
		return m_nNum;
}

void KUiComItem::ComItem(unsigned int pItem, int nWindowNum, int nNum)
{
	switch(nWindowNum)
	{
	case WINDOWS_COMP:
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_COMPITEM_COM,pItem,1);
		}
		break;
	case WINDOWS_COMP2:
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_COMPITEM_COM,pItem,2);
		}
		break;
	case WINDOWS_COMP3:
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_COMPITEM_COM,pItem,3);
		}
		break;
	case WINDOWS_DISTill:
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_COMPITEM_DISTILL,pItem,nNum);
		}
		break;
	case WINDOWS_FORG:
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_COMPITEM_FORGE,pItem,nNum);
		}
		break;
	case WINDOWS_ENCHASE:
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_COMPITEM_ENCHASE,pItem,nNum);
		}
		break;
	case WINDOWS_ATLAS:
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_COMPITEM_ALTAL,pItem,nNum);
		}
		break;
	}
}

void KUiComItem::Breathe()
{
// 	if(m_nStatus == STATUS_BEGIN_TREMBLE)
// 	{
// 		m_TrembleEffect.Show();
// 		m_TrembleEffect.SetFrame(0);
// 		m_nStatus = STATUS_TREMBLING;
// 	}
// 	else if(m_nStatus == STATUS_TREMBLING)
// 	{
// 		if(!PlayEffect())
// 		{
// 			m_nStatus = STATUS_CHANGING_ITEM;
// 			m_TrembleEffect.Hide();
// 		}
// 	}
// 	else if(m_nStatus == STATUS_CHANGING_ITEM)
// 	{
// 		m_nStatus = STATUS_FINISH;
// 	}
}

int KUiComItem::PlayEffect()
{
// 	if(m_TrembleEffect.GetMaxFrame() == 0 ||
// 		m_TrembleEffect.GetMaxFrame() >= m_TrembleEffect.GetCurrentFrame() - 1)
// 	{
// 		m_TrembleEffect.SetFrame(0);
// 		return 0;
// 	}
// 	else
// 	{
// 		m_TrembleEffect.NextFrame();
// 		return 1;
// 	}
	return 0;
}

void KUiComItem::CleanItem()
{
	m_CompoundPad.CleanItem();
	m_DistillPad.CleanItem();
	m_ForgePad.CleanItem();
	m_EnchasePad.CleanItem();
	m_AtlasPad.CleanItem();
}
KUiCompound::KUiCompound()
{

}

int KUiCompound::WndProc( unsigned int uMsg, unsigned int uParam, int nParam )
{
	switch(uMsg)
	{
	case WND_N_SCORLLBAR_POS_CHANGED:
		if(uParam == (unsigned int)&m_ListScroll)
		{
			m_Guide.SetFirstShowLine(nParam);
		}
		break;
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_Cancle)
		{
			CleanItem();
		}
		else if(uParam == (unsigned int)&m_Compound)
		{
			KUiComItem* pSelf =  KUiComItem::GetIfVisible();
			
			KUiDraggedObject pObj;
			unsigned int pUP[3] ;

			m_Box1.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[0] = pObj.uId;
			}
			else
			{
				//ÎïÆ·²»×ã
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 1;
			}

			pObj.uId = 0;
			m_Box2.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[1] = pObj.uId;
			}
			else
			{
				//ÎïÆ·²»×ã
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 1;
			}

			pObj.uId = 0;
			m_Box3.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[2] = pObj.uId;
			}
			else
			{
				//ÎïÆ·²»×ã
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 1;
			}

			pSelf->ComItem((unsigned int)(&pUP),m_nSelect,3);
			CleanItem();

		}
		break;
		
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 1;
}

void KUiCompound::PaintWindow()
{
	KWndPage::PaintWindow();
}

void KUiCompound::LoadScheme( const char* pScheme )
{
	char		Buff[128],Buffer[64];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_COMPE);
	if (Ini.Load(Buff))
	{
		KWndImage::Init(&Ini, "Main");
		m_Box1.Init(&Ini,"Ore1");
		m_Box2.Init(&Ini,"Ore2");
		m_Box3.Init(&Ini,"Ore3");
		m_Compound.Init(&Ini,"CompoundBtn");
		m_Cancle.Init(&Ini,"CancleBtn");
		m_Guide.Init(&Ini,"GuideList");
		m_ListScroll.Init(&Ini,"GuideList_Scroll");
		m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");

		int nX,nY,nColor;
		Ini.GetInteger2("Box_0","Pos",&nX,&nY);

		if (Ini.GetString("TextColor", "Font", "", Buffer, sizeof(Buffer)))
		{
			nColor = (::GetColor(Buffer) & 0xFFFFFF);
		}
		m_nSelect = 0;
		m_Pos1.SetPosition(nX-25,nY-4);
		m_Pos1.SetText("H­¬ng Nang");
		m_Pos1.SetTextColor(nColor);
		m_Pos1.BringToTop();

		Ini.GetInteger2("Box_1","Pos",&nX,&nY);

		m_Pos2.SetPosition(nX-35,nY-4);
		m_Pos2.SetText("H¹ng Liªn");
		m_Pos2.SetTextColor(nColor);
		m_Pos2.BringToTop();

		Ini.GetInteger2("Box_2","Pos",&nX,&nY);
		
		m_Pos3.SetPosition(nX-25,nY-4);
		m_Pos3.SetText("Giíi ChØ");
		m_Pos3.SetTextColor(nColor);
		m_Pos3.BringToTop();
		
	}	
}

void KUiCompound::Initialize()
{
	AddChild(&m_Box1);
	AddChild(&m_Box2);
	AddChild(&m_Box3);
	AddChild(&m_Compound);
	AddChild(&m_Cancle);

	AddChild(&m_Guide);
	AddChild(&m_ListScroll);
	AddChild(&m_ListBtn);
	 
	AddChild(&m_Pos1);
	AddChild(&m_Pos2);
	AddChild(&m_Pos3);
	m_Guide.SetScrollbar(&m_ListScroll);

	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	char		Buff[128];
	KIniFile	Ini;
	
	sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
	if (Ini.Load(Buff))
	{
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","Compound","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
		
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","CompoundRule","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
	}
	
	Wnd_AddWindow(this);
}

void KUiCompound::UpdateItem( KUiDraggedObject* pItem, int bAdd )
{
	if (bAdd == 1)
	{
		m_Box1.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	if (bAdd == 2)
	{
		m_Box2.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	if (bAdd == 3)
	{
		m_Box3.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	if (bAdd == 4)
	{
		KUiDraggedObject pObj;
		m_Box1.GetObject(pObj);
		if (pObj.uGenre)
		{
			m_Box2.GetObject(pObj);
			if (pObj.uGenre)
			{
				m_Box3.GetObject(pObj);
				if (pObj.uGenre)
				{
					m_Box1.HoldObject(pItem->uGenre,pItem->uId,58,54);
				}
				else
				{
					m_Box3.HoldObject(pItem->uGenre,pItem->uId,58,54);
				}
			}
			else
			{
				m_Box2.HoldObject(pItem->uGenre,pItem->uId,58,54);
			}
		}
		else
		{
			m_Box1.HoldObject(pItem->uGenre,pItem->uId,58,54);
		}
	}
}

void KUiCompound::SetPosText( int i )
{
	switch(i)
	{
	case 1:
		m_nSelect = 0;
		m_Pos1.SetText("H­¬ng Nang");
		m_Pos2.SetText("H¹ng Liªn");
		m_Pos3.SetText("Giíi ChØ");
		break;
	case 2:
		m_nSelect = 1;
		m_Pos1.SetText("HuyÒn Tinh");
		m_Pos2.SetText("HuyÒn Tinh");
		m_Pos3.SetText("HuyÒn Tinh");
		break;
	case 3:
		m_nSelect = 2;
		m_Pos1.SetText("Kho¸ng Th¹ch");
		m_Pos2.SetText("Kho¸ng Th¹ch");
		m_Pos3.SetText("Kho¸ng Th¹ch");
		break;
	}
}

void KUiCompound::CleanItem()
{
	m_Box1.Celar();
	m_Box2.Celar();
	m_Box3.Celar();
}
KUiDistill::KUiDistill()
{
	
}

void KUiDistill::PaintWindow()
{
	KWndPage::PaintWindow();
}

int KUiDistill::WndProc( unsigned int uMsg, unsigned int uParam, int nParam )
{	
	switch(uMsg)
	{
	case WND_N_SCORLLBAR_POS_CHANGED:
		if(uParam == (unsigned int)&m_ListScroll)
		{
			m_Guide.SetFirstShowLine(nParam);
		}
		break;
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_Cancle)
		{
			CleanItem();
		}
		else if(uParam == (unsigned int)&m_Distill)
		{ 
			int nNum = 0;
			
			KUiDraggedObject pObj;
			unsigned int pUP[11] ;

			m_BigBox.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 1;
			}

			pObj.uId = 0;
			m_Box1.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 1;
			}

			pObj.uId = 0;
			m_Box2.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 1;
			}

			for (int i = 0; i < 2;i++)
			{
				for (int j = 0; j < 4;j++)
				{
					pObj.uId = 0;
					m_ItemBox.GetObject(pObj,i,j);
					if (pObj.uId > 0)
					{
						pUP[nNum] = pObj.uId;
						nNum++;
					}
					else
					{
						break;
					}
				}
			}

			KUiComItem* pSelf = KUiComItem::GetIfVisible();
			pSelf->ComItem((unsigned int)(&pUP),3,nNum);

			CleanItem();
		}
		break;
		
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 1;
}

void KUiDistill::LoadScheme( const char* pScheme )
{
	char		Buff[128],Buffer[64];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_OUTIN);
	if (Ini.Load(Buff))
	{
		KWndImage::Init(&Ini, "Main");
		m_BigBox.Init(&Ini,"BigBox");
		m_Box1.Init(&Ini,"SmallBox1");
		m_Box2.Init(&Ini,"SmallBox2");
		m_ItemBox.Init(&Ini,"ItemBox");
		m_Distill.Init(&Ini,"DistillBtn");
		m_Cancle.Init(&Ini,"CancleBtn");
		m_Guide.Init(&Ini,"GuideList");
		m_ListScroll.Init(&Ini,"GuideList_Scroll");
		m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");
		m_ItemBox.EnableTracePutPos(FALSE);
		
		int nX,nY,nColor;
		Ini.GetInteger2("EquipPos","Pos",&nX,&nY);
		if (Ini.GetString("TextColor", "Font", "", Buffer, sizeof(Buffer)))
		{
			nColor = (::GetColor(Buffer) & 0xFFFFFF);
		}

		m_Pos1.SetPosition(nX-35,nY-4);
		m_Pos1.SetTextColor(nColor);
		m_Pos1.BringToTop();
		m_Pos1.SetText("Trang bÞ xanh");

		Ini.GetInteger2("CryolitePos","Pos",&nX,&nY);
		m_Pos2.SetPosition(nX-35,nY-4);
		m_Pos2.SetTextColor(nColor);
		m_Pos2.BringToTop();
		m_Pos2.SetText("HuyÒn Tinh");

		Ini.GetInteger2("PropMinePos","Pos",&nX,&nY);
		m_Pos3.SetPosition(nX-35,nY-4);
		m_Pos3.SetTextColor(nColor);
		m_Pos3.BringToTop();
		m_Pos3.SetText("Kho¸ng Th¹ch");

		Ini.GetInteger2("ConsumePos","Pos",&nX,&nY);
		m_Pos4.SetPosition(nX-35,nY-4);
		m_Pos4.SetTextColor(nColor);
		m_Pos4.BringToTop();
		m_Pos4.SetText("Nguyªn liÖu phô");
	}	
}

void KUiDistill::Initialize()
{
	AddChild(&m_BigBox);
	AddChild(&m_Box1);
	AddChild(&m_Box2);
	AddChild(&m_ItemBox);
	AddChild(&m_Distill);
	AddChild(&m_Cancle);
	
	AddChild(&m_Guide);
	AddChild(&m_ListScroll);
	AddChild(&m_ListBtn);

	AddChild(&m_Pos1);
	AddChild(&m_Pos2);
	AddChild(&m_Pos3);
	AddChild(&m_Pos4);
	m_Guide.SetScrollbar(&m_ListScroll);
	
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	char		Buff[128];
	KIniFile	Ini;
	
	sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
	if (Ini.Load(Buff))
	{
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","Distill","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
		
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","DistillRule","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
	}
	
	Wnd_AddWindow(this);
}

void KUiDistill::UpdateItem( KUiDraggedObject* pItem, int bAdd )
{
	if (bAdd == 1)
	{
		m_BigBox.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 2)
	{
		m_Box1.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 3)
	{
		m_Box2.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 4)
	{
		KUiDraggedObject pObj;
		for (int i = 0; i < 2;i++)
		{
			for (int j = 0; j < 4;j++)
			{
				pObj.uId = 0;
				m_ItemBox.GetObject(pObj,i,j);
				if (pObj.uId == pItem->uId)
				{
					return;
				}
				if (!pObj.uId)
				{
					pItem->DataX = i;
					pItem->DataY = j;
					m_ItemBox.AddObject(pItem,1);
					return;
				}
			}
		}
	}
}

void KUiDistill::CleanItem()
{
	m_BigBox.Celar();
	m_Box1.Celar();
	m_Box2.Celar();
	m_ItemBox.Clear();
}
KUiForge::KUiForge()
{
	
}

void KUiForge::PaintWindow()
{
	KWndPage::PaintWindow();
}

int KUiForge::WndProc( unsigned int uMsg, unsigned int uParam, int nParam )
{
	switch(uMsg)
	{
	case WND_N_SCORLLBAR_POS_CHANGED:
		if(uParam == (unsigned int)&m_ListScroll)
		{
			m_Guide.SetFirstShowLine(nParam);
		}
		break;
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_Cancle)
		{
			CleanItem();
		}
		else if(uParam == (unsigned int)&m_ForgeBtn)
		{
			int nNum = 0;
			
			KUiDraggedObject pObj;
			unsigned int pUP[2] ;
			
			m_BigBox.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 1; 
			}
			
			pObj.uId = 0;
			m_SmallBox.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 1;
			}		
			
			KUiComItem* pSelf = KUiComItem::GetIfVisible();
			pSelf->ComItem((unsigned int)(&pUP),4,nNum);
			
			CleanItem();
		}
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 1;	
}

void KUiForge::LoadScheme( const char* pScheme )
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_BUILD);
	if (Ini.Load(Buff))
	{
		KWndImage::Init(&Ini, "Main");
		m_BigBox.Init(&Ini,"BigBox");
		m_SmallBox.Init(&Ini,"SmallBox");
		m_ForgeBtn.Init(&Ini,"ForgeBtn");
		m_Cancle.Init(&Ini,"CancleBtn");
		m_Guide.Init(&Ini,"GuideList");
		m_ListScroll.Init(&Ini,"GuideList_Scroll");
		m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");

		int nX,nY,nColor;
		Ini.GetInteger2("EquipPos","Pos",&nX,&nY);
		if (Ini.GetString("TextColor", "Font", "", Buff, sizeof(Buff)))
		{
			nColor = (::GetColor(Buff) & 0xFFFFFF);
		}
		
		m_Pos1.SetPosition(nX-35,nY-4);
		m_Pos1.SetTextColor(nColor);
		m_Pos1.BringToTop();
		m_Pos1.SetText("Trang bÞ xanh");
		
		Ini.GetInteger2("CryolitePos","Pos",&nX,&nY);
		m_Pos2.SetPosition(nX-35,nY-4);
		m_Pos2.SetTextColor(nColor);
		m_Pos2.BringToTop();
		m_Pos2.SetText("HuyÒn Tinh");
		
	}	
}

void KUiForge::Initialize()
{
	AddChild(&m_BigBox);
	AddChild(&m_SmallBox);
	AddChild(&m_ForgeBtn);
	AddChild(&m_Cancle);
	
	AddChild(&m_Guide);
	AddChild(&m_ListScroll);
	AddChild(&m_ListBtn);

	AddChild(&m_Pos1);
	AddChild(&m_Pos2);
	m_Guide.SetScrollbar(&m_ListScroll);
	
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	char		Buff[128];
	KIniFile	Ini;
	
	sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
	if (Ini.Load(Buff))
	{
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","Forge","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
		
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","ForgeRule","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
	}
	
	Wnd_AddWindow(this);
}

void KUiForge::UpdateItem( KUiDraggedObject* pItem, int bAdd )
{
	if (bAdd == 1)
	{
		m_BigBox.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 2)
	{
		m_SmallBox.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
}

void KUiForge::CleanItem()
{
	m_BigBox.Celar();
	m_SmallBox.Celar();
}
KUiEnchase::KUiEnchase()
{
	
}

void KUiEnchase::PaintWindow()
{
	KWndPage::PaintWindow();
}

int KUiEnchase::WndProc( unsigned int uMsg, unsigned int uParam, int nParam )
{
	switch(uMsg)
	{
	case WND_N_SCORLLBAR_POS_CHANGED:
		if(uParam == (unsigned int)&m_ListScroll)
		{
			m_Guide.SetFirstShowLine(nParam);
		}
		break;
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_Cancle)
		{
			CleanItem();
		}
		else if(uParam == (unsigned int)&m_Distill)
		{
			int nNum = 0;
			
			KUiDraggedObject pObj;
			unsigned int pUP[11] ;
			
			m_BigBox.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_Box1.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_Box2.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			for (int i = 0; i < 2;i++)
			{
				for (int j = 0; j < 4;j++)
				{
					pObj.uId = 0;
					m_ItemBox.GetObject(pObj,i,j);
					if (pObj.uId > 0)
					{
						pUP[nNum] = pObj.uId;
						nNum++;
					}
					else
					{
						break;
					}
				}
			}
			
			KUiComItem* pSelf = KUiComItem::GetIfVisible();
			pSelf->ComItem((unsigned int)(&pUP),5,nNum);
			
			CleanItem();
		}
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 1;	
}

void KUiEnchase::LoadScheme( const char* pScheme )
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_OUTIN);
	if (Ini.Load(Buff))
	{
		KWndImage::Init(&Ini, "Main");
		m_BigBox.Init(&Ini,"BigBox");
		m_Box1.Init(&Ini,"SmallBox1");
		m_Box2.Init(&Ini,"SmallBox2");
		m_ItemBox.Init(&Ini,"ItemBox");
		m_Distill.Init(&Ini,"DistillBtn");
		m_Cancle.Init(&Ini,"CancleBtn");
		m_Guide.Init(&Ini,"GuideList");
		m_ListScroll.Init(&Ini,"GuideList_Scroll");
		m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");
		m_ItemBox.EnableTracePutPos(FALSE);
		

		int nX,nY,nColor;
		Ini.GetInteger2("EquipPos","Pos",&nX,&nY);
		if (Ini.GetString("TextColor", "Font", "", Buff, sizeof(Buff)))
		{
			nColor = (::GetColor(Buff) & 0xFFFFFF);
		}
		
		m_Pos1.SetPosition(nX-35,nY-4);
		m_Pos1.SetTextColor(nColor);
		m_Pos1.BringToTop();
		m_Pos1.SetText("Trang bÞ tÝm");
		
		Ini.GetInteger2("CryolitePos","Pos",&nX,&nY);
		m_Pos2.SetPosition(nX-35,nY-4);
		m_Pos2.SetTextColor(nColor);
		m_Pos2.BringToTop();
		m_Pos2.SetText("HuyÒn Tinh");
		
		Ini.GetInteger2("PropMinePos","Pos",&nX,&nY);
		m_Pos3.SetPosition(nX-35,nY-4);
		m_Pos3.SetTextColor(nColor);
		m_Pos3.BringToTop();
		m_Pos3.SetText("Kho¸ng Th¹ch");
		
		Ini.GetInteger2("ConsumePos","Pos",&nX,&nY);
		m_Pos4.SetPosition(nX-35,nY-4);
		m_Pos4.SetTextColor(nColor);
		m_Pos4.BringToTop();
		m_Pos4.SetText("Nguyªn liÖu phô");
	}	
}

void KUiEnchase::Initialize()
{
	AddChild(&m_BigBox);
	AddChild(&m_Box1);
	AddChild(&m_Box2);
	AddChild(&m_ItemBox);
	AddChild(&m_Distill);
	AddChild(&m_Cancle);
	
	AddChild(&m_Guide);
	AddChild(&m_ListScroll);
	AddChild(&m_ListBtn);

	AddChild(&m_Pos1);
	AddChild(&m_Pos2);
	AddChild(&m_Pos3);
	AddChild(&m_Pos4);
	m_Guide.SetScrollbar(&m_ListScroll);
	
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	char		Buff[128];
	KIniFile	Ini;
	
	sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
	if (Ini.Load(Buff))
	{
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","Enchase","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
		
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","EnchaseRule","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
	}
	
	Wnd_AddWindow(this);
}

void KUiEnchase::UpdateItem( KUiDraggedObject* pItem, int bAdd )
{
	if (bAdd == 1)
	{
		m_BigBox.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 2)
	{
		m_Box1.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 3)
	{
		m_Box2.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 4)
	{
		KUiDraggedObject pObj;
		for (int i = 0; i < 2;i++)
		{
			for (int j = 0; j < 4;j++)
			{
				pObj.uId = 0;
				m_ItemBox.GetObject(pObj,i,j);
				if (pObj.uId == pItem->uId)
				{
					return;
				}
				if (!pObj.uId)
				{
					pItem->DataX = i;
					pItem->DataY = j;
					m_ItemBox.AddObject(pItem,1);
					return;
				}
			}
		}
	}
}

void KUiEnchase::CleanItem()
{
	m_BigBox.Celar();
	m_Box1.Celar();
	m_Box2.Celar();
	m_ItemBox.Clear();
}

KUiAtlas::KUiAtlas()
{
	
}

void KUiAtlas::PaintWindow()
{
	KWndPage::PaintWindow();
}

int KUiAtlas::WndProc( unsigned int uMsg, unsigned int uParam, int nParam )
{
	switch(uMsg)
	{
	case WND_N_SCORLLBAR_POS_CHANGED:
		if(uParam == (unsigned int)&m_ListScroll)
		{
			m_Guide.SetFirstShowLine(nParam);
		}
		break;
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_Cancle)
		{
			CleanItem();
		}
		else if(uParam == (unsigned int)&m_AtlasBtn)
		{
			int nNum = 0;
			
			KUiDraggedObject pObj;
			unsigned int pUP[11] ;
			
			m_Box1.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_Box2.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_Box3.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_Box4.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_Box5.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_Box6.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_ItemBox.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_AtlasBox.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
	
			pObj.uId = 0;
			m_CryoliteBox.GetObject(pObj);
			if (pObj.uId > 0)
			{
				pUP[nNum] = pObj.uId;
				nNum++;
			}
			else
			{
				g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, KD_VP, 0);
				return 0;
			}
			
			KUiComItem* pSelf = KUiComItem::GetIfVisible();
			pSelf->ComItem((unsigned int)(&pUP),6,nNum);
		}
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 1;	
}

void KUiAtlas::LoadScheme( const char* pScheme )
{
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI_DOPHO);
	if (Ini.Load(Buff))
	{
		KWndImage::Init(&Ini, "Main");
		m_Box1.Init(&Ini,"Box1");
		m_Box2.Init(&Ini,"Box2");
		m_Box3.Init(&Ini,"Box3");
		m_Box4.Init(&Ini,"Box4");
		m_Box5.Init(&Ini,"Box5");
		m_Box6.Init(&Ini,"Box6");
		m_AtlasBox.Init(&Ini,"AtlasBox");
		m_CryoliteBox.Init(&Ini,"CryoliteBox");
		m_ItemBox.Init(&Ini,"ItemBox");
		m_AtlasBtn.Init(&Ini,"AtlasBtn");
		m_Guide.Init(&Ini,"GuideList");
		m_ListScroll.Init(&Ini,"GuideList_Scroll");
		m_ListBtn.Init(&Ini,"GuideList_Scroll_Btn");
		

		int nX,nY,nColor;
		Ini.GetInteger2("ConsumePos","Pos",&nX,&nY);
		if (Ini.GetString("TextColor", "Font", "", Buff, sizeof(Buff)))
		{
			nColor = (::GetColor(Buff) & 0xFFFFFF);
		}
		
		m_Pos1.SetPosition(nX-35,nY-15);
		m_Pos1.SetTextColor(nColor);
		m_Pos1.BringToTop();
		m_Pos1.SetText("Nguyªn liÖu phô");
		
		Ini.GetInteger2("CryolitePos","Pos",&nX,&nY);
		m_Pos2.SetPosition(nX-35,nY+8);
		m_Pos2.SetTextColor(nColor);
		m_Pos2.BringToTop();
		m_Pos2.SetText("HuyÒn Tinh");
		
		Ini.GetInteger2("AtlasPos","Pos",&nX,&nY);
		m_Pos3.SetPosition(nX-25,nY+4);
		m_Pos3.SetTextColor(nColor);
		m_Pos3.BringToTop();
		m_Pos3.SetText("§å phæ");
		
		Ini.GetInteger2("Material_0","Pos",&nX,&nY);
		m_Pos4.SetPosition(nX-35,nY-4);
		m_Pos4.SetTextColor(nColor);
		m_Pos4.BringToTop();
		m_Pos4.SetText("VËt liÖu 1");
		
		Ini.GetInteger2("Material_1","Pos",&nX,&nY);
		m_Pos5.SetPosition(nX-35,nY-4);
		m_Pos5.SetTextColor(nColor);
		m_Pos5.BringToTop();
		m_Pos5.SetText("VËt liÖu 4");
		
		Ini.GetInteger2("Material_2","Pos",&nX,&nY);
		m_Pos6.SetPosition(nX-35,nY-4);
		m_Pos6.SetTextColor(nColor);
		m_Pos6.BringToTop();	
		m_Pos6.SetText("VËt liÖu 5");

		Ini.GetInteger2("Material_3","Pos",&nX,&nY);
		m_Pos7.SetPosition(nX-35,nY-4);
		m_Pos7.SetTextColor(nColor);
		m_Pos7.BringToTop();	
		m_Pos7.SetText("VËt liÖu 6");
		
		Ini.GetInteger2("Material_4","Pos",&nX,&nY);
		m_Pos8.SetPosition(nX-35,nY-4);
		m_Pos8.SetTextColor(nColor);
		m_Pos8.BringToTop();
		m_Pos8.SetText("VËt liÖu 3");

		Ini.GetInteger2("Material_5","Pos",&nX,&nY);
		m_Pos9.SetPosition(nX-35,nY-4);
		m_Pos9.SetTextColor(nColor);
		m_Pos9.BringToTop();
		m_Pos9.SetText("VËt liÖu 2");
	}		
}

void KUiAtlas::Initialize()
{
	AddChild(&m_Box1);
	AddChild(&m_Box2);
	AddChild(&m_Box3);
	AddChild(&m_Box4);
	AddChild(&m_Box5);
	AddChild(&m_Box6);
	AddChild(&m_AtlasBox);
	AddChild(&m_CryoliteBox);
	AddChild(&m_ItemBox);
	AddChild(&m_AtlasBtn);
	
	AddChild(&m_Guide);
	AddChild(&m_ListScroll);
	AddChild(&m_ListBtn);

	AddChild(&m_Pos1);
	AddChild(&m_Pos2);
	AddChild(&m_Pos3);
	AddChild(&m_Pos4);
	AddChild(&m_Pos5);
	AddChild(&m_Pos6);
	AddChild(&m_Pos7);
	AddChild(&m_Pos8);
	AddChild(&m_Pos9);
	m_Guide.SetScrollbar(&m_ListScroll);
	
	
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);
	
	char		Buff[128];
	KIniFile	Ini;
	
	sprintf(Buff, "%s\\%s", Scheme, SCHEME_INI_SHEET);
	if (Ini.Load(Buff))
	{
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","Forge","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
		
		ZeroMemory(Buff,sizeof(Buff));
		Ini.GetString("RuleInfo","ForgeRule","",Buff,sizeof(Buff));
		
		m_Guide.AddOneMessage(Buff,sizeof(Buff));
	}
	
	Wnd_AddWindow(this);	
}

void KUiAtlas::CleanItem()
{
	m_Box1.Celar();
	m_Box2.Celar();
	m_Box3.Celar();
	m_Box4.Celar();
	m_Box5.Celar();
	m_Box6.Celar();
	m_ItemBox.Celar();
	m_AtlasBox.Celar();
	m_CryoliteBox.Celar();
}

void KUiAtlas::UpdateItem( KUiDraggedObject* pItem, int bAdd )
{
	if (bAdd == 1)
	{
		m_Box1.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 2)
	{
		m_Box2.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}
	else if (bAdd == 3)
	{
		m_Box3.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}	
	else if (bAdd == 4)
	{
		m_Box4.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}	
	else if (bAdd == 5)
	{
		m_Box5.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}	
	else if (bAdd == 6)
	{
		m_Box6.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}	
	else if (bAdd == 7)
	{
		m_AtlasBox.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}	
	else if (bAdd == 8)
	{
		m_CryoliteBox.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}	
	else if (bAdd == 9)
	{
		m_ItemBox.HoldObject(pItem->uGenre,pItem->uId,58,54);
	}	
}
int KCanGetNumImage2::GetMaxFrame()
{
	return m_Image.nNumFrames;
}


int KCanGetNumImage2::GetCurrentFrame()
{
	return m_Image.nFrame;
}
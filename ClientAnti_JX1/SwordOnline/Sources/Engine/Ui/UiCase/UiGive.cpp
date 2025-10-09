// -------------------------------------------------------------------------
//	文件名		：	UiGetString.cpp
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-3-18
//	功能描述	：	获得字符串界面
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/AutoLocateWnd.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "../UiBase.h"
#include "UiGive.h"
#include "UiInformation.h"
#include "../UiSoundSetting.h"
#include "../../../core/src/coreshell.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI 	"UiGive.ini"


KUiGive* KUiGive::m_pSelf = NULL;

KUiGive::KUiGive()
{
	m_nNum = 0;
}

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiGive* KUiGive::OpenWindow(const char* pszTitle,
						const char* pszInitString,const int nNum)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiGive;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		if (pszTitle)
			m_pSelf->m_Title.SetText(pszTitle);
		else
			m_pSelf->m_Title.SetText("");
		if (pszInitString)
			m_pSelf->m_ContentList.SetText(pszInitString);
		else
			m_pSelf->m_ContentList.SetText("");

		m_pSelf->m_ContentList.SetTextColor(0xffffffff);
		m_pSelf->BringToTop();
		m_pSelf->Show();
		m_pSelf->m_nNeed = nNum;
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：关闭销毁窗口
//--------------------------------------------------------------------------
void KUiGive::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();
	}
}

KUiGive*	KUiGive::GetIfVisible()
{
	if (m_pSelf && m_pSelf->IsVisible())
		return m_pSelf;
	return NULL;
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
void KUiGive::Initialize()
{
	AddChild(&m_Title);
	AddChild(&m_ContentList);
	AddChild(&m_OkBtn);
	AddChild(&m_CancelBtn);
	AddChild(&m_ItemBox);
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	m_ItemBox.SetContainerId((int)UOC_ITEM_GIVE);
	m_nNum = 0;
	ZeroMemory(m_nUid,sizeof(m_nUid));
	Wnd_AddWindow(this);
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiGive::LoadScheme(const char* pScheme)
{
	if (m_pSelf)
	{
		char		Buff[128];
		KIniFile	Ini;
		sprintf(Buff, "%s\\%s", pScheme, SCHEME_INI);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_Title.Init(&Ini, "Title");
			m_pSelf->m_ContentList.Init(&Ini, "ContentList");
			m_pSelf->m_OkBtn.Init(&Ini, "Assemble");
			m_pSelf->m_CancelBtn.Init(&Ini, "Close");
			m_pSelf->m_ItemBox.Init(&Ini, "Items");
			m_pSelf->m_ItemBox.EnableTracePutPos(true);
			m_pSelf->m_ItemBox.EnablePickPut(true);
		}
	}
}

//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
int KUiGive::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_OkBtn)
			OnOk();
		else if (uParam == (unsigned int)(KWndWindow*)&m_CancelBtn)
			OnCancel();
		break;
	case WM_KEYDOWN:
		if (uParam == VK_RETURN)
		{
			OnOk();
			nRet = 1;
		}
		else if (uParam == VK_ESCAPE)
		{
			OnCancel();
			nRet = 1;
		}
		break;
	case WND_N_ITEM_PICKDROP:
		OnItemPickDrop((ITEM_PICKDROP_PLACE*)uParam, (ITEM_PICKDROP_PLACE*)nParam);
		break;
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return 0;
}
//--------------------------------------------------------------------------
//	功能：响应点击确认按钮
//--------------------------------------------------------------------------
void KUiGive::OnOk()
{
	if (g_pCoreShell)
	{
//		m_nNum = m_ItemBox.GetObjectNum();
		KUiDraggedObject pObj;
		unsigned int uId[32];
		for (int i = 0;i < 32;i++)
		{
			if (i < m_nNum)
			{
				m_ItemBox.GetObjectById(pObj,i);
				uId[i] = pObj.uId;
			}
			else
			{
				uId[i] = 0;
			}
		}
		
		if (m_nNeed)
		{
			g_pCoreShell->OperationRequest(GOI_GIVE_ITEM_BACK,(unsigned int)(&uId),m_nNum);
		}
		else
		{
			g_pCoreShell->OperationRequest(GOI_GIVE_ITEM_NOBACK,(unsigned int)(&uId),m_nNum);
		}
	}
	CloseWindow(true);
}

void KUiGive::OnCancel()
{
	if (g_pCoreShell)
	{
//		m_nNum = m_ItemBox.GetObjectNum();
		KUiDraggedObject pObj;
		unsigned int uId[32];
		for (int i = 0;i < 32;i++)
		{
			if (i < m_nNum)
			{
				m_ItemBox.GetObjectById(pObj,i);
				uId[i] = pObj.uId;
			}
			else
			{
				uId[i] = 0;
			}
		}
		
		
		g_pCoreShell->OperationRequest(GOI_GIVE_ITEM_FAIL,(unsigned int)(&uId),m_nNum);
	}
	CloseWindow(true);
}

void KUiGive::UpdateItem( KUiObjAtRegion* pItem, int bAdd )
{
	if (pItem)
	{
		KUiDraggedObject Obj;
		Obj.uGenre = pItem->Obj.uGenre;
		Obj.uId = pItem->Obj.uId;
		Obj.DataX = pItem->Region.h;
		Obj.DataY = pItem->Region.v;
		Obj.DataW = pItem->Region.Width;
		Obj.DataH = pItem->Region.Height;
		if (bAdd)
		{
			if (pItem->Obj.uId)
			{
				m_nNum++;
			}
			m_ItemBox.AddObject(&Obj, 1);
		}
		else
		{
			if (pItem->Obj.uId)
			{
				m_nNum--;
			}
			
			m_ItemBox.RemoveObject(&Obj);
		}
		UiSoundPlay(UI_SI_PICKPUT_ITEM);
	}
}

void KUiGive::OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)
{
	if (!g_UiBase.IsOperationEnable(UIS_O_MOVE_ITEM) && 
		!g_UiBase.IsOperationEnable(UIS_O_TRADE_ITEM))
		return;
	KUiObjAtContRegion	Pick, Drop;
	KUiDraggedObject	Obj;
	
	UISYS_STATUS eStatus = g_UiBase.GetStatus();
	if (pPickPos)
	{
		if(!pPickPos->pWnd) return;		
		((KWndObjectMatrix*)(pPickPos->pWnd))->GetObject(
			Obj, pPickPos->h, pPickPos->v);
		Pick.Obj.uGenre = Obj.uGenre;
		Pick.Obj.uId = Obj.uId;
		Pick.Region.Width = Obj.DataW;
		Pick.Region.Height = Obj.DataH;
		Pick.Region.h = Obj.DataX;
		Pick.Region.v = Obj.DataY;
		Pick.eContainer = UOC_ITEM_GIVE;
		
		if (eStatus == UIS_S_TRADE_SALE)
		{
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_SELL,
				(unsigned int)(&Pick), 0);
			return;
		}
		else if (eStatus == UIS_S_TRADE_REPAIR)
		{
			g_pCoreShell->OperationRequest(GOI_TRADE_NPC_REPAIR,
				(unsigned int)(&Pick), 0);
			return;
		}
		else if (eStatus == UIS_S_TRADE_BUY)
			return;
	}
	
	if (pDropPos)
	{
		Wnd_GetDragObj(&Obj);
		Drop.Obj.uGenre = Obj.uGenre;
		Drop.Obj.uId = Obj.uId;
		Drop.Region.Width = Obj.DataW;
		Drop.Region.Height = Obj.DataH;
		Drop.Region.h = pDropPos->h;
		Drop.Region.v = pDropPos->v;
		Drop.eContainer = UOC_ITEM_GIVE;	
	}
	
	g_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT,
		pPickPos ? (unsigned int)&Pick : 0,
		pDropPos ? (int)&Drop : 0);
}
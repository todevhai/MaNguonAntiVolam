/*****************************************************************************************
//	界面--屏幕顶控制操作条
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-22
*****************************************************************************************/
#include "KWin32.h"
#include "KIniFile.h"
#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"
#include "../UiBase.h"
#include "UiToolsControlBar.h"
#include "UiChatCentre.h"
#include "UiUnlockBox.h"
#include "UiItemEX.h"
#include "UiAutoPlay.h"

/* Khung nut PK da ve; -1 = chua ve lan nao. Phai o DAU TEP: WndProc dung
   no nam truoc Breathe trong tep nay. */
static int s_nCoPKDaVe = -1;

#include "../ShortcutKey.h"
#include "../../../core/src/coreshell.h"
#include "GameDataDef.h"
extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI		"UiToolsControlBar.ini"

KUiToolsControlBar* KUiToolsControlBar::m_pSelf = NULL;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiToolsControlBar* KUiToolsControlBar::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiToolsControlBar;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
		m_pSelf->Show();
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：关闭窗口
//--------------------------------------------------------------------------
void KUiToolsControlBar::CloseWindow()
{
	if (m_pSelf)
	{
		m_pSelf->Destroy();
		m_pSelf = NULL;
	}
}

//初始化
void KUiToolsControlBar::Initialize()
{
	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	m_Style &= ~WND_S_VISIBLE;
	Wnd_AddWindow(this, WL_TOPMOST);
	AddChild(&m_Rec);
	AddChild(&m_ItemEx);
	AddChild(&m_Mission);
	AddChild(&m_Friend);
	AddChild(&m_ChatRoom);
	AddChild(&m_Options);
	AddChild(&m_Status);
	AddChild(&m_Items);
	AddChild(&m_Skills);
	AddChild(&m_Team);
	AddChild(&m_Faction);
	AddChild(&m_Run);
	AddChild(&m_Sit);
	AddChild(&m_Horse);
	AddChild(&m_Exchange);
	AddChild(&m_PK);
}

//载入界面方案
void KUiToolsControlBar::LoadScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	if (m_pSelf)
	{
		sprintf(Buff, "%s\\" SCHEME_INI, pScheme);
		if (Ini.Load(Buff))
		{
			m_pSelf->Init(&Ini, "Main");
			m_pSelf->m_Rec.Init(&Ini, "Rec");
			m_pSelf->m_ItemEx.Init(&Ini, "ItemEx");
			m_pSelf->m_Mission.Init(&Ini, "Mission");
			m_pSelf->m_Friend.Init(&Ini, "Friend");
			m_pSelf->m_ChatRoom.Init(&Ini, "ChatRoom");
			m_pSelf->m_Options.Init(&Ini, "Options");
			m_pSelf->m_Status  .Init(&Ini, "Status");
			m_pSelf->m_Items   .Init(&Ini, "Items");
			m_pSelf->m_Skills  .Init(&Ini, "Skills");
			m_pSelf->m_Team    .Init(&Ini, "Team");
			m_pSelf->m_Faction .Init(&Ini, "Faction");
			m_pSelf->m_Run     .Init(&Ini, "Run");
			m_pSelf->m_Sit     .Init(&Ini, "Sit");
			m_pSelf->m_Horse   .Init(&Ini, "Horse");
			m_pSelf->m_Exchange.Init(&Ini, "Exchange");
			m_pSelf->m_PK      .Init(&Ini, "PK");
		}
	}
}
int KUiToolsControlBar::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WND_N_BUTTON_DOWN:
		/* Engine vua SetFrame(m_nDownFrame) - ve lai ngay cho dung. */
		if (uParam == (unsigned int)(KWndWindow*)&m_PK && g_pCoreShell)
		{
			int nKhung = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);
			m_PK.SetFrame((nKhung < 0 || nKhung > 2) ? 0 : nKhung);
		}
		break;
	case WND_N_BUTTON_CLICK:
		if (uParam == (unsigned int)(KWndWindow*)&m_Status)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_STATUS);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Items)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_ITEMS);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Skills)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SKILLS);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Team)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_TEAM);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Faction)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_MAP);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Run)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_RUN);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Sit)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SIT);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Horse)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_HORSE);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Exchange)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_TRADE);
		else if (uParam == (unsigned int)(KWndWindow*)&m_PK)
			/* Di DUNG duong ma phim F9 dang di: Switch([[pk]]) mo cua so
			   KUiPK. Goi thang KUiPK::OpenWindow() thi F9 an ma nut khong -
			   do 30/08/2026. Dung mot duong cho ca hai, giong het chin nut
			   con lai o day va giong Player_PK::OnButtonClick ban goc. */
		{
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_PK);
			/* Tha tay xong OnLBtnUp da SetFrame(m_nUpFrame), nen phai ve lai
			   theo trang thai that o nhip ke tiep. */
			/* Tha tay xong OnLBtnUp da SetFrame(m_nUpFrame) - ve lai ngay,
			   va dat lai bien theo doi de nhip sau con chinh theo may chu. */
			if (g_pCoreShell)
			{
				int nKhung = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);
				m_PK.SetFrame((nKhung < 0 || nKhung > 2) ? 0 : nKhung);
			}
			s_nCoPKDaVe = -1;
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Friend)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_FRIEND);
		else if (uParam == (unsigned int)(KWndWindow*)&m_Options)
			KShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SYSTEM);
		else if (uParam == (unsigned int)(KWndWindow*)&m_ItemEx)
		{	
			g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, ITEMEX, 0);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Rec)
		{
			g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, HT_CN, 0);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_Mission)
		{
			g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, HT_CN, 0);
		}
		else if (uParam == (unsigned int)(KWndWindow*)&m_ChatRoom)
		{
			g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, HT_CN, 0);
		}
		break;
	default:
		nRet = KWndImage::WndProc(uMsg, uParam, nParam);
	}
	return nRet;
}
//重新初始化界面
void KUiToolsControlBar::DefaultScheme(const char* pScheme)
{
	char		Buff[128];
	KIniFile	Ini;
	if (m_pSelf)
	{
		sprintf(Buff, "%s\\" SCHEME_INI, pScheme);
		if (Ini.Load(Buff))
		{
			int	nValue1, nValue2;
			Ini.GetInteger("Main", "Left",  0, &nValue1);
			Ini.GetInteger("Main", "Top",   0, &nValue2);
			m_pSelf->SetPosition(nValue1, nValue2);
		}
	}
}

void KUiToolsControlBar::Breathe()
{
	UpdateData();
	/* Ve nut PK theo trang thai MAY CHU, nhung CHI KHI GIA TRI DOI.
	   Goi s2c_pksyncnormalflag ve ngay luc vao the gioi, co khi truoc ca
	   luc thanh cong cu dung xong; ve mot lan luc do thi nut ket o khung
	   "luyen cong" du may chu da bat chien dau. Nhung goi CheckButton MOI
	   NHIP thi no SetFrame lien tuc, de len khung "dang bam" - nguoi choi
	   thay icon nhay mot cai roi bi keo ve, va cu bam mat luon (OnLBtnUp
	   chi phat click khi co WNDBTN_F_DOWN con nguyen).
	   Nut chi co hai khung Up/Down nen chien dau va do sat trong giong nhau. */
	if (g_pCoreShell)
	{
		int nCoPK = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);
		if (nCoPK < 0 || nCoPK > 2)
			nCoPK = 0;
		if (nCoPK != s_nCoPKDaVe)
		{
			s_nCoPKDaVe = nCoPK;
			/* SetFrame chu khong CheckButton: sprite nut PK co BA khung
			   (0 luyen cong, 1 chien dau, 2 do sat) con CheckButton chi biet
			   bat/tat va lay khung Up/Down - ma khung Down dong thoi la khung
			   "dang nhan", nen icon nhay qua lai moi lan bam. */
			m_PK.SetFrame(nCoPK);
		}
	}
}


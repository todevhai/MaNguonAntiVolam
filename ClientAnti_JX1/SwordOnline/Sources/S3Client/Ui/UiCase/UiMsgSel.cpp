// -------------------------------------------------------------------------
//	文件名		：	UiMsgSel.h
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-1-6
//	功能描述	：	带滚动条的消息选择
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KIniFile.h"
#include "../Elem/WndMessage.h"
#include "../Elem/Wnds.h"
#include "UiMsgSel.h"
#include "../../../core/src/coreshell.h"
#include "../../../core/src/GameDataDef.h"
#include "../UiBase.h"
#include "../UiSoundSetting.h"

extern iCoreShell*		g_pCoreShell;

#define	SCHEME_INI	"UiMsgSel.ini"

KUiMsgSel* KUiMsgSel::m_pSelf = NULL;
int KUiMsgSel::ms_nNenDau = 0;
int KUiMsgSel::ms_nNenThan = 0;
int KUiMsgSel::ms_nNenDay = 0;
int KUiMsgSel::ms_nCaoToiThieu = 0;
int KUiMsgSel::ms_nCaoToiDa = 0;
int KUiMsgSel::ms_nLeDuoi = 0;
int KUiMsgSel::ms_nKhoangKe = 0;
int KUiMsgSel::ms_nDongHoiToiDa = 0;
int KUiMsgSel::ms_nFontHoi = 12;

//--------------------------------------------------------------------------
//	功能：打开窗口，返回唯一的一个类对象实例
//--------------------------------------------------------------------------
KUiMsgSel* KUiMsgSel::OpenWindow(KUiQuestionAndAnswer* pContent)
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMsgSel;
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	if (m_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		m_pSelf->BringToTop();
		m_pSelf->Show(pContent);
	}
	return m_pSelf;
}

//--------------------------------------------------------------------------
//	功能：关闭销毁窗口
//--------------------------------------------------------------------------
void KUiMsgSel::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		m_pSelf->m_bAutoUp = false;
		m_pSelf->m_bAutoDown = false;

		Wnd_ReleaseExclusive((KWndWindow*)m_pSelf);
		if (bDestroy)
		{
			m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
			m_pSelf->Hide();		
	}
}

//--------------------------------------------------------------------------
//	功能：初始化
//--------------------------------------------------------------------------
int KUiMsgSel::Initialize()
{
	AddChild(&m_NpcSpr);
	AddChild(&m_MsgScrollList);
	AddChild(&m_InfoScrollList);
	AddChild(&m_InfoText);
	m_Style &= ~WND_S_VISIBLE;
	Wnd_AddWindow(this, WL_TOPMOST);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	return true;
}

//--------------------------------------------------------------------------
//	功能：载入窗口的界面方案
//--------------------------------------------------------------------------
void KUiMsgSel::LoadScheme(const char* pScheme)
{
	if (m_pSelf == NULL)
		return;
	char		Buff[128];
	KIniFile	Ini;
	sprintf(Buff, "%s\\"SCHEME_INI, pScheme);
	if (Ini.Load(Buff))
	{
		m_pSelf->Init(&Ini, "Main");
		m_pSelf->m_MsgScrollList.Init(&Ini, "Select");
		m_pSelf->m_InfoScrollList.Init(&Ini, "Info");
		m_pSelf->m_InfoText.Init(&Ini, "InfoText");
		m_pSelf->m_NpcSpr.Init(&Ini, "NpcSpr");
		Ini.GetInteger("Main", "NenDau", 0, &ms_nNenDau);
		Ini.GetInteger("Main", "NenThan", 0, &ms_nNenThan);
		Ini.GetInteger("Main", "NenDay", 0, &ms_nNenDay);
		Ini.GetInteger("Main", "CaoToiThieu", 120, &ms_nCaoToiThieu);
		Ini.GetInteger("Main", "CaoToiDa", 520, &ms_nCaoToiDa);
		Ini.GetInteger("Main", "LeDuoi", 12, &ms_nLeDuoi);
		Ini.GetInteger("Main", "KhoangKe", 6, &ms_nKhoangKe);
		Ini.GetInteger("Main", "DongHoiToiDa", 12, &ms_nDongHoiToiDa);
		Ini.GetInteger("InfoText", "Font", 12, &ms_nFontHoi);
		m_pSelf->m_nKeY = -1;
	}
}

//--------------------------------------------------------------------------
//	功能：窗口消息函数
//--------------------------------------------------------------------------
int KUiMsgSel::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
		{
			int x = LOWORD(nParam) - m_nAbsoluteLeft;
			int y = HIWORD(nParam) - m_nAbsoluteTop;
			if (x >= 0 && x <= m_Width &&
				y >= 0 && y <= m_Height)
			{
				int nMLeft = 0;
				int nMTop = 0;
				m_MsgScrollList.GetMessageListBox()->GetPosition(&nMLeft, &nMTop);
				int nMWidth = 0;
				int nMHeight = 0;
				m_MsgScrollList.GetMessageListBox()->GetSize(&nMWidth, &nMHeight);
				if (x >= nMLeft && x <= nMLeft + nMWidth)
				{
					if (y <= nMTop && y >= nMTop - 50)
						m_bAutoUp = true;
					else
						m_bAutoUp = false;

					if (y >= nMTop + nMHeight && y <= nMTop + nMHeight + 50)
						m_bAutoDown = true;
					else
						m_bAutoDown = false;
				}
			}
		}
		break;
	case WND_N_LIST_ITEM_HIGHLIGHT:
		m_bAutoUp = false;
		m_bAutoDown = false;
		if (nParam >= 0)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nParam);
		else
		{
			ChangeCurSel(true);	//经实际测试在nParam < 0 时是向下
			nRet = 1;
		}
		break;
	case WND_N_LIST_ITEM_ACTIVE:
		m_bAutoUp = false;
		m_bAutoDown = false;
		if (nParam >= 0)
			OnClickMsg(nParam);
		break;
	case WM_KEYDOWN:
		m_bAutoUp = false;
		m_bAutoDown = false;
		if (uParam == VK_RETURN)
		{
			nRet = m_MsgScrollList.GetMessageListBox()->GetCurSel();
			if (nRet > 0)
				OnClickMsg(nParam);
			nRet = 1;
		}
		else if (uParam == VK_UP)
		{
			ChangeCurSel(false);
			nRet = 1;
		}
		else if (uParam == VK_DOWN)
		{
			ChangeCurSel(true);
			nRet = 1;
		}
		break;
	default:
		nRet = KWndShowAnimate::WndProc(uMsg, uParam, nParam);
		break;
	}
	return 0;
}

//--------------------------------------------------------------------------
//	功能：响应点击按钮
//--------------------------------------------------------------------------
void KUiMsgSel::OnClickMsg(int nMsg)
{
	CloseWindow(false);
	m_MsgScrollList.GetMessageListBox()->Clear();
	g_pCoreShell->OperationRequest(GOI_QUESTION_CHOOSE, 0, nMsg);
}

void KUiMsgSel::ChangeCurSel(bool bNext)
{
	int nRet = m_MsgScrollList.GetMessageListBox()->GetCurSel();
	if (bNext)
	{
		if (nRet < m_MsgScrollList.GetMessageListBox()->GetMsgCount() - 1)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nRet + 1);
	}
	else
	{
		if (nRet  > 0)
			m_MsgScrollList.GetMessageListBox()->SetCurSel(nRet - 1);
	}
}

//--------------------------------------------------------------------------
//	功能：显示窗口
//--------------------------------------------------------------------------
void KUiMsgSel::Show(KUiQuestionAndAnswer* pContent)
{
	if (pContent && pContent->AnswerCount >= 0)
	{
		m_MsgScrollList.GetMessageListBox()->Clear();
		m_InfoText.SetText(pContent->Question, pContent->QuestionLen);
		int i = 0;
		if (pContent->AnswerCount <= 0)
		{
			m_MsgScrollList.GetMessageListBox()->AddOneMessage("K誸 th骳 i tho筰", -1);
		}
		else
		{
			for (i = 0; i < pContent->AnswerCount ; i++)
			{
				if (!m_MsgScrollList.GetMessageListBox()->AddOneMessage(pContent->Answer[i].AnswerText, pContent->Answer[i].AnswerLen))
					m_MsgScrollList.GetMessageListBox()->AddOneMessage("[--脚本或脚本处理有错！--]", -1);
			}				
		}
	}
	DatCoTheoNoiDung();
	KWndShowAnimate::Show();
	Wnd_SetExclusive((KWndWindow*)this);

	m_uLastScrollTime = IR_GetCurrentTime();
	m_bAutoUp = false;
	m_bAutoDown = false;
}

void KUiMsgSel::Breathe()
{
	if (m_bAutoUp)
	{
		if (IR_IsTimePassed(200, m_uLastScrollTime))
		{
			ChangeCurSel(false);
			m_uLastScrollTime = IR_GetCurrentTime();
		}
	}

	if (m_bAutoDown)
	{
		if (IR_IsTimePassed(200, m_uLastScrollTime))
		{
			ChangeCurSel(true);
			m_uLastScrollTime = IR_GetCurrentTime();
		}
	}
}

/* Hop thoai doi chieu cao theo noi dung (truoc day co dinh 320: hai dong cau tra loi cung de trong
   ca khoang lon). Cau hoi cao dung so dong cua no (toi da DongHoiToiDa), duong ke ngay duoi, danh
   sach cau tra loi cao dung so dong cua cac muc; qua CaoToiDa thi danh sach giu chieu cao toi da va
   cuon nhu cu. Chi doi kich thuoc, khong doi vi tri tren: hop mo xuong tu cho cu. */
void KUiMsgSel::DatCoTheoNoiDung()
{
	if (ms_nNenThan <= 0)
		return;
	int nFont = ms_nFontHoi > 0 ? ms_nFontHoi : 12;
	int nDongHoi = m_InfoText.GetLineCount();
	if (nDongHoi < 1)
		nDongHoi = 1;
	if (ms_nDongHoiToiDa > 0 && nDongHoi > ms_nDongHoiToiDa)
		nDongHoi = ms_nDongHoiToiDa;
	int nTrai = 0, nTren = 0, nRong = 0, nCao = 0;
	m_InfoText.GetPosition(&nTrai, &nTren);
	m_InfoText.GetSize(&nRong, &nCao);
	int nCaoHoi = nDongHoi * (nFont + 1);
	m_InfoText.SetSize(nRong, nCaoHoi);
	m_nKeY = nTren + nCaoHoi + ms_nKhoangKe;

	KWndMessageListBox* pDs = m_MsgScrollList.GetMessageListBox();
	int nDongDap = pDs->GetItemLineCount(pDs->GetMsgCount());
	if (nDongDap < 1)
		nDongDap = 1;
	int nTrenDs = m_nKeY + ms_nKhoangKe;
	int nCaoDs = nDongDap * pDs->GetMinHeight();
	int nCaoDsToiDa = ms_nCaoToiDa - nTrenDs - ms_nLeDuoi;
	if (nCaoDsToiDa > 0 && nCaoDs > nCaoDsToiDa)
		nCaoDs = nCaoDsToiDa;
	m_MsgScrollList.GetPosition(&nTrai, &nTren);
	m_MsgScrollList.SetPosition(nTrai, nTrenDs);
	m_MsgScrollList.GetSize(&nRong, &nCao);
	m_MsgScrollList.SetSize(nRong, nCaoDs);
	pDs->GetSize(&nRong, &nCao);
	pDs->SetSize(nRong, nCaoDs);
	m_MsgScrollList.GetScrollBar()->GetSize(&nRong, &nCao);
	m_MsgScrollList.GetScrollBar()->SetSize(nRong, nCaoDs);

	int nCaoHop = nTrenDs + nCaoDs + ms_nLeDuoi;
	if (nCaoHop < ms_nCaoToiThieu)
		nCaoHop = ms_nCaoToiThieu;
	SetSize(m_Width, nCaoHop);
}

/* Nen: KWndShowAnimate ve khung 0 (dau) tai goc hop va lo phan truot hien; o day ve tiep khung 1
   (than) lat lai den sat day, khung 2 (day) va khung 3 (duong ke). Anh SPR khong co gian duoc nen
   phai ghep. Anh cu mot khung (NenThan=0) thi ve nhu truoc. */
void KUiMsgSel::PaintWindow()
{
	KWndShowAnimate::PaintWindow();
	if (ms_nNenThan <= 0)
		return;
	VeNenGhep(ms_nNenDau, ms_nNenThan, ms_nNenDay);
	if (m_nKeY > 0 && m_nKeY < m_Height - ms_nNenDay)
		VeKhungNen(3, m_nKeY);
}

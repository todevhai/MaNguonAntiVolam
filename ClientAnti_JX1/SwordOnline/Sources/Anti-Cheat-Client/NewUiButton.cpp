#include "NewUiButton.h"
#include <Text.h>
#include <Elem/WndMessage.h>
#include <fstream>
using namespace std;

//KWndButton* gWndButton;
//KWndButton* gWndButton = (KWndButton*)0x63A66C;
//KWndButton* gWndButton = (KWndButton*)CreateWndButton((unsigned int*)(KWndButton*)gWndButton);
//KWndButton gWndButton;
class KIniFile;

KWndButton* KWndButton::m_pPressedDownBtn = NULL;
unsigned int KWndButton::ms_uBtnTipTextColor = 0xffffffff;	//按钮提示名称文字的颜色
int			 KWndButton::ms_nDisableBtnTip = false;		//是否静止按钮的提示文字


void KWndButton::SetAllButtonTipTextColor(unsigned int uColor)
{
	ms_uBtnTipTextColor = uColor;
}

void KWndButton::EnableAllButtonTip(int bEnable)
{
	ms_nDisableBtnTip = !bEnable;
}

KWndButton::KWndButton()
{
	//void* window_ptr = new KWndWindow;
	
	//KWndButton* button_ptr = new KWndButton;
	
	//void* window_ptr = (void*)0x67C03C;
	//void* image_ptr = (unsigned int*)0x63CF4C;
	
	//KWndButton& button_ptr = *(KWndButton*)0x659B94;
	//KWndButton* base_button = (KWndButton*)0x659434;
	//KWndButton* button_ptr = (KWndButton*)0x659B94;
	//KWndButton* button_vtable = (KWndButton*)0x63CBCC;
	//KWndImage& image_ptr = image_ptr;

	//CreateWndButton((DWORD*)this);

	//void* image_ptr = new KWndImage;

	//KWndButton* button_base = (KWndButton*)0x659434;

	//KWndButton* button_array = (KWndButton*)0x659460;

	//KWndButton* button_vtable = (KWndButton*)0x63CBCC;

	////window_vtable = button_vtable;

	//button_base = this;

	//window_vtable = this;

	//*(void**)this = reinterpret_cast<void*>(button_vtable);

	//*this = *static_cast<KWndButton*>(button_array);
	//*this = *static_cast<KWndButton*>(*((KWndButton**)0x659B94));

	*(void**)this = (static_cast<void*>((void**)0x63CBCC));

	//void* vptr_WndButton = *((void**)0x659434);
	//vptr_WndButton = ((void**)this);
	//BaseKWndButton fpCall = (BaseKWndButton*)*((void**)0x425E20);
	//fpCall(this);
	//void(__thiscall * *KIniFile_GetInteger)(void*) = ((void(__thiscall**)(void*))0x425E20);
	//(KWndButton(*this).* ((void**)0x425E20))();
	//int (*KIniFile_GetInteger)(char* This, const char* a1, const char* a2, int a3, int* a4) = ((int(*)(char* This, const char* a1, const char* a2, int a3, int* a4))0x425E20);
	
	//*this = *dynamic_cast<KWndButton*>((KWndButton*)0x659434);

	//**((KWndButton**)this) = *dynamic_cast<KWndButton*>((KWndButton*)0x659434);

	//*this = *static_cast<KWndButton*>(*((KWndButton**)0x63CBC8));
	//CreateWndButton((DWORD*)button_base);

	//KWndImage* ptr = new KWndButton;
	//KWndButton instance;
	//KWndImage* ref = new KWndButton;
	//KWndImage* image_ptr = static_cast<KWndImage*>(ptr);
	//KWndButton* button_ptr = static_cast<KWndButton*>(image_ptr);

	//*(this) = static_cast<KWndButton&>(ref);

	//image_ptr = new KWndButton;
	//*(void**)gWndButton = +((char**)((void**)0x63CBCC));
	//*(this) = *dynamic_cast<KWndButton*>((KWndButton*)button_ptr);
	//*(this) = *static_cast<KWndButton*>((KWndButton*)button_ptr);
	//button_ptr = dynamic_cast<KWndButton*>((KWndButton*)this);
	//*((KWndButton*)image_ptr) = *(static_cast<const KWndButton*>((const KWndButton*)0x659B94));
	//void* window_ptr = new KWndWindow;
	//window_ptr = reinterpret_cast<KWndWindow*>(this);

	//*(void**)this = +*((char**)((void**)0x63CBCC));
	//*(void**)this = ++*((char**)((void**)0x659B94));

	//button_vtable = this;

	//KWndImage& bptr = *this;
	//KWndWindow& bptr_window = bptr;

	//(bptr) = static_cast<KWndButton&>(button_ptr);

	//*(void**)this = static_cast<void**>(+((void**)((void**)0x63CBCC)));
	//image_ptr = static_cast<void**>(window_ptr);
	//image_ptr = static_cast<KWndImage*>(this);

	//image_ptr = reinterpret_cast<void**>(button_ptr);
	//window_ptr = reinterpret_cast<void*>(image_ptr);

	//CreateWndWindow((unsigned int*)window_ptr);
	//CreateWndImage((unsigned int*)image_ptr);
	//void* image_ptr = new KWndImage;
	//image_ptr = this;
	//void* vTablePtr = (void*)0x63CBCC;
	//CreateWndButton((DWORD*)&bptr);
	//CreateWndButton((DWORD*)base_button);
	//*((DWORD*)this) = *((DWORD*)new KWndImage);
	//*(DWORD*)this = *(DWORD*)new KWndButton;
	
	//*this = *static_cast<KWndButton*>((KWndButton*)CreateWndButton((DWORD*)this));
	//image_ptr = static_cast<KWndImage*>(this);

	//unsigned int* button_ptr = (unsigned int*)0x63CBCC;

	//button_ptr = ((unsigned int*)this);

	//gWndButton = dynamic_cast<KWndButton*>((KWndButton*)&button_ptr);

	//*(this) = *dynamic_cast<KWndButton*>((KWndButton*)(void*)0x63CBCC);

	//gWndButton = dynamic_cast<KWndButton*>(gWndImage);
	//CreateWndImage((unsigned int*)((void*)this));
	

	//*((WORD*)&m_Flag) = 0;
	//m_Flag = 0;
	/*(*(WORD*)m_nUpFrame) = 0;
	(*(WORD*)&m_nDownFrame) = 0;
	(*(WORD*)m_nCheckOverFrame) = 0;
	(*(WORD*)m_nOverStartFrame) = 0;
	(*(WORD*)m_nDisableFrame) = 0;
	*(BYTE*)(m_szTip[0]) = 0;
	(*(DWORD*)m_nTipLen) = 0;
	(*(DWORD*)m_nData1) = 0;
	(*(DWORD*)m_nData2) = 0;*/
	//m_Flag = 0;
	/*(*(WORD*)&m_nUpFrame) = 0;
	*((WORD*)(void*)&m_nDownFrame) = 0;
	*&(m_nCheckOverFrame) = 0;
	*&(m_nOverStartFrame) = 0;
	*(WORD*)&(m_nDisableFrame) = -1;*/
	/*m_Flag = 0;
	m_nUpFrame = 0;
	m_nDownFrame = 0;
	m_nCheckOverFrame = 0;
	m_nOverStartFrame = 0;
	m_nDisableFrame = 0;
	m_szTip[0] = 0;
	m_nTipLen = 0;*/
	//m_nData1 = 0;
	//m_nData2 = 0;
	//CreateWndButton((void*)this);
	//void** vTablePtr = *(void***)0x63A66C;

	//DWORD oldProtection;

	//VirtualProtect(vTablePtr, 4, PAGE_EXECUTE_READWRITE, &oldProtection); //Removes page protection at the start of the VMT so we can overwrite its first pointer.

	//gWndButton = reinterpret_cast<KWndButton*>(*vTablePtr); //Stores the pointer to VirtualFn1 from the VMT in a global variable so that it can be accessed again later after its entry in the VMT has been 
																  //overwritten with our hook function.

	//*vTablePtr = &gWndButton; //Overwrite the pointer to VirtualFn1 within the virtual table to a pointer to our hook function (hkVirtualFn1).

	//VirtualProtect(vTablePtr, 4, oldProtection, 0);

	//this->ptr_btn = reinterpret_cast<KWndButton*>(0x63A66C);
	//void* vptr = (void*)this;
	//vptr = (void*)(KWndButton*)0x63A66C;
	//CreateWndButton((unsigned int*)vptr);
	//CreateWndButton((unsigned int*)&gWndButton);
	/** (WORD*)(this + 440) = 0;
	*(WORD*)(this + 442) = 0;
	*(WORD*)(this + 444) = 0;
	*(WORD*)(this + 446) = 0;
	*(WORD*)(this + 448) = 0;
	*(WORD*)(this + 450) = -1;
	*(BYTE*)(this + 452) = 0;
	*(DWORD*)(this + 580) = 0;
	*(DWORD*)(this + 588) = 0;
	*(DWORD*)(this + 584) = 0;*/
}

//KWndButton::~KWndButton()
//{
//	ReleaseWndButton((unsigned int*)this);
//}

//void KWndButton::Clone(KWndButton* pCopy)
//{
//	if (pCopy)
//	{
//		KWndImage::Clone(pCopy);
//		pCopy->m_nUpFrame = m_nUpFrame;
//		pCopy->m_nDownFrame = m_nDownFrame;
//		pCopy->m_nDisableFrame = m_nDisableFrame;
//		pCopy->m_nOverStartFrame = m_nOverStartFrame;
//		pCopy->SetFrame(m_nUpFrame);
//		pCopy->m_Flag = m_Flag & WNDBTN_ES_FILTER;
//		memcpy(pCopy->m_szTip, m_szTip, sizeof(m_szTip));
//		pCopy->m_nTipLen = m_nTipLen;
//	}
//}

//int KWndButton::Init(KIniFile* pIniFile, const char* pSection)
//{
//	ofstream myfile;
//	myfile.open("E:\\KWndButton_Init.txt");
//	myfile << "This size : " << pIniFile << "'\n";
//	myfile << "pSection : " << pSection << "'\n";
//	myfile.close();
//
//	return KWndButton_Init(this, pIniFile, pSection);
//
//	//if (KWndImage::Init(pIniFile, pSection))
//	////if (KWndImage_Init(this, pIniFile, pSection))
//	//{
//	//	return KWndButton_Init(this, pIniFile, pSection);
//	//	/*int	nValue;
//	//	pIniFile->GetInteger(pSection, "Up", 0, &nValue);
//	//	m_nUpFrame = nValue;
//	//	pIniFile->GetInteger(pSection, "Down", 1, &nValue);
//	//	m_nDownFrame = nValue;
//	//	SetFrame((m_Flag & WNDBTN_F_DOWN) ? m_nDownFrame : m_nUpFrame);
//
//	//	pIniFile->GetInteger(pSection, "DisableFrame", -1, &nValue);
//	//	m_nDisableFrame = nValue;
//	//	if (m_nDisableFrame >= 0 && IsDisable())
//	//		SetFrame(m_nDisableFrame);
//
//	//	int	bBool = 0;
//	//	pIniFile->GetInteger(pSection, "NoOverSound", 0, &bBool);
//	//	if (bBool)
//	//		m_Flag |= WNDBTN_ES_NO_OVERSOUND;
//	//	else
//	//		m_Flag &= ~WNDBTN_ES_NO_OVERSOUND;
//
//	//	pIniFile->GetInteger(pSection, "Over", 0, &bBool);
//	//	if (bBool)
//	//	{
//	//		m_Flag |= WNDBTN_ES_ANIMATION;
//	//		pIniFile->GetInteger(pSection, "OverFrame", 2, &nValue);
//	//		m_nOverStartFrame = nValue;
//	//	}
//	//	else
//	//		m_Flag &= ~WNDBTN_ES_ANIMATION;
//
//	//	pIniFile->GetInteger(pSection, "CheckBox", 0, &nValue);
//
//	//	if (nValue)
//	//	{
//	//		m_Flag |= WNDBTN_ES_CHECKBOX;
//	//		pIniFile->GetInteger(pSection, "CheckOver", 0, &nValue);
//	//		m_nCheckOverFrame = nValue;
//	//	}
//	//	else
//	//		m_Flag &= ~WNDBTN_ES_CHECKBOX;
//
//	//	pIniFile->GetInteger(pSection, "SendHoldMsg", 0, &nValue);
//
//	//	if (nValue)
//	//		m_Flag |= WNDBTN_ES_SEND_HOLD_MSG;
//	//	else
//	//		m_Flag &= ~WNDBTN_ES_SEND_HOLD_MSG;
//
//	//	m_nTipLen = 0;
//	//	if (pIniFile->GetString(pSection, "Tip", "", m_szTip, sizeof(m_szTip)))
//	//	{
//	//		m_nTipLen = TEncodeText(m_szTip, strlen(m_szTip));
//	//	}
//	//	pIniFile->GetInteger(pSection, "Enable", 0, &nValue);
//
//	//	if (nValue)
//	//		Enable(1);
//	//	else
//	//		Enable(0);
//
//	//	return true;*/
//	//}
//	//return false;
//}

//int KWndButton::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
//{
//	ofstream myfile;
//	myfile.open("E:\\KWndButton_OnLBtnDown.txt");
//	myfile << "uMsg : " << uMsg << "'\n";
//	myfile << "uParam : " << uParam << "'\n";
//	myfile << "nParam : " << nParam << "'\n";
//	myfile.close();
//
//	if (IsDisable())
//		return KWndWindow::WndProc(uMsg, uParam, nParam);
//
//	switch (uMsg)
//	{
//	case WM_LBUTTONUP:
//		OnLBtnUp();
//		break;
//	case WM_LBUTTONDOWN:
//		OnLBtnDown(false);
//		break;
//	case WND_M_MOUSE_ENTER:
//	case WM_MOUSEMOVE:
//		if (uParam & MK_LBUTTON)
//			OnLBtnDownMove();
//		if ((m_Flag & WNDBTN_F_OVER) == 0 && m_pPressedDownBtn != this)
//		{
//			if (m_pParentWnd)
//				m_pParentWnd->WndProc(WND_N_BUTTON_OVER, (unsigned int)(KWndWindow*)this, 0);//KWndWindow_WndProc(m_pParentWnd, WND_N_BUTTON_OVER, (unsigned int)(KWndWindow*)this, 0); //
//			m_Flag |= WNDBTN_F_OVER;
//
//			/*if ((m_Flag & WNDBTN_ES_NO_OVERSOUND) == 0)
//				UiSoundPlay(UI_SI_OVER_OUTGAME_WND_BTN);*/
//			if (m_Flag & WNDBTN_ES_ANIMATION)
//			{
//				if ((m_Flag & WNDBTN_F_DOWN) == 0)
//					SetFrame(m_nOverStartFrame);
//				else if (m_Flag & WNDBTN_ES_CHECKBOX)
//					SetFrame(m_nCheckOverFrame);
//			}
//		}
//		break;
//	case WND_M_MOUSE_LEAVE:
//		m_Flag &= ~WNDBTN_F_OVER;
//		if (m_pPressedDownBtn == this && m_Flag & WNDBTN_F_DOWN)
//		{
//			m_Flag &= ~WNDBTN_F_DOWN;
//			SetFrame(m_nUpFrame);
//		}
//		else if (m_Flag & WNDBTN_ES_ANIMATION)
//			SetFrame((m_Flag & WNDBTN_F_DOWN) ? m_nDownFrame : m_nUpFrame);
//		/*if (g_MouseOver.IsMoseHoverWndObj((void*)(KWndWindow*)this, 0))
//			g_MouseOver.CancelMouseHoverInfo();*/
//		KWndWindow::WndProc(uMsg, uParam, nParam);
//		//KWndWindow_WndProc(this, uMsg, uParam, nParam);
//		break;
//	case WM_MOUSEHOVER:
//		if (m_pPressedDownBtn == this &&
//			(m_Flag & WNDBTN_ES_SEND_HOLD_MSG) && m_pParentWnd)
//		{
//			m_pParentWnd->WndProc(WND_N_BUTTON_HOLD, (unsigned int)(KWndWindow*)this, 0);
//			//KWndWindow_WndProc(m_pParentWnd, WND_N_BUTTON_HOLD, (unsigned int)(KWndWindow*)this, 0);
//		}
//		/*else if (ms_nDisableBtnTip == false && m_szTip[0] && !g_MouseOver.IsMoseHoverWndObj((void*)(KWndWindow*)this, 0))
//		{
//			char szTip[64];
//
//			int nTipLen = GetToolTipInfo(szTip, 63);
//			szTip[nTipLen] = 0;
//
//			g_MouseOver.SetMouseHoverInfo((void*)(KWndWindow*)this, 0,
//				LOWORD(nParam), HIWORD(nParam), false, true);
//			g_MouseOver.SetMouseHoverTitle(szTip, nTipLen, ms_uBtnTipTextColor);
//		}*/
//		break;
//	case WM_LBUTTONDBLCLK:
//		OnLBtnDown(true);
//		break;
//	default:
//		return KWndWindow::WndProc(uMsg, uParam, nParam); //KWndWindow_WndProc(this, uMsg, uParam, nParam);
//	}
//	return 0;
//}

void KWndButton::OnLBtnUp()
{
	if (m_pPressedDownBtn == this && (m_Flag & WNDBTN_F_DOWN))
	{
		m_Flag &= ~WNDBTN_F_DOWN;
		SetFrame(m_nUpFrame);
		OnButtonClick();
		if (m_pParentWnd)
			KWndWindow_WndProc(m_pParentWnd, WND_N_BUTTON_CLICK, (unsigned int)(KWndWindow*)this, 0);
	}
	m_pPressedDownBtn = NULL;
}

int KWndButton::SetToolTipInfo(char* szTip, int nMax)
{
	m_nTipLen = min(nMax, sizeof(m_szTip) - 1);
	memcpy(m_szTip, szTip, m_nTipLen);
	m_szTip[m_nTipLen] = 0;
	m_nTipLen = TEncodeText(m_szTip, strlen(m_szTip));
	return m_nTipLen;
}

int	KWndButton::GetToolTipInfo(char* szTip, int nMax)
{
	int nTipLen = min(m_nTipLen, nMax);
	memcpy(szTip, m_szTip, nTipLen);
	const char* pKey = GetShortKey();
	if (pKey && pKey[0] != 0 && (nTipLen + (int)strlen(pKey)) < nMax)
	{
		szTip[nTipLen++] = '(';
		strcpy(szTip + nTipLen, pKey);
		nTipLen += strlen(pKey);
		szTip[nTipLen++] = ')';
	}

	return nTipLen;
}

void KWndButton::OnLBtnDown(bool bDoubleClick)
{
	m_Flag &= ~WNDBTN_F_OVER;
	if ((m_Flag & WNDBTN_ES_CHECKBOX) == 0)
		m_pPressedDownBtn = this;
	else
		m_pPressedDownBtn = NULL;

	if ((m_Flag & WNDBTN_ES_CHECKBOX) == 0)
	{
		m_Flag |= WNDBTN_F_DOWN;
		SetFrame(m_nDownFrame);
		if (m_pParentWnd)
		{
		/*	KWndWindow_WndProc(m_pParentWnd, bDoubleClick ? WND_N_BUTTON_DB_CLICK : WND_N_BUTTON_DOWN,
				(unsigned int)(KWndWindow*)this, 0);*/
			m_pParentWnd->WndProc(bDoubleClick ? WND_N_BUTTON_DB_CLICK : WND_N_BUTTON_DOWN, (unsigned int)(KWndWindow*)this, 0);
		}
	}
	else
	{
		if (m_Flag & WNDBTN_F_CHECKED)
		{
			m_Flag &= ~WNDBTN_F_CHECKED;
			SetFrame((m_Flag & WNDBTN_ES_ANIMATION) ? m_nOverStartFrame : m_nUpFrame);
		}
		else
		{
			m_Flag |= WNDBTN_F_CHECKED;
			SetFrame((m_Flag & WNDBTN_ES_ANIMATION) ? m_nCheckOverFrame : m_nDownFrame);
		}
		OnButtonClick();
		//if (m_pParentWnd)
		//	KWndWindow_WndProc(m_pParentWnd, WND_N_BUTTON_CLICK,
		//		(unsigned int)(KWndWindow*)this, (m_Flag & WNDBTN_F_CHECKED)); 
		
		if (m_pParentWnd)
			m_pParentWnd->WndProc(WND_N_BUTTON_CLICK, (unsigned int)(KWndWindow*)this, (m_Flag & WNDBTN_F_CHECKED));
	}
}

void KWndButton::OnLBtnDownMove()
{
	if (m_pPressedDownBtn == this)
	{
		if ((m_Flag & WNDBTN_F_DOWN) == 0)
		{
			m_Flag |= WNDBTN_F_DOWN;
			SetFrame(m_nDownFrame);
		}
	}
}

void KWndButton::PaintWindow()
{
	if ((m_Flag & (WNDBTN_F_OVER | WNDBTN_ES_ANIMATION | WNDBTN_ES_CHECKBOX)) == (WNDBTN_F_OVER | WNDBTN_ES_ANIMATION))
	{
		NextFrame();
		if (m_Image.nFrame < m_nOverStartFrame)
			SetFrame(m_nOverStartFrame);
	}
	KWndImage::PaintWindow();
#ifdef _DEBUG
	if (WND_SHOW_DEBUG_FRAME_TEXT && g_pRepresentShell &&
		m_Image.szImage[0] == 0 && (m_Flag & WNDBTN_F_DOWN) &&
		(WNDBTN_ES_CHECKBOX & m_Flag))
	{
		int x = m_nAbsoluteLeft + strlen(m_Caption) * 8;
		g_pRepresentShell->OutputText(16, "[C]", 3, x + 1, m_nAbsoluteTop + 1, 0xFF000000, 0);
		g_pRepresentShell->OutputText(16, "[C]", 3, x, m_nAbsoluteTop, 0xFFFFFFFF, 0);
	}
#endif
}

int KWndButton::IsButtonChecked()
{
	return (m_Flag & WNDBTN_F_CHECKED);
}

//--------------------------------------------------------------------------
//	功能：设置按钮的按下状态
//--------------------------------------------------------------------------
void KWndButton::CheckButton(int bChecked)
{
	if (bChecked)
	{
		m_Flag |= WNDBTN_F_CHECKED;
		SetFrame(m_nDownFrame);
	}
	else
	{
		m_Flag &= ~WNDBTN_F_CHECKED;
		SetFrame(m_nUpFrame);
	}
}

int	KWndButton::IsButtonActive()
{
	return (m_Flag & WNDBTN_F_OVER);
}

void KWndButton::Enable(int bEnable)
{
	KWndWindow::Enable(bEnable);
	if (m_nDisableFrame >= 0)
	{
		if (bEnable)
			SetFrame((m_Flag & WNDBTN_F_DOWN) ? m_nDownFrame : m_nUpFrame);
		else
			SetFrame(m_nDisableFrame);
	}
}

KWndImageTextButton::KWndImageTextButton()
{
	bPart = true;
}

int	KWndImageTextButton::Init(KIniFile* pIniFile, const char* pSection)
{
	if (KWndButton::Init(pIniFile, pSection))
	{
		pIniFile->GetInteger(pSection, "Part", 1, &bPart);
		char szName[32];
		sprintf(szName, "%s_Image", pSection);
		if (bPart)
			m_ImagePart.Init(pIniFile, szName);
		else
			m_Image.Init(pIniFile, szName);

		sprintf(szName, "%s_Text", pSection);
		m_Text.Init(pIniFile, szName);

		if (bPart)
			AddChild(&m_ImagePart);
		else
			AddChild(&m_Image);
		AddChild(&m_Text);

		m_Text.Enable(false);
		m_Image.Enable(false);
		m_ImagePart.Enable(false);
		return true;
	}
	return false;
}

void KWndImageTextButton::Set2IntText(int nNumber1, int nNumber2, char Separator)
{
	m_Text.Set2IntText(nNumber1, nNumber2, Separator);
}

void KWndImageTextButton::SetIntText(int nNumber, char Separator)
{
	m_Text.Set5IntText(nNumber, Separator);
}

void KWndImageTextButton::SetText(const char* pText, int nLen)	//设置文本文字
{
	m_Text.SetText(pText, nLen);
}

int	KWndImageTextButton::GetText(char* pBuffer, int nSize)		//获取字符串内容
{
	return m_Text.GetText(pBuffer, nSize);
}

void KWndImageTextButton::Set2IntValue(int nNumber1, int nNumber2)
{
	if (bPart)
		m_ImagePart.SetPart(nNumber1, nNumber2);
	else
		m_Image.SetMoveValue(nNumber1, nNumber2);
}
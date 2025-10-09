#pragma once
#include "NewUiImage.h"
#include "Import.h"

class KWndWindow;
class KWndImage;

//#pragma pack(push, 1)
#define	WNDBTN_ES_FILTER		0x00ff
#define	WNDBTN_ES_ANIMATION		0x0001		//鼠标指针停留在窗口上方时显示动画
#define	WNDBTN_ES_CHECKBOX		0x0002		//切换状态的按扭
#define	WNDBTN_ES_SEND_HOLD_MSG	0x0004		//发送鼠标被按住不放的消息
#define	WNDBTN_ES_NO_OVERSOUND	0x0008		//鼠标指针移到上方时，不播放声音效果

#define	WNDBTN_F_DOWN		0x0200			//按纽被按下
#define	WNDBTN_F_CHECKED	WNDBTN_F_DOWN	//按纽被选中
#define	WNDBTN_F_OVER		0x0400		
//鼠标停留在窗口上方
//class KWndImage;

//typedef int (*ptnInit)(KIniFile* pIniFile, const char* pSection);
//typedef (*(void(__thiscall**)(int))(*(DWORD*)this + 32))(0x0x42C0E0);
typedef void BaseKWndButton(void*);

class KWndButton : public KWndImage
{
protected:
	unsigned short	m_Flag;
private:
	short			m_nUpFrame;
	short			m_nDownFrame;
	short			m_nCheckOverFrame;
	short			m_nOverStartFrame;
	short			m_nDisableFrame;
	static	KWndButton* m_pPressedDownBtn;	//被点击但是没有被释放的按钮
	char			m_szTip[128];
	int				m_nTipLen;
	unsigned int	m_nTipColor;
	int				m_nData1;
	//int				m_nData2;
	//int				vptr[512];
public:
	/*KWndButton() { 

		CreateWndWindow((BYTE*)((unsigned int*)((void*)this)));
		CreateWndButton((unsigned int*)this);
		*(void**)this = (static_cast<void*>(((void**)0x63CBCC)));
		CreateWndButton((unsigned int*)this);

	}*/
	KWndButton();
	//KWndButton() : KWndImage() { CreateWndButton(this); };
	//virtual ~KWndButton() { ReleaseWndButton((unsigned int*)this); }
	//KWndButton() {}
	//KWndButton& self() { return *static_cast<KWndButton*>(this); }
	//virtual int		Init(KIniFile* pIniFile, const char* pSection) { return KWndButton_Init(this, pIniFile, pSection); }
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return static_cast<KWndButton*>(this)->Init(pIniFile, pSection); }
	//ptnInit		ptnInit;
	//virtual int		Init(KIniFile* pIniFile, const char* pSection);// { return 0; }
	virtual const char* GetShortKey() { return NULL; }
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return KWndButton_WndProc(this, uMsg, uParam, nParam); }
	//virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);// { return 0; }
	virtual void	PaintWindow();// {}
	int				IsButtonChecked();						//按钮是否出于按下状态
	int				IsButtonActive();
	void			CheckButton(int bChecked);
	void			Enable(int bEnable);
	void			Clone(KWndButton* pCopy);
	static void		SetAllButtonTipTextColor(unsigned int uColor);	//设置所有按钮提示名称文字的颜色
	static void		EnableAllButtonTip(int bEnable);					//禁止/允许所有按钮的提示文字
	virtual int		GetToolTipInfo(char* szTip, int nMax);// { return 0; }
	virtual void	OnButtonClick() {}
	int		SetToolTipInfo(char* szTip, int nMax);
	int		nullsub_1(DWORD* pCopy) { return 0; }
private:
	void			OnLBtnDown(bool bDoubleClick);	//响应鼠标左键在此按下
	void			OnLBtnUp();// {}						//响应鼠标左键在此放开
	void			OnLBtnDownMove();// {}			//响应鼠标左键按下时的移动
private:
	static unsigned int	ms_uBtnTipTextColor;
	static int			ms_nDisableBtnTip;
};

//KWndButton::~KWndButton()
//{
//	ReleaseWndButton((unsigned int*)this);
//}

//#include "WndText.h"
#include "NewUiText.h"
#include "NewUiMovingImage.h"
#include "NewImagePart.h"

class KWndImageTextButton : public KWndButton
{
protected:
	KWndMovingImage m_Image;
	KWndImagePart m_ImagePart;
	KWndText32 m_Text;
	BOOL bPart;
public:
	KWndImageTextButton();
	virtual int		Init(KIniFile* pIniFile, const char* pSection);//初始化
	void			Set2IntText(int nNumber1, int nNumber2, char Separator);
	void			SetIntText(int nNumber, char Separator);
	void			Set2IntValue(int nNumber1, int nNumber2);
	void			SetText(const char* pText, int nLen = -1);	//设置文本文字
	int				GetText(char* pBuffer, int nSize);		//获取字符串内容
};

//#pragma pack(pop)

//class KWndButton : public KWndImage
//{
//protected:
//	unsigned short	m_Flag;
//private:
//	short			m_nUpFrame;
//	short			m_nDownFrame;
//	short			m_nCheckOverFrame;
//	short			m_nOverStartFrame;
//	short			m_nDisableFrame;
//	static	KWndButton* m_pPressedDownBtn;	//被点击但是没有被释放的按钮
//	char			m_szTip[64];
//	int				m_nTipLen;
//public:
//	//virtual int		Init(KIniFile* pIniFile, const char* pSection) { return KWndButton_Init(this, pIniFile, pSection); }
//	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return 0; }
//	virtual const char* GetShortKey() { return NULL; }
//	//virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return KWndButton_WndProc(this, uMsg, uParam, nParam); }
//	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return 0; }
//	virtual void	PaintWindow() {}
//	int				IsButtonChecked() { return 0; }
//	int				IsButtonActive() { return 0; }
//	void			CheckButton(int bChecked) {}
//	void			Enable(int bEnable) {}
//	void			Clone(KWndButton* pCopy) {}
//	static void		SetAllButtonTipTextColor(unsigned int uColor) {}
//	static void		EnableAllButtonTip(int bEnable) {}
//	virtual void	OnButtonClick() {}
//	virtual int		GetToolTipInfo(char* szTip, int nMax) { return 0; }
//	int		SetToolTipInfo(char* szTip, int nMax) { return 0; }
//	//KWndButton() { CreateWndButton((unsigned int*)this); }
//	//KWndButton() { }
//private:
//	void			OnLBtnDown(bool bDoubleClick) {}
//	void			OnLBtnUp() {}
//	void			OnLBtnDownMove() {}
//private:
//	static unsigned int	ms_uBtnTipTextColor;
//	static int			ms_nDisableBtnTip;
//};

extern KWndButton gWndButton;
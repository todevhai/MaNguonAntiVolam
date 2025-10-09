#pragma once
#include "NewUiWindow.h"
#include <KWin32.h>

#define	WNDPTB_MAX_TEXT_LEN	64

#define	WNDPTB_S_CENTRE_ALIGN	0x00008000

#define	WNDPTB_F_BE_PRESSEDDOWN	0x00000001
#define	WNDPTB_F_OVER			0x00000002
#define	WNDPTB_F_CHECK			0x00000004

// 356
class KWndPureTextBtn : public KWndWindow
{
public:
	KWndPureTextBtn() {}
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return 0; }
	virtual void	PaintWindow() {}
	void			SetText(const char* pText, int nLen = -1) {}
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return 0; }
	void			CheckButton(int bChecked) {}
	void			Clone(KWndPureTextBtn* pCopy) {}
	BOOL            IsChecked() { return 0; }
private:
	void			AdjustPosition() {}
private:
	unsigned int	m_Flag;
	char			m_sText[WNDPTB_MAX_TEXT_LEN];
	int				m_nTextLen;			//×Ö·û´®µÄ´æ´¢³¤¶È
	unsigned int	m_NormalColor;
	unsigned int	m_OverColor;
	unsigned int	m_PressedColor;
	unsigned int	m_NormalBorderColor;
	unsigned int	m_OverBorderColor;
	unsigned int	m_PressedBorderColor;
	int				m_nFontSize;
};

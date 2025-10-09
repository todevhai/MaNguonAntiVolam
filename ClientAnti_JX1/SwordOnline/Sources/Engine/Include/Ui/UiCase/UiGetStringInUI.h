// -------------------------------------------------------------------------
//UiGetStringInUI.h
//Author Hoang
// -------------------------------------------------------------------------
#ifndef __UiGetStringInUI_H__
#define __UiGetStringInUI_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndEdit.h"


class KUiGetStringInUI : protected KWndImage
{
public:
	static KUiGetStringInUI*	OpenWindow(const char* pszTitle,
				const char* pszInitString, unsigned int uParam,
				int nMinLen = 0, int nMaxLen = 512,BOOL bNum = FALSE);
	static KUiGetStringInUI*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);
	static void			CloseWindow(bool bDestroy);
private:
	KUiGetStringInUI();
	~KUiGetStringInUI() {}
	void	Initialize();
	void	Show();
	void	Hide();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnOk();
private:
	static KUiGetStringInUI*	m_pSelf;
	KWndText32			m_Title;
	short				m_nMinLen, m_nMaxLen;
	KWndEdit512			m_StringEdit;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;
	KWndWindow*			m_pRequester;
	unsigned int		m_uRequesterParam;
	BOOL				m_bNum;
};


#endif
// -------------------------------------------------------------------------
//	ÎÄ¼şÃû		£º	UiMsgSel2.h
//	´´½¨Õß		£º	Wooy(Wu yue)
//	´´½¨Ê±¼ä	£º	2003-1-6
//	¹¦ÄÜÃèÊö	£º	´ø¹ö¶¯ÌõµÄÏûÏ¢Ñ¡Ôñ
// -------------------------------------------------------------------------
#ifndef __UiMsgSel2_H__
#define __UiMsgSel2_H__

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndText.h"

struct KUiQuestionAndAnswer;

class KUiMsgSel2 : protected KWndShowAnimate
{
public:
	//----½çÃæÃæ°åÍ³Ò»µÄ½Ó¿Úº¯Êı----
	static KUiMsgSel2*	OpenWindow(KUiQuestionAndAnswer* pContent);		//´ò¿ª´°¿Ú£¬·µ»ØÎ¨Ò»µÄÒ»¸öÀà¶ÔÏóÊµÀı
	static KUiMsgSel2*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//ÔØÈë½çÃæ·½°¸
	static void			CloseWindow(bool bDestroy);		//¹Ø±Õ´°¿Ú
private:
	KUiMsgSel2() {}
	~KUiMsgSel2() {}
	void	Show(KUiQuestionAndAnswer* pContent);
	int		Initialize();								//³õÊ¼»¯
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickMsg(int nMsg);			//ÏìÓ¦µã»÷ÏûÏ¢
	void	ChangeCurSel(bool bNext);
	virtual void	Breathe();

private:
	static KUiMsgSel2*	m_pSelf;
	KScrollMessageListBox	m_MsgScrollList;	//±¸Ñ¡ÎÄ×ÖºÍ¹ö¶¯Ìõ
	KWndText512			m_InfoText;	//ËµÃ÷ÎÄ×Ö
	KScrollMessageListBox	m_InfoScrollList;	//±¸Ñ¡ÎÄ×ÖºÍ¹ö¶¯ÌõÖ
	KWndImage           m_NpcSpr;

	//Îª×Ô¶¯¹ö¶¯¼ÓµÄ±äÁ¿
	bool m_bAutoUp;
	bool m_bAutoDown;
	unsigned int	m_uLastScrollTime;
};


#endif // __UiMsgSel2_H__
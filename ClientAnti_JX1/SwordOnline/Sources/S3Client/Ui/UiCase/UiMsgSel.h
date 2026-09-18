// -------------------------------------------------------------------------
//	ÎÄ¼þÃû		£º	UiMsgSel.h
//	´´½¨Õß		£º	Wooy(Wu yue)
//	´´½¨Ê±¼ä	£º	2003-1-6
//	¹¦ÄÜÃèÊö	£º	´ø¹ö¶¯ÌõµÄÏûÏ¢Ñ¡Ôñ
// -------------------------------------------------------------------------
#ifndef __UiMsgSel_H__
#define __UiMsgSel_H__

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndText.h"

struct KUiQuestionAndAnswer;

class KUiMsgSel : protected KWndShowAnimate
{
public:
	//----½çÃæÃæ°åÍ³Ò»µÄ½Ó¿Úº¯Êý----
	static KUiMsgSel*	OpenWindow(KUiQuestionAndAnswer* pContent);		//´ò¿ª´°¿Ú£¬·µ»ØÎ¨Ò»µÄÒ»¸öÀà¶ÔÏóÊµÀý
	static KUiMsgSel*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//ÔØÈë½çÃæ·½°¸
	static void			CloseWindow(bool bDestroy);		//¹Ø±Õ´°¿Ú
private:
	KUiMsgSel() {}
	~KUiMsgSel() {}
	void	Show(KUiQuestionAndAnswer* pContent);
	int		Initialize();								//³õÊ¼»¯
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickMsg(int nMsg);			//ÏìÓ¦µã»÷ÏûÏ¢
	void	ChangeCurSel(bool bNext);
	virtual void	Breathe();
	void	PaintWindow();				/* nen ghep 4 khung: dau, than lat lai, day, duong ke */
	void	DatCoTheoNoiDung();			/* doi chieu cao theo so dong cau hoi + cau tra loi */

private:
	static KUiMsgSel*	m_pSelf;
	KScrollMessageListBox	m_MsgScrollList;	//±¸Ñ¡ÎÄ×ÖºÍ¹ö¶¯Ìõ
	KWndText512			m_InfoText;	//ËµÃ÷ÎÄ×Ö
	KScrollMessageListBox	m_InfoScrollList;	//±¸Ñ¡ÎÄ×ÖºÍ¹ö¶¯ÌõÖ
	KWndImage           m_NpcSpr;
	//Îª×Ô¶¯¹ö¶¯¼ÓµÄ±äÁ¿
	bool m_bAutoUp;
	bool m_bAutoDown;
	unsigned int	m_uLastScrollTime;
	/* Co gian (doc o [Main]/[InfoText] cua UiMsgSel.ini; NenThan=0 thi giu kich thuoc co dinh). */
	static int	ms_nNenDau, ms_nNenThan, ms_nNenDay;	/* cao khung 0/1/2 cua anh nen */
	static int	ms_nCaoToiThieu, ms_nCaoToiDa;		/* chan duoi/tren chieu cao hop thoai */
	static int	ms_nLeDuoi, ms_nKhoangKe;			/* le duoi danh sach; khoang cach quanh duong ke */
	static int	ms_nDongHoiToiDa, ms_nFontHoi;		/* so dong cau hoi toi da; co chu cau hoi */
	int			m_nKeY;								/* toa do doc cua duong ke trong hop, <0 = khong ve */
};


#endif // __UiMsgSel_H__
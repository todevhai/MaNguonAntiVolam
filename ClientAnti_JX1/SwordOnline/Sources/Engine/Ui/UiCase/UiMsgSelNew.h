#ifndef __UiMsgSelNew_H__
#define __UiMsgSelNew_H__

#include "../Elem/WndMessageListBox.h"
#include "../Elem/WndShowAnimate.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndText.h"

struct KUiQuestionAndAnswer;
struct KUiNpcSpr;

class KUiMsgSelNew : protected KWndShowAnimate
{
public:
	//----界面面板统一的接口函数----
	static KUiMsgSelNew*	OpenWindow(KUiQuestionAndAnswer* pContent, KUiNpcSpr* pImage);		//打开窗口，返回唯一的一个类对象实例
	static KUiMsgSelNew*	GetIfVisible();
	static void			LoadScheme(const char* pScheme);	//载入界面方案
	static void			CloseWindow(bool bDestroy);		//关闭窗口
private:
	KUiMsgSelNew() {}
	~KUiMsgSelNew() {}
	void	Show(KUiQuestionAndAnswer* pContent);
	int		Initialize();								//初始化
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnClickMsg(int nMsg);			//响应点击消息
	void	ChangeCurSel(bool bNext);
	virtual void	Breathe();

private:
	static KUiMsgSelNew*	m_pSelf;
	KScrollMessageListBox	m_MsgScrollList;	//备选文字和滚动条
	KWndScrollBar			m_MessageScroll;	//备选文字和滚动条
	KWndText512		m_MessageList;	//说明文字
	KWndImage				 m_NpcSpr;

	//为自动滚动加的变量
	bool m_bAutoUp;
	bool m_bAutoDown;
	unsigned int	m_uLastScrollTime;
};


#endif // __UiMsgSel_H__
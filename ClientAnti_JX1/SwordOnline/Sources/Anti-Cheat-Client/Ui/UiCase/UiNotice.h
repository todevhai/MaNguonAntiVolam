/*****************************************************************************************
//	界面--选择游戏服务器窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-12
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndList.h"
#include "../Elem/WndScrollBar.h"
#include "../Elem/WndShowAnimate.h"

class KUiNotice : protected KWndShowAnimate
{
public:
	static KUiNotice* OpenWindow();				//打开窗口，返回唯一的一个类对象实例
	static void			 CloseWindow(bool bDestroy);//关闭窗口
private:
	KUiNotice();
	~KUiNotice();
	void	Initialize();						//初始化
	void	LoadScheme(const char* pScheme);	//载入界面方案
	void    ShowCompleted();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);	//窗口函数
	void	OnLogin();
	void	OnCancel();
	int		OnKeyDown(unsigned int uKey);
	void	OnClickButton(KWndButton* pBtn);
	void	GetList();

private:
	static KUiNotice* m_pSelf;
	void	UpdateNotice();
private:
	struct KLoginServer* m_pServList;

	KWndText256		m_List;
	KWndText32		m_LoveGame;
	KWndButton		m_Close;
	KWndText256		m_Notice;
	char			m_szLoginBg[32];

	bool			m_bSelRegion;
	int				m_nRegionIndex;
};

/*****************************************************************************************
//	界面--屏幕顶控制操作条
//	Copyright : Kingsoft 2003
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2003-4-22
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once
#include "../Elem/WndToolBar.h"
#include "../Elem/WndButton.h"
#include "../Elem/WndImagePart.h"
#include "../Elem/WndText.h"

class KUiHeaderControlBar : public KWndToolBar
{
public:
	//----界面面板统一的接口函数----
	static KUiHeaderControlBar* OpenWindow();	//打开窗口，返回唯一的一个类对象实例
	static void				CloseWindow();		//关闭窗口
	static void				LoadScheme(const char* pScheme);//载入界面方案
	static void				DefaultScheme(const char* pScheme);//重新初始化界面
	static KUiHeaderControlBar* GetSelf()	{return m_pSelf;}
private:
	~KUiHeaderControlBar() {}
	void	Initialize();							//初始化
	void	Breathe();
	void	UpdateData1();
private:
	static KUiHeaderControlBar*	m_pSelf;
	KWndText80	m_LevelText;
	KWndText80	m_RankWorldText;
	/* Bon thanh: anh ve theo ti le + o chu hien so. Thu tu tren thanh
	   la the luc, sinh luc, noi luc, kinh nghiem - trai sang phai. */
	KWndImagePart	m_Stamina;
	KWndImagePart	m_Life;
	KWndImagePart	m_Mana;
	KWndImagePart	m_Exp;
	KWndText32	m_StaminaText;
	KWndText32	m_LifeText;
	KWndText32	m_ManaText;
	KWndText32	m_ExpText;
};
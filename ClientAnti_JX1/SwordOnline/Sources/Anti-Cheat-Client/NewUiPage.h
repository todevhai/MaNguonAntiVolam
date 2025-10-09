#pragma once
#include "NewUiWindow.h"
#include "NewUiShowAnimate.h"

class KWndPage : public KWndImage
{
public:
	virtual int	WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return 0; }
};

class KWndButton;

class KWndPageSet : public KWndShowAnimate
{
public:
	//virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return KWndPageSet_WndProc(this, uMsg, uParam, nParam); }
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam) { return 0; }
	//bool			AddPage(KWndPage* pPage, KWndButton* pPageBtn) { return KWndPageSet_AddPage(this, pPage, pPageBtn); }
	bool			AddPage(KWndPage* pPage, KWndButton* pPageBtn) { return 0; }
	//bool			ActivePage(int nPageIndex) { return KWndPageSet_ActivePage(this, nPageIndex); }
	bool			ActivePage(int nPageIndex) { return 0; }
	int				GetActivePageIndex() { return m_nAcitvePage; }
	KWndPage* GetActivePage() { return NULL; }
	KWndPageSet() { CreateWndPageSet((unsigned int*)this); }
	~KWndPageSet() {}

private:
	//void			OnPageBtnClick(KWndWindow* pBtn) { KWndPageSet_OnPageBtnClick(this, pBtn); }
	void			OnPageBtnClick(KWndWindow* pBtn) { }

private:
	struct KWndPageBtnPair
	{
		KWndPage* pPage;						//页面窗口指针
		KWndButton* pPageBtn;					//页面窗口关联按钮指针
	}*m_pPageBtnPairList;			//指向存储全部页面及其关联按钮指针的数组
	int				m_nNumPage;					//页面的数目
	int				m_nAcitvePage;				//当前激活的页面
};
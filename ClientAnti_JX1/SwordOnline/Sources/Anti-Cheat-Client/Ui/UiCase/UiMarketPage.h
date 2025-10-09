#pragma once
#include "../Elem/WndShowAnimate.h"
#include "../elem/WndText.h"
#include "../Elem/WndPureTextBtn.h"

class KUiMarketPage : protected KWndShowAnimate
{
public:
	static KUiMarketPage* OpenWindow();				
	static void			 CloseWindow();
	static KUiMarketPage*	GetIfVisible();		
private:
	KUiMarketPage();
	~KUiMarketPage() {}
	void	Initialize();					
	void	LoadScheme(const char* pScheme);	
	void	CancelMenu();

	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);


private:
	static KUiMarketPage* m_pSelf;
private:
	KWndPureTextBtn	VatPham;
	KWndPureTextBtn	BiKip;
	KWndPureTextBtn	DacThu;
	KWndButton	VP, BK, DT;
	KPopupMenuData*	m_pSkinMenu;
};
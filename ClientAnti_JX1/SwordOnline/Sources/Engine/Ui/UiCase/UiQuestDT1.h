#pragma once
#include "../Elem/WndShowAnimate.h"

class KUiDaTau1 : protected KWndShowAnimate
{
public:
	static KUiDaTau1* OpenWindow();				
	static void			 CloseWindow();
private:
	//KUiDaTau();

	void	Initialize();					
	void	LoadScheme(const char* pScheme);	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);


private:
	static KUiDaTau1* m_pSelf;
private:
	KWndButton	item, point, lucky;
	char			m_szLoginBg[32];
};
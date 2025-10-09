#pragma once
#include "../Elem/WndShowAnimate.h"
#include "../elem/WndText.h"
#include "../Elem/WndMessageListBox.h"
class KUiDaTau : protected KWndShowAnimate
{
public:
	static KUiDaTau* OpenWindow();				
	static void			 CloseWindow();
private:
	//KUiDaTau();

	void	Initialize();					
	void	LoadScheme(const char* pScheme);	
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);


private:
	static KUiDaTau* m_pSelf;
private:
	KWndButton	EXP, random, money;
	KWndMessageListBox text;
};
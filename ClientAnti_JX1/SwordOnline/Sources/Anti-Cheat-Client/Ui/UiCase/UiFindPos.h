//*****************************************************************************************
//	Hop thoai dung tim toa do
//	Author	:   Hoang
//	CreateTime:	2014/1/29
//------------------------------------------------------------------------------------------
//*****************************************************************************************/
#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndText.h"
#include "../elem/WndEdit.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"

struct KUiObjAtRegion;

class KUiFindPos : protected KWndShowAnimate
{
public:
	static KUiFindPos* OpenWindow();
	static KUiFindPos* GetIfVisible();
	static void			CloseWindow();
	static void			LoadScheme(const char* pScheme);
private:
	KUiFindPos() {}
	~KUiFindPos() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnOK();
	void	OnCheckInput();
	void	OnDone();
private:
	static KUiFindPos*		m_pSelf;
private:
	KWndEdit32			m_X;
	KWndEdit32			m_Y;
	KWndButton			m_OKBtn;
	KWndButton			m_CancelBtn;
	KWndText80			m_Text;
	KWndText80			m_InfoText;
};
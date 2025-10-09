#pragma once
#include "NewUiMovingImage.h"

class KWndShowAnimate : public KWndMovingImage
{
public:
	virtual int	Init(KIniFile* pIniFile, const char* pSection) { return 0; }
	virtual void Show() {}
	virtual void Hide() {}
	virtual void ShowCompleted() {}
	static void	SetMoveSpeed(int nMoveSpeed, int nFullRange) {}
	static void SetInterval(unsigned int uInterval) {}
	KWndShowAnimate() {}
	void	Clone(KWndShowAnimate* pCopy) {}
protected:
	void	PaintWindow() {}
private:
	static	int	ms_nMoveSpeed;
	static	int	ms_nFullRange;
	int		m_nCurrentSpeed;
	SIZE	m_AppearRange;
	SIZE	m_DisappearRange;
	static	unsigned int ms_uMoveTimeInterval;
	unsigned int	m_uMoveLastTime;		//当前帧切换出来的时间
};
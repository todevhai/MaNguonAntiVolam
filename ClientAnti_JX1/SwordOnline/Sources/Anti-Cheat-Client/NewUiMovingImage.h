#pragma once
#include "NewUiImage.h"

class KWndMovingImage : public KWndImage
{
public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return 0; }
	void			SetMoveValue(int nCurrentValue, int nFullValue) {}
	KWndMovingImage() {}
protected:
	void	Clone(KWndMovingImage* pCopy) {}
protected:
	POINT	m_oFixPos;
	SIZE	m_MoveRange;
	int		m_nCurrentValue;
	int		m_nFullValue;
};
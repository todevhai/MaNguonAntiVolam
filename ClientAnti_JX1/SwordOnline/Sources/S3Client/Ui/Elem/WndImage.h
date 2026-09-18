/*****************************************************************************************
//	界面窗口体系结构--图形窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-7-25
------------------------------------------------------------------------------------------
	显示(spr)图形的窗口。
*****************************************************************************************/
#pragma once
#include "WndWindow.h"
#include "UiImage.h"

#define	WNDIMG_ES_EXCLUDE_TRANS		0x0001		//以图形的形状来作为窗口的形状

class KWndImage : public KWndWindow
{
public:
	virtual int		Init(KIniFile* pIniFile, const char* pSection);//初始化
	virtual int		PtInWindow(int x, int y);				//判断一个点是否在窗口范围内,传入的是绝对坐标
	virtual void	PaintWindow();							//窗体绘制
	void			Clone(KWndImage* pCopy);
	
	void	SetFrame(int nFrame);							//设置图形帧
	int		NextFrame();									//图形换帧
	void	SetImage(short nType, const char* pszImgName, bool bAdjustWndSize = false);
	void	SetMaxFrame(int nMaxFrame);
	/* Ve tiep nen ghep sau PaintWindow (khung 0 = dau da ve): khung 1 (than, cao nThan) lat lai tu
	   nDau toi sat day, khung 2 (day, cao nDay) o day cua so. Anh SPR khong co gian, cua so doi
	   chieu cao theo noi dung thi ghep nen bang cach nay. nThan <= 0: khong lam gi. */
	void	VeNenGhep(int nDau, int nThan, int nDay);
	/* Ve mot khung cua anh nen tai do cao y (tinh tu dinh cua so). */
	void	VeKhungNen(int nKhung, int y);
	void	UpdateTimer();
	KWndImage();
protected:
	KUiImageRef		m_Image;
};
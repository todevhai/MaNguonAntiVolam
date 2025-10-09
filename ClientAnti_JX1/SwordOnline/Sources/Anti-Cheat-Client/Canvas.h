//---------------------------------------------------------------------------
// Sword3 Engine (c) 1999-2000 by Kingsoft
//
// File:	KCanvas.h
// Date:	2000.08.08
// Code:	WangWei(Daphnis)
// Desc:	Header File
//---------------------------------------------------------------------------
#ifndef KCanvas_H
#define KCanvas_H
//---------------------------------------------------------------------------
#include "KList.h"
#include "KMemClass.h"
#include <WinDef.h>
#include <ddraw.h>
//---------------------------------------------------------------------------
//typedef void (* DRAW_FUNC)(void* pNode, void* pCanvas);
//---------------------------------------------------------------------------
class KDrawNode : public KNode
{
public:
	//	DRAW_FUNC	m_pDrawFunc;
	BOOL		m_bChanged;
	long		m_nX;
	long		m_nY;
	long		m_nWidth;
	long		m_nHeight;
	long		m_nColor;
	long		m_nAlpha;
	void* m_pBitmap;
	void* m_pPalette;
};
//---------------------------------------------------------------------------
typedef struct {
	long		x;			// 裁减后的X坐标
	long		y;			// 裁减后的Y坐标
	long		width;		// 裁减后的宽度
	long		height;		// 裁减后的高度
	long		left;		// 上边界裁剪量
	long		top;		// 左边界裁剪量
	long		right;		// 右边界裁剪量
} KClipper;
/*/---------------------------------------------------------------------------
typedef struct {
	long		num;		// 当前的对象数
	long		changed;	// 重绘标志
	KDrawNode*	node[10];	// 绘制对象指针
} KSubCanvas;*/
//---------------------------------------------------------------------------
class KCanvas
{
private:
	//	KList		m_DrawList;// 绘制列表
	//	KMemClass	m_Canvas;// 画布内存对象
	//	KMemClass	m_SubCanvas;//画布子区域
	LPDIRECTDRAWSURFACE*	m_pSurface;
	long		m_nWidth;// 画布宽度
	long		m_nHeight;// 画布高度

public:
	//	void*		m_pCanvasAlloc;// 分配画布内存
	void* m_pCanvas;// 画布内存   // 对齐256字节
	long		m_nMask16;// 16bit RGB Mask
	long		m_nMask32;// 32bit RGB Mask
	long		m_nScreenWidth;// 屏幕宽度
	long		m_nScreenHeight;// 屏幕高度
	long		m_nScreenPitch;// 屏幕跨度
//	long		m_nSubRows;// 行数目
//	long		m_nSubCols;// 列数目
	RECT		m_ClipRect;// 裁减区域
	BOOL		m_bChanged;// 改变标志
	BOOL        m_dwScreenMode;
private:
	//	virtual void		SubAddNode(int nX,int nY,int nW,int nH,KDrawNode* pNode);
	//	virtual void		SubAdd(int nCol, int nRow, KDrawNode* pNode);
	//	virtual void		SubChangedNode(int nX,int nY,int nW,int nH,KDrawNode* pNode);
	//	virtual void		SubChanged(int nCol, int nRow);
	//	virtual void		SubMarkChanged();
	virtual void		UpdateBackBuffer(LPRECT lpRect);
	virtual void		UpdateFrontBuffer(LPRECT lpRect);
	virtual bool		GetUpdateRect(LPRECT lpSrc, LPRECT lpDest);
public:
	KCanvas();
	~KCanvas();
	bool		Init(int nWidth, int nHeight, unsigned int* m_Canvas, unsigned int* m_DirectDraw);
	virtual void		Terminate();
	virtual void		Changed(BOOL bChanged) { m_bChanged = bChanged; };
	virtual void		FillCanvas(WORD wColor);
	virtual void		UpdateCanvas();
	virtual void		UpdateScreen();
	virtual int			GetWidth() { return m_nWidth; }
	virtual int			GetHeight() { return m_nHeight; }
	void* LockCanvas(int& nPitch);
	virtual void		UnlockCanvas();
	virtual void		BltSurface(LPDIRECTDRAWSURFACE pSurface, RECT* pDestRect);
	//	virtual void		UpdateScreen(LPRECT lpRect);
	virtual BOOL		RectClip(LPRECT pRect);
	virtual BOOL		MakeClip(long nX, long nY, long nWidth, long nHeight, KClipper* pClipper);
	virtual void		SetClipRect(LPRECT pRect) { m_ClipRect = *pRect; };
	virtual void		GetClipRect(LPRECT pRect) { *pRect = m_ClipRect; };
	virtual void		DrawPixel(int nX, int nY, int nColor);
	virtual void		DrawPixelAlpha(int nX, int nY, int nColor, int nAlpha);
	virtual void		DrawLine(int nX1, int nY1, int nX2, int nY2, int nColor);
	virtual void		DrawLineAlpha(int nX1, int nY1, int nX2, int nY2, int nColor, int nAlpha);
	virtual void		DrawSprite(int nX, int nY, int nWidth, int nHeight, void* lpSprite, void* lpPalette);
	virtual void		DrawSpriteMixColor(int nX, int nY, int nWidth, int nHeight, void* lpSprite, void* lpPalette, int nColor, int nAlpha);
	virtual void		DrawSpriteWithColor(int nX, int nY, int nWidth, int nHeight, void* lpSprite, void* lpPalette, int nColor, int nAlpha);
	virtual void		DrawSpriteAlpha(int nX, int nY, int nWidth, int nHeight, void* lpSprite, void* lpPalette, int nExAlpha);
	virtual void		DrawSpriteBorder(int nX, int nY, int nWidth, int nHeight, int nColor, void* lpSprite);
	virtual void		DrawSprite3LevelAlpha(int nX, int nY, int nWidth, int nHeight, void* lpSprite, void* lpPalette);
	virtual void		DrawFont(int nX, int nY, int nWidth, int nHeight, int nColor, int nAlpha, void* lpFont);
	virtual void		DrawFontWithBorder(int nX, int nY, int nWidth, int nHeight, int nColor, int nAlpha, void* lpFont, int nBorderColor);
	virtual void		DrawFontSolid(int nX, int nY, int nWidth, int nHeight, int nColor, int nAlpha, void* lpFont);
	virtual void		DrawBitmap(int nX, int nY, int nWidth, int nHeight, void* lpBitmap, void* lpPalette);
	virtual void		DrawBitmap16(int nX, int nY, int nWidth, int nHeight, void* lpBitmap);
	virtual void		DrawBitmap16mmx(int nX, int nY, int nWidth, int nHeight, void* lpBitmap);
	virtual void		DrawBitmap16win(int nX, int nY, int nWidth, int nHeight, void* lpBitmap);
	virtual void		DrawFade(int nX, int nY, int nWidth, int nHeight, int nAlpha);
	virtual void        Clear(int nX, int nY, int nWidth, int nHeight, int nColor);
	virtual void        DotClear(int nX, int nY, int nWidth, int nHeight, int nColor);
	virtual void        ClearAlpha(int nX, int nY, int nWidth, int nHeight, int nColor, int nAlpha);
};


extern KCanvas* g_pCanvas;
//---------------------------------------------------------------------------
#endif

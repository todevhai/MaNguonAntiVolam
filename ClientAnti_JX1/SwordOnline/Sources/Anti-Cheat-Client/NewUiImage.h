#pragma once
//#pragma pack(push, 1)

//#include "Elem/WndMessage.h"
#include "Import.h"
//#include "NewImage.h"

//#include <iRepresent/KRepresentUnit.h>
#include "NewUiWindow.h"
#include "NewImage.h"

#define	WNDIMG_ES_EXCLUDE_TRANS		0x0001		//以图形的形状来作为窗口的形状
#define	WNDIMG_ES_EXCLUDE_MSG		0x0002		//以图形的形状来作为窗口的形状

//class KWndWindow;

// 440
class KWndImage : public KWndWindow
{
public:
	//virtual int		Init(KIniFile* pIniFile, const char* pSection) { return KWndImage_Init(*(void**)this, pIniFile, pSection); }
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return static_cast<KWndImage*>(this)->Init(pIniFile, pSection); }
	virtual int		PtInWindow(int x, int y) { return KWndImage_PtInWindow(*(void**)this, x, y); }
	virtual void	PaintWindow() { KWndImage_PaintWindow(*(void**)this); }
	//void			Clone(KWndImage* pCopy);
	void	SetFrame(int nFrame) {}
	int		NextFrame() { return 0; }
	void	SetImage(short nType, const char* pszImgName, bool bAdjustWndSize = false) {}
	void	SetMaxFrame(int nMaxFrame) {}
	void	UpdateTimer() {}
	//KWndImage() { CreateWndImage((unsigned int*)((void*)this)); }
	
	/*KWndImage() {
		*((DWORD*)&this->m_Image.nNumFrames) = 0;
		IR_InitUiImageRef(m_Image);
	}*/
	KWndImage();
	//KWndImage() : KWndWindow() {
	//	*(void**)this = (static_cast<void*>(((void**)0x63CF4C)));
	//	//CreateWndImage(*(DWORD**)this);
	//};
protected:
	KUiImageRef		m_Image;
};

//KWndImage::~KWndImage()
//{
//	ReleaseWndButton((unsigned int*)this);
//}
//#pragma pack(pop)

extern KWndImage gWndImage;
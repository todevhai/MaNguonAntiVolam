/*******************************************************************************
File        : UiChangeAvatar.h
********************************************************************************/


#if !defined(AFX_KUiChangeAvatar_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_)
#define AFX_KUiChangeAvatar_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_

/*---------------------------*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*------------------------------------------*/
#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"
#include "../../../core/src/coreshell.h"	// defMAX_AVATAR qua KProtocol.h

/*---------------------------------------------------------*/
class KUiChangeAvatar : KWndImage
{
public:

	static        KUiChangeAvatar* OpenWindow(); 
	static        KUiChangeAvatar* GetIfVisible();
	static void   CloseWindow(bool bDestory = TRUE); 
	static void   LoadScheme(const char* pScheme);

public:

private:
	static        KUiChangeAvatar *m_pSelf;
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	void          Initialize();       
	void	LoadScheme(class KIniFile* pIni);	


private:

	// Mot o cho moi chan dung. Truoc day la 15 bien roi va tach nam/nu; nay dung
	// mang vi bo anh la CHUNG cho ca hai phai (35 anh 70x70, xem defMAX_AVATAR).
	KWndButton	m_Cell[defMAX_AVATAR];
	KWndButton	m_DoneBtn, m_DongBtn;
};


#endif

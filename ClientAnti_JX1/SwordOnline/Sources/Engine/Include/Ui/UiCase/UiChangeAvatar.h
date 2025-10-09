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

	KWndButton	m_Avatar1, m_Avatar2, m_Avatar3, m_Avatar4, m_Avatar5, m_Avatar6, m_Avatar7, m_Avatar8, m_Avatar9, m_Avatar10,
			  m_Avatar11, m_Avatar12, m_Avatar13, m_Avatar14 ,m_Avatar15, m_DoneBtn, m_DongBtn;
};


#endif

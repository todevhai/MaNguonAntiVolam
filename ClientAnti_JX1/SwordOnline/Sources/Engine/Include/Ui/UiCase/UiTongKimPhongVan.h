/*******************************************************************************
File        : UiTongKimPhongVan.h
********************************************************************************/


#if !defined(AFX_KUITongKimPhongVan_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_)
#define AFX_KUITongKimPhongVan_H__7CC8F62F_9A1C_4AE2_A73B_BC945DE5185F__INCLUDED_

/*---------------------------*/
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/*------------------------------------------*/
#include "../Elem/WndList.h"	
#include "../elem/wndtext.h"
#include "../elem/wndedit.h"
#include "../elem/wndimage.h"
#include "../elem/wndbutton.h"
#include "../elem/wndlabeledbutton.h"
#include "../Elem/WndMessageListBox.h"

/*---------------------------------------------------------*/
class KUiTongKimPhongVan : KWndImage
{
public:

	static        KUiTongKimPhongVan* OpenWindow(); 
	static        KUiTongKimPhongVan* GetIfVisible();
	static void   CloseWindow(bool bDestory = TRUE); 
	static void   LoadScheme(const char* pScheme);

public:

private:
	static        KUiTongKimPhongVan *ms_pSelf;
	virtual int   WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	void          Initialize();         


private:


    KWndLabeledButton	m_DongBtn;
	KWndText32	m_Hang1, m_Hang2, m_Hang3, m_Hang4, m_Hang5, m_Hang6, m_Hang7, m_Hang8, m_Hang9, m_Hang10,
			  m_Diem1, m_Diem2, m_Diem3, m_Diem4 ,m_Diem5, m_Diem6, m_Diem7, m_Diem8, m_Diem9, m_Diem10;
	KWndMessageListBox	m_ChuThich;
//	KWndList	m_XepHang;
//	KWndList	m_Name;
//	KWndList	m_Diem;
};


#endif

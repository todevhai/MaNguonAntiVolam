/********************************************************************
File        : UiTongKimPhongVan.cpp
Creator     : Fyt(Fan Zhanpeng)
create data : 08-29-2003(mm-dd-yyyy)
Description : 创建帮会的表单
*********************************************************************/

#include "KWin32.h"
#include "KIniFile.h"
#include "CoreShell.h"
#include "GameDataDef.h"

#include "../elem/wnds.h"
#include "../Elem/WndMessage.h"

#include "../UiBase.h"
#include "../UiSoundSetting.h"

#include "UiMarket.h"
#include "UiItem.h"
#include "UiTongKimPhongVan.h"


#define STR_BOX "UiTongKimPhongVan.ini"
//#define	XEPHANG_FILE				"\\Settings\\XepHangTongKim.txt"

KUiTongKimPhongVan* KUiTongKimPhongVan::ms_pSelf = NULL;

extern iCoreShell *g_pCoreShell;


KUiTongKimPhongVan* KUiTongKimPhongVan::OpenWindow()
{
	if (ms_pSelf == NULL)
	{
		ms_pSelf = new KUiTongKimPhongVan;
		if (ms_pSelf)
			ms_pSelf->Initialize();
	}
	if (ms_pSelf)
	{
		UiSoundPlay(UI_SI_WND_OPENCLOSE);
		ms_pSelf->BringToTop();
		ms_pSelf->Show();
	}
	return ms_pSelf;
}


/*********************************************************************
* 功能：如果窗口正被显示，则返回实例指针
**********************************************************************/
KUiTongKimPhongVan* KUiTongKimPhongVan::GetIfVisible()
{
	if (ms_pSelf && ms_pSelf->IsVisible())
		return ms_pSelf;
	return NULL;
}


/*********************************************************************
* 功能：关闭窗口，同时可以选则是否删除对象实例
**********************************************************************/
void KUiTongKimPhongVan::CloseWindow(bool bDestory)
{
	if (ms_pSelf)
	{
		ms_pSelf->Hide();
		if (bDestory)
		{
			ms_pSelf->Destroy();
			ms_pSelf = NULL;
		}
	}
}


/*********************************************************************
* 功能：初始化
**********************************************************************/
void KUiTongKimPhongVan::Initialize()
{
	AddChild(&m_DongBtn);
	AddChild(&m_Hang1);
	AddChild(&m_Hang2);
	AddChild(&m_Hang3);
	AddChild(&m_Hang4);
	AddChild(&m_Hang5);
	AddChild(&m_Hang6);
	AddChild(&m_Hang7);
	AddChild(&m_Hang8);
	AddChild(&m_Hang9);
	AddChild(&m_Hang10);
	AddChild(&m_Diem1);
	AddChild(&m_Diem2);
	AddChild(&m_Diem3);
	AddChild(&m_Diem4);
	AddChild(&m_Diem5);
	AddChild(&m_Diem6);
	AddChild(&m_Diem7);
	AddChild(&m_Diem8);
	AddChild(&m_Diem9);
	AddChild(&m_Diem10);
	AddChild(&m_ChuThich);

	char Scheme[256];
	g_UiBase.GetCurSchemePath(Scheme, 256);
	LoadScheme(Scheme);

	Wnd_AddWindow(this);
}


/*********************************************************************
* 功能：载入界面方案
**********************************************************************/
void KUiTongKimPhongVan::LoadScheme(const char* pScheme)
{
	if(ms_pSelf)
	{
    	char		Buff[256];
    	KIniFile	Ini;
    	sprintf(Buff, "%s\\%s", pScheme, STR_BOX);
    	if (Ini.Load(Buff))
    	{
		    ms_pSelf->Init(&Ini, "Main");
			ms_pSelf->m_DongBtn.Init(&Ini, "DongBtn");
			ms_pSelf->m_Hang1.Init(&Ini, "Hang1");
			ms_pSelf->m_Hang1.SetText("1.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang2.Init(&Ini, "Hang2");
			ms_pSelf->m_Hang2.SetText("2.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang3.Init(&Ini, "Hang3");
			ms_pSelf->m_Hang3.SetText("3.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang4.Init(&Ini, "Hang4");
			ms_pSelf->m_Hang4.SetText("4.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang5.Init(&Ini, "Hang5");
			ms_pSelf->m_Hang5.SetText("5.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang6.Init(&Ini, "Hang6");
			ms_pSelf->m_Hang6.SetText("6.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang7.Init(&Ini, "Hang7");
			ms_pSelf->m_Hang7.SetText("7.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang8.Init(&Ini, "Hang8");
			ms_pSelf->m_Hang8.SetText("8.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang9.Init(&Ini, "Hang9");
			ms_pSelf->m_Hang9.SetText("9.Ch璦 c藀 nh藅");
			ms_pSelf->m_Hang10.Init(&Ini, "Hang10");
			ms_pSelf->m_Hang10.SetText("10.Ch璦 c藀 nh藅");
			ms_pSelf->m_Diem1.Init(&Ini, "Diem1");
			ms_pSelf->m_Diem1.SetText("0");
			ms_pSelf->m_Diem2.Init(&Ini, "Diem2");
			ms_pSelf->m_Diem2.SetText("0");
			ms_pSelf->m_Diem3.Init(&Ini, "Diem3");
			ms_pSelf->m_Diem3.SetText("0");
			ms_pSelf->m_Diem4.Init(&Ini, "Diem4");
			ms_pSelf->m_Diem4.SetText("0");
			ms_pSelf->m_Diem5.Init(&Ini, "Diem5");
			ms_pSelf->m_Diem5.SetText("0");
			ms_pSelf->m_Diem6.Init(&Ini, "Diem6");
			ms_pSelf->m_Diem6.SetText("0");
			ms_pSelf->m_Diem7.Init(&Ini, "Diem7");
			ms_pSelf->m_Diem7.SetText("0");
			ms_pSelf->m_Diem8.Init(&Ini, "Diem8");
			ms_pSelf->m_Diem8.SetText("0");
			ms_pSelf->m_Diem9.Init(&Ini, "Diem9");
			ms_pSelf->m_Diem9.SetText("0");
			ms_pSelf->m_Diem10.Init(&Ini, "Diem10");
			ms_pSelf->m_Diem10.SetText("0");
			ms_pSelf->m_ChuThich.Init(&Ini, "ChuThich");
			Ini.GetString("Notice", "HuongDan", "", Buff, sizeof(Buff));
			ms_pSelf->m_ChuThich.AddOneMessage(Buff,sizeof(Buff));
			
		}
		/*--------------------*/
	}
	/*--------------------*/
}


int KUiTongKimPhongVan::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	switch(uMsg)
	{
	case WND_N_BUTTON_CLICK:
		if(uParam == (unsigned int)&m_DongBtn)
		{
			CloseWindow();
		}
	default:
		return KWndImage::WndProc(uMsg, uParam, nParam);
		break;
	}
    return 1;
}

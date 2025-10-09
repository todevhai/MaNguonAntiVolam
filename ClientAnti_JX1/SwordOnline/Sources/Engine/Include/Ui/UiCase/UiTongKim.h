/*****************************************************************************************
//Author Hoang
*****************************************************************************************/
#pragma once
#include "../elem/wndpage.h"
#include "../Elem/WndText.h"
#include "../Elem/WndShowAnimate.h"
#include "../elem/WndObjContainer.h"

enum UI_PLAYER_ATTRIBUTE;
struct KUiPlayerRuntimeInfo;
struct KUiObjAtRegion;
struct KUiPlayerAttribute;


class KUiTongKim : public KWndShowAnimate
{
public:
	static KUiTongKim*	OpenWindow();
	static KUiTongKim*	GetIfVisible();	
	static void			CloseWindow(bool bDestroy);	
	static void			LoadScheme(const char* pScheme);
						
	void	UpdateData();

private:
	KUiTongKim() {}
	~KUiTongKim() {}
	void	Initialize();						
	void	LoadScheme(class KIniFile* pIni);
private:
	static KUiTongKim*	m_pSelf;

private:
	KWndText32	m_TimeText;
	KWndText32	m_DiemText;
	KWndText32	m_PlayerText;	
	KWndText32	m_NpcText;
	KWndText32	m_XepHangText;

	int	m_nTime;
	int	m_nDiem;
	int	m_nPlayer;
	int  m_nNpc;
	int  m_nXepHang;
	KWndText256	m_Time, m_Diem, m_Player, m_Npc, m_XepHang;


};

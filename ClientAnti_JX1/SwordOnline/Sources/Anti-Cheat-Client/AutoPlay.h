#pragma once
//#include "UiSoundSetting.h"
#include "NewUiButton.h"
#include "NewUiText.h"
#include "NewUiPureTextBtn.h"
#include "NewUiPage.h"
#include "NewComWindow.h"

class KUiFighting : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
	void	Breathe();
	void	Active(BOOL bAct); //ham kich hoat
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
private:
	KWndText32				m_EatLife;			//nhung dong` chu~
	KWndText32				m_EatMana;			//nhung dong` chu~
	KWndText32				m_EatTownPortal; 	//nhung dong` chu~
	KWndText32				m_AttRadius;		//nhung dong` chu~
	KWndText32				m_Distance; 		//nhung dong` chu~
	//KWndEdit32				m_Life;				//can' mau'
	//KWndEdit32				m_Mana;             //can' mana
	//KWndEdit32				m_Portal;           //can' tho dia phu
	//KWndEdit32				m_Radius;           //pham vi tan cong
	//KWndEdit32				m_Dstnce;           //khoang cach tiep can
	KWndText32			m_NhatTienText;
	KWndText32			m_NhatItemText;
	KWndText32			m_NhatTaskText;
	KWndButton	m_NhatTien;
	KWndButton	m_NhatItem;
	KWndButton	m_NhatTask;


	//-------------------------------------------------------
	BOOL					m_bIsActived;		//duoc kich hoat hay chua?
	unsigned int			m_uLifeTime;		//thoi gian cach' cua mau'
	unsigned int			m_uManaTime;		//thoi gian cach' cua mana
};

class KUiSetting : public KWndPage
{
public:
	void	Initialize();
	void	LoadScheme();
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	KWndText32			m_AnPlayerText;
	KWndText32			m_AnNpcText;
	KWndText32			m_AnMissleText;
	KWndButton	m_AnPlayer;
	KWndButton	m_AnNpc;
	KWndButton	m_AnMissle;
};

class KUiAutoPlay : public KWndPageSet
{
public:
	static KUiAutoPlay* OpenWindow();
	static KUiAutoPlay* GetIfVisible();
	static void		 CloseWindow(bool bDestroy);
	static void		 LoadScheme();
	void			Breathe();
private:
	KUiAutoPlay() {}
	~KUiAutoPlay() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();
private:
	static KUiAutoPlay* m_pSelf;
private:
	KWndText32			m_Title;
	KWndButton			m_Close;
	KWndButton			m_Active;
	KWndPureTextBtn		m_ActiveText;
	KWndText32			m_FightText;
	KWndText32			m_SettingText;
	KUiFighting			m_FightPad;	//Bang~ Chien' dau'
	KUiSetting			m_SettingPad;//Bang~ nhat do`
	//--------------------------------------------------------
	KWndButton			m_FightingBtn;	//nut chien' dau'
	KWndButton			m_SettingBtn;	//nut nhat do`
	//--------------------------------------------------------
	char				m_szTitle[24]; //ten auto
	//Player_AutoPlay		m_BtnAutoPlay;
};


#pragma once
#include "NewUiText.h"
#include "NewUiImage.h"

struct Mission
{
	int		nIndex;
	int		nMissionID;
	KWndImage		m_Image;
	int		nFloatTime;
	int		m_LeftText;
	int		m_TopText;
	int		m_TickTime;
	char	m_TextMission[32];
	char	m_TextTime[32];
};

class KUiMissionTime : public KWndImage
{
public:
	static KUiMissionTime* OpenWindow();
	void	LoadScheme(const char* pScheme);
	static void __fastcall		UpdateData();
	static void		CloseWindow(bool bDestroy);
	virtual int		Init(KIniFile* pIniFile, const char* pSection) { return 0; }
	KUiMissionTime();
private:
	void				Initialize();
	void				LoadScheme(KIniFile* pIni);
	int			WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	int		PtInWindow(int, int) { return 0; }
	void		PaintWindow();
	void		Hide();
	void		Show();
	void		SetSize(int nWidth, int nHeight) {}
	void		Enable(int bEnable) {}
	virtual void	Breathe() {}
private:
	static KUiMissionTime* m_pSelf;
private:
	KWndImage				m_Image[10];
	KWndText32				m_Text80Time[10];
	int				m_IsShow;
	//int				m_MissionID;
	//KWndText32				m_Text32_Mission_Name;
};

extern Mission m_ListMission[10];

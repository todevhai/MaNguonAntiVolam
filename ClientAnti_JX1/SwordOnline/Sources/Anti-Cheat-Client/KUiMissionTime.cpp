#include "KUiMissionTime.h"

KUiMissionTime* KUiMissionTime::m_pSelf = NULL;
Mission	m_ListMission[10];

KUiMissionTime::KUiMissionTime()
{
	m_IsShow = 0;
}

KUiMissionTime* KUiMissionTime::OpenWindow()
{
	if (m_pSelf == NULL)
	{
		m_pSelf = new KUiMissionTime;
		CreateWndImage((DWORD*)m_pSelf);
		if (m_pSelf)
			m_pSelf->Initialize();
	}
	//unsigned char* m_Text = "Thời gian còn lại: 15 phút 60 giây";
	if (m_pSelf)
	{
		if (*(DWORD*)((unsigned int)m_pSelf + 4) != WND_S_VISIBLE)
			m_pSelf->Show();

		m_pSelf->UpdateData();
	}

	return m_pSelf;
}

void KUiMissionTime::Initialize()
{

	for (int i = 0; i < 5; ++i)
	{
		CreateWndImage((DWORD*)&m_Image[i]);
		

		SetObjectWndText32((unsigned int*)&m_Text80Time[i]);
		
	}

	char Scheme[256];
	b_sub_41A4A0((const char*)0x68D090, Scheme, 256);
	this->LoadScheme(Scheme);
	b_Wnd_AddWindow(this, 1);
}

void KUiMissionTime::LoadScheme(const char* pScheme)
{
	int	nValue;
	char		Buff[128];
	KIniFile	Ini;

	if (Ini.Load("\\Ui\\ui3\\MissionTime.ini"))
	{
		m_pSelf->LoadScheme(&Ini);
	}
}

void KUiMissionTime::LoadScheme(KIniFile* pIni)
{
	int	nValue;

	KWndImage_Init(this, pIni, "Main");

	for (int i = 0; i < 5; ++i)
	{
		b_AddChild(this, &m_Image[i]);

		KWndImage_Init(&m_Image[i], pIni, "Mission");

		_SetPosition(&m_Image[i], 0, 28 * i);

		b_AddChild(this, &m_Text80Time[i]);

		KWndText32_Init(&m_Text80Time[i], pIni, "MissionTime");

		_SetPosition(&m_Text80Time[i], 28, 24 * i);

		sprintf(m_ListMission[i].m_TextMission, "%s", "Tèng Kim");
		sprintf(m_ListMission[i].m_TextTime, "%d phót %d gi©y", 5, 60);

		b_HideWndWindow(&m_Image[i]);
		b_HideWndWindow(&m_Text80Time[i]);
		//b_ShowWndWindow(&m_Image[i]);
		//b_ShowWndWindow(&m_Text80Time[i]);

	}
	pIni->Clear();
}

void KUiMissionTime::CloseWindow(bool bDestroy)
{
	if (m_pSelf)
	{
		if (bDestroy)
		{
			if (*(DWORD*)((unsigned int)m_pSelf + 4) == WND_S_VISIBLE)
				m_pSelf->Hide();

			for (int i = 0; i < 5; ++i)
			{
				//m_pSelf->m_ImageSkill[i].Destroy();
			}

			//m_pSelf->Destroy();
			m_pSelf = NULL;
		}
		else
		{
			if (*(DWORD*)((unsigned int)m_pSelf + 4) == WND_S_VISIBLE)
				m_pSelf->Hide();
		}
		//m_pSelf->Hide();
	}
}

void KUiMissionTime::Hide()
{
	if (m_pSelf)
	{
		b_HideWndWindow(this);
	}
}

void KUiMissionTime::Show()
{
	if (m_pSelf)
	{
		b_ShowWndWindow(this);
	}
}

void KUiMissionTime::PaintWindow()
{
	_PaintWindow(this);
}

int KUiMissionTime::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)
{
	int	nRet = 0;
	/*if (IsDisable())
		return nRet;*/
	*(BYTE*)(void*)(unsigned int*)(this) = (int)this;

	//ofstream myfile;
	//myfile.open("E:\\OnClickButton.txt");
	//myfile << "uMsg " << uMsg << "\n"; // skill id
	//myfile << "KWndWindow " << uParam << "\n"; // skill id
	//myfile.close();

	switch (uMsg)
	{
	default:
		nRet = KWndWindow_WndProc(this, uMsg, uParam, nParam);
	}
	return nRet;
}

void __fastcall KUiMissionTime::UpdateData()
{
	if (*(DWORD*)((unsigned int)m_pSelf + 4) == WND_S_VISIBLE)
	{
		char	textMissionTime[80];
		memset(textMissionTime, 0, 80);
		//sprintf(textMissionTime, "Ho¹t ®éng %s: %d phót %d gi©y", "Tèng Kim", 5, 60);
		sprintf(textMissionTime, "%s: %s", m_ListMission[0].m_TextMission, m_ListMission[0].m_TextTime);
		_SetText(&m_pSelf->m_Text80Time[0], textMissionTime, -1);
		b_ShowWndWindow(&m_pSelf->m_Image[0]);
		b_ShowWndWindow(&m_pSelf->m_Text80Time[0]);
		/*int nWidth, nHeight;
		KWndWindow_GetSize(&m_pSelf->m_Image[0], &nWidth, &nHeight);
		if (nWidth > 0 && nHeight > 0)
		{
			char	textMissionTime[80];
			memset(textMissionTime, 0, 80);
			sprintf(textMissionTime, "Ho¹t ®éng %s: %d phót %d gi©y", "Tèng Kim", 5, 60);
			_SetText(&m_pSelf->m_Text80Time[0], textMissionTime, -1);
			b_ShowWndWindow(&m_pSelf->m_Image[0]);
			b_ShowWndWindow(&m_pSelf->m_Text80Time[0]);
		}*/
		if (m_pSelf->m_IsShow == 1)
		{

		}
	}
}
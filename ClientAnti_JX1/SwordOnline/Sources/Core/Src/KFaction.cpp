//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KFaction.cpp
// Date:	2002.09.26
// Code:	边城浪子
// Desc:	Faction Class
//---------------------------------------------------------------------------

#include	"KCore.h"
//#include	"MyAssert.h"
#include	"KIniFile.h"
#include	"KSkills.h"
#include	"KFaction.h"
#include	"CoreUseNameDef.h"


KFaction	g_Faction;


//---------------------------------------------------------------------------
//	功能：初始化，载入门派说明文件
//---------------------------------------------------------------------------
BOOL	KFaction::Init()
{
	KIniFile	Ini;
	char		szSection[80], szBuffer[32];
	char		szSeries[series_num][16] =
		{"S_GOLD", "S_WOOD", "S_WATER", "S_FIRE", "S_EARTH"};
	char		szCamp[camp_num][16] =
		{"C_BEGIN", "C_JUSTICE", "C_EVIL", "C_BALANCE",
		 "C_FREE", "C_ANIMAL", "C_EVENT"};
	int			i, j, k, nArrayPos;

	// 清空
	for (i = 0; i < MAX_FACTION; i++)
	{
		m_sAttribute[i].m_nIndex = i;
		m_sAttribute[i].m_nSeries = series_metal;
		m_sAttribute[i].m_nCamp = camp_justice;
		m_sAttribute[i].m_szName[0] = 0;
	}

	if ( !Ini.Load(FACTION_FILE) )
		return FALSE;

	// 读入每一个门派数据
	/* Doc THANG theo so hieu muc: muc [n] cua ini la mon phai id n. Giong het
	   ben may chu - xem ghi chu o KFaction.h. */
	for (i = 0; i < MAX_FACTION; i++)
	{
		sprintf(szSection, "%d", i);
		Ini.GetString(szSection, "Name", "", m_sAttribute[i].m_szName, sizeof(m_sAttribute[i].m_szName));
		if (m_sAttribute[i].m_szName[0] == 0)
			continue;

		Ini.GetString(szSection, "Series", "", szBuffer, sizeof(szBuffer));
		for (j = 0; j < series_num; j++)
			if (strcmp(szBuffer, szSeries[j]) == 0)
			{
				m_sAttribute[i].m_nSeries = j;
				break;
			}
		if (j >= series_num)
		{
			m_sAttribute[i].m_szName[0] = 0;
			continue;
		}

		Ini.GetString(szSection, "Camp", "", szBuffer, sizeof(szBuffer));
		for (j = 0; j < camp_num; j++)
			if (strcmp(szBuffer, szCamp[j]) == 0)
			{
				m_sAttribute[i].m_nCamp = j;
				break;
			}
	}

	return TRUE;
}

//---------------------------------------------------------------------------
//	功能：根据五行属性和本属性第几个门派得到门派编号
//---------------------------------------------------------------------------
int		KFaction::GetID(int nSeries, int nNo)
{
	if (nSeries < series_metal || nSeries >= series_num || nNo < 0 || nNo >= FACTIONS_PRR_SERIES)
		return -1;
	/* Mang phang roi nen phai DEM: tra ve phai thu nNo cua he do. */
	int nDem = 0;
	for (int i = 0; i < MAX_FACTION; i++)
	{
		if (m_sAttribute[i].m_szName[0] == 0 || m_sAttribute[i].m_nSeries != nSeries)
			continue;
		if (nDem == nNo)
			return i;
		nDem ++;
	}
	return -1;
}

//---------------------------------------------------------------------------
//	功能：根据五行属性和门派名得到门派编号
//---------------------------------------------------------------------------
int		KFaction::GetID(int nSeries, char *lpszName)
{
	if (nSeries < series_metal || nSeries >= series_num)
		return -1;
	if ( !lpszName || !lpszName[0])
		return -1;
	for (int i = 0; i < MAX_FACTION; i++)
	{
		if (m_sAttribute[i].m_nSeries != nSeries)
			continue;
		if (strcmp(lpszName, m_sAttribute[i].m_szName) == 0)
			return i;
	}
	return -1;
}

//---------------------------------------------------------------------------
//	功能：获得某个门派的阵营
//---------------------------------------------------------------------------
int		KFaction::GetCamp(int nFactionID)
{
	if (nFactionID < 0 || nFactionID >= MAX_FACTION)
		return -1;
	return m_sAttribute[nFactionID].m_nCamp;
}

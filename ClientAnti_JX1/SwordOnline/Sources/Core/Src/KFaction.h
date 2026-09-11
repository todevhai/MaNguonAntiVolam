//---------------------------------------------------------------------------
// Sword3 Engine (c) 2002 by Kingsoft
//
// File:	KFaction.h
// Date:	2002.09.26
// Code:	边城浪子
// Desc:	Faction Class
//---------------------------------------------------------------------------

#ifndef KFACTION_H
#define KFACTION_H

#include "GameDataDef.h"

/* Ban goc xep mon phai theo LUOI 5 he x 2 phai, id = he * 2 + thu tu. Luoi do
   khong chua duoc phai thu 11: settings/faction/mon-phai.ini da khai san Hoa
   Son [10] (he Thuy), Vu Hon [11], Tieu Dao [12] - ma he Thuy da du hai cho
   cho Nga Mi va Thuy Yen.

   Doi 11/09/2026: id lay THANG tu so hieu muc trong ini (phang), nen id 0..9
   giu NGUYEN. PHAI GIONG HET ben may chu (Core/KFaction.h) - hai nua ma danh
   so khac nhau thi cung mot id se ra hai mon phai khac nhau. */
#define		FACTIONS_PRR_SERIES				4			// toi da moi he
#define		MAX_FACTION						16			// tong so mon phai (phang)

class KFaction
{
public:
	struct	SFactionAttirbute							// 每个门派的属性
	{
		int		m_nIndex;								// 编号
		int		m_nSeries;								// 五行属性
		int		m_nCamp;								// 阵营
		char	m_szName[64];							// 门派名
	}				m_sAttribute[MAX_FACTION];			// 所有门派

public:
	BOOL			Init();								// 初始化，载入门派说明文件
	int				GetID(int nSeries, int nNo);		// 根据五行属性和本属性第几个门派得到门派编号
	int				GetID(int nSeries, char *lpszName);	// 根据五行属性和门派名得到门派编号
	int				GetCamp(int nFactionID);			// 获得某个门派的阵营
};

extern	KFaction	g_Faction;
#endif

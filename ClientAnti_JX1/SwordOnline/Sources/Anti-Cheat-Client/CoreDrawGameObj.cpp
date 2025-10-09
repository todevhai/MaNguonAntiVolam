//#include "KCore.h"
//#include "KNpc.h"
//#include "KMissle.h"
//#include "KItem.h"
//#include "KBuySell.h"
//#include "KPlayer.h"
#include "iRepresent/iRepresentshell.h"
//#include "KSubWorldSet.h"
#include "scene/KScenePlaceC.h"
#include "ImgRef.h"
#include "GameDataDef.h"
#include "KObjSet.h"

#define  PHYSICSSKILLICON "\\spr\\Ui\\¼¼ÄÜÍ¼±ê\\icon_sk_ty_ap.spr"

#define SHOW_SPACE_HEIGHT 5

void	CoreDrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam)
{
//	static int nSpeed = 0;
	switch(uObjGenre)
	{
	case CGOG_NPC:
	case CGOG_MISSLE:
	case CGOG_PLAYERSELLITEM:
	case CGOG_IME_ITEM:
	case CGOG_ITEM:
	case CGOG_MENU_NPC:
	case CGOG_NPC_BLUR_DETAIL(1):
	case CGOG_NPC_BLUR_DETAIL(2):
	case CGOG_NPC_BLUR_DETAIL(3):
	case CGOG_NPC_BLUR_DETAIL(4):
	case CGOG_NPC_BLUR_DETAIL(5):
	case CGOG_NPC_BLUR_DETAIL(6):
	case CGOG_NPC_BLUR_DETAIL(7):
	case CGOG_SKILL:
	case CGOG_SKILL_FIGHT:
	case CGOG_SKILL_LIVE:
	case CGOG_SKILL_SHORTCUT:
	case CGOG_OBJECT:
		if (uId)
		{
			if ((nParam & IPOT_RL_INFRONTOF_ALL) == IPOT_RL_INFRONTOF_ALL)
			{
				//if (ObjSet.CheckShowName())
					Object[uId].DrawInfo();
			}
			else //if ((nParam & IPOT_RL_COVER_GROUND) == IPOT_RL_COVER_GROUND)
			{
				Object[uId].Draw();
				if ((int)uId == Player[CLIENT_PLAYER_INDEX].GetTargetObj())
					Object[uId].DrawBorder();
			}
		}
		break;
	case CGOG_NPCSELLITEM:
	default:
		break;
	}
}

void	CoreGetGameObjLightInfo(unsigned int uObjGenre, unsigned int uId, KLightInfo *pLightInfo)
{
	switch (uObjGenre)
	{
	case CGOG_NPC:
		if (uId > 0 && uId < MAX_NPC)
		{
			Npc[uId].GetMpsPos(&pLightInfo->oPosition.nX, &pLightInfo->oPosition.nY);
			pLightInfo->oPosition.nZ = Npc[uId].m_Height;
			pLightInfo->dwColor = 0;
			if (Npc[uId].m_RedLum > 255)
			{
				Npc[uId].m_RedLum = 255;
			}
			if (Npc[uId].m_GreenLum > 255)
			{
				Npc[uId].m_GreenLum = 255;
			}
			if (Npc[uId].m_BlueLum > 255)
			{
				Npc[uId].m_BlueLum = 255;
			}
			pLightInfo->dwColor = 0xff000000 | Npc[uId].m_RedLum << 16 | Npc[uId].m_GreenLum << 8 | Npc[uId].m_BlueLum;
			pLightInfo->nRadius = Npc[uId].m_CurrentVisionRadius;
		}
		break;
	case CGOG_OBJECT:
		break;
	case CGOG_MISSLE:
		if (uId > 0 && uId < MAX_MISSLE)
		{
			if (Missle[uId].m_nMissleId > 0)
			{
				Missle[uId].GetLightInfo(pLightInfo);
			}
		}
		break;
	default:
		break;
	}
}
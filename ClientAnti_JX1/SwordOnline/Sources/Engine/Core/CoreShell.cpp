/*****************************************************************************************
//	Íâ½ç·ÃÎÊCore½Ó¿Ú·½·¨
//	Copyright : Kingsoft 2002
//	Author	:   
//	CreateTime:	2002-9-12
------------------------------------------------------------------------------------------
*****************************************************************************************/
#include "KCore.h"
#include "GameDataDef.h"
#include "CoreShell.h"

#include "CoreDrawGameObj.h"
#include "ImgRef.h"

#include "KPlayer.h"
#include "KPlayerSet.h"
#include "KPlayerTask.h"
#include "KTaskFuns.h"
#include "KItem.h"
#include "KItemList.h"
#include "KSubWorldSet.h"
#include "KProtocolProcess.h"

#include "KNpcResList.h"
#include "Scene/KScenePlaceC.h"
#include "kskills.h"
#include "GameDataDef.h"
#include "MsgGenreDef.h"
#include "KOption.h"
#include "KSubWorld.h"
#include "KViewItem.h"
#include "KTongProtocol.h"
#include "malloc.h"

#include "KBuySell.h"
#include "KThiefSkill.h"

#include "KSellItem.h"

#define	NPC_TRADE_BOX_WIDTH		6
#define	NPC_TRADE_BOX_HEIGHT	10
#define	MAX_TRADE_ITEM_WIDTH	2
#define	MAX_TRADE_ITEM_HEIGHT	4
IClientCallback* l_pDataChangedNotifyFunc = 0;

class KCoreShell : public iCoreShell
{
public:
	int	 GetProtocolSize(BYTE byProtocol);
	int	 Debug(unsigned int uDataId, unsigned int uParam, int nParam);
	int	 OperationRequest(unsigned int uOper, unsigned int uParam, int nParam);
	void ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam);
	int	 FindSelectNPC(int x, int y, int nRelation, bool bSelect, void* pReturn, int& nKind);
	int FindSelectObject(int x, int y, bool bSelect, int& nObjectIdx, int& nKind);
	int FindSpecialNPC(char* Name, void* pReturn, int& nKind);
	int ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, unsigned short nMsgLength);
	void ApplyAddTeam(void* pPlayer);
	void TradeApplyStart(void* pPlayer);
	int UseSkill(int x, int y, int nSkillID);
	int LockSomeoneUseSkill(int nTargetIndex, int nSkillID);
	int LockSomeoneAction(int nTargetIndex);
	int LockObjectAction(int nTargetIndex);
	void GotoWhere(int x, int y, int mode);	//mode 0 is auto, 1 is walk, 2 is run
	void Goto(int nDir, int mode);	//nDir 0~63, mode 0 is auto, 1 is walk, 2 is run
	void Turn(int nDir);	//nDir 0 is left, 1 is right, 2 is back
	int ThrowAwayItem();
	int GetNPCRelation(int nIndex);
	int GetNPCBAITAN(int nIndex);

	int GetGenreItem(unsigned int uId);
	int GetTypeItem(unsigned int uId);
	
	//ÓëµØÍ¼Ïà¹ØµÄ²Ù×÷,uOperµÄÈ¡ÖµÀ´×Ô GAME_SCENE_MAP_OPERATION_INDEX
	int	SceneMapOperation(unsigned int uOper, unsigned int uParam, int nParam);
	//Óë°ï»áÏà¹ØµÄ²Ù×÷, uOperµÄÈ¡ÖµÀ´×Ô GAME_TONG_OPERATION_INDEX
	int	TongOperation(unsigned int uOper, unsigned int uParam, int nParam);
	//Óë×é¶ÓÏà¹ØµÄ²Ù×÷£¬uOperµÄÈ¡ÖµÀ´×Ô GAME_TEAM_OPERATION_INDEX
	int TeamOperation(unsigned int uOper, unsigned int uParam, int nParam);

	int	 GetGameData(unsigned int uDataId, unsigned int uParam, int nParam);

	void DrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam);
	void DrawGameSpace();
	DWORD GetPing();
	//void SendPing();
	int	 SetCallDataChangedNofify(IClientCallback* pNotifyFunc);
	void NetMsgCallbackFunc(void* pMsgData);
	void SetRepresentShell(struct iRepresentShell* pRepresent);
	void SetMusicInterface(void* pMusicInterface);
	void SetRepresentAreaSize(int nWidth, int nHeight);
	int  Breathe();
	void Release();	//ÊÍ·Å½Ó¿Ú¶ÔÏó
	void SetClient(LPVOID pClient);
	void SendNewDataToServer(void* pData, int nLength);
	int GetTradeItem(unsigned int uId);
	int GetDropItem(unsigned int uId);
};

static KCoreShell	g_CoreShell;

CORE_API void g_InitCore();
#ifndef _STANDALONE
extern "C" __declspec(dllexport)
#endif
iCoreShell* CoreGetShell()
{
	g_InitCore();
	return &g_CoreShell;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º·¢³öÓÎÏ·ÊÀ½çÊý¾Ý¸Ä±äµÄÍ¨Öªº¯Êý
//	·µ»Ø£ºÈçÎ´±»×¢²áÍ¨Öªº¯Êý£¬ÔòÖ±½Ó·µ»Ø0£¬·ñÔò·µ»ØÍ¨Öªº¯ÊýÖ´ÐÐ½á¹û¡£
//--------------------------------------------------------------------------
void CoreDataChanged(unsigned int uDataId, unsigned int uParam, int nParam)
{
	if (l_pDataChangedNotifyFunc)
		l_pDataChangedNotifyFunc->CoreDataChanged(uDataId, uParam, nParam);
}


void KCoreShell::Release()
{
	g_ReleaseCore();
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º½ÓÊÜÓë·ÖÅÉ´¦ÀíÍøÂçÏûÏ¢
//--------------------------------------------------------------------------
void KCoreShell::NetMsgCallbackFunc(void* pMsgData)
{
	g_ProtocolProcess.ProcessNetMsg((BYTE *)pMsgData);
}
//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉèÖÃÓÎÏ·ÊÀ½çÊý¾Ý¸Ä±äµÄÍ¨Öªº¯Êý
//	²ÎÊý£ºfnCoreDataChangedCallback pNotifyFunc --> Í¨Öªº¯ÊýµÄÖ¸Õë¡£
//	·µ»Ø£º·µ»ØÖµÎª·Ç0Öµ±íÊ¾×¢²á³É¹¦£¬·ñÔò±íÊ¾Ê§°Ü¡£
//--------------------------------------------------------------------------
int	KCoreShell::SetCallDataChangedNofify(IClientCallback* pNotifyFunc)
{
	l_pDataChangedNotifyFunc = pNotifyFunc;
	return true;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º´ÓÓÎÏ·ÊÀ½ç»ñÈ¡Êý¾Ý
//	²ÎÊý£ºunsigned int uDataId --> ±íÊ¾»ñÈ¡ÓÎÏ·Êý¾ÝµÄÊý¾ÝÏîÄÚÈÝË÷Òý£¬ÆäÖµÎªÃ·¾ÙÀàÐÍ
//							GAMEDATA_INDEXµÄÈ¡ÖµÖ®Ò»¡£
//		  unsigned int uParam  --> ÒÀ¾ÝuDataIdµÄÈ¡ÖµÇé¿ö¶ø¶¨
//		  int nParam --> ÒÀ¾ÝuDataIdµÄÈ¡ÖµÇé¿ö¶ø¶¨
//	·µ»Ø£ºÒÀ¾ÝuDataIdµÄÈ¡ÖµÇé¿ö¶ø¶¨¡£
//--------------------------------------------------------------------------
int	KCoreShell::GetGameData(unsigned int uDataId, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uDataId)
	{
	case GDI_PLAYER_IS_MALE:
		{
			int nIndex = 0;
			if (nParam == 0)
				nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
			else
				nIndex = NpcSet.SearchID(nParam);

			if (nIndex)
				nRet = (Npc[nIndex].m_NpcSettingIdx == PLAYER_MALE_NPCTEMPLATEID);
			else
				nRet = 1;	//³ö´íÊ±ÊÇÄÐÐÔ
		}
		break;
	case GDI_REPAIR_ITEM_PRICE:
		if (uParam)
		{
			KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
			KItem*	pItem = NULL;

			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->Obj.uId > 0)
					{
						pItem = &Item[pObj->Obj.uId];
					}
				}
				break;
			default:
				break;
			}

			if (!pItem)
				break;

			KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
		
			if (pObj->eContainer == UOC_NPC_SHOP)
				break;
			pInfo->nPrice = pItem->GetRepairPrice();
			strcpy(pInfo->szItemName, pItem->GetName());
			nRet = pItem->CanBeRepaired();
		}
		else
		{
			nParam = 0;
			nRet = 0;
		}
		break;
	case GDI_TRADE_ITEM_PRICE:
		if (uParam)
		{
			KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
			KItem*	pItem = NULL;

			switch(pObj->Obj.uGenre)
			{
			case CGOG_PLAYERSELLITEM:
				{
					if (pObj->Obj.uId > 0)
					{
						KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
						pInfo->nPrice = Item[pObj->Obj.uId].GetSetPrice();
						strcpy(pInfo->szItemName,Item[pObj->Obj.uId].GetName());
						return 1;
					}
				}
				break;
			case CGOG_ITEM:
				{
					if (pObj->Obj.uId > 0)
					{
						pItem = &Item[pObj->Obj.uId];
					}
				}
				break;
			case CGOG_NPCSELLITEM:
				{
					int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
					if (nBuyIdx != -1)
					{
						int nIndex = BuySell.GetItemIndex(nBuyIdx, pObj->Obj.uId);
						if (nIndex >= 0)
							pItem = BuySell.GetItem(nIndex);
					}
				}
				break;
			default:
				break;
			}

			if (!pItem)
				break;

			KUiItemBuySelInfo *pInfo = (KUiItemBuySelInfo *)nParam;
		
			if (pObj->eContainer == UOC_NPC_SHOP)
			{
				if (pItem->GetGenre() == item_task)
				{
					pInfo->nPriceXu = pItem->GetPriceXu();
				}
				else
				{
					pInfo->nPrice = pItem->GetPrice();
				}
			}
			else if (pObj->eContainer == UOC_MARKET)
			{
				pInfo->nPriceXu = pItem->GetPriceXu();
			}
			else
			{
				pInfo->nPrice = pItem->GetPrice() / BUY_SELL_SCALE;
			}
			strcpy(pInfo->szItemName, pItem->GetName());
			nRet = (pItem->GetGenre() != item_task);
		}
		else
		{
			nParam = 0;
			nRet = 0;
		}
		break;
	//ÓÎÏ·¶ÔÏóÃèÊöËµÃ÷ÎÄ±¾´®
	//uParam = (KUiGameObject*) ÃèÊöÓÎÏ·¶ÔÏóµÄ½á¹¹Êý¾ÝµÄÖ¸Õë
	//nParam = (char*) Ö¸ÏòÒ»¸ö»º³åÇøµÄÖ¸Õë£¬Æä¿Õ¼ä²»ÉÙÓÚ256×Ö½Ú¡£
	case GDI_GAME_OBJ_DESC_INCLUDE_REPAIRINFO:
	case GDI_GAME_OBJ_DESC_INCLUDE_TRADEINFO:
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion *)uParam;
			char* pszDescript = (char *)nParam;
			pszDescript[0] = 0;
			switch(pObj->Obj.uGenre)
			{
			case CGOG_PLAYERSELLITEM:
				{
					Item[pObj->Obj.uId].GetDesc(pszDescript);
					// 					char sPrice[20];
					// 					sprintf(sPrice,"¼Û¸ñ£º%dÍò%dÁ½",Item[pObj->Obj.uId].GetSetPrice()/10000,Item[pObj->Obj.uId].GetSetPrice()%10000);
					// 					strcat(pszDescript,"<color=Yellow>");
					// 					strcat(pszDescript,sPrice);
					// 					strcat(pszDescript,"\n");
				}
				break;
			case CGOG_ITEM:
				{
					if (pObj->eContainer == UOC_EQUIPTMENT)
					{
						int nActive = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(pObj->Obj.uId);
						Item[pObj->Obj.uId].GetDesc(pszDescript, true, BUY_SELL_SCALE, nActive);
					}
					else
						Item[pObj->Obj.uId].GetDesc(pszDescript, true, BUY_SELL_SCALE);
				}
				break;			
			case CGOG_NPCSELLITEM:
				{
					int nIdx = -1;
					if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx)
						break;
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->Obj.uId);

					KItem* pItem = NULL;
					if (nIdx < 0)
						break;
					pItem = BuySell.GetItem(nIdx);

					if (!pItem)
						break;
					pItem->GetDesc(pszDescript, true);
				}
				break;
			}
		}
		break;
	case GDI_GAME_OBJ_DESC:
		if (nParam && uParam)
		{
			KUiObjAtContRegion* pObj = (KUiObjAtContRegion *)uParam;
			char* pszDescript = (char *)nParam;
			pszDescript[0] = 0;
			switch(pObj->Obj.uGenre)
			{
			case CGOG_ITEM:
				{
					if (pObj->eContainer == UOC_EQUIPTMENT)
					{
						int nActive = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetActiveAttribNum(pObj->Obj.uId);

						if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetIfActive())
						{
							nActive = 3;
						}

						Item[pObj->Obj.uId].GetDesc(pszDescript, false, 1, nActive);
					}
					else
						Item[pObj->Obj.uId].GetDesc(pszDescript);
				}
				break;
			case CGOG_SKILL:
			case CGOG_SKILL_FIGHT:
			case CGOG_SKILL_LIVE:
			case CGOG_SKILL_SHORTCUT:
				{
					int nLevel = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLevel(pObj->Obj.uId);
					_ASSERT(nLevel >= 0);
					if (pObj->Obj.uId >0)
					{
						
						ISkill * pISkill = g_SkillManager.GetSkill(pObj->Obj.uId, 1);
						if (!pISkill)
							break;
						eSkillStyle eStyle = (eSkillStyle) pISkill->GetSkillStyle();
						
						switch(eStyle)
						{
						case SKILL_SS_Missles:			//	×Óµ¯Àà		±¾¼¼ÄÜÓÃÓÚ·¢ËÍ×Óµ¯Àà
						case SKILL_SS_Melee:
						case SKILL_SS_InitiativeNpcState:	//	Ö÷¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äµ±Ç°NpcµÄÖ÷¶¯×´Ì¬
						case SKILL_SS_PassivityNpcState:		//	±»¶¯Àà		±¾¼¼ÄÜÓÃÓÚ¸Ä±äNpcµÄ±»¶¯×´Ì¬
							{
								int nList = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.FindSame(pObj->Obj.uId);
								KSkill::GetDesc(
									pObj->Obj.uId,
									nLevel,
									pszDescript,
									Player[CLIENT_PLAYER_INDEX].m_nIndex,
									(pObj->Obj.uGenre == CGOG_SKILL_SHORTCUT)?false:true,
									Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetAddPoint(nList),
									Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetEnChance(nList)
									);
							}
							break;
							
						case SKILL_SS_Thief:
							{
								((KThiefSkill *)pISkill)->GetDesc(
									pObj->Obj.uId,
									nLevel,
									pszDescript,
									Player[CLIENT_PLAYER_INDEX].m_nIndex,
									(pObj->Obj.uGenre == CGOG_SKILL_SHORTCUT)?false:true);
			
							}break;
							
						}
					}
					
				}
				break;
			
			case CGOG_PLAYER_FACE:
				break;
			case CGOG_NPCSELLITEM:
				{
					int nIdx = -1;
					if (-1 == Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx)
						break;
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->Obj.uId);

					KItem* pItem = NULL;
					if (nIdx < 0)
						break;
					pItem = BuySell.GetItem(nIdx);

					if (!pItem)
						break;
					pItem->GetDesc(pszDescript);
				}
				break;
			case CGOG_PLAYERSELLITEM:
				{
					Item[pObj->Obj.uId].GetDesc(pszDescript);
					// 					char sPrice[20];
					// 					sprintf(sPrice,"¼Û¸ñ£º%dÍò%dÁ½",Item[pObj->Obj.uId].GetSetPrice()/10000,Item[pObj->Obj.uId].GetSetPrice()%10000);
					// 					strcat(pszDescript,"<color=Yellow>");
					// 					strcat(pszDescript,sPrice);
					// 					strcat(pszDescript,"\n");
				}
				break;
			}
		}
		break;
	//Ö÷½ÇµÄÒ»Ð©²»Ò×±äµÄÊý¾Ý
	//uParam = (KUiPlayerBaseInfo*)pInfo	
	case GDI_PLAYER_BASE_INFO:
		if (uParam)
		{
			KUiPlayerBaseInfo* pInfo = (KUiPlayerBaseInfo*)uParam;
			int nIndex = 0;
			if (nParam == 0)
			{
				nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;
				pInfo->nCurFaction = Player[CLIENT_PLAYER_INDEX].m_cFaction.m_nCurFaction;
				pInfo->nCurTong = Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongNameID();
			}
			else
			{
				nIndex = NpcSet.SearchID(nParam);
				pInfo->nCurFaction = -1;
				pInfo->nCurTong = 0;
			}
			if (nIndex)
			{
				strcpy(pInfo->Name, Npc[nIndex].Name);
				//to do:no implements in this version
				pInfo->Agname[0] = 0;
				pInfo->Title[0] = 0;
				if (Npc[nIndex].m_btRankId)
				{
					char szRankId[5];
					itoa(Npc[nIndex].m_btRankId, szRankId, 10);
					g_RankTabSetting.GetString(szRankId, "RANKSTR", "", pInfo->Title, 32);
				}
				pInfo->nRankInWorld = Player[CLIENT_PLAYER_INDEX].m_nWorldStat;
				pInfo->nRankInWorld = Npc[nIndex].nRankInWorld;		//tnxh
				pInfo->nRepute = Npc[nIndex].nRepute; // danh vong
				pInfo->nPKValue = Npc[nIndex].nPKValue; // PK
				pInfo->nReBorn = Npc[nIndex].nReBorn; // trung sinh
			}
		}
		break;

	//Ö÷½ÇµÄÒ»Ð©Ò×±äµÄÊý¾Ý
	//uParam = (KUiPlayerRuntimeInfo*)pInfo
	case GDI_PLAYER_RT_INFO:
		if (uParam)
		{
			KUiPlayerRuntimeInfo* pInfo = (KUiPlayerRuntimeInfo*)uParam;
			pInfo->nLifeFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLifeMax;		//ÉúÃüÂúÖµ
			pInfo->nLife = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentLife;				//ÉúÃü
			pInfo->nManaFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentManaMax;		//ÄÚÁ¦ÂúÖµ
			pInfo->nMana = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentMana;				//ÄÚÁ¦
			pInfo->nStaminaFull = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStaminaMax;//ÌåÁ¦ÂúÖµ
			pInfo->nStamina = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentStamina;		//ÌåÁ¦

			pInfo->nAngryFull = 0;		//Å­ÂúÖµ
			pInfo->nAngry = 0;			//Å­
			
			pInfo->nExperienceFull = Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp;		//¾­ÑéÂúÖµ
			pInfo->nExperience = Player[CLIENT_PLAYER_INDEX].m_nExp;					//¾­Ñé
			pInfo->nCurLevelExperience = Player[CLIENT_PLAYER_INDEX].m_nNextLevelExp;
			
			pInfo->byActionDisable = 0;
			//to do	¸øpInfo->bActionDisable¡¢¸³ÓèºÏÊÊµÄÖµ

			pInfo->byAction = PA_NONE;

			if (Player[CLIENT_PLAYER_INDEX].m_RunStatus)
				pInfo->byAction |= PA_RUN;

			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_sit)
				pInfo->byAction |= PA_SIT;
			pInfo->wReserved = 0;
		}
		break;

	//Ö÷½ÇµÄÒ»Ð©Ò×±äµÄÊôÐÔÊý¾Ý
	//uParam = (KUiPlayerAttribute*)pInfo
	case GDI_PLAYER_RT_ATTRIBUTE:
		if (uParam)
		{
			KUiPlayerAttribute* pInfo = (KUiPlayerAttribute*)uParam;
			KNpc*	pNpc = &Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex];
			pInfo->nMoney = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);				//ÒøÁ½
			pInfo->nXu = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetXu(room_equipment);
			pInfo->nBARemainPoint = Player[CLIENT_PLAYER_INDEX].m_nAttributePoint;						//»ù±¾ÊôÐÔÊ£ÓàµãÊý
			pInfo->nStrength = Player[CLIENT_PLAYER_INDEX].m_nCurStrength;								//Á¦Á¿
			pInfo->nDexterity = Player[CLIENT_PLAYER_INDEX].m_nCurDexterity;								//Ãô½Ý
			pInfo->nVitality = Player[CLIENT_PLAYER_INDEX].m_nCurVitality;								//»îÁ¦
			pInfo->nEnergy = Player[CLIENT_PLAYER_INDEX].m_nCurEngergy;	//¾«Á¦
			pInfo->nPKValue = Player[CLIENT_PLAYER_INDEX].m_cPK.GetPKValue();	//tnpk
			pInfo->nRepute = Player[CLIENT_PLAYER_INDEX].m_cRepute.GetReputeValue(); // danh vong
			pInfo->nFuYuan = Player[CLIENT_PLAYER_INDEX].m_cFuYuan.GetFuYuanValue(); // phuc duyen
			pInfo->nReBorn = Player[CLIENT_PLAYER_INDEX].m_cReBorn.GetReBornValue(); // trung sinh
			pInfo->nRankInWorld = Player[CLIENT_PLAYER_INDEX].m_nWorldStat;

			Player[CLIENT_PLAYER_INDEX].GetEchoDamage(&pInfo->nKillMIN, &pInfo->nKillMAX, 0);				//×î´ó×îÐ¡É±ÉËÁ¦
			Player[CLIENT_PLAYER_INDEX].GetEchoDamage(&pInfo->nRightKillMin , &pInfo->nRightKillMax, 1);
			pInfo->nAttack = pNpc->m_CurrentAttackRating;				//¹¥»÷Á¦
			pInfo->nDefence = pNpc->m_CurrentDefend;					//·ÀÓùÁ¦
			pInfo->nMoveSpeed = pNpc->m_CurrentRunSpeed;				//ÒÆ¶¯ËÙ¶È
			pInfo->nAttackSpeed = pNpc->m_CurrentAttackSpeed;			//¹¥»÷ËÙ¶È
			pInfo->nCastSpeed = pNpc->m_CurrentCastSpeed;
			//ÎïÀí·ÀÓù
			if (pNpc->m_CurrentPhysicsResistMax >= pNpc->m_CurrentPhysicsResist)
				pInfo->nPhyDef = pNpc->m_CurrentPhysicsResist;
			else
				pInfo->nPhyDef = pNpc->m_CurrentPhysicsResistMax;
			//±ù¶³·ÀÓù
			if (pNpc->m_CurrentColdResistMax >= pNpc->m_CurrentColdResist)
				pInfo->nCoolDef = pNpc->m_CurrentColdResist;
			else
				pInfo->nCoolDef = pNpc->m_CurrentColdResistMax;
			//ÉÁµç·ÀÓù
			if (pNpc->m_CurrentLightResistMax >= pNpc->m_CurrentLightResist)
				pInfo->nLightDef = pNpc->m_CurrentLightResist;
			else
				pInfo->nLightDef = pNpc->m_CurrentLightResistMax;
			//»ðÑæ·ÀÓù
			if (pNpc->m_CurrentFireResistMax >= pNpc->m_CurrentFireResist)
				pInfo->nFireDef = pNpc->m_CurrentFireResist;
			else
				pInfo->nFireDef = pNpc->m_CurrentFireResistMax;
			//¶¾ËØ·ÀÓù
			if (pNpc->m_CurrentPoisonResistMax >= pNpc->m_CurrentPoisonResist)
				pInfo->nPoisonDef = pNpc->m_CurrentPoisonResist;
			else
				pInfo->nPoisonDef = pNpc->m_CurrentPoisonResistMax;
			
			pInfo->nLevel = pNpc->m_Level;

			// ¸ù¾ÝÍæ¼Ò×´Ì¬ÏÔÊ¾ ×´Ì¬ÎÄ×Ö not end ²»Ó¦¸ÃÊÇ×´Ì¬£¬Ó¦¸ÃÊÇÎåÐÐÊôÐÔ spe
			memset(pInfo->StatusDesc, 0, sizeof(pInfo->StatusDesc));
			switch(pNpc->m_Series)
			{
			case series_water:
				strcpy(pInfo->StatusDesc, "HÖ Thñy");
				break;
			case series_wood:
				strcpy(pInfo->StatusDesc, "HÖ Méc");
				break;
			case series_metal:
				strcpy(pInfo->StatusDesc, "HÖ Kim");
				break;
			case series_fire:
				strcpy(pInfo->StatusDesc, "HÖ Háa");
				break;
			case series_earth:
				strcpy(pInfo->StatusDesc, "HÖ Thæ");
				break;
			}
		}
		break;

	//Ö÷½ÇµÄÁ¢¼´Ê¹ÓÃÎïÆ·ÓëÎä¹¦
	//uParam = (KUiPlayerImmedItemSkill*)pInfo
	case GDI_PLAYER_IMMED_ITEMSKILL:
		if (uParam)
		{
			KUiPlayerImmedItemSkill* pInfo = (KUiPlayerImmedItemSkill*)uParam;
			memset(pInfo,0,sizeof(KUiPlayerImmedItemSkill));
			pInfo->IMmediaSkill[0].uGenre	= CGOG_SKILL_SHORTCUT;
			pInfo->IMmediaSkill[0].uId		= Player[CLIENT_PLAYER_INDEX].GetLeftSkill();
			pInfo->IMmediaSkill[1].uGenre	= CGOG_SKILL_SHORTCUT;
			pInfo->IMmediaSkill[1].uId		= Player[CLIENT_PLAYER_INDEX].GetRightSkill();

			for (int i = 0; i < MAX_IMMEDIACY_ITEM; i++)
			{
				pInfo->ImmediaItem[i].uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.m_Room[room_immediacy].FindItem(i, 0);
				if (pInfo->ImmediaItem[i].uId > 0)
				{
					pInfo->ImmediaItem[i].uGenre = CGOG_ITEM;
				}
				else
				{
					pInfo->ImmediaItem[i].uGenre = CGOG_NOTHING;
				}
			}
		}
		break;
	case GDI_TRADE_PLAYER_ITEM_COUNT:
		nRet = 0;
		SendClientCmdGetCount(uParam);
		break;

	case GDI_TRADE_PLAYER_ITEM:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion*)uParam;
			
			for (int i = 0; i < 60; i++)
			{
				if (g_cSellItem.m_sItem[i].nIdx)
				{
					pInfo->Obj.uGenre = CGOG_PLAYERSELLITEM;
					pInfo->Obj.uId = g_cSellItem.m_sItem[i].nIdx;
					
					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = g_cSellItem.m_sItem[i].nX;
					pInfo->Region.v = g_cSellItem.m_sItem[i].nY;
					pInfo->nContainer = g_cSellItem.m_sItem[i].nPrice;
					nCount++;
					pInfo++;
				}		
			}
			
			nRet = nCount;
		} 
		else
		{
			nRet = g_cSellItem.GetCount();
		}
		break;

	case GDI_PLAYER_IS_BAITAN:
		nRet = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_BaiTan;
		break;

	//Ö÷½ÇËæÉíÐ¯´øµÄÇ®
	//nRet = Ö÷½ÇËæÉíÐ¯´øµÄÇ®
	case GDI_PLAYER_HOLD_MONEY:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment);
		break;
	case GDI_PLAYER_HOLD_XU:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetXu(room_equipment);
		break;
	//Ö÷½ÇËæÉíÐ¯´øµÄÎïÆ·µÈ
	//uParam = (KUiObjAtRegion*) pInfo -> KUiObjAtRegion½á¹¹Êý×éµÄÖ¸Õë£¬KUiObjAtRegion
	//				½á¹¹ÓÃÓÚ´æ´¢ÎïÆ·»òÕßÎä¹¦µÄÊý¾Ý¼°Æä·ÅÖÃÇøÓòÎ»ÖÃÐÅÏ¢¡£
	//nParam = pInfoÊý×éÖÐ°üº¬KUiObjAtRegion½á¹¹µÄÊýÄ¿
	//Return = Èç¹û·µ»ØÖµÐ¡ÓÚµÈÓÚ´«Èë²ÎÊýnParam£¬ÆäÖµ±íÊ¾pInfoÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiObjAtRegion
	//			½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý£»·ñÔò±íÊ¾ÐèÒª´«Èë°üº¬¶àÉÙ¸öKUiObjAtRegion½á¹¹µÄÊý×é
	//			²Å¹»´æ´¢È«²¿µÄËæÉíÐ¯´øµÄÎïÆ·ÐÅÏ¢¡£

	// flyingÌí¼ÓÁËÒ»¸öÊÇ·ñ¿ÉÒÔÆïÂíµÄÇëÇó£¬ÔÚÕâÀïÊµÏÖ£¬gg¿ÉÀÖÒ»¹Þ£¬mmÇë³ÔKFCÅ¶
	case GDI_PLAYER_CAN_RIDE:
		// ÊµÏÖ·ÖÖ§µÄ´úÂëÔÚÕâÀïÌí¼Ó
		break;

	case GDI_ITEM_TAKEN_WITH:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproom)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproom)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproom)
					nCount++;
			}
			nRet = nCount;
		}
		break;

	//Ö÷½Ç×°±¸ÎïÆ·
	//uParam = (KUiObjAtRegion*)pInfo -> °üº¬10¸öÔªËØµÄKUiObjAtRegion½á¹¹Êý×éÖ¸Õë£¬
	//			KUiObjAtRegion½á¹¹ÓÃÓÚ´æ´¢×°±¸µÄÊý¾ÝºÍ·ÅÖÃÎ»ÖÃÐÅÏ¢¡£
	//			KUiObjAtRegion::Region::h ±íÊ¾ÊôÓÚµÚ¼¸Ì××°±¸
	//			KUiObjAtRegion::Region::v ±íÊ¾ÊôÓÚÄÄ¸öÎ»ÖÃµÄ×°±¸,ÆäÖµÎªÃ·¾ÙÀàÐÍ
	//			UI_EQUIPMENT_POSITIONµÄÈ¡ÖµÖ®Ò»¡£Çë²Î¿´UI_EQUIPMENT_POSITIONµÄ×¢ÊÍ¡£
	//nParam =	Òª»ñÈ¡µÄÊÇµÚ¼¸Ì××°±¸ÐÅÏ¢
	//Return =  ÆäÖµ±íÊ¾pInfoÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiObjAtRegion½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý¡£
	case GDI_EQUIPMENT:
		nRet = 0;
		if (uParam)
		{
			// TODO£ºÔÝÊ±Ã»ÓÐ×öµÚ¶þÌ××°±¸
			if (nParam == 1)
				break;

			int PartConvert[itempart_num] = 
			{
				UIEP_HEAD,		UIEP_BODY,
				UIEP_WAIST,		UIEP_HAND,
				UIEP_FOOT,		UIEP_FINESSE,
				UIEP_NECK,		UIEP_FINGER1,
				UIEP_FINGER2,	UIEP_WAIST_DECOR,
				UIEP_HORSE,		UIEP_MASK,	// mat na
			};

			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (int i = 0; i < itempart_num; i++)
			{
				pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipment(i);
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
				
					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartConvert[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;
	case GDI_PARADE_EQUIPMENT:
		nRet = 0;
		if (uParam)
		{
			// TODO£ºÔÝÊ±Ã»ÓÐ×öµÚ¶þÌ××°±¸
			if (nParam == 1)
				break;

			int PartConvert[itempart_num] = 
			{
				UIEP_HEAD,		UIEP_BODY,
				UIEP_WAIST,		UIEP_HAND,
				UIEP_FOOT,		UIEP_FINESSE,
				UIEP_NECK,		UIEP_FINGER1,
				UIEP_FINGER2,	UIEP_WAIST_DECOR,
				UIEP_HORSE,		UIEP_MASK,	// mat na
			};

			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;

			for (int i = 0; i < itempart_num; i++)
			{
				pInfo->Obj.uId = g_cViewItem.m_sItem[i].nIdx;
				if (pInfo->Obj.uId)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
				
					pInfo->Region.Width = Item[pInfo->Obj.uId].GetWidth();
					pInfo->Region.Height = Item[pInfo->Obj.uId].GetHeight();
					pInfo->Region.h = 0;
					pInfo->Region.v = PartConvert[i];
				}
				else
				{
					pInfo->Obj.uGenre = CGOG_NOTHING;
				}
				nCount++;
				pInfo++;
			}
			nRet = nCount;
		}
		break;
		
	//½»Ò×²Ù×÷Ïà¹ØµÄÊý¾Ý
	//uParam = (UI_TRADE_OPER_DATA)eOper ¾ßÌåº¬Òå¼ûUI_TRADE_OPER_DATA
	//nParam ¾ßÌåÓ¦ÓÃÓëº¬ÒåÓÉuParamµÄÈ¡Öµ×´¿ö¾ö¶¨,¼ûUI_TRADE_OPER_DATAµÄËµÃ÷
	//Return ¾ßÌåº¬ÒåÓÉuParamµÄÈ¡Öµ×´¿ö¾ö¶¨,¼ûUI_TRADE_OPER_DATAµÄËµÃ÷
	//Ö÷½ÇµÄÉú»î¼¼ÄÜÊý¾Ý
	case GDI_TRADE_OPER_DATA:
		if (uParam == UTOD_IS_LOCKED)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock;
		else if (uParam == UTOD_IS_TRADING)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeState;
		else if (uParam == UTOD_IS_OTHER_LOCKED)
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeDestLock;
		else if (uParam == UTOD_IS_WILLING)
		{
			nRet = (Player[CLIENT_PLAYER_INDEX].m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADEOPEN);
		}
		break;

	//uParam = (KUiPlayerLiveSkillBase*) pInfo -> Ö÷½ÇµÄÉú»î¼¼ÄÜÊý¾Ý
	case GDI_LIVE_SKILL_BASE:
		if (uParam)
		{
			KUiPlayerLiveSkillBase* pInfo = (KUiPlayerLiveSkillBase*)uParam;
			//to do:no implements in this version;
			pInfo->nLiveExperience = 0 ;
			pInfo->nRemainPoint = 0 ;
			pInfo->nLiveExperienceFull = 0 ;
			memset(pInfo,0,sizeof(KUiPlayerLiveSkillBase));
		}
		break;

	//Ö÷½ÇÕÆÎÕµÄ¸÷ÏîÉú»î¼¼ÄÜ
	//uParam = (unsigned int*) pSkills -> °üº¬10¸öunsigned intµÄÊý×éÓÃÓÚ´æ´¢¸÷ÏîÉú»î¼¼ÄÜµÄid¡£
	case GDI_LIVE_SKILLS:
		if (uParam)
		{
			//to do:no implements in this version;
			KUiSkillData* pInfo = (KUiSkillData*)uParam;
			memset(pInfo,0,sizeof(KUiSkillData)*10);
		}
		break;

	//Ê£ÓàÕ½¶·¼¼ÄÜµãÊý
	//Return = Ê£ÓàÕ½¶·¼¼ÄÜµãÊý
	case GDI_FIGHT_SKILL_POINT:
		//to do:no implements in this version;
		nRet = Player[CLIENT_PLAYER_INDEX].m_nSkillPoint;
		break;

	//Ö÷½ÇÕÆÎÕµÄ¸÷ÏîÕ½¶·¼¼ÄÜ
	//uParam = (unsigned int*) pSkills -> °üº¬30¸öunsigned intµÄÊý×éÓÃÓÚ´æ´¢¸÷ÏîÕ½¶·¼¼ÄÜµÄid¡£
	case GDI_FIGHT_SKILLS:
		if (uParam)
		{
			KUiSkillData* pSkills = (KUiSkillData*)uParam;
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetSkillSortList(pSkills);
		}
		break;
	//ÏÔÊ¾×ó¼ü¼¼ÄÜÁÐ±í
	//uParam = (KUiSkillData*) pSkills -> °üº¬65¸öKUiSkillDataµÄÊý×éÓÃÓÚ´æ´¢¸÷¼¼ÄÜµÄÊý¾Ý¡£
	//								KUiSkillData::nLevelÓÃÀ´±íÊö¼¼ÄÜÏÔÊ¾ÔÚµÚ¼¸ÐÐ
	//Return = ·µ»ØÓÐÐ§Êý¾ÝµÄSkillsµÄÊýÄ¿
	case GDI_LEFT_ENABLE_SKILLS:
		{
			KUiSkillData * pSkills = (KUiSkillData*)uParam;
			int nCount = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetLeftSkillSortList(pSkills);
			return nCount;

		}
		break;
	//ÏÔÊ¾ÓÒ¼ü¼¼ÄÜÁÐ±í
	//uParam = (KUiSkillData*) pSkills -> °üº¬65¸öKUiSkillDataµÄÊý×éÓÃÓÚ´æ´¢¸÷¼¼ÄÜµÄÊý¾Ý¡£
	//								KUiSkillData::nLevelÓÃÀ´±íÊö¼¼ÄÜÏÔÊ¾ÔÚµÚ¼¸ÐÐ
	//Return = ·µ»ØÓÐÐ§Êý¾ÝµÄSkillsµÄÊýÄ¿
	case GDI_RIGHT_ENABLE_SKILLS:
		{
			KUiSkillData * pSkills = (KUiSkillData*)uParam;
			int nCount = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetRightSkillSortList(pSkills);
			return nCount;
		}
		break;
	//Ö÷½ÇµÄ×Ô´´Îä¹¦
	//uParam = (unsigned int*) pSkills -> °üº¬5¸öunsigned intµÄÊý×éÓÃÓÚ´æ´¢¸÷Ïî×Ô´´Îä¹¦µÄid¡£
	case GDI_CUSTOM_SKILLS:
		if (uParam)
		{
			//to do: no implements in this version;
			KUiSkillData* pSkills = (KUiSkillData*)uParam;
			memset(pSkills,0,sizeof(KUiSkillData)*5);
		}
		break;
	//»ñÈ¡ÖÜÎ§Íæ¼ÒµÄÁÐ±í
	//uParam = (KUiPlayerItem*)pList -> ÈËÔ±ÐÅÏ¢ÁÐ±í
	//			KUiPlayerItem::nData = 0
	//nParam = pListÊý×éÖÐ°üº¬KUiPlayerItem½á¹¹µÄÊýÄ¿
	//Return = Èç¹û·µ»ØÖµÐ¡ÓÚµÈÓÚ´«Èë²ÎÊýnParam£¬ÆäÖµ±íÊ¾pListÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiPlayerItem
	//			½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý£»·ñÔò±íÊ¾ÐèÒª´«Èë°üº¬¶àÉÙ¸öKUiPlayerItem½á¹¹µÄÊý×é
	//			²Å¹»´æ´¢È«²¿ÈËÔ±ÐÅÏ¢¡£
	case GDI_NEARBY_PLAYER_LIST:
		nRet = NpcSet.GetAroundPlayer((KUiPlayerItem*)uParam, nParam);
		break;

	//»ñÈ¡ÖÜÎ§¹Âµ¥¿ÉÊÜÑûÇëµÄÍæ¼ÒµÄÁÐ±í
	//²ÎÊýº¬ÒåÍ¬GDI_NEARBY_PLAYER_LIST
	case GDI_NEARBY_IDLE_PLAYER_LIST:
		nRet = NpcSet.GetAroundPlayerForTeamInvite((KUiPlayerItem*)uParam, nParam);
		break;

	//Ö÷½ÇÍ³Ë§ÄÜÁ¦Ïà¹ØµÄÊý¾Ý
	//uParam = (KUiPlayerLeaderShip*) -> Ö÷½ÇÍ³Ë§ÄÜÁ¦Ïà¹ØµÄÊý¾Ý½á¹¹Ö¸Õë
	case GDI_PLAYER_LEADERSHIP:
		if (uParam)
		{
			KUiPlayerLeaderShip* pInfo = (KUiPlayerLeaderShip*)uParam ;
			pInfo->nLeaderShipExperience = Player[CLIENT_PLAYER_INDEX].m_dwLeadExp ;		//Í³Ë§Á¦¾­ÑéÖµ
			//to do: waiting for...;
			pInfo->nLeaderShipExperienceFull = Player[CLIENT_PLAYER_INDEX].m_dwNextLevelLeadExp;//Í³Ë§Á¦¾­ÑéÖµ
			pInfo->nLeaderShipLevel = Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel ;			//Í³Ë§Á¦µÈ¼¶
		}
		break;
	//»ñµÃÎïÆ·ÔÚÄ³¸ö»·¾³Î»ÖÃµÄÊôÐÔ×´Ì¬
	//uParam = (KUiGameObject*)pObj£¨µ±nParam==0Ê±£©ÎïÆ·µÄÐÅÏ¢
	//uParam = (KUiObjAtContRegion*)pObj£¨µ±nParam!=0Ê±£©ÎïÆ·µÄÐÅÏ¢
	//nParam = (int)(bool)bJustTry  ÊÇ·ñÖ»ÊÇ³¢ÊÔ·ÅÖÃ
	//Return = (ITEM_IN_ENVIRO_PROP)eProp ÎïÆ·µÄÊôÐÔ×´Ì¬
	case GDI_ITEM_IN_ENVIRO_PROP:
		{
			if (!nParam)
			{
				KUiGameObject *pObj = (KUiGameObject *)uParam;
				if (pObj->uGenre != CGOG_ITEM && pObj->uGenre != CGOG_NPCSELLITEM)
					break;

				KItem* pItem = NULL;

				if (pObj->uGenre == CGOG_ITEM && pObj->uId > 0 && pObj->uId < MAX_ITEM)
				{
					pItem = &Item[pObj->uId];
				}
				else if (pObj->uGenre == CGOG_NPCSELLITEM)
				{
					int nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObj->uId);
					pItem = BuySell.GetItem(nIdx);
				}

				_ASSERT(pItem);
				if (!pItem || pItem->GetGenre() != item_equip)
					break;

				if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CanEquip(pItem))
				{
					nRet = IIEP_NORMAL;
				}
				else
				{
					nRet = IIEP_NOT_USEABLE;
				}
			}
			else
			{
				KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
				if (pObj->Obj.uGenre != CGOG_ITEM || pObj->Obj.uId >= MAX_ITEM)
					break;

				int PartConvert[itempart_num] = 
				{ 
					itempart_head,		itempart_weapon,
					itempart_amulet,	itempart_cuff,
					itempart_body,		itempart_belt,
					itempart_ring1,		itempart_ring2,
					itempart_pendant,	itempart_foot,
					itempart_horse,		itempart_mask,	// mat na
				};

				_ASSERT(pObj->eContainer < itempart_num);
				if (pObj->eContainer >= itempart_num || pObj->eContainer < 0)
					break;

				if (Item[pObj->Obj.uId].GetGenre() != item_equip)
					break;

				int nPlace = PartConvert[pObj->eContainer];

				if (Player[CLIENT_PLAYER_INDEX].m_ItemList.CanEquip(pObj->Obj.uId, nPlace))
				{
					nRet = IIEP_NORMAL;
				}
				else
				{
					nRet = IIEP_NOT_USEABLE;
				}
			}
		}
		break;
	case GDI_IMMEDIATEITEM_NUM:
		if (uParam >= 0 && uParam < 3)
			nRet = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetSameDetailItemNum(uParam);
		break;
	//ÓëNPCµÄÂòÂô
	//uParam = (KUiObjAtContRegion*) pInfo -> KUiObjAtRegion½á¹¹Êý×éµÄÖ¸Õë£¬KUiObjAtRegion
	//				½á¹¹ÓÃÓÚ´æ´¢ÎïÆ·µÄÊý¾Ý¼°Æä·ÅÖÃÇøÓòÎ»ÖÃÐÅÏ¢¡£
	//				ÆäÖÐKUiObjAtContRegion::nContainerÖµ±íÊ¾µÚ¼¸Ò³µÄÎïÆ·
	//nParam = pInfoÊý×éÖÐ°üº¬KUiObjAtRegion½á¹¹µÄÊýÄ¿
	//Return = Èç¹û·µ»ØÖµÐ¡ÓÚµÈÓÚ´«Èë²ÎÊýnParam£¬ÆäÖµ±íÊ¾pInfoÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiObjAtRegion
	//			½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý£»·ñÔò±íÊ¾ÐèÒª´«Èë°üº¬¶àÉÙ¸öKUiObjAtRegion½á¹¹µÄÊý×é
	//			²Å¹»´æ´¢È«²¿µÄnpcÁÐ³öÀ´½»Ò×µÄÎïÆ·ÐÅÏ¢¡£
	case GDI_TRADE_NPC_ITEM:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			int nPage = 0;
			int nIndex = 0;
			int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion *)uParam;

			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			if (!BuySell.m_pShopRoom)
				break;
			BuySell.m_pShopRoom->Clear();
			for (int i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);
				
				if (nIndex >= 0 && pItem)
				{
					// Set pInfo->Obj.uGenre
					pInfo->Obj.uGenre = CGOG_NPCSELLITEM;
					// Set pInfo->Obj.uId
					pInfo->Obj.uId = i;

					POINT	Pos;
					if (BuySell.m_pShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos))
					{
						// nIndex + 1±£Ö¤²»Îª0
						BuySell.m_pShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
					}
					else
					{
						nPage++;
						BuySell.m_pShopRoom->Clear();
						// ClearÍê³Éºó±ØÈ»³É¹¦£¬ËùÒÔÃ»ÓÐÅÐ¶Ï
						BuySell.m_pShopRoom->FindRoom(pItem->GetWidth(), pItem->GetHeight(), &Pos);
						BuySell.m_pShopRoom->PlaceItem(Pos.x, Pos.y, nIndex + 1, pItem->GetWidth(), pItem->GetHeight());
					}
					pInfo->Region.h = Pos.x;
					pInfo->Region.v = Pos.y;
					pInfo->Region.Width = pItem->GetWidth();
					pInfo->Region.Height = pItem->GetHeight();
					pInfo->nContainer = nPage;
					nCount++;
					pInfo++;
				}
			}			
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			int nIndex = 0;
			int	nBuyIdx = Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx;
			if (nBuyIdx == -1)
				break;
			if (nBuyIdx >= BuySell.GetHeight())
				break;
			for (int i = 0; i < BuySell.GetWidth(); i++)
			{
				nIndex = BuySell.GetItemIndex(nBuyIdx, i);
				KItem* pItem = BuySell.GetItem(nIndex);
				
				if (nIndex >= 0 && pItem)
				{
					nCount++;
				}
			}
			nRet = nCount;
		}
		break;
	case GDI_CHAT_SEND_CHANNEL_LIST:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.SendSelectChannelList((KUiChatChannel*)uParam, nParam);
		break;
	case GDI_CHAT_RECEIVE_CHANNEL_LIST:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.SendTakeChannelList((KUiChatChannel*)uParam, nParam);
		break;
	case GDI_CHAT_CURRENT_SEND_CHANNEL:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.GetCurChannel((KUiChatChannel*)uParam);
		break;
	case GDI_CHAT_GROUP_INFO:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.IGetTeamInfo(nParam, (KUiChatGroupInfo*)uParam);
		break;
	//ÁÄÌìÒ»¸öºÃÓÑ·Ö×éÖÐºÃÓÑµÄÐÅÏ¢
	//uParam = (KUiPlayerItem*)pList -> ÈËÔ±ÐÅÏ¢ÁÐ±í
	//			KUiPlayerItem::nData = (CHAT_STATUS)eFriendStatus ºÃÓÑµÄµ±Ç°×´Ì¬
	//nParam = Òª»ñÈ¡ÁÐ±íµÄºÃÓÑ·Ö×éµÄË÷Òý
	//Return = ÆäÖµ±íÊ¾pListÊý×éÖÐµÄÇ°¶àÉÙ¸öKUiPlayerItem½á¹¹±»Ìî³äÁËÓÐÐ§µÄÊý¾Ý.
	case GDI_CHAT_FRIENDS_IN_AGROUP:
		nRet = Player[CLIENT_PLAYER_INDEX].m_cChat.IGetTeamFriendInfo(nParam, (KUiPlayerItem*)uParam);
		break;
	case GDI_ITEM_IN_STORE_BOX:
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_repository);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_repositoryroom)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_repositoryroom)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_EX_BOX1:	// mo rong ruong 1
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_exbox1);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_exbox1room)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_exbox1room)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_exbox1room)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_exbox1room)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_EX_BOX2:	// mo rong ruong 2
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_exbox2);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_exbox2room)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_exbox2room)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_exbox2room)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_exbox2room)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_IN_EX_BOX3:	// mo rong ruong 3
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_exbox3);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_exbox3room)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_exbox3room)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_exbox3room)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_exbox3room)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_ITEM_EX:	// mo rong hanh trang
		nRet = 0;
		if (uParam)
		{
			int nCount = 0;
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*)uParam;
			pInfo->Obj.uGenre = CGOG_MONEY;
			pInfo->Obj.uId = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipmentex);
			nCount++;
			pInfo++;
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproomex)
			{
				pInfo->Obj.uGenre = CGOG_ITEM;
				pInfo->Obj.uId = pItem->nIdx;
				pInfo->Region.h = pItem->nX;
				pInfo->Region.v = pItem->nY;
				pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
				pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
				nCount++;
				pInfo++;
			}
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproomex)
				{
					pInfo->Obj.uGenre = CGOG_ITEM;
					pInfo->Obj.uId = pItem->nIdx;		
					pInfo->Region.h = pItem->nX;
					pInfo->Region.v = pItem->nY;
					pInfo->Region.Width = Item[pItem->nIdx].GetWidth();
					pInfo->Region.Height = Item[pItem->nIdx].GetHeight();
					nCount++;
					pInfo++;
				}
				if (nCount > nParam)
					break;
			}
			nRet = nCount;
		}
		else
		{
			int nCount = 0;
			// µÚÒ»¸öÊÇÇ®
			nCount++;
			// ºóÃæÊÇ×°±¸
			PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem();
			if (pItem && pItem->nPlace == pos_equiproomex)
				nCount++;
			while(pItem)
			{
				pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem();
				if (pItem && pItem->nPlace == pos_equiproomex)
					nCount++;
			}
			nRet = nCount;
		}
		break;
	case GDI_PK_SETTING:					//»ñÈ¡pkÉèÖÃ
				nRet = Player[CLIENT_PLAYER_INDEX].m_cPK.GetNormalPKState();
		break;
	case GDI_SHOW_PLAYERS_NAME:			//»ñÈ¡ÏÔÊ¾¸÷Íæ¼ÒÈËÃû
		nRet = NpcSet.CheckShowName();
		break;
	case GDI_SHOW_PLAYERS_LIFE:			//»ñÈ¡ÏÔÊ¾¸÷Íæ¼ÒÉúÃü
		nRet = NpcSet.CheckShowLife();
		break;
	case GDI_SHOW_PLAYERS_MANA:			//»ñÈ¡ÏÔÊ¾¸÷Íæ¼ÒÄÚÁ¦
		nRet = NpcSet.CheckShowMana();
		break;
	case GDI_IS_CHEST_UNLOCKED:			//Kiem tra xem nguoi choi da khoa ruong lai hay chua
		nRet = Player[CLIENT_PLAYER_INDEX].m_CUnlocked;
		break;
	case GDI_EXBOX_ID: 				// truyen id mo rong ruong
		{	
			int nExBoxId = 0;
			if (nExBoxId < 0)
				return 0;
			nExBoxId = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ExBoxId;
				return nExBoxId;
			break;
		}
	case GDI_SELL_ITEM_NUM:			//truyen so luong item trong hanh trang
		{ 
			if(uParam)
			{
				KUiObjAtContRegion *pObj = (KUiObjAtContRegion *)uParam;
				int nNo = 0;
				int nIdx = pObj->Obj.uId;
				int nGenre = Item[nIdx].GetGenre();
				int nDetail = Item[nIdx].GetDetailType();
				int nParticular = Item[nIdx].GetParticular();
				int nLevel = Item[nIdx].GetLevel();
				int nX = 0, nY = 0;
				return Player[CLIENT_PLAYER_INDEX].m_ItemList.GetItemNum(nGenre, nDetail, nParticular, nLevel);
			}
			break;
		}
	case GDI_PLAYER_TK_TIME:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_TIME);
		break;
	case GDI_PLAYER_TK_POINT:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_DIEM);
		break;
	case GDI_PLAYER_TK_PLAYER:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_PLAYER);
		break;	
	case GDI_PLAYER_TK_NPC:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_NPC);
		break;	
	case GDI_PLAYER_TK_XEPHANG:	
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(TASKVALUE_XEPHANG);
		break;	
	}
	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÏòÓÎÏ··¢ËÍ²Ù×÷
//	²ÎÊý£ºunsigned int uDataId --> CoreÍâ²¿¿Í»§¶ÔcoreµÄ²Ù×÷ÇëÇóµÄË÷Òý¶¨Òå
//							ÆäÖµÎªÃ·¾ÙÀàÐÍGAMEOPERATION_INDEXµÄÈ¡ÖµÖ®Ò»¡£
//		  unsigned int uParam  --> ÒÀ¾ÝuOperIdµÄÈ¡ÖµÇé¿ö¶ø¶¨
//		  int nParam --> ÒÀ¾ÝuOperIdµÄÈ¡ÖµÇé¿ö¶ø¶¨
//	·µ»Ø£ºÈç¹û³É¹¦·¢ËÍ²Ù×÷ÇëÇó£¬º¯Êý·µ»Ø·Ç0Öµ£¬·ñÔò·µ»Ø0Öµ¡£
//--------------------------------------------------------------------------
int	KCoreShell::OperationRequest(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 1;
	switch(uOper)
	{
	case GOI_CP_UNLOCK:
		SendClientCPUnlockCmd(uParam);
		break;
	case GOI_CP_LOCK:
		SendClientCPLockCmd();
		break;
	case GOI_CP_CHANGE:
		SendClientCPChangeCmd(uParam, nParam);
		break;
	case GOI_CP_RESET:
		SendClientCPResetCmd(nParam);
		break;
	case GOI_MARKET:// KTC
		{
				if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode == 0)
				{
						SendClientOpenMarket();
				}
				else
				{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "<color=yellow>Kú Tr©n C¸c chØ më khu vùc phi chiÕn ®Êu !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
				}
				break;
	}
	break;
	case GOI_STRINGBOX:// stringbox
		SendClientString(uParam);
		break;
	case GOI_DATAU:// Da Tau
		SendClientDaTau(uParam);
		break;
	case GOI_TRADE_PLAYER_BUY:
		{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			if (CGOG_PLAYERSELLITEM != pObject1->Obj.uGenre)
				break;			
			
			int nWidth, nHeight;
			ItemPos	Pos;
			
			nWidth = Item[pObject1->Obj.uId].GetWidth();
			nHeight = Item[pObject1->Obj.uId].GetHeight();
			if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
			{
				nRet = 0;
				break;
			}
			if (Pos.nPlace != pos_equiproom)
			{
				nRet = 0;
				
				KSystemMessage	sMsg;
				
				strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}
			
			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < Item[pObject1->Obj.uId].GetSetPrice())
			{
				nRet = 0;
				KSystemMessage	sMsg;
				
				strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
				
			}
			
			
			nRet = 1;
			SendClientCmdPlayerBuy(g_cSellItem.FindIdx(pObject1->Obj.uId), nParam, pos_equiproom, Pos.nX, Pos.nY);
		}
		break;

	case GOI_VIEW_PLAYERSELLITEM_END:
		g_cSellItem.DeleteAll();
		break;
	case GOI_VIEW_PLAYERSELLITEM:
		if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_BaiTan)
			g_cSellItem.ApplyViewItem(uParam);
		break;
	case GDI_PLAYER_TRADE:
		{
			char* sShopName = (char *)uParam;
			
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, "§ang c­ëi ngùa kh«ng thÓ bµy b¸n!");
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				return 0;
			}
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode)
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, "Kh«ng thÓ bµy b¸n ë khu vùc chiÕn ®Êu!");
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				return 0;
			}
			
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_sit)
			{
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_sit);
				SendClientCmdSit(TRUE);
			}
			
	/*		if (Player[CLIENT_PLAYER_INDEX].m_cTask.GetSaveVal(100) == 0)
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, "§· hÕt h¹n giÊy phÐp bµy b¸n !");
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				return 0;
			}*/
			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_BaiTan == 0)
				SendClientCmdStartTrade(1,sShopName);
			else
				SendClientCmdStartTrade(0,"");
			// 			return 1;
		}
		break;
	case GDI_SET_TRADE_ITEM:
		{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			
			//·ÅÏÂÈ¥µÄ¶«Î÷²»Îª¿Õ£¬ËùÒÔÊÇÂô¶«Î÷
			int nIdx = pObject1->Obj.uId;	//Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
			if (nIdx > 0 && nIdx < MAX_ITEM)
			{
				Item[nIdx].SetPrice(nParam);
				SendClientCmdSetPrice(Item[nIdx].GetID(),nParam);
			}
		}
		break;


	case GOI_QUERY_RANK_INFORMATION:
		SendClientCmdQueryLadder(uParam);
		break;
	//uParam = (const char*)pszFileName
	case GOI_PLAY_SOUND:
		if (uParam)
		{
			static KCacheNode* pSndNode = NULL;
			KWavSound* pSound = NULL;
			pSndNode	= (KCacheNode*)g_SoundCache.GetNode((char *)uParam, (KCacheNode * )pSndNode);
			pSound		= (KWavSound*)pSndNode->m_lpData;
			if (pSound)
			{
				if (pSound->IsPlaying())
					break;
				pSound->Play(0, -10000 + Option.GetSndVolume() * 100, 0);
			}
		}
		break;
	case GOI_PLAYER_RENASCENCE:
		{
			int nReviveType;
			if (nParam)	// bBackTown
			{
				nReviveType = REMOTE_REVIVE_TYPE;
			}
			else
			{
				nReviveType = LOCAL_REVIVE_TYPE;
			}
//			SendClientCmdRevive(nReviveType);
			SendClientCmdRevive();
		}
		break;
	case GOI_MONEY_INOUT_STORE_BOX:
		{
			BOOL	bIn = (BOOL)uParam;
			int		nMoney = nParam;
			int		nSrcRoom, nDesRoom;


			if (bIn)
			{
				nSrcRoom = room_equipment;
				nDesRoom = room_repository;
			}
			else
			{
				nDesRoom = room_equipment;
				nSrcRoom = room_repository;
			}
			Player[CLIENT_PLAYER_INDEX].m_ItemList.ExchangeMoney(nSrcRoom, nDesRoom, nMoney);
		}
		break;
	case GOI_GIVE_ITEM_BACK:
		SendClientCmdGive(uParam,nParam,1);
		break;
	case GOI_GIVE_ITEM_NOBACK:
		SendClientCmdGive(uParam,nParam,2);
		break;
	case GOI_GIVE_ITEM_FAIL:
		SendClientCmdGive(uParam,nParam,0);
		break;
	case GOI_PKVALUE:
		SendClientPKValue(uParam);
		break;
	case GOI_LIXIAN:
		SendClientCmdLiXian();
		break;
	case GOI_EXIT_GAME:
		g_SubWorldSet.Close();
		g_ScenePlace.ClosePlace();
		break;
	case GOI_GAMESPACE_DISCONNECTED:
		g_SubWorldSet.Close();
		break;
	case GOI_TRADE_NPC_BUY:
	{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			if (CGOG_NPCSELLITEM != pObject1->Obj.uGenre)
					break;

			int nIdx = 0;
			KItem* pItem = NULL;

			nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject1->Obj.uId);
			pItem = BuySell.GetItem(nIdx);

			int nWidth, nHeight;
			ItemPos	Pos;

			nWidth = pItem->GetWidth();
			nHeight = pItem->GetHeight();
			if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
			{
				nRet = 0;
				break;
			}
			if (Pos.nPlace != pos_equiproom)
			{
				nRet = 0;

				KSystemMessage	sMsg;
				
				strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}

			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < pItem->GetPrice())
			{
				nRet = 0;
				KSystemMessage	sMsg;
				
				strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}
			if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentXu() < pItem->GetPriceXu())
			{
				nRet = 0;
				KSystemMessage	sMsg;
				
				strcpy(sMsg.szMessage, MSG_SHOP_NO_XU);
				sMsg.eType = SMT_SYSTEM;
				sMsg.byConfirmType = SMCT_CLICK;
				sMsg.byPriority = 1;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				break;
			}
			SendClientCmdBuy(pObject1->Obj.uId, pos_equiproom, Pos.nX, Pos.nY);
		}
		break;
	case GOI_TRADE_NPC_SELL:
		{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;

			if (CGOG_ITEM != pObject1->Obj.uGenre)
				break;
			//·ÅÏÂÈ¥µÄ¶«Î÷²»Îª¿Õ£¬ËùÒÔÊÇÂô¶«Î÷
			int nIdx = pObject1->Obj.uId;	//Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
			if (nIdx > 0 && nIdx < MAX_ITEM)
			{
				if (Item[nIdx].GetGenre() == item_task)
				{
					if (Item[nIdx].GetIsSell() == 1)
					{	
						SendClientCmdSell(Item[nIdx].GetID());
						return 1;
					}
					else 
					{
						KSystemMessage	sMsg;
						sprintf(sMsg.szMessage, MSG_TRADE_TASK_ITEM);
						sMsg.eType = SMT_NORMAL;
						sMsg.byConfirmType = SMCT_NONE;
						sMsg.byPriority = 0;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						return 0;
					}
				}
				SendClientCmdSell(Item[nIdx].GetID());
				return 1;
			}
			else
			{
				return 0;
			}
		}
		break;
	case GOI_TRADE_NPC_REPAIR:
		{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;

			if (CGOG_ITEM != pObject1->Obj.uGenre)
				break;
			//·ÅÏÂÈ¥µÄ¶«Î÷²»Îª¿Õ£¬ËùÒÔÊÇÂô¶«Î÷
			int nIdx = pObject1->Obj.uId;	//Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
			if (nIdx > 0 && nIdx < MAX_ITEM)
			{		
				if (Item[nIdx].GetGenre() != item_equip)
				{
/*					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_TRADE_TASK_ITEM);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);*/
					return 0;
				}
				else if (Item[nIdx].GetDurability() == -1 || Item[nIdx].GetDurability() == Item[nIdx].GetMaxDurability())
				{
					return 0;
				}
				else if (Item[nIdx].GetRepairPrice() <= Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney())
				{
					SendClientCmdRepair(Item[nIdx].GetID());
				}
				else
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_SHOP_NO_MONEY);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return 0;
				}
				
				return 1;
			}
			else
			{
				return 0;
			}
		}
		break;
	case GOI_NPC_ITEM_BREAK:
		{
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;

			if (CGOG_ITEM != pObject1->Obj.uGenre)
				break;
			//·ÅÏÂÈ¥µÄ¶«Î÷²»Îª¿Õ£¬ËùÒÔÊÇÂô¶«Î÷
			int nIdx = pObject1->Obj.uId;	//Player[CLIENT_PLAYER_INDEX].m_ItemList.Hand();
			if (nIdx < MAX_ITEM && nIdx != 0)
			{		
				if (Item[nIdx].GetStackNum() > nParam)
				{
					Item[nIdx].SetStackNum(Item[nIdx].GetStackNum() - nParam);
					SendClientCmdBreak(Item[nIdx].GetID(),nParam);
				}
				else
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, "B¹n kh«ng ®ñ vËt phÈm!");
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					return 0;
				}
				
				return 1;
			}
			else
			{
				return 0;
			}
		}
		break;
	case GOI_SWITCH_OBJECT:
		//uParam = (KUiObjAtContRegion*)pObject1 -> ÄÃÆðµÄÎïÆ·²Ù×÷Ç°µÄÐÅÏ¢
		//nParam = (KUiObjAtContRegion*)pObject2 -> ·ÅÏÂµÄÎïÆ·²Ù×÷ºóµÄÐÅÏ¢
		{
			ItemPos	P1, P2;
			int PartConvert[itempart_num] = 
			{ 
				itempart_head,		itempart_weapon,
				itempart_amulet,	itempart_cuff,
				itempart_body,		itempart_belt,
				itempart_ring1,		itempart_ring2,
				itempart_pendant,	itempart_foot,
				itempart_horse,		itempart_mask,	// mat na
			};
			KUiObjAtContRegion* pObject1 = (KUiObjAtContRegion*)uParam;
			KUiObjAtContRegion* pObject2 = (KUiObjAtContRegion*)nParam;
			
			if (!pObject1 && !pObject2)
				break;
			
			if (pObject1)
			{
				switch(pObject1->eContainer)
				{
				case UOC_STORE_BOX:
					P1.nPlace = pos_repositoryroom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_EX_BOX1:	// mo rong ruong 1
					P1.nPlace = pos_exbox1room;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_EX_BOX2:	// mo rong ruong 2
					P1.nPlace = pos_exbox2room;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_EX_BOX3:	// mo rong ruong 3
					P1.nPlace = pos_exbox3room;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_ITEM_EX:	// mo rong hanh trang 
					P1.nPlace = pos_equiproomex;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_IMMEDIA_ITEM:
					P1.nPlace = pos_immediacy;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_ITEM_TAKE_WITH:
					P1.nPlace = pos_equiproom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_EQUIPTMENT:
					{
						// TODO:ÔÝÊ±Ã»ÓÐµÚ¶þ×°±¸
						if (pObject1->Region.h == 1)
							break;
						P1.nPlace = pos_equip;
						P1.nX = PartConvert[pObject1->Region.v];
					}
					break;
				case UOC_TO_BE_TRADE:
					P1.nPlace = pos_traderoom;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_ITEM_GIVE:
					P1.nPlace = pos_give;
					P1.nX = pObject1->Region.h;
					P1.nY = pObject1->Region.v;
					break;
				case UOC_NPC_SHOP:
				{
					if (CGOG_NPCSELLITEM != pObject1->Obj.uGenre)
						break;

					int nIdx = 0;
					KItem* pItem = NULL;
					
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject1->Obj.uId);
					pItem = BuySell.GetItem(nIdx);
					
					int nWidth, nHeight;
					ItemPos	Pos;
					
					nWidth = pItem->GetWidth();
					nHeight = pItem->GetHeight();
					if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					if (Pos.nPlace != pos_equiproom)
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentMoney() < pItem->GetPrice())
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_MONEY);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					SendClientCmdBuy(pObject1->Obj.uId, pos_equiproom, Pos.nX, Pos.nY);
				}
					break;
				case UOC_MARKET:
				{
					if (CGOG_NPCSELLITEM != pObject1->Obj.uGenre)
						break;

					int nIdx = 0;
					KItem* pItem = NULL;
					
					nIdx = BuySell.GetItemIndex(Player[CLIENT_PLAYER_INDEX].m_BuyInfo.m_nBuyIdx, pObject1->Obj.uId);
					pItem = BuySell.GetItem(nIdx);
					
					int nWidth, nHeight;
					ItemPos	Pos;
					
					nWidth = pItem->GetWidth();
					nHeight = pItem->GetHeight();
					if (!Player[CLIENT_PLAYER_INDEX].m_ItemList.SearchPosition(nWidth, nHeight, &Pos))
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					if (Pos.nPlace != pos_equiproom)
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetEquipmentXu() < pItem->GetPriceXu())
					{
						nRet = 0;
						KSystemMessage	sMsg;
						
						strcpy(sMsg.szMessage, MSG_SHOP_NO_XU);
						sMsg.eType = SMT_SYSTEM;
						sMsg.byConfirmType = SMCT_CLICK;
						sMsg.byPriority = 1;
						sMsg.byParamSize = 0;
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
						break;
					}
					SendClientCmdBuy(pObject1->Obj.uId, pos_equiproom, Pos.nX, Pos.nY);
				}
					break;
				}
			}

			if (pObject2)
			{
				switch(pObject2->eContainer)
				{
				case UOC_STORE_BOX:
					P2.nPlace = pos_repositoryroom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_EX_BOX1:	// mo rong ruong 1
					P2.nPlace = pos_exbox1room;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_EX_BOX2:	// mo rong ruong 2
					P2.nPlace = pos_exbox2room;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_EX_BOX3:	// mo rong ruong 3
					P2.nPlace = pos_exbox3room;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_ITEM_EX:	// mo rong hanh trang
					P2.nPlace = pos_equiproomex;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_IMMEDIA_ITEM:
					P2.nPlace = pos_immediacy;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_ITEM_TAKE_WITH:
					P2.nPlace = pos_equiproom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_EQUIPTMENT:
					{
						// TODO:ÔÝÊ±Ã»ÓÐµÚ¶þ×°±¸
						if (pObject2->Region.h == 1)
							break;
						P2.nPlace = pos_equip;
						P2.nX = PartConvert[pObject2->Region.v];
					}
					break;
				case UOC_TO_BE_TRADE:
					P2.nPlace = pos_traderoom;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_ITEM_GIVE:
					P2.nPlace = pos_give;
					P2.nX = pObject2->Region.h;
					P2.nY = pObject2->Region.v;
					break;
				case UOC_NPC_SHOP:
					break;
				case UOC_MARKET:
					break;
				}
			}
			if (!pObject1)
			{
				memcpy(&P1, &P2, sizeof(P1));
			}
			if (!pObject2)
			{
				memcpy(&P2, &P1, sizeof(P1));
			}
			Player[CLIENT_PLAYER_INDEX].MoveItem(P1, P2);
		}
		break;

	//Íæ¼ÒµãÍê¶Ô»°¿ò
	case GOI_INFORMATION_CONFIRM_NOTIFY:
	{
		PLAYER_SELECTUI_COMMAND command;
		command.nSelectIndex = 0;
		Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_TALKDIALOG);
		break;
	}
	
	//ÎÊÌâÑ¡Ôñ´ð°¸
	//nParma = nAnswerIndex
	case GOI_QUESTION_CHOOSE:
		if (g_bUISelLastSelCount == 0 )
			break;	
		{
			PLAYER_SELECTUI_COMMAND command;
			command.nSelectIndex = nParam;
			Player[CLIENT_PLAYER_INDEX].OnSelectFromUI(&command, UI_SELECTDIALOG);
		}
		break;

	//Ê¹ÓÃÎïÆ·
	//uParam = (KUiObjAtRegion*)pInfo -> ÎïÆ·µÄÊý¾ÝÒÔ¼°ÎïÆ·Ô­À´°Ú·ÅµÄÎ»ÖÃ
	//nParam = ÎïÆ·Ê¹ÓÃÇ°·ÅÖÃµÄÎ»ÖÃ£¬È¡ÖµÎªÃ¶¾ÙÀàÐÍUIOBJECT_CONTAINER¡£
	case GOI_USE_ITEM:
		//to do: waiting for...
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
			int nPlace = nParam;
			ItemPos	Pos;
			switch(nPlace)
			{
			case UOC_ITEM_TAKE_WITH:
				Pos.nPlace = pos_equiproom;
				break;
			case UOC_IMMEDIA_ITEM:
				Pos.nPlace = pos_immediacy;
				break;
			default:
				Pos.nPlace = -1;
				break;
			}
			Pos.nX = pInfo->Region.h;
			Pos.nY = pInfo->Region.v;
					
			if (pInfo->Obj.uGenre == CGOG_ITEM && pInfo->Obj.uId > 0 && Pos.nPlace != -1)
				Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pInfo->Obj.uId, Pos);
		}
		break;

	case GOi_SET_WINDOWS_ITEM:
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
			int nPlace = nParam;
			int nDetail = Item[pInfo->Obj.uId].GetDetailType();
			int nPartuil = Item[pInfo->Obj.uId].GetParticular();
			int bAdd = 0;
			switch(nPlace)
			{
			case 0:
				if(!Item[pInfo->Obj.uId].IsPurple() && Item[pInfo->Obj.uId].GetGoldId() < 1)
				{
					if (nDetail == 3)
					{
						bAdd = 1;
					}
					if (nDetail == 4)
					{
						bAdd = 2;
					}
					if (nDetail == 9)
					{
						bAdd = 3;
					}
				}
				break;
			case 1:
				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && nPartuil == 147)
				{
					bAdd = 1;
				}
				break;
			case 2:
				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && nPartuil >= 200 && nPartuil <= 205)
				{
					bAdd = 1;
				}
				break;
			case 3:
				if(Item[pInfo->Obj.uId].GetGenre() == item_equip && Item[pInfo->Obj.uId].GetGoldId() < 1 && !Item[pInfo->Obj.uId].IsPurple())
//					if ((nDetail >= 0 && nDetail <= 2) || (nDetail >= 5 && nDetail <=8 ))
					{
						bAdd = 1;	//×°±¸
					}

				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && nPartuil == 147)
				{
					bAdd = 2;		//Ðþ¾§
				}

				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && nPartuil >= 149 && nPartuil <= 154)
				{
					bAdd = 3;		//¿óÊ¯
				}

				if (
					(Item[pInfo->Obj.uId].GetGenre() == item_mine && 
					Item[pInfo->Obj.uId].GetParticular() == 122) || 
					(Item[pInfo->Obj.uId].GetGenre() == item_task && 
					(Item[pInfo->Obj.uId].GetDetailType() == 215 ||
					Item[pInfo->Obj.uId].GetDetailType() > 238 && Item[pInfo->Obj.uId].GetDetailType() <= 241))
					
					)
				{
					bAdd = 4;		//ÆäËû
				}
				if ((Item[pInfo->Obj.uId].GetGenre() == item_mine && 
					Item[pInfo->Obj.uId].GetParticular() == 123) ||
					(Item[pInfo->Obj.uId].GetGenre() == item_task && 
					Item[pInfo->Obj.uId].GetDetailType() == 216))
					bAdd = 4;		//ÆäËû
				
				if ((Item[pInfo->Obj.uId].GetGenre() == item_mine && 
					Item[pInfo->Obj.uId].GetParticular() == 124) ||
					(Item[pInfo->Obj.uId].GetGenre() == item_task && 
					Item[pInfo->Obj.uId].GetDetailType() == 217))
					bAdd = 4;		//ÆäËû
				
				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && 
					Item[pInfo->Obj.uId].GetParticular() == 398)
						bAdd = 4;		//ÆäËû
				break;
			case 4:
				if(Item[pInfo->Obj.uId].GetGenre() == item_equip && Item[pInfo->Obj.uId].GetGoldId() < 1 && !Item[pInfo->Obj.uId].IsPurple())
					if ((nDetail >= 0 && nDetail <= 2) || (nDetail >= 5 && nDetail <=8 ))
					{
						bAdd = 1;	//×°±¸
					}
					
				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && nPartuil == 147)
				{
						bAdd = 2;		//Ðþ¾§
				}
				break;
			case 5:
				if(Item[pInfo->Obj.uId].GetGenre() == item_equip && Item[pInfo->Obj.uId].GetGoldId() < 1 && Item[pInfo->Obj.uId].IsPurple())
					if ((nDetail >= 0 && nDetail <= 2) || (nDetail >= 5 && nDetail <=8 ))
					{
						bAdd = 1;	//×°±¸
					}
					
				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && nPartuil == 147)
				{
					bAdd = 2;		//Ðþ¾§
				}
					
				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && nPartuil >= 200 && nPartuil <= 205)
				{
					bAdd = 3;		//¿óÊ¯
				}
					
				if (
					(Item[pInfo->Obj.uId].GetGenre() == item_mine && 
					Item[pInfo->Obj.uId].GetParticular() == 122) || 
					(Item[pInfo->Obj.uId].GetGenre() == item_task && 
					(Item[pInfo->Obj.uId].GetDetailType() == 215 ||
					Item[pInfo->Obj.uId].GetDetailType() > 238 && Item[pInfo->Obj.uId].GetDetailType() <= 241))

					)
				{
					bAdd = 4;		//ÆäËû
				}
				if ((Item[pInfo->Obj.uId].GetGenre() == item_mine && 
					Item[pInfo->Obj.uId].GetParticular() == 123) ||
					(Item[pInfo->Obj.uId].GetGenre() == item_task && 
					Item[pInfo->Obj.uId].GetDetailType() == 216))
						bAdd = 4;		//ÆäËû
				
				if ((Item[pInfo->Obj.uId].GetGenre() == item_mine && 
					Item[pInfo->Obj.uId].GetParticular() == 124) ||
					(Item[pInfo->Obj.uId].GetGenre() == item_task && 
					Item[pInfo->Obj.uId].GetDetailType() == 217))
						bAdd = 4;		//ÆäËû
				
				if (Item[pInfo->Obj.uId].GetGenre() == item_mine && 
					Item[pInfo->Obj.uId].GetParticular() == 398)
						bAdd = 4;		//ÆäËû

				break;
			case 6:
				break;
			default:
				bAdd = 0;
				break;
			}
			if (bAdd)
			{
				CoreDataChanged(GDCNI_SET_ITEM,(unsigned int)pInfo,bAdd);
			}
		}
		break;


	case GOI_COMPITEM_COM:
		if (uParam)
		{
			KUiObjAtContRegion* pInfo = (KUiObjAtContRegion*) uParam;

			switch(nParam)
			{
			case 1:
				SendClientCmdComp(uParam,nParam,1);
				break;
			case 2:
				SendClientCmdComp(uParam,nParam,2);
				break;
			case 3:
				SendClientCmdComp(uParam,nParam,3);
				break;
			}
		}
		break;
	case GOI_COMPITEM_DISTILL:
		if (uParam)
		{	
			SendClientCmdComp(uParam,nParam,4);
		}
		break;
	case GOI_COMPITEM_FORGE:
		if (uParam)
		{	
			SendClientCmdComp(uParam,nParam,5);
		}
		break;
	case GOI_COMPITEM_ENCHASE:
		if (uParam)
		{	
			SendClientCmdComp(uParam,nParam,6);
		}
		break;
	case GOI_COMPITEM_ALTAL:
		if (uParam)
		{	
			SendClientCmdComp(uParam,nParam,7);
		}
		break;


	//´©ÉÏ×°±¸
	//uParam = (KUiObjAtRegion*)pInfo -> ×°±¸µÄÊý¾ÝºÍ·ÅÖÃÎ»ÖÃÐÅÏ¢
	//			KUiObjAtRegion::Region::h ±íÊ¾ÊôÓÚµÚ¼¸Ì××°±¸
	//			KUiObjAtRegion::Region::v ±íÊ¾ÊôÓÚÄÄ¸öÎ»ÖÃµÄ×°±¸,ÆäÖµÎªÃ·¾ÙÀàÐÍ
	//			UI_EQUIPMENT_POSITIONµÄÈ¡ÖµÖ®Ò»¡£Çë²Î¿´UI_EQUIPMENT_POSITIONµÄ×¢ÊÍ¡£
case GOI_AUTO_COMMAND: 
        { 
            switch(uParam) 
            { 
            case 0:{ //mau' 
                ItemPos    Pos; 
                PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem(); 
                if (pItem && (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_immediacy) && 
                    (Item[pItem->nIdx].GetGenre() == item_medicine && (Item[pItem->nIdx].GetDetailType() == medicine_blood 
                    || Item[pItem->nIdx].GetDetailType() == medicine_both))) 
                    { 
                    Pos.nPlace = pItem->nPlace; 
                    Pos.nX = pItem->nX; 
                    Pos.nY = pItem->nY; 
                    Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos); 
                    return 1; 
                    } 
                while(pItem) 
                { 
                    pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem(); 
                    if (pItem && (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_immediacy) && 
                    (Item[pItem->nIdx].GetGenre() == item_medicine && (Item[pItem->nIdx].GetDetailType() == medicine_blood 
                    || Item[pItem->nIdx].GetDetailType() == medicine_both))) 
                    { 
                    Pos.nPlace = pItem->nPlace; 
                    Pos.nX = pItem->nX; 
                    Pos.nY = pItem->nY; 
                    Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos); 
                    return 1; 
                    } 
                } 
                }break; 
            case 1:{//mana 
                ItemPos    Pos; 
                PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem(); 
                if (pItem && (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_immediacy) && 
                    (Item[pItem->nIdx].GetGenre() == item_medicine && (Item[pItem->nIdx].GetDetailType() == medicine_mana 
                    || Item[pItem->nIdx].GetDetailType() == medicine_both))) 
                    { 
                    Pos.nPlace = pItem->nPlace; 
                    Pos.nX = pItem->nX; 
                    Pos.nY = pItem->nY; 
                    Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos); 
                    return 1; 
                    } 
                while(pItem) 
                { 
                    pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem(); 
                    if (pItem && (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_immediacy) && 
                    (Item[pItem->nIdx].GetGenre() == item_medicine && (Item[pItem->nIdx].GetDetailType() == medicine_mana 
                    || Item[pItem->nIdx].GetDetailType() == medicine_both))) 
                    { 
                    Pos.nPlace = pItem->nPlace; 
                    Pos.nX = pItem->nX; 
                    Pos.nY = pItem->nY; 
                    Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos); 
                    return 1; 
                    } 
                }                
                }break; 
            case 2:{//tdp
                ItemPos    Pos; 
                PlayerItem* pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetFirstItem(); 
                if (pItem && (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_immediacy) && 
                    (Item[pItem->nIdx].GetGenre() == item_townportal))
                    { 
                    Pos.nPlace = pItem->nPlace; 
                    Pos.nX = pItem->nX; 
                    Pos.nY = pItem->nY; 
                    Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos); 
                    return 1; 
                    } 
                while(pItem) 
                { 
                    pItem = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetNextItem(); 
                    if (pItem && (pItem->nPlace == pos_equiproom || pItem->nPlace == pos_immediacy) && 
                    (Item[pItem->nIdx].GetGenre() == item_townportal))
                    { 
                    Pos.nPlace = pItem->nPlace; 
                    Pos.nX = pItem->nX; 
                    Pos.nY = pItem->nY; 
                    Player[CLIENT_PLAYER_INDEX].ApplyUseItem(pItem->nIdx, Pos); 
                    return 1; 
                    } 
                } 
                }break; 
			case 3:{  //tu dong danh
					Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(166, 1);
                }break;
			case 4:{  //tu dong danh
					Player[CLIENT_PLAYER_INDEX].m_cTask.SetSaveVal(166, 0);
                }break;
            } 
        } 
        break; 

	case GOI_FINDPOS: 
		{
		} 
        break; 

    case GOI_WEAR_EQUIP:
		//to do: waiting for...
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
		}
		break;

	//Ê©Õ¹Îä¹¦/¼¼ÄÜ
	//uParam = (KUiGameObject*)pInfo -> ¼¼ÄÜÊý¾Ý
	case GOI_USE_SKILL:	
		if (uParam)
		{
			//to do:wating for...
			KUiGameObject* pInfo = (KUiGameObject*)uParam;
		}
		break;

	//ÉèÖÃÁ¢¼´¼¼ÄÜ
	//uParam = (KUiGameObject*)pSKill, ¼¼ÄÜÐÅÏ¢
	//nParam = Á¢¼´Î»ÖÃ£¬0±íÊ¾ÎªÓÒ¼ü¼¼ÄÜ£¬1ÖÁ4±íÊ¾ÎªF1ÖÁF4¼¼ÄÜ
	case GOI_SET_IMMDIA_SKILL:
		if (uParam)
		{
			KUiGameObject* pSkill = (KUiGameObject*)uParam;
			if (nParam == 0)
				//to do : modify;
			{
				if ( (int)pSkill->uId > 0 )
					Player[CLIENT_PLAYER_INDEX].SetLeftSkill((int)pSkill->uId);
				else if ((int)(pSkill->uId) == -1) //ÉèÖÃµ±Ç°ÎïÀí¼¼ÄÜÎª×ó¼¼ÄÜ
				{
					int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
					int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
					
					//½üÉíÎäÆ÷
					if (nDetailType == 0)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nMeleeWeaponSkill[nParticularType]);
					}//Ô¶³ÌÎäÆ÷
					else if (nDetailType == 1)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nRangeWeaponSkill[nParticularType]);
					}//¿ÕÊÖ
					else if (nDetailType == -1)
					{
						Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nHandSkill);
					}
					
				}
			}
			else if (nParam == 1)
			{
				if ((int)pSkill->uId > 0)
					Player[CLIENT_PLAYER_INDEX].SetRightSkill((int)pSkill->uId);
				else if ((int)(pSkill->uId) == -1)
				{
					int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
					int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
					
					if (nDetailType == 0)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nMeleeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == 1)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nRangeWeaponSkill[nParticularType]);
					}
					else if (nDetailType == -1)
					{
						Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nHandSkill);
					}
				}
			}
		}
		break;

	//ÔöÇ¿Ò»ÖÖ¼¼ÄÜ£¬£¬Ò»´Î¼ÓÒ»µã
	//uParam = ¼¼ÄÜÀàÊô
	//nParam = (uint)¼¼ÄÜid
	case GOI_TONE_UP_SKILL:
		Player[CLIENT_PLAYER_INDEX].ApplyAddSkillLevel((int)nParam, 1);
		break;

	//ÔöÇ¿Ò»Ð©ÊôÐÔµÄÖµ£¬Ò»´Î¼ÓÒ»µã
	//uParam = ±íÊ¾ÒªÔöÇ¿µÄÊÇÄÄ¸öÊôÐÔ£¬È¡ÖµÎªUI_PLAYER_ATTRIBUTEµÄÃ·¾ÙÖµÖ®Ò»
	case GOI_TONE_UP_ATTRIBUTE:
		switch (uParam)
		{
		case UIPA_STRENGTH:		//Á¦Á¿
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(0, 1);
			break;
		case UIPA_DEXTERITY:	//Ãô½Ý
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(1, 1);
			break;		
		case UIPA_VITALITY:		//»îÁ¦
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(2, 1);
			break;
		case UIPA_ENERGY:		//¾«Á¦
			Player[CLIENT_PLAYER_INDEX].ApplyAddBaseAttribute(3, 1);
			break;		
		}
		break;

	//´ðÓ¦/¾Ü¾ø½»Ò×ÇëÇó
	//uParam = (KUiPlayerItem*)pRequestPlayer ·¢³öÇëÇóµÄÍæ¼Ò
	//nParam = (int)(bool)bAccept ÊÇ·ñ½ÓÊÜÇëÇó
	case GOI_TRADE_INVITE_RESPONSE:
		if (uParam)
		{
			KTrade::ReplyInvite(((KUiPlayerItem*)uParam)->nIndex, nParam);
		}
		break;

	//Ôö¼õÒ»¸öÓûÂô³öµÄÎïÆ·
	//uParam = (KUiObjAtRegion*) pObject -> ÎïÆ·ÐÅÏ¢£¬ÆäÖÐ×ø±êÐÅÏ¢ÎªÔÚ½»Ò×½çÃæÖÐµÄ×ø±ê
	//nParam = bAdd -> 0Öµ±íÊ¾¼õÉÙ£¬1Öµ±íÊ¾Ôö¼Ó
	//Remark : Èç¹ûÎïÆ·ÊÇ½ðÇ®µÄ»°£¬ÔòKUiObjAtRegion::Obj::uId±íÊ¾°Ñ½ðÇ®¶îµ÷ÕûÎªÕâ¸öÖµ£¬ÇÒnParamÎÞÒâÒå¡£
	case GOI_TRADE_DESIRE_ITEM:
		if (uParam)
		{
			KUiObjAtRegion* pInfo = (KUiObjAtRegion*) uParam;
			if (pInfo->Obj.uGenre != CGOG_MONEY)
				break;
			Player[CLIENT_PLAYER_INDEX].TradeMoveMoney(pInfo->Obj.uId);
		}
		break;

	//ÓÐÎÞ½»Ò×ÒâÏò
	//nParam = bWilling
	case GOI_TRADE_WILLING:
		if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].GetMenuState() == PLAYER_MENU_STATE_TRADEOPEN)
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyClose();
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyOpen((char*)uParam, nParam);
		}
		break;

	//Ëø¶¨½»Ò×
	//nParam = (int)(book)bLock ÊÇ·ñËø¶¨
	case GOI_TRADE_LOCK:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock)
			Player[CLIENT_PLAYER_INDEX].TradeApplyLock(0);
		else
			Player[CLIENT_PLAYER_INDEX].TradeApplyLock(1);
		break;

	//½»Ò×
	case GOI_TRADE:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeLock != 1 || Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeDestLock != 1)
			break;
		if (Player[CLIENT_PLAYER_INDEX].m_cTrade.m_nTradeState == 0)
		{
			Player[CLIENT_PLAYER_INDEX].TradeDecision(1);		// ½»Ò×È·¶¨
		}
		else
		{
			Player[CLIENT_PLAYER_INDEX].TradeDecision(2);		// ½»Ò×È·¶¨È¡Ïû
		}
		break;

	//½»Ò×È¡Ïû
	case GOI_TRADE_CANCEL:
		if ( !Player[CLIENT_PLAYER_INDEX].CheckTrading() )
			break;
		Player[CLIENT_PLAYER_INDEX].TradeDecision(0);		// ½»Ò×È¡Ïû
		break;

	//²éÑ¯ÊÇ·ñ¿ÉÒÔ¶ªÄ³¸ö¶«Î÷µ½ÓÎÏ·´°¿Ú
	//uParam = (KUiGameObject*)pObject -> ÎïÆ·ÐÅÏ¢
	//nParam = ±»ÍÏ¶¯¶«Î÷µÄµ±Ç°×ø±ê£¨¾ø¶Ô×ø±ê£©£¬ºá×ø±êÔÚµÍ16Î»£¬×Ý×ø±êÔÚ¸ß16Î»¡£(ÏñËØµã×ø±ê)
	//Return = ÊÇ·ñ¿ÉÒÔ·ÅÏÂ
	case GOI_DROP_ITEM_QUERY:
		//to do : waiting for...
		break;

	case GOI_SEND_MSG:
		if (uParam && nParam)
			Player[CLIENT_PLAYER_INDEX].SendChat((KUiMsgParam*)nParam, (char*)uParam);
		break;
	case GOI_SET_SEND_CHAT_CHANNEL:	//ÉèÖÃµ±Ç°·¢ËÍÏûÏ¢µÄÆµµÀ
		if (uParam)
		{
			KUiChatChannel* pChannelInfo = (KUiChatChannel*)uParam;// pChannelInfo ÒªÉèÖÃµÄÆµµÀµÄÐÅÏ¢
			Player[CLIENT_PLAYER_INDEX].m_cChat.SetCurChannel(pChannelInfo->nChannelNo, pChannelInfo->uChannelId, pChannelInfo->nChannelIndex);
		}
		break;
	// ÉèÖÃµ±Ç°·¢ËÍÏûÏ¢µÄÆµµÀÎªÓëÄ³ÈËµ¥ÁÄ
	//uParam = (KUiPlayerItem*) pFriend
	case GOI_SET_SEND_WHISPER_CHANNEL:
		if (uParam)
		{
			if (((KUiPlayerItem*)uParam)->nIndex)
			{
				int	nIdx = NpcSet.SearchName(((KUiPlayerItem*)uParam)->Name);
				if (nIdx > 0)
				{
					Player[CLIENT_PLAYER_INDEX].m_cChat.SetCurChannel(CHAT_CUR_CHANNEL_SCREENSINGLE, Npc[nIdx].m_dwID, 0);
				}
				else
				{
					KSystemMessage	sMsg;
					sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_NOT_NEAR, ((KUiPlayerItem*)uParam)->Name);
					sMsg.eType = SMT_NORMAL;
					sMsg.byConfirmType = SMCT_NONE;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				}
			}
			else
			{
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_NOT_ONLINE, ((KUiPlayerItem*)uParam)->Name);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			}
		}
		break;

	case GOI_SET_RECEIVE_CHAT_CHANNEL://ÉèÖÃ¶©ÔÄ/È¡ÏûÏûÏ¢ÆµµÀ
		if (uParam)
		{
			//uParam = (KUiChatChannel*) pChannelInfo ÒªÉèÖÃµÄÆµµÀµÄÐÅÏ¢
			//nParam = (int)(bool) bEnable ÊÇ·ñ¶©ÔÄ
			KUiChatChannel* pChannelInfo = (KUiChatChannel*)uParam;// pChannelInfo ÒªÉèÖÃµÄÆµµÀµÄÐÅÏ¢
			if (nParam)
			{
				Player[CLIENT_PLAYER_INDEX].m_cChat.AddChannel(pChannelInfo->nChannelNo);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cChat.SubChannel(pChannelInfo->nChannelNo);
			}
		}
		break;

	//ÐÂ½¨ÁÄÌìºÃÓÑ×é
	//uParam = (const char*) pGroupName
	//Return = (int)(bool) bSuccessed
	case GOI_CHAT_GROUP_NEW:
		Player[CLIENT_PLAYER_INDEX].m_cChat.AddTeam(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name, (char*)uParam);
		CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
		break;

	//ÖØÃüÃûÁÄÌìºÃÓÑ×é
	//uParam = (const char*) pGroupName
	//nParam = nIndex ×éË÷Òý
	//Return = (int)(bool) bSuccessed
	case GOI_CHAT_GROUP_RENAME:
		Player[CLIENT_PLAYER_INDEX].m_cChat.RenameTeam(nParam, (char*)uParam, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
		CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
		break;

	//É¾³ýÁÄÌìºÃÓÑ×é
	//nParam = nIndex ×éË÷Òý
	//Return = (int)(bool) bSuccessed
	case GOI_CHAT_GROUP_DELETE:
		Player[CLIENT_PLAYER_INDEX].m_cChat.DeleteTeam(nParam, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
		CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
		break;

	//ÐÂÌí¼ÓÁÄÌìºÃÓÑ
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0
	case GOI_CHAT_FRIEND_ADD:
		{
			if (g_pClient)
			{
				size_t pckgsize = sizeof(tagExtendProtoHeader) + sizeof(ASK_ADDFRIEND_CMD);

				tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
				pExHeader->ProtocolType = c2s_extendfriend;
				pExHeader->wLength = pckgsize - 1;

				ASK_ADDFRIEND_CMD* pAafCmd = (ASK_ADDFRIEND_CMD*)(pExHeader + 1);
				pAafCmd->ProtocolFamily = pf_friend;
				pAafCmd->ProtocolID = friend_c2c_askaddfriend;
				pAafCmd->pckgid = -1;
				strncpy(pAafCmd->dstrole, ((KUiPlayerItem*)uParam)->Name, _NAME_LEN);
				g_pClient->SendPackToServer(pExHeader, pckgsize);
		
				//Player[CLIENT_PLAYER_INDEX].m_cChat.ApplyAddFriend(((KUiPlayerItem*)uParam)->uId);

				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_APPLY_ADD_FRIEND, ((KUiPlayerItem*)uParam)->Name);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			}
		}
		break;
	//É¾³ýÁÄÌìºÃÓÑ
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0
	//nParam = nGroupIndex ºÃÓÑËùÔÚµÄ×éµÄË÷Òý
	//Return = (int)(bool) bSuccessed
	case GOI_CHAT_FRIEND_DELETE:
		{
			Player[CLIENT_PLAYER_INDEX].m_cChat.ApplyDeleteFriend(((KUiPlayerItem*)uParam)->uId, nParam, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_CHAT_DELETE_FRIEND, ((KUiPlayerItem*)uParam)->Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;

	//ºÃÓÑÒÆµ½ÐÂ×é
	//uParam = (KUiPlayerItem*)pFriend
	//			KUiPlayerItem::nData = 0
	//nParam = nGroupIndex ºÃÓÑËùµ½µÄÐÂ×éµÄË÷Òý
	case GOI_CHAT_FRIEND_MOVE:
		{
			KUiPlayerItem	*pPlayer = (KUiPlayerItem*)uParam;
			int		nSrcTeamNo = Player[CLIENT_PLAYER_INDEX].m_cChat.GetTeamNo(pPlayer->uId);
			if (nSrcTeamNo == -1)
			{
				// Ê§°Ü£¬Èç¹ûÐèÒª·¢ÏûÏ¢£¬ÔÚ´Ë
				break;
			}
			if (Player[CLIENT_PLAYER_INDEX].m_cChat.OneFriendChangeTeam(pPlayer->uId, nSrcTeamNo, nParam))
			{	// ³É¹¦
				CoreDataChanged(GDCNI_CHAT_GROUP, 0, 0);
			}
			else
			{
				// Ê§°Ü£¬Èç¹ûÐèÒª·¢ÏûÏ¢£¬ÔÚ´Ë
			}
		}
		break;

	//·¢¸øºÃÓÑÒ»ÌõÑ¶Ï¢
	//uParam = (KUiChatMessage*)pMessage ÏûÏ¢Êý¾Ý
	//nParam = (KUiPlayerItem*)pFriend   ·¢À´Ñ¶Ï¢µÄºÃÓÑ
	case GOI_CHAT_WORDS_TO_FRIEND:
		{
			char	szSentence[MAX_SENTENCE_LENGTH];
			KUiChatMessage	*pMsg = (KUiChatMessage*)uParam;
			KUiMsgParam	sMsg;

			sMsg.eGenre = MSG_G_CHAT;
			sMsg.szName[0] = 0;
			memset(sMsg.cChatPrefix, 0, sizeof(sMsg.cChatPrefix));
			sMsg.cChatPrefixLen = sizeof(unsigned int);
			sMsg.cChatPrefix[0] = ((char*)(&pMsg->uColor))[3];
			sMsg.cChatPrefix[1] = ((char*)(&pMsg->uColor))[2];
			sMsg.cChatPrefix[2] = ((char*)(&pMsg->uColor))[1];
			sMsg.cChatPrefix[3] = ((char*)(&pMsg->uColor))[0];

			if (pMsg->nContentLen >= MAX_SENTENCE_LENGTH)
			{
				memcpy(szSentence, pMsg->szContent, MAX_SENTENCE_LENGTH - 1);
				szSentence[MAX_SENTENCE_LENGTH - 1] = 0;
				sMsg.nMsgLength = MAX_SENTENCE_LENGTH - 1;
			}
			else
			{
				memcpy(szSentence, pMsg->szContent, pMsg->nContentLen);
				szSentence[pMsg->nContentLen] = 0;
				sMsg.nMsgLength = pMsg->nContentLen;
			}

			Player[CLIENT_PLAYER_INDEX].m_cChat.QQSendSentence(
				((KUiPlayerItem*)nParam)->uId,
				((KUiPlayerItem*)nParam)->nIndex, &sMsg, szSentence);
		}
		break;

	//¶Ô±ðÈËÒª¼Ó×Ô¼ºÎªºÃÓÑµÄ»Ø¸´
	//uParam = (KUiPlayerItem*)pRequestPlayer ·¢³öÇëÇóµÄÍæ¼Ò
	//nParam = (int)(bool)bAccept ÊÇ·ñ½ÓÊÜÇëÇó
	case GOI_CHAT_FRIEND_INVITE:
		if (uParam)
		{
			if (g_pClient)
			{
				size_t pckgsize = sizeof(tagExtendProtoHeader) + sizeof(REP_ADDFRIEND_CMD);

				tagExtendProtoHeader* pExHeader = (tagExtendProtoHeader*)_alloca(pckgsize);
				pExHeader->ProtocolType = c2s_extendfriend;
				pExHeader->wLength = pckgsize - 1;
				
				REP_ADDFRIEND_CMD* pRafCmd = (REP_ADDFRIEND_CMD*)(pExHeader + 1);
				pRafCmd->ProtocolFamily = pf_friend;
				pRafCmd->ProtocolID = friend_c2c_repaddfriend;
				pRafCmd->pckgid = -1;
				strncpy(pRafCmd->dstrole, ((KUiPlayerItem*)uParam)->Name, _NAME_LEN);
				pRafCmd->answer = nParam ? answerAgree : answerDisagree;
				g_pClient->SendPackToServer(pExHeader, pckgsize);

			//if (nParam)
			//{
			//	Player[CLIENT_PLAYER_INDEX].ChatAddFriend(((KUiPlayerItem*)uParam)->nIndex);
			//}
			//else
			//{
			//	Player[CLIENT_PLAYER_INDEX].ChatRefuseFriend(((KUiPlayerItem*)uParam)->nIndex);
			//}
			}
		}
		break;

	case GOI_OPTION_SETTING:			//Ñ¡ÏîÉèÖÃ
		if (uParam == OPTION_DYNALIGHT)
		{
			g_ScenePlace.EnableDynamicLights(nParam != 0);
			if (g_pRepresent)
				g_pRepresent->SetOption(DYNAMICLIGHT, nParam != 0);
		}
		else if (uParam == OPTION_PERSPECTIVE)
		{
			if (g_pRepresent)
				g_pRepresent->SetOption(PERSPECTIVE, nParam != 0);
		}
		else if (uParam == OPTION_MUSIC_VALUE)
			Option.SetMusicVolume(nParam);
		else if (uParam == OPTION_SOUND_VALUE)
			Option.SetSndVolume(nParam);
		else if (uParam == OPTION_BRIGHTNESS)
			Option.SetGamma(nParam);
		else if (uParam == OPTION_WEATHER)
			g_ScenePlace.EnableWeather(nParam);
		break;

	case GOI_VIEW_PLAYERITEM:
		{
			g_cViewItem.ApplyViewEquip(uParam);
//			if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode == 0)
//				g_cViewItem.ApplyViewEquip(uParam);
//			else
//			{
//				KSystemMessage	sMsg;
//				sprintf(sMsg.szMessage, MSG_CAN_NOT_VIEW_ITEM);
//				sMsg.eType = SMT_NORMAL;
//				sMsg.byConfirmType = SMCT_NONE;
//				sMsg.byPriority = 0;
//				sMsg.byParamSize = 0;
//				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
//			}
		}
		break;
	case GOI_VIEW_PLAYERITEM_END:
		g_cViewItem.DeleteAll();
		break;

	case GOI_PLAYER_SYSSHOP:
		{
			SendClientCmdSysShop();
		}
		break;

	case GOI_PLAYER_ACTION:
		{
			switch(uParam)
			{
			case PA_RUN:
				Player[CLIENT_PLAYER_INDEX].m_RunStatus = !Player[CLIENT_PLAYER_INDEX].m_RunStatus;
				break;
			case PA_SIT:
				if (!Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)
				{
					if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing != do_sit)
					{
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_sit);
						SendClientCmdSit(TRUE);
					}
					else
					{
						Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
						SendClientCmdSit(FALSE);
					}
				}
				else
				{
					KSystemMessage	Msg;
					Msg.byConfirmType = SMCT_CLICK;
					Msg.eType = SMT_PLAYER;
					Msg.byPriority = 1;
					Msg.byParamSize = 0;
					strcpy(Msg.szMessage, "§ang c­ìi ngùa kh«ng thÓ thiÒn täa!");
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
				}
				break;
			case PA_RIDE:
				if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Doing == do_sit)
				{
					Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].SendCommand(do_stand);
					SendClientCmdSit(FALSE);
				}			
				
				SendClientCmdRide();
				
				break;
			case PW_NOT_SAME: // mat khau ko trung nhau
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "MËt khÈu míi nhËp vµo kh«ng gièng nhau");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case PW_NOT_LONG: // mat khau ko du do dai
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "MËt khÈu ph¶i dµi ®ñ 6 ch÷ sè");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case PW_ACCEPTED: // mat khau dc chap nhan
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "MËt khÈu nhËp vµo ®­îc chÊp nhËn");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case HT_CN:
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "<color=yellow>Chøc n¨ng nµy sÏ ®­îc ra m¾t trong thêi gian tíi !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case KD_VP:
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "Kh«ng ®ñ vËt phÈm kh«ng thÓ tiÕn hµnh!");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case CN_GH:
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "<color=yellow>Thao t¸c nµy cÇn më khãa nh©n vËt !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case MO_AUTO:
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "KÝch ho¹t chÕ ®é tù ®éng ®¸nh !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case TAT_AUTO:
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "T¾t chÕ ®é tù ®éng ®¸nh !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case ERORR_AUTO:
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "Th«ng sè c¸i ®Æt auto lçi, kh«ng thÓ b¾t ®Çu !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
				break;
			case EX_BOX:
				{	
					int nExbox = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ExBoxId;
					if(nExbox == 0)
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "<color=cyan>CÇn thªm r­¬ng ®å h·y t×m ThÈm Cöu thö xem !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
					else 
						CoreDataChanged(GDCNI_OPEN_EX_BOX, NULL, NULL);
					
					
				}
				break;
			case EX_BOX2:
				{	
					
					int nExbox = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ExBoxId;
					if(nExbox == 1)
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "<color=cyan>CÇn thªm r­¬ng ®å h·y t×m ThÈm Cöu thö xem !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
					else if(nExbox == 1 || nExbox == 2 || nExbox == 3)
							CoreDataChanged(GDCNI_OPEN_EX_BOX2, NULL, NULL);
					
					
				}
				break;
			case EX_BOX3:
				{	
					
					int nExbox = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ExBoxId;
					if(nExbox == 2 || nExbox == 1)
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "<color=cyan>CÇn thªm r­¬ng ®å h·y t×m ThÈm Cöu thö xem !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
					else if(nExbox == 1 || nExbox == 2 || nExbox == 3)
							CoreDataChanged(GDCNI_OPEN_EX_BOX3, NULL, NULL);
					
					
				}
				break;
			case ITEMEX:
				{	
					
					int nItemEX = Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_ExItemId;
					if(nItemEX == 0)
					{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "<color=cyan>CÇn hµnh trang h·y t×m ThÈm Cöu thö xem !");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
					}
					else if(nItemEX >= 1)
							CoreDataChanged(GDCNI_OPEN_ITEMEX, NULL, NULL);
				}
				break;
			case OPEN_MARKET:
				if(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_FightMode == 0)
				{
						CoreDataChanged(GDCNI_OPEN_MARKET, NULL, TRUE);
				}
				else
				{
						KSystemMessage	Msg;
						Msg.byConfirmType = SMCT_NONE;
						Msg.eType = SMT_NORMAL;
						Msg.byPriority = 1;
						Msg.byParamSize = 0;
						strcpy(Msg.szMessage, "Kh«ng thÓ më kú tr©n c¸c ë khu vùc chiÕn ®Êu.");
						CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, 0);
				}
				break;
			}
		}
		break;

	case GOI_PK_SETTING:
					Player[CLIENT_PLAYER_INDEX].m_cPK.ApplySetNormalPKState(nParam);
				break;
	//uParam = (KUiPlayerItem*) pTarget
	case GOI_REVENGE_SOMEONE:
		if (uParam)
		{
			KUiPlayerItem	*pTarget = (KUiPlayerItem*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cPK.ApplyEnmityPK(pTarget->uId);
		}
		break;
	//¸úËæÄ³ÈË
	//uParam = (KUiPlayerItem*) pTarget	¸úËæÄ¿±ê
	case GOI_FOLLOW_SOMEONE:
		if (uParam)
		{
			KUiPlayerItem	*pTarget = (KUiPlayerItem*)uParam;
			if (Npc[pTarget->nIndex].m_Kind == kind_player)
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_nPeopleIdx = pTarget->nIndex;
		}
		break;

	//ÏÔÊ¾¸÷Íæ¼ÒÈËÃû
	//nParam = (int)(bool)bShow	ÊÇ·ñÏÔÊ¾
	case GOI_SHOW_PLAYERS_NAME:
		NpcSet.SetShowNameFlag(nParam);
		break;
	//ÏÔÊ¾¸÷Íæ¼ÒÉúÃü
	//nParam = (int)(bool)bShow	ÊÇ·ñÏÔÊ¾
	case GOI_SHOW_PLAYERS_LIFE:
		NpcSet.SetShowLifeFlag(nParam);
		break;
	//ÏÔÊ¾¸÷Íæ¼ÒÄÚÁ¦
	//nParam = (int)(bool)bShow	ÊÇ·ñÏÔÊ¾
	case GOI_SHOW_PLAYERS_MANA:
		NpcSet.SetShowManaFlag(nParam);
		break;
	default:
		nRet = 0;
		break;
	}

	return nRet;
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º·¢ËÍÊäÈëÉè±¸µÄÊäÈë²Ù×÷ÏûÏ¢
//--------------------------------------------------------------------------
void KCoreShell::ProcessInput(unsigned int uMsg, unsigned int uParam, int nParam)
{
	Player[CLIENT_PLAYER_INDEX].ProcessInputMsg(uMsg, uParam, nParam);
}

int KCoreShell::FindSelectNPC(int x, int y, int nRelation, bool bSelect, void* pReturn, int& nKind)
{
	Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, nRelation);
	int nT = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();

	if (!bSelect)
		Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);

	if (nT > 0)
	{
		if (pReturn)
		{
			KUiPlayerItem* p = (KUiPlayerItem*)pReturn;
			strncpy(p->Name, Npc[nT].Name, 32);
			p->nIndex = Npc[nT].m_Index;
			p->uId = Npc[nT].m_dwID;
			p->nData = Npc[nT].GetMenuState();
		}
		nKind = Npc[nT].m_Kind;
		return true;
	}
	return false;
}

int KCoreShell::FindSelectObject(int x, int y, bool bSelect, int& nObjectIdx, int& nKind)
{
	Player[CLIENT_PLAYER_INDEX].FindSelectObject(x, y);
	int nT = Player[CLIENT_PLAYER_INDEX].GetTargetObj();
	
	if (!bSelect)
		Player[CLIENT_PLAYER_INDEX].SetTargetObj(0);

	if (nT > 0)
	{
		nObjectIdx = nT;
		nKind = Object[nT].m_nKind;
		return true;
	}
	return false;
}


int KCoreShell::FindSpecialNPC(char* Name, void* pReturn, int& nKind)
{
	if (Name == NULL || Name[0] == 0)
		return false;
	for (int nT = 0; nT < MAX_NPC; nT++)
	{
		if	(strcmp(Npc[nT].Name, Name) == 0)
		{
			if (pReturn)
			{
				KUiPlayerItem* p = (KUiPlayerItem*)pReturn;
				strncpy(p->Name, Npc[nT].Name, 32);
				p->nIndex = Npc[nT].m_Index;
				p->uId = Npc[nT].m_dwID;
				p->nData = Npc[nT].GetMenuState();
			}
			nKind = Npc[nT].m_Kind;
			return true;
		}
	}
	return false;
}

int KCoreShell::ChatSpecialPlayer(void* pPlayer, const char* pMsgBuff, unsigned short nMsgLength)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC)
		{
			int nTalker = p->nIndex;
			if (Npc[nTalker].m_Kind == kind_player &&
				Npc[nTalker].m_dwID == p->uId)
			{
				Npc[nTalker].SetChatInfo(p->Name, pMsgBuff, nMsgLength);
				return true;
			}
		}
	}

	return false;
}

void KCoreShell::ApplyAddTeam(void* pPlayer)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC && !Player[CLIENT_PLAYER_INDEX].CheckTrading())
		{
			Player[CLIENT_PLAYER_INDEX].ApplyAddTeam(p->nIndex);
		}
	}
}

void KCoreShell::TradeApplyStart(void* pPlayer)
{
	KUiPlayerItem* p = (KUiPlayerItem*)pPlayer;
	if (p)
	{
		if (p->nIndex >= 0 && p->nIndex < MAX_NPC && !Player[CLIENT_PLAYER_INDEX].CheckTrading())
		{
			Player[CLIENT_PLAYER_INDEX].TradeApplyStart(p->nIndex);
		}
	}
}

int KCoreShell::UseSkill(int x, int y, int nSkillID)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;

	int nX = x;
	int nY = y;
	int nZ = 0;
	g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (Npc[nIndex].IsCanInput())
	{
		int nIdx = 0;
		
		nIdx = Npc[nIndex].m_SkillList.FindSame(nSkillID);
		g_DebugLog("[skill]Active");
		Npc[nIndex].SetActiveSkill(nIdx);
	}
	else
	{
		g_DebugLog("[skill]return");
		return 0;
	}

	if (Npc[nIndex].m_ActiveSkillID > 0)
	{
		ISkill * pISkill =  g_SkillManager.GetSkill(Npc[nIndex].m_ActiveSkillID, 1);
		if (!pISkill) 
            return 0;

		if (pISkill->IsAura())
			return 0;

		int nAttackRange = pISkill->GetAttackRadius();

		int nTargetIdx = 0;
		//°´ÕÕObject / Enemy / Ally µÄÓÅÏÈ¼¶ÕÒµ½ÐèÒª´òµÄ¶ÔÏóid
		
		if (pISkill->IsTargetAlly())
		{
			Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, relation_ally);
			if (Player[CLIENT_PLAYER_INDEX].GetTargetNpc())
			{
				nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();
			}
		}

		if (pISkill->IsTargetEnemy())
		{
			Player[CLIENT_PLAYER_INDEX].FindSelectNpc(x, y, relation_enemy);
			if (Player[CLIENT_PLAYER_INDEX].GetTargetNpc())
			{
				nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetNpc();
			}
		}

		if (pISkill->IsTargetObj())
		{
			Player[CLIENT_PLAYER_INDEX].FindSelectObject(x, y);
			if (Player[CLIENT_PLAYER_INDEX].GetTargetObj())
			{
				nTargetIdx = Player[CLIENT_PLAYER_INDEX].GetTargetObj();
			}
		}
		//Èç¹û¼¼ÄÜ±ØÐëÖ¸¶¨¶ÔÏó£¬¶øµ±Ç°Î»ÖÃÎÞ¶ÔÏóµÄ»°£¬Ö±½ÓÍË³ö
		if (pISkill->IsTargetOnly() && !nTargetIdx)
        {
			Npc[nIndex].m_nPeopleIdx = 0;
			Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
			return 0;
		}
		
		if (nIndex == nTargetIdx)
		{
			Npc[nIndex].m_nPeopleIdx = 0;
			Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
			return 0;
		}

		if ((!Npc[nIndex].m_SkillList.CanCast(Npc[nIndex].m_ActiveSkillID, SubWorld[Npc[nIndex].m_SubWorldIndex].m_dwCurrentTime))
			||
			(!Npc[nIndex].Cost(pISkill->GetSkillCostType(), pISkill->GetSkillCost(&Npc[nIndex]), TRUE))
			)
		{
			Npc[nIndex].m_nPeopleIdx = 0;
			Player[CLIENT_PLAYER_INDEX].SetTargetNpc(0);
			return 0;
		}

		//ÎÞ¶ÔÏó£¬Ö±½Ó·¢×ø±ê
		if (!nTargetIdx)
		{
			Npc[nIndex].SendCommand(do_skill, Npc[nIndex].m_ActiveSkillID, nX, nY);
			// Send to Server		
			SendClientCmdSkill(Npc[nIndex].m_ActiveSkillID, nX, nY);
		}
		else
		{
			if (pISkill->IsTargetOnly())
			{
				int distance = NpcSet.GetDistance(nIndex , nTargetIdx);
				if (distance > pISkill->GetAttackRadius())
				{
					Player[CLIENT_PLAYER_INDEX].SetTargetNpc(nTargetIdx);
					return 0;
				}
			}
			// 
			if (nIndex == nTargetIdx && pISkill->GetSkillStyle() == SKILL_SS_Missles) 
				return 0;
			Npc[nIndex].SendCommand(do_skill, Npc[nIndex].m_ActiveSkillID, -1, nTargetIdx);
			// Send to Server		
			SendClientCmdSkill(Npc[nIndex].m_ActiveSkillID, -1, Npc[nTargetIdx].m_dwID);
		}
	}
	Npc[nIndex].m_nPeopleIdx = 0;
	return 1;
}

int KCoreShell::LockSomeoneUseSkill(int nTargetIndex, int nSkillID)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;
	
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (nTargetIndex == nIndex)
		return 0;

	if (Npc[nIndex].IsCanInput())
	{
		int nIdx = 0;
		
		nIdx = Npc[nIndex].m_SkillList.FindSame(nSkillID);
		g_DebugLog("[skill]Active");

		Npc[nIndex].SetActiveSkill(nIdx);
	}
	else
	{
		g_DebugLog("[skill]return");
		return 0;
	}

	int nRelation = NpcSet.GetRelation(nIndex, nTargetIndex);
	if (nRelation == relation_enemy)
	{
		Npc[nIndex].m_nPeopleIdx = nTargetIndex;
		return 1;
	}

	return 0;
}

int KCoreShell::LockSomeoneAction(int nTargetIndex)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;
	
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (nTargetIndex == nIndex)
		return 0;
	if (nTargetIndex <= 0 || nTargetIndex >= MAX_NPC)	//È¡ÏûLock
	{
		Npc[nIndex].m_nPeopleIdx = 0;
		return 1;
	}

	int nRelation = NpcSet.GetRelation(nIndex, nTargetIndex);
	if (nRelation != relation_enemy)
	{
		Npc[nIndex].m_nPeopleIdx = nTargetIndex;
		return 1;
	}

	return 0;
}

int KCoreShell::LockObjectAction(int nTargetIndex)
{
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
		return 0;
	
	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	if (nTargetIndex <= 0)	//È¡ÏûLock
		Npc[nIndex].m_nObjectIdx = 0;
	else
		Npc[nIndex].m_nObjectIdx = nTargetIndex;

	return 1;
}

void KCoreShell::GotoWhere(int x, int y, int mode)
{
	if (mode < 0 || mode > 2)
		return;

	if (Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
	{
		int bRun = false;

		if ((mode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) ||
			mode == 2)
			bRun = true;

		int nX = x;
		int nY = y;
		int nZ = 0;
		g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
		int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

		if (!bRun)
		{
			Npc[nIndex].SendCommand(do_walk, nX, nY);
			// Send to Server
			SendClientCmdWalk(nX, nY);
		}
		else
		{
			Npc[nIndex].SendCommand(do_run, nX, nY);
			// Send to Server
			SendClientCmdRun(nX, nY);
		}
		Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
	}
}

void KCoreShell::Goto(int nDir, int mode)
{
	if (nDir < 0 || nDir > 63)
		return;

	if (mode < 0 || mode > 2)
		return;

	int bRun = false;

	if ((mode == 0 && Player[CLIENT_PLAYER_INDEX].m_RunStatus) ||
		mode == 2)
		bRun = true;

	int nIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;

	int nSpeed;
	if (bRun)
		nSpeed = Npc[nIndex].m_CurrentRunSpeed;
	else
		nSpeed = Npc[nIndex].m_CurrentWalkSpeed;

	Player[CLIENT_PLAYER_INDEX].Walk(nDir, nSpeed);

	Player[CLIENT_PLAYER_INDEX].m_nSendMoveFrames = 0;
}

void KCoreShell::Turn(int nDir)
{
	if (nDir < 0 || nDir > 3)
		return;

	if (nDir == 0)
		Player[CLIENT_PLAYER_INDEX].TurnLeft();
	else if (nDir == 1)
		Player[CLIENT_PLAYER_INDEX].TurnRight();
	else
		Player[CLIENT_PLAYER_INDEX].TurnBack();
}

int KCoreShell::ThrowAwayItem()
{
	return Player[CLIENT_PLAYER_INDEX].ThrowAwayItem();
}

int KCoreShell::GetNPCRelation(int nIndex)
{
	return NpcSet.GetRelation(Player[CLIENT_PLAYER_INDEX].m_nIndex, nIndex);
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»æÖÆÓÎÏ·¶ÔÏó
//--------------------------------------------------------------------------
void KCoreShell::DrawGameObj(unsigned int uObjGenre, unsigned int uId, int x, int y, int Width, int Height, int nParam)
{
	if (g_pRepresent)
		CoreDrawGameObj(uObjGenre, uId, x, y, Width, Height, nParam);
}

#include "../../Represent/iRepresent/iRepresentshell.h"

//--------------------------------------------------------------------------
//	¹¦ÄÜ£º»æÖÆÓÎÏ·ÊÀ½ç
//--------------------------------------------------------------------------
void KCoreShell::DrawGameSpace()
{
	if (g_pRepresent)
	{
		g_ScenePlace.Paint();
		Player[CLIENT_PLAYER_INDEX].DrawSelectInfo();
	}
}

//--------------------------------------------------------------------------
//	¹¦ÄÜ£ºÉèÖÃ»æÍ¼½Ó¿ÚÊµÀýµÄÖ¸Õë
//--------------------------------------------------------------------------
void KCoreShell::SetRepresentShell(struct iRepresentShell* pRepresent)
{
	g_pRepresent = pRepresent;
	g_ScenePlace.RepresentShellReset();
	if (g_pAdjustColorTab && g_ulAdjustColorCount && g_pRepresent)
		g_pRepresent->SetAdjustColorList(g_pAdjustColorTab, g_ulAdjustColorCount);
}

void KCoreShell::SetMusicInterface(void* pMusicInterface)
{
	g_pMusic = (KMusic*)pMusicInterface;
	Option.SetMusicVolume(Option.GetMusicVolume());
}

//ÈÕ³£»î¶¯£¬coreÈç¹ûÒªÊÙÖÕÕýÇÞÔò·µ»Ø0£¬·ñÔò·µ»Ø·Ç0Öµ
int KCoreShell::Breathe()
{
	g_SubWorldSet.MessageLoop();
	g_SubWorldSet.MainLoop();
	g_ScenePlace.Breathe();
	return true;
}

int KCoreShell::GetProtocolSize(BYTE byProtocol)
{
	if (byProtocol <= s2c_clientbegin || byProtocol >= s2c_end)
		return -1;
	return g_nProtocolSize[byProtocol - s2c_clientbegin - 1];
}

#ifdef SWORDONLINE_SHOW_DBUG_INFO
extern int		g_bShowObstacle;
extern bool		g_bShowGameInfo;	//ÊÇ·ñÏÔÊ¾ÓÎÏ·£¨³¡¾°£©ÐÅÏ¢
#endif
int KCoreShell::Debug(unsigned int uDataId, unsigned int uParam, int nParam)
{
#ifdef SWORDONLINE_SHOW_DBUG_INFO
	switch(uDataId)
	{
	case DEBUG_SHOWINFO:
		Player[CLIENT_PLAYER_INDEX].m_DebugMode = !Player[CLIENT_PLAYER_INDEX].m_DebugMode;
		g_bShowGameInfo = !g_bShowGameInfo;
		break;
	case DEBUG_SHOWOBSTACLE:
		g_bShowObstacle = !g_bShowObstacle;
		break;
	}
#endif
	return 0;
}

DWORD KCoreShell::GetPing()
{
	return g_SubWorldSet.GetPing();
}

//void KCoreShell::SendPing()
//{
//	SendClientCmdPing();
//}

void KCoreShell::SetRepresentAreaSize(int nWidth, int nHeight)
{
	g_ScenePlace.SetRepresentAreaSize(nWidth, nHeight);
}

void KCoreShell::SetClient(LPVOID pClient)
{
	g_SetClient(pClient);
}

void KCoreShell::SendNewDataToServer(void* pData, int nLength)
{
	if (g_pClient)
		g_pClient->SendPackToServer(pData, nLength);
}

//ÓëµØÍ¼Ïà¹ØµÄ²Ù×÷
int	KCoreShell::SceneMapOperation(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uOper)
	{
	case GSMOI_SCENE_TIME_INFO:
		if (uParam)
		{
			KUiSceneTimeInfo* pInfo = (KUiSceneTimeInfo*)uParam;
			g_ScenePlace.GetSceneNameAndFocus(pInfo->szSceneName, pInfo->nSceneId,
				pInfo->nScenePos0, pInfo->nScenePos1);
			pInfo->nGameSpaceTime = (SubWorld[0].m_dwCurrentTime / 100) % 1440;
		}
		break;
	case GSMOI_SCENE_MAP_INFO:
		nRet = g_ScenePlace.GetMapInfo((KSceneMapInfo*)uParam);
		break;
	case GSMOI_IS_SCENE_MAP_SHOWING:
		g_ScenePlace.SetMapParam(uParam, nParam);
		break;
	case GSMOI_PAINT_SCENE_MAP:
		g_ScenePlace.PaintMap(uParam, nParam);
		break;
	case GSMOI_SCENE_MAP_FOCUS_OFFSET:
		g_ScenePlace.SetMapFocusPositionOffset((int)uParam, nParam);
		break;
	case GSMOI_SCENE_FOLLOW_WITH_MAP:	//ÉèÖÃ³¡¾°ÊÇ·ñËæ×ÅµØÍ¼µÄÒÆ¶¯¶øÒÆ¶¯
		g_ScenePlace.FollowMapMove(nParam);
		break;
	}
	return nRet;
}

//Óë°ï»áÏà¹ØµÄ²Ù×÷, uOperµÄÈ¡ÖµÀ´×Ô GAME_TONG_OPERATION_INDEX
int	KCoreShell::TongOperation(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uOper)
	{
	case GTOI_TONG_CREATE:		//´´½¨°ï»á
		//uParam = (const char*) pszTongName °ï»áµÄÃû×Ö
		//nParam = (NPCCAMP)enFaction °ï»áÕóÓª
		Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyCreateTong(nParam, (char *)uParam);
		break;
	case GTOI_TONG_IS_RECRUIT:		//²éÑ¯Ä³ÈËµÄÕÐÈË¿ª¹Ø
		//uParam = (KUiPlayerItem*) Òª²éË­
		//Return = (int)(bool)		ÊÇ·ñ¿ª×ÅµÄÕÐÈË¿ª¹Ø
		if (uParam)
		{
			KUiPlayerItem	*pItem = (KUiPlayerItem*)uParam;
			nRet = Npc[pItem->nIndex].m_nTongFlag;
		}
		break;
	case GTOI_TONG_RECRUIT:     //ÕÐÈË¿ª¹Ø
		//uParam = (int)(bool)bRecruit ÊÇ·ñÔ¸ÒâÕÐÈË
		break;
	case GTOI_TONG_ACTION:         //¶Ô°ïÄÚ³ÉÔ±×öµÄ¶¯×÷£¬»ò×Ô¼ºÓë°ï»áµÄ¹ØÏµµÄ¸Ä±ä
		//uParam = (KTongOperationParam*) pOperParam ¶¯×÷Ê±µÄ²ÎÊý
		//nParam = (KTongMemberItem*) pMember Ö¸³öÁË²Ù×÷£¨°ï»á³ÉÔ±£©¶ÔÏó£¬
		{
    		KTongOperationParam *Oper = (KTongOperationParam *)uParam;
		    KTongMemberItem *TargetInfo = (KTongMemberItem *)nParam;
		    KUiPlayerItem TargetPlayer;
		    int nKind;

		    switch(Oper->eOper)
		    {
		    case TONG_ACTION_DISMISS:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyKick(Oper->nData[0], Oper->nData[1], TargetInfo->Name);
    			break;
		    case TONG_ACTION_ASSIGN:
    			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInstate(Oper->nData[0], Oper->nData[1], Oper->nData[2], Oper->nData[3], TargetInfo->Name);
			    break;
		    case TONG_ACTION_DEMISE:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyChangeMaster(Oper->nData[0], Oper->nData[1], TargetInfo->Name);
    			break;
		    case TONG_ACTION_LEAVE:
    			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyLeave();
    			break;
		    case TONG_ACTION_RECRUIT:
    			break;
		    case TONG_ACTION_APPLY:
				if(!FindSpecialNPC(TargetInfo->Name, &TargetPlayer, nKind))
				{
    				break;
				}
    			Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyAddTong(TargetPlayer.uId);
			    break;
		    }
		}
		break;

	//¶ÔÉêÇë¼ÓÈëµÄ´ð¸´
	//uPAram = (KUiPlayerItem *) pTarget   ÉêÇë·½
	//nParam : != 0  Í¬Òâ   ==0  ¾Ü¾ø
	case GTOI_TONG_JOIN_REPLY:
		if (uParam)
		{
			KUiPlayerItem	*pItem = (KUiPlayerItem*)uParam;
			Player[CLIENT_PLAYER_INDEX].m_cTong.AcceptMember(pItem->nIndex, g_FileName2Id(pItem->Name), nParam);
		}
		break;

	case GTOI_REQUEST_PLAYER_TONG:	//²éÑ¯Ä³ÈËÊÇÄÄ°ï»á
		//uParam = (KUiPlayerItem*) Òª²éË­
		//nParam = (int)(bool)bReturnTongDetail ÊÇ·ñÒª·µ»ØÄÇ¸ö°ï»áµÄÐÅÏ¢
		if (uParam)
		{
			KUiPlayerItem	*pItem = (KUiPlayerItem*)uParam;
			if (pItem->nIndex == Player[CLIENT_PLAYER_INDEX].m_nIndex)
			{
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					pItem->nIndex, 0, 0);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					pItem->nIndex, 0, 0);
			}
		}
		break;
	case GTOI_REQUEST_TONG_DATA:     //ÒªÇóÄ³¸ö°ï»áµÄ¸÷ÖÖ×ÊÁÏ
	//uParam = (KUiGameObjectWithName*)pTong Òª²éÑ¯µÄ°ï»á
	//KUiGameObjectWithName::szName °ï»áµÄÃû×Ö
	//KUiGameObjectWithName::nData ×ÊÁÏµÄÖÖÀà£¬ÖµÈ¡×ÔÃ¶¾ÙTONG_MEMBER_FIGURE
	//			ÁÐ±íµÄÖÖÀàÊÇenumTONG_FIGURE_MASTERµÄ»°´ú±íÒªÇóµÄÊÇ°ï»áµÄ×ÊÑ¶¡£
	//KUiGameObjectWithName::nParam ¿ªÊ¼µÄË÷Òý
		if (uParam)
		{
			if (Player[CLIENT_PLAYER_INDEX].m_cTong.CheckIn() == 0)
				break;

			KUiGameObjectWithName	*pObj = (KUiGameObjectWithName*)uParam;
			char	szTongName[32];
			DWORD	dwTongNameID;

			szTongName[0] = 0;
			Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongName(szTongName);
			if (!szTongName[0])
				break;

			// Ö»ÄÜ²éÑ¯×Ô¼º°ï»áÐÅÏ¢
			dwTongNameID = g_FileName2Id(pObj->szName);
			if (dwTongNameID != Player[CLIENT_PLAYER_INDEX].m_cTong.GetTongNameID())
				break;

			switch (pObj->nData)
			{
			case enumTONG_FIGURE_MASTER:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					Player[CLIENT_PLAYER_INDEX].m_nIndex, 0, 0);
				break;
			case enumTONG_FIGURE_DIRECTOR:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_TONG_HEAD,
					Player[CLIENT_PLAYER_INDEX].m_nIndex,  pObj->nParam, defTONG_ONE_PAGE_MAX_NUM);
				break;
			case enumTONG_FIGURE_MANAGER:
				if (!Player[CLIENT_PLAYER_INDEX].m_cTong.CanGetManagerInfo(dwTongNameID))
					break;
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_MANAGER,
					dwTongNameID, pObj->nParam, defTONG_ONE_PAGE_MAX_NUM);
				break;
			case enumTONG_FIGURE_MEMBER:
				if (!Player[CLIENT_PLAYER_INDEX].m_cTong.CanGetMemberInfo(dwTongNameID))
					break;
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyInfo(
					enumTONG_APPLY_INFO_ID_MEMBER,
					dwTongNameID, pObj->nParam, defTONG_ONE_PAGE_MAX_NUM);
				break;
			}
		}
		break;
	case GTOI_TONG_MONEY_ACTION:
		if (uParam)
		{
			KUiPlayerItem	*pItem = (KUiPlayerItem*)uParam;
			switch(nParam)
			{
			case TONG_ACTION_SAVE:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplySaveMoney(pItem->nData);
				break;
			case TONG_ACTION_GET:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplyGetMoney(pItem->nData);
				break;
			case TONG_ACTION_SND:
				Player[CLIENT_PLAYER_INDEX].m_cTong.ApplySndMoney(pItem->nData);
				break;
			}
		}
		break;
	}
	return nRet;
}

//Óë×é¶ÓÏà¹ØµÄ²Ù×÷£¬uOperµÄÈ¡ÖµÀ´×Ô GAME_TEAM_OPERATION_INDEX
int KCoreShell::TeamOperation(unsigned int uOper, unsigned int uParam, int nParam)
{
	int nRet = 0;
	switch(uOper)
	{
	case TEAM_OI_GD_INFO:		//Ö÷½ÇËùÔÚµÄ¶ÓÎéÐÅÏ¢
		if (uParam)
		{
			KUiPlayerTeam* pTeam = (KUiPlayerTeam*)uParam;
			nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.GetInfo(pTeam);
		}
		break;
	case TEAM_OI_GD_MEMBER_LIST://»ñÈ¡Ö÷½ÇËùÔÚ¶ÓÎé³ÉÔ±ÁÐ±í
		nRet = g_Team[0].GetMemberInfo((KUiPlayerItem *)uParam, nParam);
		break;
	case TEAM_OI_GD_REFUSE_INVITE_STATUS://»ñÈ¡¾Ü¾øÑûÇëµÄ×´Ì¬
		nRet = Player[CLIENT_PLAYER_INDEX].m_cTeam.GetAutoRefuseState();
		break;
	case TEAM_OI_COLLECT_NEARBY_LIST://»ñÈ¡ÖÜÎ§¶ÓÎéµÄÁÐ±í
		NpcSet.GetAroundOpenCaptain(Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp);
		break;
	case TEAM_OI_APPLY:			//ÉêÇë¼ÓÈëËýÈË¶ÓÎé
		if (uParam)
			Player[CLIENT_PLAYER_INDEX].ApplyAddTeam(((KUiTeamItem*)uParam)->Leader.nIndex);
		break;
	case TEAM_OI_CREATE:		//ÐÂ×é¶ÓÎé
		Player[CLIENT_PLAYER_INDEX].ApplyCreateTeam();//(char*)uParam);
		break;
	case TEAM_OI_APPOINT:		//ÈÎÃü¶Ó³¤£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		Player[CLIENT_PLAYER_INDEX].ApplyTeamChangeCaptain(((KUiPlayerItem*)uParam)->uId);		
		break;
	case TEAM_OI_INVITE:		//ÑûÇë±ðÈË¼ÓÈë¶ÓÎé£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		if (uParam)
		{
			Player[CLIENT_PLAYER_INDEX].TeamInviteAdd(((KUiPlayerItem*)uParam)->uId);

			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TEAM_SEND_INVITE, ((KUiPlayerItem*)uParam)->Name);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		break;
	case TEAM_OI_KICK:			//Ìß³ý¶ÓÀïµÄÒ»¸ö¶ÓÔ±£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		Player[CLIENT_PLAYER_INDEX].TeamKickMember(((KUiPlayerItem*)uParam)->uId);
		break;
	case TEAM_OI_LEAVE:			//Àë¿ª¶ÓÎé
		Player[CLIENT_PLAYER_INDEX].LeaveTeam();
		break;
	case TEAM_OI_CLOSE:			//¹Ø±Õ×é¶Ó£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		Player[CLIENT_PLAYER_INDEX].ApplyTeamOpenClose(nParam);
		break;
	case TEAM_OI_REFUSE_INVITE:		//¾Ü¾ø±ðÈËÑûÇë×Ô¼º¼ÓÈë¶ÓÎé
		Player[CLIENT_PLAYER_INDEX].m_cTeam.SetAutoRefuseInvite(nParam);
		break;
	case TEAM_OI_APPLY_RESPONSE:		//Åú×¼ËûÈË¼ÓÈë¶ÓÎé£¬Ö»ÓÐ¶Ó³¤µ÷ÓÃ²ÅÓÐÐ§¹û
		if (uParam)
		{
			if (nParam)
			{
				Player[CLIENT_PLAYER_INDEX].AcceptTeamMember(((KUiPlayerItem*)uParam)->uId);
			}
			else
			{
				Player[CLIENT_PLAYER_INDEX].m_cTeam.DeleteOneFromApplyList(((KUiPlayerItem*)uParam)->uId);
//				Player[CLIENT_PLAYER_INDEX].m_cTeam.UpdateInterface();
			}
		}
		break;
	case TEAM_OI_INVITE_RESPONSE://¶Ô×é¶ÓÑûÇëµÄ»Ø¸´
		if (uParam)
			Player[CLIENT_PLAYER_INDEX].m_cTeam.ReplyInvite(((KUiPlayerItem*)uParam)->nIndex, nParam);
		break;
	}
	return nRet;
}

int KCoreShell::GetGenreItem(unsigned int uId )
{
	return Item[uId].GetGenre();
}

int KCoreShell::GetTypeItem(unsigned int uId )
{
	if (Item[uId].GetGoldId())
	{
		return 1;
	}
	else if (Item[uId].IsPurple())
	{
		return 2;
	}
	return 0;
}

int KCoreShell::GetNPCBAITAN(int nIndex)
{
	int nID = NpcSet.SearchID(nIndex);
	return Npc[nID].m_BaiTan;
}

int KCoreShell::GetTradeItem(unsigned int uId )
{
    return Item[uId].GetIsTrade();

}


int KCoreShell::GetDropItem(unsigned int uId )
{
    return Item[uId].GetIsDrop();
}
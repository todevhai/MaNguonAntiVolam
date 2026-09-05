#include "KCore.h"
#ifndef WM_MOUSEHOVER
#define WM_MOUSEHOVER 0x02A1
#endif
#include "KEngine.h"
#ifdef _SERVER
//#include "KNetServer.h"
//#include "../MultiServer/Heaven/Interface/iServer.h"
#else
//#include "KNetClient.h"
#include "../../Headers/IClient.h"
#include "Scene/KScenePlaceC.h"
#include "KIme.h"
#endif
#include "KNpc.h"
#include "KObj.h"
#include "KNpcSet.h"
#include "KSubWorld.h"
#include "KPlayer.h"

#include "LuaFuns.h"
#include "KSortScript.h"
#include "KScriptValueSet.h"
#include "KObjSet.h"
#include "KSkills.h"
#include "KPlayerSet.h"
#include "KSubWorldSet.h"
#include "KFile.h"
#include "GameDataDef.h"
#include "KBuySell.h"
//#include "MyAssert.h"
#include "MsgGenreDef.h"
#include "KItemSet.h"
#include "KTaskFuns.h"
#include "Text.h"

#ifdef _SERVER
#ifndef _STANDALONE
#include "../../../lib/S3DBInterface.h"
#else
#include "S3DBInterface.h"
#endif
#else
#include "CoreShell.h"
#endif

extern int LuaGetNpcTalk(Lua_State *L);

#define		defPLAYER_LOGIN_TIMEOUT			10 * 20// 10 sec
#define		defPLAYER_SAVE_TIMEOUT			30 * 20

#define		PLAYER_LEVEL_1_EXP				48
#define		PLAYER_LEVEL_ADD_ATTRIBUTE		5
#define		PLAYER_LEVEL_ADD_SKILL			1
#define		PLAYER_SHARE_EXP_DISTANCE		768
#define		PLAYER_TEAM_EXP_ADD				50

#define		MAX_APPLY_TEAM_TIME				500

#define		BASE_WALK_SPEED					5
#define		BASE_RUN_SPEED					10

#define		BASE_FIRE_RESIST_MAX			75
#define		BASE_COLD_RESIST_MAX			75
#define		BASE_POISON_RESIST_MAX			75
#define		BASE_LIGHT_RESIST_MAX			75
#define		BASE_PHYSICS_RESIST_MAX			75

#define		BASE_ATTACK_SPEED				0
#define		BASE_CAST_SPEED					0
#define		BASE_VISION_RADIUS				120
#define		BASE_HIT_RECOVER				6

#define		TOWN_PORTAL_TIME				1800


KPlayer	Player[MAX_PLAYER];
int		g_nLastNetMsgLoop;

//-------------------------------------------------------------------------
//	���ܣ����캯��
//-------------------------------------------------------------------------
KPlayer::KPlayer()
{
	Release();
}

//-------------------------------------------------------------------------
//	���ܣ���������
//-------------------------------------------------------------------------
KPlayer::~KPlayer()
{
	Release();
}

//-------------------------------------------------------------------------
//	���ܣ����
//-------------------------------------------------------------------------
void	KPlayer::Release()
{
#ifndef _SERVER
	m_RunStatus = 0;
	m_dwNextLevelLeadExp = 0;
	m_nLeftSkillID = 0;
	m_nLeftSkillLevel = 0;
	m_nRightSkillID = 0;
	m_nRightSkillLevel = 0;
	m_nSendMoveFrames = defMAX_PLAYER_SEND_MOVE_FRAME;
	m_MouseDown[0] = FALSE;
	m_MouseDown[1] = FALSE;
	m_nImageNpcID = 0;
#endif
	m_dwID = 0;
	m_nIndex = 0;
	m_nNetConnectIdx = -1;
	m_cMenuState.Release();
	m_cTrade.Release();
	m_nAttributePoint = 0;
	m_nSkillPoint = 0;
	
	m_nStrength = 0;
	m_nDexterity = 0;
	m_nVitality = 0;
	m_nEngergy = 0;
	m_nLucky = 0;
	m_nCurStrength = 0;
	m_nCurDexterity = 0;
	m_nCurVitality = 0;
	m_nCurEngergy = 0;
	m_nCurLucky = 0;
	m_btChatSpecialChannel = 0;
	m_nUpExp = 0;
	
	m_nExp = 0;
	m_nNextLevelExp = PLAYER_LEVEL_1_EXP;
	m_bExchangeServer = FALSE;
	m_CUnlocked = 0;
	
	m_dwLeadExp = 0;
	m_dwLeadLevel = 1;
	
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	m_bWaitingPlayerFeedBack = false;
	m_btTryExecuteScriptTimes = 0;	//

	
#ifdef _SERVER
	m_bUseReviveIdWhenLogin = 0;
	m_dwDeathScriptId = 0;
	m_nGiveCallBackScript = 0;
	m_nGiveCallBackNum = 0;
	
	m_sLoginRevivalPos.m_nSubWorldID = 0;
	m_sLoginRevivalPos.m_nMpsX = 0;
	m_sLoginRevivalPos.m_nMpsY = 0;
	
	m_sDeathRevivalPos.m_nSubWorldID = 0;
	m_sDeathRevivalPos.m_nMpsX = 0;
	m_sDeathRevivalPos.m_nMpsY = 0;
	
	m_sPortalPos.m_nSubWorldId = 0;
	m_sPortalPos.m_nTime = 0;
	m_sPortalPos.m_nMpsX = 0;
	m_sPortalPos.m_nMpsY = 0;
	
	m_pLastScriptCacheNode = NULL;
	m_dwLoginTime			= -1;
	//	m_uLastPingTime			= -1;
	m_bFinishLoading = FALSE;
	m_uMustSave = SAVE_IDLE;
	m_bIsQuiting = FALSE;
	m_TimerTask.SetOwner(this);
	m_bSleepMode = FALSE;
	m_nLastNetOperationTime = 0;
	m_nPrePayMoney = 0;
	m_nPrePayXu = 0;
	m_nForbiddenFlag = 0;
	m_nExtPoint = 0;
	m_nChangeExtPoint = 0;
	m_nLiXian = 0;
	//	ZeroMemory(m_SaveBuffer, sizeof(m_SaveBuffer));
#endif
}

//-------------------------------------------------------------------------
//	���ܣ��趨 m_nPlayerIndex (������ֻ������PlayerSet��Init�е���)
//-------------------------------------------------------------------------
void	KPlayer::SetPlayerIndex(int nNo)
{
	if (nNo < 0)
		m_nPlayerIndex = 0;
	else
		m_nPlayerIndex = nNo;
}

//-------------------------------------------------------------------------
//	���ܣ���ñ�ʵ���� Player �����е�λ��
//-------------------------------------------------------------------------
int		KPlayer::GetPlayerIndex()
{
	return m_nPlayerIndex;
}

//-------------------------------------------------------------------------
//	���ܣ����ÿ����Ϸѭ������Ҫ�����Ķ���
//-------------------------------------------------------------------------
void	KPlayer::Active()
{
#ifdef _SERVER
	if (m_nNetConnectIdx == -1 || m_bExchangeServer)
		return;
	
	//ʱ������Ĵ���
	m_TimerTask.Activate(g_PlayerTimerCallBackFun);
	this->SendCurNormalSyncData();
	
	this->m_cChat.TimerAdd();
	
	// ��ɱ����ʱ
	this->m_cPK.Active();
#define	defMAX_SLEEP_TIME	3600
	if (Npc[m_nIndex].m_FightMode == 0 && m_bSleepMode == FALSE && g_SubWorldSet.GetGameTime() - m_nLastNetOperationTime > defMAX_SLEEP_TIME)
	{
		NPC_SLEEP_SYNC	SleepSync;
		SleepSync.ProtocolType = s2c_npcsleepmode;
		SleepSync.bSleep = 1;
		SleepSync.NpcID = Npc[m_nIndex].m_dwID;
		Npc[m_nIndex].SendDataToNearRegion(&SleepSync, sizeof(NPC_SLEEP_SYNC));
		m_bSleepMode = TRUE;
	}
#else
	// ���������˵Ĵ���
	if ( !m_cTeam.m_nFlag )
	{
		if (m_cTeam.m_nApplyCaptainID > 0)
		{
			if ( m_cTeam.m_dwApplyTimer == 0 )
			{
				m_cTeam.m_nApplyCaptainID = 0;
				
			}
			else
			{
				m_cTeam.m_dwApplyTimer--;
				if ( !NpcSet.SearchID(m_cTeam.m_nApplyCaptainID) )
				{
					m_cTeam.m_nApplyCaptainID = 0;
					m_cTeam.m_dwApplyTimer = 0;
				}
			}
		}
	}
	// �ӳ��Ĵ���
	else if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
	{
		for (int i = 0; i < MAX_TEAM_APPLY_LIST; i++)
		{
			if (m_cTeam.m_sApplyList[i].m_dwNpcID > 0)
			{
				if (m_cTeam.m_sApplyList[i].m_dwTimer == 0)
				{
					m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
					m_cTeam.UpdateInterface();
				}
				else
				{
					m_cTeam.m_sApplyList[i].m_dwTimer--;
					if ( !Npc[this->m_nIndex].SearchAroundID(m_cTeam.m_sApplyList[i].m_dwNpcID) )
					{
						m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
						m_cTeam.m_sApplyList[i].m_dwTimer = 0;
						m_cTeam.UpdateInterface();
					}
				}
			}
		}
	}
	
	m_nSendMoveFrames++;
	
	// ��ɱ����ʱ
	this->m_cPK.Active();
	
#endif
}


void	KPlayer::ProcessMsg(KWorldMsgNode *lpMsg)
{
	switch (lpMsg->m_dwMsgType)
	{
	case GWM_PLAYER_SKILL:
		break;
	case GWM_PLAYER_RUNTO:
		break;
	case GWM_PLAYER_WALKTO:
		break;
	case GWM_PLAYER_JUMPTO:
		break;
	default:
		break;
	}	
}

#ifndef _SERVER
void KPlayer::ProcessInputMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_MOUSEMOVE:
	case WM_MOUSEHOVER:
		if (wParam & MK_LBUTTON)
			OnButtonMove(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_left);
		else if (wParam & MK_RBUTTON)
			OnButtonMove(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_right);
		else
			OnMouseMove(LOWORD(lParam), HIWORD(lParam));
		break;
		
	case WM_LBUTTONUP:
		OnButtonUp(LOWORD(lParam), HIWORD(lParam), button_left);
		break;
		
	case WM_LBUTTONDOWN:
		OnButtonDown(LOWORD(lParam), HIWORD(lParam), 
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_left);
		break;
		
	case WM_RBUTTONUP:
		OnButtonUp(LOWORD(lParam), HIWORD(lParam), button_right);
		break;	
		
	case WM_RBUTTONDOWN:
		OnButtonDown(LOWORD(lParam), HIWORD(lParam),
			(wParam & ~(MK_LBUTTON | MK_MBUTTON | MK_RBUTTON)), button_right);
		break;
	}
}

void KPlayer::OnButtonDown(int x,int y, int Key, MOUSE_BUTTON nButton)
{
	m_MouseDown[(int)nButton] = TRUE;

	FindSelectNpc(x, y, relation_all);
	FindSelectObject(x, y);
	//	Npc[m_nIndex].m_nPeopleIdx = m_nPeapleIdx;
	ProcessMouse(x, y, Key, nButton);
}

void KPlayer::OnButtonMove(int x,int y,int Key, MOUSE_BUTTON nButton)
{
	if (m_MouseDown[(int)nButton])
	{
		ProcessMouse(x, y, Key, nButton);
	}
}

void KPlayer::OnButtonUp(int x,int y,MOUSE_BUTTON nButton)
{
	m_MouseDown[(int)nButton] = 0;
}

void KPlayer::OnMouseMove(int x,int y)
{
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	FindSelectNpc(x, y, relation_all);
	FindSelectObject(x, y);
	if (m_nPeapleIdx)
	{
		if (Npc[m_nPeapleIdx].m_Kind == kind_dialoger)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		else if (NpcSet.GetRelation(m_nIndex, m_nPeapleIdx) == relation_enemy)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_FIGHT);
		else
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL);
	}
	else if (m_nObjectIdx)
	{
		if (Object[m_nObjectIdx].m_nKind == Obj_Kind_MapObj)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_DIALOG);
		else if (Object[m_nObjectIdx].m_nKind == Obj_Kind_Item || Object[m_nObjectIdx].m_nKind == Obj_Kind_Money)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_PICK);
		else if (Object[m_nObjectIdx].m_nKind == Obj_Kind_Prop)
			CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_USE);
	}
	else
		CoreDataChanged(GDCNI_SWITCH_CURSOR, NULL, MOUSE_CURSOR_NORMAL);
}

void KPlayer::ProcessMouse(int x, int y, int Key, MOUSE_BUTTON nButton)
{
	if (CheckTrading())
		return;

	if (m_ItemList.m_Hand > 0)
	{
		if (nButton == button_left)
		{
			ThrowAwayItem();
			return;
		}
	}

	int nX = x;
	int nY = y;
	int nZ = 0;
	g_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);
	
	/*	if (Key & MK_SHIFT)
	{
	Npc[m_nIndex].SendCommand(do_jump, nX, nY);
	SendClientCmdJump(nX, nY);
	return;
}*/
	
	if (Npc[m_nIndex].IsCanInput())
	{

		int nIdx = 0;
		
		if (nButton == button_right)
		{
			nIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nRightSkillID);
			g_DebugLog("[skill]right");
		}
		else
		{
			nIdx = Npc[m_nIndex].m_SkillList.FindSame(m_nLeftSkillID);
			g_DebugLog("[skill]left");
		}
		Npc[m_nIndex].SetActiveSkill(nIdx);
	}
	else
	{
		g_DebugLog("[skill]return");
		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}
	
	if ((Key & MK_SHIFT) || (nButton == button_right))
	{
		if (Npc[m_nIndex].m_ActiveSkillID > 0)
		{
			ISkill * pISkill = (KSkill *) g_SkillManager.GetSkill(Npc[m_nIndex].m_ActiveSkillID, 1);
			if (!pISkill) 
                return;
			
			if (pISkill->IsAura())
				return;
			int nAttackRange = pISkill->GetAttackRadius();
			
			int nTargetIdx = 0;
			//			m_nPeapleIdx = 0;
			//����Object / Enemy / Ally �����ȼ��ҵ���Ҫ��Ķ���id
			
			
			if (pISkill->IsTargetAlly())
			{
				FindSelectNpc(x, y, relation_ally);
				if (m_nPeapleIdx)
				{
					nTargetIdx = m_nPeapleIdx;
				}
			}
			
			if (pISkill->IsTargetEnemy())
			{
				FindSelectNpc(x, y, relation_enemy);
				if (m_nPeapleIdx)
				{
					nTargetIdx = m_nPeapleIdx;
				}
			}
			
			if (pISkill->IsTargetObj())
			{
				FindSelectObject(x, y);
				if (m_nObjectIdx)
				{
					nTargetIdx = m_nObjectIdx;
				}
			}
			//������ܱ���ָ�����󣬶���ǰλ���޶���Ļ���ֱ���˳�
			if (pISkill->IsTargetOnly() && !nTargetIdx)
            {
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return;
			}
			
			if (m_nIndex == nTargetIdx)
			{
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return;
			}
			
			if ((!Npc[m_nIndex].m_SkillList.CanCast(Npc[m_nIndex].m_ActiveSkillID, SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_dwCurrentTime))
				||
				(!Npc[m_nIndex].Cost(pISkill->GetSkillCostType() , pISkill->GetSkillCost(&Npc[m_nIndex]), TRUE))
				)
			{
				Npc[m_nIndex].m_nPeopleIdx = 0;
				m_nPeapleIdx = 0;
				return ;
			}
			
			//�޶���ֱ�ӷ�����
			if (!nTargetIdx)
			{
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, nX, nY);
				// Send to Server		
				SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, nX, nY);
			}
			else
			{
				if (pISkill->IsTargetOnly())
				{
					int distance = NpcSet.GetDistance(m_nIndex , nTargetIdx);
					if (distance > pISkill->GetAttackRadius())
					{
						m_nPeapleIdx = nTargetIdx;
						return ;
					}
				}
				// 
				if (m_nIndex == nTargetIdx && pISkill->GetSkillStyle() == SKILL_SS_Missles) 
					return ;
				Npc[m_nIndex].SendCommand(do_skill, Npc[m_nIndex].m_ActiveSkillID, -1, nTargetIdx);
				// Send to Server		
				SendClientCmdSkill(Npc[m_nIndex].m_ActiveSkillID, -1, Npc[nTargetIdx].m_dwID);
			}
		}
		Npc[m_nIndex].m_nPeopleIdx = 0;
		return;
	}
	
	if (Key == 0 )
	{
		int nRelation = NpcSet.GetRelation(m_nIndex, m_nPeapleIdx);
		if(nRelation == relation_enemy || nRelation == relation_dialog)
		{
			Npc[m_nIndex].m_nPeopleIdx = m_nPeapleIdx;
		}
		
		if (m_nSendMoveFrames >= defMAX_PLAYER_SEND_MOVE_FRAME)
		{
			m_nPickObjectIdx = m_nObjectIdx;
			Npc[m_nIndex].m_nObjectIdx = m_nPickObjectIdx;

			if (!m_RunStatus)
			{
				Npc[m_nIndex].SendCommand(do_walk, nX, nY);
				// Send to Server
				SendClientCmdWalk(nX, nY);
			}
			else
			{
				Npc[m_nIndex].SendCommand(do_run, nX, nY);
				// Send to Server
				SendClientCmdRun(nX, nY);
			}
			m_nSendMoveFrames = 0;
		}
		return;
	}
	
	/*	if (Key & MK_ALT)
	{
	Npc[m_nIndex].SendCommand(do_jump, nX, nY);
	SendClientCmdJump(nX, nY);
	}*/
}

void KPlayer::Walk(int nDir, int nSpeed)
{
	int	nMapX = Npc[m_nIndex].m_MapX;
	int nMapY = Npc[m_nIndex].m_MapY;
	int	nOffX = Npc[m_nIndex].m_OffX;
	int	nOffY = Npc[m_nIndex].m_OffY;
	int	nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	int	nRegion = Npc[m_nIndex].m_RegionIndex;
	int	nX, nY;
	
	SubWorld[nSubWorld].Map2Mps(nRegion, nMapX, nMapY, nOffX, nOffY, &nX, &nY);
	SubWorld[nSubWorld].GetMps(&nX, &nY, nSpeed * 2, nDir);
	
	if (m_RunStatus)
	{
		Npc[m_nIndex].SendCommand(do_run, nX, nY);
		// Send to Server
		if ( !CheckTrading() )
			SendClientCmdRun(nX, nY);
	}
	else
	{
		Npc[m_nIndex].SendCommand(do_walk, nX, nY);
		// Send to Server
		if (!CheckTrading())
			SendClientCmdWalk(nX, nY);
	}
}


void KPlayer::TurnLeft()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir > 8)
		Npc[m_nIndex].m_Dir -= 8;
	else
		Npc[m_nIndex].m_Dir = MAX_NPC_DIR - 1;
}

void KPlayer::TurnRight()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir < MAX_NPC_DIR - 9)
		Npc[m_nIndex].m_Dir += 8;
	else
		Npc[m_nIndex].m_Dir = 0;
}

void KPlayer::TurnBack()
{
	if (Npc[m_nIndex].m_Doing != do_stand &&
		Npc[m_nIndex].m_Doing != do_sit)
		return;
	
	if (Npc[m_nIndex].m_Dir < MAX_NPC_DIR / 2)
		Npc[m_nIndex].m_Dir += MAX_NPC_DIR / 2;
	else
		Npc[m_nIndex].m_Dir -= MAX_NPC_DIR / 2;
}

void KPlayer::FindSelectNpc(int x, int y, int nRelation)
{
	int	nNpcIdx = 0;
	
	nNpcIdx = NpcSet.SearchNpcAt(x, y, nRelation, 40);

	if (Npc[nNpcIdx].m_Hide.nTime > 0 && NpcSet.GetRelation(m_nIndex,nNpcIdx) == relation_enemy)
		m_nPeapleIdx = 0;
	else if (nNpcIdx)
		{
			m_nPeapleIdx = nNpcIdx;
			m_nImageNpcID = nNpcIdx;
		}
	else
		m_nPeapleIdx = 0;
}

void KPlayer::FindSelectObject(int x, int y)
{
	int	nObjIdx = 0;
	
	nObjIdx = ObjSet.SearchObjAt(x, y, 40);
	if (nObjIdx)
		m_nObjectIdx = nObjIdx;
	else
		m_nObjectIdx = 0;
}
// need change
int	KPlayer::NetCommandPlayerTalk(BYTE * pProtocol)
{
/*	DWORD	dwNpcID;
int		nSize;
dwNpcID = *(DWORD *) &pProtocol[1];
nSize = pProtocol[5];
int nIdx = NpcSet.SearchID(dwNpcID);

  if (ConformIdx(nIdx))
  {
		strcpy(Npc[nIdx].m_szChatBuffer, (char *)(pProtocol+6));
		Npc[nIdx].m_nCurChatTime = NPC_SHOW_CHAT_TIME_LENGTH;
		}
		
		  return (7 + nSize);	// need test
	*/
	return 0;
}


BOOL KPlayer::ConformIdx(int nIdx)
{
	if (nIdx == m_nIndex || nIdx == 0)
		return FALSE;
	return TRUE;
}
#endif

#ifdef _SERVER
BOOL KPlayer::IsWaitingRemove()
{
	if (!m_dwID)
		return FALSE;
	return m_bIsQuiting;
}

void KPlayer::WaitForRemove()
{
	m_bIsQuiting = TRUE;
}

BOOL KPlayer::IsLoginTimeOut()
{
	if (m_nNetConnectIdx != -1)
		return FALSE;
	
	if (!m_dwID)
		return FALSE;
	
	if (-1 != m_dwLoginTime &&
		g_SubWorldSet.GetGameTime() - m_dwLoginTime > defPLAYER_LOGIN_TIMEOUT)
	{
		//		m_dwLoginTime = -1;
		return TRUE;
	}
	
	return FALSE;
}


void KPlayer::LoginTimeOut()
{
	// ֪ͨ��������Լ�������
	m_cChat.OffLine(m_dwID);
	
	m_pStatusLoadPlayerInfo = NULL;
	
	Release();
}

BOOL KPlayer::Save()
{
	if (m_nIndex <= 0 && m_dwID == 0)
		return FALSE;
	
		/*	if (m_uMustSave != SAVE_IDLE)
		return FALSE;
	*/
	TRoleData* pData = (TRoleData *)m_SaveBuffer;
	
	if (UpdateDBPlayerInfo((BYTE *)pData) == -1)
		return FALSE;
		/*
		#ifdef _DEBUG
		KFile File;
		char szFileName[32];
		sprintf(szFileName, "d:\\%sSave.Buf", m_PlayerName);
		File.Create(szFileName);
		File.Write(pData, pData->dwDataLen);
		File.Close();
		#endif
	*/
	_ASSERT(pData->dwDataLen < sizeof(m_SaveBuffer));
	if (pData->dwDataLen <= 0)
		return FALSE;
	
	//	m_uMustSave = SAVE_REQUEST;
	m_ulLastSaveTime = g_SubWorldSet.GetGameTime();
	return TRUE;
}

BOOL KPlayer::CanSave()
{
	if (m_nNetConnectIdx == -1)
		return FALSE;
	
	if (m_nIndex <= 0)
		return FALSE;
	
	if (m_bExchangeServer)
		return FALSE;
	
	if (m_bIsQuiting)
		return FALSE;
	
	if (CheckTrading())
	{
		return FALSE;
	}
	
	if (m_uMustSave == SAVE_DOING && g_SubWorldSet.GetGameTime() - m_ulLastSaveTime > defPLAYER_SAVE_TIMEOUT)
		return TRUE;
	
	if (m_uMustSave != SAVE_IDLE)
		return FALSE;
	
	return TRUE;
}

BOOL KPlayer::SendSyncData(int &nStep, unsigned int &nParam)
{	
	BOOL bRet = FALSE;
	switch(nStep)
	{
	case STEP_BASE_INFO:
		{	
			bRet = SubWorld[Npc[m_nIndex].m_SubWorldIndex].SendSyncData(m_nIndex, m_nNetConnectIdx);
			if (!bRet)
			{
				printf("SubWorld Fail.\n");
				break;
			}
			bRet = Npc[m_nIndex].SendSyncData(m_nNetConnectIdx);
			if (!bRet)
			{
				printf("NPC Fail.\n");
				break;
			}
			// �����Ϣ������ͬ������NPC���ݺ�����ʹ�ͻ������ҵ���ǰ����ڿͻ��˵�Npc����
			CURPLAYER_SYNC	sSync;	// ͬ����ǰ��ҵ�����������Ϣ���ͻ��ˣ�װ���ȣ�
			sSync.ProtocolType = (BYTE)s2c_synccurplayer;
			sSync.m_dwID = Npc[m_nIndex].m_dwID;
			sSync.m_btLevel = (DWORD)Npc[m_nIndex].m_Level;
			sSync.m_btSex = Npc[m_nIndex].m_nSex;
			sSync.m_btKind = Npc[m_nIndex].m_Kind;
			sSync.m_btSeries = Npc[m_nIndex].m_Series;
			sSync.m_wLifeMax = Npc[m_nIndex].m_LifeMax;
			sSync.m_wStaminaMax = Npc[m_nIndex].m_StaminaMax;
			sSync.m_wManaMax = Npc[m_nIndex].m_ManaMax;
			sSync.m_HeadImage = Npc[m_nIndex].m_HeadImage;
			sSync.m_wAttributePoint = this->m_nAttributePoint;
			sSync.m_wSkillPoint = this->m_nSkillPoint;
			sSync.m_wStrength = this->m_nStrength;
			sSync.m_wDexterity = this->m_nDexterity;
			sSync.m_wVitality = this->m_nVitality;
			sSync.m_wEngergy = this->m_nEngergy;
			sSync.m_wLucky = this->m_nLucky;
			sSync.m_nExp = this->m_nExp;
			sSync.m_dwLeadExp = this->m_dwLeadExp;
			sSync.m_btCUnlocked = this->m_CUnlocked;
			sSync.m_btCurFaction = this->m_cFaction.m_nCurFaction;
			sSync.m_btFirstFaction = this->m_cFaction.m_nFirstAddFaction;
			sSync.m_nFactionAddTimes = this->m_cFaction.m_nAddTimes;
			sSync.m_wWorldStat = (WORD)m_nWorldStat;
			sSync.m_wSectStat = (WORD)m_nSectStat;
			sSync.m_nMoney1 = this->m_ItemList.GetMoney(room_equipment);
			sSync.m_nMoney2 = this->m_ItemList.GetMoney(room_repository);
			sSync.m_nXu = this ->m_ItemList.GetXu(room_equipment);
			if (SUCCEEDED(g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(CURPLAYER_SYNC))))
			{
				bRet = TRUE;
			}
			else
			{
				printf("player Packing sync data failed...\n");
				bRet = FALSE;
				break;
			}
		}
		break;
	case STEP_FIGHT_SKILL_LIST:	
		{
			bRet = SendSyncData_Skill();
		}
		break;
	case STEP_LIFE_SKILL_LIST:
		bRet = TRUE;
		break;
	case STEP_ITEM_LIST:
		bRet = TRUE;
		break;
	case STEP_TASK_LIST:
		bRet = TRUE;
		break;
	case STEP_SYNC_END:
		bRet = TRUE;
		break;
	}
	if (!bRet)
	{
		printf("Load PlayerInfo Step %d Failed...\n", nStep);
	}
	ExecuteScript("\\script\\system\\online.lua","main","");
	return bRet;
}

BOOL KPlayer::SendSyncData_Skill()
{
	SKILL_SEND_ALL_SYNC		sSkill;
	
	sSkill.ProtocolType = s2c_synccurplayerskill;
	sSkill.m_wProtocolLong = 2;
	int i;
	int j;
	for (i = 0,j = 0; i < MAX_NPCSKILL; i++)
	{
		sSkill.m_sAllSkill[i].SkillId = 0;
		sSkill.m_sAllSkill[i].SkillLevel = 0;
		if( Npc[m_nIndex].m_SkillList.m_Skills[i].SkillId <= 0)
			continue;
		sSkill.m_sAllSkill[j].SkillId = Npc[m_nIndex].m_SkillList.m_Skills[i].SkillId;
		sSkill.m_sAllSkill[j].SkillLevel = Npc[m_nIndex].m_SkillList.m_Skills[i].SkillLevel;
		sSkill.m_sAllSkill[j].SkillAdd = Npc[m_nIndex].m_SkillList.m_Skills[i].AddPoint;
		j++;
	}
	sSkill.m_wProtocolLong += sizeof(SKILL_SEND_ALL_SYNC_DATA) * j;
	
	if (SUCCEEDED(g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSkill, sSkill.m_wProtocolLong + 1)))
	{
		return TRUE;
	}
	else
	{
		printf("player Packing skill sync data failed...\n");
		return FALSE;
	}
}

void	KPlayer::SendCurNormalSyncData()
{
	CURPLAYER_NORMAL_SYNC	sSync;
	
	sSync.ProtocolType = s2c_synccurplayernormal;
	sSync.m_shLife = Npc[m_nIndex].m_CurrentLife;
	sSync.m_shStamina = Npc[m_nIndex].m_CurrentStamina;
	sSync.m_shMana = Npc[m_nIndex].m_CurrentMana;
	sSync.m_shLifeMax = Npc[m_nIndex].m_CurrentLifeMax;
	sSync.m_shStaminaMax = Npc[m_nIndex].m_CurrentStaminaMax;
	sSync.m_shManaMax = Npc[m_nIndex].m_CurrentManaMax;
	sSync.m_shAngry = 0;
	if ( !m_cTeam.m_nFlag )
		sSync.m_btTeamData = 0;
	else
	{
		if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
			sSync.m_btTeamData = 0x03;
		else
			sSync.m_btTeamData = 0x01;
	}
	
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(CURPLAYER_NORMAL_SYNC));
}

void KPlayer::QuitGame(int nType)
{
	Save();
}

void KPlayer::BuyItem(BYTE* pProtocol)
{
	PLAYER_BUY_ITEM_COMMAND* pCommand = (PLAYER_BUY_ITEM_COMMAND *)pProtocol;
	BuySell.Buy(m_nPlayerIndex, m_BuyInfo.m_nBuyIdx, pCommand->m_BuyIdx, pCommand->m_Place, pCommand->m_X, pCommand->m_Y);
}

void KPlayer::SellItem(BYTE* pProtocol)
{
	PLAYER_SELL_ITEM_COMMAND* pCommand = (PLAYER_SELL_ITEM_COMMAND *)pProtocol;
	BuySell.Sell(m_nPlayerIndex, m_BuyInfo.m_nBuyIdx, m_ItemList.SearchID(pCommand->m_ID));
}

#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ���ȡ���������λ��
//-------------------------------------------------------------------------
void KPlayer::GetLoginRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY)
{
	*lpnSubWorld = m_sLoginRevivalPos.m_nSubWorldID;
	*lpnMpsX = m_sLoginRevivalPos.m_nMpsX;
	*lpnMpsY = m_sLoginRevivalPos.m_nMpsY;
}


void KPlayer::GetDeathRevivalPos(int *lpnSubWorld, int *lpnMpsX, int *lpnMpsY)
{
	*lpnSubWorld = m_sDeathRevivalPos.m_nSubWorldID;
	*lpnMpsX = m_sDeathRevivalPos.m_nMpsX;
	*lpnMpsY = m_sDeathRevivalPos.m_nMpsY;
}
#endif
#ifdef _SERVER
void	KPlayer::SetTimer(DWORD nTime, int nTimerTaskId)					//ʱ������ű���������ʱ��
{
	if (!nTime || !nTimerTaskId) return ;
	m_TimerTask.SetTimer(nTime, nTimerTaskId);
}

void	KPlayer::CloseTimer()							//�ر�ʱ���ʱ��
{
	m_TimerTask.CloseTimer();
}
#endif


#ifdef _SERVER
//------------------------------------------------------------------------------
//	���ܣ��趨���������λ��
//------------------------------------------------------------------------------
void	KPlayer::SetRevivalPos(int nSubWorld, int nReviveId)
{
	if (nSubWorld >= 0)  //���С��0����ʾ���õ�ǰ��
	{
		m_sLoginRevivalPos.m_nSubWorldID = nSubWorld;
	}
	else
	{
		m_sLoginRevivalPos.m_nSubWorldID = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	}
	
	POINT Pos;
	
	g_SubWorldSet.GetRevivalPosFromId(m_sLoginRevivalPos.m_nSubWorldID, nReviveId, &Pos);
	m_sLoginRevivalPos.m_ReviveID = nReviveId;
	m_sLoginRevivalPos.m_nMpsX = Pos.x;
	m_sLoginRevivalPos.m_nMpsY = Pos.y;
	
	m_sDeathRevivalPos = m_sLoginRevivalPos;
}
#endif

#ifdef _SERVER
//------------------------------------------------------------------------------
//	���ܣ��յ��ͻ��˲�ѯĳ��npc�����Ϣ�������������ͻ��˷��Ͷ�����Ϣ
//------------------------------------------------------------------------------
void	KPlayer::S2CSendTeamInfo(BYTE* pProtocol)
{
	PLAYER_APPLY_TEAM_INFO	*pApplyTeamInfo = (PLAYER_APPLY_TEAM_INFO*)pProtocol;
	
	//------------------------------ ��ѯ���������Ϣ -----------------------------
	if (pApplyTeamInfo->m_dwTarNpcID == Npc[m_nIndex].m_dwID)
	{
		SendSelfTeamInfo();
		return;
	}
	
	//--------------------------- ��ѯ���� npc �����Ϣ -----------------------------
	PLAYER_SEND_TEAM_INFO	sTeamInfo;
	PLAYER_APPLY_TEAM_INFO_FALSE	sTeamInfoFalse;
	// Ѱ����Ӧ npc id �� player ���
	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(pApplyTeamInfo->m_dwTarNpcID);
	if ( nPlayer >= 0)
		goto SendMsg;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(pApplyTeamInfo->m_dwTarNpcID);
		if (nPlayer >= 0)
			break;
	}
	if (i >= 8)
		goto SendFalse;
	
SendMsg:
	// Ѱ�Ҷ���
	KTeam	*pTeam;
	if ( !Player[nPlayer].m_cTeam.m_nFlag )
		goto SendFalse;
	pTeam = &g_Team[Player[nPlayer].m_cTeam.m_nID];
	if (pTeam->m_nCaptain < 0 || !pTeam->IsOpen())
		goto SendFalse;
	
	// ���Ͷ�����Ϣ
	sTeamInfo.ProtocolType = (BYTE)s2c_teaminfo;
	sTeamInfo.m_nCaptain = Npc[Player[pTeam->m_nCaptain].m_nIndex].m_dwID;
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (pTeam->m_nMember[i] < 0)
			sTeamInfo.m_nMember[i] = 0;
		else
			sTeamInfo.m_nMember[i] = Npc[Player[pTeam->m_nMember[i]].m_nIndex].m_dwID;
	}
	//	strcpy(sTeamInfo.m_szTeamName, pTeam->m_szName);
	
	// ����
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sTeamInfo, sizeof(PLAYER_SEND_TEAM_INFO));
	return;
	
	// ����ʧ��֪ͨ
SendFalse:
	sTeamInfoFalse.ProtocolType = s2c_teamapplyinfofalse;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sTeamInfoFalse, sizeof(PLAYER_APPLY_TEAM_INFO_FALSE));
	return;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ���������ͻ��˷��Ͷ�����Ϣ
//-------------------------------------------------------------------------
void	KPlayer::SendSelfTeamInfo()
{
	if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)	// ���Ͷ�����Ϣ
	{
		int		nNpcIndex;
		PLAYER_SEND_SELF_TEAM_INFO	sSelfInfo;
		sSelfInfo.ProtocolType = s2c_teamselfinfo;
		sSelfInfo.nTeamServerID = m_cTeam.m_nID;
		sSelfInfo.m_dwLeadExp = m_dwLeadExp;
		sSelfInfo.m_btState = g_Team[m_cTeam.m_nID].m_nState;
		
		// �ӳ�
		nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex;
		sSelfInfo.m_dwNpcID[0] = Npc[nNpcIndex].m_dwID;
		sSelfInfo.m_btLevel[0] = (DWORD)Npc[nNpcIndex].m_Level;
		strcpy(sSelfInfo.m_szNpcName[0], Npc[nNpcIndex].Name);
		// ��Ա
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0)
			{
				nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
				sSelfInfo.m_dwNpcID[i + 1] = Npc[nNpcIndex].m_dwID;
				sSelfInfo.m_btLevel[i + 1] = (DWORD)Npc[nNpcIndex].m_Level;
				strcpy(sSelfInfo.m_szNpcName[i + 1], Npc[nNpcIndex].Name);
			}
			else
			{
				sSelfInfo.m_dwNpcID[i + 1] = 0;
				sSelfInfo.m_btLevel[i + 1] = 0;
			}
		}
		// ������Ϣ
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSelfInfo, sizeof(PLAYER_SEND_SELF_TEAM_INFO));
	}
	else	// ���������Ϣ
	{
		PLAYER_LEAVE_TEAM	sLeaveTeam;
		sLeaveTeam.ProtocolType = s2c_teamleave;
		sLeaveTeam.m_dwNpcID = Npc[m_nIndex].m_dwID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ������󴴽�һ֧����
//-------------------------------------------------------------------------
BOOL	KPlayer::CreateTeam(BYTE* pProtocol)
{
	return m_cTeam.CreateTeam(this->m_nPlayerIndex, (PLAYER_APPLY_CREATE_TEAM*)pProtocol);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ������󿪷š��رձ�player�����Ƿ�������Ա����״̬
//-------------------------------------------------------------------------
BOOL	KPlayer::SetTeamState(BYTE* pProtocol)
{
	if (this->CheckTrading())
		return FALSE;
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN || m_cTeam.m_nID < 0)
	{
		SendSelfTeamInfo();
		return FALSE;
	}
	
	PLAYER_TEAM_OPEN_CLOSE	*pTeamState = (PLAYER_TEAM_OPEN_CLOSE*)pProtocol;
	if (pTeamState->m_btOpenClose == 0)
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
	}
	else
	{
		if (m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADEOPEN)
			m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
		g_Team[m_cTeam.m_nID].SetTeamOpen();
	}
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ����������һ֧����
//-------------------------------------------------------------------------
BOOL	KPlayer::S2CSendAddTeamInfo(BYTE* pProtocol)
{
	if (!m_cTeam.m_bCanTeamFlag)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_CANNOT_ADD_TEAM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	PLAYER_APPLY_ADD_TEAM	*pAddTeam = (PLAYER_APPLY_ADD_TEAM*)pProtocol;
	
	// Ѱ�Ҷӳ� npc id �� player ���
	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(pAddTeam->m_dwTarNpcID);
	if ( nPlayer >= 0)
		goto SendMsg;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(pAddTeam->m_dwTarNpcID);
		if (nPlayer >= 0)
			break;
	}
	if (i >= 8)
		return FALSE;
	
SendMsg:
	if (Npc[Player[nPlayer].m_nIndex].m_Camp == camp_begin && Npc[m_nIndex].m_Camp != camp_begin)
		return FALSE;
	m_cTeam.m_nApplyCaptainID = pAddTeam->m_dwTarNpcID;
	PLAYER_APPLY_ADD_TEAM	sAddTeam;
	sAddTeam.ProtocolType = s2c_teamgetapply;
	sAddTeam.m_dwTarNpcID = Npc[m_nIndex].m_dwID;
	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sAddTeam, sizeof(PLAYER_APPLY_ADD_TEAM));
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ӳ�֪ͨ����������ĳ��npcΪ�����Ա
//-------------------------------------------------------------------------
BOOL	KPlayer::AddTeamMember(BYTE* pProtocol)
{
	// ������(�Ƿ���ӡ��Ƿ�ӳ������鿪��״̬����Ա����ͳ����)
	if ( !m_cTeam.m_nFlag ||
		m_cTeam.m_nFigure != TEAM_CAPTAIN ||
		!g_Team[m_cTeam.m_nID].IsOpen() ||
		g_Team[m_cTeam.m_nID].m_nMemNum >= MAX_TEAM_MEMBER ||
		g_Team[m_cTeam.m_nID].m_nMemNum >= g_Team[m_cTeam.m_nID].CalcCaptainPower())
	{
		SendSelfTeamInfo();
		return FALSE;
	}
	
	// ��npc id����player
	int		nPlayer, i;
	PLAYER_ACCEPT_TEAM_MEMBER	*pAccept = (PLAYER_ACCEPT_TEAM_MEMBER*)pProtocol;
	nPlayer = FindAroundPlayer(pAccept->m_dwNpcID);
	if (nPlayer < 0)
		return FALSE;
	// �Է������ڲ������״̬
	if (!Player[nPlayer].m_cTeam.m_bCanTeamFlag)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TARGET_CANNOT_ADD_TEAM;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	// �Ѿ��ڶ�����
	if (Player[nPlayer].m_cTeam.m_nFlag)
		return FALSE;
	if (Player[nPlayer].m_cTeam.m_nApplyCaptainID != Npc[this->m_nIndex].m_dwID)
		return FALSE;
	// �������ӳ�Ա
	if ( !g_Team[m_cTeam.m_nID].AddMember(nPlayer) )
		return FALSE;
	// �ı����״̬
	if (g_Team[m_cTeam.m_nID].m_nMemNum >= MAX_TEAM_MEMBER || g_Team[m_cTeam.m_nID].CheckFull())
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
	}
	// �����ܶ�Ա�������ݴ���
	Player[nPlayer].m_cTeam.Release();
	Player[nPlayer].m_cTeam.m_nFlag = 1;
	Player[nPlayer].m_cTeam.m_nFigure = TEAM_MEMBER;
	Player[nPlayer].m_cTeam.m_nID = m_cTeam.m_nID;
	Npc[Player[nPlayer].m_nIndex].SetCurrentCamp(Npc[m_nIndex].m_Camp);
	
	// ��������Ա����Ϣ
	PLAYER_TEAM_ADD_MEMBER	sAddMem;
	sAddMem.ProtocolType = s2c_teamaddmember;
	sAddMem.m_dwNpcID = Npc[Player[nPlayer].m_nIndex].m_dwID;
	sAddMem.m_btLevel = (DWORD)Npc[Player[nPlayer].m_nIndex].m_Level;
	strcpy(sAddMem.m_szName, Npc[Player[nPlayer].m_nIndex].Name);
	
	// ���ӳ�����Ϣ
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sAddMem, sizeof(PLAYER_TEAM_ADD_MEMBER));
	
	// ���϶�Ա����Ϣ
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] < 0 || g_Team[m_cTeam.m_nID].m_nMember[i] == nPlayer)
			continue;
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sAddMem, sizeof(PLAYER_TEAM_ADD_MEMBER));
	}
	
	// ------------------------------------- ���¶�Ա����Ϣ --------------------------
	int		nNpcIndex;
	PLAYER_SEND_SELF_TEAM_INFO	sSelfInfo;
	sSelfInfo.ProtocolType = s2c_teamselfinfo;
	sSelfInfo.nTeamServerID = m_cTeam.m_nID;
	sSelfInfo.m_dwLeadExp = Player[nPlayer].m_dwLeadExp;
	sSelfInfo.m_btState = g_Team[m_cTeam.m_nID].m_nState;
	// �ӳ�����
	nNpcIndex = m_nIndex;
	sSelfInfo.m_dwNpcID[0] = Npc[nNpcIndex].m_dwID;
	sSelfInfo.m_btLevel[0] = (DWORD)Npc[nNpcIndex].m_Level;
	strcpy(sSelfInfo.m_szNpcName[0], Npc[nNpcIndex].Name);
	// ��Ա����
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0)
		{
			nNpcIndex = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
			sSelfInfo.m_dwNpcID[i + 1] = Npc[nNpcIndex].m_dwID;
			sSelfInfo.m_btLevel[i + 1] = (DWORD)Npc[nNpcIndex].m_Level;
			strcpy(sSelfInfo.m_szNpcName[i + 1], Npc[nNpcIndex].Name);
		}
		else
		{
			sSelfInfo.m_dwNpcID[i + 1] = 0;
			sSelfInfo.m_btLevel[i + 1] = 0;
		}
	}
	// ������Ϣ
	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sSelfInfo, sizeof(PLAYER_SEND_SELF_TEAM_INFO));
	
	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_SELF_ADD;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
	sMsg.m_lpBuf = 0;
	g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	
	// --------------------------------- ���¶�Ա����Ϣ end --------------------------
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ��˶�Ա֪ͨ�뿪����
//-------------------------------------------------------------------------
void	KPlayer::LeaveTeam(BYTE* pProtocol)
{
	if (!m_cTeam.m_nFlag)
		return;
	if (this->m_cTeam.m_nFigure == TEAM_CAPTAIN && g_Team[m_cTeam.m_nID].IsOpen())
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
	}
	
	// �Ƕӳ��뿪���飬��ϵͳ��Ϣ���ӳ��뿪�����ϵͳ��Ϣ������DeleteMember����
	if (m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_LEAVE;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (void*)Npc[this->m_nIndex].m_dwID;
		
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] > 0)
			{
				g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			}
		}
		sMsg.m_lpBuf = 0;
	}
	
	// �������˶�Ա�뿪�Ĵ���
	g_Team[m_cTeam.m_nID].DeleteMember(GetPlayerIndex());
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ��˶ӳ�֪ͨ�߳�ĳ����Ա
//-------------------------------------------------------------------------
void	KPlayer::TeamKickOne(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	int		nMemNo;
	PLAYER_TEAM_KICK_MEMBER	*pKickOne = (PLAYER_TEAM_KICK_MEMBER*)pProtocol;
	nMemNo = g_Team[m_cTeam.m_nID].FindMemberID(pKickOne->m_dwNpcID);
	if (nMemNo < 0)
		return;
	int		nPlayerNo;
	// �������˶�Ա�뿪�Ĵ���
	nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[nMemNo];
	g_Team[m_cTeam.m_nID].DeleteMember(nPlayerNo);
	
	// ����Ϣ���ͻ���
	int		nLength;
	SHOW_MSG_SYNC	sMsg;
	
	nLength = strlen(Npc[Player[nPlayerNo].m_nIndex].Name);
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_KICK_One;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + nLength;
	sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];
	
	memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
	memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), Npc[Player[nPlayerNo].m_nIndex].Name, nLength);
	
	g_pServer->PackDataToClient(Player[nPlayerNo].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	
	g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	for (int i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] > 0)
			g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
	}
	sMsg.Release();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ��˶ӳ�֪ͨ�Ѷӳ����ݽ���ĳ����Ա
//-------------------------------------------------------------------------
void	KPlayer::TeamChangeCaptain(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	int		i, nMember, nPlayerNo, nIsOpen = 0;
	PLAYER_APPLY_TEAM_CHANGE_CAPTAIN	*pChange = (PLAYER_APPLY_TEAM_CHANGE_CAPTAIN*)pProtocol;
	nMember = g_Team[m_cTeam.m_nID].FindMemberID(pChange->m_dwNpcID);
	if (nMember < 0)
		return;
	if (Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[nMember]].m_nIndex].m_Camp == camp_begin && Npc[this->m_nIndex].m_Camp != camp_begin)
	{
		// �ӳ��ƽ�ʧ�ܣ��Է�ͳ˧������
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL2;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)pChange->m_dwNpcID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	if (g_Team[m_cTeam.m_nID].m_nMemNum > PlayerSet.m_cLeadExp.GetMemNumFromLevel(Player[g_Team[m_cTeam.m_nID].m_nMember[nMember]].m_dwLeadLevel))
	{
		// �ӳ��ƽ�ʧ�ܣ��Է�ͳ˧������
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TEAM_CHANGE_CAPTAIN_FAIL;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)pChange->m_dwNpcID;
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	if (g_Team[m_cTeam.m_nID].IsOpen())
	{
		g_Team[m_cTeam.m_nID].SetTeamClose();
		nIsOpen = 1;
	}
	nPlayerNo = g_Team[m_cTeam.m_nID].m_nCaptain;
	g_Team[m_cTeam.m_nID].m_nCaptain = g_Team[m_cTeam.m_nID].m_nMember[nMember];
	g_Team[m_cTeam.m_nID].m_nMember[nMember] = nPlayerNo;
	m_cTeam.m_nFigure = TEAM_MEMBER;
	Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_cTeam.m_nFigure = TEAM_CAPTAIN;
	// �ı���Ӫ
	Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].RestoreCurrentCamp();
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[i];
		if (nPlayerNo < 0)
			continue;
		Npc[Player[nPlayerNo].m_nIndex].SetCurrentCamp(Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].m_Camp);
	}
	
	// ��������Ա����Ϣ
	PLAYER_TEAM_CHANGE_CAPTAIN	sChangeCaptain;
	sChangeCaptain.ProtocolType = s2c_teamchangecaptain;
	sChangeCaptain.m_dwCaptainID = pChange->m_dwNpcID;
	sChangeCaptain.m_dwMemberID = Npc[m_nIndex].m_dwID;
	// ���ӳ�����Ϣ
	g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, (BYTE*)&sChangeCaptain, sizeof(PLAYER_TEAM_CHANGE_CAPTAIN));
	// ����Ա����Ϣ
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		nPlayerNo = g_Team[m_cTeam.m_nID].m_nMember[i];
		if (nPlayerNo < 0)
			continue;
		g_pServer->PackDataToClient(Player[nPlayerNo].m_nNetConnectIdx, (BYTE*)&sChangeCaptain, sizeof(PLAYER_TEAM_CHANGE_CAPTAIN));
	}
	
	if (nIsOpen)
	{
		g_Team[m_cTeam.m_nID].SetTeamOpen();
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ��˶ӳ������ɢ����
//-------------------------------------------------------------------------
void	KPlayer::TeamDismiss(BYTE* pProtocol)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_LEAVE_TEAM	sLeaveTeam;
	int		i;
	SHOW_MSG_SYNC	sMsg;
	
	// �����ɢ��Ϣ
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TEAM_DISMISS;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
	g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	
	// ���ӳ�����Ϣ
	sLeaveTeam.ProtocolType = s2c_teamleave;
	sLeaveTeam.m_dwNpcID = Npc[m_nIndex].m_dwID;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));
	// ����Ա����Ϣ
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		
		sLeaveTeam.m_dwNpcID = Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].m_dwID;
		g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sLeaveTeam, sizeof(PLAYER_LEAVE_TEAM));
		// ��Ա���ݸı�
		Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_cTeam.m_nFlag = 0;
		Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].RestoreCurrentCamp();;
	}
	// �������
	g_Team[m_cTeam.m_nID].Release();
	// �ӳ����ݸı�
	m_cTeam.m_nFlag = 0;
	Npc[m_nIndex].RestoreCurrentCamp();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ��������趨PK״̬
//-------------------------------------------------------------------------
void	KPlayer::SetPK(BYTE* pProtocol)
{
	PLAYER_SET_PK	*pPK = (PLAYER_SET_PK*)pProtocol;
	
	if (pPK->m_btPKFlag)
		Npc[m_nIndex].SetCurrentCamp(camp_free);
	else
		Npc[m_nIndex].RestoreCurrentCamp();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ�����������������
//-------------------------------------------------------------------------
void	KPlayer::SendFactionData(BYTE* pProtocol)
{
	SendFactionData();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ���ͻ��˷�����������
//-------------------------------------------------------------------------
void	KPlayer::SendFactionData()
{
	// ���ͻ��˷���Ϣ
	PLAYER_FACTION_DATA	sData;
	sData.ProtocolType = s2c_playerfactiondata;
	sData.m_btCamp = Npc[m_nIndex].m_Camp;
	sData.m_btCurFaction = m_cFaction.m_nCurFaction;
	sData.m_btFirstFaction = m_cFaction.m_nFirstAddFaction;
	sData.m_nAddTimes = m_cFaction.m_nAddTimes;
	
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sData, sizeof(PLAYER_FACTION_DATA));
}
#endif

#ifdef _SERVER
// not end
void KPlayer::GetAboutPos(KMapPos *pMapPos)
{
	if (m_nIndex <= 0)
		return;
	
	if (Npc[m_nIndex].m_SubWorldIndex < 0)
		return;
	
	POINT Pos;
	int nX, nY;
	Npc[m_nIndex].GetMpsPos(&nX, &nY);
	Pos.x = nX;
	Pos.y = nY;
	
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].GetFreeObjPos(Pos);
	
	pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Mps2Map(
		Pos.x, 
		Pos.y, 
		&pMapPos->nRegion, 
		&pMapPos->nMapX, 
		&pMapPos->nMapY, 
		&pMapPos->nOffX, 
		&pMapPos->nOffY);
}

/*
void	KPlayer::GetAboutPos(KMapPos *pMapPos)
{
POINT	Pos[8] = 
{
{0, 32}, {-32, 32}, {-32, 0}, {-32, -32},
{0, -32}, {32, -32}, {32, 0}, {32, 32},
};

  int nMpsX, nMpsY, nTmpX, nTmpY;
  int nR, nMapX, nMapY, nOffX, nOffY;
  Npc[m_nIndex].GetMpsPos(&nMpsX, &nMpsY);
  
	for (int i = 0; i < 8; i++)
	{
	nTmpX = nMpsX + Pos[i].x;
	nTmpY = nMpsY + Pos[i].y;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].GetBarrier(nTmpX, nTmpY))
	continue;
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Mps2Map(nTmpX, nTmpY, &nR, &nMapX, &nMapY, &nOffX, &nOffY);
	if (nR == -1)
	continue;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nR].GetRef(nMapX, nMapY, obj_object))
	continue;
	else
	break;
	}
	
	  if (i == 8)
	  {
	  pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	  pMapPos->nRegion = Npc[m_nIndex].m_RegionIndex;
	  pMapPos->nMapX = Npc[m_nIndex].m_MapX;
	  pMapPos->nMapY = Npc[m_nIndex].m_MapY;
	  pMapPos->nOffX = Npc[m_nIndex].m_OffX;
	  pMapPos->nOffY = Npc[m_nIndex].m_OffY;
	  }
	  else
	  {
	  pMapPos->nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	  pMapPos->nRegion = nR;
	  pMapPos->nMapX = nMapX;
	  pMapPos->nMapY = nMapY;
	  pMapPos->nOffX = nOffX;
	  pMapPos->nOffY = nOffY;
	  }
}*/
#endif


#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�Ѱ�������Χ��ĳ��ָ��npc id��player index
//-------------------------------------------------------------------------
int		KPlayer::FindAroundPlayer(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return -1;
	
	int		nPlayer, nRegionNo, i;
	nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].FindPlayer(dwNpcID);
	if ( nPlayer >= 0)
		return nPlayer;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nPlayer = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].FindPlayer(dwNpcID);
		if (nPlayer >= 0)
			return nPlayer;
	}
	
	return -1;
}

//-------------------------------------------------------------------------
//	���ܣ��ж�ĳ����Ƿ�����Χ
//-------------------------------------------------------------------------
BOOL	KPlayer::CheckPlayerAround(int nPlayerIdx)
{
	if (nPlayerIdx <= 0)
		return FALSE;
	if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].CheckPlayerIn(nPlayerIdx))
		return TRUE;
	int		nRegionNo;
	for (int i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		if (SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].CheckPlayerIn(nPlayerIdx))
			return TRUE;
	}
	
	return FALSE;
}

//-------------------------------------------------------------------------
//	���ܣ�Ѱ�������Χ��ĳ��ָ��npc id��npc index
//-------------------------------------------------------------------------
int		KPlayer::FindAroundNpc(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return 0;
	
	int		nNpc, nRegionNo, i;
	nNpc = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].SearchNpc(dwNpcID);
	if ( nNpc > 0)
		return nNpc;
	for (i = 0; i < 8; i++)
	{
		nRegionNo = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[Npc[m_nIndex].m_RegionIndex].m_nConnectRegion[i];
		if ( nRegionNo < 0)
			continue;
		nNpc = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_Region[nRegionNo].SearchNpc(dwNpcID);
		if (nNpc > 0)
			return nNpc;
	}
	
	return 0;
}
#endif

//-------------------------------------------------------------------------
//	���ܣ�����ҵ�½ʱ�����������Բ��� ���� ���� ���� ���� ������ֵ
//-------------------------------------------------------------------------
BOOL	KPlayer::NewPlayerGetBaseAttribute(int Series)
{
	if (Series < series_metal || Series > series_earth)
		return FALSE;
	
	Npc[m_nIndex].SetSeries(Series);
	
	m_nStrength = PlayerSet.m_cNewPlayerAttribute.m_nStrength[Series];
	m_nDexterity = PlayerSet.m_cNewPlayerAttribute.m_nDexterity[Series];
	m_nVitality = PlayerSet.m_cNewPlayerAttribute.m_nVitality[Series];
	m_nEngergy = PlayerSet.m_cNewPlayerAttribute.m_nEngergy[Series];
	m_nLucky = PlayerSet.m_cNewPlayerAttribute.m_nLucky[Series];
	m_nUpExp = 0;
	
	m_nCurStrength = m_nStrength;
	m_nCurDexterity = m_nDexterity;
	m_nCurVitality = m_nVitality;
	m_nCurEngergy = m_nEngergy;
	CalcCurLucky();
	
	m_cFaction.SetSeries(Series);
	
	return TRUE;
}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurStrength()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurStrength = m_nStrength;
//}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurDexterity()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurDexterity = m_nDexterity;
//}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurVitality()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurVitality = m_nVitality;
//}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
//void	KPlayer::CalcCurEngergy()
//{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
//	m_nCurEngergy = m_nEngergy;
//}

//-------------------------------------------------------------------------
//	���ܣ����㵱ǰ����
//-------------------------------------------------------------------------
void	KPlayer::CalcCurLucky()
{	// ����Ҫ���� װ�������ܡ�״̬ ��Ӱ��
	m_nCurLucky = m_nLucky;
}

#ifdef _SERVER
void  KPlayer::SyncCurrentBaseAttriibute(int type,int attribute,int curAttribute)
{	
	PLAYER_ATTRIBUTE_SYNC	sSync;
	sSync.ProtocolType = s2c_playersyncattribute;
	sSync.m_btAttribute = type;
	sSync.m_nBasePoint = attribute;
	sSync.m_nCurPoint = curAttribute;
	sSync.m_nLeavePoint = m_nAttributePoint;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
}
//-------------------------------------------------------------------------
//	���ܣ����ӻ�������
//-------------------------------------------------------------------------
void	KPlayer::AddBaseStrength(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nStrength += nData;
		m_nAttributePoint -= nData;
		m_nCurStrength += nData;
		
		UpdataCurData();
		SetNpcPhysicsDamage();
		// �ѵ�ǰ���������ͻ���
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 0;
		sSync.m_nBasePoint = m_nStrength;
		sSync.m_nCurPoint = m_nCurStrength;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
void	KPlayer::ResetBaseStrength(int nData)
{		
	m_nStrength = nData;	
	m_nCurStrength = nData;
	UpdataCurData();
	SetNpcPhysicsDamage();
	SyncCurrentBaseAttriibute(0,m_nStrength,m_nCurStrength);	
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ����ӻ�������
//-------------------------------------------------------------------------
void	KPlayer::AddBaseDexterity(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nDexterity += nData;
		m_nAttributePoint -= nData;
		m_nCurDexterity += nData;
		
		SetNpcAttackRating();
		SetNpcDefence();
		UpdataCurData();
		SetNpcPhysicsDamage();
		
		// �ѵ�ǰ���������ͻ���
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 1;
		sSync.m_nBasePoint = m_nDexterity;
		sSync.m_nCurPoint = m_nCurDexterity;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
void	KPlayer::ResetBaseDexterity(int nData)
{
		m_nDexterity = nData;		
		m_nCurDexterity = nData;

		SetNpcAttackRating();
		SetNpcDefence();
		UpdataCurData();
		SetNpcPhysicsDamage();

		// �ѵ�ǰ���������ͻ���
		SyncCurrentBaseAttriibute(1,m_nDexterity,m_nCurDexterity);

}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ����ӻ�������
//-------------------------------------------------------------------------
void	KPlayer::AddBaseVitality(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nVitality += nData;
		m_nAttributePoint -= nData;
		m_nCurVitality += nData;
		
		Npc[m_nIndex].AddBaseLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
		Npc[m_nIndex].AddBaseStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		
		// �ѵ�ǰ���������ͻ���
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 2;
		sSync.m_nBasePoint = m_nVitality;
		sSync.m_nCurPoint = m_nCurVitality;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
void	KPlayer::ResetBaseVitality(int nData)
{	
	m_nVitality = nData;		
	m_nCurVitality = nData;

	Npc[m_nIndex].SetBaseLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
	Npc[m_nIndex].SetBaseStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
	UpdataCurData();

	// �ѵ�ǰ���������ͻ���
	SyncCurrentBaseAttriibute(2,m_nVitality,m_nCurVitality);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ����ӻ�������
//-------------------------------------------------------------------------
void	KPlayer::AddBaseEngergy(int nData)
{
	if (nData > 0 && nData <= m_nAttributePoint)
	{
		m_nEngergy += nData;
		m_nAttributePoint -= nData;
		m_nCurEngergy += nData;
		
		Npc[m_nIndex].AddBaseManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		
		// �ѵ�ǰ���������ͻ���
		PLAYER_ATTRIBUTE_SYNC	sSync;
		sSync.ProtocolType = s2c_playersyncattribute;
		sSync.m_btAttribute = 3;
		sSync.m_nBasePoint = m_nEngergy;
		sSync.m_nCurPoint = m_nCurEngergy;
		sSync.m_nLeavePoint = m_nAttributePoint;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(PLAYER_ATTRIBUTE_SYNC));
	}
}
void	KPlayer::ResetBaseEngergy(int nData)
{
	m_nEngergy = nData;		
	m_nCurEngergy = nData;

	Npc[m_nIndex].SetBaseManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
	UpdataCurData();

	// �ѵ�ǰ���������ͻ���
	SyncCurrentBaseAttriibute(3,m_nEngergy,m_nCurEngergy);
}
#endif

//-------------------------------------------------------------------------
//	���ܣ��ı䵱ǰ����(�� nData С�� 0 ʱ������)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurStrength(int nData)
{
/*	if (m_nCurStrength + nData < m_nStrength)
return;
	*/
	m_nCurStrength += nData;
	SetNpcPhysicsDamage();
	// �ѵ�ǰ���������ͻ���
}

//-------------------------------------------------------------------------
//	���ܣ��ı䵱ǰ����(�� nData С�� 0 ʱ������)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurDexterity(int nData)
{
/*	if (m_nCurDexterity + nData < 0)
return;
	*/
	m_nCurDexterity += nData;
	
	int nRating = nData * 4;
	Npc[m_nIndex].m_CurrentAttackRating += nRating;
	
	int nDefence = nData / 4;
	Npc[m_nIndex].m_CurrentDefend += nDefence;
	
	SetNpcPhysicsDamage();
	
}


//-------------------------------------------------------------------------
//	���ܣ��ı䵱ǰ����(�� nData С�� 0 ʱ������)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurVitality(int nData)
{
/*	if (nData + m_nAttributePoint < 0)
return;
	*/
	m_nCurVitality += nData;
	
	Npc[m_nIndex].AddCurLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
	Npc[m_nIndex].AddCurStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
	
}

//-------------------------------------------------------------------------
//	���ܣ��ı䵱ǰ����(�� nData С�� 0 ʱ������)
//-------------------------------------------------------------------------
void	KPlayer::ChangeCurEngergy(int nData)
{
/*	if (m_nAttributePoint + nData < 0)
return;
	*/
	m_nCurEngergy += nData;
	
	Npc[m_nIndex].AddCurManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
	
}

//-------------------------------------------------------------------------
//	���ܣ����ӻ�������
//-------------------------------------------------------------------------
void	KPlayer::AddBaseLucky(int nData)
{
	m_nLucky += nData;
	CalcCurLucky();
}

//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���������Ӧnpc�������˺�(PhysicsDamage)
//-------------------------------------------------------------------------
void	KPlayer::SetNpcPhysicsDamage()
{
	int		nMinDamage, nMaxDamage;
	m_ItemList.GetWeaponDamage(&nMinDamage, &nMaxDamage);
	
	if (m_ItemList.GetWeaponType() == equip_meleeweapon)	// (��ǰװ���ǽ�������)
	{
	/*		nMinDamage = nMinDamage * (m_nCurStrength + STRENGTH_SET_DAMAGE_VALUE) / STRENGTH_SET_DAMAGE_VALUE;
	nMaxDamage = nMaxDamage * (m_nCurStrength + STRENGTH_SET_DAMAGE_VALUE) / STRENGTH_SET_DAMAGE_VALUE;
		*/
		// ��ֵ���㷽���޸ģ���ӢҪ�� by Spe 03/06/11
		nMinDamage += m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE;
		nMaxDamage += m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE;
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
	else if (m_ItemList.GetWeaponType() == equip_rangeweapon)	// (��ǰװ����Զ������)
	{
	/*		nMinDamage = nMinDamage * (m_nCurDexterity + DEXTERITY_SET_DAMAGE_VALUE) / DEXTERITY_SET_DAMAGE_VALUE;
	nMaxDamage = nMaxDamage * (m_nCurDexterity + DEXTERITY_SET_DAMAGE_VALUE) / DEXTERITY_SET_DAMAGE_VALUE;
		*/
		// ��ֵ���㷽���޸ģ���ӢҪ�� by Spe 03/06/11
		nMinDamage += m_nCurDexterity / DEXTERITY_SET_DAMAGE_VALUE;
		nMaxDamage += m_nCurDexterity / DEXTERITY_SET_DAMAGE_VALUE;
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
	else
	{
		Npc[m_nIndex].SetPhysicsDamage(nMinDamage, nMaxDamage);
	}
}

//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���ݼ����Ӧnpc�Ĺ���������(AttackRating)
//-------------------------------------------------------------------------
void	KPlayer::SetNpcAttackRating()
{
	int		nRating;
	
	nRating = m_nDexterity * 4 - 28;
	Npc[m_nIndex].SetBaseAttackRating(nRating);
}

//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���ݼ����Ӧnpc�ķ�����
//-------------------------------------------------------------------------
void	KPlayer::SetNpcDefence()
{
	int		nDefence;
	
	nDefence = m_nDexterity / 4;
	Npc[m_nIndex].SetBaseDefence(nDefence);
}

/*
#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���ݼ����Ӧnpc�������ٶ�
//-------------------------------------------------------------------------
void	KPlayer::SetNpcWalkSpeed()
{
int		nSpeed;
nSpeed = BASE_WALK_SPEED * (m_nCurDexterity + 320) / 320;
Npc[m_nIndex].SetBaseWalkSpeed(nSpeed);
}
#endif
*/

/*
#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ɵ�ǰ���ݼ����Ӧnpc���ܲ��ٶ�
//-------------------------------------------------------------------------
void	KPlayer::SetNpcRunSpeed()
{
int		nSpeed;
nSpeed = BASE_RUN_SPEED * (m_nCurDexterity + 320) / 320;
Npc[m_nIndex].SetBaseRunSpeed(nSpeed);
}
#endif
*/

//-------------------------------------------------------------------------
//	���ܣ����Ӿ���(ԭʼ���ݣ���δ������������Ҫ������ӵ�Ӱ��)
//-------------------------------------------------------------------------
void	KPlayer::AddExp(int nExp, int nTarLevel)
{
#ifndef _SERVER
	AddSelfExp(nExp, nTarLevel);
#endif

#ifdef _SERVER
	// û�����
	if (g_GlobalMissionArray.GetMissionValue(27) == 2)	// nhan 2 exp toan sv
	{
		AddSelfExp(nExp*2, nTarLevel);
		return;		
	}
	else if (g_GlobalMissionArray.GetMissionValue(27) == 3)	// nhan 3 exp toan sv
	{
		AddSelfExp(nExp*3, nTarLevel);
		return;		
	}
	else if (g_GlobalMissionArray.GetMissionValue(27) == 4)	// nhan 4 exp toan sv
	{
		AddSelfExp(nExp*4, nTarLevel);
		return;		
	}
	else if (g_GlobalMissionArray.GetMissionValue(27) == 5)	// nhan 5 exp toan sv
	{
		AddSelfExp(nExp*5, nTarLevel);
		return;		
	}
	if ( !m_cTeam.m_nFlag )
	{
		AddSelfExp(nExp, nTarLevel);
		return;
	}
	
	// �Ѿ����
	int		i, j, nShareFlag[MAX_TEAM_MEMBER + 1], nRegion, nTotalLevel = 0, nTotalPlayer = 0;
	memset(nShareFlag, 0, sizeof(nShareFlag));
	
	int		nMpsX, nMpsY, nNpcIdx, nSelfSubIdx, nSelfMpsX, nSelfMpsY;
	
	nSelfSubIdx = Npc[m_nIndex].m_SubWorldIndex;
	SubWorld[nSelfSubIdx].Map2Mps(
		Npc[m_nIndex].m_RegionIndex,
		Npc[m_nIndex].m_MapX,
		Npc[m_nIndex].m_MapY,
		Npc[m_nIndex].m_OffX,
		Npc[m_nIndex].m_OffY,
		&nSelfMpsX,
		&nSelfMpsY);
	
	// �ӳ�
	nNpcIdx = Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex;
	if (nSelfSubIdx == Npc[nNpcIdx].m_SubWorldIndex)
	{
		SubWorld[Npc[nNpcIdx].m_SubWorldIndex].Map2Mps(
			Npc[nNpcIdx].m_RegionIndex,
			Npc[nNpcIdx].m_MapX,
			Npc[nNpcIdx].m_MapY,
			Npc[nNpcIdx].m_OffX,
			Npc[nNpcIdx].m_OffY,
			&nMpsX,
			&nMpsY);
		if ( (nMpsX - nSelfMpsX) * (nMpsX - nSelfMpsX) + 
			(nMpsY - nSelfMpsY) * (nMpsY - nSelfMpsY) <= 
			PLAYER_SHARE_EXP_DISTANCE * PLAYER_SHARE_EXP_DISTANCE)
		{
			nTotalLevel += Npc[nNpcIdx].m_Level;
			nShareFlag[0] = 1;
			nTotalPlayer++;
			if (nTotalPlayer >= g_Team[m_cTeam.m_nID].m_nMemNum + 1)
				goto SharePlace;
		}
	}
	// ��Ա
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if (g_Team[m_cTeam.m_nID].m_nMember[i] < 0)
			continue;
		nNpcIdx = Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex;
		if (nSelfSubIdx != Npc[nNpcIdx].m_SubWorldIndex)
			continue;
		SubWorld[Npc[nNpcIdx].m_SubWorldIndex].Map2Mps(
			Npc[nNpcIdx].m_RegionIndex,
			Npc[nNpcIdx].m_MapX,
			Npc[nNpcIdx].m_MapY,
			Npc[nNpcIdx].m_OffX,
			Npc[nNpcIdx].m_OffY,
			&nMpsX,
			&nMpsY);
		if ( (nMpsX - nSelfMpsX) * (nMpsX - nSelfMpsX) + 
			(nMpsY - nSelfMpsY) * (nMpsY - nSelfMpsY) > 
			PLAYER_SHARE_EXP_DISTANCE * PLAYER_SHARE_EXP_DISTANCE)
			continue;
		nTotalLevel += Npc[nNpcIdx].m_Level;
		nShareFlag[i + 1] = 1;
		nTotalPlayer++;
		if (nTotalPlayer >= g_Team[m_cTeam.m_nID].m_nMemNum + 1)
			goto SharePlace;
	}
	
SharePlace:
	_ASSERT(nTotalPlayer >= 1);
	_ASSERT(nTotalLevel >= 1);
	if (nTotalPlayer < 1 || nTotalLevel < 1)
		return;
	if (nTotalPlayer == 1)
	{
		AddSelfExp(nExp, nTarLevel);
		return;
	}
	int		k, nGetExp;
	k = 100 + nTotalPlayer * 15;
	
	
	// ���ӳ����侭��
	if (nShareFlag[0])
	{
		nGetExp = k * Npc[Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nIndex].m_Level / nTotalLevel;
		if (nGetExp < 5)
			nGetExp = 5;
		if(g_Team[m_cTeam.m_nID].m_nCaptain == m_nPlayerIndex)
			Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp, nTarLevel);
		else
			Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		

		// LeadExp = �����ṩ�ľ���ֵ * �� * �����ߵȼ� / �����ߵȼ��ͣ�
		int		nLeadExp = nExp * k * PlayerSet.m_cLevelAdd.GetLeadExpShare(Npc[m_nIndex].m_Series) * (nTotalLevel - Npc[m_nIndex].m_Level) / (nTotalLevel * 10000);
		Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddLeadExp(nLeadExp);

		//		if (g_Team[m_cTeam.m_nID].m_nCaptain == m_nPlayerIndex)
		//		{
		//			if (nGetExp < 50)
		//				nGetExp = 50;
		//			Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		}
		//		else
		//		{
		//			if (nGetExp < 5)
		//				nGetExp = 5;
		//			Player[g_Team[m_cTeam.m_nID].m_nCaptain].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		}
	}
	// ����Ա���侭��
	for (i = 0; i < MAX_TEAM_MEMBER; i++)
	{
		if ( !nShareFlag[i + 1] )
			continue;
		nGetExp = k * Npc[Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nIndex].m_Level / nTotalLevel;
		if (nGetExp < 5)
			nGetExp = 5;
		if(g_Team[m_cTeam.m_nID].m_nMember[i] == m_nPlayerIndex)
			Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp, nTarLevel);
		else
			Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		if (g_Team[m_cTeam.m_nID].m_nMember[i] == m_nPlayerIndex)
		//		{
		//			if (nGetExp < 50)
		//				nGetExp = 50;
		//			Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		}
		//		else
		//		{
		//			if (nGetExp < 5)
		//				nGetExp = 5;
		//			Player[g_Team[m_cTeam.m_nID].m_nMember[i]].AddSelfExp(nExp * nGetExp / 100, nTarLevel);
		//		}
	}
	
#endif
}

//-------------------------------------------------------------------------
//	���ܣ����Ӿ���(����Ҫ�پ����������Ĵ���)
//  nExp ���ӵľ���  nTarLevel ��������npc�ĵȼ�
//	������� sourcesafe�߻���\document\System\ս�������趨.doc
//-------------------------------------------------------------------------
void	KPlayer::AddSelfExp(int nExp, int nTarLevel)
{
	if (nExp <= 0)
		return;
	if (Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive)
		return;
	nExp += nExp * m_nUpExp / 100;
	int		nSubLevel, nGetExp;
	//	int		nAbsLevel;

	nSubLevel = Npc[m_nIndex].m_Level - nTarLevel;
	if (nSubLevel <= 5)
	{
		nGetExp = nExp;
	}
	else if (nSubLevel <= 15)
	{
		//		nGetExp = nExp * (100 - (95 / 10) * (nSubLevel - 5)) / 100;
		nGetExp = nExp * (295 - 19 * nSubLevel) / 200;
	}
	else
	{
		nGetExp = nExp / 20;
	}
	
	/*
	nAbsLevel = abs(nTarLevel - Npc[m_nIndex].m_Level);
	
	  if (nAbsLevel <= 10)
	  {
	  nGetExp = nExp;
	  }
	  else if (nAbsLevel <= 20)
	  {
	  nGetExp = nExp * (30 - nAbsLevel) / 20;	// EXP=X*(150-|LM-LP|*5)/100
	  }
	  else
	  {
	  nGetExp = nExp / 2;
	  }
	*/
	if (nGetExp <= 0)
		nGetExp = 1;
	
	m_nExp += nGetExp;
	g_DebugLog("[��ֵ]%sʵ�����Ӿ���%d��", Npc[m_nIndex].Name, nGetExp);
#ifdef _SERVER
	if (m_nExp >= m_nNextLevelExp)	// ����
	{
		LevelUp();
	}
	else								// ͬ������
	{
		PLAYER_EXP_SYNC	sSyncExp;
		sSyncExp.ProtocolType = (BYTE)s2c_playerexp;
		sSyncExp.m_nExp = m_nExp;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSyncExp, sizeof(PLAYER_EXP_SYNC));
	}
#endif
}

//-------------------------------------------------------------------------
//	���ܣ�ֱ�����Ӿ���ֵ����������������
//-------------------------------------------------------------------------
void	KPlayer::DirectAddExp(int nExp)
{
	if (nExp > 0 && Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	
	m_nExp += nExp;
	if (m_nExp < 0)
	{
		if (-m_nExp > m_nNextLevelExp)
			m_nExp = -m_nNextLevelExp;
	}
#ifdef _SERVER
	if (m_nExp >= m_nNextLevelExp)	// ����
	{
		LevelUp();
	}
	else								// ͬ������
	{
		PLAYER_EXP_SYNC	sSyncExp;
		sSyncExp.ProtocolType = (BYTE)s2c_playerexp;
		sSyncExp.m_nExp = m_nExp;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSyncExp, sizeof(PLAYER_EXP_SYNC));
	}
#endif
}

//-------------------------------------------------------------------------
//	���ܣ������һ��
//-------------------------------------------------------------------------
void	KPlayer::LevelUp()
{
	m_nExp = 0;
	if (Npc[m_nIndex].m_Level >= MAX_LEVEL)
		return;
	Npc[m_nIndex].m_Level++;
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	

	// -------------------------- �ȼ�����ʱ������������ ----------------------
	// ����δ�������Ե�
	m_nAttributePoint += PLAYER_LEVEL_ADD_ATTRIBUTE;
	// ����δ���似�ܵ�
	m_nSkillPoint += PLAYER_LEVEL_ADD_SKILL;
	
	// ��������������������ֵ��Ӱ������������������������ֵ�����أ��ȼ������Ե㣬Ӱ�쵱ǰ�������ֵ�����أ�װ�������ܡ�ҩ���ʱ����
	LevelAddBaseLifeMax();
	LevelAddBaseStaminaMax();
	LevelAddBaseManaMax();
	//	Npc[m_nIndex].ResetLifeReplenish();
	
	// �������ֿ��Եı仯 �𡢱��������硢����
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;

	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	// ����װ����������Ϣ���µ�ǰ����
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;
	
	// ����������������ֵ����
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	
#ifdef _SERVER
	// ���ͻ��˷�������
	// �Լ�������
	PLAYER_LEVEL_UP_SYNC	sLevelUp;
	sLevelUp.ProtocolType		= s2c_playerlevelup;
	sLevelUp.m_btLevel			= (DWORD)Npc[m_nIndex].m_Level;
	sLevelUp.m_nExp				= m_nExp;
	sLevelUp.m_nAttributePoint	= m_nAttributePoint;
	sLevelUp.m_nSkillPoint		= m_nSkillPoint;
	sLevelUp.m_nBaseLifeMax		= Npc[m_nIndex].m_LifeMax;
	sLevelUp.m_nBaseStaminaMax	= Npc[m_nIndex].m_StaminaMax;
	sLevelUp.m_nBaseManaMax		= Npc[m_nIndex].m_ManaMax;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLevelUp, sizeof(PLAYER_LEVEL_UP_SYNC));
	// �����ѷ��͵ȼ�����
	if ( m_cTeam.m_nFlag )
	{
		PLAYER_TEAMMATE_LEVEL_SYNC	sLevel;
		sLevel.ProtocolType = s2c_teammatelevel;
		sLevel.m_btLevel = (DWORD)Npc[m_nIndex].m_Level;
		sLevel.m_dwTeammateID = Npc[m_nIndex].m_dwID;
		// ���ӳ���
		if (m_cTeam.m_nFigure != TEAM_CAPTAIN)
			g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nCaptain].m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_TEAMMATE_LEVEL_SYNC));
		for (int i = 0; i < MAX_TEAM_MEMBER; i++)
		{
			if (g_Team[m_cTeam.m_nID].m_nMember[i] >= 0 && g_Team[m_cTeam.m_nID].m_nMember[i] != GetPlayerIndex())
				g_pServer->PackDataToClient(Player[g_Team[m_cTeam.m_nID].m_nMember[i]].m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_TEAMMATE_LEVEL_SYNC));
		}
	}
	ExecuteScript("\\script\\system\\levelup.lua","main","");
#endif
}

// need spe edit not end
void	KPlayer::UpdataCurData()
{
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC)
		return;

	for (int j = 0 ; j< MAX_NPCSKILL;j++)
	{
		int nAdd = 	Npc[m_nIndex].m_SkillList.GetAddPoint(j);
		if (nAdd > 0)
		{
			nAdd = -nAdd;
#ifdef _SERVER	
			Npc[m_nIndex].m_SkillList.IncreaseLevel(j,nAdd);
#endif
			Npc[m_nIndex].m_SkillList.QeuipAddPoint(j,nAdd);
		}
		Npc[m_nIndex].m_SkillList.m_Skills[j].AddPoint = 0;
		Npc[m_nIndex].m_SkillList.m_Skills[j].EnChance = 0;
	}

	ZeroMemory(&Npc[m_nIndex].m_MagicPhysicsDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentLifeMax		= Npc[m_nIndex].m_LifeMax;
	Npc[m_nIndex].m_CurrentManaMax		= Npc[m_nIndex].m_ManaMax;
	Npc[m_nIndex].m_CurrentStaminaMax	= Npc[m_nIndex].m_StaminaMax;
	Npc[m_nIndex].m_CurrentAttackRating	= Npc[m_nIndex].m_AttackRating;
	Npc[m_nIndex].m_CurrentAttackSpeed	= Npc[m_nIndex].m_AttackSpeed;
//	Npc[m_nIndex].m_CurrentCamp			= Npc[m_nIndex].m_Camp;
	Npc[m_nIndex].m_CurrentCastSpeed	= Npc[m_nIndex].m_CastSpeed;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicColdDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentColdDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentColdEnhance	= 0;
	Npc[m_nIndex].m_CurrentColdResist	= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentDamage2Mana	= 0;
	Npc[m_nIndex].m_CurrentDamageReduce	= 0;
	Npc[m_nIndex].m_CurrentDeadlyStrike	= 0;
	Npc[m_nIndex].m_CurrentDefend		= Npc[m_nIndex].m_Defend;
	Npc[m_nIndex].m_CurrentElementDamageReduce = 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicFireDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentFireDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentFireEnhance	= 0;
	Npc[m_nIndex].m_CurrentFireResist	= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentHandEnhance	= 0;
	Npc[m_nIndex].m_CurrentHitRecover	= Npc[m_nIndex].m_HitRecover;
	Npc[m_nIndex].m_CurrentJumpSpeed	= Npc[m_nIndex].m_JumpSpeed;
	Npc[m_nIndex].m_CurrentKnockBack	= 0;
	Npc[m_nIndex].m_CurrentLifeReplenish	= Npc[m_nIndex].m_LifeReplenish;
	Npc[m_nIndex].m_CurrentLifeStolen	= 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicLightDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentLightDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentLightEnhance	= 0;
	Npc[m_nIndex].m_CurrentLightResist	= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_CurrentLightResistMax = Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentManaPerEnemy	= 0;
	Npc[m_nIndex].m_CurrentManaReplenish	= Npc[m_nIndex].m_ManaReplenish;
	Npc[m_nIndex].m_CurrentManaStolen	= 0;
	Npc[m_nIndex].m_CurrentMeleeDmgRet	= 0;
	Npc[m_nIndex].m_CurrentMeleeDmgRetPercent = 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMeleeEnhance, sizeof(Npc[m_nIndex].m_CurrentMeleeEnhance));
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
	Npc[m_nIndex].m_CurrentPiercePercent	= 0;
	ZeroMemory(&Npc[m_nIndex].m_CurrentMagicPoisonDamage, sizeof(KMagicAttrib));
	ZeroMemory(&Npc[m_nIndex].m_CurrentPoisonDamage, sizeof(KMagicAttrib));
	Npc[m_nIndex].m_CurrentPoisonEnhance	= 0;
	Npc[m_nIndex].m_CurrentPoisonResist	= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentRangeDmgRet	= 0;
	Npc[m_nIndex].m_CurrentRangeDmgRetPercent	= 0;
	Npc[m_nIndex].m_CurrentRangeEnhance	= 0;
	Npc[m_nIndex].m_CurrentRunSpeed	= Npc[m_nIndex].m_RunSpeed;
	Npc[m_nIndex].m_CurrentSlowMissle	= 0;
	Npc[m_nIndex].m_CurrentStaminaGain	= Npc[m_nIndex].m_StaminaGain;
	Npc[m_nIndex].m_CurrentStaminaLoss	= Npc[m_nIndex].m_StaminaLoss;
	Npc[m_nIndex].m_CurrentStaminaStolen	= 0;
	Npc[m_nIndex].m_CurrentVisionRadius	= Npc[m_nIndex].m_VisionRadius;
	Npc[m_nIndex].m_CurrentWalkSpeed	= Npc[m_nIndex].m_WalkSpeed;
	Npc[m_nIndex].m_CurrentAddPhysicsDamage = 0;
	m_nCurStrength						= m_nStrength;
	m_nCurDexterity						= m_nDexterity;
	m_nCurVitality						= m_nVitality;
	m_nCurEngergy						= m_nEngergy;
	m_nCurLucky							= m_nLucky;
	m_nUpExp							= 0;
	Npc[m_nIndex].m_CurrentFreezeTimeReducePercent = 0;
	Npc[m_nIndex].m_CurrentPoisonTimeReducePercent = 0;
	Npc[m_nIndex].m_CurrentStunTimeReducePercent = 0;
	Npc[m_nIndex].m_CurrentSerisesEnhance = 0;
	Npc[m_nIndex].m_CurrentDamageReduce = 0;
	ReCalcEquip();
	ReCalcState();
}

void KPlayer::ReCalcEquip()
{
	_ASSERT(m_nIndex > 0 && m_nIndex < MAX_NPC);
	
	for (int i = 0; i < itempart_num; i++)
	{
		int nIdx = m_ItemList.m_EquipItem[i];
		if (!nIdx)
			continue;
		int nActive = m_ItemList.GetEquipEnhance(i);

		if (m_ItemList.m_bActiveSet)
		{
			nActive = 3;
		}

		if (i != itempart_horse || Npc[m_nIndex].m_bRideHorse)
				Item[nIdx].ApplyMagicAttribToNPC(&Npc[m_nIndex], nActive);
	}
}

void KPlayer::ReCalcState()
{
	_ASSERT(m_nIndex > 0 && m_nIndex < MAX_NPC);
	
	Npc[m_nIndex].ReCalcStateEffect();
	Npc[m_nIndex].m_SkillList.ReEnChance();
}

//-------------------------------------------------------------------------
//	���ܣ�����ͳ��������
//-------------------------------------------------------------------------
void	KPlayer::AddLeadExp(int nExp)
{
	if (m_dwLeadLevel >= MAX_LEAD_LEVEL)
		return;
	if (Npc[m_nIndex].m_Doing == do_death || Npc[m_nIndex].m_Doing == do_revive)
		return;
	m_dwLeadExp += nExp;
	if (m_dwLeadExp >= (DWORD)PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel))
	{
		// ����
		m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(m_dwLeadExp, m_dwLeadLevel);
#ifndef _SERVER
		m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel);
#endif
	}
	
#ifdef _SERVER
	PLAYER_LEAD_EXP_SYNC	sLeadExp;
	sLeadExp.ProtocolType = s2c_playersyncleadexp;
	sLeadExp.m_dwLeadExp = m_dwLeadExp;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeadExp, sizeof(PLAYER_LEAD_EXP_SYNC));
#endif
	
}

//-------------------------------------------------------------------------
//	���ܣ������һ��������
//-------------------------------------------------------------------------
void KPlayer::LevelAddBaseLifeMax()
{
	Npc[m_nIndex].m_LifeMax += PlayerSet.m_cLevelAdd.GetLifePerLevel(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
}

//-------------------------------------------------------------------------
//	���ܣ������һ��������
//-------------------------------------------------------------------------
void KPlayer::LevelAddBaseStaminaMax()
{
	Npc[m_nIndex].m_StaminaMax += PlayerSet.m_cLevelAdd.GetStaminaPerLevel(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
}

//-------------------------------------------------------------------------
//	���ܣ������һ��������
//-------------------------------------------------------------------------
void KPlayer::LevelAddBaseManaMax()
{
	Npc[m_nIndex].m_ManaMax += PlayerSet.m_cLevelAdd.GetManaPerLevel(Npc[m_nIndex].m_Series);
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
}

//-------------------------------------------------------------------------
//	���ܣ��ı������Ӫ
//-------------------------------------------------------------------------
void	KPlayer::ChangePlayerCamp(int nCamp)
{
	if (nCamp < camp_begin || nCamp >= camp_num)
		return;
	Npc[m_nIndex].m_Camp = (NPCCAMP)nCamp;
}

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�������������ѯĳ��npc���ڶ������Ϣ
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamInfo(DWORD dwNpcID)
{
	if (dwNpcID <= 0)
		return;
	PLAYER_APPLY_TEAM_INFO	sApplyInfo;
	sApplyInfo.ProtocolType = (BYTE)c2s_teamapplyinfo;
	sApplyInfo.m_dwTarNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApplyInfo, sizeof(PLAYER_APPLY_TEAM_INFO));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�������������ѯ��������Ķ������
//-------------------------------------------------------------------------
void	KPlayer::ApplySelfTeamInfo()
{
	ApplyTeamInfo(Npc[m_nIndex].m_dwID);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��������������봴������
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyCreateTeam()//char *lpszTeamName)
{
	return m_cTeam.ApplyCreate();//lpszTeamName);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ӳ�����������뿪�š��رն����Ƿ����������Ա״̬
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyTeamOpenClose(BOOL bFlag)
{
	if ( !m_cTeam.m_nFlag )					// δ���
		return FALSE;
	if (m_cTeam.m_nFigure != TEAM_CAPTAIN)	// ���Ƕӳ�
		return FALSE;
	if (this->CheckTrading())	// ���ڽ���״̬
		return FALSE;
	
	if (bFlag)
	{
		if (PlayerSet.m_cLeadExp.GetMemNumFromLevel(m_dwLeadLevel) <= g_Team[0].m_nMemNum)	// ͳ˧������
			return FALSE;
	}
	
	PLAYER_TEAM_OPEN_CLOSE	sTeamState;
	sTeamState.ProtocolType = c2s_teamapplyopenclose;
	sTeamState.m_btOpenClose = bFlag;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamState, sizeof(PLAYER_TEAM_OPEN_CLOSE));
	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ������������������ĳ������
//	������int nNpcIndex Ŀ�����ӳ��ڿͻ���Npc�����е�λ��
//-------------------------------------------------------------------------
void	KPlayer::ApplyAddTeam(int nNpcIndex)
{
	if (this->CheckTrading())
		return;
	if (m_cTeam.m_nFlag)					// �Ѿ���ӣ�������������Ķ���
		return;
	
	// ��ͬ��Ӫ���ڿ�����ӣ����ֲ��ܼ������˶��飬���˿��Լ������ֶ���
	if (Npc[nNpcIndex].m_Camp == camp_begin && Npc[m_nIndex].m_Camp != camp_begin)
		return;
	//	if (Npc[nNpcIndex].m_Camp != Npc[m_nIndex].m_Camp)	// ��Ӫ��ͬ
	//		return;
	
	// �ж�Ŀ�����ӳ��ǲ��Ǵ��ڶ��鿪��״̬
	if (Npc[nNpcIndex].GetMenuState() != PLAYER_MENU_STATE_TEAMOPEN)
		return;
	
	m_cTeam.m_nApplyCaptainID = Npc[nNpcIndex].m_dwID;
	m_cTeam.m_dwApplyTimer = MAX_APPLY_TEAM_TIME;
	PLAYER_APPLY_ADD_TEAM	sAddTeam;
	sAddTeam.ProtocolType = c2s_teamapplyadd;
	sAddTeam.m_dwTarNpcID = Npc[nNpcIndex].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAddTeam, sizeof(PLAYER_APPLY_ADD_TEAM));
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_APPLY_ADD_SELF_MSG, Npc[nNpcIndex].Name);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ����֪ͨ����������ĳ��npcΪ�����Ա
//-------------------------------------------------------------------------
void	KPlayer::AcceptTeamMember(DWORD dwNpcID)
{
	// �ж�״̬ ��ȱ�ٶ�npc���鿪��״̬�ļ�� not end��
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		ApplySelfTeamInfo();
		return;
	}
	if (dwNpcID == 0)
		return;
	
	// ���������б�
	int		i;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
			m_cTeam.UpdateInterface();
			break;
		}
	}
	// ���û�ҵ�
//	if (i >= MAX_TEAM_APPLY_LIST)
//		return;
	
	// �������������
	PLAYER_ACCEPT_TEAM_MEMBER	sAcceptTeam;
	sAcceptTeam.ProtocolType = c2s_teamacceptmember;
	sAcceptTeam.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAcceptTeam, sizeof(PLAYER_ACCEPT_TEAM_MEMBER));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ӳ�ɾ��������������б��е�ĳ��npc
//-------------------------------------------------------------------------
void	KPlayer::TeamDropApplyOne(DWORD dwNpcID)
{
	if (dwNpcID == 0)
		return;
	// ���������б�
	int		i;
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == dwNpcID)
		{
			m_cTeam.m_sApplyList[i].m_dwNpcID = 0;
			m_cTeam.m_sApplyList[i].m_dwTimer = 0;
			m_cTeam.m_sApplyList[i].m_nLevel = 0;
			m_cTeam.m_sApplyList[i].m_szName[0] = 0;
			break;
		}
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�֪ͨ������������뿪����
//-------------------------------------------------------------------------
void	KPlayer::LeaveTeam()
{
	PLAYER_APPLY_LEAVE_TEAM	sLeaveTeam;
	sLeaveTeam.ProtocolType = c2s_teamapplyleave;
	if (g_pClient)
		g_pClient->SendPackToServer(&sLeaveTeam, sizeof(PLAYER_APPLY_LEAVE_TEAM));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ӳ�֪ͨ�������߳�ĳ����Ա
//-------------------------------------------------------------------------
void	KPlayer::TeamKickMember(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_TEAM_KICK_MEMBER	sKickOne;
	sKickOne.ProtocolType = c2s_teamapplykickmember;
	sKickOne.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sKickOne, sizeof(PLAYER_TEAM_KICK_MEMBER));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ӳ��������������Լ��Ķӳ����ݽ�����Ķ�Ա
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamChangeCaptain(DWORD dwNpcID)
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_APPLY_TEAM_CHANGE_CAPTAIN	sTeamChange;
	sTeamChange.ProtocolType = c2s_teamapplychangecaptain;
	sTeamChange.m_dwNpcID = dwNpcID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamChange, sizeof(PLAYER_APPLY_TEAM_CHANGE_CAPTAIN));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ӳ�������������ɢ����
//-------------------------------------------------------------------------
void	KPlayer::ApplyTeamDismiss()
{
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
		return;
	
	PLAYER_APPLY_TEAM_DISMISS	sTeamDismiss;
	sTeamDismiss.ProtocolType = c2s_teamapplydismiss;
	if (g_pClient)
		g_pClient->SendPackToServer(&sTeamDismiss, sizeof(PLAYER_APPLY_TEAM_DISMISS));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ���������������򿪡��ر�pk����
//-------------------------------------------------------------------------
void	KPlayer::ApplySetPK(BOOL bPK)
{
	PLAYER_SET_PK	sPK;
	sPK.ProtocolType = c2s_playerapplysetpk;
	sPK.m_btPKFlag = bPK;
	if (g_pClient)
		g_pClient->SendPackToServer(&sPK, sizeof(PLAYER_SET_PK));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�����������������������
//-------------------------------------------------------------------------
void	KPlayer::ApplyFactionData()
{
	PLAYER_APPLY_FACTION_DATA	sFaction;
	sFaction.ProtocolType = c2s_playerapplyfactiondata;
	if (g_pClient)
		g_pClient->SendPackToServer(&sFaction, sizeof(PLAYER_APPLY_FACTION_DATA));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ͻ��˷�����������������
//-------------------------------------------------------------------------
void	KPlayer::SendChat(KUiMsgParam *pMsg, char *lpszSentence)
{
	if (!lpszSentence || !pMsg || pMsg->nMsgLength <= 0)
		return;
	int nLength = pMsg->nMsgLength;
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	
	if (pMsg->eGenre == MSG_G_CHAT)
	{
		Npc[m_nIndex].SetChatInfo(pMsg->szName, lpszSentence, nLength);
		
		m_cChat.SendSentence(pMsg, Npc[m_nIndex].m_szChatBuffer);
	}
	else if (pMsg->eGenre == MSG_G_CMD)	// not end
	{
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ӳ��������������������������ĳһ��ĵ���(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
//-------------------------------------------------------------------------
void	KPlayer::ApplyAddBaseAttribute(int nAttribute, int nNo)
{
	if (nAttribute < 0 || nAttribute > 3)
		return;
	if (nNo <= 0 || nNo >= 255)
		return;
	PLAYER_ADD_BASE_ATTRIBUTE_COMMAND	sAdd;
	sAdd.ProtocolType = c2s_playeraddbaseattribute;
	sAdd.m_btAttribute = nAttribute;
	sAdd.m_nAddNo = nNo;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(PLAYER_ADD_BASE_ATTRIBUTE_COMMAND));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ������������ĳ����������
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyAddSkillLevel(int nSkillID, int nAddPoint)
{
	if ( !Npc[m_nIndex].m_SkillList.FindSame(nSkillID) )
		return FALSE;
	if (m_nSkillPoint < nAddPoint || nAddPoint + Npc[m_nIndex].m_SkillList.GetLevel(nSkillID) > MAX_SKILLLEVEL)
		return FALSE;
	
	PLAYER_ADD_SKILL_POINT_COMMAND	sSkill;
	sSkill.ProtocolType = c2s_playerapplyaddskillpoint;
	sSkill.m_nSkillID = nSkillID;
	sSkill.m_nAddPoint = nAddPoint;
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sSkill, sizeof(PLAYER_ADD_SKILL_POINT_COMMAND));
	
	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ������������ʹ��ĳ����Ʒ������Ҽ��������Ʒ��ֻ�����ڳ�ҩ��
//-------------------------------------------------------------------------
BOOL	KPlayer::ApplyUseItem(int nItemID, ItemPos SrcPos)
{
	if (this->CheckTrading())
		return FALSE;

	int nRet = m_ItemList.UseItem(nItemID);
	if (nRet == 0)
		return FALSE;
	
	if (nRet == REQUEST_EQUIP_ITEM)
	{
	}
	else if (nRet == REQUEST_EAT_MEDICINE)
	{
		PLAYER_EAT_ITEM_COMMAND	sEat;
		sEat.ProtocolType = c2s_playereatitem;
		sEat.m_nItemID = nItemID;
		sEat.m_btPlace = SrcPos.nPlace;
		sEat.m_btX = SrcPos.nX;
		sEat.m_btY = SrcPos.nY;
		if (g_pClient)
			g_pClient->SendPackToServer(&sEat, sizeof(PLAYER_EAT_ITEM_COMMAND));
	}
	
	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ͻ��������obj����ĳ����Ʒ�������������Ϣ
//-------------------------------------------------------------------------
void	KPlayer::PickUpObj(int nObjIndex)
{
	if (this->CheckTrading())
		return;
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Item && Object[nObjIndex].m_nKind != Obj_Kind_Money)
		return;
	
	PLAYER_PICKUP_ITEM_COMMAND	sPickUp;
	if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
	{
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = 0;
		sPickUp.m_btPosX = 0;
		sPickUp.m_btPosY = 0;
	}
	else
	{
		ItemPos	sItemPos;
		if ( FALSE == m_ItemList.SearchPosition(Object[nObjIndex].m_nItemWidth, Object[nObjIndex].m_nItemHeight, &sItemPos) )
		{
			KSystemMessage	sMsg;
			
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.eType = SMT_NORMAL;
			sMsg.byParamSize = 0;
			sMsg.byPriority = 0;
			
			strcpy(sMsg.szMessage, MSG_SHOP_NO_ROOM);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			return;
		}
		sPickUp.ProtocolType = c2s_playerpickupitem;
		sPickUp.m_nObjID = Object[nObjIndex].m_nID;
		sPickUp.m_btPosType = sItemPos.nPlace;
		sPickUp.m_btPosX = sItemPos.nX;
		sPickUp.m_btPosY = sItemPos.nY;
	}
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sPickUp, sizeof(PLAYER_PICKUP_ITEM_COMMAND));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��ͻ��������obj�������������Ϣ
//-------------------------------------------------------------------------
void	KPlayer::ObjMouseClick(int nObjIndex)
{
	if (this->CheckTrading())
		return;
	if (nObjIndex <= 0)
		return;
	if (Object[nObjIndex].m_nKind != Obj_Kind_Box && Object[nObjIndex].m_nKind != Obj_Kind_Prop)
		return;
	SendObjMouseClick(Object[nObjIndex].m_nID, SubWorld[0].m_Region[Object[nObjIndex].m_nRegionIdx].m_RegionID);
}
#endif

#ifndef _SERVER
// DownPos ������ϵ���Ʒ�ĵ�ǰ���꣬UpPos ������������Ʒ�ŵ�����ϵ�����
void	KPlayer::MoveItem(ItemPos DownPos, ItemPos UpPos)
{
	g_DebugLog("[MOVE-DBG] Down.place=%d(x%d,y%d) Up.place=%d(x%d,y%d) Hand=%d",
		DownPos.nPlace, DownPos.nX, DownPos.nY, UpPos.nPlace, UpPos.nX, UpPos.nY,
		m_ItemList.Hand());
	if (this->CheckTrading() && DownPos.nPlace == pos_traderoom)
	{
		if (this->m_cTrade.m_nTradeLock)
			return;
		
		// ������Ʒ������
		int	nHandIdx = m_ItemList.Hand();
		if (nHandIdx && Item[nHandIdx].GetGenre() == item_task)
		{
			KSystemMessage	sMsg;
			sprintf(sMsg.szMessage, MSG_TRADE_TASK_ITEM);
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
			return;
		}
	}
	
	if (!CheckTrading() && DownPos.nPlace == pos_immediacy)
	{
		int nHandIdx = m_ItemList.Hand();
		if (nHandIdx)
		{
			if (m_ItemList.m_Room[room_immediacy].CheckSameDetailType(Item[nHandIdx].GetGenre(), Item[nHandIdx].GetDetailType()))
			{
				KSystemMessage	sMsg;
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				sprintf(sMsg.szMessage, MSG_ITEM_SAME_DETAIL_IN_IMMEDIATE);
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
				// Tha trung loai bi tu choi: tra mon ve tui thay vi de ket vo hinh
				// tren con tro (truoc day chi return -> click ra ngoai moi roi dat).
				BOOL bReturned = m_ItemList.ReturnHandToBag();
				g_DebugLog("[MOVE-DBG] dup immediacy -> ReturnHandToBag=%d", (int)bReturned);
				return;
			}
		}
	}

	SendClientCmdMoveItem(&DownPos, &UpPos);
}
#endif

#ifndef _SERVER
int	KPlayer::ThrowAwayItem()
{
	if (this->CheckTrading())
		return 0;
	if ( !m_ItemList.Hand() )
		return 0;
	
	PLAYER_THROW_AWAY_ITEM_COMMAND	sThrow;
	
	sThrow.ProtocolType = c2s_playerthrowawayitem;
	
	if (g_pClient)
		g_pClient->SendPackToServer(&sThrow, sizeof(PLAYER_THROW_AWAY_ITEM_COMMAND));
	
	return 1;
}
#endif

#ifndef _SERVER
void	KPlayer::ChatAddFriend(int nPlayerIdx)
{
	CHAT_ADD_FRIEND_COMMAND	sAdd;
	sAdd.ProtocolType = c2s_chataddfriend;
	sAdd.m_nTargetPlayerIdx = nPlayerIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sAdd, sizeof(CHAT_ADD_FRIEND_COMMAND));
}
#endif

#ifndef _SERVER
void	KPlayer::ChatRefuseFriend(int nPlayerIdx)
{
	CHAT_REFUSE_FRIEND_COMMAND	sRefuse;
	sRefuse.ProtocolType = c2s_chatrefusefriend;
	sRefuse.m_nTargetPlayerIdx = nPlayerIdx;
	if (g_pClient)
		g_pClient->SendPackToServer(&sRefuse, sizeof(CHAT_REFUSE_FRIEND_COMMAND));
}
#endif

#ifndef _SERVER
void	KPlayer::TradeApplyOpen(char *lpszSentence, int nLength)
{
	if (this->CheckTrading())
		return;
	if (!lpszSentence)
		nLength = 0;
	TRADE_APPLY_OPEN_COMMAND	sOpen;
	sOpen.ProtocolType = c2s_tradeapplystateopen;
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	sOpen.m_wLength = sizeof(TRADE_APPLY_OPEN_COMMAND) - 1 - sizeof(sOpen.m_szSentence) + nLength;
	memset(m_cMenuState.m_szSentence, 0, sizeof(m_cMenuState.m_szSentence));
	if (lpszSentence)
		memcpy(m_cMenuState.m_szSentence, lpszSentence, nLength);
	memset(sOpen.m_szSentence, 0, sizeof(sOpen.m_szSentence));
	memcpy(sOpen.m_szSentence, m_cMenuState.m_szSentence, nLength);
	if (g_pClient)
		g_pClient->SendPackToServer(&sOpen, sOpen.m_wLength + 1);
}
#endif

#ifndef _SERVER
void	KPlayer::TradeApplyClose()
{
	TRADE_APPLY_CLOSE_COMMAND	sApply;
	sApply.ProtocolType = c2s_tradeapplystateclose;
	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(TRADE_APPLY_CLOSE_COMMAND));
}
#endif

#ifndef _SERVER
void	KPlayer::TradeApplyStart(int nNpcIdx)
{
	if (nNpcIdx < 0 || nNpcIdx >= MAX_NPC)
		return;
	TRADE_APPLY_START_COMMAND	sStart;
	sStart.ProtocolType = c2s_tradeapplystart;
	sStart.m_dwID = Npc[nNpcIdx].m_dwID;
	if (g_pClient)
		g_pClient->SendPackToServer(&sStart, sizeof(TRADE_APPLY_START_COMMAND));
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TRADE_SEND_APPLY, Npc[nNpcIdx].Name);
	sMsg.eType = SMT_NORMAL;
	sMsg.byConfirmType = SMCT_NONE;
	sMsg.byPriority = 0;
	sMsg.byParamSize = 0;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
}
#endif

#ifndef _SERVER
BOOL	KPlayer::TradeMoveMoney(int nMoney)
{
	if (!CheckTrading())
		return FALSE;
	if (this->m_cTrade.m_nTradeLock)
		return FALSE;
	// Ǯ��������
	if (nMoney < 0 || nMoney > m_ItemList.GetEquipmentMoney())
		return FALSE;
	
	TRADE_MOVE_MONEY_COMMAND	sMoney;
	
	sMoney.ProtocolType = c2s_trademovemoney;
	sMoney.m_nMoney = nMoney;
	if (g_pClient)
		g_pClient->SendPackToServer(&sMoney, sizeof(TRADE_MOVE_MONEY_COMMAND));
	
	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�����ȷ����ȡ��
//	if nDecision == 0 �Ƴ�����  if nDecision == 1 ȷ������  if nDecision == 2 ȡ������ȷ��
//-------------------------------------------------------------------------
void	KPlayer::TradeDecision(int nDecision)
{
	if ( !CheckTrading() )
		return;
	TRADE_DECISION_COMMAND	sDecision;
	sDecision.ProtocolType = c2s_tradedecision;
	if (nDecision == 1)
	{
		sDecision.m_btDecision = 1;
	}
	else if (nDecision == 0)
	{
		sDecision.m_btDecision = 0;
	}
	else if (nDecision == 2)
	{
		sDecision.m_btDecision = 4;
	}
	if (g_pClient)
		g_pClient->SendPackToServer(&sDecision, sizeof(TRADE_DECISION_COMMAND));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�����������ȡ������
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyLock(int nLockOrNot)
{
	if ( !CheckTrading() )
		return;
	TRADE_DECISION_COMMAND	sDecision;
	sDecision.ProtocolType = c2s_tradedecision;
	if (nLockOrNot)
	{
		sDecision.m_btDecision = 2;
	}
	else
	{
		sDecision.m_btDecision = 3;
	}
	if (g_pClient)
		g_pClient->SendPackToServer(&sDecision, sizeof(TRADE_DECISION_COMMAND));
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ��������������
//-------------------------------------------------------------------------
void	KPlayer::s2cApplyAddTeam(BYTE* pProtocol)
{
	// ״̬��� (����Ҫ��� npc �Ķ��鿪��״̬ not end)
	if ( !m_cTeam.m_nFlag || m_cTeam.m_nFigure != TEAM_CAPTAIN)
	{
		ApplySelfTeamInfo();
		return;
	}
	
	// Ѱ�� npc
	PLAYER_APPLY_ADD_TEAM	*pAddTeam = (PLAYER_APPLY_ADD_TEAM*)pProtocol;
	int nNpcNo = NpcSet.SearchID(pAddTeam->m_dwTarNpcID);
	if (nNpcNo == 0)
		return;
	
	int i, nFreeListNo;
	// ����Ѿ����ڣ�����
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == pAddTeam->m_dwTarNpcID)
		{
			m_cTeam.m_sApplyList[i].m_nLevel = Npc[nNpcNo].m_Level;
			m_cTeam.m_sApplyList[i].m_dwTimer = MAX_APPLY_TEAM_TIME;
			strcpy(m_cTeam.m_sApplyList[i].m_szName, Npc[nNpcNo].Name);
			m_cTeam.UpdateInterface();
			return;
		}
	}
	// Ѱ��������list��λ
	for (i = 0; i < MAX_TEAM_APPLY_LIST; i++)
	{
		if (m_cTeam.m_sApplyList[i].m_dwNpcID == 0)
		{
			nFreeListNo = i;
			break;
		}
	}
	if (i >= MAX_TEAM_APPLY_LIST)	// û�ҵ������˿�λ
		return;
	
	m_cTeam.m_sApplyList[nFreeListNo].m_dwNpcID = pAddTeam->m_dwTarNpcID;
	m_cTeam.m_sApplyList[nFreeListNo].m_nLevel = Npc[nNpcNo].m_Level;
	m_cTeam.m_sApplyList[nFreeListNo].m_dwTimer = MAX_APPLY_TEAM_TIME;
	strcpy(m_cTeam.m_sApplyList[nFreeListNo].m_szName, Npc[nNpcNo].Name);
	
	m_cTeam.UpdateInterface();
	
	KSystemMessage	sMsg;
	sprintf(sMsg.szMessage, MSG_TEAM_APPLY_ADD, Npc[nNpcNo].Name);
	sMsg.eType = SMT_TEAM;
	sMsg.byConfirmType = SMCT_UI_TEAM_APPLY;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);

	KUiPlayerItem	player;
	strcpy(player.Name, Npc[nNpcNo].Name);
	player.nIndex = 0;
	player.uId = pAddTeam->m_dwTarNpcID;
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&player);
	
	return;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ���������
//-------------------------------------------------------------------------
BOOL	KPlayer::AddFaction(char *lpszFactionName)
{
	if ( !m_cFaction.AddFaction(Npc[m_nIndex].m_Series, lpszFactionName) )
		return FALSE;
	
	// �ı���Ӫ
	Npc[m_nIndex].SetCamp(m_cFaction.GetGurFactionCamp());
	
	// ���ͻ��˷���Ϣ
	SendFactionData();
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ���������
//-------------------------------------------------------------------------
BOOL	KPlayer::AddFaction(int nFactionID)
{
	if ( !m_cFaction.AddFaction(Npc[m_nIndex].m_Series, nFactionID) )
		return FALSE;
	
	// �ı���Ӫ
	Npc[m_nIndex].SetCamp(m_cFaction.GetGurFactionCamp());
	
	// ���ͻ��˷���Ϣ
	SendFactionData();
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��뿪����
//-------------------------------------------------------------------------
BOOL	KPlayer::LeaveCurFaction()
{
	m_cFaction.LeaveFaction();
	
	// �ı���Ӫ
	Npc[m_nIndex].SetCamp(camp_free);
	
	// ���ͻ��˷���Ϣ
	PLAYER_LEAVE_FACTION	sLeave;
	sLeave.ProtocolType = s2c_playerleavefaction;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLeave, sizeof(PLAYER_LEAVE_FACTION));
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ����ŵ�ǰ����ĳ���ȼ��ļ���
//-------------------------------------------------------------------------
BOOL	KPlayer::CurFactionOpenSkill(int nLevel)
{
	if ( !m_cFaction.OpenCurSkillLevel(nLevel, &Npc[m_nIndex].m_SkillList) )
		return FALSE;
	
	PLAYER_FACTION_SKILL_LEVEL	sLevel;
	sLevel.ProtocolType = s2c_playerfactionskilllevel;
	sLevel.m_btCurFactionID = m_cFaction.m_nCurFaction;
	sLevel.m_btLevel = nLevel;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sLevel, sizeof(PLAYER_FACTION_SKILL_LEVEL));
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ����������ʱ����
//-------------------------------------------------------------------------
void	KPlayer::TaskClearTempVal()
{
	this->m_cTask.ClearTempVar();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��õ�����������
//-------------------------------------------------------------------------
int		KPlayer::TaskGetSaveVal(int nNo)
{
	return this->m_cTask.GetSaveVal(nNo);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��趨����������
//-------------------------------------------------------------------------
void	KPlayer::TaskSetSaveVal(int nNo, BOOL bFlag)
{
	this->m_cTask.SetSaveVal(nNo, bFlag);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��õ�������ʱ���̿��Ʊ���ֵ
//-------------------------------------------------------------------------
int		KPlayer::TaskGetClearVal(int nNo)
{
	return this->m_cTask.GetClearVal(nNo);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��趨������ʱ���̿��Ʊ���ֵ
//-------------------------------------------------------------------------
void	KPlayer::TaskSetClearVal(int nNo, int nVal)
{
	this->m_cTask.SetClearVal(nNo, nVal);
}
#endif


#ifndef _SERVER
void	KPlayer::SetDefaultImmedSkill()
{
	//��ȡ������Ҽ�����ΪĬ����������
	int nDetailType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponType();
	int nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();
		
	//��������
	if (nDetailType == 0)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nMeleeWeaponSkill[nParticularType]);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nMeleeWeaponSkill[nParticularType]);
	}//Զ������
	else if (nDetailType == 1)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nRangeWeaponSkill[nParticularType]);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nRangeWeaponSkill[nParticularType]);
	}//����
	else if (nDetailType == -1)
	{
		Player[CLIENT_PLAYER_INDEX].SetLeftSkill(g_nHandSkill);
		Player[CLIENT_PLAYER_INDEX].SetRightSkill(g_nHandSkill);
	}
}

void	KPlayer::SetLeftSkill(int nSkillID)
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) <= 0) return;
	m_nLeftSkillID = nSkillID;
	
	KUiGameObject Info;
	Info.uGenre = CGOG_SKILL_SHORTCUT;
	Info.uId = m_nLeftSkillID;
	CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (unsigned int)&Info, -1);
}

void	KPlayer::SetRightSkill(int nSkillID)
{
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(nSkillID) <= 0) return;
	m_nRightSkillID = nSkillID;
	// ����Ƿ��ǹ⻷����
	ISkill * pOrdinSkill = g_SkillManager.GetSkill(nSkillID, 1);
	if (!pOrdinSkill) 
        return;
	
	if (pOrdinSkill->IsAura())
	{
		Npc[m_nIndex].SetAuraSkill(nSkillID);
	}
	else
	{
		Npc[m_nIndex].SetAuraSkill(0);
	}

	KUiGameObject Info;
	Info.uGenre = CGOG_SKILL_SHORTCUT;
	Info.uId = m_nRightSkillID;
	CoreDataChanged(GDCNI_PLAYER_IMMED_ITEMSKILL, (unsigned int)&Info, -2);
}

void KPlayer::UpdateWeaponSkill()
{
	if (m_nLeftSkillID > 0)
	{
		ISkill * pISkill = g_SkillManager.GetSkill(m_nLeftSkillID, 1);
		if (!pISkill) 
            return;
		
		if (pISkill->IsPhysical())
		{
			SetLeftSkill(Npc[m_nIndex].GetCurActiveWeaponSkill());
		}
		
	}
	if (m_nRightSkillID > 0)
	{
		ISkill * pISkill = (KSkill *) g_SkillManager.GetSkill(m_nRightSkillID, 1);
		if (!pISkill) 
            return;
		
		if (pISkill->IsPhysical())
		{
			SetRightSkill(Npc[m_nIndex].GetCurActiveWeaponSkill());
		}
	}
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��趨��ǰ����Ƶ��
//-------------------------------------------------------------------------
void	KPlayer::SetChatCurChannel(int nChannelNo)
{
	m_cChat.SetCurChannel(nChannelNo);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�����������
//-------------------------------------------------------------------------
void	KPlayer::TeamInviteAdd(DWORD dwNpcID)
{
	m_cTeam.InviteAdd(dwNpcID);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ��˷������������
//-------------------------------------------------------------------------
void	KPlayer::ServerSendChat(BYTE* pProtocol)
{
	m_cChat.ServerSendChat(m_nPlayerIndex, pProtocol);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ���Ҫ�����ӻ������Ե�(0=Strength 1=Dexterity 2=Vitality 3=Engergy)
//-------------------------------------------------------------------------
void	KPlayer::AddBaseAttribute(BYTE* pProtocol)
{
	PLAYER_ADD_BASE_ATTRIBUTE_COMMAND	*pAdd = (PLAYER_ADD_BASE_ATTRIBUTE_COMMAND*)pProtocol;
	switch (pAdd->m_btAttribute)
	{
	case ATTRIBUTE_STRENGTH:
		AddBaseStrength(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_DEXTERITY:
		AddBaseDexterity(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_VITALITY:
		AddBaseVitality(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_ENGERGY:
		AddBaseEngergy(pAdd->m_nAddNo);
		break;
	}
}
void	KPlayer::ResetBaseAttribute(BYTE* pProtocol)
{
	PLAYER_ADD_BASE_ATTRIBUTE_COMMAND	*pAdd = (PLAYER_ADD_BASE_ATTRIBUTE_COMMAND*)pProtocol;
	switch (pAdd->m_btAttribute)
	{
	case ATTRIBUTE_STRENGTH:
		ResetBaseStrength(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_DEXTERITY:
		ResetBaseDexterity(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_VITALITY:
		ResetBaseVitality(pAdd->m_nAddNo);
		break;
	case ATTRIBUTE_ENGERGY:
		ResetBaseEngergy(pAdd->m_nAddNo);
		break;
	}
}
#endif

#define MAX_ORDINSKILL_LEVEL_ALWAYS  20//��������������Լ��ܵȼ��ı䶯֮��ģ�һ������ܵȼ�
#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ���Ҫ������ĳ�����ܵĵ���
//-------------------------------------------------------------------------
void	KPlayer::AddSkillPoint(BYTE* pProtocol)
{
	PLAYER_ADD_SKILL_POINT_COMMAND	*pAdd = (PLAYER_ADD_SKILL_POINT_COMMAND*)pProtocol;
	
	int		nSkillIndex, nSkillLevel;
	
	nSkillIndex = Npc[m_nIndex].m_SkillList.FindSame(pAdd->m_nSkillID);
	if (nSkillIndex == 0)
		return;
	nSkillLevel = Npc[m_nIndex].m_SkillList.GetSkillIdxLevel(nSkillIndex);
	
	
	if (m_nSkillPoint >= pAdd->m_nAddPoint && nSkillIndex )//&& nSkillLevel + pAdd->m_nAddPoint <= MAX_SKILLLEVEL
	{
		ISkill * pSkill = NULL;
		if (nSkillLevel == 0)
		{
			pSkill =  g_SkillManager.GetSkill(pAdd->m_nSkillID, 1);
		}
		else
		{
			pSkill =  g_SkillManager.GetSkill(pAdd->m_nSkillID, nSkillLevel);
		}
		
		if (pSkill)
		{
			//�ж�Ӧ�ļ��������ű�,����ִ�нű�
			if (pSkill->GetSkillLevelUpScriptId())
			{
				ExecuteScript(pSkill->GetSkillLevelUpScriptId(), "main", pAdd->m_nAddPoint);
				nSkillLevel = Npc[m_nIndex].m_SkillList.GetSkillIdxLevel(nSkillIndex);
			}
			else
			{
			/*
			���������ĳ���
			1�������ܹ��ӵ�ļ��ܣ����ֻ�ܼӵ�20����
			2������һ����ҵĵȼ���x,һ�����ܵĻ���ѧϰ�ȼ���y����ô���ܹ�ѧϰ������ܵ���ߵȼ���x-y��
			3��ʹ�ü�����Ҫ����һЩ������жϣ���һ���Ƿ�������״̬���ڶ�����װ������������ϸ����Ƿ�Ϊָ����ֵ��-1��ʾû������
				*/
				switch(pSkill->GetSkillStyle())
				{
				case SKILL_SS_Missles:			//	�ӵ���		���������ڷ����ӵ���
				case SKILL_SS_Melee:
				case SKILL_SS_InitiativeNpcState:	//	������		���������ڸı䵱ǰNpc������״̬
				case SKILL_SS_PassivityNpcState:		//	������		���������ڸı�Npc�ı���״̬
					{
						int nWantToBeLevel = nSkillLevel + pAdd->m_nAddPoint;
						
						//Limit2
						if ((nWantToBeLevel -Npc[m_nIndex].m_SkillList.GetAddPoint(nSkillIndex)) <= g_SkillManager.GetSkillMaxLevel(pAdd->m_nSkillID) && nWantToBeLevel <= Npc[m_nIndex].m_Level - ((KSkill*)pSkill)->GetSkillReqLevel() + 1)
						{
							if ( !Npc[m_nIndex].m_SkillList.IncreaseLevel(nSkillIndex, pAdd->m_nAddPoint) )
								return;
							m_nSkillPoint -= pAdd->m_nAddPoint;
							nSkillLevel += pAdd->m_nAddPoint;
						}
						else
						{
							if (nWantToBeLevel >=  g_SkillManager.GetSkillMaxLevel(pAdd->m_nSkillID))
							{
								char szMsg[100];
								sprintf(szMsg, "K� n�ng <color=green>%s <color>�� ��t c�p t�i �a, kh�ng th� c�ng th�m!", pSkill->GetSkillName());
								KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "H� th�ng", (char *) szMsg, strlen(szMsg) );
							}
							else 
							{
								char szMsg[100];
								sprintf(szMsg, "B�n c�n ��t c�p <color=green>%d<color> m�i c� th� n�ng k� n�ng <color=green>%s <color>.", ((KSkill *)pSkill)->GetSkillReqLevel() -1 +  pAdd->m_nAddPoint + nSkillLevel,  ((KSkill*)pSkill)->GetSkillName());
								KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "H� th�ng", (char *) szMsg, strlen(szMsg) );
							}
							
						}
					}
					break;
					
				case SKILL_SS_Thief:
					{
						return;
					}break;
					
				}
				
			}
			
			UpdataCurData();
			// �ѵ�ǰ�˼��ܵĵ�����ʣ�༼�ܵ㷢���ͻ���
			PLAYER_SKILL_LEVEL_SYNC	sSkill;
			sSkill.ProtocolType = s2c_playerskilllevel;
			sSkill.m_nSkillID = pAdd->m_nSkillID;
			sSkill.m_nSkillLevel = Npc[m_nIndex].m_SkillList.GetCurrentLevel(pAdd->m_nSkillID);
			sSkill.m_nLeavePoint = m_nSkillPoint;
			sSkill.m_nAddPoint = Npc[m_nIndex].m_SkillList.GetAddPoint(nSkillIndex);
			g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSkill, sizeof(PLAYER_SKILL_LEVEL_SYNC));
		}
	}
	
	
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��������˼���Ʒ
//-------------------------------------------------------------------------
BOOL	KPlayer::ServerPickUpItem(BYTE* pProtocol)
{
	PLAYER_PICKUP_ITEM_COMMAND	*pPickUp = (PLAYER_PICKUP_ITEM_COMMAND*)pProtocol;
	
	int		nObjIndex, nNpcX, nNpcY, nObjX, nObjY;
	nObjIndex = ObjSet.FindID(pPickUp->m_nObjID);
	if (nObjIndex == 0)
		return FALSE;
	
	if (Object[nObjIndex].m_nBelong != -1)
	{
		if (!m_cTeam.m_nFlag)
		{
			if (Object[nObjIndex].m_nBelong != m_nPlayerIndex)
			{
				SHOW_MSG_SYNC	sMsg;
				sMsg.ProtocolType = s2c_msgshow;
				if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
					sMsg.m_wMsgID = enumMSG_ID_MONEY_CANNOT_PICKUP;
				else
					sMsg.m_wMsgID = enumMSG_ID_OBJ_CANNOT_PICKUP;
				sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
				g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				return FALSE;
			}
		}
		else
		{
			if (Object[nObjIndex].m_nKind == Obj_Kind_Money)
			{
				if (Object[nObjIndex].m_nBelong != m_nPlayerIndex &&
					!g_Team[m_cTeam.m_nID].CheckIn(Object[nObjIndex].m_nBelong))
				{
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wMsgID = enumMSG_ID_MONEY_CANNOT_PICKUP;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					return FALSE;
				}
			}
			else if (Object[nObjIndex].m_nKind == Obj_Kind_Item)
			{
				if (Object[nObjIndex].m_nItemDataID <= 0 || Object[nObjIndex].m_nItemDataID >= MAX_ITEM)
				{
					_ASSERT(0);
					return FALSE;
				}
				if ((Item[Object[nObjIndex].m_nItemDataID].GetGenre() == item_task && Object[nObjIndex].m_nBelong != m_nPlayerIndex) ||
					!g_Team[m_cTeam.m_nID].CheckIn(Object[nObjIndex].m_nBelong))
				{
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wMsgID = enumMSG_ID_OBJ_CANNOT_PICKUP;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					return FALSE;
				}
			}
			else
			{
				return FALSE;
			}
		}
	}
	// �жϾ���
	if (Object[nObjIndex].m_nSubWorldID != Npc[m_nIndex].m_SubWorldIndex)
		return FALSE;
	SubWorld[Object[nObjIndex].m_nSubWorldID].Map2Mps(
		Object[nObjIndex].m_nRegionIdx,
		Object[nObjIndex].m_nMapX,
		Object[nObjIndex].m_nMapY,
		Object[nObjIndex].m_nOffX,
		Object[nObjIndex].m_nOffY,
		&nObjX,
		&nObjY);
	SubWorld[Npc[m_nIndex].m_SubWorldIndex].Map2Mps(
		Npc[m_nIndex].m_RegionIndex,
		Npc[m_nIndex].m_MapX,
		Npc[m_nIndex].m_MapY,
		Npc[m_nIndex].m_OffX,
		Npc[m_nIndex].m_OffY,
		&nNpcX,
		&nNpcY);
	if ( PLAYER_PICKUP_SERVER_DISTANCE < (nNpcX - nObjX) * (nNpcX - nObjX) + (nNpcY - nObjY) * (nNpcY - nObjY))
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_OBJ_TOO_FAR;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		return FALSE;
	}
	
	switch (Object[nObjIndex].m_nKind)
	{
	case Obj_Kind_Item:				// ���ڵ��ϵ�װ��
		{
			int nItemIdx = m_ItemList.Add(Object[nObjIndex].m_nItemDataID, pPickUp->m_btPosType, pPickUp->m_btPosX, pPickUp->m_btPosY);
			if (nItemIdx <= 0 || nItemIdx >= MAX_PLAYER_ITEM)
				return FALSE;
			if (Object[nObjIndex].m_nItemDataID <= 0 || Object[nObjIndex].m_nItemDataID >= MAX_ITEM)
				return FALSE;
			// ���ͻ��˷��ͻ��װ������Ϣ
			SHOW_MSG_SYNC	sMsg;
			sMsg.ProtocolType = s2c_msgshow;
			sMsg.m_wMsgID = enumMSG_ID_GET_ITEM;
			sMsg.m_lpBuf = (LPVOID)Item[Object[nObjIndex].m_nItemDataID].GetID();
			sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
			g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
			sMsg.m_lpBuf = 0;
			
			// ȥ��Object[nObjIndex]����ߵĹ���������ItemSet��Remove��Object��Remove����
			Object[nObjIndex].m_nItemDataID = 0;
			Object[nObjIndex].m_nItemWidth = 0;
			Object[nObjIndex].m_nItemHeight = 0;
			Object[nObjIndex].Remove(FALSE);
			
		}
		break;
	case Obj_Kind_Money:			// ���ڵ��ϵ�Ǯ
		if ( !Earn(Object[nObjIndex].m_nMoneyNum) )
			return FALSE;
		Object[nObjIndex].SyncRemove(TRUE);
		if (Object[nObjIndex].m_nRegionIdx >= 0)
			SubWorld[Object[nObjIndex].m_nSubWorldID].m_Region[Object[nObjIndex].m_nRegionIdx].RemoveObj(nObjIndex);
		ObjSet.Remove(nObjIndex);
		break;
	}
	
	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ���Ҫ��ʹ��ĳ����Ʒ(����Ҽ����)
//-------------------------------------------------------------------------
void	KPlayer::EatItem(BYTE* pProtocol)
{
	PLAYER_EAT_ITEM_COMMAND	*pEat = (PLAYER_EAT_ITEM_COMMAND*)pProtocol;
	
	m_ItemList.EatMecidine(pEat->m_btPlace, pEat->m_btX, pEat->m_btY);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ���Ҫ��ʹ��ĳ����Ʒ(����Ҽ����)
//-------------------------------------------------------------------------
//void	KPlayer::UseItem(BYTE* pProtocol)
//{
//	PLAYER_USE_ITEM_COMMAND	*pItem = (PLAYER_USE_ITEM_COMMAND*)pProtocol;
//}
#endif

#ifdef _SERVER
void	KPlayer::ServerMoveItem(BYTE* pProtocol)
{
	PLAYER_MOVE_ITEM_COMMAND	*pMove = (PLAYER_MOVE_ITEM_COMMAND*)pProtocol;
	ItemPos		DownPos, UpPos;
	if ((pMove->m_btDownPos == pos_repositoryroom || pMove->m_btUpPos == pos_repositoryroom) && !m_CUnlocked)
	{
		// Client dang co lay (hoac cho them do) vao ruong khi ruong con dang bi khoa ~> thoat ra ngay
		return;
	}
	
//	if ((pMove->m_btDownPos == pos_expandbox || pMove->m_btUpPos == pos_expandbox) && !m_CUnlocked)
//	{
		// Client dang co lay (hoac cho them do) vao ruong mo rong khi ruong con dang bi khoa ~> thoat ra ngay
//		return;
//	}
	
	if ((pMove->m_btDownPos == pos_equip || pMove->m_btUpPos == pos_equip) && !m_CUnlocked)
	{
		// Client dang co lay (hoac cho them do) vao trang bi khi ruong con dang bi khoa ~> thoat ra ngay
		return;
	}	
	DownPos.nPlace = pMove->m_btDownPos;
	DownPos.nX = pMove->m_btDownX;
	DownPos.nY = pMove->m_btDownY;
	UpPos.nPlace = pMove->m_btUpPos;
	UpPos.nX = pMove->m_btUpX;
	UpPos.nY = pMove->m_btUpY;
	
	//EXCHANGE_ITEM_RESPONE	eir;
	//eir.ProtocolType = s2c_exchangerespone;
	
	if (this->CheckTrading() && DownPos.nPlace == pos_traderoom)
	{
		if (this->m_cTrade.m_nTradeLock)
			return;
	}
	
	m_ItemList.ExchangeItem(&DownPos, &UpPos);
	
	BYTE	byFinished = s2c_itemexchangefinish;
	if (g_pServer)
		g_pServer->PackDataToClient(m_nNetConnectIdx, &byFinished, sizeof(BYTE));
}
#endif

#ifdef _SERVER
void	KPlayer::ServerThrowAwayItem(BYTE* pProtocol)
{
	PLAYER_THROW_AWAY_ITEM_COMMAND	*pThrow = (PLAYER_THROW_AWAY_ITEM_COMMAND*)pProtocol;
	KMapPos		sMapPos;
	
	if ( !this->m_ItemList.Hand() )
		return;
	
	int nItemIdx = m_ItemList.m_Hand;
	GetAboutPos(&sMapPos);
	if (m_ItemList.Remove(nItemIdx))
	{
		int		nObj;
		KObjItemInfo	sInfo;
		sInfo.m_nItemID = nItemIdx;
		sInfo.m_nItemWidth = Item[nItemIdx].GetWidth();
		sInfo.m_nItemHeight = Item[nItemIdx].GetHeight();
		sInfo.m_nMoneyNum = 0;
		strcpy(sInfo.m_szName, Item[nItemIdx].m_CommonAttrib.szItemName);
		sInfo.m_nColorID = 0;
		sInfo.m_nMovieFlag = 1;
		sInfo.m_nSoundFlag = 1;
		
		nObj = ObjSet.Add(Item[nItemIdx].GetObjIdx(), sMapPos, sInfo);
		if (nObj >= 0)
		{
			if (Item[nItemIdx].GetGenre() == item_task)
			{
				Object[nObj].SetEntireBelong(this->m_nPlayerIndex);
			}
			else
			{
				Object[nObj].SetItemBelong(-1);
			}
		}
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ�����Ϣ�趨���충��Ƶ��
//-------------------------------------------------------------------------
void	KPlayer::ChatSetTakeChannel(BYTE* pProtocol)
{
	CHAT_SET_CHANNEL_COMMAND	*pChannel = (CHAT_SET_CHANNEL_COMMAND*)pProtocol;
	this->m_cChat.SetTakeChannel(pChannel->m_dwTakeChannel);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ�������ת���������Ӻ�����Ϣ
//-------------------------------------------------------------------------
void	KPlayer::ChatTransmitApplyAddFriend(BYTE* pProtocol)
{
	CHAT_APPLY_ADD_FRIEND_COMMAND	*pAdd = (CHAT_APPLY_ADD_FRIEND_COMMAND*)pProtocol;
	int	nPlayerIdx = FindAroundPlayer(pAdd->m_dwTargetNpcID);
	if (nPlayerIdx < 0)
		return;
	
	m_cChat.m_nAddFriendList[m_cChat.m_nListPos++] = nPlayerIdx;
	m_cChat.m_nListPos %= CHAT_APPLY_ADD_FRIEND_LIST;
	
	CHAT_APPLY_ADD_FRIEND_SYNC	sAdd;
	sAdd.ProtocolType = s2c_chatapplyaddfriend;
	sAdd.m_nSrcPlayerIdx = m_nPlayerIndex;
	strcpy(sAdd.m_szSourceName, Npc[m_nIndex].Name);
	memset(sAdd.m_szInfo, 0, sizeof(sAdd.m_szInfo));
	memcpy(sAdd.m_szInfo, pAdd->m_szInfo, pAdd->m_wLength + 1 + sizeof(pAdd->m_szInfo) - sizeof(CHAT_APPLY_ADD_FRIEND_COMMAND));
	g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, (BYTE*)&sAdd, sizeof(CHAT_APPLY_ADD_FRIEND_SYNC));
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ�����Ϣ�����������
//-------------------------------------------------------------------------
BOOL	KPlayer::ChatAddFriend(BYTE* pProtocol)
{
	CHAT_ADD_FRIEND_COMMAND	*pAdd = (CHAT_ADD_FRIEND_COMMAND*)pProtocol;
	
	if (pAdd->m_nTargetPlayerIdx < 0 || pAdd->m_nTargetPlayerIdx >= MAX_PLAYER)
		goto AddFalse;
	if ( !Player[pAdd->m_nTargetPlayerIdx].m_nIndex )
		goto AddFalse;
	int		i;
	for (i = 0; i < CHAT_APPLY_ADD_FRIEND_LIST; i++)
	{
		if (m_nPlayerIndex == Player[pAdd->m_nTargetPlayerIdx].m_cChat.m_nAddFriendList[i])
			break;
	}
	if (i >= CHAT_APPLY_ADD_FRIEND_LIST)
		goto AddFalse;
	
	m_cChat.AddFriendData(m_nPlayerIndex, pAdd->m_nTargetPlayerIdx);
	Player[pAdd->m_nTargetPlayerIdx].m_cChat.AddFriendData(pAdd->m_nTargetPlayerIdx, m_nPlayerIndex);
	
	return TRUE;
	
AddFalse:
	CHAT_ADD_FRIEND_FAIL_SYNC	sFail;
	sFail.ProtocolType = s2c_chataddfriendfail;
	sFail.m_nTargetPlayerIdx = pAdd->m_nTargetPlayerIdx;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sFail, sizeof(CHAT_ADD_FRIEND_FAIL_SYNC));
	
	return FALSE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ�����Ϣ�ܾ������������
//-------------------------------------------------------------------------
void	KPlayer::ChatRefuseFriend(BYTE* pProtocol)
{
	CHAT_REFUSE_FRIEND_COMMAND	*pRefuse = (CHAT_REFUSE_FRIEND_COMMAND*)pProtocol;
	
	if (pRefuse->m_nTargetPlayerIdx < 0 || pRefuse->m_nTargetPlayerIdx >= MAX_PLAYER)
		return;
	if (Player[pRefuse->m_nTargetPlayerIdx].m_nIndex == 0)
		return;
	for (int i = 0; i < CHAT_APPLY_ADD_FRIEND_LIST; i++)
	{
		if (m_nPlayerIndex == Player[pRefuse->m_nTargetPlayerIdx].m_cChat.m_nAddFriendList[i])
		{
			CHAT_REFUSE_FRIEND_SYNC	sRefuse;
			sRefuse.ProtocolType = s2c_chatrefusefriend;
			g_StrCpy(sRefuse.m_szName, Npc[m_nIndex].Name);
			sRefuse.m_wLength = sizeof(CHAT_REFUSE_FRIEND_SYNC) - 1 - sizeof(sRefuse.m_szName) + strlen(sRefuse.m_szName);
			
			g_pServer->PackDataToClient(Player[pRefuse->m_nTargetPlayerIdx].m_nNetConnectIdx, (BYTE*)&sRefuse, sRefuse.m_wLength + 1);
			return;
		}
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ���ҵ�¼ʱ�յ��ͻ����������·�����������������ݣ������֣�
//-------------------------------------------------------------------------
void	KPlayer::ChatResendAllFriend(BYTE* pProtocol)
{
	if ( m_cChat.m_nSyncState != 1)
		return;
	m_cChat.m_nSyncState = 2;
	m_cChat.m_pStateSendPos = (CChatFriend*)m_cChat.m_cFriendTeam[0].m_cEveryOne.GetHead();
	if ( !m_cChat.m_pStateSendPos )
		m_cChat.m_nSyncState = 0;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ���ҵ�¼ʱ�յ��ͻ���������ĳ������������ݣ������֣�
//-------------------------------------------------------------------------
void	KPlayer::ChatSendOneFriendData(BYTE* pProtocol)
{
	CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND	*pApply = (CHAT_APPLY_SEND_ONE_FRIEND_NAME_COMMAND*)pProtocol;
	
	m_cChat.ResendOneFriendData(pApply->m_dwID, m_nPlayerIndex);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ�������ɾ��ĳ���������
//-------------------------------------------------------------------------
void	KPlayer::ChatDeleteFriend(BYTE* pProtocol)
{
	CHAT_DELETE_FRIEND_COMMAND	*pDelete = (CHAT_DELETE_FRIEND_COMMAND*)pProtocol;
	
	m_cChat.DeleteFriend(pDelete->m_dwID, m_dwID);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ��������ٴ�ɾ��ĳ���������
//-------------------------------------------------------------------------
void	KPlayer::ChatRedeleteFriend(BYTE* pProtocol)
{
	CHAT_REDELETE_FRIEND_COMMAND	*pDelete = (CHAT_REDELETE_FRIEND_COMMAND*)pProtocol;
	
	this->m_cChat.RedeleteFriend(pDelete->m_dwID, m_nPlayerIndex);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ���������������״̬
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyOpen(BYTE* pProtocol)
{
	if ( CheckTrading() )
		return;
	TRADE_APPLY_OPEN_COMMAND	*pApply = (TRADE_APPLY_OPEN_COMMAND*)pProtocol;
	char	szSentence[MAX_SENTENCE_LENGTH];
	int		nLength = pApply->m_wLength - (sizeof(TRADE_APPLY_OPEN_COMMAND) - 1 - sizeof(pApply->m_szSentence));
	if (nLength >= MAX_SENTENCE_LENGTH)
		nLength = MAX_SENTENCE_LENGTH - 1;
	memset(szSentence, 0, sizeof(szSentence));
	memcpy(szSentence, pApply->m_szSentence, nLength);
	
	switch (m_cMenuState.m_nState)
	{
	case PLAYER_MENU_STATE_NORMAL:
	case PLAYER_MENU_STATE_TRADEOPEN:
		m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADEOPEN, szSentence, nLength);
		break;
	case PLAYER_MENU_STATE_TEAMOPEN:
		if (m_cTeam.m_nFlag && m_cTeam.m_nID >= 0)
			g_Team[m_cTeam.m_nID].SetTeamClose();
		m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADEOPEN, szSentence, nLength);
		break;
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ�������ȡ��������״̬
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyClose(BYTE* pProtocol)
{
	m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ��յ��ͻ������뿪ʼ����
//-------------------------------------------------------------------------
void	KPlayer::TradeApplyStart(BYTE* pProtocol)
{
	if (this->CheckTrading())
		return;
	
	TRADE_APPLY_START_COMMAND	*pStart = (TRADE_APPLY_START_COMMAND*)pProtocol;
	
	int		nPlayer;
	nPlayer = FindAroundPlayer(pStart->m_dwID);
	if (nPlayer < 0)
		return;
	if (Player[nPlayer].m_cMenuState.m_nState != PLAYER_MENU_STATE_TRADEOPEN)
		return;
	this->m_cTrade.m_nApplyIdx = nPlayer;
	
	TRADE_APPLY_START_SYNC	sApply;
	sApply.ProtocolType = s2c_tradeapplystart;
	sApply.m_nDestIdx = this->m_nPlayerIndex;
	sApply.m_dwNpcId = Npc[m_nIndex].m_dwID;
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayer].m_nNetConnectIdx, (BYTE*)&sApply, sizeof(TRADE_APPLY_START_SYNC));
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�
//-------------------------------------------------------------------------
void	KPlayer::TradeMoveMoney(BYTE* pProtocol)
{
	if ( !CheckTrading() )
		return;
	if (this->m_cTrade.m_nTradeLock)
		return;
	
	int		nMoney;
	TRADE_MOVE_MONEY_COMMAND	*pMoney = (TRADE_MOVE_MONEY_COMMAND*)pProtocol;
	if (pMoney->m_nMoney < 0 || pMoney->m_nMoney > m_ItemList.GetEquipmentMoney())
		nMoney = 0;
	else
		nMoney = pMoney->m_nMoney;
	m_ItemList.TradeMoveMoney(nMoney);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�ִ�л�ȡ������
//-------------------------------------------------------------------------
void	KPlayer::TradeDecision(BYTE* pProtocol)
{
	if ( !CheckTrading() )
		return;
	
	TRADE_DECISION_COMMAND	*pDecision = (TRADE_DECISION_COMMAND*)pProtocol;
	
	if (pDecision->m_btDecision == 2)	// ��������
	{
		if (this->m_cTrade.m_nTradeLock == 0)
		{
			this->m_cTrade.m_nTradeLock = 1;
			this->m_cTrade.m_nTradeState = 0;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState = 0;
		}
		
		SyncTradeState();
		return;
	}
	else if (pDecision->m_btDecision == 3)	// ����ȡ��
	{
		if (this->m_cTrade.m_nTradeLock == 1)
		{
			// ȡ������������ͬʱҲȡ���˶Է�������
			this->m_cTrade.m_nTradeLock = 0;
			this->m_cTrade.m_nTradeState = 0;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock = 0;
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState = 0;
		}
		SyncTradeState();
		return;
	}
	else if (pDecision->m_btDecision == 4)	// ȡ��ȷ��
	{
		if (m_cTrade.m_nTradeLock == 1 && Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock == 1)
		{
			m_cTrade.m_nTradeState = 0;
			SyncTradeState();
		}
		return;
	}
	else if (pDecision->m_btDecision == 0)	// ȡ������
	{
		int	nDestIdx;
		nDestIdx = this->m_cTrade.m_nTradeDest;
		// ȡ�����׵����ݻָ�
		this->m_ItemList.RecoverTrade();
		this->m_ItemList.ClearRoom(room_trade);
		this->m_ItemList.ClearRoom(room_trade1);
		this->m_cTrade.Release();
		Player[nDestIdx].m_ItemList.RecoverTrade();
		Player[nDestIdx].m_ItemList.ClearRoom(room_trade);
		Player[nDestIdx].m_ItemList.ClearRoom(room_trade1);
		Player[nDestIdx].m_cTrade.Release();
		
		TRADE_DECISION_SYNC	sSync;
		sSync.ProtocolType = s2c_tradedecision;
		sSync.m_btDecision = 0;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
		g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
		
		// ȡ�����׵�״̬�ָ�(����״̬һ�������� trading)(����)
		this->m_cMenuState.RestoreBackupState(m_nPlayerIndex);
		
		// ȡ�����׵�״̬�ָ�(����״̬һ�������� trading)(�Է�)
		Player[nDestIdx].m_cMenuState.RestoreBackupState(nDestIdx);
		
		return;
	}
	else if (pDecision->m_btDecision == 1)	// ִ�н���
	{
		// ���˫��û��ͬʱ����������ȷ��
		if (this->m_cTrade.m_nTradeLock != 1 ||
			Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock != 1)
		{
			SyncTradeState();
			return;
		}
		// �Է���ûȷ����֪ͨ�Է��Լ���ȷ��
		else if (Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState == 0)
		{
			m_cTrade.m_nTradeState = 1;
			
			SyncTradeState();
			
			return;
		}
		// �Է���ȷ����ִ�н���
		else
		{
			// ִ�н���֮ǰ���ȼ��һ���Ƿ��д���
			// -------------------- ��� money --------------------
			int	nSM1, nSM2, nDM1, nDM2;
			int	nDestIdx = m_cTrade.m_nTradeDest;
			nSM1 = m_ItemList.GetMoney(room_equipment);
			nSM2 = m_ItemList.GetMoney(room_trade);
			nDM1 = Player[nDestIdx].m_ItemList.GetMoney(room_equipment);
			nDM2 = Player[nDestIdx].m_ItemList.GetMoney(room_trade);
			if (nSM2 < 0 || nSM1 + nDM2 - nSM2 < 0)
			{
				this->m_ItemList.TradeMoveMoney(0);
				// ֪ͨ�ͻ���Ǯ������ not end
				return;
			}
			if (nDM2 < 0 || nDM1 + nSM2 - nDM2 < 0)
			{
				Player[nDestIdx].m_ItemList.TradeMoveMoney(0);
				// ֪ͨ�ͻ���Ǯ������ not end
				return;
			}
			// ------------------ ��� money end ------------------
			
			// --------- ���˫����Ʒ���ܷ�����������Ʒ ---------
			int		nSelfCanGet, nDestCanGet;
			
			// �жϼ�����Ʒ���ܷ�����������Ʒ
			Player[nDestIdx].m_ItemList.GetTradeRoomItemInfo();
			nSelfCanGet = m_ItemList.TradeCheckCanPlace();
			ItemSet.BackItemInfo();
			
			// �ж϶Է���Ʒ���ܷ�����������Ʒ
			this->m_ItemList.GetTradeRoomItemInfo();
			nDestCanGet = Player[nDestIdx].m_ItemList.TradeCheckCanPlace();
			
			if (!nSelfCanGet || !nDestCanGet)
			{
				if (!nSelfCanGet)
				{
					this->m_cTrade.m_nTradeState = 0;
					
					// ֪ͨ�ͻ�������Ķ����Ų���
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_SELF_ROOM_FULL;
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_DEST_ROOM_FULL;
					g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				}
				if (!nDestCanGet)
				{
					Player[nDestIdx].m_cTrade.m_nTradeState = 0;
					
					// ֪ͨ�ͻ�������Ķ����Ų���
					SHOW_MSG_SYNC	sMsg;
					sMsg.ProtocolType = s2c_msgshow;
					sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_SELF_ROOM_FULL;
					g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
					sMsg.m_wMsgID = enumMSG_ID_TRADE_DEST_ROOM_FULL;
					g_pServer->PackDataToClient(m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
				}
				SyncTradeState();
				return;
			}
			// ------- ���˫����Ʒ���ܷ�����������Ʒ end -------
			
			// ֪ͨ�ͻ��˽������
			TRADE_DECISION_SYNC	sSync;
			sSync.ProtocolType = s2c_tradedecision;
			sSync.m_btDecision = 1;
			g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
			g_pServer->PackDataToClient(Player[nDestIdx].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_DECISION_SYNC));
			
			// �������˽�������״̬
			m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_NORMAL);
			Player[nDestIdx].m_cMenuState.SetState(nDestIdx, PLAYER_MENU_STATE_NORMAL);
			this->m_cTrade.Release();
			Player[nDestIdx].m_cTrade.Release();
			
			// �����н�Ǯ�Ľ���
			if (nSM2 <= nDM2)
			{
				this->Earn(nDM2 - nSM2);
				Player[nDestIdx].Pay(nDM2 - nSM2);
			}
			else
			{
				this->Pay(nSM2 - nDM2);
				Player[nDestIdx].Earn(nSM2 - nDM2);
			}
			
			//--------------- ��������Ʒ�Ľ��� ---------------------
			// �Է�����Ʒ������
			int		nPos;
			nPos = 0;
			while (1)
			{
				if (nPos >= TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT)
					break;
				if (!ItemSet.m_psBackItemInfo[nPos].m_nIdx)
					break;
				// �Է�ȥ������Ʒ
				Player[nDestIdx].m_ItemList.Remove(ItemSet.m_psBackItemInfo[nPos].m_nIdx);
				// �����õ�����Ʒ
				m_ItemList.Add(
					ItemSet.m_psBackItemInfo[nPos].m_nIdx,
					pos_equiproom,
					ItemSet.m_psBackItemInfo[nPos].m_nX,
					ItemSet.m_psBackItemInfo[nPos].m_nY);
				nPos++;
			}
			// ��������Ʒ���Է�
			nPos = 0;
			while (1)
			{
				if (nPos >= TRADE_ROOM_WIDTH * TRADE_ROOM_HEIGHT)
					break;
				if (!ItemSet.m_psItemInfo[nPos].m_nIdx)
					break;
				// ����ȥ������Ʒ
				this->m_ItemList.Remove(ItemSet.m_psItemInfo[nPos].m_nIdx);
				// �Է��õ�����Ʒ
				Player[nDestIdx].m_ItemList.Add(
					ItemSet.m_psItemInfo[nPos].m_nIdx,
					pos_equiproom,
					ItemSet.m_psItemInfo[nPos].m_nX,
					ItemSet.m_psItemInfo[nPos].m_nY);
				nPos++;
			}
			//------------- ��������Ʒ�Ľ��� end -------------------
			return;
		}
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�ִ�л�ȡ������
//-------------------------------------------------------------------------
void	KPlayer::c2sTradeReplyStart(BYTE* pProtocol)
{
	if (this->CheckTrading())
		return;
	if (!pProtocol)
		return;
	TRADE_REPLY_START_COMMAND *pReply = (TRADE_REPLY_START_COMMAND*)pProtocol;
	
	int		nPlayerIdx = pReply->m_nDestIdx;
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return;
	if (Player[nPlayerIdx].m_nIndex <= 0)
		return;
	if (Player[nPlayerIdx].m_cTrade.m_nApplyIdx != this->m_nPlayerIndex)
		return;
	
	// �ܾ�
	if (pReply->m_bDecision == 0)
	{
		SHOW_MSG_SYNC	sMsg;
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TRADE_REFUSE_APPLY;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1;
		sMsg.m_lpBuf = (LPVOID)Npc[m_nIndex].m_dwID;
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
		sMsg.m_lpBuf = 0;
		return;
	}
	
	if (this->m_cMenuState.m_nState != PLAYER_MENU_STATE_TRADEOPEN)
	{
		return;
	}
	
	// ������ڶ��鿪��״̬���ر�֮
	if (Player[nPlayerIdx].m_cMenuState.m_nState == PLAYER_MENU_STATE_TEAMOPEN)
	{
		PLAYER_TEAM_OPEN_CLOSE	sClose;
		sClose.m_btOpenClose = 0;
		Player[nPlayerIdx].SetTeamState((BYTE*)&sClose);
	}
	// �����Ļ���Ҫվ����
	if (Npc[m_nIndex].m_Doing == do_sit)
	{
		Npc[m_nIndex].SendCommand(do_stand);
	}
	if (Npc[Player[nPlayerIdx].m_nIndex].m_Doing == do_sit)
	{
		Npc[Player[nPlayerIdx].m_nIndex].SendCommand(do_stand);
	}
	
	// �������Ĵ������ı�״̬����������
	this->m_ItemList.StartTrade();
	this->m_cTrade.Release();
	this->m_cTrade.StartTrade(nPlayerIdx);
	this->m_cMenuState.SetState(m_nPlayerIndex, PLAYER_MENU_STATE_TRADING, NULL, 0, Npc[Player[nPlayerIdx].m_nIndex].m_dwID);
	
	Player[nPlayerIdx].m_ItemList.StartTrade();
	Player[nPlayerIdx].m_cTrade.Release();
	Player[nPlayerIdx].m_cTrade.StartTrade(m_nPlayerIndex);
	Player[nPlayerIdx].m_cMenuState.SetState(nPlayerIdx, PLAYER_MENU_STATE_TRADING, NULL, 0, Npc[m_nIndex].m_dwID);
}
#endif

//-------------------------------------------------------------------------
//	���ܣ�������˫���Ŀͻ��˷��ͽ���״̬��Ϣ
//-------------------------------------------------------------------------
#ifdef _SERVER
void	KPlayer::SyncTradeState()
{
	if ( !CheckTrading() )
		return;
	
	TRADE_STATE_SYNC	sSync;
	sSync.ProtocolType = s2c_tradepressoksync;
	sSync.m_btSelfLock = this->m_cTrade.m_nTradeLock;
	sSync.m_btDestLock = Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock;
	sSync.m_btSelfOk = m_cTrade.m_nTradeState;
	sSync.m_btDestOk = Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_STATE_SYNC));
	
	sSync.m_btSelfLock = Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeLock;
	sSync.m_btDestLock = m_cTrade.m_nTradeLock;
	sSync.m_btSelfOk = Player[m_cTrade.m_nTradeDest].m_cTrade.m_nTradeState;
	sSync.m_btDestOk = m_cTrade.m_nTradeState;
	g_pServer->PackDataToClient(Player[m_cTrade.m_nTradeDest].m_nNetConnectIdx, (BYTE*)&sSync, sizeof(TRADE_STATE_SYNC));
}
#endif

//-------------------------------------------------------------------------
//	���ܣ������Լ�װ�������ϵ�װ����Ϣ�����˿�
//-------------------------------------------------------------------------
#ifdef _SERVER
void	KPlayer::SendEquipItemInfo(int nTargetPlayer)
{
	if (nTargetPlayer <= 0 || nTargetPlayer >= MAX_PLAYER || Player[nTargetPlayer].m_nIndex <= 0)
		return;
	
	int				nIdx, i, j;
	SViewItemInfo	*pInfo;
	VIEW_EQUIP_SYNC	sView;
	
	sView.ProtocolType = s2c_viewequip;
	sView.m_dwNpcID = Npc[m_nIndex].m_dwID;
	for (i = 0; i < itempart_num; i++)
	{
		nIdx = m_ItemList.m_EquipItem[i];
		pInfo = &sView.m_sInfo[i];
		if (nIdx <= 0)
		{
			memset(pInfo, 0, sizeof(SViewItemInfo));
			continue;
		}
		pInfo->m_nID			= Item[nIdx].GetID();
		pInfo->m_btGenre		= Item[nIdx].GetGenre();
		pInfo->m_btDetail		= Item[nIdx].GetDetailType();
		pInfo->m_btParticur		= Item[nIdx].GetParticular();
		pInfo->m_btSeries		= Item[nIdx].GetSeries();
		pInfo->m_btLevel		= Item[nIdx].GetLevel();
		pInfo->m_bStack			= Item[nIdx].GetStackNum();
		pInfo->m_bEnChance		= Item[nIdx].GetEnChance();
		pInfo->m_bPoint			= Item[nIdx].IsPurple();
		pInfo->m_nGoldId		= Item[nIdx].GetGoldId();
		pInfo->m_btLuck			= Item[nIdx].m_GeneratorParam.nLuck;
		pInfo->m_dwRandomSeed	= Item[nIdx].m_GeneratorParam.uRandomSeed;
		pInfo->m_wVersion		= Item[nIdx].m_GeneratorParam.nVersion;
		for (j = 0; j < 6; j++)
			pInfo->m_btMagicLevel[j] = Item[nIdx].m_GeneratorParam.nGeneratorLevel[j];
	}
	
	g_pServer->PackDataToClient(Player[nTargetPlayer].m_nNetConnectIdx, (BYTE*)&sView, sizeof(sView));
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ������봫���ŵ���صĺ���
//-------------------------------------------------------------------------
void KPlayer::UseTownPortal()
{
	if (m_nPlayerIndex <= 0)
		return;
	if (m_nIndex <= 0)
		return;
	if (Npc[m_nIndex].m_SubWorldIndex < 0)
		return;
	
	if (!Npc[m_nIndex].m_FightMode)
	{
		return;
	}
	
	m_sPortalPos.m_nSubWorldId = SubWorld[Npc[m_nIndex].m_SubWorldIndex].m_SubWorldID;
	Npc[m_nIndex].GetMpsPos(&m_sPortalPos.m_nMpsX, &m_sPortalPos.m_nMpsY);
	m_sPortalPos.m_nTime = TOWN_PORTAL_TIME;
	
	Npc[m_nIndex].ChangeWorld(m_sDeathRevivalPos.m_nSubWorldID, m_sDeathRevivalPos.m_nMpsX, m_sDeathRevivalPos.m_nMpsY);
	Npc[m_nIndex].SetFightMode(0);
}

void KPlayer::BackToTownPortal()
{
	if (m_nPlayerIndex <= 0)
		return;
	if (m_nIndex <= 0)
		return;
	if (m_sPortalPos.m_nTime <= 0)
		return;
	
	Npc[m_nIndex].ChangeWorld(m_sPortalPos.m_nSubWorldId, m_sPortalPos.m_nMpsX, m_sPortalPos.m_nMpsY);
	m_sPortalPos.m_nSubWorldId = 0;
	m_sPortalPos.m_nTime = 0;
	m_sPortalPos.m_nMpsX = 0;
	m_sPortalPos.m_nMpsY = 0;
	Npc[m_nIndex].SetFightMode(TRUE);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	���ܣ�������������
//-------------------------------------------------------------------------
void KPlayer::Revive(int nType)
{
	int	nSubWorldID = 0;
	int nMpsX = 0, nMpsY = 0;

	NPC_REVIVE_SYNC	Sync;
	Sync.ProtocolType = s2c_playerrevive;
	Sync.ID = Npc[m_nIndex].m_dwID;
	Sync.Type = nType;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (const void*)&Sync, sizeof(NPC_REVIVE_SYNC));
	Npc[m_nIndex].BroadCastRevive(nType);

	switch(nType)
	{
	case REMOTE_REVIVE_TYPE:
		GetDeathRevivalPos(&nSubWorldID, &nMpsX, &nMpsY);
		Npc[m_nIndex].ChangeWorld(nSubWorldID, nMpsX, nMpsY);
		Npc[m_nIndex].m_FightMode = 0;
		break;
	case LOCAL_REVIVE_TYPE:
		Npc[m_nIndex].m_FightMode = 1;
		break;
	default:
		break;
	}
	
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	//RestoreLiveData();	
	Npc[m_nIndex].SendCommand(do_revive);
}


void KPlayer::RestoreLiveData()
{
	Npc[m_nIndex].RestoreNpcBaseInfo();
}

BOOL KPlayer::Pay(int nMoney)
{
	if (nMoney < 0)
		return FALSE;
	
	return m_ItemList.CostMoney(nMoney);
}

BOOL KPlayer::PayXu(int nXu)
{
	if (nXu < 0)
		return FALSE;
	
	return m_ItemList.CostXu(nXu);
}

BOOL KPlayer::PrePay(int nMoney)
{
	if (nMoney < 0)
		return FALSE;

	if (Pay(nMoney))
	{
		m_nPrePayMoney = nMoney;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
BOOL KPlayer::PrePayXu(int nXu)
{
	if (nXu < 0)
		return FALSE;

	if (Pay(nXu))
	{
		m_nPrePayXu = nXu;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL KPlayer::Earn(int nMoney)
{
	if (nMoney < 0)
		return FALSE;
	return m_ItemList.AddMoney(room_equipment, nMoney);
}

BOOL KPlayer::EarnXu(int nXu)
{
	if (nXu < 0)
		return FALSE;
	return m_ItemList.AddXu(room_equipment, nXu);
}

//�������������ݿ��л�����ȫ�����ݣ�������֮��ʹ�������Ч
void	KPlayer::LaunchPlayer()
{
	int nSubWorld = Npc[m_nIndex].m_SubWorldIndex;
	int nRegion = Npc[m_nIndex].m_RegionIndex;
	int nX = Npc[m_nIndex].m_MapX;
	int nY = Npc[m_nIndex].m_MapY;
	int nOffX = Npc[m_nIndex].m_OffX;
	int nOffY = Npc[m_nIndex].m_OffY;
	SubWorld[nSubWorld].AddPlayer(nRegion, m_nPlayerIndex);
	m_ulLastSaveTime = g_SubWorldSet.GetGameTime();
	if (SubWorld[nSubWorld].m_Region[nRegion].GetBarrierMin(nX, nY, nOffX, nOffY, FALSE) != Obstacle_NULL)
	{
		Npc[m_nIndex].ChangeWorld(m_sLoginRevivalPos.m_nSubWorldID, m_sLoginRevivalPos.m_nMpsX, m_sLoginRevivalPos.m_nMpsY);
	}
	this->ExecuteScript("\\script\\system\\startserver.lua","startgame","");
}
#endif


BOOL	KPlayer::ExecuteScript(char * ScriptFileName, char * szFunName, int nParam)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	return ExecuteScript(g_FileName2Id(ScriptFileName), szFunName, nParam);	
}
#define MAX_TRYEXECUTESCRIPT_COUNT 5
BOOL	KPlayer::ExecuteScript(DWORD dwScriptId,  char * szFunName, int nParam)
{
/*	bool	bCanExecuteScript = true;

  //��ǰ�ű�δ�ÿ�
  if (m_bWaitingPlayerFeedBack)
  {
		if (Npc[m_nIndex].m_ActionScriptID && (dwScriptId != Npc[m_nIndex].m_ActionScriptID))
		{
		m_btTryExecuteScriptTimes ++;
		if (m_btTryExecuteScriptTimes <= MAX_TRYEXECUTESCRIPT_COUNT)//�����ִ�нű��Ĵ�������������󣬾�ִ�е�ǰ�ű���������ԭ���Ľű���
		{
		bCanExecuteScript = false;
		}
		else
		{
		m_btTryExecuteScriptTimes = 0;
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		}
		}
		}
		
		  if (!bCanExecuteScript) return FALSE;
	*/
	try
	{
		m_btTryExecuteScriptTimes = 0;
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		if (pScript)
		{
			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			int nTopIndex = 0;

			pScript->SafeCallBegin(&nTopIndex);
			if (pScript->CallFunction(szFunName,0, "d", nParam)) 
			{
				bExecuteScriptMistake = false;
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_btTryExecuteScriptTimes = 0;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}

BOOL	KPlayer::ExecuteScript(DWORD dwScriptId, char * szFunName, char *  szParams)
{
/*bool	bCanExecuteScript = true;

  //��ǰ�ű�δ�ÿ�
  if (m_bWaitingPlayerFeedBack)
  {
		if (Npc[m_nIndex].m_ActionScriptID && (dwScriptId != Npc[m_nIndex].m_ActionScriptID))
		{
		m_btTryExecuteScriptTimes ++;
		if (m_btTryExecuteScriptTimes <= MAX_TRYEXECUTESCRIPT_COUNT)//�����ִ�нű��Ĵ�������������󣬾�ִ�е�ǰ�ű���������ԭ���Ľű���
		{
		bCanExecuteScript = false;
		}
		else
		{
		m_btTryExecuteScriptTimes = 0;
		m_bWaitingPlayerFeedBack = false;
		Npc[m_nIndex].m_ActionScriptID = 0;
		}
		}
		}
		
		  bCanExecuteScript = true;//test
		  
			if (!bCanExecuteScript) return FALSE;
	*/
	
	try
	{
		m_btTryExecuteScriptTimes = 0;
		bool bExecuteScriptMistake = true;
		KLuaScript * pScript = (KLuaScript* )g_GetScript(dwScriptId);
		int nTopIndex = 0;
		
		if (pScript)
		{
			Npc[m_nIndex].m_ActionScriptID = dwScriptId;
			Lua_PushNumber(pScript->m_LuaState, m_nPlayerIndex);
			pScript->SetGlobalName(SCRIPT_PLAYERINDEX);
			
			Lua_PushNumber(pScript->m_LuaState, m_dwID);
			pScript->SetGlobalName(SCRIPT_PLAYERID);
			
			Lua_PushNumber(pScript->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
			pScript->SetGlobalName(SCRIPT_SUBWORLDINDEX);
			
			pScript->SafeCallBegin(&nTopIndex);
			
			if ( (!szParams) || !szParams[0]) 
			{
				if (pScript->CallFunction(szFunName, 0,""))
				{
					bExecuteScriptMistake = false;
				}
			}
			else
			{
				if (pScript->CallFunction(szFunName,0, "sd", szParams,0)) 
				{
					bExecuteScriptMistake = false;
				}
			}
			pScript->SafeCallEnd(nTopIndex);
		}
		
		if (bExecuteScriptMistake)
		{
			m_bWaitingPlayerFeedBack = false;
			m_btTryExecuteScriptTimes = 0;
			Npc[m_nIndex].m_ActionScriptID = 0;
			return FALSE;
		}
		return TRUE;
	}
	catch(...)
	{
		printf("Exception Have Caught When Execute Script[%d]!!!!!", dwScriptId);
		m_bWaitingPlayerFeedBack = false;
		m_btTryExecuteScriptTimes = 0;
		Npc[m_nIndex].m_ActionScriptID = 0;
		return FALSE;
	}
	return TRUE;
}


BOOL	KPlayer::ExecuteScript(char * ScriptFileName, char * szFunName, char *  szParams)
{
	if (!ScriptFileName || !ScriptFileName[0] || !szFunName  || !szFunName[0]) return FALSE;
	DWORD dwScriptId = g_FileName2Id(ScriptFileName);
	return ExecuteScript(dwScriptId, szFunName, szParams);
}

BOOL	KPlayer::DoScript(char * ScriptCommand)
{
	if (NULL == ScriptCommand) return FALSE;
	KLuaScript *Script = new KLuaScript;
	Script->Init();
	Script->RegisterFunctions(GameScriptFuns, g_GetGameScriptFunNum());
	
	//GM Standand Script Functions 
	Script->Load("\\script\\system\\startserver.lua.lua");
	
	
	Lua_PushNumber(Script->m_LuaState, m_nPlayerIndex);
	Script->SetGlobalName(SCRIPT_PLAYERINDEX);
	Lua_PushNumber(Script->m_LuaState, m_dwID);
	Script->SetGlobalName(SCRIPT_PLAYERID);
	
	Lua_PushNumber(Script->m_LuaState, Npc[m_nIndex].m_SubWorldIndex);
	Script->SetGlobalName(SCRIPT_SUBWORLDINDEX);
	
	if (Script->LoadBuffer((PBYTE)ScriptCommand, strlen(ScriptCommand))) 
	{
		BOOL bResult = Script->ExecuteCode();	
		delete Script; //Question!
		return bResult;
	}
	delete Script; //Question!
	return FALSE;
}


void	KPlayer::DoScriptAction(PLAYER_SCRIPTACTION_SYNC * pUIInfo) //Ҫ����ʾĳ��UI����
{
	if (!pUIInfo) return;
	
	//�������˽ű�ʱ
	if (pUIInfo->m_bParam2 == 1)
	{
#ifdef _SERVER
		pUIInfo->ProtocolType = (BYTE)s2c_scriptaction;
		pUIInfo->m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - MAX_SCIRPTACTION_BUFFERNUM + pUIInfo->m_nBufferLen - 1;
		g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)pUIInfo, pUIInfo->m_wProtocolLong + 1 );
#else
	}
	else //�ͻ��˽ű�Ҫ����ʾ�ű� ֱ������
	{
		OnScriptAction((PLAYER_SCRIPTACTION_SYNC *)pUIInfo);
#endif
	}
	
}

//�������˻�֪���ѡ����ĳ��󣬴���~~
void	KPlayer::ProcessPlayerSelectFromUI(BYTE* pProtocol)			// ��������Ҵ�ѡ��˵�ѡ��ĳ��ʱ�Ĳ���	
{
	PLAYER_SELECTUI_COMMAND * pSelUI = (PLAYER_SELECTUI_COMMAND*) pProtocol;
	m_bWaitingPlayerFeedBack = false;
	//������ظ�������ʾ�˳��ýű�ִ�л���
	if (pSelUI->nSelectIndex < 0) m_nAvailableAnswerNum = 0;
	if (m_nAvailableAnswerNum > pSelUI->nSelectIndex)
	{
		if (m_szTaskAnswerFun[pSelUI->nSelectIndex][0])
		{
			//			g_SetFilePath("\\script");
			if (m_nIndex)
			{
				ExecuteScript(Npc[m_nIndex].m_ActionScriptID, m_szTaskAnswerFun[pSelUI->nSelectIndex], pSelUI->nSelectIndex);
			}
		}
	}
}

#ifndef _SERVER
//����ڽ��潻����ѡ����ĳ�����������˷���
void	KPlayer::OnSelectFromUI(PLAYER_SELECTUI_COMMAND * pSelectUI, UIInfo eUIInfo)	//����Ҵ�ѡ�����ѡ��ĳ��󣬽������������			
{
	if (!pSelectUI) return;
	
	switch(eUIInfo)
	{
	case UI_SELECTDIALOG:
		{
			if (g_bUISelIntelActiveWithServer)
			{
				pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
			}
			else
			{
				ProcessPlayerSelectFromUI((BYTE *)pSelectUI);			// ��������Ҵ�ѡ��˵�ѡ��ĳ��ʱ�Ĳ���	
			}
		}
		break;
	case UI_TALKDIALOG:
		{
			if (g_bUISpeakActiveWithServer)
			{
				pSelectUI->ProtocolType = (BYTE)c2s_playerselui;
				if (g_pClient)
					g_pClient->SendPackToServer((BYTE*)pSelectUI, sizeof(PLAYER_SELECTUI_COMMAND));
			}
			else
			{
				ProcessPlayerSelectFromUI((BYTE *)pSelectUI);			// ��������Ҵ�ѡ��˵�ѡ��ĳ��ʱ�Ĳ���	
			}
			
		}break;
	}
}
#endif

//-------------------------------------------------------------------------
//	���ܣ����֪ͨĳ����������
//-------------------------------------------------------------------------
void	KPlayer::ChatFriendOnLine(DWORD dwID, int nFriendIdx)
{
	if (nFriendIdx <= 0)
		return;
	for (int i = 0; i < MAX_FRIEND_TEAM; i++)
	{
		if (m_cChat.m_cFriendTeam[i].m_nFriendNo == 0)
			continue;
		CChatFriend	*pFriend;
		pFriend = (CChatFriend*)m_cChat.m_cFriendTeam[i].m_cEveryOne.GetHead();
		while (pFriend)
		{
			if (pFriend->m_dwID == dwID)
			{
				pFriend->m_nPlayerIdx = nFriendIdx;
#ifdef _SERVER
				CHAT_FRIEND_ONLINE_SYNC	sFriend;
				sFriend.ProtocolType = s2c_chatfriendonline;
				sFriend.m_dwID = dwID;
				sFriend.m_nPlayerIdx = nFriendIdx;
				g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&sFriend, sizeof(CHAT_FRIEND_ONLINE_SYNC));
#else
				// ֪ͨ�����к�������
				KUiPlayerItem	sPlayer;
				strcpy(sPlayer.Name, pFriend->m_szName);
				sPlayer.uId = dwID;
				sPlayer.nIndex = nFriendIdx;
				sPlayer.nData = CHAT_S_ONLINE;
				CoreDataChanged(GDCNI_CHAT_FRIEND_STATUS, (unsigned int)&sPlayer, i);
				
				KSystemMessage	sMsg;
				sprintf(sMsg.szMessage, MSG_CHAT_FRIEND_ONLINE, pFriend->m_szName);
				sMsg.eType = SMT_NORMAL;
				sMsg.byConfirmType = SMCT_NONE;
				sMsg.byPriority = 0;
				sMsg.byParamSize = 0;
				CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
#endif
				return;
			}
			pFriend = (CChatFriend*)pFriend->GetNext();
		}
	}
#ifdef _SERVER
	// ���û�ҵ���˵�������Ѿ�ɾ���˶Է�������֪ͨ�Է�ɾ��
	Player[nFriendIdx].m_cChat.DeleteFriendData(m_dwID, nFriendIdx);
#endif
}

// ��õ�ǰ��������
void	KPlayer::GetFactionName(char *lpszName, int nSize)
{
	this->m_cFaction.GetCurFactionName(lpszName);
	return;
}
#ifdef _SERVER
void	KPlayer::S2CExecuteScript(char * ScriptName, char * szParam = NULL)
{
	if (!ScriptName || (!ScriptName[0])) return; 
	
	PLAYER_SCRIPTACTION_SYNC  ScriptAction;
	ScriptAction.m_nOperateType = SCRIPTACTION_EXESCRIPT;
	ScriptAction.ProtocolType = s2c_scriptaction;
	char * script = NULL;
	if (szParam == NULL || szParam[0] == 0) 
	{
		ScriptAction.m_nBufferLen = strlen(ScriptName) + 1 ;
		strcpy(ScriptAction.m_pContent, ScriptName);
	}
	else
	{
		ScriptAction.m_nBufferLen = strlen(ScriptName) + 2 + strlen(szParam);
		sprintf(ScriptAction.m_pContent, "%s|%s", ScriptName, szParam);
	}
	ScriptAction.m_wProtocolLong = sizeof(PLAYER_SCRIPTACTION_SYNC) - 300 + ScriptAction.m_nBufferLen - 1;
	g_pServer->PackDataToClient(m_nNetConnectIdx, (BYTE*)&ScriptAction, sizeof(PLAYER_SCRIPTACTION_SYNC) - 300 + ScriptAction.m_nBufferLen);	
	
}
#endif

#ifndef _SERVER
void	KPlayer::OnScriptAction(PLAYER_SCRIPTACTION_SYNC * pMsg)
{
	PLAYER_SCRIPTACTION_SYNC * pScriptAction = (PLAYER_SCRIPTACTION_SYNC *)pMsg;
	char szString[1000];
	
	switch(pScriptAction->m_nOperateType)
	{
	case SCRIPTACTION_UISHOW:
		{
			switch(pScriptAction->m_bUIId)
			{
			case UI_SELECTDIALOG://֪ͨ�ͻ�����ʾѡ�񴰿�
				{
					KUiQuestionAndAnswer	*pQuest = NULL;
					KUiNpcSpr                *pImage = NULL;
					if (pScriptAction->m_nBufferLen <= 0) break;
					
					if (pScriptAction->m_bOptionNum <= 0)
						pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer));
					else
						pQuest = (KUiQuestionAndAnswer *)malloc(sizeof(KUiQuestionAndAnswer) + sizeof(KUiAnswer) * (pScriptAction->m_bOptionNum - 1));
					pImage = (KUiNpcSpr *)malloc(sizeof(KUiNpcSpr));
					char strContent[1024];
					char * pAnswer = NULL;
					pQuest->AnswerCount = 0;
					//����ϢΪ�ַ���
					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
						pAnswer = strstr(strContent, "|");
						if (!pAnswer)
						{
							pScriptAction->m_bOptionNum = 0;
							pQuest->AnswerCount = 0;
						}
						else
							*pAnswer++ = 0;
						
						g_StrCpyLen(pQuest->Question, strContent, sizeof(pQuest->Question));
						pQuest->QuestionLen = TEncodeText(pQuest->Question, strlen(pQuest->Question));
					}
					//����ϢΪ���ֱ�ʶ
					else 
					{
						g_StrCpyLen(pQuest->Question, g_GetStringRes(*(int *)pScriptAction->m_pContent, szString, 1000), sizeof(pQuest->Question));
						pQuest->QuestionLen = TEncodeText(pQuest->Question, strlen(pQuest->Question));
						
						g_StrCpyLen(strContent, pScriptAction->m_pContent + sizeof(int), pScriptAction->m_nBufferLen - sizeof(int) + 1);
						pAnswer = strContent + 1;
					}
					
					for (int i = 0; i < pScriptAction->m_bOptionNum; i ++)
					{
						char * pNewAnswer = strstr(pAnswer, "|");
						
						if (pNewAnswer)
						{
							*pNewAnswer = 0;
							strcpy(pQuest->Answer[i].AnswerText, pAnswer);
							pQuest->Answer[i].AnswerLen = -1;
							pAnswer = pNewAnswer + 1;
						}
						else
						{
							strcpy(pQuest->Answer[i].AnswerText, pAnswer);
							pQuest->Answer[i].AnswerLen = -1;
							pQuest->AnswerCount = i + 1;
							break;
						}
					}
					
					g_bUISelIntelActiveWithServer = pScriptAction->m_bParam2;
					g_bUISelLastSelCount = pQuest->AnswerCount;
                    /*******************************************Code by thienthanden2*******************************************************/ 
                    if (m_nImageNpcID) 
                    { 
                    char szBuffer[128]; 
                    for (int i = 0; i < 16; i++) 
                        { Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetFileName(i, 3, 0, "", szBuffer, sizeof(szBuffer)); 
                        if (szBuffer[0]) 
                            { 
                            strcpy(pImage->ImageFile, szBuffer); 
                            pImage->MaxFrame = (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalFrames(i, 3, 0, 16))/ 
                            (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalDirs(i, 3, 0, 16)); goto Next; 
                            } 
                        } 
                    for (int j = 0; j < 16; j++) 
                        {  
                         Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetFileName(j, 0, 0, "", szBuffer, sizeof(szBuffer)); 
                        if (szBuffer[0]) 
                            { 
                            strcpy(pImage->ImageFile, szBuffer); 
                            pImage->MaxFrame = (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalFrames(j, 0, 0, 16))/ 
                            (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalDirs(j, 0, 0, 16)); goto Next; 
                            } 
                        } 
                     
                    } 
                    Next: 
                    if (pScriptAction->m_Select == 1 && m_nImageNpcID) 
                    CoreDataChanged(GDCNI_QUESTION_CHOOSE,(unsigned int) pQuest, (int) pImage); 
                    else CoreDataChanged(GDCNI_QUESTION_CHOOSE,(unsigned int) pQuest, 0); 
                       free(pImage); 
                    pImage = NULL; 
                    /******************************************the end********************************************************/ 
                     
                    free(pQuest); 
                    pQuest = NULL; 
                } 
                break;  
				
			case UI_TALKDIALOG: 
				{
					BOOL bUsingSpeakId = pScriptAction->m_bParam1;
					int	 nSentenceCount = pScriptAction->m_bOptionNum;
					if (nSentenceCount <= 0) return ;
					KUiInformationParam *pSpeakList  = new KUiInformationParam[nSentenceCount];
					memset(pSpeakList, 0, sizeof(KUiInformationParam) * nSentenceCount);
					
					char * pAnswer = new char [pScriptAction->m_nBufferLen + 1];
					char * pBackupAnswer = pAnswer;
					g_StrCpyLen(pAnswer, pScriptAction->m_pContent, pScriptAction->m_nBufferLen + 1);
					
					int nCount = 0;
					for (int i = 0; i < pScriptAction->m_bOptionNum; i ++)
					{
						char * pNewAnswer = strstr(pAnswer, "|");
						
						if (pNewAnswer)
						{
							*pNewAnswer = 0;
							if (!bUsingSpeakId)
							{
								strcpy(pSpeakList[i].sInformation, pAnswer);
								
							}
							else
							{
								strcpy(pSpeakList[i].sInformation, g_GetStringRes(atoi(pAnswer), szString, sizeof(szString)));
							}
							
							
							if (i < pScriptAction->m_bOptionNum - 1)
								strcpy(pSpeakList[i].sConfirmText, "Kplayer so 1");
							else 
							{
								strcpy(pSpeakList[i].sConfirmText, "Kplayer so 2");
								if (pScriptAction->m_nParam == 1)						
									pSpeakList[i].bNeedConfirmNotify = TRUE;
								
							}
							pSpeakList[i].nInforLen = TEncodeText(pSpeakList[i].sInformation, strlen(pSpeakList[i].sInformation));
							pAnswer = pNewAnswer + 1;
						}
						else
						{
							if (!bUsingSpeakId)
							{
								strcpy(pSpeakList[i].sInformation, pAnswer);
							}
							else
							{
								strcpy(pSpeakList[i].sInformation, g_GetStringRes(atoi(pAnswer), szString ,sizeof(szString)));
							}
							
							strcpy(pSpeakList[i].sConfirmText, "Kplayer so 3");
							
							if (pScriptAction->m_nParam == 1)						
								pSpeakList[i].bNeedConfirmNotify = TRUE;
							
							pSpeakList[i].nInforLen = TEncodeText(pSpeakList[i].sInformation, strlen(pSpeakList[i].sInformation));
							nCount++;
							break;
						}
						nCount ++;
					}
					if (pBackupAnswer)
					{
						delete []pBackupAnswer;
						pBackupAnswer = NULL;
					}
					g_bUISpeakActiveWithServer = pScriptAction->m_bParam2;
					CoreDataChanged(GDCNI_SPEAK_WORDS,(unsigned int)pSpeakList, nCount);
					if (pSpeakList)
					{
						delete pSpeakList;	
						pSpeakList = NULL;
					}
				}
				break;
			case UI_NOTEINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					KMissionRecord Record;
					//����ϢΪ�ַ���
					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(Record.sContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(int) - 1);
						Record.uValue = *(unsigned int*) (pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(int));
					}
					else
					{
						g_GetStringRes(*(int *)pScriptAction->m_pContent, Record.sContent ,sizeof(Record.sContent));
						Record.uValue = *(unsigned int* ) (Record.sContent + sizeof(int));
					}
					
					
					Record.nContentLen = TEncodeText(Record.sContent, strlen(Record.sContent));
					
					CoreDataChanged(GDCNI_MISSION_RECORD, (unsigned long)&Record, 0);
					
				}
				break;
			case UI_MSGINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					
					char strContent[1024];
					//����ϢΪ�ַ���
					if (pScriptAction->m_bParam1 == 0)
					{
						g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
					}
					else
					{
						g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
					}
					
					KSystemMessage	sMsg;
					sMsg.eType = SMT_PLAYER;
					sMsg.byConfirmType = SMCT_MSG_BOX;
					sMsg.byPriority = 0;
					sMsg.byParamSize = 0;
					g_StrCpyLen(sMsg.szMessage, strContent, sizeof(sMsg.szMessage));
					
					CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
					//CoreDataChanged(GDCNI_MISSION_RECORD, (unsigned int)strContent, strlen(strContent));
					
				}break;
				
			case UI_NEWSINFO:
				{
					if (pScriptAction->m_nBufferLen <= 0) 
						break;
					switch(pScriptAction->m_bOptionNum) //OPtionNum��������һ��News����
					{
					case NEWSMESSAGE_NORMAL:
						{
							
							KNewsMessage News;
							News.nType = NEWSMESSAGE_NORMAL;
							
							char strContent[1024];
							//����ϢΪ�ַ���
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));

							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, 0);
						}
						break;
						//STRING|STRINGID + TIME(INT)
					case NEWSMESSAGE_COUNTING:
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							//����ϢΪ�ַ���
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(int) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							SYSTEMTIME systime;
							
							memset(&systime, 0, sizeof(SYSTEMTIME));
							systime.wSecond = *(int *)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(int)));
							
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, (unsigned int)&systime);
							
						}break;

					case NEWSMESSAGE_TIMEEND:
						{
							KNewsMessage News;
							News.nType = pScriptAction->m_bOptionNum;
							
							char strContent[1024];
							int nTime = 0;
							//����ϢΪ�ַ���
							if (pScriptAction->m_bParam1 == 0)
							{
								g_StrCpyLen(strContent, pScriptAction->m_pContent,  pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME) + 1);
							}
							else
							{
								g_GetStringRes(*(int *)pScriptAction->m_pContent, strContent ,sizeof(strContent));
							}
							
							g_StrCpyLen(News.sMsg,  strContent, sizeof(News.sMsg));
							News.nMsgLen = TEncodeText(News.sMsg, strlen(News.sMsg));
							SYSTEMTIME systime;
							systime = *(SYSTEMTIME*)((pScriptAction->m_pContent + pScriptAction->m_nBufferLen - sizeof(SYSTEMTIME)));
							CoreDataChanged(GDCNI_NEWS_MESSAGE, (unsigned int)&News, (unsigned int)&systime);
							
						}break;
					
					}
					
				}break;
			case UI_PLAYMUSIC:
				{
					char szMusicFile[MAX_PATH];
					memcpy(szMusicFile, pScriptAction->m_pContent, pScriptAction->m_nBufferLen);
					szMusicFile[pScriptAction->m_nBufferLen] = 0;
					g_SubWorldSet.m_cMusic.ScriptPlay(szMusicFile);
				}break;
			
			case UI_OPENTONGUI:				
				{
					Player[CLIENT_PLAYER_INDEX].m_cTong.OpenCreateInterface();
					break;
				}
			}
	} break;
	case SCRIPTACTION_EXESCRIPT://Ҫ��ͻ��˵���ĳ���ű�
		{
			if (pScriptAction->m_nBufferLen <= 0 ) break;
			char szScriptInfo[1000];
			g_StrCpyLen(szScriptInfo, pScriptAction->m_pContent,pScriptAction->m_nBufferLen + 1);
			char * pDivPos = strstr(szScriptInfo, "/");
			if (pDivPos)	*pDivPos++ = 0; 
			if (pDivPos)
				ExecuteScript(szScriptInfo, "OnCall", pDivPos);
			else
				ExecuteScript(szScriptInfo, "OnCall", "");
		}
		break;
	}
	
	
}
#endif

#ifdef _SERVER
int KPlayer::AddTempTaskValue(void* pData)
{
	memcpy(m_cTask.nClear, pData, sizeof(int) * MAX_TEMP_TASK);
	return 1;
}

//�ͻ���������ĳ��Npc�Ի�
//�������汾
void	KPlayer::DialogNpc(BYTE * pProtocol)
{
	//if (m_nAvailableAnswerNum > 0) return ;
	PLAYER_DIALOG_NPC_COMMAND * pDialogNpc = (PLAYER_DIALOG_NPC_COMMAND*) pProtocol;
	int nIdx = 0;
	if (pDialogNpc->nNpcId < 0) return;
	nIdx = FindAroundNpc(pDialogNpc->nNpcId);//NpcSet.SearchID(pDialogNpc->nNpcId);
	if (nIdx > 0) 
	{
		int distance = NpcSet.GetDistance(nIdx, m_nIndex);
		// С�ڶԻ��뾶�Ϳ�ʼ�Ի�
		if ((Npc[nIdx].m_Kind == kind_dialoger) ||
			(NpcSet.GetRelation(m_nIndex, nIdx) == relation_none))
		{
			if (distance <= Npc[nIdx].m_DialogRadius * 2)	// �Ŵ�server�Ի��뾶
			{
				if (Npc[nIdx].ActionScript[0])
				{
					ExecuteScript(Npc[nIdx].m_ActionScriptID, "main", "");
				}
			}
		}
	}
}
#endif

#ifndef _SERVER
void	KPlayer::s2cTradeChangeState(BYTE* pMsg)
{
	TRADE_CHANGE_STATE_SYNC	*pTrade = (TRADE_CHANGE_STATE_SYNC*)pMsg;
	
	
	switch (m_cMenuState.m_nState)
	{
	case PLAYER_MENU_STATE_NORMAL:		// ��ǰ�ͻ��� NORMAL
	case PLAYER_MENU_STATE_TRADEOPEN:	// ��ǰ�ͻ��� TRADEOPEN
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADING);
			m_cTrade.StartTrade(pTrade->m_dwNpcID);
			m_ItemList.StartTrade();
			
			// ֪ͨ������뽻�׽���
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	case PLAYER_MENU_STATE_TEAMOPEN:	// ��ǰ�ͻ��� TEAMOPEN
		g_Team[0].SetTeamClose();
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADING);
			m_cTrade.StartTrade(pTrade->m_dwNpcID);
			m_ItemList.StartTrade();
			
			// ֪ͨ������뽻�׽���
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	case PLAYER_MENU_STATE_TRADING:		// ��ǰ�ͻ��� TRADING
		// ���ͻ��˴��� TRADING ʱ����Ӧ���յ���Э��
		if (pTrade->m_btState == 0)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 1)
		{
			m_cMenuState.SetState(PLAYER_MENU_STATE_TRADEOPEN);
			CoreDataChanged(GDCNI_TRADE_OPER_DATA, 0, 0);
		}
		else if (pTrade->m_btState == 2)	// �ͻ��˴�ʱ��Ӧ�������״̬���ͻ����ѳ���
		{
			m_cMenuState.m_nState = PLAYER_MENU_STATE_TRADING;
			m_cMenuState.m_nTradeDest = pTrade->m_dwNpcID;
			m_cMenuState.m_nTradeState = 0;
			m_cMenuState.m_nTradeDestState = 0;
			this->m_ItemList.RecoverTrade();
			this->m_ItemList.BackupTrade();
			this->m_ItemList.ClearRoom(room_trade);
			this->m_ItemList.ClearRoom(room_trade1);
			
			// ֪ͨ������뽻�׽���
			KUiPlayerItem	sTradePlayer;
			int	nIdx;
			
			nIdx = NpcSet.SearchID(pTrade->m_dwNpcID);
			if (nIdx > 0)
				strcpy(sTradePlayer.Name, Npc[nIdx].Name);
			else
				sTradePlayer.Name[0] = 0;
			sTradePlayer.Name[0] = 0;
			sTradePlayer.nIndex = 0;
			sTradePlayer.uId = 0;
			sTradePlayer.nData = 0;
			CoreDataChanged(GDCNI_TRADE_START, (unsigned int)(&sTradePlayer), 0);
			
			if (Npc[m_nIndex].m_Doing == do_sit)
			{
				Npc[m_nIndex].SendCommand(do_stand);
			}
		}
		break;
	}
}
#endif

#ifndef _SERVER
void	KPlayer::s2cLevelUp(BYTE* pMsg)
{
	PLAYER_LEVEL_UP_SYNC	*pLevel = (PLAYER_LEVEL_UP_SYNC*)pMsg;

	if ((DWORD)Npc[m_nIndex].m_Level < (DWORD)pLevel->m_btLevel && m_nExp < PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level))
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_GET_EXP, PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level) - m_nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	
	m_nExp = 0;
	Npc[m_nIndex].m_Level = (DWORD)pLevel->m_btLevel;
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(Npc[m_nIndex].m_Level);
	
	// -------------------------- �ȼ�����ʱ��������ͬ�� ----------------------
	// ͬ��δ�������Ե�
	if (pLevel->m_nAttributePoint > m_nAttributePoint)
	{
		KSystemMessage Msg;
		sprintf(Msg.szMessage, MSG_GET_ATTRIBUTE_POINT, pLevel->m_nAttributePoint - m_nAttributePoint);
		Msg.eType = SMT_PLAYER;
		Msg.byConfirmType = SMCT_UI_ATTRIBUTE;
		Msg.byPriority = 3;
		Msg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	}
	m_nAttributePoint = pLevel->m_nAttributePoint;
	
	// ͬ��δ���似�ܵ�
	if (m_nSkillPoint < pLevel->m_nSkillPoint)
	{
		KSystemMessage Msg;
		sprintf(Msg.szMessage, MSG_GET_SKILL_POINT, pLevel->m_nSkillPoint - m_nSkillPoint);
		Msg.eType = SMT_PLAYER;
		Msg.byConfirmType = SMCT_UI_SKILLS;
		Msg.byPriority = 3;
		Msg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	}
	m_nSkillPoint = pLevel->m_nSkillPoint;
	
	// ��������������������ֵ��Ӱ������������������������ֵ�����أ��ȼ������Ե㣬Ӱ�쵱ǰ�������ֵ�����أ�װ�������ܡ�ҩ���ʱ����
	Npc[m_nIndex].m_LifeMax = pLevel->m_nBaseLifeMax;
	Npc[m_nIndex].m_StaminaMax = pLevel->m_nBaseStaminaMax;
	Npc[m_nIndex].m_ManaMax = pLevel->m_nBaseManaMax;
	//	Npc[m_nIndex].ResetLifeReplenish();
	Npc[m_nIndex].m_CurrentLifeMax = Npc[m_nIndex].m_LifeMax;
	Npc[m_nIndex].m_CurrentStaminaMax = Npc[m_nIndex].m_StaminaMax;
	Npc[m_nIndex].m_CurrentManaMax = Npc[m_nIndex].m_ManaMax;
	
	// �������ֿ��Եı仯 �𡢱��������硢����
	Npc[m_nIndex].m_FireResist				= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentFireResist		= Npc[m_nIndex].m_FireResist;
	Npc[m_nIndex].m_ColdResist				= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentColdResist		= Npc[m_nIndex].m_ColdResist;
	Npc[m_nIndex].m_PoisonResist			= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPoisonResist		= Npc[m_nIndex].m_PoisonResist;
	Npc[m_nIndex].m_LightResist				= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentLightResist		= Npc[m_nIndex].m_LightResist;
	Npc[m_nIndex].m_PhysicsResist			= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_CurrentPhysicsResist	= Npc[m_nIndex].m_PhysicsResist;
	Npc[m_nIndex].m_FireResistMax			= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax			= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax			= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax			= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax		= BASE_PHYSICS_RESIST_MAX;
	Npc[m_nIndex].m_CurrentFireResistMax	= Npc[m_nIndex].m_FireResistMax;
	Npc[m_nIndex].m_CurrentColdResistMax	= Npc[m_nIndex].m_ColdResistMax;
	Npc[m_nIndex].m_CurrentPoisonResistMax	= Npc[m_nIndex].m_PoisonResistMax;
	Npc[m_nIndex].m_CurrentLightResistMax	= Npc[m_nIndex].m_LightResistMax;
	Npc[m_nIndex].m_CurrentPhysicsResistMax	= Npc[m_nIndex].m_PhysicsResistMax;
	
	int nOldCurCamp = Npc[m_nIndex].m_CurrentCamp;
	// ����װ����������Ϣ���µ�ǰ����
	this->UpdataCurData();
	SetNpcPhysicsDamage();
	Npc[m_nIndex].m_CurrentCamp = nOldCurCamp;
	
	// ����������������ֵ����
	Npc[m_nIndex].m_CurrentLife = Npc[m_nIndex].m_CurrentLifeMax;
	Npc[m_nIndex].m_CurrentStamina = Npc[m_nIndex].m_CurrentStaminaMax;
	Npc[m_nIndex].m_CurrentMana = Npc[m_nIndex].m_CurrentManaMax;
	
	// �ͻ��˶������Լ��ĵȼ���һ
	if (this->m_cTeam.m_nFlag)
	{
		if (m_cTeam.m_nFigure == TEAM_CAPTAIN)
		{
			g_Team[0].m_nMemLevel[0] = Npc[m_nIndex].m_Level;
		}
		else
		{
			for (int i = 0; i < MAX_TEAM_MEMBER; i++)
			{
				if ((DWORD)g_Team[0].m_nMember[i] == Npc[m_nIndex].m_dwID)
				{
					g_Team[0].m_nMemLevel[i] = Npc[m_nIndex].m_Level;
					break;
				}
			}
		}
	}
	
	KSystemMessage Msg;
				
	Msg.byConfirmType = SMCT_CLICK;
	Msg.byParamSize = 0;
	Msg.byPriority = 1;
	Msg.eType = SMT_PLAYER;
	sprintf(Msg.szMessage, MSG_LEVEL_UP, Npc[m_nIndex].m_Level);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&Msg, NULL);
	
	Npc[this->m_nIndex].SetInstantSpr(enumINSTANT_STATE_LEVELUP);
}
#endif

#ifndef _SERVER
void	KPlayer::s2cGetCurAttribute(BYTE* pMsg)
{
	PLAYER_ATTRIBUTE_SYNC	*pAttribute = (PLAYER_ATTRIBUTE_SYNC*)pMsg;
	m_nAttributePoint = pAttribute->m_nLeavePoint;
	int nData;
	switch (pAttribute->m_btAttribute)
	{
	case ATTRIBUTE_STRENGTH:
		nData = pAttribute->m_nBasePoint - m_nStrength;
		m_nStrength = pAttribute->m_nBasePoint;
		m_nCurStrength = pAttribute->m_nCurPoint;
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	case ATTRIBUTE_DEXTERITY:
		nData = pAttribute->m_nBasePoint - m_nDexterity;
		m_nDexterity = pAttribute->m_nBasePoint;
		SetNpcAttackRating();
		SetNpcDefence();
		UpdataCurData();
		SetNpcPhysicsDamage();
		break;
	case ATTRIBUTE_VITALITY:
		nData = pAttribute->m_nBasePoint - m_nVitality;
		m_nVitality = pAttribute->m_nBasePoint;
		Npc[m_nIndex].AddBaseLifeMax(PlayerSet.m_cLevelAdd.GetLifePerVitality(Npc[m_nIndex].m_Series) * nData);
		Npc[m_nIndex].AddBaseStaminaMax(PlayerSet.m_cLevelAdd.GetStaminaPerVitality(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		break;
	case ATTRIBUTE_ENGERGY:
		nData = pAttribute->m_nBasePoint - m_nEngergy;
		m_nEngergy = pAttribute->m_nBasePoint;
		Npc[m_nIndex].AddBaseManaMax(PlayerSet.m_cLevelAdd.GetManaPerEnergy(Npc[m_nIndex].m_Series) * nData);
		UpdataCurData();
		break;
	}
	CoreDataChanged(GDCNI_PLAYER_RT_ATTRIBUTE, 0, 0);
}
#endif

#ifndef _SERVER
void	KPlayer::s2cSetExp(int nExp)
{
	if (nExp > m_nExp)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_GET_EXP, nExp - m_nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	else if (nExp < m_nExp)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_DEC_EXP, m_nExp - nExp);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
	}
	
	this->m_nExp = nExp;
}
#endif

#ifndef _SERVER
void	KPlayer::s2cSyncMoney(BYTE* pMsg)
{
	PLAYER_MONEY_SYNC	*pMoney = (PLAYER_MONEY_SYNC*)pMsg;
	
	if ( CheckTrading() )
	{
		if (pMoney->m_nMoney1 >= pMoney->m_nMoney3)
		{
			m_ItemList.SetMoney(pMoney->m_nMoney1 - pMoney->m_nMoney3, pMoney->m_nMoney2, pMoney->m_nMoney3);
		}
		else
		{
			m_ItemList.SetMoney(0, pMoney->m_nMoney2 + pMoney->m_nMoney1 - pMoney->m_nMoney3, pMoney->m_nMoney3);
		}
		m_cTrade.m_nTradeState = 0;
		m_cTrade.m_nTradeDestState = 0;
		m_cTrade.m_nBackEquipMoney = pMoney->m_nMoney1;
		m_cTrade.m_nBackRepositoryMoney = pMoney->m_nMoney2;
	}
	else
	{
		// ��Ǯһ���ǵ�room_equipment
		int nMoney1 = m_ItemList.GetMoney(room_equipment);
		if (pMoney->m_nMoney1 - nMoney1 > 0)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_EARN_MONEY, pMoney->m_nMoney1 - nMoney1);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		m_ItemList.SetMoney(pMoney->m_nMoney1, pMoney->m_nMoney2, pMoney->m_nMoney3);
	}
}
void	KPlayer::s2cSyncXu(BYTE* pMsg)
{
		PLAYER_XU_SYNC	*pXu = (PLAYER_XU_SYNC*)pMsg;
		int nXu = m_ItemList.GetXu(room_equipment);
		if (pXu->m_nXu - nXu > 0)
		{
			KSystemMessage	sMsg;
			sMsg.eType = SMT_NORMAL;
			sMsg.byConfirmType = SMCT_NONE;
			sMsg.byPriority = 0;
			sMsg.byParamSize = 0;
			sprintf(sMsg.szMessage, MSG_EARN_XU, pXu->m_nXu - nXu);
			CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		}
		m_ItemList.SetXu(pXu->m_nXu);
}
#endif

#ifndef _SERVER
//---------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ�������뽻��
//---------------------------------------------------------------------
void	KPlayer::s2cTradeApplyStart(BYTE* pMsg)
{
	if (!pMsg)
		return;
	TRADE_APPLY_START_SYNC	*pApply = (TRADE_APPLY_START_SYNC*)pMsg;
	int		nNpcIdx;
	nNpcIdx = NpcSet.SearchID(pApply->m_dwNpcId);
	if (nNpcIdx == 0)
		return;
	
	KSystemMessage	sMsg;
	KUiPlayerItem	sPlayer;
	
	strcpy(sPlayer.Name, Npc[nNpcIdx].Name);
	sPlayer.nIndex = pApply->m_nDestIdx;
	sPlayer.uId = pApply->m_dwNpcId;
	sPlayer.nData = 0;
	
	sprintf(sMsg.szMessage, MSG_TRADE_GET_APPLY, Npc[nNpcIdx].Name);
	sMsg.eType = SMT_SYSTEM;
	sMsg.byConfirmType = SMCT_UI_TRADE;
	sMsg.byPriority = 3;
	sMsg.byParamSize = sizeof(KUiPlayerItem);
	CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, (int)&sPlayer);
}
#endif

#ifndef _SERVER
void	KPlayer::s2cTradeMoneySync(BYTE* pMsg)
{
	TRADE_MONEY_SYNC	*pMoney = (TRADE_MONEY_SYNC*)pMsg;
	
	m_ItemList.SetRoomMoney(room_trade1, pMoney->m_nMoney);
	
	// ֪ͨ����
	KUiObjAtRegion	sMoney;
	sMoney.Obj.uGenre = CGOG_MONEY;
	sMoney.Obj.uId = pMoney->m_nMoney;
	CoreDataChanged(GDCNI_TRADE_DESIRE_ITEM, (unsigned int)&sMoney, 0);
}
#endif

#ifndef _SERVER
//---------------------------------------------------------------------
//	���ܣ��յ�������֪ͨ������ɻ�ȡ��
//---------------------------------------------------------------------
void	KPlayer::s2cTradeDecision(BYTE* pMsg)
{
	TRADE_DECISION_SYNC	*pSync = (TRADE_DECISION_SYNC*)pMsg;
	if (pSync->m_btDecision == 1)	// ��ɽ���
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TRADE_SUCCESS, m_cTrade.m_szDestName);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		
		// ����ItemList.Remove����� modify by spe 03/06/14
		//m_ItemList.ClearRoom(room_trade);
		m_ItemList.RemoveAllInOneRoom(room_trade1);
		//m_ItemList.ClearRoom(room_trade1);
		m_ItemList.ClearRoom(room_tradeback);
		this->m_cTrade.Release();
		m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
		
		// ֪ͨ���潻�׽���
		CoreDataChanged(GDCNI_TRADE_END, 0, 0);
		
		m_ItemList.MenuSetMouseItem();
	}
	else if (pSync->m_btDecision == 0)	// ȡ������
	{
		if ( !CheckTrading() )
			return;
		
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, MSG_TRADE_FAIL, m_cTrade.m_szDestName);
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		
		// ȡ�����׵����ݻָ�
		m_ItemList.RecoverTrade();
		m_ItemList.SetMoney(m_cTrade.m_nBackEquipMoney, m_cTrade.m_nBackRepositoryMoney, 0);
		m_ItemList.ClearRoom(room_trade);
		m_ItemList.RemoveAllInOneRoom(room_trade1);
		//		m_ItemList.ClearRoom(room_trade1);
		m_ItemList.ClearRoom(room_tradeback);
		m_cTrade.Release();
		m_cMenuState.SetState(PLAYER_MENU_STATE_NORMAL);
		
		// ֪ͨ���潻�׽���
		CoreDataChanged(GDCNI_TRADE_END, 0, 0);
		// ������Ʒ���
		CoreDataChanged(GDCNI_CONTAINER_OBJECT_CHANGED, UOC_ITEM_TAKE_WITH, 0);
		
		m_ItemList.MenuSetMouseItem();
	}
}
#endif

#ifndef _SERVER
void	KPlayer::SyncCurPlayer(BYTE* pMsg)
{
	m_ItemList.RemoveAll();
	
	CURPLAYER_SYNC* PlaySync = (CURPLAYER_SYNC *)pMsg;
	this->m_nIndex = NpcSet.SearchID(PlaySync->m_dwID);
	this->m_dwID = g_FileName2Id(Npc[m_nIndex].Name); 
	
	Npc[m_nIndex].m_Kind = kind_player;
	Npc[m_nIndex].m_Level = (DWORD)PlaySync->m_btLevel;
	Npc[m_nIndex].m_nSex = PlaySync->m_btSex;
	Npc[m_nIndex].m_Series  = PlaySync->m_btSeries;
	Npc[m_nIndex].SetPlayerIdx(CLIENT_PLAYER_INDEX);

	
	m_nAttributePoint = PlaySync->m_wAttributePoint;
	m_nSkillPoint = PlaySync->m_wSkillPoint;
	m_nStrength = PlaySync->m_wStrength;
	m_nDexterity = PlaySync->m_wDexterity;
	m_nVitality = PlaySync->m_wVitality;
	m_nEngergy = PlaySync->m_wEngergy;
	m_nLucky = PlaySync->m_wLucky;
	
	m_nCurStrength = m_nStrength;
	m_nCurDexterity = m_nDexterity;
	m_nCurVitality = m_nVitality;
	m_nCurEngergy = m_nEngergy;
	m_nCurLucky = m_nLucky;
	SetFirstDamage();
	SetBaseAttackRating();
	SetBaseDefence();
	Npc[m_nIndex].m_ActionScriptID = 0;
	Npc[m_nIndex].m_TrapScriptID = 0;
	m_nExp = PlaySync->m_nExp;
	m_nNextLevelExp = PlayerSet.m_cLevelAdd.GetLevelExp(PlaySync->m_btLevel);
	m_dwLeadExp = PlaySync->m_dwLeadExp;
	m_CUnlocked = PlaySync->m_btCUnlocked;
	m_dwLeadLevel = PlayerSet.m_cLeadExp.GetLevel(m_dwLeadExp);
	m_dwNextLevelLeadExp = PlayerSet.m_cLeadExp.GetLevelExp(m_dwLeadLevel);
	
	m_cFaction.m_nCurFaction = (char)PlaySync->m_btCurFaction;
	m_cFaction.m_nFirstAddFaction = (char)PlaySync->m_btFirstFaction;
	m_cFaction.m_nAddTimes = PlaySync->m_nFactionAddTimes;
	
	m_nWorldStat = (int)PlaySync->m_wWorldStat;
	m_nSectStat = (int)PlaySync->m_wSectStat;

	m_ItemList.Init(CLIENT_PLAYER_INDEX);
	m_ItemList.SetMoney(PlaySync->m_nMoney1, PlaySync->m_nMoney2, 0);
	m_ItemList.SetXu(PlaySync->m_nXu);
	Npc[m_nIndex].m_LifeMax = PlaySync->m_wLifeMax;
	Npc[m_nIndex].m_ManaMax = PlaySync->m_wManaMax;
	Npc[m_nIndex].m_StaminaMax = PlaySync->m_wStaminaMax;
	Npc[m_nIndex].m_LifeReplenish = PLAYER_LIFE_REPLENISH;
	Npc[m_nIndex].m_ManaReplenish = PLAYER_MANA_REPLENISH;
	Npc[m_nIndex].m_StaminaGain = PLAYER_STAMINA_GAIN;
	Npc[m_nIndex].m_StaminaLoss = PLAYER_STAMINA_LOSS;
	
	SetBaseResistData();
	SetBaseSpeedAndRadius();
	
	Npc[m_nIndex].RestoreNpcBaseInfo();
	
	m_BuyInfo.Clear();
	m_cMenuState.Release();
	m_cChat.Release();
	memset(m_szTaskAnswerFun, 0, sizeof(m_szTaskAnswerFun));
	m_nAvailableAnswerNum = 0;
	
	Npc[m_nIndex].m_Experience = 0;
	memset(Npc[m_nIndex].m_szChatBuffer, 0, sizeof(Npc[m_nIndex].m_szChatBuffer));
	Npc[m_nIndex].m_nCurChatTime = 0;
	
	m_RunStatus = 1;
	m_nLeftSkillID = 0;
	m_nLeftSkillLevel = 0;
	m_nRightSkillID = 0;
	m_nRightSkillLevel = 0;
	m_nPeapleIdx = 0;
	m_nObjectIdx = 0;
	m_MouseDown[0] = 0;
	m_MouseDown[1] = 0;
	
	Npc[m_nIndex].m_SyncSignal = 0;
	//Npc[m_nIndex].RestoreLiveData();
	//	SubWorld[0].AddPlayer(Npc[m_nIndex].m_RegionIndex, 0);//m_Region[Npc[m_nIndex].m_RegionIndex].AddPlayer(0);// m_WorldMessage.Send(GWM_PLAYER_ADD, Npc[m_nIndex].m_RegionIndex, 0);
}
#endif

BOOL	KPlayer::CheckTrading()
{
	return (m_cMenuState.m_nState == PLAYER_MENU_STATE_TRADING);
}

void	KPlayer::SetFirstDamage()
{
	int nDamageBase;
	
	
	nDamageBase = m_nCurStrength / STRENGTH_SET_DAMAGE_VALUE + 1;
	
	Npc[m_nIndex].m_PhysicsDamage.nValue[0] = nDamageBase;
	Npc[m_nIndex].m_PhysicsDamage.nValue[2] = nDamageBase;
	Npc[m_nIndex].m_PhysicsDamage.nValue[1] = 0;
	
	Npc[m_nIndex].m_CurrentFireDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentFireDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentFireDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentColdDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentColdDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentColdDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentLightDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentLightDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentLightDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentPoisonDamage.nValue[2] = 0;

	Npc[m_nIndex].m_MagicPhysicsDamage.nValue[0] = 0;
	Npc[m_nIndex].m_MagicPhysicsDamage.nValue[2] = 0;
	Npc[m_nIndex].m_MagicPhysicsDamage.nValue[1] = 0;
	
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[2] = 0;
	
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0] = 0;
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[1] = 0;
	Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[2] = 0;
	
}

void	KPlayer::SetBaseAttackRating()
{
	Npc[m_nIndex].m_AttackRating = m_nDexterity * 4 - 28;
}

void	KPlayer::SetBaseDefence()
{
	Npc[m_nIndex].m_Defend = m_nDexterity >> 2;
}

void	KPlayer::SetBaseResistData()
{
	Npc[m_nIndex].m_FireResist			= PlayerSet.m_cLevelAdd.GetFireResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_ColdResist			= PlayerSet.m_cLevelAdd.GetColdResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_PoisonResist		= PlayerSet.m_cLevelAdd.GetPoisonResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_LightResist			= PlayerSet.m_cLevelAdd.GetLightResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_PhysicsResist		= PlayerSet.m_cLevelAdd.GetPhysicsResist(Npc[m_nIndex].m_Series, Npc[m_nIndex].m_Level);
	Npc[m_nIndex].m_FireResistMax		= BASE_FIRE_RESIST_MAX;
	Npc[m_nIndex].m_ColdResistMax		= BASE_COLD_RESIST_MAX;
	Npc[m_nIndex].m_PoisonResistMax		= BASE_POISON_RESIST_MAX;
	Npc[m_nIndex].m_LightResistMax		= BASE_LIGHT_RESIST_MAX;
	Npc[m_nIndex].m_PhysicsResistMax	= BASE_PHYSICS_RESIST_MAX;
}

void	KPlayer::SetBaseSpeedAndRadius()
{
	Npc[m_nIndex].m_WalkSpeed = BASE_WALK_SPEED;
	Npc[m_nIndex].m_RunSpeed = BASE_RUN_SPEED;
	Npc[m_nIndex].m_AttackSpeed = BASE_ATTACK_SPEED;
	Npc[m_nIndex].m_CastSpeed = BASE_CAST_SPEED;
	Npc[m_nIndex].m_VisionRadius = BASE_VISION_RADIUS;
	Npc[m_nIndex].m_HitRecover = BASE_HIT_RECOVER;
}

#ifndef _SERVER
//�ͻ��˰汾
void KPlayer::DialogNpc(int nIndex)
{
	if (nIndex > 0 && Npc[nIndex].m_Index > 0)
	{
		if (Npc[nIndex].ActionScript[0])
		{
			ExecuteScript(Npc[nIndex].m_ActionScriptID,"main","");
		}
		else
		{
			PLAYER_DIALOG_NPC_COMMAND DialogNpcCmd;
			DialogNpcCmd.nNpcId = Npc[nIndex].m_dwID;
			DialogNpcCmd.ProtocolType = c2s_dialognpc;		
			if (g_pClient)
				g_pClient->SendPackToServer(&DialogNpcCmd, sizeof(PLAYER_DIALOG_NPC_COMMAND));
			
		}
	}
}
#endif

#ifndef _SERVER
void KPlayer::CheckObject(int nIdx)
{
/*	enum	// �������
{
Obj_Kind_MapObj = 0,		// ��ͼ�������Ҫ���ڵ�ͼ����
Obj_Kind_Body,				// npc ��ʬ��
Obj_Kind_Box,				// ����
Obj_Kind_Item,				// ���ڵ��ϵ�װ��
Obj_Kind_Money,				// ���ڵ��ϵ�Ǯ
Obj_Kind_LoopSound,			// ѭ����Ч
Obj_Kind_RandSound,			// �����Ч
Obj_Kind_Light,				// ��Դ��3Dģʽ�з���Ķ�����
Obj_Kind_Door,				// ����
Obj_Kind_Trap,				// ����
Obj_Kind_Prop,				// С���ߣ�������
Obj_Kind_Num,				// �����������
};*/
	
	switch(Object[nIdx].m_nKind)
	{
	case Obj_Kind_Item:
	case Obj_Kind_Money:
		PickUpObj(nIdx);
		Npc[m_nIndex].m_nObjectIdx = 0;
		break;
	case Obj_Kind_Box:
	case Obj_Kind_Door:
	case Obj_Kind_Trap:
	case Obj_Kind_Prop:
		this->ObjMouseClick(nIdx);
		Npc[m_nIndex].m_nObjectIdx = 0;
		break;
	default:
		break;
	}
	m_nObjectIdx = 0;
	m_nPickObjectIdx = 0;
}
#endif

#ifndef _SERVER
void KPlayer::DrawSelectInfo()
{
	if (m_nIndex <= 0)
		return;
	
	if (m_nPeapleIdx)
	{
		//		Npc[m_nPeapleIdx].DrawBorder();
		if (Npc[m_nPeapleIdx].m_Kind == kind_player)
		{
			if (!NpcSet.CheckShowName())
			{
				Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
			}
		}
		else if (Npc[m_nPeapleIdx].m_Kind == kind_dialoger)
		{
			if (!NpcSet.CheckShowName())
			{
				Npc[m_nPeapleIdx].PaintInfo(Npc[m_nPeapleIdx].GetNpcPate(), true);
			}
		}
		else
		{
			Npc[m_nPeapleIdx].DrawBlood();
		}
		
		return;
	}
	if (m_nObjectIdx)
	{
		if (!ObjSet.CheckShowName())
			Object[m_nObjectIdx].DrawInfo();
		//		Object[m_nObjectIdx].DrawBorder();
		return;
	}
}
#endif

#ifndef _SERVER
void KPlayer::GetEchoDamage(int* nMin, int* nMax, int nType)
{
	int nSkillId;
	*nMin = 0;
	*nMax = 0;
	
	if (m_nIndex <= 0 || m_nIndex >= MAX_NPC 
		|| nType < 0 || nType > 1 
		|| NULL == nMin || NULL == nMax)
		return;
	
	nSkillId = (0 == nType)?m_nLeftSkillID:m_nRightSkillID;
	int nLevel = Npc[m_nIndex].m_SkillList.GetCurrentLevel(nSkillId);
	int nEnChance = Npc[m_nIndex].m_SkillList.GetEnChance(Npc[m_nIndex].m_SkillList.FindSame(nSkillId));
	
	if (nSkillId <= 0 ||nLevel <= 0 || nLevel >= MAX_SKILLLEVEL)
		return;
	
	KMagicAttrib*	pMagicData = NULL;
	
	KSkill * pOrdinSkill = (KSkill *) g_SkillManager.GetSkill(nSkillId, nLevel);
	if (!pOrdinSkill) 
        return ;

	switch(pOrdinSkill->GetSkillStyle())
	{
	case SKILL_SS_Missles:			//	�ӵ���		���������ڷ����ӵ���
	case SKILL_SS_Melee:
	case SKILL_SS_InitiativeNpcState:	//	������		���������ڸı䵱ǰNpc������״̬
	case SKILL_SS_PassivityNpcState:		//	������		���������ڸı�Npc�ı���״̬
		{
		}
		break;
	default:
		return;
	}
	
	BOOL	bIsPhysical = pOrdinSkill->IsPhysical();
	BOOL	bIsMagic = pOrdinSkill->IsMagic();

	
	pMagicData = pOrdinSkill->GetDamageAttribs();
	
	if (!pMagicData)
		return;
	
	
	
	int nMinNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[0] + Npc[m_nIndex].m_CurrentAddPhysicsDamage;
	int nMaxNpcDamage = Npc[m_nIndex].m_PhysicsDamage.nValue[2] + Npc[m_nIndex].m_CurrentAddPhysicsDamage;
	// Skip attackrating
	pMagicData++;
	// Skip ignoredefense
	pMagicData++;
	// Calc physics damage
	if (magic_physicsenhance_p == pMagicData->nAttribType && !bIsMagic)
	{
		*nMin += nMinNpcDamage * (100 + pMagicData->nValue[0]) / 100;
		*nMax += nMaxNpcDamage * (100 + pMagicData->nValue[0]) / 100;
		
		int nEnhance;
		if (equip_meleeweapon == m_ItemList.GetWeaponType())
		{
			nEnhance = Npc[m_nIndex].m_CurrentMeleeEnhance[m_ItemList.GetWeaponParticular()];
		}
		else if (equip_rangeweapon == m_ItemList.GetWeaponType())
		{
			nEnhance = Npc[m_nIndex].m_CurrentRangeEnhance;
		}
		else 
		{
			nEnhance = Npc[m_nIndex].m_CurrentHandEnhance;
		}
		*nMin += nMinNpcDamage * nEnhance / 100;
		*nMax += nMaxNpcDamage * nEnhance / 100;
	}
	else if (magic_physicsdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];

		if (bIsMagic)
		{
			*nMin += Npc[m_nIndex].m_MagicPhysicsDamage.nValue[0];
			*nMax += Npc[m_nIndex].m_MagicPhysicsDamage.nValue[2];
		}
	}
	pMagicData++;
	// Calc cold damage
	if (magic_colddamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];

		if (bIsMagic)
		{
			*nMin += Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[0];
			*nMax += Npc[m_nIndex].m_CurrentMagicColdDamage.nValue[2];
		}
	}
	if (bIsPhysical)
	{
		*nMin += Npc[m_nIndex].m_CurrentColdDamage.nValue[0];
		*nMax += Npc[m_nIndex].m_CurrentColdDamage.nValue[2];
	}
	pMagicData++;
	// Calc fire damage
	if (magic_firedamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];

		if (bIsMagic)
		{
			*nMin += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[0];
			*nMax += Npc[m_nIndex].m_CurrentMagicFireDamage.nValue[2];
		}
	}
	if (bIsPhysical)
	{
		*nMin += Npc[m_nIndex].m_CurrentFireDamage.nValue[0];
		*nMax += Npc[m_nIndex].m_CurrentFireDamage.nValue[2];
	}
	pMagicData++;
	// Calc lighting damage
	if (magic_lightingdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];

		if (bIsMagic)
		{
			*nMin += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[0];
			*nMax += Npc[m_nIndex].m_CurrentMagicLightDamage.nValue[2];
		}
	}
	if (bIsPhysical)
	{
		*nMin += Npc[m_nIndex].m_CurrentLightDamage.nValue[0];
		*nMax += Npc[m_nIndex].m_CurrentLightDamage.nValue[2];
	}
	pMagicData++;
	// Calc poison damage
	if (magic_poisondamage_v == pMagicData->nAttribType)
	{
			*nMin += pMagicData->nValue[0];
			*nMax += pMagicData->nValue[0];

			if (bIsMagic)
			{
				*nMin += Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0];
				*nMax += Npc[m_nIndex].m_CurrentMagicPoisonDamage.nValue[0];
			}
	}
	if (bIsPhysical)
	{
			int nPoisonDamage = Npc[m_nIndex].m_CurrentPoisonDamage.nValue[0];
			*nMin += nPoisonDamage;
			*nMax += nPoisonDamage;
	}
	pMagicData++;
	// Calc magic damage
	if (magic_magicdamage_v == pMagicData->nAttribType)
	{
		*nMin += pMagicData->nValue[0];
		*nMax += pMagicData->nValue[2];
	}
	*nMin += *nMin * nEnChance / 100;
	*nMax += *nMax * nEnChance / 100;
}
#endif
void KPlayer::SetNpcDamageAttrib()
{
	SetNpcPhysicsDamage();
}

#ifdef _SERVER
void KPlayer::TobeExchangeServer(DWORD dwMapID, int nX, int nY)
{
	if (!m_dwID || m_nNetConnectIdx == -1)
		return;
	
	m_sExchangePos.m_dwMapID = dwMapID;
	m_sExchangePos.m_nX = nX;
	m_sExchangePos.m_nY = nY;
	m_bExchangeServer = TRUE;
}
#endif

/*
#ifdef _SERVER
void KPlayer::UpdateEnterGamePos(DWORD dwSubWorldID, int nX, int nY, int nFightMode)
{
	TRoleData* pData = (TRoleData *)&m_SaveBuffer;
	pData->BaseInfo.cFightMode = nFightMode;
	pData->BaseInfo.cUseRevive = 0;
	pData->BaseInfo.ientergameid = dwSubWorldID;
	pData->BaseInfo.ientergamex = nX;
	pData->BaseInfo.ientergamey = nY;
}
#endif
*/

#ifdef _SERVER
BOOL KPlayer::IsExchangingServer()
{
	return m_bExchangeServer;
}
#endif

#ifdef _SERVER
void KPlayer::Market(int nId) 
{ 
	BuySell.OpenMarket(nId, 19);
}
#endif


#ifdef _SERVER
void KPlayer::String(int nbutton)
	{//stringbox
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\system\\input.lua");
		ExecuteScript(scriptName,"main",nbutton); 
	}
#endif

#ifdef _SERVER
void KPlayer::script(int nId) 
{ 
	if (nId == 1)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\quest_exp.lua");  //datau
		ExecuteScript(scriptName,"main",0); 
	}
	else if (nId == 2)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\quest_money.lua");
		ExecuteScript(scriptName,"main",0); 	
	}
	
	else if (nId == 3)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\quest_random.lua");
		ExecuteScript(scriptName,"main",0); 	
	}
	
	else if (nId == 4)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\quest_lucky.lua");
		ExecuteScript(scriptName,"main",0); 	
	}
	
	else if (nId == 5)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\quest_item.lua");
		ExecuteScript(scriptName,"main",0); 	
	}
	
	else if (nId == 6)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\quest_point.lua");
		ExecuteScript(scriptName,"main",0); 	
	}
	
}
#endif


#ifdef _SERVER
void KPlayer::pkvalue(int nId) 
{ 
	if (nId == 1)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\trangthai.lua");  //datau
		ExecuteScript(scriptName,"main",1); 
	}
	else if (nId == 2)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\trangthai.lua");
		ExecuteScript(scriptName,"main",2); 	
	}
	
	else if (nId == 3)
	{
		char scriptName[255]; 
		sprintf(scriptName,"\\script\\special\\trangthai.lua");
		ExecuteScript(scriptName,"main",3); 	
	}	
	
}
#endif

#ifdef _SERVER
void	KPlayer::SetChestLock(BOOL unlockFlag)
{
	if (unlockFlag)
	{
		// Ruong duoc mo
		m_CUnlocked = 1;
		char szMsg[100];
		sprintf(szMsg, "<color=cyan> B�n c� th� s� d�ng c�c ch�c n�ng !");
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "B�o m�t", (char *) szMsg, strlen(szMsg) );
	}
	else
	{
		// Ruong duoc khoa
		m_CUnlocked = 0;
		char szMsg[100];
		sprintf(szMsg, "<color=cyan> T� kh�a nh�n v�t ho�n t�t !");
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "B�o m�t", (char *) szMsg, strlen(szMsg) );
	}
}
#endif

#ifdef _SERVER
BOOL	KPlayer::CheckChestPW(int iPassword)
{
	if (iPassword == m_nChestPW)
	{
		// Mat khau ruong chinh xac
		char szMsg[100];
		sprintf(szMsg, "<color=cyan> Nh�n v�t �� m� kh�a !");
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "B�o m�t", (char *) szMsg, strlen(szMsg) );
		return TRUE;
	}
	else
	{
		// Mat khau ruong SAI
		char szMsg[100];
		sprintf(szMsg, "<color=cyan> M�t m� sai, xin nh�p l�i !");
		KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "B�o m�t", (char *) szMsg, strlen(szMsg) );
		return FALSE;
	}
}
#endif

#ifdef _SERVER
void	KPlayer::SetChestPW(int newPW)
{
	m_nChestPW = newPW;
	
	// Thong bao cho Client
	char szMsg[100];
	sprintf(szMsg, "<color=cyan> M�t m� �� ���c thay ��i !");
	KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "B�o m�t", (char *) szMsg, strlen(szMsg) );
	
}
#endif

#ifdef _SERVER
void	KPlayer::SetNewPW(int resetPW)
{
	m_nChestPW = resetPW;
	
	// Thong bao cho Client
	char szMsg[100];
	sprintf(szMsg, "<color=cyan> M�t m� thay ��i th�nh c�ng !");
	KPlayerChat::SendSystemInfo(1, m_nPlayerIndex, "B�o m�t", (char *) szMsg, strlen(szMsg) );
	
}
#endif

#ifdef _SERVER
void KPlayer::SetLastNetOperationTime(int nTime)
{
	if (m_nNetConnectIdx == -1 || m_dwID == 0 || m_nIndex <= 0)
		return;
	
	m_nLastNetOperationTime = nTime;
	if (m_bSleepMode)
	{
		m_bSleepMode = FALSE;
		NPC_SLEEP_SYNC	SleepSync;
		SleepSync.ProtocolType = s2c_npcsleepmode;
		SleepSync.bSleep = m_bSleepMode;
		SleepSync.NpcID = Npc[m_nIndex].m_dwID;
		Npc[m_nIndex].SendDataToNearRegion(&SleepSync, sizeof(NPC_SLEEP_SYNC));
	}
}
#endif

#ifdef _SERVER
BOOL	KPlayer::CreateTong(int nCamp, char *lpszTongName)
{
	return this->m_cTong.Create(nCamp, lpszTongName);
}
#endif

#ifdef _SERVER
void KPlayer::RepairItem(DWORD dwItemID)
{
	int nIdx = m_ItemList.SearchID(dwItemID);
	int nCost = Item[nIdx].GetRepairPrice();
	if (!nCost)
		return;
	int nMaxDur = Item[nIdx].GetMaxDurability();
	int nDur = Item[nIdx].GetDurability();
	if (Pay(nCost) && m_nNetConnectIdx >= 0)
	{
		Item[nIdx].SetDurability(nMaxDur);
		ITEM_DURABILITY_CHANGE	IDC;
		IDC.ProtocolType = s2c_itemdurabilitychange;
		IDC.dwItemID = dwItemID;
		IDC.nChange = nMaxDur - nDur;
		if (g_pServer)
			g_pServer->PackDataToClient(m_nNetConnectIdx, &IDC, sizeof(ITEM_DURABILITY_CHANGE));
	}
}
#endif

#ifdef _SERVER
void KPlayer::BreakItem(DWORD dwItemID, int nNum)
{
	int nIdx = m_ItemList.SearchID(dwItemID);
	if (Item[nIdx].GetStackNum() > nNum)
	{
		Item[nIdx].SetStackNum(Item[nIdx].GetStackNum() - nNum);
		int nIndex = ItemSet.Add(Item[nIdx].GetGenre(),
								Item[nIdx].GetSeries(), 
								Item[nIdx].GetLevel(), 
								10, 
								Item[nIdx].GetDetailType(), 
								Item[nIdx].GetParticular(),
								NULL, g_SubWorldSet.GetGameVersion(),
								0,
								nNum);
		if (nIndex <= 0)
		{
			return ;
		}
		
		int		nIdx = m_ItemList.Hand();
		if (nIdx)
		{
			m_ItemList.Remove(nIdx);
				
			KMapPos sMapPos;
			KObjItemInfo	sInfo;
				
			GetAboutPos(&sMapPos);
				
			sInfo.m_nItemID = nIdx;
			sInfo.m_nItemWidth = Item[nIdx].GetWidth();
			sInfo.m_nItemHeight = Item[nIdx].GetHeight();
			sInfo.m_nMoneyNum = 0;
			strcpy(sInfo.m_szName, Item[nIdx].GetName());
			sInfo.m_nColorID = 0;
			sInfo.m_nMovieFlag = 1;
			sInfo.m_nSoundFlag = 1;
				
			int nObj = ObjSet.Add(Item[nIdx].GetObjIdx(), sMapPos, sInfo);
			if (nObj >= 0)
			{
				if (Item[nIdx].GetGenre() == item_task)
				{
					Object[nObj].SetEntireBelong(m_nPlayerIndex);
				}
				else
				{
					Object[nObj].SetItemBelong(m_nPlayerIndex);
				}
			}
		}
		m_ItemList.Add(nIndex, pos_hand, 0 ,0);
	}
}
#endif

#ifdef _SERVER
void KPlayer::SetExtPoint(int nPoint, int nChangePoint)
{
	m_nExtPoint = nPoint;
	m_nChangeExtPoint = nChangePoint;
}

int KPlayer::GetExtPoint()
{
	return m_nExtPoint;
}

BOOL KPlayer::PayExtPoint(int nPoint)
{
	if (m_nExtPoint < nPoint)
		return FALSE;

	m_nExtPoint -= nPoint;
	m_nChangeExtPoint += nPoint;
	return TRUE;
}

int KPlayer::GetExtPointChanged()
{
	return m_nChangeExtPoint;
}

int KPlayer::ComItem( int nIdx1,int nIdx2,int nIdx3 ,int nKind)
{
	int nIndex;

	if (nKind == 1)
	{
		int nLevel = Item[nIdx1].GetLevel();
		nLevel += Item[nIdx2].GetLevel();
		nLevel += Item[nIdx3].GetLevel();
		
		int nMax = nLevel / 5;
		int nMin = nLevel / 10;
		if (nMin < 0) nMin = 1;
		
		nIndex = ItemSet.Add(2,1,::GetRandomNumber(nMin,nMax),10,1,147);
	}
	else if (nKind == 2)
	{
		int nLevel = Item[nIdx1].GetLevel();
		int nNum = Item[nIdx1].GetStackNum() + Item[nIdx2].GetStackNum() + Item[nIdx3].GetStackNum();
		
		nNum = nNum / 3;
		
		for (int i = 0;i < nNum;i++)
		{
			if (GetRandomNumber(0,10) <= 1)
			{
				nNum--;
			}
		}
		
		if (nNum == 0)
		{		
			m_ItemList.Remove(nIdx1);
			return 0;//�ϳ�ʧ��
		}
		
		nIndex = ItemSet.Add(item_mine,3,nLevel + 1,10,1,147,NULL,0,0,nNum,0);
	}
	else if (nKind == 3)
	{
		int nParticular = Item[nIdx1].GetParticular();
		int nSerise = Item[nIdx1].GetSeries();
		int nLevel = Item[nIdx1].GetLevel();
		int nMagic = Item[nIdx1].GetBaseMagic();
		
		if (::GetRandomNumber(0,10) <= 1)
		{
			m_ItemList.Remove(nIdx1);
			return 0;//�ϳ�ʧ��
		}
		
		nIndex = ItemSet.Add(2,nSerise,nLevel + 1,nMagic,1,nParticular);

	}
	

	m_ItemList.Remove(nIdx1);
	m_ItemList.Remove(nIdx2);
	m_ItemList.Remove(nIdx3);
	
	if (nIndex < 0)
	{						
		return 0;
	}

	int		x, y;
	if (m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		m_ItemList.Add(nIndex, pos_equiproom, x, y);
	}
	return 1;
}

int KPlayer::ComDiyItem( int nId1,int nId2,int nId3,int nKind,int nMin )
{
	int nIndex;
	if (nKind == 1)
	{
		int nPlace = Item[nId3].GetParticular() - 149;
		
		nMin += Item[nId2].GetLevel() + Item[nId1].GetItemParam()->nGeneratorLevel[nPlace]
			- 15;
		if (nMin < 1) nMin = 1;
		
		int nMax = nMin + Item[nId2].GetLevel();
		
		int nAdd = ::GetRandomNumber(nMin,nMax);

		if (nAdd > 10) nAdd = 10;

		if (3 > ::GetRandomNumber(nMin,nMax))
		{
			m_ItemList.Remove(nId2);
			m_ItemList.Remove(nId3);
			return 0;			
		}
		
		int nSerise = Item[nId1].GetSeries();
		
		int nMagic = Item[nId1].GetAttribType(nPlace);

		if (nMagic < 0)
		{
			return 0;
		}
		
		nIndex = ItemSet.Add(2,nSerise,nAdd,nMagic,1,Item[nId3].GetParticular() + 51);
	}
	else if (nKind == 2)
	{
		int nMax = Item[nId2].GetLevel() + 2;
		int nMin = Item[nId2].GetLevel();
		
		if (nMax > 6) nMax = 6;
		if (nMin > 6) nMax = 6;
		
		if (nMax < 1) nMax = 1;
		if (nMin < 1) nMax = 1;
		
		int nPoint = ::GetRandomNumber(nMin,nMax);
		
		int nItemClass		= Item[nId1].GetGenre();
		int nDetailType		= Item[nId1].GetDetailType();
		int nParticularType	= Item[nId1].GetParticular();
		int nLevel			= Item[nId1].GetLevel();
		int nSeries			= Item[nId1].GetSeries();
		int nLuck			= Item[nId1].GetItemParam()->nLuck;
		int nItemLevel[6];
		nItemLevel[0] = 0;
		nItemLevel[1] = 0;
		nItemLevel[2] = 0;
		nItemLevel[3] = 0;
		nItemLevel[4] = 0;
		nItemLevel[5] = 0;
		
		nIndex = ItemSet.Add(nItemClass,nSeries,nLevel,nLuck,nDetailType,nParticularType,nItemLevel,0,0,1,0,nPoint);

	}
	else if (nKind == 3)
	{
		int nPlace = Item[nId3].GetParticular() - 200;

		nMin = nMin / 2;
		nMin += Item[nId2].GetLevel() + Item[nId3].GetLevel() - 9;
		
		int nMax = nMin + Item[nId2].GetLevel();

		if (nMin <= 0)
		{
			nMin = 1;
		}
		
		if (::GetRandomNumber(nMin,nMax) < 3) 
		{
			m_ItemList.Remove(nId2);
			m_ItemList.Remove(nId3);
			return 0;					
		}
		
		int nSerise = Item[nId1].GetSeries();
		int nPoint = Item[nId1].IsPurple();
		
		int nItemClass		= Item[nId1].GetGenre();
		int nDetailType		= Item[nId1].GetDetailType();
		int nParticularType	= Item[nId1].GetParticular();
		int nLevel			= Item[nId1].GetLevel();
		int nSeries			= Item[nId1].GetSeries();
		int nLuck			= Item[nId1].GetItemParam()->nLuck;
		int nItemLevel[6];
		
		KTabFile tabMagic;
		tabMagic.Load("\\Settings\\Item\\magicattriblevel_index.txt");
		
		int nRow;
		char szTmp[8];
		sprintf(szTmp,"%d",Item[nId3].GetBaseMagic());
		nRow = tabMagic.FindRow(szTmp);
/*		for (int l = 0;l < tabMagic.GetHeight();l++)
		{
			tabMagic.GetInteger(l,1,0,&szTmp);
			if (szTmp == Item[nId3].GetBaseMagic())
			{
				nRow = l;
				break;
			}
		}*/

		int nAdd = ::GetRandomNumber(nMin,nMax) + ((Item[nId3].GetLevel() + Item[nId2].GetLevel())/2);
		if (nLuck < 0) nLuck = 0;
		
		nLuck += nMin ;
		if (nLuck > 10) nLuck = 10;
		if (nAdd > 10) nAdd = 10;
		int nMinRow; 
		tabMagic.GetInteger(nRow,"MAGATTRLVL_START",0,&nMinRow);
		nMinRow +=  nAdd;
		tabMagic.Clear();
		tabMagic.Load("\\Settings\\Item\\magicattrib.txt");
		int nMagic;
		tabMagic.GetInteger(nMinRow,Item[nId1].GetDetailType() + 13,0,&nMagic);
		if (nMagic <= 0)
		{
			return 0;
		}
		tabMagic.Clear();
		for (int k = 0;k < 6;k++)
		{
			if (nPlace > k)
			{
				if (Item[nId1].GetAttribType(k) <= 0 || Item[nId1].GetAttribType(k) == Item[nId3].GetBaseMagic())
				{
					return 0;
				}
			}
			else if (nPlace == k &&  Item[nId1].GetAttribType(k) <= 0)
			{
				nItemLevel[k] = nMinRow;
				continue;
			}
			else if (nPlace == k && Item[nId1].GetAttribType(k) > 0)
			{
				return 0;
			}
			nItemLevel[k] = Item[nId1].GetItemParam()->nGeneratorLevel[k];
		}
		
		int nStackNum = 1;
		int nEnChance = Item[nId1].GetEnChance();
		
		
		nIndex = ItemSet.Add(nItemClass,nSeries,nLevel,nLuck,nDetailType,nParticularType,nItemLevel,0,0,1,0,nPoint);
	}

	m_ItemList.Remove(nId1);
	m_ItemList.Remove(nId2);
	m_ItemList.Remove(nId3);
	
	if (nIndex < 0)
	{						
		return 0;
	}
	
	int		x, y;
	if (m_ItemList.CheckCanPlaceInEquipment(Item[nIndex].GetWidth(), Item[nIndex].GetHeight(), &x, &y))
	{
		m_ItemList.Add(nIndex, pos_equiproom, x, y);
	}
	return 1;
}


void KPlayer::SendSellItemInfo( int nTargetPlayer, int nPrcess, BOOL bUpdate)
{
	if (nTargetPlayer <= 0 || nTargetPlayer >= MAX_PLAYER || Player[nTargetPlayer].m_nIndex <= 0)
		return;

	int				nIdx, i, j, k = 0;
	SViewSellItemInfo	*pInfo;
	VIEW_ITEM_SYNC	sView;
	ZeroMemory(sView.m_sInfo,sizeof(sView.m_sInfo));
	
	if (!bUpdate)
		sView.ProtocolType = s2c_viewsellitem;
	else
		sView.ProtocolType =s2c_viewupdateitem;
	sView.m_dwNpcID = Npc[m_nIndex].m_dwID;

	for (i = 0; i < MAX_PLAYER_ITEM; i++)
	{
		nIdx = m_ItemList.m_Items[i].nIdx;
		if (nIdx <= 0 || m_ItemList.m_Items[i].nPrice == 0 || m_ItemList.m_Items[i].nPlace != pos_equiproom)
		{
//			memset(pInfo, 0, sizeof(SViewItemInfo));
			continue;
		}
		if (nPrcess)
		{
			nPrcess--;
			continue;
		}

		pInfo = &sView.m_sInfo[k];
		pInfo->m_uPrice = m_ItemList.m_Items[i].nPrice;
		pInfo->m_bX = m_ItemList.m_Items[i].nX;
		pInfo->m_bY = m_ItemList.m_Items[i].nY;
		pInfo->m_nIdx = m_ItemList.m_Items[i].nIdx;

		pInfo->m_nID			= Item[nIdx].GetID();
		pInfo->m_btGenre		= Item[nIdx].GetGenre();
		pInfo->m_btDetail		= Item[nIdx].GetDetailType();
		pInfo->m_btParticur		= Item[nIdx].GetParticular();
		pInfo->m_btSeries		= Item[nIdx].GetSeries();
		pInfo->m_btLevel		= Item[nIdx].GetLevel();
		pInfo->m_bStack			= Item[nIdx].GetStackNum();
		pInfo->m_bEnChance		= Item[nIdx].GetEnChance();
		pInfo->m_nGoldId		= Item[nIdx].GetGoldId();
		pInfo->m_btLuck			= Item[nIdx].m_GeneratorParam.nLuck;
		pInfo->m_dwRandomSeed	= Item[nIdx].m_GeneratorParam.uRandomSeed;
		pInfo->m_wVersion		= Item[nIdx].m_GeneratorParam.nVersion;

		for (int l = 0;l < 6;l++)
		{
			pInfo->m_btMagicLevel[l] = Item[nIdx].m_GeneratorParam.nGeneratorLevel[l];
		}

		pInfo->m_bPoint = Item[nIdx].IsPurple();
		k++;

		if (k == 6)
		{
			break;
		}
	}

	g_pServer->PackDataToClient(Player[nTargetPlayer].m_nNetConnectIdx, (BYTE*)&sView, sizeof(sView));
}

void KPlayer::SendSellItemCount( int nIndex )
{
	if (nIndex <= 0 || nIndex >= MAX_PLAYER || Player[nIndex].m_nIndex <= 0)
		return;

	PLAYER_GET_COUNT pGetCount;
	pGetCount.ProtocolType = s2c_playergetcount;
	pGetCount.dwId = Npc[m_nIndex].m_dwID;

	int nCount = 0;
	for (int i = 0;i < MAX_PLAYER_ITEM;i++)
	{
		int nIdx = m_ItemList.m_Items[i].nIdx;
		if (nIdx <= 0 || m_ItemList.m_Items[i].nPrice == 0 || m_ItemList.m_Items[i].nPlace != pos_equiproom)
		{
			//			memset(pInfo, 0, sizeof(SViewItemInfo));
			continue;
		}
		nCount++;
	}

	pGetCount.nCount = nCount;

	if (nCount == 0)
	{
		Npc[m_nIndex].m_BaiTan = 0;
	}

	g_pServer->PackDataToClient(Player[nIndex].m_nNetConnectIdx, (BYTE*)&pGetCount, sizeof(pGetCount));
}

int KPlayer::GetTradeCount()
{
	int nCount = 0;
	for (int i = 0;i < MAX_PLAYER_ITEM;i++)
	{
		int nIdx = m_ItemList.m_Items[i].nIdx;
		if (nIdx <= 0 || m_ItemList.m_Items[i].nPrice == 0 || m_ItemList.m_Items[i].nPlace != pos_equiproom)
		{
			//			memset(pInfo, 0, sizeof(SViewItemInfo));
			continue;
		}
		nCount++;
	}

	return nCount;
}
#endif
// ��Ʒ��Ӧ�Ĳ�����
//     �����Ʒ��1���ӵ��ϼ���Ʒ 2���ű�����ֱ�Ӹ� 3������ 4��player ֮����
//               ��(ͨ������ʵ��)
//         �ͻ�������������ϣ�Ȼ��ͻ���ͨ������ó���Ʒid��Ӧ�ó�����װ��������
//         ��Ʒ����λ�û��߸������ļ��㣬�Ѽ������������������������յ���������
//         ���������Ʒ��λ�ù�ϵ��Ȼ����ͻ��˵ļ������Ƿ���ȷ��Ȼ�������Ӧ��
//         �������Ѵ�����������ͻ���
//     ��Ʒ��λ�õ������ͻ��˴�������Ʒ��Դλ�á��������ƷĿ��λ�ã�Ȼ�󷢸���������
//     ������ȷ�Ϻ�֪ͨ�ͻ�������ȷ���Ĳ���������ɹ�����Э��ԭ������ȥ�����������
//     ֵ�������仯������֪ͨ�ͻ��ˣ�
//     ʹ����Ʒ��1����ҩ 2��װ��������Ҽ������
//         �ͻ�����������������ʲôλ�õ�ҩ��ͬʱ�ͻ��˵���Ӧ��ֵ������Ӧ�仯����
//         �����յ��ͻ�������󣬴�������Ӧ���ݣ�ͨ��playerͬ���ķ�ʽ֪ͨ�ͻ�������
//         �ı仯��
//         װ��������Ҽ�����һ��װ���ϣ��Զ�����װ���ϡ�ж�µ�λ����Ϣ��������������
//         �������������ԭ����������

// ���׵Ĵ�����û��

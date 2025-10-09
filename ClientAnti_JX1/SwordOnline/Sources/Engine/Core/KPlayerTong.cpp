//---------------------------------------------------------------------------
// Sword3 Engine (c) 2003 by Kingsoft
//
// File:	KPlayerTong.cpp
// Date:	2003.08.12
// Code:	±ﬂ≥«¿À◊”
// Desc:	KPlayerTong Class
//---------------------------------------------------------------------------

#include	"KCore.h"
#include	"KNpc.h"
#include	"KPlayer.h"
#include	"KPlayerSet.h"
#include	"KPlayerTong.h"
#ifndef _SERVER
#include	"CoreShell.h"
#endif

//#define		defTONG_NAME_LENGTH			8
#define		defFuncShowNormalMsg(str)		\
	{										\
		KSystemMessage	sMsg;				\
		sMsg.eType = SMT_NORMAL;			\
		sMsg.byConfirmType = SMCT_NONE;		\
		sMsg.byPriority = 0;				\
		sMsg.byParamSize = 0;				\
		sprintf(sMsg.szMessage, str);		\
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);\
	}

//-------------------------------------------------------------------------
//	π¶ƒ‹£∫≥ı ºªØ
//-------------------------------------------------------------------------
void	KPlayerTong::Init(int nPlayerIdx)
{
	m_nPlayerIndex = nPlayerIdx;

	Clear();
}

//-------------------------------------------------------------------------
//	π¶ƒ‹£∫«Âø’
//-------------------------------------------------------------------------
void	KPlayerTong::Clear()
{
	m_nFlag				= 0;
	m_nFigure			= enumTONG_FIGURE_MEMBER;
	m_nCamp				= 0;
	m_dwTongNameID		= 0;
	m_szName[0]			= 0;
	m_szTitle[0]		= 0;
	m_szMasterName[0]	= 0;
	m_nApplyTo			= 0;
	m_nMoney			= 0;
}

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫…Í«Î¥¥Ω®∞Ôª·
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyCreateTong(int nCamp, char *lpszTongName)
{
	defFuncShowNormalMsg(MSG_TONG_APPLY_CREATE);
	// ∞Ôª·√˚Œ Ã‚
	if (!lpszTongName || !lpszTongName[0] || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH)
	{
		defFuncShowNormalMsg(MSG_TONG_CREATE_ERROR01);
		return FALSE;
	}
	// ∞Ôª·’Û”™Œ Ã‚
	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
	{
		defFuncShowNormalMsg(MSG_TONG_CREATE_ERROR02);
		return FALSE;
	}
	// “—æ≠ «∞Ôª·≥…‘±
	if (m_nFlag)
	{
		defFuncShowNormalMsg(MSG_TONG_CREATE_ERROR03);
		return FALSE;
	}
	// ◊‘º∫µƒ’Û”™Œ Ã‚
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp != camp_free)
	{
		defFuncShowNormalMsg(MSG_TONG_CREATE_ERROR04);
		return FALSE;
	}
	// µ»º∂Œ Ã‚
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Level < PlayerSet.m_sTongParam.m_nLevel)
	{
		defFuncShowNormalMsg(MSG_TONG_CREATE_ERROR05);
		return FALSE;
	}
	if ((int)Player[CLIENT_PLAYER_INDEX].m_dwLeadLevel < PlayerSet.m_sTongParam.m_nLeadLevel)
	{
		defFuncShowNormalMsg(MSG_TONG_CREATE_ERROR06);
		return FALSE;
	}
	// Ωª“◊π˝≥Ã÷–
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
	{
		return FALSE;
	}
	// «ÆŒ Ã‚
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoneyAmount() < PlayerSet.m_sTongParam.m_nMoney)
	{
		char	szBuf[80];
		sprintf(szBuf, MSG_TONG_CREATE_ERROR07, PlayerSet.m_sTongParam.m_nMoney);
		defFuncShowNormalMsg(szBuf);
		return FALSE;
	}
	// ◊È∂”≤ªƒ‹Ω®∞Ôª·
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		defFuncShowNormalMsg(MSG_TONG_CREATE_ERROR08);
		return FALSE;
	}

	TONG_APPLY_CREATE_COMMAND	sApply;
	sApply.ProtocolType = c2s_extendtong;
	sApply.m_wLength = sizeof(TONG_APPLY_CREATE_COMMAND) - 1;
	sApply.m_btMsgId = enumTONG_COMMAND_ID_APPLY_CREATE;
	sApply.m_btCamp = (BYTE)nCamp;
	strcpy(sApply.m_szName, lpszTongName);

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sApply.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫…Í«Îº”»Î∞Ôª·
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyAddTong(DWORD dwNpcID)
{
	defFuncShowNormalMsg(MSG_TONG_APPLY_ADD);
	// “—æ≠ «∞Ôª·≥…‘±
	if (m_nFlag)
	{
		defFuncShowNormalMsg(MSG_TONG_APPLY_ADD_ERROR1);
		return FALSE;
	}
	// ◊‘º∫µƒ’Û”™Œ Ã‚
	if (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp != camp_free)
	{
		defFuncShowNormalMsg(MSG_TONG_APPLY_ADD_ERROR2);
		return FALSE;
	}
	// Ωª“◊π˝≥Ã÷–
	if (Player[CLIENT_PLAYER_INDEX].CheckTrading())
	{
		return FALSE;
	}
	// ◊È∂”≤ªƒ‹Ω®∞Ôª·
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
	{
		defFuncShowNormalMsg(MSG_TONG_APPLY_ADD_ERROR3);
		return FALSE;
	}

	TONG_APPLY_ADD_COMMAND	sApply;
	sApply.ProtocolType = c2s_extendtong;
	sApply.m_wLength = sizeof(TONG_APPLY_ADD_COMMAND) - 1;
	sApply.m_btMsgId = enumTONG_COMMAND_ID_APPLY_ADD;
	sApply.m_dwNpcID = dwNpcID;

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sizeof(TONG_APPLY_ADD_COMMAND));

	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫≈–∂œ¥¥Ω®∞Ôª·Ãıº˛ «∑Ò≥…¡¢
//-------------------------------------------------------------------------
int		KPlayerTong::CheckCreateCondition(int nCamp, char *lpszTongName)
{
	if (Player[m_nPlayerIndex].m_nIndex <= 0)
		return enumTONG_CREATE_ERROR_ID1;
	// Ωª“◊π˝≥Ã÷–
	if (Player[m_nPlayerIndex].CheckTrading())
		return enumTONG_CREATE_ERROR_ID2;
	// ∞Ôª·√˚Œ Ã‚
	if (!lpszTongName || !lpszTongName[0] || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH)
		return enumTONG_CREATE_ERROR_ID3;
	// ∞Ôª·’Û”™Œ Ã‚
	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
		return enumTONG_CREATE_ERROR_ID4;
	// “—æ≠ «∞Ôª·≥…‘±
	if (m_nFlag)
		return enumTONG_CREATE_ERROR_ID5;
	// ◊‘º∫µƒ’Û”™Œ Ã‚
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp != camp_free ||
		Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp != camp_free)
		return enumTONG_CREATE_ERROR_ID6;
	// µ»º∂Œ Ã‚
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_Level < PlayerSet.m_sTongParam.m_nLevel || 
		(int)Player[m_nPlayerIndex].m_dwLeadLevel < PlayerSet.m_sTongParam.m_nLeadLevel)
		return enumTONG_CREATE_ERROR_ID7;
	// «ÆŒ Ã‚
	if (Player[m_nPlayerIndex].m_ItemList.GetMoneyAmount() < PlayerSet.m_sTongParam.m_nMoney)
		return enumTONG_CREATE_ERROR_ID8;
	// ◊È∂”≤ªƒ‹Ω®∞Ôª·
	if (Player[m_nPlayerIndex].m_cTeam.m_nFlag)
		return enumTONG_CREATE_ERROR_ID9;

	return 0;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫µ√µΩrelayÕ®÷™£¨∞Ôª·¥¥Ω®≥…π¶£¨¥¶¿Ìœ‡”¶ ˝æ›
//-------------------------------------------------------------------------
BOOL	KPlayerTong::Create(int nCamp, char *lpszTongName)
{
//	if (!CheckCreateCondition(nCamp, lpszTongName))
//		return FALSE;

	m_nFlag			= 1;
	m_nFigure		= enumTONG_FIGURE_MASTER;
	m_nCamp			= nCamp;
	m_szTitle[0]	= 0;
	m_nMoney		= 0;
	strcpy(m_szName, lpszTongName);
	strcpy(m_szMasterName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	m_dwTongNameID	= g_FileName2Id(m_szName);

	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = m_nCamp;
	Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = m_nCamp;
	Player[m_nPlayerIndex].m_ItemList.DecMoney(PlayerSet.m_sTongParam.m_nMoney);
	Npc[Player[m_nPlayerIndex].m_nIndex].m_btRankId = this->m_nFigure + 74;
	// Õ®÷™øÕªß∂À
	TONG_CREATE_SYNC	sCreate;
	sCreate.ProtocolType = s2c_tongcreate;
	sCreate.m_btCamp = nCamp;
	if (strlen(lpszTongName) < sizeof(sCreate.m_szName))
		strcpy(sCreate.m_szName, lpszTongName);
	else
	{
		memcpy(sCreate.m_szName, lpszTongName, sizeof(sCreate.m_szName) - 1);
		sCreate.m_szName[sizeof(sCreate.m_szName) - 1] = 0;
	}
	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sCreate, sizeof(TONG_CREATE_SYNC));

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫µ√µΩ∑˛ŒÒ∆˜Õ®÷™¥¥Ω®∞Ôª·
//-------------------------------------------------------------------------
void	KPlayerTong::Create(TONG_CREATE_SYNC *psCreate)
{
	if (!psCreate)
		return;

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = psCreate->m_btCamp;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = psCreate->m_btCamp;

	m_nFlag			= 1;
	m_nFigure		= enumTONG_FIGURE_MASTER;
	m_nCamp			= psCreate->m_btCamp;
	m_szTitle[0]	= 0;
	memset(m_szName, 0, sizeof(m_szName));
	memcpy(m_szName, psCreate->m_szName, sizeof(psCreate->m_szName));
	strcpy(m_szMasterName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
	m_dwTongNameID	= g_FileName2Id(m_szName);

	// Õ®÷™ΩÁ√Ê∞Ôª·Ω®¡¢≥…π¶
	defFuncShowNormalMsg(MSG_TONG_CREATE_SUCCESS);

	// Õ®÷™∞Ôª·∆µµ¿
	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);

}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫Õ∑…œ «∑Ò–Ë“™∂•’“»À±Í÷æ
//-------------------------------------------------------------------------
BOOL	KPlayerTong::GetOpenFlag()
{
	return (m_nFlag && m_nFigure != enumTONG_FIGURE_MEMBER);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫◊™∑¢º”»Î∞Ôª·…Í«Î∏¯∂‘∑ΩøÕªß∂À
//-------------------------------------------------------------------------
BOOL	KPlayerTong::TransferAddApply(DWORD dwNpcID)
{
	// “—æ≠ «∞Ôª·≥…‘±
	if (m_nFlag)
		return FALSE;
	// ◊‘º∫µƒ’Û”™Œ Ã‚
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp != camp_free ||
		Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp != camp_free)
		return FALSE;
	// Ωª“◊π˝≥Ã÷–
	if (Player[m_nPlayerIndex].CheckTrading())
		return FALSE;
	// ◊È∂”≤ªƒ‹º”»Î∞Ôª·
	if (Player[m_nPlayerIndex].m_cTeam.m_nFlag)
		return FALSE;

	int	nTarget = Player[m_nPlayerIndex].FindAroundPlayer(dwNpcID);
	if (nTarget == -1)
		return FALSE;
	if (!Player[nTarget].m_cTong.CheckAcceptAddApplyCondition())
		return FALSE;

	m_nApplyTo = nTarget;

	// Õ®÷™øÕªß∂À
	TONG_APPLY_ADD_SYNC	sAdd;
	sAdd.ProtocolType = s2c_extendtong;
	sAdd.m_btMsgId = enumTONG_SYNC_ID_TRANSFER_ADD_APPLY;
	sAdd.m_nPlayerIdx = m_nPlayerIndex;
	strcpy(sAdd.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	sAdd.m_wLength = sizeof(TONG_APPLY_ADD_SYNC) - 1 - sizeof(sAdd.m_szName) + strlen(sAdd.m_szName);
	if (g_pServer)
		g_pServer->PackDataToClient(Player[nTarget].m_nNetConnectIdx, &sAdd, sAdd.m_wLength + 1);

	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫≈–∂œ «∑Òø…“‘◊™∑¢±»Àµƒº”»Î∞Ôª·…Í«Î
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CheckAcceptAddApplyCondition()
{
	if (!m_nFlag || m_nFigure == enumTONG_FIGURE_MEMBER)
		return FALSE;

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ «∑ÒΩ” ‹≥…‘± bFlag == TRUE Ω” ‹ == FALSE ≤ªΩ” ‹
//-------------------------------------------------------------------------
void	KPlayerTong::AcceptMember(int nPlayerIdx, DWORD dwNameID, BOOL bFlag)
{
	if (nPlayerIdx <= 0)
		return;

	TONG_ACCEPT_MEMBER_COMMAND	sAccept;
	sAccept.ProtocolType	= c2s_extendtong;
	sAccept.m_wLength		= sizeof(TONG_ACCEPT_MEMBER_COMMAND) - 1;
	sAccept.m_btMsgId		= enumTONG_COMMAND_ID_ACCEPT_ADD;
	sAccept.m_nPlayerIdx	= nPlayerIdx;
	sAccept.m_dwNameID		= dwNameID;
	sAccept.m_btFlag		= (bFlag != 0);

	if (g_pClient)
		g_pClient->SendPackToServer(&sAccept, sAccept.m_wLength + 1);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫∑¢œ˚œ¢Õ®÷™æ‹æ¯ƒ≥»À…Í«Î
//-------------------------------------------------------------------------
void	KPlayerTong::SendRefuseMessage(int nPlayerIdx, DWORD dwNameID)
{
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return;
	if (Player[nPlayerIdx].m_cTong.m_nApplyTo != m_nPlayerIndex ||
		Player[nPlayerIdx].m_nIndex <= 0 ||
		Player[m_nPlayerIndex].m_nIndex <= 0)
		return;
	if (g_FileName2Id(Npc[Player[nPlayerIdx].m_nIndex].Name) != dwNameID)
		return;
	
	int nLength = strlen(Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	SHOW_MSG_SYNC	sMsg;
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TONG_REFUSE_ADD;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID) + nLength;
	sMsg.m_lpBuf = new BYTE[sMsg.m_wLength + 1];

	memcpy(sMsg.m_lpBuf, &sMsg, sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID));
	memcpy((char*)sMsg.m_lpBuf + sizeof(SHOW_MSG_SYNC) - sizeof(LPVOID), Npc[Player[m_nPlayerIndex].m_nIndex].Name, nLength);

	if (g_pServer)
		g_pServer->PackDataToClient(Player[nPlayerIdx].m_nNetConnectIdx, sMsg.m_lpBuf, sMsg.m_wLength + 1);
}
#endif

void	KPlayerTong::GetTongName(char *lpszGetName)
{
	if (!lpszGetName)
		return;
	if (!m_nFlag)
	{
		lpszGetName[0] = 0;
		return;
	}

	strcpy(lpszGetName, m_szName);
}

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫≈–∂œ±»Àº”»Î◊‘º∫∞Ôª·Ãıº˛ «∑Ò≥…¡¢
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CheckAddCondition(int nPlayerIdx)
{
//--------------------- ◊‘º∫µƒÃıº˛ -----------------------
	if (nPlayerIdx <= 0 || nPlayerIdx >= MAX_PLAYER)
		return FALSE;
	if (!m_nFlag || m_nFigure == enumTONG_FIGURE_MEMBER)
		return FALSE;

//--------------------- ∂‘∑ΩµƒÃıº˛ -----------------------
	//  «∑Ò…Í«Î¡À
	if (Player[nPlayerIdx].m_cTong.m_nApplyTo != this->m_nPlayerIndex)
		return FALSE;
	// “—æ≠ «∞Ôª·≥…‘±
	if (Player[nPlayerIdx].m_cTong.m_nFlag)
		return FALSE;
	// ◊‘º∫µƒ’Û”™Œ Ã‚
	if (Npc[Player[nPlayerIdx].m_nIndex].m_CurrentCamp != camp_free ||
		Npc[Player[nPlayerIdx].m_nIndex].m_Camp != camp_free)
		return FALSE;
	// Ωª“◊π˝≥Ã÷–
	if (Player[nPlayerIdx].CheckTrading())
		return FALSE;
	// ◊È∂”≤ªƒ‹º”»Î∞Ôª·
	if (Player[nPlayerIdx].m_cTeam.m_nFlag)
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫º”»Î∞Ôª·£¨≥…Œ™∆’Õ®∞Ô÷⁄
//-------------------------------------------------------------------------
BOOL	KPlayerTong::AddTong(int nCamp, char *lpszTongName, char *lpszMasterName, char *lpszTitleName)
{
	if (!lpszTongName || !lpszMasterName || !lpszTitleName)
		return FALSE;
	// ∞Ôª·’Û”™Œ Ã‚
	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
		return FALSE;

	m_nFlag		= 1;
	m_nFigure	= enumTONG_FIGURE_MEMBER;
	m_nCamp		= nCamp;
	strcpy(this->m_szName, lpszTongName);
	strcpy(this->m_szMasterName, lpszMasterName);
	strcpy(this->m_szTitle, lpszTitleName);
	m_dwTongNameID	= g_FileName2Id(m_szName);

	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = m_nCamp;
	Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = m_nCamp;
	Npc[Player[m_nPlayerIndex].m_nIndex].m_btRankId = this->m_nFigure + 74;
	// Õ®÷™øÕªß∂À
	TONG_Add_SYNC	sAdd;
	sAdd.ProtocolType = s2c_extendtong;
	sAdd.m_wLength = sizeof(sAdd) - 1;
	sAdd.m_btMsgId = enumTONG_SYNC_ID_ADD;
	sAdd.m_btCamp = this->m_nCamp;
	strcpy(sAdd.m_szTongName, m_szName);
	strcpy(sAdd.m_szTitle, m_szTitle);
	strcpy(sAdd.m_szMaster, m_szMasterName);

	if (g_pServer)
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sAdd, sAdd.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫º”»Î∞Ôª·£¨≥…Œ™∆’Õ®∞Ô÷⁄
//-------------------------------------------------------------------------
BOOL	KPlayerTong::AddTong(int nCamp, char *lpszTongName, char *lpszTitle, char *lpszMaster)
{
	if (!lpszTongName || !lpszTongName[0] || strlen(lpszTongName) > defTONG_NAME_MAX_LENGTH)
		return FALSE;
	// ∞Ôª·’Û”™Œ Ã‚
	if (nCamp != camp_justice && nCamp != camp_evil && nCamp != camp_balance)
		return FALSE;

	m_nFlag		= 1;
	m_nFigure	= enumTONG_FIGURE_MEMBER;
	m_nCamp		= nCamp;
	strcpy(m_szName, lpszTongName);
	if (!lpszTitle)
		this->m_szTitle[0] = 0;
	else
		strcpy(m_szTitle, lpszTitle);
	if (!lpszMaster)
		this->m_szMasterName[0] = 0;
	else
		strcpy(m_szMasterName, lpszMaster);
	m_dwTongNameID	= g_FileName2Id(m_szName);

	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = m_nCamp;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = m_nCamp;

	defFuncShowNormalMsg(MSG_TONG_ADD_SUCCESS);

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫…Í«Î»Œ√¸
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyInstate(int nCurFigure, int nCurPos, int nNewFigure, int nNewPos, char *lpszName)
{
	if (!lpszName)
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (nCurFigure < 0 || nCurFigure >= enumTONG_FIGURE_NUM ||
		nNewFigure < 0 || nNewFigure >= enumTONG_FIGURE_NUM)
		return FALSE;
	if (nCurPos < 0 || nNewPos < 0)
		return FALSE;
	if (nCurFigure == nNewFigure && nCurPos == nNewPos)
		return FALSE;

	switch (m_nFigure)
	{
	case enumTONG_FIGURE_MEMBER:
		// ∞Ô÷⁄√ª”–»Œ√¸»®¡¶
		return FALSE;
	case enumTONG_FIGURE_MANAGER:
		// ∂”≥§√ª”–»Œ√¸»®¡¶
		return FALSE;
	case enumTONG_FIGURE_DIRECTOR:
		// ≥§¿œ÷ª”–∂‘∂”≥§°¢∞Ô÷⁄µƒ»Œ√¸»®¡¶
		if ((nCurFigure != enumTONG_FIGURE_MANAGER && nCurFigure != enumTONG_FIGURE_MEMBER) ||
			(nNewFigure != enumTONG_FIGURE_MANAGER && nNewFigure != enumTONG_FIGURE_MEMBER))
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_MANAGER && nCurPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_MANAGER && nNewPos >= defTONG_MAX_MANAGER)
			return FALSE;
		break;
	case enumTONG_FIGURE_MASTER:
		// ∞Ô÷˜”–∂‘≥§¿œ°¢∂”≥§°¢∞Ô÷⁄µƒ»Œ√¸»®¡¶
		if (nCurFigure == enumTONG_FIGURE_MASTER || nNewFigure == enumTONG_FIGURE_MASTER)
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_MANAGER && nCurPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nCurFigure == enumTONG_FIGURE_DIRECTOR && nCurPos >= defTONG_MAX_DIRECTOR)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_MANAGER && nNewPos >= defTONG_MAX_MANAGER)
			return FALSE;
		if (nNewFigure == enumTONG_FIGURE_DIRECTOR && nNewPos >= defTONG_MAX_DIRECTOR)
			return FALSE;
		break;
	default:
		return FALSE;
	}

	TONG_APPLY_INSTATE_COMMAND	sApply;

	if (strlen(lpszName) >= sizeof(sApply.m_szName))
		return FALSE;
	sApply.ProtocolType = c2s_extendtong;
	sApply.m_btMsgId = enumTONG_COMMAND_ID_APPLY_INSTATE;
	sApply.m_dwTongNameID = g_FileName2Id(this->m_szName);
	sApply.m_btCurFigure = nCurFigure;
	sApply.m_btCurPos = nCurPos;
	sApply.m_btNewFigure = nNewFigure;
	sApply.m_btNewPos = nNewPos;
	memset(sApply.m_szName, 0, sizeof(sApply.m_szName));
	strcpy(sApply.m_szName, lpszName);
	sApply.m_wLength = sizeof(sApply) - 1;

	if (g_pClient)
		g_pClient->SendPackToServer(&sApply, sApply.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫…Í«ÎÃﬂ»À
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyKick(int nCurFigure, int nCurPos, char *lpszName)
{
	if (!lpszName || !lpszName[0] || strlen(lpszName) >= 32)
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (nCurFigure != enumTONG_FIGURE_MANAGER && nCurFigure != enumTONG_FIGURE_MEMBER)
		return FALSE;
	if (nCurFigure == enumTONG_FIGURE_MANAGER && (nCurPos < 0 || nCurPos >= defTONG_MAX_MANAGER))
		return FALSE;

	TONG_APPLY_KICK_COMMAND	sKick;
	sKick.ProtocolType		= c2s_extendtong;
	sKick.m_wLength			= sizeof(sKick) - 1;
	sKick.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_KICK;
	sKick.m_btFigure		= nCurFigure;
	sKick.m_btPos			= nCurPos;
	sKick.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	strcpy(sKick.m_szName, lpszName);

	if (g_pClient)
		g_pClient->SendPackToServer(&sKick, sKick.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫…Í«Î¥´Œª
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyChangeMaster(int nCurFigure, int nPos, char *lpszName)
{
	if (!lpszName || !lpszName[0] || strlen(lpszName) >= 32)
		return FALSE;
	if (!m_nFlag)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;

	TONG_APPLY_CHANGE_MASTER_COMMAND	sChange;
	sChange.ProtocolType	= c2s_extendtong;
	sChange.m_wLength		= sizeof(sChange) - 1;
	sChange.m_btMsgId		= enumTONG_COMMAND_ID_APPLY_CHANGE_MASTER;
	sChange.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	sChange.m_btFigure		= nCurFigure;
	sChange.m_btPos			= nPos;
	strcpy(sChange.m_szName, lpszName);
	if (g_pClient)
		g_pClient->SendPackToServer(&sChange, sChange.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫…Í«Î¿Îø™∞Ôª·
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyLeave()
{
	if (!m_nFlag)
		return FALSE;
	if (m_nFigure == enumTONG_FIGURE_MASTER)
	{
		defFuncShowNormalMsg(MSG_TONG_CANNOT_LEAVE1);
		return FALSE;
	}
	if (m_nFigure == enumTONG_FIGURE_DIRECTOR)
	{
		defFuncShowNormalMsg(MSG_TONG_CANNOT_LEAVE2);
		return FALSE;
	}

	TONG_APPLY_LEAVE_COMMAND	sLeave;
	sLeave.ProtocolType		= c2s_extendtong;
	sLeave.m_wLength		= sizeof(sLeave) - 1;
	sLeave.m_btMsgId		= enumTONG_COMMAND_ID_APPLY_LEAVE;
	sLeave.m_btFigure		= m_nFigure;
	sLeave.m_btPos			= 0;
	sLeave.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	strcpy(sLeave.m_szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);

	if (g_pClient)
		g_pClient->SendPackToServer(&sLeave, sLeave.m_wLength + 1);

	return TRUE;
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫…Í«ÎªÒµ√∞Ôª·–≈œ¢
//-------------------------------------------------------------------------
BOOL	KPlayerTong::ApplyInfo(int nInfoID, int nParam1, int nParam2, int nParam3, char *lpszName/*=NULL*/)
{
	if (nInfoID < 0 || nInfoID >= enumTONG_APPLY_INFO_ID_NUM)
		return FALSE;

	TONG_APPLY_INFO_COMMAND	sInfo;
	sInfo.ProtocolType = c2s_extendtong;
	sInfo.m_btMsgId = enumTONG_COMMAND_ID_APPLY_INFO;
	sInfo.m_btInfoID = nInfoID;
	sInfo.m_nParam1 = nParam1;
	sInfo.m_nParam2 = nParam2;
	sInfo.m_nParam3 = nParam3;

	switch (nInfoID)
	{
	case enumTONG_APPLY_INFO_ID_SELF:
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;

	case enumTONG_APPLY_INFO_ID_MASTER:
		break;

	case enumTONG_APPLY_INFO_ID_DIRECTOR:
		break;

	case enumTONG_APPLY_INFO_ID_MANAGER:
		if (!m_nFlag)
			break;
		if (this->m_nFigure != enumTONG_FIGURE_MASTER &&
			this->m_nFigure != enumTONG_FIGURE_DIRECTOR)
			break;

		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);

		break;

	case enumTONG_APPLY_INFO_ID_MEMBER:
		if (!m_nFlag)
			break;
		if (this->m_nFigure != enumTONG_FIGURE_MASTER &&
			this->m_nFigure != enumTONG_FIGURE_DIRECTOR)
			break;

		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);

		break;

	case enumTONG_APPLY_INFO_ID_ONE:
		if (!lpszName || !lpszName[0])
			break;
		if (strlen(lpszName) >= 32)
			break;
		strcpy(sInfo.m_szBuf, lpszName);
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf) + strlen(lpszName);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;
	case enumTONG_APPLY_INFO_ID_TONG_HEAD:
		if (nParam1 <= 0 || nParam1 >= MAX_NPC)
			break;
		if (nParam1 != Player[CLIENT_PLAYER_INDEX].m_nIndex && Npc[nParam1].m_nTongFlag == 0)
			break;
		sInfo.m_nParam1 = Npc[nParam1].m_dwID;
		sInfo.m_wLength = sizeof(sInfo) - 1 - sizeof(sInfo.m_szBuf);
		if (g_pClient)
			g_pClient->SendPackToServer(&sInfo, sInfo.m_wLength + 1);
		break;
	}
	return TRUE;
}
#endif

DWORD	KPlayerTong::GetTongNameID()
{
	return (m_nFlag ? m_dwTongNameID : 0);
//	if (!m_nFlag)
//		return 0;
//	if (m_szName[0])
//		return 0;
//	return g_FileName2Id(m_szName);
}

//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ «∑Ò”–»®¡¶≤È—Ø∂”≥§–≈œ¢
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CanGetManagerInfo(DWORD dwTongNameID)
{
	if (!m_nFlag)
		return FALSE;
	if (dwTongNameID != g_FileName2Id(this->m_szName))
		return FALSE;
	if (m_nFigure == enumTONG_FIGURE_MASTER || m_nFigure == enumTONG_FIGURE_DIRECTOR)
		return TRUE;
	return FALSE;
}

//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ «∑Ò”–»®¡¶≤È—Ø∞Ô÷⁄–≈œ¢
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CanGetMemberInfo(DWORD dwTongNameID)
{
	if (!m_nFlag)
		return FALSE;
	if (dwTongNameID != g_FileName2Id(this->m_szName))
		return FALSE;
	if (m_nFigure == enumTONG_FIGURE_MASTER || m_nFigure == enumTONG_FIGURE_DIRECTOR)
		return TRUE;
	return FALSE;
}

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫∏¯øÕªß∂À∑¢ÀÕ◊‘º∫‘⁄∞Ôª·÷–µƒ–≈œ¢
//-------------------------------------------------------------------------
void	KPlayerTong::SendSelfInfo()
{
	TONG_SELF_INFO_SYNC	sInfo;
	sInfo.ProtocolType = s2c_extendtong;
	sInfo.m_wLength = sizeof(sInfo) - 1;
	sInfo.m_btMsgId = enumTONG_SYNC_ID_SELF_INFO;
	sInfo.m_btJoinFlag = this->m_nFlag;
	sInfo.m_btFigure = this->m_nFigure;
	sInfo.m_btCamp = this->m_nCamp;
	sInfo.m_dwMoney = this->m_nMoney;
	strcpy(sInfo.m_szMaster, this->m_szMasterName);
	strcpy(sInfo.m_szTitle, this->m_szTitle);
	strcpy(sInfo.m_szTongName, this->m_szName);
	if (g_pServer)
		g_pServer->PackDataToClient(Player[this->m_nPlayerIndex].m_nNetConnectIdx, &sInfo, sInfo.m_wLength + 1);
}
#endif

#ifndef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫…Ë∂®◊‘…Ì∞Ôª·–≈œ¢
//-------------------------------------------------------------------------
void	KPlayerTong::SetSelfInfo(TONG_SELF_INFO_SYNC *pInfo)
{
	if (pInfo->m_btJoinFlag == 0)
	{
		if (m_nFlag)
		{
			KUiGameObjectWithName	sUi;
			strcpy(sUi.szName, Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].Name);
			sUi.nData = TONG_ACTION_LEAVE;
			sUi.nParam = m_nFigure;
			sUi.uParam = 0;
			sUi.szString[0] = 0;
			CoreDataChanged(GDCNI_TONG_ACTION_RESULT, (unsigned int)&sUi, 1);

			// ±ªÃﬂ≥ˆ∞Ôª·
//			defFuncShowNormalMsg("ƒ˙±ªÃﬂ≥ˆ∞Ôª·£°");
			Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = camp_free;
			if (!Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag)
				Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = camp_free;
		}
		Clear();

		CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);

		return;
	}

	if (m_nFlag == 1 && m_nFigure != pInfo->m_btFigure)
	{
		// ±ª»Œ√¸
		defFuncShowNormalMsg("CÀp nhÀt danh s∏ch bang hÈi !");
	}

	this->m_nFlag = 1;
	this->m_nFigure = pInfo->m_btFigure;
	this->m_nCamp = pInfo->m_btCamp;
	this->m_nMoney = pInfo->m_dwMoney;
	Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_Camp = m_nCamp;
	if (Player[CLIENT_PLAYER_INDEX].m_cTeam.m_nFlag == 0)
		Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_CurrentCamp = m_nCamp;
	memcpy(this->m_szMasterName, pInfo->m_szMaster, sizeof(pInfo->m_szMaster));
	memcpy(this->m_szName, pInfo->m_szTongName, sizeof(pInfo->m_szTongName));
	memcpy(this->m_szTitle, pInfo->m_szTitle, sizeof(pInfo->m_szTitle));
	m_dwTongNameID	= g_FileName2Id(m_szName);

	CoreDataChanged(GDCNI_PLAYER_BASE_INFO, 0, 0);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ºÏ≤‚ «∑Ò”–»Œ√¸»®¿˚
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CheckInstatePower(TONG_APPLY_INSTATE_COMMAND *pApply)
{
	if (!pApply)
		return FALSE;
	if (pApply->m_wLength <= sizeof(TONG_APPLY_INSTATE_COMMAND) - 1 - sizeof(pApply->m_szName))
		return FALSE;
	if (!m_nFlag || (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR))
		return FALSE;
	if (pApply->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	if (pApply->m_btCurFigure == pApply->m_btNewFigure && pApply->m_btCurPos == pApply->m_btNewPos)
		return FALSE;

	char	szName[32];
	memcpy(szName, pApply->m_szName, sizeof(pApply->m_szName));
	szName[31] = 0;
	if (strlen(szName) >= 31)
		return FALSE;

	if (m_nFigure == enumTONG_FIGURE_MASTER)
	{
		if (pApply->m_btCurFigure == enumTONG_FIGURE_DIRECTOR)
		{
			if (pApply->m_btCurPos >= defTONG_MAX_DIRECTOR)
				return FALSE;
		}
		else if (pApply->m_btCurFigure == enumTONG_FIGURE_MANAGER)
		{
			if (pApply->m_btCurPos >= defTONG_MAX_MANAGER)
				return FALSE;
		}
		else if (pApply->m_btCurFigure == enumTONG_FIGURE_MEMBER)
		{
		}
		else
		{
			return FALSE;
		}

		if (pApply->m_btNewFigure == enumTONG_FIGURE_DIRECTOR)
		{
			if (pApply->m_btNewPos >= defTONG_MAX_DIRECTOR)
				return FALSE;
		}
		else if (pApply->m_btNewFigure == enumTONG_FIGURE_MANAGER)
		{
			if (pApply->m_btNewPos >= defTONG_MAX_MANAGER)
				return FALSE;
		}
		else if (pApply->m_btNewFigure == enumTONG_FIGURE_MEMBER)
		{
		}
		else
		{
			return FALSE;
		}
	}
	else // if (m_nFigure == enumTONG_FIGURE_DIRECTOR)
	{
		if (pApply->m_btCurFigure == enumTONG_FIGURE_MANAGER)
		{
			if (pApply->m_btCurPos >= defTONG_MAX_MANAGER)
				return FALSE;
		}
		else if (pApply->m_btCurFigure == enumTONG_FIGURE_MEMBER)
		{
		}
		else
		{
			return FALSE;
		}

		if (pApply->m_btNewFigure == enumTONG_FIGURE_MANAGER)
		{
			if (pApply->m_btNewPos >= defTONG_MAX_MANAGER)
				return FALSE;
		}
		else if (pApply->m_btNewFigure == enumTONG_FIGURE_MEMBER)
		{
		}
		else
		{
			return FALSE;
		}
	}

	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ºÏ≤‚ «∑Ò”–Ãﬂ»À»®¿˚
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CheckKickPower(TONG_APPLY_KICK_COMMAND *pKick)
{
	if (!pKick)
		return FALSE;
	if (pKick->m_wLength + 1 != sizeof(TONG_APPLY_KICK_COMMAND))
		return FALSE;
	if (!m_nFlag || (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR))
		return FALSE;
	if (pKick->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;

	if (pKick->m_btFigure == enumTONG_FIGURE_MANAGER)
	{
		if (pKick->m_btPos >= defTONG_MAX_MANAGER)
			return FALSE;
	}
	else if (pKick->m_btFigure == enumTONG_FIGURE_MEMBER)
	{
	}
	else
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫±ª»Œ√¸
//-------------------------------------------------------------------------
void	KPlayerTong::BeInstated(STONG_SERVER_TO_CORE_BE_INSTATED *pSync)
{
	this->m_nFlag = 1;
	this->m_nFigure = pSync->m_btFigure;
	strcpy(m_szTitle, pSync->m_szTitle);

	Npc[Player[m_nPlayerIndex].m_nIndex].m_btRankId = this->m_nFigure + 74;

	SendSelfInfo();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫±ªÃﬂ≥ˆ∞Ôª·
//-------------------------------------------------------------------------
void	KPlayerTong::BeKicked(STONG_SERVER_TO_CORE_BE_KICKED *pSync)
{
	Clear();

	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = camp_free;
	if (!Player[this->m_nPlayerIndex].m_cTeam.m_nFlag)
		Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = camp_free;

	Npc[Player[m_nPlayerIndex].m_nIndex].m_btRankId = 72;
	SendSelfInfo();

	SHOW_MSG_SYNC	sMsg;
	// ±ªÃﬂ≥ˆ∞Ôª·
	sMsg.ProtocolType = s2c_msgshow;
	sMsg.m_wMsgID = enumMSG_ID_TONG_BE_KICK;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ºÏ≤‚ «∑Ò”–¿Îø™»®¿˚
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CheckLeavePower(TONG_APPLY_LEAVE_COMMAND *pLeave)
{
	if (!pLeave)
		return FALSE;
	if (pLeave->m_wLength + 1 != sizeof(TONG_APPLY_LEAVE_COMMAND))
		return FALSE;
	if (!m_nFlag || m_nFigure == enumTONG_FIGURE_MASTER || m_nFigure == enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (pLeave->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫¿Îø™∞Ôª·
//-------------------------------------------------------------------------
void	KPlayerTong::Leave(STONG_SERVER_TO_CORE_LEAVE *pLeave)
{
	if (strcmp(pLeave->m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name) != 0)
		return;

	if (pLeave->m_bSuccessFlag)
	{
		Clear();
		Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = camp_free;
		if (!Player[this->m_nPlayerIndex].m_cTeam.m_nFlag)
			Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = camp_free;
		SendSelfInfo();

		SHOW_MSG_SYNC	sMsg;
		// ¿Îø™∞Ôª·≥…π¶
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TONG_LEAVE_SUCCESS;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	}
	else
	{
		SendSelfInfo();

		SHOW_MSG_SYNC	sMsg;
		// ¿Îø™∞Ôª· ß∞‹
		sMsg.ProtocolType = s2c_msgshow;
		sMsg.m_wMsgID = enumMSG_ID_TONG_LEAVE_FAIL;
		sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
		g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);
	}
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ºÏ≤‚ «∑Ò”–»®¿˚ªª∞Ô÷˜
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CheckChangeMasterPower(TONG_APPLY_CHANGE_MASTER_COMMAND *pChange)
{
	if (!pChange)
		return FALSE;
	if (pChange->m_wLength + 1 != sizeof(TONG_APPLY_CHANGE_MASTER_COMMAND))
		return FALSE;
	if (!m_nFlag || m_nFigure != enumTONG_FIGURE_MASTER)
		return FALSE;
	if (pChange->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	char	szName[32];
	memcpy(szName, pChange->m_szName, sizeof(pChange->m_szName));
	szName[31] = 0;
	if (strlen(szName) >= 31)
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ºÏ≤‚ «∑Ò”–ƒ‹¡¶Ω” ‹¥´Œª
//-------------------------------------------------------------------------
BOOL	KPlayerTong::CheckGetMasterPower(STONG_SERVER_TO_CORE_CHECK_GET_MASTER_POWER *pCheck)
{
	if (!pCheck)
		return FALSE;
	if (!m_nFlag || m_nFigure == enumTONG_FIGURE_MASTER)
		return FALSE;
	if (pCheck->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
//	if (m_nFigure != pCheck->m_btFigure)
//		return FALSE;
	if (strcmp(pCheck->m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name) != 0)
		return FALSE;
	// µ»º∂Œ Ã‚
	if (Npc[Player[m_nPlayerIndex].m_nIndex].m_Level < PlayerSet.m_sTongParam.m_nLevel || 
		(int)Player[m_nPlayerIndex].m_dwLeadLevel < PlayerSet.m_sTongParam.m_nLeadLevel)
		return FALSE;

	return TRUE;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫¥´Œªµº÷¬…Ì∑›∏ƒ±‰
//-------------------------------------------------------------------------
void	KPlayerTong::ChangeAs(STONG_SERVER_TO_CORE_CHANGE_AS *pAs)
{
	if (!pAs)
		return;
	if (!m_nFlag)
		return;
	if (pAs->m_dwTongNameID != g_FileName2Id(m_szName))
		return;

	m_nFigure = pAs->m_btFigure;
	strcpy(m_szTitle, pAs->m_szTitle);
	strcpy(this->m_szMasterName, pAs->m_szName);

	this->SendSelfInfo();

	SHOW_MSG_SYNC	sMsg;

	sMsg.ProtocolType = s2c_msgshow;
	if (m_nFigure == enumTONG_FIGURE_MASTER)
		sMsg.m_wMsgID = enumMSG_ID_TONG_CHANGE_AS_MASTER;
	else
		sMsg.m_wMsgID = enumMSG_ID_TONG_CHANGE_AS_MEMBER;
	sMsg.m_wLength = sizeof(SHOW_MSG_SYNC) - 1 - sizeof(LPVOID);
	g_pServer->PackDataToClient(Player[m_nPlayerIndex].m_nNetConnectIdx, &sMsg, sMsg.m_wLength + 1);

	return;
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫ªª∞Ô÷˜
//-------------------------------------------------------------------------
void	KPlayerTong::ChangeMaster(char *lpszMaster)
{
	if (!lpszMaster || !lpszMaster[0])
		return;
	if (strlen(lpszMaster) >= sizeof(this->m_szMasterName))
		return;
	strcpy(m_szMasterName, lpszMaster);

	this->SendSelfInfo();
}
#endif

#ifdef _SERVER
//-------------------------------------------------------------------------
//	π¶ƒ‹£∫µ«¬Ω ±∫ÚªÒµ√∞Ôª·–≈œ¢
//-------------------------------------------------------------------------
void	KPlayerTong::Login(STONG_SERVER_TO_CORE_LOGIN *pLogin)
{
	if (strcmp(Npc[Player[this->m_nPlayerIndex].m_nIndex].Name, pLogin->m_szName) != 0)
		return;

	if (pLogin->m_nFlag == 0)
	{
		Clear();
		Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = camp_free;
		if (!Player[m_nPlayerIndex].m_cTeam.m_nFlag)
			Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = m_nCamp;
		this->SendSelfInfo();

		return;
	}

	m_nFlag			= 1;
	m_nMoney		= pLogin->m_nMoney;
	m_nFigure		= pLogin->m_nFigure;
	m_nCamp			= pLogin->m_nCamp;
	m_dwTongNameID	= g_FileName2Id(pLogin->m_szTongName);
	m_nApplyTo		= 0;
	strcpy(m_szName, pLogin->m_szTongName);
	strcpy(m_szTitle, pLogin->m_szTitle);
	strcpy(m_szMasterName, pLogin->m_szMaster);

	Npc[Player[m_nPlayerIndex].m_nIndex].m_Camp = m_nCamp;
	if (!Player[m_nPlayerIndex].m_cTeam.m_nFlag)
		Npc[Player[m_nPlayerIndex].m_nIndex].m_CurrentCamp = m_nCamp;
	this->SendSelfInfo();
}
#endif

#ifndef _SERVER
void    KPlayerTong::OpenCreateInterface()
{
	CoreDataChanged(GDCNI_OPEN_TONG_CREATE_SHEET, 1, 0);
}

BOOL KPlayerTong::ApplySaveMoney(DWORD nMoney)
{
	if (!m_nFlag)
		return FALSE;
	if (nMoney <= 0)
		return FALSE;
	if (Player[CLIENT_PLAYER_INDEX].m_ItemList.GetMoney(room_equipment) < nMoney)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "SË ti“n nhÀp v≠Ót qu∏ sË ti“n cÒa b∂n th©n !");
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return FALSE;
	}
	TONG_APPLY_SAVE_COMMAND	sSave;
	sSave.ProtocolType		= c2s_extendtong;
	sSave.m_wLength			= sizeof(sSave) - 1;
	sSave.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_SAVE;
	sSave.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	sSave.m_dwMoney			= nMoney;
	strcpy(sSave.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	if (g_pClient)
		g_pClient->SendPackToServer(&sSave, sSave.m_wLength + 1);
	
	return TRUE;
}

BOOL KPlayerTong::ApplyGetMoney(DWORD nMoney)
{
	if (!m_nFlag)
		return FALSE;
	if (nMoney <= 0)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (this->m_nMoney < nMoney)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "Ng©n qu¸ kh´ng ÆÒ !");
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return FALSE;
	}

	TONG_APPLY_SAVE_COMMAND	sSave;
	sSave.ProtocolType		= c2s_extendtong;
	sSave.m_wLength			= sizeof(sSave) - 1;
	sSave.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_GET;
	sSave.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	sSave.m_dwMoney			= nMoney;
	strcpy(sSave.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	if (g_pClient)
		g_pClient->SendPackToServer(&sSave, sSave.m_wLength + 1);
	
	return TRUE;
}

BOOL KPlayerTong::ApplySndMoney(DWORD nMoney)
{
	if (!m_nFlag)
		return FALSE;
	if (nMoney <= 0)
		return FALSE;
	if (m_nFigure != enumTONG_FIGURE_MASTER && m_nFigure != enumTONG_FIGURE_DIRECTOR)
		return FALSE;
	if (this->m_nMoney < nMoney)
	{
		KSystemMessage	sMsg;
		sprintf(sMsg.szMessage, "Ng©n qu¸ kh´ng ÆÒ !");
		sMsg.eType = SMT_NORMAL;
		sMsg.byConfirmType = SMCT_NONE;
		sMsg.byPriority = 0;
		sMsg.byParamSize = 0;
		CoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);
		return FALSE;
	}
	
	TONG_APPLY_SAVE_COMMAND	sSave;
	sSave.ProtocolType		= c2s_extendtong;
	sSave.m_wLength			= sizeof(sSave) - 1;
	sSave.m_btMsgId			= enumTONG_COMMAND_ID_APPLY_SND;
	sSave.m_dwTongNameID	= g_FileName2Id(this->m_szName);
	sSave.m_dwMoney			= nMoney;
	strcpy(sSave.m_szName, Npc[Player[m_nPlayerIndex].m_nIndex].Name);
	if (g_pClient)
		g_pClient->SendPackToServer(&sSave, sSave.m_wLength + 1);
	
	return TRUE;
}
#endif

#ifdef _SERVER

BOOL KPlayerTong::CheckMoneyPower( TONG_APPLY_SAVE_COMMAND *pSave )
{
	if (!pSave)
		return FALSE;
	if (pSave->m_wLength + 1 != sizeof(TONG_APPLY_SAVE_COMMAND))
		return FALSE;
	if ((!m_nFlag || m_nFigure == enumTONG_FIGURE_MANAGER || m_nFigure == enumTONG_FIGURE_MEMBER) && pSave->m_btMsgId != enumTONG_COMMAND_ID_APPLY_SAVE)
		return FALSE;
	if (!m_nFlag && pSave->m_btMsgId == enumTONG_COMMAND_ID_APPLY_SAVE)
		return FALSE;
	if (pSave->m_dwTongNameID != g_FileName2Id(m_szName))
		return FALSE;
	
	return TRUE;
}

void KPlayerTong::ChangeMoney( DWORD dwMoney )
{
	m_nMoney = dwMoney;
	
	this->SendSelfInfo();
}
#endif




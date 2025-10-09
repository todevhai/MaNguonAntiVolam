#include "stdafx.h"
#include "Util.h"
#include "KPlayerNew.h"
#include <unistd.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <sys/mman.h>
//#include "subhook.c"
#include <stdio.h>
//#include <string>
//#include <linux/string.h>
//#include <unistd.h>
#include <iostream>
#include <cstdarg>
#include <thread>
#include <cassert>
#include <cstring>
#include <assert.h>
#include <fstream>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string.hpp>
#include "KItem.h"
#include <signal.h>
#include <execinfo.h>
#include <stdio.h>
#include "KLuaScript.h"
#include "include/INIReader.h"
#include "Main.h"
#include "Authenticate.h"
#include "Sqlite.h"
#include "KFirewall.h"
#include <sys/time.h>
#include <cmath>
#include "PluginWeb.h"
#include "TextList.h"
#include <sys/resource.h>
//using namespace std;
//subhook::Hook foo_hook;
//subhook_t func_hook;
KNpc* BasePlayer;
KPlayer* CurrenPlayer;
KPlayer* Player;
KNpc* Npc;
//KServerCore* g_pCoreServerShell;
//KServerCore* g_pServerCore;

//IServer* g_pServer;
//IGameServer* g_pGameServer;

KClientProcess ClientProcess;
KPlayerSet* PlayerSet;
KPlayerNew gKPlayerNew;
KPlayerInfo	m_KPlayerInfo[1200];
KClientInfo	m_ClientInfo[1200];
//KNpcTemplate*	g_pNpcTemplate[4000][6][120];
//KLuaScript g_CustomScript;
//_ShutdownClient ShutdownClient = (_ShutdownClient)0x80C0230;
_SyncGoldCoin SyncGoldCoin = (_SyncGoldCoin)0x80BFAC0;
_SendData SendData = (_SendData)0x80BFA80;

//_KLuaScript KLuaScript = (_KLuaScript)0x8264BB0; // `typeinfo name for'KLuaScript	08264BB0	
//_KLuaScript KLuaScript = (_KLuaScript)0x8264BB0; // `vtable for'KLuaScript	08264B80		
//_KLuaScript KLuaScript = (_KLuaScript)0x8264BB0; // `typeinfo for'KLuaScript	08264BC0		

//_RegisterFunctions RegisterFunctions = (_RegisterFunctions)0x821CCD0; // KLuaScriptSet::RegisterFunctions(TLua_Funcs *,int)	0821CCD0	
//_lua_pushcclosure _custom_lua_pushcclosure = (_lua_pushcclosure)0x822D390; // KLuaScriptSet::RegisterFunctions(TLua_Funcs *,int)	0821CCD0	
//_lua_setglobal _custom_lua_setglobal = (_lua_setglobal)0x822D110; // KLuaScriptSet::RegisterFunctions(TLua_Funcs *,int)	0821CCD0	
//_lua_pushstring _custom_lua_pushstring = (_lua_pushstring)0x822D400; // lua_pushstring	0822D400	
//_lua_gettop _custom_lua_gettop = (_lua_gettop)0x822C420; // lua_gettop	0822C420	
//_lua_tostring tostring = (_lua_tostring)0x822D4A0; // lua_tostring	0822D4A0	
//_lua_tonumber tonumber = (_lua_tonumber)0x822D4F0; // lua_tostring	0822D4A0	
////
//_GetPlayerIndex GetPlayerIndex = (_GetPlayerIndex)0x811DB20; // GetPlayerIndex(lua_State *)	0811DB20	

//_SendSystemInfo SendSystemInfo = (_SendSystemInfo)0x81DF070; // KPlayerChat::SendSystemInfo(int,int,char const*,char const*,int)	081DF070	


_CheckProtocolSize nCheckProtocolSize = (_CheckProtocolSize)0x8050280; // KClientProcess::ProcessMessage(ulong,void const*,uint)	0x81F74F0	
_ProcessLoginProtocol nProcessLoginProtocol = (_ProcessLoginProtocol)0x81F5FD0; // KClientProcess::ProcessLoginProtocol(ulong,void const*,uint)	081F5FD0	
_KProtocolProcess nKProtocolProcess = (_KProtocolProcess)0x80F3270; // KClientProcess::ProcessMessage(ulong,void const*,uint)	0x81F74F0	
_ProcessMessage nProcessMessage = (_ProcessMessage)0x81F74F0; // KClientProcess::ProcessMessage(ulong,void const*,uint)	0x81F74F0	
_AttachPlayer nAttachPlayer = (_AttachPlayer)0x80DD9A0; // KServerCore::AttachPlayer(ulong,_GUID const*)	0804EF10		
#define Execute_ProcessNetMsg		((void(__cdecl*)(KProtocolProcess* This, int nIndex, BYTE* pMsg)) 0x80F3270)//0x0063A578 
#define Execute_GetPlayerAccount		((bool(__cdecl*)(KPlayerSet *This, int nPlayerIndex, char *szName)) 0x80DBC00)//0x0063A578 
#define Execute_GetPlayerName		((bool(__cdecl*)(KPlayerSet *This, int nPlayerIndex, char *szName)) 0x80DBC50)//0x0063A578 

#define Execute_OnQuit		((void(__cdecl*)(KPlayer *This)) 0x80C5C10)//0x0063A578 
#define Execute_OnLeaveCurMap		((bool(__cdecl*)(KPlayer *This)) 0x80C7350)//0x0063A578 
#define Execute_WaitForRemove		((void(__cdecl*)(KPlayer *This)) 0x80BF160)//0x0063A578 

#define Execute_LoadPlayerBaseInfo		((int(__cdecl*)(KPlayer *This, char* pRoleBuffer, const char* pCurData, unsigned int *const nParam, BOOL bLogin)) 0x80D95A0)//0x0063A578 
#define Execute_LoadRoleData		((bool(__cdecl*)(LPCSTR * This, TRoleData* pRoleData, GUID* pGuid, KACCOUNT_INFO2* pAccountInfo)) 0x81ECC80)//0x0063A578 
#define Execute_SendData		((bool(__cdecl*)(KPlayer*, void*, int)) 0x80BFA80)//0x0063A578 
#define Execute_Msg2Player		((void(__cdecl*)(KPlayer *This, const char *szMsg)) 0x80C0310)//0x0063A578 

#define Execute_FileName2Id		((DWORD(__cdecl*)(LPCSTR *ScriptFileName)) 0x8217FB0) 
#define Execute_ExecuteScript_Name		((bool(__cdecl*)(KPlayer* This, LPCSTR ScriptFileName, LPCSTR szFunName, int nResultCount, LPCSTR cFormat, ...)) 0x80C7760)//0x0063A578 
#define Execute_ExecuteScript_vlist		((bool(__cdecl*)(KPlayer* This, DWORD ScriptID, LPCSTR szFunName, int nResultCount, LPCSTR cFormat, va_list vlist)) 0x80C7450)//0x0063A578 
#define Execute_ShutdownPlayer		((void(__cdecl*)(const KPlayer *This)) 0x80C0230)//0x0063A578 
#define Execute_CleanConnectionStatus		((void(__cdecl*)(KPlayer *)) 0x80C17D0)//0x0063A578 
#define Execute_DialogNpc		((void(__cdecl*)(KPlayer *This, BYTE *pProtocol)) 0x80C7EC0)//0x0063A578 
#define Execute_KItemSet_Add		((int(__cdecl*)(int This, int nItemGenre, int nItemQuality, int nSeries, int nLevel, int nLuck, int nDetailType, int nParticularType, int *pnMagicLevel, int nVersion, UINT nRandomSeed, IItemEnv *pEnv, BYTE bExtParam0, BYTE bExtParam1, int nExtParamMask)) 0x8083E80) 
#define Execute_AddItemOnBody		((bool(__cdecl*)(KPlayer *This, int nItemIdx, BOOL bDropIfFull, BOOL bAutoStack, BOOL bInEquipBox)) 0x80C68B0) 
#define Execute_ConsumeItem		((bool(__cdecl*)(KItemList *This, int nGenre, int nDetailType, int nParticular, int nLevel, ITEM_POSITION pos, int nConsumeNum)) 0x807B520) 
#define Execute_PayCoin		((bool(__cdecl*)(KItemList *This, int nPayNum)) 0x807B740) 
#define Execute_Remove		((bool(__cdecl*)(int This, int nIdx, int nOwnPlayerIdx, int mode, BOOL bEraseInMap)) 0x80825B0) 
#define Execute_DealSellItem		((int(__cdecl*)(KPlayerStall *This, int nIdx, int nFullPrice, int nTaxMoney)) 0x80E2490) 
#define Execute_DealBuyItem		((int(__cdecl*)(KPlayerStall *This, int nIdx, int nX, int nY, int nPrice)) 0x80E1C70) 
#define Execute_GetEquipmentCoin		((int(__cdecl*)(KItemList* This)) 0x8075B90) 
#define Execute_HasEnoughMoney		((bool(__cdecl*)(KPlayerStall *This, int nNeedMoney)) 0x80E1340) 
#define Execute_SetStallShopName		((void(__cdecl*)(KPlayerStall *This, BYTE *msg)) 0x80E3730) 
#define Execute_GetItemIdxFromID		((int(__cdecl*)(KPlayerStall *This, int nPlayerIdx, DWORD dwItemID)) 0x80E1AB0) 
#define Execute_GetPlayerIdxFromID		((int(__cdecl*)(KPlayerStall *This, DWORD dwPlayerID)) 0x80E1A60) 
#define Execute_GetStallInfo		((int(__cdecl*)(KPlayerStall *This, StallViewItemInfo *pStallViewItemInfo, int *pnCount)) 0x80E1AE0) 
#define Execute_CheckItemPriceRange		((int(__cdecl*)(KPlayerStall *This, int nPrice, BOOL bContainTax, int nSellerIndex)) 0x80E12B0) 
#define Execute_gMakeSyncItem_Info		((void(__cdecl*)(SViewItemInfo * itemInfo, KItem *const curItem)) 0x8075240) 
#define Execute_SyncStallInfo		((int(__cdecl*)(KPlayerStall *This, DWORD nPlayerIndex)) 0x80E3470) 


#define Execute_ProcessSyncReplyProtocol		((bool(__cdecl*)(KClientProcess *This, const unsigned int lnID, int nPlayerIndex, const char *pData, size_t dataLength)) 0x81F5D50)//0x0063A578 
#define Execute_OnLogin		((void(__cdecl*)(KPlayer *This)) 0x80C1A70)//0x0063A578 
#define Execute_LoadDBPlayerInfo		((bool(__cdecl*)(KPlayer *This, BYTE *pPlayerInfo, int* nStep, unsigned int* nParam)) 0x80D9600)//0x0063A578 
#define Execute_TextMsgProcessGMCmd		((bool(__cdecl*)(int nPlayerIdx, const char *pGMCmd, int nLen)) 0x82032E0)//0x0063A578 
#define Execute_ExecuteRelayScript		((void(__cdecl*)(KNewProtocolProcess* This, const void* pData, size_t nLen)) 0x806E370)//0x0063A578 
#define Execute_SendData_Host		((bool(__cdecl*)(CClientConnection* This, const void* pData, unsigned int nLen)) 0x804CF20)//0x0063A578 
#define Execute_RegisterProcessEvent		((bool(__cdecl*)(KPlayer *This, int nEvent)) 0x80C2F40)//0x0063A578 
//#define Execute_ExecuteScript		((bool(__cdecl*)(KPlayer *This, _DWORD dwScriptId, LPCSTR szFunName, LPCSTR szParams, unsigned int nResultCount)) 0x80C6E80)//0x0063A578 
#define Execute_NotifyHostEnterGame		((void(__cdecl*)(KServerCore *This, LPCSTR pszAccount, LPCSTR pszRole, int nPlayerIndex, _DWORD dwNameID, unsigned int lnID, int nLevel)) 0x80516C0)//0x0063A578 
#define Execute_NotifyBishopEnterGame		((void(__cdecl*)(KServerCore *This, LPCSTR pszAccount)) 0x80509D0)//0x0063A578 
#define Execute_AddPlayerToWorld		((void(__cdecl*)(KServerCore *This, int nIndex, _DWORD dwInitKey, _DWORD dwProcessId)) 0x804FA70)//0x0063A578 
#define Execute_ProcessClientMessage		((void(__cdecl*)(IGameServer *const This, unsigned int lnID, const void *pData, size_t nLen)) 0x804DDE0)//0x0063A578 
#define Execute_ProcessServerMessage		((void(__cdecl*)(IGameServer* const This, KE_SERVERTYPE nType, const void *pData, int)) 0x804DE40)//0x0063A578 
#define Execute_ProcessMessage		((void(__cdecl*)(IGameServer* const This, int, const void *, int)) 0x824A5E8)//0x0063A578 
#define Execute_CheckProtocolSize		((bool(__cdecl*)(KServerCore *This, int nPlayerIndex, const char *pChar, int nSize, BOOL *const bShutDown)) 0x8050280)//0x0063A578 
#define Execute_PlayerExchangeServer		((void(__cdecl*)(KServerCore *This)) 0x8050DC0)//0x0063A578 
#define Execute_IsPlayerExchangingServer		((bool(__cdecl*)(KServerCore *This, int nIndex)) 0x804ED00)//0x0063A578 
#define Execute_SavePlayerData		((bool(__cdecl*)(KServerCore *This, int nIndex, bool bUnLock, bool bClearMap)) 0x8050C40)//0x0063A578 
#define Execute_SendSyncCurPlayer		((bool(__cdecl*)(KPlayer *This, int nPlayerIndex)) 0x80C4BD0)//0x0063A578 
#define Execute_LaunchPlayer		((void(__cdecl*)(KPlayer * This)) 0x80CB900) 
#define Execute_SetPlayerIndex		((void(__cdecl*)(KPlayer * This, int nNo)) 0x80BF070) 
#define Execute_GetExpPercent		((bool(__cdecl*)(KPlayer * This, int *pnGetPercent)) 0x80BF7F0) 
#define Execute_ChatRoomRelayMsgProcess		((void(__cdecl*)(CChatRoomRelayMsgProcess *This)) 0x8191140)//0x0063A578 
#define Execute_CChatRoomRelayMsgProcess		((void(__cdecl*)(CChatRoomRelayMsgProcess *This)) 0x8190920)//0x0063A578 
#define Execute_NotifyChatChannel		((void(__cdecl*)(CChatRoomRelayMsgProcess *This, int nPlayerIndex)) 0x8191810)//0x0063A578 
#define Execute_KPlayerSave		((bool(__cdecl*)(KPlayer *This, BOOL bQuitSave)) 0x80C5140)//0x0063A578 
#define Execute_KPlayer_Earn		((bool(__cdecl*)(KPlayer *This, int nMoney)) 0x80C1BE0)//0x0063A578 
#define Execute_SavePlayerBaseInfo		((int(__cdecl*)(KPlayer *This, BYTE *pRoleBuffer, BOOL bQuitSave)) 0x80D7B60)//0x0063A578 
#define Execute_GetTenRoleListPlus		((TRoleList*(__cdecl*)(KLadder *This, DWORD nID)) 0x8085A80)//0x0063A578 

#define Execute_KObjSet_FindID		((int(__cdecl*)(KObjSet *This, int nID)) 0x80BD2B0)//0x0063A578 
#define Execute_KNpc_SendDataToNearRegion		((void(__cdecl*)(KNpc* This, void* pBuffer, DWORD dwSize, int nMaxSync, BOOL bForce)) 0x8091490)//0x0063A578 
#define Execute_ProcessPlayerTongMsg		((void(__cdecl*)(KClientProcess* This,const unsigned int nPlayerIdx,const char* pData,size_t dataLength)) 0x81F6020)//0x0063A578 
#define Execute_NotifyHostLeaveGame		((void(__cdecl*)(KServerCore *This, LPCSTR pszAccount, LPCSTR pszRole, int nType)) 0x8051750)//0x0063A578 

#define Execute_GetStateSkillEffect_Npc		((bool(__cdecl*)(KNpc * This, int nSkillID, int* nSkillLevel, int* nStateType, int* nEndTime, bool* bIsNoClearOnDeath)) 0x80905E0)//0x0063A578 
#define Execute_Activate_Npc		((void(__cdecl*)(KNpcAI* This, int nIndex)) 0x80AC8E0)//0x0063A578 
#define Execute_GetWeaponDamage		((void(__cdecl*)(KItemList* This, int *nMin, int *nMax)) 0x8077D70)//0x0063A578 
#define Execute_GetWeaponType		((int(__cdecl*)(KItemList* This)) 0x8075210)//0x0063A578 
#define Execute_UpdataCurData		((void(__cdecl*)(KPlayer* This, BOOL bReCalcState)) 0x80C3610)//0x0063A578 
#define Execute_SetNpcDamageAttrib		((void(__cdecl*)(KPlayer* This)) 0x80C3950)//0x0063A578 
#define Execute_GetMyNpc		((KNpc*(__cdecl*)(KPlayer* This)) 0x80BF1A0)//0x0063A578 
#define Execute_GetEquipEnhance		((int(__cdecl*)(KItemList *This, int nPlace, BOOL bIgnoreSuite)) 0x8078F20)//0x0063A578 
#define Execute_DeactivateEquip		((void(__cdecl*)(KItemList *This, int nIdx, int nEquipPlace, std::vector<KITEM_CONSUME> *pItems)) 0x807A9E0)//0x0063A578 

#define Execute_RemoveBaseAttribFromNPC		((void(__cdecl*)(KItem *This, KNpc *pNpc)) 0x806EDD0)//0x0063A578 
#define Execute_UnEquipMagicForSuite		((bool(__cdecl*)(KItemList *This, KNpc *pNpc, int nEquipPlace, int nItemIdx)) 0x80793D0)//0x0063A578 

#define Execute_GroupChat		((bool(__cdecl*)(KServerCore *This, BYTE tgtcls, DWORD tgtid, const void *pData, size_t size)) 0x804EB80)//0x0063A578 
#define Execute_SetPhysicsDamage		((void(__cdecl*)(KNpc* This, int nMinDamage, int nMaxDamage)) 0x80906B0)//0x0063A578 
#define Execute_SetHide		((void(__cdecl*)(KNpc* This, int nHideMode)) 0x8099860)//0x0063A578 
#define Execute_NpcAttribModify		((void(__cdecl*)(KNpcAttribModify *This, KNpc *pLauncher, KNpc *pNpc, KMagicAttrib *pMagic, BOOL bIsEndProcess)) 0x80ACA30)//0x0063A578 
#define Execute_ModifyAttrib		((void(__cdecl*)(KNpc* This, int nAttacker, KMagicAttrib *pData, BOOL bIsEndProcess)) 0x80930F0)//0x0063A578 
#define Execute_NormalSync		((void(__cdecl*)(KNpc* This, KPlayer *pReceiver)) 0x8098AF0)//0x0063A578 
#define Execute_Npc_SendSyncData		((bool(__cdecl*)(KNpc* This, KPlayer *pReceiver)) 0x80994B0)//0x0063A578 
#define Execute_DoMagic		((void(__cdecl*)(KNpc* This)) 0x8096E80)//0x0063A578 
#define Execute_ClearStateSkillEffect		((void(__cdecl*)(KNpc* This, BOOL bClearCastPassivitySkill, BOOL bSyncClient, BOOL bNegativeStateOnly, BOOL bOnDeath)) 0x809CDD0)//0x0063A578 
#define Execute_ReCalcState		((void(__cdecl*)(KPlayer* This)) 0x80C34C0)//0x0063A578 
#define Execute_SyncAttribute		((void(__cdecl*)(KNpc* This)) 0x8094610)//0x0063A578 
#define Execute_SyncAttributes		((void(__cdecl*)(KNpc* This, KPlayer* Player)) 0x8094490)//0x0063A578 
#define Execute_DisWatch		((void(__cdecl*)(KPlayer* This)) 0x80C4DA0)//0x0063A578 
#define Execute_OnEnterNewMap		((bool(__cdecl*)(KPlayer* This)) 0x80C73D0)//0x0063A578 
#define Execute_SyncNoChangeSpeed		((void(__cdecl*)(KNpc* This)) 0x80943F0)//0x0063A578 
#define Execute_ReCalcStateEffect		((void(__cdecl*)(KNpc* This)) 0x8094540)//0x0063A578 
#define Execute_SendCurNormalSyncData		((void(__cdecl*)(KPlayer* This)) 0x80C36E0)//0x0063A578 
#define Execute_UpdateNpcStateInfo		((void(__cdecl*)(KNpc* This)) 0x809F220)//0x0063A578 
#define Execute_RemoveStateSkillEffect		((void(__cdecl*)(KNpc* This, int nSkillID, BOOL bSyncClient)) 0x8093140)//0x0063A578 
#define Execute_GetMyPlayer		((KPlayer*(__cdecl*)(KNpc* This)) 0x8094180)//0x0063A578 
#define Execute_GetMyNpc		((KNpc*(__cdecl*)(KPlayer* This)) 0x80BF1A0)//0x0063A578 
#define Execute_SyncCastState		((void(__cdecl*)(KNpc* This)) 0x8090DD0)//0x0063A578 
#define Execute_ClearHideState		((void(__cdecl*)(KNpc* This)) 0x80947D0)//0x0063A578 
#define Execute_SyncSkillFailed		((void(__cdecl*)(KNpc* This, int nSkillID)) 0x809B660)//0x0063A578 
#define Execute_DoOrdinSkill		((int(__cdecl*)(KNpc* This, KSkill* pSkill, int nX, int nY)) 0x809C160)//0x0063A578 
#define Execute_SyncPlayer		((bool(__cdecl*)(KNpc *This, KPlayer *pReceiver)) 0x8091EC0)//0x0063A578 
#define Execute_SendSyncData		((void(__cdecl*)(KRegion *This, KPlayer *pReceiver)) 0x80F94A0)//0x0063A578 
#define Execute_OnAddTagRequest		((void(__cdecl*)(KProtocolProcess *This, int nIndex, BYTE *pData)) 0x80F4380)//0x0063A578 
#define Execute_OnGetTagsRequest		((void(__cdecl*)(KProtocolProcess *This, int nIndex, BYTE *pData)) 0x80F0190)//0x0063A578 
#define Execute__RecordDataArrival		((void(__cdecl*)(tagRelaySDBOper *pResult, int nSize)) 0x814D790)//0x0063A578 
#define Execute_SyncTaskValueToClient		((bool(__cdecl*)(KPlayer *This, int nTaskID)) 0x80C0600)//0x0063A578 
#define Execute_AddCharacter		((int(__cdecl*)(KServerCore *This, const tagExtPointInfo *const cExtPointInfo, const tagExtPointInfo *const cChangeExtPointInfo, _DWORD nTotalLeftTime, _DWORD nLeftTimeOfPoint, const KACCOUNT_LIMITTIME_INFO *const LimitTime, BOOL bExchange, const TRoleData *pData, const GUID *pGuid)) 0x804FC00)//0x0063A578 
#define Execute_ProcessTransfer		((void(__cdecl*)(KHostProcess *This, const void *pData, size_t nLen, KTRAN_UNIT *pUnit)) 0x81EE3D0)//0x0063A578 
#define Execute_BishopGatewayLargePackProcess		((void(__cdecl*)(KBishopProcess *This, const void *pData, size_t dataLength)) 0x81ED420)//0x0063A578 
#define Execute_GatewaySmallPackProcess		((void(__cdecl*)(KBishopProcess *This, const void *pData, size_t dataLength)) 0x81ED700)//0x0063A578 
//#define Execute_ProcessApplyJoinTongFromZhaoMu		((void(__cdecl*)(KServerCore* This, int nPlayerIndex, DWORD dwTongID)) 0x804EFB0)//0x0063A578 
#define Execute_AcceptOrRefuseApplyToRelay		((bool(__cdecl*)(KPlayerTong* This, BOOL bAccept, char* pszName, int nPageIndex)) 0x80E9290)//0x0063A578 
#define Execute_ProcessApplyJoinTongFromZhaoMu		((void(__cdecl*)(KPlayerTong *This, DWORD dwTongID)) 0x80E9980)//0x0063A578 
#define Execute_FindAccount		((bool(__cdecl*)(KPlayerSet *This, LPCSTR pszAccount, int *pnPlayerIndex)) 0x80DE490)//0x0063A578 
#define Execute_FindRole		((int(__cdecl*)(KPlayerSet *This, LPCSTR pszRoleName)) 0x80DDAC0)//0x0063A578 
#define Execute_GetMemberCount		((int(__cdecl*)(int nMask)) 0x80E6C40)//0x0063A578 
#define Execute_OnJoinTong		((void(__cdecl*)(KPlayerTong *This, DWORD dwTongID, DWORD dwOffer)) 0x80E8F80)//0x0063A578 
#define Execute_CheckAddCondition		((bool(__cdecl*)(KPlayerTong *This, int nPlayerIdx)) 0x80E7DB0)//0x0063A578 
#define Execute_TongScriptFunsGetTongMember		((int(__cdecl*)(DWORD dwTongID, DWORD dwMember)) 0x81A6C90)//0x0063A578 
#define Execute_GetTongNameID		((_DWORD(__cdecl*)(KPlayerTong * This)) 0x80E6BB0)//0x0063A578 
#define Execute_LoadFromTongData		((void(__cdecl*)(KPlayerTong *This)) 0x80EBF30)//0x0063A578 
#define Execute_GetTongName		((bool(__cdecl*)(KPlayerTong * This, char *lpszGetName)) 0x80E7010)//0x0063A578 
#define Execute_HashString2Id		((_DWORD(__cdecl*)(LPCSTR lpString)) 0x8218030)//0x0063A578 
#define Execute_ProcessCheckApplyJoinTong		((void(__cdecl*)(KServerCore *This, BYTE *pChar, size_t size)) 0x8050720)//0x0063A578 
#define Execute_OfflineDestroy		((void (__cdecl*)(KServerCore *This, int nIndex, BOOL bTimeOut)) 0x8053830)//0x0063A578 
#define Execute_AddSkillExp		((int (__cdecl*)(KSkillList* This, KMagicAttrib *pAttrib, BOOL bPercent)) 0x80FDD60)//0x0063A578 
#define Execute_ReceiveDamage		((bool (__cdecl*)(KNpc *This, int nLauncher, int nSeries, BOOL bIsMelee, void *pData, BOOL bUseAR, int nDoHurt, NPC_RELATION eRelation, int nSkillID)) 0x80A1DD0)//0x0063A578 
#define Execute_TongManagerAgentProcessClientProtocol		((bool (__cdecl*)(KTongManagerAgent *This, int nPlayerIdx, BYTE *pData)) 0x81CC3A0)//0x0063A578 
#define Execute_Member_Add		((bool(__cdecl*)(KTongLogic *This, _DWORD dwExecutorID, _DWORD dwTongID, LPCSTR szName, _DWORD dwSex, _DWORD dwLevel, _DWORD dwJoinTime, _DWORD dwMemberID, _DWORD dwOfferBring)) 0x81C1BA0)//0x0063A578 
#define Execute_RunSuffixScript		((bool(__cdecl*)(KTongLogic *This, _DWORD dwTongID, _DWORD dwExecutorID, LPCSTR szScriptName, LPCSTR szFuncName, LPCSTR szParamFormat, ...)) 0x81B7E20)//0x0063A578 
#define Execute_ProcessProtocol		((bool(__cdecl*)(KTongLogic *This, TongExProtocol eTongProtocolID, BYTE *pTongProtocolData, _DWORD dwTongProtocolSize)) 0x81CD840)//0x0063A578 
#define Execute_TONG_AddMoneyFund		((bool(__cdecl*)(KTongLogic *This, DWORD dwExecutorID, DWORD dwTongID, INT64 llValue)) 0x81BE7A0)//0x0063A578 
#define Execute_TongProcess_GetPlayerIndex		((int(__cdecl*)(LPCSTR pcszRole, int nLinkId)) 0x81F3810)//0x0063A578 
#define Execute_FindTongMemberPlayerIdx		((int(__cdecl*)(const KTongMember *pTongMember)) 0x80E8B20)//0x0063A578 
#define Execute_TONGM_GetOffer		((int(__cdecl*)(DWORD dwTongID, DWORD dwMember)) 0x81AD2D0)//0x0063A578 
#define Execute_ProcessClientProtocol		((bool(__cdecl*)(KTongManagerAgent *This, int nPlayerIdx, BYTE *pData)) 0x81CC3A0)//0x0063A578 
#define Execute_NpcSkillCommand		((void(__cdecl*)(KProtocolProcess *This, int nIndex, BYTE *pProtocol)) 0x80F29D0)//0x0063A578 
#define Execute_PlayerPrepareQuit		((void(__cdecl*)(KServerCores *This, int nPlayerIndex)) 0x804E2F0)//0x0063A578 
#define Execute_S2CExecuteScript		((void(__cdecl*)(KPlayer *This, char *ScriptName, char *szParam)) 0x80C13C0)//0x0063A578 

#define Execute_GetTongMember		((std::_Rb_tree_node_base*(__cdecl*)(DWORD dwTongID, DWORD dwMember)) 0x81A6C90)//0x0063A578 
//#define Execute_findTongData		((void(__cdecl*)(_Rb_tree<long unsigned int, std::pair<const long unsigned int, KTongData*>, std::_Select1st<std::pair<const long unsigned int, KTongData*>>, std::less<long unsigned int>, std::allocator<std::pair<const long unsigned int, KTongData*>>>*, void *, unsigned int*)) 0x8056800)//0x0063A578 
#define Execute_findTongData		((std::_Rb_tree_iterator<std::pair<const long unsigned int,KTongData*> > *(__cdecl*)(std::_Rb_tree_iterator<std::pair<const long unsigned int,KTongData*> >*, void *, unsigned int*)) 0x8056800)//0x0063A578 
#define Execute_findPlayerData		((std::_Rb_tree_iterator<std::pair<const long unsigned int,KTongMember*> > *(__cdecl*)(std::_Rb_tree_iterator<std::pair<const long unsigned int,KTongMember*> > *, void *, unsigned int*)) 0x80567A0)//0x0063A578 
#define Execute_KNpcGetPlayerIdx		((int(__cdecl*)(KNpc *This)) 0x8090360)//0x0063A578 

#define Execute_Init		((int(__cdecl*)(const KItemList* This, int nPlayerIdx)) 0x80778F0)//0x0063A578 
#define Execute_SendMoneySync		((void(__cdecl*)(const KItemList* This)) 0x8075DD0)//0x0063A578 
#define Execute_GetEquipmentMoney		((int(__cdecl*)(const KItemList* This)) 0x8075840)//0x0063A578 
#define Execute_GetMoneyAmount		((int(__cdecl*)(const KItemList* This)) 0x80785F0)//0x0063A578 
#define Execute_CostMoney		((bool(__cdecl*)(KItemList * This, int nMoney)) 0x8076EA0)//0x0063A578 
#define Execute_PrePay		((bool(__cdecl*)(KPlayer *This, int nMoney)) 0x80C0860)//0x0063A578 
#define Execute_Pay		((bool(__cdecl*)(KPlayer* This, int nMoney)) 0x80C0830)
//#define Execute_GetIP		((const char *(__cdecl*)((IServer* This, DWORD nPlayer)) *(DWORD *)0x82B8824 + 52)

#define Execute_ServerSendChat		((void(__cdecl*)(KPlayerChat* This, int nPlayerIdx, BYTE* pProtocol)) 0x81DF800)//0x0063A578 
#define Execute_ChatWorld		((bool(__cdecl*)(KServerCore *This, DWORD dwIndex, LPCVOID pMsg, size_t size)) 0x804E5C0)//0x0063A578 
#define Execute_ChatScreen		((bool(__cdecl*)(KServerCore *This, DWORD dwIndex, const void* pMsg, size_t size)) 0x8050510)//0x0063A578 
#define Execute_CheckTalkChannel		((bool(__cdecl*)(KPlayerChat *This, int nChannel)) 0x81DE780)//0x0063A578 
#define Execute_ProcessRelayMsg		((bool(__cdecl*)(KClientProcess* This, unsigned int lnID, int nIndex, const char* pData, size_t dataLength)) 0x81F7130)//0x0063A578 
#define Execute_LoadRoleDataInfo		((int(__cdecl*)(KPlayer *This, TRoleBaseInfo *pBaseInfo, BOOL bLogin)) 0x80D8A90)//0x0063A578 
#define Execute_ExecuteScript_File		((bool(__cdecl*)(KPlayer* This, LPCSTR ScriptFileName, LPCSTR szFunName, _DWORD nResultCount, LPCSTR cFormat, ...)) 0x80C7760)//0x0063A578 
#define Execute_ExecuteScript		((bool(__cdecl*)(KPlayer* This, LPCSTR ScriptFileName, LPCSTR szFunName, int nParam, unsigned int nResultCount)) 0x80C8D30)//0x0063A578 
#define Execute_ExecuteScript_2		((bool(__cdecl*)(KPlayer* This, LPCSTR ScriptFileName, LPCSTR szFunName, LPCSTR szParams, unsigned int nResultCount)) 0x80C7140)//0x0063A578 
#define Execute_GetTmpScript		((KLuaScript*(__cdecl*)(KLuaScriptSet *This)) 0x821C970)//0x0063A578 
#define Execute_CallFunction		((bool(__cdecl*)(KLuaScript *This, LPCSTR cFuncName, int nResults, LPCSTR cFormat, ...)) 0x821C440)//0x0063A578 
#define pGetItemBmdStruct(ItemId)	(ItemBmdStruct*)(*(DWORD*)0x8128AC0 + sizeof(ItemBmdStruct) * ItemId)
#define g_ProtocolProcess		((void *(*)()) 0x80F4AC0)
#define NotifyGoddessEnterGame		(*((void(__cdecl**)(IGameServer* , _DWORD, char*, int)) g_pCoreServerShell->m_pServer->_vptr_IGameServer + 2))
#define Execute_IsSpectator		((BOOL(__cdecl*)(CSpectatorMap *This, int nIndex)) 0x8176320)//0x0063A578
#define Execute_GetGameData		((int(__cdecl*)(KServerCores *This, unsigned int uDataId, unsigned int uParam, int nParam)) 0x8052030)//0x0063A578
#define Execute_GetClientInfo		((LPCSTR(__cdecl*)(KSOServer *This, unsigned int nId)) 0x804B070)//0x0063A578
#define Execute_ShutdownClient		((void(__cdecl*)(KSOServer *This, unsigned int nId)) 0x804B040)//0x0063A578
#define Execute_PackDataToAll		((unsigned int(__cdecl*)(KMission *This, const char* pBuffer, unsigned int ulBufferSize)) 0x8204390)//0x0063A578

#define Execute_LoadMap		((bool(__cdecl*)(KSubWorld *This, int nId)) 0x810A980)
//#define pKPlayerThis			((LPVOID(*)()) 0x88A1A20)
//#define pGetKPlayer		((DWORD(__thiscall*)(LPVOID This, int PlayerIndex)) 0x0096A4C0)

BYTE		szBuffIpMac[64];
//struct sigaction sig_action;
//struct sigaction sig_old_action;
////KLadder* Ladder = (KLadder*)Player_Rank;
void* LoadRank(void*);

KPlayerNew::KPlayerNew() {
	//Hook();
}
// -----------------------------------------------------------------------------

KPlayerNew::~KPlayerNew() {
}

//handler(int signalNum, , struct sigcontext ctx, siginfo_t* si, void* unused, , struct sigcontext ctx)

static void sigaction_player(int signal, siginfo_t* si, void* arg)
{
	ucontext_t* ctx = (ucontext_t*)arg;
	
	/* We are on linux x86, the returning IP is stored in RIP (64bit) or EIP (32bit).
	   In this example, the length of the offending instruction is 6 bytes.
	   So we skip the offender ! */
#if __WORDSIZE == 64
	   //printf("Caught SIGSEGV, addr %p, RIP 0x%lx\n", si->si_addr, ctx->uc_mcontext.gregs[REG_RIP]);
	ctx->uc_mcontext.gregs[REG_RIP] += 6;
#else
	   //printf("Caught SIGSEGV, addr %p, EIP 0x%x\n", si->si_addr, ctx->uc_mcontext.gregs[REG_EIP]);
	ctx->uc_mcontext.gregs[REG_EIP] += 6;
#endif

	//sigaction(11, &sig_old_action, NULL);

	//printf("signal_handler: %d\n", nSignal);
	//sigignore(SIGINT);
	//signal(SIGSEGV, SIG_IGN);
	//int pid;

	//pid = fork();
	//signal(SIGINT, signal_handler);
	//signal(SIGINT, SIG_IGN);

	//signal(SIGPIPE, SIG_IGN);
	/*if (pid > 0)
	{
		signal(SIGINT, signal_handler);
		kill(pid, SIGTERM);
	}*/
	//printf("Got SIGSEGV at address: %p\n", si->si_addr);
	//exit(EXIT_FAILURE);
	//throw(nSignal);
	//exit(signal);
}

static void handler(int signalNum, siginfo_t* si, void* unused)
{
	//signal(SIGINT, SIG_IGN);
	//printf("Got SIGSEGV at address: %p\n", si->si_code);
	//printf("255.255.255.255|255.255.255.255|00:00:00:00:00:00", si->si_addr);
	//exit(EXIT_FAILURE);
	void* trace[16];
	char** messages = (char**)NULL;
	int i, trace_size = 0;

	if (signalNum == SIGSEGV)
		printf("Got signal %d, faulty address is %p, "
			"from %p\n", signalNum, si->si_addr, si->si_call_addr);
	else
		printf("Got signal %d\n", signalNum);

	trace_size = backtrace(trace, 16);
	/* overwrite sigaction with caller's address */
	trace[1] = (void*)si->si_call_addr;
	messages = backtrace_symbols(trace, trace_size);
	/* skip first stack frame (points here) */
	printf("[bt] Execution path:\n");
	for (i = 1; i < trace_size; ++i)
	{
		printf("[bt] #%d %s\n", i, messages[i]);

		/* find first occurence of '(' or ' ' in message[i] and assume
		 * everything before that is the file name. (Don't go beyond 0 though
		 * (string terminator)*/
		size_t p = 0;
		while (messages[i][p] != '(' && messages[i][p] != ' '
			&& messages[i][p] != 0)
			++p;

		char syscom[256];
		sprintf(syscom, "Line %p -e %.*s", trace[i], p, messages[i]);
		//last parameter is the file name of the symbol
		system(syscom);
	}

	//throw(signalNum);
	//exit(signal);
}

GUID _StringToGuid(const std::string& str)
{
	GUID guid;
	sscanf(str.c_str(),
		"%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx",
		&guid.Data1, &guid.Data2, &guid.Data3,
		&guid.Data4[0], &guid.Data4[1], &guid.Data4[2], &guid.Data4[3],
		&guid.Data4[4], &guid.Data4[5], &guid.Data4[6], &guid.Data4[7]);

	return guid;
}

std::string _GuidToString(GUID* guid)
{
	// "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}"
	// "{%8x-%4hx-%4hx-%2hhx%2hhx-%2hhx%2hhx%2hhx%2hhx%2hhx%2hhx}"
	// "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}"
	char guid_cstr[39];
	snprintf(guid_cstr, sizeof(guid_cstr),
		"%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		guid->Data1, guid->Data2, guid->Data3,
		guid->Data4[0], guid->Data4[1], guid->Data4[2], guid->Data4[3],
		guid->Data4[4], guid->Data4[5], guid->Data4[6], guid->Data4[7]);

	return std::string(guid_cstr);
}

//const GUID _StringToGUID(const char* szGUID) {
//	GUID g;
//	assert(StringToGUID(szGUID, &g));
//	return g;
//}

void* LoadRank(void* ptr)
//void LoadRank()
{
//#if(USE_RECHECK_AUTH_TRAP > 0)
//	if (gAuthenticate.ReconnectCheck())
//	{
//		if (USE_RELOAD_RANK)
//		{
//			//printf("LoadRank start\n");
//			int nRank = 1;
//			fstream newfile;
//			newfile.open("worldrank/toplist.txt", ios::in);
//			if (newfile.is_open()) {
//				std::string tp;
//				while (std::getline(newfile, tp)) {
//					std::string delimiter = "	";
//					std::vector<std::string> results = gUtil.split(tp, delimiter);
//					//std::vector<std::string> results;
//					boost::algorithm::split(results, tp, boost::is_any_of(delimiter));
//					//cout << nRank << " - " << results[0] << "\n";
//					gKPlayerNew.WorldRank_Data.insert(pair<std::string, int>(results[0], nRank));
//					nRank++;
//					if (nRank > 1000)
//						break;
//					std::this_thread::sleep_for(std::chrono::milliseconds(30));
//				}
//			}
//			newfile.close();
//			printf("\n[FWCLD] Update Rank OK\n");
//		}
//	}
//#else
//	if (USE_RELOAD_RANK)
//	{
	//printf("LoadRank start\n");
	
	int nRank = 1;
	if (gUtil.nTopListType == 2)
		nRank = 0;
	int nBoQua = 0;
	fstream newfile;

	while (true) {
		time_t now = time(nullptr);
		//printf("Time rank %d\n", (now >= gUtil.TimeUpdateRank));
		if (now >= gUtil.TimeUpdateRank)
		{
			TRoleList* Rank = Execute_GetTenRoleListPlus((KLadder*)((char*)0x82BCE40), 10270);

			for (int i = 0; i < 10; i++)
			{
				gKPlayerNew.WorldRank_Data.insert(pair<std::string, int>(Rank[i].Name, (i + 1)));
			}

			if (gUtil.WORLDRANK_ACTIVE == 1)
			{
				newfile.open(gUtil.nFileTopList, ios::in);
				if (newfile.is_open()) {
					std::string tp;
					while (std::getline(newfile, tp)) {
						if (nBoQua < 10)
						{
							nBoQua++;
							continue;
						}

						if (nRank == 0)
						{
							nRank++;
							continue;
						}

						std::string delimiter = "	";
						std::vector<std::string> results = gUtil.split(tp, delimiter);
						//std::vector<std::string> results;
						boost::algorithm::split(results, tp, boost::is_any_of(delimiter));
						//cout << nRank << " - " << results[0] << "\n";
						gKPlayerNew.WorldRank_Data.insert(pair<std::string, int>(results[0], nRank));
						nRank++;
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
						if (nRank > 1000)
						{
							size_t Minutes = gUtil.nTimeUpdateRank;
							time_t newTime = now + (60 * Minutes);
							//time_t newTime = now + 10;
							gUtil.TimeUpdateRank = newTime;
							nRank = 1;
							if (gUtil.nTopListType == 2)
								nRank = 0;
							nBoQua = 0;
							//printf("\n[JXGUARD] Update Rank OK\n");
						}
					}
				}
				newfile.close();
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 60));
		}
	}
		
		//newfile.open("worldrank/toplist.txt", ios::in);
		
		//printf("\n[JXGUARD] Update Rank OK\n");
//	}
//#endif
	pthread_exit(NULL);
}

void _trace(char* fmt, ...)
{
	char string[1024] = { 0 };
	char out[1024] = { 0 };

	va_list body;
	va_start(body, fmt);

	vsprintf(string, fmt, body);
	va_end(body);

	if (strlen(string))
	{
		sprintf(out, "[TRACE]: %s", string);
		//OutputDebugString(out);
	}
}

//vector<string> split(string s, string delimiter) {
//	size_t pos_start = 0, pos_end, delim_len = delimiter.length();
//	string token;
//	vector<string> res;
//
//	while ((pos_end = s.find(delimiter, pos_start)) != string::npos) {
//		token = s.substr(pos_start, pos_end - pos_start);
//		pos_start = pos_end + delim_len;
//		res.push_back(token);
//	}
//
//	res.push_back(s.substr(pos_start));
//	return res;
//}

void KPlayerNew::HookAll() {

	size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x8092804;
	uintptr_t start = (uintptr_t)(void*)0x8092804;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	//gUtil.SetCompleteHook(0xE8, 0x80BF2A0, &GetEngergyDamageHook); // loi nBuyIdx
	//gUtil.SetCompleteHook(0xE9, 0x8092804, &GetEngergyDamageHook); // loi nBuyIdx

	gUtil.SetCompleteHook(0xE9, start, &GetEngergyDamageHook); // loi nBuyIdx

}

void KPlayerNew::HookMoc() {

	size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x8092804;
	uintptr_t start = (uintptr_t)(void*)0x8092AF6;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	//gUtil.SetCompleteHook(0xE8, 0x80BF2A0, &GetEngergyDamageHook); // loi nBuyIdx
	//gUtil.SetCompleteHook(0xE9, 0x8092804, &GetEngergyDamageHook); // loi nBuyIdx

	gUtil.SetCompleteHook(0xE8, start, &SetEngergyDamageAgi); // loi nBuyIdx

}

inline std::string format(const char* fmt, ...) {
	int size = 512;
	char* buffer = 0;
	buffer = new char[size];
	va_list vl;
	va_start(vl, fmt);
	int nsize = vsnprintf(buffer, size, fmt, vl);
	if (size <= nsize) { //fail delete buffer and try again
		delete[] buffer;
		buffer = 0;
		buffer = new char[nsize + 1]; //+1 for /0
		nsize = vsnprintf(buffer, size, fmt, vl);
	}
	std::string ret(buffer);
	va_end(vl);
	delete[] buffer;
	return ret;
}

void KPlayerNew::HookFixAll()
{
	/*struct sigaction sa;
	struct sigaction old_action;

	memset(&sa, 0, sizeof(sa));
	memset(&old_action, 0, sizeof(old_action));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigaction_player;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);*/

	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start;
	uintptr_t pagestart;

	start = (uintptr_t)(void*)0x81116E8;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeLoadMap); // Set Battle map Nice

	//0804C54B	0x2	00 00 	58 02
	//082490A3	0x1	72 	73
	gUtil.SetWord(0x804C54B, 0x258);
	gUtil.SetByte(0x82490A3, 0x73);
	//start = (uintptr_t)(void*)0x81F7067;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeGroupChat); // Fix chat room

	 //start = (uintptr_t)(void*)0x80A3190;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeModifyAttrib); // Fix Toc Do Danh MAX
	 
	 
	//start = (uintptr_t)(void*)0x80931B8;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeModifyAttrib); // Fix Toc Do Danh MAX


	//start = (uintptr_t)(void*)0x807AF46;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeDeactivateEquip); // Fix Bug Luc tay
	 
	 
	//start = (uintptr_t)(void*)0x80997E0;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeSyncPlayer); // Fix Toc Do Danh MAX
	
	//===========================================\\

	//start = (uintptr_t)(void*)0x807AA55;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeGetEquipEnhance); // Fix Bug Luc tay

	//===========================================\\

	//start = (uintptr_t)(void*)0x807AC12;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeSetNpcDamageAttrib); // Fix Bug Luc tay

	/*start = (uintptr_t)(void*)0x80FE08D;
	pagestart = start & -pagesize;
	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xFF, start, &SkillExpOnPlayer);*/

	//gUtil.SetJmp(start, SkillExpOnPlayer);

	//size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x8122BE8; // set rank
	//uintptr_t pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}
	//gUtil.SetCompleteHook(0xE8, start, &FixGetIP); // loi nBuyIdx
	// 
	 
	//start = (uintptr_t)(void*)0x80D8A0C;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}
	//gUtil.SetCompleteHook(0xE8, start, &SafeSavePlayerBaseInfo);
}

bool __cdecl SafeLoadMap(KSubWorld* This, int nId)
{
	if (nId == 381 || nId == 382 || nId == 383 || nId == 378 || nId == 379 || nId == 380 || nId == 900)
	{
		setpriority(PRIO_PROCESS, 0, -19);
	}
	return Execute_LoadMap(This, nId);
}

bool __cdecl SafeGroupChat(KServerCore* This, BYTE tgtcls, DWORD tgtid, const void* pData, size_t size)
{
	//void* v6 = *((void*)0x82B0F60 + 2 * tgtcls);
	/*memset(&sig_action, 0, sizeof(sig_action));
	memset(&sig_old_action, 0, sizeof(sig_old_action));
	sigemptyset(&sig_action.sa_mask);
	sig_action.sa_sigaction = sigaction_player;
	sig_action.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sig_action, &sig_old_action);*/

	KIniFile FileHandle;
	Execute_KIniFile(&FileHandle);
	Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
	Execute_KIniFile_GetInteger(&FileHandle, "Setting", "OpenGroupChat", 1, &gUtil.nGroupChat);
	Execute_KIniFile_Clear(&FileHandle);

	try
	{
		if (gUtil.nGroupChat == 1)
		{
			if (*((DWORD*)(void*)0x82B0F60 + 2 * tgtcls) == NULL)
				return 0;
			else if (*((DWORD*)(void*)0x82B0F60 + 2 * tgtcls) == 0)
				return 0;

			return Execute_GroupChat(This, tgtcls, tgtid, pData, size);
		}
		else
			return 0;
	}
	catch (std::exception& e) {
		return 0;
	}
}

//
//void __cdecl SafeActivate(KNpcAI* This, int nIndex)
//{
//	int nSkillID = 0;
//	KIniFile FileHandle1;
//	Execute_KIniFile(&FileHandle1);
//	Execute_KIniFile_Load(&FileHandle1, "servercfg.ini");
//	Execute_KIniFile_GetInteger(&FileHandle1, "Setting", "SkillID", 0, &nSkillID);
//	Execute_KIniFile_Clear(&FileHandle1);
//
//	printf("nSkillID : %d\n", nSkillID);
//
//	if (nSkillID > 0)
//	{
//		bool bIsNoClearOnDeath;
//		int nEndTime = 0;
//		int nStateType = 0;
//		int nSkillLevel = 0;
//		Execute_GetStateSkillEffect_Npc((KNpc *)((char *)0x8320A00 + 6436 * nIndex), nSkillID, &nSkillLevel, &nStateType, &nEndTime, &bIsNoClearOnDeath);
//		printf("nSkillLevel : %d\n", nSkillLevel);
//		printf("nStateType : %d\n", nStateType);
//		printf("nEndTime : %d\n", nEndTime);
//		printf("bIsNoClearOnDeath : %d\n", bIsNoClearOnDeath);
//		Execute_Activate_Npc(This, nIndex);
//	}
//	else
//		Execute_Activate_Npc(This, nIndex);
//}
//
//int __cdecl SafeSavePlayerBaseInfo(KPlayer* This, BYTE* pRoleBuffer, BOOL bQuitSave)
//{
//	int NewValue = 10;
//	size_t pagesize = sysconf(_SC_PAGESIZE);
//	uintptr_t start = (uintptr_t)&This->tmp[0x28004];
//	uintptr_t end = start + sizeof NewValue;
//
//	uintptr_t pagestart = start & -pagesize;
//
//	if (mprotect((void*)pagestart, end - pagestart,
//		PROT_READ | PROT_WRITE | PROT_EXEC))
//	{
//		perror("mprotect");
//		//exit(EXIT_FAILURE);
//	}
//
//	memcpy((void*)start, &NewValue, sizeof NewValue);
//
//	start = (uintptr_t)&This->tmp[0x28C] + 293;
//	end = start + sizeof NewValue;
//
//	pagestart = start & -pagesize;
//
//	if (mprotect((void*)pagestart, end - pagestart,
//		PROT_READ | PROT_WRITE | PROT_EXEC))
//	{
//		perror("mprotect");
//		//exit(EXIT_FAILURE);
//	}
//
//	memcpy((void*)start, &NewValue, sizeof NewValue);
//
//	return Execute_SavePlayerBaseInfo(This, pRoleBuffer, bQuitSave);
//}
//void __cdecl SafeModifyAttrib(KNpc* This, int nAttacker, KMagicAttrib* pData, BOOL bIsEndProcess)
//{
//	//printf("ModifyAttrib : %d , %d, %d - %d, %d, %d\n", This->tmp[0x0], This->tmp[0x17E0], pData->nAttribType, pData->nValue[0], pData->nValue[1], pData->nValue[2]);
//	//gUtil.SetByte((DWORD)&pData->nValue[0], 255);
//	Execute_ModifyAttrib(This, nAttacker, pData, bIsEndProcess);
//	//Execute_RemoveStateSkillEffect(This, nSkillID, true);
//	//Execute_SyncSkillFailed(This, 109);
//}
//
//void __cdecl SafeSetNpcDamageAttrib(KPlayer* This)
//{
//	int nMaxDamage;
//	int nMinDamage[3];
//	int nStrength;
//	int nDexterity;
//	int m_nCurStrength;
//	int m_nCurDexterity;
//	int m_nCurDamage;
//	KNpc* m_CurNpc;
//
//	//Execute_GetWeaponDamage((KItemList*)&This->tmp[0x2040C], nMinDamage, &nMaxDamage);
//	int result = Execute_GetWeaponType((KItemList*)&This->tmp[0x2040C]);
//	/*if (!Player)
//		Player = (KPlayer*)Player_Struct;*/
//	m_CurNpc = Execute_GetMyNpc(This);
//	m_nCurDamage = 0;
//	//unsigned char* variableValue1 = (unsigned char*)This->tmp[0x25C5C];
//	//m_nCurStrength = *variableValue1;
//	m_nCurStrength = *(int*)((char*)&This->tmp[0x25C5C]);
//	m_nCurDexterity = *(int*)((char*)&This->tmp[0x25C60]);
//	int nCurStrength = m_nCurStrength / 5 + 1;
//	printf("Series : %d\n", m_CurNpc->tmp[0x28]);
//	printf("GetWeaponType : %d\n", result);
//	printf("CurDamage : %d\n", nCurStrength);
//	printf("CurStrength : %d\n", m_nCurStrength);
//	printf("CurDexterity : %d\n", m_nCurDexterity);
//	nMaxDamage = nCurStrength;
//	*nMinDamage = nCurStrength;
//	if (m_CurNpc->tmp[0x28] == 0)
//	{
//		printf("MinDamage : %d\n", nMinDamage[0]);
//		printf("MaxDamage : %d\n", nMaxDamage);
//		Execute_SetPhysicsDamage(Execute_GetMyNpc(This), nMinDamage[0], nMaxDamage);
//	}
//	else
//	{
//		Execute_SetNpcDamageAttrib(This);
//	}
//
//	/*if (result == 0)
//	{
//		m_nCurDamage = m_nCurStrength;
//		nMaxDamage += m_nCurDamage / 5;
//		nMinDamage[0] += m_nCurDamage / 5;
//		printf("MinDamage : %d\n", nMinDamage[0]);
//		printf("MaxDamage : %d\n", nMaxDamage);
//		Execute_SetPhysicsDamage(Execute_GetMyNpc(This), nMinDamage[0], nMaxDamage);
//	}
//	else if (result == 1)
//	{
//		m_nCurDamage = m_nCurDexterity;
//		nMaxDamage += m_nCurDamage / 5;
//		nMinDamage[0] += m_nCurDamage / 5;
//		printf("MinDamage : %d\n", nMinDamage[0]);
//		printf("MaxDamage : %d\n", nMaxDamage);
//		Execute_SetPhysicsDamage(Execute_GetMyNpc(This), nMinDamage[0], nMaxDamage);
//	}
//	else
//		Execute_SetNpcDamageAttrib(This);*/
//
//	
//	//printf("CurDamage : %d\n", nCurStrength);
//	//printf("PlayerIndex : %d\n", This->tmp[0xC0]);
//	//Execute_UpdataCurData(This, 0);
//	
//	//Execute_SetNpcDamageAttrib(This);
//	//Execute_SendSyncCurPlayer(This, *(int*)(&This->tmp[0xC0]));
//	//Execute_SetHide(This, 0);
//	//Execute_RemoveStateSkillEffect(This, 713, false);
//	//Execute_ClearHideState(This);
//	//Execute_UpdateNpcStateInfo(This);
//	//Execute_ReCalcStateEffect(This);
//	//Execute_Npc_SendSyncData(This, 0);
//	//Execute_NormalSync(This, 0);
//	//Execute_DoMagic(This);
//}
//
//void __cdecl SafeDeactivateEquip(KItemList* This, int nIdx, int nEquipPlace, std::vector<KITEM_CONSUME>* pItems)
//{
//	try
//	{
//		printf("nIdx : %d\n", nIdx);
//		printf("EquipPlace : %d\n", nEquipPlace);
//		printf("GoldEquipSuiteID : %d\n", This->tmp[0x4C1C]);
//		printf("CMP EquipPlace : %d\n", (unsigned int)(nEquipPlace - 10));
//		//0x82BBD40
//		if (nEquipPlace == 3)
//		{
//			KNpc* m_CurNpc;
//			KPlayer* m_CurPlayer = (KPlayer*)((char*)Player_Struct + 165956 * This->tmp[0x0]);
//			m_CurNpc = Execute_GetMyNpc(m_CurPlayer);
//			printf("Series : %d\n", m_CurNpc->tmp[0x28]);
//			if (m_CurNpc->tmp[0x28] == 0)
//			{
//				int m_nCurStrength;
//				int m_nCurDexterity;
//				
//				m_nCurStrength = *(int*)((char*)&m_CurPlayer->tmp[0x25C5C]);
//				m_nCurDexterity = *(int*)((char*)&m_CurPlayer->tmp[0x25C60]);
//
//				printf("CurStrength : %d\n", m_nCurStrength);
//				printf("CurDexterity : %d\n", m_nCurDexterity);
//
//				//KItem* m_CurItem;
//				//m_CurItem = (KItem*)((char*)0x82BBD40)[nIdx];
//				//Execute_UnEquipMagicForSuite(This, m_CurNpc, nEquipPlace, nIdx);
//				//gUtil.SetByte((DWORD)&This->tmp[0x4C1C], 0);
// 				gUtil.SetDword((DWORD)&m_CurPlayer->tmp[0x25C5C], m_nCurStrength / 5 + 1);
// 				gUtil.SetDword((DWORD)&m_CurPlayer->tmp[0x25C60], m_nCurStrength / 5 + 1);
//				//printf("GoldEquipSuiteID : %d\n", This->tmp[0x4C1C]);
//				Execute_DeactivateEquip(This, nIdx, nEquipPlace, pItems);
//			}
//		}
//	}
//	catch (...)
//	{
//		Execute_DeactivateEquip(This, nIdx, nEquipPlace, pItems);
//	}
//}
//
//int __cdecl SafeGetEquipEnhance(KItemList* This, int nPlace, BOOL bIgnoreSuite)
//{
//	try
//	{
//		int result = 0;
//		result = Execute_GetEquipEnhance(This, nPlace, bIgnoreSuite);
//		printf("nPlace : %d\n", nPlace);
//		printf("result : %d\n", result);
//		return result;
//	}
//	catch (...)
//	{
//		return Execute_GetEquipEnhance(This, nPlace, bIgnoreSuite);
//	}
//}

bool __cdecl SafeSyncPlayer(KNpc* This, KPlayer* pReceiver)
{
	try
	{
		/*size_t pagesize = sysconf(_SC_PAGESIZE);
		uintptr_t start;
		uintptr_t end;
		uintptr_t pagestart;
		int NewValue = 0;*/

		//printf("HideMode : %d\n", This->tmp[0x1878]);
		//printf("SyncPlayer 1 : %d\n", This->tmp[0x1910]);
		//printf("SyncPlayer 2 : %d\n", This->tmp[0x190C]);
		//printf("SyncPlayer 3 : %d\n", This->tmp[0x1914]);
		//printf("SyncPlayer 4 : %d\n", This->tmp[0x1918]);
		
		if (This->tmp[0x1878] == 1)
		{
			//Execute_RemoveStateSkillEffect(This, 496, true);
			//Execute_RemoveStateSkillEffect(This, 713, true);
			
			//Execute_ClearHideState(This);
			mprotect((void*)This->tmp[0x1878], 0x6u, 7);
			This->tmp[0x1878] = -1;
			//Execute_RemoveStateSkillEffect(This, 713, true);
			Execute_UpdateNpcStateInfo(This);
			Execute_ReCalcStateEffect(This);
			Execute_SetHide(This, 1);
			//Execute_RemoveStateSkillEffect(This, nSkillID, bSyncClient);
			//return Execute_SyncPlayer(This, Execute_GetMyPlayer(This));
			return true;
		}
		else
			return Execute_SyncPlayer(This, Execute_GetMyPlayer(This));

		//if (This->tmp[0x1910] < 0)
		//{
		//	//Execute_SyncSkillFailed(This, 109);
		//	int CurrenSpeed = This->tmp[0x1910] + 256;
		//	if (CurrenSpeed < 120)
		//		NewValue = CurrenSpeed;
		//	else
		//		NewValue = 120;
		//	start = (uintptr_t)&This->tmp[0x1910];
		//	end = start + sizeof NewValue;
		//	pagestart = start & -pagesize;

		//	if (mprotect((void*)pagestart, end - pagestart,
		//		PROT_READ | PROT_WRITE | PROT_EXEC))
		//	{
		//		perror("mprotect");
		//	}

		//	memcpy((void*)start, &NewValue, sizeof NewValue);
		//}

		//if (This->tmp[0x190C] < 0)
		//{
		//	int CurrenSpeed = This->tmp[0x190C] + 256;
		//	if (CurrenSpeed < 250)
		//		NewValue = CurrenSpeed;
		//	else
		//		NewValue = -6;
		//	start = (uintptr_t)&This->tmp[0x190C];
		//	end = start + sizeof NewValue;
		//	pagestart = start & -pagesize;

		//	if (mprotect((void*)pagestart, end - pagestart,
		//		PROT_READ | PROT_WRITE | PROT_EXEC))
		//	{
		//		perror("mprotect");
		//	}

		//	memcpy((void*)start, &NewValue, sizeof NewValue);
		//}

		//if (This->tmp[0x1914] < 0)
		//{
		//	int CurrenSpeed = This->tmp[0x1914] + 256;
		//	if (CurrenSpeed < 250)
		//		NewValue = CurrenSpeed;
		//	else
		//		NewValue = -6;
		//	start = (uintptr_t)&This->tmp[0x1914];
		//	end = start + sizeof NewValue;
		//	pagestart = start & -pagesize;

		//	if (mprotect((void*)pagestart, end - pagestart,
		//		PROT_READ | PROT_WRITE | PROT_EXEC))
		//	{
		//		perror("mprotect");
		//	}

		//	memcpy((void*)start, &NewValue, sizeof NewValue);
		//}

		//if (This->tmp[0x1918] < 0)
		//{
		//	//Execute_SyncSkillFailed(This, 109);
		//	int CurrenSpeed = This->tmp[0x1918] + 256;
		//	if (CurrenSpeed < 120)
		//		NewValue = CurrenSpeed;
		//	else
		//		NewValue = 120;
		//	start = (uintptr_t)&This->tmp[0x1918];
		//	end = start + sizeof NewValue;
		//	pagestart = start & -pagesize;

		//	if (mprotect((void*)pagestart, end - pagestart,
		//		PROT_READ | PROT_WRITE | PROT_EXEC))
		//	{
		//		perror("mprotect");
		//	}

		//	memcpy((void*)start, &NewValue, sizeof NewValue);
		//}

		return Execute_SyncPlayer(This, pReceiver);
	}
	catch (...)
	{
		return Execute_SyncPlayer(This, pReceiver);
	}
}

bool isNumber(const string& str)
{
	return !str.empty() && find_if(str.begin(), str.end(),
		[](char c) { return !isdigit(c); }) == str.end();
}

bool validateIP(string ip)
{
	std::string delimiter = ".";

	vector<string> list = gUtil.split(ip, delimiter);

	// if the token size is not equal to four
	if (list.size() != 4) {
		return false;
	}

	// validate each token
	for (string str : list)
	{
		// verify that the string is a number or not, and the numbers
		// are in the valid range
		if (!isNumber(str) || stoi(str) > 255 || stoi(str) < 0) {
			return false;
		}
	}

	return true;
}

void FixGetIP(char* dest, const char* src)
{
	//std::cout << src << std::endl;
	std::string delimiter = ":";
	std::string s = "null";
	s = src;

	if (s.find(':') != std::string::npos)
	{
		vector<string> list = gUtil.split(src, delimiter);
		if (list.size() != 2)
			memcpy(dest, "127.0.0.1 : 1", strlen("127.0.0.1 : 1"));
		else
			memcpy(dest, src, strlen(src));
	}
	else
	{
		memcpy(dest, "127.0.0.1 : 1", strlen("127.0.0.1 : 1"));
	}

	
	//if (gUtil.isValidIpAddress((char*)src))
	/*if (validateIP(gUtil.split(src, delimiter)[0]))
		strcpy(dest, src);
	else
		strcpy(dest, "127.0.0.1");*/
}

void KPlayerNew::HookRank()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x80D8E7F;
	// 
	//uintptr_t start = (uintptr_t)(void*)0x80C4CFB; // set rank

	uintptr_t start = (uintptr_t)(void*)0x80C4D66; // set rank

	//uintptr_t start = (uintptr_t)(void*)0x80BF670;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}
	//*(BYTE*)(0x80CAB80) = -4;
	gUtil.SetCompleteHook(0xE8, start, &SetRankHook); // loi nBuyIdx
	
	pthread_t thread_id;

	if (gUtil.nTimeUpdateRank > 0)
		pthread_create(&thread_id, NULL, &LoadRank, NULL);

	//memcpy((void*)start, , );
	//memset((void*)start, *(char*)"B80100000090", 6); // set rank
	//memset((void*)start, *(char*)hexRank.c_str(), 6);
	//memcpy((void*)0x80D8E80, &NewValue, sizeof NewValue);
	//(void*)start
}

int __cdecl SafeGetPlayerIdx(KNpc* This)
{
	//signal(11, signal_handler);

	if (Execute_KNpcGetPlayerIdx(This))
		return 0;
}

int __cdecl SkillExpOnPlayer()
{
	//signal(11, signal_handler);
	return 0;
}

void HookSkillExp()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);

	uintptr_t start = (uintptr_t)(void*)0x80FE08D;

	uintptr_t pagestart = start & -pagesize;

	/*if (mprotect((void*)pagestart, 0x5u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xFF, start, &SkillExpOnPlayer);*/
	
	start = (uintptr_t)(void*)0x80FE053;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeGetPlayerIdx);
}

void KPlayerNew::HookCustomScriptLua()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x80D8E7F;
	// 
	//uintptr_t start = (uintptr_t)(void*)0x80C4CFB; // set rank

	//uintptr_t start = (uintptr_t)(void*)0x81F6001; // KServerCore::AttachPlayer

	uintptr_t start = (uintptr_t)(void*)0x80670D8; // set rank

	//uintptr_t start = (uintptr_t)(void*)0x80BF670;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}
	//*(BYTE*)(0x80CAB80) = -4;
	gUtil.SetCompleteHook(0xE8, start, &SetCustomScriptLua); // loi nBuyIdx

	//start = (uintptr_t)(void*)0x821BA65; // set rank

	////uintptr_t start = (uintptr_t)(void*)0x80BF670;
	////uintptr_t end = start + sizeof(GetEngergyDamageHook);

	////  Calculate start of page for mprotect.
	//pagestart = start & -pagesize;

	////  Change memory protection.
	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}
	////*(BYTE*)(0x80CAB80) = -4;
	//gUtil.SetCompleteHook(0xE8, start, &SetCustomScriptLua); // loi nBuyIdx

}


void KPlayerNew::Hook_CheckProtocolSize()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start;
	uintptr_t pagestart;

	start = (uintptr_t)(void*)0x804DE37; // 0x804DE37 KClientProcess::ProcessMessage
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &ProcessMessage_Hook);

	for (int i = 0; i < 1200; i++)
	{
		m_ClientInfo[i].m_NetConnectIdx = -1;
		memset(m_ClientInfo[i].m_TextGuid, 0, 33);
		memset(m_ClientInfo[i].m_Guid, 0, 16);
		m_ClientInfo[i].m_TickCount = 0L;
		m_ClientInfo[i].m_Flood = 0;
		m_ClientInfo[i].m_Checked = false;
	}

	if (gUtil.nDdos > 0)
	{
		g_Firewall.m_ListIPAddress.clear();

		if (gUtil.nDdos == 1)
		{
			printf("ANTI DDOS: ACTIVE\n");
		}
		else
		{
			printf("DETECT DDOS: ACTIVE\n");
		}
	}

	
	

	//start = (uintptr_t)(void*)0x8262B3B;
	//pagestart = start & -pagesize;

	////  Change memory protection.
	//if (mprotect((void*)pagestart, 0x100u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//}

	//strncpy((char*)start, (char*)"Game limited: %lu\x0A", 19);

	//size_t pagesize = sysconf(_SC_PAGESIZE);
	//start = (uintptr_t)(void*)0x81F7538; // 0x804DE37 KClientProcess::ProcessMessage
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &CheckProtocolSize_Hook);
	//gUtil.SetCustomHook(0xE8, 0x81F7538, &CheckProtocolSize_Hook);
	printf("===========================================\n");
}

bool __cdecl CheckProtocolSize_Hook(KServerCore* This, int nPlayerIndex, const char* pChar, int nSize, BOOL* const bShutDown)
{
	try
	{
		if (Execute_IsSpectator((CSpectatorMap*)0x9472420, nPlayerIndex))
		{
			*bShutDown = 1;
			return 0;
		}
		else
		{
			char szAccount_0[32];
			//Execute_GetGameData(This, 3, (unsigned int)szAccount_0, nPlayerIndex);
			if (*(BYTE*)(unsigned int)szAccount_0 == 0)
			{
				*bShutDown = 1;
				return 0;
			}
			else
			{
				if (strlen(szAccount_0) > 4)
				{
					if (Sqlite_Check_HardwareId_On_EnterGame((char*)szAccount_0))
					{

					}
				}
				else
				{
					*bShutDown = 1;
					return 0;
				}
			}
		}
		//assert(pData && dataLength);

		//const char* pBuffer = pData;

		/*if (dataLength != 17 || *pBuffer != 66)
		{
			return 0;
		}
		else
		{*/

		//WORD wCheckSize;
		//BYTE protocoltype = *(BYTE*)pChar;
		BYTE nProtocol = (BYTE)pChar[0];
		//tagLogicLoginBew* pLL;

		//std::cout << "nProtocol " << nProtocol << "\n";
		//std::cout << "nSize " << nSize << "\n";
		// 
		//char chartext[100];

		//sprintf((char*)chartext, (const char*)"[Info] nProtocol < %s >, nSize %s \n", nProtocol, nSize);

		//printf("[Info] nProtocol < %d >, nSize %d \n", nProtocol, nSize);

		//return Execute_CheckProtocolSize(This, nPlayerIndex, pChar, nSize, bShutDown);
		//return nCheckProtocolSize(This, nPlayerIndex, pChar, nSize, bShutDown);
		//if (nProtocol == 68 && nSize == 18)
		//{
		//	////std::cout << "ok 66\n";
		//	////if (dataLength == 18)
		//	////{
		//	//	//dataLength = 17;
		//	//	//nProcessMessage(This, lnID, pData, 17);
		//	////	//std::cout << "ok\n";
		//	////}
		//	////std::cout << "no\n";
		//	////else
		//	////{
		//	////	break;
		//	////}


		//	//////const char* pChar = NULL;
		//	//////dataLength = 17;
		//	////char* guid;
		//	//////char* IpAddress;
		//	//////char MacAddress[512];
		//	////////char* HardwareId;
		//	////unsigned char mac[6] = { '\0' };

		//	//////memcpy(&pChar, pData, dataLength);

		//	//tagLogicLoginBew* pLL = (tagLogicLoginBew*)pChar;

		//	////tagLogicLogin ll;
		//	////ll.cProtocol = 66;
		//	//////
		//	////////memcmp(&ll.guid, &pLL->_guid, sizeof(GUID));
		//	////memcpy(&ll.guid, &pLL->_guid, sizeof(GUID));

		//	////guid_to_str(&pLL->guid, guid);



		//	//printf("CheckProtocolSize Guid = {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}\n",
		//	//	&pLL->guid.Data1, &pLL->guid.Data2, &pLL->guid.Data3,
		//	//	&pLL->guid.Data4[0], &pLL->guid.Data4[1], &pLL->guid.Data4[2], &pLL->guid.Data4[3],
		//	//	&pLL->guid.Data4[4], &pLL->guid.Data4[5], &pLL->guid.Data4[6], &pLL->guid.Data4[7]);

		//	///*char MacAddress[18];
		//	//memcpy(MacAddress, pLL->guid.Data5, 18);

		//	//printf("MacAddress = %s\n\n", MacAddress);

		//	//std::string newguid;
		//	//char str_guid[39] = {};
		//	//sprintf(str_guid, "{%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}",
		//	//	&pLL->guid.Data1, &pLL->guid.Data2, &pLL->guid.Data3,
		//	//	&pLL->guid.Data4[0], &pLL->guid.Data4[1], &pLL->guid.Data4[2], &pLL->guid.Data4[3],
		//	//	&pLL->guid.Data4[4], &pLL->guid.Data4[5], &pLL->guid.Data4[6], &pLL->guid.Data4[7]);

		//	//GUID guid;
		//	//guid = _StringToGuid(str_guid);

		//	//newguid = _GuidToString(guid);
		//	//printf("newguid2 = %s\n", newguid.c_str());*/

		//	////printf("Guid = {%02X:%02X:%02X:%02X:%02X:%02X}\n",
		//	////	&pLL->guid.Data5[0], &pLL->guid.Data5[1], &pLL->guid.Data5[2], &pLL->guid.Data5[3],
		//	////	&pLL->guid.Data5[4], &pLL->guid.Data5[5], &pLL->guid.Data5[6]);
		//	////std::cout << "\n";
		//	////std::cout << guid << "\n";


		//	////strcpy((char*)reg.Name, (char*)eg2.szCharacterName);

		//	////memcpy(&pLL, &pData, sizeof(tagLogicLoginBew));

		//	////memcpy(guid, &pLL->_guid.Data4, sizeof(&pLL->_guid.Data4));
		//	////memcpy(mac, &pLL->_guid.Data5, sizeof(pLL->_guid.Data5));
		//	////strncpy(mac, (char*)&pLL->_guid.Data5, sizeof(mac));
		//	////strncpy(MacAddress, (char*)&pLL->_guid.Data6, sizeof(IpAddress));
		//	////memcpy(MacAddress, &pLL->_guid.Data6, sizeof(IpAddress));
		//	////memcpy(HardwareId, &pLL->_guid.Data7, sizeof(&pLL->_guid.Data7));

		//	////char MacAddresses[18];

		//	////sprintf(MacAddresses, "%02X:%02X:%02X:%02X:%02X:%02X", MacAddress[0], MacAddress[1], MacAddress[2], MacAddress[3], MacAddress[4], MacAddress[5]);

		//	////sprintf(IpAddress, "%.2hhX", &pLL->_guid.Data5);
		//	////sprintf(MacAddress, "%.2hhX", &pLL->_guid.Data6);
		//	////sprintf(HardwareId, "%.2hhX", &pLL->_guid.Data7);
		//	////sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		//	////std::cout << guid << "\n";
		//	////std::cout << IpAddress << "\n";
		//	////std::cout << MacAddress << "\n";
		//	////std::cout << HardwareId << "\n";

		//	////dataLength = 17;

		//	////nCheckProtocolSize(This, lnID, (const char*)&ll, dataLength);
		//	////return nCheckProtocolSize(This, nPlayerIndex, pChar, nSize, bShutDown);
		//	return true;

		//}
		//else
		//{
		//	//std::cout << "out ok\n";
		//	return nCheckProtocolSize(This, nPlayerIndex, pChar, nSize, bShutDown);
		//}


		//int nIndex = nProcessLoginProtocol(This, lnID, pData, dataLength);

		//std::cout << nIndex << std::endl;

		//return nIndex;
		//tagLogicLogin* pLL = (tagLogicLogin*)pData;
		//std::cout << "A client try to login..." << std::endl;
		//int nIdx = nAttachPlayer((KPlayerSet*)This->tmp[0x8], lnID, &pLL->guid);
		//std::cout << nIdx << std::endl;
		//if (nIdx)
		//{
		//	std::cout << "Found player " << nIdx << " is logging in system!\n" << std::endl;
		//	return nIdx;
		//}
		//else
		//{
		//	std::cout << "Found player\n" << std::endl;
		//	return 0;
		//}
		//}
		//return 0;
	}
	catch (std::exception const& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 0;
	}
}

void KPlayerNew::Hook_AttachPlayer()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x80D8E7F;
	// 
	//uintptr_t start = (uintptr_t)(void*)0x80C4CFB; // set rank

	//uintptr_t start = (uintptr_t)(void*)0x81F6001; // KServerCore::AttachPlayer

	uintptr_t start = (uintptr_t)(void*)0x804EF1B; // KPlayerSet::AttachPlayer 0x804EF1B

	//uintptr_t start = (uintptr_t)(void*)0x80BF670;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}
	//*(BYTE*)(0x80CAB80) = -4;
	gUtil.SetCompleteHook(0xE9, start, &AttachPlayer_Hook); // loi nBuyIdx

}

int __cdecl AttachPlayer_Hook(KPlayerSet* This, unsigned int lnID, GUID* pGuid)
{
	//KPlayerSet PlayerSet 0x88A1EE0
	//int nIndex = ProcessLoginProtocol(nID, pData, nLen);
	/*if (!PlayerSet)
		PlayerSet = (KPlayerSet*)This;*/
	//printf("AttachPlayer Guid = {%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX}\n",
	//	&pGuid->Data1, &pGuid->Data2, &pGuid->Data3,
	//	&pGuid->Data4[0], &pGuid->Data4[1], &pGuid->Data4[2], &pGuid->Data4[3],
	//	&pGuid->Data4[4], &pGuid->Data4[5], &pGuid->Data4[6], &pGuid->Data4[7]);
	//printf("[Info] lnID <%d>, nSize %d\n", lnID, sizeof(pGuid));
	int nIndex = nAttachPlayer(This, lnID, pGuid);
	//KPlayer* Player;
	//KPlayer* cPlayer;
	//if (!Player)
	//	Player = (KPlayer*)Player_Struct;
	//cPlayer = &Player[nIndex];
	//m_KPlayerInfo[nIndex].m_nIndex = nIndex;

	//memcpy((void*)m_KPlayerInfo[nIndex].m_NetIP, "255.255.255.255", sizeof "255.255.255.255");
	//memcpy((void*)m_KPlayerInfo[nIndex].m_LanIP, "192.168.1.255", sizeof "192.168.1.255");
	//memcpy((void*)m_KPlayerInfo[nIndex].m_Mac, "AA:BB:CC:DD:EE:FF", sizeof "AA:BB:CC:DD:EE:FF");

	/*char* NewValue = "255.255.255.255-192.168.1.255-AA:BB:CC:DD:EE:FF";
	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start = (uintptr_t)&cPlayer->tmp[0x28828];
	uintptr_t end = start + sizeof NewValue;

	uintptr_t pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, end - pagestart,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	memcpy((void*)start, &NewValue, sizeof NewValue);*/
	//std::cout << "====================" << std::endl;
	//std::cout << m_KPlayerInfo[nIndex].m_Mac << std::endl;
	/*std::cout << &cPlayer->tmp[0x203E4] << std::endl;
	std::cout << nIndex << std::endl;
	std::cout << "Index: " << &cPlayer->tmp[0x20404] << "\n" << std::endl;
	std::cout << "m_nPlayerIndex: " << &cPlayer->tmp[0xC0] << "\n" << std::endl;
	std::cout << "m_dwEnergy: " << &cPlayer->tmp[0x2876C] << "\n" << std::endl;
	std::cout << "m_PlayerName: " << &cPlayer->tmp[0x25C88] << "\n" << std::endl;
	std::cout << "m_nWorldStat: " << &cPlayer->tmp[0x28004] << "\n" << std::endl;
	std::cout << "m_AccountName: " << cPlayer->tmp[0x264] << "\n" << std::endl;
	std::cout << "m_PlayerName: " << cPlayer->tmp[0x25C88] << "\n" << std::endl;*/
	return nIndex;
}

//void KPlayerNew::Hook_ProcessMessage()
//{
//	size_t pagesize = sysconf(_SC_PAGESIZE);
//	uintptr_t start = (uintptr_t)(void*)0x804DE37; // 0x804DE37 KClientProcess::ProcessMessage
//	uintptr_t pagestart = start & -pagesize;
//
//	if (mprotect((void*)pagestart, 0x2000u,
//		PROT_READ | PROT_WRITE | PROT_EXEC))
//	{
//		perror("mprotect");
//	}
//
//	gUtil.SetCompleteHook(0xE9, start, &ProcessMessage_Hook);
//
//	start = (uintptr_t)(void*)0x80DCA6C;
//	pagestart = start & -pagesize;
//
//	if (mprotect((void*)pagestart, 0x2000u,
//		PROT_READ | PROT_WRITE | PROT_EXEC))
//	{
//		perror("mprotect");
//	}
//
//	gUtil.SetCompleteHook(0xE9, start, &ProcessNetMsg_Hook);
//}
//
//void __cdecl ProcessNetMsg_Hook(KProtocolProcess* This, int nIndex, BYTE* pMsg)
//{
//	std::time_t result = std::time(nullptr);
//	std::string time_res = std::asctime(std::localtime(&result));
//	std::string::size_type i = time_res.find("\n");
//	if (i != std::string::npos)
//		time_res.erase(i, time_res.length());
//
//	//BYTE byProtocol; // [esp+1Bh] [ebp-Dh]
//
//	//byProtocol = *pMsg;
//	//KPlayer* Player;
//	//KPlayer* cPlayer;
//	//if (!Player)
//	//	Player = (KPlayer*)Player_Struct;
//	//cPlayer = &Player[nIndex];
//
//	//std::string s(reinterpret_cast<char*>(pMsg), sizeof(pMsg));
//	//int nPlayerIdx = *((_DWORD*)&cPlayer->tmp[0xC0]);
//
//	//gUtil.WriteLogFile("ProcessNetMsg 1\t-\t[%s]\nIndex:%d, Size:%d - %u, %u, %u, %u\n",
//	//	time_res.c_str(), nIndex, sizeof(pMsg),*((_DWORD*)pMsg + 1), pMsg[3], pMsg[2], pMsg[1]);
//
//	//if (pMsg[4] != NULL || pMsg[4] > 0 || pMsg[5] != NULL || pMsg[5] > 0 || pMsg[6] != NULL || pMsg[6] > 0 || pMsg[7] != NULL || pMsg[7] > 0 || pMsg[8] != NULL || pMsg[8] > 0)
//	//{
//	//	gUtil.WriteLogFile("ProcessNetMsg 2\t-\t[%s]\nIndex:%d, Size:%d - %u, %u, %u, %u, %u, %u, %u, %u, %u - %u, %u, %u, %u, %u, %u, %u, %u\n",
//	//		time_res.c_str(), nIndex, sizeof(pMsg), *((_DWORD*)pMsg + 1), pMsg[0], pMsg[1], pMsg[2], pMsg[3], pMsg[4], pMsg[5], pMsg[6], pMsg[7], pMsg[8], pMsg[9], pMsg[10], pMsg[11], pMsg[12], pMsg[13], pMsg[14], pMsg[15]);
//	//	//return;
//	//}
//
//	//if (nIndex == NULL || nIndex <= 0 || nIndex >= gUtil.nMaxPlayer)
//	//{
//	//	return;
//	//}
//
//	if (nIndex > 0 && nIndex < 1000)
//	{
//
//		//*(_DWORD*)(&Player[nIndex].tmp[0x20408]) = *(_DWORD*)This;
//		//m_pCore->m_pProtocol = This;
//
//		//printf("No data! %s %s\n", m_pCore->m_bySendBuffer, m_pCore->m_bySendBuffer[35]);
//
//		//BYTE		szBuff[64];
//		////S2R_ADD_TAG_REQUEST* pDatatag;
//		////pDatatag = (S2R_ADD_TAG_REQUEST*)&szBuff;
//		////memcpy(&szBuff[0], (char*)&m_KPlayerInfo[nIndex].m_Hwid, 32);
//		////memcpy(&szBuff[33], (char*)&m_KPlayerInfo[nIndex].m_LicenseId, 32);
//		////Execute_OnAddTagRequest(This, nIndex, szBuff);
//		//if ((char*)&m_KPlayerInfo[nIndex].m_Hwid == NULL || (char*)&m_KPlayerInfo[nIndex].m_Hwid == "")
//		//{
//		//	printf("No data!\n");
//		//	Execute_OnGetTagsRequest(m_pCore->m_pProtocol, nIndex, szBuff);
//		//}
//
//		//printf("No data! %s %s\n", m_pCore->m_bySendBuffer, m_pCore->m_bySendBuffer[35]);
//		////strcpy(mHwid, (char*)&m_KPlayerInfo[PlayerIndex].m_Hwid);
//		nKProtocolProcess(This, nIndex, pMsg);
//	}
//	//if (nPlayerIdx <= 0 && nPlayerIdx >= gUtil.nMaxPlayer)
//	//	return;
//}

void __cdecl ProcessMessage_Hook(KClientProcess* This, const unsigned long lnID, const char* pData, size_t dataLength)
{
	try
	{
		//printf("NetConnectIdx: %d, Protocol : %d, GUID : %d, v8 : %d, dataLength : %d\n", lnID, *(BYTE*)pData, (const GUID*)((char*)pData + 1), (unsigned __int8)(*pData - 65), dataLength);

		if (lnID >= 0 && gUtil.nMaxPlayer > lnID)
		{
			/*KIniFile FileHandle;
			Execute_KIniFile(&FileHandle);
			Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
			Execute_KIniFile_GetInteger(&FileHandle, "Ddos", "Active", 0, &gUtil.nDdos);
			Execute_KIniFile_GetInteger(&FileHandle, "Ddos", "Delay", 0, &gUtil.nDdosDelay);
			Execute_KIniFile_GetInteger(&FileHandle, "Ddos", "FloodLimit", 0, &gUtil.nDdosFloodLimit);
			Execute_KIniFile_Clear(&FileHandle);*/

			/*KServerCore* m_pCore = This->m_pCore;
			unsigned int* v6;
			unsigned int v7;
			unsigned int v8;

			v6 = (unsigned int*)&m_pCore->m_pGameStatus[lnID];
			v7 = v6[1];
			v8 = *v6;

			int nNetStatus = *(int*)&m_pCore->m_pGameStatus[lnID].nNetStatus;*/
			
			//printf("NetConnectIdx: %d,nNetStatus : %d,v7 : %d,v8 : %d, dataLength : %d\n", lnID, nNetStatus, v7, v8, dataLength);

			//printf("NetConnectIdx: %d\n", lnID);
			//nProcessMessage(This, lnID, pData, dataLength);
			
			if (gUtil.nDdos <= 0)
			{
				nProcessMessage(This, lnID, pData, dataLength);
			}
			else
			{
				/*struct timeval tv;
				struct timezone tz;
				struct tm* tm;
				gettimeofday(&tv, &tz);
				tm = localtime(&tv.tv_sec);

				long long time_in_mill = tv.tv_sec * 1000LL + tv.tv_usec / 1000;

				printf("StartTime: %d:%02d:%02d %d %lld \n", tm->tm_hour, tm->tm_min, tm->tm_sec, tv.tv_usec, time_in_mill);*/
				bool IsLogin = true;

				const char* pClientInfo;
				pClientInfo = Execute_GetClientInfo((KSOServer*)This->m_pServer, lnID);
				if (pClientInfo != NULL && pClientInfo != "" && strlen(pClientInfo) > 0)
				{
					std::string delimiter = " : ";
					std::string s = "null";
					s = pClientInfo;
					std::vector<std::string> results;
					results.clear();
					boost::algorithm::split(results, s, boost::is_any_of(delimiter));
					if (!g_Firewall.CheckIpAddress((char*)results[0].c_str(), false))
					{
						printf("\n[JXGUARD] Detect Attack IP Address [ %s ]\n", results[0].c_str());
						if (gUtil.nDdos >= 2)
						{
							gUtil.SetByte((DWORD)&pData[0], 255);
							
						}
						if (gUtil.nDdos >= 3)
						{
							std::string cmd = "iptables -A INPUT -s ";
							std::string ipaddr = results[0].c_str();
							cmd += ipaddr;
							cmd += " -j DROP";
							system(cmd.c_str());
							system("iptables-save | uniq | iptables-restore");
						}
					}
				}

				long long nTickCount = gUtil.GetCurrentTimestamp();

				if ((int)(nTickCount - g_Firewall.m_TickClearIpAddress) >= (1000*10))
				{
					g_Firewall.m_TickClearIpAddress = nTickCount;
					g_Firewall.ClearIpAddress();
				}

				//if (m_ClientInfo[lnID].m_TickCount == 0LL && m_ClientInfo[lnID].m_Checked == false)
				//{
				//	if (dataLength == 17)
				//	{
				//		memcpy(m_ClientInfo[lnID].m_Guid, (char*)pData + 1, 16);
				//		std::string newguid = _GuidToString(m_ClientInfo[lnID].m_Guid);
				//		//printf("GUID: %s\n", newguid.c_str());
				//		strncpy(m_ClientInfo[lnID].m_TextGuid, newguid.c_str(), 32);
				//		m_ClientInfo[lnID].m_TextGuid[sizeof(m_ClientInfo[lnID].m_TextGuid) - 1] = 0;
				//		//printf("Save GUID: %s\n", m_ClientInfo[lnID].m_TextGuid);
				//		m_ClientInfo[lnID].m_TickCount = gUtil.GetCurrentTimestamp();
				//		m_ClientInfo[lnID].m_Checked = true;
				//		IsLogin = true;
				//	}
				//	else
				//	{
				//		long long nTickCount = gUtil.GetCurrentTimestamp();
				//		//printf("Info: %lld, %lld, %lld / %d\n", nTickCount, m_ClientInfo[lnID].m_TickCount, (nTickCount - m_ClientInfo[lnID].m_TickCount), gUtil.nDdosDelay);
				//		if ((int)(nTickCount - m_ClientInfo[lnID].m_TickCount) >= gUtil.nDdosDelay)
				//		{
				//			//printf("Set TickCount: %lld\n", nTickCount);
				//			m_ClientInfo[lnID].m_TickCount = nTickCount;
				//			IsLogin = true;
				//		}
				//		else
				//		{
				//			m_ClientInfo[lnID].m_Flood++;
				//			//printf("Set Flood: %d / %d\n", m_ClientInfo[lnID].m_Flood, gUtil.nDdosFloodLimit);
				//			if (m_ClientInfo[lnID].m_Flood >= gUtil.nDdosFloodLimit)
				//			{
				//				const char* pClientInfo;
				//				pClientInfo = Execute_GetClientInfo((KSOServer*)This->m_pServer, lnID);
				//				if (pClientInfo != NULL && pClientInfo != "" && strlen(pClientInfo) > 0)
				//				{
				//					std::string delimiter = " : ";
				//					std::string s = "null";
				//					s = pClientInfo;
				//					std::vector<std::string> results;
				//					results.clear();
				//					boost::algorithm::split(results, s, boost::is_any_of(delimiter));
				//					//printf("IPAddress: %s\n", results[0].c_str());
				//					if (gUtil.nDdos == 2)
				//					{
				//						if (!g_Firewall.CheckIpAddress((char*)results[0].c_str()))
				//						{
				//							g_Firewall.InsertIpAddress((char*)results[0].c_str(), false, false);
				//							std::string cmd = "iptables -A INPUT -s ";
				//							std::string ipaddr = results[0].c_str();
				//							cmd += ipaddr;
				//							cmd += " -j DROP";
				//							//printf("Block: %s\n", cmd.c_str());
				//							system(cmd.c_str());
				//							printf("Block IP Address attack: %s\n", results[0].c_str());
				//						}
				//						IsLogin = false;
				//						Execute_ShutdownClient((KSOServer*)This->m_pServer, lnID);
				//					}
				//					else if (gUtil.nDdos == 3)
				//					{
				//						if (!Sqlite_Check_WanAddress_GS(results[0].c_str()))
				//						{
				//							g_Firewall.InsertIpAddress((char*)results[0].c_str(), false, false);
				//							std::string cmd = "iptables -A INPUT -s ";
				//							std::string ipaddr = results[0].c_str();
				//							cmd += ipaddr;
				//							cmd += " -j DROP";
				//							//printf("Block: %s\n", cmd.c_str());
				//							system(cmd.c_str());
				//							printf("Block IP Address attack: %s\n", results[0].c_str());
				//						}
				//						IsLogin = false;
				//						Execute_ShutdownClient((KSOServer*)This->m_pServer, lnID);
				//					}
				//					//else if (gUtil.nDdos == 3)
				//					//{
				//					//	
				//					//	//char strCommand[100];
				//					//	//sprintf(strCommand, "iptables -I INPUT -s %s -j DROP", pWanAddress);
				//					//	//system(strCommand);

				//					//	//std::string cmd = "iptables -I INPUT -s ";
				//					//	//std::string ipaddr = results[0].c_str();
				//					//	//cmd += ipaddr;
				//					//	//cmd += " -j DROP";

				//					//	std::string cmd = "csf -td ";
				//					//	std::string ipaddr = results[0].c_str();
				//					//	cmd += ipaddr;
				//					//	printf("Block ip attack: %s\n", results[0].c_str());

				//					//	//cmd += " -j DROP";
				//					//	//printf("Block: %s\n", cmd.c_str());
				//					//	system(cmd.c_str());
				//					//	
				//					//}
				//				}
				//				else
				//				{
				//					IsLogin = false;
				//					Execute_ShutdownClient((KSOServer*)This->m_pServer, lnID);
				//					printf("Shut down client %d attack can't get IP Address\n", lnID);
				//				}
				//			}
				//		}
				//	}
				//}
				//else
				//{
				//	//printf("Guid len %d \n", strlen(m_ClientInfo[lnID].m_TextGuid));
				//	if (strlen(m_ClientInfo[lnID].m_TextGuid) < 31)
				//	{
				//		IsLogin = false;
				//		Execute_ShutdownClient((KSOServer*)This->m_pServer, lnID);
				//	}
				//}

				if (IsLogin)
					nProcessMessage(This, lnID, pData, dataLength);
				
			}

			
			
			//if (gUtil.nDdos == 2)
			//{
			//	const char* pClientInfo;
			//	//char pWanAddress[50];
			//	//memset(pWanAddress, 0, 50);
			//	pClientInfo = Execute_GetClientInfo((KSOServer*)This->m_pServer, lnID);
			//	if (!pClientInfo)
			//	{
			//		nProcessMessage(This, lnID, pData, dataLength);
			//	}
			//	else
			//	{
			//		//printf("ClientInfo: %s\n", pClientInfo);
			//		std::string delimiter = " : ";
			//		std::string s = "null";
			//		s = pClientInfo;
			//		//vector<string> list = gUtil.split(s, delimiter);
			//		std::vector<std::string> results;
			//		results.clear();
			//		boost::algorithm::split(results, s, boost::is_any_of(delimiter));
			//		//std::string::iterator end_pos = std::remove(list[0].begin(), list[0].end(), ' ');
			//		//list[0].erase(end_pos, list[0].end());
			//		//pWanAddress = results[0].c_str();
			//		//memcpy(pWanAddress, results[0].c_str(), 50);
			//		//pWanAddress[50] = 0;
			//		//printf("ClientInfo: %s\n", pWanAddress);
			//		if (!Sqlite_Check_WanAddress_GS(results[0].c_str()))
			//		{
			//			//printf("Block ip attack: %s\n", pWanAddress);
			//			//char strCommand[100];
			//			//sprintf(strCommand, "iptables -I INPUT -s %s -j DROP", pWanAddress);
			//			//system(strCommand);
			//			std::string cmd = "iptables -I INPUT -s ";
			//			std::string ipaddr = results[0].c_str();
			//			cmd += ipaddr;
			//			cmd += " -j DROP";
			//			//printf("Block: %s\n", cmd.c_str());
			//			system(cmd.c_str());
			//			printf("Block IP Address attack: %s\n", results[0].c_str());
			//			Execute_ShutdownClient((KSOServer*)This->m_pServer, lnID);
			//		}
			//		else
			//		{
			//			nProcessMessage(This, lnID, pData, dataLength);
			//		}
			//	}
			//	
			//}
			//else if (gUtil.nDdos == 3)
			//{
			//	const char* pClientInfo;
			//	//char pWanAddress[50];
			//	//memset(pWanAddress, 0, 50);
			//	pClientInfo = Execute_GetClientInfo((KSOServer*)This->m_pServer, lnID);
			//	if (!pClientInfo)
			//	{
			//		nProcessMessage(This, lnID, pData, dataLength);
			//	}
			//	else
			//	{
			//		//printf("ClientInfo: %s\n", pClientInfo);
			//		std::string delimiter = " : ";
			//		std::string s = "null";
			//		s = pClientInfo;
			//		//vector<string> list = gUtil.split(s, delimiter);
			//		std::vector<std::string> results;
			//		results.clear();
			//		boost::algorithm::split(results, s, boost::is_any_of(delimiter));
			//		//std::string::iterator end_pos = std::remove(list[0].begin(), list[0].end(), ' ');
			//		//list[0].erase(end_pos, list[0].end());
			//		//pWanAddress = results[0].c_str();
			//		//memcpy(pWanAddress, results[0].c_str(), 50);
			//		//pWanAddress[50] = 0;
			//		//printf("ClientInfo: %s\n", pWanAddress);
			//		if (!Sqlite_Check_WanAddress_GS(results[0].c_str()))
			//		{
			//			printf("Block ip attack: %s\n", results[0].c_str());
			//			//char strCommand[100];
			//			//sprintf(strCommand, "iptables -I INPUT -s %s -j DROP", pWanAddress);
			//			//system(strCommand);

			//			//std::string cmd = "iptables -I INPUT -s ";
			//			//std::string ipaddr = results[0].c_str();
			//			//cmd += ipaddr;
			//			//cmd += " -j DROP";

			//			std::string cmd = "csf -td ";
			//			std::string ipaddr = results[0].c_str();
			//			cmd += ipaddr;


			//			//cmd += " -j DROP";
			//			//printf("Block: %s\n", cmd.c_str());
			//			system(cmd.c_str());

			//			Execute_ShutdownClient((KSOServer*)This->m_pServer, lnID);
			//		}
			//		else
			//		{
			//			nProcessMessage(This, lnID, pData, dataLength);
			//		}
			//	}
			//}
			//else
			//{
			//	nProcessMessage(This, lnID, pData, dataLength);
			//}
		}

		//assert(pData && dataLength);

		//const char* pBuffer = pData;

		/*if (dataLength != 17 || *pBuffer != 66)
		{
			return 0;
		}
		else
		{*/
		//struct sigaction sa;

		//sa.sa_flags = SA_SIGINFO;
		//sigemptyset(&sa.sa_mask);
		//sa.sa_sigaction = handler;
		//if (sigaction(SIGSEGV, &sa, NULL) == -1)
		//	handle_error("sigaction");

		//BYTE nProtocol = (BYTE)pData[0];
		//const char* pBuffer = pData;
		
		// 
		//if (nProtocol == 66)
		//{
		//	//if (dataLength > 17 && dataLength < 18)
		//	//if (dataLength == 18)
		//	if (dataLength == 18)
		//	{
		//		dataLength = 17;
		//		nProcessMessage(This, lnID, pData, dataLength);
		//		//std::cout << "ok\n";
		//	}
		//	//
		//	////std::cout << "no\n";
		//	////else
		//	////{
		//	////	break;
		//	////}
		//	////std::cout << dataLength << "\n";

		//	////const char* pChar = NULL;
		//	////dataLength = 17;
		//	////char* guid;
		//	////char* IpAddress;
		//	//char MacAddress[18];
		//	//////char* HardwareId;
		//	//unsigned char mac[6] = { '\0' };

		//	////memcpy(&pChar, pData, dataLength);

		//	//tagLogicLoginBew* pLL = (tagLogicLoginBew*)pData;
		//	//std::string newguid;
		//	//char str_guid[39] = {};
		//	//sprintf(str_guid, "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
		//	//	&pLL->guid.Data1, &pLL->guid.Data2, &pLL->guid.Data3,
		//	//	&pLL->guid.Data4[0], &pLL->guid.Data4[1], &pLL->guid.Data4[2], &pLL->guid.Data4[3],
		//	//	&pLL->guid.Data4[4], &pLL->guid.Data4[5], &pLL->guid.Data4[6], &pLL->guid.Data4[7]);

		//	//printf("newguid 1 = %s\n", str_guid);
		//	//GUID guid;
		//	//guid = _StringToGuid(str_guid);
		//	///*nProcessMessage(This, lnID, pData, dataLength);*/

		//	//newguid = _GuidToString(guid);
		//	//printf("newguid 2 = %s\n", newguid.c_str());

		//	////memcpy(&mac, &pLL->MacAddress, sizeof(pLL->MacAddress));
		//	////sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		//	////memcpy(&MacAddress, &pData[18], sizeof(MacAddress));
		//	////sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		//	////memcpy(&MacAddress, &pLL->MacAddress, sizeof(pLL->MacAddress));

		//	////memcpy(&mac, &pLL->guid.MacAddress, 6);
		//	////sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		//	////printf("Size MacAddress = %d\n", sizeof(MacAddress));
		//	////printf("MacAddress = %s\n", MacAddress);

		//	////uint32_t ip = &pLL->guid.pIpAddr;
		//	////struct in_addr ip_addr;
		//	////ip_addr.s_addr = ip;

		//	////itoch.integer = pLL->guid.integer;
		//	////printf("Size pIpAddr = %d\n", sizeof(pLL->guid.integer));
		//	////printf("pIpAddr = %d\n", pLL->guid.integer);
		//	////printf("char[] values: %u.%u.%u.%u\n", itoch.byte[0], itoch.byte[1], itoch.byte[2], itoch.byte[3]);
		//	////unsigned int nIP = pLL->guid.integer; // some 32-bit integer
		//	////printf("%i.%i.%i.%i\n",
		//	////	(nIP >> 24) & 0xFF,
		//	////	(nIP >> 16) & 0xFF,
		//	////	(nIP >> 8) & 0xFF,
		//	////	nIP & 0xFF);

		//	////unsigned char bytes[4];
		//	////bytes[0] = nIP & 0xFF;
		//	////bytes[1] = (nIP >> 8) & 0xFF;
		//	////bytes[2] = (nIP >> 16) & 0xFF;
		//	////bytes[3] = (nIP >> 24) & 0xFF;
		//	////printf("%d.%d.%d.%d\n", bytes[3], bytes[2], bytes[1], bytes[0]);

		//	////printf("The IP address is %s\n", inet_ntoa(ip_addr));
		//	////printf("Size MacAddress = %d\n", sizeof(MacAddress));
		//	////printf("MacAddress = %s\n", MacAddress);

		//	//tagLogicLogin ll;
		//	//ll.cProtocol = 66;
		//	////ll.guid = _StringToGuid(newguid);
		//	////
		//	//////memcmp(&ll.guid, &pLL->_guid, sizeof(GUID));
		//	////memcpy(&ll.guid, &pLL->guid, 16);
		//	//memcpy(&ll.guid, (const GUID*)((char*)pData + 1), sizeof(GUID));
		//	////memcpy(&ll.guid, &pLL->guid, sizeof(GUID));
		//	////memcpy(&ll.guid, &pLL->guid, sizeof(GUID));

		//	//printf("Size tagLogicLogin = %d\n", sizeof(tagLogicLogin));
		//	//
		//	//char str_guid2[39] = {};
		//	//sprintf(str_guid2, "%08lX-%04hX-%04hX-%02hhX%02hhX-%02hhX%02hhX%02hhX%02hhX%02hhX%02hhX",
		//	//	&ll.guid.Data1, &ll.guid.Data2, &ll.guid.Data3,
		//	//	&ll.guid.Data4[0], &ll.guid.Data4[1], &ll.guid.Data4[2], &ll.guid.Data4[3],
		//	//	&ll.guid.Data4[4], &ll.guid.Data4[5], &ll.guid.Data4[6], &ll.guid.Data4[7]);

		//	//printf("newguid 3 = %s\n", str_guid2);

		//	////guid_to_str(&ll.guid, guid);
		//	////std::cout << guid << "\n";
		//	//

		//	////strcpy((char*)reg.Name, (char*)eg2.szCharacterName);

		//	////memcpy(&pLL, &pData, sizeof(tagLogicLoginBew));

		//	////memcpy(guid, &pLL->_guid.Data4, sizeof(&pLL->_guid.Data4));
		//	////memcpy(mac, &pLL->_guid.Data5, sizeof(pLL->_guid.Data5));
		//	////strncpy(mac, (char*)&pLL->_guid.Data5, sizeof(mac));
		//	////strncpy(MacAddress, (char*)&pLL->_guid.Data6, sizeof(IpAddress));
		//	////memcpy(MacAddress, &pLL->_guid.Data6, sizeof(IpAddress));
		//	////memcpy(HardwareId, &pLL->_guid.Data7, sizeof(&pLL->_guid.Data7));

		//	////char MacAddresses[18];

		//	////sprintf(MacAddresses, "%02X:%02X:%02X:%02X:%02X:%02X", MacAddress[0], MacAddress[1], MacAddress[2], MacAddress[3], MacAddress[4], MacAddress[5]);

		//	////sprintf(IpAddress, "%.2hhX", &pLL->_guid.Data5);
		//	////sprintf(MacAddress, "%.2hhX", &pLL->_guid.Data6);
		//	////sprintf(HardwareId, "%.2hhX", &pLL->_guid.Data7);
		//	////sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

		//	////std::cout << guid << "\n";
		//	////std::cout << IpAddress << "\n";
		//	////std::cout << MacAddress << "\n";
		//	////std::cout << HardwareId << "\n";
		//	//

		//	//nProcessMessage(This, lnID, (const char*)&ll, sizeof(tagLogicLogin));
		//	////nProcessMessage(This, lnID, (const char*)&ll, sizeof(tagLogicLogin));
		//	
		//}
		//else
		//{
		//	nProcessMessage(This, lnID, pData, dataLength);
		//}
		

		//int nIndex = nProcessLoginProtocol(This, lnID, pData, dataLength);

		//std::cout << nIndex << std::endl;

		//return nIndex;
		//tagLogicLogin* pLL = (tagLogicLogin*)pData;
		//std::cout << "A client try to login..." << std::endl;
		//int nIdx = nAttachPlayer((KPlayerSet*)This->tmp[0x8], lnID, &pLL->guid);
		//std::cout << nIdx << std::endl;
		//if (nIdx)
		//{
		//	std::cout << "Found player " << nIdx << " is logging in system!\n" << std::endl;
		//	return nIdx;
		//}
		//else
		//{
		//	std::cout << "Found player\n" << std::endl;
		//	return 0;
		//}
		//}
		//return 0;
	}
	catch (...)
	{
	}
}

void KPlayerNew::Hook_ProcessLoginProtocol()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);
	//uintptr_t start = (uintptr_t)(void*)0x80D8E7F;
	// 
	//uintptr_t start = (uintptr_t)(void*)0x80C4CFB; // set rank

	//uintptr_t start = (uintptr_t)(void*)0x81F6001; // KServerCore::AttachPlayer

	uintptr_t start = (uintptr_t)(void*)0x81F76CB; // KClientProcess::ProcessLoginProtocol 0x81F76CB

	//uintptr_t start = (uintptr_t)(void*)0x80BF670;
	//uintptr_t end = start + sizeof(GetEngergyDamageHook);

	//  Calculate start of page for mprotect.
	uintptr_t pagestart = start & -pagesize;

	//  Change memory protection.
	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}
	//*(BYTE*)(0x80CAB80) = -4;
	gUtil.SetCompleteHook(0xE8, start, &ProcessLoginProtocol_Hook);

}

int __cdecl ProcessLoginProtocol_Hook(KClientProcess* This, const unsigned long lnID, const char* pData, size_t dataLength)
{	
	const char* pBuffer = pData;

	if (dataLength != 18 || *pBuffer != 68)
	{
		return 0;
	}
	else
	{

		//char* test;
		//char* test2;

		//tagLogicLogin* pLL = (tagLogicLogin*)pData;

		//guid_to_str(&pLL->guid, test);

		//gUtil.SetByte((_DWORD)pData[0], 0x44);
		*(BYTE*)pData = 66;
		dataLength = 17;

		//std::cout << "pData: " << pData << std::endl;
		//std::cout << "dataLength: " << dataLength << std::endl;
		//std::cout << "pData size: " << sizeof(pData) << std::endl;

		//0x82512C7

		//memcpy(test2, &pLL->guid.Data4, sizeof(pLL->guid.Data4));

		//std::cout << test2 << std::endl;


		//std::cout << nIndex << std::endl;

		//std::cout << "A client try to login..." << std::endl;
		//int nIdx = nAttachPlayer((KPlayerSet*)((KClientProcess*)This->tmp[0x8]), lnID, &pLL->guid);
		int nIdx = nProcessLoginProtocol(This, lnID, pData, dataLength);
		//int nIdx = nAttachPlayer((KPlayerSet*)((KClientProcess*)This->tmp[0x8]), lnID, (GUID*)((char*)pData + 1));
		//std::cout << nIdx << std::endl;
		if (nIdx)
		{
			//std::cout << "Found player " << nIdx << " is logging in system!\n" << std::endl;
			return nIdx;
		}
		else
		{
			//std::cout << "Found player\n" << std::endl;
			return 0;
		}

		//return nIndex;
		//tagLogicLogin* pLL = (tagLogicLogin*)pData;
		//return 0;
	}
}

bool __cdecl SetRankHook(KPlayer* const cPlayer, CURPLAYER_SYNC* const pData, size_t nLen)
{
	try
	{
		if (gUtil.WORLDRANK_ACTIVE > 0)
		{
			bool resul = SendData(cPlayer, pData, nLen);
			//pthread_t thread_id;
			//time_t now = time(nullptr);
			//size_t Minutes = 5;
			//time_t newTime = now + (60 * Minutes);
			//unsigned long int secnewTime = now + (60 * Minutes);
			//unsigned long int sec = time(NULL);
			//std::cout << now << std::endl;
			//std::cout << sec << std::endl;
			//std::cout << newTime << std::endl;
			//std::cout << secnewTime << std::endl;
			int NewValue = 0;
			std::map<std::string, int>::iterator it = gKPlayerNew.WorldRank_Data.find((std::string)&cPlayer->tmp[0x25C88]);

			if (it != gKPlayerNew.WorldRank_Data.end())
			{
				NewValue = it->second;
			}
			//int NewValue = 2;
			if (NewValue > 0)
			{
				size_t pagesize = sysconf(_SC_PAGESIZE);
				uintptr_t start = (uintptr_t)&cPlayer->tmp[0x28004];
				uintptr_t end = start + sizeof NewValue;

				uintptr_t pagestart = start & -pagesize;

				if (mprotect((void*)pagestart, end - pagestart,
					PROT_READ | PROT_WRITE | PROT_EXEC))
				{
					perror("mprotect");
					//exit(EXIT_FAILURE);
				}

				memcpy((void*)start, &NewValue, sizeof NewValue);

				//pagesize = sysconf(_SC_PAGESIZE);
				start = (uintptr_t)&pData->tmp[0x36];
				end = start + sizeof NewValue;

				pagestart = start & -pagesize;

				if (mprotect((void*)pagestart, end - pagestart,
					PROT_READ | PROT_WRITE | PROT_EXEC))
				{
					perror("mprotect");
					//exit(EXIT_FAILURE);
				}
				memcpy((void*)start, &NewValue, sizeof NewValue);

				if (cPlayer == NULL || pData == NULL || nLen == -1 || nLen == NULL)
					return false;

				resul = SendData(cPlayer, pData, nLen);
				if (resul != NULL)
					return resul;
				else
					return false;

			}

			//if (now >= gUtil.TimeUpdateRank)
			//{
			//	/*KIniFile FileHandle;
			//	Execute_KIniFile(&FileHandle);
			//	Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
			//	Execute_KIniFile_GetInteger(&FileHandle, "Setting", "TimeUpdateRank", 0, &gUtil.nTimeUpdateRank);
			//	Execute_KIniFile_Clear(&FileHandle);*/

			//	size_t Minutes = gUtil.nTimeUpdateRank;
			//	time_t newTime = now + (60 * Minutes);
			//	//time_t newTime = now + 10;
			//	gUtil.TimeUpdateRank = newTime;
			//	//std::cout << now << " >= " << gUtil.TimeUpdateRank << std::endl;

			//	/*if (gUtil.nTimeUpdateRank > 0)
			//		pthread_create(&thread_id, NULL, &LoadRank, NULL);*/

			//	//LoadRank();

			//	std::map<std::string, int>::iterator it = gKPlayerNew.WorldRank_Data.find((std::string)&cPlayer->tmp[0x25C88]);


			//	if (it != gKPlayerNew.WorldRank_Data.end())
			//	{
			//		NewValue = it->second;
			//	}

			//	if (NewValue > 0)
			//	{
			//		//int NewValue = 2;
			//		size_t pagesize = sysconf(_SC_PAGESIZE);
			//		uintptr_t start = (uintptr_t)&cPlayer->tmp[0x28004];
			//		uintptr_t end = start + sizeof NewValue;

			//		uintptr_t pagestart = start & -pagesize;

			//		if (mprotect((void*)pagestart, end - pagestart,
			//			PROT_READ | PROT_WRITE | PROT_EXEC))
			//		{
			//			perror("mprotect");
			//			//exit(EXIT_FAILURE);
			//		}

			//		memcpy((void*)start, &NewValue, sizeof NewValue);

			//		//pagesize = sysconf(_SC_PAGESIZE);
			//		start = (uintptr_t)&pData->tmp[0x36];
			//		end = start + sizeof NewValue;

			//		pagestart = start & -pagesize;

			//		if (mprotect((void*)pagestart, end - pagestart,
			//			PROT_READ | PROT_WRITE | PROT_EXEC))
			//		{
			//			perror("mprotect");
			//			//exit(EXIT_FAILURE);
			//		}

			//		memcpy((void*)start, &NewValue, sizeof NewValue);

			//		if (cPlayer == NULL || pData == NULL || nLen == -1 || nLen == NULL)
			//			return false;

			//		resul = SendData(cPlayer, pData, nLen);
			//		if (resul != NULL)
			//			return resul;
			//		else
			//			return false;
			//	}

			//}
			//else
			//{
			//	//pthread_create(&thread_id, NULL, &LoadRank, NULL);
			//	//std::cout << now << " else " << gUtil.TimeUpdateRank << std::endl;
			//	
			//}

			//printf("%s\n", client::body);
			
			if (cPlayer == NULL || pData == NULL || nLen == -1 || nLen == NULL)
				return false;

			resul = SendData(cPlayer, pData, nLen);
			if (resul != NULL)
				return resul;
			else
				return false;

		}
		else
		{
			bool resul = SendData(cPlayer, pData, nLen);
			if (resul != NULL)
				return resul;
			else
				return false;
		}
		

		//CURPLAYER_SYNC sSyn;
		//sSyn = pData;
		//std::cout << (std::string*)cPlayer->tmp[0x25C88] << "\n";
		//printf("m_nPlayerIndex: %d\n", *(_DWORD*)&cPlayer->tmp[0xC0]);
		//printf("m_nIndex: %d\n", *(_DWORD*)&cPlayer->tmp[0x20404]);
		//printf("m_nStrength: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C44]);
		//printf("m_nDexterity: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C48]);
		//printf("m_nVitality: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C4C]);
		//printf("m_nEngergy: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C50]);
		//printf("m_nLucky: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C54]);
		//printf("m_nCurEngergy: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C68]);
		//printf("m_nCurLucky: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C6C]);
		//printf("m_nCurStrength: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C5C]);
		//printf("m_nCurDexterity: %d\n", *(_DWORD*)&cPlayer->tmp[0x25C60]);
		//if (!Npc)
		//	Npc = (KNpc*)Npc_Struct;
		//KPlayer* Player;
		//KPlayer* nPlayer;
		//if (!Player)
		//{
		//	Player = (KPlayer*)Player_Struct;
		//	nPlayer = &Player[cPlayer->tmp[0x20404]];
		//}
		//Npc[cPlayer->tmp[0x20404]].tmp[0x1504] = cPlayer->tmp[0x25C48] >> 2;
		////std::cout << Player->tmp[0xC0] << "\n";
		//std::cout << Player->tmp[0x20404] << "\n";

		/*int NewValue = 1;
		size_t pagesize = sysconf(_SC_PAGESIZE);
		uintptr_t start = (uintptr_t)&cPlayer->tmp[0x28004];
		uintptr_t end = start + sizeof NewValue;

		uintptr_t pagestart = start & -pagesize;

		if (mprotect((void*)pagestart, end - pagestart,
			PROT_READ | PROT_WRITE | PROT_EXEC))
		{
			perror("mprotect");
			exit(EXIT_FAILURE);
		}

		memcpy((void*)start, &NewValue, sizeof NewValue);*/

		//printf("m_Kind: %d\n", *(_DWORD*)&cPlayer->tmp[0x24]);
		//printf("m_nPlayerIndex: %d\n", *(_DWORD*)&Player->tmp[0xC0]);
		//printf("m_nPlayerIndex: %d\n", *(_DWORD*)&cPlayer->tmp[0xC0]);
		//printf("m_AccountName: %s\n", (std::string*)&cPlayer->tmp[0x264]);
		//printf("m_PlayerName: %s\n", (std::string*)&cPlayer->tmp[0x25C88]);
		//printf("m_nWorldStat: %d\n", (int)cPlayer->tmp[0x28004]);
		//printf("m_nWorldStat: %d\n", (int)&cPlayer->tmp[0x28004]);
		//printf("m_nIndex: %d\n", *(_DWORD*)&nPlayer->tmp[0x20404]);
		//printf("m_nPlayerIdx: %d\n", *(_DWORD*)&nPlayer->tmp[0x28004]);
		//printf("m_nWorldStat: %d\n", *(_DWORD*)&nPlayer->tmp[0x28004]);
		//std::map<std::string, int>::iterator it = gKPlayerNew.WorldRank_Data.find((std::string)&cPlayer->tmp[0x25C88]);
		//int NewValue = 0;

		//if (it != gKPlayerNew.WorldRank_Data.end())
		//{
		//	NewValue = it->second;

		//}

		////std::string hexRank;
		////std::cout << hexRank << std::hex << NewValue;
		////std::cout << hexRank;

		//std::string hexRank = format("%X", NewValue);
		////std::cout << hexRank.c_str()[0];
		////std::cout << hexRank.c_str()[1];
		////std::cout << hexRank.c_str()[2];
		//hexRank = (std::string)"B8" + hexRank.c_str()[1] + hexRank.c_str()[2] + "0" + hexRank.c_str()[0] + "000090";
		//std::cout << hexRank.c_str();

		//SyncGoldCoin(cPlayer, 0);
		//int NewValue = 1;
		//size_t pagesize = sysconf(_SC_PAGESIZE);
		//uintptr_t start = (uintptr_t)&cPlayer->tmp[0x28004];
		//uintptr_t end = start + sizeof NewValue;

		////  Calculate start of page for mprotect.
		//uintptr_t pagestart = start & -pagesize;

		////  Change memory protection.
		//if (mprotect((void*)pagestart, end - pagestart,
		//	PROT_READ | PROT_WRITE | PROT_EXEC))
		//{
		//	perror("mprotect");
		//	exit(EXIT_FAILURE);
		//}

		////  Write new bytes to desired location.
		//memcpy((void*)start, &NewValue, sizeof NewValue);
		//return 1;
	}
	catch (std::exception const& e)
	{
		//std::cerr << "Error: " << e.what() << std::endl;
		//printf("Error: %s\n", e.what());
		return false;
	}
}

int __cdecl GetEngergyDamageHook(KPlayer *const cPlayer)
{
	if (gUtil.THANPHAP_ACTIVE == 2)
	{
		if (*(int*)(&cPlayer->tmp[0x25CE0] + 8) == 2 || *(int*)(&cPlayer->tmp[0x25CE0] + 8) == 3)
		{
			return 1;
		}
		else
			return oGetEngergyDamage(cPlayer);
	}
	else
		return 1;
	/*if (gUtil.THANPHAP_ACTIVE == 1)
		return 0;
	else if (gUtil.THANPHAP_ACTIVE == 2)
		return 0;
	else
		return (int)&cPlayer->tmp[0x25C68];*/
}

int __cdecl SetEngergyDamageAgi(KNpc*const cKNpc)
{
	return 1;
	/*if (gUtil.THANPHAP_ACTIVE == 1)
		return 0;
	else if (gUtil.THANPHAP_ACTIVE == 2)
		return 0;
	else
		return (int)&cPlayer->tmp[0x25C68];*/
}

char* guid_to_str(const GUID* id, char* out) {
	int i;
	char* ret = out;
	out += sprintf(out, "%.8lX-%.4hX-%.4hX-", id->Data1, id->Data2, id->Data3);
	for (i = 0; i < sizeof(id->Data4); ++i) {
		out += sprintf(out, "%.2hhX", id->Data4[i]);
		if (i == 1) *(out++) = '-';
	}
	return ret;
}

char* data_to_str(const CUSTOM_GUID* data, char* out) {
	int i;
	char* ret = out;
	out += sprintf(out, "%.8lX-%.4hX-%.4hX-", data->Data1, data->Data2, data->Data3);
	for (i = 0; i < sizeof(data->Data4); ++i) {
		out += sprintf(out, "%.2hhX", data->Data4[i]);
		if (i == 1) *(out++) = '-';
	}
	return ret;
}

//static void
//handler(int signalNum, siginfo_t* si, void* unused)
//{
//	signal(SIGINT, SIG_DFL);
//	//printf("Got SIGSEGV at address: %p\n", si->si_addr);
//	//exit(EXIT_FAILURE);
//	throw(signalNum);
//	//exit(signal);
//}

//
//int __cdecl GetEngergyDamageHookAgi(KPlayer* cPlayer)
//{
//	int result = 0;
//	if (gUtil.THANPHAP_ACTIVE == 2)
//		return result;
//	else
//		return cPlayer->m_nCurEngergy;
//}

void KPlayerNew::Hook_LoadRoleData()
{
	bool SqlInit;

	SqlInit = Sqlite_Init();

	if (!SqlInit)
		exit(0);

	size_t pagesize = sysconf(_SC_PAGESIZE);
	uintptr_t start;
	uintptr_t pagestart;

	/*if (g_pCoreServerShell)
		g_pCoreServerShell = (KServerCore*)(LPVOID*)0x82B8830;*/

	//0x824A5E0

	/*start = (uintptr_t)(void*)0x88A1A20;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	if (!Player)
		Player = (KPlayer*)start;*/
	
	/*if (!CurrenPlayer)
		CurrenPlayer = (KPlayer*)start;*/


	//start = (uintptr_t)(void*)0x81ED1A8;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &BishopLoadRoleData);
	//
	
	start = (uintptr_t)(void*)0x81F78EE;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &NotifyHostEnterGame);

	// ================================= NotifyHostLeaveGame ===========================

	/*start = (uintptr_t)(void*)0x81F7A02;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeNotifyHostLeaveGame);*/

	// ================================= Update Battle Member Count ===========================
	//start = (uintptr_t)(void*)0x81DC4E2;
	//start = (uintptr_t)(void*)0x81DACDA;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeSendMemberCount);

	//start = (uintptr_t)(void*)0x81DC3E0;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeSendMemberCount);

	// ================================= Update Battle Member Count ===========================
	// 
	//start = (uintptr_t)(void*)0x80CE856;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SprintfLogsGiaoDich);

	// ========== Func LeaveGame ======== \\

	/*start = (uintptr_t)(void*)0x81F7A02;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeNotifyHostLeaveGame);*/

	//start = (uintptr_t)(void*)0x81EE8F1;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &HostProcessTransfer);

	

	//start = (uintptr_t)(void*)0x804FBCB; // OnLogin 0x81F77BE

	start = (uintptr_t)(void*)0x809AABC; // ProcessSyncReplyProtocol
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeOnEnterNewMap);
	
	start = (uintptr_t)(void*)0x81F77BE; // ProcessSyncReplyProtocol
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeProcessSyncReplyProtocol);


	start = (uintptr_t)(void*)0x80CB9C0; // OnEnterNewMap
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeOnLogin);

	for (int i = 0; i < 1200; i++)
	{
		m_KPlayerInfo[i].m_nIndex = -1;
		memset(m_KPlayerInfo[i].m_AccountName, 0, 32);
		memset(m_KPlayerInfo[i].m_PlayerName, 0, 32);
		memset(m_KPlayerInfo[i].m_LanIP, 0, 17);
		memset(m_KPlayerInfo[i].m_Hwid, 0, 32);
		m_KPlayerInfo[i].m_TickCount = 0LL;
		m_KPlayerInfo[i].m_NPCTalkFlood = 0;
		m_KPlayerInfo[i].m_ChatTick = 0LL;
		m_KPlayerInfo[i].m_ChatFlood = 0;
		m_KPlayerInfo[i].m_Faction = -1;
		m_KPlayerInfo[i].m_KPlayer = NULL;
		m_KPlayerInfo[i].m_AutoCheck = false;
	}

	//start = (uintptr_t)(void*)0x80DECDB; // KPlayer::OnQuit
		//start = (uintptr_t)(void*)0x8053A6C; // KServerCore::PlayerPrepareQuit
		//start = (uintptr_t)(void*)0x80DCECC; // OnLeaveCurMap
	start = (uintptr_t)(void*)0x804FA2C; // KPlayer::WaitForRemove
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &SafeOnQuit);

	//if (gUtil.WORLDRANK_ACTIVE == 2)
	//{

	//	// ============================================ \\

	//	start = (uintptr_t)(void*)0x804FB6C;
	//	pagestart = start & -pagesize;

	//	if (mprotect((void*)pagestart, 0x6u,
	//		PROT_READ | PROT_WRITE | PROT_EXEC))
	//	{
	//		perror("mprotect");
	//		exit(EXIT_FAILURE);
	//	}

	//	gUtil.SetCompleteHook(0xE9, start, &SafeLoadDBPlayerInfo);

	//}

	//start = (uintptr_t)(void*)0x81F5D86;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &AddPlayerToWorld);

	//start = (uintptr_t)(void*)0x80D95E1;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &LoadRoleBaseInfo);
	
	start = (uintptr_t)(void*)0x81EDA03;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &BishopGatewaySmallPackProcess);
	
	// =============================================================== \\

	/*start = (uintptr_t)(void*)0x81B4C17;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &TongLogicMember_Add);*/

	// =============================================================== \\

	/*start = (uintptr_t)(void*)0x81CB1FB;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x200u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &ProcessProtocolTongExProtoco);*/

	// =========================================================== \\

	//start = (uintptr_t)(void*)0x80EB78B;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &OnJoinTong);

	// =========================================================== \\

	if (gUtil.nMaxTongMember > 0)
	{

		start = (uintptr_t)(void*)0x8052462;
		pagestart = start & -pagesize;

		if (mprotect((void*)pagestart, 0x6u,
			PROT_READ | PROT_WRITE | PROT_EXEC))
		{
			perror("mprotect");
			exit(EXIT_FAILURE);
		}

		gUtil.SetCompleteHook(0xFF, start, &CheckAddCondition);
	}

	// =========================================================== \\

	/*start = (uintptr_t)(void*)0x804F00C;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &ProcessApplyJoinTongFromZhaoMu);*/

	// =========================================================== \\
	
	if (gUtil.nMaxTongMember > 0)
	{
		start = (uintptr_t)(void*)0x804EF9C;
		pagestart = start & -pagesize;

		if (mprotect((void*)pagestart, 0x6u,
			PROT_READ | PROT_WRITE | PROT_EXEC))
		{
			perror("mprotect");
			exit(EXIT_FAILURE);
		}

		gUtil.SetCompleteHook(0xE9, start, &SafeAcceptOrRefuseApplyToRelay);
	}
	
	
	// =========================================================== \\

	//start = (uintptr_t)(void*)0x8137040;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE9, start, &DisableFunctionLua);

	// =========================================================== \\

	//start = (uintptr_t)(void*)0x8136E80;
	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE9, start, &DisableFunctionLua);

	// =========================================================== \\

	start = (uintptr_t)(void*)0x81F7067;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeGroupChat); // Fix chat room

	/*start = (uintptr_t)(void*)0x812B1C6;
	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &KSkillListAddSkillExp);*/


	/*start = (uintptr_t)(void*)0x80A232C;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SubAddSkillExp);*/


	/*start = (uintptr_t)(void*)0x8099048;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeSendDataToNearRegion);*/



	/*start = (uintptr_t)(void*)0x0809219B;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeSendDataToNearRegion);*/


	start = (uintptr_t)(void*)0x08094824;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeRemoveStateSkillEffect);

	//===================== ClearHideState ==================\\

	//gUtil.SetByte(0x809323B, 0x01);

	start = (uintptr_t)(void*)0x80998EB;
	pagestart = start & -pagesize;
	mprotect((void*)pagestart, 0x6u, PROT_READ | PROT_WRITE | PROT_EXEC);
	gUtil.SetCompleteHook(0xE8, start, &SafeSendSyncData);

	/*start = (uintptr_t)(void*)0x809C43C;
	pagestart = start & -pagesize;
	mprotect((void*)pagestart, 0x6u, PROT_READ | PROT_WRITE | PROT_EXEC);
	gUtil.SetCompleteHook(0xE8, start, &SafeClearHideState);

	start = (uintptr_t)(void*)0x8094881;
	pagestart = start & -pagesize;
	mprotect((void*)pagestart, 0x6u, PROT_READ | PROT_WRITE | PROT_EXEC);
	gUtil.SetCompleteHook(0xE9, start, &SafeClearHideState);

	start = (uintptr_t)(void*)0x809DF72;
	pagestart = start & -pagesize;
	mprotect((void*)pagestart, 0x6u, PROT_READ | PROT_WRITE | PROT_EXEC);
	gUtil.SetCompleteHook(0xE8, start, &SafeClearHideState);
	
	start = (uintptr_t)(void*)0x811E22A;
	pagestart = start & -pagesize;
	mprotect((void*)pagestart, 0x6u, PROT_READ | PROT_WRITE | PROT_EXEC);
	gUtil.SetCompleteHook(0xE8, start, &SafeClearHideState);*/

	//start = (uintptr_t)(void*)0x80997E0;
	//pagestart = start & -pagesize;
	//mprotect((void*)pagestart, 0x6u, PROT_READ | PROT_WRITE | PROT_EXEC);
	//gUtil.SetCompleteHook(0xE8, start, &SafeSyncPlayer);

	//===================== ProcessClientProtocol ==================\\

	start = (uintptr_t)(void*)0x80F11EB;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &SafeProcessClientProtocol);

	//===================== AutoAttackNpc ==================\\

	//start = (uintptr_t)(void*)0x80F2C08;

	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x6u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE9, start, &SafeNpcSkillCommand);

	/*if (!Npc)
		Npc = (KNpc*)0x8320A00;*/

	//Npc = (KNpc*)0x8320A00;

	//memset((void*)start, *(char*)"B80000000090", 6);

	/*if (gUtil.nAddTongMoneyUi == 0)
	{
		gUtil.SetDword(0x825FEE8, (DWORD)&Fix_AddMoneyFund);
		gUtil.SetDword(0x825FD08, (DWORD)&Fix_AddMoneyFund);
	}*/
	
	 
	//gUtil.SetDword(0x825FB6C, (DWORD)&NotifyEnterGame);

	//gUtil.SetByte(0x80A265E, 3);

	/*if (!Player)
		Player = (KPlayer*)0x88A1A20;
	
	if (!PlayerSet)
		PlayerSet = (KPlayerSet*)0x88A1EE0;*/

}

void __cdecl SafeNpcSkillCommand(KProtocolProcess* This, int nIndex, BYTE* pProtocol)
{
	printf("nPlayerIdx: %d\n", nIndex);
	if (gUtil.nCheckAuto > 0)
	{
		if (m_KPlayerInfo[nIndex].m_AutoCheck == false)
		{
			Execute_OfflineLive((KPlayer*)m_KPlayerInfo[nIndex].m_KPlayer);
			Execute_KickOut((KPlayer*)m_KPlayerInfo[nIndex].m_KPlayer, 6);
		}
		else
			Execute_NpcSkillCommand(This, nIndex, pProtocol);
	}
	else
		Execute_NpcSkillCommand(This, nIndex, pProtocol);
}

bool __cdecl SafeProcessClientProtocol(KTongManagerAgent* This, int nPlayerIdx, BYTE* pData)
{
	try
	{
		//printf("nPlayerIdx: %d\n", nPlayerIdx);

		//KPlayer* Player;

		//KPlayer* CurrenPlayer;

		/*if (!Player)
			Player = (KPlayer*)0x88a1a20;*/

			//CurrenPlayer = (KPlayer*)Player;

			//Execute_Init((KItemList*)&CurrenPlayer->tmp[0x2040C], nPlayerIdx);
			//Execute_SendMoneySync((KItemList*)&CurrenPlayer->tmp[0x2040C]);

			//printf("ndwID: %d\n", CurrenPlayer->tmp[0x20400]);
			//printf("ndwID: %d\n", CurrenPlayer[nPlayerIdx].tmp[0x20400]);

			//printf("nIndex: %d\n", CurrenPlayer->tmp[0x20404]);
			//printf("nIndex: %d\n", CurrenPlayer[nPlayerIdx].tmp[0x20404]);

			//printf("nPlayerIdx: %d\n", CurrenPlayer->tmp[0xC0]);

		/*start = (uintptr_t)(void*)0x88A1A20;
		pagestart = start & -pagesize;

		if (mprotect((void*)pagestart, 0x2000u,
			PROT_READ | PROT_WRITE | PROT_EXEC))
		{
			perror("mprotect");
			exit(EXIT_FAILURE);
		}

		if (!Player)
			Player = (KPlayer*)start;*/
		
		//printf("nPlayerIdx: %d\n", CurrenPlayer[nPlayerIdx].tmp[0xC0]);

		//KPlayerTong* cTong = (KPlayerTong*)&CurrenPlayer[nPlayerIdx].tmp[0x25DA4];

		//printf("cTong PlayerIdx: %d\n", cTong->tmp[0x24]);

		//Execute_GetGameData();	

		//printf("cTong PlayerIdx: %d\n", cTong->tmp[0x24]);

		//printf("CurrenPlayer: %d\n", (DWORD*)CurrenPlayer);

		TongExProtocol tongExProtocol;

		tongExProtocol = (TongExProtocol) * (unsigned __int16*)(pData + 3);
		int dwTongProtocolSize = *(unsigned __int16*)(pData + 1) - 4;

		//printf("TongExProtocol: %d - %d\n", tongExProtocol, *(unsigned __int16*)(pData + 3));

		if (tongExProtocol == 17674)
		{
			

			//printf("dwTongProtocolSize: %d\n", dwTongProtocolSize);

			if (dwTongProtocolSize == 16)
			{

				KIniFile FileHandle;
				Execute_KIniFile(&FileHandle);
				Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
				Execute_KIniFile_GetInteger(&FileHandle, "Overload", "AddTongMoneyUi", 0, &gUtil.nAddTongMoneyUi);
				Execute_KIniFile_Clear(&FileHandle);

				if (gUtil.nAddTongMoneyUi == 2)
				{
					return Execute_ProcessClientProtocol(This, nPlayerIdx, pData);
				}

				if ((unsigned int)(nPlayerIdx) > 1198)
					return false;

				TONG_MEMBER_PAGE_DATA_REQ* pTongProtocol;

				pTongProtocol = (TONG_MEMBER_PAGE_DATA_REQ*)(pData + 5);

				//printf("m_dwTongID: %d\n", pTongProtocol->m_dwTongID);
				//int nValue;
				BYTE* pTongProtocolData = &pTongProtocol->m_byFilterFlag;

				if (*((int*)pTongProtocolData + 2))
				{
					INT64 nValue = *((int*)pTongProtocolData + 2);

					//printf("Value: %d\n", nValue);

					if (nValue < 0)
					{
						return Execute_ProcessClientProtocol(This, nPlayerIdx, pData);
					}
					else
					{
						int nOldIndex;
						INT64 pScriptResult;
						pScriptResult = 0;
						bool runScript;

						//KPlayer* Player = (KPlayer*)((char*)0x88A1A20 + 165956 * nPlayerIdx);
						KPlayer* Player = (KPlayer*)m_KPlayerInfo[nPlayerIdx].m_KPlayer;

						//Execute_SetPlayerIndex(Player, nPlayerIdx);
						//runScript = Execute_ExecuteScript_Name(Player, "\\script\\lib\\player.lua", "Player:GetCash", 1, "ds", nPlayerIdx, "GetCash");
						//char mParam[32];
						//sprintf(mParam, "%d", nPlayerIdx);
						//Execute_ExecuteScript_Name(Player, "\\script\\lib\\player.lua", "Player:Init", 1, "ds", nPlayerIdx, "Init");
						//Execute_ExecuteScript_2(Player, "\\script\\lib\\player.lua", "Init", mParam, 1);
						runScript = Execute_ExecuteScript_2(Player, "\\script\\lib\\player.lua", "GetCash", 0, 1);
						//runScript = Execute_ExecuteScript_Name(Player, "\\script\\lib\\gprotect.lua", "CheckCurrenMoney", 1, "dd", nPlayerIdx, nValue);
						/*DWORD scriptid = Execute_FileName2Id((LPCSTR)"\\script\\lib\\gprotect.lua");
						va_list body;
						va_start(body, "dd");
						runScript = Execute_ExecuteScript_vlist(Player, scriptid, "CheckCurrenMoney", 1, "dd", body);
						va_end(body)*/;

						//printf("runScript: %d\n", runScript);
						//nPlayerIdx = Player[nPlayerIdx].tmp[0xC0];
						//CurrenPlayer[nPlayerIdx].tmp[0xC0] = nOldIndex;
						/*bool PayOK = Execute_Pay(Player, nValue);
						if (PayOK)
						{
							return Execute_ProcessClientProtocol(This, nPlayerIdx, pData);
						}
						else
							return false;*/

						if (runScript)
						{
							//printf("Money: %d\n", *(int*)(&Player->tmp[0x27C00]));
							/*if (!*(int*)(&Player->tmp[0x27C00]))
								return false;*/

							pScriptResult = *(int*)(&Player->tmp[0x27C00]);
							//printf("Money: %d\n", pScriptResult);
							if (pScriptResult < nValue)
							{
								//return false;
								*(INT64*)(pTongProtocolData + 2) = pScriptResult;
								//nValue = pScriptResult;
								return Execute_ProcessClientProtocol(This, nPlayerIdx, pData);
							}
							else
								return Execute_ProcessClientProtocol(This, nPlayerIdx, pData);
						}
						else
							return false;

					}
					//const char* Func = "GetCash";

					/*if (!CurrenPlayer)
						CurrenPlayer = (KPlayer*)(uintptr_t)(void*)0x88A1A20;*/

						//nOldIndex = CurrenPlayer[nPlayerIdx].tmp[0xC0];
						//if (CurrenPlayer[nPlayerIdx].tmp[0xC0])
						//{
					//}
					//else
						//return false;
				}
				else
					return false;
			}
			else
				return Execute_ProcessClientProtocol(This, nPlayerIdx, pData);

			
		}
		else
			return Execute_ProcessClientProtocol(This, nPlayerIdx, pData);

		//return 0;

		//*(_DWORD*)pTongProtocolData

		//printf("TongProtocolData 1: %d\n", *((DWORD*)pTongProtocolData + 1));
		//printf("TongProtocolData 2: %d\n", *((DWORD*)pTongProtocolData + 2)); // Money Count
		//printf("TongProtocolData 3: %d\n", *((DWORD*)pTongProtocolData + 3));
		//printf("TongProtocolData Data: %d\n", *(DWORD*)pTongProtocolData);

		/*int ndwID;
		int nIndex;
		int nPlayerCashMoney1;
		int nPlayerCashMoney2;
		KItemList* ItemList;
		KInventory Inventory[15];*/



		//printf("ItemList: %x\n", (DWORD*)(&CurrenPlayer[nPlayerIdx].tmp[0x2040C]));

		//ndwID = (int)CurrenPlayer->tmp[0x25C88];

		//ndwID = (DWORD*)&CurrenPlayer->tmp[0x20400];

		//nIndex = (DWORD*)&CurrenPlayer->tmp[0x20404];

		//printf("CostMoney: %d\n", Execute_CostMoney((KItemList*)&CurrenPlayer[nPlayerIdx].tmp[0x2040C], 10000));

		//printf("MoneyAmount: %d\n", Execute_GetMoneyAmount((KItemList*)&CurrenPlayer[nPlayerIdx].tmp[0x2040C]));

		//printf("GetEquipmentMoney: %d\n", Execute_GetEquipmentMoney((KItemList*)&CurrenPlayer[nPlayerIdx].tmp[0x2040C]));

		//Execute_ShutdownPlayer(CurrenPlayer);

		//printf("PlayerName: %s\n", (char*)CurrenPlayer->tmp[0x25C88]);

		//nPlayerCashMoney1 = (double)((char*)&Player->tmp[0x2040C + 0x4C2C + 5927 * nPlayerIdx + 0x4]);

		//ItemList = (KItemList*)(&Player->tmp[0x2040C]);

		//Inventory = *(KInventory*)((char*)&ItemList.tmp[0x4C2C]);

		//nPlayerCashMoney2 = (double)ItemList->m_Room[5927 * nPlayerIdx].m_nMoney;

		//printf("nPlayerCashMoney 1: %d\n", nPlayerCashMoney1);

		//printf("nPlayerCashMoney 2: %d\n", nPlayerCashMoney2);

		/*int TongMemberID;

		int PlayerIdx;

		PlayerIdx = 165956 * nPlayerIdx;

		TongMemberID = *(DWORD*)((char*)&Player->tmp[0x25DA4] + 0x28 + PlayerIdx);

		printf("TongMemberID: %d\n", TongMemberID);

		*/
		
	}
	catch (...)
	{
		return false;
	}
	
}

//bool __cdecl SafeSendMemberCount(KPlayer* This, const char* pData, size_t nLen)
unsigned int __cdecl SafeSendMemberCount(KMission* This, const char* pBuffer, unsigned int ulBufferSize)
{
	try
	{
		//printf("m_FakeUserBattles: %d \n", gUtil.m_RateFakeBattles);

		//printf("m_FakeUserBattles: %d \n", *(int*)&pBuffer[7]);
		//printf("m_FakeUserBattles: %d \n", *(int*)&pBuffer[11]);
		
		//printf("m_FakeUserBattles: %d\n", *(int*)&gUtil.m_FakeUserBattles);

		if (gUtil.m_RateFakeBattles > 0)
		{
			*(int*)&pBuffer[7] = *(int*)&pBuffer[7] + floor((*(int*)&pBuffer[7] * gUtil.m_RateFakeBattles) / 100);
			*(int*)&pBuffer[11] = *(int*)&pBuffer[11] + floor((*(int*)&pBuffer[11] * gUtil.m_RateFakeBattles) / 100);
		}
	}
	catch (...)
	{
	}
	
	
	return Execute_PackDataToAll(This, pBuffer, ulBufferSize);
	//return Execute_SendData(This, (void*)pData, nLen);
}

int __cdecl SprintfLogsGiaoDich(char* szBuffer, const char* szText, const char* szAccountSell, const char* szRoleNameSell, const char* szItemName, LPCSTR GmCommand, int nCount, const char* szAccountNameBuy, const char* szRoleNameBuy, DWORD nTime)
{
	printf("szAccountSell: %s\n", szAccountSell);
	printf("szRoleNameSell: %s\n", szRoleNameSell);
	printf("szItemName: %s\n", szItemName);

	printf("nCount: %d\n", nCount);

	printf("szAccountNameBuy: %s\n", szAccountNameBuy);
	printf("szRoleNameBuy: %s\n", szRoleNameBuy);

	printf("nTime: %d\n", nTime);

	return sprintf(szBuffer, szText, szAccountSell, szRoleNameSell, szItemName, GmCommand, nCount, szAccountNameBuy, szRoleNameBuy, nTime);
}

int __cdecl LoadRoleBaseInfo(KPlayer* This, TRoleBaseInfo* pBaseInfo, BOOL bLogin)
{
	int m_Data;
	m_Data = Execute_LoadRoleDataInfo(This, pBaseInfo, bLogin);

	BYTE szBuff[sizeof(TRoleBaseInfo) + 1];

	*(BYTE*)&szBuff[0] = 30;

	//TRoleBaseInfo* dataShort = (TRoleBaseInfo*)&szBuff[1];
	//memset(&dataShort, 0, sizeof(sizeof(TRoleBaseInfo)));
	//dataShort = pBaseInfo;
	///*dataShort->ProtocolID = 29;
	//dataShort->Size = sizeof(KDataShort);
	//dataShort->nPlayerIndex = nPlayerIndex;
	//dataShort->lnID = lnID;
	//dataShort->nLevel = nLevel;
	//memcpy(&dataShort->m_szAccount, pszAccount, sizeof(dataShort->m_szAccount));
	//memcpy(&dataShort->m_szRole, pszRole, sizeof(dataShort->m_szAccount));*/

	//printf("ServerID: %d\n", gUtil.m_SubWorld->tmp[0xF0]);
	//printf("ServerID: %d\n", dataShort->ifightlevel);
	//printf("ServerID: %s %s\n", dataShort->caccname, dataShort->szName);
	//printf("ServerID: %d %d\n", dataShort->imoney, dataShort->fightexp);

	//gBishopClient->ClientSend((const void*)szBuff, sizeof(KDataShort) + 1);

	return m_Data;
}

bool __cdecl Fix_AddMoneyFund(KTongLogic* This, DWORD dwExecutorID, DWORD dwTongID, INT64 llValue)
{
	try
	{
		KIniFile FileHandle;
		Execute_KIniFile(&FileHandle);
		Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
		Execute_KIniFile_GetInteger(&FileHandle, "Overload", "AddTongMoneyUi", 0, &gUtil.nAddTongMoneyUi);
		Execute_KIniFile_Clear(&FileHandle);

		if (gUtil.nAddTongMoneyUi == 1)
		{
			return false;
		}
		/*if (llValue <= 0)
			return Execute_TONG_AddMoneyFund(This, dwExecutorID, dwTongID, llValue);
		else
			return false;*/

		if (dwTongID <= 0 || dwExecutorID <= 0)
			return false;

		int pScriptResult;
		pScriptResult = 0;
		bool runScript;
		std::_Rb_tree_node_base* M_parent;
		int nPlayerIndex;
		int Role; // ebx
		int nCurrenMoney; // ebx
		int nCurrenMoney1; // ebx
		int nMoney; // ebx
		bool nGetMoney; // ebx
		TONG_CHANGE_MONEY_COMMAND* psMoney;

		std::_Rb_tree_node_base* v13; // eax
		std::_Rb_tree_iterator<std::pair<const long unsigned int, KTongMember*> > TongMember;
		std::_Rb_tree_iterator<std::pair<const long unsigned int, KTongData*> > TongData;

		KPlayer* Player4;

		//KPlayer* Player = reinterpret_cast<KPlayer*(__cdecl*)(DWORD)>(0x88A1A20)(0x28844);

		unsigned int __k; // [esp+38h] [ebp-10h] BYREF
		//0x947AAE0 0x947AAE0 0x947AB00 0x8056800 0x947AB04
		runScript = 0;
		nPlayerIndex = 0;

		//printf("0: %d\n", llValue);

		if (dwExecutorID > 0 || dwTongID > 0)
		{
			__k = dwTongID;
			TongData = ((std::_Rb_tree<long unsigned int, std::pair<const long unsigned int, KTongData*>, std::_Select1st<std::pair<const long unsigned int, KTongData*> >, std::less<long unsigned int>, std::allocator<std::pair<const long unsigned int, KTongData*> > >*)(void*)0x947AB00)->find(__k);
			//std::map<const long unsigned int, KTongData*>::iterator TongData = this->m_LoginHardwareIdLimitInfo.find(__k);
			//Execute_findTongData(&TongData, (std::_Rb_tree<long unsigned int, std::pair<const long unsigned int, KTongData*>, std::_Select1st<std::pair<const long unsigned int, KTongData*> >, std::less<long unsigned int>, std::allocator<std::pair<const long unsigned int, KTongData*> > >*)(void*)0x947AB00, &__k);
			if (TongData._M_node != (std::_Rb_tree_node_base*)0x947AB04)
			{
				M_parent = TongData._M_node[1]._M_parent;
				if (M_parent)
				{
					__k = dwExecutorID;
					TongMember = ((std::_Rb_tree<long unsigned int, std::pair<const long unsigned int, KTongMember*>, std::_Select1st<std::pair<const long unsigned int, KTongMember*> >, std::less<long unsigned int>, std::allocator<std::pair<const long unsigned int, KTongMember*> > > *) & M_parent[77]._M_left)->find(__k);
					//Execute_findPlayerData(&TongMember, (std::_Rb_tree<long unsigned int, std::pair<const long unsigned int, KTongMember*>, std::_Select1st<std::pair<const long unsigned int, KTongMember*> >, std::less<long unsigned int>, std::allocator<std::pair<const long unsigned int, KTongMember*> > > *)&M_parent[77]._M_left, &__k);
					if (TongMember._M_node != (std::_Rb_tree_node_base*)&M_parent[77]._M_right)
					{
						v13 = TongMember._M_node[1]._M_parent;
						if (v13)
						{
							Role = Execute_FindRole((KPlayerSet*)(void*)0x88A1EE0, (LPCSTR)&v13[3]._M_left);
							if (Role - 1 < 1199) {
								nPlayerIndex = Role;
							}
							else
								return false;
						}
					}
				}
			}

		}
		else
			return false;

		//const char* pBuffer;

		nMoney = llValue;

		////printf("0: %d - %u %d\n", dwExecutorID, dwTongID, llValue);
		////printf("1: %d - %s\n", nPlayerIndex, (LPCSTR)&v13[3]._M_left);

		////char szMsg[42];
		////szMsg[9] = 0;
		////*(_DWORD*)&szMsg[5] = 0;
		////strcpy(szMsg, "4 ");
		////*(WORD*)&szMsg[3] = 0;
		////strncpy(&szMsg[10], (LPCSTR)&v13[3]._M_left, 0x1Fu);
		////szMsg[41] = 0;

		////NotifyGoddessEnterGame(g_pCoreServerShell->m_pServer, 0, szMsg, 42);
		////(*((void(__cdecl**)(IGameServer*, _DWORD, char*, int)) &g_pCoreServerShell->m_pServer->_vptr_IGameServer + 2))((IGameServer*)&g_pCoreServerShell->m_pServer, 0, szMsg, 42);
		////if(!Player)
		//	//Player = (KPlayer*)0x88a1a20;
		////Player = (KPlayer*)0x88a1a20;
		////pBuffer = (char*)(void*)0x88a1a20;

		//if(!Player4)
			//Player4 = (LPVOID*)((char*)0x88a1a20 + 165956 * nPlayerIndex);

		//printf("PlayerIndex: %d\n", nPlayerIndex);

		Player4 = (KPlayer*)0x88A1A20;

		//printf("KPlayer: %d\n", *(int*)((char*)&Player4->tmp[0xC0] + 165956 * nPlayerIndex));
		////Player4 = (KPlayer*)0x8250754;
		////088a1a20
		////Player4 = (KPlayer*)Player_Struct;
		////KLuaScript* TmpScript;
		////TmpScript = Execute_GetTmpScript((KLuaScriptSet*)(void*)0x82B9DA0);
		////Execute_CallFunction(TmpScript, "callPlayerFunction", 1, "ds", nPlayerIndex, "GetCash");
		////Player4 = (KPlayer*)&Player[nPlayerIndex];
		////_ShutdownClient ShutdownClient = (_ShutdownClient)0x80C0230;

		////KItemList* itemlist = (KItemList*)(DWORD*)&Player4 + 0x2040C;
		////KInventory* Inventory = (KInventory*)&itemlist->tmp[0x4C2C];

		if (Player4)
		{
			if (nMoney < 0)
			{
				return Execute_TONG_AddMoneyFund(This, dwExecutorID, dwTongID, llValue);
			}
			else
			{
				runScript = Execute_ExecuteScript_Name(Player4, "\\script\\lib\\gprotect.lua", "CheckCurrenMoney", 1, "dd", nPlayerIndex, nMoney);
				//printf("1: %d\n", runScript);
				if (!runScript)
					return false;
				else
				{
					pScriptResult = *(int*)((char*)&Player4->tmp[0x27C00]);
					//printf("2: %d\n", pScriptResult);
					if (pScriptResult > 0)
						return Execute_TONG_AddMoneyFund(This, dwExecutorID, dwTongID, llValue);
					else
						return false;
				}
			}
		}
		else
			return false;

		//bool runScript = Execute_ExecuteScript_Name((LPVOID*)&Player4, "\\script\\lib\\player.lua", "GetCurrenMoney", 1, "d", nPlayerIndex);
		//bool runScript = Execute_ExecuteScript((LPVOID*)&Player4, "\\script\\lib\\player.lua","GetCash", 0, 1u);
		//Execute_ExecuteScript_File(Player4,"\\script\\lib\\player.lua","Player:GetCash", 1,"d");

		//nCurrenMoney = (int)&Player4 + 0x27C00;

		//printf("0: %s %d - %s %d\n", (char*)&Player4 + 0x27C00, runScript, atoi((char*)&Player4 + 0x27C04), Execute_GetEquipmentMoney((LPVOID*)&itemlist));

		//printf("1: %d %d\n", Inventory[0].m_nMoney, Inventory[1].m_nMoney);
		//printf("2: %d %d\n", nCurrenMoney, (int)*(&Player4 + 0x27C04));
		//// 
		//printf("3: %d %d\n", (int)*(&Player4->tmp[0x2040C] + 0x4C2C + 4), *(&Inventory + 4));
		////printf("3: %d %d\n", (int)&Player4 + 154700, (int)(&Player4 + 154704));
		//printf("4: %d %d\n", *(&itemlist + 0x4C2C + 4), (DWORD)*(&itemlist + 0x4C2C + 4));

		//printf("5: %d\n", Execute_GetMoneyAmount((LPVOID*)&itemlist));

		//Execute_Msg2Player((LPVOID*)&Player4, "Limit");
		//Execute_ShutdownClient((LPVOID*)&Player4);
		//ShutdownClient(&Player[nPlayerIndex]);
		//psMoney->m_dwMoneyFundChange = nMoney;
		//psMoney->m_btSaveFlag = false;

		//nGetMoney = Execute_CheckChangeMoneyPower((KPlayerTong*)&Player4 + 0x25DA4, psMoney);
		//printf("4: %d\n", nGetMoney);

		//KItemList* itemlist = (KItemList*)&Player4->tmp[0x2040C];
		//KInventory* Inventory = (KInventory*)&itemlist->tmp[0x4C2C];
		//nCurrenMoney = Inventory[0].m_nMoney;
		//nCurrenMoney = (DWORD)&Player + 165956 * nPlayerIndex + 0x2503C + 4;
		//nCurrenMoney1 = (DWORD)&Player + 165956 * nPlayerIndex + 0x2503C + 4;

		//printf("5: %d - %d - %d - %d\n", (_DWORD)((char*)Player + 0x28844 * nPlayerIndex + 0x2040C), (_DWORD)*((char*)Player + 0x28844 * nPlayerIndex + 0x2040C4), *((char*)Player + 165956 * nPlayerIndex + 0x2503c), *((char*)Player + 165956 * nPlayerIndex + 0x2503c + 4));

		//if(!atoi((char*)&Player4 + 0x27C04))



		//nCurrenMoney = Player->tmp[0x2040C] + 0x4C2C + (5927 * nPlayerIndex) + 4;

		//KItemList* p_m_ItemList;

		//return false;
	}
	catch (...)
	{
		return false;
	}
}

int __cdecl SubAddSkillExp(KSkillList* This, KMagicAttrib* pAttrib, BOOL bPercent)
{
	int m_nNpcIndex;
	int nPlayerIdx;
	DWORD m_Kind;
	BYTE NpcIndex;
	BYTE pBuffer;
	//KNpc* Npc;
	DWORD* v6;
	KNpc* v4;

	m_nNpcIndex = (int)This->tmp[0x00];

	printf("0: %d\n", m_nNpcIndex);
	//printf("0: %d %d %d - %d %d\n", m_nNpcIndex, *(DWORD*)&This[0], &Npc[m_nNpcIndex].tmp[0x0], &Npc[m_nNpcIndex].tmp[0x4], &Npc[m_nNpcIndex].tmp[0x24]);
	//printf("0: %d %d %d - %d %d\n", *(BYTE*)&This[0], *(DWORD*)&This[0], &Npc[m_nNpcIndex].tmp[0x0], &Npc[m_nNpcIndex].tmp[0x4], &Npc[m_nNpcIndex].tmp[0x24]);
	

	if (m_nNpcIndex <= 0)
		return 0;


	v4 = (KNpc*)0x8320A00;
	//*(&Npc->m_Kind + 1609 * m_nNpcIndex)
	m_Kind = *(DWORD*)((char*)&v4->tmp[0x24] + 1609 * m_nNpcIndex);
	//m_Kind = (BYTE)v4[m_nNpcIndex].tmp[0x24];

	printf("2: %d\n", m_Kind);

	

	nPlayerIdx = Execute_KNpcGetPlayerIdx((KNpc*)((char*)v4 + 6436 * m_nNpcIndex));

	printf("3: %d\n", nPlayerIdx);

	if (nPlayerIdx)
	{
		return 0;
	}

	/*if (mKind == 1)
	{
	}*/
	//printf("1: %d %d %d %d %d\n", &Npc[m_nNpcIndex].tmp[0x24], &Npc[m_nNpcIndex].tmp[0x17E0], &v4->tmp[0x0], &v4->tmp[0x4], &v4->tmp[0x24]);

	return Execute_AddSkillExp(This, pAttrib, bPercent);
}

bool __cdecl KSkillListAddSkillExp(KNpc* This, int nLauncher, int nSeries, BOOL bIsMelee, void* pData, BOOL bUseAR, int nDoHurt, NPC_RELATION eRelation, int nSkillID)
{
	int m_nNpcIndex;
	int nPlayerIdx;
	int m_Kind;
	BYTE NpcIndex;
	BYTE pBuffer;
	KNpc* Npc;
	DWORD* v6;
	char* v4;

	//Npc = (KNpc*)((void*)0x8320A00 + 6436 * nLauncher);
	v4 = (char*)0x8320A00 + 6436 * nLauncher;

	//m_nNpcIndex = *(DWORD*)(char*)&This[0];
	//m_nNpcIndex = (BYTE)&This->tmp[0x0];
	//printf("pAttrib: %d %d %d %d\n", pAttrib->nAttribType, pAttrib->nValue[0], pAttrib->nValue[1], pAttrib->nValue[2]);
	//printf("0: %d %d %d\n", This->tmp[0x0], This->tmp[0x4], This->tmp[0x146C]);
	//printf("1: Level: %d Kind: %d Index: %d\n", This->tmp[0x20], This->tmp[0x24], nLauncher);
	

	//if (m_nNpcIndex <= 0)
	//	return 0;

	//cNpc = &Npc[m_nNpcIndex];
	
	//m_Kind = *(&Npc->tmp[9] + 1609 * m_nNpcIndex);
	//m_Kind = *(BYTE*)&cNpc->tmp[0x24];
	//m_Kind = *(&Npc->tmp[0x24] + 6436 * m_nNpcIndex);

	//printf("Npc[m_nNpcIndex].tmp[0x24]: %d %d %d %d %d %d\n", m_Kind, *(&Npc->tmp[0x13] + 1609 * m_nNpcIndex), *(&Npc->tmp[0x14] + 1609 * m_nNpcIndex), *(&Npc->tmp[0x15] + 1609 * m_nNpcIndex), *(&Npc->tmp[0x16] + 1609 * m_nNpcIndex), *(&Npc->tmp[0x17] + 1609 * m_nNpcIndex));
	//printf("Npc[m_nNpcIndex].tmp[0x24]: %u %u\n", *((_DWORD*)v4 + 9), *(&Npc->tmp[0x20] + 1609 * m_nNpcIndex));
	//printf("Npc[m_nNpcIndex].tmp[0x24]: %d %d %d\n", &cNpc[m_nNpcIndex].tmp[0x24], *(DWORD*)&cNpc[m_nNpcIndex].tmp[0x24], *(BYTE*)&cNpc[m_nNpcIndex].tmp[0x24]);
	//printf("Npc[m_nNpcIndex].tmp[0x24]: %d\n", m_Kind);
	/*
	if (*(DWORD*)&Npc[m_nNpcIndex].tmp[0x24] == 1)
	{
		if (*(DWORD*)&Npc[m_nNpcIndex].tmp[0x24] == 1)
			nPlayerIdx = *(DWORD*)&Npc[m_nNpcIndex].tmp[0x17E0];

		printf("nPlayerIdx: %d\n", nPlayerIdx);

		m_nNpcIndex = This->tmp[0x0];

		printf("m_nNpcIndex: %d\n", m_nNpcIndex);

	}

	v4 = &Npc[m_nNpcIndex];

	printf("v4->m_Kind: %d\n", *(DWORD*)&v4->tmp[0x24]);*/

	return Execute_ReceiveDamage(This, nLauncher, nSeries, bIsMelee, pData, bUseAR, nDoHurt, eRelation, nSkillID);
}

int __cdecl DisableFunctionLua(lua_State* L)
{
	try
	{
		return 0;
	}
	catch (...)
	{
		return 0;
	}
}

bool __cdecl SafeAcceptOrRefuseApplyToRelay(KPlayerTong* This, BOOL bAccept, char* pszName, int nPageIndex)
{
	if (gUtil.PACKETFILTER_ACTIVE)
	{
		if ((unsigned int)(This->tmp[0x24] - 1) <= 1198)
		{
			DWORD MemberCount;
			MemberCount = 1;
			Execute_LoadFromTongData(This);
			MemberCount = MemberCount + (unsigned int)(This->tmp[0x4D8]) + (unsigned int)(This->tmp[0x4DC]) + (unsigned int)(This->tmp[0x4E0]);
			//printf("Tong Master : %s [%s] Player : %s Member Count : %d / %d \n", (char*)This, (char*)((bAccept) ? "Accept" : "Refuse"), pszName, MemberCount, gUtil.nMaxTongMember);
			//std::cout << "Player : " << (char*)This->tmp[0x4DC] << " Join Tong : " << (char*)This << " Member Count : " << MemberCount << std::endl;
			//if (MemberCount >= reader.GetInteger("Overload", "MaxTongLimit", 120))
			if (MemberCount < gUtil.nMaxTongMember)
			{
				return Execute_AcceptOrRefuseApplyToRelay(This, bAccept, pszName, nPageIndex);
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

void __cdecl ProcessApplyJoinTongFromZhaoMu(KPlayerTong* This, DWORD dwTongID)
{
	try
	{
		if (gUtil.PACKETFILTER_ACTIVE)
		{
			if ((unsigned int)(This->tmp[0x24] - 1) <= 1198 && !This->tmp[0x2C])
			{
				DWORD MemberCount;
				KPlayer* v4;

				MemberCount = 1;

				/*INIReader reader("./servercfg.ini");

				if (reader.ParseError() < 0) {
					printf("Failed to load servercfg.ini\n");
					return;
				}*/

				//Execute_LoadFromTongData(This);

				MemberCount = MemberCount + (unsigned int)(This->tmp[0x4D8]) + (unsigned int)(This->tmp[0x4DC]) + (unsigned int)(This->tmp[0x4E0]);
				//printf("Player : %s Join Tong : %s Member Count : %d \n", (char*)This->tmp[0x4DC], (char*)This, MemberCount);
				//std::cout << "Player : " << (char*)This->tmp[0x4DC] << " Join Tong : " << (char*)This << " Member Count : " << MemberCount << std::endl;
				// 				if (MemberCount < gUtil.nMaxTongMember)
				if (MemberCount < gUtil.nMaxTongMember)
				{
					return Execute_ProcessApplyJoinTongFromZhaoMu(This, dwTongID);
				}
			}
			
		}
	}
	catch (...)
	{
		return Execute_ProcessApplyJoinTongFromZhaoMu(This, dwTongID);
	}
}

bool __cdecl CheckAddCondition(KPlayerTong* This, int nPlayerIdx)
{
	if (gUtil.PACKETFILTER_ACTIVE)
	{
		int MemberCount;
		//KPlayer* v4;

		//MemberCount = 1;
		//v4 = (KPlayer*)0x88A1A20;
		//MemberCount = Execute_GetMemberCount(15);

		/*INIReader reader("./servercfg.ini");

		if (reader.ParseError() < 0) {
			printf("Failed to load servercfg.ini\n");
			return 0;
		}*/

		Execute_LoadFromTongData(This);

		MemberCount = MemberCount + (unsigned int)(This->tmp[0x4D8]) + (unsigned int)(This->tmp[0x4DC]) + (unsigned int)(This->tmp[0x4E0]);

		//printf("Join Tong : %s, Member Count : %d / %d \n", (char*)This, MemberCount, gUtil.nMaxTongMember);

		//printf("MemberCount %d %d %d\n", This->tmp[0x4D8], This->tmp[0x4DC], This->tmp[0x4E0]);

		//printf("MemberCount %d %d %d\n", MemberCount, reader.GetInteger("Overload", "MaxTongLimit", 120), (MemberCount >= reader.GetInteger("Overload", "MaxTongLimit", 120)));

		//printf("m_szMasterName %s %d \n", (char*)This, nPlayerIdx);

		//int PlayerTongIndex = This->tmp[0x24];

		//Execute_ExecuteScript_Name(&v4[PlayerTongIndex], "\\script\\tong\\tong_mix.lua", "TongMemberCount", 1, "u", This->tmp[0x38]);

		//KPlayer::ExecuteScript(&Player[this->m_nPlayerIndex], "\\script\\global\\tongleavesuccess.lua", "OnLeaveTong", 0, 0);
		//return 0;

		//printf("ScriptResult %d %d \n", v4[PlayerTongIndex].tmp[0x27C00], (DWORD*)&v4[PlayerTongIndex].tmp[0x27C00]);

		//printf("TongNameID %u\n", This->tmp[0x38]);

		//if (This->tmp[0x2C])
		//	return This->tmp[0x38];

		//Execute_Msg2Player(&v4[PlayerTongIndex], "Limit");
		//if ((1 + (unsigned int)(This->tmp[0x4D8]) + (unsigned int)(This->tmp[0x4DC]) + (unsigned int)(This->tmp[0x4E0])) >= gUtil.nMaxTongMember)

		if (MemberCount >= gUtil.nMaxTongMember)
			return 0;
		else
			return Execute_CheckAddCondition(This, nPlayerIdx);
	}
	else
	{
		return Execute_CheckAddCondition(This, nPlayerIdx);
	}
}

void __cdecl OnJoinTong(KPlayerTong* This, DWORD dwTongID, DWORD dwOffer)
{
	int nResult;

	/*int MemberCount;

	MemberCount = 0;

	MemberCount = Execute_GetMemberCount(This, 15);

	printf("MemberCount %d %u\n", MemberCount, dwTongID);

	MemberCount = Execute_GetMemberCount(This, -1);

	printf("MemberCount %d %u\n", MemberCount, dwTongID);

	MemberCount = Execute_GetMemberCount(This, 1 | 2 | 4);

	printf("MemberCount %d %u\n", MemberCount, dwTongID);*/

	//nResult = Execute_RunSuffixScript(This, *((_DWORD*)pTongProtocolData + 1), *(_DWORD*)pTongProtocolData, "\\script\\tong\\tong_mix.lua", "TONG_ADDMEMBER_CHECK_ALLOW", "uu", *((_DWORD*)pTongProtocolData + 1), *((_DWORD*)pTongProtocolData + 2));

	////printf("nResult %d\n", nResult);

	//if (nResult)
	//{
	//	//printf("true\n");
	//	/*nResult = Execute_RunSuffixScript(
	//		This,
	//		*((_DWORD*)pTongProtocolData + 1),
	//		*(_DWORD*)pTongProtocolData,
	//		"\\script\\tong\\tong_mix.lua",
	//		"MEMBER_ADD",
	//		"uuu",
	//		*((_DWORD*)pTongProtocolData + 1),
	//		*((_DWORD*)pTongProtocolData + 2),
	//		*(_DWORD*)(pTongProtocolData + 54));*/
	//	return Execute_ProcessProtocol(This, eTongProtocolID, pTongProtocolData, dwTongProtocolSize);
	//}
	//else
	//{
	//	//printf("false\n");
	//	return 0;
	//}

	Execute_OnJoinTong(This, dwTongID, dwOffer);
		
}

bool __cdecl ProcessProtocolTongExProtoco(KTongLogic* This, TongExProtocol eTongProtocolID, BYTE* pTongProtocolData, _DWORD dwTongProtocolSize)
{

	int v11;

	//if (!pTongProtocolData)
	//	return 0;

	
//	if (eTongProtocolID == tp_member_add)
//	{
//		//v11 = *((_DWORD*)pTongProtocolData + 3);
//		//if (v11 + 16 != dwTongProtocolSize || v11 != 42)
//		//	return 0;
//
//		//DWORD dwTongNameID;
//		//int MemberCount;
//		//dwTongNameID = Execute_GetTongNameID();
//		//printf("dwTongID: %d\n", dwTongID);
//		//printf("szName: %s\n", szName);
//		//printf("dwLevel: %d\n", dwLevel);
//		//printf("dwMemberID: %d\n", dwMemberID);
//		//MemberCount = Execute_GetMemberCount(TongProtocolData + 1, -1);
//// 
//		//printf("dwTongID: %u\n", *((_DWORD*)pTongProtocolData + 1));
//		//printf("dwExecutorID: %u\n", *(_DWORD*)pTongProtocolData);
//		//printf("dwMemberID: %u\n", *((_DWORD*)pTongProtocolData + 2));
//
//		//return Execute_ProcessProtocol(This, eTongProtocolID, pTongProtocolData, dwTongProtocolSize);
//
//		int nResult;
//
//		nResult = Execute_RunSuffixScript(This, *((_DWORD*)pTongProtocolData + 1), *(_DWORD*)pTongProtocolData, "\\script\\tong\\tong_mix.lua", "TONG_ADDMEMBER_CHECK_ALLOW", "uu", *((_DWORD*)pTongProtocolData + 1), *((_DWORD*)pTongProtocolData + 2));
//		
//		//printf("nResult %d\n", nResult);
//
//		if (nResult)
//		{
//			//printf("true\n");
//			/*nResult = Execute_RunSuffixScript(
//				This,
//				*((_DWORD*)pTongProtocolData + 1),
//				*(_DWORD*)pTongProtocolData,
//				"\\script\\tong\\tong_mix.lua",
//				"MEMBER_ADD",
//				"uuu",
//				*((_DWORD*)pTongProtocolData + 1),
//				*((_DWORD*)pTongProtocolData + 2),
//				*(_DWORD*)(pTongProtocolData + 54));*/
//			return Execute_ProcessProtocol(This, eTongProtocolID, pTongProtocolData, dwTongProtocolSize);
//		}
//		else
//		{
//			//printf("false\n");
//			return 0;
//		}
//	}
	//else if (eTongProtocolID < tp_member_add)
	//{
	//	//printf("dwTongID: %u\n", *((_DWORD*)pTongProtocolData + 1));
	//	//printf("dwExecutorID: %u\n", *(_DWORD*)pTongProtocolData);
	//	//printf("dwMemberID: %u\n", *((_DWORD*)pTongProtocolData + 2));

	//	if (dwTongProtocolSize != 56)
	//		return 0;
	//	return (*((int(__cdecl**)(KTongLogic* const, _DWORD, _DWORD, _DWORD, _DWORD, BYTE*))This->tmp[0x00]
	//		+ 30))(
	//			This,
	//			*(_DWORD*)pTongProtocolData,
	//			*((_DWORD*)pTongProtocolData + 1),
	//			*((_DWORD*)pTongProtocolData + 12),
	//			*((_DWORD*)pTongProtocolData + 13),
	//			pTongProtocolData + 8);
	//}
	//printf("eTongProtocolID: %x\n", eTongProtocolID);
	//printf("dwTongProtocolSize: %d\n", dwTongProtocolSize);
	//printf("dwTongID: %u\n", *((_DWORD*)pTongProtocolData + 1));
	//printf("dwExecutorID: %u\n", *(_DWORD*)pTongProtocolData);
	//printf("dwMemberID: %u\n", *((_DWORD*)pTongProtocolData + 2));
	if (eTongProtocolID == tp_member_add)
	{
	}
	else
		return Execute_ProcessProtocol(This, eTongProtocolID, pTongProtocolData, dwTongProtocolSize);

}

void __cdecl BishopGatewaySmallPackProcess(KBishopProcess* This, const void* pData, size_t dataLength)
{
	
	unsigned __int8 v3;
	//KPlayer* v11;
	v3 = *(BYTE*)pData;
	//TRoleData* pRoleData;
	//pRoleData = (TRoleData*)((char*)pData + 109);
	//printf("Protocol: %d\n", v3);

#if(_DEBUG==1)
	printf("cProtocol: %d\n", v3);
	//printf("dwDataLen: %d\n", pRoleData->dwDataLen);

	//printf("HardwareId: %s\n", pRoleData->HardwareId);

	//printf("MacAddress: %s\n", pRoleData->MacAddress);
	//printf("IpAddress: %s\n", pRoleData->IpAddress);

	//printf("LicenseId: %s\n", pRoleData->LicenseId);
#endif

	if(v3 < 211)
		Execute_GatewaySmallPackProcess(This, pData, dataLength);
	else
	{
		//printf("Work!\n");

		if (v3 == 211)
		{
			try
			{
				int m_PlayerIndex;
#if(_DEBUG==1)
				//printf("cProtocol: %d\n", v3);
				printf("AccName: %s\n", pData + 1);
				printf("HardwareId: %s\n", pData + 33);
				//printf("LicenseId: %s\n", pData + 65);
#endif
			/*tagEnterGame eg;
			eg.cProtocol = 52;
			strncpy((char*)eg.szAccountName, (LPCSTR)pData + 1, 0x1Fu);
			eg.szAccountName[31] = 0;

			Execute_ProcessMessage(g_pCoreServerShell->m_pServer, 1, &eg, 34);*/

				if (Execute_FindAccount((KPlayerSet*)0x88A1EE0, (LPCSTR)pData + 1, &m_PlayerIndex))
				{
					//printf("AccName: %s\n", pData + 1);
					//printf("PlayerIndex: %d\n", m_PlayerIndex);
					if (m_PlayerIndex > 0)
					{
						if (strlen((char*)pData + 33) > 0)
						{
							memset(&m_KPlayerInfo[m_PlayerIndex].m_Hwid, 0, 32);
							memcpy(&m_KPlayerInfo[m_PlayerIndex].m_Hwid, (char*)pData + 33, sizeof(m_KPlayerInfo[m_PlayerIndex].m_Hwid));
						}

						/*if (strlen((char*)pData + 70) > 0)
						{
							memset(&m_KPlayerInfo[m_PlayerIndex].m_Mac, 0, 18);
							memcpy(&m_KPlayerInfo[m_PlayerIndex].m_Mac, (char*)pData + 70, sizeof(m_KPlayerInfo[m_PlayerIndex].m_Mac));
						}

						if (strlen((char*)pData + 88) > 0)
						{
							memset(&m_KPlayerInfo[m_PlayerIndex].m_LanIP, 0, 16);
							memcpy(&m_KPlayerInfo[m_PlayerIndex].m_LanIP, (char*)pData + 88, sizeof(m_KPlayerInfo[m_PlayerIndex].m_LanIP));
						}*/

						//m_KPlayerInfo[m_PlayerIndex].m_Hwid[36] = 0;

						//m_KPlayerInfo[m_PlayerIndex].m_Mac[18] = 0;

						//m_KPlayerInfo[m_PlayerIndex].m_LanIP[16] = 0;

						//printf("Hwid: %s\n", m_KPlayerInfo[m_PlayerIndex].m_Hwid);
						//printf("Mac: %s\n", m_KPlayerInfo[m_PlayerIndex].m_Mac);
						//printf("LanIP: %s\n", m_KPlayerInfo[m_PlayerIndex].m_LanIP);
						//memcpy(&m_KPlayerInfo[m_PlayerIndex].m_LicenseId, (char*)pData + 65, sizeof(m_KPlayerInfo[m_PlayerIndex].m_LicenseId));
						//printf("HardwareId: %s\n", m_KPlayerInfo[m_PlayerIndex].m_Hwid);
					}
				}
			}
			catch (...)
			{

			}
		}
		if (v3 == 212)
		{
			if (*(BYTE*)((char*)pData + 5) == 1)
			{
				gUtil.nLimit = *(int*)((char*)pData + 6);
				printf(UPDATE_LIMIT_GAME.c_str(), gUtil.nLimit);
			}
			else if (*(BYTE*)((char*)pData + 5) == 2)
			{
				gUtil.nLimitIP = *(int*)((char*)pData + 6);
				printf(UPDATE_LIMIT_IP.c_str(), gUtil.nLimitIP);
			}
			else if (*(BYTE*)((char*)pData + 5) == 3)
			{
				g_Firewall.InsertIpAddress((char*)pData + 6, false, true);
			}
		}
	}

}

void __cdecl BishopGatewayLargePackProcess(KBishopProcess* This, const void* pData, size_t dataLength)
{

	TRoleData* pRoleData;
	pRoleData = (TRoleData*)((char*)pData + 109);
	//printf("dataLength: %d\n", dataLength);
	//printf("dwDataLen: %d\n", pRoleData->dwDataLen);

	//printf("HardwareId: %s\n", pRoleData->HardwareId);

	//printf("MacAddress: %s\n", pRoleData->MacAddress);
	//printf("IpAddress: %s\n", pRoleData->IpAddress);

	//printf("LicenseId: %s\n", pRoleData->LicenseId);

	Execute_BishopGatewayLargePackProcess(This, pData, dataLength);

}

void __cdecl HostProcessTransfer(KHostProcess* This, const void* pData, size_t nLen, KTRAN_UNIT* pUnit)
{
	TRoleData* pRoleData;
	/*int result;
	KFileSyncCenterGS* v4;
	int v5;
	int bOnlyForGS;
	_DWORD dwFileListID;
	KPlayer* v8;
	int nSrcIndex;*/
	int v4 = *(_DWORD*)((char*)pData + 105);

	//printf("v4: %d\n", v4);
	//printf("v4: %d\n", v4 + 109);
	//printf("v4: %d\n", *((_DWORD*)pData + 128));
	//0x17D;
	pRoleData = (TRoleData*)((char*)pData + 109);

	//strcpy(HardwareId, pRoleData->HardwareId);
	//strcpy(LicenseId, pRoleData->LicenseId);
	//strcpy(MacAddress, pRoleData->MacAddress);
	//strcpy(IpAddress, pRoleData->IpAddress);

	//printf("nTotalLeftTime: %d\n", pAccountInfo->nTotalLeftTime);
	//printf("dwVersion: %d\n", pRoleData->dwVersion);

	//printf("HardwareId: %s\n", m_KPlayerInfo[pRoleData->BaseInfo.dwId].m_Hwid);
	//printf("IpAddress: %s\n", m_KPlayerInfo[pRoleData->BaseInfo.dwId].m_LanIP);
	//printf("MacAddress: %s\n", m_KPlayerInfo[pRoleData->BaseInfo.dwId].m_Mac);
	//printf("LicenseId: %s\n", m_KPlayerInfo[pRoleData->BaseInfo.dwId].m_LicenseId);

	//printf("nLen: %d\n", nLen);
	//printf("szName: %s\n", pRoleData->BaseInfo.szName);
	//printf("dwDataLen: %d\n", pRoleData->dwDataLen);
	//pRoleData->dwDataLen = nLen;
	//printf("HardwareId: %s\n", pRoleData->HardwareId);

	//printf("MacAddress: %s\n", pRoleData->MacAddress);
	//printf("IpAddress: %s\n", pRoleData->IpAddress);
	
	//printf("LicenseId: %s\n", pRoleData->LicenseId);

	Execute_ProcessTransfer(This, pData, nLen, pUnit);

}

bool __cdecl SafeLoadDBPlayerInfo(KPlayer* This, BYTE* pPlayerInfo, int* nStep, unsigned int* nParam)
{
	//printf("nStep: %d\n", *nStep);

	if (*nStep == 0)
	{
		try
		{
			TRoleBaseInfo* pBaseInfo = (TRoleBaseInfo*)(pPlayerInfo + 4);

			//printf("szName: %s\n", pBaseInfo->szName);
			//printf("caccname: %s\n", pBaseInfo->caccname);
			//printf("imoney: %d\n", pBaseInfo->imoney);
			//printf("iliveexp: %d\n", pBaseInfo->iliveexp);
			//printf("nExp: %d\n", This->tmp[0x25C70]);
			//printf("nNextLevelExp: %d\n", This->tmp[0x25C78]);
			//printf("nWorldStat: %d\n", pBaseInfo->nWorldStat);

			int nWorldStat = Sqlite_GetHardwareIdByAccount((const char*)&pBaseInfo->caccname);

			//printf("nWorldStat: %d\n", nWorldStat);

			if (nWorldStat > 0)
			{
				gUtil.SetInt((DWORD)&pBaseInfo->nWorldStat, nWorldStat);
			}
		}
		catch (...)
		{
		}
		
		return Execute_LoadDBPlayerInfo(This, pPlayerInfo, nStep, nParam);
	}
	else
		return Execute_LoadDBPlayerInfo(This, pPlayerInfo, nStep, nParam);
}

bool __cdecl SafeOnEnterNewMap(KPlayer* This)
{
	bool result = Execute_OnEnterNewMap(This);
	int nFaction = *(int*)(&This->tmp[0x25CE0] + 8);
	int nPlayerIndex = *(int*)(&This->tmp[0xC0]);

	if (m_KPlayerInfo[nPlayerIndex].m_AllowLimit == 0)
	{
		//printf("[%d] GetFaction: %d, limit: %d\n", result, nFacetion, gUtil.FactionLimit[nFacetion]);
		if (nFaction >= 0 && nFaction < 10)
		{
			if (gUtil.FactionLimit[nFaction] > 0)
			{
				//printf("GetFaction: %d, limit: %d\n", nFaction, gUtil.FactionLimit[nFaction]);
				if (!Sqlite_CheckFaction_IpAddress(m_KPlayerInfo[nPlayerIndex].m_LanIP, nFaction, gUtil.FactionLimit[nFaction]))
				{
					//if (gUtil.nTimerID > 0)
					//{
						
						//char szAction[256];
						//memset(szAction, 0, 256);
						char* szAction = (char*)malloc(256);
						sprintf(szAction, "Say([[M«n ph¸i nµy bÞ giíi h¹n %d tµi kho¶n mçi IP]], 0)", gUtil.FactionLimit[nFaction]);
						Execute_DoScript(This, szAction);
						free(szAction);

						Execute_ExecuteScript(This, "\\script\\lib\\kickplayer.lua", "KickOutPlayer", 0, 0u);

						/*KScriptTimer* scriptTimer;
						ITimer* iTimer;
						scriptTimer = KScriptTimer_Create();
						iTimer = scriptTimer;
						DWORD scriptid = Execute_FileName2Id((LPCSTR*)"\\script\\lib\\kickplayer.lua");
						KScriptTimer_SetCallBack(scriptTimer, scriptid, "OnTimer", nPlayerIndex);
						KServerCore_AddTimer((KServerCore*)(0x82B8830), iTimer, 5 * 18);*/
						
						//char* szAction = (char*)malloc(256);
						//sprintf(szAction, "Say([[M«n ph¸i nµy bÞ giíi h¹n %d tµi kho¶n mçi IP]], 0)", gUtil.FactionLimit[m_KPlayerInfo[nPlayerIndex].m_Faction]);
						//Execute_DoScript(This, szAction);
						//free(szAction);
						/*"SetTimer(54, %d);"
							"ForbitStamina(1);"
							"ForbidEnmity(1);"
							"SetChatFlag(1);"*/

						//char szTimer[32];
						//memset(szTimer, 0, 32);
						////char* szTimer = (char*)malloc(64);
						////memset(szAction, 0, 256);
						//sprintf(szTimer, "SetTimer(54, %d)", gUtil.nTimerID);
						//Execute_DoScript(This, szTimer);
						////free(szTimer);
						////return false;
					//}
					//else
					//{
					//	Execute_OfflineLive(This);
					//	Execute_KickOut(This, 6);
					//	//return false;
					//}
					//gUtil.SetByte((DWORD)&This->tmp[0x287B0], 1);
					//printf("GetFaction: %d\n", m_KPlayerInfo[nPlayerIndex].m_Faction);
					//printf("WanIpAddress: %s\n", &AllHardwareId[32]);



					//Execute_DoScript((KPlayer*)&CurrenPlayer[nPlayerIndex].tmp[165956], szAction);

					//Execute_OfflineDestroy((KServerCore*)0x82B8834, pPlayerIndex, 0);
					//Execute_CleanConnectionStatus(This);
					//Execute_ShutdownPlayer(This);
					//return 0;
				}
			}
		}

		//printf("IP Limit: %d\n", gUtil.nLimitIP);
		if (gUtil.nLimitIP > 0)
		{
			if (strlen(m_KPlayerInfo[nPlayerIndex].m_LanIP) > 0)
			{
				if (!Sqlite_Check_IpAddress(m_KPlayerInfo[nPlayerIndex].m_LanIP))
				{
					//if (gUtil.nTimerID > 0)
					//{
						//char szAction[256];
						//memset(szAction, 0, 256);
						char* szAction = (char*)malloc(256);
						sprintf(szAction, "Say([[Mçi ®Þa chØ IP chØ ®­îc truy cËp %d tµi kho¶n]], 0);", gUtil.nLimitIP);
						Execute_DoScript(This, szAction);
						free(szAction);

						Execute_ExecuteScript(This, "\\script\\lib\\kickplayer.lua", "KickOutPlayer", 0, 0u);

						/*KScriptTimer* scriptTimer;
						ITimer* iTimer;
						scriptTimer = KScriptTimer_Create();
						iTimer = scriptTimer;
						DWORD scriptid = Execute_FileName2Id((LPCSTR*)"\\script\\lib\\kickplayer.lua");
						KScriptTimer_SetCallBack(scriptTimer, scriptid, "OnTimer", nPlayerIndex);
						KServerCore_AddTimer((KServerCore*)(0x82B8830), iTimer, 5 * 18);*/

						//char* szAction = (char*)malloc(256);
						////char szAction[256];
						////memset(szAction, 0, 256);
						//sprintf(szAction, "Say([[Mçi ®Þa chØ IP chØ ®­îc truy cËp %d tµi kho¶n]], 0)", gUtil.nLimitIP);
						//Execute_DoScript(This, szAction);
						//free(szAction);

						/*"SetTimer(54, %d);"
							"ForbitStamina(1); "
							"ForbidEnmity(1);"
							"SetChatFlag(1);"*/

						//char szTimer[32];
						//memset(szTimer, 0, 32);
						////char* szTimer = (char*)malloc(32);
						////memset(szTimer, 0, 32);
						//sprintf(szTimer, "SetTimer(54, %d)", gUtil.nTimerID);
						//Execute_DoScript(This, szTimer);
						////free(szTimer);
						////return false;
					//}
					//else
					//{
					//	Execute_OfflineLive(This);
					//	Execute_KickOut(This, 6);
					//	//return false;
					//}

					//sleep(3);
					//Execute_OfflineLive((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer);
					//Execute_KickOut((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, 6);
				}
			}
		}

	}
	return result;
}

bool __cdecl SafeProcessSyncReplyProtocol(KClientProcess* This, const unsigned int lnID, int nPlayerIndex, const char* pData, size_t dataLength)
{
	bool result = Execute_ProcessSyncReplyProtocol(This, lnID, nPlayerIndex, pData, dataLength);
	//KPlayer* CurrenPlayer = (KPlayer*)Player_Struct; // (KPlayer *const)&CurrenPlayer[165956 * nIndex]
	//printf("IP Limit: %d\n", gUtil.nLimitIP);
	//printf("AllowLimit: %d\n", m_KPlayerInfo[nPlayerIndex].m_AllowLimit);
	//printf("GetFaction: %d, limit: %d\n", m_KPlayerInfo[nPlayerIndex].m_Faction, gUtil.FactionLimit[m_KPlayerInfo[nPlayerIndex].m_Faction]);
	if (m_KPlayerInfo[nPlayerIndex].m_AllowLimit == 0)
	{
		if (m_KPlayerInfo[nPlayerIndex].m_Faction >= 0 && m_KPlayerInfo[nPlayerIndex].m_Faction < 10)
		{
			if (gUtil.FactionLimit[m_KPlayerInfo[nPlayerIndex].m_Faction] > 0)
			{
				//if (!Sqlite_CheckFaction(m_KPlayerInfo[nPlayerIndex].m_AccountName, m_KPlayerInfo[nPlayerIndex].m_LanIP, m_KPlayerInfo[nPlayerIndex].m_Faction, gUtil.FactionLimit[m_KPlayerInfo[nPlayerIndex].m_Faction]))
				if (!Sqlite_CheckFaction_IpAddress(m_KPlayerInfo[nPlayerIndex].m_LanIP, m_KPlayerInfo[nPlayerIndex].m_Faction, gUtil.FactionLimit[m_KPlayerInfo[nPlayerIndex].m_Faction]))
				{
					if (m_KPlayerInfo[nPlayerIndex].m_KPlayer != NULL)
					{
						//char szAction[256];
						//char szFactionName[20];
						//memset(szAction, 0, 256);
						/*memset(szFactionName, 0, 256);

						memcpy(szFactionName, "V« hÖ", sizeof("V« hÖ"));

						int nIdFaction = m_KPlayerInfo[nPlayerIndex].m_Faction;

						if (nIdFaction == 0) {
							memcpy(szFactionName, "ThiÕuÙl©m", sizeof("ThiÕuÙl©m"));
						}
						else if (nIdFaction == 1) {
							memcpy(szFactionName, "ThiªnÙv­¬ng", sizeof("ThiªnÙv­¬ng"));
						}
						else if (nIdFaction == 2) {
							memcpy(szFactionName, "§­êngÙm«n", sizeof("§­êngÙm«n"));
						}
						else if (nIdFaction == 3) {
							memcpy(szFactionName, "NgòÙ®éc", sizeof("NgòÙ®éc"));
						}
						else if (nIdFaction == 4) {
							memcpy(szFactionName, "NgaÙmi", sizeof("NgaÙmi"));
						}
						else if (nIdFaction == 5) {
							memcpy(szFactionName, "ThóyÙyªn", sizeof("ThóyÙyªn"));
						}
						else if (nIdFaction == 6) {
							memcpy(szFactionName, "C¸iÙbang", sizeof("C¸iÙbang"));
						}
						else if (nIdFaction == 7) {
							memcpy(szFactionName, "ThiªnÙnhÉn", sizeof("ThiªnÙnhÉn"));
						}
						else if (nIdFaction == 8) {
							memcpy(szFactionName, "VâÙ®ang", sizeof("VâÙ®ang"));
						}
						else if (nIdFaction == 9) {
							memcpy(szFactionName, "C«nÙl«n", sizeof("C«nÙl«n"));
						}
						szFactionName[sizeof(szFactionName) - 1] = 0;*/
						//0x824A5E0
						//Execute_ExecuteScript((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, "\\script\\lib\\kickplayer.lua", "SetKickTimer", 0, 1u);
						//Execute_OfflineQuit((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, false);
						 //Execute_ShutdownPlayer((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer);
						//Execute_KickOut((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, 6);
						//printf("GetFaction: %d, limit: %d\n", m_KPlayerInfo[nPlayerIndex].m_Faction, gUtil.FactionLimit[m_KPlayerInfo[nPlayerIndex].m_Faction]);
						//if (gUtil.nTimerID > 0)
						//{
							char* szAction = (char*)malloc(256);
							sprintf(szAction, "Say([[M«n ph¸i nµy bÞ giíi h¹n %d tµi kho¶n mçi IP]], 0)", gUtil.FactionLimit[m_KPlayerInfo[nPlayerIndex].m_Faction]);
							Execute_DoScript((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, szAction);
							free(szAction);

							Execute_ExecuteScript((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, "\\script\\lib\\kickplayer.lua", "KickOutPlayer", 0, 0u);

							//char* szTimer = (char*)malloc(64);
							////memset(szAction, 0, 256);
							//sprintf(szTimer, "SetTimer(54, %d)", gUtil.nTimerID);
							//Execute_DoScript((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, szTimer);
							//free(szTimer);
						//}
						//else
						//{
							//Execute_OfflineLive((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer);
							//Execute_KickOut((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, 6);
						//}

						return result;
						//gUtil.SetByte((DWORD)&This->tmp[0x287B0], 1);
						//printf("GetFaction: %d\n", m_KPlayerInfo[nPlayerIndex].m_Faction);
						//printf("WanIpAddress: %s\n", &AllHardwareId[32]);



						//Execute_DoScript((KPlayer*)&CurrenPlayer[nPlayerIndex].tmp[165956], szAction);

						//Execute_OfflineDestroy((KServerCore*)0x82B8834, pPlayerIndex, 0);
						//Execute_CleanConnectionStatus(This);
						//Execute_ShutdownPlayer(This);
						//return 0;
					}
				}
			}

			if (gUtil.nLimitIP > 0)
			{
				if (strlen(m_KPlayerInfo[nPlayerIndex].m_LanIP) > 0)
				{
					if (!Sqlite_Check_IpAddress(m_KPlayerInfo[nPlayerIndex].m_LanIP))
					{
						if (m_KPlayerInfo[nPlayerIndex].m_KPlayer != NULL)
						{


							//if (gUtil.nTimerID > 0)
							//{
								char* szAction = (char*)malloc(256);
								//char szAction[256];
								//memset(szAction, 0, 256);
								sprintf(szAction, "Say([[Mçi ®Þa chØ IP chØ ®­îc truy cËp %d tµi kho¶n]], 0)", gUtil.nLimitIP);
								Execute_DoScript((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, szAction);
								free(szAction);

								Execute_ExecuteScript((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, "\\script\\lib\\kickplayer.lua", "KickOutPlayer", 0, 0u);

								//char* szTimer = (char*)malloc(64);
								////memset(szTimer, 0, 256);
								//sprintf(szTimer, "SetTimer(54, %d)", gUtil.nTimerID);
								//Execute_DoScript((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, szTimer);
								//free(szTimer);
							//}
							//else
							//{
								//Execute_OfflineLive((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer);
								//Execute_KickOut((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, 6);
							//}
							return result;
						}
						//sleep(3);
						//Execute_OfflineLive((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer);
						//Execute_KickOut((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, 6);
					}
				}
			}

		}

		//printf("IP Limit: %d\n", gUtil.nLimitIP);
		
	}

	return result;
}

void __cdecl SafeOnLogin(KPlayer* This)
{
	if (gUtil.PACKETFILTER_ACTIVE)
	{
		//Execute_OnLogin(This);
		int pPlayerIndex;
		int nLevel;
		INT64 nPlayerExp;
		int nWorldStat;
		int nSectStat;
		//INT64 m_nNextLevelExp;
		//int nPlayerID;
		//KNpc* pKNpc;
		const char* pAccName;
		const char* pRoleName;
		const char* pSaveBuffer;
		bool  nIsLimited = false;
		int  nHwidCount = 0;
		int nNetConnectIdx = -1;
		int m_nCurFaction = -1;
		//KPlayer* Player = (KPlayer*)((char*)0x88A1A20 + 165956 * nIndex);
		/*if (This->tmp[0x20408] != -1 && This->tmp[0x20400] && This->tmp[0x20404] > 0)
		{

		}*/

		pPlayerIndex = *(int*)(&This->tmp[0xC0]);


		if (pPlayerIndex > 0)
		{
			try
			{
				pAccName = &This->tmp[0x264];
				pRoleName = &This->tmp[0x25C88];
				nNetConnectIdx = *(int*)(&This->tmp[0x20408]);
				//m_nCurFaction = *(int*)(&This->tmp[0x25CE0] + 8);
				//nWorldStat = *(int*)(&This->tmp[0x28004]);
				//nSectStat = *(int*)(&This->tmp[0x28008]);

				char AllHardwareId[72];
				memset(&AllHardwareId, 0, 72);
				//printf("SafeOnLogin pAccName: %s\n", pAccName);
				nHwidCount = Sqlite_GetAndInsert_HardwareId_On_EnterGame(pAccName, (const void*)AllHardwareId, 3);
				AllHardwareId[sizeof(AllHardwareId) - 1] = 0;
				//printf("SafeOnLogin HwidCount: %d\n", nHwidCount);
				//printf("SafeOnLogin NetConnectIdx: %d\n", nNetConnectIdx);
				//const char* szIP = Execute_GetIP((IServer*)((char*)0x82B8824), *((DWORD*)&This[165956 * pPlayerIndex + 132104]));
				/*char szDesMsg[200];
				const char* szIP = (const char*)(*(int(__cdecl**)(IServer*, DWORD))(*(DWORD*)((char*)0x82B8824) + 52))((IServer*)((char*)0x82B8824), *(DWORD*)&This->tmp[132104]);
				strcpy(szDesMsg, szIP);
				printf("SafeOnLogin NetConnectIdx: %s\n", szDesMsg);*/

				//printf("GetFaction: %d\n", *(int*)(&This->tmp[0x25CE0] + 8));
				//printf("WanIpAddress: %s\n", &AllHardwareId[32]);
				m_KPlayerInfo[pPlayerIndex].m_KPlayer = (DWORD*)This;
				m_KPlayerInfo[pPlayerIndex].m_AutoCheck = false;
				memset(&m_KPlayerInfo[pPlayerIndex].m_AccountName, 0, 32);
				memcpy(&m_KPlayerInfo[pPlayerIndex].m_AccountName, pAccName, sizeof(m_KPlayerInfo[pPlayerIndex].m_AccountName));
				m_KPlayerInfo[pPlayerIndex].m_AccountName[31] = 0;

				memset(&m_KPlayerInfo[pPlayerIndex].m_PlayerName, 0, 32);
				memcpy(&m_KPlayerInfo[pPlayerIndex].m_PlayerName, pRoleName, sizeof(m_KPlayerInfo[pPlayerIndex].m_PlayerName));
				m_KPlayerInfo[pPlayerIndex].m_PlayerName[31] = 0;

				m_KPlayerInfo[pPlayerIndex].m_TickCount = 0LL;
				m_KPlayerInfo[pPlayerIndex].m_NPCTalkFlood = 0;

				m_KPlayerInfo[pPlayerIndex].m_ChatTick = 0LL;
				m_KPlayerInfo[pPlayerIndex].m_ChatFlood = 0;

				m_StallCheck[pPlayerIndex].m_nPlayerIndex = pPlayerIndex;
				m_StallCheck[pPlayerIndex].m_nProtocol = 0;
				m_StallCheck[pPlayerIndex].m_nError = false;

				m_KPlayerInfo[pPlayerIndex].m_StallCoin = 0;
				m_KPlayerInfo[pPlayerIndex].m_SBuyType = 0;

				m_KPlayerInfo[pPlayerIndex].m_Faction = *(int*)(&This->tmp[0x25CE0] + 8);

				nPlayerExp = *(INT64*)(&This->tmp[0x25C70]);

				Sqlite_UpdateRoleInfo_On_EnterGame(pAccName, pRoleName, 0, nPlayerExp);

				if (nHwidCount > 0)
				{
					//pKNpc = (KNpc*)0x8320A00;

					//nPlayerID = *(int*)(&This->tmp[0x20404]);

					//nLevel = *(double*)(&pKNpc->tmp[0x20] + 1609 * nPlayerID);

					//printf("Data: %d %d\n", nLevel, *(&This->tmp[0x20404]));

					//m_nNextLevelExp = *(int*)(&This->tmp[0x25C78]);

					//pSaveBuffer = (char*)(&This->tmp[0x284]);

					//printf("nPlayerExp: %lld\n", nPlayerExp);

					//nWorldStat = (int)(AllHardwareId[51]);

					//printf("nWorldStat: %d , %d\n", AllHardwareId[51], nWorldStat);



					//Sqlite_Sort_Top_Data();

					/*if (nWorldStat > 0)
					{
						TRoleBaseInfo* pBaseInfo = (TRoleBaseInfo*)(pSaveBuffer + 4);

						gUtil.SetInt((DWORD)&pBaseInfo->nWorldStat, nWorldStat);

						gUtil.SetInt((DWORD)&This->tmp[0x28004], nWorldStat);
					}*/

#if(_DEBUG==1)
					printf("HardwareId: %s\n", AllHardwareId);
					printf("WanIpAddress: %s\n", &AllHardwareId[32]);
					printf("AllowLimit: %s\n", &AllHardwareId[49]);
					printf("Faction: %d\n", *(int*)(&This->tmp[0x25CE0]));
#endif

					//memcpy(m_KPlayerInfo[pPlayerIndex].m_Guid, (char*)&This->tmp[0x203E4] + 165956 * pPlayerIndex, 16);


					/*if (m_KPlayerInfo[pPlayerIndex].m_Faction >= 0 && m_KPlayerInfo[pPlayerIndex].m_Faction < 10)
					{
						memcpy(&m_KPlayerInfo[pPlayerIndex].m_FactionName, gUtil.GetFactionName(m_KPlayerInfo[pPlayerIndex].m_Faction), sizeof(m_KPlayerInfo[pPlayerIndex].m_FactionName));
					}*/

					//printf("HardwareId Len: %d\n", strlen(AllHardwareId));
					if (strlen(AllHardwareId) > 0)
					{
						memset(&m_KPlayerInfo[pPlayerIndex].m_Hwid, 0, 32);
						memcpy(&m_KPlayerInfo[pPlayerIndex].m_Hwid, AllHardwareId, sizeof(m_KPlayerInfo[pPlayerIndex].m_Hwid));
						m_KPlayerInfo[pPlayerIndex].m_Hwid[31] = 0;
						//printf("m_AllowLimit: %s\n", &AllHardwareId[49]);
						m_KPlayerInfo[pPlayerIndex].m_AllowLimit = (memcmp(&AllHardwareId[49], "1", 1) == 0);
					}
					//else
					//{
					//	Sqlite_BackNumber_On_Kichout_ByLimit(AllHardwareId, &AllHardwareId[33], &AllHardwareId[51]);
					//	Execute_KickOut(This, 5);
					//	Execute_ShutdownPlayer(This);
					//}

					/*if (strlen(&AllHardwareId[33]) > 0)
					{
						memset(&m_KPlayerInfo[pPlayerIndex].m_Mac, 0, 18);
						memcpy(&m_KPlayerInfo[pPlayerIndex].m_Mac, &AllHardwareId[33], sizeof(m_KPlayerInfo[pPlayerIndex].m_Mac));
					}*/

					if (strlen(&AllHardwareId[32]) > 0)
					{
						memset(&m_KPlayerInfo[pPlayerIndex].m_LanIP, 0, 17);
						//memcpy(&m_KPlayerInfo[pPlayerIndex].m_LanIP, &AllHardwareId[32], sizeof(m_KPlayerInfo[pPlayerIndex].m_LanIP) - 1);
						strcpy(m_KPlayerInfo[pPlayerIndex].m_LanIP, &AllHardwareId[32]);
						m_KPlayerInfo[pPlayerIndex].m_LanIP[16] = 0;
						Sqlite_Insert_IpAddr_JoinGS(pAccName, m_KPlayerInfo[pPlayerIndex].m_LanIP);
					}

					//Execute_OnLogin(This);

				}

#if(_DEBUG==1)
				printf("WanIpAddress: %s\n", m_KPlayerInfo[pPlayerIndex].m_LanIP);
#endif
			}
			catch (...)
			{
			}

			if (gUtil.nLoginToAuthen > 0)
			{
				Execute_ExecuteScript(This, "\\script\\lib\\jxguard.lua", "AcctionLog2Auth", 0, 0u);
			}

			Execute_OnLogin(This);

			try
			{
				
				//printf("IP Limit: %d\n", gUtil.nLimitIP);
				/*printf("AllowLimit: %d\n", m_KPlayerInfo[pPlayerIndex].m_AllowLimit);
				printf("GetFactionLogin: %d, limit: %d, limit 2: %d\n", m_KPlayerInfo[pPlayerIndex].m_Faction, gUtil.FactionLimit[m_KPlayerInfo[pPlayerIndex].m_Faction], gUtil.FactionLimit[*(int*)(&This->tmp[0x25CE0] + 8)]);

				for (int i = 0; i < 10; ++i)
				{
					printf("Mon Phai Limit: %d\n", gUtil.FactionLimit[i]);
				}*/

				if (m_KPlayerInfo[pPlayerIndex].m_AllowLimit == 0)
				{

					if (*(int*)(&This->tmp[0x25CE0] + 8) >= 0 && *(int*)(&This->tmp[0x25CE0] + 8) < 10)
					{
						if (gUtil.FactionLimit[*(int*)(&This->tmp[0x25CE0] + 8)] > 0)
						{
							if (!Sqlite_CheckFaction(m_KPlayerInfo[pPlayerIndex].m_AccountName, m_KPlayerInfo[pPlayerIndex].m_LanIP, *(int*)(&This->tmp[0x25CE0] + 8), gUtil.FactionLimit[*(int*)(&This->tmp[0x25CE0] + 8)]))
							{
								//char szAction[256];
									//char szFactionName[20];
									//memset(szAction, 0, 256);
									/*memset(szFactionName, 0, 256);

									memcpy(szFactionName, "V« hÖ", sizeof("V« hÖ"));

									int nIdFaction = m_KPlayerInfo[nPlayerIndex].m_Faction;

									if (nIdFaction == 0) {
										memcpy(szFactionName, "ThiÕuÙl©m", sizeof("ThiÕuÙl©m"));
									}
									else if (nIdFaction == 1) {
										memcpy(szFactionName, "ThiªnÙv­¬ng", sizeof("ThiªnÙv­¬ng"));
									}
									else if (nIdFaction == 2) {
										memcpy(szFactionName, "§­êngÙm«n", sizeof("§­êngÙm«n"));
									}
									else if (nIdFaction == 3) {
										memcpy(szFactionName, "NgòÙ®éc", sizeof("NgòÙ®éc"));
									}
									else if (nIdFaction == 4) {
										memcpy(szFactionName, "NgaÙmi", sizeof("NgaÙmi"));
									}
									else if (nIdFaction == 5) {
										memcpy(szFactionName, "ThóyÙyªn", sizeof("ThóyÙyªn"));
									}
									else if (nIdFaction == 6) {
										memcpy(szFactionName, "C¸iÙbang", sizeof("C¸iÙbang"));
									}
									else if (nIdFaction == 7) {
										memcpy(szFactionName, "ThiªnÙnhÉn", sizeof("ThiªnÙnhÉn"));
									}
									else if (nIdFaction == 8) {
										memcpy(szFactionName, "VâÙ®ang", sizeof("VâÙ®ang"));
									}
									else if (nIdFaction == 9) {
										memcpy(szFactionName, "C«nÙl«n", sizeof("C«nÙl«n"));
									}

									szFactionName[sizeof(szFactionName) - 1] = 0;*/



								//if (gUtil.nTimerID > 0)
								//{
									//char szAction[256];
									//memset(szAction, 0, 256);
									char* szAction = (char*)malloc(256);
									sprintf(szAction, "Say([[M«n ph¸i nµy bÞ giíi h¹n %d tµi kho¶n mçi IP]], 0)", gUtil.FactionLimit[*(int*)(&This->tmp[0x25CE0] + 8)]);
									Execute_DoScript(This, szAction);
									free(szAction);

									Execute_ExecuteScript(This, "\\script\\lib\\kickplayer.lua", "KickOutPlayer", 0, 0u);

									/*KScriptTimer* scriptTimer;
									ITimer* iTimer;
									scriptTimer = KScriptTimer_Create();
									iTimer = scriptTimer;
									DWORD scriptid = Execute_FileName2Id((LPCSTR*)"\\script\\lib\\kickplayer.lua");
									KScriptTimer_SetCallBack(scriptTimer, scriptid, "OnTimer", pPlayerIndex);
									KServerCore_AddTimer((KServerCore*)(0x82B8830), iTimer, 5 * 18);*/

									/*char* szAction = (char*)malloc(256);
									sprintf(szAction, "Say([[M«n ph¸i nµy bÞ giíi h¹n %d tµi kho¶n mçi IP]], 0)", gUtil.FactionLimit[*(int*)(&This->tmp[0x25CE0] + 8)]);
									Execute_DoScript(This, szAction);
									free(szAction);

									OfflineActions_Execute(This_OfflineActions(), This, 3);*/
									/*"SetTimer(54, %d);"
										"ForbitStamina(1);"
										"ForbidEnmity(1);"
										"SetChatFlag(1);"*/

									//char szTimer[32];
									//memset(szTimer, 0, 32);
									////char* szTimer = (char*)malloc(32);
									//sprintf(szTimer, "SetTimer(54, %d)", gUtil.nTimerID);
									//Execute_DoScript(This, szTimer);
									////free(szTimer);
								/*}
								else
								{
									Execute_OfflineLive(This);
									Execute_KickOut(This, 6);
								}*/

								nIsLimited = true;

								//gUtil.SetByte((DWORD)&This->tmp[0x287B0], 1);
								//printf("GetFaction: %d\n", m_KPlayerInfo[nPlayerIndex].m_Faction);
								//printf("WanIpAddress: %s\n", &AllHardwareId[32]);



								//Execute_DoScript((KPlayer*)&CurrenPlayer[nPlayerIndex].tmp[165956], szAction);

								//Execute_OfflineDestroy((KServerCore*)0x82B8834, pPlayerIndex, 0);
								//Execute_CleanConnectionStatus(This);
								//Execute_ShutdownPlayer(This);
								//return 0;
							}
						}
					}

					if (gUtil.nLimitIP > 0)
					{
						if (strlen(m_KPlayerInfo[pPlayerIndex].m_LanIP) > 0)
						{
							if (!Sqlite_Check_IpAddress(m_KPlayerInfo[pPlayerIndex].m_LanIP))
							{
								if (m_KPlayerInfo[pPlayerIndex].m_KPlayer != NULL)
								{

									/*char* szAction = (char*)malloc(256);
									sprintf(szAction, "ForbitStamina(1);ForbidEnmity(1);SetChatFlag(1);Say([[Mçi ®Þa chØ IP chØ ®­îc truy cËp %d tµi kho¶n]], 1, [[Tho¸t game/exitgame]]);function OnCancel();PlayerIndex = SearchPlayer(GetName())OfflineLive(PlayerIndex);KickOutSelf(); return 0 end", gUtil.nLimitIP);
									Execute_DoScript((KPlayer*)m_KPlayerInfo[pPlayerIndex].m_KPlayer, szAction);
									free(szAction);*/

									//if (gUtil.nTimerID > 0)
									//{
										//char szAction[256];
										//memset(szAction, 0, 256);
										char* szAction = (char*)malloc(256);
										sprintf(szAction, "Say([[Mçi ®Þa chØ IP chØ ®­îc truy cËp %d tµi kho¶n]], 0);", gUtil.nLimitIP);
										Execute_DoScript(This, szAction);
										free(szAction);

										Execute_ExecuteScript(This, "\\script\\lib\\kickplayer.lua", "KickOutPlayer", 0, 0u);

										/*KScriptTimer* scriptTimer;
										ITimer* iTimer;
										scriptTimer = KScriptTimer_Create();
										iTimer = scriptTimer;
										DWORD scriptid = Execute_FileName2Id((LPCSTR*)"\\script\\lib\\kickplayer.lua");
										KScriptTimer_SetCallBack(scriptTimer, scriptid, "OnTimer", pPlayerIndex);
										KServerCore_AddTimer((KServerCore*)(0x82B8830), iTimer, 5 * 18);*/

										//char szAction[256];
										////memset(szAction, 0, 256);
										//char* szAction = (char*)malloc(256);
										//sprintf(szAction, "ForbitStamina(1);ForbidEnmity(1);SetChatFlag(1);Say([[Mçi ®Þa chØ IP chØ ®­îc truy cËp %d tµi kho¶n]], 1, [[Tho¸t game/exitgame]]);function OnCancel();PlayerIndex = SearchPlayer(GetName())OfflineLive(PlayerIndex);KickOutSelf(); return 0 end", gUtil.nLimitIP);
										//Execute_DoScript((KPlayer*)m_KPlayerInfo[pPlayerIndex].m_KPlayer, szAction);
										//free(szAction);

										//OfflineActions_Execute(This_OfflineActions(), This, 3);
										/*"SetTimer(54, %d);"
											"ForbitStamina(1); "
											"ForbidEnmity(1);"
											"SetChatFlag(1);"*/

										//char szTimer[32];
										//memset(szTimer, 0, 32);
										////char* szTimer = (char*)malloc(32);
										//sprintf(szTimer, "SetTimer(54, %d)", gUtil.nTimerID);
										//Execute_DoScript(This, szTimer);
										////free(szTimer);
									/*}
									else
									{
										Execute_OfflineLive(This);
										Execute_KickOut(This, 6);
									}*/
									nIsLimited = true;
								}
								//sleep(3);
								//Execute_OfflineLive((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer);
								//Execute_KickOut((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, 6);
							}
						}
					}
				}
			}
			catch (...)
			{

			}


			
			/*char* szScript = (char*)malloc(32);
			sprintf(szScript, "//script//callclient.lua");

			char* szParam = (char*)malloc(32);
			sprintf(szParam, "", 1);
			
			Execute_S2CExecuteScript(This, szScript, "");
			free(szScript);
			free(szParam);*/

			/*if (!nIsLimited)
				Execute_OnLogin(This);*/

			//Execute_OnLogin(This);

			//else
			//{
			//	//printf("ERROR DATA: %d %s %s\n", pPlayerIndex, pAccName, pRoleName);
			//	//Sqlite_BackNumber_On_Kichout_ByLimit(AllHardwareId, &AllHardwareId[33], &AllHardwareId[51]);
			//	Execute_KickOut(This, 5);
			//	Execute_ShutdownPlayer(This);
			//}

			//if (m_KPlayerInfo[pPlayerIndex].m_Faction >= 0 && m_KPlayerInfo[pPlayerIndex].m_Faction < 10)
			//{
			//	if (gUtil.FactionLimit[m_KPlayerInfo[pPlayerIndex].m_Faction] > 0)
			//	{
			//		if (!Sqlite_CheckFaction(m_KPlayerInfo[pPlayerIndex].m_AccountName, m_KPlayerInfo[pPlayerIndex].m_LanIP, m_KPlayerInfo[pPlayerIndex].m_Faction, gUtil.FactionLimit[m_KPlayerInfo[pPlayerIndex].m_Faction]))
			//		{
			//			if (This)
			//			{
			//				Execute_OfflineLive(This);
			//				Execute_KickOut(This, 6);
			//			}
			//			//gUtil.SetByte((DWORD)&This->tmp[0x287B0], 1);
			//			//printf("GetFaction: %d\n", m_KPlayerInfo[nPlayerIndex].m_Faction);
			//			//printf("WanIpAddress: %s\n", &AllHardwareId[32]);
			//			//char szAction[256];
			//			//memset(szAction, 0, 256);
			//			//strcpy(szFactionName, gUtil.GetFactionName(m_KPlayerInfo[nPlayerIndex].m_Faction));
			//			//gUtil.Convert(szFactionName, gUtil.GetFactionName(m_KPlayerInfo[nPlayerIndex].m_Faction));

			//			//sprintf(szAction, "Say([[<color=green>M«n ph¸i <color=yellow>%s<color> bÞ giíi h¹n <color=red>%d<color> tµi kho¶n mçi IP<color>]], 0)", gUtil.GetFactionName(m_KPlayerInfo[nPlayerIndex].m_Faction), gUtil.FactionLimit[m_KPlayerInfo[nPlayerIndex].m_Faction]);
			//			//Execute_DoScript((KPlayer*)m_KPlayerInfo[nPlayerIndex].m_KPlayer, szAction);
			//			//sleep(1);
			//			//Execute_DoScript((KPlayer*)&CurrenPlayer[nPlayerIndex].tmp[165956], szAction);

			//			//Execute_OfflineDestroy((KServerCore*)0x82B8834, pPlayerIndex, 0);
			//			//Execute_CleanConnectionStatus(This);
			//			//Execute_ShutdownPlayer(This);
			//			//return 0;
			//		}
			//	}
			//}
			//printf("OfflineLive: %d\n", *(int*)(&This->tmp[0x287B0]));

			//gUtil.SetByte((DWORD)&This->tmp[0x287B0], 1);

			//printf("OfflineLive: %d\n", *(int*)(&This->tmp[0x287B0]));

			//if (nHwidCount > 0)
			//{
			//	int nFactionID = *(int*)(&This->tmp[0x25CE0] + 8);
			//	if (nFactionID >= 0 && nFactionID < 10)
			//	{
			//		if (gUtil.FactionLimit[nFactionID] > 0)
			//		{
			//			if (!Sqlite_CheckFaction(pAccName, &AllHardwareId[32], nFactionID, gUtil.FactionLimit[nFactionID]))
			//			{
			//				//gUtil.SetByte((DWORD)&This->tmp[0x287B0], 1);
			//				//printf("GetFaction: %d\n", *(int*)(&This->tmp[0x25CE0] + 8));
			//				//printf("WanIpAddress: %s\n", &AllHardwareId[32]);
			//				//char szAction[256];
			//				//sprintf(szAction, "Say([[M«n ph¸i <color=yellow>%s<color> bÞ giíi h¹n <color=yellow>%d<color> tµi kho¶n mçi IP]], 0)", gUtil.GetFactionName(nFactionID), gUtil.FactionLimit[nFactionID]);
			//				//Execute_DoScript(Player, szAction);
			//				Execute_OfflineLive(This);
			//				Execute_KickOut(This, 8);
			//				//Execute_OfflineDestroy((KServerCore*)0x82B8834, pPlayerIndex, 0);
			//				//Execute_CleanConnectionStatus(This);
			//				//Execute_ShutdownPlayer(This);
			//			}
			//		}
			//	}
			//}

			/*

			nPlayerExp = *(int*)(&This->tmp[0x25C70]);

			Sqlite_UpdateRoleInfo_On_EnterGame(pAccName, pRoleName, 0, nPlayerExp);

			AllHardwareId[72] = 0;

	#if(_DEBUG==1)
			printf("HardwareId: %s\n", AllHardwareId);
			printf("MacAddress: %s\n", &AllHardwareId[37]);
			printf("IpAddress: %s\n", &AllHardwareId[55]);
	#endif
			memset(&m_KPlayerInfo[pPlayerIndex].m_AccountName, 0, 32);
			memcpy(&m_KPlayerInfo[pPlayerIndex].m_AccountName, pAccName, sizeof(m_KPlayerInfo[pPlayerIndex].m_AccountName));
			memset(&m_KPlayerInfo[pPlayerIndex].m_PlayerName, 0, 32);
			memcpy(&m_KPlayerInfo[pPlayerIndex].m_PlayerName, pRoleName, sizeof(m_KPlayerInfo[pPlayerIndex].m_PlayerName));
			if (strlen(AllHardwareId) > 0)
			{
				memset(&m_KPlayerInfo[pPlayerIndex].m_Hwid, 0, 36);
				memcpy(&m_KPlayerInfo[pPlayerIndex].m_Hwid, AllHardwareId, sizeof(m_KPlayerInfo[pPlayerIndex].m_Hwid));
			}

			if (strlen(&AllHardwareId[37]) > 0)
			{
				memset(&m_KPlayerInfo[pPlayerIndex].m_Mac, 0, 18);
				memcpy(&m_KPlayerInfo[pPlayerIndex].m_Mac, &AllHardwareId[37], sizeof(m_KPlayerInfo[pPlayerIndex].m_Mac));
			}

			if (strlen(&AllHardwareId[55]) > 0)
			{
				memset(&m_KPlayerInfo[pPlayerIndex].m_LanIP, 0, 17);
				memcpy(&m_KPlayerInfo[pPlayerIndex].m_LanIP, &AllHardwareId[55], sizeof(m_KPlayerInfo[pPlayerIndex].m_LanIP));
			}

			m_KPlayerInfo[pPlayerIndex].m_TickCount = gUtil.millis();
			m_KPlayerInfo[pPlayerIndex].m_NPCTalkFlood = 0;

			Execute_OnLogin(This);*/
			
		}
	}
	else
	{
		Execute_OnLogin(This);
	}
	//printf("SafeOnLogin: %d %s %s\n", pPlayerIndex, pAccName, pRoleName);		
}

void __cdecl SafeOnQuit(KPlayer* This)
{
	//pthread_create(&gPluginWeb.thread_check, NULL, &gPluginWeb.pull_one_url, NULL);

	int nPlayerIndex = 0;
	
	nPlayerIndex = *(int*)(&This->tmp[0xC0]);

	if (nPlayerIndex > 0)
	{

		try
		{
			int nNetConnectIdx = -1;
			int nLevel = 0;
			int nExpPercent;
			int PlayerBaseInfo;
			INT64 nPlayerExp;

			char* pAccName;
			char* pRoleName;
			char* pSaveBuffer;

			m_KPlayerInfo[nPlayerIndex].m_KPlayer = NULL;

			nNetConnectIdx = *(int*)((char*)&This->tmp[0x20408]);

			pAccName = &This->tmp[0x264];

			pRoleName = &This->tmp[0x25C88];

			nLevel = LOBYTE(Execute_GetMyNpc(This)->tmp[0x20]);

			nPlayerExp = *(INT64*)((char*)&This->tmp[0x25C70]);

#if(_DEBUG==1)
			printf("nPlayerIndex [ %d ] nNetConnectIdx [ %d ] Level [ %d ] Exp [ %lld ] Account [ %s ] Role [ %s ]\n", nPlayerIndex, nNetConnectIdx, nLevel, nPlayerExp, pAccName, pRoleName);
#endif
			try
			{
				Sqlite_FactionRemove((char*)pAccName);

				if (gUtil.nLimitIP > 0)
				{
					Sqlite_Remove_IP_On_Outgame((char*)pAccName);
				}

			}
			catch (...)
			{
			}

			Sqlite_UpdateRoleInfo_On_EnterGame(pAccName, pRoleName, nLevel, nPlayerExp);

			/*m_ClientInfo[nNetConnectIdx].m_NetConnectIdx = -1;
			memset(m_ClientInfo[nNetConnectIdx].m_TextGuid, 0, 33);
			memset(m_ClientInfo[nNetConnectIdx].m_Guid, 0, 16);
			m_ClientInfo[nNetConnectIdx].m_TickCount = 0LL;
			m_ClientInfo[nNetConnectIdx].m_Flood = 0;
			m_ClientInfo[nNetConnectIdx].m_Checked = false;

			m_KPlayerInfo[nPlayerIndex].m_nIndex = -1;
			memset(m_KPlayerInfo[nPlayerIndex].m_AccountName, 0, 32);
			memset(m_KPlayerInfo[nPlayerIndex].m_PlayerName, 0, 32);
			memset(m_KPlayerInfo[nPlayerIndex].m_LanIP, 0, 17);
			memset(m_KPlayerInfo[nPlayerIndex].m_Hwid, 0, 32);
			m_KPlayerInfo[nPlayerIndex].m_TickCount = 0LL;
			m_KPlayerInfo[nPlayerIndex].m_NPCTalkFlood = 0;
			m_KPlayerInfo[nPlayerIndex].m_ChatTick = 0LL;
			m_KPlayerInfo[nPlayerIndex].m_ChatFlood = 0;*/

		}
		catch (...)
		{
		}

		//Execute_OnQuit(This);
		//Execute_OnLeaveCurMap(This);
		Execute_WaitForRemove(This);

	}
}

void __cdecl AddPlayerToWorld(KServerCore* This, int nIndex, _DWORD dwInitKey, _DWORD dwProcessId)
{
	//int nPlayerIndex;
	int v4;
	//KPlayer* Player;
	KPlayer* v5;
	//CChatRoomRelayMsgProcess v6;
	TRoleData* pRoleData;
	//nPlayerIndex = nIndex;

	v4 = nIndex;

	//Execute_LaunchPlayer(&Player[nIndex]);
	//Execute_SendSyncData();

	v5 = Player;

	//Player[v4].tmp[0x203B8] = dwInitKey;
	//v5[v4].tmp[0x203BC] = dwProcessId;

	//Player = (KPlayer*)((char*)Player + 165956 * nIndex);

	printf("nPlayerIndex: %d\n", nIndex);
	//printf("LicenseId: %s\n", &v5[v4].tmp[0x287F0]);
	//printf("MacAddress: %s\n", &v5[v4].tmp[0x287F0 + 33]);
	//printf("IpAddress: %s\n", &Player[v4].tmp[0x287F0 + 33 + 18]);
	//printf("HardwareId: %s\n", &Player[v4].tmp[0x287F0 + 33 + 18 + 16]);

	pRoleData = (TRoleData*)&v5[v4].tmp[0x28C];

	printf("HardwareId: %s\n", pRoleData->BaseInfo.caccname);

	

	//BYTE		szBuff[64];
	//S2R_ADD_TAG_REQUEST* pDatatag;
	//pDatatag = (S2R_ADD_TAG_REQUEST*)&szBuff;
	//memcpy(&szBuff[0], (char*)&m_KPlayerInfo[nIndex].m_Hwid, 32);
	//memcpy(&szBuff[33], (char*)&m_KPlayerInfo[nIndex].m_LicenseId, 32);
	//Execute_OnAddTagRequest(This, nIndex, szBuff);
	//if ((char*)m_KPlayerInfo[nIndex].m_Hwid == NULL || (char*)m_KPlayerInfo[nIndex].m_Hwid == "")
	//{
	//	printf("No data!\n");
	//	//Execute_OnGetTagsRequest(m_pCore->m_pProtocol, nIndex, szBuff);
	//	Execute_OnGetTagsRequest((KProtocolProcess*)(void*)(0x88B8C20), nIndex, szBuff);
	//}

	//KPlayerTask* m_cTask;

	//m_cTask = (KPlayerTask*)&Player[nIndex].tmp[0x28188];

	//Execute_SyncTaskValueToClient(&Player[nIndex], 0);
	//Execute_SyncTaskValueToClient(&Player[nIndex], 1);
	//Execute_SyncTaskValueToClient(&Player[nIndex], 2);
	//Execute_SyncTaskValueToClient(&Player[nIndex], 3);
	/*Execute_KPlayerSave(&Player[nIndex], 0);

	printf(" %s \n", m_cTask->szTask[0]);
	printf(" %s \n", m_cTask->szTask[1]);
	printf(" %s \n", m_cTask->szTask[2]);
	printf(" %s \n", m_cTask->szTask[3]);*/

	//strncpy((char*)&szBuffHwid[0], &Player->tmp[0x25C88], 32);

	//printf("PlayerName %s\n", &v5->tmp[0x25C88] + 165956 * nIndex);

	//Execute_OnGetTagsRequest((KProtocolProcess*)This->m_pHostProto, nIndex, &szBuffHwid[0]);

	//Execute_OnGetTagsRequest((KProtocolProcess*)This->m_pHostProto, nIndex, &szBuffIpMac[0]);

	////printf("No data! %s %s\n", (char*)This->m_bySendBuffer, (char*)This->m_bySendBuffer[35]);

	//printf("PlayerName %s\n", &szBuffHwid[0]);

	//printf("Hwid %s\n", &szBuffHwid[33]);

	//if ((char*)&m_KPlayerInfo[nIndex].m_Hwid == "")
	//{
		
		

		//printf("No data %s %s\n", (char*)&szBuffIpMac[0], (char*)&szBuffIpMac[33]);

		
		//memcpy(&m_KPlayerInfo[nIndex].m_Hwid, (char*)&szBuffHwid[0], sizeof(m_KPlayerInfo[nIndex].m_Hwid));

		//memcpy(&m_KPlayerInfo[nIndex].m_Mac, (char*)&szBuffIpMac[0], sizeof(m_KPlayerInfo[nIndex].m_Mac));

		//memcpy(&m_KPlayerInfo[nIndex].m_LanIP, (char*)&szBuffIpMac[33], sizeof(m_KPlayerInfo[nIndex].m_LanIP));

		//memcpy(&m_KPlayerInfo[nIndex].m_LicenseId, (char*)&szBuffHwid[33], sizeof(m_KPlayerInfo[nIndex].m_LicenseId));

		//Execute_OnAddTagRequest((KProtocolProcess*)This->m_pHostProto, nIndex, &szBuffHwid[0]);

		//Execute_OnAddTagRequest((KProtocolProcess*)This->m_pHostProto, nIndex, &szBuffIpMac[0]);
	//}
	
	//printf("- %s\n", (char*)&m_KPlayerInfo[nIndex].m_Hwid);

	//strcpy(mHwid, (char*)&m_KPlayerInfo[PlayerIndex].m_Hwid);
	 
	//Execute_ChatRoomRelayMsgProcess(&v6);
	//Execute_CChatRoomRelayMsgProcess(&v6);
	//Execute_NotifyChatChannel(&v6, nIndex);

	/*if (Execute_SendSyncCurPlayer(&v5[nIndex], nIndex))
	{
		printf("Sync OK\n");
	}*/

	/*Player = (KPlayer*)((char*)Player + 165956 * nIndex);

	*(_DWORD*)((char*)&Player->tmp[0x203B8] + v4) = dwInitKey;
	*(_DWORD*)((char*)&v5->tmp[0x203BC] + v4) = dwProcessId;

	memcpy(&v5[nIndex].tmp[0x287F0], (char*)&Player->tmp[0x287F0] + v4, 33);

	memcpy(&v5[nIndex].tmp[0x287F0 + 33], (char*)&Player->tmp[0x287F0 + 33] + v4, 18);

	memcpy(&v5[nIndex].tmp[0x287F0 + 33 + 18], (char*)&Player->tmp[0x287F0 + 33 + 18] + v4, 16);

	memcpy(&v5[nIndex].tmp[0x287F0 + 33 + 18 + 16], (char*)&Player->tmp[0x287F0 + 33 + 18 + 16] + v4, 32);

	v5->tmp[0x287F0 + 33 + 18 + 16 + 32] = 0;*/

	//printf("\n\n", nPlayerIndex);
	//printf("nPlayerIndex: %d\n", nPlayerIndex);
	//printf("LicenseId: %s\n", (char*)Player->tmp[0x287F0]);
	//printf("MacAddress: %s\n", (char*)Player->tmp[0x287F0 + 33]);
	//printf("IpAddress: %s\n", (char*)Player->tmp[0x287F0 + 33 + 18]);
	//printf("HardwareId: %s\n", (char*)Player->tmp[0x287F0 + 33 + 18 + 16]);

	//printf("dwProcessId: %d\n", dwProcessId);

	//v5[nPlayerIndex].tmp[0x203B8] = dwProcessId;

	/*memcpy(&v5[nPlayerIndex].tmp[0x287F0], (char*)&m_KPlayerInfo[nPlayerIndex].m_LicenseId, 33);

	memcpy(&v5[nPlayerIndex].tmp[0x287F0 + 33], (char*)&m_KPlayerInfo[nPlayerIndex].m_Mac, 18);

	memcpy(&v5[nPlayerIndex].tmp[0x287F0 + 33 + 18], (char*)&m_KPlayerInfo[nPlayerIndex].m_LanIP, 16);

	memcpy(&v5[nPlayerIndex].tmp[0x287F0 + 33 + 18 + 16], (char*)&m_KPlayerInfo[nPlayerIndex].m_Hwid, 32);

	v5[nPlayerIndex].tmp[0x287F0 + 33 + 18 + 16 + 33] = 0;*/

	//printf("nPlayerIndex: %d\n", nPlayerIndex);
	//printf("LicenseId: %s\n", (char*)&v5[nPlayerIndex].tmp[0x287F0]);
	//printf("MacAddress: %s\n", (char*)&v5[nPlayerIndex].tmp[0x287F0 + 33]);
	//printf("IpAddress: %s\n", (char*)&v5[nPlayerIndex].tmp[0x287F0 + 33 + 18]);
	//printf("HardwareId: %s\n", (char*)&v5[nPlayerIndex].tmp[0x287F0 + 33 + 18 + 16]);
	//printf("dwInitKey: %d\n", dwInitKey);
	//printf("dwProcessId: %d\n", dwProcessId);

	Execute_AddPlayerToWorld(This, nIndex, dwInitKey, dwProcessId);
}

void __cdecl SafeNotifyHostLeaveGame(KServerCore* This, LPCSTR pszAccount, LPCSTR pszRole, int nType)
{
	//printf("pszAccount: %s\n", pszAccount);
	//printf("pszRole: %s\n", pszRole);
	//printf("nType: %d\n", nType);
	
	Execute_NotifyHostLeaveGame(This, pszAccount, pszRole, nType);
	try
	{
		if (gUtil.nLimitIP > 0)
		{
			Sqlite_Remove_IP_On_Outgame((char*)pszAccount);
		}

		if (gUtil.nLimit > 0)
		{
			Sqlite_Remove_HardwareId_On_Outgame(pszAccount);
		}
		
	}
	catch (...)
	{ }
}

void __cdecl NotifyHostEnterGame(KServerCore* This, LPCSTR pszAccount, LPCSTR pszRole, int nPlayerIndex, _DWORD dwNameID, unsigned int lnID, int nLevel)
{
	//tagEnterGame2 eg2;
	int nIndex;

	//printf("m_pServer: %d\n", This->m_pServer);
	//printf("pszAccount: %s\n", pszAccount);
	//printf("pszRole: %s\n", pszRole);
	//printf("nPlayerIndex: %d\n", nPlayerIndex);
	//printf("dwNameID: %d\n", dwNameID);
	//printf("lnID: %d\n", lnID);
	//printf("nLevel: %d\n", nLevel);
	//printf("nSize: %d\n", sizeof(tagEnterGame2));
	Execute_NotifyHostEnterGame(This, pszAccount, pszRole, nPlayerIndex, dwNameID, lnID, nLevel);
	//printf("HardwareId: %s\n", (char*)&m_KPlayerInfo[nPlayerIndex].m_Hwid);
	//printf("MacAddress: %s\n", (char*)&m_KPlayerInfo[nPlayerIndex].m_Mac);
	//printf("IpAddress: %s\n", (char*)&m_KPlayerInfo[nPlayerIndex].m_LanIP);
	try
	{
		Sqlite_UpdateRoleInfo_Level_On_EnterGame(pszAccount, nLevel);
	}
	catch (...)
	{

	}
	
	//Sqlite_GetAndInsert_HardwareId_On_EnterGame_Gs((char*)&m_KPlayerInfo[nPlayerIndex].m_Hwid, (char*)&m_KPlayerInfo[nPlayerIndex].m_Mac, (char*)&m_KPlayerInfo[nPlayerIndex].m_LanIP);
	//printf("LicenseId: %s\n", (char*)&Player[nPlayerIndex].tmp[0x287F0]);
	//printf("MacAddress: %s\n", (char*)&Player[nPlayerIndex].tmp[0x287F0 + 33]);
	//printf("IpAddress: %s\n", (char*)&Player[nPlayerIndex].tmp[0x287F0 + 33 + 18]);
	//printf("HardwareId: %s\n", (char*)&Player[nPlayerIndex].tmp[0x287F0 + 33 + 18 + 16]);

	//BYTE szBuff[sizeof(tagEnterGame2) + 1];
	//memset(szBuff, 0, sizeof(tagEnterGame2) + 1);
	//szBuff[0] = 0;

	//eg2 = (tagEnterGame2*)&szBuff[1];
	/*
	eg2.ProtocolFamily = 0;
	eg2.ProtocolID = 71;
	eg2.dwParam = nPlayerIndex;
	eg2.dwNameID = dwNameID;
	eg2.lnID = lnID;
	eg2.byLevel = nLevel;
	memcpy(eg2.szAccount, pszAccount, 31u);
	eg2.szAccount[31] = 0;
	memcpy(eg2.szRole, pszRole, 31u);
	eg2.szRole[31] = 0;

	printf("Begin send\n");*/
	//0x8249310
	/*memcpy(&Player[nPlayerIndex].tmp[0x287F0], (char*)&m_KPlayerInfo[nPlayerIndex].m_LicenseId, 33);

	memcpy(&Player[nPlayerIndex].tmp[0x287F0 + 33], (char*)&m_KPlayerInfo[nPlayerIndex].m_Mac, 18);

	memcpy(&Player[nPlayerIndex].tmp[0x287F0 + 33 + 18], (char*)&m_KPlayerInfo[nPlayerIndex].m_LanIP, 16);

	memcpy(&Player[nPlayerIndex].tmp[0x287F0 + 33 + 18 + 16], (char*)&m_KPlayerInfo[nPlayerIndex].m_Hwid, 32);

	Player[nPlayerIndex].tmp[0x287F0 + 33 + 18 + 16 + 33] = 0;*/

	//IGameServer*  
	//Execute_ProcessClientMessage((IGameServer*)This->m_pServer->_vptr_IGameServer + 2, 2, &eg2, 79);
	//BYTE szBuff[33];
	//szBuff[0] = 52;
	//memcpy(&szBuff[1], pszAccount, 32);
	//szBuff[32] = 0;
	//szBuff[33] = nPlayerIndex;
	//Execute_ProcessClientMessage((IGameServer*)This->m_pServer->_vptr_IGameServer + 2, 1, &szBuff, 35);
	//(*((void(__cdecl**)(IGameServer*, int, tagEnterGame2*, int))This->m_pServer->_vptr_IGameServer + 2)) (This->m_pServer, 1, &szBuff, 79);
	
	//(*((void(__cdecl**)(IGameServer*, int, tagEnterGame2*, int))This->m_pServer->_vptr_IGameServer + 2))(
	//	This->m_pServer,
	//	2,
	//	&eg2,
	//	79);

	//Execute_ProcessServerMessage((IGameServer*)This->m_pServer, emSERVER_BISHOP, &szBuff, 35);
	//Execute_NotifyBishopEnterGame();

	//(*((void(__cdecl**)(IGameServer*, int, const void* pData, int))This->m_pServer->_vptr_IGameServer + 2))(
	//	This->m_pServer,
	//	1,
	//	&szBuff,
	//	33);
	/*BYTE szBuff[sizeof(KDataShort) + 1];

	KDataShort* dataShort = (KDataShort*)&szBuff;
	dataShort->ProtocolID = 29;
	dataShort->Size = sizeof(KDataShort);
	dataShort->nPlayerIndex = nPlayerIndex;
	dataShort->lnID = lnID;
	dataShort->nLevel = nLevel;
	memcpy(&dataShort->m_szAccount, pszAccount, sizeof(dataShort->m_szAccount));
	memcpy(&dataShort->m_szRole, pszRole, sizeof(dataShort->m_szAccount));

	gBishopClient->ClientSend((const char*)&szBuff, sizeof(KDataShort));*/

	

	//Execute_NotifyHostEnterGame(This, pszAccount, pszRole, nPlayerIndex, dwNameID, lnID, nLevel);
}

int __cdecl BishopLoadRoleData(LPCSTR* This, TRoleData* pRoleData, GUID* pGuid, KACCOUNT_INFO2* pAccountInfo)
{
	int v4;
	int nIndex;
	//KPlayer* v5;

	char HardwareId[32];
	//char LicenseId[33];
	//char MacAddress[18];
	//char IpAddress[16];

	strcpy(HardwareId, pRoleData->HardwareId);
	//strcpy(LicenseId, pRoleData->LicenseId);
	//strcpy(MacAddress, pRoleData->MacAddress);
	//strcpy(IpAddress, pRoleData->IpAddress);

	//printf("nTotalLeftTime: %d\n", pAccountInfo->nTotalLeftTime);
	//printf("dwVersion: %d\n", pRoleData->dwVersion);

	//printf("LicenseId: %s\n", LicenseId);
	//printf("MacAddress: %s\n", MacAddress);
	//printf("IpAddress: %s\n", IpAddress);
	//printf("HardwareId: %s\n", HardwareId);
	//printf("szName: %s\n", pRoleData->BaseInfo.szName);
	//printf("dwDataLen1: %d\n", pRoleData->dwDataLen);
	//pRoleData->dwDataLen = pRoleData->dwDataLen + sizeof(pRoleData->HardwareId) + sizeof(pRoleData->MacAddress) + sizeof(pRoleData->IpAddress) + sizeof(pRoleData->LicenseId);
	//printf("dwDataLen2: %d\n", pRoleData->dwDataLen);
	//printf("TRoleData: %d\n", sizeof(TRoleData));
	//printf("TRoleDataOrigin: %d\n", sizeof(TRoleDataOrigin));

	//TRoleDataOrigin* pRoleDataOrigin;
	//BYTE szBuff[sizeof(TRoleDataOrigin)+1];
	//pRoleDataOrigin = (TRoleDataOrigin*)&szBuff;

	//memset(szBuff, 0, sizeof(TRoleDataOrigin));

	//memcpy(pRoleDataOrigin, pRoleData, sizeof(TRoleDataOrigin));

	//printf("dwVersion: %d\n", pRoleDataOrigin->dwVersion);
	memset(pRoleData->HardwareId, 0, 32);
	nIndex = Execute_LoadRoleData(This, pRoleData, pGuid, pAccountInfo);

	//v5 = Player;

	//printf("nIndex: %d\n", nIndex);
	v4 = nIndex;

	memcpy(&m_KPlayerInfo[nIndex].m_Hwid, HardwareId, sizeof(m_KPlayerInfo[nIndex].m_Hwid));
	HardwareId[31] = 0;
	//memcpy(&m_KPlayerInfo[nIndex].m_LanIP, IpAddress, sizeof(m_KPlayerInfo[nIndex].m_LanIP));
	//memcpy(&m_KPlayerInfo[nIndex].m_Mac, MacAddress, sizeof(m_KPlayerInfo[nIndex].m_Mac));
	//memcpy(&m_KPlayerInfo[nIndex].m_LicenseId, LicenseId, sizeof(m_KPlayerInfo[nIndex].m_LicenseId));

	//memcpy(&Player[nIndex].tmp[0x287F0], LicenseId, 33);

	//memcpy(&Player[nIndex].tmp[0x287F0 + 33], MacAddress, 18);

	//memcpy(&Player[nIndex].tmp[0x287F0 + 33 + 18], IpAddress, 16);

	//memcpy(&Player[nIndex].tmp[0x287F0 + 33 + 18 + 16], HardwareId, 32);
	 
	//memcpy(&Player[nIndex].tmp[0x28845], HardwareId, 32);

	//*(_DWORD*)&Player[nIndex].tmp[0x287F0 + 33 + 18 + 16 + 33] = 0;
	
	//BYTE		szBuffHwid[66];

	//memset(&szBuffHwid, 0, 66);
	//memcpy(&szBuffHwid[1], pRoleData->BaseInfo.szName, 32);
	//memcpy(&szBuffHwid[33], HardwareId, 32);
	//Execute_OnAddTagRequest(m_pCore->m_pProtocol, nIndex, szBuff);
	//Execute_OnAddTagRequest((KProtocolProcess*)g_ProtocolProcess(), nIndex, &szBuffHwid[0]);

	//printf(" %s \n", (char*)&szBuffHwid[1]);

	/*KPlayerTask* p_m_cTask;
	
	p_m_cTask = (KPlayerTask*)&Player[nIndex].tmp[0x28188];

	strcpy(p_m_cTask->szTask[0], HardwareId);
	strcpy(p_m_cTask->szTask[1], MacAddress);
	strcpy(p_m_cTask->szTask[2], IpAddress);
	strcpy(p_m_cTask->szTask[3], LicenseId);*/

	//printf(" %s - %s \n", (char*)&szBuffHwid[0], (char*)&szBuffHwid[33]);

	//KPlayerTask* m_cTask;

	//m_cTask = (KPlayerTask*)&Player[nIndex].tmp[0x28188];

	//printf(" %s \n", m_cTask->szTask[0]);
	//printf(" %s \n", m_cTask->szTask[1]);
	//printf(" %s \n", m_cTask->szTask[2]);
	//printf(" %s \n", m_cTask->szTask[3]);

	//memset(&szBuffIpMac, 0, 64);
	//memcpy(&szBuffIpMac[0], MacAddress, 18);
	//memcpy(&szBuffIpMac[33], IpAddress, 16);

	//Execute_OnAddTagRequest(m_pCore->m_pProtocol, nIndex, szBuff);
	//Execute_OnAddTagRequest((KProtocolProcess*)g_ProtocolProcess(), nIndex, &szBuffIpMac[0]);

	//printf(" %s - %s \n", (char*)&szBuffIpMac[0], (char*)&szBuffIpMac[33]);

	//tagRelaySDBOper pData;
	//Execute__RecordDataArrival(pData, );
	//Execute_SyncTaskValueToClient(&Player[nIndex], 0);
	//Execute_SyncTaskValueToClient(&Player[nIndex], 1);
	//Execute_SyncTaskValueToClient(&Player[nIndex], 2);
	//Execute_SyncTaskValueToClient(&Player[nIndex], 3);

	//Execute_KPlayerSave(&Player[nIndex], 1);

	//printf("-LicenseId: %s\n", &Player[nIndex].tmp[0x287F0]);
	//printf("-MacAddress: %s\n", &Player[nIndex].tmp[0x287F0 + 33]);
	//printf("-IpAddress: %s\n", &Player[nIndex].tmp[0x287F0 + 33 + 18]);
	//printf("-HardwareId: %s\n", &Player[nIndex].tmp[0x287F0 + 33 + 18 + 16]);

	//0x28844; 165956 * nIndex
	//m_KPlayerInfo[nIndex].m_nIndex = nIndex;

	

	return nIndex;
}

void KPlayerNew::Hook_TextGMFilter()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);

	uintptr_t start;
	
	uintptr_t pagestart;

	//==========================================

	/*start = (uintptr_t)(void*)0x81DF9F3;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &TextGMFilter);*/

	//==========================================

	/*start = (uintptr_t)(void*)0x81F7421;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &TextGMFilter);*/

	//==================================================

	/*start = (uintptr_t)(void*)0x82033D1;
	pagestart = start & -pagesize;
	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}
	gUtil.SetCompleteHook(0xE8, start, &ProcessGMCommand);*/

	//==================================================

	/*start = (uintptr_t)(void*)0x8203429;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeTextMsgProcessGMCmd);*/

	/*start = (uintptr_t)(void*)0x806E497;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &SafeTextMsgProcessGMCmd);*/

	/*start = (uintptr_t)(void*)0x806E467;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &SafeTextMsgProcessGMCmd);*/
	
	//=======================================================

	/*start = (uintptr_t)(void*)0x80DCA6C;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &SafeProcessNetMsg);*/


	//start = (uintptr_t)(void*)0x80C2F92;

	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeServerSendChat);

	//
	//start = (uintptr_t)(void*)0x81DFA41;

	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE8, start, &SafeSendDataToNearRegion);
	
	//=======================================================

	/*start = (uintptr_t)(void*)0x812E60F;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeExecuteRelayScript);*/
	// 
	//gUtil.SetDword(0x82492D0, &SafeExecuteRelayScript);

	//=======================================================


	// == HOST ==

	start = (uintptr_t)(void*)0x804B195;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &ClientConnection_SendData_Host);

	// == TONG ==

	start = (uintptr_t)(void*)0x804B17F;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &ClientConnection_SendData_Tong);

	// == CHAT ==

	/*start = (uintptr_t)(void*)0x804B169;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &ClientConnection_SendData_Chat);*/

	// =======================================

	/*start = (uintptr_t)(void*)0x804DE8A;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &KChatProcessProcessMessage);*/

	/*start = (uintptr_t)(void*)0x81F7A3E;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeProcessPlayerTongMsg);*/

	//===============================================//

	//start = (uintptr_t)(void*)0x80F283D;

	//pagestart = start & -pagesize;

	//if (mprotect((void*)pagestart, 0x2000u,
	//	PROT_READ | PROT_WRITE | PROT_EXEC))
	//{
	//	perror("mprotect");
	//	exit(EXIT_FAILURE);
	//}

	//gUtil.SetCompleteHook(0xE9, start, &SafeDoPlayerTalk);

}

void __cdecl SafeProcessPlayerTongMsg(KClientProcess* This, const unsigned int nPlayerIdx, const char* pData, size_t dataLength)
{
	time_t now;
	struct tm* timenow;
	now = time(0);
	timenow = gmtime(&now);
	char TextCurrenTime[100];
	char TextData[100];
	//KCONNECTION m_pConnection;

	strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);
	BYTE byProtocol = pData[3];
	gUtil.WriteTongMsg("TongMsg - Time: %s, Protocol: [ %d ], PlayerIdx: [ %d ], HardwareId: [ %s ], Data 1: [ %d ], Len: [ %d ]\n",
		TextCurrenTime, byProtocol, nPlayerIdx, m_KPlayerInfo[nPlayerIdx].m_Hwid, *(WORD*)(pData + 1), dataLength);
	Execute_ProcessPlayerTongMsg(This, nPlayerIdx, pData, dataLength);
}

void __cdecl SafeProcessNetMsg(KProtocolProcess* This, int nIndex, BYTE* pMsg)
{
	KPlayer*  Player = (KPlayer*)((char*)0x88A1A20 + 165956 * nIndex);

	printf("nIndex: %d, Protocol : %d\n", nIndex, *pMsg);

	Execute_ProcessNetMsg(This, nIndex, pMsg);

	/*if (Player->tmp[0x20408] != -1 && Player->tmp[0x20400] && Player->tmp[0x20404] > 0)
	{
		Execute_ProcessNetMsg(This, nIndex, pMsg);
	}*/
	/*if (nIndex <= 0 || nIndex > gUtil.nMaxPlayer)
	{
		return;
	}
	else
		Execute_ProcessNetMsg(This, nIndex, pMsg);*/
}

bool __cdecl ProcessGMCommand(int nPlayerIdx, EGameMasterCommand eCommand, const char* pParam, int nLen)
{
	//Execute_ProcessGMCommand(nPlayerIdx, eCommand, pParam, nLen);
	return 0;
}

bool __cdecl ClientConnection_SendData_Host(CClientConnection* This, const void* pData, unsigned int nLen)
{
	try
	{
		KIniFile FileHandle;
		Execute_KIniFile(&FileHandle);
		Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
		Execute_KIniFile_GetInteger(&FileHandle, "Setting", "SpamTong", 0, &gUtil.nPacket);
		Execute_KIniFile_Clear(&FileHandle);

		if (gUtil.nPacket > 0)
		{
			int PacketCount = 0;
			char NumPacket[2];
			char Buffer[32];
			//LPCSTR lpDefault = "0,0,0";


			time_t now;
			struct tm* timenow;
			now = time(0);
			timenow = gmtime(&now);
			char TextCurrenTime[100];
			char TextData[100];
			//KCONNECTION m_pConnection;

			strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

			BYTE byProtocol;
			byProtocol = *(BYTE*)pData;
			//m_pConnection = *(KCONNECTION*)(const char*)This->tmp[0x14];

			//gUtil.WritePacketSpam("ProcessNet Info - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %s ], BYTE2: [ %s ], BYTE3: [ %s ], BYTE4: [ %s ], Len: [ %d ]\n",
				//TextCurrenTime, byProtocol, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);

			try
			{
				if (byProtocol != 4 && nLen != 42 | byProtocol != 4 && nLen != 18)
				{
					gUtil.WritePacketSpam("Packet 0 - Time: %s, Protocol: [ %d ], [ %d ], Data: [ %s ], Len: [ %d ]\n",
						TextCurrenTime, byProtocol, *((BYTE*)pData + 1), gUtil.hexStr((unsigned char*)pData, nLen).c_str(), nLen);
				}
				//if (nLen != 562 || nLen != 563 || nLen != 564 || nLen != 557 || nLen != 558 || nLen != 556 || nLen != 559 || nLen != 561)
				if (nLen < 512)
				{
					if (PacketCount > 0)
					{
						if (gUtil.CheckPacket(0, *((BYTE*)pData + 1), *((BYTE*)pData + 2), *((BYTE*)pData + 3)))
							return false;
						else
							return Execute_SendData_Host(This, pData, nLen);
					}
					else
					{
						if (*((BYTE*)pData + 1) == 60 && *((BYTE*)pData + 2) == 45 && *((BYTE*)pData + 3) == 29)
						{
							return false;
						}
						else if (*((BYTE*)pData + 1) == 33 && *((BYTE*)pData + 2) == 13 && *((BYTE*)pData + 3) == 10)
						{
							return false;
						}
						else
							return Execute_SendData_Host(This, pData, nLen);
					}
				}
				else
				{
					if (PacketCount > 0)
					{
						if (gUtil.CheckPacket(0, *((BYTE*)pData + 1), *((BYTE*)pData + 2), *((BYTE*)pData + 3)))
							return false;
						else
							return Execute_SendData_Host(This, pData, nLen);
					}
					else
					{
						if (*((BYTE*)pData + 1) == 60 && *((BYTE*)pData + 2) == 45 && *((BYTE*)pData + 3) == 29)
						{
							return false;
						}
						else if (*((BYTE*)pData + 1) == 33 && *((BYTE*)pData + 2) == 13 && *((BYTE*)pData + 3) == 10)
						{
							return false;
						}
						else
						{
							return Execute_SendData_Host(This, pData, nLen);
						}
					}
				}
				/*char test;

				test = *((BYTE*)pData + 1);

				gUtil.WritePacketSpam(" Packet Info 2 - Time: %s, Protocol: [ %d ], Data: [ %x ], [ %s ], Len: [ %d ]\n",
					TextCurrenTime, byProtocol, *(BYTE*)pData, test, nLen);*/

					//if (byProtocol == 0)
					//{
					//	if (nLen == 79)
					//	{
					//		//return false;
					//		char Data1[10];

					//		memcpy(Data1, (char*)pData + 3, 10);

					//		Data1[10] = 0;

					//		/*gUtil.WritePacketSpam(" Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
					//			TextCurrenTime, byProtocol, (char*)pData, Data1, Data1, (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);*/

					//		std::string s = Data1;

					//		if (s.find('ex') != std::string::npos || s.find('ci') != std::string::npos)
					//		{
					//			//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
					//				//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

					//			/*gUtil.WritePacketSpam(" Packet Spam 2 - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
					//				TextCurrenTime, byProtocol, (char*)pData, Data1, Data1, (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);*/

					//			return false;
					//		}
					//		else
					//			return Execute_SendData_Host(This, pData, nLen);
					//	}
					//	else if (nLen == 67)
					//	{
					//		char Data1[10];

					//		memcpy(Data1, (char*)pData + 3, 10);

					//		Data1[10] = 0;

					//		/*gUtil.WritePacketSpam(" Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
					//			TextCurrenTime, byProtocol, (char*)pData, Data1, Data1, (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);*/

					//		std::string s = Data1;

					//		if (s.find('ex') != std::string::npos || s.find('ci') != std::string::npos)
					//		{
					//			//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
					//				//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

					//			/*gUtil.WritePacketSpam(" Packet Spam 2 - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
					//				TextCurrenTime, byProtocol, (char*)pData, Data1, Data1, (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);*/

					//			return false;
					//		}
					//		else
					//			return Execute_SendData_Host(This, pData, nLen);

					//	}
					//	else
					//		return Execute_SendData_Host(This, pData, nLen);
					//}
					//else if (byProtocol == 1)
				if (byProtocol == 1)
				{
					if (nLen == 12)
					{
						//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
							//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

						/*gUtil.WritePacketSpam(" Packet Spam - Time: %s, Protocol: [ %d ], [ %x ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
							TextCurrenTime, byProtocol, (DWORD*)This, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);*/

						return false;
					}
					//{
					//	char Data2[4];

					//	memcpy(Data2, (char*)pData + 2, 4);

					//	Data2[4] = 0;

					//	/*gUtil.WritePacketSpam(" Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
					//		TextCurrenTime, byProtocol, (char*)pData, (char*)pData + 1, Data2, (char*)pData + 3, (char*)pData + 4, nLen);*/

					//	std::string s = Data2;

					//	if (s.find('$') != std::string::npos)
					//	{
					//		//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
					//			//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

					//		/*gUtil.WritePacketSpam(" Packet Spam 2 - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
					//			TextCurrenTime, byProtocol, (char*)pData, (char*)pData + 1, Data2, (char*)pData + 3, (char*)pData + 4, nLen);*/

					//		return false;
					//	}
					//	else
					//		return Execute_SendData_Host(This, pData, nLen);
					//}
					//else if (nLen >= 65538)
					//{
					//	return false;
					//}
					else
						return Execute_SendData_Host(This, pData, nLen);

					//std::string s = (char*)pData + 1;
					//if (s.find('¤^N') != std::string::npos || s.find('>²h÷') != std::string::npos || s.find('!gŽÚ') != std::string::npos)
					//{
					//	//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
					//		//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

					//	gUtil.WritePacketSpam(" Packet Spam - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
					//		TextCurrenTime, byProtocol, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);

					//	return false;
					//}
					//else
					//	return Execute_SendData_Host(This, pData, nLen);

					//if (strcmp((char*)pData + 4, "¤^Nÿ") == 0)
					//{
					//	//gUtil.WritePacketSpam("Packet Spam 2 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
					//		//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);
					//	return false;
					//}
					//else
					//	return Execute_SendData_Host(This, pData, nLen);
				}
				else
					return Execute_SendData_Host(This, pData, nLen);

				/*if (byProtocol == 0)
				{
					if (*(BYTE*)pData + 1 == 1 && *(BYTE*)pData + 2 == 2 && *(BYTE*)pData + 3 == 3)
					{
						gUtil.WritePacketSpam("Packet Spam - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
							TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);
						return false;
					}
					else
						return Execute_SendData_Host(This, pData, nLen);
				}
				else if (byProtocol == 1)
				{
					if (*(BYTE*)pData + 1 == 2 && *(BYTE*)pData + 2 == 3 && *(BYTE*)pData + 3 == 4)
					{
						gUtil.WritePacketSpam("Packet Spam - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
							TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);
						return false;
					}
					else
						return Execute_SendData_Host(This, pData, nLen);
				}
				else if (byProtocol == 4)
				{
					if (strcmp((const char*)pData + 1, "!GM") == 0)
					{
						gUtil.WritePacketSpam("Block Chanel GM - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
							TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);
						return false;
					}
					else
						return Execute_SendData_Host(This, pData, nLen);
				}
				else if (byProtocol == 14)
				{

					if (*(BYTE*)pData + 1 == 15 && *(BYTE*)pData + 2 == 16 && *(BYTE*)pData + 3 == 17)
					{
						gUtil.WritePacketSpam("Packet Spam - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
							TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);
						return false;
					}
					else
						return Execute_SendData_Host(This, pData, nLen);
				}
				else
					return Execute_SendData_Host(This, pData, nLen);*/
			}
			catch (...)
			{
				return false;
			}
		}
		else
		{
			return Execute_SendData_Host(This, pData, nLen);
		}
	}
	catch (...)
	{
		return Execute_SendData_Host(This, pData, nLen);
	}
}

bool __cdecl ClientConnection_SendData_Tong(CClientConnection* This, const void* pData, unsigned int nLen)
{
	try
	{
		KIniFile FileHandle;
		Execute_KIniFile(&FileHandle);
		Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
		Execute_KIniFile_GetInteger(&FileHandle, "Setting", "SpamTong", 0, &gUtil.nPacket);
		Execute_KIniFile_Clear(&FileHandle);

		if (gUtil.nPacket > 0)
		{

			int PacketCount = 0;
			char NumPacket[2];
			char Buffer[32];
			//LPCSTR lpDefault = "0,0,0";
			KIniFile FileHandle1;
			Execute_KIniFile(&FileHandle1);
			Execute_KIniFile_Load(&FileHandle1, "servercfg.ini");
			Execute_KIniFile_GetInteger(&FileHandle1, "Packet2", "Count", 0, &PacketCount);
			//printf("Packet2 Count: %d\n", PacketCount);
			if (PacketCount > 0)
			{
				for (int i = 0; i < PacketCount; ++i)
				{
					memset(NumPacket, 0, 2);
					memset(Buffer, 0, 32);
					sprintf(NumPacket, "%d", i);
					Execute_KIniFile_GetString(&FileHandle1, "Packet2", NumPacket, "0,0,0", Buffer, sizeof(Buffer));
					gUtil.InsertHardwareId(1, Buffer);
					//printf("Buffer2: %s\n", Buffer);
				}
				
			}
			Execute_KIniFile_Clear(&FileHandle1);

			time_t now;
			struct tm* timenow;
			now = time(0);
			timenow = gmtime(&now);
			char TextCurrenTime[100];
			char TextData[100];
			//KCONNECTION m_pConnection;

			strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

			try
			{
				BYTE byProtocol;
				byProtocol = *(BYTE*)pData;
				//m_pConnection = *(KCONNECTION*)(const char*)This->tmp[0x14];

				//gUtil.WritePacketSpam("ProcessNet Info - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %s ], BYTE2: [ %s ], BYTE3: [ %s ], BYTE4: [ %s ], Len: [ %d ]\n",
					//TextCurrenTime, byProtocol, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);

				gUtil.WritePacketSpam("Packet 1 - Time: %s, Protocol: [ %d ], [ %d ], Data: [ %s ], Len: [ %d ]\n",
					TextCurrenTime, byProtocol, *((BYTE*)pData + 1), gUtil.hexStr((unsigned char*)pData, nLen).c_str(), nLen);

				/*char test;

				test = *((BYTE*)pData + 1);

				gUtil.WritePacketSpam("Packet Info 2  - Time: %s, Protocol: [ %d ], Data: [ %x ], [ %s ], Len: [ %d ]\n",
					TextCurrenTime, byProtocol, *(BYTE*)pData, test, nLen);*/

					//return Execute_SendData_Host(This, pData, nLen);
				if (PacketCount > 0)
				{
					if (gUtil.CheckPacket(1, *((BYTE*)pData + 1), *((BYTE*)pData + 2), *((BYTE*)pData + 3)))
						return false;
					else
						return Execute_SendData_Host(This, pData, nLen);
				}

				if (byProtocol == 14)
				{
					if (nLen == 16)
					{
						return false;
						////gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
						//	//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

						///*gUtil.WritePacketSpam("Packet Spam  - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
						//	TextCurrenTime, byProtocol, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);*/

						//char Data2[nLen];

						//memcpy(Data2, (char*)pData + 4, nLen);

						//Data2[nLen] = 0;

						///*gUtil.WritePacketSpam(" Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
						//	TextCurrenTime, byProtocol, (char*)pData, (char*)pData + 1, Data2, (char*)pData + 3, (char*)pData + 4, nLen);*/

						//std::string s = Data2;

						//if (s.find('E') != std::string::npos)
						//{
						//	//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
						//		//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

						//	/*gUtil.WritePacketSpam(" Packet Spam 2 - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
						//		TextCurrenTime, byProtocol, (char*)pData, (char*)pData + 1, Data2, (char*)pData + 3, (char*)pData + 4, nLen);*/

						//	return false;
						//}
						//else
						//	return Execute_SendData_Host(This, pData, nLen);
					}
					else
						return Execute_SendData_Host(This, pData, nLen);
				}
				else if (byProtocol == 17)
				{
					if (nLen == 12)
					{
						//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
							//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

						/*gUtil.WritePacketSpam("Packet Spam  - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
							TextCurrenTime, byProtocol, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);*/

						return false;
					}
					else
						return Execute_SendData_Host(This, pData, nLen);

					//std::string s = (char*)pData + 1;
					//if (s.find('¤^N') != std::string::npos || s.find('>²h÷') != std::string::npos)
					//{
					//	//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
					//		//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

					//	gUtil.WritePacketSpam("Packet Spam  - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
					//		TextCurrenTime, byProtocol, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);

					//	return false;
					//}
					//else
					//	return Execute_SendData_Host(This, pData, nLen);
				}
				else
					return Execute_SendData_Host(This, pData, nLen);
			}
			catch (...)
			{
				return false;
			}
		}
		else
		{
			return Execute_SendData_Host(This, pData, nLen);
		}
	}
	catch (...)
	{
		return Execute_SendData_Host(This, pData, nLen);
	}
}

void __cdecl KChatProcessProcessMessage(KChatProcess* This, const void* pMsg, size_t size)
{
	time_t now;
	struct tm* timenow;
	now = time(0);
	timenow = gmtime(&now);
	char TextCurrenTime[100];
	char TextData[100];
	//KCONNECTION m_pConnection;

	strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

	BYTE byProtocol;
	byProtocol = *(BYTE*)pMsg;

	gUtil.WriteLogAll("Packet Info - Time: %s, Protocol: [ %d ], [ %x ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
		TextCurrenTime, byProtocol, (DWORD*)This, (char*)pMsg, ((char*)pMsg + 1), ((char*)pMsg + 1), (char*)pMsg + 2, (char*)pMsg + 3, (char*)pMsg + 4, (char*)pMsg + 22, size);

}

bool __cdecl ClientConnection_SendData_Chat(CClientConnection* This, const void* pData, unsigned int nLen)
{
	try
	{
		/*KIniFile FileHandle;
		Execute_KIniFile(&FileHandle);
		Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
		Execute_KIniFile_GetInteger(&FileHandle, "Setting", "Packet", 0, &gUtil.nPacket);
		Execute_KIniFile_Clear(&FileHandle);*/

		if (gUtil.nPacket > 0)
		{
			int PacketCount = 0;
			char NumPacket[2];
			char Buffer[32];
			//LPCSTR lpDefault = "0,0,0";
			KIniFile FileHandle1;
			Execute_KIniFile(&FileHandle1);
			Execute_KIniFile_Load(&FileHandle1, "servercfg.ini");
			Execute_KIniFile_GetInteger(&FileHandle1, "Packet3", "Count", 0, &PacketCount);
			//printf("Packet3 Count: %d\n", PacketCount);
			if (PacketCount > 0)
			{
				for (int i = 0; i < PacketCount; ++i)
				{
					memset(NumPacket, 0, 2);
					memset(Buffer, 0, 32);
					sprintf(NumPacket, "%d", i);
					Execute_KIniFile_GetString(&FileHandle1, "Packet3", NumPacket, "0,0,0", Buffer, sizeof(Buffer));
					gUtil.InsertHardwareId(1, Buffer);
					//printf("Buffer3: %s\n", Buffer);
				}
			}
			Execute_KIniFile_Clear(&FileHandle1);

			time_t now;
			struct tm* timenow;
			now = time(0);
			timenow = gmtime(&now);
			char TextCurrenTime[100];
			char TextData[100];
			//KCONNECTION m_pConnection;

			strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

			BYTE byProtocol;
			byProtocol = *(BYTE*)pData;
			//m_pConnection = *(KCONNECTION*)(const char*)This->tmp[0x14];

			//gUtil.WritePacketSpam("ProcessNet Info - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %s ], BYTE2: [ %s ], BYTE3: [ %s ], BYTE4: [ %s ], Len: [ %d ]\n",
				//TextCurrenTime, byProtocol, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);

			gUtil.WritePacketSpam("Packet 2 - Time: %s, Protocol: [ %d ], [ %d ], Data: [ %s ], Len: [ %d ]\n",
				TextCurrenTime, byProtocol, *((BYTE*)pData + 1), gUtil.hexStr((unsigned char*)pData, nLen).c_str(), nLen);

			if (PacketCount > 0)
			{
				if (gUtil.CheckPacket(2, *((BYTE*)pData + 1), *((BYTE*)pData + 2), *((BYTE*)pData + 3)))
					return false;
				else
					return Execute_SendData_Host(This, pData, nLen);
			}
			else
				return Execute_SendData_Host(This, pData, nLen);

			/*char test;

			test = *((BYTE*)pData + 1);

			gUtil.WritePacketSpam("Packet Info 2 - Time: %s, Protocol: [ %d ], Data: [ %x ], [ %s ], Len: [ %d ]\n",
				TextCurrenTime, byProtocol, *(BYTE*)pData, test, nLen);*/

				//gUtil.WriteLogAll("Info - Time: %s, nPlayerIndex: %d, HardwareId: [ %s ], [ %d ], [ %d ], [ %s ], [ %d ]\n",
				//	TextCurrenTime, byProtocol, m_KPlayerInfo[nPlayerIdx].m_Hwid, pData[3], pData[5], (const char*)&pProtocol[pProtocol[5] + 16], *((WORD*)pProtocol + 3));

				//std::string s = (char*)pData + 1;
				//if (s.find('!GM') != std::string::npos || s.find('GM') != std::string::npos)
				//{
				//	//gUtil.WritePacketSpam("Packet Spam 1 - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
				//		//TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);

				//	gUtil.WritePacketSpam("Block Chanel GM - Time: %s, Protocol: [ %d ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
				//		TextCurrenTime, byProtocol, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);

				//	return false;
				//}
				//else
				//	return Execute_SendData_Host(This, pData, nLen);

				//if (strcmp((char*)pData + 1, "!GM") == 0)
				//{
				//	/*gUtil.WritePacketSpam("Block Chanel GM - Time: %s, Protocol: [ %d ], Data: [ %s ], Text1: [ %s ], BYTE1: [ %d ], BYTE2: [ %d ], BYTE3: [ %d ], BYTE4: [ %d ], Len: [ %d ]\n",
				//		TextCurrenTime, byProtocol, (const char*)pData, (const char*)pData + 1, *(BYTE*)pData + 1, *(BYTE*)pData + 2, *(BYTE*)pData + 3, *(BYTE*)pData + 4, nLen);*/

				//	gUtil.WritePacketSpam("Block Chanel GM - Time: %s, Protocol: [ %d ], [ %x ], Data: [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], [ %s ], Len: [ %d ]\n",
				//		TextCurrenTime, byProtocol, (DWORD*)This, (char*)pData, ((char*)pData + 1), ((char*)pData + 1), (char*)pData + 2, (char*)pData + 3, (char*)pData + 4, nLen);

				//	return false;
				//}
				//else
				//	return Execute_SendData_Host(This, pData, nLen);
		}
		else
		{
			return Execute_SendData_Host(This, pData, nLen);
		}
	}
	catch (...)
	{
		return Execute_SendData_Host(This, pData, nLen);
	}
}

bool __cdecl SafeExecuteRelayScript(KNewProtocolProcess* This, const void* pData, size_t nLen)
{

	time_t now;
	struct tm* timenow;
	now = time(0);
	timenow = gmtime(&now);
	char TextCurrenTime[100];

	strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

	//BYTE byProtocol;
	//byProtocol = *pMsg;

	gUtil.WriteTextGMCommand("ProcessNet Info - Time: %s, Data: [ %s ], Len: [ %d ]\n",
		TextCurrenTime, pData, nLen);
	Execute_ExecuteRelayScript(This, pData, nLen);
}

bool __cdecl SafeTextMsgProcessGMCmd(int nPlayerIdx, const char* pGMCmd, int nLen)
{

	time_t now;
	struct tm* timenow;
	now = time(0);
	timenow = gmtime(&now);
	char TextCurrenTime[100];
	strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);
	//const char* pGMCmd;
	int nNewLen;
	const char* v3; // eax
	signed int v4; // eax
	int v6; // esi
	int v8; // edi
	int nTempLen; // [esp+18h] [ebp-30h]
	char szCmd[20]; // [esp+28h] [ebp-20h] BYREF
	TGameMaster_Command* v7;

	if (nLen <= 0 || !pGMCmd)
		return 0;

	v3 = strchr(pGMCmd, 32);
	if (v3)
	{
		v4 = v3 - pGMCmd;
		nTempLen = v4;
		if (v4 > 19)
			return 0;
		memcpy(szCmd, pGMCmd, v4);
		szCmd[nTempLen] = 0;
	}
	else
	{
		if (nLen > 19)
			return 0;
		memcpy(szCmd, pGMCmd, nLen);
		szCmd[nLen] = 0;
		nTempLen = nLen;
	}

	v6 = 0;
	v7 = (TGameMaster_Command*)0x82B7060;
	v8 = 0;
	while (strcmp(v7->Command, szCmd))
	{
		++v6;
		v8 += 24;
		++v7;
		if (v6 == 10)
			return 0;
	}

	gUtil.WriteTextGMCommand("Process GMCommand Info - Time: %s, nPlayerIndex: %d, HardwareId: [ %s ], Text: [ %s ], Data: [ %d ], Len: [ %d ] Command: [ %d ]\n",
		TextCurrenTime, nPlayerIdx, m_KPlayerInfo[nPlayerIdx].m_Hwid, pGMCmd, *(_DWORD*)pGMCmd, nLen, &pGMCmd[nTempLen + 1], *(EGameMasterCommand*)(v8 + 137064564));

	/*if (!pText || nLen <= 3 || *pText != 63 || *(_DWORD*)pText != 544040767 && *(_DWORD*)pText != 541935423)
		return 0;*/
	if (nPlayerIdx == -1)
		Execute_TextMsgProcessGMCmd(nPlayerIdx, pGMCmd, nLen);
	else
		return 0;
}

bool __cdecl TextGMFilter(int nPlayerIdx, const char* pText, int nLen)
{
	time_t now;
	struct tm* timenow;
	now = time(0);
	timenow = gmtime(&now);
	char TextCurrenTime[100];
	strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);
	const char* pGMCmd;
	int nNewLen;
	const char* v3; // eax
	signed int v4; // eax
	int v6; // esi
	int v8; // edi
	int nTempLen; // [esp+18h] [ebp-30h]
	char szCmd[20]; // [esp+28h] [ebp-20h] BYREF
	TGameMaster_Command* v7;

	pGMCmd = pText + 4;
	nNewLen = nLen - 4;

	if (nNewLen <= 0 || !pGMCmd)
		return 0;

	v3 = strchr(pGMCmd, 32);
	if (v3)
	{
		v4 = v3 - pGMCmd;
		nTempLen = v4;
		if (v4 > 19)
			return 0;
		memcpy(szCmd, pGMCmd, v4);
		szCmd[nTempLen] = 0;
	}
	else
	{
		if (nNewLen > 19)
			return 0;
		memcpy(szCmd, pGMCmd, nNewLen);
		szCmd[nNewLen] = 0;
		nTempLen = nNewLen;
	}

	v6 = 0;
	v7 = (TGameMaster_Command*)0x82B7060;
	v8 = 0;
	while (strcmp(v7->Command, szCmd))
	{
		++v6;
		v8 += 24;
		++v7;
		if (v6 == 10)
			return 0;
	}

	gUtil.WriteTextGMCommand("GMCommand Info - Time: %s, nPlayerIndex: %d, HardwareId: [ %s ], Text: [ %s ], Data: [ %d ], Len: [ %d ] Command: [ %d ]\n",
		TextCurrenTime, nPlayerIdx, m_KPlayerInfo[nPlayerIdx].m_Hwid, pText, *(_DWORD*)pText, nLen, &pGMCmd[nTempLen + 1], *(EGameMasterCommand*)(v8 + 137064564));

	/*if (!pText || nLen <= 3 || *pText != 63 || *(_DWORD*)pText != 544040767 && *(_DWORD*)pText != 541935423)
		return 0;*/

	return 0;
}

void KPlayerNew::Hook_DialogNpc()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);

	uintptr_t start = (uintptr_t)(void*)0x80F219C;

	uintptr_t pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &ExecDialogNpc);

}

void __cdecl ExecDialogNpc(KPlayer* This, BYTE* pProtocol)
{

	//int nActive;
	//int nTickCount;
	//int nLimitNPCTalkFlood;

	KIniFile FileHandle;
	Execute_KIniFile(&FileHandle);
	Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
	Execute_KIniFile_GetInteger(&FileHandle, "NPCTalk", "Active", 0, &gUtil.nNPCTalkActive);
	Execute_KIniFile_GetInteger(&FileHandle, "NPCTalk", "DelayNpcTalk", 0, &gUtil.nTickCount);
	Execute_KIniFile_GetInteger(&FileHandle, "NPCTalk", "NPCTalkFloodLimit", 0, &gUtil.nLimitNPCTalkFlood);
	Execute_KIniFile_Clear(&FileHandle);

	if (gUtil.nNPCTalkActive > 0)
	{
		int pPlayerIndex;
		time_t now;
		struct tm* timenow;
		now = time(0);
		timenow = gmtime(&now);
		char TextCurrenTime[100];
		strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

		pPlayerIndex = *(int*)(&This->tmp[0xC0]);

		if (pPlayerIndex >= 0 && pPlayerIndex < gUtil.nMaxPlayer)
		{

			if ((gUtil.GetCurrentTimestamp() - m_KPlayerInfo[pPlayerIndex].m_TickCount) >= gUtil.nTickCount)
			{
				m_KPlayerInfo[pPlayerIndex].m_TickCount = gUtil.GetCurrentTimestamp();
				Execute_DialogNpc(This, pProtocol);
			}
			else
			{
				m_KPlayerInfo[pPlayerIndex].m_NPCTalkFlood++;

				gUtil.WriteLogTalkNPC("DialogNpc Info - Time: %s, nPlayerIndex: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], TickCheck: %d, CurrenTick: %lld, Tick: %lld, FloodCount: %d\n",
					TextCurrenTime, pPlayerIndex
					, m_KPlayerInfo[pPlayerIndex].m_AccountName
					, m_KPlayerInfo[pPlayerIndex].m_PlayerName
					, m_KPlayerInfo[pPlayerIndex].m_Hwid
					, gUtil.nTickCount, gUtil.GetCurrentTimestamp(), m_KPlayerInfo[pPlayerIndex].m_TickCount, m_KPlayerInfo[pPlayerIndex].m_NPCTalkFlood);

				if (m_KPlayerInfo[pPlayerIndex].m_NPCTalkFlood >= gUtil.nLimitNPCTalkFlood)
				{
					gUtil.WriteLogTalkNPC("DialogNpc Flooder - Time: %s, nPlayerIndex: %d, Account: [ %s ], PlayerName: [ %s ], HardwareId: [ %s ], FloodCount: %d\n",
						TextCurrenTime, pPlayerIndex
						, m_KPlayerInfo[pPlayerIndex].m_AccountName
						, m_KPlayerInfo[pPlayerIndex].m_PlayerName
						, m_KPlayerInfo[pPlayerIndex].m_Hwid, m_KPlayerInfo[pPlayerIndex].m_NPCTalkFlood);
					Execute_KickOut(This, 5);
					//Execute_ShutdownClient(This);
				}
			}
		}
		else
		{
			gUtil.WriteLogTalkNPC("DialogNpc Overload - Time: %s, nPlayerIndex: %d, HardwareId: [ %s ]\n",
				TextCurrenTime, pPlayerIndex, m_KPlayerInfo[pPlayerIndex].m_Hwid);
		}
	}
	else
		Execute_DialogNpc(This, pProtocol);
}

void KPlayerNew::Hook_ServerSendChat()
{

	/*struct sigaction sa;
	struct sigaction old_action;

	memset(&sa, 0, sizeof(sa));
	memset(&old_action, 0, sizeof(old_action));
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = sigaction_player;
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sa, NULL);*/
	//if (gUtil.nCheckChatActive == 2)
	//{

		size_t pagesize = sysconf(_SC_PAGESIZE);
		uintptr_t start;
		uintptr_t pagestart;
	
		////==================================================

		start = (uintptr_t)(void*)0x81F7689;

		pagestart = start & -pagesize;

		if (mprotect((void*)pagestart, 0x6u,
			PROT_READ | PROT_WRITE | PROT_EXEC))
		{
			perror("mprotect");
			exit(EXIT_FAILURE);
		}

		gUtil.SetCompleteHook(0xE8, start, &SafeProcessRelayMsg);

	//}

	//signal(SIGSEGV, signal_handler);
	//mprotect((void*)0x82B0F78, 4, PROT_READ | PROT_WRITE | PROT_EXEC);
	//*(DWORD*)(0x82B0F78) = *(DWORD*)&SafeChatScreen;

	//mprotect((void*)0x82B0F80, 4, PROT_READ | PROT_WRITE | PROT_EXEC);
	//*(DWORD*)(0x82B0F80) = *(DWORD*)&SafeChatWorld;

	//gUtil.SetDword(0x82B0F78, (unsigned __int32)&SafeChatScreen);
	//gUtil.SetDword(0x82B0F80, (unsigned __int32)&SafeChatWorld);

	//==================================================

}

bool __cdecl sub_F644D90C(int a1)
{
	//printf("sub_F644D90C : %d\n", (unsigned __int8)(a1 - 1));
	//printf("sub_F644D90C : %d\n", a1);
	//printf("sub_F644D90C : %d\n", strlen((char*)a1));
	//printf("sub_F644D90C : %d\n", (int)a1);
	//printf("sub_F644D90C : %d\n", *(BYTE*)(strlen((char*)a1) - 1 + a1));

	if (a1 != 8)
		return 1;
	else
		return 0;
	//return 1;
}

bool __cdecl SafeChatScreen(KServerCore* This, DWORD dwIndex, const void* pMsg, size_t size)
{
	if (!pMsg || !size)
		return 0;

	time_t now;
	struct tm* timenow;
	now = time(0);
	timenow = gmtime(&now);
	char TextCurrenTime[100];
	strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

	std::string AllPacket = gUtil.hexStrArray((unsigned char*)pMsg, size);
	
	gUtil.WriteLogChat("[CHAT] Chat - Time: %s, AccountName: %s, PlayerName: %s, HardwareId: %s, Chat[%d]: [ %s ], Data: [ %s ]\n", TextCurrenTime, m_KPlayerInfo[dwIndex].m_AccountName
		, m_KPlayerInfo[dwIndex].m_PlayerName
		, m_KPlayerInfo[dwIndex].m_Hwid, size, pMsg, AllPacket.c_str());

	return Execute_ChatScreen(This, dwIndex, pMsg, size);

	

	//if (size <= 299)
	//{
	//	if (size >= 45)
	//	{
	//		

	//		//const void* pExPckg = pMsg;
	//		////DWORD byProtocol = *(DWORD*)(pExPckg);
	//		//unsigned __int8 v8 = *(BYTE*)(pMsg + 43);
	//		////printf("v8 1 : %d\n", v8);
	//		//if (v8 == 0xFF)
	//		//{
	//		//	gUtil.WriteLogChat("[ChatScreen] Chat length is exceeded [%s]\n", (const char*)(pExPckg + 7));
	//		//	return 0;
	//		//}
	//		//else
	//		//{
	//		//	BYTE* v7 = (BYTE*)(pExPckg + 44);
	//		//	//printf("v7 : %d\n", *(&v7[0]));
	//		//	//printf("v9 : %x\n", *(DWORD*)&pExPckg);
	//		//	if ((*(DWORD*)&pExPckg) != 0xFFFFFFD4 && *v7)
	//		//	{
	//		//		if (v7[v8 - 1] == 8)
	//		//		{
	//		//			v7[--v8] = 0;
	//		//			//--* (BYTE*)(pExPckg + 43);
	//		//			gUtil.WriteLogChat("[CHAT] BUG 1 [%s]\n", (const char*)(pExPckg + 7));
	//		//			return 0;
	//		//		}
	//		//		else
	//		//		{
	//		//			if (v8 <= 1u && v7[0] == 27)
	//		//			{
	//		//				gUtil.WriteLogChat("[CHAT] BUG 2 [%s]\n", (const char*)(pExPckg + 7));
	//		//				return 0;
	//		//			}
	//		//			else
	//		//				return Execute_ChatScreen(This, dwIndex, pMsg, size);
	//		//			//char v5[1024];
	//		//			//char checkByte[2];
	//		//			//memset(v5, 0, sizeof(v5));
	//		//			//memcpy(v5, v7, v8);
	//		//			//v5[v8] = 0;
	//		//			//int v6 = strlen(v5);
	//		//			////printf("v5 : %d, %d, %d, %d, %s\n", v6, (*(WORD*)((char*)v5 + v6 - 1)), (*(BYTE*)((char*)&v5[v6 - 1])), *(BYTE*)((char*)&v5[v6 - 1]), (char*)&v5[v6 - 1]);
	//		//			////printf("v5 : %d, %d, %d, %d\n", v6, (*(WORD*)((char*)v5 + v6 - 1)), (*(BYTE*)((char*)&v5[v6 - 1])), *(BYTE*)((char*)&v5[v6 - 1]));
	//		//			//std::string sPacket = gUtil.hexStr((unsigned char*)v5, v8);
	//		//			////sscanf(checkByte, "%02x", &v5[v8 - 1]);
	//		//			////printf("v5 : %d, %x , %s, %s , %d \n", (int)atoi(checkByte), (int)&v5, sPacket.c_str(), sPacket.substr((strlen(v5) * 2) - 2, 2).c_str(), atoi(sPacket.substr((strlen(v5) * 2) - 2, 2).c_str()));
	//		//			////printf("v6 : %d\n", v6);
	//		//			////printf("v8 : %d\n", v8);
	//		//			////unsigned __int8 v3 = *(BYTE*)(&v5[v6 - 1]);
	//		//			////v3 = 80;

	//		//			//if ((v8 <= 1u || v6 == 1) && v5[0] == 27)
	//		//			//{
	//		//			//	gUtil.WriteLogChat("[CHAT] BUG 2 [%s]\n", (const char*)(pExPckg + 7));
	//		//			//	return 0;
	//		//			//}
	//		//			//else
	//		//			//{
	//		//			//	if (v5[v6 - 1] == 8)
	//		//			//	{
	//		//			//		gUtil.WriteLogChat("[CHAT] BUG 3 [%s]\n", (const char*)(pExPckg + 7));
	//		//			//		return 0;
	//		//			//	}
	//		//			//	else
	//		//			//	{
	//		//			//		time_t now;
	//		//			//		struct tm* timenow;
	//		//			//		now = time(0);
	//		//			//		timenow = gmtime(&now);
	//		//			//		char TextCurrenTime[100];
	//		//			//		strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

	//		//			//		std::string AllPacket = gUtil.hexStr((unsigned char*)v5, v8);

	//		//			//		gUtil.WriteLogChat("[CHAT] - Time: %s, nIndex: %d, End: %d, Data: [ %s ]\n",
	//		//			//			TextCurrenTime, dwIndex, v5[v6 - 1], sPacket.c_str());

	//		//			//		return Execute_ChatScreen(This, dwIndex, pMsg, size);
	//		//			//	}
	//		//			//}
	//		//		}
	//		//	}
	//		//}
	//	}
	//	else
	//	{
	//		return 0;
	//	}
	//}
	//else
	//{
	//	gUtil.WriteLogChat("[CHAT] - Packet length is exceeded [%d]\n", dwIndex);
	//	return 0;
	//}
}

bool __cdecl SafeChatWorld(KServerCore* This, DWORD dwIndex, const void* pMsg, size_t size)
{
	if (!pMsg || !size)
		return 0;

	time_t now;
	struct tm* timenow;
	now = time(0);
	timenow = gmtime(&now);
	char TextCurrenTime[100];
	strftime(TextCurrenTime, sizeof(TextCurrenTime), "%Y-%m-%d %H:%M:%S", timenow);

	std::string AllPacket = gUtil.hexStrArray((unsigned char*)pMsg, size);
	
	gUtil.WriteLogChat("[CHAT] Chat - Time: %s, AccountName: %s, PlayerName: %s, HardwareId: %s, Chat[%d]: [ %s ], Data: [ %s ]\n", TextCurrenTime, m_KPlayerInfo[dwIndex].m_AccountName
		, m_KPlayerInfo[dwIndex].m_PlayerName
		, m_KPlayerInfo[dwIndex].m_Hwid, size, pMsg, AllPacket.c_str());

	return Execute_ChatWorld(This, dwIndex, pMsg, size);

}

bool __cdecl SafeProcessRelayMsg(KClientProcess* This, unsigned int lnID, int nIndex, const char* pData, size_t dataLength)
{
	if (gUtil.nCheckChatActive > 0 && gUtil.PACKETFILTER_ACTIVE > 0)
	{
		if (LOBYTE(*pData) == 251)
		{
			if (*(BYTE*)(pData + 3) == 33)
			{
				KIniFile FileHandle;
				Execute_KIniFile(&FileHandle);
				Execute_KIniFile_Load(&FileHandle, "servercfg.ini");
				Execute_KIniFile_GetInteger(&FileHandle, "Setting", "CheckBugChat", 1, &gUtil.nCheckChatActive);
				Execute_KIniFile_GetInteger(&FileHandle, "Setting", "LogChat", 1, &gUtil.nLogChat);
				Execute_KIniFile_Clear(&FileHandle);
				
				const void* pMsg = pData + 3;
				const char* pExPckg = pData + 3;
				std::string sPacket = gUtil.hexStr((unsigned char*)pData, dataLength);
				if (!pData || !dataLength)
				{
					return 1;
				}

				if (dataLength > 299)
				{
					return 1;
				}

				try
				{
					/*if (*((BYTE*)pMsg + 8) == 2 || *((BYTE*)pMsg + 8) == 4)
					{
						gUtil.WriteLogChat("[CHAT] - PlayerName: %s, HardwareId: %s, Chat[%d]: %s, Data: [ %s%d ]\n", m_KPlayerInfo[nIndex].m_PlayerName, m_KPlayerInfo[nIndex].m_Hwid, *((BYTE*)pExPckg + 13), (char*)pExPckg + 15, gUtil.hexStrArray((unsigned char*)pData, dataLength).c_str(), dataLength);
					}
					else
					{
						gUtil.WriteLogChat("[CHAT] - PlayerName: %s, HardwareId: %s, Data: [ %s%d ]\n", m_KPlayerInfo[nIndex].m_PlayerName, m_KPlayerInfo[nIndex].m_Hwid, gUtil.hexStrArray((unsigned char*)pData, dataLength).c_str(), dataLength);
					}*/

					if (gUtil.nLogChat > 0)
						gUtil.WriteLogChat("[CHAT] - PlayerName: %s, HardwareId: %s, Chat[%d]: %s, Data: [ %s%d ]\n", m_KPlayerInfo[nIndex].m_PlayerName, m_KPlayerInfo[nIndex].m_Hwid, *((BYTE*)pExPckg + 13), (char*)pExPckg + 15, gUtil.hexStrArray((unsigned char*)pData, dataLength).c_str(), dataLength);
				
				}
				catch (...)
				{
				}

				if (m_KPlayerInfo[nIndex].m_Hwid)
				{
					if (strlen(m_KPlayerInfo[nIndex].m_Hwid) <= 0)
					{
						try
						{
							gUtil.WriteLogChat("[CHAT] BUG Invalid HardwareId - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
								, m_KPlayerInfo[nIndex].m_PlayerName
								, m_KPlayerInfo[nIndex].m_Hwid);
						}
						catch (...)
						{
							return 1;
						}

						return 1;
					}
				}
				else
				{
					try
					{
						gUtil.WriteLogChat("[CHAT] BUG Invalid HardwareId - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
							, m_KPlayerInfo[nIndex].m_PlayerName
							, m_KPlayerInfo[nIndex].m_Hwid);
					}
					catch (...)
					{
						return 1;
					}

					return 1;
				}

				if (dataLength >= 45)
				{
					if ((int)pData[23] == 0xff || ((int)pData[39] == 0xff))
					{
						try
						{
							gUtil.WriteLogChat("[CHAT] Chat length is exceeded - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
								, m_KPlayerInfo[nIndex].m_PlayerName
								, m_KPlayerInfo[nIndex].m_Hwid);
						}
						catch (...)
						{
							return 1;
						}
						return 1;
					}
				}

				if ((int)pData[dataLength - 1] == 0x08)
				{
					try
					{
						gUtil.WriteLogChat("[CHAT] BUG #08 - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
							, m_KPlayerInfo[nIndex].m_PlayerName
							, m_KPlayerInfo[nIndex].m_Hwid);
					}
					catch (...)
					{
						return 1;
					}
					return 1;
				}

				if ((int)pData[dataLength - (1 + LOBYTE(*((BYTE*)pExPckg + 7)))] == 8)
				{
					try
					{
						gUtil.WriteLogChat("[CHAT] BUG #33 - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
							, m_KPlayerInfo[nIndex].m_PlayerName
							, m_KPlayerInfo[nIndex].m_Hwid);
					}
					catch (...)
					{
						return 1;
					}
					return 1;
				}

				if (LOBYTE(*((BYTE*)pExPckg + 13)) == 1 && LOBYTE(*((BYTE*)pExPckg + 15)) == 27)
				{
					try
					{
						gUtil.WriteLogChat("[CHAT] BUG #1 - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
							, m_KPlayerInfo[nIndex].m_PlayerName
							, m_KPlayerInfo[nIndex].m_Hwid);
					}
					catch (...)
					{
						return 1;
					}
					return 1;
				}
				else if (LOBYTE(*((BYTE*)pExPckg + 13)) > 149)
				{
					try
					{
						gUtil.WriteLogChat("[CHAT] BUG #2 - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
							, m_KPlayerInfo[nIndex].m_PlayerName
							, m_KPlayerInfo[nIndex].m_Hwid);
					}
					catch (...)
					{
						return 1;
					}

					return 1;
				}

				if (sPacket.find("ffffffff", 6) != std::string::npos)
				{
					if (sPacket.substr(25, 3).find("ff") != std::string::npos)
					{
						try
						{
							gUtil.WriteLogChat("[CHAT] BUG #316 - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
								, m_KPlayerInfo[nIndex].m_PlayerName
								, m_KPlayerInfo[nIndex].m_Hwid);
						}
						catch (...)
						{
							return 1;
						}
						return 1;
					}

					if (sPacket.find("21680", sPacket.size() - 8) != std::string::npos)
					{
						try
						{
							gUtil.WriteLogChat("[CHAT] BUG #3 - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
								, m_KPlayerInfo[nIndex].m_PlayerName
								, m_KPlayerInfo[nIndex].m_Hwid);
						}
						catch (...)
						{
							return 1;
						}
						return 1;
					}

					if (gUtil.nCheckChatActive == 2)
					{
						try
						{
							gUtil.WriteLogChat("[CHAT] - Data: [ %s%d ]\n", gUtil.hexStrArray((unsigned char*)pData, dataLength).c_str(), dataLength);
						}
						catch (...)
						{
						}
						return 1;
					}

					return Execute_ProcessRelayMsg(This, lnID, nIndex, pData, dataLength);

				}
				else
				{
					if (LOBYTE(*((BYTE*)pExPckg + 7)) == 1)
					{
						if (LOBYTE(*((BYTE*)pExPckg + 13)) == 1 && LOBYTE(*((BYTE*)pExPckg + 15)) == 27)
						{
							try
							{
								gUtil.WriteLogChat("[CHAT] BUG #1 - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
									, m_KPlayerInfo[nIndex].m_PlayerName
									, m_KPlayerInfo[nIndex].m_Hwid);
							}
							catch (...)
							{
								return 1;
							}
							return 1;
						}
						else if (LOBYTE(*((BYTE*)pExPckg + 13)) > 149)
						{
							try
							{
								gUtil.WriteLogChat("[CHAT] BUG #2 - AccountName: %s, PlayerName: %s, HardwareId: %s\n", m_KPlayerInfo[nIndex].m_AccountName
									, m_KPlayerInfo[nIndex].m_PlayerName
									, m_KPlayerInfo[nIndex].m_Hwid);
							}
							catch (...)
							{
								return 1;
							}

							return 1;
						}
					}
				}

				if (gUtil.nCheckChatActive == 2)
				{
					try
					{
						gUtil.WriteLogChat("[CHAT] - Data: [ %s%d ]\n", gUtil.hexStrArray((unsigned char*)pData, dataLength).c_str(), dataLength);
					}
					catch (...)
					{
					}
					return 1;
				}

				return Execute_ProcessRelayMsg(This, lnID, nIndex, pData, dataLength);

			}
			else
			{
				return Execute_ProcessRelayMsg(This, lnID, nIndex, pData, dataLength);
			}
		}
		else
		{
			return Execute_ProcessRelayMsg(This, lnID, nIndex, pData, dataLength);
		}
	}
	else
	{
		//return Execute_ProcessRelayMsg(This, lnID, nIndex, pData, dataLength);
		return 1;
	}
}

void KPlayerNew::Hook_PickUpMoney()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);

	uintptr_t start = (uintptr_t)(void*)0x80D1AD0;

	uintptr_t pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x2000u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeEarn);

}

bool __cdecl SafeEarn(KPlayer* This, int nMoney)
{
	bool runScript;
	int pPlayerIndex;
	int pScriptResult;
	pPlayerIndex = -1;
	runScript = 0;
	pScriptResult = 0;
	pPlayerIndex = *(int*)(&This->tmp[0xC0]);
	
	if (gUtil.nPickUpMoney > 0)
	{
		runScript = Execute_ExecuteScript_Name(This, gUtil.nScriptPickUpMoney, "PickUp", 1, "dd", pPlayerIndex, nMoney);
		pScriptResult = *(int*)(&This->tmp[0x27C00]);

		if (!runScript)
			return false;
		else
		{
			if (pScriptResult > 0)
				return Execute_KPlayer_Earn(This, pScriptResult);
			else
				return false;
		}
	}
	else
	{
		return Execute_KPlayer_Earn(This, nMoney);
	}
}

void __cdecl SafeSendSyncData(KNpc* This, int nHideMode)
{
	Execute_SyncNoChangeSpeed(This);
}

void __cdecl SafeClearHideState(KNpc* This)
{
	if (This->tmp[0x1878] > 0)
	{

		KPlayer* CurrenPlayer = Execute_GetMyPlayer(This);
		KNpc* CurrenNpc = Execute_GetMyNpc(CurrenPlayer);
		Execute_RemoveStateSkillEffect(CurrenNpc, 713, 1);
		mprotect((void*)CurrenNpc->tmp[0x1878], 0x6u, 7);
		CurrenNpc->tmp[0x1878] = -1;
		Execute_SetHide(CurrenNpc, 0);
		Execute_ReCalcState(CurrenPlayer);
		Execute_Npc_SendSyncData(CurrenNpc, CurrenPlayer);
		Execute_SyncNoChangeSpeed(CurrenNpc);
	}
	else
	{
		Execute_SetHide(This, 1);
	}
}

void __cdecl SafeRemoveStateSkillEffect(KNpc* This, int nSkillID, BOOL bSyncClient)
{
	if (nSkillID != 713)
	{
		Execute_RemoveStateSkillEffect(This, nSkillID, bSyncClient);
		Execute_SyncAttribute(This);
	}
	else
	{
		Execute_RemoveStateSkillEffect(This, nSkillID, bSyncClient);
	}
}

void KPlayerNew::Hook_StallCoin()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);

	uintptr_t start = (uintptr_t)(void*)0x80E251C;

	uintptr_t pagestart = start & -pagesize;

	//================================================

	pagesize = sysconf(_SC_PAGESIZE);

	start = (uintptr_t)(void*)0x80E2A35; //  80E251C;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeDealSellItem);

	pagesize = sysconf(_SC_PAGESIZE);

	start = (uintptr_t)(void*)0x80E2A6E; // 80E1C91;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeDealBuyItem);

	//==================================================

	pagesize = sysconf(_SC_PAGESIZE);

	start = (uintptr_t)(void*)0x80E2734;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeHasEnoughMoney);

	pagesize = sysconf(_SC_PAGESIZE);

	start = (uintptr_t)(void*)0x80E25C0;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeGetPlayerIdxFromID);

	//===================================================

	pagesize = sysconf(_SC_PAGESIZE);

	start = (uintptr_t)(void*)0x80E350A;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeSyncStallInfo);

	//===================================================

	pagesize = sysconf(_SC_PAGESIZE);

	start = (uintptr_t)(void*)0x80E3594;

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE9, start, &SafeSyncStallInfoGetPlayerIDFromIdx);

}

DWORD __cdecl SafeSyncStallInfoGetPlayerIDFromIdx(KPlayerStall* This, int nPlayerIdx)
{
	if (nPlayerIdx > 0)
	{
		int pSellPlayerIndex = *(int*)(&This->tmp[0x4]);
		m_KPlayerInfo[nPlayerIdx].m_SBuyType = 0;
		if (m_KPlayerInfo[pSellPlayerIndex].m_StallCoin > 0)
		{
			m_KPlayerInfo[nPlayerIdx].m_SBuyType = m_KPlayerInfo[pSellPlayerIndex].m_StallCoin;
		}
	}
	return Execute_SyncStallInfo(This, nPlayerIdx);
}

bool __cdecl SafeSyncStallInfo(KPlayer* This, void* pData, size_t nLen)
{
	int nPlayerIndex = *(int*)(&This->tmp[0xC0]);
	if (m_KPlayerInfo[nPlayerIndex].m_SBuyType > 0)
	{
		gUtil.SetByte((DWORD)((void*)pData + 3), *(BYTE*)(pData + 3) + 10);
	}
	else
	{
		gUtil.SetByte((DWORD)((void*)pData + 3), 2);
	}
	return Execute_SendData(This, pData, nLen);
}

int __cdecl SafeGetPlayerIdxFromID(KPlayerStall* This, DWORD dwPlayerID)
{
	int pBuyPlayerIndex = *(int*)(&This->tmp[0x4]);
	int pSellPlayerIndex = Execute_GetPlayerIdxFromID(This, dwPlayerID);
	m_KPlayerInfo[pBuyPlayerIndex].m_SBuyType = 0;

	if (pSellPlayerIndex > 0)
	{
		if (m_KPlayerInfo[pSellPlayerIndex].m_StallCoin > 0)
		{
			m_KPlayerInfo[pBuyPlayerIndex].m_SBuyType = m_KPlayerInfo[pSellPlayerIndex].m_StallCoin;
		}
	}

	return pSellPlayerIndex;
}

bool __cdecl SafeHasEnoughMoney(KPlayerStall* This, int nNeedMoney)
{
	int pPlayerIndex = *(int*)(&This->tmp[0x4]);
	if (m_KPlayerInfo[pPlayerIndex].m_SBuyType > 0)
	{
		KPlayer* CurrenPlayer = (KPlayer*)m_KPlayerInfo[pPlayerIndex].m_KPlayer;
		KItemList* p_m_ItemList = (KItemList*)&CurrenPlayer->tmp[0x2040C];
		int nCoin = Execute_GetEquipmentCoin(p_m_ItemList);
		if (nCoin < nNeedMoney)
		{
			const char* v3;
			char* szText = (char*)malloc(64);
			char* szCoinType = (char*)malloc(32);
			if (m_KPlayerInfo[pPlayerIndex].m_SBuyType == 0) { memcpy(szCoinType, (char*)"TiÒn v¹n", 10); }
			else if (m_KPlayerInfo[pPlayerIndex].m_SBuyType == 1) { memcpy(szCoinType, (char*)"Danh väng", 10); }
			else if (m_KPlayerInfo[pPlayerIndex].m_SBuyType == 2) { memcpy(szCoinType, (char*)"TiÒn ®ång", 12); }
			else if (m_KPlayerInfo[pPlayerIndex].m_SBuyType == 3) { memcpy(szCoinType, (char*)"TÝch lòy", 10); }
			else if (m_KPlayerInfo[pPlayerIndex].m_SBuyType == 4) { memcpy(szCoinType, (char*)"KNB", 3); }
			else if (m_KPlayerInfo[pPlayerIndex].m_SBuyType == 5) { memcpy(szCoinType, (char*)"Cèng hiÕn", 11); }
			else if (m_KPlayerInfo[pPlayerIndex].m_SBuyType == 6) { memcpy(szCoinType, (char*)"Vinh dù", 8); }
			sprintf(szText, "B¹n kh«ng ®ñ <color=green> %d <color> %s", nNeedMoney, szCoinType);
			free(szCoinType);
			v3 = szText;
			if (szText)
			{
				Execute_Msg2Player(CurrenPlayer, szText);
			}
			free(szText);
			return false;
		}
		return Execute_HasEnoughMoney(This, 0);
	}
	return Execute_HasEnoughMoney(This, nNeedMoney);
}

bool __cdecl SafeDealSellItem(KPlayerStall* This, int nIdx, int nFullPrice, int nTaxMoney)
{
	int pPlayerIndex = *(int*)(&This->tmp[0x4]);
	KItem* item = (KItem*)((void*)0x82BBD40);
	KPlayer* CurrenPlayer = (KPlayer*)m_KPlayerInfo[pPlayerIndex].m_KPlayer;
	if (m_KPlayerInfo[pPlayerIndex].m_StallCoin == 2)
	{
		bool result = Execute_DealSellItem(This, nIdx, 0, 0);
		KItem* item = (KItem*)((char*)0x82BBD40);
		item[nIdx].tmp[0x244] = m_KPlayerInfo[pPlayerIndex].m_StallCoin;

		KItemEnvCustom itemEnv;
		itemEnv.tmp[0x4] = *((char*)This);
		itemEnv.tmp[0x0] = reinterpret_cast<char>(*(char*)0x824D070);
		itemEnv.tmp[0x408] = *(char*)0x8263667;
		for (int i = 0; i < nFullPrice; i++)
		{
			int nItemIndex = Execute_KItemSet_Add(0x82BBE60, 4, 0, 0, 0, 0, 417, 0, 0, 4, 0, &itemEnv, 0, 0, 0);
			if (!Execute_AddItemOnBody(CurrenPlayer, nItemIndex, 1, 1, 0))
				Execute_Remove(0x82BBE60, nItemIndex, 0, 0, 1);
		}

		std::time_t timenow = std::time(nullptr);
		std::string TextCurrenTime = std::asctime(std::localtime(&timenow));
		std::string::size_type i = TextCurrenTime.find("\n");
		if (i != std::string::npos)
			TextCurrenTime.erase(i, TextCurrenTime.length());

		gUtil.WriteBayBan("Stall Coin Seller \t - \t [ %s ] \t nPlayerIdx: %d, Account: [ %s ], PlayerName: [ %s ], SellCoin: [ %d ], ItemIdx: [ %d ]\n",
			TextCurrenTime.c_str(),
			pPlayerIndex,
			m_KPlayerInfo[pPlayerIndex].m_AccountName,
			m_KPlayerInfo[pPlayerIndex].m_PlayerName,
			nFullPrice, nIdx);
		return result;
	}
	else
	{
		return Execute_DealSellItem(This, nIdx, nFullPrice, nTaxMoney);
	}
	return false;
}

bool __cdecl SafeDealBuyItem(KPlayerStall* This, int nIdx, int nX, int nY, int nPrice)
{
	int pPlayerIndex = This->tmp[0x4];
	if (m_KPlayerInfo[pPlayerIndex].m_SBuyType == 2)
	{
		KPlayer* CurrenPlayer = (KPlayer*)m_KPlayerInfo[pPlayerIndex].m_KPlayer;
		KItemList* p_m_ItemList = (KItemList*)&CurrenPlayer->tmp[0x2040C];
		Execute_PayCoin(p_m_ItemList, nPrice);

		std::time_t timenow = std::time(nullptr);
		std::string TextCurrenTime = std::asctime(std::localtime(&timenow));
		std::string::size_type i = TextCurrenTime.find("\n");
		if (i != std::string::npos)
			TextCurrenTime.erase(i, TextCurrenTime.length());

		gUtil.WriteBayBan("Stall Coin Player \t -\ t[ %s ] \t nPlayerIdx: %d, Account: [ %s ], PlayerName: [ %s ], IpAddress: [ %s ], HardwareId: [ %s ], BuyCoin: [ %d ], ItemIdx: [ %d ]\n",
			TextCurrenTime.c_str(),
			pPlayerIndex,
			m_KPlayerInfo[pPlayerIndex].m_AccountName,
			m_KPlayerInfo[pPlayerIndex].m_PlayerName,
			m_KPlayerInfo[pPlayerIndex].m_LanIP,
			m_KPlayerInfo[pPlayerIndex].m_Hwid,
			nPrice,
			nIdx);

		return Execute_DealBuyItem(This, nIdx, nX, nY, 0);
	}
	else
	{
		return Execute_DealBuyItem(This, nIdx, nX, nY, nPrice);
	}
	return false;
}

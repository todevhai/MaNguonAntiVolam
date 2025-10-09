#pragma once
#include "Defines.h"
#include "Util.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <sys/mman.h>
#include <thread>
#include <fstream>

//#define oRoleRankInitialize		((void(__cdecl*)(CRoleRank* This)) 0x8078FEE)
#define oRoleRankInitialize		((void(__cdecl*)(DWORD This)) 0x8078FEE)

struct GService
{
	char tmp[0x490C];
};

struct IMainUi
{
	char tmp[0x4];
};

struct CRoleRank
{
	char tmp[0x1C];
};

#pragma pack(push, 1)
struct __attribute__((packed)) __attribute__((aligned(1))) TRoleStat
{
	char Name[32];
	INT64 LevelExp;
	int Level;
	INT64 Money;
	int Sect;
	int FightExp;
	int KillNum;
	BYTE Sort;
	BYTE Valid;
	BYTE Gender;
	INT64 Fortune;
	int BossKilled;
	int LeagueWinner;
};

struct KSTATINFO
{
	TRoleStat LevelStat[1000];
	TRoleStat MoneyStat[1000];
	TRoleStat KillerStat[10];
	TRoleStat MoneyStatBySect[11][100];
	TRoleStat LevelStatBySect[11][100];
	__attribute__((packed)) __attribute__((aligned(1))) int SectPlayerNum[11];
	__attribute__((packed)) __attribute__((aligned(1))) int SectMoneyMost[11];
	__attribute__((packed)) __attribute__((aligned(1))) int SectLevelMost[11];
	TRoleStat LevelStatByCamp[5][10];
	TRoleStat MoneyStatByCamp[5][10];
	TRoleStat FortuneStat[10];
	TRoleStat BossKillerStat[10];
	TRoleStat LeagueWinnerStat[10];
};
#pragma pack(pop)

class KGoddess
{
public:
	void Hook_RoleNameFilter();
	void Hook_RoleRankInitialize();
public:
	CRoleRank* m_pRoleRank;
};

bool SafeCtlInitialize(GService* This, IMainUi* pUi);
void __cdecl SafeSetWorRank(CRoleRank* This, LPCSTR pszRoleName, int nWorRank);
void __cdecl SafeRoleRankInitialize(CRoleRank* This);
void* UpdateRoleRank(void* ptr);

extern KGoddess m_Goddess;
extern DWORD m_pRoleRank;
extern pthread_t UpdateRoleRankThread;
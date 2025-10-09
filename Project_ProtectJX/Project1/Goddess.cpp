#include "stdafx.h"
#include "Goddess.h"

DWORD m_pRoleRank;
KGoddess m_Goddess;

#define Execute_GetFullPath		((void(__cdecl*)(LPSTR pszPath, size_t nLen, LPCSTR pszFile)) 0x807BAB0)
#define Execute_LockRead		((bool(__cdecl*)(CRoleRank *This, LPCSTR pszFile, void *pRet, size_t nLen)) 0x8078224)
#define Execute_SetWorRank		((bool(__cdecl*)(CRoleRank *This, LPCSTR pszRoleName, int nWorRank)) 0x80784E0)

void KGoddess::Hook_RoleNameFilter()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);

	uintptr_t start = (uintptr_t)(void*)0x806DD30; // set rank

	uintptr_t pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeCtlInitialize); // loi nBuyIdx
}

void KGoddess::Hook_RoleRankInitialize()
{
	size_t pagesize = sysconf(_SC_PAGESIZE);

	uintptr_t start = (uintptr_t)(void*)0x806DDA9; // RoleRank Initialize

	uintptr_t pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeRoleRankInitialize); // loi nBuyIdx
	
	//===============
	/*
	start = (uintptr_t)(void*)0x80790AF; // SetWorRank

	pagestart = start & -pagesize;

	if (mprotect((void*)pagestart, 0x6u,
		PROT_READ | PROT_WRITE | PROT_EXEC))
	{
		perror("mprotect");
		exit(EXIT_FAILURE);
	}

	gUtil.SetCompleteHook(0xE8, start, &SafeSetWorRank); // SafeSetWorRank
	*/
	time_t now = time(nullptr);
	size_t Minutes = gUtil.nTimeUpdateRank;
	time_t newTime = now + (60 * Minutes);;
	gUtil.TimeUpdateRank = newTime;

	pthread_t thread_id;
	pthread_create(&thread_id, NULL, &UpdateRoleRank, NULL);

}

void __cdecl SafeSetWorRank(CRoleRank* This, LPCSTR pszRoleName, int nWorRank)
{
	//printf("Role: %s, Rank: %d\n", pszRoleName, nWorRank);
	Execute_SetWorRank(This, pszRoleName, nWorRank);
}

bool SafeCtlInitialize(GService* This, IMainUi* pUi)
{
	return true;
}

void __cdecl SafeRoleRankInitialize(CRoleRank* This)
{
	m_pRoleRank = (DWORD)This;
	oRoleRankInitialize((DWORD)This);
	printf("[ANTIVOLAM] LOAD SERVICE SUCCESSFULLY!\n");
	//if (m_Goddess.m_pRoleRank != NULL)
	//	printf("[ANTIVOLAM] LOAD SERVICE SUCCESSFULLY!\n");
}

void* UpdateRoleRank(void* ptr)
{

	while (true) {

		time_t now = time(nullptr);

		int NewValue = 0;

		if (now >= gUtil.TimeUpdateRank)
		{
			size_t Minutes = gUtil.nTimeUpdateRank;

			time_t newTime = now + (60 * Minutes);

			gUtil.TimeUpdateRank = newTime;

			oRoleRankInitialize(m_pRoleRank);

			printf("[JXGUARD] UPDATE RANK SUCCESSFULLY!\n");
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000 * 60));
	}

	return 0;

}
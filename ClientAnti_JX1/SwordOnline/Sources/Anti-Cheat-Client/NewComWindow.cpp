#include <KWin32.h>
#include "NewComWindow.h"
#include "string.h"

static KComClassFactory gs_Factory;

KComClassFactory* gGetComClassFactory()
{
	//gs_Factory = _ComClassFactory();
	gs_Factory = *(KComClassFactory*)_ComClassFactory();
	return &gs_Factory;
	//return _ComClassFactory();
}

#define MAX_CLASSINFO 100
//static KComClassInfo g_ClassInfs[MAX_CLASSINFO];
static KComClassInfo* g_ClassInfs = (KComClassInfo*)0x68D2D0;
static int g_nClassCount = *(int*)(void*)0x68D2C8;

KComClassFactory::KComClassFactory()
{
	g_ClassInfs = (KComClassInfo*)0x68D2D0;
	g_nClassCount = *(int*)(void*)0x68D2C8;
}

KComClassInfo* KComClassFactory::FindComClass(const char* szClassType)
{
	for (int i = 0; i < g_nClassCount; i++)
	{
		if (strcmp(szClassType, g_ClassInfs[i].szClassType) == 0)
			return g_ClassInfs + i;
	}

	return NULL;
}

KComClassInfo* KComClassFactory::FindComClass(const KComClassInfo* pDest)
{
	if (pDest == NULL)
		return NULL;
	return FindComClass(pDest->szClassType);
}

int KComClassFactory::RegisterComClass(const KComClassInfo& one)
{
	/*if (!FindComClass(one.szClassType))
	{
		if (g_nClassCount < MAX_CLASSINFO - 1)
		{
			g_ClassInfs[g_nClassCount] = one;
			g_nClassCount++;
			return g_nClassCount;
		}
	}

	return 0;*/
	return _RegisterComClass((const void*)&one);
}

KWndWindow* KComClassFactory::CreateComObject(const char* szClassType)
{
	KComClassInfo* p = FindComClass(szClassType);
	if (p && p->pfnCreateObject)
	{
		return p->pfnCreateObject();
	}

	return NULL;
}
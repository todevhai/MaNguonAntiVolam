#ifndef __COMWINDOW_H__
#define __COMWINDOW_H__
#include "Import.h"

class KWndWindow;

struct KComClassInfo
{
	char szClassType[32];
	KWndWindow* (*pfnCreateObject)();
};

class KComClassFactory
{
public:
	KComClassFactory();
	KComClassInfo* FindComClass(const char* szClassType);
	KComClassInfo* FindComClass(const KComClassInfo* pDest);
	KWndWindow* CreateComObject(const char* szClassType);
	int RegisterComClass(const KComClassInfo& one);
};

KComClassFactory* gGetComClassFactory();

#define DECLARE_COMCLASS(class_name) \
	static KComClassInfo m_one; \
	static void class_name::RegisterSelfClass() \
		{ gGetComClassFactory()->RegisterComClass(m_one); } \
	static KWndWindow* class_name::CreateObject() \
		{ return new class_name; } \
	static KComClassInfo* class_name::GetComClassInfo() \
		{ return &m_one; }

#define IMPLEMENT_COMCLASS(class_name) \
	KComClassInfo class_name::m_one = \
	{ #class_name,  class_name::CreateObject };


#endif // __COMWINDOW_H__
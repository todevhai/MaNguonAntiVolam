#include "stdafx.h"
#include "Protect.h"
#include "HackServerProtocol.h"
#include "Util.h"
#include "KItemEx.h"

#define ExecOperationRequest		((int(*)(DWORD* This, unsigned int uOper, unsigned int uParam, int nParam)) 0x53B770)

struct KUiObjAtContRegion : public KUiObjAtRegion
{
	union
	{
		UIOBJECT_CONTAINER	eContainer;
		int					nContainer;
	};
};

void AutoPickDropItem(DWORD* pItem)
{
	//ItemPos	Pos;
	//KUiObjAtContRegion	Pick, Drop;
	//Pick.Obj.uGenre = *pItem;
	//Pick.Obj.uId = pItem[1];
	//Pick.Region.h = pItem[2];
	//Pick.Region.v = pItem[3];
	//Pick.Region.Width = pItem[4];
	//Pick.Region.Height = pItem[5];
	//Pick.eContainer = UOC_ITEM_TAKE_WITH;

	//Drop.Obj.uGenre = 5;
	//Drop.Obj.uId = pItem[1];
	//Drop.Region.h = pItem[2];
	//Drop.Region.v = pItem[3];
	//Drop.Region.Width = pItem[4];
	//Drop.Region.Height = pItem[5];
	//Drop.eContainer = UOC_ITEM_GIVE;

	//CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo1, 0);
	//CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&pInfo2, 0);
	//CoreDataChanged(GDCNI_HOLD_OBJECT, (DWORD)&pInfo1, 1);
	//CoreDataChanged(GDCNI_OBJECT_CHANGED, (DWORD)&pInfo2, 1);

	//ExecOperationRequest((DWORD*)0x6DE314, 3, (unsigned int)(&Pick), (unsigned int)(&Drop));
	//ExecOperationRequest((DWORD*)0x6DE314, 67, (unsigned int)(&Pick), (unsigned int)(&Drop));
}
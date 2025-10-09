#pragma once

#pragma pack(push, 1)
struct KUiGameObject
{
	unsigned int uGenre;	//对象类属
	unsigned int uId;		//对象id
	//	int			 nData;		//与对象实例相关的某数据
};

struct KUiRegion
{
	int		h;		//左上角起点横坐标
	int		v;		//左上角起点纵坐标
	int		Width;	//区域横宽
	int		Height;	//区域纵宽
};

struct KUiObjAtRegion
{
	KUiGameObject	Obj;
	KUiRegion		Region;
};
#pragma (pop)
enum UIOBJECT_CONTAINER
{
	UOC_IN_HAND = 1,		//手中拿着
	UOC_GAMESPACE,			//游戏窗口
	UOC_IMMEDIA_ITEM,		//快捷物品
	UOC_IMMEDIA_SKILL,		//快捷武功0->右键武功，1,2...-> F1,F2...快捷武功
	UOC_ITEM_TAKE_WITH,		//随身携带
	UOC_TO_BE_TRADE,		//要被买卖，买卖面板上
	UOC_OTHER_TO_BE_TRADE,	//买卖面板上，别人要卖给自己的，
	UOC_EQUIPTMENT,			//身上装备
	UOC_NPC_SHOP,			//npc买卖场所
	UOC_MARKET,
	UOC_STORE_BOX,			//储物箱
	UOC_EX_BOX1,
	UOC_EX_BOX2,
	UOC_EX_BOX3,
	UOC_ITEM_EX,
	UOC_SKILL_LIST,			//列出全部拥有技能的窗口，技能窗口
	UOC_SKILL_TREE,			//左、右可用技能树
	UOC_ITEM_GIVE,
};

typedef struct
{
	int		nPlace;
	int		nX;
	int		nY;
} ItemPos;

class KItemEx
{
};


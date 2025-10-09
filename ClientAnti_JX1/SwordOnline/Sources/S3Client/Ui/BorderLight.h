#ifndef BORDERLIGHT_H_
#define BORDERLIGHT_H_
#pragma once


#include "../../Represent/iRepresent/KRepresentUnit.h"

enum eTypeItem // loai vat pham
{
    Normal = 0,
    BlueItem,
    PurpleItem,
    GoldItem,
    NumType
};
void DrawBorder(BOOL &bCountFrame, unsigned short &nFrame, unsigned long &ulTimeDelay, int nX, int nY, int nWidth, int nHeight, int nType = GoldItem);
#endif  
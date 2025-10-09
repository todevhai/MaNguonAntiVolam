#include "KWin32.h"
#include "../../Represent/iRepresent/iRepresentShell.h"
#include "BorderLight.h"


extern iRepresentShell*    g_pRepresentShell;

#define     FRAMEGOITEMBACK 1
#define  MAXFRAMEITEMBACK 120

unsigned int l_BlueBorder[] =
{    
    0x0a00002d,
    0x0a000032,
    0x0a000037,
    0x0a00003c,
    0x0a000041,
    0x0a000046,
    0x0a00004b,
    0x0a000050,
    0x0a000055,
    0x0a00005a,
    0x0a00005f,
    0x0a000064,
    0x0a000069,
};    

unsigned int l_PurpleBorder[] =
{    
    0x0a2d002d,
    0x0a320032,
    0x0a370037,
    0x0a3c003c,
    0x0a410041,
    0x0a460046,
    0x0a4b004b,
    0x0a500050,
    0x0a550055,
    0x0a5a005a,
    0x0a5f005f,
    0x0a640064,
    0x0a690069,
};

unsigned int l_GoldBorder[] =
{    
    0x0a2e2200,
    0x0a332600,
    0x0a382a00,
    0x0a3d2e00,
    0x0a403000,
    0x0a453400,
    0x0a4a3700,
    0x0a4f3b00,
    0x0a543f00,
    0x0a594300,
    0x0a5e4700,
    0x0a634b00,
    0x0a694e00,    
};  

void DrawBorder(BOOL &bCountFrame, unsigned short &nFrame, unsigned long &ulTimeDelay, int nX, int nY, int nWidth, int nHeight, int nType /*= GoldItem*/)
{    
    KRURect        Rect;
    
    switch(nType)
    {
        case BlueItem:
            Rect.Color.Color_dw = l_BlueBorder[nFrame];
            break;
        case PurpleItem:
            Rect.Color.Color_dw = l_PurpleBorder[nFrame];
            break;
        case GoldItem:
            Rect.Color.Color_dw = l_GoldBorder[nFrame];
            break;
        default:
            return;
            break;
    }

    if(GetTickCount() -  ulTimeDelay >= MAXFRAMEITEMBACK)
    {    
        ulTimeDelay = GetTickCount();
        if(bCountFrame == 0)
        {
            nFrame++;
            if(nFrame >= 12)
                bCountFrame = 1;
        }
        else
        {
            nFrame--;
            if(nFrame <= 0)
                bCountFrame = 0;
        }
        
        switch(nType)
        {
            case BlueItem:
                Rect.Color.Color_dw = l_BlueBorder[nFrame];
                break;
            case PurpleItem:
                Rect.Color.Color_dw = l_PurpleBorder[nFrame];
                break;
            case GoldItem:
                Rect.Color.Color_dw = l_GoldBorder[nFrame];
                break;
            default:
                return;
                break;
        }
    }
    for(int i = 0; i < 2; i++)
    {    
        Rect.oPosition.nX = nX + i - 1;
        Rect.oPosition.nY = nY + i - 1;
        Rect.oEndPos.nX = nX + nWidth - i;
        Rect.oEndPos.nY = nY + nHeight - i;
        Rect.oEndPos.nZ = Rect.oPosition.nZ = 0;
        g_pRepresentShell->DrawPrimitives(1, &Rect, RU_T_RECT, true);
        
    }    
} 
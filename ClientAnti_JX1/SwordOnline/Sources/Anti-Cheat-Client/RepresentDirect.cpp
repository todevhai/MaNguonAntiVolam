
#include "KEngine.h"
#include "RepresentDirect.h"
#include <ddraw.h>
#include <crtdbg.h.>

RepresentDirect::RepresentDirect()
{

}

void RepresentDirect::ReleaseAFont(int nId)
{
	for (int i = 0; i < RS2_MAX_FONT_ITEM_NUM; i++)
	{
		if (m_FontTable[i].nId == nId)
		{
			m_FontTable[i].nId = 0;
			if (m_FontTable[i].pFontObj)
			{
				m_FontTable[i].pFontObj->Release();
				m_FontTable[i].pFontObj = NULL;
			}
			break;
		}
	}
}
//---------------------------------------------------------------------------
// Kenh lenh dieu khien trong game. Xem KAutoControl.h.
//---------------------------------------------------------------------------
#include "KWin32.h"					// PCH: phai la include dau tien nhu moi .cpp trong project
#include "KEngine.h"					// g_DebugLog
#include "KAutoControl.h"
#include "../Ui/UiCase/UiStatus.h"
#include "../Ui/UiCase/UiSkills.h"
#include "../Ui/UiCase/UiItem.h"
#include "../Ui/UiCase/UiMiniMap.h"		// mo ban do lon (WORLD_MAP) de verify tim duong xa
#include "../Ui/Elem/Wnds.h"			// Wnd_ProcessInput: bom click vao cay cua so UI
#include "../../core/src/coreshell.h"
/* Cac .cpp trong Core deu nap KCore.h TRUOC roi moi toi KNpc.h - thieu no thi
   KNpcSet.h/KRegion.h khong co KClientNpcID, KIndexNode va vo hang loat. */
#include "../../core/src/KCore.h"
#include "../../core/src/KNpc.h"			// Npc[] - tim quai gan nhat cho lenh "danh"
#include "../../core/src/KPlayer.h"
#include "../../core/src/KPlayerDef.h"	// CLIENT_PLAYER_INDEX
#include "../../core/src/KSubWorld.h"	// Mps2Screen: doi toa do khong gian sang man hinh
#include "../../core/src/gamedatadef.h"	// PA_RIDE
#include <stdio.h>
#include <string.h>

extern iCoreShell*	g_pCoreShell;

// Ten file lenh, doc tu thu muc chay (cung cho voi config.ini / game.exe).
static const char* const AUTO_CMD_FILE = "lenh-auto.txt";

//---------------------------------------------------------------------------
// Mo mot bang dung nhu bam phim tat, nhung goi thang OpenWindow() nen luon MO
// (khong bat/tat lai) va khong qua cong kiem tra nao -- tien cho viec verify.
//---------------------------------------------------------------------------
void KAutoControl::OpenByName(const char* szName)
{
	if      (!strcmp(szName, "status")) KUiStatus::OpenWindow();
	else if (!strcmp(szName, "skills")) KUiSkills::OpenWindow();
	else if (!strcmp(szName, "items"))  KUiItem::OpenWindow();
	else if (!strcmp(szName, "worldmap"))
	{
		// Ban do LON (keo/phong duoc) - dung ca minimap user bam click di xa.
		// MapSetMode la ham tu do trong UiMiniMap.cpp (nut WorldMapBtn goi no).
		extern void MapSetMode(MINIMAP_MODE eMode);
		KUiMiniMap::OpenWindow();
		MapSetMode(MINIMAP_M_WORLD_MAP);
	}
	else	{ g_DebugLog("[AUTO] khong biet bang: %s", szName); return; }
	g_DebugLog("[AUTO] mo bang: %s", szName);
}

//---------------------------------------------------------------------------
// Phan tich mot dong lenh va thuc thi.
//---------------------------------------------------------------------------
void KAutoControl::RunLine(const char* szLine)
{
	char szCmd[32]  = {0};
	char szArg[224] = {0};
	int  n = sscanf(szLine, "%31s %223[^\r\n]", szCmd, szArg);
	if (n < 1) return;

	if (!strcmp(szCmd, "open"))
	{
		OpenByName(szArg);
	}
	else if (!strcmp(szCmd, "goto"))
	{
		int x = 0, y = 0;
		if (sscanf(szArg, "%d %d", &x, &y) == 2 && g_pCoreShell)
		{
			g_pCoreShell->GotoWhere(x, y, 0);		// mode 0 = auto
			g_DebugLog("[AUTO] goto %d,%d", x, y);
		}
	}
	else if (!strcmp(szCmd, "gotospace"))
	{
		// Bom thang toa do KHONG GIAN (world/mps) qua mode 10 -- dung duong ma
		// mot cu bam ban do lon sinh ra. Cho phep chon dich xa tuy y de test tim
		// duong (khong bi ke't o ban do nho). Mode 10 -> ServeMove -> AutoPathFindStep.
		int x = 0, y = 0;
		if (sscanf(szArg, "%d %d", &x, &y) == 2 && g_pCoreShell)
		{
			g_pCoreShell->GotoWhere(x, y, 10);
			g_DebugLog("[AUTO] gotospace %d,%d", x, y);
		}
	}
	else if (!strcmp(szCmd, "lclick") || !strcmp(szCmd, "rclick"))
	{
		// Bom click PASSIVE vao cay cua so UI tai toa do CLIENT (0..1024 x 0..768).
		// Wnd_ProcessInput doc x=LOWORD, y=HIWORD tu nParam va tu set con tro UI
		// noi bo -- KHONG dung con tro macOS, khong can frontmost. Dinh tuyen y
		// het cu click that (vd o tui: WM_RBUTTONDOWN -> mac/dung do).
		int x = 0, y = 0;
		if (sscanf(szArg, "%d %d", &x, &y) == 2)
		{
			bool bRight = (szCmd[0] == 'r');
			unsigned int uDown = bRight ? WM_RBUTTONDOWN : WM_LBUTTONDOWN;
			unsigned int uUp   = bRight ? WM_RBUTTONUP   : WM_LBUTTONUP;
			int nPos = (int)MAKELONG((short)x, (short)y);
			Wnd_ProcessInput(uDown, 0, nPos);
			Wnd_ProcessInput(uUp,   0, nPos);
			g_DebugLog("[AUTO] %s %d,%d", szCmd, x, y);
		}
	}
	else if (!strcmp(szCmd, "hover"))
	{
		// Re con tro UI toi (x,y) client -> hien tooltip mon do (xem option) ma
		// KHONG bam. Dung WM_MOUSEMOVE qua Wnd_ProcessInput (thu dong, khong focus).
		int x = 0, y = 0;
		if (sscanf(szArg, "%d %d", &x, &y) == 2)
		{
			Wnd_ProcessInput(WM_MOUSEMOVE, 0, (int)MAKELONG((short)x, (short)y));
			g_DebugLog("[AUTO] hover %d,%d", x, y);
		}
	}
	/* Danh con quai GAN NHAT. Bam chuot len quai la thao tac kho nhat khi
	   dieu khien tu ngoai: phai doan toa do man hinh cua no qua anh chup, lech
	   vai diem la thanh lenh DI. Day tim quai ngay trong bo nho roi bom dung
	   cu bam ay - cung duong ma lclick di qua, khong them loi tat nao. */
	else if (!strcmp(szCmd, "danh"))
	{
		int nMinh = Player[CLIENT_PLAYER_INDEX].m_nIndex;
		if (nMinh <= 0)
			return;
		int nX, nY;
		Npc[nMinh].GetMpsPos(&nX, &nY);
		int nGan = 0, nKhoangCachGan = 0;
		for (int i = 1; i < MAX_NPC; i++)
		{
			if (i == nMinh || Npc[i].m_Index <= 0)
				continue;
			if (Npc[i].m_Kind != kind_normal || Npc[i].m_Doing == do_death)
				continue;
			int nQx, nQy;
			Npc[i].GetMpsPos(&nQx, &nQy);
			int nDx = nQx - nX, nDy = nQy - nY;
			int nKc = nDx * nDx + nDy * nDy;
			if (nGan == 0 || nKc < nKhoangCachGan)
			{
				nGan = i;
				nKhoangCachGan = nKc;
			}
		}
		if (nGan == 0)
		{
			g_DebugLog("[AUTO] danh: khong thay quai nao");
			return;
		}
		int nSx, nSy;
		Npc[nGan].GetMpsPos(&nSx, &nSy);
		SubWorld[0].Mps2Screen(&nSx, &nSy);
		g_DebugLog("[AUTO] danh npc %d (%s) tai man hinh %d,%d",
			nGan, Npc[nGan].Name, nSx, nSy);
		int nPos = (int)MAKELONG((short)nSx, (short)nSy);
		Wnd_ProcessInput(WM_LBUTTONDOWN, 0, nPos);
		Wnd_ProcessInput(WM_LBUTTONUP,   0, nPos);
	}
	else if (!strcmp(szCmd, "ride"))
	{
		// Dung hanh dong cua phim M: Switch([[horse]]) -> PA_RIDE. Server quyet
		// co ngua (da mac) thi len/xuong. Test M ma khong bam phim (khong focus).
		if (g_pCoreShell)
		{
			g_pCoreShell->OperationRequest(GOI_PLAYER_ACTION, PA_RIDE, 0);
			g_DebugLog("[AUTO] ride (PA_RIDE)");
		}
	}
	else
	{
		g_DebugLog("[AUTO] lenh la: %s", szCmd);
	}
}

//---------------------------------------------------------------------------
// Doc file lenh (neu co), chay tung dong, roi cat rong file de moi lenh chi
// chay dung mot lan. Binh thuong file khong ton tai -> fopen NULL -> ve ngay,
// re, khong tra dia moi khung.
//---------------------------------------------------------------------------
void KAutoControl::Tick()
{
	FILE* fp = fopen(AUTO_CMD_FILE, "rb");
	if (!fp) return;

	char szLine[256];
	bool bAny = false;
	while (fgets(szLine, sizeof(szLine), fp))
	{
		char* p = szLine;
		while (*p == ' ' || *p == '\t') p++;			// bo khoang trang dau
		if (*p == '\r' || *p == '\n' || *p == 0) continue;	// bo dong trong
		RunLine(p);
		bAny = true;
	}
	fclose(fp);

	if (bAny)
	{
		FILE* fw = fopen(AUTO_CMD_FILE, "wb");			// cat rong
		if (fw) fclose(fw);
	}
}

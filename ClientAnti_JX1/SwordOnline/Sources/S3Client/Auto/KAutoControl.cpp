//---------------------------------------------------------------------------
// Kenh lenh dieu khien trong game. Xem KAutoControl.h.
//---------------------------------------------------------------------------
#include "KWin32.h"					// PCH: phai la include dau tien nhu moi .cpp trong project
#include "KEngine.h"					// g_DebugLog
#include "KAutoControl.h"
#include "../Ui/UiCase/UiStatus.h"
#include "../Ui/UiCase/UiSkills.h"
#include "../Ui/UiCase/UiItem.h"
#include "../../core/src/coreshell.h"
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

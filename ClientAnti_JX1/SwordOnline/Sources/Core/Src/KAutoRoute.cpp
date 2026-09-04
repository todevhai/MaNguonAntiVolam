// -------------------------------------------------------------------------
//  KAutoRoute.cpp  --  Tu tim duong LIEN ban do. Xem KAutoRoute.h.
//
//  Bang settings\autopathfindroutes.txt: moi dong la MOT tuyen, cot NODE_k =
//  "mapId,xDen,yDen,xRoi,yRoi". Don vi toa do = Mps/32 (cung don vi station.txt
//  va tham so SetPos/EnterNewWorld cua script) -> nhan 32 de ra Mps.
//
//  Moi cap node lien nhau cho HAI canh: o map[k] di toi (xRoi_k) thi sang map[k+1];
//  o map[k+1] di toi (xDen_k+1) thi ve map[k]. Tu 76 tuyen ra ~240 canh co huong
//  tren 143 map. Do thi KHONG lien thong toan cuc (23 thanh phan) -- moi thanh
//  phan la mot thanh + he dong cua no; thanh<->thanh phai di Xa phu.
//
//  File nguon cua ta (khong phai Kingsoft goc). Comment ASCII.
//  Luu y: tep nay chi duoc them vao build boi port-fixes.py, tuc luon bien dich
//  tren cay DA VA -- nen goi duoc helper AutoRouteGotoSpace ma ban va them vao.
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KCore.h"				// keo theo KDebug.h (g_DebugLog) + KTabFile.h
#include "KSubWorld.h"		// SubWorld[0].m_SubWorldID = map dang dung
#include "KAutoRoute.h"
#include <stdio.h>
#include <string.h>

#define AR_TABFILE		"\\settings\\autopathfindroutes.txt"
#define AR_MAX_EDGE		1024	// thuc te ~240, de rong phong bang lon hon
#define AR_MAX_MAP		256		// so map phan biet toi da khi BFS
#define AR_NODE_COL		10		// NODE_0 .. NODE_9
#define AR_MPS_PER_UNIT	32		// don vi bang -> Mps
#define AR_WAIT_FRAME	30		// cho sau khi sang map moi roi hay di tiep
#define AR_STALL_FRAME	600		// dung mai trong mot chang -> gui lai lenh di
#define AR_MAX_RESEND	3

// Canh cua do thi: o map nFrom, di toi (nX,nY) thi sang map nTo.
struct KRouteEdge
{
	short	nFrom, nTo;
	short	nX, nY;				// don vi bang (Mps/32)
};

static KRouteEdge	s_Edge[AR_MAX_EDGE];
static int			s_nEdge		= 0;
static int			s_bLoaded	= 0;

// Tuyen dang chay.
static int	s_RouteMap[AUTOROUTE_MAX_HOP];
static int	s_nRouteCnt	= 0;	// so map tren tuyen; 0 = khong co tuyen
static int	s_nRouteIdx	= 0;	// dang o map s_RouteMap[s_nRouteIdx]
static int	s_nWait		= 0;	// khung con phai cho sau khi sang map
static int	s_nStall	= 0;	// dem khung dung yen trong chang hien tai
static int	s_nResend	= 0;

// -------------------------------------------------------------------------
//  Nap bang + dung do thi (mot lan, luoi -- goi truoc lan dung dau tien).
// -------------------------------------------------------------------------
static void ArAddEdge(int nFrom, int nTo, int nX, int nY)
{
	if (s_nEdge >= AR_MAX_EDGE)
		return;
	s_Edge[s_nEdge].nFrom	= (short)nFrom;
	s_Edge[s_nEdge].nTo		= (short)nTo;
	s_Edge[s_nEdge].nX		= (short)nX;
	s_Edge[s_nEdge].nY		= (short)nY;
	s_nEdge++;
}

static void ArLoad()
{
	if (s_bLoaded)
		return;
	s_bLoaded = 1;

	KTabFile	Tab;
	if (!Tab.Load(AR_TABFILE))
	{
		g_DebugLog("[AUTOROUTE] khong nap duoc %s", AR_TABFILE);
		return;
	}

	// GetString(nRow,...) goi GetValue(nRow - 1,...) -> nRow 1 la dong TIEU DE,
	// du lieu chay tu nRow 2 den GetHeight(). Dem nham o day thi hut dong cuoi.
	int		nRows = Tab.GetHeight();
	char	szCol[16], szVal[64], szDef[2];
	szDef[0] = 0;

	for (int r = 2; r <= nRows; r++)
	{
		int nPrevMap = 0, nPrevLx = 0, nPrevLy = 0, bHavePrev = 0;

		for (int k = 0; k < AR_NODE_COL; k++)
		{
			sprintf(szCol, "NODE_%d", k);
			if (!Tab.GetString(r, szCol, szDef, szVal, sizeof(szVal)))
				break;						// o trong -> het node cua tuyen nay

			int nMap = 0, nAx = 0, nAy = 0, nLx = 0, nLy = 0;
			if (sscanf(szVal, "%d,%d,%d,%d,%d", &nMap, &nAx, &nAy, &nLx, &nLy) != 5)
				break;

			if (bHavePrev)
			{
				ArAddEdge(nPrevMap, nMap, nPrevLx, nPrevLy);	// di: diem ROI cua node truoc
				ArAddEdge(nMap, nPrevMap, nAx, nAy);			// ve: diem DEN cua node nay
			}
			nPrevMap = nMap; nPrevLx = nLx; nPrevLy = nLy; bHavePrev = 1;
		}
	}
	g_DebugLog("[AUTOROUTE] nap %s: %d dong, %d canh", AR_TABFILE, nRows - 1, s_nEdge);
}

// -------------------------------------------------------------------------
//  BFS tren do thi canh. Tra ve so map tren duong di (>=2), 0 neu khong toi duoc.
// -------------------------------------------------------------------------
static int ArBfs(int nFrom, int nTo, int* pOutMap, int nMaxOut)
{
	static int	aMap[AR_MAX_MAP], aPre[AR_MAX_MAP], aQueue[AR_MAX_MAP];
	int			nCnt = 0, nHead = 0, nTail = 0, nFound = -1;

	aMap[0] = nFrom; aPre[0] = -1; aQueue[nTail++] = 0; nCnt = 1;

	while (nHead < nTail && nFound < 0)
	{
		int nCurIdx = aQueue[nHead++];
		int nCurMap = aMap[nCurIdx];

		for (int e = 0; e < s_nEdge && nFound < 0; e++)
		{
			if (s_Edge[e].nFrom != nCurMap)
				continue;

			int nNext = s_Edge[e].nTo, j;
			for (j = 0; j < nCnt; j++)
				if (aMap[j] == nNext)
					break;
			if (j < nCnt || nCnt >= AR_MAX_MAP)
				continue;						// da tham / het cho

			aMap[nCnt] = nNext; aPre[nCnt] = nCurIdx; aQueue[nTail++] = nCnt;
			nCnt++;
			if (nNext == nTo)
				nFound = nCnt - 1;
		}
	}
	if (nFound < 0)
		return 0;

	int aTmp[AR_MAX_MAP], n = 0;
	for (int u = nFound; u >= 0; u = aPre[u])
		aTmp[n++] = aMap[u];
	if (n > nMaxOut)
	{
		g_DebugLog("[AUTOROUTE] tuyen %d chang, dai qua %d -> bo", n, nMaxOut);
		return 0;
	}
	for (int i = 0; i < n; i++)
		pOutMap[i] = aTmp[n - 1 - i];			// dao lai cho dung chieu di
	return n;
}

static int ArFindEdgePoint(int nFrom, int nTo, int* pnX, int* pnY)
{
	for (int e = 0; e < s_nEdge; e++)
		if (s_Edge[e].nFrom == nFrom && s_Edge[e].nTo == nTo)
		{
			*pnX = s_Edge[e].nX; *pnY = s_Edge[e].nY;
			return 1;
		}
	return 0;
}

// -------------------------------------------------------------------------
//  Di toi mot diem Mps trong map hien tai.
//
//  KHONG tu viet lai phan di lai: GotoWhere da co san nhanh mode >= 10 nghia la
//  "toa do truyen vao DA LA toa do khong gian", va nhanh do da chay A* + luu
//  waypoint + bao server tung waypoint. Ta chi can goi lai qua helper
//  AutoRouteGotoSpace (port-fixes them vao CoreShell.cpp, noi thay g_CoreShell).
//  Helper cung go throttle m_nSendMoveFrames -- neu khong, lenh dau chang co the
//  bi throttle nuot IM LANG va tuyen dung yen cho toi luc dem treo kick vao.
// -------------------------------------------------------------------------
extern void AutoRouteGotoSpace(int nMpsX, int nMpsY);

// Bat dau chang hien tai. KHONG dung toi s_nResend (nguoi goi lo).
static void ArStartLeg()
{
	if (s_nRouteIdx + 1 >= s_nRouteCnt)
	{
		g_DebugLog("[AUTOROUTE] xong, dang o map %d", s_RouteMap[s_nRouteIdx]);
		s_nRouteCnt = 0;
		return;
	}

	int nFrom = s_RouteMap[s_nRouteIdx], nTo = s_RouteMap[s_nRouteIdx + 1], nX = 0, nY = 0;
	if (!ArFindEdgePoint(nFrom, nTo, &nX, &nY))
	{
		AutoRouteCancel("khong tim thay canh giua hai map");
		return;
	}
	s_nStall = 0;
	g_DebugLog("[AUTOROUTE] chang %d/%d: map %d -> %d, diem bang %d,%d",
			   s_nRouteIdx + 1, s_nRouteCnt - 1, nFrom, nTo, nX, nY);
	AutoRouteGotoSpace(nX * AR_MPS_PER_UNIT, nY * AR_MPS_PER_UNIT);
}

// -------------------------------------------------------------------------
int AutoRouteStart(int nToMapId)
{
	ArLoad();
	s_nRouteCnt = 0;

	int nCur = SubWorld[0].m_SubWorldID;
	if (nCur == nToMapId)
	{
		g_DebugLog("[AUTOROUTE] dang o map %d roi, khong can di", nCur);
		return 0;
	}

	int n = ArBfs(nCur, nToMapId, s_RouteMap, AUTOROUTE_MAX_HOP);
	if (n < 2)
	{
		g_DebugLog("[AUTOROUTE] khong co tuyen %d -> %d (bang %d canh)", nCur, nToMapId, s_nEdge);
		return 0;
	}

	char szPath[128]; szPath[0] = 0;
	for (int i = 0; i < n && strlen(szPath) < sizeof(szPath) - 8; i++)
		sprintf(szPath + strlen(szPath), i ? ",%d" : "%d", s_RouteMap[i]);
	g_DebugLog("[AUTOROUTE] tuyen %d chang: %s", n - 1, szPath);

	s_nRouteCnt = n; s_nRouteIdx = 0; s_nWait = 0; s_nStall = 0; s_nResend = 0;
	ArStartLeg();
	return n;
}

void AutoRouteCancel(const char* szWhy)
{
	if (s_nRouteCnt <= 0)
		return;
	g_DebugLog("[AUTOROUTE] huy tuyen: %s (dang o map %d, chang %d/%d)",
			   szWhy ? szWhy : "?", SubWorld[0].m_SubWorldID, s_nRouteIdx + 1, s_nRouteCnt - 1);
	s_nRouteCnt = 0; s_nRouteIdx = 0; s_nWait = 0; s_nStall = 0; s_nResend = 0;
}

void AutoRouteTick()
{
	if (s_nRouteCnt <= 0)
		return;

	if (s_nWait > 0)						// vua sang map: cho region nap xong
	{
		if (--s_nWait == 0)
			ArStartLeg();
		return;
	}

	int nCur = SubWorld[0].m_SubWorldID;

	if (nCur == s_RouteMap[s_nRouteIdx])	// van o map cu -> dang di toi trap
	{
		if (++s_nStall > AR_STALL_FRAME)
		{
			s_nStall = 0;
			if (++s_nResend > AR_MAX_RESEND)
			{
				AutoRouteCancel("gui lai nhieu lan van khong sang duoc map");
				return;
			}
			g_DebugLog("[AUTOROUTE] treo o map %d -> gui lai lan %d", nCur, s_nResend);
			ArStartLeg();
		}
		return;
	}

	if (s_nRouteIdx + 1 < s_nRouteCnt && nCur == s_RouteMap[s_nRouteIdx + 1])
	{
		s_nRouteIdx++;
		s_nResend = 0;
		if (s_nRouteIdx + 1 >= s_nRouteCnt)
		{
			g_DebugLog("[AUTOROUTE] xong, da toi map %d", nCur);
			s_nRouteCnt = 0;
			return;
		}
		s_nWait = AR_WAIT_FRAME;
		g_DebugLog("[AUTOROUTE] sang map %d, cho %d khung roi di tiep", nCur, s_nWait);
		return;
	}

	AutoRouteCancel("map la (chet / dich chuyen ngoai tuyen)");
}

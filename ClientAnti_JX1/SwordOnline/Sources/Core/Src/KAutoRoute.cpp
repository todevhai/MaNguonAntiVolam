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
#define AR_STATION_TAB	"\\settings\\station.txt"		// 16 tram Xa phu + vi tri NPC
#define AR_STPRICE_TAB	"\\settings\\stationprice.txt"	// ma tran gia: -1 = khong co tuyen
#define AR_MAX_STATION	32
#define AR_MAX_STPOS	4		// so vi tri Xa phu toi da moi thanh (station.txt co SECT1..4)
#define AR_NAME_LEN		24
#define AR_MAX_EDGE		1024	// thuc te ~240, de rong phong bang lon hon
#define AR_MAX_MAP		256		// so map phan biet toi da khi BFS
#define AR_NODE_COL		10		// NODE_0 .. NODE_9
#define AR_MPS_PER_UNIT	32		// don vi bang -> Mps
#define AR_WAIT_FRAME	30		// cho sau khi sang map moi roi hay di tiep
#define AR_STALL_FRAME	600		// dung mai trong mot chang -> gui lai lenh di
#define AR_MAX_RESEND	3

// Canh cua do thi: o map nFrom, di toi (nX,nY) thi sang map nTo.
// bStation = 1: khong phai di bo dam trap ma la BAT XA PHU -- (nX,nY) la cho
// dung cua NPC Xa phu, va nguoi choi phai tu chon thanh trong hop thoai
// (menu loc theo "nhung thanh thi da di qua", tuy tung nguoi -> khong tu bam duoc).
struct KRouteEdge
{
	short	nFrom, nTo;
	short	nX, nY;				// don vi bang (Mps/32)
	short	bStation;
	short	nToStation;			// chi so tram dich, de goi ten trong loi nhac
};

// Mot tram Xa phu (mot thanh).
struct KRouteStation
{
	short	nMap;
	short	nCnt;
	short	nX[AR_MAX_STPOS], nY[AR_MAX_STPOS];
	char	szName[AR_NAME_LEN];
};

static KRouteEdge		s_Edge[AR_MAX_EDGE];
static int				s_nEdge		= 0;
static int				s_bLoaded	= 0;
static KRouteStation	s_Station[AR_MAX_STATION];
static int				s_nStation	= 0;

// Tuyen dang chay.
static int	s_RouteMap[AUTOROUTE_MAX_HOP];
static int	s_nRouteCnt	= 0;	// so map tren tuyen; 0 = khong co tuyen
static int	s_nRouteIdx	= 0;	// dang o map s_RouteMap[s_nRouteIdx]
static int	s_nWait		= 0;	// khung con phai cho sau khi sang map
static int	s_nStall	= 0;	// dem khung dung yen trong chang hien tai
static int	s_nResend	= 0;
static int	s_bChoXaPhu	= 0;	// dang dung o Xa phu doi nguoi choi tu chon thanh

// -------------------------------------------------------------------------
//  Nap bang + dung do thi (mot lan, luoi -- goi truoc lan dung dau tien).
// -------------------------------------------------------------------------
static void ArAddEdge(int nFrom, int nTo, int nX, int nY, int bStation, int nToStation)
{
	if (s_nEdge >= AR_MAX_EDGE)
		return;
	s_Edge[s_nEdge].nFrom		= (short)nFrom;
	s_Edge[s_nEdge].nTo			= (short)nTo;
	s_Edge[s_nEdge].nX			= (short)nX;
	s_Edge[s_nEdge].nY			= (short)nY;
	s_Edge[s_nEdge].bStation	= (short)bStation;
	s_Edge[s_nEdge].nToStation	= (short)nToStation;
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
				ArAddEdge(nPrevMap, nMap, nPrevLx, nPrevLy, 0, -1);	// di: diem ROI cua node truoc
				ArAddEdge(nMap, nPrevMap, nAx, nAy, 0, -1);			// ve: diem DEN cua node nay
			}
			nPrevMap = nMap; nPrevLx = nLx; nPrevLy = nLy; bHavePrev = 1;
		}
	}
	int nEdgeWalk = s_nEdge;

	// --- Tram Xa phu: noi cac thanh phan lien thong lai voi nhau ---------------
	// station.txt   : ID | DESC | COUNT | SECT1..SECT4, moi SECT = "map, x, y"
	// stationprice.txt: ma tran gia, dong i / cot j deu theo dung thu tu station.txt,
	//                   -1 = khong co tuyen truc tiep.
	KTabFile	St;
	if (!St.Load(AR_STATION_TAB))
	{
		g_DebugLog("[AUTOROUTE] khong nap duoc %s -> bo qua Xa phu", AR_STATION_TAB);
		return;
	}
	int nStRows = St.GetHeight();
	for (int i = 2; i <= nStRows && s_nStation < AR_MAX_STATION; i++)
	{
		KRouteStation* pSt = &s_Station[s_nStation];
		pSt->nMap = -1; pSt->nCnt = 0; pSt->szName[0] = 0;
		St.GetString(i, (LPSTR)"DESC", szDef, pSt->szName, AR_NAME_LEN);

		for (int k = 1; k <= AR_MAX_STPOS; k++)
		{
			sprintf(szCol, "SECT%d", k);
			if (!St.GetString(i, szCol, szDef, szVal, sizeof(szVal)))
				break;
			int nMap = 0, nX = 0, nY = 0;
			if (sscanf(szVal, "%d,%d,%d", &nMap, &nX, &nY) != 3)
				break;
			pSt->nMap = (short)nMap;
			pSt->nX[pSt->nCnt] = (short)nX;
			pSt->nY[pSt->nCnt] = (short)nY;
			pSt->nCnt++;
		}
		if (pSt->nMap >= 0 && pSt->nCnt > 0)
			s_nStation++;
	}

	KTabFile	Pr;
	if (!Pr.Load(AR_STPRICE_TAB))
	{
		g_DebugLog("[AUTOROUTE] khong nap duoc %s -> bo qua Xa phu", AR_STPRICE_TAB);
		return;
	}
	// Dong 1 la tieu de; tram i nam o dong i+2, gia sang tram j o cot j+2
	// (cot 1 la ten tram). Chi them canh MOT chieu i->j theo dung o ma tran.
	for (int i = 0; i < s_nStation; i++)
		for (int j = 0; j < s_nStation; j++)
		{
			if (i == j)
				continue;
			int nPrice = -1;
			if (!Pr.GetInteger(i + 2, j + 2, -1, &nPrice) || nPrice < 0)
				continue;
			if (s_Station[i].nMap == s_Station[j].nMap)
				continue;
			ArAddEdge(s_Station[i].nMap, s_Station[j].nMap,
					  s_Station[i].nX[0], s_Station[i].nY[0], 1, j);
		}

	g_DebugLog("[AUTOROUTE] nap xong: %d canh di bo + %d canh Xa phu (%d tram)",
			   nEdgeWalk, s_nEdge - nEdgeWalk, s_nStation);
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

// Tra ve canh dau tien noi hai map. Canh di bo duoc uu tien hon canh Xa phu vi
// duoc them vao truoc -- di bo thi khong ton tien va khong can nguoi choi bam gi.
static const KRouteEdge* ArFindEdge(int nFrom, int nTo)
{
	for (int e = 0; e < s_nEdge; e++)
		if (s_Edge[e].nFrom == nFrom && s_Edge[e].nTo == nTo)
			return &s_Edge[e];
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
extern void AutoRouteSay(const char* szMsg);	// nhan mot dong vao khung he thong

// Bat dau chang hien tai. KHONG dung toi s_nResend (nguoi goi lo).
static void ArStartLeg()
{
	if (s_nRouteIdx + 1 >= s_nRouteCnt)
	{
		g_DebugLog("[AUTOROUTE] xong, dang o map %d", s_RouteMap[s_nRouteIdx]);
		s_nRouteCnt = 0;
		return;
	}

	int nFrom = s_RouteMap[s_nRouteIdx], nTo = s_RouteMap[s_nRouteIdx + 1];
	const KRouteEdge* pE = ArFindEdge(nFrom, nTo);
	if (!pE)
	{
		AutoRouteCancel("khong tim thay canh giua hai map");
		return;
	}
	s_nStall = 0;
	s_bChoXaPhu = pE->bStation;
	g_DebugLog("[AUTOROUTE] chang %d/%d: map %d -> %d, diem bang %d,%d%s",
			   s_nRouteIdx + 1, s_nRouteCnt - 1, nFrom, nTo, pE->nX, pE->nY,
			   pE->bStation ? " (Xa phu)" : "");
	AutoRouteGotoSpace(pE->nX * AR_MPS_PER_UNIT, pE->nY * AR_MPS_PER_UNIT);

	if (pE->bStation)
	{
		// Menu Xa phu loc theo "nhung thanh thi da di qua" cua tung nguoi choi va
		// con tru tien -> KHONG tu bam ho. Chi dan nguoi choi toi noi roi nhac mot
		// cau; sang duoc thanh kia thi AutoRouteTick thay doi map va di tiep.
		char szMsg[128];
		const char* szTo = (pE->nToStation >= 0 && pE->nToStation < s_nStation)
						   ? s_Station[pE->nToStation].szName : "";
		sprintf(szMsg, "Toi Xa phu roi. Hay chon %s de di tiep.", szTo);
		AutoRouteSay(szMsg);
	}
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
	s_bChoXaPhu = 0;
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
	s_bChoXaPhu = 0;
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
		// Chang Xa phu: doi nguoi choi tu bam hop thoai, doi bao lau cung duoc.
		// Khong gui lai lenh di (se keo nguoi choi ra khoi NPC giua chung hop thoai)
		// va khong tu huy -- muon bo thi click di cho khac, GotoWhere se huy.
		if (s_bChoXaPhu)
			return;

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
		s_bChoXaPhu = 0;
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

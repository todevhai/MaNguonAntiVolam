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
#define AR_ARRIVE_MPS	96		// coi nhu da toi (3 o) -- GetDir dung tu truoc do
#define AR_MAX_CHO		3		// so lan bao "dang cho" truoc khi bo (chang di bo)
#define AR_CHO_FRAME	900		// nhip bao "dang cho" (~30 giay), dung noi qua day

// Canh cua do thi: o map nFrom, di toi (nX,nY) thi sang map nTo.
// bStation = 1: khong phai di bo dam trap ma la BAT XA PHU -- (nX,nY) la cho
// dung cua NPC Xa phu, va nguoi choi phai tu chon thanh trong hop thoai
// (menu loc theo "nhung thanh thi da di qua", tuy tung nguoi -> khong tu bam duoc).
struct KRouteEdge
{
	short	nFrom, nTo;
	short	nX, nY;				// don vi bang (Mps/32)
	short	bStation;
	short	nFromStation;		// chi so tram nguon (-1 neu la canh di bo)
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
static int	s_bChoXaPhu	= 0;	// dang dung o Xa phu doi nguoi choi tu chon thanh
static int	s_nGoalX	= 0;	// dich CUOI cua chang hien tai (Mps)
static int	s_nGoalY	= 0;
static int	s_nCho		= 0;	// so lan da bao "dang cho" sau khi toi dich chang
static int	s_nToStation= -1;	// tram dich cua chang Xa phu (de goi ten khi toi noi)
static int	s_bDaNhac	= 0;	// da nhac nguoi choi bam Xa phu chua

// -------------------------------------------------------------------------
//  Nap bang + dung do thi (mot lan, luoi -- goi truoc lan dung dau tien).
// -------------------------------------------------------------------------
static void ArAddEdge(int nFrom, int nTo, int nX, int nY, int bStation,
					  int nFromStation, int nToStation)
{
	if (s_nEdge >= AR_MAX_EDGE)
		return;
	s_Edge[s_nEdge].nFrom		= (short)nFrom;
	s_Edge[s_nEdge].nTo			= (short)nTo;
	s_Edge[s_nEdge].nX			= (short)nX;
	s_Edge[s_nEdge].nY			= (short)nY;
	s_Edge[s_nEdge].bStation	= (short)bStation;
	s_Edge[s_nEdge].nFromStation= (short)nFromStation;
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
				ArAddEdge(nPrevMap, nMap, nPrevLx, nPrevLy, 0, -1, -1);	// di: diem ROI cua node truoc
				ArAddEdge(nMap, nPrevMap, nAx, nAy, 0, -1, -1);			// ve: diem DEN cua node nay
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
					  s_Station[i].nX[0], s_Station[i].nY[0], 1, i, j);
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
extern void AutoRouteGetPlayerMps(int* pnX, int* pnY);

static int ArKhoangCach(int ax, int ay, int bx, int by)
{
	int dx = ax - bx, dy = ay - by;
	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	return dx + dy;
}

// =========================================================================
//  DI TOI MOT DIEM XA -- co che dung CHUNG cho ca click minimap lan chang
//  cua tuyen lien ban do.
//
//  Vi sao phai co: client chi nap vai vung quanh nguoi choi. Diem ngoai do thi
//  FindRegion tra -1 va TestBarrier ra 0xff, A* doc thanh "ngoai ban do" nen bo
//  cuoc (wp=0) roi GotoWhere di thang, dam tuong dung im. Do duoc in-game 04/09:
//  dung o 1488,3101 ma diem cach 50 o da bao 0xff.
//
//  Cach lam: giu DICH CUOI, moi lan ra lenh chi nham toi diem XA NHAT ma vua
//  biet vua di duoc (lui dan tu dich ve phia nguoi choi). Toi nac do thi vung
//  moi da nap -> ra lenh tiep. Cu the cho toi khi cham dich that.
// =========================================================================
#define AW_ARRIVE_MPS	96		// coi nhu da toi (3 o)
#define AW_STALL_FRAME	150		// khong nhich duoc bay lau thi ra lenh lai
#define AW_MAX_STEP		200		// so nac toi da, tranh lap vo han

static int	s_bWalkOn	= 0;
static int	s_nWalkGoalX= 0, s_nWalkGoalY = 0;	// dich CUOI (Mps)
static int	s_nWalkSubX	= 0, s_nWalkSubY = 0;	// nac dang nham toi (Mps)
static int	s_nWalkStep	= 0;
static int	s_nWalkStall= 0;

// Lui dan tu dich ve phia nguoi choi, lay o dau tien vua biet vua di duoc.
static void ArClampGoal(int nPx, int nPy, int nGx, int nGy, int* pnX, int* pnY)
{
	*pnX = nGx; *pnY = nGy;
	if (SubWorld[0].TestBarrier(nGx, nGy) == 0)
		return;

	int dx = nGx - nPx, dy = nGy - nPy;
	int nSteps = (((dx < 0) ? -dx : dx) + ((dy < 0) ? -dy : dy)) / AR_MPS_PER_UNIT;
	if (nSteps > 2048)
		nSteps = 2048;

	for (int i = nSteps; i >= 1; i--)
	{
		int x = nPx + dx * i / nSteps;
		int y = nPy + dy * i / nSteps;
		if (SubWorld[0].TestBarrier(x, y) == 0)
		{
			*pnX = x; *pnY = y;
			return;
		}
	}
}

// GotoWhere goi truoc khi chay A*: ghi nhan dich cuoi, tra ve nac nen di toi.
// Moi lenh di (nguoi choi click, minimap, hay chang cua tuyen) deu qua day.
void AutoWalkSetGoal(int nGoalX, int nGoalY, int* pnOutX, int* pnOutY)
{
	int nPx = 0, nPy = 0;
	AutoRouteGetPlayerMps(&nPx, &nPy);

	// Lenh MOI (dich khac) thi dem lai tu dau; con chinh ArWalkTiep goi lai voi
	// DUNG dich cu thi giu nguyen so nac -- neu dat lai 0 thi tran AW_MAX_STEP
	// khong bao gio cham va se lap vo han khi khong toi duoc.
	if (!s_bWalkOn || nGoalX != s_nWalkGoalX || nGoalY != s_nWalkGoalY)
		s_nWalkStep = 0;
	s_bWalkOn    = 1;
	s_nWalkGoalX = nGoalX; s_nWalkGoalY = nGoalY;
	s_nWalkStall = 0;

	ArClampGoal(nPx, nPy, nGoalX, nGoalY, &s_nWalkSubX, &s_nWalkSubY);
	*pnOutX = s_nWalkSubX; *pnOutY = s_nWalkSubY;

	if (s_nWalkSubX != nGoalX || s_nWalkSubY != nGoalY)
		g_DebugLog("[AUTOWALK] dich %d,%d ngoai tam biet -> nham nac %d,%d (con %d)",
				   nGoalX, nGoalY, s_nWalkSubX, s_nWalkSubY,
				   ArKhoangCach(nPx, nPy, nGoalX, nGoalY));
}

void AutoWalkCancel()
{
	s_bWalkOn = 0;
}

// Ra lenh di tiep toi nac ke. Khong goi AutoWalkSetGoal truc tiep: di qua
// AutoRouteGotoSpace -> GotoWhere -> AutoWalkSetGoal, de A* chay nhu moi lan.
static void ArWalkTiep()
{
	int nGx = s_nWalkGoalX, nGy = s_nWalkGoalY;
	s_nWalkStep++;
	g_DebugLog("[AUTOWALK] nac %d -> dich %d,%d", s_nWalkStep, nGx, nGy);
	AutoRouteGotoSpace(nGx, nGy);
}

static void AutoWalkTick()
{
	if (!s_bWalkOn)
		return;

	int nPx = 0, nPy = 0;
	AutoRouteGetPlayerMps(&nPx, &nPy);

	if (ArKhoangCach(nPx, nPy, s_nWalkGoalX, s_nWalkGoalY) <= AW_ARRIVE_MPS)
	{
		s_bWalkOn = 0;					// toi dich that
		g_DebugLog("[AUTOWALK] toi dich %d,%d sau %d nac", s_nWalkGoalX, s_nWalkGoalY, s_nWalkStep);
		return;
	}

	// Toi nac trung gian -> di tiep ngay. Khong nhich duoc lau -> cung thu lai
	// (vung co the vua nap them, hoac A* vua thoat khoi cho ket).
	if (ArKhoangCach(nPx, nPy, s_nWalkSubX, s_nWalkSubY) <= AW_ARRIVE_MPS
		|| ++s_nWalkStall > AW_STALL_FRAME)
	{
		s_nWalkStall = 0;
		if (s_nWalkStep >= AW_MAX_STEP)
		{
			s_bWalkOn = 0;
			g_DebugLog("[AUTOWALK] qua %d nac ma khong toi dich -> bo", AW_MAX_STEP);
			return;
		}
		ArWalkTiep();

		// Nac moi van nam ngay cho dang dung ma chua phai dich: khong con duong
		// nao gan hon de nham toi -> bo ngay, dung ra lenh 200 lan trong 7 giay.
		if (ArKhoangCach(nPx, nPy, s_nWalkSubX, s_nWalkSubY) <= AW_ARRIVE_MPS
			&& (s_nWalkSubX != s_nWalkGoalX || s_nWalkSubY != s_nWalkGoalY))
		{
			s_bWalkOn = 0;
			g_DebugLog("[AUTOWALK] khong nhich them duoc ve phia dich %d,%d (con %d) -> bo",
					   s_nWalkGoalX, s_nWalkGoalY,
					   ArKhoangCach(nPx, nPy, s_nWalkGoalX, s_nWalkGoalY));
		}
	}
}

// Bat dau chang hien tai.
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
	s_nStall = 0; s_nCho = 0; s_bDaNhac = 0;
	s_bChoXaPhu = pE->bStation;
	s_nToStation = pE->nToStation;

	int nX = pE->nX, nY = pE->nY;
	if (pE->bStation && pE->nFromStation >= 0 && pE->nFromStation < s_nStation)
	{
		// Moi thanh co 1-4 Xa phu -> chon cai GAN NHAT cho do phai di xa.
		int nPx = 0, nPy = 0;
		AutoRouteGetPlayerMps(&nPx, &nPy);
		nPx /= AR_MPS_PER_UNIT; nPy /= AR_MPS_PER_UNIT;

		const KRouteStation* pS = &s_Station[pE->nFromStation];
		int nBest = -1, nBestD = 0;
		for (int k = 0; k < pS->nCnt; k++)
		{
			int dx = pS->nX[k] - nPx, dy = pS->nY[k] - nPy;
			int d  = dx * dx + dy * dy;
			if (nBest < 0 || d < nBestD) { nBest = k; nBestD = d; }
		}
		if (nBest >= 0)
		{
			nX = pS->nX[nBest]; nY = pS->nY[nBest];
			g_DebugLog("[AUTOROUTE] Xa phu gan nhat: cai %d/%d o %d,%d (nguoi choi %d,%d)",
					   nBest + 1, pS->nCnt, nX, nY, nPx, nPy);
		}
	}

	s_nGoalX = nX * AR_MPS_PER_UNIT;
	s_nGoalY = nY * AR_MPS_PER_UNIT;
	g_DebugLog("[AUTOROUTE] chang %d/%d: map %d -> %d, diem bang %d,%d%s",
			   s_nRouteIdx + 1, s_nRouteCnt - 1, nFrom, nTo, nX, nY,
			   pE->bStation ? " (Xa phu)" : "");
	AutoRouteGotoSpace(s_nGoalX, s_nGoalY);	// AutoWalk lo phan di tung nac
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

	s_nRouteCnt = n; s_nRouteIdx = 0; s_nWait = 0;
	ArStartLeg();
	return n;
}

void AutoRouteCancel(const char* szWhy)
{
	if (s_nRouteCnt <= 0)
		return;
	g_DebugLog("[AUTOROUTE] huy tuyen: %s (dang o map %d, chang %d/%d)",
			   szWhy ? szWhy : "?", SubWorld[0].m_SubWorldID, s_nRouteIdx + 1, s_nRouteCnt - 1);
	s_nRouteCnt = 0; s_nRouteIdx = 0; s_nWait = 0; s_nStall = 0;
	s_bChoXaPhu = 0; s_nCho = 0; s_bDaNhac = 0; s_nToStation = -1;
}

void AutoRouteTick()
{
	AutoWalkTick();			// di tung nac -- chay ca khi khong co tuyen nao

	if (s_nRouteCnt <= 0)
		return;

	if (s_nWait > 0)						// vua sang map: cho region nap xong
	{
		if (--s_nWait == 0)
			ArStartLeg();
		return;
	}

	int nCur = SubWorld[0].m_SubWorldID;

	if (nCur != s_RouteMap[s_nRouteIdx])	// da doi map
	{
		AutoWalkCancel();					// dich cu thuoc map cu, bo di
		if (s_nRouteIdx + 1 < s_nRouteCnt && nCur == s_RouteMap[s_nRouteIdx + 1])
		{
			s_nRouteIdx++;
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
		return;
	}

	int nPx = 0, nPy = 0;
	AutoRouteGetPlayerMps(&nPx, &nPy);
	if (ArKhoangCach(nPx, nPy, s_nGoalX, s_nGoalY) > AR_ARRIVE_MPS)
		return;								// con dang di, AutoWalk lo

	// Da dung dung diem cua chang.
	if (s_bChoXaPhu && !s_bDaNhac)
	{
		s_bDaNhac = 1;
		char szMsg[128];
		const char* szTo = (s_nToStation >= 0 && s_nToStation < s_nStation)
						   ? s_Station[s_nToStation].szName : "";
		sprintf(szMsg, "Toi Xa phu roi. Hay chon %s de di tiep.", szTo);
		AutoRouteSay(szMsg);
	}

	// PHAI keu dinh ky: im hoan toan thi nhin tu ngoai khong phan biet duoc
	// "dang cho" voi "ket duong".
	if (++s_nStall % AR_CHO_FRAME)
		return;
	s_nCho++;
	g_DebugLog("[AUTOROUTE] da toi diem chang %d tren map %d, dang cho %s (lan %d)",
			   s_nRouteIdx + 1, nCur,
			   s_bChoXaPhu ? "nguoi choi bam Xa phu" : "server chuyen map", s_nCho);
	if (!s_bChoXaPhu && s_nCho >= AR_MAX_CHO)
		AutoRouteCancel("dung dung diem chuyen map ma khong sang duoc");
}

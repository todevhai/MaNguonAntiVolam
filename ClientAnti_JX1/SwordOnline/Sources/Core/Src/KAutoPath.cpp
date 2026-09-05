// -------------------------------------------------------------------------
//  KAutoPath.cpp  --  A* toan cuc cho player auto-di (click dich -> di vong tuong)
//
//  Toa do lam viec: Mps (map sub-position). Luoi A* buoc defLOGIC_CELL_WIDTH (=32 Mps).
//  Passable: g_ScenePlace.GetObstacleInfo(mps) == Obstacle_NULL. KHONG dung
//  TestBarrier() == 0 -- xem chu thich o CellPassable, no tra DO CAO dia hinh.
//  Output: waypoint (Mps) da rut gon chi giu diem DOI HUONG (line-of-sight smoothing).
//  Tra ve so waypoint; 0 = di thang duoc / khong co duong.
//
//  File nguon cua ta (khong phai Kingsoft goc). Comment ASCII. Mang cap phat theo
//  cua so tim gioi han + cap so node de tranh treo/ton RAM tren map lon.
// -------------------------------------------------------------------------
#include "KWin32.h"
#include "KCore.h"
#include "KSubWorld.h"
#include "Scene/KScenePlaceC.h"      // g_ScenePlace.GetObstacleInfo -> LOAI VAT CAN
#include "Scene/ObstacleDef.h"       // Obstacle_NULL / Normal / Fly / Jump / JumpFly
#include "KAutoPath.h"
#include "KDebug.h"    // g_DebugLog (build chan doan so sanh duong)
#include <stdlib.h>

#ifndef defLOGIC_CELL_WIDTH
#define defLOGIC_CELL_WIDTH   32
#endif

#define AP_CELL          defLOGIC_CELL_WIDTH   // Mps moi o luoi
#define AP_MAX_WIN       256                   // canh cua so tim toi da (o luoi)
#define AP_MAX_NODES     20000                 // cap so node mo -> tranh treo
#define AP_MARGIN        48                    // no cua so quanh bbox(start,goal) (o luoi)

// SubWorld[] la global khai bao extern trong KSubWorld.h -> khong khai lai.
static int  TestBarrierMps(int nMpsX, int nMpsY)
{
    return SubWorld[0].TestBarrier(nMpsX, nMpsY);
}

// -------------------------------------------------------------------------
//  O NAY CO DI DUOC KHONG.
//
//  KHONG dung TestBarrier() == 0 nhu truoc. Doc lai chu thich cua chinh engine:
//      KRegion::GetBarrier(...)      // 地图高度   = DO CAO dia hinh
//      KRegion::GetBarrierMin(...)   // 障碍类型 (== Obstacle_NULL la khong can)
//  ma KSubWorld::TestBarrier phia client lam:
//      bRet = g_ScenePlace.GetObstacleInfo(mps);
//      if (bRet != Obstacle_NULL) return bRet;
//      return m_Region[nRegion].GetBarrier(...);   <-- tra DO CAO
//  Tuc khong co vat can canh thi no tra DO CAO DIA HINH. Phep so "== 0" cu vi
//  the coi MOI cho dat co do cao khac 0 la tuong -> A* thay gan nhu ca ban do la
//  tuong -> tra 0 gan nhu moi lan -> GotoWhere di thang -> gap can la dung.
//  Do in-game 04/09: trong 17 mau, o nhan vat DANG DUNG chua bao gio la 0.
//
//  Nay hoi thang loai vat can. Van giu TestBarrier de biet vung CHUA NAP (0xff).
// -------------------------------------------------------------------------
static int  ObstacleKindMps(int nMpsX, int nMpsY)
{
    return (int)g_ScenePlace.GetObstacleInfo(nMpsX, nMpsY);
}

// Tam o luoi (cell) <-> Mps (lay tam o de test barrier on dinh).
static int CellToMps(int c) { return c * AP_CELL + AP_CELL / 2; }
static int MpsToCell(int m) { return m / AP_CELL; }

static int CellPassable(int cx, int cy)
{
    int nMx = CellToMps(cx), nMy = CellToMps(cy);
    if (TestBarrierMps(nMx, nMy) == 0xff)
        return 0;                                   // vung chua nap -> coi nhu khong biet
    return ObstacleKindMps(nMx, nMy) == Obstacle_NULL;
}

struct APNode
{
    int  idx;   // chi so trong mang cua so
    int  f;     // fScore
};

// Line-of-sight giua hai o luoi (Bresenham tren o): co the di thang khong vuong?
static int LineClear(int c0x, int c0y, int c1x, int c1y)
{
    int dx = c1x - c0x, dy = c1y - c0y;
    int sx = dx > 0 ? 1 : (dx < 0 ? -1 : 0);
    int sy = dy > 0 ? 1 : (dy < 0 ? -1 : 0);
    int adx = dx < 0 ? -dx : dx;
    int ady = dy < 0 ? -dy : dy;
    int x = c0x, y = c0y;
    if (adx >= ady)
    {
        int err = adx / 2;
        while (x != c1x)
        {
            if (!CellPassable(x, y)) return 0;
            err -= ady;
            if (err < 0) { y += sy; err += adx; }
            x += sx;
        }
    }
    else
    {
        int err = ady / 2;
        while (y != c1y)
        {
            if (!CellPassable(x, y)) return 0;
            err -= adx;
            if (err < 0) { x += sx; err += ady; }
            y += sy;
        }
    }
    return CellPassable(c1x, c1y);
}

// Heuristic octile (cheo 14, thang 10).
static int Heuristic(int ax, int ay, int bx, int by)
{
    int dx = ax > bx ? ax - bx : bx - ax;
    int dy = ay > by ? ay - by : by - ay;
    int mn = dx < dy ? dx : dy;
    int mx = dx < dy ? dy : dx;
    return 10 * (mx - mn) + 14 * mn;
}

int AutoPathFind(int nStartMpsX, int nStartMpsY, int nGoalMpsX, int nGoalMpsY,
                 int* pOutX, int* pOutY, int nMaxOut)
{
    if (!pOutX || !pOutY || nMaxOut <= 0) return 0;

    int sCx = MpsToCell(nStartMpsX), sCy = MpsToCell(nStartMpsY);
    int gCx = MpsToCell(nGoalMpsX),  gCy = MpsToCell(nGoalMpsY);

    // Di thang thay duoc dich (khong vuong can): tra MOT waypoint = dich, KHONG tra 0.
    // Ly do: caller dat m_nAutoPathCnt theo so waypoint; neu tra 0 thi m_nAutoPathCnt=0
    // -> phat hien ket theo frame TAT -> neu duong "thang" that ra vuong can (LOS luoi
    // 32 coi o-tam thong nhung va cham chan, vd cua thanh) thi nhan vat giat vao tuong
    // ma khong tinh lai duoc. Tra 1 waypoint -> phat hien ket van chay -> ket thi
    // recompute (full A*) tim duong vong. Van re: khong chay A* cho duong thay ro.
    if (CellPassable(gCx, gCy) && LineClear(sCx, sCy, gCx, gCy))
    {
        pOutX[0] = nGoalMpsX;
        pOutY[0] = nGoalMpsY;
        g_DebugLog("[AP-WP] thang: goal=%d,%d", nGoalMpsX, nGoalMpsY);
        return 1;
    }

    // Dich khong di duoc (bam trung nha/tuong): keo ve o di duoc gan nhat.
    // Ban kinh 2 o CHUA DU: bam tren minimap thi dich hay roi sau vao giua mot
    // toa nha, cach mep di duoc chuc o -> khong tim thay -> tra 0 -> nguoi choi
    // thay "bam khong an gi". Do in-game 04/09. Noi ra 12 o.
    int bGoalMoved = 0;
    if (!CellPassable(gCx, gCy))
    {
        int found = 0, r, ox, oy;
        for (r = 1; r <= 12 && !found; r++)
            for (oy = -r; oy <= r && !found; oy++)
                for (ox = -r; ox <= r && !found; ox++)
                    if (CellPassable(gCx + ox, gCy + oy))
                    { gCx += ox; gCy += oy; found = 1; }
        if (!found) return 0;
        bGoalMoved = 1;
    }
    if (sCx == gCx && sCy == gCy) return 0;   // da o dich

    // Cua so tim = bbox(start,goal) no AP_MARGIN, gioi han AP_MAX_WIN.
    int minx = (sCx < gCx ? sCx : gCx) - AP_MARGIN;
    int miny = (sCy < gCy ? sCy : gCy) - AP_MARGIN;
    int maxx = (sCx > gCx ? sCx : gCx) + AP_MARGIN;
    int maxy = (sCy > gCy ? sCy : gCy) + AP_MARGIN;
    int W = maxx - minx + 1;
    int H = maxy - miny + 1;
    if (W > AP_MAX_WIN || H > AP_MAX_WIN) return 0;   // qua xa -> fallback vector

    int N = W * H;
    int* g      = (int*)malloc(sizeof(int) * N);
    int* came   = (int*)malloc(sizeof(int) * N);
    unsigned char* closed = (unsigned char*)malloc(N);
    APNode* heap = (APNode*)malloc(sizeof(APNode) * (AP_MAX_NODES + 8));
    if (!g || !came || !closed || !heap)
    { free(g); free(came); free(closed); free(heap); return 0; }
    int i;
    for (i = 0; i < N; i++) { g[i] = -1; came[i] = -1; closed[i] = 0; }

    #define AP_IDX(cx,cy)  (((cy) - miny) * W + ((cx) - minx))
    int startIdx = AP_IDX(sCx, sCy);
    int goalIdx  = AP_IDX(gCx, gCy);
    g[startIdx] = 0;

    int heapN = 0;
    heap[heapN].idx = startIdx;
    heap[heapN].f   = Heuristic(sCx, sCy, gCx, gCy);
    heapN++;

    static const int DX[8] = { 1,-1, 0, 0, 1, 1,-1,-1 };
    static const int DY[8] = { 0, 0, 1,-1, 1,-1, 1,-1 };
    static const int DC[8] = { 10,10,10,10,14,14,14,14 };

    int expanded = 0, foundGoal = 0;
    while (heapN > 0 && expanded < AP_MAX_NODES)
    {
        int cur = heap[0].idx;
        heapN--;
        heap[0] = heap[heapN];
        int p = 0;
        for (;;)
        {
            int l = 2 * p + 1, rr = 2 * p + 2, sm = p;
            if (l < heapN && heap[l].f < heap[sm].f) sm = l;
            if (rr < heapN && heap[rr].f < heap[sm].f) sm = rr;
            if (sm == p) break;
            APNode t = heap[p]; heap[p] = heap[sm]; heap[sm] = t; p = sm;
        }
        if (closed[cur]) continue;
        closed[cur] = 1;
        expanded++;
        if (cur == goalIdx) { foundGoal = 1; break; }

        int cx = minx + (cur % W);
        int cy = miny + (cur / W);
        int k;
        for (k = 0; k < 8; k++)
        {
            int nx = cx + DX[k], ny = cy + DY[k];
            if (nx < minx || nx > maxx || ny < miny || ny > maxy) continue;
            if (!CellPassable(nx, ny)) continue;
            // cheo: khong cat goc qua hai tuong ke
            if (DC[k] == 14 && (!CellPassable(cx + DX[k], cy) || !CellPassable(cx, cy + DY[k])))
                continue;
            int nIdx = AP_IDX(nx, ny);
            if (closed[nIdx]) continue;
            int ng = g[cur] + DC[k];
            if (g[nIdx] < 0 || ng < g[nIdx])
            {
                g[nIdx] = ng;
                came[nIdx] = cur;
                if (heapN < AP_MAX_NODES)
                {
                    int f = ng + Heuristic(nx, ny, gCx, gCy);
                    heap[heapN].idx = nIdx; heap[heapN].f = f;
                    int c = heapN++;
                    while (c > 0)
                    {
                        int par = (c - 1) / 2;
                        if (heap[par].f <= heap[c].f) break;
                        APNode t = heap[par]; heap[par] = heap[c]; heap[c] = t; c = par;
                    }
                }
            }
        }
    }

    int nOut = 0;
    int* pathC = (int*)malloc(sizeof(int) * N);   // du chua duong dai nhat trong cua so
    if (foundGoal && pathC)
    {
        int pc = 0, node = goalIdx;
        while (node >= 0 && pc < N)
        {
            pathC[pc++] = node;
            if (node == startIdx) break;
            node = came[node];
        }
        // pathC[0]=goal ... pathC[pc-1]=start; rut gon: tu start nhay xa nhat con LineClear.
        int curNode = pc - 1;
        while (curNode > 0 && nOut < nMaxOut)
        {
            int c0 = pathC[curNode];
            int c0x = minx + (c0 % W), c0y = miny + (c0 / W);
            int best = curNode - 1;
            int j;
            for (j = 0; j < curNode; j++)   // j = 0 la goal (xa nhat)
            {
                int cj = pathC[j];
                int cjx = minx + (cj % W), cjy = miny + (cj / W);
                if (LineClear(c0x, c0y, cjx, cjy)) { best = j; break; }
            }
            int bc = pathC[best];
            pOutX[nOut] = CellToMps(minx + (bc % W));
            pOutY[nOut] = CellToMps(miny + (bc / W));
            nOut++;
            curNode = best;
        }
        // waypoint cuoi = dich that (tru khi dich goc nam tren can -> giu o keo ve)
        if (nOut > 0 && !bGoalMoved) { pOutX[nOut - 1] = nGoalMpsX; pOutY[nOut - 1] = nGoalMpsY; }
    }

    {
        int _i;
        g_DebugLog("[AP-WP] N=%d start=%d,%d goal=%d,%d", nOut, nStartMpsX, nStartMpsY, nGoalMpsX, nGoalMpsY);
        for (_i = 0; _i < nOut; _i++)
            g_DebugLog("[AP-WP]   %d/%d %d,%d", _i, nOut, pOutX[_i], pOutY[_i]);
    }
    free(pathC);
    free(g); free(came); free(closed); free(heap);
    #undef AP_IDX
    return nOut;
}

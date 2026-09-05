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
#define AP_MAX_WIN       416                   // canh cua so tim toi da (o luoi) - lon de thay cong lech ngoai tam
#define AP_MAX_NODES     60000                 // cap so node mo -> tranh treo (cua so lon hon)
#define AP_MARGIN        64                    // no cua so quanh bbox(start,goal) (o luoi)

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

    // Di thang thay dich (khong vuong): tra 1 waypoint = dich.
    if (CellPassable(gCx, gCy) && LineClear(sCx, sCy, gCx, gCy))
    {
        pOutX[0] = nGoalMpsX;
        pOutY[0] = nGoalMpsY;
        g_DebugLog("[AP-WP] thang: goal=%d,%d", nGoalMpsX, nGoalMpsY);
        return 1;
    }

    // tCx,tCy = O DICH DE A* NHAM (co the bi keo/kep). Giu (gCx,gCy) lam dich THAT
    // de chon bien gan dich nhat khi khong toi thang.
    int tCx = gCx, tCy = gCy;
    int bFar = 0, bGoalMoved = 0;

    // Dich XA (bbox vuot cua so): kep dich vao cua so quanh NGUOI CHOI. A* se tim
    // duong toi ria cua so ve huong dich -> di qua CONG neu cong nam trong vung nap.
    {
        int bbx = sCx < gCx ? gCx - sCx : sCx - gCx;
        int bby = sCy < gCy ? gCy - sCy : sCy - gCy;
        if (bbx + 2 * AP_MARGIN > AP_MAX_WIN || bby + 2 * AP_MARGIN > AP_MAX_WIN)
        {
            bFar = 1;
            int half = AP_MAX_WIN / 2 - AP_MARGIN - 2;
            if (tCx > sCx + half) tCx = sCx + half; else if (tCx < sCx - half) tCx = sCx - half;
            if (tCy > sCy + half) tCy = sCy + half; else if (tCy < sCy - half) tCy = sCy - half;
        }
    }

    // Keo dich A* ve o di duoc gan nhat. Xa ma khong keo duoc thi VAN chay A*
    // (dung bien gan dich nhat); gan ma khong keo duoc -> chiu.
    if (!CellPassable(tCx, tCy))
    {
        int found = 0, r, ox, oy;
        for (r = 1; r <= 12 && !found; r++)
            for (oy = -r; oy <= r && !found; oy++)
                for (ox = -r; ox <= r && !found; ox++)
                    if (CellPassable(tCx + ox, tCy + oy)) { tCx += ox; tCy += oy; found = 1; }
        if (found) bGoalMoved = 1;
        /* Khong keo duoc (dich la tuong/ngoai thanh/chua nap): VAN chay A*, dung
           BIEN gan dich nhat (frontier). Truoc day return 0 o day khi !bFar -> nhan
           vat dung im khi bam diem ngoai tuong (gan nguong xa). */
    }
    if (sCx == tCx && sCy == tCy) return 0;

    // Cua so: xa -> quanh nguoi choi; gan -> bbox(start,target).
    int minx, miny, maxx, maxy;
    if (bFar)
    {
        minx = sCx - AP_MAX_WIN / 2; miny = sCy - AP_MAX_WIN / 2;
        maxx = sCx + AP_MAX_WIN / 2; maxy = sCy + AP_MAX_WIN / 2;
    }
    else
    {
        minx = (sCx < tCx ? sCx : tCx) - AP_MARGIN;
        miny = (sCy < tCy ? sCy : tCy) - AP_MARGIN;
        maxx = (sCx > tCx ? sCx : tCx) + AP_MARGIN;
        maxy = (sCy > tCy ? sCy : tCy) + AP_MARGIN;
    }
    if (tCx < minx) tCx = minx; else if (tCx > maxx) tCx = maxx;
    if (tCy < miny) tCy = miny; else if (tCy > maxy) tCy = maxy;
    int W = maxx - minx + 1;
    int H = maxy - miny + 1;
    if (W <= 0 || H <= 0 || W > AP_MAX_WIN + 2 || H > AP_MAX_WIN + 2) return 0;

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
    int goalIdx  = AP_IDX(tCx, tCy);
    g[startIdx] = 0;

    // Bien tot nhat = o DA XET gan DICH THAT nhat (dung khi khong toi duoc target).
    int bestNode = startIdx;
    int bestH    = Heuristic(sCx, sCy, gCx, gCy);

    int heapN = 0;
    heap[heapN].idx = startIdx;
    heap[heapN].f   = Heuristic(sCx, sCy, tCx, tCy);
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
        {
            int ccx = minx + (cur % W);
            int ccy = miny + (cur / W);
            int h = Heuristic(ccx, ccy, gCx, gCy);   // do toi DICH THAT
            if (h < bestH) { bestH = h; bestNode = cur; }
        }
        if (cur == goalIdx) { foundGoal = 1; break; }

        int cx = minx + (cur % W);
        int cy = miny + (cur / W);
        int k;
        for (k = 0; k < 8; k++)
        {
            int nx = cx + DX[k], ny = cy + DY[k];
            if (nx < minx || nx > maxx || ny < miny || ny > maxy) continue;
            if (!CellPassable(nx, ny)) continue;
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
                    int f = ng + Heuristic(nx, ny, tCx, tCy);
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

    // Dich cua duong: toi target thi lay target; khong thi lay BIEN gan dich that nhat.
    int endNode = foundGoal ? goalIdx : bestNode;

    int nOut = 0;
    int* pathC = (int*)malloc(sizeof(int) * N);
    if (endNode > 0 && endNode != startIdx && came[endNode] >= 0 && pathC)
    {
        int pc = 0, node = endNode;
        while (node >= 0 && pc < N)
        {
            pathC[pc++] = node;
            if (node == startIdx) break;
            node = came[node];
        }
        int curNode = pc - 1;
        while (curNode > 0 && nOut < nMaxOut)
        {
            int c0 = pathC[curNode];
            int c0x = minx + (c0 % W), c0y = miny + (c0 / W);
            int best = curNode - 1;
            int j;
            for (j = 0; j < curNode; j++)
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
        // Chi dat waypoint cuoi = DICH THAT khi that su toi noi (khong xa/keo).
        if (nOut > 0 && foundGoal && !bFar && !bGoalMoved)
        { pOutX[nOut - 1] = nGoalMpsX; pOutY[nOut - 1] = nGoalMpsY; }
    }

    g_DebugLog("[AP-WP] N=%d %s start=%d,%d goal=%d,%d", nOut, foundGoal ? "toi-dich" : "bien", nStartMpsX, nStartMpsY, nGoalMpsX, nGoalMpsY);
    free(pathC);
    free(g); free(came); free(closed); free(heap);
    #undef AP_IDX
    return nOut;
}

// AutoPathFindStep: nay AutoPathFind da tu lo dich xa (cua so quanh nguoi choi +
// tra duong toi bien gan dich nhat -> qua cong). Chi con dat co "da toi dich that".
int AutoPathFindStep(int sx, int sy, int gx, int gy,
                     int* pOutX, int* pOutY, int nMaxOut, int* pbFinal)
{
    if (pbFinal) *pbFinal = 1;
    int n = AutoPathFind(sx, sy, gx, gy, pOutX, pOutY, nMaxOut);
    if (n > 0)
    {
        int dx = pOutX[n - 1] - gx; if (dx < 0) dx = -dx;
        int dy = pOutY[n - 1] - gy; if (dy < 0) dy = -dy;
        if (pbFinal) *pbFinal = (dx <= AP_CELL && dy <= AP_CELL) ? 1 : 0;
        return n;
    }

    // A* ket o BIEN vung nap (bestNode == start). Bat chuoc ban hoan chinh jx9tn:
    // no khong dung o bien ma NHAY THANG ve phia dich vao ca vung chua nap - server
    // (co du lieu day du) se di + client nap vung doc duong, roi tinh lai. Chi lam
    // khi huong dich la vung CHUA NAP (0xff), khong phai tuong THAT (da nap ma chan).
    int scx = MpsToCell(sx), scy = MpsToCell(sy);
    int gcx = MpsToCell(gx), gcy = MpsToCell(gy);
    int dcx = gcx - scx, dcy = gcy - scy;
    int adx = dcx < 0 ? -dcx : dcx;
    int ady = dcy < 0 ? -dcy : dcy;
    int dist = adx > ady ? adx : ady;
    if (dist <= 0) return 0;

    // Tham do o cach ~4 o ve phia dich: chua nap thi moi nhay (tin server).
    int probe = dist < 4 ? dist : 4;
    int pcx = scx + (int)((double)dcx * probe / dist);
    int pcy = scy + (int)((double)dcy * probe / dist);
    if (TestBarrierMps(CellToMps(pcx), CellToMps(pcy)) != 0xff)
        return 0;                        // tuong that / da nap ma A* ket -> chiu, dung

    int hop = dist < 48 ? dist : 48;     // buoc nhay ~48 o (jx9tn nhay toi 57)
    int hx = scx + (int)((double)dcx * hop / dist);
    int hy = scy + (int)((double)dcy * hop / dist);
    pOutX[0] = CellToMps(hx);
    pOutY[0] = CellToMps(hy);
    if (pbFinal) *pbFinal = (hop >= dist) ? 1 : 0;
    g_DebugLog("[AP-WP] nhay-nap %d,%d (dich %d,%d dist %d o)", pOutX[0], pOutY[0], gx, gy, dist);
    return 1;
}

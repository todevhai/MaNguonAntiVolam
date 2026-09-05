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
#include "KEngine.h"                 // KPakFile (dinh nghia day du) de doc file region
#include "KSubWorld.h"
#include "Scene/KScenePlaceC.h"      // g_ScenePlace.GetObstacleInfo -> LOAI VAT CAN
#include "Scene/ObstacleDef.h"       // Obstacle_NULL / Normal / Fly / Jump / JumpFly
#include "Scene/KScenePlaceRegionC.h" // RWPP_AREGION_*, RWP_OBSTACLE_*, m_ObstacleInfo layout
#include "Scene/SceneDataDef.h"      // KCombinFileSection, REGION_OBSTACLE_FILE_INDEX
#include "KAutoPath.h"
#include "KDebug.h"    // g_DebugLog (build chan doan so sanh duong)
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

// =========================================================================
//  NAP TRUOC TOAN BO LOP VAT CAN CUA SUBWORLD (client von streaming tung vung).
//
//  Ly do: client chi giu vung quanh nhan vat, vung xa tra 0xff (chua nap) ->
//  A* coi la tuong -> ket o bien nap / cuc tieu cuc bo -> "khong di het".
//  Lop vat can rat nhe (moi region 16x32 o = 2KB), va o vat can = 32x32 Mps =
//  DUNG mot o A*. Nap truoc ca subworld -> A* thay het tuong + cong -> di tron.
//  Chi doc muc OBSTACLE trong Region_C.dat (bo do hoa) nen re. Cross-map van
//  dung bang tuyen autopathfindroutes.txt.
// =========================================================================
#define AP_OBS_RX   16     // ban kinh quet region theo x (region rong 512 Mps)
#define AP_OBS_RY   10     // ban kinh quet region theo y (region cao 1024 Mps)
#define AP_RG_W     (KScenePlaceRegionC::RWPP_AREGION_WIDTH)   // 512
#define AP_RG_H     (KScenePlaceRegionC::RWPP_AREGION_HEIGHT)  // 1024
#define AP_RG_CW    16     // o vat can moi region theo x (512/32)
#define AP_RG_CH    32     // o vat can moi region theo y (1024/32)

static long* g_apObs   = 0;   // luoi vat can da nap: [g_apObsCH][g_apObsCW], raw lInfo; -1 = trong
static int   g_apObsCX0 = 0, g_apObsCY0 = 0;   // goc luoi theo o vat can (toan cuc)
static int   g_apObsCW  = 0, g_apObsCH  = 0;
static int   g_apObsCRX = -9999, g_apObsCRY = -9999;  // tam (region) cua lan nap
static char  g_apObsMap[96] = { 0 };           // root path map da nap (doi map thi nap lai)

// Doc rieng muc OBSTACLE (2KB) cua mot region tu Region_C.dat. true neu co.
static bool AP_ReadRegionObstacle(const char* root, int rx, int ry, long out[AP_RG_CW][AP_RG_CH])
{
    char file[260];
    sprintf(file, "%s\\v_%03d\\%03d_" REGION_COMBIN_FILE_NAME_CLIENT, root, ry, rx);
    KPakFile f;
    if (!f.Open(file)) return false;
    unsigned int uMax = 0;
    f.Read(&uMax, sizeof(unsigned int));
    if (uMax == 0 || uMax > 64) { f.Close(); return false; }
    KCombinFileSection elem[REGION_ELEM_FILE_COUNT];
    memset(elem, 0, sizeof(elem));
    if (uMax > REGION_ELEM_FILE_COUNT)
    {
        f.Read(elem, sizeof(KCombinFileSection) * REGION_ELEM_FILE_COUNT);
        f.Seek(sizeof(KCombinFileSection) * (uMax - REGION_ELEM_FILE_COUNT), FILE_CURRENT);
    }
    else
        f.Read(elem, sizeof(KCombinFileSection) * uMax);
    unsigned int ahead = sizeof(unsigned int) + sizeof(KCombinFileSection) * uMax;
    bool ok = false;
    if (elem[REGION_OBSTACLE_FILE_INDEX].uLength >= sizeof(long) * AP_RG_CW * AP_RG_CH)
    {
        f.Seek(ahead + elem[REGION_OBSTACLE_FILE_INDEX].uOffset, FILE_BEGIN);
        f.Read(out, sizeof(long) * AP_RG_CW * AP_RG_CH);
        ok = true;
    }
    f.Close();
    return ok;
}

// Nap truoc lop vat can quanh (centerMpsX,Y) trong ban kinh AP_OBS_R*.
static void AP_BuildFullObs(const char* root, int cMpsX, int cMpsY)
{
    if (g_apObs) { free(g_apObs); g_apObs = 0; }
    int crx = cMpsX / AP_RG_W, cry = cMpsY / AP_RG_H;
    int rx0 = crx - AP_OBS_RX; if (rx0 < 0) rx0 = 0;
    int ry0 = cry - AP_OBS_RY; if (ry0 < 0) ry0 = 0;
    int rx1 = crx + AP_OBS_RX;
    int ry1 = cry + AP_OBS_RY;
    g_apObsCX0 = rx0 * AP_RG_CW;
    g_apObsCY0 = ry0 * AP_RG_CH;
    g_apObsCW  = (rx1 - rx0 + 1) * AP_RG_CW;
    g_apObsCH  = (ry1 - ry0 + 1) * AP_RG_CH;
    int N = g_apObsCW * g_apObsCH;
    g_apObs = (long*)malloc(sizeof(long) * N);
    if (!g_apObs) { g_apObsCW = g_apObsCH = 0; return; }
    int i;
    for (i = 0; i < N; i++) g_apObs[i] = -1;

    int nLoaded = 0, rx, ry, lx, ly;
    for (ry = ry0; ry <= ry1; ry++)
        for (rx = rx0; rx <= rx1; rx++)
        {
            long ob[AP_RG_CW][AP_RG_CH];
            if (!AP_ReadRegionObstacle(root, rx, ry, ob)) continue;
            nLoaded++;
            for (lx = 0; lx < AP_RG_CW; lx++)
                for (ly = 0; ly < AP_RG_CH; ly++)
                {
                    int gcx = rx * AP_RG_CW + lx - g_apObsCX0;
                    int gcy = ry * AP_RG_CH + ly - g_apObsCY0;
                    g_apObs[gcy * g_apObsCW + gcx] = ob[lx][ly];
                }
        }
    strncpy(g_apObsMap, root, sizeof(g_apObsMap) - 1);
    g_apObsMap[sizeof(g_apObsMap) - 1] = 0;
    g_apObsCRX = crx; g_apObsCRY = cry;
    g_DebugLog("[AP-OBS] nap %d region quanh r(%d,%d) luoi %dx%d o = %d KB",
               nLoaded, crx, cry, g_apObsCW, g_apObsCH, (int)(sizeof(long) * N / 1024));
}

// Nap lai neu doi map hoac nhan vat ra gan ria vung da nap.
static void AP_EnsureFullObs(int mpsX, int mpsY)
{
    const char* root = g_ScenePlace.GetPlaceRootPath();
    if (!root || !root[0]) return;
    int crx = mpsX / AP_RG_W, cry = mpsY / AP_RG_H;
    int dcx = crx - g_apObsCRX; if (dcx < 0) dcx = -dcx;
    int dcy = cry - g_apObsCRY; if (dcy < 0) dcy = -dcy;
    if (g_apObs && strcmp(g_apObsMap, root) == 0 && dcx <= AP_OBS_RX - 6 && dcy <= AP_OBS_RY - 4)
        return;                                  // con trong vung da nap -> thoi
    AP_BuildFullObs(root, mpsX, mpsY);
}

// Loai vat can tu luoi da nap truoc. -1 = ngoai vung nap / region trong.
static int AP_FullObsKind(int mpsX, int mpsY)
{
    if (!g_apObs || mpsX < 0 || mpsY < 0) return -1;
    int cx = mpsX / AP_CELL - g_apObsCX0;
    int cy = mpsY / AP_CELL - g_apObsCY0;
    if (cx < 0 || cy < 0 || cx >= g_apObsCW || cy >= g_apObsCH) return -1;
    long info = g_apObs[cy * g_apObsCW + cx];
    if (info < 0) return -1;
    int kind = (int)(info & 0x0f);
    int type = (int)((info >> 4) & 0x0f);
    int mx = mpsX - (mpsX / AP_CELL) * AP_CELL;
    int my = mpsY - (mpsY / AP_CELL) * AP_CELL;
    switch (type)
    {
    case Obstacle_LT: if (mx + my > AP_CELL) kind = Obstacle_NULL; break;
    case Obstacle_RT: if (mx < my)          kind = Obstacle_NULL; break;
    case Obstacle_LB: if (mx > my)          kind = Obstacle_NULL; break;
    case Obstacle_RB: if (mx + my < AP_CELL) kind = Obstacle_NULL; break;
    default: break;
    }
    return kind;
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

// -------------------------------------------------------------------------
//  BLACKLIST O KET (khe hep / va cham): ServeMove phat hien dung yen qua lau thi
//  danh dau o phia truoc; A* coi vung 3x3 quanh o do la tuong o lan sau -> vong
//  qua. Neu khe la duong DUY NHAT thi A* bo cuoc (dung), chap nhan - jx9tn cung
//  khong luon duoc khe hep. Xoa khi bat dau lenh di moi (AutoPathClearStuck).
// -------------------------------------------------------------------------
#define AP_STUCK_MAX 24
static int g_apStuckX[AP_STUCK_MAX], g_apStuckY[AP_STUCK_MAX];
static int g_apStuckN = 0;
static int g_apStuckHead = 0;   // ring buffer khi day

void AutoPathClearStuck() { g_apStuckN = 0; g_apStuckHead = 0; }

void AutoPathMarkStuckMps(int nMpsX, int nMpsY)
{
    int cx = nMpsX / AP_CELL, cy = nMpsY / AP_CELL;
    int i;
    for (i = 0; i < g_apStuckN; i++)
        if (g_apStuckX[i] == cx && g_apStuckY[i] == cy) return;   // da co
    if (g_apStuckN < AP_STUCK_MAX)
    {
        g_apStuckX[g_apStuckN] = cx; g_apStuckY[g_apStuckN] = cy; g_apStuckN++;
    }
    else
    {
        g_apStuckX[g_apStuckHead] = cx; g_apStuckY[g_apStuckHead] = cy;
        g_apStuckHead = (g_apStuckHead + 1) % AP_STUCK_MAX;   // ghi de cu nhat
    }
    g_DebugLog("[AP-STUCK] danh dau o ket %d,%d (tong %d)", cx, cy, g_apStuckN);
}

static int IsStuckCell(int cx, int cy)
{
    int i;
    for (i = 0; i < g_apStuckN; i++)
    {
        int dx = cx - g_apStuckX[i]; if (dx < 0) dx = -dx;
        int dy = cy - g_apStuckY[i]; if (dy < 0) dy = -dy;
        if (dx <= 1 && dy <= 1) return 1;    // vung 3x3 quanh o ket
    }
    return 0;
}

static int CellPassable(int cx, int cy)
{
    if (IsStuckCell(cx, cy)) return 0;              // o tung ket -> ne ra (vong khe hep)
    int nMx = CellToMps(cx), nMy = CellToMps(cy);
    // Uu tien lop vat can DA NAP TRUOC (thay ca subworld, khong chi vung streaming).
    int fk = AP_FullObsKind(nMx, nMy);
    if (fk >= 0)
        return fk == Obstacle_NULL;
    // Ngoai vung nap truoc -> quay ve du lieu streaming.
    if (TestBarrierMps(nMx, nMy) == 0xff)
        return 0;                                   // vung chua nap -> coi nhu khong biet
    return ObstacleKindMps(nMx, nMy) == Obstacle_NULL;
}

// O nay CO PHAI vat can cung (tuong/nha/hang rao...) khong -- bat ky loai != NULL,
// khong tinh vung chua nap. Dung de NOI vat can (phat chi phi o ke ben).
static int HardObstacleCell(int cx, int cy)
{
    int nMx = CellToMps(cx), nMy = CellToMps(cy);
    int fk = AP_FullObsKind(nMx, nMy);
    if (fk >= 0)
        return fk != Obstacle_NULL;
    if (TestBarrierMps(nMx, nMy) == 0xff)
        return 0;                                   // chua nap -> khong ket luan la tuong
    return ObstacleKindMps(nMx, nMy) != Obstacle_NULL;
}

// NOI VAT CAN (mem): o di-duoc ma KE tuong (4 huong chinh) bi PHAT chi phi -> A*
// thich di GIUA duong/khoang trong, tranh nem than vao goc nha. Khong CHAN nen loi
// hep/cong 1 o van qua duoc (chi dat hon) -> khong mat cong, khong hoi quy toi-duoc.
#define AP_WALL_NEAR  12
static int WallNearPenalty(int cx, int cy)
{
    if (HardObstacleCell(cx + 1, cy) || HardObstacleCell(cx - 1, cy) ||
        HardObstacleCell(cx, cy + 1) || HardObstacleCell(cx, cy - 1))
        return AP_WALL_NEAR;
    return 0;
}

// -------------------------------------------------------------------------
//  DI GIUA HANH LANG (y tuong cua user): tien mot buoc ngan ve phia dich, nhung
//  DO 2 BEN vuong goc -> don nhan vat vao TRUNG DIEM giua hai tuong. Goi lien tuc
//  (server luon co dich gan phia truoc -> di MUOT, khong dung o nga re; va luon o
//  giua duong -> khong nem than vao goc nha). Diem tra ve la Mps de gui c2s_npcrun.
// -------------------------------------------------------------------------
void AutoPathCenteredStep(int nCurX, int nCurY, int nTowX, int nTowY,
                          int* pOutX, int* pOutY)
{
    int dx = nTowX - nCurX, dy = nTowY - nCurY;
    double d = sqrt((double)dx * dx + (double)dy * dy);
    if (d < 1.0) { *pOutX = nTowX; *pOutY = nTowY; return; }
    double ux = dx / d, uy = dy / d;
    int step = (int)(d < 96.0 ? d : 96.0);              // buoc ~3 o ve phia dich
    int ax = nCurX + (int)(ux * step);
    int ay = nCurY + (int)(uy * step);                 // diem truoc mat
    double px = -uy, py = ux;                            // vuong goc huong di
    int maxR = 6, R = maxR, L = maxR, i;
    for (i = 1; i <= maxR; i++)                          // do ben +perp
    {
        int cx = MpsToCell(ax + (int)(px * i * AP_CELL));
        int cy = MpsToCell(ay + (int)(py * i * AP_CELL));
        if (HardObstacleCell(cx, cy)) { R = i - 1; break; }
    }
    for (i = 1; i <= maxR; i++)                          // do ben -perp
    {
        int cx = MpsToCell(ax - (int)(px * i * AP_CELL));
        int cy = MpsToCell(ay - (int)(py * i * AP_CELL));
        if (HardObstacleCell(cx, cy)) { L = i - 1; break; }
    }
    // Lech ve trung diem: (R - L)/2 o ve phia +perp.
    int off = ((R - L) * AP_CELL) / 2;
    int cx = ax + (int)(px * off);
    int cy = ay + (int)(py * off);
    if (HardObstacleCell(MpsToCell(cx), MpsToCell(cy))) { cx = ax; cy = ay; }
    *pOutX = cx; *pOutY = cy;
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

    // Nap truoc lop vat can ca subworld (lazy, nap lai khi doi map / ra ria vung).
    AP_EnsureFullObs(nStartMpsX, nStartMpsY);

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
            int ng = g[cur] + DC[k] + WallNearPenalty(nx, ny);   // noi vat can: tranh ke tuong
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
    {
        // TUONG THAT chan huong dich (da nap ma A* ket = tui cut / cuc tieu cuc bo).
        // Frontier straight-line khong thoat duoc khi phai di NGANG/LUI de vong tuong.
        // Bug-algorithm: thu di VONG NGANG - nham diem lech vuong goc huong dich
        // (hai ben, vai khoang cach), diem nao A* toi duoc + tien that su thi di theo.
        // Nhip sau tinh lai tu vi tri moi -> lach dan quanh vat can.
        double len = (double)dist;                 // dist = max(|dcx|,|dcy|) ~ do dai huong
        double ux = dcx / len, uy = dcy / len;     // huong dich (xap xi)
        double px = -uy, py = ux;                   // vuong goc
        static const int OFF[3] = { 32, 64, 96 };  // o lech ngang thu dan
        int side, oi, bestN = 0;
        for (oi = 0; oi < 3 && bestN == 0; oi++)
            for (side = 1; side >= -1 && bestN == 0; side -= 2)
            {
                // Lech ngang OFF o + nhich toi truoc 16 o (sidestep-and-advance).
                int lcx = scx + (int)(side * px * OFF[oi] + ux * 16.0);
                int lcy = scy + (int)(side * py * OFF[oi] + uy * 16.0);
                int nl = AutoPathFind(sx, sy, CellToMps(lcx), CellToMps(lcy), pOutX, pOutY, nMaxOut);
                if (nl <= 0) continue;
                // Waypoint dau phai roi khoi cho dung (>1 o) moi coi la thoat that.
                int wdx = MpsToCell(pOutX[0]) - scx; if (wdx < 0) wdx = -wdx;
                int wdy = MpsToCell(pOutY[0]) - scy; if (wdy < 0) wdy = -wdy;
                if (wdx + wdy < 2) continue;
                bestN = nl;
                g_DebugLog("[AP-WP] vong-ngang side=%d off=%d wp=%d -> %d,%d (dich %d,%d)",
                           side, OFF[oi], nl, pOutX[0], pOutY[0], gx, gy);
            }
        if (bestN > 0)
        {
            if (pbFinal) *pbFinal = 0;   // moi lach mot doan, dich xa van con
            return bestN;
        }
        return 0;                        // bi vay kin that su -> chiu, dung
    }

    int hop = dist < 48 ? dist : 48;     // buoc nhay ~48 o (jx9tn nhay toi 57)
    int hx = scx + (int)((double)dcx * hop / dist);
    int hy = scy + (int)((double)dcy * hop / dist);
    pOutX[0] = CellToMps(hx);
    pOutY[0] = CellToMps(hy);
    if (pbFinal) *pbFinal = (hop >= dist) ? 1 : 0;
    g_DebugLog("[AP-WP] nhay-nap %d,%d (dich %d,%d dist %d o)", pOutX[0], pOutY[0], gx, gy, dist);
    return 1;
}

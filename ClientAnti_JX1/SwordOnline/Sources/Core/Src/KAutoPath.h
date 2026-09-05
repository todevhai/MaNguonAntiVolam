// -------------------------------------------------------------------------
//  KAutoPath.h  --  A* toan cuc cho player auto-di (click dich -> di vong tuong)
//  File nguon cua ta (khong phai Kingsoft goc). Comment ASCII.
// -------------------------------------------------------------------------
#ifndef KAUTOPATH_H
#define KAUTOPATH_H

#define AUTOPATH_MAX_WP   64      // so waypoint toi da luu tren player NPC

// Tim duong A* trong subworld hien tai (SubWorld[0]). Toa do Mps.
// Tra ve so waypoint (>=0). 0 = di thang duoc HOAC khong co duong -> caller cu
// di thang nhu cu (fallback vector). Waypoint ghi vao pOutX/pOutY (Mps), da rut
// gon chi giu diem doi huong (line-of-sight smoothing), diem cuoi = dich.
int AutoPathFind(int nStartMpsX, int nStartMpsY, int nGoalMpsX, int nGoalMpsY,
                 int* pOutX, int* pOutY, int nMaxOut);

// Nhu tren nhung di toi dich XA (ngoai vung nap) theo tung nac. *pbFinal=1 neu
// toi dung dich, 0 neu moi toi mot nac trung gian (caller goi lai khi toi nac).
int AutoPathFindStep(int nStartMpsX, int nStartMpsY, int nGoalMpsX, int nGoalMpsY,
                     int* pOutX, int* pOutY, int nMaxOut, int* pbFinal);

// Blacklist o dang KET (khe hep than khong lot / va cham). A* coi vung 3x3 quanh
// o do la tuong o lan tinh sau -> vong qua thay vi lao vao lai. Goi khi ServeMove
// phat hien dung yen qua lau. Xoa danh sach khi bat dau lenh di moi.
void AutoPathMarkStuckMps(int nMpsX, int nMpsY);
void AutoPathClearStuck();

// Buoc ke tiep di GIUA hanh lang: tien ~3 o ve phia (nTowX,nTowY), do 2 ben vuong
// goc va don ve trung diem. Goi lien tuc (moi vai khung) -> di muot, khong nem tuong.
void AutoPathCenteredStep(int nCurX, int nCurY, int nTowX, int nTowY,
                          int* pOutX, int* pOutY);

#endif

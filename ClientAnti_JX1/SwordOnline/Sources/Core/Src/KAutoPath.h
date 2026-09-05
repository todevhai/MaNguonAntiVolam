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

#endif

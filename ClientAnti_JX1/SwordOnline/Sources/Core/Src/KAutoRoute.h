// -------------------------------------------------------------------------
//  KAutoRoute.h  --  Tu tim duong LIEN ban do theo bang autopathfindroutes.txt
//
//  A* (KAutoPath) chi lo duong trong MOT subworld. Module nay lo phan noi map:
//  doc bang tuyen, dung do thi map<->map, BFS ra day chang, moi chang la mot
//  diem (Mps) phai di toi trong map dang dung -- di toi do la dam vao trap,
//  server tu chuyen map, client thay m_SubWorldID doi thi sang chang ke.
//
//  File nguon cua ta (khong phai Kingsoft goc). Comment ASCII.
// -------------------------------------------------------------------------
#ifndef KAUTOROUTE_H
#define KAUTOROUTE_H

#define AUTOROUTE_MAX_HOP   16      // so map toi da tren mot tuyen

// Bat dau di toi map nToMapId (id trong maplist.ini, cung khong gian voi
// KSubWorld::m_SubWorldID). Tra ve so chang (>0) hoac 0 neu khong co tuyen.
int  AutoRouteStart(int nToMapId);

// Huy tuyen dang chay (nguoi choi tu click di cho khac, chet, dich chuyen...).
void AutoRouteCancel(const char* szWhy);

// Goi moi khung hinh khi dang trong game. Ve ngay neu khong co tuyen.
// Chiu trach nhiem: phat hien doi map -> sang chang ke; phat hien treo -> gui lai.
void AutoRouteTick();

#endif

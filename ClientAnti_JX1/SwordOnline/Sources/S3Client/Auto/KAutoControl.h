//---------------------------------------------------------------------------
// Kenh lenh dieu khien trong game -- nen tang auto in-game.
//
// Moi khung game (goi tu UiHeartBeat, ~18Hz, chi khi da vao game) doc file
// lenh "lenh-auto.txt" o thu muc chay, chay tung dong roi cat rong file (moi
// lenh chay dung mot lan). Muc dich truoc mat: verify giao dien ma KHONG cuop
// focus chuot -- ghi lenh vao file, game tu mo bang, roi chup bang
// screencapture cua so nen. Ve sau: cho cac hanh dong auto (di chuyen, dung
// ky nang, danh quai...).
//
// Lenh v1:
//   open <ten>     mo mot bang: status | skills | items
//   goto <x> <y>   di toi toa do MAN HINH (pixel viewport), dung nguyen thuy
//                  GotoWhere cua core -- KHONG phai toa do the gioi.
//   lclick <x> <y> bom click TRAI vao cay cua so UI tai toa do CLIENT
//   rclick <x> <y> bom click PHAI (vd o tui = mac/dung do neu du dieu kien)
//                  Ca hai qua Wnd_ProcessInput -- KHONG dung con tro macOS.
//   hover <x> <y>  re con tro UI toi o (WM_MOUSEMOVE) de hien tooltip mon do
//   ride           len/xuong ngua (PA_RIDE) -- dung hanh dong cua phim M.
//
// Day la ma CUA TA (khong phai nguon Kingsoft goc), nen commit thang vao kho.
// Chi phan noi day (them vao S3Client.vcxproj + 1 dong o UiHeartBeat) di qua
// port-fixes.py vi cham vao file goc.
//
// Xem docs/04-dung-client/09-kenh-lenh-dieu-khien-trong-game.md
//---------------------------------------------------------------------------
#ifndef KAUTOCONTROL_H
#define KAUTOCONTROL_H

class KAutoControl
{
public:
	static void	Tick();				// goi moi khung tu UiHeartBeat (khi da vao game)

private:
	static void	RunLine(const char* szLine);
	static void	OpenByName(const char* szName);
};

#endif // KAUTOCONTROL_H

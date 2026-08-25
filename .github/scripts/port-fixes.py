#!/usr/bin/env python3
"""Vá nguồn JX1 cho trình dịch hiện đại (MSVC v143).

Moi sua doi o day deu la mot bat nhat that trong nguon, khong phai "lam cho qua build".
Chay tu goc repo. Idempotent — chay nhieu lan khong sao.
"""
import os, sys, re

ROOT = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else '.')
SRC  = os.path.join(ROOT, 'ClientAnti_JX1', 'SwordOnline', 'Sources')
LIB  = os.path.join(ROOT, 'ClientAnti_JX1', 'SwordOnline', 'Lib')
n_ok = n_skip = 0

def edit(rel, old, new, why):
    """Thay chinh xac mot chuoi byte. Bao 'da vá roi' neu khong tim thay old nhung thay new."""
    global n_ok, n_skip
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); return
    d = open(p, 'rb').read()
    if old not in d:
        if new in d:
            print('  bo qua (da va): %s' % rel); n_skip += 1
        else:
            print('  KHONG KHOP: %s  <- %s' % (rel, why)); n_skip += 1
        return
    open(p, 'wb').write(d.replace(old, new, 1))
    print('  va: %-42s %s' % (rel, why)); n_ok += 1

def edit_all(rel, old, new, why):
    """Thay MOI lan xuat hien."""
    global n_ok, n_skip
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); return
    d = open(p, 'rb').read()
    c = d.count(old)
    if c == 0:
        print('  bo qua (da va?): %s' % rel); n_skip += 1; return
    open(p, 'wb').write(d.replace(old, new))
    print('  va: %-42s %s (%d cho)' % (rel, why, c)); n_ok += 1

def edit_after(rel, anchor, old, new, why, window=400):
    """Va lan xuat hien dau tien cua `old` nam trong `window` byte sau `anchor`.
    Dung khi chuoi can sua xuat hien nhieu lan trong file nhung chi mot cho sai."""
    global n_ok, n_skip
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); return
    d = open(p, 'rb').read()
    a = d.find(anchor)
    if a < 0:
        print('  KHONG THAY MOC: %s  <- %s' % (rel, why)); n_skip += 1; return
    seg = d[a:a+window]
    if old not in seg:
        if new in seg:
            print('  bo qua (da va): %s' % rel); n_skip += 1
        else:
            print('  KHONG KHOP: %s  <- %s' % (rel, why)); n_skip += 1
        return
    d = d[:a] + seg.replace(old, new, 1) + d[a+window:]
    open(p, 'wb').write(d)
    print('  va: %-42s %s' % (rel, why)); n_ok += 1

# ---------------------------------------------------------------- Core
# Khai bao friend thieu kieu tra ve. C++ khong con mac dinh int.
edit('Core/Src/KPlayer.h',
     b'friend\t\t\tLuaInitStandAloneGame(Lua_State * L);',
     b'friend\t\t\tint LuaInitStandAloneGame(Lua_State * L);',
     'friend thieu kieu tra ve')

# Bien toan cuc bi comment mat dinh nghia nhung con 5 cho dung no.
edit('Core/Src/KCore.cpp',
     b'//KLuaScript\t*\tg_pNpcLevelScript = NULL;',
     b'KLuaScript\t*\tg_pNpcLevelScript = NULL;',
     'dinh nghia bi comment nhung van con noi dung')

# strstr nhan const char* thi tra const char*; gan vao char* phai ep kieu.
edit('Core/Src/KGMCommand.cpp',
     b'char * pStart = strstr(pGMCmd," ");',
     b'char * pStart = (char *)strstr(pGMCmd," ");',
     'strstr tra const char*')

# ---------------------------------------------------------------- S3Client
# Repo co HAI ban iRepresentShell.h khac nhau dung 8 dong o ham khoi tao.
# S3Client.cpp viet theo ban cu (CreateRepresentShell, khong tham so) nhung lai
# include ban moi (CreateRepresentDirect). Giao dien 33 ham ao thi giong het,
# nen chi can khai bao lai dung mot typedef con thieu.
edit('S3Client/S3Client.cpp',
     b'#include "../../Represent/iRepresent/iRepresentShell.h"',
     b'#include "../../Represent/iRepresent/iRepresentShell.h"\n'
     b'/* Ban header duoc include dat ten ham khoi tao la CreateRepresentDirect va bo\n'
     b'   typedef nay; file nay goi CreateRepresentShell() khong tham so (xem\n'
     b'   CREATE_REPRESENT_SHELL_FUN ben duoi). Hai ban chi khac o ham khoi tao. */\n'
     b'typedef struct iRepresentShell* (*fnCreateRepresentShell)();',
     'typedef fnCreateRepresentShell bi bo o ban header moi')

# ---------------------------------------------------------------- Engine
# KDirectDraw::SetWindowStyle() mo dau bang exit(1) — ket thuc tien trinh ngay
# dong dau tien, moi thu con lai trong ham la ma chet.
#
# Day la rac go loi ai do de quen, khong phai logic game: ham nay chi dat kieu
# cua so va goi SetWindowPos. KDirectDraw::Init() goi no ngay sau CreateDirectDraw(),
# tuc moi client dung Engine nay deu chet o buoc khoi tao do hoa.
#
# Do bang WINEDEBUG=+relay ngay 25/08/2026: tien trinh goi ucrtbase.exit(00000001)
# tu Engine.dll+0x127e8; tra bang Engine.map ra dung
# ?SetWindowStyle@KDirectDraw@@QAEXXZ + 0x8.
# Mau mot dong: kho GitHub checkout ra CRLF nen mau nhieu dong dung \n khong bao gio
# khop. Chuoi "\texit(1);" xuat hien DUNG MOT LAN trong file nay.
edit('Engine/Src/KDDraw.cpp',
     b'\texit(1);',
     b'\t/* Da bo exit(1) o day: rac go loi de quen, giet tien trinh ngay khi khoi\n'
     b'\t   tao do hoa. Xem docs/chay-thu-client-tu-build-lan-dau.md. */',
     'bo exit(1) bo quen dau SetWindowStyle')

# ---------------------------------------------------------------- Engine: nhat ky
# g_DebugLog gui thong diep bang WM_COPYDATA toi mot cua so go loi. Khong co cua so
# do thi no la HAM RONG — moi loi khoi tao bien mat khong dau vet.
#
# Trong mot dem port (25/08/2026) diem mu nay che mat it nhat ba thu: KEvent tao
# handle hong, "Can't open ini file", va cac loi nap tai nguyen. Cho no ghi them ra
# tep de con thay duong ma di. Chi la nhat ky, khong doi hanh vi game.
#
# Mau MOT DONG (kho checkout ra CRLF). "\tif (m_hWndDebug)" xuat hien dung mot lan.
edit('Engine/Src/KDebug.cpp',
     b'\tif (m_hWndDebug)',
     b'\t{\n'
     b'\t\t/* Ghi ra tep de thay duoc ca khi khong co cua so go loi. */\n'
     b'\t\tchar _szNhatKy[512];\n'
     b'\t\tva_list _vaNhatKy;\n'
     b'\t\tva_start(_vaNhatKy, Fmt);\n'
     b'\t\t_vsnprintf(_szNhatKy, sizeof(_szNhatKy) - 2, Fmt, _vaNhatKy);\n'
     b'\t\tva_end(_vaNhatKy);\n'
     b'\t\t_szNhatKy[sizeof(_szNhatKy) - 1] = 0;\n'
     b'\t\t{\n'
     b'\t\t\tFILE *_fNhatKy = fopen("engine-debug.log", "a");\n'
     b'\t\t\tif (_fNhatKy) { fputs(_szNhatKy, _fNhatKy); fputc(10, _fNhatKy); fclose(_fNhatKy); }\n'
     b'\t\t}\n'
     b'\t}\n'
     b'\tif (m_hWndDebug)',
     'g_DebugLog ghi them ra tep engine-debug.log')

# ------------------------------- Represent2: mo lai loi ve anh alpha bi comment
# Day la thu lam man hinh den. KRepresentShell2::DrawPrimitives, nhanh
# IMAGE_RENDER_STYLE_ALPHA (kieu ve MAC DINH cua moi anh spr — xem
# KWndImage::Init, WndImage.cpp:57) co loi goi ve BI COMMENT:
#
#     case IMAGE_RENDER_STYLE_ALPHA:
#     case IMAGE_RENDER_STYLE_ALPHA_NOT_BE_LIT:
#     /*  m_Canvas.DrawSpriteAlpha(...); */
#         break;
#
# Nen moi anh spr deu "ve" ma khong ra pixel nao. Do bang WINEDEBUG + log: cua so
# goi PaintWindow moi khung hinh, nhung g_DrawSprite KHONG BAO GIO duoc goi.
# Cung ho voi exit(1) trong SetWindowStyle: rac de quen, khong phai logic game.
# DrawSpriteAlpha co that va dang duoc dung o hai nhanh khac trong chinh ham nay.
#
# Hai ban va mot dong (kho checkout ra CRLF), moi chuoi xuat hien dung mot lan.
edit('Represent/Represent2/KRepresentShell2.cpp',
     b'/*\tm_Canvas.DrawSpriteAlpha(nX, nY, pFrame->Width, pFrame->Height,',
     b'\tm_Canvas.DrawSpriteAlpha(nX, nY, pFrame->Width, pFrame->Height,',
     'mo lai loi ve alpha (bo dau mo comment)')

edit('Represent/Represent2/KRepresentShell2.cpp',
     b'a / 8);*/',
     b'a / 8);',
     'mo lai loi ve alpha (bo dau dong comment)')

# ---------------------------------------------------------------- Represent2
# Cung mot bat nhat "hai doi API" nhu o S3Client tren, nhung o phia DLL.
#
# Represent2 trong kho la doi CHONG GIAN LAN: exe cap vung nho, DLL dung
# KCanvas/KDirectDraw NGAY TRONG vung nho do, va xuat CreateRepresentDirect(uint*, uint*).
# Con Engine/Src va S3Client la doi CU: doi tuong tu so huu, ham xuat khong tham so.
#
# Do bang client tu build 24/08/2026: Game.exe goi GetProcAddress("CreateRepresentShell")
# roi pCreate() khong tham so. Neu chi vá cho Represent2 DICH duoc thi luc CHAY
# GetProcAddress van tra NULL -> ERR_T_MODULE_UNCORRECT.
#
# Keo Represent2 ve doi cu (thay vi nang Engine len doi moi) vi dem phe: S3Client cu,
# Engine/Src cu, chi Represent2/3 va mot header la moi. Va khong dung Engine —
# thu dung chung voi Core/CoreClient.
#
# Giu nguyen CreateRepresentDirect: no khong hai, va de danh cho huong chong gian lan sau nay.
edit('Represent/Represent2/KRepresentShell2.cpp',
     b'iRepresentShell* CreateRepresentDirect(unsigned int* m_Canvas, unsigned int* m_DirectDraw)',
     b'/* Ham xuat doi cu - dung cai nay. S3Client/S3Client.cpp:34,139 tim dung ten\n'
     b'   "CreateRepresentShell" va goi khong tham so. Hai con tro vung nho cua doi\n'
     b'   chong gian lan truyen NULL: ctor chi cat chung vao m_CanvasLocal/\n'
     b'   m_DirectDrawLocal, ma hai bien do khong con cho nao dung nua sau hai ban va\n'
     b'   ben duoi. */\n'
     b'iRepresentShell* CreateRepresentShell()\n'
     b'{\n'
     b'\treturn (new KRepresentShell2(NULL, NULL));\n'
     b'}\n'
     b'\n'
     b'extern "C" __declspec(dllexport)\n'
     b'iRepresentShell* CreateRepresentDirect(unsigned int* m_Canvas, unsigned int* m_DirectDraw)',
     'them ham xuat doi cu CreateRepresentShell()')

# KDirectDraw::Init(KDirectDraw*) doi mot KDirectDraw KHAC da khoi tao — dung khi tao
# be mat phu (cua so thu hai). Nhanh do chi chay khi m_lpDirectDraw != NULL, ma ctor
# (Engine/Src/KDDraw.cpp:29) dat no = NULL. Nen o lan khoi tao dau, tham so KHONG duoc
# dung den, va NULL la dung nghia "tu tao be mat chinh cua minh".
edit('Represent/Represent2/KRepresentShell2.cpp',
     b'if (m_DirectDraw.Init(m_DirectDrawLocal))',
     b'if (m_DirectDraw.Init(NULL))',
     'Init nhan KDirectDraw* chu khong phai vung nho ngoai')

# KCanvas::Init cua Engine/Src (KCanvas.h:83) la Init(int, int) — khong co tham so
# vung nho ngoai.
edit('Represent/Represent2/KRepresentShell2.cpp',
     b'm_Canvas.Init(nWidth, nHeight, m_CanvasLocal);',
     b'm_Canvas.Init(nWidth, nHeight);',
     'KCanvas::Init doi cu chi nhan 2 tham so')

# Bang con tro ham thanh vien: MSVC doi phai co & truoc ten ham thanh vien.
# Truoc day trinh dich cho phep bo, gio bao C3867. 101 cho, cung mot dang.
def fix_member_fnptr():
    global n_ok, n_skip
    rel = 'Core/Src/KProtocolProcess.cpp'
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); return
    d = open(p, 'rb').read()
    pat = re.compile(rb'(ProcessFunc\[[^\]\n]+\]\s*=\s*)(?!&|NULL\b|0\b)([A-Za-z_][A-Za-z_0-9]*)(\s*;)')
    n = [0]
    def rep(m):
        n[0] += 1
        return m.group(1) + b'&KProtocolProcess::' + m.group(2) + m.group(3)
    out = pat.sub(rep, d)
    if n[0] == 0:
        print('  bo qua (da va?): %s' % rel); n_skip += 1; return
    open(p, 'wb').write(out)
    print('  va: %-42s them & truoc ten ham thanh vien (%d cho)' % (rel, n[0])); n_ok += 1

print('\nBang con tro ham thanh vien (loi C3867):')
fix_member_fnptr()

# Bien dem cua for khong con song sau vong lap (MSVC bo /Zc:forScope- tu VS2017).
# Cac vong duoi dung lai bien dem khai bao o vong TRUOC do -> khai bao lai tai cho.
print('\nBien dem for dung lai tu vong truoc:')
edit_all('Core/Src/KItem.cpp',
         b'for (i = 0; i < 6; i++)', b'for (int i = 0; i < 6; i++)',
         'khai bao lai bien dem')
edit('Core/Src/KItemList.cpp',
     b'for (i = 0;i < MAX_PLAYER_ITEM;i++)', b'for (int i = 0;i < MAX_PLAYER_ITEM;i++)',
     'khai bao lai bien dem')
edit('Core/Src/KNpcSet.cpp',
     b'for (i = MAX_NPC_REQUEST - 1', b'for (int i = MAX_NPC_REQUEST - 1',
     'khai bao lai bien dem')
edit('Core/Src/KSkillList.cpp',
     b'for(i = 0;i < MAX_NPCSKILL;i++)', b'for(int i = 0;i < MAX_NPCSKILL;i++)',
     'khai bao lai bien dem')

# CANH BAO — day la cho DUY NHAT doi hanh vi, khong chi la cu phap:
# vong lap tim vi tri co thoi gian nho nhat va luu vao n_mMin, nhung hai dong sau
# vong lai dung j. Sau vong j == 15, ma mang chi co 15 phan tu [0..14] -> ghi
# NGOAI BIEN. Trinh dich cu cho j song sau vong nen loi nay chay im. Dung n_mMin
# la dung y do da the hien ngay trong vong.
# `k` duoc doc SAU vong lap de biet vong co chay het hay khong (kieu kiem tra
# "khong tim thay"). Doi thanh khai bao lai trong vong se lam sai nghia, nen phai
# dua khai bao ra ngoai. Giu tren MOT dong vi repo dung CRLF.
edit('Core/Src/Scene/KScenePlaceC.cpp',
     b'for (unsigned int k = 0; k < nTotalLineObj; k++)',
     b'unsigned int k; for (k = 0; k < nTotalLineObj; k++)',
     'k duoc doc sau vong -> dua khai bao ra ngoai')
edit('Core/Src/Scene/KScenePlaceC.cpp',
     b'for (i = 0; i < nNum; i++)', b'for (int i = 0; i < nNum; i++)',
     'khai bao lai bien dem')
edit_after('Core/Src/Scene/KScenePlaceC.cpp',
           b'ClearPreprocess(true);',
           b'for (i = 0; i < SPWP_MAX_NUM_REGIONS; i++)',
           b'for (int i = 0; i < SPWP_MAX_NUM_REGIONS; i++)',
           'khai bao lai bien dem (file co 19 vong dung i, chi cho nay thieu)')

print('\nGhi ngoai bien do dung bien dem sau khi vong ket thuc:')
# Tach thanh hai sua doi MOT DONG: repo checkout ra CRLF tren Windows nen mau
# nhieu dong dung \n se khong bao gio khop.
edit('Core/Src/KNpc.cpp',
     b'm_nBloodNo[j][0] = nNo;', b'm_nBloodNo[n_mMin][0] = nNo;',
     'j == 15 sau vong -> ghi ngoai bien; y do la n_mMin')
edit('Core/Src/KNpc.cpp',
     b'm_nBloodNo[j][1] = defMAX_SHOW_BLOOD_TIME;', b'm_nBloodNo[n_mMin][1] = defMAX_SHOW_BLOOD_TIME;',
     'cung cho tren, dong thu hai')

# ---------------------------------------------------------------- FilterText
# `perr` duoc doc SAU vong lap — chinh comment trong nguon noi ro la co y ("since
# we've fallen off the array"). Dua khai bao ra ngoai de giu nguyen nghia.
print('\nFilterText — bien dem doc sau vong lap:')
edit('FilterText/Regexp.cpp',
     b'for ( struct regErr * perr = errors;',
     b'struct regErr * perr; for ( perr = errors;',
     'perr duoc doc sau vong -> dua khai bao ra ngoai')

# ------------------------------------------------- hau to literal nguoi dung
def scan_udl(data):
    """Tim dau " ket thuc chuoi ma bi ngay mot dinh danh dinh sau.
    C++03 coi la noi chuoi; tu C++11 doc thanh hau to literal (UDL) -> loi C3688."""
    hits = []; i = 0; n = len(data)
    in_s = in_c = in_lc = in_bc = False
    while i < n:
        ch = data[i]
        if ch == 0x0a: in_lc = False
        if in_lc: i += 1; continue
        if in_bc:
            if ch == 0x2a and i+1 < n and data[i+1] == 0x2f: in_bc = False; i += 2; continue
            i += 1; continue
        if not in_s and not in_c:
            if ch == 0x2f and i+1 < n and data[i+1] == 0x2f: in_lc = True; i += 2; continue
            if ch == 0x2f and i+1 < n and data[i+1] == 0x2a: in_bc = True; i += 2; continue
        if in_s:
            if ch == 0x5c: i += 2; continue
            if ch == 0x22:
                in_s = False
                if i+1 < n:
                    nx = data[i+1]
                    if (0x41 <= nx <= 0x5a) or (0x61 <= nx <= 0x7a) or nx == 0x5f:
                        hits.append(i)
            i += 1; continue
        if in_c:
            if ch == 0x5c: i += 2; continue
            if ch == 0x27: in_c = False
            i += 1; continue
        if ch == 0x22: in_s = True
        elif ch == 0x27: in_c = True
        i += 1
    return hits

print('\nChen dau cach giua chuoi va dinh danh dinh lien (loi C3688):')
tot = 0
for sub in ('Core/Src', 'S3Client', 'Engine/Src'):
    base = os.path.join(SRC, sub)
    for dp, _, fns in os.walk(base):
        for fn in fns:
            if not fn.lower().endswith(('.cpp', '.c', '.h', '.hpp', '.cxx')): continue
            p = os.path.join(dp, fn)
            d = open(p, 'rb').read()
            h = scan_udl(d)
            if not h: continue
            out = bytearray(d)
            for idx in reversed(h): out[idx+1:idx+1] = b' '
            open(p, 'wb').write(bytes(out))
            print('  %-58s %d cho' % (os.path.relpath(p, SRC), len(h)))
            tot += len(h)
print('  -> tong %d cho' % tot)

# ---------------------------------------------------- Lib/Release va Lib/Debug
# Project tro toi ..\..\Lib\release\*.lib (S3Client link CoreClient.lib o day, va
# co #pragma comment(lib) tro toi FilterText_StaticLib.lib). Nhung .gitignore cua
# repo co dong "Release" nen thu muc Lib/Release/ khong bao gio duoc commit — ban
# .lib that nam ngay o Lib/. Dung lai hai thu muc do va copy sang.
print('\nDung Lib/Release va Lib/Debug (bi .gitignore loai khoi repo):')
libs = [f for f in os.listdir(LIB) if f.lower().endswith(('.lib', '.dll'))] if os.path.isdir(LIB) else []
for sub in ('Release', 'Debug'):
    d = os.path.join(LIB, sub)
    os.makedirs(d, exist_ok=True)
    for f in libs:
        open(os.path.join(d, f), 'wb').write(open(os.path.join(LIB, f), 'rb').read())
    print('  Lib/%s <- %d file' % (sub, len(libs)))

# Cac .lib dung san (JpgLib, FilterText_StaticLib) duoc dich truoc thoi SAFESEH nen
# trinh lien ket tu choi sinh anh co bang xu ly ngoai le an toan. SAFESEH la metadata
# cua item <Link> trong vcxproj, khong ghi de duoc bang /p: tren dong lenh -> sua file.
print('\nTat SAFESEH + them duong thu vien vao vcxproj:')
for rel in ('Engine/Engine.vcxproj', 'Core/Core.vcxproj', 'S3Client/S3Client.vcxproj'):
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); continue
    d = open(p, 'rb').read()
    if b'<ImageHasSafeExceptionHandlers>' in d:
        print('  bo qua (da co): %s' % rel); continue
    # Nguon Engine co #pragma comment(lib,"common.lib") va "CoreClient.lib" nhung
    # project khong khai bao duong thu vien nao -> trinh lien ket khong thay Lib/.
    # LibraryPath cua MSBuild ghi de bien LIB cua shell, nen phai ghi vao vcxproj.
    tag = (b'<ImageHasSafeExceptionHandlers>false</ImageHasSafeExceptionHandlers>'
           b'\r\n      <AdditionalLibraryDirectories>'
           b'$(SolutionDir)..\\Lib;$(SolutionDir)..\\Lib\\Release;'
           b'$(MSBuildThisFileDirectory)..\\..\\Lib;'
           b'$(MSBuildThisFileDirectory)..\\..\\Lib\\Release;'
           b'%(AdditionalLibraryDirectories)</AdditionalLibraryDirectories>'
           # common.lib doi VC6 mang chi thi /DEFAULTLIB:LIBC. LIBC.lib (CRT tinh don
           # luong) da bi go khoi MSVC tu lau -> phai bao trinh lien ket bo qua.
           b'\r\n      <IgnoreSpecificDefaultLibraries>'
           b'LIBC.lib;LIBCD.lib;%(IgnoreSpecificDefaultLibraries)'
           b'</IgnoreSpecificDefaultLibraries>')
    out = d.replace(b'<Link>', b'<Link>\r\n      ' + tag)
    c = d.count(b'<Link>')
    open(p, 'wb').write(out)
    print('  va: %-42s %d khoi <Link>' % (rel, c))

# Lenh hau-build cua chinh cac project lo viec copy .lib/.dll vao Lib\release va
# bin\client\release. Chung mo dau bang `md <thu muc>` — lenh nay tra ve loi khi thu
# muc DA TON TAI, va MSBuild coi ma thoat khac 0 la build that bai (MSB3073), du
# .lib/.dll da dung xong. Them `exit /b 0` de khoi phan copy tien loi nay khong the
# lam sap build.
print('\nLam lenh hau-build khong the lam sap build:')
for rel in ('Engine/Engine.vcxproj', 'Core/Core.vcxproj', 'S3Client/S3Client.vcxproj'):
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); continue
    d = open(p, 'rb').read()
    if b'exit /b 0</Command>' in d:
        print('  bo qua (da co): %s' % rel); continue
    c = d.count(b'</Command>')
    out = d.replace(b'</Command>', b'\r\nexit /b 0</Command>')
    open(p, 'wb').write(out)
    print('  va: %-42s %d khoi <Command>' % (rel, c))

# Tao san cac thu muc dich cua lenh hau-build, de phan copy chay that thay vi truot.
print('\nTao san thu muc dich cua lenh hau-build:')
for d in ('Lib/release', 'Lib/debug',
          '../bin/client/release', '../bin/client/debug',
          '../bin/server/release', '../bin/server/debug'):
    full = os.path.normpath(os.path.join(ROOT, 'ClientAnti_JX1', 'SwordOnline', d))
    os.makedirs(full, exist_ok=True)
    print('  %s' % os.path.relpath(full, ROOT))

# ------------------------------------------------- dang nhap tu dong cua ta
# Nguon co san mot duong tu dang nhap (KLogin::AutoLogin, ban bang Alt+A tren
# man khoi dong), nhung no doi ba thu: config.ini [Main] AutoLogin=6323, tai
# khoan/mat khau da ma hoa trong UserData\UiCommon.ini, VA ten nhan vat trong
# UserData\<id>\UiConfig.ini. Cai cuoi la vong tron: <id> chi sinh ra tu
# (tai khoan + ten nhan vat) SAU lan dang nhap dau tien, nen tren mot may sach
# IsAutoLoginEnable() luon tra false.
#
# Ta so huu nguon nen khong phai cam ngoai hay ghi thang vao bo nho: them mot
# duong vao doc [AutoLogin] trong \Ui\Setting.ini. Khong hook, khong dia chi
# cung, song cung binary cua ta.
print('\nDang nhap tu dong theo cau hinh:')

def _crlf(s):
    return s.replace(b'\n', b'\r\n')

# DOI HANH VI: sscanf ghi octet thu ba vao &nValue (dia chi CA MANG = &nValue[0])
# thay vi &nValue[2]. Hau qua: nValue[2] giu rac ngan xep, phep kiem
# `nValue[2] < 256` truot -> GetIpAddress tra false -> MOI may chu trong
# ServerList.ini bi bo qua -> danh sach may chu rong.
# Do duoc 25/08/2026 bang log cua chinh game: "[TuDong] so may chu=0" trong khi
# settings/ServerList.ini co dung mot muc hop le.
#
# Khong dung edit(): mau neo phai KHONG chua ky tu xuong dong (repo checkout ra
# CRLF tren CI nhung LF tren macOS), ma neu bo xuong dong thi "&nValue" lai nam
# trong chinh ket qua "&nValue[2]" -> khong con idempotent. Nen tu kiem truoc.
def fix_getipaddress():
    global n_ok, n_skip
    rel = 'S3Client/Login/Login.cpp'
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); return
    d = open(p, 'rb').read()
    if b'&nValue[1], &nValue[2]' in d:
        print('  bo qua (da va): %s' % rel); n_skip += 1; return
    if b'&nValue[1], &nValue' not in d:
        print('  KHONG KHOP: %s  <- GetIpAddress octet thu ba' % rel); n_skip += 1; return
    d = d.replace(b'&nValue[1], &nValue', b'&nValue[1], &nValue[2]', 1)
    open(p, 'wb').write(d)
    print('  va: %-42s GetIpAddress: &nValue -> &nValue[2]' % rel); n_ok += 1

fix_getipaddress()

edit('S3Client/Login/Login.cpp',
     b'#include "KEngine.h"',
     b'#include "KEngine.h"\r\n#include "KDebug.h"',
     'them KDebug.h cho g_DebugLog')

edit('S3Client/Ui/UiCase/UiInit.cpp',
     b'#include "KIniFile.h"',
     b'#include "KIniFile.h"\r\n#include "KDebug.h"',
     'them KDebug.h cho g_DebugLog')

edit('S3Client/Login/Login.h',
     b'\tvoid AutoLogin();',
     _crlf(b'\tvoid AutoLogin();\n'
           b'\t//Dang nhap tu dong tu tai khoan cho san, khong qua UserData\\<id>\\UiConfig.ini\n'
           b'\tint\tDangNhapTuDongTheoCauHinh(const char* pszAccount, const KSG_PASSWORD& crPassword);'),
     'khai bao DangNhapTuDongTheoCauHinh')

edit('S3Client/Login/Login.cpp',
     b'void KLogin::AutoLogin()',
     _crlf(b'/* Dang nhap tu dong tu tai khoan + mat khau cho san.\n'
           b'   Khac AutoLogin() goc o cho khong doi ten nhan vat va ten may chu da luu:\n'
           b'   danh sach nhan vat ve toi dau thi m_bInAutoProgress tu chon muc dau tien,\n'
           b'   con dia chi may chu lay tu Settings\\ServerList.ini. */\n'
           b'int KLogin::DangNhapTuDongTheoCauHinh(const char* pszAccount, const KSG_PASSWORD& crPassword)\n'
           b'{\n'
           b'\tif (!pszAccount || !pszAccount[0])\n'
           b'\t\treturn false;\n'
           b'\n'
           b'\tReturnToIdle();\n'
           b'\tLoadLoginChoice();\t\t\t/* memset m_Choices - phai goi TRUOC khi dat tai khoan */\n'
           b'\tSetAccountPassword(pszAccount, &crPassword);\n'
           b'\tm_bInAutoProgress = true;\n'
           b'\n'
           b'\tint nCount = 0, nSel = 0;\n'
           b'\t/* Ham nay dong thoi dien m_Choices.AccountServer.Address tu ServerList.ini */\n'
           b'\tKLoginServer* pList = GetServerList(-1, nCount, nSel);\n'
           b'\tif (pList)\n'
           b'\t\tfree(pList);\n'
           b'\tg_DebugLog("[TuDong] tai khoan=%s so may chu=%d dia chi=%d.%d.%d.%d",\n'
           b'\t\tpszAccount, nCount,\n'
           b'\t\tm_Choices.AccountServer.Address[0], m_Choices.AccountServer.Address[1],\n'
           b'\t\tm_Choices.AccountServer.Address[2], m_Choices.AccountServer.Address[3]);\n'
           b'\tif (nCount <= 0)\n'
           b'\t{\n'
           b'\t\tm_bInAutoProgress = false;\n'
           b'\t\treturn false;\n'
           b'\t}\n'
           b'\tint nNoi = CreateConnection(m_Choices.AccountServer.Address);\n'
           b'\tg_DebugLog("[TuDong] CreateConnection tra %d, trang thai %d", nNoi, (int)m_Status);\n'
           b'\treturn nNoi;\n'
           b'}\n'
           b'\n'
           b'void KLogin::AutoLogin()'),
     'them KLogin::DangNhapTuDongTheoCauHinh')

# ShowCompleted() chay NGAY TRONG KUiInit::OpenWindow. Goi CloseWindow() o do
# se dat m_pSelf = NULL truoc khi OpenWindow kip tra ve no -> UiStart() thay NULL
# -> hop thoai "Khoi tao module that bai: UiStart (ma 4)". Nen o day chi dung co,
# viec that lam trong WndProc, dung cho ma OnAutoLogin (Alt+A) van lam.
edit('S3Client/Ui/UiCase/UiInit.cpp',
     b'    m_EnterGame.SetCursorAbove();',
     _crlf(b'    m_EnterGame.SetCursorAbove();\n'
           b'    g_bXinDangNhapTuDong = true;'),
     'dung co xin dang nhap tu dong')

edit('S3Client/Ui/UiCase/UiInit.cpp',
     b'int KUiInit::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)',
     _crlf(b'/* Dang nhap tu dong theo cau hinh cua ta. Dat o day chu khong o ShowCompleted\n'
           b'   vi ShowCompleted chay ngay trong OpenWindow: dong cua so o do se lam\n'
           b'   UiStart() thay NULL. */\n'
           b'bool g_bXinDangNhapTuDong = false;\n'
           b'\n'
           b'static void DangNhapTuDongTheoCauHinh()\n'
           b'{\n'
           b'    char szTepCauHinh[] = "\\\\Ui\\\\Setting.ini";\n'
           b'    KIniFile Ini;\n'
           b'    if (!Ini.Load(szTepCauHinh))\n'
           b'        return;\n'
           b'\n'
           b'    int nBat = 0;\n'
           b'    Ini.GetInteger("AutoLogin", "Enable", 0, &nBat);\n'
           b'    if (!nBat)\n'
           b'        return;\n'
           b'\n'
           b'    char szTaiKhoan[32];\n'
           b'    KSG_PASSWORD MatKhau;\n'
           b'    szTaiKhoan[0] = 0;\n'
           b'    memset(&MatKhau, 0, sizeof(MatKhau));\n'
           b'    Ini.GetString("AutoLogin", "Account", "", szTaiKhoan, sizeof(szTaiKhoan));\n'
           b'    Ini.GetString("AutoLogin", "Password", "", MatKhau.szPassword, sizeof(MatKhau.szPassword));\n'
           b'    g_DebugLog("[TuDong] Setting.ini Enable=%d Account=\\"%s\\"", nBat, szTaiKhoan);\n'
           b'    if (!szTaiKhoan[0])\n'
           b'        return;\n'
           b'\n'
           b'    KUiInit::CloseWindow();\n'
           b'    KUiConnectInfo::OpenWindow(CI_MI_CONNECTING, LL_S_IN_GAME);\n'
           b'    g_LoginLogic.DangNhapTuDongTheoCauHinh(szTaiKhoan, MatKhau);\n'
           b'}\n'
           b'\n'
           b'int KUiInit::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)'),
     'ham dang nhap tu dong + co toan cuc')

# WndProc chi chay khi CO thong diep (chuot/phim). Man khoi dong khong nhan gi
# thi co khong bao gio duoc tieu thu. Nen ban tu UiPaint() — ham chay MOI KHUNG HINH,
# va la diem an toan (ngoai luc dung cua so).
edit('S3Client/Ui/UiCase/UiInit.h',
     b'#endif // __UIINIT_H__',
     _crlf(b'//Kiem xem co xin dang nhap tu dong khong; goi moi khung hinh tu UiPaint().\n'
           b'void KiemDangNhapTuDong();\n'
           b'\n'
           b'#endif // __UIINIT_H__'),
     'khai bao KiemDangNhapTuDong')

edit('S3Client/Ui/UiCase/UiInit.cpp',
     b'int KUiInit::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)',
     _crlf(b'void KiemDangNhapTuDong()\n'
           b'{\n'
           b'    if (!g_bXinDangNhapTuDong)\n'
           b'        return;\n'
           b'    g_bXinDangNhapTuDong = false;\n'
           b'    DangNhapTuDongTheoCauHinh();\n'
           b'}\n'
           b'\n'
           b'int KUiInit::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)'),
     'ham KiemDangNhapTuDong')

edit('S3Client/Ui/UiShell.cpp',
     b'\tif (g_pRepresentShell == NULL ||',
     _crlf(b'\tKiemDangNhapTuDong();\n'
           b'\tif (g_pRepresentShell == NULL ||'),
     'goi KiemDangNhapTuDong moi khung hinh')


# Log chan doan tang giao thuc client: biet dich xac opcode nao khong co nguoi xu ly,
# va biet client co coi la "da vao game" hay khong.
edit('Core/Src/KProtocolProcess.cpp',
     b'\t\tg_DebugLog("[error]Net Msg Error");',
     b'\t\tg_DebugLog("[error]Net Msg Error, opcode=%d", pMsg ? (int)pMsg[0] : -1);',
     'ghi ca opcode vao Net Msg Error')

edit('Core/Src/KProtocolProcess.cpp',
     b'\tg_DebugLog("[net]Msg:%c", pMsg[0]);',
     b'\tg_DebugLog("[net]Msg:%d", (int)pMsg[0]);',
     'in opcode dang so thay vi ky tu')

edit('S3Client/Login/Login.cpp',
     b'\t\tm_Status = LL_S_IN_GAME;',
     _crlf(b'\t\tg_DebugLog("[TuDong] DA VAO GAME, nhan vat=%s", m_Choices.szProcessingRoleName);\n'
           b'\t\tm_Status = LL_S_IN_GAME;'),
     'log khi client coi la da vao game')

# ProcessToLoginGameServResponse so ten tai khoan bang cach DAO BIT m_Choices.Account.
# SetAccountPassword dung strncpy (dem bang '\0') roi dao ca 32 byte, nen ban tham
# chieu pzAc la "taikhoanthu" + 21 byte 0xFF. Goi tu cong vao lai dem bang '\0'.
# strcmp gap 0x00 vs 0xFF o vi tri 11 -> KHONG BAO GIO khop voi tai khoan ngan hon
# 32 ky tu -> client bo qua goi va khong noi toi world server.
# Chinh tac gia ban nay da COMMENT phep so do trong ham anh em ProcessGetRoleResponse.
# Nen: giu phep so TEN NHAN VAT, bo phep so tai khoan.
edit('S3Client/Login/Login.cpp',
     b'&& strcmp((const char*)pResponse->szAccountName, pzAc) == 0)',
     b'/* bo phep so tai khoan: pzAc dem bang 0xFF nen khong bao gio khop */)',
     'bo phep so tai khoan khong bao gio khop trong notifyplayerlogin')

edit('S3Client/Login/Login.cpp',
     b'\t\t\tif (g_NetConnectAgent.ConnectToGameSvr(',
     _crlf(b'\t\t\tg_DebugLog("[TuDong] noi world server %d.%d.%d.%d:%d",\n'
           b'\t\t\t\t((unsigned char*)&pResponse->nIPAddr)[0], ((unsigned char*)&pResponse->nIPAddr)[1],\n'
           b'\t\t\t\t((unsigned char*)&pResponse->nIPAddr)[2], ((unsigned char*)&pResponse->nIPAddr)[3],\n'
           b'\t\t\t\t(int)pResponse->nPort);\n'
           b'\t\t\tif (g_NetConnectAgent.ConnectToGameSvr('),
     'log dia chi world server truoc khi noi')

# Log dung cho vao ra tren socket world server: bao nhieu byte, byte dau la gi,
# va bang kich thuoc cua CLIENT tra ve bao nhieu cho tung opcode. Day la cho duy
# nhat biet duoc client CO doc dung khoi du lieu dang nhap hay khong.
edit('S3Client/NetConnect/NetConnectAgent.cpp',
     b'\t\t\twhile(pMsg < (PROTOCOL_MSG_TYPE*)(pBuffer + nSize))',
     _crlf(b'\t\t\tg_DebugLog("[TuDong] the gioi: %u byte, dau %d %d %d %d", nSize,\n'
           b'\t\t\t\t(int)(unsigned char)pBuffer[0], (int)(unsigned char)pBuffer[1],\n'
           b'\t\t\t\t(int)(unsigned char)pBuffer[2], (int)(unsigned char)pBuffer[3]);\n'
           b'\t\t\twhile(pMsg < (PROTOCOL_MSG_TYPE*)(pBuffer + nSize))'),
     'log khoi du lieu tu world server')

edit('S3Client/NetConnect/NetConnectAgent.cpp',
     b'\t\t\t\t\tg_pCoreShell->NetMsgCallbackFunc(pMsg);',
     _crlf(b'\t\t\t\t\tg_DebugLog("[TuDong]   opcode=%d, bang bao %d byte",\n'
           b'\t\t\t\t\t\t(int)Msg, g_pCoreShell->GetProtocolSize(Msg));\n'
           b'\t\t\t\t\tg_pCoreShell->NetMsgCallbackFunc(pMsg);'),
     'log tung opcode va kich thuoc bang tra ve')

# Log duong dung nhan vat phia client: s2c_syncnpc tao doi tuong, s2c_syncplayer
# dat trang bi, s2c_synccurplayer gan nhan vat cua minh. Neu mot khau tra chi so 0
# thi khong co gi de ve.
edit('Core/Src/KProtocolProcess.cpp',
     b'\t\tnIdx = NpcSet.Add(NpcSync->NpcSettingIdx, 0, NpcSync->MapX, NpcSync->MapY);',
     _crlf(b'\t\tnIdx = NpcSet.Add(NpcSync->NpcSettingIdx, 0, NpcSync->MapX, NpcSync->MapY);\n'
           b'\t\tg_DebugLog("[TuDong] SyncNpc: mau=%d cap=%d vitri=%d,%d -> nIdx=%d",\n'
           b'\t\t\t(int)(short)HIWORD(NpcSync->NpcSettingIdx), (int)LOWORD(NpcSync->NpcSettingIdx),\n'
           b'\t\t\t(int)NpcSync->MapX, (int)NpcSync->MapY, nIdx);'),
     'log SyncNpc tao doi tuong')

edit('Core/Src/KPlayer.cpp',
     b'\tthis->m_nIndex = NpcSet.SearchID(PlaySync->m_dwID);',
     _crlf(b'\tthis->m_nIndex = NpcSet.SearchID(PlaySync->m_dwID);\n'
           b'\tg_DebugLog("[TuDong] SyncCurPlayer: dwID=%u -> m_nIndex=%d",\n'
           b'\t\t(unsigned)PlaySync->m_dwID, this->m_nIndex);'),
     'log SyncCurPlayer tim doi tuong')

# KNpc::Load voi nhan vat NGUOI CHOI dat szNpcTypeName = "" (nhanh bi rut ruot),
# ma KNpcRes::Init tra FALSE ngay khi ten rong (KNpcRes.cpp:58) -> m_DataRes khong
# bao gio duoc dung -> m_DataRes.Draw() trong KNpc::Paint khong ve gi.
#
# Ten loai tai nguyen nam san trong settings/npcres/<nhan vat>.txt: dong 2 la
# MainMan, dong 3 la MainLady. Nhanh NPC thuong ngay ben duoi da lay dung bang
# nay bang g_NpcKindFile.GetString(2, ...), chi nhanh nguoi choi la bo trong.
edit('Core/Src/KNpc.cpp',
     b'\t\t\tm_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);',
     _crlf(b'\t\t\t/* dong 2 cua bang loai nhan vat = MainMan */\n'
           b'\t\t\tg_NpcKindFile.GetString(2, "", "", szNpcTypeName, sizeof(szNpcTypeName));\n'
           b'\t\t\tm_StandFrame = NpcSet.GetPlayerStandFrame(TRUE);'),
     'lay ten tai nguyen nam nhan vat tu bang loai')

edit('Core/Src/KNpc.cpp',
     b'\t\t\tm_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);',
     _crlf(b'\t\t\t/* dong 3 cua bang loai nhan vat = MainLady */\n'
           b'\t\t\tg_NpcKindFile.GetString(3, "", "", szNpcTypeName, sizeof(szNpcTypeName));\n'
           b'\t\t\tm_StandFrame = NpcSet.GetPlayerStandFrame(FALSE);'),
     'lay ten tai nguyen nu nhan vat tu bang loai')

edit('Core/Src/KNpc.cpp',
     b'\tm_DataRes.Init(szNpcTypeName, &g_NpcResList);',
     _crlf(b'\tg_DebugLog("[TuDong] KNpc::Load mau=%d ten tai nguyen=\\"%s\\" -> Init=%d",\n'
           b'\t\tnNpcSettingIdx, szNpcTypeName,\n'
           b'\t\t(int)m_DataRes.Init(szNpcTypeName, &g_NpcResList));'),
     'log ket qua dung tai nguyen hinh')

# Log tung cua ai trong KNpcResNode::Init de biet dung cho nao tra FALSE.
edit('Core/Src/KNpcResNode.cpp',
     b'#include\t"KFilePath.h"',
     b'#include\t"KFilePath.h"\r\n#include\t"KDebug.h"',
     'them KDebug.h cho g_DebugLog')

edit('Core/Src/KNpcResNode.cpp',
     b'\tnFindNo = KindFile.FindRow(lpszNpcName);',
     _crlf(b'\tnFindNo = KindFile.FindRow(lpszNpcName);\n'
           b'\tg_DebugLog("[TuDong] ResNode \\"%s\\": dong trong bang = %d", lpszNpcName, nFindNo);'),
     'log tim dong trong bang loai nhan vat')

edit('Core/Src/KNpcResNode.cpp',
     b'\t\tif ( !PartFile.Load(szBuf) )',
     _crlf(b'\t\tg_DebugLog("[TuDong] ResNode: tep bo phan = \\"%s\\"", szBuf);\n'
           b'\t\tif ( !PartFile.Load(szBuf) )'),
     'log tep danh sach bo phan')

# Bang settings/npcres/<nhan vat>.txt trong bo du lieu ta dung co dong tieu de
# bang TIENG ANH:
#   CharacterName CharacterType ResFilePath PartFileName WeaponActionTab1
#   WeaponActionTab2 ActionRenderOrderTab Head Hair Shoulder Body ...
# con nguon tra cot bang TEN TIENG TRUNG -> khong khop -> GetString tra rong ->
# KNpcResNode::Init tra FALSE -> nhan vat khong co bo hinh nao.
# Do duoc 25/08/2026: log "ResNode MainMan: dong trong bang = 2" chay qua, nhung
# log ke tiep (tep bo phan) khong bao gio in ra.
#
# Doi dinh nghia sang dung ten cot cua du lieu. Gia tri CharacterType cung vay:
# du lieu ghi 'SpecialNpc'/'NormalNpc' chu khong phai chu Trung.
for cu, moi, ten in (
        (b'"\xc8\xcb\xce\xef\xc0\xe0\xd0\xcd"', b'"CharacterType"', 'KIND_NAME_SECT'),
        (b'"\xcc\xd8\xca\xe2npc"', b'"SpecialNpc"', 'KIND_NAME_SPECIAL'),
        (b'"\xc6\xd5\xcd\xa8npc"', b'"NormalNpc"', 'KIND_NAME_NORMAL'),
        (b'"\xb2\xbf\xbc\xfe\xcb\xb5\xc3\xf7\xce\xc4\xbc\xfe\xc3\xfb"', b'"PartFileName"', 'KIND_FILE_SECT1'),
        (b'"\xce\xe4\xc6\xf7\xd0\xd0\xce\xaa\xb9\xd8\xc1\xaa\xb1\xed1"', b'"WeaponActionTab1"', 'KIND_FILE_SECT2'),
        (b'"\xce\xe4\xc6\xf7\xd0\xd0\xce\xaa\xb9\xd8\xc1\xaa\xb1\xed2"', b'"WeaponActionTab2"', 'KIND_FILE_SECT3'),
        (b'"\xb6\xaf\xd7\xf7\xcc\xf9\xcd\xbc\xcb\xb3\xd0\xf2\xb1\xed"', b'"ActionRenderOrderTab"', 'KIND_FILE_SECT4'),
        (b'"\xd7\xca\xd4\xb4\xce\xc4\xbc\xfe\xc2\xb7\xbe\xad"', b'"ResFilePath"', 'KIND_FILE_SECT5'),
):
    edit('Core/Src/CoreUseNameDef.h', cu, moi, 'ten cot %s -> %s' % (ten, moi.decode()))

# --------------------------------------------------------- header bu ten ham
# Engine khai bao g_strcpy / g_strcpyLen (chu 's' thuong) nhung Core goi
# g_StrCpy / g_StrCpyLen (chu 'S' hoa). Ca nhom con lai (g_StrCat, g_StrCmp,
# g_StrLen, g_StrLower, g_StrUpper) thi khop. Hai ten nay la di san lech ban.
compat = os.path.join(SRC, 'msvc-compat.h')
open(compat, 'wb').write(b'''/* Nap cuong buc khi dung Core (/FI). Engine khai bao g_strcpy va g_strcpyLen
   voi chu 's' thuong; Core goi chung voi chu 'S' hoa. Moi ten g_Str* con lai
   thi khop, nen chi bac cau dung hai ten nay. */
#ifndef MSVC_COMPAT_H
#define MSVC_COMPAT_H
/* KProtocolProcess.cpp dung std::string va std::vector ma khong include; truoc day
   chung den qua <windows.h> hoac qua header khac da doi. */
#include <string>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#define g_StrCpy    g_strcpy
#define g_StrCpyLen g_strcpyLen
#endif
''')
print('\ntao %s' % os.path.relpath(compat, ROOT))
print('\n=== va %d cho, bo qua %d ===' % (n_ok, n_skip))

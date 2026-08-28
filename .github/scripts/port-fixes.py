#!/usr/bin/env python3
"""Vá nguồn JX1 cho trình dịch hiện đại (MSVC v143).

Moi sua doi o day deu la mot bat nhat that trong nguon, khong phai "lam cho qua build".
Chay tu goc repo. Idempotent — chay nhieu lan khong sao.
"""
import os, sys, re

ROOT = os.path.abspath(sys.argv[1] if len(sys.argv) > 1 else '.')
SRC  = os.path.join(ROOT, 'ClientAnti_JX1', 'SwordOnline', 'Sources')
LIB  = os.path.join(ROOT, 'ClientAnti_JX1', 'SwordOnline', 'Lib')
n_ok = n_skip = n_hong = 0

def edit(rel, old, new, why):
    """Thay chinh xac mot chuoi byte. Bao 'da vá roi' neu khong tim thay old nhung thay new."""
    global n_ok, n_skip, n_hong
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); n_hong += 1; return
    d = open(p, 'rb').read()
    if old not in d:
        if new in d:
            print('  bo qua (da va): %s' % rel); n_skip += 1
        else:
            print('  KHONG KHOP: %s  <- %s' % (rel, why)); n_hong += 1
        return
    open(p, 'wb').write(d.replace(old, new, 1))
    print('  va: %-42s %s' % (rel, why)); n_ok += 1

def edit_all(rel, old, new, why):
    """Thay MOI lan xuat hien."""
    global n_ok, n_skip, n_hong
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); n_hong += 1; return
    d = open(p, 'rb').read()
    c = d.count(old)
    if c == 0:
        print('  bo qua (da va?): %s' % rel); n_skip += 1; return
    open(p, 'wb').write(d.replace(old, new))
    print('  va: %-42s %s (%d cho)' % (rel, why, c)); n_ok += 1

def edit_after(rel, anchor, old, new, why, window=400):
    """Va lan xuat hien dau tien cua `old` nam trong `window` byte sau `anchor`.
    Dung khi chuoi can sua xuat hien nhieu lan trong file nhung chi mot cho sai."""
    global n_ok, n_skip, n_hong
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); n_hong += 1; return
    d = open(p, 'rb').read()
    a = d.find(anchor)
    if a < 0:
        print('  KHONG THAY MOC: %s  <- %s' % (rel, why)); n_hong += 1; return
    seg = d[a:a+window]
    if old not in seg:
        if new in seg:
            print('  bo qua (da va): %s' % rel); n_skip += 1
        else:
            print('  KHONG KHOP: %s  <- %s' % (rel, why)); n_hong += 1
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
    global n_ok, n_skip, n_hong
    rel = 'Core/Src/KProtocolProcess.cpp'
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); n_hong += 1; return
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
    global n_ok, n_skip, n_hong
    rel = 'S3Client/Login/Login.cpp'
    p = os.path.join(SRC, rel)
    if not os.path.exists(p):
        print('  THIEU FILE %s' % rel); n_hong += 1; return
    d = open(p, 'rb').read()
    if b'&nValue[1], &nValue[2]' in d:
        print('  bo qua (da va): %s' % rel); n_skip += 1; return
    if b'&nValue[1], &nValue' not in d:
        print('  KHONG KHOP: %s  <- GetIpAddress octet thu ba' % rel); n_hong += 1; return
    d = d.replace(b'&nValue[1], &nValue', b'&nValue[1], &nValue[2]', 1)
    open(p, 'wb').write(d)
    print('  va: %-42s GetIpAddress: &nValue -> &nValue[2]' % rel); n_ok += 1

fix_getipaddress()

edit('S3Client/Login/Login.cpp',
     b'#include "KEngine.h"',
     b'#include "KEngine.h"\r\n#include "KDebug.h"',
     'them KDebug.h cho g_DebugLog')

# KiemDangNhapTuDong goi KSG_StringToMD5String nen phai keo header vao.
edit('S3Client/Ui/UiCase/UiInit.cpp',
     b'#include "KIniFile.h"',
     b'#include "KIniFile.h"\n#include "KSG_MD5_String.h"',
     'them KSG_MD5_String.h cho UiInit')

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
           b'    char szMatKhauTho[KSG_PASSWORD_MAX_SIZE];\n'
           b'    szMatKhauTho[0] = 0;\n'
           b'    Ini.GetString("AutoLogin", "Password", "", szMatKhauTho, sizeof(szMatKhauTho));\n'
           b'    /* Phai di DUNG duong ma giao dien di (UiLogin.cpp:316), khong thi\n'
           b'       tu dang nhap gui mat khau THO con go tay gui MD5 -> cung mot tai\n'
           b'       khoan ma hai bi mat khac nhau, duong nao dang ky truoc thi duong\n'
           b'       kia bi khoa ngoai. Chi lo ra khi may chu bat kiem mat khau. */\n'
           b'#ifdef SWORDONLINE_USE_MD5_PASSWORD\n'
           b'    KSG_StringToMD5String(MatKhau.szPassword, szMatKhauTho);\n'
           b'#else\n'
           b'    strncpy(MatKhau.szPassword, szMatKhauTho, sizeof(MatKhau.szPassword) - 1);\n'
           b'#endif\n'
           b'    memset(szMatKhauTho, 0, sizeof(szMatKhauTho));\n'
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

# Doi ten tep giao dien tu GBK sang ASCII cua ta.
# Ban do va ly do: client/ten-ini-tu-viet.txt trong kho VLTK-Origin.
# Ten GBK khong song duoc qua nhieu tang: APFS tu choi (Errno 92), va hien
# chi chay duoc nho mot trung hop trong cach Wine doi ANSI CP1252 sang UTF-8.
edit('S3Client/Ui/UiChatPhrase.cpp',
     b'"\\\\Ui\\\\\xc1\xc4\xcc\xec\xb6\xaf\xd7\xf7.ini"',
     b'"\\\\Ui\\\\dong-tac-chat.ini"',
     'ten tep EMOTE_INI_FILE -> dong-tac-chat.ini')

edit('S3Client/Ui/UiCase/UiEquip.cpp',
     b'"\xd7\xb0\xb1\xb8\xb7\xd6\xd2\xb3.ini"',
     b'"trang-bi-phan-trang.ini"',
     'ten tep EQUIP_INI_PAGE -> trang-bi-phan-trang.ini')

edit('S3Client/Ui/ShortcutKey.cpp',
     b'"\\\\Ui\\\\\xd4\xd3\xbb\xe2.ini"',
     b'"\\\\Ui\\\\tap-hop.ini"',
     'ten tep GAME_CONFIG_STRING_INI -> tap-hop.ini')

edit('S3Client/Ui/UiCase/UiEquip.cpp',
     b'"\xd7\xb0\xb1\xb8\xc2\xed\xb7\xd6\xd2\xb3.ini"',
     b'"trang-bi-ngua-phan-trang.ini"',
     'ten tep HORSE_INI_PAGE -> trang-bi-ngua-phan-trang.ini')

edit('S3Client/Ui/UiCase/UiStrengthRank.cpp',
     b'"\xc5\xc5\xc3\xfb\xb5\xc4\xc1\xd9\xca\xb1\xce\xc4\xbc\xfe.ini"',
     b'"xep-hang-tam.ini"',
     'ten tep INDEX_AND_RANK_DATA_INI -> xep-hang-tam.ini')

edit('S3Client/Ui/GameSpaceChangedNotify.cpp',
     b'"\\\\Ui\\\\\xd4\xd3\xbb\xe2.ini"',
     b'"\\\\Ui\\\\tap-hop.ini"',
     'ten tep LEVEL_TIPS_INI -> tap-hop.ini')

edit('S3Client/Ui/UiCase/UiManage.cpp',
     b'"\xb0\xef\xc5\xc9\xb9\xdc\xc0\xed.ini"',
     b'"quan-ly-bang-phai.ini"',
     'ten tep MANAGE_INI_CLIQUE -> quan-ly-bang-phai.ini')

edit('S3Client/Ui/UiCase/UiManage.cpp',
     b'"\xb0\xef\xbb\xe1\xb9\xdc\xc0\xed.ini"',
     b'"quan-ly-bang-hoi.ini"',
     'ten tep MANAGE_INI_CONFRATERNITY -> quan-ly-bang-hoi.ini')

edit('S3Client/Ui/UiCase/UiManage.cpp',
     b'"\xb9\xdc\xc0\xed\xd6\xf7\xb4\xb0\xbf\xda.ini"',
     b'"quan-ly-cua-so-chinh.ini"',
     'ten tep MANAGE_INI_SHEET -> quan-ly-cua-so-chinh.ini')

edit('S3Client/Ui/UiCase/UiEquip.cpp',
     b'"\xd7\xb0\xb1\xb8.ini"',
     b'"trang-bi.ini"',
     'ten tep SCHEME_INI_EQUIP -> trang-bi.ini')

edit('S3Client/Ui/UiCase/UiNewPlayer2.cpp',
     b'"\xd0\xc2\xbd\xa8\xbd\xc7\xc9\xab\xd1\xa1\xca\xf4\xd0\xd4.ini"',
     b'"tao-nhan-vat-chon-thuoc-tinh.ini"',
     'ten tep SCHEME_INI_NEWPLAYER2 -> tao-nhan-vat-chon-thuoc-tinh.ini')

edit('S3Client/Ui/UiCase/UiSelDlg.cpp',
     b'"\xb6\xe0\xcf\xee\xd1\xa1\xd4\xf1\xbd\xe7\xc3\xe6.ini"',
     b'"chon-nhieu-muc.ini"',
     'ten tep SCHEME_INI_SELDLG -> chon-nhieu-muc.ini')

edit('S3Client/Ui/UiCase/UiUpdatePatch.cpp',
     b'"\xcf\xc2\xd4\xd8\xb2\xb9\xb6\xa4.ini"',
     b'"tai-ban-va.ini"',
     'ten tep SCHEME_INI_UPDATAPATCH -> tai-ban-va.ini')

edit('S3Client/Ui/UiCase/UiTongManager.cpp',
     b'"\xb0\xef\xbb\xe1\xc1\xd9\xca\xb1\xce\xc4\xbc\xfe.ini"',
     b'"bang-hoi-tam.ini"',
     'ten tep TONG_DATA_TEMP_FILE -> bang-hoi-tam.ini')

edit('Core/Src/CoreUseNameDef.h',
     b'"\\\\Ui\\\\Default\\\\\xd0\xa1\xb5\xd8\xcd\xbc\xd1\xd5\xc9\xab.ini"',
     b'"\\\\Ui\\\\Default\\\\mau-ban-do-nho.ini"',
     'ten tep defLITTLE_MAP_SET_FILE -> mau-ban-do-nho.ini')

edit('S3Client/Ui/UiCase/UiFriendInterview.cpp',
     b'"\xc4\xe3\xd2\xbb\xbe\xe4\xce\xd2\xd2\xbb\xbe\xe4\xc1\xc4\xcc\xec.ini"',
     b'"chat-doi-dap.ini"',
     'ten tep SCHEME_INI -> chat-doi-dap.ini')

edit('S3Client/Ui/UiCase/UiTeamApply.cpp',
     b'"\xbc\xd3\xc8\xeb\xd7\xe9\xb6\xd3.ini"',
     b'"gia-nhap-doi.ini"',
     'ten tep SCHEME_INI -> gia-nhap-doi.ini')

edit('S3Client/Ui/UiCase/UiFastInputMsg.cpp',
     b'"\xbf\xec\xcb\xd9\xca\xe4\xc8\xeb\xcf\xfb\xcf\xa2\xbd\xe7\xc3\xe6.ini"',
     b'"nhap-nhanh-tin-nhan.ini"',
     'ten tep SCHEME_INI -> nhap-nhanh-tin-nhan.ini')

edit('S3Client/Ui/UiCase/UiTeamNew.cpp',
     b'"\xd0\xc2\xbd\xa8\xb6\xd3\xce\xe9.ini"',
     b'"tao-doi-moi.ini"',
     'ten tep SCHEME_INI -> tao-doi-moi.ini')

edit('S3Client/Ui/UiCase/UiEquip.cpp',
     b'"\xd7\xb0\xb1\xb8\xb7\xd6\xd2\xb3.ini"',
     b'"trang-bi-phan-trang.ini"',
     'ten tep EQUIP_INI_PAGE -> trang-bi-phan-trang.ini')

edit('S3Client/Ui/UiCase/UiEquip.cpp',
     b'"\xd7\xb0\xb1\xb8\xc2\xed\xb7\xd6\xd2\xb3.ini"',
     b'"trang-bi-ngua-phan-trang.ini"',
     'ten tep HORSE_INI_PAGE -> trang-bi-ngua-phan-trang.ini')

edit('S3Client/Ui/UiCase/UiManage.cpp',
     b'"\xb9\xdc\xc0\xed\xd6\xf7\xb4\xb0\xbf\xda.ini"',
     b'"quan-ly-cua-so-chinh.ini"',
     'ten tep MANAGE_INI_SHEET -> quan-ly-cua-so-chinh.ini')

edit('S3Client/Ui/UiCase/UiManage.cpp',
     b'"\xb0\xef\xc5\xc9\xb9\xdc\xc0\xed.ini"',
     b'"quan-ly-bang-phai.ini"',
     'ten tep MANAGE_INI_CLIQUE -> quan-ly-bang-phai.ini')

edit('S3Client/Ui/UiCase/UiManage.cpp',
     b'"\xb0\xef\xbb\xe1\xb9\xdc\xc0\xed.ini"',
     b'"quan-ly-bang-hoi.ini"',
     'ten tep MANAGE_INI_CONFRATERNITY -> quan-ly-bang-hoi.ini')

# g_DirSin/g_DirCos EP mang byte g_InternalDirSinCosCode thanh con tro ham roi
# GOI no — ma may x86 nhung lam du lieu, mot mieng chong sua cua ban AntiVolam.
# Tren Linux vung du lieu co bit NX nen nhay vao do la segfault; do 25/08/2026:
# MOI buoc di deu lam server sap, dung tai g_DirCos. Client dung y het ma nay
# nen cung se sap khi nhan vat di chuyen.
#
# Bang thi that va dung: g_nSinBuffer chua 65 so 1024,1019,1004,...,-1024,...
# tuc 1024*cos(2*pi*d/64). Nen chi can TRA BANG.
#
# g_IsAccrue/g_IsConquer (khac che ngu hanh) dung dung meo do; bang cua chung
# da duoc g_InitSeries() dien san.
# Neo MOT DONG: CI checkout ra CRLF nen mau neo chua \n khong bao gio khop.
for ham, bang in (('g_DirSin', 'g_nSin'), ('g_DirCos', 'g_nCos')):
    edit('Core/Src/KMath.h',
         ('    return (*(g_InternalDirSinCosFunction *)(&(g_InternalDirSinCosCode[0])))(%s, nDir, nMaxDir);' % bang).encode('latin-1'),
         ('    /* Ban goc goi ma may nhung lam du lieu; Linux co bit NX -> segfault.\n'
          '       Chep dung ban C tuong duong dat trong comment o KMath.cpp,\n'
          '       ke ca viec tra -1 khi huong ngoai khoang (ma may: 83 C8 FF). */\n'
          '    if (nDir < 0 || nDir >= nMaxDir)\n'
          '        return -1;\n'
          '    return %s[(nDir << 6) / nMaxDir];' % bang).encode('latin-1'),
         'tra bang thay vi goi ma may: %s' % ham)

for ham, bang in (('g_IsAccrue', 'g_nAccrueSeries'), ('g_IsConquer', 'g_nConquerSeries')):
    edit('Core/Src/KMath.h',
         ('    return (*(g_InternalIsAccrueConquerFunction *)(&(g_InternalIsAccrueConquerCode[0])))(%s, nSrcSeries, nDesSeries);' % bang).encode('latin-1'),
         ('    if (nSrcSeries < 0 || nSrcSeries >= series_num)\n'
          '        return 0;\n'
          '    return %s[nSrcSeries] == nDesSeries;' % bang).encode('latin-1'),
         'tra bang thay vi goi ma may: %s' % ham)

# ------------------------------------------------- tran nhip ve (CPU 100%)
# KMyApp::GameLoop gioi han LOGIC o GAME_FPS=18 nhung KHONG gioi han nhip VE:
# ve xong chi Sleep(1) roi ve tiep. Nam 2003 ve mot khung mat vai ms nen tu no
# da cham; nay ve xong con thua thoi gian -> vong lap quay het mot loi CPU.
# Do 25/08/2026 tren cung prefix Wine, cung cua so 800x62x, cung man dang nhap:
# ban ta 83% CPU, ban voz2 26%. Ho so `sample` cho thay ca hai di cung duong
# CA::Render::copy_image -> CGContextDrawImage, ban ta nhieu gap ~4 lan mau,
# tuc ve nhieu gap ~4 lan chu khong phai moi khung dat hon.
#
# Khoa [Client] FPS da co san trong config.ini tu doi nao ma nguon KHONG he doc.
# Dung lai dung no. FPS=0 giu nguyen hanh vi goc.
edit('S3Client/S3Client.cpp',
     b'#define\tGAME_FPS\t\t\t18',
     b'#define\tGAME_FPS\t\t\t18\n'
     b'\n'
     b'/* Tran nhip VE, doc tu [Client] FPS trong config.ini. 0 = khong gioi han. */\n'
     b'int\tg_nTranNhipVe = 0;',
     'khai bao tran nhip ve')

edit('S3Client/S3Client.cpp',
     b'\tIniFile.GetInteger("Client", "FullScreen", FALSE, &g_bScreen);',
     b'\tIniFile.GetInteger("Client", "FullScreen", FALSE, &g_bScreen);\n'
     b'\tIniFile.GetInteger("Client", "FPS", 0, &g_nTranNhipVe);',
     'doc [Client] FPS lam tran nhip ve')

edit('S3Client/S3Client.cpp',
     b'\t\tUiPaint(nGameFps);',
     b'\t\tif (g_nTranNhipVe <= 0)\n'
     b'\t\t{\n'
     b'\t\t\tUiPaint(nGameFps);\n'
     b'\t\t}\n'
     b'\t\telse\n'
     b'\t\t{\n'
     b'\t\t\t/* GetTickCount quay vong sau 49 ngay; hieu hai DWORD van dung. */\n'
     b'\t\t\tstatic DWORD\ts_dwVeLanTruoc = 0;\n'
     b'\t\t\tDWORD\t\tdwBayGio = GetTickCount();\n'
     b'\t\t\tif ((DWORD)(dwBayGio - s_dwVeLanTruoc) >= (DWORD)(1000 / g_nTranNhipVe))\n'
     b'\t\t\t{\n'
     b'\t\t\t\ts_dwVeLanTruoc = dwBayGio;\n'
     b'\t\t\t\tUiPaint(nGameFps);\n'
     b'\t\t\t}\n'
     b'\t\t}',
     'ap tran nhip ve trong GameLoop')

# ------------------------------------------------- do: vi sao khong co dong tac danh
# May chu da chung minh sach: nhan lenh danh (ky nang=53), qua ca ba cua ai trong
# DoSkill, va co sat thuong. Nhung nhan vat KHONG co dong tac danh nao.
#
# KNpcAI.cpp:688 goi SendCommand(do_skill,...) NGAY TRUOC SendClientCmdSkill, tuc
# lenh co vao toi DoSkill ben client. Trong DoSkill chi con mot loi thoat im lang
# truoc khi dat m_ClientDoing:
#     if (IsPlayer()) { if (!m_FightMode) return; }
# nen nghi ngo m_FightMode ben client bang 0. Nhung s2c_syncplayermin (opcode 75)
# ve toi 3345 lan, va SyncPlayerMin co dat m_FightMode tu bit 0x02.
#
# Hai dong duoi in ra hai dau: gia tri m_FightMode luc DoSkill, va nIdx ma
# SyncPlayerMin tra ve (no KHONG kiem nIdx == 0 - truot la ghi vao Npc[0]).
edit_after('Core/Src/KNpc.cpp',
     b'KNpc::DoSkill(int nX, int nY)',
     b'\tm_Hide.nTime = 0;',
     _crlf(b'\tm_Hide.nTime = 0;\n'
           b'\tif (IsPlayer())\n'
           b'\t\tg_DebugLog("[TuDong] DoSkill: che-do-chien-dau=%d dang-lam=%d chieu=%d",\n'
           b'\t\t\t(int)m_FightMode, (int)m_Doing, m_ActiveSkillID);'),
     'log che do chien dau luc tung chieu')

edit_after('Core/Src/KProtocolProcess.cpp',
     b'void KProtocolProcess::SyncPlayerMin(BYTE* pMsg)',
     b'\tint nIdx = NpcSet.SearchID(pPlaySync->ID);',
     _crlf(b'\tint nIdx = NpcSet.SearchID(pPlaySync->ID);\n'
           b'\t{\n'
           b'\t\t/* 3345 goi mot phien - chi in thua ra 1/200 de khong ngap nhat ky. */\n'
           b'\t\tstatic int s_nDem = 0;\n'
           b'\t\tif ((s_nDem++ % 200) == 0)\n'
           b'\t\t\tg_DebugLog("[TuDong] SyncPlayerMin: dwID=%u -> nIdx=%d co=%02X minh=%d",\n'
           b'\t\t\t\t(unsigned)pPlaySync->ID, nIdx, (int)pPlaySync->m_btSomeFlag,\n'
           b'\t\t\t\t(int)(nIdx == Player[CLIENT_PLAYER_INDEX].m_nIndex));\n'
           b'\t}'),
     'log SyncPlayerMin tim doi tuong va co trang thai')

# Do ngay 28/08: DoSkill KHONG HE duoc goi (0 dong nhat ky) du client CO gui goi
# len may chu (ky nang=53) - tuc SendCommand da xep lenh vao hang. Chet o khau THI
# HANH: KNpc::Activate goi ProcCommand(m_ProcessAI), ma ProcCommand BO QUA TAT CA
# khi nAI == 0. Nhieu nhanh trong KNpcAI tu dat m_ProcessAI = 0 ngay sau khi ra
# lenh. Kha nang con lai: FindSame(53) tra 0 vi danh sach vo cong ben client
# khong co chieu do -> roi vao DoStand().
# Mot dong duoi chia not hai kha nang do.
edit_after('Core/Src/KNpc.cpp',
     b'void KNpc::ProcCommand(int nAI)',
     b'\tif (nAI)',
     _crlf(b'\tif (IsPlayer() && m_Command.CmdKind == do_skill)\n'
           b'\t\tg_DebugLog("[TuDong] ProcCommand: nAI=%d chieu=%d timthay=%d vung=%d",\n'
           b'\t\t\tnAI, m_Command.Param_X, m_SkillList.FindSame(m_Command.Param_X),\n'
           b'\t\t\tm_RegionIndex);\n'
           b'\tif (nAI)'),
     'log ProcCommand co thi hanh lenh danh khong')

# DOI HANH VI - vi sao nhan vat khong co dong tac danh (do xong 28/08/2026):
#
#   [TuDong] ProcCommand: nAI=1 chieu=0 timthay=0 vung=1
#
# nAI=1 (lenh CO duoc thi hanh) nhung chieu=0. Client xep lenh do_skill voi so
# hieu chieu bang KHONG, nen FindSame(0) tra 0 va roi vao DoStand() - khong ve gi.
# May chu van thay "ky nang=53" chi vi NpcSkillCommand cua ta TU THAY 0 bang chieu
# vu khi dang cam; do la gia tri SAU khi thay, khong phai cai client gui.
#
# Goc: KPlayer::SetLeftSkill BO IM LANG khi chieu chua co trong danh sach vo cong
#     if (m_SkillList.GetCurrentLevel(nSkillID) <= 0) return;
# ma SetDefaultImmedSkill() lai chay trong SyncEnd (opcode 67). Danh sach vo cong
# den bang opcode 69, va thu tu hai goi nay KHONG dam bao - do duoc: khung dang
# nhap dau tien chua 159,162,73,74,68,67 con 69 nam o khung khac. Luc 67 chay thi
# danh sach con rong -> gan chieu that bai -> m_nLeftSkillID = 0 mai mai.
#
# Va: goi lai SetDefaultImmedSkill() ngay sau khi nap xong danh sach vo cong.
# Khong dong cham guard trong SetLeftSkill - guard do dung, chi la chay qua som.
edit_after('Core/Src/KProtocolProcess.cpp',
     b'KProtocolProcess::s2cSyncAllSkill(BYTE * pMsg)',
     b'\tint nNpcIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;',
     _crlf(b'\tint nNpcIndex = Player[CLIENT_PLAYER_INDEX].m_nIndex;\n'
           b'\tg_DebugLog("[TuDong] s2cSyncAllSkill: nNpcIndex=%d so chieu=%d",\n'
           b'\t\tnNpcIndex, nSkillCount);'),
     'log nap danh sach vo cong')

edit_after('Core/Src/KProtocolProcess.cpp',
     b'KProtocolProcess::s2cSyncAllSkill(BYTE * pMsg)',
     b'            );',
     _crlf(b'            );\n'
           b'\t\t\t/* Dat TRONG vong lap la co y: moc neo phai la MOT DONG thi moi\n'
           b'\t\t\t   song duoc ca LF (ban sao duoi may) lan CRLF (checkout tren CI).\n'
           b'\t\t\t   Goi vai lan khong hai gi.\n'
           b'\t\t\t   Gan lai chieu cho nut chuot SAU khi da co danh sach vo cong.\n'
           b'\t\t\t   SetLeftSkill bo im lang neu chieu chua co trong danh sach, ma\n'
           b'\t\t\t   SetDefaultImmedSkill() chay o SyncEnd - som hon goi nay. Khong\n'
           b'\t\t\t   goi lai thi m_nLeftSkillID bang 0 mai mai va nhan vat khong bao\n'
           b'\t\t\t   gio co dong tac danh. */\n'
           b'\t\t\tPlayer[CLIENT_PLAYER_INDEX].SetDefaultImmedSkill();'),
     'gan lai chieu chuot sau khi co danh sach vo cong',
     window=1200)

# Van chieu=0 sau khi goi lai SetDefaultImmedSkill(). Ham do chi co ba nhanh theo
# loai vu khi; tay khong (GetWeaponType tra -1) thi lay g_nHandSkill. Nen hoac
# g_nHandSkill = 0 (bang \\settings\\<GBK>.txt khong nap duoc), hoac SetLeftSkill
# bi guard tu choi vi danh sach vo cong khong co chieu do.
#
# Luu y: ten tep bang do la GBK, ma APFS tu choi ten GBK (Errno 92) nen ban tren
# dia mang ten da boc hai lan - client KHONG mo duoc, phai lay tu update01.pak.
#
# In het mot the: loai vu khi, g_nHandSkill, cap cua chieu do trong danh sach, va
# chieu dau tien co trong danh sach.
edit_after('Core/Src/KPlayer.cpp',
     b'void\tKPlayer::SetDefaultImmedSkill()',
     b'\tint nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();',
     _crlf(b'\tint nParticularType = Player[CLIENT_PLAYER_INDEX].m_ItemList.GetWeaponParticular();\n'
           b'\tg_DebugLog("[TuDong] SetDefaultImmedSkill: loai=%d rieng=%d tay-khong=%d cap=%d chieu0=%d",\n'
           b'\t\tnDetailType, nParticularType, g_nHandSkill,\n'
           b'\t\tNpc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.GetCurrentLevel(g_nHandSkill),\n'
           b'\t\tNpc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_SkillList.m_Skills[1].SkillId);'),
     'log vi sao khong gan duoc chieu vao nut chuot')

# DOI HANH VI - mo khoa 38.679 muc bi ket trong cac pak nen kieu 0x20.
#
# XPackFile::ExtractRead chi biet TYPE_NONE va TYPE_UCL (0x01000000). Ca 14 tep
# updatejx*.pak cua ban Viet hoa nen bang 0x20000000 - gap la ham tra false va
# tep KHONG NAP DUOC, khong mot dong bao loi. Hau qua thay duoc: thanh HUD trong
# tron (da chua tam bang cach xep lai thu tu pak), va nhan vat thieu dau/tay.
#
# DO DUOC 28/08/2026: 0x20 CUNG LA NRV2B, chi khac so hieu. Bang chung:
#   * \settings\skills.txt co ca hai ban - update01.pak (0x01) va slistcache.pak
#     (0x20). 24 byte dau cua HAI khoi nen GIONG HET NHAU:
#       ff536b696c6c4e616dff650950726f706572ec747909127f
#   * Giai ban 0x20 bang chinh ucl_nrv2b ra dung 673205 byte, noi dung la bang
#     skills.txt hop le.
#   * Thu 237 muc 0x20 lay ngau nhien tu 6 pak khac nhau: 237 thanh cong, 0 hong.
#     Ket qua ra SPR hop le (127 muc), .ini (12 muc), va du lieu nhi phan.
#
# Nen chi can nhan them so hieu 0x20 la doc duoc het.
edit('Engine/Src/XPackFile.cpp',
     b'\t\t    if (lCompressType == TYPE_UCL && DirectRead(pReadBuffer, uOffset, uSize))',
     _crlf(b'\t\t    /* 0x20000000 la nrv2b y het TYPE_UCL, chi khac so hieu - ban Viet\n'
           b'\t\t       hoa dung no cho toan bo updatejx*.pak. Do 28/08/2026: 237/237\n'
           b'\t\t       muc thu ngau nhien giai nen dung. */\n'
           b'\t\t    if ((lCompressType == TYPE_UCL || lCompressType == 0x20000000) &&\n'
           b'\t\t        DirectRead(pReadBuffer, uOffset, uSize))'),
     'nhan them phuong phap nen 0x20 (cung la nrv2b)')

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
print('\n=== va %d cho, bo qua %d, HONG %d ===' % (n_ok, n_skip, n_hong))
# Mau neo truot (thuong do CRLF) tung lam CI xanh trong khi Game.exe chua he
# duoc va. Hong mot cho la hong ca ban dung -> dung han.
if n_hong:
    sys.exit(1)

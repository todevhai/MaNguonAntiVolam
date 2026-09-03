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
           b'static bool DangNhapTuDongTheoCauHinh()\n'
           b'{\n'
           b'    char szTepCauHinh[] = "\\\\Ui\\\\Setting.ini";\n'
           b'    KIniFile Ini;\n'
           b'    if (!Ini.Load(szTepCauHinh))\n'
           b'        return false;\n'
           b'\n'
           b'    int nBat = 0;\n'
           b'    Ini.GetInteger("AutoLogin", "Enable", 0, &nBat);\n'
           b'    if (!nBat)\n'
           b'        return false;\n'
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
           b'        return false;\n'
           b'\n'
           b'    KUiInit::CloseWindow();\n'
           b'    KUiConnectInfo::OpenWindow(CI_MI_CONNECTING, LL_S_IN_GAME);\n'
           b'    return g_LoginLogic.DangNhapTuDongTheoCauHinh(szTaiKhoan, MatKhau) != 0;\n'
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

# ---------------------------------------------------------------------------
# Vo cong dat vao o phim tat so (1..9).
#
# Nguon 8.x chi luu duoc chieu o TAY TRAI va TAY PHAI: GOI_SET_IMMDIA_SKILL
# (CoreShell.cpp) chi hien thuc nParam 0 va 1, du chu thich ngay tren no hua
# "1..4 -> F1..F4". Khong co phong nao ben may chu chua chieu theo o so, khac han
# vat pham - vat pham di qua GOI_SWITCH_OBJECT vi may chu giu tui do.
#
# Nen giu ngay o client. Chieu trong o MAT khi thoat game; muon giu lai phai them
# duong luu xuong may chu, chua lam.
edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'void KUiPlayerBar::OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)',
     _crlf(b'/* Chieu dang nam trong tung o phim tat. 0 = o do khong giu chieu\n'
           b'   (dang trong, hoac dang giu vat pham - vat pham do may chu quan ly). */\n'
           b'static unsigned int s_uChieuTrongO[UPB_IMMEDIA_ITEM_COUNT] = {0};\n'
           b'static unsigned int s_uLoaiChieuTrongO[UPB_IMMEDIA_ITEM_COUNT] = {0};\n'
           b'\n'
           b'void KUiPlayerBar::OnObjPickedDropped(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos)'),
     'cho o phim tat nho duoc chieu')

# Chan truoc GOI_SWITCH_OBJECT: yeu cau do la duong cua VAT PHAM, may chu se
# khong hieu mot cai id chieu gui vao room_immediacy.
edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\tg_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, ',
     _crlf(b'\tif ((pPickPos && (Pick.Obj.uGenre & 0xFFFF) == CGOG_SKILL) ||\n'
           b'\t\t(pDropPos && (Drop.Obj.uGenre & 0xFFFF) == CGOG_SKILL))\n'
           b'\t{\n'
           b'\t\tif (pPickPos && Pick.Region.h >= 0 && Pick.Region.h < UPB_IMMEDIA_ITEM_COUNT)\n'
           b'\t\t{\n'
           b'\t\t\ts_uChieuTrongO[Pick.Region.h] = 0;\n'
           b'\t\t\ts_uLoaiChieuTrongO[Pick.Region.h] = 0;\n'
           b'\t\t\tm_ImmediaItem[Pick.Region.h].HoldObject(CGOG_NOTHING, 0, 0, 0);\n'
           b'\t\t}\n'
           b'\t\tif (pDropPos && Drop.Region.h >= 0 && Drop.Region.h < UPB_IMMEDIA_ITEM_COUNT)\n'
           b'\t\t{\n'
           b'\t\t\ts_uChieuTrongO[Drop.Region.h] = Drop.Obj.uId;\n'
           b'\t\t\ts_uLoaiChieuTrongO[Drop.Region.h] = Drop.Obj.uGenre;\n'
           b'\t\t\tm_ImmediaItem[Drop.Region.h].HoldObject(Drop.Obj.uGenre, Drop.Obj.uId, 0, 0);\n'
           b'\t\t}\n'
           b'\t\treturn;\n'
           b'\t}\n'
           b'\tg_pCoreShell->OperationRequest(GOI_SWITCH_OBJECT, '),
     'tha vo cong vao o phim tat thi giu o client')

# Do khoang cho dau khi vao game: gan GIO THAT vao hai moc cua client de ghep
# voi truc thoi gian cua may chu (may chu da co [do] hh:mm:ss.mmm).
edit('Core/Src/KNpcSet.cpp',
     b'\tg_DebugLog("[Request]Insert %d at %d on %d", dwID, nIndex, SubWorld[0].m_dwCurrentTime);',
     _crlf(b'\t{\n'
           b'\t\tSYSTEMTIME st;\n'
           b'\t\tGetLocalTime(&st);\n'
           b'\t\tg_DebugLog("[Request]%02d:%02d:%02d.%03d Insert %d at %d on %d",\n'
           b'\t\t\tst.wHour, st.wMinute, st.wSecond, st.wMilliseconds,\n'
           b'\t\t\tdwID, nIndex, SubWorld[0].m_dwCurrentTime);\n'
           b'\t}'),
     'gan gio that vao log hoi NPC')




edit('S3Client/Ui/UiShell.cpp',
     b'#include "UiCase/UiChatCentre.h"',
     b'#include "UiCase/UiChatCentre.h"\r\n#include "UiCase/UiToolsControlBar.h"',
     'them UiToolsControlBar.h cho UiShell')

edit('S3Client/Ui/UiCase/UiToolsControlBar.cpp',
     b'\tUpdateData();',
     _crlf(b'\tUpdateData();\n'
           b'\t/* Ve nut PK theo trang thai MAY CHU, nhung CHI KHI GIA TRI DOI.\n'
           b'\t   Goi s2c_pksyncnormalflag ve ngay luc vao the gioi, co khi truoc ca\n'
           b'\t   luc thanh cong cu dung xong; ve mot lan luc do thi nut ket o khung\n'
           b'\t   "luyen cong" du may chu da bat chien dau. Nhung goi CheckButton MOI\n'
           b'\t   NHIP thi no SetFrame lien tuc, de len khung "dang bam" - nguoi choi\n'
           b'\t   thay icon nhay mot cai roi bi keo ve, va cu bam mat luon (OnLBtnUp\n'
           b'\t   chi phat click khi co WNDBTN_F_DOWN con nguyen).\n'
           b'\t   Nut chi co hai khung Up/Down nen chien dau va do sat trong giong nhau. */\n'
           b'\tif (g_pCoreShell)\n'
           b'\t{\n'
           b'\t\tint nCoPK = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);\n'
           b'\t\tif (nCoPK < 0 || nCoPK > 2)\n'
           b'\t\t\tnCoPK = 0;\n'
           b'\t\tif (nCoPK != s_nCoPKDaVe)\n'
           b'\t\t{\n'
           b'\t\t\ts_nCoPKDaVe = nCoPK;\n'
           b'\t\t\t/* SetFrame chu khong CheckButton: sprite nut PK co BA khung\n'
           b'\t\t\t   (0 luyen cong, 1 chien dau, 2 do sat) con CheckButton chi biet\n'
           b'\t\t\t   bat/tat va lay khung Up/Down - ma khung Down dong thoi la khung\n'
           b'\t\t\t   "dang nhan", nen icon nhay qua lai moi lan bam. */\n'
           b'\t\t\tm_PK.SetFrame(nCoPK);\n'
           b'\t\t}\n'
           b'\t}'),
     've nut PK moi nhip theo trang thai may chu')

# ---------------------------------------------------------------------------
# Nut chon che do PK tren thanh cong cu.
#
# Ban chuan co menu "Luyen cong / Chien dau / Do sat". Ben ta muc [PK] da co
# trong UiToolsControlBar.ini nhung ma KHONG khai bao nut nao, nen no khong ve
# ra va bam khong duoc.
#
# May chu giu ba co rieng (m_nNormalPKFlag, m_nExercisePKFlag, m_nEnmityPKState)
# nhung chi mo HAI lenh c2s: doi co PK thuong va do sat. Do sat con phai chon
# nguoi truoc nen khong dat vao menu nay duoc; menu chi co hai muc that su chay.



# ---------------------------------------------------------------------------
# Chon ngu hanh: bam mot lan khong an, phai bam hai lan.
#
# Vong lap goc dat gia tri roi break NGAY, nen UpdateProperty() khong bao gio
# chay cho nut vua bam; con nhanh "if (i < series_num)" thi LUON dung, nen no ve
# lai giao dien theo he CU o moi vong truoc do. Ket qua: hien thi tre dung mot
# nhip - bam Thuy thi thay Moc, bam lan hai moi ra Thuy.
edit('S3Client/Ui/UiCase/UiNewPlayer.cpp',
     b'\t\t\t\tm_Info.Attribute = i;',
     _crlf(b'\t\t\t\tm_Info.Attribute = i;\n'
           b'\t\t\t\tUiSoundPlay(UI_SI_PLAYER_ATTRIB);\n'
           b'\t\t\t\tUpdateProperty();'),
     'chon ngu hanh: cap nhat ngay cho nut vua bam')

edit('S3Client/Ui/UiCase/UiNewPlayer.cpp',
     b'\t\t\tif (i < series_num)',
     b'\t\t\tif (0)\t/* nhanh nay luon dung va ve lai theo he CU - xem tren */',
     'bo nhanh ve lai theo he cu')

# Do vi sao tha vat pham vao o phim tat lai khong an: van con cam tren tay, tuc
# DropObject tu choi hoac WndProc khong toi noi. In ra loai vat dang keo.
edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\tif ((pPickPos && (Pick.Obj.uGenre & 0xFFFF) == CGOG_SKILL) ||',
     _crlf(b'\tg_DebugLog("[o]pick=%d drop=%d gpick=%u gdrop=%u o=%d",\n'
           b'\t\tpPickPos ? 1 : 0, pDropPos ? 1 : 0,\n'
           b'\t\tpPickPos ? Pick.Obj.uGenre : 0, pDropPos ? Drop.Obj.uGenre : 0,\n'
           b'\t\tpDropPos ? Drop.Region.h : (pPickPos ? Pick.Region.h : -1));\n'
           b'\tif ((pPickPos && (Pick.Obj.uGenre & 0xFFFF) == CGOG_SKILL) ||'),
     'log loai vat keo vao o phim tat')

edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'#include "UiPlayerBar.h"',
     b'#include "UiPlayerBar.h"\r\n#include "../../../Engine/src/KDebug.h"',
     'them KDebug.h cho UiPlayerBar')

# Bam so vao o giu chieu: thi trien chieu ngay, khong dong toi chieu tay trai.
edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\t\tm_pSelf->m_ImmediaItem[nIndex].GetObject(Obj);',
     _crlf(b'\t\tif (s_uChieuTrongO[nIndex])\n'
           b'\t\t{\n'
           b'\t\t\t/* Thi trien NGAY ve phia con tro. Khong dung\n'
           b'\t\t\t   KShortcutKeyCentre::ms_MouseX: bien do chi duoc gan trong\n'
           b'\t\t\t   HandleMouseInput, tuc luc BAM chuot - bam phim so khong di qua\n'
           b'\t\t\t   do nen no giu toa do cu va chieu ra theo huong nhan vat. */\n'
           b'\t\t\tint nChuotX = 0, nChuotY = 0;\n'
           b'\t\t\tWnd_GetCursorPos(&nChuotX, &nChuotY);\n'
           b'\t\t\tg_pCoreShell->UseSkill(nChuotX, nChuotY, s_uChieuTrongO[nIndex]);\n'
           b'\t\t\treturn;\n'
           b'\t\t}\n'
           b'\t\tm_pSelf->m_ImmediaItem[nIndex].GetObject(Obj);'),
     'bam so vao o giu chieu thi thi trien ngay')

# Log NGAY TRUOC nhanh `if (nIndex > 0)`: hai log duoi nam trong nhanh do nen
# khong noi gi khi ItemSet.Add tra 0 - va do dung la truong hop dang gap.
edit_after('Core/Src/KProtocolProcess.cpp',
     b'void KProtocolProcess::s2cSyncItem(BYTE* pMsg)',
     b'\tif (nIndex > 0)',
     _crlf(b'\tg_DebugLog("[vatpham]goi genre=%d detail=%d part=%d level=%d series=%d gold=%d -> idx=%d",\n'
           b'\t\t(int)pItemSync->m_Genre, (int)pItemSync->m_Detail, (int)pItemSync->m_Particur,\n'
           b'\t\t(int)pItemSync->m_Level, (int)pItemSync->m_Series, (int)pItemSync->m_GoldId, nIndex);\n'
           b'\tif (nIndex > 0)'),
     'log ca khi ItemSet.Add that bai', 2000)

# Do vi sao vat pham may chu cap xuong khong hien trong tui: goi s2c_syncitem CO
# toi client (dem duoc o log), nhung tui van trong. Hai cho co the nuot im lang:
# ItemSet.Add tra 0 khi het o, va KItemList::Add tra 0 khi PlaceItem tu choi
# (vat pham rong 0x0 vi bang du lieu client khong khop se roi vao truong hop nay).
edit('Core/Src/KProtocolProcess.cpp',
     b'\t\tItem[nIndex].SetStackNum(pItemSync->m_StackNum);',
     _crlf(b'\t\tg_DebugLog("[vatpham]tao idx=%d genre=%d detail=%d part=%d level=%d series=%d",\n'
           b'\t\t\tnIndex, (int)pItemSync->m_Genre, (int)pItemSync->m_Detail,\n'
           b'\t\t\t(int)pItemSync->m_Particur, (int)pItemSync->m_Level, (int)pItemSync->m_Series);\n'
           b'\t\tItem[nIndex].SetStackNum(pItemSync->m_StackNum);'),
     'log vat pham nhan duoc tu may chu')

edit('Core/Src/KProtocolProcess.cpp',
     b'\t\tPlayer[CLIENT_PLAYER_INDEX].m_ItemList.Add(nIndex, pItemSync->m_btPlace, pItemSync->m_btX, pItemSync->m_btY);',
     _crlf(b'\t\tint nVaoTui = Player[CLIENT_PLAYER_INDEX].m_ItemList.Add(nIndex, pItemSync->m_btPlace, pItemSync->m_btX, pItemSync->m_btY);\n'
           b'\t\tg_DebugLog("[vatpham]dat place=%d x=%d y=%d cd=%dx%d ket qua=%d",\n'
           b'\t\t\t(int)pItemSync->m_btPlace, (int)pItemSync->m_btX, (int)pItemSync->m_btY,\n'
           b'\t\t\tItem[nIndex].GetWidth(), Item[nIndex].GetHeight(), nVaoTui);'),
     'log ket qua dat vat pham vao tui')

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

# DOI HANH VI - doi ten bang vu khi->chieu sang ASCII.
#
# Do 28/08/2026:
#   [TuDong] SetDefaultImmedSkill: loai=-1 rieng=-1 tay-khong=0 cap=0 chieu0=53
# loai=-1 la tay khong nen nhanh dung chay, chieu0=53 nen danh sach vo cong CO
# chieu 53. Nhung tay-khong=0, tuc g_nHandSkill = 0: bang vu khi->chieu KHONG
# NAP DUOC. Ma SetLeftSkill(0) bi guard tu choi -> m_nLeftSkillID = 0 -> client
# xep lenh do_skill voi chieu 0 -> FindSame(0) = 0 -> DoStand() -> khong co dong
# tac danh. May chu van ra don vi NpcSkillCommand tu thay 0 bang chieu vu khi.
#
# Vi sao khong nap duoc: ten tep la GBK, ma APFS TU CHOI ten GBK (Errno 92) nen
# ban tren dia mang ten da boc hai lan. Doi sang ASCII la duong da dung cho toan
# bo tep giao dien (client/ten-ini-tu-viet.txt) va cho chinh may chu - may chu
# doc settings/vukhi-kynang-vatly.txt.
#
# Tep di kem: client/ui-tu-viet/settings/vukhi-kynang-vatly.txt (cot DetailType,
# ParticularType, PhysicsSkillID; dong -1 0 53 cho tay khong).
edit('Core/Src/CoreUseNameDef.h',
     b'#define WEAPON_PHYSICSSKILLFILE\t\t\t"\\\\settings\\\\\xce\xe4\xc6\xf7\xce\xef\xc0\xed\xb9\xa5\xbb\xf7\xb6\xd4\xd5\xd5\xb1\xed.txt"',
     b'#define WEAPON_PHYSICSSKILLFILE\t\t\t"\\\\settings\\\\vukhi-kynang-vatly.txt"',
     'doi bang vu khi->chieu sang ten ASCII (APFS tu choi ten GBK)')

# Doi ten TEP thoi thi chua du: FindColumn tra cuu theo TEN COT, va ba ten cot
# trong nguon van la GBK trong khi tep tren dia dung tieu de ASCII. Khong khop
# thi FindColumn tra -1, GetInteger doc rong, g_nHandSkill giu nguyen 0 - va do
# duoc bang probe: 'tay-khong=0' du bang da nam dung cho. May chu da doi ca ba
# ten cot tu truoc (Core/CoreUseNameDef.h), day la ve con thieu ben client.
edit('Core/Src/CoreUseNameDef.h',
     b'#define WEAPON_PARTICULARTYPE\t\t\t"\xcf\xea\xcf\xb8\xc0\xe0\xb1\xf0"',
     b'#define WEAPON_PARTICULARTYPE\t\t\t"ParticularType"',
     'ten cot ParticularType sang ASCII')

edit('Core/Src/CoreUseNameDef.h',
     b'#define\tWEAPON_DETAILTYPE\t\t\t\t"\xbe\xdf\xcc\xe5\xc0\xe0\xb1\xf0"',
     b'#define\tWEAPON_DETAILTYPE\t\t\t\t"DetailType"',
     'ten cot DetailType sang ASCII')

edit('Core/Src/CoreUseNameDef.h',
     b'#define\tWEAPON_SKILLID\t\t\t\t\t"\xb6\xd4\xd3\xa6\xce\xef\xc0\xed\xbc\xbc\xc4\xdc\xb1\xe0\xba\xc5"',
     b'#define\tWEAPON_SKILLID\t\t\t\t\t"PhysicsSkillID"',
     'ten cot PhysicsSkillID sang ASCII')

# --------------------------------------------------- do phan giai doc tu cau hinh
# DOI HANH VI - be ngang man hinh dang la HANG SO bien dich (SCREEN_WIDTH 800),
# nen doi do phan giai phai dung lai CI. Bo giao dien cua ban chay that chi con
# ban 1024 cho khung duoi (bo 800 cua ho khong con tep do), nen muon bo cuc cua
# ho la phai chay 1024.
#
# Khoa [General] Resolution trong config.ini la cua ban 8.x - nguon ta KHONG doc
# no o dau ca. Cho doc that: 0 = 800x600 (mac dinh, giu nguyen hanh vi cu),
# 1 = 1024x768. IniFile da duoc mo san ngay tren cho nay.
# Moc neo phai la MOT DONG: ban sao duoi may la LF con checkout tren CI ra CRLF,
# mau nhieu dong khong bao gio khop ca hai.
edit('S3Client/S3Client.cpp',
     b'#include "KWin32.h"',
     b'#include "KWin32.h"\r\n#include "KDebug.h"',
     'them KDebug.h cho dong in do phan giai')

edit('S3Client/S3Client.cpp',
     b'#define\tSCREEN_WIDTH\t800',
     _crlf(b'/* Khong con la hang so: doc tu [General] Resolution trong config.ini.\n'
           b'   Dat mac dinh 800x600 de khong doi hanh vi khi thieu khoa. */\n'
           b'int\tg_nBeNgangManHinh = 800;\n'
           b'int\tg_nBeDocManHinh = 600;\n'
           b'#define\tSCREEN_WIDTH\tg_nBeNgangManHinh'),
     'be ngang man hinh thanh bien')

edit('S3Client/S3Client.cpp',
     b'#define SCREEN_HEIGHT\t600',
     b'#define SCREEN_HEIGHT\tg_nBeDocManHinh',
     'be doc man hinh thanh bien')

edit('S3Client/S3Client.cpp',
     b'\tIniFile.GetInteger("Client", "FullScreen", FALSE, &g_bScreen);',
     _crlf(b'\tIniFile.GetInteger("Client", "FullScreen", FALSE, &g_bScreen);\n'
           b'\t{\n'
           b'\t\t/* Doc ca hai cho: [Client] la muc da chac chan doc duoc (FullScreen\n'
           b'\t\t   ngay tren dung no), [General] la cho ban 8.x quen dat. */\n'
           b'\t\tint nDoPhanGiai = 0;\n'
           b'\t\tIniFile.GetInteger("General", "Resolution", 0, &nDoPhanGiai);\n'
           b'\t\tif (nDoPhanGiai != 1)\n'
           b'\t\t\tIniFile.GetInteger("Client", "Resolution", nDoPhanGiai, &nDoPhanGiai);\n'
           b'\t\tif (nDoPhanGiai == 1)\n'
           b'\t\t{\n'
           b'\t\t\tg_nBeNgangManHinh = 1024;\n'
           b'\t\t\tg_nBeDocManHinh = 768;\n'
           b'\t\t}\n'
           b'\t\tg_DebugLog("[man hinh] Resolution=%d -> %dx%d", nDoPhanGiai,\n'
           b'\t\t\tg_nBeNgangManHinh, g_nBeDocManHinh);\n'
           b'\t}'),
     'doc Resolution tu [Client] hoac [General]')

# ----------------------------------------- cua so OS theo dung do phan giai
# DOI HANH VI - cua so that van 800x600 du be ngang VE da la 1024.
#
# Do duoc 28/08/2026: dong in cua ta noi "Resolution=1 -> 1024x768" nhung ngay
# sau do engine ghi "Screen Width = 800". Hai con so nay den tu hai cho khac
# nhau: be ngang VE do S3Client truyen vao, con CUA SO THAT do
# KWin32App::InitWindow dung bang hang so rieng cua engine (WND_INIT_WIDTH 800),
# khong lien quan gi den tham so kia.
#
# Sua o KDirectDraw::Mode - noi DA nhan duoc be ngang/doc that va chay TRUOC
# Init(). Cong them vien dung bang cong thuc san co cua InitWindow (+6, +25).
edit('Engine/Src/KDDraw.cpp',
     b'\tm_dwScreenHeight = nHeight;',
     _crlf(b'\tm_dwScreenHeight = nHeight;\n'
           b'\t/* Cua so that duoc dung bang hang so WND_INIT_WIDTH tu truoc khi biet\n'
           b'\t   do phan giai. Keo lai cho khop, neu khong thi khung game van 800\n'
           b'\t   du be ngang ve da doi. Vien +6/+25 lay dung cong thuc cua\n'
           b'\t   KWin32App::InitWindow. */\n'
           b'\tif (!bFullScreen && g_GetMainHWnd())\n'
           b'\t\tSetWindowPos(g_GetMainHWnd(), NULL, 0, 0, nWidth + 6, nHeight + 25,\n'
           b'\t\t\tSWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);'),
     'keo cua so that theo do phan giai')

# ------------------------------------------- chin o mang theo thay vi ba
# DOI HANH VI - o phim tat 4..9 khong bao gio hien vi nguon 2003 CHI CO BA o.
#
# Bon cho cung rang buoc, ba trong so do la 3 va chi moi tep giao dien khai 9:
#   IMMEDIACY_ROOM_WIDTH   3   ngan do mang theo (GameDataDef.h)
#   ImmediaItem[3]             cau truc dua len giao dien (GameDataDef.h)
#   UPB_IMMEDIA_ITEM_COUNT 3   so o lop khung duoi dung (UiPlayerBar.h)
#
# MAX_IMMEDIACY_ITEM tham gia MAX_PLAYER_ITEM (tong so o do cua nhan vat), nen
# doi no lam mang m_Items dai them 6. May chu dung CHINH cong thuc do, va ban va
# ben may chu doi cung luc - hai ben van khop. Cau truc ImmediaItem chi di trong
# mot tien trinh (Core -> S3Client), khong qua mang.
#
# Ban ghi nhan vat cu se lech nen phai tao lai nhan vat sau khi doi.
edit('Core/Src/GameDataDef.h',
     b'#define\t\tIMMEDIACY_ROOM_WIDTH\t\t3',
     b'#define\t\tIMMEDIACY_ROOM_WIDTH\t\t9',
     'ngan do mang theo 3 -> 9 o')

edit('Engine/Core/GameDataDef.h',
     b'#define\t\tIMMEDIACY_ROOM_WIDTH\t\t3',
     b'#define\t\tIMMEDIACY_ROOM_WIDTH\t\t9',
     'ban sao trong Engine cung 3 -> 9')

edit('Core/Src/GameDataDef.h',
     b'\tKUiGameObject\tImmediaItem[3];',
     b'\tKUiGameObject\tImmediaItem[9];',
     'cau truc dua len giao dien 3 -> 9')

edit('Engine/Core/GameDataDef.h',
     b'\tKUiGameObject\tImmediaItem[3];',
     b'\tKUiGameObject\tImmediaItem[9];',
     'ban sao trong Engine cung 3 -> 9')

edit('S3Client/Ui/UiCase/UiPlayerBar.h',
     b'#define\tUPB_IMMEDIA_ITEM_COUNT\t3',
     b'#define\tUPB_IMMEDIA_ITEM_COUNT\t9',
     'so o lop khung duoi dung 3 -> 9')

# ------------------------------------- muoi nut chuc nang tren thanh cong cu
# DOI HANH VI - KUiToolsControlBar chi khai SAU nut (Rec, ItemEx, Mission,
# Friend, ChatRoom, Options), thieu het cac nut hay dung nhat: nhan vat, hanh
# trang, vo cong, to doi, chay, ngoi, ngua, giao dich, bang phai, PK.
#
# Engine da co san duong thi hanh cho tung cai (ShortcutKey.h khai 18 kich ban
# SCK_*), chi thieu nut de bam. Anh cua ca muoi nut deu co san trong spr.pak.
#
# Ban 8.x lam bang ClassType + xuong lop; duong do khong di duoc ben ta vi xuong
# lop rong. Khai thang thanh vien, y het sau nut da co.
edit('S3Client/Ui/UiCase/UiToolsControlBar.h',
     b'\tKWndButton\tm_Options;',
     _crlf(b'\tKWndButton\tm_Options;\n'
           b'\t/* Muoi nut them 28/08/2026. Ba nut cuoi la CONG TAC (bat/tat) chu\n'
           b'\t   khong mo cua so: chay, ngoi, cuoi ngua, giao dich, PK. */\n'
           b'\tKWndButton\tm_Status;\n'
           b'\tKWndButton\tm_Items;\n'
           b'\tKWndButton\tm_Skills;\n'
           b'\tKWndButton\tm_Team;\n'
           b'\tKWndButton\tm_Faction;\n'
           b'\tKWndButton\tm_Run;\n'
           b'\tKWndButton\tm_Sit;\n'
           b'\tKWndButton\tm_Horse;\n'
           b'\tKWndButton\tm_Exchange;\n'
           b'\tKWndButton\tm_PK;'),
     'khai muoi nut chuc nang')

edit('S3Client/Ui/UiCase/UiToolsControlBar.cpp',
     b'\t\t\tm_pSelf->m_Options.Init(&Ini, "Options");',
     _crlf(b'\t\t\tm_pSelf->m_Options.Init(&Ini, "Options");\n'
           b'\t\t\tm_pSelf->m_Status  .Init(&Ini, "Status");\n'
           b'\t\t\tm_pSelf->m_Items   .Init(&Ini, "Items");\n'
           b'\t\t\tm_pSelf->m_Skills  .Init(&Ini, "Skills");\n'
           b'\t\t\tm_pSelf->m_Team    .Init(&Ini, "Team");\n'
           b'\t\t\tm_pSelf->m_Faction .Init(&Ini, "Faction");\n'
           b'\t\t\tm_pSelf->m_Run     .Init(&Ini, "Run");\n'
           b'\t\t\tm_pSelf->m_Sit     .Init(&Ini, "Sit");\n'
           b'\t\t\tm_pSelf->m_Horse   .Init(&Ini, "Horse");\n'
           b'\t\t\tm_pSelf->m_Exchange.Init(&Ini, "Exchange");\n'
           b'\t\t\tm_pSelf->m_PK      .Init(&Ini, "PK");'),
     'dung muoi nut tu tep cau hinh')

edit('S3Client/Ui/UiCase/UiToolsControlBar.cpp',
     b'\tAddChild(&m_Options);',
     _crlf(b'\tAddChild(&m_Options);\n'
           b'\tAddChild(&m_Status);\n'
           b'\tAddChild(&m_Items);\n'
           b'\tAddChild(&m_Skills);\n'
           b'\tAddChild(&m_Team);\n'
           b'\tAddChild(&m_Faction);\n'
           b'\tAddChild(&m_Run);\n'
           b'\tAddChild(&m_Sit);\n'
           b'\tAddChild(&m_Horse);\n'
           b'\tAddChild(&m_Exchange);\n'
           b'\tAddChild(&m_PK);'),
     'gan muoi nut lam con')

edit('S3Client/Ui/UiCase/UiToolsControlBar.cpp',
     b'\t\tif (uParam == (unsigned int)(KWndWindow*)&m_Friend)',
     _crlf(b'\t\tif (uParam == (unsigned int)(KWndWindow*)&m_Status)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_STATUS);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Items)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_ITEMS);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Skills)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SKILLS);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Team)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_TEAM);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Faction)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_MAP);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Run)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_RUN);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Sit)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_SIT);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Horse)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_HORSE);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Exchange)\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_TRADE);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_PK)\n'
           b'\t\t\t/* Di DUNG duong ma phim F9 dang di: Switch([[pk]]) mo cua so\n'
           b'\t\t\t   KUiPK. Goi thang KUiPK::OpenWindow() thi F9 an ma nut khong -\n'
           b'\t\t\t   do 30/08/2026. Dung mot duong cho ca hai, giong het chin nut\n'
           b'\t\t\t   con lai o day va giong Player_PK::OnButtonClick ban goc. */\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_PK);\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_Friend)'),
     'noi muoi nut vao kich ban co san cua engine')

# ------------------------------------------- do vi sao phim tat khong an
# Phim tat khong chay mot cai nao (F1..F6, P, Tab...). Da loai: ky tu la trong
# tep (thu ban chi ASCII, van khong an), macOS chiem phim F (Fn+F3 cung khong),
# o nhap chu nuot phim (OnKeyDown tra 0 voi phim chuc nang nen di tiep),
# HandleKeyInput khong duoc goi (Wnds.cpp:387 co goi), InitScript khong chay
# (UiShell.cpp:200 co goi).
#
# Con lai hai kha nang, va chung khac nhau o mot con so: tep kich ban co nap
# duoc khong, va co bao nhieu lenh vao bang. In ra de biet.
edit('S3Client/Ui/ShortcutKey.cpp',
     b'#include "KIniFile.h"',
     b'#include "KIniFile.h"\r\n#include "KDebug.h"',
     'them KDebug.h cho hai dong in phim tat')

edit('S3Client/Ui/ShortcutKey.cpp',
     b'\treturn ms_Script.Load(pFileName);',
     _crlf(b'\t{\n'
           b'\t\tBOOL bXong = ms_Script.Load(pFileName);\n'
           b'\t\tg_DebugLog("[phim] nap \\"%s\\" -> %d", pFileName, (int)bXong);\n'
           b'\t\treturn bXong;\n'
           b'\t}'),
     'in ket qua nap kich ban phim tat')

edit('S3Client/Ui/ShortcutKey.cpp',
     b'\tKShortcutKeyCentre::AddCommand(&cs);',
     _crlf(b'\tKShortcutKeyCentre::AddCommand(&cs);\n'
           b'\t{\n'
           b'\t\tstatic int nDemLenh = 0;\n'
           b'\t\tnDemLenh++;\n'
           b'\t\tif (nDemLenh <= 3 || (nDemLenh % 40) == 0)\n'
           b'\t\t\tg_DebugLog("[phim] lenh thu %d: phim=%u lam=\\"%s\\"",\n'
           b'\t\t\t\tnDemLenh, (unsigned)cs.uKey, cs.szDo);\n'
           b'\t}'),
     'dem so lenh phim tat da dang ky')

# Bang phim da nap du 127 lenh ma bam van khong mo cua so nao. Nut that o khau
# sau: phim -> FindCommand -> ExcuteScript -> Open(...). ExcuteScript la cho hoi
# tu cua ca phim lan chuot, va no co MOT cua ai im lang o dong dau. In ra.
edit('S3Client/Ui/ShortcutKey.cpp',
     b'\tif (g_UiBase.GetStatus() != UIS_S_IDLE || !ms_Enable)',
     _crlf(b'\tg_DebugLog("[phim] chay \\"%s\\" trang-thai=%d bat=%d",\n'
           b'\t\tScriptCommand ? ScriptCommand : "(rong)",\n'
           b'\t\t(int)g_UiBase.GetStatus(), (int)ms_Enable);\n'
           b'\tif (g_UiBase.GetStatus() != UIS_S_IDLE || !ms_Enable)'),
     'in lenh nhan duoc va hai cua ai cua ExcuteScript')

# ExcuteScript KHONG duoc goi lan nao - ke ca cho chuot. Vay phim khong toi
# duoc khau tra bang, hoac tra khong ra. In ngay trong FindCommand: no la cho
# ca phim lan chuot deu di qua, va chi co MOT ban theo ma phim.
edit('S3Client/Ui/ShortcutKey.cpp',
     b'\tif (uKey == 0)',
     _crlf(b'\tg_DebugLog("[phim] tra ma=%u trong %d lenh", (unsigned)uKey, ms_nCommands);\n'
           b'\tif (uKey == 0)'),
     'in moi lan tra bang theo ma phim')

# ------------------------------------- so cot cua cay chon chieu doc tu cau hinh
# DOI HANH VI - cay chon chieu (bam vao o T/P tren thanh duoi) xep TAM chieu mot
# hang, cung nhac trong ma: KSkillList gan nLevel = nCount / 8, va ca ba cho ben
# giao dien (UpdateWndRect, PaintWindow, GetSkillAtPos) deu ngat hang theo nLevel.
#
# Ban chay that xep NAM cot. Khoa MaxBtnCountPerRow da co san trong tep cau hinh
# nhung khong ai doc. Cho doc that, roi gan lai nLevel ngay sau khi lay danh sach
# - mot vong lap, khong phai sua ca ba cho kia.
edit('S3Client/Ui/UiCase/UiSkillTree.h',
     b'\tint\t\t\t\tm_nWidthPerSkill, m_nHeightPerSkill;',
     _crlf(b'\tint\t\t\t\tm_nWidthPerSkill, m_nHeightPerSkill;\n'
           b'\tint\t\t\t\tm_nMaxPerRow;\t/* MaxBtnCountPerRow, mac dinh 5 */'),
     'khai so cot moi hang cua cay chon chieu')

edit('S3Client/Ui/UiCase/UiSkillTree.cpp',
     b'\t\tIni.GetInteger("Main", "KeyFont", 12, &m_pSelf->m_nFont);',
     _crlf(b'\t\tIni.GetInteger("Main", "MaxBtnCountPerRow", 5, &m_pSelf->m_nMaxPerRow);\n'
           b'\t\tif (m_pSelf->m_nMaxPerRow < 1)\n'
           b'\t\t\tm_pSelf->m_nMaxPerRow = 1;\n'
           b'\t\tIni.GetInteger("Main", "KeyFont", 12, &m_pSelf->m_nFont);'),
     'doc MaxBtnCountPerRow')

edit('S3Client/Ui/UiCase/UiSkillTree.cpp',
     b'\tm_nNumSkills = g_pCoreShell->GetGameData(',
     _crlf(b'\t/* Gan lai chi so hang theo so cot cua cau hinh. KSkillList chia TAM\n'
           b'\t   chieu mot hang bang hang so; ca ba cho ngat hang deu doc nLevel nen\n'
           b'\t   sua o day la du. */\n'
           b'\tif (m_nMaxPerRow < 1)\n'
           b'\t\tm_nMaxPerRow = 5;\n'
           b'\tm_nNumSkills = g_pCoreShell->GetGameData('),
     'chan so cot truoc khi lay danh sach')

edit('S3Client/Ui/UiCase/UiSkillTree.cpp',
     b'\t\tm_bLeft ? GDI_LEFT_ENABLE_SKILLS : GDI_RIGHT_ENABLE_SKILLS, (unsigned int)&m_Skills, 0);',
     _crlf(b'\t\tm_bLeft ? GDI_LEFT_ENABLE_SKILLS : GDI_RIGHT_ENABLE_SKILLS, (unsigned int)&m_Skills, 0);\n'
           b'\t{\n'
           b'\t\tfor (int nHang = 0; nHang < m_nNumSkills; nHang++)\n'
           b'\t\t\tm_Skills[nHang].nLevel = nHang / m_nMaxPerRow;\n'
           b'\t}'),
     'xep lai hang theo so cot cau hinh')

# --------------------------------- do vi sao o nhap ten nhan vat khong go duoc
# Da loai bang doc ma: WM_CHAR CO sinh ra (KWin32App goi TranslateMessage),
# Wnd_ProcessInput nhan moi thong diep, KWndEdit tu SetFocus khi bi bam, va
# KUiNewPlayer nam o WL_TOPMOST nen no la cua so active.
#
# Con lai: o ten co thuc su duoc dung dung kich thuoc khong. PtInWindow doi
# WND_S_VISIBLE va x < left + Width - Width bang 0 thi khong bao gio bat duoc
# chuot. In ra ngay sau khi Init de biet cau hinh co toi noi.
edit('S3Client/Ui/UiCase/UiNewPlayer.cpp',
     b'#include "UiNewPlayer.h"',
     b'#include "UiNewPlayer.h"\r\n#include "KDebug.h"',
     'them KDebug.h cho dong in o nhap ten')

edit('S3Client/Ui/UiCase/UiNewPlayer.cpp',
     b'\t\tm_Name  .Init(&Ini, "Name");',
     _crlf(b'\t\tm_Name  .Init(&Ini, "Name");\n'
           b'\t\t{\n'
           b'\t\t\tint nL = 0, nT = 0, nW = 0, nH = 0;\n'
           b'\t\t\tm_Name.GetPosition(&nL, &nT);\n'
           b'\t\t\tm_Name.GetSize(&nW, &nH);\n'
           b'\t\t\tg_DebugLog("[ten] o nhap: %d,%d %dx%d hien=%d",\n'
           b'\t\t\t\tnL, nT, nW, nH, (int)m_Name.IsVisible());\n'
           b'\t\t}'),
     'in vi tri va kich thuoc o nhap ten')

# ------------------------------------------- do duong di cua TIEU DIEM ban phim
# O nhap ten dung kich thuoc, dang hien, khong muc nao phu len (da do het), ma
# van khong co con tro nhap nhay - tuc no KHONG giu tieu diem. Phim go vao lai
# di thang xuong bo phim tat, thay vi bi o nhap nuot.
#
# In moi lan doi tieu diem: tu cua so nao sang cua so nao, va o nhap ten o dau.
edit('S3Client/Ui/Elem/Wnds.cpp',
     b'#include "Wnds.h"',
     b'#include "Wnds.h"\r\n#include "KDebug.h"',
     'them KDebug.h cho dong in tieu diem')

edit('S3Client/Ui/Elem/Wnds.cpp',
     b'\tif (s_WndStation.pFocusWnd != pWnd)',
     _crlf(b'\tg_DebugLog("[tieu diem] doi %p -> %p", s_WndStation.pFocusWnd, pWnd);\n'
           b'\tif (s_WndStation.pFocusWnd != pWnd)'),
     'in moi lan doi tieu diem')

# ------------------------------- SUA LOI: moi ten dai hon MOT ky tu deu bi tu choi
# KUiNewPlayer::GetInputInfo duyet tung ky tu cua ten, roi kiem "i < nLen" NGAY
# TRONG vong lap:
#
#     for (int i = 0; i < nLen;) {
#         ... i++ hoac i += 2 ...
#         if (i < nLen) { bao "ten co ky tu khong hop le"; return false; }
#     }
#
# Sau ky tu DAU TIEN, neu ten con ky tu nua thi i < nLen dung -> bao loi va thoat.
# Nghia la chi ten dai DUNG MOT ky tu moi qua duoc, ma do lai ngan hon muc toi
# thieu ba ky tu. Khong ten nao tao duoc.
#
# Cho kiem tra do le ra nam SAU vong lap, de bat truong hop vong lap break giua
# chung vi gap ky tu ngoai khoang cho phep. Vo hieu trong vong lap, dat lai sau.
# Bien dem khai TRONG for; MSVC moi khong cho dung no sau vong lap nua.
edit('S3Client/Ui/UiCase/UiNewPlayer.cpp',
     b'\tfor (int i = 0; i < nLen;)',
     _crlf(b'\tint i = 0;\n'
           b'\tfor (; i < nLen;)'),
     'dua bien dem ra ngoai vong lap')

edit('S3Client/Ui/UiCase/UiNewPlayer.cpp',
     b'\t\tif (i < nLen)',
     b'\t\tif (0)\t/* kiem tra nay da chuyen ra SAU vong lap */',
     'bo kiem tra dat nham trong vong lap duyet ten')

edit('S3Client/Ui/UiCase/UiNewPlayer.cpp',
     b'\tif (nLen >= LOGIN_ROLE_NAME_MIN_LEN && nLen <= LOGIN_ROLE_NAME_MAX_LEN)',
     _crlf(b'\t/* Vong lap tren break khi gap ky tu ngoai khoang cho phep; luc do i\n'
           b'\t   dung lai truoc cuoi chuoi. Day moi la cho kiem dung. */\n'
           b'\tif (i < nLen)\n'
           b'\t{\n'
           b'\t\tCloseWindow(false);\n'
           b'\t\tKUiConnectInfo::OpenWindow(CI_MI_INVALID_LOGIN_INPUT1, CI_NS_NEW_ROLE_WND);\n'
           b'\t\treturn false;\n'
           b'\t}\n'
           b'\tif (nLen >= LOGIN_ROLE_NAME_MIN_LEN && nLen <= LOGIN_ROLE_NAME_MAX_LEN)'),
     'dat lai kiem tra ky tu ten SAU vong lap')

# ---------------------------------------- DO: goi mo rong den client mang gi
# Tam thoi. May chu bao da gui 4 goi mo kenh, client nhan duoc ma 250 (s2c_extend),
# nhung danh sach kenh van rong. Can biet goi den mang family/id/ten gi.
edit('Kernel/Core/KProtocolProcess.cpp',
     b'\tEXTEND_HEADER* pHeader = (EXTEND_HEADER*)(pMsg + sizeof(tagExtendProtoHeader));',
     b'\tEXTEND_HEADER* pHeader = (EXTEND_HEADER*)(pMsg + sizeof(tagExtendProtoHeader));\r\n'
     b'\tg_DebugLog("[goi mo rong] family=%d id=%d", (int)pHeader->ProtocolFamily, (int)pHeader->ProtocolID);',
     'DO: in family/id cua goi mo rong')

edit('Kernel/Core/KProtocolProcess.cpp',
     b'\t\t\tl_pDataChangedNotifyFunc->NotifyChannelID(pNChann->channel, pNChann->channelid, pNChann->cost);',
     b'\t\t\tg_DebugLog("[goi mo kenh] ten=\\"%s\\" id=%d gia=%d", pNChann->channel, (int)pNChann->channelid, (int)pNChann->cost);\r\n'
     b'\t\t\tl_pDataChangedNotifyFunc->NotifyChannelID(pNChann->channel, pNChann->channelid, pNChann->cost);',
     'DO: in ten kenh nhan duoc')

# ---------------------------------------- keo vat pham / vo cong len con tro
# DOI HANH VI - khung keo tha da dung DU nhung chua bao gio duoc noi.
#
# Wnds.cpp co san cho luu vat dang cam, Wnd_GetDragObj de hoi, va
# Wnd_RenderWindows DA ve vat theo con tro neu duoc dua cho mot ham ve. Nhung
# Wnd_DragBegin - ham bat trang thai "dang cam" - CHI duoc khai trong Wnds.h,
# khong mot cho nao goi. Nen bDragging vinh vien false: nhac khong len, tha
# khong duoc, ca vat pham lan vo cong.
#
# Noi o LOP NEN (WndObjContainer) nen moi o deu duoc: tui, kho, vo cong, phim tat.
# Moi moc vao deu la MOT DONG: nguon o day dung LF con CI checkout ra CRLF, mau
# nhieu dong khong bao gio khop.

# 1. Ham ve vat dang cam theo con tro. Dat truoc KWndObjectBox::WndProc vi do la
#    cho dung DAU TIEN - de sau thi trinh dich khong thay.
#    Dung dung duong ma o vat pham ve chinh no (g_pCoreShell->DrawGameObj).
edit('S3Client/Ui/Elem/WndObjContainer.cpp',
     b'int KWndObjectBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)',
     _crlf(b'/* Ve vat dang cam theo con tro. Wnd_RenderWindows goi ham nay moi khung\n'
           b'   hinh khi co vat duoc nhac len. 32x32 vua mot o phim tat.\n'
           b'   Tra 1 de con tro chuot van duoc ve len tren. */\n'
           b'static int VeVatDangCam(int x, int y, const KUiDraggedObject& Obj, int nDropQueryResult)\n'
           b'{\n'
           b'\tif (g_pCoreShell && Obj.uGenre != CGOG_NOTHING)\n'
           b'\t\tg_pCoreShell->DrawGameObj(Obj.uGenre, Obj.uId, x - 16, y - 16, 32, 32, 0);\n'
           b'\treturn 1;\n'
           b'}\n'
           b'\n'
           b'int KWndObjectBox::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)'),
     'them ham ve vat dang cam theo con tro')

# 2. O DON - nhac len: bat trang thai cam truoc khi bao len cha.
edit('S3Client/Ui/Elem/WndObjContainer.cpp',
     b'\t\t\t\telse if (m_Object.uGenre != CGOG_NOTHING )',
     b'\t\t\t\telse if (m_Object.uGenre != CGOG_NOTHING && Wnd_DragBegin(&m_Object, VeVatDangCam))',
     'o don: bat trang thai cam khi nhac len')

# 3. O DON - tha xong thi nha vat khoi con tro.
edit('S3Client/Ui/Elem/WndObjContainer.cpp',
     b'\t\t\t\t\tDropObject(false);',
     b'\t\t\t\t\tDropObject(false);\tWnd_DragFinished();',
     'o don: nha vat sau khi tha')

# 4. O MA TRAN (tui do, bang vo cong) - tha xong thi nha.
edit('S3Client/Ui/Elem/WndObjContainer.cpp',
     b'\t\t\t\tDropObject(LOWORD(nParam), HIWORD(nParam), false);',
     b'\t\t\t\t{ DropObject(LOWORD(nParam), HIWORD(nParam), false); Wnd_DragFinished(); }',
     'o ma tran: nha vat sau khi tha')

# 5. O MA TRAN - nhac len.
edit('S3Client/Ui/Elem/WndObjContainer.cpp',
     b'\t\tPick.h = m_pObjects[nPicked].DataX;',
     _crlf(b'\t\tWnd_DragBegin(&m_pObjects[nPicked], VeVatDangCam);\n'
           b'\t\tPick.h = m_pObjects[nPicked].DataX;'),
     'o ma tran: bat trang thai cam khi nhac len')

# ---------------------------------------- bang vo cong: het diem thi nhac chieu
# DOI HANH VI. Bang vo cong khoa nhac (UiSkills.cpp:61 EnablePickPut(false)) nen
# click trai di sang nhanh WND_N_LEFT_CLICK_ITEM, ma nhanh do dung de CONG DIEM.
# Nhin ban chuan thi thay do la hai viec KHAC nhau: moi chieu co mot nut dau cong
# rieng de cong diem, con click vao chinh chieu la NHAC no len con tro.
#
# Nen: click vao chieu LUON nhac len, khong con cong diem nua. Nut dau cong chua
# co - phai them vao so do bang va vao ma, la viec rieng.
# Hai lop (chieu thuc va ky nang song) dung chung mot khuon nen doi ca hai cho.
edit('S3Client/Ui/UiCase/UiSkills.cpp',
     b'int\tKUiFightSkillSubPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)',
     _crlf(b'/* Ve chieu dang cam theo con tro. Wnd_RenderWindows goi moi khung hinh. */\n'
           b'static int VeChieuDangCam(int x, int y, const KUiDraggedObject& Obj, int nDropQueryResult)\n'
           b'{\n'
           b'\tif (g_pCoreShell && Obj.uGenre != CGOG_NOTHING)\n'
           b'\t\tg_pCoreShell->DrawGameObj(Obj.uGenre, Obj.uId, x - 16, y - 16, 32, 32, 0);\n'
           b'\treturn 1;\n'
           b'}\n'
           b'\n'
           b'int\tKUiFightSkillSubPage::WndProc(unsigned int uMsg, unsigned int uParam, int nParam)'),
     'them ham ve chieu dang cam')

# Vo hieu nhanh cong diem cu: no dung CU CLICK VAO CHIEU, tuc cuop mat cu click
# le ra de nhac chieu len. Cong diem se lam bang nut dau cong rieng nhu ban chuan.
edit_all('S3Client/Ui/UiCase/UiSkills.cpp',
     b'\tif (uMsg == WND_N_LEFT_CLICK_ITEM && uParam && m_nRemainSkillPoint)',
     b'\tif (0)\t/* cong diem chuyen sang nut dau cong rieng, xem ghi chu tren */',
     'bo cong diem khoi cu click vao chieu')

edit_all('S3Client/Ui/UiCase/UiSkills.cpp',
     b'\treturn KWndPage::WndProc(uMsg, uParam, nParam);',
     _crlf(b'\t/* Click vao chieu: NHAC len con tro de dat vao o phim tat.\n'
           b'\t   Cong diem la viec cua nut dau cong rieng, khong phai cua cu click nay. */\n'
           b'\tif (uMsg == WND_N_LEFT_CLICK_ITEM && uParam)\n'
           b'\t{\n'
           b'\t\tKUiDraggedObject* pChieu = (KUiDraggedObject*)uParam;\n'
           b'\t\tif (pChieu->uGenre != CGOG_NOTHING)\n'
           b'\t\t\tWnd_DragBegin(pChieu, VeChieuDangCam);\n'
           b'\t\treturn 0;\n'
           b'\t}\n'
           b'\treturn KWndPage::WndProc(uMsg, uParam, nParam);'),
     'het diem thi nhac chieu len con tro')

# O phim tat: nhan CA vo cong, khong chi vat pham. CGOG_NOTHING = khong loc loai.
edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\t\tm_ImmediaItem[i].SetObjectGenre(CGOG_ITEM);',
     b'\t\tm_ImmediaItem[i].SetObjectGenre(CGOG_NOTHING);\t/* nhan ca vo cong lan vat pham */',
     'o phim tat nhan ca vo cong')

# ---------------------------------------- SUA LOI: hover vat pham lam sap client
# KItem::GetDesc dung sprintf khong gioi han de ghep TEN vat pham vao hai bo dem
# qua nho tren ngan xep:
#   char sItemName[64];  sprintf(sItemName, "%s + %d", szItemName, nEnChance);
#   char TextLevel[10];  sprintf(TextLevel, "%s [Cap %d]", szItemName, ...);
# Ma szItemName dai 80 byte (KItem.h:105). Ten thuong ngan nen it lo, nhung vat
# pham co cap hoac co so lan ep thi tran ngay - hong con hem ngan xep, Wine bao
# "Unhandled exception code c0000409".
# Do 29/08/2026: F4 mo binh thuong, HOVER vao con ngua la sap.
edit('Core/Src/KItem.cpp',
     b'\t\tchar sItemName[64];',
     b'\t\tchar sItemName[160];\t/* ten dai 80 byte, cong hau to - xem ghi chu */',
     'noi bo dem ten trong mo ta vat pham')

edit_all('Core/Src/KItem.cpp',
     b'\t\t\tchar    TextLevel[10];',
     b'\t\t\tchar    TextLevel[160];\t/* phai chua ca TEN 80 byte, khong chi con so */',
     'noi bo dem ten kem cap trong mo ta vat pham')

# ---------------------------------------- xoa nhan vat: ten bi cat con BON ky tu
# SUA LOI. KLogin::DeleteRole chep ten nhan vat vao goi xoa bang
#   strncpy(NetCommand.szRoleName, (const char*)pResponse->szRoleName,
#           sizeof((const char*)pResponse->szRoleName));
# sizeof cua mot bieu thuc DA EP sang con tro la 4 - kich thuoc con tro, khong
# phai kich thuoc mang. Nen chi bon ky tu dau duoc chep; phan con lai giu nguyen
# rac tu RandMemSet o tren. May chu doc ra ten kieu "Thie" + rac va tu choi.
# Do 29/08/2026: nhat ky cong vao ghi  xoa "ThieNC...rac...": SAI MAT KHAU.
edit('S3Client/Login/Login.cpp',
     b'\t\tstrncpy(NetCommand.szRoleName, (const char*)pResponse->szRoleName, sizeof((const char*)pResponse->szRoleName));',
     b'\t\tstrncpy(NetCommand.szRoleName, (const char*)pResponse->szRoleName, sizeof(NetCommand.szRoleName));',
     'chep du ten nhan vat vao goi xoa (sizeof con tro -> sizeof mang)')

# ---------------------------------------- thanh mau/noi/the/kinh nghiem tren dinh
# DOI HANH VI - bon thanh tren dinh man hinh chua bao gio ve ra gi.
#
# KUiHeaderControlBar DA duoc mo san (UiShell.cpp goi OpenWindow khi vao game) va
# DA doc UiHeaderControlBar.ini, nhung lop chi khai HAI o chu: cap va hang. Khong
# co o nao cho mau, noi luc, the luc, kinh nghiem - nen thanh tren dinh trong tron.
#
# Ban 8.x lam viec nay bang cach khai ClassType=Player_Life trong ini roi tra mot
# xuong lop. Duong do KHONG di duoc ben ta: xuong lop (KComClassFactory) co ton tai
# nhung KHONG lop nao dang ky vao, nen CreateComObject tra NULL voi moi ten.
#
# Duong ngan hon va dung kieu nguon 2003: khai thang bon KWndImagePart lam thanh
# vien, y het cach KUiPlayerBar khai cac nut cua no. KWndImagePart::SetPart(hien,
# day) ve mot phan anh theo ti le - dung thu can. PartType quyet dinh chieu voi:
# 0 = trai sang phai, 1 = phai sang trai (lay tu bo giao dien ban chay that).
#
# Bon anh .spr lay tu \Spr\Ui3\<GBK>\ - deu co san trong spr.pak/ui.pak/uik.pak.
edit('S3Client/Ui/UiCase/UiHeaderControlBar.cpp',
     b'#include "../../../core/src/coreshell.h"',
     b'#include "../../../core/src/gamedatadef.h"\r\n#include "../../../core/src/coreshell.h"',
     'keo gamedatadef.h cho KUiPlayerRuntimeInfo')

edit('S3Client/Ui/UiCase/UiHeaderControlBar.h',
     b'#include "../Elem/WndButton.h"',
     b'#include "../Elem/WndButton.h"\r\n#include "../Elem/WndImagePart.h"\r\n#include "../Elem/WndText.h"',
     'them header cho thanh ve theo ti le')

edit('S3Client/Ui/UiCase/UiHeaderControlBar.h',
     b'\tKWndText80\tm_RankWorldText;',
     _crlf(b'\tKWndText80\tm_RankWorldText;\n'
           b'\t/* Bon thanh: anh ve theo ti le + o chu hien so. Thu tu tren thanh\n'
           b'\t   la the luc, sinh luc, noi luc, kinh nghiem - trai sang phai. */\n'
           b'\tKWndImagePart\tm_Stamina;\n'
           b'\tKWndImagePart\tm_Life;\n'
           b'\tKWndImagePart\tm_Mana;\n'
           b'\tKWndImagePart\tm_Exp;\n'
           b'\tKWndText32\tm_StaminaText;\n'
           b'\tKWndText32\tm_LifeText;\n'
           b'\tKWndText32\tm_ManaText;\n'
           b'\tKWndText32\tm_ExpText;'),
     'khai bon thanh va bon o chu')

edit('S3Client/Ui/UiCase/UiHeaderControlBar.cpp',
     b'\t\t\tm_pSelf->m_RankWorldText.Init(&Ini,"RankWorldText");',
     _crlf(b'\t\t\tm_pSelf->m_RankWorldText.Init(&Ini,"RankWorldText");\n'
           b'\t\t\tm_pSelf->m_Stamina.Init(&Ini, "Stamina");\n'
           b'\t\t\tm_pSelf->m_Life   .Init(&Ini, "Life");\n'
           b'\t\t\tm_pSelf->m_Mana   .Init(&Ini, "Mana");\n'
           b'\t\t\tm_pSelf->m_Exp    .Init(&Ini, "Exp");\n'
           b'\t\t\tm_pSelf->m_StaminaText.Init(&Ini, "StaminaText");\n'
           b'\t\t\tm_pSelf->m_LifeText   .Init(&Ini, "LifeText");\n'
           b'\t\t\tm_pSelf->m_ManaText   .Init(&Ini, "ManaText");\n'
           b'\t\t\tm_pSelf->m_ExpText    .Init(&Ini, "ExpText");\n'
           b'\t\t\t/* Khong gan con thi cua so co ton tai cung khong ai ve.\n'
           b'\t\t\t   Hai o chu cu cung chua tung duoc gan - gan luon o day. */\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_LevelText);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_RankWorldText);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_Stamina);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_Life);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_Mana);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_Exp);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_StaminaText);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_LifeText);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_ManaText);\n'
           b'\t\t\tm_pSelf->AddChild(&m_pSelf->m_ExpText);'),
     'dung bon thanh va gan chung lam con')

edit('S3Client/Ui/UiCase/UiHeaderControlBar.cpp',
     b'void KUiHeaderControlBar::Breathe()',
     _crlf(b'/* Cap nhat bon thanh moi nhip tho. GDI_PLAYER_RT_INFO la duong ma\n'
           b'   KUiPlayerBar da dung san de lay mau va noi luc, dung lai o day.\n'
           b'   Kinh nghiem tinh theo phan da di trong cap HIEN TAI, khong phai\n'
           b'   tong tich luy - nExperienceFull la moc cua cap truoc. */\n'
           b'static void DatThanh(KWndImagePart& Thanh, KWndText32& Chu, int nHienTai, int nDay)\n'
           b'{\n'
           b'\tif (nDay <= 0)\n'
           b'\t\tnDay = 1;\n'
           b'\tif (nHienTai < 0)\n'
           b'\t\tnHienTai = 0;\n'
           b'\tif (nHienTai > nDay)\n'
           b'\t\tnHienTai = nDay;\n'
           b'\tThanh.SetPart(nHienTai, nDay);\n'
           b'\tchar szSo[48];\n'
           b'\tsprintf(szSo, "%d/%d", nHienTai, nDay);\n'
           b'\tChu.SetText(szSo);\n'
           b'}\n'
           b'\n'
           b'void KUiHeaderControlBar::Breathe()'),
     'ham dat mot thanh theo ti le')

edit('S3Client/Ui/UiCase/UiHeaderControlBar.cpp',
     b'\tUpdateData();',
     _crlf(b'\tUpdateData();\n'
           b'\tKUiPlayerRuntimeInfo Info;\n'
           b'\tmemset(&Info, 0, sizeof(Info));\n'
           b'\tg_pCoreShell->GetGameData(GDI_PLAYER_RT_INFO, (int)&Info, 0);\n'
           b'\tDatThanh(m_Stamina, m_StaminaText, Info.nStamina, Info.nStaminaFull);\n'
           b'\tDatThanh(m_Life,    m_LifeText,    Info.nLife,    Info.nLifeFull);\n'
           b'\tDatThanh(m_Mana,    m_ManaText,    Info.nMana,    Info.nManaFull);\n'
           b'\tDatThanh(m_Exp,     m_ExpText,\n'
           b'\t\tInfo.nExperience - Info.nExperienceFull,\n'
           b'\t\tInfo.nCurLevelExperience - Info.nExperienceFull);'),
     'cap nhat bon thanh moi nhip tho')

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

# ---------------------------------------------------------------------------
# Nhan menu Ctrl+chuot phai va ba nut cua so PK: KHONG va gi ca.
#
# Nguon 8.x von da go san bang TCVN3 ("Luy\xd6n c\xabng", "T\xb8n g\xc9u"). Truoc
# day chung bi doi sang khong dau vi font chua ve duoc chu Viet, roi lai duoc
# them dau bang mot bang ma tu che - hai lop chong len nhau tren mot thu von da
# dung san. Font phu TCVN3 duoi day lo ca hai lop di.

# ---------------------------------------------------------------------------
# Chu Viet co dau: bang ma TCVN3 MOT BYTE, ve bang font phu.
#
# Toan bo du lieu hien thi cua ban Viet hoa (settings/, ui/, lang/vn/) deu la
# TCVN3: "Bach Ho" nam tren dia la 42 B9 63 68 20 48 E6. Engine 2003 doc byte
# > 0x80 la BYTE DAN cua o hai byte GBK, nen no nuot "B9 63" thanh mot chu Han
# va hien ra "B<han>h H". Do 31/08/2026 tren ten quai, thanh trang thai va ten
# ban do.
#
# Font chu Viet KHONG phai tu che: font.pak cua ca ban6 lan voz2 deu chua HAI bo
# moi co - bo 23940 ky tu (chu Han GBK) va bo 512 ky tu (chu Viet mot byte, chi
# so danh THANG bang ma byte). Bo 512 lau nay bi bo qua vi trung ten. Rut ca hai
# bang client/rut-font-day-du.py: bo lon vao font/, bo 512 vao font/vn/.
#
# Danh doi da biet: mot ma byte chi thuoc ve MOT bang. Ma nao co glyph trong font
# Viet thi khong con lam byte dan GBK duoc nua, nen chu Han o cac ma do se vo.
# Chap nhan - do dung la cach ban Viet hoa lam, va du lieu ta dang dung chinh la
# du lieu cua ho.
edit('Represent/iRepresent/Font/KFontData.h',
     b'\tunsigned char*\tGetCharacterData(unsigned char cFirst, unsigned char cNext) const;',
     _crlf(b'\tunsigned char*\tGetCharacterData(unsigned char cFirst, unsigned char cNext) const;\n'
           b'\t/* Font Viet mot byte: chi so danh thang bang ma byte. */\n'
           b'\tunsigned char*\tGetVietCharacterData(unsigned char cCode) const;'),
     'khai bao tra glyph chu Viet mot byte')

edit('Represent/iRepresent/Font/KFontData.cpp',
     b'unsigned char*\tKFontData::GetCharacterData(unsigned char cFirst, unsigned char cNext) const',
     _crlf(b'unsigned char*\tKFontData::GetVietCharacterData(unsigned char cCode) const\n'
           b'{\n'
           b'\tif (m_pFontData && (unsigned int)cCode < m_uCharCount && m_pdwOffs[cCode])\n'
           b'\t\treturn (m_pFontData + m_pdwOffs[cCode]);\n'
           b'\treturn NULL;\n'
           b'}\n'
           b'\n'
           b'unsigned char*\tKFontData::GetCharacterData(unsigned char cFirst, unsigned char cNext) const'),
     'tra glyph chu Viet mot byte')

edit('Represent/iRepresent/Font/KFont2.h',
     b'\tvoid\tDrawCharacter(int x, int y, unsigned char cFirst, unsigned char cNext, int nColor) const;',
     _crlf(b'\tvoid\tDrawCharacter(int x, int y, unsigned char cFirst, unsigned char cNext, int nColor) const;\n'
           b'\t/* Ve mot chu Viet TCVN3 (mot byte) tu font phu. */\n'
           b'\tvoid\tDrawVietCharacter(int x, int y, unsigned char cCode, int nColor) const;'),
     'khai bao ve chu Viet')

edit('Represent/iRepresent/Font/KFont2.h',
     b'\tKFontData\t\t\tm_Resources;',
     _crlf(b'\tKFontData\t\t\tm_ResourcesVn;\t\t/* font chu Viet TCVN3 mot byte */\n'
           b'\tKFontData\t\t\tm_Resources;'),
     'them font phu chu Viet')

# Load: nap them font Viet nam trong thu muc con "vn" canh font chinh.
edit('Represent/iRepresent/Font/KFont2.cpp',
     b'\t\t\tm_Resources.GetInfo(m_nFontWidth, m_nFontHeight);',
     _crlf(b'\t\t\tm_Resources.GetInfo(m_nFontWidth, m_nFontHeight);\n'
           b'\t\t\t/* Font chu Viet: CUNG TEN, nam trong thu muc con "vn". Thieu no thi\n'
           b'\t\t\t   chu Viet khong ve duoc nhung font chinh van chay - khong chet. */\n'
           b'\t\t\t{\n'
           b'\t\t\t\tconst char* pszTen = strrchr(pszFontFile, 0x5C);\n'
           b'\t\t\t\tif (pszTen == NULL)\n'
           b'\t\t\t\t\tpszTen = strrchr(pszFontFile, 0x2F);\n'
           b'\t\t\t\tif (pszTen)\n'
           b'\t\t\t\t{\n'
           b'\t\t\t\t\tchar szViet[260];\n'
           b'\t\t\t\t\tint nThuMuc = (int)(pszTen - pszFontFile) + 1;\n'
           b'\t\t\t\t\tif (nThuMuc + 3 + (int)strlen(pszTen + 1) < (int)sizeof(szViet))\n'
           b'\t\t\t\t\t{\n'
           b'\t\t\t\t\t\tmemcpy(szViet, pszFontFile, nThuMuc);\n'
           b'\t\t\t\t\t\tmemcpy(szViet + nThuMuc, "vn", 2);\n'
           b'\t\t\t\t\t\tszViet[nThuMuc + 2] = 0x5C;\n'
           b'\t\t\t\t\t\tstrcpy(szViet + nThuMuc + 3, pszTen + 1);\n'
           b'\t\t\t\t\t\tm_ResourcesVn.Load(szViet);\n'
           b'\t\t\t\t\t}\n'
           b'\t\t\t\t}\n'
           b'\t\t\t}'),
     'nap font chu Viet mot byte')

# OutputText: byte cao ma font Viet co glyph -> mot byte, buoc nua o.
edit('Represent/iRepresent/Font/KFont2.cpp',
     b'\t\tif (lpByte[nPos] > 0x80 && nPos + 1 < nCount)',
     _crlf(b'\t\t/* Chu Viet TCVN3 di TRUOC: mot byte, ve nua o nhu chu Latin. Chi cac\n'
           b'\t\t   ma co glyph trong font Viet moi vao day, con lai van la GBK. */\n'
           b'\t\tif (lpByte[nPos] > 0x80 && m_ResourcesVn.GetVietCharacterData(lpByte[nPos]))\n'
           b'\t\t{\n'
           b'\t\t\tDrawVietCharacter(nX + h, nY, lpByte[nPos], sColor);\n'
           b'\t\t\tnPos++;\n'
           b'\t\t\th += m_nFontHalfWidth[nHalfIndex];\n'
           b'\t\t\tnHalfIndex ^= 1;\n'
           b'\t\t}\n'
           b'\t\telse if (lpByte[nPos] > 0x80 && nPos + 1 < nCount)'),
     'chu Viet TCVN3 mot byte, buoc nua o')

edit('Represent/iRepresent/Font/KFont2.cpp',
     b'void KFont2::GetFontSize(int* pWidth, int* pHeight)',
     _crlf(b'void KFont2::DrawVietCharacter(int x, int y, unsigned char cCode, int nColor) const\n'
           b'{\n'
           b'\tif (m_pDevice && m_bLoaded)\n'
           b'\t{\n'
           b'\t\tunsigned char* pCharacterData = m_ResourcesVn.GetVietCharacterData(cCode);\n'
           b'\t\tif (pCharacterData)\n'
           b'\t\t{\n'
           b'\t\t\tif (m_nDrawBorderWithDeffColor == false)\n'
           b'\t\t\t\t((KCanvas*)m_pDevice)->DrawFont(x, y, m_nFontWidth, m_nFontHeight, nColor, 31, pCharacterData);\n'
           b'\t\t\telse\n'
           b'\t\t\t\t((KCanvas*)m_pDevice)->DrawFontWithBorder(x, y, m_nFontWidth, m_nFontHeight, nColor, 31, pCharacterData, m_nBorderColor);\n'
           b'\t\t}\n'
           b'\t}\n'
           b'}\n'
           b'\n'
           b'void KFont2::GetFontSize(int* pWidth, int* pHeight)'),
     've mot chu Viet tu font phu')

# ---------------------------------------------------------------------------
# Nut PK / phim F9 mo cua so PK: LUON mo, khong bat-tat.
#
# Ban goc lam toggle: dang hien thi dong, khong thi mo. Nhung sau khi chon mot
# muc, KUiPK::WndProc goi Hide() - doi tuong con song, chi an di. Lan bam sau
# GetIfVisible() tra NULL nen goi OpenWindow(), ma ham do chi Show() lai doi
# tuong cu; neu co WND_S_VISIBLE chua duoc xoa dung cach thi Show() khong lam gi
# va cua so dung im. Do 31/08/2026: may chu nhan du ba lenh "chon", nhung nguoi
# choi bao "menu chi hien lan dau".
#
# Dong cua so bang cach CHON mot muc la du - bo toggle thi bot han mot lop loi.
edit('S3Client/Ui/ShortcutKey.cpp',
     b'\t\t\t\tif (KUiPK::GetIfVisible())',
     b'\t\t\t\tif (false)\t/* xem ghi chu: khong bat-tat nua, luon mo */',
     'nut PK / F9 luon mo cua so, khong bat-tat')

# ---------------------------------------------------------------------------
# Cua so PK: bo nhanh TU AN khi co "viec khac".
#
# WND_M_OTHER_WORK_RESULT duoc engine phat moi khi co viec khac xay ra - ke ca
# goi tin ve sau khi nguoi choi vua chon che do. Cua so vua mo da bi an ngay,
# nhin ra ngoai la "lan dau duoc, lan sau khong". Dong cua so bang cach CHON
# mot muc la du.
#
# Neo phai la MOT dong (kho GitHub checkout ra CRLF), nen khong xoa duoc thang
# "Hide(); break;" cu; thay vao do cho case that thoat ngay, con than cu treo
# vao mot ma thong diep khong bao gio xay ra.
edit('S3Client/Ui/UiCase/UiPK.cpp',
     b'\tcase WND_M_OTHER_WORK_RESULT:',
     b'\tcase WND_M_OTHER_WORK_RESULT:\r\n\t\tbreak;\r\n\tcase 0x7FFFFFFF:\t/* khong bao gio xay ra */',
     'cua so PK khong tu an khi co viec khac')

# ---------------------------------------------------------------------------
# Do vi sao cua so PK chi hien lan dau.
#
# Da thu ba cach ma khong dut: bo toggle o nut, bo nhanh tu an khi co viec khac,
# day dem ngay khi doi co. Het duong suy luan tu ma - in thang trang thai co ra
# engine-debug.log moi lan OpenWindow duoc goi.



# Case 3 cua Switch: in gia tri co "da mo ruong" - no quyet dinh mo cua so PK
# hay lam dong tac. Neo phai la dong RIENG cua case 3 nen lay ca dong truoc.

# Bien theo doi khung da ve cua nut PK. De o pham vi TEP chu khong trong ham,
# vi nhanh bam nut phai dat lai duoc: OnLBtnUp goi SetFrame(m_nUpFrame) khi tha
# tay, keo icon ve khung "luyen cong" bat ke trang thai that. Dat lai -1 thi
# nhip ke tiep ve lai cho dung.
edit('S3Client/Ui/UiCase/UiToolsControlBar.cpp',
     b'#include "UiAutoPlay.h"',
     _crlf(b'#include "UiAutoPlay.h"\n'
           b'\n'
           b'/* Khung nut PK da ve; -1 = chua ve lan nao. Phai o DAU TEP: WndProc dung\n'
           b'   no nam truoc Breathe trong tep nay. */\n'
           b'static int s_nCoPKDaVe = -1;'),
     'bien theo doi khung nut PK o dau tep')

edit('S3Client/Ui/UiCase/UiToolsControlBar.cpp',
     b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_PK);',
     _crlf(b'\t\t{\n'
           b'\t\t\tKShortcutKeyCentre::ExcuteScript(SCK_SHORTCUT_PK);\n'
           b'\t\t\t/* Tha tay xong OnLBtnUp da SetFrame(m_nUpFrame), nen phai ve lai\n'
           b'\t\t\t   theo trang thai that o nhip ke tiep. */\n'
           b'\t\t\ts_nCoPKDaVe = -1;\n'
           b'\t\t}'),
     've lai nut PK sau khi bam')

# ---------------------------------------------------------------------------
# Do be rong dong trong Text.cpp: CHUA sua o buoc nay.
#
# Text.cpp dem chu Han la hai byte / mot o day. Chu Viet TCVN3 la MOT byte ve
# nua o, nen phep dem o day lech - anh huong cho xuong dong va can giua trong
# hop thoai, khong lam chu sai. Sua sau khi da nhin thay chu hien dung, vi hai
# khoi dem trong tep thut le khac nhau nen neo de khop nham.

# ---------------------------------------------------------------------------
# Nut PK: ve lai khung NGAY trong luc xu ly su kien, khong doi nhip sau.
#
# ini dat Down=0 nen khi nhan engine goi SetFrame(0). Dang o khung 0 (luyen
# cong) thi khong thay gi, con dang o khung 1 (chien dau) hay 2 (do sat) thi
# icon nhay ve 0 roi moi tro lai - do 31/08/2026, nguoi choi bao "bam van nhay
# mot cai". Tha tay thi OnLBtnUp lai SetFrame(m_nUpFrame) = 0, nhay lan nua.
#
# Chua bang cach ve lai ngay trong ca hai nhanh su kien, nen mat khong kip thay.
edit('S3Client/Ui/UiCase/UiToolsControlBar.cpp',
     b'\tcase WND_N_BUTTON_CLICK:',
     _crlf(b'\tcase WND_N_BUTTON_DOWN:\n'
           b'\t\t/* Engine vua SetFrame(m_nDownFrame) - ve lai ngay cho dung. */\n'
           b'\t\tif (uParam == (unsigned int)(KWndWindow*)&m_PK && g_pCoreShell)\n'
           b'\t\t{\n'
           b'\t\t\tint nKhung = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);\n'
           b'\t\t\tm_PK.SetFrame((nKhung < 0 || nKhung > 2) ? 0 : nKhung);\n'
           b'\t\t}\n'
           b'\t\tbreak;\n'
           b'\tcase WND_N_BUTTON_CLICK:'),
     've lai nut PK ngay khi nhan xuong')

edit('S3Client/Ui/UiCase/UiToolsControlBar.cpp',
     b'\t\t\ts_nCoPKDaVe = -1;',
     _crlf(b'\t\t\t/* Tha tay xong OnLBtnUp da SetFrame(m_nUpFrame) - ve lai ngay,\n'
           b'\t\t\t   va dat lai bien theo doi de nhip sau con chinh theo may chu. */\n'
           b'\t\t\tif (g_pCoreShell)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tint nKhung = g_pCoreShell->GetGameData(GDI_PK_SETTING, 0, 0);\n'
           b'\t\t\t\tm_PK.SetFrame((nKhung < 0 || nKhung > 2) ? 0 : nKhung);\n'
           b'\t\t\t}\n'
           b'\t\t\ts_nCoPKDaVe = -1;'),
     've lai nut PK ngay khi tha tay')

# ---------------------------------------------------------------------------
# Bam trai tren ban do nho -> nhan vat chay toi diem do.
#
# KUiMiniMap::WndProc chuyen NGUYEN XI su kien chuot trai xuong khong gian game
# bang Wnd_TransmitInputToGameSpace, tuc UiGame hieu do la mot diem trong THE
# GIOI nam duoi con tro - ma con tro dang o tren khung ban do goc man hinh. Nen
# lenh di (neu co) tro toi cho nam DUOI khung ban do, khong phai diem vua bam.
# Khong co buoc doi toa do diem-tren-ban-do sang toa do the gioi o dau ca.
#
# Phep doi do thi da co san trong MapScroll (keo chuot PHAI de cuon ban do):
# lay KSceneMapInfo qua GSMOI_SCENE_MAP_INFO roi
#     space = nOrigFocus + nFocusOffset + nScall * (diem - tam)
# Chi viec dung lai dung phep tinh ay cho chuot trai.
#
# GotoWhere(x, y, mode) nhan toa do VIEWPORT roi tu goi
# ViewPortCoordToSpaceCoord. Ta da co san toa do KHONG GIAN nen them mot che do
# moi: mode >= 10 nghia la "toa do da la khong gian, dung doi nua". Giu nguyen
# chu ky ham nen khong pha vo iCoreShell hay cho goi nao khac.
edit('Core/Src/CoreShell.cpp',
     b'\tif (mode < 0 || mode > 2)',
     _crlf(b'\t/* mode >= 10: toa do truyen vao DA LA toa do khong gian (ban do nho\n'
           b'\t   bam de chay toi). Tru 10 ra che do di lai nhu cu. */\n'
           b'\tbool bDaLaKhongGian = (mode >= 10);\n'
           b'\tif (bDaLaKhongGian)\n'
           b'\t\tmode -= 10;\n'
           b'\telse if (s_nSoChang > 0)\n'
           b'\t{\n'
           b'\t\t/* Lenh di TAY cua nguoi choi (click khung game -> Mouse_Action ->\n'
           b'\t\t   GotoWhere mode 0/1/2) trong luc dang chay duong tu tim: HUY duong.\n'
           b'\t\t   Phai huy O DAY, TRUOC throttle m_nSendMoveFrames ben duoi: auto-path\n'
           b'\t\t   moi nhip reset m_nSendMoveFrames ve 0 nen lenh click thuong bi\n'
           b'\t\t   throttle nuot, khong toi SendCommand - huy o SendCommand vo hieu.\n'
           b'\t\t   Lenh noi bo cua duong tu tim luon mode >= 10 nen khong vao nhanh nay. */\n'
           b'\t\ts_nSoChang = 0;\n'
           b'\t\ts_nChangDang = 0;\n'
           b'\t\ts_nSoLanKet = 0;\n'
           b'\t\ts_bTinhLai = 0;\n'
           b'\t}\n'
           b'\tif (mode < 0 || mode > 2)'),
     'GotoWhere: click khung game huy duong tu tim (truoc throttle)')

edit('Core/Src/CoreShell.cpp',
     b'\t\tg_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);',
     _crlf(b'\t\tif (!bDaLaKhongGian)\n'
           b'\t\t\tg_ScenePlace.ViewPortCoordToSpaceCoord(nX, nY, nZ);'),
     'GotoWhere: bo qua phep doi khi toa do da la khong gian')

edit('S3Client/Ui/UiCase/UiMiniMap.cpp',
     b'\tcase WM_LBUTTONDOWN:',
     _crlf(b'\tcase WM_LBUTTONDOWN:\n'
           b'\t\t{\n'
           b'\t\t\t/* Bam trong vung ve ban do -> doi sang toa do khong gian roi chay\n'
           b'\t\t\t   toi. Cung phep tinh MapScroll dung cho chuot phai. */\n'
           b'\t\t\t/* Dung DUNG hai bien ma PaintWindow dung de dat goc ve ban do,\n'
           b'\t\t\t   khong dung GetAbsolutePos - hai thu nay lech nhau. */\n'
           b'\t\t\tint nRelX = (short)LOWORD(nParam) - m_nAbsoluteLeft - m_MapPos.x;\n'
           b'\t\t\tint nRelY = (short)HIWORD(nParam) - m_nAbsoluteTop  - m_MapPos.y;\n'
           b'\t\t\tif (g_pCoreShell && nRelX >= 0 && nRelY >= 0 &&\n'
           b'\t\t\t\tnRelX < (int)m_MapSize.cx && nRelY < (int)m_MapSize.cy)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tKSceneMapInfo MapInfo;\n'
           b'\t\t\t\tif (g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_INFO, (unsigned int)&MapInfo, 0))\n'
           b'\t\t\t\t{\n'
           b'\t\t\t\t\tint nSpaceX = MapInfo.nOrigFocusH + MapInfo.nFocusOffsetH +\n'
           b'\t\t\t\t\t\tMapInfo.nScallH * (nRelX - (int)m_MapSize.cx / 2);\n'
           b'\t\t\t\t\tint nSpaceY = MapInfo.nOrigFocusV + MapInfo.nFocusOffsetV +\n'
           b'\t\t\t\t\t\tMapInfo.nScallV * (nRelY - (int)m_MapSize.cy / 2);\n'
           b'\t\t\t\t\tg_nDichSpaceX = nSpaceX;\n'
           b'\t\t\t\t\tg_nDichSpaceY = nSpaceY;\n'
           b'\t\t\t\t\tg_pCoreShell->GotoWhere(nSpaceX, nSpaceY, 10);\n'
           b'\t\t\t\t\tbreak;\n'
           b'\t\t\t\t}\n'
           b'\t\t\t}\n'
           b'\t\t}\n'
           b'\t\tWnd_TransmitInputToGameSpace(uMsg, uParam, nParam);\n'
           b'\t\tbreak;'),
     'bam trai tren ban do nho -> chay toi diem do')


# Vector chi duong tu nhan vat toi diem vua bam tren ban do, nhu ban goc.
# Nho theo toa do KHONG GIAN chu khong theo diem anh: nhan vat di thi tam ban do
# doi theo, moi khung phai chieu lai ra diem anh.
edit('S3Client/Ui/UiCase/UiMiniMap.cpp',
     b'static MINIMAP_MODE\ts_eMapMode = MINIMAP_M_NONE;',
     _crlf(b'static MINIMAP_MODE\ts_eMapMode = MINIMAP_M_NONE;\n'
           b'\n'
           b'/* Diem vua bam tren ban do, theo toa do KHONG GIAN. -1 = chua bam lan nao\n'
           b'   hoac da toi noi. Dung ve duong chi huong trong PaintWindow. */\n'
           b'int\tg_nDichSpaceX = -1;\n'
           b'int\tg_nDichSpaceY = -1;'),
     'nho diem dich tren ban do de ve vector')

edit('S3Client/Ui/UiCase/UiMiniMap.cpp',
     b'\t\t\tg_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);',
     _crlf(b'\t\t\tg_pRepresentShell->DrawPrimitives(1, &rect, RU_T_RECT, true);\n'
           b'\n'
           b'\t\t\t/* Duong chi huong tu nhan vat (tam ban do) toi diem vua bam. */\n'
           b'\t\t\tif (g_nDichSpaceX >= 0)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tKSceneMapInfo MapInfo;\n'
           b'\t\t\t\tif (g_pCoreShell->SceneMapOperation(GSMOI_SCENE_MAP_INFO, (unsigned int)&MapInfo, 0))\n'
           b'\t\t\t\t{\n'
           b'\t\t\t\t\tint nTamSpaceX = MapInfo.nOrigFocusH + MapInfo.nFocusOffsetH;\n'
           b'\t\t\t\t\tint nTamSpaceY = MapInfo.nOrigFocusV + MapInfo.nFocusOffsetV;\n'
           b'\t\t\t\t\tint nDX = g_nDichSpaceX - nTamSpaceX;\n'
           b'\t\t\t\t\tint nDY = g_nDichSpaceY - nTamSpaceY;\n'
           b'\t\t\t\t\t/* Toi noi roi thi thoi ve. */\n'
           b'\t\t\t\t\tif (nDX > -MapInfo.nScallH && nDX < MapInfo.nScallH &&\n'
           b'\t\t\t\t\t\tnDY > -MapInfo.nScallV && nDY < MapInfo.nScallV)\n'
           b'\t\t\t\t\t{\n'
           b'\t\t\t\t\t\tg_nDichSpaceX = -1;\n'
           b'\t\t\t\t\t}\n'
           b'\t\t\t\t\telse if (MapInfo.nScallH && MapInfo.nScallV)\n'
           b'\t\t\t\t\t{\n'
           b'\t\t\t\t\t\tint nTamX = nX + m_MapSize.cx / 2;\n'
           b'\t\t\t\t\t\tint nTamY = nY + m_MapSize.cy / 2;\n'
           b'\t\t\t\t\t\tint nDichX = nTamX + nDX / MapInfo.nScallH;\n'
           b'\t\t\t\t\t\tint nDichY = nTamY + nDY / MapInfo.nScallV;\n'
           b'\t\t\t\t\t\tif (nDichX >= nX && nDichY >= nY &&\n'
           b'\t\t\t\t\t\t\tnDichX < nX + (int)m_MapSize.cx && nDichY < nY + (int)m_MapSize.cy)\n'
           b'\t\t\t\t\t\t{\n'
           b'\t\t\t\t\t\t\tKRULine line;\n'
           b'\t\t\t\t\t\t\tline.Color.Color_dw = 0xffffff00;\n'
           b'\t\t\t\t\t\t\tline.oPosition.nX = nTamX;\n'
           b'\t\t\t\t\t\t\tline.oPosition.nY = nTamY;\n'
           b'\t\t\t\t\t\t\tline.oPosition.nZ = 0;\n'
           b'\t\t\t\t\t\t\tline.oEndPos.nX = nDichX;\n'
           b'\t\t\t\t\t\t\tline.oEndPos.nY = nDichY;\n'
           b'\t\t\t\t\t\t\tline.oEndPos.nZ = 0;\n'
           b'\t\t\t\t\t\t\tg_pRepresentShell->DrawPrimitives(1, &line, RU_T_LINE, true);\n'
           b'\n'
           b'\t\t\t\t\t\t\t/* La co cam o dich: mot cot doc va mot tam giac.\n'
           b'\t\t\t\t\t\t\t   Engine khong co phan tu "co" nao san (chi co\n'
           b'\t\t\t\t\t\t\t   PIC/CHARACTER/PARTNER) nen ve bang duong thang. */\n'
           b'\t\t\t\t\t\t\tKRULine co[5];\n'
           b'\t\t\t\t\t\t\tint nC;\n'
           b'\t\t\t\t\t\t\tfor (nC = 0; nC < 5; nC++)\n'
           b'\t\t\t\t\t\t\t{\n'
           b'\t\t\t\t\t\t\t\tco[nC].Color.Color_dw = 0xffff3030;\n'
           b'\t\t\t\t\t\t\t\tco[nC].oPosition.nZ = 0;\n'
           b'\t\t\t\t\t\t\t\tco[nC].oEndPos.nZ = 0;\n'
           b'\t\t\t\t\t\t\t}\n'
           b'\t\t\t\t\t\t\tco[0].oPosition.nX = nDichX;      co[0].oPosition.nY = nDichY;\n'
           b'\t\t\t\t\t\t\tco[0].oEndPos.nX   = nDichX;      co[0].oEndPos.nY   = nDichY - 9;\n'
           b'\t\t\t\t\t\t\tco[1].oPosition.nX = nDichX;      co[1].oPosition.nY = nDichY - 9;\n'
           b'\t\t\t\t\t\t\tco[1].oEndPos.nX   = nDichX + 6;  co[1].oEndPos.nY   = nDichY - 7;\n'
           b'\t\t\t\t\t\t\tco[2].oPosition.nX = nDichX + 6;  co[2].oPosition.nY = nDichY - 7;\n'
           b'\t\t\t\t\t\t\tco[2].oEndPos.nX   = nDichX;      co[2].oEndPos.nY   = nDichY - 5;\n'
           b'\t\t\t\t\t\t\tco[3].oPosition.nX = nDichX;      co[3].oPosition.nY = nDichY - 8;\n'
           b'\t\t\t\t\t\t\tco[3].oEndPos.nX   = nDichX + 5;  co[3].oEndPos.nY   = nDichY - 7;\n'
           b'\t\t\t\t\t\t\tco[4].oPosition.nX = nDichX - 2;  co[4].oPosition.nY = nDichY;\n'
           b'\t\t\t\t\t\t\tco[4].oEndPos.nX   = nDichX + 2;  co[4].oEndPos.nY   = nDichY;\n'
           b'\t\t\t\t\t\t\tg_pRepresentShell->DrawPrimitives(5, co, RU_T_LINE, true);\n'
           b'\t\t\t\t\t\t}\n'
           b'\t\t\t\t\t}\n'
           b'\t\t\t\t}\n'
           b'\t\t\t}'),
     've duong chi huong tren ban do')


# ---------------------------------------------------------------------------
# Tim duong A* cho lenh "bam ban do de chay toi".
#
# KNpcFindPath chi tranh vat can CUC BO, va chinh chu thich trong do ghi "chi
# quec mot lan": m_nFindTimes > 1 thi bo cuoc. Nen gap tuong dai la nhan vat
# dung luon. Day khong phai tim duong.
#
# Vat lieu de lam cho tu te thi da co san: SubWorld::TestBarrier(nMpsX, nMpsY)
# cho biet mot diem co di duoc khong, va o luoi logic la 32x32 diem
# (defLOGIC_CELL_WIDTH trong KSubWorld.cpp). The la du de chay A*.
#
# Hop tim duong 64x64 o (2048 diem moi chieu) quanh trung diem giua nguoi va
# dich - dich xa hon hop thi kep vao mep, di duoc toi dau hay toi day. Duong
# tim xong duoc RUT GON chi giu diem doi huong, roi gui tung chang mot.
edit('Core/Src/CoreShell.cpp',
     b'void KCoreShell::GotoWhere(int x, int y, int mode)',
     _crlf(b'/* ---- Tim duong cho lenh bam ban do (xem ghi chu o port-fixes) ---- */\n'
           b'#define defTD_O          32     /* mot o luoi = 32 diem ban do */\n'
           b'#define defTD_CANH       64     /* hop tim duong 64x64 o */\n'
           b'#define defTD_SO_O       (defTD_CANH * defTD_CANH)\n'
           b'#define defTD_MAX_CHANG  96\n'
           b'#define defTD_TOI_NOI    48     /* coi nhu toi chang khi con cach duoi 48 diem */\n'
           b'#define defTD_MAX_XET    4096   /* het luoi 64x64; 1500 cu qua thap cho duong chu Z */\n'
           b'#define defTD_NHIP_KET   60      /* bao nhieu nhip khong ngan duoc khoang cach thi coi la ket */\n'
           b'#define defTD_MAX_KET    6      /* tinh lai qua nhieu lan thi thoi, tranh lap vo han */\n'
           b'\n'
           b'static int   s_nTDGocX = 0, s_nTDGocY = 0;\n'
           b'static short s_nTDG[defTD_SO_O];\n'
           b'static short s_nTDCha[defTD_SO_O];\n'
           b'static unsigned char s_bTDDaXet[defTD_SO_O];\n'
           b'static unsigned char s_bTDNpc[defTD_SO_O];\t/* o co NPC/quai dung chan */\n'
           b'static short s_nTDLan[defTD_SO_O];\n'
           b'\n'
           b'static int s_nChangX[defTD_MAX_CHANG];\n'
           b'static int s_nChangY[defTD_MAX_CHANG];\n'
           b'static int s_nSoChang  = 0;\n'
           b'static int s_nChangDang = 0;\n'
           b'static int  s_bDoDang = 0;      /* duong chi toi duoc mot phan */\n'
           b'static int  s_nKcTruoc = 0x7fffffff;  /* khoang cach toi chang o nhip truoc */\n'
           b'static int  s_nDemKet = 0;      /* so nhip lien tiep dung yen */\n'
           b'static int  s_nSoLanKet = 0;    /* da tinh lai bao nhieu lan cho mot lenh */\n'
           b'static int  s_bTinhLai = 0;     /* lenh nay do chinh ta phat lai sau khi ket */\n'
           b'static int  s_nDichCuoiX = 0;   /* diem nguoi choi bam, de tinh tiep */\n'
           b'static int  s_nDichCuoiY = 0;\n'
           b'static int s_nChangMode = 0;\n'
           b'\n'
           b'/* Vi tri nhan vat theo diem ban do (mps). */\n'
           b'static void TD_ViTriNguoi(int* pnX, int* pnY)\n'
           b'{\n'
           b'\tint nIdx = Player[CLIENT_PLAYER_INDEX].m_nIndex;\n'
           b'\t*pnX = 0;\n'
           b'\t*pnY = 0;\n'
           b'\tif (nIdx > 0 && Npc[nIdx].m_RegionIndex >= 0)\n'
           b'\t\tNpc[nIdx].GetMpsPos(pnX, pnY);\n'
           b'}\n'
           b'\n'
           b'static int TD_DiDuoc(int nX, int nY)\n'
           b'{\n'
           b'\treturn SubWorld[0].GetBarrier(nX, nY) == 0;\n'
           b'}\n'
           b'\n'
           b'/* Tra ve so chang tim duoc, 0 neu khong co duong. */\n'
           b'static int TD_TimDuong(int nTuX, int nTuY, int nDenX, int nDenY)\n'
           b'{\n'
           b'\tint i;\n'
           b'\tstatic const int nDX8[8] = { 1, -1, 0, 0, 1, 1, -1, -1 };\n'
           b'\tstatic const int nDY8[8] = { 0, 0, 1, -1, 1, -1, 1, -1 };\n'
           b'\n'
           b'\ts_nSoChang = 0;\n'
           b'\ts_nChangDang = 0;\n'
           b'\t/* Luoi phai bao NHAN VAT, khong phai trung diem giua nguoi va dich:\n'
           b'\t   dat theo trung diem thi dich cang xa luoi cang troi ve phia dich va\n'
           b'\t   chinh nguoi choi roi ra ngoai (do duoc o_bat(35,-17)), A* bo cuoc\n'
           b'\t   ngay. Dich nam ngoai luoi thi da co duong di do dang lo. */\n'
           b'\ts_nTDGocX = nTuX - (defTD_CANH / 2) * defTD_O;\n'
           b'\ts_nTDGocY = nTuY - (defTD_CANH / 2) * defTD_O;\n'
           b'\n'
           b'\tint nBatX = (nTuX - s_nTDGocX) / defTD_O;\n'
           b'\tint nBatY = (nTuY - s_nTDGocY) / defTD_O;\n'
           b'\tint nDichX = (nDenX - s_nTDGocX) / defTD_O;\n'
           b'\tint nDichY = (nDenY - s_nTDGocY) / defTD_O;\n'
           b'\tif (nBatX < 0 || nBatY < 0 || nBatX >= defTD_CANH || nBatY >= defTD_CANH)\n'
           b'\t{\n'
           b'\t\t/* Ra khoi luoi: xay ra khi TD_ViTriNguoi khong lay duoc vi tri\n'
           b'\t\t   nhan vat (tra 0,0). Bo cuoc, GotoWhere se di thang. */\n'
           b'\t\treturn 0;\n'
           b'\t}\n'
           b'\tif (nDichX < 0) nDichX = 0;\n'
           b'\tif (nDichY < 0) nDichY = 0;\n'
           b'\tif (nDichX >= defTD_CANH) nDichX = defTD_CANH - 1;\n'
           b'\tif (nDichY >= defTD_CANH) nDichY = defTD_CANH - 1;\n'
           b'\n'
           b'\tfor (i = 0; i < defTD_SO_O; i++)\n'
           b'\t{\n'
           b'\t\ts_nTDG[i] = -1;\n'
           b'\t\ts_nTDCha[i] = -1;\n'
           b'\t\ts_bTDDaXet[i] = 0;\n'
           b'\t\ts_bTDNpc[i] = 0;\n'
           b'\t}\n'
           b'\tint nBatDau  = nBatY * defTD_CANH + nBatX;\n'
           b'\tint nKetThuc = nDichY * defTD_CANH + nDichX;\n'
           b'\ts_nTDG[nBatDau] = 0;\n'
           b'\n'
           b'\t/* Quai va NPC KHONG nam trong du lieu chuong ngai cua ban do nen A*\n'
           b'\t   khong biet ne. Nhung client giu san danh sach NPC quanh nguoi choi -\n'
           b'\t   danh dau o cua chung lai, coi nhu tuong. */\n'
           b'\tint nNpcIdx = 0;\n'
           b'\tint nMinh = Player[CLIENT_PLAYER_INDEX].m_nIndex;\n'
           b'\twhile ((nNpcIdx = NpcSet.GetNextIdx(nNpcIdx)) != 0)\n'
           b'\t{\n'
           b'\t\tif (nNpcIdx == nMinh || Npc[nNpcIdx].m_RegionIndex < 0)\n'
           b'\t\t\tcontinue;\n'
           b'\t\tint nNx = 0, nNy = 0;\n'
           b'\t\tNpc[nNpcIdx].GetMpsPos(&nNx, &nNy);\n'
           b'\t\tnNx -= s_nTDGocX;\n'
           b'\t\tnNy -= s_nTDGocY;\n'
           b'\t\tif (nNx < 0 || nNy < 0)\t/* chia so am trong C lam tron ve 0 */\n'
           b'\t\t\tcontinue;\n'
           b'\t\tint nOx = nNx / defTD_O;\n'
           b'\t\tint nOy = nNy / defTD_O;\n'
           b'\t\tif (nOx < defTD_CANH && nOy < defTD_CANH)\n'
           b'\t\t\ts_bTDNpc[nOy * defTD_CANH + nOx] = 1;\n'
           b'\t}\n'
           b'\t/* O dang dung va o dich thi khong duoc coi la tuong. */\n'
           b'\ts_bTDNpc[nBatDau] = 0;\n'
           b'\ts_bTDNpc[nKetThuc] = 0;\n'
           b'\n'
           b'\tint nSoXet = 0;\n'
           b'\twhile (nSoXet < defTD_MAX_XET)\n'
           b'\t{\n'
           b'\t\tint nTot = -1, nFTot = 0;\n'
           b'\t\tfor (i = 0; i < defTD_SO_O; i++)\n'
           b'\t\t{\n'
           b'\t\t\tif (s_nTDG[i] < 0 || s_bTDDaXet[i])\n'
           b'\t\t\t\tcontinue;\n'
           b'\t\t\tint nCx = i % defTD_CANH;\n'
           b'\t\t\tint nCy = i / defTD_CANH;\n'
           b'\t\t\tint nAx = nCx - nDichX; if (nAx < 0) nAx = -nAx;\n'
           b'\t\t\tint nAy = nCy - nDichY; if (nAy < 0) nAy = -nAy;\n'
           b'\t\t\tint nF = s_nTDG[i] + nAx + nAy;\n'
           b'\t\t\tif (nTot < 0 || nF < nFTot)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tnTot = i;\n'
           b'\t\t\t\tnFTot = nF;\n'
           b'\t\t\t}\n'
           b'\t\t}\n'
           b'\t\tif (nTot < 0 || nTot == nKetThuc)\n'
           b'\t\t\tbreak;\n'
           b'\t\ts_bTDDaXet[nTot] = 1;\n'
           b'\t\tnSoXet++;\n'
           b'\n'
           b'\t\tint nCx = nTot % defTD_CANH;\n'
           b'\t\tint nCy = nTot / defTD_CANH;\n'
           b'\t\tfor (i = 0; i < 8; i++)\n'
           b'\t\t{\n'
           b'\t\t\tint nNx = nCx + nDX8[i];\n'
           b'\t\t\tint nNy = nCy + nDY8[i];\n'
           b'\t\t\tif (nNx < 0 || nNy < 0 || nNx >= defTD_CANH || nNy >= defTD_CANH)\n'
           b'\t\t\t\tcontinue;\n'
           b'\t\t\tint nJ = nNy * defTD_CANH + nNx;\n'
           b'\t\t\tif (s_bTDDaXet[nJ])\n'
           b'\t\t\t\tcontinue;\n'
           b'\t\t\tif (s_bTDNpc[nJ])\n'
           b'\t\t\t\tcontinue;\n'
           b'\t\t\t/* Khong duoc CAT GOC: di cheo ma mot trong hai o ke bi chan\n'
           b'\t\t\t   thi that ra nhan vat khong lach qua khe do duoc. Day la cho\n'
           b'\t\t\t   lam duong hinh chu Z dut giua chung. */\n'
           b'\t\t\tif (i >= 4)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tint nKe1 = nCy * defTD_CANH + nNx;\n'
           b'\t\t\t\tint nKe2 = nNy * defTD_CANH + nCx;\n'
           b'\t\t\t\tif (s_bTDNpc[nKe1] || s_bTDNpc[nKe2])\n'
           b'\t\t\t\t\tcontinue;\n'
           b'\t\t\t\tif (!TD_DiDuoc(s_nTDGocX + nNx * defTD_O + defTD_O / 2,\n'
           b'\t\t\t\t\t\ts_nTDGocY + nCy * defTD_O + defTD_O / 2))\n'
           b'\t\t\t\t\tcontinue;\n'
           b'\t\t\t\tif (!TD_DiDuoc(s_nTDGocX + nCx * defTD_O + defTD_O / 2,\n'
           b'\t\t\t\t\t\ts_nTDGocY + nNy * defTD_O + defTD_O / 2))\n'
           b'\t\t\t\t\tcontinue;\n'
           b'\t\t\t}\n'
           b'\t\t\tif (!TD_DiDuoc(s_nTDGocX + nNx * defTD_O + defTD_O / 2,\n'
           b'\t\t\t\t\ts_nTDGocY + nNy * defTD_O + defTD_O / 2))\n'
           b'\t\t\t\tcontinue;\n'
           b'\t\t\tshort nGMoi = (short)(s_nTDG[nTot] + 1);\n'
           b'\t\t\tif (s_nTDG[nJ] < 0 || nGMoi < s_nTDG[nJ])\n'
           b'\t\t\t{\n'
           b'\t\t\t\ts_nTDG[nJ] = nGMoi;\n'
           b'\t\t\t\ts_nTDCha[nJ] = (short)nTot;\n'
           b'\t\t\t}\n'
           b'\t\t}\n'
           b'\t}\n'
           b'\n'
           b'\t/* Client chi giu du lieu vung quanh nhan vat: dich o xa gan nhu\n'
           b'\t   luon nam ngoai vung da nap va khong bao gio toi duoc trong mot\n'
           b'\t   lan tinh. Khi do di toi o DA MO gan dich nhat; toi noi thi vung\n'
           b'\t   moi duoc nap va nhip sau tinh tiep. */\n'
           b'\ts_bDoDang = 0;\n'
           b'\tif (s_nTDG[nKetThuc] < 0)\n'
           b'\t{\n'
           b'\t\tint nGanNhat = -1, nKcGan = 0;\n'
           b'\t\tfor (i = 0; i < defTD_SO_O; i++)\n'
           b'\t\t{\n'
           b'\t\t\tif (s_nTDG[i] < 0)\n'
           b'\t\t\t\tcontinue;\n'
           b'\t\t\tint nAx = (i % defTD_CANH) - nDichX; if (nAx < 0) nAx = -nAx;\n'
           b'\t\t\tint nAy = (i / defTD_CANH) - nDichY; if (nAy < 0) nAy = -nAy;\n'
           b'\t\t\tif (nGanNhat < 0 || nAx + nAy < nKcGan)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tnGanNhat = i;\n'
           b'\t\t\t\tnKcGan = nAx + nAy;\n'
           b'\t\t\t}\n'
           b'\t\t}\n'
           b'\t\tif (nGanNhat < 0 || nGanNhat == nBatDau)\n'
           b'\t\t\treturn 0;\n'
           b'\t\tnKetThuc = nGanNhat;\n'
           b'\t\ts_bDoDang = 1;\n'
           b'\t}\n'
           b'\n'
           b'\t/* Lan nguoc tu dich ve dau. */\n'
           b'\tint nSo = 0;\n'
           b'\tint nO = nKetThuc;\n'
           b'\twhile (nO >= 0 && nSo < defTD_SO_O)\n'
           b'\t{\n'
           b'\t\ts_nTDLan[nSo++] = (short)nO;\n'
           b'\t\tif (nO == nBatDau)\n'
           b'\t\t\tbreak;\n'
           b'\t\tnO = s_nTDCha[nO];\n'
           b'\t}\n'
           b'\tif (nSo <= 0)\n'
           b'\t\treturn 0;\n'
           b'\n'
           b'\t/* Dao lai va CHI GIU diem doi huong, cho so chang it di. */\n'
           b'\tint nHTruocX = 999, nHTruocY = 999;\n'           b'\tint nOTruoc = 0;\n'

           b'\tfor (i = nSo - 1; i >= 0 && s_nSoChang < defTD_MAX_CHANG; i--)\n'
           b'\t{\n'
           b'\t\tint nCx = s_nTDLan[i] % defTD_CANH;\n'
           b'\t\tint nCy = s_nTDLan[i] / defTD_CANH;\n'
           b'\t\tint nHx = 0, nHy = 0;\n'
           b'\t\tif (i < nSo - 1)\n'
           b'\t\t{\n'
           b'\t\t\tnHx = nCx - (s_nTDLan[i + 1] % defTD_CANH);\n'
           b'\t\t\tnHy = nCy - (s_nTDLan[i + 1] / defTD_CANH);\n'
           b'\t\t}\n'
           b'\t\tint nDaDi = i < nSo - 1 ? (nSo - 1 - i) : 0;\n'
           b'\t\tif (nHx != nHTruocX || nHy != nHTruocY || nDaDi - nOTruoc >= 4)\n'
           b'\t\t{\n'
           b'\t\t\ts_nChangX[s_nSoChang] = s_nTDGocX + nCx * defTD_O + defTD_O / 2;\n'
           b'\t\t\ts_nChangY[s_nSoChang] = s_nTDGocY + nCy * defTD_O + defTD_O / 2;\n'
           b'\t\t\ts_nSoChang++;\n'
           b'\t\t\tnHTruocX = nHx;\n'
           b'\t\t\tnHTruocY = nHy;\n'           b'\t\t\tnOTruoc = nDaDi;\n'

           b'\t\t}\n'
           b'\t}\n'
           b'\tif (s_nSoChang <= 0)\n'
           b'\t\treturn 0;\n'
           b'\t/* Chang cuoi lay dung diem nguoi choi bam -- chi khi duong di\n'
           b'\t   that su cham dich, khong phai duong do dang. */\n'
           b'\t/* Chang dau thuong chinh la o dang dung - di toi do thi nhan vat\n'
           b'\t   dung yen. Bo di, bat dau tu chang thu hai. */\n'
           b'\tif (s_nSoChang > 1)\n'
           b'\t{\n'
           b'\t\tint nBoX = s_nChangX[0] - nTuX; if (nBoX < 0) nBoX = -nBoX;\n'
           b'\t\tint nBoY = s_nChangY[0] - nTuY; if (nBoY < 0) nBoY = -nBoY;\n'
           b'\t\tif (nBoX < defTD_O && nBoY < defTD_O)\n'
           b'\t\t{\n'
           b'\t\t\tint nK;\n'
           b'\t\t\tfor (nK = 1; nK < s_nSoChang; nK++)\n'
           b'\t\t\t{\n'
           b'\t\t\t\ts_nChangX[nK - 1] = s_nChangX[nK];\n'
           b'\t\t\t\ts_nChangY[nK - 1] = s_nChangY[nK];\n'
           b'\t\t\t}\n'
           b'\t\t\ts_nSoChang--;\n'
           b'\t\t}\n'
           b'\t}\n'
           b'\tif (!s_bDoDang)\n'
           b'\t{\n'
           b'\t\ts_nChangX[s_nSoChang - 1] = nDenX;\n'
           b'\t\ts_nChangY[s_nSoChang - 1] = nDenY;\n'
           b'\t}\n'
           b'\treturn s_nSoChang;\n'
           b'}\n'
           b'\n'
           b'void KCoreShell::GotoWhere(int x, int y, int mode)'),
     'A* tim duong cho lenh bam ban do')

edit('Core/Src/CoreShell.cpp',
     b'\tbool bDaLaKhongGian = (mode >= 10);',
     _crlf(b'\t/* mode >= 20: toa do khong gian VA phai tim duong tranh vat can. */\n'
           b'\tif (mode >= 20)\n'
           b'\t{\n'
           b'\t\tint nTuX = 0, nTuY = 0;\n'
           b'\t\tTD_ViTriNguoi(&nTuX, &nTuY);\n'
           b'\t\ts_nChangMode = mode - 20;\n'
           b'\t\ts_nDichCuoiX = x;\n'
           b'\t\ts_nDichCuoiY = y;\n'           b'\t\t/* Lenh MOI cua nguoi choi thi xoa so lan ket; con lan tu tinh\n'
           b'\t\t   lai sau khi ket thi giu nguyen, neu khong se lap vo han. */\n'
           b'\t\ts_nDemKet = 0;\n'
           b'\t\ts_nKcTruoc = 0x7fffffff;\n'
           b'\t\tif (s_bTinhLai)\n'
           b'\t\t\ts_bTinhLai = 0;\n'
           b'\t\telse\n'
           b'\t\t\ts_nSoLanKet = 0;\n'

           b'\t\tif (TD_TimDuong(nTuX, nTuY, x, y) > 0)\n'
           b'\t\t\tGotoWhere(s_nChangX[0], s_nChangY[0], 10 + s_nChangMode);\n'
           b'\t\telse\n'
           b'\t\t\tGotoWhere(x, y, 10 + s_nChangMode);\n'
           b'\t\treturn;\n'
           b'\t}\n'
           b'\tbool bDaLaKhongGian = (mode >= 10);'),
     'GotoWhere: mode >= 20 thi tim duong truoc')

edit('Core/Src/CoreShell.cpp',
     b'\tg_ScenePlace.Breathe();',
     _crlf(b'\tg_ScenePlace.Breathe();\n'
           b'\t/* Nhip di theo duong da tim. Viet thang o day chu khong tach ra ham\n'
           b'\t   rieng: ham tu do trong Core khong co g_pCoreShell (bien do thuoc\n'
           b'\t   S3Client), con o day GotoWhere goi duoc vi cung lop. */\n'
           b'\tif (s_nSoChang > 0)\n'
           b'\t{\n'
           b'\t\tint nHX = 0, nHY = 0;\n'
           b'\t\tTD_ViTriNguoi(&nHX, &nHY);\n'
           b'\t\tint nHDx = nHX - s_nChangX[s_nChangDang]; if (nHDx < 0) nHDx = -nHDx;\n'
           b'\t\tint nHDy = nHY - s_nChangY[s_nChangDang]; if (nHDy < 0) nHDy = -nHDy;\n'
           b'\t\tif (nHDx < defTD_TOI_NOI && nHDy < defTD_TOI_NOI)\n'
           b'\t\t{\n'
           b'\t\t\ts_nChangDang++;\n'
           b'\t\t\ts_nKcTruoc = 0x7fffffff;\n'
           b'\t\t\ts_nDemKet = 0;\n'
           b'\t\t\tif (s_nChangDang >= s_nSoChang)\n'
           b'\t\t\t{\n'
           b'\t\t\t\ts_nSoChang = 0;\n'
           b'\t\t\t\ts_nSoLanKet = 0;\n'
           b'\t\t\t\t/* Duong moi di duoc mot phan: gio da dung gan hon, vung\n'
           b'\t\t\t\t   quanh do da nap, tinh tiep den dich that. */\n'
           b'\t\t\t\tif (s_bDoDang)\n'
           b'\t\t\t\t{\n'
           b'\t\t\t\t\ts_bTinhLai = 1;\n'
           b'\t\t\t\t\tGotoWhere(s_nDichCuoiX, s_nDichCuoiY, 20 + s_nChangMode);\n'
           b'\t\t\t\t}\n'
           b'\t\t\t}\n'
           b'\t\t}\n'
           b'\t\telse\n'
           b'\t\t{\n'
           b'\t\t\t/* Gui LAI lenh di moi nhip, y het nguoi choi giu chuot trai:\n'
           b'\t\t\t   Mouse_Action cung chi goi GotoWhere, va chinh GotoWhere tu\n'
           b'\t\t\t   han che tan suat bang m_nSendMoveFrames roi BO QUA IM LANG\n'
           b'\t\t\t   khi goi qua day. Truoc day chi gui mot lan luc doi chang nen\n'
           b'\t\t\t   lenh de bi bo mat, nhan vat dung giua duong. */\n'
           b'\t\t\tGotoWhere(s_nChangX[s_nChangDang], s_nChangY[s_nChangDang], 10 + s_nChangMode);\n'
           b'\n'
           b'\t\t\t/* Coi la ket khi KHOANG CACH toi chang khong ngan lai, chu khong\n'
           b'\t\t\t   phai khi nhan vat nhich it: mot nhip di bo chi duoc vai diem,\n'
           b'\t\t\t   nguong cu (<4 diem) lam ca luc dang di cung bi tinh la ket. */\n'
           b'\t\t\tint nKc = nHDx + nHDy;\n'
           b'\t\t\tif (nKc + 8 < s_nKcTruoc)\n'
           b'\t\t\t{\n'
           b'\t\t\t\ts_nKcTruoc = nKc;\n'
           b'\t\t\t\ts_nDemKet = 0;\n'
           b'\t\t\t}\n'
           b'\t\t\telse if (++s_nDemKet > defTD_NHIP_KET)\n'
           b'\t\t\t{\n'
           b'\t\t\t\ts_nDemKet = 0;\n'
           b'\t\t\t\ts_nSoLanKet++;\n'
           b'\t\t\t\ts_nSoChang = 0;\n'
           b'\t\t\t\tif (s_nSoLanKet <= defTD_MAX_KET)\n'
           b'\t\t\t\t{\n'
           b'\t\t\t\t\ts_bTinhLai = 1;\n'
           b'\t\t\t\t\tGotoWhere(s_nDichCuoiX, s_nDichCuoiY, 20 + s_nChangMode);\n'
           b'\t\t\t\t}\n'
           b'\t\t\t}\n'
           b'\t\t}\n'
           b'\t}'),
     'moi nhip gui lai lenh di, va do ket bang khoang cach')

# Bay ban: bat buoc dat Loi rao (ten sap) truoc. Mac dinh de RONG thay vi
# "Cua hang cua toi" san, va chan o GDI_PLAYER_TRADE neu ten rong.
edit('S3Client/Ui/UiCase/UiItem.cpp',
     b'\tstrcpy(m_ShopName,"C\xf6a h\xb5ng c\xf1a t\xabi");',
     b'\tm_ShopName[0] = 0;\t/* rong: bat buoc dat Loi rao truoc khi Rao ban */',
     'Loi rao mac dinh de rong')

edit('Core/Src/CoreShell.cpp',
     b'\t\t\tif (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)',
     _crlf(b'\t\t\tif (sShopName == 0 || sShopName[0] == 0)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tKSystemMessage\tsMsg;\n'
           b'\t\t\t\tsprintf(sMsg.szMessage, "Hay dat Loi rao truoc khi bay ban!");\n'
           b'\t\t\t\tsMsg.eType = SMT_SYSTEM;\t/* SMT_SYSTEM: popup noi, khong chim vao chat */\n'
           b'\t\t\t\tsMsg.byConfirmType = SMCT_NONE;\n'
           b'\t\t\t\tsMsg.byPriority = 0;\n'
           b'\t\t\t\tsMsg.byParamSize = 0;\n'
           b'\t\t\t\tCoreDataChanged(GDCNI_SYSTEM_MESSAGE, (unsigned int)&sMsg, 0);\n'
           b'\t\t\t\treturn 0;\n'
           b'\t\t\t}\n'
           b'\t\t\tif (Npc[Player[CLIENT_PLAYER_INDEX].m_nIndex].m_bRideHorse)'),
     'Rao ban: chan neu chua dat Loi rao')

edit('S3Client/Ui/UiCase/UiMiniMap.cpp',
     b'\t\t\t\t\tg_pCoreShell->GotoWhere(nSpaceX, nSpaceY, 10);',
     b'\t\t\t\t\tg_pCoreShell->GotoWhere(nSpaceX, nSpaceY, 20);',
     'bam ban do thi TIM DUONG chu khong di thang')

# Anh nen thanh trang thai (\Spr\Ui3\...\ *huyet thieu bang* .spr) DA IN SAN
# chu "Cap" va "Hang" trong chinh anh. Code ve them nhan cung noi dung de
# ngay canh -> tren man hinh doc ra "Cap Cap" va "HaMang". Bo nhan di, giu
# chu tren nen.
edit('S3Client/Ui/UiCase/UiHeaderControlBar.cpp',
     b'\tm_LevelText.SetText("C\xca\x70");',
     b'\t/* chu "Cap" da co san tren anh nen, khong ve de len */',
     'bo nhan Cap ve de len anh nen')

edit('S3Client/Ui/UiCase/UiHeaderControlBar.cpp',
     b'\tm_RankWorldText.SetText("H\xb9ng");',
     b'\t/* chu "Hang" da co san tren anh nen, khong ve de len */',
     'bo nhan Hang ve de len anh nen')

# O nhap chat co InputEdit/Face/SendBtn nhung KHONG co anh nen: nguon chua he
# co m_InputBack, nen ba nut do treo lo lung tren nen canh. Them anh nen va
# dat lam con DAU TIEN de no nam duoi cac nut.
edit('S3Client/Ui/UiCase/UiPlayerBar.h',
     b'\tKWndButton\t\tm_Face;',
     b'\tKWndImage\t\tm_InputBack;\t/* anh nen o nhap chat */\r\n\tKWndButton\t\tm_Face;',
     'khai bao anh nen o nhap chat')

edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\tm_Face    .Init(pIni, "Face");',
     b'\tm_InputBack.Init(pIni, "InputBack");\r\n\tm_Face    .Init(pIni, "Face");',
     'nap anh nen o nhap chat tu ini')

edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\tAddChild(&m_ChatBar);',
     b'\tAddChild(&m_InputBack);\r\n\tAddChild(&m_ChatBar);',
     'anh nen o nhap chat ve truoc cac nut')

# ---------------------------------------------------------------------------
# Hop thoai "tim vi tri" (nut co tren ban do nho) von la khung RONG: OnDone()
# chi dong cua so, con OnOK()/OnCheckInput() khai trong .h ma khong he co than
# trong .cpp. Header ghi tac gia "Hoang" 2014 - tinh nang nguoi Viet them sau,
# ban 2003 khong co. Nay noi no vao dung duong da lam cho lenh bam ban do:
# nhap toa do O BAN DO (32 diem moi o) -> cam co -> tu tim duong chay toi.
edit('S3Client/Ui/UiCase/UiFindPos.cpp',
     b'#include <crtdbg.h>',
     b'#include <crtdbg.h>\n#include <stdlib.h>\t/* atoi */',
     'UiFindPos: them stdlib cho atoi')

edit('S3Client/Ui/UiCase/UiFindPos.cpp',
     b'extern iCoreShell*\t\tg_pCoreShell;',
     (b'extern iCoreShell*\t\tg_pCoreShell;\n'
           b'\n'
           b'/* Diem dang cam co tren ban do nho, theo toa do KHONG GIAN.\n'
           b'   Dinh nghia trong UiMiniMap.cpp. */\n'
           b'extern int\tg_nDichSpaceX;\n'
           b'extern int\tg_nDichSpaceY;'),
     'UiFindPos: dung chung diem cam co voi ban do nho')

# CI checkout ra CRLF, nen mau NHIEU DONG khong bao gio khop - moi neo o day
# phai la MOT dong. Vi vay khong thay tron than OnDone ma: viet than cho OnOK
# (da khai trong .h nhung chua he co than) roi goi no ngay truoc OnDone.
edit('S3Client/Ui/UiCase/UiFindPos.cpp',
     b'void KUiFindPos::OnDone()',
     _crlf(b'/* Nguoi choi nhap theo O BAN DO - dung don vi hien o dong "Dang o" khi mo\n'
           b'   hop thoai - con GotoWhere nhan toa do khong gian: x/256 va y/512.\n'
           b'   mode 20 = da la toa do khong gian VA phai tim duong tranh vat can. */\n'
           b'void KUiFindPos::OnOK()\n'
           b'{\n'
           b'\tchar szX[16], szY[16];\n'
           b'\tszX[0] = 0;\n'
           b'\tszY[0] = 0;\n'
           b'\tm_X.GetText(szX, sizeof(szX), true);\n'
           b'\tm_Y.GetText(szY, sizeof(szY), true);\n'
           b'\tint nO_X = atoi(szX);\n'
           b'\tint nO_Y = atoi(szY);\n'
           b'\tif (nO_X > 0 && nO_Y > 0 && g_pCoreShell)\n'
           b'\t{\n'
           b'\t\tg_nDichSpaceX = nO_X * 256;\n'
           b'\t\tg_nDichSpaceY = nO_Y * 512;\n'
           b'\t\tg_pCoreShell->GotoWhere(g_nDichSpaceX, g_nDichSpaceY, 20);\n'
           b'\t}\n'
           b'}\n'
           b'\n'
           b'void KUiFindPos::OnDone()'),
     'UiFindPos: than cho OnOK - doc toa do, cam co, tim duong')

edit('S3Client/Ui/UiCase/UiFindPos.cpp',
     b'\t\t\tOnDone();',
     _crlf(b'\t\t\tOnOK();\n'
           b'\t\t\tOnDone();'),
     'UiFindPos: nut OK goi OnOK truoc khi dong')

# Mo hop thoai thi hien luon vi tri dang dung, de nguoi choi biet don vi ma nhap.
edit('S3Client/Ui/UiCase/UiFindPos.cpp',
     b'\t\tWnd_GameSpaceHandleInput(false);',
     (b'\t\tWnd_GameSpaceHandleInput(false);\n'
           b'\t\tKUiSceneTimeInfo Info;\n'
           b'\t\tif (g_pCoreShell &&\n'
           b'\t\t\tg_pCoreShell->SceneMapOperation(GSMOI_SCENE_TIME_INFO, (unsigned int)&Info, 0))\n'
           b'\t\t{\n'
           b'\t\t\tchar szTin[64];\n'
           b'\t\t\t/* Dung y het dong toa do duoi ban do nho:\n'
           b'\t\t\t   m_ScenePos.Set2IntText(nScenePos0 / 8, nScenePos1 / 8, ...) */\n'
           b'\t\t\tsprintf(szTin, "Dang o: %d / %d", Info.nScenePos0 / 8, Info.nScenePos1 / 8);\n'
           b'\t\t\tm_pSelf->m_InfoText.SetText(szTin);\n'
           b'\t\t}'),
     'UiFindPos: hien vi tri hien tai khi mo hop thoai')

# ---------------------------------------------------------------------------
# Nut "Tim" ngay canh dong toa do duoi ban do nho, dung cho nguoi choi quen tay
# nhu ban 8.x: bam vao do moi ra hop nhap toa do, khong phai bam nut co.
# KWndPureTextBtn la nut ve bang chu thuan, co san trong Elem ma chua ai dung.
edit('S3Client/Ui/UiCase/UiMiniMap.h',
     b'#include "../Elem/WndButton.h"',
     b'#include "../Elem/WndButton.h"\r\n#include "../Elem/WndPureTextBtn.h"',
     'UiMiniMap: nap nut chu thuan')

edit('S3Client/Ui/UiCase/UiMiniMap.h',
     b'\tKWndButton\t\tm_BtnFlag;',
     b'\tKWndButton\t\tm_BtnFlag;\r\n\tKWndPureTextBtn\tm_BtnTim;\t/* mo hop nhap toa do */',
     'UiMiniMap: khai nut Tim')

edit('S3Client/Ui/UiCase/UiMiniMap.cpp',
     b'\tAddChild(&m_BtnFlag);',
     b'\tAddChild(&m_BtnFlag);\r\n\tAddChild(&m_BtnTim);',
     'UiMiniMap: them nut Tim vao cua so')

edit('S3Client/Ui/UiCase/UiMiniMap.cpp',
     b'\tm_BtnFlag.Init(pIni, "BtnFlag");',
     _crlf(b'\tm_BtnFlag.Init(pIni, "BtnFlag");\n'
           b'\tm_BtnTim.Init(pIni, "BtnTim");\n'
           b'\tm_BtnTim.SetText("T\xecm");'),
     'UiMiniMap: nap nut Tim tu ini')

edit('S3Client/Ui/UiCase/UiMiniMap.cpp',
     b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_BtnFlag)',
     _crlf(b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_BtnTim)\n'
           b'\t\t\tKUiFindPos::OpenWindow();\n'
           b'\t\telse if (uParam == (unsigned int)(KWndWindow*)&m_BtnFlag)'),
     'UiMiniMap: bam Tim thi mo hop nhap toa do')

# ---------------------------------------------------------------------------
# Kenh chat do MAY CHU cap (NotifyChannelID -> KUiMsgCentrePad::OpenChannel),
# client khop ten do voi khoa FormatName cua tung muc [CH_*] trong ini.
#
# Tach OpenChannel thanh vo boc + OpenChannelThat: vo boc con phai dem lai kenh
# khi cua so chat chua ton tai (xem ban va ben duoi), con XuLyKenhCho thi goi
# THANG ham that de khong dem lai lan nua.
edit('S3Client/Ui/UiCase/UiMsgCentrePad.cpp',
     b'void KUiMsgCentrePad::OpenChannel(char* channelName, DWORD nChannelID, BYTE cost)',
     _crlf(b'void KUiMsgCentrePad::OpenChannel(char* channelName, DWORD nChannelID, BYTE cost)\n'
           b'{\n'
           b'\tOpenChannelThat(channelName, nChannelID, cost);\n'
           b'}\n'
           b'\n'
           b'void KUiMsgCentrePad::OpenChannelThat(char* channelName, DWORD nChannelID, BYTE cost)'),
     'tach OpenChannel de con dem duoc kenh bao som')

# May chu bao mo kenh NGAY khi vao game, con cua so chat thi mai sau moi duoc
# tao (do duoc: "cua so chat=CHUA CO" cho ca bon kenh). Nen moi thong bao deu
# roi vao hu khong va menu kenh trong rong. Nay giu lai, mo cua so xong thi xu ly.
edit('S3Client/Ui/UiCase/UiMsgCentrePad.cpp',
     b'KUiMsgCentrePad* KUiMsgCentrePad::OpenWindow()',
     _crlf(b'#define defCHAT_KENH_CHO\t16\n'
           b'struct KKenhCho\n'
           b'{\n'
           b'\tchar\tszTen[32];\n'
           b'\tDWORD\tnId;\n'
           b'\tBYTE\tcost;\n'
           b'};\n'
           b'static KKenhCho\ts_KenhCho[defCHAT_KENH_CHO];\n'
           b'static int\ts_nKenhCho = 0;\n'
           b'\n'
           b'/* Goi sau khi cua so chat da san sang: xu ly nhung kenh may chu bao\n'
           b'   truoc do. */\n'
           b'void KUiMsgCentrePad::XuLyKenhCho()\n'
           b'{\n'
           b'\tstatic int s_bDangChay = 0;\n'
           b'\tif (s_bDangChay)\n'
           b'\t\treturn;\n'
           b'\ts_bDangChay = 1;\n'
           b'\t/* GIU lai danh sach: doi canh thi ReleaseActivateChannelAll xoa\n'
           b'\t   sach kenh dang ky ma may chu khong gui lai, phai tu dang ky lai. */\n'
           b'\tint nSo = s_nKenhCho;\n'
           b'\tfor (int i = 0; i < nSo; i++)\n'
           b'\t\tOpenChannelThat(s_KenhCho[i].szTen, s_KenhCho[i].nId, s_KenhCho[i].cost);\n'
           b'\ts_bDangChay = 0;\n'
           b'}\n'
           b'\n'
           b'KUiMsgCentrePad* KUiMsgCentrePad::OpenWindow()'),
     'giu lai kenh may chu bao truoc khi cua so chat kip tao')

edit('S3Client/Ui/UiCase/UiMsgCentrePad.cpp',
     b'\t\tm_pSelf->m_Sys.Show();',
     _crlf(b'\t\tif (s_nKenhCho > 0)\n'
           b'\t\t\tm_pSelf->XuLyKenhCho();\n'
           b'\t\tm_pSelf->m_Sys.Show();'),
     'mo cua so xong thi xu ly kenh dang cho')


edit('S3Client/Ui/UiCase/UiMsgCentrePad.cpp',
     b'\tOpenChannelThat(channelName, nChannelID, cost);',
     _crlf(b'\tif (m_pSelf == NULL)\n'
           b'\t{\n'
           b'\t\tif (channelName && s_nKenhCho < defCHAT_KENH_CHO)\n'
           b'\t\t{\n'
           b'\t\t\tstrncpy(s_KenhCho[s_nKenhCho].szTen, channelName, sizeof(s_KenhCho[0].szTen) - 1);\n'
           b'\t\t\ts_KenhCho[s_nKenhCho].szTen[sizeof(s_KenhCho[0].szTen) - 1] = 0;\n'
           b'\t\t\ts_KenhCho[s_nKenhCho].nId = nChannelID;\n'
           b'\t\t\ts_KenhCho[s_nKenhCho].cost = cost;\n'
           b'\t\t\ts_nKenhCho++;\n'
           b'\t\t}\n'
           b'\t\treturn;\n'
           b'\t}\n'
           b'\tOpenChannelThat(channelName, nChannelID, cost);'),
     'chua co cua so chat thi giu kenh lai')


edit('S3Client/Ui/UiCase/UiMsgCentrePad.h',
     b'\tstatic void\t\t\t\tOpenChannel(char* channelName, DWORD nChannelID, BYTE cost);',
     b'\tstatic void\t\t\t\tOpenChannel(char* channelName, DWORD nChannelID, BYTE cost);\r\n\tstatic void\t\t\t\tOpenChannelThat(char* channelName, DWORD nChannelID, BYTE cost);\r\n\tstatic void\t\t\t\tXuLyKenhCho();',
     'khai OpenChannelThat va XuLyKenhCho')

# ---------------------------------------------------------------------------
# Menu chon kenh lay chieu cao moi dong TU CHIEU CAO ANH bieu tuong kenh
# (GetImageParam cua MenuImage). Anh nao khong doc duoc thi nHei = 0, va neu ca
# ba kenh deu vay thi nItemHeight = 0 - menu van bung ra nhung cao bang khong,
# nhin nhu khong co gi. Dat san mot chieu cao toi thieu.
edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\tKPopupMenu::Popup(pMenuData, (KWndWindow*)this, SEL_CHANNEL_MENU);',
     _crlf(b'\tif (pMenuData->nItemHeight < 16)\n'
           b'\t\tpMenuData->nItemHeight = 16;\t/* du cho chu 12 diem */\n'
           b'\tKPopupMenu::Popup(pMenuData, (KWndWindow*)this, SEL_CHANNEL_MENU);'),
     'menu kenh: chieu cao dong toi thieu')

# ---------------------------------------------------------------------------
# Nut chon kenh o dau thanh nhap lay nhan la ANH INLINE cua kenh dang chon
# (SetCurrentChannel gan KTC_INLINE_PIC + chi so anh). Anh nap qua
# AddCustomInlinePic; khong nap duoc thi chi so = 0xffff va nut thanh o trong.
# Them phuong an du phong: ve TEN kenh bang chu, va ghi lai de biet duong nao.
edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\t\t\tm_pSelf->m_ChannelSwitchBtn.SetText(buffer, 3);',
     _crlf(b'\t\t\tchar szTenKenh[64];\n'
           b'\t\t\tszTenKenh[0] = 0;\n'
           b'\t\t\tWORD nCaoAnh = 0;\n'
           b'\t\t\tKRColor uMau1, uMau2;\n'
           b'\t\t\tshort nCheck = -1;\n'
           b'\t\t\tKUiMsgCentrePad::GetChannelMenuinfo(m_pSelf->m_nCurChannel,\n'
           b'\t\t\t\tNULL, &nCaoAnh, &uMau1, &uMau2, szTenKenh, &nCheck);\n'
           b'\t\t\tif (nPicIndex == (WORD)-1 && szTenKenh[0])\n'
           b'\t\t\t\tm_pSelf->m_ChannelSwitchBtn.SetText(szTenKenh);\t/* anh hong -> ve chu */\n'
           b'\t\t\telse\n'
           b'\t\t\t\tm_pSelf->m_ChannelSwitchBtn.SetText(buffer, 3);'),
     'nut kenh: anh hong thi ve ten bang chu')

# Doi canh goi ReleaseActivateChannelAll xoa sach kenh dang ky ma may chu
# khong gui lai -> GetChannelCount ve 0, menu kenh rong. Thanh nguoi choi ve
# moi nhip nen dung no lam cho de dang ky lai tu danh sach da giu.
edit('S3Client/Ui/UiCase/UiPlayerBar.cpp',
     b'\tint nChannelDataCount = KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName;',
     _crlf(b'\tint nChannelDataCount = KUiMsgCentrePad::GetChannelCount() + m_nRecentPlayerName;\n'
           b'\t/* Doi canh xoa sach kenh dang ky ma may chu khong gui lai; dang ky\n'
           b'\t   lai tu danh sach da giu. Gioi han so lan de khong thu mai. */\n'
           b'\tstatic int s_nThuLai = 0;\n'
           b'\tif (KUiMsgCentrePad::GetChannelCount() > 0)\n'
           b'\t\ts_nThuLai = 0;\n'
           b'\telse if (s_nThuLai < 30)\n'
           b'\t{\n'
           b'\t\ts_nThuLai++;\n'
           b'\t\tKUiMsgCentrePad::XuLyKenhCho();\n'
           b'\t}'),
     'dang ky lai kenh sau khi doi canh xoa sach')

# ---------------------------------------------------------------------------
# BAT NHAT THAT: KUiMsgCentrePad::PtInWindow khai vung bam cua khung chat la
# HOP cua mot danh sach con CUNG CHET - MoveImg, SizeBtn, ChatRoom, Mat, Phong,
# Bang, Phai, Khac, BgShadowBtn - va QUEN sau nut tab (m_TabButton[0..5]),
# von cung la cua so con that su, duoc AddChild trong ShowChatTab.
#
# Hau qua: con tro nam tren mot nut tab thi PtInWindow tra 0, Wnd_GetActive
# khong chon khung chat, TopChildFromPoint khong duoc goi, nen nut tab khong
# he nhan WM_MOUSEMOVE hay WM_LBUTTONDOWN. Nhin ra ngoai: tab ve dung cho,
# dung kich thuoc, ma ro chuot khong doi mau va bam khong an.
#
# Do 01/09/2026: log tai Wnd_ProcessInput cho "active=00000000" ngay tai diem
# nam GON trong khung chat (0,520)-(360,675). Luu y Wnd_ProcessInput chi gui
# tin chuot cho pMouseOverWnd = pActiveWnd->TopChildFromPoint(x,y), nen cua so
# CHA im lang la binh thuong - dat log o KUiMsgCentrePad::WndProc khong chung
# minh duoc gi.
edit('S3Client/Ui/UiCase/UiMsgCentrePad.cpp',
     _crlf(b'\t\t\t\tm_BgShadowBtn.PtInWindow(x, y)\n\t\t\t\t);'),
     _crlf(b'\t\t\t\tm_BgShadowBtn.PtInWindow(x, y)\n'
           b'\t\t\t\t);\n'
           b'\t\t/* Hang tab cung la cua so con - thieu no thi tab khong nhan chuot. */\n'
           b'\t\tfor (int i = 0; !nRet && i < m_ChatTabCount && i < MAX_CHAT_TAB; i++)\n'
           b'\t\t\tnRet = m_TabButton[i].PtInWindow(x, y);'),
     'khung chat: vung bam phai gom ca hang tab')

# ---------------------------------------------------------------------------
# BAT NHAT THAT: NewChannelMessageArrival doc m_pActivateChannel[nChannelIndex]
# TRUOC khi kiem tra nChannelIndex >= 0. FindActivateChannelIndex tra -1 khi
# tin den tu mot kenh chua dang ky, luc do day la mot phep doc ngoai mang.
# Dua phep doc vao trong than if.
edit('S3Client/Ui/UiCase/UiMsgCentrePad.cpp',
     _crlf(b'\t\tint nID = m_pSelf->m_pActivateChannel[nChannelIndex].ResourceIndex;\n'
           b'\t\tif (nChannelIndex >= 0)\n'
           b'\t\t{'),
     _crlf(b'\t\tint nID = -1;\n'
           b'\t\tif (nChannelIndex >= 0)\n'
           b'\t\t{\n'
           b'\t\t\tnID = m_pSelf->m_pActivateChannel[nChannelIndex].ResourceIndex;'),
     'tin kenh: doc muc kenh sau khi da kiem tra chi so')

# ---------------------------------------------------------------------------
# Cua so TUY CHON: hai muc "bong dong" va "xuyen thau" chi Represent3 lam duoc
# (KUiOptions dat bInvalid = !g_bRepresent3), ma ta chay Represent2 - config.ini
# ghi Represent=0. Chung bi Enable(false) va ve bang DisableFrame=2 cua
# \Spr\Ui3\<he thong>\<nut danh dau>.spr, ma khung 2 la o XAM CO DAU TICK MO.
# Nhin ra ngoai: hai o luon "da tich" va bam khong nha ra duoc.
#
# Chi hien nhung muc may nay lam duoc. Bang anh xa cho phep bo qua muc hong ma
# khong pha thu tu cua m_ToggleItemList; neu sau nay bat Represent3 thi chung
# tu hien lai vi bInvalid tinh tu g_bRepresent3.
edit('S3Client/Ui/UiCase/UiOptions.h',
     b'\tint\t\t\t\t\tm_nToggleBtnValidCount;',
     _crlf(b'\tint\t\t\t\t\tm_nToggleBtnValidCount;\n'
           b'\t/* Muc thu i tren man hinh la muc nao trong m_ToggleItemList.\n'
           b'\t   Muc may nay khong lam duoc thi khong chiem cho. */\n'
           b'\tint\t\t\t\t\tm_ToggleIndexMap[MAX_TOGGLE_BTN_COUNT];'),
     'TUY CHON: bang anh xa cho tung o danh dau')

edit('S3Client/Ui/UiCase/UiOptions.cpp',
     _crlf(b'\tm_nFirstControlableIndex = 0;\n'
           b'\tm_nToggleBtnValidCount = (OPTION_INDEX_COUNT <= MAX_TOGGLE_BTN_COUNT) ? OPTION_INDEX_COUNT : MAX_TOGGLE_BTN_COUNT;'),
     _crlf(b'\tm_nFirstControlableIndex = 0;\n'
           b'\tm_nToggleBtnValidCount = 0;\n'
           b'\tfor (i = 0; i < OPTION_INDEX_COUNT && m_nToggleBtnValidCount < MAX_TOGGLE_BTN_COUNT; i++)\n'
           b'\t{\n'
           b'\t\tif (m_ToggleItemList[i].bInvalid)\n'
           b'\t\t\tcontinue;\t/* may nay khong lam duoc - khong chiem cho */\n'
           b'\t\tm_ToggleIndexMap[m_nToggleBtnValidCount] = i;\n'
           b'\t\tm_nToggleBtnValidCount++;\n'
           b'\t}'),
     'TUY CHON: chi xep cho cho muc may chay duoc')

edit('S3Client/Ui/UiCase/UiOptions.cpp',
     b'\t\tint nIndex = m_nFirstControlableIndex + i;',
     b'\t\tint nIndex = m_ToggleIndexMap[i];',
     'TUY CHON: nhan va mau lay theo bang anh xa')

edit('S3Client/Ui/UiCase/UiOptions.cpp',
     _crlf(b'\t\tif (m_ToggleItemList[m_nFirstControlableIndex + i].bInvalid == false)\n'
           b'\t\t{\n'
           b'\t\t\tnFrame = m_ToggleItemList[m_nFirstControlableIndex + i].bEnable ?'),
     _crlf(b'\t\tif (m_ToggleItemList[m_ToggleIndexMap[i]].bInvalid == false)\n'
           b'\t\t{\n'
           b'\t\t\tnFrame = m_ToggleItemList[m_ToggleIndexMap[i]].bEnable ?'),
     'TUY CHON: khung anh lay theo bang anh xa')

edit('S3Client/Ui/UiCase/UiOptions.cpp',
     b'\t\t\t\t\tToggleOption(m_nFirstControlableIndex + i);',
     b'\t\t\t\t\tToggleOption(m_ToggleIndexMap[i]);',
     'TUY CHON: bam vao o nao thi bat tat dung muc do')

# ---------------------------------------------------------------------------
# Thanh tren dinh: hai o so CAP va HANG duoc dung, duoc gan lam con, nhung
# khong noi nao do chu vao - hai o luon rong. Chu "Cap"/"Hang" nam san tren
# anh nen, hai o nay la CHO DIEN SO.
edit('S3Client/Ui/UiCase/UiHeaderControlBar.cpp',
     _crlf(b'\tDatThanh(m_Exp,     m_ExpText,\n'
           b'\t\tInfo.nExperience - Info.nExperienceFull,\n'
           b'\t\tInfo.nCurLevelExperience - Info.nExperienceFull);'),
     _crlf(b'\tDatThanh(m_Exp,     m_ExpText,\n'
           b'\t\tInfo.nExperience - Info.nExperienceFull,\n'
           b'\t\tInfo.nCurLevelExperience - Info.nExperienceFull);\n'
           b'\t/* Cap va hang xep the gioi nam o KUiPlayerAttribute, khong phai\n'
           b'\t   KUiPlayerRuntimeInfo - phai hoi rieng. */\n'
           b'\tKUiPlayerAttribute ThuocTinh;\n'
           b'\tmemset(&ThuocTinh, 0, sizeof(ThuocTinh));\n'
           b'\tg_pCoreShell->GetGameData(GDI_PLAYER_RT_ATTRIBUTE, (int)&ThuocTinh, 0);\n'
           b'\tchar szSo[32];\n'
           b'\tsprintf(szSo, "%d", ThuocTinh.nLevel);\n'
           b'\tm_LevelText.SetText(szSo);\n'
           b'\tsprintf(szSo, "%d", ThuocTinh.nRankInWorld);\n'
           b'\tm_RankWorldText.SetText(szSo);'),
     'thanh tren: dien so cap va hang xep the gioi')

# ---------------------------------------------------------------------------
# MAC DINH CUA TUYEN NAY: vao game thi IM TIENG va KHONG hieu ung thoi tiet.
# Nguon 2003 de nhac 100, tieng dong 100 va ca ba muc bat-tat deu bat.
# Nguoi choi van chinh duoc va lua chon van duoc ghi vao UserData/UiCommon.ini
# muc [Options]; day chi la gia tri khi CHUA co lua chon nao.
edit('S3Client/Ui/UiCase/UiOptions.cpp',
     b'"MusicValue", 100',
     b'"MusicValue", 0',
     'mac dinh: khong mo nhac nen')

edit('S3Client/Ui/UiCase/UiOptions.cpp',
     b'"SoundValue", 100',
     b'"SoundValue", 0',
     'mac dinh: khong mo am thanh')

edit('S3Client/Ui/UiCase/UiOptions.cpp',
     b'ls_ToggleOptionName[i], true, &bOptionsEnable[i]',
     b'ls_ToggleOptionName[i], false, &bOptionsEnable[i]',
     'mac dinh: ba muc bat-tat deu tat')

edit('S3Client/Ui/UiCase/UiOptions.cpp',
     _crlf(b'\t\ttrue, true, true\n'),
     _crlf(b'\t\tfalse, false, false\t/* dung khi khong mo duoc tep cai dat */\n'),
     'mac dinh khi khong doc duoc tep cai dat')

# BAT NHAT THAT - ba dong gan gia tri vua doc vao bien thanh vien bi COMMENT.
# Hau qua: mo bang roi dong ma khong dong vao thanh keo thi StoreSetting ghi
# lai GIA TRI CU cua bien thanh vien (100 tu ham dung), de len gia tri vua doc.
# Nghia la doi mac dinh thoi khong du - lan dong bang dau tien la no quay ve 100.
edit('S3Client/Ui/UiCase/UiOptions.cpp',
     _crlf(b'\t\t\t//m_pSelf->m_nBrightness = nBrightness;\n'
           b'\t\t\t//m_pSelf->m_nSoundValue = nSoundValue;\n'
           b'\t\t\t//m_pSelf->m_nMusicValue = nMusicValue;'),
     _crlf(b'\t\t\tm_pSelf->m_nBrightness = nBrightness;\n'
           b'\t\t\tm_pSelf->m_nSoundValue = nSoundValue;\n'
           b'\t\t\tm_pSelf->m_nMusicValue = nMusicValue;'),
     'bien thanh vien phai theo gia tri vua doc, khong thi StoreSetting ghi de')

# ---------------------------------------------------------------------------
# BAT NHAT THAT - bam vao NPC la CHET CLIENT.
#
# Khoi ve chan dung NPC trong hop thoai (KPlayer::OnScriptAction) do mot chuoi
# con tro ma khong chan cho nao:
#     Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetFileName(...)
# GetNpcRes() tra &m_DataRes nen khong bao gio NULL, nhung m_pcResNode BEN TRONG
# no thi co - khi tai nguyen NPC chua nap. Doc vao la chet.
#
# Va MaxFrame = GetTotalFrames(...) / GetTotalDirs(...) lay ket qua ham lam MAU
# SO. Tra 0 la chia cho 0 -> SIGFPE, tien trinh chet ngay.
#
# Do 01/09/2026: goi s2c_scriptaction (Msg 122) CO toi client - khong phai mat
# goi - roi client dung han. Nguoi choi thay "bam vao NPC la dung game".
#
# Chan ca hai, va coi chan dung la KHONG BAT BUOC: khong lay duoc anh thi van
# mo hop thoai qua nhanh KUiMsgSel (nhanh do da co san trong
# GameSpaceChangedNotify.cpp va co san UiMsgSel.ini).
edit('Core/Src/KPlayer.cpp',
     _crlf(b'if (m_nImageNpcID) \n'
           b'                    { \n'
           b'                    char szBuffer[128]; \n'),
     _crlf(b'if (m_nImageNpcID && Npc[m_nImageNpcID].GetNpcRes() &&\n'
           b'                        Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode) \n'
           b'                    { \n'
           b'                    char szBuffer[128]; \n'),
     'chan dung NPC: khong doc m_pcResNode khi no NULL')

edit_all('Core/Src/KPlayer.cpp',
     _crlf(b'                            pImage->MaxFrame = (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalFrames(i, 3, 0, 16))/ \n'
           b'                            (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalDirs(i, 3, 0, 16)); goto Next; \n'),
     _crlf(b'                            { \n'
           b'                            int nHuong = Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalDirs(i, 3, 0, 16); \n'
           b'                            pImage->MaxFrame = nHuong > 0 ? \n'
           b'                                (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalFrames(i, 3, 0, 16)) / nHuong : 1; \n'
           b'                            } \n'
           b'                            goto Next; \n'),
     'chan dung NPC: so huong = 0 la chia cho 0')

edit_all('Core/Src/KPlayer.cpp',
     _crlf(b'                            pImage->MaxFrame = (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalFrames(j, 0, 0, 16))/ \n'
           b'                            (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalDirs(j, 0, 0, 16)); goto Next; \n'),
     _crlf(b'                            { \n'
           b'                            int nHuong = Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalDirs(j, 0, 0, 16); \n'
           b'                            pImage->MaxFrame = nHuong > 0 ? \n'
           b'                                (Npc[m_nImageNpcID].GetNpcRes()->m_pcResNode->GetTotalFrames(j, 0, 0, 16)) / nHuong : 1; \n'
           b'                            } \n'
           b'                            goto Next; \n'),
     'chan dung NPC: so huong = 0 la chia cho 0 (vong thu hai)')

# KUiMsgSelNew::OpenWindow doc thang pImage->ImageFile ma khong kiem NULL.
# Cho goi no la nhanh "if (nParam)" nen pImage khong NULL o duong binh thuong,
# nhung mot lan tay doi la chet ca cua so. Chan cho chac.
edit('S3Client/Ui/UiCase/UiMsgSelNew.cpp',
     b'KUiMsgSelNew* KUiMsgSelNew::OpenWindow(KUiQuestionAndAnswer* pContent, KUiNpcSpr *pImage)\r\n{',
     b'KUiMsgSelNew* KUiMsgSelNew::OpenWindow(KUiQuestionAndAnswer* pContent, KUiNpcSpr *pImage)\r\n{\r\n\tif (pImage == NULL)\t/* nguoi goi da lai NULL sang KUiMsgSel; chan cho chac */\r\n\t\treturn NULL;',
     'hop thoai co chan dung: pImage NULL thi thoat, khong doc vao')

# ---------------------------------------------------------------------------
# So thanh vien to doi (party) 8 -> 20.
#
# MAX_TEAM_MEMBER la so DOI VIEN khong ke doi truong, nen 7 = 8 nguoi, 19 = 20.
# Hang nay nam trong cac struct protocol to doi (PLAYER_TEAM_ADD_MEMBER,
# KUiPlayerTeam, m_nMember[MAX_TEAM_MEMBER]...) gui giua world server va client.
# PHAI doi GIONG HET ca hai phia, khong thi struct lech kich thuoc -> client doc
# nham -> treo. Ban server doi truc tiep o server/linux-server/Core/GameDataDef.h.
# Ma to doi dung toan MAX_TEAM_MEMBER (khong hard-code 8) nen mang/vong lap tu
# co gian. UI [MemberList] la danh sach cuon nen chua duoc nhieu hon 8.
#
# Doi CA BON ban trong cay client de khong lech noi bo (moi don vi bien dich lay
# ban nao cung ra 19).
for _gd in ('Core/Src/GameDataDef.h', 'LINUX/Core/GameDataDef.h',
            'Engine/Core/GameDataDef.h', 'Kernel/Core/GameDataDef.h'):
    edit(_gd,
         b'#define\t\tMAX_TEAM_MEMBER\t\t\t\t\t\t7\t\t',
         b'#define\t\tMAX_TEAM_MEMBER\t\t\t\t\t\t19\t\t',
         'so thanh vien to doi 8 -> 20')

# ---------------------------------------------------------------------------
# TOOLTIP hien ANH GOC cua vat pham.
#
# Bag hien icon da co ve 1 o (spr/item/...); tooltip hien anh GOC day du tu
# spr/item-goc/... (batch client/co-icon-batch.py luu ca hai). Vi dia de len pak
# o dung ten, phai giu ban goc duoi ten khac (item-goc) roi nap rieng.

# 1) KItem: getter cong khai lay ten anh icon.
edit('Core/Src/KItem.h',
     _crlf(b'class KItem\n{\npublic:\n\tKItem();\n\t~KItem();\n'),
     _crlf(b'class KItem\n{\npublic:\n\tKItem();\n\t~KItem();\n'
           b'\tconst char* GetImageName() { return m_CommonAttrib.szImageName; }\n'),
     'getter ten anh icon vat pham')

# 2) CoreShell: them chi so lay ten anh vat pham.
edit('Core/Src/CoreShell.h',
     _crlf(b'GDI_PLAYER_TK_XEPHANG,\n\n};'),
     _crlf(b'GDI_PLAYER_TK_XEPHANG,\n\n\tGDI_GAME_OBJ_IMAGE,\t//ten anh icon vat pham (tooltip)\n\n};'),
     'them GDI_GAME_OBJ_IMAGE')

# 3) CoreShell: xu ly chi so do - copy ten anh cua item.
edit('Core/Src/CoreShell.cpp',
     _crlf(b'\n\tcase GDI_GAME_OBJ_DESC:\n'),
     _crlf(b'\n\tcase GDI_GAME_OBJ_IMAGE:\n'
           b'\t\tif (nParam && uParam)\n'
           b'\t\t{\n'
           b'\t\t\tKUiObjAtContRegion* pObjImg = (KUiObjAtContRegion *)uParam;\n'
           b'\t\t\tchar* pszImg = (char *)nParam;\n'
           b'\t\t\tpszImg[0] = 0;\n'
           b'\t\t\tif (pObjImg->Obj.uGenre == CGOG_ITEM && pObjImg->Obj.uId)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tconst char* pNm = Item[pObjImg->Obj.uId].GetImageName();\n'
           b'\t\t\t\tint iNm = 0;\n'
           b'\t\t\t\tfor (; pNm[iNm] && iNm < 79; iNm++) pszImg[iNm] = pNm[iNm];\n'
           b'\t\t\t\tpszImg[iNm] = 0;\n'
           b'\t\t\t}\n'
           b'\t\t}\n'
           b'\t\tbreak;\n'
           b'\n\tcase GDI_GAME_OBJ_DESC:\n'),
     'xu ly GDI_GAME_OBJ_IMAGE')

# 3b) Dau "dang bay ban" tren icon vat pham da dinh gia (giong anh mau #14).
# GDI lay gia theo uId, roi WndObjContainer ve o vuong nho goc tren-phai.
edit('Core/Src/CoreShell.h',
     _crlf(b'\tGDI_GAME_OBJ_IMAGE,\t//ten anh icon vat pham (tooltip)\n\n};'),
     _crlf(b'\tGDI_GAME_OBJ_IMAGE,\t//ten anh icon vat pham (tooltip)\n\n\tGDI_ITEM_SALE_PRICE,\t//gia rao ban (dau tren icon tui)\n\n};'),
     'them GDI_ITEM_SALE_PRICE')

edit('Core/Src/CoreShell.cpp',
     _crlf(b'\n\tcase GDI_GAME_OBJ_IMAGE:\n'),
     _crlf(b'\n\tcase GDI_ITEM_SALE_PRICE:\n'
           b'\t\tif (uParam > 0 && uParam < MAX_ITEM)\n'
           b'\t\t\tnRet = Item[uParam].GetSetPrice();\t/* gia NIEM YET (uPrice), khong phai gia goc (nPrice) */\n'
           b'\t\tbreak;\n'
           b'\n\tcase GDI_GAME_OBJ_IMAGE:\n'),
     'xu ly GDI_ITEM_SALE_PRICE')

edit('S3Client/Ui/Elem/WndObjContainer.cpp',
     _crlf(b'\t\tint width = m_nUnitWidth * pObj->DataW - m_nUnitBorder * 2;'),
     _crlf(b'\t\t/* To NEN o vat pham dang bay ban (da dinh gia), neu chua co mau\n'
           b'\t\t   hover/chon de khong de len. Nen ve truoc icon nen nam duoi. */\n'
           b'\t\tif (Shadow.Color.Color_dw == 0 && pObj->uGenre == CGOG_ITEM &&\n'
           b'\t\t\tg_pCoreShell->GetGameData(GDI_ITEM_SALE_PRICE, pObj->uId, 0) > 0)\n'
           b'\t\t\tShadow.Color.Color_dw = 0x26FFFF00;\t/* vang, opacity ~0.15 (ARGB) */\n'
           b'\t\tint width = m_nUnitWidth * pObj->DataW - m_nUnitBorder * 2;'),
     'to nen o vat pham dang bay ban')

# 4) MouseHover.h: khai bao ham + bien icon.
edit('S3Client/Ui/Elem/MouseHover.h',
     _crlf(b'\tvoid\tCancelMouseHoverInfo();\n'),
     _crlf(b'\tvoid\tCancelMouseHoverInfo();\n'
           b'\tvoid\tSetMouseHoverIcon(const char* szImage);\n'),
     'khai bao SetMouseHoverIcon')
edit('S3Client/Ui/Elem/MouseHover.h',
     _crlf(b'\tint\t\tm_nObj;\n'),
     _crlf(b'\tint\t\tm_nObj;\n'
           b'\tKRUImage m_ItemIcon;\n'
           b'\tint m_nItemIconW;\n'
           b'\tint m_nItemIconH;\n'
           b'\tint m_nItemIconFrames;\n'
           b'\tbool m_bHasItemIcon;\n'),
     'bien icon anh goc tooltip')

# 5) MouseHover.cpp ctor: khoi tao co icon = false.
edit('S3Client/Ui/Elem/MouseHover.cpp',
     _crlf(b'\tm_bFollowCursor = false;\n\tm_bShow = false;\n}'),
     _crlf(b'\tm_bFollowCursor = false;\n\tm_bHasItemIcon = false;\n\tm_nItemIconFrames = 0;\n\tm_bShow = false;\n}'),
     'ctor: m_bHasItemIcon = false')

# 6) MouseHover.cpp Cancel: reset icon + dinh nghia SetMouseHoverIcon.
edit('S3Client/Ui/Elem/MouseHover.cpp',
     _crlf(b'\tm_nDescLen = 0;\n\tm_bShow = false;\n}\n\n/'),
     _crlf(b'\tm_nDescLen = 0;\n\tm_bHasItemIcon = false;\n\tm_bShow = false;\n}\n'
           b'\n'
           b'void KMouseOver::SetMouseHoverIcon(const char* szImage)\n'
           b'{\n'
           b'\tm_bHasItemIcon = false;\n'
           b'\tm_nItemIconW = 0;\n'
           b'\tm_nItemIconH = 0;\n'
           b'\tm_nItemIconFrames = 0;\n'
           b'\tif (szImage == NULL || szImage[0] == 0 || g_pRepresentShell == NULL)\n'
           b'\t\treturn;\n'
           b'\tmemset(&m_ItemIcon, 0, sizeof(KRUImage));\n'
           b'\tm_ItemIcon.nType = ISI_T_SPR;\n'
           b'\tm_ItemIcon.bRenderStyle = IMAGE_RENDER_STYLE_ALPHA;\n'
           b'\tm_ItemIcon.Color.Color_b.a = 255;\n'
           b'\tm_ItemIcon.uImage = 0;\n'
           b'\tm_ItemIcon.nISPosition = IMAGE_IS_POSITION_INIT;\n'
           b'\tm_ItemIcon.nFrame = 0;\n'
           b'\tstrncpy(m_ItemIcon.szImage, szImage, sizeof(m_ItemIcon.szImage) - 1);\n'
           b'\tKImageParam Param;\n'
           b'\tParam.nWidth = 0;\n'
           b'\tParam.nHeight = 0;\n'
           b'\tg_pRepresentShell->GetImageParam(m_ItemIcon.szImage, &Param, m_ItemIcon.nType);\n'
           b'\tm_nItemIconW = Param.nWidth;\n'
           b'\tm_nItemIconH = Param.nHeight;\n'
           b'\tm_nItemIconFrames = Param.nNumFrames;\n'
           b'\tif (m_nItemIconW > 0 && m_nItemIconH > 0)\n'
           b'\t\tm_bHasItemIcon = true;\n'
           b'\tUpdate(m_nApplyX, m_nApplyY);\n'
           b'}\n'
           b'\n/'),
     'dinh nghia SetMouseHoverIcon')

# 7) MouseHover.cpp Update: cong chieu cao icon vao cua so.
edit('S3Client/Ui/Elem/MouseHover.cpp',
     _crlf(b'\tm_nWndHeight = (m_nFontSize + 1) * nNumLine;\n'
           b'\tif (m_bHeadTailImg)\n'
           b'\t\tm_nWndHeight += m_nImgHeight * 2;\n'),
     _crlf(b'\tm_nWndHeight = (m_nFontSize + 1) * nNumLine;\n'
           b'\tif (m_bHeadTailImg)\n'
           b'\t\tm_nWndHeight += m_nImgHeight * 2;\n'
           b'\tif (m_bHasItemIcon)\n'
           b'\t{\n'
           b'\t\tm_nWndHeight += m_nItemIconH;\n'
           b'\t\tif (m_nItemIconW + m_nIndent * 2 > m_nWndWidth)\n'
           b'\t\t\tm_nWndWidth = m_nItemIconW + m_nIndent * 2;\n'
           b'\t}\n'),
     'Update: cong chieu cao icon')

# 8) MouseHover.cpp Paint: ve icon o dau + day chu xuong.
edit('S3Client/Ui/Elem/MouseHover.cpp',
     _crlf(b'\tif (m_bHeadTailImg)\n'
           b'\t\tShadow.oPosition.nY = m_nTop + m_nImgHeight;\n'
           b'\telse\n'
           b'\t\tShadow.oPosition.nY = m_nTop;\n'
           b'\tShadow.oEndPos.nX = m_nLeft + m_nWndWidth;\n'),
     _crlf(b'\tif (m_bHeadTailImg)\n'
           b'\t\tShadow.oPosition.nY = m_nTop + m_nImgHeight;\n'
           b'\telse\n'
           b'\t\tShadow.oPosition.nY = m_nTop;\n'
           b'\tif (m_bHasItemIcon)\n'
           b'\t{\n'
           b'\t\tm_ItemIcon.oPosition.nX = m_nLeft + (m_nWndWidth - m_nItemIconW) / 2;\n'
           b'\t\tm_ItemIcon.oPosition.nY = Shadow.oPosition.nY;\n'
           b'\t\tif (m_nItemIconFrames > 1)\n'
           b'\t\t{\n'
           b'\t\t\tstatic unsigned s_uIconTick = 0;\n'
           b'\t\t\ts_uIconTick++;\n'
           b'\t\t\tm_ItemIcon.nFrame = (s_uIconTick / 8) % m_nItemIconFrames;\n'
           b'\t\t}\n'
           b'\t\tg_pRepresentShell->DrawPrimitives(1, &m_ItemIcon, RU_T_IMAGE, true);\n'
           b'\t\tShadow.oPosition.nY += m_nItemIconH;\n'
           b'\t}\n'
           b'\tShadow.oEndPos.nX = m_nLeft + m_nWndWidth;\n'),
     'Paint: ve icon goc o dau tooltip')


# 9) MouseHover.cpp: goi SetMouseHoverIcon sau khi dung xong desc.
edit('S3Client/Ui/Elem/MouseHover.cpp',
     _crlf(b'SetMouseHoverDesc(Desc.szDesc, nLenDesc, s_uHoverObjDestTextColor);\n\t\t}\n\t}\t\n}'),
     _crlf(b'SetMouseHoverDesc(Desc.szDesc, nLenDesc, s_uHoverObjDestTextColor);\n\t\t}\n'
           b'\t\t// Icon GOC cua vat pham vao tooltip (bag 1 o, tooltip full).\n'
           b'\t\tif (uId)\n'
           b'\t\t{\n'
           b'\t\t\tchar szImg[80];\n'
           b'\t\t\tszImg[0] = 0;\n'
           b'\t\t\tg_pCoreShell->GetGameData(GDI_GAME_OBJ_IMAGE, (unsigned int)&Obj, (int)szImg);\n'
           b'\t\t\tchar* pIt = strstr(szImg, "\\\\item\\\\");\n'
           b'\t\t\tif (pIt)\n'
           b'\t\t\t{\n'
           b'\t\t\t\tchar szGoc[96];\n'
           b'\t\t\t\tint nPre = (int)(pIt - szImg) + 5;\n'
           b'\t\t\t\tmemcpy(szGoc, szImg, nPre);\n'
           b'\t\t\t\tszGoc[nPre] = 0;\n'
           b'\t\t\t\tstrcat(szGoc, "-goc");\n'
           b'\t\t\t\tstrcat(szGoc, pIt + 5);\n'
           b'\t\t\t\tg_MouseOver.SetMouseHoverIcon(szGoc);\n'
           b'\t\t\t}\n'
           b'\t\t}\n'
           b'\t}\t\n}'),
     'goi SetMouseHoverIcon voi anh goc')

# ---------------------------------------------------------------------------
# Gia BAN vat pham 1:1 (bang gia goc), khong chia 4.
# Tuyen nay KHONG co cho mua (do chi rot tu quai), nen ban lai nua/phan tu gia la
# thiet. Dat BUY_SELL_SCALE=1: gia ban hien va tien nhan deu = gia goc.
# Server doi rieng o server/linux-server/Core/KBuySell.h.
edit('Core/Src/KBuySell.h',
     b'#define\tBUY_SELL_SCALE\t\t4',
     b'#define\tBUY_SELL_SCALE\t\t1',
     'gia ban vat pham 1:1 (khong chia 4)')

# ---------------------------------------------------------------------------
# Noi day module auto CUA TA (S3Client/Auto/KAutoControl.*, commit thang trong
# kho) vao build va vao vong lap game. File nguon la ma moi cua ta; chi phan
# CHAM VAO FILE GOC di qua day: them vao vcxproj + 1 dong o UiHeartBeat.
print('\nNoi day module auto (kenh lenh dieu khien trong game):')
edit('S3Client/S3Client.vcxproj',
     b'<ClCompile Include="ErrorCode.cpp" />',
     b'<ClCompile Include="ErrorCode.cpp" />\r\n    <ClCompile Include="Auto\\KAutoControl.cpp" />',
     'them KAutoControl.cpp vao build')
edit('S3Client/S3Client.vcxproj',
     b'<ClInclude Include="ErrorCode.h" />',
     b'<ClInclude Include="ErrorCode.h" />\r\n    <ClInclude Include="Auto\\KAutoControl.h" />',
     'them KAutoControl.h vao project')
edit('S3Client/Ui/UiShell.cpp',
     b'#include "UiBase.h"',
     b'#include "UiBase.h"\r\n#include "../Auto/KAutoControl.h"',
     'UiShell include KAutoControl')
# Goi Tick moi khung, chi khi da vao game (s_UiLiveSeed == UI_LIVING_S_INGAME).
# Wnd_Heartbeat() chay moi nhip khi con ket noi; dat Tick ngay sau no.
edit('S3Client/Ui/UiShell.cpp',
     b'Wnd_Heartbeat();',
     b'Wnd_Heartbeat();\r\n\t\t\tif (s_UiLiveSeed == UI_LIVING_S_INGAME) KAutoControl::Tick();',
     'goi KAutoControl::Tick() moi khung trong game')

# ---------------------------------------------------------------------------
# Bat lai PHAI CHUOT MAC DO (bi tat khi port). Right-click o tui -> OnClickItem
# -> GOI_USE_ITEM -> ApplyUseItem, nhung nhanh item_equip bi bo trong nen khong
# gui gi. Sua CLIENT-ONLY: tra REQUEST_EQUIP_ITEM roi gui mot lenh MOVE tu o tui
# sang o trang bi (pos_equip). Server ExchangeItem da tu Equip + dong bo (chinh
# la duong keo-tha dang chay), khong can doi server hay them protocol.
print('\nBat lai phai chuot mac do (client-only, tai dung duong move):')
edit('Core/Src/KItemList.cpp',
     _crlf(b'\tcase item_equip:\n\t\tbreak;\n/*\t\tif (Equip(nNpcIdx, nIdx))\n\t\t\tnRet = REQUEST_EQUIP_ITEM;*/\n\t\tbreak;'),
     _crlf(b'\tcase item_equip:\n\t\tnRet = REQUEST_EQUIP_ITEM;\t// mac do: de ApplyUseItem gui move len o trang bi\n\t\tbreak;'),
     'UseItem tra REQUEST_EQUIP_ITEM cho do trang bi')
edit('Core/Src/KPlayer.cpp',
     _crlf(b'\tif (nRet == REQUEST_EQUIP_ITEM)\n\t{\n\t}'),
     _crlf(b'\tif (nRet == REQUEST_EQUIP_ITEM)\n\t{\n'
           b'\t\t// Mac do: gui lenh move tu o tui (SrcPos) sang o trang bi (pos_equip).\n'
           b'\t\t// Server ExchangeItem se tu Equip + dong bo; tai dung duong keo-tha.\n'
           b'\t\tItemPos EquipPos;\n'
           b'\t\tEquipPos.nPlace = pos_equip;\n'
           b'\t\tEquipPos.nX = m_ItemList.GetEquipPlace(Item[nItemID].GetDetailType());\n'
           b'\t\tEquipPos.nY = 0;\n'
           b'\t\tMoveItem(SrcPos, EquipPos);\n'
           b'\t}'),
     'ApplyUseItem gui move len o trang bi khi mac do')

# ---------------------------------------------------------------------------
# Tong ket PHAI o cuoi tep. Truoc day no nam giua, nen moi ban va viet them sau
# do khong duoc dem va - hong mot cho o phan sau van cho CI mau xanh.
print('\n=== va %d cho, bo qua %d, HONG %d ===' % (n_ok, n_skip, n_hong))
# Mau neo truot (thuong do CRLF) tung lam CI xanh trong khi Game.exe chua he
# duoc va. Hong mot cho la hong ca ban dung -> dung han.
if n_hong:
    sys.exit(1)

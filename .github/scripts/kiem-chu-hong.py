#!/usr/bin/env python3
"""Chan chu bi thay bang dau hoi U+FFFD lot vao nguon.

Chay truoc khi dich. Tra ve ma loi khac 0 neu tim thay -> build dung lai.

  python .github/scripts/kiem-chu-hong.py [thu-muc...]

Vi sao can: nguon game tron TCVN3/GBK, moi byte >= 0x80 la chu that. Cong cu
nao doc tep bang UTF-8 roi ghi lai se thay tung byte do bang EF BF BD (ky tu
thay the U+FFFD) - chu bien mat vinh vien, va TRINH DICH KHONG HE BAO GI.

Da xay ra that: dem 06/09/2026 mot phien va nguon lam hong 12.626 cho, trong
do 31 chuoi hien thi. Menu chuot phai hien "Tün g?u" thay vi "Tan gau"; phai
ba ngay sau moi phat hien, va phai lay lai tung chuoi tu ban goc trong git.

Chi bao loi cho chuoi trong dau nhay ("...") - chu thich hong thi kho chiu
nhung khong lam sai chuong trinh, nen chi dem va nhac.
"""
import re
import sys
from pathlib import Path

HONG = b'\xef\xbf\xbd'
CHUOI = re.compile(rb'"[^"\n]*"')
DUOI = ('.cpp', '.h', '.c', '.lua', '.ini', '.txt')

# Cay nguon co nhieu BAN SAO khong tham gia build (workflow chi dich Common,
# Engine, Core, FilterText, Represent2, Rainbow, S3Client). Cac ban sao do von
# da hong tu luc nhap kho va khong co ban sach de lay lai, nen dung chan build
# vi chung - chi soi phan that su duoc dich.
BO_QUA = ('/Anti-Cheat-Client/', '/LINUX/', '/Engine/Include/', '/Engine/Core/',
          '/Engine/Ui/', '/Kernel/', '/jxmu/')


def quet(goc: Path):
    loi = []
    chu_thich = 0
    for p in goc.rglob('*'):
        if not p.is_file() or p.suffix.lower() not in DUOI:
            continue
        # as_posix(): tren Windows duong dan dung '\\' nen so chuoi kieu '/LINUX/'
        # se khong bao gio khop - da lam CI do mot lan vi dung str(p).
        duong = p.as_posix()
        if '/.git/' in duong or any(x in duong for x in BO_QUA):
            continue
        try:
            b = p.read_bytes()
        except OSError:
            continue
        if HONG not in b:
            continue
        trong_chuoi = [m for m in CHUOI.finditer(b) if HONG in m.group(0)]
        for m in trong_chuoi:
            dong = b.count(b'\n', 0, m.start()) + 1
            loi.append((p, dong, m.group(0)[:60].decode('latin-1')))
        chu_thich += b.count(HONG) - sum(m.group(0).count(HONG) for m in trong_chuoi)
    return loi, chu_thich


def main() -> int:
    goc = Path(__file__).resolve().parent.parent.parent
    thu_muc = [Path(a) for a in sys.argv[1:] if not a.startswith('--')]
    if not thu_muc:
        thu_muc = [goc / 'ClientAnti_JX1' / 'SwordOnline' / 'Sources']

    tong_loi = []
    tong_chu_thich = 0
    for t in thu_muc:
        if not t.is_dir():
            continue
        l, c = quet(t)
        tong_loi += l
        tong_chu_thich += c

    if tong_chu_thich:
        print('[chu hong] %d dau hoi U+FFFD trong CHU THICH (khong chan build)'
              % tong_chu_thich)
    if not tong_loi:
        print('[chu hong] khong co chuoi nao hong. OK')
        return 0

    print('[chu hong] %d CHUOI bi hong - nguon da bi doc/ghi qua UTF-8:' % len(tong_loi))
    for p, dong, s in tong_loi[:20]:
        print('    %s:%d  %s' % (p, dong, s))
    if len(tong_loi) > 20:
        print('    ... va %d cho nua' % (len(tong_loi) - 20))
    print('Lay lai tung chuoi tu ban goc trong git: git show <commit-goc>:<tep>')
    return 1


if __name__ == '__main__':
    raise SystemExit(main())

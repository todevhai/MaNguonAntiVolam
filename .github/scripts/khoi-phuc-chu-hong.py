#!/usr/bin/env python3
"""Khoi phuc cac chuoi bi thay bang dau hoi U+FFFD, lay tu BAN GOC trong git.

Vi sao khong doan: moi ky tu hong bien thanh EF BF BD, va bo chuyen doi co the
gop nhieu byte loi thanh MOT dau hoi - nen khong suy nguoc duoc do dai that.
Ban goc Kingsoft nam ngay trong lich su git va sach 100%, cu lay tu do.

  python .github/scripts/khoi-phuc-chu-hong.py --xem     # chi liet ke
  python .github/scripts/khoi-phuc-chu-hong.py           # khoi phuc

Cach doi chieu: lay mot doan ASCII lien truoc dau nhay mo lam NEO (vd
'GetInteger(szEventItm, ') roi tim chinh no trong ban goc. Neo phai duy nhat -
cho nao khong duy nhat thi bo qua va bao ra, tu xu ly.

Sau khi khoi phuc, chuoi van la chu Han. Chuoi nao NGUOI CHOI THAY thi dich
sang tieng Viet (TCVN3, xem tcvn3.py); chuoi nao dung lam KHOA tra bang du lieu
(vd "具体类别" trong questkey.txt) thi PHAI giu nguyen chu Han.
"""
import re
import subprocess
import sys
from pathlib import Path

GOC = Path(__file__).resolve().parent.parent.parent
BAN_GOC = '907e128'          # commit nguon goc trong kho nay
HONG = b'\xef\xbf\xbd'
CHUOI = re.compile(rb'"[^"\n]*"')
DAI_NEO = 40


def ban_goc(duong: str) -> bytes:
    r = subprocess.run(['git', 'show', '%s:%s' % (BAN_GOC, duong)],
                       cwd=GOC, capture_output=True)
    return r.stdout if r.returncode == 0 else b''


def tim_trong_goc(g: bytes, d: bytes, m) -> int:
    """Tra vi tri chuoi tuong ung trong ban goc, hoac -1.

    Neo bang doan TRUOC dau nhay mo. Doan 40 byte thuong trung nhau (nhieu cho
    goi cung mot ham), nen noi dan ra 80 roi 200 byte; van trung thi kem them
    doan SAU dau nhay dong - hai dau cong lai gan nhu luon duy nhat.
    """
    for dai in (40, 80, 200, 400):
        truoc = d[max(0, m.start() - dai):m.start()]
        if len(truoc) < 8:
            continue
        n = g.count(truoc)
        if n == 1:
            return g.find(truoc) + len(truoc)
        if n == 0:
            continue
        # con trung: chot them bang duoi
        sau = d[m.end():m.end() + 60]
        sau = sau.split(b'\n')[0]
        if len(sau) >= 6:
            i = -1
            thay = []
            while True:
                i = g.find(truoc, i + 1)
                if i < 0:
                    break
                j = i + len(truoc)
                mg = CHUOI.match(g, j)
                if mg and g[mg.end():mg.end() + len(sau)] == sau:
                    thay.append(j)
            if len(thay) == 1:
                return thay[0]
    return -1


def lam(duong: str, chi_xem: bool):
    p = GOC / duong
    d = p.read_bytes()
    g = ban_goc(duong)
    if not g:
        print('  !! khong lay duoc ban goc: %s' % duong)
        return 0, 0
    if HONG in g:
        print('  !! ban goc cung hong, bo qua: %s' % duong)
        return 0, 0

    duoc = bo = 0
    ra = bytearray()
    cuoi = 0
    for m in CHUOI.finditer(d):
        if HONG not in m.group(0):
            continue
        i = tim_trong_goc(g, d, m)
        mg = CHUOI.match(g, i) if i >= 0 else None
        if not mg:
            print('  bo qua (khong doi chieu duoc): %s' % m.group(0)[:44].decode('latin-1'))
            bo += 1
            continue
        ra += d[cuoi:m.start()] + mg.group(0)
        cuoi = m.end()
        duoc += 1
        if chi_xem:
            try:
                doc = mg.group(0)[1:-1].decode('gbk')
            except Exception:
                doc = mg.group(0)[1:-1].decode('latin-1')
            print('  %-52s -> %s' % (m.group(0)[:50].decode('latin-1'), doc))
    ra += d[cuoi:]
    if duoc and not chi_xem:
        p.write_bytes(bytes(ra))
    print('  %s: khoi phuc %d, bo qua %d' % (duong.split('/')[-1], duoc, bo))
    return duoc, bo


def main() -> int:
    chi_xem = '--xem' in sys.argv
    tep = [a for a in sys.argv[1:] if not a.startswith('--')]
    if not tep:
        # tu tim: moi tep nguon co dau hoi trong CHUOI
        tep = []
        for p in (GOC / 'ClientAnti_JX1' / 'SwordOnline' / 'Sources').rglob('*'):
            if p.suffix.lower() not in ('.cpp', '.h', '.c'):
                continue
            b = p.read_bytes()
            if any(HONG in m.group(0) for m in CHUOI.finditer(b)):
                tep.append(str(p.relative_to(GOC)))
    tong_duoc = tong_bo = 0
    for t in sorted(tep):
        a, b = lam(t, chi_xem)
        tong_duoc += a
        tong_bo += b
    print('TONG: khoi phuc %d chuoi, bo qua %d' % (tong_duoc, tong_bo))
    return 0


if __name__ == '__main__':
    raise SystemExit(main())

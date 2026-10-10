// Chien bao Tong Kim (phim `): giai goi s2c_chientruongbao vao g_ChienBao. Luat theo client ban6 (switch 0x5f08e0):
// loai 0 bat co hien, 1 mot dong (hang 0 = ban than, 1..10 = bang muoi nguoi), 2 game data, 5 roi tran (xoa),
// 6 bon chuoi mo ta, 10 so nguoi hai phe. Cua so UiBattleReport doc g_ChienBao qua GetGameData(GDI_CHIEN_BAO).
#include "KCore.h"
#include "KEngine.h"
#include "GameDataDef.h"
#include "KProtocol.h"
#include <string.h>

KChienBaoDuLieu g_ChienBao;

// Chen / cap nhat theo ten, xep giam dan theo v[1] (tong tich luy) - may chu ChienBao TChienBaoBang::Chen.
static void ChenDong(const KChienBaoDong & moi)
{
	KChienBaoDuLieu & c = g_ChienBao;
	for (int i = 0; i < c.nSoDong; ++i)
	{
		if (strcmp(c.Bang[i].szTen, moi.szTen) == 0)
		{
			memmove(&c.Bang[i], &c.Bang[i + 1], (c.nSoDong - i - 1) * sizeof(KChienBaoDong));
			--c.nSoDong;
			break;
		}
	}
	int p = 0;
	while (p < c.nSoDong && c.Bang[p].v[1] >= moi.v[1])
		++p;
	if (p >= CHIEN_BAO_DONG)
		return;
	int nDoi = (c.nSoDong < CHIEN_BAO_DONG ? c.nSoDong : CHIEN_BAO_DONG - 1) - p;
	if (nDoi > 0)
		memmove(&c.Bang[p + 1], &c.Bang[p], nDoi * sizeof(KChienBaoDong));
	c.Bang[p] = moi;
	if (c.nSoDong < CHIEN_BAO_DONG)
		++c.nSoDong;
}

// Doc chuoi co NUL tu than goi (con nCon byte), chep toi da nDai - 1. Tra so byte da doc (ca NUL).
static int DocChuoi(const BYTE * p, int nCon, char * szDich, int nDai)
{
	int n = 0;
	while (n < nCon && p[n])
		++n;
	int nChep = n < nDai - 1 ? n : nDai - 1;
	memcpy(szDich, p, nChep);
	szDich[nChep] = 0;
	return n < nCon ? n + 1 : n;
}

// Tra loai goi (de UI biet), -1 khi goi hong.
int ChienBaoNhan(const BYTE * pMsg)
{
	const CHIEN_TRUONG_BAO_HEAD * h = (const CHIEN_TRUONG_BAO_HEAD *)pMsg;
	int nTong = 1 + h->m_wLength;
	if (nTong < (int)sizeof(*h))
		return -1;
	const BYTE * pThan = pMsg + sizeof(*h);
	int nThan = nTong - (int)sizeof(*h);
	KChienBaoDuLieu & c = g_ChienBao;
	switch (h->m_btLoai)
	{
	case chienbao_xem:
		for (int i = 0; i < h->m_btSoMuc && i < nThan; ++i)
			if (pThan[i] < CHIEN_BAO_LOAI)
				c.Xem[pThan[i]] = 1;
		break;
	case chienbao_dong:
		{
			// Ten khong NUL: do dai = m_wLength - 5 x so muc - 6 (ban6), phai < 32.
			int nTen = h->m_wLength - 5 * h->m_btSoMuc - 6;
			if (nTen < 0 || nTen >= 32 || nTen + 5 * h->m_btSoMuc > nThan)
				return -1;
			KChienBaoDong d;
			memset(&d, 0, sizeof(d));
			memcpy(d.szTen, pThan, nTen);
			d.btPhe = h->m_btPhe;
			for (int i = 0; i < h->m_btSoMuc; ++i)
			{
				const BYTE * q = pThan + nTen + 5 * i;
				if (q[0] < CHIEN_BAO_LOAI)
					memcpy(&d.v[q[0]], q + 1, sizeof(int));
			}
			if (h->m_btHang == 0)
			{
				c.BanThan = d;
				c.bCoBanThan = 1;
			}
			else if (h->m_btHang <= CHIEN_BAO_DONG)
				ChenDong(d);
		}
		break;
	case chienbao_tran:
		for (int i = 0; i < h->m_btSoMuc && 5 * i + 5 <= nThan; ++i)
		{
			int id = pThan[5 * i] - 51;
			if (id >= 0 && id < 16)
				memcpy(&c.nTran[id], pThan + 5 * i + 1, sizeof(int));
		}
		c.uNhanGio = GetTickCount();
		break;
	case chienbao_roi:
		memset(&c, 0, sizeof(c));
		break;
	case chienbao_mota:
		{
			int k = 0;
			k += DocChuoi(pThan + k, nThan - k, c.szTenTran, sizeof(c.szTenTran));
			k += DocChuoi(pThan + k, nThan - k, c.szMoTaCap, sizeof(c.szMoTaCap));
			k += DocChuoi(pThan + k, nThan - k, c.szPhuongThuc, sizeof(c.szPhuongThuc));
			DocChuoi(pThan + k, nThan - k, c.szMoTa, sizeof(c.szMoTa));
		}
		break;
	case chienbao_quanso:
		if (nThan >= 8)
		{
			memcpy(&c.nPhe1, pThan, sizeof(int));
			memcpy(&c.nPhe2, pThan + 4, sizeof(int));
		}
		break;
	default:
		return -1;
	}
	return h->m_btLoai;
}

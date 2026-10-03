//---------------------------------------------------------------------------
// Phep tron sprite CONG SANG - TACH RIENG de dung chung.
//
// Vi sao tach: jxstudio (macOS) dich CHINH tep nay thanh .dylib de xem sprite
// hieu ung dung y nhu trong game. Truoc 13/09/2026 no cat than ham theo moc
// trong ban va - moc truot la hong am tham. Nay ca hai ben cung MOT tep.
//
// KHONG duoc dinh KCanvas/KDrawNode/DirectDraw o day: tep nay phai dich duoc
// bang clang tren macOS. Cat va khoa mat ve van nam ben g_DrawSpriteAdd.
//
// Luong sprite (xem DrawSpriteMP.inc): tung cap [do dai][alpha]
//   alpha == 0  -> doan trong suot, KHONG co byte mau di kem
//   alpha != 0  -> <do dai> byte chi so, tra mau qua bang mau 16 bit
//---------------------------------------------------------------------------
#ifndef KTronSprite_H
#define KTronSprite_H
inline void g_TronSpriteVaoDem(
	char* pDongDau, int nPitch, int bLa565,
	const unsigned char* pNguon, const unsigned short* pBangMau,
	int nRong, int nCao,
	int nHangDau, int nHangCuoi, int nCotDau, int nCotCuoi, int nLechX)
{
	/* 0x07e0f81f la mat na cua 565, con lai la 555 */
	int nDichR = bLa565 ? 11 : 10;
	int nDichG = 5;
	int nTranR = 31;
	int nTranG = bLa565 ? 63 : 31;
	int nTranB = 31;
	int nChiaG = bLa565 ? 6 : 5;
	int nTongDiem = nRong * nCao;
	int nHang = 0, nCot = 0, nDiem = 0;
	int nVongAn = nTongDiem * 2 + 64;	/* chan vong lap vo han khi du lieu hong */
	while (nDiem < nTongDiem && nHang < nHangCuoi && nVongAn-- > 0)
	{
		int nDai = pNguon[0];
		int nAlpha = pNguon[1];
		pNguon += 2;
		/* He so ve: 32 = giu nguyen mau goc cua sprite; alpha cua DOAN chi la
		   co trong/duc chu KHONG phai trong so. Do lai engine.dll cua ban hoan
		   thien 12/09/2026: KCanvas::DrawSpriteScreen chi so alpha doan voi 0 de
		   bo qua diem. Nhan them alpha doan lam sprite nay dam sprite kia nhat -
		   dung trieu chung "chieu nay dat thi chieu kia nhat". */
		int nHeSo = nAlpha ? 32 : 0;
		for (int nI = 0; nI < nDai && nDiem < nTongDiem; nI++, nDiem++)
		{
			unsigned char byIdx = 0;
			if (nAlpha)
				byIdx = *pNguon++;
			if (nHeSo > 0 && nHang >= nHangDau && nHang < nHangCuoi &&
				nCot >= nCotDau && nCot < nCotCuoi)
			{
				unsigned short* pO = (unsigned short*)(pDongDau + (nHang - nHangDau) * nPitch)
					+ (nLechX + nCot - nCotDau);
				unsigned short wNguon = pBangMau[byIdx];
				int nSR0 = (wNguon >> nDichR) & nTranR;
				int nSG0 = (wNguon >> nDichG) & nTranG;
				int nSB0 = wNguon & nTranB;
				int nDR = (*pO >> nDichR) & nTranR;
				int nDG = (*pO >> nDichG) & nTranG;
				int nDB = *pO & nTranB;
				/* Chep dung phep cua KCanvas::DrawSpriteScreen trong engine.dll
				   ban hoan thien (0x1000ad50): ca ba kenh tinh o 5 BIT - kenh luc
				   cua 565 bi bo bit thap - roi S + D - (S*D + 31)/32, lam tron LEN.
				   Cong thuan lam loi lua chay trang, con alpha thi diem TOI cua
				   sprite keo nen xuong thanh quang den - screen khong dinh ca hai. */
				int nDR5 = nDR, nDG5 = (nChiaG == 6) ? (nDG >> 1) : nDG, nDB5 = nDB;
				int nTR = (nSR0 * nHeSo) >> 5;
				int nTG = (((nChiaG == 6) ? (nSG0 >> 1) : nSG0) * nHeSo) >> 5;
				int nTB = (nSB0 * nHeSo) >> 5;
				if (nTR > 31) nTR = 31;
				if (nTG > 31) nTG = 31;
				if (nTB > 31) nTB = 31;
				int nR = nDR5 + nTR - ((nDR5 * nTR + 31) >> 5);
				int nG = nDG5 + nTG - ((nDG5 * nTG + 31) >> 5);
				int nB = nDB5 + nTB - ((nDB5 * nTB + 31) >> 5);
				if (nChiaG == 6) nG <<= 1;
				if (nR > nTranR) nR = nTranR;
				if (nG > nTranG) nG = nTranG;
				if (nB > nTranB) nB = nTranB;
				*pO = (unsigned short)((nR << nDichR) | (nG << nDichG) | nB);
			}
			nCot++;
			if (nCot >= nRong)
			{
				nCot = 0;
				nHang++;
			}
		}
	}
}
#endif

#ifndef KMathH
#define	KMathH

#include <math.h>
#include "GameDataDef.h"

int g_InitMath();
int g_UnInitMath();

void g_InitSeries();

#ifdef __linux
#define __cdecl
#endif

//---------------------------------------------------------------------------
// 正弦表 (将浮点数 *1024 整型化)
extern int		*g_nSin;

// 余弦表 (将浮点数 *1024 整型化)
extern int		*g_nCos;

// 正弦余弦的查表函数代码缓冲区
extern unsigned char *g_InternalDirSinCosCode;

typedef int	__cdecl g_InternalDirSinCosFunction(int pSinCosTable[], int nDir, int nMaxDir);

inline int g_DirSin(int nDir, int nMaxDir)
{
    /* Ban goc goi ma may nhung lam du lieu; Linux co bit NX -> segfault.
       Chep dung ban C tuong duong dat trong comment o KMath.cpp,
       ke ca viec tra -1 khi huong ngoai khoang (ma may: 83 C8 FF). */
    if (nDir < 0 || nDir >= nMaxDir)
        return -1;
    return g_nSin[(nDir << 6) / nMaxDir];
}


inline int g_DirCos(int nDir, int nMaxDir)
{
    /* Ban goc goi ma may nhung lam du lieu; Linux co bit NX -> segfault.
       Chep dung ban C tuong duong dat trong comment o KMath.cpp,
       ke ca viec tra -1 khi huong ngoai khoang (ma may: 83 C8 FF). */
    if (nDir < 0 || nDir >= nMaxDir)
        return -1;
    return g_nCos[(nDir << 6) / nMaxDir];
}

//---------------------------------------------------------------------------
// 五行相生相克
extern int		g_nAccrueSeries[series_num];
extern int		g_nConquerSeries[series_num];

// 五行相生相克函数代码缓冲区
extern unsigned char *g_InternalIsAccrueConquerCode;

typedef int __cdecl g_InternalIsAccrueConquerFunction(int pAccrueConquerTable[], int nSrcSeries, int nDesSeries);

inline int g_IsAccrue(int nSrcSeries, int nDesSeries)
{
    if (nSrcSeries < 0 || nSrcSeries >= series_num)
        return 0;
    return g_nAccrueSeries[nSrcSeries] == nDesSeries;
}

inline int g_IsConquer(int nSrcSeries, int nDesSeries)
{
    if (nSrcSeries < 0 || nSrcSeries >= series_num)
        return 0;
    return g_nConquerSeries[nSrcSeries] == nDesSeries;
}


//---------------------------------------------------------------------------
inline int	g_GetDistance(int nX1, int nY1, int nX2, int nY2)
{
	return (int)sqrt((nX1 - nX2) * (nX1 - nX2) + (nY1 - nY2) * (nY1 - nY2));
}


inline int	g_GetDirIndex(int nX1, int nY1, int nX2, int nY2)
{
	int		nRet = -1;

	if (nX1 == nX2 && nY1 == nY2)
		return -1;

//	int		nDistance = g_GetDistance(nX1, nY1 * 2, nX2, nY2 * 2);
	int		nDistance = g_GetDistance(nX1, nY1, nX2, nY2);
	
	if (nDistance == 0 ) return -1;
	
//	int		nYLength = (nY2 - nY1) * 2;
	int		nYLength = nY2 - nY1;
	int		nSin = (nYLength << 10) / nDistance;	// 放大1024倍
	

	for (int i = 0; i < 32; i++)		// 顺时针方向 从270度到90度，sin值递减
	{
		if (nSin > g_nSin[i])
			break;
		nRet = i;
	}
	/* Lay huong GAN NHAT nhu ban6 (KSkill::CastMissles 0x8104bfb: so khoang cach toi hai moc sin ke nhau, gan moc sau
	   hon thi +1). Ban 2003 dung o moc cuoi chua vuot = lam tron XUONG, dan lech toi gan mot buoc (5.6 do) ve mot phia
	   - Vo Tuong Tram 321 do 08/10 lech 1-5.4 do, bay ne bao cat 4-21 diem nen khong cham (CollidRange 1). */
	if (nRet >= 0 && nRet < 32 && g_nSin[nRet] != nSin && (g_nSin[nRet] - nSin) > (nSin - g_nSin[nRet + 1]))
		nRet++;

	/* Nua phai doi xung qua truc doc: 64 - nRet nhu ban6 (0x8104c34), nRet 0 (thang xuong) giu 0. Ban 2003 lay
	   63 - nRet nen moi huong ben phai lech them mot buoc. */
	if ((nX2 - nX1) >= 0 && nRet > 0)
	{
		nRet = 64 - nRet;
	}
	return nRet;
}

inline	int g_Dir2DirIndex(int nDir, int nMaxDir)
{
	int nRet = -1;

	if (nMaxDir <= 0)
		return nRet;

	nRet = (nDir << 6) / nMaxDir;
	return nRet;
}

inline int	g_DirIndex2Dir(int nDir, int nMaxDir)
{
	int		nRet = -1;

	if (nMaxDir <= 0)
		return nRet;

	nRet = (nMaxDir * nDir) >> 6;	// (nMaxDir / 64) * nDir
	return nRet;
}


inline BOOL g_RandPercent(int nPercent)
{
	return ((int)g_Random(100) < nPercent);

/*	int i = g_Random(100);

	if (i >= nPercent)
		return FALSE;
	else
		return TRUE;*/
}

#endif //KMathH

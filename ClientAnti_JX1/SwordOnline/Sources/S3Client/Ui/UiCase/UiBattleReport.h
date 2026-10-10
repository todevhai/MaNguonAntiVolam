/*****************************************************************************************
//	Chien bao Tong Kim (phim `) - xem UiBattleReport.cpp.
*****************************************************************************************/
#pragma once
#include "../Elem/WndShadow.h"
#include "../../../core/src/gamedatadef.h"

class KUiBattleReport : public KWndShadow
{
public:
	static KUiBattleReport*	OpenWindow();
	static KUiBattleReport*	GetIfVisible();
	static void				CloseWindow(bool bDestroy);
	static void				LoadScheme(const char* pScheme);
	static void				OnCoreData(unsigned int uLoai);

private:
	KUiBattleReport() {}
	~KUiBattleReport() {}
	void	Initialize();
	void	PaintWindow();
	void	Chu(const char* sz, int nCot, int y, unsigned int uMau);	// nCot tinh theo so chu nua o
	void	ChuCot(const char* sz, int nCot, int nRong, int y, unsigned int uMau);	// cat theo do rong cot
	void	Vach(int y);
	static KUiBattleReport*	m_pSelf;

	unsigned int	m_uMauTieuDeMuoi, m_uMauLe, m_uMauChan, m_uMauTieuDeBan, m_uMauBan, m_uMauDau, m_uMauVach;
	int		m_nLe;
	int		m_nYDau, m_nYVach1, m_nYBan, m_nYChan, m_nYVach2, m_nYMuoi;
	int		m_nCotDau[5];		// TopTypeSet: BattleWar, Level, WinCondition, Proportion, LeftTime
	int		m_nCotBan[2];		// SelfTypeSet: ReportItem, ObjNum
	int		m_nCotTen;			// TenTypeSet PlayerName
	char	m_szTenLoai[CHIEN_BAO_LOAI][24];	// [ItemName]
};

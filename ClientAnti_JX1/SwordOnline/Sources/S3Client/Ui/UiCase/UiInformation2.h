/*****************************************************************************************
//	界面--消息窗口
//	Copyright : Kingsoft 2002
//	Author	:   Wooy(Wu yue)
//	CreateTime:	2002-8-14
------------------------------------------------------------------------------------------
*****************************************************************************************/
#pragma once

#include "../Elem/WndText.h"
#include "../Elem/WndPureTextBtn.h"
#include "../Elem/WndShowAnimate.h"

struct KUiInformationParam;

class KUiInformation2 : protected KWndShowAnimate
{
public:
	void	Initialize();							//初始化
	void	LoadScheme(const char* pScheme);		//载入界面方案
	void	Show(const char* pInformation, int nInforLen, const char* pBtnLabel,
				KWndWindow* pCallerWnd = 0, unsigned int uParam = 0);//显示窗口

	void	SpeakWords(KUiInformationParam* pWordDataList, int nCount);	//显示对话内容
	KWndWindow* TopChildFromPoint(int x, int y) {
		return this;
	}
	void	Hide();									//隐藏窗口
	void	Close();
	KUiInformation2();
	~KUiInformation2();
private:
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	PaintWindow();				/* nen ghep 3 khung: dau, than lat lai, day */
	void	DatCoTheoNoiDung();			/* doi chieu cao theo so dong loi thoai *///窗口函数
private:
	KWndText512			m_Information;	/* 256 -> 512: loi thoai 8.x dai hon 255 byte (Da Tau Task_Info 316) */
	KWndPureTextBtn		m_OKBtn;
	KWndWindow*			m_pCallerWnd;
	unsigned int		m_uCallerParam;

	KUiInformationParam*	m_pWordDataList;
	int						m_nNumWordData;
	int						m_nCurrentWord;
	/* Co gian (doc o UiInformation2.ini; NenThan=0 = kich thuoc co dinh nhu ban goc). */
	static int	ms_nNenDau, ms_nNenThan, ms_nNenDay;	/* cao khung 0/1/2 cua anh nen */
	static int	ms_nCaoToiDa, ms_nLeDuoi;			/* chieu cao hop toi da; le duoi o chu */
	static int	ms_nCaoChuToiThieu, ms_nFont;		/* cao o chu theo ini (giu can giua); co chu */
};

void UIMessageBox2(const char* pMsg, int nMsgLen = -1, const char* pBtnLabel = 0,
				   KWndWindow* pCaller = 0, unsigned int uParam = 0);

extern KUiInformation2	g_UiInformation2;
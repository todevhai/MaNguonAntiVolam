// -------------------------------------------------------------------------
//	文件名		：	交易确认框
//	创建者		：	Wooy(Wu yue)
//	创建时间	：	2003-5-28
// -------------------------------------------------------------------------
#ifndef __UiTradeConfirmXu_H__
#define __UiTradeConfirmXu_H__

#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../../../core/src/gamedatadef.h"
#include "../Elem/WndEdit.h"

enum TRADE_CONFIRM_ACTION
{
	TCA_BUY,
};

class KUiTradeConfirmXu : protected KWndImage
{
public:
	
	static KUiTradeConfirmXu*	OpenWindow(int nNumber, KUiObjAtContRegion* pObj,
			KUiItemBuySelInfo* pPriceInfo, TRADE_CONFIRM_ACTION eAction);//so luong mua ban	
	static KUiTradeConfirmXu*	GetIfVisible();				
	static void			CloseWindow(bool bDestroy);		
private:
	KUiTradeConfirmXu();
	~KUiTradeConfirmXu() {}
	int		Initialize();								
	void	Show();
	void	Hide();
	void	LoadScheme(const char* pScheme);			
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	OnCancel();
	void	OnBuy();
/*	void	OnIncrease();//so luong mua ban
	void	OnDecrease();
	void	OnCheckInput();*/
private:
	static KUiTradeConfirmXu*	m_pSelf;
	KWndText32			m_ItemName;
	KWndText32			m_Money;
	KWndEdit32			m_BuyNumber;//so luong mua ban
	KWndImage			m_BuyImg;
	KWndButton			m_OkBtn;
	KWndButton			m_CancelBtn;
	KWndButton			m_Increase;//so luong mua ban
	KWndButton			m_Decrease;
	KUiObjAtContRegion	m_ItemInfo;
	KUiItemBuySelInfo	m_PriceInfo;
	unsigned int		m_uNormalPriceColor;
	unsigned int		m_uNotEnoughMoneyPriceColor;
};


#endif // __UiTradeConfirm_H__
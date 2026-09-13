// "Mo rong ruong" - the extra pages of the storage chest.
//
// The chest room on both server and client is REPOSITORY_ROOM_PAGES pages tall: page k is
// rows k*REPOSITORY_ROOM_HEIGHT .. +HEIGHT-1 of room_repository / pos_repositoryroom. The
// chest window (KUiStoreBox) shows page 0; this window shows pages 1..EXBOX_PAGE_COUNT and
// translates between its 6x10 grid and the room rows. No new item position or packet: items
// on the extra pages are ordinary chest items with a larger y.
//
// The class keeps its old name so the existing callers (UiBase, UiShell,
// GameSpaceChangedNotify, UiExBox2/3) still compile.
#pragma once

#include "../elem/WndButton.h"
#include "../elem/WndObjContainer.h"
#include "../Elem/WndShowAnimate.h"

struct KUiObjAtRegion;
struct KUiDraggedObject;
struct ITEM_PICKDROP_PLACE;

#define EXBOX_PAGE_COUNT	5	// REPOSITORY_ROOM_PAGES - 1 (page 0 is the chest window)

class KUiExBox1 : protected KWndShowAnimate
{
public:
	static KUiExBox1*	OpenWindow();
	static KUiExBox1*	OpenWindow2() { return OpenWindow(); }	// old entry used by UiExBox2/3
	static KUiExBox1*	GetIfVisible();
	static void			CloseWindow();
	static void			LoadScheme(const char* pScheme);
	void				UpdateItem(KUiObjAtRegion* pItem, int bAdd);	// chest-room coordinates
	BOOL				DepositBagItem(KUiDraggedObject* pBagItem);	// right click in the bag
private:
	KUiExBox1() : m_nPage(1) {}
	~KUiExBox1() {}
	void	Initialize();
	int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	void	UpdateData();
	void	SelectPage(int nPage);
	int		RowOffset() const;
	void	OnItemPickDrop(ITEM_PICKDROP_PLACE* pPickPos, ITEM_PICKDROP_PLACE* pDropPos);
	BOOL	WithdrawBoxItem(KUiDraggedObject* pBoxItem);
private:
	static KUiExBox1*	m_pSelf;
	KWndButton			m_CloseBtn;
	KWndObjectMatrix	m_ItemBox;
	KWndButton			m_PageBtn[EXBOX_PAGE_COUNT];
	int					m_nPage;	// 1..EXBOX_PAGE_COUNT
};

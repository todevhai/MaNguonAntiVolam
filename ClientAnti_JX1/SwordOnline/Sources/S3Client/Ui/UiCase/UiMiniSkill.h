/*****************************************************************************************
//	Thanh mini skill: hang icon cac trang thai (buff/debuff/vong sang) dang tac dung len
//	nhan vat, duoi moi icon la thoi gian con lai.
------------------------------------------------------------------------------------------
	Du lieu hien thi (ten, anh, mo ta, co debuff/vong sang) doc tu \Ui\MiniSkill.ini - tep
	co san trong bo client Viet hoa nhung nguon 2003 khong co ma nao doc. Vi tri, co icon va
	mau chu ([txtBuffTime] / [txtDebuffTime] / [txtWarningTime]) cung lay tu tep do.
	Danh sach trang thai va so nhip con lai lay tu Core (GDI_PLAYER_STATE_SKILLS): may chu gui
	thoi gian khi tac dung (STATE_EFFECT_SYNC), client tu dem lui moi nhip.
	Chi hien trang thai CO khai trong ini: do la cac chieu tay phai tac len nguoi choi.
*****************************************************************************************/
#pragma once

#include "../Elem/WndButton.h"
#include "../Elem/WndText.h"
#include "../../../core/src/coreshell.h"

#define	MINI_SKILL_MAX_BUFF		256		// so muc toi da trong [BuffList] cua MiniSkill.ini

struct KMiniSkillBuff
{
	int		nSkillId;
	int		bDebuff;
	int		bAura;
	char	szName[32];
	char	szImage[128];
	char	szDesc[128];
};

class KUiMiniSkill : protected KWndWindow
{
public:
	static KUiMiniSkill*	OpenWindow();
	static void				CloseWindow(bool bDestroy);

private:
	KUiMiniSkill();
	~KUiMiniSkill() {}
	void	Initialize();
	void	LoadScheme();
	virtual void	Breathe();
	virtual int		WndProc(unsigned int uMsg, unsigned int uParam, int nParam);
	const KMiniSkillBuff*	FindBuff(int nSkillId) const;
	void	UpdateSlot(int nSlot, const KUiStateSkill& State, const KMiniSkillBuff& Buff);

private:
	static KUiMiniSkill*	m_pSelf;

	KWndButton		m_Icon[UI_MAX_STATE_SKILL];
	KWndText32		m_Time[UI_MAX_STATE_SKILL];
	int				m_nSlotSkill[UI_MAX_STATE_SKILL];	// chieu dang ve o o nay (doi thi nap lai anh/tooltip)
	int				m_nSlotSecond[UI_MAX_STATE_SKILL];	// giay dang ghi o o nay (doi thi ghi lai chu)
	int				m_nShown;							// so o dang hien

	int				m_nIconWidth;
	int				m_nIconHeight;
	int				m_nTimeHeight;
	unsigned int	m_uBuffColor;
	unsigned int	m_uDebuffColor;
	unsigned int	m_uWarningColor;

	KMiniSkillBuff	m_Buff[MINI_SKILL_MAX_BUFF];
	int				m_nBuffCount;
};
